#include "RankListLayer.h"
#include "../GlobalDef/GlobalDef.h"

Scene* RankListLayer::createScene(){
	Scene* rankListScene = Scene::create();

	RankListLayer* rankListLayer = RankListLayer::create();
	rankListScene->addChild(rankListLayer);

	return rankListScene;
}

bool RankListLayer::init(){
	if(!Layer::init()){
		return false;
	}

	rankListSlider = nullptr;
	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();

	m_bTable = true;
    m_bSlider = true;

	showRankListType = RankListType::RANK_LIST_WEALTH;

	visibleSize = Director::getInstance()->getVisibleSize();

	m_rect.origin.x = 50;
	m_rect.origin.y = visibleSize.height - 141;
	m_rect.size.width = 206;
	m_rect.size.height = 60;

	if(deltaScreenY > 0){
		tableRect.origin.x = 0;
		tableRect.origin.y = 0 - deltaScreenY * 2 + 83;
		tableRect.size.width = 960;
		tableRect.size.height = 415;
	}
	else{
		tableRect.origin.x = 0;
		tableRect.origin.y = 0;
		tableRect.size.width = 960;
		tableRect.size.height = 498;
	}
	//背景
	Sprite* rankListBackground = Sprite::create("rank_list_layer_bg.png");
	rankListBackground->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(rankListBackground);

	//标题字
	auto title_label = Sprite::create("rank_title_label.png");
	title_label->setAnchorPoint(Vec2(0.5, 0.5));
	title_label->setPosition(visibleSize.width / 2, visibleSize.height - 40);
	this->addChild(title_label);

	//红色条
	auto red_title = Sprite::create("rank_red_title.png");
	red_title->setAnchorPoint(Vec2(0.5, 1));
	red_title->setPosition(visibleSize.width / 2, visibleSize.height - 78);
	this->addChild(red_title);

	//两个排行榜按钮
	Sprite* wealthRankUp = Sprite::create("wealth_rank_up.png");
	wealthRankUp->setTag(WEALTH_RANK_UP_TAG);
	wealthRankUp->setAnchorPoint(Vec2(0, 1));
	wealthRankUp->setPosition(50, visibleSize.height - 81);
	this->addChild(wealthRankUp);

	Sprite* wealthRankDown = Sprite::create("wealth_rank_down.png");
	wealthRankDown->setTag(WEALTH_RANK_DOWN_TAG);
	wealthRankDown->setAnchorPoint(Vec2(0, 1));
	wealthRankDown->setPosition(50, visibleSize.height - 81);
	this->addChild(wealthRankDown);

	Sprite* experienceRankUp = Sprite::create("experience_rank_up.png");
	experienceRankUp->setTag(EXPERIENCE_RANK_UP_TAG);
	experienceRankUp->setAnchorPoint(Vec2(0, 1));
	experienceRankUp->setPosition(153, visibleSize.height - 81);
	this->addChild(experienceRankUp);

	Sprite* experienceRankDown = Sprite::create("experience_rank_down.png");
	experienceRankDown->setTag(EXPERIENCE_RANK_DOWN_TAG);
	experienceRankDown->setAnchorPoint(Vec2(0, 1));
	experienceRankDown->setPosition(153, visibleSize.height - 81);
	experienceRankDown->setVisible(false);
	this->addChild(experienceRankDown);

	// 返回menu
	MenuItemImage *btn_go_back_menuitem = MenuItemImage::create("btn_go_back_up.png", "btn_go_back_down.png", CC_CALLBACK_1(RankListLayer::btGoBackMenuCallback, this));
	btn_go_back_menuitem->setAnchorPoint(Vec2(0, 1));
	btn_go_back_menuitem->setPosition(30, visibleSize.height - 9);

	auto btn_menu = Menu::create(btn_go_back_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	this->addChild(btn_menu);

	auto btnRankListener = EventListenerTouchOneByOne::create();
	btnRankListener->onTouchBegan = CC_CALLBACK_2(RankListLayer::onTouchBegan, this);
	btnRankListener->onTouchMoved = CC_CALLBACK_2(RankListLayer::onTouchMoved, this);
	btnRankListener->onTouchEnded = CC_CALLBACK_2(RankListLayer::onTouchEnded, this);
//	btnRankListener->setSwallowTouches(true);//不向下传递触摸
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(btnRankListener, this);

	// tableview
	if(deltaScreenY > 0){
		tableView = TableView::create(this, Size(960, 415));
		tableView->setAnchorPoint(Vec2(0, 0));
		tableView->setPosition(0, 0 - deltaScreenY * 2 + 83);
	}
	else{
		tableView = TableView::create(this, Size(960, 498));
		tableView->setAnchorPoint(Vec2(0, 0));
		tableView->setPosition(0, 0);
	}

	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tableView->setDelegate(this);
	this->addChild(tableView);

	getRankListData(RankListType::RANK_LIST_WEALTH);

	auto disableSliderListener = EventListenerTouchOneByOne::create();
	disableSliderListener->onTouchBegan = [=](Touch *touch, Event *unused_event){
		auto touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
		if (!tableRect.containsPoint(touchPoint))
			return false;
		return true;
	};
	disableSliderListener->onTouchEnded = [=](Touch *touch, Event *unused_event){
		if(rankListSlider != NULL){
			if(rankListSlider->isVisible()){
				FadeOut* fadeOut = FadeOut::create(1.0f);
				CallFunc* disableFunc = CallFunc::create([=](){
					rankListSlider->setVisible(false);
				});

				rankListSlider->runAction(Sequence::create(fadeOut, disableFunc, NULL));
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(disableSliderListener, this);

	return true;
}

void RankListLayer::btGoBackMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	Scene* mainScene = MainScene::createScene();
	Director::getInstance()->replaceScene(mainScene);
}

bool RankListLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_beginPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (!m_rect.containsPoint(m_beginPoint))
		return false;
	return true;
}

void RankListLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	
}

void RankListLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (m_rect.containsPoint(m_endPoint)
		&& fabs(m_endPoint.x - m_beginPoint.x) < 10.0
		&& fabs(m_endPoint.y - m_beginPoint.y) < 10.0)	// clicked
	{
		int nItem = (int)((m_beginPoint.x - m_rect.origin.x) / (m_rect.size.width / 2));

		if(nItem == 0){
			this->getChildByTag(EXPERIENCE_RANK_DOWN_TAG)->setVisible(false);
			this->getChildByTag(WEALTH_RANK_DOWN_TAG)->setVisible(true);

			showRankListType = RankListType::RANK_LIST_WEALTH;

			//刷新财富排行榜
			getRankListData(RankListType::RANK_LIST_WEALTH);
		}
		else if(nItem == 1){
			this->getChildByTag(WEALTH_RANK_DOWN_TAG)->setVisible(false);
			this->getChildByTag(EXPERIENCE_RANK_DOWN_TAG)->setVisible(true);

			showRankListType = RankListType::RANK_LIST_EXPERIENCE;

			//刷新等级排行榜
			getRankListData(RankListType::RANK_LIST_EXPERIENCE);
		}
	}
}

