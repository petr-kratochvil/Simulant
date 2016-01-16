#include "Random.h"
#include <limits.h>

std::mt19937* Random::generator = nullptr;

void Random::init(int seed)
{
	Random::generator = new std::mt19937(seed);
}

void Random::init()
{
	std::uniform_int_distribution<int> dist(0, INT_MAX - 1);
	std::random_device rd;
	Random::init(dist(rd));
}

int Random::gen(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(*Random::generator);
}

bool Random::genPct(int pct)
{
	return (Random::gen(0, 99) < pct);
}

bool Random::genPml(int pml)
{
	return (Random::gen(0, 999) < pml);
}

bool Random::genP4(int p4)
{
	return (Random::gen(0, 9999) < p4);
}