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
	void set_data(int country, int x, int y);//���ò���
	void change_situation(std::vector<int>& situation, int condition);//�ı�situation
	bool is_covered_by_biulding_or_land(int x, int y, std::vector<int> situation);//���ý���ʱ���ã��ж��Ƿ�ͽ������ߵ����ص�������Ϊ��Ƭ����
	cocos2d::Rect building_rect_when_click(int x, int y);//������Ž���ʱ�����ľ��Σ�x,y����Ƭ����
};

class mine_car :public soldier
{
public:
	unit* my_mine;
	unit* my_mines;
	int if_add = 0;//0����δ������1������Լ�Ǯ
	virtual void play_attack_music(int alternative) override
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tank_emey.WAV", false, 1.0f, alternative);
	}
	static mine_car* create(const char* filename);
	void set_data(int country, unit* mine, unit* mines);//���ò���
	void movetomine(float dt);//ȥ��
	void movetomines(float dt);//ȥ��
	void updatecar1(float dt);//ˢ�¿󳵵�״̬

	//void updatecar2(float dt);
	//void updatecar3(float dt);
	//void updatecar4(float dt);
};


#endif // CAMP_H

