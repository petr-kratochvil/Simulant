#pragma once
#include "SimulantCore/ReelSet.h"

class ZeroReelSet21 : public ReelSet
{
public:
	ZeroReelSet21(const JSONValue* source, const SymbolSet& symbolSet);
	void setBonusStackSize(int size);
protected:
	virtual Reel& getReel(int position);
	double substitutionPmls[4];
	int bonusStackSize;
};