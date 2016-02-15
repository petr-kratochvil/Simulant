#include "Symbol.h"

Symbol::Symbol(int id, const std::wstring& name, const std::vector<int>& pay)
	: id(id)
	, name(name)
	, pay(pay)
	, wild(false)
	, scatter(false)
{
}

Symbol::Symbol(const JSONValue* source)
	: wild(false)
	, scatter(false)
{
	// TODO throw exception
	JSONObject symbol = source->AsObject();
	this->id = symbol[L"id"]->AsNumber();
	this->name = symbol[L"name"]->AsString();
	
	JSONArray array = symbol[L"pay"]->AsArray();
	this->pay.resize(array.size());
	for (int i = 0; i < array.size(); i++)
		this->pay[i] = array[i]->AsNumber();
	if (symbol.find(L"wild") != symbol.end())
	{
		this->wild = symbol[L"wild"]->AsBool();
	}
	if (symbol.find(L"scatter") != symbol.end())
	{
		this->wild = symbol[L"scatter"]->AsBool();
	}
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
	return this->pay[symbolsInLine-1];
}

bool Symbol::isWild() const
{
	return this->wild;
}

bool Symbol::isScatter() const
{
	return this->scatter;
}

bool Symbol::operator==(const Symbol & symbol) const
{
	if (this->isWild() || symbol.isWild())
		return true;
	return this->getId() == symbol.getId();
}
