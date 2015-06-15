//
//  HelpWebView.cpp
//  mangosanguo
//
//  Created by Gino on 12-10-24.
//  Copyright (c) 2012年 private. All rights reserved.
//

#include "HelpWebView.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "CCEAGLView-ios.h"
#endif
#import <UIKit/UIKit.h>

HelpWebView::HelpWebView()
{
    pWebView_ = NULL;
}

HelpWebView::~HelpWebView()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (pWebView_) {
        UIWebView* webView =  (UIWebView* )pWebView_;
        [webView removeFromSuperview];
        [webView release];
    }
#endif
}

void HelpWebView::onEnter()
{

    if (pWebView_) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UIWebView* webView =  (UIWebView* )pWebView_;
        
        auto view = cocos2d::Director::getInstance()->getOpenGLView();
        CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
        [eaglview addSubview:webView];
//        [[CCEAGLView sharedEGLView] addSubview:webView];
#endif
    }else{
        addWebView();
    }

}

void HelpWebView::onExit()
{
    if (pWebView_) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UIWebView* webView =  (UIWebView* )pWebView_;
        [webView removeFromSuperview];
#endif
    }
}

bool HelpWebView::init()
{
    if(Layer::init())
    {
        this->setTouchEnabled(true);
        return true;
    }
    return false;
}

void HelpWebView::addWebView()
{
    deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int w = 240;
    int h = 110;
    CGFloat x3 =120;
    CGFloat y3=120;
    
    
    if(deltaScreenY > 0){
        w = 280;
        h = 130;
        x3=144;
        y3=114;
	}else if(deltaScreenX > 0){
        w = 288*2;
        h = 130*2;
        x3=225;
        y3=290;
        
    }else{
       
	}
//    
//    float scale=1.0;
//    if(isIPad())
//        scale=2.0;
//    if(fabs(CCDirector::sharedDirector()->getScreneSize().width-480) < 0.1){
//        x3=15;
//        y3=40;
//    }
//    else if(isIPhone5Retina())
//    {
//        x3=60;
//        y3=45;
//    }else if(isIPhone4Retina()){
//        
//        x3=15;
//        y3=40;
//    }
//    else if(isIPadRetina()){
//        scale=2.0;
//        x3=65;
//        y3=140;
//    }else if(isIPad()){
//        x3=65;
//        y3=140;
//    }
//    else{
//        x3=15;
//        y3=40;
//    }
    
    CGRect rect = CGRectMake(x3 , y3, w*1.0, h*1.0);//1.0=scale
    UIWebView* webView = [[UIWebView alloc]initWithFrame:rect];
    
    //缩放
    [webView setScalesPageToFit:YES];
    webView.backgroundColor = [UIColor clearColor];
    webView.opaque = NO;
    
    //Before iOS 5
    for (id subview in webView.subviews){
        if ([[subview class] isSubclassOfClass: [UIScrollView class]]){
            ((UIScrollView *)subview).bounces = NO;
        }
    }
    
    //After iOS 5
    NSString *urlStr = [[[NSString alloc] initWithCString:"http://192.168.1.120/hallroom/announce.jsp" encoding:NSUTF8StringEncoding] autorelease];
    webView.scrollView.bounces = NO;
    NSURL* url = [NSURL URLWithString:urlStr];
    NSURLRequest* request = [NSURLRequest requestWithURL:url];
    [webView loadRequest:request];

    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    [eaglview addSubview:webView];
    pWebView_ = (void *)webView;
#endif
}

void HelpWebView::requestUrl(__String str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (pWebView_) {
        UIWebView* webView =  (UIWebView* )pWebView_;
        NSURL* url = [NSURL URLWithString:@"http://192.168.1.120/hallroom/announce.jsp"];
        NSURLRequest* request = [NSURLRequest requestWithURL:url];
        [webView loadRequest:request];
    }
#endif
}

__String HelpWebView::HelpToUrl(int type)
{
        return "http://www.hao123.com/";
}

__String HelpWebView::HelpToBuildingUrl(int key)
{
        __String keyId = "";
        return "http://www.hao123.com/";
}

void HelpWebView::loadData()
{
    
}