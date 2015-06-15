#include "GameHallScene.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "MainScene.h"
#include "SFScale9Sprite.h"
#include "../GlobalDef/GlobalDef.h"
#define DEFAULT_HEAD_URL		"http://1251038220.cdn.myqcloud.com/1251038220/"
//#define DEFAULT_HEAD_URL1       "http://1251038220.cdn.myqcloud.com/1251038220/headpic/"
Scene* GameHallScene::createScene(){
	Scene* scene = Scene::create();

	auto layer = GameHallScene::create();

	scene->addChild(layer);

	return scene;
}

GameHallScene::~GameHallScene(){
	m_roomDatas.clear();
}

bool GameHallScene::init(){
	if(!Layer::init()){
		return false;
	}
    
    gameScene = nullptr;

	doubleClicked = false;
	m_nLastIndex = -1;
	m_curRoom = ROOM_NEWBIE;
	hideEmptyRoom = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();
    
	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();
    float scale =1.0;
    if(deltaScreenY > 0.01){
        
        scale=1.0;
        
    }else if (deltaScreenX>0.01){
        
        scale=visibleSize.width/960;
        
    }else{
        
    }

	// 大厅背景
//	Sprite *hall_bg = Sprite::create("gamehall_bg.png");
//	hall_bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
//	this->addChild(hall_bg);
    
    SFScale9Sprite *hall_bg = SFScale9Sprite::spriteWithFile("gamehall_bg.png");
    hall_bg->ignoreAnchorPointForPosition(false);
    hall_bg->setContentSize(visibleSize);
	hall_bg->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(hall_bg);
    
    Sprite* bg_up = Sprite::create("gamehall_bg_up.png");
	bg_up->setAnchorPoint(Vec2(0, 1));
	bg_up->setPosition(0, visibleSize.height+3);
	hall_bg->addChild(bg_up,1);
    
//    // 用户头像
//    Sprite* user_icon_1 = Sprite::create("default_photo88.png");
//	user_icon_1->setAnchorPoint(Vec2(0, 1));
//	user_icon_1->setScale(0.8f);
//	user_icon_1->setPosition(Vec2(Vec2(0, visibleSize.height),Vec2(30,0)));
//	hall_bg->addChild(user_icon_1, 0);
    // 用户头像
//    Sprite* user_icon = Sprite::create("default_photo88.png");
//	user_icon->setAnchorPoint(Vec2(0, 1));
//	user_icon->setScale(0.8f);
//	user_icon->setPosition(Vec2(bg_up->getPositionX()+40,bg_up->getPositionY()-5));
//	hall_bg->addChild(user_icon, 0);
    dataPro=new ProductData();
    GlobalDef::getInstance()->setHeadImageScale(0.8);
    std::string strUrl = UserInfo::getInstance()->getHeadPhotoSerial();
    strUrl = strUrl.replace(0, strlen(DEFAULT_HEAD_URL), "");
    dataPro->setMall_path(strUrl.c_str());
    DownHeadImageView *head = DownHeadImageView::create(dataPro);
//    head->setDelegate(this);
    head->setName("AsyncImageLayer");
    head->setAnchorPoint(Vec2(0, 1));
    head->setviewRect(Rect(0, 0, 96, 96));
    head->setPosition(Vec2(bg_up->getPositionX()+80,bg_up->getPositionY()-43));
    hall_bg->addChild(head);

	// 用户昵称
	Label* name = Label::createWithSystemFont(UserInfo::getInstance()->getNikeName().c_str(), "Arial", 30);
//	name->setString();
	name->setAnchorPoint(Vec2(0, 1));
    name->setScale(scale);
	name->setPosition(Vec2(120,visibleSize.height-33));
	hall_bg->addChild(name,2);
    
    Sprite* bg_table = Sprite::create("gamehall_bg_tab.png");
	bg_table->setAnchorPoint(Vec2(0, 1));
    bg_table->setScale(scale);
	bg_table->setPosition(Vec2(bg_up->getPositionX()+10,bg_up->getPositionY()-bg_up->getContentSize().height));
	hall_bg->addChild(bg_table,1);
    
	//筹码
	Sprite* chipSymbol = Sprite::create("chip_symbol.png");
    chipSymbol->cocos2d::Node::setAnchorPoint(Vec2(0,1));
	chipSymbol->setPosition(Vec2(name->getPositionX()+name->getContentSize().width+10,name->getPositionY()-5));
	chipSymbol->setScale(1.3f*scale);
	hall_bg->addChild(chipSymbol,1);

	// 用户筹码
	Label* chip = Label::createWithSystemFont(UserInfo::getInstance()->getChipAccount(), "Arial", 35);
	chip->setAnchorPoint(Vec2(0,1));
    chip->setScale(scale);
    chip->setPosition(Vec2(chipSymbol->getPositionX()+chipSymbol->getContentSize().width+8,chipSymbol->getPositionY()+8));
	hall_bg->addChild(chip,1);
    
    //yin chang fang jiang bei jing
    Sprite* gamehall_hide_room_bg = Sprite::create("gamehall_hide_room_bg.png");
    gamehall_hide_room_bg->setAnchorPoint(Vec2(0.5, 1));
    gamehall_hide_room_bg->setScaleX(scale);
    if(deltaScreenY > 0){
        gamehall_hide_room_bg->setPosition(Vec2(visibleSize.width / 2, 130 - deltaScreenY * 2 + 94));
    }
    else if(deltaScreenX > 0){
        gamehall_hide_room_bg->setPosition(Vec2(visibleSize.width / 2 + 1, 130 + 0.5));
    }
    else{
        gamehall_hide_room_bg->setPosition(Vec2(visibleSize.width / 2, 130));
    }
    hall_bg->addChild(gamehall_hide_room_bg);

	// 隐藏空房间
	CheckboxLayer* hide_room = CheckboxLayer::create();
    hide_room->setAnchorPoint(Vec2(0, 0));
	hide_room->setContentSize(Size(240, 50));
	hide_room->setPosition(45, 90);
	hide_room->setCheckboxDelegate(this);
	hall_bg->addChild(hide_room);
    
    // tableview
    
	// PagePane 
	PagePaneLayer* pagePane = PagePaneLayer::create();
	pagePane->setContentSize(Size(760, 80));
	pagePane->setAnchorPoint(Vec2(0, 0));
	pagePane->setPosition(0, 0);
    pagePane->setScale(scale);
	pagePane->setPagePaneDelegate(this);
	hall_bg->addChild(pagePane);
    
//	tableView = TableView::create(this, Size(942, 360));
    
    if(deltaScreenY > 0){
		tableView = TableView::create(this, Size(960, 260));
		tableView->setAnchorPoint(Vec2(0, 0));
        tableView->setScale(scale);
//		tableView->setPosition(0, 0 - deltaScreenY * 2 + 94);
        tableView->setPosition(8, 130 - deltaScreenY * 2 + 94);
	}else if(deltaScreenX > 0){
        tableView = TableView::create(this, Size(960, 400));
		tableView->setAnchorPoint(Vec2(0, 0));
        tableView->setScale(scale);
		tableView->setPosition(8, 130);
        
    }else{
		tableView = TableView::create(this, Size(960, 360));
		tableView->setAnchorPoint(Vec2(0, 0));
        tableView->setScale(scale);
		tableView->setPosition(8, 130);
	}
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	
	tableView->setDelegate(this);
	hall_bg->addChild(tableView);

	//加入牌桌菜单
	MenuItemImage *btn_join_table_menuitem = MenuItemImage::create("btn_join_table_up.png", "btn_join_table_down.png", CC_CALLBACK_1(GameHallScene::btJoinTableMenuCallback, this));
//	btn_join_table_menuitem->setPosition(675, 595.5);
    btn_join_table_menuitem->setAnchorPoint(Vec2(1,1));
    btn_join_table_menuitem->setScale(scale);
    btn_join_table_menuitem->setPosition(Vec2(visibleSize.width-160,visibleSize.height-15));

	//返回
	MenuItemImage *btn_go_back_menuitem = MenuItemImage::create("btn_go_back_up.png", "btn_go_back_down.png", CC_CALLBACK_1(GameHallScene::btGoBackMenuCallback, this));
    btn_go_back_menuitem->setScale(scale);
//	btn_go_back_menuitem->setPosition(872, 595.5);
    btn_go_back_menuitem->setAnchorPoint(Vec2(1,1));
    btn_go_back_menuitem->setPosition(Vec2(visibleSize.width-20,visibleSize.height-15));

	//刷新
	MenuItemImage *btn_refresh_room_menuitem = MenuItemImage::create("btn_refresh_room_up.png", "btn_refresh_room_down.png", CC_CALLBACK_1(GameHallScene::btnRefreshRoomMenuCallback, this));
	btn_refresh_room_menuitem->setPosition(910*scale, 106);

	auto btn_menu = Menu::create(btn_join_table_menuitem, btn_go_back_menuitem, btn_refresh_room_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	hall_bg->addChild(btn_menu,2);

	getGameHallData(ROOM_NEWBIE, false);
	return true;
}

void GameHallScene::onCreate(){

}

void GameHallScene::onConnected(){
	//发送进入房间消息
	CStream stream;

	//json数据构造
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("act_commond", "370001", alloc);
	auto sessionId = UserInfo::getInstance()->getSessionId();
	ctx.AddMember("sessionid", sessionId.c_str(), alloc);
	auto actUserId = UserInfo::getInstance()->getUserAccount();
	ctx.AddMember("act_user_id", actUserId.c_str(), alloc);
	__String* roomId = __String::createWithFormat("%ld", m_roomDatas.at(m_nLastIndex)->getroomID());
	ctx.AddMember("room_number", roomId->getCString(), alloc);
	ctx.AddMember("timer_id", "0", alloc);
	ctx.AddMember("msg_head", "dzpk", alloc);
	ctx.AddMember("game_number", "dzpk", alloc);
	ctx.AddMember("area_number", "normal", alloc);
	ctx.AddMember("cps_record_id", "0", alloc);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	ctx.Accept(writer);

	const char *jsonData = buffer.GetString();	

	NetBean::getInstance()->addQQHead(&stream);
	NetBean::getInstance()->addHead(&stream, strlen(jsonData) + 1, 370001);
	NetBean::getInstance()->addData(&stream, (char *)jsonData, strlen(jsonData));
	NetBean::getInstance()->write(stream);
}

void GameHallScene::onMessage(char *buff, int len){
	rapidjson::Document docEnterRoom;
	docEnterRoom.Parse<0>(buff);

	//根据服务器返回信息，判断是否进入房间成功，若进入房间成功，切换到房间场景
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

void GameHallScene::btJoinTableMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	if(m_nLastIndex == -1){
		m_nLastIndex = 0;
	}
    
    loadLayer = LoadLayer::create();
    this->addChild(loadLayer);

	//数据在m_roomDatas中，发送进入房间消息给服务器
	NetBean::getInstance()->setNetDelegate((NetDelegate *)this);
    auto socketIp = GlobalDef::getInstance()->getSocketIp();
	NetBean::getInstance()->setAddress(socketIp.c_str(), 8006);
//	NetBean::getInstance()->setAddress("192.168.1.101", 8006);
	NetBean::getInstance()->connect();
}

void GameHallScene::btGoBackMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	Scene* mainScene = MainScene::createScene();
	Director::getInstance()->replaceScene(mainScene);
}

