// File : board.h
// Author : Skyler Routon
// Date : 5/29/19
// Description : Header for the board.cpp file
// (containing the prototypes and member variables)
#include <vector>
using namespace std;

class Board
{
public:
	Board();
	void UpdateBoard(int); // Update specific cell to be shown. false -> true
	void ResetBoard(int); // reset specific cell to be hidden. true -> false
	vector<bool> AssembleVisibleCells(); // returns bool vector (true = visible, false = invisible).
	// functions to retrieve private variable info:
	int returnCellX(int);
	int returnCellY(int);
	int returnCellWidth(int);
	int returnCellHeight(int);
	void setOwnership(int,int); // cell number, ownership type.
	int getOwnership(int);
	bool isShown(int);

private:
	struct Color
	{
		int r, g, b;
	};
	struct Cell
	{
		int x; // x coord. of cell
		int y; // y coord. of cell
		int width; // width of cell
		int height; // height of cell
		bool shown; // shown flag for output
		Color cellColor; // color of cell
		int ownership; // 0 = NONE, 1 = Player1 (x), 2 = Player2 (y).
	};
	vector<Cell> boardVector;
	int numberOfCells;
	
	int returnCurrentCellX(int);
	int returnCurrentCellY(int);
	int returnCurrentCellWidth(int);
	int returnCurrentCellHeight(int);
	void setCellOwnership(int,int); // cell number, ownership type
	int getCellOwnership(int);
	bool isCellShown(int);
};