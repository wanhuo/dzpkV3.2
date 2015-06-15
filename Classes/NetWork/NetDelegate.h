#ifndef _NET_DELEGATE_
#define _NET_DELEGATE_

#include "stream.h"

//定义流类型
typedef CStream STREAM;

typedef struct tagPacketHead //4 2 4 4 4 4 2 1 1 1 1 8  
{
	unsigned int nMark;				//识别
	unsigned short nLen;			//消息长度
	unsigned int nMsgID;			//消息ID
	unsigned int nOrder;			//包序号
	unsigned int nSourceID;			//用户ID
	unsigned int nAimID;			//目标ID
	short nAck;						//执行结果
	unsigned char szPackType;		//包类型 0 为一个完整包 1为开始包 2为中间包 3为结束包
	unsigned char szPackTotal;		//总共有多少个包
	unsigned char szPackOrder;		//包顺序
	unsigned char szType;			//类型
	char szReserve[8];				//保留
}PACKET_HEAD_T;

/***************************************************
*网络代理接口
***************************************************/
class NetDelegate{
public:
	NetDelegate(){}
	virtual ~NetDelegate(){}

public:
	///当创建成功 开始连接时调用
	virtual void onCreate(){}
	///当连接成功时调用
	virtual void onConnected(){}
	///当断开连接时调用
	virtual void onDisconnected(){}
	///当连接错误时调用
	virtual void onConnectError(){}
	///当连接超时时调用
	virtual void onConnectTimeout(){}
	///当读到消息时调用
	virtual void onMessage(char *buff, int len){}
};

#endif