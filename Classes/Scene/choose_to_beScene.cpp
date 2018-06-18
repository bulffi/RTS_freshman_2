#include"cocos2d.h"
#include"choose_to_beScene.h"
#include"loginScnen.h"
#include"roomScene.h"
#include"searchScene.h"
#include<fstream>
#include<string>
#include<vector>
#include"my_Network\client.h"
#include"utility\information.h"
#include"my_Network\server.h"
USING_NS_CC;
std::vector<std::string> ip_chosen;
std::vector<cocos2d::Node*> things_to_clear;
Scene* choose_to_beScene::createScene()
{
	auto scene = Scene::create();
	auto layer = choose_to_beScene::create();

	scene->addChild(layer);
	return scene;
}
void to_check();
template <typename T>
void my_unique(T &vt)
{
	sort(vt.begin(), vt.end());
	vt.erase(unique(vt.begin(), vt.end()), vt.end());
}
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
	things_to_clear.push_back(qury);
	auto num_1 = Sprite::create("scene/number-1-red-hi.png");
	num_1->setScale(0.12, 0.12);
	auto num_2 = Sprite::create("scene/red-number-2-hi.png");
	num_2->setScale(0.12,0.12);
	num_1->setPosition(origin.x + visibleSize.width / 3, origin.y + visibleSize.height*0.5);
	num_2->setPosition(origin.x + visibleSize.width / 3, origin.y + visibleSize.height*0.3);
	this->addChild(num_1);
	things_to_clear.push_back(num_1);
	this->addChild(num_2);
	things_to_clear.push_back(num_2);
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

	system("arp -a >> ip.txt");
	std::ifstream ip_list("ip.txt");
	if (ip_list.is_open())
	{
		while (!ip_list.eof())
		{
			std::string tempt;
			ip_list >> tempt;
			if (tempt.find("192.168.1.") == 0 && tempt != "192.168.1.255")
			{
				ip_chosen.push_back(tempt);
			}
		}
	}
	else
	{
		cocos2d::log("ip>txt is not opened!");
	}
	std::string my_ip;
	if (!ip_chosen.empty())
	{
		//ip_chosen.erase(ip_chosen.begin());
		my_ip = ip_chosen.front();
	}
	my_unique(ip_chosen);
	for (auto it = ip_chosen.begin(); it != ip_chosen.end(); )
	{
		if (*it == my_ip)
			it = ip_chosen.erase(it);
		else
			++it;
	}
	Menu* mn = Menu::create(be_a_guest,be_a_host,nullptr);
	mn->setPosition(0.0f, 0.0f);
	this->addChild(mn);
	things_to_clear.push_back(mn);
	return 1;

}
void choose_to_beScene::gustCallback(Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	imformation::am_i_host = false;
	imformation::am_i_in_room = false;
	for (auto &node : things_to_clear)
	{
		this->removeChild(node);
	}
	auto qury = Label::create("Choose your host's ip.", "fonts/Marker Felt.ttf", 80);
	qury->setColor(Color3B(0, 0, 0));
	qury->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.85);
	this->addChild(qury);
	auto circle = Sprite::create("scene/loading.png");
	circle->setPosition(Point(visibleSize.width / 4-100, visibleSize.height / 2 + 100));
	circle->setScale(0.4, 0.4);
	this->addChild(circle, 1);
	circle->runAction(RepeatForever::create(RotateBy::create(1, 360)));
	for (int i = 0; i < ip_chosen.size(); i++)
	{
		auto be_a_guest = MenuItemLabel::create(Label::createWithTTF(ip_chosen[i], "fonts/Marker Felt.ttf", 60),
			CC_CALLBACK_1(choose_to_beScene::replace_callBack, this));
		be_a_guest->setScale(1.0);
		be_a_guest->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.7-i*100));
		be_a_guest->setColor(Color3B(0, 0, 0));
		be_a_guest->setTag(i);
		Menu* mn = Menu::create(be_a_guest, nullptr);
		mn->setPosition(0.0f, 0.0f);
		this->addChild(mn);
	}
	auto be_a_guest = MenuItemLabel::create(Label::createWithTTF("yourself", "fonts/Marker Felt.ttf", 70),
		CC_CALLBACK_1(choose_to_beScene::self_callBack, this));
	be_a_guest->setScale(1.0);
	be_a_guest->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.1));
	be_a_guest->setColor(Color3B(0, 0, 0));
	Menu* mn = Menu::create(be_a_guest, nullptr);
	mn->setPosition(0.0f, 0.0f);
	this->addChild(mn);
	
}
void choose_to_beScene::self_callBack(Ref* pSender)
{
	auto prt_sender = dynamic_cast<MenuItemLabel*>(pSender);
	auto my_client = client::create("127.0.0.1");
	auto help = searchScene::createScene(my_client);
	auto reScene = TransitionFade::create(1.0f, help);
	Director::getInstance()->pushScene(reScene);
}
void choose_to_beScene::replace_callBack(Ref* pSender)
{
	auto prt_sender = dynamic_cast<MenuItemLabel*>(pSender);
	auto my_client = client::create(ip_chosen[prt_sender->getTag()]);
	auto help=searchScene::createScene(my_client);
	auto reScene = TransitionFade::create(1.0f, help);
	Director::getInstance()->pushScene(reScene);

}
void choose_to_beScene::hostCallback(Ref* pSender)
{
	imformation::am_i_host = true;
	imformation::am_i_in_room = true;
	imformation::my_host_name = imformation::myname;
	imformation::guest_list.push_back(std::make_tuple(imformation::myname,false,false));
	auto server=LocalServer::create();
	this->addChild(server);

	auto my_client = client::create("127.0.0.1");
	log("something happend");
	auto help = roomScene::createScene(my_client);
	auto reScene = TransitionFade::create(1.0f, help);
	Director::getInstance()->pushScene(reScene);
	

}
