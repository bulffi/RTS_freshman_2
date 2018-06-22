#pragma once
#ifndef TANK_H
#define TANK_H
#include"soldier.h"
class tank :public soldier
{
public:
	static tank* create(const char* filename);
	void set_data(int country);//ÉèÖÃ²ÎÊı
};



#endif // !1

