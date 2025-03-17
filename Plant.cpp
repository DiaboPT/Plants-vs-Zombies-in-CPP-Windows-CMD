#include "Plant.hpp"

Plant::Plant(string new_name, int new_health, int new_cost) {
	Set_Name(new_name);
	Set_Health(new_health);
	Set_Cost(new_cost);
};

void Plant::Set_Name(string value) {
	name = value;
}
string Plant::Get_Name() {
	return name;
}

void Plant::Set_Health(int value) {
	health = value;
}
void Plant::Add_Health(int value) {
	health += value;
}
int Plant::Get_Health() {
	return health;
}

void Plant::Set_Cost(int value) {
	cost = value;
}
int Plant::Get_Cost() {
	return cost;
}

void Plant::Set_Transform(int x, int y) {
	transform = { x , y };
}

Coordenates Plant::Get_Transform() {
	return transform;
}
