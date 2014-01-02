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
#include "CCSceneEx.h"
#include "CCDirectorEx.h"
using namespace std;

NS_CC_BEGIN

CCSceneEx::CCSceneEx()
: m_strClassName("")
, m_pExtraObject(NULL)
, m_bFirstShow(true)
, m_nLoadCounter(0)
, m_nLoadCount(0)
{

}

CCSceneEx::~CCSceneEx()
{
	CC_SAFE_RELEASE(m_pExtraObject);
#if COCOS2D_DEBUG >= 1
	if( !m_strClassName.empty() )
	{
		CCLOG("CocosBase [DEBUG] : destroy Scene %s", m_strClassName.c_str());
	}
#endif
}

bool CCSceneEx::init()
{
#if COCOS2D_DEBUG >= 1
	CCLOG("CocosBase [DEBUG] : new Scene %s", m_strClassName.c_str());
	return CCScene::init();
#endif
}

void CCSceneEx::onEnter()
{
	m_nTouchPriority = CCDirectorEx::sharedDirector()->getTouchPriority();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
		this, m_nTouchPriority, true);

	CCDirectorEx::sharedDirector()->registerMessageHandler(this);

	CCScene::onEnter();
}

void CCSceneEx::onExit()
{
	CCDirectorEx::sharedDirector()->unregisterMessageHandler(this);

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	CCScene::onExit();
}

bool CCSceneEx::isCachable()
{
	return false;
}

const char* CCSceneEx::getClassName()
{
	return m_strClassName.c_str();
}

void CCSceneEx::setClassName(const char* pClassName)
{
	m_strClassName = pClassName;
}

CCObject* CCSceneEx::getExtraObject()
{
	return m_pExtraObject;
}

void CCSceneEx::setExtraObject(CCObject* pExtraObject)
{
	CC_SAFE_RETAIN(pExtraObject);
	CC_SAFE_RELEASE(m_pExtraObject);
	m_pExtraObject = pExtraObject;
}

int CCSceneEx::getTouchPriority() const
{
	return m_nTouchPriority;
}

void CCSceneEx::setFirstShow(bool bFirstShow)
{
	m_bFirstShow = bFirstShow;
}

bool CCSceneEx::isFirstShow() const
{
	return m_bFirstShow;		
}

bool CCSceneEx::isLoadingResourcesAsync()
{
	return !m_vLoadImageFilesAsync.empty();
}

void CCSceneEx::loadResourcesSync()
{
	if( m_vLoadImageFilesSync.empty() )
		return;

	vector<string>::iterator itr = m_vLoadImageFilesSync.begin();
	vector<string>::iterator end = m_vLoadImageFilesSync.end();

	for(; itr != end; ++itr)
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(itr->c_str());
		CCAssert(pTexture, "load image failed");
#if COCOS2D_DEBUG >= 1
		CCLOG("CocosBase [DEBUG] : Scene [%s] load image sync %s", m_strClassName.c_str(), itr->c_str());
#endif
	}

	m_vLoadImageFilesSync.clear();
}

void CCSceneEx::loadResourcesAsync()
{
	CCAssert(!m_vLoadImageFilesAsync.empty(), "empty");

	m_nLoadCount = m_vLoadImageFilesAsync.size();
	m_nLoadCounter = 0;

	vector<string>::iterator itr = m_vLoadImageFilesAsync.begin();
	vector<string>::iterator end = m_vLoadImageFilesAsync.end();

	for(; itr != end; ++itr)
	{
		CCTextureCache::sharedTextureCache()->addImageAsync(
			itr->c_str(), 
			this, 
			callfuncO_selector(CCSceneEx::loadingResourcesCallBack));
#if COCOS2D_DEBUG >= 1
		CCLOG("CocosBase [DEBUG] : Scene [%s] load image async %s", m_strClassName.c_str(), itr->c_str());
#endif
	}

	m_vLoadImageFilesAsync.clear();
}

void CCSceneEx::loadingResourcesCallBack(CCObject* pTextureObj)
{
	CCAssert(pTextureObj, "load resources async failed");

	++m_nLoadCounter;

	if( m_nLoadCounter == m_nLoadCount )
	{
		m_nLoadCount = 0;
		m_nLoadCounter = 0;
		onLoadResourcesCompleted();
		onLoadScene();

		CCDirectorEx::sharedDirector()->unloackSceneSwitch(m_strClassName);
		return;
	}
}

void CCSceneEx::addImage(const char* pFile)
{
	CCAssert(pFile && strlen(pFile), "pFile invalidate");
	m_vLoadImageFilesSync.push_back(pFile);
}

void CCSceneEx::addImageAsync(const char* pFile)
{
	CCAssert(pFile && strlen(pFile), "pFile invalidate");
	m_vLoadImageFilesAsync.push_back(pFile);
}


NS_CC_END