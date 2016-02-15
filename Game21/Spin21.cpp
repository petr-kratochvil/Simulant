#include "Spin21.h"

Spin21::Spin21(Window * window, bool final)
	:Spin(window, final)
	, bonusStackWinPosition(-1)
{
	this->computeWin();
}

Spin21::Spin21(Window * window, const std::vector<const Symbol*>& bonusStack, bool final, int bonusStackWinPosition)
	: Spin(window, final)
	, bonusStack(bonusStack)
	, bonusStackWinPosition(bonusStackWinPosition)
{
	this->computeWin();
}

Spin21::Spin21(const Spin & spin, const std::vector<const Symbol*>& bonusStack, bool final)
	: Spin(spin)
	, bonusStack(bonusStack)
	, bonusStackWinPosition(-1)
{
	this->final = final;
	this->computeWin();
}

void Spin21::stackPop()
{
	this->bonusStack.pop_back();
}

void Spin21::stackPush(const Symbol & symbol)
{
	this->bonusStack.push_back(&symbol);
}

const std::vector<const Symbol*>& Spin21::getStack() const
{
	return this->bonusStack;
}

int Spin21::getBonusPosition() const
{
	return this->bonusStackWinPosition;
}

void Spin21::computeWin()
{
	this->wwin = &this->window->winCrissCross3x3_game21(this->bet);
}