#pragma once
#include "Window.h"

// Represents one spin, or respin, or freespin
class Spin
{
public:
	Spin(Window* window, bool final = true);
	~Spin();

	// Total win amount, including payline wins and bonuses,
	int getWin() const;
	const Window& getWindow() const;
	// Is next spin basic (not a respin)?
	bool isFinal() const;

protected:
	bool final;
	Window* window;
};