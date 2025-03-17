#include "Zombie.hpp"

Zombie::Zombie(string new_name, int new_health, int new_cost) {
	Set_Name(new_name);
	Set_Health(new_health);
	Set_Cost(new_cost);
};

void Zombie::Set_Name(string value) {
	name = value;
}
string Zombie::Get_Name() {
	return name;
}

void Zombie::Set_Health(int value) {
	health = value;
}
void Zombie::Add_Health(int value) {
	health += value;
}
int Zombie::Get_Health() {
	return health;
}

void Zombie::Set_Cost(int value) {
	cost = value;
}
int Zombie::Get_Cost() {
	return cost;
}

void Zombie::Set_Transform(int x, int y) {
	transform = { x , y };
}
void Zombie::Add_Transform(int x) {
	transform.x += x;
}
Coordenates Zombie::Get_Transform() {
	return transform;
}
