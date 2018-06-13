#include "soldier.h"
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <math.h>
#define num_soldier 5
USING_NS_CC;
int* soldier::p_map_situation = nullptr;
soldier* soldier::create(const char* filename, int* ptr)
{
	p_map_situation = ptr;
	soldier* sprite = new soldier();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void soldier::move_to(int x, int y)
{
	this->stopActionByTag(1);
	auto to = tileCoordForPosition(Vec2(x, y));
	int result = SetPos(to.x, to.y);
	
	if (result == micropather::MicroPather::NO_SOLUTION)
	{
		log("NO way");
	}
	if (result == micropather::MicroPather::SOLVED)
	{
		popStepAndAnimate();
	}
}
int soldier::SetPos(int nx, int ny)
{
	//this->stopActionByTag(1);
	path.clear();
	cocos2d::Point now_in_GL = this->getPosition();
	cocos2d::Point now_in_tile = tileCoordForPosition(now_in_GL);
	playerX = now_in_tile.x;
	playerY = now_in_tile.y;
	int result = 0;
	if (Passable(nx, ny))
	{
		float totalCost;

		pather->Reset();///decide later


		result = pather->Solve(XYToNode(playerX, playerY), XYToNode(nx, ny), &path, &totalCost);
		if (result == micropather::MicroPather::SOLVED) {
			//playerX = nx;
			//playerY = ny;
		}
	}
	return result;
}

void soldier::popStepAndAnimate()
{
	cocos2d::Point currentPosition = tileCoordForPosition(this->getPosition());
	if (path.size() == 0)
	{
		return;
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

	double distance= sqrt(pow(diff.x, 2) + pow(diff.y, 2));
	auto moveAction = cocos2d::MoveTo::create(distance/10, positionForTileCoord(cocos2d::Point(tempt_x, tempt_y)));
	auto *moveCallback = cocos2d::CallFunc::create(CC_CALLBACK_0(soldier::popStepAndAnimate, this));
	path.erase(path.begin());
	// 运行动作
	auto moveSequence = cocos2d::Sequence::create(moveAction, moveCallback, nullptr);
	moveSequence->setTag(1);
	playerX = tempt_x;
	playerY = tempt_y;
	this->runAction(moveSequence);
}

void soldier::updateMove(float dt)
{
	if (enemy_target)
	{
		double distance = sqrt(pow((enemy_target->getPositionX() - this->getPositionX()), 2) + pow((enemy_target->getPositionY() - this->getPositionY()), 2));
		if (distance <= attack_distance)
		{
			this->stopAction(move_action);
			this->schedule(schedule_selector(soldier::updateAttack), attack_speed, kRepeatForever, 0.2f);
			this->unschedule(schedule_selector(soldier::updateMove));
		}
		else
		{
			this->stopAction(move_action);
			this->move_action = MoveTo::create(distance / this->speed, cocos2d::Vec2(enemy_target->getPositionX(), enemy_target->getPositionY()));
			this->runAction(this->move_action);
		}
	}
	else
	{
		this->stopAction(move_action);
		this->unschedule(schedule_selector(soldier::updateMove));
	}
}

void soldier::updateAttack(float dt)
{
	if (enemy_target)
	{
		double distance = sqrt(pow((enemy_target->getPositionX() - this->getPositionX()), 2) + pow((enemy_target->getPositionY() - this->getPositionY()), 2));
		if (distance <= attack_distance)
		{			
			this->attack_action = JumpBy::create(0.3f, Vec2(0, 0), 10, 1);
			this->runAction(this->attack_action);
			enemy_target->stopAction(enemy_target->be_attack);
			enemy_target->be_attack = Blink::create(0.3, 1);
			enemy_target->runAction(enemy_target->be_attack);
			enemy_target->health -= attack;
		}
		else
		{
			this->stopAction(attack_action);
			this->schedule(schedule_selector(soldier::updateMove), 0.1f);
			this->unschedule(schedule_selector(soldier::updateAttack));
		}
	}
	else
	{
		this->stopAction(attack_action);
		this->unschedule(schedule_selector(soldier::updateAttack));
	}
}
void soldier::move_all_time(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (this->getPositionY() < origin.y + visibleSize.height && this->getPositionY() > origin.y)
	{
		if (this->move_direction == 1)
			this->setPositionY(this->getPositionY() + 5);
		else if (this->move_direction == -1)
			this->setPositionY(this->getPositionY() - 5);
	}
	else
	{
		if (this->getPositionY() >= origin.y + visibleSize.height)
		{
			this->move_direction = -1;
			this->setPositionY(this->getPositionY() - 5);
		}
		else if (this->getPositionY() <= origin.y)
		{
			this->move_direction = 1;
			this->setPositionY(this->getPositionY() + 5);
		}
	}
}