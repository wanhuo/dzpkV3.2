#ifndef _NET_DELEGATE_
#define _NET_DELEGATE_

#include "stream.h"

//����������
typedef CStream STREAM;

typedef struct tagPacketHead //4 2 4 4 4 4 2 1 1 1 1 8  
{
	unsigned int nMark;				//ʶ��
	unsigned short nLen;			//��Ϣ����
	unsigned int nMsgID;			//��ϢID
	unsigned int nOrder;			//�����
	unsigned int nSourceID;			//�û�ID
	unsigned int nAimID;			//Ŀ��ID
	short nAck;						//ִ�н��
	unsigned char szPackType;		//������ 0 Ϊһ�������� 1Ϊ��ʼ�� 2Ϊ�м�� 3Ϊ������
	unsigned char szPackTotal;		//�ܹ��ж��ٸ���
	unsigned char szPackOrder;		//��˳��
	unsigned char szType;			//����
	char szReserve[8];				//����
}PACKET_HEAD_T;

/***************************************************
*�������ӿ�
***************************************************/
class NetDelegate{
public:
	NetDelegate(){}
	virtual ~NetDelegate(){}

public:
	///�������ɹ� ��ʼ����ʱ����
	virtual void onCreate(){}
	///�����ӳɹ�ʱ����
	virtual void onConnected(){}
	///���Ͽ�����ʱ����
	virtual void onDisconnected(){}
	///�����Ӵ���ʱ����
	virtual void onConnectError(){}
	///�����ӳ�ʱʱ����
	virtual void onConnectTimeout(){}
	///��������Ϣʱ����
	virtual void onMessage(char *buff, int len){}
};

#endif