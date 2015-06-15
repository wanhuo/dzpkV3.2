#ifndef __MARCODEFINE__H__
#define __MARCODEFINE__H__

//#define CONNECT_168

#include "cocos-ext.h"
#include "cocos2d.h"
#include<string>
using namespace std;
USING_NS_CC;
const unsigned int MAX_CHAT_LEN = 128;
//消息类型========================
const unsigned short SMT_INFO = 0x0001;
const unsigned short SMT_EJECT = 0x0002;
const unsigned short SMT_GLOBAL = 0x0004;
const unsigned short SMT_PROMPT = 0x0008;
const unsigned short SMT_CLOSE_GAME = 0x0010;
const unsigned short SMT_TABLE_ROLL = 0x0020;
const unsigned short SMT_LESS_SCORE = 0x0040;
const unsigned short SMT_CLOSE_ROOM = 0x0080;
const unsigned short SMT_INTERMIT_LINE = 0x0100;
const unsigned short SMT_RE_LOGON = 0x0200;
enum matchSignUpRes
{
	MATCH_SIGNUP_SUCCESS = 0,//报名成功
	MATCH_FAIL_1,//你的账号出现异常，不能进行比赛
	MATCH_FAIL_2,//不能重复报名
	MATCH_FAIL_3,//筹码不足
	MATCH_FAIL_4,//积分不足
	MATCH_FAIL_5//人数已满
};
const unsigned short HEAD_URL_LEN = 128;
const unsigned int T_JINBIAO = 0x0004;//锦标赛类型 
const unsigned int T_TAOTAI = 0x0010;//淘汰赛类型
enum matchStatus
{
	S_SIGNUP_ING = 1,//报名状态
	S_PEOPLE_FULL = 3,//人数已满 
	S_PLAY = 4,//进行状态
	S_OVER = 5//结束状态
};
enum matchSignUpMode
{
	MATCH_NEED_CHIP = 0,//需要筹码
	MATCH_NEED_SCORE,//需要积分
	MATCH_NEED_VIP,//需要VIP
	MATCH_NEED_WIN//需要冠军
};
#define LOGINSTATE 1  //登陆状态
#define LOADSTATE  2  //加载状态
#define INITSTATE  3  //初始状态
#define LOBBYSTATE 4  //大厅状态
#define ROOMSTATE  5  //房间状态
#define LOGINSERVER "loginServer"  //登陆服务器
#define ROOMSERVER "roomServer"   //房间服务器
#define GAMEVERSION 131073     //游戏版本号
#define BOY 1
#define GIRL 2
//=====================玩家状态==============================
#ifndef NULL
#define NULL 0x00
#endif // !NULL	
#define US_FREE 0x01		//站立状态	
#define US_SIT 0x02			//坐下状态	
#define US_READY 0x03		//同意状态
#define US_LOOKON 0x04		//旁观状态	
#define US_PLAY 0x05		//游戏状态	
#define US_OFFLINE 0x06		//断线状态
#define ROLE_DEFAULT_ICON "headerIcon"  //玩家默认头像图标
#define NUM_LEN							16									//数字长度
#define NAME_LEN						60									//昵称长度
#define PASS_LEN                        33									//密码长度
#define CONTENT_LEN						72									//描述长度
#define DESC_LEN						128									//任务描述长度
#define MAX_ACHIEVEMENT					29									//成就个数
//支付方式定义
#define PAY_TYPE_QBI					0x0001								//Q币支付
#define PAY_TYPE_CHOUMA					0x0002								//筹码支付
#define PAY_TYPE_JIFEN					0x0004								//积分支付
#define PAY_TYPE_						0x0008								//
//===================================================数据类定义=================================================
#define USERPROXY "UserProxy"
#define LOBBYPROXY "LobbyProxy"
#define RANKPROXY "RankProxy"
#define MSGPROXY "MsgProxy"
#define GAMEPROXY "GameProxy"
#define ROOMPROXY "RoomProxy"
#define BETTINGPOOLPROXY "BettingPoolProxy"
#define SPREEPROXY "SpreeProxy"
#define PROPSPROXY "PropsProxy"
#define ACHIEVEPROXY "AchieveProxy"
const string MATCHPROXY("MatchProxy");
//==========================================房间服务定义================================================
#define COMMON_LOBBY 2   //普通场
#define MATCH_LOOBY 1    //比赛场
#define MATCH_LEVEL_1 6  //锦标赛
#define MATCH_LEVEL_2 7  //淘汰赛
#define COMMON_LEVEL_1 1   //新手场
#define COMMON_LEVEL_2 2   //初级场
#define COMMON_LEVEL_3  3  //中级场
#define COMMON_LEVEL_4  4  //高级场
#define COMMON_LEVEL_5  5 //快速场 
#define SERVER_LEN						32									//房间长度
#define HEAD_PATH_LEN					128									//头像路径长度
#define TIME_LEN						20									//时间字符串长度
#define GUID_LEN						64									//GUID长度
#define JSON_TITLE_LEN					60									//Json系统消息标题
#define JSON_CONTENT_LEN				500									//Json系统消息内容
#define SERVER_ADDRESS_LEN              256                                 //服务器地址
//===================换房间服务器类型===================
enum ConnectRoomType
{
	FAST_START_TYPE = 1,
	COMMON_SERVER_TYPE,   //普通场切换
	LOOK_ENTER_TABLE     //进入桌子
};
#define GAME_PLAYER						9									//游戏人数
#define MAX_COUNT						2									//最大数目
#define MAX_CENTERCOUNT					5									//最大数目
enum LoginType
{
	YOUKE_LOGIN_TYPE = 3,
	QQ_LOGIN_TYPE = 1,
	WEIBO_LOGIN_TYPE
};
enum PlayRsType
{
	PLAY_MOVIE_NULL = 0,
	PLAY_MOVIE_GAMESTART,
	PLAY_MOVIE_ADDCHIP,
	PLAY_MOVIE_GIVEUP,
	PLAY_MOVIE_SENDCARD,
	PLAY_MOVIE_GAMEEND
};
//无效数值
#define INVALID_BYTE				((unsigned char)(0xFF))						//无效数值
#define INVALID_WORD				((unsigned short)(0xFFFF))					//无效数值
#define INVALID_DWORD				((unsigned int)(0xFFFFFFFF))				//无效数值

