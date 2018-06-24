#include"cocos2d.h"
#include"roomScene.h"
#include"ui\CocosGUI.h"
#include"lose.h"
#include"SimpleAudioEngine.h"
#include"game\HelloWorldScene.h"
USING_NS_CC;
client* pClient;
int which_map;
int cnt_ask;
std::string host_name;
std::string my_name;
typedef std::tuple<std::string, int, int> player_imfor;
std::vector<player_imfor> players;
std::vector<cocos2d::Node*> name_nation_team;
Scene* roomScene::createScene(client* ptr_client)

{
	std::vector<std::vector<cocos2d::Sprite*>> my_big;
	typedef std::vector<cocos2d::Sprite*> a_type_of_one_player;
	a_type_of_one_player someone;
	someone.push_back(cocos2d::Sprite::create());
	my_big.push_back(someone);
	pClient = ptr_client;
	host_name = imformation::my_host_name;
	my_name = imformation::myname;
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
	pClient->sendMessage(std::string("map_changed ") + mun);

}
void roomScene::update(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	while (cnt_ask < 5)
	{
		
		pClient->sendMessage("whoin");
		cnt_ask++;
	}
	while(cnt_ask < 20)
	{
		pClient->sendMessage("which_map");
		cnt_ask++;
	}
	std::string tempt = pClient->executeOrder();
	if (tempt != "no")
	{
		if (tempt.find("which_map") == 0)
		{
			if (imformation::am_i_host)
			{
				pClient->sendMessage(std::string("map_changed ") + std::to_string(which_map));
			}
		}
		else if (tempt.find("who_are_you") == 0)
		{
			if (imformation::am_i_host)
			{
				pClient->sendMessage("my_name_is"+imformation::my_host_name);
			}
		}
		else if (tempt.find("want_to_join") == 0)
		{
			if (imformation::am_i_host)
			{
				std::string client_name = tempt.substr(sizeof("want_to_join") - 1);
				if (imformation::guest_list.size() > 4)
				{
					pClient->sendMessage("no_room");
				}
				else 
				{
					imformation::guest_list.push_back(std::make_tuple(client_name, 0, 0));
					pClient->sendMessage("join"+imformation::myname);
				}
				
			}
		}
		else if (tempt.find("whoin") == 0)
		{
			
			if (imformation::am_i_host)
			{
				std::string answer("client_in_room ");
				for (auto guest : imformation::guest_list)
				{
					std::string tempt;
					tempt += std::to_string(std::get<1>(guest)) + std::to_string(std::get<2>(guest)) + std::get<0>(guest);
					answer += tempt + " ";
				}
				pClient->sendMessage(answer);
			}
		
		}
		else if (tempt.find("client_in_room ") == 0)
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
		else if (tempt.find("map_changed ") == 0)
		{
			std::istringstream in_2(tempt);
			std::string holder_2;
			in_2 >> holder_2 >> holder_2;
			which_map = std::atoi(holder_2.c_str());
		}
		else if (tempt.find("START!!!!!!!!!!") == 0)
		{
			unscheduleUpdate();
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			typedef std::chrono::duration<int, std::milli> millisceond;
			std::this_thread::sleep_for(millisceond(90));
			imformation::map_index = which_map+1;
			HelloWorld::num_player = 4;
			auto help=HelloWorld::createScene(pClient, imformation::my_number+1);
			auto reScene = TransitionFade::create(1.0f, help);
			Director::getInstance()->replaceScene(reScene);
		}
		else if (tempt.find("0") == 0 || tempt.find("1") == 0 || tempt.find("2") == 0 || tempt.find("3") == 0)
		{
			std::string what_happen = tempt.substr(1);
			if (imformation::am_i_host)
			{
				std::string target_name = std::get<0>(imformation::guest_list[(tempt[0] - '0')]);
				if (what_happen.find("team") == 0)
				{
					if (imformation::am_i_host)
					{
						for (auto &guset : imformation::guest_list)
						{
							if (std::get<0>(guset) == target_name)
							{
								std::get<2>(guset) = atoi(what_happen.substr(4).c_str());
							}

						}
					}
				}
				else if (what_happen.find("nation") == 0)
				{
					if (imformation::am_i_host)
					{
						for (auto &guset : imformation::guest_list)
						{
							if (std::get<0>(guset) == target_name)
							{
								std::get<1>(guset) = atoi(what_happen.substr(6).c_str());
							}

						}
					}
				}

				if (what_happen.find("team") == 0 || what_happen.find("nation") == 0)
				{
					if (imformation::am_i_host)
						pClient->sendMessage("whoin");
				}
			}
			if (what_happen.find("talk") == 0)
			{
				auto saying = cocos2d::ui::Button::create();
				std::string message_to_display;
				switch (tempt[0]-'0')
				{
				case 0:message_to_display = "E: " + what_happen.substr(4); break;
				case 1:message_to_display = "D: " + what_happen.substr(4); break;
				case 2:message_to_display = "P: " + what_happen.substr(4); break;
				case 3:message_to_display = "H: " + what_happen.substr(4); break;
				default:
					break;
				}
				
				auto text = Label::create(message_to_display, "fonts/Marker Felt.ttf", 50);
				text->setColor(Color3B(0, 0, 0));
				saying->setTitleLabel(text);
				DelayTime * delay = DelayTime::create(1.0f);
				auto callFunc1 = CallFunc::create([=] {
					this->removeChild(saying);
				});
				auto sequence = Sequence::create(delay, callFunc1, NULL);
				saying->addClickEventListener(CC_CALLBACK_1(roomScene::shut_down_callback, this));
				saying->setPosition(Vec2(origin.x + visibleSize.width*0.4-250, origin.y + visibleSize.height*0.7 - 120 * (tempt[0] - '0')));
				saying->runAction(sequence);
				this->addChild(saying);
			}
		}
		if (tempt.find("map_changed ") == 0 || tempt.find("client_in_room ") == 0)
		{
			int guest_count = 0;
			for (auto &node : name_nation_team)
			{
				this->removeChild(node);
			}
			for (auto begin = players.begin(); begin != players.end(); begin++)
			{

//==============================host's map toggle========================================================
				if (imformation::my_host_name == imformation::myname)
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
					map_toggle->setSelectedIndex(which_map);
					auto mn = Menu::create(map_toggle, nullptr);
					name_nation_team.push_back(mn);
					mn->setPosition(0, 0);
					this->addChild(mn);
				}

//=========================================everyone's name================================
				auto host_name_label = Label::create(std::get<0>(*begin), "fonts/Marker Felt.ttf", 60);
				host_name_label->setColor(Color3B(0, 0, 0));
				host_name_label->setPosition(origin.x + visibleSize.width*0.4, origin.y + visibleSize.height*0.7 - 120 * (guest_count));
				name_nation_team.push_back(host_name_label);
				this->addChild(host_name_label);
//========================================my own part includeing map_sprite, nation_toggle, team_toggle============================
				if (my_name == std::get<0>(*begin))
				{
					imformation::my_number = begin - players.begin();
					Sprite* map_sprite;
					if (which_map == 1)
						map_sprite = Sprite::create("scene/red-number-2-hi.png");
					else
						map_sprite = Sprite::create("scene/number-1-red-hi.png");
					map_sprite->setScale(0.4);
					map_sprite->setPosition(origin.x + visibleSize.width*0.2, origin.y + visibleSize.height*0.8);
					name_nation_team.push_back(map_sprite);
					this->addChild(map_sprite);
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
					nation_toggle->setPosition(origin.x + visibleSize.width*0.6, origin.y + visibleSize.height*0.7 - 120 * (guest_count));
					nation_toggle->setSelectedIndex(static_cast<int>(imformation::my_nation));
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
					team_toggle->setPosition(origin.x + visibleSize.width*0.8, origin.y + visibleSize.height*0.7 - 120 * (guest_count));
					team_toggle->setSelectedIndex(imformation::my_team);
					auto mn = Menu::create(nation_toggle, team_toggle, nullptr);
					name_nation_team.push_back(mn);
					mn->setPosition(0, 0);
					this->addChild(mn);
				}
//=======================================================others includeing nation_sprite, team_sprite================================
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
					nation_sprite->setPosition(origin.x + visibleSize.width*0.6, origin.y + visibleSize.height*0.7 - 120 * (guest_count));
					name_nation_team.push_back(nation_sprite);
					this->addChild(nation_sprite);
					if (std::get<2>(*begin))
					{
						team_sprite = Sprite::create("scene/red-rounded-with-number-2-th.png");
					}
					else
					{
						team_sprite = Sprite::create("scene/red-rounded-with-number-1-th.png");
					}
					team_sprite->setPosition(origin.x + visibleSize.width*0.8, origin.y + visibleSize.height*0.7 - 120 * (guest_count));
					name_nation_team.push_back(team_sprite);
					this->addChild(team_sprite);
				}
				guest_count++;
			}
		}

	}
}
std::string current_str;
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
	nation->setPosition(origin.x + visibleSize.width*0.6, origin.y + visibleSize.height*0.7 + 100);
	this->addChild(nation);
	auto team = Label::create("Team", "fonts/Marker Felt.ttf", 50);
	team->setColor(Color3B(0, 0, 0));
	team->setPosition(origin.x + visibleSize.width*0.8, origin.y + visibleSize.height*0.7 + 100);
	this->addChild(team);
	auto send_mes = MenuItemLabel::create(Label::createWithTTF("send", "fonts/Marker Felt.ttf", 70),
		CC_CALLBACK_1(roomScene::send_message, this));
	send_mes->setScale(1.0);
	send_mes->setPosition(Vec2(origin.x + visibleSize.width - send_mes->getContentSize().width, origin.y + 0.5*send_mes->getContentSize().height));
	send_mes->setColor(Color3B(0, 0, 0));
	auto mesge = Menu::create(send_mes, nullptr);
	mesge->setPosition(0, 0);
	this->addChild(mesge);
	_chatWindow = cocos2d::ui::TextField::create("input words here", "fonts/Marker Felt.ttf", 50);
	_chatWindow->setMaxLengthEnabled(true);
	_chatWindow->setMaxLength(30);
	_chatWindow->setPosition(Vec2(origin.x + visibleSize.width*0.5, origin.y + 0.5*_chatWindow->getContentSize().height));
	_chatWindow->addEventListener(CC_CALLBACK_2(roomScene::textFieldEvent, this));
	_chatWindow->setColor(Color3B(0, 0, 0));
	this->addChild(_chatWindow, 2);
	int cnt_whion = 0;
	while (cnt_whion < 3)
	{
		pClient->sendMessage("whoin");
		cnt_whion++;
	}
	if (imformation::am_i_host)
	{
		auto be_a_guest = MenuItemLabel::create(Label::createWithTTF("begin", "fonts/Marker Felt.ttf", 70),
			CC_CALLBACK_1(roomScene::playCallback, this));
		be_a_guest->setScale(1.0);
		be_a_guest->setPosition(Vec2(origin.x + visibleSize.width-100, origin.y + visibleSize.height*0.2));
		be_a_guest->setColor(Color3B(0, 0, 0));
		Menu* mn = Menu::create(be_a_guest, nullptr);
		mn->setPosition(0.0f, 0.0f);
		this->addChild(mn);
	}
	auto key_board = EventListenerKeyboard::create();
	key_board->onKeyPressed = [&](EventKeyboard::KeyCode code, Event* what_happen)
	{
		if (code == EventKeyboard::KeyCode::KEY_ENTER)
		{
			if (!current_str.empty())
			{
				pClient->sendMessage(std::to_string(imformation::my_number) + "talk" + current_str);
				_chatWindow->setText("");
			}
		}
	};
	auto dispather = Director::getInstance()->getEventDispatcher();
	dispather->addEventListenerWithSceneGraphPriority(key_board, this);
	this->scheduleUpdate();
	return true;
}
void roomScene::playCallback(Ref *pSender)
{

	if (imformation::guest_list.size())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		this->unschedule(schedule_selector(roomScene::update));
		pClient->sendMessage("START!!!!!!!!!!");
		
		//auto help = loseScene::createScene();
		//auto reScene = TransitionFade::create(1.0f, help);
		//Director::getInstance()->replaceScene(reScene);
	}
}

void roomScene::send_message(Ref *pSender)
{
	
	if (!current_str.empty())
	{
		pClient->sendMessage(std::to_string(imformation::my_number) + "talk" + current_str);
		_chatWindow->setText("");
	}

}
void roomScene::tell_team_changed(Ref *pSender)
{
	auto toggle = dynamic_cast<MenuItemToggle*>(pSender);
	int choice_now=toggle->getSelectedIndex();
	for (auto &pla : players)
	{
		if (std::get<0>(pla) == my_name)
		{
			std::get<2>(pla) = choice_now;
		}
	}
	imformation::my_team = choice_now;
	pClient->sendMessage(std::to_string(imformation::my_number) + "team" + std::to_string(choice_now));
}
void roomScene::tell_nation_changed(Ref *pSender)
{
	auto toggle = dynamic_cast<MenuItemToggle*>(pSender);
	int choice_now = toggle->getSelectedIndex();
	for (auto &pla : players)
	{
		if (std::get<0>(pla) == my_name)
		{
			std::get<1>(pla) = choice_now;
		}
	}
	imformation::my_nation = choice_now;
	pClient->sendMessage(std::to_string(imformation::my_number) + "nation" + std::to_string(choice_now));

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