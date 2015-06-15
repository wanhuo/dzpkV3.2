#include "LoginScene.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "../GlobalDef/GlobalDef.h"
#include "SimpleAudioEngine.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../EncryptAndDecrypt/Md5/md5.h"
#include "MainScene.h"
#include "../GlobalDef/ResourcePreload.h"
#include "../GlobalDef/GlobalNode.h"

using namespace CocosDenshion;
using namespace network;

Scene* LoginScene::createScene(){
	Scene* scene = Scene::create();

	auto layer = LoginScene::create();

	scene->addChild(layer);

	return scene;
}

LoginScene::~LoginScene(){
	loadLayer->release();
}

bool LoginScene::init(){
	if(!Layer::init()){
		return false;
	}
    
    gameScene = nullptr;

	GlobalNode::getInstance();    //添加全局刷新节点

	roomId = 0;

	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	//大背景，填充黑框
	Sprite *big_bg = Sprite::create("login_bg.png");
	big_bg->setPosition(visibleSize	.width / 2, visibleSize.height / 2);
	this->addChild(big_bg);
    
    auto login_menu_bg = Sprite::create("login_menu_bg.png");
    login_menu_bg->setAnchorPoint(Vec2(0.5, 0.5));
    login_menu_bg->setPosition(visibleSize.width - 260, visibleSize.height / 2);
    this->addChild(login_menu_bg);

	//QQ登录菜单
	MenuItemImage *btn_qq_login_menuItem = MenuItemImage::create("btn_qq_login_up.png", "btn_qq_login_down.png", CC_CALLBACK_1(LoginScene::qqLoginMenuCallback, this));
    btn_qq_login_menuItem->setAnchorPoint(Vec2(0.5, 0.5));
	btn_qq_login_menuItem->setPosition(visibleSize.width - 260 + 10, visibleSize.height / 2 - 8);

	//游客登录菜单
	MenuItemImage *btn_login_guest_menuItem = MenuItemImage::create("btn_login_guest_up.png", "btn_login_guest_down.png", CC_CALLBACK_1(LoginScene::guestLoginMenuCallback, this));
    btn_login_guest_menuItem->setAnchorPoint(Vec2(0.5, 0.5));
	btn_login_guest_menuItem->setPosition(visibleSize.width - 260 + 10, visibleSize.height / 2 - 120);

	auto btn_menu = Menu::create(btn_qq_login_menuItem, btn_login_guest_menuItem, NULL);
	btn_menu->setPosition(0, 0);
	this->addChild(btn_menu);

	//加载层，加载时弹出，加载完之后删除
	ResourcePreload::getInstance()->preload();

	loadLayer = LoadLayer::create();
	loadLayer->retain();

	scheduleUpdate();

	return true;
}

void LoginScene::update(float dt){
	//服务器连接成功后会发送消息到队列，update检测消息队列是否连接成功，成功时进入下一个界面

}

//读取数据库信息并登录
void LoginScene::readSqlAndLogin(){
	DBUtil* dbUtil = DBUtil::getInstance();
	UserInfoTable userInfoData;
	UserInfoTable* table_data = NULL;
	std::string strsql = std::string("select user_id, account, password, sessionid from userInfo");
	dbUtil->getDataInfo(strsql, &userInfoData);
	if(userInfoData.m_datas.size() > 0){
		table_data = (UserInfoTable *)userInfoData.m_datas.at(0);

		//json数据构造
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
		rapidjson::Value ctx(rapidjson::kObjectType);
		ctx.AddMember("chan", "0", alloc);
		ctx.AddMember("mver", "1", alloc);
		auto account = table_data->getAccount();
		auto password = table_data->getPassword();
		ctx.AddMember("user_id", table_data->getUserId().c_str(), alloc);
		ctx.AddMember("account", account.c_str(), alloc);
		ctx.AddMember("sessionid", table_data->getSessionId().c_str(), alloc);
		ctx.AddMember("ter", "web", alloc);
		srand((int)time(NULL));
		auto serial = __String::createWithFormat("%d", rand() % 100000);
		ctx.AddMember("serial", serial->getCString(), alloc);
	
		rapidjson::Value req(rapidjson::kObjectType);
		req.AddMember("t", "login", alloc);
		req.AddMember("account", account.c_str(), alloc);
		//md5对用户名和密码加密
		std::string str = account + password;
		MD5 pass(str);
		str = pass.md5();
		char cstr[33];
		for(int i = 0; i < 33; i++){  //直接使用str.c_str()会有问题，得不到正确的数据
			cstr[i] = str[i];
		}
		req.AddMember("password", cstr, alloc);

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
		request->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpRequestCompleted, this));
    
		// write the post data
		request->setRequestData(encryptData.c_str(), encryptData.size()); 
		request->setTag("POST Binary test");
		HttpClient::getInstance()->send(request);
		request->release();
	}
}

