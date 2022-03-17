#include <random>
#include <iostream>
#include <string>
#include <conio.h> 
#include "worm.h"
#include "constants.h"
#include "move_x_y.h"

Worm::Worm(Apples* apples, GameScore* score, int worm_num)
{
    this->apples_ = apples;
    this->score_ = score;
    this->worm_num_ = worm_num;
    int start_position_x = (worm_num * kFieldSizeByX / 16) * 2 + 4;
    for (int i = 0; i < 4; i++)
    {
        int y_pos = (kFieldSizeByY / 2) + i;
        BodySegment BodySegment = { start_position_x, y_pos };
        apples->LockCell(start_position_x, y_pos);
        worm_body_.push_back(BodySegment);
    }
}

bool Worm::IsDestroyed() 
{
    return destroyed_;
}

void Worm::WormBodySegmentDrawOnXY(int x, int y, std::string body_segment_color)
{
    MoveXY(x, y);
    std::cout << "\x1b[3" + body_segment_color + "m";
    std::cout << char(254);
    std::cout << "\x1b[0m";
}

void Worm::WormShowOnScreen()
{
    WormBodySegmentDrawOnXY(worm_body_[0].x, worm_body_[0].y, "0;1");
    for (int i = 1; i < worm_size_; i++)
    {
        WormBodySegmentDrawOnXY(worm_body_[i].x, worm_body_[i].y, kWormColors[worm_num_]);
    }
}

void Worm::WormUpdateOnScreen(int worm_move_correction_x, int worm_move_correction_y)
{
    WormBodySegmentDrawOnXY(worm_body_.front().x, worm_body_.front().y, kWormColors[worm_num_]);

    int new_head_position_x = worm_body_.front().x + worm_move_correction_x;
    int new_head_position_y = worm_body_.front().y + worm_move_correction_y;
    worm_body_.push_front({ new_head_position_x,new_head_position_y });

    WormBodySegmentDrawOnXY(new_head_position_x, new_head_position_y, "0;1");
    apples_->LockCell(new_head_position_x, new_head_position_y);

    BodySegment old_tail_coord = worm_body_.back();
    worm_body_.pop_back();
    if (worm_body_.back() != old_tail_coord) //освобождаем клетку (программно и визуально) если 
    {										//со стороны хвоста закончились одинаковые элементы
        MoveXY(old_tail_coord.x, old_tail_coord.y);
        std::cout << " ";
        apples_->UnlockCell(old_tail_coord.x, old_tail_coord.y);
    }
}

void Worm::DestroyWorm()
{
    Apples apples;
    while (!worm_body_.empty())
    {
        MoveXY(worm_body_.back().x, worm_body_.back().y);
        std::cout << "\x1b[3" + kWormColors[worm_num_] + "m" << 1 << "\x1b[0m";
        worm_body_.pop_back();
    }
    destroyed_ = true;
}

wchar_t Worm::GetCharacterAheadOfTheWorm(int worm_move_correction_x, int worm_move_correction_y)
{
    int char_position_x = worm_body_.front().x + worm_move_correction_x;
    int char_position_y = worm_body_.front().y + worm_move_correction_y;
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
        worm_body_.push_back(worm_body_.back());
    }
    score_->TableUpdateScore(worm_num_, character_ahead_of_the_worm - 48);
    if (character_ahead_of_the_worm!=49)
    {
        apples_->UpdateApple();
    }
}

void Worm::WormMove()
{
    if (destroyed_ == true) return;

    score_->TableUpdateTurn(worm_num_);
    while(1)
    {
        int x_corection;
        int y_corection;
        char ch = _getch();
        if (ch == 72 && last_worm_direction_ != DOWN) //up
        {
            x_corection = 0;
            y_corection = -1;
            last_worm_direction_ = UP;
        }
        else if (ch == 77 && last_worm_direction_ != LEFT) //right
        {
            x_corection = 2;
            y_corection = 0;
            last_worm_direction_ = RIGHT;
        }
        else if (ch == 75 && last_worm_direction_ != RIGHT) //left
        {
            x_corection = -2;
            y_corection = 0;
            last_worm_direction_ = LEFT;
        }
        else if (ch == 80 && last_worm_direction_ != UP) //down
        {
            x_corection = 0;
            y_corection = 1;
            last_worm_direction_ = DOWN;
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
    score_->TableUpdateTurn(worm_num_);
}