// GameBoard.hpp
#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include "Board.hpp"
#include <vector>

class GameBoard : public Board {
private:
    int currency;
    CellContent Nothing = CellContent();

    CellContent Basic = CellContent("B", 3, 4);
    CellContent Conned = CellContent("C", 6, 8);

    std::vector<std::vector<CellContent>> grid;

public:
    GameBoard() : Board(11, 5), currency(0) {
        // Initialize grid with spaces.
        grid.resize(height, std::vector<CellContent>(width, Nothing));
    }

    // Place a symbol at a specific position.
    void PlaceInGrid(const Coordenates& pos, CellContent symbol) {
        if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height)
            grid[pos.y][pos.x] = symbol;
    }

    int Get_Sunflowers() {
        int n = 1;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                if (GetCellContent(i, j).Get_Name() == "S") {
                    n++;
                }
            }
        }
        return n;
    }

    // Returns the content of the cell at (x, y) from the grid.
    CellContent GetCellContent(int x, int y) override {
        if (y >= 0 && y < height && x >= 0 && x < width)
            return grid[y][x];
        else
            return Nothing;
    }

    // Sets the current currency and updates the grid for cells in column 0 (except row 1).
    void Set_Currency(int value) {
        currency = value;
    }

    void Add_Currency(int value) {
        currency += value;
    }

    int Get_Currency() const {
        return currency;
    }

    // Returns the plant symbol based on the column.
    // Column 1 -> Basic.
    CellContent GetZombie(int col) {
        if (col == 1) return Basic;
        return Nothing;
    }
};

#endif
