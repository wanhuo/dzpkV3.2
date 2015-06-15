//
//  NetConnectErrorLayer.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-24.
//
//

#include "NetConnectErrorLayer.h"

bool NetConnectErrorLayer::init(){
    if(!BasePopupBox::init()){
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* netConnectErrorBg = Sprite::create("net_connect_error.png");
    netConnectErrorBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(netConnectErrorBg);
    
    MenuItemImage* menuGoHall = MenuItemImage::create("login_out_ok_up.png", "login_out_ok_down.png", CC_CALLBACK_1(NetConnectErrorLayer::btGoHallCallback, this));
	menuGoHall->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 82);
    
	MenuItemImage* menuCloseBox = MenuItemImage::create("btn_close_up.png", "btn_close_down.png", CC_CALLBACK_1(NetConnectErrorLayer::btGoHallCallback, this));
	menuCloseBox->setPosition(visibleSize.width / 2 + 228, visibleSize.height / 2 + 130);
    
	Menu* menu = Menu::create(menuGoHall, menuCloseBox, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);
    
    return true;
}

void NetConnectErrorLayer::btGoHallCallback(Ref *ref){
    this->removeFromParent();
    auto gameHallScene = GameHallScene::createScene();
    Director::getInstance()->replaceScene(gameHallScene);
}