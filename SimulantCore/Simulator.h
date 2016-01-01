#pragma once
#include "Statistics.h"
#include "Spin.h"

class Simulator
{
public:
	Simulator();
	
	// Spin all the respins, included in one bet
	void spinOneBet(Statistics* statistics = nullptr);
	// Spin just one spin / respin / freespin (one Start-button click)
	void spinOneStart(Statistics* statistics = nullptr);

	// Next spin is not a respin
	bool isNextSpinBasic();
	
	const Spin& getLastSpin();
};