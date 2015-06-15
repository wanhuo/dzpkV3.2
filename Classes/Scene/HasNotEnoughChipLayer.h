#ifndef _HAS_NOT_ENOUGH_CHIP_LAYER_
#define _HAS_NOT_ENOUGH_CHIP_LAYER_

#include "BasePopupBox.h"
#include "HasNotEnoughChipDelegate.h"
#include "../GlobalDef/GlobalDef.h"

class HasNotEnoughChipLayer : public BasePopupBox{	
public:
	CREATE_FUNC(HasNotEnoughChipLayer);
	bool init();

	void setHasNotEnoughChipDelegate(HasNotEnoughChipDelegate *hasNotEnoughChipDelegate);

protected:
	void btGoNowCallback(Ref *ref);
	void btCloseHasNotEnoughChipLayerCallback(Ref *ref);

protected:
	Size visibleSize;

	HasNotEnoughChipDelegate* hasNotEnoughChipDelegate;
};

#endif