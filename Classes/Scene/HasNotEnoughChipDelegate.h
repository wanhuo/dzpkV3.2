#ifndef _HAS_NOT_ENOUGH_CHIP_DELEGATE_
#define _HAS_NOT_ENOUGH_CHIP_DELEGATE_

class HasNotEnoughChipDelegate{
public:
	virtual void onCloseHasNotEnoughChipLayer() = 0;  //�رճ��벻��Ի���
	virtual void onHasNotEnoughChip() = 0;			  //���벻�㣬����ǰ��������
};

#endif