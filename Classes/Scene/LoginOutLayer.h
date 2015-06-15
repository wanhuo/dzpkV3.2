//
//  LoginOutLayer.h
//  dzpkV3.2
//
//  Created by apple on 14-9-11.
//
//

#ifndef _LOGIN_OUT_LAYER_
#define _LOGIN_OUT_LAYER_

#include "BasePopupBox.h"

class LoginOutLayer : public BasePopupBox{
public:
    CREATE_FUNC(LoginOutLayer);
    virtual bool init();
    
    void btOKCallback(Ref *ref);
    void btCancelCallback(Ref *ref);
    
protected:
    Size visibleSize;
};

#endif
