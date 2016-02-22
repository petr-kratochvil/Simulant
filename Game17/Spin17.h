#pragma once

#include "SimulantCore/Spin.h"

class Spin17 : public Spin
{
public:
	Spin17(Window* window, const PayLineSet& payLineSet, const SymbolSet& symbolSet, bool final = true);
	Spin17(const Spin& spin, const PayLineSet& payLineSet);
	bool getScatterWin() const;
	void setFsCount(int count);
	void freeze(const Window& freezeState);
	virtual int getFsCount() const;
protected:
	virtual void computeWin();
	const PayLineSet& payLineSet;
	bool scatterWin;
	int freeSpinCount;
};