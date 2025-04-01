// FrameWork.cpp
#include "FrameWork.hpp"

void InitWindow(int width, int height) {
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
	string command = "gsettings set org.gnome.desktop.interface monospace-font-name 'Monospace " + to_string(size) + "'";
	system(command.c_str());  // Execute the command to change font size
#endif
}

void enableANSI() {
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void ClearScreen() {
	std::cout << "\033[2J\033[H"; // ANSI escape for clearing console
}

void RenderFrame(const string& output, string& old_output) { // Pass old_output by reference
	if (output != old_output) {
		ResetCursor();
		cout << output;
		old_output = output; // Now this persists across frames
	}
}

// Moves cursor to the top-left instead of clearing the screen
void ResetCursor() {
	cout << "\033[H"; // ANSI escape sequence for resetting cursor
}

// Cross-platform kbhit() equivalent for Linux
bool isKeyPressed() {
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
char getKeyPressed() {
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

void Start(bool& gameloop, function<void()> func) {
	enableANSI();
	gameloop = true;  // Ensure gameloop is set before running func
	func();
}

// InputEvents will trigger the appropriate function for the key press
void InputEvents(char key, const std::unordered_map<char, std::function<void()>>& keyActions) {
	auto it = keyActions.find(key);  // Check if the key exists in the map
	if (it != keyActions.end()) {
		it->second();  // Call the function associated with the key
	}
	else {
		std::cout << "No action assigned for key '" << key << "'." << std::endl;
	}
}

void Update(bool& gameloop, string& output, float fps, function<void()> func) {
	static string old_output = "";
	auto lastFrameTime = steady_clock::now();

	while (gameloop) {
		auto currentTime = steady_clock::now();
		auto deltaTime = duration_cast<milliseconds>(currentTime - lastFrameTime).count();

		if (deltaTime >= 1000 / fps) {
			if (isKeyPressed()) {
				char key = getKeyPressed();
				// InputEvents(key, keyActions);
			}

			func();
			RenderFrame(output, old_output);
			output = "";
			lastFrameTime = steady_clock::now();
		}
	}
}

