#pragma once
#include "SpinSource.h"

class SpinSourceGenerator : SpinSource
{
public:
	virtual Spin* getNextSpin() { this->getDummySpin(); }
};