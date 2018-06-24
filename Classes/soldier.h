//#pragma once
#ifndef __SOLDIER_H__
#define __SOLDIER_H__


#include "cocos2d.h"

class soldier :public cocos2d::Sprite
{
public:
	int country = 0;//所属阵营
	int health = 20;//生命值
	int attack = 1;//攻击力
	int speed = 100;//速度
	int attack_distance = 100;//攻击范围
	float attack_speed = 1.0f;//攻击速度
	bool can_attack = 1;//能否进行下一次攻击
	soldier* enemy_target = 0;//即将攻击的目标

	static soldier* create(const char* filename);//构造函数
	void update(float dt) override;//单位碰撞
	void updateMove(float dt);//移向敌军
	void updateAttack(float dt);//攻击敌军
	void move_all_time(float dt);//仅限测验使用，以后要删掉，让敌人不停移动
	int move_direction = 1;//仅限测验使用，以后要删掉
	//void updateHealth(float dt);//监测生命值
	float my_positionX;
	float my_positionY;
	cocos2d::Action* move_action = 0;//移动动作
	cocos2d::Action* attack_action = 0;//攻击动作
	cocos2d::Action* be_attack = 0;//被攻击动作
};



#endif // __SOLDIER_H__