#include "SpinSourceGeneratorSeq.h"

SpinSourceGeneratorSeq::SpinSourceGeneratorSeq(const SymbolSet* symbolSet, JSONArray reelSets)
	: SpinSourceGenerator(symbolSet, reelSets)
	, seqID(0)
	, totalSeqCount(1)
{
	// TODO throw exception, if number of reelSets > 1
	this->reelsCount = this->reelSets[0]->getReelsCount();
	this->reelLengths.resize(this->reelsCount);
	for (int i = 0; i < this->reelsCount; i++)
	{
		this->reelLengths[i] = this->reelSets[0]->getReelLength(i);
		this->totalSeqCount *= this->reelLengths[i];
	}
}

Spin * SpinSourceGeneratorSeq::getNextSpin()
{
	std::vector<int> positions(this->reelsCount);
	int seq = this->seqID;
	for (int i = 0; i < this->reelsCount; i++)
	{
		positions[i] = seq % this->reelLengths[i];
		seq /= this->reelLengths[i];
	}
	this->reelSets[0]->spinToPosition(positions);
	this->seqID++;
	Window* w = this->reelSets[0]->getWindow();
	return new Spin(w);
}

bool SpinSourceGeneratorSeq::seqEnded()
{
	return (this->seqID >= this->totalSeqCount);
}
