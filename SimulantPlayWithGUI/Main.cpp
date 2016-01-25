#include "SimulantGUI/MainWindow.h"
#include "resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MainWindow mw(hInstance, L"Okénko ìšèøžýáí", 800, 600);
	mw.show(nCmdShow);
	
	mw.addSymbol(IDB_BITMAP0);
	mw.addSymbol(IDB_BITMAP1);
	mw.addSymbol(IDB_BITMAP2);
	mw.addSymbol(IDB_BITMAP3);
	mw.addSymbol(IDB_BITMAP4);
	mw.addSymbol(IDB_BITMAP5);
	mw.addSymbol(IDB_BITMAP6);
	mw.addSymbol(IDB_BITMAP7);

	mw.setNewSpin(Spin(NULL));

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}