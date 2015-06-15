#include "GameScene.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../NetWork/netbean.h"
#include "GameHallScene.h"
#include "../GlobalDef/Data/RoomData.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "MallLayer.h"
#include "CommunicateInputLayer.h"

GameScene::~GameScene(){
    this->winArray.clear();
}

Scene* GameScene::createScene(){
	auto scene = Scene::create();

	auto layer = GameScene::create();
    layer->setTag(GAME_SCENE_LAYER_TAG);
	scene->addChild(layer);

	return scene;
}

bool GameScene::init(){
	if(!Layer::init()){
		return false;
	}

	NetBean::getInstance()->setNetDelegate(this);

	visibleSize = Director::getInstance()->getVisibleSize();
	visibleOrigin = Director::getInstance()->getVisibleOrigin();

	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();

	sitProgressTagTurn = 0;
    progressVolumnPlayed = false;
    progressVolumnId = 0;
    
    seatDelta = 0;
    auto strSelfUserId = UserInfo::getInstance()->getUserId();
    auto playerList = RoomData::getInstance()->getPlayerList();
	for(int i = 0; i < RoomData::getInstance()->getPlayerListNum(); i++){
        auto player = playerList.at(i);
        if(strcmp(strSelfUserId.c_str(), player->getUserId().c_str()) == 0){
            int selfSeatNum = player->getSeatNum();
            if(selfSeatNum >= 1 && selfSeatNum <= 9){
                seatDelta = 5 - selfSeatNum;
            }
        }
	}

	//±≥æ∞
	auto game_scene_bg = Sprite::create("game_scene_bg.png");
	game_scene_bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(game_scene_bg);

	//≈∆◊¿
	Sprite* sp_gamedesk = Sprite::create("sp_gamedesk.png");
	sp_gamedesk->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 20);
	this->addChild(sp_gamedesk);

	//◊Û≤Àµ•
	MenuItemImage* btn_menu_item = MenuItemImage::create("btn_menu_up.png", "btn_menu_down.png", CC_CALLBACK_1(GameScene::btMenuCallback, this));
	btn_menu_item->setAnchorPoint(Vec2(0, 1));
	btn_menu_item->setPosition(10, visibleSize.height);

	Menu* btn_menu = Menu::create(btn_menu_item, NULL);
	btn_menu->setPosition(0, 0);
	btn_menu->setTag(LEFT_MENU_TAG);
	this->addChild(btn_menu);

	//…Ã≥«
	MenuItemImage* btn_shopping = MenuItemImage::create("btn_shopping_up.png", "btn_shopping_down.png", CC_CALLBACK_1(GameScene::btShoppingCallback, this));
	btn_shopping->setAnchorPoint(Vec2(1, 1));
	btn_shopping->setPosition(visibleSize.width - 10, visibleSize.height - 10);

	//≥‰÷µøÏΩ›≤Àµ•
	MenuItemImage* btn_charge = MenuItemImage::create("btn_charge_up.png", "btn_charge_down.png", CC_CALLBACK_1(GameScene::btChargeCallback, this));
	btn_charge->setAnchorPoint(Vec2(1, 1));
	btn_charge->setPosition(visibleSize.width - 105, visibleSize.height - 10);

	Menu* btn_shop_menu = Menu::create(btn_shopping, btn_charge, NULL);
	btn_shop_menu->setPosition(0, 0);
	this->addChild(btn_shop_menu);

	//±Ì«È
	MenuItemImage* btn_smile = MenuItemImage::create("bt_smile_up.png", "bt_smile_down.png", CC_CALLBACK_1(GameScene::btSmileCallback, this));
	btn_smile->setAnchorPoint(Vec2(0, 0));
	btn_smile->setPosition(25, 0 - 5);

	//øÏΩ›∂Ã”Ô
	MenuItemImage* btn_communicate = MenuItemImage::create("btn_communicate_up.png", "btn_communicate_down.png", CC_CALLBACK_1(GameScene::btCommunicateCallback, this));
	btn_communicate->setAnchorPoint(Vec2(0, 0));
	btn_communicate->setPosition(125, 15);

	Menu* btn_smile_and_communicate_menu = Menu::create(btn_smile, btn_communicate, NULL);
	btn_smile_and_communicate_menu->setTag(SMILE_COMM_MENU_TAG);
	btn_smile_and_communicate_menu->setPosition(0, 0);
	this->addChild(btn_smile_and_communicate_menu);
	showSmileAndCommunicateMenu();

	//∑øº‰ID£¨–°¥Û√§◊¢
	char* roomId = GlobalChineseXml::getInstance()->getValue("roomId");
    auto strRoomMsg = __String::create(roomId);
    strRoomMsg->appendWithFormat("%d", RoomData::getInstance()->getRoomInfo()->getRoomNumber());
    Label* roomMsgLabel = Label::createWithSystemFont(strRoomMsg->getCString(), "Arial", 17);
	roomMsgLabel->setColor(Color3B(252, 152, 3));
    roomMsgLabel->setAnchorPoint(Vec2(0, 1));
	roomMsgLabel->setPosition(100, visibleSize.height - 5);
	this->addChild(roomMsgLabel);
    
	char* blind = GlobalChineseXml::getInstance()->getValue("blind");
	auto strBlindMsg = __String::create(blind);
    auto roomMinBlind = RoomData::getInstance()->getRoomInfo()->getMinBlind();
    if(roomMinBlind < 1000){
        strBlindMsg->appendWithFormat("%d/%d", roomMinBlind, roomMinBlind * 2);
    }
    else if(roomMinBlind >= 1000 && roomMinBlind < 1000000){
        strBlindMsg->appendWithFormat("%dK/%dK", roomMinBlind / 1000, roomMinBlind * 2 / 1000);
    }
    else if(roomMinBlind >= 1000000 && roomMinBlind < 1000000000){
        strBlindMsg->appendWithFormat("%dM/%dM", roomMinBlind / 1000000, roomMinBlind * 2 / 1000000);
    }
	Label* blindMsgLabel = Label::createWithSystemFont(strBlindMsg->getCString(), "Arial", 17);
	blindMsgLabel->setColor(Color3B(252, 152, 3));
    blindMsgLabel->setAnchorPoint(Vec2(0, 1));
	blindMsgLabel->setPosition(100, visibleSize.height - 25);
	this->addChild(blindMsgLabel);

	//◊˘Œª
	drawSeat();
	//…Ë÷√ «∑Òœ‘ æ◊˘Œª
	setSeatVisible();

	//ªÊ÷∆ÕÊº“
	drawAllInlinePlayers();

	//ªÊ÷∆◊¿√Êπ´π≤≈∆
	drawPublicCards();

	//ªÊ÷∆”√ªß◊¥Ã¨
	drawUserStatus();

	//ªÊ÷∆ ÷≈∆
	drawHandCardsForEnterRoom();

	//ªÊ÷∆≥Ô¬Î
	//ªÊ÷∆ÕÊº“◊¿√Ê≥Ô¬Î
	int cardSeat = 0;
	for (int i = 0; i < RoomData::getInstance()->getCardSeatNumber().size(); i++){
		cardSeat = RoomData::getInstance()->getCardSeatNumber().at(i);
		auto player = RoomData::getInstance()->getPlayerBySeatNum(cardSeat);
		if(player->getDeskChip() > 0){
			showStaticDeskChip(cardSeat, player->getDeskChip());
		}
	}
	//ªÊ÷∆≥Ô¬Î≥ÿ
	auto deskPool = RoomData::getInstance()->getRoomInfo()->desk_pool;
	if(!deskPool.empty()){
		for(int i = 0; i < deskPool.size(); i++){
			drawStaticPool(deskPool.at(i)->getId(), deskPool.at(i)->getPool());
		}
	}

    log("roomStep: %d", RoomData::getInstance()->getRoomInfo()->getStep());
    
	//œ‘ æ¥Û–°√§◊¢°¢◊Øº“
    if((RoomData::getInstance()->getRoomInfo()->getStep() == 4) && (RoomData::getInstance()->getRoomInfo()->deskCard[0] == 0)){         //biao shi ci shi fang jian zheng zai jie suan, dan wei kai ju, bu yao hui zhi zhuang jia he da xiao mang zhu
    }
    else{
        drawBankerAndBlind();
    }

	//ªÊ÷∆µπº∆ ±
	drawSitProgress();

	//ªÊ÷∆≤Ÿ◊˜∞¥≈•≤¢Ω´∆‰“˛≤ÿ
	drawOperateMenuAndHide();
    
    showOperateMenu(RoomData::getInstance()->getRoomInfo()->getTurn());

	//≈–∂œµ±«∞ «∑Ò‘⁄ÕÊ≈∆£¨œ‘ æœ‡”¶≤Ÿ◊˜∞¥≈•£¨∑≈µΩ370062œ˚œ¢÷–¥¶¿Ì

	//–ƒÃ¯£¨√ø30s∑¢“ª¥Œ
	schedule(schedule_selector(GameScene::onHeartWaveUpdate), 30);

	//µ„ª˜∆¡ƒª£¨Ω´◊Û¿≠≤Àµ•°¢¡ƒÃÏøÚ°¢±Ì«ÈøÚ“˛≤ÿ
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [=](Touch* t, Event *e){
		if(this->getChildByTag(COMMUNICATE_LAYER_TAG) != NULL && !this->getChildByTag(COMMUNICATE_LAYER_TAG)->isVisible()){
			this->removeChildByTag(COMMUNICATE_LAYER_TAG);
		}

        auto point = Director::getInstance()->convertToGL(t->getLocationInView());
        
		//“˛≤ÿ≤ª”√µƒ≤Àµ•
		if(this->getChildByTag(COMMUNICATE_LAYER_TAG) != NULL && this->getChildByTag(COMMUNICATE_LAYER_TAG)->isVisible()){
			auto refSize = this->getChildByTag(COMMUNICATE_LAYER_TAG)->getContentSize();
			auto refRect = Rect(23, 71, refSize.width, refSize.height);
			if(!refRect.containsPoint(point)){
                if(this->getChildByTag(COMMUNICATE_LAYER_TAG) != NULL){
                    this->removeChildByTag(COMMUNICATE_LAYER_TAG);
                }
			}
		}
        
        //yin chang biao qing ceng
        if(this->getChildByTag(SMILE_LAYER_TAG) != NULL && this->getChildByTag(SMILE_LAYER_TAG)->isVisible()){
			auto refSize = this->getChildByTag(SMILE_LAYER_TAG)->getContentSize();
			auto refRect = Rect(23, 71, refSize.width, refSize.height);
			if(!refRect.containsPoint(point)){
                if(this->getChildByTag(SMILE_LAYER_TAG) != NULL){
                    this->removeChildByTag(SMILE_LAYER_TAG);
                }
			}
		}

		if(this->getChildByTag(BTN_LEFT_MENU_TAG) != NULL && this->getChildByTag(BTN_LEFT_MENU_TAG)->isVisible()){
			if(e->getCurrentTarget() != this->getChildByTag(BTN_LEFT_MENU_TAG)){
                if(this->getChildByTag(BTN_LEFT_MENU_TAG) != NULL){
                    this->removeChildByTag(BTN_LEFT_MENU_TAG);
                    if(this->getChildByTag(STAND_UP_DISABLE_TAG) != NULL && this->getChildByTag(STAND_UP_DISABLE_TAG)->isVisible()){
                        this->getChildByTag(STAND_UP_DISABLE_TAG)->setVisible(false);
                    }
                }
				this->getChildByTag(LEFT_MENU_TAG)->setVisible(true);
			}
		}

		return true;
	};
	Director::getInstance()->getEventDispatcher()->setPriority(listener, 1);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

//yin chang biao qing ceng
void GameScene::hideSmileLayer(){
    if(this->getChildByTag(SMILE_LAYER_TAG) != NULL){
		this->removeChildByTag(SMILE_LAYER_TAG);
	}
}

//yin chang liao tian ceng
void GameScene::hideCommunicateLayer(){
    if(this->getChildByTag(COMMUNICATE_LAYER_TAG) != NULL){
		this->removeChildByTag(COMMUNICATE_LAYER_TAG);
	}
}

//yin chang zuo bian cai dan
void GameScene::hideLeftMenu(){
    if(this->getChildByTag(BTN_LEFT_MENU_TAG) != NULL){
		this->removeChildByTag(BTN_LEFT_MENU_TAG);
        if(this->getChildByTag(STAND_UP_DISABLE_TAG) != NULL && this->getChildByTag(STAND_UP_DISABLE_TAG)->isVisible()){
            this->getChildByTag(STAND_UP_DISABLE_TAG)->setVisible(false);
        }
	}
    
    this->getChildByTag(LEFT_MENU_TAG)->setVisible(true);
}

//ªÊ÷∆∫Õ»•µÙ±Ì«È°¢øÏΩ›∂Ã”Ô≤Àµ•
void GameScene::showSmileAndCommunicateMenu(){
/*	if(this->getChildByTag(COMMUNICATE_LAYER_TAG) != NULL){
		this->removeChildByTag(COMMUNICATE_LAYER_TAG);
	}
    
    if(this->getChildByTag(SMILE_LAYER_TAG) != NULL){
		this->removeChildByTag(SMILE_LAYER_TAG);
	}
*/
	auto userId = UserInfo::getInstance()->getUserId();
    auto player = RoomData::getInstance()->getPlayer((char *)userId.c_str());
    if(player != NULL){
        if(player->getUserStatus() > 0){    //◊¯œ¬◊¥Ã¨£¨ø…“‘∑¢±Ì«È∫Õ∂Ã”Ô
            if ((this->getChildByTag(SMILE_COMM_MENU_TAG) != NULL) && !this->getChildByTag(SMILE_COMM_MENU_TAG)->isVisible()){
                this->getChildByTag(SMILE_COMM_MENU_TAG)->setVisible(true);
                ((Menu *)this->getChildByTag(SMILE_COMM_MENU_TAG))->setEnabled(true);
            }
        }
        else{		//≈‘π€◊¥Ã¨£¨“˛≤ÿ≤Àµ•
            if ((this->getChildByTag(SMILE_COMM_MENU_TAG) != NULL) && this->getChildByTag(SMILE_COMM_MENU_TAG)->isVisible()){
                this->getChildByTag(SMILE_COMM_MENU_TAG)->setVisible(false);
                ((Menu *)this->getChildByTag(SMILE_COMM_MENU_TAG))->setEnabled(false);
            }
        }
    }
}

//∂Ã”Ôµ•‘™—°÷–¥˙¿Ì∑Ω∑®£¨cellIndexŒ™±ª—°÷–µ•‘™µƒtagº”1£¨º¥¥”1ø™ º
void GameScene::onCommunicateCellTouched(int cellIndex){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    if(this->getChildByTag(COMMUNICATE_LAYER_TAG) != NULL){
		this->getChildByTag(COMMUNICATE_LAYER_TAG)->setVisible(false);
	}
    
	//∑¢ÀÕ—°÷–µƒŒƒ◊÷œ˚œ¢∏¯∑˛ŒÒ∆˜
	//chat_type£∫¿‡–Õ£∫ 2£∫∂Øª≠  1:Œƒ◊÷
	CStream communicateStream;

	rapidjson::Document communicateDoc;
	communicateDoc.SetObject();
	rapidjson::Document::AllocatorType& alloc = communicateDoc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	auto actUserId = UserInfo::getInstance()->getUserAccount();
	ctx.AddMember("act_user_id", actUserId.c_str(), alloc);
	ctx.AddMember("msg_head", "dzpk", alloc);
	ctx.AddMember("game_number", "dzpk", alloc);
	auto areaNumber = RoomData::getInstance()->getAreaNumber();
	ctx.AddMember("area_number", areaNumber.c_str(), alloc);
	ctx.AddMember("timer_id", "0", alloc);
	__String* roomNumber = __String::createWithFormat("%d", RoomData::getInstance()->getRoomInfo()->getRoomNumber());
	ctx.AddMember("room_number", roomNumber->getCString(), alloc);
	ctx.AddMember("act_commond", "370020", alloc);
	ctx.AddMember("chat_type", 1, alloc);
	auto userId = UserInfo::getInstance()->getUserId();
	__String* seatId = __String::createWithFormat("%d", RoomData::getInstance()->getPlayer((char *)userId.c_str())->getSeatNum());
	ctx.AddMember("seatId", seatId->getCString(), alloc);
	auto msgValueKey = __String::createWithFormat("communicateContent%d", cellIndex);
	ctx.AddMember("chat_msg", GlobalChineseXml::getInstance()->getValue((char *)msgValueKey->getCString()), alloc);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	ctx.Accept(writer);

	const char *jsonData = buffer.GetString();
			
	NetBean::getInstance()->addQQHead(&communicateStream);
	NetBean::getInstance()->addHead(&communicateStream, strlen(jsonData) + 1, 370020);
	NetBean::getInstance()->addData(&communicateStream, (char *)jsonData, strlen(jsonData));
	NetBean::getInstance()->write(communicateStream);
}

//shu ru kuang bei dian ji hou de hui diao fang fa
void GameScene::onCommunicateInputTouchedCallback(){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    //tan chu da de liao tian ceng
    if(this->getChildByTag(COMMUNICATE_LAYER_TAG) != NULL){
		this->removeChildByTag(COMMUNICATE_LAYER_TAG);
	}
    
    auto communicateInputLayer = CommunicateInputLayer::create();
    communicateInputLayer->setTag(COMMUNICATE_INPUT_LAYER_TAG);
    communicateInputLayer->setCommunicateInputDelegate(this);
    this->addChild(communicateInputLayer, 10);
}

//dian ji fa song cai dan hui diao fang fa
void GameScene::onCommunicateInputSendCallback(int type, std::string msg){
    if(this->getChildByTag(COMMUNICATE_INPUT_LAYER_TAG) != NULL){
		this->removeChildByTag(COMMUNICATE_INPUT_LAYER_TAG);
	}
    
    if(this->getChildByTag(SMILE_LAYER_TAG) != NULL){
		this->removeChildByTag(SMILE_LAYER_TAG);
	}
    
    CommunicateData::getInstance()->setInputStr("");
    
    //∑¢ÀÕ—°÷–µƒŒƒ◊÷œ˚œ¢∏¯∑˛ŒÒ∆˜
	//chat_type£∫¿‡–Õ£∫ 2£∫∂Øª≠  1:Œƒ◊÷
	CStream communicateStream;
    
	rapidjson::Document communicateDoc;
	communicateDoc.SetObject();
	rapidjson::Document::AllocatorType& alloc = communicateDoc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	auto actUserId = UserInfo::getInstance()->getUserAccount();
	ctx.AddMember("act_user_id", actUserId.c_str(), alloc);
	ctx.AddMember("msg_head", "dzpk", alloc);
	ctx.AddMember("game_number", "dzpk", alloc);
	auto areaNumber = RoomData::getInstance()->getAreaNumber();
	ctx.AddMember("area_number", areaNumber.c_str(), alloc);
	ctx.AddMember("timer_id", "0", alloc);
	__String* roomNumber = __String::createWithFormat("%d", RoomData::getInstance()->getRoomInfo()->getRoomNumber());
	ctx.AddMember("room_number", roomNumber->getCString(), alloc);
	ctx.AddMember("act_commond", "370020", alloc);
	ctx.AddMember("chat_type", type, alloc);
	auto userId = UserInfo::getInstance()->getUserId();
	__String* seatId = __String::createWithFormat("%d", RoomData::getInstance()->getPlayer((char *)userId.c_str())->getSeatNum());
	ctx.AddMember("seatId", seatId->getCString(), alloc);
	ctx.AddMember("chat_msg", msg.c_str(), alloc);
    
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	ctx.Accept(writer);
    
	const char *jsonData = buffer.GetString();
    
	NetBean::getInstance()->addQQHead(&communicateStream);
	NetBean::getInstance()->addHead(&communicateStream, strlen(jsonData) + 1, 370020);
	NetBean::getInstance()->addData(&communicateStream, (char *)jsonData, strlen(jsonData));
	NetBean::getInstance()->write(communicateStream);
}

//ªÊ÷∆”√ªß◊¥Ã¨£∫0£∫≈‘π€  1£∫◊¯œ¬(µ»¥˝œ¬“ªæ÷)   2£∫ º”◊¢ 3£∫∏˙◊¢ 4£∫ø¥≈∆   5:∆˙≈∆  6:»´œ¬  7£∫µ»¥˝œ¬◊¢£¨∏˘æ›”√ªß◊¥Ã¨ªÊ÷∆ ÷≈∆°¢∆˙≈∆µ»£¨∞¸¿®∏√◊˘Œª…œµƒÕÊº“ ÷…œ≥Ô¬Î
void GameScene::drawUserStatus(){
	int playerListNum = RoomData::getInstance()->getPlayerListNum();
	Vector<ROOM_USER_INFO_T*> playerList = RoomData::getInstance()->getPlayerList();
	int userStatus = 0;
	int seatNum = 0;
	long handChips = 0;

	//ªÊ÷∆ø’µƒ◊¥Ã¨±Í«©£¨”√”⁄∫Û√Ê∏¸–¬±Í«©ƒ⁄»›
	int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
	if(roomSeatNum == 5){
		for(int i = 0; i < 5; i++){
			//◊¥Ã¨±Í«©
			Label* statusLabel = Label::createWithSystemFont("", "Arial", 27);
			statusLabel->setScale(0.8f);
			statusLabel->setColor(Color3B(255, 255, 255));
			statusLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getStatusLabelCenterPosition(2 * i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getStatusLabelCenterPosition(2 * i + 1, 1));
            statusLabel->setDimensions(100, 27);
            statusLabel->setAlignment(TextHAlignment::CENTER);
			statusLabel->setTag(STATUS_LABEL_TAG + 2 * i + 1);
			this->addChild(statusLabel, 1);

			//∏√◊˘Œª…œµƒÕÊº“≥Ô¬Î±Í«©
			Label* handChipLabel = Label::createWithSystemFont("", "Arial", 27);
			handChipLabel->setScale(0.8f);
			handChipLabel->setColor(Color3B(255, 255, 255));
			handChipLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatHandChipCenterPosition(2 * i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatHandChipCenterPosition(2 * i + 1, 1));
			handChipLabel->setTag(SEAT_HAND_CHIP_TAG + 2 * i + 1);
			this->addChild(handChipLabel, 1);
		}
	}
	else{
		for(int i = 1; i <= 9; i++){
			//◊¥Ã¨±Í«©
			Label* statusLabel = Label::createWithSystemFont("", "Arial", 27);
			statusLabel->setScale(0.8f);
			statusLabel->setColor(Color3B(255, 255, 255));
			statusLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getStatusLabelCenterPosition(i, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getStatusLabelCenterPosition(i, 1));
            statusLabel->setDimensions(100, 27);
            statusLabel->setAlignment(TextHAlignment::CENTER);
			statusLabel->setTag(STATUS_LABEL_TAG + i);
			this->addChild(statusLabel, 1);

			//∏√◊˘Œª…œµƒÕÊº“≥Ô¬Î±Í«©
			Label* handChipLabel = Label::createWithSystemFont("", "Arial", 27);
			handChipLabel->setScale(0.8f);
			handChipLabel->setColor(Color3B(255, 255, 255));
			handChipLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatHandChipCenterPosition(i, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatHandChipCenterPosition(i, 1));
			handChipLabel->setTag(SEAT_HAND_CHIP_TAG + i);
			this->addChild(handChipLabel, 1);
		}
	}

	//ªÊ÷∆◊˘Œª◊¥Ã¨
	for(int i = 0; i < playerListNum; i++){
		userStatus = playerList.at(i)->getUserStatus();
		seatNum = playerList.at(i)->getSeatNum();
		handChips = playerList.at(i)->getHandChipsToShow();
		if(seatNum != 0){
			//ÕÊº“‘⁄ÕÊ≈∆◊¥Ã¨ ±≤≈ªÊ÷∆ ÷≈∆
			if(userStatus != 0 && userStatus != 1){
				RoomData::getInstance()->addCardSeat(seatNum);
			}

			showStatus(seatNum, userStatus);
			showSeatHandChip(seatNum, userStatus, handChips);
		}
	}
}

//ªÊ÷∆æﬂÃÂ◊˘Œªƒ≥“ª◊¥Ã¨
void GameScene::showStatus(int seatNum, int status){
	if(seatNum != 0){
		char* labelText = nullptr;
        std::string strUserAccount;
        std::string showLabel;
        ROOM_USER_INFO_T* player;
        
        int newSeatNum = seatNum;
        int tempSeatNum = seatNum + seatDelta;
        if(tempSeatNum <= 0){
            int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
            if(roomSeatNum == 5){
                newSeatNum = tempSeatNum + 9 + 1;
            }
            else{
                newSeatNum = tempSeatNum + 9;
            }
        }
        else{
            int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
            if(roomSeatNum == 5){
                if(tempSeatNum > 9){
                    newSeatNum = tempSeatNum % 9 - 1;
                }
                else{
                    newSeatNum = tempSeatNum;
                }
            }
            else{
                newSeatNum = tempSeatNum % 9;
                if(newSeatNum == 0){
                    newSeatNum = 9;
                }
            }
        }
        
        auto label = (Label *)this->getChildByTag(STATUS_LABEL_TAG + newSeatNum);
        if(label == NULL){
            log("statucLabel: %d NULL, oldSeat: %d", newSeatNum, seatNum);
            return;
        }
        
		switch(status){
		case 0:		//≈‘π€
			label->setString("");
			break;
		case 1:		//◊¯œ¬(µ»¥˝œ¬“ªæ÷), xian shi ni chen
//			labelText = GlobalChineseXml::getInstance()->getValue("waitNextTurn");
            player = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
            if(player != NULL){
                strUserAccount = player->getNickName();
//                if(strUserAccount.size() > 9){
//                    showLabel = strUserAccount.substr(0, 7);
//                    showLabel.push_back('.');
//                    showLabel.push_back('.');
//                    labelText = (char *)showLabel.c_str();
//                }
//                else{
                    labelText = (char *)strUserAccount.c_str();
//                }
            }
            else{
                labelText = "";
            }
            
			label->setString(labelText);
			break;
		case 2:		//º”◊¢
			labelText = GlobalChineseXml::getInstance()->getValue("addChip");
			label->setString(labelText);
			break;
		case 3:		//∏˙◊¢
			labelText = GlobalChineseXml::getInstance()->getValue("followChip");
			label->setString(labelText);
			break;
		case 4:		//ø¥≈∆
			labelText = GlobalChineseXml::getInstance()->getValue("lookCard");
			label->setString(labelText);
			break;
		case 5:		//∆˙≈∆
			labelText = GlobalChineseXml::getInstance()->getValue("throwCard");
			label->setString(labelText);
			break;
		case 6:		//»´œ¬
			labelText = GlobalChineseXml::getInstance()->getValue("allIn");
			label->setString(labelText);
			break;
		case 7:		//µ»¥˝œ¬◊¢£¨¥Û–°√§◊¢µƒœ‘ æ¥Û–°√§◊¢, xian shi ni chen
			int bigBlind = RoomData::getInstance()->getRoomInfo()->getBigBlind();
			int smallBlind = RoomData::getInstance()->getRoomInfo()->getSmallBlind();
			if(seatNum != bigBlind && seatNum != smallBlind){
//				labelText = GlobalChineseXml::getInstance()->getValue("waitAction");
                player = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
                if(player != NULL){
                    strUserAccount = player->getNickName();
//                    if(strUserAccount.size() > 9){
//                        showLabel = strUserAccount.substr(0, 7);
//                        showLabel.push_back('.');
//                        showLabel.push_back('.');
//                        labelText = (char *)showLabel.c_str();
//                    }
//                    else{
                        labelText = (char *)strUserAccount.c_str();
//                    }
                }
                else{
                    labelText = "";
                }
                
				label->setString(labelText);
			}
			break;
		}
	}
}

//ªÊ÷∆æﬂÃÂ◊˘ŒªÕÊº“ ÷…œ≥Ô¬Î
void GameScene::showSeatHandChip(int seatNum, int status, long chipValue){
	if(seatNum != 0){
        int newSeatNum = seatNum;
        int tempSeatNum = seatNum + seatDelta;
        if(tempSeatNum <= 0){
            int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
            if(roomSeatNum == 5){
                newSeatNum = tempSeatNum + 9 + 1;
            }
            else{
                newSeatNum = tempSeatNum + 9;
            }
        }
        else{
            int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
            if(roomSeatNum == 5){
                if(tempSeatNum > 9){
                    newSeatNum = tempSeatNum % 9 - 1;
                }
                else{
                    newSeatNum = tempSeatNum;
                }
            }
            else{
                newSeatNum = tempSeatNum % 9;
                if(newSeatNum == 0){
                    newSeatNum = 9;
                }
            }
        }
        
		if(status != 0){
			__String* chipText = nullptr;
			if(chipValue < 1000){
				chipText = __String::createWithFormat("%ld", chipValue);
			}
			else if(chipValue >= 1000 && chipValue < 1000000){
				chipText = __String::createWithFormat("%.1fK", chipValue / 1000.0);
			}
			else if(chipValue >= 1000000 && chipValue < 1000000000){
				chipText = __String::createWithFormat("%.1fM", chipValue / 1000000.0);
			}
			else if(chipValue >= 1000000000){
				chipText = __String::createWithFormat("%.1fB", chipValue / 1000000000.0);
			}

			((Label *)this->getChildByTag(SEAT_HAND_CHIP_TAG + newSeatNum))->setString(chipText->getCString());
		}
		else{	
			((Label *)this->getChildByTag(SEAT_HAND_CHIP_TAG + newSeatNum))->setString("");
		}
	}
}

//œ‘ æ¥Û–°√§◊¢°¢◊Øº“
void GameScene::drawBankerAndBlind(){
	int step = RoomData::getInstance()->getRoomInfo()->getStep();
	char* labelText = nullptr;
	int bigBlind = RoomData::getInstance()->getRoomInfo()->getBigBlind();
	int smallBlind = RoomData::getInstance()->getRoomInfo()->getSmallBlind();
    
    //hui zhi zhuang jia
    auto banker = this->getChildByTag(BANKER_SPRITE_TAG);
    int bankerSeatNum = RoomData::getInstance()->getRoomInfo()->getBanker();
    
    int newBankerSeatNum = bankerSeatNum;
    int tempBankerSeatNum = bankerSeatNum + seatDelta;
    if(tempBankerSeatNum <= 0){
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            newBankerSeatNum = tempBankerSeatNum + 9 + 1;
        }
        else{
            newBankerSeatNum = tempBankerSeatNum + 9;
        }
    }
    else{
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            if(tempBankerSeatNum > 9){
                newBankerSeatNum = tempBankerSeatNum % 9 - 1;
            }
            else{
                newBankerSeatNum = tempBankerSeatNum;
            }
        }
        else{
            newBankerSeatNum = tempBankerSeatNum % 9;
            if(newBankerSeatNum == 0){
                newBankerSeatNum = 9;
            }
        }
    }
    
    if(banker == NULL){
        if(bankerSeatNum > 0){
            Sprite* bankerSprite = Sprite::create("banker.png");
            bankerSprite->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getBankerPosition(newBankerSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getBankerPosition(newBankerSeatNum, 1)));
            bankerSprite->setTag(BANKER_SPRITE_TAG);
            this->addChild(bankerSprite, 1);
        }
    }
    else{
        if(bankerSeatNum > 0){
            banker->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getBankerPosition(newBankerSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getBankerPosition(newBankerSeatNum, 1)));
        }
    }

	if(step == 0){            //0£∫ Œ¥ø™ º
		return;
	}
	else if(step == 1){       //1£∫∑¢µ◊≈∆
		if(RoomData::getInstance()->getRoomInfo()->getBigBlind() > 0){    //–Ë“™œ‘ æ¥Û–°√§◊¢∫Õ◊Øº“
			labelText = GlobalChineseXml::getInstance()->getValue("bigBlind");
            int newBigBlindSeatNum = bigBlind;
            int tempBigBlindSeatNum = bigBlind + seatDelta;
            if(tempBigBlindSeatNum <= 0){
                newBigBlindSeatNum = tempBigBlindSeatNum + 9;
            }
            else{
                newBigBlindSeatNum = tempBigBlindSeatNum % 9;
                if(newBigBlindSeatNum == 0){
                    newBigBlindSeatNum = 9;
                }
            }
            auto bigBlindLabel = (Label *)this->getChildByTag(STATUS_LABEL_TAG + newBigBlindSeatNum);
            if(bigBlindLabel != NULL){
                bigBlindLabel->setString(labelText);
            }

            labelText = GlobalChineseXml::getInstance()->getValue("smallBlind");
            int newSmallBlindSeatNum = smallBlind;
            int tempSmallBlindSeatNum = smallBlind + seatDelta;
            if(tempSmallBlindSeatNum <= 0){
                newSmallBlindSeatNum = tempBigBlindSeatNum + 9;
            }
            else{
                newSmallBlindSeatNum = tempBigBlindSeatNum % 9;
                if(newSmallBlindSeatNum == 0){
                    newSmallBlindSeatNum = 9;
                }
            }
            auto smallBlindLabel = (Label *)this->getChildByTag(STATUS_LABEL_TAG + newSmallBlindSeatNum);
			if(smallBlindLabel != NULL){
                smallBlindLabel->setString(labelText);
            }
		}
	}
	else{					  //2:∑≠»˝’≈  3:∑≠µ⁄Àƒ’≈  4£∫∑≠◊Ó∫Û“ª’≈£®±»¥Û–°£©
//		labelText = GlobalChineseXml::getInstance()->getValue("bigBlind");
//		((LabelTTF *)this->getChildByTag(STATUS_LABEL_TAG + bigBlind))->setString(labelText);

//		labelText = GlobalChineseXml::getInstance()->getValue("smallBlind");
//		((LabelTTF *)this->getChildByTag(STATUS_LABEL_TAG + smallBlind))->setString(labelText);
	}
}

