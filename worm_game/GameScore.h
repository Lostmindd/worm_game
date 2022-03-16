#ifndef GameScore_H
#define GameScore_H
class GameScore
{
public:
	GameScore(int player_num, std::string names[]);
	~GameScore();

	void DrawStrip(int char_num, int size);

	void TableCreate(int* score_lines);

	void TableUpdate(int player, int score_correction);

	char GetPlayerNum();

	void ChangeOutputColor(int player);

	void ResetOutputColor();

private:
	std::string* names;
	char player_num;
	int* scores;
};

#endif