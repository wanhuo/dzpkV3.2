#ifndef _PAGE_PANE_LAYER_
#define _PAGE_PANE_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

enum PAGE_PANE_ROOM_FLAG
{
	ROOM_NEWBIE = 1,	// 新手场
	ROOM_JUNIOR,		// 初级场
	ROOM_MIDDLE,		// 中级场
	ROOM_SENIOR,		// 高级场
	ROOM_QUICK			// 快速场
};

enum PAGE_PANE_MALL_FLAG
{
	MALL_INFO_CHIP = 1,	// 筹码
	MALL_INFO_PROP,		// 道具
    MALL_INFO_PRESENT	// 礼物
};

class PagePaneDelegate {
public:
	PagePaneDelegate() {};
	~PagePaneDelegate() {};

	// nRoom: 选中房间类型
	virtual void clicked(int nRoom) = 0;
};

class PagePaneLayer : public Layer {
public:
	virtual bool init();
	CREATE_FUNC(PagePaneLayer);
	CC_SYNTHESIZE(PagePaneDelegate*, pDelegate, PagePaneDelegate);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

protected:
	Rect m_rect;	// 记录当前layer在screen的矩形区域
	int m_width;

	Vec2 m_beginPoint;
	Vec2 m_endPoint;
};

class MallPagePaneLayer : public PagePaneLayer {
public:
    static MallPagePaneLayer* create(PAGE_PANE_MALL_FLAG pagePaneMallFlag = MALL_INFO_CHIP);
    
	virtual bool init();
    
protected:
    PAGE_PANE_MALL_FLAG pagePaneMallFlag;
};

#endif