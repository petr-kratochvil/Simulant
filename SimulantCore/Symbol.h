#pragma once
#include <string>
#include <vector>

// One kind of symbols (for exaple CHERRIES)
class Symbol
{
public:
	Symbol(int id, const std::wstring& name, const std::vector<int>& pay);
	const std::wstring& getName() const;
	int getId() const;
	int getWin(int symbolsInLine) const;

protected:
	std::wstring name;
	int id;
	std::vector<int> pay;
};