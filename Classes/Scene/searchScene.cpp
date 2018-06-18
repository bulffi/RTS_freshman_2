#include"cocos2d.h"
#include"searchScene.h"
#include"roomScene.h"
#include<vector>
#include<string>
USING_NS_CC;
std::vector<std::string> host_names;
client* pClient_2;
std::string name_of_mine;
int room_count;
Scene* searchScene::createScene(client* ptr_client)

{
	pClient_2 = ptr_client;
	
	name_of_mine = UserDefault::getInstance()->getStringForKey("UserName");
	auto scene = Scene::create();
	auto layer = searchScene::create();
	scene->addChild(layer);
	return scene;
}
bool searchScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background_color = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background_color, -1);
	auto circle = Sprite::create("scene/loading.png");
	circle->setPosition(Point(visibleSize.width / 4-100, visibleSize.height / 2 + 100));
	circle->setScale(0.4, 0.4);
	this->addChild(circle, 1);
	circle->runAction(RepeatForever::create(RotateBy::create(1, 360)));
	pClient_2->sendMessage("who_are_you");//if a real ´óÌü it should be a vector!!
	this->scheduleUpdate();
	return true;
}
void searchScene::joinCallback(Ref* pSender)
{
	pClient_2->sendMessage("want_to_join"+name_of_mine);

}
void searchScene::update(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	std::string tempt=pClient_2->executeOrder();
	if (tempt != "no")
	{
		if (tempt.find("my_name_is")==0)
		{
			std::istringstream in(tempt);
			std::string hostname(tempt.substr(sizeof("my_name_is")-1));
			room_count++;
			auto room_name = Label::create(hostname.c_str(), "fonts/Marker Felt.ttf", 70);
			room_name->setColor(Color3B(0, 0, 0));
			room_name->setPosition(origin.x + visibleSize.width*0.5, origin.y + visibleSize.height*0.7 -150*(room_count-1));
			this->addChild(room_name);
			host_names.push_back(hostname);
			auto join = MenuItemLabel::create(Label::createWithTTF("join", "fonts/Marker Felt.ttf", 70),
				CC_CALLBACK_1(searchScene::joinCallback, this));
			join->setScale(1.0);
			join->setPosition(Vec2(room_name->getPosition().x+room_name->getContentSize().width+100 , room_name->getPosition().y));
			join->setColor(Color3B(0, 0, 0));
			Menu* mn = Menu::create(join,nullptr);
			mn->setPosition(0.0f, 0.0f);
			this->addChild(mn);
		}
		if (tempt.find("join") == 0)
		{
			this->unschedule(schedule_selector(searchScene::update));
			std::string host_to_join = tempt.substr(sizeof("join" - 1));
			imformation::am_i_host = false;
			imformation::my_host_name = host_to_join;
			imformation::am_i_in_room = true;
			typedef std::chrono::duration<int, std::milli> millisecond;
			std::this_thread::sleep_for(millisecond(90));
			auto help = roomScene::createScene(pClient_2);
			auto reScene = TransitionFade::create(1.0f, help);
			Director::getInstance()->replaceScene(reScene);
		}









	}



}
