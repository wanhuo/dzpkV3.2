/******************************************************************************
文件名: CNetBean.h
编写者: csdn_viva@foxmail.com ccnet1.0
编写日期: 2012-5-27
功能描述: 客户端网络对象，一个网络对象对应一个SOCKET
******************************************************************************/
#ifndef __CCNET_NETBEAN_H__
#define __CCNET_NETBEAN_H__

#include "socket.h"
#include "stream.h"
#include "NetDelegate.h"

//连接超时时间(秒)
#define SOCK_CONNECT_TIMEOUT 10
//接收器缓冲区大小
#define SOCK_RECVBUFFERSIZE 16384
//接收数据是否开启封包组包算法
#define CLIP_PACKET 0

///创建单例方法
#define DEFINE_SINGLE_FUNCTION(__TYPE__) \
static __TYPE__* getInstance() { \
static __TYPE__ *pInstance = NULL; \
if(pInstance == NULL){ \
	pInstance = new __TYPE__(); \
} \
return pInstance; \
}

/******************************************************************************
类    名: CNetBean
功能描述: 客户端网络对象，一个网络对象对应一个SOCKET
******************************************************************************/
class NetBean
{
public:
	DEFINE_SINGLE_FUNCTION(NetBean);

protected:
	///构造函数，初始化默认值
	NetBean();
	///析构函数，释放所有资源
	virtual ~NetBean();

public:
	//设置与服务器连接的地址与端口
	void setAddress(const char* ip, unsigned short port);

	//给发送流添加qq头信息
	void addQQHead(STREAM *stream);

	//给发送流添加消息头信息
	void addHead(STREAM *stream, unsigned short len, unsigned int msgId);

	//给发送流添加消息体
	void addData(STREAM *stream, char *data, long len);

public:
	///开始无阻塞方式连接
	virtual bool connect();
	///是否处于连接状态
	virtual bool isConnected();
	///关闭连接
	virtual void close();
	///帧循环，读写数据
	virtual void drive();
	///往服务端写数据，无阻塞
	virtual void write(STREAM stream);
	///释放本网络对象
	virtual void release();

public:
	NetDelegate *netDelegate;
	void setNetDelegate(NetDelegate *delegate);
	NetDelegate* getNetDelegate();

private:
	///读数据
	void read(char* buff, int len);

protected:
	///枚举连接状态
	enum {
		ENULL			= 1,	//无状态
		EConnecting		= 2,	//正在进行连接
		EConnected		= 3,	//连接成功
		EConnectTimeout = 4,	//连接超时
		EConnectError	= 5,	//连接异常
		EDisconnected	= 6		//连接中断
	} 
	m_nConnectStatus;

	///超时时间记录
	time_t m_ccConnectTime;

protected:
	///接收缓冲器
	char m_RecvBuffer[SOCK_RECVBUFFERSIZE];

#if CLIP_PACKET
	///数据缓冲区
	deque<char> m_vFrameDecodeBuffer;
	///本次数据包的长度
	int m_nRecvPackLength;
#endif

protected:
	///服务器IP地址
	string m_nnAddress;
	///服务器端口号
	unsigned short m_nnPort;

protected:
	///IO操作
	CSocket	m_Sock;

	std::string recvBuf;
};

#endif //__CCNET_NETBEAN_H__