// GameLoop.cpp
#include "header.hpp"
#include <string>

using namespace std;

// Notes frequencies (in Hz)
const int C4 = 261;
const int D4 = 294;
const int E4 = 329;
const int F4 = 349;
const int G4 = 392;
const int A4 = 440;
const int B4 = 466;
const int C5 = 523;

// Helper function to create a rest between sections (helps add phrasing)
void static rest(int duration) {
	this_thread::sleep_for(chrono::milliseconds(duration));
}

// Helper function to play a note with a specified frequency and duration
void static playNote(int frequency, int duration) {
#ifdef _WIN32
	Beep(frequency, duration);
	rest(50);  // Short pause between notes
#endif
}

void static GrasswalkSong() {
#ifdef _WIN32
	thread([] {
		while (true) {
			playNote(E4, 400); // E
			playNote(E4, 400); // E
			playNote(E4, 400); // E
			playNote(E4, 400); // E
			playNote(D4, 400); // D
			playNote(D4, 400); // D
			playNote(E4, 400); // E
			playNote(D4, 400); // D
			playNote(C4, 400); // C

			rest(200);  // Pause between phrases

			// Phrase 2: Transition section
			playNote(G4, 400); // G
			playNote(G4, 400); // G
			playNote(F4, 400); // F
			playNote(F4, 400); // F
			playNote(E4, 400); // E
			playNote(E4, 400); // E
			playNote(D4, 400); // D
			playNote(D4, 400); // D

			rest(200);  // Pause for phrasing

			// Phrase 3: High note variation
			playNote(C5, 400); // C5
			playNote(C5, 400); // C5
			playNote(B4, 400); // B
			playNote(A4, 400); // A
			playNote(G4, 400); // G
			playNote(F4, 400); // F

			rest(200);  // Pause for phrasing

			// Phrase 4: Repetition with slight variation
			playNote(E4, 400); // E
			playNote(E4, 400); // E
			playNote(E4, 400); // E
			playNote(E4, 400); // E
			playNote(D4, 400); // D
			playNote(D4, 400); // D
			playNote(E4, 400); // E
			playNote(D4, 400); // D

			// End with a long fade-out note
			playNote(E4, 800); // E (long note to signal the end)
		}
		}).detach();
#endif
}

void static PlayZombieBiteSound() {
#ifdef _WIN32
	thread([] {
		Beep(C4, 100); // Low-frequency bite
		Beep(F4, 120); // Mid-range crunch
		Beep(G4, 80);  // Tearing sound
		}).detach();
#endif
}

void static PlayZombieHitSound() {
#ifdef _WIN32
	thread([] {
		Beep(C5, 50);  // Quick high-pitch "impact"
		Beep(D4, 70);  // Slightly lower "reverberation"
		}).detach();
#endif
}

// Cross-platform GetConsoleWindow() equivalent for Linux
void static SetConsoleSize(int width, int height) {
#ifdef _WIN32
	HWND console = GetConsoleWindow(); // Get console window handle
	if (!console) return;

	// Move and resize window: (x, y, width, height)
	MoveWindow(console, 100, 100, width, height, TRUE);
#else
	// For Linux, using stty for terminal resizing
	string command = "stty cols " + to_string(width) + " rows " + to_string(height);
	system(command.c_str());  // Execute the command to resize the terminal
#endif
}

void static SetConsoleFontSize(int size) {
#ifdef _WIN32
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info{ sizeof(CONSOLE_FONT_INFOEX) };
	if (!GetCurrentConsoleFontEx(output, false, &info))
		return;
	info.dwFontSize.Y = size;  // Set font size (Y is the height of the font)
	SetCurrentConsoleFontEx(output, false, &info);
#else
	// Assuming you are on GNOME or similar
	string command = "gsettings set org.gnome.desktop.interface monospace-font-name 'Monospace " + to_string(size) + "'";
	system(command.c_str());  // Execute the command to change font size
#endif
}

#ifdef _WIN32
void static enableANSI() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
void static enableANSI() {} // No need to enable on Linux
#endif

