
#include "GameBoard.hpp"

GameBoard::GameBoard() {
	grid = { 0, 0 };
}

GameBoard::GameBoard(Coords size) : cell(size.x, std::vector<CellContentClass>(size.y)) {
	grid = size;

	for (int y = 0; y < grid.y; y++) {
		for (int x = 0; x < grid.x; x++) {
			GameBoard::SetCell(grid, Nothing);
		}
	}

}

const std::string GameBoard::DrawBoard(Coords selected, std::string selectedColor, std::string resetColor) {
	std::string returned = resetColor;
	returned += (selected.x == 0 && selected.y == 0 ? selectedColor + std::string("+") + resetColor : std::string("+"));
	for (int x = 0; x < grid.x; x++) {
		returned += selected.x == x && selected.y == 0 ? selectedColor + std::string("---") + resetColor : std::string("---");
		returned += ((selected.x == x && selected.y == 0) || (selected.x == x + 1 && selected.y == 0)) ? selectedColor + std::string("+") + resetColor : std::string("+");
	}
	returned += resetColor + "\n";

	for (int y = 0; y < grid.y; y++) {
		returned += selected.x == 0 && selected.y == y ? selectedColor + std::string("| ") + resetColor : std::string("| ");
		for (int x = 0; x < grid.x; x++) {
			returned += cell[x][y].Color() + cell[x][y].Char() + resetColor;
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

void GameBoard::SetCell(Coords coords, CellContentClass value) {
	cell
		[(coords.x < 0) ? 0 : (coords.x > grid.x - 1) ? static_cast<std::vector<std::vector<CellContentClass, std::allocator<CellContentClass>>, std::allocator<std::vector<CellContentClass, std::allocator<CellContentClass>>>>::size_type>(grid.x) - 1 : coords.x]
		[(coords.y < 0) ? 0 : (coords.y > grid.y - 1) ? static_cast<std::vector<CellContentClass, std::allocator<CellContentClass>>::size_type>(grid.y) - 1 : coords.y]
		= value;
}

CellContentClass GameBoard::GetCell(Coords coords) {
	return cell
		[(coords.x < 0) ? 0 : (coords.x > grid.x - 1) ? static_cast<std::vector<std::vector<CellContentClass, std::allocator<CellContentClass>>, std::allocator<std::vector<CellContentClass, std::allocator<CellContentClass>>>>::size_type>(grid.x) - 1 : coords.x]
		[(coords.y < 0) ? 0 : (coords.y > grid.y - 1) ? static_cast<std::vector<CellContentClass, std::allocator<CellContentClass>>::size_type>(grid.y) - 1 : coords.y];
}

void GameBoard::SetGrid(Coords value) {
	grid = value;
}
Coords GameBoard::GetGrid() const {
	return grid;
}
