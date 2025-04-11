#pragma once

#include <vector>

#include "CellContent.hpp"
#include "cppFrameWork/FrameWork.hpp"

class Levels {
private:
	Coords maxLevel = { 0 , 0 };
	Coords level = { 0 , 0 };
	float winCondiction = 0.0f;

	std::vector<CellContentClass> plantsTypes;
	std::vector<CellContentClass> zombiesTypes;
public:
	Levels();

	void SetLevel(Coords value);
	void AddLevel();
	Coords GetLevel() const;

	void SetWinCondiction(float value);
	float GetWinCondiction() const;

	// Pass by reference to avoid copying
	void SetPlantsTypes(const std::vector<CellContentClass> value);

	// Pass by reference to avoid copying
	void AddPlantsTypes(const CellContentClass value);

	// Return by const reference to avoid copying
	const std::vector<CellContentClass> GetPlantsTypes() const;

	void SetZombiesTypes(const std::vector<CellContentClass> value);

	void AddZombiesTypes(const CellContentClass value);

	const std::vector<CellContentClass> GetZombiesTypes() const;

	void ClearTypes();

};

static Levels level = Levels();
