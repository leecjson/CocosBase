#ifndef __CCBASE_DIALOG_H__
#define __CCBASE_DIALOG_H__

#include "CCDirectorEx.h"
#include "CCMessageProtocol.h"
#include <string>

NS_CC_BEGIN

class CCDialog : public CCNode, public CCMessageProtocol, public CCTouchDelegate
{
public:
	CCDialog();
	virtual ~CCDialog();

	/**
	 * 当初始化对话框时调用
	 * 仅初始化时执行一次
	 */
	virtual void onLoadDialog(){};

	/**
	 * 当对话框被加入渲染树时回调
	 * 根据addChild与removeChild来触发onEnter与onExit
	 */
	virtual void onEnter();

	/**
	 * 当对话框从渲染树上删除时调用
	 * 根据addChild与removeChild来触发onEnter与onExit
	 */
	virtual void onExit();

public:
	// 是否是模态形式
	virtual bool isModalable();
	// 获取对话框是否以缓存形式存在
	virtual bool isCachable();
	// 获取对话框类名
	const char* getClassName();

	// 当触摸按下时调用
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	// 当触摸移动时调用
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}
	// 当触摸结束时调用
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}
	// 当触摸被中断时调用
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}

public:
	virtual bool init();

protected:
	// 本对话框触摸优先级
	int m_nTouchPriority;
	// 获取管理器在本对话框onEnter时为本场景分配的触摸优先级
	int getTouchPriority() const;
	// 获取附加参数
	CCObject* getExtraObject();

private:
	friend class CCDirectorEx;
	CCObject* m_pExtraObject;
	std::string m_strClassName;

private:
	void setClassName(const char* pClassName);
	void setExtraObject(CCObject* pExtraObject);
};

NS_CC_END

#endif //__CCBASE_DIALOG_H__