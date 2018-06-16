#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <math.h>
//#include"menu.h"



cocos2d::Vector<soldier*> HelloWorld::vec_soldier;
cocos2d::Vector<soldier*> HelloWorld::vec_enemy;
cocos2d::Vector<soldier*> HelloWorld::vec_chosed_soldier;
cocos2d::Vector<build_menu*> HelloWorld::vec_buimenu;//���潨���˵�������
cocos2d::Vector<sol_menu*> HelloWorld::vec_solmenu;//������ֲ˵�������
cocos2d::Vector<cocos2d::Sprite*>HelloWorld::vec_building;//���潨��������
cocos2d::Vector<cocos2d::Sprite*> HelloWorld::vec_solbuild;//�����Ӫ������
cocos2d::Vector<cocos2d::Sprite*> HelloWorld::vec_carbuild;//����ս������������
cocos2d::TMXTiledMap* HelloWorld::tilemap;


USING_NS_CC;

Scene* HelloWorld::createScene()
{
	
	//auto scene = Scene::create();

	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	cocos2d::Vect gravity(0, 0.0f);
	scene->getPhysicsWorld()->setGravity(gravity);

    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	

	//��������ı߽�
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 5.0f);
	auto edgeNode = Node::create();
	edgeNode->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);

	
	//�����˵���ť
	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(15);
	
	MenuItemFont* item1 = MenuItemFont::create("create friend", CC_CALLBACK_1(HelloWorld::menuItem1Callback, this));
	item1->setPositionX(origin.x + item1->getContentSize().width / 2);
	item1->setPositionY(origin.y + item1->getContentSize().height / 2);

	MenuItemFont* item2 = MenuItemFont::create("create enemy", CC_CALLBACK_1(HelloWorld::menuItem2Callback, this));
	item2->setPositionX(origin.x - item2->getContentSize().width / 2 + visibleSize.width);
	item2->setPositionY(origin.y + item2->getContentSize().height / 2);

	Menu* mn = Menu::create(item1,item2, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn,1);

	
	//������ͼ
	tilemap = TMXTiledMap::create("map/first.tmx");
	this->addChild(tilemap, 0, 100);
	startpos = tilemap->getPosition();
	mappos = startpos;
	
	//��ʼ������
    initmenu();
	//�򿪼��Ѫ���ĵ�����
	this->scheduleUpdate();
	

	//������������ļ�����
	auto _mouseListen = EventListenerMouse::create();
	//��갴�£���¼����ʱ�̵�λ��

	_mouseListen->onMouseDown = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		mouseDownPosition.set(e->getCursorX()-mappos.x, e->getCursorY()-mappos.y);
	};
	//����ɿ�
	_mouseListen->onMouseUp = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		//����ɿ�ʱ��λ��
		HelloWorld::mouseUpPosition.set(e->getCursorX()-mappos.x, e->getCursorY()-mappos.y);
		//������º��ɿ���������ͬһ��λ��
		if (sqrt(pow((mouseUpPosition.x - mouseDownPosition.x), 2) + pow((mouseUpPosition.y - mouseDownPosition.y), 2)) <= 5)
		{
			if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)//�����������
			{
				bui_lable = iftobuild(mouseDownPosition);
				if (iftogo == 1 && bui_lable >= 0)
				{
					vec_buimenu.at(bui_lable)->cretimer();
				}
				else
				{
					sol_lable = ifbuildsol(mouseDownPosition);
					if (iftogo == 1 && sol_lable >= 0)
					{
						if (vec_solmenu.at(sol_lable)->num_tobuild == 1)
							vec_solmenu.at(sol_lable)->cretimer();
					}
					else
					{
						bool is_click_on_a_friend = 0;
						for (int i = 0; vec_soldier.size() != 0 && i <= vec_soldier.size() - 1; i++)
						{
							//�ж��Ƿ������
							Rect rect = vec_soldier.at(i)->getBoundingBox();
							if (rect.containsPoint(mouseDownPosition))//���������
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
				}
			}
			else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)//���������Ҽ�
			{
				//�ж��Ҽ��Ƿ�����˵о���
				int enemy_index = -1;
				for (int i = 0;vec_enemy.size() != 0 && i <= vec_enemy.size() - 1;i++)
				{
					cocos2d::Rect rect = vec_enemy.at(i)->getBoundingBox();
					if (rect.containsPoint(mouseUpPosition))
					{
						enemy_index = i;
						break;
					}
				}
				if (enemy_index < 0)//������Ҽ�ʱû�е㵽�о�
				{
					//�ƶ�ʿ��
					for (int i = 0;vec_chosed_soldier.size() != 0 && i <= vec_chosed_soldier.size() - 1;i++)
					{
						vec_chosed_soldier.at(i)->stopAction(vec_chosed_soldier.at(i)->move_action);
						vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateMove));
						vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateAttack));
						double distance = sqrt(pow((e->getCursorX()-mappos.x - vec_chosed_soldier.at(i)->getPositionX()), 2) + pow((e->getCursorY()-mappos.y - vec_chosed_soldier.at(i)->getPositionY()), 2));
						vec_chosed_soldier.at(i)->move_action = MoveTo::create(distance / vec_chosed_soldier.at(i)->speed, cocos2d::Vec2(e->getCursorX()-mappos.x, -mappos.y+e->getCursorY()));
						vec_chosed_soldier.at(i)->runAction(vec_chosed_soldier.at(i)->move_action);
					}
				}
				else//������Ҽ�ʱ�㵽�˵з�
				{
					//�ƶ�ʿ��ֱ�����Թ����з�
					for (int i = 0;vec_chosed_soldier.size() != 0 && i <= vec_chosed_soldier.size() - 1;i++)
					{
						vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateMove));
						vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateAttack));

						vec_chosed_soldier.at(i)->enemy_target = vec_enemy.at(enemy_index);
						vec_chosed_soldier.at(i)->schedule(schedule_selector(soldier::updateMove),0.1f);
					}
				}
			}
		}
		else//�����ɿ�����ͬһλ�ã�����ѡ��
		{
			if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)//�����������
			{
				bui_lable = ifbuild(mouseDownPosition);
				if (iftogo==1&&bui_lable>=0)//����״̬
				{
					if (vec_buimenu.at(bui_lable)->ifready == 1)
					{
						vec_buimenu.at(bui_lable)->build(mouseUpPosition);
						addmenu(bui_lable);
					}
				}
				else//�ǽ���״̬
				{
					vec_chosed_soldier.clear();
					for (int i = 0; vec_soldier.size() != 0 && i <= vec_soldier.size() - 1; i++)
					{
						//������껭�ķ���rect
						float rectX = mouseDownPosition.x < mouseUpPosition.x ? mouseDownPosition.x : mouseUpPosition.x;
						float rectY = mouseDownPosition.y < mouseUpPosition.y ? mouseDownPosition.y : mouseUpPosition.y;
						Rect mouseRect(rectX, rectY, abs(mouseDownPosition.x - mouseUpPosition.x), abs(mouseDownPosition.y - mouseUpPosition.y));
						//�жϿ�ѡ����Щ
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
					};
				}
			}
			else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)//�����������Ҽ�
			{
				mappos = mappos - mouseDownPosition + mouseUpPosition;          //�������ƫ�����ı��ͼλ��

				float mapWidth = tilemap->getMapSize().width*tilemap->getTileSize().width;
				float mapHeight = tilemap->getMapSize().width*tilemap->getTileSize().height;
				Vec2 winSize = CCDirector::sharedDirector()->getWinSize();//��Ļ��С
				cocos2d::Vec2 posmax = Vec2(winSize.x / 2, winSize.y / 2);
				cocos2d::Vec2 posmin = Vec2(-mapWidth / 2 + winSize.x / 2, -mapHeight / 2 + winSize.y / 2);

				//�ж��Ƿ��ϳ���ͼ�߽�
				if (mappos.x < posmin.x)
					mappos.x = posmin.x;
				if (mappos.x > posmax.x)
					mappos.x = posmax.x;
				if (mappos.y < posmin.y)
					mappos.y = posmin.y;
				if (mappos.y > posmax.y)
					mappos.y = posmax.y;

				tilemap->setPosition(mappos);
			}
		}
	};
	//ע�������
	auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListen, this);

    return true;
}