constexpr auto RESET = "\033[0m";
#define COLOR(h)   "\033[38;5;" #h "m"
#define PLANTRESET COLOR(208)
#define GAMERESET  COLOR(46)

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
	// float cooldown;
	int hp;
	float speed;

public:
	CellContent() : name(" "), cost(0), hp(0), speed(0) {}

	CellContent(string new_name, int new_cost)
		: name(new_name), cost(new_cost), hp(0), speed(0) {
	}

	CellContent(string new_name, int new_cost, int new_hp)
		: name(new_name), cost(new_cost), hp(new_hp), speed(0) {
	}

	CellContent(string new_name, int new_cost, int new_hp, float new_speed)
		: name(new_name), cost(new_cost), speed(new_speed), hp(new_hp) {
	}

	void Set_Name(string value) { name = value; }
	const string Get_Name() const { return name; }

	void Set_Cost(int value) { cost = value; }
	void Add_Cost(int value) { cost += value; }
	const int Get_Cost() const { return cost; }

	void Set_HP(int value) { hp = value; }
	void Add_HP(int value) { hp += value; }
	const int Get_HP() const { return hp; }

	void Set_Speed(float value) { speed = value; }
	const float Get_Speed() const { return speed; }
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

	const string DrawBoard(Coords selected, string selectedColor, string resetColor) {
		string returned = resetColor;
		returned += (selected.x == 0 && selected.y == 0 ? selectedColor + string("+") + resetColor : string("+"));
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

	void SetCell(Coords coords, CellContent value) { cell[coords.x][coords.y] = value; }
	const CellContent GetCell(Coords coords) const { return cell[coords.x][coords.y]; }
};

class Levels {
private:
	Coords maxLevel = { 0 , 0 };
	Coords level = { 0 , 0 };
	int winCondiction = 0.0f;

	vector<CellContent> plantsTypes;
	vector<CellContent> zombiesTypes;
public:
	Levels() : maxLevel({ 5 , 10 }), level({ 0 , 0 }) {}

	void SetLevel(Coords value) {
		level = value;
	}
	void AddLevel() {
		level.x++;

		if (level.x > 10) {
			level.x = 0;
			level.y++;
		}
	}
	Coords GetLevel() const {
		return { level.x + 1 , level.y + 1 };
	}

	void SetWinCondiction(float value) {
		winCondiction = value;
	}
	int GetWinCondiction() const {
		return winCondiction;
	}

	// Pass by reference to avoid copying
	void SetPlantsTypes(const vector<CellContent> value) {
		plantsTypes = value;
	}

	// Pass by reference to avoid copying
	void AddPlantsTypes(const CellContent value) {
		plantsTypes.push_back(value);
	}

	// Return by const reference to avoid copying
	const vector<CellContent> GetPlantsTypes() const {
		return plantsTypes;
	}

	void SetZombiesTypes(const vector<CellContent> value) {
		zombiesTypes = value;
	}

	void AddZombiesTypes(const CellContent value) {
		zombiesTypes.push_back(value);
	}

	const vector<CellContent> GetZombiesTypes() const {
		return zombiesTypes;
	}

};

