#pragma once

#include <vector>
using std::vector;

#include "Transform.hpp"
#include "CellContent.hpp"

// GameBoard class
class GameBoard {
private:
	Transform grid;
	vector<vector<CellContent>> cell;

public:
	GameBoard(int x, int y);

	const string DrawBoard(Coords selected, string selectedColor, string resetColor);

	void SetCell(Coords coords, CellContent value);
	const CellContent GetCell(Coords coords) const;
};
