//#pragma once
#ifndef __SOLDIER_H__
#define __SOLDIER_H__


#include "cocos2d.h"

class soldier :public cocos2d::Sprite
{
public:
	int country = 0;//������Ӫ
	int health = 20;//����ֵ
	int attack = 1;//������
	int speed = 100;//�ٶ�
	int attack_distance = 100;//������Χ
	float attack_speed = 1.0f;//�����ٶ�
	bool can_attack = 1;//�ܷ������һ�ι���
	soldier* enemy_target = 0;//����������Ŀ��

	static soldier* create(const char* filename);//���캯��
	void update(float dt) override;//��λ��ײ
	void updateMove(float dt);//����о�
	void updateAttack(float dt);//�����о�
	void move_all_time(float dt);//���޲���ʹ�ã��Ժ�Ҫɾ�����õ��˲�ͣ�ƶ�
	int move_direction = 1;//���޲���ʹ�ã��Ժ�Ҫɾ��
	//void updateHealth(float dt);//�������ֵ
	float my_positionX;
	float my_positionY;
	cocos2d::Action* move_action = 0;//�ƶ�����
	cocos2d::Action* attack_action = 0;//��������
	cocos2d::Action* be_attack = 0;//����������
};



#endif // __SOLDIER_H__