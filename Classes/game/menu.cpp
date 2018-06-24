#include <math.h>
#include "menu.h"
#include"HelloWorldScene.h"
#include"utility\information.h"
USING_NS_CC;

build_menu* build_menu::create(int num)//构造函数
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	cocos2d::String* filename1;
	if (num == 0)
	{
		filename1 = String::createWithFormat("menu/redalert0.%d.png", num);
	}
	else
		  filename1= String::create("menu/shade.png");
		
		
		const char* filename = filename1->getCString();
		build_menu* sprite = new build_menu();
	
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
           
            sprite->lable = num;
		    double posy=0, posx = 0;
			if (num == 0)
			{
				posy = 2.1;
				posx = 1.9;
			}
			else if (num == 1)
			{
				posy = 2.1;
				posx = 0.85;
			}
			else if (num == 2)
			{
				posy = 3.15;
				posx = 1.9;
			}
			else if (num == 3)
			{
				posy = 3.15;
				posx = 0.85;
			}
			sprite->position =cocos2d::Vec2(origin.x - sprite->getContentSize().width * posx + visibleSize.width + 10, origin.y - sprite->getContentSize().height * posy + visibleSize.height - 5);
			

			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
}
void build_menu::recreate(int my_country, int country)
{
	if (my_country == country)
	{
		cocos2d::String* picture = String::createWithFormat("menu/redalert0.%d.png", lable);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(picture->getCString());
		this->setTexture(texture);
	}
	ifready[country] = -1;
}
void build_menu::build(cocos2d::Vec2 pos,int country)
{
	if ( this->ifready[country] == 1)
	{
		//删去“就绪”图片 
		if (mycountry == country)
		{
			auto layer = this->getParent();
			cocos2d::String* readyname = String::createWithFormat("ready%d%d", lable,country);
			auto menu_ready = layer->getChildByName(readyname->getCString());
			menu_ready->removeFromParent();	
		}
        ifready[country] = -1;
	}
}
void build_menu::cretimer(int my_country,int country)//创建菜单CD条,造兵
{
	country_did = country;
	mycountry = my_country;
		Sprite* bg = Sprite::create("menu/shade1.png");
		ProgressTimer* shade = ProgressTimer::create(bg);
		shade->setPosition(position);
		shade->setType(ProgressTimer::Type::RADIAL);
		cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable,country);
		shade->setName(shadename->getCString());
		shade->setPercentage(100);
		auto layer = this->getParent();
		if (my_country == country)
			layer->addChild(shade, 20);
		else
			layer->addChild(shade, 10);
		ifready[country] = 0;
		if(country==1)
		    this->schedule(schedule_selector(build_menu::updateload1), 0.05f);
		else if(country==2)
			this->schedule(schedule_selector(build_menu::updateload2), 0.05f);
		else if (country == 3)
			this->schedule(schedule_selector(build_menu::updateload3), 0.05f);
		else if (country == 4)
			this->schedule(schedule_selector(build_menu::updateload4), 0.05f);
}
void build_menu::updateload1(float dt)//用作建筑刷新
{
	
	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable,1);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);
	
	if (shade->getPercentage() == 0)
	{
		this->unschedule(schedule_selector(build_menu::updateload1));
		shade->removeFromParent();
		ifready[1] = 1;
		if (mycountry == 1)
		{
			cocos2d::String* filename2 = String::createWithFormat("menu/redalert4.%d.png", lable);
			Sprite* photo_ready = Sprite::create(filename2->getCString());
			cocos2d::String* readyname = String::createWithFormat("ready%d%d", lable,1);
			photo_ready->setName(readyname->getCString());
			//this->addChild(photo_ready);
			photo_ready->setPosition(this->position);
			auto layer = this->getParent();	
			layer->addChild(photo_ready, 30);
		}
	}
}
void build_menu::updateload2(float dt)//用作建筑刷新
{
	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable, 2);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);

	if (shade->getPercentage() == 0)
	{
		this->unschedule(schedule_selector(build_menu::updateload2));
		shade->removeFromParent();
		ifready[2] = 1;
		if (mycountry == 2)
		{
			cocos2d::String* filename2 = String::createWithFormat("menu/redalert4.%d.png", lable);
			Sprite* photo_ready = Sprite::create(filename2->getCString());
			cocos2d::String* readyname = String::createWithFormat("ready%d%d", lable,2);
			photo_ready->setName(readyname->getCString());
			//this->addChild(photo_ready);
			photo_ready->setPosition(this->position);
			auto layer = this->getParent();
			layer->addChild(photo_ready, 30);
		}
	}
}
void build_menu::updateload3(float dt)//用作建筑刷新
{
	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable, 3);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);

	if (shade->getPercentage() == 0)
	{
		this->unschedule(schedule_selector(build_menu::updateload3));
		shade->removeFromParent();
		ifready[3] = 1;
		if (mycountry == 3)
		{
			cocos2d::String* filename2 = String::createWithFormat("menu/redalert4.%d.png", lable);
			Sprite* photo_ready = Sprite::create(filename2->getCString());
			cocos2d::String* readyname = String::createWithFormat("ready%d%d", lable,3);
			photo_ready->setName(readyname->getCString());
			//this->addChild(photo_ready);
			photo_ready->setPosition(this->position);
			auto layer = this->getParent();
			layer->addChild(photo_ready, 30);
		}
	}
}
void build_menu::updateload4(float dt)//用作建筑刷新
{
	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable, 4);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);

	if (shade->getPercentage() == 0)
	{
		this->unschedule(schedule_selector(build_menu::updateload4));
		shade->removeFromParent();
		ifready[4] = 1;
		if (mycountry == 4)
		{
			cocos2d::String* filename2 = String::createWithFormat("menu/redalert4.%d.png", lable);
			Sprite* photo_ready = Sprite::create(filename2->getCString());
			cocos2d::String* readyname = String::createWithFormat("ready%d%d", lable,4);
			photo_ready->setName(readyname->getCString());
			//this->addChild(photo_ready);
			photo_ready->setPosition(this->position);
			auto layer = this->getParent();
			layer->addChild(photo_ready, 30);
		}
	}
}