//////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_SERVER				INVALID_WORD						//无效服务器
#define INVALID_TABLE				INVALID_WORD						//无效桌子
#define INVALID_CHAIR				INVALID_WORD						//无效椅子
//游戏操作状态
#define GO_NO						0x00								//无状态
#define GO_ADD_SOCRE				0x01								//加注
#define GO_GIVE_UP					0x02								//弃牌
#define GO_ALL_IN					0x03								//All in
#define GO_FOLLOW					0x04								//跟注
#define GO_LOOK						0x05								//看牌
#define GO_OPERATOR					0x06								//操作中
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define CT_SINGLE					1									//单牌类型
#define CT_ONE_LONG					2									//对子类型
#define CT_TWO_LONG					3									//两对类型
#define CT_THREE_TIAO				4									//三条类型
#define	CT_SHUN_ZI					5									//顺子类型
#define CT_TONG_HUA					6									//同花类型
#define CT_HU_LU					7									//葫芦类型
#define CT_TIE_ZHI					8									//铁支类型
#define CT_TONG_HUA_SHUN			9									//同花顺型
#define CT_KING_TONG_HUA_SHUN		10									//皇家同花顺
//数目定义
#define FULL_COUNT					52									//全牌数目
//数组维数
#define CountArray(array) (sizeof(array)/sizeof(array[0]))
enum CardColor
{
	CARDCOLOR_FANG = 0, //方块
	CARDCOLOR_MEI = 16, //梅花
	CARDCOLOR_HONG = 32,//红桃
	CARDCOLOR_HEI = 48  //黑桃
};
#define TIME_LEN				    20									//时间字符串长度
#define DWORD_LEN   4
#define WORD_LEN 2
#define LONGLONG_LEN 8
enum TimeSpreeID
{
	//跟服务器对应，不能改变
	TIMESPREE_1 = 1,
	TIMESPREE_2,
	TIMESPREE_3,
	TIMESPREE_4,
	TIMESPREE_5
};
enum TimeSpreeSatus
{
	TIMESPREE_INACTIVE = 0,       //时间未到，未激活的
	TIMESPREE_NO_RECEIVED = 2,    //已激活，未领取奖励的
	TIMESPREE_RECEIVED = 1        //激活，已领取过奖励的
};
#define NUMOFSPREE 9              //礼包里可装的数量
#define MAX_ACHIEVEMENT	 29		  //成就个数
#endif