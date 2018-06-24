#include"electric.h"
#include"cocos2d.h"
#include"utility\information.h"

electric* electric::create(const char* filename)
{
	electric* sprite = new electric();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void electric::set_data(int country, int x, int y)//ÉèÖÃ²ÎÊý
{
	this->tile_position_point[0] = x;
	this->tile_position_point[1] = y;
	my_country = country;
	health = 20;
	beginhealth = health;
	this->setAnchorPoint(cocos2d::Vec2(0.25, 0.75));
}
void electric::change_situation(std::vector<int>& situation, int condition)
{
	for (int i = 0; i <= 1; i++)
		for (int j = 0; j <= 1; j++)
			situation[(this->tile_position_point[0] + j) + imformation::XMAX*(this->tile_position_point[1] + i)] = condition;
}
bool electric::is_covered_by_biulding_or_land(int x, int y, std::vector<int> situation)
{
	if (situation[x + imformation::XMAX*y] == 1 && situation[x + 1 + imformation::XMAX*y] == 1 && situation[x + imformation::XMAX*(y + 1)] == 1 && situation[x + 1 + imformation::XMAX*(y + 1)] == 1)
		return false;
	else
		return true;
}
cocos2d::Rect electric::building_rect_when_click(int x, int y)
{
	cocos2d::Rect rect(32 * x, 32 * (imformation::YMAX - y - 2), 64, 64);
	return rect;
}