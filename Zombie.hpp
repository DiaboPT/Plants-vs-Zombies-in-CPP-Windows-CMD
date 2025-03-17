#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include <string>
#include "Transform.hpp"

using std::string;

class Zombie {
public:
	Zombie(string name, int health, int cost);

	void Set_Name(string value);
	string Get_Name();

	void Set_Health(int value);
	void Add_Health(int value);
	int Get_Health();

	void Set_Cost(int value);
	int Get_Cost();

	void Set_Transform(int x, int y);
	void Add_Transform(int x);
	Coordenates Get_Transform();

private:
	string name;
	int health;

	int cost;
	Coordenates transform = { 0 , 0 };
};

#endif // !PLANT_HPP
