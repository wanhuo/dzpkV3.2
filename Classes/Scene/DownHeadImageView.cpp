#include  "DownHeadImageView.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../Utils/DBUtil.h"
#include "../EncryptAndDecrypt/Md5/md5.h"
#include "DownHeadImage.h"

void DownHeadImageView::removeAsyncImage()
{
	DownHeadImage* pAsyncImg = DownHeadImage::getInstance();
	pAsyncImg->getData().erase(m_pData->getMall_path());
}

DownHeadImageView* DownHeadImageView::create(ProductData* data,int type)
{
	DownHeadImageView* pRet = new DownHeadImageView();
	if (pRet && (pRet->m_pData = data) && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool DownHeadImageView::init(){
	if(!Layer::init()){
		return false;
	}

	m_bDefaultPic = true;
	std::string imageFile = getImageLocalFile();
	if (imageFile.compare(DEFAULT_CHIP_IMAGE_FILE) == 0)
	{
		sp_image = Sprite::create();
		Animation* animation = Animation::create();
		for (int i=1; i<13; i++)
		{
			char strFile[256];
			sprintf(strFile, DEFAULT_CHIP_IMAGE_FILE, i);
			animation->addSpriteFrameWithFile(strFile);
		}
		animation->setDelayPerUnit(2.0 / 12);
		animation->setRestoreOriginalFrame(true);
		animation->setLoops(-1);
		Animate* animate = Animate::create(animation);
		sp_image->runAction(Sequence::create(animate, NULL));
	}
	else
	{
		sp_image = Sprite::create(imageFile.c_str());
		sp_image->setPosition(0, 0);
	}
	this->addChild(sp_image);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(DownHeadImageView::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(DownHeadImageView::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(DownHeadImageView::onTouchEnded,this);
	listener->setSwallowTouches(true);//�����´��ݴ���
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}

Sprite* DownHeadImageView::getSpImage(){
    if(sp_image != NULL){
        return sp_image;
    }
    
    return NULL;
}

void DownHeadImageView::updateImage(const std::string& strFile)
{
	if (sp_image)
	{
		sp_image->removeFromParent();
		sp_image = Sprite::create(strFile.c_str());
        
        if(sp_image){
        float scale=GlobalDef::getInstance()->getHeadImageScale();
		sp_image->setPosition(0, 0);
        sp_image->setScale(scale);
		this->addChild(sp_image);
        }

		m_bDefaultPic = false;
	}
}

std::string DownHeadImageView::getImageLocalFile()
{
	if (!m_pData
		|| m_pData->getMall_path().length() <= 0)
	{
		return std::string(DEFAULT_CHIP_IMAGE_FILE);
	}
    
	DBUtil* pDB = DBUtil::getInstance();
	MallInfoTable mall_data;
	std::string strPath;
	MallInfoTable* table_data = NULL;
	std::string mall_path = m_pData->getMall_path();
	std::string strsql = std::string("select mall_path, local_path from mall_info where mall_path = '") +  mall_path + std::string("'");
	pDB->getDataInfo(strsql, &mall_data);
	if (mall_data.m_datas.size() <= 0)	// ��һ�λ�ȡͼƬ����Ҫ�������ݿ⣬�����첽����ͼƬ
	{
		strsql = std::string("insert into mall_info (mall_path, local_path) values('") + mall_path + std::string("', '')");
		pDB->insertData(strsql);
		strPath = "";
	}
	else
	{
		table_data = (MallInfoTable*)mall_data.m_datas.at(0);	
		strPath = FileUtils::getInstance()->getWritablePath() + table_data->getlocalPath();
	}
	
	if (table_data
		&& table_data->getlocalPath().length() > 0
		&& FileUtils::getInstance()->isFileExist(strPath))
	{
		m_bDefaultPic = false;
		return strPath;
	}
	
	// first get image data from network
	DownHeadImage* pAsyncImg = DownHeadImage::getInstance();
	pAsyncImg->getData().insert(mall_path, this);
	pAsyncImg->getImage(mall_path);
		
	// second return default image file
	return std::string(DEFAULT_CHIP_IMAGE_FILE);
}

bool DownHeadImageView::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_beginPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (m_rect.containsPoint(m_beginPoint))
	{
		return true;
	}

	return false;
}

void DownHeadImageView::onTouchMoved(Touch *touch, Event *unused_event)
{
	
}

void DownHeadImageView::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	if (fabs(m_endPoint.x - m_beginPoint.x) < 10.0
		&& fabs(m_endPoint.y - m_beginPoint.y) < 10.0)
	{
		if (getDelegate())
		{
			getDelegate()->clickByDown(m_pData);
		}
	}	
}