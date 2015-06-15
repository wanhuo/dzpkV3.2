#include "MallLayer.h"
#include "MainScene.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "LoginScene.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../EncryptAndDecrypt/Md5/md5.h"
#include "../NetWork/stream.h"
#include "../GlobalDef/GlobalNode.h"
#include "GameScene.h"
#include "GameHallScene.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "GameScene.h"
#include "../GlobalDef/Data/MallInfoData.h"
#include "../GlobalDef/ResourcePreload.h"
#include "../GlobalDef/table/MallInfoTable.h"
#include "../Utils/DBUtil.h"
#include "PagePaneLayer.h"
#include "GoodsPriceLayer.h"

MallLayer* MallLayer::create(MALL_INFO_FLAG mallInfoFlag){
    MallLayer* layer = new MallLayer();
    if(layer){
        layer->mallInfoFlag = mallInfoFlag;
    }
    
    if(layer && layer->init()){
        layer->autorelease();
        return layer;
    }
    else{
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool MallLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	goodsTips = NULL;
    switch(this->mallInfoFlag){
        case MALL_CHIP:
            m_nCurMall = (int)MALL_INFO_CHIP;
            break;
        case MALL_PROP:
            m_nCurMall = (int)MALL_INFO_PROP;
            break;
        default:
            m_nCurMall = (int)MALL_INFO_CHIP;
            break;
    }
	
	GlobalNode::getInstance();    //添加全局刷新节点

	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();
    
    //bei jing
    auto mall_bg = Sprite::create("rank_list_layer_bg.png");
    mall_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(mall_bg);
    
    //shang bian mu kuang
    auto mall_top_frame = Sprite::create("mall_top_frame.png");
    mall_top_frame->setAnchorPoint(Vec2(0.5, 1));
    mall_top_frame->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 90));
    this->addChild(mall_top_frame);
    
    //shang bian deng guang
    auto mall_top_light = Sprite::create("mall_light.png");
    mall_top_light->setAnchorPoint(Vec2(0.5, 1));
    mall_top_light->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 120));
    this->addChild(mall_top_light);
    
    //xia bian mu kuang
    mall_bottom_frame = Sprite::create("mall_bottom_frame.png");
    mall_bottom_frame->setAnchorPoint(Vec2(0.5, 0));
    mall_bottom_frame->setPosition(Vec2(visibleSize.width / 2, 0 - 50));
    this->addChild(mall_bottom_frame);
    
    float scaleY = 1.0f;
    scaleY = (visibleSize.height - 90) * 1.0 / 482;
    
    //zuo bian mu kuang
    auto mall_left_frame = Sprite::create("mall_left_frame.png");
    mall_left_frame->setScaleY(scaleY);
    mall_left_frame->setAnchorPoint(Vec2(0, 0.5));
    mall_left_frame->setPosition(Vec2(0, (visibleSize.height - 100) * 1.0 / 2));
    this->addChild(mall_left_frame);
    
    //you bian mu kuang
    auto mall_right_frame = Sprite::create("mall_right_frame.png");
    mall_right_frame->setScaleY(scaleY);
    mall_right_frame->setAnchorPoint(Vec2(1, 0.5));
    mall_right_frame->setPosition(Vec2(visibleSize.width, (visibleSize.height - 100) * 1.0 / 2));
    this->addChild(mall_right_frame);
    
    //zhong jian mu kuang
    mall_middle_frame = Sprite::create("mall_middle_frame.png");
    mall_middle_frame->setScaleY(scaleY);
    mall_middle_frame->setAnchorPoint(Vec2(0.5, 0.5));
    mall_middle_frame->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height - 50) * 1.0 / 2));
    this->addChild(mall_middle_frame);
    
    //zhong jian deng guang
    auto mall_middle_light = Sprite::create("mall_light.png");
    mall_middle_light->setAnchorPoint(Vec2(0.5, 1));
    mall_middle_light->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height - 50) * 1.0 / 2 - 50));
    this->addChild(mall_middle_light);
    
    //shang bian biao ti
    auto mall_top_bg = Sprite::create("mall_top_bg.png");
    mall_top_bg->setAnchorPoint(Vec2(0, 1));
    mall_top_bg->setPosition(Vec2(0, visibleSize.height));
    this->addChild(mall_top_bg);

    // pagepane
	MallPagePaneLayer* pageLayer = MallPagePaneLayer::create((PAGE_PANE_MALL_FLAG)this->m_nCurMall);
	pageLayer->setAnchorPoint(Vec2(0, 1));
	pageLayer->setPosition(visibleSize.width / 2 - 40, visibleSize.height - 100);
	pageLayer->setPagePaneDelegate(this);
	this->addChild(pageLayer);

	// 关闭
	MenuItemImage *btn_close_menuitem = MenuItemImage::create("btn_close_up.png", "btn_close_down.png", CC_CALLBACK_1(MallLayer::btnCloseMallMenuCallback, this));
    btn_close_menuitem->setAnchorPoint(Vec2(1, 1));
	btn_close_menuitem->setPosition(visibleSize.width - 50, visibleSize.height - 30);

	auto btn_menu = Menu::create(btn_close_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	this->addChild(btn_menu);

	goodsLayer = Sprite::create();
	goodsLayer->setPosition(0, 0);
	this->addChild(goodsLayer);

	getMallInfoDatas(this->mallInfoFlag);

	return true;
}

