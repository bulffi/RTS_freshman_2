#include "setting.h"

USING_NS_CC;

Scene* setting::createscene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = setting::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void setting::MenusoundCallback(cocos2d::Ref* pSender)
{
	
}

void setting::menuokCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->popScene();
}
// on "init" you need to initialize your instance
bool setting::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	////////////
	Sprite *bg = Sprite::create("scene2.jpg");

	// position the label on the center of the screen
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);

	/////////////
	auto soundonmenuitem = MenuItemImage::create(
		"on.jpg",
		"on.jpg");
	auto soundoffmenuitem = MenuItemImage::create(
		"off.jpg",
		"off.jpg");
	auto soundtoggle = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(setting::MenusoundCallback, this),
		soundonmenuitem,
		soundoffmenuitem,
		NULL);
	soundtoggle->setAnchorPoint(Vec2(0, 0));
	soundtoggle->setPosition(Director::getInstance()->convertToGL(Vec2(627, 252)));

	auto okMenuItem = MenuItemImage::create(
		"ok-down.png",
		"ok-up.png",
		CC_CALLBACK_1(setting::menuokCallback, this));
	okMenuItem->setAnchorPoint(Vec2(0, 0));
	okMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(376, 513)));


	Menu* mn = Menu::create(soundtoggle,okMenuItem, NULL);
	mn->setPosition(Vec2(Vec2::ZERO));
	this->addChild(mn);

	return true;
}
