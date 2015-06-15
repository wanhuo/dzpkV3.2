#ifndef _HAS_NOT_ENOUGH_CHIP_DELEGATE_
#define _HAS_NOT_ENOUGH_CHIP_DELEGATE_

class HasNotEnoughChipDelegate{
public:
	virtual void onCloseHasNotEnoughChipLayer() = 0;  //关闭筹码不足对话框
	virtual void onHasNotEnoughChip() = 0;			  //筹码不足，立即前往代理方法
};

#endif