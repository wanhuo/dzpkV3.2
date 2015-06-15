#ifndef _MESSAGE_DATA_
#define _MESSAGE_DATA_

#include <iostream>

/**********************
*数据包类
**********************/

class MessageData{
public:
	MessageData();
	~MessageData();
	char* getData();
	int getLength();
	void addChars(char* data);

private:
	std::string messageData;  //包数据
};

#endif