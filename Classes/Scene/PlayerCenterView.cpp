#include "PlayerCenterView.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/ResourcePreload.h"
#include "GlobalDef.h"

#define DEFAULT_HEAD_URL		"http://1251038220.cdn.myqcloud.com/1251038220/"
#define DEFAULT_HEAD_URL1       "http://1251038220.cdn.myqcloud.com/1251038220/headpic/"
//#import "ImagePickerController.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool PlayerCenterView::init(){
	if(!Layer::init()){
		return false;
	}
//    productDatas=ProductData::TestData();
    rankListSlider = nullptr;
    dataPro = nullptr;
    m_bTable = true;
    m_bSlider = true;
    isDown = false;
    oldIndex = 0;
    paper_index = 0;
    select_index = 0;
    curent_head_index = 1;
    tableView = NULL;
    nike_name = UserInfo::getInstance()->getNikeName();
    imagePath = UserInfo::getInstance()->getHeadPhotoSerial();
    sexStr = UserInfo::getInstance()->getSex();
    std::string strUrl = UserInfo::getInstance()->getHeadPhotoSerial();
    strUrl = strUrl.replace(0, strlen(DEFAULT_HEAD_URL1), "");
    strUrl = strUrl.substr(0, 5);
    old_Index = std::atoi(strUrl.c_str());
    cureent_index = old_Index;
    
	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();
    paper_index = GlobalDef::getInstance()->getPaperIndex();
    initView();
   
    playerBagDataReq();
//	MainView();
   
	return true;
}

