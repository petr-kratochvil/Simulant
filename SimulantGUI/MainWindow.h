#pragma once
#include <windows.h>
#include "resource.h"

class MainWindow
{
public:
	MainWindow(HINSTANCE hInstance, const wchar_t* title, int width, int height);
	void show(int showFlag = SW_SHOW);
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
};