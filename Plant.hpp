#ifndef PLANT_HPP
#define PLANT_HPP

#include <string>
#include "Transform.hpp"

using std::string;

class Plant {
public:
	Plant(string name, int cost);

	void Set_Name(string value);
	string Get_Name();

	void Set_Cost(int value);
	int Get_Cost();

	void Set_Transform(int x, int y);
	Coordenates Get_Transform();

private:
	string name;

	int cost;
	Coordenates transform = { 0 , 0 };
};

#endif // !PLANT_HPP
