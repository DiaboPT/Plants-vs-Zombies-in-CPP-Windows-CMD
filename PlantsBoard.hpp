// PlantsBoard.hpp
#ifndef PLANTSBOARD_HPP
#define PLANTSBOARD_HPP

#include "Board.hpp"
#include "CellContent.hpp"
#include <vector>
#include <string>

class PlantsBoard : public Board {
private:
    int currency;
    CellContent Nothing = CellContent();

    CellContent Sunflower = CellContent("S", 3, 2);
    CellContent Peashooter = CellContent("P", 3, 4);
    CellContent WallNut = CellContent("W", 75, 2);

    std::vector<std::vector<CellContent>> grid;  // internal grid

public:
    PlantsBoard() : Board(4, 2), currency(2) {

        // Initialize grid dimensions.
        grid.resize(height, std::vector<CellContent>(width, Sunflower));

        // Fill the grid with the initial values.
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0) {
                    grid[y][x].Set_Name((y == 0) ? "C" : std::to_string(Get_Currency()));
                }
                else if (x == 1) {
                    grid[y][x].Set_Name((y == 0) ? Sunflower.Get_Name() : std::to_string(Sunflower.Get_Cost()));
                }
                else if (x == 2) {
                    grid[y][x].Set_Name((y == 0) ? Peashooter.Get_Name() : std::to_string(Peashooter.Get_Cost()));
                }
                else if (x == 3) {
                    grid[y][x].Set_Name((y == 0) ? WallNut.Get_Name() : std::to_string(WallNut.Get_Cost()));
                }
            }
        }
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
        for (int y = 0; y < height; y++) {
            if (y != 0) {
                grid[y][0].Set_Name(std::to_string(currency));
            }
        }
    }

    void Add_Currency(int value) {
        currency += value;
        for (int y = 0; y < height; y++) {
            if (y != 0) {
                grid[y][0].Set_Name(std::to_string(currency));
            }
        }
    }

    int Get_Currency() const {
        return currency;
    }

    // Returns the CellContent symbol based on the column.
    // Column 1 -> Sunflower, Column 2 -> Peashooter, Column 3 -> WallNut.
    CellContent GetPlant(int col) {
        switch (col) {
        case 1:
            return Sunflower;
            break;
        case 2:
            return Peashooter;
            break;
        case 3:
            return WallNut;
            break;
        }
        return Nothing;
    }
};

#endif
