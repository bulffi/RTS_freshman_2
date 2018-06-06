#include "go.h"

USING_NS_CC;

Scene* ground::createscene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ground::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void ground::menuCloseCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->popScene();
}

bool ground::init()
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
		CC_CALLBACK_1(ground::menuCloseCallback, this));

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
	mospos = tilemap->getPosition();
	/////////////
	//setTouchEnabled(true);
	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	return true;
}

void ground::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerMouse::create();

	mospos0 = tilemap->getPosition();

	listener->onMouseDown = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		m_bdrag = true;
		mospos1.set(e->getCursorX(), e->getCursorY());  //记录鼠标点击时的位置
	};
	listener->onMouseMove = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		/*if (m_bdrag == true)
		{
			
		}*/
	};

	listener->onMouseUp = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		mospos2.set(e->getCursorX(), e->getCursorY());  //记录鼠标抬起时位置
		mospos0 = mospos0 - mospos1 + mospos2;          //根据鼠标偏移量改变地图位置

		float mapWidth = tilemap->getMapSize().width*tilemap->getTileSize().width;
		log("%f", mapWidth);
		float mapHeight = tilemap->getMapSize().width*tilemap->getTileSize().height;
		log("%f", mapHeight);
		Vec2 winSize = CCDirector::sharedDirector()->getWinSize();//屏幕大小
		log("%f %f", winSize.x, winSize.y);
		cocos2d::Vec2 posmax = Vec2(winSize.x / 2, winSize.y / 2);
		cocos2d::Vec2 posmin = Vec2(-mapWidth + winSize.x / 2, -mapHeight + winSize.y / 2);

		//判断是否拖出地图边界
		if (mospos0.x < posmin.x)
			mospos0.x = posmin.x;
		if (mospos0.x > posmax.x)
			mospos0.x = posmax.x;
		if (mospos0.y < posmin.y)
			mospos0.y = posmin.y;
		if (mospos0.y > posmax.y)
			mospos0.y = posmax.y;

		tilemap->setPosition(mospos0);
		m_bdrag = false;
	};
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

/*bool ground::onMouseUp( cocos2d::Event* event)
{
	log("ontouchbegan");
	return true;
}

void ground::onMouseMove( cocos2d::Event* event)
{
	log("ontouchmoved");
}

void ground::onMouseDown( cocos2d::Event* event)
{
	this->setPosition(->getDelta());
}*/