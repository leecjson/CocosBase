#include "MainScene.h"

MainScene::MainScene()
{
	
}

void MainScene::onLoadResources()
{
	addImage("back1.png");
	addImage("back2.png");
	addImage("toggle1_1.png");
	addImage("toggle1_2.png");
}

void MainScene::onLoadScene()
{
	CCLayerColor* pColor = CCLayerColor::create(ccc4(125, 125, 125, 255));
	pColor->setPosition(CCPointZero);
	addChild(pColor);

	CCLabelTTF* pTitle = CCLabelTTF::create("MainScene", "", 60);
	pTitle->setPosition(ccp(480, 590));
	pTitle->setColor(ccBLUE);
	addChild(pTitle);

	pBack = CCMenuItemImage::create(
		"back1.png", "back2.png", this, menu_selector(MainScene::onClick));
	pBack->setPosition(ccp(350, 320));

	pBackpack = CCMenuItemImage::create(
		"toggle1_1.png", "toggle1_2.png", this, menu_selector(MainScene::onClick));
	pBackpack->setPosition(ccp(600, 320));

	pMenu = CCMenu::create(pBack, pBackpack, NULL);
	pMenu->setPosition(CCPointZero);
	
	addChild(pMenu);

	CCLabelTTF* pBkLabel = CCLabelTTF::create("BackpackScene", "", 27);
	pBkLabel->setPosition(ccp(600, 320));
	pBkLabel->setColor(ccRED);
	addChild(pBkLabel);

	m_pNamePassword = CCLabelTTF::create("", "", 28);
	m_pNamePassword->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_pNamePassword->setPosition(ccp(480, 150));
	addChild(m_pNamePassword);
}

void MainScene::onMessage(unsigned int uMsg, CCObject* pMsgObj, void* wParam, void* lParam)
{
	if( pMsgObj )
	{
		CCBundle* pBundle = (CCBundle*) pMsgObj;
		CCLOG("MainScene Handle Message : %s", pBundle->getString("msg"));
	}
}

void MainScene::onEnter()
{
	CCSceneEx::onEnter();

	pMenu->setTouchPriority(m_nTouchPriority);

	CCBundle* pBundle = (CCBundle*) getExtraObject();
	if( pBundle )
	{
		const char* pUserName = pBundle->getString("username");
		const char* pPassword = pBundle->getString("password");

		char buff[200];
		sprintf(buff, "UserName : %s\nPassword : %s", pUserName, pPassword);
		m_pNamePassword->setString(buff);
	}	
}

void MainScene::onClick(CCObject* pSender)
{
	if( pSender == pBack )
	{
		CCDirectorEx::sharedDirector()->replaceScene(SeekScene("LoginScene"));
	}
	else if( pBackpack == pSender )
	{
		CCDirectorEx::sharedDirector()->pushPopupScene(SeekScene("BackpackScene"));	
	}
}