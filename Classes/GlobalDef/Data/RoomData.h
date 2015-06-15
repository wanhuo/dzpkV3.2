#ifndef _ROOM_DATA_
#define _ROOM_DATA_

#include "cocos2d.h"
#include "../GlobalDef.h"

USING_NS_CC;

/*边池信息*/
class ROOM_DESK_POOL_T : public Ref{
public:
	ROOM_DESK_POOL_T(){
		this->pool = 0;
		this->type = 0;
		this->id = 0;
	}

	~ROOM_DESK_POOL_T(){}

	CC_SYNTHESIZE(long, pool, Pool);		//边池里总注金
	CC_SYNTHESIZE(int, type, Type);		    //池子的种类：1: 主池 0:边池
	CC_SYNTHESIZE(int, id, Id);		        //池子的id：0: 主池 >1:边池
};

/*房间信息*/
class ROOM_INFO_T : public Ref{
public:
	ROOM_INFO_T(){
		this->step = 0;
		this->roomType = 0;
		this->roomSeatNum = 0;
		this->allDeskChips = 0;
		this->banker = 0;
		this->betTime = 0;
		this->turn = 0;
		this->bigBlind = 0;
		this->smallBlind = 0;
		memset(deskCard, 0, sizeof(int) * 5);
		desk_pool.clear();
		this->maxBuyChip = 0;
		this->minBuyChip = 0;
		this->minBlind = 0;
		this->maxBlind = 0;
		this->roomName = "";
		this->roomNumber = 0;
	}
	~ROOM_INFO_T(){}

	CC_SYNTHESIZE(int, step, Step);				                //当前房间状态 (牌局的步骤  0： 未开始  1：发底牌   2:翻三张  3:翻第四张  4：翻最后一张（比大小）)
	CC_SYNTHESIZE(int, roomType, RoomType);				        //房间场次类别   2：游戏场  3：比赛场
	CC_SYNTHESIZE(int, roomSeatNum, RoomSeatNum);			    //房间内座位数     5人房  9人房
	CC_SYNTHESIZE(long, allDeskChips, AllDeskChips);			//桌面总筹码
	CC_SYNTHESIZE(int, banker, Banker);							//庄家位置
	CC_SYNTHESIZE(int, betTime, BetTime);						//下注间隙
	CC_SYNTHESIZE(int, turn, Turn);				                //当前到那个座位
	CC_SYNTHESIZE(int, bigBlind, BigBlind);						//大盲注位置
	CC_SYNTHESIZE(int, smallBlind, SmallBlind);					//小盲注位置
	int deskCard[5];											//公共牌，每张牌由三位整数构成 最左位代表花色 例如 103（方片3） 312（红桃Q)，注意：A是14
	Vector<ROOM_DESK_POOL_T *> desk_pool;						//边池信息
	CC_SYNTHESIZE(long, maxBuyChip, MaxBuyChip);				//最大买入
	CC_SYNTHESIZE(long, minBuyChip, MinBuyChip);				//最小买入
	CC_SYNTHESIZE(long, minBlind, MinBlind);					//小盲注额
	CC_SYNTHESIZE(long, maxBlind, MaxBlind);					//大盲注额
	CC_SYNTHESIZE(std::string, roomName, RoomName);				//房间名称
	CC_SYNTHESIZE(int, roomNumber, RoomNumber);					//房间号
};

/*房间用户信息*/
class ROOM_USER_INFO_T : public Ref{
public:
	ROOM_USER_INFO_T(){
		this->userId = "";
		this->userAccount = "";
		this->nickName = "";
		this->dzpkLevel = 0;
		this->dzpkExperience = 0;
		this->vipLevel = 0;
		this->chipAccount = 0;
		this->coinAccount = 0;
		this->city = "";
		this->totalTime = "";
		this->winPer = "";
		this->rankName = "";
		this->headPhotoSerial = "";
		this->deskChip = 0;
		this->sex = 0;
		this->userStatus = 0;
		this->roomNum = 0;
		this->seatNum = 0;
		this->handChips = 0;
		this->handChipsToShow = 0;
		this->giftId = 0;
		this->levelGift = "";
		this->mallPath = "";
		this->levelTitle = "";
	}
	~ROOM_USER_INFO_T(){}

