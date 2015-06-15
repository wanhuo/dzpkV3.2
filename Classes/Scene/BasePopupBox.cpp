#include "BasePopupBox.h"

bool BasePopupBox::init(){
	if(!Layer::init()){
		return false;
	}

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