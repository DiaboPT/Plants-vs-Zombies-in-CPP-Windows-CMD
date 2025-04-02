#pragma once

#include <vector>
using std::vector;

#include "FrameWork.hpp"
#include "CellContent.hpp"

// GameBoard class
class GameBoard {
private:
	Coords grid;
	vector<vector<CellContent>> cell;

public:
	GameBoard(int x, int y);
	GameBoard();

	const std::string DrawBoard(Coords selected, std::string selectedColor, std::string resetColor);

	void SetCell(Coords coords, CellContent value);
	const CellContent GetCell(Coords coords) const;
};
