#ifndef _COMMUNICATE_LAYER_DELEGATE_
#define _COMMUNICATE_LAYER_DELEGATE_

class CommunicateLayerDelegate{
public:
	virtual void onCommunicateCellTouched(int cellIndex) = 0;    //短语单元选中代理方法，cellIndex为被选中单元的tag加1，即从1开始
    virtual void onCommunicateInputTouchedCallback() = 0;        //shu ru kuang bei dian ji hou de hui diao fang fa
};

#endif