#include "Window.h"

Window::Window(int width, int height)
	: width(width)
	, height(height)
	, matrix(width, std::vector<const Symbol*>(height))
	, highlights(width, std::vector<bool>(height, false))
{
	this->highlightOff();
}

int Window::getWidth() const
{
	return this->width;
}

int Window::getHeight() const
{
	return this->height;
}

const Symbol& Window::getSymbol(int x, int y) const
{
	// TODO throw exception
	return *this->matrix[x][y];
}

void Window::setSymbol(int x, int y, const Symbol& symbol)
{
	this->matrix[x][y] = &symbol;
}

const WindowWin& Window::winCrissCross3x3_game21(int bet)
{
	this->wwin.clear();
	this->highlightOff();
	bool isWildReplacing = true;
	int wildCount = 0;
	int winAmount = 0;
	
	for (int i = 0; i < this->width; i++)
		for (int j = 0; j < this->height; j++)
			if (this->matrix[i][j]->getId() == 6)
				wildCount++;
	if (wildCount >= 3)
		isWildReplacing = false;

	for (int i = 0; i < this->height; i++)
		for (int j = 0; j < this->height; j++)
			for (int k = 0; k < this->height; k++)
			{
				const Symbol* main = this->matrix[0][i];
				if (isWildReplacing)
				{
					if (main->getId() == 6) // wild
						main = this->matrix[1][j];
					if (main->getId() == 6) // wild
						main = this->matrix[2][k];
				}
				int sameSymbolsCount = 1;
				if (isWildReplacing)
				{
					if ((main == this->matrix[1][j])
						|| (this->matrix[1][j]->getId() == 6))
					{
						sameSymbolsCount = 2;
						if ((this->matrix[2][k] == main)
							|| (this->matrix[2][k]->getId() == 6))
							sameSymbolsCount = 3;
					}
				}
				else
				{
					if (main == this->matrix[1][j])
					{
						sameSymbolsCount = 2;
						if (this->matrix[2][k] == main)
							sameSymbolsCount = 3;
					}
				}
				int pay = main->getWin(sameSymbolsCount) * bet / 5;
				if (pay > 0)
				{
					this->wwin.addLine(*main, sameSymbolsCount, pay);
					this->highlights[0][i] = true;
					this->highlights[1][j] = true;
					this->highlights[2][k] = true;
				}
			}
	return this->wwin;
}

bool Window::highlighted(int x, int y) const
{
	return this->highlights[x][y];
}

void Window::setHighlight(int x, int y, bool status)
{
	this->highlights[x][y] = status;
}

int Window::getHighlightedCount() const
{
	int count = 0;
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if (this->highlights[i][j])
				count++;
	return count;
}

WindowWin& Window::winPayLineSet(const PayLineSet& paylines, int bet)
{
	this->wwin.clear();
	this->highlightOff();
	for (int i = 0; i < paylines.size(); i++)
		this->winPayLine(paylines[i], bet);
	return this->wwin;
}

int Window::winPayLine(const Payline & line, int bet)
{
	const Symbol* main = this->matrix[0][line[0]];
	int symbolCount = 0;
	int wildPayMultiply = 1;
	for (int j = 0; j < this->getWidth(); j++)
	{
		const Symbol* current = this->matrix[j][line[j]];
		if (main->isWild())
			main = current;
		if (current->isWild())
		{
			if (current->getWildType() == Symbol::EachX2)
				wildPayMultiply *= 2;
		}
		if (*current == *main)
			symbolCount = j + 1;
		else
			break;
	}
	int pay = wildPayMultiply * main->getWin(symbolCount) * bet / 5;
	if (pay > 0)
	{
		this->wwin.addLine(*main, symbolCount, pay);
		for (int j = 0; j < symbolCount; j++)
			this->highlights[j][line[j]] = true;
	}
	return pay;
}

int Window::winScatter(const Symbol& symbol, int bet)
{
	int count = 0;
	for (int i = 0; i < this->width; i++)
		for (int j = 0; j < this->height; j++)
		{
			if (this->matrix[i][j]->getId() == symbol.getId())
				count++;
		}
	int pay = symbol.getWin(count) * bet / 5;
	if (pay > 0)
	{
		this->wwin.addLine(symbol, count, pay);
	}
	return count;
}

void Window::highlightScatter(const Symbol& symbol)
{
	for (int i = 0; i < this->width; i++)
	for (int j = 0; j < this->height; j++)
			if (this->matrix[i][j]->getId() == symbol.getId())
				this->highlights[i][j] = true;
}

void Window::highlightOff()
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			this->highlights[i][j] = false;
}
