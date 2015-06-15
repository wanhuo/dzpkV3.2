#ifndef _NoticeView_
#define _NoticeView_


#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "BasePopupBox.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SFScale9Sprite.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../NetWork/netbean.h"
#include "LoadLayer.h"
#include "UserInfo.h"
#include "NoticeData.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;
class NoticeView : public Layer,public TableViewDataSource, public TableViewDelegate{
public:
	virtual bool init();

	virtual void onEnter();
	CREATE_FUNC(NoticeView);
    
    void btGoBackMenuCallback(Ref *object);
    void clickCheckBtn(Ref *object);

	void initView();//3?ê??ˉò3??1ì?¨UI

	void MainView();//?÷òa?úèYU
    void updateData(int type);
    void siliderOver(cocos2d::Ref *pSender, cocos2d::ui::ListViewEventType type);
    void valueChanged(Ref* object, Control::EventType type);
    
    virtual void scrollViewDidScroll(ScrollView *view);
	virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    
    void playerBagDataReq();
    void playerBagDataResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void deleteMessageDataReq();
    void deleteMessageDataResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
protected:
	Size visibleSize;
	Vec2 visibleOrigin;
	float deltaScreenX;
	float deltaScreenY;
    MenuItemToggle *rememberPasswordItem;
    MenuItemToggle *rememberNameItem ;
    	Rect tableRect;
//  cocos2d::ui::ListView *listview;
    SFScale9Sprite *bg;
    float scales;
    TableView* tableView;
    ControlSlider* rankListSlider;
    LoadLayer* loadLayer;
    Vector<NoticeData*> noticeDatas;
    Vector<NoticeData*> noticeDatasAll;
    Vector<NoticeData*> deleteDatas;

//    Vector<int> IdNums;//超过30条需要删除的ID
    int type;

    
    bool m_bTable;
    bool m_bSlider;
    enum{
        TAG_MENUITEM1,
        TAG_MENUITEM2,
    };
};

#endif