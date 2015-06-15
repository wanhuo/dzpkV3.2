#include "SFScale9Sprite.h"
#include <iostream>

#define NEW_SFScale9Sprite  
#ifndef NEW_SFScale9Sprite
#endif

SFScale9Sprite::SFScale9Sprite()
{
}

SFScale9Sprite::~SFScale9Sprite()
{
}

bool SFScale9Sprite::initWithBatchNode(SpriteBatchNode* batchnode, Rect rect, Rect capInsets)
{
	
	CCAssert(batchnode != NULL, "The batchnode must be not nil.");
	
	scale9Image = batchnode;

	if ( rect.equals(Rect::ZERO) )
	{
		Size textureSize = scale9Image->getTextureAtlas()->getTexture()->getContentSize();
		
		rect = Rect(0, 0, textureSize.width, textureSize.height);
	}
	
	m_spriteRect = rect;
	m_originalSize = rect.size;
	m_preferredSize = m_originalSize;
	m_capInsets = capInsets;
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	
#ifndef NEW_SFScale9Sprite
    
#else
    float bound = 0;
#endif
	if ( m_capInsets.equals(Rect::ZERO) )
	{
		m_capInsets = Rect(
								rect.origin.x + m_originalSize.width / 3, 
								rect.origin.y + m_originalSize.height / 3, 
								m_originalSize.width / 3,
								m_originalSize.height / 3);
	}
#ifndef NEW_SFScale9Sprite
#else
    else {
        bound = 1.0 / Director::getInstance()->getContentScaleFactor();
    }
     
    if (m_capInsets.origin.x + m_capInsets.size.width + 0.1 > m_spriteRect.size.width)
    {
        m_capInsets = Rect(m_capInsets.origin.x / 2, m_capInsets.origin.y / 2, m_capInsets.size.width / 2, m_capInsets.size.height / 2);
    }
#endif
	
	float l = rect.origin.x;
	float t = rect.origin.y;
	float h = rect.size.height;
	float w = rect.size.width;

#ifndef NEW_SFScale9Sprite
#else
    l += bound;
    t += bound;
    h = std::max(0.0f, h - bound * 2);
    w = std::max(0.0f, w - bound * 2);
#endif

        
	centre = Sprite::createWithTexture(scale9Image->getTexture(), m_capInsets);
	scale9Image->addChild(centre ,0 ,pCentre);
	
	top = Sprite::createWithTexture(scale9Image->getTexture(), Rect(m_capInsets.origin.x,
																t,
																m_capInsets.size.width,
																m_capInsets.origin.y - t));
	scale9Image->addChild(top, 1, pTop);
	
	bottom = Sprite::createWithTexture(scale9Image->getTexture(), Rect( m_capInsets.origin.x,
																   m_capInsets.origin.y + m_capInsets.size.height,
																   m_capInsets.size.width,
																   h - (m_capInsets.origin.y - t + m_capInsets.size.height) ));
	scale9Image->addChild(bottom, 1, pBottom);
	
	left = Sprite::createWithTexture(scale9Image->getTexture(), Rect(
																 l,
																 m_capInsets.origin.y,
																 m_capInsets.origin.x - l,
																 m_capInsets.size.height) );
	scale9Image->addChild(left, 1, pLeft);
	
	right = Sprite::createWithTexture(scale9Image->getTexture(), Rect(
																  m_capInsets.origin.x + m_capInsets.size.width,
																  m_capInsets.origin.y,
																  w - (m_capInsets.origin.x - l + m_capInsets.size.width),
																  m_capInsets.size.height));
	scale9Image->addChild(right, 1, pRight);
	
	topLeft = Sprite::createWithTexture(scale9Image->getTexture(), Rect(
																	l,
																	t,
																	m_capInsets.origin.x - l,
																	m_capInsets.origin.y - t));

	scale9Image->addChild(topLeft, 2, pTopLeft);
	
	topRight = Sprite::createWithTexture(scale9Image->getTexture(), Rect(
																	 m_capInsets.origin.x + m_capInsets.size.width,
																	 t,
																	 w - (m_capInsets.origin.x - l + m_capInsets.size.width),
																	 m_capInsets.origin.y - t));
	
	scale9Image->addChild(topRight, 2, pTopRight);
	
	bottomLeft = Sprite::createWithTexture(scale9Image->getTexture(), Rect(
																	   l,
																	   m_capInsets.origin.y + m_capInsets.size.height,
																	   m_capInsets.origin.x - l,
																	   h - (m_capInsets.origin.y - t + m_capInsets.size.height)) );
	scale9Image->addChild(bottomLeft, 2, pBottomLeft);
	
	bottomRight = Sprite::createWithTexture(scale9Image->getTexture(), Rect(
																		m_capInsets.origin.x + m_capInsets.size.width,
																		m_capInsets.origin.y + m_capInsets.size.height,
																		w - (m_capInsets.origin.x - l + m_capInsets.size.width),
																		h - (m_capInsets.origin.y - t + m_capInsets.size.height)) );
	scale9Image->addChild(bottomRight, 2, pBottomRight);
	
	this->setContentSize(rect.size);
	this->addChild(scale9Image);
	
	return true;
}

