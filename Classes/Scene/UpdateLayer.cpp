#include "UpdateLayer.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/ResourcePreload.h"
#include "SFScale9Sprite.h"


bool UpdateLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();

    SFScale9Sprite *bg = SFScale9Sprite::spriteWithFile("bg3.webp");
    bg->ignoreAnchorPointForPosition(false);
    bg->setContentSize(visibleSize);
	bg->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(bg);

	Sprite* bg1 = Sprite::create("updateLayer_bg.png");
	bg1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg1);

	Label* current_vesion_Lab = Label::createWithSystemFont("V1.0.0", "Arial", 25);
	current_vesion_Lab->setPosition(bg1->getPositionX()-70,bg1->getPositionY()-133.5);
	current_vesion_Lab->setColor(Color3B(157, 220, 66));
	this->addChild(current_vesion_Lab);

	Label* new_vesion_Lab = Label::createWithSystemFont("V1.0.0", "Arial", 25);
	new_vesion_Lab->setPosition(bg1->getPositionX()+50,bg1->getPositionY()+2);
	new_vesion_Lab->setColor(Color3B(157, 220, 66));
	this->addChild(new_vesion_Lab);

	auto btn_close_item = MenuItemImage::create("updateLayer_close_btn_t.png", "updateLayer_close_btn_n.png", CC_CALLBACK_1(UpdateLayer::btGoBackMenuCallback, this));
	btn_close_item->setPosition(bg1->getPositionX()+bg1->getContentSize().width*0.5-10,bg1->getPositionY()+bg1->getContentSize().height*0.5-10);

	auto update_btn_item = MenuItemImage::create("updatelayer_update_btn_t.png", "updatelayer_update_btn_f.png", CC_CALLBACK_1(UpdateLayer::btGoBackMenuCallback, this));
	update_btn_item->setPosition(bg1->getPositionX(),bg1->getPositionY()-bg1->getContentSize().height*0.5+90);

    auto menu = Menu::create(btn_close_item,update_btn_item, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);
	
	return true;
}

//关闭按钮点击事件
void UpdateLayer::btGoBackMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}

//版本更新按钮点击事件
void UpdateLayer::onclickUpdateBtn(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}