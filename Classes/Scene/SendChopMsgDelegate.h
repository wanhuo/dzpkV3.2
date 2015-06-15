//
//  SendChopMsgDelegate.h
//  dzpkV3.2
//
//  Created by apple on 14-10-8.
//
//

#ifndef dzpkV3_2_SendChopMsgDelegate_h
#define dzpkV3_2_SendChopMsgDelegate_h

class SendChopMsgDelegate{
public:
    virtual void onSendChopMsg(int seatNum, int chopIndex) = 0;       //发送道具消息代理方法
};

#endif
