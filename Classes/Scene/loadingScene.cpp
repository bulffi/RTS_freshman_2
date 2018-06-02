#include"cocos2d.h"
#include"loadingScene.h"
#include"BaseLayer.h"
USING_NS_CC;

Scene* loadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = loadingScene::create();

	scene->addChild(layer);
	return scene;
}
bool loadingScene::init()
{

	if (!Layer::init())
	{
		return 0;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto circle = Sprite::create("scene/loading.png");
	circle->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2-100));
	circle->setScale(0.6, 0.6);
	this->addChild(circle, 1);
	auto background_color = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background_color, -1);
	circle->runAction(RepeatForever::create(RotateBy::create(1, 360)));
	auto are_you_ready = Label::create("Ready to roll? click : wait", "fonts/Marker Felt.ttf", 60);
	are_you_ready->setColor(Color3B(0, 0, 0));
	are_you_ready->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 150));
	this->addChild(are_you_ready, 1);
	//Sleep(3000);
	auto listener = EventListenerMouse::create();
	listener->onMouseDown=
		[](Ref* pSender)
	{
		auto login = BaseLayer::createScene();
		auto reScene = TransitionFade::create(1.0f, login);
		Director::getInstance()->replaceScene(reScene);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void loadingScene::onClickCallBack(Ref* pSender)
{

}
