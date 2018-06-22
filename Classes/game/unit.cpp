#include"unit.h"
#include "cocos2d.h"
unit* unit::create(const char* filename)
{
	unit* sprite = new unit();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}