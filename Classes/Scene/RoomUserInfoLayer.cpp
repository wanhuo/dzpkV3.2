//
//  RoomUserInfoLayer.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-20.
//
//

#include "RoomUserInfoLayer.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"

RoomUserInfoLayer* RoomUserInfoLayer::create(int seatNum, int seatDelta, ROOM_USER_INFO_T *userInfo, Texture2D* texture){
    RoomUserInfoLayer* layer = new RoomUserInfoLayer();
    if(layer){
        layer->seatNum = seatNum;
        layer->seatDelta = seatDelta;
        layer->userInfo = userInfo;
        layer->texture = texture;
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

bool RoomUserInfoLayer::init(){
    if(!Layer::init()){
        return false;
    }
    
    touchedCell = nullptr;
    
    visibleSize = Director::getInstance()->getVisibleSize();
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
    centerPositionFromScreenCenter.x = GlobalDef::getInstance()->getUserInfoLayerCenterPosition(newSeatNum, 0);
    centerPositionFromScreenCenter.y = GlobalDef::getInstance()->getUserInfoLayerCenterPosition(newSeatNum, 1);
    
    btnRect.origin.x = visibleSize.width / 2 + centerPositionFromScreenCenter.x - 260;
    btnRect.origin.y = visibleSize.height / 2 + centerPositionFromScreenCenter.y + 130;
    btnRect.size.width = 520;
    btnRect.size.height = 58;
    
    layerRect.origin.x = visibleSize.width / 2 + centerPositionFromScreenCenter.x - 271;
    layerRect.origin.y = visibleSize.height / 2 + centerPositionFromScreenCenter.y - 193;
    layerRect.size.width = 543;
    layerRect.size.height = 385;
    
    tableViewRect.origin.x = visibleSize.width / 2 + centerPositionFromScreenCenter.x - 266;
    tableViewRect.origin.y = visibleSize.height / 2 + centerPositionFromScreenCenter.y - 143 - 47;
    tableViewRect.size.width = 539;
    tableViewRect.size.height = 94;
    
    //背景
    auto room_user_info_layer_bg = Sprite::create("room_user_info_layer_bg.png");
    room_user_info_layer_bg->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x, visibleSize.height / 2 + centerPositionFromScreenCenter.y));
    this->addChild(room_user_info_layer_bg);
    
    //按钮
    room_user_btn_up = Sprite::create("room_user_btn_up.png");
    room_user_btn_up->setPosition(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 134, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 156);
    room_user_btn_up->setVisible(false);
    this->addChild(room_user_btn_up, 1);
    
    room_user_btn_down = Sprite::create("room_user_btn_down.png");
    room_user_btn_down->setPosition(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 134, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 156);
    this->addChild(room_user_btn_down, 1);
    
    room_card_btn_up = Sprite::create("room_card_btn_up.png");
    room_card_btn_up->setPosition(visibleSize.width / 2 + centerPositionFromScreenCenter.x + 131, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 156);
    this->addChild(room_card_btn_up, 1);
    
    room_card_btn_down = Sprite::create("room_card_btn_down.png");
    room_card_btn_down->setPosition(visibleSize.width / 2 + centerPositionFromScreenCenter.x + 131, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 156);
    room_card_btn_down->setVisible(false);
    this->addChild(room_card_btn_down, 1);
    
    //个人信息页面
    //头像
    Sprite* photoSprite = nullptr;
    if(this->texture != NULL){
        photoSprite = Sprite::create("default_photo88.png");
        photoSprite->setTexture(this->texture);
    }
    else{
        photoSprite = Sprite::create("default_photo88.png");
    }
    photoSprite->setTag(PHOTO_SPRITE_TAG);
    photoSprite->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 140, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 15));
    photoSprite->setScale(1.9f);
    this->addChild(photoSprite);
    
    //基本信息
    auto userAccount = this->userInfo->getNickName();
    auto chineseNickName = GlobalChineseXml::getInstance()->getValue("nickName");
    auto strUserAccount = __String::create(chineseNickName);
    strUserAccount->append(userAccount);
    auto userAccountLabel = Label::createWithSystemFont(strUserAccount->getCString(), "Arial", 27);
    userAccountLabel->setTag(USER_ACCOUNT_LABEL_TAG);
    userAccountLabel->setDimensions(260, 30);
    userAccountLabel->setAnchorPoint(Vec2(0, 1));
    userAccountLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 10, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 110));
    this->addChild(userAccountLabel);
    
    auto userId = this->userInfo->getUserId();
    auto chineseUserId = GlobalChineseXml::getInstance()->getValue("gameId");
    auto strUserId = __String::create(chineseUserId);
    strUserId->append(userId);
    auto userIdLabel = Label::createWithSystemFont(strUserId->getCString(), "Arial", 27);
    userIdLabel->setTag(USER_ID_LABEL_TAG);
    userIdLabel->setAnchorPoint(Vec2(0, 1));
    userIdLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 10, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 70));
    this->addChild(userIdLabel);
    
    auto chineseChipAccount = GlobalChineseXml::getInstance()->getValue("chipCount");
    auto strChipAccount = __String::create(chineseChipAccount);
    auto userChipAccount = this->userInfo->getChipAccount();
    strChipAccount->appendWithFormat("%ld", userChipAccount);
    auto userChipAccountLabel = Label::createWithSystemFont(strChipAccount->getCString(), "Arial", 27);
    userChipAccountLabel->setTag(USER_CHIP_ACCOUNT_LABEL_TAG);
    userChipAccountLabel->setAnchorPoint(Vec2(0, 1));
    userChipAccountLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 10, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 30));
    this->addChild(userChipAccountLabel);
    
    auto chineseDzpkLevel = GlobalChineseXml::getInstance()->getValue("dzpkLevel");
    auto strLevel = __String::create(chineseDzpkLevel);
    auto userLevel = this->userInfo->getDzpkLevel();
    strLevel->appendWithFormat("%d", userLevel);
    auto userLevelLabel = Label::createWithSystemFont(strLevel->getCString(), "Arial", 27);
    userLevelLabel->setTag(USER_DZPK_LEVEL_LABEL_TAG);
    userLevelLabel->setAnchorPoint(Vec2(0, 1));
    userLevelLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 10, visibleSize.height / 2 + centerPositionFromScreenCenter.y - 10));
    this->addChild(userLevelLabel);
    //互动道具
    //互动道具背景
    auto userChopBg = Sprite::create("user_chop_bg.png");
    userChopBg->setTag(USER_CHOP_BG_TAG);
    userChopBg->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x, visibleSize.height / 2 + centerPositionFromScreenCenter.y - 143));
    this->addChild(userChopBg);
    //互动道具tableview
    tableView = TableView::create(this, Size(539, 94));
	tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tableView->setAnchorPoint(Vec2(0, 0));
	tableView->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 266, visibleSize.height / 2 + centerPositionFromScreenCenter.y - 143 - 47));
	tableView->setDelegate(this);
	this->addChild(tableView);
    
    //牌局信息页面
    auto userTotalTimeLabel = Label::createWithSystemFont("", "Arial", 27);
    userTotalTimeLabel->setTag(USER_TOTAL_TIME_LABEL_TAG);
    userTotalTimeLabel->setAnchorPoint(Vec2(0, 1));
    userTotalTimeLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 240, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 110));
    this->addChild(userTotalTimeLabel);
    
    auto userWinAndFailTimeLabel = Label::createWithSystemFont("", "Arial", 27);
    userWinAndFailTimeLabel->setTag(USER_WIN_AND_FAIL_TIME_LABEL_TAG);
    userWinAndFailTimeLabel->setAnchorPoint(Vec2(0, 1));
    userWinAndFailTimeLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 240, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 70));
    this->addChild(userWinAndFailTimeLabel);
    
    auto userWinPerLabel = Label::createWithSystemFont("", "Arial", 27);
    userWinPerLabel->setTag(USER_WIN_PER_LABEL_TAG);
    userWinPerLabel->setAnchorPoint(Vec2(0, 1));
    userWinPerLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 240, visibleSize.height / 2 + centerPositionFromScreenCenter.y + 30));
    this->addChild(userWinPerLabel);
    
    auto biggestWinChipLabel = Label::createWithSystemFont("", "Arial", 27);
    biggestWinChipLabel->setTag(BIGGEST_WIN_CHIP_LABEL_TAG);
    biggestWinChipLabel->setAnchorPoint(Vec2(0, 1));
    biggestWinChipLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 240, visibleSize.height / 2 + centerPositionFromScreenCenter.y - 10));
    this->addChild(biggestWinChipLabel);
    
    auto strBiggestHandCard = GlobalChineseXml::getInstance()->getValue("biggestHandCard");
    auto biggestHandCardLabel = Label::createWithSystemFont(strBiggestHandCard, "Arial", 27);
    biggestHandCardLabel->setTag(BIGGEST_HAND_CARD_LABEL_TAG);
    biggestHandCardLabel->setAnchorPoint(Vec2(0, 1));
    biggestHandCardLabel->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 240, visibleSize.height / 2 + centerPositionFromScreenCenter.y - 50));
    this->addChild(biggestHandCardLabel);
    
    //5张手牌
    for (int i = 0; i < 5; i++) {
        auto handCard = Sprite::create("p0a.png");
        handCard->setTag(USER_CARD_INFO_HAND_CARD_TAG + i);
        handCard->setScale(0.9f);
        handCard->setAnchorPoint(Vec2(0, 1));
        handCard->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x - 100 + 60 * i, visibleSize.height / 2 + centerPositionFromScreenCenter.y - 60));
        this->addChild(handCard);
    }
    
    //隐藏牌局信息页面
    showAndHideCardInfo(false);
    
    //∆¡±Œ¥•√˛≤„
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* t, Event* e){
        log("RoomUserInfoLayer Touched");
        auto touchPosition = Director::getInstance()->convertToGL(t->getLocationInView());
        if(layerRect.containsPoint(touchPosition)){
            m_beginPoint = touchPosition;
            return true;
        }
        else{
            //移除弹出框
            this->removeFromParent();
            
            return false;
        }
        
		return false;
	};
    
    listener->onTouchMoved = [](Touch* t, Event* e){
        
    };
    
    listener->onTouchEnded = [=](Touch* t, Event* e){
        auto touchPosition = Director::getInstance()->convertToGL(t->getLocationInView());
        if(btnRect.containsPoint(touchPosition)){
            m_endPoint = touchPosition;
            if(fabs(m_endPoint.x - m_beginPoint.x) < 10.0f && fabs(m_endPoint.y - m_beginPoint.y) < 10.0f){
                int itemIndex = (int)((m_beginPoint.x - btnRect.origin.x) / (btnRect.size.width / 2));
                if(itemIndex == 0){
                    if(!room_card_btn_up->isVisible()){
                        room_card_btn_up->setVisible(true);
                    }
                    if(room_card_btn_down->isVisible()){
                        room_card_btn_down->setVisible(false);
                    }
                    if(room_user_btn_up->isVisible()){
                        room_user_btn_up->setVisible(false);
                    }
                    if(!room_user_btn_down->isVisible()){
                        room_user_btn_down->setVisible(true);
                    }
                    
                    showAndHideCardInfo(false);
                    showAndHideUserInfo(true);
                }
                else if(itemIndex == 1){
                    if(!room_user_btn_up->isVisible()){
                        room_user_btn_up->setVisible(true);
                    }
                    if(room_user_btn_down->isVisible()){
                        room_user_btn_down->setVisible(false);
                    }
                    if(room_card_btn_up->isVisible()){
                        room_card_btn_up->setVisible(false);
                    }
                    if(!room_card_btn_down->isVisible()){
                        room_card_btn_down->setVisible(true);
                    }
                    
                    showAndHideUserInfo(false);
                    showAndHideCardInfo(true);
                    
                    //弹出刷新界面，发送http请求获取玩家信息
                    //刷新
                    auto refreshSprite = Sprite::create("sp_loading.png");
                    refreshSprite->setTag(REFRESH_SPRITE_TAG);
                    refreshSprite->setPosition(Vec2(visibleSize.width / 2 + centerPositionFromScreenCenter.x, visibleSize.height / 2 + centerPositionFromScreenCenter.y));
                    this->addChild(refreshSprite);
                    
                    refreshSprite->runAction(RepeatForever::create(RotateBy::create(1, -360.0)));
                    
                    //发送http请求
                    rapidjson::Document getUserInfodoc;
                    getUserInfodoc.SetObject();
                    rapidjson::Document::AllocatorType& alloc = getUserInfodoc.GetAllocator();
                    rapidjson::Value ctx(rapidjson::kObjectType);
                    ctx.AddMember("ter", "web", alloc);
                    auto userAccount = this->userInfo->getUserAccount();
                    ctx.AddMember("account", userAccount.c_str(), alloc);
                    ctx.AddMember("mver", "1", alloc);
                    auto strUserId = UserInfo::getInstance()->getUserId();
                    ctx.AddMember("user_id", strUserId.c_str(), alloc);
                    ctx.AddMember("chan", "0", alloc);
                    auto strSessionId = UserInfo::getInstance()->getSessionId();
                    ctx.AddMember("sessionid", strSessionId.c_str(), alloc);
                    srand((int)time(NULL));
                    auto serial = __String::createWithFormat("%d", rand() % 100000);
                    ctx.AddMember("serial", serial->getCString(), alloc);
                    
                    rapidjson::Value req(rapidjson::kObjectType);
                    req.AddMember("t", "getuserinfo", alloc);
                    auto otherUserId = this->userInfo->getUserId();
                    req.AddMember("other_id", otherUserId.c_str(), alloc);
                    
                    getUserInfodoc.AddMember("ctx", ctx, alloc);
                    getUserInfodoc.AddMember("req", req, alloc);
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    getUserInfodoc.Accept(writer);
                    
                    const char *jsonData = buffer.GetString();
                    
                    //base64º”√‹
                    const std::string encryptData = gloox::Base64::encode64(jsonData);
                    
                    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
                    auto httpServerAddress = GlobalDef::getInstance()->getHttpServerAddress();
                    request->setUrl(httpServerAddress.c_str());
                    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
                    request->setResponseCallback(CC_CALLBACK_2(RoomUserInfoLayer::onGetUserInfoHttpRequestCompleted, this));
                    
                    // write the post data
                    request->setRequestData(encryptData.c_str(), encryptData.size());
                    request->setTag("POST Binary test");
                    cocos2d::network::HttpClient::getInstance()->send(request);
                    request->release();
                }
            }
        }
        else if(tableViewRect.containsPoint(touchPosition)){
            log("touchPosition");
            if(touchedCell != NULL){
                auto selfUserId = UserInfo::getInstance()->getUserId();
                auto playerSelf = RoomData::getInstance()->getPlayer((char *)selfUserId.c_str());
                auto userId = this->userInfo->getUserId();
                if((strcmp(selfUserId.c_str(), userId.c_str()) == 0) || (playerSelf == NULL) || ((playerSelf != NULL) && (playerSelf->getSeatNum() == 0))){
                    
                }
                else{
                    int index = cellTouchedIndex + 1;
                    auto chopSprite = touchedCell->getChildByTag(index);
                    touchedCell = NULL;
                    
                    auto scaleBig = ScaleTo::create(0.15, 1.3);
                    auto scaleNormal = ScaleTo::create(0.15, 1.0);
                    auto callFunc = CallFunc::create([=](){
                        this->sendChopMsgDelegate->onSendChopMsg(this->seatNum, index);
                    });
                    
                    chopSprite->runAction(Sequence::create(scaleBig, scaleNormal, callFunc, NULL));
                }
            }
        }
    };
    
    listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->setPriority(listener, -128);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void RoomUserInfoLayer::onGetUserInfoHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
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
    
	//json ˝æ›Ω‚Œˆ
	rapidjson::Document docGetUserInfoRecv;
	docGetUserInfoRecv.Parse<0>(recvData.c_str());
    
	std::string strTotalTime = docGetUserInfoRecv["resp"]["userinfo"]["total_time"].GetString();
	std::string strWinPer = docGetUserInfoRecv["resp"]["userinfo"]["win_per"].GetString();
    std::string strBiggestWinChip = docGetUserInfoRecv["resp"]["userinfo"]["bigest_win_chip"].GetString();
	std::string strBestWinCard = docGetUserInfoRecv["resp"]["userinfo"]["best_win_card"].GetString();
    
    //总牌局信息
    auto chineseTotalTime = GlobalChineseXml::getInstance()->getValue("countTime");
    auto totalTimeLabelText = __String::create(chineseTotalTime);
    totalTimeLabelText->append(strTotalTime);
    auto bureau = GlobalChineseXml::getInstance()->getValue("bureau");
    totalTimeLabelText->append(bureau);
    ((Label *)this->getChildByTag(USER_TOTAL_TIME_LABEL_TAG))->setString(totalTimeLabelText->getCString());
    
    //胜负局数
    auto chineseWinAndFailTime = GlobalChineseXml::getInstance()->getValue("winAndFailTime");
    auto userWinPer = atoi(strWinPer.c_str());        //获取到的winper为整数，winper实际上为赢的次数，比如：30
    int userWinTime = userWinPer;
    int userTotalTime = atoi(strTotalTime.c_str());
    int userFailTime = userTotalTime - userWinTime;
    auto strWin = GlobalChineseXml::getInstance()->getValue("win");
    auto strFail = GlobalChineseXml::getInstance()->getValue("fail");
    
    auto winAndFailTimeLabelText = __String::create(chineseWinAndFailTime);
    winAndFailTimeLabelText->appendWithFormat("%d", userWinTime);
    winAndFailTimeLabelText->append(strWin);
    winAndFailTimeLabelText->appendWithFormat("/%d", userFailTime);
    winAndFailTimeLabelText->append(strFail);
    ((Label *)this->getChildByTag(USER_WIN_AND_FAIL_TIME_LABEL_TAG))->setString(winAndFailTimeLabelText->getCString());
    
    //胜率
    auto chineseWinPer = GlobalChineseXml::getInstance()->getValue("winPer");
    auto winPerLabelText = __String::create(chineseWinPer);
    winPerLabelText->appendWithFormat("%.2f", (userWinTime * 100.0 / userTotalTime));
    winPerLabelText->append("%");
    ((Label *)this->getChildByTag(USER_WIN_PER_LABEL_TAG))->setString(winPerLabelText->getCString());
    
    //最大赢取
    auto chineseBiggestWinChip = GlobalChineseXml::getInstance()->getValue("biggestWinChip");
    auto biggestWinChipLabelText = __String::create(chineseBiggestWinChip);
    biggestWinChipLabelText->append(strBiggestWinChip);
    ((Label *)this->getChildByTag(BIGGEST_WIN_CHIP_LABEL_TAG))->setString(biggestWinChipLabelText->getCString());
    
    //最大手牌
    for(int i = 0;i < 5; i++){
        int cardNum = atoi(strBestWinCard.substr(i * 4, 3).c_str());
        int color = cardNum / 100;
        int card = cardNum % 100;
        
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
        
        ((Sprite *)this->getChildByTag(USER_CARD_INFO_HAND_CARD_TAG + i))->setTexture(strCard->getCString());
    }
    
    auto refreshSprite = this->getChildByTag(REFRESH_SPRITE_TAG);
    if(refreshSprite != NULL){
        this->removeChildByTag(REFRESH_SPRITE_TAG);
    }
}

