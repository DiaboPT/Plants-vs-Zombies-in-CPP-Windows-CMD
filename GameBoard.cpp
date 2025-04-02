#include "GameBoard.hpp"

GameBoard::GameBoard(int x, int y) : cell(x, vector<CellContent>(y)) {
	grid = { x, y };
}

GameBoard::GameBoard() {
	grid = {0, 0};
}

const std::string GameBoard::DrawBoard(Coords selected, std::string selectedColor, std::string resetColor) {
	string returned = resetColor;
	returned += (selected.x == 0 && selected.y == 0 ? selectedColor + std::string("+") + resetColor : std::string("+"));
	for (int x = 0; x < grid.x; x++) {
		returned += selected.x == x && selected.y == 0 ? selectedColor + std::string("---") + resetColor : std::string("---");
		returned += ((selected.x == x && selected.y == 0) || (selected.x == x + 1 && selected.y == 0)) ? selectedColor + std::string("+") + resetColor : std::string("+");
	}
	returned += resetColor + "\n";

	for (int y = 0; y < grid.y; y++) {
		returned += selected.x == 0 && selected.y == y ? selectedColor + std::string("| ") + resetColor : std::string("| ");
		for (int x = 0; x < grid.x; x++) {
			returned += cell[x][y].Get_Name() + resetColor;
			if (x < grid.x - 1) {
				returned += ((selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y)) ? selectedColor + std::string(" | ") + resetColor : std::string(" | ");
			}
			else {
				returned += ((selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y)) ? selectedColor + std::string(" |") + resetColor : std::string(" |");
			}
		}
		returned += resetColor + "\n";
		returned += ((selected.x == 0 && selected.y == y) || (selected.x == 0 && selected.y == y + 1)) ? selectedColor + std::string("+") + resetColor : std::string("+");
		for (int x = 0; x < grid.x; x++) {
			returned += ((selected.x == x && selected.y == y) || (selected.x == x && selected.y == y + 1)) ? selectedColor + std::string("---") + resetColor : std::string("---");
			returned += ((selected.x == x + 1 && selected.y == y + 1) || (selected.x == x && selected.y == y) || (selected.x == x + 1 && selected.y == y) || (selected.x == x && selected.y == y + 1)) ? selectedColor + std::string("+") + resetColor : std::string("+");
		}
		returned += resetColor + "\n";
	}

	return returned;
}

void GameBoard::SetCell(Coords coords, CellContent value) { cell[coords.x][coords.y] = value; }
const CellContent GameBoard::GetCell(Coords coords) const { return cell[coords.x][coords.y]; }