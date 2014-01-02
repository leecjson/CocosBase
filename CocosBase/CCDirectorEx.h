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
#ifndef __CCBASE_DIRECTOREX_H__
#define __CCBASE_DIRECTOREX_H__

/////////////////////////////////////////////////////////////////////////////
/// BugFix : 
///
/////////////////////////////////////////////////////////////////////////////

#include "cocos2d.h"
#include "CCBundle.h"
#include "CCMessageProtocol.h"
#include "CCSceneEx.h"
#include "CCDialog.h"
#include <list>
#include <map>
#include <string>

NS_CC_BEGIN

class CCPopupScene;
class CCDirectorEx;
class CCBundle;
class CCSceneEx;
class CCDialog;

#ifndef CREATEFNC
#define CREATEFNC
typedef CCSceneEx* (*Fn_CreateScene)();
typedef CCDialog* (*Fn_CreateDialog)();
#endif //CREATEFNC

#ifndef CREATE_SCENE_FUNC
#define CREATE_SCENE_FUNC(_CLASS_) \
static CCSceneEx* createThisScene() { return new _CLASS_; }
#endif //CREATE_SCENE_FUNC

#ifndef CREATE_DIALOG_FUNC
#define CREATE_DIALOG_FUNC(_CLASS_) \
static CCSceneEx* createThisDialog() { return new _CLASS_; }
#endif //CREATE_DIALOG_FUNC

