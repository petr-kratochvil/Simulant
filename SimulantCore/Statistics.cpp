#include "Statistics.h"


Statistics::Statistics(int symbolCount, int credit)
	: totalWin(0)
	, totalWinSquared(0)
	, totalWinBasic(0)
	, totalWinBonus(0)
	, totalBetSpent(0)
	, spinCount(0)
	, betCount(0)
	, zeroCount(0)
	, bonusCount(0)
	, credit(credit)
	, lastWin(0)
	, oneBetWin(0)
	, fsCount(0)
	, symbolCount(symbolCount)
	, respinNext(false)
	, symbolStats(symbolCount)
	, winStatsBonus(2000)
	, winStatsBasic(2000)
	, winStatsByBets(2000)
	, winStatsByCount(5)
{
	for (int i = 0; i < symbolCount; i++)
		this->symbolStats[i] = { 0, 0 };
	for (int i = 0; i < this->winStatsBonus.size(); i++)
		this->winStatsBonus[i] = 0;
	for (int i = 0; i < this->winStatsBasic.size(); i++)
		this->winStatsBasic[i] = 0;
	for (int i = 0; i < this->winStatsByBets.size(); i++)
		this->winStatsByBets[i] = 0;
	for (int i = 0; i < this->winStatsByCount.size(); i++)
		this->winStatsByCount[i] = { 0,0 };
}

void Statistics::addSpin(const Spin & spin)
{
	if (!respinNext)
		this->oneBetWin = 0;
	const WindowWin& ww = spin.getWin();
	this->lastWin = ww.getTotal();
	this->totalWin += this->lastWin;
	this->totalWinSquared += this->lastWin * this->lastWin;

	this->credit += this->lastWin;
	this->oneBetWin += this->lastWin;
	this->spinCount += 1;
	this->fsCount = spin.getFsCount();

	const std::vector<std::wstring>& c = spin.getCharacteristics();
	for (int i = 0; i < c.size(); i++)
	{
		if (this->characteristicsStats.find(c[i]) == this->characteristicsStats.end())
			this->characteristicsStats[c[i]] = 1;
		else
			this->characteristicsStats[c[i]]++;
	}

	if (!respinNext)
	{
		this->betCount += 1;
		this->totalBetSpent += spin.getBet();
		this->credit -= spin.getBet();
		this->totalWinBasic += this->lastWin;
		int index = this->lastWin / spin.getBet();
		if (index >= 2000)
			index = 1999;
		this->winStatsBasic[index]++;
		
		const std::vector<WindowWinItem>& wins = spin.getWin().getList();
		for (int i = 0; i < wins.size(); i++)
		{
			this->symbolStats[wins[i].symbol.getId()].winCount++;
			this->symbolStats[wins[i].symbol.getId()].totalWin += wins[i].pay;
			this->winStatsByCount[wins[i].count-1].winCount++;
			this->winStatsByCount[wins[i].count-1].totalWin += wins[i].pay;
		}
	}
	else
	{
		this->totalWinBonus += this->lastWin;
		int index = this->lastWin / spin.getBet();
		if (index >= 2000)
			index = 1999;
		if (std::find(c.begin(), c.end(), L"BonusFinal") != c.end())
			this->winStatsBonus[index]++;
	}
	if (spin.isFinal())
	{
		respinNext = false;
		int index = this->oneBetWin / spin.getBet();
		if (index >= 2000)
			index = 1999;
		this->winStatsByBets[index]++;
		if (this->oneBetWin == 0)
			this->zeroCount += 1;
	}
	else
	{
		if (!respinNext)
			this->bonusCount++;
		respinNext = true;
	}
	this->lastReelset = spin.getReelset();
}


const std::wstring & Statistics::getDescription()
{
	std::wstringstream description;
	description << L"Kredit:\t\t" << this->credit << L"\r\n";
	description << L"Poslední výhra:\t" << this->lastWin << L"\r\n";
	description << L"Počet freespinů:\t" << this->fsCount << L"\r\n";
	description << L"Sada válců:\t" << this->lastReelset << L"\r\n";
	description << L"RTP:\t\t" << double(this->totalWin) / (double(this->totalBetSpent)) * 100.0 << L" %\r\n";
	description << L"RTP základ:\t" << double(this->totalWinBasic) / (double(this->totalBetSpent)) * 100.0 << L" %\r\n";
	description << L"RTP bonus:\t" << double(this->totalWinBonus) / (double(this->totalBetSpent)) * 100.0 << L" %\r\n";
	description << L"Počet spinů:\t" << this->spinCount << L"\r\n";
	description << L"Počet sázek:\t" << this->betCount << L"\r\n";
	description << L"Bonus 1x za:\t" << double(this->betCount) / double(this->bonusCount) << L" otáček\r\n";
	double zeros = double(this->zeroCount) / double(this->betCount);
	description << L"Nulové otáčky:\t" << zeros * 100.0 << L" % , tj. "
		<< zeros / (1-zeros) << " za sebou\r\n";
	description << L"Následuje respin?\t" << this->respinNext << L"\r\n";
	description << L"Charakteristiky:\r\n";
	for (std::map<std::wstring, int>::iterator iter = this->characteristicsStats.begin(); iter != this->characteristicsStats.end(); iter++)
	{
		description << std::wstring(iter->first.begin(), iter->first.end());
		description << L"\t" << double(iter->second) / double(this->spinCount) * 100.0 << L" %, tj. 1x za ";
		description << double(this->betCount) / double(iter->second) << L" otáček\r\n";
	}
	for (int i = 0; i < this->symbolCount; i++)
	{
		description << L"symbol" << i << ": RTP " << double(this->symbolStats[i].totalWin) / double(this->totalBetSpent) * 100.0 << L" %\r\n";
	}
	for (int i = 0; i < 5; i++)
	{
		description << L"count" << i+1 << ": RTP " << double(this->winStatsByCount[i].totalWin) / double(this->totalBetSpent) * 100.0 << L" %\r\n";
	}
	this->descOut = description.str();
	return this->descOut;
}

std::string Statistics::getWinStatsBasic() const
{
	return this->getStatsString(this->winStatsBasic);
}

std::string Statistics::getWinStatsBonus() const
{
	return this->getStatsString(this->winStatsBonus);
}

std::string Statistics::getWinStatsByBets() const
{
	return this->getStatsString(this->winStatsByBets);
}

std::string Statistics::getStatsString(const std::vector<int64_t>& stats) const
{
	std::stringstream ss;
	for (int i = 0; i < stats.size(); i++)
	{
		ss << i * 5 << "\t" << stats[i] << "\n";
	}
	return ss.str();
}
