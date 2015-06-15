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
#include "SettingLayer.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "../Scene/GameScene.h"
#include "../GlobalDef/Data/RoomData.h"
#include "../GlobalDef/ResourcePreload.h"
#include "../GlobalDef/GlobalDef.h"
#include "LoginOutLayer.h"
#include "BroadCastView.h"
#include "NoticeView.h"
#include "PlayerCenterView.h"
#include "ProductData.h"
#include "LoginRewardView.h"
#define DEFAULT_HEAD_URL		"http://1251038220.cdn.myqcloud.com/1251038220/"
Scene* MainScene::createScene(){
	Scene* scene = Scene::create();

	auto layer = MainScene::create();
	scene->addChild(layer);

	return scene;
}

bool MainScene::init(){
	if(!Layer::init()){
		return false;
	}
    
    gameScene = nullptr;

	GlobalNode::getInstance();    //添加全局刷新节点

	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();

	selectMenuRect.origin.x = visibleSize.width - 188;
	selectMenuRect.origin.y = visibleSize.height - 217;
	selectMenuRect.size.width = 188;
	selectMenuRect.size.height = 136;
    
    float frameScale = GlobalDef::getInstance()->getFrameScaleX();

	//main背景
	auto bg_main = Sprite::create("main_bg.png");
    bg_main->setScale(1.0f, 1.0f / frameScale);
	bg_main->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg_main);

	// 中心选择区域
	SelectLayer* select_layer = SelectLayer::create();
	select_layer->setPosition(0, 0);
	select_layer->setSelectDelegate(this);
	this->addChild(select_layer);

	//顶部条
	auto main_top_bg = Sprite::create("main_top_bg.png");
	main_top_bg->setAnchorPoint(Vec2(0, 1));
	main_top_bg->setPosition(0, visibleSize.height+1);
	this->addChild(main_top_bg);

	//顶部标志
	auto main_top_logo = Sprite::create("main_top_logo.png");
	main_top_logo->setAnchorPoint(Vec2(0, 0.5));
	main_top_logo->setPosition(20, visibleSize.height - 42.5);
	this->addChild(main_top_logo);

	//顶部菜单
	auto main_msg_menu = MenuItemImage::create("main_msg_menu_up.png", "main_msg_menu_down.png", CC_CALLBACK_1(MainScene::btMsgMenuCallback, this));
	main_msg_menu->setAnchorPoint(Vec2(1, 0.5));
	main_msg_menu->setPosition(visibleSize.width - 198, visibleSize.height - 42.5);
	
	auto main_mail_menu = MenuItemImage::create("main_mail_menu_up.png", "main_mail_menu_down.png", CC_CALLBACK_1(MainScene::btMailMenuCallback, this));
	main_mail_menu->setAnchorPoint(Vec2(1, 0.5));
	main_mail_menu->setPosition(visibleSize.width - 109, visibleSize.height - 42.5);

	auto main_select_menu = MenuItemImage::create("main_select_menu_up.png", "main_select_menu_down.png", CC_CALLBACK_1(MainScene::btSelectMenuCallback, this));
	main_select_menu->setAnchorPoint(Vec2(1, 0.5));
	main_select_menu->setPosition(visibleSize.width - 20, visibleSize.height - 42.5);

	auto main_top_menu = Menu::create(main_msg_menu, main_mail_menu, main_select_menu, NULL);
	main_top_menu->setPosition(0, 0);
	this->addChild(main_top_menu);

	
//    auto playerInfoItem = MenuItemImage::create("default_photo88.png", "default_photo88.png", CC_CALLBACK_1(MainScene::btShowPlayerInfoView, this));
//	playerInfoItem->setAnchorPoint(Vec2(0.5, 0.5));
//    playerInfoItem->setScale(0.8f);
//	playerInfoItem->setPosition(72, 44);
//	auto playerInfoMenu = Menu::create(playerInfoItem, NULL);
//	playerInfoMenu->setPosition(0, 0);
//	this->addChild(playerInfoMenu);
    
    GlobalDef::getInstance()->setHeadImageScale(0.8);
    dataPro=new ProductData();
    std::string strUrl = UserInfo::getInstance()->getHeadPhotoSerial();
    strUrl = strUrl.replace(0, strlen(DEFAULT_HEAD_URL), "");
    dataPro->setMall_path(strUrl.c_str());
    head = DownHeadImageView::create(dataPro);