//QQ登录回调方法
void LoginScene::qqLoginMenuCallback(Ref *object){
	if(GlobalDef::getInstance()->getSoundOn()){
        GlobalDef::getInstance()->getVolumnValue();
		SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
//	this->addChild(loadLayer);
    
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

//游客登录回调方法
void LoginScene::guestLoginMenuCallback(Ref *object){
	if(GlobalDef::getInstance()->getSoundOn()){
        GlobalDef::getInstance()->getVolumnValue();
		SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
	this->addChild(loadLayer);

	//游客登录，如果本地没有用户信息，说明是新用户，发送新用户登录信息给服务器
	DBUtil* dbUtil = DBUtil::getInstance();
//    std::string dropTableSql = "DROP TABLE userInfo";
//    dbUtil->deleteTable(dropTableSql, "userInfo");
	UserInfoTable userInfoData;
	UserInfoTable* table_data = NULL;
	std::string strsql = std::string("select user_id, account, password, sessionid from userInfo");
	dbUtil->getDataInfo(strsql, &userInfoData);
	if(userInfoData.m_datas.size() > 0){
		table_data = (UserInfoTable *)userInfoData.m_datas.at(0);
		if(strcmp(table_data->getAccount().c_str(), "") == 0){
			registerNewGuest();
		}
		else{			 //读取数据卡账户信息，登录游戏
			readSqlAndLogin();
		}
	}
	else{
		registerNewGuest();
	}
}

void LoginScene::registerNewGuest(){
	rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
    rapidjson::Value ctx(rapidjson::kObjectType);
    ctx.AddMember("ter", "IOS", alloc);                 //she bei lei xing: IOS, WEB, ANDROID
    ctx.AddMember("chan", "NA", alloc);                 //qu dao shang hao ma
    ctx.AddMember("sessionid", "", alloc);
    ctx.AddMember("user_id", "", alloc);
    ctx.AddMember("mver", "1", alloc);
    srand((int)time(NULL));
    auto serial = __String::createWithFormat("%d", rand() % 100000);
    ctx.AddMember("serial", serial->getCString(), alloc);

    rapidjson::Value req(rapidjson::kObjectType);
    req.AddMember("t", "reguatourist", alloc);

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
    request->setResponseCallback(CC_CALLBACK_2(LoginScene::onNewGuestLoginHttpRequestCompleted, this));
    
    // write the post data
    request->setRequestData(encryptData.c_str(), encryptData.size());
    request->setTag("POST Binary test");
    HttpClient::getInstance()->send(request);
    request->release();
}

//新用户第一次注册登陆回调方法
void LoginScene::onNewGuestLoginHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
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

	//json数据解析
	rapidjson::Document docGuestLoginRecv;
	docGuestLoginRecv.Parse<0>(recvData.c_str());

//	std::string userId = docGuestLoginRecv["resp"]["user_id"].GetString();
	std::string userId = "";
	std::string account = docGuestLoginRecv["resp"]["account"].GetString();
	std::string password = docGuestLoginRecv["resp"]["password"].GetString();
//	std::string sessionId = docGuestLoginRecv["ctx"]["sessionid"].GetString();
	std::string sessionId = "";

	auto dbUtil = DBUtil::getInstance();
	std::string sqlCreateUserInfoTable = "create table userInfo(user_id text,account text,password text,sessionid text)";
	dbUtil->createTable(sqlCreateUserInfoTable, "userInfo");
	std::string insertSql = std::string("insert into userInfo (user_id, account, password, sessionid) values('") + userId + std::string("', '") + account + std::string("', '") + password + std::string("', '") + sessionId + std::string("')");

	dbUtil->insertData(insertSql);

	//读取数据库账户信息，登录
	readSqlAndLogin();
}

//用户登陆回调方法
void LoginScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response){
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

	//json数据解析
	rapidjson::Document docLoginRecv;
	docLoginRecv.Parse<0>(recvData.c_str());
	if(atoi(docLoginRecv["ctx"]["err"].GetString()) == 0){
		const char *sessionId = docLoginRecv["ctx"]["sessionid"].GetString();
		const char *userId = docLoginRecv["resp"]["user_id"].GetString();
		const char *account = docLoginRecv["resp"]["account"].GetString();
		roomId = atoi(docLoginRecv["resp"]["roomId"].GetString());
		srand((int)time(NULL));
		char serial[6];
		sprintf(serial, "%d", rand() % 100000);

		UserInfo::getInstance()->setSessionId(sessionId);

		//拉取个人信息json数据构造
		rapidjson::Document docGetUserInfo;
		docGetUserInfo.SetObject();
		rapidjson::Document::AllocatorType &allocGetUserInfo = docGetUserInfo.GetAllocator();
		rapidjson::Value ctx(rapidjson::kObjectType);
		ctx.AddMember("chan", "0", allocGetUserInfo);
		ctx.AddMember("mver", "1", allocGetUserInfo);
		ctx.AddMember("user_id", userId, allocGetUserInfo);
		ctx.AddMember("account", account, allocGetUserInfo);
		ctx.AddMember("sessionid", sessionId, allocGetUserInfo);
		ctx.AddMember("ter", "web", allocGetUserInfo);
		ctx.AddMember("serial", serial, allocGetUserInfo);

		rapidjson::Value req(rapidjson::kObjectType);
		req.AddMember("t", "getuserinfo", allocGetUserInfo);
	//	req.AddMember("other_id", "", allocGetUserInfo);
	
		docGetUserInfo.AddMember("ctx", ctx, allocGetUserInfo);
		docGetUserInfo.AddMember("req", req, allocGetUserInfo);

		rapidjson::StringBuffer strBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strBuffer);
		docGetUserInfo.Accept(writer);

		const char* jsonDataGetUserInfo = strBuffer.GetString();

		//base64加密
		const std::string encryptData = gloox::Base64::encode64(jsonDataGetUserInfo);

		HttpRequest *requestGetUserInfo = new HttpRequest();
		auto httpServerAddress = GlobalDef::getInstance()->getHttpServerAddress();
        requestGetUserInfo->setUrl(httpServerAddress.c_str());
		requestGetUserInfo->setRequestType(HttpRequest::Type::POST);
		requestGetUserInfo->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpRequestGetUserInfoCompleted, this));

		requestGetUserInfo->setRequestData(encryptData.c_str(), encryptData.size());
		requestGetUserInfo->setTag("Post Get User Info");
		HttpClient::getInstance()->send(requestGetUserInfo);
		requestGetUserInfo->release();
	}
}

