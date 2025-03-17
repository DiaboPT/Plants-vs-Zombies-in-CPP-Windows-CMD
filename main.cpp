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
	int auto_currency = fps * 10;
	char plantSymbol = ' ';

	// Boards
	PlantsBoard plantsBoard;
	Coordenates plantsSelection{ 1 , 1 };
	GameBoard gameBoard;
	Coordenates gameSelection{ 0 , 1 };

	// Store the previous frame
	string prevBuffer = "";

	// Update - Generate game per frame
	while (gameLoop) {
		// Detects inputs
		string newBuffer = "Plants Board:\n" + plantsBoard.GenerateBoard(plantsSelection);
		newBuffer += "Q - Move to Left | E - Move to Right\n";
		newBuffer += "Game Board:\n" + gameBoard.GenerateBoard(gameSelection);
		newBuffer += "W - Move Up | A - Move Left | S - Move Down | D - Move Right\n";

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
			case 'q': case 'Q': plantsSelection.x--; break;
			case 'e': case 'E': plantsSelection.x++; break;
			case 'w': case 'W': gameSelection.y -= 2; break;
			case 'a': case 'A': gameSelection.x--; break;
			case 's': case 'S': gameSelection.y += 2; break;
			case 'd': case 'D': gameSelection.x++; break;
			case ' ':
				// If you have currency
				if (plantsBoard.Get_Currency() >= stoi(plantsBoard.GetCellContent(plantsSelection.x, 2))) {
					// Get the Plant symbol from PlantsBoard
					plantSymbol = plantsBoard.GetPlantSymbol(plantsSelection.x);
					// Reduce currency by the amount
					plantsBoard.Set_Currency(plantsBoard.Get_Currency() - stoi(plantsBoard.GetCellContent(plantsSelection.x, 2)));
					// Place the Plant
					if (plantSymbol != ' ') gameBoard.PlacePlant(gameSelection, plantSymbol);
				}
				break;
			case '\033': gameLoop = false; break;
			}
		}

		// Auto-generate currency every few frames
		if (auto_currency <= 0) {
			plantsBoard.Set_Currency(plantsBoard.Get_Currency() + gameBoard.Get_Sunflowers());
			auto_currency = fps * 10;
		}
		else {
			auto_currency--;
		}

		// FPS control
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
	}

	ClearScreen();
	cout << "Thanks for playing!" << "\n";

	return 0;
}
