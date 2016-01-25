#pragma once
#include <windows.h>
#include "ReelMachine.h"
#include "SimulantCore/Spin.h"

class MainWindow
{
public:
	MainWindow(HINSTANCE hInstance, const wchar_t* title, int width, int height);
	~MainWindow();
	void show(int showFlag = SW_SHOW);
	void setNewSpin(const Spin& spin);
	void addSymbol(int resourceID);
private:
	MainWindow(const MainWindow&);
	MainWindow& operator=(const MainWindow&);
	void registerWindowClass();
	void createWindow();
	static LRESULT CALLBACK internal_windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	HINSTANCE hInstance;
	HWND hWnd;
	int width, height;
	wchar_t title[250];
	HDC hDC; // DC to be painted to - gets copied to the window on WM_PAINT
	HBITMAP hBMP; // bitmap associated with hDC
	ReelMachine* reelMachine;
	std::vector<HBITMAP> symbols;
};