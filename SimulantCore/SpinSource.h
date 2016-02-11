#pragma once
#include "Spin.h"
#include "SymbolSet.h"

class SpinSource
{
public:
	SpinSource(const SymbolSet* symbolSet = nullptr);
	~SpinSource();
	virtual Spin* getNextSpin() = 0;
	Spin* getDummySpin(int width, int height) const;
	virtual int getBet() const;
	virtual void setBet(int bet);
	virtual void betUp();
	virtual void betDown();
private:
	SpinSource(const SpinSource&);
	SpinSource& operator=(const SpinSource&);
	bool symbolSetIsMyOwn;	// symbolSet should be deleted in destructor
protected:
	const SymbolSet* symbolSet;
	int bet;
};