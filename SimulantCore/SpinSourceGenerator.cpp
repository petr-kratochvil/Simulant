#include "SpinSourceGenerator.h"
#include "JSON/JSON.h"
#include "Random.h"

SpinSourceGenerator::SpinSourceGenerator(const SymbolSet * symbolSet)
	: SpinSource(symbolSet)
	, reelSetCount(0)
{
}

SpinSourceGenerator::SpinSourceGenerator(const SymbolSet * symbolSet, JSONArray reelSets)
	: SpinSource(symbolSet)
{
	this->reelSetCount = reelSets.size();
	this->reelSets.resize(this->reelSetCount);
	this->reelSetPmls.resize(this->reelSetCount);
	
	int totalPml = 0;
	for (int i = 0; i < this->reelSetCount; i++)
	{
		this->reelSets[i] = new ReelSet(reelSets[i], *symbolSet);
		JSONObject rs = reelSets[i]->AsObject();
		totalPml += int(rs[L"pml"]->AsNumber());
		this->reelSetPmls[i] = totalPml;
	}
	// TODO throw exception if totalPml > 1000
}

Spin * SpinSourceGenerator::getNextSpin()
{
	int pml = Random::gen(0, 999);
	int reelSetChosen = -1;
	for (int i = 0; i < this->reelSetCount; i++)
	{
		if (pml < this->reelSetPmls[i])
		{
			reelSetChosen = i;
			break;
		}
	}
	// TODO throw exception if reelSetChosen == -1
	this->reelSets[reelSetChosen]->spin();
	Window* w = this->reelSets[reelSetChosen]->getWindow();
	Spin* spin = new Spin(w);
	spin->setSSet(this->symbolSet);
	spin->setReelset(this->reelSets[reelSetChosen]->getName());
	return spin;
}
