#include "Symbol.h"

Symbol::Symbol(int id, const std::wstring& name, const std::vector<int>& pay)
	: id(id)
	, name(name)
	, pay(pay)
{
	this->pay.insert(this->pay.begin(), 0);
}

const std::wstring& Symbol::getName() const
{
	return this->name;
}

int Symbol::getId() const
{
	return this->id;
}

int Symbol::getWin(int symbolsInLine) const
{
	// TODO throw exception
	return this->pay[symbolsInLine];
}
