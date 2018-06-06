#include "go.h"

USING_NS_CC;

Scene* go::createscene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = go::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void go::menuCloseCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->popScene();
}

bool go::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//////////////////
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(go::menuCloseCallback, this));

		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	
		// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	////////////
	tilemap = TMXTiledMap::create("map/first.tmx");
	addChild(tilemap, 0,100);
	/////////////
	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	return true;
}

/*void go::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(go::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(go::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(go::onTouchEnded, this);

}*/

bool go::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	log("ontouchbegan");
	return true;
}

void go::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	log("ontouchmoved");
}

void go::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	this->setPosition(touch->getDelta());
}