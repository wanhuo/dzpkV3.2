#include "BroadCastView.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/ResourcePreload.h"
#include "SFScale9Sprite.h"
#include "HelpWebView.h"
//消息公告类
bool BroadCastView::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();

    //全屏背景，为了后期可以拓展整体动画效果
    SFScale9Sprite *bg = SFScale9Sprite::spriteWithFile("bg3.webp");
    bg->ignoreAnchorPointForPosition(false);
    bg->setContentSize(visibleSize);
	bg->setPosition(visibleSize.width / 2,visibleSize.height / 2);
    this->addChild(bg);
    
   //消息公告类背景
	Sprite* bg1 = Sprite::create("broadcast_bg.png");
	bg1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg1);

    //消息公告关闭按钮
	auto btn_close_item = MenuItemImage::create("updateLayer_close_btn_t.png", "updateLayer_close_btn_n.png", CC_CALLBACK_1(BroadCastView::btGoBackMenuCallback, this));
	btn_close_item->setPosition(bg1->getPositionX()+bg1->getContentSize().width*0.5-10,bg1->getPositionY()+bg1->getContentSize().height*0.5-10);

//	auto update_btn_item = MenuItemImage::create("updatelayer_update_btn_t.png", "updatelayer_update_btn_f.png", CC_CALLBACK_1(BroadCastView::btGoBackMenuCallback, this));
//	update_btn_item->setPosition(bg1->getPositionX(),bg1->getPositionY()-bg1->getContentSize().height*0.5+90);

    auto menu = Menu::create(btn_close_item, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);
    
    //加载网页
    HelpWebView *helpWebView = HelpWebView::create();
    helpWebView->setAnchorPoint(Vec2(0.5,0.5));
    helpWebView->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(helpWebView);
//     HelpWebView *webview = [[HelpWebView alloc]initWithNibName:nil bundle:nil];
//    [[glview].keyWindow addSubview:[webview view]];
	return true;
}

//¹Ø±Õ°´Å¥µã»÷ÊÂ¼þ
void BroadCastView::btGoBackMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}

//°æ±¾¸üÐÂ°´Å¥µã»÷ÊÂ¼þ
void BroadCastView::onclickUpdateBtn(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}