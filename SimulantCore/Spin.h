#pragma once
#include "Window.h"
#include "SymbolSet.h"

// Represents one spin, or respin, or freespin
class Spin
{
public:
	Spin(Window* window, bool final = true);
	virtual ~Spin();

	// Total win amount, including payline wins and bonuses,
	int getTotalWin() const;
	const Window& getWindow() const;
	// Is next spin basic (not a respin)?
	bool isFinal() const;
	const WindowWin& getWin() const;
	void setSSet(const SymbolSet* sSet);
	const SymbolSet* getSSet() const;
	void setBet(int bet);
	int getBet() const;
	const std::wstring& getReelset() const;
	void setReelset(const std::wstring& reelset);
	const std::vector<std::wstring>& getCharacteristics() const;
	void addCharacteristic(std::wstring name);

protected:
	bool final;
	Window* window;
	int bet;
	std::wstring reelsetUsed;
	virtual void computeWin();
	const WindowWin* wwin;
	Spin(const Spin& spin);
	const SymbolSet* sSet;
	std::vector<std::wstring> characteristics;
private:
	Spin& operator=(const Spin&);
};