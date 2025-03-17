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
            if (transform.position.y % 2 == 0) {
                s += "+";
                for (transform.position.x = 0; transform.position.x < transform.size.x; transform.position.x++) {
                    s += "---+";
                }
            }
            else {
                s += "|";
                for (transform.position.x = 0; transform.position.x < transform.size.x; transform.position.x++) {
                    s += " ";
                    if (selected.x == transform.position.x and selected.y == transform.position.y) {
                        s += "X";
                    }
                    else {
                        s += GetCellContent(transform.position.x, transform.position.y);
                    }
                    s += " |";
                }
            }
            s += "\n";
        }
        s += "\n";

        return s;
    }

    virtual std::string GetCellContent(int x, int y) = 0;  // To be overridden

    virtual ~Board() {}
};

#endif