bool static Menu() {
	bool returned = false;
	char key = 'A';
	int selection = 0;
	bool menuloop = true;
	while (menuloop) {
		ResetCursor();
		cout << COLOR(46) << "+-" << "------" << "---" << "---------" << "------" << "-----" << "------" << "------" << "------" << "---" << "-----" << "---" << "-----" << "-+" << RESET << "\n";
		cout << COLOR(46) << "| " << COLOR(46) << "PPPPP " << "   " << COLOR(46) << "VV     VV" << " SSSS " << "     " << COLOR(165) << "ZZZZZZ" << "      " << COLOR(46) << " CCCCC" << "   " << "  +  " << "   " << "  +  " << COLOR(46) << " |" << "\n";
		cout << COLOR(208) << "| " << COLOR(46) << "PP  PP" << "   " << COLOR(208) << " VV   VV " << "SS   S" << "     " << COLOR(165) << "   ZZ " << "      " << COLOR(208) << "CCC   " << "   " << "  +  " << "   " << "  +  " << COLOR(208) << " |" << "\n";
		cout << COLOR(208) << "| " << COLOR(46) << "PPPPP " << "   " << COLOR(208) << "  VV VV  " << "  SS  " << "     " << COLOR(165) << "  ZZ  " << "      " << COLOR(208) << "CC    " << "   " << "+++++" << "   " << "+++++" << COLOR(208) << " |" << "\n";
		cout << COLOR(208) << "| " << COLOR(46) << "PP    " << "   " << COLOR(208) << "   VVV   " << "S   SS" << "..   " << COLOR(165) << " ZZ   " << "      " << COLOR(208) << "CCC   " << "   " << "  +  " << "   " << "  +  " << COLOR(208) << " |" << "\n";
		cout << COLOR(208) << "| " << COLOR(46) << "PP    " << "   " << COLOR(208) << "    V    " << " SSSS " << "..   " << COLOR(165) << "ZZZZZZ" << "      " << COLOR(208) << " CCCCC" << "   " << "  +  " << "   " << "  +  " << COLOR(208) << " |" << "\n";
		cout << COLOR(208) << "+-" << "------" << "---" << "---------" << "------" << "-----" << "------" << "------" << "------" << "---" << "-----" << "---" << "-----" << "-+" << RESET << "\n";

		cout << RESET;
		cout << "| " << "        Menu Keys         " << " |" << "\n";
		cout << "+-" << "--------------------------" << "-+" << "\n";
		cout << "| " << "     W | Move up          " << " |" << "\n";
		cout << "| " << "     S | Move down        " << " |" << "\n";
		cout << "+-" << "--------------------------" << "-+" << "\n";
		cout << "| " << "      Overall Keys        " << " |" << "\n";
		cout << "+-" << "--------------------------" << "-+" << "\n";
		cout << "| " << " Space | Select           " << " |" << "\n";
		cout << "| " << "Escape | Exit             " << " |" << "\n";
		cout << "+-" << "--------------------------" << "-+" << "\n";

		cout << "| " << " Options " << " |" << "\n";
		cout << "+-----------+" << "\n";
		cout << "| " << ((selection == 0) ? COLOR(46) + string("> Start <") + RESET + string(" |") : string("  Start   |")) << "\n";
		cout << "| " << ((selection == 1) ? COLOR(46) + string("> Guide <") + RESET + string(" | <- Recommended") : string("  Guide   | <- Recommended")) + string("\n");
		cout << "| " << ((selection == 2) ? COLOR(46) + string("> Quit  <") + RESET + string(" |") : string("  Quit    |")) << "\n";
		cout << "+-----------+" << "\n";

		key = getKeyPressed();
		switch (key) {
		case 'w': case 'W': if (selection > 0) selection--; break;
		case 's': case 'S': if (selection < 2) selection++; break;

		case ' ':
			switch (selection) {
			case 0:
				menuloop = false;
				ClearScreen();
				returned = true;
				break;
			case 1:
				for (int i = 1; i <= 4; i++) {
					switch (i) {
					case 1:
						ClearScreen();
						cout << "Plants Board is where the plants are and where you can select them" << "\n";
						cout << "\n";

						cout << "+---+---+---+-----+                                " << "\n";
						cout << "| C |   | P | ... | <- Plants (Colored if selected)" << "\n";
						cout << "+---+---+---+-----+                                " << "\n";
						cout << "| A |   | B | ... | <- Cost of the plant on top    " << "\n";
						cout << "+---+---+---+-----+                                " << "\n";
						cout << "\n";

						cout << "C - Currency / A - Your currency right now                  " << "\n";
						cout << "P - Plant    / B - Currency you need in A to place the plant" << "\n";
						cout << "\n";

						cout << "Keys for Plants Board:" << "\n";
						cout << "- Q - Move to the left in the Plant Board" << "\n";
						cout << "- E - Move to the right in the Plant Board" << "\n";
						cout << "\n";

						getKeyPressed();
						ClearScreen();
						break;
					case 2:
						cout << "Game Board is where you can place plants to actually affect the game" << "\n";
						cout << "\n";

						cout << "+---+---+---+-----+" << "\n";
						cout << "| S | P |   | ... |" << "\n";
						cout << "+---+---+---+-----+" << "\n";
						cout << "| S |   |   | ... |" << "\n";
						cout << "+---+---+---+-----+" << "\n";
						cout << "|   | W | A | ... |" << "\n";
						cout << "+---+---+---+-----+" << "\n";
						cout << "|   | W | B | ... |" << "\n";
						cout << "+---+---+---+-----+" << "\n";
						cout << "|   |   | C | ... |" << "\n";
						cout << "+---+---+---+-----+" << "\n";
						cout << "\n";

						cout << "S - Sunflower   (Plant  - gives more currency each time)" << "\n";
						cout << "P - Peashooter  (Plant  - kills zombies)" << "\n";
						cout << "W - Wall-Nut    (Plant  - tanky)" << "\n";
						cout << "\n";

						cout << "A - Basic       (Zombie - normal)" << "\n";
						cout << "B - Cone-Head   (Zombie - double stats from Basic)" << "\n";
						cout << "C - Bucket-Head (Zombie - triple stats from Basic)" << "\n";
						cout << "\n";

						cout << "Keys for Plants Board:" << "\n";
						cout << "- W - Move to the up in the Game Board" << "\n";
						cout << "- A - Move to the left in the Game Board" << "\n";
						cout << "- S - Move to the down in the Game Board" << "\n";
						cout << "- D - Move to the right in the Game Board" << "\n";
						cout << "\n";

						getKeyPressed();
						ClearScreen();
						break;
					case 3:
						cout << "Zombie Board is where the zombies are and where the pc can select them" << "\n";
						cout << "\n";

						cout << "+---+---+---+-----+                                 " << "\n";
						cout << "| C |   | Z | ... | <- Zombies (Colored if selected)" << "\n";
						cout << "+---+---+---+-----+                                 " << "\n";
						cout << "| A |   | B | ... | <- Cost of the zombie on top    " << "\n";
						cout << "+---+---+---+-----+                                 " << "\n";
						cout << "\n";

						cout << "C - Currency / A - Pc currency right now                   " << "\n";
						cout << "Z - Zombie   / B - Currency pc needs in A to place a zombie" << "\n";
						cout << "\n";

						getKeyPressed();
						ClearScreen();
						break;
					case 4:
						cout << "Objective: Survive";
						cout << "\n";

						getKeyPressed();
						ClearScreen();
						break;
					}

				}
				break;
			case 2:
				menuloop = false;
				break;
			}
			break;
		case 27: menuloop = false; break;
		}
	}
	return returned;
}

