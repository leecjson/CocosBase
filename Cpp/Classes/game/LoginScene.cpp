#include "LoginScene.h"

LoginScene::LoginScene()
{

}

LoginScene::~LoginScene()
{

}

void LoginScene::onLoadScene()
{
	CCLabelTTF* pTitle = CCLabelTTF::create("LoginScene", "", 60);
	pTitle->setPosition(ccp(480, 590));
	pTitle->setColor(ccBLUE);
	addChild(pTitle);

	//CCLabelTTF* pLabel = CCLabelTTF::create("Login To Main Scene", "", 32);
	//pLabel->setPosition(ccp(480, 400));
	//addChild(pLabel);

	CCMenuItemImage* pLogin = CCMenuItemImage::create(
		"next1.png", "next2.png", this, menu_selector(LoginScene::onClick));
	pLogin->setPosition(ccp(480, 320));

	CCMenu* pMenu = CCMenu::create(pLogin, NULL);
	pMenu->setPosition(CCPointZero);
	pMenu->setTouchPriority(0);
	addChild(pMenu);

	CCLabelTTF* pNamePassword = CCLabelTTF::create("UserName : Viva - Lijunlin\nPassword : gogogo", "", 28);
	pNamePassword->setHorizontalAlignment(kCCTextAlignmentLeft);
	pNamePassword->setPosition(ccp(480, 150));
	addChild(pNamePassword);
}

void LoginScene::onClick(CCObject* pSender)
{
	CCBundle* pBundle = CCBundle::create();
	pBundle->putString("username", "Viva - Lijunlin");
	pBundle->putString("password", "gogogo");

	CCDirectorEx::sharedDirector()->replaceScene(
		CCTransitionFade::create(1.0f, SeekSceneExtra("MainScene", pBundle))
	);
}