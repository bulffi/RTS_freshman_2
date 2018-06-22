#pragma
#ifndef HELLOWORLDSCENE_H
#define HELLOWORLDSCENE_H


#include "cocos2d.h"
#include"my_Network\client.h"
//���ָ���
#include"soldier.h"
#include"building.h"
//���ֽ���
#include"camp.h"
#include"electric.h"
#include"base.h"
#include"mine.h"
#include"factory.h"
//����ʿ��
#include"people.h"
#include"dog.h"
#include"tank.h"

#include"menu.h"

#define XMAX 50
#define YMAX 50

class HelloWorld : public cocos2d::Layer
{
	
public:
	static int situation[XMAX*YMAX];

	static cocos2d::TMXTiledMap* _tileMap;
	cocos2d::TMXLayer* _colliable;
    static cocos2d::Scene* createScene(client*,int);

    virtual bool init();

	int my_country;//��Ӫ
	int num_player;//�������

	base* base1;//1������
	base* base2;//2������
	base* base3;//3������
	base* base4;//4������

	//�����濼�Ǹĳ�����ָ��
	static std::vector<std::vector<people*>> vec_people;//������С��
	static std::vector<std::vector<dog*>> vec_dog;//�����Ĺ�
	static std::vector<std::vector<tank*>> vec_tank;//������̹��

	static std::vector<std::vector<camp*>> vec_camp;//�����ı�Ӫ
	static std::vector<std::vector<mine*>> vec_mine;//�����Ŀ�
	static std::vector<std::vector<electric*>> vec_electric;//�����ĵ糧
	static std::vector<std::vector<factory*>> vec_factory;//������ս������

	static std::vector<std::vector<soldier*>> vec_chosed_soldier;//������ѡ�е�λ


	std::vector<int> set_building;//�Ƿ����ڽ��콨��
	std::vector<cocos2d::Vec2> mouseDownPosition;//��갴�µ�����
	static std::vector<camp*> major_camp;//��������Ҫ��Ӫ
	static std::vector<factory*> major_factory;//��������Ҫս������


	void menuItemsoldierCallback(cocos2d::Ref* pSender);
	void menuItemdogCallback(cocos2d::Ref* pSender);
	void menuItemtankCallback(cocos2d::Ref* pSender);
	void menuItemcampCallback(cocos2d::Ref* pSender);
	void menuItemmineCallback(cocos2d::Ref* pSender);
	void menuItemelectricCallback(cocos2d::Ref* pSender);
	void menuItemfactoryCallback(cocos2d::Ref* pSender);

	void update(float dt) override;//���շ�������Ϣ
	void updateHealth(float dt);//���Ѫ��
	cocos2d::String message;//���ڴ����������Ϣ

	void button_soldier(int country);
	void button_dog(int country);
	void button_tank(int country);
	void button_camp(int country);
	void button_electric(int country);
	void button_mine(int country);
	void button_factory(int country);


	
	static void createblood(unit* s,int x);

	//������
	int money[5] = { 99999,99999,99999,99999,99999 };//��Ǯ
	int elec[5] = { 1000,1000,1000,1000,1000 };//����
	int electotal[5] = { 1000,1000,1000,1000,1000 };//�ܵ���
	//int money = 9999;
	//int elec = 1000;
	//int electotal = 1000;
	cocos2d::Vec2 startpos;//�����ʼλ��
	cocos2d::Vec2 mappos;//�����ͼλ��

	int iftogo = -1, bui_lable = -1, sol_lable = -1;//�����Ƿ����
	static cocos2d::Vector<build_menu*> vec_buimenu;//���潨���˵�������
	static cocos2d::Vector<sol_menu*> vec_solmenu;//������ֲ˵�������
	

	void initmenu();//��ʾ����
	void addmenu(int lable,int country);//�ж��Ƿ���Ҫ���Ӳ˵���
	int iftobuild(cocos2d::Vec2 pos,int country);//̧������ͬһ�㣬�ж��Ƿ���н����˵���׼������
	int ifbuild(cocos2d::Vec2 pos,int country);//̧�����²�ͬ�㣬�ж��Ƿ���н����˵�����ʼ����
	int ifbuildsol(cocos2d::Vec2 pos,int country);//�ж��Ƿ���б��ֲ˵�

	bool create_a_electric(int country, cocos2d::Point tile_position);
	bool create_a_mine(int country, cocos2d::Point tile_position);
	bool create_a_camp(int country, cocos2d::Point tile_position);
	bool create_a_factory(int country, cocos2d::Point tile_position);
	
	void destruct_last_solbuild(int country,char c_or_f);

//	cocos2d::Vec2 move_to_GLposition(unit* penemy, int enemy_type);

    CREATE_FUNC(HelloWorld);
};

#endif // !HELLOWORLDSCENE_H