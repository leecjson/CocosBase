#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "../CocosBase/cocos-base.h"
using namespace cocos2d;

class LoginScene : public CCSceneEx
{
public:
	CREATE_SCENE_FUNC(LoginScene);

public:
	LoginScene();
	virtual ~LoginScene();
	virtual void onLoadScene();
	
	void onClick(CCObject* pSender);
};


#endif //__LOGIN_SCENE_H__