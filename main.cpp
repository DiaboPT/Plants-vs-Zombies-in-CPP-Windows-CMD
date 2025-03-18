// main.cpp
#include <iostream>
#include <string>
#include <thread>
#include <conio.h>
#include <random>

#include "CellContent.hpp"
#include "PlantsBoard.hpp"
#include "GameBoard.hpp"

using std::cout;
using std::string;
using std::to_string;
using std::srand;

// Clears the console.
void static ClearScreen() {
#ifdef _WIN32
	system("CLS");
#else
	system("clear");
#endif
}

#include <windows.h>

// Moves cursor to the top-left instead of clearing the screen
void static ResetCursor() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(hOut, pos);
}

int main() {
	// Start
	// - Game loop logic
	bool gameLoop = true;
	int fps = 60;
	string output = "";

	int auto_currency = fps * 5;
	int zombie_speed = 0;
	CellContent symbol = CellContent();

	// - Boards
	PlantsBoard plantsBoard;
	Coordenates plantsSelection{ 1 , 0 };
	GameBoard gameBoard;
	Coordenates gameSelection{ 0 , 0 };

	// - Store the previous frame
	string prevBuffer = "";

	// Update - Generate game per frame
	while (gameLoop) {
		// Detects inputs
		string newBuffer = "Plants Board:\n" + plantsBoard.GenerateBoard(plantsSelection);
		newBuffer += "Q - Move to Left | E - Move to Right\n";
		newBuffer += "Game Board:\n" + gameBoard.GenerateBoard(gameSelection);
		newBuffer += "W - Move Up | A - Move Left | S - Move Down | D - Move Right\n";
		newBuffer += "Esc - Exit game | R - Restart game\n";

		// Only update if something changed (avoids unnecessary redraws)
		if (newBuffer != prevBuffer) {
			ResetCursor();
			cout << newBuffer;
			prevBuffer = newBuffer;
		}

		// Handle input
		if (_kbhit()) {
			char key = _getch();
			switch (key) {

			// Plants Selection
			case 'q': case 'Q': if (plantsSelection.x > 1) { plantsSelection.x--; }
					break;
			case 'e': case 'E': if (plantsSelection.x < 3) { plantsSelection.x++; }
					break;

			// Game Selection
			case 'w': case 'W': if (gameSelection.y > 0) { gameSelection.y--; }
					break;
			case 'a': case 'A': if (gameSelection.x > 0) { gameSelection.x--; }
					break;
			case 's': case 'S': if (gameSelection.y < 4) { gameSelection.y++; }
					break;
			case 'd': case 'D': if (gameSelection.x < 10) { gameSelection.x++; }
					break;

			// Game Options
			case ' ':
				// If you have currency
				if (plantsBoard.Get_Currency() >= stoi(plantsBoard.GetCellContent(plantsSelection.x, 1).Get_Name())) {
					// Get the Plant symbol from PlantsBoard
					symbol = plantsBoard.GetPlant(plantsSelection.x);
					// Reduce currency by the amount
					plantsBoard.Add_Currency(-plantsBoard.GetPlant(plantsSelection.x).Get_Cost());
					// Place the Plant
					if (symbol.Get_Name() != " ") gameBoard.PlaceInGrid(gameSelection, symbol); symbol = CellContent();
				}
				break;
			case 'r': case 'R': main(); return 0;
			case '\033': gameLoop = false; break;
			}
		}

		// Auto-generate currency every few frames
		if (auto_currency <= 0) {
			plantsBoard.Add_Currency(gameBoard.Get_Sunflowers());
			auto_currency = fps * 5;
		}
		else {
			auto_currency--;
		}

		// Generates a zombie if board has enough currency
		if (gameBoard.Get_Currency() >= gameBoard.GetZombie(1).Get_Cost() * fps) {
			// Get the symbol from GameBoard
			symbol = gameBoard.GetZombie(1);
			// Reduce currency by the amount
			gameBoard.Add_Currency(-gameBoard.GetZombie(1).Get_Cost());
			// Place the Zombie
			if (symbol.Get_Name() != " ") gameBoard.PlaceInGrid({ 3 , 3 }, symbol); symbol = CellContent();
		}
		else {
			gameBoard.Add_Currency(1);
		}

		// If zombie reachs the end, game over
		if (gameBoard.GetZombie(1).Get_Transform().x < 0) {
			gameLoop = false;
		}

		// FPS control
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
	}

	// ClearScreen();
	cout << "Thanks for playing!" << "\n";

	return 0;
}
