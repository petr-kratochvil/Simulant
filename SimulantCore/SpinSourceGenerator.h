#pragma once
#include "SpinSource.h"

class SpinSourceGenerator : public SpinSource
{
public:
	SpinSourceGenerator(const SymbolSet* symbolSet = nullptr);
	virtual Spin* getNextSpin();
private:
	SpinSourceGenerator(const SpinSourceGenerator&);
	SpinSourceGenerator& operator=(const SpinSourceGenerator&);
};