//    head->setDelegate(this);
    head->setName("AsyncImageLayer");
    head->setAnchorPoint(Vec2(0.5, 0.5));
    head->setviewRect(Rect(0, 0, 96, 96));
    head->setPosition(73, 44);
    this->addChild(head);
    
//    headRect
    headRect.origin.x  = 25;
    headRect.origin.y = 0;
    headRect.size.width = 100;
    headRect.size.height = 100;

	//底部背景
	auto main_bottom_bg = Sprite::create("main_bottom_bg.png");
	main_bottom_bg->setAnchorPoint(Vec2(0, 0));
	main_bottom_bg->setPosition(0, 0);
	this->addChild(main_bottom_bg);

	//用户等级
	Label* name_grade = Label::createWithSystemFont("", "Arial", 23);
	auto strDzpkLevel = __String::create("LV.");
	strDzpkLevel->append(UserInfo::getInstance()->getDzpkLevel());
	name_grade->setString(strDzpkLevel->getCString());
	name_grade->setScale(0.8f);
	name_grade->setAnchorPoint(Vec2(0, 0));
	name_grade->setPosition(120, 47.5);
	this->addChild(name_grade);

	//用户昵称
	Label* name = Label::createWithSystemFont("", "Arial", 23);
	name->setString(UserInfo::getInstance()->getNikeName());
	name->setScale(0.8f);
	name->setAnchorPoint(Vec2(0, 0));
	name->setPosition(165, 47.5);
	this->addChild(name);

	//筹码
	auto stdChipAccount = UserInfo::getInstance()->getChipAccount();
	long chipAccount = atol(stdChipAccount.c_str());
	chip = Label::createWithSystemFont("", "Arial", 23);
    chip->setTag(TAG_LABEL_GOLDNUM);
	if(chipAccount > 1000000){      //大于1000000，数值显示不够
		__String* strChipAccount = __String::createWithFormat("筹码：%fM", chipAccount * 1.0 / 1000000);
		chip->setString(strChipAccount->getCString());
	}
	else{
		auto strChipAccount = __String::create(GlobalChineseXml::getInstance()->getValue("chipLabel"));
		strChipAccount->append(UserInfo::getInstance()->getChipAccount());
		chip->setString(strChipAccount->getCString());
	}
    
	chip->setScale(0.8f);
	chip->setAnchorPoint(Vec2(0, 1));
	chip->setPosition(120, 42.5);
	this->addChild(chip);

	//底部菜单
	auto main_shop_menu = MenuItemImage::create("main_shop_menu_up.png", "main_shop_menu_down.png", CC_CALLBACK_1(MainScene::btShopMenuCallback, this));
	main_shop_menu->setAnchorPoint(Vec2(1, 0.5));
	main_shop_menu->setPosition(visibleSize.width, 45);

	auto main_rank_menu = MenuItemImage::create("main_rank_menu_up.png", "main_rank_menu_down.png", CC_CALLBACK_1(MainScene::btRankMenuCallback, this));
	main_rank_menu->setAnchorPoint(Vec2(1, 0.5));
	main_rank_menu->setPosition(visibleSize.width - 239, 45);

	auto main_active_menu = MenuItemImage::create("main_active_menu_up.png", "main_active_menu_down.png", CC_CALLBACK_1(MainScene::btActiveMenuCallback, this));
	main_active_menu->setAnchorPoint(Vec2(1, 0.5));
	main_active_menu->setPosition(visibleSize.width - 333, 45);

	auto main_packge_menu = MenuItemImage::create("main_packge_menu_up.png", "main_packge_menu_down.png", CC_CALLBACK_1(MainScene::btPackgeMenuCallback, this));
	main_packge_menu->setAnchorPoint(Vec2(1, 0.5));
	main_packge_menu->setPosition(visibleSize.width - 427, 45);

	auto main_bottom_menu = Menu::create(main_shop_menu, main_rank_menu, main_active_menu, main_packge_menu, NULL);
	main_bottom_menu->setPosition(0, 0);
	this->addChild(main_bottom_menu);

	//触摸层
	MainSceneTouchLayer* touchLayer = MainSceneTouchLayer::create();
	touchLayer->setMainSceneTouchDelegate(this);
	touchLayer->setPosition(0, 0);
	this->addChild(touchLayer);

    isLoginRewardReq();
	return true;
}