void GameHallScene::btnRefreshRoomMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	getGameHallData(m_curRoom, hideEmptyRoom);
}

void GameHallScene::checked(bool bChecked)
{
	hideEmptyRoom = bChecked;
	getGameHallData(m_curRoom, hideEmptyRoom);
}

void GameHallScene::clicked(int nRoom)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	m_curRoom = (PAGE_PANE_ROOM_FLAG)nRoom;
	getGameHallData((PAGE_PANE_ROOM_FLAG)nRoom, hideEmptyRoom);
}

void GameHallScene::getGameHallData(PAGE_PANE_ROOM_FLAG nRoom, bool hideRoomEmpty)
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
	req.AddMember("t", "getroominfo", alloc);
	switch (nRoom)
	{
	case ROOM_QUICK:
		req.AddMember("blind_area_start", "1", alloc);
		req.AddMember("blind_area_end", "20000", alloc);
		break;
	case ROOM_SENIOR:
		req.AddMember("blind_area_start", "5000", alloc);
		req.AddMember("blind_area_end", "20000", alloc);
		break;
	case ROOM_MIDDLE:
		req.AddMember("blind_area_start", "200", alloc);
		req.AddMember("blind_area_end", "2000", alloc);
		break;
	case ROOM_JUNIOR:
		req.AddMember("blind_area_start", "2", alloc);
		req.AddMember("blind_area_end", "100", alloc);
		break;
	case ROOM_NEWBIE:
		req.AddMember("blind_area_start", "1", alloc);
		req.AddMember("blind_area_end", "1", alloc);
		break;
	default:
		break;
	}
	if(hideRoomEmpty){
		req.AddMember("hide_room_empty", "1", alloc);
	}
	else{
		req.AddMember("hide_room_empty", "0", alloc);
	}
	req.AddMember("hide_room_full", "0", alloc);
	req.AddMember("room_type", "2", alloc);
	req.AddMember("bet_time", "20", alloc);
	
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
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameHallScene::onHttpRequestCompleted, this));

	// write the post data
	request->setRequestData(encryptData.c_str(), encryptData.size()); 
	request->setTag("POST Binary test");
	HttpClient::getInstance()->send(request);
	request->release();
}

