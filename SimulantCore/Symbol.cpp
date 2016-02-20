#include "Symbol.h"

Symbol::Symbol(int id, const std::wstring& name, const std::vector<int>& pay)
	: id(id)
	, name(name)
	, pay(pay)
	, wild(false)
	, scatter(false)
	, wildApplied(true)
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
		this->wildType = Symbol::Normal;
		if (symbol.find(L"wildType") != symbol.end())
		{
			const std::wstring& wt = symbol[L"wildType"]->AsString();
			if (symbol.find(L"wild") != symbol.end())
			{
				if (wt == L"Normal")
					this->wildType = Symbol::Normal;
				else if (wt == L"EachX2")
					this->wildType = Symbol::EachX2;
			}
		}
	}
	if (symbol.find(L"scatter") != symbol.end())
	{
		this->scatter = symbol[L"scatter"]->AsBool();
		this->wildApplied = false;
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
	if (symbolsInLine == 0)
		return 0;
	if (symbolsInLine > this->pay.size())
		return 0;
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

bool Symbol::isWildApplied() const
{
	return this->wildApplied;
}

bool Symbol::operator==(const Symbol & symbol) const
{
	if (this->isWild() && symbol.isWildApplied()
		|| (symbol.isWild() && this->isWildApplied()))
		return true;
	return this->getId() == symbol.getId();
}

Symbol::WildType Symbol::getWildType() const
{
	return this->wildType;
}
