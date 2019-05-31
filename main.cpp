// File : main.cpp
// Author : Skyler Routon
// Date : 5/29/19
// Description : main function for tic tac toe game program.
#include "game.h"
//#include "board.h" // including caused redefinition error.
bool success = false;

int main(int argc, char* argv[])
{
	Game game;
	Board gameBoard;
	bool success = game.Initialize(); // start up game
	//game.Initialize(testBoard); // start up game

	if (success)
	{
		game.RunLoop(gameBoard);
	}
	game.Shutdown();
	return 0;
}