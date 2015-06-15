#include "GlobalDef.h"
#include "GlobalChineseXml.h"

static GlobalDef* globalDef = nullptr;
GlobalDef* GlobalDef::getInstance(){
	if(globalDef == NULL){
		globalDef = new GlobalDef();
	}

	return globalDef;
}

GlobalDef::GlobalDef(){
	this->MUSIC_ON = true;
	this->SOUND_ON = true;
    paper_Index=0;
    headImage=1.0;
	this->volumnValue = UserDefault::getInstance()->getFloatForKey("VolumnValue", 0.5f);		//从本地文件加载音量设置值
	this->isAutoSeatOn = UserDefault::getInstance()->getBoolForKey("AutoSeat", false);			//从本地文件加载自动入座开关
	this->isVibrateOn = UserDefault::getInstance()->getBoolForKey("Vibrate", false);			//从本地文件加载振动开关

	this->deltaScreenX = 0.0f;
	this->deltaScreenY = 0.0f;
    
    this->httpServerAddress = "";
    this->socketIp = "";
    this->cdnAddress = "";

	//座位相对于屏幕中心点的坐标
	seatCenterPointPosition[0][0] = 49 + 105 + 38;
	seatCenterPointPosition[0][1] = 240 - 30 - 17;
	seatCenterPointPosition[1][0] = 49 + 105 * 2 + 38 * 3 - 15;
	seatCenterPointPosition[1][1] = 240 - 126 + 20 - 10; 
	seatCenterPointPosition[2][0] = 49 + 105 * 2 + 38 * 3 -15;
	seatCenterPointPosition[2][1] = 240 - 126 * 2 - 20 - 10;
	seatCenterPointPosition[3][0] = 49 + 105 + 38;
	seatCenterPointPosition[3][1] = 240 - 126 * 3 + 10;
	seatCenterPointPosition[4][0] = 0;
	seatCenterPointPosition[4][1] = 240 - 126 * 3 + 10;
	seatCenterPointPosition[5][0] = -49 - 105 - 38;
	seatCenterPointPosition[5][1] = 240 - 126 * 3 + 10;
	seatCenterPointPosition[6][0] = -49 - 105 * 2 - 38 * 3 + 15;
	seatCenterPointPosition[6][1] = 240 - 126 * 2 - 20 - 10;
	seatCenterPointPosition[7][0] = -49 - 105 * 2 - 38 * 3 + 15;
	seatCenterPointPosition[7][1] = 240 - 126 + 20 - 10;
	seatCenterPointPosition[8][0] = -49 - 105 - 38;
	seatCenterPointPosition[8][1] = 240 - 30 - 17;

	//公共牌相对于屏幕中心点的坐标
	deskCardPosition[0][0] = 0 - 71 * 2 - 5 * 2;
	deskCardPosition[0][1] = 30;
	deskCardPosition[1][0] = 0 - 71 -5;
	deskCardPosition[1][1] = 30;
	deskCardPosition[2][0] = 0;
	deskCardPosition[2][1] = 30;
	deskCardPosition[3][0] = 0 + 71 + 5;
	deskCardPosition[3][1] = 30;
	deskCardPosition[4][0] = 0 + 71 * 2 + 5 * 2;
	deskCardPosition[4][1] = 30;

	//9个座位上的两张手牌的位置
	handCardsCenterPosition[0][0][0] = 49 + 105 + 38 + 68 - 149;
	handCardsCenterPosition[0][0][1] = 240 - 43;
	handCardsCenterPosition[0][1][0] = 49 + 105 + 38 + 68 + 25 - 149;
	handCardsCenterPosition[0][1][1] = 240 - 43;

	handCardsCenterPosition[1][0][0] = 49 + 105 * 2 + 38 * 3 + 68 - 15 - 149;
	handCardsCenterPosition[1][0][1] = 240 - 126 + 20 - 23 + 12;
	handCardsCenterPosition[1][1][0] = 49 + 105 * 2 + 38 * 3 + 68 + 25 - 15 - 149;
	handCardsCenterPosition[1][1][1] = 240 - 126 + 20 - 23 + 12;

	handCardsCenterPosition[2][0][0] = 49 + 105 * 2 + 38 * 3 + 68 - 15 - 149;
	handCardsCenterPosition[2][0][1] = 240 - 126 * 2 - 20 - 23;
	handCardsCenterPosition[2][1][0] = 49 + 105 * 2 + 38 * 3 + 68 + 25 - 15 - 149;
	handCardsCenterPosition[2][1][1] = 240 - 126 * 2 - 20 - 23;

	handCardsCenterPosition[3][0][0] = 49 + 105 + 38 + 68 - 7;
	handCardsCenterPosition[3][0][1] = 240 - 126 * 3 - 3;
	handCardsCenterPosition[3][1][0] = 49 + 105 + 38 + 68 + 25 - 7;
	handCardsCenterPosition[3][1][1] = 240 - 126 * 3 - 3;

	handCardsCenterPosition[4][0][0] = 0 + 68 - 7;
	handCardsCenterPosition[4][0][1] = 240 - 126 * 3 - 3;
	handCardsCenterPosition[4][1][0] = 0 + 68 + 25 - 7;
	handCardsCenterPosition[4][1][1] = 240 - 126 * 3 - 3;

	handCardsCenterPosition[5][0][0] = -49 - 105 - 38 + 68 - 7;
	handCardsCenterPosition[5][0][1] = 240 - 126 * 3 - 3;
	handCardsCenterPosition[5][1][0] = -49 - 105 - 38 + 68 + 25 - 7;
	handCardsCenterPosition[5][1][1] = 240 - 126 * 3 - 3;

	handCardsCenterPosition[6][0][0] = -49 - 105 * 2 - 38 * 3 - 69 + 142;
	handCardsCenterPosition[6][0][1] = 240 - 126 * 2 - 20 - 23;
	handCardsCenterPosition[6][1][0] = -49 - 105 * 2 - 38 * 3 - 68 + 25 + 142;
	handCardsCenterPosition[6][1][1] = 240 - 126 * 2 - 20 - 23;

	handCardsCenterPosition[7][0][0] = -49 - 105 * 2 - 38 * 3 - 68 + 142;
	handCardsCenterPosition[7][0][1] = 240 - 126 + 20 - 23 + 12;
	handCardsCenterPosition[7][1][0] = -49 - 105 * 2 - 38 * 3 - 68 + 25 + 142;
	handCardsCenterPosition[7][1][1] = 240 - 126 + 20 - 23 + 12;

	handCardsCenterPosition[8][0][0] = -49 - 105 - 38 + 68 - 7;
	handCardsCenterPosition[8][0][1] = 240 - 43;
	handCardsCenterPosition[8][1][0] = -49 - 105 - 38 + 68 + 25 - 7;
	handCardsCenterPosition[8][1][1] = 240 - 43;

	//9个座位上的用户状态标签显示的中心点坐标
	statusLabelCenterPosition[0][0] = 49 + 105 + 38;
	statusLabelCenterPosition[0][1] = 240 - 68 - 10 - 27;
	statusLabelCenterPosition[1][0] = 49 + 105 * 2 + 38 * 3 - 15;
	statusLabelCenterPosition[1][1] = 240 - 126 + 20 - 68 + 10 - 10;
	statusLabelCenterPosition[2][0] = 49 + 105 * 2 + 38 * 3 - 15;
	statusLabelCenterPosition[2][1] = 240 - 126 * 2 - 20 - 68 + 10 - 10;
	statusLabelCenterPosition[3][0] = 49 + 105 + 38;
	statusLabelCenterPosition[3][1] = 240 - 126 * 3 - 68 + 30 - 10;
	statusLabelCenterPosition[4][0] = 0;
	statusLabelCenterPosition[4][1] = 240 - 126 * 3 - 68 + 30 - 10;
	statusLabelCenterPosition[5][0] = -49 - 105 - 38;
	statusLabelCenterPosition[5][1] = 240 - 126 * 3 - 68 + 30 - 10;
	statusLabelCenterPosition[6][0] = -49 - 105 * 2 - 38 * 3 + 15;
	statusLabelCenterPosition[6][1] = 240 - 126 * 2 - 20 - 68 + 10 - 10;
	statusLabelCenterPosition[7][0] = -49 - 105 * 2 - 38 * 3  + 15;
	statusLabelCenterPosition[7][1] = 240 - 126 + 20 - 68 + 10 - 10;
	statusLabelCenterPosition[8][0] = -49 - 105 - 38;
	statusLabelCenterPosition[8][1] = 240 - 68 - 10 - 27;

	//9个座位上的用户手上筹码状态显示的中心点坐标
	seatHandChipCenterPosition[0][0] = 49 + 105 + 38;
	seatHandChipCenterPosition[0][1] = 240 + 48 - 30 - 7;
	seatHandChipCenterPosition[1][0] = 49 + 105 * 2 + 38 * 3 - 15;
	seatHandChipCenterPosition[1][1] = 240 - 126 + 20 + 48 - 10 + 10;
	seatHandChipCenterPosition[2][0] = 49 + 105 * 2 + 38 * 3 - 15;
	seatHandChipCenterPosition[2][1] = 240 - 126 * 2 - 20 + 48 - 10 + 10;
	seatHandChipCenterPosition[3][0] = 49 + 105 + 38;
	seatHandChipCenterPosition[3][1] = 240 - 126 * 3 + 48 + 10 + 10;
	seatHandChipCenterPosition[4][0] = 0;
	seatHandChipCenterPosition[4][1] = 240 - 126 * 3 + 48 + 10 + 10;
	seatHandChipCenterPosition[5][0] = -49 - 105 - 38;
	seatHandChipCenterPosition[5][1] = 240 - 126 * 3 + 48 + 10 + 10;
	seatHandChipCenterPosition[6][0] = -49 - 105 * 2 - 38 * 3 + 15;
	seatHandChipCenterPosition[6][1] = 240 - 126 * 2 - 20 + 48 - 10 + 10;
	seatHandChipCenterPosition[7][0] = -49 - 105 * 2 - 38 * 3 + 15;
	seatHandChipCenterPosition[7][1] = 240 - 126 + 20 + 48 - 10 + 10;
	seatHandChipCenterPosition[8][0] = -49 - 105 - 38;
	seatHandChipCenterPosition[8][1] = 240 + 48 - 30 - 7;

	//9个座位上的用户下注筹码显示位置
	userDeskChipPosition[0][0] = 170 - 10 - 95;
	userDeskChipPosition[0][1] = 140 - 10 - 23 + 30;
	userDeskChipPosition[1][0] = 255 - 10 - 10 - 5;
	userDeskChipPosition[1][1] = 65;
	userDeskChipPosition[2][0] = 255 - 10 - 10 - 5;
	userDeskChipPosition[2][1] = 7;
	userDeskChipPosition[3][0] = 170 - 10;
	userDeskChipPosition[3][1] = -60 + 10 + 8;
	userDeskChipPosition[4][0] = -20 - 10;
	userDeskChipPosition[4][1] = -60 + 10 + 8;
	userDeskChipPosition[5][0] = -215 - 10;
	userDeskChipPosition[5][1] = -60 + 10 + 8;
	userDeskChipPosition[6][0] = -298 + 5;
	userDeskChipPosition[6][1] = 7;
	userDeskChipPosition[7][0] = -298 + 5;
	userDeskChipPosition[7][1] = 65;
	userDeskChipPosition[8][0] = -215 - 10 + 102;
	userDeskChipPosition[8][1] = 140 - 10 - 23 + 30;

	//7个筹码池位置
	deskPoolPosition[5][0] = -264;
	deskPoolPosition[5][1] = 92;
	deskPoolPosition[3][0] = -182;
	deskPoolPosition[3][1] = 92;
	deskPoolPosition[1][0] = -101;
	deskPoolPosition[1][1] = 92;
	deskPoolPosition[0][0] = -21;
	deskPoolPosition[0][1] = 92;
	deskPoolPosition[2][0] = 57;
	deskPoolPosition[2][1] = 92;
	deskPoolPosition[4][0] = 138;
	deskPoolPosition[4][1] = 92;
	deskPoolPosition[6][0] = 215;
	deskPoolPosition[6][1] = 92;
    
    //9 ge zuo wei de pao pao xian shi wei zhi, mao dian wei (0.5, 1)
    seatPaoPaoPosition[0][0] = 49 + 105 + 38;
    seatPaoPaoPosition[0][1] = 240 + 48 - 30 + 5;
    seatPaoPaoPosition[1][0] = 49 + 105 * 2 + 38 * 3 - 15 - 40;
	seatPaoPaoPosition[1][1] = 240 - 126 + 20 + 48 - 10 + 10 + 20;
	seatPaoPaoPosition[2][0] = 49 + 105 * 2 + 38 * 3 - 15 - 40;
	seatPaoPaoPosition[2][1] = 240 - 126 * 2 - 20 + 48 - 10 + 10 + 20;
	seatPaoPaoPosition[3][0] = 49 + 105 + 38;
	seatPaoPaoPosition[3][1] = 240 - 126 * 3 + 48 + 10 + 10 + 20;
	seatPaoPaoPosition[4][0] = 0;
	seatPaoPaoPosition[4][1] = 240 - 126 * 3 + 48 + 10 + 10 + 20;
	seatPaoPaoPosition[5][0] = -49 - 105 - 38;
	seatPaoPaoPosition[5][1] = 240 - 126 * 3 + 48 + 10 + 10 + 20;
	seatPaoPaoPosition[6][0] = -49 - 105 * 2 - 38 * 3 + 15 + 40;
	seatPaoPaoPosition[6][1] = 240 - 126 * 2 - 20 + 48 - 10 + 10 + 20;
	seatPaoPaoPosition[7][0] = -49 - 105 * 2 - 38 * 3 + 15 + 40;
	seatPaoPaoPosition[7][1] = 240 - 126 + 20 + 48 - 10 + 10 + 20;
	seatPaoPaoPosition[8][0] = -49 - 105 - 38;
	seatPaoPaoPosition[8][1] = 240 + 48 - 30 + 5;
    
    //zhuang jia biao zhi xian shi wei zhi, mao dian wei (0.5, 0.5)
    bankerPosition[0][0] = 49 + 105 + 38 - 50;
    bankerPosition[0][1] = 240 - 30 - 17 - 73;
    bankerPosition[1][0] = 49 + 105 * 2 + 38 * 3 - 15 - 50;
    bankerPosition[1][1] = 240 - 126 + 20 - 10 - 73;
    bankerPosition[2][0] = 49 + 105 * 2 + 38 * 3 -15 - 50;
	bankerPosition[2][1] = 240 - 126 * 2 - 20 - 10 + 73;
	bankerPosition[3][0] = 49 + 105 + 38 - 50;
	bankerPosition[3][1] = 240 - 126 * 3 + 10 + 73;
	bankerPosition[4][0] = 0 - 50;
	bankerPosition[4][1] = 240 - 126 * 3 + 10 + 73;
	bankerPosition[5][0] = -49 - 105 - 38 + 50;
	bankerPosition[5][1] = 240 - 126 * 3 + 10 + 73;
	bankerPosition[6][0] = -49 - 105 * 2 - 38 * 3 + 15 + 50;
	bankerPosition[6][1] = 240 - 126 * 2 - 20 - 10 + 73;
	bankerPosition[7][0] = -49 - 105 * 2 - 38 * 3 + 15 + 50;
	bankerPosition[7][1] = 240 - 126 + 20 - 10 - 73;
	bankerPosition[8][0] = -49 - 105 - 38 + 50;
	bankerPosition[8][1] = 240 - 30 - 17 - 73;
    
    userInfoLayerCenterPosition[0][0] = 0;
    userInfoLayerCenterPosition[0][1] = 240 - 30 - 17 - 193;
    userInfoLayerCenterPosition[1][0] = 49 + 105 * 2 + 38 * 3 - 15 - 271;
    userInfoLayerCenterPosition[1][1] = 0;
    userInfoLayerCenterPosition[2][0] = 49 + 105 * 2 + 38 * 3 - 15 - 271;
    userInfoLayerCenterPosition[2][1] = 0;
    userInfoLayerCenterPosition[3][0] = 0;
    userInfoLayerCenterPosition[3][1] = 240 - 126 * 3 + 10 + 193;
    userInfoLayerCenterPosition[4][0] = 0;
    userInfoLayerCenterPosition[4][1] = 240 - 126 * 3 + 10 + 193;
    userInfoLayerCenterPosition[5][0] = 0;
    userInfoLayerCenterPosition[5][1] = 240 - 126 * 3 + 10 + 193;
    userInfoLayerCenterPosition[6][0] = -49 - 105 * 2 - 38 * 3 + 15 + 271;
    userInfoLayerCenterPosition[6][1] = 0;
    userInfoLayerCenterPosition[7][0] = -49 - 105 * 2 - 38 * 3 + 15 + 271;
    userInfoLayerCenterPosition[7][1] = 0;
    userInfoLayerCenterPosition[8][0] = 0;
    userInfoLayerCenterPosition[8][1] = 240 - 30 - 17 - 193;
    
	communicateList.push_back(GlobalChineseXml::getInstance()->getValue("communicateContent1"));
	communicateList.push_back(GlobalChineseXml::getInstance()->getValue("communicateContent2"));
	communicateList.push_back(GlobalChineseXml::getInstance()->getValue("communicateContent3"));
	communicateList.push_back(GlobalChineseXml::getInstance()->getValue("communicateContent4"));
	communicateList.push_back(GlobalChineseXml::getInstance()->getValue("communicateContent5"));
	communicateList.push_back(GlobalChineseXml::getInstance()->getValue("communicateContent6"));
	communicateList.push_back(GlobalChineseXml::getInstance()->getValue("communicateContent7"));
	communicateList.push_back(GlobalChineseXml::getInstance()->getValue("communicateContent8"));
}