//初始化界面固定显示的控件
void PlayerCenterView::initView(){
	// ±≥æ∞
    scales =1.0;
    scaleY=1.0;
    if(deltaScreenY > 0.01){
        scales=1.0;
        scaleY=visibleSize.height/640;
        
    }else if (deltaScreenX>0.01){
        scales=visibleSize.width/960;
        scaleY=1.0;
    }
    
    //黑色背景
	bg_Instructions = Sprite::create("notice_bg.png");
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
    bg_Instructions->addChild(bg);
   //点击截取获取区域
    tableRect.origin.x = 0;
    tableRect.origin.y = 0;
    tableRect.size.width = bg_Instructions->getContentSize().width;//*0.96*scales
    tableRect.size.height =bg_Instructions->getContentSize().height;//*scales*0.96;
  
	//标题栏黑色背景
	auto title = Sprite::create("set_title.png");
	title->setAnchorPoint(Vec2(0.5, 1));
	title->setPosition(visibleSize.width / 2, visibleSize.height);
	this->addChild(title);
	
    //返回按钮
	MenuItemImage *btn_go_back_menuitem = MenuItemImage::create("btn_go_back_up.png", "btn_go_back_down.png", CC_CALLBACK_1(PlayerCenterView::btGoBackMenuCallback, this));
	btn_go_back_menuitem->setAnchorPoint(Vec2(0, 1));
	btn_go_back_menuitem->setPosition(30, visibleSize.height - 9);

	auto btn_menu = Menu::create(btn_go_back_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	this->addChild(btn_menu);

    //标题栏
	Label* labelIcon = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_titile"), "Arial", 33);
	labelIcon->setColor(Color3B(255, 255, 234));
	labelIcon->setPosition(title->getPositionX(),title->getPositionY()-title->getContentSize().height*0.5);
	this->addChild(labelIcon);
    
    //选择按钮－道具背包，这里两个按钮合并组成了一组 CheckButton
    MenuItemImage *rememberPassword1 = MenuItemImage::create("notice_btn_f.png","notice_btn_f.png");
    MenuItemImage *rememberPassword2 = MenuItemImage::create("notice_btn_t.png","notice_btn_t.png");
    rememberPasswordItem = MenuItemToggle::createWithTarget(this, menu_selector(PlayerCenterView::clickCheckBtn), rememberPassword1,rememberPassword2, NULL);
    rememberPasswordItem->setPosition(Vec2(visibleSize.width-rememberPassword1->getContentSize().width*0.5-28+deltaScreenX,bg->getPositionY()+bg->getContentSize().height/2+26-num_height));
    rememberPasswordItem->setTag(TAG_MENUITEM2);
    
    Label* SelectTab1 = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_bagtitile"), "Arial", 25);
	SelectTab1->setColor(Color3B(0, 0, 0));
    SelectTab1->setPosition(Vec2(visibleSize.width-rememberPassword1->getContentSize().width*0.5-28+deltaScreenX,bg->getPositionY()+bg->getContentSize().height/2+26-num_height));
    bg_Instructions->addChild(SelectTab1,1);
    
//    Sprite* custommerLab = Sprite::create("notice_btn3.png");
//	custommerLab->setPosition(Vec2(visibleSize.width-rememberPassword1->getContentSize().width*0.5-28+deltaScreenX,bg->getPositionY()+bg->getContentSize().height/2+26-num_height));
//	bg_Instructions->addChild(custommerLab,1);
    
    MenuItemImage *rememberName1 = MenuItemImage::create("notice_btn_f.png","notice_btn_f.png");
    MenuItemImage *rememberName2 = MenuItemImage::create("notice_btn_t.png","notice_btn_t.png");
    rememberNameItem = MenuItemToggle::createWithTarget(this, menu_selector(PlayerCenterView::clickCheckBtn), rememberName1,rememberName2, NULL);
    rememberNameItem->setPosition(Vec2(rememberPasswordItem->getPositionX()-153,bg->getPositionY()+bg->getContentSize().height/2+26-num_height));
    rememberNameItem->setTag(TAG_MENUITEM1);
    
    Label* SelectTab2 = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_infotitile"), "Arial", 25);
	SelectTab2->setColor(Color3B(0, 0, 0));
    SelectTab2->setPosition(rememberPasswordItem->getPositionX()-153,bg->getPositionY()+bg->getContentSize().height/2+26-num_height);
     bg_Instructions->addChild(SelectTab2,1);
    
//    Sprite* systemLab = Sprite::create("notice_btn1.png");
//	systemLab->setPosition(rememberPasswordItem->getPositionX()-153,bg->getPositionY()+bg->getContentSize().height/2+26-num_height);
//	bg_Instructions->addChild(systemLab,1);
    
    Menu *itemToggleMenu = Menu::create(rememberNameItem,rememberPasswordItem, NULL);
	itemToggleMenu->setPosition(0, 0);
	bg_Instructions->addChild(itemToggleMenu);
    if(paper_index==0){
    rememberNameItem->setSelectedIndex(0);
    rememberPasswordItem->setSelectedIndex(1);
    }else{
    rememberNameItem->setSelectedIndex(1);
    rememberPasswordItem->setSelectedIndex(0);
    }
    
    //注册点击控制，防止穿透，并且对滚动条进行操作，滚动条影藏暂时注释；
    auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* t, Event* e){
		auto touchPoint = Director::getInstance()->convertToGL(t->getLocationInView());
        //        star_poitn=touchPoint;
		if (!tableRect.containsPoint(touchPoint))
			return false;
		return true;
	};
    listener->onTouchEnded = [=](Touch *touch, Event *unused_event){
        
        auto touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
        
        
		if(rankListSlider != NULL){
			if(rankListSlider->isVisible()){
				FadeOut* fadeOut = FadeOut::create(1.0f);
				CallFunc* disableFunc = CallFunc::create([=](){
                    //					rankListSlider->setVisible(false);
                    
                    
				});
                
				rankListSlider->runAction(Sequence::create(fadeOut, disableFunc, NULL));
			}
		}
	};
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->setPriority(listener, -128);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void PlayerCenterView::onEnter(){
	Layer::onEnter();
}

//玩家信息页面显示
void PlayerCenterView::PlayerInfoView(){
    infoLayer=Layer::create();
    infoLayer->ignoreAnchorPointForPosition(false);
    infoLayer->setContentSize(bg->getContentSize());
    infoLayer->setPosition(Vec2(bg->getPositionX(),bg->getPositionY()*scaleY));
    bg_Instructions->addChild(infoLayer);
    float pianyi=0;
    if(deltaScreenY>0.01){
        pianyi=30.0;
    }
    
    title = Sprite::create("guide_head_big_2.png");
    title->setAnchorPoint(Vec2(0, 0));
    title->setScale(0.8);
    title->setPosition(100, 200*scaleY);
//   infoLayer->addChild(title);

    
    dataPro=new ProductData();
    GlobalDef::getInstance()->setHeadImageScale(1.5);
    std::string strUrl = UserInfo::getInstance()->getHeadPhotoSerial();
    strUrl = strUrl.replace(0, strlen(DEFAULT_HEAD_URL), "");
    dataPro->setMall_path(strUrl.c_str());
    head = DownHeadImageView::create(dataPro);
    head->setDelegate(this);
    head->setName("AsyncImageLayer");
    head->setAnchorPoint(Vec2(.5, .5));
    head->setviewRect(Rect(0, 0, 96, 96));
    head->setPosition(title->getPosition()+Vec2(title->getContentSize().width*0.5,title->getContentSize().height*0.5));
    infoLayer->addChild(head);
    
    Label* nameLab = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_names"), "Arial", 25);
    nameLab->setColor(Color3B(0, 0, 0));
    nameLab->setAnchorPoint(Vec2(0,0.5));
    nameLab->setPosition(infoLayer->getPosition()+Vec2(0, infoLayer->getContentSize().height*0.5-70*scaleY+pianyi));
    infoLayer->addChild(nameLab);
    
    __String contexty = UserInfo::getInstance()->getNikeName();
    Label* nameContext = Label::createWithSystemFont(contexty.getCString(), "Arial", 25);
    nameContext->setColor(Color3B(0, 0, 0));
    nameContext->setAnchorPoint(Vec2(0,0.5));
    nameContext->setPosition(nameLab->getPosition()+Vec2(nameLab->getContentSize().width+3, 0));
    infoLayer->addChild(nameContext);
    
    Label* idLab = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_info_id"), "Arial", 25);
    idLab->setColor(Color3B(0, 0, 0));
    idLab->setAnchorPoint(Vec2(0,0.5));
    idLab->setPosition(nameLab->getPosition()+Vec2(0, 0-45));
    infoLayer->addChild(idLab);
  
    contexty=UserInfo::getInstance()->getUserId();
    Label* idContext = Label::createWithSystemFont(contexty.getCString(), "Arial", 25);
    idContext->setColor(Color3B(0, 0, 0));
    idContext->setAnchorPoint(Vec2(0,0.5));
    idContext->setPosition(idLab->getPosition()+Vec2(idLab->getContentSize().width+3, 0));
    infoLayer->addChild(idContext);
    
    Label* sexLab = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_info_sex"), "Arial", 25);
    sexLab->setColor(Color3B(0, 0, 0));
    sexLab->setAnchorPoint(Vec2(0,0.5));
    sexLab->setPosition(idLab->getPosition()+Vec2(0, 0-45));
    infoLayer->addChild(sexLab);
    
    contexty=UserInfo::getInstance()->getSex();
    
    //选择按钮－道具背包，这里两个按钮合并组成了一组 CheckButton
    MenuItemImage *checkBoy1 = MenuItemImage::create("checkbox_check_false.png","checkbox_check_false.png.png");
    MenuItemImage *checkBoy2 = MenuItemImage::create("checkbox_check_true.png","checkbox_check_true.png");
    checkBoy = MenuItemToggle::createWithTarget(this, menu_selector(PlayerCenterView::clickCheckSexRadioBtn), checkBoy1,checkBoy2, NULL);
    checkBoy->setAnchorPoint(Vec2(0,0.5));
    checkBoy->setPosition(sexLab->getPosition()+Vec2(sexLab->getContentSize().width+3, 0));
    checkBoy->setTag(TAG_CHECK_BOY);
    
    Label* SelectTab1 = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_sexb"), "Arial", 25);
	SelectTab1->setColor(Color3B(0, 0, 0));
    SelectTab1->setAnchorPoint(Vec2(0,0.5));
    SelectTab1->setPosition(checkBoy->getPosition()+Vec2(checkBoy->getContentSize().width+3, 0));
    infoLayer->addChild(SelectTab1,1);
    
    MenuItemImage *checkGirl1 = MenuItemImage::create("checkbox_check_false.png","checkbox_check_false.png");
    MenuItemImage *checkGirl2= MenuItemImage::create("checkbox_check_true.png","checkbox_check_true.png");
    checkGirl = MenuItemToggle::createWithTarget(this, menu_selector(PlayerCenterView::clickCheckSexRadioBtn), checkGirl1,checkGirl2, NULL);
    checkGirl->setAnchorPoint(Vec2(0,0.5));
    checkGirl->setPosition(SelectTab1->getPosition()+Vec2(SelectTab1->getContentSize().width+16, 0));
    checkGirl->setTag(TAG_CHECK_GIRL);
    
    Label* SelectTab2 = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_sexg"), "Arial", 25);
    SelectTab2->setAnchorPoint(Vec2(0,0.5));
	SelectTab2->setColor(Color3B(0, 0, 0));
    SelectTab2->setPosition(checkGirl->getPosition()+Vec2(checkGirl->getContentSize().width+3, 0));
    infoLayer->addChild(SelectTab2,1);
    
    Menu *checkButtonMenu = Menu::create(checkBoy,checkGirl, NULL);
	checkButtonMenu->setPosition(0, 0);
	infoLayer->addChild(checkButtonMenu);
    
    if((std::strcmp(contexty.getCString(),"male") == 0)||
       (std::strcmp(contexty.getCString(),GlobalChineseXml::getInstance()->getValue("playercenterview_sexb")) == 0)
       ){
        checkBoy->setSelectedIndex(1);
        checkGirl->setSelectedIndex(0);
        cureent_sex=0;
        old_sex=0;
    }else{
        checkBoy->setSelectedIndex(0);
        checkGirl->setSelectedIndex(1);
        cureent_sex=1;
        old_sex=1;
    }
    
    Label* moneyLab = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_gold"), "Arial", 25);
    moneyLab->setColor(Color3B(0, 0, 0));
    moneyLab->setAnchorPoint(Vec2(0,0.5));
    moneyLab->setPosition(sexLab->getPosition()+Vec2(0, 0-45));
    infoLayer->addChild(moneyLab);
    
//    contexty=UserInfo::getInstance()->get;
     auto moneyNum= UserInfo::getInstance()->getChipAccount();
    long chipAccount = atol(moneyNum.c_str());
    __String* strChipAccount;
    if(chipAccount > 1000000){      //¥Û”⁄1000000£¨ ˝÷µœ‘ æ≤ªπª
       strChipAccount=__String::createWithFormat("¥%.2fM", chipAccount * 1.0 / 1000000);
	}
	else{
        strChipAccount = __String::create("");
		strChipAccount->append(UserInfo::getInstance()->getChipAccount());
	}
    Label* moneyContext = Label::createWithSystemFont(strChipAccount->getCString(), "Arial", 25);
    moneyContext->setColor(Color3B(0, 0, 0));
    moneyContext->setAnchorPoint(Vec2(0,0.5));
    moneyContext->setPosition(moneyLab->getPosition()+Vec2(moneyLab->getContentSize().width+3, 0));
    infoLayer->addChild(moneyContext);
    
    Label* leveLab = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_leve"), "Arial", 25);
    leveLab->setColor(Color3B(0, 0, 0));
    leveLab->setAnchorPoint(Vec2(0,0.5));
    leveLab->setPosition(moneyLab->getPosition()+Vec2(0, 0-45));
    infoLayer->addChild(leveLab);
    
    contexty = UserInfo::getInstance()->getDzpkLevel();
    Label* leverContext = Label::createWithSystemFont(contexty.getCString(), "Arial", 25);
    leverContext->setColor(Color3B(0, 0, 0));
    leverContext->setAnchorPoint(Vec2(0,0.5));
    leverContext->setPosition(leveLab->getPosition()+Vec2(leveLab->getContentSize().width+3, 0));
    infoLayer->addChild(leverContext);
    
    Label* successLab = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_winper"), "Arial", 25);
    successLab->setColor(Color3B(0, 0, 0));
    successLab->setAnchorPoint(Vec2(0,0.5));
    successLab->setPosition(leveLab->getPosition()+Vec2(0, 0-45));
    infoLayer->addChild(successLab);
    
    stringstream osStr;
    auto strWinPer = UserInfo::getInstance()->getWinPer();
    int winper = atoi(strWinPer.c_str());
    __String contexty1 = UserInfo::getInstance()->getTotalTime();
    int totalTimes = atoi(contexty1.getCString());
    
    int winTimes = winper;
    int failTime = totalTimes - winTimes;
    float winPercent = winTimes * 100.0 / totalTimes;
    
    auto strWinPercent = __String::createWithFormat("%.2f", winPercent);
    
    osStr<<strWinPercent->getCString()<<"%"<<GlobalChineseXml::getInstance()->getValue("playercenterview_brackets")<<winTimes<<GlobalChineseXml::getInstance()->getValue("playercenterview_success")<<failTime<<GlobalChineseXml::getInstance()->getValue("playercenterview_fail");
    __String xx=osStr.str();
    Label* successContext = Label::createWithSystemFont(xx.getCString(), "Arial", 25);
    successContext->setColor(Color3B(0, 0, 0));
    successContext->setAnchorPoint(Vec2(0,0.5));
    successContext->setPosition(successLab->getPosition()+Vec2(successLab->getContentSize().width+3, 0));
    infoLayer->addChild(successContext);
    
    //返回按钮
	MenuItemImage *updateHeadItem = MenuItemImage::create("update_head_t.png", "update_head_f.png", CC_CALLBACK_1(PlayerCenterView::updateHeadImage, this));
	updateHeadItem->setAnchorPoint(Vec2(0.5, 1));
	updateHeadItem->setPosition(title->getPosition()+Vec2(updateHeadItem->getContentSize().width*0.5, -40*scaleY));
    
    if(deltaScreenY>0.01){
        
        pianyi+=20 ;
    }
    MenuItemImage *keepSettingItem = MenuItemImage::create("setting_btn_t.png", "setting_btn_f.png", CC_CALLBACK_1(PlayerCenterView::keepSettingBtn, this));
    keepSettingItem->setAnchorPoint(Vec2(0, 0.5));
	keepSettingItem->setPosition(successLab->getPosition()+Vec2(160,-120*scaleY+pianyi));
    
    
	auto btn_menu = Menu::create(updateHeadItem,keepSettingItem, NULL);
	btn_menu->setPosition(0, 0);
	infoLayer->addChild(btn_menu);
}

//修改头像的事件
void PlayerCenterView::updateHeadImage(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    curent_head_index++;
    if(curent_head_index>14){
      curent_head_index=1;
    }
    stringstream os;
    os.clear();
    os.str("");
    
    if(head){
        infoLayer->removeChild(head);
    }
    if(!dataPro){
      dataPro=new ProductData();
    }
//    http://1251038220.cdn.myqcloud.com/1251038220/headpic/00011_big.jpg
//    std::string strUrl = UserInfo::getInstance()->getHeadPhotoSerial();
//    strUrl = strUrl.replace(0, strlen(DEFAULT_HEAD_URL1), "");
//    strUrl= strUrl.substr(0, 5);
//
//    int number= std::atoi(strUrl.c_str());
    cureent_index++;
    if(cureent_index>28){
        cureent_index=0;
    }
    if(cureent_index<10){
        os<<"headpic/0000"<<cureent_index<<"_big.jpg";
    }else if(cureent_index<100){
        os<<"headpic/000"<<cureent_index<<"_big.jpg";
    }
    dataPro->setMall_path(os.str());
    os.clear();
    os.str("");
    GlobalDef::getInstance()->setHeadImageScale(1.5);
    head = DownHeadImageView::create(dataPro);
    head->setDelegate(this);
    head->setName("AsyncImageLayer");
    head->setAnchorPoint(Vec2(.5, .5));
    head->setviewRect(Rect(0, 0, 96, 96));
//    head->setPosition(200,200);
    head->setPosition(195,300*scaleY);
    infoLayer->addChild(head);
//    os<<"general_"<<curent_head_index<<".webp";
//    title->setTexture(os.str());
    
//    ImagePickerController *myImagePickerController = [[ImagePickerController alloc]init];
//    [myImagePickerController viewDidLoad];
//    NSLog(@"you have entered the function of PlayerCenterView::imageTest()!");
//    [myImagePickerController release];
}

//保存设置事件，
void PlayerCenterView::keepSettingBtn(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
//    std::string nike_name="";
//    std::string imagePath="";
//     std::string sexStr="";
    
    stringstream os;
    //头像是有修改
    if(old_Index!=cureent_index){
        if(cureent_index<10){
            os<<DEFAULT_HEAD_URL1<<"0000"<<cureent_index<<"_big.jpg";
        }else if(cureent_index<100){
            os<<DEFAULT_HEAD_URL1<<"000"<<cureent_index<<"_big.jpg";
        }
        
        imagePath=os.str();
    }else{
        //头像是没有有修改
        imagePath=UserInfo::getInstance()->getHeadPhotoSerial();
    }
    
    if(old_sex!=cureent_sex){
//      (std::strcmp(contexty.getCString(),"male") == 0)
        if(cureent_sex==0){
            sexStr="male";
        }
        else{
            sexStr="female";
        }
    }
    else{
        sexStr=UserInfo::getInstance()->getSex();
    }

    //
    if((old_Index!=cureent_index)||(old_sex!=cureent_sex)){
        
//      replacePlayerInfoReq(nike_name,sexStr,imagePath);
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();
        
        rapidjson::Value ctx(rapidjson::kObjectType);
//        ctx.AddMember("account", "0", alloc);
        
        auto sessionId = UserInfo::getInstance()->getSessionId();
        ctx.AddMember("sessionid", sessionId.c_str(), alloc);
        
        ctx.AddMember("ter", "WEB", alloc);
        
        ctx.AddMember("chan","ios", alloc);
        
        ctx.AddMember("mver", "1", alloc);
        
        auto userId=UserInfo::getInstance()->getUserId();
        ctx.AddMember("user_id", userId.c_str(), alloc);
       
        srand(time(NULL));
        __String* serial = __String::createWithFormat("%d", rand() % 100000);
        ctx.AddMember("serial", serial->getCString(), alloc);
        
//        ctx.AddMember("msgnum","30", alloc);
        
        rapidjson::Value req(rapidjson::kObjectType);
        req.AddMember("t", "comon_setinfo", alloc);
        auto strNickName = UserInfo::getInstance()->getNikeName();
        req.AddMember("nickname", strNickName.c_str(), alloc);
        req.AddMember("from", "ios", alloc);
        req.AddMember("sex", sexStr.c_str(), alloc);
        req.AddMember("pic", imagePath.c_str(), alloc);
        req.AddMember("user_id", userId.c_str(), alloc);
        
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
        request->setRequestType(HttpRequest::Type::POST);
        request->setResponseCallback(CC_CALLBACK_2(PlayerCenterView::replacePlayerInfoResp, this));
        
        request->setRequestData(encryptData.c_str(), encryptData.size());
        request->setTag("POST Message test");
        HttpClient::getInstance()->send(request);
        request->release();
    }
}

//玩家背包道具的显示
void PlayerCenterView::MainView(){
    if(productDataAll.size()>0){
        if(!tableView){
            tableView = TableView::create(this, Size(Size(960*scales,bg->getContentSize().height*scales*0.96)));
            tableView->setAnchorPoint(Vec2(0, 0));
            tableView->setPosition(Vec2(0, 22));
    
            tableView->setDirection(ScrollView::Direction::VERTICAL);
            tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
            tableView->setDelegate(this);
            this->addChild(tableView);
        }
    }
    else{
        labelNone = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("playercenterview_none"), "Arial", 25);
        labelNone->setColor(Color3B(0, 0, 0));
        labelNone->setAnchorPoint(Vec2(0.5,0.5));
        labelNone->setPosition(bg->getPosition());
        this->addChild(labelNone);
    }
}

