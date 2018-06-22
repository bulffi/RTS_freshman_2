#include"mine.h"
#include"cocos2d.h"
#define XMAX 50
#define YMAX 50
#define X_SIZE 32
#define Y_SIZE 32
mine* mine::create(const char* filename)
{
	mine* sprite = new mine();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void mine::set_data(int country, int x, int y)//���ò���
{
	this->tile_position_point[0] = x;
	this->tile_position_point[1] = y;
	my_country = country;
	health = 20;
	beginhealth = health;
	this->setAnchorPoint(cocos2d::Vec2(0.5, 0.75));
}
void mine::change_situation(int* situation, int condition)
{
	for (int i = 0; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			situation[(this->tile_position_point[0] + j) + XMAX*(this->tile_position_point[1] + i)] = condition;
}
bool mine::is_covered_by_biulding_or_land(int x, int y, int* situation)
{
	if (situation[x - 1 + XMAX*y] == 1 && situation[x - 1 + XMAX*(y + 1)] == 1 && situation[x + XMAX*y] == 1 && situation[x + 1 + XMAX*y] == 1 && situation[x + XMAX*(y + 1)] == 1 && situation[x + 1 + XMAX*(y + 1)] == 1)
		return false;
	else
		return true;
}
cocos2d::Rect mine::building_rect_when_click(int x, int y)
{
	cocos2d::Rect rect(32 * (x - 1), 32 * (YMAX - y - 2), 96, 64);
	return rect;
}