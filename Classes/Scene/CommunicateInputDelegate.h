//
//  CommunicateInputDelegate.h
//  dzpkV3.2
//
//  Created by apple on 14-9-13.
//
//

#ifndef dzpkV3_2_CommunicateInputDelegate_h
#define dzpkV3_2_CommunicateInputDelegate_h

class CommunicateInputDelegate{
public:
    virtual void onCommunicateInputSendCallback(int type, std::string msg) = 0;          //点击发送菜单时的回调方法
};

#endif