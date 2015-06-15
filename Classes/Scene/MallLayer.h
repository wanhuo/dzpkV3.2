#ifndef _MALL_LAYER_
#define _MALL_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "../GlobalDef/Data/MallInfoData.h"
#include "../GlobalDef/Data/RoomData.h"
#include "PagePaneLayer.h"
#include "LoadLayer.h"
#include "AsyncImageLayer.h"
#include "MallBuyInfoLayer.h"
#include "BasePopupBox.h"
#include "MallLayerDelegate.h"

using namespace network;
USING_NS_CC;
USING_NS_CC_EXT;

enum MALL_INFO_FLAG
{
	MALL_PRESENT = 0,	// 礼物
	MALL_CHIP,			// 筹码
	MALL_VIP,
	MALL_PROP
};

class MallLayer : public BasePopupBox 
				, public PagePaneDelegate
				, public AsyncImageDelegate {
public:
    static MallLayer* create(MALL_INFO_FLAG mallInfoFlag = MALL_CHIP);
                    
	virtual bool init();

	void btnCloseMallMenuCallback(Ref *object);
	void btnBuyGoodsMenuCallback(Ref *object);
	virtual void clicked(int nRoom);
	virtual void clicked(MallInfoData* data);
                    
    void setMallLayerDelegate(MallLayerDelegate* mallLayerDelegate);

protected:
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);  // 用户登陆回调方法

	void getMallInfoDatas(MALL_INFO_FLAG nFlag);
	void createGoods(int nIndex);
	void removeGoodsLayer(bool bDeleteLayer = true);

protected:
	Size visibleSize;
	Vec2 visibleOrigin;

private:
	int m_nCurMall;
	LoadLayer* loadLayer;
	Sprite* goodsLayer;
	Sprite* goodsTips;
	Vector<MallInfoData*> m_mallChipDatas;
	Vector<MallInfoData*> m_mallDatas;
                    
    Sprite* mall_bottom_frame;
    Sprite* mall_middle_frame;
                    
    MALL_INFO_FLAG mallInfoFlag;
                    
    MallLayerDelegate* mallLayerDelegate;
};

#endif