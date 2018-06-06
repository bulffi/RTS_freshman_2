#pragma once
#ifndef __setting_H__
#define __setting_H__

#include "cocos2d.h"

class setting :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createscene();
	virtual bool init();
	void MenusoundCallback(cocos2d::Ref* pSender);
	void menuokCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(setting);
};

#endif // __setting_H__
