#include "Symbol.h"

Symbol::Symbol(int id, const std::wstring& name, const std::vector<int>& pay)
	: id(id)
	, name(name)
	, pay(pay)
{
}

Symbol::Symbol(const JSONValue* source)
{
	// TODO throw exception
	JSONObject symbol = source->AsObject();
	this->id = symbol[L"id"]->AsNumber();
	this->name = symbol[L"name"]->AsString();
	
	JSONArray array = symbol[L"pay"]->AsArray();
	this->pay.resize(array.size());
	for (int i = 0; i < array.size(); i++)
		this->pay[i] = array[i]->AsNumber();
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
