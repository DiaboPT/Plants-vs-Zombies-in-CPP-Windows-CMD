// header.hpp
#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <cmath>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#endif

#include "GameLoop.hpp"

using namespace std;
