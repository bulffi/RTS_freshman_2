#include"tank.h"
#include"cocos2d.h"
tank* tank::create(const char* filename)
{
	tank* sprite = new tank();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void tank::set_data(int country)
{
	this->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	my_country = country;
	health = 10;
	beginhealth = 10;
	attack_power = 1;
	move_speed = 10;
	attack_distance = 200;
	attack_speed = 1.0f;
	if (country == 1)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("tank1.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_tank1.png");
	}
	else if (country == 2)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("tank2.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_tank2.png");
	}
	else if (country == 3)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("tank3.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_tank3.png");
	}
	else if (country == 4)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("tank4.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_tank4.png");
	}
}