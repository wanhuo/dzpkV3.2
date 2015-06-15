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

	void onNewGuestLoginHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  //���û���һ��ע���½�ص�����
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  //�û���½�ص�����
	void onHttpRequestGetUserInfoCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  //��ȡ�û���Ϣ�ص�����

	virtual void update(float dt);
	
	void registerNewGuest();
	void readSqlAndLogin();			//��ȡ���ݿ���Ϣ����¼

public:
	virtual void onCreate();

	virtual void onConnected();

	virtual void onMessage(char *buff, int len);

	///���Ͽ�����ʱ����
	virtual void onDisconnected(){}
	///�����ӳ�ʱʱ����
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