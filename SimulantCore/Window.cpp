#include "Window.h"

Window::Window(int width, int height)
	: width(width)
	, height(height)
	, matrix(width, std::vector<const Symbol*>(height))
{
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