// 切换选择性别的按钮
void PlayerCenterView::clickCheckSexRadioBtn(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    int currentIndex1=checkBoy->getSelectedIndex();
    int currentIndex2=checkGirl->getSelectedIndex();
    MenuItemToggle* pItem = (MenuItemToggle*)object;
     if(pItem){
         int Index=pItem->getTag();
         if(Index==TAG_CHECK_BOY&&currentIndex1==0){
             checkBoy->setSelectedIndex(1);
             checkGirl->setSelectedIndex(0);
             cureent_sex=0;
         }
         else if(Index==TAG_CHECK_BOY&&currentIndex1==1){
             checkBoy->setSelectedIndex(1);
             checkGirl->setSelectedIndex(0);
             cureent_sex=0;
             return;
         }
         else if(currentIndex2==TAG_CHECK_GIRL&&currentIndex1==0){
             checkBoy->setSelectedIndex(0);
             checkGirl->setSelectedIndex(1);
             cureent_sex=1;
             return;
         }
         else{
             checkBoy->setSelectedIndex(0);
             checkGirl->setSelectedIndex(1);
             cureent_sex=1;
         }
     }
}

//进行切换个人信息页面和玩家背包页面的操作
void PlayerCenterView::clickCheckBtn(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    int currentIndex1=rememberNameItem->getSelectedIndex();
    int currentIndex2=rememberPasswordItem->getSelectedIndex();
    MenuItemToggle* pItem = (MenuItemToggle*)object;
    int type=0;
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
        else if(Index==TAG_MENUITEM2&&currentIndex1==0){
            type=1;
            rememberNameItem->setSelectedIndex(1);
            rememberPasswordItem->setSelectedIndex(0);
        }else{
            type=1;
            rememberNameItem->setSelectedIndex(1);
            rememberPasswordItem->setSelectedIndex(0);
            return;
        }
    }
    
    //进行数据刷新的操作
    //type=0显示玩家信息界面，反之则显示玩家背包道具，
    //
    if(type==0){
        if(tableView){
            tableView->setVisible(false);
        }
        else if(labelNone){
            labelNone->setVisible(false);
        }
        
        if(infoLayer){
            infoLayer->setVisible(true);
        }
        else{
            PlayerInfoView();
        }
    }
    else{
        if(tableView){
            tableView->setVisible(true);
        }
        else if(labelNone){
            labelNone->setVisible(true);
        }
        else{
            MainView();
        }
        if(infoLayer){
            infoLayer->setVisible(false);
        }
    }
}

