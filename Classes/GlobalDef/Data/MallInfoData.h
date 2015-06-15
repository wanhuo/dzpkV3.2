#ifndef _MALL_INFO_DATA_
#define _MALL_INFO_DATA_

#include "cocos2d.h"
#include <iostream>
#include "../GlobalDef.h"
#include "../../GlobalDef/JsonWritter.h"

USING_NS_CC;
using namespace std;

class MallInfoData : public Ref {
public:
	static Vector<MallInfoData*> parseData(const string strData);

	CC_SYNTHESIZE(string, mall_id, mallID);
	CC_SYNTHESIZE(string, mall_type, mallType);
	CC_SYNTHESIZE(string, type_name, typeName);
	CC_SYNTHESIZE(string, mall_style, mallStyle);
	CC_SYNTHESIZE(string, stylename, styleName);
	CC_SYNTHESIZE(string, mall_name, mallName);
	CC_SYNTHESIZE(string, mall_path, mallPath);
	CC_SYNTHESIZE(string, mall_firstchip, mallFirstChip);
	CC_SYNTHESIZE(string, mall_secondchip, mallSecondChip);
	CC_SYNTHESIZE(string, mall_usetime, mallUseTime);
	CC_SYNTHESIZE(string, mall_givechip, mallGiveChip);
	CC_SYNTHESIZE(string, mall_explain, mallExplain);
	CC_SYNTHESIZE(string, coin_type, coinType);
	CC_SYNTHESIZE(string, givename, giveName);
	CC_SYNTHESIZE(string, mall_delete, mallDelete);

	MallInfoData(); 
	~MallInfoData() {};
};

#endif