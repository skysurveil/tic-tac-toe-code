// File : game.cpp
// Author : Skyler Routon
// Date : 5/29/19
// Description : Contains the implementation for the game functs.
#include "game.h"
//#include "board.h" // header for all of the cells in the board.
const int windowWidth = 500;
const int windowHeight = 500;
int playerTurn = 0; // if % 2 == 0, player1 turn, else player2 turn.
bool player1Wins = false;
bool player2Wins = false;
bool tieGame = false;
bool testing = false; // for console debugging.
extern bool success;

enum position { // the 9 cells of the board (corresponds to vector of Cells)
	topLeft, // 0
	topMiddle, // 1
	topRight, // 2
	middleLeft, // 3
	middleCenter, // 4
	middleRight, // 5
	bottomLeft, // 6
	bottomMiddle, // 7
	bottomRight // 8
};

Game::Game() // game default constructor
{
	mWindow = nullptr;
	mRenderer = nullptr; // because compiler complained about lack of initialization
	mIsRunning = true;
	mTicksCount = 0;
	deltaTime = 0; // because compiler complained about lack of initialization
	player1WinsFlag = false;
	player2WinsFlag = false;
	tieGameFlag = false;
	player1WinnerString = "Player 1 Wins!";
	playAgainString = "Press R to play again";
	player2WinnerString = "Player 2 Wins!";
	tieString = "It's a tie!";
	mainMenuCheck = false; // while false, display main menu.
}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
		//exit(-1);
	}
	mWindow = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
	// Set window icon
	SDL_Surface* icon;
	icon = IMG_Load("icon.bmp");
	SDL_SetWindowIcon(mWindow, icon);
	SDL_FreeSurface(icon);
	if (!mWindow)
	{
		SDL_Log("Failed to create window %s", SDL_GetError());
		return false;
		//exit(-1);
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create a renderer %s", SDL_GetError());
		return false;
		//exit(-1);
	}
	int sdlTTFresult = TTF_Init();
	if (sdlTTFresult != 0)
	{
		SDL_Log("Unable to initialize SDL_ttf %s", SDL_GetError());
		return false;
		//exit(-1);
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		SDL_Log("Unable to initialize SDL_mixer %s", SDL_GetError());
		return false;
		//exit(-1);
	}
	int imageFlags = IMG_INIT_PNG;
	//int sdlImageResult = IMG_Init(imageFlags);
	if (!(IMG_Init(imageFlags) & imageFlags))
	{
		SDL_Log("Failed to initialize PNG support %s", IMG_GetError());
		return false;
	}
	// Initialize game board***

	return true;
}

void Game::RunLoop(Board &gameBoard)
{
	while (mIsRunning)
	{
		ProcessInput(gameBoard);
		UpdateGame(gameBoard);
		GenerateOutput(gameBoard);
	}
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::ProcessInput(Board &gameBoard)
{
	SDL_Event event;
	// while there are still events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// handles different types of events here
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			PlayerMove(gameBoard);
			playerTurn++; // incrementing playerTurn counter after each player takes turn.
		}
	}
	// get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape key is pressed, also end loop.
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	// more to come..

	if (state[SDL_SCANCODE_R])
	{
		for (int i = 0; i < 9; i++)
		{
			gameBoard.setOwnership(i, 0); // reset ownership to None.
			gameBoard.ResetBoard(i); // reset shown variable to false for each cell.
			// reset all endgame flags:
			player1Wins = false;
			player1WinsFlag = false;
			player2Wins = false;
			player2WinsFlag = false;
			tieGame = false;
			tieGameFlag = false;
			playerTurn = 0; // reset player counter.
		}
	}
	
}

void Game::UpdateGame(Board &gameBoard)
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	// Delta time is different in ticks from last frame
	// (converted to seconds)
	deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// clamp max. delta time value
	if (deltaTime < 0.05f)
	{
		deltaTime = 0.05f;
	}
	winnerCheck(gameBoard);
	if (player1Wins == true)
	{
		if (testing)
			cout << "Player 1 Wins!\n";
		player1WinsFlag = true;
		//mIsRunning = false; // Quit the application *** for now ***
	}
	if (player2Wins == true)
	{
		if (testing)
			cout << "Player 2 Wins!\n";
		player2WinsFlag = true;
		//mIsRunning = false; // Quit the application *** for now ***
	}
	if (tieGame == true)
	{
		if (testing)
			cout << "Tie game!\n";
		tieGameFlag = true;
		//mIsRunning = false; // Quit the application *** for now ***
	}

}

