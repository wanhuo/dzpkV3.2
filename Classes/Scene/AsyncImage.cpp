#include "AsyncImage.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../Utils/DBUtil.h"
#include "../EncryptAndDecrypt/Md5/md5.h"
#include "AsyncImageLayer.h"

static AsyncImage* g_asyncImg =  nullptr;

AsyncImage* AsyncImage::getInstance()
{
	if (!g_asyncImg)
	{
		g_asyncImg = new AsyncImage();
	}
	return g_asyncImg;
}

void AsyncImage::destoryInstance()
{
	if (g_asyncImg)
	{
		delete g_asyncImg;
	}
	g_asyncImg = nullptr;
}

void AsyncImage::getImage(const std::string& strUrl)
{
	HttpRequest* request = new HttpRequest();
	std::string strURL = std::string(DEFAULT_GOODS_URL) + strUrl;
	request->setUrl(strURL.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(AsyncImage::onHttpRequestCompleted, this));
	HttpClient::getInstance()->send(request);
	request->release();
}

void AsyncImage::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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
	AsyncImageLayer* pLayer = m_data.at(strUrl);
	if (pLayer)
	{
		pLayer->updateImage(strPath);
	}
}