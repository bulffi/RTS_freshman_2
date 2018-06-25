#include"people.h"
#include"cocos2d.h"
people* people::create(const char* filename)
{
	people* sprite = new people();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void people::set_data(int country)
{
	this->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	my_country = country;
	health = 7;
	beginhealth = 7;
	attack_power = 2;
	move_speed = 3;
	attack_distance = 120;
	attack_speed = 2.0f;
	if (country == 1)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("people1.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_people1.png");
	}
	else if (country == 2)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("people2.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_people2.png");
	}
	else if (country == 3)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("people3.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_people3.png");
	}
	else if (country == 4)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("people4.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_people4.png");
	}
}