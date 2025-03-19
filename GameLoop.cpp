// GameLoop.cpp
#include "header.hpp"

using namespace std;

// Cross-platform GetConsoleWindow() equivalent for Linux
void SetConsoleSize(int width, int height) {
#ifdef _WIN32
	HWND console = GetConsoleWindow(); // Get console window handle
	if (!console) return;

	// Move and resize window: (x, y, width, height)
	MoveWindow(console, 100, 100, width, height, TRUE);
#else
	// For Linux, using stty for terminal resizing
	std::string command = "stty cols " + std::to_string(width) + " rows " + std::to_string(height);
	system(command.c_str());  // Execute the command to resize the terminal
#endif
}

void SetConsoleFontSize(int size) {
#ifdef _WIN32
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info{ sizeof(CONSOLE_FONT_INFOEX) };
	if (!GetCurrentConsoleFontEx(output, false, &info))
		return;
	info.dwFontSize.Y = size;  // Set font size (Y is the height of the font)
	SetCurrentConsoleFontEx(output, false, &info);
#else
	// Assuming you are on GNOME or similar
	std::string command = "gsettings set org.gnome.desktop.interface monospace-font-name 'Monospace " + std::to_string(size) + "'";
	system(command.c_str());  // Execute the command to change font size
#endif
}

#ifdef _WIN32
void enableANSI() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
void enableANSI() {} // No need to enable on Linux
#endif

#define RESET   "\033[0m"
#define PLANTRESET   "\033[38;5;208m"
#define GAMERESET   "\033[38;5;46m"
#define COLOR(h) "\033[38;5;" #h "m"
#define BGCOLOR(h) "\033[48;5;" #h "m"

// Full colors
const char* full_colors[] = {
		COLOR(196), // 0° - Red
		COLOR(208), // 30° - Orange
		COLOR(220), // 60° - Yellow
		COLOR(46),  // 90° - Lime Green
		COLOR(51),  // 120° - Cyan
		COLOR(37),  // 150° - Teal
		COLOR(21),  // 180° - Blue
		COLOR(57),  // 210° - Indigo
		COLOR(93),  // 240° - Purple
		COLOR(165), // 270° - Magenta
		COLOR(201), // 300° - Pink
		COLOR(203)  // 330° - Scarlet
};

// Much darker versions of the previous full color
const char* dark_colors[] = {
		BGCOLOR(88),  // 15° - Dark Red
		BGCOLOR(130), // 45° - Dark Orange
		BGCOLOR(136), // 75° - Dark Yellow
		BGCOLOR(22),  // 105° - Dark Green
		BGCOLOR(23),  // 135° - Dark Cyan
		BGCOLOR(24),  // 165° - Dark Teal
		BGCOLOR(18),  // 195° - Dark Blue
		BGCOLOR(56),  // 225° - Dark Indigo
		BGCOLOR(55),  // 255° - Dark Purple
		BGCOLOR(90),  // 285° - Dark Magenta
		BGCOLOR(95),  // 315° - Dark Pink
		BGCOLOR(131)  // 345° - Dark Scarlet
};

class Random {
public:
	static int Range(int min, int max) {
		static mt19937 mt(Seed()); // Shared RNG
		uniform_int_distribution<int> dist(min, max);
		return dist(mt);
	}

