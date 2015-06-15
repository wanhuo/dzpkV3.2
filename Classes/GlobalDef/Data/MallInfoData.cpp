#include "MallInfoData.h"


MallInfoData::MallInfoData()
{
	mall_id = "";
	mall_type = "";
	type_name = "";
	mall_style = "";
	stylename = "";
	mall_name = "";
	mall_path = "";
	mall_firstchip = "";
	mall_secondchip = "";
	mall_usetime = "";
	mall_givechip = "";
	mall_explain = "";
	coin_type = "";
	givename = "";
	mall_delete = "";
}

Vector<MallInfoData*> MallInfoData::parseData(const string strData)
{
	Vector<MallInfoData*> MallDatas;
	rapidjson::Document docMallRecv;
	docMallRecv.Parse<0>(strData.c_str());

	rapidjson::Value& resp = docMallRecv["resp"];
	rapidjson::Value& mallInfos = resp["mallinfo"];
	if (mallInfos.IsArray())
	{
		for (int i=0; i< mallInfos.Size(); i++)
		{
			rapidjson::Value& MallData = mallInfos[i];
			MallInfoData* data = new MallInfoData();
			data->setmallID(MallData["mall_id"].GetString());
			data->setmallType(MallData["mall_type"].GetString());
			data->settypeName(MallData["typename"].GetString());
			data->setmallStyle(MallData["mall_style"].GetString());
			data->setstyleName(MallData["stylename"].GetString());
			data->setmallName(MallData["mall_name"].GetString());
			data->setmallPath(MallData["mall_path"].GetString());
			data->setmallFirstChip(MallData["mall_firstchip"].GetString());
			data->setmallSecondChip(MallData["mall_secondchip"].GetString());
			data->setmallUseTime(MallData["mall_usetime"].GetString());
			data->setmallGiveChip(MallData["mall_givechip"].GetString());
			data->setmallExplain(MallData["mall_explain"].GetString());
			data->setcoinType(MallData["coin_type"].GetString());
			//data->setgiveName(MallData["givename"].GetString());
			data->setmallDelete(MallData["mall_delete"].GetString());
			
			MallDatas.pushBack(data);
		}
	}

	return MallDatas;
}