#ifndef _MAIN_SCENE_TOUCH_LAYER_
#define _MAIN_SCENE_TOUCH_LAYER_

#include "cocos2d.h"

USING_NS_CC;

class MainSceneTouchDelegate{	
public:
	virtual void onTouchDelegate(Touch *t) = 0;
};

class MainSceneTouchLayer : public Layer{		
public:
	CREATE_FUNC(MainSceneTouchLayer);

	virtual bool init();

	void setMainSceneTouchDelegate(MainSceneTouchDelegate *touchDelegate);

private:
	MainSceneTouchDelegate *touchDelegate;
};

#endif