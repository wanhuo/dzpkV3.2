#ifndef _EXIT_GAME_DELEGATE_
#define _EXIT_GAME_DELEGATE_

class ExitGameDelegate{	
public:
	virtual void onGoHall() = 0;			//���ش���������
	virtual void onGameContinue() = 0;      //������Ϸ������
};

#endif