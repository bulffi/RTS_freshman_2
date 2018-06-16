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
	/*int bui_drag = -1;//表示是否处于建筑状态
	int sol_build = -1;//表示是否点中建造兵种菜单
	int build_ready[4] = { -1,-1,-1,-1 };//判断建筑是否准备就绪,-1未点击，0准备中，1已就绪
	int sol_tobuild[3] = { 0 };//判断兵种有多少等待建造
	int build_money[4] = { 100,100,100,100 };//建筑所需金钱
	int build_elec[4] = { 100,100,100,100 };//建筑所需电量
	int sol_money[3] = { 100,100,100 };
	int sol_elec[3] = { 100,100,100 };*/

	int money = 99999;//金钱
	int elec = 99999;//电量
	cocos2d::Vec2 startpos;//储存初始位置
	cocos2d::Vec2 mappos;//储存地图位置
	cocos2d::Vec2 mouseDownPosition;//第一次点鼠标时鼠标的位置
	cocos2d::Vec2 mouseUpPosition;//鼠标抬起时的位置
	static cocos2d::Vector<soldier*> vec_soldier;//存储我方小兵的容器
	static cocos2d::Vector<soldier*> vec_chosed_soldier;//存储我方选中小兵的容器
	static cocos2d::Vector<soldier*> vec_enemy;//存储敌方小兵的容器

	int iftogo = -1,bui_lable=-1,sol_lable=-1;//操作是否继续
    static cocos2d::Vector<build_menu*> vec_buimenu;//储存建筑菜单的容器
	static cocos2d::Vector<sol_menu*> vec_solmenu;//储存兵种菜单的容器
    static cocos2d::Vector<cocos2d::Sprite*> vec_building;//储存建筑的容器
	static cocos2d::Vector<cocos2d::Sprite*> vec_solbuild;//储存兵营的容器
	static cocos2d::Vector<cocos2d::Sprite*> vec_carbuild;//储存战车工厂的容器

    void initmenu();//显示旁栏
    void addmenu(int lable);//判断是否需要增加菜单栏
	int iftobuild(cocos2d::Vec2 pos);//抬起落下同一点，判断是否点中建筑菜单，准备建造
	int ifbuild(cocos2d::Vec2 pos);//抬起落下不同点，判断是否点中建筑菜单，开始建造
	int ifbuildsol(cocos2d::Vec2 pos);//判断是否点中兵种菜单
	/*
	cocos2d::Vector<cocos2d::Sprite*> vec_buimenu_ready ;//储存建筑就绪菜单的容器
	//cocos2d::Vector<cocos2d::Sprite*> vec_solmenu_ready ;//储存兵种菜单的容器
    void buildsol( int i);//执行建造兵种操作
	void cretimer(int i);//创建菜单CD条
	void updateload(float dt);//更新CD条
	void updateload2(float dt);//更新CD条*/

	void menuItem1Callback(cocos2d::Ref* pSender);
	void menuItem2Callback(cocos2d::Ref* pSender);
	static void createblood(soldier* s);

	void update(float dt) override;//监测所有对象生命值

    CREATE_FUNC(HelloWorld);
};

#endif// __HELLOWORLD_SCENE_H__
