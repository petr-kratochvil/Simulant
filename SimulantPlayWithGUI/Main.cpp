#include "SimulantGUI/MainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MainWindow mw(hInstance, L"Okénko ìšèøžýáí", 800, 600);
	mw.show(nCmdShow);

	mw.setNewSpin(Spin(NULL));

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}