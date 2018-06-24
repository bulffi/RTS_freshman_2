#ifndef __LOSESCENE_H__
#define __LOSESCENE_H__
#include "cocos2d.h"
USING_NS_CC;

class loseScene : public cocos2d::Layer
{
	int where_am_i;// have to be changed Don't use a global!!!!
public:
	static cocos2d::Scene*createScene();
	virtual bool init();
	CREATE_FUNC(loseScene);
};





#endif
