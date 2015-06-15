//
//  NetConnectErrorLayer.h
//  dzpkV3.2
//
//  Created by apple on 14-9-24.
//
//

#ifndef dzpkV3_2_NetConnectErrorLayer_h
#define dzpkV3_2_NetConnectErrorLayer_h

#include "BasePopupBox.h"
#include "GameHallScene.h"

class NetConnectErrorLayer : public BasePopupBox{
public:
    CREATE_FUNC(NetConnectErrorLayer);
    virtual bool init();
    
    void btGoHallCallback(Ref *ref);
    
protected:
    Size visibleSize;
};

#endif
