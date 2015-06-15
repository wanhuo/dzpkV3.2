#include "SelectHeadView.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/ResourcePreload.h"
#include "SFScale9Sprite.h"


bool SelectHeadView::init(){
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
	bg->setPosition(visibleSize.width/3,visibleSize.height/3);
    this->addChild(bg);



	return true;
}

//关闭按钮点击事件
void SelectHeadView::btGoBackMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}

//版本更新按钮点击事件
void SelectHeadView::onclickUpdateBtn(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	this->removeFromParent();
}