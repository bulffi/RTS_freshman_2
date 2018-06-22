#include "soldier.h"
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <math.h>
#include"information.h"
#define num_soldier 5
USING_NS_CC;

soldier* soldier::create(const char* filename)
{
	
	soldier* sprite = new soldier();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}


//追赶敌人的调度器
void soldier::updateMove(float dt)
{
	if (enemy_target)//如果敌方单位还活着
	{
		double distance = sqrt(pow((enemy_target->getPositionX() - this->getPositionX()), 2) + pow((enemy_target->getPositionY() - this->getPositionY()), 2));
		//判断敌军是否进入攻击范围
		if (distance <= attack_distance)//如果进入，停止前进
		{

			this->stopAction(move_action);
			this->schedule(schedule_selector(soldier::updateAttack), attack_speed, kRepeatForever, 0.2f);
			this->unschedule(schedule_selector(soldier::updateMove));
		}
		else//如果没进入,继续前进
		{
			this->stopAction(move_action);
			this->move_action = MoveTo::create(distance / this->move_speed, cocos2d::Vec2(enemy_target->getPositionX(), enemy_target->getPositionY()));
			this->runAction(this->move_action);

		}
	}
	else//入果敌方单位突然死了
	{
		this->stopAction(move_action);
		this->unschedule(schedule_selector(soldier::updateMove));
	}
}

//持续攻击敌人的调度器
void soldier::updateAttack(float dt)
{
	if (enemy_target)//如果敌方单位还活着
	{
		double distance = sqrt(pow((enemy_target->getPositionX() - this->getPositionX()), 2) + pow((enemy_target->getPositionY() - this->getPositionY()), 2));
		if (distance <= attack_distance)//如果敌军在攻击范围内
		{
			//攻击
			this->attack_action = JumpBy::create(0.3f, Vec2(0, 0), 10, 1);
			this->runAction(this->attack_action);

			//enemy_target->stopAction(enemy_target->be_attack);
			//enemy_target->be_attack = Blink::create(0.3, 1);
			//enemy_target->runAction(enemy_target->be_attack);
			if(imformation::am_i_host)
				enemy_target->health -= attack_power;

		}
		else//如果敌军脱离了攻击范围
		{
			this->stopAction(attack_action);
			//this->schedule(schedule_selector(soldier::updateMove), 0.1f);
			this->move_to(enemy_target->getPositionX(), enemy_target->getPositionY(),0);
			this->unschedule(schedule_selector(soldier::updateAttack));
		}
	}
	else//如果敌方单位突然死亡
	{
		this->stopAction(attack_action);
		this->unschedule(schedule_selector(soldier::updateAttack));
	}
}


int soldier::SetPos(int nx, int ny)
{
	//this->stopActionByTag(1);
	path.clear();
	cocos2d::Point now_in_GL = this->getPosition();//现在的GL坐标
	cocos2d::Point now_in_tile = tileCoordForPosition(now_in_GL);//现在的瓦片坐标
	playerX = now_in_tile.x;//更新位置
	playerY = now_in_tile.y;
	int result = 0;
	if (Passable(nx, ny))
	{
		float totalCost;

		pather->Reset();///decide later


		result = pather->Solve(XYToNode(playerX, playerY), XYToNode(nx, ny), &path, &totalCost);///就是这一步在找路
		if (result == micropather::MicroPather::SOLVED) {
			//playerX = nx;
			//playerY = ny;
		}
	}
	return result;
}