void HelloWorld::menuItem1Callback(cocos2d::Ref* pSender)//����һ���ѷ���λ
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	soldier* a_soldier = soldier::create("CloseSelected.png");
	//��������
	HelloWorld::createblood(a_soldier);

	auto body = PhysicsBody::createCircle(a_soldier->getContentSize().width / 2);
	//body->setContactTestBitmask(0xFFFFFFFF);
	a_soldier->setPhysicsBody(body);

	vec_soldier.pushBack(a_soldier);
	a_soldier->setPosition(cocos2d::Vec2(a_soldier->getContentSize().width/2 + origin.x, visibleSize.height / 2 + origin.y));
	tilemap->addChild(a_soldier, 0);
	
};
void HelloWorld::menuItem2Callback(cocos2d::Ref* pSender)//����һ���з���λ
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
   
	soldier* an_enemy = soldier::create("enemy.png");
    HelloWorld::createblood(an_enemy);
	vec_enemy.pushBack(an_enemy);
	an_enemy->setPosition(cocos2d::Vec2(visibleSize.width - an_enemy->getContentSize().width/2 + origin.x, visibleSize.height / 2 + origin.y));
	tilemap->addChild(an_enemy, 0);
	an_enemy->country = 1;

	
	an_enemy->schedule(schedule_selector(soldier::move_all_time), 0.1f);
}
void HelloWorld::update(float dt)
{
	//ˢ��Ѫ����ɾ��ûѪ���ѷ�
	for (int i = 0;vec_soldier.size() > 0 && i <= vec_soldier.size() - 1;)
	{
		auto progress = (ProgressTimer *)vec_soldier.at(i)->getChildByTag(1);
		progress->setPercentage((((float)vec_soldier.at(i)->health) / vec_soldier.at(i)->beginhealth) * 100);  //�����ǰٷ�����ʾ

		if (vec_soldier.at(i)->health <= 0)
		{
			auto bar0 = vec_soldier.at(i)->getChildByTag(0);
			bar0->removeFromParent();
			//bar0->erase();
			progress->removeFromParent();
			//progress->erase();
			vec_soldier.at(i)->removeFromParent();
			vec_soldier.erase(i);
			continue;
		}
		i++;
	}
	//ˢ��Ѫ����ɾ��ûѪ�ĵз�
	for (int i = 0;vec_enemy.size() > 0 && i <= vec_enemy.size() - 1;)
	{
		auto progress = (ProgressTimer *)vec_enemy.at(i)->getChildByTag(1);
		progress->setPercentage((((float)vec_enemy.at(i)->health) / vec_enemy.at(i)->beginhealth) * 100);  //�����ǰٷ�����ʾ

		if (vec_enemy.at(i)->health <= 0)//����õ�λûѪ
		{
			//�������ڹ����õ�λ���Ѿ�
			for (int j = 0;vec_soldier.size() > 0 && j <= vec_soldier.size() - 1;j++)
			{
				if (vec_enemy.at(i) == vec_soldier.at(j)->enemy_target)
				{
					vec_soldier.at(j)->enemy_target = NULL;
				}
			}
			auto bar0 = vec_enemy.at(i)->getChildByTag(0);
			bar0->removeFromParent();
			progress->removeFromParent();
			vec_enemy.at(i)->removeFromParent();
			vec_enemy.erase(i);
			continue;
		}
		i++;
	}
}