//拓展预留方法，获取后台数据的方法
void PlayerCenterView::getData(){
    
    
    
}

//点击选择时候模拟手动插入一条数据到Vector
void PlayerCenterView::clickIsSlelect(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    MenuItemToggle* pItem = (MenuItemToggle*)object;
    int select =pItem->getSelectedIndex();
    int Index=(pItem->getTag()-TAG_TABLE_CELL);
    if(select==0){
        insertData(Index,false);
    }
    else{
        insertData(Index,true);
    }
}

//插入模拟数据的方法，参数1:表示当前选择cell行标示，insertOrdelete：标示现在模拟的是插入操作还是删除操作
//这个方法可能看得比较乱，之前策划案文档可以直接拿数据，后来服务端少了字段，
//后来修改为单次拉取单条数据：及主要是商品的描述
void PlayerCenterView::insertData(int index,bool insertOrdelete){
    select_index=index;

    if(insertOrdelete){
        sendOnceDataReq(productDatas.at(index)->getMall_id());
    }
    else{
        productDatas=productDataAll;
        for(int i=0;i<productDatas.size();i++){
            productDatas.at(i)->setisSelect(0);
        }
        
        tableView->reloadData();
    
    
        if(deltaScreenY<0.1){
            if(index>=4){
                Vec2 starPoint= tableView->getContentOffset();
                float KK=starPoint.y+100*(index-3);
                tableView->setContentOffset(Vec2(0, KK+100));
            }
        }
        else{
            if(index>=3){
                Vec2 starPoint= tableView->getContentOffset();
                float KK=starPoint.y+100*(index-2);
                tableView->setContentOffset(Vec2(0, KK+100));
            }
        }
        
        oldIndex=index;
    }
}

