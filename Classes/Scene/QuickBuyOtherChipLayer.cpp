//
//  QuickBuyOtherChipLayer.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-17.
//
//

#include "QuickBuyOtherChipLayer.h"

bool QuickBuyOtherChipLayer::init(){
    if(!BasePopupBox::init()){
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    infoDatas = QuickChipData::getInstance()->getMallChipDatas();
    
    auto strChipAccount = UserInfo::getInstance()->getChipAccount();
    auto chipAccount = atol(strChipAccount.c_str());
    auto minBuyChip = RoomData::getInstance()->getRoomInfo()->getMinBuyChip();
    for (int i = 10; i < infoDatas.size(); i++) {
        auto giveChip = atol(infoDatas.at(i)->getmallGiveChip().c_str());
        if(giveChip > (minBuyChip - chipAccount)){
            for(int j = i; j > 10; j--){
                infoDatas.swap(j, j - 1);
            }
            break;
        }
    }
    
    auto quick_buy_other_bg = Sprite::create("quick_buy_other_bg.png");
    quick_buy_other_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(quick_buy_other_bg);
    
    chipAccountLabel = Label::createWithSystemFont(strChipAccount.c_str(), "Arial", 33);
    chipAccountLabel->setAnchorPoint(Vec2(0, 0));
    chipAccountLabel->setPosition(Vec2(visibleSize.width / 2 - 130, visibleSize.height / 2 + 189));
    this->addChild(chipAccountLabel);
    
    tableView = TableView::create(this, Size(772, 396));
    tableView->setAnchorPoint(Vec2(0, 0));
    tableView->setPosition(visibleSize.width / 2 - 386, visibleSize.height / 2 - 230);

    tableView->setDirection(ScrollView::Direction::VERTICAL);
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setDelegate(this);
    this->addChild(tableView);
    
    auto refreshMenu = MenuItemImage::create("quick_buy_other_refresh_up.png", "quick_buy_other_refresh_down.png", CC_CALLBACK_1(QuickBuyOtherChipLayer::btnRefreshMenuCallback, this));
    refreshMenu->setPosition(Vec2(visibleSize.width / 2 - 350, visibleSize.height / 2 + 206));
    
    auto buyChipCloseMenu = MenuItemImage::create("btn_close_up.png", "btn_close_down.png", CC_CALLBACK_1(QuickBuyOtherChipLayer::btnCloseMenuCallback, this));
    buyChipCloseMenu->setPosition(Vec2(visibleSize.width / 2 + 392, visibleSize.height / 2 + 243));
    
    auto menu = Menu::create(refreshMenu, buyChipCloseMenu, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu);
    
    return true;
}

void QuickBuyOtherChipLayer::btnCloseMenuCallback(Ref *ref){
    this->removeFromParent();
}

void QuickBuyOtherChipLayer::btnRefreshMenuCallback(Ref *ref){
    auto chipAccount = UserInfo::getInstance()->getChipAccount();
    chipAccountLabel->setString(chipAccount.c_str());
}

void QuickBuyOtherChipLayer::btnQuickBuyMenuCallback(Ref *ref){
    auto infoData = infoDatas.at(((MenuItemImage *)ref)->getTag());
    
    std::string strValue1 = "";
	std::string strValue2 = "";
    strValue1 = GlobalChineseXml::getInstance()->getValue("mall_buy_chip") + infoData->getmallGiveChip();
    strValue1 += GlobalChineseXml::getInstance()->getValue("mall_chip");
    strValue2 = GlobalChineseXml::getInstance()->getValue("mall_buy_sell") + infoData->getmallFirstChip();
    strValue2 += GlobalChineseXml::getInstance()->getValue("mall_yuan");
    MallBuyInfoLayer* infoLayer = MallBuyInfoLayer::create(MALL_BUY_INFO_CHIP, strValue1, strValue2);

    infoLayer->setData(infoData);
    infoLayer->setPosition(0, 0);
    this->addChild(infoLayer);
}

Size QuickBuyOtherChipLayer::cellSizeForTable(TableView *table){
    return Size(772, 99);
}

TableViewCell* QuickBuyOtherChipLayer::tableCellAtIndex(TableView *table, ssize_t idx){
    auto cell = TableViewCell::create();
    cell->setContentSize(Size(772, 99));
    
    auto quick_buy_other_cell_bg = Sprite::create("quick_buy_other_cell_bg.png");
    quick_buy_other_cell_bg->setAnchorPoint(Vec2(0, 0));
    quick_buy_other_cell_bg->setPosition(Vec2(0, 0));
    cell->addChild(quick_buy_other_cell_bg);
    
    auto giveChip = infoDatas.at(10 + idx)->getmallGiveChip();
    auto price = infoDatas.at(10 + idx)->getmallFirstChip();
    
    auto chipText = GlobalChineseXml::getInstance()->getValue("mall_chip");
    auto strGiveChip = __String::create(giveChip);
    strGiveChip->append(chipText);
    auto chipLabel = Label::createWithSystemFont(strGiveChip->getCString(), "Arial", 27);
    chipLabel->setColor(Color3B(0, 0, 0));
    chipLabel->setAnchorPoint(Vec2(0, 0.5));
    chipLabel->setPosition(Vec2(135, 49.5));
    cell->addChild(chipLabel);

    auto quick_buy_cell = GlobalChineseXml::getInstance()->getValue("quick_buy_cell");
    auto yuanText = GlobalChineseXml::getInstance()->getValue("mall_yuan");
    auto strPrice = __String::create(quick_buy_cell);
    strPrice->append(price);
    strPrice->append(yuanText);
    auto priceLabel = Label::createWithSystemFont(strPrice->getCString(), "Arial", 27);
    priceLabel->setColor(Color3B(206, 111, 29));
    priceLabel->setAnchorPoint(Vec2(0, 0.5));
    priceLabel->setPosition(Vec2(330, 49.5));
    cell->addChild(priceLabel);
    
    auto quickBuyMenu = MenuItemImage::create("quick_buy_other_cell_up.png", "quick_buy_other_cell_down.png", CC_CALLBACK_1(QuickBuyOtherChipLayer::btnQuickBuyMenuCallback, this));
    quickBuyMenu->setPosition(Vec2(650, 49.5));
    quickBuyMenu->setTag(10 + idx);
    
    auto menu = Menu::create(quickBuyMenu, NULL);
    menu->setPosition(0, 0);
    cell->addChild(menu);
    
    return cell;
}

ssize_t QuickBuyOtherChipLayer::numberOfCellsInTableView(TableView *table){
    return infoDatas.size() - 10;
}

void QuickBuyOtherChipLayer::tableCellTouched(TableView* table, TableViewCell* cell){
    
}