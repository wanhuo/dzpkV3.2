#ifndef _RESOURCE_PRELOAD_
#define _RESOURCE_PRELOAD_

#include "cocos2d.h"

USING_NS_CC;

class ResourcePreload{
public:
	static ResourcePreload* getInstance();

	void preload();

	void addSpriteFrame(SpriteFrame *spriteFrame, char *key);

protected:
	ResourcePreload();
	~ResourcePreload();

	void preloadSpriteFrame();
	void preloadAudio();
};

#endif