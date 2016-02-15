#pragma once
#include <string>
#include <vector>
#include "JSON/JSON.h"

typedef std::vector<int> Payline;
typedef std::vector<Payline> PayLineSet;

// One kind of symbols (for exaple CHERRIES)
class Symbol
{
public:
	enum WildType
	{
		Normal,
		EachX2
	};
	Symbol(int id, const std::wstring& name, const std::vector<int>& pay);
	Symbol(const JSONValue* source);
	const std::wstring& getName() const;
	int getId() const;
	int getWin(int symbolsInLine) const;
	bool isWild() const;
	bool isScatter() const;
	bool operator==(const Symbol& symbol) const;
	WildType getWildType() const;

protected:
	std::wstring name;
	int id;
	std::vector<int> pay;
	bool wild;
	bool scatter;
	WildType wildType;
};