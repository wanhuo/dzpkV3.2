#include "DownHeadImage.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../Utils/DBUtil.h"
#include "../EncryptAndDecrypt/Md5/md5.h"
#include "DownHeadImageView.h"

static DownHeadImage* g_asyncImg =  nullptr;

DownHeadImage* DownHeadImage::getInstance()
{
	if (!g_asyncImg)
	{
		g_asyncImg = new DownHeadImage();
	}
	return g_asyncImg;
}

void DownHeadImage::destoryInstance()
{
	if (g_asyncImg)
	{
		delete g_asyncImg;
	}
	g_asyncImg = nullptr;
}

void DownHeadImage::getImage(const std::string& strUrl)
{
    
    HttpRequest* request = new HttpRequest();
	std::string strURLNew=std::string(DEFAULT_GOODS_URL) + strUrl;
	request->setUrl(strURLNew.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(DownHeadImage::onHttpRequestCompleted, DownHeadImage::getInstance()));
	HttpClient::getInstance()->send(request);
	request->release();
}

void DownHeadImage::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (!response->isSucceed()) 
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// get AsyncImageLayer
	std::string strUrl = response->getHttpRequest()->getUrl();
	strUrl = strUrl.replace(0, strlen(DEFAULT_GOODS_URL), "");

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	unsigned char* pData = new unsigned char[buffer->size()];
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		pData[i] = (*buffer)[i];
	}
	
	// save to image file
	Image* pImage = new Image();
	pImage->initWithImageData(pData, buffer->size());
	std::string mall_path = strUrl;
	int nPos = mall_path.find(".");
	std::string strPath = FileUtils::getInstance()->getWritablePath() + MD5(mall_path).md5() + mall_path.substr(nPos, mall_path.length()-nPos);
	pImage->saveToFile(strPath, false);
	delete pImage;
	pImage = NULL;
	delete[] pData;
	pData = NULL;

	// update sqlite
	DBUtil* pDB = DBUtil::getInstance();
	MallInfoTable mall_data;
	std::string strsql = std::string("update mall_info set local_path = '") 
							+ MD5(mall_path).md5() + mall_path.substr(nPos, mall_path.length()-nPos)
							+ std::string("' where mall_path = '") + mall_path + std::string("'");
	pDB->updateData(strsql);

	// update sprite image
	DownHeadImageView *pLayer = m_data.at(strUrl);
	if (pLayer)
	{
		pLayer->updateImage(strPath);
	}
}