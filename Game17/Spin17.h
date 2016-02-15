#pragma once

#include "SimulantCore/Spin.h"

class Spin17 : public Spin
{
public:
	Spin17(Window* window, const PayLineSet& payLineSet, bool final = true);
	Spin17(const Spin& spin, const PayLineSet& payLineSet);
protected:
	virtual void computeWin();
	const PayLineSet& payLineSet;
};