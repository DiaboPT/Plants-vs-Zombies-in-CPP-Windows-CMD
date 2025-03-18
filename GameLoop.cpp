// GameLoop.cpp
#include "header.hpp"

// Clears the console.
void static ClearScreen() {
#ifdef _WIN32
	system("CLS");
#else
	system("clear");
#endif
}

// Moves cursor to the top-left instead of clearing the screen
void static ResetCursor() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(hOut, pos);
}

struct Coords {
	int x, y;
};

struct Transform {
	Coords position, size;
};

class CellContent {
private:
	string name;

	int cost;
	int hp;
	int speed;

public:
	// Empty
	CellContent() {
		Set_Name(" ");
		Set_Cost(0);
		Set_HP(0);
		Set_Speed(0);
	}

	// Plants
	CellContent(string new_name, int new_cost, int new_hp) {
		Set_Name(new_name);
		Set_Cost(new_cost);
		Set_HP(new_hp);
		Set_Speed(0);
	}

	// Zombies
	CellContent(string new_name, int new_cost, int new_hp, int new_speed) {
		Set_Name(new_name);
		Set_Cost(new_cost);
		Set_HP(new_hp);
		Set_Speed(new_speed);
	}

	void Set_Name(string value) {
		name = value;
	}
	string Get_Name() {
		return name;
	}

	void Set_Cost(int value) {
		cost = value;
	}
	void Add_Cost(int value) {
		cost += value;
	}
	int Get_Cost() {
		return cost;
	}

	void Set_HP(int value) {
		hp = value;
	}
	void Add_HP(int value) {
		hp += value;
	}
	int Get_HP() {
		return hp;
	}

	void Set_Speed(int value) {
		speed = value;
	}
	int Get_Speed() {
		return speed;
	}

};

#include <vector>

class GameBoard {
private:
	Transform grid;
	std::vector<std::vector<CellContent>> cell;  // Use vector of vectors for dynamic allocation

public:
	GameBoard(int x, int y) : cell(x, std::vector<CellContent>(y)) {  // Initialize the vector with size x * y
		grid = { { 0 , 0 }, { x , y } };

		for (grid.position.y = 0; grid.position.y < grid.size.y; grid.position.y++) {
			for (grid.position.x = 0; grid.position.x < grid.size.x; grid.position.x++) {
				Set_Cell(grid.position.x, grid.position.y, CellContent());
			}
		}
	}

	string DrawBoard(Coords selected) {
		string returned = "";

		returned += "+";
		for (grid.position.x = 0; grid.position.x < grid.size.x; grid.position.x++) {
			returned += "---+";
		}
		returned += '\n';

		for (grid.position.y = 0; grid.position.y < grid.size.y; grid.position.y++) {
			for (grid.position.x = 0; grid.position.x < grid.size.x; grid.position.x++) {
				returned += "| ";
				if (selected.x == grid.position.x && selected.y == grid.position.y) {
					returned += "X";
				}
				else {
					returned += cell[grid.position.x][grid.position.y].Get_Name();
				}
				returned += " ";
			}
			returned += "|";
			returned += '\n';

			returned += "+";
			for (grid.position.x = 0; grid.position.x < grid.size.x; grid.position.x++) {
				returned += "---+";
			}
			returned += '\n';
		}

		return returned;
	}

	void Set_Cell(int x, int y, CellContent value) {
		cell[x][y] = value;
	}

	CellContent Get_Cell(int x, int y) {
		return cell[x][y];
	}

	CellContent Get_Cell(Coords coords) {
		return cell[coords.x][coords.y];
	}
};

