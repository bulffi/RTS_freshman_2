#ifndef __HELPSCENE_H__
#define __HELPSCENE_H__
#include "cocos2d.h"
USING_NS_CC;

class helpScene : public cocos2d::Layer
{
	int where_am_i ;// have to be changed Don't use a global!!!!
public:
	static cocos2d::Scene*createScene();
	virtual bool init();
	CREATE_FUNC(helpScene);
	MenuItem* creatNext();
	MenuItem*creatPrivious();
	void menuBackCallback(Ref *pSender);
	void menuNextCallback(Ref *pSender);
	void menuPriviousCallback(Ref *pSender);
};





#endif
