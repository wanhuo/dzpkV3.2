#ifndef _MESSAGE_DATA_
#define _MESSAGE_DATA_

#include <iostream>

/**********************
*���ݰ���
**********************/

class MessageData{
public:
	MessageData();
	~MessageData();
	char* getData();
	int getLength();
	void addChars(char* data);

private:
	std::string messageData;  //������
};

#endif