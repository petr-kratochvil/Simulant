#include "MainWindow.h"
#include "resource.h"

MainWindow::MainWindow(HINSTANCE hInstance, const wchar_t* title, int width, int height)
	: hInstance(hInstance)
	, width(width)
	, height(height)
	, hDC(nullptr)
{
	wcsncpy_s(this->title, title, 250);
	this->registerWindowClass();
	this->createWindow();
	int rmW = 446;
	this->reelMachine = new ReelMachine((width-rmW) / 2, 50, rmW, 5, 3);
}

MainWindow::~MainWindow()
{
	if (this->hDC != nullptr)
		DeleteDC(this->hDC);
	delete this->reelMachine;
	for (int i = 0; i < this->symbols.size(); i++)
		DeleteObject(this->symbols[i]);
}

void MainWindow::show(int showFlag)
{
	ShowWindow(this->hWnd, showFlag);
	UpdateWindow(this->hWnd);
}

void MainWindow::setNewSpin(const Spin & spin)
{
	this->reelMachine->draw(this->hDC, this->symbols);
	InvalidateRect(this->hWnd, NULL, false);
	UpdateWindow(this->hWnd);
}

void MainWindow::addSymbol(int resourceID)
{
	this->symbols.push_back(LoadBitmap(this->hInstance, MAKEINTRESOURCE(resourceID)));
}

void MainWindow::registerWindowClass()
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

void MainWindow::createWindow()
{
	// TODO throw exception
	this->hWnd = CreateWindow(L"MainWindowClass", this->title, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, this->width, this->height, NULL, NULL, this->hInstance, NULL);
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
			HBRUSH hBrBackground = CreateSolidBrush(RGB(0, 0, 0));
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
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
