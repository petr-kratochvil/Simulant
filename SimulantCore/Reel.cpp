#include "Reel.h"
#include "Random.h"

Reel::Reel(const JSONValue * source, const SymbolSet& symbolSet)
    : position(0)
{
	// TODO throw exception
	JSONObject reel = source->AsObject();
	JSONArray array = reel[L"symbols"]->AsArray();
	this->symbols.resize(array.size());
	this->length = array.size();
	for (int i = 0; i < array.size(); i++)
	{
		this->symbols[i] = &symbolSet.getSymbol(int(array[i]->AsNumber()));
	}
}

void Reel::spin()
{
	this->position = Random::gen(0, length - 1);
}

void Reel::spinToPosition(int position)
{
	// TODO throw exception
	this->position = position;
}

const Symbol& Reel::getSymbol(int y) const
{
	return *this->symbols[(this->position + y) % this->length];
}

int Reel::getLength()
{
	return this->length;
}
