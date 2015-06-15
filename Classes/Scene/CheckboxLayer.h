#ifndef _CHECK_BOX_LAYER_
#define _CHECK_BOX_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class CheckboxDelegate {
public:
	CheckboxDelegate() {};
	~CheckboxDelegate() {};

	// bChecked: true 表示选中, false 为未选中
	virtual void checked(bool bChecked) = 0;
};

class CheckboxLayer : public Layer{
public:
	virtual bool init();
	CREATE_FUNC(CheckboxLayer);
	CC_SYNTHESIZE(CheckboxDelegate*, pDelegate, CheckboxDelegate);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

private:
	Sprite *sp_checked;
	Sprite *sp_unchecked;
    Rect m_rect;
	Point m_beginPoint;
	Point m_endPoint;
	bool m_bChecked;
};

#endif