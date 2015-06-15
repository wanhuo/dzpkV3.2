#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "Utils/DBUtil.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    
    void onHttpGetServerSetting(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
protected:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 visibleOrigin;
};

#endif // _APP_DELEGATE_H_