void MainScene::btMsgMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    BroadCastView *broadCastView = BroadCastView::create();
    broadCastView->setAnchorPoint(Vec2(0, 0.5));
    broadCastView->setPosition(broadCastView->getPosition()+Vec2(0,broadCastView->getContentSize().width * 0.5));
    MoveTo* action = MoveTo::create(0.5, (broadCastView->getPosition()+Vec2(0, -broadCastView->getContentSize().width * 0.5)));
    EaseOut* ease = EaseOut::create(action, 0.2);
    Action *actions = Sequence::create(ease,NULL);
    broadCastView->runAction(actions);
    this->addChild(broadCastView);
}

void MainScene::btMailMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    NoticeView *notiveciew = NoticeView::create();
    notiveciew->setAnchorPoint(Vec2(0, 0.5));
    notiveciew->setPosition(notiveciew->getPosition()+Vec2(0,notiveciew->getContentSize().width * 0.5));
    MoveTo* action = MoveTo::create(0.5, (notiveciew->getPosition()+Vec2(0,-notiveciew->getContentSize().width * 0.5)));
    EaseOut* ease = EaseOut::create(action, 0.2);
    Action *actions = Sequence::create(ease,NULL);
    notiveciew->runAction(actions);
    this->addChild(notiveciew);
}

void MainScene::btSelectMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	if(this->getChildByTag(MAIN_SELECT_MENU_TAG) != NULL){
		this->removeChildByTag(MAIN_SELECT_MENU_TAG, true);
	}
	else{	
		//退出游戏菜单
		auto main_logout_menu = MenuItemImage::create("main_logout_menu_up.png", "main_logout_menu_down.png", CC_CALLBACK_1(MainScene::btLogOutMenuCallback, this));
		main_logout_menu->setAnchorPoint(Vec2(1, 1));
		main_logout_menu->setPosition(visibleSize.width, visibleSize.height - 81);

		//设置菜单
		auto main_set_menu = MenuItemImage::create("main_set_menu_up.png", "main_set_menu_down.png", CC_CALLBACK_1(MainScene::btSetMenuCallback, this));
		main_set_menu->setAnchorPoint(Vec2(1, 1));
		main_set_menu->setPosition(visibleSize.width, visibleSize.height - 149);

		auto main_select_menu = Menu::create(main_logout_menu, main_set_menu, NULL);
		main_select_menu->setTag(MAIN_SELECT_MENU_TAG);
		main_select_menu->setPosition(0, 0);
		this->addChild(main_select_menu);
	}
}

void MainScene::btLogOutMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    if(this->getChildByTag(MAIN_SELECT_MENU_TAG) != NULL){
        this->removeChildByTag(MAIN_SELECT_MENU_TAG, true);
    }
	
    LoginOutLayer *loginOutLayer = LoginOutLayer::create();
    this->addChild(loginOutLayer);
}

void MainScene::btSetMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    if(this->getChildByTag(MAIN_SELECT_MENU_TAG) != NULL){
        this->removeChildByTag(MAIN_SELECT_MENU_TAG, true);
    }
	SettingLayer* settingLayer = SettingLayer::create();
	this->addChild(settingLayer);
}

void MainScene::btShopMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	MallLayer* mallLayer = MallLayer::create();
    mallLayer->setMallLayerDelegate(this);
    mallLayer->setTag(MAIN_SCENE_MALL_LAYER_TAG);
	this->addChild(mallLayer);
}

void MainScene::onCloseMallLayerCallback(){
    this->removeChildByTag(MAIN_SCENE_MALL_LAYER_TAG);
    auto strChipAccount = __String::create(GlobalChineseXml::getInstance()->getValue("chipLabel"));
    strChipAccount->append(UserInfo::getInstance()->getChipAccount());
    chip->setString(strChipAccount->getCString());
}

void MainScene::btRankMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	Scene* rankListScene = RankListLayer::createScene();
	Director::getInstance()->replaceScene(rankListScene);
}

