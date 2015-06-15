//
//  QuickChipData.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-17.
//
//

#include "QuickChipData.h"

static QuickChipData* instance = nullptr;

QuickChipData* QuickChipData::getInstance(){
    if (instance == NULL) {
        instance = new QuickChipData();
    }
    
    return instance;
}

void QuickChipData::removeInstance(){
    delete instance;
}

QuickChipData::QuickChipData(){
    m_mallChipDatas.clear();
}

QuickChipData::~QuickChipData(){
    m_mallChipDatas.clear();
}

Vector<MallInfoData*> QuickChipData::getMallChipDatas(){
    return this->m_mallChipDatas;
}

void QuickChipData::setMallChipDatas(Vector<MallInfoData*> mallChipDatas){
    this->m_mallChipDatas = mallChipDatas;
}