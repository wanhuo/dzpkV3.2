#include "PreloadScene.h"
#include "LoginScene.h"
#include "../GlobalDef/GlobalChineseXml.h"
#include "../GlobalDef/GlobalDef.h"

Scene* PreloadScene::createScene(){
	auto scene = Scene::create();

	auto layer = PreloadScene::create();
	scene->addChild(layer);

	return scene;
}

bool PreloadScene::init(){
	if(!Layer::init()){
		return false;
	}

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

	Sprite* bg_main = Sprite::create("bg_game.png");
	bg_main->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg_main);

	Sprite* sp_login_logo = Sprite::create("sp_login_logo.png");
	sp_login_logo->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(sp_login_logo);

	DelayTime* delayAction = DelayTime::create(1.5f);

	CallFunc* callFunc = CallFunc::create([](){
		Scene *scene = LoginScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});

	sp_login_logo->runAction(Sequence::create(delayAction, callFunc, NULL));

	// 打开数据库
	DBUtil* pDB = DBUtil::getInstance();
	if (pDB){
        std::string fullPath = FileUtils::getInstance()->getWritablePath();
        
        std::string name="dzpk.db";
        
        fullPath = fullPath+name;
        
        pDB->initDB(fullPath.c_str());
    }

	return true;
}