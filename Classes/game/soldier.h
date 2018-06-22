
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
	//函数
	static soldier* create(const char* filename);//构造函数
	void updateMove(float dt);//移向敌军
	void updateAttack(float dt);//攻击敌军

	//需要调用子类init_data来初始化的变量
	int attack_power = 0;//攻击力
	int move_speed = 0;//移动速度
	int attack_distance = 0;//攻击范围
	float attack_speed = 0;//攻击速度
	cocos2d::Texture2D* texture_normal;//未被选中时的贴图
	cocos2d::Texture2D* texture_chosed;//被选中时的贴图
	

	//状态变量
	bool can_attack = 1;//能否进行下一次攻击
	unit* enemy_target = 0;//即将攻击的目标
	cocos2d::Action* move_action = 0;//移动动作
	cocos2d::Action* attack_action = 0;//攻击动作
	
	virtual ~soldier()
	{
		if (pather)
			delete pather;
	}

	//判断【在瓦片坐标下】某个位置是否可以走
	//实现原理就是查看situation数组，如果是0就不行，是1就行
	//这个函数在【规划路径】的过程中会不断被调用
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

	//micropather接受的是void*来用作路径指示
	//这就是参数中的void* node
	//我们需要在void* 和x y坐标中转化【当然都是瓦片坐标的视角下】

	void NodeToXY(void* node, int* x, int* y)
	{
		intptr_t index = (intptr_t)node;
		*y = index / XMAX;
		*x = index - *y * XMAX;
	}
	//上一个函数的逆
	void* XYToNode(int x, int y)
	{
		return (void*)(y*XMAX + x);
	}
	// LeastCostEstimate 以及下一个函数 AgjacentCost 无需了解
	// 我继承的graph类里面有这两个纯虚函数 我必须override
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
	///这是我们问题的来源【按照昨天我们的讨论】
	///这个函数是把传进来的鼠标GL坐标转化为瓦片坐标
	cocos2d::Point tileCoordForPosition(cocos2d::Vec2 pos)
	{
		int x = pos.x / X_SIZE;
		int y = (Y_SIZE * YMAX - pos.y) / Y_SIZE;

		return cocos2d::Vec2(x, y);
	}

	///这个函数是把瓦片坐标转化为鼠标GL坐标
	cocos2d::Point positionForTileCoord(cocos2d::Vec2 pos)
	{
		int x = pos.x*X_SIZE+X_SIZE*0.5;
		int y = Y_SIZE*YMAX - pos.y*Y_SIZE-Y_SIZE*0.5;

		return cocos2d::Vec2(x, y);
	}

	//用于debug输出的信息
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


	///SetPos函数里面的solve函数就是把找出来的路径存在这个path数组里面的
	std::vector<void*> path;



	int SetPos(int nx, int ny);
	void popStepAndAnimate();

	//move_to函数就是先调用SetPos来获得路径 再调用popStepAndAnimate来根据路径移动的
	void move_to(int x, int y,int type_info);


private:

	///现在的坐标【瓦片视角】
	int playerX;
	int playerY;
	int type_of_enemy;
	micropather::MicroPather* pather;
	int* p_map_situation;

	
	
};



#endif // __SOLDIER_H__#pragma once