void MallLayer::setMallLayerDelegate(MallLayerDelegate* mallLayerDelegate){
    this->mallLayerDelegate = mallLayerDelegate;
}

void MallLayer::removeGoodsLayer(bool bDeleteLayer)
{
	Vector<Node*> children = goodsLayer->getChildren();
	for (int i=0; i<children.size(); i++)
	{
		Node* pNode = children.at(i);
		if (pNode->getName().compare("AsyncImageLayer") == 0)
		{
			AsyncImageLayer* pLayer = (AsyncImageLayer*)pNode;
			pLayer->removeAsyncImage();
		}
	}
	if (bDeleteLayer)
	{
		goodsLayer->removeAllChildren();
	}

	if (goodsTips)
	{
		goodsTips->setVisible(false);
	}
}

void MallLayer::getMallInfoDatas(MALL_INFO_FLAG nFlag)
{
	loadLayer = LoadLayer::create();
	this->addChild(loadLayer);

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("chan", "0", alloc);
	ctx.AddMember("mver", "1", alloc);
	auto userId = UserInfo::getInstance()->getUserId();
	ctx.AddMember("user_id", userId.c_str(), alloc);
	auto sessionId = UserInfo::getInstance()->getSessionId();
	ctx.AddMember("sessionid", sessionId.c_str(), alloc);
	ctx.AddMember("ter", "WEB", alloc);

	srand(time(NULL));
	__String* serial = __String::createWithFormat("%d", rand() % 100000);
	ctx.AddMember("serial", serial->getCString(), alloc);

	rapidjson::Value req(rapidjson::kObjectType);
	req.AddMember("t", "getmallinfo", alloc);
	__String* str_type = __String::createWithFormat("%d", nFlag);
	req.AddMember("mall_type", str_type->getCString(), alloc);
	
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
	request->setResponseCallback(CC_CALLBACK_2(MallLayer::onHttpRequestCompleted, this));

	// write the post data
	request->setRequestData(encryptData.c_str(), encryptData.size()); 
	request->setTag("POST Binary test");
	HttpClient::getInstance()->send(request);
	request->release();
}

