#include "MallBuyData.h"


MallBuyData::MallBuyData()
{
	handselchip = 0;
	result = "";
	usechip = "";
}

MallBuyData::MallBuyData(const MallBuyData& data)
{
	handselchip = data.handselchip;
	result = data.result;
	usechip = data.usechip;
}

MallBuyData MallBuyData::parseData(const string strData)
{
	MallBuyData MallDatas;
	rapidjson::Document docMallRecv;
	docMallRecv.Parse<0>(strData.c_str());

	rapidjson::Value& resp = docMallRecv["resp"];
	MallDatas.setHandSelChip(resp["handselchip"].GetInt());
	MallDatas.setResultString(resp["result"].GetString());
	MallDatas.setUseChip(resp["usechip"].GetString());
	
	return MallDatas;
}