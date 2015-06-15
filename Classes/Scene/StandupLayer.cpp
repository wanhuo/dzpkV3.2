#include "StandupLayer.h"

bool StandupLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* standupLayerBackground = Sprite::create("stand_up_layer_bg.png");
	standupLayerBackground->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(standupLayerBackground);

	MenuItemImage* menuStandup = MenuItemImage::create("bt_standup_up.png", "bt_standup_down.png", CC_CALLBACK_1(StandupLayer::btStandupCallback, this));
	menuStandup->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2 - 82);

	MenuItemImage* menuGameContinue = MenuItemImage::create("game_continue_up.png", "game_continue_down.png", CC_CALLBACK_1(StandupLayer::btGameContinueCallback, this));
	menuGameContinue->setPosition(visibleSize.width / 2 + 130, visibleSize.height / 2 - 79);

	MenuItemImage* menuCloseBox = MenuItemImage::create("close_box_up.png", "close_box_down.png", CC_CALLBACK_1(StandupLayer::btGameContinueCallback, this));
	menuCloseBox->setPosition(visibleSize.width / 2 + 224, visibleSize.height / 2 + 122);

	Menu* menu = Menu::create(menuStandup, menuGameContinue, menuCloseBox, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);

	return true;
}

//站起菜单回调方法
void StandupLayer::btStandupCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->standupDelegate->onStandupLayerStandup();
}

//继续游戏、关闭菜单回调方法
void StandupLayer::btGameContinueCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->standupDelegate->onStandupLayerCloseLayer();
}

//设置代理
void StandupLayer::setStandupDelegate(StandupDelegate* standupDelegate){
	this->standupDelegate = standupDelegate;
}