void RoomUserInfoLayer::setSendChopMsgDelegate(SendChopMsgDelegate* sendChopMsgDelegate){
    this->sendChopMsgDelegate = sendChopMsgDelegate;
}

void RoomUserInfoLayer::showAndHideUserInfo(bool show){
    auto selfUserId = UserInfo::getInstance()->getUserId();
    auto playerSelf = RoomData::getInstance()->getPlayer((char *)selfUserId.c_str());
    auto userId = this->userInfo->getUserId();
    
    if(show == true){
        auto photoSprite = (Sprite *)(this->getChildByTag(PHOTO_SPRITE_TAG));
        if (photoSprite != NULL && !(photoSprite->isVisible())) {
            photoSprite->setVisible(true);
        }
        
        auto userAccountLabel = (Label *)(this->getChildByTag(USER_ACCOUNT_LABEL_TAG));
        if(userAccountLabel != NULL && !(userAccountLabel->isVisible())){
            userAccountLabel->setVisible(true);
        }
        
        auto userIdLabel = (Label *)(this->getChildByTag(USER_ID_LABEL_TAG));
        if(userIdLabel != NULL && !(userIdLabel->isVisible())){
            userIdLabel->setVisible(true);
        }
        
        auto userChipAccountLabel = (Label *)(this->getChildByTag(USER_CHIP_ACCOUNT_LABEL_TAG));
        if(userChipAccountLabel != NULL && !(userChipAccountLabel->isVisible())){
            userChipAccountLabel->setVisible(true);
        }
        
        auto userDzpkLevelLabel = (Label *)(this->getChildByTag(USER_DZPK_LEVEL_LABEL_TAG));
        if(userDzpkLevelLabel != NULL && !(userDzpkLevelLabel->isVisible())){
            userDzpkLevelLabel->setVisible(true);
        }
        
        auto userChopBg = (Label *)(this->getChildByTag(USER_CHOP_BG_TAG));
        if(userChopBg != NULL && !(userChopBg->isVisible())){
            userChopBg->setVisible(true);
        }
        
        if(tableView != NULL && !(tableView->isVisible())){
            tableView->setVisible(true);
            
            auto cellCount = tableView->getContainer()->getChildrenCount();
            for(int i = 0; i < cellCount; i++){
                auto cell = (TableViewCell *)(tableView->getContainer()->getChildByTag(i));
                
                auto chopSprite = cell->getChildByTag(i + 1);
                if(chopSprite != NULL && !(chopSprite->isVisible())){
                    chopSprite->setVisible(true);
                }
                
                auto userChopDisableBg = cell->getChildByTag(USER_CHOP_DISABLE_BG_TAG + i + 1);
                if((strcmp(selfUserId.c_str(), userId.c_str()) == 0) || (playerSelf == NULL) || ((playerSelf != NULL) && (playerSelf->getSeatNum() == 0))){
                    if(userChopDisableBg != NULL && !(userChopDisableBg->isVisible())){
                        userChopDisableBg->setVisible(true);
                    }
                }
            }
        }
    }
    else{
        auto photoSprite = (Sprite *)(this->getChildByTag(PHOTO_SPRITE_TAG));
        if (photoSprite != NULL && photoSprite->isVisible()) {
            photoSprite->setVisible(false);
        }
        
        auto userAccountLabel = (Label *)(this->getChildByTag(USER_ACCOUNT_LABEL_TAG));
        if(userAccountLabel != NULL && userAccountLabel->isVisible()){
            userAccountLabel->setVisible(false);
        }
        
        auto userIdLabel = (Label *)(this->getChildByTag(USER_ID_LABEL_TAG));
        if(userIdLabel != NULL && userIdLabel->isVisible()){
            userIdLabel->setVisible(false);
        }
        
        auto userChipAccountLabel = (Label *)(this->getChildByTag(USER_CHIP_ACCOUNT_LABEL_TAG));
        if(userChipAccountLabel != NULL && userChipAccountLabel->isVisible()){
            userChipAccountLabel->setVisible(false);
        }
        
        auto userDzpkLevelLabel = (Label *)(this->getChildByTag(USER_DZPK_LEVEL_LABEL_TAG));
        if(userDzpkLevelLabel != NULL && userDzpkLevelLabel->isVisible()){
            userDzpkLevelLabel->setVisible(false);
        }
        
        auto userChopBg = (Label *)(this->getChildByTag(USER_CHOP_BG_TAG));
        if(userChopBg != NULL && userChopBg->isVisible()){
            userChopBg->setVisible(false);
        }
        
        if(tableView != NULL && !(tableView->isVisible())){
            tableView->setVisible(true);
        }
        
        if(tableView != NULL && tableView->isVisible()){
            tableView->setVisible(false);
            
            auto cellCount = tableView->getContainer()->getChildrenCount();
            log("cellCount: %ld", cellCount);
            for(int i = 0; i < cellCount; i++){
                auto cell = (TableViewCell *)(tableView->getContainer()->getChildByTag(i));
                
                auto chopSprite = cell->getChildByTag(i + 1);
                if(chopSprite != NULL && chopSprite->isVisible()){
                    chopSprite->setVisible(false);
                }
                
                auto userChopDisableBg = cell->getChildByTag(USER_CHOP_DISABLE_BG_TAG + i + 1);
                if(userChopDisableBg != NULL && !(userChopDisableBg->isVisible())){
                    userChopDisableBg->setVisible(false);
                }
            }
        }
    }
}

