#ifndef __BASICSCENETEST_H__
#define __BASICSCENETEST_H__

#include "../../../../CocosBase/cocos-base.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class BasicSceneTest : public CCSceneExtension
{
public:
	CREATE_SCENE_FUNC(BasicSceneTest);

public:
	BasicSceneTest();
	virtual ~BasicSceneTest();
	virtual void onLoadResources();
	virtual void onLoadScene();
	virtual void onEnterScene();

private:
	CCMenu* m_pMenu;
	void onClick(CCObject* sender);
};


class BasicUIScene : public CCSceneExtension
{
public:
	CREATE_SCENE_FUNC(BasicUIScene);

	virtual void onLoadResources();
	virtual void onLoadScene();
	virtual void onEnterScene();

private:
	CCMenu* m_pMenu;
	void onClick(CCObject* sender);
};



#endif //__BASICSCENE_H__