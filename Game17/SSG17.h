#pragma once

#include "SimulantCore/SpinSourceGenerator.h"
#include "Spin17.h"

class SSG17 : public SpinSourceGenerator
{
public:
	SSG17(const SymbolSet* symbolSet, JSONArray reelSets, JSONArray payLines);
	virtual Spin17* getNextSpin();
	virtual void betUp();
	virtual void betDown();
private:
	enum State
	{
		Basic,
		Bonus,
		BonusRespin
	} state;
	std::vector<int> betValues;
	int currentBetId;
	int freeSpinCount;
	Window* freezeState;
};