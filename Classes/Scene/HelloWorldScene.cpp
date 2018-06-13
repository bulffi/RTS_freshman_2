#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <math.h>
#include"soldier\soldier.h"
#include"buliding\building.h"
#include"cocos2d.h"

cocos2d::Vector<soldier*> HelloWorld::vec_soldier;
cocos2d::Vector<soldier*> HelloWorld::vec_enemy;
cocos2d::Vector<soldier*> HelloWorld::vec_chosed_soldier;
cocos2d::Vector<building*> HelloWorld::vec_building;

USING_NS_CC;

int situation[2500];
Scene* HelloWorld::createScene()
{

	//auto scene = Scene::create();

	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//显示body的边框
	cocos2d::Vect gravity(0, 0.0f);
	scene->getPhysicsWorld()->setGravity(gravity);

	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}
	int i = 1;
	for (int i = 0; i < 2500; i++)
	{
		situation[i] = 1;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//瓦片地图×
	_tilemap = TMXTiledMap::create("map/first.tmx");
	addChild(_tilemap, 0, 100);
	//_collidable = _tilemap->getLayer("collidable");
	//_collidable->setVisible(false);

	//定义世界的边界√
	auto body = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(1000,0,0), 5.0f);
	auto edgeNode = Node::create();
	edgeNode->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);

	//创建菜单按钮√
	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(15);

	MenuItemFont* item1 = MenuItemFont::create("create friend", CC_CALLBACK_1(HelloWorld::menuItem1Callback, this));
	item1->setPositionX(origin.x + item1->getContentSize().width / 2);
	item1->setPositionY(origin.y + item1->getContentSize().height / 2);

	/*MenuItemFont* item2 = MenuItemFont::create("create enemy", CC_CALLBACK_1(HelloWorld::menuItem2Callback, this));
	item2->setPositionX(origin.x - item2->getContentSize().width / 2 + visibleSize.width);
	item2->setPositionY(origin.y + item2->getContentSize().height / 2);*/

	MenuItemFont* item3 = MenuItemFont::create("create building", CC_CALLBACK_1(HelloWorld::menuItem3Callback, this));
	item3->setPositionX(origin.x + visibleSize.width / 2);
	item3->setPositionY(origin.y + item3->getContentSize().height / 2);

	Menu* mn = Menu::create(item1,item3, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	//打开监测血量的调度器√
	this->scheduleUpdate();


	//创建场景本身的监听器√
	auto _mouseListen = EventListenerMouse::create();

	//鼠标按下，记录按下时刻的位置√
	_mouseListen->onMouseDown = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		mouseDownPosition.set(e->getCursorX(), e->getCursorY());
	};
	//鼠标松开
	_mouseListen->onMouseUp = [&](Event* event)
	{
		
		EventMouse* e = (EventMouse*)event;
		//鼠标松开时的位置
		cocos2d::Vec2 mouseUpPosition(e->getCursorX(), e->getCursorY());
		//如果按下和松开几乎是在同一个位置
		if (sqrt(pow((mouseUpPosition.x - mouseDownPosition.x), 2) + pow((mouseUpPosition.y - mouseDownPosition.y), 2)) <= 5)
		{
			if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)//如果点击了左键
			{
				if (set_building == 0)//如果此时不在放置建筑
				{
					bool is_click_on_a_friend = 0;
					for (int i = 0; vec_soldier.size() != 0 && i <= vec_soldier.size() - 1; i++)
					{
						//判断是否点上了
						Rect rect = vec_soldier.at(i)->getBoundingBox();
						if (rect.containsPoint(mouseDownPosition))//如果点上了
						{
							Texture2D* textureNo = Director::getInstance()->getTextureCache()->addImage("CloseSelected.png");
							for (int i = 0; vec_soldier.size() != 0 && i <= vec_soldier.size() - 1; i++)
							{
								vec_soldier.at(i)->setTexture(textureNo);
							}

							Texture2D* textureYes = Director::getInstance()->getTextureCache()->addImage("CloseNormal.png");
							vec_soldier.at(i)->setTexture(textureYes);
							vec_chosed_soldier.clear();
							vec_chosed_soldier.pushBack(vec_soldier.at(i));
							is_click_on_a_friend = 1;
						}
						if (is_click_on_a_friend == 1)
							break;
					}
				}
				else if (set_building == 1)//如果此时正在放置建筑
				{
					//创建建筑物精灵，并设置body
					building* a_building = building::create("building/building.png");
					cocos2d::Size building_size;
					building_size.setSize(a_building->getContentSize().width, a_building->getContentSize().height);
					auto body = PhysicsBody::createBox(building_size);

					//a_building->setPhysicsBody(body);

					//判断是否单位重叠
					bool is_cross = 0;
					//创建建筑物的RECT
					cocos2d::Rect building_rect;
					building_rect.setRect(mouseDownPosition.x - a_building->getContentSize().width / 2, mouseDownPosition.y - a_building->getContentSize().height / 2, a_building->getContentSize().width, a_building->getContentSize().height);
					//判断是否和友军重叠
					for (int i = 0; vec_soldier.size() != 0 && i <= vec_soldier.size() - 1; i++)
					{
						if (building_rect.intersectsRect(vec_soldier.at(i)->getBoundingBox()))
						{
							is_cross = 1;
							break;
						}
					}
					//判断是否和敌军重叠
					if (is_cross == 0)
					{
						for (int i = 0; vec_enemy.size() != 0 && i <= vec_enemy.size() - 1; i++)
						{
							if (building_rect.intersectsRect(vec_enemy.at(i)->getBoundingBox()))
							{
								is_cross = 1;
								break;
							}
						}
					}
					//判断是否和我方建筑重叠
					if (is_cross == 0)
					{
						for (int i = 0; vec_building.size() != 0 && i <= vec_building.size() - 1; i++)
						{
							if (building_rect.intersectsRect(vec_building.at(i)->getBoundingBox()))
							{
								is_cross = 1;
								break;
							}
						}
					}
					if (is_cross == 0)//如果都没有重叠，可以放置
					{
						a_building->setPosition(mouseDownPosition);
						this->addChild(a_building, 0);
						vec_building.pushBack(a_building);
						set_building = 0;

						//创建building的wall
						Size buildingSize;
						buildingSize.setSize(a_building->getContentSize().width*2/5, a_building->getContentSize().height*2/5);
						auto a_body = PhysicsBody::createEdgeBox(buildingSize, PhysicsMaterial(10, 0, 0), 10.0f);
						

						auto a_edgeNode = Node::create();
						a_edgeNode->setPosition(mouseDownPosition);
						a_edgeNode->setPhysicsBody(a_body);
						this->addChild(a_edgeNode);
						a_building->wall = a_edgeNode;

					}
				}
				else
				{
					set_building = 1;
				}
			}
			else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)//如果点击了右键
			{
				if (set_building == 0)//如果此时不在放置建筑
				{
					//判断右键是否点在了敌军上
					int enemy_index = -1;
					for (int i = 0; vec_enemy.size() != 0 && i <= vec_enemy.size() - 1; i++)
					{
						cocos2d::Rect rect = vec_enemy.at(i)->getBoundingBox();
						if (rect.containsPoint(mouseUpPosition))
						{
							enemy_index = i;
							break;
						}
					}
					if (enemy_index < 0)//如果点右键时没有点到敌军
					{
						//移动士兵
						for (int i = 0; vec_chosed_soldier.size() != 0 && i <= vec_chosed_soldier.size() - 1; i++)
						{
						//	vec_chosed_soldier.at(i)->stopAction(vec_chosed_soldier.at(i)->move_action);
						//	vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateMove));
						//	vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateAttack));
						//	double distance = sqrt(pow((e->getCursorX() - vec_chosed_soldier.at(i)->getPositionX()), 2) + pow((e->getCursorY() - vec_chosed_soldier.at(i)->getPositionY()), 2));
						//	vec_chosed_soldier.at(i)->move_action = MoveTo::create(distance / vec_chosed_soldier.at(i)->speed, cocos2d::Vec2(e->getCursorX(), e->getCursorY()));
						//	vec_chosed_soldier.at(i)->runAction(vec_chosed_soldier.at(i)->move_action);
							vec_chosed_soldier.at(i)->move_to(e->getCursorX(), e->getCursorY());

						}
					}
					else//如果点右键时点到了敌方
					{
						//移动士兵直到可以攻击敌方
						for (int i = 0; vec_chosed_soldier.size() != 0 && i <= vec_chosed_soldier.size() - 1; i++)
						{
							vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateMove));
							vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateAttack));

							vec_chosed_soldier.at(i)->enemy_target = vec_enemy.at(enemy_index);
							vec_chosed_soldier.at(i)->schedule(schedule_selector(soldier::updateMove), 0.1f);
						}
					}
				}
				else//如果此时正在放置建筑
				{
					set_building = 0;//取消放置建筑
				}
			}
		}
		else//按下松开不在同一位置（即框选）
		{
			if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)//如果点击了左键
			{
				vec_chosed_soldier.clear();
				for (int i = 0; vec_soldier.size() != 0 && i <= vec_soldier.size() - 1; i++)
				{
					//创建鼠标画的方框rect
					float rectX = mouseDownPosition.x < mouseUpPosition.x ? mouseDownPosition.x : mouseUpPosition.x;
					float rectY = mouseDownPosition.y < mouseUpPosition.y ? mouseDownPosition.y : mouseUpPosition.y;
					Rect mouseRect(rectX, rectY, abs(mouseDownPosition.x - mouseUpPosition.x), abs(mouseDownPosition.y - mouseUpPosition.y));
					//判断框选了哪些
					if (vec_soldier.at(i)->getBoundingBox().intersectsRect(mouseRect))
					{
						Texture2D* textureYes = Director::getInstance()->getTextureCache()->addImage("CloseNormal.png");
						vec_soldier.at(i)->setTexture(textureYes);
						vec_chosed_soldier.pushBack(vec_soldier.at(i));
					}
					else
					{
						Texture2D* textureNo = Director::getInstance()->getTextureCache()->addImage("CloseSelected.png");
						vec_soldier.at(i)->setTexture(textureNo);
					}
				}
			}
		}
	};
	//注册监听器
	auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListen, this);

	

	return true;
}

