#pragma once

#include <random>

class Random {
public:
	static int Range(int min, int max);

	static int FromList(const std::vector<int>& numbers);

private:
	static std::mt19937::result_type Seed() {
		static std::random_device rd;
		return rd();
	}
};