void GameHallScene::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
	std::string encodeRecvData; //接收到的base64加密数据
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		printf("%c", (*buffer)[i]);
		encodeRecvData.push_back((*buffer)[i]);
	}
	const std::string recvData = gloox::Base64::decode64(encodeRecvData); //解密后的数据
	m_roomDatas.clear();
	m_roomDatas = GameHallData::parseData(recvData);
	tableView->reloadData();
}


Size GameHallScene::cellSizeForTable(TableView *table){
	return Size(942, 45);
}

TableViewCell* GameHallScene::tableCellAtIndex(TableView *table, ssize_t idx)
{ 
	TableViewCell *cell = new TableViewCell();
	cell->setContentSize(Size(942, 45));
	GameHallData* pData = m_roomDatas.at(idx);

	if (idx % 2 == 0)
	{
		Sprite *cell_bg = Sprite::create("hall_cell_white_bg.png");
		cell_bg->setAnchorPoint(Vec2(0, 0));
		cell_bg->setPosition(Vec2(4, 0));   
		cell_bg->setZOrder(-100);
		cell->addChild(cell_bg);
	}
	else{	
		Sprite *cell_bg = Sprite::create("hall_cell_dark_bg.png");
		cell_bg->setAnchorPoint(Vec2(0, 0));
		cell_bg->setPosition(Vec2(4, 0));   
		cell_bg->setZOrder(-100);
		cell->addChild(cell_bg);
	}

	__String *roomid = __String::createWithFormat("%ld/", pData->getroomID());
	roomid->append(pData->getroomName());
	Label* roomidLabel = Label::createWithSystemFont(roomid->getCString(), "Arial", 20);
	roomidLabel->setContentSize(Size(260, 45));
	roomidLabel->setColor(Color3B(52, 52, 52));
	roomidLabel->setPosition(145, 22);
	cell->addChild(roomidLabel);

	__String *blind = __String::createWithFormat("%ld/%ld", pData->getsmall_Blind(), 2 * pData->getsmall_Blind());
	Label* blindLabel = Label::createWithSystemFont(blind->getCString(), "Arial", 20);
	blindLabel->setColor(Color3B(52, 52, 52));
	blindLabel->setPosition(365, 22);
	cell->addChild(blindLabel);

	__String *bring = __String::createWithFormat("%ld/%ld", pData->getminBring(), pData->getmaxBring());
	Label* bringLabel = Label::createWithSystemFont(bring->getCString(), "Arial", 20);
	bringLabel->setColor(Color3B(52, 52, 52));
	bringLabel->setPosition(595, 22);
	cell->addChild(bringLabel);

	Sprite *seat = Sprite::create("hall_seat_bg.png");   
	seat->setPosition(Vec2(760, 15));  
	seat->setAnchorPoint(Vec2(0, 0));
	cell->addChild(seat); 

	float percents = pData->getcur_Seat_Person() * 1.0 / pData->getroom_Seat_Number();
	Sprite* person = Sprite::create("hall_person_bg.png");
	ProgressTimer* setProgressTimer = ProgressTimer::create(person);
	setProgressTimer->setType(ProgressTimer::Type::BAR);
	setProgressTimer->setMidpoint(Vec2(0, 0));
	setProgressTimer->setBarChangeRate(Vec2(1, 0));
	setProgressTimer->setPosition(761, 17);
	setProgressTimer->setAnchorPoint(Vec2(0, 0));
	setProgressTimer->setPercentage(percents * 100);
	cell->addChild(setProgressTimer);
	
	__String *percent = __String::createWithFormat("%ld/%ld", pData->getcur_Seat_Person(), pData->getroom_Seat_Number());
	Label* percentLabel = Label::createWithSystemFont(percent->getCString(), "Arial", 15);
	percentLabel->setColor(Color3B::WHITE);
	percentLabel->setPosition(815, 16);
	percentLabel->setAnchorPoint(Vec2(0, 0));
	cell->addChild(percentLabel);

	cell->setTag(idx);
	return cell;   
}

