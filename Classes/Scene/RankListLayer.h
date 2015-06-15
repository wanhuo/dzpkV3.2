#ifndef _RANK_LIST_LAYER_
#define _RANK_LIST_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MainScene.h"
#include "LoadLayer.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../GlobalDef/JsonWritter.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/Data/RankListData.h"

USING_NS_CC;

#define WEALTH_RANK_UP_TAG 100
#define WEALTH_RANK_DOWN_TAG 101
#define EXPERIENCE_RANK_UP_TAG 102
#define EXPERIENCE_RANK_DOWN_TAG 103

typedef enum{
	RANK_LIST_WEALTH = 1,
	RANK_LIST_EXPERIENCE
}RankListType;

class RankListLayer : public Layer, public TableViewDataSource, public TableViewDelegate{	
public:
	static Scene* createScene();

	CREATE_FUNC(RankListLayer);

	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 
protected:
	void btGoBackMenuCallback(Ref *ref);

	void valueChanged(Ref* object, Control::EventType type);

	void getRankListData(RankListType rankListType);				//获取排行榜数据
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  //获取排行榜数据回调方法

	virtual void scrollViewDidScroll(ScrollView *view);
	virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell);

protected:
	RankListType showRankListType;
	Size visibleSize;
	float deltaScreenX;
	float deltaScreenY;

	Vector<RankListData*> rankListDatas;

	ControlSlider* rankListSlider;
	TableView* tableView;
	LoadLayer* loadLayer;

	Rect m_rect;	// 记录当前layer在screen的矩形区域
	Rect tableRect;

	Vec2 m_beginPoint;
	Vec2 m_endPoint;

	bool m_bTable;
    bool m_bSlider;
};

#endif