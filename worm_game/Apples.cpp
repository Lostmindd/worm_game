#include <random>
#include <iostream>
#include "Apples.h"
#include "MoveXY.h"
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
	//int cells_num = (kFieldSizeByX + 1) / 2 * kFieldSizeByY;
	//empty_cells_.reserve(cells_num);
	//for (int y = 0; y < kFieldSizeByY + 1; y++)
	//{
	//	for (int x = 0; x < kFieldSizeByX + 2; x++)
	//	{
	//		if (x > 0 && y > 0 && x % 2 == 0)
	//			empty_cells_.push_back({ x,y });
	//	}
	//}
};

void Apples::LockCell(int x, int y) //Блокирует клетку для появления яблок
{		
	empty_cells_.erase({ x,y });
	//int vector_index_xy = (x - 2) / 2 + (y - 1) * 60; // Вычисление номера ячейки вектора где лежат x и y
	//CellCoord coord = { x,y };
	//if (empty_cells_[vector_index_xy] == coord) // Проверка, что ячейка не заблокирована
	//{
	//	int rand_cell;
	//	do { rand_cell = rand() % empty_cells_.size(); } 
	//	while (empty_cells_[rand_cell] == empty_cells_[vector_index_xy]);
	//	empty_cells_[vector_index_xy] = empty_cells_[rand_cell];
	//}
}

void Apples::UnlockCell(int x, int y) //Разблокирует клетку для появления яблок
{
	empty_cells_.insert({ x,y });


	//int vector_index_xy = (x - 2) / 2 + (y - 1) * 60; // Вычисление номера ячейки вектора где лежат x и y
	//empty_cells_[vector_index_xy].x = empty_cells_[x].x;
	//empty_cells_[vector_index_xy].y = empty_cells_[x].y;
}

void Apples::CreateApples(int count)
{
	srand(time(0));
	for (int i = 0; i < 5; i++)
		UpdateApple();
}

void Apples::UpdateApple()
{
	int random_place = rand() % empty_cells_.size(); 
	int AppleWeight = 2 + (rand() % 4); // 1-5
	auto it = empty_cells_.begin();
	std::advance(it, random_place);
	MoveXY((*it).x, (*it).y);
	std::cout << "\x1b[34;1m" << AppleWeight << "\x1b[0m";
	LockCell((*it).x, (*it).y);
}


