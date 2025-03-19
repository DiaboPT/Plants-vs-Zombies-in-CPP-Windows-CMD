// GameLoop.cpp
#include "header.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

using namespace std;

class Random {
public:
	static int Range(int min, int max) {
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(min, max);
		return dist(mt);
	}

	static int FromList(const std::vector<int>& numbers) {
		if (numbers.empty()) return 0;
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(0, numbers.size() - 1);
		return numbers[dist(mt)];
	}
};

// Clears the console.
void static ClearScreen() {
	cout << "\033[2J\033[H"; // ANSI escape sequence for clearing screen and moving cursor to top-left
}

// Moves cursor to the top-left instead of clearing the screen
void static ResetCursor() {
	cout << "\033[H"; // ANSI escape sequence for resetting cursor
}

// Cross-platform kbhit() equivalent for Linux
static bool isKeyPressed() {
#ifdef _WIN32
	return _kbhit();
#else
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if (ch != EOF) {
		ungetc(ch, stdin);
		return true;
	}

	return false;
#endif
}

// Cross-platform getch() equivalent for Linux
static char getKeyPressed() {
#ifdef _WIN32
	return _getch();
#else
	struct termios oldt, newt;
	char ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
#endif
}

// Structs for game components
struct Coords {
	int x, y;
};

struct Transform {
	Coords position, size;
};

// CellContent class (same as your original)
class CellContent {
private:
	string name;

	int cost;
	int hp;
	int speed;

public:
	CellContent() : name(" "), cost(0), hp(0), speed(0) {}

	CellContent(string new_name, int new_cost, int new_hp)
		: name(new_name), cost(new_cost), hp(new_hp), speed(0) {
	}

	CellContent(string new_name, int new_cost, int new_hp, int new_speed)
		: name(new_name), cost(new_cost), hp(new_hp), speed(new_speed) {
	}

	void Set_Name(string value) { name = value; }
	string Get_Name() { return name; }

	void Set_Cost(int value) { cost = value; }
	void Add_Cost(int value) { cost += value; }
	int Get_Cost() const { return cost; }

	void Set_HP(int value) { hp = value; }
	void Add_HP(int value) { hp += value; }
	int Get_HP() const { return hp; }

	void Set_Speed(int value) { speed = value; }
	int Get_Speed() const { return speed; }
};

// GameBoard class (same as your original)
class GameBoard {
private:
	Transform grid;
	std::vector<std::vector<CellContent>> cell;

public:
	GameBoard(int x, int y) : cell(x, std::vector<CellContent>(y)) {
		grid = { {0, 0}, {x, y} };
	}

	string DrawBoard(Coords selected) {
		string returned = "+";
		for (int x = 0; x < grid.size.x; x++) returned += "---+";
		returned += '\n';

		for (int y = 0; y < grid.size.y; y++) {
			for (int x = 0; x < grid.size.x; x++) {
				returned += "| ";
				returned += (selected.x == x && selected.y == y) ? "X" : cell[x][y].Get_Name();
				returned += " ";
			}
			returned += "|\n+";
			for (int x = 0; x < grid.size.x; x++) returned += "---+";
			returned += '\n';
		}

		return returned;
	}

	void Set_Cell(Coords coords, CellContent value) { cell[coords.x][coords.y] = value; }
	CellContent Get_Cell(Coords coords) { return cell[coords.x][coords.y]; }
};

