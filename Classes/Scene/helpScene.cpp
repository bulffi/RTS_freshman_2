#include"cocos2d.h"
#include"helpscene.h"
Scene* helpScene::createScene()
{
	auto scene = Scene::create();
	auto layer = helpScene::create();
	scene->addChild(layer);
	return scene;
}
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool helpScene::init()
{
	
	if (!Layer::init())
	{
		return false;
	}
	where_am_i = UserDefault::getInstance()->getIntegerForKey("where_am_i", 0);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background_color = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background_color, -1);
	auto sprite_bg = Sprite::create("scene/helpScene.png");
	if (sprite_bg == nullptr)
	{
		problemLoading("'scene/helpScene.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		
		Size size = Director::getInstance()->getWinSize();
		auto Scale_x = size.width*0.8 / (sprite_bg->getContentSize().width);
		auto Scale_Y = size.height*0.3 / (sprite_bg->getContentSize().height);
		sprite_bg->setScale(Scale_x, Scale_Y);
		sprite_bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height*(1-0.35*0.5) + origin.y));
		// add the sprite as a child to this layer
		this->addChild(sprite_bg, 0);
	}
	auto labbel_back = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 50);
	auto meunitem_back = MenuItemLabel::create(labbel_back, CC_CALLBACK_1(helpScene::menuBackCallback, this));
	meunitem_back->setColor(Color3B::BLACK);
	if (where_am_i == 0)
	{
		auto next = creatNext();
		next->setColor(Color3B::BLACK);
		meunitem_back->setPosition(origin.x + visibleSize.width / 2, origin.y + 0.15*visibleSize.height);
		next->setPosition(meunitem_back->getPosition().x + 0.3*visibleSize.width, meunitem_back->getPosition().y);
		auto menu_bellow = Menu::create(meunitem_back, next, nullptr);
		menu_bellow->setPosition(0, 0);
		auto label_1 = Label::createWithTTF("No.1", "fonts/Marker Felt.ttf", 30);
		label_1->setColor(Color3B::BLACK);
		label_1->setPosition(meunitem_back->getPosition().x - 0.3*visibleSize.width, visibleSize.height*(1 - 0.4) + origin.y);
		addChild(label_1);
		addChild(menu_bellow);
	}
	else if (where_am_i == 1)
	{
		auto next = creatNext();
		auto privious = creatPrivious();
		privious->setColor(Color3B::BLACK);
		next->setColor(Color3B::BLACK);
		meunitem_back->setPosition(origin.x + visibleSize.width / 2, origin.y + 0.15*visibleSize.height);
		next->setPosition(meunitem_back->getPosition().x + 0.3*visibleSize.width, meunitem_back->getPosition().y);
		privious->setPosition(meunitem_back->getPosition().x - 0.3*visibleSize.width, meunitem_back->getPosition().y);
		auto menu_bellow = Menu::create(privious, meunitem_back, next, nullptr);
		auto label_2 = Label::createWithTTF("No.2", "fonts/Marker Felt.ttf", 30);
		label_2->setPosition(meunitem_back->getPosition().x - 0.3*visibleSize.width, visibleSize.height*(1 - 0.4) + origin.y);
		menu_bellow->setPosition(0, 0);
		label_2->setColor(Color3B::BLACK);
		addChild(label_2);
		addChild(menu_bellow);
	}
	else
	{
		auto privious = creatPrivious();
		privious->setColor(Color3B::BLACK);
		meunitem_back->setPosition(origin.x + visibleSize.width / 2, origin.y + 0.15*visibleSize.height);
		privious->setPosition(meunitem_back->getPosition().x - 0.3*visibleSize.width, meunitem_back->getPosition().y);
		auto menu_bellow = Menu::create(meunitem_back, privious, nullptr);
		menu_bellow->setPosition(0, 0);
		auto label_3 = Label::createWithTTF("No.3", "fonts/Marker Felt.ttf", 30);
		label_3->setPosition(meunitem_back->getPosition().x - 0.3*visibleSize.width, visibleSize.height*(1 - 0.4) + origin.y);
		label_3->setColor(Color3B::BLACK);
		addChild(label_3);
		addChild(menu_bellow);
	}

	auto NextEventListener = EventListenerKeyboard::create();
	NextEventListener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		where_am_i = UserDefault::getInstance()->getIntegerForKey("where_am_i", 0);
		
		if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			if (where_am_i < 2)
			{
				UserDefault::getInstance()->setIntegerForKey("where_am_i", ++where_am_i);
				Director::getInstance()->replaceScene(TransitionFade::create(1.0f, helpScene::createScene()));
			}
		}
	}; 
	auto PriviousEventListener = EventListenerKeyboard::create();
	PriviousEventListener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		where_am_i = UserDefault::getInstance()->getIntegerForKey("where_am_i", 0);
		if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			if (where_am_i >0 )
			{
				UserDefault::getInstance()->setIntegerForKey("where_am_i", --where_am_i);
				Director::getInstance()->replaceScene(TransitionFade::create(1.0f, helpScene::createScene()));
			}
		}
	};

	auto BackEventListener = EventListenerKeyboard::create();
	BackEventListener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		where_am_i = UserDefault::getInstance()->getIntegerForKey("where_am_i", 0);
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			UserDefault::getInstance()->setIntegerForKey("where_am_i", 0);
			Director::getInstance()->popScene();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(BackEventListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(PriviousEventListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(NextEventListener, this);


	return true;
}
MenuItem* helpScene::creatNext()
{
	auto labbel_next = Label::createWithTTF("Next", "fonts/Marker Felt.ttf", 50);
	auto meunitem_next = MenuItemLabel::create(labbel_next, CC_CALLBACK_1(helpScene::menuNextCallback, this));
	return meunitem_next;
}
MenuItem* helpScene::creatPrivious()
{
	auto labbel_Pri = Label::createWithTTF("Privious", "fonts/Marker Felt.ttf", 50);
	auto meunitem_Pri = MenuItemLabel::create(labbel_Pri, CC_CALLBACK_1(helpScene::menuPriviousCallback, this));
	return meunitem_Pri;
}

void helpScene::menuBackCallback(Ref *pSender)
{
	Director::getInstance()->popScene();
}
void helpScene::menuNextCallback(Ref *pSender)
{
	where_am_i = UserDefault::getInstance()->getIntegerForKey("where_am_i", 0);
	UserDefault::getInstance()->setIntegerForKey("where_am_i", ++where_am_i);
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, helpScene::createScene()));
}
void helpScene::menuPriviousCallback(Ref *pSender)
{
	where_am_i = UserDefault::getInstance()->getIntegerForKey("where_am_i", 0);
	UserDefault::getInstance()->setIntegerForKey("where_am_i", --where_am_i);
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, helpScene::createScene()));
}