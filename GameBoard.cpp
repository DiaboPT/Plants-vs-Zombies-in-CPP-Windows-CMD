#include "GameBoard.hpp"

GameBoard::GameBoard(int x, int y) : cell(x, vector<CellContent>(y)) {
	grid = { {0, 0}, {x, y} };
}

const string GameBoard::DrawBoard(Coords selected, string selectedColor, string resetColor) {
	string returned = resetColor;
	returned += (selected.x == 0 && selected.y == 0 ? selectedColor + string("+") + resetColor : string("+"));
	for (int x = 0; x < grid.size.x; x++) {
		returned += selected.x == x && selected.y == 0 ? selectedColor + string("---") + resetColor : string("---");
		returned += ((selected.x == x && selected.y == 0) || (selected.x == x + 1 && selected.y == 0)) ? selectedColor + string("+") + resetColor : string("+");
	}
	returned += resetColor + "\n";

	for (int y = 0; y < grid.size.y; y++) {
		returned += selected.x == 0 && selected.y == y ? selectedColor + string("| ") + resetColor : string("| ");
		for (int x = 0; x < grid.size.x; x++) {
			returned += cell[x][y].Get_Name() + resetColor;
			if (x < grid.size.x - 1) {
				returned += ((selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y)) ? selectedColor + string(" | ") + resetColor : string(" | ");
			}
			else {
				returned += ((selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y)) ? selectedColor + string(" |") + resetColor : string(" |");
			}
		}
		returned += resetColor + "\n";
		returned += ((selected.x == 0 && selected.y == y) || (selected.x == 0 && selected.y == y + 1)) ? selectedColor + string("+") + resetColor : string("+");
		for (int x = 0; x < grid.size.x; x++) {
			returned += ((selected.x == x && selected.y == y) || (selected.x == x && selected.y == y + 1)) ? selectedColor + string("---") + resetColor : string("---");
			returned += ((selected.x == x + 1 && selected.y == y + 1) || (selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y) || (selected.x == x && selected.y == y + 1)) ? selectedColor + string("+") + resetColor : string("+");
		}
		returned += resetColor + "\n";
	}

	return returned;
}

void GameBoard::SetCell(Coords coords, CellContent value) { cell[coords.x][coords.y] = value; }
const CellContent GameBoard::GetCell(Coords coords) const { return cell[coords.x][coords.y]; }