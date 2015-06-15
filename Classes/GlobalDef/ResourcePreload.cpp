#include "ResourcePreload.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

static ResourcePreload* resourcePreload = nullptr;

ResourcePreload* ResourcePreload::getInstance(){
	if(resourcePreload == NULL){
		resourcePreload = new ResourcePreload();
	}

	return resourcePreload;
}

ResourcePreload::ResourcePreload(){
	
}

ResourcePreload::~ResourcePreload(){

}

void ResourcePreload::preload(){
	preloadSpriteFrame();
	preloadAudio();
}

void ResourcePreload::addSpriteFrame(SpriteFrame *spriteFrame, char *key){
	SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, key);
}

void ResourcePreload::preloadSpriteFrame(){
	SpriteFrame* sp_clip = SpriteFrame::create("sp_clip.png", Rect(0, 0, 32, 33));
	SpriteFrameCache::getInstance()->addSpriteFrame(sp_clip, "sp_clip");

	SpriteFrame* sp_loading_heart = SpriteFrame::create("sp_loading_heart.png", Rect(0, 0, 38, 35));
	SpriteFrameCache::getInstance()->addSpriteFrame(sp_loading_heart, "sp_loading_heart");

	SpriteFrame* btn_sit_up = SpriteFrame::create("btn_sit_up.png", Rect(0, 0, 86, 126)); 
	SpriteFrameCache::getInstance()->addSpriteFrame(btn_sit_up, "btn_sit_up");

	SpriteFrame* btn_sit_down = SpriteFrame::create("btn_sit_down.png", Rect(0, 0, 86, 126)); 
	SpriteFrameCache::getInstance()->addSpriteFrame(btn_sit_down, "btn_sit_down");
}

void ResourcePreload::preloadAudio(){
	SimpleAudioEngine::getInstance()->preloadEffect("click.wav");
}