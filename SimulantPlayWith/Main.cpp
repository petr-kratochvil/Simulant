#include <stdio.h>
#include "SimulantCore/SpinSourceGenerator.h"
#include "SimulantCore/SpinSource.h"
#include "SimulantCore/Simulator.h"
#include "SimulantCore/SymbolSet.h"
#include "SimulantCore/ReelSet.h"
#include <io.h>
#include <fcntl.h>
#include "SimulantCore/JSON/JSON.h"
#include "SimulantCore/Random.h"

// dummy spin and default symbol set
void testSpinSourceGenerator()
{
	SpinSourceGenerator sg;
	{
		Simulator simul(sg);
		simul.spinOneBet();
		{
			const Spin& spin = simul.getLastSpin();
			wprintf(L"The win amount is: %d.\n", spin.getWin());
			Symbol symbol = spin.getWindow().getSymbol(1, 2);
			wprintf(L"The symbol at (2,3) is: id=%d, name=%s\n", symbol.getId(), symbol.getName().c_str());
		}
	}
	Spin* spin2 = sg.getDummySpin(50, 1);
	Symbol symbol2 = spin2->getWindow().getSymbol(43, 0);
	wprintf(L"Dummy spin2 at (42,1) is: id=%d, name=%s\n", symbol2.getId(), symbol2.getName().c_str());
}

void testSymbolSet(SymbolSet& sset)
{
	int newId = sset.addSymbol(L"Ředkvička", { 1,2,3,0,0 });
	wprintf(L"New id for symbol: %d\n", newId);
	wprintf(L"Newly added symbol is: id=%d, name=%s\n", sset.getSymbol(newId).getId(), sset.getSymbol(newId).getName().c_str());

	wprintf(L"Symbol count: %d, pay for line of 4 %s: %d\n"
		, sset.getSymbolCount()
		, sset.getSymbol(0).getName().c_str()
		, sset.getSymbol(0).getWin(4));
}

void testReelSet(ReelSet& rs)
{
	rs.spin();
	Window* w = rs.getWindow();
	for (int x = 0; x < w->getWidth(); x++)
	{
		for (int y = 0; y < w->getHeight(); y++)
			wprintf(L"%s\t", w->getSymbol(x, y).getName().c_str());
		wprintf(L"\n");
	}
	delete w;
}

void getJSONInput(wchar_t* json)
{
	FILE *fr;
	errno_t err = fopen_s(&fr, "examples/example1.json", "rt,ccs=UTF-8");

	wchar_t buffer[1000];
	json[0] = '\0';
	int jsonLength = 0;
	while (!feof(fr))
	{
		fgetws(buffer, 1000, fr);
		wcsncat_s(json, 999, buffer, 1000 - jsonLength);
		jsonLength += wcslen(buffer);
	}
}

void testRandom()
{
	for (int i = 0; i < 10; i++)
	{
		Random::init();
		wprintf(L"Random numbers: %d, %d, %d, %d\n", Random::gen(100, 1000), Random::gen(100, 1000), Random::gen(100, 1000), Random::gen(100, 1000));
	}
}

int main()
{
	Random::init();

	_setmode(_fileno(stdout), _O_U8TEXT);
	wprintf(L"SimulantPlayWith\n");

	testSpinSourceGenerator();
	
	wchar_t json[1000];
	getJSONInput(json);
	wprintf(L"json file:\n%s\n", json);
	const JSONValue* parsedJSON = JSON::Parse(json);

	SymbolSet sset(parsedJSON);
	testSymbolSet(sset);

	JSONObject parsedJSONObject = parsedJSON->AsObject();
	JSONArray parsedJSONReelSets = parsedJSONObject[L"reelSets"]->AsArray();
	
	wprintf(L"Test ReelSetu:\n");
	ReelSet rs(parsedJSONReelSets[0], sset);
	testReelSet(rs);

	testRandom();

	delete parsedJSON;
	Random::release();
	return 0;
}