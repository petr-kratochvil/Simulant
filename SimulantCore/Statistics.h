#pragma once
#include "Spin.h"
#include <sstream>

struct SymbolStatItem
{
	int64_t winCount;
	int64_t totalWin;
};

class Statistics
{
public:
	Statistics(int symbolCount, int credit = 0);
	void addSpin(const Spin& spin);
	const std::wstring& getDescription();
protected:
	int64_t totalWin;
	int64_t totalWinSquared;
	int64_t spinCount;
	int64_t betCount;
	int64_t zeroCount;
	int64_t credit;
	int lastWin;
	int symbolCount;
	bool respinNext;
	std::vector<SymbolStatItem> symbolStats;
	std::wstring descOut;
	std::wstring lastReelset;
};