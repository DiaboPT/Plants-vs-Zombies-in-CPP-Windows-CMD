#pragma once

#include <iostream>

// CellContent class
class CellContent {
private:
	std::string name;
	std::string color;

	int cost;
	// float cooldown;
	int hp;
	float speed;

public:
	CellContent();
	CellContent(std::string new_color, std::string new_name, int new_cost);
	CellContent(std::string new_color, std::string new_name, int new_cost, int new_hp);
	CellContent(std::string new_color, std::string new_name, int new_cost, int new_hp, float new_speed);

	void Set_Name(std::string value);
	const std::string Get_Name() const;
	const char Get_Char() const;

	void SetColor(std::string value);
	const std::string GetColor() const;

	void SetCost(int value);
	void AddCost(int value);
	const int GetCost() const;

	void SetHP(int value);
	void AddHP(int value);
	const int GetHP() const;

	void SetSpeed(float value);
	const float GetSpeed() const;
};