//获取用户信息回调方法
void LoginScene::onHttpRequestGetUserInfoCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
	if(!response){
		return;
	}
	if(!response->isSucceed()){
		log("Get User Info Failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	//提取个人信息，填充个人信息类
	std::vector<char> *buffer = response->getResponseData();
	std::string encodeRecvData;
	for(unsigned int i = 0; i < buffer->size(); i++){
		encodeRecvData.push_back((*buffer)[i]);
	}
	const std::string recvDataUserInfo = gloox::Base64::decode64(encodeRecvData);
	const char *data = recvDataUserInfo.c_str();

	//json数据解析
	rapidjson::Document docUserInfo;
	docUserInfo.Parse<0>(data);

	UserInfo::getInstance()->setUserId(docUserInfo["resp"]["userinfo"]["user_id"].GetString());
	UserInfo::getInstance()->setUserAccount(docUserInfo["resp"]["userinfo"]["user_account"].GetString());
	UserInfo::getInstance()->setPersonalSign(docUserInfo["resp"]["userinfo"]["personal_sign"].GetString());
	UserInfo::getInstance()->setNikeName(docUserInfo["resp"]["userinfo"]["nick_name"].GetString());
	UserInfo::getInstance()->setSex(docUserInfo["resp"]["userinfo"]["sex"].GetString());
	UserInfo::getInstance()->setAge(docUserInfo["resp"]["userinfo"]["age"].GetString());
	UserInfo::getInstance()->setBirthday(docUserInfo["resp"]["userinfo"]["birthday"].GetString());
	UserInfo::getInstance()->setIsYellowVip(docUserInfo["resp"]["userinfo"]["isyellowvip"].GetString());
	UserInfo::getInstance()->setOnlineStatus(docUserInfo["resp"]["userinfo"]["online_status"].GetString());
	UserInfo::getInstance()->setLastIpAddress(docUserInfo["resp"]["userinfo"]["last_ip_address"].GetString());
	UserInfo::getInstance()->setLastOnlineTime(docUserInfo["resp"]["userinfo"]["last_online_time"].GetString());
	UserInfo::getInstance()->setRealName(docUserInfo["resp"]["userinfo"]["real_name"].GetString());
//	UserInfo::getInstance()->setIdCardNum(docUserInfo["resp"]["userinfo"]["id_card_number"].GetString());
//	UserInfo::getInstance()->setCellPhoneNum(docUserInfo["resp"]["userinfo"]["cell_phone_number"].GetString());
	UserInfo::getInstance()->setEmail(docUserInfo["resp"]["userinfo"]["e_mail"].GetString());
	UserInfo::getInstance()->setCountry(docUserInfo["resp"]["userinfo"]["country"].GetString());
//	UserInfo::getInstance()->setProvince(docUserInfo["resp"]["userinfo"]["province"].GetString());
	UserInfo::getInstance()->setCity(docUserInfo["resp"]["userinfo"]["city"].GetString());
	UserInfo::getInstance()->setArea(docUserInfo["resp"]["userinfo"]["area"].GetString());
	UserInfo::getInstance()->setDzpkLevel(docUserInfo["resp"]["userinfo"]["dzpk_level"].GetString());
	UserInfo::getInstance()->setDzpkExperience(docUserInfo["resp"]["userinfo"]["dzpk_experience"].GetString());
	UserInfo::getInstance()->setVipLevel(docUserInfo["resp"]["userinfo"]["vip_level"].GetString());
	UserInfo::getInstance()->setVipPath(docUserInfo["resp"]["userinfo"]["vippath"].GetString());
	UserInfo::getInstance()->setCoinAccount(docUserInfo["resp"]["userinfo"]["coin_account"].GetString());
	UserInfo::getInstance()->setChipAccount(docUserInfo["resp"]["userinfo"]["chip_account"].GetString());
	UserInfo::getInstance()->setBestWinCard(docUserInfo["resp"]["userinfo"]["best_win_card"].GetString());
	UserInfo::getInstance()->setRankName(docUserInfo["resp"]["userinfo"]["rank_name"].GetString());
    UserInfo::getInstance()->setHeadPhotoSerial(docUserInfo["resp"]["userinfo"]["head_photo_serial"].GetString());
	UserInfo::getInstance()->setTotalTime(docUserInfo["resp"]["userinfo"]["total_time"].GetString());
	UserInfo::getInstance()->setWinPer(docUserInfo["resp"]["userinfo"]["win_per"].GetString());
	UserInfo::getInstance()->setBigestWinChip(docUserInfo["resp"]["userinfo"]["bigest_win_chip"].GetString());
	UserInfo::getInstance()->setBigestChip(docUserInfo["resp"]["userinfo"]["bigest_chip"].GetString());
	UserInfo::getInstance()->setFriendCount(docUserInfo["resp"]["userinfo"]["friendcount"].GetString());

	//解析json数据，继续http请求数据，然后进入mainScene界面
	if(roomId == 0){
		Scene* mainScene = MainScene::createScene();
		Director::getInstance()->replaceScene(mainScene);
	}
	else if(roomId > 0){        //断线重连
		NetBean::getInstance()->setNetDelegate((NetDelegate *)this);
		auto socketIp = GlobalDef::getInstance()->getSocketIp();
        NetBean::getInstance()->setAddress(socketIp.c_str(), 8006);
		NetBean::getInstance()->connect();
	}
}

void LoginScene::onCreate(){

}

void LoginScene::onConnected(){
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
	__String* strRoomId = __String::createWithFormat("%d", roomId);
	ctx.AddMember("room_number", strRoomId->getCString(), alloc);
	roomId = 0;
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
	NetBean::getInstance()->addData(&stream, (char *)jsonData, (int)strlen(jsonData));
	NetBean::getInstance()->write(stream);
}

void LoginScene::onMessage(char *buff, int len){
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