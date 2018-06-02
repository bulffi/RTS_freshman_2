#ifndef __CHOOSE_TO_BESCENE__H_
#define __CHOOSE_TO_BESCENE__H_
#include"cocos2d.h"

class choose_to_beScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene*createScene();
	virtual bool init();
	CREATE_FUNC(choose_to_beScene);

private:
	void gustCallback(Ref* Psender);
	void hostCallback(Ref*);
	void menuBackCallback(Ref*);
};






#endif