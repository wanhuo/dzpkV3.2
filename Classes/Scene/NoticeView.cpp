
#include "NoticeView.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/ResourcePreload.h"
USING_NS_CC;
USING_NS_CC_EXT;
bool NoticeView::init(){
	if(!Layer::init()){
		return false;
	}
    rankListSlider = nullptr;
    m_bTable = true;
    m_bSlider = true;
	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();
    type=0;
	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();
    
    initView();
    
	playerBagDataReq();
	return true;
}

void NoticeView::initView(){

	// ±≥æ∞
    scales =1.0;
    if(deltaScreenY > 0.01){
        
        scales=1.0;
        
    }else if (deltaScreenX>0.01){
        
        scales=visibleSize.width/960;
        
    }
    
	Sprite* bg_Instructions = Sprite::create("notice_bg.png");
	bg_Instructions->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg_Instructions);

    float num_height=0;
    //白色圆角背景
    bg = SFScale9Sprite::spriteWithFile("notice_bg_down.png");
    bg->ignoreAnchorPointForPosition(false);
    if(deltaScreenY > 0){
        bg->setContentSize(Size(920,360));
        bg->setPosition(visibleSize.width/2,visibleSize.height/2-25);
	}else if(deltaScreenX > 0){
        bg->setContentSize(Size(920,500));
        bg->setPosition(visibleSize.width/2+deltaScreenX,visibleSize.height/2-75);
        bg->setScale(scales);
        num_height=bg->getContentSize().height*(1.0-scales)/2;
        
    }else{
        bg->setContentSize(Size(920,460));
		bg->setPosition(visibleSize.width/2,visibleSize.height/2-75);
	}
	
   
    tableRect.origin.x = 0;
    tableRect.origin.y = 0;
    tableRect.size.width = bg_Instructions->getContentSize().width;//*0.96*scales;
    tableRect.size.height =bg_Instructions->getContentSize().height;//*scales*0.96;

    
    bg_Instructions->addChild(bg);
	// 标题背景
	auto title = Sprite::create("set_title.png");
	title->setAnchorPoint(Vec2(0.5, 1));
	title->setPosition(visibleSize.width / 2, visibleSize.height);
	this->addChild(title);
	// 返回按钮
	MenuItemImage *btn_go_back_menuitem = MenuItemImage::create("btn_go_back_up.png", "btn_go_back_down.png", CC_CALLBACK_1(NoticeView::btGoBackMenuCallback, this));
	btn_go_back_menuitem->setAnchorPoint(Vec2(0, 1));
	btn_go_back_menuitem->setPosition(30, visibleSize.height - 9);

	auto btn_menu = Menu::create(btn_go_back_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	this->addChild(btn_menu);
    
    // 消息界面标题
	Label* labelIcon = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("noticeView_titile"),"Arial", 33);
	labelIcon->setColor(Color3B(255, 255, 234));
	labelIcon->setPosition(title->getPositionX(),title->getPositionY()-title->getContentSize().height*0.5);
	this->addChild(labelIcon);
    
    // 切换个人消息和系统消息的CheckButton
    MenuItemImage *rememberPassword1 = MenuItemImage::create("notice_btn_f.png","notice_btn_f.png");
    MenuItemImage *rememberPassword2 = MenuItemImage::create("notice_btn_t.png","notice_btn_t.png");
    rememberPasswordItem = MenuItemToggle::createWithTarget(this, menu_selector(NoticeView::clickCheckBtn), rememberPassword1,rememberPassword2, NULL);
    rememberPasswordItem->setPosition(Vec2(visibleSize.width-rememberPassword1->getContentSize().width*0.5-28+deltaScreenX,bg->getPositionY()+bg->getContentSize().height/2+26-num_height));
    rememberPasswordItem->setTag(TAG_MENUITEM2);
    
    //系统消息，可改成文字
    Sprite* custommerLab = Sprite::create("notice_btn3.png");
	custommerLab->setPosition(Vec2(visibleSize.width-rememberPassword1->getContentSize().width*0.5-28+deltaScreenX,bg->getPositionY()+bg->getContentSize().height/2+26-num_height));
	bg_Instructions->addChild(custommerLab,1);
    
    //和上面组成一组按钮
    MenuItemImage *rememberName1 = MenuItemImage::create("notice_btn_f.png","notice_btn_f.png");
    MenuItemImage *rememberName2 = MenuItemImage::create("notice_btn_t.png","notice_btn_t.png");
    rememberNameItem = MenuItemToggle::createWithTarget(this, menu_selector(NoticeView::clickCheckBtn), rememberName1,rememberName2, NULL);
    rememberNameItem->setPosition(Vec2(rememberPasswordItem->getPositionX()-153,bg->getPositionY()+bg->getContentSize().height/2+26-num_height));
    rememberNameItem->setTag(TAG_MENUITEM1);
    
    //个人消息，可改成文字
    Sprite* systemLab = Sprite::create("notice_btn1.png");
	systemLab->setPosition(rememberPasswordItem->getPositionX()-153,bg->getPositionY()+bg->getContentSize().height/2+26-num_height);
	bg_Instructions->addChild(systemLab,1);
    
    Menu *itemToggleMenu = Menu::create(rememberNameItem,rememberPasswordItem, NULL);
	itemToggleMenu->setPosition(0, 0);
	bg_Instructions->addChild(itemToggleMenu);
    rememberNameItem->setSelectedIndex(0);
    rememberPasswordItem->setSelectedIndex(1);
    
    
}
void NoticeView::onEnter(){
    
	Layer::onEnter();
	
    
}
//消息的列表显示，通过控制数据，更新TableView
void NoticeView::MainView(){
    
    
    tableView = TableView::create(this, Size(Size(bg->getContentSize().width*0.96*scales,bg->getContentSize().height*scales*0.96)));
    tableView->setAnchorPoint(Vec2(0, 0));
    tableView->setPosition(Vec2(0, 22));
    
    
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tableView->setDelegate(this);
	this->addChild(tableView);
   
    auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* t, Event* e){
		auto touchPoint = Director::getInstance()->convertToGL(t->getLocationInView());
		if (!tableRect.containsPoint(touchPoint))
			return false;
		return true;
	};
    listener->onTouchEnded = [=](Touch *touch, Event *unused_event){
        
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
	listener->setSwallowTouches(true);
    
	Director::getInstance()->getEventDispatcher()->setPriority(listener, -128);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


}
//更新数据的操作,这里需要注意的是，后台消息分类分了很多类，前段只需要两种类型
//type=1的时候是系统消息，其他的则都归类到type＝0
void  NoticeView::updateData(int type){
    
    noticeDatas.clear();
    deleteDatas.clear();
    int type_=0;
    for(int i=0;i<noticeDatasAll.size();i++){
        type_=noticeDatasAll.at(i)->gettype();
        //type=1的时候是系统消息，其他的则都归类到type＝0
        if(type_!=1){
            type_=0;
        }
        if(type_==type){
            noticeDatas.pushBack(noticeDatasAll.at(i));
        }
    }
    //这里操作是进行多余数据分类删除
    //每个种类的消息最多显示30条
    Vector<NoticeData*> noticeDataIndex;
    noticeDataIndex.clear();
    if(noticeDatas.size()>30){
        for(int i=0;i<noticeDatas.size();i++){
            if(i>=30){
                deleteDatas.pushBack(noticeDatasAll.at(i));
            }else{
                noticeDataIndex.pushBack(noticeDatasAll.at(i));
            }
        }
    }
    if(noticeDataIndex.size()==30){
       noticeDatas=noticeDataIndex;
    }
    deleteMessageDataReq();
    
}

