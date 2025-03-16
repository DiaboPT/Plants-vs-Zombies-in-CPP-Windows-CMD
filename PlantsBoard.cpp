// PlantsBoard.cpp
#include "PlantsBoard.hpp"

vector<Coordenates> PlantsCells = {
	{0, 1}, {1, 1}, {2, 1}, {3, 1},  // 'C', 'S', 'P', 'W'
	{0, 3}, {1, 3}, {2, 3}, {3, 3}   // Currency and numbers
};
vector<Coordenates> PlantsSelection = {
	{0, 1}, {1, 1}, {2, 1}, {3, 1},  // 'C', 'S', 'P', 'W'
	{0, 3}, {1, 3}, {2, 3}, {3, 3}   // Currency and numbers
};

int currency = 2;

string PlantsBoard() {
	string s = "";
	Transform transform = { { 4, 5 }, { 0 , 0 } };

	Plant Sunflower = Plant("S", 1);
	Plant Peashooter = Plant("P", 1);
	Plant Wall_Nut = Plant("W", 1);

	for (transform.position.y = 0; transform.position.y < transform.size.y; transform.position.y++) {
		if (transform.position.y % 2 == 0) {
			s += "+";
			for (transform.position.x = 0; transform.position.x < transform.size.x; transform.position.x++) {
				if (transform.position.x == 1) {
					s += "   +";
				}
				s += "---+";
			}
		}
		else {
			s += "|";
			for (transform.position.x = 0; transform.position.x < transform.size.x; transform.position.x++) {
				if (transform.position.x == 1) {
					s += "   |";
				}
				for (const auto& coord : PlantsCells) {
					if (coord.x == transform.position.x && coord.y == transform.position.y) {
						if (coord.x == 0) {
							if (coord.y == 1) {
								s += " C "; // Currency
							}
							else {
								s += " " + to_string(currency) + " "; // Currency
							}
						}
						if (coord.x == 1) {
							if (coord.y == 1) {
								s += " " + Sunflower.Get_Name() + " ";
							}
							else {
								s += " " + to_string(Sunflower.Get_Cost()) + " ";
							}
						}
						if (coord.x == 2) {
							if (coord.y == 1) {
								s += " " + Peashooter.Get_Name() + " ";
							}
							else {
								s += " " + to_string(Peashooter.Get_Cost()) + " ";
							}
						}
						if (coord.x == 3) {
							if (coord.y == 1) {
								s += " " + Wall_Nut.Get_Name() + " ";
							}
							else {
								s += " " + to_string(Wall_Nut.Get_Cost()) + " ";
							}
						}
						break;
					}
				}
				s += "|";
			}
		}
		s += "\n";
	}
	s += "\n";

	return s;
}
