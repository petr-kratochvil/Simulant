#include "Game21GUI/GUI21.h"
#include "SimulantGUI/Controller.h"
#include "resource.h"
#include "SimulantCore/JSON/JSON.h"
#include "SimulantCore/Random.h"

void getJSONInput(wchar_t* json)
{
	FILE *fr;
	errno_t err = fopen_s(&fr, "game21.json", "rt,ccs=UTF-8");

	wchar_t buffer[10000];
	json[0] = '\0';
	int jsonLength = 0;
	while (!feof(fr))
	{
		fgetws(buffer, 10000, fr);
		wcsncat_s(json, 9999, buffer, 10000 - jsonLength);
		jsonLength += wcslen(buffer);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Random::init();
	
	wchar_t json[10000];
	getJSONInput(json);
	
	Controller controller(json);
	GUI21 gui(hInstance);
	controller.assignGUI(&gui);
	gui.assignController(&controller);
	
	gui.addSymbol(IDB_BITMAP0);
	gui.addSymbol(IDB_BITMAP1);
	gui.addSymbol(IDB_BITMAP2);
	gui.addSymbol(IDB_BITMAP3);
	gui.addSymbol(IDB_BITMAP4);
	gui.addSymbol(IDB_BITMAP5);
	gui.addSymbol(IDB_BITMAP6);
	gui.addSymbol(IDB_BITMAP7);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}