#ifndef REGISTER_SCENE_FUNC
#define REGISTER_SCENE_FUNC(_CLASS_) \
CCDirectorEx::sharedDirector()->registerSceneClass(#_CLASS_, &_CLASS_::createThisScene)
#endif //REGISTER_SCENE_FUNC

#ifndef REGISTER_DIALOG_FUNC
#define REGISTER_DIALOG_FUNC(_CLASS_) \
CCDirectorEx::sharedDirector()->registerDialogClass(#_CLASS_, &_CLASS_::createThisDialog)
#endif //REGISTER_DIALOG_FUNC


#ifndef SeekScene
#define SeekScene(_CLASS_) \
(CCDirectorEx::sharedDirector()->seekScene(_CLASS_))
#define SeekSceneExtra(_CLASS_, _OBJ_) \
(CCDirectorEx::sharedDirector()->seekScene(_CLASS_, _OBJ_))
#define SeekDialog(_CLASS_) \
(CCDirectorEx::sharedDirector()->seekDialog(_CLASS_))
#define SeekDialogExtra(_CLASS_, _OBJ_) \
(CCDirectorEx::sharedDirector()->seekDialog(_CLASS_, _OBJ_))
#endif //SeekScene


/**
 * class    : CCDirectorEx
 * author   : viva - Lijunlin
 * email    : csdn_viva@foxmail.com
 * function : 导演拓展类 用于管理所有场景的声明周期，消息等。
 */
class CCDirectorEx : public CCNode
{
private:
	CCDirectorEx();
	virtual ~CCDirectorEx();

public:
	// 获取单实例对象
	static CCDirectorEx* sharedDirector();
	// 清理所有内容
	void purgeDirector();

public:
	// 注册一个消息接收器
	void registerMessageHandler(CCMessageProtocol* pProtocol);
	// 取消注册指定的消息接收器
	void unregisterMessageHandler(CCMessageProtocol* pProtocol);

	// 发送消息到所有消息接收器
	void PostMessage(unsigned int uMsg, CCObject* pMsgObj = NULL, void* wParam = NULL, void* lParam = NULL);
	// 发送消息到指定消息接收器
	void PostMessage(CCMessageProtocol* pProtocol, unsigned int uMsg, CCObject* pMsgObj = NULL, void* wParam = NULL, void* lParam = NULL);

public:
	// 运行场景
	void runWithScene(CCScene* pScene);
	// 替换栈顶场景
	void replaceScene(CCScene* pScene);
	// 场景入栈
	void pushScene(CCScene* pScene);
	// 场景出栈
	void popScene();
	// 连续出栈到栈底
	void popToRootScene();
	// 连续出栈到指定等级 等级为零将退出游戏
	void popToSceneStackLevel(int nLevel);

	// 替换栈顶悬浮场景
	void replacePopupScene(CCScene* pScene);
	// 悬浮场景入栈
	void pushPopupScene(CCScene* pScene);
	// 悬浮场景出栈
	void popPopupScene();
	// 连续出栈到栈底
	void popToRootPopupScene();
	// 连续出栈到指定等级 等级为零将关闭所有悬浮场景
	void popToPopupSceneStackLevel(int nLevel);

	// 退出游戏
	void end();

	// 获取当前运行场景
	CCScene* getRunningScene();
	// 获取当前运行的悬浮场景
	CCScene* getRunningPopupScene();

	// 通过场景类名解锁场景的切换
	void unloackSceneSwitch(const std::string& strClassName);

public:
	// 移除指定缓存场景
	void removeCachedScene(const char* pSceneName);
	// 移除所有缓存场景
	void removeAllCachedScenes();
	// 移除所有计数为1的场景
	void removeUnusedCachedScenes();

public:
	// 获得触摸优先级 每次调用减1
	int getTouchPriority();
	// 锁定触摸操作
	void lockTouchEvent();
	// 解锁触摸操作
	void unlockTouchEvent();

public:
	// 注册场景实例
	void registerSceneClass(const char* pSceneName, Fn_CreateScene pFn);
	// 搜索场景 存在则取出 不存在则创建
	CCSceneEx* seekScene(const char* pSceneName, CCObject* pExtraObj = NULL);

public:
	// 注册对话框实例
	void registerDialogClass(const char* pDialogName, Fn_CreateDialog pFn);
	// 搜索对话框实例
	CCDialog* seekDialog(const char* pDialogName, CCObject* pExtraObj = NULL);

	// 显示对话框
	void showDialog(CCDialog* pDialog, int nZOrder = 0);
	// 关闭对话框
	void closeDialog(const char* pDialogName);
	// 移除指定缓存对话框
	void removeCachedDialog(const char* pSceneName);
	// 移除所有缓存对话框
	void removeAllCachedDialogs();
	// 移除所有计数为1的对话框
	void removeUnusedCachedDialogs();

private:
	// 消息结构定义
	struct tagMessage
	{
		void*        wParam;   // 参数1
		void*        lParam;   // 参数2
		CCObject*    pMsgObj;  // 对象参数
		unsigned int uMsg;     // 消息号
	};

	// 目标消息结构定义
	struct tagTarget
	{
		tagMessage			tMessage;			// 消息结构
		CCMessageProtocol*  pTarget;			// 目标对象
	};

	// 场景切换方式
	enum SceneSwitchType
	{
		eSceneSwitchRunWithScene,				// 运行场景
		eSceneSwitchPushScene,					// 场景入栈
		eSceneSwitchReplaceScene,				// 替换栈顶场景
		eSceneSwitchPopScene,					// 场景出栈 当没有场景可出栈时退出游戏
		eSceneSwitchPopToSceneStackLevel,		// 场景连续出栈 当没有场景可出栈时退出游戏

		eSceneSwitchPushPopupScene,				// 悬浮场景入栈
		eSceneSwitchReplacePopupScene,			// 替换栈顶悬浮场景
		eSceneSwitchPopPopupScene,				// 悬浮场景出栈
		eSceneSwitchPopToPopupSceneStackLevel,	// 悬浮场景连续出栈
	};

	// 场景切换结构定义
	struct tagSceneSwitch
	{
		bool             bIsLocked;				// 是否已经解锁
		int              nPopLevel;				// 连续出栈到等级
		std::string      strClassName;			// 本次切换场景的类名
		CCScene*         pNextScene;			// 下一个场景
		SceneSwitchType  eType;					// 切换类型
	};

private:
	// 主循环
	void mainLoop(float dt);
	// 设置下一个悬浮场景
	void setNextPopupScene(CCScene* pNextScene, bool bCleanUp);
	// 执行场景切换
	void performSceneSwitch(SceneSwitchType eType, CCScene* pScene = NULL, int nLevel = 0);
	// 处理场景切换
	void handleSceneSwitch(tagSceneSwitch& tag);
public:
	virtual void visit();

private:
	// 触摸事件是否被锁定
	bool m_bTouchEventLocked;
	// 主循环是否被锁定
	bool m_bMainLoopLocked;
	// 被搜索出的场景，用于下一次切换
	CCSceneEx* m_pNextSwitchScene;

	// 当前运行的悬浮场景
	CCScene* m_pRunningPopupScene;
	// 场景栈
	CCArray* m_pSceneStack;
	// 悬浮场景栈
	CCArray* m_pPopupSceneStack;

	std::vector<tagMessage> m_vMsgTemps;
	std::list<tagMessage> m_lMsgQueue;
	std::vector<tagTarget> m_vTargetMsgTemps;
	std::list<tagTarget> m_lTargetMsgQueue;
	std::list<CCMessageProtocol*> m_lMsgHandlers;
	std::list<CCMessageProtocol*> m_lMsgHandlersRemoved;
	std::list<CCMessageProtocol*> m_lMsgHandlersAdded;
	std::list<tagSceneSwitch> m_lSceneSwitchQueue;
	std::map<std::string, Fn_CreateScene> m_mFnSceneCreatePointers;
	std::map<std::string, CCSceneEx*> m_mSceneCachePool;
	std::map<std::string, Fn_CreateDialog> m_mFnDialogCreatePointers;
	std::map<std::string, CCDialog*> m_mDialogCachePool;
};

NS_CC_END

#endif //__CCBASE_DIRECTOREX_H__