// main.cpp
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "PlantsBoard.hpp"
#include "GameBoard.hpp"

using std::cout;
using std::string;
using std::to_string;
using std::vector;

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

	// testing limited gameloop
	int i = 10;

	// Update
	while (gameLoop) {
		ClearScreen();
		output = "";
		output += PlantsBoard();
		output += GameBoard();

		// testing limited gameloop
		if (i > 0) {
			i--;
		}
		else {
			gameLoop = false;
		}

		// - Game loop logic
		cout << output;
		std::this_thread::sleep_for(std::chrono::milliseconds(fps));
	}
	ClearScreen();
	cout << "PlantsBoard: " << PlantsBoard() << "\n";
	cout << "Thanks for playing!" << "\n";

	return 0;
}
