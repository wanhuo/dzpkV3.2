#ifndef _GAME_SCENE_
#define _GAME_SCENE_

#include "cocos2d.h"
#include "../NetWork/NetDelegate.h"
#include "../GlobalDef/Data/RoomData.h"
#include "../GlobalDef/JsonWritter.h"
#include "../GlobalDef/GlobalDef.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "ExitGameDelegate.h"
#include "ExitGameLayer.h"
#include "AddChipLayer.h"
#include "AddChipDelegate.h"
#include "HasNotEnoughChipLayer.h"
#include "HasNotEnoughChipDelegate.h"
#include "HasNotEnoughChopLayer.h"
#include "HasNotEnoughChopDelegate.h"
#include "StandupLayer.h"
#include "StandupDelegate.h"
#include "CommunicateLayer.h"
#include "CommunicateLayerDelegate.h"
#include "CommunicateInputDelegate.h"
#include "../GlobalDef/Data/CommunicateData.h"
#include "../GlobalDef/Data/QuickChipData.h"
#include "QuickBuyChipLayer.h"
#include "LoadLayer.h"
#include "../EncryptAndDecrypt/Base64/base64.h"
#include "QuickBuyOtherChipLayer.h"
#include "SmileLayer.h"
#include "MallLayerDelegate.h"
#include "SimpleAudioEngine.h"
#include "NetConnectErrorLayer.h"
#include "RoomUserInfoLayer.h"
#include "SendChopMsgDelegate.h"

USING_NS_CC;

#define GAME_SCENE_LAYER_TAG 10

#define SEAT_START_TAG 11
//’º”√21~25
#define PUBLIC_CARD_TAG 21
#define HAND_CARD_TAG 30
#define STATUS_LABEL_TAG 40
#define USER_DESK_CHIP_TAG 50
#define SEAT_HAND_CHIP_TAG 80
#define ST_HDCP_L_TAG 90

//60~79÷Æº‰“—’º”√
#define LEFT_MENU_TAG 100
#define BTN_LEFT_MENU_TAG 101
#define SEAT_MENU_TAG 102

#define SELF_OPERATE_TAG 103
#define ADD_CHIP_TAG 104
#define OK_TAG 105
#define FOLLOW_CHIP_TAG 106
#define LABEL_FOLLOW_CHIP_TAG 107
#define LOOK_CARD_TAG 108
#define LEAST_CARD_TAG 109
#define OTHER_OPERATE_TAG 110
#define LOOK_OR_LEAST_CARD_TAG 111
#define AUTO_LOOK_CARD_TAG 112
#define FOLLOW_ANY_CHIP_TAG 113

#define SLIDER_NODE_TAG 114
#define NUM_SPRITE_TAG 115
#define NUM_LABEL_TAG 116
#define ADD_CHIP_SLIDER_TAG 117
//’º”√119~137
#define SEAT_CHIP_P_ST_TAG 118
//129~138
#define STCP_LN_ST_TAG 128
//139~148
#define STCP_L_ST_TAG 138
//’º”√151~159
#define STDKCP_ST_TAG 150

#define DESK_POOL_START_TAG 160
#define DKPL_LN_ST_TAG 168
#define DKPL_L_ST_TAG 176

#define EXIT_GAME_LAYER_TAG 186
#define ADD_CHIP_LAYER_TAG 187
#define HAS_NOT_ENOUGH_CHIP_LAYER_TAG 188
#define STAND_UP_LAYER_TAG 189
#define SMILE_COMM_MENU_TAG 190
#define COMMUNICATE_LAYER_TAG 191
#define COMMUNICATE_INPUT_LAYER_TAG 192
#define SMILE_LAYER_TAG 193
//’º”√201~209
#define SIT_PROGRESS_TAG 200
//211~219
#define QI_PAO_LABEL_START_TAG 210
//221~229
#define PLAYER_PHOTO_TAG 220
#define MALL_LAYER_TAG 230
#define BANKER_SPRITE_TAG 231
//241~249
#define PLAYER_SEAT_MENU_TAG 240
//251~259
#define PLAYER_SEAT_MENU_ITEM_TAG 250
//261~269
#define PLAYER_PHOTO_BACKGROUND_TAG 260
//271~279
#define SEAT_CHIP_LABEL_SPRITE_BG_TAG 270
#define STAND_UP_DISABLE_TAG 280
//291~299
#define ROOM_USER_INFO_LAYER_TAG 290
#define HAS_NOT_ENOUGH_CHOP_LAYER_TAG 300

