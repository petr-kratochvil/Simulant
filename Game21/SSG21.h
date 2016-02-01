#pragma once
#include "SimulantCore/SpinSourceGenerator.h"

class SSG21 : public SpinSourceGenerator
{
public:
	SSG21(const SymbolSet* symbolSet, JSONArray reelSets);
};