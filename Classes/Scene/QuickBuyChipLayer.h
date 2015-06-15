//
//  QuickBuyChipLayer.h
//  dzpkV3.2
//
//  Created by apple on 14-9-17.
//
//

#ifndef dzpkV3_2_QuickBuyChipLayer_h
#define dzpkV3_2_QuickBuyChipLayer_h

#include "cocos2d.h"
#include "LoadLayer.h"
#include "BasePopupBox.h"
#include "../GlobalDef/Data/MallInfoData.h"
#include "../GlobalDef/Data/UserInfo.h"
#include "QuickBuyOtherChipLayer.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "../Utils/IOSiAP.h"
#endif

USING_NS_CC;

class QuickBuyChipLayer : public BasePopupBox
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    , public IOSiAPDelegate
#endif
{
public:
    CREATE_FUNC(QuickBuyChipLayer);
    ~QuickBuyChipLayer();
    virtual bool init();
    
    void btnBuyChipOkMenuCallback(Ref *ref);
    void btnOtherChipMenuCallback(Ref *ref);
    void btnCloseMenuCallback(Ref *ref);
    
    // iap callback
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    virtual void onRequestProductsFinish(void);
    virtual void onRequestProductsError(int code);
    virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity, const char *bytes);
    
    void onPaymentPurchaedHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
#endif
    
protected:
    void showResultTips(bool buyChip, bool bSuccess = false);
    void endAction(Node* node);
    
protected:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSiAP *m_IAP;
#endif
    
    Size visibleSize;
    
    MallInfoData* infoData;
    
    LoadLayer* loadLayer;
};

#endif
