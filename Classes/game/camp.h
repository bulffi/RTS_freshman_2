#pragma once
#ifndef CAMP_H
#define CAMP_H
#include"building.h"

class camp :public building
{
public:
	cocos2d::Texture2D* texture_normal;//未被选为主要建筑时的贴图
	cocos2d::Texture2D* texture_major;//被选为主要建筑时的贴图

	static camp* create(const char* filename);
	void set_data(int country,int x,int y);//设置参数
	void change_situation(int* situation,int condition);//改变situation
	bool is_covered_by_biulding_or_land(int x, int y, int* situation);//放置建筑时调用，判断是否和建筑或者地形重叠，参数为瓦片坐标
	cocos2d::Rect building_rect_when_click(int x, int y);//点击鼠标放建筑时建筑的矩形，x,y是瓦片坐标
};


#endif // CAMP_H
