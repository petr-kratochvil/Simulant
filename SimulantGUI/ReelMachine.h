#pragma once
#include <windows.h>
#include <GdiPlus.h>
#include <vector>
#include "SimulantCore/Spin.h"

class ReelMachine
{
public:
	ReelMachine(int left, int top, int width, int reels, int rows);
	~ReelMachine();
	void draw(HDC hDC, const std::vector<HBITMAP>& symbols, const Spin* spin = nullptr);
protected:
	void drawSymbolHighlights(Gdiplus::Graphics& graphics, const Window& w);
	int left;
	int top;
	int width;
	int height;
	int symbolWidth;
	int symbolHeight;
	int reels;
	int rows;

	// for highlight drawing
	int highlightMargin;
	Gdiplus::Pen* penHighlightEven;
	Gdiplus::Pen* penHighlightOdd;
	Gdiplus::LinearGradientBrush* highlightBrushEven;
	Gdiplus::LinearGradientBrush* highlightBrushOdd;
	Gdiplus::Color highlightColor1;
	Gdiplus::Color highlightColor2;
	Gdiplus::Graphics* graphics;
};