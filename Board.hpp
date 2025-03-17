#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include "Transform.hpp"

class Board {
protected:
    int width, height;

public:
    Board(int w, int h) : width(w), height(h) {}

    virtual std::string GenerateBoard(Coordenates selected) {
        std::string s = "";
        Transform transform = { {0, 0}, {width, height} };

        for (transform.position.y = 0; transform.position.y < transform.size.y; transform.position.y++) {
            s += "+";
            for (transform.position.x = 0; transform.position.x < transform.size.x; transform.position.x++) {
                s += "---+";
            }
            s += "\n|";
            for (transform.position.x = 0; transform.position.x < transform.size.x; transform.position.x++) {
                s += " ";
                if (selected.x == transform.position.x and selected.y == transform.position.y) {
                    s += "\033[44m";  // Start blue background
                    s += GetCellContent(transform.position.x, transform.position.y);
                    s += "\033[0m";   // Reset color
                }
                else {
                    s += GetCellContent(transform.position.x, transform.position.y);
                }
                s += " |";
            }
            s += "\n";
        }
        s += "+";
        for (transform.position.x = 0; transform.position.x < transform.size.x; transform.position.x++) {
            s += "---+";
        }
        s += "\n";

        return s;
    }


    virtual std::string GetCellContent(int x, int y) = 0;  // To be overridden

    virtual ~Board() {}
};

#endif
