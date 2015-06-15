/******************************************************************************
文件名: CNetBean.cpp
编写者: csdn_viva@foxmail.com ccnet1.0
编写日期: 2012-5-27
功能描述: 客户端网络对象，一个网络对象对应一个SOCKET
******************************************************************************/
#include "netbean.h"

///构造函数，初始化默认值
NetBean::NetBean():
m_nnPort(0),
m_nConnectStatus(ENULL)
{
#if CLIP_PACKET
	m_nRecvPackLength = 0;
#endif
}

///析构函数，释放所有资源
NetBean::~NetBean(){}


///设置与服务器连接的地址与端口
void NetBean::setAddress(const char* ip, unsigned short port)
{
	this->m_nnPort = port;
	this->m_nnAddress = ip;
}

//给发送流添加qq头信息
void NetBean::addQQHead(STREAM *stream){
	char* head = (char *)"tgw_l7_forward\r\nHost:app1101073617.qzone.qzoneapp.com:8006\r\n\r\n";
	stream->writeData(head, strlen(head));
}

//给发送流添加消息头信息
void NetBean::addHead(STREAM *stream, unsigned short len, unsigned int msgId){
	char* strPacketHead = new char[36];
	PACKET_HEAD_T packetHead = { 0 };
	packetHead.nMark = htonl(0xA35CD932);
	packetHead.nLen = htons(len);
	packetHead.nMsgID = htonl(msgId);
	packetHead.nOrder = htonl(3);
	packetHead.nSourceID = htonl(4);
	packetHead.nAimID = htonl(5);
	packetHead.nAck = htons(1);
	packetHead.szPackType = 10;
	packetHead.szPackTotal = 10;
	packetHead.szPackOrder = 10;
	packetHead.szType = 10;
	packetHead.szReserve[0] = 'l';
	packetHead.szReserve[1] = 'a';
	packetHead.szReserve[2] = 'm';
	packetHead.szReserve[3] = 'b';
	packetHead.szReserve[4] = 'e';
	packetHead.szReserve[5] = 'r';
	packetHead.szReserve[6] = 't';
	packetHead.szReserve[7] = 't';

	memcpy(strPacketHead, &packetHead.nMark, 4);
	memcpy(strPacketHead + 4, &packetHead.nLen, 2);
	memcpy(strPacketHead + 6, &packetHead.nMsgID, 4);
	memcpy(strPacketHead + 10, &packetHead.nOrder, 4);
	memcpy(strPacketHead + 14, &packetHead.nSourceID, 4);
	memcpy(strPacketHead + 18, &packetHead.nAimID, 4);
	memcpy(strPacketHead + 22, &packetHead.nAck, 2);
	memcpy(strPacketHead + 24, &packetHead.szPackType, 1);
	memcpy(strPacketHead + 25, &packetHead.szPackTotal, 1);
	memcpy(strPacketHead + 26, &packetHead.szPackOrder, 1);
	memcpy(strPacketHead + 27, &packetHead.szType, 1);
	memcpy(strPacketHead + 28, &packetHead.szReserve, 8);

	stream->writeData(strPacketHead, 36);
    delete strPacketHead;
}

//给发送流添加消息体
void NetBean::addData(STREAM *stream, char *data, long len){
	stream->writeData(data, len);
	stream->writeData("\n", 1);
}

///开始无阻塞方式连接
bool NetBean::connect()
{
	//there is connected or connecting 
	if(this->m_nConnectStatus == EConnected || this->m_nConnectStatus == EConnecting){
		return false;
	}
	//validate value
	if(this->m_nnAddress == "" || this->m_nnPort == 0) {
		return false;
	}
	if(!m_Sock.Create()) {
		return false;
	}
	if(!m_Sock.Connect(this->m_nnAddress.c_str(), this->m_nnPort)) {
		return false;
	}
	//set the connecting status
	this->m_nConnectStatus = EConnecting;
	//get the connect time of started.
	m_ccConnectTime = time(NULL);
	//call back to virtual function 
	this->netDelegate->onCreate();

	return true;
}

///是否处于连接状态
bool NetBean::isConnected()
{
	if(this->m_nConnectStatus == EConnected)
	{
		return true;
	}
	return false;
}

///关闭连接
void NetBean::close()
{
    log("NetBean close");
	this->m_Sock.Close();
	this->m_nConnectStatus = EDisconnected;
}

///释放本网络对象
void NetBean::release()
{
    log("NetBean release");
	this->close();
	delete this;
}

