#include "ReelSet.h"

ReelSet::ReelSet(const JSONValue * source, const SymbolSet& symbolSet)
	: windowMaxHeight(3)
{
    // TODO throw exception
	JSONObject rset = source->AsObject();
	JSONArray array = rset[L"reels"]->AsArray();
	this->reels.resize(array.size());
	this->reelsCount = array.size();
	this->visibleReelsCount = this->reelsCount;
	for (int i = 0; i < array.size(); i++)
	{
		this->reels[i] = new Reel(array[i], symbolSet);
	}
}

ReelSet::~ReelSet()
{
    for (int i = 0; i < this->reels.size(); i++)
        delete this->reels[i];
}

Window * ReelSet::getWindow(Window* window) const
{
	Window* w = window;
	if (w == nullptr)
		w = new Window(this->visibleReelsCount, this->windowMaxHeight);
	for (int x = 0; x < w->getWidth(); x++)
		for (int y = 0; y < w->getHeight(); y++)
			w->setSymbol(x, y, this->reels[x]->getSymbol(y));
	return w;
}
