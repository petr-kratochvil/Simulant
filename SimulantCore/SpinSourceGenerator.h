#pragma once
#include "SpinSource.h"

class SpinSourceGenerator : public SpinSource
{
public:
	SpinSourceGenerator(const SymbolSet* symbolSet = NULL);
	virtual Spin* getNextSpin();
private:
	SpinSourceGenerator(const SpinSourceGenerator&);
	SpinSourceGenerator& operator=(const SpinSourceGenerator&);
};