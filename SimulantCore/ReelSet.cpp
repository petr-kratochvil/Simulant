#include "ReelSet.h"

ReelSet::ReelSet(const JSONValue * source, const SymbolSet& symbolSet)
	: windowMaxHeight(3)
	, permCount(0)
	, isShuffling(false)
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
	this->name = rset[L"name"]->AsString();
	if (rset.find(L"shuffle") != rset.end())
	{
		this->isShuffling = rset[L"shuffle"]->AsBool();
	}
	if (rset.find(L"permutations") != rset.end())
	{
		JSONArray perms = rset[L"permutations"]->AsArray();
		this->permCount = perms.size();
		this->visibleReelsCount = perms[0]->AsArray().size();
		this->permutations.resize(this->permCount);
		for (int i = 0; i < this->permCount; i++)
		{
			this->permutations[i].resize(this->visibleReelsCount);
			for (int j = 0; j < this->visibleReelsCount; j++)
			{
				JSONArray perm = perms[i]->AsArray();
				this->permutations[i][j] = int(perm[j]->AsNumber());
			}
		}
	}
	this->reelID.resize(this->visibleReelsCount);
	for (int i = 0; i < this->visibleReelsCount; i++)
		this->reelID[i] = i;
}

ReelSet::~ReelSet()
{
    for (int i = 0; i < this->reels.size(); i++)
        delete this->reels[i];
}

void ReelSet::shuffle()
{
	// TODO: if permCount == 0, do full shuffle
	int permID = Random::gen(0, this->permCount - 1);
	for (int i = 0; i < this->visibleReelsCount; i++)
	{
		this->reelID[i] = this->permutations[permID][i];
	}
}

void ReelSet::spin()
{
	if (this->isShuffling)
		this->shuffle();
	for (int i = 0; i < this->visibleReelsCount; i++)
		this->reels[this->reelID[i]]->spin();
}

void ReelSet::spinToPosition(const std::vector<int>& position)
{
	for (int i = 0; i < this->visibleReelsCount; i++)
		this->reels[this->reelID[i]]->spinToPosition(position[i]);
}

void ReelSet::spinAndFind21(int symbolID)
{
	this->spin();
	for (int i = 0; i < this->visibleReelsCount; i++)
	{
		while (this->reels[this->reelID[i]]->getSymbol(0).getId() != symbolID)
			this->reels[this->reelID[i]]->spinToPosition(this->reels[this->reelID[i]]->getPosition() + 1);
	}
}

Window * ReelSet::getWindow(Window* window) const
{
	Window* w = window;
	if (w == nullptr)
		w = new Window(this->visibleReelsCount, this->windowMaxHeight);
	for (int x = 0; x < w->getWidth(); x++)
		for (int y = 0; y < w->getHeight(); y++)
			w->setSymbol(x, y, this->reels[this->reelID[x]]->getSymbol(y));
	return w;
}

int ReelSet::getReelsCount()
{
	return this->reelsCount;
}

int ReelSet::getReelLength(int reelID)
{
	return this->reels[reelID]->getLength();
}

const std::wstring & ReelSet::getName() const
{
	return this->name;
}
