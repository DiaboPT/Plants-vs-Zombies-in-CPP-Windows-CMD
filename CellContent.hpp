#pragma once

#include <iostream>

// CellContent class
class CellContent {
private:
	std::string name;

	int cost;
	// float cooldown;
	int hp;
	float speed;

public:
	CellContent();
	CellContent(std::string new_name, int new_cost);
	CellContent(std::string new_name, int new_cost, int new_hp);
	CellContent(std::string new_name, int new_cost, int new_hp, float new_speed);

	void Set_Name(std::string value);
	const std::string Get_Name() const;

	void Set_Cost(int value);
	void Add_Cost(int value);
	const int Get_Cost() const;

	void Set_HP(int value);
	void Add_HP(int value);
	const int Get_HP() const;

	void Set_Speed(float value);
	const float Get_Speed() const;
};
