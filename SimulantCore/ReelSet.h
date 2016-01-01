#pragma once
#include "Window.h"

class ReelSet
{
public:
	void shuffle();
	void spin();
	Window* getWindow() const;
};