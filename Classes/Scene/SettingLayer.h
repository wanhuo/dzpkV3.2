#ifndef _SETTING_SCENE_
#define _SETTING_SCENE_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "BasePopupBox.h"
#include "ContactLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SettingLayer : public BasePopupBox, public TableViewDataSource, public TableViewDelegate{
public:
	virtual bool init();
	CREATE_FUNC(SettingLayer);

	void btnExamVersionMenuCallback(Ref *object);								//检查版本回调方法
	void btnConnectCustomerMenuCallback(Ref *object);							//联系客服回调方法
	void btnAboutMenuCallback(Ref *object);										//关于回调方法
	void btGoBackMenuCallback(Ref *object);

	void volumnValueChanged(Ref* object, Control::EventType type);				//音量变化回调方法
	void autoSeatValueChanged(Ref* object, Control::EventType type);			//自动入座变化回调方法
	void vibrateValueChanged(Ref* object, Control::EventType type);				//振动提示变化回调方法

public:
	virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);

protected:
	Size visibleSize;
	Vec2 visibleOrigin;

	float deltaScreenX;
	float deltaScreenY;
};

#endif