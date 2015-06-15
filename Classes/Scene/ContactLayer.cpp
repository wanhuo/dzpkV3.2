
#include "ContactLayer.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/ResourcePreload.h"
#include "ContactWebView.h"
bool ContactLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}
	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();


	return true;
}

//≥ı ºªØøÿº˛
void ContactLayer::initView(){

	// ±≥æ∞
	Sprite* bg_Instructions = Sprite::create("bg_setting.png");
	bg_Instructions->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg_Instructions);

	//±ÍÃ‚±≥æ∞
	auto title = Sprite::create("set_title.png");
	title->setAnchorPoint(Vec2(0.5, 1));
	title->setPosition(visibleSize.width / 2, visibleSize.height+4);
	this->addChild(title);
	// ∑µªÿmenu
	MenuItemImage *btn_go_back_menuitem = MenuItemImage::create("btn_go_back_up.png", "btn_go_back_down.png", CC_CALLBACK_1(ContactLayer
                                                                                                                            ::btGoBackMenuCallback, this));
	btn_go_back_menuitem->setAnchorPoint(Vec2(0, 1));
	btn_go_back_menuitem->setPosition(30, visibleSize.height - 9);

	auto btn_menu = Menu::create(btn_go_back_menuitem, NULL);
	btn_menu->setPosition(0, 0);
	this->addChild(btn_menu);

	Label* labelIcon = Label::createWithSystemFont(GlobalChineseXml::getInstance()->getValue("instructionslayer_titile"), "Arial", 33);
	labelIcon->setColor(Color3B(255, 255, 234));
	labelIcon->setPosition(title->getPositionX(),title->getPositionY()-title->getContentSize().height*0.5);
	this->addChild(labelIcon);
}
void ContactLayer::onEnter(){

	Layer::onEnter();
	initView();
	MainView();

}
void ContactLayer::MainView(){

    
    //加载网页
    ContactWebView *contactWebView = ContactWebView::create();
    contactWebView->setAnchorPoint(Vec2(0.5,0.5));
    contactWebView->setPosition(visibleSize.width/2,visibleSize.height/2-20);
    this->addChild(contactWebView);
    
    //	Label* contentLab = Label::createWithSystemFont("hello hello hello hello ", "Arial", 33);
    //	contentLab->setAnchorPoint(Vec2(.5,.5));
    //	contentLab->setPosition(deltaScreenX*0.5,deltaScreenY*0.5);
//	this->addChild(contentLab);

}
//πÿ±’∑Ω∑®
void ContactLayer::btGoBackMenuCallback(Ref *object)
{
	this->removeFromParent();
}