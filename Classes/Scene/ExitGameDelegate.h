#ifndef _EXIT_GAME_DELEGATE_
#define _EXIT_GAME_DELEGATE_

class ExitGameDelegate{	
public:
	virtual void onGoHall() = 0;			//返回大厅代理方法
	virtual void onGameContinue() = 0;      //继续游戏代理方法
};

#endif