#include "Simulator.h"

Simulator::Simulator(SpinSource & spinSource)
	: spinSource(spinSource)
	, lastSpin(nullptr)
{
}

Simulator::~Simulator()
{
	if (this->lastSpin != nullptr)
		delete this->lastSpin;
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
	delete this->lastSpin;
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

