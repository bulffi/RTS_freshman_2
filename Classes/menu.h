//#pragma once
#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"

class menu :public cocos2d::Sprite
{
public:
	int lable;//序号
	int money = 100;//耗费金币
	int elec = 100;//耗费电力
	cocos2d::Vec2 position;//position
};

class build_menu :public menu
{
public:
	int ifready = -1;//是否准备完毕,-1代表未建造，0表示正在建造，1代表已就绪
	//cocos2d::Sprite* menu_ready;//就绪图片

	static build_menu* create(int num);//构造函数
	//int iftobuild(cocos2d::Vec2 pos);//抬起落下同一点，判断是否点中建筑菜单，准备建造
	//int ifbuild(cocos2d::Vec2 pos);//抬起落下不同点，判断是否点中建筑菜单，开始建造
	void build(cocos2d::Vec2 pos);//执行建筑操作
	void cretimer();//创建菜单CD条
	void updateload(float dt);//更新CD条
};

class sol_menu :public menu
{
public:
	int num_tobuild = 0;//等待建造的数目

	static sol_menu* create(int num);//构造函数
	//int ifbuildsol(cocos2d::Vec2 pos);//判断是否点中兵种菜单
	void buildsol();//执行建造兵种操作
	void cretimer();//创建菜单CD条
	void updateload(float dt);//更新CD条
};

#endif // __MENU_H__