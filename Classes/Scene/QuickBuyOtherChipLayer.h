//
//  QuickBuyOtherChipLayer.h
//  dzpkV3.2
//
//  Created by apple on 14-9-17.
//
//

#ifndef dzpkV3_2_QuickBuyOtherChipLayer_h
#define dzpkV3_2_QuickBuyOtherChipLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BasePopupBox.h"
#include "../GlobalDef/Data/QuickChipData.h"
#include "../GlobalDef/Data/MallInfoData.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "MallBuyInfoLayer.h"
#include "../GlobalDef/Data/RoomData.h"

USING_NS_CC;
USING_NS_CC_EXT;

class QuickBuyOtherChipLayer : public BasePopupBox, public TableViewDataSource, public TableViewDelegate{
public:
    CREATE_FUNC(QuickBuyOtherChipLayer);
    virtual bool init();
    
    void btnCloseMenuCallback(Ref *ref);
    void btnRefreshMenuCallback(Ref *ref);
    
    void btnQuickBuyMenuCallback(Ref *ref);
    
    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    
protected:
    Size visibleSize;
    
    Label* chipAccountLabel;
    TableView* tableView;
    
    Vector<MallInfoData*> infoDatas;
};

#endif
