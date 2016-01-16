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
	// Gets the spin window; if the parametr is nullptr, it allocates a new window,
	// which the caller is responsible to destroy
	Window* getWindow(Window* window = nullptr) const;
protected:
    int reelsCount;
	int visibleReelsCount;
	int windowMaxHeight;
	std::vector<Reel*> reels;
};