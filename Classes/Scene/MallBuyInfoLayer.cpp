#include "MallBuyInfoLayer.h"
#include "cocos2d.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../EncryptAndDecrypt/Md5/md5.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "../GlobalDef/Data/MallBuyData.h"


MallBuyInfoLayer* MallBuyInfoLayer::create(MALL_BUY_INFO_FLAG nFlag, const std::string& value1, const std::string& value2)
{
	MallBuyInfoLayer* pRet = new MallBuyInfoLayer();
	if (pRet)
	{
		pRet->m_nFlag = nFlag;
		pRet->m_value1 = value1;
		pRet->m_value2 = value2;
		pRet->init();
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool MallBuyInfoLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_IAP = new IOSiAP();
    m_IAP->delegate = this;
#endif
    visibleSize = Director::getInstance()->getVisibleSize();
	action_duration = 2.0;

	Sprite* sp_bg = nullptr;
	if (m_nFlag == MALL_BUY_INFO_CHIP)
	{
		sp_bg = Sprite::create("mall_buy_chip_bg.png");
	}
	else
	{
		sp_bg = Sprite::create("mall_buy_others_bg.png");
	}
	sp_bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(sp_bg);

	auto label1 = Label::createWithSystemFont(m_value1, "Arial", 20);
	label1->setAnchorPoint(Vec2(0, 0));
	label1->setDimensions(400, 50);
	label1->setHorizontalAlignment(TextHAlignment::LEFT);
	label1->setPosition(visibleSize.width / 2 - 200, visibleSize.height / 2 + 5);
	this->addChild(label1);

	auto label2 = Label::createWithSystemFont(m_value2, "Arial", 20);
	label2->setAnchorPoint(Vec2(0, 0));
	label2->setDimensions(400, 50);
	if (MALL_BUY_INFO_CHIP == m_nFlag)
	{
		label2->setHorizontalAlignment(TextHAlignment::LEFT);
		label2->setPosition(visibleSize.width / 2 - 200, visibleSize.height / 2 - 45);
	}
	else
	{
		label2->setColor(Color3B::GREEN);
		label2->setSystemFontSize(30);
		label2->setHorizontalAlignment(TextHAlignment::CENTER);
		label2->setPosition(visibleSize.width / 2 - 200, visibleSize.height / 2 - 50);
	}
	this->addChild(label2);

	MenuItemImage *btn_buy_ok_menuitem = MenuItemImage::create("btn_buy_ok_up.png", "btn_buy_ok_down.png", CC_CALLBACK_1(MallBuyInfoLayer::btnBuyOKMenuCallback, this));
	btn_buy_ok_menuitem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);

	MenuItemImage *btn_buy_close_menuitem = MenuItemImage::create("btn_buy_close_up.png", "btn_buy_close_down.png", CC_CALLBACK_1(MallBuyInfoLayer::btnBuyCloseMenuCallback, this));
	btn_buy_close_menuitem->setPosition(visibleSize.width / 2 + 248, visibleSize.height / 2 + 143);

	auto btn_menu = Menu::create(btn_buy_ok_menuitem, btn_buy_close_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	this->addChild(btn_menu);

	return true;
}

MallBuyInfoLayer::~MallBuyInfoLayer(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    delete m_IAP;
#endif
}

void MallBuyInfoLayer::btnBuyOKMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    if (!pData) {
        return;
    }
    
    loadLayer = LoadLayer::create();
    this->addChild(loadLayer);
    
    if (m_nFlag == MALL_BUY_INFO_CHIP) {
        buyChip();
    } else {
        buyProp();
    }
}

void MallBuyInfoLayer::btnBuyCloseMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}

void MallBuyInfoLayer::buyChip()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // first request apple store products
    std::vector<std::string> products;
    products.push_back(pData->getmallID());
    m_IAP->requestProducts(products);
#else
    loadLayer->removeFromParent();
    loadLayer = nullptr;
#endif
}