// GameLoop function with Linux compatibility
void GameLoop() {
	bool gameloop = true;
	int fps = 60;
	int frameCount = 0;
	string output = "", old_output = "";
	auto lastFrameTime = chrono::steady_clock::now();

	// Important cells
	CellContent Nothing, PlantCurrency("C", 1, 0, 10), ZombieCurrency(PlantCurrency);
	int zombieTimeCount = 1;

	// Plants cells
	CellContent Sunflower = CellContent("S", 2, 3, 24);
	CellContent Peashooter = CellContent("P", 4, 3, 2);
	CellContent Wall_Nut = CellContent("W", 2, 9);

	// Zombies cells
	const int maxAmountZombies = 15;
	std::vector<CellContent> zombies;
	for (int x = 0; x < maxAmountZombies; x++) {
		zombies[x] = Nothing;
	}

	CellContent Basic = CellContent("A", 5, 3, 5);
	CellContent ConedHead = CellContent("B", Basic.Get_Cost() * 2, Basic.Get_HP() * 2, Basic.Get_Speed());
	CellContent BucketHead = CellContent("C", Basic.Get_Cost() * 3, Basic.Get_HP() * 3, Basic.Get_Speed());

	// Board setup
	const int plantBoardWidth = 5, plantBoardHeight = 2;
	const int gameBoardWidth = 9, gameBoardHeight = 5;
	GameBoard plantsBoard(plantBoardWidth, plantBoardHeight), gameBoard(gameBoardWidth, gameBoardHeight);
	Coords plantBoardSelection{ 2, 0 }, gameBoardSelection{ 0, 0 };

	for (int y = 0; y < plantBoardHeight; y++) {
		for (int x = 0; x < plantBoardWidth; x++) {
			switch (x) {
			case 0: plantsBoard.Set_Cell({ x , y }, y == 0 ? PlantCurrency : CellContent()); break;
			case 1: plantsBoard.Set_Cell({ x , y }, y == 0 ? CellContent() : CellContent()); break;
			case 2: plantsBoard.Set_Cell({ x , y }, y == 0 ? Sunflower : CellContent(to_string(Sunflower.Get_Cost()), Sunflower.Get_Cost(), 0)); break;
			case 3: plantsBoard.Set_Cell({ x , y }, y == 0 ? Peashooter : CellContent(to_string(Peashooter.Get_Cost()), Peashooter.Get_Cost(), 0)); break;
			case 4: plantsBoard.Set_Cell({ x , y }, y == 0 ? Wall_Nut : CellContent(to_string(Wall_Nut.Get_Cost()), Wall_Nut.Get_Cost(), 0)); break;
			}
		}
	}

	while (gameloop) {
		auto currentTime = chrono::steady_clock::now();
		auto deltaTime = chrono::duration_cast<chrono::milliseconds>(currentTime - lastFrameTime).count();

		if (deltaTime >= 1000 / fps) {
			if (isKeyPressed()) {
				char key = getKeyPressed();
				bool hasMoney = PlantCurrency.Get_Cost() >= plantsBoard.Get_Cell({ plantBoardSelection.x, 1 }).Get_Cost();
				bool canPlace = (
						gameBoard.Get_Cell({ gameBoardSelection }).Get_Name() == Basic.Get_Name() ||
						gameBoard.Get_Cell({ gameBoardSelection }).Get_Name() == ConedHead.Get_Name() ||
						gameBoard.Get_Cell({ gameBoardSelection }).Get_Name() == BucketHead.Get_Name()
					) ? false : true;

				switch (key) {
				case 'q': case 'Q': if (plantBoardSelection.x > 2) plantBoardSelection.x--; break;
				case 'e': case 'E': if (plantBoardSelection.x < 4) plantBoardSelection.x++; break;

				case 'w': case 'W': if (gameBoardSelection.y > 0) gameBoardSelection.y--; break;
				case 'a': case 'A': if (gameBoardSelection.x > 0) gameBoardSelection.x--; break;
				case 's': case 'S': if (gameBoardSelection.y < 4) gameBoardSelection.y++; break;
				case 'd': case 'D': if (gameBoardSelection.x < 8) gameBoardSelection.x++; break;

				case ' ':
					if (hasMoney && canPlace) {
						gameBoard.Set_Cell(gameBoardSelection, plantsBoard.Get_Cell(plantBoardSelection));
						PlantCurrency.Add_Cost(-plantsBoard.Get_Cell({ plantBoardSelection.x, 1 }).Get_Cost());
					}
					break;
				case 27: gameloop = false; break;
				}
			}
			plantsBoard.Set_Cell({ 0, 1 }, CellContent(to_string(PlantCurrency.Get_Cost()), PlantCurrency.Get_Cost(), 0));

			output = "Plants Board:\n" + plantsBoard.DrawBoard(plantBoardSelection);
			output += "\nGame Board:\n" + gameBoard.DrawBoard(gameBoardSelection) + '\n';

			if (output != old_output) {
				ResetCursor();
				cout << output;
				old_output = output;
			}

			// Currency Update
			if ((frameCount % (fps * PlantCurrency.Get_Speed())) == 0) {
				PlantCurrency.Add_Cost(1);
			}

			// Sunflowers Currency Update
			if ((frameCount % (fps * Sunflower.Get_Speed())) == 0) {
				int sunflowerCount = 0;
				for (int y = 0; y < gameBoardHeight; y++) {
					for (int x = 0; x < gameBoardWidth; x++) {
						bool hasSunflowerHere = gameBoard.Get_Cell({ x, y }).Get_Name() == "S";

						if (hasSunflowerHere) {
							sunflowerCount++;
						}
					}
				}
				PlantCurrency.Add_Cost(sunflowerCount);  // Increase cost based on Sunflowers
			}

			// Peashooter Shoots Update
			if ((frameCount % (fps * Peashooter.Get_Speed())) == 0) {
				for (int y = 0; y < gameBoardHeight; y++) {
					for (int x = 0; x < gameBoardWidth; x++) {
						bool hasPeashooterHere = gameBoard.Get_Cell({ x, y }).Get_Name() == "P";

						if (hasPeashooterHere) {
							for (int i = 0; i < gameBoardWidth; i++) {

								bool hasZombieNext = (
									gameBoard.Get_Cell({ i , y }).Get_Name() == Basic.Get_Name() ||
									gameBoard.Get_Cell({ i , y }).Get_Name() == ConedHead.Get_Name() ||
									gameBoard.Get_Cell({ i , y }).Get_Name() == BucketHead.Get_Name()
									) ? true : false;

								if (hasZombieNext) {
									// Damages next cell
									CellContent damagedZombie = gameBoard.Get_Cell({ i , y });
									damagedZombie.Add_HP(-1);
									if (damagedZombie.Get_HP() != 0) { gameBoard.Set_Cell({ i , y }, damagedZombie); }
									else { gameBoard.Set_Cell({ i , y }, Nothing); }
									break;
								}
							}
						}
					}
				}
			}

			//// Peashooter Shoots Update
			//if ((frameCount % (fps * Peashooter.Get_Speed())) == 0) {
			//	for (int y = 0; y < gameBoardHeight; y++) {
			//		for (int x = 0; x < gameBoardWidth; x++) {
			//			if (gameBoard.Get_Cell({ x, y }).Get_Name() == "P") { // Found Peashooter
			//				for (int i = x + 1; i < gameBoardWidth; i++) {
			//					if (!zombies.empty()) {
			//						for (auto it = zombies.begin(); it != zombies.end() - 1; ++it) {
			//							if (gameBoard.Get_Cell({ i, y }).Get_Name() == it->Get_Name()) {
			//								// Damage zombie
			//								it->Add_HP(-1);

			//								if (it->Get_HP() > 0) {
			//									gameBoard.Set_Cell({ i, y }, *it);
			//								}
			//								else {
			//									gameBoard.Set_Cell({ i, y }, Nothing);
			//									zombies.erase(it); // Remove from list
			//								}
			//								break;
			//							}
			//						}
			//					}
			//				}
			//			}
			//		}
			//	}
			//}

			// Zombies Currency Update
			if ((frameCount % (fps * ZombieCurrency.Get_Speed())) == 0) {
				ZombieCurrency.Add_Cost(zombieTimeCount);
			}

			bool zombieHasMoney = ZombieCurrency.Get_Cost() >= Basic.Get_Cost();
			bool zombieFull = false;

			for (int i = 0; i < maxAmountZombies; i++) {
				if (zombies[i].Get_Name() == " ") {
					zombieFull = false;
					break;
				}
				else {
					zombieFull = true;
				}
			}
			if (zombieHasMoney && !zombieFull) {
				for (int i = 0; i < maxAmountZombies; i++) {
					if (zombies[i].Get_Name() == Nothing.Get_Name()) {
						if (ZombieCurrency.Get_Cost() >= BucketHead.Get_Cost()) {
							zombies[i] = BucketHead;
							zombieTimeCount++;
						}
						else if (ZombieCurrency.Get_Cost() >= ConedHead.Get_Cost()) {
							zombies[i] = ConedHead;
							zombieTimeCount++;
						}
						else if (ZombieCurrency.Get_Cost() >= Basic.Get_Cost()) {
							zombies[i] = Basic;
							zombieTimeCount++;
						}

						vector<int> possibleLines(5);
						int k = 0;
						for (int y = 0; y < gameBoardHeight; y++) {
							if (gameBoard.Get_Cell({ gameBoardWidth - 1 , y }).Get_Name() == " ") {
								possibleLines[k] = y;
								k++;
							}
						}

						int randomLine = Random::FromList(possibleLines);
						gameBoard.Set_Cell({ gameBoardWidth - 1, randomLine }, zombies[i]);
						ZombieCurrency.Add_Cost(-zombies[i].Get_Cost());
						break;
					}
				}
			}

			// Detects Zombies // Moves them or damages next Plant
			// Loop the board
			for (int y = 0; y < gameBoardHeight; y++) {
				for (int x = 0; x < gameBoardWidth; x++) {
					// Loop all zombies
					for (int i = 0; i < maxAmountZombies; i++) {
						// If theres a zombie in the all zombies array
						if (zombies[i].Get_Name() != Nothing.Get_Name()) {
							// If board cell has the zombie
							if (gameBoard.Get_Cell({ x, y }).Get_Name() == zombies[i].Get_Name()) {
								// If speed of the zombie equals the frame (if speed = 5 and frame = 5 or 10 or ...
								if ((frameCount % (fps * zombies[i].Get_Speed())) == 0) {
									// If next cell has hp
									if (gameBoard.Get_Cell({ x - 1, y }).Get_HP() >= 1) {
										// Damages next cell
										CellContent damagedPlant = gameBoard.Get_Cell({ x - 1, y });
										damagedPlant.Add_HP(-1);
										if (damagedPlant.Get_HP() != 0) { gameBoard.Set_Cell({ x - 1, y }, damagedPlant); }
										else { gameBoard.Set_Cell({ x - 1, y }, Nothing); }
									}
									else {
										// If next cell is not bellow 0
										if (x - 1 >= 0) {
											// Moves a cell foward
											gameBoard.Set_Cell({ x - 1, y }, zombies[i]);
											gameBoard.Set_Cell({ x, y }, Nothing);
										}
										else {
											// Finishs the game
											gameloop = false;
										}
									}
								}
							}
						}
					}
				}
			}

			frameCount++;
			lastFrameTime = currentTime;
		}
	}
	cout << "Thanks for playing!\n";
}
