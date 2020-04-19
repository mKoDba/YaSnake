
#include "randGenerator.h"

RandGenerator::RandGenerator() {};

RandGenerator& RandGenerator::getInstance() {
	static RandGenerator sInstance;
	return sInstance;
}

int RandGenerator::getIntInRangeInternal(int a, int b) {
	std::uniform_int_distribution<int> dist{ a, b };
	return dist(engine);
}