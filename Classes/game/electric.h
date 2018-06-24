#pragma once
#ifndef ELECTRIC_H
#define ELECTRIC_H
#include"building.h"

class electric :public building
{
public:
	static electric* create(const char* filename);
	void set_data(int country, int x, int y);//设置参数
	void change_situation(std::vector<int>& situation, int condition);//改变situation
	bool is_covered_by_biulding_or_land(int x, int y, std::vector<int> situation);//放置建筑时调用，判断是否和建筑或者地形重叠，参数为瓦片坐标
	cocos2d::Rect building_rect_when_click(int x, int y);//点击鼠标放建筑时建筑的矩形，x,y是瓦片坐标
};


#endif // 
