#include "HasNotEnoughChipLayer.h"

bool HasNotEnoughChipLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* backGround = Sprite::create("has_not_enough_bg.png");
	backGround->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(backGround);

	MenuItemImage* goNowMenuItem = MenuItemImage::create("bt_go_now_up.png", "bt_go_now_down.png", CC_CALLBACK_1(HasNotEnoughChipLayer::btGoNowCallback, this));
	goNowMenuItem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 120);

	MenuItemImage* menuCloseBox = MenuItemImage::create("close_box_up.png", "close_box_down.png", CC_CALLBACK_1(HasNotEnoughChipLayer::btCloseHasNotEnoughChipLayerCallback, this));
	menuCloseBox->setPosition(visibleSize.width / 2 + 224, visibleSize.height / 2 + 134);

	Menu* menu = Menu::create(goNowMenuItem, menuCloseBox, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);

	return true;
}

//设置代理
void HasNotEnoughChipLayer::setHasNotEnoughChipDelegate(HasNotEnoughChipDelegate *hasNotEnoughChipDelegate){
	this->hasNotEnoughChipDelegate = hasNotEnoughChipDelegate;
}

void HasNotEnoughChipLayer::btGoNowCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	//跳转到充值界面
	this->hasNotEnoughChipDelegate->onHasNotEnoughChip();
}

void HasNotEnoughChipLayer::btCloseHasNotEnoughChipLayerCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->hasNotEnoughChipDelegate->onCloseHasNotEnoughChipLayer();
}