void GlobalDef::setMusicOn(bool musicOn){
	this->MUSIC_ON = musicOn;
}

bool GlobalDef::getMusicOn(){
	return this->MUSIC_ON;
}

void GlobalDef::setSoundOn(bool soundOn){
	this->SOUND_ON = soundOn;
}

bool GlobalDef::getSoundOn(){
	return this->SOUND_ON;
}

void GlobalDef::setVolumnValue(float volumnValue){
	this->volumnValue = volumnValue;
	UserDefault::getInstance()->setFloatForKey("VolumnValue", volumnValue);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volumnValue);
}

float GlobalDef::getVolumnValue(){
	this->volumnValue = UserDefault::getInstance()->getFloatForKey("VolumnValue", 0.5f);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(this->volumnValue);
	return this->volumnValue;
}

void GlobalDef::setIsAutoSeatOn(bool isAutoSeatOn){
	this->isAutoSeatOn = isAutoSeatOn;
	UserDefault::getInstance()->setBoolForKey("AutoSeat", isAutoSeatOn);
}

bool GlobalDef::getIsAutoSeatOn(){
	this->isAutoSeatOn = UserDefault::getInstance()->getBoolForKey("AutoSeat", false);
	return this->isAutoSeatOn;
}

void GlobalDef::setIsVibrateOn(bool isVibrateOn){
	this->isVibrateOn = isVibrateOn;
	UserDefault::getInstance()->setBoolForKey("Vibrate", isVibrateOn);
}

