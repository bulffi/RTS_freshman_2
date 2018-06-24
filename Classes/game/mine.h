#pragma once
#ifndef MINE_H
#define MINE_H
#include"building.h"
#include"soldier.h"
#include"SimpleAudioEngine.h"
class mine :public building
{
public:
	static mine* create(const char* filename);
	void set_data(int country, int x, int y);//设置参数
	void change_situation(std::vector<int>& situation, int condition);//改变situation
	bool is_covered_by_biulding_or_land(int x, int y, std::vector<int> situation);//放置建筑时调用，判断是否和建筑或者地形重叠，参数为瓦片坐标
	cocos2d::Rect building_rect_when_click(int x, int y);//点击鼠标放建筑时建筑的矩形，x,y是瓦片坐标
};

class mine_car :public soldier
{
public:
	unit* my_mine;
	unit* my_mines;
	int if_add = 0;//0代表未就绪，1代表可以加钱
	virtual void play_attack_music(int alternative) override
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tank_emey.WAV", false, 1.0f, alternative);
	}
	static mine_car* create(const char* filename);
	void set_data(int country, unit* mine, unit* mines);//设置参数
	void movetomine(float dt);//去矿场
	void movetomines(float dt);//去矿
	void updatecar1(float dt);//刷新矿车的状态

	//void updatecar2(float dt);
	//void updatecar3(float dt);
	//void updatecar4(float dt);
};


#endif // CAMP_H

