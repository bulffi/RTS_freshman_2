#pragma once
#ifndef DOG_H
#define DOG_H
#include"soldier.h"
class dog :public soldier
{
public:
	static dog* create(const char* filename);
	void set_data(int country);//���ò���
};



#endif // !1
