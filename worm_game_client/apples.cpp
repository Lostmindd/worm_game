#include <random>
#include <iostream>
#include "apples.h"
#include "move_x_y.h"
#include "constants.h"

Apples::Apples() 
{
	for (int y = 0; y < kFieldSizeByY+1; y++)
	{
		for (int x = 0; x < kFieldSizeByX+2; x++)
		{
			if(x > 0 && y>0 && x%2 == 0)
				empty_cells_.insert({ x,y });
		}
	}
};

void Apples::LockCell(int x, int y) //Блокирует клетку для появления яблок
{		
	empty_cells_.erase({ x,y });
}

void Apples::UnlockCell(int x, int y) //Разблокирует клетку для появления яблок
{
	empty_cells_.insert({ x,y });
}

void Apples::CreateApples(int count, int random_seed)
{
	srand(random_seed);
	for (int i = 0; i < 5; i++)
		UpdateApple();
}

void Apples::UpdateApple()
{
	int random_place = rand() % empty_cells_.size(); 
	int apple_weight = 2 + (rand() % 4); // 1-5
	auto iter = empty_cells_.begin();
	std::advance(iter, random_place);
	MoveXY((*iter).x, (*iter).y);
	std::cout << "\x1b[34;1m" << apple_weight << "\x1b[0m";
	LockCell((*iter).x, (*iter).y);
}