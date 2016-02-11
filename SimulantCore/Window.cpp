#include "Window.h"

Window::Window(int width, int height)
	: width(width)
	, height(height)
	, matrix(width, std::vector<const Symbol*>(height))
	, highlights(width, std::vector<bool>(height, false))
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			this->highlights[i][j] = false;
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

const WindowWin& Window::winCrissCross3x3(int bet)
{
	this->wwin.clear();
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
