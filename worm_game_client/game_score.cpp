#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "game_score.h"
#include "constants.h"
#include "move_x_y.h"

GameScore::GameScore(int player_num, std::string names[])
{
	this->player_num_ = player_num;
	this->names_ = names;
	scores_ = new int[player_num]{0};
}

GameScore::~GameScore()
{
	delete[] scores_;
}

void GameScore::DrawStrip(int char_num, int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << char(char_num);
	}
}

void GameScore::ChangeOutputColor(std::string color)
{
	std::cout << "\x1b[3" + color + "m";
}

void GameScore::ResetOutputColor()
{
	std::cout << "\x1b[0m";
}

void GameScore::TableCreate(int* current_score_line)
{
	if ((*current_score_line) == 0) return;
	int draw_step = 3 + 2 * player_num_ - *current_score_line;
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
			ChangeOutputColor(kWormColors[curent_player] + ";4");
			std::cout << std::setw(2) << char(204) << " ";
			std::cout.width(3);
			std::cout << names_[curent_player] << " : ";
			std::cout.width(5);
			std::cout << 0;
			std::cout << std::setw(2) << char(185);
		}
		else
		{
			int curent_player = (draw_step / 2) - 2;
			ChangeOutputColor(kWormColors[curent_player] + ";4");
			std::cout << std::setw(2) << char(185);
			DrawStrip(176, 13);
			std::cout << char(204);
		}
		ResetOutputColor();
	}
	(*current_score_line)--;
}

void GameScore::TableUpdateScore(int player, int score_correction)
{
	ChangeOutputColor(kWormColors[player] + ";4");
	int y = 4 + 2 * player;
	int x = kFieldSizeByX + 13;
	MoveXY(x, y);
	std::cout.width(5);
	scores_[player] += score_correction;
	std::cout << scores_[player];
	ResetOutputColor();
}

wchar_t GameScore::GetCharacterXY(int x, int y)
{
	COORD coord = { x,y };
	DWORD readed_characters = 0;
	wchar_t ch = 0;
	ReadConsoleOutputCharacter(std_out_handle, &ch, 1, coord, &readed_characters);
	return ch;
}

void GameScore::TableUpdateTurn(int player)
{
	ChangeOutputColor(kWormColors[player]);
	int y = 4 + 2 * player;
	int x = kFieldSizeByX + 21;
	MoveXY(x, y);
	char ch = GetCharacterXY(x, y);
	if(ch == 32)
		std::cout << "<<_Your_turn_<<";
	else
		std::cout << "               ";
	ResetOutputColor();
}

void GameScore::GameOver()
{
	struct PlayersAndScores
	{
		int score;
		std::string name;
		int player_num;
		bool operator< (const PlayersAndScores& other)
		{
			return this->score > other.score;
		}
	};
	std::vector<PlayersAndScores> score_output;
	score_output.reserve(player_num_);
	for (int i = 0; i < player_num_; i++)
		score_output.push_back({ scores_[i], names_[i],i });
	std::sort(score_output.begin(), score_output.end());
	ChangeOutputColor(kWormColors[0]);
	system("cls");
	std::cout << std::setw(130) << "|**\\        ########       #      ##   ##  ######           ######  #        #  ######  ######        /**|\n";
	std::cout << std::setw(130) << "|*>*\\       #             # #     # # # #  #                #    #  #        #  #       #    #       /*<*|\n";
	std::cout << std::setw(130) << "|*>*>*\\     #   ####     #   #    #  #  #  ######           #    #   #      #   ######  ######     /*<*<*|\n";
	std::cout << std::setw(130) << "|*>*>*/     #      #    #######   #     #  #                #    #    #    #    #       # #        \\*<*<*|\n";
	std::cout << std::setw(130) << "|*>*/       #      #   #       #  #     #  #                #    #     #  #     #       #  #         \\*<*|\n";
	std::cout << std::setw(130) << "|**/        ########  #         # #     #  ######           ######      ##      ######  #    #        \\**|\n";
	std::cout << std::setw(130) << "|*/                                                                                                    \\*|\n";
	std::cout << std::setw(130) << "|/                                                                                                      \\|\n";
	std::cout << std::setw(130) << "|                                              #=-=-=-=-=-=#                                             |\n";
	for (int i = 0; i < player_num_; i++)
	{
		std::cout << std::setw(24);
		std::cout << "|";
		for (int j = 45; j >= 0; j--)
		{
			if (i != 0 && j%(i*2) == 0)
			{
				std::cout << "#";
			}
			else std::cout << " ";
		}
		std::cout << "|";
		ResetOutputColor();
		ChangeOutputColor(kWormColors[score_output[i].player_num]);
		std::cout << score_output[i].name << " - ";
		std::cout.width(5);
		std::cout << score_output[i].score;
		ResetOutputColor();
		ChangeOutputColor(kWormColors[0]);
		std::cout << "|";
		for (int j = 0; j < 45; j++)
		{
			if (i != 0 && j % (i * 2) == 0)
			{
				std::cout << "#";
			}
			else std::cout << " ";
		}
		std::cout << "|\n";
	}
	std::cout << std::setw(30);
	ChangeOutputColor(kWormColors[0]);
	std::cout << "\\=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\\===========/-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-/";
	ResetOutputColor();
	std::cout << "\n";
}

char GameScore::GetPlayerNum() 
{
	return player_num_;
}