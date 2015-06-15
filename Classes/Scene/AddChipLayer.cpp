#include "AddChipLayer.h"
#include "../GlobalDef/Data/RoomData.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "../GlobalDef/GlobalChineseXml.h"

bool AddChipLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	this->seatNum = 0;

	//背景
	Sprite* addChipBackground = Sprite::create("add_chip_bg.png");
	addChipBackground->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(addChipBackground);

	//现有筹码标签
	Label* chipAccountLabel = Label::createWithSystemFont("", "Arial", 33);
	chipAccountLabel->setColor(Color3B(219, 219, 219));
	auto userId = UserInfo::getInstance()->getUserId();
	__String* strChipAccount = __String::createWithFormat("%ld", RoomData::getInstance()->getPlayer((char *)userId.c_str())->getChipAccount());
	chipAccountLabel->setString(strChipAccount->getCString());
	chipAccountLabel->setPosition(visibleSize.width / 2 + 90, visibleSize.height / 2 + 172 - 31);
	this->addChild(chipAccountLabel);

	//最低买入数字标签
	Label* minBuyLabel = Label::createWithSystemFont("", "Arial", 23);
	minBuyLabel->setColor(Color3B(183, 183, 183));
	__String* strMinBuyChip = __String::createWithFormat("%ld", RoomData::getInstance()->getRoomInfo()->getMinBuyChip());
	minBuyLabel->setString(strMinBuyChip->getCString());
	minBuyLabel->setPosition(visibleSize.width / 2 - 170, visibleSize.height / 2 + 97.5 + 20 - 45);
	this->addChild(minBuyLabel);

	//最高买入数字标签
	Label* maxBuyLabel = Label::createWithSystemFont("", "Arial", 23);
	maxBuyLabel->setColor(Color3B(183, 183, 183));
	auto strUserId = UserInfo::getInstance()->getUserId();
	long chipAccount = RoomData::getInstance()->getPlayer((char *)strUserId.c_str())->getChipAccount();
	long maxBuyChip = RoomData::getInstance()->getRoomInfo()->getMaxBuyChip();
	__String* strMaxBuyChip = nullptr;
	if(chipAccount > maxBuyChip){
		strMaxBuyChip = __String::createWithFormat("%ld", maxBuyChip);
	}
	else{
		strMaxBuyChip = __String::createWithFormat("%ld", chipAccount);
	}
	maxBuyLabel->setString(strMaxBuyChip->getCString());
	maxBuyLabel->setPosition(visibleSize.width / 2 + 170, visibleSize.height / 2 + 97.5 + 20 - 45);
	this->addChild(maxBuyLabel);

	//当前买入数字标签
	Label* realBuyLabel = Label::createWithSystemFont("", "Arial", 45);
	realBuyLabel->setTag(REAL_BUY_LABEL_TAG);
	realBuyLabel->setColor(Color3B(213, 156, 45));
	__String* strRealBuyChip = __String::createWithFormat("%ld", 2 * RoomData::getInstance()->getRoomInfo()->getMinBuyChip());
	realBuyLabel->setString(strRealBuyChip->getCString());
	realBuyLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 97.5 - 45);
	this->addChild(realBuyLabel);

	//滑动条
	auto slider = ControlSlider::create("adcp_sr_bg.png", "adcp_sr_progress.png", "adcp_srtb_up.png", "adcp_srtb_down.png");
	slider->setTag(ADD_CHIP_LAYER_SLIDER_TAG);
	slider->setScale(0.87f);
	slider->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 7.5 - 30);
	slider->setMinimumValue(RoomData::getInstance()->getRoomInfo()->getMinBuyChip());	//最小值
	slider->setMaximumValue(RoomData::getInstance()->getRoomInfo()->getMaxBuyChip());	//最大值
	slider->setValue(2 * RoomData::getInstance()->getRoomInfo()->getMinBuyChip());		//将滑动条位置设置到默认加注值
	this->addChild(slider);
	//添加消息响应，滑动时调整数值框的位置
	slider->addTargetWithActionForControlEvents(this, cccontrol_selector(AddChipLayer::valueChanged), Control::EventType::VALUE_CHANGED);

	//打勾菜单，购买坐下菜单
	MenuItemImage* rightMenuItem = MenuItemImage::create("bt_right_menu_up.png", "bt_right_menu_up.png", CC_CALLBACK_1(AddChipLayer::btRightMenuCallback, this));
	rightMenuItem->setPosition(visibleSize.width / 2 - 214.5, visibleSize.height / 2 - 60.5 - 24);

	MenuItemImage* buyAndSitMenuItem = MenuItemImage::create("bt_buy_sit_up.png", "bt_buy_sit_down.png", CC_CALLBACK_1(AddChipLayer::btBuyAndSitMenuCallback, this));
	buyAndSitMenuItem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 141);

	MenuItemImage* menuCloseBox = MenuItemImage::create("close_box_up.png", "close_box_down.png", CC_CALLBACK_1(AddChipLayer::btCloseMenuCallback, this));
	menuCloseBox->setPosition(visibleSize.width / 2 + 245, visibleSize.height / 2 + 157.5);

	Menu* menu = Menu::create(rightMenuItem, buyAndSitMenuItem, menuCloseBox, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);

	return true;
}

//打勾菜单回调方法
void AddChipLayer::btRightMenuCallback(Ref *ref){
	bool isAutoBuy = RoomData::getInstance()->getIsAutoBuy();

	if(isAutoBuy){
		//去掉勾
		this->removeChildByTag(RIGHT_MENU_TAG);

		RoomData::getInstance()->setIsAutoBuy(false);
	}
	else{
		//打勾
		Sprite* rightSprite = Sprite::create("bt_right_menu.png");
		rightSprite->setTag(RIGHT_MENU_TAG);
		rightSprite->setPosition(visibleSize.width / 2 - 214.5, visibleSize.height / 2 - 60.5 - 24);
		this->addChild(rightSprite);

		RoomData::getInstance()->setIsAutoBuy(true);
	}
}

//购买坐下菜单回调方法
void AddChipLayer::btBuyAndSitMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	RoomData::getInstance()->setAutoBuyChip(((ControlSlider *)this->getChildByTag(ADD_CHIP_LAYER_SLIDER_TAG))->getValue());

	this->addChipDelegate->onBuyAndSit(this->seatNum);
}

//关闭菜单回调方法
void AddChipLayer::btCloseMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	RoomData::getInstance()->setAutoBuyChip(((ControlSlider *)this->getChildByTag(ADD_CHIP_LAYER_SLIDER_TAG))->getValue());

	this->addChipDelegate->onCloseAddChipLayer();
}

//滑动条滑动回调方法
void AddChipLayer::valueChanged(Ref* object, Control::EventType type){
	ControlSlider* pSlider = (ControlSlider *)object;
	__String* strValue = __String::createWithFormat("%d", (int)pSlider->getValue());
	((Label *)this->getChildByTag(REAL_BUY_LABEL_TAG))->setString(strValue->getCString());
}

//设置代理
void AddChipLayer::setAddChipDelegate(AddChipDelegate* addChipDelegate){
	this->addChipDelegate = addChipDelegate;
}

//设置坐下对象
void AddChipLayer::setSeatNum(int seatNum){
	this->seatNum = seatNum;
}