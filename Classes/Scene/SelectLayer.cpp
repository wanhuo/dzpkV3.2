#include "SelectLayer.h"
#include "../GlobalDef/GlobalChineseXml.h"

char* SelectLayer::getSpriteTexture(SELECT_GAME_FLAG nFlag, bool bDown)
{
	switch (nFlag)
	{
	case GAME_COMPETITION:
		if (bDown)
			return "main_left_down.png";
		else
			return "main_left_up.png";
	case GAME_QUICKSTART:
		if (bDown)
			return "main_mid_down.png";
		else
			return "main_mid_up.png";
	case GAME_COMMONHALL:
		if (bDown)
			return "main_right_down.png";
		else
			return "main_right_up.png";
	default:
		return "main_mid_up.png";
	}
}

void SelectLayer::updateZOrder(bool right /* = true */)
{
	if (right)
	{
		switch (m_curSelected)
		{
		case GAME_COMPETITION:
			sp_left->setZOrder(3);
			sp_right->setZOrder(2);
			sp_mid->setZOrder(1);
			break;
		case GAME_QUICKSTART:
			sp_mid->setZOrder(3);
			sp_left->setZOrder(2);
			sp_right->setZOrder(1);
			break;
		case GAME_COMMONHALL:
			sp_right->setZOrder(3);
			sp_mid->setZOrder(2);
			sp_left->setZOrder(1);
			break;
		default:
			break;
		}
	} 
	else
	{
		switch (m_curSelected)
		{
		case GAME_COMPETITION:
			sp_left->setZOrder(2);
			sp_right->setZOrder(1);
			sp_mid->setZOrder(3);
			break;
		case GAME_QUICKSTART:
			sp_mid->setZOrder(2);
			sp_left->setZOrder(1);
			sp_right->setZOrder(3);
			break;
		case GAME_COMMONHALL:
			sp_right->setZOrder(2);
			sp_mid->setZOrder(1);
			sp_left->setZOrder(3);
			break;
		default:
			break;
		}
	}
}

bool SelectLayer::init(){
	if(!Layer::init()){
		return false;
	}

	action_duration = 0.3;
	m_bEndScrolled = true;
	m_curSelected = GAME_QUICKSTART;
	Size visibleSize = Director::getInstance()->getVisibleSize();
    
	m_rect.origin.x = visibleSize.width / 2 - 230;
	m_rect.origin.y = visibleSize.height / 2 - 153;
	m_rect.size.width = 481;
	m_rect.size.height = 307;

	sp_circle = Sprite::create("main_circle.png");
	sp_circle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(sp_circle);

	sp_left = Sprite::create(getSpriteTexture(GAME_COMPETITION));
	sp_left->setTag(GAME_COMPETITION);
	sp_left->setPosition(visibleSize.width / 3 - 100, visibleSize.height / 2);
	sp_left->setScale(0.6);
	this->addChild(sp_left);

	sp_mid = Sprite::create(getSpriteTexture(GAME_QUICKSTART, true));
	sp_mid->setTag(GAME_QUICKSTART);
	sp_mid->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(sp_mid);

	sp_right = Sprite::create(getSpriteTexture(GAME_COMMONHALL));
	sp_right->setTag(GAME_COMMONHALL);
	sp_right->setScale(0.6);
	sp_right->setPosition(visibleSize.width * 2 / 3 + 100, visibleSize.height / 2);
	this->addChild(sp_right);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SelectLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(SelectLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(SelectLayer::onTouchEnded,this);
	listener->setSwallowTouches(true);//不向下传递触摸
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	this->scheduleUpdate();
	return true;
}

void SelectLayer::update(float delta)
{
	float angle = sp_circle->getRotation();
	angle = (angle - 0.174) < 0 ?  3.14 : (angle + 0.174);
	sp_circle->setRotation(angle);
}

void SelectLayer::scrollRight()
{
	m_bScrolled = true;
	m_bEndScrolled = false;

	sp_circle->setVisible(false);
	Point temp_pt = sp_mid->getPosition();
	float temp_scale = sp_mid->getScale();

	//
    CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(SelectLayer::endAction, this));

	MoveTo* mid_move = MoveTo::create(action_duration, sp_right->getPosition());
	ScaleTo* mid_scale = ScaleTo::create(action_duration, sp_right->getScale());
	Spawn* mid_action = Spawn::create(mid_move, mid_scale, NULL);
	Sequence* mid = Sequence::create(mid_action, func, NULL);

	MoveTo* right_move = MoveTo::create(action_duration, sp_left->getPosition());
	ScaleTo* right_scale = ScaleTo::create(action_duration, sp_left->getScale());
	Spawn* right_action = Spawn::create(right_move, right_scale, NULL);
	Sequence* right = Sequence::create(right_action, func, NULL);

	MoveTo* left_move = MoveTo::create(action_duration, temp_pt);
	ScaleTo* left_scale = ScaleTo::create(action_duration, temp_scale);
	Spawn* left_action = Spawn::create(left_move, left_scale, NULL);
	Sequence* left = Sequence::create(left_action, func, NULL);

	updateZOrder();
	sp_mid->runAction(mid);
	sp_right->runAction(right);
	sp_left->runAction(left);

}

