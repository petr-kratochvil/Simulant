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

	Spin* spin = nullptr;
	int fsZeroReelsetId = 22 + (Random::genPct(70)?1:0);
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
			((this->fsWinCount < 1) || (this->fsWinTotal < 50)))
		{
			spin17 = this->rescueSpin1();
			this->rescue = true;
			this->rescueSpin = 1;
			this->reelSetBonus = 20;
		}
		else if
			((this->freeSpinCount == 1) &&
				((this->fsWinCount < 1) || (this->fsWinTotal < 100)))
		{
			spin17 = this->rescueSpin2();
			this->rescue = true;
			this->rescueSpin = 2;
			this->reelSetBonus = 21;
		}
		else
		{
			this->reelSetBonus = Random::gen(0, 1) ? 20 : 21;
			if (this->forceZeroReelsetInFS())
				spin = SpinSourceGenerator::getNextSpin(fsZeroReelsetId);
			else
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
		if (this->fsWinTotal + spin17->getTotalWin() > 1500)
		{
			delete spin17;
			spin = SpinSourceGenerator::getNextSpin(fsZeroReelsetId);
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
		//if (spin17->getTotalWin() > 1500)
		//	__debugbreak();
		break;
	case SSG17::BonusRespin:
		if (this->rescue)
			spin = SpinSourceGenerator::getNextSpin(this->reelSetBonus+4);
		else
			spin = SpinSourceGenerator::getNextSpin(this->reelSetBonus);

			spin17 = new Spin17(*spin, this->payLineSet);
			delete spin;

			spin17->freeze(*this->freezeState);

			if ((spin17->getTotalWin() > 550) || (this->fsWinTotal + spin17->getTotalWin() > 1500))
			{
				delete spin17;
				spin = SpinSourceGenerator::getNextSpin(this->reelSetBonus+4); // we use the reelset "konec"
				spin17 = new Spin17(*spin, this->payLineSet);
				delete spin;
				//if (spin17->getTotalWin() > 1500)
				//	__debugbreak();
				spin17->freeze(*this->freezeState);
				//if (spin17->getTotalWin() > 1500)
				//	__debugbreak();
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
	if (std::find(spin17->getCharacteristics().begin(), spin17->getCharacteristics().end(), L"BonusFinal") != spin17->getCharacteristics().end())
	{
		int wildCount = 0;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 3; j++)
			{
				if (spin17->getWindow().getSymbol(i, j).getId() == 10)
					wildCount++;
			}
		if (wildCount == 0)
			spin17->addCharacteristic(L"WildCount 0");
		else if (wildCount == 1)
			spin17->addCharacteristic(L"WildCount 1");
		else if (wildCount == 2)
			spin17->addCharacteristic(L"WildCount 2");
		else if (wildCount == 3)
			spin17->addCharacteristic(L"WildCount 3");
		else spin17->addCharacteristic(L"WildCount 4+");
	}
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

bool SSG17::forceZeroReelsetInFS()
{
	bool zero = Random::genPct(80);
	switch (this->freeSpinCount)
	{
	case 1:
		/*if (this->fsWinTotal > 1200)
			return true;
		else*/ return zero;
	case 2:
		if (this->fsWinTotal < 100)
			return false;
		else return zero;
	case 3:
		if (this->fsWinTotal < 80)
			return false;
		else return zero;
	case 4:
		if (this->fsWinTotal < 75)
			return false;
		else return zero;
	case 5:
		if (this->fsWinTotal < 60)
			return false;
		else return zero;
	case 6:
		if (this->fsWinTotal < 50)
			return false;
		else return zero;
	case 7:
		if (this->fsWinTotal < 40)
			return false;
		else return zero;
	case 8:
		if (this->fsWinTotal < 20)
			return false;
		else return zero;
	case 9:
	case 10:
	default:
		return false;
	};
}
//((this->freeSpinCount < 5) && (this->fsWinTotal < 75)) || Random::genPct(30)