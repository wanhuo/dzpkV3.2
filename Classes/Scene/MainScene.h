#ifndef _MAIN_SCENE_
#define _MAIN_SCENE_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../NetWork/netbean.h"
#include "SelectLayer.h"
#include "RankListLayer.h"
#include "MallLayer.h"
#include "MallLayerDelegate.h"
#include "MainSceneTouchLayer.h"
#include "LoadLayer.h"
#include "ProductData.h" 
#include "DownHeadImageView.h"
#include "PlayerCenterViewDelegate.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;

#define RANK_LIST_LAYER_TAG 101
#define MAIN_SELECT_MENU_TAG 201
#define MAIN_SCENE_MALL_LAYER_TAG 211
#define PLAYER_CENTER_VIEW_TAG 212

class MainScene : public Layer
				, public MainSceneTouchDelegate
				, public NetDelegate
				, public SelectDelegate
                , public MallLayerDelegate
                , public PlayerCenterViewDelegate{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(MainScene);

	virtual void onTouchDelegate(Touch *t);

	void btMsgMenuCallback(Ref *object);
	void btMailMenuCallback(Ref *object);
	void btSelectMenuCallback(Ref *object);

	void btLogOutMenuCallback(Ref *object);
	void btSetMenuCallback(Ref *object);

	void btShopMenuCallback(Ref *object);
	void btRankMenuCallback(Ref *object);
	void btActiveMenuCallback(Ref *object);
	void btPackgeMenuCallback(Ref *object);
                    
    void btShowPlayerInfoView(Ref *object);

	virtual void selected(SELECT_GAME_FLAG nFlag);
                    
	//读取数据库信息并登录
    void isLoginRewardReq();
    
    void onLoginRewardHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
                    
    virtual void onCloseMallLayerCallback();
                    
    void onHttpGetPhoto(HttpClient *sender, HttpResponse *response);

public:
	virtual void onCreate();

	virtual void onConnected();

	virtual void onMessage(char *buff, int len);
                    
    virtual void onPlayerCenterViewGoBack();

protected:
                    
    Rect headRect;
	Size visibleSize;
	Vec2 visibleOrigin;
    LoadLayer* loadLayer;

	float deltaScreenX;
	float deltaScreenY;

	Label* chip;				//用户筹码

    ProductData  *dataPro;

    DownHeadImageView *head;
    
	Rect selectMenuRect;
    enum{
        TAG_LABEL_GOLDNUM=10,
    };
                    //弹出菜单矩形区
    Scene* gameScene;
};

#endif