void SelectLayer::endAction(Node* node)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 pt = node->getPosition();
	Sprite* sp = (Sprite*)node;
	SELECT_GAME_FLAG tag = (SELECT_GAME_FLAG)sp->getTag();

	if (fabs(pt.x - visibleSize.width / 2) < 1.0
		&& fabs(pt.y - visibleSize.height / 2) < 1.0)
	{
		sp->setTexture(getSpriteTexture(tag, true));
		sp_circle->setVisible(true);
		m_curSelected = tag;
	} else {
		sp->setTexture(getSpriteTexture(tag));
	}
	m_bEndScrolled = true;
}

void SelectLayer::scrollLeft()
{
	m_bScrolled = true;
	m_bEndScrolled = false;

	sp_circle->setVisible(false);
	Vec2 temp_pt = sp_mid->getPosition();
	float temp_scale = sp_mid->getScale();

	//
    CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(SelectLayer::endAction, this));

	MoveTo* mid_move = MoveTo::create(action_duration, sp_left->getPosition());
	ScaleTo* mid_scale = ScaleTo::create(action_duration, sp_left->getScale());
	Spawn* mid_action = Spawn::create(mid_move, mid_scale, NULL);
	Sequence* mid = Sequence::create(mid_action, func, NULL);

	MoveTo* right_move = MoveTo::create(action_duration, temp_pt);
	ScaleTo* right_scale = ScaleTo::create(action_duration, temp_scale);
	Spawn* right_action = Spawn::create(right_move, right_scale, NULL);
	Sequence* right = Sequence::create(right_action, func, NULL);

	MoveTo* left_move = MoveTo::create(action_duration, sp_right->getPosition());
	ScaleTo* left_scale = ScaleTo::create(action_duration, sp_right->getScale());
	Spawn* left_action = Spawn::create(left_move, left_scale, NULL);
	Sequence* left = Sequence::create(left_action, func, NULL);

	updateZOrder(false);
	sp_mid->runAction(mid);
	sp_right->runAction(right);
	sp_left->runAction(left);
}

bool SelectLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_beginPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	m_bScrolled = false;
	if (m_rect.containsPoint(m_beginPoint)
		&& m_bEndScrolled)
	{
		return true;
	}

	return false;
}

void SelectLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	Point movedPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (!m_bScrolled
		&& m_rect.containsPoint(m_beginPoint)
		&& m_rect.containsPoint(movedPoint)
		&& movedPoint.x - m_beginPoint.x > 30)	// move right
	{
		scrollRight();
	}else if (!m_bScrolled
		&& m_rect.containsPoint(m_beginPoint)
		&& m_rect.containsPoint(movedPoint)
		&& m_beginPoint.x - movedPoint.x > 30) // move left
	{
		scrollLeft();
	}
}

void SelectLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (m_rect.containsPoint(m_beginPoint)
		&& m_rect.containsPoint(m_endPoint)
		&& fabs(m_endPoint.x - m_beginPoint.x) < 10
		&& fabs(m_endPoint.y - m_beginPoint.y) < 10)	// clicked
	{
		if (getSelectDelegate()) {
			log("selected: %d", m_curSelected);
			getSelectDelegate()->selected(m_curSelected);
		}
	}
}