#include "RoomData.h"

static RoomData* roomData = nullptr;

RoomData::RoomData(){
	this->actUserId = "";
	this->areaNumber = "";
	this->autoBuy = "";
	this->maxBuy = "";
    this->playerList.clear();
	this->watcherList.clear();
	this->playerListNum = 0;
	this->watcherListNum = 0;
	this->ctrlRight.clear();
    this->isAutoLookCard = false;
    this->isFollowAnyChip = false;
    this->isLookOrLeastCard = false;
	this->cardSeatNumber.clear();
	this->defaultAddChip = 0;
	this->followChip = 0;
	this->maxFollowChip = 0;
	this->handCard[0] = 0;
	this->handCard[1] = 0;
	this->timerId = "";
    this->roomInfo = nullptr;

	this->isAutoBuy = false;								//是否自动买入
	this->autoBuyChip = 0;									//自动买入筹码数
}

RoomData::~RoomData(){
	if (this->roomInfo != NULL){
		this->roomInfo->desk_pool.clear();
		delete this->roomInfo;
		this->roomInfo = NULL;
	}
	if (this->playerListNum > 0){
		this->playerList.clear();
	}
	if (this->watcherListNum > 0){
		this->watcherList.clear();
	}
}

RoomData* RoomData::getInstance(){
	if(roomData == NULL){
		roomData = new RoomData();
	}
	return roomData;
}

void RoomData::removeInstance(){
    if(roomData){
        delete roomData;
    }
    roomData = nullptr;
}

void RoomData::eraseData(){
	this->actUserId = "";
	this->areaNumber = "";
	this->autoBuy = "";
	this->maxBuy = "";
    this->playerList.clear();
	this->watcherList.clear();
	this->playerListNum = 0;
	this->watcherListNum = 0;
	this->ctrlRight.clear();
    this->isAutoLookCard = false;
    this->isFollowAnyChip = false;
    this->isLookOrLeastCard = false;
	this->cardSeatNumber.clear();
	this->defaultAddChip = 0;
	this->followChip = 0;
	this->maxFollowChip = 0;
	this->handCard[0] = 0;
	this->handCard[1] = 0;
	this->timerId = "";
	if(this->roomInfo != NULL){
		this->roomInfo->desk_pool.clear();
		delete this->roomInfo;
		this->roomInfo = NULL;
	}

	this->isAutoBuy = false;								//是否自动买入
	this->autoBuyChip = 0;									//自动买入筹码数
}

void RoomData::setRoomInfo(ROOM_INFO_T* roomInfo){
	this->roomInfo = roomInfo;
}

ROOM_INFO_T* RoomData::getRoomInfo(){
	return this->roomInfo;
}

void RoomData::addCtrlRight(int right){
	this->ctrlRight.push_back(right);
}

std::vector<int> RoomData::getCtrlRights(){
	return this->ctrlRight;
}

void RoomData::clearCtrlRights(){
	this->ctrlRight.clear();
}

void RoomData::addCardSeat(int cardSeat){
	this->cardSeatNumber.push_back(cardSeat);
}

void RoomData::removeCardSeat(int cardSeat){
	for(Vector<int>::iterator it = this->cardSeatNumber.begin(); it != this->cardSeatNumber.end(); it++){
		if(*it == cardSeat){
			this->cardSeatNumber.erase(it);
			return;
		}
	}
}

bool RoomData::hasCardSeat(int cardSeat){
	for(int i = 0; i < this->cardSeatNumber.size(); i++){
		if(this->cardSeatNumber.at(i) == cardSeat){
			return true;
		}
	}

	return false;
}

std::vector<int> RoomData::getCardSeatNumber(){
	return this->cardSeatNumber;
}

void RoomData::clearCardSeatNumber(){
	this->cardSeatNumber.clear();
}

void RoomData::setHandCard(int cardIndex, int cardNum){
	if(cardIndex == 1 || cardIndex == 2){
		this->handCard[cardIndex - 1] = cardNum;
	}
}

int RoomData::getHandCard(int cardIndex){
	if(cardIndex == 1 || cardIndex == 2){
		return this->handCard[cardIndex - 1];
	}
}

void RoomData::addPlayer(ROOM_USER_INFO_T* player){
	this->playerList.pushBack(player);
	this->playerListNum++;
}

ROOM_USER_INFO_T* RoomData::getPlayer(char* userId){
	for(int i = 0; i < this->playerListNum; i++){
		auto strUserId = this->playerList.at(i)->getUserId();
		if(strcmp(strUserId.c_str(), userId) == 0){
			return this->playerList.at(i);
		}
	}
	return NULL;
}

//通过座位号获取对应玩家
ROOM_USER_INFO_T* RoomData::getPlayerBySeatNum(int seatNum){
	for(int i = 0; i < this->playerList.size(); i++){
		if(this->playerList.at(i)->getSeatNum() == seatNum){
			return this->playerList.at(i);
		}
	}
	return NULL;
}

void RoomData::delPlayer(char* userId){
	for(Vector<ROOM_USER_INFO_T*>::iterator it = this->playerList.begin(); it != this->playerList.end(); it++){
		auto strUserId = (*it)->getUserId();
		if(strcmp(strUserId.c_str(), userId) == 0){
			this->playerList.erase(it);
			this->playerListNum--;
			return;
		}
	}
}

Vector<ROOM_USER_INFO_T*> RoomData::getPlayerList(){
	return this->playerList;
}

void RoomData::addWatcher(ROOM_USER_INFO_T* watcher){
	this->watcherList.pushBack(watcher);
	this->watcherListNum++;
}

ROOM_USER_INFO_T* RoomData::getWatcher(char* userId){
	for(int i = 0; i < this->watcherListNum; i++){
		auto strUserId = this->watcherList.at(i)->getUserId();
		if(strcmp(strUserId.c_str(), userId) == 0){
			return this->watcherList.at(i);
		}
	}
	return NULL;
}

void RoomData::delWatcher(char* userId){
	for(Vector<ROOM_USER_INFO_T*>::iterator it = this->watcherList.begin(); it != this->watcherList.end(); it++){
		auto strUserId = (*it)->getUserId();
		if(strcmp(strUserId.c_str(), userId) == 0){
			this->watcherList.erase(it);
			this->watcherListNum--;
			return;
		}
	}
}