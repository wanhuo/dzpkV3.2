#include "AddChipLayer.h"
#include "../GlobalDef/Data/RoomData.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "../GlobalDef/GlobalChineseXml.h"

bool AddChipLayer::init(){
	if(!BasePopupBox::init()){
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	this->seatNum = 0;

	//����
	Sprite* addChipBackground = Sprite::create("add_chip_bg.png");
	addChipBackground->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(addChipBackground);

	//���г����ǩ
	Label* chipAccountLabel = Label::createWithSystemFont("", "Arial", 33);
	chipAccountLabel->setColor(Color3B(219, 219, 219));
	auto userId = UserInfo::getInstance()->getUserId();
	__String* strChipAccount = __String::createWithFormat("%ld", RoomData::getInstance()->getPlayer((char *)userId.c_str())->getChipAccount());
	chipAccountLabel->setString(strChipAccount->getCString());
	chipAccountLabel->setPosition(visibleSize.width / 2 + 90, visibleSize.height / 2 + 172 - 31);
	this->addChild(chipAccountLabel);

	//����������ֱ�ǩ
	Label* minBuyLabel = Label::createWithSystemFont("", "Arial", 23);
	minBuyLabel->setColor(Color3B(183, 183, 183));
	__String* strMinBuyChip = __String::createWithFormat("%ld", RoomData::getInstance()->getRoomInfo()->getMinBuyChip());
	minBuyLabel->setString(strMinBuyChip->getCString());
	minBuyLabel->setPosition(visibleSize.width / 2 - 170, visibleSize.height / 2 + 97.5 + 20 - 45);
	this->addChild(minBuyLabel);

	//����������ֱ�ǩ
	Label* maxBuyLabel = Label::createWithSystemFont("", "Arial", 23);
	maxBuyLabel->setColor(Color3B(183, 183, 183));
	auto strUserId = UserInfo::getInstance()->getUserId();
	long chipAccount = RoomData::getInstance()->getPlayer((char *)strUserId.c_str())->getChipAccount();
	long maxBuyChip = RoomData::getInstance()->getRoomInfo()->getMaxBuyChip();
	__String* strMaxBuyChip = nullptr;
	if(chipAccount > maxBuyChip){
		strMaxBuyChip = __String::createWithFormat("%ld", maxBuyChip);
	}
	else{
		strMaxBuyChip = __String::createWithFormat("%ld", chipAccount);
	}
	maxBuyLabel->setString(strMaxBuyChip->getCString());
	maxBuyLabel->setPosition(visibleSize.width / 2 + 170, visibleSize.height / 2 + 97.5 + 20 - 45);
	this->addChild(maxBuyLabel);

	//��ǰ�������ֱ�ǩ
	Label* realBuyLabel = Label::createWithSystemFont("", "Arial", 45);
	realBuyLabel->setTag(REAL_BUY_LABEL_TAG);
	realBuyLabel->setColor(Color3B(213, 156, 45));
	__String* strRealBuyChip = __String::createWithFormat("%ld", 2 * RoomData::getInstance()->getRoomInfo()->getMinBuyChip());
	realBuyLabel->setString(strRealBuyChip->getCString());
	realBuyLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 97.5 - 45);
	this->addChild(realBuyLabel);

	//������
	auto slider = ControlSlider::create("adcp_sr_bg.png", "adcp_sr_progress.png", "adcp_srtb_up.png", "adcp_srtb_down.png");
	slider->setTag(ADD_CHIP_LAYER_SLIDER_TAG);
	slider->setScale(0.87f);
	slider->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 7.5 - 30);
	slider->setMinimumValue(RoomData::getInstance()->getRoomInfo()->getMinBuyChip());	//��Сֵ
	slider->setMaximumValue(RoomData::getInstance()->getRoomInfo()->getMaxBuyChip());	//���ֵ
	slider->setValue(2 * RoomData::getInstance()->getRoomInfo()->getMinBuyChip());		//��������λ�����õ�Ĭ�ϼ�עֵ
	this->addChild(slider);
	//�����Ϣ��Ӧ������ʱ������ֵ���λ��
	slider->addTargetWithActionForControlEvents(this, cccontrol_selector(AddChipLayer::valueChanged), Control::EventType::VALUE_CHANGED);

	//�򹴲˵����������²˵�
	MenuItemImage* rightMenuItem = MenuItemImage::create("bt_right_menu_up.png", "bt_right_menu_up.png", CC_CALLBACK_1(AddChipLayer::btRightMenuCallback, this));
	rightMenuItem->setPosition(visibleSize.width / 2 - 214.5, visibleSize.height / 2 - 60.5 - 24);

	MenuItemImage* buyAndSitMenuItem = MenuItemImage::create("bt_buy_sit_up.png", "bt_buy_sit_down.png", CC_CALLBACK_1(AddChipLayer::btBuyAndSitMenuCallback, this));
	buyAndSitMenuItem->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 141);

	MenuItemImage* menuCloseBox = MenuItemImage::create("close_box_up.png", "close_box_down.png", CC_CALLBACK_1(AddChipLayer::btCloseMenuCallback, this));
	menuCloseBox->setPosition(visibleSize.width / 2 + 245, visibleSize.height / 2 + 157.5);

	Menu* menu = Menu::create(rightMenuItem, buyAndSitMenuItem, menuCloseBox, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu);

	return true;
}

//�򹴲˵��ص�����
void AddChipLayer::btRightMenuCallback(Ref *ref){
	bool isAutoBuy = RoomData::getInstance()->getIsAutoBuy();

	if(isAutoBuy){
		//ȥ����
		this->removeChildByTag(RIGHT_MENU_TAG);

		RoomData::getInstance()->setIsAutoBuy(false);
	}
	else{
		//��
		Sprite* rightSprite = Sprite::create("bt_right_menu.png");
		rightSprite->setTag(RIGHT_MENU_TAG);
		rightSprite->setPosition(visibleSize.width / 2 - 214.5, visibleSize.height / 2 - 60.5 - 24);
		this->addChild(rightSprite);

		RoomData::getInstance()->setIsAutoBuy(true);
	}
}

//�������²˵��ص�����
void AddChipLayer::btBuyAndSitMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	RoomData::getInstance()->setAutoBuyChip(((ControlSlider *)this->getChildByTag(ADD_CHIP_LAYER_SLIDER_TAG))->getValue());

	this->addChipDelegate->onBuyAndSit(this->seatNum);
}

//�رղ˵��ص�����
void AddChipLayer::btCloseMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
	RoomData::getInstance()->setAutoBuyChip(((ControlSlider *)this->getChildByTag(ADD_CHIP_LAYER_SLIDER_TAG))->getValue());

	this->addChipDelegate->onCloseAddChipLayer();
}

//�����������ص�����
void AddChipLayer::valueChanged(Ref* object, Control::EventType type){
	ControlSlider* pSlider = (ControlSlider *)object;
	__String* strValue = __String::createWithFormat("%d", (int)pSlider->getValue());
	((Label *)this->getChildByTag(REAL_BUY_LABEL_TAG))->setString(strValue->getCString());
}

//���ô���
void AddChipLayer::setAddChipDelegate(AddChipDelegate* addChipDelegate){
	this->addChipDelegate = addChipDelegate;
}

//�������¶���
void AddChipLayer::setSeatNum(int seatNum){
	this->seatNum = seatNum;
}