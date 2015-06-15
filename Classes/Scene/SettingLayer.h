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

	void btnExamVersionMenuCallback(Ref *object);								//���汾�ص�����
	void btnConnectCustomerMenuCallback(Ref *object);							//��ϵ�ͷ��ص�����
	void btnAboutMenuCallback(Ref *object);										//���ڻص�����
	void btGoBackMenuCallback(Ref *object);

	void volumnValueChanged(Ref* object, Control::EventType type);				//�����仯�ص�����
	void autoSeatValueChanged(Ref* object, Control::EventType type);			//�Զ������仯�ص�����
	void vibrateValueChanged(Ref* object, Control::EventType type);				//����ʾ�仯�ص�����

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