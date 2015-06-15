#include "SettingLayer.h"

#include "UpdateLayer.h"
#include "InstructionsLayer.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/ResourcePreload.h"

bool SettingLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();

	// main 背景
	Sprite* bg_setting = Sprite::create("bg_setting.png");
	bg_setting->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg_setting);

	//标题
	auto title = Sprite::create("set_title.png");
	title->setAnchorPoint(Vec2(0.5, 1));
	title->setPosition(visibleSize.width / 2, visibleSize.height + 2);
	this->addChild(title);

	//标题字
	auto title_label = Sprite::create("set_set_label.png");
	title_label->setAnchorPoint(Vec2(0.5, 0.5));
	title_label->setPosition(visibleSize.width / 2, visibleSize.height - 40);
	this->addChild(title_label);

	//tableview
	TableView* settingTable = nullptr;
	if(deltaScreenY > 0){
		settingTable = TableView::create(this, Size(960, 470));
		settingTable->setAnchorPoint(Vec2(0, 0));
		settingTable->setPosition(0, 0 - deltaScreenY * 2 + 94);
	}
	else{
		settingTable = TableView::create(this, Size(960, 564));
		settingTable->setAnchorPoint(Vec2(0, 0));
		settingTable->setPosition(0, 0);
	}
	settingTable->setDirection(ScrollView::Direction::VERTICAL);
	settingTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	settingTable->setDelegate(this);
	this->addChild(settingTable);

	// 返回menu
	MenuItemImage *btn_go_back_menuitem = MenuItemImage::create("btn_go_back_up.png", "btn_go_back_down.png", CC_CALLBACK_1(SettingLayer::btGoBackMenuCallback, this));
	btn_go_back_menuitem->setAnchorPoint(Vec2(0, 1));
	btn_go_back_menuitem->setPosition(30, visibleSize.height - 9);

	auto btn_menu = Menu::create(btn_go_back_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	this->addChild(btn_menu);

	return true;
}


void SettingLayer::btGoBackMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}

Size SettingLayer::cellSizeForTable(TableView *table){
	return Size(960, 94);
}

