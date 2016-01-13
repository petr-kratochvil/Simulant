#pragma once
#include "Symbol.h"
#include "JSON/JSON.h"

class SymbolSet
{
public:
	SymbolSet();
	SymbolSet(const JSONValue* source);
	~SymbolSet();
	static SymbolSet* createDummySet();

	const Symbol& getSymbol(int symbolId) const;

	// Returns assigned id
	int addSymbol(const std::wstring& name, const std::vector<int>& pay);
	int getSymbolCount() const;

protected:
	int symbolCount;
	std::vector<const Symbol*> symbols;
};