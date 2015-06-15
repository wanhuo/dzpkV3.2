#ifndef _UPDATE_LAYER_
#define _UPDATE_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "BasePopupBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

class UpdateLayer : public BasePopupBox{
public:
	virtual bool init();
	CREATE_FUNC(UpdateLayer);
    
	void btGoBackMenuCallback(Ref *object);
	void onclickUpdateBtn(Ref *object);
protected:
	Size visibleSize;
	Vec2 visibleOrigin;

	float deltaScreenX;
	float deltaScreenY;
};

#endif