void Game::GenerateOutput(Board &gameBoard)
{
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // rgba
	SDL_RenderClear(mRenderer);
	// grid color based on game state:
	if (tieGame == true)
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255); // black for board layout (if tie game)
	else if (player1Wins == true)
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255); // blue for player 1
	else if (player2Wins == true)
		SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255); // red for player 2
	else if (playerTurn % 2 == 0)
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255); // blue for player 1's turn (X)
	else
		SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255); // red for player 2's turn (O)

	SDL_Rect leftVWall = { windowWidth / 3,0,10,windowHeight };
	SDL_RenderFillRect(mRenderer, &leftVWall);
	SDL_Rect rightVWall = { windowWidth - windowWidth / 3,0,10,windowHeight };
	SDL_RenderFillRect(mRenderer, &rightVWall);
	SDL_Rect topHWall = { 0,windowHeight / 3,windowWidth,10 };
	SDL_RenderFillRect(mRenderer, &topHWall);
	SDL_Rect bottomHWall = { 0, windowHeight - windowHeight / 3,windowWidth,10 };
	SDL_RenderFillRect(mRenderer, &bottomHWall);

	//SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);

	// AssembleVisibleCells function call goes here. returns vector of bools. (then 2nd vector w/ player info)
	vector<bool> visibleCells = gameBoard.AssembleVisibleCells();
	// Draw visible cells in grid.
	for (int i = 0; i < 9; i++)
	{
		if (visibleCells[i] == true)
		{
			int x = gameBoard.returnCellX(i);
			int y = gameBoard.returnCellY(i);
			int width = gameBoard.returnCellWidth(i);
			int height = gameBoard.returnCellHeight(i);
			SDL_Rect cell = { x, y, width, height };
			int cellOwnership = gameBoard.getOwnership(i);
			/* // non-image version (red & blue squares)
			if (cellOwnership == 1)
				SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255); // red for player 1
			else
				SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255); // blue for player 2
			SDL_RenderFillRect(mRenderer, &cell);
			*/
			if (cellOwnership == 1)
			{
				SDL_Surface* player1X;
				player1X = IMG_Load("x.png");
				SDL_Texture* player1Texture = SDL_CreateTextureFromSurface(mRenderer, player1X);
				SDL_Rect player1Rect = { gameBoard.returnCellX(i), gameBoard.returnCellY(i), gameBoard.returnCellWidth(i), gameBoard.returnCellHeight(i) };
				SDL_FreeSurface(player1X);
				SDL_RenderCopy(mRenderer, player1Texture, NULL, &player1Rect);
				SDL_DestroyTexture(player1Texture);
			}
			else
			{
				SDL_Surface* player2O;
				player2O = IMG_Load("o.png");
				SDL_Texture* player2Texture = SDL_CreateTextureFromSurface(mRenderer, player2O);
				SDL_Rect player2Rect = { gameBoard.returnCellX(i), gameBoard.returnCellY(i), gameBoard.returnCellWidth(i), gameBoard.returnCellHeight(i) };
				SDL_FreeSurface(player2O);
				SDL_RenderCopy(mRenderer, player2Texture, NULL, &player2Rect);
				SDL_DestroyTexture(player2Texture);
			}
		}
	}
	//DisplayBoard(gameBoard);
	if (player1WinsFlag) // == true
	{
		TTF_Font* textFont = TTF_OpenFont("Sans2.ttf", 36);
		// "Player 1 Wins!" text:
		int textWidth, textHeight;
		TTF_SizeText(textFont, "Player 1 Wins!", &textWidth, &textHeight);
		if (testing)
			cout << "Text width is: " << textWidth << ", text height is: " << textHeight << endl;
		SDL_Color textColor;
		textColor.r = 0; textColor.g = 0; textColor.b = 0; textColor.a = 255; // black for text
		SDL_Color bgColor;
		bgColor.r = 255; bgColor.g = 255; bgColor.b = 255; bgColor.a = 255; // white for background.
		SDL_Rect winnerRect;
		winnerRect.x = (windowWidth / 2) - (textWidth / 2);
		winnerRect.y = (float)(windowHeight / 2) - (float)(textHeight * 1.25);
		const char* cStringWinner1 = player1WinnerString.c_str();
		SDL_Surface* surfaceWinner1 = TTF_RenderText_Shaded(textFont, cStringWinner1, textColor, bgColor); // _solid for transparent text, _shaded for non transparent.
		SDL_Texture* Winner1 = SDL_CreateTextureFromSurface(mRenderer, surfaceWinner1);
		SDL_QueryTexture(Winner1, nullptr, nullptr, &winnerRect.w, &winnerRect.h);
		SDL_FreeSurface(surfaceWinner1);
		SDL_RenderCopy(mRenderer, Winner1, NULL, &winnerRect);
		// "Press R to play again" text:
		int playAgainWidth, playAgainHeight;
		TTF_SizeText(textFont, "Press R to play again", &playAgainWidth, &playAgainHeight);
		if (testing)
			cout << "Text width is: " << playAgainWidth << ", text height is: " << playAgainHeight << endl;
		SDL_Rect playAgainRect;
		playAgainRect.x = (windowWidth / 2) - (playAgainWidth / 2);
		playAgainRect.y = winnerRect.y + textHeight;
		const char* cStringPlayAgain = playAgainString.c_str();
		SDL_Surface* surfacePlayAgain = TTF_RenderText_Shaded(textFont, cStringPlayAgain, textColor, bgColor);
		SDL_Texture* PlayAgain = SDL_CreateTextureFromSurface(mRenderer, surfacePlayAgain);
		SDL_QueryTexture(PlayAgain, nullptr, nullptr, &playAgainRect.w, &playAgainRect.h);
		SDL_FreeSurface(surfacePlayAgain);
		SDL_RenderCopy(mRenderer, PlayAgain, NULL, &playAgainRect);

		SDL_DestroyTexture(Winner1);
		SDL_DestroyTexture(PlayAgain);
		TTF_CloseFont(textFont);

	}
	if (player2WinsFlag)
	{
		TTF_Font* textFont = TTF_OpenFont("Sans2.ttf", 36);
		// "Player 2 Wins!" text:
		int textWidth, textHeight;
		TTF_SizeText(textFont, "Player 2 Wins!", &textWidth, &textHeight);
		if (testing)
			cout << "Text width is: " << textWidth << ", text height is: " << textHeight << endl;
		SDL_Color textColor;
		textColor.r = 0; textColor.g = 0; textColor.b = 0; textColor.a = 255; // black for text
		SDL_Color bgColor;
		bgColor.r = 255; bgColor.g = 255; bgColor.b = 255; bgColor.a = 255; // white for background.
		SDL_Rect winnerRect;
		winnerRect.x = (windowWidth / 2) - (textWidth / 2);
		winnerRect.y = (float)(windowHeight / 2) - (float)(textHeight * 1.25);
		const char* cStringWinner2 = player2WinnerString.c_str();
		SDL_Surface* surfaceWinner2 = TTF_RenderText_Shaded(textFont, cStringWinner2, textColor, bgColor); // _solid for transparent text, _shaded for non transparent.
		SDL_Texture* Winner2 = SDL_CreateTextureFromSurface(mRenderer, surfaceWinner2);
		SDL_QueryTexture(Winner2, nullptr, nullptr, &winnerRect.w, &winnerRect.h);
		SDL_FreeSurface(surfaceWinner2);
		SDL_RenderCopy(mRenderer, Winner2, NULL, &winnerRect);
		// "Press R to play again" text:
		int playAgainWidth, playAgainHeight;
		TTF_SizeText(textFont, "Press R to play again", &playAgainWidth, &playAgainHeight);
		if (testing)
			cout << "Text width is: " << playAgainWidth << ", text height is: " << playAgainHeight << endl;
		SDL_Rect playAgainRect;
		playAgainRect.x = (windowWidth / 2) - (playAgainWidth / 2);
		playAgainRect.y = winnerRect.y + textHeight;
		const char* cStringPlayAgain = playAgainString.c_str();
		SDL_Surface* surfacePlayAgain = TTF_RenderText_Shaded(textFont, cStringPlayAgain, textColor, bgColor);
		SDL_Texture* PlayAgain = SDL_CreateTextureFromSurface(mRenderer, surfacePlayAgain);
		SDL_QueryTexture(PlayAgain, nullptr, nullptr, &playAgainRect.w, &playAgainRect.h);
		SDL_FreeSurface(surfacePlayAgain);
		SDL_RenderCopy(mRenderer, PlayAgain, NULL, &playAgainRect);

		SDL_DestroyTexture(Winner2);
		SDL_DestroyTexture(PlayAgain);
		TTF_CloseFont(textFont);
	}
	if (tieGameFlag)
	{
		TTF_Font* textFont = TTF_OpenFont("Sans2.ttf", 36);
		// "It's a tie!" text:
		int textWidth, textHeight;
		TTF_SizeText(textFont, "It's a tie!", &textWidth, &textHeight);
		if (testing)
			cout << "Text width is: " << textWidth << ", text height is: " << textHeight << endl;
		SDL_Color textColor;
		textColor.r = 0; textColor.g = 0; textColor.b = 0; textColor.a = 255; // black for text
		SDL_Color bgColor;
		bgColor.r = 255; bgColor.g = 255; bgColor.b = 255; bgColor.a = 255; // white for background.
		SDL_Rect winnerRect;
		winnerRect.x = (windowWidth / 2) - (textWidth / 2);
		winnerRect.y = (float)(windowHeight / 2) - (float)(textHeight * 1.25);
		const char* cStringTie = tieString.c_str();
		SDL_Surface* surfaceTie = TTF_RenderText_Shaded(textFont, cStringTie, textColor, bgColor); // _solid for transparent text, _shaded for non transparent.
		SDL_Texture* Tie = SDL_CreateTextureFromSurface(mRenderer, surfaceTie);
		SDL_QueryTexture(Tie, nullptr, nullptr, &winnerRect.w, &winnerRect.h);
		SDL_FreeSurface(surfaceTie);
		SDL_RenderCopy(mRenderer, Tie, NULL, &winnerRect);
		// "Press R to play again" text:
		int playAgainWidth, playAgainHeight;
		TTF_SizeText(textFont, "Press R to play again", &playAgainWidth, &playAgainHeight);
		if (testing)
			cout << "Text width is: " << playAgainWidth << ", text height is: " << playAgainHeight << endl;
		SDL_Rect playAgainRect;
		playAgainRect.x = (windowWidth / 2) - (playAgainWidth / 2);
		playAgainRect.y = winnerRect.y + textHeight;
		const char* cStringPlayAgain = playAgainString.c_str();
		SDL_Surface* surfacePlayAgain = TTF_RenderText_Shaded(textFont, cStringPlayAgain, textColor, bgColor);
		SDL_Texture* PlayAgain = SDL_CreateTextureFromSurface(mRenderer, surfacePlayAgain);
		SDL_QueryTexture(PlayAgain, nullptr, nullptr, &playAgainRect.w, &playAgainRect.h);
		SDL_FreeSurface(surfacePlayAgain);
		SDL_RenderCopy(mRenderer, PlayAgain, NULL, &playAgainRect);

		SDL_DestroyTexture(Tie);
		SDL_DestroyTexture(PlayAgain);
		TTF_CloseFont(textFont);
	}

	if (!mainMenuCheck)
	{
		const Uint8* state = SDL_GetKeyboardState(NULL);
		SDL_Surface* mainMenu;
		mainMenu = IMG_Load("mainMenu.png");
		SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(mRenderer, mainMenu);
		SDL_Rect mainMenuRect = { 0, 0, windowWidth, windowHeight };
		SDL_FreeSurface(mainMenu);
		SDL_RenderCopy(mRenderer, menuTexture, NULL, &mainMenuRect);
		SDL_DestroyTexture(menuTexture);
		if (state[SDL_SCANCODE_RETURN])
		{
			mainMenuCheck = true;
		}
	}

	// final line, renderpresent.
	SDL_RenderPresent(mRenderer);
}

