#include"camp.h"
#include"cocos2d.h"
#define XMAX 50
#define YMAX 50
#define X_SIZE 32
#define Y_SIZE 32
camp* camp::create(const char* filename)
{
	camp* sprite = new camp();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void camp::set_data(int country,int x,int y)//ÉèÖÃ²ÎÊý
{
	if (country == 1)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("camp1.png");
		texture_major = cocos2d::Director::getInstance()->getTextureCache()->addImage("major_camp1.png");
	}
	else if (country == 2)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("camp2.png");
		texture_major = cocos2d::Director::getInstance()->getTextureCache()->addImage("major_camp2.png");
	}
	else if (country == 3)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("camp3.png");
		texture_major = cocos2d::Director::getInstance()->getTextureCache()->addImage("major_camp3.png");
	}
	else if (country == 4)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("camp4.png");
		texture_major = cocos2d::Director::getInstance()->getTextureCache()->addImage("major_camp4.png");
	}
	this->tile_position_point[0] = x;
	this->tile_position_point[1] = y;
	my_country = country;
	health = 20;
	beginhealth = health;
	this->setAnchorPoint(cocos2d::Vec2(0.25,0.75));
}
void camp::change_situation(int* situation, int condition)
{
	for (int i = 0; i <= 1; i++)
		for (int j = 0; j <= 1; j++)
			situation[(this->tile_position_point[0] + j) + XMAX*(this->tile_position_point[1] + i)] = condition;
}
bool camp::is_covered_by_biulding_or_land(int x, int y, int* situation)
{
	if (situation[x + XMAX*y] == 1 && situation[x + 1 + XMAX*y] == 1 && situation[x + XMAX*(y + 1)] == 1 && situation[x + 1 + XMAX*(y + 1)] == 1)
		return false;
	else
		return true;
}
cocos2d::Rect camp::building_rect_when_click(int x, int y)
{
	cocos2d::Rect rect(32*x,32*(YMAX-y-2),64,64);
	return rect;
}