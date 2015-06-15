//
//  HelpWebView.h
//  mangosanguo
//

#ifndef __mangosanguo__ContactWebView__
#define __mangosanguo__ContactWebView__

#include "cocos2d.h"
#include "../GlobalDef/GlobalDef.h"
using namespace cocos2d;
class ContactWebView:public Layer {

public:
    ContactWebView();
    virtual ~ContactWebView();
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    CREATE_FUNC(ContactWebView);

public:
    void addWebView();
    
    static __String HelpToUrl(int type);
    static __String HelpToBuildingUrl(int key);
    
    void loadData();
    void requestUrl(__String str);
    
private:
    void* pWebView_;
    cocos2d::Size visibleSize;
    float deltaScreenX;
	float deltaScreenY;
   
};

#endif /* defined(__·__HelpWebView__) */
