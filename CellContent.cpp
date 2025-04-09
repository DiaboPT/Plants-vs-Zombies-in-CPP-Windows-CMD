#include "CellContent.hpp"

CellContent::CellContent() : name(" "), cost(0), hp(0), speed(0) {}

CellContent::CellContent(std::string new_color, std::string new_name, int new_cost)
	: color(new_color), name(new_name), cost(new_cost), hp(0), speed(0) {
}

CellContent::CellContent(std::string new_color, std::string new_name, int new_cost, int new_hp)
	: color(new_color), name(new_name), cost(new_cost), hp(new_hp), speed(0) {
}

CellContent::CellContent(std::string new_color, std::string new_name, int new_cost, int new_hp, float new_speed)
	: color(new_color), name(new_name), cost(new_cost), speed(new_speed), hp(new_hp) {
}

void CellContent::Set_Name(std::string value) { name = value; }
const std::string CellContent::Get_Name() const { return name; }
const char CellContent::Get_Char() const { return name[0]; }

void CellContent::SetColor(std::string value) { color = value; }
const std::string CellContent::GetColor() const { return color; }

void CellContent::SetCost(int value) { cost = value; }
void CellContent::AddCost(int value) { cost += value; }
const int CellContent::GetCost() const { return cost; }

void CellContent::SetHP(int value) { hp = value; }
void CellContent::AddHP(int value) { hp += value; }
const int CellContent::GetHP() const { return hp; }

void CellContent::SetSpeed(float value) { speed = value; }
const float CellContent::GetSpeed() const { return speed; }