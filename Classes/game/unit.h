#pragma once
#ifndef UNIT_H
#define UNIT_H
#include"cocos2d.h"
class unit :public cocos2d::Sprite
{
public:
	static unit* create(const char* filename);

	//��Ҫ����������init_data����ʼ���ı���
	int my_country = 0;//������Ӫ
	int health = 0;//����ֵ
	int beginhealth;//��ʼ����ֵ

	//״̬����
	cocos2d::Action* be_attack = 0;//����������

};


#endif // !UNIT_H