void Game::PlayerMove(Board &gameBoard)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if ((x < 500 && x > 0) && (y < 500 && y > 0)) // valid click
	{
		if (testing)
			cout << "Mouse click registered " << x << ", " << y << endl;
	}
	if ((x > 0 && x < 165) && (y > 0 && y < 165)) // top left
	{
		if (testing)
			cout << "Top Left Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(topLeft);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(topLeft);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(topLeft, 1); // 1 == player1
		else
			gameBoard.setOwnership(topLeft, 2); // 2 == player2
	}
	else if ((x > 175 && x < 334) && (y > 0 && y < 165)) // top center
	{
		if (testing)
			cout << "Top Center Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(topMiddle);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(topMiddle);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(topMiddle, 1); // 1 == player1
		else
			gameBoard.setOwnership(topMiddle, 2); // 2 == player2
	}
	else if ((x > 344 && x < 500) && (y > 0 && y < 165)) // top right
	{
		if (testing)
			cout << "Top Right Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(topRight);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(topRight);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(topRight, 1); // 1 == player1
		else
			gameBoard.setOwnership(topRight, 2); // player2
	}
	else if ((x > 0 && x < 165) && (y > 175 && y < 334)) // middle left
	{
		if (testing)
			cout << "Middle Left Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(middleLeft);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(middleLeft);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(middleLeft, 1); // player1
		else
			gameBoard.setOwnership(middleLeft, 2); // player2
	}
	else if ((x > 175 && x < 334) && (y > 175 && y < 334)) // middle center
	{
		if (testing)
			cout << "Middle Center Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(middleCenter);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(middleCenter);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(middleCenter, 1); // player1
		else
			gameBoard.setOwnership(middleCenter, 2); // player2
	}
	else if ((x > 344 && x < 500) && (y > 175 && y < 334)) // middle right
	{
		if (testing)
			cout << "Middle Right Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(middleRight);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(middleRight);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(middleRight, 1); // 1
		else
			gameBoard.setOwnership(middleRight, 2); // 2
	}
	else if ((x > 0 && x < 165) && (y > 334 && y < 500)) // bottom left
	{
		if (testing)
			cout << "Bottom Left Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(bottomLeft);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(bottomLeft);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(bottomLeft, 1); // 1
		else
			gameBoard.setOwnership(bottomLeft, 2); // 2
	}
	else if ((x > 175 && x < 334) && (y > 334 && y < 500)) // bottom middle
	{
		if (testing)
			cout << "Bottom Middle Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(bottomMiddle);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(bottomMiddle);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(bottomMiddle, 1); // 1
		else
			gameBoard.setOwnership(bottomMiddle, 2); // 2
	}
	else if ((x > 334 && x < 500) && (y > 334 && y < 500)) // bottom right
	{
		if (testing)
			cout << "Bottom Right Space selected\n";
		// Check to make sure this position has not been claimed yet.
		int playedCheck = gameBoard.getOwnership(bottomRight);
		if (playedCheck != 0)
			return;
		gameBoard.UpdateBoard(bottomRight);
		if (playerTurn % 2 == 0)
			gameBoard.setOwnership(bottomRight, 1); // 1
		else
			gameBoard.setOwnership(bottomRight, 2); // 2
	}
	else
	{
		if (testing)
			cout << "Not a valid click\n";
	}
	// 
}

