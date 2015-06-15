#include "CommunicateLayer.h"
#include "../GlobalDef/GlobalDef.h"

bool CommunicateLayer::init(){
	if(!Layer::init()){
		return false;
	}

    tableRect.origin.x = 23;
    tableRect.origin.y = 71;
    tableRect.size.width = 319;
    tableRect.size.height = 338;
    
	this->setContentSize(Size(319, 338));

	visibleSize = Director::getInstance()->getVisibleSize();
	communicateList = GlobalDef::getInstance()->getCommunicateList();

	//文字背景
	Sprite* communicate_up_background = Sprite::create("communicate_bg.png");
    communicate_up_background->setAnchorPoint(Vec2(0, 0));
	communicate_up_background->setPosition(23, 71);
	this->addChild(communicate_up_background);

	// tableview
	TableView *tableView = TableView::create(this, Size(316, 270));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tableView->setAnchorPoint(Vec2(0, 0));
	tableView->setPosition(24.5, 136);
	tableView->setDelegate(this);
	this->addChild(tableView);
    
    //shu ru kuang
//    auto communicate_input = Sprite::create("communicate_input.png");
    auto communicate_input = MenuItemImage::create("communicate_input_up.png", "communicate_input_down.png", CC_CALLBACK_1(CommunicateLayer::btnCommunicateInputMenuCallback, this));
    communicate_input->setAnchorPoint(Vec2(0, 1));
    communicate_input->setPosition(32.5, 135);
    
    auto communicate_input_menu = Menu::create(communicate_input, NULL);
    communicate_input_menu->setPosition(0, 0);
    this->addChild(communicate_input_menu);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* t, Event* e){
        log("onTouchBegan");
        
        return true;
    };
    listener->onTouchMoved = [=](Touch* t, Event* e){
        
    };
    listener->onTouchEnded = [=](Touch* t, Event* e){
        //dian ji chu li
        this->removeFromParent();
    };
    listener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->setPriority(listener, -128);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

Size CommunicateLayer::cellSizeForTable(TableView *table){
	return Size(316, 54);
}

TableViewCell* CommunicateLayer::tableCellAtIndex(TableView *table, ssize_t idx){
	TableViewCell *cell = new TableViewCell();
	cell->setContentSize(Size(316, 54));

	//背景
	Sprite* cell_bg_up = Sprite::create("cell_bg_up.png");
	cell_bg_up->setAnchorPoint(Vec2(0, 0));
	cell_bg_up->setPosition(0, 0);
	cell->addChild(cell_bg_up);

	std::string content = this->communicateList.at(idx);
	Label* contentLabel = Label::createWithSystemFont(content.c_str(), "Arial", 27);
	contentLabel->setContentSize(Size(286, 51));
	contentLabel->setScale(0.8f);
	contentLabel->setAnchorPoint(Vec2(0, 0));
	contentLabel->setPosition(12, 12);
	cell->addChild(contentLabel, 1);

	cell->setTag(idx);

	return cell;
}

ssize_t CommunicateLayer::numberOfCellsInTableView(TableView *table){
	return this->communicateList.size();
}

void CommunicateLayer::tableCellTouched(TableView* table, TableViewCell* cell){
	//调用代理方法
	this->communicateDelegate->onCommunicateCellTouched(cell->getTag() + 1);
}

void CommunicateLayer::tableCellHighlight(TableView* table, TableViewCell* cell){
	//显示选中背景
	Sprite* cell_bg_down = Sprite::create("cell_bg_down.png");
	cell_bg_down->setTag(CELL_BG_DOWN_TAG);
	cell_bg_down->setAnchorPoint(Vec2(0, 0));
	cell_bg_down->setPosition(0, 0);
	cell->addChild(cell_bg_down);
}

void CommunicateLayer::tableCellUnhighlight(TableView* table, TableViewCell* cell){
	//去掉选中背景
	cell->removeChildByTag(CELL_BG_DOWN_TAG);
}

void CommunicateLayer::setCommunicateLayerDelegate(CommunicateLayerDelegate* communicateDelegate){
	this->communicateDelegate = communicateDelegate;
}

void CommunicateLayer::btnCommunicateInputMenuCallback(Ref *ref){
    this->communicateDelegate->onCommunicateInputTouchedCallback();
}