#pragma once
#ifndef building_h
#define building_h

#include "cocos2d.h"

class building :public cocos2d::Sprite
{
public:
	static building* create(const char* filename);
	Node* wall;
};

#endif // !building_h
#pragma once
