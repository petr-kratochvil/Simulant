#include "Spin21.h"

Spin21::Spin21(Window * window, bool final)
	:Spin(window, final)
{
}

Spin21::Spin21(const Spin & spin, const std::vector<const Symbol*>& bonusStack)
	: Spin(spin)
	, bonusStack(bonusStack)
{
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
