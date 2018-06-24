#include "BaseLayer.h"
#include"cocos2d.h"

USING_NS_CC;
using namespace CocosDenshion;
Scene* BaseLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = BaseLayer::create();

	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


bool BaseLayer::init()
{
	
	if (!Layer::init())
	{
		return false;
	}
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vector<MenuItem *> items = {
		MenuItemLabel::create(
			Label::createWithTTF("Play", "fonts/Marker Felt.ttf", 70),
			CC_CALLBACK_1(BaseLayer::menuPlayCallback, this)),
		MenuItemLabel::create(
			Label::createWithTTF("Settings", "fonts/Marker Felt.ttf", 70),
			CC_CALLBACK_1(BaseLayer::menuSettingsCallback, this)),
		MenuItemLabel::create(
			Label::createWithTTF("Help", "fonts/Marker Felt.ttf",70),
			CC_CALLBACK_1(BaseLayer::menuHelpCallback, this)),
		MenuItemLabel::create(
			Label::createWithTTF("Exit", "fonts/Marker Felt.ttf",70),
			CC_CALLBACK_1(BaseLayer::menuExitCallback, this))
	};
	int starting_position = origin.y + (visibleSize.height)*0.618;//gloden devision
	for (ssize_t i = 0; i < items.size(); ++i)
	{

		auto item = dynamic_cast<MenuItemLabel *>(items.at(i));
		if (item == nullptr ||
			item->getContentSize().width <= 0 ||
			item->getContentSize().height <= 0)
		{
			problemLoading("'something wrong with your fonts'");
		}
		else
		{
			item->setColor(Color3B::BLACK);
			item->setPosition(item->getContentSize().width / 2 + 60, starting_position-(item->getContentSize().height+90*i));
		}
	}

	auto menu = Menu::createWithArray(items);
	menu->setPosition(0, 0);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
	// add "HelloWorld" splash screen"
	auto background_color = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background_color, -1);
	auto sprite_bg = Sprite::create("scene/Background.jpg");
	if (sprite_bg == nullptr)
	{
		problemLoading("'scene/HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite_bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		Size size = Director::getInstance()->getWinSize();
		auto Scale_x = size.width / (sprite_bg->getContentSize().width);
		auto Scale_Y = size.height / (sprite_bg->getContentSize().height);
		sprite_bg->setScale(Scale_x, Scale_Y);
		// add the sprite as a child to this layer
		this->addChild(sprite_bg, 0);
	}
	if (UserDefault::getInstance()->getBoolForKey("isBackgroundMusic", true))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/PaintItBlack.wav", true);
	}
	log("%d",SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying());//返回布尔型参数，是否在放着背景音乐 //返回布尔型参数，是否在放着背景音乐 )
	return true;
}

void BaseLayer::menuPlayCallback(Ref* pSender)
{
	auto login = loginScene::createScene();
	auto reScene = TransitionFade::create(1.0f, login);
	Director::getInstance()->pushScene(reScene);
}
void BaseLayer::menuHelpCallback(Ref* pSender)
{
	auto help = helpScene::createScene();
	auto reScene = TransitionFade::create(1.0f, help);
	Director::getInstance()->pushScene(reScene);
}
void BaseLayer::menuSettingsCallback(Ref* pSender)
{
	auto settings = settingsScene::creatscene();
	auto reScene = TransitionFade::create(1.0f, settings);
	Director::getInstance()->pushScene(reScene);
}

void BaseLayer::menuExitCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

void BaseLayer::onEnter()
{
	Layer::onEnter();
}
void BaseLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	
}
void BaseLayer::onExit()
{
	Layer::onExit();
}
void BaseLayer::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
}
void BaseLayer::cleanup()
{
	Layer::cleanup();
	SimpleAudioEngine::getInstance()->stopAllEffects();
}