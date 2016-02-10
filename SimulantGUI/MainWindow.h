#pragma once
#include <windows.h>
#include <string>

class GUI;

class MainWindow
{
public:
	MainWindow(GUI* gui, HINSTANCE hInstance, const wchar_t* title, int width, int height);
	~MainWindow();
	void show(int showFlag = SW_SHOW);
	void refresh();
	HDC getDC();
	void setDescription(const std::wstring description);
	void setButtonBackEnabled(bool enabled);

private:
	MainWindow(const MainWindow&);
	MainWindow& operator=(const MainWindow&);
	void registerWindowClass(HINSTANCE hInstance);
	void createWindow(HINSTANCE hInstance);
	static LRESULT CALLBACK internal_windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	HWND hWnd;
	HWND editInfo;
	HWND buttonStart;
	HWND buttonBack;
	HWND buttonBetUp;
	HWND buttonBetDown;
	int width, height;
	wchar_t title[250];
	HDC hDC; // DC to be painted to - gets copied to the window on WM_PAINT
	HBITMAP hBMP; // bitmap associated with hDC
	HFONT hFontHeading;
	GUI* gui;
	static WNDPROC oldButtonProc;
	static LRESULT CALLBACK newButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};