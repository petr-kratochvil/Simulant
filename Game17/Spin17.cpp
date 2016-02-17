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
	, scatterWin(false)
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
}

void Spin17::freeze(const Window & freezeState)
{
	for (int i = 0; i < this->window->getWidth(); i++)
		for (int j = 0; j < this->window->getHeight(); j++)
			if (freezeState.highlighted(i, j))
				this->window->setSymbol(i, j, freezeState.getSymbol(i, j));
	this->computeWin();
}

int Spin17::getFsCount() const
{
	return this->freeSpinCount;
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
