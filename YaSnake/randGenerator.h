#pragma once

#include <random>

// make it a Singleton class

class RandGenerator {
private:
	RandGenerator();

	std::random_device dev;
	std::mt19937 engine{ dev() };

	int getIntInRangeInternal(int a, int b);

public:
	RandGenerator(const RandGenerator&) = delete;
	RandGenerator& operator=(const RandGenerator&) = delete;

	static RandGenerator& getInstance();

	static int getIntInRange(int a, int b) {
		return getInstance().getIntInRangeInternal(a, b);
	}

};