void HelloWorld::createblood(soldier* s)//����Ѫ��
{
	auto bar = Sprite::create("bar1.png");
	bar->setPosition(Vec2(s->getPosition().x+10, s->getPosition().y + s->getContentSize().height+3));
	bar->setTag(0);
	s->addChild(bar);
	auto blood = Sprite::create("blood1.png");
	//blood->setPosition(Vec2(s->getPosition().x + 10, s->getPosition().y + s->getContentSize().height + 3));
	//s->addChild(blood);
	ProgressTimer* bloodbar = ProgressTimer::create(blood);
	bloodbar->setType(ProgressTimer::Type::BAR);        //���ͣ���״
	bloodbar->setPosition(Vec2(s->getPosition().x+10, s->getPosition().y + s->getContentSize().height+3));
	//���ҵ������Ѫ��
	bloodbar->setMidpoint(Point(0, 0.5));     //����Ǵ����ҵĻ����ĳ�(1,0.5)����
	bloodbar->setBarChangeRate(Point(1, 0));
	bloodbar->setTag(1);       //��һ�����
	bloodbar->setPercentage(100);
	s->addChild(bloodbar);
	//schedule(schedule_selector(HelloWorld::scheduleBlood), 0.1f);  //ˢ�º�����ÿ��0.1��
}

