#include "ReelMachine.h"
#include "SimulantCore/Random.h"

ReelMachine::ReelMachine(int left, int top, int width, int reels, int rows)
	: left(left)
	, top(top)
	, width(width)
	, reels(reels)
	, rows(rows)
{
	this->height = ceil(double(this->width) / double(this->reels) * double(this->rows));
	this->symbolWidth = this->width / this->reels;
	this->symbolHeight = this->height / this->rows;
	Random::init();
}

void ReelMachine::draw(HDC hDC, const std::vector<HBITMAP>& symbols, const Spin* spin)
{
	RECT rect;
	rect.left = this->left, rect.top = this->top;
	rect.right = this->left+this->width, rect.bottom = this->top+this->height;
	HBRUSH hBrBackground = CreateSolidBrush(RGB(88, 88, 128));
	FillRect(hDC, &rect, hBrBackground);
	DeleteObject(hBrBackground);

	// Draw symbols
	// TODO throw exception
	if (spin != nullptr)
	{
		const Window& w = spin->getWindow();
		for (int i = 0; i < this->reels; i++)
			for (int j = 0; j < this->rows; j++)
			{
				if ((i < w.getWidth()) && (j <= w.getHeight()))
				{
					DrawState(hDC, NULL, NULL, LPARAM(symbols[w.getSymbol(i,j).getId()]), 0
						, 1 + this->left + i * this->symbolWidth, 1 + this->top + j * this->symbolHeight
						, 0, 0, DST_BITMAP);
				}
			}
	}

	// Draw the grid
	/*for (int j = 0; j <= this->rows; j++)
	{
		MoveToEx(hDC, this->left, this->top + j * this->symbolHeight, NULL);
		LineTo(hDC, this->left + this->width, this->top + j * this->symbolHeight);
	}
	for (int i = 0; i <= this->reels; i++)
	{
		MoveToEx(hDC, this->left + i * this->symbolWidth, this->top, NULL);
		LineTo(hDC, this->left + i * this->symbolWidth, this->top + this->height);
	}*/
}

