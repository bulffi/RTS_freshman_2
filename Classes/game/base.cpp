#include"base.h"
#include"cocos2d.h"
#define XMAX 50
#define YMAX 50
#define X_SIZE 32
#define Y_SIZE 32
base* base::create(const char* filename)
{
	base* sprite = new base();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void base::set_data(int country)//设置参数
{
	my_country = country;
	health = 99;
	beginhealth = health;
}
void base::change_situation(int* situation, int condition)//改变situation
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			situation[(this->tile_position_point[0] + j) + XMAX*(this->tile_position_point[1] + i)] = condition;
}