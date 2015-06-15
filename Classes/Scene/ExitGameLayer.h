#ifndef _EXIT_GAME_LAYER_
#define _EXIT_GAME_LAYER_

#include "BasePopupBox.h"
#include "ExitGameDelegate.h"
#include "../GlobalDef/GlobalDef.h"

class ExitGameLayer : public BasePopupBox{	
public:
	CREATE_FUNC(ExitGameLayer);
	virtual bool init();

	void setExitGameDelegate(ExitGameDelegate *exitGameDelegate);

protected:
	void btGoHallCallback(Ref *ref);
	void btGameContinueCallback(Ref *ref);

protected:
	Size visibleSize;
	ExitGameDelegate *exitGameDelegate;
};

#endif