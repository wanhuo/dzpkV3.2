#include "CheckboxLayer.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/GlobalDef.h"

bool CheckboxLayer::init(){
	if(!Layer::init()){
		return false;
	}

	m_bChecked = false;

	// checked//
	sp_checked = Sprite::create("gamehall_check_t.png");
	sp_checked->setPosition(43, 15);
	sp_checked->setVisible(m_bChecked);
	this->addChild(sp_checked);
	
	// unchecked
	sp_unchecked = Sprite::create("gamehall_check_f.png");
	sp_unchecked->setPosition(43, 15);
	sp_unchecked->setVisible(!m_bChecked);
	this->addChild(sp_unchecked);
    
    
    m_rect.origin.x = (25);//hide_room->setPosition(25, 90);
	m_rect.origin.y = (90);//hide_room->setPosition(25, 90);
	m_rect.size.width = sp_unchecked->getContentSize().width;
	m_rect.size.height = sp_unchecked->getContentSize().height;
    
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CheckboxLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(CheckboxLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(CheckboxLayer::onTouchEnded,this);
	listener->setSwallowTouches(true);//不向下传递触摸
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}

bool CheckboxLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_beginPoint  = Director::getInstance()->convertToGL(touch->getLocationInView());
	return true;
}

void CheckboxLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	
}

void CheckboxLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
//	m_endPoint = touch->getLocationInView();
//    m_endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
    
    m_endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (m_rect.containsPoint(m_endPoint)){
        
    
    if (fabs(m_endPoint.x - m_beginPoint.x) < 10.0){
	  if( fabs(m_endPoint.y - m_beginPoint.y) < 10.0){
        m_bChecked = !m_bChecked;
		sp_unchecked->setVisible(!m_bChecked);
		sp_checked->setVisible(m_bChecked);
		 if (getCheckboxDelegate()) {
		 	getCheckboxDelegate()->checked(m_bChecked);
		  }

       }
     }
    }

}