typedef enum _chipValueType{	
	VALUE_ONE_TYPE = 1,					//√Ê÷µŒ™1
	VALUE_TWO_TYPE,						//√Ê÷µŒ™2
	VALUE_FIVE_TYPE,					//√Ê÷µŒ™5
	VALUE_TEN_TYPE,						//√Ê÷µŒ™10
	VALUE_TWENTY_TYPE,					//√Ê÷µŒ™20
	VALUE_FIFTY_TYPE,					//√Ê÷µŒ™50
	VALUE_ONE_HUNDRED_TYPE,				//√Ê÷µŒ™100
	VALUE_TWO_HUNDRED_TYPE,				//√Ê÷µŒ™200
	VALUE_FIVE_HUNDRED_TYPE,			//√Ê÷µŒ™500
	VALUE_ONE_THOUSAND_TYPE,			//√Ê÷µŒ™1000
	VALUE_TWO_THOUSAND_TYPE,			//√Ê÷µŒ™2000
	VALUE_FIVE_THOUSAND_TYPE,			//√Ê÷µŒ™5000
	VALUE_TEN_THOUSAND_TYPE,			//√Ê÷µŒ™10000
	VALUE_TWENTY_THOUSAND_TYPE,			//√Ê÷µŒ™20000
	VALUE_FIFTY_THOUSAND_TYPE,			//√Ê÷µŒ™50000
	VALUE_ONE_HUNDRED_THOUSAND_TYPE,	//√Ê÷µŒ™100000
	VALUE_TWO_HUNDRED_THOUSAND_TYPE,	//√Ê÷µŒ™200000
	VALUE_FIVE_HUNDRED_THOUSAND_TYPE,	//√Ê÷µŒ™500000
	VALUE_ONE_MILLION_TYPE,				//√Ê÷µŒ™1000000
	VALUE_TWO_MILLION_TYPE,				//√Ê÷µŒ™2000000
	VALUE_FIVE_MILLION_TYPE,			//√Ê÷µŒ™5000000
	VALUE_TEN_MILLION_TYPE,				//√Ê÷µŒ™10000000
	VALUE_TWENTY_MILLION_TYPE,			//√Ê÷µŒ™20000000
	VALUE_FIFTY_MILLION_TYPE,			//√Ê÷µŒ™50000000
	VALUE_ONE_HUNDRED_MILLION_TYPE,		//√Ê÷µŒ™100000000
	VALUE_TWO_HUNDRED_MILLION_TYPE,		//√Ê÷µŒ™200000000
	VALUE_FIVE_HUNDRED_MILLION_TYPE,	//√Ê÷µŒ™500000000
	VALUE_ONE_BILLION_TYPE,				//√Ê÷µŒ™1000000000
	VALUE_TWO_BILLION_TYPE,				//√Ê÷µŒ™2000000000
	VALUE_FIVE_BILLION_TYPE,			//√Ê÷µŒ™5000000000
	VALUE_TEN_BILLION_TYPE,				//√Ê÷µŒ™10000000000
	VALUE_TWENTY_BILLION_TYPE,			//√Ê÷µŒ™20000000000
	VALUE_FIFTY_BILLION_TYPE,			//√Ê÷µŒ™50000000000
	VALUE_ONE_HUNDRED_BILLION_TYPE,		//√Ê÷µŒ™100000000000
	VALUE_TWO_HUNDRED_BILLION_TYPE,		//√Ê÷µŒ™200000000000
	VALUE_FIVE_HUNDRED_BILLION_TYPE,	//√Ê÷µŒ™500000000000
}CHIP_VALUE_TYPE;

class WinStruct : public Ref{
public:
	WinStruct(){
		this->seatNum = 0;
		this->poolIdList.clear();
	}
	~WinStruct(){}

	CC_SYNTHESIZE(int, seatNum, SeatNum);
	std::vector<int> poolIdList;
};

