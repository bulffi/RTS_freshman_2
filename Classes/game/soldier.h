
#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include"unit.h"
#include "cocos2d.h"
#include"micropather.h"
#define XMAX 50
#define YMAX 50
#define X_SIZE 32
#define Y_SIZE 32
class soldier :public unit, public micropather::Graph
{
public:
	//����
	static soldier* create(const char* filename);//���캯��
	void updateMove(float dt);//����о�
	void updateAttack(float dt);//�����о�

	//��Ҫ��������init_data����ʼ���ı���
	int attack_power = 0;//������
	int move_speed = 0;//�ƶ��ٶ�
	int attack_distance = 0;//������Χ
	float attack_speed = 0;//�����ٶ�
	cocos2d::Texture2D* texture_normal;//δ��ѡ��ʱ����ͼ
	cocos2d::Texture2D* texture_chosed;//��ѡ��ʱ����ͼ
	

	//״̬����
	bool can_attack = 1;//�ܷ������һ�ι���
	unit* enemy_target = 0;//����������Ŀ��
	cocos2d::Action* move_action = 0;//�ƶ�����
	cocos2d::Action* attack_action = 0;//��������
	
	virtual ~soldier()
	{
		if (pather)
			delete pather;
	}

	//�жϡ�����Ƭ�����¡�ĳ��λ���Ƿ������
	//ʵ��ԭ����ǲ鿴situation���飬�����0�Ͳ��У���1����
	//��������ڡ��滮·�����Ĺ����л᲻�ϱ�����
	bool Passable(int nx, int ny)
	{
		if (nx >= 0 && nx < XMAX
			&& ny >= 0 && ny < YMAX)
		{
			int index = ny*XMAX + nx;
			if ((p_map_situation)[index])
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	}

	//micropather���ܵ���void*������·��ָʾ
	//����ǲ����е�void* node
	//������Ҫ��void* ��x y������ת������Ȼ������Ƭ������ӽ��¡�

	void NodeToXY(void* node, int* x, int* y)
	{
		intptr_t index = (intptr_t)node;
		*y = index / XMAX;
		*x = index - *y * XMAX;
	}
	//��һ����������
	void* XYToNode(int x, int y)
	{
		return (void*)(y*XMAX + x);
	}
	// LeastCostEstimate �Լ���һ������ AgjacentCost �����˽�
	// �Ҽ̳е�graph�����������������麯�� �ұ���override
	virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd) override
	{
		int xStart, yStart, xEnd, yEnd;
		NodeToXY(nodeStart, &xStart, &yStart);
		NodeToXY(nodeEnd, &xEnd, &yEnd);
		int dx = xStart - xEnd;
		int dy = yStart - yEnd;
		return (float)sqrt((double)(dx*dx) + (double)(dy*dy));
	}


	virtual void AdjacentCost(void* state, std::vector< micropather::StateCost > *adjacent) override
	{
		int x, y;
		const int dx[8] = { 1, 1, 0, -1, -1, -1,  0, 1 };
		const int dy[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };
		const int cost[8] = { 10, 14, 10, 14, 10, 14, 10, 14 };

		NodeToXY(state, &x, &y);

		for (int i = 0; i < 8; ++i)
		{
			const int nx = x + dx[i];
			const int ny = y + dy[i];

			//int pass = Passable(nx, ny);
			if (Passable(nx, ny))
			{
				if (i == 0 || i == 2 || i == 4 || i == 6)
				{
					micropather::StateCost nodeCost = { XYToNode(nx, ny), 10 };
					adjacent->push_back(nodeCost);
				}
				else if(i == 1 || i == 3 || i == 5)
				{
					if (Passable(x + dx[i - 1], y + dy[i - 1]) && Passable(x + dx[i + 1], y + dy[i + 1]))
					{
						micropather::StateCost nodeCost = { XYToNode(nx, ny), 14 };
						adjacent->push_back(nodeCost);
					}
					else
					{
						micropather::StateCost nodeCost = { XYToNode(nx, ny), FLT_MAX };
						adjacent->push_back(nodeCost);
					}
				}
				else
				{
					if (Passable(x + dx[6], y + dy[6]) && Passable(x + dx[0], y + dy[0]))
					{
						micropather::StateCost nodeCost = { XYToNode(nx, ny), 14 };
						adjacent->push_back(nodeCost);
					}
					else
					{
						micropather::StateCost nodeCost = { XYToNode(nx, ny), FLT_MAX };
						adjacent->push_back(nodeCost);
					}
				}
				/*micropather::StateCost nodeCost = { XYToNode(nx, ny), cost[i] };
				adjacent->push_back(nodeCost);*/
			}
			else
			{
				micropather::StateCost nodeCost = { XYToNode(nx, ny), FLT_MAX };
				adjacent->push_back(nodeCost);
			}

		}
	}
	std::string report_my_position()
	{
		std::string my_position;
		float X = this->getPosition().x;
		float Y = this->getPosition().y;
		int blod = health;
		for (int m = 1000; m >= 1; m /= 10)
		{
			int n = X / m;
			my_position += std::to_string(n);
			X -= n*m;
		}
		for (int m = 1000; m >= 1; m /= 10)
		{
			int n = Y / m;
			my_position += std::to_string(n);
			Y -= n*m;
		}
		for (int m = 10; m >= 1; m /= 10)
		{
			int n = blod / m;
			my_position += std::to_string(n);
			blod -= n*m;
		}
		return my_position;
	}

	//void soldier::move_to();
	///���������������Դ�������������ǵ����ۡ�
	///��������ǰѴ����������GL����ת��Ϊ��Ƭ����
	cocos2d::Point tileCoordForPosition(cocos2d::Vec2 pos)
	{
		int x = pos.x / X_SIZE;
		int y = (Y_SIZE * YMAX - pos.y) / Y_SIZE;

		return cocos2d::Vec2(x, y);
	}

	///��������ǰ���Ƭ����ת��Ϊ���GL����
	cocos2d::Point positionForTileCoord(cocos2d::Vec2 pos)
	{
		int x = pos.x*X_SIZE+X_SIZE*0.5;
		int y = Y_SIZE*YMAX - pos.y*Y_SIZE-Y_SIZE*0.5;

		return cocos2d::Vec2(x, y);
	}

	//����debug�������Ϣ
	virtual void PrintStateInfo(void* state) override
	{
		int x, y;
		NodeToXY(state, &x, &y);
		cocos2d::log("(%d,%d)", x, y);
	}


	bool soldier::init(int* ptr)
	{
		p_map_situation = ptr;
		pather = new micropather::MicroPather(this, 2500, 8);
		if (pather)
			return true;
		else
			return false;
	}


	///SetPos���������solve�������ǰ��ҳ�����·���������path���������
	std::vector<void*> path;



	int SetPos(int nx, int ny);
	void popStepAndAnimate();

	//move_to���������ȵ���SetPos�����·�� �ٵ���popStepAndAnimate������·���ƶ���
	void move_to(int x, int y,int type_info);


private:

	///���ڵ����꡾��Ƭ�ӽǡ�
	int playerX;
	int playerY;
	int type_of_enemy;
	micropather::MicroPather* pather;
	int* p_map_situation;

	
	
};



#endif // __SOLDIER_H__#pragma once
