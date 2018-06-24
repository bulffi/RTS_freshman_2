#ifndef __SETTINGSSCENE_H__
#define __SETTINGSSCENE_H__
#include "cocos2d.h"
USING_NS_CC;
class settingsScene : public cocos2d::Layer
{
	void menuBackCallback(Ref *pSender);
	void menuEffectToggleCallback(Ref *pSender);
	void menuBackmusicToggleCallback(Ref* pSender);
	bool isEffect;
	bool isBackgroundMusic;
public:
	static cocos2d::Scene* creatscene();
	virtual bool init();
	CREATE_FUNC(settingsScene);

};

#endif