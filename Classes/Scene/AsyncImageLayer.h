#ifndef _ASYNC_IMAGE_LAYER_
#define _ASYNC_IMAGE_LAYER_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GlobalDef/GlobalDef.h"
#include "../GlobalDef/table/MallInfoTable.h"
#include "../GlobalDef/Data/MallInfoData.h"

#define DEFAULT_CHIP_IMAGE_FILE "mall_default_goods_%d.png"

USING_NS_CC;
USING_NS_CC_EXT;

class AsyncImageDelegate {
public:
	AsyncImageDelegate() {};
	~AsyncImageDelegate() {};

	virtual void clicked(MallInfoData* data) = 0;
};

class AsyncImageLayer : public Layer{
public:
	virtual bool init();
	static AsyncImageLayer* create(MallInfoData* data);
	CC_SYNTHESIZE(AsyncImageDelegate*, pDelegate, Delegate);
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

	MallInfoData* m_pData;
};

#endif