//切换按钮的事件，进行系统消息和个人消息界面的切换
//模拟单选按钮组
void NoticeView::clickCheckBtn(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    int currentIndex1=rememberNameItem->getSelectedIndex();
    int currentIndex2=rememberPasswordItem->getSelectedIndex();
    MenuItemToggle* pItem = (MenuItemToggle*)object;
  
    if(pItem){
        int Index=pItem->getTag();
        if(Index==TAG_MENUITEM1&&currentIndex1==0){
            rememberNameItem->setSelectedIndex(0);
            rememberPasswordItem->setSelectedIndex(1);
            type=0;

            
        }else if(Index==TAG_MENUITEM1&&currentIndex1==1){
            type=0;
            rememberNameItem->setSelectedIndex(0);
            rememberPasswordItem->setSelectedIndex(1);
              return;
        }
        
        else if(currentIndex2==TAG_MENUITEM2&&currentIndex1==0){
            type=1;
            rememberNameItem->setSelectedIndex(1);
            rememberPasswordItem->setSelectedIndex(0);
            return;
        }else{
            type=1;
            rememberPasswordItem->setSelectedIndex(0);
            rememberNameItem->setSelectedIndex(1);
        }
    }
    
    //进行数据刷新的操作
    if(type==0){
        updateData(type);
        if(tableView)
        tableView->reloadData();
        
    }else{
        
       updateData(type);
        if(tableView)
       tableView->reloadData();
    }
    
    
    
}


