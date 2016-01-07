#pragma once
#include "Symbol.h"
#include "JSON/JSON.h"

class Reel
{
public:
	Reel(const JSONValue* source);
	void spin();
	// get the i-th visible symbol from the top of the window,
	// according to the last spin
	const Symbol& getSymbol(int y) const;
};