#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include"utility\information.h"
#include<iostream>
#include<vector>
#include"menu.h"
#include"Scene\lose.h"
USING_NS_CC;
int HelloWorld::my_country;
int imformation::map_index;
//类外默认初始化地图参数
int imformation::XMAX;
int imformation::YMAX;
int imformation::X_SIZE;
int imformation::Y_SIZE;
int imformation::X_BASE1;
int imformation::Y_BASE1;
int imformation::X_BASE2;
int imformation::Y_BASE2;
int imformation::X_BASE3;
int imformation::Y_BASE3;
int imformation::X_BASE4;
int imformation::Y_BASE4;
//矿车的瓦片坐标
int imformation::X_MINE1;
int imformation::Y_MINE1;
int imformation::X_MINE2;
int imformation::Y_MINE2;
int imformation::X_MINE3;
int imformation::Y_MINE3;
int imformation::X_MINE4;
int imformation::Y_MINE4;

//服务器数据
/*std::string imformation::myname;
bool imformation::am_i_host;
bool imformation::am_i_in_room;
std::string imformation::my_host_name;
bool imformation::my_nation;
bool imformation::my_team;
int imformation::my_number;
std::vector<std::tuple<std::string, bool, bool>> imformation::guest_list;//name nation team*/

extern client* pClient;
int tempt;
int HelloWorld::num_player;
bool if_there_is_effect;
//地图数据
cocos2d::TMXTiledMap* HelloWorld::_tileMap;
//int HelloWorld::situation[imformation::XMAX*imformation::YMAX];
std::vector<int> HelloWorld::situation;
std::vector<cocos2d::Vec2> HelloWorld::vec_mappos;//地图偏移量

//兵种及建筑
std::vector<std::vector<soldier*>> HelloWorld::vec_chosed_soldier;
std::vector<std::vector<people*>> HelloWorld::vec_people;
std::vector<std::vector<dog*>> HelloWorld::vec_dog;
std::vector<std::vector<tank*>> HelloWorld::vec_tank;
std::vector<base*> HelloWorld::vec_base;
std::vector<std::vector<camp*>> HelloWorld::vec_camp;
std::vector<camp*> HelloWorld::major_camp;//各方的主要兵营
std::vector<std::vector<mine*>> HelloWorld::vec_mine;
std::vector<std::vector<mine_car*>> HelloWorld::vec_mine_car;
std::vector<unit*> HelloWorld::vec_mines;
std::vector<std::vector<electric*>> HelloWorld::vec_electric;
std::vector<std::vector<factory*>> HelloWorld::vec_factory;
std::vector<factory*> HelloWorld::major_factory;//各方的主要战车工厂

//菜单数据
cocos2d::Vector<build_menu*> HelloWorld::vec_buimenu;//储存建筑菜单的容器
cocos2d::Vector<sol_menu*> HelloWorld::vec_solmenu;//储存兵种菜单的容器