	CC_SYNTHESIZE(std::string, userId, UserId);						//用户ID
	CC_SYNTHESIZE(std::string, userAccount, UserAccount);			//用户名
	CC_SYNTHESIZE(std::string, nickName, NickName);					//用户昵称
	CC_SYNTHESIZE(int, dzpkLevel, DzpkLevel);						//用户等级
	CC_SYNTHESIZE(int, dzpkExperience, DzpkExperience);				//用户经验
	CC_SYNTHESIZE(int, vipLevel, VipLevel);							//用户vip等级
	CC_SYNTHESIZE(long, chipAccount, ChipAccount);					//筹码账户
	CC_SYNTHESIZE(long, coinAccount, CoinAccount);					//龙币账户
	CC_SYNTHESIZE(std::string, city, City);							//用户所在城市
	CC_SYNTHESIZE(std::string, totalTime, TotalTime);				//玩总次数
	CC_SYNTHESIZE(std::string, winPer, WinPer);						//胜率
	CC_SYNTHESIZE(std::string, rankName, RankName);					//头衔
	CC_SYNTHESIZE(std::string, headPhotoSerial, HeadPhotoSerial);	//头像序列号
	CC_SYNTHESIZE(long, deskChip, DeskChip);						//当前这一轮丢到桌面的筹码
	CC_SYNTHESIZE(int, sex, Sex);									//性别  1男 0女
	CC_SYNTHESIZE(int, userStatus, UserStatus);						//用户状态：0：旁观 1：坐下(等待下一局) 2：加注 3：跟注 4：看牌 5:弃牌 6:全下 7：等待下注
	CC_SYNTHESIZE(int, roomNum, RoomNum);							//所在的房间号
	CC_SYNTHESIZE(int, seatNum, SeatNum);							//分配的座位号
	CC_SYNTHESIZE(long, handChips, HandChips);						//手上有多少筹码，一局未结束时不改变，清算时更新
	CC_SYNTHESIZE(long, handChipsToShow, HandChipsToShow);			//显示在桌面上的手上筹码，下注时进行改变
	CC_SYNTHESIZE(int, giftId, GiftId);								//礼品ID
	CC_SYNTHESIZE(std::string, levelGift, LevelGift);				
	CC_SYNTHESIZE(std::string, mallPath, MallPath);					//商品路径
	CC_SYNTHESIZE(std::string, levelTitle, LevelTitle);				//级别头衔
};

/***********************************************
*房间信息
***********************************************/
class RoomData : public Ref{
public:
	static RoomData* getInstance();
    static void removeInstance();
	void eraseData();

	void setRoomInfo(ROOM_INFO_T* roomInfo);
	ROOM_INFO_T* getRoomInfo();

	void addCtrlRight(int right);
	std::vector<int> getCtrlRights();
	void clearCtrlRights();

	void addCardSeat(int cardSeat);
	void removeCardSeat(int cardSeat);
	bool hasCardSeat(int cardSeat);
	std::vector<int> getCardSeatNumber();
	void clearCardSeatNumber();

	void setHandCard(int cardIndex, int cardNum);
	int getHandCard(int cardIndex);

	void addPlayer(ROOM_USER_INFO_T* player);
	ROOM_USER_INFO_T* getPlayer(char* userId);
	ROOM_USER_INFO_T* getPlayerBySeatNum(int seatNum);		//通过座位号获取对应玩家
	void delPlayer(char* userId);
	Vector<ROOM_USER_INFO_T*> getPlayerList();

	void addWatcher(ROOM_USER_INFO_T* watcher);
	ROOM_USER_INFO_T* getWatcher(char* userId);
	void delWatcher(char* userId);

protected:
	RoomData();
	~RoomData();

public:
	CC_SYNTHESIZE(std::string, actUserId, ActUserId);						//act_user_id
	CC_SYNTHESIZE(std::string, areaNumber, AreaNumber);	
	CC_SYNTHESIZE(std::string, autoBuy, AutoBuy);							//游戏币为0时是否自动买入继续游戏
	CC_SYNTHESIZE(std::string, maxBuy, MaxBuy);								//每盘自动购买“最大买入”继续游戏
	std::vector<int> ctrlRight;												// 0:加注 1：跟注  2：看牌  3：弃牌
	CC_SYNTHESIZE(bool, isLookOrLeastCard, IsLookOrLeastCard);				//看或弃牌复选框变量
	CC_SYNTHESIZE(bool, isAutoLookCard, IsAutoLookCard);					//自动看牌复选框变量
	CC_SYNTHESIZE(bool, isFollowAnyChip, IsFollowAnyChip);					//跟任何注复选框变量
	CC_SYNTHESIZE(long, defaultAddChip, DefaultAddChip);					//默认加注筹码
	CC_SYNTHESIZE(long, followChip, FollowChip);							//当前最小跟注
	CC_SYNTHESIZE(long, maxFollowChip, MaxFollowChip);						//该房间最大跟注
	CC_SYNTHESIZE(std::string, timerId, TimerId);							//定时器ID
	std::vector<int> cardSeatNumber;										//发手牌的座位列表
	int handCard[2];														//玩家自己的手牌
	ROOM_INFO_T* roomInfo;													//房间信息，结构在上面
	CC_SYNTHESIZE(int, atGame, AtGame);										//该玩家是否在游戏中   1是 0否
	Vector<ROOM_USER_INFO_T*> playerList;									//房间用户信息
	CC_SYNTHESIZE(int, playerListNum, PlayerListNum);						//房间用户数量
	Vector<ROOM_USER_INFO_T*> watcherList;									//房间旁观用户信息
	CC_SYNTHESIZE(int, watcherListNum, WatcherListNum);						//房间旁观用户数量

	CC_SYNTHESIZE(int, photoNum, PhotoNum);									//网络照片下载个数，房间用户全部下载完成后再进入房间

	CC_SYNTHESIZE(bool, isAutoBuy, IsAutoBuy);								//是否自动买入
	CC_SYNTHESIZE(long, autoBuyChip, AutoBuyChip);							//自动买入筹码数
};

#endif