bool GlobalDef::getIsVibrateOn(){
	this->isVibrateOn = UserDefault::getInstance()->getBoolForKey("Vibrate", false);
	return this->isVibrateOn;
}

void GlobalDef::setDeltaScreenX(float deltaScreenX){
	this->deltaScreenX = deltaScreenX;
}

float GlobalDef::getDeltaScreenX(){
	return this->deltaScreenX;
}

void GlobalDef::setDeltaScreenY(float deltaScreenY){
	this->deltaScreenY = deltaScreenY;
}

float GlobalDef::getDeltaScreenY(){
	return this->deltaScreenY;
}

void GlobalDef::setFrameScaleX(float frameScaleX){
    this->frameScaleX = frameScaleX;
}

float GlobalDef::getFrameScaleX(){
    return this->frameScaleX;
}

void GlobalDef::setPaperIndex(int  paperIndex){
    paper_Index = paperIndex;
}

int GlobalDef::getPaperIndex(){
    return this->paper_Index;
}


void GlobalDef::setHeadImageScale(float  scales){
     headImage = scales;
}

float GlobalDef::getHeadImageScale(){
    return this->headImage;
}
void GlobalDef::setLoginRewardNum(int  paperIndex){
    LoginRewardNum = paperIndex;
}

int GlobalDef::getLoginRewardNum(){
    return this->LoginRewardNum;
}

