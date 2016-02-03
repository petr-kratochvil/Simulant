#include "Spin.h"

Spin::Spin(Window* window, bool final)
	: final(final)
	, window(window)
	, bet(5)
{
	this->computeWin();
	this->sSet = nullptr;
}

Spin::~Spin()
{
	delete this->window;
}

const WindowWin& Spin::getWin() const
{
	return *this->wwin;
}

void Spin::setSSet(const SymbolSet * sSet)
{
	this->sSet = sSet;
}

const SymbolSet * Spin::getSSet() const
{
	return this->sSet;
}

void Spin::setBet(int bet)
{
	this->bet = bet;
}

int Spin::getBet() const
{
	return this->bet;
}

const std::wstring& Spin::getReelset() const
{
	return this->reelsetUsed;
}

void Spin::setReelset(const std::wstring & reelset)
{
	this->reelsetUsed = reelset;
}

int Spin::getTotalWin() const
{
	return this->wwin->getTotal();
}

const Window & Spin::getWindow() const
{
	return *this->window;
}

bool Spin::isFinal() const
{
	return this->final;
}

void Spin::computeWin()
{
	this->wwin = &this->window->winCrissCross3x3();
}

Spin::Spin(const Spin& spin)
	: final(spin.final)
	, sSet(spin.sSet)
	, bet(spin.bet)
{
	this->window = new Window(*spin.window);
	this->computeWin();
}
