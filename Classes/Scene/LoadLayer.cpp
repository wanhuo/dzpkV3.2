#include "LoadLayer.h"

bool LoadLayer::init(){
	if(!LayerColor::initWithColor(Color4B(0, 0, 0, 144))){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite *sp_loading = Sprite::create("sp_loading.png");
	sp_loading->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 30);
	this->addChild(sp_loading);
	sp_loading->runAction(RepeatForever::create(RotateBy::create(1, -360.0)));

	Sprite *sp_loading_text = Sprite::create("sp_loading_text.png");
	sp_loading_text->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 30);
	this->addChild(sp_loading_text);

	//ÆÁ±Î´¥Ãþ²ã
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* t, Event* e){
		return true;
	};
	listener->setSwallowTouches(true);

	Director::getInstance()->getEventDispatcher()->setPriority(listener, -128);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}