#ifndef _BroadCastView_
#define _BroadCastView_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "BasePopupBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

class BroadCastView : public BasePopupBox{
public:
	virtual bool init();
	CREATE_FUNC(BroadCastView);
    
	void btGoBackMenuCallback(Ref *object);
	void onclickUpdateBtn(Ref *object);
protected:
	Size visibleSize;
	Vec2 visibleOrigin;

	float deltaScreenX;
	float deltaScreenY;
};

#endif