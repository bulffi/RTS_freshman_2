#pragma once
#ifndef __go_H__
#define __go_H__

#include "cocos2d.h"

class ground :public cocos2d::Layer
{
	cocos2d::TMXTiledMap* tilemap;
	
public:
	cocos2d::Vec2 mospos;   //初始时刻地图坐标
	cocos2d::Vec2 mospos0;
	cocos2d::Vec2 mospos1;
	cocos2d::Vec2 mospos2;
	bool m_bdrag;
	static cocos2d::Scene* createscene();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);

	virtual void onEnter();
	/*virtual void onExit();

	virtual bool onMouseUp( cocos2d::Event* event);
	virtual void onMouseMove(cocos2d::Event* event);
	virtual void onMouseDown(cocos2d::Event* event);*/

	CREATE_FUNC(ground);
};

#endif // __go_H__
