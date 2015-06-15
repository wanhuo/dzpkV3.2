#ifndef _DownHeadImageViewDelegate_
#define _DownHeadImageViewDelegate_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "../GlobalDef/table/MallInfoTable.h"
#include "ProductData.h"
//#include "../GlobalDef/Data/UserInfo.h"
//#include "../GlobalDef/Data/MallInfoData.h"
#define DEFAULT_CHIP_IMAGE_FILE "mall_default_goods_%d.png"

USING_NS_CC;
USING_NS_CC_EXT;

class DownHeadImageViewDelegate {
public:
	DownHeadImageViewDelegate() {};
	~DownHeadImageViewDelegate() {};

	virtual void clickByDown(ProductData* data) = 0;
};

class DownHeadImageView : public Layer{
public:
	virtual bool init();
	static DownHeadImageView* create(ProductData* data,int type=0);
	CC_SYNTHESIZE(DownHeadImageViewDelegate*, pDelegate, Delegate);
	CC_SYNTHESIZE(Rect, m_rect, viewRect);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

	void updateImage(const std::string& strFile);
	void removeAsyncImage();
    
    Sprite* getSpImage();

private:
	std::string getImageLocalFile();
	
private:
	Sprite* sp_image;
	Vec2 m_beginPoint;
	Vec2 m_endPoint;
	bool m_bDefaultPic;

	ProductData* m_pData;
};

#endif