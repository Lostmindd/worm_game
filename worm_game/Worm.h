#ifndef WORM_H
#define WORM_H
#include "Apples.h"
#include <deque>
#include "GameScore.h"
class Worm
{
public:
	Worm(int FieldSizeByX, int FieldSizeByY, Apples* apples, GameScore* score, int WormNum);
	void WormShowOnScreen();
	bool IsDestroyed();
	void WormMove();
private:
	void IncreaseWormLength(char character_ahead_of_the_worm);
	void DestroyWorm();
	void WormUpdateOnScreen(int worm_move_correction_x, int worm_move_correction_y);
	void WormBodySegmentDrawOnXY(int x, int y, std::string BodySegmentColor);
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
	bool destroyed = false;
	int WormSize = 4;
	int worm_num_;
	Apples* apples;
	GameScore* score;
	enum WormDirection { UP, RIGHT, DOWN, LEFT }; //0 1 2 3
	int LastWormDirection = UP;
	std::deque<BodySegment> worm_body;
};

#endif
