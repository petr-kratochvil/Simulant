#include "SSG17.h"

SSG17::SSG17(const SymbolSet * symbolSet, JSONArray reelSets, JSONArray payLines)
	: SpinSourceGenerator(symbolSet, reelSets, payLines)
	, state(SSG17::Basic)
	, currentBetId(0)
	, betValues({ 5, 10, 15, 25, 50, 100, 200, 300, 500, 1000 })
	, freeSpinCount(0)
	, freezeState(nullptr)
{
}

Spin17 * SSG17::getNextSpin()
{
	Spin17* spin17 = nullptr;

	// napevno umely spin
	/* Window* w = new Window(5, 3);
	w->setSymbol(0, 0, this->symbolSet->getSymbol(0));
	w->setSymbol(1, 0, this->symbolSet->getSymbol(3));
	w->setSymbol(2, 0, this->symbolSet->getSymbol(2));
	w->setSymbol(3, 0, this->symbolSet->getSymbol(0));
	w->setSymbol(4, 0, this->symbolSet->getSymbol(5));

	w->setSymbol(0, 1, this->symbolSet->getSymbol(0));
	w->setSymbol(1, 1, this->symbolSet->getSymbol(2));
	w->setSymbol(2, 1, this->symbolSet->getSymbol(3));
	w->setSymbol(3, 1, this->symbolSet->getSymbol(10));
	w->setSymbol(4, 1, this->symbolSet->getSymbol(7));

	w->setSymbol(0, 2, this->symbolSet->getSymbol(2));
	w->setSymbol(1, 2, this->symbolSet->getSymbol(4));
	w->setSymbol(2, 2, this->symbolSet->getSymbol(1));
	w->setSymbol(3, 2, this->symbolSet->getSymbol(1));
	w->setSymbol(4, 2, this->symbolSet->getSymbol(4));

	spin17 = new Spin17(w, this->payLineSet, *this->symbolSet);
	return spin17;
	*/////////////////////

	Spin* spin = nullptr;
	
	spin = SpinSourceGenerator::getNextSpin();
	spin17 = new Spin17(*spin, this->payLineSet);
	delete spin;
	if (!this->winIsOK(spin17->getTotalWin()))
	{
		delete spin17;
		spin = SpinSourceGenerator::getNextSpin(0);
		spin17 = new Spin17(*spin, this->payLineSet);
		delete spin;
	}

	switch (this->state)
	{
	case SSG17::Basic:
		break;
	case SSG17::Bonus:
		this->freeSpinCount--;
		if (spin17->getTotalWin() > 0)
		{
			this->state = SSG17::BonusRespin;
			spin17->clearWin();
			if (this->freezeState != nullptr)
				delete this->freezeState;
			this->freezeState = new Window(spin17->getWindow());
		}
		break;
	case SSG17::BonusRespin:
		spin17->freeze(*this->freezeState);
		if (spin17->getWindow().getHighlightedCount() <= this->freezeState->getHighlightedCount())
		{
			this->state = SSG17::Bonus;
		}
		else
		{
			spin17->clearWin();
			if (this->freezeState != nullptr)
				delete this->freezeState;
			this->freezeState = new Window(spin17->getWindow());
		}
		break;
	}

	if ((this->state == SSG17::Bonus) && (this->freeSpinCount <= 0))
		this->state = SSG17::Basic;

	if ((this->state == SSG17::Basic) && spin17->getScatterWin())
	{
		this->state = SSG17::Bonus;
		this->freeSpinCount = 10;
	}

	spin17->setFsCount(this->freeSpinCount);
	spin17->setFinal(this->state == SSG17::Basic);
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

bool SSG17::winIsOK(int win) const
{
	if (win <= 400)
		return true;
	if (win > 1000)
		return false;
	if ((win == 480) || (win == 600) || (win == 640) || (win == 800) || (win == 960) || (win == 1000))
		return true;
	return false;
}