void HelloWorld::initmenu()//��ʼ�����غ��Ҳ�����
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* building_0 = Sprite::create("menu/redalert2.1.png");
	building_0->setPosition(cocos2d::Vec2(50,50));
	tilemap->addChild(building_0, 9);

	Sprite* bg = Sprite::create("menu/redalert2.0.png");
	bg->setPositionX(origin.x - bg->getContentSize().width / 2 + visibleSize.width);
	bg->setPositionY(origin.y - bg->getContentSize().height / 2 + visibleSize.height);
    this->addChild(bg,10);

	build_menu* building1 = build_menu::create(0);
	building1->setPosition(building1->position);
	this->addChild(building1,11);
	vec_buimenu.pushBack(building1);
}
void HelloWorld::addmenu(int lable)//�ж��Ƿ���Ҫ���Ӳ˵���
{
	if (lable == vec_buimenu.size() - 1 && lable < 3)//�Ƿ���չ�����˵�
	{
		build_menu* building2 = build_menu::create(lable + 1);
		building2->setPosition(building2->position);
		this->addChild(building2, 12);
		vec_buimenu.pushBack(building2);
	}

	if (lable == 2 && vec_solmenu.size() == 0)//�Ƿ���չ���ֲ˵�
	{
		sol_menu* soldier1 = sol_menu::create(0);
		soldier1->setPosition(soldier1->position);
		this->addChild(soldier1, 13);
		vec_solmenu.pushBack(soldier1);

		sol_menu* soldier2 = sol_menu::create(1);
		soldier2->setPosition(soldier2->position);
		this->addChild(soldier2, 14);
		vec_solmenu.pushBack(soldier2);
	}
	else if (lable == 3 && vec_solmenu.size() == 2)
	{
		sol_menu* soldier3 = sol_menu::create(2);
		soldier3->setPosition(soldier3->position);
		this->addChild(soldier3, 15);
		vec_solmenu.pushBack(soldier3);
	}
}

