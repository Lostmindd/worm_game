#ifndef WORM_H
#define WORM_H
#include <deque>
#include "apples.h"
#include "game_score.h"
class Worm
{
public:
	Worm(Apples* apples, GameScore* score, int worm_num);
	Worm();
	void WormShowOnScreen();
	bool IsDestroyed();
	void WormMove(char ch);
	char WormMoveFromKeyboard();
private:
	void IncreaseWormLength(char character_ahead_of_the_worm);
	void DestroyWorm();
	void WormUpdateOnScreen(int worm_move_correction_x, int worm_move_correction_y);
	void WormBodySegmentDrawOnXY(int x, int y, std::string body_segment_color);
	wchar_t GetCharacterAheadOfTheWorm(int worm_move_correction_x, int worm_move_correction_y);
	struct BodySegment
	{
		int x;
		int y;
		bool operator !=(const BodySegment& other)
		{
			return !(this->x == other.x && this->y == other.y);
		}
	};
	bool destroyed_ = false;
	int worm_size_ = 4;
	int worm_num_;
	Apples* apples_;
	GameScore* score_;
	enum class WormDirection { UP, RIGHT, DOWN, LEFT }; //0 1 2 3
	WormDirection last_worm_direction_ = WormDirection::UP;
	std::deque<BodySegment> worm_body_;
};
#endif
