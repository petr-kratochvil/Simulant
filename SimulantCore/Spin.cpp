#include "Spin.h"

Spin::Spin(Window* window, bool final)
	: final(final)
	, window(window)
{
	this->computeWin();
}

Spin::~Spin()
{
	delete this->window;
}

const WindowWin& Spin::getWin() const
{
	return *this->wwin;
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
{
	this->window = new Window(*spin.window);
	this->computeWin();
}
