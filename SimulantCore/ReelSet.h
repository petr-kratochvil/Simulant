#pragma once
#include "Window.h"
#include "Reel.h"
#include "JSON/JSON.h"

class ReelSet
{
public:
	ReelSet(const JSONValue* source, const SymbolSet& symbolSet);
    ~ReelSet();
	void shuffle();
	void spin();
	void spinToPosition(const std::vector<int>& position);
	// Gets the spin window; if the parametr is nullptr, it allocates a new window,
	// which the caller is responsible to destroy
	Window* getWindow(Window* window = nullptr) const;
	int getReelsCount();
	int getReelLength(int reelID);
	const std::wstring& getName() const;
protected:
    int reelsCount;
	int visibleReelsCount;
	int windowMaxHeight;
	std::vector<Reel*> reels;
	std::wstring name;
};