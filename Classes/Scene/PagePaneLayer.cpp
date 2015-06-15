#include "PagePaneLayer.h"
#include "cocos2d.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/GlobalDef.h"

bool PagePaneLayer::init(){
	if(!Layer::init()){
		return false;
	}

    Size visibleSize = Director::getInstance()->getVisibleSize();
    float deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	float deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();
    float scale =1.0;
    if(deltaScreenY > 0.01){
        
        scale=1.0;
        
    }else if (deltaScreenX>0.01){
        
        scale=visibleSize.width/960;
        
    }else{
        
    }
    
	m_rect.origin.x = (visibleSize.width / 2 - 470)*scale+deltaScreenX;
	m_rect.origin.y = (visibleSize.height / 2 - 320)*scale+deltaScreenY;
	m_rect.size.width = 947.5*scale;
	m_rect.size.height = 80*scale;
	m_width = (int)(m_rect.size.width / 5);

	// 新手场
	Sprite* newbie_up = Sprite::create("btn_newbie_room_up.png");
	newbie_up->setPosition(10, 12);
	newbie_up->setAnchorPoint(Vec2(0, 0));
	newbie_up->setTag(ROOM_NEWBIE);
	newbie_up->setName("up");
	newbie_up->setVisible(false);
	this->addChild(newbie_up);

	Sprite* newbie_down = Sprite::create("btn_newbie_room_down.png");
	newbie_down->setPosition(10, 12);
	newbie_down->setAnchorPoint(Vec2(0, 0));
	newbie_down->setTag(ROOM_NEWBIE);
	newbie_down->setName("down");
	newbie_down->setVisible(true);
	this->addChild(newbie_down);

	// 初级场
	Sprite* junior_up = Sprite::create("btn_junior_room_up.png");
	junior_up->setPosition(199.5, 12);
	junior_up->setAnchorPoint(Vec2(0, 0));
	junior_up->setTag(ROOM_JUNIOR);
	junior_up->setName("up");
	junior_up->setVisible(true);
	this->addChild(junior_up);

	Sprite* junior_down = Sprite::create("btn_junior_room_down.png");
	junior_down->setPosition(199.5, 12);
	junior_down->setAnchorPoint(Vec2(0, 0));
	junior_down->setTag(ROOM_JUNIOR);
	junior_down->setName("down");
	junior_down->setVisible(false);
	this->addChild(junior_down);

	// 中级场
	Sprite* middle_up = Sprite::create("btn_middle_room_up.png");
	middle_up->setPosition(389, 12);
	middle_up->setAnchorPoint(Vec2(0, 0));
	middle_up->setTag(ROOM_MIDDLE);
	middle_up->setName("up");
	middle_up->setVisible(true);
	this->addChild(middle_up);

	Sprite* middle_down = Sprite::create("btn_middle_room_down.png");
	middle_down->setPosition(389, 12);
	middle_down->setAnchorPoint(Vec2(0, 0));
	middle_down->setTag(ROOM_MIDDLE);
	middle_down->setName("down");
	middle_down->setVisible(false);
	this->addChild(middle_down);

	// 高级场
	Sprite* senior_up = Sprite::create("btn_senior_room_up.png");
	senior_up->setPosition(578.5, 12);
	senior_up->setAnchorPoint(Vec2(0, 0));
	senior_up->setTag(ROOM_SENIOR);
	senior_up->setName("up");
	senior_up->setVisible(true);
	this->addChild(senior_up);

	Sprite* senior_down = Sprite::create("btn_senior_room_down.png");
	senior_down->setPosition(578.5, 12);
	senior_down->setAnchorPoint(Vec2(0, 0));
	senior_down->setTag(ROOM_SENIOR);
	senior_down->setName("down");
	senior_down->setVisible(false);
	this->addChild(senior_down);

	// 快速场
	Sprite* quick_up = Sprite::create("btn_quick_room_up.png");
	quick_up->setPosition(768, 12);
	quick_up->setAnchorPoint(Vec2(0, 0));
	quick_up->setTag(ROOM_QUICK);
	quick_up->setName("up");
	quick_up->setVisible(true);
	this->addChild(quick_up);

	Sprite* quick_down = Sprite::create("btn_quick_room_down.png");
	quick_down->setPosition(768, 12);
	quick_down->setAnchorPoint(Vec2(0, 0));
	quick_down->setTag(ROOM_QUICK);
	quick_down->setName("down");
	quick_down->setVisible(false);
	this->addChild(quick_down);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PagePaneLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PagePaneLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PagePaneLayer::onTouchEnded, this);
	listener->setSwallowTouches(true);//不向下传递触摸
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}

bool PagePaneLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_beginPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (!m_rect.containsPoint(m_beginPoint)){
        return false;
    }
		
	return true;
}

void PagePaneLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	
}

void PagePaneLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (m_rect.containsPoint(m_endPoint)
		&& fabs(m_endPoint.x - m_beginPoint.x) < 10.0
		&& fabs(m_endPoint.y - m_beginPoint.y) < 10.0)	// clicked
	{
		int nItem = (int)(m_beginPoint.x - m_rect.origin.x) / m_width;
		
		Vector<Node*> nodes = getChildren();
		for (Vector<Node*>::iterator ite = nodes.begin(); ite < nodes.end(); ite++)
		{
			Node* pNode = *ite;
			if (pNode->getTag() == nItem + 1)
			{
				if (pNode->getName() == "down")
				{
					pNode->setVisible(true);
				}
				else
				{
					pNode->setVisible(false);
				}
			}
			else
			{
				if (pNode->getName() == "down")
				{
					pNode->setVisible(false);
				}
				else
				{
					pNode->setVisible(true);
				}
			}
		}
		
		if (getPagePaneDelegate()) {
			getPagePaneDelegate()->clicked(nItem+1);
		}
	}
}

