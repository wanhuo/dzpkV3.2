#ifndef _ProductData_
#define _ProductData_

#include "cocos2d.h"
#include <iostream>
#include "../GlobalDef.h"
#include "../../GlobalDef/JsonWritter.h"

USING_NS_CC;
using namespace std;

class ProductData : public Ref {
public:
	static Vector<ProductData*> parseData(const string strData);
    static Vector<ProductData*> TestData();
 

    CC_SYNTHESIZE(int, paperIndex, paperIndex);             //这个是控制页面的Index
	CC_SYNTHESIZE(long, type, type);                        //道具类型／为拓展可能的道具分类
    CC_SYNTHESIZE(long, isSelect, isSelect);                //道具是否选中
    
    CC_SYNTHESIZE(int, _id, ID);                            //道具ID
    CC_SYNTHESIZE(string, buy_time, buy_time);              //道具购买的时间
    CC_SYNTHESIZE(int, mall_id, Mall_id);                   //道具ID
    CC_SYNTHESIZE(int, apply, Apply);                       //道具？
    CC_SYNTHESIZE(string, mall_name, Mall_name);            //道具名字
	CC_SYNTHESIZE(string, mall_path, Mall_path);            //道具图片地址
    CC_SYNTHESIZE(int, nowday, Nowday);                     //？
	CC_SYNTHESIZE(int, sale_chip, Sale_chip);               //道具？
    CC_SYNTHESIZE(int, source, Source);                     //道具？
    CC_SYNTHESIZE(string, starttime, starttime);            //道具开始时间
    CC_SYNTHESIZE(int, timeleft, Timeleft);                 //道具数量
    CC_SYNTHESIZE(int, usertype, usertype);                 //玩家类型
    
    CC_SYNTHESIZE(string, context, context);                //玩家类型
    
	ProductData();
	~ProductData() {};
};

#endif