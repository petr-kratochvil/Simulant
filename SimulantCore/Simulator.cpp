#include "Simulator.h"

Simulator::Simulator(SpinSource & spinSource)
	: spinSource(spinSource)
	, lastSpin(nullptr)
	, historySize(50)
{
}

Simulator::~Simulator()
{
	if (this->lastSpin != nullptr)
		delete this->lastSpin;
	for (int i = 0; i < this->spinHistory.size(); i++)
		delete this->spinHistory[i];
}

// TODO zatim nefunguje, nezapocita non-final spiny do statistiky
void Simulator::spinOneBet(Statistics * statistics)
{
	do
	{
		this->spinOneStart(statistics);
	} while (!this->lastSpin->isFinal());
}

void Simulator::spinOneStart(Statistics * statistics)
{
	// TODO throw exception
	Spin* newSpin = this->spinSource.getNextSpin();

	if (this->lastSpin != nullptr)
		this->spinHistory.push_back(this->lastSpin);
	if (this->spinHistory.size() > this->historySize)
	{
		delete this->spinHistory.front();
		this->spinHistory.pop_front();
	}
	this->lastSpin = newSpin;
}

bool Simulator::isFinal() const
{
	// TODO throw exception
	return this->lastSpin->isFinal();
}

const Spin& Simulator::getLastSpin() const
{
	// TODO throw exception
	return *this->lastSpin;
}

void Simulator::goBackOneSpin()
{
	// TODO throw exception
	if (this->lastSpin != nullptr)
		delete this->lastSpin;
	this->lastSpin = this->spinHistory.back();
	this->spinHistory.pop_back();
}

bool Simulator::canGoBack() const
{
	return !this->spinHistory.empty();
}

