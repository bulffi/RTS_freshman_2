#pragma once
#ifndef __go_H__
#define __go_H__

#include "cocos2d.h"

class go :public cocos2d::Layer
{
	cocos2d::TMXTiledMap* tilemap;
public:
	static cocos2d::Scene* createscene();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);

	/*virtual void onEnter();
	virtual void onExit();*/

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	CREATE_FUNC(go);
};

#endif // __go_H__
