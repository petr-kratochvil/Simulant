#include "Spin21.h"

Spin21::Spin21(Window * window, bool final)
	:Spin(window, final)
{
}

Spin21::Spin21(Window * window, const std::vector<const Symbol*>& bonusStack, bool final)
	: Spin(window, final)
	, bonusStack(bonusStack)
{
}

Spin21::Spin21(const Spin & spin, const std::vector<const Symbol*>& bonusStack, bool final)
	: Spin(spin)
	, bonusStack(bonusStack)
{
	this->final = final;
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