/*
void Game::DisplayBoard(Board &gameBoard)
{
	for (int i = 0; i < 9; i++)
	{
		//if (gameBoard.UpdateBoard())//if (gameBoard->boardVector[i].shown == true)
		{

		}
	}
}
*/

// still need to add in player turns to determine color of cells..
// -- and add member variable to the cells specifying which player has ownership..

void Game::winnerCheck(Board& gameBoard)
{
	// visibility variables for solution checks
	bool topLeftShown = gameBoard.isShown(topLeft); // using enumerated type for cell location
	bool topCenterShown = gameBoard.isShown(topMiddle);
	bool topRightShown = gameBoard.isShown(topRight);
	bool middleLeftShown = gameBoard.isShown(middleLeft);
	bool middleCenterShown = gameBoard.isShown(middleCenter);
	bool middleRightShown = gameBoard.isShown(middleRight);
	bool bottomLeftShown = gameBoard.isShown(bottomLeft);
	bool bottomCenterShown = gameBoard.isShown(bottomMiddle);
	bool bottomRightShown = gameBoard.isShown(bottomRight);
	// ownership variables for solution checks
	int topLeftOwnership = gameBoard.getOwnership(topLeft);
	int topCenterOwnership = gameBoard.getOwnership(topMiddle);
	int topRightOwnership = gameBoard.getOwnership(topRight);
	int middleLeftOwnership = gameBoard.getOwnership(middleLeft);
	int middleCenterOwnership = gameBoard.getOwnership(middleCenter);
	int middleRightOwnership = gameBoard.getOwnership(middleRight);
	int bottomLeftOwnership = gameBoard.getOwnership(bottomLeft);
	int bottomCenterOwnership = gameBoard.getOwnership(bottomMiddle);
	int bottomRightOwnership = gameBoard.getOwnership(bottomRight);
	// vertical left solution check
	if (topLeftShown && middleLeftShown && bottomLeftShown && topLeftOwnership == 1 && middleLeftOwnership == 1 && bottomLeftOwnership == 1)
		player1Wins = true;
	else if (topLeftShown && middleLeftShown && bottomLeftShown && topLeftOwnership == 2 && middleLeftOwnership == 2 && bottomLeftOwnership == 2)
		player2Wins = true;
	// vertical center solution check
	else if (topCenterShown && middleCenterShown && bottomCenterShown && topCenterOwnership == 1 && middleCenterOwnership == 1 && bottomCenterOwnership == 1)
		player1Wins = true;
	else if (topCenterShown && middleCenterShown && bottomCenterShown && topCenterOwnership == 2 && middleCenterOwnership == 2 && bottomCenterOwnership == 2)
		player2Wins = true;
	// vertical right solution check
	else if (topRightShown && middleRightShown && bottomRightShown && topRightOwnership == 1 && middleRightOwnership == 1 && bottomRightOwnership == 1)
		player1Wins = true;
	else if (topRightShown && middleRightShown && bottomRightShown && topRightOwnership == 2 && middleRightOwnership == 2 && bottomRightOwnership == 2)
		player2Wins = true;
	// horizontal top solution check
	else if (topLeftShown && topCenterShown && topRightShown && topLeftOwnership == 1 && topCenterOwnership == 1 && topRightOwnership == 1)
		player1Wins = true;
	else if (topLeftShown && topCenterShown && topRightShown && topLeftOwnership == 2 && topCenterOwnership == 2 && topRightOwnership == 2)
		player2Wins = true;
	// horizontal center solution check
	else if (middleLeftShown && middleCenterShown && middleRightShown && middleLeftOwnership == 1 && middleCenterOwnership == 1 && middleRightOwnership == 1)
		player1Wins = true;
	else if (middleLeftShown && middleCenterShown && middleRightShown && middleLeftOwnership == 2 && middleCenterOwnership == 2 && middleRightOwnership == 2)
		player2Wins = true;
	// horizontal bottom solution check
	else if (bottomLeftShown && bottomCenterShown && bottomRightShown && bottomLeftOwnership == 1 && bottomCenterOwnership == 1 && bottomRightOwnership == 1)
		player1Wins = true;
	else if (bottomLeftShown && bottomCenterShown && bottomRightShown && bottomLeftOwnership == 2 && bottomCenterOwnership == 2 && bottomRightOwnership == 2)
		player2Wins = true;
	// diagonal top left to bottom right solution check
	else if (topLeftShown && middleCenterShown && bottomRightShown && topLeftOwnership == 1 && middleCenterOwnership == 1 && bottomRightOwnership == 1)
		player1Wins = true;
	else if (topLeftShown && middleCenterShown && bottomRightShown && topLeftOwnership == 2 && middleCenterOwnership == 2 && bottomRightOwnership == 2)
		player2Wins = true;
	// diagonal top right to bottom left solution check
	else if (topRightShown && middleCenterShown && bottomLeftShown && topRightOwnership == 1 && middleCenterOwnership == 1 && bottomLeftOwnership == 1)
		player1Wins = true;
	else if (topRightShown && middleCenterShown && bottomLeftShown && topRightOwnership == 1 && middleCenterOwnership == 1 && bottomLeftOwnership == 1)
		player2Wins = true;
	else if (topLeftShown && topCenterShown && topRightShown && middleLeftShown && middleCenterShown && middleRightShown && bottomLeftShown && bottomCenterShown && bottomRightShown)
		tieGame = true;
	else
		return;
}

// board:
// 0 1 2
// 3 4 5
// 6 7 8