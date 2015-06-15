#include "RankListData.h"

RankListData::RankListData(){
	chipAccount = 0;
	dzpkLevel = 0;
	headPhotoSerial = "";
	nickName = "";
}

Vector<RankListData*> RankListData::parseData(const std::string strData){
	Vector<RankListData*> rankListDatas;
	rapidjson::Document docRankList;
	docRankList.Parse<0>(strData.c_str());

	rapidjson::Value& resp = docRankList["resp"];
	rapidjson::Value& rankList = resp["list"];
	if (rankList.IsArray())
	{
		for (int i=0; i< rankList.Size(); i++)
		{
			rapidjson::Value& rankData = rankList[i];
			RankListData* data = new RankListData();
			data->setChipAccount(rankData["chip_account"].GetInt());
			data->setDzpkLevel(rankData["dzpk_level"].GetInt());
			data->setHeadPhotoSerial(rankData["head_photo_serial"].GetString());
			data->setNickName(rankData["nick_name"].GetString());
			
			rankListDatas.pushBack(data);
		}
	}

	return rankListDatas;
}
