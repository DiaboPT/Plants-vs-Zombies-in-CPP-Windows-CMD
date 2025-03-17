#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include "Board.hpp"
#include <vector>

class GameBoard : public Board {
private:
    std::vector<std::vector<char>> grid;
public:
    GameBoard() : Board(11, 11) {
        // Initialize grid with spaces.
        grid.resize(height, std::vector<char>(width, ' '));
    }

    // Override to display what is in grid.
    std::string GetCellContent(int x, int y) override {
        return std::string(1, grid[y][x]);
    }

    // Place a plant symbol at a specific position.
    void PlacePlant(const Coordenates& pos, char plantSymbol) {
        if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height)
            grid[pos.y][pos.x] = plantSymbol;
    }

    int Get_Sunflowers() {
        int n = 1;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                if (GetCellContent(i, j) == "S") {
                    n++;
                }
            }
        }
        return n;
    }
};

#endif
