#include "MenuScene.h"

MenuScene::MenuScene()
{

}

MenuScene::~MenuScene()
{
	
}

void MenuScene::onLoadScene()
{
	m_vMenuItems.push_back("Basic Test");

	CCSize tContainerSize = CCSize(700, 100 * m_vMenuItems.size());

	CCLayer* pScrollViewContainer = CCLayer::create();
	pScrollViewContainer->setPosition(CCPointZero);
	pScrollViewContainer->setContentSize(tContainerSize);

	m_pScrollView = CCScrollView::create(CCSize(700, 640));
	m_pScrollView->setDirection(kCCScrollViewDirectionVertical);
	m_pScrollView->ignoreAnchorPointForPosition(false);
	m_pScrollView->setPosition(CCPoint(480, 320));
	m_pScrollView->setContainer(pScrollViewContainer);
	addChild(m_pScrollView);

	CCArray* pArray = CCArray::create();
	
	float height = tContainerSize.height;

	unsigned int i = 0;
	unsigned int end = m_vMenuItems.size();
	for(; i < end; ++i )
	{
		CCMenuItemFont* pFont = CCMenuItemFont::create(m_vMenuItems[i].c_str());
		pFont->setTarget(this, menu_selector(MenuScene::onClick));
		pFont->setFontSizeObj(45.0f);
		pFont->setPosition(ccp(700 / 2, height - 100 / 2));
		pFont->setTag(i + 1);
		pArray->addObject(pFont);

		height -= 100;
	}

	CCMenu* pMenu = CCMenu::createWithArray(pArray);
	pMenu->setPosition(ccp(0,0));
	pScrollViewContainer->addChild(pMenu);

	m_pScrollView->setContentOffset(ccp(0, -(tContainerSize.height - 640)));
}

void MenuScene::onClick(CCObject* sender)
{
	CCMenuItemFont* pFont = (CCMenuItemFont*) sender;
	switch( pFont->getTag() )
	{
	case 1:
		{
			CCSceneManager::sharedManager()->pushScene(
				CCSceneExTransitionFade::create(1.0f, LoadScene("BasicSceneTest"))
				);
		}
		break;
	}
}