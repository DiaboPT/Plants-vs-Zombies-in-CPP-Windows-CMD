#ifndef PLANTSBOARD_HPP
#define PLANTSBOARD_HPP

#include "Board.hpp"
#include "Plant.hpp"
#include <vector>
#include <string>

class PlantsBoard : public Board {
private:
    int currency;
    Plant Sunflower;
    Plant Peashooter;
    Plant WallNut;
    std::vector<std::vector<std::string>> grid;  // internal grid

public:
    // Constructs a PlantsBoard with a grid of 4 columns x 5 rows.
    // The grid is filled based on these rules:
    // - Column 0: if row == 1, display "C"; otherwise display the current currency.
    // - Column 1: if row == 1, display Sunflower's name; otherwise display its cost.
    // - Column 2: if row == 1, display Peashooter's name; otherwise display its cost.
    // - Column 3: if row == 1, display WallNut's name; otherwise display its cost.
    PlantsBoard() : Board(4, 2), currency(2),
        Sunflower("S", 3, 1), Peashooter("P", 3, 2), WallNut("W", 75, 1) {

        // Initialize grid dimensions.
        grid.resize(height, std::vector<std::string>(width, " "));

        // Fill the grid with the initial values.
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0) {
                    grid[y][x] = (y == 0) ? "C" : std::to_string(Get_Currency());
                }
                else if (x == 1) {
                    grid[y][x] = (y == 0) ? Sunflower.Get_Name() : std::to_string(Sunflower.Get_Cost());
                }
                else if (x == 2) {
                    grid[y][x] = (y == 0) ? Peashooter.Get_Name() : std::to_string(Peashooter.Get_Cost());
                }
                else if (x == 3) {
                    grid[y][x] = (y == 0) ? WallNut.Get_Name() : std::to_string(WallNut.Get_Cost());
                }
            }
        }
    }

    // Returns the content of the cell at (x, y) from the grid.
    std::string GetCellContent(int x, int y) override {
        if (y >= 0 && y < height && x >= 0 && x < width)
            return grid[y][x];
        return " ";
    }

    // Sets the current currency and updates the grid for cells in column 0 (except row 1).
    void Set_Currency(int value) {
        currency = value;
        for (int y = 0; y < height; y++) {
            if (y != 0) {
                grid[y][0] = std::to_string(currency);
            }
        }
    }

    void Add_Currency(int value) {
        currency += value;
        for (int y = 0; y < height; y++) {
            if (y != 0) {
                grid[y][0] = std::to_string(currency);
            }
        }
    }

    int Get_Currency() {
        return currency;
    }

    // Returns the plant symbol based on the column.
    // Column 1 -> Sunflower, Column 2 -> Peashooter, Column 3 -> WallNut.
    char GetPlantSymbol(int col) {
        if (col == 1) return Sunflower.Get_Name()[0];
        if (col == 2) return Peashooter.Get_Name()[0];
        if (col == 3) return WallNut.Get_Name()[0];
        // return grid[0][col];
        return ' ';
    }
};

#endif