//方法的重写，方法内更新滚动条操作
void PlayerCenterView::scrollViewDidScroll(ScrollView *view){
	if(rankListSlider != NULL){
		m_bSlider = false;
		if ( m_bTable  )
		{
//            float kk=tableView->getContentOffset().y;
			rankListSlider->setValue(tableView->getContentOffset().y);
		}
		m_bSlider = true;
	}
}

void PlayerCenterView::scrollViewDidZoom(ScrollView* view){
    
}

Size PlayerCenterView::cellSizeForTable(TableView *table){
	return Size(960*scales, 100);
}

//返回按钮事件
void PlayerCenterView::btGoBackMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    this->playerCenterViewDelegate->onPlayerCenterViewGoBack();
//    this->removeFromParent();
}

//TableView没一个TableViewCell
TableViewCell* PlayerCenterView::tableCellAtIndex(TableView *table, ssize_t idx){
    
    ProductData* pData = productDatas.at(idx);
	TableViewCell* cell = TableViewCell::create();
	cell->setContentSize(Size(960*scales, 100));
   
    //pData->gettype()==0代表是真实数据，pData->gettype()==1代表是显示插入的模拟数据：道具信息
    if(pData->gettype()==0){
        stringstream os;
        os<<"product_"<<1<<".png";
        Sprite* image = Sprite::create(os.str());
        image->setAnchorPoint(Vec2(0, 0.5));
        image->cocos2d::Node::setPosition(80*scales,50);
        image->setScale(0.6);
        //	cell->addChild(image);
        os.clear();
        os.str("");
  
   
        HandleDownLoadView* doload = HandleDownLoadView::create(pData);
        doload->setDelegate(this);
        doload->setName("AsyncImageLayer");
        doload->setAnchorPoint(Vec2(0, 0.5));
        doload->setviewRect(Rect(0, 0, 54, 54));
        doload->cocos2d::Node::setPosition(80*scales,50);
        cell->addChild(doload);
    
        cocos2d::ui::Text *text=cocos2d::ui::Text::create();
        text->setAnchorPoint(Point(0, 0.5));
        text->setFontSize(23);
        text->setText(pData->getMall_name());
        text->setColor(Color3B(0, 0, 0));
        text->setFontName("Arial");
        text->cocos2d::Node::setPosition(image->getPosition()+Vec2((image->getContentSize().width+30)*scales,0));
        cell->addChild(text);
    
        os<<GlobalChineseXml::getInstance()->getValue("playercenterview_product_num")<<pData->getTimeleft();
        cocos2d::ui::Text *context=cocos2d::ui::Text::create();
        context->setAnchorPoint(Point(0, 0.5));
        context->setFontSize(23);
        context->setText(os.str());
        
        os.clear();
        os.str("");
        context->setColor(Color3B(0, 0, 0.5));
        context->setFontName("Arial");
        context->cocos2d::Node::setPosition(text->getPosition()+Vec2((text->getContentSize().width+450)*scales,0));
        cell->addChild(context);
    
        SFScale9Sprite *h_line = SFScale9Sprite::spriteWithFile("notice_line.png");
        h_line->setContentSize(Size(880*scales, h_line->getContentSize().height));
        h_line->setAnchorPoint(Vec2(0,0));
        h_line->setPosition(Vec2(40,image->getPositionY()-image->getContentSize().width*0.5-7));
        cell->addChild(h_line);
        //    Sprite* h_line = Sprite::create("notice_line.png");
        //    h_line->setAnchorPoint(Point(0, 0));
        //    h_line->setPosition(Vec2(image->getPositionX()-30,image->getPositionY()-image->getContentSize().width*0.5-7));
        //    cell->addChild(h_line);
        
        MenuItemImage *select_f = MenuItemImage::create("product_select_f.png", "product_select_f.png");
        MenuItemImage *select_t = MenuItemImage::create("product_select_t.png", "product_select_t.png");
        MenuItemToggle *isSelect = MenuItemToggle::createWithTarget(this, menu_selector(PlayerCenterView::clickIsSlelect), select_f,select_t, NULL);
        isSelect->setTag(TAG_TABLE_CELL + idx);
        isSelect->cocos2d::Node::setPosition(text->getPosition()+Vec2((text->getContentSize().width+220)*scales,0));
    
        Menu *itemToggleMenu = Menu::create(isSelect,NULL);
        itemToggleMenu->setPosition(0, 0);
        cell->addChild(itemToggleMenu);
        isSelect->setSelectedIndex(pData->getisSelect());
    }
    else{
        SFScale9Sprite *contnetBg = SFScale9Sprite::spriteWithFile("cell_bg_down.png");
        contnetBg->setContentSize(Size(960*scales, 100));
        contnetBg->setAnchorPoint(Vec2(0,0));
        contnetBg->setPosition(Vec2(0,0));
        cell->addChild(contnetBg);
        
        Label* labelIcon = Label::createWithSystemFont(pData->getcontext(), "Arial", 25);
        labelIcon->setColor(Color3B(255, 255, 255));
        labelIcon->setAnchorPoint(Vec2(0.5,0.5));
        labelIcon->setPosition(contnetBg->getPositionX()+contnetBg->getContentSize().width*0.5,contnetBg->getPositionY()+contnetBg->getContentSize().height*0.5);
        cell->addChild(labelIcon);
    }
    
	return cell;
}