class GameScene : public Layer, 
				  public NetDelegate, 
				  public ExitGameDelegate, 
				  public AddChipDelegate, 
				  public HasNotEnoughChipDelegate,
                  public HasNotEnoughChopDelegate,
				  public StandupDelegate, 
				  public CommunicateLayerDelegate,
                  public CommunicateInputDelegate,
                  public MallLayerDelegate,
                  public SendChopMsgDelegate{
public:
    ~GameScene();
                      
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(GameScene);

public:
	///µ±∂œø™¡¨Ω” ±µ˜”√
	virtual void onDisconnected();
	///µ±¡¨Ω”¥ÌŒÛ ±µ˜”√
	virtual void onConnectError();
	///µ±¡¨Ω”≥¨ ± ±µ˜”√
	virtual void onConnectTimeout();
	///µ±∂¡µΩœ˚œ¢ ±µ˜”√
	virtual void onMessage(char *buff, int len);

	virtual void update(float delta);

	void onHeartWaveUpdate(float delta);										//–ƒÃ¯
                      
    void onQuickBuyChipHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
                      
    void onHttpGetPhoto(HttpClient *sender, HttpResponse *response);

	
	virtual void onGoHall();													//∑µªÿ¥ÛÃ¸¥˙¿Ì∑Ω∑®
	virtual void onGameContinue();												//ºÃ–¯”Œœ∑¥˙¿Ì∑Ω∑®

	virtual void onBuyAndSit(int seatNum);										//π∫¬Ú◊¯œ¬¥˙¿Ì∑Ω∑®
	virtual void onCloseAddChipLayer();											//πÿ±’¥˙¿Ì∑Ω∑®

	virtual void onCloseHasNotEnoughChipLayer();								//πÿ±’≥Ô¬Î≤ª◊„∂‘ª∞øÚ
	virtual void onHasNotEnoughChip();											//≥Ô¬Î≤ª◊„£¨¡¢º¥«∞Õ˘¥˙¿Ì∑Ω∑®
                      
    virtual void onCloseHasNotEnoughChopLayer();                                //πÿ±’≥Ô¬Î≤ª◊„∂‘ª∞øÚ
    virtual void onHasNotEnoughChop();                                          //≥Ô¬Î≤ª◊„£¨¡¢º¥«∞Õ˘¥˙¿Ì∑Ω∑®

	virtual void onStandupLayerStandup();										//’æ∆≤Àµ•¥˙¿Ì∑Ω∑®
	virtual void onStandupLayerCloseLayer();									//ºÃ–¯”Œœ∑°¢πÿ±’≤Àµ•¥˙¿Ì∑Ω∑®

	virtual void onCommunicateCellTouched(int cellIndex);						//∂Ã”Ôµ•‘™—°÷–¥˙¿Ì∑Ω∑®£¨cellIndexŒ™±ª—°÷–µ•‘™µƒtagº”1£¨º¥¥”1ø™ º
    virtual void onCommunicateInputTouchedCallback();                           //shu ru kuang bei dian ji hou de hui diao fang fa
                      
    virtual void onCommunicateInputSendCallback(int type, std::string msg);     //dian ji fa song cai dan hui diao fang fa
                      
    virtual void onCloseMallLayerCallback();
                      
    virtual void onSendChopMsg(int seatNum, int chopIndex);
                      
protected:
	void drawSeat();															//ª≠◊˘Œª
	void drawPlayer(ROOM_USER_INFO_T *userInfo);								//ªÊ÷∆ƒ≥“ª∏ˆæﬂÃÂÕÊº“
	void drawAllInlinePlayers();												//ªÊ÷∆»´≤ø◊¯œ¬ÕÊº“
	void setSeatVisible();														//“˛≤ÿªÚœ‘ æ◊˘Œª
	void drawPublicCards();														//ªÊ÷∆π´π≤≈∆£¨÷ª‘⁄≥ı ºªØµƒ ±∫Úµ˜”√£¨ÕÊ≈∆π˝≥Ã÷–µƒªÊ÷∆‘⁄œ˚œ¢œÏ”¶÷–Ω¯––
	void drawUserStatus();														//ªÊ÷∆”√ªß◊¥Ã¨£∫0£∫≈‘π€  1£∫◊¯œ¬(µ»¥˝œ¬“ªæ÷)   2£∫ º”◊¢ 3£∫∏˙◊¢ 4£∫ø¥≈∆   5:∆˙≈∆  6:»´œ¬  7£∫µ»¥˝œ¬◊¢£¨∏˘æ›”√ªß◊¥Ã¨ªÊ÷∆ ÷≈∆°¢∆˙≈∆µ»
	void showStatus(int seatNum, int status);									//ªÊ÷∆æﬂÃÂ◊˘Œªƒ≥“ª◊¥Ã¨
	void showSeatHandChip(int seatNum, int status, long chipValue);				//ªÊ÷∆æﬂÃÂ◊˘ŒªÕÊº“ ÷…œ≥Ô¬Î
	void drawBankerAndBlind();													//œ‘ æ¥Û–°√§◊¢°¢◊Øº“
	void drawSitProgress();														//ªÊ÷∆µπº∆ ±
	void drawHandCardsForEnterRoom();											//ªÊ÷∆ ÷≈∆£¨≤ª¥¯∂Øª≠£¨”√”⁄Ω¯»Î∑øº‰ ±
	void drawHandCards();														//ªÊ÷∆ ÷≈∆£¨¥¯∂Øª≠
	void showStaticDeskChip(int seatNum, long deskChip);						//ªÊ÷∆ÕÊº“◊¿√Ê≥Ô¬Î£¨æ≤Ã¨œ‘ æ£¨µ´tag“™Õ≥“ª
	void showDeskChip(int seatNum, long seatDeskChipValue, long deskChip);		//ªÊ÷∆ÕÊº“≤Ÿ◊˜∫ÛµƒÃÌº”≥Ô¬Î∂Øª≠
	void drawOperateMenuAndHide();												//ªÊ÷∆≤Ÿ◊˜∞¥≈•≤¢Ω´∆‰“˛≤ÿ£¨”√‘⁄”Œœ∑≥°æ∞≥ı ºªØ ±£¨∆‰À¸ ±∫Úøÿ÷∆≤Ÿ◊˜∞¥≈•“˛≤ÿ∫Õœ‘ æ
	void showOperateMenu(int nextTurn);											//œ‘ æ≤Ÿ◊˜∞¥≈•
	void hideOperateMenu();														//“˛≤ÿ≤Ÿ◊˜∞¥≈•
    void hideAllOperateMenu();                                                  //隐藏所有操作按钮，winlist时使用
	void hideOkAndShowAddChipMenu();											//“˛≤ÿ»∑∂®≤Àµ•£¨œ‘ æº”◊¢≤Àµ•£¨“˛≤ÿº”◊¢ª¨∂ØÃı
	void drawAddChipSlider();													//ªÊ÷∆º”◊¢ª¨∂ØÃı
	void showSmileAndCommunicateMenu();											//œ‘ æ∫Õ“˛≤ÿ±Ì«È°¢øÏΩ›∂Ã”Ô≤Àµ•

	void sendOperateMessage(int ctrlRight, long betMoney);						//∑¢ÀÕ∏˙◊¢°¢º”◊¢°¢ø¥≈∆°¢∆˙≈∆œ˚œ¢∏¯∑˛ŒÒ∆˜
	void collectDeskChip(int roomSeatNum);										// ’ºØ◊¿√Êœ¬◊¢≥Ô¬ÎµΩ“ª∆
	void drawStaticPool(int id, long poolValue);								//ªÊ÷∆≥Ô¬Î≥ÿ£¨æ≤Ã¨œ‘ æ£¨µ´tag“™Õ≥“ª
	void drawPoolAnimation(int id, long poolValue);								//ªÊ÷∆≥Ô¬ÎπÈ≥ÿ∂Øª≠£¨idŒ™≥ÿ±‡∫≈0~6£¨poolValueŒ™≥ÿ≥Ô¬Î ˝

	void sendExitRoomMessage();													//∑¢ÀÕ¿Îø™∑øº‰œ˚œ¢∏¯∑˛ŒÒ∆˜
	void sendSitDownMessage(int seatNum);										//∑¢ÀÕ◊¯œ¬œ˚œ¢∏¯∑˛ŒÒ∆˜
	void sendLeaveSeatMessage(ROOM_USER_INFO_T *userInfo);						//∑¢ÀÕ¿Î◊˘œ˚œ¢∏¯∑˛ŒÒ∆˜
                      
    void hideSmileLayer();                                                      //yin chang biao qing ceng
    void hideCommunicateLayer();                                                //yin chang liao tian ceng
    void hideLeftMenu();                                                        //yin chang zuo bian cai dan

protected:
	///≤Àµ•ªÿµ˜∑Ω∑®
	void btMenuCallback(Ref *object);
	void btGoHallCallback(Ref *object);
	void btStandupCallback(Ref *object);
	void btMenuCloseCallback(Ref *object);
	void btShoppingCallback(Ref *object);
	void btChargeCallback(Ref *object);
	void btSmileCallback(Ref *object);
	void btCommunicateCallback(Ref *object);
	void btSitDownCallback(Ref *object);
	void btAddChipCallback(Ref *object);
	void btMenuOkCallback(Ref *object);
	void btFollowChipCallback(Ref *object);
	void btLookCardCallback(Ref *object);
	void btLeastCardCallback(Ref *object);
	void btLookOrLeastCardCallback(Ref *object);
	void btAutoLookCardCallback(Ref *object);
	void btFollowAnyChipCallback(Ref *object);
	void btAllInCallback(Ref *object);
    void btPlayerSeatMenuCallback(Ref *object);

	void valueChanged(Ref* object, Control::EventType type);					//ª¨∂ØÃıª¨∂Øœ˚œ¢œÏ”¶

	///∑˛ŒÒ∆˜œ˚œ¢¥¶¿Ì∑Ω∑®
	void onSitDown(char *buff, int len);										//◊¯œ¬
	void onAddChip(char *buff, int len);										//º”◊¢
	void onDrawCard(char *buff, int len);										//∑≠π´π≤≈∆
	void onLeaveSeat(char *buff, int len);										//¿Î◊˘
	void onDealCard(char *buff, int len);										//∑¢≈∆
	void onBlindSeat(char *buff, int len);										//√§◊¢◊˘Œª
	void onNextTurn(char *buff, int len);										//¬÷µΩœ¬“ª∏ˆ∂Ø◊˜µƒÕÊº“
	void onWinList(char *buff, int len);										//”Æ≈∆¡–±Ì
	void onGetSpeekWords(char *buff, int len);									//¡ƒÃÏœ˚œ¢
	void onExitRoom(char *buff, int len);										//¿Îø™∑øº‰
    void onGetChopMsg(char *buff, int len);                                     //道具消息
	void onReEnterTurnMe(char *buff, int len);									//Ω¯»ÎµΩŒ“≤Ÿ◊˜–≈œ¢£¨÷˜“™ «ªÒ»°timer_id
    void onGetErrorCode(char *buff, int len);                                   //接收到错误码
	void onCleanRoomStatus(char *buff, int len);								//“ªæ÷ÕÍ≥…«Â≥˝◊¿√Ê≥ÿ◊”°¢π´π≤≈∆°¢ ÷≈∆°¢◊Øº“Œª÷√
                      
    void displayOver();															//«Â¿Ì≈∆æ÷◊¿√Ê

	void calculateChipValueType(std::map<CHIP_VALUE_TYPE, int> *chipValueTypeMap, long long chipValue);	//º∆À„≥Ô¬Î√Ê÷µ∂‘”¶µƒ≥Ô¬Î¿‡–Õ∫Õ∏ˆ ˝¡–±Ì

protected:
	Size visibleSize;
	Vec2 visibleOrigin;

	float deltaScreenX;
	float deltaScreenY;

	Vector<WinStruct *> winArray;

	int sitProgressTagTurn;
    bool progressVolumnPlayed;          //san miao dao ji shi yin xiao shi fou yi jing kai shi bo fang
    int progressVolumnId;               //bo fang san miao dao ji shi shen yin fan hui de yin xiao Id
                      
    int seatDelta;                      //zuo wei pian yi, da yu ling wang you pian yi, xiao yu ling wang zuo pian yi
                      
    LoadLayer* loadLayer;
};

#endif