//#pragma once
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include"soldier\soldier.h"
#include"buliding\building.h"

class HelloWorld : public cocos2d::Layer
{
	cocos2d::Sprite* _player;
	cocos2d::TMXTiledMap* _tilemap;
	cocos2d::TMXLayer* _collidable;

public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	int set_building = 0;
	cocos2d::Vec2 mouseDownPosition;//第一次点鼠标时鼠标的位置
	static cocos2d::Vector<soldier*> vec_soldier;//存储我方小兵的容器√
	static cocos2d::Vector<soldier*> vec_chosed_soldier;//存储我方选中小兵的容器
	static cocos2d::Vector<soldier*> vec_enemy;//存储敌方小兵的容器√
	static cocos2d::Vector<building*> vec_building;//存储我方建筑的容器√

	void menuItem1Callback(cocos2d::Ref* pSender);
	void menuItem2Callback(cocos2d::Ref* pSender);
	void menuItem3Callback(cocos2d::Ref* pSender);

	void update(float dt) override;//监测所有对象生命值

	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
