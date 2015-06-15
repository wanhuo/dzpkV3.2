#include "GameHallData.h"


GameHallData::GameHallData()
{
	roomid = 0;
	room_name = "";
	current_seat_person = 0;
	current_sideline_person = 0;
	small_blind = 0;
	minimum_bring = 0;
	maximum_bring = 0;
	room_seat_number = 0;
	room_type = 0;
	room_level_st = 0;
	room_level_nd = 0;
	bet_time = 0;
	host_ip = "";
	port = "";
}

Vector<GameHallData*> GameHallData::parseData(const string strData)
{
	Vector<GameHallData*> HallDatas;
	rapidjson::Document docHallRecv;
	docHallRecv.Parse<0>(strData.c_str());

	rapidjson::Value& resp = docHallRecv["resp"];
	rapidjson::Value& rooms = resp["rooms"];
	if (rooms.IsArray())
	{
		for (int i=0; i< rooms.Size(); i++)
		{
			rapidjson::Value& hallData = rooms[i];
			GameHallData* data = new GameHallData();
			data->setroomID(hallData["room_id"].GetInt());
			data->setroomName(hallData["room_name"].GetString());
			data->setcur_Seat_Person(hallData["current_seat_person"].GetInt());
//			rapidjson::Type ty = hallData["current_sideline_person"].GetType();
			data->setcur_Sideline_Person(hallData["current_sideline_person"].GetInt());
			data->setsmall_Blind(hallData["small_blind"].GetInt());
			data->setminBring(hallData["minimum_bring"].GetInt());
			data->setmaxBring(hallData["maximum_bring"].GetInt());
			data->setroom_Seat_Number(hallData["room_seat_number"].GetInt());
			data->setroom_Type(hallData["room_type"].GetInt());
			data->setroom_Level_st(hallData["room_level_st"].GetInt());
			data->setroom_level_nd(hallData["room_level_nd"].GetInt());
			data->setbet_time(hallData["bet_time"].GetInt());
			data->sethost_ip(hallData["host_ip"].GetString());
			data->setport(hallData["port"].GetString());
			
			HallDatas.pushBack(data);
		}
	}

	return HallDatas;
}