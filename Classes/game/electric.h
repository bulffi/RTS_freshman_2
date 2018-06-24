#pragma once
#ifndef ELECTRIC_H
#define ELECTRIC_H
#include"building.h"

class electric :public building
{
public:
	static electric* create(const char* filename);
	void set_data(int country, int x, int y);//���ò���
	void change_situation(std::vector<int>& situation, int condition);//�ı�situation
	bool is_covered_by_biulding_or_land(int x, int y, std::vector<int> situation);//���ý���ʱ���ã��ж��Ƿ�ͽ������ߵ����ص�������Ϊ��Ƭ����
	cocos2d::Rect building_rect_when_click(int x, int y);//������Ž���ʱ�����ľ��Σ�x,y����Ƭ����
};


#endif // 
