#pragma once
#include "SimulantCore/Spin.h"
#include "ReelMachine.h"
#include "MainWindow.h"
#include <gdiplus.h>

// link the Gdiplus library
#pragma comment(lib,"gdiplus.lib")

class Controller;

class GUI
{
public:
	GUI(HINSTANCE hInstance);
	virtual ~GUI();
	virtual void setNewSpin(const Spin & spin);
	virtual void setNewDescription(const std::wstring& description);
	void assignController(Controller* controller);
	void addSymbol(int resourceID);
	void userStartClicked();
	void userBackClicked();
	void setGoBackEnabled(bool enabled);

protected:
	Controller* controller;
	ReelMachine* reelMachine;
	std::vector<HBITMAP> symbols;
	HINSTANCE hInstance;
	MainWindow* mainWindow;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

private:
	GUI(const GUI&);
	GUI& operator=(const GUI&);
};