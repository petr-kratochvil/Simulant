#include "Game17GUI/GUI17.h"
#include "SimulantGUI/Controller.h"
#include "resource.h"
#include "SimulantCore/JSON/JSON.h"
#include "SimulantCore/Random.h"

void getJSONInput(HINSTANCE hInstance, wchar_t* json)
{
	// TODO throw exception
	HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(IDT_GAME_JSON), L"TEXT");
	HGLOBAL hResource = LoadResource(hInstance, hResInfo);
	char* buff = (char*)LockResource(hResource);
	int resSize = SizeofResource(hInstance, hResInfo);
	char tmpFileName[L_tmpnam];
	tmpnam(tmpFileName);
	FILE* tmp = fopen(tmpFileName, "wb");
	fwrite(buff, sizeof(char), resSize / sizeof(char), tmp);
	fclose(tmp);
	FreeResource(hResource);
	FILE *fr = fopen(tmpFileName, "rt,ccs=UTF-8");

	wchar_t buffer[20000];
	json[0] = '\0';
	int jsonLength = 0;
	while (!feof(fr))
	{
		fgetws(buffer, 20000, fr);
		wcsncat_s(json, 19999, buffer, 20000 - jsonLength);
		jsonLength += wcslen(buffer);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Random::init();

	wchar_t json[20000];
	getJSONInput(hInstance, json);

	Controller controller(json);
	GUI17 gui(hInstance);
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
	gui.addSymbol(IDB_BITMAP8);
	gui.addSymbol(IDB_BITMAP9);
	gui.addSymbol(IDB_BITMAP10);
	gui.addSymbol(IDB_BITMAP11);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}