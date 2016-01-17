#pragma once
#include "SpinSource.h"
#include "ReelSet.h"

class SpinSourceGenerator : public SpinSource
{
public:
	SpinSourceGenerator(const SymbolSet* symbolSet = nullptr);
	SpinSourceGenerator(const SymbolSet* symbolSet, JSONArray reelSets);
	virtual Spin* getNextSpin();
private:
	SpinSourceGenerator(const SpinSourceGenerator&);
	SpinSourceGenerator& operator=(const SpinSourceGenerator&);
protected:
	int reelSetCount;
	std::vector<ReelSet*> reelSets;
	std::vector<int> reelSetPmls;
};