//ªÊ÷∆µπº∆ ±
void GameScene::drawSitProgress(){
	int step = RoomData::getInstance()->getRoomInfo()->getStep();
	if(step > 0){    //‘⁄ÕÊ≈∆
		int turn = RoomData::getInstance()->getRoomInfo()->getTurn();
		if(turn > 0){    //”–¬÷µΩÕÊº“≤Ÿ◊˜
            int newSeatNum = turn;
            int tempSeatNum = turn + seatDelta;
            if(tempSeatNum <= 0){
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    newSeatNum = tempSeatNum + 9 + 1;
                }
                else{
                    newSeatNum = tempSeatNum + 9;
                }
            }
            else{
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    if(tempSeatNum > 9){
                        newSeatNum = tempSeatNum % 9 - 1;
                    }
                    else{
                        newSeatNum = tempSeatNum;
                    }
                }
                else{
                    newSeatNum = tempSeatNum % 9;
                    if(newSeatNum == 0){
                        newSeatNum = 9;
                    }
                }
            }
            
			//µπº∆ ±∂Øª≠
            if(this->getChildByTag(SIT_PROGRESS_TAG + turn) != NULL && !this->getChildByTag(SIT_PROGRESS_TAG + turn)->isVisible()){
                this->getChildByTag(SIT_PROGRESS_TAG + turn)->setVisible(true);
                this->getChildByTag(SIT_PROGRESS_TAG + turn)->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1));
                ((ProgressTimer *)(this->getChildByTag(SIT_PROGRESS_TAG + turn)))->setPercentage(100.0f);
            }
            else{
                Sprite* sitProgress = Sprite::create("btn_sit_progress.png");
                ProgressTimer* setProgressTimer = ProgressTimer::create(sitProgress);
                setProgressTimer->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1));
                setProgressTimer->setTag(SIT_PROGRESS_TAG + turn);
                setProgressTimer->setReverseDirection(true);
                this->addChild(setProgressTimer);
                setProgressTimer->setPercentage(100.0f);
            }

            sitProgressTagTurn = turn;
			this->scheduleUpdate();
		}
	}
}

//ªÊ÷∆π´π≤≈∆£¨÷ª‘⁄≥ı ºªØµƒ ±∫Úµ˜”√£¨ÕÊ≈∆π˝≥Ã÷–µƒªÊ÷∆‘⁄œ˚œ¢œÏ”¶÷–Ω¯––
void GameScene::drawPublicCards(){
	//∏˘æ›step»∑∂®ªÊ÷∆º∏’≈π´π≤≈∆
	int step = RoomData::getInstance()->getRoomInfo()->getStep();
    log("DrawPublicCards->Step: %d", step);
	int cardNum = 0;
	int color = 0;
	int card = 0;
	int publicCardNum = 0;
	if (step == 2){		//∑≠»˝’≈
		publicCardNum = 3;
	}
	else if (step == 3){		//∑¢µ⁄Àƒ’≈
		publicCardNum = 4;
	}
	else if (step == 4){		//∑¢µ⁄ŒÂ’≈
		publicCardNum = 5;
	}
	else{		//ªπ√ª”–∑¢π´π≤≈∆
		publicCardNum = 0;
		return;
	}

	for (int i = 0; i < publicCardNum; i++){
		cardNum = RoomData::getInstance()->getRoomInfo()->deskCard[i];
        log("publicCardNum: %d", cardNum);
		color = cardNum / 100;
		card = cardNum % 100;

		__String* strCard = nullptr;
		if (color == 1){    //∑Ω∆¨
			strCard = __String::createWithFormat("p%dd.png", card);
		}
		else if (color == 2){    //√∑ª®
			strCard = __String::createWithFormat("p%dc.png", card);
		}
		else if (color == 3){    //∫ÏÃ“
			strCard = __String::createWithFormat("p%db.png", card);
		}
		else if (color == 4){    //∫⁄Ã“
			strCard = __String::createWithFormat("p%da.png", card);
		}
		else{	
			return;
		}
		auto cardSprite = Sprite::create(strCard->getCString());
		cardSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskCardPosition(i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskCardPosition(i + 1, 1));
		cardSprite->setScale(0.8f);
		cardSprite->setTag(PUBLIC_CARD_TAG + i);
		this->addChild(cardSprite);
	}
}

//ªÊ÷∆ ÷≈∆£¨≤ª¥¯∂Øª≠£¨”√”⁄Ω¯»Î∑øº‰ ±
void GameScene::drawHandCardsForEnterRoom(){
    if(RoomData::getInstance()->getRoomInfo()->getStep() == 4){         //biao shi ci shi fang jian zheng zai jie suan, dan wei kai ju, bu yao hui zhi shou pai
        if(RoomData::getInstance()->getRoomInfo()->deskCard[0] == 0){
            return;
        }
    }
    
	auto userId = UserInfo::getInstance()->getUserId();
	int userSeatNum = RoomData::getInstance()->getPlayer((char *)userId.c_str())->getSeatNum();
	int cardSeat = 0;
	int userCardNum = 0;
	int color = 0;
	int card = 0;
	for (int j = 0; j < 2; j++){
		for (int i = 0; i < RoomData::getInstance()->getCardSeatNumber().size(); i++){
            Sprite* handCard = nullptr;
            __String* strCard = nullptr;
            
			cardSeat = RoomData::getInstance()->getCardSeatNumber().at(i);
            
            int newSeatNum = cardSeat;
            int tempSeatNum = cardSeat + seatDelta;
            if(tempSeatNum <= 0){
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    newSeatNum = tempSeatNum + 9 + 1;
                }
                else{
                    newSeatNum = tempSeatNum + 9;
                }
            }
            else{
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    if(tempSeatNum > 9){
                        newSeatNum = tempSeatNum % 9 - 1;
                    }
                    else{
                        newSeatNum = tempSeatNum;
                    }
                }
                else{
                    newSeatNum = tempSeatNum % 9;
                    if(newSeatNum == 0){
                        newSeatNum = 9;
                    }
                }
            }

			//ÕÊº“◊‘º∫£¨–Ë“™œ‘ ææﬂÃÂµƒ ÷≈∆
			if (cardSeat == userSeatNum){
				//µ⁄“ª’≈ ÷≈∆
				userCardNum = RoomData::getInstance()->getHandCard(j + 1);
				color = userCardNum / 100;
				card = userCardNum % 100;

				if (color == 1){         //∑Ω∆¨
					strCard = __String::createWithFormat("p%dd.png", card);
				}
				else if (color == 2){    //√∑ª®
					strCard = __String::createWithFormat("p%dc.png", card);
				}
				else if (color == 3){    //∫ÏÃ“
					strCard = __String::createWithFormat("p%db.png", card);
				}
				else if (color == 4){    //∫⁄Ã“
					strCard = __String::createWithFormat("p%da.png", card);
				}

				handCard = Sprite::create(strCard->getCString());
			}
			else{		//œ‘ æ∆‰À¸ÕÊº“µƒ ÷≈∆
				handCard = Sprite::create("p0a.png");
			}

			handCard->setTag((HAND_CARD_TAG + cardSeat) * 2 + j);
            log("createHandCard: %d", cardSeat);
			handCard->setScale(0.6f);
			if (j == 0){
				handCard->setRotation(-13.0f);
			}
			else if (j == 1){
				handCard->setRotation(13.0f);
			}
            handCard->getTexture()->setAliasTexParameters();
			handCard->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 1));
			this->addChild(handCard, 1);
		}
	}
}

//ªÊ÷∆ ÷≈∆£¨¥¯∂Øª≠
void GameScene::drawHandCards(){
    //xian chuang jian shou pai, bing jiang shou pai yin chang, deng xiang ying wei zhi dong hua dao le de shi hou, xian shi shou pai
    auto userId = UserInfo::getInstance()->getUserId();
    int userSeatNum = RoomData::getInstance()->getPlayer((char *)userId.c_str())->getSeatNum();
    int cardSeat = 0;
    int userCardNum = 0;
    int color = 0;
    int card = 0;
    
    //bo fang shen yin
    for (int j = 0; j < 2; j++){
        for (int i = 0; i < RoomData::getInstance()->getCardSeatNumber().size(); i++){
            Sprite* handCard1 = nullptr;
            __String* strCard = nullptr;
            
            cardSeat = RoomData::getInstance()->getCardSeatNumber().at(i);
            
            int newSeatNum = cardSeat;
            int tempSeatNum = cardSeat + seatDelta;
            if(tempSeatNum <= 0){
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    newSeatNum = tempSeatNum + 9 + 1;
                }
                else{
                    newSeatNum = tempSeatNum + 9;
                }
            }
            else{
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    if(tempSeatNum > 9){
                        newSeatNum = tempSeatNum % 9 - 1;
                    }
                    else{
                        newSeatNum = tempSeatNum;
                    }
                }
                else{
                    newSeatNum = tempSeatNum % 9;
                    if(newSeatNum == 0){
                        newSeatNum = 9;
                    }
                }
            }
            
            log("drawHandCards->cardSeatCount: %lu", RoomData::getInstance()->getCardSeatNumber().size());
            log("drawHandCards->cardSeatNumber: %d", cardSeat);
            log("cardSeat: %d, userSeatNum: %d", cardSeat, userCardNum);
            
            //ÕÊº“◊‘º∫£¨–Ë“™œ‘ ææﬂÃÂµƒ ÷≈∆
            if (cardSeat == userSeatNum){
                //µ⁄“ª’≈ ÷≈∆
                userCardNum = RoomData::getInstance()->getHandCard(j + 1);
                color = userCardNum / 100;
                card = userCardNum % 100;
                
                if (color == 1){    //∑Ω∆¨
                    strCard = __String::createWithFormat("p%dd.png", card);
                }
                else if (color == 2){    //√∑ª®
                    strCard = __String::createWithFormat("p%dc.png", card);
                }
                else if (color == 3){    //∫ÏÃ“
                    strCard = __String::createWithFormat("p%db.png", card);
                }
                else if (color == 4){    //∫⁄Ã“
                    strCard = __String::createWithFormat("p%da.png", card);
                }
                
                handCard1 = Sprite::create(strCard->getCString());
            }
            else{		//œ‘ æ∆‰À¸ÕÊº“µƒ ÷≈∆
                handCard1 = Sprite::create("p0a.png");
            }
            
            handCard1->setTag((HAND_CARD_TAG + cardSeat) * 2 + j);
            log("createHandCard: %d", cardSeat);
            handCard1->setScale(0.6f);
            if (j == 0){
                handCard1->setRotation(-13.0f);
            }
            else if (j == 1){
                handCard1->setRotation(13.0f);
            }
            handCard1->getTexture()->setAliasTexParameters();
            handCard1->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 1));
            handCard1->setVisible(false);
            this->addChild(handCard1, 1);
        }
    }
    
    
    auto createHandCardNode = Node::create();
    createHandCardNode->setPosition(Vec2(0, 0));
    this->addChild(createHandCardNode);
    
    Vector<FiniteTimeAction*> createActionVector;

    for (int j = 0; j < 2; j++){
        for (int i = 0; i < RoomData::getInstance()->getCardSeatNumber().size(); i++){
            auto createFunc = CallFunc::create([=](){
                if(GlobalDef::getInstance()->getSoundOn()){
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("draw_hand_card.wav");
                }
                
                int tempCardSeat = RoomData::getInstance()->getCardSeatNumber().at(i);
                
                int newSeatNum = tempCardSeat;
                int tempSeatNum = tempCardSeat + seatDelta;
                if(tempSeatNum <= 0){
                    int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                    if(roomSeatNum == 5){
                        newSeatNum = tempSeatNum + 9 + 1;
                    }
                    else{
                        newSeatNum = tempSeatNum + 9;
                    }
                }
                else{
                    int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                    if(roomSeatNum == 5){
                        if(tempSeatNum > 9){
                            newSeatNum = tempSeatNum % 9 - 1;
                        }
                        else{
                            newSeatNum = tempSeatNum;
                        }
                    }
                    else{
                        newSeatNum = tempSeatNum % 9;
                        if(newSeatNum == 0){
                            newSeatNum = 9;
                        }
                    }
                }
                
                Sprite* tempHandCard = Sprite::create("p0a.png");
                tempHandCard->setScale(0.5f);
                tempHandCard->setPosition(visibleSize.width / 2 + 0, visibleSize.height / 2 + 100);
                this->addChild(tempHandCard, 1);
                
                auto moveToAction = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 1)));
                auto showHandAction = CallFunc::create([=](){
                    if(this->getChildByTag((HAND_CARD_TAG + tempCardSeat) * 2 + j) != NULL){
                        this->getChildByTag((HAND_CARD_TAG + tempCardSeat) * 2 + j)->setVisible(true);
                    }
                });
                auto removeTempHandCardAction = CallFunc::create([=](){
                    this->removeChild(tempHandCard);
                });
                
                tempHandCard->runAction(Sequence::create(moveToAction, showHandAction, removeTempHandCardAction, NULL));
            });
            auto delayAction = DelayTime::create(0.1f);
            
            createActionVector.pushBack(createFunc);
            createActionVector.pushBack(delayAction);
        }
    }
    
    auto removeNodeFunc = CallFunc::create([=](){
        this->removeChild(createHandCardNode);
    });
    createActionVector.pushBack(removeNodeFunc);
    
    createHandCardNode->runAction(Sequence::create(createActionVector));
}

//ªÊ÷∆ÕÊº“◊¿√Ê≥Ô¬Î£¨æ≤Ã¨œ‘ æ£¨µ´tag“™Õ≥“ª
void GameScene::showStaticDeskChip(int seatNum, long deskChip){
    int newSeatNum = seatNum;
    int tempSeatNum = seatNum + seatDelta;
    if(tempSeatNum <= 0){
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            newSeatNum = tempSeatNum + 9 + 1;
        }
        else{
            newSeatNum = tempSeatNum + 9;
        }
    }
    else{
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            if(tempSeatNum > 9){
                newSeatNum = tempSeatNum % 9 - 1;
            }
            else{
                newSeatNum = tempSeatNum;
            }
        }
        else{
            newSeatNum = tempSeatNum % 9;
            if(newSeatNum == 0){
                newSeatNum = 9;
            }
        }
    }
    
	if (this->getChildByTag(SEAT_CHIP_P_ST_TAG + seatNum) == NULL){
		Node* seatChipNode = Node::create();
		seatChipNode->setTag(SEAT_CHIP_P_ST_TAG + seatNum);
        log("create seatParent: %d", seatNum);
		seatChipNode->setPosition(0, 0);
		this->addChild(seatChipNode, 1);

		if (this->getChildByTag(STCP_LN_ST_TAG + seatNum) == NULL){
			Node* seatChipLabelNode = Node::create();
			seatChipLabelNode->setPosition(0, 0);
			seatChipLabelNode->setTag(STCP_LN_ST_TAG + seatNum);
			this->addChild(seatChipLabelNode);

			Sprite* seatChipLabelSprite = Sprite::create("desk_chip_label.png");
            seatChipLabelSprite->setTag(SEAT_CHIP_LABEL_SPRITE_BG_TAG + seatNum);
			seatChipLabelSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0) + 36.5, visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1));
			seatChipLabelNode->addChild(seatChipLabelSprite);

			Label* seatChipLabel = Label::createWithSystemFont("", "Arial", 27);
			seatChipLabel->setTag(STCP_L_ST_TAG + seatNum);
			seatChipLabel->setScale(0.65f);
			seatChipLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0) + 41.25, visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1));
			seatChipLabelNode->addChild(seatChipLabel, 1);
		}
	}

	long minBlind = RoomData::getInstance()->getRoomInfo()->getMinBlind();
	Sprite* seatChipChild = nullptr;
	if (deskChip < 5 * minBlind){
		seatChipChild = Sprite::create("chip_1.png");
	}
	else if (deskChip >= 5 * minBlind && deskChip < 10 * minBlind){
		seatChipChild = Sprite::create("chip_2.png");
	}
	else if (deskChip >= 10 * minBlind && deskChip < 20 * minBlind){
		seatChipChild = Sprite::create("chip_3.png");
	}
	else if (deskChip >= 20 * minBlind && deskChip < 50 * minBlind){
		seatChipChild = Sprite::create("chip_4.png");
	}
	else if (deskChip >= 50 * minBlind && deskChip < 100 * minBlind){
		seatChipChild = Sprite::create("chip_5.png");
	}
	else if (deskChip >= 100 * minBlind && deskChip < 200 * minBlind){
		seatChipChild = Sprite::create("chip_6.png");
	}
	else if (deskChip >= 200 * minBlind){
		seatChipChild = Sprite::create("chip_7.png");
	}

	seatChipChild->setTag(STDKCP_ST_TAG + seatNum);
	seatChipChild->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1));
	this->addChild(seatChipChild, 2);

	__String* chipText = nullptr;
	if (deskChip < 1000){
		chipText = __String::createWithFormat("%ld", deskChip);
	}
	else if (deskChip >= 1000 && deskChip < 1000000){
		chipText = __String::createWithFormat("%.1fK", deskChip / 1000.0);
	}
	else if (deskChip >= 1000000 && deskChip < 1000000000){
		chipText = __String::createWithFormat("%.1fM", deskChip / 1000000.0);
	}
	else if (deskChip >= 1000000000){
		chipText = __String::createWithFormat("%.1fB", deskChip / 1000000000.0);
	}
	((Label *)this->getChildByTag(STCP_LN_ST_TAG + seatNum)->getChildByTag(STCP_L_ST_TAG + seatNum))->setString(chipText->getCString());
}

//ªÊ÷∆ÕÊº“≤Ÿ◊˜∫ÛµƒÃÌº”≥Ô¬Î∂Øª≠£¨∏˘æ›∂™µΩ◊¿√Ê…œµƒ≥Ô¬Îº∆À„≥Ô¬Îµƒ∏ˆ ˝∫Õ√Ê÷µ
void GameScene::showDeskChip(int seatNum, long seatDeskChipValue, long deskChip){
    int newSeatNum = seatNum;
    int tempSeatNum = seatNum + seatDelta;
    if(tempSeatNum <= 0){
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            newSeatNum = tempSeatNum + 9 + 1;
        }
        else{
            newSeatNum = tempSeatNum + 9;
        }
    }
    else{
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            if(tempSeatNum > 9){
                newSeatNum = tempSeatNum % 9 - 1;
            }
            else{
                newSeatNum = tempSeatNum;
            }
        }
        else{
            newSeatNum = tempSeatNum % 9;
            if(newSeatNum == 0){
                newSeatNum = 9;
            }
        }
    }
    
	//∏˘æ›∏√¬÷œ¬◊¢µƒ≥Ô¬ÎªÊ÷∆œ‡”¶µƒ≥Ô¬Î∂Øª≠£¨∂Øª≠ÕÍ∫Û“∆≥˝∏√∂‘œÛ£¨≤¢∏˘æ›µ±«∞œ¬µƒ◊‹≥Ô¬ÎªÊ÷∆œ‡”¶µƒ≥Ô¬Îœ‘ æ‘⁄◊¿√Ê…œ
	long minBlind = RoomData::getInstance()->getRoomInfo()->getMinBlind();
	Sprite* chipChild = nullptr;
	if(seatDeskChipValue < 5 * minBlind){
		chipChild = Sprite::create("chip_1.png");
	}
	else if(seatDeskChipValue >= 5 * minBlind && seatDeskChipValue < 10 * minBlind){
		chipChild = Sprite::create("chip_2.png");
	}
	else if(seatDeskChipValue >= 10 * minBlind && seatDeskChipValue < 20 * minBlind){
		chipChild = Sprite::create("chip_3.png");
	}
	else if(seatDeskChipValue >= 20 * minBlind && seatDeskChipValue < 50 * minBlind){
		chipChild = Sprite::create("chip_4.png");
	}
	else if(seatDeskChipValue >= 50 * minBlind && seatDeskChipValue < 100 * minBlind){
		chipChild = Sprite::create("chip_5.png");
	}
	else if(seatDeskChipValue >= 100 * minBlind && seatDeskChipValue < 200 * minBlind){
		chipChild = Sprite::create("chip_6.png");
	}
	else if(seatDeskChipValue >= 200 * minBlind){
		chipChild = Sprite::create("chip_7.png");
	}

	chipChild->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1));
    this->addChild(chipChild);
    
	auto move = MoveTo::create(0.3f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1)));
	auto func = CallFunc::create([=](){
        //∏˘æ›◊¿√Êœ¬◊¢≥Ô¬ÎªÊ÷∆œ‡”¶µƒ≥Ô¬Î
        if (this->getChildByTag(SEAT_CHIP_P_ST_TAG + seatNum) == NULL){
            Node* seatChipNode = Node::create();
            seatChipNode->setTag(SEAT_CHIP_P_ST_TAG + seatNum);
            log("create seatParent: %d", seatNum);
            seatChipNode->setPosition(0, 0);
            this->addChild(seatChipNode, 1);
            
            if (this->getChildByTag(STCP_LN_ST_TAG + seatNum) == NULL){
                Node* seatChipLabelNode = Node::create();
                seatChipLabelNode->setPosition(0, 0);
                seatChipLabelNode->setTag(STCP_LN_ST_TAG + seatNum);
                this->addChild(seatChipLabelNode);
                
                Sprite* seatChipLabelSprite = Sprite::create("desk_chip_label.png");
                seatChipLabelSprite->setTag(SEAT_CHIP_LABEL_SPRITE_BG_TAG + seatNum);
                seatChipLabelSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0) + 36.5, visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1));
                seatChipLabelNode->addChild(seatChipLabelSprite);
                
                Label* seatChipLabel = Label::createWithSystemFont("", "Arial", 27);
                seatChipLabel->setTag(STCP_L_ST_TAG + seatNum);
                seatChipLabel->setScale(0.65f);
                seatChipLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0) + 41.25, visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1));
                seatChipLabelNode->addChild(seatChipLabel, 1);
            }
        }
        
		auto seatParent = this->getChildByTag(SEAT_CHIP_P_ST_TAG + seatNum);
		auto funcChange = CallFunc::create([=](){
            log("showDeskChip: funcChange");
            if (this->getChildByTag(STDKCP_ST_TAG + seatNum) == NULL){
				Sprite* seatChipChild = nullptr;
                if (deskChip < 5 * minBlind){
                    seatChipChild = Sprite::create("chip_1.png");
                }
                else if (deskChip >= 5 * minBlind && deskChip < 10 * minBlind){
                    seatChipChild = Sprite::create("chip_2.png");
                }
                else if (deskChip >= 10 * minBlind && deskChip < 20 * minBlind){
                    seatChipChild = Sprite::create("chip_3.png");
                }
                else if (deskChip >= 20 * minBlind && deskChip < 50 * minBlind){
                    seatChipChild = Sprite::create("chip_4.png");
                }
                else if (deskChip >= 50 * minBlind && deskChip < 100 * minBlind){
                    seatChipChild = Sprite::create("chip_5.png");
                }
                else if (deskChip >= 100 * minBlind && deskChip < 200 * minBlind){
                    seatChipChild = Sprite::create("chip_6.png");
                }
                else if (deskChip >= 200 * minBlind){
                    seatChipChild = Sprite::create("chip_7.png");
                }
                
                seatChipChild->setTag(STDKCP_ST_TAG + seatNum);
                seatChipChild->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1));
                this->addChild(seatChipChild, 2);
			}
            else{
                auto seatChipChild = (Sprite *)this->getChildByTag(STDKCP_ST_TAG + seatNum);
                if(!seatChipChild->isVisible()){
                    seatChipChild->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1));
                    seatChipChild->setVisible(true);
                }
                if (deskChip < 5 * minBlind){
                    seatChipChild->setTexture("chip_1.png");
                }
                else if (deskChip >= 5 * minBlind && deskChip < 10 * minBlind){
                    seatChipChild->setTexture("chip_2.png");
                }
                else if (deskChip >= 10 * minBlind && deskChip < 20 * minBlind){
                    seatChipChild->setTexture("chip_3.png");
                }
                else if (deskChip >= 20 * minBlind && deskChip < 50 * minBlind){
                    seatChipChild->setTexture("chip_4.png");
                }
                else if (deskChip >= 50 * minBlind && deskChip < 100 * minBlind){
                    seatChipChild->setTexture("chip_5.png");
                }
                else if (deskChip >= 100 * minBlind && deskChip < 200 * minBlind){
                    seatChipChild->setTexture("chip_6.png");
                }
                else if (deskChip >= 200 * minBlind){
                    seatChipChild->setTexture("chip_7.png");
                }

            }

			if (!this->getChildByTag(STCP_LN_ST_TAG + seatNum)->isVisible()){
				this->getChildByTag(STCP_LN_ST_TAG + seatNum)->setVisible(true);
			}

			__String* chipText = nullptr;
			if (deskChip < 1000){
				chipText = __String::createWithFormat("%ld", deskChip);
			}
			else if (deskChip >= 1000 && deskChip < 1000000){
				chipText = __String::createWithFormat("%.1fK", deskChip / 1000.0);
			}
			else if (deskChip >= 1000000 && deskChip < 1000000000){
				chipText = __String::createWithFormat("%.1fM", deskChip / 1000000.0);
			}
			else if (deskChip >= 1000000000){
				chipText = __String::createWithFormat("%.1fB", deskChip / 1000000000.0);
			}
			((Label *)(this->getChildByTag(STCP_LN_ST_TAG + seatNum)->getChildByTag(STCP_L_ST_TAG + seatNum)))->setString(chipText->getCString());
		});
		seatParent->runAction(Sequence::create(funcChange, NULL));
	});
    auto removeChipChildFunc = CallFunc::create([=](){
        this->removeChild(chipChild);
    });
    
	chipChild->runAction(Sequence::create(move, func, removeChipChildFunc, NULL));
}

//“˛≤ÿ≤Ÿ◊˜∞¥≈•
void GameScene::hideOperateMenu(){
	if(this->getChildByTag(SELF_OPERATE_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->isVisible()){
		if(this->getChildByTag(LABEL_FOLLOW_CHIP_TAG) != NULL && this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->isVisible()){
			this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->setVisible(false);
		}
		if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->isVisible()){
			this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->setVisible(false);
            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG))->setEnabled(false);
		}
		if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->isVisible()){
			this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->setVisible(false);
            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG))->setEnabled(false);
		}
		if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->isVisible()){
			this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->setVisible(false);
            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG))->setEnabled(false);
		}
		if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->isVisible()){
			this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->setVisible(false);
            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG))->setEnabled(false);
		}
		if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->isVisible()){
			this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->setVisible(false);
            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG))->setEnabled(false);
		}
		this->getChildByTag(SELF_OPERATE_TAG)->setVisible(false);
        ((Menu *)this->getChildByTag(SELF_OPERATE_TAG))->setEnabled(false);
	}
	else if(this->getChildByTag(OTHER_OPERATE_TAG) != NULL && !this->getChildByTag(OTHER_OPERATE_TAG)->isVisible()){
		//œ‘ æ∆‰À¸≤Ÿ◊˜∞¥≈•
        this->getChildByTag(OTHER_OPERATE_TAG)->setVisible(true);
        ((Menu *)this->getChildByTag(OTHER_OPERATE_TAG))->setEnabled(true);
        
        //œ‘ æπ¥
        if(RoomData::getInstance()->getIsLookOrLeastCard()){
            if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && !this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
                this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(true);
            }
        }
        if(RoomData::getInstance()->getIsAutoLookCard()){
            if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && !this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
                this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(true);
            }
        }
        if(RoomData::getInstance()->getIsFollowAnyChip()){
            if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && !this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
                this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(true);
            }
        }
	}
}

