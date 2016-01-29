#include "MainWindow.h"
#include "GUI.h"
#include "resource.h"

MainWindow::MainWindow(GUI* gui, HINSTANCE hInstance, const wchar_t* title, int width, int height)
	: width(width)
	, height(height)
	, hDC(nullptr)
	, gui(gui)
{
	wcsncpy_s(this->title, title, 250);
	this->registerWindowClass(hInstance);
	this->createWindow(hInstance);
}

MainWindow::~MainWindow()
{
	if (this->hDC != nullptr)
		DeleteDC(this->hDC);
}

void MainWindow::show(int showFlag)
{
	ShowWindow(this->hWnd, showFlag);
	UpdateWindow(this->hWnd);
}

void MainWindow::refresh()
{
	InvalidateRect(this->hWnd, NULL, false);
	UpdateWindow(this->hWnd);
}

HDC MainWindow::getDC()
{
	return this->hDC;
}

void MainWindow::registerWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWindow::internal_windowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MainWindowClass";
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	// TODO throw exception
	RegisterClassEx(&wcex);
}

void MainWindow::createWindow(HINSTANCE hInstance)
{
	// TODO throw exception
	this->hWnd = CreateWindow(L"MainWindowClass", this->title, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, this->width, this->height, NULL, NULL, hInstance, NULL);
	SetWindowLong(this->hWnd, GWLP_USERDATA, long(this));
}

LRESULT MainWindow::internal_windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MainWindow* mw = (MainWindow*)GetWindowLong(hWnd, GWLP_USERDATA);
	if (mw == nullptr)
		return DefWindowProc(hWnd, message, wParam, lParam);
	else
		return mw->windowProc(hWnd, message, wParam, lParam);
}

LRESULT MainWindow::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SHOWWINDOW:
		{
			if (this->hDC != nullptr)
			{
				DeleteDC(this->hDC);
				DeleteObject(this->hBMP);
			}

			// Create hDC and bitmap
			HDC hDCWindow = GetDC(this->hWnd);
			this->hDC = CreateCompatibleDC(hDCWindow);
			this->hBMP = CreateCompatibleBitmap(hDCWindow, GetDeviceCaps(this->hDC, HORZRES), GetDeviceCaps(this->hDC, VERTRES));
			SelectObject(hDC, this->hBMP);
			ReleaseDC(this->hWnd, hDCWindow);

			// fill background
			RECT rect; rect.left = 0, rect.top = 0;
			rect.right = this->width, rect.bottom = this->height;
			HBRUSH hBrBackground = CreateSolidBrush(RGB(255, 255, 255));
			FillRect(this->hDC, &rect, hBrBackground);
			DeleteObject(hBrBackground);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDCPaint = BeginPaint(hWnd, &ps);
			bool a = BitBlt(hDCPaint, 0, 0, this->width, this->height, this->hDC, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_KEYDOWN:
		this->gui->userStartClicked();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
