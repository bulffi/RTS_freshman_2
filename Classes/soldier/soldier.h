#ifndef __SOLDIER_H__
#define __SOLDIER_H__
#include "cocos2d.h"
#include"utility\open_source_Astar\micropather.h"
#define XMAX 50
#define YMAX 50
#define X_SIZE 32
#define Y_SIZE 32
class soldier :public cocos2d::Sprite,public micropather::Graph
{
public:
	int country = 0;
	int health = 20;
	int attack = 1;
	int speed = 100;
	int attack_distance = 100;
	float attack_speed = 1.0f;
	bool can_attack = 1;
	soldier* enemy_target = 0;
	
	
	virtual ~soldier()
	{
		if (pather)
			delete pather;
	}



	int Passable(int nx, int ny)
	{
		if (nx >= 0 && nx < XMAX
			&& ny >= 0 && ny < YMAX)
		{
			int index = ny*XMAX + nx;
			if ((p_map_situation)[index])
				return 1;
			else
				return 0;
		}
		return 0;
	}



	void NodeToXY(void* node, int* x, int* y)
	{
		intptr_t index = (intptr_t)node;
		*y = index / XMAX;
		*x = index - *y * XMAX;
	}

	void* XYToNode(int x, int y)
	{
		return (void*)(y*XMAX + x);
	}
	
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
		const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		const int cost[8] = { 10, 14, 10, 14, 10, 14, 10, 14 };

		NodeToXY(state, &x, &y);

		for (int i = 0; i < 8; ++i) 
		{
			int nx = x + dx[i];
			int ny = y + dy[i];

			int pass = Passable(nx, ny);
			if (pass)
			{
				micropather::StateCost nodeCost = { XYToNode(nx, ny), cost[i] };
				adjacent->push_back(nodeCost);
			}
			else
			{
				micropather::StateCost nodeCost = { XYToNode(nx, ny), FLT_MAX };
				adjacent->push_back(nodeCost);
			}

		}
	}


	cocos2d::Point tileCoordForPosition(cocos2d::Vec2 pos)
	{
		int x = pos.x / X_SIZE;
		int y = (Y_SIZE * YMAX - pos.y) / Y_SIZE;

		return cocos2d::Vec2(x, y);
	}


	cocos2d::Point positionForTileCoord(cocos2d::Vec2 pos)
	{
		int x = pos.x*X_SIZE;
		int y = Y_SIZE*YMAX - pos.y*Y_SIZE;

		return cocos2d::Vec2(x, y);
	}


	virtual void PrintStateInfo(void* state) override
	{
		int x, y;
		NodeToXY(state, &x, &y);
		cocos2d::log("(%d,%d)", x, y);
	}


	bool soldier::init()
	{
		pather = new micropather::MicroPather(this,2500,8);
		if (pather)
			return true;
		else
			return false;
	}
	

	static soldier* create(const char* filename, int*);
	void updateMove(float dt);
	void updateAttack(float dt);
	void move_all_time(float dt);

		int move_direction = 1;
	cocos2d::Action* move_action = 0;
	cocos2d::Action* attack_action = 0;
	cocos2d::Action* be_attack = 0;
	std::vector<void*> path;
	int SetPos(int nx, int ny);
	void popStepAndAnimate();
	void move_to(int x, int y);
private:
	int playerX;
	int playerY;
	
	micropather::MicroPather* pather;
    static int* p_map_situation;
};






#endif // __SOLDIER_H__

