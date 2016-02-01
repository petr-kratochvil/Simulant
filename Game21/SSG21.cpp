#include "SSG21.h"

SSG21::SSG21(const SymbolSet * symbolSet, JSONArray reelSets)
	: SpinSourceGenerator(symbolSet, reelSets)
{
}

Spin21 * SSG21::getNextSpin()
{
	Spin* spin = SpinSourceGenerator::getNextSpin();
	std::vector<WindowWinItem> winList = spin->getWin().getList();

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

	Spin21* spin21 = new Spin21(*spin, this->bonusStack);
	delete spin;

	return spin21;
}