// GameLoop function with Linux compatibility
void GameLoop() {
	enableANSI();
	SetConsoleFontSize(20);
	SetConsoleSize(1600, 900);

	// Start
	bool gameloop = Menu();
	int fps = 60;
	int frameCount = 0;
	string output = "", old_output = "";
	auto lastFrameTime = chrono::steady_clock::now();
	Levels level = Levels();

	// Important cells
	CellContent Nothing, plantsCurrency(COLOR(220) + string("C"), 1, 0, 10), zombiesCurrency(plantsCurrency);
	int zombieTimeCount = 1;

	// Define plant and zombie objects manually
	CellContent Peashooter = CellContent(COLOR(46) + string("P"), 4, 6, 1.5f);
	CellContent Sunflower = CellContent(COLOR(220) + string("S"), 2, 6, 24.0f);
	CellContent CherryBomb = CellContent(COLOR(1) + string("C"), 6, 6, 1.2f);
	CellContent WallNut = CellContent(COLOR(208) + string("W"), 2, 72);
	vector<CellContent> plantsTypes = {
		Peashooter,
		Sunflower,
		CherryBomb,
		WallNut
	};

	CellContent Basic = CellContent(COLOR(165) + string("A"), 5, 10, 6.5f);
	CellContent ConeHead = CellContent(COLOR(208) + string("B"), Basic.Get_Cost() * 2, Basic.Get_HP() * 2, Basic.Get_Speed());
	CellContent BucketHead = CellContent("\033[97m" + string("C"), Basic.Get_Cost() * 3, Basic.Get_HP() * 3, Basic.Get_Speed());
	vector<CellContent> zombiesTypes = {
		Basic,
		ConeHead,
		BucketHead
	};

	int i = level.GetLevel().x - 1;
	float j = 5.0f;
	float k = 1.5f + (level.GetLevel().x * .5f);

	level.AddPlantsTypes(plantsTypes[i]);
	level.AddZombiesTypes(zombiesTypes[i]);

	level.SetWinCondiction(pow(j, k));

	// Calculate board width dynamically
	int plantsBoardWidth = level.GetPlantsTypes().size() + 2;  // +2 for currency and empty slot
	int zombiesBoardWidth = level.GetZombiesTypes().size() + 2;
	int plantsBoardHeight = 2, zombiesBoardHeight = 2, gameBoardWidth = 9, gameBoardHeight = 1;

	// Create the boards
	GameBoard plantsBoard(plantsBoardWidth, plantsBoardHeight);
	Coords plantsBoardSelection{ 2,0 };
	GameBoard zombiesBoard(zombiesBoardWidth, zombiesBoardHeight);
	Coords zombiesBoardSelection{ 2,0 };

	GameBoard gameBoard(gameBoardWidth, gameBoardHeight);
	Coords gameBoardSelection{ 0,0 };

	// Populate the plantsBoard
	for (int y = 0; y < plantsBoardHeight; y++) {
		for (int x = 0; x < plantsBoardWidth; x++) {
			if (x == 0) {
				plantsBoard.SetCell({ x, y }, y == 0 ? plantsCurrency : CellContent());
			}
			else if (x == 1) {
				plantsBoard.SetCell({ x, y }, CellContent()); // Empty cell
			}
			else {
				int index = x - 2;
				if (index < level.GetPlantsTypes().size()) {
					plantsBoard.SetCell({ x, y }, y == 0 ? level.GetPlantsTypes()[index] :
						CellContent(to_string(level.GetPlantsTypes()[index].Get_Cost()), level.GetPlantsTypes()[index].Get_Cost(), 0));
				}
			}
		}
	}

	// Populate the zombiesBoard
	for (int y = 0; y < zombiesBoardHeight; y++) {
		for (int x = 0; x < zombiesBoardWidth; x++) {
			if (x == 0) {
				zombiesBoard.SetCell({ x, y }, y == 0 ? zombiesCurrency : CellContent());
			}
			else if (x == 1) {
				zombiesBoard.SetCell({ x, y }, CellContent()); // Empty cell
			}
			else {
				int index = x - 2;
				if (index < level.GetZombiesTypes().size()) {
					zombiesBoard.SetCell({ x, y }, y == 0 ? level.GetZombiesTypes()[index] :
						CellContent(to_string(level.GetZombiesTypes()[index].Get_Cost()), level.GetZombiesTypes()[index].Get_Cost(), 0));
				}
			}
		}
	}

	// Update
	while (gameloop) {
		auto currentTime = chrono::steady_clock::now();
		auto deltaTime = chrono::duration_cast<chrono::milliseconds>(currentTime - lastFrameTime).count();

		if (deltaTime >= 1000 / fps) {

			// Detects if key is press
			if (isKeyPressed()) {
				char key = getKeyPressed();
				bool hasMoney = plantsCurrency.Get_Cost() >= plantsBoard.GetCell({ plantsBoardSelection.x, 1 }).Get_Cost();

				bool canPlace = true;
				for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
					if (gameBoard.GetCell({ gameBoardSelection }).Get_Name() == level.GetZombiesTypes()[i].Get_Name()) {
						canPlace = false;
						break;
					}
				}

				switch (key) {
				case 'q': case 'Q': if (plantsBoardSelection.x > 2) plantsBoardSelection.x--; break;
				case 'e': case 'E': if (plantsBoardSelection.x < (level.GetPlantsTypes().size() + 2) - 1) plantsBoardSelection.x++; break;

				case 'w': case 'W': if (gameBoardSelection.y > 0) gameBoardSelection.y--; break;
				case 'a': case 'A': if (gameBoardSelection.x > 0) gameBoardSelection.x--; break;
				case 's': case 'S': if (gameBoardSelection.y < gameBoardHeight - 1) gameBoardSelection.y++; break;
				case 'd': case 'D': if (gameBoardSelection.x < gameBoardWidth - 1) gameBoardSelection.x++; break;

				case ' ':
					if (hasMoney && canPlace) {
						gameBoard.SetCell(gameBoardSelection, plantsBoard.GetCell(plantsBoardSelection));
						plantsCurrency.Add_Cost(-plantsBoard.GetCell({ plantsBoardSelection.x, 1 }).Get_Cost());
					}
					break;
				case 27: gameloop = false; break;
				}
			}

			// Currency Update
			if (fmod(frameCount, fps * plantsCurrency.Get_Speed()) < 1) {
				plantsCurrency.Add_Cost(1);
				zombiesCurrency.Add_Cost(zombieTimeCount);
			}

			// Sunflowers Currency Update
			if (fmod(frameCount, fps * Sunflower.Get_Speed()) < 1) {
				int sunflowerCount = 0;

				for (int y = 0; y < gameBoardHeight; y++) {
					for (int x = 0; x < gameBoardWidth; x++) {

						bool hasSunflowerHere = gameBoard.GetCell({ x, y }).Get_Name() == Sunflower.Get_Name();
						if (hasSunflowerHere) {
							sunflowerCount++;
						}
					}
				}
				plantsCurrency.Add_Cost(sunflowerCount);  // Increase cost based on Sunflowers
			}

			// Peashooter Shoots Update
			if (fmod(frameCount, fps * Peashooter.Get_Speed()) < 1) {

				for (int y = 0; y < gameBoardHeight; y++) {
					for (int x = 0; x < gameBoardWidth; x++) {

						bool hasPeashooterHere = gameBoard.GetCell({ x, y }).Get_Name() == Peashooter.Get_Name();
						if (hasPeashooterHere) {

							for (int i = x + 1; i < gameBoardWidth; i++) {

								bool hasZombieNext = false;
								for (int j = 0; j < level.GetZombiesTypes().size(); j++) {
									if (gameBoard.GetCell({ i , y }).Get_Name() == level.GetZombiesTypes()[j].Get_Name()) {
										hasZombieNext = true;
										break;
									}
								}
								if (hasZombieNext) {

									// Damages next cell
									CellContent damagedZombie = gameBoard.GetCell({ i , y });
									damagedZombie.Add_HP(-1);
									if (damagedZombie.Get_HP() > 0) { gameBoard.SetCell({ i , y }, damagedZombie); }
									else { gameBoard.SetCell({ i , y }, Nothing); zombiesCurrency.Add_Cost(damagedZombie.Get_Cost() * .5); }
									PlayZombieHitSound();
									break;
								}
							}
						}
					}
				}
			}

			// Cherry Bomb Update
			if (fmod(frameCount, fps * CherryBomb.Get_Speed()) < 1) {

				for (int y = 0; y < gameBoardHeight; y++) {
					for (int x = 0; x < gameBoardWidth; x++) {

						bool hasCherryBombHere = gameBoard.GetCell({ x, y }).Get_Name() == CherryBomb.Get_Name();
						if (hasCherryBombHere) {

							for (int j = max(0, y - 1); j <= min(gameBoardHeight - 1, y + 1); j++) {
								for (int i = max(0, x - 1); i <= min(gameBoardWidth - 1, x + 1); i++) {

									for (int k = 0; k < level.GetZombiesTypes().size(); k++) {
										if (gameBoard.GetCell({ i , j }).Get_Name() == level.GetZombiesTypes()[k].Get_Name()) {

											CellContent damagedZombie = gameBoard.GetCell({ i , j });
											damagedZombie.Add_HP(-90);
											if (damagedZombie.Get_HP() > 0) { gameBoard.SetCell({ i , j }, damagedZombie); }
											else { gameBoard.SetCell({ i , y }, Nothing); zombiesCurrency.Add_Cost(damagedZombie.Get_Cost() * .5); }
											PlayZombieHitSound();
										}
									}
								}
							}
							gameBoard.SetCell({ x , y }, Nothing);
						}
					}
				}
			}

			// Zombies movement + damages next Plant
			for (int y = 0; y < gameBoardHeight; y++) {
				for (int x = 0; x < gameBoardWidth; x++) {

					bool hasZombie = false;
					for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
						if (gameBoard.GetCell({ x , y }).Get_Name() == level.GetZombiesTypes()[i].Get_Name()) {
							hasZombie = true;
							break;
						}
					}
					if (hasZombie) {

						if (x == 0) {
							// Finishs the game
							gameloop = false;
						}
						else {
							bool hasPlantNext = false;
							for (int i = 0; i < level.GetPlantsTypes().size(); i++) {
								if (gameBoard.GetCell({ x - 1  , y }).Get_Name() == level.GetPlantsTypes()[i].Get_Name()) {
									hasPlantNext = true;
									break;
								}
							}

							bool hasZombieNext = false;
							for (int i = 0; i < level.GetZombiesTypes().size(); i++) {
								if (gameBoard.GetCell({ x - 1  , y }).Get_Name() == level.GetZombiesTypes()[i].Get_Name()) {
									hasZombieNext = true;
									break;
								}
							}

							if (hasPlantNext) {

								// Damages next cell
								if (frameCount % fps == 0) {
									CellContent damagedPlant = gameBoard.GetCell({ x - 1, y });
									damagedPlant.Add_HP(-1);
									if (damagedPlant.Get_HP() != 0) { gameBoard.SetCell({ x - 1, y }, damagedPlant); }
									else { gameBoard.SetCell({ x - 1, y }, Nothing); }
									PlayZombieBiteSound();
								}
							}
							else if (!hasPlantNext && !hasZombieNext) {
								if (fmod(frameCount, fps * gameBoard.GetCell({ x,y }).Get_Speed()) < 1) {

									// Moves a cell foward
									gameBoard.SetCell({ x - 1, y }, gameBoard.GetCell({ x,y }));
									gameBoard.SetCell({ x, y }, Nothing);
								}
							}
						}
					}
				}
			}

			// Buy zombies
			if ((frameCount % fps) == 0) {

				bool zombieHasMoney = zombiesCurrency.Get_Cost() >= Basic.Get_Cost();
				if (zombieHasMoney) {

					CellContent zombie;
					for (int i = level.GetZombiesTypes().size() - 1; i >= 0; i--) {
						if (zombiesCurrency.Get_Cost() >= level.GetZombiesTypes()[i].Get_Cost()) {
							zombie = level.GetZombiesTypes()[i];
							zombieTimeCount++;
							break;
						}
					}

					vector<int> possibleLines(0);
					if (true) {
						int k = 0;
						for (int y = 0; y < gameBoardHeight; y++) {
							if (gameBoard.GetCell({ gameBoardWidth - 1 , y }).Get_Name() == Nothing.Get_Name()) {
								possibleLines.resize(k + 1);
								possibleLines[k] = y;
								k++;
							}
						}
					}

					if (possibleLines.size() > 0) {
						int randomLine = Random::FromList(possibleLines);
						gameBoard.SetCell({ gameBoardWidth - 1, randomLine }, zombie);
						zombiesCurrency.Add_Cost(-zombie.Get_Cost());
					}
				}
			}

			plantsBoard.SetCell({ 0, 1 }, CellContent(to_string(plantsCurrency.Get_Cost()), plantsCurrency.Get_Cost(), 0));
			zombiesBoard.SetCell({ 0, 1 }, CellContent(to_string(zombiesCurrency.Get_Cost()), zombiesCurrency.Get_Cost(), 0));

			output = RESET;
			output += "Plants Board:\n" + plantsBoard.DrawBoard(plantsBoardSelection, COLOR(46), PLANTRESET);
			output += RESET;

			output += "\nGame Board:\n" + gameBoard.DrawBoard(gameBoardSelection, COLOR(3), GAMERESET);
			output += RESET;

			output += "Selected: ";
			output += "Name: ";
			output += string(gameBoard.GetCell(gameBoardSelection).Get_Name());
			output += RESET;
			output += " | HP: ";
			output += gameBoard.GetCell(gameBoardSelection).Get_HP() > 9 ? to_string(gameBoard.GetCell(gameBoardSelection).Get_HP()) + " " : "0" + to_string(gameBoard.GetCell(gameBoardSelection).Get_HP()) + " ";
			output += "\n";

			output += "\nZombies Board:\n" + string(PLANTRESET) + zombiesBoard.DrawBoard(zombiesBoardSelection, COLOR(165), PLANTRESET) + '\n';
			output += RESET;

			if (output != old_output) {
				ResetCursor();
				cout << output;
				old_output = output;
			}

			// Win
			if (zombiesCurrency.Get_Cost() >= level.GetWinCondiction()) {
				plantsCurrency = CellContent(COLOR(220) + string("C"), 1, 0, 10);
				zombiesCurrency = plantsCurrency;
				zombieTimeCount = 1;

				level.AddLevel();

				if (true) {
					int i = level.GetLevel().x - 1;
					float j = 5.0f;
					float k = 1.5f + (level.GetLevel().x * .5f);

					level.AddPlantsTypes(plantsTypes[i]);
					level.AddZombiesTypes(zombiesTypes[i]);

					level.SetWinCondiction(pow(j, k));

				}

				plantsBoardWidth = level.GetPlantsTypes().size() + 2;  // +2 for currency and empty slot
				zombiesBoardWidth = level.GetZombiesTypes().size() + 2;

				switch (gameBoardHeight) {
				case 1: case 3:
					gameBoardHeight += 2;
					break;
				}

				plantsBoard = GameBoard(plantsBoardWidth, plantsBoardHeight);
				plantsBoardSelection = { 2,0 };
				zombiesBoard = GameBoard(zombiesBoardWidth, zombiesBoardHeight);
				zombiesBoardSelection = { 2,0 };

				gameBoard = GameBoard(gameBoardWidth, gameBoardHeight);
				gameBoardSelection = { 0,0 };

				// Populate the plantsBoard
				for (int y = 0; y < plantsBoardHeight; y++) {
					for (int x = 0; x < plantsBoardWidth; x++) {
						if (x == 0) {
							plantsBoard.SetCell({ x, y }, y == 0 ? plantsCurrency : CellContent());
						}
						else if (x == 1) {
							plantsBoard.SetCell({ x, y }, CellContent()); // Empty cell
						}
						else {
							int index = x - 2;
							if (index < level.GetPlantsTypes().size()) {
								plantsBoard.SetCell({ x, y }, y == 0 ? level.GetPlantsTypes()[index] :
									CellContent(to_string(level.GetPlantsTypes()[index].Get_Cost()), level.GetPlantsTypes()[index].Get_Cost(), 0));
							}
						}
					}
				}

				// Populate the zombiesBoard
				for (int y = 0; y < zombiesBoardHeight; y++) {
					for (int x = 0; x < zombiesBoardWidth; x++) {
						if (x == 0) {
							zombiesBoard.SetCell({ x, y }, y == 0 ? zombiesCurrency : CellContent());
						}
						else if (x == 1) {
							zombiesBoard.SetCell({ x, y }, CellContent()); // Empty cell
						}
						else {
							int index = x - 2;
							if (index < level.GetZombiesTypes().size()) {
								zombiesBoard.SetCell({ x, y }, y == 0 ? level.GetZombiesTypes()[index] :
									CellContent(to_string(level.GetZombiesTypes()[index].Get_Cost()), level.GetZombiesTypes()[index].Get_Cost(), 0));
							}
						}
					}
				}

				ClearScreen();
				cout << RESET;
				cout << "+----------+\n";
				cout << "| You Win! |\n";
				cout << "+----------+\n";
				cout << RESET;
				rest(3000);
				ClearScreen();
			}

			frameCount++;
			lastFrameTime = currentTime;
		}
	}
	cout << RESET;
	cout << "\n";
	cout << "+---------------------+\n";
	cout << "| Thanks for playing! |\n";
	cout << "| Time: " << to_string(frameCount / fps) << (" seconds!  ") << " |\n";
	cout << "+---------------------+\n";
	cout << RESET;
}