void NoticeView::scrollViewDidScroll(ScrollView *view){
	if(rankListSlider != NULL){
		m_bSlider = false;
		if ( m_bTable  )
		{
			rankListSlider->setValue(tableView->getContentOffset().y);
		}
		m_bSlider = true;
	}
}
Size NoticeView::cellSizeForTable(TableView *table){
	return Size(960, 90);
}


//πÿ±’∑Ω∑®
void NoticeView::btGoBackMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}
//TableView每一行的内容布局
TableViewCell* NoticeView::tableCellAtIndex(TableView *table, ssize_t idx){
    
    NoticeData *pData = noticeDatas.at(idx);
    
	TableViewCell* cell = TableViewCell::create();
	cell->setContentSize(Size(940, 90));
	
    cocos2d::ui::Text *text=cocos2d::ui::Text::create();
    text->setAnchorPoint(Point(0, 0));
    text->setFontSize(23);
    text->setText(pData->gettime_str());
    text->setColor(Color3B(0, 0, 0));
    text->setFontName("Arial");
    text->cocos2d::Node::setPosition(50,60);
    cell->addChild(text);
    
    cocos2d::ui::Text *context=cocos2d::ui::Text::create();
    context->setAnchorPoint(Point(0, 0));
    context->setFontSize(23);
    context->setText(pData->getcontext());
    context->setColor(Color3B(0, 0, 0));
    context->setFontName("Arial");
    context->cocos2d::Node::setPosition(Vec2(text->getPositionX(),text->getPositionY()-35));
    cell->addChild(context);
    
    Sprite* h_line = Sprite::create("notice_line.png");
    h_line->setAnchorPoint(Point(0, 0));
    h_line->setPosition(Vec2(context->getPositionX()-10,context->getPositionY()-15));
    cell->addChild(h_line);
    
	return cell;
}

ssize_t NoticeView::numberOfCellsInTableView(TableView *table){
	return noticeDatas.size();
}

void NoticeView::tableCellTouched(TableView* table, TableViewCell* cell){
	
}
//滚动条绑定TableView的操作
void NoticeView::tableCellHighlight(TableView* table, TableViewCell* cell){
	//ª¨∂ØÃı
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
		rankListSlider->setMinimumValue(-(noticeDatas.size()* 90 - 516));				//◊Ó–°÷µ
		rankListSlider->setMaximumValue(0);								//◊Ó¥Û÷µ
		rankListSlider->setValue(tableView->getContentOffset().y);		//Ω´ª¨∂ØÃıŒª÷√…Ë÷√µΩƒ¨»œº”◊¢÷µ
		rankListSlider->setVisible(false);
		this->addChild(rankListSlider);
		//ÃÌº”œ˚œ¢œÏ”¶£¨ª¨∂Ø ±µ˜’˚ ˝÷µøÚµƒŒª÷√
    rankListSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(NoticeView::valueChanged), Control::EventType::VALUE_CHANGED);
	}
    
	if(!rankListSlider->isVisible()){
        rankListSlider->setVisible(true);
	}
    
}
//滚动条相互绑定TableView的操作
void NoticeView::valueChanged(Ref* object, Control::EventType type){
	m_bTable = false;
    if ( m_bSlider )
    {
        tableView->setContentOffset(Vec2(0, rankListSlider->getValue()));
    }
    m_bTable = true;
}

