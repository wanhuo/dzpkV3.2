//
//  LoginOutLayer.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-11.
//
//

#include "LoginOutLayer.h"
#include "LoginScene.h"
#include "../NetWork/netbean.h"

bool LoginOutLayer::init(){
    if(!BasePopupBox::init()){
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
	Sprite* backGroundSprite = Sprite::create("exit_game_bg.png");
	backGroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(backGroundSprite);
    
	MenuItemImage* menuOk = MenuItemImage::create("login_out_ok_up.png", "login_out_ok_down.png", CC_CALLBACK_1(LoginOutLayer::btOKCallback, this));
	menuOk->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2 - 82);
    
	MenuItemImage* menuCancel = MenuItemImage::create("login_out_cancel_up.png", "login_out_cancel_down.png", CC_CALLBACK_1(LoginOutLayer::btCancelCallback, this));
	menuCancel->setPosition(visibleSize.width / 2 + 130, visibleSize.height / 2 - 79);
    
	MenuItemImage* menuCloseBox = MenuItemImage::create("close_box_up.png", "close_box_down.png", CC_CALLBACK_1(LoginOutLayer::btCancelCallback, this));
	menuCloseBox->setPosition(visibleSize.width / 2 + 224, visibleSize.height / 2 + 122);
    
	Menu* menu = Menu::create(menuOk, menuCancel, menuCloseBox, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);
    
    return true;
}

void LoginOutLayer::btOKCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    NetBean::getInstance()->close();
	Scene* loginScene = LoginScene::createScene();
	Director::getInstance()->replaceScene(loginScene);
}

void LoginOutLayer::btCancelCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    this->removeFromParent();
}