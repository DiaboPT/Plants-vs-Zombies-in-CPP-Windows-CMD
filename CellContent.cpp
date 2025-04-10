#include "CellContent.hpp"

void CellContentClass::Name(std::string value) { name = value; }
const std::string CellContentClass::Name() const { return color + name; }
const char CellContentClass::Char() const { return name != "" ? name[0] : ' '; }

void CellContentClass::Color(std::string value) { color = value; }
const std::string CellContentClass::Color() const { return color; }

void CellContentClass::Cost(int value) { cost = value; }
const int CellContentClass::Cost() const { return cost; }
void CellContentClass::AddCost(int value) { cost += value; }

void CellContentClass::HP(int value) { hp = value; }
const int CellContentClass::HP() const { return hp; }
void CellContentClass::AddHP(int value) { hp += value; }

void CellContentClass::Speed(float value) { speed = value / 1000.f; }
const float CellContentClass::Speed() const { return speed; }
