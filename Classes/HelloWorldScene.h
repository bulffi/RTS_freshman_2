//#pragma once
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "soldier.h"
#include "menu.h"


class HelloWorld : public cocos2d::Layer
{
	cocos2d::Sprite* _player;

public:
    static cocos2d::Scene* createScene();
	virtual bool init();
    static cocos2d::TMXTiledMap* tilemap;
	/*int bui_drag = -1;//��ʾ�Ƿ��ڽ���״̬
	int sol_build = -1;//��ʾ�Ƿ���н�����ֲ˵�
	int build_ready[4] = { -1,-1,-1,-1 };//�жϽ����Ƿ�׼������,-1δ�����0׼���У�1�Ѿ���
	int sol_tobuild[3] = { 0 };//�жϱ����ж��ٵȴ�����
	int build_money[4] = { 100,100,100,100 };//���������Ǯ
	int build_elec[4] = { 100,100,100,100 };//�����������
	int sol_money[3] = { 100,100,100 };
	int sol_elec[3] = { 100,100,100 };*/

	int money = 99999;//��Ǯ
	int elec = 99999;//����
	cocos2d::Vec2 startpos;//�����ʼλ��
	cocos2d::Vec2 mappos;//�����ͼλ��
	cocos2d::Vec2 mouseDownPosition;//��һ�ε����ʱ����λ��
	cocos2d::Vec2 mouseUpPosition;//���̧��ʱ��λ��
	static cocos2d::Vector<soldier*> vec_soldier;//�洢�ҷ�С��������
	static cocos2d::Vector<soldier*> vec_chosed_soldier;//�洢�ҷ�ѡ��С��������
	static cocos2d::Vector<soldier*> vec_enemy;//�洢�з�С��������

	int iftogo = -1,bui_lable=-1,sol_lable=-1;//�����Ƿ����
    static cocos2d::Vector<build_menu*> vec_buimenu;//���潨���˵�������
	static cocos2d::Vector<sol_menu*> vec_solmenu;//������ֲ˵�������
    static cocos2d::Vector<cocos2d::Sprite*> vec_building;//���潨��������
	static cocos2d::Vector<cocos2d::Sprite*> vec_solbuild;//�����Ӫ������
	static cocos2d::Vector<cocos2d::Sprite*> vec_carbuild;//����ս������������

    void initmenu();//��ʾ����
    void addmenu(int lable);//�ж��Ƿ���Ҫ���Ӳ˵���
	int iftobuild(cocos2d::Vec2 pos);//̧������ͬһ�㣬�ж��Ƿ���н����˵���׼������
	int ifbuild(cocos2d::Vec2 pos);//̧�����²�ͬ�㣬�ж��Ƿ���н����˵�����ʼ����
	int ifbuildsol(cocos2d::Vec2 pos);//�ж��Ƿ���б��ֲ˵�
	/*
	cocos2d::Vector<cocos2d::Sprite*> vec_buimenu_ready ;//���潨�������˵�������
	//cocos2d::Vector<cocos2d::Sprite*> vec_solmenu_ready ;//������ֲ˵�������
    void buildsol( int i);//ִ�н�����ֲ���
	void cretimer(int i);//�����˵�CD��
	void updateload(float dt);//����CD��
	void updateload2(float dt);//����CD��*/

	void menuItem1Callback(cocos2d::Ref* pSender);
	void menuItem2Callback(cocos2d::Ref* pSender);
	static void createblood(soldier* s);

	void update(float dt) override;//������ж�������ֵ

    CREATE_FUNC(HelloWorld);
};

#endif// __HELLOWORLD_SCENE_H__
