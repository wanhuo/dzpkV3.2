#ifndef _GAME_HALL_DATA_
#define _GAME_HALL_DATA_

#include "cocos2d.h"
#include <iostream>
#include "../GlobalDef.h"
#include "../../GlobalDef/JsonWritter.h"

USING_NS_CC;
using namespace std;

class GameHallData : public Ref {
public:
	static Vector<GameHallData*> parseData(const string strData);

	CC_SYNTHESIZE(long, roomid, roomID);
	CC_SYNTHESIZE(string, room_name, roomName);
	CC_SYNTHESIZE(long, current_seat_person, cur_Seat_Person);
	CC_SYNTHESIZE(long, current_sideline_person, cur_Sideline_Person);
	CC_SYNTHESIZE(long, small_blind, small_Blind);
	CC_SYNTHESIZE(long, minimum_bring, minBring);
	CC_SYNTHESIZE(long, maximum_bring, maxBring);
	CC_SYNTHESIZE(long, room_seat_number, room_Seat_Number);
	CC_SYNTHESIZE(unsigned, room_type, room_Type);
	CC_SYNTHESIZE(long, room_level_st, room_Level_st);
	CC_SYNTHESIZE(long, room_level_nd, room_level_nd);
	CC_SYNTHESIZE(long, bet_time, bet_time);
	CC_SYNTHESIZE(string, host_ip, host_ip);
	CC_SYNTHESIZE(string, port, port);

	GameHallData(); 
	~GameHallData() {};
};

#endif