Scene* HelloWorld::createScene(client* p_to_Client,int side)
{
	if_there_is_effect = UserDefault::getInstance()->getBoolForKey("isEffect",true);
	pClient = p_to_Client;
	tempt = side;
	if (side == 1)
	{
		imformation::am_i_host = 1;
		imformation::my_number = 1;
	}
	else
	{
		imformation::am_i_host = 0;
		imformation::my_number = 2;
	}

	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//显示body的边框
	cocos2d::Vect gravity(0, 0.0f);
	scene->getPhysicsWorld()->setGravity(gravity);

	//设置场景的大小
	cocos2d::Size scene_size(imformation::XMAX*imformation::X_SIZE, imformation::YMAX*imformation::Y_SIZE);
	scene->setContentSize(scene_size);

	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
cocos2d::Point tileCoordFromPosition(cocos2d::Vec2 pos)//GL坐标转瓦片坐标
{
	int x = pos.x / imformation::X_SIZE;
	int y = (imformation::Y_SIZE * imformation::YMAX - pos.y) / imformation::Y_SIZE;
	return cocos2d::Vec2(x, y);
}
cocos2d::Vec2 tileToGL(cocos2d::Point point)//瓦片坐标转GL坐标
{
	int x = (point.x + 0.5) * 32;
	int y = (imformation::YMAX - point.y - 0.5) * 32;
	cocos2d::Vec2 vec(x, y);
	return vec;
}
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	my_country = tempt;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/fire_enemy.WAV");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/dog_emeey.WAV");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/tank_emey.WAV");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/construct.WAV");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/baozha.WAV");

	//初始化所有vector
	std::vector<people*> vec_1;
	std::vector<camp*> vec_2;
	std::vector<soldier*> vec_3;
	std::vector<dog*> vec_4;
	std::vector<tank*> vec_5;
	std::vector<factory*> vec_6;
	std::vector<mine*> vec_7;
	std::vector<electric*> vec_8;
	std::vector<mine_car*> vec_0;
	camp* mmp;
	factory* mmp2;
	for (int i = 1; i <= 5; i++) {
		vec_people.push_back(vec_1);
		vec_camp.push_back(vec_2);
		vec_chosed_soldier.push_back(vec_3);
		vec_dog.push_back(vec_4);
		vec_tank.push_back(vec_5);
		vec_factory.push_back(vec_6);
		vec_mine.push_back(vec_7);
		vec_electric.push_back(vec_8);
		vec_mine_car.push_back(vec_0);
		set_building.push_back(0);
		mouseDownPosition.push_back(cocos2d::Vec2(0, 0));
		real_mouseDownPosition.push_back(cocos2d::Vec2(0, 0));
		major_camp.push_back(mmp);
		major_factory.push_back(mmp2);
		vec_base.push_back(NULL);
	}

	//初始化地图参数
	if (imformation::map_index == 1) {
		imformation::XMAX = 50;
		imformation::YMAX = 50;
		imformation::X_SIZE = 32;
		imformation::Y_SIZE = 32;
		imformation::X_BASE1 = 4;
		imformation::Y_BASE1 = 45;
		imformation::X_BASE2 = 45;
		imformation::Y_BASE2 = 4;
		imformation::X_BASE3 = 4;
		imformation::Y_BASE3 = 4;
		imformation::X_BASE4 = 45;
		imformation::Y_BASE4 = 45;
		imformation::X_MINE1 = 2;
		imformation::Y_MINE1 = 40;
		imformation::X_MINE2 = 40;
		imformation::Y_MINE2 = 2;
		imformation::X_MINE3 = 2;
		imformation::Y_MINE3 = 2;
		imformation::X_MINE4 = 40;
		imformation::Y_MINE4 = 40;
	}
	else if(imformation::map_index == 2) {
		imformation::XMAX = 100;
		imformation::YMAX = 100;
		imformation::X_SIZE = 32;
		imformation::Y_SIZE = 32;
		imformation::X_BASE1 = 8;
		imformation::Y_BASE1 = 95;
		imformation::X_BASE2 = 90;
		imformation::Y_BASE2 = 10;
		imformation::X_BASE3 = 5;
		imformation::Y_BASE3 = 11;
		imformation::X_BASE4 = 90;
		imformation::Y_BASE4 = 85;
		imformation::X_MINE1 = 4;
		imformation::Y_MINE1 = 81;
		imformation::X_MINE2 = 76;
		imformation::Y_MINE2 = 4;
		imformation::X_MINE3 = 19;
		imformation::Y_MINE3 = 14;
		imformation::X_MINE4 = 76;
		imformation::Y_MINE4 = 95;
	}


	//设置层的大小
	cocos2d::Size scene_size(imformation::XMAX*imformation::X_SIZE, imformation::YMAX*imformation::Y_SIZE);
	this->setContentSize(scene_size);

	//加入瓦片地图，并设置位置
	if(imformation::map_index==1)
		_tileMap = TMXTiledMap::create("map/first.tmx");
	else if(imformation::map_index == 2)
		_tileMap = TMXTiledMap::create("map2/redalert2.tmx");
	if(imformation::my_number==1)
		_tileMap->setPosition(Vec2(0, 0));
	else if(imformation::my_number == 2)
		_tileMap->setPosition(Vec2(-_tileMap->getContentSize().width - 211 + visibleSize.width, -_tileMap->getContentSize().height+visibleSize.height));
	else if (imformation::my_number == 3)
		_tileMap->setPosition(Vec2(0, -_tileMap->getContentSize().height + visibleSize.height));
	else if (imformation::my_number == 4)
		_tileMap->setPosition(Vec2(-_tileMap->getContentSize().width - 211 + visibleSize.width, 0));
	startpos = _tileMap->getPosition();
	vec_mappos.push_back(Vec2(0, 0));
	vec_mappos.push_back(Vec2(0, 0));
	vec_mappos.push_back(Vec2(-_tileMap->getContentSize().width - 211 + visibleSize.width, -_tileMap->getContentSize().height + visibleSize.height));
	vec_mappos.push_back(Vec2(0, -_tileMap->getContentSize().height + visibleSize.height));
	vec_mappos.push_back(Vec2(-_tileMap->getContentSize().width - 211 + visibleSize.width, 0));
	this->addChild(_tileMap, 0, 100);

	//加入碰撞层
	_colliable = _tileMap->layerNamed("collidable");
	_colliable->setVisible(false);
	//初始化碰撞层situation
	for (int i = 0; i < imformation::YMAX; i++)
	{
		for (int j = 0; j < imformation::XMAX; j++)
		{
			Vec2 tileCoord(j, i);
			int tileGid = _colliable->getTileGIDAt(tileCoord);
			if (tileGid > 0)
				//situation[j + XMAX*i] = 0;
				situation.push_back(0);
			else
				//situation[j + XMAX*i] = 1;
				situation.push_back(1);
		}
	}

	//世界的边界的物理引擎
	auto abody = PhysicsBody::createEdgeBox(scene_size, PhysicsMaterial(1000, 0, 0), 1.0f);
	this->setPhysicsBody(abody);

	//创建每一方基地
	vec_base[1] = base::create("base1.png");
	vec_base[2] = base::create("base2.png");
	vec_base[3] = base::create("base3.png");
	vec_base[4] = base::create("base4.png");
	for (int i = 1; i <= 4; i++)
		vec_base[i]->set_data(i);
	//创建每一方矿
	unit* mines1 = unit::create("mountain.png");
	unit* mines2 = unit::create("mountain.png");
	unit* mines3 = unit::create("mountain.png");
	unit* mines4 = unit::create("mountain.png");
	vec_mines.push_back(mines1);
	vec_mines.push_back(mines1);
	vec_mines.push_back(mines2);
	vec_mines.push_back(mines3);
	vec_mines.push_back(mines4);
	//设置基地物理引擎边框
	cocos2d::Size building_size(vec_base[1]->getContentSize().width, vec_base[1]->getContentSize().height);
	cocos2d::PhysicsBody* body1 = PhysicsBody::createEdgeBox(building_size, PhysicsMaterial(10, 0, 0), 1.0f);
	cocos2d::PhysicsBody* body2 = PhysicsBody::createEdgeBox(building_size, PhysicsMaterial(10, 0, 0), 1.0f);
	cocos2d::PhysicsBody* body3 = PhysicsBody::createEdgeBox(building_size, PhysicsMaterial(10, 0, 0), 1.0f);
	cocos2d::PhysicsBody* body4= PhysicsBody::createEdgeBox(building_size, PhysicsMaterial(10, 0, 0), 1.0f);
	vec_base[1]->setPhysicsBody(body1);
	vec_base[2]->setPhysicsBody(body2);
	vec_base[3]->setPhysicsBody(body3);
	vec_base[4]->setPhysicsBody(body4);
	//设置基地位置
	for (int i = 1; i <= 4; i++)
		vec_base[i]->setAnchorPoint(Vec2(0.5, 0.5));
	vec_base[1]->setPosition(cocos2d::Vec2(32 * (imformation::X_BASE1 + 0.5), 32 * (imformation::YMAX - imformation::Y_BASE1 - 0.5)));
	vec_base[2]->setPosition(cocos2d::Vec2(32 * (imformation::X_BASE2 + 0.5), 32 * (imformation::YMAX - imformation::Y_BASE2 - 0.5)));
	if(num_player>=3)
		vec_base[3]->setPosition(cocos2d::Vec2(32 * (imformation::X_BASE3 + 0.5), 32 * (imformation::YMAX - imformation::Y_BASE3 - 0.5)));
	if (num_player >= 4)
		vec_base[4]->setPosition(cocos2d::Vec2(32 * (imformation::X_BASE4 + 0.5), 32 * (imformation::YMAX - imformation::Y_BASE4 - 0.5)));
	vec_base[1]->tile_position_point[0] = imformation::X_BASE1;
	vec_base[1]->tile_position_point[1] = imformation::Y_BASE1;
	vec_base[2]->tile_position_point[0] = imformation::X_BASE2;
	vec_base[2]->tile_position_point[1] = imformation::Y_BASE2;
	vec_base[3]->tile_position_point[0] = imformation::X_BASE3;
	vec_base[3]->tile_position_point[1] = imformation::Y_BASE3;
	vec_base[4]->tile_position_point[0] = imformation::X_BASE4;
	vec_base[4]->tile_position_point[1] = imformation::Y_BASE4;
	//设置基地血条
	for (int i = 1; i <= 4; i++)
		createblood(vec_base[i], 4);
	//将基地加入到map中
	_tileMap->addChild(vec_base[1]);
	_tileMap->addChild(vec_base[2]);
	if(num_player>=3)
		_tileMap->addChild(vec_base[3]);
	if (num_player >= 4)
		_tileMap->addChild(vec_base[4]);
	//更改基地的situation
	for(int i=-1;i<=1;i++)
		for(int j= -1;j<=1;j++)
			situation[(imformation::X_BASE1+j) + imformation::XMAX*(imformation::Y_BASE1+i)] = 0;
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			situation[(imformation::X_BASE2+j) + imformation::XMAX*(imformation::Y_BASE2+i)] = 0;
	if(num_player>=3)
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				situation[(imformation::X_BASE3 + j) + imformation::XMAX*(imformation::Y_BASE3 + i)] = 0;
	if (num_player >= 4)
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				situation[(imformation::X_BASE4 + j) + imformation::XMAX*(imformation::Y_BASE4 + i)] = 0;


	//设置矿山物理引擎边框
	cocos2d::Size mine_size(vec_mines[1]->getContentSize().width, vec_mines[1]->getContentSize().height);
	cocos2d::PhysicsBody* body5 = PhysicsBody::createEdgeBox(mine_size, PhysicsMaterial(10, 0, 0), 1.0f);
	cocos2d::PhysicsBody* body6 = PhysicsBody::createEdgeBox(mine_size, PhysicsMaterial(10, 0, 0), 1.0f);
	cocos2d::PhysicsBody* body7 = PhysicsBody::createEdgeBox(mine_size, PhysicsMaterial(10, 0, 0), 1.0f);
	cocos2d::PhysicsBody* body8 = PhysicsBody::createEdgeBox(mine_size, PhysicsMaterial(10, 0, 0), 1.0f);
	vec_base[1]->setPhysicsBody(body5);
	vec_base[2]->setPhysicsBody(body6);
	vec_base[3]->setPhysicsBody(body7);
	vec_base[4]->setPhysicsBody(body8);
	//矿山的位置
	int x[5] = { 0,imformation::X_MINE1,imformation::X_MINE2 ,imformation::X_MINE3 ,imformation::X_MINE4 };
	int y[5] = { 0,imformation::Y_MINE1,imformation::Y_MINE2,imformation::Y_MINE3,imformation::Y_MINE4 };
	for (int i = 1; i <= 4; i++)
	{
		vec_mines[i]->setAnchorPoint(Vec2(0.5, 0.5));
		vec_mines[i]->setPosition(cocos2d::Vec2(32 * (x[i] + 0.5), 32 * (imformation::YMAX - y[i] - 0.5)));
	}
	//将矿山加入到map中
	_tileMap->addChild(vec_mines[1]);
	_tileMap->addChild(vec_mines[2]);
	if (num_player >= 3)
		_tileMap->addChild(vec_mines[3]);
	if (num_player >= 4)
		_tileMap->addChild(vec_mines[4]);
	//更改矿山的situation
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			situation[(imformation::X_MINE1 + j) + imformation::XMAX*(imformation::Y_MINE1 + i)] = 0;
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			situation[(imformation::X_MINE2 + j) + imformation::XMAX*(imformation::Y_MINE2 + i)] = 0;
	if (num_player >= 3)
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				situation[(imformation::X_MINE3 + j) + imformation::XMAX*(imformation::Y_MINE3 + i)] = 0;
	if (num_player >= 4)
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				situation[(imformation::X_MINE4 + j) + imformation::XMAX*(imformation::Y_MINE4 + i)] = 0;

	//初始化旁栏
	initmenu();

	//打开血量监测调度器
	this->schedule(schedule_selector(HelloWorld::updateHealth));

	//打开接收服务器消息的调度器
	this->scheduleUpdate();

	//鼠标监听器
	auto _mouseListen1 = EventListenerMouse::create();

	//鼠标按下
	_mouseListen1->onMouseDown = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		
		cocos2d::__String str_write;
		std::ostringstream ostr;
		ostr << my_country << "MD";
		float X = e->getCursorX();
		float Y = e->getCursorY();
		for (int m = 1000; m >= 1; m /= 10)
		{
			int n = X / m;
			ostr << n;
			X -= n*m;
		}
		for (int m = 1000; m >= 1; m /= 10)
		{
			int n = Y / m;
			ostr << n;
			Y -= n*m;
		}
		str_write = ostr.str();
		pClient->sendMessage(str_write.getCString());
		int i=1;
	};
	//鼠标松开
	_mouseListen1->onMouseUp = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		cocos2d::__String str_write;
		std::ostringstream ostr;
		ostr << my_country << "MU";
		if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)//如果点击了左键
			ostr << 'L';
		else
			ostr << 'R';
		float X = e->getCursorX();
		float Y = e->getCursorY();
		for (int m = 1000; m >= 1; m /= 10)
		{
			int n = X / m;
			ostr << n;
			X -= n*m;
		}
		for (int m = 1000; m >= 1; m /= 10)
		{
			int n = Y / m;
			ostr << n;
			Y -= n*m;
		}
		str_write = ostr.str();
		pClient->sendMessage(str_write.getCString());
		int i = 1;
	};
	//注册监听器
	auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListen1, this);

    return true;
}
int waiting_to_ask = 0;
void HelloWorld::update(float dt)//接收服务器消息
{
	waiting_to_ask++;
	std::string message_ = pClient->executeOrder();
	const char* pmessage = message_.c_str();

	if (pmessage[0] != 'n')//如果有消息传来
	{
		int country;
		if (pmessage[0] == '1')
			country = 1;
		else if (pmessage[0] == '2')
			country = 2;
		else if (pmessage[0] == '3')
			country = 3;
		else if (pmessage[0] == '4')
			country = 4;
		if (pmessage[1] == 'M')//如果是鼠标事件 
		{
			if (pmessage[2] == 'D')//如果是按下鼠标
			{
				//解析鼠标点击的坐标
				int X = (pmessage[3] - '0') * 1000 + (pmessage[4] - '0') * 100 + (pmessage[5] - '0') * 10 + (pmessage[6] - '0') - vec_mappos[country].x;
				int Y = (pmessage[7] - '0') * 1000 + (pmessage[8] - '0') * 100 + (pmessage[9] - '0') * 10 + (pmessage[10] - '0') - vec_mappos[country].y;
				mouseDownPosition[country].set(X, Y);
				real_mouseDownPosition[country].set(X + vec_mappos[country].x, Y + vec_mappos[country].y);

			}
			else if (pmessage[2] == 'U')//如果是抬起鼠标
			{
				//解析鼠标点击的坐标
				int X = (pmessage[4] - '0') * 1000 + (pmessage[5] - '0') * 100 + (pmessage[6] - '0') * 10 + (pmessage[7] - '0') - vec_mappos[country].x;
				int Y = (pmessage[8] - '0') * 1000 + (pmessage[9] - '0') * 100 + (pmessage[10] - '0') * 10 + (pmessage[11] - '0') - vec_mappos[country].y;
				cocos2d::Vec2 mousePosition(X, Y);
				//解析为瓦片坐标
				auto tile_position = tileCoordFromPosition(Vec2(X, Y));

				if (sqrt(pow((X - mouseDownPosition[country].x), 2) + pow((Y - mouseDownPosition[country].y), 2)) <= 5)//如果按下和松开几乎是在同一个位置
				{
					if (pmessage[3] == 'L')//如果点击了左键
					{
						bui_lable = iftobuild(real_mouseDownPosition[country],country);
						if (iftogo == 1 && bui_lable >= 0)
						{
							vec_buimenu.at(bui_lable)->cretimer(my_country, country);
						}
						else
						{
							sol_lable = ifbuildsol(real_mouseDownPosition[country], country);
							if (iftogo == 1 && sol_lable >= 0)
							{
								if (vec_solmenu.at(sol_lable)->num_tobuild[ country ]== 1)
									vec_solmenu.at(sol_lable)->cretimer(my_country, country);
							}
							else
							{
								bool is_click_on_a_friend = 0;
								//判断左键是否点上了小兵
								for (int i = 0; vec_people[country].size() != 0 && i <= vec_people[country].size() - 1; i++)
								{
									cocos2d::Rect rect = vec_people[country][i]->getBoundingBox();
									if (rect.containsPoint(mouseDownPosition[country]))//如果点上了
									{
										if (my_country == country)//如果该鼠标操作是本人的，则换图标
										{
											for (int i = 0; vec_people[country].size() != 0 && i <= vec_people[country].size() - 1; i++)
												vec_people[country][i]->setTexture(vec_people[country][i]->texture_normal);
											for (int i = 0; vec_dog[country].size() != 0 && i <= vec_dog[country].size() - 1; i++)
												vec_dog[country][i]->setTexture(vec_dog[country][i]->texture_normal);
											for (int i = 0; vec_tank[country].size() != 0 && i <= vec_tank[country].size() - 1; i++)
												vec_tank[country][i]->setTexture(vec_tank[country][i]->texture_normal);
											vec_people[country][i]->setTexture(vec_people[country][i]->texture_chosed);
										}
										vec_chosed_soldier[country].clear();
										vec_chosed_soldier[country].push_back(vec_people[country][i]);
										is_click_on_a_friend = 1;
									}
									if (is_click_on_a_friend == 1)
										break;
								}
								if (is_click_on_a_friend == 0)
									for (int i = 0; vec_dog[country].size() != 0 && i <= vec_dog[country].size() - 1; i++)//判断是否点上了狗
									{
										cocos2d::Rect rect = vec_dog[country][i]->getBoundingBox();
										if (rect.containsPoint(mouseDownPosition[country]))//如果点上了
										{
											if (my_country == country)//如果该鼠标操作是本人的，则换图标
											{
												for (int i = 0; vec_people[country].size() != 0 && i <= vec_people[country].size() - 1; i++)
													vec_people[country][i]->setTexture(vec_people[country][i]->texture_normal);
												for (int i = 0; vec_dog[country].size() != 0 && i <= vec_dog[country].size() - 1; i++)
													vec_dog[country][i]->setTexture(vec_dog[country][i]->texture_normal);
												for (int i = 0; vec_tank[country].size() != 0 && i <= vec_tank[country].size() - 1; i++)
													vec_tank[country][i]->setTexture(vec_tank[country][i]->texture_normal);
												vec_dog[country][i]->setTexture(vec_dog[country][i]->texture_chosed);
											}
											vec_chosed_soldier[country].clear();
											vec_chosed_soldier[country].push_back(vec_dog[country][i]);
											is_click_on_a_friend = 1;
										}
										if (is_click_on_a_friend == 1)
											break;
									}
								if (is_click_on_a_friend == 0)
									for (int i = 0; vec_tank[country].size() != 0 && i <= vec_tank[country].size() - 1; i++)//判断是否点上了坦克
									{
										cocos2d::Rect rect = vec_tank[country][i]->getBoundingBox();
										if (rect.containsPoint(mouseDownPosition[country]))//如果点上了
										{
											if (my_country == country)//如果该鼠标操作是本人的，则换图标
											{
												for (int i = 0; vec_people[country].size() != 0 && i <= vec_people[country].size() - 1; i++)
													vec_people[country][i]->setTexture(vec_people[country][i]->texture_normal);
												for (int i = 0; vec_dog[country].size() != 0 && i <= vec_dog[country].size() - 1; i++)
													vec_dog[country][i]->setTexture(vec_dog[country][i]->texture_normal);
												for (int i = 0; vec_tank[country].size() != 0 && i <= vec_tank[country].size() - 1; i++)
													vec_tank[country][i]->setTexture(vec_tank[country][i]->texture_normal);
												vec_tank[country][i]->setTexture(vec_tank[country][i]->texture_chosed);
											}
											vec_chosed_soldier[country].clear();
											vec_chosed_soldier[country].push_back(vec_tank[country][i]);
											is_click_on_a_friend = 1;
										}
										if (is_click_on_a_friend == 1)
											break;
									}
								if (is_click_on_a_friend == 0)
									for (int i = 0; vec_camp[country].size() != 0 && i <= vec_camp[country].size() - 1; i++)//判断是否点上了兵营
									{
										cocos2d::Rect rect = vec_camp[country][i]->getBoundingBox();
										if (rect.containsPoint(mouseDownPosition[country]))//如果点上了
										{
											if (my_country == country)//如果该鼠标操作是本人的，则改变主要建筑图标
											{
												major_camp[country]->setTexture(major_camp[country]->texture_normal);
												vec_camp[country][i]->setTexture(vec_camp[country][i]->texture_major);
											}
											major_camp[country] = vec_camp[country][i];
											is_click_on_a_friend = 1;
										}
										if (is_click_on_a_friend == 1)
											break;
									}
								if (is_click_on_a_friend == 0)
									for (int i = 0; vec_factory[country].size() != 0 && i <= vec_factory[country].size() - 1; i++)//判断是否点上了战车工厂
									{
										cocos2d::Rect rect = vec_factory[country][i]->getBoundingBox();
										if (rect.containsPoint(mouseDownPosition[country]))//如果点上了
										{
											if (my_country == country)//如果该鼠标操作是本人的，则改变主要建筑图标
											{
												major_factory[country]->setTexture(major_factory[country]->texture_normal);
												vec_factory[country][i]->setTexture(vec_factory[country][i]->texture_major);
											}
											major_factory[country] = vec_factory[country][i];
											is_click_on_a_friend = 1;
										}
										if (is_click_on_a_friend == 1)
											break;
									}
							}
						}
					}
					else if (pmessage[3] == 'R')//如果点击了右键
					{
						if (set_building[country] == 0)//如果此时不在放置建筑
						{
							unit* penemy = NULL;
							int type_of_enemy;//0代表小兵，1代表建筑
							for (int j = 1; j <= 4; j++)//判断右键是否点在了其他方小兵上
							{
								for (int i = 0; vec_people[j].size() != 0 && i <= vec_people[j].size() - 1; i++)
									if (j != country) {
										auto rect = vec_people[j][i]->getBoundingBox();
										if (rect.containsPoint(mouseDownPosition[country])) {
											penemy = vec_people[j][i];
											type_of_enemy = 0;
											break;
										}
									}
								if (penemy)
									break;
							}
							if (penemy == NULL)//判断右键是否点在了其他方狗上
								for (int j = 1; j <= 4; j++) {
									for (int i = 0; vec_dog[j].size() != 0 && i <= vec_dog[j].size() - 1; i++)
										if (j != country) {
											auto rect = vec_dog[j][i]->getBoundingBox();
											if (rect.containsPoint(mouseDownPosition[country])) {
												penemy = vec_dog[j][i];
												type_of_enemy = 0;
												break;
											}
										}
									if (penemy)
										break;
								}
							if (penemy == NULL)//判断右键是否点在了其他方坦克上
								for (int j = 1; j <= 4; j++) {
									for (int i = 0; vec_tank[j].size() != 0 && i <= vec_tank[j].size() - 1; i++)
										if (j != country) {
											auto rect = vec_tank[j][i]->getBoundingBox();
											if (rect.containsPoint(mouseDownPosition[country])) {
												penemy = vec_tank[j][i];
												type_of_enemy = 0;
												break;
											}
										}
									if (penemy)
										break;
								}
							if (penemy == NULL)//判断右键是否点在了其他方兵营上
								for (int j = 1; j <= 4; j++) {
									for (int i = 0; vec_camp[j].size() != 0 && i <= vec_camp[j].size() - 1; i++) {
										if (j != country) {
											auto rect = vec_camp[j][i]->getBoundingBox();
											if (rect.containsPoint(mouseDownPosition[country])) {
												penemy = vec_camp[j][i];
												type_of_enemy = 1;
												break;
											}
										}
									}
									if (penemy)
										break;
								}
							if (penemy == NULL)//判断右键是否点在了其他方电厂上
								for (int j = 1; j <= 4; j++) {
									for (int i = 0; vec_electric[j].size() != 0 && i <= vec_electric[j].size() - 1; i++) {
										if (j != country) {
											auto rect = vec_electric[j][i]->getBoundingBox();
											if (rect.containsPoint(mouseDownPosition[country])) {
												penemy = vec_electric[j][i];
												type_of_enemy = 2;
												break;
											}
										}
									}
									if (penemy)
										break;
								}
							if (penemy == NULL)//判断右键是否点在了其他方矿场上
								for (int j = 1; j <= 4; j++) {
									for (int i = 0; vec_mine[j].size() != 0 && i <= vec_mine[j].size() - 1; i++) {
										if (j != country) {
											auto rect = vec_mine[j][i]->getBoundingBox();
											if (rect.containsPoint(mouseDownPosition[country])) {
												penemy = vec_mine[j][i];
												type_of_enemy = 3;
												break;
											}
										}
									}
									if (penemy)
										break;
								}
							if (penemy == NULL)//判断右键是否点在了其他方战车工厂上
								for (int j = 1; j <= 4; j++) {
									for (int i = 0; vec_factory[j].size() != 0 && i <= vec_factory[j].size() - 1; i++) {
										if (j != country) {
											auto rect = vec_factory[j][i]->getBoundingBox();
											if (rect.containsPoint(mouseDownPosition[country])) {
												penemy = vec_factory[j][i];
												type_of_enemy = 4;
												break;
											}
										}
									}
									if (penemy)
										break;
								}
							if (penemy == NULL)//判断右键是否点在了其他方基地上
								for (int j = 1; j <= 4; j++) {
									if (j != country) {
										if (vec_base[j])
										{
											auto rect = vec_base[j]->getBoundingBox();
											if (rect.containsPoint(mouseDownPosition[country])) 
											{
												penemy = vec_base[j];
												type_of_enemy = 5;
												break;
											}
										}
									}
									if (penemy)
										break;
								}
							if (penemy == NULL)//如果点右键时没有点到敌军
							{
								for (int i = 0; vec_chosed_soldier[country].size() != 0 && i <= vec_chosed_soldier[country].size() - 1; i++) {
									vec_chosed_soldier[country][i]->enemy_target = NULL;
									vec_chosed_soldier[country][i]->unschedule(schedule_selector(soldier::updateAttack));
									vec_chosed_soldier[country][i]->move_to(mouseDownPosition[country].x, mouseDownPosition[country].y,0);

								}
							}
							else//如果点右键时点到了敌方
							{
								for (int i = 0; vec_chosed_soldier[country].size() != 0 && i <= vec_chosed_soldier[country].size() - 1; i++)
								{
									vec_chosed_soldier[country][i]->unschedule(schedule_selector(soldier::updateAttack));
									vec_chosed_soldier[country][i]->enemy_target = penemy;
									vec_chosed_soldier[country][i]->move_to(X, Y,type_of_enemy);
								}
							}
						}
						else//如果此时正在放置建筑
						{
							set_building[country] = 0;//取消放置建筑
						}
					}
				}
				else//按下松开不在同一位置（即框选）
				{
					if (pmessage[3] == 'L')//如果点击了左键
					{
						bui_lable = ifbuild(real_mouseDownPosition[country], country);
						if (iftogo == 1 && bui_lable >= 0)//建筑状态
						{
							if (vec_buimenu.at(bui_lable)->ifready[country] == 1)
							{
								bool is_setbuilding_successfully;
								if (vec_buimenu.at(bui_lable)->lable == 0)
									is_setbuilding_successfully=create_a_electric(country, tile_position);
								else if(vec_buimenu.at(bui_lable)->lable == 1)
									is_setbuilding_successfully = create_a_mine(country, tile_position);
								else if (vec_buimenu.at(bui_lable)->lable == 2)
									is_setbuilding_successfully = create_a_camp(country, tile_position);
								else if (vec_buimenu.at(bui_lable)->lable == 3)
									is_setbuilding_successfully = create_a_factory(country, tile_position);
								if (is_setbuilding_successfully) {
									vec_buimenu.at(bui_lable)->build(mousePosition, country);
									addmenu(bui_lable, country);
								}
							}
						}
						else//非建筑状态
						{
							vec_chosed_soldier[country].clear();
							//创建鼠标画的方框rect
							float rectX = mouseDownPosition[country].x < X ? mouseDownPosition[country].x : X;
							float rectY = mouseDownPosition[country].y < Y ? mouseDownPosition[country].y : Y;
							Rect mouseRect(rectX, rectY, abs(mouseDownPosition[country].x - X), abs(mouseDownPosition[country].y - Y));
							//判断框选了哪些小兵
							for (int i = 0; vec_people[country].size() != 0 && i <= vec_people[country].size() - 1; i++) {
								if (vec_people[country][i]->getBoundingBox().intersectsRect(mouseRect)) {
									if (my_country == country)
										vec_people[country][i]->setTexture(vec_people[country][i]->texture_chosed);
									vec_chosed_soldier[country].push_back(vec_people[country][i]);
								}
								else if (my_country == country)
									vec_people[country][i]->setTexture(vec_people[country][i]->texture_normal);
							}
							//判断框选了哪些狗
							for (int i = 0; vec_dog[country].size() != 0 && i <= vec_dog[country].size() - 1; i++) {
								if (vec_dog[country][i]->getBoundingBox().intersectsRect(mouseRect)) {
									if (my_country == country)
										vec_dog[country][i]->setTexture(vec_dog[country][i]->texture_chosed);
									vec_chosed_soldier[country].push_back(vec_dog[country][i]);
								}
								else if (my_country == country)
									vec_dog[country][i]->setTexture(vec_dog[country][i]->texture_normal);
							}
							//判断框选了哪些坦克
							for (int i = 0; vec_tank[country].size() != 0 && i <= vec_tank[country].size() - 1; i++) {
								if (vec_tank[country][i]->getBoundingBox().intersectsRect(mouseRect)) {
									if (my_country == country)
										vec_tank[country][i]->setTexture(vec_tank[country][i]->texture_chosed);
									vec_chosed_soldier[country].push_back(vec_tank[country][i]);
								}
								else if (my_country == country)
									vec_tank[country][i]->setTexture(vec_tank[country][i]->texture_normal);
							}
						}
					}
					else if(pmessage[3] == 'R')//如果点击了右键
					{
						vec_mappos[country] = vec_mappos[country] - mouseDownPosition[country] + mousePosition;          //根据鼠标偏移量改变地图位置

						float mapWidth = _tileMap->getMapSize().width*_tileMap->getTileSize().width;
						float mapHeight = _tileMap->getMapSize().width*_tileMap->getTileSize().height;
						Vec2 winSize = CCDirector::sharedDirector()->getWinSize();//屏幕大小
						cocos2d::Vec2 posmax = Vec2(0, 0);
						cocos2d::Vec2 posmin = Vec2(-mapWidth - 211 + winSize.x, -mapHeight + winSize.y);
						
						//判断是否拖出地图边界
						if (vec_mappos[country].x < posmin.x)
							vec_mappos[country].x = posmin.x;
						if (vec_mappos[country].x > posmax.x)
							vec_mappos[country].x = posmax.x;
						if (vec_mappos[country].y < posmin.y)
							vec_mappos[country].y = posmin.y;
						if (vec_mappos[country].y > posmax.y)
							vec_mappos[country].y = posmax.y;

						if(my_country==country)
							_tileMap->setPosition(vec_mappos[country]);
					}
				}
			}
		}
	}
	if (waiting_to_ask > 4 && imformation::am_i_host)
	{
		waiting_to_ask = 0;
		std::string answer;
		answer += "check ";
		for (int i = 1; i < vec_people.size(); i++)
		{
			for (int j = 0; j < vec_people[i].size(); j++)
			{
				std::string tempt;
				if (j < 10)
				{
					tempt = "0" + std::to_string(j);
				}
				else
				{
					tempt = std::to_string(j);
				}
				answer += "P" + std::to_string(i) + tempt + vec_people[i][j]->report_my_position() + " ";
			}
		}
		for (int i = 1; i < vec_dog.size(); i++)
		{
			for (int j = 0; j < vec_dog[i].size(); j++)
			{
				std::string tempt;
				if (j < 10)
				{
					tempt = "0" + std::to_string(j);
				}
				else
				{
					tempt = std::to_string(j);
				}
				answer += "D" + std::to_string(i) + tempt + vec_dog[i][j]->report_my_position() + " ";
			}
		}
		for (int i = 1; i < vec_tank.size(); i++)
		{
			for (int j = 0; j < vec_tank[i].size(); j++)
			{
				std::string tempt;
				if (j < 10)
				{
					tempt = "0" + std::to_string(j);
				}
				else
				{
					tempt = std::to_string(j);
				}
				answer += "T" + std::to_string(i) + tempt + vec_tank[i][j]->report_my_position() + " ";
			}
		}
		for (int i = 1; i < vec_camp.size(); i++)
		{
			for (int j = 0; j < vec_camp[i].size(); j++)
			{
				std::string tempt;
				if (j < 10)
				{
					tempt = "0" + std::to_string(j);
				}
				else
				{
					tempt = std::to_string(j);
				}
				answer += "C" + std::to_string(i) + tempt + vec_camp[i][j]->report_my_position() + " ";
			}
		}
		for (int i = 1; i < vec_electric.size(); i++)
		{
			for (int j = 0; j < vec_electric[i].size(); j++)
			{
				std::string tempt;
				if (j < 10)
				{
					tempt = "0" + std::to_string(j);
				}
				else
				{
					tempt = std::to_string(j);
				}
				answer += "E" + std::to_string(i) + tempt + vec_electric[i][j]->report_my_position() + " ";
			}
		}
		for (int i = 1; i < vec_factory.size(); i++)
		{
			for (int j = 0; j < vec_factory[i].size(); j++)
			{
				std::string tempt;
				if (j < 10)
				{
					tempt = "0" + std::to_string(j);
				}
				else
				{
					tempt = std::to_string(j);
				}
				answer += "F" + std::to_string(i) + tempt + vec_factory[i][j]->report_my_position() + " ";
			}
		}
		for (int i = 1; i < vec_mine.size(); i++)
		{
			for (int j = 0; j < vec_mine[i].size(); j++)
			{
				std::string tempt;
				if (j < 10)
				{
					tempt = "0" + std::to_string(j);
				}
				else
				{
					tempt = std::to_string(j);
				}
				answer += "M" + std::to_string(i) + tempt + vec_mine[i][j]->report_my_position() + " ";
			}
		}
		for (int i = 1; i < vec_base.size(); i++)
		{
			std::string tempt;
			tempt = std::to_string(i);
			if (vec_base[i])
			{
				answer += "B" + tempt + vec_base[i]->report_my_position() + " ";
			}
		}
		pClient->sendMessage(answer);
	}
	if (message_.find("check ") == 0 && (!imformation::am_i_host))
	{
		std::istringstream in{ message_.substr(sizeof("check ") - 1) };
		std::string tempt;
		while (in >> tempt)
		{
			std::string which_one = tempt.substr(2, 2);
			int which = atoi(which_one.c_str());
			switch (tempt[0])
			{
			case('B'):
			{
				int base_num = atoi(tempt.substr(1, 1).c_str());
				if (base_num < vec_base.size())
				{
					std::string blood_value = tempt.substr(2);
					int blood = atoi(blood_value.c_str());
					vec_base[base_num]->health = blood;
				}

			}break;
			case('P') :
			{
				if (which < vec_people[tempt[1] - '0'].size())
				{
					std::string x_position = tempt.substr(4, 4);
					std::string y_position = tempt.substr(8, 4);
					std::string blood_value = tempt.substr(12);
					int X = atoi(x_position.c_str());
					int Y = atoi(y_position.c_str());
					int blood = atoi(blood_value.c_str());
					vec_people[tempt[1] - '0'][which]->setPosition(Vec2(X, Y));
					vec_people[tempt[1] - '0'][which]->health = blood;
				}
			}break;
			case('D') :
			{

				if (which < vec_dog[tempt[1] - '0'].size())
				{
					std::string x_position = tempt.substr(4, 4);
					std::string y_position = tempt.substr(8, 4);
					std::string blood_value = tempt.substr(12);
					int X = atoi(x_position.c_str());
					int Y = atoi(y_position.c_str());
					int blood = atoi(blood_value.c_str());
					vec_dog[tempt[1] - '0'][which]->setPosition(Vec2(X, Y));
					vec_dog[tempt[1] - '0'][which]->health = blood;
				}

			}break;
			case('T') :
			{
				if (which < vec_tank[tempt[1] - '0'].size())
				{
					std::string x_position = tempt.substr(4, 4);
					std::string y_position = tempt.substr(8, 4);
					std::string blood_value = tempt.substr(12);
					int X = atoi(x_position.c_str());
					int Y = atoi(y_position.c_str());
					int blood = atoi(blood_value.c_str());
					vec_tank[tempt[1] - '0'][which]->setPosition(Vec2(X, Y));
					vec_tank[tempt[1] - '0'][which]->health = blood;
				}

			}break;
			case('C') :
			{
				if (which < vec_camp[tempt[1] - '0'].size())
				{
					std::string blood_value = tempt.substr(4);
					int blood = atoi(blood_value.c_str());
					vec_camp[tempt[1] - '0'][which]->health = blood;
				}

			}break;
			case('E') :
			{
				if (which < vec_electric[tempt[1] - '0'].size())
				{
					std::string blood_value = tempt.substr(4);
					int blood = atoi(blood_value.c_str());
					vec_electric[tempt[1] - '0'][which]->health = blood;
				}

			}break;
			case('F') :
			{
				if (which < vec_factory[tempt[1] - '0'].size())
				{
					std::string blood_value = tempt.substr(4);
					int blood = atoi(blood_value.c_str());
					vec_factory[tempt[1] - '0'][which]->health = blood;
				}
			
			}break;
			case('M') :
			{
				if (which < vec_mine[tempt[1] - '0'].size())
				{
					std::string blood_value = tempt.substr(4);
					int blood = atoi(blood_value.c_str());
					vec_mine[tempt[1] - '0'][which]->health = blood;
				}

			}break;
			default:break;
			}
		}


	}
	if (message_.find("death") == 0)
	{
		std::istringstream in{ message_.substr(sizeof("death ") - 1) };
		std::string tempt;
		while (in >> tempt)
		{
			std::string which_one = tempt.substr(2);
			int which = atoi(which_one.c_str());
			switch (tempt[0])
			{
			case('P') :
			{
				if (which < vec_people[tempt[1] - '0'].size())
				{
					int j = tempt[1] - '0';
					auto iter = vec_people[tempt[1] - '0'].begin();
					for (int i = 0; i < which; i++)
					{
						iter++;
					}
					//auto iter = &vec_people[tempt[1] - '0'][which];
					auto progress = (*iter)->getChildByTag(1);
					//progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);
					auto bar0 = (*iter)->getChildByTag(0);
					bar0->removeFromParent();
					progress->removeFromParent();
					//查找正在攻击该单位的其他国家单位
					for (int j2 = 1; j2 <= 4; j2++)
						if (j2 != tempt[1] - '0')
						{
							for (int i2 = 0; vec_people[j2].size() > 0 && i2 <= vec_people[j2].size() - 1; i2++)
								if (*iter == vec_people[j2][i2]->enemy_target)
									vec_people[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_dog[j2].size() > 0 && i2 <= vec_dog[j2].size() - 1; i2++)
								if (*iter == vec_dog[j2][i2]->enemy_target)
									vec_dog[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_tank[j2].size() > 0 && i2 <= vec_tank[j2].size() - 1; i2++)
								if (*iter == vec_tank[j2][i2]->enemy_target)
									vec_tank[j2][i2]->enemy_target = NULL;
						}

					(*iter)->removeFromParent();
					for (auto iter2 = vec_chosed_soldier[j].begin(); iter2 != vec_chosed_soldier[j].end(); iter2++)
					{
						if (*iter == *iter2) {
							vec_chosed_soldier[j].erase(iter2);
							break;
						}
					}
					vec_people[tempt[1] - '0'].erase(iter);
				}
			}break;
			case('D') :
			{
				if (which < vec_dog[tempt[1] - '0'].size())
				{
					auto iter = vec_dog[tempt[1] - '0'].begin();
					int j = tempt[1] - '0';
					for (int i = 0; i < which; i++)
					{
						iter++;
					}
					auto progress = (*iter)->getChildByTag(1);
					auto bar0 = (*iter)->getChildByTag(0);
					bar0->removeFromParent();
					progress->removeFromParent();
					//查找正在攻击该单位的其他国家单位
					for (int j2 = 1; j2 <= 4; j2++)
						if (j2 != tempt[1] - '0')
						{
							for (int i2 = 0; vec_people[j2].size() > 0 && i2 <= vec_people[j2].size() - 1; i2++)
								if (*iter == vec_people[j2][i2]->enemy_target)
									vec_people[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_dog[j2].size() > 0 && i2 <= vec_dog[j2].size() - 1; i2++)
								if (*iter == vec_dog[j2][i2]->enemy_target)
									vec_dog[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_tank[j2].size() > 0 && i2 <= vec_tank[j2].size() - 1; i2++)
								if (*iter == vec_tank[j2][i2]->enemy_target)
									vec_tank[j2][i2]->enemy_target = NULL;
						}

					(*iter)->removeFromParent();
					for (auto iter2 = vec_chosed_soldier[j].begin(); iter2 != vec_chosed_soldier[j].end(); iter2++)
					{
						if (*iter == *iter2) {
							vec_chosed_soldier[j].erase(iter2);
							break;
						}
					}
					vec_dog[tempt[1] - '0'].erase(iter);
				}
			}break;
			case('T') :
			{
				if (which < vec_tank[tempt[1] - '0'].size())
				{
					auto iter = vec_tank[tempt[1] - '0'].begin();
					int j = tempt[1] - '0';
					for (int i = 0; i < which; i++)
					{
						iter++;
					}
					auto bar0 = (*iter)->getChildByTag(0);
					auto progress = (*iter)->getChildByTag(1);
					bar0->removeFromParent();
					progress->removeFromParent();
					//查找正在攻击该单位的其他国家单位
					for (int j2 = 1; j2 <= 4; j2++)
						if (j2 != tempt[1] - '0')
						{
							for (int i2 = 0; vec_people[j2].size() > 0 && i2 <= vec_people[j2].size() - 1; i2++)
								if (*iter == vec_people[j2][i2]->enemy_target)
									vec_people[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_dog[j2].size() > 0 && i2 <= vec_dog[j2].size() - 1; i2++)
								if (*iter == vec_dog[j2][i2]->enemy_target)
									vec_dog[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_tank[j2].size() > 0 && i2 <= vec_tank[j2].size() - 1; i2++)
								if (*iter == vec_tank[j2][i2]->enemy_target)
									vec_tank[j2][i2]->enemy_target = NULL;
						}

					(*iter)->removeFromParent();
					for (auto iter2 = vec_chosed_soldier[j].begin(); iter2 != vec_chosed_soldier[j].end(); iter2++)
					{
						if (*iter == *iter2) {
							vec_chosed_soldier[j].erase(iter2);
							break;
						}
					}
					vec_tank[tempt[1] - '0'].erase(iter);


				}
			}break;
			case('C') :
			{
				if (which < vec_camp[tempt[1] - '0'].size())
				{
					auto iter = vec_camp[tempt[1] - '0'].begin();
					int j = tempt[1] - '0';
					for (int i = 0; i < which; i++)
					{
						iter++;
					}	
					if (if_there_is_effect)
					{
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("baozha.WAV");
					}
					auto bar0 = (*iter)->getChildByTag(0);
					auto progress = (*iter)->getChildByTag(1);
					bar0->removeFromParent();
					progress->removeFromParent();
					for (int j2 = 1; j2 <= 4; j2++)
						if (j2 != tempt[1] - '0')
						{
							for (int i2 = 0; vec_people[j2].size() > 0 && i2 <= vec_people[j2].size() - 1; i2++)
								if (*iter == vec_people[j2][i2]->enemy_target)
									vec_people[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_dog[j2].size() > 0 && i2 <= vec_dog[j2].size() - 1; i2++)
								if (*iter == vec_dog[j2][i2]->enemy_target)
									vec_dog[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_tank[j2].size() > 0 && i2 <= vec_tank[j2].size() - 1; i2++)
								if (*iter == vec_tank[j2][i2]->enemy_target)
									vec_tank[j2][i2]->enemy_target = NULL;
						}
					//切换主要建筑
					if ((*iter) == major_camp[tempt[1] - '0']) //如果它是主要建筑
					{
						if (vec_camp[j].size() <= 1)//如果只剩这一个建筑了
						{
							major_camp[j] = NULL;
							destruct_last_solbuild(j, 'c');
						}
						else //如果除这个建筑之外还有其他建筑
						{
							//找到第一个其他建筑，将其设为主要建筑
							for (int k = 0; k < vec_camp[j].size(); k++)
								if (vec_camp[j][k] != (*iter)) {
									major_camp[j] = vec_camp[j][k];
									if (my_country == j)
										vec_camp[j][k]->setTexture(vec_camp[j][k]->texture_major);
								}
						}

					}
					elec[j] += vec_solmenu.at(2)->elec;
					if (j == my_country)
					{
						auto elecblood = (ProgressTimer *)this->getChildByName("elecblood");
						elecblood->setPercentage(((float)elec[j] / electotal[j]) * 100);
					}
					(*iter)->change_situation(situation, 1);
					(*iter)->removeFromParent();
					vec_camp[j].erase(iter);
				
				}
			}break;
			case('M') :
			{
				if (which < vec_mine[tempt[1] - '0'].size())
				{
					auto iter = vec_mine[tempt[1] - '0'].begin();
					auto cariter = vec_mine_car[tempt[1] - '0'].begin();
					int j = tempt[1] - '0'; 
					if (if_there_is_effect)
					{
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("baozha.WAV");
					}
					for (int i = 0; i < which; i++)
					{
						iter++;
						cariter++;
					}
					auto bar0 = (*iter)->getChildByTag(0);
					auto progress = (*iter)->getChildByTag(1);
					bar0->removeFromParent();
					progress->removeFromParent();
					if (vec_mine[j].size() == 0)
					{
						if (j == 1)
							this->unschedule(schedule_selector(HelloWorld::updatemoney1));
						else if (j == 2)
							this->unschedule(schedule_selector(HelloWorld::updatemoney2));
						else if (j == 3)
							this->unschedule(schedule_selector(HelloWorld::updatemoney3));
						else if (j == 4)
							this->unschedule(schedule_selector(HelloWorld::updatemoney4));
					}
					(*cariter)->unschedule(schedule_selector(mine_car::updatecar1));
					(*cariter)->removeFromParent();
					vec_mine_car[j].erase(cariter);
					//查找正在攻击该单位的其他国家单位
					for (int j2 = 1; j2 <= 4; j2++)
						if (j2 != j)
						{
							for (int i2 = 0; vec_people[j2].size() > 0 && i2 <= vec_people[j2].size() - 1; i2++)
								if (*iter == vec_people[j2][i2]->enemy_target)
									vec_people[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_dog[j2].size() > 0 && i2 <= vec_dog[j2].size() - 1; i2++)
								if (*iter == vec_dog[j2][i2]->enemy_target)
									vec_dog[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_tank[j2].size() > 0 && i2 <= vec_tank[j2].size() - 1; i2++)
								if (*iter == vec_tank[j2][i2]->enemy_target)
									vec_tank[j2][i2]->enemy_target = NULL;
						}
					elec[j] += vec_solmenu.at(1)->elec;
					if (j == my_country)
					{
						auto elecblood = (ProgressTimer *)this->getChildByName("elecblood");
						elecblood->setPercentage(((float)elec[j] / electotal[j]) * 100);
					}
					(*iter)->change_situation(situation, 1);
					(*iter)->removeFromParent();
					vec_mine[j].erase(iter);

				}
			}break;
			case('E') :
			{
				if (which < vec_electric[tempt[1] - '0'].size())
				{
					auto iter = vec_electric[tempt[1] - '0'].begin();
					int j = tempt[1] - '0';
					for (int i = 0; i < which; i++)
					{
						iter++;
					}
					if (if_there_is_effect)
					{
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("baozha.WAV");
					}
					auto bar0 = (*iter)->getChildByTag(0);
					auto progress = (*iter)->getChildByTag(1);
					bar0->removeFromParent();
					progress->removeFromParent();
					//查找正在攻击该单位的其他国家单位
					for (int j2 = 1; j2 <= 4; j2++)
						if (j2 != j)
						{
							for (int i2 = 0; vec_people[j2].size() > 0 && i2 <= vec_people[j2].size() - 1; i2++)
								if (*iter == vec_people[j2][i2]->enemy_target)
									vec_people[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_dog[j2].size() > 0 && i2 <= vec_dog[j2].size() - 1; i2++)
								if (*iter == vec_dog[j2][i2]->enemy_target)
									vec_dog[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_tank[j2].size() > 0 && i2 <= vec_tank[j2].size() - 1; i2++)
								if (*iter == vec_tank[j2][i2]->enemy_target)
									vec_tank[j2][i2]->enemy_target = NULL;
						}
					elec[j] -= 400;
					electotal[j] -= 400;
					if (j = my_country)
					{
						auto elecblood = (ProgressTimer *)this->getChildByName("elecblood");
						if (elec[j] < 0)
							elecblood->setPercentage((float)0);
						else
							elecblood->setPercentage(((float)elec[j] / electotal[j]) * 100);
					}
					(*iter)->change_situation(situation, 1);
					(*iter)->removeFromParent();
					vec_electric[j].erase(iter);
				}
			}break;
			case('F') :
			{
				if (which < vec_factory[tempt[1] - '0'].size())
				{
					auto iter = vec_factory[tempt[1] - '0'].begin();
					int j = tempt[1] - '0';
					for (int i = 0; i < which; i++)
					{
						iter++;
					}
					if (if_there_is_effect)
					{
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("baozha.WAV");
					}
					auto bar0 = (*iter)->getChildByTag(0);
					auto progress = (*iter)->getChildByTag(1);
					bar0->removeFromParent();
					progress->removeFromParent();
					//查找正在攻击该单位的其他国家单位
					for (int j2 = 1; j2 <= 4; j2++)
						if (j2 != j)
						{
							for (int i2 = 0; vec_people[j2].size() > 0 && i2 <= vec_people[j2].size() - 1; i2++)
								if (*iter == vec_people[j2][i2]->enemy_target)
									vec_people[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_dog[j2].size() > 0 && i2 <= vec_dog[j2].size() - 1; i2++)
								if (*iter == vec_dog[j2][i2]->enemy_target)
									vec_dog[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_tank[j2].size() > 0 && i2 <= vec_tank[j2].size() - 1; i2++)
								if (*iter == vec_tank[j2][i2]->enemy_target)
									vec_tank[j2][i2]->enemy_target = NULL;
						}
					//切换主要建筑
					if ((*iter) == major_factory[j]) //如果它是主要建筑
					{
						if (vec_factory[j].size() <= 1)//如果只剩这一个建筑了
						{
							major_factory[j] = NULL;
							destruct_last_solbuild(j, 'f');
						}
						else //如果除这个建筑之外还有其他建筑
						{
							//找到第一个其他建筑，将其设为主要建筑
							for (int k = 0; k < vec_factory[j].size(); k++)
								if (vec_factory[j][k] != (*iter)) {
									major_factory[j] = vec_factory[j][k];
									if (my_country == j)
										vec_factory[j][k]->setTexture(vec_factory[j][k]->texture_major);
								}
						}

					}
					elec[j] += vec_solmenu.at(3)->elec;
					if (j == my_country)
					{
						auto elecblood = (ProgressTimer *)this->getChildByName("elecblood");
						elecblood->setPercentage(((float)elec[j] / electotal[j]) * 100);
					}
					(*iter)->change_situation(situation, 1);
					(*iter)->removeFromParent();
					vec_factory[j].erase(iter);
				}
			}break;
			case('B'):
			{
				std::string which_base_to_die = tempt.substr(1);
				int which_base = atoi(which_base_to_die.c_str());
				if (which_base < vec_base.size())
				{
					auto iter = vec_base.begin();
					for (int i = 0; i < which_base; i++)
					{
						iter++;
					}
					if (if_there_is_effect)
					{
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("baozha.WAV");
					}
					if (*iter)
					{
						auto bar0 = (*iter)->getChildByTag(0);
						auto progress = (*iter)->getChildByTag(1);
						bar0->removeFromParent();
						progress->removeFromParent();
						//查找正在攻击该单位的其他国家单位
						for (int j2 = 1; j2 <= 4; j2++)
						{
							for (int i2 = 0; vec_people[j2].size() > 0 && i2 <= vec_people[j2].size() - 1; i2++)
								if (*iter == vec_people[j2][i2]->enemy_target)
									vec_people[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_dog[j2].size() > 0 && i2 <= vec_dog[j2].size() - 1; i2++)
								if (*iter == vec_dog[j2][i2]->enemy_target)
									vec_dog[j2][i2]->enemy_target = NULL;
							for (int i2 = 0; vec_tank[j2].size() > 0 && i2 <= vec_tank[j2].size() - 1; i2++)
								if (*iter == vec_tank[j2][i2]->enemy_target)
									vec_tank[j2][i2]->enemy_target = NULL;
						}
						//切换主要建筑

						(*iter)->change_situation(situation, 1);
						(*iter)->removeFromParent();
						*iter = NULL;
						if (which_base == my_country)
						{
							auto help = loseScene::createScene();
							auto reScene = TransitionFade::create(1.0f, help);
							Director::getInstance()->pushScene(reScene);
						}
						
					}
				}
			}; break;
			default:break;
			}
		}
	}
}

