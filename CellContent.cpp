#include "CellContent.hpp"

CellContent::CellContent() : name(" "), cost(0), hp(0), speed(0) {}

CellContent::CellContent(string new_name, int new_cost)
	: name(new_name), cost(new_cost), hp(0), speed(0) {
}

CellContent::CellContent(string new_name, int new_cost, int new_hp)
	: name(new_name), cost(new_cost), hp(new_hp), speed(0) {
}

CellContent::CellContent(string new_name, int new_cost, int new_hp, float new_speed)
	: name(new_name), cost(new_cost), speed(new_speed), hp(new_hp) {
}

void CellContent::Set_Name(string value) { name = value; }
const string CellContent::Get_Name() const { return name; }

void CellContent::Set_Cost(int value) { cost = value; }
void CellContent::Add_Cost(int value) { cost += value; }
const int CellContent::Get_Cost() const { return cost; }

void CellContent::Set_HP(int value) { hp = value; }
void CellContent::Add_HP(int value) { hp += value; }
const int CellContent::Get_HP() const { return hp; }

void CellContent::Set_Speed(float value) { speed = value; }
const float CellContent::Get_Speed() const { return speed; }