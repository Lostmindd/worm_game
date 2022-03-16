#include <random>
#include <iostream>
#include <string>
#include "Worm.h"
#include "constants.h"
#include "MoveXY.h"
#include <conio.h> 

Worm::Worm(int FieldSizeByX, int FieldSizeByY, Apples* apples, GameScore* score, int WormNum)
{
	this->apples = apples;
	this->score = score;
	this->worm_num_ = WormNum;
	int StartPositionX = (WormNum * FieldSizeByX / 16) * 2 + 4;
	for (int i = 0; i < 4; i++)
	{
		int y_pos = (FieldSizeByY / 2) + i;
		BodySegment BodySegment = { StartPositionX, y_pos };
		apples->LockCell(StartPositionX, y_pos); 
		worm_body.push_back(BodySegment);
	}
	
}

bool Worm::IsDestroyed() // TODO : Удалить если не используеться
{
	return destroyed;
}

void Worm::WormBodySegmentDrawOnXY(int x, int y, std::string BodySegmentColor)
{
	MoveXY(x, y);
	std::cout << "\x1b[3" + BodySegmentColor + "m";
	std::cout << char(254);
	std::cout << "\x1b[0m";
}

void Worm::WormShowOnScreen()
{
	WormBodySegmentDrawOnXY(worm_body[0].x, worm_body[0].y, "0;1"); 
	for (int i = 1; i < WormSize; i++)
	{
		WormBodySegmentDrawOnXY(worm_body[i].x, worm_body[i].y, kWormColors[worm_num_]);
	}

}

void Worm::WormUpdateOnScreen(int worm_move_correction_x, int worm_move_correction_y)
{
	WormBodySegmentDrawOnXY(worm_body.front().x, worm_body.front().y, kWormColors[worm_num_]);

	int new_head_position_x = worm_body.front().x + worm_move_correction_x;
	int new_head_position_y = worm_body.front().y + worm_move_correction_y;
	worm_body.push_front({ new_head_position_x,new_head_position_y });

	WormBodySegmentDrawOnXY(new_head_position_x, new_head_position_y, "0;1");
	apples->LockCell(new_head_position_x, new_head_position_y);

	BodySegment old_tail_coord = worm_body.back();
	worm_body.pop_back();
	if (worm_body.back() != old_tail_coord) //освобождаем клетку (программно и визуально) если 
	{										//со стороны хвоста закончились одинаковые элементы
		MoveXY(old_tail_coord.x, old_tail_coord.y);
		std::cout << " ";
		apples->UnlockCell(old_tail_coord.x, old_tail_coord.y);
	}
}

void Worm::DestroyWorm()
{
	Apples apples;
	while (!worm_body.empty())
	{
		MoveXY(worm_body.back().x, worm_body.back().y);
		std::cout << "\x1b[3" + kWormColors[worm_num_] + "m" << 1 << "\x1b[0m";
		worm_body.pop_back();
	}
	destroyed = true;
}

wchar_t Worm::GetCharacterAheadOfTheWorm(int worm_move_correction_x, int worm_move_correction_y)
{
	int char_position_x = worm_body.front().x + worm_move_correction_x;
	int char_position_y = worm_body.front().y + worm_move_correction_y;
	COORD coord = { char_position_x,char_position_y };
	DWORD readed_characters = 0;
	wchar_t ch = 0;
	ReadConsoleOutputCharacter(std_out_handle, &ch, 1, coord, &readed_characters);
	return ch;
}

void Worm::IncreaseWormLength(char character_ahead_of_the_worm)
{
	for (int i = 0; i < character_ahead_of_the_worm-48; i++)
	{
		worm_body.push_back(worm_body.back());
	}
	apples->UpdateApple();
	score->TableUpdate(worm_num_, character_ahead_of_the_worm - 48);
}

void Worm::WormMove()
{
	if (destroyed == true) return;
	while(1)
	{
		int x_corection;
		int y_corection;
		char ch = _getch();
		if (ch == 72 && LastWormDirection != DOWN) //up
		{
			x_corection = 0;
			y_corection = -1;
			LastWormDirection = UP;
		}
		else if (ch == 77 && LastWormDirection != LEFT) //right
		{
			x_corection = 2;
			y_corection = 0;
			LastWormDirection = RIGHT;
		}
		else if (ch == 75 && LastWormDirection != RIGHT) //left
		{
			x_corection = -2;
			y_corection = 0;
			LastWormDirection = LEFT;
		}
		else if (ch == 80 && LastWormDirection != UP) //down
		{
			x_corection = 0;
			y_corection = 1;
			LastWormDirection = DOWN;
		}
		else continue;

		wchar_t character_ahead_of_the_worm = GetCharacterAheadOfTheWorm(x_corection, y_corection);
		if (character_ahead_of_the_worm == 32)
			WormUpdateOnScreen(x_corection, y_corection);
		else if (character_ahead_of_the_worm >= 49 && character_ahead_of_the_worm <= 53)
		{
			WormUpdateOnScreen(x_corection, y_corection);
			IncreaseWormLength(character_ahead_of_the_worm);
		}
		else
			DestroyWorm();
		break;
	}
}