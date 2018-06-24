#include"dog.h"
#include"cocos2d.h"
dog* dog::create(const char* filename)
{
	dog* sprite = new dog();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void dog::set_data(int country)
{
	this->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	my_country = country;
	health = 15;
	beginhealth = 15;
	attack_power = 2;
	move_speed = 8;
	attack_distance = 30;
	attack_speed = 1.5f;
	if (country == 1)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("dog1.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_dog1.png");
	}
	else if (country == 2)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("dog2.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_dog2.png");
	}
	else if (country == 3)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("dog3.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_dog3.png");
	}
	else if (country == 4)
	{
		texture_normal = cocos2d::Director::getInstance()->getTextureCache()->addImage("dog4.png");
		texture_chosed = cocos2d::Director::getInstance()->getTextureCache()->addImage("chosed_dog4.png");
	}
}