#include "Simulator.h"

Simulator::Simulator(SpinSource & spinSource)
	: spinSource(spinSource)
	, lastSpin(nullptr)
{
}

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

bool Simulator::isFinal()
{
	// TODO throw exception
	return this->lastSpin->isFinal();
}

const Spin& Simulator::getLastSpin()
{
	// TODO throw exception
	return *this->lastSpin;
}

