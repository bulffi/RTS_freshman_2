#include"cocos2d.h"
#include"roomScene.h"
#include"ui\CocosGUI.h"

USING_NS_CC;
talk_to_svr::ptr pClient;
//bool am_i_host;
int which_map;
int cnt_ask;
std::string host_name;
std::string my_name;
int length_of_name;
cocos2d::MenuItemToggle* map_index;
typedef std::tuple<std::string, int, int> player_imfor;
std::vector<player_imfor> players;
//typedef std::tuple<std::string, cocos2d::MenuItemToggle*, cocos2d::MenuItemToggle*> client_set;
//std::vector<client_set>;
Scene* roomScene::createScene(talk_to_svr::ptr ptr_client, bool if_host,std::string host,std::string my)

{
	pClient = ptr_client;
	//am_i_host = if_host;
	host_name = host;
	my_name = my;
	
	
	length_of_name = sizeof(my_name) - 1;
	
	auto scene = Scene::create();
	auto layer = roomScene::create();
	
	scene->addChild(layer);
	return scene;
}

void roomScene::tell_map_changed(Ref *pSender)
{
	if (which_map)
		which_map = 0;
	else
		which_map = 1;
	std::string mun = std::to_string(which_map);
	pClient->write("POdf&*(+345*" + std::string("map_changed")+mun+"$");

}
void to_check_2();
void roomScene::update(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	std::string tempt= pClient->read();
	while (cnt_ask < 5)
	{
		pClient->write("whoin$");
		cnt_ask++;
	}
	if (tempt != "no")
	{

	//	std::string tempt = pClient->read();
		if (tempt.find("client_in_room ") == 0)
		{
			players.clear();
			std::string name_tem;
			int nation_tem = 1;
			int team_tem = 2;
			std::istringstream in(tempt);
			std::string holder;
			in >> name_tem;
			while (in >> holder)
			{
				if (holder[0] == '0')
					nation_tem = 0;
				else
					nation_tem = 1;
				if (holder[1] == '0')
					team_tem = 0;
				else
					team_tem = 1;
				name_tem = holder.substr(2);
				players.push_back(player_imfor(name_tem, nation_tem, team_tem));
			}
		}
		if (tempt.find("map ") == 0)
		{
			std::istringstream in_2(tempt);
			std::string holder_2;
			in_2 >> holder_2 >> holder_2;
			which_map = std::atoi(holder_2.c_str());
		}
		if (tempt.find("map ") != 0 || tempt.find("client_in_room ") == 0)
		{
			int guest_count = 0;
			for (auto begin = players.begin(); begin != players.end(); begin++)
			{
				if (my_name == host_name)
				{
					auto map_1_menu = MenuItemImage::create(
						"scene/number-1-red-hi.png",
						"scene/number-1-red-hi.png"
						);
					map_1_menu->setScale(0.4);
					auto map_2_menu = MenuItemImage::create(
						"scene/red-number-2-hi.png",
						"scene/red-number-2-hi.png"
						);
					map_2_menu->setScale(0.4);
					auto map_toggle = MenuItemToggle::createWithCallback(
						CC_CALLBACK_1(roomScene::tell_map_changed, this),
						map_1_menu,
						map_2_menu,
						nullptr
						);
					map_toggle->setPosition(origin.x + visibleSize.width*0.2, origin.y + visibleSize.height*0.8);
					auto mn = Menu::create(map_toggle, nullptr);
					mn->setPosition(0, 0);
					this->addChild(mn);
				}
				else
				{
					Sprite* map_sprite;
					if (which_map == 1)
						map_sprite = Sprite::create("scene/red-number-2-hi.png");
					else
						map_sprite = Sprite::create("scene/number-1-red-hi.png");
					map_sprite->setScale(0.4);
					map_sprite->setPosition(origin.x + visibleSize.width*0.2, origin.y + visibleSize.height*0.8);
					this->addChild(map_sprite);
				}
				auto host_name_label = Label::create(std::get<0>(*begin), "fonts/Marker Felt.ttf", 60);
				host_name_label->setColor(Color3B(0, 0, 0));
				host_name_label->setPosition(origin.x + visibleSize.width*0.4, origin.y + visibleSize.height*0.7 - 100 * (guest_count));
				this->addChild(host_name_label);
				if (my_name == std::get<0>(*begin))
				{
					auto nation_1_menu = MenuItemImage::create(
						"scene/red-rounded-with-number-1-th.png",
						"scene/red-rounded-with-number-1-th.png"
						);
					//nation_1_menu->setScale(0.1);
					auto nation_2_menu = MenuItemImage::create(
						"scene/red-rounded-with-number-2-th.png",
						"scene/red-rounded-with-number-2-th.png"
						);
					//nation_2_menu->setScale(0.1);
					auto nation_toggle = MenuItemToggle::createWithCallback(
						CC_CALLBACK_1(roomScene::tell_nation_changed, this),
						nation_1_menu,
						nation_2_menu,
						nullptr
						);
					nation_toggle->setPosition(origin.x + visibleSize.width*0.6, origin.y + visibleSize.height*0.7 - 100 * (guest_count));
					auto team_1_menu = MenuItemImage::create(
						"scene/red-rounded-with-number-1-th.png",
						"scene/red-rounded-with-number-1-th.png"
						);
					//team_1_menu->setScale(0.1);
					auto team_2_menu = MenuItemImage::create(
						"scene/red-rounded-with-number-2-th.png",
						"scene/red-rounded-with-number-2-th.png"
						);
					//team_2_menu->setScale(0.1);
					auto team_toggle = MenuItemToggle::createWithCallback(
						CC_CALLBACK_1(roomScene::tell_team_changed, this),
						team_1_menu,
						team_2_menu,
						nullptr
						);
					team_toggle->setPosition(origin.x + visibleSize.width*0.8, origin.y + visibleSize.height*0.7 - 100 * (guest_count));
					auto mn = Menu::create(nation_toggle, team_toggle, nullptr);
					mn->setPosition(0, 0);
					this->addChild(mn);
				}
				else
				{
					Sprite* nation_sprite;
					Sprite* team_sprite;
					if (std::get<1>(*begin))
					{
						nation_sprite = Sprite::create("scene/red-rounded-with-number-2-th.png");
					}
					else
					{
						nation_sprite = Sprite::create("scene/red-rounded-with-number-1-th.png");
					}
					nation_sprite->setPosition(origin.x + visibleSize.width*0.6, origin.y + visibleSize.height*0.7 - 100 * (guest_count));
					this->addChild(nation_sprite);
					if (std::get<2>(*begin))
					{
						team_sprite = Sprite::create("scene/red-rounded-with-number-2-th.png");
					}
					else
					{
						team_sprite = Sprite::create("scene/red-rounded-with-number-1-th.png");
					}
					team_sprite->setPosition(origin.x + visibleSize.width*0.8, origin.y + visibleSize.height*0.7 - 100 * (guest_count));
					this->addChild(team_sprite);
				}
				guest_count++;
			}
		}

	}
}
bool roomScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background_color = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background_color, -1);
	auto nation = Label::create("Nation", "fonts/Marker Felt.ttf", 50);
	nation->setColor(Color3B(0, 0, 0));
	nation->setPosition(origin.x + visibleSize.width*0.6, origin.y + visibleSize.height*0.7+100);
	this->addChild(nation);
	auto team = Label::create("Team", "fonts/Marker Felt.ttf", 50);
	team->setColor(Color3B(0, 0, 0));
	team->setPosition(origin.x + visibleSize.width*0.8, origin.y + visibleSize.height*0.7+100);
	this->addChild(team);
	auto send_mes = MenuItemLabel::create(Label::createWithTTF("send", "fonts/Marker Felt.ttf", 70),
			CC_CALLBACK_1(roomScene::send_message, this));
	send_mes->setScale(1.0);
	send_mes->setPosition(Vec2(origin.x + visibleSize.width-send_mes->getContentSize().width, origin.y + 0.5*send_mes->getContentSize().height));
	send_mes->setColor(Color3B(0, 0, 0));
	auto mesge = Menu::create(send_mes, nullptr);
	mesge->setPosition(0, 0);
	this->addChild(mesge);
	_chatWindow = cocos2d::ui::TextField::create("input words here", "fonts/Marker Felt.ttf", 50);
	_chatWindow->setMaxLengthEnabled(true);
	_chatWindow->setMaxLength(20);
	_chatWindow->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+0.5*_chatWindow->getContentSize().height));
	_chatWindow->addEventListener(CC_CALLBACK_2(roomScene::textFieldEvent, this));
	_chatWindow->setColor(Color3B(0, 0, 0));
	this->addChild(_chatWindow, 2);
	pClient->write("whoin$");
	//pClient->write("which_map$");
	
	/*auto BackEventListener = EventListenerKeyboard::create();
	BackEventListener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		if (code == EventKeyboard::KeyCode::KEY_SPACE)
		{
			to_check_2();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(BackEventListener, this);*/
	this->scheduleUpdate();
	return true;
}



