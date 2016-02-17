#include "SSG17.h"

SSG17::SSG17(const SymbolSet * symbolSet, JSONArray reelSets, JSONArray payLines)
	: SpinSourceGenerator(symbolSet, reelSets, payLines)
	, state(SSG17::Basic)
	, currentBetId(0)
	, betValues({ 5, 10, 15, 25, 50, 100, 200, 300, 500, 1000 })
	, freeSpinCount(0)
{
}

Spin17 * SSG17::getNextSpin()
{
	Spin17* spin17;
	Spin* spin = SpinSourceGenerator::getNextSpin();
	spin17 = new Spin17(*spin, this->payLineSet);
	delete spin;
	if (spin17->getScatterWin())
		this->freeSpinCount += 10;
	if (this->freeSpinCount > 0)
		this->state = SSG17::Bonus;
	else
		this->state = SSG17::Basic;
	spin17->setFsCount(this->freeSpinCount);
	return spin17;
}

void SSG17::betUp()
{
	this->currentBetId = (this->currentBetId + 1) % this->betValues.size();
	SpinSourceGenerator::setBet(this->betValues[this->currentBetId]);
}

void SSG17::betDown()
{
	this->currentBetId = (this->currentBetId + this->betValues.size() - 1) % this->betValues.size();
	SpinSourceGenerator::setBet(this->betValues[this->currentBetId]);
}