void MallLayer::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	loadLayer->removeFromParent();
	removeGoodsLayer();
	
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (!response->isSucceed()) 
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
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
	m_mallDatas.clear();
	m_mallDatas = MallInfoData::parseData(recvData);
	if (m_nCurMall == MALL_INFO_CHIP)
	{
		m_mallChipDatas.clear();
        for(int i = 10; i < m_mallDatas.size(); i++){
            m_mallChipDatas.pushBack(m_mallDatas.at(i));
        }
	}

    auto mallDataSize = m_mallDatas.size();
    if(mallDataSize > 8){
        mallDataSize = 8;
    }
    
    if(m_nCurMall == MALL_INFO_CHIP){
        for (int i=10; i<17; i++)
        {
            createGoods(i);
        }
    }
    else{
        for (int i=0; i<mallDataSize; i++)
        {
            createGoods(i);
        }
    }
}

void MallLayer::createGoods(int nIndex)
{
    int nRow = 0;
    int nLine = 0;
    if(nIndex > 8){
        nRow = (nIndex - 10) / 4;
        nLine = (nIndex - 10) % 4;
    }
    else{
        nRow = nIndex / 4;
        nLine = nIndex % 4;
    }

	MallInfoData* pData = m_mallDatas.at(nIndex);
	AsyncImageLayer* goods = AsyncImageLayer::create(pData);
	goods->setDelegate(this);
	goods->setName("AsyncImageLayer");
    float mallFramePosY;
    if(nRow == 0){
        mallFramePosY = mall_middle_frame->getPositionY();
    }
    else if(nRow == 1){
        mallFramePosY = mall_bottom_frame->getPositionY() + 65;
    }
    goods->setviewRect(Rect(visibleSize.width / 2 - 340 + nLine * 195, mallFramePosY + 50, 100, 100));
    goods->setPosition(visibleSize.width / 2 - 290 + nLine * 195, mallFramePosY + 100);
	goodsLayer->addChild(goods);

	bool bMoney = (m_nCurMall == (int)MALL_INFO_CHIP) ? true : false;
	GoodsPriceLayer* price = GoodsPriceLayer::create(atol(pData->getmallFirstChip().c_str()), bMoney);
	price->setAnchorPoint(Vec2(0, 0));
	price->setZOrder(100);
    price->setPosition(visibleSize.width / 2 - 360 + nLine * 195, mallFramePosY - 20);
	goodsLayer->addChild(price);
	
	// 购买
	MenuItemImage *btn_buy_menuitem = MenuItemImage::create("btn_buy_up.png", "btn_buy_down.png", CC_CALLBACK_1(MallLayer::btnBuyGoodsMenuCallback, this));
	btn_buy_menuitem->setTag(nIndex);
    btn_buy_menuitem->setPosition(visibleSize.width / 2 - 290 + nLine * 195, mallFramePosY + 5);

	auto btn_menu = Menu::create(btn_buy_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	goodsLayer->addChild(btn_menu);
}

void MallLayer::btnCloseMallMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	removeGoodsLayer(false);
    this->mallLayerDelegate->onCloseMallLayerCallback();
//	this->removeFromParent();
}

