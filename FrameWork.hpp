// FrameWork.hpp
#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include <iostream>

#include <string>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <chrono>

#include <functional>

void SetConsoleSize(int width, int height);

void SetConsoleFontSize(int size);

void enableANSI();

constexpr auto RESET = "\033[0m";
#define COLOR(h)   "\033[38;5;" #h "m"

// Clears the console.
void ClearScreen();

void RenderFrame(const std::string& output, std::string& old_output);

// Moves cursor to the top-left instead of clearing the screen
void ResetCursor();

// Cross-platform kbhit() equivalent for Linux
bool isKeyPressed();

// Cross-platform getch() equivalent for Linux
char getKeyPressed();

struct Coords {
    int x, y, z;
};

void Start(bool &gameloop, std::function<void()> func);

void Update(bool& gameloop, std::string& output, float fps, std::function<void()> func);

#endif //FRAMEWORK_HPP
