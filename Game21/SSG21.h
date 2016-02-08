#pragma once
#include "SimulantCore/SpinSourceGenerator.h"
#include "Spin21.h"
#include "ZeroReelSet21.h"

class SSG21 : public SpinSourceGenerator
{
	enum State
	{
		Basic,
		Bonus
	} state;
public:
	SSG21(const SymbolSet* symbolSet, JSONArray reelSets);
	virtual Spin21* getNextSpin();
protected:
	std::vector<const Symbol*> bonusStack;
	std::vector<const Symbol*> bonusStackVisible;
	Window* windowBonus;
	int reelsetIdBonus;
	int zeroReelSetId;
	std::vector<int> bonusCreationPmls;
};