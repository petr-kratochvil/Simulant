#include "WindowWin.h"

int WindowWin::getTotal() const
{
	return this->totalWin;
}

int WindowWin::getLinesCount() const
{
	return this->linesCount;
}

const std::vector<WindowWinItem>& WindowWin::getList() const
{
	return this->winList;
}

WindowWin::WindowWin()
	: linesCount(0)
	, totalWin(0)
{
}

void WindowWin::clear()
{
	this->winList.clear();
	this->linesCount = 0;
	this->totalWin = 0;
}

void WindowWin::addLine(const Symbol & symbol, int count, int pay)
{
	this->winList.push_back({ symbol, count, pay });
	this->linesCount++;
	this->totalWin += pay;
}