int cnt_health_waiting = 0;
void HelloWorld::updateHealth(float dt)//血量监测
{
	//删除没血的people
	std::string death_imfor("death ");
	for (int j = 1; j <= 4; j++)
		for (auto iter = vec_people[j].begin(); iter != vec_people[j].end();)
		{
			auto progress = (ProgressTimer *)(*iter)->getChildByTag(1);
			progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);  //这里是百分制显示

			if ((*iter)->health <= 0)//如果该单位没血了
			{
				if (imformation::am_i_host)
					death_imfor += "P" + std::to_string(j) + std::to_string(iter - vec_people[j].begin()) + " ";
			}
			iter++;
		}
	//删除没血的dog
	for (int j = 1; j <= 4; j++)
		for (auto iter = vec_dog[j].begin(); iter != vec_dog[j].end();)
		{
			auto progress = (ProgressTimer *)(*iter)->getChildByTag(1);
			progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);
			if ((*iter)->health <= 0)//如果该单位没血了
			{
				if (imformation::am_i_host)
					death_imfor += "D" + std::to_string(j) + std::to_string(iter - vec_dog[j].begin()) + " ";
			}
			iter++;
		}
	//删除没血的tank
	for (int j = 1; j <= 4; j++)
		for (auto iter = vec_tank[j].begin(); iter != vec_tank[j].end();)
		{
			auto progress = (ProgressTimer *)(*iter)->getChildByTag(1);
			progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);
			if ((*iter)->health <= 0)//如果该单位没血了
			{
				if (imformation::am_i_host)
					death_imfor += "T" + std::to_string(j) + std::to_string(iter - vec_tank[j].begin()) + " ";
			}
			iter++;
		}
	//删除没血的camp
	for (int j = 1; j <= 4; j++)
		for (auto iter = vec_camp[j].begin(); iter != vec_camp[j].end();)
		{
			auto progress = (ProgressTimer *)(*iter)->getChildByTag(1);
			progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);  //这里是百分制显示
			if ((*iter)->health <= 0)//如果该单位没血了
			{
				if (imformation::am_i_host)
					death_imfor += "C" + std::to_string(j) + std::to_string(iter - vec_camp[j].begin()) + " ";
			}
			iter++;
		}
	//删除没血的mine
	for (int j = 1; j <= 4; j++)
		for (auto iter = vec_mine[j].begin(); iter != vec_mine[j].end();)
		{
			auto progress = (ProgressTimer *)(*iter)->getChildByTag(1);
			progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);  //这里是百分制显示
			if ((*iter)->health <= 0)//如果该单位没血了
			{
				if (imformation::am_i_host)
					death_imfor += "M" + std::to_string(j) + std::to_string(iter - vec_mine[j].begin()) + " ";
			}
			iter++;
		}
	//删除没血的electric
	for (int j = 1; j <= 4; j++)
		for (auto iter = vec_electric[j].begin(); iter != vec_electric[j].end();)
		{
			auto progress = (ProgressTimer *)(*iter)->getChildByTag(1);
			progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);  //这里是百分制显示
			if ((*iter)->health <= 0)//如果该单位没血了
			{
				if (imformation::am_i_host)
					death_imfor += "E" + std::to_string(j) + std::to_string(iter - vec_electric[j].begin()) + " ";
			}
			iter++;
		}
	//删除没血的factory
	for (int j = 1; j <= 4; j++)
		for (auto iter = vec_factory[j].begin(); iter != vec_factory[j].end();)
		{
			auto progress = (ProgressTimer *)(*iter)->getChildByTag(1);
			progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);  //这里是百分制显示
			if ((*iter)->health <= 0)//如果该单位没血了
			{
				if (imformation::am_i_host)
					death_imfor += "F" + std::to_string(j) + std::to_string(iter - vec_factory[j].begin()) + " ";
			}
			iter++;
		}
	//删除没血的base
		for (auto iter = vec_base.begin(); iter != vec_base.end(); iter++) 
		{
			if (*iter)
			{
				if (iter == vec_base.begin())
					continue;
				auto progress = (ProgressTimer *)(*iter)->getChildByTag(1);
				progress->setPercentage((((float)(*iter)->health) / (*iter)->beginhealth) * 100);  //这里是百分制显示
				if ((*iter)->health <= 0)//如果该单位没血了
				{
					if (imformation::am_i_host)
						death_imfor += "B" + std::to_string(0) + std::to_string(iter - vec_base.begin()) + " ";
				}
			}
		}
	cnt_health_waiting++;
	if (death_imfor != "death "&&imformation::am_i_host&&cnt_health_waiting>3)
	{
		pClient->sendMessage(death_imfor);
		cnt_health_waiting = 0;
	}
}

