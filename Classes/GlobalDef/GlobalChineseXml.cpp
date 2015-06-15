#include "GlobalChineseXml.h"

static GlobalChineseXml* globalChineseXml = nullptr;

GlobalChineseXml::GlobalChineseXml(){
	
}

GlobalChineseXml* GlobalChineseXml::getInstance(){
	if(globalChineseXml == NULL){
		globalChineseXml = new GlobalChineseXml();
	}
	return globalChineseXml;
}

std::map<std::string, std::string> GlobalChineseXml::getChinessMap(){
	return this->chineseMap;
}

char* GlobalChineseXml::getValue(char* keyName){
	return (char*)this->chineseMap.find(keyName)->second.c_str();
}

void GlobalChineseXml::parseFile(char* fileName){
	if(fileName == NULL){
		return;
	}
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse(FileUtils::getInstance()->getStringFromFile(fileName).c_str());

	auto root = doc->RootElement();
	for(auto e = root->FirstAttribute(); e; e = e->Next()){
		const char* name = e->Name();
		const char* value = e->Value();
		this->chineseMap[name] = value;
	}
    delete doc;
}