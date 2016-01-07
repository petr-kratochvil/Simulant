#pragma once
#include "Window.h"
#include "JSON/JSON.h"

class ReelSet
{
public:
	ReelSet(JSONValue* source);
	void shuffle();
	void spin();
	Window* getWindow() const;
};