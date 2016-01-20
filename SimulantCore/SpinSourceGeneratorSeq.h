#pragma once
#include "SpinSourceGenerator.h"

class SpinSourceGeneratorSeq : public SpinSourceGenerator
{
public:
	SpinSourceGenerator(const SymbolSet* symbolSet, JSONArray reelSets);
	virtual Spin* getNextSpin();
	bool seqEnded();
protected:
	// ID of the next spin to be done
	int seqID;
	// Total count of spins in the sequence
	int totalSeqCount;
	int reelsCount;
	std::vector<int> reelLengths;
};