//
//  CommunicateData.cpp
//  dzpkV3.2
//
//  Created by apple on 14-9-12.
//
//

#include "CommunicateData.h"

static CommunicateData* communicateData = nullptr;

CommunicateData::CommunicateData(){
    this->inputStr = "";
    this->msgList.clear();
}

CommunicateData::~CommunicateData(){
    this->msgList.clear();
}

CommunicateData* CommunicateData::getInstance(){
    if(communicateData == NULL){
        communicateData = new CommunicateData();
    }
    
    return communicateData;
}

void CommunicateData::removeInstance(){
    if(communicateData != NULL){
        delete communicateData;
    }
    
    communicateData = nullptr;
}

std::vector<std::string> CommunicateData::getMsgList(){
    return this->msgList;
}

void CommunicateData::insertMsg(std::string msg){
    if(this->msgList.size() == 30){
        for (int i = 0; i < 29; i++) {
            this->msgList.at(i) = this->msgList.at(i + 1);
        }
    }
    this->msgList.push_back(msg);
}

std::string CommunicateData::getMsgAt(int index){
    return this->msgList.at(index);
}