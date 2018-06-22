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


//׷�ϵ��˵ĵ�����
void soldier::updateMove(float dt)
{
	if (enemy_target)//����з���λ������
	{
		double distance = sqrt(pow((enemy_target->getPositionX() - this->getPositionX()), 2) + pow((enemy_target->getPositionY() - this->getPositionY()), 2));
		//�жϵо��Ƿ���빥����Χ
		if (distance <= attack_distance)//������룬ֹͣǰ��
		{

			this->stopAction(move_action);
			this->schedule(schedule_selector(soldier::updateAttack), attack_speed, kRepeatForever, 0.2f);
			this->unschedule(schedule_selector(soldier::updateMove));
		}
		else//���û����,����ǰ��
		{
			this->stopAction(move_action);
			this->move_action = MoveTo::create(distance / this->move_speed, cocos2d::Vec2(enemy_target->getPositionX(), enemy_target->getPositionY()));
			this->runAction(this->move_action);

		}
	}
	else//����з���λͻȻ����
	{
		this->stopAction(move_action);
		this->unschedule(schedule_selector(soldier::updateMove));
	}
}

//�����������˵ĵ�����
void soldier::updateAttack(float dt)
{
	if (enemy_target)//����з���λ������
	{
		double distance = sqrt(pow((enemy_target->getPositionX() - this->getPositionX()), 2) + pow((enemy_target->getPositionY() - this->getPositionY()), 2));
		if (distance <= attack_distance)//����о��ڹ�����Χ��
		{
			//����
			this->attack_action = JumpBy::create(0.3f, Vec2(0, 0), 10, 1);
			this->runAction(this->attack_action);

			//enemy_target->stopAction(enemy_target->be_attack);
			//enemy_target->be_attack = Blink::create(0.3, 1);
			//enemy_target->runAction(enemy_target->be_attack);
			if(imformation::am_i_host)
				enemy_target->health -= attack_power;

		}
		else//����о������˹�����Χ
		{
			this->stopAction(attack_action);
			//this->schedule(schedule_selector(soldier::updateMove), 0.1f);
			this->move_to(enemy_target->getPositionX(), enemy_target->getPositionY(),0);
			this->unschedule(schedule_selector(soldier::updateAttack));
		}
	}
	else//����з���λͻȻ����
	{
		this->stopAction(attack_action);
		this->unschedule(schedule_selector(soldier::updateAttack));
	}
}


int soldier::SetPos(int nx, int ny)
{
	//this->stopActionByTag(1);
	path.clear();
	cocos2d::Point now_in_GL = this->getPosition();//���ڵ�GL����
	cocos2d::Point now_in_tile = tileCoordForPosition(now_in_GL);//���ڵ���Ƭ����
	playerX = now_in_tile.x;//����λ��
	playerY = now_in_tile.y;
	int result = 0;
	if (Passable(nx, ny))
	{
		float totalCost;

		pather->Reset();///decide later


		result = pather->Solve(XYToNode(playerX, playerY), XYToNode(nx, ny), &path, &totalCost);///������һ������·
		if (result == micropather::MicroPather::SOLVED) {
			//playerX = nx;
			//playerY = ny;
		}
	}
	return result;
}


