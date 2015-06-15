#ifndef _GLOBAL_DEF_
#define _GLOBAL_DEF_

#include <iostream>
#include <vector>
#include "SimpleAudioEngine.h"

#define HEAD_DESK_POOL_COUNT 10
#define HEAD_CHAR_LEN 50
#define HEAD_LONG_CHAR_LEN 250
#define HEAD_ROOM_USER_COUNT 30

typedef enum{	
	VOLUMN_TAG = 1,
	VOLICE_TAG
}VolumeObjectTag;

typedef enum{
	VIBRATE_TAG = 1,
	SIT_TAG,
	ENTER_TAG
}ToggleObjectTag;

class GlobalDef{
public:
	static GlobalDef* getInstance();

	void setMusicOn(bool musicOn);
	bool getMusicOn();

	void setSoundOn(bool soundOn);
	bool getSoundOn();

	void setVolumnValue(float volumnValue);
	float getVolumnValue();

	void setIsAutoSeatOn(bool isAutoSeatOn);
	bool getIsAutoSeatOn();

	void setIsVibrateOn(bool isVibrateOn);
	bool getIsVibrateOn();

	void setDeltaScreenX(float deltaScreenX);
	float getDeltaScreenX();

	void setDeltaScreenY(float deltaScreenY);
	float getDeltaScreenY();
    
    void setFrameScaleX(float frameScaleX);
    float getFrameScaleX();
    void setPaperIndex(int paperIndex);
    int getPaperIndex();
    
    void setHeadImageScale(float scales);
    float getHeadImageScale();
    
    void setLoginRewardNum(int paperIndex);
    int getLoginRewardNum();
    
    void setHttpServerAddress(std::string httpServerAddress);
    std::string getHttpServerAddress();
    
    void setSocketIp(std::string socketIp);
    std::string getSocketIp();
    
    void setCDNAddress(std::string cdnAddress);
    std::string getCDNAddress();

	int getSeatCenterPointPosition(int seatNum, int index);						//获取座位相对于屏幕中心点的坐标，index表示x或y，seatNum为1~9
	int getDeskCardPosition(int cardIndex, int index);							//获取公共牌相对于屏幕中心点的坐标，index表示x或y
	int getHandCardsCenterPosition(int seatNum, int cardIndex, int index);		//获取9个座位上的两张手牌的位置，index表示x或y
	int getStatusLabelCenterPosition(int seatNum, int index);					//获取9个座位上的用户状态标签显示的中心点坐标，index表示x或y
	int getSeatHandChipCenterPosition(int seatNum, int index);					//获取9个座位上的用户手上筹码状态显示的中心点坐标，index表示x或y
	int getUserDeskChipPosition(int seatNum, int index);						//获取9个座位上的用户下注筹码显示位置，index表示x或y
	int getDeskPoolPosition(int id, int index);									//获取7个筹码池位置，index表示x或y，id为0~6
    int getSeatPaoPaoPosition(int seatNum, int index);                          //9 ge zuo wei de pao pao xian shi wei zhi, mao dian wei (0.5, 1)
    int getBankerPosition(int seatNum, int index);                              //9 ge zuo wei de zhuang jia biao zhi xian shi wei zhi
    int getUserInfoLayerCenterPosition(int seatNum, int index);                 //9 ge zuo wei de yong hu xin xi tan kuang zhong xin wei zhi, mao dian wei (0.5, 0.5)

	std::vector<std::string> getCommunicateList();								//获取聊天内容列表

private:
	GlobalDef();

	bool MUSIC_ON;							//设置打开音乐
	bool SOUND_ON;							//设置打开声音
	float volumnValue;						//音量大小
	bool isAutoSeatOn;						//是否自动入座
	bool isVibrateOn;						//是否开启振动
	 
	float deltaScreenX;						//屏幕拉伸时X方向偏移量
	float deltaScreenY;						//屏幕拉伸时Y方向偏移量
    float frameScaleX;                      //ping mu X fang xiang la sheng bi li
    
    std::string httpServerAddress;
    std::string socketIp;
    std::string cdnAddress;

	int seatCenterPointPosition[9][2];		//座位相对于屏幕中心点的坐标
	int deskCardPosition[5][2];				//公共牌相对于屏幕中心点的坐标
	int handCardsCenterPosition[9][2][2];	//9个座位上的两张手牌的位置
	int statusLabelCenterPosition[9][2];	//9个座位上的用户状态标签显示的中心点坐标
	int seatHandChipCenterPosition[9][2];	//9个座位上的用户手上筹码状态显示的中心点坐标
	int userDeskChipPosition[9][2];			//9个座位上的用户下注筹码显示位置
	int deskPoolPosition[7][2];				//7个筹码池位置
    int seatPaoPaoPosition[9][2];           //9 ge zuo wei de pao pao xian shi wei zhi, mao dian wei (0.5, 1)
    int bankerPosition[9][2];               //zhuang jia biao zhi xian shi wei zhi, mao dian wei (0.5, 0.5)
    int userInfoLayerCenterPosition[9][2];  //9 ge zuo wei de yong hu xin xi tan kuang zhong xin wei zhi, mao dian wei (0.5, 0.5)

	std::vector<std::string> communicateList;	//聊天内容列表
protected:
    int paper_Index;//玩家中心控制页面
    int LoginRewardNum;//玩家中心控制页面
    float headImage;//控制头像缩放的
};

#endif