#ifndef _COMMUNICATE_LAYER_DELEGATE_
#define _COMMUNICATE_LAYER_DELEGATE_

class CommunicateLayerDelegate{
public:
	virtual void onCommunicateCellTouched(int cellIndex) = 0;    //���ﵥԪѡ�д�������cellIndexΪ��ѡ�е�Ԫ��tag��1������1��ʼ
    virtual void onCommunicateInputTouchedCallback() = 0;        //shu ru kuang bei dian ji hou de hui diao fang fa
};

#endif