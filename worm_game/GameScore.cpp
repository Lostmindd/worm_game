#include <string>
#include <iostream>
#include <iomanip>
#include "GameScore.h"
#include "constants.h"
#include "MoveXY.h"

GameScore::GameScore(int player_num, std::string names[])
{
	this->player_num = player_num;
	this->names = names;
	scores = new int[player_num]{0};
}

GameScore::~GameScore()
{
	delete[] scores;
}

void GameScore::DrawStrip(int char_num, int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << char(char_num);
	}
}


void GameScore::ChangeOutputColor(int player)
{
	std::cout << "\x1b[3" + kWormColors[player] + "m";
}

void GameScore::ResetOutputColor()
{
	std::cout << "\x1b[0m";
}

void GameScore::TableCreate(int* current_score_line)
{
	if ((*current_score_line) == 0) return;
	int draw_step = 3 + 2 * player_num - *current_score_line;
	if (draw_step < 3)
	{
		if (draw_step == 0)
		{
			std::cout << std::setw(3) << char(201);
			DrawStrip(205, 11);
			std::cout << char(187);
		}
		if (draw_step == 1)
		{
			std::cout << std::setw(3) << char(186) << " S c o r e " << char(186);

		}
		if (draw_step == 2)
		{
			std::cout << std::setw(2) << char(204) << char(202);
			DrawStrip(205, 11);
			std::cout << char(202) << char(185);
		}
	}
	else
	{
		if (draw_step % 2 != 0)
		{
			int curent_player = (draw_step / 2) - 1;
			ChangeOutputColor(curent_player);
			std::cout << std::setw(2) << char(204) << " ";
			std::cout.width(3);
			std::cout << names[curent_player] << " : ";
			std::cout.width(5);
			std::cout << 0;
			std::cout << std::setw(2) << char(185);
		}
		else
		{
			int curent_player = (draw_step / 2) - 2;
			ChangeOutputColor(curent_player);
			std::cout << std::setw(2) << char(185);
			DrawStrip(176, 13);
			std::cout << char(204);
		}
		ResetOutputColor();
	}

	(*current_score_line)--;
}

void GameScore::TableUpdate(int player, int score_correction)
{
	ChangeOutputColor(player);
	int y = 4 + 2 * player;
	int x = kFieldSizeByX + 13;
	MoveXY(x, y);
	std::cout.width(5);
	scores[player] += score_correction;
	std::cout << scores[player];
	ResetOutputColor();
}

char GameScore::GetPlayerNum() //TODO: мб убрать
{
	return player_num;
}