#pragma once
#include <vector>
#include "Symbol.h"

struct WindowWinItem
{
	const Symbol& symbol;
	int count;
	int pay;
};

class WindowWin
{
public:
	WindowWin();
	int getTotal() const;
	int getLinesCount() const;
	const std::vector<WindowWinItem>& getList() const;
	void clear();
	void addLine(const Symbol& symbol, int count, int pay);
protected:
	std::vector<WindowWinItem> winList;
	int linesCount;
	int totalWin;
};