int HelloWorld::iftobuild(cocos2d::Vec2 pos)//����̧��ͬһ�㣬�Ƿ�׼������
{
	iftogo = -1;
	for (int i = 0; vec_buimenu.size() != 0 && i <= vec_buimenu.size() - 1; i++)
	{
		//�ж��Ƿ������
		float rectx1 = vec_buimenu.at(i)->getPosition().x - vec_buimenu.at(i)->getContentSize().width / 2-mappos.x;
		float recty1 = vec_buimenu.at(i)->getPosition().y - vec_buimenu.at(i)->getContentSize().height / 2-mappos.y;
		Rect buirect(rectx1, recty1, vec_buimenu.at(i)->getContentSize().width, vec_buimenu.at(i)->getContentSize().height);
			if (buirect.containsPoint(pos))//���������
			{
				if (money >=vec_buimenu.at(i)->money && elec >= vec_buimenu.at(i)->elec)//�ж�Ǯ���Ƿ��㹻
				{
					if (vec_buimenu.at(i)->ifready == -1)
					{
						money -= vec_buimenu.at(i)->money;
						elec -= vec_buimenu.at(i)->elec;
						vec_buimenu.at(i)->ifready= 0;
						iftogo = 1;
						return i;
					}
				}
			}
	}
	return -1;
}
int HelloWorld::ifbuild(cocos2d::Vec2 pos)//̧�����²�ͬ�㣬�ж��Ƿ���н����˵�����ʼ����
{
	iftogo = -1;
	for (int i = 0; vec_buimenu.size() != 0 && i <= vec_buimenu.size() - 1; i++)
	{
		//�ж��Ƿ������
		float rectx1 = vec_buimenu.at(i)->getPosition().x - vec_buimenu.at(i)->getContentSize().width / 2 - mappos.x;
		float recty1 = vec_buimenu.at(i)->getPosition().y - vec_buimenu.at(i)->getContentSize().height / 2 - mappos.y;
		Rect buirect(rectx1, recty1, vec_buimenu.at(i)->getContentSize().width, vec_buimenu.at(i)->getContentSize().height);
		if (buirect.containsPoint(pos))//���������
		{
			if (vec_buimenu.at(i)->ifready == 1)
			{
				iftogo = 1;
				return i;
			}
		}
	}
	return -1;
}
int HelloWorld::ifbuildsol(cocos2d::Vec2 pos) //�ж��Ƿ���б��ֲ˵�
{
	iftogo = -1;
	for (int i = 0; vec_solmenu.size() != 0 && i <= vec_solmenu.size() - 1; i++)
	{
		//�ж��Ƿ������
		float rectx1 = vec_solmenu.at(i)->getPosition().x - vec_solmenu.at(i)->getContentSize().width / 2 - mappos.x;
		float recty1 = vec_solmenu.at(i)->getPosition().y - vec_solmenu.at(i)->getContentSize().height / 2 - mappos.y;
		Rect solrect(rectx1, recty1, vec_solmenu.at(i)->getContentSize().width, vec_solmenu.at(i)->getContentSize().height);
		if (solrect.containsPoint(pos))//���������
		{
			if (money >= vec_solmenu.at(i)->money && elec >= vec_solmenu.at(i)->elec)//�ж�Ǯ���Ƿ��㹻
			{
				vec_solmenu.at(i)->num_tobuild++;
				money -= vec_solmenu.at(i)->money;
				elec -= vec_solmenu.at(i)->elec;
				iftogo = 1;

				cocos2d::String* numname = String::createWithFormat("soltobuild%d", vec_solmenu.at(i)->lable);
				cocos2d::String* num = String::createWithFormat("%d", vec_solmenu.at(i)->num_tobuild);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + vec_solmenu.at(i)->position.x, 10 + vec_solmenu.at(i)->position.y));
				auto label0 = this->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				label1->setName(numname->getCString());
				this->addChild(label1, 21);
				return i;
			}
		}
	}
	return -1;
}

