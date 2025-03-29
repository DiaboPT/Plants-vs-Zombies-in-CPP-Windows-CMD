// header.hpp
#pragma once

#include <iostream>
using std::cout;

#include <string>
using std::to_string;

#include <thread>
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;

#include <chrono>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#endif

#include "Random.hpp"
#include "CellContent.hpp"
#include "GameBoard.hpp"
#include "Levels.hpp"
#include "SFX.hpp"
#include "GameLoop.hpp"
