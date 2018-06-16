#include <math.h>
#include "menu.h"
#include"HelloWorldScene.h"
USING_NS_CC;

/*int build_menu::iftobuild(cocos2d::Vec2 pos)//落下抬起同一点，是否准备建造
{
	for (int i = 0; vec_buimenu.size() != 0 && i <= vec_buimenu.size() - 1; i++)
	{
		//判断是否点上了
		float rectx1 = vec_buimenu.at(i)->getPosition().x - vec_buimenu.at(i)->getContentSize().width / 2 - mappos.x;
		float recty1 = vec_buimenu.at(i)->getPosition().y - vec_buimenu.at(i)->getContentSize().height / 2 - mappos.y;
		Rect buirect(rectx1, recty1, vec_buimenu.at(i)->getContentSize().width, vec_buimenu.at(i)->getContentSize().height);
		if (buirect.containsPoint(pos))//如果点上了
		{
			if (HelloWorld::money >= build_money[i] && elec >= build_elec[i])//判断钱电是否足够
			{
				if (build_ready[i] == -1)
				{
					money -= build_money[i];
					elec -= build_elec[i];
					log("ok");
					build_ready[i] == 0;
					return i;
				}
			}
		}
	}
	return -1;
}
int build_menu::ifbuild(cocos2d::Vec2 pos)//抬起落下不同点，判断是否点中建筑菜单，开始建造
{
	for (int i = 0; vec_buimenu.size() != 0 && i <= vec_buimenu.size() - 1; i++)
	{
		//判断是否点上了
		float rectx1 = vec_buimenu.at(i)->getPosition().x - vec_buimenu.at(i)->getContentSize().width / 2 - mappos.x;
		float recty1 = vec_buimenu.at(i)->getPosition().y - vec_buimenu.at(i)->getContentSize().height / 2 - mappos.y;
		Rect buirect(rectx1, recty1, vec_buimenu.at(i)->getContentSize().width, vec_buimenu.at(i)->getContentSize().height);
		if (buirect.containsPoint(pos))//如果点上了
		{
			if (build_ready[i] == 1)
				return i;
		}
	}
	return -1;
}*/
build_menu* build_menu::create(int num)//构造函数
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

		cocos2d::String* filename1 = String::createWithFormat("menu/redalert0.%d.png", num);
		
		
		const char* filename = filename1->getCString();
		build_menu* sprite = new build_menu();
	
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
           
            sprite->lable = num;
		    int posy=0, posx = 2- num % 2;
			if (num == 0||num==1)
			{
				posy = 2;
			}
			else if (num == 2)
			{
				posy = 3.5;
			}
			else if (num == 3)
			{
				posy = 3;
			}
			sprite->position =cocos2d::Vec2(origin.x - sprite->getContentSize().width * posx + visibleSize.width + 10, origin.y - sprite->getContentSize().height * posy + visibleSize.height - 5);
			

			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
}
void build_menu::build(cocos2d::Vec2 pos)
{
	if ( ifready == 1)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//建造建筑
		cocos2d::String* filename1 = String::createWithFormat("menu/redalert0.%d.png", lable);
		Sprite* building_1 = Sprite::create(filename1->getCString());
		building_1->setPosition(pos);
		HelloWorld::tilemap->addChild(building_1, lable+10);
		HelloWorld::vec_building.pushBack(building_1);

		//删去“就绪”图片 
		auto layer = this->getParent();
		cocos2d::String* readyname = String::createWithFormat("ready%d", lable);
		auto menu_ready =layer->getChildByName(readyname->getCString());
		menu_ready->removeFromParent();
		ifready = -1;

		//如果是兵营，则需记录位置
		if (lable == 2)//记录兵营位置
		{
			HelloWorld::vec_solbuild.pushBack(building_1);
		}
		else if (lable == 3)
		{
			HelloWorld::vec_carbuild.pushBack(building_1);
		}
	}
}
void build_menu::cretimer()//创建菜单CD条,造兵
{
		Sprite* bg = Sprite::create("menu/shade1.png");
		ProgressTimer* shade = ProgressTimer::create(bg);
		shade->setPosition(position);
		shade->setType(ProgressTimer::Type::RADIAL);
		cocos2d::String* shadename = String::createWithFormat("shade%d", lable);
		shade->setName(shadename->getCString());
		shade->setPercentage(100);
		auto layer = this->getParent();
		layer->addChild(shade, 20);
		ifready = 0;
		this->schedule(schedule_selector(build_menu::updateload), 0.05f);
}
void build_menu::updateload(float dt)//用作建筑刷新
{
	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d", lable);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);
	
	if (shade->getPercentage() == 0)
	{
		this->unschedule(schedule_selector(build_menu::updateload));
		shade->removeFromParent();
		ifready = 1;
		cocos2d::String* filename2 = String::createWithFormat("menu/redalert4.%d.png", lable);
		Sprite* photo_ready = Sprite::create(filename2->getCString());
		cocos2d::String* readyname = String::createWithFormat("ready%d", lable);
		photo_ready->setName(readyname->getCString());
		//this->addChild(photo_ready);
		photo_ready->setPosition(this->position);
		auto layer=this->getParent();
		layer->addChild(photo_ready, 30);
	}
}


