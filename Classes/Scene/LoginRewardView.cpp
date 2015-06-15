#include "LoginRewardView.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/JsonWritter.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "../GlobalDef/ResourcePreload.h"
#include "SFScale9Sprite.h"
bool LoginRewardView::init(){
	if(!BasePopupBox::init()){
		return false;
	}
    redNB=0;
	visibleOrigin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	deltaScreenX = GlobalDef::getInstance()->getDeltaScreenX();
	deltaScreenY = GlobalDef::getInstance()->getDeltaScreenY();

    SFScale9Sprite *bg = SFScale9Sprite::spriteWithFile("bg3.webp");
    bg->ignoreAnchorPointForPosition(false);
    bg->setContentSize(visibleSize);
	bg->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(bg);

	Sprite* bg1 = Sprite::create("loginreward_bg.png");
	bg1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg1);
    
    redNB=GlobalDef::getInstance()->getLoginRewardNum();
   __String *numbers= __String::createWithFormat("%d",redNB);
    Label* rewardNum = Label::createWithSystemFont("", "Arial", 40);
	rewardNum->setString(numbers->getCString());
	rewardNum->setAnchorPoint(Vec2(0, 0.5));
    rewardNum->setColor(Color3B(213, 201, 57));
	rewardNum->setPosition(bg1->getPositionX()+20,bg1->getPositionY());
	this->addChild(rewardNum);
    
	auto btn_close_item = MenuItemImage::create("updateLayer_close_btn_t.png", "updateLayer_close_btn_n.png", CC_CALLBACK_1(LoginRewardView::btGoBackMenuCallback, this));
	btn_close_item->setPosition(bg1->getPositionX()+bg1->getContentSize().width*0.5-10,bg1->getPositionY()+bg1->getContentSize().height*0.5-10);
    
    auto btn_confirm_item = MenuItemImage::create("loginreward_btn_t.png", "loginreward_btn_f.png", CC_CALLBACK_1(LoginRewardView::btGoBackMenuCallback, this));
	btn_confirm_item->setPosition(bg1->getPositionX(),bg1->getPositionY()-bg1->getContentSize().height*0.5+70);


    auto menu = Menu::create(btn_close_item,btn_confirm_item, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);
	return true;
}

//关闭按钮点击事件
void LoginRewardView::btGoBackMenuCallback(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    Label *chip=(Label *)this->getParent()->getChildByTag(TAG_LABEL_GOLDNUM);
  
    auto stdChipAccount =UserInfo::getInstance()->getChipAccount();
	int  number= atoi(stdChipAccount.c_str());
    int  chipAccount=number+redNB;
    __String* strChipAccount = __String::createWithFormat("%d",chipAccount);
    UserInfo::getInstance()->setChipAccount(strChipAccount->getCString());
	if(chipAccount > 1000000){
		strChipAccount = __String::createWithFormat("筹码：%fM", chipAccount * 1.0 / 1000000);
		chip->setString(strChipAccount->getCString());
	}
	else{
		strChipAccount = __String::create(GlobalChineseXml::getInstance()->getValue("chipLabel"));
		strChipAccount->append(UserInfo::getInstance()->getChipAccount());
		chip->setString(strChipAccount->getCString());
	}
    this->removeFromParent();
}

//版本更新按钮点击事件
void LoginRewardView::onclickRewardBtn(Ref *object)
{
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    Label *chip=(Label *)this->getParent()->getChildByTag(TAG_LABEL_GOLDNUM);
    auto stdChipAccount =UserInfo::getInstance()->getChipAccount();
	int  number= atoi(stdChipAccount.c_str());
    int  chipAccount=number+redNB;
    __String* strChipAccount = __String::createWithFormat("%d",chipAccount);
    UserInfo::getInstance()->setChipAccount(strChipAccount->getCString());
	if(chipAccount > 1000000){
		strChipAccount = __String::createWithFormat("筹码：%fM", chipAccount * 1.0 / 1000000);
		chip->setString(strChipAccount->getCString());
	}
	else{
		strChipAccount = __String::create(GlobalChineseXml::getInstance()->getValue("chipLabel"));
		strChipAccount->append(UserInfo::getInstance()->getChipAccount());
		chip->setString(strChipAccount->getCString());
	}
    this->removeFromParent();
}