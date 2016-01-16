#include "SymbolSet.h"

SymbolSet::SymbolSet()
	: symbolCount(0)
{
}

SymbolSet::SymbolSet(const JSONValue* source)
{
	// TODO throw exception
	JSONObject sset = source->AsObject();
	JSONArray array = sset[L"symbolSet"]->AsArray();
	this->symbols.resize(array.size());
	this->symbolCount = array.size();
	for (int i = 0; i < array.size(); i++)
	{
		this->symbols[i] = new Symbol(array[i]);
	}
}

SymbolSet::~SymbolSet()
{
	for (int i = 0; i < this->symbols.size(); i++)
		delete this->symbols[i];
}

SymbolSet * SymbolSet::createDummySet()
{
	SymbolSet* ss = new SymbolSet();
	ss->addSymbol(L"Třešeň", { 0,5,10,15,20 });
	ss->addSymbol(L"Citron", { 0,0,10,15,20 });
	ss->addSymbol(L"Pomeranč", { 0,0,20,30,50 });
	ss->addSymbol(L"Švestka", { 0,0,20,30,50 });
	ss->addSymbol(L"Meloun", { 0,0,50,75,100 });
	ss->addSymbol(L"Hrozny", { 0,0,50,75,100 });
	ss->addSymbol(L"Sedm", { 0,0,75,100,150 });
	return ss;
}

const Symbol& SymbolSet::getSymbol(int symbolId) const
{
	return *this->symbols[symbolId];
}

int SymbolSet::addSymbol(const std::wstring& name, const std::vector<int>& pay)
{
	this->symbols.push_back(new Symbol(this->symbolCount, name, pay));
	return this->symbolCount++;
}

int SymbolSet::getSymbolCount() const
{
	return this->symbolCount;
}