void RoomUserInfoLayer::showAndHideCardInfo(bool show){
    if(show == true){
        auto userTotalTimeLabel = (Label *)(this->getChildByTag(USER_TOTAL_TIME_LABEL_TAG));
        if(userTotalTimeLabel != NULL && !(userTotalTimeLabel->isVisible())){
            userTotalTimeLabel->setVisible(true);
        }
        
        auto userWinAndFailTimeLabel = (Label *)(this->getChildByTag(USER_WIN_AND_FAIL_TIME_LABEL_TAG));
        if(userWinAndFailTimeLabel != NULL && !(userWinAndFailTimeLabel->isVisible())){
            userWinAndFailTimeLabel->setVisible(true);
        }
        
        auto userWinPerLabel = (Label *)(this->getChildByTag(USER_WIN_PER_LABEL_TAG));
        if(userWinPerLabel != NULL && !(userWinPerLabel->isVisible())){
            userWinPerLabel->setVisible(true);
        }
        
        auto biggestWinChipLabel = (Label *)(this->getChildByTag(BIGGEST_WIN_CHIP_LABEL_TAG));
        if(biggestWinChipLabel != NULL && !(biggestWinChipLabel->isVisible())){
            biggestWinChipLabel->setVisible(true);
        }
        
        auto biggestHandCardLabel = (Label *)(this->getChildByTag(BIGGEST_HAND_CARD_LABEL_TAG));
        if(biggestHandCardLabel != NULL && !(biggestHandCardLabel->isVisible())){
            biggestHandCardLabel->setVisible(true);
        }
        
        for (int i = 0; i < 5; i++) {
            auto handCard = (Sprite *)(this->getChildByTag(USER_CARD_INFO_HAND_CARD_TAG + i));
            if (handCard != NULL && !(handCard->isVisible())) {
                handCard->setVisible(true);
            }
        }
    }
    else{
        auto userTotalTimeLabel = (Label *)(this->getChildByTag(USER_TOTAL_TIME_LABEL_TAG));
        if(userTotalTimeLabel != NULL && userTotalTimeLabel->isVisible()){
            userTotalTimeLabel->setVisible(false);
        }
        
        auto userWinAndFailTimeLabel = (Label *)(this->getChildByTag(USER_WIN_AND_FAIL_TIME_LABEL_TAG));
        if(userWinAndFailTimeLabel != NULL && userWinAndFailTimeLabel->isVisible()){
            userWinAndFailTimeLabel->setVisible(false);
        }
        
        auto userWinPerLabel = (Label *)(this->getChildByTag(USER_WIN_PER_LABEL_TAG));
        if(userWinPerLabel != NULL && userWinPerLabel->isVisible()){
            userWinPerLabel->setVisible(false);
        }
        
        auto biggestWinChipLabel = (Label *)(this->getChildByTag(BIGGEST_WIN_CHIP_LABEL_TAG));
        if(biggestWinChipLabel != NULL && biggestWinChipLabel->isVisible()){
            biggestWinChipLabel->setVisible(false);
        }
        
        auto biggestHandCardLabel = (Label *)(this->getChildByTag(BIGGEST_HAND_CARD_LABEL_TAG));
        if(biggestHandCardLabel != NULL && biggestHandCardLabel->isVisible()){
            biggestHandCardLabel->setVisible(false);
        }
        
        for (int i = 0; i < 5; i++) {
            auto handCard = (Sprite *)(this->getChildByTag(USER_CARD_INFO_HAND_CARD_TAG + i));
            if (handCard != NULL && handCard->isVisible()) {
                handCard->setVisible(false);
            }
        }
    }
}