void GlobalDef::setHttpServerAddress(std::string httpServerAddress){
    this->httpServerAddress = httpServerAddress;
}

std::string GlobalDef::getHttpServerAddress(){
    return this->httpServerAddress;
}

void GlobalDef::setSocketIp(std::string socketIp){
    this->socketIp = socketIp;
}

std::string GlobalDef::getSocketIp(){
    return this->socketIp;
}

void GlobalDef::setCDNAddress(std::string cdnAddress){
    this->cdnAddress = cdnAddress;
}

std::string GlobalDef::getCDNAddress(){
    return this->cdnAddress;
}

//获取座位相对于屏幕中心点的坐标，index表示x或y
int GlobalDef::getSeatCenterPointPosition(int seatNum, int index){
	if(seatNum > 0 && seatNum <= 9){
		if(index == 0 || index == 1){
			return seatCenterPointPosition[seatNum - 1][index];
		}
	}
    
    return 0;
}

//获取公共牌相对于屏幕中心点的坐标，index表示x或y
int GlobalDef::getDeskCardPosition(int cardIndex, int index){
	if(cardIndex > 0 && cardIndex <= 5){
		if(index == 0 || index == 1){
			return deskCardPosition[cardIndex - 1][index];
		}
	}
    
    return 0;
}

//获取9个座位上的两张手牌的位置，index表示x或y
int GlobalDef::getHandCardsCenterPosition(int seatNum, int cardIndex, int index){
	if(seatNum > 0 && seatNum <= 9){
		if(cardIndex == 1 || cardIndex == 2){
			if(index == 0 || index == 1){
				return handCardsCenterPosition[seatNum - 1][cardIndex - 1][index];
			}
		}
	}
    
    return 0;
}

