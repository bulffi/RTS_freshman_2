#pragma once
#ifndef building_h
#define building_h
#include"unit.h"
#include "cocos2d.h"

class building :public unit
{
public:
	static building* create(const char* filename);
	cocos2d::PhysicsBody* wall;
	int tile_position_point[2];//锚点所在的瓦片的瓦片坐标
	std::string report_my_position()
	{
		std::string my_position;
		int blod = health;
		for (int m = 10; m >= 1; m /= 10)
		{
			int n = blod / m;
			my_position += std::to_string(n);
			blod -= n*m;
		}
		return my_position;
	}
};

#endif // !building_h
#pragma once