MallPagePaneLayer* MallPagePaneLayer::create(PAGE_PANE_MALL_FLAG pagePaneMallFlag){
    MallPagePaneLayer* layer = new MallPagePaneLayer();
    if(layer){
        layer->pagePaneMallFlag = pagePaneMallFlag;
    }
    
    if(layer && layer->init()){
        layer->autorelease();
        return layer;
    }
    else{
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool MallPagePaneLayer::init()
{
	if(!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_rect.origin.x = visibleSize.width / 2 + 82;    //visibleSize.width / 2 - 40
	m_rect.origin.y = visibleSize.height - 100;
	m_rect.size.width = 244;    //366
	m_rect.size.height = 100;
	m_width = (int)(m_rect.size.width / 2);

	// 筹码
/*	Sprite* chip_up = Sprite::create("btn_chip_mall_up.png");
	chip_up->setPosition(0, 28);
	chip_up->setAnchorPoint(Vec2(0, 0));
	chip_up->setTag(MALL_INFO_CHIP);
	chip_up->setName("up");
	chip_up->setVisible(false);
	this->addChild(chip_up);

	Sprite* chip_down = Sprite::create("btn_chip_mall_down.png");
	chip_down->setPosition(0, 28);
	chip_down->setAnchorPoint(Vec2(0, 0));
	chip_down->setTag(MALL_INFO_CHIP);
	chip_down->setName("down");
	chip_down->setVisible(true);
	this->addChild(chip_down);

	// 礼物
	Sprite* present_up = Sprite::create("btn_present_mall_up.png");
	present_up->setPosition(122, 28);
	present_up->setAnchorPoint(Vec2(0, 0));
	present_up->setTag(MALL_INFO_PRESENT);
	present_up->setName("up");
	present_up->setVisible(true);
	this->addChild(present_up);

	Sprite* present_down = Sprite::create("btn_present_mall_down.png");
	present_down->setPosition(122, 28);
	present_down->setAnchorPoint(Vec2(0, 0));
	present_down->setTag(MALL_INFO_PRESENT);
	present_down->setName("down");
	present_down->setVisible(false);
	this->addChild(present_down);
*/
    if(this->pagePaneMallFlag == MALL_INFO_CHIP){
        Sprite* chip_up = Sprite::create("btn_chip_mall_up.png");
        chip_up->setPosition(122, 28);
        chip_up->setAnchorPoint(Vec2(0, 0));
        chip_up->setTag(MALL_INFO_CHIP);
        chip_up->setName("up");
        chip_up->setVisible(false);
        this->addChild(chip_up);
        
        Sprite* chip_down = Sprite::create("btn_chip_mall_down.png");
        chip_down->setPosition(122, 28);
        chip_down->setAnchorPoint(Vec2(0, 0));
        chip_down->setTag(MALL_INFO_CHIP);
        chip_down->setName("down");
        chip_down->setVisible(true);
        this->addChild(chip_down);
        
        // 道具
        Sprite* prop_up = Sprite::create("btn_prop_mall_up.png");
        prop_up->setPosition(244, 28);
        prop_up->setAnchorPoint(Vec2(0, 0));
        prop_up->setTag(MALL_INFO_PROP);
        prop_up->setName("up");
        prop_up->setVisible(true);
        this->addChild(prop_up);
        
        Sprite* prop_down = Sprite::create("btn_prop_mall_down.png");
        prop_down->setPosition(244, 28);
        prop_down->setAnchorPoint(Vec2(0, 0));
        prop_down->setTag(MALL_INFO_PROP);
        prop_down->setName("down");
        prop_down->setVisible(false);
        this->addChild(prop_down);
    }
    else if(this->pagePaneMallFlag == MALL_INFO_PROP){
        Sprite* chip_up = Sprite::create("btn_chip_mall_up.png");
        chip_up->setPosition(122, 28);
        chip_up->setAnchorPoint(Vec2(0, 0));
        chip_up->setTag(MALL_INFO_CHIP);
        chip_up->setName("up");
        chip_up->setVisible(true);
        this->addChild(chip_up);
        
        Sprite* chip_down = Sprite::create("btn_chip_mall_down.png");
        chip_down->setPosition(122, 28);
        chip_down->setAnchorPoint(Vec2(0, 0));
        chip_down->setTag(MALL_INFO_CHIP);
        chip_down->setName("down");
        chip_down->setVisible(false);
        this->addChild(chip_down);
        
        // 道具
        Sprite* prop_up = Sprite::create("btn_prop_mall_up.png");
        prop_up->setPosition(244, 28);
        prop_up->setAnchorPoint(Vec2(0, 0));
        prop_up->setTag(MALL_INFO_PROP);
        prop_up->setName("up");
        prop_up->setVisible(false);
        this->addChild(prop_up);
        
        Sprite* prop_down = Sprite::create("btn_prop_mall_down.png");
        prop_down->setPosition(244, 28);
        prop_down->setAnchorPoint(Vec2(0, 0));
        prop_down->setTag(MALL_INFO_PROP);
        prop_down->setName("down");
        prop_down->setVisible(true);
        this->addChild(prop_down);
    }

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MallPagePaneLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MallPagePaneLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MallPagePaneLayer::onTouchEnded, this);
	listener->setSwallowTouches(true);//不向下传递触摸
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}