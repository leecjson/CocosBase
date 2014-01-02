#include "BackpackScene.h"


void BackpackScene::onLoadResources()
{
	addImageAsync("background.png");
	addImageAsync("CloseNormal.png");
	addImageAsync("CloseSelected.png");
	addImageAsync("btn1_1.png");
	addImageAsync("btn1_2.png");
}

void BackpackScene::onLoadScene()
{
	CCSprite* pBg = CCSprite::create("background.png");
	pBg->setPosition(ccp(480, 320));
	addChild(pBg);

	CCLabelTTF* pTitle = CCLabelTTF::create("BackpackScene", "", 60);
	pTitle->setPosition(ccp(480, 420));
	pTitle->setColor(ccBLUE);
	addChild(pTitle);

	pClose = CCMenuItemImage::create(
		"CloseNormal.png", "CloseSelected.png", this, menu_selector(BackpackScene::onClick));
	pClose->setPosition(ccp(690, 185));

	pPost = CCMenuItemImage::create(
		"btn1_1.png", "btn1_2.png", this, menu_selector(BackpackScene::onClick));
	pPost->setPosition(ccp(480, 320));

	pMenu = CCMenu::create(pClose, pPost, NULL);
	pMenu->setPosition(CCPointZero);
	addChild(pMenu);

	CCLabelTTF* pLabel = CCLabelTTF::create("PostMessage", "", 30);
	pLabel->setPosition(ccp(480, 320));
	pLabel->setColor(ccRED);
	addChild(pLabel);
}

void BackpackScene::onClick(CCObject* pSender)
{
	if( pSender == pClose  )
	{
		CCDirectorEx::sharedDirector()->popPopupScene();
	}
	else if( pSender == pPost )
	{
		CCBundle* pBundle = CCBundle::create();
		pBundle->putString("msg", "this is a message from backpack Scene");

		CCDirectorEx::sharedDirector()->PostMessage(1, pBundle);
	}
}

void BackpackScene::onMessage(unsigned int uMsg, CCObject* pMsgObj, void* wParam, void* lParam)
{
	if( pMsgObj )
	{
		CCBundle* pBundle = (CCBundle*) pMsgObj;
		CCLOG("BackpackScene Handle Message : %s", pBundle->getString("msg"));
	}
}

void BackpackScene::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_nTouchPriority, true);
	pMenu->setTouchPriority(m_nTouchPriority);
	CCSceneEx::onEnter();
}

void BackpackScene::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCSceneEx::onExit();
}