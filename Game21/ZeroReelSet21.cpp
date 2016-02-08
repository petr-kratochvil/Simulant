#include "ZeroReelSet21.h"

ZeroReelSet21::ZeroReelSet21(const JSONValue * source, const SymbolSet & symbolSet)
	: ReelSet(source, symbolSet)
	, isBonusSubstituting(false)
{
	JSONObject rset = source->AsObject();
	JSONArray probs = rset[L"substitutePmls21"]->AsArray();
	for (int i = 0; i < 4; i++)
		this->substitutionPmls[i] = probs[i]->AsNumber();
}

void ZeroReelSet21::setBonusStackSize(int size)
{
	this->bonusStackSize = size;
}

void ZeroReelSet21::spin()
{
	ReelSet::spin();
	if (Random::genPml(this->substitutionPmls[this->bonusStackSize]))
		this->isBonusSubstituting = true;
	else
		this->isBonusSubstituting = false;
}

Reel & ZeroReelSet21::getReel(int position)
{
	if (!this->isBonusSubstituting)
		return ReelSet::getReel(position);
	else
	{
		if (std::find(this->substitutePositions.begin(), this->substitutePositions.end(), position) == this->substitutePositions.end())
			return ReelSet::getReel(position);
		else
		{
			return *this->substituteBonusReels21[this->reelID[position]];
		}
	}
}
