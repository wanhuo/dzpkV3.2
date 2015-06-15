#ifndef _ADD_CHIP_DELEGATE_
#define _ADD_CHIP_DELEGATE_

#include "cocos2d.h"

USING_NS_CC;

class AddChipDelegate{	
public:
	virtual void onBuyAndSit(int seatNum) = 0;			   //购买坐下代理方法
	virtual void onCloseAddChipLayer() = 0;                 //关闭代理方法
};

#endif