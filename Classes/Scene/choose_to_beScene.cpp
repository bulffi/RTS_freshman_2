#include"cocos2d.h"
#include"choose_to_beScene.h"
#include"loginScnen.h"
#include"roomScene.h"
#include"searchScene.h"
#include"my_Network\client.h"
#include"my_Network\server.h"
USING_NS_CC;
Scene* choose_to_beScene::createScene()
{
	auto scene = Scene::create();
	auto layer = choose_to_beScene::create();

	scene->addChild(layer);
	return scene;
}
void to_check();
bool choose_to_beScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background_color = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background_color, -1);
	auto qury = Label::create((UserDefault::getInstance()->getStringForKey("UserName","Rolling Stone") + " ! you wanna be a"), "fonts/Marker Felt.ttf", 80);
	qury->setColor(Color3B(0, 0, 0));
	qury->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.7);
	this->addChild(qury);
	auto num_1 = Sprite::create("scene/number-1-red-hi.png");
	num_1->setScale(0.12, 0.12);
	auto num_2 = Sprite::create("scene/red-number-2-hi.png");
	num_2->setScale(0.12,0.12);
	num_1->setPosition(origin.x + visibleSize.width / 3, origin.y + visibleSize.height*0.5);
	num_2->setPosition(origin.x + visibleSize.width / 3, origin.y + visibleSize.height*0.3);
	this->addChild(num_1);
	this->addChild(num_2);
	auto be_a_guest = MenuItemLabel::create(Label::createWithTTF("guest", "fonts/Marker Felt.ttf", 70),
		CC_CALLBACK_1(choose_to_beScene::gustCallback, this));
	be_a_guest->setScale(1.0);
	be_a_guest->setPosition(Vec2(origin.x + visibleSize.width / 2+50, origin.y + visibleSize.height*0.3));
	be_a_guest->setColor(Color3B(0, 0, 0));
	//this->addChild(be_a_guest);
	auto be_a_host = MenuItemLabel::create(Label::createWithTTF("host", "fonts/Marker Felt.ttf", 70),
		CC_CALLBACK_1(choose_to_beScene::hostCallback, this));
	be_a_host->setScale(1.0);
	be_a_host->setPosition(Vec2(origin.x + visibleSize.width / 2+50, origin.y + visibleSize.height*0.5));
	be_a_host->setColor(Color3B(0, 0, 0));
	//this->addChild(be_a_host);
	auto BackEventListener = EventListenerKeyboard::create();
	BackEventListener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		if (code == EventKeyboard::KeyCode::KEY_SPACE)
		{
			to_check();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(BackEventListener, this);
	
	Menu* mn = Menu::create(be_a_guest,be_a_host,nullptr);
	mn->setPosition(0.0f, 0.0f);
	this->addChild(mn);
	return 1;

}
io_service service;
void run()
{
	service.run();
}
void run_client()
{
	std::thread  t(run);
	t.detach();
}

void choose_to_beScene::gustCallback(Ref* pSender)
{
	
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
	auto my_client = talk_to_svr::start(ep, UserDefault::getInstance()->getStringForKey("UserName"),service);
	run_client();
	auto help=searchScene::createScene(my_client);
	auto reScene = TransitionFade::create(1.0f, help);
	Director::getInstance()->pushScene(reScene);
	//enter search scene.
}
talk_to_svr::ptr my_client;
void choose_to_beScene::hostCallback(Ref* pSender)
{
	//io_service service;
	talk_to_client::ptr client = talk_to_client::new_();
	acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
	run_server();


	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);

	
	my_client = talk_to_svr::start(ep, UserDefault::getInstance()->getStringForKey("UserName"),service);
	//auto my_client_2 = talk_to_svr::start(ep, "zzj");
	run_client();
	//boost::thread t(boost::bind(&boost::asio::io_service::run, &service));
	//t.detach();

	log("something happend");
	auto help = roomScene::createScene(my_client,1,UserDefault::getInstance()->getStringForKey("UserName"),
		UserDefault::getInstance()->getStringForKey("UserName"));
	auto reScene = TransitionFade::create(1.0f, help);
	Director::getInstance()->pushScene(reScene);
	

}
void to_check()
{
	std::string tempt = my_client->read();
	log(tempt.c_str());
}