#ifndef _USER_INFO_TABLE_
#define _USER_INFO_TABLE_

#include "cocos2d.h"
#include <iostream>
#include "../../Utils/TableBase.h"

USING_NS_CC;
using namespace std;

class UserInfoTable : public TableBase {
public:
	CC_SYNTHESIZE(string, user_id, UserId);
	CC_SYNTHESIZE(string, account, Account);
	CC_SYNTHESIZE(string, password, Password);
	CC_SYNTHESIZE(string, sessionid, SessionId);

	virtual int toObject(int n_column, char ** column_value, char ** column_name);

	UserInfoTable(); 
	~UserInfoTable() {};
};

#endif