#include "SSG21.h"
#include "SimulantCore/Random.h"
#include <algorithm>

SSG21::SSG21(const SymbolSet * symbolSet, JSONArray reelSets)
	: SpinSourceGenerator(symbolSet, reelSets)
	, state(SSG21::Basic)
	, reelsetIdBonus(0)
{
}

Spin21 * SSG21::getNextSpin()
{
	Spin21* spin21;

	if (this->state == SSG21::Bonus)
	{
		int bonusPos = Random::gen(0, 3);
		int symbolID = this->bonusStack[bonusPos]->getId();

		this->reelSets[this->reelsetIdBonus]->spinAndFind21(symbolID);
		Window* w = this->reelSets[this->reelsetIdBonus]->getWindow();
		spin21 = new Spin21(w, this->bonusStackVisible, true, bonusPos);
		spin21->setSSet(this->symbolSet);
		spin21->setReelset(this->reelSets[this->reelsetIdBonus]->getName());
		this->state = SSG21::Basic;
		return spin21;
	}

	Spin* spin = SpinSourceGenerator::getNextSpin();
	std::vector<WindowWinItem> winList = spin->getWin().getList();

	if (this->bonusStack.size() >= 4)
	{
		//for (int i = 0; i < 4; i++)
		//	this->bonusStack.erase(this->bonusStack.begin());
		this->bonusStack.clear();
	}
	if (spin->getTotalWin() == 0)
	{
		const Window& w = spin->getWindow();
		for (int i = 0; i < w.getHeight(); i++)
		{
			if (w.getSymbol(1, i).getId() == 7)
			{
				this->bonusStack.pop_back();
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < winList.size(); i++)
		{
			bool found = false;
			for (int j = 0; j < this->bonusStack.size(); j++)
			{
				if (this->bonusStack[j] == &winList[i].symbol)
				{
					found = true;
					break; 
				}
			}
			if (!found)
				this->bonusStack.push_back(&winList[i].symbol);
		}
	}

	this->bonusStackVisible.clear();
	for (int i = 0; i < std::min(int(this->bonusStack.size()), 4); i++)
	{
		this->bonusStackVisible.push_back(this->bonusStack[i]);
	}
	bool final = !(this->bonusStackVisible.size() >= 4);

	if (!final)
		this->state = SSG21::Bonus;

	spin21 = new Spin21(*spin, this->bonusStackVisible, final);
	delete spin;

	return spin21;
}
