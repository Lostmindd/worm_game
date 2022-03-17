#ifndef APPLES_H
#define APPLES_H
#include <set>
class Apples
{
public:
	void CreateApples(int count); 
	void UpdateApple();
	void LockCell(int x, int y);
	void UnlockCell(int x, int y);
	Apples();
private:
	struct CellCoord
	{
		int x;
		int y;
		bool operator <(const CellCoord& other) const
		{
			return (this->x/ 2 + this->y*60) < (other.x / 2 + other.y * 60);
		}
	};
	std::set <CellCoord> empty_cells_;
};
#endif
