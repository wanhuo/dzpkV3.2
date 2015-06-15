#ifndef _USER_INFO_
#define _USER_INFO_

#include <iostream>
#include "cocos2d.h"
#include "../GlobalDef.h"

USING_NS_CC;

class UserInfo : public Ref{
public:
	static UserInfo* getInstance();  //本机用户，全局，只有一个
    static void removeInstance();

protected:
	UserInfo();  

public:
	CC_SYNTHESIZE(std::string, userId, UserId);				 //用户ID
	CC_SYNTHESIZE(std::string, userAccount, UserAccount);		 //用户账号
	CC_SYNTHESIZE(std::string, sessionId, SessionId);
	CC_SYNTHESIZE(std::string, personalSign, PersonalSign);
	CC_SYNTHESIZE(std::string, nikeName, NikeName);
	CC_SYNTHESIZE(std::string, sex, Sex);
	CC_SYNTHESIZE(std::string, age, Age);
	CC_SYNTHESIZE(std::string, birthday, Birthday);
	CC_SYNTHESIZE(std::string, isYellowVip, IsYellowVip);
	CC_SYNTHESIZE(std::string, onlineStatus, OnlineStatus);
	CC_SYNTHESIZE(std::string, lastIpAddress, LastIpAddress);
	CC_SYNTHESIZE(std::string, lastOnlineTime, LastOnlineTime);
	CC_SYNTHESIZE(std::string, realName, RealName);
	CC_SYNTHESIZE(std::string, idCardNum, IdCardNum);						//身份证号码
	CC_SYNTHESIZE(std::string, cellPhoneNum, CellPhoneNum);				    //手机号码		 
	CC_SYNTHESIZE(std::string, email, Email);
	CC_SYNTHESIZE(std::string, country, Country);
	CC_SYNTHESIZE(std::string, province, Province);
	CC_SYNTHESIZE(std::string, city, City);
	CC_SYNTHESIZE(std::string, area, Area);
	CC_SYNTHESIZE(std::string, dzpkLevel, DzpkLevel);			 //玩家等级
	CC_SYNTHESIZE(std::string, dzpkExperience, DzpkExperience);		 //玩家经验值
	CC_SYNTHESIZE(std::string, vipLevel, VipLevel);			 //会员等级
	CC_SYNTHESIZE(std::string, vipPath, VipPath);
	CC_SYNTHESIZE(std::string, coinAccount, CoinAccount);		 //虚拟币筹码
	CC_SYNTHESIZE(std::string, chipAccount, ChipAccount);		 //账户筹码
	CC_SYNTHESIZE(std::string, bestWinCard, BestWinCard);		 //最牛牌型
	CC_SYNTHESIZE(std::string, rankName, RankName);			 //头衔名称
    CC_SYNTHESIZE(std::string, headPhotoSerial, HeadPhotoSerial);
	CC_SYNTHESIZE(std::string, totalTime, TotalTime);           //总场数
	CC_SYNTHESIZE(std::string, winPer, WinPer);				 //胜率
	CC_SYNTHESIZE(std::string, bigestWinChip, BigestWinChip);		 //单次最高赢得筹码
	CC_SYNTHESIZE(std::string, bigestChip, BigestChip);		 //最高筹码数 
	CC_SYNTHESIZE(std::string, friendCount, FriendCount);
	CC_SYNTHESIZE(int, countDownNo, CountDownNo);						 //倒计时宝箱编号 1~7
	CC_SYNTHESIZE(int, cdRestTime, CdRestTime);						     //当前倒计时宝箱领取剩余时间
};

#endif