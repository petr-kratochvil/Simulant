#pragma once
#include "Symbol.h"
#include "WindowWin.h"

// A visible set of symbols on the screen
// Different types of win can be evaluated
class Window
{
public:
	Window(int width, int height);

	// In case of non-rectangular window, returns the maximum possible indexes
	int getWidth() const;
	int getHeight() const;

	const Symbol& getSymbol(int x, int y) const;
	void setSymbol(int x, int y, const Symbol& symbol);

	// Evaluate the win, with optional stats reported to the StatDelegate
	int winCrissCross() const;
	// implementation of a special case - should be replaced by a general variant
	const WindowWin& winCrissCross3x3();

protected:
	int width, height;
	std::vector<std::vector<const Symbol*>> matrix;
	WindowWin wwin;
};