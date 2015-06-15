#ifndef _MALL_BUY_DATA_
#define _MALL_BUY_DATA_

#include "cocos2d.h"
#include <iostream>
#include "../GlobalDef.h"
#include "../../GlobalDef/JsonWritter.h"

USING_NS_CC;
using namespace std;

class MallBuyData : public Ref {
public:
	static MallBuyData parseData(const string strData);

	CC_SYNTHESIZE(int, handselchip, HandSelChip);
	CC_SYNTHESIZE(string, result, ResultString);
	CC_SYNTHESIZE(string, usechip, UseChip);
	
	MallBuyData(); 
	MallBuyData(const MallBuyData& data);
	~MallBuyData() {};
};

#endif