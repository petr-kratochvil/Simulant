#pragma once

#include "SimulantGUI/GUI.h"
#include "Game21/Spin21.h"

class BonusStack
{
public:
	BonusStack(int left, int top, int width);
	~BonusStack();
	void draw(HDC hDC, const std::vector<HBITMAP>& symbols, const Spin21* spin = nullptr);
protected:
	int left;
	int top;
	int width;
	int height;
	int symbolWidth;
	int symbolHeight;
	ReelMachine* rm;
};

class GUI21 : public GUI
{
public:
	GUI21(HINSTANCE hInstance);
	~GUI21();
	virtual void setNewSpin(const Spin & spin);
protected:
	BonusStack* bonusStack;
};