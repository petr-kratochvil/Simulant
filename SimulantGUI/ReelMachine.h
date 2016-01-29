#pragma once
#include <windows.h>
#include <vector>
#include "SimulantCore/Spin.h"

class ReelMachine
{
public:
	ReelMachine(int left, int top, int width, int reels, int rows);
	void draw(HDC hDC, const std::vector<HBITMAP>& symbols, const Spin* spin = nullptr);
protected:
	int left;
	int top;
	int width;
	int height;
	int symbolWidth;
	int symbolHeight;
	int reels;
	int rows;
};