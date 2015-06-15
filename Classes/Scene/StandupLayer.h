#ifndef _STAND_UP_LAYER_
#define _STAND_UP_LAYER_

#include "BasePopupBox.h"
#include "StandupDelegate.h"
#include "../GlobalDef/GlobalDef.h"

class StandupLayer : public BasePopupBox{
public:
	CREATE_FUNC(StandupLayer);
	virtual bool init();

	void setStandupDelegate(StandupDelegate* standupDelegate);

protected:
	void btStandupCallback(Ref *ref);
	void btGameContinueCallback(Ref *ref);

protected:
	Size visibleSize;

	StandupDelegate* standupDelegate;
};

#endif