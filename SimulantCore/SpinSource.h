#pragma once
#include "Spin.h"
#include "SymbolSet.h"

class SpinSource
{
public:
	SpinSource();
	~SpinSource();
	virtual Spin* getNextSpin() = 0;
	Spin* getDummySpin(int width, int height, const SymbolSet* ss = NULL);
private:
	SymbolSet* dummySet;
};