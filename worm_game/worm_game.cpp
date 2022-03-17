#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include "game_score.h"
#include "apples.h"
#include "worm.h"
#include "move_x_y.h"
#include "constants.h"
const std::string kWormColors[8] = { "1;1","2;1", "3;1", "4;1", "5;1", "6;1", "3", "1" };
const int kFieldSizeByX = 119;
const int kFieldSizeByY = 30;

HANDLE std_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
void MoveXY(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(std_out_handle, coord);
}

int main()
{

	std::string names[8] = {"vas","dgh","dgh" ,"dgh" ,"dgh" ,"dgh" ,"dgh" ,"dgh" };
	GameScore score = { 8, names };
	system("mode con cols=160 lines=32");

	auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO struct_cursor_info;
	GetConsoleCursorInfo(handle, &struct_cursor_info);
	struct_cursor_info.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &struct_cursor_info);

		int current_score_line = 3 + 2 * score.GetPlayerNum();
		std::cout << "\x1b[30;1m";
		for (int i = 0; i < kFieldSizeByX + 4; i++)
		{
			std::cout << char(219);
		}
		std::cout << "\x1b[0m";
		std::cout << "\n";
		for (int i = 0; i < kFieldSizeByY; i++)
		{
			std::cout << "\x1b[30;1m";
			std::cout << char(219) << char(221) << std::setw(kFieldSizeByX + 1) << char(222) << char(219);
			std::cout << "\x1b[0m";
			score.TableCreate(&current_score_line);
			std::cout << "\n";
		}
		std::cout << "\x1b[30;1m";
		for (int i = 0; i < kFieldSizeByX + 4; i++)
		{
			std::cout << char(219);
		}
		std::cout << "\x1b[0m";
		Apples apples;
		apples.CreateApples(5);
		Worm worm = {&apples,&score,0 };
		Worm worm2 = {&apples,&score,3 };
		worm.WormShowOnScreen();
		worm2.WormShowOnScreen();
		while (!worm.IsDestroyed())
		{
			worm.WormMove();
		}
		while (!worm2.IsDestroyed())
		{
			worm2.WormMove();
		}
		score.GameOver();
}