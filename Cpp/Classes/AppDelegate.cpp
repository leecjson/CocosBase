#include "AppDelegate.h"
#include "../CocosBase/cocos-base.h"
#include "MenuScene.h"
#include "Test/BasicSceneTest/BasicSceneTest.h"

USING_NS_CC;

AppDelegate::AppDelegate() 
{

}

AppDelegate::~AppDelegate() 
{
	
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	pEGLView->setDesignResolutionSize(960, 640, kResolutionShowAll);

	REGISTER_SCENE_FUNC(MenuScene);
	REGISTER_SCENE_FUNC(BasicSceneTest);
	REGISTER_SCENE_FUNC(BasicUIScene);

	CCSceneManager::sharedManager()->runWithScene(LoadScene("MenuScene"));

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
