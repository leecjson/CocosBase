#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "../../CocosBase/cocos-base.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class MenuScene : public CCSceneExtension
{
public:
	CREATE_SCENE_FUNC(MenuScene);

public:
	MenuScene();
	virtual ~MenuScene();

	virtual void onLoadScene();

private:
	CCScrollView* m_pScrollView;
	vector<string> m_vMenuItems;
	void onClick(CCObject* sender);
};

#endif //__MENUSCENE_H__