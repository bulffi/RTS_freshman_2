#pragma once
#ifndef PEOPLE_H
#define PEOPLE_H
#include"soldier.h"
class people :public soldier
{
public:
	static people* create(const char* filename);
	void set_data(int country);//ÉèÖÃ²ÎÊı
};



#endif // !1
