#include"mine.h"
#include"cocos2d.h"
#include"utility\information.h"
#include"HelloWorldScene.h"

mine* mine::create(const char* filename)
{
	mine* sprite = new mine();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void mine::set_data(int country, int x, int y)//设置参数
{
	this->tile_position_point[0] = x;
	this->tile_position_point[1] = y;
	my_country = country;
	health = 15	;
	beginhealth = health;
	this->setAnchorPoint(cocos2d::Vec2(0.5, 0.75));
}
void mine::change_situation(std::vector<int>& situation, int condition)
{
	for (int i = 0; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			situation[(this->tile_position_point[0] + j) + imformation::XMAX*(this->tile_position_point[1] + i)] = condition;
}
bool mine::is_covered_by_biulding_or_land(int x, int y, std::vector<int> situation)
{
	if (situation[x - 1 + imformation::XMAX*y] == 1 && situation[x - 1 + imformation::XMAX*(y + 1)] == 1 && situation[x + imformation::XMAX*y] == 1 && situation[x + 1 + imformation::XMAX*y] == 1 && situation[x + imformation::XMAX*(y + 1)] == 1 && situation[x + 1 + imformation::XMAX*(y + 1)] == 1)
		return false;
	else
		return true;
}
cocos2d::Rect mine::building_rect_when_click(int x, int y)
{
	cocos2d::Rect rect(32 * (x - 1), 32 * (imformation::YMAX - y - 2), 96, 64);
	return rect;
}

mine_car* mine_car::create(const char* filename)
{
	mine_car* sprite = new mine_car();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void mine_car::set_data(int country, unit* mine, unit* mines)//设置参数
{
	my_mine = mine;
	my_mines = mines;
	move_speed = 5;
	my_country = country;
	health = 50;
	beginhealth = health;
	//attack_distance = 40;
	this->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
}
void mine_car::movetomines(float dt)
{
	//double distance = sqrt(pow((my_mine->getPositionX() - this->getPositionX()), 2) + pow((my_mine->getPositionY() - this->getPositionY()), 2));
	//double distance1 = sqrt(pow((my_mines->getPositionX() - this->getPositionX()), 2) + pow((my_mines->getPositionY() - this->getPositionY()), 2));
	cocos2d::Rect rect = my_mine->getBoundingBox();
	if (rect.intersectsCircle(this->getPosition(), 30)) 
	//if (distance < 51)
	{
		this->enemy_target = my_mines;
		this->move_to(HelloWorld::vec_mines[this->my_country]->getPosition().x, HelloWorld::vec_mines[this->my_country]->getPosition().y, 5);
	}
}
void mine_car::movetomine(float dt)
{
	//double distance = sqrt(pow((my_mine->getPositionX() - this->getPositionX()), 2) + pow((my_mine->getPositionY() - this->getPositionY()), 2));
	//double distance1 = sqrt(pow((my_mines->getPositionX() - this->getPositionX()), 2) + pow((my_mines->getPositionY() - this->getPositionY()), 2));
	cocos2d::Rect rect = my_mines->getBoundingBox();
	//if (distance1<51)
	if (rect.intersectsCircle(this->getPosition(), 30))
	{
		enemy_target = this->my_mine;
		this->move_to(my_mine->getPosition().x, my_mine->getPosition().y, 3);
		if_add = 1;
	}
}
void mine_car::updatecar1(float dt)
{
	/*double distance = sqrt(pow((my_mines->getPositionX() -this->getPositionX()), 2) + pow((my_mines->getPositionY() - this->getPositionY()), 2));
	if (distance < 52) {
		this->scheduleOnce(schedule_selector(mine_car::movetomine), 2.0f);
	}
	double distance1 = sqrt(pow((this->my_mine->getPositionX() - this->getPositionX()), 2) + pow((my_mine->getPositionY() - this->getPositionY()), 2));
	if (distance1 < 52)
	{
		this->scheduleOnce(schedule_selector(mine_car::movetomines), 2.0f);
		if (this->if_add == 1)
		{
			this->if_add = 2;
		}
	}*/
	cocos2d::Rect rect_mine = my_mine->getBoundingBox();
	cocos2d::Rect rect_mines = my_mines->getBoundingBox();
	if (rect_mines.intersectsCircle(this->getPosition(), 30))
		this->scheduleOnce(schedule_selector(mine_car::movetomine), 2.0f);
	if(rect_mine.intersectsCircle(this->getPosition(),30))
	{
		this->scheduleOnce(schedule_selector(mine_car::movetomines), 2.0f);
		if (this->if_add == 1)
		{
			this->if_add = 2;
		}
	}
	

}
/*void mine_car::updatecar2(float dt)
{
	double distance = sqrt(pow((vec_mines[2]->getPositionX() - vec_mine_car[2][0]->getPositionX()), 2) + pow((vec_mines[2]->getPositionY() - vec_mine_car[2][0]->getPositionY()), 2));
	if (distance < 52)
		vec_mine_car[2][0]->scheduleOnce(schedule_selector(mine_car::movetomine), 2.0f);
	double distance1 = sqrt(pow((vec_mine_car[2][0]->my_mine->getPositionX() - vec_mine_car[2][0]->getPositionX()), 2) + pow((vec_mine[2][0]->getPositionY() - vec_mine_car[2][0]->getPositionY()), 2));
	if (distance1 < 52)
	{
		vec_mine_car[2][0]->scheduleOnce(schedule_selector(mine_car::movetomines), 2.0f);
		if (vec_mine_car[2][0]->if_add == 1)
		{
			money[2] += 50;
			auto moneynum = (Label*)this->getChildByName("moneynum");
			cocos2d::String* num1 = String::createWithFormat("%d", money[2]);
			moneynum->setString(num1->getCString());
			vec_mine_car[2][0]->if_add = 0;
		}
	}
}
void mine_car::updatecar3(float dt)
{
	double distance = sqrt(pow((vec_mines[3]->getPositionX() - vec_mine_car[3][0]->getPositionX()), 2) + pow((vec_mines[3]->getPositionY() - vec_mine_car[3][0]->getPositionY()), 2));
	if (distance < 52)
		vec_mine_car[3][0]->scheduleOnce(schedule_selector(mine_car::movetomine), 2.0f);
	double distance1 = sqrt(pow((vec_mine_car[3][0]->my_mine->getPositionX() - vec_mine_car[3][0]->getPositionX()), 2) + pow((vec_mine[3][0]->getPositionY() - vec_mine_car[3][0]->getPositionY()), 2));
	if (distance1 < 52)
	{
		vec_mine_car[3][0]->scheduleOnce(schedule_selector(mine_car::movetomines), 2.0f);
		if (vec_mine_car[3][0]->if_add == 1)
		{
			money[3] += 50;
			auto moneynum = (Label*)this->getChildByName("moneynum");
			cocos2d::String* num1 = String::createWithFormat("%d", money[3]);
			moneynum->setString(num1->getCString());
			vec_mine_car[3][0]->if_add = 0;
		}
	}
}
void mine_car::updatecar4(float dt)
{
	double distance = sqrt(pow((vec_mines[4]->getPositionX() - vec_mine_car[4][0]->getPositionX()), 2) + pow((vec_mines[4]->getPositionY() - vec_mine_car[4][0]->getPositionY()), 2));
	if (distance < 52)
		vec_mine_car[4][0]->scheduleOnce(schedule_selector(mine_car::movetomine), 2.0f);
	double distance1 = sqrt(pow((vec_mine_car[4][0]->my_mine->getPositionX() - vec_mine_car[4][0]->getPositionX()), 2) + pow((vec_mine[4][0]->getPositionY() - vec_mine_car[4][0]->getPositionY()), 2));
	if (distance1 < 52)
	{
		vec_mine_car[4][0]->scheduleOnce(schedule_selector(mine_car::movetomines), 2.0f);
		if (vec_mine_car[4][0]->if_add == 1)
		{
			money[4] += 50;
			auto moneynum = (Label*)this->getChildByName("moneynum");
			cocos2d::String* num1 = String::createWithFormat("%d", money[4]);
			moneynum->setString(num1->getCString());
			vec_mine_car[4][0]->if_add = 0;
		}
	}
}*/