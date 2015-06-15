//
//  RoomUserInfoLayer.h
//  dzpkV3.2
//
//  Created by apple on 14-9-20.
//
//

#ifndef dzpkV3_2_RoomUserInfoLayer_h
#define dzpkV3_2_RoomUserInfoLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include <stdlib.h>
#include "../GlobalDef/Data/RoomData.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/GlobalDef.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "SendChopMsgDelegate.h"

#define PHOTO_SPRITE_TAG 101
#define USER_ACCOUNT_LABEL_TAG 102
#define USER_ID_LABEL_TAG 103
#define USER_CHIP_ACCOUNT_LABEL_TAG 104
#define USER_DZPK_LEVEL_LABEL_TAG 105

#define USER_TOTAL_TIME_LABEL_TAG 106
#define USER_WIN_AND_FAIL_TIME_LABEL_TAG 107
#define USER_WIN_PER_LABEL_TAG 108
#define BIGGEST_WIN_CHIP_LABEL_TAG 109
#define BIGGEST_HAND_CARD_LABEL_TAG 110
//占用111～115
#define USER_CARD_INFO_HAND_CARD_TAG 111

#define USER_CHOP_BG_TAG 116
#define USER_CHOP_MENU_TAG 117
//占用121~127
#define USER_CHOP_MENU_ITEM_TAG 120
//占用141～145
#define USER_CHOP_DISABLE_BG_TAG 140

#define REFRESH_SPRITE_TAG 150

USING_NS_CC;
USING_NS_CC_EXT;

class RoomUserInfoLayer : public Layer, public TableViewDataSource, public TableViewDelegate{
public:
    static RoomUserInfoLayer* create(int seatNum, int seatDelta, ROOM_USER_INFO_T *userInfo, Texture2D* texture);
    virtual bool init();
    
    void onGetUserInfoHttpRequestCompleted(network::HttpClient *sender, network::HttpResponse *response);
    
    void setSendChopMsgDelegate(SendChopMsgDelegate* sendChopMsgDelegate);
    
    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    
protected:
    void showAndHideUserInfo(bool show);
    void showAndHideCardInfo(bool show);
    
protected:
    Size visibleSize;
    Vec2 centerPositionFromScreenCenter;
    
    int seatDelta;
    
    TableView *tableView;
    
    Rect btnRect;
    Rect layerRect;
    
    Rect tableViewRect;
    int cellTouchedIndex;
    TableViewCell* touchedCell;
    
    Vec2 m_beginPoint;
	Vec2 m_endPoint;
    
    int seatNum;
    ROOM_USER_INFO_T *userInfo;
    Texture2D* texture;
    
    Sprite* room_user_btn_up;
    Sprite* room_user_btn_down;
    Sprite* room_card_btn_up;
    Sprite* room_card_btn_down;
    
    SendChopMsgDelegate* sendChopMsgDelegate;
};

#endif