void MainScene::btActiveMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    //tan chu ti shi tiao, ti shi zan wu huo dong
    Sprite* sp_tips = Sprite::create("mall_buy_result_bg.png");;
    sp_tips->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(sp_tips);
    
    std::string str;
    str = GlobalChineseXml::getInstance()->getValue("game_active_msg");
    auto label = Label::createWithSystemFont(str, "Arial", 20);
    label->setPosition(401.5, 36);
    sp_tips->addChild(label);
    
    FadeOut* fade_action = FadeOut::create(2.0f);
    CallFunc* removeTips = CallFunc::create([=](){
        this->removeChild(sp_tips);
    });
    
    Sequence* fade = Sequence::create(fade_action, removeTips, NULL);
    sp_tips->runAction(fade);
   
//    LoginRewardView* loginRewardView = LoginRewardView::create();
//    this->addChild(loginRewardView);
}

void MainScene::btPackgeMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    GlobalDef::getInstance()->setPaperIndex(1);
    PlayerCenterView *notiveciew = PlayerCenterView::create();
    notiveciew->setTag(PLAYER_CENTER_VIEW_TAG);
    notiveciew->setPlayerCenterViewDelegate(this);
    
    notiveciew->setScale(0.5);
    notiveciew->setOpacity(0);
//    ScaleTo *scaleBig = ScaleTo::create(0.1, 1.01 * 1.0);
    ScaleTo *scaleNormal = ScaleTo::create(0.3, 1.0 * 1.0);
    FadeIn *fadein = FadeIn::create(.2);
    notiveciew->runAction(fadein);
    notiveciew->runAction(Sequence::create(scaleNormal, NULL));
    this->addChild(notiveciew);
}

void MainScene::btShowPlayerInfoView(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    GlobalDef::getInstance()->setPaperIndex(0);
    PlayerCenterView *notiveciew = PlayerCenterView::create();
    notiveciew->setTag(PLAYER_CENTER_VIEW_TAG);
    notiveciew->setPlayerCenterViewDelegate(this);
    notiveciew->setScale(0.5);
    notiveciew->setOpacity(0);
    //    ScaleTo *scaleBig = ScaleTo::create(0.1, 1.01 * 1.0);
    ScaleTo *scaleNormal = ScaleTo::create(.3, 1.0 * 1.0);
    FadeIn *fadein=CCFadeIn::create(.2);
    notiveciew->runAction(fadein);
    notiveciew->runAction(CCSequence::create(scaleNormal, NULL));
    this->addChild(notiveciew);
}

void MainScene::onPlayerCenterViewGoBack(){
    this->removeChildByTag(PLAYER_CENTER_VIEW_TAG);
    
    //将用户网络头像添加到spriteframecache
    HttpRequest* request = new HttpRequest();
    auto selfHeadPhotoSerial = UserInfo::getInstance()->getHeadPhotoSerial();
    request->setUrl(selfHeadPhotoSerial.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(MainScene::onHttpGetPhoto, this));
    
    auto strUserId = UserInfo::getInstance()->getUserId();
    request->setTag(strUserId.c_str());
    HttpClient::getInstance()->send(request);
    request->release();
}

void MainScene::onHttpGetPhoto(HttpClient *sender, HttpResponse *response){
    do{
		log("onHttpGetPhoto");
		char *endChar = strrchr((char *)response->getHttpRequest()->getUrl(), '.');
        log(endChar);
        //没有.jpg\.png等后缀，cocos2d-x引擎解析会报异常，对于这种情况，添加本地图片, .png , .jpeg, .pvr, .gz, .webp, .tiff, .ccz
        if((strcmp(endChar, ".png") == 0) || (strcmp(endChar, ".PNG") == 0)
           || (strcmp(endChar, ".jpeg") == 0) || (strcmp(endChar, ".JPEG") == 0)
           || (strcmp(endChar, ".pvr") == 0) || (strcmp(endChar, ".PVR") == 0)
           || (strcmp(endChar, ".gz") == 0) || (strcmp(endChar, ".GZ") == 0)
           || (strcmp(endChar, ".webp") == 0) || (strcmp(endChar, ".WEBP") == 0)
           || (strcmp(endChar, ".tiff") == 0) || (strcmp(endChar, ".TIFF") == 0)
           || (strcmp(endChar, ".ccz") == 0) || (strcmp(endChar, ".CCZ") == 0)
           || (strcmp(endChar, ".jpg") == 0) || (strcmp(endChar, ".JPG") == 0)){
            
        }
        else{               //没有.jpg\.png等后缀，cocos2d-x引擎解析会报异常，对于这种情况，添加本地图片, .png , .jpeg, .pvr, .gz, .webp, .tiff, .ccz
            
			break;
        }
        
		if (!response){
			break;
		}
        
		if (!response->isSucceed()){
			log("response failed");
			log("error buffer: %s", response->getErrorBuffer());
			break;
		}
        
		vector<char> *buffer = response->getResponseData();
        
		//create image
		Image* img = new Image();
        //        img->autorelease();
		img->initWithImageData((unsigned char*)buffer->data(),buffer->size());
        
		//create texture
		Texture2D* texture = new Texture2D();
        texture->autorelease();
		bool isImg = texture->initWithImage(img);
        img->release();
		if(isImg){
            if(head != NULL){
                auto headImage = head->getSpImage();
                if(headImage != NULL){
                    headImage->setTexture(texture);
                }
            }
		}
	}while(0);
}

