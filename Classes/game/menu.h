#pragma once
#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"
//#include"HelloWorldScene.h"
class menu :public cocos2d::Sprite
{
public:
	int lable;//序号
	int money = 100;//耗费金币
	int elec = 100;//耗费电力
	int country_did;//做出操作的国家
	int mycountry;//正在操作的国家
	cocos2d::Vec2 position;//position
};

class build_menu :public menu
{
public:
	int ifready[5] = { -2,-2,-2,-2, -2 };//是否准备完毕,-2代表未出现，-1代表未建造，0表示正在建造，1代表已就绪
	//cocos2d::Sprite* menu_ready;//就绪图片

	static build_menu* create(int num);//构造函数
	//int iftobuild(cocos2d::Vec2 pos);//抬起落下同一点，判断是否点中建筑菜单，准备建造
	//int ifbuild(cocos2d::Vec2 pos);//抬起落下不同点，判断是否点中建筑菜单，开始建造
	void recreate(int my_country,int country);
	void build(cocos2d::Vec2 pos,int country);//执行建筑操作
	void cretimer(int my_country,int country);//创建菜单CD条
	void updateload1(float dt);//更新CD条
	void updateload2(float dt);//更新CD条
	void updateload3(float dt);//更新CD条
	void updateload4(float dt);//更新CD条
};

class sol_menu :public menu
{
public:
	int num_tobuild[5] = { 0 };//等待建造的数目
	int ifready[5] = { 0 };//0代表未出现或者被摧毁,1代表已出现
	static sol_menu* create(int num);//构造函数
	void recreate(int my_country, int country);
	//int ifbuildsol(cocos2d::Vec2 pos);//判断是否点中兵种菜单
	void buildsol(int country);//执行建造兵种操作
	void buildsol1();//执行建造兵种操作
	void buildsol2();//执行建造兵种操作
	void buildsol3();//执行建造兵种操作
	void buildsol4();//执行建造兵种操作
	void cretimer(int my_country,int country);//创建菜单CD条
	void updateload1(float dt);//更新CD条
	void updateload2(float dt);
	void updateload3(float dt);//更新CD条
	void updateload4(float dt);//更新CD条
};

#endif // __MENU_H__