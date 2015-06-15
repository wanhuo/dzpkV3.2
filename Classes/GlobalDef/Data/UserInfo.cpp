#include "UserInfo.h"

static UserInfo *userInfo = nullptr;

UserInfo::UserInfo(){
	this->userId = "";
	this->userAccount = "";
	this->sessionId = "";
	this->personalSign = "";
	this->nikeName = "";
	this->sex = "";
	this->age = "";
	this->birthday = "";
	this->isYellowVip = "";
	this->onlineStatus = "";
	this->lastIpAddress = "";
	this->lastOnlineTime = "";
	this->realName = "";
	this->idCardNum = "";
	this->cellPhoneNum = "";
	this->email = "";
	this->country = "";
	this->province = "";
	this->city = "";
	this->area = "";
	this->dzpkLevel = "";
	this->dzpkExperience = "";
	this->vipLevel = "";
	this->vipPath = "";
	this->coinAccount = "";
	this->chipAccount = "";
	this->bestWinCard = "";
	this->rankName = "";
	this->totalTime = "";
	this->winPer = "";
	this->bigestWinChip = "";
	this->bigestChip = "";
	this->friendCount = "";
	this->countDownNo = 0;
	this->cdRestTime = 0;
}

UserInfo* UserInfo::getInstance(){
	if(userInfo == NULL){
		userInfo = new UserInfo();
	}
	return userInfo;
}

void UserInfo::removeInstance(){
    if(userInfo != NULL){
        delete userInfo;
    }
    userInfo = nullptr;
}