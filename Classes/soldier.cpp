#include "soldier.h"
#include <math.h>
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
			this->move_action = MoveTo::create(distance / this->speed, cocos2d::Vec2(enemy_target->getPositionX(), enemy_target->getPositionY()));
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
			
			enemy_target->stopAction(enemy_target->be_attack);
			enemy_target->be_attack = Blink::create(0.3, 1);
			enemy_target->runAction(enemy_target->be_attack);
			enemy_target->health -= attack;
			
		}
		else//����о������˹�����Χ
		{
			this->stopAction(attack_action);
			this->schedule(schedule_selector(soldier::updateMove), 0.1f);
			this->unschedule(schedule_selector(soldier::updateAttack));
		}
	}
	else//����з���λͻȻ����
	{
		this->stopAction(attack_action);
		this->unschedule(schedule_selector(soldier::updateAttack));
	}
}

//���������ƶ�
void soldier::move_all_time(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//�����������Ļ��
	if (this->getPositionY() < origin.y + visibleSize.height && this->getPositionY() > origin.y)
	{
		if (this->move_direction == 1)
			this->setPositionY(this->getPositionY() + 5);
		else if (this->move_direction == -1)
			this->setPositionY(this->getPositionY() - 5);
	}
	else//���������Ļ��
	{
		if (this->getPositionY() >= origin.y + visibleSize.height)//�����ϱ߿�
		{
			this->move_direction = -1;
			this->setPositionY(this->getPositionY() - 5);
		}
		else if (this->getPositionY() <= origin.y)//�����±߿�
		{
			this->move_direction = 1;
			this->setPositionY(this->getPositionY() + 5);
		}
	}
}