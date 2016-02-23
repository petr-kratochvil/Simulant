#include "SSG17.h"

SSG17::SSG17(const SymbolSet * symbolSet, JSONArray reelSets, JSONArray payLines)
	: SpinSourceGenerator(symbolSet, reelSets, payLines)
	, state(SSG17::Basic)
	, currentBetId(0)
	, betValues({ 5, 10, 15, 25, 50, 100, 200, 300, 500, 1000 })
	, freeSpinCount(0)
	, freezeState(nullptr)
	, rescue(false)
{
}

Spin17 * SSG17::getNextSpin()
{
	Spin17* spin17 = nullptr;

	// napevno umely spin
	
	/////////////////////

	Spin* spin = nullptr;
	switch (this->state)
	{
	case SSG17::Basic:
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
		break;
	case SSG17::Bonus:
		this->rescue = false;
		if ((this->freeSpinCount ==2) &&
			((this->fsWinCount < 2) || (this->fsWinTotal < 100)))
		{
			spin17 = this->rescueSpin1();
			this->rescue = true;
			this->rescueSpin = 1;
			this->reelSetBonus = 21;
		}
		else if
			((this->freeSpinCount == 1) &&
				((this->fsWinCount < 2) || (this->fsWinTotal < 100)))
		{
			spin17 = this->rescueSpin2();
			this->rescue = true;
			this->rescueSpin = 2;
			this->reelSetBonus = 20;
		}
		else
		{
			this->reelSetBonus = Random::gen(0, 1) ? 20 : 21;
			spin = SpinSourceGenerator::getNextSpin(this->reelSetBonus);
			spin17 = new Spin17(*spin, this->payLineSet);
			delete spin;
		}
		break;
	case SSG17::BonusRespin:
		

		break;
	}

	switch (this->state)
	{
	case SSG17::Basic:
		break;
	case SSG17::Bonus:
		this->freeSpinCount--;
		if (this->fsWinTotal + spin17->getTotalWin() > 1440)
		{
			delete spin17;
			spin = SpinSourceGenerator::getNextSpin(22);
			spin17 = new Spin17(*spin, this->payLineSet);
			delete spin;
		}
		if (spin17->getTotalWin() > 0)
		{
			this->state = SSG17::BonusRespin;
			spin17->clearWin();
			if (this->freezeState != nullptr)
				delete this->freezeState;
			this->freezeState = new Window(spin17->getWindow());
		}
		else
		{
			spin17->addCharacteristic(L"BonusFinal");
		}
		break;
	case SSG17::BonusRespin:
			spin = SpinSourceGenerator::getNextSpin(this->reelSetBonus);
			spin17 = new Spin17(*spin, this->payLineSet);
			delete spin;

			spin17->freeze(*this->freezeState);

			if ((spin17->getTotalWin() > 1000) || (this->fsWinTotal + spin17->getTotalWin() > 1440))
			{
				delete spin17;
				spin = SpinSourceGenerator::getNextSpin(41-this->reelSetBonus);
				spin17 = new Spin17(*spin, this->payLineSet);
				delete spin;
				spin17->freeze(*this->freezeState);
			}

			if (spin17->getWindow().getHighlightedCount() <= this->freezeState->getHighlightedCount())
			{
				this->state = SSG17::Bonus;
				spin17->addCharacteristic(L"BonusFinal");
				this->fsWinCount++;
				this->fsWinTotal += spin17->getTotalWin();
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
		this->fsWinCount = 0;
		this->fsWinTotal = 0;
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

Spin17 * SSG17::rescueSpin1()
{
	Window* w = new Window(5, 3);
	w->setSymbol(0, 0, this->symbolSet->getSymbol(3));
	w->setSymbol(1, 0, this->symbolSet->getSymbol(3));
	w->setSymbol(2, 0, this->symbolSet->getSymbol(3));
	w->setSymbol(3, 0, this->symbolSet->getSymbol(3));
	w->setSymbol(4, 0, this->symbolSet->getSymbol(7));

	w->setSymbol(0, 1, this->symbolSet->getSymbol(5));
	w->setSymbol(1, 1, this->symbolSet->getSymbol(5));
	w->setSymbol(2, 1, this->symbolSet->getSymbol(1));
	w->setSymbol(3, 1, this->symbolSet->getSymbol(5));
	w->setSymbol(4, 1, this->symbolSet->getSymbol(0));

	w->setSymbol(0, 2, this->symbolSet->getSymbol(0));
	w->setSymbol(1, 2, this->symbolSet->getSymbol(0));
	w->setSymbol(2, 2, this->symbolSet->getSymbol(5));
	w->setSymbol(3, 2, this->symbolSet->getSymbol(0));
	w->setSymbol(4, 2, this->symbolSet->getSymbol(7));

	Spin17* spin17 = new Spin17(w, this->payLineSet, *this->symbolSet);
	return spin17;
}

Spin17 * SSG17::rescueSpin2()
{
	Window* w = new Window(5, 3);
	w->setSymbol(0, 0, this->symbolSet->getSymbol(2));
	w->setSymbol(1, 0, this->symbolSet->getSymbol(2));
	w->setSymbol(2, 0, this->symbolSet->getSymbol(2));
	w->setSymbol(3, 0, this->symbolSet->getSymbol(2));
	w->setSymbol(4, 0, this->symbolSet->getSymbol(6));

	w->setSymbol(0, 1, this->symbolSet->getSymbol(4));
	w->setSymbol(1, 1, this->symbolSet->getSymbol(4));
	w->setSymbol(2, 1, this->symbolSet->getSymbol(1));
	w->setSymbol(3, 1, this->symbolSet->getSymbol(4));
	w->setSymbol(4, 1, this->symbolSet->getSymbol(0));

	w->setSymbol(0, 2, this->symbolSet->getSymbol(0));
	w->setSymbol(1, 2, this->symbolSet->getSymbol(0));
	w->setSymbol(2, 2, this->symbolSet->getSymbol(4));
	w->setSymbol(3, 2, this->symbolSet->getSymbol(0));
	w->setSymbol(4, 2, this->symbolSet->getSymbol(1));

	Spin17* spin17 = new Spin17(w, this->payLineSet, *this->symbolSet);
	return spin17;
}