void HelloWorld::menuItem1Callback(cocos2d::Ref* pSender)//创建一个友方单位
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	soldier* a_soldier = soldier::create("CloseSelected.png",situation);
	a_soldier->init();
	//物理引擎
	auto body = PhysicsBody::createCircle(a_soldier->getContentSize().width / 2, PhysicsMaterial(1000, 0, 0));
	//body->setContactTestBitmask(0xFFFFFFFF);
	a_soldier->setPhysicsBody(body);

	vec_soldier.pushBack(a_soldier);
	a_soldier->setPosition(cocos2d::Vec2(a_soldier->getContentSize().width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(a_soldier, 0);

};
void HelloWorld::menuItem2Callback(cocos2d::Ref* pSender)//创建一个敌方单位
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	soldier* an_enemy = soldier::create("enemy.png",situation);
	vec_enemy.pushBack(an_enemy);
	an_enemy->setPosition(cocos2d::Vec2(visibleSize.width - an_enemy->getContentSize().width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(an_enemy, 0);
	an_enemy->country = 1;

	an_enemy->schedule(schedule_selector(soldier::move_all_time), 0.1f);
}
void HelloWorld::update(float dt)
{
	//删除没血的友方
	for (int i = 0; vec_soldier.size() > 0 && i <= vec_soldier.size() - 1;)
	{
		if (vec_soldier.at(i)->health <= 0)
		{
			vec_soldier.at(i)->removeFromParent();
			vec_soldier.erase(i);
			continue;
		}
		i++;
	}
	//删除没血的敌方
	for (int i = 0; vec_enemy.size() > 0 && i <= vec_enemy.size() - 1;)
	{
		if (vec_enemy.at(i)->health <= 0)//如果该单位没血
		{
			//查找正在攻击该单位的友军
			for (int j = 0; vec_soldier.size() > 0 && j <= vec_soldier.size() - 1; j++)
			{
				if (vec_enemy.at(i) == vec_soldier.at(j)->enemy_target)
				{
					vec_soldier.at(j)->enemy_target = NULL;
				}
			}
			vec_enemy.at(i)->removeFromParent();
			vec_enemy.erase(i);
			continue;
		}
		i++;
	}
}
void HelloWorld::menuItem3Callback(cocos2d::Ref* pSender)//创建一个友方建筑
{
	//先取消所有己方单位的选中状态
	for (int i = 0; vec_soldier.size() != 0 && i <= vec_soldier.size() - 1; i++)
	{
		Texture2D* textureNo = Director::getInstance()->getTextureCache()->addImage("building/building.png");
		vec_soldier.at(i)->setTexture(textureNo);
		vec_chosed_soldier.clear();
	}
	set_building = 2;
	

}