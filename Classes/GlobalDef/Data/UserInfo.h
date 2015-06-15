#ifndef _USER_INFO_
#define _USER_INFO_

#include <iostream>
#include "cocos2d.h"
#include "../GlobalDef.h"

USING_NS_CC;

class UserInfo : public Ref{
public:
	static UserInfo* getInstance();  //�����û���ȫ�֣�ֻ��һ��
    static void removeInstance();

protected:
	UserInfo();  

public:
	CC_SYNTHESIZE(std::string, userId, UserId);				 //�û�ID
	CC_SYNTHESIZE(std::string, userAccount, UserAccount);		 //�û��˺�
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
	CC_SYNTHESIZE(std::string, idCardNum, IdCardNum);						//���֤����
	CC_SYNTHESIZE(std::string, cellPhoneNum, CellPhoneNum);				    //�ֻ�����		 
	CC_SYNTHESIZE(std::string, email, Email);
	CC_SYNTHESIZE(std::string, country, Country);
	CC_SYNTHESIZE(std::string, province, Province);
	CC_SYNTHESIZE(std::string, city, City);
	CC_SYNTHESIZE(std::string, area, Area);
	CC_SYNTHESIZE(std::string, dzpkLevel, DzpkLevel);			 //��ҵȼ�
	CC_SYNTHESIZE(std::string, dzpkExperience, DzpkExperience);		 //��Ҿ���ֵ
	CC_SYNTHESIZE(std::string, vipLevel, VipLevel);			 //��Ա�ȼ�
	CC_SYNTHESIZE(std::string, vipPath, VipPath);
	CC_SYNTHESIZE(std::string, coinAccount, CoinAccount);		 //����ҳ���
	CC_SYNTHESIZE(std::string, chipAccount, ChipAccount);		 //�˻�����
	CC_SYNTHESIZE(std::string, bestWinCard, BestWinCard);		 //��ţ����
	CC_SYNTHESIZE(std::string, rankName, RankName);			 //ͷ������
    CC_SYNTHESIZE(std::string, headPhotoSerial, HeadPhotoSerial);
	CC_SYNTHESIZE(std::string, totalTime, TotalTime);           //�ܳ���
	CC_SYNTHESIZE(std::string, winPer, WinPer);				 //ʤ��
	CC_SYNTHESIZE(std::string, bigestWinChip, BigestWinChip);		 //�������Ӯ�ó���
	CC_SYNTHESIZE(std::string, bigestChip, BigestChip);		 //��߳����� 
	CC_SYNTHESIZE(std::string, friendCount, FriendCount);
	CC_SYNTHESIZE(int, countDownNo, CountDownNo);						 //����ʱ������ 1~7
	CC_SYNTHESIZE(int, cdRestTime, CdRestTime);						     //��ǰ����ʱ������ȡʣ��ʱ��
};

#endif