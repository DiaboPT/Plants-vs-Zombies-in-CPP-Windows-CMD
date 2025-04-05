#pragma once

#include <vector>

#include "CellContent.hpp"
#include "cppFrameWork/FrameWork.hpp"

class Levels {
private:
	Coords maxLevel = { 0 , 0 };
	Coords level = { 0 , 0 };
	float winCondiction = 0.0f;

	std::vector<CellContent> plantsTypes;
	std::vector<CellContent> zombiesTypes;
public:
	Levels();

	void SetLevel(Coords value);
	void AddLevel();
	Coords GetLevel() const;

	void SetWinCondiction(float value);
	float GetWinCondiction() const;

	// Pass by reference to avoid copying
	void SetPlantsTypes(const std::vector<CellContent> value);

	// Pass by reference to avoid copying
	void AddPlantsTypes(const CellContent value);

	// Return by const reference to avoid copying
	const std::vector<CellContent> GetPlantsTypes() const;

	void SetZombiesTypes(const std::vector<CellContent> value);

	void AddZombiesTypes(const CellContent value);

	const std::vector<CellContent> GetZombiesTypes() const;
};
