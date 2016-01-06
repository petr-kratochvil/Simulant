#include "SpinSourceGenerator.h"

SpinSourceGenerator::SpinSourceGenerator(const SymbolSet * symbolSet)
	: SpinSource(symbolSet)
{
}

Spin * SpinSourceGenerator::getNextSpin()
{
	return this->getDummySpin(5, 3);
}
