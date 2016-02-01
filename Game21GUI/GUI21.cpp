#include "GUI21.h"

GUI21::GUI21(HINSTANCE hInstance)
	: GUI(hInstance)
{
	this->bonusStack = new BonusStack(368, 268+50+100, 357);
	this->bonusStack->draw(this->mainWindow->getDC(), this->symbols);
	this->mainWindow->refresh();
}

GUI21::~GUI21()
{
	delete this->bonusStack;
}

void GUI21::setNewSpin(const Spin & spin)
{
	this->bonusStack->draw(this->mainWindow->getDC(), this->symbols, dynamic_cast<const Spin21*>(&spin));
	GUI::setNewSpin(spin);
}

BonusStack::BonusStack(int left, int top, int width)
{
	this->rm = new ReelMachine(left, top, width, 4, 1);
}

BonusStack::~BonusStack()
{
	delete this->rm;
}

void BonusStack::draw(HDC hDC, const std::vector<HBITMAP>& symbols, const Spin21 * spin)
{
	if (spin == nullptr)
	{
		this->rm->draw(hDC, symbols, nullptr);
		return;
	}
	Window* w = new Window(4, 1);
	const std::vector<const Symbol*>& stack = spin->getStack();
	for (int i = 0; i < stack.size(); i++)
	{
		w->setSymbol(i, 0, *stack[i]);
	}
	for (int j = stack.size(); j < 4; j++)
	{
		w->setSymbol(j, 0, spin->getSSet()->getSymbol(7));
	}
	Spin* newSpin = new Spin(w, spin->isFinal());
	this->rm->draw(hDC, symbols, newSpin);
	delete newSpin;
}