TableViewCell* SettingLayer::tableCellAtIndex(TableView *table, ssize_t idx){
	auto tableViewCell = TableViewCell::create();
	auto volumnBackground = Sprite::create("set_bg.png");
	volumnBackground->setPosition(visibleSize.width / 2, 46);
	tableViewCell->addChild(volumnBackground);

	if(idx == 0){						//第一栏，音效调节
		auto volumnLabel = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("volume_change"), "Arial", 33);
		volumnLabel->setColor(Color3B(0, 0, 0));
		volumnLabel->setAnchorPoint(Vec2(0, 0.5f));
		volumnLabel->setPosition(50, 46);
		tableViewCell->addChild(volumnLabel);

		auto minVolumnSprite = Sprite::create("set_min_volume.png");
		minVolumnSprite->setPosition(visibleSize.width - 230 - 162, 46);
		tableViewCell->addChild(minVolumnSprite);

		auto maxVolumnSprite = Sprite::create("set_max_volume.png");
		maxVolumnSprite->setPosition(visibleSize.width - 230 + 162, 46);
		tableViewCell->addChild(maxVolumnSprite);

		//滑动条
		auto volumnSlider = ControlSlider::create("set_volumn_slider_bg.png", "set_volumn_slider_front.png", "set_volumn_thumb.png", "set_volumn_thumb.png");
//		slider->setScale(0.8f);
		volumnSlider->setPosition(visibleSize.width - 230, 46);
		volumnSlider->setMinimumValue(0);										//最小值
		volumnSlider->setMaximumValue(1);										//最大值
		volumnSlider->setValue(GlobalDef::getInstance()->getVolumnValue());		//将滑动条位置设置到默认加注值
		tableViewCell->addChild(volumnSlider);
		//添加消息响应，滑动时调整数值框的位置
		volumnSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingLayer::volumnValueChanged), Control::EventType::VALUE_CHANGED);
	}
	else if(idx == 1){					//第二栏，自动入座
		auto autoSeatLabel = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("auto_sit"), "Arial", 33);
		autoSeatLabel->setColor(Color3B(0, 0, 0));
		autoSeatLabel->setAnchorPoint(Vec2(0, 0.5f));
		autoSeatLabel->setPosition(50, 46);
		tableViewCell->addChild(autoSeatLabel);

		auto switchBg = Sprite::create("set_switch_bg.png");
		auto switchOn = Sprite::create("set_switch_on.png");
		auto switchOff = Sprite::create("set_switch_off.png");
		auto switchThumb = Sprite::create("set_switch_thumb.png");
		auto labelOn = LabelTTF::create("", "Arial", 27);
		auto labelOff = LabelTTF::create("", "Arial", 27);
		auto autoSeatControlSwith = ControlSwitch::create(switchBg, switchOn, switchOff, switchThumb, (Label *)labelOn, (Label *)labelOff);
		autoSeatControlSwith->setPosition(visibleSize.width - 90, 46);
		autoSeatControlSwith->setOn(GlobalDef::getInstance()->getIsAutoSeatOn());
		tableViewCell->addChild(autoSeatControlSwith);
		autoSeatControlSwith->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingLayer::autoSeatValueChanged), Control::EventType::VALUE_CHANGED);
	}
	else if(idx == 2){					//第三栏，震动提示
		auto vibrateLabel = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("vibrate"), "Arial", 33);
		vibrateLabel->setColor(Color3B(0, 0, 0));
		vibrateLabel->setAnchorPoint(Vec2(0, 0.5f));
		vibrateLabel->setPosition(50, 46);
		tableViewCell->addChild(vibrateLabel);

		auto switchBg = Sprite::create("set_switch_bg.png");
		auto switchOn = Sprite::create("set_switch_on.png");
		auto switchOff = Sprite::create("set_switch_off.png");
		auto switchThumb = Sprite::create("set_switch_thumb.png");
		auto labelOn = LabelTTF::create("", "Arial", 27);
		auto labelOff = LabelTTF::create("", "Arial", 27);
		auto vibrateControlSwith = ControlSwitch::create(switchBg, switchOn, switchOff, switchThumb, (Label *)labelOn, (Label *)labelOff);
		vibrateControlSwith->setPosition(visibleSize.width - 90, 46);
		vibrateControlSwith->setOn(GlobalDef::getInstance()->getIsVibrateOn());
		tableViewCell->addChild(vibrateControlSwith);
		vibrateControlSwith->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingLayer::vibrateValueChanged), Control::EventType::VALUE_CHANGED);
	}
	else if(idx == 3){					//第四栏，检查版本
		auto vibrateLabel = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("exam_version"), "Arial", 33);
		vibrateLabel->setColor(Color3B(0, 0, 0));
		vibrateLabel->setAnchorPoint(Vec2(0, 0.5f));
		vibrateLabel->setPosition(50, 46);
		tableViewCell->addChild(vibrateLabel);

		auto examVersionMenuItem = MenuItemImage::create("set_btn_up.png", "set_btn_down.png", CC_CALLBACK_1(SettingLayer::btnExamVersionMenuCallback, this));
		examVersionMenuItem->setPosition(visibleSize.width - 90, 46);

		auto menu = Menu::create(examVersionMenuItem, NULL);
		menu->setPosition(0, 0);
		tableViewCell->addChild(menu);
	}
	else if(idx == 4){					//第五栏，联系客服
		auto connectCustomerLabel = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("connect_customer"), "Arial", 33);
		connectCustomerLabel->setColor(Color3B(0, 0, 0));
		connectCustomerLabel->setAnchorPoint(Vec2(0, 0.5f));
		connectCustomerLabel->setPosition(50, 46);
		tableViewCell->addChild(connectCustomerLabel);

		auto connectCustomerMenuItem = MenuItemImage::create("set_btn_up.png", "set_btn_down.png", CC_CALLBACK_1(SettingLayer::btnConnectCustomerMenuCallback, this));
		connectCustomerMenuItem->setPosition(visibleSize.width - 90, 46);

		auto menu = Menu::create(connectCustomerMenuItem, NULL);
		menu->setPosition(0, 0);
		tableViewCell->addChild(menu);
	}
	else if(idx == 5){					//第六栏，关于
		auto aboutLabel = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("about"), "Arial", 33);
		aboutLabel->setColor(Color3B(0, 0, 0));
		aboutLabel->setAnchorPoint(Vec2(0, 0.5f));
		aboutLabel->setPosition(50, 46);
		tableViewCell->addChild(aboutLabel);

		auto aboutMenuItem = MenuItemImage::create("set_btn_up.png", "set_btn_down.png", CC_CALLBACK_1(SettingLayer::btnAboutMenuCallback, this));
		aboutMenuItem->setPosition(visibleSize.width - 90, 46);

		auto menu = Menu::create(aboutMenuItem, NULL);
		menu->setPosition(0, 0);
		tableViewCell->addChild(menu);
	}

	return tableViewCell;
}

ssize_t SettingLayer::numberOfCellsInTableView(TableView *table){
	return 6;
}

void SettingLayer::tableCellTouched(TableView* table, TableViewCell* cell){

}

//检查版本回调方法
void SettingLayer::btnExamVersionMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	/*;*/

	UpdateLayer* settingLayer = UpdateLayer::create();
	this->addChild(settingLayer);
}  

//联系客服回调方法
void SettingLayer::btnConnectCustomerMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    ContactLayer* settingLayer = ContactLayer::create();
	this->addChild(settingLayer);
}

//关于回调方法
void SettingLayer::btnAboutMenuCallback(Ref *object){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	InstructionsLayer* settingLayer = InstructionsLayer::create();
	this->addChild(settingLayer);
}

//音量变化回调方法
void SettingLayer::volumnValueChanged(Ref* object, Control::EventType type){
	ControlSlider* pSlider = (ControlSlider *)object;
	float volumnValue = pSlider->getValue();
    log("volumnValue: %f", volumnValue);
	GlobalDef::getInstance()->setVolumnValue(volumnValue);
    if(volumnValue < (1e-6)){
        log("Sound Off");
        GlobalDef::getInstance()->setSoundOn(false);
    }
    else{
        log("Sound On");
        GlobalDef::getInstance()->setSoundOn(true);
    }
}

//自动入座变化回调方法
void SettingLayer::autoSeatValueChanged(Ref* object, Control::EventType type){
	ControlSwitch* pAutoSeatSwtich = (ControlSwitch *)object;
	bool isAutoSeatOn = pAutoSeatSwtich->isOn();
	GlobalDef::getInstance()->setIsAutoSeatOn(isAutoSeatOn);
}

//振动提示变化回调方法
void SettingLayer::vibrateValueChanged(Ref* object, Control::EventType type){
	ControlSwitch* pVibrateSwtich = (ControlSwitch *)object;
	bool isVibrateOn = pVibrateSwtich->isOn();
	GlobalDef::getInstance()->setIsVibrateOn(isVibrateOn);
}