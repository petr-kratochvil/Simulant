#include "SpinSource.h"

SpinSource::SpinSource()
	: dummySet(SymbolSet::createDummySet())
{
}

SpinSource::~SpinSource()
{
	delete this->dummySet;
}

Spin* SpinSource::getDummySpin(int width, int height, const SymbolSet* ss)
{
	if (ss == nullptr)
		ss = this->dummySet;
	Window* window = new Window(width, height);
	for (int i = 1; i <= width; i++)
		for (int j = 1; j <= height; j++)
		{
			window->setSymbol(i, j, ss->getSymbol(0));
		}
	return new Spin(window);
}