void MallLayer::btnBuyGoodsMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	removeGoodsLayer(false);
	MenuItemImage* pItem = (MenuItemImage*)object;
	int nIndex = pItem->getTag();
	MallInfoData* pData = m_mallDatas.at(nIndex);

	std::string strValue1 = "";
	std::string strValue2 = "";
	MallBuyInfoLayer* infoLayer = NULL;
    
    if(mallInfoFlag == MALL_CHIP){
        strValue1 = GlobalChineseXml::getInstance()->getValue("mall_buy_chip") + pData->getmallName();
		strValue2 = GlobalChineseXml::getInstance()->getValue("mall_buy_sell") + pData->getmallFirstChip();
		strValue2 += GlobalChineseXml::getInstance()->getValue("mall_yuan");
		infoLayer = MallBuyInfoLayer::create(MALL_BUY_INFO_CHIP, strValue1, strValue2);
    }
    else if(mallInfoFlag == MALL_PROP){
        if (atol(pData->getmallFirstChip().c_str()) > atol(UserInfo::getInstance()->getChipAccount().c_str() - RoomData::getInstance()->getAutoBuyChip()))
        {
            for (int i=0; i<m_mallChipDatas.size(); i++)
            {
                MallInfoData* chipData = m_mallChipDatas.at(i);
                if (atol(chipData->getmallGiveChip().c_str()) + atol(UserInfo::getInstance()->getChipAccount().c_str())
					>= atol(pData->getmallFirstChip().c_str())
					|| i == m_mallChipDatas.size() - 1)
                {
                    strValue2 = chipData->getmallFirstChip() + GlobalChineseXml::getInstance()->getValue("mall_yuan")
                    + std::string("=") + chipData->getmallGiveChip() + GlobalChineseXml::getInstance()->getValue("mall_chip");
                    break;
                }
            }
            strValue1 = GlobalChineseXml::getInstance()->getValue("mall_chip_not_enough");
            infoLayer = MallBuyInfoLayer::create(MALL_BUY_INFO_CHIP, strValue1, strValue2);
        }
        else
        {
            strValue1 = GlobalChineseXml::getInstance()->getValue("mall_buy_prop") + pData->getmallName();
            strValue2 = GlobalChineseXml::getInstance()->getValue("mall_buy_sell") + pData->getmallFirstChip();
            strValue2 += GlobalChineseXml::getInstance()->getValue("mall_chip");
            infoLayer = MallBuyInfoLayer::create(MALL_BUY_INFO_OTHERS, strValue1, strValue2);
        }
    }

	infoLayer->setData(pData);
	infoLayer->setPosition(0, 0);
	this->addChild(infoLayer);
}

void MallLayer::clicked(int nRoom)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	m_nCurMall = nRoom;
	
	switch (nRoom)
	{
	case MALL_INFO_CHIP:
        mallInfoFlag = MALL_CHIP;
		getMallInfoDatas(MALL_CHIP);
		break;
	case MALL_INFO_PROP:
        mallInfoFlag = MALL_PROP;
		getMallInfoDatas(MALL_PROP);
		break;
    case MALL_INFO_PRESENT:
        mallInfoFlag = MALL_PRESENT;
        getMallInfoDatas(MALL_PRESENT);
        break;
	default:
		break;
	}
}

void MallLayer::clicked(MallInfoData* data)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	int nIndex = m_mallDatas.getIndex(data);
	if (nIndex < 0 || nIndex >= m_mallDatas.size())
	{
		if(goodsTips)
			goodsTips->setVisible(false);
		return;
	}

	if (!goodsTips)
	{
		goodsTips = Sprite::create("mall_tips_bg.png");
		Label* tips = Label::createWithSystemFont("", "Arial", 20);
		tips->setName("tips");
		tips->setAnchorPoint(Vec2(0, 0));
        tips->setDimensions(95, 120);
		tips->setHorizontalAlignment(TextHAlignment::LEFT);
		tips->setPosition(5, 5);
		goodsTips->addChild(tips);
		this->addChild(goodsTips);
	}
	goodsTips->setVisible(true);
    int nRow = 0;
    int nLine = 0;
    if(nIndex > 8){
        nRow = (nIndex - 10) / 4;
        nLine = (nIndex - 10) % 4;
    }
    else{
        nRow = nIndex / 4;
        nLine = nIndex % 4;
    }
    
    float mallFramePosY;
    if(nRow == 0){
        mallFramePosY = mall_middle_frame->getPositionY() - 20;
    }
    else if(nRow == 1){
        mallFramePosY = mall_bottom_frame->getPositionY() + 65;
    }
    
    goodsTips->setPosition(visibleSize.width / 2 - 200 + nLine * 195, mallFramePosY + 115);
	
	Label* tips = (Label*)goodsTips->getChildByName("tips");
	if (tips)
	{
		tips->setString(data->getmallExplain().c_str());
	}
}
