#include "Statistics.h"


Statistics::Statistics(int symbolCount, int credit)
	: totalWin(0)
	, totalWinSquared(0)
	, spinCount(0)
	, betCount(0)
	, zeroCount(0)
	, credit(credit)
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
	this->totalWin += ww.getTotal();
	this->totalWinSquared += ww.getTotal() * ww.getTotal();
	if (ww.getTotal() == 0)
		this->zeroCount += 1;
	this->credit += ww.getTotal();
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
}

const std::wstring & Statistics::getDescription()
{
	std::wstringstream description;
	description << L"Kredit:\t" << this->credit << L"\r\n";
	description << L"Poèet spinù:\t" << this->spinCount << L"\r\n";
	description << L"Poèet sázek:\t" << this->betCount << L"\r\n";
	description << L"Nulové otáèky:\t" << double(this->zeroCount) / double(this->betCount) * 100.0 << L"%\r\n";
	description << L"Následuje respin?\t" << this->respinNext << L"\r\n";
	this->descOut = description.str();
	return this->descOut;
}
