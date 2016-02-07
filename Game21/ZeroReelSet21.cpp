#include "ZeroReelSet21.h"

ZeroReelSet21::ZeroReelSet21(const JSONValue * source, const SymbolSet & symbolSet)
	: ReelSet(source, symbolSet)
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

Reel & ZeroReelSet21::getReel(int position)
{
	if (!this->isSubstituting)
	{
		return *this->reels[this->reelID[position]];
	}
	else
	{
		if (std::find(this->substitutePositions.begin(), this->substitutePositions.end(), position) == this->substitutePositions.end())
			return *this->reels[this->reelID[position]];
		else
		{
			if (Random::genPml(this->substitutionPmls[this->bonusStackSize]))
				return *this->substituteReels[this->reelID[position]];
			else
				return *this->reels[this->reelID[position]];
		}
	}
}
