//
//  SmileLayer.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-18.
//
//

#include "SmileLayer.h"

bool SmileLayer::init(){
    if(!Layer::init()){
        return false;
    }
    
    touched = false;
    cellTouchedIndex = 0;
    visibleSize = Director::getInstance()->getVisibleSize();
    
    tableRect.origin.x = 25.5;
    tableRect.origin.y = 90;
    tableRect.size.width = 480;
    tableRect.size.height = 280;
    
    this->setContentSize(Size(484, 304));
    
    auto smile_bg = Sprite::create("smile_bg.png");
    smile_bg->setAnchorPoint(Vec2(0, 0));
    smile_bg->setPosition(Vec2(23, 71));
    this->addChild(smile_bg);
    
    // tableview
	TableView *tableView = TableView::create(this, Size(480, 280));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tableView->setAnchorPoint(Vec2(0, 0));
	tableView->setPosition(25.5, 90);
	tableView->setDelegate(this);
	this->addChild(tableView);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* t, Event* e){
        log("onTouchBegan");
        touched = true;
        
        return true;
    };
    listener->onTouchMoved = [=](Touch* t, Event* e){
        touched = false;
    };
    listener->onTouchEnded = [=](Touch* t, Event* e){
        if(touched){
            touched = false;
            //dian ji chu li
            auto tPosition = Director::getInstance()->convertToGL(t->getLocationInView());
            if(tableRect.containsPoint(tPosition)){
                int index = (int)((tPosition.x - 25.5) / 120) + 1;
                int touchedTag = cellTouchedIndex * 4 + index;
                if(touchedTag <= 20){
                    auto smile = touchedCell->getChildByTag(index);
                    auto scaleBig = ScaleTo::create(0.25, 1.3);
                    auto scaleNormal = ScaleTo::create(0.25, 1.0);
                    auto callFunc = CallFunc::create([=](){
                        btnSmileMenuCallback(touchedTag);
                    });
                    
                    smile->runAction(Sequence::create(scaleBig, scaleNormal, callFunc, NULL));
                }
            }
            else{
                this->removeFromParent();
            }
        }
    };
    listener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->setPriority(listener, -128);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void SmileLayer::btnSmileMenuCallback(int touchedTag){
    log("btnSmileMenuCallback: %d", touchedTag);
    auto strMsg = __String::createWithFormat("%d", touchedTag);
    this->inputDelegate->onCommunicateInputSendCallback(2, strMsg->getCString());
}

void SmileLayer::setCommunicateInputDelegate(CommunicateInputDelegate* inputDelegate){
    this->inputDelegate = inputDelegate;
}

Size SmileLayer::cellSizeForTable(TableView *table){
    return Size(480, 140);
}

TableViewCell* SmileLayer::tableCellAtIndex(TableView *table, ssize_t idx){
    auto cell = TableViewCell::create();
    cell->setContentSize(Size(480, 140));
    cell->setTag((int)idx);
    
/*    if(idx == 2){
        int mod = 9 % 4;
        for(int i = 1; i <= mod; i++){
            int index = (int)(idx * 4 + i);
            auto smilePngName = __String::createWithFormat("smile_%d_1.png", index);
            auto smileSprite = Sprite::create(smilePngName->getCString());
            smileSprite->setTag(i);
            smileSprite->setPosition(Vec2((i - 1) * 120 + 60, 70));
            
            cell->addChild(smileSprite);
        }
    }
    else{*/
        for(int i = 1; i < 5; i++){
            int index = (int)(idx * 4 + i);
            auto smilePngName = __String::createWithFormat("smile_%d_1.png", index);
            auto smileSprite = Sprite::create(smilePngName->getCString());
            smileSprite->setTag(i);
            smileSprite->setPosition(Vec2((i - 1) * 120 + 60, 70));
            
            cell->addChild(smileSprite);
        }
//    }
    
    return cell;
}

ssize_t SmileLayer::numberOfCellsInTableView(TableView *table){
    return 20 / 4;
}

void SmileLayer::tableCellTouched(TableView* table, TableViewCell* cell){
    log("tableCellTouched");
    cellTouchedIndex = cell->getTag();
    touchedCell = cell;
}