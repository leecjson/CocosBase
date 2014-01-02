#include "CCDialog.h"

NS_CC_BEGIN

CCDialog::CCDialog()
{

}

CCDialog::~CCDialog()
{
	CC_SAFE_RELEASE(m_pExtraObject);
#if COCOS2D_DEBUG >= 1
	if( !m_strClassName.empty() )
	{
		CCLOG("CocosBase [DEBUG] : destroy Dialog %s", m_strClassName.c_str());
	}
#endif
}

bool CCDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return isModalable();
}

void CCDialog::onEnter()
{
	m_nTouchPriority = CCDirectorEx::sharedDirector()->getTouchPriority();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
		this, m_nTouchPriority, true);

	CCDirectorEx::sharedDirector()->registerMessageHandler(this);

	CCNode::onEnter();
}

void CCDialog::onExit()
{
	CCDirectorEx::sharedDirector()->unregisterMessageHandler(this);

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	CCNode::onExit();
}

bool CCDialog::init()
{
#if COCOS2D_DEBUG >= 1
	setContentSize(CCDirector::sharedDirector()->getWinSize());
	CCLOG("CocosBase [DEBUG] : new Dialog %s", m_strClassName.c_str());
	return CCNode::init();
#endif
}

bool CCDialog::isModalable()
{
	return true;
}

bool CCDialog::isCachable()
{
	return false;
}

const char* CCDialog::getClassName()
{
	return m_strClassName.c_str();
}

int CCDialog::getTouchPriority() const
{
	return m_nTouchPriority;
}

CCObject* CCDialog::getExtraObject()
{
	return m_pExtraObject;
}

void CCDialog::setClassName(const char* pClassName)
{
	m_strClassName = pClassName;
}

void CCDialog::setExtraObject(CCObject* pExtraObject)
{
	CC_SAFE_RETAIN(pExtraObject);
	CC_SAFE_RELEASE(m_pExtraObject);
	m_pExtraObject = pExtraObject;
}

NS_CC_END