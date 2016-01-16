#pragma once
#include "Spin.h"
#include "SymbolSet.h"

class SpinSource
{
public:
	SpinSource(const SymbolSet* symbolSet = nullptr);
	~SpinSource();
	virtual Spin* getNextSpin() = 0;
	Spin* getDummySpin(int width, int height) const;
private:
	SpinSource(const SpinSource&);
	SpinSource& operator=(const SpinSource&);
	const SymbolSet* symbolSet;
	bool symbolSetIsMyOwn;	// symbolSet should be deleted in destructor
};