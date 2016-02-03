#include "ReelMachine.h"
#include "SimulantCore/Random.h"

ReelMachine::ReelMachine(int left, int top, int width, int reels, int rows)
	: left(left)
	, top(top)
	, width(width)
	, reels(reels)
	, rows(rows)
	, highlightMargin(3)
{
	this->height = ceil(double(this->width) / double(this->reels) * double(this->rows));
	this->symbolWidth = this->width / this->reels;
	this->symbolHeight = this->height / this->rows;

	this->highlightColor1 = Gdiplus::Color(80, 80, 0);
	this->highlightColor2 = Gdiplus::Color(255, 255, 25);

	this->highlightBrushEven = new Gdiplus::LinearGradientBrush(Gdiplus::Point(0, this->symbolHeight), Gdiplus::Point(this->symbolWidth, 0)
		, this->highlightColor1, this->highlightColor2);
	this->highlightBrushOdd = new Gdiplus::LinearGradientBrush(Gdiplus::Point(0, 2 * (this->symbolHeight)), Gdiplus::Point(this->symbolWidth, this->symbolHeight)
		, this->highlightColor1, this->highlightColor2);

	this->highlightBrushEven->TranslateTransform(this->left+15, this->top+11);
	this->highlightBrushOdd->TranslateTransform(this->left+15, this->top+11);

	this->penHighlightEven = new Gdiplus::Pen(this->highlightBrushEven, 4.0);
	this->penHighlightOdd = new Gdiplus::Pen(this->highlightBrushOdd, 4.0);

	this->penHighlightEven->SetLineJoin(Gdiplus::LineJoin::LineJoinRound);
	this->penHighlightOdd->SetLineJoin(Gdiplus::LineJoin::LineJoinRound);
}

ReelMachine::~ReelMachine()
{
	delete this->penHighlightEven;
	delete this->penHighlightOdd;
	delete this->highlightBrushEven;
	delete this->highlightBrushOdd;
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
		this->graphics = new Gdiplus::Graphics(hDC);
		graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		this->drawSymbolHighlights(*this->graphics, w);
		delete this->graphics;
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

void ReelMachine::drawSymbolHighlights(Gdiplus::Graphics & graphics, const Window& w)
{
	int margin = this->highlightMargin;
	for (int i = 0; i < this->reels; i++)
		for (int j = 0; j < this->rows; j++)
		{
			if (w.highlighted(i, j))
			{
				graphics.DrawRectangle((i + j) % 2 ? this->penHighlightOdd : this->penHighlightEven
					, this->left + i * this->symbolWidth + margin
					, this->top + j * this->symbolHeight + margin
					, this->symbolWidth - 2 * margin
					, this->symbolHeight - 2 * margin);
			}
		}
}

