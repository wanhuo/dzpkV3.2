#ifndef _LoginRewardView_
#define _LoginRewardView_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "BasePopupBox.h"
#include "UserInfo.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LoginRewardView : public BasePopupBox{
public:
	virtual bool init();
	CREATE_FUNC(LoginRewardView);
    
	void btGoBackMenuCallback(Ref *object);
	void onclickRewardBtn(Ref *object);
protected:
	Size visibleSize;
	Vec2 visibleOrigin;
    int redNB;
	float deltaScreenX;
	float deltaScreenY;
    enum{
        
        TAG_LABEL_GOLDNUM=10,
    };
};

#endif