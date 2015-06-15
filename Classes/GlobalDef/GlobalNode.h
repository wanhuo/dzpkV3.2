#ifndef _GLOBAL_NODE_
#define _GLOBAL_NODE_

#include "cocos2d.h"

USING_NS_CC;

class GlobalNode : public Node{
public:
	static GlobalNode* getInstance();

protected:
	GlobalNode();
	~GlobalNode();

public:
	virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags);
};

#endif