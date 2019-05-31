// File : board.cpp
// Author : Skyler Routon
// Date : 5/29/19
// Description : Contains the implementation for the board functions.
#include "board.h"
#include <iostream>

Board::Board()
{
	numberOfCells = 0;
	for (int i = 0; i < 9; i++)
	{
		Cell boardCell;
		boardCell.width = 156; // 166
		boardCell.height = 156; // 166 original - 166 final. (now 156, matching width for final images)
		boardCell.cellColor.r = 255;
		boardCell.cellColor.g = 0;
		boardCell.cellColor.b = 0;
		boardCell.shown = false;
		boardCell.ownership = 0; // 0 = None.
		if (i == 0) // top left
		{
			boardCell.x = 10; // 0 originally before fix.
			boardCell.y = 10; // 0 before fix.
		}
		else if (i == 1) // top middle
		{
			boardCell.x = 177; // 176 -> 177
			boardCell.y = 10; // (0 before fix, +10)
		}
		else if (i == 2) // top right
		{
			boardCell.x = 344;
			boardCell.y = 10;
		}
		else if (i == 3) // middle left
		{
			boardCell.x = 10; // 0 before fix (+10)
			boardCell.y = 177; // 176 -> 177
		}
		else if (i == 4) // middle center
		{
			boardCell.x = 177; // 176 -> 177
			boardCell.y = 177;
		}
		else if (i == 5) // middle right
		{
			boardCell.x = 345; // 344 -> 345
			boardCell.y = 177; // 175 -> 177
		}
		else if (i == 6) // bottom left
		{
			boardCell.x = 10; // (+10)
			boardCell.y = 344;
		}
		else if (i == 7) // bottom middle
		{
			boardCell.x = 177; // 176 -> 177
			boardCell.y = 344; // 344
		}
		else if (i == 8) // bottom right
		{
			boardCell.x = 344;
			boardCell.y = 344;
		}
		else
			cerr << "Out of bounds -> board constructor\n";
		boardVector.push_back(boardCell);
		numberOfCells++;
	}
}

void Board::UpdateBoard(int position) // sets specific cell to shown = true.
{
	if (this->numberOfCells != 9)
		cout << "Incorrect number of cells\n";
	this->boardVector[position].shown = true;
}

vector<bool> Board::AssembleVisibleCells()
{
	vector<bool> visibleCells;
	for (int i = 0; i < 9; i++)
	{
		if (this->boardVector[i].shown == true)
			visibleCells.push_back(true);
		else
			visibleCells.push_back(false);
	}
	return visibleCells;
}

int Board::returnCurrentCellX(int i)
{
	return this->boardVector[i].x;
}

int Board::returnCurrentCellY(int i)
{
	return this->boardVector[i].y;
}

int Board::returnCellX(int i)
{
	return returnCurrentCellX(i);
}

int Board::returnCellY(int i)
{
	return returnCurrentCellY(i);
}

int Board::returnCurrentCellWidth(int i)
{
	return this->boardVector[i].width;
}

int Board::returnCurrentCellHeight(int i)
{
	return this->boardVector[i].height;
}

int Board::returnCellWidth(int i)
{
	return returnCurrentCellWidth(i);
}

int Board::returnCellHeight(int i)
{
	return returnCurrentCellHeight(i);
}

void Board::setCellOwnership(int cell,int currentPlayer)
{
	this->boardVector[cell].ownership = currentPlayer;
}

void Board::setOwnership(int cell, int currentPlayer)
{
	setCellOwnership(cell, currentPlayer);
}

int Board::getCellOwnership(int cell)
{
	return this->boardVector[cell].ownership;
}

int Board::getOwnership(int cell)
{
	return getCellOwnership(cell);
}

bool Board::isCellShown(int cell)
{
	if (this->boardVector[cell].shown == true)
		return true;
	else
		return false;
}

bool Board::isShown(int cell)
{
	return isCellShown(cell);
}