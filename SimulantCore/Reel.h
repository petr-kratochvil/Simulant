#pragma once
#include "SymbolSet.h"
#include "JSON/JSON.h"

class Reel
{
public:
	Reel(const JSONValue* source, const SymbolSet& symbolSet);
	void spin();
	void spinToPosition(int position);
	// get the i-th visible symbol from the top of the window,
	// according to the last spin
	const Symbol& getSymbol(int y) const;
	int getLength();
protected:
	int length;
    int position;
    std::vector<const Symbol*> symbols;
};