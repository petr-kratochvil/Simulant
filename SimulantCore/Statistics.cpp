#include "Statistics.h"


Statistics::Statistics(int symbolCount, int credit)
	: totalWin(0)
	, totalWinSquared(0)
	, spinCount(0)
	, betCount(0)
	, zeroCount(0)
	, credit(credit)
	, lastWin(0)
	, symbolCount(symbolCount)
	, respinNext(false)
	, symbolStats(symbolCount)
{
	for (int i = 0; i < symbolCount; i++)
		this->symbolStats[i] = { 0, 0 };
}

void Statistics::addSpin(const Spin & spin)
{
	const WindowWin& ww = spin.getWin();
	this->lastWin = ww.getTotal();
	this->totalWin += this->lastWin;
	this->totalWinSquared += this->lastWin * this->lastWin;
	if (this->lastWin == 0)
		this->zeroCount += 1;
	this->credit += this->lastWin;
	this->spinCount += 1;
	if (!respinNext)
	{
		this->betCount += 1;
		this->credit -= spin.getBet();
	}
	if (spin.isFinal())
	{
		respinNext = false;
	}
	else
	{
		respinNext = true;
	}
	this->lastReelset = spin.getReelset();
}

const std::wstring & Statistics::getDescription()
{
	std::wstringstream description;
	description << L"Kredit:\t\t" << this->credit << L"\r\n";
	description << L"Poslední výhra:\t" << this->lastWin << L"\r\n";
	description << L"Sada válcù:\t" << this->lastReelset << L"\r\n";
	description << L"RTP:\t\t" << double(this->totalWin) / (double(this->betCount) * 5.0) * 100.0 << L" %\r\n";
	description << L"Poèet spinù:\t" << this->spinCount << L"\r\n";
	description << L"Poèet sázek:\t" << this->betCount << L"\r\n";
	description << L"Nulové otáèky:\t" << double(this->zeroCount) / double(this->betCount) * 100.0 << L" %\r\n";
	description << L"Následuje respin?\t" << this->respinNext << L"\r\n";
	this->descOut = description.str();
	return this->descOut;
}
