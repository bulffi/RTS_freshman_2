#pragma once
#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"
//#include"HelloWorldScene.h"
class menu :public cocos2d::Sprite
{
public:
	int lable;//���
	int money = 100;//�ķѽ��
	int elec = 100;//�ķѵ���
	int country_did;//���������Ĺ���
	int mycountry;//���ڲ����Ĺ���
	cocos2d::Vec2 position;//position
};

class build_menu :public menu
{
public:
	int ifready[5] = { -2,-2,-2,-2, -2 };//�Ƿ�׼�����,-2����δ���֣�-1����δ���죬0��ʾ���ڽ��죬1�����Ѿ���
	//cocos2d::Sprite* menu_ready;//����ͼƬ

	static build_menu* create(int num);//���캯��
	//int iftobuild(cocos2d::Vec2 pos);//̧������ͬһ�㣬�ж��Ƿ���н����˵���׼������
	//int ifbuild(cocos2d::Vec2 pos);//̧�����²�ͬ�㣬�ж��Ƿ���н����˵�����ʼ����
	void recreate(int my_country,int country);
	void build(cocos2d::Vec2 pos,int country);//ִ�н�������
	void cretimer(int my_country,int country);//�����˵�CD��
	void updateload1(float dt);//����CD��
	void updateload2(float dt);//����CD��
	void updateload3(float dt);//����CD��
	void updateload4(float dt);//����CD��
};

class sol_menu :public menu
{
public:
	int num_tobuild[5] = { 0 };//�ȴ��������Ŀ
	int ifready[5] = { 0 };//0����δ���ֻ��߱��ݻ�,1�����ѳ���
	static sol_menu* create(int num);//���캯��
	void recreate(int my_country, int country);
	//int ifbuildsol(cocos2d::Vec2 pos);//�ж��Ƿ���б��ֲ˵�
	void buildsol(int country);//ִ�н�����ֲ���
	void buildsol1();//ִ�н�����ֲ���
	void buildsol2();//ִ�н�����ֲ���
	void buildsol3();//ִ�н�����ֲ���
	void buildsol4();//ִ�н�����ֲ���
	void cretimer(int my_country,int country);//�����˵�CD��
	void updateload1(float dt);//����CD��
	void updateload2(float dt);
	void updateload3(float dt);//����CD��
	void updateload4(float dt);//����CD��
};

#endif // __MENU_H__