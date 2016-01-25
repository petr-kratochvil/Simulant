#pragma once
#include <windows.h>

class ReelMachine
{
public:
	ReelMachine(int left, int top, int width, int height);
	void draw(HDC hDC);
protected:
	int left;
	int top;
	int width;
	int height;
};