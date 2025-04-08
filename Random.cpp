#include "Random.hpp"

int Random::Range(int min, int max) {
	static std::mt19937 mt(Seed()); // Shared RNG
	std::uniform_int_distribution<int> dist(min, max);
	return dist(mt);
}

int Random::FromList(const std::vector<int>& numbers) {
	if (numbers.empty()) return 0;
	static std::mt19937 mt(Seed()); // Shared RNG
	std::uniform_int_distribution<int> dist(0, numbers.size() - 1);
	return numbers[dist(mt)];
}