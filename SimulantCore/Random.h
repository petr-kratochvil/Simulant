#pragma once

#include <stdlib.h>
#include <time.h>
#include <random>

// All randomness comes from this class
// Uses Mersenne twister with period of 2^19937-1
class Random
{
	static std::mt19937* generator;
public:
	static void init();
	static void init(int seed);
	static void release();
	static int gen(int min, int max);
	static bool genPct(int pct);
	static bool genPml(int pml);
	static bool genP4(int p4);
};