void to_check_2()
{
	std::string tempt = pClient->read();
	log(tempt.c_str());
}

std::string current_str;
void roomScene::send_message(Ref *pSender)
{
	if (!current_str.empty())
	{
		pClient->write("talk"+ std::to_string(length_of_name) + my_name + current_str + "$");
	}

}
void roomScene::tell_team_changed(Ref *pSender)
{
	int result = 0;
	for (auto &pla : players)
	{
		if (std::get<0>(pla) == my_name)
		{
			if (std::get<2>(pla))
			{
				std::get<2>(pla) = 0;
				result = 0;
			}
			else
			{
				std::get<2>(pla) = 1;
				result = 1;
			}
		}
	}
	pClient->write("team"+std::to_string(length_of_name) + my_name + std::to_string(result) + "$");
}
void roomScene::tell_nation_changed(Ref *pSender)
{
	int result=0;
	for (auto &pla : players)
	{
		if (std::get<0>(pla) == my_name)
		{
			if (std::get<1>(pla))
			{
				std::get<1>(pla) = 0;
				result = 0;
			}
			else
			{
				std::get<1>(pla) = 1;
				result = 1;
			}
		}
	}
	pClient->write(my_name + "nation" + std::to_string(result)+"$");

}

void roomScene::textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
	{
		cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		current_str = textField->getString();
	}
	break;

	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
	{
		cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		current_str = textField->getString();
	}
	break;

	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
	{
		cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		current_str = textField->getString();
	}
	break;

	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
	{
		cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		current_str = textField->getString();
	}
	break;

	default:
		break;
	}
}