//请求玩家个人背包全部信息
void NoticeView::playerBagDataReq(){
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
    ctx.AddMember("msgnum","30", alloc);
    
	rapidjson::Value req(rapidjson::kObjectType);
	req.AddMember("t", "message", alloc);
    
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
	request->setResponseCallback(CC_CALLBACK_2(NoticeView::playerBagDataResp, this));
    
	request->setRequestData(encryptData.c_str(), encryptData.size());
	request->setTag("POST Message test");
	HttpClient::getInstance()->send(request);
	request->release();
}
//请求玩家个人背包全部信息返回数据
void NoticeView::playerBagDataResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
	std::string encodeRecvData; //Ω” ’µΩµƒbase64º”√‹ ˝æ›
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		printf("%c", (*buffer)[i]);
		encodeRecvData.push_back((*buffer)[i]);
	}
	const std::string recvData = gloox::Base64::decode64(encodeRecvData); //Ω‚√‹∫Ûµƒ ˝æ›
	noticeDatasAll.clear();//noticeDatas.size()
	noticeDatasAll= NoticeData::parseData(recvData);
    if(noticeDatasAll.size()>0){
        updateData(type);
        MainView();
    }
  
	
}
//删除多余消息的操作
void NoticeView::deleteMessageDataReq(){
    if(deleteDatas.size()<=0){
        return;
    }
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
	
    auto userAccount=UserInfo::getInstance()->getUserAccount();
    rapidjson::Value ctx(rapidjson::kObjectType);
	ctx.AddMember("account", userAccount.c_str(), alloc);
    
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
    
//    rapidjson::Value arrays(rapidjson::kArrayType);
    rapidjson::Value arrays1(rapidjson::kArrayType);
    for(int i=0;i<deleteDatas.size();i++){
        arrays1.PushBack(deleteDatas.at(i)->getm_Id(), alloc);
    }
    rapidjson::Value arrays2(rapidjson::kArrayType);
    rapidjson::Value arrays3(rapidjson::kArrayType);
    rapidjson::Value message_type1(rapidjson::kObjectType);
    rapidjson::Value message_type2(rapidjson::kObjectType);
    rapidjson::Value message_type3(rapidjson::kObjectType);
    if(deleteDatas.at(0)->gettype()==1){
        
        message_type1.AddMember("message_id",arrays1, alloc);
        message_type1.AddMember("type","1", alloc);
    
        message_type2.AddMember("message_id",arrays2, alloc);
        message_type2.AddMember("type","2", alloc);
        
        message_type3.AddMember("message_id",arrays3, alloc);
        message_type3.AddMember("type","3", alloc);
        
    }else if(deleteDatas.at(0)->gettype()==2){
   
        message_type1.AddMember("message_id",arrays2, alloc);
        message_type1.AddMember("type","1", alloc);

        message_type2.AddMember("message_id",arrays1, alloc);
        message_type2.AddMember("type","2", alloc);
        
        message_type3.AddMember("message_id",arrays3, alloc);
        message_type3.AddMember("type","3", alloc);
        
        
    }else{
        message_type1.AddMember("message_id",arrays2, alloc);
        message_type1.AddMember("type","1", alloc);
        
        message_type2.AddMember("message_id",arrays2, alloc);
        message_type2.AddMember("type","2", alloc);
        
        message_type3.AddMember("message_id",arrays1, alloc);
        message_type3.AddMember("type","3", alloc);
    }
     rapidjson::Value messagetype(rapidjson::kArrayType);
    messagetype.PushBack(message_type1,alloc).PushBack(message_type2,alloc).PushBack(message_type3,alloc);
	rapidjson::Value req(rapidjson::kObjectType);
	req.AddMember("t", "message_delete", alloc);
    req.AddMember("message_type", messagetype, alloc);
    
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
//	request->setUrl("http://203.195.138.89/hallroom/jsonServlet");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(NoticeView::deleteMessageDataResp, this));
    
	request->setRequestData(encryptData.c_str(), encryptData.size());
	request->setTag("POST Message test");
	HttpClient::getInstance()->send(request);
	request->release();
}
//删除多余返回，暂时无用
void NoticeView::deleteMessageDataResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
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
	const std::string recvData =gloox::Base64::decode64(encodeRecvData); //Ω‚√‹∫Ûµƒ ˝æ›
	
	
}