#include "ReelMachine.h"

ReelMachine::ReelMachine(int left, int top, int width, int reels, int rows)
	: left(left)
	, top(top)
	, width(width)
	, reels(reels)
	, rows(rows)
{
	this->height = int(double(this->width) / double(this->reels) * double(this->rows));
	this->symbolWidth = this->width / this->reels;
	this->symbolHeight = this->height / this->rows;
}

void ReelMachine::draw(HDC hDC)
{
	RECT rect;
	rect.left = this->left, rect.top = this->top;
	rect.right = this->left+this->width, rect.bottom = this->top+this->height;
	HBRUSH hBrBackground = CreateSolidBrush(RGB(255, 255, 128));
	FillRect(hDC, &rect, hBrBackground);
	DeleteObject(hBrBackground);

	// Draw the grid
	for (int j = 0; j <= this->rows; j++)
	{
		MoveToEx(hDC, this->left, this->top + j * this->symbolHeight, NULL);
		LineTo(hDC, this->left + this->width, this->top + j * this->symbolHeight);
	}
	for (int i = 0; i <= this->reels; i++)
	{
		MoveToEx(hDC, this->left + i * this->symbolWidth, this->top, NULL);
		LineTo(hDC, this->left + i * this->symbolWidth, this->top + this->height);
	}
}
