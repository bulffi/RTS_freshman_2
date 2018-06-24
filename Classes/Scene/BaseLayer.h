#ifndef __BaseLayer_Layer_H__
#define __BaseLayer_Layer_H__
#include "cocos2d.h"
#include"helpscene.h"
#include"settingsScene.h"
#include"SimpleAudioEngine.h"
#include"loginScnen.h"
USING_NS_CC;
class BaseLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene*createScene();
	virtual bool init();
	CREATE_FUNC(BaseLayer);
	void menuPlayCallback(Ref *pSender);
	void menuSettingsCallback(Ref *pSender);
	void menuHelpCallback(Ref *pSender);
	void menuExitCallback(Ref *pSender);

	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();

};



#endif
