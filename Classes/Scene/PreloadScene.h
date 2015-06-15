#ifndef _PRELOAD_SCENE_
#define _PRELOAD_SCENE_

#include "cocos2d.h"
#include "../Utils/DBUtil.h"

USING_NS_CC;

class PreloadScene : public Layer{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(PreloadScene);

protected:
	Size visibleSize;
	Vec2 visibleOrigin;
};

#endif