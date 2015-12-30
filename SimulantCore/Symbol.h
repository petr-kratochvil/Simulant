#pragma once
#include <string>
#include <vector>

// One kind of symbols (for exaple CHERRIES)
class Symbol
{
public:
	Symbol(int id, std::wstring name, std::vector<int> pay);
	const std::wstring& getName() const;
	int getID() const;
	int getWin(int symbolsInLine) const;
};