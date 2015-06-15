#include "GoodsPriceLayer.h"
#include "../GlobalDef/GlobalChineseXml.h"

GoodsPriceLayer* GoodsPriceLayer::create(int price, bool bMoney /* = true */)
{
	GoodsPriceLayer* pRet = new GoodsPriceLayer();
	if (pRet && (pRet->m_bMoney = bMoney, pRet->m_nPrice = price) && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool GoodsPriceLayer::init(){
	if(!Layer::init()){
		return false;
	}

//	Size visibleSize = Director::getInstance()->getVisibleSize();

	char str_price[64];
	memset(str_price, 0, 64);
	int nLen = 0;
	int nWidth = 0;

	if (m_bMoney)
	{
		sprintf(str_price, "%d", m_nPrice);
		nLen = strlen(str_price);
		nWidth = nLen * 15 + 50;
		
		Sprite* sell = Sprite::create("mall_info_sell.png");
		sell->setAnchorPoint(Vec2(0, 0));
		sell->setPosition((140-nWidth)/2, 16);
		this->addChild(sell);

		for (int i=0; i<nLen; i++)
		{
			char digitsFile[64];
			sprintf(digitsFile, "mall_info_%c.png", str_price[i]);
			Sprite* digits = Sprite::create(digitsFile);
			digits->setAnchorPoint(Vec2(0, 0));
			digits->setPosition((140-nWidth)/2 + 27 + i*15, 17);
			this->addChild(digits);
		}

		Sprite* yuan = Sprite::create("mall_info_yuan.png");
		yuan->setAnchorPoint(Vec2(0, 0));
		yuan->setPosition((140-nWidth)/2 + 27 + nLen*15, 16);
		this->addChild(yuan);
	}
	else
	{
		bool bHasKilo = true;
		if (m_nPrice >= 1000)
		{
			sprintf(str_price, "%d", m_nPrice / 1000);
		}
		else
		{
			bHasKilo = false;
			sprintf(str_price, "%d", m_nPrice);
		}
		nLen = strlen(str_price);
		if (bHasKilo)
		{
			nWidth = nLen * 15 + 50;
		}
		else
		{
			nWidth = nLen * 15 + 30;
		}

		Sprite* chip = Sprite::create("mall_info_chip.png");
		chip->setAnchorPoint(Vec2(0, 0));
		chip->setPosition((140-nWidth)/2, 20);
		this->addChild(chip);

		for (int i=0; i<nLen; i++)
		{
			char digitsFile[64];
			sprintf(digitsFile, "mall_info_%c.png", str_price[i]);
			Sprite* digits = Sprite::create(digitsFile);
			digits->setAnchorPoint(Vec2(0, 0));
			digits->setPosition((140-nWidth)/2 + 25 + i*15, 19);
			this->addChild(digits);
		}

		if (bHasKilo)
		{
			Sprite* kilo = Sprite::create("mall_info_kilo.png");
			kilo->setAnchorPoint(Vec2(0, 0));
			kilo->setPosition((140-nWidth)/2 + 25 + nLen*15, 18);
			this->addChild(kilo);
		}
	}

	return true;
}