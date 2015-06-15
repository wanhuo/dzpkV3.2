#include "GlobalNode.h"
#include "../NetWork/netbean.h"

static GlobalNode* globalNode = nullptr;

GlobalNode* GlobalNode::getInstance(){
	if(globalNode == NULL){
		globalNode = new GlobalNode();
	}

	return globalNode;
}

GlobalNode::GlobalNode(){
	Director::getInstance()->setNotificationNode(this);
	this->onEnter();
}

GlobalNode::~GlobalNode(){
	
}

void GlobalNode::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags){
	NetBean::getInstance()->drive();
}