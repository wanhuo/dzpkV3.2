#ifndef _ADD_CHIP_DELEGATE_
#define _ADD_CHIP_DELEGATE_

#include "cocos2d.h"

USING_NS_CC;

class AddChipDelegate{	
public:
	virtual void onBuyAndSit(int seatNum) = 0;			   //�������´�����
	virtual void onCloseAddChipLayer() = 0;                 //�رմ�����
};

#endif