//获取排行榜数据
void RankListLayer::getRankListData(RankListType rankListType){
	loadLayer = LoadLayer::create();
	this->addChild(loadLayer);

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
	rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("chan", "0", alloc);
	auto sessionId = UserInfo::getInstance()->getSessionId();
	ctx.AddMember("sessionid", sessionId.c_str(), alloc);
	ctx.AddMember("mver", "1", alloc);
	ctx.AddMember("ter", "WEB", alloc);
	srand(time(NULL));
	__String* serial = __String::createWithFormat("%d", rand() % 100000);
	ctx.AddMember("serial", serial->getCString(), alloc);
	auto userId = UserInfo::getInstance()->getUserId();
	ctx.AddMember("user_id", userId.c_str(), alloc);

	rapidjson::Value req(rapidjson::kObjectType);

	req.AddMember("module", 2, alloc);
	req.AddMember("t", "sequence", alloc);
	req.AddMember("type", 1, alloc);
	if(rankListType == RankListType::RANK_LIST_WEALTH){
		req.AddMember("order", 1, alloc);
	}
	else if(rankListType == RankListType::RANK_LIST_EXPERIENCE){
		req.AddMember("order", 2, alloc);
	}
	
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
	request->setResponseCallback(CC_CALLBACK_2(RankListLayer::onHttpRequestCompleted, this));

	// write the post data
	request->setRequestData(encryptData.c_str(), encryptData.size()); 
	request->setTag("POST Binary test");
	HttpClient::getInstance()->send(request);
	request->release();
}

void RankListLayer::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
	rankListDatas.clear();
	rankListDatas = RankListData::parseData(recvData);
	tableView->reloadData();
}

//滚动时更新滑动条值
void RankListLayer::scrollViewDidScroll(ScrollView *view){
	if(rankListSlider != NULL){
		m_bSlider = false;
		if ( m_bTable  )
		{
			rankListSlider->setValue(tableView->getContentOffset().y);
		}
		m_bSlider = true;
	}
}

void RankListLayer::valueChanged(Ref* object, Control::EventType type){
	m_bTable = false;
    if ( m_bSlider )
    {
        tableView->setContentOffset(Vec2(0, rankListSlider->getValue()));
    }
    m_bTable = true;
}

Size RankListLayer::cellSizeForTable(TableView *table){
	return Size(960, 83);
}

