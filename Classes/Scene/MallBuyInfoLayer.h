#ifndef _MALL_BUY_INFO_LAYER_
#define _MALL_BUY_INFO_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BasePopupBox.h"
#include "LoadLayer.h"
#include "../GlobalDef/Data/MallInfoData.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "../Utils/IOSiAP.h"
#endif

using namespace network;
USING_NS_CC;

enum MALL_BUY_INFO_FLAG
{
	MALL_BUY_INFO_CHIP = 1,
	MALL_BUY_INFO_OTHERS
};

class MallBuyInfoLayer : public BasePopupBox
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                       , public IOSiAPDelegate
#endif
{
public:
    ~MallBuyInfoLayer();
	virtual bool init();
	static MallBuyInfoLayer* create(MALL_BUY_INFO_FLAG nFlag, const std::string& value1, const std::string& value2);
	CC_SYNTHESIZE(MallInfoData*, pData, Data);

protected:
	void btnBuyOKMenuCallback(Ref *object);
	void btnBuyCloseMenuCallback(Ref *object);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

    // iap callback
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    virtual void onRequestProductsFinish(void);
    virtual void onRequestProductsError(int code);
    virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity, const char *bytes);
    
    void onPaymentPurchaedHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
#endif
    
	void buyProp();
    void buyChip();
	void showResultTips(bool buyChip, bool bSuccess = false);
	void endAction(Node* node);

protected:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSiAP *m_IAP;
#endif
	float action_duration;
	MALL_BUY_INFO_FLAG m_nFlag;
	std::string m_value1;
	std::string m_value2;
	LoadLayer* loadLayer;
	Size visibleSize;
};


#endif