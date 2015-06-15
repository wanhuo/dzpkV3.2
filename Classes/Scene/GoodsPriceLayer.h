#ifndef _GOODS_PRICE_LAYER_
#define _GOODS_PRICE_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"

USING_NS_CC;

class GoodsPriceLayer : public Layer{
public:
	virtual bool init();
	static GoodsPriceLayer* create(int price, bool bMoney = true);

private:
	int m_nPrice;
	bool m_bMoney;
};

#endif