///往服务端写数据，无阻塞
void NetBean::write(STREAM stream)
{
	//check status
	if( this->m_nConnectStatus != EConnected ) {
		return;
	}
	//check io is alive
	if( m_Sock.IsWritable() ) {
		//pack length
		int length = stream.size();
#if CLIP_PACKET
		char* data = stream.flush();
		char* buff = new char[ length + 4 ];
		if( length == 0 )
			return;
		//write length of pack to pack header
		buff[0] = ((0xff000000 & length) >> 24);
		buff[1] = ((0xff0000 & length) >> 16);
		buff[2] = ((0xff00 & length) >> 8);
		buff[3] = ((0xff & length));
		//write data
		for(int idx = 0; idx < length; idx++) {
			buff[idx+4] = data[idx];
		}
		length += 4;
#else
		char* buff = stream.flush();
#endif
		if( length != 0 ) {
			int nLen = m_Sock.Write( buff, length );
			if( nLen == SOCKET_ERROR ) {
				//set the connecting status
				this->m_nConnectStatus = EDisconnected;
				//call back to virtual function
				this->netDelegate->onDisconnected();
				//release socket
				this->close();
				//log
				log("## [DEBUG] Write Disconnected if(nLen == SOCKET_ERROR)");
			}
		}
#if CLIP_PACKET
		delete buff;
#endif
	} else {
		//log
		log("## [DEBUG] Write Disconnected if( m_Sock.IsWritable() )");
	}
}

///读数据
void NetBean::read(char* buff, int len)
{
#if CLIP_PACKET
	//add data to buffer
	for(int idx = 0; idx < len; idx++ ) {
		this->m_vFrameDecodeBuffer.push_back(m_RecvBuffer[idx]);
	}
	//check pack recv
	if(this->m_nRecvPackLength == 0) {
		//read length of pack head
		if(this->m_vFrameDecodeBuffer.size() >= 4) {
			//read 4 char to int
			this->m_nRecvPackLength = 0;
			for(int i = 0; i < 4; i++) {
				this->m_nRecvPackLength <<= 8;
				this->m_nRecvPackLength |= (this->m_vFrameDecodeBuffer[i] & 0xff);
			}
		} else {
			//there is no 4 bytes in buffer
			return;
		}
	}
	int readable_size = this->m_vFrameDecodeBuffer.size() - 4;
	//if readable
	if(readable_size >= this->m_nRecvPackLength) {
		//to stream buffer
		char* buffer = new char[this->m_nRecvPackLength];
		//to delelte 4 char of pack length
		this->m_vFrameDecodeBuffer.pop_front();
		this->m_vFrameDecodeBuffer.pop_front();
		this->m_vFrameDecodeBuffer.pop_front();
		this->m_vFrameDecodeBuffer.pop_front();
		//read data from buffer
		for(int idx = 0; idx < m_nRecvPackLength; idx++) {
			buffer[idx] = this->m_vFrameDecodeBuffer.front();
			this->m_vFrameDecodeBuffer.pop_front();
		}
		//make stream to logic
		CStream stream(buffer, this->m_nRecvPackLength);
		//release temp buffer;
		delete buffer;
		//pack read successed.
		this->m_nRecvPackLength = 0;
		//call back to virtual function 
		this->netDelegate->onMessage(stream);
	}
#else
	//call back to virtual function 
	this->netDelegate->onMessage(buff, len);
#endif

}

///帧循环，读写数据
void NetBean::drive()
{   
	//connect successed
	if(this->m_nConnectStatus == EConnected){
		//Read Buffer
		while(m_Sock.IsReadable())
		{
			int nLen = m_Sock.Read(m_RecvBuffer, SOCK_RECVBUFFERSIZE);
			//check error
			if (nLen == 0 || nLen  == SOCKET_ERROR) {
				//change connecting status
				this->m_nConnectStatus = EDisconnected;
				//call back to virtual function
				this->netDelegate->onDisconnected();
				//release socket
				this->close();
				//stop while and loop;
				break;
			} else {
				//read data
				recvBuf.append(m_RecvBuffer, nLen);
				long lineDataEnPosIndex = 0;
				while ( lineDataEnPosIndex != -1)
				{
					lineDataEnPosIndex = recvBuf.find('\n');
					if (lineDataEnPosIndex == -1)
					{
						break;
					}
					else
					{
						std::string subData = recvBuf.substr(0, lineDataEnPosIndex);
						recvBuf = recvBuf.substr(lineDataEnPosIndex + 1);
						this->read((char *)subData.c_str(), (int)subData.size());
                        subData.clear();
					}
				}	
			}
		}
	}
	//sock was keeping connecting status
	else if (this->m_nConnectStatus == EConnecting) {
		//check connected status of unblock socket
		int nRet = this->m_Sock.IsConnected();	
		if (nRet == 1) {
			//set the connecting status
			this->m_nConnectStatus = EConnected;
			//call back to virtual function
			this->netDelegate->onConnected();
		} else if (nRet < 0) {	//error
			//set the connecting status
			this->m_nConnectStatus = EConnectError;
			//call back to virtual function
			this->netDelegate->onConnectError();
			//close socket
			this->close();
		} else {
			//if it didn't finish the work at time
			time_t ccTimeNow;
			ccTimeNow = time(NULL);
			if(ccTimeNow - m_ccConnectTime >= SOCK_CONNECT_TIMEOUT){
				//set the connecting status
				this->m_nConnectStatus = EConnectTimeout;
				//call back to virtual function
				this->netDelegate->onConnectTimeout();
				//release socket
				this->close();
			}
		}
	}
}

void NetBean::setNetDelegate(NetDelegate *delegate){
	this->netDelegate = delegate;
}

NetDelegate* NetBean::getNetDelegate(){
	return this->netDelegate;
}