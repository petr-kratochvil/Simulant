#pragma once
#include <windows.h>
#include <vector>

class ReelMachine
{
public:
	ReelMachine(int left, int top, int width, int reels, int rows);
	void draw(HDC hDC, const std::vector<HBITMAP>& symbols);
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