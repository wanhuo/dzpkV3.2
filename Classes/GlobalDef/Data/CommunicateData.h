//
//  CommunicateData.h
//  dzpkV3.2
//
//  Created by apple on 14-9-12.
//
//

#ifndef dzpkV3_2_CommunicateData_h
#define dzpkV3_2_CommunicateData_h

#include "cocos2d.h"
#include <iostream>

USING_NS_CC;

class CommunicateData : public Ref{
public:
    static CommunicateData* getInstance();
    static void removeInstance();
    
    std::vector<std::string> getMsgList();
    void insertMsg(std::string msg);
    std::string getMsgAt(int index);
    
    CC_SYNTHESIZE(std::string, inputStr, InputStr);         //shang ci shu ru de wei fa song de nei rong
    
protected:
    CommunicateData();
    ~CommunicateData();
    
protected:
    std::vector<std::string> msgList;            //yuan shi xiao xi lie biao, you ke neng xiao xi hen chang
};

#endif
