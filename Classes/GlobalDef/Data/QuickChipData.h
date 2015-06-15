//
//  QuickChipData.h
//  dzpkV3.2
//
//  Created by apple on 14-9-17.
//
//

#ifndef dzpkV3_2_QuickChipData_h
#define dzpkV3_2_QuickChipData_h

#include "cocos2d.h"
#include "MallInfoData.h"

USING_NS_CC;

class QuickChipData{
public:
    static QuickChipData* getInstance();
    static void removeInstance();
    
    Vector<MallInfoData*> getMallChipDatas();
    void setMallChipDatas(Vector<MallInfoData*> mallChipDatas);
    
protected:
    QuickChipData();
    ~QuickChipData();
    
protected:
    Vector<MallInfoData*> m_mallChipDatas;
};

#endif
