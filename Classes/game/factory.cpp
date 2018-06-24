#include"factory.h"
#include"cocos2d.h"
#include"utility\information.h"


factory* factory::create(const char* filename)
{
	factory* sprite = new factory();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void factory::set_data(int country, int x, int y)//ÉèÖÃ²ÎÊý
{
	if (country == 1)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("factory1.png");
		texture_major = cocos2d::Director::getInstance()->getTextureCache()->addImage("major_factory1.png");
	}
	else if (country == 2)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("factory2.png");
		texture_major = cocos2d::Director::getInstance()->getTextureCache()->addImage("major_factory2.png");
	}
	else if (country == 3)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("factory3.png");
		texture_major = cocos2d::Director::getInstance()->getTextureCache()->addImage("major_factory3.png");
	}
	else if (country == 4)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("factory4.png");
		texture_major = cocos2d::Director::getInstance()->getTextureCache()->addImage("major_factory4.png");
	}
	this->tile_position_point[0] = x;
	this->tile_position_point[1] = y;
	my_country = country;
	health = 20;
	beginhealth = health;
	this->setAnchorPoint(cocos2d::Vec2(0.5, 0.75));
}
void factory::change_situation(std::vector<int>& situation, int condition)
{
	for (int i = 0; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			situation[(this->tile_position_point[0] + j) + imformation::XMAX*(this->tile_position_point[1] + i)] = condition;
}
bool factory::is_covered_by_biulding_or_land(int x, int y, std::vector<int> situation)
{
	if (situation[x - 1 + imformation::XMAX*y] == 1 && situation[x - 1 + imformation::XMAX*(y + 1)] == 1 && situation[x + imformation::XMAX*y] == 1 && situation[x + 1 + imformation::XMAX*y] == 1 && situation[x + imformation::XMAX*(y + 1)] == 1 && situation[x + 1 + imformation::XMAX*(y + 1)] == 1)
		return false;
	else
		return true;
}
cocos2d::Rect factory::building_rect_when_click(int x, int y)
{
	cocos2d::Rect rect(32 * (x-1), 32 * (imformation::YMAX - y - 2), 96, 64);
	return rect;
}