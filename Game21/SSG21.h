#pragma once
#include "SimulantCore/SpinSourceGenerator.h"
#include "Spin21.h"

class SSG21 : public SpinSourceGenerator
{
public:
	SSG21(const SymbolSet* symbolSet, JSONArray reelSets);
	virtual Spin21* getNextSpin();
protected:
	std::vector<const Symbol*> bonusStack;
	std::vector<const Symbol*> bonusStackVisible;
};