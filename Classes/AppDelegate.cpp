#include "AppDelegate.h"
#include "Scene/LoginScene.h"
#include "Scene/AsyncImage.h"
#include "GlobalDef/Data/RoomData.h"
#include "GlobalDef/Data/UserInfo.h"
#include "GlobalDef/GlobalChineseXml.h"
#include "GlobalDef/GlobalDef.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {
	DBUtil* pDB = DBUtil::getInstance();
	pDB->closeDB();
	DBUtil::release();
	AsyncImage::destoryInstance();
    RoomData::removeInstance();
    UserInfo::removeInstance();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("dzpk");
        director->setOpenGLView(glview);
    }
//	glview->setFrameSize(800, 561);
//	glview->setFrameSize(1136, 640);
//	glview->setFrameSize(640, 480);
	auto viewSize = glview->getFrameSize();
	if((viewSize.width * 1.0 / viewSize.height) >= (960.0 / 640.0)){
		glview->setDesignResolutionSize(960, 640, ResolutionPolicy::FIXED_WIDTH);
	}
	else{
		glview->setDesignResolutionSize(960, 640, ResolutionPolicy::FIXED_HEIGHT);
	}

    // turn on display FPS
//  director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    //初始化
    GlobalChineseXml::getInstance()->parseFile("chinese.xml");
    
	visibleSize = Director::getInstance()->getVisibleSize();
	visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
	GlobalDef::getInstance()->setDeltaScreenX((960 - visibleSize.width) / 2);
	GlobalDef::getInstance()->setDeltaScreenY((640 - visibleSize.height) / 2);
    auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    if((frameSize.width / frameSize.height) > (960.0 / 640.0)){
        if(frameSize.width > 960){
			GlobalDef::getInstance()->setFrameScaleX(frameSize.width * 1.0 / 960.0);
		}
		else{
			GlobalDef::getInstance()->setFrameScaleX(960.0 / (frameSize.width * 1.0));
		}
    }
    else{
        GlobalDef::getInstance()->setFrameScaleX(1.0f);
    }
    
    // ¥Úø™ ˝æ›ø‚
	DBUtil* pDB = DBUtil::getInstance();
	if (pDB){
        std::string fullPath = FileUtils::getInstance()->getWritablePath();
        
        std::string name="dzpk.db";
        
        fullPath = fullPath+name;
        
        pDB->initDB(fullPath.c_str());
    }
    
    //Ω´”√ªßÕ¯¬ÁÕ∑œÒÃÌº”µΩspriteframecache
    HttpRequest* request = new HttpRequest();
    request->setUrl("http://1251038220.cdn.myqcloud.com/1251038220/mobile/dzpk_setting.xml");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(AppDelegate::onHttpGetServerSetting, this));
    
    //ø…“‘Õ®π˝tag±Í ∂”√ªßID
    request->setTag("get server setting");
    HttpClient::getInstance()->send(request);
    request->release();

    // create a scene. it's an autorelease object
//    auto scene = LoginScene::createScene();

    // run
//    director->runWithScene(scene);

    return true;
}

void AppDelegate::onHttpGetServerSetting(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
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
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
	std::string encodeRecvData; //Ω” ’µΩµƒbase64º”√‹ ˝æ›
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        printf("%c", (*buffer)[i]);
		encodeRecvData.push_back((*buffer)[i]);
    }
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse(encodeRecvData.c_str());
    
//    httpSeverAddress="http://203.195.153.199/hallroom/jsonServlet" socketIp="203.195.138.89" cdnAddress="http://1251038220.cdn.myqcloud.com/1251038220/mobile/"
    
	auto root = doc->RootElement();
	for(auto e = root->FirstAttribute(); e; e = e->Next()){
		const char* name = e->Name();
		const char* value = e->Value();
        if (strcmp(name, "httpSeverAddress") == 0) {
            GlobalDef::getInstance()->setHttpServerAddress(value);
//            GlobalDef::getInstance()->setHttpServerAddress("http://192.168.1.101/hallroom/jsonServlet");
        }
        else if (strcmp(name, "socketIp") == 0) {
            GlobalDef::getInstance()->setSocketIp(value);
//            GlobalDef::getInstance()->setSocketIp("192.168.1.101");
        }
        else if (strcmp(name, "cdnAddress") == 0) {
            GlobalDef::getInstance()->setCDNAddress(value);
        }
	}
    delete doc;
    
    // create a scene. it's an autorelease object
    auto scene = LoginScene::createScene();
    
    // run
    Director::getInstance()->runWithScene(scene);
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
