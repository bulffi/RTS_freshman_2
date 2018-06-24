#pragma once
#ifndef UNIT_H
#define UNIT_H
#include"cocos2d.h"
class unit :public cocos2d::Sprite
{
public:
	static unit* create(const char* filename);

	//需要调用子子类init_data来初始化的变量
	int my_country = 0;//所属阵营
	int health = 0;//生命值
	int beginhealth;//初始生命值

	//状态变量
	cocos2d::Action* be_attack = 0;//被攻击动作

};


#endif // !UNIT_H
