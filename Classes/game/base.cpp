#include"base.h"
#include"cocos2d.h"
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
void base::set_data(int country)//ÉèÖÃ²ÎÊı
{
	my_country = country;
	health = 100;
	beginhealth = health;
}