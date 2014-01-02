#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "../CocosBase/cocos-base.h"
using namespace cocos2d;

class MainScene : public CCSceneEx
{
public:
	CREATE_SCENE_FUNC(MainScene);

public:
	MainScene();
	virtual void onLoadResources();
	virtual void onLoadScene();
	virtual void onEnter();
	virtual void onMessage(unsigned int uMsg, CCObject* pMsgObj, void* wParam, void* lParam);
	virtual bool isCachable(){ return true; }
	void onClick(CCObject* pSender);

private:
	CCLabelTTF* m_pNamePassword;
	CCMenuItemImage* pBack;
	CCMenuItemImage* pBackpack;
	CCMenu* pMenu;
};

#endif //__MAIN_SCENE_H__