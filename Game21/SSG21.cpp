#include "SSG21.h"
#include "SimulantCore/Random.h"
#include <algorithm>

SSG21::SSG21(const SymbolSet * symbolSet, JSONArray reelSets)
	: SpinSourceGenerator(symbolSet)
	, state(SSG21::Basic)
	, reelsetIdBonus(0)
	, zeroReelSetId(-1)
	, currentBetId(0)
	, betValues({ 5, 10, 15, 25, 50, 100, 200, 300, 500, 1000 })
{
	this->reelSetCount = reelSets.size();
	this->reelSets.resize(this->reelSetCount);
	this->reelSetPmls.resize(this->reelSetCount);

	int totalPml = 0;
	for (int i = 0; i < this->reelSetCount; i++)
	{
		JSONObject rs = reelSets[i]->AsObject();
		if (rs.find(L"substitutePmls21") != rs.end())
		{
			this->reelSets[i] = new ZeroReelSet21(reelSets[i], *symbolSet);
			this->zeroReelSetId = i;
		}
		else if (rs.find(L"createdOf") != rs.end())
		{
			JSONArray c = rs[L"createdOf"]->AsArray();
			this->bonusCreationPmls.resize(c.size());
			for (int j = 0; j < c.size(); j++)
				this->bonusCreationPmls[j] = (int)c[j]->AsNumber();
		}
		else
			this->reelSets[i] = new ReelSet(reelSets[i], *symbolSet);
		totalPml += int(rs[L"pml"]->AsNumber());
		this->reelSetPmls[i] = totalPml;
	}
	// TODO throw exception if totalPml > 1000
}

Spin21 * SSG21::getNextSpin()
{
	Spin21* spin21;

	if (this->state == SSG21::Bonus)
	{
		int bonusPos = Random::gen(0, 3);
		int symbolID = this->bonusStack[bonusPos]->getId();
		int pml = Random::gen(0, 999);
		int totalPml = 0;
		int reelSetId = -1;
		do
		{
			totalPml += this->bonusCreationPmls[++reelSetId];
		} while (totalPml <= pml);
		this->reelsetIdBonus = reelSetId;
		Window* w = nullptr;
		int spinWin;
		do {
			this->reelSets[this->reelsetIdBonus]->spinAndFind21(symbolID);
			if (w != nullptr)
				delete w;
			 w = this->reelSets[this->reelsetIdBonus]->getWindow();
			 spinWin = w->winCrissCross3x3_game21().getTotal();
		} while ((spinWin > 400) && (spinWin != 540) && (spinWin != 600) && (spinWin != 800) && (spinWin != 1080));
		spin21 = new Spin21(w, this->bonusStackVisible, true, bonusPos);
		spin21->setSSet(this->symbolSet);
		spin21->setReelset(this->reelSets[this->reelsetIdBonus]->getName());
		this->state = SSG21::Basic;
		return spin21;
	}

	if (this->bonusStack.size() >= 4)
	{
		//for (int i = 0; i < 4; i++)
		//	this->bonusStack.erase(this->bonusStack.begin());
		this->bonusStack.clear();
	}

	if (this->zeroReelSetId >= 0)
		dynamic_cast<ZeroReelSet21*>(this->reelSets[this->zeroReelSetId])->setBonusStackSize(this->bonusStack.size());

	Spin* spin = nullptr;
	int spinWin;
	do
	{
		if (spin != nullptr)
			delete spin;
		spin = SpinSourceGenerator::getNextSpin();
		spinWin = spin->getTotalWin();
	} while ((spinWin > 400) && (spinWin != 540) && (spinWin != 600) && (spinWin != 800) && (spinWin != 1080) && (spinWin != 480) && (spinWin != 720));
	std::vector<WindowWinItem> winList = spin->getWin().getList();

	std::wstring bonusChar;
	switch (this->bonusStack.size())
	{
	case 0:
		bonusChar = L"0 bonusových";
		break;
	case 1:
		bonusChar = L"1 bonusové";
		break;
	case 2:
		bonusChar = L"2 bonusové";
		break;
	case 3:
		bonusChar = L"3 bonusové";
		break;
	}
	spin->addCharacteristic(bonusChar);
	if (spin->getTotalWin() == 0)
	{
		const Window& w = spin->getWindow();
		for (int i = 0; i < w.getHeight(); i++)
		{
			if (w.getSymbol(1, i).getId() == 7)
			{
				if (this->bonusStack.size() > 0)
					this->bonusStack.pop_back();
				spin->addCharacteristic(L"symbolX");
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < winList.size(); i++)
		{
			bool found = false;
			for (int j = 0; j < this->bonusStack.size(); j++)
			{
				if (this->bonusStack[j] == &winList[i].symbol)
				{
					found = true;
					break; 
				}
			}
			if (!found)
				this->bonusStack.push_back(&winList[i].symbol);
		}
	}

	this->bonusStackVisible.clear();
	for (int i = 0; i < std::min(int(this->bonusStack.size()), 4); i++)
	{
		this->bonusStackVisible.push_back(this->bonusStack[i]);
	}
	bool final = !(this->bonusStackVisible.size() >= 4);

	if (!final)
		this->state = SSG21::Bonus;

	spin21 = new Spin21(*spin, this->bonusStackVisible, final);
	delete spin;

	return spin21;
}

void SSG21::betUp()
{
	this->currentBetId = (this->currentBetId + 1) % this->betValues.size();
	SpinSourceGenerator::setBet(this->betValues[this->currentBetId]);
	this->bonusStack.clear();
}

void SSG21::betDown()
{
	this->currentBetId = (this->currentBetId + this->betValues.size() - 1) % this->betValues.size();
	SpinSourceGenerator::setBet(this->betValues[this->currentBetId]);
}
