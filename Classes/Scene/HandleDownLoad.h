#ifndef _HandleDownLoad_
#define _HandleDownLoad_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "../GlobalDef/table/MallInfoTable.h"
#include "../GlobalDef/Data/MallInfoData.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../NetWork/netbean.h"

#define DEFAULT_GOODS_URL		"http://1251038220.cdn.myqcloud.com/1251038220/asset/"
using namespace network;
USING_NS_CC;
USING_NS_CC_EXT;

class HandleDownLoadView;
class HandleDownLoad {
public:
	static HandleDownLoad* getInstance();
	static void destoryInstance();
	
	void getImage(const std::string& strURL);
	Map<std::string, HandleDownLoadView*>& getData() { return m_data; }

private:
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

private:
	Map<std::string, HandleDownLoadView*> m_data;
};

#endif