void GameLoop() {
	// Start
	bool gameloop = true;
	int fps = 60;
	int frameCount = 0;  // Frame counter
	string output = "";
	string old_output = "";
	auto lastFrameTime = chrono::steady_clock::now();

	// Importante cells
	CellContent Nothing = CellContent();
	CellContent PlantCurrency = CellContent("C", 2, 0);
	CellContent ZombieCurrency = CellContent("Z", 2, 0);
	int zombieTimeCount = 1;

	// Plants cells
	CellContent Sunflower = CellContent("S", 2, 3);
	CellContent Peashooter = CellContent("P", 4, 3);
	CellContent Wall_Nut = CellContent("W", 2, 9);

	// Zombies cells
	CellContent zombies[15];
	for (int i = 0; i < 15; i++) {
		zombies[i] = Nothing;
	}

	CellContent Basic = CellContent("B", 4, 3, 1);
	CellContent ConedHead = CellContent("C", Basic.Get_Cost() * 2, Basic.Get_HP() * 2, 1);

	// Plants Board Initialization
	int plantBoardWidth = 7;
	int plantBoardHeight = 2;
	Coords plantBoardSelection{ 2 , 0 };

	GameBoard plantsBoard = GameBoard(plantBoardWidth, plantBoardHeight);
	for (int j = 0; j < plantBoardHeight; j++) {
		for (int i = 0; i < plantBoardWidth; i++) {
			switch (i) {
			case 0:
				plantsBoard.Set_Cell(i, j, j == 0 ? PlantCurrency : CellContent());
				break;
			case 1:
				plantsBoard.Set_Cell(i, j, j == 0 ? CellContent() : CellContent());
				break;
			case 2:
				plantsBoard.Set_Cell(i, j, j == 0 ? Sunflower : CellContent(to_string(Sunflower.Get_Cost()), Sunflower.Get_Cost(), 0));
				break;
			case 3:
				plantsBoard.Set_Cell(i, j, j == 0 ? Peashooter : CellContent(to_string(Peashooter.Get_Cost()), Peashooter.Get_Cost(), 0));
				break;
			case 4:
				plantsBoard.Set_Cell(i, j, j == 0 ? Wall_Nut : CellContent(to_string(Wall_Nut.Get_Cost()), Wall_Nut.Get_Cost(), 0));
				break;
			case 5:
				plantsBoard.Set_Cell(i, j, j == 0 ? CellContent() : CellContent());
				break;
			case 6:
				plantsBoard.Set_Cell(i, j, j == 0 ? ZombieCurrency : CellContent());
				break;
			}
		}
	}

	// Game Board Initialization
	int gameBoardWidth = 9;
	int gameBoardHeight = 5;
	Coords gameBoardSelection{ 0 , 0 };
	GameBoard gameBoard = GameBoard(gameBoardWidth, gameBoardHeight);

	// Update
	while (gameloop) {
		auto currentTime = chrono::steady_clock::now();
		auto deltaTime = chrono::duration_cast<chrono::milliseconds>(currentTime - lastFrameTime).count();

		output = "";

		if (deltaTime >= 1000 / fps) {

			// Handle input
			if (_kbhit()) {
				char key = _getch();
				switch (key) {

					// Plants Selection
				case 'q': case 'Q': if (plantBoardSelection.x > 2) plantBoardSelection.x--;
					break;
				case 'e': case 'E': if (plantBoardSelection.x < 4) plantBoardSelection.x++;
					break;

					// Game Selection
				case 'w': case 'W': if (gameBoardSelection.y > 0) gameBoardSelection.y--;
					break;
				case 'a': case 'A': if (gameBoardSelection.x > 0) gameBoardSelection.x--;
					break;
				case 's': case 'S': if (gameBoardSelection.y < 4) gameBoardSelection.y++;
					break;
				case 'd': case 'D': if (gameBoardSelection.x < 10) gameBoardSelection.x++;
					break;

					// Game Options
				case ' ':
					// If you have currency
					if (PlantCurrency.Get_Cost() >= plantsBoard.Get_Cell(plantBoardSelection.x, 1).Get_Cost()) {
						CellContent newCell;
						if (plantsBoard.Get_Cell(plantBoardSelection).Get_Name() == "S") {
							newCell = Sunflower;
						}
						if (plantsBoard.Get_Cell(plantBoardSelection).Get_Name() == "P") {
							newCell = Peashooter;
						}
						if (plantsBoard.Get_Cell(plantBoardSelection).Get_Name() == "W") {
							newCell = Wall_Nut;
						}
						gameBoard.Set_Cell(gameBoardSelection.x, gameBoardSelection.y, newCell);
						PlantCurrency.Add_Cost(-plantsBoard.Get_Cell(plantBoardSelection.x, 1).Get_Cost());
					}
					break;
				case '\033': gameloop = false; break;
				}
			}

			output += "Plants Board:\n";
			plantsBoard.Set_Cell(0, 1, CellContent(to_string(PlantCurrency.Get_Cost()), PlantCurrency.Get_Cost(), 0));
			plantsBoard.Set_Cell(6, 1, CellContent(to_string(ZombieCurrency.Get_Cost()), ZombieCurrency.Get_Cost(), 0));
			output += plantsBoard.DrawBoard(plantBoardSelection);
			output += '\n';

			output += "Game Board:\n";
			if (ZombieCurrency.Get_Cost() >= Basic.Get_Cost()) {
				gameBoard.Set_Cell(3, 3, Basic);
				ZombieCurrency.Add_Cost(-Basic.Get_Cost());
			}
			output += gameBoard.DrawBoard(gameBoardSelection);
			output += '\n';

			if (output != old_output) {
				ResetCursor();
				cout << output;
				old_output = output;
			}

			// Update every 10 frames
			if ((frameCount % (fps * 10)) == 0) {
				int sunflowerCount = 1;
				for (int y = 0; y < gameBoardHeight; y++) {
					for (int x = 0; x < gameBoardWidth; x++) {
						if (gameBoard.Get_Cell(x, y).Get_Name() == "S") {
							sunflowerCount++;
						}
					}
				}
				PlantCurrency.Add_Cost(sunflowerCount);  // Increase cost based on Sunflowers
				ZombieCurrency.Add_Cost(zombieTimeCount);
				zombieTimeCount++;
			}
			frameCount++;  // Increment frame count
			lastFrameTime = currentTime;
		}
	}

	cout << "\033[2J\033[1;1H";
	cout << "Thanks for playing!\n";
}
