#include "Spin17.h"

Spin17::Spin17(Window * window, const PayLineSet& payLineSet, bool final)
	: Spin(window, final)
	, payLineSet(payLineSet)
	, scatterWin(false)
	, freeSpinCount(0)
{
	this->computeWin();
}

Spin17::Spin17(const Spin& spin, const PayLineSet & payLineSet)
	: Spin(spin)
	, payLineSet(payLineSet)
{
	this->computeWin();
}

bool Spin17::getScatterWin() const
{
	return this->scatterWin;
}

void Spin17::setFsCount(int count)
{
	this->freeSpinCount = count;
	if (this->freeSpinCount > 0)
		this->final = false;
	else
		this->final = true;
}

void Spin17::computeWin()
{
	this->wwin = &this->window->winPayLineSet(this->payLineSet, this->bet);
	if (this->window->winScatter(this->sSet->getSymbol(11)) >= 3)
	{
		this->scatterWin = true;
		this->window->highlightScatter(this->sSet->getSymbol(11));
	}
}
