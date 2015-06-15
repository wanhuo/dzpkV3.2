#ifndef _GAME_HALL_SCENE_
#define _GAME_HALL_SCENE_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../NetWork/netbean.h"
#include "CheckboxLayer.h"
#include "PagePaneLayer.h"
#include "../GlobalDef/Data/GameHallData.h"
#include "LoadLayer.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "../GlobalDef/Data/RoomData.h"
#include "GameScene.h"
#include "../GlobalDef/ResourcePreload.h"
#include "LoadLayer.h"
#include "ProductData.h"

#define HALL_CELL_SELECTED_TAG 100

using namespace network;
USING_NS_CC;
USING_NS_CC_EXT;

class GameHallScene : public Layer
					, public CheckboxDelegate
					, public PagePaneDelegate
					, public TableViewDataSource
					, public TableViewDelegate
					, public NetDelegate{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(GameHallScene);

	void btJoinTableMenuCallback(Ref *object);
	void btGoBackMenuCallback(Ref *object);
	void btnRefreshRoomMenuCallback(Ref *object);

	virtual void checked(bool bChecked);
	virtual void clicked(int nRoom);

public:
	virtual void onCreate();

	virtual void onConnected();

	virtual void onMessage(char *buff, int len);

	void onHttpGetPhoto(HttpClient *sender, HttpResponse *response);

protected:
	void onDoubleClicked(float dt);				//�ڻص������д������¼�

private:
	void getGameHallData(PAGE_PANE_ROOM_FLAG nRoom, bool hideRoomEmpty);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  //�û���½�ص�����
	~GameHallScene();

    virtual Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);

    ProductData *dataPro;            
                    
    Size visibleSize;
    Vec2 visibleOrigin;
    
    float deltaScreenX;
    float deltaScreenY;
	Vector<GameHallData*> m_roomDatas;
	LoadLayer* loadLayer;
	TableView* tableView;
	int m_nLastIndex;
	PAGE_PANE_ROOM_FLAG m_curRoom;			//�������

	bool doubleClicked;						//����˫���¼��ж�

	bool hideEmptyRoom;						//�Ƿ����ؿշ���
                        
    Scene* gameScene;
};

#endif