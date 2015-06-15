//
//  QuickBuyChipLayer.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-17.
//
//

#include "QuickBuyChipLayer.h"
#include "../GlobalDef/Data/QuickChipData.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../EncryptAndDecrypt/Base64/base64.h"

QuickBuyChipLayer::~QuickBuyChipLayer(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    delete m_IAP;
#endif
}

bool QuickBuyChipLayer::init(){
    if(!BasePopupBox::init()){
        return false;
    }
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        m_IAP = new IOSiAP();
        m_IAP->delegate = this;
    #endif
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    auto quick_buy_chip_bg = Sprite::create("quick_buy_chip_bg.png");
    quick_buy_chip_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(quick_buy_chip_bg);
    
    auto mallChipDatas = QuickChipData::getInstance()->getMallChipDatas();
    infoData = mallChipDatas.at(10);
    auto price = infoData->getmallFirstChip();
    auto giveChip = infoData->getmallGiveChip();
    
    auto chipText = GlobalChineseXml::getInstance()->getValue("mall_chip");
    auto strGiveChip = __String::create(giveChip);
    strGiveChip->append(chipText);
    auto chipLabel = Label::createWithSystemFont(strGiveChip->getCString(), "Arial", 27);
    chipLabel->setAnchorPoint(Vec2(0, 0));
    chipLabel->setPosition(Vec2(visibleSize.width / 2 - 60, visibleSize.height / 2 + 38));
    this->addChild(chipLabel);
    
    auto yuanText = GlobalChineseXml::getInstance()->getValue("mall_yuan");
    auto strPrice = __String::create(price);
    strPrice->append(yuanText);
    auto priceLabel = Label::createWithSystemFont(strPrice->getCString(), "Arial", 27);
    priceLabel->setAnchorPoint(Vec2(0, 0));
    priceLabel->setPosition(Vec2(visibleSize.width / 2 - 60, visibleSize.height / 2 - 1));
    this->addChild(priceLabel);
    
    auto buyChipOkMenu = MenuItemImage::create("quick_buy_chip_ok_up.png", "quick_buy_chip_ok_down.png", CC_CALLBACK_1(QuickBuyChipLayer::btnBuyChipOkMenuCallback, this));
    buyChipOkMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 50);
    
    auto buyChipOtherMenu = MenuItemImage::create("quick_buy_chip_other_up.png", "quick_buy_chip_other_down.png", CC_CALLBACK_1(QuickBuyChipLayer::btnOtherChipMenuCallback, this));
    buyChipOtherMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);
    
    auto buyChipCloseMenu = MenuItemImage::create("btn_close_up.png", "btn_close_down.png", CC_CALLBACK_1(QuickBuyChipLayer::btnCloseMenuCallback, this));
    buyChipCloseMenu->setPosition(Vec2(visibleSize.width / 2 + 245, visibleSize.height / 2 + 155));
    
    auto menu = Menu::create(buyChipOkMenu, buyChipOtherMenu, buyChipCloseMenu, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu);
    
    return true;
}

void QuickBuyChipLayer::btnBuyChipOkMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    loadLayer = LoadLayer::create();
    this->addChild(loadLayer);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // first request apple store products
    std::vector<std::string> products;
    products.push_back(infoData->getmallID());
    m_IAP->requestProducts(products);
#else
    loadLayer->removeFromParent();
    loadLayer = nullptr;
#endif
}

void QuickBuyChipLayer::btnOtherChipMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    auto parent = this->getParent();
    this->removeFromParent();
    QuickBuyOtherChipLayer* otherChipLayer = QuickBuyOtherChipLayer::create();
    parent->addChild(otherChipLayer, 10);
}

void QuickBuyChipLayer::btnCloseMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    this->removeFromParent();
}

void QuickBuyChipLayer::showResultTips(bool buyChip, bool bSuccess)
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
    
	CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(QuickBuyChipLayer::endAction, this));
    
	FadeOut* fade_action = FadeOut::create(2.0f);
	Sequence* fade = Sequence::create(fade_action, func, NULL);
	sp_tips->runAction(fade);
}

void QuickBuyChipLayer::endAction(Node* node)
{
	this->removeFromParent();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void QuickBuyChipLayer::onRequestProductsFinish(void){
    std::string identifier = infoData->getmallID();
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

void QuickBuyChipLayer::onRequestProductsError(int code){
    if (loadLayer) {
        loadLayer->removeFromParent();
        loadLayer = nullptr;
        showResultTips(true, false);
    }
}

void QuickBuyChipLayer::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity, const char *bytes){
    log("on payment:%d", event);
    switch (event) {
        case IOSIAP_PAYMENT_PURCHASING:
            break;
        case IOSIAP_PAYMENT_PURCHAED:
            {
                log("IOSIAP_PAYMENT_PURCHAED");
            
                //json ˝æ›ππ‘Ï
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
            
                //base64º”√‹
                const std::string encryptData = gloox::Base64::encode64(jsonData);
            
                HttpRequest* request = new HttpRequest();
                auto httpServerAddress = GlobalDef::getInstance()->getHttpServerAddress();
                request->setUrl(httpServerAddress.c_str());
//              request->setUrl("http://192.168.1.120:8080/hallroom/jsonServlet");
                request->setRequestType(HttpRequest::Type::POST);
                request->setResponseCallback(CC_CALLBACK_2(QuickBuyChipLayer::onPaymentPurchaedHttpRequestCompleted, this));
            
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

void QuickBuyChipLayer::onPaymentPurchaedHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
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
	std::string encodeRecvData; //Ω” ’µΩµƒbase64º”√‹ ˝æ›
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		printf("%c", (*buffer)[i]);
		encodeRecvData.push_back((*buffer)[i]);
	}
	const std::string recvData = gloox::Base64::decode64(encodeRecvData); //Ω‚√‹∫Ûµƒ ˝æ›
    
    //json ˝æ›Ω‚Œˆ
	rapidjson::Document docOnPaymentRecv;
	docOnPaymentRecv.Parse<0>(recvData.c_str());
    
    //jie xi fu wu qi fan hui shu ju
    int result = docOnPaymentRecv["resp"]["result"].GetInt();
    if(result == 1){                //gou mai cheng gong
        if(loadLayer) {
            loadLayer->removeFromParent();
            loadLayer = nullptr;
            
            long chips = atol(UserInfo::getInstance()->getChipAccount().c_str()) + atol(infoData->getmallGiveChip().c_str());
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