// CellContent.cpp
#include "CellContent.hpp"

CellContent::CellContent() {
	Set_Name(" ");
	Set_Health(0);
	Set_Cost(0);
};

CellContent::CellContent(string new_name, int new_health, int new_cost) {
	Set_Name(new_name);
	Set_Health(new_health);
	Set_Cost(new_cost);
};

void CellContent::Set_Name(string value) {
	name = value;
}
string CellContent::Get_Name() {
	return name;
}

void CellContent::Set_Health(int value) {
	health = value;
}
void CellContent::Add_Health(int value) {
	health += value;
}
int CellContent::Get_Health() const {
	return health;
}

void CellContent::Set_Cost(int value) {
	cost = value;
}
int CellContent::Get_Cost() const {
	return cost;
}

void CellContent::Set_Transform(int x, int y) {
	transform = { x , y };
}

Coordenates CellContent::Get_Transform() const {
	return transform;
}
