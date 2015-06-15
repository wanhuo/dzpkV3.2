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

	int getSeatCenterPointPosition(int seatNum, int index);						//��ȡ��λ�������Ļ���ĵ�����꣬index��ʾx��y��seatNumΪ1~9
	int getDeskCardPosition(int cardIndex, int index);							//��ȡ�������������Ļ���ĵ�����꣬index��ʾx��y
	int getHandCardsCenterPosition(int seatNum, int cardIndex, int index);		//��ȡ9����λ�ϵ��������Ƶ�λ�ã�index��ʾx��y
	int getStatusLabelCenterPosition(int seatNum, int index);					//��ȡ9����λ�ϵ��û�״̬��ǩ��ʾ�����ĵ����꣬index��ʾx��y
	int getSeatHandChipCenterPosition(int seatNum, int index);					//��ȡ9����λ�ϵ��û����ϳ���״̬��ʾ�����ĵ����꣬index��ʾx��y
	int getUserDeskChipPosition(int seatNum, int index);						//��ȡ9����λ�ϵ��û���ע������ʾλ�ã�index��ʾx��y
	int getDeskPoolPosition(int id, int index);									//��ȡ7�������λ�ã�index��ʾx��y��idΪ0~6
    int getSeatPaoPaoPosition(int seatNum, int index);                          //9 ge zuo wei de pao pao xian shi wei zhi, mao dian wei (0.5, 1)
    int getBankerPosition(int seatNum, int index);                              //9 ge zuo wei de zhuang jia biao zhi xian shi wei zhi
    int getUserInfoLayerCenterPosition(int seatNum, int index);                 //9 ge zuo wei de yong hu xin xi tan kuang zhong xin wei zhi, mao dian wei (0.5, 0.5)

	std::vector<std::string> getCommunicateList();								//��ȡ���������б�

private:
	GlobalDef();

	bool MUSIC_ON;							//���ô�����
	bool SOUND_ON;							//���ô�����
	float volumnValue;						//������С
	bool isAutoSeatOn;						//�Ƿ��Զ�����
	bool isVibrateOn;						//�Ƿ�����
	 
	float deltaScreenX;						//��Ļ����ʱX����ƫ����
	float deltaScreenY;						//��Ļ����ʱY����ƫ����
    float frameScaleX;                      //ping mu X fang xiang la sheng bi li
    
    std::string httpServerAddress;
    std::string socketIp;
    std::string cdnAddress;

	int seatCenterPointPosition[9][2];		//��λ�������Ļ���ĵ������
	int deskCardPosition[5][2];				//�������������Ļ���ĵ������
	int handCardsCenterPosition[9][2][2];	//9����λ�ϵ��������Ƶ�λ��
	int statusLabelCenterPosition[9][2];	//9����λ�ϵ��û�״̬��ǩ��ʾ�����ĵ�����
	int seatHandChipCenterPosition[9][2];	//9����λ�ϵ��û����ϳ���״̬��ʾ�����ĵ�����
	int userDeskChipPosition[9][2];			//9����λ�ϵ��û���ע������ʾλ��
	int deskPoolPosition[7][2];				//7�������λ��
    int seatPaoPaoPosition[9][2];           //9 ge zuo wei de pao pao xian shi wei zhi, mao dian wei (0.5, 1)
    int bankerPosition[9][2];               //zhuang jia biao zhi xian shi wei zhi, mao dian wei (0.5, 0.5)
    int userInfoLayerCenterPosition[9][2];  //9 ge zuo wei de yong hu xin xi tan kuang zhong xin wei zhi, mao dian wei (0.5, 0.5)

	std::vector<std::string> communicateList;	//���������б�
protected:
    int paper_Index;//������Ŀ���ҳ��
    int LoginRewardNum;//������Ŀ���ҳ��
    float headImage;//����ͷ�����ŵ�
};

#endif