void MallBuyInfoLayer::buyProp()
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("chan", "0", alloc);
	ctx.AddMember("mver", "1", alloc);
    auto strUserId = UserInfo::getInstance()->getUserId();
	ctx.AddMember("user_id", strUserId.c_str(), alloc);
    auto strSessionId = UserInfo::getInstance()->getSessionId();
	ctx.AddMember("sessionid", strSessionId.c_str(), alloc);
	ctx.AddMember("ter", "WEB", alloc);

	srand(time(NULL));
	__String* serial = __String::createWithFormat("%d", (rand() + 10000) % 100000);
	ctx.AddMember("serial", serial->getCString(), alloc);

	rapidjson::Value req(rapidjson::kObjectType);
	req.AddMember("t", "buyprops", alloc);
	req.AddMember("num", "1", alloc);
	__String* str_buyid = __String::createWithFormat("%s", pData->getmallID().c_str());
	req.AddMember("buyid", str_buyid->getCString(), alloc);

	doc.AddMember("ctx", ctx, alloc);
	doc.AddMember("req", req, alloc);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char *jsonData = buffer.GetString();

	//base64加密
	const std::string encryptData = gloox::Base64::encode64(jsonData);

	HttpRequest* request = new HttpRequest();
	auto httpServerAddress = GlobalDef::getInstance()->getHttpServerAddress();
    request->setUrl(httpServerAddress.c_str());
//  request->setUrl("http://203.195.138.89/hallroom/jsonServlet");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(MallBuyInfoLayer::onHttpRequestCompleted, this));

	// write the post data
	request->setRequestData(encryptData.c_str(), encryptData.size()); 
	request->setTag("POST Binary test");
	HttpClient::getInstance()->send(request);
	request->release();
}

void MallBuyInfoLayer::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	if (loadLayer){
        loadLayer->removeFromParent();
        loadLayer = nullptr;
    }

	if (!response)
	{
		showResultTips(false, false);
		return;
	}

	// You can get original request type from: response->request->reqType
	if (!response->isSucceed()) 
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		showResultTips(false, false);
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string encodeRecvData; //接收到的base64加密数据
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		printf("%c", (*buffer)[i]);
		encodeRecvData.push_back((*buffer)[i]);
	}
	const std::string recvData = gloox::Base64::decode64(encodeRecvData); //解密后的数据
	MallBuyData data = MallBuyData::parseData(recvData);
	std::string strSuc = std::string(GlobalChineseXml::getInstance()->getValue("mall_buy_success"));
	if (data.getResultString().compare(strSuc) == 0)
	{
		long chips = atol(UserInfo::getInstance()->getChipAccount().c_str()) + atol(data.getUseChip().c_str());
        auto strChips = __String::createWithFormat("%ld", chips);
		UserInfo::getInstance()->setChipAccount(strChips->getCString());
		showResultTips(false, true);
	}
}

void MallBuyInfoLayer::showResultTips(bool buyChip, bool bSuccess)
{
	Vector<Node*> nodes = this->getChildren();
	for (int i=0; i<nodes.size(); i++)
	{
		Node* pNode = nodes.at(i);
		pNode->setVisible(false);
	}

	Sprite* sp_tips = Sprite::create("mall_buy_result_bg.png");;
	sp_tips->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(sp_tips);

	std::string str;
	if(buyChip == true){
        if (bSuccess)
        {
            str = GlobalChineseXml::getInstance()->getValue("chip_buy_success_msg");
        }
        else
        {
            str = GlobalChineseXml::getInstance()->getValue("chip_buy_failure_msg");
        }
    }
    else{
        if (bSuccess)
        {
            str = GlobalChineseXml::getInstance()->getValue("chop_buy_success_msg");
        }
        else
        {
            str = GlobalChineseXml::getInstance()->getValue("chop_buy_failure_msg");
        }
    }
    
	auto label = Label::createWithSystemFont(str, "Arial", 20);
	label->setPosition(401.5, 26);
	sp_tips->addChild(label);

	CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(MallBuyInfoLayer::endAction, this));

	FadeOut* fade_action = FadeOut::create(action_duration);
	Sequence* fade = Sequence::create(fade_action, func, NULL);
	sp_tips->runAction(fade);
}