void HelloWorld::createblood(unit* s, int x)//创建血条
{
	cocos2d::Sprite* bar;
	if (x == 1) {
		bar = Sprite::create("bar1.png");
		bar->setPosition(Vec2(10, 30));
	}
	else if(x==2){
		bar = Sprite::create("bar2.png");
		bar->setPosition(Vec2(32, 70));
	}
	else if (x == 3) {
		bar = Sprite::create("bar3.png");
		bar->setPosition(Vec2(48, 70));
	}
	else if (x == 4) {
		bar = Sprite::create("bar3.png");
		bar->setPosition(Vec2(48, 102));
	}
	bar->setTag(0);
	s->addChild(bar);
	cocos2d::Sprite* blood;
	
	if (x == 1) 
		blood = Sprite::create("blood1.png");
	else if (x == 2) 
		blood = Sprite::create("blood2.png");
	else if (x == 3 || x == 4)
		blood = Sprite::create("blood3.png");
	ProgressTimer* bloodbar = ProgressTimer::create(blood);
	bloodbar->setType(ProgressTimer::Type::BAR);        //类型：条状
	if(x==1)
		bloodbar->setPosition(Vec2(10, 30));
	else if(x==2)
		bloodbar->setPosition(Vec2(32, 70));
	else if (x == 3)
		bloodbar->setPosition(Vec2(48, 70));
	else if(x==4)
		bloodbar->setPosition(Vec2(48, 102));
	//从右到左减少血量
	bloodbar->setMidpoint(Point(0, 0.5));     //如果是从左到右的话，改成(1,0.5)即可
	bloodbar->setBarChangeRate(Point(1, 0));
	bloodbar->setTag(1);       //做一个标记
	bloodbar->setPercentage(100);
	s->addChild(bloodbar);
	//schedule(schedule_selector(HelloWorld::scheduleBlood), 0.1f);  //刷新函数，每隔0.1秒
}
void HelloWorld::initmenu()//初始化右侧旁栏
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* bg = Sprite::create("menu/redalert2.0.png");//菜单背景
	bg->setPositionX(origin.x - bg->getContentSize().width / 2 + visibleSize.width);
	bg->setPositionY(origin.y - bg->getContentSize().height / 2 + visibleSize.height);
	this->addChild(bg, 10);

	build_menu* building1 = build_menu::create(0);//第一个建筑的菜单
	building1->setPosition(building1->position);
	this->addChild(building1, 11);
	vec_buimenu.pushBack(building1);
	building1->ifready[4] = -1;
	building1->ifready[1] = -1;
	building1->ifready[2] = -1;
	building1->ifready[3] = -1;

	build_menu* building2 = build_menu::create(1);//第2个建筑的菜单
	building2->setPosition(building2->position);
	this->addChild(building2, 11);
	vec_buimenu.pushBack(building2);

	build_menu* building3 = build_menu::create(2);//第3个建筑的菜单
	building3->setPosition(building3->position);
	this->addChild(building3, 11);
	vec_buimenu.pushBack(building3);

	build_menu* building4 = build_menu::create(3);//第4个建筑的菜单
	building4->setPosition(building4->position);
	this->addChild(building4, 11);
	vec_buimenu.pushBack(building4);

	sol_menu* solmenu1 = sol_menu::create(0);//第一个兵种的菜单
	solmenu1->setPosition(solmenu1->position);
	this->addChild(solmenu1, 11);
	vec_solmenu.pushBack(solmenu1);

	sol_menu* solmenu2 = sol_menu::create(1);//第二个兵种的菜单
	solmenu2->setPosition(solmenu2->position);
	this->addChild(solmenu2, 11);
	vec_solmenu.pushBack(solmenu2);

	sol_menu* solmenu3 = sol_menu::create(2);//第三个兵种的菜单
	solmenu3->setPosition(solmenu3->position);
	this->addChild(solmenu3, 11);
	vec_solmenu.pushBack(solmenu3);


	//电力栏
	auto bar = Sprite::create("menu/elec2.png");
	bar->setPosition(building1->position + Vec2(-50, -170));
	bar->setName("elecbar");
	this->addChild(bar, 12);
	auto blood = Sprite::create("menu/elec1.png");
	ProgressTimer* elecblood = ProgressTimer::create(blood);
	elecblood->setPosition(building1->position + Vec2(-50, -170));
	elecblood->setType(ProgressTimer::Type::BAR);
	//从下到上减少电量
	elecblood->setMidpoint(Point(0, 0));     //如果是从左到右的话，改成(1,0.5)即可
	elecblood->setBarChangeRate(Point(0, 1));
	elecblood->setName("elecblood");       //做一个标记
	elecblood->setPercentage(100);
	this->addChild(elecblood, 13);
	//schedule(schedule_selector(HelloWorld::scheduleBlood), 0.1f);  //刷新函数，每隔0.1秒

	//金钱栏
	auto label1 = Label::createWithSystemFont("money:", "Arial", 20);
	label1->setName("moneylabel");
	label1->setPosition(building1->position + Vec2(0, 136));
	label1->setColor(ccc3(255, 215, 0));
	this->addChild(label1, 13);
	cocos2d::String* num = String::createWithFormat("%d", money[my_country]);
	auto moneynum = Label::createWithSystemFont(num->getCString(), "Arial", 20);
	moneynum->setPosition(building1->position + Vec2(70, 135));
	moneynum->setName("moneynum");
	moneynum->setColor(ccc3(255, 215, 0));
	this->addChild(moneynum, 13);
}
void HelloWorld::addmenu(int lable,int country)//判断是否需要增加菜单栏
{
	int buimenusize=0,solmenusize=0;
	for (int i = 0; i < vec_buimenu.size(); i++)
		if (vec_buimenu.at(i)->ifready[country] >= -1)
			buimenusize++;
	for (int i = 0; i < vec_solmenu.size(); i++)
		if (vec_solmenu.at(i)->ifready[country] >= 1)
			solmenusize++;
	if (lable == buimenusize - 1 && lable < 3)//是否拓展建筑菜单
	{
		/*build_menu* building2 = build_menu::create(lable + 1);
		building2->setPosition(building2->position);
		this->addChild(building2, 12);
		vec_buimenu.pushBack(building2);*/
		vec_buimenu.at(lable+1)->recreate(my_country, country);
	}

	if (lable == 2 && solmenusize <2)//是否拓展兵种菜单
	{
		/*sol_menu* soldier1 = sol_menu::create(0);
		soldier1->setPosition(soldier1->position);
		this->addChild(soldier1, 13);
		vec_solmenu.pushBack(soldier1);

		sol_menu* soldier2 = sol_menu::create(1);
		soldier2->setPosition(soldier2->position);
		this->addChild(soldier2, 14);
		vec_solmenu.pushBack(soldier2);*/
		vec_solmenu.at(0)->recreate(my_country, country);
		vec_solmenu.at(1)->recreate(my_country, country);
	}
	else if (lable == 3 &&solmenusize == 2)
	{
		/*sol_menu* soldier3 = sol_menu::create(2);
		soldier3->setPosition(soldier3->position);
		this->addChild(soldier3, 15);
		vec_solmenu.pushBack(soldier3);*/
		vec_solmenu.at(2)->recreate(my_country, country);
	}
}
int HelloWorld::iftobuild(cocos2d::Vec2 pos,int country)//落下抬起同一点，是否准备建造
{
	iftogo = -1;
	for (int i = 0; vec_buimenu.size() != 0 && i <= vec_buimenu.size() - 1; i++)
	{
		//判断是否点上了
		float rectx1 = vec_buimenu.at(i)->getPosition().x - vec_buimenu.at(i)->getContentSize().width / 2;
		float recty1 = vec_buimenu.at(i)->getPosition().y - vec_buimenu.at(i)->getContentSize().height / 2;
		Rect buirect(rectx1, recty1, vec_buimenu.at(i)->getContentSize().width, vec_buimenu.at(i)->getContentSize().height);
		if (buirect.containsPoint(pos))//如果点上了
		{
			if (vec_buimenu.at(i)->ifready[country] == -1)
			{
				if (money[country] >= vec_buimenu.at(i)->money && elec[country] >= vec_buimenu.at(i)->elec)//判断钱电是否足够
				{
					
						//减少电量和金钱
						money[country] -= vec_buimenu.at(i)->money;
						elec[country] -= vec_buimenu.at(i)->elec;

						if (my_country == country)
						{
							auto elecblood = (ProgressTimer *)this->getChildByName("elecblood");
							elecblood->setPercentage(((float)elec[country] / electotal[country]) * 100);
							auto moneynum = (Label*)this->getChildByName("moneynum");
							cocos2d::String* num = String::createWithFormat("%d", money[country]);
							moneynum->setString(num->getCString());
						}
						//更换状态
						vec_buimenu.at(i)->ifready[country] = 0;
						iftogo = 1;
						return i;
					
				}
			}
			/*else if (vec_buimenu.at(i)->ifready == 1) {
				vec_buimenu.at(bui_lable)->build(mouseDownPosition[country]);
				addmenu(bui_lable,country);
			}*/

		}
	}
	return -1;
}
int HelloWorld::ifbuild(cocos2d::Vec2 pos, int country)//抬起落下不同点，判断是否点中建筑菜单，开始建造
{
	iftogo = -1;
	for (int i = 0; vec_buimenu.size() != 0 && i <= vec_buimenu.size() - 1; i++)
	{
		//判断是否点上了
		float rectx1 = vec_buimenu.at(i)->getPosition().x - vec_buimenu.at(i)->getContentSize().width / 2;
		float recty1 = vec_buimenu.at(i)->getPosition().y - vec_buimenu.at(i)->getContentSize().height / 2;
		Rect buirect(rectx1, recty1, vec_buimenu.at(i)->getContentSize().width, vec_buimenu.at(i)->getContentSize().height);
		
		if (buirect.containsPoint(pos))//如果点上了
		{
			if (vec_buimenu.at(i)->ifready[ country ]== 1)
			{
				iftogo = 1;
				return i;
			}
		}
	}
	return -1;
}
int HelloWorld::ifbuildsol(cocos2d::Vec2 pos, int country) //判断是否点中兵种菜单
{
	iftogo = -1;
	for (int i = 0; vec_solmenu.size() != 0 && i <= vec_solmenu.size() - 1; i++)
	{
		//判断是否点上了
		float rectx1 = vec_solmenu.at(i)->getPosition().x - vec_solmenu.at(i)->getContentSize().width / 2;
		float recty1 = vec_solmenu.at(i)->getPosition().y - vec_solmenu.at(i)->getContentSize().height / 2;
		Rect solrect(rectx1, recty1, vec_solmenu.at(i)->getContentSize().width, vec_solmenu.at(i)->getContentSize().height);
		if (solrect.containsPoint(pos)&& vec_solmenu.at(i)->ifready[country]==1)//如果点上了
		{
			if (money[country] >= vec_solmenu.at(i)->money && elec[country] >= vec_solmenu.at(i)->elec)//判断钱电是否足够
			{
				vec_solmenu.at(i)->num_tobuild[country]++;
				money[country] -= vec_solmenu.at(i)->money;
				//elec[country] -= vec_solmenu.at(i)->elec;
				iftogo = 1;
				if (my_country == country)
				{
					//auto elecblood = (ProgressTimer *)this->getChildByName("elecblood");
					//elecblood->setPercentage(((float)elec[country] / electotal[country]) * 100);
					auto moneynum = (Label*)this->getChildByName("moneynum");
					cocos2d::String* num1 = String::createWithFormat("%d", money[country]);
					moneynum->setString(num1->getCString());
					//切换状态
					
					//产生或者更换标签
					cocos2d::String* numname = String::createWithFormat("soltobuild%d", vec_solmenu.at(i)->lable);
					cocos2d::String* num = String::createWithFormat("%d", vec_solmenu.at(i)->num_tobuild[country]);
					auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
					label1->setPosition(cocos2d::Vec2(10 + vec_solmenu.at(i)->position.x, 10 + vec_solmenu.at(i)->position.y));
					auto label0 = this->getChildByName(numname->getCString());
					if (label0)
					{
						label0->removeFromParent();
					}
					label1->setName(numname->getCString());
					this->addChild(label1, 21);
				}
				return i;
			}
		}
	}
	return -1;
}
bool HelloWorld::create_a_electric(int country, cocos2d::Point tile_position) {
	//创建电厂
	electric* a_building;
	if (country == 1)
		a_building = electric::create("electric1.png");
	else if (country == 2)
		a_building = electric::create("electric2.png");
	else if (country == 3)
		a_building = electric::create("electric3.png");
	else if (country == 4)
		a_building = electric::create("electric4.png");

	//判断是否单位重叠
	bool is_cross = 0;
	//创建建筑物的RECT
	cocos2d::Rect building_rect;
	building_rect = a_building->building_rect_when_click(tile_position.x, tile_position.y);
	//判断是否和小兵重叠
	for (int j = 1; j <= 4; j++)
		for (int i = 0;vec_people[j].size() != 0 && i <= vec_people[j].size() - 1; i++)
			if (building_rect.intersectsRect(vec_people[j][i]->getBoundingBox())) {
				is_cross = 1;
				break;
			}
	//判断是否和狗重叠
	if (is_cross == 0)
		for (int j = 1; j <= 4; j++)
			for (int i = 0; vec_dog[j].size() != 0 && i <= vec_dog[j].size() - 1; i++)
				if (building_rect.intersectsRect(vec_dog[j][i]->getBoundingBox())) {
					is_cross = 1;
					break;
				}
	//判断是否和坦克重叠
	if (is_cross == 0)
		for (int j = 1; j <= 4; j++)
			for (int i = 0; vec_tank[j].size() != 0 && i <= vec_tank[j].size() - 1; i++)
				if (building_rect.intersectsRect(vec_tank[j][i]->getBoundingBox())) {
					is_cross = 1;
					break;
				}
	//判断是否和地形重叠
	if (is_cross == 0 && a_building->is_covered_by_biulding_or_land(tile_position.x, tile_position.y, situation) == true)
		is_cross = 1;
	if (is_cross == 0)//如果都没有重叠，可以放置
	{
		a_building->setPosition(tileToGL(tile_position));
		a_building->set_data(country, tile_position.x, tile_position.y);
		createblood(a_building, 2);
		_tileMap->addChild(a_building, 0);
		if (if_there_is_effect)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/construct.WAV");
		}
		vec_electric[country].push_back(a_building);
		set_building[country] = 0;

		//创建building的wall
		auto a_body = PhysicsBody::createEdgeBox(a_building->getContentSize(), PhysicsMaterial(10, 0, 0), 1.0f);
		a_building->setPhysicsBody(a_body);
		a_building->wall = a_body;

		//更新situation
		a_building->change_situation(situation, 0);
		//增加电量
		electotal[country] += 400;
		elec[country] += 400;
		if (my_country == country)
		{
			auto elecblood = (ProgressTimer *)this->getChildByName("elecblood");
			elecblood->setPercentage(((float)elec[country] / electotal[country]) * 100);
		}
		return true;
	}
	else
		return false;
}
bool HelloWorld::create_a_mine(int country, cocos2d::Point tile_position)
{
	//创建矿厂
	mine* a_building;
	if (country == 1)
		a_building = mine::create("mine1.png");
	else if (country == 2)
		a_building = mine::create("mine2.png");
	else if (country == 3)
		a_building = mine::create("mine3.png");
	else if (country == 4)
		a_building = mine::create("mine4.png");

	//判断是否单位重叠
	bool is_cross = 0;
	//创建建筑物的RECT
	cocos2d::Rect building_rect;
	building_rect = a_building->building_rect_when_click(tile_position.x, tile_position.y);
	//判断是否和小兵重叠
	for (int j = 1; j <= 4; j++)
		for (int i = 0; vec_people[j].size() != 0 && i <= vec_people[j].size() - 1; i++)
			if (building_rect.intersectsRect(vec_people[j][i]->getBoundingBox())) {
				is_cross = 1;
				break;
			}
	//判断是否和狗重叠
	if (is_cross == 0)
		for (int j = 1; j <= 4; j++)
			for (int i = 0; vec_dog[j].size() != 0 && i <= vec_dog[j].size() - 1; i++)
				if (building_rect.intersectsRect(vec_dog[j][i]->getBoundingBox())) {
					is_cross = 1;
					break;
				}
	//判断是否和坦克重叠
	if (is_cross == 0)
		for (int j = 1; j <= 4; j++)
			for (int i = 0; vec_tank[j].size() != 0 && i <= vec_tank[j].size() - 1; i++)
				if (building_rect.intersectsRect(vec_tank[j][i]->getBoundingBox())) {
					is_cross = 1;
					break;
				}
	//判断是否和地形重叠
	if (is_cross == 0 && a_building->is_covered_by_biulding_or_land(tile_position.x, tile_position.y, situation) == true)
		is_cross = 1;
	if (is_cross == 0)//如果都没有重叠，可以放置
	{
		a_building->setPosition(tileToGL(tile_position));
		a_building->set_data(country, tile_position.x, tile_position.y);
		createblood(a_building, 3);
		_tileMap->addChild(a_building, 0);
		if (if_there_is_effect)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/construct.WAV");
		}
		vec_mine[country].push_back(a_building);
		set_building[country] = 0;

		//创建building的wall
		auto a_body = PhysicsBody::createEdgeBox(a_building->getContentSize(), PhysicsMaterial(10, 0, 0), 1.0f);
		a_building->setPhysicsBody(a_body);
		a_building->wall = a_body;

		//创建矿车
		mine_car* car;
		if(country==1)
			car= mine_car::create("mine_car1.png");
		else if (country == 2)
			car = mine_car::create("mine_car2.png");
		else if (country == 3)
			car = mine_car::create("mine_car3.png");
		else if (country == 4)
			car = mine_car::create("mine_car4.png");
		car->init(&HelloWorld::situation);
		car->set_data(country, a_building, vec_mines[country]);
		car->setPosition(a_building->getPosition() + cocos2d::Vec2(0, 30));
		vec_mine_car[country].push_back(car);
		_tileMap->addChild(car, 0);
		car->schedule(schedule_selector(mine_car::updatecar1), 2.2f,CC_REPEAT_FOREVER, 0);
		if (country == 1)
			this->schedule(schedule_selector(HelloWorld::updatemoney1), 2.2f);
		else if (country == 2)
			this->schedule(schedule_selector(HelloWorld::updatemoney2), 2.2f);
		else if (country == 3)
			this->schedule(schedule_selector(HelloWorld::updatemoney3), 2.2f);
		else if (country == 4)
			this->schedule(schedule_selector(HelloWorld::updatemoney4), 2.2f);
		//更新situation
		a_building->change_situation(situation, 0);
		return true;
	}
	else
		return false;
}
bool HelloWorld::create_a_camp(int country, cocos2d::Point tile_position)
{
	//创建兵营
	camp* a_building;
	if (country == 1) {
		if (vec_camp[1].empty() && my_country == country)
			a_building = camp::create("major_camp1.png");
		else
			a_building = camp::create("camp1.png");
	}
	else if (country == 2) {
		if (vec_camp[2].empty() && my_country == country)
			a_building = camp::create("major_camp2.png");
		else
			a_building = camp::create("camp2.png");
	}
	else if (country == 3) {
		if (vec_camp[3].empty() && my_country == country)
			a_building = camp::create("major_camp3.png");
		else
			a_building = camp::create("camp3.png");
	}
	else if (country == 4) {
		if (vec_camp[4].empty() && my_country == country)
			a_building = camp::create("major_camp4.png");
		else
			a_building = camp::create("camp4.png");
	}

	//判断是否单位重叠
	bool is_cross = 0;
	//创建建筑物的RECT
	cocos2d::Rect building_rect;
	building_rect = a_building->building_rect_when_click(tile_position.x, tile_position.y);
	//判断是否和小兵重叠
	for (int j = 1; j <= 4; j++)
		for (int i = 0; vec_people[j].size() != 0 && i <= vec_people[j].size() - 1; i++)
			if (building_rect.intersectsRect(vec_people[j][i]->getBoundingBox())) {
				is_cross = 1;
				break;
			}
	//判断是否和狗重叠
	if (is_cross == 0)
		for (int j = 1; j <= 4; j++)
			for (int i = 0; vec_dog[j].size() != 0 && i <= vec_dog[j].size() - 1; i++)
				if (building_rect.intersectsRect(vec_dog[j][i]->getBoundingBox())) {
					is_cross = 1;
					break;
				}
	//判断是否和坦克重叠
	if (is_cross == 0)
		for (int j = 1; j <= 4; j++)
			for (int i = 0; vec_tank[j].size() != 0 && i <= vec_tank[j].size() - 1; i++)
				if (building_rect.intersectsRect(vec_tank[j][i]->getBoundingBox())) {
					is_cross = 1;
					break;
				}
	//判断是否和地形重叠
	if (is_cross == 0 && a_building->is_covered_by_biulding_or_land(tile_position.x, tile_position.y, situation) == true)
		is_cross = 1;
	if (is_cross == 0)//如果都没有重叠，可以放置
	{
		a_building->setPosition(tileToGL(tile_position));
		a_building->set_data(country, tile_position.x, tile_position.y);
		createblood(a_building, 2);
		_tileMap->addChild(a_building, 0);
		if (if_there_is_effect)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/construct.WAV");
		}
		if (vec_camp[country].empty())
			major_camp[country] = a_building;
		vec_camp[country].push_back(a_building);
		set_building[country] = 0;

		//创建building的wall
		auto a_body = PhysicsBody::createEdgeBox(a_building->getContentSize(), PhysicsMaterial(10, 0, 0), 1.0f);
		a_building->setPhysicsBody(a_body);
		a_building->wall = a_body;

		//更新situation
		a_building->change_situation(situation, 0);
		return true;
	}
	else
		return false;
}
bool HelloWorld::create_a_factory(int country, cocos2d::Point tile_position)
{
	//创建战车工厂
	factory* a_building;
	if (country == 1) {
		if (vec_factory[1].empty())
			a_building = factory::create("major_factory1.png");
		else
			a_building = factory::create("factory1.png");
	}
	else if (country == 2) {
		if (vec_factory[2].empty())
			a_building = factory::create("major_factory2.png");
		else
			a_building = factory::create("factory2.png");
	}
	else if (country == 3) {
		if (vec_factory[3].empty())
			a_building = factory::create("major_factory3.png");
		else
			a_building = factory::create("factory3.png");
	}
	else if (country == 4) {
		if (vec_factory[4].empty())
			a_building = factory::create("major_factory4.png");
		else
			a_building = factory::create("factory4.png");
	}

	//判断是否单位重叠
	bool is_cross = 0;
	//创建建筑物的RECT
	cocos2d::Rect building_rect;
	building_rect = a_building->building_rect_when_click(tile_position.x, tile_position.y);
	//判断是否和小兵重叠
	for (int j = 1; j <= 4; j++)
		for (int i = 0; vec_people[j].size() != 0 && i <= vec_people[j].size() - 1; i++)
			if (building_rect.intersectsRect(vec_people[j][i]->getBoundingBox())) {
				is_cross = 1;
				break;
			}
	//判断是否和狗重叠
	if (is_cross == 0)
		for (int j = 1; j <= 4; j++)
			for (int i = 0; vec_dog[j].size() != 0 && i <= vec_dog[j].size() - 1; i++)
				if (building_rect.intersectsRect(vec_dog[j][i]->getBoundingBox())) {
					is_cross = 1;
					break;
				}
	//判断是否和坦克重叠
	if (is_cross == 0)
		for (int j = 1; j <= 4; j++)
			for (int i = 0; vec_tank[j].size() != 0 && i <= vec_tank[j].size() - 1; i++)
				if (building_rect.intersectsRect(vec_tank[j][i]->getBoundingBox())) {
					is_cross = 1;
					break;
				}
	//判断是否和地形重叠
	if (is_cross == 0 && a_building->is_covered_by_biulding_or_land(tile_position.x, tile_position.y, situation) == true)
		is_cross = 1;
	if (is_cross == 0)//如果都没有重叠，可以放置
	{
		a_building->setPosition(tileToGL(tile_position));
		a_building->set_data(country, tile_position.x, tile_position.y);
		createblood(a_building, 3);
		_tileMap->addChild(a_building, 0);
		if (if_there_is_effect)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/construct.WAV");
		}
		if (vec_factory[country].empty())
			major_factory[country] = a_building;
		vec_factory[country].push_back(a_building);
		set_building[country] = 0;

		//创建building的wall
		auto a_body = PhysicsBody::createEdgeBox(a_building->getContentSize(), PhysicsMaterial(10, 0, 0), 1.0f);
		a_building->setPhysicsBody(a_body);
		a_building->wall = a_body;

		//更新situation
		a_building->change_situation(situation, 0);
		return true;
	}
	else
		return false;
}
void HelloWorld::destruct_last_solbuild(int country,char c_or_f)
{
	if (c_or_f == 'c')
	{
		cocos2d::String* shadename = String::createWithFormat("shade%d%d", vec_solmenu.at(0)->lable + 10, country);
		auto shade = (ProgressTimer *)this->getChildByName(shadename->getCString());
		cocos2d::String* numname = String::createWithFormat("soltobuild%d", vec_solmenu.at(0)->lable);
		auto label0 = this->getChildByName(numname->getCString());
		if (shade)
		{
			if(country==1)
			vec_solmenu.at(0)->unschedule(schedule_selector(sol_menu::updateload1));
			else if (country == 2)
				vec_solmenu.at(0)->unschedule(schedule_selector(sol_menu::updateload2));
			else if (country == 3)
				vec_solmenu.at(0)->unschedule(schedule_selector(sol_menu::updateload3));
			else if (country == 4)
				vec_solmenu.at(0)->unschedule(schedule_selector(sol_menu::updateload4));
			shade->removeFromParent();
		}
		if (label0)
		{
			label0->removeFromParent();
		}
		vec_solmenu.at(0)->ifready[country] = 0;
		vec_solmenu.at(0)->num_tobuild[country] = 0;
		cocos2d::String* shadename1 = String::createWithFormat("shade%d%d", vec_solmenu.at(1)->lable + 10, country);
		auto shade1 = (ProgressTimer *)this->getChildByName(shadename1->getCString());
		cocos2d::String* numname1 = String::createWithFormat("soltobuild%d", vec_solmenu.at(1)->lable);
		auto label1 = this->getChildByName(numname1->getCString());
		if (shade1)
		{
			if (country == 1)
				vec_solmenu.at(1)->unschedule(schedule_selector(sol_menu::updateload1));
			else if (country == 2)
				vec_solmenu.at(1)->unschedule(schedule_selector(sol_menu::updateload2));
			else if (country == 3)
				vec_solmenu.at(1)->unschedule(schedule_selector(sol_menu::updateload3));
			else if (country == 4)
				vec_solmenu.at(1)->unschedule(schedule_selector(sol_menu::updateload4));
			shade1->removeFromParent();
		}
		if (label1)
		{
			label1->removeFromParent();
		}
		vec_solmenu.at(1)->ifready[country] = 0;
		vec_solmenu.at(1)->num_tobuild[country] = 0;
	}
	else if (c_or_f == 'f')
	{
		cocos2d::String* shadename = String::createWithFormat("shade%d%d", vec_solmenu.at(2)->lable + 10, country);
		auto shade = (ProgressTimer *)this->getChildByName(shadename->getCString());
		cocos2d::String* numname = String::createWithFormat("soltobuild%d", vec_solmenu.at(2)->lable);
		auto label0 = this->getChildByName(numname->getCString());
		if (shade)
		{
			if (country == 1)
				vec_solmenu.at(2)->unschedule(schedule_selector(sol_menu::updateload1));
			else if (country == 2)
				vec_solmenu.at(2)->unschedule(schedule_selector(sol_menu::updateload2));
			else if (country == 3)
				vec_solmenu.at(2)->unschedule(schedule_selector(sol_menu::updateload3));
			else if (country == 4)
				vec_solmenu.at(2)->unschedule(schedule_selector(sol_menu::updateload4));
			shade->removeFromParent();
		}
		if (label0)
		{
			label0->removeFromParent();
		}
		vec_solmenu.at(2)->ifready[country] = 0;
		vec_solmenu.at(2)->num_tobuild[country] = 0;
	}
	if (my_country == country)
	{
		cocos2d::String* picture = String::createWithFormat("menu/shade.png");
		CCTexture2D* texture1 = CCTextureCache::sharedTextureCache()->addImage(picture->getCString());
		if (c_or_f == 'c')
		{
			vec_solmenu.at(0)->setTexture(texture1);
			vec_solmenu.at(1)->setTexture(texture1);
		}
		if (c_or_f == 'f')
		{
			vec_solmenu.at(2)->setTexture(texture1);
		}
	}
		

}
void HelloWorld::updatemoney1(float dt)
{
	for (int i = 0; i < vec_mine_car[1].size(); i++)
	{
		if (vec_mine_car[1][i]->if_add == 2)
		{
			money[1] += 50;
			vec_mine_car[1][i]->if_add = 0;
		}
	}
	if (my_country == 1)
	{
		auto moneynum = (Label*)this->getChildByName("moneynum");
		cocos2d::String* num1 = String::createWithFormat("%d", money[1]);
		moneynum->setString(num1->getCString());
	}
}
void HelloWorld::updatemoney2(float dt)
{
	for (int i = 0; i < vec_mine_car[2].size(); i++)
	{
		if (vec_mine_car[2][i]->if_add == 2)
		{
			money[2] += 50;
			vec_mine_car[2][i]->if_add = 0;
		}
	}
	if (my_country == 2)
	{
		auto moneynum = (Label*)this->getChildByName("moneynum");
		cocos2d::String* num1 = String::createWithFormat("%d", money[2]);
		moneynum->setString(num1->getCString());
	}
}
void HelloWorld::updatemoney3(float dt)
{
	for (int i = 0; i < vec_mine_car[3].size(); i++)
	{
		if (vec_mine_car[3][i]->if_add == 2)
		{
			money[3] += 50;
			vec_mine_car[3][i]->if_add = 0;
		}
	}
	if (my_country == 3)
	{
		auto moneynum = (Label*)this->getChildByName("moneynum");
		cocos2d::String* num1 = String::createWithFormat("%d", money[3]);
		moneynum->setString(num1->getCString());
	}
}
void HelloWorld::updatemoney4(float dt)
{
	for (int i = 0; i < vec_mine_car[4].size(); i++)
	{
		if (vec_mine_car[4][i]->if_add == 2)
		{
			money[4] += 50;
			vec_mine_car[4][i]->if_add = 0;
		}
	}
	if (my_country == 4)
	{
		auto moneynum = (Label*)this->getChildByName("moneynum");
		cocos2d::String* num1 = String::createWithFormat("%d", money[4]);
		moneynum->setString(num1->getCString());
	}
}
cocos2d::Vec2 move_to_GLposition(unit* penemy, int enemy_type)
{
	if (enemy_type == 0)
		return penemy->getPosition();
	else if (enemy_type == 1 || enemy_type == 2)
	{
		cocos2d::Point building_tile_position = tileCoordFromPosition(penemy->getPosition());
		for (int i = building_tile_position.x; i <= building_tile_position.x + 1; i++) {
			int j = imformation::XMAX*(building_tile_position.y - 1);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y - 1));
		}
		for (int i = building_tile_position.x - 1; i != building_tile_position.x + 2; i += 3) {
			int j = imformation::XMAX*building_tile_position.y;
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y));
		}
		for (int i = building_tile_position.x - 1; i != building_tile_position.x + 2; i += 3) {
			int j = imformation::XMAX*(building_tile_position.y + 1);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y + 1));
		}
		for (int i = building_tile_position.x; i <= building_tile_position.x + 1; i++) {
			int j = imformation::XMAX*(building_tile_position.y + 2);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y + 2));
		}
	}
	else if (enemy_type == 3 || enemy_type == 4)
	{
		cocos2d::Point building_tile_position = tileCoordFromPosition(penemy->getPosition());
		for (int i = building_tile_position.x - 1; i <= building_tile_position.x + 1; i++) {
			int j = imformation::XMAX*(building_tile_position.y - 1);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y - 1));
		}
		for (int i = building_tile_position.x - 2; i != building_tile_position.x + 2; i += 4) {
			int j = imformation::XMAX*building_tile_position.y;
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y));
		}
		for (int i = building_tile_position.x - 2; i != building_tile_position.x + 2; i += 4) {
			int j = imformation::XMAX*(building_tile_position.y + 1);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y + 1));
		}
		for (int i = building_tile_position.x - 1; i <= building_tile_position.x + 1; i++) {
			int j = imformation::XMAX*(building_tile_position.y + 2);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y + 2));
		}
	}
	else if (enemy_type == 5)
	{
		cocos2d::Point building_tile_position = tileCoordFromPosition(penemy->getPosition());
		for (int i = building_tile_position.x - 1; i <= building_tile_position.x + 1; i++) {
			int j = imformation::XMAX*(building_tile_position.y - 2);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y - 2));
		}
		for (int i = building_tile_position.x - 2; i != building_tile_position.x + 2; i += 4) {
			int j = imformation::XMAX*(building_tile_position.y-1);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y-1));
		}
		for (int i = building_tile_position.x - 2; i != building_tile_position.x + 2; i += 4) {
			int j = imformation::XMAX*(building_tile_position.y);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y));
		}
		for (int i = building_tile_position.x - 2; i != building_tile_position.x + 2; i += 4) {
			int j = imformation::XMAX*(building_tile_position.y + 1);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y + 1));
		}
		for (int i = building_tile_position.x - 1; i <= building_tile_position.x + 1; i++) {
			int j = imformation::XMAX*(building_tile_position.y + 2);
			if (HelloWorld::situation[i + j] == 1)
				return tileToGL(cocos2d::Point(i, building_tile_position.y + 2));
		}
	}
}
void soldier::move_to(int x, int y, int type_of_ene)
{
	this->stopActionByTag(1);///先停止这句  auto moveSequence = cocos2d::Sequence::create(moveAction, moveCallback, nullptr);
							 //moveSequence->setTag(1);  这句在popStepAndAnimate里面
	auto to = tileCoordForPosition(Vec2(x, y));//换成瓦片坐标
	if (enemy_target)
	{
		type_of_enemy = type_of_ene;
		to = tileCoordForPosition(move_to_GLposition(enemy_target, type_of_ene));
	}
	int result = SetPos(to.x, to.y);///招路

	if (result == micropather::MicroPather::NO_SOLUTION)
	{
		log("NO way");//没找到
	}
	else
	{
		popStepAndAnimate();//找到了
	}
}
int waiting = 0;
void soldier::popStepAndAnimate()
{
	cocos2d::Point currentPosition = tileCoordForPosition(this->getPosition());
	if (enemy_target)
	{
		auto enemy_rect = enemy_target->getBoundingBox();
		if (enemy_rect.intersectsCircle(this->getPosition(), this->attack_distance))
		{
			if (this->my_country != this->enemy_target->my_country) {
				this->stopAllActions();
				this->schedule(schedule_selector(soldier::updateAttack), attack_speed, kRepeatForever, 0.2f);
			}
		}
	}
	if (path.size() == 0)
	{
		return;
	}

	if (enemy_target && (waiting>5))
	{
		path.clear();
		cocos2d::Point now_in_GL = this->getPosition();//现在的GL坐标
		cocos2d::Point now_in_tile = tileCoordForPosition(now_in_GL);//现在的瓦片坐标
		playerX = now_in_tile.x;//更新位置
		playerY = now_in_tile.y;
		auto tempt = tileCoordForPosition(move_to_GLposition(enemy_target, type_of_enemy));
		if (Passable(tempt.x, tempt.y))
		{
			float totalCost;
			pather->Reset();///decide later
			pather->Solve(XYToNode(playerX, playerY), XYToNode(tempt.x, tempt.y), &path, &totalCost);///就是这一步在找路
		}
		waiting = 0;
	}
	// 得到下一步移动的步骤
	this->PrintStateInfo(path.at(0));
	int*s = static_cast<int*>(path.at(0));
	int tempt_x;
	int tempt_y;
	NodeToXY(s, &tempt_x, &tempt_y);
	auto diff = cocos2d::Point(tempt_x, tempt_y) - currentPosition;
	// 准备动作和回调
	//assume speed=10;

	double distance = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
	auto moveAction = cocos2d::MoveTo::create(distance / move_speed, positionForTileCoord(cocos2d::Point(tempt_x, tempt_y)));
	cocos2d::Point now_position = this->getPosition();
	//cocos2d::CallFunc* move_to_call_back;
	cocos2d::CallFunc* moveCallback;
	path.erase(path.begin());
	// 运行动作
	cocos2d::Sequence* moveSequence;
	if (enemy_target)
	{
		auto enemy_rect = enemy_target->getBoundingBox();
		if (enemy_rect.intersectsCircle(this->getPosition(),this->attack_distance))
		{
			if (this->my_country != this->enemy_target->my_country) {
				this->stopAllActions();
				this->schedule(schedule_selector(soldier::updateAttack), attack_speed, kRepeatForever, 0.2f);
			}
		}
		else
		{
			moveCallback = cocos2d::CallFunc::create(CC_CALLBACK_0(soldier::popStepAndAnimate, this));
			moveSequence = cocos2d::Sequence::create(moveAction, moveCallback, nullptr);
			moveSequence->setTag(1);
			playerX = tempt_x;
			playerY = tempt_y;
			waiting++;
			this->runAction(moveSequence);
		}
	}
	else
	{
		moveCallback = cocos2d::CallFunc::create(CC_CALLBACK_0(soldier::popStepAndAnimate, this));
		moveSequence = cocos2d::Sequence::create(moveAction, moveCallback, nullptr);
		moveSequence->setTag(1);
		playerX = tempt_x;
		playerY = tempt_y;
		waiting++;
		this->runAction(moveSequence);
	}



}
