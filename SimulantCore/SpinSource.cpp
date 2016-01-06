#include "SpinSource.h"

SpinSource::SpinSource(const SymbolSet* symbolSet)
	: symbolSet(symbolSet)
	, symbolSetIsMyOwn(false)
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
	for (int i = 1; i <= width; i++)
		for (int j = 1; j <= height; j++)
		{
			window->setSymbol(i, j, ss->getSymbol(0));
		}
	return new Spin(window);
}
