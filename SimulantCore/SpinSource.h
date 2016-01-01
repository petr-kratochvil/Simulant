#pragma once
#include "Spin.h"

class SpinSource
{
public:
	virtual Spin* getNextSpin() = 0;
	Spin* getDummySpin() { return new Spin(); }
};