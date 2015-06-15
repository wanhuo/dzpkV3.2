#ifndef _LOAD_LAYER_
#define _LOAD_LAYER_

#include "cocos2d.h"

USING_NS_CC;

class LoadLayer : public LayerColor{
public:
	virtual bool init();
	CREATE_FUNC(LoadLayer);
};

#endif