//œ‘ æ≤Ÿ◊˜∞¥≈•
void GameScene::showOperateMenu(int nextTurn){
	//œ‘ æ≤Ÿ◊˜∞¥≈•£¨∏˘æ› «∑Ò¬÷µΩÕÊº“◊‘º∫œ‘ æ≤ªÕ¨µƒ≤Ÿ◊˜ΩÁ√Ê
	auto userId = UserInfo::getInstance()->getUserId();
    auto userPlayer = RoomData::getInstance()->getPlayer((char *)userId.c_str());
    if(userPlayer != NULL){
        if(userPlayer->getSeatNum() == nextTurn){
            log("showOperateMenu: 1");
            //“˛≤ÿ∆‰À¸≤Ÿ◊˜∞¥≈•
            if(this->getChildByTag(OTHER_OPERATE_TAG) != NULL && this->getChildByTag(OTHER_OPERATE_TAG)->isVisible()){
                this->getChildByTag(OTHER_OPERATE_TAG)->setVisible(false);
                ((Menu *)this->getChildByTag(OTHER_OPERATE_TAG))->setEnabled(false);
                if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
                    this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(false);
                }
                if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
                    this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(false);
                }
                if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
                    this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(false);
                }
            }
            
            //œ‘ æÕÊº“◊‘…Ì≤Ÿ◊˜∞¥≈•
            if(this->getChildByTag(SELF_OPERATE_TAG) != NULL){
                this->getChildByTag(SELF_OPERATE_TAG)->setVisible(true);
                ((Menu *)this->getChildByTag(SELF_OPERATE_TAG))->setEnabled(true);
                if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->isVisible()){
                    this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->setVisible(false);
                    ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG))->setEnabled(false);
                }
                if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->isVisible()){
                    this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->setVisible(false);
                    ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG))->setEnabled(false);
                }
                if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->isVisible()){
                    this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->setVisible(false);
                    ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG))->setEnabled(false);
                    this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->setVisible(false);
                }
                if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->isVisible()){
                    this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->setVisible(false);
                    ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG))->setEnabled(false);
                }
                if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->isVisible()){
                    this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->setVisible(false);
                    ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG))->setEnabled(false);
                }
            }
            
            auto ctrlRights = RoomData::getInstance()->getCtrlRights();
            int ctrlRight = 0;
            for(int i = 0; i < ctrlRights.size(); i++){
                ctrlRight = ctrlRights.at(i);
                switch(ctrlRight){
                    case 0:					//0:º”◊¢
                    {
                        if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG) != NULL && !this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->isVisible()){
                            this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->setVisible(true);
                            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG))->setEnabled(true);
                        }
                    }
                        break;
                    case 1:					//1£∫∏˙◊¢
                    {
                        if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG) != NULL && !this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->isVisible()){
                            this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->setVisible(true);
                            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG))->setEnabled(true);
                            this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->setVisible(true);
                            __String* chipText = nullptr;
                            long chipValue = RoomData::getInstance()->getFollowChip();
                            if(chipValue < 1000){
                                chipText = __String::createWithFormat("%ld", chipValue);
                            }
                            else if(chipValue >= 1000 && chipValue < 1000000){
                                chipText = __String::createWithFormat("%.1fK", chipValue / 1000.0);
                            }
                            else if(chipValue >= 1000000 && chipValue < 1000000000){
                                chipText = __String::createWithFormat("%.1fM", chipValue / 1000000.0);
                            }
                            else if(chipValue >= 1000000000){
                                chipText = __String::createWithFormat("%.1fB", chipValue / 1000000000.0);
                            }
                            ((Label *)this->getChildByTag(LABEL_FOLLOW_CHIP_TAG))->setString(chipText->getCString());
                        }
                    }
                        break;
                    case 2:					//2£∫ø¥≈∆
                    {
                        if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG) != NULL && !this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->isVisible()){
                            this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->setVisible(true);
                            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG))->setEnabled(true);
                        }
                    }
                        break;
                    case 3:					//3£∫∆˙≈∆
                    {
                        if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG) != NULL && !this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->isVisible()){
                            this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->setVisible(true);
                            ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG))->setEnabled(true);
                        }
                    }
                        break;
                    default:
                        break;
                }
            }
        }
        else{
            auto userStatus = RoomData::getInstance()->getPlayer((char *)userId.c_str())->getUserStatus();
            log("userStatus: %d", userStatus);
            if(userStatus == 2 || userStatus == 3 || userStatus == 4 || userStatus == 7){
                log("showOperateMenu: 2");
                //“˛≤ÿÕÊº“≤Ÿ◊˜∞¥≈•
                if(this->getChildByTag(SELF_OPERATE_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->isVisible()){
                    this->getChildByTag(SELF_OPERATE_TAG)->setVisible(false);
                    if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->isVisible()){
                        this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->setVisible(false);
                        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG))->setEnabled(false);
                    }
                    if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->isVisible()){
                        this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->setVisible(false);
                        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG))->setEnabled(false);
                    }
                    if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->isVisible()){
                        this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->setVisible(false);
                        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG))->setEnabled(false);
                        this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->setVisible(false);
                    }
                    if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->isVisible()){
                        this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->setVisible(false);
                        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG))->setEnabled(false);
                    }
                    if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->isVisible()){
                        this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->setVisible(false);
                        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG))->setEnabled(false);
                    }
                }
                
                //œ‘ æ∆‰À¸≤Ÿ◊˜∞¥≈•
                if(this->getChildByTag(OTHER_OPERATE_TAG) != NULL && !this->getChildByTag(OTHER_OPERATE_TAG)->isVisible()){
                    this->getChildByTag(OTHER_OPERATE_TAG)->setVisible(true);
                    ((Menu *)this->getChildByTag(OTHER_OPERATE_TAG))->setEnabled(true);
                }
                
                //œ‘ æπ¥
                if(RoomData::getInstance()->getIsLookOrLeastCard()){
                    if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && !this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
                        this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(true);
                    }
                }
                if(RoomData::getInstance()->getIsAutoLookCard()){
                    if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && !this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
                        this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(true);
                    }
                }
                if(RoomData::getInstance()->getIsFollowAnyChip()){
                    if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && !this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
                        this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(true);
                    }
                }
            }
            else{
                log("showOperateMenu: 3");
                if(this->getChildByTag(SELF_OPERATE_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->isVisible()){
                    this->getChildByTag(SELF_OPERATE_TAG)->setVisible(false);
                    ((Menu *)this->getChildByTag(SELF_OPERATE_TAG))->setEnabled(false);
                }
                
                if(this->getChildByTag(LABEL_FOLLOW_CHIP_TAG) != NULL && this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->isVisible()){
                    this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->setVisible(false);
                }
                
                if(this->getChildByTag(OTHER_OPERATE_TAG) != NULL && this->getChildByTag(OTHER_OPERATE_TAG)->isVisible()){
                    this->getChildByTag(OTHER_OPERATE_TAG)->setVisible(false);
                    ((Menu *)this->getChildByTag(OTHER_OPERATE_TAG))->setEnabled(false);
                }
                
                if(RoomData::getInstance()->getIsLookOrLeastCard()){
                    if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
                        this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(false);
                    }
                }
                if(RoomData::getInstance()->getIsAutoLookCard()){
                    if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
                        this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(false);
                    }
                }
                if(RoomData::getInstance()->getIsFollowAnyChip()){
                    if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
                        this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(false);
                    }
                }
            }
        }
    }
}

void GameScene::hideAllOperateMenu(){
    if(this->getChildByTag(SELF_OPERATE_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->isVisible()){
        this->getChildByTag(SELF_OPERATE_TAG)->setVisible(false);
        ((Menu *)this->getChildByTag(SELF_OPERATE_TAG))->setEnabled(false);
    }
    
    if(this->getChildByTag(LABEL_FOLLOW_CHIP_TAG) != NULL && this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->isVisible()){
        this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->setVisible(false);
    }
    
    if(this->getChildByTag(OTHER_OPERATE_TAG) != NULL && this->getChildByTag(OTHER_OPERATE_TAG)->isVisible()){
        this->getChildByTag(OTHER_OPERATE_TAG)->setVisible(false);
        ((Menu *)this->getChildByTag(OTHER_OPERATE_TAG))->setEnabled(false);
    }
    
    if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
        this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(false);
    }
    if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
        this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(false);
    }
    if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
        this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(false);
    }
}

//ªÊ÷∆≤Ÿ◊˜∞¥≈•≤¢Ω´∆‰“˛≤ÿ£¨”√‘⁄”Œœ∑≥°æ∞≥ı ºªØ ±£¨∆‰À¸ ±∫Úøÿ÷∆≤Ÿ◊˜∞¥≈•“˛≤ÿ∫Õœ‘ æ
void GameScene::drawOperateMenuAndHide(){
	//ÃÌº”ÕÊº“◊‘…Ì≤Ÿ◊˜∞¥≈•
	MenuItemImage* menuAddChip = MenuItemImage::create("btn_adcp_up.png", "btn_adcp_down.png", CC_CALLBACK_1(GameScene::btAddChipCallback, this));
	menuAddChip->setAnchorPoint(Vec2(1, 0));
	menuAddChip->setPosition(visibleSize.width - 10, 5);
	menuAddChip->setTag(ADD_CHIP_TAG);

	MenuItemImage* menuOk = MenuItemImage::create("btn_ok_up.png", "btn_ok_down.png", CC_CALLBACK_1(GameScene::btMenuOkCallback, this));
	menuOk->setAnchorPoint(Vec2(1, 0));
	menuOk->setPosition(visibleSize.width - 10, 5);
	menuOk->setTag(OK_TAG);

	MenuItemImage* menuFollowChip = MenuItemImage::create("btn_fwcp_up.png", "btn_fwcp_down.png", CC_CALLBACK_1(GameScene::btFollowChipCallback, this));
	menuFollowChip->setAnchorPoint(Vec2(1, 0));
	menuFollowChip->setPosition(visibleSize.width - 190, 5);
	menuFollowChip->setTag(FOLLOW_CHIP_TAG);

	Label* labelFollowChip = Label::createWithSystemFont("", "Arial", 27);
	labelFollowChip->setAnchorPoint(Vec2(0.5, 0.5));
	labelFollowChip->setPosition(visibleSize.width - 190 - 50, 5 + 34);
	labelFollowChip->setTag(LABEL_FOLLOW_CHIP_TAG);
	this->addChild(labelFollowChip, 1);

	MenuItemImage* menuLookCard = MenuItemImage::create("btn_look_card_up.png", "btn_look_card_down.png", CC_CALLBACK_1(GameScene::btLookCardCallback, this));
	menuLookCard->setAnchorPoint(Vec2(1, 0));
	menuLookCard->setPosition(visibleSize.width - 190, 5);
	menuLookCard->setTag(LOOK_CARD_TAG);

	MenuItemImage* menuLeastCard = MenuItemImage::create("btn_least_card_up.png", "btn_least_card_down.png", CC_CALLBACK_1(GameScene::btLeastCardCallback, this));
	menuLeastCard->setAnchorPoint(Vec2(1, 0));
	menuLeastCard->setPosition(visibleSize.width - 370, 5);
	menuLeastCard->setTag(LEAST_CARD_TAG);
    
    Menu* selfOperateMenu = Menu::create(menuAddChip, menuOk, menuFollowChip, menuLookCard, menuLeastCard, NULL);
	selfOperateMenu->setPosition(0, 0);
	selfOperateMenu->setTag(SELF_OPERATE_TAG);
	this->addChild(selfOperateMenu);

	//“˛≤ÿÕÊº“◊‘…Ì≤Ÿ◊˜∞¥≈•
	this->getChildByTag(SELF_OPERATE_TAG)->setVisible(false);
	this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->setVisible(false);
	this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->setVisible(false);
	this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(FOLLOW_CHIP_TAG)->setVisible(false);
	this->getChildByTag(LABEL_FOLLOW_CHIP_TAG)->setVisible(false);
	this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LOOK_CARD_TAG)->setVisible(false);
	this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(LEAST_CARD_TAG)->setVisible(false);

	//ÃÌº”∆‰À¸≤Ÿ◊˜∞¥≈•
	MenuItemImage* menuLookOrLeastCard = MenuItemImage::create("btn_lkolt_cd_up.png", "btn_lkolt_cd_down.png", CC_CALLBACK_1(GameScene::btLookOrLeastCardCallback, this));
	menuLookOrLeastCard->setAnchorPoint(Vec2(1, 0));
	menuLookOrLeastCard->setPosition(visibleSize.width - 370, 5);

	MenuItemImage* menuAutoLookCard = MenuItemImage::create("btn_aolk_cd_up.png", "btn_aolk_cd_down.png", CC_CALLBACK_1(GameScene::btAutoLookCardCallback, this));
	menuAutoLookCard->setAnchorPoint(Vec2(1, 0));
	menuAutoLookCard->setPosition(visibleSize.width - 190, 5);

	MenuItemImage* menuFollowAnyChip = MenuItemImage::create("btn_fwaycp_up.png", "btn_fwaycp_down.png", CC_CALLBACK_1(GameScene::btFollowAnyChipCallback, this));
	menuFollowAnyChip->setAnchorPoint(Vec2(1, 0));
	menuFollowAnyChip->setPosition(visibleSize.width - 10, 5);
    
    Menu* otherOperateMenu = Menu::create(menuLookOrLeastCard, menuAutoLookCard, menuFollowAnyChip, NULL);
	otherOperateMenu->setTag(OTHER_OPERATE_TAG);
	otherOperateMenu->setPosition(0, 0);
	this->addChild(otherOperateMenu);

	//“˛≤ÿ∆‰À¸≤Ÿ◊˜∞¥≈•
	this->getChildByTag(OTHER_OPERATE_TAG)->setVisible(false);

	Sprite* lookOrLeastCardSprite = Sprite::create("right.png");
	lookOrLeastCardSprite->setTag(LOOK_OR_LEAST_CARD_TAG);
	lookOrLeastCardSprite->setAnchorPoint(Vec2(0.5, 0.5));
	lookOrLeastCardSprite->setPosition(visibleSize.width - 370 - 144, 5 + 33);
	this->addChild(lookOrLeastCardSprite);

	Sprite* autoLookCardSprite = Sprite::create("right.png");
	autoLookCardSprite->setTag(AUTO_LOOK_CARD_TAG);
	autoLookCardSprite->setAnchorPoint(Vec2(0.5, 0.5));
	autoLookCardSprite->setPosition(visibleSize.width - 190 - 144, 5 + 33);
	this->addChild(autoLookCardSprite);

	Sprite* followAnyChipSprite = Sprite::create("right.png");
	followAnyChipSprite->setTag(FOLLOW_ANY_CHIP_TAG);
	followAnyChipSprite->setAnchorPoint(Vec2(0.5, 0.5));
	followAnyChipSprite->setPosition(visibleSize.width - 10 - 144, 5 + 33);
	this->addChild(followAnyChipSprite);

	//“˛≤ÿ¥Úπ¥
	this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(false);
	this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(false);
	this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(false);
}

//ªÊ÷∆ƒ≥“ª∏ˆæﬂÃÂÕÊº“£¨‘›”√±æµÿÕº∆¨£¨∫Û∆⁄ π”√Õ¯¬ÁÕº∆¨
void GameScene::drawPlayer(ROOM_USER_INFO_T *userInfo){
	if(userInfo->getSeatNum() != 0){
        auto seatNum = userInfo->getSeatNum();
        
        //◊˘ŒªΩ⁄µ„
        int newSeatNum = seatNum;
        int tempSeatNum = seatNum + seatDelta;
        if(tempSeatNum <= 0){
            int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
            if(roomSeatNum == 5){
                newSeatNum = tempSeatNum + 9 + 1;
            }
            else{
                newSeatNum = tempSeatNum + 9;
            }
        }
        else{
            int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
            if(roomSeatNum == 5){
                if(tempSeatNum > 9){
                    newSeatNum = tempSeatNum % 9 - 1;
                }
                else{
                    newSeatNum = tempSeatNum;
                }
            }
            else{
                newSeatNum = tempSeatNum % 9;
                if(newSeatNum == 0){
                    newSeatNum = 9;
                }
            }
        }
        
        auto oldSeatNode = this->getChildByTag(seatNum);
        if(oldSeatNode == NULL){
            Node* seatNode = Node::create();
            seatNode->setTag(seatNum);
            
            //◊˘Œª±≥æ∞
            Sprite* seatBackground = Sprite::create("seat_background.png");
            seatBackground->setTag(PLAYER_PHOTO_BACKGROUND_TAG + seatNum);
            seatBackground->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1));
            seatNode->addChild(seatBackground);
            
            //Õ∑œÒ
            Sprite* player = Sprite::create("default_photo88.png");
            player->setTag(PLAYER_PHOTO_TAG + seatNum);
            player->setScale(0.9f);
            player->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1));
            seatNode->addChild(player);
            this->addChild(seatNode);
        }
        else{
            oldSeatNode->getChildByTag(PLAYER_PHOTO_BACKGROUND_TAG + seatNum)->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
            oldSeatNode->getChildByTag(PLAYER_PHOTO_TAG + seatNum)->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
            oldSeatNode->setVisible(true);
            ((Sprite *)oldSeatNode->getChildByTag(PLAYER_PHOTO_TAG + seatNum))->setTexture("default_photo88.png");
        }
        
        if(this->getChildByTag(PLAYER_SEAT_MENU_TAG + seatNum) == NULL){
            auto playerSeatMenuItem = MenuItemImage::create("player_seat_menu.png", "player_seat_menu.png", CC_CALLBACK_1(GameScene::btPlayerSeatMenuCallback, this));
            playerSeatMenuItem->setTag(PLAYER_SEAT_MENU_ITEM_TAG + seatNum);
            playerSeatMenuItem->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
            
            Menu* playerSeatMenu = Menu::create(playerSeatMenuItem, NULL);
            playerSeatMenu->setTag(PLAYER_SEAT_MENU_TAG + seatNum);
            playerSeatMenu->setPosition(Vec2(0, 0));
            this->addChild(playerSeatMenu, 1);
        }
        else{
            this->getChildByTag(PLAYER_SEAT_MENU_TAG + seatNum)->getChildByTag(PLAYER_SEAT_MENU_ITEM_TAG + seatNum)->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
            ((Menu *)this->getChildByTag(PLAYER_SEAT_MENU_TAG + seatNum))->setEnabled(true);
        }
        
        //Ω´”√ªßÕ¯¬ÁÕ∑œÒÃÌº”µΩspriteframecache
        HttpRequest* request = new HttpRequest();
        request->setUrl(userInfo->getHeadPhotoSerial().c_str());
        request->setRequestType(HttpRequest::Type::GET);
        request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpGetPhoto, this));
        
        auto strUserId = userInfo->getUserId();
        request->setTag(strUserId.c_str());
        HttpClient::getInstance()->send(request);
        request->release();
	}
}

//wan jia tou xiang bei dian ji, tan chu wan jia xin xi kuang
void GameScene::btPlayerSeatMenuCallback(Ref *object){
    int seatNum = ((MenuItemImage *)object)->getTag() - PLAYER_SEAT_MENU_ITEM_TAG;
    
    auto userInfo = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
    if(userInfo != NULL){
        auto seatNode = this->getChildByTag(seatNum);
        if(seatNode != NULL){
            auto player = (Sprite *)(seatNode->getChildByTag(PLAYER_PHOTO_TAG + seatNum));
            if(player != NULL){
                auto texture = player->getTexture();
                auto roomUserInfoLayer = RoomUserInfoLayer::create(seatNum, seatDelta, userInfo, texture);
                roomUserInfoLayer->setTag(ROOM_USER_INFO_LAYER_TAG + seatNum);
                roomUserInfoLayer->setSendChopMsgDelegate(this);
                this->addChild(roomUserInfoLayer, 10);
            }
        }
    }
}

//发送道具，道具数量为0时，弹出对话框提示
void GameScene::onSendChopMsg(int seatNum, int chopIndex){
    if(this->getChildByTag(ROOM_USER_INFO_LAYER_TAG + seatNum) != NULL){
        this->removeChildByTag(ROOM_USER_INFO_LAYER_TAG + seatNum);
    }
    
    log("onSendChopMsg: %d, %d", seatNum, chopIndex);
    
    CStream sendChopMsgStream;
    
    rapidjson::Document sendChopMsgDoc;
    sendChopMsgDoc.SetObject();
    rapidjson::Document::AllocatorType& alloc = sendChopMsgDoc.GetAllocator();
    rapidjson::Value ctx(rapidjson::kObjectType);
    
    auto areaNumber = RoomData::getInstance()->getAreaNumber();
    ctx.AddMember("area_number", areaNumber.c_str(), alloc);
    ctx.AddMember("act_commond", "370028", alloc);
    __String* strCmdType = nullptr;
    auto strUserId = UserInfo::getInstance()->getUserId();
    auto player = RoomData::getInstance()->getPlayer((char *)strUserId.c_str());
    if(player != NULL){
        auto userSeatNum = player->getSeatNum();
        switch(chopIndex){
            case 1:
                strCmdType = __String::createWithFormat("%d;%d;flower", userSeatNum, seatNum);
                break;
            case 2:
                strCmdType = __String::createWithFormat("%d;%d;egg", userSeatNum, seatNum);
                break;
            case 3:
                strCmdType = __String::createWithFormat("%d;%d;cheers", userSeatNum, seatNum);
                break;
            case 4:
                strCmdType = __String::createWithFormat("%d;%d;cake", userSeatNum, seatNum);
                break;
            case 5:
                strCmdType = __String::createWithFormat("%d;%d;bome", userSeatNum, seatNum);
                break;
            case 6:
                strCmdType = __String::createWithFormat("%d;%d;kiss", userSeatNum, seatNum);
                break;
            case 7:
                strCmdType = __String::createWithFormat("%d;%d;love", userSeatNum, seatNum);
                break;
            default:
                break;
        }
        ctx.AddMember("cmd_type", strCmdType->getCString(), alloc);
        ctx.AddMember("type", "1", alloc);
        __String* roomNum = __String::createWithFormat("%d", RoomData::getInstance()->getRoomInfo()->getRoomNumber());
        ctx.AddMember("room_number", roomNum->getCString(), alloc);
        ctx.AddMember("msg_head", "dzpk", alloc);
        ctx.AddMember("game_number", "dzpk", alloc);
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        ctx.Accept(writer);
        
        const char *jsonData = buffer.GetString();
        
        NetBean::getInstance()->addQQHead(&sendChopMsgStream);
        NetBean::getInstance()->addHead(&sendChopMsgStream, strlen(jsonData) + 1, 370028);
        NetBean::getInstance()->addData(&sendChopMsgStream, (char *)jsonData, strlen(jsonData));
        NetBean::getInstance()->write(sendChopMsgStream);
    }
}

//ªÊ÷∆»´≤ø◊¯œ¬ÕÊº“
void GameScene::drawAllInlinePlayers(){
    auto playerList = RoomData::getInstance()->getPlayerList();
	for(int i = 0; i < RoomData::getInstance()->getPlayerListNum(); i++){
		drawPlayer(playerList.at(i));
	}
}

//ª≠◊˘Œª
void GameScene::drawSeat(){
	int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
	Menu *seatMenu = Menu::create();
	seatMenu->setPosition(0, 0);
	seatMenu->setTag(SEAT_MENU_TAG);
	this->addChild(seatMenu);
	if(roomSeatNum == 5){
		for(int i = 0; i < 5; i++){
			MenuItemImage *seat = MenuItemImage::create("btn_sit_up.png", "btn_sit_down.png", CC_CALLBACK_1(GameScene::btSitDownCallback, this));
			seat->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(2 * i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(2 * i + 1, 1));
			seat->setTag(SEAT_START_TAG + 2 * i + 1);

			seatMenu->addChild(seat);
		}
	}
	else if(roomSeatNum == 9){
		for(int i = 1; i <= 9; i++){
			MenuItemImage *seat = MenuItemImage::create("btn_sit_up.png", "btn_sit_down.png", CC_CALLBACK_1(GameScene::btSitDownCallback,this));
			seat->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(i, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(i, 1));
			seat->setTag(SEAT_START_TAG + i);

			seatMenu->addChild(seat);
		}
	}
}

//“˛≤ÿªÚœ‘ æ◊˘Œª
void GameScene::setSeatVisible(){
	auto userId = UserInfo::getInstance()->getUserId();
	ROOM_USER_INFO_T *userInfo = RoomData::getInstance()->getPlayer((char *)userId.c_str());
	if(userInfo != NULL){
		if(userInfo->getUserStatus() == 0){
			this->getChildByTag(SEAT_MENU_TAG)->setVisible(true);
			int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
			Vector<ROOM_USER_INFO_T*> playerList = RoomData::getInstance()->getPlayerList();
			bool hasPlayer = false;
			if(roomSeatNum == 5){
				for(int i = 0; i < 5; i++){
					hasPlayer = false;
                    
                    int newSeatNum = 2 * i + 1;
                    int tempSeatNum = 2 * i + 1 + seatDelta;
                    if(tempSeatNum <= 0){
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            newSeatNum = tempSeatNum + 9 + 1;
                        }
                        else{
                            newSeatNum = tempSeatNum + 9;
                        }
                    }
                    else{
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            if(tempSeatNum > 9){
                                newSeatNum = tempSeatNum % 9 - 1;
                            }
                            else{
                                newSeatNum = tempSeatNum;
                            }
                        }
                        else{
                            newSeatNum = tempSeatNum % 9;
                            if(newSeatNum == 0){
                                newSeatNum = 9;
                            }
                        }
                    }
                    
					for(int j = 0; j < RoomData::getInstance()->getPlayerListNum(); j++){    //»Áπ˚∏√◊˘Œª…œ“—”–»À‘ÚÃ¯π˝
                        auto player = playerList.at(j);
                        if(player != NULL){
                            if(player->getSeatNum() == (2 * i + 1)){
                                hasPlayer = true;
                                if(this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum) != NULL && this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->isVisible()){
                                    this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->setVisible(false);
                                }
                                break;
                            }
                        }
					}
					if(hasPlayer == false){
						if (this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum) != NULL && !(this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->isVisible())){
							this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->setVisible(true);
						}
//						this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->setVisible(true);
					}
				}
			}
			else if(roomSeatNum == 9){
				for(int i = 1; i <= 9; i++){
					hasPlayer = false;
                    
                    int newSeatNum = i;
                    int tempSeatNum = i + seatDelta;
                    if(tempSeatNum <= 0){
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            newSeatNum = tempSeatNum + 9 + 1;
                        }
                        else{
                            newSeatNum = tempSeatNum + 9;
                        }
                    }
                    else{
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            if(tempSeatNum > 9){
                                newSeatNum = tempSeatNum % 9 - 1;
                            }
                            else{
                                newSeatNum = tempSeatNum;
                            }
                        }
                        else{
                            newSeatNum = tempSeatNum % 9;
                            if(newSeatNum == 0){
                                newSeatNum = 9;
                            }
                        }
                    }
                    
					for(int j = 0; j < RoomData::getInstance()->getPlayerListNum(); j++){    //»Áπ˚∏√◊˘Œª…œ“—”–»À‘ÚÃ¯π˝
                        auto player = playerList.at(j);
                        if(player != NULL){
                            if(player->getSeatNum() == i){
                                hasPlayer = true;
                                if(this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum) != NULL && this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->isVisible()){
                                    this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->setVisible(false);
                                }
                                break;
                            }
                        }
					}
					if(hasPlayer == false){
						if (this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum) != NULL && !(this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->isVisible())){
							this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->setVisible(true);
						}
//						this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + newSeatNum)->setVisible(true);
					}
				}
			}
		}
		else{	
			this->getChildByTag(SEAT_MENU_TAG)->setVisible(false);
		}
	}
}

//π∫¬Ú◊¯œ¬¥˙¿Ì∑Ω∑®
void GameScene::onBuyAndSit(int seatNum){
    if(this->getChildByTag(ADD_CHIP_LAYER_TAG) != NULL){
        this->removeChildByTag(ADD_CHIP_LAYER_TAG);
    }
	//∑¢ÀÕ◊¯œ¬√¸¡Ó∏¯∑˛ŒÒ∆˜
	sendSitDownMessage(seatNum);
}

 //πÿ±’¥˙¿Ì∑Ω∑®
void GameScene::onCloseAddChipLayer(){
    if(this->getChildByTag(ADD_CHIP_LAYER_TAG) != NULL){
        this->removeChildByTag(ADD_CHIP_LAYER_TAG);
    }
}

//πÿ±’≥Ô¬Î≤ª◊„∂‘ª∞øÚ
void GameScene::onCloseHasNotEnoughChopLayer(){
    if (this->getChildByTag(HAS_NOT_ENOUGH_CHOP_LAYER_TAG) != NULL){
		this->removeChildByTag(HAS_NOT_ENOUGH_CHOP_LAYER_TAG);
    }
}

//≥Ô¬Î≤ª◊„£¨¡¢º¥«∞Õ˘¥˙¿Ì∑Ω∑®
void GameScene::onHasNotEnoughChop(){
    if (this->getChildByTag(HAS_NOT_ENOUGH_CHOP_LAYER_TAG) != NULL){
		this->removeChildByTag(HAS_NOT_ENOUGH_CHOP_LAYER_TAG);
    }
    
	//Ã¯◊™µΩ≥‰÷µΩÁ√Ê
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
    MallLayer* mallLayer = MallLayer::create(MALL_INFO_FLAG::MALL_PROP);
    mallLayer->setMallLayerDelegate(this);
    mallLayer->setTag(MALL_LAYER_TAG);
	this->addChild(mallLayer, 10);
}

//πÿ±’≥Ô¬Î≤ª◊„∂‘ª∞øÚ
void GameScene::onCloseHasNotEnoughChipLayer(){
	if (this->getChildByTag(HAS_NOT_ENOUGH_CHIP_LAYER_TAG) != NULL){
		this->removeChildByTag(HAS_NOT_ENOUGH_CHIP_LAYER_TAG);
    }
}

//≥Ô¬Î≤ª◊„£¨¡¢º¥«∞Õ˘¥˙¿Ì∑Ω∑®
void GameScene::onHasNotEnoughChip(){
	if (this->getChildByTag(HAS_NOT_ENOUGH_CHIP_LAYER_TAG) != NULL){
		this->removeChildByTag(HAS_NOT_ENOUGH_CHIP_LAYER_TAG);
    }

	//Ã¯◊™µΩ≥‰÷µΩÁ√Ê
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
    MallLayer* mallLayer = MallLayer::create();
    mallLayer->setMallLayerDelegate(this);
    mallLayer->setTag(MALL_LAYER_TAG);
	this->addChild(mallLayer, 10);
}

//◊¯œ¬≤Àµ•ªÿµ˜∑Ω∑®£¨∑¢ÀÕ◊¯œ¬œ˚œ¢£¨µØ≥ˆ∂‘ª∞øÚ»√ÕÊº“…Ë÷√“ª–©–≈œ¢
void GameScene::btSitDownCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
	//»Áπ˚≥Ô¬Î≤ª◊„£¨µØ≥ˆ≥Ô¬Î≤ª◊„∂‘ª∞øÚ
	auto userId = UserInfo::getInstance()->getUserId();
	if(RoomData::getInstance()->getPlayer((char *)userId.c_str()) != NULL && RoomData::getInstance()->getPlayer((char *)userId.c_str())->getChipAccount() < RoomData::getInstance()->getRoomInfo()->getMinBuyChip()){
		HasNotEnoughChipLayer* hasNotEnoughChipLayer = HasNotEnoughChipLayer::create();
		hasNotEnoughChipLayer->setHasNotEnoughChipDelegate(this);
		hasNotEnoughChipLayer->setTag(HAS_NOT_ENOUGH_CHIP_LAYER_TAG);
		this->addChild(hasNotEnoughChipLayer, 10);
	}
	else{
		int seatNum = 5;
        seatNum = ((MenuItemImage*)object)->getTag() - SEAT_START_TAG;

		//µØ≥ˆ∂‘ª∞øÚ
		AddChipLayer* addChipLayer = AddChipLayer::create();
		addChipLayer->setSeatNum(seatNum);
		addChipLayer->setTag(ADD_CHIP_LAYER_TAG);
		addChipLayer->setAddChipDelegate(this);
		this->addChild(addChipLayer, 10);
	}
}

void GameScene::sendSitDownMessage(int seatNum){
	//“˛≤ÿ◊˘Œª
	setSeatVisible();

	auto userId = UserInfo::getInstance()->getUserId();
//	ROOM_USER_INFO_T *roomUserInfo = RoomData::getInstance()->getPlayer((char *)userId.c_str());

	//∑¢ÀÕ◊¯œ¬œ˚œ¢
	CStream sitdownStream;

	rapidjson::Document sitdownDoc;
	sitdownDoc.SetObject();
	rapidjson::Document::AllocatorType& alloc = sitdownDoc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("act_commond", "370002", alloc);
	__String* roomNumber = __String::createWithFormat("%d", RoomData::getInstance()->getRoomInfo()->getRoomNumber());
	ctx.AddMember("room_number", roomNumber->getCString(), alloc);
	if(RoomData::getInstance()->getIsAutoBuy()){
		__String* autoBuyChip = __String::createWithFormat("%ld", RoomData::getInstance()->getAutoBuyChip());
		ctx.AddMember("auto_buy", autoBuyChip->getCString(), alloc);
	}
	else{
		ctx.AddMember("auto_buy", "0", alloc);
	}
	ctx.AddMember("max_buy", "0", alloc);
	ctx.AddMember("msg_head", "dzpk", alloc);
	ctx.AddMember("timer_id", "0", alloc);
	auto actUserId = RoomData::getInstance()->getActUserId();
	ctx.AddMember("act_user_id", actUserId.c_str(), alloc);
	ctx.AddMember("game_number", "dzpk", alloc);
	auto areaNumber = RoomData::getInstance()->getAreaNumber();
	ctx.AddMember("area_number", areaNumber.c_str(), alloc);
	__String* seatNumber = __String::createWithFormat("%d", seatNum);
	ctx.AddMember("seat_number", seatNumber->getCString(), alloc);
	__String* strHandChips = __String::createWithFormat("%ld", RoomData::getInstance()->getAutoBuyChip());
	ctx.AddMember("hand_chips", strHandChips->getCString(), alloc);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	ctx.Accept(writer);

	const char *jsonData = buffer.GetString();
			
	NetBean::getInstance()->addQQHead(&sitdownStream);
	NetBean::getInstance()->addHead(&sitdownStream, strlen(jsonData) + 1, 370002);
	NetBean::getInstance()->addData(&sitdownStream, (char *)jsonData, strlen(jsonData));
	NetBean::getInstance()->write(sitdownStream);
}

