#pragma once
#include "Symbol.h"

class Reel
{
public:
	void spin();
	// get the i-th visible symbol from the top of the window,
	// according to the last spin
	const Symbol& getSymbol(int y) const;
};