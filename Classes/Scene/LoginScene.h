#ifndef _LOGIN_SCENE_
#define _LOGIN_SCENE_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "LoadLayer.h"
#include "../NetWork/netbean.h"
#include "GameScene.h"
#include "../Utils/DBUtil.h"
#include "../GlobalDef/table/UserInfoTable.h"

using namespace network;
USING_NS_CC;
USING_NS_CC_EXT;

class LoginScene : public Layer, public NetDelegate{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(LoginScene);

	void qqLoginMenuCallback(Ref *object);
	void guestLoginMenuCallback(Ref *object);
    
    void onHttpGetServerSetting(HttpClient *sender, HttpResponse *response);

	void onNewGuestLoginHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  //新用户第一次注册登陆回调方法
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  //用户登陆回调方法
	void onHttpRequestGetUserInfoCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  //获取用户信息回调方法

	virtual void update(float dt);
	
	void registerNewGuest();
	void readSqlAndLogin();			//读取数据库信息并登录

public:
	virtual void onCreate();

	virtual void onConnected();

	virtual void onMessage(char *buff, int len);

	///当断开连接时调用
	virtual void onDisconnected(){}
	///当连接超时时调用
	virtual void onConnectTimeout(){}

private:
	~LoginScene();

	Size visibleSize;
	Vec2 visibleOrigin;

	int roomId;

	LoadLayer* loadLayer;
    
    Scene* gameScene;
};

#endif