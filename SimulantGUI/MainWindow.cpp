#include "MainWindow.h"
#include "GUI.h"
#include "resource.h"

WNDPROC MainWindow::oldButtonProc;

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
	DeleteObject(this->hFontHeading);
	DeleteObject(this->hBMP);
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

void MainWindow::setDescription(const std::wstring description)
{
	SetWindowText(this->editInfo, description.c_str());
}

void MainWindow::setButtonBackEnabled(bool enabled)
{
	EnableWindow(this->buttonBack, enabled);
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

	int voff = 100;

	this->editInfo = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L""
		, WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_READONLY
		, 368, voff
		, 800-368-50, 268, this->hWnd, HMENU(NULL), hInstance, NULL);
	ShowWindow(this->editInfo, SW_SHOW);

	this->buttonStart = CreateWindow(L"BUTTON", L"Start !"
		, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 50, 268+50+voff
		, 150, 40, this->hWnd, HMENU(NULL), hInstance, NULL);
	ShowWindow(this->buttonStart, SW_SHOW);

	this->buttonBack = CreateWindow(L"BUTTON", L"Zpìt"
		, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED
		, 50, 268 + 50 + 50 + voff
		, 150, 40, this->hWnd, HMENU(NULL), hInstance, NULL);
	ShowWindow(this->buttonBack, SW_SHOW);

	this->buttonBetUp = CreateWindow(L"BUTTON", L"Sázka +"
		, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 218, 268 + 50 + voff
		, 100, 40, this->hWnd, HMENU(NULL), hInstance, NULL);
	ShowWindow(this->buttonBack, SW_SHOW);

	this->buttonBetDown = CreateWindow(L"BUTTON", L"Sázka -"
		, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 218, 268 + 50 + 50 + voff
		, 100, 40, this->hWnd, HMENU(NULL), hInstance, NULL);
	ShowWindow(this->buttonBack, SW_SHOW);

	this->hFontHeading = CreateFont(30, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
	MainWindow::oldButtonProc = (WNDPROC)SetWindowLong(this->buttonStart, GWL_WNDPROC, (LONG)this->newButtonProc);
	SetWindowLong(this->buttonBack, GWL_WNDPROC, (LONG)this->newButtonProc);
	SetWindowLong(this->buttonBetUp, GWL_WNDPROC, (LONG)this->newButtonProc);
	SetWindowLong(this->buttonBetDown, GWL_WNDPROC, (LONG)this->newButtonProc);
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


			rect.top = 268+50+70, rect.left = 368, rect.right = 800, rect.bottom = 268 + 50 + 100;
			DrawText(this->hDC, L"Bonusová okénka:", -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			rect.top = 0, rect.left = 50, rect.right = 750, rect.bottom = 100;
			SelectObject(this->hDC, this->hFontHeading);
			DrawText(this->hDC, L"Hra 21 Scratch It!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			if ((HWND)lParam == this->buttonStart)
				this->gui->userStartClicked();
			else if ((HWND)lParam == this->buttonBack)
				this->gui->userBackClicked();
			else if ((HWND)lParam == this->buttonBetUp)
				this->gui->userBetUpClicked();
			else if ((HWND)lParam == this->buttonBetDown)
				this->gui->userBetDownClicked();
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT MainWindow::newButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			SendMessage(hWnd, BM_CLICK, 0, 0);
			break;
		}
	default:
		return CallWindowProc(MainWindow::oldButtonProc, hWnd, message, wParam, lParam);
	}
	return 0;
}