void MallBuyInfoLayer::endAction(Node* node)
{
	this->removeFromParent();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void MallBuyInfoLayer::onRequestProductsFinish()
{
    std::string identifier = pData->getmallID();
    IOSProduct* product = m_IAP->iOSProductByIdentifier(identifier);
    if (product)
    {
        m_IAP->paymentWithProduct(product);
    }
    else{
        if (loadLayer){
            loadLayer->removeFromParent();
            loadLayer = nullptr;
            showResultTips(true, false);
        }
    }
}

void MallBuyInfoLayer::onRequestProductsError(int code)
{
    if (loadLayer) {
        loadLayer->removeFromParent();
        loadLayer = nullptr;
        showResultTips(true, false);
    }
}

void MallBuyInfoLayer::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity, const char *bytes)
{
    log("on payment:%d", event);
    switch (event) {
        case IOSIAP_PAYMENT_PURCHASING:
            break;
        case IOSIAP_PAYMENT_PURCHAED:               //gou mai cheng gong, fa song xiao xi gei fu qu qi yan zheng
            {
                log("IOSIAP_PAYMENT_PURCHAED");
                
                //json数据构造
                rapidjson::Document doc;
                doc.SetObject();
                rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
                rapidjson::Value ctx(rapidjson::kObjectType);
                ctx.AddMember("chan", "ios", alloc);
                ctx.AddMember("mver", "1", alloc);
                auto strUserId = UserInfo::getInstance()->getUserId();
                auto strSessionId = UserInfo::getInstance()->getSessionId();
                ctx.AddMember("user_id", strUserId.c_str(), alloc);
                ctx.AddMember("sessionid", strSessionId.c_str(), alloc);
                ctx.AddMember("ter", "WEB", alloc);
                srand((int)time(NULL));
                auto serial = __String::createWithFormat("%d", rand() % 100000);
                ctx.AddMember("serial", serial->getCString(), alloc);
            
                rapidjson::Value req(rapidjson::kObjectType);
                req.AddMember("t", "iosbuy", alloc);
                req.AddMember("sandbox", "0", alloc);              //0:ce shi, 1:zheng shi
                req.AddMember("receipt", bytes, alloc);
            
                doc.AddMember("ctx", ctx, alloc);
                doc.AddMember("req", req, alloc);
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                doc.Accept(writer);
            
                const char *jsonData = buffer.GetString();
            
                //base64加密
                const std::string encryptData = gloox::Base64::encode64(jsonData);
            
                HttpRequest* request = new HttpRequest();
                auto httpServerAddress = GlobalDef::getInstance()->getHttpServerAddress();
                request->setUrl(httpServerAddress.c_str());
//                request->setUrl("http://192.168.1.120:80/hallroom/jsonServlet");
                request->setRequestType(HttpRequest::Type::POST);
                request->setResponseCallback(CC_CALLBACK_2(MallBuyInfoLayer::onPaymentPurchaedHttpRequestCompleted, this));
            
                // write the post data
                request->setRequestData(encryptData.c_str(), encryptData.size());
                request->setTag("POST Binary test");
                HttpClient::getInstance()->send(request);
                request->release();
            }
            break;
        case IOSIAP_PAYMENT_FAILED:
            if (loadLayer) {
                loadLayer->removeFromParent();
                loadLayer = nullptr;
                showResultTips(true, false);
            }
            break;
        case IOSIAP_PAYMENT_RESTORED:
            break;
        case IOSIAP_PAYMENT_REMOVED:
            break;
        default:
            break;
    }
}

void MallBuyInfoLayer::onPaymentPurchaedHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
    //fu wu qi yan zheng fan hui jie guo
	if (!response)
	{
		showResultTips(true, false);
		return;
	}
    
	// You can get original request type from: response->request->reqType
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		showResultTips(true, false);
		return;
	}
    
	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string encodeRecvData; //接收到的base64加密数据
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		printf("%c", (*buffer)[i]);
		encodeRecvData.push_back((*buffer)[i]);
	}
	const std::string recvData = gloox::Base64::decode64(encodeRecvData); //解密后的数据
    
    //json数据解析
	rapidjson::Document docOnPaymentRecv;
	docOnPaymentRecv.Parse<0>(recvData.c_str());
    
    //jie xi fu wu qi fan hui shu ju
    int result = docOnPaymentRecv["resp"]["result"].GetInt();
    if(result == 1){                //gou mai cheng gong
        if(loadLayer) {
            loadLayer->removeFromParent();
            loadLayer = nullptr;
            
            long chips = atol(UserInfo::getInstance()->getChipAccount().c_str()) + atol(pData->getmallGiveChip().c_str());
            auto strChips = __String::createWithFormat("%ld", chips);
            UserInfo::getInstance()->setChipAccount(strChips->getCString());
            showResultTips(true, true);
        }
    }
    else{
        if(loadLayer) {
            loadLayer->removeFromParent();
            loadLayer = nullptr;
            
            showResultTips(true, false);
        }
    }
}
#endif

