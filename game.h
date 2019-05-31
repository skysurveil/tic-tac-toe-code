// File : game.h
// Author : Skyler Routon
// Date : 5/29/19
// Description : Header for the game.cpp file
// (containing the prototypes and member variables)
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include "board.h"
using namespace std;

class Game
{
public:
	Game();
	bool Initialize(); // Initialize the game.
	void RunLoop(Board&); // Runs the game loop until game is over.
	void Shutdown(); // Shutdown the game.
private:
	// helper functions for the game loop.
	void ProcessInput(Board&);
	void UpdateGame(Board&);
	void GenerateOutput(Board&);
	// Window created by SDL
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	// additional functions
	void PlayerMove(Board&);
	void DisplayBoard(Board&);
	void winnerCheck(Board&);

	// game should continue to run
	bool mIsRunning;

	Uint32 mTicksCount; // for calculating delta time w/ GetTicks

	float deltaTime;
};