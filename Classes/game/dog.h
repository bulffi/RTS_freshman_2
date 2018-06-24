#pragma once
#ifndef DOG_H
#define DOG_H
#include"soldier.h"
#include"SimpleAudioEngine.h"
class dog :public soldier
{
public:
	static dog* create(const char* filename);
	void set_data(int country);//ÉèÖÃ²ÎÊý
	virtual void play_attack_music(int alternative) override
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/dog_emeey.WAV", false, 1.0f, alternative);
	}

};



#endif // !1