ssize_t PlayerCenterView::numberOfCellsInTableView(TableView *table){
    return productDatas.size();
}

void PlayerCenterView::tableCellTouched(TableView* table, TableViewCell* cell){
	
}

void PlayerCenterView::tableCellHighlight(TableView* table, TableViewCell* cell){
	//ª¨∂ØÃı
    //滚动条创建
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
		rankListSlider->setMinimumValue(-(productDatas.size()* 100 - 516));				//◊Ó–°÷µ
		rankListSlider->setMaximumValue(0);								//◊Ó¥Û÷µ
		rankListSlider->setValue(tableView->getContentOffset().y);		//Ω´ª¨∂ØÃıŒª÷√…Ë÷√µΩƒ¨»œº”◊¢÷µ
		rankListSlider->setVisible(false);
		this->addChild(rankListSlider);
		//ÃÌº”œ˚œ¢œÏ”¶£¨ª¨∂Ø ±µ˜’˚ ˝÷µøÚµƒŒª÷√
     rankListSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(PlayerCenterView::valueChanged), Control::EventType::VALUE_CHANGED);
	}
    
	if(!rankListSlider->isVisible()){
        rankListSlider->setVisible(true);
	}
    
}

////滚动条拉动时候更新TableView的操作
void PlayerCenterView::valueChanged(Ref* object, Control::EventType type){
	m_bTable = false;
    if ( m_bSlider )
    {
//        float xx= rankListSlider->getValue();
        tableView->setContentOffset(Vec2(0, rankListSlider->getValue()));
    }
    m_bTable = true;
}

