#ifndef _STAND_UP_DELEGATE_
#define _STAND_UP_DELEGATE_

class StandupDelegate{	
public:
	virtual void onStandupLayerStandup() = 0;			//վ��˵�������
	virtual void onStandupLayerCloseLayer() = 0;		//������Ϸ���رղ˵�������
};

#endif