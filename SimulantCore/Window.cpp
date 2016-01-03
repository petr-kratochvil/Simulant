#include "Window.h"

Window::Window(int maxWidth, int maxHeight)
	: maxWidth(maxWidth)
	, maxHeight(maxHeight)
	, matrix(maxWidth, std::vector<const Symbol*>(maxHeight))
{
}

int Window::getWidth() const
{
	return this->maxWidth;
}

int Window::getHeight() const
{
	return this->maxHeight;
}

const Symbol& Window::getSymbol(int x, int y) const
{
	// TODO throw exception
	return *this->matrix[x-1][y-1];
}

void Window::setSymbol(int x, int y, const Symbol& symbol)
{
	this->matrix[x-1][y-1] = &symbol;
}