//获取9个座位上的用户状态标签显示的中心点坐标，index表示x或y
int GlobalDef::getStatusLabelCenterPosition(int seatNum, int index){
	if(seatNum > 0 && seatNum <= 9){
		if(index == 0 || index == 1){
			return statusLabelCenterPosition[seatNum - 1][index];
		}
	}
    
    return 0;
}

//获取9个座位上的用户手上筹码状态显示的中心点坐标，index表示x或y
int GlobalDef::getSeatHandChipCenterPosition(int seatNum, int index){
	if(seatNum > 0 && seatNum <= 9){
		if(index == 0 || index == 1){
			return seatHandChipCenterPosition[seatNum - 1][index];
		}
	}
    
    return 0;
}

//获取9个座位上的用户下注筹码显示位置
int GlobalDef::getUserDeskChipPosition(int seatNum, int index){
	if(seatNum > 0 && seatNum <= 9){
		if(index == 0 || index == 1){
			return userDeskChipPosition[seatNum - 1][index];
		}
	}
    
    return 0;
}

//获取7个筹码池位置，index表示x或y，id为0~6
int GlobalDef::getDeskPoolPosition(int id, int index){
	if(id >= 0 && id < 7){
		if(index == 0 || index == 1){
			return deskPoolPosition[id][index];
		}
	}
    
    return 0;
}

//9 ge zuo wei de pao pao xian shi wei zhi, mao dian wei (0.5, 1)
int GlobalDef::getSeatPaoPaoPosition(int seatNum, int index){
    if(seatNum > 0 && seatNum <= 9){
		if(index == 0 || index == 1){
			return seatPaoPaoPosition[seatNum - 1][index];
		}
	}
    
    return 0;
}

//9 ge zuo wei de zhuang jia biao zhi xian shi wei zhi
int GlobalDef::getBankerPosition(int seatNum, int index){
    if(seatNum > 0 && seatNum <= 9){
		if(index == 0 || index == 1){
			return bankerPosition[seatNum - 1][index];
		}
	}
    
    return 0;
}

//9 ge zuo wei de yong hu xin xi tan kuang zhong xin wei zhi, mao dian wei (0.5, 0.5)
int GlobalDef::getUserInfoLayerCenterPosition(int seatNum, int index){
    if(seatNum > 0 && seatNum <= 9){
		if(index == 0 || index == 1){
			return userInfoLayerCenterPosition[seatNum - 1][index];
		}
	}
    
    return 0;
}

//获取聊天内容列表
std::vector<std::string> GlobalDef::getCommunicateList(){
	return this->communicateList;
}