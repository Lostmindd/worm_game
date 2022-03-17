#ifndef GameScore_H
#define GameScore_H
class GameScore
{
public:
	GameScore(int player_num, std::string names[]);
	~GameScore();
	void TableCreate(int* score_lines);
	void TableUpdateScore(int player, int score_correction);
	void TableUpdateTurn(int player);
	char GetPlayerNum();
	void GameOver();
private:
	void ChangeOutputColor(int player);
	void DrawStrip(int char_num, int size);
	void ResetOutputColor();
	wchar_t GetCharacterXY(int x, int y);
	std::string* names_;
	int player_num_;
	int* scores_;
};
#endif