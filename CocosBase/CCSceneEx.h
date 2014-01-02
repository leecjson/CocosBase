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
#ifndef __CCBASE_SCENEEX_H__
#define __CCBASE_SCENEEX_H__

/////////////////////////////////////////////////////////////////////////////
/// BugFix : 
///
/////////////////////////////////////////////////////////////////////////////

#include "cocos2d.h"
#include "CCMessageProtocol.h"
#include "CCBundle.h"
#include <string>

NS_CC_BEGIN

/**
 * class    : CCSceneEx
 * author   : viva - Lijunlin
 * email    : csdn_viva@foxmail.com
 * function : 场景拓展类
 */
class CCSceneEx : public CCScene, public CCMessageProtocol, public CCTouchDelegate
{
public:
	CCSceneEx();
	virtual ~CCSceneEx();

	/**
	 * 当场景被初次创建时 在此方法内部初始化场景所需要的资源
	 * 目前支持图片的加载
	 * 同步方式：addImage()
	 * 异步方式：addImageAsync()
	 * 当所有资源成功加载后 会调用onLoadResourcesCompleted
	 * 仅初始化时回调一次
	 */
	virtual void onLoadResources(){};

	/**
	 * 当场景内的资源被成功加载后 回调此方法
	 * 同步加载后立即回调 如果存在异步方式 
	 * 则异步加载完成后回调
	 * 仅初始化时回调一次
	 */
	virtual void onLoadResourcesCompleted(){};

	/**
	 * 场景资源加载完成后会调用此接口 用于
	 * 初始化场景内所有元素 
	 * 仅初始化时回调一次
	 */
	virtual void onLoadScene(){};

	/**
	 * 当场景被加入渲染树时回调
	 * 根据addChild与removeChild来触发onEnter与onExit
	 */
	virtual void onEnter();

	/**
	 * 当场景从渲染树上删除时调用
	 * 根据addChild与removeChild来触发onEnter与onExit
	 */
	virtual void onExit();

public:
	// 获取场景是否以缓存形式存在
	virtual bool isCachable();
	// 获取场景类名
	const char* getClassName();

	// 当触摸按下时调用
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {return false;}
	// 当触摸移动时调用
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}
	// 当触摸结束时调用
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}
	// 当触摸被中断时调用
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}
	
public:
	virtual bool init();

protected:
	// 本场景触摸优先级
	int m_nTouchPriority;
	// 获取场景管理器在本场景onEnter时为本场景分配的触摸优先级
	int getTouchPriority() const;
	// 获取附加参数
	CCObject* getExtraObject();

	// 以同步的方式加载图像资源
	void addImage(const char* pFile);
	// 以异步的方式加载图像资源
	void addImageAsync(const char* pFile);

private:
	friend class CCDirectorEx;
	bool m_bFirstShow;
	CCObject* m_pExtraObject;
	std::string m_strClassName;

	int m_nLoadCount;
	int m_nLoadCounter;

	std::vector<std::string> m_vLoadImageFilesSync;
	std::vector<std::string> m_vLoadImageFilesAsync;

private:
	void setClassName(const char* pClassName);
	void setExtraObject(CCObject* pExtraObject);
	void setFirstShow(bool bFirstShow);
	bool isFirstShow() const;

	bool isLoadingResourcesAsync();
	void loadResourcesSync();
	void loadResourcesAsync();
	void loadingResourcesCallBack(CCObject* pTextureObj);
};

NS_CC_END

#endif //__CCBASE_SCENEEX_H__