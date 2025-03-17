// main.cpp
#include <iostream>
#include <string>
#include <thread>
#include <conio.h>
#include "PlantsBoard.hpp"
#include "GameBoard.hpp"

using std::cout;
using std::string;

// Clears the console.
void static ClearScreen() {
#ifdef _WIN32
	system("CLS");
#else
	system("clear");
#endif
}

int main() {
	// Start
	// - Game loop logic
	bool gameLoop = true;
	int fps = 1000 / 10; // fps = 1000 ms / nº of fps
	string output = "";
	int auto_currency = 10;
	char plantSymbol = ' ';

	PlantsBoard plantsBoard;
	Coordenates plantsSelection{ 1 , 1 };

	GameBoard gameBoard;
	Coordenates gameSelection{ 0 , 1 };

	// testing limited gameloop
	int i = 10;

	// Update
	while (gameLoop) {
		ClearScreen();
		output = "";
		output += "Plants Board:\n" + plantsBoard.GenerateBoard(plantsSelection);
		output += "Q - Move to Left | E - Move to Right\n";
		output += "Game Board:\n" + gameBoard.GenerateBoard(gameSelection);
		output += "W - Move Up | A - Move Left | S - Move Down | D - Move Right\n";

		if (_kbhit()) {  // Check if a key has been pressed
			char key = _getch();  // Read the key without echo
			switch (key) {
			case 'q':
			case 'Q':
				// handle move left on the Plants Board
				plantsSelection.x--;
				break;
			case 'e':
			case 'E':
				// handle move right on the Plants Board
				plantsSelection.x++;
				break;
			case 'w':
			case 'W':
				// handle move up on the Game Board
				gameSelection.y -= 2;
				break;
			case 'a':
			case 'A':
				// handle move left on the Game Board
				gameSelection.x--;
				break;
			case 's':
			case 'S':
				// handle move down on the Game Board
				gameSelection.y += 2;
				break;
			case 'd':
			case 'D':
				// handle move right on the Game Board
				gameSelection.x++;
				break;
			case ' ':
				if (plantsBoard.Get_Currency() >= stoi(plantsBoard.GetCellContent(plantsSelection.x, 2))) {
					plantSymbol = plantsBoard.GetPlantSymbol(plantsSelection.x);
					plantsBoard.Set_Currency(plantsBoard.Get_Currency() - stoi(plantsBoard.GetCellContent(plantsSelection.x, 2)));
				}
				else {
					plantSymbol = ' ';
				}

				// If we got a valid symbol, place it on the game board.
				if (plantSymbol != ' ') {
					gameBoard.PlacePlant(gameSelection, plantSymbol);
				}
				break;
			case '\033':
				gameLoop = false;
				break;
			default:
				break;
			}
		}

		if (auto_currency <= 0) {
			plantsBoard.Set_Currency(plantsBoard.Get_Currency() + 1);
			auto_currency = 10;
		}
		else {
			auto_currency--;
		}

		// - Game loop logic
		cout << output;
		std::this_thread::sleep_for(std::chrono::milliseconds(fps));
	}

	// ClearScreen();
	cout << "Thanks for playing!" << "\n";

	return 0;
}
