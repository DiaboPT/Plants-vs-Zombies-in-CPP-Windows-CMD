#include "Random.hpp"

int Random::Range(int min, int max) {
	static mt19937 mt(Seed()); // Shared RNG
	uniform_int_distribution<int> dist(min, max);
	return dist(mt);
}

int Random::FromList(const vector<int>& numbers) {
	if (numbers.empty()) return 0;
	static mt19937 mt(Seed()); // Shared RNG
	uniform_int_distribution<int> dist(0, numbers.size() - 1);
	return numbers[dist(mt)];
}