#pragma once

#include <vector>

#include "CellContent.hpp"
#include "cppFrameWork/FrameWork.hpp"

// GameBoard class
class GameBoard {
private:
	Coords grid;
	std::vector<std::vector<CellContentClass>> cell;

public:
	GameBoard(Coords size);
	GameBoard();

	const std::string DrawBoard(Coords selected, std::string selectedColor, std::string resetColor);

	void SetCell(Coords coords, CellContentClass value);
	CellContentClass GetCell(Coords coords);

	void SetGrid(Coords value);
	Coords GetGrid();
};