void MainScene::selected(SELECT_GAME_FLAG nFlag)
{
	switch (nFlag)
	{
	case GAME_COMPETITION:
        {
            //tan chu ti shi tiao, ti shi wei kai fang
            Sprite* sp_tips = Sprite::create("mall_buy_result_bg.png");;
            sp_tips->setPosition(visibleSize.width / 2, visibleSize.height / 2);
            this->addChild(sp_tips);
            
            std::string str;
            str = GlobalChineseXml::getInstance()->getValue("game_competion_msg");
            auto label = Label::createWithSystemFont(str, "Arial", 20);
            label->setPosition(401.5, 36);
            sp_tips->addChild(label);
            
            FadeOut* fade_action = FadeOut::create(2.0f);
            CallFunc* removeTips = CallFunc::create([=](){
                this->removeChild(sp_tips);
            });
            
            Sequence* fade = Sequence::create(fade_action, removeTips, NULL);
            sp_tips->runAction(fade);
        }
		break;
	case GAME_QUICKSTART:
        {
            if(GlobalDef::getInstance()->getSoundOn()){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
            }
            
            loadLayer = LoadLayer::create();
            this->addChild(loadLayer);
            
            NetBean::getInstance()->setNetDelegate((NetDelegate *)this);
            auto socketIp = GlobalDef::getInstance()->getSocketIp();
            NetBean::getInstance()->setAddress(socketIp.c_str(), 8006);
            //    NetBean::getInstance()->setAddress("192.168.1.118", 8006);
            NetBean::getInstance()->connect();
        }
		break;
	case GAME_COMMONHALL:
		{
            if(GlobalDef::getInstance()->getSoundOn()){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
            }
            
			Scene *gameHallScene = GameHallScene::createScene();
			Director::getInstance()->replaceScene(gameHallScene);
		}
		break;
	default:
		break;
	}
}

void MainScene::onTouchDelegate(Touch *t){
	log("onTouchCallback");
	//点击弹出对话框外部时，移除弹出对话框
	auto touchPoint = Director::getInstance()->convertToGL(t->getLocationInView());
	if (!selectMenuRect.containsPoint(touchPoint)){
		if(this->getChildByTag(MAIN_SELECT_MENU_TAG) != NULL){
			this->removeChildByTag(MAIN_SELECT_MENU_TAG, true);
		}
	}
    if (headRect.containsPoint(touchPoint)){
        btShowPlayerInfoView(nullptr);
    }
}

void MainScene::onCreate(){
	log("tcp connect successed");
}

void MainScene::onConnected(){
	//发送快速开始消息
	CStream stream;

	//json数据构造
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("act_commond", "370011", alloc);
	auto sessionId = UserInfo::getInstance()->getSessionId();
	ctx.AddMember("sessionid", sessionId.c_str(), alloc);
	ctx.AddMember("area_number", "normal", alloc);
	ctx.AddMember("msg_head", "dzpk", alloc);
	ctx.AddMember("timer_id", "0", alloc);
	ctx.AddMember("game_number", "dzpk", alloc);
	auto actUserId = UserInfo::getInstance()->getUserAccount();
	ctx.AddMember("act_user_id", actUserId.c_str(), alloc);
	ctx.AddMember("room_number", "0", alloc);
	ctx.AddMember("cps_record_id", "0", alloc);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	ctx.Accept(writer);

	const char *jsonData = buffer.GetString();	

	NetBean::getInstance()->addQQHead(&stream);
	NetBean::getInstance()->addHead(&stream, strlen(jsonData) + 1, 370011);
	NetBean::getInstance()->addData(&stream, (char *)jsonData, strlen(jsonData));
	NetBean::getInstance()->write(stream);
}

