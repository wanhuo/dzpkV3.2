#ifndef _RANK_LIST_DATA_
#define _RANK_LIST_DATA_

#include "cocos2d.h"
#include <iostream>
#include "../../GlobalDef/JsonWritter.h"

USING_NS_CC;

class RankListData : public Ref{
public:
	static Vector<RankListData*> parseData(const std::string strData);

	CC_SYNTHESIZE(long, chipAccount, ChipAccount);
	CC_SYNTHESIZE(int, dzpkLevel, DzpkLevel);
	CC_SYNTHESIZE(std::string, headPhotoSerial, HeadPhotoSerial);
	CC_SYNTHESIZE(std::string, nickName, NickName);

	RankListData();
	~RankListData() {}
};

#endif