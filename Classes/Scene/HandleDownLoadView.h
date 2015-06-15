#ifndef _HandleDownLoadViewDelegate_
#define _HandleDownLoadViewDelegate_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "../GlobalDef/table/MallInfoTable.h"
#include "ProductData.h"
//#include "../GlobalDef/Data/MallInfoData.h"
#define DEFAULT_CHIP_IMAGE_FILE "mall_default_goods_%d.png"

USING_NS_CC;
USING_NS_CC_EXT;

class HandleDownLoadViewDelegate {
public:
	HandleDownLoadViewDelegate() {};
	~HandleDownLoadViewDelegate() {};

	virtual void clicked(ProductData* data) = 0;
};

class HandleDownLoadView : public Layer{
public:
	virtual bool init();
	static HandleDownLoadView* create(ProductData* data,int type=0);
	CC_SYNTHESIZE(HandleDownLoadViewDelegate*, pDelegate, Delegate);
	CC_SYNTHESIZE(Rect, m_rect, viewRect);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

	void updateImage(const std::string& strFile);
	void removeAsyncImage();

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