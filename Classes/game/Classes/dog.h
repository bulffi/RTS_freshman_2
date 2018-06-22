#pragma once
#ifndef DOG_H
#define DOG_H
#include"soldier.h"
class dog :public soldier
{
public:
	static dog* create(const char* filename);
	void set_data(int country);//ÉèÖÃ²ÎÊı
};



#endif // !1