//“˛≤ÿ»∑∂®≤Àµ•£¨œ‘ æº”◊¢≤Àµ•£¨“˛≤ÿº”◊¢ª¨∂ØÃı
void GameScene::hideOkAndShowAddChipMenu(){
	if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->isVisible()){
		this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->setVisible(false);
        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG))->setEnabled(false);
	}

	if(this->getChildByTag(SLIDER_NODE_TAG) != NULL){
        this->removeChildByTag(SLIDER_NODE_TAG, true);
    }

	if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG) != NULL && !this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->isVisible()){
		this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->setVisible(true);
        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG))->setEnabled(true);
	}
}

//ªÊ÷∆º”◊¢ª¨∂ØÃı
void GameScene::drawAddChipSlider(){
	if (this->getChildByTag(SLIDER_NODE_TAG) == NULL){
		Node* sliderNode = Node::create();
		sliderNode->setTag(SLIDER_NODE_TAG);
		sliderNode->setPosition(0, 0);
		this->addChild(sliderNode, 2);
		//ª¨∂ØÃı±≥æ∞
		Sprite* sliderBigBg = Sprite::create("slider_big_bg.png");
		sliderBigBg->setPosition(visibleSize.width - 95, 245);
		sliderNode->addChild(sliderBigBg);
		//ª¨∂ØÃı
		auto slider = ControlSlider::create("slider_bg.png", "slider_progress.png", "slider_thumb_up.png", "slider_thumb_down.png");
		slider->setTag(ADD_CHIP_SLIDER_TAG);
		slider->setRotation(-90);
		slider->setScale(0.8f);
		slider->setPosition(visibleSize.width - 95, 245);
		slider->setMinimumValue(RoomData::getInstance()->getFollowChip());				//◊Ó–°÷µ
		slider->setMaximumValue(RoomData::getInstance()->getMaxFollowChip());			//◊Ó¥Û÷µ
		slider->setValue(RoomData::getInstance()->getDefaultAddChip());					//Ω´ª¨∂ØÃıŒª÷√…Ë÷√µΩƒ¨»œº”◊¢÷µ
		sliderNode->addChild(slider);
		//ÃÌº”œ˚œ¢œÏ”¶£¨ª¨∂Ø ±µ˜’˚ ˝÷µøÚµƒŒª÷√
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(GameScene::valueChanged), Control::EventType::VALUE_CHANGED);

		Vec2 thumbSpritePosition = slider->getThumbSprite()->getPosition();
		float delta = (thumbSpritePosition.x / 318) * 35.25;
		// ˝÷µøÚ
		Sprite* numSprite = Sprite::create("slider_value_label.png");
		numSprite->setPosition(visibleSize.width - 85 - 97.75 - delta * 0.6, 245 - 130 + thumbSpritePosition.x * 0.8);
		numSprite->setTag(NUM_SPRITE_TAG);
		sliderNode->addChild(numSprite);
		// ˝÷µ±Í«©
		__String* addChip = __String::createWithFormat("%ld", RoomData::getInstance()->getDefaultAddChip());
		auto numLabel = Label::createWithSystemFont(addChip->getCString(), "Arial", 27);
		numLabel->setColor(Color3B(255, 255, 255));
		numLabel->setPosition(visibleSize.width - 85 - 103.75 - delta * 0.6, 245 - 126 + thumbSpritePosition.x * 0.8);
		numLabel->setTag(NUM_LABEL_TAG);
		sliderNode->addChild(numLabel);
		//AllIn
		MenuItemImage* menuItemAllIn = MenuItemImage::create("slider_allin_up.png", "slider_allin_down.png", CC_CALLBACK_1(GameScene::btAllInCallback, this));
		menuItemAllIn->setPosition(visibleSize.width - 95, 435);

		Menu* menuAllIn = Menu::create(menuItemAllIn, NULL);
		menuAllIn->setPosition(0, 0);
		sliderNode->addChild(menuAllIn);
	}
	else{		//–ﬁ∏ƒª¨∂ØÃı◊Ó–°÷µ°¢◊Ó¥Û÷µ°¢ ˝÷µ±Í«©ƒ¨»œœ‘ æ÷µ
		if (this->getChildByTag(SLIDER_NODE_TAG) != NULL && !this->getChildByTag(SLIDER_NODE_TAG)->isVisible()){
			this->getChildByTag(SLIDER_NODE_TAG)->setVisible(true);
			ControlSlider* slider = (ControlSlider *)this->getChildByTag(SLIDER_NODE_TAG)->getChildByTag(ADD_CHIP_SLIDER_TAG);
			slider->setMinimumValue(RoomData::getInstance()->getFollowChip());
			slider->setMaximumValue(RoomData::getInstance()->getMaxFollowChip());
			slider->setValue(RoomData::getInstance()->getDefaultAddChip());
			__String* addChip = __String::createWithFormat("%ld", RoomData::getInstance()->getDefaultAddChip());
			((Label *)this->getChildByTag(SLIDER_NODE_TAG)->getChildByTag(NUM_LABEL_TAG))->setString(addChip->getCString());
		}
	}
}

//º”◊¢≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btAddChipCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
	//“˛≤ÿº”◊¢≤Àµ•£¨œ‘ æ»∑∂®≤Àµ•£¨œ‘ æº”◊¢ª¨∂ØÃı
	if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG) != NULL && this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->isVisible()){
		this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG)->setVisible(false);
        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(ADD_CHIP_TAG))->setEnabled(false);
	}

	if(this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG) != NULL && !this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->isVisible()){
		this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG)->setVisible(true);
        ((MenuItemImage *)this->getChildByTag(SELF_OPERATE_TAG)->getChildByTag(OK_TAG))->setEnabled(true);
	}

	drawAddChipSlider();
}

//»∑∂®≤Àµ•ªÿµ˜∑Ω∑®£¨∑¢ÀÕº”◊¢œ˚œ¢∏¯∑˛ŒÒ∆˜
void GameScene::btMenuOkCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
    hideOperateMenu();

	int money = (int)((ControlSlider *)this->getChildByTag(SLIDER_NODE_TAG)->getChildByTag(ADD_CHIP_SLIDER_TAG))->getValue();
	sendOperateMessage(1, money);
    
    //“˛≤ÿ»∑∂®≤Àµ•£¨œ‘ æº”◊¢≤Àµ•
	hideOkAndShowAddChipMenu();
}

//∏˙◊¢≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btFollowChipCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
	//“˛≤ÿ»∑∂®≤Àµ•£¨œ‘ æº”◊¢≤Àµ•
	hideOkAndShowAddChipMenu();
    
    hideOperateMenu();

	sendOperateMessage(1, RoomData::getInstance()->getFollowChip());
}

//ø¥≈∆≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btLookCardCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
	//“˛≤ÿ»∑∂®≤Àµ•£¨œ‘ æº”◊¢≤Àµ•
	hideOkAndShowAddChipMenu();
    
    hideOperateMenu();

	sendOperateMessage(1, 0);
}

//∆˙≈∆≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btLeastCardCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
	//“˛≤ÿ»∑∂®≤Àµ•£¨œ‘ æº”◊¢≤Àµ•
	hideOkAndShowAddChipMenu();
    
    hideOperateMenu();

	sendOperateMessage(0, 0);
}

//ø¥≈∆ªÚ∆˙≈∆≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btLookOrLeastCardCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
	if(RoomData::getInstance()->getIsLookOrLeastCard()){    //“—¥Úπ¥
		RoomData::getInstance()->setIsLookOrLeastCard(false);
		//“˛≤ÿπ¥
		if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
			this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(false);
		}
	}
	else{    //Œ¥¥Úπ¥
		RoomData::getInstance()->setIsLookOrLeastCard(true);
		//œ‘ æπ¥
		if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && !this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
			this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(true);
		}
        
        if(RoomData::getInstance()->getIsAutoLookCard()){
            RoomData::getInstance()->setIsAutoLookCard(false);
            
            //“˛≤ÿπ¥
            if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
                this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(false);
            }
        }
        
        if(RoomData::getInstance()->getIsFollowAnyChip()){
            RoomData::getInstance()->setIsFollowAnyChip(false);
            //“˛≤ÿπ¥
            if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
                this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(false);
            }
        }
	}
}

//◊‘∂Øø¥≈∆≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btAutoLookCardCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
	if(RoomData::getInstance()->getIsAutoLookCard()){
		RoomData::getInstance()->setIsAutoLookCard(false);
		//“˛≤ÿπ¥
		if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
			this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(false);
		}
	}
	else{	
		RoomData::getInstance()->setIsAutoLookCard(true);
		//œ‘ æπ¥
		if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && !this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
			this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(true);
		}
        
        if(RoomData::getInstance()->getIsLookOrLeastCard()){    //“—¥Úπ¥
            RoomData::getInstance()->setIsLookOrLeastCard(false);
            //“˛≤ÿπ¥
            if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
                this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(false);
            }
        }
        
        if(RoomData::getInstance()->getIsFollowAnyChip()){
            RoomData::getInstance()->setIsFollowAnyChip(false);
            //“˛≤ÿπ¥
            if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
                this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(false);
            }
        }
	}
}

//∏˙»Œ∫Œ◊¢≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btFollowAnyChipCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
	if(RoomData::getInstance()->getIsFollowAnyChip()){
		RoomData::getInstance()->setIsFollowAnyChip(false);
		//“˛≤ÿπ¥
		if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
			this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(false);
		}
	}
	else{	
		RoomData::getInstance()->setIsFollowAnyChip(true);
		//œ‘ æπ¥
		if(this->getChildByTag(FOLLOW_ANY_CHIP_TAG) != NULL && !this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->isVisible()){
			this->getChildByTag(FOLLOW_ANY_CHIP_TAG)->setVisible(true);
		}
        
        if(RoomData::getInstance()->getIsLookOrLeastCard()){    //“—¥Úπ¥
            RoomData::getInstance()->setIsLookOrLeastCard(false);
            //“˛≤ÿπ¥
            if(this->getChildByTag(LOOK_OR_LEAST_CARD_TAG) != NULL && this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->isVisible()){
                this->getChildByTag(LOOK_OR_LEAST_CARD_TAG)->setVisible(false);
            }
        }
        
        if(RoomData::getInstance()->getIsAutoLookCard()){
            RoomData::getInstance()->setIsAutoLookCard(false);
            //“˛≤ÿπ¥
            if(this->getChildByTag(AUTO_LOOK_CARD_TAG) != NULL && this->getChildByTag(AUTO_LOOK_CARD_TAG)->isVisible()){
                this->getChildByTag(AUTO_LOOK_CARD_TAG)->setVisible(false);
            }
        }
	}
}

//AllIn≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btAllInCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
    hideOperateMenu();

	//µ„ª˜AllIn≤Àµ•£¨∑¢ÀÕº”◊¢œ˚œ¢∏¯∑˛ŒÒ∆˜£¨º”◊¢◊Ó¥Û÷µ
	long allInValue = RoomData::getInstance()->getMaxFollowChip();

	sendOperateMessage(1, allInValue);
    
    hideOkAndShowAddChipMenu();
}

//ª¨∂ØÃıª¨∂Øœ˚œ¢œÏ”¶
void GameScene::valueChanged(Ref* object, Control::EventType type){
	ControlSlider* pSlider = (ControlSlider *)object;
	Vec2 thumbSpritePosition = pSlider->getThumbSprite()->getPosition();
	__String* strValue = __String::createWithFormat("%d", (int)pSlider->getValue());
	((Label *)(this->getChildByTag(SLIDER_NODE_TAG)->getChildByTag(NUM_LABEL_TAG)))->setString(strValue->getCString());
	float delta = (thumbSpritePosition.x / 318) * 35.25;
	this->getChildByTag(SLIDER_NODE_TAG)->getChildByTag(NUM_SPRITE_TAG)->setPosition(visibleSize.width -85 - 97.75 - delta * 0.6, 260 - 130 + thumbSpritePosition.x * 0.8);
	this->getChildByTag(SLIDER_NODE_TAG)->getChildByTag(NUM_LABEL_TAG)->setPosition(visibleSize.width - 85 - 103.75 - delta * 0.6, 260 - 126 + thumbSpritePosition.x * 0.8);
}

//∑¢ÀÕ∏˙◊¢°¢º”◊¢°¢ø¥≈∆°¢∆˙≈∆œ˚œ¢∏¯∑˛ŒÒ∆˜
void GameScene::sendOperateMessage(int ctrlRight, long betMoney){
	auto userId = UserInfo::getInstance()->getUserId();
	ROOM_USER_INFO_T *userInfo = RoomData::getInstance()->getPlayer((char *)userId.c_str());
	if(userInfo != NULL){
		CStream operateStream;

		rapidjson::Document operateDoc;
		operateDoc.SetObject();
		rapidjson::Document::AllocatorType& alloc = operateDoc.GetAllocator();
		rapidjson::Value ctx(rapidjson::kObjectType);
		auto actUserId = RoomData::getInstance()->getActUserId();
		ctx.AddMember("act_user_id", actUserId.c_str(), alloc);
		__String* roomNum = __String::createWithFormat("%d", userInfo->getRoomNum());
		ctx.AddMember("room_number", roomNum->getCString(), alloc);
		__String* seatNum = __String::createWithFormat("%d", userInfo->getSeatNum());
		ctx.AddMember("seat_number", seatNum->getCString(), alloc);
		ctx.AddMember("act_commond", "370003", alloc);
		ctx.AddMember("msg_head", "dzpk", alloc);
		__String* ctrlType = __String::createWithFormat("%d", ctrlRight);
		ctx.AddMember("ctl_type", ctrlType->getCString(), alloc);
		ctx.AddMember("game_number", "dzpk", alloc);
		__String* money = __String::createWithFormat("%ld", betMoney);
		ctx.AddMember("betMoney", money->getCString(), alloc);
		auto timerId = RoomData::getInstance()->getTimerId();
		ctx.AddMember("timer_id", timerId.c_str(), alloc);
		auto areaNumber = RoomData::getInstance()->getAreaNumber();
		ctx.AddMember("area_number", areaNumber.c_str(), alloc);
			
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		ctx.Accept(writer);

		const char *jsonData = buffer.GetString();
			
		NetBean::getInstance()->addQQHead(&operateStream);
		NetBean::getInstance()->addHead(&operateStream, strlen(jsonData) + 1, 370003);
		NetBean::getInstance()->addData(&operateStream, (char *)jsonData, strlen(jsonData));
		NetBean::getInstance()->write(operateStream);
	}
}

// ’ºØ◊¿√Êœ¬◊¢≥Ô¬ÎµΩ“ª∆
void GameScene::collectDeskChip(int roomSeatNum){
	//≈–∂œ◊¿√Ê…œ «∑Ò”–ÕÊº“deskChip≤ªŒ™¡„£¨»Áπ˚≤ªŒ™¡„£¨‘Ú ’ºØ≥Ô¬Î£¨∑Ò‘Ú≤ª ’ºØ≥Ô¬Î
	bool playerHasDeskChip = false;
	for(int i = 1; i <= 9; i++){
		if(RoomData::getInstance()->getPlayerBySeatNum(i) != NULL && RoomData::getInstance()->getPlayerBySeatNum(i)->getDeskChip() != 0){
			playerHasDeskChip = true; 
			break;
		}
	}
    
    //yan chi zui shao 0.35s, yi mian xia zhu dong hua hai mei jie su, jiu xian sou ji chou ma
    auto collectNode = Node::create();
    collectNode->setPosition(0, 0);
    this->addChild(collectNode);
    auto delayTime = DelayTime::create(0.35f);
    auto collectFunc = CallFunc::create([=](){
        if(playerHasDeskChip){
            //√ø∏ˆ»À◊¿√Ê…œµƒdeskChip«Â¡„
            for(int i = 1; i <= 9; i++){
                if(RoomData::getInstance()->getPlayerBySeatNum(i) != NULL){
                    RoomData::getInstance()->getPlayerBySeatNum(i)->setDeskChip(0);
                }
            }
            
            for(int i = 1; i <= 9; i++){
                auto seatParent = this->getChildByTag(SEAT_CHIP_P_ST_TAG + i);
                if(seatParent != NULL){
                    log("seatParent: %d", i);
                    Node* node = Node::create();
                    node->setPosition(0, 0);
                    this->addChild(node);
                    
                    DelayTime* delayAction = DelayTime::create(0.2f);
                    auto moveFunc = CallFunc::create([=](){
                        CallFunc* preFunc = CallFunc::create([=](){
                            this->getChildByTag(STCP_LN_ST_TAG + i)->setVisible(false);
                        });
                        auto moveAction = MoveTo::create(0.3f, Vec2(visibleSize.width / 2, visibleSize.height / 2 + 125 + (i - 1) * 2));
                        DelayTime* nextDelayAction = DelayTime::create(0.2f);
                        auto nextFuncAction = CallFunc::create([=](){
                            //“∆≥˝∂‘œÛ
                            this->removeChildByTag(SEAT_CHIP_P_ST_TAG + i);
                            log("remove seatParent: %d", i);
                            if (this->getChildByTag(STDKCP_ST_TAG + i) != NULL && this->getChildByTag(STDKCP_ST_TAG + i)->isVisible()){
                                this->getChildByTag(STDKCP_ST_TAG + i)->setVisible(false);
                            }
                        });
                        
                        this->getChildByTag(STDKCP_ST_TAG + i)->runAction(Sequence::create(preFunc, moveAction, nextDelayAction, nextFuncAction, NULL));
                    });
                    CallFunc* removeNodeFunc = CallFunc::create([=](){
                        this->removeChild(node);
                    });
                    
                    node->runAction(Sequence::create(delayAction, moveFunc, removeNodeFunc, NULL));
                }
            }
        }
    });
    auto removeFunc = CallFunc::create([=](){
        this->removeChild(collectNode);
    });
	
    collectNode->runAction(Sequence::create(delayTime, collectFunc, removeFunc, NULL));
}

//ªÊ÷∆≥Ô¬Î≥ÿ£¨æ≤Ã¨œ‘ æ£¨µ´tag“™Õ≥“ª
void GameScene::drawStaticPool(int id, long poolValue){
	if(this->getChildByTag(DESK_POOL_START_TAG + id) == NULL){
		Node* deskPoolParent = Node::create();
		deskPoolParent->setPosition(0, 0);
		deskPoolParent->setTag(DESK_POOL_START_TAG + id);
		this->addChild(deskPoolParent, 1);

		//≥Ô¬Î≥ÿ±Í«©
		if (this->getChildByTag(DKPL_LN_ST_TAG + id) == NULL){
			Node* deskPoolLabelNode = Node::create();
			deskPoolLabelNode->setPosition(0, 0);
			deskPoolLabelNode->setTag(DKPL_LN_ST_TAG + id);
			this->addChild(deskPoolLabelNode);

			Sprite* deskPoolLabelSprite = Sprite::create("desk_chip_label.png");
			deskPoolLabelSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 0) + 36.5, visibleSize.height / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 1));
			deskPoolLabelNode->addChild(deskPoolLabelSprite);

			Label* deskPoolLabel = Label::createWithSystemFont("", "Arial", 27);
			deskPoolLabel->setTag(DKPL_L_ST_TAG + id);
			deskPoolLabel->setScale(0.65f);
			deskPoolLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 0) + 41.25, visibleSize.height / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 1));
			deskPoolLabelNode->addChild(deskPoolLabel, 1);
		}
	}

	long minBlind = RoomData::getInstance()->getRoomInfo()->getMinBlind();
	auto poolParent = this->getChildByTag(DESK_POOL_START_TAG + id);
    int poolChildCount = poolParent->getChildrenCount();    //dang qian yi jing xian shi de chou ma dui xiang ge shu
    int needChildCount = 0;    //xu yao xian shi de chou ma dui xiang ge shu
    if(poolValue < 5 * minBlind){
        needChildCount = 1;
    }
    else if(poolValue >= 5 * minBlind && poolValue < 10 * minBlind){
        needChildCount = 3;
    }
    else if(poolValue >= 10 * minBlind){
        needChildCount = 5;
    }
    
    if(needChildCount > poolChildCount){
        for(int i = poolChildCount; i < needChildCount; i++){
            auto child = Sprite::create();
            child->setTag(i);
			child->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 1) + i * 3);
			poolParent->addChild(child);
        }
    }
	
	if(poolValue < 5 * minBlind){
        ((Sprite *)poolParent->getChildByTag(0))->setTexture("chip_1.png");
	}
	else if(poolValue >= 5 * minBlind && poolValue < 10 * minBlind){
		for(int i = 0; i < 2; i++){
            ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_1.png");
		}
        ((Sprite *)poolParent->getChildByTag(2))->setTexture("chip_2.png");
	}
	else if(poolValue >= 10 * minBlind && poolValue < 20 * minBlind){
		for(int i = 0; i < 2; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_1.png");
		}

		for(int i = 2; i < 4; i++){
            ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_2.png");
		}
        ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_3.png");
	}
	else if(poolValue >= 20 * minBlind && poolValue < 50 * minBlind){
		for(int i = 0; i < 2; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_2.png");
		}

		for(int i = 2; i < 4; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_3.png");
		}
        ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_4.png");
	}
	else if(poolValue >= 50 * minBlind && poolValue < 100 * minBlind){
		for(int i = 0; i < 2; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_3.png");
		}

		for(int i = 2; i < 4; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_4.png");
		}
        ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_5.png");
	}
	else if(poolValue >= 100 * minBlind && poolValue < 200 * minBlind){
		for(int i = 0; i < 2; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_4.png");
		}

		for(int i = 2; i < 4; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_5.png");
		}
        ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_6.png");
	}
	else if(poolValue >= 200 * minBlind){
		for(int i = 0; i < 2; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_5.png");
		}

		for(int i = 2; i < 4; i++){
			((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_6.png");
		}
        ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_7.png");
	}

	//≥Ô¬Î≥ÿ±Í«©
	if (!this->getChildByTag(DKPL_LN_ST_TAG + id)->isVisible()){
		this->getChildByTag(DKPL_LN_ST_TAG + id)->setVisible(true);
	}

	__String* deskPoolText = nullptr;
	if (poolValue < 1000){
		deskPoolText = __String::createWithFormat("%ld", poolValue);
	}
	else if (poolValue >= 1000 && poolValue < 1000000){
		deskPoolText = __String::createWithFormat("%.1fK", poolValue / 1000.0);
	}
	else if (poolValue >= 1000000 && poolValue < 1000000000){
		deskPoolText = __String::createWithFormat("%.1fM", poolValue / 1000000.0);
	}
	else if (poolValue >= 1000000000){
		deskPoolText = __String::createWithFormat("%.1fB", poolValue / 1000000000.0);
	}
	((Label *)this->getChildByTag(DKPL_LN_ST_TAG + id)->getChildByTag(DKPL_L_ST_TAG + id))->setString(deskPoolText->getCString());
}

//ªÊ÷∆≥Ô¬ÎπÈ≥ÿ∂Øª≠£¨idŒ™≥ÿ±‡∫≈0~6£¨poolValueŒ™≥ÿ≥Ô¬Î ˝
void GameScene::drawPoolAnimation(int id, long poolValue){
	Node* node = Node::create();
	node->setPosition(0, 0);
	this->addChild(node);
	DelayTime* delayAction = DelayTime::create(0.9f);
	auto moveFunc = CallFunc::create([=](){
		long minBlind = RoomData::getInstance()->getRoomInfo()->getMinBlind();
		Sprite* tempPool = nullptr;
		if(poolValue < 5 * minBlind){
			tempPool = Sprite::create("chip_1.png");
		}
		else if(poolValue >= 5 * minBlind && poolValue < 10 * minBlind){
			tempPool = Sprite::create("chip_2.png");
		}
		else if(poolValue >= 10 * minBlind && poolValue < 20 * minBlind){
			tempPool = Sprite::create("chip_3.png");
		}
		else if(poolValue >= 20 * minBlind && poolValue < 50 * minBlind){
			tempPool = Sprite::create("chip_4.png");
		}
		else if(poolValue >= 50 * minBlind && poolValue < 100 * minBlind){
			tempPool = Sprite::create("chip_5.png");
		}
		else if(poolValue >= 100 * minBlind && poolValue < 200 * minBlind){
			tempPool = Sprite::create("chip_6.png");
		}
		else if(poolValue >= 200 * minBlind){
			tempPool = Sprite::create("chip_7.png");
		}
		tempPool->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 140);
		this->addChild(tempPool);
		auto moveAction = MoveTo::create(0.3f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 1)));
		auto funcAction = CallFunc::create([=](){
			this->removeChild(tempPool);

			if(this->getChildByTag(DESK_POOL_START_TAG + id) == NULL){
				Node* deskPoolParent = Node::create();
				deskPoolParent->setPosition(0, 0);
				deskPoolParent->setTag(DESK_POOL_START_TAG + id);
				this->addChild(deskPoolParent, 1);

				//≥Ô¬Î≥ÿ±Í«©
				if (this->getChildByTag(DKPL_LN_ST_TAG + id) == NULL){
					Node* deskPoolLabelNode = Node::create();
					deskPoolLabelNode->setPosition(0, 0);
					deskPoolLabelNode->setTag(DKPL_LN_ST_TAG + id);
					this->addChild(deskPoolLabelNode);

					Sprite* deskPoolLabelSprite = Sprite::create("desk_chip_label.png");
					deskPoolLabelSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 0) + 36.5, visibleSize.height / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 1));
					deskPoolLabelNode->addChild(deskPoolLabelSprite);

					Label* deskPoolLabel = Label::createWithSystemFont("", "Arial", 27);
					deskPoolLabel->setTag(DKPL_L_ST_TAG + id);
					deskPoolLabel->setScale(0.65f);
					deskPoolLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 0) + 41.25, visibleSize.height / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 1));
					deskPoolLabelNode->addChild(deskPoolLabel, 1);
				}
			}

			auto poolParent = this->getChildByTag(DESK_POOL_START_TAG + id);
            
			auto funcChange = CallFunc::create([=](){
                int poolChildCount = poolParent->getChildrenCount();    //dang qian yi jing xian shi de chou ma dui xiang ge shu
                int needChildCount = 0;    //xu yao xian shi de chou ma dui xiang ge shu
                if(poolValue < 5 * minBlind){
                    needChildCount = 1;
                }
                else if(poolValue >= 5 * minBlind && poolValue < 10 * minBlind){
                    needChildCount = 3;
                }
                else if(poolValue >= 10 * minBlind){
                    needChildCount = 5;
                }
                
                if(needChildCount > poolChildCount){
                    for(int i = poolChildCount; i < needChildCount; i++){
                        auto child = Sprite::create();
                        child->setTag(i);
                        child->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskPoolPosition(id, 1) + i * 3);
                        poolParent->addChild(child);
                    }
                }
                
                if(poolValue < 5 * minBlind){
                    ((Sprite *)poolParent->getChildByTag(0))->setTexture("chip_1.png");
                }
                else if(poolValue >= 5 * minBlind && poolValue < 10 * minBlind){
                    for(int i = 0; i < 2; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_1.png");
                    }
                    ((Sprite *)poolParent->getChildByTag(2))->setTexture("chip_2.png");
                }
                else if(poolValue >= 10 * minBlind && poolValue < 20 * minBlind){
                    for(int i = 0; i < 2; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_1.png");
                    }
                    
                    for(int i = 2; i < 4; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_2.png");
                    }
                    ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_3.png");
                }
                else if(poolValue >= 20 * minBlind && poolValue < 50 * minBlind){
                    for(int i = 0; i < 2; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_2.png");
                    }
                    
                    for(int i = 2; i < 4; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_3.png");
                    }
                    ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_4.png");
                }
                else if(poolValue >= 50 * minBlind && poolValue < 100 * minBlind){
                    for(int i = 0; i < 2; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_3.png");
                    }
                    
                    for(int i = 2; i < 4; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_4.png");
                    }
                    ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_5.png");
                }
                else if(poolValue >= 100 * minBlind && poolValue < 200 * minBlind){
                    for(int i = 0; i < 2; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_4.png");
                    }
                    
                    for(int i = 2; i < 4; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_5.png");
                    }
                    ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_6.png");
                }
                else if(poolValue >= 200 * minBlind){
                    for(int i = 0; i < 2; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_5.png");
                    }
                    
                    for(int i = 2; i < 4; i++){
                        ((Sprite *)poolParent->getChildByTag(i))->setTexture("chip_6.png");
                    }
                    ((Sprite *)poolParent->getChildByTag(4))->setTexture("chip_7.png");
                }

				//≥Ô¬Î≥ÿ±Í«©
				if (!this->getChildByTag(DKPL_LN_ST_TAG + id)->isVisible()){
					this->getChildByTag(DKPL_LN_ST_TAG + id)->setVisible(true);
				}

				__String* deskPoolText = nullptr;
				if (poolValue < 1000){
					deskPoolText = __String::createWithFormat("%ld", poolValue);
				}
				else if (poolValue >= 1000 && poolValue < 1000000){
					deskPoolText = __String::createWithFormat("%.1fK", poolValue / 1000.0);
				}
				else if (poolValue >= 1000000 && poolValue < 1000000000){
					deskPoolText = __String::createWithFormat("%.1fM", poolValue / 1000000.0);
				}
				else if (poolValue >= 1000000000){
					deskPoolText = __String::createWithFormat("%.1fB", poolValue / 1000000000.0);
				}
				((Label *)this->getChildByTag(DKPL_LN_ST_TAG + id)->getChildByTag(DKPL_L_ST_TAG + id))->setString(deskPoolText->getCString());
			});

			poolParent->runAction(Sequence::create(funcChange, NULL));
		});

		tempPool->runAction(Sequence::create(moveAction, funcAction, NULL));

		this->removeChild(node);
	});
	node->runAction(Sequence::create(delayAction, moveFunc, NULL));
}

///◊Û≤Àµ•
void GameScene::btMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    
	this->getChildByTag(LEFT_MENU_TAG)->setVisible(false);

	MenuItemImage* btn_go_hall = MenuItemImage::create("btn_go_hall_up.png", "btn_go_hall_down.png", CC_CALLBACK_1(GameScene::btGoHallCallback, this));
	btn_go_hall->setAnchorPoint(Vec2(0, 1));
	btn_go_hall->setPosition(10, visibleSize.height);

	MenuItemImage* btn_standup = MenuItemImage::create("btn_standup_up.png", "btn_standup_down.png", CC_CALLBACK_1(GameScene::btStandupCallback, this));
	btn_standup->setAnchorPoint(Vec2(0, 1));
	btn_standup->setPosition(10, visibleSize.height - 67);
    auto strUserId = UserInfo::getInstance()->getUserId();
    auto player = RoomData::getInstance()->getPlayer((char *)strUserId.c_str());
    if(player != NULL){
        auto status = player->getUserStatus();
        if(status == 0){
            btn_standup->setEnabled(false);
            if(this->getChildByTag(STAND_UP_DISABLE_TAG) != NULL && !this->getChildByTag(STAND_UP_DISABLE_TAG)->isVisible()){
                this->getChildByTag(STAND_UP_DISABLE_TAG)->setVisible(true);
            }
            else{
                auto standUpDisable = Sprite::create("btn_standup_disable.png");
                standUpDisable->setTag(STAND_UP_DISABLE_TAG);
                standUpDisable->setAnchorPoint(Vec2(0, 1));
                standUpDisable->setPosition(Vec2(10, visibleSize.height - 67));
                this->addChild(standUpDisable, 3);
            }
        }
        else{
            btn_standup->setEnabled(true);
            if(this->getChildByTag(STAND_UP_DISABLE_TAG) != NULL && this->getChildByTag(STAND_UP_DISABLE_TAG)->isVisible()){
                this->getChildByTag(STAND_UP_DISABLE_TAG)->setVisible(false);
            }
        }
    }

	MenuItemImage* btn_menu_close = MenuItemImage::create("btn_menu_close_up.png", "btn_menu_close_down.png", CC_CALLBACK_1(GameScene::btMenuCloseCallback, this));
	btn_menu_close->setAnchorPoint(Vec2(0, 1));
	btn_menu_close->setPosition(10, visibleSize.height - 127);

	Menu* btn_left_menu = Menu::create(btn_go_hall, btn_standup, btn_menu_close, NULL);
	btn_left_menu->setContentSize(Size(185, 191));
	btn_left_menu->setTag(BTN_LEFT_MENU_TAG);
	btn_left_menu->setPosition(0, 0);
	this->addChild(btn_left_menu, 2);
}

