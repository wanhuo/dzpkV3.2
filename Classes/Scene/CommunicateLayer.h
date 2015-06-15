#ifndef _COMMUNICATE_LAYER_
#define _COMMUNICATE_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include "CommunicateLayerDelegate.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define CELL_BG_DOWN_TAG 200

class CommunicateLayer : public Layer, public TableViewDataSource, public TableViewDelegate{	
public:
	CREATE_FUNC(CommunicateLayer);

	virtual bool init();

	void setCommunicateLayerDelegate(CommunicateLayerDelegate* communicateDelegate);

	virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
	virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);

    void btnCommunicateInputMenuCallback(Ref *ref);

protected:
	Size visibleSize;
    Rect tableRect;

	std::vector<std::string> communicateList;	//聊天内容列表

	CommunicateLayerDelegate* communicateDelegate;
};

#endif