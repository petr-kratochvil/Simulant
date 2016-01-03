#include "Spin.h"

Spin::Spin(Window* window, bool final)
	: final(final)
	, window(window)
{}

Spin::~Spin()
{
	delete this->window;
}

int Spin::getWin() const
{
	return 123;
}

const Window & Spin::getWindow() const
{
	return *this->window;
}

bool Spin::isFinal() const
{
	return this->final;
}
