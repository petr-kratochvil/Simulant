#pragma once
#include "Window.h"

// Represents one spin, or respin, or freespin
class Spin
{
public:
	Spin(Window* window, bool final = true);
	~Spin();

	// Total win amount, including payline wins and bonuses,
	int getTotalWin() const;
	const Window& getWindow() const;
	// Is next spin basic (not a respin)?
	bool isFinal() const;
	const WindowWin& getWin() const;

protected:
	bool final;
	Window* window;
	void computeWin();
	const WindowWin* wwin;
private:
	Spin(const Spin&);
	Spin& operator=(const Spin&);
};