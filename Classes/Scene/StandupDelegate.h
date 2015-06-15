#ifndef _STAND_UP_DELEGATE_
#define _STAND_UP_DELEGATE_

class StandupDelegate{	
public:
	virtual void onStandupLayerStandup() = 0;			//站起菜单代理方法
	virtual void onStandupLayerCloseLayer() = 0;		//继续游戏、关闭菜单代理方法
};

#endif