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
	return { level.x , level.y };
}

void Levels::SetWinCondiction(float value) {
	winCondiction = value;
}
float Levels::GetWinCondiction() const {
	return winCondiction;
}

// Pass by reference to avoid copying
void Levels::SetPlantsTypes(const std::vector<CellContentClass> value) {
	plantsTypes = value;
}

// Pass by reference to avoid copying
void Levels::AddPlantsTypes(const CellContentClass value) {
	plantsTypes.push_back(value);
}

// Return by const reference to avoid copying
const std::vector<CellContentClass> Levels::GetPlantsTypes() const {
	return plantsTypes;
}

void Levels::SetZombiesTypes(const std::vector<CellContentClass> value) {
	zombiesTypes = value;
}

void Levels::AddZombiesTypes(const CellContentClass value) {
	zombiesTypes.push_back(value);
}

const std::vector<CellContentClass> Levels::GetZombiesTypes() const {
	return zombiesTypes;
}

void Levels::ClearTypes() {
	plantsTypes.clear();
	zombiesTypes.clear();
}