//ªÒ»°≈≈––∞Ò ˝æ›
void PlayerCenterView::playerBagDataReq(){
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
	req.AddMember("mall_type", 2, alloc);
    req.AddMember("other_id", 0, alloc);
	req.AddMember("t", "getuserpackage", alloc);
    
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
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(PlayerCenterView::playerBagDataResp, this));
    
	request->setRequestData(encryptData.c_str(), encryptData.size());
	request->setTag("POST LoginReward test");
	HttpClient::getInstance()->send(request);
	request->release();
}

void PlayerCenterView::playerBagDataResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
	const std::string recvData = gloox::Base64::decode64(encodeRecvData);       //Ω‚√‹∫Ûµƒ ˝æ›
	productDataAll.clear();
	productDataAll = ProductData::parseData(recvData);
    productDatas = productDataAll;
    if(paper_index == 0){
        PlayerInfoView();
    }
    else{
        MainView();
    }
}

void PlayerCenterView::clicked(ProductData* data)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
//	int nIndex = m_mallDatas.getIndex(data);
//	if (nIndex < 0 || nIndex >= m_mallDatas.size())
//	{
//		if(goodsTips)
//			goodsTips->setVisible(false);
//		return;
//	}
//    
//	if (!goodsTips)
//	{
//		goodsTips = Sprite::create("mall_tips_bg.png");
//		Label* tips = Label::createWithSystemFont("", "Arial", 20);
//		tips->setName("tips");
//		tips->setAnchorPoint(Vec2(0, 0));
//        tips->setDimensions(95, 120);
//		tips->setHorizontalAlignment(TextHAlignment::LEFT);
//		tips->setPosition(5, 5);
//		goodsTips->addChild(tips);
//		this->addChild(goodsTips);
//	}
//	goodsTips->setVisible(true);
//	int nRow = nIndex / 4;
//	int nLine = nIndex % 4;
//    
//    float mallFramePosY;
//    if(nRow == 0){
//        mallFramePosY = mall_middle_frame->getPositionY() - 20;
//    }
//    else if(nRow == 1){
//        mallFramePosY = mall_bottom_frame->getPositionY() + 65;
//    }
//    
//    goodsTips->setPosition(visibleSize.width / 2 - 200 + nLine * 195, mallFramePosY + 115);
//	
//	Label* tips = (Label*)goodsTips->getChildByName("tips");
//	if (tips)
//	{
//		tips->setString(data->getmallExplain().c_str());
//	}
}

