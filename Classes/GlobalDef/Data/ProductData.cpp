#include "ProductData.h"


ProductData::ProductData():type(0),isSelect(0),paperIndex(0)
{
	
}

Vector<ProductData*> ProductData::parseData(const string strData)
{
	Vector<ProductData*> noticeDatas;
	rapidjson::Document docHallRecv;
	docHallRecv.Parse<0>(strData.c_str());
	rapidjson::Value& resp = docHallRecv["resp"]["packageinfo"];
	if (resp.IsArray())
	{
		for (int i=0; i< resp.Size(); i++)
		{
			rapidjson::Value& noticeData = resp[i];
			ProductData* data = new ProductData();
			data->setID(noticeData["id"].GetInt());
			data->setbuy_time(noticeData["buy_time"].GetString());
            data->setApply(noticeData["apply"].GetInt());
            data->setMall_id(noticeData["mall_id"].GetInt());
            data->setMall_name(noticeData["mall_name"].GetString());
            data->setMall_path(noticeData["mall_path"].GetString());
            data->setNowday(noticeData["nowday"].GetInt());
            data->setSale_chip(noticeData["sale_chip"].GetInt());
            data->setSource(atoi(noticeData["source"].GetString()));
            data->setstarttime(noticeData["starttime"].GetString());
            data->setTimeleft(noticeData["timeleft"].GetInt());
            data->setusertype(noticeData["usertype"].GetInt());
            data->setcontext("");
            data->settype(0);
			noticeDatas.pushBack(data);
		}
	}

	return noticeDatas;
}

Vector<ProductData*> ProductData::TestData(){
    Vector<ProductData*> noticeDatas;
    stringstream os;
    for(int i=0; i< 0; i++){
        ProductData* data = new ProductData();
//        data->set_ID(i);
//        data->setimageID(i);
//        data->setnumber(i*5);
//        os<<"商品"<<i;
//        data->setname(os.str());
//        os.str("");
//        os.clear();
//        os<<"商品描述：这是第"<<i<<"件商品的描述";
//        data->setcontext(os.str());
//        os.str("");
//        os.clear();
//        noticeDatas.pushBack(data);
    }
    return noticeDatas;
}