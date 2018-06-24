#include"settingsScene.h"
#include"SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
Scene* settingsScene::creatscene()
{
	auto scene = Scene::create();
	auto layer = settingsScene::create();
	scene->addChild(layer);

	return scene;
}
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
bool settingsScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	isEffect = UserDefault::getInstance()->getBoolForKey("isEffect", 1);
	isBackgroundMusic = UserDefault::getInstance()->getBoolForKey("isBackgroundMusic", 1);
	Size size = Director::getInstance()->getWinSize();
	Size visibleSize = Director::getInstance()->getVisibleSize();
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

		
		auto Scale_x = size.width*0.8 / (sprite_bg->getContentSize().width);
		auto Scale_Y = size.height*0.3 / (sprite_bg->getContentSize().height);
		sprite_bg->setScale(Scale_x, Scale_Y);
		sprite_bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height*(1 - 0.35*0.5) + origin.y));
		// add the sprite as a child to this layer
		this->addChild(sprite_bg, 0);
	}
	auto labbel_back = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 60);
	auto meunitem_back = MenuItemLabel::create(labbel_back, CC_CALLBACK_1(settingsScene::menuBackCallback, this));
	meunitem_back->setColor(Color3B::BLACK);
	meunitem_back->setPosition(origin.x + visibleSize.width / 2, origin.y + 0.15*visibleSize.height);
	auto labbel_backgroundMusic = Label::createWithTTF("Background Music", "fonts/Marker Felt.ttf", 50);
	auto labbel_effectMusic = Label::createWithTTF("Effect Music", "fonts/Marker Felt.ttf", 50);
	auto labbel_connection = Label::createWithTTF("Connection", "fonts/Marker Felt.ttf", 50);
	labbel_backgroundMusic->setPosition(Vec2(visibleSize.width*0.25 + origin.x, visibleSize.height*(1 - 0.4) + origin.y));
	labbel_effectMusic->setPosition(Vec2(labbel_backgroundMusic->getPosition().x, labbel_backgroundMusic->getPosition().y - 0.15*visibleSize.height));
	labbel_connection->setPosition(Vec2(labbel_effectMusic->getPosition().x, labbel_effectMusic->getPosition().y - 0.15*visibleSize.height));
	labbel_backgroundMusic->setColor(Color3B::BLACK);
	labbel_connection->setColor(Color3B::BLACK);
	labbel_effectMusic->setColor(Color3B::BLACK);
	addChild(labbel_backgroundMusic);
	addChild(labbel_connection);
	addChild(labbel_effectMusic);
	auto BackEventListener = EventListenerKeyboard::create();
	BackEventListener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			Director::getInstance()->popScene();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(BackEventListener, this);
	auto OnMenuItem = MenuItemImage::create("scene/switch_on-256.png",
		"scene/switch_on-256.png");
	auto OffMenuItem=MenuItemImage::create("scene/switch_off-256.png",
		"scene/switch_off-256.png");
	auto OnMenuItem_2 = MenuItemImage::create("scene/switch_on-256.png",
		"scene/switch_on-256.png");
	auto OffMenuItem_2 = MenuItemImage::create("scene/switch_off-256.png",
		"scene/switch_off-256.png"); 
	auto OnMenuItem_3 = MenuItemImage::create("scene/switch_on-256.png",
			"scene/switch_on-256.png");
	auto OffMenuItem_3 = MenuItemImage::create("scene/switch_off-256.png",
		"scene/switch_off-256.png");
	auto Scale_x_of_menu = size.width*0.2 / (OnMenuItem->getContentSize().width);
	auto Scale_Y_of_menu = size.height*0.1 / (OnMenuItem->getContentSize().height);
	OnMenuItem->setScale(Scale_x_of_menu, Scale_Y_of_menu);
	OnMenuItem_2->setScale(Scale_x_of_menu, Scale_Y_of_menu);
	OnMenuItem_3->setScale(Scale_x_of_menu, Scale_Y_of_menu);
	OffMenuItem->setScale(Scale_x_of_menu, Scale_Y_of_menu);
	OffMenuItem_2->setScale(Scale_x_of_menu, Scale_Y_of_menu);
	OffMenuItem_3->setScale(Scale_x_of_menu, Scale_Y_of_menu);
	auto backmusicToggleMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(settingsScene::menuBackmusicToggleCallback, this), 
		OffMenuItem, 
		OnMenuItem,
		nullptr);
	backmusicToggleMenuItem->setPosition(Vec2(labbel_backgroundMusic->getPosition().x + visibleSize.width*0.6, labbel_backgroundMusic->getPosition().y));
	auto effectMusicToggleMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(settingsScene::menuEffectToggleCallback, this), 
		OffMenuItem_2, 
		OnMenuItem_2,
		nullptr);
	effectMusicToggleMenuItem->setPosition(Vec2(backmusicToggleMenuItem->getPosition().x, labbel_effectMusic->getPosition().y));
	auto connectionToggleMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(settingsScene::menuEffectToggleCallback, this),
		OffMenuItem_3, 
		OnMenuItem_3, 
		nullptr);
	connectionToggleMenuItem->setPosition(Vec2(backmusicToggleMenuItem->getPosition().x, labbel_connection->getPosition().y));

	Menu* mn = Menu::create(meunitem_back,backmusicToggleMenuItem, effectMusicToggleMenuItem, connectionToggleMenuItem,nullptr);
	mn->setPosition(0.0f, 0.0f);
	this->addChild(mn);
	return true;
}
void settingsScene::menuBackCallback(Ref *pSender)
{
	Director::getInstance()->popScene();
}
void settingsScene::menuEffectToggleCallback(Ref *pSender)
{
	auto effect = static_cast<MenuItemToggle*> (pSender);
	if (isEffect)
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
		isEffect = false;
		UserDefault::getInstance()->setBoolForKey("isEffect", isEffect);
	}
	else
	{
		isEffect = true;
		UserDefault::getInstance()->setBoolForKey("isEffect", isEffect);
	}


}
void settingsScene::menuBackmusicToggleCallback(Ref* pSender)
{
	auto music = static_cast<MenuItemToggle*> (pSender);
	if (isBackgroundMusic)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		isBackgroundMusic = false;
		UserDefault::getInstance()->setBoolForKey("isBackgroundMusic", isBackgroundMusic);
	}
	else
	{
		isBackgroundMusic = true;
		UserDefault::getInstance()->setBoolForKey("isBackgroundMusic", isBackgroundMusic);
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/PaintItBlack.wma", true);
	}
}