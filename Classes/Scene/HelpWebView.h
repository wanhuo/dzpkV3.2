//
//  HelpWebView.h
//  mangosanguo
//
//  Created by Gino on 12-10-24.
//  Copyright (c) 2012年 private. All rights reserved.
//

#ifndef __mangosanguo__HelpWebView__
#define __mangosanguo__HelpWebView__

#include "cocos2d.h"
#include "../GlobalDef/GlobalDef.h"

using namespace cocos2d;

class HelpWebView:public Layer {
public:
    HelpWebView();
    virtual ~HelpWebView();
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    CREATE_FUNC(HelpWebView);

public:
    void addWebView();
    
    static __String HelpToUrl(int type);
    static __String HelpToBuildingUrl(int key);
    
    void loadData();
    void requestUrl(__String str);
    
private:
    void* pWebView_;
    float deltaScreenX;
	float deltaScreenY;
};

#endif /* defined(__·__HelpWebView__) */
