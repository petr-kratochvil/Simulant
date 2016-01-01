#pragma once
#include "Symbol.h"

class SymbolSet
{
public:
	const Symbol& getSymbol(int symbolID);
	void addSymbol(std::wstring name, std::vector<int> pay);
};