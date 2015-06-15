#include "MainSceneTouchLayer.h"

bool MainSceneTouchLayer::init(){
	if(!Layer::init()){
		return false;
	}

	//ÆÁ±Î´¥Ãþ²ã
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* t, Event* e){
		this->touchDelegate->onTouchDelegate(t);
		return false;
	};
	listener->setSwallowTouches(false);

	Director::getInstance()->getEventDispatcher()->setPriority(listener, -128);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void MainSceneTouchLayer::setMainSceneTouchDelegate(MainSceneTouchDelegate *touchDelegate){
	this->touchDelegate = touchDelegate;
}