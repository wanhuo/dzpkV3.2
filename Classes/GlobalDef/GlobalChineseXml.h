#ifndef _GLOBAL_CHINESE_XML_
#define _GLOBAL_CHINESE_XML_

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
#include <iostream>

USING_NS_CC;

class GlobalChineseXml{
public:
	static GlobalChineseXml* getInstance();
	std::map<std::string, std::string> getChinessMap();
	void parseFile(char* fileName);
	char* getValue(char* keyName);

private:
	GlobalChineseXml();

private:
	std::map<std::string, std::string> chineseMap;
};

#endif