void MainScene::onMessage(char *buff, int len){
	rapidjson::Document docEnterRoom;
	docEnterRoom.Parse<0>(buff);

	//根据服务器返回信息，判断是否快速启动成功，若快速开始成功，切换到房间场景
	RoomData::getInstance()->eraseData();
	RoomData* roomData = RoomData::getInstance();
	ROOM_INFO_T* roomInfo = new ROOM_INFO_T();
	if(docEnterRoom["act_commond"].GetInt() == 370001){   //进入房间成功
		roomInfo->setStep(docEnterRoom["dealer_property"]["step"].GetInt());
		roomInfo->setRoomType(docEnterRoom["dealer_property"]["room_type"].GetInt());
		roomInfo->setRoomSeatNum(docEnterRoom["dealer_property"]["room_seat_num"].GetInt());
		roomInfo->setAllDeskChips(docEnterRoom["dealer_property"]["all_desk_chips"].GetInt());
		roomInfo->setBanker(docEnterRoom["dealer_property"]["banker"].GetInt());
		roomInfo->setBetTime(docEnterRoom["dealer_property"]["bet_time"].GetInt());
		roomInfo->setTurn(docEnterRoom["dealer_property"]["turn"].GetInt());
		roomInfo->setBigBlind(docEnterRoom["dealer_property"]["big_blind"].GetInt());
		roomInfo->setSmallBlind(docEnterRoom["dealer_property"]["small_blind"].GetInt());
		for (int index = 0; index < 5; index++){
			roomInfo->deskCard[index] = docEnterRoom["dealer_property"]["desk_card"][index].GetInt();
		}
		if (!docEnterRoom["dealer_property"]["desk_pool"].IsNull()){
			roomInfo->desk_pool.clear();
			int deskPoolArraySize = docEnterRoom["dealer_property"]["desk_pool"].Capacity();
			for(int i = 0; i < deskPoolArraySize; i++){
				ROOM_DESK_POOL_T *roomDeskPool;
				roomDeskPool = new ROOM_DESK_POOL_T();
				roomDeskPool->setType(docEnterRoom["dealer_property"]["desk_pool"][i]["type"].GetInt());
				roomDeskPool->setId(docEnterRoom["dealer_property"]["desk_pool"][i]["id"].GetInt());
				roomDeskPool->setPool(docEnterRoom["dealer_property"]["desk_pool"][i]["pool"].GetInt());
				roomInfo->desk_pool.pushBack(roomDeskPool);
			}
		}
		roomInfo->setMaxBuyChip(docEnterRoom["dealer_property"]["max_buy_chip"].GetInt());
		roomInfo->setMinBuyChip(docEnterRoom["dealer_property"]["min_buy_chip"].GetInt());
		roomInfo->setMinBlind(docEnterRoom["dealer_property"]["min_blind"].GetInt());
		roomInfo->setRoomName(docEnterRoom["dealer_property"]["room_name"].GetString());
		roomInfo->setRoomNumber(docEnterRoom["room_number"].GetInt());
		//设置房间信息
		roomData->setRoomInfo(roomInfo);

		if(!docEnterRoom["hand_cards"].IsNull()){
			int handCardsNum = docEnterRoom["hand_cards"].Capacity();
			for(int i = 0; i < handCardsNum; i++){
				roomData->setHandCard(i + 1, docEnterRoom["hand_cards"][i].GetInt());
			}
		}

		roomData->setAtGame(atoi(docEnterRoom["at_game"].GetString()));
		roomData->setActUserId((char *)docEnterRoom["act_user_id"].GetString());
		roomData->setAreaNumber((char *)docEnterRoom["area_number"].GetString());

		//玩家信息
		if (!docEnterRoom["player_list"].IsNull()){
			int playerNum = docEnterRoom["player_list"].Capacity();
			if (playerNum > 0){
				ROOM_USER_INFO_T *roomUserInfo = nullptr;
				for (int i = 0; i < playerNum; i++){    
					roomUserInfo = new ROOM_USER_INFO_T();
					roomUserInfo->setUserId(docEnterRoom["player_list"][i]["user_id"].GetString());
					roomUserInfo->setUserAccount(docEnterRoom["player_list"][i]["user_account"].GetString());
					roomUserInfo->setNickName(docEnterRoom["player_list"][i]["nick_name"].GetString());
					roomUserInfo->setDzpkLevel(docEnterRoom["player_list"][i]["dzpk_level"].GetInt());
					roomUserInfo->setDzpkExperience(atoi(docEnterRoom["player_list"][i]["dzpk_experience"].GetString()));
					roomUserInfo->setVipLevel(docEnterRoom["player_list"][i]["vip_level"].GetInt());
					roomUserInfo->setChipAccount(atoi(docEnterRoom["player_list"][i]["chip_account"].GetString()));
					roomUserInfo->setCoinAccount(atoi(docEnterRoom["player_list"][i]["coin_account"].GetString()));
					roomUserInfo->setCity(docEnterRoom["player_list"][i]["city"].GetString());
					roomUserInfo->setTotalTime(docEnterRoom["player_list"][i]["total_time"].GetString());
					roomUserInfo->setWinPer(docEnterRoom["player_list"][i]["win_per"].GetString());
					roomUserInfo->setRankName(docEnterRoom["player_list"][i]["rank_name"].GetString());
					roomUserInfo->setHeadPhotoSerial(docEnterRoom["player_list"][i]["head_photo_serial"].GetString());
					roomUserInfo->setDeskChip(docEnterRoom["player_list"][i]["desk_chip"].GetInt());
					roomUserInfo->setSex(docEnterRoom["player_list"][i]["sex"].GetInt());
					roomUserInfo->setUserStatus(docEnterRoom["player_list"][i]["user_staus"].GetInt());
					roomUserInfo->setRoomNum(docEnterRoom["player_list"][i]["room_num"].GetInt());
					roomUserInfo->setSeatNum(docEnterRoom["player_list"][i]["seat_num"].GetInt());
					roomUserInfo->setHandChips(docEnterRoom["player_list"][i]["hand_chips"].GetInt());
					roomUserInfo->setHandChipsToShow(docEnterRoom["player_list"][i]["hand_chips"].GetInt());
					roomUserInfo->setGiftId(docEnterRoom["player_list"][i]["giftid"].GetInt());
					roomUserInfo->setLevelGift(docEnterRoom["player_list"][i]["level_gift"].GetString());
					roomUserInfo->setMallPath(docEnterRoom["player_list"][i]["mall_path"].GetString());
					roomUserInfo->setLevelTitle(docEnterRoom["player_list"][i]["level_title"].GetString());

					roomData->addPlayer(roomUserInfo);
				}
			}
		}

		//旁观者信息
		if(!docEnterRoom["watcher_list"].IsNull()){
			int watcherNum = docEnterRoom["watcher_list"].Capacity();
			if(watcherNum > 0){
				ROOM_USER_INFO_T *watcher = nullptr;
				for(int i = 0; i < watcherNum; i++){
					watcher = new ROOM_USER_INFO_T();
					watcher->setChipAccount(atoi(docEnterRoom["watcher_list"][i]["chip_account"].GetString()));
					watcher->setCoinAccount(atoi(docEnterRoom["watcher_list"][i]["coin_account"].GetString()));
					watcher->setCity(docEnterRoom["watcher_list"][i]["city"].GetString());
					watcher->setDeskChip(docEnterRoom["watcher_list"][i]["desk_chip"].GetInt());
					watcher->setDzpkExperience(atoi(docEnterRoom["watcher_list"][i]["dzpk_experience"].GetString()));
					watcher->setDzpkLevel(docEnterRoom["watcher_list"][i]["dzpk_level"].GetInt());
					watcher->setHandChips(docEnterRoom["watcher_list"][i]["hand_chips"].GetInt());
					watcher->setHandChipsToShow(docEnterRoom["watcher_list"][i]["hand_chips"].GetInt());
					watcher->setGiftId(docEnterRoom["watcher_list"][i]["giftid"].GetInt());
					watcher->setHeadPhotoSerial(docEnterRoom["watcher_list"][i]["head_photo_serial"].GetString());
					watcher->setLevelGift(docEnterRoom["watcher_list"][i]["level_gift"].GetString());
					watcher->setMallPath(docEnterRoom["watcher_list"][i]["mall_path"].GetString());
					watcher->setLevelTitle(docEnterRoom["watcher_list"][i]["level_title"].GetString());
					watcher->setNickName(docEnterRoom["watcher_list"][i]["nick_name"].GetString());
					watcher->setRankName(docEnterRoom["watcher_list"][i]["rank_name"].GetString());
					watcher->setRoomNum(docEnterRoom["watcher_list"][i]["room_num"].GetInt());
					watcher->setSeatNum(docEnterRoom["watcher_list"][i]["seat_num"].GetInt());
					watcher->setSex(docEnterRoom["watcher_list"][i]["sex"].GetInt());
					watcher->setTotalTime(docEnterRoom["watcher_list"][i]["total_time"].GetString());
					watcher->setUserId(docEnterRoom["watcher_list"][i]["user_id"].GetString());
					watcher->setUserAccount(docEnterRoom["watcher_list"][i]["user_account"].GetString());
					watcher->setUserStatus(docEnterRoom["watcher_list"][i]["user_staus"].GetInt());
					watcher->setVipLevel(docEnterRoom["watcher_list"][i]["vip_level"].GetInt());
					watcher->setWinPer(docEnterRoom["watcher_list"][i]["win_per"].GetString());

					roomData->addWatcher(watcher);
					roomData->addPlayer(watcher);
				}
			}
		}

		gameScene = GameScene::createScene();
		Director::getInstance()->replaceScene(gameScene);
	}
	else{
		log("进入房间失败！");
	}
}

