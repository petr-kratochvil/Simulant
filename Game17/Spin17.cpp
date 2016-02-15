#include "Spin17.h"

Spin17::Spin17(Window * window, const PayLineSet& payLineSet, bool final)
	: Spin(window, final)
	, payLineSet(payLineSet)
{
	this->computeWin();
}

Spin17::Spin17(const Spin& spin, const PayLineSet & payLineSet)
	: Spin(spin)
	, payLineSet(payLineSet)
{
	this->computeWin();
}

void Spin17::computeWin()
{
	this->wwin = &this->window->winPayLineSet(this->payLineSet, this->bet);
}
