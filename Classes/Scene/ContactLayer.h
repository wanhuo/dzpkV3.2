#ifndef _ContactLayer_
#define _ContactLayer_


#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "BasePopupBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ContactLayer : public BasePopupBox{
public:
	virtual bool init();

	virtual void onEnter();
	CREATE_FUNC(ContactLayer);
    void btGoBackMenuCallback(Ref *object);

	void initView();//初始化页面固定UI

	void MainView();//主要内容UI

protected:
	Size visibleSize;
	Vec2 visibleOrigin;
	float deltaScreenX;
	float deltaScreenY;
};

#endif