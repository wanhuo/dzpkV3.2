//
//  HasNotEnoughChopLayer.cpp
//  dzpkV3.2
//
//  Created by apple on 14-10-9.
//
//

#include "HasNotEnoughChopLayer.h"

bool HasNotEnoughChopLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}
    
	visibleSize = Director::getInstance()->getVisibleSize();
    
	Sprite* backGround = Sprite::create("has_not_enough_chop_bg.png");
	backGround->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(backGround);
    
	MenuItemImage* goNowMenuItem = MenuItemImage::create("has_not_buy_chop_up.png", "has_not_buy_chop_down.png", CC_CALLBACK_1(HasNotEnoughChopLayer::btGoNowCallback, this));
	goNowMenuItem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 75);
    
	MenuItemImage* menuCloseBox = MenuItemImage::create("btn_close_up.png", "btn_close_down.png", CC_CALLBACK_1(HasNotEnoughChopLayer::btCloseHasNotEnoughChopLayerCallback, this));
	menuCloseBox->setPosition(visibleSize.width / 2 + 240, visibleSize.height / 2 + 125);
    
	Menu* menu = Menu::create(goNowMenuItem, menuCloseBox, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);
    
	return true;
}

//…Ë÷√¥˙¿Ì
void HasNotEnoughChopLayer::setHasNotEnoughChopDelegate(HasNotEnoughChopDelegate *hasNotEnoughChopDelegate){
	this->hasNotEnoughChopDelegate = hasNotEnoughChopDelegate;
}

void HasNotEnoughChopLayer::btGoNowCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	//Ã¯◊™µΩ≥‰÷µΩÁ√Ê
	this->hasNotEnoughChopDelegate->onHasNotEnoughChop();
}

void HasNotEnoughChopLayer::btCloseHasNotEnoughChopLayerCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->hasNotEnoughChopDelegate->onCloseHasNotEnoughChopLayer();
}