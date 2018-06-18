#include"lose.h"
#include"cocos2d.h"
#include"SimpleAudioEngine.h"
using namespace CocosDenshion;
Scene* loseScene::createScene()
{
	auto scene = Scene::create();
	auto layer = loseScene::create();
	scene->addChild(layer);
	return scene;
}
bool loseScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background_color = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background_color, -1);
	auto qury = Label::create("There is no winner in war", "fonts/Marker Felt.ttf", 80);
	qury->setColor(Color3B(0, 0, 0));
	qury->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.85);
	this->addChild(qury);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/David Bowie - Heroes.wav");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/David Bowie - Heroes.wav");



	return true;
}