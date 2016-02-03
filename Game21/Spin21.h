#pragma once

#include "SimulantCore/Spin.h"

class Spin21 : public Spin
{
public:
	Spin21(Window* window, bool final = true);
	Spin21(Window* window, const std::vector<const Symbol*>& bonusStack, bool final = true, int bonusStackWinPosition = -1);
	Spin21(const Spin& spin, const std::vector<const Symbol*>& bonusStack, bool final = true);
	void stackPop();
	void stackPush(const Symbol& symbol);
	const std::vector<const Symbol*>& getStack() const;
	int getBonusPosition() const;
protected:
	std::vector<const Symbol*> bonusStack;
	int bonusStackWinPosition;
};