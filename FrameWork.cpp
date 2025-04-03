// FrameWork.cpp
#include "FrameWork.hpp"

void SetConsoleSize(int width, int height) {
#ifdef _WIN32
	HWND console = GetConsoleWindow(); // Get console window handle
	if (!console) return;

	// Move and resize window: (x, y, width, height)
	MoveWindow(console, 100, 100, width, height, TRUE);
#else
	// For Linux, using stty for terminal resizing
	string command = "stty cols " + std::to_string(width) + " rows " + std::to_string(height);
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
	string command = "gsettings set org.gnome.desktop.interface monospace-font-name 'Monospace " + std::to_string(size) + "'";
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

void RenderFrame(const std::string& output, std::string& old_output) { // Pass old_output by reference
	if (output != old_output) {
		ResetCursor();
		std::cout << output;
		old_output = output; // Now this persists across frames
	}
}

// Moves cursor to the top-left instead of clearing the screen
void ResetCursor() {
	std::cout << "\033[H"; // ANSI escape sequence for resetting cursor
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

void Start(bool& gameloop, std::function<void()> func) {
	enableANSI();
	gameloop = true;  // Ensure gameloop is set before running func
	func();
}

void Update(bool& gameloop, std::string& output, float fps, std::function<void()> func) {
	static std::string old_output = "";
	auto lastFrameTime = std::chrono::steady_clock::now();

	while (gameloop) {
		auto currentTime = std::chrono::steady_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime).count();

		if (deltaTime >= 1000 / fps) {
			func();
			RenderFrame(output, old_output);
			output = "";
			lastFrameTime = std::chrono::steady_clock::now();
		}
	}
}
