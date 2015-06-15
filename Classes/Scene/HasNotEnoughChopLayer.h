//
//  HasNotEnoughChopLayer.h
//  dzpkV3.2
//
//  Created by apple on 14-10-9.
//
//

#ifndef dzpkV3_2_HasNotEnoughChopLayer_h
#define dzpkV3_2_HasNotEnoughChopLayer_h

#include "BasePopupBox.h"
#include "HasNotEnoughChopDelegate.h"
#include "../GlobalDef/GlobalDef.h"

class HasNotEnoughChopLayer : public BasePopupBox{
public:
	CREATE_FUNC(HasNotEnoughChopLayer);
	bool init();
    
	void setHasNotEnoughChopDelegate(HasNotEnoughChopDelegate *hasNotEnoughChopDelegate);
    
protected:
	void btGoNowCallback(Ref *ref);
	void btCloseHasNotEnoughChopLayerCallback(Ref *ref);
    
protected:
	Size visibleSize;
    
	HasNotEnoughChopDelegate* hasNotEnoughChopDelegate;
};

#endif