/*int sol_menu::ifbuildsol(cocos2d::Vec2 pos) //判断是否点中兵种菜单
{
	for (int i = 0; vec_solmenu.size() != 0 && i <= vec_solmenu.size() - 1; i++)
	{
		//判断是否点上了
		float rectx1 = vec_solmenu.at(i)->getPosition().x - vec_solmenu.at(i)->getContentSize().width / 2 - mappos.x;
		float recty1 = vec_solmenu.at(i)->getPosition().y - vec_solmenu.at(i)->getContentSize().height / 2 - mappos.y;
		Rect solrect(rectx1, recty1, vec_solmenu.at(i)->getContentSize().width, vec_solmenu.at(i)->getContentSize().height);
		if (solrect.containsPoint(pos))//如果点上了
		{
			if (money >= sol_money[i] && elec >= sol_elec[i])//判断钱电是否足够
			{
				sol_tobuild[i]++;
				money -= sol_money[i];
				elec -= sol_elec[i];
				return (i + 10);
			}
		}
	}
	return -1;
}*/
sol_menu* sol_menu::create(int num)//构造函数
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::String* filename1 = String::createWithFormat("menu/redalert3.%d.png", num);
	const char* filename = filename1->getCString();
	sol_menu* sprite = new sol_menu();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();

		sprite->lable = num;
		int kx = 0, ky = 0;
		if (num == 0)
		{
			kx = 2.8, ky = 5;
		}
		else if (num == 1)
		{
			kx = 1.8, ky = 7;
		}
		else if (num == 2)
		{
			kx = 3, ky = 8.1;
		}
		sprite->position = cocos2d::Vec2(origin.x - sprite->getContentSize().width * kx + visibleSize.width + 10, origin.y - sprite->getContentSize().height * ky + visibleSize.height - 5);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void sol_menu::buildsol()
{
	if (num_tobuild > 0)
	{
		if (lable == 0)//大兵
		{
			soldier* a_soldier = soldier::create("CloseNormal.png");
			//物理引擎
			HelloWorld::createblood(a_soldier);

			auto body = PhysicsBody::createCircle(a_soldier->getContentSize().width / 2);
			//body->setContactTestBitmask(0xFFFFFFFF);
			a_soldier->setPhysicsBody(body);

			HelloWorld::vec_soldier.pushBack(a_soldier);
			a_soldier->setPosition(Vec2(HelloWorld::vec_solbuild.at(0)->getPosition().x + HelloWorld::vec_solbuild.at(0)->getContentSize().width + 2, HelloWorld::vec_solbuild.at(0)->getPosition().y + HelloWorld::vec_solbuild.at(0)->getContentSize().height + 2));
			HelloWorld::tilemap->addChild(a_soldier, 0);
		}
		else if (lable == 1)//犬
		{
			soldier* a_soldier = soldier::create("CloseNormal.png");
			//物理引擎
			HelloWorld::createblood(a_soldier);

			auto body = PhysicsBody::createCircle(a_soldier->getContentSize().width / 2);
			//body->setContactTestBitmask(0xFFFFFFFF);
			a_soldier->setPhysicsBody(body);

			HelloWorld::vec_soldier.pushBack(a_soldier);
			a_soldier->setPosition(Vec2(HelloWorld::vec_solbuild.at(0)->getPosition().x + HelloWorld::vec_solbuild.at(0)->getContentSize().width + 2, HelloWorld::vec_solbuild.at(0)->getPosition().y + HelloWorld::vec_solbuild.at(0)->getContentSize().height + 2));
			HelloWorld::tilemap->addChild(a_soldier, 0);
		}
		else if (lable == 2)//坦克
		{
			soldier* a_soldier = soldier::create("CloseNormal.png");
			//物理引擎
			HelloWorld::createblood(a_soldier);

			auto body = PhysicsBody::createCircle(a_soldier->getContentSize().width / 2);
			//body->setContactTestBitmask(0xFFFFFFFF);
			a_soldier->setPhysicsBody(body);

			HelloWorld::vec_soldier.pushBack(a_soldier);
			a_soldier->setPosition(Vec2(HelloWorld::vec_carbuild.at(0)->getPosition().x + HelloWorld::vec_carbuild.at(0)->getContentSize().width + 2, HelloWorld::vec_carbuild.at(0)->getPosition().y +HelloWorld::vec_carbuild.at(0)->getContentSize().height + 2));
			HelloWorld::tilemap->addChild(a_soldier, 0);
		}
		num_tobuild--;
	}
}
void sol_menu::cretimer()//创建菜单CD条,造兵
{
	Sprite* bg = Sprite::create("menu/shade1.png");
	ProgressTimer* shade = ProgressTimer::create(bg);
	shade->setPosition(position);
	shade->setType(ProgressTimer::Type::RADIAL);
	cocos2d::String* shadename = String::createWithFormat("shade%d", lable+10);
	shade->setName(shadename->getCString());
	shade->setPercentage(100);
	auto layer = this->getParent();
	layer->addChild(shade, 20);

	
	this->schedule(schedule_selector(sol_menu::updateload), 0.05f);
}
void sol_menu::updateload(float dt)//用作建筑刷新
{
	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d", lable + 10);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);
	if (shade->getPercentage() == 0)
	{
		
		if (num_tobuild > 1)
		{
			buildsol();
			cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
			cocos2d::String* num = String::createWithFormat("%d", num_tobuild);
			auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
			label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
			auto label0 = layer->getChildByName(numname->getCString());
			if (label0)
			{
				label0->removeFromParent();
			}
			label1->setName(numname->getCString());
			layer->addChild(label1, 21);
			shade->setPercentage(100);
			//this->schedule(schedule_selector(sol_menu::updateload), 0.1f);
		}
		else if (num_tobuild == 1)
		{
			buildsol();
			cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
			cocos2d::String* num = String::createWithFormat("%d", num_tobuild);
			auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
			label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
			auto label0 = layer->getChildByName(numname->getCString());
			if (label0)
			{
				label0->removeFromParent();
			}
			if (num_tobuild == 2)
			{
				label1->setName(numname->getCString());
				layer->addChild(label1, 21);
			}
			//shade->setPercentage(100);
			this->unschedule(schedule_selector(sol_menu::updateload));
			shade->removeFromParent();
		}
	}
}
