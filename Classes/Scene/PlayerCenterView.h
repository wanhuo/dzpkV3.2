#ifndef _PlayerCenterView_
#define _PlayerCenterView_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../NetWork/netbean.h"
#include "../GlobalDef/GlobalDef.h"
#include "BasePopupBox.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SFScale9Sprite.h"
#include "ProductData.h"
#include "LoadLayer.h"
#include "HandleDownLoadView.h"
#include "DownHeadImageView.h"
#include "RoomData.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "PlayerCenterViewDelegate.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;
class PlayerCenterView : public Layer,public TableViewDataSource, public TableViewDelegate,public HandleDownLoadViewDelegate,public DownHeadImageViewDelegate{
public:
	virtual bool init();

	virtual void onEnter();
    
    virtual void clicked(ProductData* data);
    virtual void clickByDown(ProductData* data);
    
    bool isDown;
	CREATE_FUNC(PlayerCenterView);
    //返回按钮事件
    void btGoBackMenuCallback(Ref *object);
    
    //tablecell的查看道具按钮事件，
    void clickCheckBtn(Ref *object);
    
    void clickCheckSexRadioBtn(Ref *object);
    //tablecell的查看道具按钮事件，
    void clickIsSlelect(Ref *object);
    //改变头像，
    void updateHeadImage(Ref *object);
    //保存设置事件，
    void keepSettingBtn(Ref *object);
    //插入数据操作
    void insertData(int index,bool isdelete);
	void initView();//≥ı ºªØ“≥√ÊπÃ∂®UI

    void PlayerInfoView();
	void MainView();//÷˜“™ƒ⁄»›U
    void getData();
//    void siliderOver(cocos2d::Ref *pSender, cocos2d::ui::ListViewEventType type);
    void valueChanged(Ref* object, Control::EventType type);
    
    virtual void scrollViewDidScroll(ScrollView *view);
    
    virtual void scrollViewDidZoom(ScrollView* view);
	virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    
    
    
    void playerBagDataReq();
    
    void playerBagDataResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void replacePlayerInfoResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void sendOnceDataReq(int ids);
    
    void sendOnceDataResp(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void setPlayerCenterViewDelegate(PlayerCenterViewDelegate* playerCenterViewDelegate);
    
protected:
	Size visibleSize;
	Vec2 visibleOrigin;
	float deltaScreenX;
	float deltaScreenY;
    int paper_index;
     LoadLayer* loadLayer;
    MenuItemToggle *rememberPasswordItem;
    MenuItemToggle *rememberNameItem ;
    
    MenuItemToggle *checkBoy;
    MenuItemToggle *checkGirl ;
    Sprite* bg_Instructions;
    Layer *infoLayer;
    Label *labelNone;
//    int set_num;
    
    DownHeadImageView* head;
    Rect tableRect;
//    cocos2d::ui::ListView *listview;
    SFScale9Sprite *bg;
    float scales;
    float scaleY;
    TableView* tableView;
    ControlSlider* rankListSlider;
    bool m_bTable;
    bool m_bSlider;
    int oldIndex;
    Sprite *title;
    int curent_head_index;
    Vector<ProductData*> productDatas;
    Vector<ProductData*> productDataAll;
    ProductData *dataPro;
    int cureent_index;//新的头像系数
    int old_Index;//判断是否改变了头像
    int cureent_sex;//新的头像系数
    int old_sex;//判断是否改变了头像
    int select_index;
    std::string nike_name;
    std::string imagePath;
    std::string sexStr;
    enum{
        TAG_MENUITEM1=0,
        TAG_MENUITEM2=2,
        TAG_TABLE_CELL=3,
         TAG_CHECK_BOY=4,
        TAG_CHECK_GIRL=5,
    };
    
    PlayerCenterViewDelegate* playerCenterViewDelegate;
};

#endif