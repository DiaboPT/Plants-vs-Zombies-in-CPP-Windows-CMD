#include "Levels.hpp"

Levels::Levels() : maxLevel({ 5 , 10 }), level({ 0 , 0 }) {}

void Levels::SetLevel(Coords value) {
	level = value;
}
void Levels::AddLevel() {
	level.x++;

	if (level.x > 10) {
		level.x = 0;
		level.y++;
	}
}
Coords Levels::GetLevel() const {
	return { level.x + 1 , level.y + 1 };
}

void Levels::SetWinCondiction(float value) {
	winCondiction = value;
}
float Levels::GetWinCondiction() const {
	return winCondiction;
}

// Pass by reference to avoid copying
void Levels::SetPlantsTypes(const vector<CellContent> value) {
	plantsTypes = value;
}

// Pass by reference to avoid copying
void Levels::AddPlantsTypes(const CellContent value) {
	plantsTypes.push_back(value);
}

// Return by const reference to avoid copying
const vector<CellContent> Levels::GetPlantsTypes() const {
	return plantsTypes;
}

void Levels::SetZombiesTypes(const vector<CellContent> value) {
	zombiesTypes = value;
}

void Levels::AddZombiesTypes(const CellContent value) {
	zombiesTypes.push_back(value);
}

const vector<CellContent> Levels::GetZombiesTypes() const {
	return zombiesTypes;
}