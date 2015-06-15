#include "ExitGameLayer.h"

bool ExitGameLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* backGroundSprite = Sprite::create("exit_game_bg.png");
	backGroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(backGroundSprite);

	MenuItemImage* menuGoHall = MenuItemImage::create("login_out_ok_up.png", "login_out_ok_down.png", CC_CALLBACK_1(ExitGameLayer::btGoHallCallback, this));
	menuGoHall->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2 - 82);

	MenuItemImage* menuGameContinue = MenuItemImage::create("login_out_cancel_up.png", "login_out_cancel_down.png", CC_CALLBACK_1(ExitGameLayer::btGameContinueCallback, this));
	menuGameContinue->setPosition(visibleSize.width / 2 + 130, visibleSize.height / 2 - 79);

	MenuItemImage* menuCloseBox = MenuItemImage::create("close_box_up.png", "close_box_down.png", CC_CALLBACK_1(ExitGameLayer::btGameContinueCallback, this));
	menuCloseBox->setPosition(visibleSize.width / 2 + 224, visibleSize.height / 2 + 122);

	Menu* menu = Menu::create(menuGoHall, menuGameContinue, menuCloseBox, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);

	return true;
}

void ExitGameLayer::btGoHallCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->exitGameDelegate->onGoHall();
}

void ExitGameLayer::btGameContinueCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->exitGameDelegate->onGameContinue();
}

void ExitGameLayer::setExitGameDelegate(ExitGameDelegate *exitGameDelegate){
	this->exitGameDelegate = exitGameDelegate;
}