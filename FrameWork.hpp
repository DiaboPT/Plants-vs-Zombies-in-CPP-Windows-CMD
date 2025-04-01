// FrameWork.hpp
#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include <iostream>
using std::cout;

#include <string>
using std::string;
using std::to_string;

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <chrono>
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;

#include <functional>  // Needed for std::function
using std::function;

#include <unordered_map>
using std::unordered_map;

// Define USE_OPENGL if compiling for OpenGL
#ifdef USE_OPENGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

void InitWindow(int width, int height);

void SetConsoleFontSize(int size);

void enableANSI();

constexpr auto RESET = "\033[0m";
#define COLOR(h)   "\033[38;5;" #h "m"

// Clears the console.
void ClearScreen();

void RenderFrame(const string& output, string& old_output);

// Moves cursor to the top-left instead of clearing the screen
void ResetCursor();

// Cross-platform kbhit() equivalent for Linux
bool isKeyPressed();

// Cross-platform getch() equivalent for Linux
char getKeyPressed();

struct Coords {
    int x, y, z;
};

class InputHandler {
public:
	unordered_map<char, function<void()>> keyBindings;

	void RegisterKey(char key, function<void()> action);

	void HandleInput();
};

void Start(bool &gameloop, function<void()> func);

void Update(bool& gameloop, string& output, float fps, function<void()> func);

#endif //FRAMEWORK_HPP