void PlayerCenterView::clickByDown(ProductData *userinfo)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
}

void PlayerCenterView::replacePlayerInfoResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
	const std::string recvData = gloox::Base64::decode64(encodeRecvData);
    rapidjson::Document docGuestLoginRecv;
	docGuestLoginRecv.Parse<0>(recvData.c_str());
    
    int resultId=atoi(docGuestLoginRecv["ctx"]["err"].GetString());
    std::string str=docGuestLoginRecv["ctx"]["result"].GetString();
    if(resultId==0){
        //        int k=0;
        //        str = GlobalChineseXml::getInstance()->getValue("PlayerCenterView_fail_tips");
        UserInfo::getInstance()->setSex(sexStr);
        UserInfo::getInstance()->setHeadPhotoSerial(imagePath);
        UserInfo::getInstance()->setNikeName(nike_name);
    }else {
        //        str = GlobalChineseXml::getInstance()->getValue("PlayerCenterView_fail_tips");
        //提示消息失败
    }
    Sprite* sp_tips = Sprite::create("mall_buy_result_bg.png");;
    sp_tips->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(sp_tips);
    
    
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

void PlayerCenterView::sendOnceDataReq(int id){
	loadLayer = LoadLayer::create();
	this->addChild(loadLayer);
    id=90;
    stringstream os;
    os<<id<<"";
    __String idStr=os.str();
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
    req.AddMember("mall_id", idStr.getCString(), alloc);
	req.AddMember("mall_type", 3, alloc);
//    req.AddMember("other_id", 5, alloc);
	req.AddMember("t", "getmallinfo", alloc);
    
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
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(PlayerCenterView::sendOnceDataResp, this));
    
	request->setRequestData(encryptData.c_str(), encryptData.size());
	request->setTag("POST LoginReward test");
	HttpClient::getInstance()->send(request);
	request->release();
}

void PlayerCenterView::sendOnceDataResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
    
    Vector<ProductData*> noticeDatas;
	rapidjson::Document docHallRecv;
	docHallRecv.Parse<0>(recvData.c_str());
    rapidjson::Value& respdata = docHallRecv["resp"]["mallinfo"];
	__String contextStr="";
    if (respdata.IsArray())
	{
//        __String contextStr =respdata["mall_explain"].GetString();
        for (int i=0; i< respdata.Size(); i++)
		{
			rapidjson::Value& noticeData = respdata[i];
			contextStr=noticeData["mall_explain"].GetString();
        }
    }
    
    Vector<ProductData*> productDataNew;
    int oldSize=productDatas.size();
    int index=select_index;
    productDatas=productDataAll;
    //在连续两次插入操作：即两续点击两个查看道具信息，并且第二个在第一个后面，改变标示
    if(oldSize>productDatas.size()&&index>oldIndex){
        index=index-1;
    }
    for(int i=0;i<productDatas.size();i++){
        productDatas.at(i)->setisSelect(0);
        productDataNew.pushBack(productDatas.at(i));
        
        if(i==index){
            ProductData* data=productDatas.at(i);
            data->setisSelect(1);
        }
        if(i==index){
            ProductData* data1 = new ProductData();
            data1->settype(1);
            data1->setcontext(contextStr.getCString());
            productDataNew.pushBack(data1);
        }
    }
    productDatas=productDataNew;
    
    tableView->reloadData();
    if(deltaScreenY<0.1){
        if(index>2){
            Vec2 starPoint= tableView->getContentOffset();
            float KK=starPoint.y+100*(index-3);
            tableView->setContentOffset(Vec2(0, KK+100));
        }
    }
    else{
        if(index>1){
            Vec2 starPoint= tableView->getContentOffset();
            float KK=starPoint.y+100*(index-2);
            tableView->setContentOffset(Vec2(0, KK+100));
        }
    }
    
    oldIndex=index;
}

void PlayerCenterView::setPlayerCenterViewDelegate(PlayerCenterViewDelegate* playerCenterViewDelegate){
    this->playerCenterViewDelegate = playerCenterViewDelegate;
}