#ifndef __SFScale9Sprite_H__
#define __SFScale9Sprite_H__

//
//¾Å¹¬¸ñ¸Ä±à°æ±¾
//
//
//
#include "cocos2d.h"
using namespace cocos2d;

enum positions
{
	pCentre = 0,
	pTop,
	pLeft,
	pRight,
	pBottom,
	pTopRight,
	pTopLeft,
	pBottomRight,
	pBottomLeft
};

class SFScale9Sprite : public __NodeRGBA
{
public:
	SFScale9Sprite(void);
	virtual ~SFScale9Sprite(void);

	public:
	CC_SYNTHESIZE(Size, m_originalSize, OriginalSize);
	CC_SYNTHESIZE(Size, m_preferredSize, PreferredSize);
	CC_PROPERTY(Rect, m_capInsets, CapInsets);
	
	
protected:
	Rect m_spriteRect;
    
    SpriteBatchNode* scale9Image;
    Sprite* topLeft;
    Sprite* top;
    Sprite* topRight;
    Sprite* left;
    Sprite* centre;
    Sprite* right;
    Sprite* bottomLeft;
    Sprite* bottom;
    Sprite* bottomRight;
	
	Color3B m_sColorUnmodified;
	bool m_bIsOpacityModifyRGB;
	
public:
	
	void virtual setContentSize(const Size &size);
	
	virtual bool initWithBatchNode(SpriteBatchNode* batchnode,Rect rect, Rect capInsets);
	virtual bool initWithFile(const char* file, Rect rect,  Rect capInsets);
	
	static SFScale9Sprite* spriteWithFile(const char* file, Rect rect,  Rect capInsets);	
	
	virtual bool initWithFile(const char* file, Rect rect);
	
	static SFScale9Sprite* spriteWithFile(const char* file, Rect rect);
	
	virtual bool initWithFile(Rect capInsets, const char* file);
	
	static SFScale9Sprite* spriteWithFile(Rect capInsets, const char* file);
	
	virtual bool initWithFile(const char* file);
	
	static SFScale9Sprite* spriteWithFile(const char* file);
	
	virtual bool initWithSpriteFrame(SpriteFrame* spriteFrame, Rect capInsets);
	
	static SFScale9Sprite* spriteWithSpriteFrame(SpriteFrame* spriteFrame, Rect capInsets);	

	virtual bool initWithSpriteFrame(SpriteFrame* spriteFrame);
	
	static SFScale9Sprite* spriteWithSpriteFrame(SpriteFrame* spriteFrame);	
	
	virtual bool initWithSpriteFrameName(const char*spriteFrameName, Rect capInsets);
	
	static SFScale9Sprite* spriteWithSpriteFrameName(const char*spriteFrameName, Rect capInsets);	
	
	virtual bool initWithSpriteFrameName(const char*spriteFrameName);
	
	static SFScale9Sprite* spriteWithSpriteFrameName(const char*spriteFrameName);
	
	SFScale9Sprite* resizableSpriteWithCapInsets(Rect capInsets);
	
	
	virtual bool getIsOpacityModifyRGB(void);
    
};

#endif

