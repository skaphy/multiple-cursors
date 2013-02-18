#include "HotkeyWindow.h"
#include <stdio.h>

/*
modifier(Ctrl,Alt,Shift)を先に押してmodifier以外が押された時点で確定
*/

#if defined(__MINGW32__)
#undef  MAKEINTRESOURCE
#define MAKEINTRESOURCE(i) ((LPSTR)((ULONG_PTR)(i)))
#endif

char* classname = "mc-config-hotkey-window";

void RegisterHotkeyWindowClass();
LRESULT CALLBACK HotkeyWndProc(HWND, UINT, WPARAM, LPARAM);
UINT GetModifiers(BYTE*);

void RegisterHotkeyWindowClass()
{
	static bool registered = false;
	if (!registered)
	{
		WNDCLASSEX wc;

		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = HotkeyWndProc;
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
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = (LPCSTR)classname;
		RegisterClassEx(&wc);
		registered = true;
	}
}

HWND CreateHotkeyWindow(HWND hParentWnd)
{
	RegisterHotkeyWindowClass();
	HWND hWnd = CreateWindowEx(
			0,
			classname,
			"Hotkey",
			WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			300,
			200,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL
	);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)hParentWnd);
	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOWNORMAL);
	return hWnd;
}

LRESULT CALLBACK HotkeyWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_CREATE:
			CreateWindowEx(
					0,
					"STATIC",
					"Press any key combinations to set for hotkey",
					WS_CHILD | WS_VISIBLE,
					0,
					0,
					300,
					200,
					hWnd,
					NULL,
					GetModuleHandle(NULL),
					NULL
			);
			break;
		case WM_KEYDOWN:
			if (wp != VK_CONTROL && wp != VK_MENU && wp != VK_SHIFT && wp != VK_LWIN && wp != VK_RWIN)
			{
				// modifier以外が押された
				BYTE state[256];
				GetKeyboardState(state);
				if (state[VK_CONTROL] || state[VK_MENU] || state[VK_SHIFT] || state[VK_LWIN] || state[VK_RWIN])
				{
					// modifierを1つでも押している
					SendMessage((HWND)GetWindowLong(hWnd, GWL_USERDATA), WM_HOTKEY_PRESSED, GetModifiers(state), wp);
					SendMessage(hWnd, WM_CLOSE, 0, 0);
				}
			}
			break;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

UINT GetModifiers(BYTE* state)
{
	return
		(state[VK_CONTROL]&0x80 ? MOD_CONTROL : 0) |
		(state[VK_MENU]&0x80 ? MOD_ALT : 0) |
		(state[VK_SHIFT]&0x80 ? MOD_SHIFT : 0) |
		(state[VK_LWIN]&0x80 || state[VK_RWIN]&0x80 ? MOD_WIN : 0);
}

