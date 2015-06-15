#include "MessageData.h"

MessageData::MessageData(){
	
}

MessageData::~MessageData(){
	this->messageData.clear();
}

char* MessageData::getData(){
	return (char *)this->messageData.c_str();
}

int MessageData::getLength(){
	return this->messageData.size() + 1;
}

void MessageData::addChars(char* data){
	this->messageData.append(data);
}