	static int FromList(const vector<int>& numbers) {
		if (numbers.empty()) return 0;
		static mt19937 mt(Seed()); // Shared RNG
		uniform_int_distribution<int> dist(0, numbers.size() - 1);
		return numbers[dist(mt)];
	}

private:
	static mt19937::result_type Seed() {
		static random_device rd;
		return rd();
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

// CellContent class
class CellContent {
private:
	string name;

	int cost;
	int hp;
	float speed;

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

// GameBoard class
class GameBoard {
private:
	Transform grid;
	vector<vector<CellContent>> cell;

public:
	GameBoard(int x, int y) : cell(x, vector<CellContent>(y)) {
		grid = { {0, 0}, {x, y} };
	}

	string DrawBoard(Coords selected, string selectedColor, string resetColor, string bgColor) {
		string returned = bgColor + (selected.x == 0 && selected.y == 0 ? selectedColor + string("+") + resetColor : string("+"));
		for (int x = 0; x < grid.size.x; x++) {
			returned += selected.x == x && selected.y == 0 ? selectedColor + string("---") + resetColor : string("---");
			returned += ((selected.x == x && selected.y == 0) || (selected.x == x + 1 && selected.y == 0)) ? selectedColor + string("+") + resetColor : string("+");
		}
		returned += resetColor + "\n";

		for (int y = 0; y < grid.size.y; y++) {
			returned += selected.x == 0 && selected.y == y ? selectedColor + string("| ") + resetColor : string("| ");
			for (int x = 0; x < grid.size.x; x++) {
				returned += cell[x][y].Get_Name() + resetColor;
				if (x < grid.size.x - 1) {
					returned += ((selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y)) ? selectedColor + string(" | ") + resetColor : string(" | ");
				}
				else {
					returned += ((selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y)) ? selectedColor + string(" |") + resetColor : string(" |");
				}
			}
			returned += resetColor + "\n";
			returned += ((selected.x == 0 && selected.y == y) || (selected.x == 0 && selected.y == y + 1)) ? selectedColor + string("+") + resetColor : string("+");
			for (int x = 0; x < grid.size.x; x++) {
				returned += ((selected.x == x && selected.y == y) || (selected.x == x && selected.y == y + 1)) ? selectedColor + string("---") + resetColor : string("---");
				returned += ((selected.x == x + 1 && selected.y == y + 1) || (selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y) || (selected.x == x && selected.y == y + 1)) ? selectedColor + string("+") + resetColor : string("+");
			}
			returned += resetColor + "\n";
		}

		return returned;
	}

	void Set_Cell(Coords coords, CellContent value) { cell[coords.x][coords.y] = value; }
	CellContent Get_Cell(Coords coords) { return cell[coords.x][coords.y]; }
};

bool Menu() {
	char key = 'A';
	int selection = 0;
	while (key != ' ') {
		ResetCursor();
		cout << "+---------------------------------------------------------+" << "\n";
		cout << "| " << "PPPPP " << "   " << "VV     VV" << "   " << "ZZZZZZ" << "   " << "   " << " CCCCC" << "   " << "  +  " << "   " << "  +  " << " |" << "\n";
		cout << "| " << "PP  PP" << "   " << " VV   VV " << "   " << "   ZZ " << "   " << "   " << "CCC   " << "   " << "  +  " << "   " << "  +  " << " |" << "\n";
		cout << "| " << "PPPPP " << "   " << "  VV VV  " << "   " << "  ZZ  " << "   " << "   " << "CC    " << "   " << "+++++" << "   " << "+++++" << " |" << "\n";
		cout << "| " << "PP    " << "   " << "   VVV   " << "   " << " ZZ   " << "   " << "   " << "CCC   " << "   " << "  +  " << "   " << "  +  " << " |" << "\n";
		cout << "| " << "PP    " << "   " << "    V    " << "   " << "ZZZZZZ" << "   " << "   " << " CCCCC" << "   " << "  +  " << "   " << "  +  " << " |" << "\n";
		cout << "+---------------------------------------------------------+" << "\n";
		cout << ((selection == 0) ? string("> Start <") : string("  Start  ")) << "\n";
		cout << ((selection == 1) ? string("> Quit  <") : string("  Quit   ")) << "\n";
		key = getKeyPressed();
		switch (key) {
		case 'w': case 'W': if (selection > 0) selection--; break;
		case 's': case 'S': if (selection < 1) selection++; break;

		case ' ':
			ClearScreen();
			return (selection == 0) ? true : false;
			break;
		case 27: return false; break;
		}
	}
}

// GameLoop function with Linux compatibility
void GameLoop() {
	enableANSI();
	SetConsoleFontSize({ 24 });
	SetConsoleSize(1600, 900);
	// Start
	bool gameloop = Menu();
	int fps = 60;
	int frameCount = 0;
	string output = "", old_output = "";
	auto lastFrameTime = chrono::steady_clock::now();

	// Important cells
	CellContent Nothing, PlantCurrency(COLOR(220) + string("C"), 1, 0, 10), ZombieCurrency(PlantCurrency);
	int zombieTimeCount = 1;

	// Plants cells
	CellContent Sunflower = CellContent(COLOR(220) + string("S"), 2, 6, 24);
	CellContent Peashooter = CellContent(COLOR(46) + string("P"), 4, 6, 1.5);
	CellContent Wall_Nut = CellContent(COLOR(208) + string("W"), 2, 72);

	// Zombies cells
	CellContent Basic = CellContent(COLOR(196) + string("A"), 5, 3, 4);
	CellContent ConedHead = CellContent(COLOR(196) + string("B"), Basic.Get_Cost() * 2, Basic.Get_HP() * 2, Basic.Get_Speed());
	CellContent BucketHead = CellContent(COLOR(196) + string("C"), Basic.Get_Cost() * 3, Basic.Get_HP() * 3, Basic.Get_Speed());

	// Board setup
	const int plantBoardWidth = 5, plantBoardHeight = 2;
	const int gameBoardWidth = 9, gameBoardHeight = 5;
	const int zombieBoardWidth = 5, zombieBoardHeight = 2;
	GameBoard plantsBoard(plantBoardWidth, plantBoardHeight), gameBoard(gameBoardWidth, gameBoardHeight), zombieBoard(zombieBoardWidth, zombieBoardHeight);
	Coords plantBoardSelection{ 2, 0 }, gameBoardSelection{ 0, 0 }, zombieBoardSelection{ 2, 0 };

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

	for (int y = 0; y < zombieBoardHeight; y++) {
		for (int x = 0; x < zombieBoardWidth; x++) {
			switch (x) {

			case 0: zombieBoard.Set_Cell({ x , y }, y == 0 ? ZombieCurrency : CellContent()); break;
			case 1: zombieBoard.Set_Cell({ x , y }, y == 0 ? CellContent() : CellContent()); break;
			case 2: zombieBoard.Set_Cell({ x , y }, y == 0 ? Basic : CellContent(to_string(Basic.Get_Cost()), Basic.Get_Cost(), 0)); break;
			case 3: zombieBoard.Set_Cell({ x , y }, y == 0 ? ConedHead : CellContent(to_string(ConedHead.Get_Cost()), ConedHead.Get_Cost(), 0)); break;
			case 4: zombieBoard.Set_Cell({ x , y }, y == 0 ? BucketHead : CellContent(to_string(BucketHead.Get_Cost()), BucketHead.Get_Cost(), 0)); break;
			}
		}
	}

	// Update
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
			zombieBoard.Set_Cell({ 0, 1 }, CellContent(to_string(ZombieCurrency.Get_Cost()), ZombieCurrency.Get_Cost(), 0));

			output = RESET;
			output += "Plants Board:\n" + string(PLANTRESET) + plantsBoard.DrawBoard(plantBoardSelection, COLOR(51), PLANTRESET, BGCOLOR(88));
			output += RESET;
			output += "\nGame Board:\n" + string(GAMERESET) + gameBoard.DrawBoard(gameBoardSelection, COLOR(51), GAMERESET, BGCOLOR(22));
			output += RESET;
			output += "\nZombies Board\n" + string(PLANTRESET) + zombieBoard.DrawBoard(zombieBoardSelection, COLOR(51), PLANTRESET, BGCOLOR(88)) + '\n';
			output += RESET;

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
						bool hasSunflowerHere = gameBoard.Get_Cell({ x, y }).Get_Name() == Sunflower.Get_Name();

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
						bool hasPeashooterHere = gameBoard.Get_Cell({ x, y }).Get_Name() == Peashooter.Get_Name();

						if (hasPeashooterHere) {
							for (int i = x + 1; i < gameBoardWidth; i++) {

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

			// Zombies Currency Update
			if ((frameCount % (fps * ZombieCurrency.Get_Speed())) == 0) {
				ZombieCurrency.Add_Cost(zombieTimeCount);
				zombieBoard.Set_Cell({ 0, 1 }, CellContent(to_string(ZombieCurrency.Get_Cost()), ZombieCurrency.Get_Cost(), 0));
			}

			if ((frameCount % fps) == 0) {
				bool zombieHasMoney = ZombieCurrency.Get_Cost() >= Basic.Get_Cost();
				if (zombieHasMoney) {
					CellContent zombie;
					for (int y = 0; y < plantBoardHeight; y++) {
						if (gameBoard.Get_Cell({ gameBoardWidth - 1, y }).Get_Name() == Nothing.Get_Name()) {
							if (ZombieCurrency.Get_Cost() >= BucketHead.Get_Cost()) {
								zombie = BucketHead;
								zombieTimeCount++;
								zombieBoardSelection = { 4, 0 };
							}
							else if (ZombieCurrency.Get_Cost() >= ConedHead.Get_Cost()) {
								zombie = ConedHead;
								zombieTimeCount++;
								zombieBoardSelection = { 3, 0 };
							}
							else if (ZombieCurrency.Get_Cost() >= Basic.Get_Cost()) {
								zombie = Basic;
								zombieTimeCount++;
								zombieBoardSelection = { 2, 0 };
							}

							vector<int> possibleLines(5);
							int k = 0;
							for (int y = 0; y < gameBoardHeight; y++) {
								if (gameBoard.Get_Cell({ gameBoardWidth - 1 , y }).Get_Name() == Nothing.Get_Name()) {
									possibleLines[k] = y;
									k++;
								}
							}

							int randomLine = Random::FromList(possibleLines);
							gameBoard.Set_Cell({ gameBoardWidth - 1, randomLine }, zombie);
							ZombieCurrency.Add_Cost(-zombie.Get_Cost());
							break;
						}
					}
				}
			}

			// Detects Zombies // Moves them or damages next Plant
			// Loop the board
			for (int y = 0; y < gameBoardHeight; y++) {
				for (int x = 0; x < gameBoardWidth; x++) {
					bool hasZombie = (
						gameBoard.Get_Cell({ x , y }).Get_Name() == Basic.Get_Name() ||
						gameBoard.Get_Cell({ x , y }).Get_Name() == ConedHead.Get_Name() ||
						gameBoard.Get_Cell({ x , y }).Get_Name() == BucketHead.Get_Name()
						) ? true : false;

					if (hasZombie) {
						if ((frameCount % (fps * gameBoard.Get_Cell({ x,y }).Get_Speed())) == 0) {
							// If next cell has hp
							bool hasPlantNext = (
								gameBoard.Get_Cell({ x - 1 , y }).Get_Name() == Sunflower.Get_Name() ||
								gameBoard.Get_Cell({ x - 1 , y }).Get_Name() == Peashooter.Get_Name() ||
								gameBoard.Get_Cell({ x - 1 , y }).Get_Name() == Wall_Nut.Get_Name()
								) ? true : false;

							if (hasPlantNext) {
								// Damages next cell
								CellContent damagedPlant = gameBoard.Get_Cell({ x - 1, y });
								damagedPlant.Add_HP(-1);
								if (damagedPlant.Get_HP() != 0) { gameBoard.Set_Cell({ x - 1, y }, damagedPlant); }
								else { gameBoard.Set_Cell({ x - 1, y }, Nothing); }
							}
							else {
								// If next cell is not Plant
								if (x - 1 >= 0) {
									// Moves a cell foward
									gameBoard.Set_Cell({ x - 1, y }, gameBoard.Get_Cell({ x,y }));
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

			frameCount++;
			lastFrameTime = currentTime;
		}
	}
	cout << "Thanks for playing!\n";
}