#include "NoticeData.h"


NoticeData::NoticeData():time(0),type(0),time_str(""),context("")
{
	
}
Vector<NoticeData*> NoticeData::parseData(const string strData)
{
	Vector<NoticeData*> noticeDatas;
	rapidjson::Document docHallRecv;
	docHallRecv.Parse<0>(strData.c_str());
	rapidjson::Value& resp1 = docHallRecv["resp"]["message_fri"];
    rapidjson::Value& resp2 = docHallRecv["resp"]["message_pay"];
    rapidjson::Value& resp3 = docHallRecv["resp"]["message_sys"];
	if (resp1.IsArray())
	{
		for (int i=0; i< resp1.Size(); i++)
		{
			rapidjson::Value& noticeData = resp1[i];
			NoticeData* data = new NoticeData();
//            data->setm_Id(noticeData["id"].GetInt());
            int k=0;
            data->setm_Id(atoi(noticeData["id"].GetString()));
			data->settime(atoi(noticeData["time"].GetString()));
			data->settime_str(noticeData["createtime"].GetString());
            data->setcontext(noticeData["message"].GetString());
//            data->settype(0);
           data->settype(noticeData["message_type"].GetInt());
//            data->settime(atoi(noticeData["message_type"].GetString()));
			noticeDatas.pushBack(data);
		}
	}
    
    if (resp2.IsArray())
	{
		for (int i=0; i< resp2.Size(); i++)
		{
			rapidjson::Value& noticeData = resp2[i];
			NoticeData* data = new NoticeData();
            int k=0;
            data->setm_Id(atoi(noticeData["id"].GetString()));
			data->settime(atoi(noticeData["time"].GetString()));
			data->settime_str(noticeData["createtime"].GetString());
            data->setcontext(noticeData["message"].GetString());
            data->settype(noticeData["message_type"].GetInt());
//            data->settype(0);
			noticeDatas.pushBack(data);
		}
	}
    
    if (resp3.IsArray())
	{
		for (int i=0; i< resp3.Size(); i++)
		{
			rapidjson::Value& noticeData = resp3[i];
			NoticeData* data = new NoticeData();
            int k=0;
            data->setm_Id(atoi(noticeData["id"].GetString()));
			data->settime(atoi(noticeData["time"].GetString()));
			data->settime_str(noticeData["createtime"].GetString());
            data->setcontext(noticeData["message"].GetString());
            int type=atoi(noticeData["message_type"].GetString());
            data->settype(atoi(noticeData["message_type"].GetString()));
			noticeDatas.pushBack(data);
		}
	}
    
	return noticeDatas;
}