Size RoomUserInfoLayer::cellSizeForTable(TableView *table){
    return Size(89.83, 94);
}

TableViewCell* RoomUserInfoLayer::tableCellAtIndex(TableView *table, ssize_t idx){
    auto cell = TableViewCell::create();
    cell->setContentSize(Size(89.83, 94));
    cell->setTag((int)idx);
    
    auto chopPngName = __String::createWithFormat("user_chop_%ld.png", (idx + 1));
    auto chopSprite = Sprite::create(chopPngName->getCString());
    chopSprite->setTag((int)(idx + 1));
    chopSprite->setPosition(Vec2(44.915, 40));
    
    cell->addChild(chopSprite);
    
    auto userChopDisableBg = Sprite::create("user_chop_disable_bg.png");
    userChopDisableBg->setTag((int)(USER_CHOP_DISABLE_BG_TAG + idx + 1));
    userChopDisableBg->setPosition(Vec2(44.915, 40));
    cell->addChild(userChopDisableBg, 1);
    auto selfUserId = UserInfo::getInstance()->getUserId();
    auto playerSelf = RoomData::getInstance()->getPlayer((char *)selfUserId.c_str());
    auto userId = this->userInfo->getUserId();
    if((strcmp(selfUserId.c_str(), userId.c_str()) == 0) || (playerSelf == NULL) || ((playerSelf != NULL) && (playerSelf->getSeatNum() == 0))){
        userChopDisableBg->setVisible(true);
    }
    else{
        userChopDisableBg->setVisible(false);
    }
    
    return cell;
}

ssize_t RoomUserInfoLayer::numberOfCellsInTableView(TableView *table){
    return 7;
}

void RoomUserInfoLayer::tableCellTouched(TableView* table, TableViewCell* cell){
    log("tableCellTouched");
    cellTouchedIndex = cell->getTag();
    touchedCell = cell;
}