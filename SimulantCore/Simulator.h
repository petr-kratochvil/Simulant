#pragma once
#include "Statistics.h"
#include "SpinSource.h"
#include "Spin.h"

class Simulator
{
public:
	Simulator(SpinSource& spinSource);
	~Simulator();
	
	// Spin all the respins, included in one bet
	void spinOneBet(Statistics* statistics = nullptr);
	// Spin just one spin / respin / freespin (one Start-button click)
	void spinOneStart(Statistics* statistics = nullptr);

	// Next spin is not a respin
	bool isFinal() const;
	
	const Spin& getLastSpin() const;

protected:
	SpinSource& spinSource;
	Spin* lastSpin;
private:
	Simulator(const Simulator&);
	Simulator& operator=(const Simulator&);
};