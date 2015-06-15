#ifndef _JSON_CLASS_
#define _JSON_CLASS_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <iostream>

USING_NS_CC;
USING_NS_CC_EXT;

typedef enum{
	TYPE_CTX = 1,
	TYPE_REQ
}WRITETYPE;

/**********************************************
*Json数据包构造
**********************************************/
class JsonWritter{
public:
	JsonWritter();
	~JsonWritter();

	void writeValue(WRITETYPE writeType, char *key, char *value);
	const char* getData();

private:
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType allocator;
	rapidjson::Value ctx;
	rapidjson::Value req;
};

#endif