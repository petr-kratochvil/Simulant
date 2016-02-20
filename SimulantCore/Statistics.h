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
	std::string getWinStatsBasic() const;
	std::string getWinStatsBonus() const;
	std::string getWinStatsByBets() const;
protected:
	std::string getStatsString(const std::vector<int64_t>& stats) const;
	int64_t totalWin;
	int64_t totalWinBasic;
	int64_t totalWinBonus;
	int64_t totalWinSquared;
	int64_t totalBetSpent;
	int64_t oneBetWin;
	int64_t spinCount;
	int64_t betCount;
	int64_t zeroCount;
	int64_t credit;
	int64_t bonusCount;
	int lastWin;
	int symbolCount;
	bool respinNext;
	int fsCount;
	std::vector<SymbolStatItem> symbolStats;
	std::vector<int64_t> winStatsBonus;
	std::vector<int64_t> winStatsBasic;
	std::vector<int64_t> winStatsByBets;
	std::vector<SymbolStatItem>  winStatsByCount;
	std::map<std::wstring, int> characteristicsStats;
	std::wstring descOut;
	std::wstring lastReelset;
};