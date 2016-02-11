#include "SpinSource.h"
#include <algorithm>


SpinSource::SpinSource(const SymbolSet* symbolSet)
	: symbolSet(symbolSet)
	, symbolSetIsMyOwn(false)
	, bet(5)
{
	if (this->symbolSet == nullptr)
	{
		this->symbolSet = SymbolSet::createDummySet();
		this->symbolSetIsMyOwn = true;
	}
}

SpinSource::~SpinSource()
{
	if (this->symbolSetIsMyOwn)
		delete this->symbolSet;
}

Spin* SpinSource::getDummySpin(int width, int height) const
{
	const SymbolSet* ss = this->symbolSet;
	Window* window = new Window(width, height);
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			window->setSymbol(i, j, ss->getSymbol(0));
		}
	return new Spin(window);
}

int SpinSource::getBet() const
{
	return this->bet;
}

void SpinSource::setBet(int bet)
{
	this->bet = bet;
}

void SpinSource::betUp()
{
	this->setBet(this->getBet() + 5);
}

void SpinSource::betDown()
{
	this->setBet(std::max(5, this->getBet() - 5));
}
