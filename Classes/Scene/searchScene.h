#ifndef __SEARCH_SCENE_H__
#define __SEARCH_SCENE_H__
#include"cocos2d.h"
#include"my_Network\client.h"
//USING_NS_CC;
class searchScene :public cocos2d::Layer
{
	void update(float dt) override ;
	void joinCallback(Ref* pSender);
public:
	static cocos2d::Scene* createScene(talk_to_svr::ptr);
	virtual bool init();
	CREATE_FUNC(searchScene);

};










#endif
