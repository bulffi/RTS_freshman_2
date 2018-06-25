#pragma
#ifndef HELLOWORLDSCENE_H
#define HELLOWORLDSCENE_H


#include "cocos2d.h"
#include"my_Network\client.h"
//两种父类
#include"soldier.h"
#include"building.h"
//五种建筑
#include"camp.h"
#include"electric.h"
#include"base.h"
#include"mine.h"
#include"factory.h"
//三种士兵
#include"people.h"
#include"dog.h"
#include"tank.h"

#include"menu.h"

/*#define XMAX 50
#define YMAX 50*/

class HelloWorld : public cocos2d::Layer
{
	
public:
	
	static std::vector<int> situation;

	static cocos2d::TMXTiledMap* _tileMap;
	cocos2d::TMXLayer* _colliable;
    static cocos2d::Scene* createScene(client*,int);

    virtual bool init();

	static int my_country;///第几个
	static int num_player;///总共

	static std::vector<base*> vec_base;

	//到后面考虑改成智能指针
	static std::vector<std::vector<people*>> vec_people;//各方的小兵
	static std::vector<std::vector<dog*>> vec_dog;//各方的狗
	static std::vector<std::vector<tank*>> vec_tank;//各方的坦克

	static std::vector<std::vector<camp*>> vec_camp;//各方的兵营
	static std::vector<std::vector<mine*>> vec_mine;//各方的矿场
	static std::vector<std::vector<mine_car*>> vec_mine_car;//各方的矿车
	static std::vector<unit*> vec_mines;//各方的矿
	static std::vector<std::vector<electric*>> vec_electric;//各方的电厂
	static std::vector<std::vector<factory*>> vec_factory;//各方的战车工厂

	static std::vector<std::vector<soldier*>> vec_chosed_soldier;//各方的选中单位


	std::vector<int> set_building;//是否正在建造建筑
	std::vector<cocos2d::Vec2> mouseDownPosition;//鼠标按下的坐标
	std::vector<cocos2d::Vec2> real_mouseDownPosition;
	static std::vector<camp*> major_camp;//各方的主要兵营
	static std::vector<factory*> major_factory;//各方的主要战车工厂

	void update(float dt) override;//接收服务器消息
	void updateHealth(float dt);//监测血量
	cocos2d::String message;//用于储存服务器消息

	static void createblood(unit* s,int x);

	//周泓光
	int money[5] = { 300,300,300,300,300 };//金钱
	int elec[5] = { 100,100,100,100,100 };//电量
	int electotal[5] = { 100,100,100,100,100 };//总电量
	
	int iftogo = -1, bui_lable = -1, sol_lable = -1;//操作是否继续
	static cocos2d::Vector<build_menu*> vec_buimenu;//储存建筑菜单的容器
	static cocos2d::Vector<sol_menu*> vec_solmenu;//储存兵种菜单的容器
	

	void initmenu();//显示旁栏
	void addmenu(int lable,int country);//判断是否需要增加菜单栏
	int iftobuild(cocos2d::Vec2 pos,int country);//抬起落下同一点，判断是否点中建筑菜单，准备建造
	int ifbuild(cocos2d::Vec2 pos,int country);//抬起落下不同点，判断是否点中建筑菜单，开始建造
	int ifbuildsol(cocos2d::Vec2 pos,int country);//判断是否点中兵种菜单
	void updatemoney1(float dt);
	void updatemoney2(float dt);
	void updatemoney3(float dt);
	void updatemoney4(float dt);

	bool create_a_electric(int country, cocos2d::Point tile_position);
	bool create_a_mine(int country, cocos2d::Point tile_position);
	bool create_a_camp(int country, cocos2d::Point tile_position);
	bool create_a_factory(int country, cocos2d::Point tile_position);
	
	void destruct_last_solbuild(int country,char c_or_f);

	cocos2d::Vec2 startpos;//储存初始位置
	static std::vector<cocos2d::Vec2> vec_mappos;//储存地图位置

    CREATE_FUNC(HelloWorld);
};

#endif // !HELLOWORLDSCENE_H