//aò?°≈≈––∞ò????
void MainScene::isLoginRewardReq(){
	loadLayer = LoadLayer::create();
	this->addChild(loadLayer);
    
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
	
    rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("account", "0", alloc);
    
    auto userId=UserInfo::getInstance()->getUserId();
    ctx.AddMember("user_id", userId.c_str(), alloc);
    
	auto sessionId = UserInfo::getInstance()->getSessionId();
	ctx.AddMember("sessionid", sessionId.c_str(), alloc);
    
	ctx.AddMember("mver", "1", alloc);
	ctx.AddMember("ter", "WEB", alloc);
	srand(time(NULL));
	__String* serial = __String::createWithFormat("%d", rand() % 100000);
	ctx.AddMember("serial", serial->getCString(), alloc);
	ctx.AddMember("chan","0", alloc);
    
	rapidjson::Value req(rapidjson::kObjectType);
	req.AddMember("module", 2, alloc);
	req.AddMember("t", "getchipdate", alloc);
    
	doc.AddMember("req", req, alloc);
	doc.AddMember("ctx", ctx, alloc);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char *jsonData = buffer.GetString();
    
	const std::string encryptData = gloox::Base64::encode64(jsonData);
    
	HttpRequest* request = new HttpRequest();
	auto httpServerAddress = GlobalDef::getInstance()->getHttpServerAddress();
    request->setUrl(httpServerAddress.c_str());
//  request->setUrl("http://203.195.138.89/hallroom/jsonServlet");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(MainScene::onLoginRewardHttpRequestCompleted, this));
    
	request->setRequestData(encryptData.c_str(), encryptData.size());
	request->setTag("POST LoginReward test");
	HttpClient::getInstance()->send(request);
	request->release();
}

void MainScene::onLoginRewardHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
    loadLayer->removeFromParent();
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
	std::string encodeRecvData; //Ω”?’μΩμ?base64o”√?????
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		printf("%c", (*buffer)[i]);
		encodeRecvData.push_back((*buffer)[i]);
	}
	const std::string recvData = gloox::Base64::decode64(encodeRecvData);
    
    const char *data = recvData.c_str();
    
	rapidjson::Document docUserInfo;
	docUserInfo.Parse<0>(data);
    
    int respID=atoi(docUserInfo["ctx"]["err"].GetString());
    if(respID==0){
        int rewardNum=atoi(docUserInfo["resp"]["getchip"].GetString());
        if(rewardNum<=0){
            return;
        }
        GlobalDef::getInstance()->setLoginRewardNum(rewardNum);
        LoginRewardView* mallLayer = LoginRewardView::create();
        mallLayer->setScale(0.5);
        mallLayer->setOpacity(0);
        ScaleTo *scaleNormal = ScaleTo::create(.3, 1.0 * 1.0);
        FadeIn *fadein=CCFadeIn::create(.2);
        mallLayer->runAction(fadein);
        mallLayer->runAction(CCSequence::create(scaleNormal, NULL));
        this->addChild(mallLayer);
    }
}