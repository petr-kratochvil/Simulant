#include <stdio.h>
#include "SimulantCore/SpinSourceGenerator.h"
#include "SimulantCore/SpinSource.h"
#include "SimulantCore/Simulator.h"
#include "SimulantCore/SymbolSet.h"
#include <io.h>
#include <fcntl.h>
#include "SimulantCore/JSON/JSON.h"

int main()
{
	_setmode(_fileno(stdout), _O_U8TEXT);
	wprintf(L"SimulantPlayWith\n");

	SpinSourceGenerator sg;
	{
		Simulator simul(sg);
		simul.spinOneBet();
		{
			const Spin& spin = simul.getLastSpin();
			wprintf(L"The win amount is: %d.\n", spin.getWin());
			Symbol symbol = spin.getWindow().getSymbol(2, 3);
			wprintf(L"The symbol at (2,3) is: id=%d, name=%s\n", symbol.getId(), symbol.getName().c_str());
		}
	}


	FILE *fr;
	errno_t err = fopen_s(&fr, "examples/example1.json", "rt,ccs=UTF-8");
	wchar_t json[1000];
	wchar_t buffer[1000];
	json[0] = '\0';
	int jsonLength = 0;
	while (!feof(fr))
	{
		fgetws(buffer, 1000, fr);
		wcsncat_s(json, buffer, 1000 - jsonLength);
		jsonLength += wcslen(buffer);
	}
	wprintf(L"json file:\n%s\n", json);
	const JSONValue* symbolSetJson = JSON::Parse(json);

	SymbolSet* sset = new SymbolSet(symbolSetJson);
	int newId = sset->addSymbol(L"Ředkvička", { 1,2,3,0,0 });
	wprintf(L"New id for symbol: %d\n", newId);
	wprintf(L"Newly added symbol is: id=%d, name=%s\n", sset->getSymbol(newId).getId(), sset->getSymbol(newId).getName().c_str());
	Spin* spin2 = sg.getDummySpin(50, 1);
	Symbol symbol2 = spin2->getWindow().getSymbol(43, 1);
	wprintf(L"Dummy spin2 at (42,1) is: id=%d, name=%s\n", symbol2.getId(), symbol2.getName().c_str());

	wprintf(L"Symbol count: %d, pay for line of 4 %s: %d\n"
		, sset->getSymbolCount()
		, sset->getSymbol(0).getName().c_str()
		, sset->getSymbol(0).getWin(4));

	delete sset;
	delete symbolSetJson;
	return 0;
}