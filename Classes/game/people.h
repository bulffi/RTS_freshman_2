#pragma once
#ifndef PEOPLE_H
#define PEOPLE_H
#include"soldier.h"
#include"SimpleAudioEngine.h"
class people :public soldier
{
public:
	static people* create(const char* filename);
	void set_data(int country);//ÉèÖÃ²ÎÊý
	virtual void play_attack_music(int alternative) override
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fire_enemy.WAV", false, 1.0f, alternative);
	}
};



#endif // !1