TableViewCell* RankListLayer::tableCellAtIndex(TableView *table, ssize_t idx){
	TableViewCell* cell = TableViewCell::create();
	cell->setContentSize(Size(960, 83));
	RankListData* pData = rankListDatas.at(idx);

	//头像
	Sprite* headPhoto = Sprite::create("default_photo88.png");
	headPhoto->setScale(0.75f);
	headPhoto->setPosition(267, 39);
	cell->addChild(headPhoto);

	//单元格背景
	if (idx % 2 == 0)
	{
		Sprite *cell_bg = Sprite::create("rank_cell_white_bg.png");
		cell_bg->setAnchorPoint(Vec2(0, 0));
		cell_bg->setPosition(Vec2(0, 0));   
		cell->addChild(cell_bg);
	}
	else{	
		Sprite *cell_bg = Sprite::create("rank_cell_dark_bg.png");
		cell_bg->setAnchorPoint(Vec2(0, 0));
		cell_bg->setPosition(Vec2(0, 0));   
		cell->addChild(cell_bg);
	}

	//排名
	if(idx == 0){
		Sprite* rankIcon = Sprite::create("rank_icon_1.png");
		rankIcon->setPosition(106, 41.5);
		cell->addChild(rankIcon);
	}
	else if(idx == 1){
		Sprite* rankIcon = Sprite::create("rank_icon_2.png");
		rankIcon->setPosition(106, 41.5);
		cell->addChild(rankIcon);
	}
	else if(idx == 2){
		Sprite* rankIcon = Sprite::create("rank_icon_3.png");
		rankIcon->setPosition(106, 41.5);
		cell->addChild(rankIcon);
	}
	else if(idx >= 3){
		Label* labelIcon = Label::createWithSystemFont(__String::createWithFormat("%ld", idx + 1)->getCString(), "Arial", 33);
		labelIcon->setColor(Color3B(255, 255, 234));
		labelIcon->setPosition(106, 41.5);
		cell->addChild(labelIcon);
	}

	//昵称
	Label* nickNameLabel = Label::createWithSystemFont(pData->getNickName(), "Arial", 27);
	nickNameLabel->setColor(Color3B(255, 255, 234));
	nickNameLabel->setContentSize(Size(240, 86));
	nickNameLabel->setPosition(460, 41.5);
	cell->addChild(nickNameLabel);

	if(showRankListType == RankListType::RANK_LIST_WEALTH){
		//筹码标识
		Sprite* chipSymbol = Sprite::create("chip_symbol.png");
		chipSymbol->setPosition(670, 41.5);
		cell->addChild(chipSymbol);

		Label* chipLabel = Label::createWithSystemFont(__String::createWithFormat("%ld", pData->getChipAccount())->getCString(), "Arial", 27);
		chipLabel->setScale(0.9f);
		chipLabel->setColor(Color3B(255, 255, 234));
		chipLabel->setAnchorPoint(Vec2(0, 0.5));
		chipLabel->setPosition(693, 41.5);
		cell->addChild(chipLabel);
	}
	else if(showRankListType == RankListType::RANK_LIST_EXPERIENCE){
		//等级标识
		Label* levelLabel = Label::createWithSystemFont(__String::createWithFormat("LV : %d", pData->getDzpkLevel())->getCString(), "Arial", 27);
		levelLabel->setScale(0.9f);
		levelLabel->setColor(Color3B(255, 255, 234));
		levelLabel->setAnchorPoint(Vec2(0, 0.5));
		levelLabel->setPosition(670, 41.5);
		cell->addChild(levelLabel);
	}

	return cell;
}

ssize_t RankListLayer::numberOfCellsInTableView(TableView *table){
	return rankListDatas.size();
}

void RankListLayer::tableCellTouched(TableView* table, TableViewCell* cell){
	
}

void RankListLayer::tableCellHighlight(TableView* table, TableViewCell* cell){
	//滑动条
	if(rankListSlider == NULL){
		rankListSlider = ControlSlider::create("rank_list_slider_bg.png", "rank_list_slider_bg.png", "rank_list_slider_thumb.png");
		rankListSlider->setRotation(90);
		if(deltaScreenY > 0){
			rankListSlider->setScaleX(0.6f);
			rankListSlider->setPosition(visibleSize.width - 17, 207);
		}
		else{
			rankListSlider->setScaleX(0.8f);
			rankListSlider->setPosition(visibleSize.width - 17, 249);
		}
		rankListSlider->setMinimumValue(-(30 * 86 - 516));				//最小值
		rankListSlider->setMaximumValue(0);								//最大值
		rankListSlider->setValue(tableView->getContentOffset().y);		//将滑动条位置设置到默认加注值
		rankListSlider->setVisible(false);
		this->addChild(rankListSlider);
		//添加消息响应，滑动时调整数值框的位置
		rankListSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(RankListLayer::valueChanged), Control::EventType::VALUE_CHANGED);
	}

	if(!rankListSlider->isVisible()){
			rankListSlider->setVisible(true);
	}
}