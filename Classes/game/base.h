#pragma once
#ifndef BASE_H
#define BASE_H
#include"building.h"

class base :public building
{
public:
	static base* create(const char* filename);
	void set_data(int country);//ÉèÖÃ²ÎÊı
};


#endif // 
