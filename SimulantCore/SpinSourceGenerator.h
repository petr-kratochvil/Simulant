#pragma once
#include "SpinSource.h"

class SpinSourceGenerator : public SpinSource
{
public:
	virtual Spin* getNextSpin() { return this->getDummySpin(5, 3); }
};