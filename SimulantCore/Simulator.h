#pragma once
#include "Statistics.h"
#include "SpinSource.h"
#include "Spin.h"
#include <deque>

class Simulator
{
public:
	Simulator(SpinSource& spinSource);
	~Simulator();
	
	// Spin all the respins, included in one bet
	void spinOneBet();
	// Spin just one spin / respin / freespin (one Start-button click)
	void spinOneStart();

	// Next spin is not a respin
	bool isFinal() const;
	
	const Spin& getLastSpin() const;
	void goBackOneSpin();
	bool canGoBack() const;
	int getBet() const;
	void betUp();
	void betDown();

protected:
	SpinSource& spinSource;
	Spin* lastSpin;
	int historySize;
	std::deque<Spin*> spinHistory;
private:
	Simulator(const Simulator&);
	Simulator& operator=(const Simulator&);
};