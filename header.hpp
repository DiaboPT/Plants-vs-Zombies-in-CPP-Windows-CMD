// header.hpp
#pragma once

#include <iostream>

#include <string>

#include <thread>

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
