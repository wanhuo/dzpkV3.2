#ifndef _ASYNC_IMAGE_
#define _ASYNC_IMAGE_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "../GlobalDef/table/MallInfoTable.h"
#include "../GlobalDef/Data/MallInfoData.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../NetWork/netbean.h"

#define DEFAULT_GOODS_URL "http://1251038220.cdn.myqcloud.com/1251038220/mobile/"

using namespace network;
USING_NS_CC;
USING_NS_CC_EXT;

class AsyncImageLayer;
class AsyncImage {
public:
	static AsyncImage* getInstance();
	static void destoryInstance();
	
	void getImage(const std::string& strURL);
	Map<std::string, AsyncImageLayer*>& getData() { return m_data; }

private:
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

private:
	Map<std::string, AsyncImageLayer*> m_data;
};

#endif