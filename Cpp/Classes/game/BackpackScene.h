#ifndef __BACKPACK_SCENE_H__
#define __BACKPACK_SCENE_H__

#include "../CocosBase/cocos-base.h"
using namespace cocos2d;

class BackpackScene : public CCSceneEx
{
public:
	CREATE_SCENE_FUNC(BackpackScene);

public:
	virtual void onLoadResources();
	virtual void onLoadScene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onMessage(unsigned int uMsg, CCObject* pMsgObj, void* wParam, void* lParam);

	CCMenuItemImage* pClose;
	CCMenuItemImage* pPost;
	CCMenu* pMenu;
	void onClick(CCObject* pSender);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){ return true; }
};

#endif //__BACKPACK_SCENE_H__