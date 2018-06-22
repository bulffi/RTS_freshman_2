#pragma once
#ifndef BASE_H
#define BASE_H
#include"building.h"

class base :public building
{
public:
	static base* create(const char* filename);
	void set_data(int country);//设置参数
	void change_situation(int* situation, int condition);//改变situation
};


#endif // 