//∑µªÿ¥ÛÃ¸¥˙¿Ì∑Ω∑®
void GameScene::onGoHall(){
	auto userId = UserInfo::getInstance()->getUserId();
	ROOM_USER_INFO_T *player = RoomData::getInstance()->getPlayer((char *)userId.c_str());

	//∑¢ÀÕ’æ∆µƒ√¸¡Óœ˚œ¢∏¯∑˛ŒÒ∆˜
	CStream standupStream;

	rapidjson::Document standupDoc;
	standupDoc.SetObject();
	rapidjson::Document::AllocatorType& standupAlloc = standupDoc.GetAllocator();
	rapidjson::Value standupCtx(rapidjson::kObjectType);
	standupCtx.AddMember("act_commond", "370005", standupAlloc);
	auto actUserId = RoomData::getInstance()->getActUserId();
	standupCtx.AddMember("act_user_id", actUserId.c_str(), standupAlloc);
	auto areaNumber = RoomData::getInstance()->getAreaNumber();
	standupCtx.AddMember("area_number", areaNumber.c_str(), standupAlloc);
	auto chipAccount = UserInfo::getInstance()->getChipAccount();
	standupCtx.AddMember("chip_account", chipAccount.c_str(), standupAlloc);
	standupCtx.AddMember("force_exit", "2", standupAlloc);
	standupCtx.AddMember("game_number", "dzpk", standupAlloc);
	standupCtx.AddMember("msg_head", "dzpk", standupAlloc);
	__String* roomNum = __String::createWithFormat("%d", player->getRoomNum());
	standupCtx.AddMember("room_number", roomNum->getCString(), standupAlloc);
	__String* seatNum = __String::createWithFormat("%d", player->getSeatNum());
	standupCtx.AddMember("seat_number", seatNum->getCString(), standupAlloc);
			
	rapidjson::StringBuffer standupBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> standupWriter(standupBuffer);
	standupCtx.Accept(standupWriter);

	const char *standupJsonData = standupBuffer.GetString();
			
	NetBean::getInstance()->addQQHead(&standupStream);
	NetBean::getInstance()->addHead(&standupStream, strlen(standupJsonData) + 1, 370005);
	NetBean::getInstance()->addData(&standupStream, (char *)standupJsonData, strlen(standupJsonData));
	NetBean::getInstance()->write(standupStream);

//	//∑¢ÀÕ¿Îø™∑øº‰œ˚œ¢∏¯∑˛ŒÒ∆˜£¨∑¢ÀÕ∫Û≥ˆœ÷“Ï≥££¨‘› ±◊¢ Õ£¨≤ª”∞œÏ’˝≥£ π”√
//	sendExitRoomMessage();

	//∑µªÿ¥ÛÃ¸
	this->unscheduleAllSelectors();
	NetBean::getInstance()->close();
	RoomData::getInstance()->eraseData();
    CommunicateData::removeInstance();
	auto gameHallScene = GameHallScene::createScene();
	Director::getInstance()->replaceScene(gameHallScene);
}

//∑¢ÀÕ¿Îø™∑øº‰œ˚œ¢∏¯∑˛ŒÒ∆˜
void GameScene::sendExitRoomMessage(){
	CStream exitRoomStream;

	rapidjson::Document exitRoomDoc;
	exitRoomDoc.SetObject();
	rapidjson::Document::AllocatorType& exitRoomAlloc = exitRoomDoc.GetAllocator();
	rapidjson::Value exitRoomCtx(rapidjson::kObjectType);
	exitRoomCtx.AddMember("msg_head", "dzpk", exitRoomAlloc);
	auto timerId = RoomData::getInstance()->getTimerId();
	exitRoomCtx.AddMember("timer_id", timerId.c_str(), exitRoomAlloc);
	__String* roomNum = __String::createWithFormat("%d", RoomData::getInstance()->getRoomInfo()->getRoomNumber());
	exitRoomCtx.AddMember("room_number", roomNum->getCString(), exitRoomAlloc);
	exitRoomCtx.AddMember("area_number", "normal", exitRoomAlloc);
	auto sessionId = UserInfo::getInstance()->getSessionId();
	exitRoomCtx.AddMember("sessionid", sessionId.c_str(), exitRoomAlloc);
	exitRoomCtx.AddMember("force_exit", "1", exitRoomAlloc);
	auto actUserId = UserInfo::getInstance()->getUserAccount();
	exitRoomCtx.AddMember("act_user_id", actUserId.c_str(), exitRoomAlloc);
	exitRoomCtx.AddMember("game_number", "dzpk", exitRoomAlloc);
	exitRoomCtx.AddMember("act_commond", "370022", exitRoomAlloc);

	rapidjson::StringBuffer exitRoomBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> exitRoomWriter(exitRoomBuffer);
	exitRoomCtx.Accept(exitRoomWriter);

	const char *exitRoomJsonData = exitRoomBuffer.GetString();

	NetBean::getInstance()->addQQHead(&exitRoomStream);
	NetBean::getInstance()->addHead(&exitRoomStream, strlen(exitRoomJsonData) + 1, 370022);
	NetBean::getInstance()->addData(&exitRoomStream, (char *)exitRoomJsonData, strlen(exitRoomJsonData));
	NetBean::getInstance()->write(exitRoomStream);
}

//ºÃ–¯”Œœ∑¥˙¿Ì∑Ω∑®
void GameScene::onGameContinue(){
    if(this->getChildByTag(EXIT_GAME_LAYER_TAG) != NULL){
        this->removeChildByTag(EXIT_GAME_LAYER_TAG);
    }
}

//∑µªÿ¥ÛÃ¸≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btGoHallCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    if(this->getChildByTag(BTN_LEFT_MENU_TAG) != NULL){
        this->removeChildByTag(BTN_LEFT_MENU_TAG);
        if(this->getChildByTag(STAND_UP_DISABLE_TAG) != NULL && this->getChildByTag(STAND_UP_DISABLE_TAG)->isVisible()){
            this->getChildByTag(STAND_UP_DISABLE_TAG)->setVisible(false);
        }
    }
	this->getChildByTag(LEFT_MENU_TAG)->setVisible(true);

	//»Áπ˚’˝‘⁄”Œœ∑£¨µØ≥ˆ∂‘ª∞øÚ£¨ «∑Ò“™’æ∆≤¢∑µªÿ¥ÛÃ¸
	auto userId = UserInfo::getInstance()->getUserId();
	ROOM_USER_INFO_T *player = RoomData::getInstance()->getPlayer((char *)userId.c_str());
	if(player->getUserStatus() > 1){
		//µØ≥ˆ∂‘ª∞øÚ
		ExitGameLayer* exitGameLayer = ExitGameLayer::create();
		exitGameLayer->setTag(EXIT_GAME_LAYER_TAG);
		exitGameLayer->setExitGameDelegate(this);
		this->addChild(exitGameLayer, 10);
	}
	else{	
		//∑¢ÀÕ¿Îø™∑øº‰œ˚œ¢∏¯∑˛ŒÒ∆˜
		sendExitRoomMessage();

		this->unscheduleAllSelectors();
		NetBean::getInstance()->close();
		RoomData::getInstance()->eraseData();
        CommunicateData::removeInstance();
		auto gameHallScene = GameHallScene::createScene();
		Director::getInstance()->replaceScene(gameHallScene);
	}
}

//’æ∆≤Àµ•¥˙¿Ì∑Ω∑®
void GameScene::onStandupLayerStandup(){
    if(this->getChildByTag(STAND_UP_LAYER_TAG) != NULL){
        this->removeChildByTag(STAND_UP_LAYER_TAG);
    }

	//∑¢ÀÕ’æ∆µƒ√¸¡Óœ˚œ¢∏¯∑˛ŒÒ∆˜£¨µ»¥˝∑˛ŒÒ∆˜œÏ”¶œ˚œ¢£¨»ª∫ÛΩ¯––≤Ÿ◊˜
	auto userId = UserInfo::getInstance()->getUserId();
	ROOM_USER_INFO_T *userInfo = RoomData::getInstance()->getPlayer((char *)userId.c_str());

	sendLeaveSeatMessage(userInfo);
}

//ºÃ–¯”Œœ∑°¢πÿ±’≤Àµ•¥˙¿Ì∑Ω∑®
void GameScene::onStandupLayerCloseLayer(){
    if(this->getChildByTag(STAND_UP_LAYER_TAG) != NULL){
        this->removeChildByTag(STAND_UP_LAYER_TAG);
    }
}

//’æ∆≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btStandupCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    if(this->getChildByTag(BTN_LEFT_MENU_TAG) != NULL){
        this->removeChildByTag(BTN_LEFT_MENU_TAG);
        if(this->getChildByTag(STAND_UP_DISABLE_TAG) != NULL && this->getChildByTag(STAND_UP_DISABLE_TAG)->isVisible()){
            this->getChildByTag(STAND_UP_DISABLE_TAG)->setVisible(false);
        }
    }
	this->getChildByTag(LEFT_MENU_TAG)->setVisible(true);

	auto userId = UserInfo::getInstance()->getUserId();
	ROOM_USER_INFO_T *userInfo = RoomData::getInstance()->getPlayer((char *)userId.c_str());
	if(userInfo != NULL){
		if(userInfo->getUserStatus() != 0){
			//µØ≥ˆæØ∏Ê∂‘ª∞øÚ£¨’˝‘⁄”Œœ∑ «∑Ò“™«ø÷∆’æ∆£¨◊¥Ã¨Œ™1ø…“‘’˝≥£’æ∆
			if(userInfo->getUserStatus() != 1){
				StandupLayer* standupLayer = StandupLayer::create();
				standupLayer->setStandupDelegate(this);
				standupLayer->setTag(STAND_UP_LAYER_TAG);
				this->addChild(standupLayer, 10);
			}
			else{
				//∑¢ÀÕ’æ∆µƒ√¸¡Óœ˚œ¢∏¯∑˛ŒÒ∆˜£¨µ»¥˝∑˛ŒÒ∆˜œÏ”¶œ˚œ¢£¨»ª∫ÛΩ¯––≤Ÿ◊˜
				sendLeaveSeatMessage(userInfo);
			}
		}
	}
}

//∑¢ÀÕ¿Î◊˘œ˚œ¢∏¯∑˛ŒÒ∆˜
void GameScene::sendLeaveSeatMessage(ROOM_USER_INFO_T *userInfo){
	CStream standupStream;

	rapidjson::Document standupDoc;
	standupDoc.SetObject();
	rapidjson::Document::AllocatorType& alloc = standupDoc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("act_commond", "370005", alloc);
	auto actUserId = RoomData::getInstance()->getActUserId();
	ctx.AddMember("act_user_id", actUserId.c_str(), alloc);
	auto areaNumber = RoomData::getInstance()->getAreaNumber();
	ctx.AddMember("area_number", areaNumber.c_str(), alloc);
	auto chipAccount = UserInfo::getInstance()->getChipAccount();
	ctx.AddMember("chip_account", chipAccount.c_str(), alloc);
	ctx.AddMember("force_exit", "2", alloc);
	ctx.AddMember("game_number", "dzpk", alloc);
	ctx.AddMember("msg_head", "dzpk", alloc);
	__String* roomNum = __String::createWithFormat("%d", userInfo->getRoomNum());
	ctx.AddMember("room_number", roomNum->getCString(), alloc);
	__String* seatNum = __String::createWithFormat("%d", userInfo->getSeatNum());
	ctx.AddMember("seat_number", seatNum->getCString(), alloc);
			
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	ctx.Accept(writer);

	const char *jsonData = buffer.GetString();
			
	NetBean::getInstance()->addQQHead(&standupStream);
	NetBean::getInstance()->addHead(&standupStream, strlen(jsonData) + 1, 370005);
	NetBean::getInstance()->addData(&standupStream, (char *)jsonData, strlen(jsonData));
	NetBean::getInstance()->write(standupStream);
}

void GameScene::btMenuCloseCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    if(this->getChildByTag(BTN_LEFT_MENU_TAG) != NULL){
        this->removeChildByTag(BTN_LEFT_MENU_TAG);
        if(this->getChildByTag(STAND_UP_DISABLE_TAG) != NULL && this->getChildByTag(STAND_UP_DISABLE_TAG)->isVisible()){
            this->getChildByTag(STAND_UP_DISABLE_TAG)->setVisible(false);
        }
    }
	this->getChildByTag(LEFT_MENU_TAG)->setVisible(true);
}

//…Ã≥«≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btShoppingCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
    MallLayer* mallLayer = MallLayer::create();
    mallLayer->setMallLayerDelegate(this);
    mallLayer->setTag(MALL_LAYER_TAG);
	this->addChild(mallLayer, 10);
}

void GameScene::onCloseMallLayerCallback(){
    this->removeChildByTag(MALL_LAYER_TAG);
}

//≥‰÷µ≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btChargeCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideSmileLayer();
    hideLeftMenu();
    
    loadLayer = LoadLayer::create();
	this->addChild(loadLayer, 10);
    
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
//	__String* str_type = __String::createWithFormat("%d", nFlag);
	req.AddMember("mall_type", "1", alloc);
	
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
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onQuickBuyChipHttpRequestCompleted, this));
    
	// write the post data
	request->setRequestData(encryptData.c_str(), encryptData.size());
	request->setTag("POST Binary test");
	HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::onQuickBuyChipHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
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
	std::string encodeRecvData; //Ω” ’µΩµƒbase64º”√‹ ˝æ›
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		printf("%c", (*buffer)[i]);
		encodeRecvData.push_back((*buffer)[i]);
	}
	const std::string recvData = gloox::Base64::decode64(encodeRecvData); //Ω‚√‹∫Ûµƒ ˝æ›
    QuickChipData::getInstance()->getMallChipDatas().clear();
    QuickChipData::getInstance()->setMallChipDatas(MallInfoData::parseData(recvData));
    
    //xian shi kuai jie zhi fu jie mian
    auto minBlind = RoomData::getInstance()->getRoomInfo()->getMinBlind();
    if(minBlind <= 200){        //kuai jie zhi fu jie mian
        auto quickBuyChipLayer = QuickBuyChipLayer::create();
        this->addChild(quickBuyChipLayer, 10);
    }
    else{                       //qi ta chou ma jie mian
        QuickBuyOtherChipLayer* otherChipLayer = QuickBuyOtherChipLayer::create();
        this->addChild(otherChipLayer, 10);
    }
}

//±Ì«È≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btSmileCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideCommunicateLayer();
    hideLeftMenu();
    
    if(this->getChildByTag(SMILE_LAYER_TAG) == NULL){
        SmileLayer* smileLayer = SmileLayer::create();
        smileLayer->setCommunicateInputDelegate(this);
        smileLayer->setTag(SMILE_LAYER_TAG);
        this->addChild(smileLayer, 10);
    }
    else{
        this->removeChildByTag(SMILE_LAYER_TAG);
    }
}

//øÏΩ›∂Ã”Ô≤Àµ•ªÿµ˜∑Ω∑®
void GameScene::btCommunicateCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    hideSmileLayer();
    hideLeftMenu();
    
	if(this->getChildByTag(COMMUNICATE_LAYER_TAG) == NULL){
		CommunicateLayer* communicateLayer = CommunicateLayer::create();
		communicateLayer->setCommunicateLayerDelegate(this);
		communicateLayer->setTag(COMMUNICATE_LAYER_TAG);
		this->addChild(communicateLayer, 10);
	}
	else{
        if(this->getChildByTag(COMMUNICATE_LAYER_TAG) != NULL){
            if(this->getChildByTag(COMMUNICATE_LAYER_TAG)->isVisible()){
                this->removeChildByTag(COMMUNICATE_LAYER_TAG);
            }
            else{
                this->getChildByTag(COMMUNICATE_LAYER_TAG)->setVisible(true);
            }
        }
	}
}

///µ±∂œø™¡¨Ω” ±µ˜”√
void GameScene::onDisconnected(){
	log("onDisconnected");
    
    auto netConnectErrorLayer = NetConnectErrorLayer::create();
    this->addChild(netConnectErrorLayer, 10);
}

///µ±¡¨Ω”¥ÌŒÛ ±µ˜”√
void GameScene::onConnectError(){
	log("onConnectError");
    
    auto netConnectErrorLayer = NetConnectErrorLayer::create();
    this->addChild(netConnectErrorLayer, 10);
}

///µ±¡¨Ω”≥¨ ± ±µ˜”√
void GameScene::onConnectTimeout(){
	log("onConnectTimeout");
    
    auto netConnectErrorLayer = NetConnectErrorLayer::create();
    this->addChild(netConnectErrorLayer, 10);
}

