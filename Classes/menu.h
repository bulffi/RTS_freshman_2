//#pragma once
#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"

class menu :public cocos2d::Sprite
{
public:
	int lable;//���
	int money = 100;//�ķѽ��
	int elec = 100;//�ķѵ���
	cocos2d::Vec2 position;//position
};

class build_menu :public menu
{
public:
	int ifready = -1;//�Ƿ�׼�����,-1����δ���죬0��ʾ���ڽ��죬1�����Ѿ���
	//cocos2d::Sprite* menu_ready;//����ͼƬ

	static build_menu* create(int num);//���캯��
	//int iftobuild(cocos2d::Vec2 pos);//̧������ͬһ�㣬�ж��Ƿ���н����˵���׼������
	//int ifbuild(cocos2d::Vec2 pos);//̧�����²�ͬ�㣬�ж��Ƿ���н����˵�����ʼ����
	void build(cocos2d::Vec2 pos);//ִ�н�������
	void cretimer();//�����˵�CD��
	void updateload(float dt);//����CD��
};

class sol_menu :public menu
{
public:
	int num_tobuild = 0;//�ȴ��������Ŀ

	static sol_menu* create(int num);//���캯��
	//int ifbuildsol(cocos2d::Vec2 pos);//�ж��Ƿ���б��ֲ˵�
	void buildsol();//ִ�н�����ֲ���
	void cretimer();//�����˵�CD��
	void updateload(float dt);//����CD��
};

#endif // __MENU_H__