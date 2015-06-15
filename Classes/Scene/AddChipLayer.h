#ifndef _ADD_CHIP_LAYER_
#define _ADD_CHIP_LAYER_

#include "BasePopupBox.h"
#include "AddChipDelegate.h"
#include "cocos-ext.h"
#include "../GlobalDef/Data/RoomData.h"

USING_NS_CC_EXT;

#define REAL_BUY_LABEL_TAG 200
#define RIGHT_MENU_TAG 201
#define ADD_CHIP_LAYER_SLIDER_TAG 202

class AddChipLayer : public BasePopupBox{
public:
	CREATE_FUNC(AddChipLayer);
	virtual bool init();

	void setAddChipDelegate(AddChipDelegate* addChipDelegate);

	void setSeatNum(int seatNum);

protected:
	void btRightMenuCallback(Ref *ref);
	void btBuyAndSitMenuCallback(Ref *ref);
	void btCloseMenuCallback(Ref *ref);

	void valueChanged(Ref* object, Control::EventType type);

protected:
	Size visibleSize;

	int seatNum;
	AddChipDelegate* addChipDelegate;
};

#endif