///µ±∂¡µΩœ˚œ¢ ±µ˜”√
void GameScene::onMessage(char *buff, int len){
	rapidjson::Document doc;
	doc.Parse<0>(buff);

	int actCommond;
	if(doc["act_commond"].IsString()){
        std::string strActCommond = doc["act_commond"].GetString();
		actCommond = atoi(strActCommond.c_str());
	}
	else{
		actCommond = doc["act_commond"].GetInt();
	}
    
    log("onMessage: %d", actCommond);

	switch(actCommond){
	case 370002:				//◊¯œ¬
        {
            onSitDown(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	case 370003:				//º”◊¢
        {
            onAddChip(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	case 370004:				//∑≠π´π≤≈∆
        {
            onDrawCard(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	case 370005:				//¿Î◊˘
        {
            onLeaveSeat(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	case 370006:				//∑¢≈∆
        {
            onDealCard(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	case 370007:				//√§◊¢◊˘Œª
        {
            onBlindSeat(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	case 370008:				//¬÷µΩœ¬“ª∏ˆ∂Ø◊˜µƒÕÊº“
        {
            onNextTurn(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	case 370009:				//”Æ≈∆¡–±Ì
        {
            if(this->getChildByTag(BANKER_SPRITE_TAG) == NULL){         //zhuang jia biao zhi wei kong, shuo ming hai mei kai ju, ci shi shou dao de 370009 shi fu wu qi bug
                return;
            }
            else{
                onWinList(buff, len);
                auto render = Director::getInstance()->getRenderer();
                if(render != NULL){
                    render->render();
                }
            }
        }
		break;
	case 370020:				//¡ƒÃÏœ˚œ¢
        {
            onGetSpeekWords(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	case 370022:				//¿Îø™∑øº‰
        {
            onExitRoom(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
    case 370028:                //道具消息
        {
            onGetChopMsg(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
        break;
	case 370062:				//Ω¯»ÎµΩŒ“≤Ÿ◊˜–≈œ¢£¨÷˜“™ «ªÒ»°timer_id
        {
            onReEnterTurnMe(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
    case 370068:                //接收到错误码
        {
            onGetErrorCode(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
        break;
	case 370072:				//“ªæ÷ÕÍ≥…«Â≥˝◊¿√Ê≥ÿ◊”°¢π´π≤≈∆°¢ ÷≈∆°¢◊Øº“Œª÷√£¨£®‘› ±≤ª”√£©
        {
            onCleanRoomStatus(buff, len);
            auto render = Director::getInstance()->getRenderer();
            if(render != NULL){
                render->render();
            }
        }
		break;
	default:
		break;
	}
}

//◊¯œ¬£¨ÃÌº”µΩÕÊº“¡–±Ì
void GameScene::onSitDown(char *buff, int len){
	rapidjson::Document docSitDown;
	docSitDown.Parse<0>(buff);
    
    if(docSitDown["player_property"].IsNull()){
        //tan chu ti shi tiao, ti shi wei kai fang
        Sprite* sp_tips = Sprite::create("mall_buy_result_bg.png");;
        sp_tips->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        this->addChild(sp_tips);
        
        int errCode = docSitDown["err_code"].GetInt();
        std::string str;
        if(errCode == 211002){
            str = GlobalChineseXml::getInstance()->getValue("msg_211002");
        }
        else if(errCode == 211010){
            str = GlobalChineseXml::getInstance()->getValue("msg_211010");
        }
        else if(errCode == 211009){
            str = GlobalChineseXml::getInstance()->getValue("msg_211009");
        }
        else{
            str = GlobalChineseXml::getInstance()->getValue("msg_211010");
        }
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
    else{
        ROOM_USER_INFO_T *roomUserInfo = RoomData::getInstance()->getPlayer((char *)docSitDown["player_property"]["user_id"].GetString());
        
        if(roomUserInfo == NULL){
            roomUserInfo = new ROOM_USER_INFO_T();
            RoomData::getInstance()->addPlayer(roomUserInfo);
        }
        
        roomUserInfo->setUserId(docSitDown["player_property"]["user_id"].GetString());
        roomUserInfo->setUserAccount(docSitDown["player_property"]["user_account"].GetString());
        roomUserInfo->setNickName(docSitDown["player_property"]["nick_name"].GetString());
        roomUserInfo->setDzpkLevel(docSitDown["player_property"]["dzpk_level"].GetInt());
        roomUserInfo->setDzpkExperience(atoi(docSitDown["player_property"]["dzpk_experience"].GetString()));
        roomUserInfo->setVipLevel(docSitDown["player_property"]["vip_level"].GetInt());
        roomUserInfo->setChipAccount(atoi(docSitDown["player_property"]["chip_account"].GetString()));
        roomUserInfo->setCoinAccount(atoi(docSitDown["player_property"]["coin_account"].GetString()));
        roomUserInfo->setCity(docSitDown["player_property"]["city"].GetString());
        roomUserInfo->setTotalTime(docSitDown["player_property"]["total_time"].GetString());
        roomUserInfo->setWinPer(docSitDown["player_property"]["win_per"].GetString());
        roomUserInfo->setRankName(docSitDown["player_property"]["rank_name"].GetString());
        roomUserInfo->setHeadPhotoSerial(docSitDown["player_property"]["head_photo_serial"].GetString());
        roomUserInfo->setDeskChip(docSitDown["player_property"]["desk_chip"].GetInt());
        roomUserInfo->setSex(docSitDown["player_property"]["sex"].GetInt());
        roomUserInfo->setUserStatus(docSitDown["player_property"]["user_staus"].GetInt());
        roomUserInfo->setRoomNum(docSitDown["player_property"]["room_num"].GetInt());
        roomUserInfo->setSeatNum(docSitDown["player_property"]["seat_num"].GetInt());
        roomUserInfo->setHandChips(docSitDown["player_property"]["hand_chips"].GetInt());
        roomUserInfo->setHandChipsToShow(docSitDown["player_property"]["hand_chips"].GetInt());
        roomUserInfo->setGiftId(docSitDown["player_property"]["giftid"].GetInt());
        //	strcpy(roomUserInfo->levelGift, (char *)docSitDown["player_property"]["level_gift"].GetString());
        roomUserInfo->setMallPath(docSitDown["player_property"]["mall_path"].GetString());
        roomUserInfo->setLevelTitle(docSitDown["player_property"]["level_title"].GetString());
        
        //“˛≤ÿªÚœ‘ æ¡ƒÃÏ≤Àµ•
        showSmileAndCommunicateMenu();
        
        //◊¯œ¬£¨“˛≤ÿ◊˘Œª
        setSeatVisible();
        drawPlayer(roomUserInfo);
        
        showStatus(roomUserInfo->getSeatNum(), roomUserInfo->getUserStatus());
        showSeatHandChip(roomUserInfo->getSeatNum(), roomUserInfo->getUserStatus(), roomUserInfo->getHandChipsToShow());
        
        //如果是玩家自己坐下，旋转座位
        auto strSelfUserId = UserInfo::getInstance()->getUserId();
        auto strUserId = roomUserInfo->getUserId();
        if(strcmp(strSelfUserId.c_str(), strUserId.c_str()) == 0){
            
            
            int selfSeatNum = roomUserInfo->getSeatNum();
            if(selfSeatNum >= 1 && selfSeatNum <= 9){
                seatDelta = 5 - selfSeatNum;
            }
            
            //将状态和手上筹码清空
            for(int i = 1; i <= 9; i++){
                if(this->getChildByTag(STATUS_LABEL_TAG + i) != NULL){
                    showStatus(i, 0);
                    showSeatHandChip(i, 0, 0);
                }
            }
            
            //移动座位或头像，头像菜单，倒计时，桌上筹码，庄家标志，手牌
            for(int i = 1; i <= 9; i++){
                int newSeatNum = i;
                int tempSeatNum = i + seatDelta;
                if(tempSeatNum <= 0){
                    int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                    if(roomSeatNum == 5){
                        newSeatNum = tempSeatNum + 9 + 1;
                    }
                    else{
                        newSeatNum = tempSeatNum + 9;
                    }
                }
                else{
                    int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                    if(roomSeatNum == 5){
                        if(tempSeatNum > 9){
                            newSeatNum = tempSeatNum % 9 - 1;
                        }
                        else{
                            newSeatNum = tempSeatNum;
                        }
                    }
                    else{
                        newSeatNum = tempSeatNum % 9;
                        if(newSeatNum == 0){
                            newSeatNum = 9;
                        }
                    }
                }
                
                log("oldSeat: %d, newSeat: %d", i, newSeatNum);
                
                auto seatMenu = this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + i);
                if(seatMenu != NULL){
                    auto seatMenuMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                    seatMenu->runAction(seatMenuMoveTo);
                }
                
                auto seatPlayer = this->getChildByTag(i);
                if(seatPlayer != NULL){
                    auto seatPlayerPhotoBg = seatPlayer->getChildByTag(PLAYER_PHOTO_BACKGROUND_TAG + i);
                    auto seatPlayerPhoto = seatPlayer->getChildByTag(PLAYER_PHOTO_TAG + i);
                    auto seatPlayerPhotoBgMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                    auto seatPlayerPhotoMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                    seatPlayerPhotoBg->runAction(seatPlayerPhotoBgMoveTo);
                    seatPlayerPhoto->runAction(seatPlayerPhotoMoveTo);
                }
                
                auto playerSeatMenu = this->getChildByTag(PLAYER_SEAT_MENU_TAG + i);
                if(playerSeatMenu != NULL){
                    auto playerSeatMenuItem = playerSeatMenu->getChildByTag(PLAYER_SEAT_MENU_ITEM_TAG + i);
                    auto playerSeatMenuItemMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                    playerSeatMenuItem->runAction(playerSeatMenuItemMoveTo);
                }
                
                auto seatChipLabel = this->getChildByTag(STCP_LN_ST_TAG + i);
                if(seatChipLabel != NULL){
                    auto seatChipLabelSpriteBg = seatChipLabel->getChildByTag(SEAT_CHIP_LABEL_SPRITE_BG_TAG + i);
                    auto seatChipLabelSprite = seatChipLabel->getChildByTag(STCP_L_ST_TAG + i);
                    auto seatChipLabelSpriteBgMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0) + 36.5, visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1)));
                    auto seatChipLabelSpriteMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0) + 41.25, visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1)));
                    seatChipLabelSpriteBg->runAction(seatChipLabelSpriteBgMoveTo);
                    seatChipLabelSprite->runAction(seatChipLabelSpriteMoveTo);
                }
                auto seatDeskChip = this->getChildByTag(STDKCP_ST_TAG + i);
                if(seatDeskChip != NULL){
                    auto seatDeskChipMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1)));
                    seatDeskChip->runAction(seatDeskChipMoveTo);
                }
                
                for(int j = 0; j < 2; j++){
                    auto handCard = this->getChildByTag((HAND_CARD_TAG + i) * 2 + j);
                    if(handCard != NULL){
                        auto handCardMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 1)));
                        handCard->runAction(handCardMoveTo);
                    }
                }
            }
            
            int turn = RoomData::getInstance()->getRoomInfo()->getTurn();
            if(turn > 0){    //”–¬÷µΩÕÊº“≤Ÿ◊˜
                //µπº∆ ±∂Øª≠
                if(this->getChildByTag(SIT_PROGRESS_TAG + turn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + turn)->isVisible()){
                    int newSeatNum = turn;
                    int tempSeatNum = turn + seatDelta;
                    if(tempSeatNum <= 0){
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            newSeatNum = tempSeatNum + 9 + 1;
                        }
                        else{
                            newSeatNum = tempSeatNum + 9;
                        }
                    }
                    else{
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            if(tempSeatNum > 9){
                                newSeatNum = tempSeatNum % 9 - 1;
                            }
                            else{
                                newSeatNum = tempSeatNum;
                            }
                        }
                        else{
                            newSeatNum = tempSeatNum % 9;
                            if(newSeatNum == 0){
                                newSeatNum = 9;
                            }
                        }
                    }
                    
                    auto sitProgressMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                    this->getChildByTag(SIT_PROGRESS_TAG + turn)->runAction(sitProgressMoveTo);
                }
            }
            
            auto bankerSprite = this->getChildByTag(BANKER_SPRITE_TAG);
            if(bankerSprite != NULL){
                int bankerSeatNum = RoomData::getInstance()->getRoomInfo()->getBanker();
                
                int newBankerSeatNum = bankerSeatNum;
                int tempBankerSeatNum = bankerSeatNum + seatDelta;
                if(tempBankerSeatNum <= 0){
                    int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                    if(roomSeatNum == 5){
                        newBankerSeatNum = tempBankerSeatNum + 9 + 1;
                    }
                    else{
                        newBankerSeatNum = tempBankerSeatNum + 9;
                    }
                }
                else{
                    int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                    if(roomSeatNum == 5){
                        if(tempBankerSeatNum > 9){
                            newBankerSeatNum = tempBankerSeatNum % 9 - 1;
                        }
                        else{
                            newBankerSeatNum = tempBankerSeatNum;
                        }
                    }
                    else{
                        newBankerSeatNum = tempBankerSeatNum % 9;
                        if(newBankerSeatNum == 0){
                            newBankerSeatNum = 9;
                        }
                    }
                }
                
                auto bankerSpriteMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getBankerPosition(newBankerSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getBankerPosition(newBankerSeatNum, 1)));
                bankerSprite->runAction(bankerSpriteMoveTo);
            }
            
            //xian shi zhuang tai he shou shang chou ma
            for(int i = 1; i <= 9; i++){
                if(this->getChildByTag(STATUS_LABEL_TAG + i) != NULL){
                    auto player = RoomData::getInstance()->getPlayerBySeatNum(i);
                    if(player != NULL){
                        showStatus(i, player->getUserStatus());
                        showSeatHandChip(i, player->getUserStatus(), player->getHandChipsToShow());
                    }
                }
            }
        }
    }
}

void GameScene::onHttpGetPhoto(HttpClient *sender, HttpResponse *response){
	do{
		log("onHttpGetPhoto");
		char *endChar = strrchr((char *)response->getHttpRequest()->getUrl(), '.');
        log(endChar);
        //√ª”–.jpg\.pngµ»∫Û◊∫£¨cocos2d-x“˝«ÊΩ‚Œˆª·±®“Ï≥££¨∂‘”⁄’‚÷÷«Èøˆ£¨ÃÌº”±æµÿÕº∆¨, .png , .jpeg, .pvr, .gz, .webp, .tiff, .ccz
        if((strcmp(endChar, ".png") == 0) || (strcmp(endChar, ".PNG") == 0)
           || (strcmp(endChar, ".jpeg") == 0) || (strcmp(endChar, ".JPEG") == 0)
           || (strcmp(endChar, ".pvr") == 0) || (strcmp(endChar, ".PVR") == 0)
           || (strcmp(endChar, ".gz") == 0) || (strcmp(endChar, ".GZ") == 0)
           || (strcmp(endChar, ".webp") == 0) || (strcmp(endChar, ".WEBP") == 0)
           || (strcmp(endChar, ".tiff") == 0) || (strcmp(endChar, ".TIFF") == 0)
           || (strcmp(endChar, ".ccz") == 0) || (strcmp(endChar, ".CCZ") == 0)
           || (strcmp(endChar, ".jpg") == 0) || (strcmp(endChar, ".JPG") == 0)){
            
        }
        else{               //√ª”–.jpg\.pngµ»∫Û◊∫£¨cocos2d-x“˝«ÊΩ‚Œˆª·±®“Ï≥££¨∂‘”⁄’‚÷÷«Èøˆ£¨ÃÌº”±æµÿÕº∆¨, .png , .jpeg, .pvr, .gz, .webp, .tiff, .ccz
            
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
            auto userId = response->getHttpRequest()->getTag();
            auto player = RoomData::getInstance()->getPlayer((char *)userId);
            if(player != NULL){
                auto playerSeatNum = player->getSeatNum();
                log("onHttpGetPhoto: %d", playerSeatNum);
                if(playerSeatNum > 0){
                    Sprite* headPhoto = (Sprite *)(this->getChildByTag(playerSeatNum)->getChildByTag(PLAYER_PHOTO_TAG + playerSeatNum));
                    headPhoto->setTexture(texture);
                }
            }
		}
	}while(0);
    
}

//º”◊¢
void GameScene::onAddChip(char *buff, int len){
    log("onAddChip");
    
	rapidjson::Document docAddChip;
	docAddChip.Parse<0>(buff);

	int seatNum = docAddChip["seat_number"].GetInt();
	int chipValue = docAddChip["chips_value"].GetInt();
	int userStatus = docAddChip["user_status"].GetInt();
    auto player = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
	player->setUserStatus(userStatus);
    
    //bo fang shen yin
    if(GlobalDef::getInstance()->getSoundOn()){
        auto userSex = player->getSex();
        if(userSex == 1){                   //nan
            if(userStatus == 2){            //jia zhu
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("add_chip_male.wav");
            }
            else if(userStatus == 3){       //gen zhu
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("follow_chip_male.wav");
            }
            else if(userStatus == 4){       //kan pai
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("look_card_male.wav");
            }
            else if(userStatus == 5){       //qi pai
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("least_card_male.wav");
            }
            else if(userStatus == 6){       //all in
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("all_in_male.wav");
            }
        }
        else if(userSex == 0){              //nv
            if(userStatus == 2){            //jia zhu
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("add_chip_female.wav");
            }
            else if(userStatus == 3){       //gen zhu
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("follow_chip_female.wav");
            }
            else if(userStatus == 4){       //kan pai
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("look_card_female.wav");
            }
            else if(userStatus == 5){       //qi pai
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("least_card_female.wav");
            }
            else if(userStatus == 6){       //all in
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("all_in_female.wav");
            }
        }
    }
    
	if(chipValue > 0){
		//∏¸–¬ ÷…œ≥Ô¬Î∫Õµ±«∞’‚“ª¬÷∂™µΩ◊¿√Êµƒ≥Ô¬Î
		auto oldHandChipsToShow = player->getHandChipsToShow();
		player->setHandChipsToShow(oldHandChipsToShow - chipValue);
		auto oldDeskChip = player->getDeskChip();
		player->setDeskChip(oldDeskChip + chipValue);
		
		//≤Ÿ◊˜≥Ô¬Î∂Øª≠
		showDeskChip(seatNum, chipValue, player->getDeskChip());
        
        if(GlobalDef::getInstance()->getSoundOn()){
            long minBlind = RoomData::getInstance()->getRoomInfo()->getMinBlind();
            if(chipValue < (5 * minBlind)){
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("drop_little_chip.wav");
            }
            else{
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("drop_much_chip.wav");
            }
        }
	}

	//∏¸–¬◊¥Ã¨
	showStatus(seatNum, userStatus);
	showSeatHandChip(seatNum, userStatus, player->getHandChipsToShow());
}

//∑≠π´π≤≈∆
void GameScene::onDrawCard(char *buff, int len){
	//»•µÙ…œ“ª∏ˆµπº∆ ±
	if (this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->isVisible()){
		this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->setVisible(false);
		this->unscheduleUpdate();
	}

	rapidjson::Document docDrawCard;
	docDrawCard.Parse<0>(buff);

	int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();

	//≥Ô¬ÎπÈ≥ÿ
	//≥Ô¬Î∑…µΩ◊¿√Êƒ≥“ª¥¶£¨»ª∫Ûœ¬√Ê‘Ÿ∑÷µΩ∏˜∏ˆ≥ÿ
	collectDeskChip(roomSeatNum);

	//±ﬂ≥ÿ–≈œ¢
	if(!docDrawCard["desk_pool"].IsNull()){
		int arraySize = docDrawCard["desk_pool"].Capacity();
		if(arraySize > 0){
			ROOM_DESK_POOL_T *roomDeskPool;
			bool isExist = false;
			for(int i = 0; i < arraySize; i++){
				isExist = false;
				int id = docDrawCard["desk_pool"][i]["id"].GetInt();
				for(int j = 0; j < RoomData::getInstance()->getRoomInfo()->desk_pool.size(); j++){
					if(RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->getId() == id){
						isExist = true;
						auto oldPool = RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->getPool();
						RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->setPool(oldPool + docDrawCard["desk_pool"][i]["pool"].GetInt());
						RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->setType(docDrawCard["desk_pool"][i]["type"].GetInt());

						//¥¥Ω®≥Ô¬ÎµΩ÷∏∂®≥ÿ∂Øª≠£¨∂Øª≠∫Û¥¥Ω®≥ÿ≥Ô¬Î
						drawPoolAnimation(id, RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->getPool());
					}
				}

				if(!isExist){
					roomDeskPool = new ROOM_DESK_POOL_T();
					roomDeskPool->setId(docDrawCard["desk_pool"][i]["id"].GetInt());
					roomDeskPool->setPool(docDrawCard["desk_pool"][i]["pool"].GetInt());
					roomDeskPool->setType(docDrawCard["desk_pool"][i]["type"].GetInt());

					RoomData::getInstance()->getRoomInfo()->desk_pool.pushBack(roomDeskPool);

					//¥¥Ω®≥Ô¬ÎµΩ÷∏∂®≥ÿ∂Øª≠£¨∂Øª≠∫Û¥¥Ω®≥ÿ≥Ô¬Î
					drawPoolAnimation(roomDeskPool->getId(), roomDeskPool->getPool());
				}
			}
		}
	}

	//√ø∏ˆ»À◊¿√Ê…œµƒdeskChip«Â¡„
	for(int i = 1; i <= roomSeatNum; i++){
		if(RoomData::getInstance()->getPlayerBySeatNum(i) != NULL){
			RoomData::getInstance()->getPlayerBySeatNum(i)->setDeskChip(0);
		}
	}

	RoomData::getInstance()->getRoomInfo()->setStep(docDrawCard["step"].GetInt());
	//π´π≤≈∆
	if(!docDrawCard["public_brand"].IsNull()){ 
		int arraySize = docDrawCard["public_brand"].Capacity();
		int card;
		if(RoomData::getInstance()->getRoomInfo()->getStep() == 2){
			for(int i = 0; i < arraySize; i++){
                //bo fang shen yin
                if(GlobalDef::getInstance()->getSoundOn()){
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("draw_public_card.wav");
                }
                
				card = docDrawCard["public_brand"][i].GetInt();
				RoomData::getInstance()->getRoomInfo()->deskCard[i] = card;
				//œ‘ æπ´π≤≈∆
				int color = card / 100;
				int num = card % 100;
				char strCard[HEAD_CHAR_LEN] = {'\0'};
				if(color == 1){			//∑Ω∆¨
					sprintf(strCard, "p%dd.png", num);
				}
				else if(color == 2){    //√∑ª®
					sprintf(strCard, "p%dc.png", num);
				}
				else if(color == 3){    //∫ÏÃ“
					sprintf(strCard, "p%db.png", num);
				}
				else if(color == 4){    //∫⁄Ã“
					sprintf(strCard, "p%da.png", num);
				}
				auto cardSprite = Sprite::create(strCard);
				cardSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskCardPosition(i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskCardPosition(i + 1, 1));
				cardSprite->setScale(0.8f);
				cardSprite->setTag(PUBLIC_CARD_TAG + i);
				this->addChild(cardSprite);
			}
		}
		else if(RoomData::getInstance()->getRoomInfo()->getStep() == 3){
			for(int i = 0; i < arraySize; i++){
                //bo fang shen yin
                if(GlobalDef::getInstance()->getSoundOn()){
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("draw_public_card.wav");
                }
                
				card = docDrawCard["public_brand"][i].GetInt();
				RoomData::getInstance()->getRoomInfo()->deskCard[3 + i] = card;
				//œ‘ æπ´π≤≈∆
				int color = card / 100;
				int num = card % 100;
				char strCard[HEAD_CHAR_LEN] = {'\0'};
				if(color == 1){    //∑Ω∆¨
					sprintf(strCard, "p%dd.png", num);
				}
				else if(color == 2){
					sprintf(strCard, "p%dc.png", num);
				}
				else if(color == 3){
					sprintf(strCard, "p%db.png", num);
				}
				else if(color == 4){
					sprintf(strCard, "p%da.png", num);
				}
				auto cardSprite = Sprite::create(strCard);
				cardSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskCardPosition(3 + i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskCardPosition(3 + i + 1, 1));
				cardSprite->setScale(0.8f);
				cardSprite->setTag(PUBLIC_CARD_TAG + 3 + i);
				this->addChild(cardSprite);
			}
		}
		else if(RoomData::getInstance()->getRoomInfo()->getStep() == 4){
			if(arraySize == 1){
				for(int i = 0; i < arraySize; i++){
                    //bo fang shen yin
                    if(GlobalDef::getInstance()->getSoundOn()){
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("draw_public_card.wav");
                    }
                    
					card = docDrawCard["public_brand"][i].GetInt();
					RoomData::getInstance()->getRoomInfo()->deskCard[4 + i] = card;
					//œ‘ æπ´π≤≈∆
					int color = card / 100;
					int num = card % 100;
					char strCard[HEAD_CHAR_LEN] = {'\0'};
					if(color == 1){    //∑Ω∆¨
						sprintf(strCard, "p%dd.png", num);
					}
					else if(color == 2){
						sprintf(strCard, "p%dc.png", num);
					}
					else if(color == 3){
						sprintf(strCard, "p%db.png", num);
					}
					else if(color == 4){
						sprintf(strCard, "p%da.png", num);
					}
					auto cardSprite = Sprite::create(strCard);
					cardSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskCardPosition(4 + i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskCardPosition(4 + i + 1, 1));
					cardSprite->setScale(0.8f);
					cardSprite->setTag(PUBLIC_CARD_TAG + 4 + i);
					this->addChild(cardSprite);
				}
			}
			else if(arraySize == 2){
				for(int i = 0; i < arraySize; i++){
                    //bo fang shen yin
                    if(GlobalDef::getInstance()->getSoundOn()){
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("draw_public_card.wav");
                    }
                    
					card = docDrawCard["public_brand"][i].GetInt();
					RoomData::getInstance()->getRoomInfo()->deskCard[3 + i] = card;
					//œ‘ æπ´π≤≈∆
					int color = card / 100;
					int num = card % 100;
					char strCard[HEAD_CHAR_LEN] = {'\0'};
					if(color == 1){    //∑Ω∆¨
						sprintf(strCard, "p%dd.png", num);
					}
					else if(color == 2){    //√∑ª®
						sprintf(strCard, "p%dc.png", num);
					}
					else if(color == 3){    //∫ÏÃ“
						sprintf(strCard, "p%db.png", num);
					}
					else if(color == 4){    //∫⁄Ã“
						sprintf(strCard, "p%da.png", num);
					}
					auto cardSprite = Sprite::create(strCard);
					cardSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskCardPosition(3 + i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskCardPosition(3 + i + 1, 1));
					cardSprite->setScale(0.8f);
					cardSprite->setTag(PUBLIC_CARD_TAG + 3 + i);
					this->addChild(cardSprite);
				}
			}
			else{	
				for(int i = 0; i < arraySize; i++){
                    //bo fang shen yin
                    if(GlobalDef::getInstance()->getSoundOn()){
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("draw_public_card.wav");
                    }
                    
					card = docDrawCard["public_brand"][i].GetInt();
					RoomData::getInstance()->getRoomInfo()->deskCard[i] = card;
					//œ‘ æπ´π≤≈∆
					int color = card / 100;
					int num = card % 100;
					char strCard[HEAD_CHAR_LEN] = {'\0'};
					if(color == 1){    //∑Ω∆¨
						sprintf(strCard, "p%dd.png", num);
					}
					else if(color == 2){    //√∑ª®
						sprintf(strCard, "p%dc.png", num);
					}
					else if(color == 3){    //∫ÏÃ“
						sprintf(strCard, "p%db.png", num);
					}
					else if(color == 4){    //∫⁄Ã“
						sprintf(strCard, "p%da.png", num);
					}
					auto cardSprite = Sprite::create(strCard);
					cardSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskCardPosition(i + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskCardPosition(i + 1, 1));
					cardSprite->setScale(0.8f);
					cardSprite->setTag(PUBLIC_CARD_TAG + i);
					this->addChild(cardSprite);
				}
			}
		}
	}

    auto nextTurn = docDrawCard["next_turn"].GetInt();
	RoomData::getInstance()->getRoomInfo()->setTurn(nextTurn);

	if(!docDrawCard["default_add_chip"].IsNull()){
		RoomData::getInstance()->setDefaultAddChip(docDrawCard["default_add_chip"].GetInt());
		RoomData::getInstance()->setFollowChip(docDrawCard["follow_chip"].GetInt());
		RoomData::getInstance()->setMaxFollowChip(docDrawCard["max_follow_chip"].GetInt());
		RoomData::getInstance()->setTimerId((char *)docDrawCard["timer_id"].GetString());
	}

	//≤Ÿ◊˜»®œﬁ
	if(!docDrawCard["ctl_rights"].IsNull()){
		int arraySize = docDrawCard["ctl_rights"].Capacity();
		if(arraySize > 0){
			RoomData::getInstance()->clearCtrlRights();
			for(int i = 0; i < arraySize; i++){
				RoomData::getInstance()->addCtrlRight(docDrawCard["ctl_rights"][i].GetInt());
			}

			showOperateMenu(RoomData::getInstance()->getRoomInfo()->getTurn());
		}
	}
    
    //yu cao zuo an niu
    auto userId = UserInfo::getInstance()->getUserId();
    auto userPlayer = RoomData::getInstance()->getPlayer((char *)userId.c_str());
    if(userPlayer != NULL){
        if(userPlayer->getSeatNum() == nextTurn){
            //ru guo kai qi le zheng dong ti shi, bo fang zheng dong yin xiao
            if(GlobalDef::getInstance()->getIsVibrateOn()){
                CocosDenshion::SimpleAudioEngine::getInstance()->vibrate();
            }
            
            do{
                auto ctrlRightList = RoomData::getInstance()->getCtrlRights();
                auto ctrlRightSize = ctrlRightList.size();
                if(ctrlRightSize > 0){
                    if(RoomData::getInstance()->getIsLookOrLeastCard()){
                        bool autoLook = false;
                        for(int i = 0; i < ctrlRightSize; i++){
                            auto right = ctrlRightList.at(i);
                            if(right == 2){
                                //fa song kan pai zhi ling
                                hideOperateMenu();
                                sendOperateMessage(1, 0);
                                
                                autoLook = true;
                                break;
                            }
                        }
                        if(autoLook == true){
                            break;
                        }
                        
                        bool autoLeast = false;
                        for(int i = 0; i < ctrlRightSize; i++){
                            auto right = ctrlRightList.at(i);
                            if(right == 3){
                                //fa song qi pai zhi ling
                                hideOperateMenu();
                                sendOperateMessage(0, 0);
                                
                                autoLeast = true;
                                break;
                            }
                        }
                        if(autoLeast == true){
                            break;
                        }
                    }
                    else if(RoomData::getInstance()->getIsAutoLookCard()){
                        bool autoLook = false;
                        for(int i = 0; i < ctrlRightSize; i++){
                            auto right = ctrlRightList.at(i);
                            if(right == 2){
                                //fa song kan pai zhi ling
                                hideOperateMenu();
                                sendOperateMessage(1, 0);
                                
                                autoLook = true;
                                break;
                            }
                        }
                        if(autoLook == true){
                            break;
                        }
                        else{
                            //œ‘ æ≤Ÿ◊˜∞¥≈•
                            showOperateMenu(nextTurn);
                        }
                    }
                    else if(RoomData::getInstance()->getIsFollowAnyChip()){
                        bool followAndChip = false;
                        for(int i = 0; i < ctrlRightSize; i++){
                            auto right = ctrlRightList.at(i);
                            if(right == 2){
                                //fa song kan pai zhi ling
                                hideOperateMenu();
                                sendOperateMessage(1, 0);
                                
                                followAndChip = true;
                                break;
                            }
                            
                            if(right == 1){
                                //fa song gen zhu zhi ling
                                hideOperateMenu();
                                sendOperateMessage(1, RoomData::getInstance()->getFollowChip());
                                
                                followAndChip = true;
                                break;
                            }
                        }
                        if(followAndChip == true){
                            break;
                        }
                        
                    }
                    else{
                        //œ‘ æ≤Ÿ◊˜∞¥≈•
                        showOperateMenu(nextTurn);
                    }
                }
            }while(0);
        }
        else{
            //œ‘ æ≤Ÿ◊˜∞¥≈•
            showOperateMenu(nextTurn);
        }
    }
    else{
        //œ‘ æ≤Ÿ◊˜∞¥≈•
        showOperateMenu(nextTurn);
    }

	//≤Ÿ◊˜µπº∆ ±
	drawSitProgress();
    
    showStatus(nextTurn, 1);
}

//¿Î◊˘£¨»Áπ˚ «◊‘º∫£¨‘Ú∏¸–¬ÕÊº“–≈œ¢
void GameScene::onLeaveSeat(char *buff, int len){
    bool isSelfLeave = false;
    
	//À˘”–”√ªß¿Îø™∂º≤•∑≈¿Î◊˘∂Øª≠£¨»Ù «◊‘º∫’æ∆£¨‘ÚªÊ÷∆◊˘Œª£¨¥”ÕÊº“¡–±Ì…æ≥˝£¨ÃÌº”µΩ≈‘π€’ﬂ¡–±Ì
	rapidjson::Document docLeaveSeat;
	docLeaveSeat.Parse<0>(buff);

	//¿Î◊˘∂Øª≠
	int seatNum = docLeaveSeat["seat_number"].GetInt();
    log("onLeaveSeat: %d", seatNum);
    if(this->getChildByTag(seatNum) != NULL){
        ((Node *)this->getChildByTag(seatNum))->setVisible(false);
    }
    
    auto playerSeatMenu = ((Menu *)this->getChildByTag(PLAYER_SEAT_MENU_TAG + seatNum));
    if(playerSeatMenu != NULL && playerSeatMenu->isEnabled()){
        log("removePlayerSeatMenu: %d", seatNum);
        playerSeatMenu->setEnabled(false);
    }

	//»Áπ˚ «ÕÊº“◊‘º∫¿Îø™£¨“∆≥˝≤Ÿ◊˜∞¥≈•
	auto userId = UserInfo::getInstance()->getUserId();
    auto player = RoomData::getInstance()->getPlayer((char *)userId.c_str());
	if((player != NULL) && (player->getSeatNum() == seatNum)){
		showOperateMenu(RoomData::getInstance()->getRoomInfo()->getTurn());
	}

	//∏¸–¬ÕÊº“–≈œ¢£¨≤ªπ‹ «ÕÊº“◊‘º∫ªπ «±µƒÕÊº“∂ºµ√∏¸–¬–≈œ¢£¨÷˜“™µƒ «∏¸–¬seatNum–≈œ¢£¨”√”⁄ªÊ÷∆◊˘Œª ±≈–∂œ
	if(!docLeaveSeat["player_property"].IsNull()){
		char *userId = (char *)docLeaveSeat["player_property"]["user_id"].GetString();
		ROOM_USER_INFO_T *roomUserInfo = RoomData::getInstance()->getPlayer(userId);
        if(roomUserInfo != NULL){
            roomUserInfo->setUserAccount(docLeaveSeat["player_property"]["user_account"].GetString());
            roomUserInfo->setNickName(docLeaveSeat["player_property"]["nick_name"].GetString());
            roomUserInfo->setDzpkLevel(docLeaveSeat["player_property"]["dzpk_level"].GetInt());
            roomUserInfo->setDzpkExperience(atoi(docLeaveSeat["player_property"]["dzpk_experience"].GetString()));
            roomUserInfo->setVipLevel(docLeaveSeat["player_property"]["vip_level"].GetInt());
            roomUserInfo->setChipAccount(atoi(docLeaveSeat["player_property"]["chip_account"].GetString()));
            roomUserInfo->setRankName(docLeaveSeat["player_property"]["rank_name"].GetString());
            roomUserInfo->setHeadPhotoSerial(docLeaveSeat["player_property"]["head_photo_serial"].GetString());
            roomUserInfo->setDeskChip(docLeaveSeat["player_property"]["desk_chip"].GetInt());
            roomUserInfo->setUserStatus(docLeaveSeat["player_property"]["user_staus"].GetInt());
            roomUserInfo->setRoomNum(docLeaveSeat["player_property"]["room_num"].GetInt());
            roomUserInfo->setSeatNum(docLeaveSeat["player_property"]["seat_num"].GetInt());
            roomUserInfo->setHandChips(docLeaveSeat["player_property"]["hand_chips"].GetInt());
            roomUserInfo->setHandChipsToShow(docLeaveSeat["player_property"]["hand_chips"].GetInt());
            roomUserInfo->setGiftId(docLeaveSeat["player_property"]["giftid"].GetInt());
            roomUserInfo->setMallPath(docLeaveSeat["player_property"]["mall_path"].GetString());
            roomUserInfo->setLevelTitle(docLeaveSeat["player_property"]["level_title"].GetString());
        }

		//»Áπ˚ «ÕÊº“◊‘º∫£¨∏¸–¬UserInfo–≈œ¢£¨÷˜“™ «”√ªßµ»º∂∫Õ≥Ô¬Î
		auto strUserId = UserInfo::getInstance()->getUserId();
		if(strcmp(userId, (char *)strUserId.c_str()) == 0){
            isSelfLeave = true;
            
			UserInfo::getInstance()->setChipAccount(docLeaveSeat["player_property"]["chip_account"].GetString());
			UserInfo::getInstance()->setDzpkLevel(__String::createWithFormat("%d", docLeaveSeat["player_property"]["dzpk_level"].GetInt())->getCString());
			UserInfo::getInstance()->setDzpkExperience(docLeaveSeat["player_property"]["dzpk_experience"].GetString());
			UserInfo::getInstance()->setVipLevel(__String::createWithFormat("%d", docLeaveSeat["player_property"]["vip_level"].GetInt())->getCString());
            
            //yi chu jia zhu hua dong tiao
            if(this->getChildByTag(SLIDER_NODE_TAG) != NULL){
                this->removeChildByTag(SLIDER_NODE_TAG, true);
            }
		}
        
        auto leavePlayer = RoomData::getInstance()->getPlayer(userId);
        if(leavePlayer != NULL){
            //    leavePlayer->setDeskChip(0);
            leavePlayer->setSeatNum(0);
            leavePlayer->setUserStatus(0);
            auto leaveUserId = leavePlayer->getUserId();
            auto strUserId = UserInfo::getInstance()->getUserId();
            if(strcmp(leaveUserId.c_str(), strUserId.c_str()) != 0){
                RoomData::getInstance()->delPlayer((char *)leaveUserId.c_str());
            }
        }
	}
	else{	//√ª”–”√ªß–≈œ¢£¨–Ë“™Õ®π˝◊˘Œª∫≈∏¸–¬”√ªß◊¥Ã¨£¨”√”⁄∫Û√Êµƒ◊˘Œªœ‘ æ
        auto strUserId = UserInfo::getInstance()->getUserId();
        auto player = RoomData::getInstance()->getPlayer((char *)strUserId.c_str());
        if(player != NULL){
            int userSeatNum = player->getSeatNum();
            if(userSeatNum == seatNum){
                isSelfLeave = true;
                
                //yi chu jia zhu hua dong tiao
                if(this->getChildByTag(SLIDER_NODE_TAG) != NULL){
                    this->removeChildByTag(SLIDER_NODE_TAG, true);
                }
            }
        }
        
        auto leavePlayer = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
        if(leavePlayer != NULL){
            //        leavePlayer->setDeskChip(0);
            leavePlayer->setSeatNum(0);
            leavePlayer->setUserStatus(0);
            auto leaveUserId = leavePlayer->getUserId();
            auto strUserId = UserInfo::getInstance()->getUserId();
            if(strcmp(leaveUserId.c_str(), strUserId.c_str()) != 0){
                RoomData::getInstance()->delPlayer((char *)leaveUserId.c_str());
            }
        }
	}

	//∂™≈∆£¨≤¢¥” ÷≈∆¡–±Ì÷–…æ≥˝
	bool hasCardSeat = RoomData::getInstance()->hasCardSeat(seatNum);
	if(hasCardSeat){
		RoomData::getInstance()->removeCardSeat(seatNum);
        
        log("removeCardSeat: %d", seatNum);

		//∂™≈∆∂Øª≠
		if(this->getChildByTag((HAND_CARD_TAG + seatNum) * 2) != NULL){
			this->removeChildByTag((HAND_CARD_TAG + seatNum) * 2);
            log("removeCardSeatNode: %d", seatNum);
		}

		if(this->getChildByTag((HAND_CARD_TAG + seatNum) * 2 + 1) != NULL){
			this->removeChildByTag((HAND_CARD_TAG + seatNum) * 2 + 1);
		}
	}

	//»Áπ˚ «¬÷µΩµƒµ±«∞ÕÊº“¿Î◊˘£¨«Â≥˝µπº∆ ±
	int turn = RoomData::getInstance()->getRoomInfo()->getTurn();
	if(turn == seatNum){
		if (this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->isVisible()){
            this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->setVisible(false);
            this->unscheduleUpdate();
        }
	}

	//“˛≤ÿªÚœ‘ æ¡ƒÃÏ≤Àµ•
	showSmileAndCommunicateMenu();

	//À¢–¬◊˘Œª◊¥Ã¨(∞¸¿® ÷…œ≥Ô¬Î)
	showStatus(seatNum, 0);
	showSeatHandChip(seatNum, 0, 0);

	//ªÊ÷∆◊˘Œª
	setSeatVisible();
    
    Director::getInstance()->getRenderer()->render();
    
    //ru guo shi wan jia zi ji li zuo, xuan zhuan zuo wei dao zheng chang wei zhi
    if(isSelfLeave == true){
        showOperateMenu(10);
        
        seatDelta = 0;
                
        //xian jiang zhuang tai he shou shang chou ma qing kong
        for(int i = 1; i <= 9; i++){
            if(this->getChildByTag(STATUS_LABEL_TAG + i) != NULL){
                showStatus(i, 0);
                showSeatHandChip(i, 0, 0);
            }
        }
                
        //yi dong zuo wei huo tou xiang, tou xiang cai dan, dao ji shi, zhuo shang chou ma, zhuang jia biao zhi, shou pai
        for(int i = 1; i <= 9; i++){
            int newSeatNum = i;
            int tempSeatNum = i + seatDelta;
            if(tempSeatNum <= 0){
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    newSeatNum = tempSeatNum + 9 + 1;
                }
                else{
                    newSeatNum = tempSeatNum + 9;
                }
            }
            else{
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    if(tempSeatNum > 9){
                        newSeatNum = tempSeatNum % 9 - 1;
                    }
                    else{
                        newSeatNum = tempSeatNum;
                    }
                }
                else{
                    newSeatNum = tempSeatNum % 9;
                    if(newSeatNum == 0){
                        newSeatNum = 9;
                    }
                }
            }
                    
            log("oldSeat: %d, newSeat: %d", i, newSeatNum);
                    
            auto seatMenu = this->getChildByTag(SEAT_MENU_TAG)->getChildByTag(SEAT_START_TAG + i);
            if(seatMenu != NULL){
                auto seatMenuMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                seatMenu->runAction(seatMenuMoveTo);
            }
                    
            auto seatPlayer = this->getChildByTag(i);
            if(seatPlayer != NULL){
                auto seatPlayerPhotoBg = seatPlayer->getChildByTag(PLAYER_PHOTO_BACKGROUND_TAG + i);
                auto seatPlayerPhoto = seatPlayer->getChildByTag(PLAYER_PHOTO_TAG + i);
                auto seatPlayerPhotoBgMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                auto seatPlayerPhotoMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                seatPlayerPhotoBg->runAction(seatPlayerPhotoBgMoveTo);
                seatPlayerPhoto->runAction(seatPlayerPhotoMoveTo);
            }
                    
            auto playerSeatMenu = this->getChildByTag(PLAYER_SEAT_MENU_TAG + i);
            if(playerSeatMenu != NULL){
                auto playerSeatMenuItem = playerSeatMenu->getChildByTag(PLAYER_SEAT_MENU_ITEM_TAG + i);
                auto playerSeatMenuItemMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                playerSeatMenuItem->runAction(playerSeatMenuItemMoveTo);
            }
                    
            auto seatChipLabel = this->getChildByTag(STCP_LN_ST_TAG + i);
            if(seatChipLabel != NULL){
                auto seatChipLabelSpriteBg = seatChipLabel->getChildByTag(SEAT_CHIP_LABEL_SPRITE_BG_TAG + i);
                auto seatChipLabelSprite = seatChipLabel->getChildByTag(STCP_L_ST_TAG + i);
                auto seatChipLabelSpriteBgMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0) + 36.5, visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1)));
                auto seatChipLabelSpriteMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0) + 41.25, visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1)));
                seatChipLabelSpriteBg->runAction(seatChipLabelSpriteBgMoveTo);
                seatChipLabelSprite->runAction(seatChipLabelSpriteMoveTo);
            }
            auto seatDeskChip = this->getChildByTag(STDKCP_ST_TAG + i);
            if(seatDeskChip != NULL){
                auto seatDeskChipMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getUserDeskChipPosition(newSeatNum, 1)));
                seatDeskChip->runAction(seatDeskChipMoveTo);
            }
                    
            for(int j = 0; j < 2; j++){
                auto handCard = this->getChildByTag((HAND_CARD_TAG + i) * 2 + j);
                if(handCard != NULL){
                    auto handCardMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getHandCardsCenterPosition(newSeatNum, j + 1, 1)));
                    handCard->runAction(handCardMoveTo);
                }
            }
        }
                
        int turn = RoomData::getInstance()->getRoomInfo()->getTurn();
        if(turn > 0){    //”–¬÷µΩÕÊº“≤Ÿ◊˜
            //µπº∆ ±∂Øª≠
            if(this->getChildByTag(SIT_PROGRESS_TAG + turn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + turn)->isVisible()){
                int newSeatNum = turn;
                int tempSeatNum = turn + seatDelta;
                if(tempSeatNum <= 0){
                    int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                    if(roomSeatNum == 5){
                        newSeatNum = tempSeatNum + 9 + 1;
                    }
                    else{
                        newSeatNum = tempSeatNum + 9;
                    }
                }
                else{
                    int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                    if(roomSeatNum == 5){
                        if(tempSeatNum > 9){
                            newSeatNum = tempSeatNum % 9 - 1;
                        }
                        else{
                            newSeatNum = tempSeatNum;
                        }
                    }
                    else{
                        newSeatNum = tempSeatNum % 9;
                        if(newSeatNum == 0){
                            newSeatNum = 9;
                        }
                    }
                }
                        
                auto sitProgressMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
                this->getChildByTag(SIT_PROGRESS_TAG + turn)->runAction(sitProgressMoveTo);
            }
        }
                
        auto bankerSprite = this->getChildByTag(BANKER_SPRITE_TAG);
        if(bankerSprite != NULL){
            int bankerSeatNum = RoomData::getInstance()->getRoomInfo()->getBanker();
            
            int newBankerSeatNum = bankerSeatNum;
            int tempBankerSeatNum = bankerSeatNum + seatDelta;
            if(tempBankerSeatNum <= 0){
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    newBankerSeatNum = tempBankerSeatNum + 9 + 1;
                }
                else{
                    newBankerSeatNum = tempBankerSeatNum + 9;
                }
            }
            else{
                int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                if(roomSeatNum == 5){
                    if(tempBankerSeatNum > 9){
                        newBankerSeatNum = tempBankerSeatNum % 9 - 1;
                    }
                    else{
                        newBankerSeatNum = tempBankerSeatNum;
                    }
                }
                else{
                    newBankerSeatNum = tempBankerSeatNum % 9;
                    if(newBankerSeatNum == 0){
                        newBankerSeatNum = 9;
                    }
                }
            }
                    
            auto bankerSpriteMoveTo = MoveTo::create(0.1f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getBankerPosition(newBankerSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getBankerPosition(newBankerSeatNum, 1)));
            bankerSprite->runAction(bankerSpriteMoveTo);
        }
                
        //xian shi zhuang tai he shou shang chou ma
        for(int i = 1; i <= 9; i++){
            if(this->getChildByTag(STATUS_LABEL_TAG + i) != NULL){
                auto player = RoomData::getInstance()->getPlayerBySeatNum(i);
                if(player != NULL){
                    showStatus(i, player->getUserStatus());
                    showSeatHandChip(i, player->getUserStatus(), player->getHandChipsToShow());
                }
            }
        }
        
        setSeatVisible();
    }
}

