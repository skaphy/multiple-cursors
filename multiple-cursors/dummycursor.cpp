#include "dummycursor.h"

#if defined(__MINGW32__)
#undef  MAKEINTRESOURCE
#define MAKEINTRESOURCE(i) ((LPSTR)((ULONG_PTR)(i)))
#endif

const char DummyCursor::classname[] = "dummycursor";

void DummyCursor::RegisterWindowClass()
{
	static bool registered = false;
	if (!registered)
	{
		WNDCLASSEX wc;

		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = DummyCursor::WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = (HICON)LoadImage(
				NULL,
				MAKEINTRESOURCE(IDI_APPLICATION),
				IMAGE_ICON,
				0,
				0,
				LR_DEFAULTSIZE | LR_SHARED
		);
		wc.hIconSm = (HICON)LoadImage(
				NULL,
				MAKEINTRESOURCE(IDI_APPLICATION),
				IMAGE_ICON,
				0,
				0,
				LR_DEFAULTSIZE | LR_SHARED
		);
		wc.hCursor = (HCURSOR)LoadImage(
				NULL,
				MAKEINTRESOURCE(IDC_ARROW),
				IMAGE_CURSOR,
				0,
				0,
				LR_DEFAULTSIZE | LR_SHARED
		);
		wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = (LPCSTR)classname;
		RegisterClassEx(&wc);
		registered = true;
	}
}

void DummyCursor::CreateCursorWindow()
{
	hWnd = CreateWindowEx(
			WS_EX_LAYERED | WS_EX_TOOLWINDOW,
			classname,
			"",
			WS_POPUP,
			100,
			100,
			32,
			32,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL
	);
	UpdateWindow(hWnd);
}

DummyCursor::DummyCursor()
{
	RegisterWindowClass();
	CreateCursorWindow();
	MoveCursorToCenter();
}

DummyCursor::~DummyCursor()
{
}

void DummyCursor::MoveCursorToCenter()
{
	// stub
	Move(300, 300);
}

void DummyCursor::Move(int _x, int _y)
{
	x = _x;
	y = _y;
}

void DummyCursor::Show()
{
	SetWindowPos(hWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd, SW_SHOWNORMAL);
}

void DummyCursor::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}

LRESULT CALLBACK DummyCursor::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_CREATE:
			{
				SetLayeredWindowAttributes(hWnd, RGB(64, 64, 64), 0, LWA_COLORKEY);
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hWnd, &ps);
				DrawIcon(hdc, 0, 0, (HICON)LoadImage(
							NULL,
							MAKEINTRESOURCE(IDC_ARROW),
							IMAGE_CURSOR,
							0,
							0,
							LR_DEFAULTSIZE | LR_SHARED
				));
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			break;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}
