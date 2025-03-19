#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <random>

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

#endif //HEADER_HPP