//∑¢≈∆
void GameScene::onDealCard(char *buff, int len){
    log("onDealCard");
    
	rapidjson::Document docDealCard;
	docDealCard.Parse<0>(buff);

	//∑¢ ÷≈∆¡–±Ì
	if(!docDealCard["card_seatnumber"].IsNull()){
		int arraySize = docDealCard["card_seatnumber"].Capacity();
		if(arraySize > 0){
			int cardSeat = 0;
			RoomData::getInstance()->clearCardSeatNumber();
			for(int i = 0; i < arraySize; i++){
				cardSeat = docDealCard["card_seatnumber"][i].GetInt();
				RoomData::getInstance()->addCardSeat(cardSeat);
                log("onDealCard->addCardSeatNum: %d", cardSeat);
			}
		}
	}

	//ÕÊº“◊‘º∫ ÷≈∆
	if(!docDealCard["hand_cards"].IsNull()){
		int handCard = 0;
		for(int i = 0; i < 2; i++){
			handCard = docDealCard["hand_cards"][i].GetInt();
			RoomData::getInstance()->setHandCard(i + 1, handCard);
		}
	}

	RoomData::getInstance()->getRoomInfo()->setStep(1);

	//∑¢≈∆∂Øª≠
	drawHandCards();
}

//√§◊¢◊˘Œª
void GameScene::onBlindSeat(char *buff, int len){
    log("onBlindSeat");
    
	rapidjson::Document docBlindSeat;
	docBlindSeat.Parse<0>(buff);

	RoomData::getInstance()->getRoomInfo()->setBanker(docBlindSeat["banker"].GetInt());
	RoomData::getInstance()->getRoomInfo()->setBigBlind(docBlindSeat["blinds"]["big_blind"].GetInt());
	RoomData::getInstance()->getRoomInfo()->setSmallBlind(docBlindSeat["blinds"]["small_blind"].GetInt());
	RoomData::getInstance()->getRoomInfo()->setMaxBlind(docBlindSeat["max_blind"].GetInt());
	RoomData::getInstance()->getRoomInfo()->setMinBlind(docBlindSeat["min_blind"].GetInt());

	//œ‘ æ¥Û–°√§◊¢°¢◊Øº“
	drawBankerAndBlind();

	//À¢–¬◊˘Œª◊¥Ã¨
	if(!docBlindSeat["user_staus"].IsNull()){
		auto playerList = RoomData::getInstance()->getPlayerList();
		int arraySize = docBlindSeat["user_staus"].Capacity();
		int seatNum = 0;
		int status = 0;
		for(int i = 0; i < arraySize; i++){
			seatNum = docBlindSeat["user_staus"][i]["seat_num"].GetInt();
			status = docBlindSeat["user_staus"][i]["staus"].GetInt();
			auto player = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
			if(player != NULL){
				player->setUserStatus(status);
			}

			//À¢–¬◊¥Ã¨œ‘ æ
			showStatus(seatNum, status);
		}
	}

	//À¢–¬”√ªß≥Ô¬Î
	if(!docBlindSeat["hand_chip_list"].IsNull()){
		auto playerList = RoomData::getInstance()->getPlayerList();
		int arraySize = docBlindSeat["hand_chip_list"].Capacity();
		int seatNum = 0;
		for(int i = 0; i < arraySize; i++){
			seatNum = docBlindSeat["hand_chip_list"][i]["seat_num"].GetInt();
			auto player = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
			if(player != NULL){
				player->setChipAccount(atoi(docBlindSeat["hand_chip_list"][i]["chip_account"].GetString()));
				player->setHandChipsToShow(docBlindSeat["hand_chip_list"][i]["hand_chips"].GetInt());

				//À¢–¬ ÷…œ≥Ô¬Îœ‘ æ
				showSeatHandChip(seatNum, player->getUserStatus(), player->getHandChipsToShow());
			}
			else{
				//À¢–¬ ÷…œ≥Ô¬Îœ‘ æ
				showSeatHandChip(seatNum, 0, 0);
			}
		}
	}

	//œ‘ æ¥Û–°√§◊¢◊¿√Ê≥Ô¬Îœ¬◊¢
    auto bigBlindPlayer = RoomData::getInstance()->getPlayerBySeatNum(RoomData::getInstance()->getRoomInfo()->getBigBlind());
    auto smallBlindPlayer = RoomData::getInstance()->getPlayerBySeatNum(RoomData::getInstance()->getRoomInfo()->getSmallBlind());
    if(bigBlindPlayer != NULL){
        bigBlindPlayer->setDeskChip(RoomData::getInstance()->getRoomInfo()->getMaxBlind());
    }
	if(smallBlindPlayer != NULL){
        smallBlindPlayer->setDeskChip(RoomData::getInstance()->getRoomInfo()->getMinBlind());
    }
	
	showDeskChip(RoomData::getInstance()->getRoomInfo()->getBigBlind(), RoomData::getInstance()->getRoomInfo()->getMaxBlind(), RoomData::getInstance()->getRoomInfo()->getMaxBlind());
	showDeskChip(RoomData::getInstance()->getRoomInfo()->getSmallBlind(), RoomData::getInstance()->getRoomInfo()->getMinBlind(), RoomData::getInstance()->getRoomInfo()->getMinBlind());
}

//¬÷µΩœ¬“ª∏ˆ∂Ø◊˜µƒÕÊº“
void GameScene::onNextTurn(char *buff, int len){
	//»•µÙ…œ“ª∏ˆµπº∆ ±
	if (this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->isVisible()){
		this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->setVisible(false);
		this->unscheduleUpdate();
	}
    
    if(progressVolumnPlayed == true){
        if(GlobalDef::getInstance()->getSoundOn()){
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(progressVolumnId);
        }
        
        progressVolumnPlayed = false;
    }

	rapidjson::Document docNextTurn;
	docNextTurn.Parse<0>(buff);

	int nextTurn = docNextTurn["next_turn"].GetInt();
	RoomData::getInstance()->getRoomInfo()->setTurn(nextTurn);
	RoomData::getInstance()->setDefaultAddChip(docNextTurn["default_add_chip"].GetInt());
	RoomData::getInstance()->setFollowChip(docNextTurn["follow_chip"].GetInt());
	RoomData::getInstance()->setMaxFollowChip(docNextTurn["max_follow_chip"].GetInt());
	RoomData::getInstance()->setTimerId((char *)docNextTurn["timer_id"].GetString());
	if(!docNextTurn["ctl_rights"].IsNull()){
		int arraySize = docNextTurn["ctl_rights"].Capacity();
		if(arraySize > 0){
			RoomData::getInstance()->clearCtrlRights();
			for(int i = 0; i < arraySize; i++){
				RoomData::getInstance()->addCtrlRight(docNextTurn["ctl_rights"][i].GetInt());
			}
		}
	}
    
    //yu cao zuo an niu
    auto userId = UserInfo::getInstance()->getUserId();
    auto userPlayer = RoomData::getInstance()->getPlayer((char *)userId.c_str());
    if(userPlayer != NULL){
        if(userPlayer->getSeatNum() == nextTurn){
            //ru guo kai qi le zheng dong ti shi, bo fang zheng dong yin xiao
            if(GlobalDef::getInstance()->getIsVibrateOn()){
                CocosDenshion::SimpleAudioEngine::getInstance()->vibrate();
            }
            
            do{
                auto ctrlRightList = RoomData::getInstance()->getCtrlRights();
                auto ctrlRightSize = ctrlRightList.size();
                if(ctrlRightSize > 0){
                    if(RoomData::getInstance()->getIsLookOrLeastCard()){
                        bool autoLook = false;
                        for(int i = 0; i < ctrlRightSize; i++){
                            auto right = ctrlRightList.at(i);
                            if(right == 2){
                                //fa song kan pai zhi ling
                                hideOperateMenu();
                                sendOperateMessage(1, 0);
                                
                                autoLook = true;
                                break;
                            }
                        }
                        if(autoLook == true){
                            break;
                        }
                        
                        bool autoLeast = false;
                        for(int i = 0; i < ctrlRightSize; i++){
                            auto right = ctrlRightList.at(i);
                            if(right == 3){
                                //fa song qi pai zhi ling
                                hideOperateMenu();
                                sendOperateMessage(0, 0);
                                
                                autoLeast = true;
                                break;
                            }
                        }
                        if(autoLeast == true){
                            break;
                        }
                    }
                    else if(RoomData::getInstance()->getIsAutoLookCard()){
                        bool autoLook = false;
                        for(int i = 0; i < ctrlRightSize; i++){
                            auto right = ctrlRightList.at(i);
                            if(right == 2){
                                //fa song kan pai zhi ling
                                hideOperateMenu();
                                sendOperateMessage(1, 0);
                                
                                autoLook = true;
                                break;
                            }
                        }
                        if(autoLook == true){
                            break;
                        }
                        else{
                            //œ‘ æ≤Ÿ◊˜∞¥≈•
                            showOperateMenu(nextTurn);
                        }
                    }
                    else if(RoomData::getInstance()->getIsFollowAnyChip()){
                        bool followAndChip = false;
                        for(int i = 0; i < ctrlRightSize; i++){
                            auto right = ctrlRightList.at(i);
                            if(right == 2){
                                //fa song kan pai zhi ling
                                hideOperateMenu();
                                sendOperateMessage(1, 0);
                                
                                followAndChip = true;
                                break;
                            }
                            
                            if(right == 1){
                                //fa song gen zhu zhi ling
                                hideOperateMenu();
                                sendOperateMessage(1, RoomData::getInstance()->getFollowChip());
                                
                                followAndChip = true;
                                break;
                            }
                        }
                        if(followAndChip == true){
                            break;
                        }

                    }
                    else{
                        //œ‘ æ≤Ÿ◊˜∞¥≈•
                        showOperateMenu(nextTurn);
                    }
                }
            }while(0);
        }
        else{
            //œ‘ æ≤Ÿ◊˜∞¥≈•
            showOperateMenu(nextTurn);
        }
    }
    else{
        //œ‘ æ≤Ÿ◊˜∞¥≈•
        showOperateMenu(nextTurn);
    }

	//µπº∆ ±∂Øª≠
	drawSitProgress();
    
    showStatus(nextTurn, 1);

	//‘§≤Ÿ◊˜£¨‘› ±Œ¥¥¶¿Ì

}

//Œ™ÕÊº“≤Ÿ◊˜µπº∆ ±À¢–¬£¨Ω¯∂»ÃıΩ•±‰—’…´£¨—’…´–ËΩ¯“ª≤Ωµ˜’˚
void GameScene::update(float delta){
	int betTime = RoomData::getInstance()->getRoomInfo()->getBetTime();
	if (this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->isVisible()){
		ProgressTimer* progress = (ProgressTimer*)this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn);
		float time = (100.0 - progress->getPercentage()) * betTime / 100.0 + delta;
		if(time < betTime){
            if(((betTime - time) <= 3.0) && (progressVolumnPlayed == false)){           //dao ji shi san miao
                if(GlobalDef::getInstance()->getSoundOn()){
                    progressVolumnId = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sit_progress_volumn.wav");
                }
                
                progressVolumnPlayed = true;
            }
            
			float percentage = 100.0 - (time * 100.0 / betTime);
			if (percentage > 80 && percentage <= 100){
				progress->setColor(Color3B(0, 255, 0));
			}
			else if(percentage <= 80 && percentage > 60){
				progress->setColor(Color3B(65, 190, 0));
			}
			else if(percentage <= 60 && percentage > 40){
				progress->setColor(Color3B(130, 125, 0));
			}
			else if (percentage <= 40 && percentage > 20){
				progress->setColor(Color3B(190, 65, 0));
			}
			else if (percentage <= 20 && percentage >= 0){
				progress->setColor(Color3B(255, 0, 0));
			}
			progress->setPercentage(percentage);
		}
		else{	
			if (this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->isVisible()){
                this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->setVisible(false);
            }
			this->unscheduleUpdate();
		}
	}
}

//–ƒÃ¯
void GameScene::onHeartWaveUpdate(float delta){
	CStream displayOverStream;

	rapidjson::Document heartWaveDoc;
	heartWaveDoc.SetObject();
	rapidjson::Document::AllocatorType& alloc = heartWaveDoc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);

	ctx.AddMember("game_number", "dzpk", alloc);
	auto areaNumber = RoomData::getInstance()->getAreaNumber();
	ctx.AddMember("area_number", areaNumber.c_str(), alloc);
	ctx.AddMember("act_commond", "370026", alloc);
	ctx.AddMember("msg_head", "dzpk", alloc);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	ctx.Accept(writer);

	const char *jsonData = buffer.GetString();
			
	NetBean::getInstance()->addQQHead(&displayOverStream);
	NetBean::getInstance()->addHead(&displayOverStream, strlen(jsonData) + 1, 370026);
	NetBean::getInstance()->addData(&displayOverStream, (char *)jsonData, strlen(jsonData));
	NetBean::getInstance()->write(displayOverStream);
}

//”Æ≈∆¡–±Ì
void GameScene::onWinList(char *buff, int len){
	//»•µÙ…œ“ª∏ˆµπº∆ ±
	if (this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->isVisible()){
		this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->setVisible(false);
		this->unscheduleUpdate();
	}

	//“˛≤ÿº”◊¢ª¨∂ØÃı
	if(this->getChildByTag(SLIDER_NODE_TAG) != NULL){
        this->removeChildByTag(SLIDER_NODE_TAG, true);
    }

	//“∆≥˝≤Ÿ◊˜∞¥≈•, she cheng 10, biao shi ying chang suo you cao zuo an niu
    RoomData::getInstance()->setIsLookOrLeastCard(false);
    RoomData::getInstance()->setIsAutoLookCard(false);
    RoomData::getInstance()->setIsFollowAnyChip(false);
    hideAllOperateMenu();

	//∑÷≈‰≥Ô¬Î∂Øª≠£¨’π æ”Æ≈∆∂Øª≠£¨À¢–¬ ÷…œ≥Ô¬Î
	rapidjson::Document docWinList;
	docWinList.Parse<0>(buff);

	int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
	if(!docWinList["timer_id"].IsNull()){
		RoomData::getInstance()->setTimerId((char *)docWinList["timer_id"].GetString());
	}

	//≥Ô¬ÎπÈ≥ÿ
	//≥Ô¬Î∑…µΩ◊¿√Êƒ≥“ª¥¶£¨»ª∫Ûœ¬√Ê‘Ÿ∑÷µΩ∏˜∏ˆ≥ÿ
	collectDeskChip(roomSeatNum);

	//±ﬂ≥ÿ–≈œ¢
	if(!docWinList["desk_pool"].IsNull()){
		int arraySize = docWinList["desk_pool"].Capacity();
		if(arraySize > 0){
			ROOM_DESK_POOL_T *roomDeskPool;
			bool isExist = false;
			for(int i = 0; i < arraySize; i++){
				isExist = false;
				int id = docWinList["desk_pool"][i]["id"].GetInt();
				for(int j = 0; j < RoomData::getInstance()->getRoomInfo()->desk_pool.size(); j++){
					if(RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->getId() == id){
						isExist = true;
						auto oldPool = RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->getPool();
						RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->setPool(oldPool + docWinList["desk_pool"][i]["pool"].GetInt());
						RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->setType(docWinList["desk_pool"][i]["type"].GetInt());

						//¥¥Ω®≥Ô¬ÎµΩ÷∏∂®≥ÿ∂Øª≠£¨∂Øª≠∫Û¥¥Ω®≥ÿ≥Ô¬Î
						drawPoolAnimation(id, RoomData::getInstance()->getRoomInfo()->desk_pool.at(j)->getPool());
					}
				}

				if(!isExist){
					roomDeskPool = new ROOM_DESK_POOL_T();
					roomDeskPool->setId(docWinList["desk_pool"][i]["id"].GetInt());
					roomDeskPool->setPool(docWinList["desk_pool"][i]["pool"].GetInt());
					roomDeskPool->setType(docWinList["desk_pool"][i]["type"].GetInt());

					RoomData::getInstance()->getRoomInfo()->desk_pool.pushBack(roomDeskPool);

					//¥¥Ω®≥Ô¬ÎµΩ÷∏∂®≥ÿ∂Øª≠£¨∂Øª≠∫Û¥¥Ω®≥ÿ≥Ô¬Î
					drawPoolAnimation(roomDeskPool->getId(), roomDeskPool->getPool());
				}
			}
		}
	}

	//∑÷≈‰≥Ô¬Î
	if (!docWinList["win_list"].IsNull()){
		//¥¥Ω®winlist ˝æ›Ω·ππ
		int arraySize = docWinList["win_list"].Capacity();
		winArray.clear();
		for (int i = 0; i < arraySize; i++){
			WinStruct* win = new WinStruct();
			win->setSeatNum(docWinList["win_list"][i]["seat_num"].GetInt());
			int poolIdSize = docWinList["win_list"][i]["pool_id"].Capacity();
			for (int j = 0; j < poolIdSize; j++){
				win->poolIdList.push_back(docWinList["win_list"][i]["pool_id"][j].GetInt());
			}

			winArray.pushBack(win);
		}

		//»Áπ˚ÕÊº“◊‘º∫”Æ¡À£¨‘Ú≤•∑≈”Æ≈∆∂Øª≠£¨»Áπ˚ «±µƒÕÊº“”Æ¡À£¨‘Ú‘⁄∂‘”¶◊˘Œª…œ≤•∑≈–°µƒ”Æ≈∆∂Øª≠
		auto userId = UserInfo::getInstance()->getUserId();
		for (int i = 0; i < winArray.size(); i++){
			int seatNum = winArray.at(i)->getSeatNum();
            auto seatPlayer = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
            if(seatPlayer != NULL){
                auto strUserId = seatPlayer->getUserId();
                if (strcmp(strUserId.c_str(), userId.c_str()) == 0){
                    Node* successNode = Node::create();
                    successNode->setPosition(0, 0);
                    this->addChild(successNode);
                    
                    DelayTime* preDelayTime = DelayTime::create(1.3f);
                    CallFunc* showSuccess = CallFunc::create([=](){
                        Sprite* successSprite = Sprite::create("success.png");
                        successSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
                        this->addChild(successSprite);
                        
                        FadeIn* fadeInAction = FadeIn::create(0.5f);
                        MoveTo* moveToAction = MoveTo::create(1.0f, Vec2(visibleSize.width / 2, visibleSize.height / 2 + 200));
                        CallFunc* removeSuccessSprite = CallFunc::create([=](){
                            this->removeChild(successSprite);
                        });
                        
                        successSprite->runAction(Sequence::create(fadeInAction, moveToAction, removeSuccessSprite, NULL));
                    });
                    CallFunc* removeFunc = CallFunc::create([=](){
                        this->removeChild(successNode);
                    });
                    successNode->runAction(Sequence::create(preDelayTime, showSuccess, removeFunc, NULL));
                }
                else{
                    int newSeatNum = seatNum;
                    int tempSeatNum = seatNum + seatDelta;
                    if(tempSeatNum <= 0){
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            newSeatNum = tempSeatNum + 9 + 1;
                        }
                        else{
                            newSeatNum = tempSeatNum + 9;
                        }
                    }
                    else{
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            if(tempSeatNum > 9){
                                newSeatNum = tempSeatNum % 9 - 1;
                            }
                            else{
                                newSeatNum = tempSeatNum;
                            }
                        }
                        else{
                            newSeatNum = tempSeatNum % 9;
                            if(newSeatNum == 0){
                                newSeatNum = 9;
                            }
                        }
                    }
                    
                    Node* otherPlayerWinNode = Node::create();
                    otherPlayerWinNode->setPosition(0, 0);
                    this->addChild(otherPlayerWinNode);
                    
                    DelayTime* preDelayTime = DelayTime::create(1.3f);
                    CallFunc* showSuccess = CallFunc::create([=](){
                        Sprite* successSprite = Sprite::create("win.png");
                        successSprite->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1) - 20);
                        this->addChild(successSprite);
                        
                        FadeIn* fadeInAction = FadeIn::create(0.5f);
                        MoveTo* moveToAction = MoveTo::create(1.0f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1) + 60));
                        CallFunc* removeSuccessSprite = CallFunc::create([=](){
                            this->removeChild(successSprite);
                        });
                        
                        successSprite->runAction(Sequence::create(fadeInAction, moveToAction, removeSuccessSprite, NULL));
                    });
                    CallFunc* removeFunc = CallFunc::create([=](){
                        this->removeChild(otherPlayerWinNode);
                    });
                    otherPlayerWinNode->runAction(Sequence::create(preDelayTime, showSuccess, removeFunc, NULL));
                }
            }
		}
		
		//∑÷≈‰≥Ô¬Î∂Øª≠
		Node* winNode = Node::create();
		winNode->setPosition(0, 0);
		this->addChild(winNode);

		DelayTime* preDelayAction = DelayTime::create(1.3f);
		CallFunc* moveFuncAction = CallFunc::create([=](){
			for (int i = 0; i < winArray.size(); i++){
				int seatNum = winArray.at(i)->getSeatNum();
				long poolIdSize = winArray.at(i)->poolIdList.size();
                
                int newSeatNum = seatNum;
                int tempSeatNum = seatNum + seatDelta;
                auto roomInfo = RoomData::getInstance()->getRoomInfo();
                if(roomInfo != NULL){
                    int roomSeatNum = roomInfo->getRoomSeatNum();
                    if(tempSeatNum <= 0){
                        if(roomSeatNum == 5){
                            newSeatNum = tempSeatNum + 9 + 1;
                        }
                        else{
                            newSeatNum = tempSeatNum + 9;
                        }
                    }
                    else{
                        if(roomSeatNum == 5){
                            if(tempSeatNum > 9){
                                newSeatNum = tempSeatNum % 9 - 1;
                            }
                            else{
                                newSeatNum = tempSeatNum;
                            }
                        }
                        else{
                            newSeatNum = tempSeatNum % 9;
                            if(newSeatNum == 0){
                                newSeatNum = 9;
                            }
                        }
                    }
                }
                else{
                    return;
                }
                
				//¥”∏˜∏ˆ≥ÿ∑÷±ªÊ÷∆≥Ô¬Î∂Øª≠∑…µΩ÷∏∂®◊˘Œª
				for (int j = 0; j < poolIdSize; j++){
					int poolId = winArray.at(i)->poolIdList.at(j);
					Sprite* winChip = Sprite::create("chip_1.png");
					winChip->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getDeskPoolPosition(poolId, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getDeskPoolPosition(poolId, 1));
					this->addChild(winChip);
					MoveTo* moveAction = MoveTo::create(0.5f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
					CallFunc* removeFunc = CallFunc::create([=](){
						this->removeChild(winChip);
					});
					winChip->runAction(Sequence::create(moveAction, removeFunc, NULL));
				}
			}
		});
		CallFunc* removeFuncAction = CallFunc::create([=](){
			this->removeChild(winNode);
			winArray.clear();
		});
		winNode->runAction(Sequence::create(preDelayAction, moveFuncAction, removeFuncAction, NULL));
	}

	//≥Ô¬Î≥ÿ¬˝¬˝œ˚ ß∂Øª≠
	Node* removePoolNode = Node::create();
	removePoolNode->setPosition(0, 0);
	this->addChild(removePoolNode);

	DelayTime* removePoolDelay = DelayTime::create(1.5f);
	CallFunc* removePoolFunc = CallFunc::create([=](){
		auto deskPool = RoomData::getInstance()->getRoomInfo()->desk_pool;
		for (int i = 0; i < deskPool.size(); i++){
			int poolId = deskPool.at(i)->getId();
			if (this->getChildByTag(DESK_POOL_START_TAG + poolId) != NULL){
//				FadeOut* fadeAction = FadeOut::create(0.2f);
				CallFunc* preFunc = CallFunc::create([=](){
					this->getChildByTag(DKPL_LN_ST_TAG + i)->setVisible(false);
				});
				CallFunc* removeFunc = CallFunc::create([=](){
                    if(this->getChildByTag(DESK_POOL_START_TAG + poolId) != NULL){
                        this->removeChildByTag(DESK_POOL_START_TAG + poolId);
                    }
				});
				this->getChildByTag(DESK_POOL_START_TAG + poolId)->runAction(Sequence::create(preFunc, removeFunc, NULL));
			}
		}
	});
    auto clearRoomInfoDelay = DelayTime::create(0.3f);
	CallFunc* removePoolClearPoolFunc = CallFunc::create([=](){
        auto roomInfo = RoomData::getInstance()->getRoomInfo();
        if(roomInfo != NULL){
            if(!(roomInfo->desk_pool.empty())){
                roomInfo->desk_pool.clear();
            }
        }
		
		this->removeChild(removePoolNode);
	});
	removePoolNode->runAction(Sequence::create(removePoolDelay, removePoolFunc, clearRoomInfoDelay, removePoolClearPoolFunc, NULL));

	//∏¸–¬æ≠—È÷µ
	if (!docWinList["exp_update"].IsNull()){
		int expUpdateSize = docWinList["exp_update"].Capacity();
		for(int i = 0; i < expUpdateSize; i++){
			int expAdd = docWinList["exp_update"][i]["exp_add"].GetInt();
			if(expAdd > 0){
				int seatNum = docWinList["exp_update"][i]["seat_num"].GetInt();
//				char* userId = (char *)docWinList["exp_update"][i]["user_id"].GetString();
				ROOM_USER_INFO_T* player = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
                if(player != NULL){
                    int newSeatNum = seatNum;
                    int tempSeatNum = seatNum + seatDelta;
                    if(tempSeatNum <= 0){
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            newSeatNum = tempSeatNum + 9 + 1;
                        }
                        else{
                            newSeatNum = tempSeatNum + 9;
                        }
                    }
                    else{
                        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
                        if(roomSeatNum == 5){
                            if(tempSeatNum > 9){
                                newSeatNum = tempSeatNum % 9 - 1;
                            }
                            else{
                                newSeatNum = tempSeatNum;
                            }
                        }
                        else{
                            newSeatNum = tempSeatNum % 9;
                            if(newSeatNum == 0){
                                newSeatNum = 9;
                            }
                        }
                    }
                    
                    auto oldDzpkExperience = player->getDzpkExperience();
                    player->setDzpkExperience(oldDzpkExperience + expAdd);
                    player->setDzpkLevel(docWinList["exp_update"][i]["dzpk_level"].GetInt());
                    auto userId = UserInfo::getInstance()->getUserId();
                    auto playerUserId = player->getUserId();
                    if(strcmp(playerUserId.c_str(), userId.c_str()) == 0){
                        UserInfo::getInstance()->setDzpkExperience(__String::createWithFormat("%d", player->getDzpkExperience())->getCString());
                        UserInfo::getInstance()->setDzpkLevel(__String::createWithFormat("%d", player->getDzpkLevel())->getCString());
                    }
                    
                    Node* expNode = Node::create();
                    expNode->setPosition(0, 0);
                    this->addChild(expNode);
                    
                    DelayTime* delayAction = DelayTime::create(1.3f);
                    CallFunc* showExpLabelFunc = CallFunc::create([=](){
                        __String* expText = __String::createWithFormat("+%dExp", expAdd);
                        Label* expLabel = Label::createWithSystemFont(expText->getCString(), "Arial", 27);
                        expLabel->setColor(Color3B(232, 190, 62));
                        expLabel->setPosition(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1) - 20);
                        this->addChild(expLabel, 1);
                        
                        MoveTo* moveToAction = MoveTo::create(0.7f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1) + 60));
                        CallFunc* removeExpLabel = CallFunc::create([=](){
                            this->removeChild(expLabel);
                        });
                        
                        expLabel->runAction(Sequence::create(moveToAction, removeExpLabel, NULL));
                    });
                    CallFunc* removeExpNode = CallFunc::create([=](){
                        this->removeChild(expNode);
                    });
                    
                    expNode->runAction(Sequence::create(delayAction, showExpLabelFunc, removeExpNode, NULL));
                }
			}
		}
	}

	//∏¸–¬ÕÊº“≥Ô¬Î
	if(!docWinList["update_chip_account"].IsNull()){
		int arraySize = docWinList["update_chip_account"].Capacity();
		for(int i = 0; i < arraySize; i++){
			int seatNum = docWinList["update_chip_account"][i]["seat_num"].GetInt();
			auto player = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
			if(player != NULL){
				int chipsAdd = docWinList["update_chip_account"][i]["chips_add"].GetInt();
				auto oldHandChips = player->getHandChips();
				player->setHandChips(oldHandChips + chipsAdd);
                if(player->getHandChips() < 0){
                    player->setHandChipsToShow(0);
                }
                else{
                    player->setHandChipsToShow(player->getHandChips());
                }
				
				auto oldChipAccount = player->getChipAccount();
				player->setChipAccount(oldChipAccount + chipsAdd);
				auto userId = UserInfo::getInstance()->getUserId();
				auto playerUserId = player->getUserId();
				if(strcmp(playerUserId.c_str(), userId.c_str()) == 0){
					UserInfo::getInstance()->setChipAccount(__String::createWithFormat("%ld", player->getChipAccount())->getCString());
				}

				showSeatHandChip(seatNum, player->getUserStatus(), player->getHandChipsToShow());
			}
			else{
				showSeatHandChip(seatNum, 0, 0);
			}
		}
	}

	//«Â≥˝±‰¡ø
	RoomData::getInstance()->getRoomInfo()->setAllDeskChips(0);
