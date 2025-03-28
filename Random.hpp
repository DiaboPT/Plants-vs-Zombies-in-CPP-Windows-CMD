#pragma once

#include <random>
using std::mt19937;
using std::uniform_int_distribution;
using std::vector;
using std::random_device;

class Random {
public:
	static int Range(int min, int max);

	static int FromList(const vector<int>& numbers);

private:
	static mt19937::result_type Seed() {
		static random_device rd;
		return rd();
	}
};
