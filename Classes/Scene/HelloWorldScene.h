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
	cocos2d::Vec2 mouseDownPosition;//��һ�ε����ʱ����λ��
	static cocos2d::Vector<soldier*> vec_soldier;//�洢�ҷ�С����������
	static cocos2d::Vector<soldier*> vec_chosed_soldier;//�洢�ҷ�ѡ��С��������
	static cocos2d::Vector<soldier*> vec_enemy;//�洢�з�С����������
	static cocos2d::Vector<building*> vec_building;//�洢�ҷ�������������

	void menuItem1Callback(cocos2d::Ref* pSender);
	void menuItem2Callback(cocos2d::Ref* pSender);
	void menuItem3Callback(cocos2d::Ref* pSender);

	void update(float dt) override;//������ж�������ֵ

	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
