#include "ReelMachine.h"

ReelMachine::ReelMachine(int left, int top, int width, int height)
	: left(left)
	, top(top)
	, width(width)
	, height(height)
{
}

void ReelMachine::draw(HDC hDC)
{
	RECT rect;
	rect.left = this->left, rect.top = this->top;
	rect.right = this->left+this->width, rect.bottom = this->top+this->height;
	HBRUSH hBrBackground = CreateSolidBrush(RGB(255, 255, 128));
	FillRect(hDC, &rect, hBrBackground);
	DeleteObject(hBrBackground);
}