ssize_t GameHallScene::numberOfCellsInTableView(TableView *table){
	return m_roomDatas.size();
}

void GameHallScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if(doubleClicked){        //双击事件
		doubleClicked = false;

        loadLayer = LoadLayer::create();
        this->addChild(loadLayer);
		//数据在m_roomDatas中，发送进入房间消息给服务器
		NetBean::getInstance()->setNetDelegate((NetDelegate *)this);
		auto socketIp = GlobalDef::getInstance()->getSocketIp();
        NetBean::getInstance()->setAddress(socketIp.c_str(), 8006);
//		NetBean::getInstance()->setAddress("192.168.1.101", 8006);
		NetBean::getInstance()->connect();
		return;
	}
	else{	
		 //延时0.25s执行（注意在这0.25s的延时过程中clicked已经为true），
        //如果在这个过程中再次click，那么就执行上面的双击事件处理了
        //否则，那么就执行下面的回调函数了，处理单击事件
        scheduleOnce(schedule_selector(GameHallScene::onDoubleClicked), 0.25f);
        doubleClicked = true;
	}

	if (m_nLastIndex >= 0)
	{
		TableViewCell* pLastCell = table->cellAtIndex(m_nLastIndex);
		if (pLastCell)
		{
			pLastCell->removeChildByTag(HALL_CELL_SELECTED_TAG);
		}
	}

	Sprite *cell_selected = Sprite::create("hall_cell_selected_bg.png");
	cell_selected->setAnchorPoint(Vec2(0, 0));
	cell_selected->setPosition(Vec2(4, 0));
	cell_selected->setContentSize(Size(925, 45));
	cell_selected->setTag(HALL_CELL_SELECTED_TAG);
	cell_selected->setZOrder(-90);
	cell->addChild(cell_selected);

	m_nLastIndex = cell->getTag();
}

//在回调函数中处理单击事件
void GameHallScene::onDoubleClicked(float dt){
	 if(doubleClicked){               //单机事件
        doubleClicked = false;
    }
}