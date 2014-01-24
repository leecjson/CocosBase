#include "BasicSceneTest.h"

BasicSceneTest::BasicSceneTest()
{

}

BasicSceneTest::~BasicSceneTest()
{

}

void BasicSceneTest::onLoadResources()
{
	addImageAsync("startMenuBG.png");
	addImageAsync("btn1_1.png");
	addImageAsync("btn1_2.png");
}

void BasicSceneTest::onLoadScene()
{
	setAutoRemoveUnusedTexture(true);

	CCSprite* bg = CCSprite::create("startMenuBG.png");
	bg->setPosition(ccp(480, 320));
	addChild(bg);

	m_pMenu = CCMenu::create();
	m_pMenu->setPosition(CCPointZero);
	addChild(m_pMenu);

	CCMenuItemFont* pFont = CCMenuItemFont::create("Back");
	pFont->setTarget(this, menu_selector(BasicSceneTest::onClick));
	pFont->setFontSizeObj(45.0f);
	pFont->setPosition(CCPoint(893, 35));
	pFont->setTag(1);
	m_pMenu->addChild(pFont);

	CCMenuItemImage* pUiSceneBtn = CCMenuItemImage::create("btn1_1.png", "btn1_2.png");
	pUiSceneBtn->setTarget(this, menu_selector(BasicSceneTest::onClick));
	pUiSceneBtn->setPosition(CCPoint(600, 200));
	pUiSceneBtn->setTag(2);
	m_pMenu->addChild(pUiSceneBtn);

	CCLabelTTF* pLabel1 = CCLabelTTF::create("runUIScene", "", 30);
	pLabel1->setColor(ccRED);
	pLabel1->setPosition(CCPoint(600, 200));
	addChild(pLabel1);

	CCLabelTTF* pLabel = CCLabelTTF::create("hello CocosBase and Cocos2d-x", "", 30);
	pLabel->setColor(ccRED);
	pLabel->setPosition(ccp(480, 320));
	addChild(pLabel);
}

void BasicSceneTest::onEnterScene()
{
	m_pMenu->setTouchPriority(CCSceneManager::sharedManager()->getTouchPriority());
}

void BasicSceneTest::onClick(CCObject* sender)
{
	CCNode* pNode = (CCNode*) sender;
	switch( pNode->getTag() )
	{
	case 1:
		{
			CCSceneManager::sharedManager()->popScene();
		}
		break;
	case 2:
		{
			CCSceneManager::sharedManager()->runUIScene(LoadScene("BasicUIScene"));
		}
		break;
	}
}




void BasicUIScene::onLoadResources()
{
	addImage("background.png");
}

void BasicUIScene::onLoadScene()
{
	setAutoRemoveUnusedTexture(true);

	CCSprite* bg = CCSprite::create("background.png");
	bg->setPosition(ccp(480, 320));
	addChild(bg);

	m_pMenu = CCMenu::create();
	m_pMenu->setPosition(CCPointZero);
	addChild(m_pMenu);

	CCMenuItemImage* pBtn = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png");
	pBtn->setTarget(this, menu_selector(BasicUIScene::onClick));
	pBtn->setPosition(ccp(480, 320));
	m_pMenu->addChild(pBtn);
}

void BasicUIScene::onEnterScene()
{
	m_pMenu->setTouchPriority(CCSceneManager::sharedManager()->getTouchPriority());
}

void BasicUIScene::onClick(CCObject* sender)
{
	CCSceneManager::sharedManager()->popUIScene(this);
}