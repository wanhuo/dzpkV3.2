#ifndef _NoticeDataBash_h_
#define _NoticeDataBash_h_

#include "cocos2d.h"
#include <iostream>
#include "../GlobalDef.h"
#include "../../GlobalDef/JsonWritter.h"

USING_NS_CC;
using namespace std;

class NoticeData : public Ref {
public:
	static Vector<NoticeData*> parseData(const string strData);
    CC_SYNTHESIZE(int, m_Id, m_Id);
	CC_SYNTHESIZE(int, time, time);
    CC_SYNTHESIZE(int, type, type);
	CC_SYNTHESIZE(string, time_str, time_str);
	CC_SYNTHESIZE(string, context, context);

	NoticeData();
	~NoticeData() {};
};

#endif