#pragma once
#include "SimulantCore/Spin.h"
#include "ReelMachine.h"
#include "MainWindow.h"

class Controller;

class GUI
{
public:
	GUI(HINSTANCE hInstance);
	~GUI();
	void setNewSpin(const Spin & spin);
	void assignController(Controller* controller);
	void addSymbol(int resourceID);
	void userStartClicked();

protected:
	Controller* controller;
	ReelMachine* reelMachine;
	std::vector<HBITMAP> symbols;
	HINSTANCE hInstance;
	MainWindow* mainWindow;

private:
	GUI(const GUI&);
	GUI& operator=(const GUI&);
};