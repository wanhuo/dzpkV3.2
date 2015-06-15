//
//  CommunicateInputLayer.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-12.
//
//

#include "CommunicateInputLayer.h"

bool CommunicateInputLayer::init(){
    if(!LayerColor::initWithColor(Color4B(0, 0, 0, 120))){
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    //chuang jian listview
    listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setSize(Size(visibleSize.width, visibleSize.height - 72));
    listView->setAnchorPoint(Vec2(0, 1));
    listView->setPosition(Vec2(0, visibleSize.height));
    this->addChild(listView);
    
    auto msgList = CommunicateData::getInstance()->getMsgList();
    if(msgList.size() > 0){
        for(int i = 0; i < msgList.size(); i++){
            auto msg = CommunicateData::getInstance()->getMsgAt(i);
            showText(msg);
        }
    }
    
    auto communicate_input_edit_bg = Sprite::create("communicate_input_edit_bg.png");
    communicate_input_edit_bg->setAnchorPoint(Vec2(0, 0));
    communicate_input_edit_bg->setPosition(Vec2(0, 0));
    this->addChild(communicate_input_edit_bg);
    
    editBox = EditBox::create(Size(visibleSize.width - 180, 60), Scale9Sprite::create("communicate_input_editbox.png"));
    editBox->setMaxLength(40);
    editBox->setDelegate(this);
    editBox->setAnchorPoint(Vec2(0, 0));
    editBox->setPosition(Vec2(10, 6));
    this->addChild(editBox);
    editBox->setInputMode(EditBox::InputMode::ANY);
    editBox->setReturnType(EditBox::KeyboardReturnType::DONE);
    
    editBox->setText(CommunicateData::getInstance()->getInputStr().c_str());
    
    //close menu
    auto close_menu = MenuItemImage::create("communicate_close_menu_up.png", "communicate_close_menu_down.png", CC_CALLBACK_1(CommunicateInputLayer::btnCloseMenuCallback, this));
    close_menu->setAnchorPoint(Vec2(1, 1));
    close_menu->setPosition(visibleSize.width, visibleSize.height);
    
    auto send_menu = MenuItemImage::create("communicate_send_up.png", "communicate_send_down.png", CC_CALLBACK_1(CommunicateInputLayer::btnSendMenuCallback, this));
    send_menu->setAnchorPoint(Vec2(1, 0));
    send_menu->setPosition(Vec2(visibleSize.width - 10, 7));
    
    auto menu = Menu::create(close_menu, send_menu, NULL);
    menu->setPosition(0, 0);
    this->addChild(menu);
    
    schedule(schedule_selector(CommunicateInputLayer::scheduleScrollListView), 0.3f);
    
    //∆¡±Œ¥•√˛≤„
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* t, Event* e){
		return true;
	};
	listener->setSwallowTouches(true);
    
	Director::getInstance()->getEventDispatcher()->setPriority(listener, -128);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void CommunicateInputLayer::scheduleScrollListView(float dt){
    listView->scrollToBottom(0.3f, true);
}

void CommunicateInputLayer::btnCloseMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    CommunicateData::getInstance()->setInputStr(editBox->getText());
    editBox->detachWithIME();
    this->removeFromParent();
}

void CommunicateInputLayer::btnSendMenuCallback(Ref *ref){
    if(GlobalDef::getInstance()->getSoundOn()){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	}
    
    auto sendText = this->editBox->getText();
    
    if(strcmp(sendText, "") == 0){
        return;
    }
    
    this->inputDelegate->onCommunicateInputSendCallback(1, sendText);
}

void CommunicateInputLayer::showRecvMsg(std::string msg){
    showText(msg);
}

void CommunicateInputLayer::setCommunicateInputDelegate(CommunicateInputDelegate* inputDelegate){
    this->inputDelegate = inputDelegate;
}

/**
 * This method is called when an edit box gains focus after keyboard is shown.
 * @param editBox The edit box object that generated the event.
 * @js NA
 * @lua NA
 */
void CommunicateInputLayer::editBoxEditingDidBegin(EditBox* editBox){
    
}

/**
 * This method is called when an edit box loses focus after keyboard is hidden.
 * @param editBox The edit box object that generated the event.
 * @js NA
 * @lua NA
 */
void CommunicateInputLayer::editBoxEditingDidEnd(EditBox* editBox){
    this->listView->scrollToBottom(0.3f, true);
}

/**
 * This method is called when the edit box text was changed.
 * @param editBox The edit box object that generated the event.
 * @param text The new text.
 * @js NA
 * @lua NA
 */
void CommunicateInputLayer::editBoxTextChanged(EditBox* editBox, const std::string& text){
    
}

/**
 * This method is called when the return button was pressed or the outside area of keyboard was touched.
 * @param editBox The edit box object that generated the event.
 * @js NA
 * @lua NA
 */
void CommunicateInputLayer::editBoxReturn(EditBox* editBox){
    CommunicateData::getInstance()->setInputStr(editBox->getText());
}

void CommunicateInputLayer::showText(std::string text){
    int textSize = text.size();
    
    auto tempLabel = Label::createWithSystemFont("*", "Arial", 27);
    auto singleFontWidth = tempLabel->getContentSize().width;
    
    int singleLineFontNum = visibleSize.width / singleFontWidth;
    
    int richTextWidth = 33;
    richTextWidth = ((textSize / singleLineFontNum) + 1) * 33;
    
    auto rText = ui::RichText::create();
    rText->ignoreContentAdaptWithSize(false);
    rText->setContentSize(Size(visibleSize.width - 70, richTextWidth));
    auto re = ui::RichElementText::create(1, Color3B(255, 255, 255), 255, text, "Arial", 27);
    rText->pushBackElement(re);
    
    this->listView->pushBackCustomItem(rText);
}
