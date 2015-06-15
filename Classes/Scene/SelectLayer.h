#ifndef _SELECT_LAYER_
#define _SELECT_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

enum SELECT_GAME_FLAG
{
	GAME_COMPETITION = 1,
	GAME_QUICKSTART,
	GAME_COMMONHALL
};

class SelectDelegate {
public:
	SelectDelegate() {};
	~SelectDelegate() {};

	virtual void selected(SELECT_GAME_FLAG nFlag) = 0;
};

class SelectLayer : public Layer{
public:
	virtual bool init();
	CREATE_FUNC(SelectLayer);
	CC_SYNTHESIZE(SelectDelegate*, pDelegate, SelectDelegate);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

	virtual void update(float delta);

private:
	void scrollRight();
	void scrollLeft();
	void endAction(Node* node);
	char* getSpriteTexture(SELECT_GAME_FLAG nFlag, bool bDown = false);
	void updateZOrder(bool right = true);

	Sprite *sp_left;
	Sprite *sp_mid;
	Sprite *sp_right;
	Sprite *sp_circle;

	bool m_bScrolled;
	bool m_bEndScrolled;
	Rect m_rect;
	Point m_beginPoint;
	Point m_endPoint;
	SELECT_GAME_FLAG m_curSelected;
	float action_duration;
};

#endif