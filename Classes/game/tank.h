#pragma once
#ifndef TANK_H
#define TANK_H
#include"soldier.h"
#include"SimpleAudioEngine.h"
class tank :public soldier
{
public:
	static tank* create(const char* filename);
	void set_data(int country);//ÉèÖÃ²ÎÊý
	virtual void play_attack_music(int alternative) override
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tank_emey.WAV", false, 1.0f, alternative);
	}
};



#endif // !1

