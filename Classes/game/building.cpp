#include"building.h"
#include "cocos2d.h"
building* building::create(const char* filename)
{
	building* sprite = new building();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}