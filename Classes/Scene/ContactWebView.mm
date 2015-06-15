//
//  HelpWebView.cpp
//  mangosanguo
//
//  Created by Gino on 12-10-24.
//  Copyright (c) 2012年 private. All rights reserved.
//

#include "ContactWebView.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "CCEAGLView-ios.h"
#endif
#import <UIKit/UIKit.h>
ContactWebView::ContactWebView()
{
    pWebView_ = NULL;
}

ContactWebView::~ContactWebView()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (pWebView_) {
        UIWebView* webView =  (UIWebView* )pWebView_;
        [webView removeFromSuperview];
        [webView release];
    }
#endif
}

void ContactWebView::onEnter()
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

void ContactWebView::onExit()
{
    if (pWebView_) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        UIWebView* webView =  (UIWebView* )pWebView_;
        [webView removeFromSuperview];
#endif
    }
}

bool ContactWebView::init()
{
    if(Layer::init())
    {
        this->setTouchEnabled(true);
        return true;
    }
    return false;
}

void ContactWebView::addWebView()
{
    visibleSize = Director::getInstance()->getVisibleSize();

    deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int w = 480;
    int h = 277;
    CGFloat x3 =00;
    CGFloat y3=43;
    
    
    if(deltaScreenY > 0){
        w = 568;
        h = 272;
        x3 =00;
        y3=48;
	}else if(deltaScreenX > 0){
        w = 512*2;
        h = 339*2;
        x3 =00;
        y3=45*2;
    }else{
       
	}
////
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
    NSString *urlStr = [[[NSString alloc] initWithCString:"http://www.hao123.com/" encoding:NSUTF8StringEncoding] autorelease];
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

void ContactWebView::requestUrl(__String str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (pWebView_) {
        UIWebView* webView =  (UIWebView* )pWebView_;
        NSURL* url = [NSURL URLWithString:@"http://www.hao123.com/"];
        NSURLRequest* request = [NSURLRequest requestWithURL:url];
        [webView loadRequest:request];
    }
#endif
}


__String ContactWebView::HelpToUrl(int type)
{
        return "http://www.hao123.com/";
}


__String ContactWebView::HelpToBuildingUrl(int key)
{
        __String keyId = "";
        return "http://www.hao123.com/";
}

void ContactWebView::loadData()
{
    
}