//	RoomData::getInstance()->getRoomInfo()->desk_pool.clear();
	RoomData::getInstance()->getRoomInfo()->setBanker(0);
	RoomData::getInstance()->getRoomInfo()->setBigBlind(0);
	RoomData::getInstance()->getRoomInfo()->setSmallBlind(0);

	//«Â¿Ì≈∆æ÷◊¿√Ê
    auto removeHandCard = Node::create();
    removeHandCard->setPosition(Vec2(0, 0));
    this->addChild(removeHandCard);
    DelayTime* removeHandCardDelay = DelayTime::create(2.0f);
    CallFunc* removeHandCardDisplayFunc = CallFunc::create([=](){
        displayOver();
        log("onWinList: displayOver");
    });
	CallFunc* removeRemoveHandCardNode = CallFunc::create([=](){
        this->removeChild(removeHandCard);
    });
    removeHandCard->runAction(Sequence::create(removeHandCardDelay, removeHandCardDisplayFunc, removeRemoveHandCardNode, NULL));

	//∑¢ÀÕ370021≈∆æ÷’π æÕÍ±œœ˚œ¢∏¯∑˛ŒÒ∆˜£¨3√Î∫Û∑¢ÀÕ
	Node* sendDisplayOverMessageNode = Node::create();
	sendDisplayOverMessageNode->setPosition(0, 0);
	this->addChild(sendDisplayOverMessageNode);

	DelayTime* preSendDisplayOverDelayAction = DelayTime::create(3.0f);
	CallFunc* sendDisplayOverFunc = CallFunc::create([=](){
		CStream displayOverStream;

		rapidjson::Document displayOverDoc;
		displayOverDoc.SetObject();
		rapidjson::Document::AllocatorType& alloc = displayOverDoc.GetAllocator();
		rapidjson::Value ctx(rapidjson::kObjectType);

		__String* roomNum = __String::createWithFormat("%d", RoomData::getInstance()->getRoomInfo()->getRoomNumber());
		ctx.AddMember("room_number", roomNum->getCString(), alloc);
		ctx.AddMember("act_commond", "370021", alloc);
		ctx.AddMember("msg_head", "dzpk", alloc);
		auto timerId = RoomData::getInstance()->getTimerId();
		ctx.AddMember("timer_id", timerId.c_str(), alloc);
		auto actUserId = RoomData::getInstance()->getActUserId();
		ctx.AddMember("act_user_id", actUserId.c_str(), alloc);
		ctx.AddMember("game_number", "dzpk", alloc);
		auto userId = UserInfo::getInstance()->getUserId();
		__String* seatNum = __String::createWithFormat("%d", RoomData::getInstance()->getPlayer((char *)userId.c_str())->getSeatNum());
		ctx.AddMember("seat_number", seatNum->getCString(), alloc);
		auto areaNumber = RoomData::getInstance()->getAreaNumber();
		ctx.AddMember("area_number", areaNumber.c_str(), alloc);
	
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		ctx.Accept(writer);

		const char *jsonData = buffer.GetString();
			
		NetBean::getInstance()->addQQHead(&displayOverStream);
		NetBean::getInstance()->addHead(&displayOverStream, strlen(jsonData) + 1, 370021);
		NetBean::getInstance()->addData(&displayOverStream, (char *)jsonData, strlen(jsonData));
		NetBean::getInstance()->write(displayOverStream);
	});
	CallFunc* removeSendDisplayOverNode = CallFunc::create([=](){
		this->removeChild(sendDisplayOverMessageNode);
	});

	sendDisplayOverMessageNode->runAction(Sequence::create(preSendDisplayOverDelayAction, sendDisplayOverFunc, removeSendDisplayOverNode, NULL));
}

//¡ƒÃÏœ˚œ¢
void GameScene::onGetSpeekWords(char *buff, int len){
	//œ‘ æ∂‘”¶◊˘Œªµƒ∂‘”¶¡ƒÃÏœ˚œ¢
	rapidjson::Document docSpeekWords;
	docSpeekWords.Parse<0>(buff);

	//"act_user_id":"si", "chat_msg":"......3.....................", "chat_type":1, "chips_value":0, "seatId":"0"
	int seatNum = atoi(docSpeekWords["seatId"].GetString());
	if(seatNum > 0){            //¥Û”⁄0£¨≤≈‘⁄∂‘”¶◊˘Œªœ‘ æ¡ƒÃÏ–≈œ¢
        char* userAccount = (char *)docSpeekWords["nick_name"].GetString();
		int chatType = docSpeekWords["chat_type"].GetInt();				//1£∫Œƒ◊÷£¨2£∫∂Øª≠£¨3£∫VIP±Ì«È
		int chipsValue = docSpeekWords["chips_value"].GetInt();			//»Áπ˚ « ’∑—±Ì«È–Ëø€»°≥Ô¬Î
		char* chatMsg = (char *)docSpeekWords["chat_msg"].GetString();

		auto player = RoomData::getInstance()->getPlayerBySeatNum(seatNum);
		if(chipsValue != 0){
            auto oldHandChips = player->getHandChips();
            auto oldHandChipsToShow = player->getHandChipsToShow();
            player->setHandChips(oldHandChips - chipsValue);
            player->setHandChipsToShow(oldHandChipsToShow - chipsValue);
            showSeatHandChip(seatNum, player->getUserStatus(), player->getHandChipsToShow());
		}
        
        int newSeatNum = seatNum;
        int tempSeatNum = seatNum + seatDelta;
        if(tempSeatNum <= 0){
            int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
            if(roomSeatNum == 5){
                newSeatNum = tempSeatNum + 9 + 1;
            }
            else{
                newSeatNum = tempSeatNum + 9;
            }
        }
        else{
            int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
            if(roomSeatNum == 5){
                if(tempSeatNum > 9){
                    newSeatNum = tempSeatNum % 9 - 1;
                }
                else{
                    newSeatNum = tempSeatNum;
                }
            }
            else{
                newSeatNum = tempSeatNum % 9;
                if(newSeatNum == 0){
                    newSeatNum = 9;
                }
            }
        }
        
		//œ‘ æ–≈œ¢
        if(chatType == 1){          //文字
            std::string sendText(userAccount);
            sendText.append(": ");
            sendText.append(chatMsg);
            CommunicateData::getInstance()->insertMsg(sendText);
            
            if(this->getChildByTag(COMMUNICATE_INPUT_LAYER_TAG) != NULL){
                ((CommunicateInputLayer *)this->getChildByTag(COMMUNICATE_INPUT_LAYER_TAG))->showRecvMsg(sendText);
            }
            
            //xian shi qi pao
            if(this->getChildByTag(QI_PAO_LABEL_START_TAG + seatNum) == NULL){
                unsigned int beginPos = 0;
                std::vector<std::string> str_vec;
                std::string strMsg(chatMsg);
                do{
                    str_vec.push_back(strMsg.substr(beginPos, 36));
                    if(beginPos + 36 > strMsg.size()){
                        break;
                    }
                    else{
                        beginPos += 36;
                    }
                }while(true);
                std::string resultStr = "";
                for(int i = 0; i < str_vec.size(); i++){
                    resultStr.append(str_vec.at(i));
                    resultStr.append("\n");
                }
                resultStr.pop_back();
                auto textLabel = Label::createWithSystemFont(resultStr.c_str(), "Arial", 17);
                textLabel->setColor(Color3B(26, 26, 26));
                textLabel->setTag(QI_PAO_LABEL_START_TAG + seatNum);
                textLabel->setAnchorPoint(Vec2(0.5, 1));
                textLabel->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatPaoPaoPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatPaoPaoPosition(newSeatNum, 1) - 4));
                
                auto textBg = Scale9Sprite::create("communicate_paopao_bg.png");
                textBg->setContentSize(Size(textLabel->getContentSize().width + 10, textLabel->getContentSize().height + 10));
                
                textBg->setAnchorPoint(Vec2(0.5, 1));
                textBg->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatPaoPaoPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatPaoPaoPosition(newSeatNum, 1)));
                this->addChild(textBg, 1);
                this->addChild(textLabel, 1);
                
                auto labelDelay = DelayTime::create(3.0f);
                auto removeLabelFunc = CallFunc::create([=](){
                    this->removeChild(textLabel);
                });
                textLabel->runAction(Sequence::create(labelDelay, removeLabelFunc, NULL));
                
                auto bgDelay = DelayTime::create(3.0f);
                auto removeBgFunc = CallFunc::create([=](){
                    this->removeChild(textBg);
                });
                textBg->runAction(Sequence::create(bgDelay, removeBgFunc, NULL));
            }
        }
        else if(chatType == 2){     //动画表情
            int index = atoi(chatMsg);
            if(index > 20){
                return;
            }
            
            Vector<SpriteFrame *> arraySpriteFrame;
            
            auto smile1PngName = __String::createWithFormat("smile_%d_1.png", index);
            auto smileFrame1 = SpriteFrame::create(smile1PngName->getCString(), Rect(0, 0, 95, 95));
            arraySpriteFrame.pushBack(smileFrame1);
            
            auto smile2PngName = __String::createWithFormat("smile_%d_2.png", index);
            auto smileFrame2 = SpriteFrame::create(smile2PngName->getCString(), Rect(0, 0, 95, 95));
            arraySpriteFrame.pushBack(smileFrame2);
            
            auto animation = Animation::createWithSpriteFrames(arraySpriteFrame, 0.2f);
            auto animate = Animate::create(animation);
            
            auto node = Sprite::create(smile1PngName->getCString());
            node->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSeatNum, 1)));
            this->addChild(node);
            
            auto smileAction = Repeat::create(animate, 5.0f);
            auto removeFunc = CallFunc::create([=](){
//                animate->release();
                this->removeChild(node);
            });
            
            node->runAction(Sequence::create(smileAction, removeFunc, NULL));
        }
        else if(chatType == 3){     //vip biao qing
            
        }
	}
}

//¿Îø™∑øº‰
void GameScene::onExitRoom(char *buff, int len){
	rapidjson::Document docExitRoom;
	docExitRoom.Parse<0>(buff);

	if(!docExitRoom["player_property"].IsNull()){
        //¿Î◊˘∂Øª≠
        int seatNum = docExitRoom["player_property"]["seat_num"].GetInt();
        if(seatNum > 0){
            if(this->getChildByTag(seatNum) != NULL){
                ((Node *)this->getChildByTag(seatNum))->setVisible(false);
                log("removeChild: %d", seatNum);
            }
            
            auto playerSeatMenu = ((Menu *)this->getChildByTag(PLAYER_SEAT_MENU_TAG + seatNum));
            if(playerSeatMenu != NULL && playerSeatMenu->isEnabled()){
                log("disablePlayerSeatMenu: %d", seatNum);
                playerSeatMenu->setEnabled(false);
            }
        }
        
		char* userId = (char *)docExitRoom["player_property"]["user_id"].GetString();
		auto strUserId = UserInfo::getInstance()->getUserId();
		if(strcmp(userId, (char *)strUserId.c_str()) != 0){
			RoomData::getInstance()->delPlayer(userId);
		}
        
        //∂™≈∆£¨≤¢¥” ÷≈∆¡–±Ì÷–…æ≥˝
        bool hasCardSeat = RoomData::getInstance()->hasCardSeat(seatNum);
        if(hasCardSeat){
            RoomData::getInstance()->removeCardSeat(seatNum);
            
            log("removeCardSeat: %d", seatNum);
            
            //∂™≈∆∂Øª≠
            if(this->getChildByTag((HAND_CARD_TAG + seatNum) * 2) != NULL){
                log("removeCardSeatNode: %d", seatNum);
                this->removeChildByTag((HAND_CARD_TAG + seatNum) * 2);
            }
            
            if(this->getChildByTag((HAND_CARD_TAG + seatNum) * 2 + 1) != NULL){
                this->removeChildByTag((HAND_CARD_TAG + seatNum) * 2 + 1);
            }
        }
        
        //»Áπ˚ «¬÷µΩµƒµ±«∞ÕÊº“¿Î◊˘£¨«Â≥˝µπº∆ ±
        int turn = RoomData::getInstance()->getRoomInfo()->getTurn();
        if(turn == seatNum){
            if (this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->isVisible()){
                this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->setVisible(false);
                this->unscheduleUpdate();
            }
        }
 
        //À¢–¬◊˘Œª◊¥Ã¨(∞¸¿® ÷…œ≥Ô¬Î)
        showStatus(seatNum, 0);
        showSeatHandChip(seatNum, 0, 0);
        
        //ªÊ÷∆◊˘Œª
        setSeatVisible();
	}
}

//道具消息
void GameScene::onGetChopMsg(char *buff, int len){
    log("onGetChopMsg");
    
    rapidjson::Document docChopMsg;
	docChopMsg.Parse<0>(buff);
    
    std::string strCmdType = docChopMsg["cmd_type"].GetString();
    int sendSeatNum = atoi(strCmdType.substr(0, 1).c_str());
    int recvSeatNum = atoi(strCmdType.substr(2, 1).c_str());
    std::string strType = strCmdType.substr(4);
    
    //从发送道具的座位到接收道具的座位，绘制动画
    //旋转后的发送座位
    int newSendSeatNum = sendSeatNum;
    int tempSendSeatNum = sendSeatNum + seatDelta;
    if(tempSendSeatNum <= 0){
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            newSendSeatNum = tempSendSeatNum + 9 + 1;
        }
        else{
            newSendSeatNum = tempSendSeatNum + 9;
        }
    }
    else{
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            if(tempSendSeatNum > 9){
                newSendSeatNum = tempSendSeatNum % 9 - 1;
            }
            else{
                newSendSeatNum = tempSendSeatNum;
            }
        }
        else{
            newSendSeatNum = tempSendSeatNum % 9;
            if(newSendSeatNum == 0){
                newSendSeatNum = 9;
            }
        }
    }
    
    //旋转后的接收座位
    int newRecvSeatNum = recvSeatNum;
    int tempRecvSeatNum = recvSeatNum + seatDelta;
    if(tempRecvSeatNum <= 0){
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            newRecvSeatNum = tempRecvSeatNum + 9 + 1;
        }
        else{
            newRecvSeatNum = tempRecvSeatNum + 9;
        }
    }
    else{
        int roomSeatNum = RoomData::getInstance()->getRoomInfo()->getRoomSeatNum();
        if(roomSeatNum == 5){
            if(tempRecvSeatNum > 9){
                newRecvSeatNum = tempRecvSeatNum % 9 - 1;
            }
            else{
                newRecvSeatNum = tempRecvSeatNum;
            }
        }
        else{
            newRecvSeatNum = tempRecvSeatNum % 9;
            if(newRecvSeatNum == 0){
                newRecvSeatNum = 9;
            }
        }
    }
    
    Vector<SpriteFrame *> arraySpriteFrame;
    Sprite* animateNode = nullptr;
    if(strcmp(strType.c_str(), "flower") == 0){
        for(int i = 1; i <= 37; i++){
            auto strFlowerPngName = __String::createWithFormat("chop_flower_%d.png", i);
            auto flowerFrame = SpriteFrame::create(strFlowerPngName->getCString(), Rect(0, 0, 175, 160));
            arraySpriteFrame.pushBack(flowerFrame);
        }
        
        animateNode = Sprite::create("user_chop_1.png");
    }
    else if(strcmp(strType.c_str(), "egg") == 0){
        for(int i = 1; i <= 48; i++){
            auto strEggPngName = __String::createWithFormat("chop_egg_%d.png", i);
            auto eggFrame = SpriteFrame::create(strEggPngName->getCString(), Rect(0, 0, 175, 160));
            arraySpriteFrame.pushBack(eggFrame);
        }
        
        animateNode = Sprite::create("user_chop_2.png");
    }
    else if(strcmp(strType.c_str(), "cheers") == 0){
        for(int i = 1; i <= 47; i++){
            auto strCheersPngName = __String::createWithFormat("chop_cheers_%d.png", i);
            auto cheersFrame = SpriteFrame::create(strCheersPngName->getCString(), Rect(0, 0, 240, 160));
            arraySpriteFrame.pushBack(cheersFrame);
        }
        
        animateNode = Sprite::create("user_chop_3.png");
    }
    else if(strcmp(strType.c_str(), "cake") == 0){
        for(int i = 1; i <= 30; i++){
            auto strCakePngName = __String::createWithFormat("chop_cake_%d.png", i);
            auto cakeFrame = SpriteFrame::create(strCakePngName->getCString(), Rect(0, 0, 175, 160));
            arraySpriteFrame.pushBack(cakeFrame);
        }
        
        animateNode = Sprite::create("user_chop_4.png");
    }
    else if(strcmp(strType.c_str(), "bome") == 0){
        for(int i = 1; i <= 34; i++){
            auto strBomePngName = __String::createWithFormat("chop_bome_%d.png", i);
            auto bomeFrame = SpriteFrame::create(strBomePngName->getCString(), Rect(0, 0, 175, 160));
            arraySpriteFrame.pushBack(bomeFrame);
        }
        
        animateNode = Sprite::create("user_chop_5.png");
    }
    else if(strcmp(strType.c_str(), "kiss") == 0){
        for(int i = 1; i <= 51; i++){
            auto strKissPngName = __String::createWithFormat("chop_kiss_%d.png", i);
            auto kissFrame = SpriteFrame::create(strKissPngName->getCString(), Rect(0, 0, 175, 160));
            arraySpriteFrame.pushBack(kissFrame);
        }
        
        animateNode = Sprite::create("user_chop_6.png");
    }
    else if(strcmp(strType.c_str(), "love") == 0){
        for(int i = 1; i <= 13; i++){
            auto strLovePngName = __String::createWithFormat("chop_love_%d.png", i);
            auto loveFrame = SpriteFrame::create(strLovePngName->getCString(), Rect(0, 0, 175, 160));
            arraySpriteFrame.pushBack(loveFrame);
        }
        for(int i = 1; i <= 13; i++){
            auto strLovePngName = __String::createWithFormat("chop_love_%d.png", i);
            auto loveFrame = SpriteFrame::create(strLovePngName->getCString(), Rect(0, 0, 175, 160));
            arraySpriteFrame.pushBack(loveFrame);
        }
        
        animateNode = Sprite::create("user_chop_7.png");
    }
    else{
        return;
    }
    
    auto animation = Animation::createWithSpriteFrames(arraySpriteFrame, 0.042f);
    auto animate = Animate::create(animation);
    
    animateNode->setPosition(Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSendSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newSendSeatNum, 1)));
    this->addChild(animateNode, 5);
    
    auto animateNodeMoveAction = MoveTo::create(0.3f, Vec2(visibleSize.width / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newRecvSeatNum, 0), visibleSize.height / 2 + GlobalDef::getInstance()->getSeatCenterPointPosition(newRecvSeatNum, 1)));
    auto chopAnimateAction = Repeat::create(animate, 1.0f);
    auto removeFunc = CallFunc::create([=](){
        this->removeChild(animateNode);
    });
    
    animateNode->runAction(Sequence::create(animateNodeMoveAction, chopAnimateAction, removeFunc, NULL));
}

//«Â¿Ì≈∆æ÷◊¿√Ê
void GameScene::displayOver(){
    //qing chu zhuang jia
    if(this->getChildByTag(BANKER_SPRITE_TAG) != NULL){
        this->removeChildByTag(BANKER_SPRITE_TAG);
    }
    
	//«Â≥˝π´π≤≈∆
	for(int i = 0; i < 5; i++){
        if(this->getChildByTag(PUBLIC_CARD_TAG + i) != NULL){
            this->removeChildByTag(PUBLIC_CARD_TAG + i);
            log("removePublicCard");
        }
	}

	//À¢–¬◊˘Œª◊¥Ã¨
	//«Â≥˝ ÷≈∆£¨≤¢¥” ÷≈∆¡–±Ì÷–…æ≥˝
	auto cardSeatNumber = RoomData::getInstance()->getCardSeatNumber();
	int cardSeat = 0;
	for(int i = 0; i < cardSeatNumber.size(); i++){
		cardSeat = cardSeatNumber.at(i);

		//À¢–¬◊˘Œª◊¥Ã¨
        auto player = RoomData::getInstance()->getPlayerBySeatNum(cardSeat);
        if(player != NULL){
            player->setUserStatus(1);
            showStatus(cardSeat, 1);
        }
		else{
            showStatus(cardSeat, 0);
        }
        
        bool hasCardSeat = RoomData::getInstance()->hasCardSeat(cardSeat);
        if(hasCardSeat){
            RoomData::getInstance()->removeCardSeat(cardSeat);
            
            log("removeCardSeat: %d", cardSeat);
            
            //∂™≈∆∂Øª≠
            if(this->getChildByTag((HAND_CARD_TAG + cardSeat) * 2) != NULL){
                log("removeCardSeatNode: %d", cardSeat);
                this->removeChildByTag((HAND_CARD_TAG + cardSeat) * 2);
            }
            
            if(this->getChildByTag((HAND_CARD_TAG + cardSeat) * 2 + 1) != NULL){
                this->removeChildByTag((HAND_CARD_TAG + cardSeat) * 2 + 1);
            }
        }
	}
}

//Ω¯»ÎµΩŒ“≤Ÿ◊˜–≈œ¢£¨÷˜“™ «ªÒ»°timer_id
void GameScene::onReEnterTurnMe(char *buff, int len){
	rapidjson::Document docReEnterTurnMe;
	docReEnterTurnMe.Parse<0>(buff);

	RoomData::getInstance()->setTimerId((char *)__String::createWithFormat("%d", docReEnterTurnMe["timer_id"].GetInt())->getCString());
	RoomData::getInstance()->setFollowChip(docReEnterTurnMe["min_chip"].GetInt());
	RoomData::getInstance()->setMaxFollowChip(docReEnterTurnMe["max_chip"].GetInt());
	RoomData::getInstance()->setDefaultAddChip(2 * docReEnterTurnMe["min_chip"].GetInt());
	RoomData::getInstance()->clearCtrlRights();
	if(docReEnterTurnMe["min_chip"].GetInt() > 0){        //∏˙◊¢
		RoomData::getInstance()->addCtrlRight(0);
		RoomData::getInstance()->addCtrlRight(1);
		RoomData::getInstance()->addCtrlRight(3);
	}
	else if(docReEnterTurnMe["min_chip"].GetInt() == 0){  //ø¥≈∆
		RoomData::getInstance()->addCtrlRight(0);
		RoomData::getInstance()->addCtrlRight(2);
		RoomData::getInstance()->addCtrlRight(3);
	}

	if(!docReEnterTurnMe["hand_cards"].IsNull()){
		int handCardsNum = docReEnterTurnMe["hand_cards"].Capacity();
		for(int i = 0; i < handCardsNum; i++){
			RoomData::getInstance()->setHandCard(i + 1, docReEnterTurnMe["hand_cards"][i].GetInt());
		}
	}

	//ªÊ÷∆≤Ÿ◊˜∞¥≈•
	showOperateMenu(RoomData::getInstance()->getRoomInfo()->getTurn());
}

//接收到错误码
void GameScene::onGetErrorCode(char *buff, int len){
    rapidjson::Document docGetErrorCode;
	docGetErrorCode.Parse<0>(buff);
    
    int errorCode = docGetErrorCode["error_code"].GetInt();
    
    switch(errorCode){
        case 211007:
        {
            auto hasNotEnoughChopLayer = HasNotEnoughChopLayer::create();
            hasNotEnoughChopLayer->setHasNotEnoughChopDelegate(this);
            hasNotEnoughChopLayer->setTag(HAS_NOT_ENOUGH_CHOP_LAYER_TAG);
            this->addChild(hasNotEnoughChopLayer, 10);
        }
        break;
        default:
            break;
    }
}

//“ªæ÷ÕÍ≥…«Â≥˝◊¿√Ê≥ÿ◊”°¢π´π≤≈∆°¢ ÷≈∆°¢◊Øº“Œª÷√
void GameScene::onCleanRoomStatus(char *buff, int len){
	//»•µÙ…œ“ª∏ˆµπº∆ ±
	if (this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn) != NULL && this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->isVisible()){
		this->getChildByTag(SIT_PROGRESS_TAG + sitProgressTagTurn)->setVisible(false);
		this->unscheduleUpdate();
	}

	displayOver();
}

//º∆À„≥Ô¬Î√Ê÷µ∂‘”¶µƒ≥Ô¬Î¿‡–Õ∫Õ∏ˆ ˝¡–±Ì
void GameScene::calculateChipValueType(std::map<CHIP_VALUE_TYPE, int> *chipValueTypeMap, long long chipValue){
	if(chipValue >= 5000000000000){
		long fiveHundredBillionTypeNum = chipValue / 5000000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIVE_HUNDRED_BILLION_TYPE, fiveHundredBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 5000000000000);
		return;
	}
	else if(chipValue >= 200000000000 && chipValue < 5000000000000){
		long twoHundredBillionTypeNum = chipValue / 200000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWO_HUNDRED_BILLION_TYPE, twoHundredBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 200000000000);
		return;
	}
	else if(chipValue >= 100000000000 && chipValue < 200000000000){
		long oneHundredBillionTypeNum = chipValue / 100000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_ONE_HUNDRED_BILLION_TYPE, oneHundredBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 100000000000);
		return;
	}
	else if(chipValue >= 50000000000 && chipValue < 100000000000){
		long fiftyBillionTypeNum = chipValue / 50000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIFTY_BILLION_TYPE, fiftyBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 50000000000);
		return;
	}
	else if(chipValue >= 20000000000 && chipValue < 50000000000){
		long twentyBillionTypeNum = chipValue / 20000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWENTY_BILLION_TYPE, twentyBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 20000000000);
		return;
	}
	else if(chipValue >= 10000000000 && chipValue < 20000000000){
		long tenBillionTypeNum = chipValue / 10000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TEN_BILLION_TYPE, tenBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 10000000000);
		return;
	}
	else if(chipValue >= 5000000000 && chipValue < 10000000000){
		long fiveBillionTypeNum = chipValue / 5000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIVE_BILLION_TYPE, fiveBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 5000000000);
		return;
	}
	else if(chipValue >= 2000000000 && chipValue < 5000000000){
		long twoBillionTypeNum = chipValue / 2000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWO_BILLION_TYPE, twoBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 2000000000);
		return;
	}
	else if(chipValue >= 1000000000 && chipValue < 2000000000){
		long oneBillionTypeNum = chipValue / 1000000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_ONE_BILLION_TYPE, oneBillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 1000000000);
		return;
	}
	else if(chipValue >= 500000000 && chipValue < 1000000000){
		long fiveHundredMillionTypeNum = chipValue / 500000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIVE_HUNDRED_MILLION_TYPE, fiveHundredMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 500000000);
		return;
	}
	else if(chipValue >= 200000000 && chipValue < 500000000){
		long twoHundredMillionTypeNum = chipValue / 200000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWO_HUNDRED_MILLION_TYPE, twoHundredMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 200000000);
		return;
	}
	else if(chipValue >= 100000000 && chipValue < 200000000){
		long oneHundredMillionTypeNum = chipValue / 100000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_ONE_HUNDRED_MILLION_TYPE, oneHundredMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 100000000);
		return;
	}
	else if(chipValue >= 50000000 && chipValue < 100000000){
		long fiftyMillionTypeNum = chipValue / 50000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIFTY_MILLION_TYPE, fiftyMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 50000000);
		return;
	}
	else if(chipValue >= 20000000 && chipValue < 50000000){
		long twentyMillionTypeNum = chipValue / 20000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWENTY_MILLION_TYPE, twentyMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 20000000);
		return;
	}
	else if(chipValue >= 10000000 && chipValue < 20000000){
		long tenMillionTypeNum = chipValue / 10000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TEN_MILLION_TYPE, tenMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 10000000);
		return;
	}
	else if(chipValue >= 5000000 && chipValue < 10000000){
		long fiveMillionTypeNum = chipValue / 5000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIVE_MILLION_TYPE, fiveMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 5000000);
		return;
	}
	else if(chipValue >= 2000000 && chipValue < 5000000){
		long twoMillionTypeNum = chipValue / 2000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWO_MILLION_TYPE, twoMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 2000000);
		return;
	}
	else if(chipValue >= 1000000 && chipValue < 2000000){
		long oneMillionTypeNum = chipValue / 1000000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_ONE_MILLION_TYPE, oneMillionTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 1000000);
		return;
	}
	else if(chipValue >= 500000 && chipValue < 1000000){
		long fiveHundredThousandTypeNum = chipValue / 500000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIVE_HUNDRED_THOUSAND_TYPE, fiveHundredThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 500000);
		return;
	}
	else if(chipValue >= 200000 && chipValue < 500000){
		long twoHundredThousandTypeNum = chipValue / 200000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWO_HUNDRED_THOUSAND_TYPE, twoHundredThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 200000);
		return;
	}
	else if(chipValue >= 100000 && chipValue < 200000){
		long oneHundredThousandTypeNum = chipValue / 100000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_ONE_HUNDRED_THOUSAND_TYPE, oneHundredThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 100000);
		return;
	}
	else if(chipValue >= 50000 && chipValue < 100000){
		long fiftyThousandTypeNum = chipValue / 50000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIFTY_THOUSAND_TYPE, fiftyThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 50000);
		return;
	}
	else if(chipValue >= 20000 && chipValue < 50000){
		long twentyThousandTypeNum = chipValue / 20000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWENTY_THOUSAND_TYPE, twentyThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 20000);
		return;
	}
	else if(chipValue >= 10000 && chipValue < 20000){
		long tenThousandTypeNum = chipValue / 10000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TEN_THOUSAND_TYPE, tenThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 10000);
		return;
	}
	else if(chipValue >= 5000 && chipValue < 10000){
		long fiveThousandTypeNum = chipValue / 5000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIVE_THOUSAND_TYPE, fiveThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 5000);
		return;
	}
	else if(chipValue >= 2000 && chipValue < 5000){
		long twoThousandTypeNum = chipValue / 2000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWO_THOUSAND_TYPE, twoThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 2000);
		return;
	}
	else if(chipValue >= 1000 && chipValue < 2000){
		long oneThousandTypeNum = chipValue / 1000;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_ONE_THOUSAND_TYPE, oneThousandTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 1000);
		return;
	}
	else if(chipValue >= 500 && chipValue < 1000){
		long fiveHundredTypeNum = chipValue / 500;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIVE_HUNDRED_TYPE, fiveHundredTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 500);
		return;
	}
	else if(chipValue >= 200 && chipValue < 500){
		long twoHundredTypeNum = chipValue / 200;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWO_HUNDRED_TYPE, twoHundredTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 200);
		return;
	}
	else if(chipValue >= 100 && chipValue < 200){
		long oneHundredTypeNum = chipValue / 100;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_ONE_HUNDRED_TYPE, oneHundredTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 100);
		return;
	}
	else if(chipValue >= 50 && chipValue < 100){
		long fiftyTypeNum = chipValue / 50;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIFTY_TYPE, fiftyTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 50);
		return;
	}
	else if(chipValue >= 20 && chipValue < 50){
		long twentyTypeNum = chipValue / 20;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWENTY_TYPE, twentyTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 20);
		return;
	}
	else if(chipValue >= 10 && chipValue < 20){
		long tenTypeNum = chipValue / 10;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TEN_TYPE, tenTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 10);
		return;
	}
	else if(chipValue >= 5 && chipValue < 10){
		long fiveTypeNum = chipValue / 5;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_FIVE_TYPE, fiveTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 5);
		return;
	}
	else if(chipValue >= 2 && chipValue < 5){
		long twoTypeNum = chipValue / 2;
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_TWO_TYPE, twoTypeNum));
		calculateChipValueType(chipValueTypeMap, chipValue % 2);
		return;
	}
	else if(chipValue >= 1 && chipValue < 2){
		chipValueTypeMap->insert(std::pair<CHIP_VALUE_TYPE, int>(VALUE_ONE_TYPE, chipValue));
		return;
	}
	else{	
		return;
	}
}