/****************************************************************************
Copyright (c) 2014 viva-Lijunlin

Created by Li JunLin on 2014

csdn_viva@foxmail.com
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "CCDirectorEx.h"
using namespace std;

NS_CC_BEGIN

CCDirectorEx::CCDirectorEx()
: m_bMainLoopLocked(false)
, m_bTouchEventLocked(false)
, m_pPopupSceneStack(NULL)
, m_pRunningPopupScene(NULL)
, m_pSceneStack(NULL)
, m_pNextSwitchScene(NULL)
{
	m_pPopupSceneStack = new CCArray(5);
	ignoreAnchorPointForPosition(true);
	setContentSize(CCDirector::sharedDirector()->getWinSize());
	setAnchorPoint(CCPointZero);
	setPosition(CCPointZero);
	schedule(schedule_selector(CCDirectorEx::mainLoop));
}

CCDirectorEx::~CCDirectorEx()
{
	CC_SAFE_RELEASE(m_pRunningPopupScene);
	CC_SAFE_DELETE(m_pPopupSceneStack);

	while(!m_lTargetMsgQueue.empty())
	{
		CC_SAFE_RELEASE(m_lTargetMsgQueue.back().tMessage.pMsgObj);
		CC_SAFE_RELEASE(dynamic_cast<CCObject*>(m_lTargetMsgQueue.back().pTarget));
		m_lTargetMsgQueue.pop_back();
	}

	while(!m_vTargetMsgTemps.empty())
	{
		CC_SAFE_RELEASE(m_vTargetMsgTemps.back().tMessage.pMsgObj);
		CC_SAFE_RELEASE(dynamic_cast<CCObject*>(m_vTargetMsgTemps.back().pTarget));
		m_vTargetMsgTemps.pop_back();
	}

	while(!m_lMsgQueue.empty())
	{
		CC_SAFE_RELEASE(m_lMsgQueue.front().pMsgObj);
		m_lMsgQueue.pop_front();
	}

	while(!m_vMsgTemps.empty())
	{
		CC_SAFE_RELEASE(m_vMsgTemps.back().pMsgObj);
		m_vMsgTemps.pop_back();
	}

	removeAllCachedScenes();

	while(!m_lMsgHandlersAdded.empty())
	{
		CCObject* pObj = dynamic_cast<CCObject*>(m_lMsgHandlers.front());
		CC_SAFE_RELEASE(pObj);
		m_lMsgHandlersAdded.pop_front();
	}

	while(!m_lMsgHandlers.empty())
	{
		CCObject* pObj = dynamic_cast<CCObject*>(m_lMsgHandlers.front());
		CC_SAFE_RELEASE(pObj);
		m_lMsgHandlers.pop_front();
	}

	m_lMsgHandlersRemoved.clear();
	m_mFnSceneCreatePointers.clear();

	unschedule(schedule_selector(CCDirectorEx::mainLoop));
}

void CCDirectorEx::mainLoop(float dt)
{
// >> locked
	m_bMainLoopLocked = true;

	while(!m_lMsgQueue.empty())
	{
		tagMessage& tagMsg = m_lMsgQueue.front();
		if(!m_lMsgHandlers.empty())
		{
			list<CCMessageProtocol*>::iterator pIter = m_lMsgHandlers.begin();
			for(; pIter != m_lMsgHandlers.end(); ++pIter)
			{
				(*pIter)->onMessage(tagMsg.uMsg, tagMsg.pMsgObj, tagMsg.wParam, tagMsg.lParam);
			}
		}
		CC_SAFE_RELEASE(tagMsg.pMsgObj);
		m_lMsgQueue.pop_front();
	}

	while(!m_lTargetMsgQueue.empty())
	{
		tagTarget& tTarget = m_lTargetMsgQueue.front();
		tagMessage& tagMsg = tTarget.tMessage;

		tTarget.pTarget->onMessage(tagMsg.uMsg, tagMsg.pMsgObj, tagMsg.wParam, tagMsg.lParam);
		CCObject* pObj = dynamic_cast<CCObject*>(tTarget.pTarget);
		CC_SAFE_RELEASE(pObj);
		CC_SAFE_RELEASE(tagMsg.pMsgObj);

		m_lTargetMsgQueue.pop_front();
	}

	while(!m_lSceneSwitchQueue.empty())
	{
		tagSceneSwitch& tag = m_lSceneSwitchQueue.front();

		if( tag.bIsLocked )
			break;

		if( tag.pNextScene ) tag.pNextScene->autorelease();
		handleSceneSwitch(tag);
		m_lSceneSwitchQueue.pop_front();
	}

	m_bMainLoopLocked = false;
// << locked

	while(!m_lMsgHandlersAdded.empty())
	{
		CCMessageProtocol* pProtocol = m_lMsgHandlersAdded.front();
		m_lMsgHandlers.push_front(pProtocol);
		m_lMsgHandlersAdded.pop_front();
	}

	while(!m_lMsgHandlersRemoved.empty())
	{
		CCMessageProtocol* pProtocol = m_lMsgHandlersRemoved.front();
		if( pProtocol )
		{
			CCObject* pObject = dynamic_cast<CCObject*>(pProtocol);
			m_lMsgHandlers.remove(pProtocol);
			pObject->release();
		}
		m_lMsgHandlersRemoved.pop_front();
	}

	if(!m_vMsgTemps.empty())
	{
		vector<tagMessage>::iterator iter = m_vMsgTemps.begin();
		for(; iter != m_vMsgTemps.end(); ++iter)
		{
			m_lMsgQueue.push_back(*iter);
		}
		m_vMsgTemps.clear();
	}

	if(!m_vTargetMsgTemps.empty())
	{
		vector<tagTarget>::iterator iter = m_vTargetMsgTemps.begin();
		for(; iter != m_vTargetMsgTemps.end(); ++iter)
		{
			m_lTargetMsgQueue.push_back(*iter);
		}
		m_vTargetMsgTemps.clear();
	}
}

void CCDirectorEx::PostMessage(unsigned int uMsg, CCObject* pMsgObj, void* wParam, void* lParam)
{
	tagMessage     tagMsg;
	tagMsg.uMsg    = uMsg;
	tagMsg.pMsgObj = pMsgObj;
	tagMsg.wParam  = wParam;
	tagMsg.lParam  = lParam;

	CC_SAFE_RETAIN(pMsgObj);

	if( m_bMainLoopLocked )
	{
		m_vMsgTemps.push_back(tagMsg);
	}
	else
	{
		m_lMsgQueue.push_back(tagMsg);
	}
}

void CCDirectorEx::PostMessage(CCMessageProtocol* pProtocol, unsigned int uMsg, CCObject* pMsgObj, void* wParam, void* lParam)
{
	CCAssert(pProtocol && dynamic_cast<CCObject*>(pProtocol), "this target should not null");

	CCObject* pObj = dynamic_cast<CCObject*>(pProtocol);
	
	tagTarget                tTarget;
	tTarget.pTarget          = pProtocol;
	tTarget.tMessage.uMsg    = uMsg;
	tTarget.tMessage.pMsgObj = pMsgObj;
	tTarget.tMessage.wParam  = wParam;
	tTarget.tMessage.lParam  = lParam;

	pObj->retain();
	CC_SAFE_RETAIN(pMsgObj);
	
	if( m_bMainLoopLocked )
	{
		m_vTargetMsgTemps.push_back(tTarget);
	}
	else
	{
		m_lTargetMsgQueue.push_back(tTarget);
	}
}

void CCDirectorEx::registerMessageHandler(CCMessageProtocol* pProtocol)
{
	list<CCMessageProtocol*>::iterator itr = m_lMsgHandlers.begin();
	list<CCMessageProtocol*>::iterator end = m_lMsgHandlers.end();

	for(; itr != end; ++itr )
	{
		if( *itr == pProtocol )
		{
			return;
		}
	}

	CCObject* pObject = dynamic_cast<CCObject*>(pProtocol);

	if( pObject == NULL )
		return;

	CC_SAFE_RETAIN(pObject);

	if( m_bMainLoopLocked )
	{
		m_lMsgHandlersAdded.push_back(pProtocol);
	}
	else
	{
		m_lMsgHandlers.push_front(pProtocol);
	}
}

void CCDirectorEx::unregisterMessageHandler(CCMessageProtocol* pProtocol)
{
	CCObject* pObject = dynamic_cast<CCObject*>(pProtocol);

	if( pObject == NULL )
		return;

	if( m_bMainLoopLocked )
	{
		m_lMsgHandlersRemoved.push_back(pProtocol);
	}
	else
	{
		m_lMsgHandlers.remove(pProtocol);
		pObject->release();
	}
}

void CCDirectorEx::registerSceneClass(const char* pSceneName, Fn_CreateScene pFn)
{
	if( pFn && pSceneName && strlen(pSceneName) )
	{
		m_mFnSceneCreatePointers[pSceneName] = pFn;
	}
}

CCSceneEx* CCDirectorEx::seekScene(const char* pSceneName, CCObject* pExtraObj)
{
	map<string, CCSceneEx*>::iterator mitr = m_mSceneCachePool.find(pSceneName);
	if( mitr != m_mSceneCachePool.end() )
	{
		m_pNextSwitchScene = mitr->second;
		m_pNextSwitchScene->setExtraObject(pExtraObj);
		return m_pNextSwitchScene;
	}

	map<string, Fn_CreateScene>::iterator itr = m_mFnSceneCreatePointers.find(pSceneName);
	if( itr != m_mFnSceneCreatePointers.end() )
	{
		m_pNextSwitchScene = (*itr->second)();
		m_pNextSwitchScene->setExtraObject(pExtraObj);
		m_pNextSwitchScene->setClassName(pSceneName);
		m_pNextSwitchScene->init();

		m_pNextSwitchScene->onLoadResources();
		m_pNextSwitchScene->loadResourcesSync();

		if( !m_pNextSwitchScene->isLoadingResourcesAsync() )
		{
			m_pNextSwitchScene->onLoadResourcesCompleted();
			m_pNextSwitchScene->onLoadScene();
		}

		if( m_pNextSwitchScene->isCachable() )
		{
			m_mSceneCachePool.insert(make_pair(pSceneName, m_pNextSwitchScene));
		}
		else
		{
			m_pNextSwitchScene->autorelease();
		}

		return m_pNextSwitchScene;
	}

	return NULL;
}

void CCDirectorEx::runWithScene(CCScene* pScene)
{
	CCAssert(pScene && m_pNextSwitchScene, "should not null");

	performSceneSwitch(eSceneSwitchRunWithScene, pScene);
}

void CCDirectorEx::replaceScene(CCScene* pScene)
{
	CCAssert(pScene && m_pNextSwitchScene, "should not null");

	performSceneSwitch(eSceneSwitchReplaceScene, pScene);
}

void CCDirectorEx::pushScene(CCScene* pScene)
{
	CCAssert(pScene && m_pNextSwitchScene, "should not null");

	performSceneSwitch(eSceneSwitchPushScene, pScene);
}

void CCDirectorEx::popScene()
{
	performSceneSwitch(eSceneSwitchPopScene);
}

void CCDirectorEx::popToRootScene()
{
	popToSceneStackLevel(1);
}

void CCDirectorEx::popToSceneStackLevel(int nLevel)
{
	performSceneSwitch(eSceneSwitchPopToSceneStackLevel, NULL, nLevel);
}

void CCDirectorEx::replacePopupScene(CCScene* pScene)
{
	CCAssert(m_pRunningPopupScene, "Use pushPopupScene: instead to start a PopupScene");
    CCAssert(pScene != NULL && m_pNextSwitchScene, "the scene should not be null");

	performSceneSwitch(eSceneSwitchReplacePopupScene, pScene);
}

void CCDirectorEx::pushPopupScene(CCScene* pScene)
{
	CCAssert(pScene && m_pNextSwitchScene, "the scene should not null");

	performSceneSwitch(eSceneSwitchPushPopupScene, pScene);
}

void CCDirectorEx::popPopupScene()
{
	if( !m_pRunningPopupScene || m_pPopupSceneStack->count() == 0 )
		return;

	performSceneSwitch(eSceneSwitchPopPopupScene);
}

void CCDirectorEx::popToRootPopupScene()
{
	popToPopupSceneStackLevel(1);
}

void CCDirectorEx::popToPopupSceneStackLevel(int nLevel)
{
	if( m_pPopupSceneStack->count() == 0 || nLevel >= (int)m_pPopupSceneStack->count() )
		return;

	performSceneSwitch(eSceneSwitchPopToPopupSceneStackLevel, NULL, nLevel);
}

void CCDirectorEx::unloackSceneSwitch(const std::string& strClassName)
{
	CCAssert(!m_lSceneSwitchQueue.empty(), "empty");

	list<tagSceneSwitch>::iterator itr = m_lSceneSwitchQueue.begin();
	list<tagSceneSwitch>::iterator end = m_lSceneSwitchQueue.end();

	for(; itr != end; ++itr)
	{
		if( itr->strClassName == strClassName )
		{
			itr->bIsLocked = false;
			return;
		}
	}
}


void CCDirectorEx::performSceneSwitch(SceneSwitchType eType, CCScene* pScene, int nLevel)
{
	switch( eType )
	{
	case eSceneSwitchRunWithScene:
	case eSceneSwitchPushScene:
	case eSceneSwitchReplaceScene:
	case eSceneSwitchPushPopupScene:
	case eSceneSwitchReplacePopupScene:
		{
			pScene->retain();

			tagSceneSwitch   tag;
			tag.eType        = eType;
			tag.strClassName = m_pNextSwitchScene->getClassName();
			tag.pNextScene   = pScene;
			tag.bIsLocked    = true;

			if( m_pNextSwitchScene->isFirstShow() && m_pNextSwitchScene->isLoadingResourcesAsync() )
			{
				m_lSceneSwitchQueue.push_back(tag);
				m_pNextSwitchScene->loadResourcesAsync();
			}
			else
			{
				tag.bIsLocked = false;
				m_lSceneSwitchQueue.push_back(tag);
			}

			if( m_pNextSwitchScene->isFirstShow() )
			{
				m_pNextSwitchScene->setFirstShow(false);
			}
	
			m_pNextSwitchScene = NULL;
		}
		break;
	case eSceneSwitchPopScene:
	case eSceneSwitchPopToSceneStackLevel:
	case eSceneSwitchPopPopupScene:
	case eSceneSwitchPopToPopupSceneStackLevel:
		{
			tagSceneSwitch tag;
			tag.eType      = eType;
			tag.bIsLocked  = false;
			tag.pNextScene = NULL;
			tag.nPopLevel  = nLevel;

			m_lSceneSwitchQueue.push_back(tag);
		}
		break;
	default:
		break;
	}
}

void CCDirectorEx::handleSceneSwitch(tagSceneSwitch& tag)
{
	switch( tag.eType )
	{
	case eSceneSwitchRunWithScene:
		{
			CCDirector::sharedDirector()->runWithScene(tag.pNextScene);
		}
		break;
	case eSceneSwitchPushScene:
		{
			CCDirector::sharedDirector()->pushScene(tag.pNextScene);
		}
		break;
	case eSceneSwitchReplaceScene:
		{
			CCDirector::sharedDirector()->replaceScene(tag.pNextScene);
		}
		break;
	case eSceneSwitchPopScene:
		{
			CCDirector::sharedDirector()->popScene();
		}
		break;
	case eSceneSwitchPopToSceneStackLevel:
		{
			CCDirector::sharedDirector()->popToSceneStackLevel(tag.nPopLevel);
		}
		break;
	case eSceneSwitchPushPopupScene:
		{
			m_pPopupSceneStack->addObject(tag.pNextScene);
			setNextPopupScene(tag.pNextScene, false);
		}
		break;
	case eSceneSwitchReplacePopupScene:
		{
			m_pPopupSceneStack->removeLastObject();
			m_pPopupSceneStack->addObject(tag.pNextScene);
			setNextPopupScene(tag.pNextScene, true);
		}
		break;
	case eSceneSwitchPopPopupScene:
		{
			m_pPopupSceneStack->removeLastObject();
			setNextPopupScene((CCScene*) m_pPopupSceneStack->lastObject(), true);
		}
		break;
	case eSceneSwitchPopToPopupSceneStackLevel:
		{
			int nLevel = MAX(0, tag.nPopLevel);
			int nCount = (int) m_pPopupSceneStack->count();

			for(; nCount > nLevel; --nCount)
			{
				CCScene* pLastScene = (CCScene*) m_pPopupSceneStack->lastObject();

				if( pLastScene->isRunning() )
				{
					pLastScene->onExitTransitionDidStart();
					pLastScene->onExit();
				}

				pLastScene->cleanup();
				m_pPopupSceneStack->removeLastObject();
			}

			if( m_pPopupSceneStack->count() > 0 )
			{
				setNextPopupScene((CCScene*) m_pPopupSceneStack->lastObject(), false);
			}
		}
		break;
	}
}

void CCDirectorEx::setNextPopupScene(CCScene* pNextScene, bool bCleanUp)
{
	if( pNextScene )
	{
		if( m_pRunningPopupScene )
		{
			m_pRunningPopupScene->onExitTransitionDidStart();
			m_pRunningPopupScene->onExit();

			if( bCleanUp )
			{
				m_pRunningPopupScene->cleanup();
			}
		}

		CC_SAFE_RELEASE(m_pRunningPopupScene);
		m_pRunningPopupScene = pNextScene;
		CC_SAFE_RETAIN(pNextScene);

		m_pRunningPopupScene->onEnter();
		m_pRunningPopupScene->onEnterTransitionDidFinish();
	}
	else
	{
		if( m_pRunningPopupScene )
		{
			m_pRunningPopupScene->onExitTransitionDidStart();
			m_pRunningPopupScene->onExit();

			if( bCleanUp )
			{
				m_pRunningPopupScene->cleanup();
			}
		}
		CC_SAFE_RELEASE_NULL(m_pRunningPopupScene);
	}
}


void CCDirectorEx::registerDialogClass(const char* pDialogName, Fn_CreateDialog pFn)
{
	if( pFn && pDialogName && strlen(pDialogName) )
	{
		m_mFnDialogCreatePointers[pDialogName] = pFn;
	}
}

CCDialog* CCDirectorEx::seekDialog(const char* pDialogName, CCObject* pExtraObj)
{
	map<string, CCDialog*>::iterator mitr = m_mDialogCachePool.find(pDialogName);
	if( mitr != m_mDialogCachePool.end() )
	{
		mitr->second->setExtraObject(pExtraObj);
		return mitr->second;
	}

	map<string, Fn_CreateDialog>::iterator itr = m_mFnDialogCreatePointers.find(pDialogName);
	if( itr != m_mFnDialogCreatePointers.end() )
	{
		CCDialog* pDialog = (*itr->second)();
		pDialog->setExtraObject(pExtraObj);
		pDialog->setClassName(pDialogName);
		pDialog->init();
		pDialog->onLoadDialog();

		if( pDialog->isCachable() )
		{
			m_mDialogCachePool.insert(make_pair(pDialogName, pDialog));
		}
		else
		{
			pDialog->autorelease();
		}

		return pDialog;
	}

	return NULL;
}

void CCDirectorEx::removeCachedDialog(const char* pSceneName)
{
	map<string, CCDialog*>::iterator itr = m_mDialogCachePool.find(pSceneName);
	if( itr != m_mDialogCachePool.end() )
	{
		itr->second->release();
		m_mDialogCachePool.erase(itr);
	}
}

void CCDirectorEx::removeAllCachedDialogs()
{
	if( m_mDialogCachePool.empty() )
		return;

	map<string, CCDialog*>::iterator itr = m_mDialogCachePool.begin();
	for(; itr != m_mDialogCachePool.end(); ++itr)
	{
		itr->second->release();
	}
	m_mDialogCachePool.clear();
}

void CCDirectorEx::removeUnusedCachedDialogs()
{
	if( m_mDialogCachePool.empty() )
		return;

	map<string, CCDialog*>::iterator itr = m_mDialogCachePool.begin();
	map<string, CCDialog*>::iterator end = m_mDialogCachePool.end();

	for(; itr != end; )
	{
		if( itr->second->isSingleReference() )
		{
			itr->second->release();
			itr = m_mDialogCachePool.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void CCDirectorEx::showDialog(CCDialog* pDialog, int nZOrder = 0)
{
	CCAssert(pDialog, "should not null");

	addChild(pDialog, nZOrder);
}

void CCDirectorEx::closeDialog(const char* pDialogName)
{
	CCAssert(pDialogName && strlen(pDialogName), "should not null");

	map<string, CCDialog*>::iterator itr = m_mDialogCachePool.find(pDialogName);
	if( itr != m_mDialogCachePool.end() )
	{
		removeChild(itr->second);
	}
}

void CCDirectorEx::visit()
{
	if( m_pRunningPopupScene )
	{
		m_pRunningPopupScene->visit();
	}

	CCNode::visit();
}

void CCDirectorEx::end()
{
	retain();
	purgeDirector();
	CCDirector::sharedDirector()->end();
	release();
}

CCScene* CCDirectorEx::getRunningScene()
{
	return CCDirector::sharedDirector()->getRunningScene();
}

CCScene* CCDirectorEx::getRunningPopupScene()
{
	return m_pRunningPopupScene;
}

void CCDirectorEx::removeCachedScene(const char* pSceneName)
{
	map<string, CCSceneEx*>::iterator itr = m_mSceneCachePool.find(pSceneName);
	if( itr != m_mSceneCachePool.end() )
	{
		itr->second->release();
		m_mSceneCachePool.erase(itr);
	}
}

void CCDirectorEx::removeAllCachedScenes()
{
	if( m_mSceneCachePool.empty() )
		return;

	map<string, CCSceneEx*>::iterator itr = m_mSceneCachePool.begin();
	for(; itr != m_mSceneCachePool.end(); ++itr)
	{
		itr->second->release();
	}
	m_mSceneCachePool.clear();
}

void CCDirectorEx::removeUnusedCachedScenes()
{
	if( m_mSceneCachePool.empty() )
		return;

	map<string, CCSceneEx*>::iterator itr = m_mSceneCachePool.begin();
	map<string, CCSceneEx*>::iterator end = m_mSceneCachePool.end();

	for(; itr != end; )
	{
		if( itr->second->isSingleReference() )
		{
			itr->second->release();
			itr = m_mSceneCachePool.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

int CCDirectorEx::getTouchPriority()
{
	static int nPriority = 0;
	return nPriority--;
}

void CCDirectorEx::lockTouchEvent()
{
	if( !m_bTouchEventLocked )
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(false);
		m_bTouchEventLocked = true;
	}
}

void CCDirectorEx::unlockTouchEvent()
{
	if( m_bTouchEventLocked )
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(true);
		m_bTouchEventLocked = false;
	}
}

CCDirectorEx* CCDirectorEx::sharedDirector()
{
	static CCDirectorEx* theInstance = NULL;
	if( theInstance == NULL )
	{
		theInstance = new CCDirectorEx();
		CCDirector::sharedDirector()->setNotificationNode(theInstance);
		theInstance->release();
		theInstance->onEnter();
		theInstance->onEnterTransitionDidFinish();
	}
	return theInstance;
}

void CCDirectorEx::purgeDirector()
{
	popToPopupSceneStackLevel(0);
	onExitTransitionDidStart();
	onExit();
	CCDirector::sharedDirector()->setNotificationNode(NULL);
}

NS_CC_END