/*void HelloWorld::build(cocos2d::Vec2 pos, int i)
{
	if (i >= 0 && build_ready[i] == 1)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

		int a = i + 2, b = i + 12, c = i + 1;//a�ǵ�һ��������ͼƬ���ţ�bΪ�ڶ���ͼƬ����ʾ���ȼ���cΪ������ͼƬ���ļ���
		int posx = 1 + i % 2, posy = 0;
		//���콨��
		cocos2d::String* filename1 = String::createWithFormat("menu/redalert2.%d.png", a);
		Sprite* building_1 = Sprite::create(filename1->getCString());
		building_1->setPosition(pos);
		tilemap->addChild(building_1, a);
		//ɾȥ��������ͼƬ
		vec_buimenu_ready.at(i)->removeFromParent();
		build_ready[i] = -1;
		//����Ǳ�Ӫ�������¼λ��
		if (i == 2)//��¼��Ӫλ��
		{
			vec_solbuid.pushBack(building_1);
		}
		else if (i == 3)
		{
			vec_carbuid.pushBack(building_1);
		}
		//�Ƿ���Ҫ��չ�˵�
		if (i == 0)
		{
			posy = 2;
		}
		else if (i == 1 || i == 2)
		{
			posy = 3;
		}
		if (i == vec_buimenu.size() - 1 && i < 3)//�Ƿ���չ�����˵�
		{
			cocos2d::String* filename2 = String::createWithFormat("menu/redalert2.%d.png", a + 1);
			Sprite* building2 = Sprite::create(filename2->getCString());
			building2->setPositionX(origin.x - building2->getContentSize().width * posx + visibleSize.width + 10);
			building2->setPositionY(origin.y - building2->getContentSize().height * posy + visibleSize.height - 5);
			this->addChild(building2, b);
			vec_buimenu.pushBack(building2);
		}

		if (i == 2 && vec_solmenu.size() == 0)//�Ƿ���չ���ֲ˵�
		{
			Sprite* soldier1 = Sprite::create("menu/redalert3.1.png");
			soldier1->setPositionX(origin.x - building_1->getContentSize().width * 2 + visibleSize.width + 10);
			soldier1->setPositionY(origin.y - building_1->getContentSize().height * 5 + visibleSize.height - 5);
			this->addChild(soldier1, 13);
			vec_solmenu.pushBack(soldier1);

			Sprite* soldier2 = Sprite::create("menu/redalert3.2.png");
			soldier2->setPositionX(origin.x - building_1->getContentSize().width * 1 + visibleSize.width + 20);
			soldier2->setPositionY(origin.y - building_1->getContentSize().height * 5 + visibleSize.height - 5);
			this->addChild(soldier2, 14);
			vec_solmenu.pushBack(soldier2);
		}
		else if (i == 3 && vec_solmenu.size() == 2)
		{
			Sprite* soldier3 = Sprite::create("menu/redalert3.3.png");
			soldier3->setPositionX(origin.x - building_1->getContentSize().width * 2 + visibleSize.width + 10);
			soldier3->setPositionY(origin.y - building_1->getContentSize().height * 6 + visibleSize.height - 5);
			this->addChild(soldier3, 15);
			vec_solmenu.pushBack(soldier3);
		}
		bui_drag = -1;
	}
}
void HelloWorld::buildsol( int i)
{
	if (i >= 10)
	{
		i = i - 10;

		if (sol_tobuild[i] > 0)
		{
			if (i == 0)//���
			{
				soldier* a_soldier = soldier::create("CloseNormal.png");
				//��������
				HelloWorld::createblood(a_soldier);

				auto body = PhysicsBody::createCircle(a_soldier->getContentSize().width / 2);
				//body->setContactTestBitmask(0xFFFFFFFF);
				a_soldier->setPhysicsBody(body);

				vec_soldier.pushBack(a_soldier);
				a_soldier->setPosition(Vec2(vec_solbuid.at(0)->getPosition().x + vec_solbuid.at(0)->getContentSize().width + 2, vec_solbuid.at(0)->getPosition().y + vec_solbuid.at(0)->getContentSize().height + 2));
				tilemap->addChild(a_soldier, 0);
			}
		}
		else if (i == 1)//Ȯ
		{
			soldier* a_soldier = soldier::create("CloseNormal.png");
			//��������
			HelloWorld::createblood(a_soldier);

			auto body = PhysicsBody::createCircle(a_soldier->getContentSize().width / 2);
			//body->setContactTestBitmask(0xFFFFFFFF);
			a_soldier->setPhysicsBody(body);

			vec_soldier.pushBack(a_soldier);
			a_soldier->setPosition(Vec2(vec_solbuid.at(0)->getPosition().x + vec_solbuid.at(0)->getContentSize().width + 2, vec_solbuid.at(0)->getPosition().y + vec_solbuid.at(0)->getContentSize().height + 2));
			tilemap->addChild(a_soldier, 0);
		}
		else if (i == 2)//̹��
		{
			soldier* a_soldier = soldier::create("CloseNormal.png");
			//��������
			HelloWorld::createblood(a_soldier);

			auto body = PhysicsBody::createCircle(a_soldier->getContentSize().width / 2);
			//body->setContactTestBitmask(0xFFFFFFFF);
			a_soldier->setPhysicsBody(body);

			vec_soldier.pushBack(a_soldier);
			a_soldier->setPosition(Vec2(vec_carbuid.at(0)->getPosition().x + vec_carbuid.at(0)->getContentSize().width + 2, vec_carbuid.at(0)->getPosition().y + vec_carbuid.at(0)->getContentSize().height + 2));
			tilemap->addChild(a_soldier, 0);
		}
	}
}
void HelloWorld::cretimer(int i)//�����˵�CD��,���
{
	if (i >= 0)
	{
		Sprite* bg = Sprite::create("menu/shade1.png");
		ProgressTimer* shade = ProgressTimer::create(bg);
		shade->setPosition(vec_buimenu.at(i)->getPosition());
		shade->setType(ProgressTimer::Type::RADIAL);
		shade->setTag(1);
		if (i < 10)
		{
			shade->setPercentage(100);
			vec_buimenu.at(i)->addChild(shade, 20);
			build_ready[i] = 0;
			this->schedule(schedule_selector(HelloWorld::updateload),  0.1f);
		}
		else
		{
			shade->setPercentage(100);
			vec_solmenu.at(i-10)->addChild(shade, 20);
			this->schedule(schedule_selector(HelloWorld::updateload2), 0.1f);
		}
	}
}
void HelloWorld::updateload(float dt)//��������ˢ��
{
	auto shade= (ProgressTimer *)vec_buimenu.at(bui_drag)->getChildByTag(1);
	shade->setPercentage(shade->getPercentage() - 1);
	if (bui_drag < 10 && bui_drag>=0)
	{
		if (shade->getPercentage() == 0)
		{
			this->unschedule(schedule_selector(HelloWorld::updateload));
			shade->removeFromParent();
			build_ready[bui_drag] = 1;
			cocos2d::String* filename = String::createWithFormat("menu/redalert4.%d.png", bui_drag);
			Sprite* build_menu_ready = Sprite::create(filename->getCString());
			build_menu_ready->setPosition(vec_buimenu.at(bui_drag)->getPosition());
			this->addChild(build_menu_ready, 30);
			int h = vec_buimenu.size();
			log("%d",h);
			log("%d",bui_drag);
			if (vec_buimenu.size() > bui_drag+1)
				vec_buimenu_ready.replace(bui_drag, build_menu_ready);
			else
				vec_buimenu_ready.pushBack(build_menu_ready);
			bui_drag = -1;
		}
	}
}
void HelloWorld::updateload2(float dt)//��������ˢ��
{
	int i = sol_build;
	auto shade = (ProgressTimer *)vec_buimenu.at(i-10)->getChildByTag(1);
	shade->setPercentage(shade->getPercentage() - 1);
	if (i >= 10)
	{
		if (shade->getPercentage() == 0)
		{
			if (sol_tobuild[i - 10] > 0)
			{
				sol_tobuild[i - 10]--;
				buildsol(i);
				shade->setPercentage(100);
				this->schedule(schedule_selector(HelloWorld::updateload2), 0.1f);
			}
			else {
				sol_tobuild[i - 10]--;
				buildsol(i);
				sol_build = -1;
				this->unschedule(schedule_selector(HelloWorld::updateload2));
			}
		}
	}
}*/