void SFScale9Sprite::setContentSize(const Size &size)
{
	
	Node::setContentSize(size);
	setAnchorPoint(Point(0.5f,0.5f));
    
    float sizableWidth = size.width - topLeft->getContentSize().width - topRight->getContentSize().width;
    float sizableHeight = size.height - topLeft->getContentSize().height - bottomRight->getContentSize().height;
    float horizontalScale = sizableWidth/centre->getContentSize().width;
    float verticalScale = sizableHeight/centre->getContentSize().height;
    centre->setScaleX(horizontalScale);
    centre->setScaleY(verticalScale);
    float rescaledWidth = centre->getContentSize().width * horizontalScale;
    float rescaledHeight = centre->getContentSize().height * verticalScale;
    
    float leftWidth = bottomLeft->getContentSize().width;
    float bottomHeight = bottomLeft->getContentSize().height;
    
    bottomLeft->setAnchorPoint(Point(0,0));
    bottomRight->setAnchorPoint(Point(0,0));
    topLeft->setAnchorPoint(Point(0,0));
    topRight->setAnchorPoint(Point(0,0));
    left->setAnchorPoint(Point(0,0));
    right->setAnchorPoint(Point(0,0));
    top->setAnchorPoint(Point(0,0));
    bottom->setAnchorPoint(Point(0,0));
    centre->setAnchorPoint(Point(0,0));
    
    // Position corners
    bottomLeft->setPosition(Point(0,0));
    bottomRight->setPosition(Point(leftWidth+rescaledWidth,0));
    topLeft->setPosition(Point(0, bottomHeight+rescaledHeight));
    topRight->setPosition(Point(leftWidth+rescaledWidth, bottomHeight+rescaledHeight));
    
    // Scale and position borders
    left->setPosition(Point(0, bottomHeight));
    left->setScaleY(verticalScale);
    right->setPosition(Point(leftWidth+rescaledWidth,bottomHeight));
    right->setScaleY(verticalScale);
    bottom->setPosition(Point(leftWidth,0));
    bottom->setScaleX(horizontalScale);
    top->setPosition(Point(leftWidth,bottomHeight+rescaledHeight));
    top->setScaleX(horizontalScale);
    
    // Position centre
    centre->setPosition(Point(leftWidth, bottomHeight));
	
}
bool SFScale9Sprite::initWithFile(const char* file, Rect rect,  Rect capInsets)
{
	//CCAssert(file != NULL, "Invalid file for sprite");
    
    SpriteBatchNode *batchnode = SpriteBatchNode::create(file, 9);
    bool pReturn = this->initWithBatchNode(batchnode, rect, capInsets);
    return pReturn;
}

SFScale9Sprite* SFScale9Sprite::spriteWithFile(const char* file, Rect rect,  Rect capInsets)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithFile(file, rect, capInsets) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

bool SFScale9Sprite::initWithFile(const char* file, Rect rect)
{
    bool pReturn = this->initWithFile(file, rect, Rect::ZERO);

    return pReturn;
}
SFScale9Sprite* SFScale9Sprite::spriteWithFile(const char* file, Rect rect)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithFile(file, rect) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}


bool SFScale9Sprite::initWithFile(Rect capInsets, const char* file)
{
    bool pReturn = this->initWithFile(file, Rect::ZERO, capInsets);
    return pReturn;
}

SFScale9Sprite* SFScale9Sprite::spriteWithFile(Rect capInsets, const char* file)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithFile(file, capInsets) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

bool SFScale9Sprite::initWithFile(const char* file)
{
    bool pReturn = this->initWithFile(file, Rect::ZERO);
    return pReturn; 
	
}
SFScale9Sprite* SFScale9Sprite::spriteWithFile(const char* file)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithFile(file) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

bool SFScale9Sprite::initWithSpriteFrame(SpriteFrame* spriteFrame, Rect capInsets)
{
	CCAssert(spriteFrame != NULL, "Sprite frame must be not nil");
    
    SpriteBatchNode *batchnode = SpriteBatchNode::createWithTexture(spriteFrame->getTexture(), 9);
    bool pReturn = this->initWithBatchNode(batchnode, spriteFrame->getRect(), capInsets);
    return pReturn;
}

SFScale9Sprite* SFScale9Sprite::spriteWithSpriteFrame(SpriteFrame* spriteFrame, Rect capInsets)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithSpriteFrame(spriteFrame, capInsets) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
bool SFScale9Sprite::initWithSpriteFrame(SpriteFrame* spriteFrame)
{
	bool pReturn = this->initWithSpriteFrame(spriteFrame, Rect::ZERO);
    return pReturn;
}

SFScale9Sprite* SFScale9Sprite::spriteWithSpriteFrame(SpriteFrame* spriteFrame)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithSpriteFrame(spriteFrame) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}

bool SFScale9Sprite::initWithSpriteFrameName(const char* spriteFrameName, Rect capInsets)
{
	CCAssert(spriteFrameName != NULL, "Invalid spriteFrameName for sprite");
    
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    bool pReturn = this->initWithSpriteFrame(frame, capInsets);
    return pReturn;
}

SFScale9Sprite* SFScale9Sprite::spriteWithSpriteFrameName(const char* spriteFrameName, Rect capInsets)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithSpriteFrameName(spriteFrameName, capInsets) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
	
}

bool SFScale9Sprite::initWithSpriteFrameName(const char* spriteFrameName)
{
	bool pReturn = this->initWithSpriteFrameName(spriteFrameName, Rect::ZERO);
    return pReturn;
}

SFScale9Sprite* SFScale9Sprite::spriteWithSpriteFrameName(const char* spriteFrameName)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithSpriteFrameName(spriteFrameName) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
	
}

SFScale9Sprite* SFScale9Sprite::resizableSpriteWithCapInsets(Rect capInsets)
{
	SFScale9Sprite* pReturn = new SFScale9Sprite();
	if ( pReturn && pReturn->initWithBatchNode(scale9Image, m_spriteRect, capInsets) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
void SFScale9Sprite::setCapInsets(Rect capInsets)
{
	m_capInsets = capInsets;
}
Rect SFScale9Sprite::getCapInsets()
{
	return m_capInsets;
}

bool SFScale9Sprite::getIsOpacityModifyRGB()
{
	return m_bIsOpacityModifyRGB;
}


