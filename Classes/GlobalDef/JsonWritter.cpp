#include "JsonWritter.h"

JsonWritter::JsonWritter(){
	doc.SetObject();
	allocator = doc.GetAllocator();
}

JsonWritter::~JsonWritter(){
	
}

void JsonWritter::writeValue(WRITETYPE writeType, char *key, char *name){
	if(writeType == TYPE_CTX){
		ctx.AddMember(key, name, allocator);
	}
	else if(writeType == TYPE_REQ){
		req.AddMember(key, name, allocator);
	}
}

const char* JsonWritter::getData(){
	doc.AddMember("ctx", ctx, allocator);
	doc.AddMember("req", req, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	return buffer.GetString();
}