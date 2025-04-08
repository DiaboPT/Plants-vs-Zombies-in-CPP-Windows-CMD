#pragma once

#include <vector>

#include "CellContent.hpp"
#include "cppFrameWork/FrameWork.hpp"

// GameBoard class
class GameBoard {
private:
	Coords grid;
	std::vector<std::vector<CellContent>> cell;

public:
	GameBoard(int x, int y);
	GameBoard();

	const std::string DrawBoard(Coords selected, std::string selectedColor, std::string resetColor);

	void SetCell(Coords coords, CellContent value);
	CellContent GetCell(Coords coords);
};
