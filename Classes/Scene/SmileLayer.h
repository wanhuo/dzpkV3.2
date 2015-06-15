//
//  SmileLayer.h
//  dzpkV3.2
//
//  Created by apple on 14-9-18.
//
//

#ifndef dzpkV3_2_SmileLayer_h
#define dzpkV3_2_SmileLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CommunicateInputDelegate.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SmileLayer : public Layer, public TableViewDataSource, public TableViewDelegate{
public:
    CREATE_FUNC(SmileLayer);
    virtual bool init();
    
    void setCommunicateInputDelegate(CommunicateInputDelegate* inputDelegate);
    
    void btnSmileMenuCallback(int touchedTag);
    
    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    
protected:
    Size visibleSize;
    
    bool touched;
    int cellTouchedIndex;
    TableViewCell* touchedCell;
    Rect tableRect;
    
    CommunicateInputDelegate* inputDelegate;
};

#endif