sol_menu* sol_menu::create(int num)//构造函数
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::String* filename1;
	/*if (num == 0)
		filename1 = String::createWithFormat("menu/redalert3.%d.png", num);
	else*/
		filename1 = String::create("menu/shade.png");

	const char* filename = filename1->getCString();
	sol_menu* sprite = new sol_menu();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();

		sprite->lable = num;
		double kx = 0, ky = 0;
		if (num == 0)
		{
			kx = 2.0, ky = 5;
		}
		else if (num == 1)
		{
			kx = 0.9, ky = 5;
		}
		else if (num == 2)
		{
			kx = 2.0, ky = 6.05;
		}
		sprite->position = cocos2d::Vec2(origin.x - sprite->getContentSize().width * kx + visibleSize.width + 10, origin.y - sprite->getContentSize().height * ky + visibleSize.height - 5);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void sol_menu::recreate(int my_country, int country)
{
	if (my_country == country)
	{
		cocos2d::String* picture = String::createWithFormat("menu/redalert0.%d.png", lable);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(picture->getCString());
		this->setTexture(texture);
	}
	ifready[country] = 1;
}
void sol_menu::buildsol(int country)
{
	if (num_tobuild[country] > 0)
	{
		if (lable == 0 && HelloWorld::major_camp[country]!=NULL)//大兵
		{
			people* a_people;
			if (country == 1)
				a_people = people::create("people1.png");
			else if (country == 2)
				a_people = people::create("people2.png");
			else if (country == 3)
				a_people = people::create("people3.png");
			else if (country == 4)
				a_people = people::create("people4.png");
			a_people->init(&HelloWorld::situation);
			a_people->set_data(country);
			a_people->setPosition(cocos2d::Vec2(HelloWorld::major_camp[country]->getPositionX() + 2 * imformation::X_SIZE, HelloWorld::major_camp[country]->getPositionY()));
			HelloWorld::vec_people[country].push_back(a_people);

			//auto body = PhysicsBody::createBox(a_people->getContentSize(), PhysicsMaterial(1000, 0, 0));
			auto body = PhysicsBody::createCircle(15, PhysicsMaterial(1000, 0, 0));
			a_people->setPhysicsBody(body);
			HelloWorld::createblood(a_people, 1);
			auto layer = this->getParent();
			HelloWorld::_tileMap->addChild(a_people, 0);
		}
		else if (lable == 1 && HelloWorld::major_camp[country] != NULL)//犬
		{
			dog* a_dog;
			if (country == 1)
				a_dog = dog::create("dog1.png");
			else if (country == 2)
				a_dog = dog::create("dog2.png");
			else if (country == 3)
				a_dog = dog::create("dog3.png");
			else if (country == 4)
				a_dog = dog::create("dog4.png");
			a_dog->init(&HelloWorld::situation);
			a_dog->set_data(country);
			a_dog->setPosition(cocos2d::Vec2(HelloWorld::major_camp[country]->getPositionX() + 2 * imformation::X_SIZE, HelloWorld::major_camp[country]->getPositionY()));
			HelloWorld::vec_dog[country].push_back(a_dog);

			//auto body = PhysicsBody::createBox(a_people->getContentSize(), PhysicsMaterial(1000, 0, 0));
			auto body = PhysicsBody::createCircle(15, PhysicsMaterial(1000, 0, 0));
			a_dog->setPhysicsBody(body);
			HelloWorld::createblood(a_dog, 1);
			auto layer = this->getParent();
			HelloWorld::_tileMap->addChild(a_dog, 0);
		}
		else if (lable == 2 && HelloWorld::major_factory[country] != NULL)//坦克
		{
			tank* a_tank;
			if (country == 1)
				a_tank = tank::create("tank1.png");
			else if (country == 2)
				a_tank = tank::create("tank2.png");
			else if (country == 3)
				a_tank = tank::create("tank3.png");
			else if (country == 4)
				a_tank = tank::create("tank4.png");
			a_tank->init(&HelloWorld::situation);
			a_tank->set_data(country);
			a_tank->setPosition(cocos2d::Vec2(HelloWorld::major_factory[country]->getPositionX() + 2 * imformation::X_SIZE, HelloWorld::major_factory[country]->getPositionY()));
			HelloWorld::vec_tank[country].push_back(a_tank);

			//auto body = PhysicsBody::createBox(a_people->getContentSize(), PhysicsMaterial(1000, 0, 0));
			auto body = PhysicsBody::createCircle(15, PhysicsMaterial(1000, 0, 0));
			a_tank->setPhysicsBody(body);
			HelloWorld::createblood(a_tank, 1);
			auto layer = this->getParent();
			HelloWorld::_tileMap->addChild(a_tank, 0);
		}
	}
}
void sol_menu::cretimer(int my_country, int country)//创建菜单CD条,造兵
{
	country_did = country;
	mycountry = my_country;
	Sprite* bg = Sprite::create("menu/shade1.png");
	ProgressTimer* shade = ProgressTimer::create(bg);
	shade->setPosition(position);
	shade->setType(ProgressTimer::Type::RADIAL);
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable+10,country);
	shade->setName(shadename->getCString());
	shade->setPercentage(100);
	auto layer = this->getParent();
	if (my_country == country)
		layer->addChild(shade, 20);
	else
		layer->addChild(shade, 10);
	if (country == 1)
		this->schedule(schedule_selector(sol_menu::updateload1), 0.05f);
	else if (country == 2)
		this->schedule(schedule_selector(sol_menu::updateload2), 0.05f);
	else if (country == 3)
		this->schedule(schedule_selector(sol_menu::updateload3), 0.05f);
	else if (country == 4)
		this->schedule(schedule_selector(sol_menu::updateload4), 0.05f);
}
void sol_menu::updateload1(float dt)//用作兵种刷新
{
	
	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable + 10,1);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);
	if (shade->getPercentage() == 0)
	{
			
		if (num_tobuild[1] > 1)
		{
			buildsol(1);//造兵
			num_tobuild[1]--;
			//更换标签
			if (mycountry == 1)
			{
				cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
				cocos2d::String* num = String::createWithFormat("%d", num_tobuild[1]);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
				auto label0 = layer->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				label1->setName(numname->getCString());
				layer->addChild(label1, 21);
			}
				shade->setPercentage(100);
				//this->schedule(schedule_selector(sol_menu::updateload), 0.1f);
			
		}
		else if (num_tobuild[1] ==1)
		{
			buildsol(1);
			num_tobuild[1]--;
			if (mycountry == 1)
			{
				cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
				cocos2d::String* num = String::createWithFormat("%d", num_tobuild[1]);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
				auto label0 = layer->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				/*if (num_tobuild[1] == 2)
				{
					label1->setName(numname->getCString());
					layer->addChild(label1, 21);
				}
				shade->setPercentage(100);*/
			}this->unschedule(schedule_selector(sol_menu::updateload1));
				shade->removeFromParent();
		}
	}
}
void sol_menu::updateload2(float dt)//用作兵种刷新
{

	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable + 10, 2);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);
	if (shade->getPercentage() == 0)
	{

		if (num_tobuild[2] > 1)
		{
			buildsol(2);//造兵
			num_tobuild[2]--;
			//更换标签
			if (mycountry == 2)
			{
				cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
				cocos2d::String* num = String::createWithFormat("%d", num_tobuild[2]);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
				auto label0 = layer->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				label1->setName(numname->getCString());
				layer->addChild(label1, 21);
			}
				shade->setPercentage(100);
				//this->schedule(schedule_selector(sol_menu::updateload), 0.1f);
		}
		else if (num_tobuild[2] == 1)
		{
			buildsol(2);
			num_tobuild[2]--;
			if (mycountry == 2)
			{
				cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
				cocos2d::String* num = String::createWithFormat("%d", num_tobuild[2]);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
				auto label0 = layer->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				/*if (num_tobuild[2] == 2)
				{
				label1->setName(numname->getCString());
				layer->addChild(label1, 21);
				}
				shade->setPercentage(100);*/
			}this->unschedule(schedule_selector(sol_menu::updateload2));
				shade->removeFromParent();
		}
	}
}
void sol_menu::updateload3(float dt)//用作兵种刷新
{

	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable + 10, 3);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);
	if (shade->getPercentage() == 0)
	{

		if (num_tobuild[3] > 1)
		{
			buildsol(3);//造兵
			num_tobuild[3]--;
			//更换标签
			if (mycountry == 3)
			{
				cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
				cocos2d::String* num = String::createWithFormat("%d", num_tobuild[3]);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
				auto label0 = layer->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				label1->setName(numname->getCString());
				layer->addChild(label1, 21);
			}
				shade->setPercentage(100);
				//this->schedule(schedule_selector(sol_menu::updateload), 0.1f);
			
		}
		else if (num_tobuild[3] == 1)
		{
			buildsol(3);
			num_tobuild[3]--;
			if (mycountry == 3)
			{
				cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
				cocos2d::String* num = String::createWithFormat("%d", num_tobuild[3]);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
				auto label0 = layer->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				/*if (num_tobuild[3] == 2)
				{
				label1->setName(numname->getCString());
				layer->addChild(label1, 21);
				}
				shade->setPercentage(100);*/
			}this->unschedule(schedule_selector(sol_menu::updateload3));
				shade->removeFromParent();
		}
	}
}
void sol_menu::updateload4(float dt)//用作兵种刷新
{

	auto layer = this->getParent();
	cocos2d::String* shadename = String::createWithFormat("shade%d%d", lable + 10, 4);
	auto shade = (ProgressTimer *)layer->getChildByName(shadename->getCString());
	shade->setPercentage(shade->getPercentage() - 1);
	if (shade->getPercentage() == 0)
	{

		if (num_tobuild[4] > 1)
		{
			buildsol(4);//造兵
			num_tobuild[4]--;
			//更换标签
			if (mycountry == 4)
			{
				cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
				cocos2d::String* num = String::createWithFormat("%d", num_tobuild[4]);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
				auto label0 = layer->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				label1->setName(numname->getCString());
				layer->addChild(label1, 21);
			}
				shade->setPercentage(100);
				//this->schedule(schedule_selector(sol_menu::updateload), 0.1f);
			
		}
		else if (num_tobuild[4] == 1)
		{
			buildsol(4);
			num_tobuild[4]--;
			if (mycountry == 4)
			{
				cocos2d::String* numname = String::createWithFormat("soltobuild%d", lable);
				cocos2d::String* num = String::createWithFormat("%d", num_tobuild[4]);
				auto label1 = Label::createWithSystemFont(num->getCString(), "Arial", 10);
				label1->setPosition(cocos2d::Vec2(10 + position.x, 10 + position.y));
				auto label0 = layer->getChildByName(numname->getCString());
				if (label0)
				{
					label0->removeFromParent();
				}
				/*if (num_tobuild[4] == 2)
				{
				label1->setName(numname->getCString());
				layer->addChild(label1, 21);
				}
				shade->setPercentage(100);*/
			}this->unschedule(schedule_selector(sol_menu::updateload4));
				shade->removeFromParent();
		}
	}
}