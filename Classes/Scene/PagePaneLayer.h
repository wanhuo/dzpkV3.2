#ifndef _PAGE_PANE_LAYER_
#define _PAGE_PANE_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

enum PAGE_PANE_ROOM_FLAG
{
	ROOM_NEWBIE = 1,	// ���ֳ�
	ROOM_JUNIOR,		// ������
	ROOM_MIDDLE,		// �м���
	ROOM_SENIOR,		// �߼���
	ROOM_QUICK			// ���ٳ�
};

enum PAGE_PANE_MALL_FLAG
{
	MALL_INFO_CHIP = 1,	// ����
	MALL_INFO_PROP,		// ����
    MALL_INFO_PRESENT	// ����
};

class PagePaneDelegate {
public:
	PagePaneDelegate() {};
	~PagePaneDelegate() {};

	// nRoom: ѡ�з�������
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
	Rect m_rect;	// ��¼��ǰlayer��screen�ľ�������
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