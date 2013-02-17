#include "MainWindow.h"
#include <stdio.h>
#include <commctrl.h>

const char MainWindow::classname[] = "MainWindow";

void MainWindow::RegisterWindowClass()
{
	static bool registered = false;
	if (!registered)
	{
		WNDCLASSEX wc;

		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MainWindow::WndProc;
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

const char* MainWindow::ConfigFilename()
{
	static char config_filename[MAX_PATH+1];
	if (config_filename[0] == 0)
	{
		char* dirlast;
		GetModuleFileName(NULL, config_filename, MAX_PATH+1);
		dirlast = strrchr(config_filename, '\\');
		strncpy(dirlast+1, "multiple-cursors.ini", (MAX_PATH+1)-(dirlast+1-config_filename));
	}
	return config_filename;
}

void MainWindow::LoadPreference()
{
	pref.Load(ConfigFilename());
}

MainWindow::MainWindow()
{
	hWnd = NULL;
	LoadPreference();
}

MainWindow::~MainWindow()
{
	UnregisterHotKey(hWnd, next_hotkey);
	GlobalDeleteAtom(next_hotkey);
	UnregisterHotKey(hWnd, escape_hotkey);
	GlobalDeleteAtom(escape_hotkey);
	Shell_NotifyIcon(NIM_DELETE, &nicon);
	DestroyMenu(hTrayMenu);
	DeleteObject(hFont);
	pref.Save(ConfigFilename());
}

LRESULT CALLBACK MainWindow::RealWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_HOTKEY:
			char classname[16];
			GetClassName(GetFocus(), classname, 16);
			if (strcmp(classname, HOTKEY_CLASS) != 0)
			{
				if (wp == next_hotkey)
				{
					cursors.Next();
				}
				else if (wp == escape_hotkey)
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
				}
			}
			break;
		case WM_NOTIFYICON:
			switch (lp)
			{
				case WM_RBUTTONUP:
					POINT point;
					GetCursorPos(&point);
					TrackPopupMenu(hTrayMenu, TPM_BOTTOMALIGN, point.x, point.y, 0, hWnd, NULL);
					break;
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wp))
			{
				case IDM_QUIT:
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	if (msg == msgTaskbarRestart) Shell_NotifyIcon(NIM_ADD, &nicon);
	return 0;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	MainWindow* inst = (MainWindow*)GetWindowLong(hWnd, GWL_USERDATA);
	if (inst) inst->RealWndProc(msg, wp, lp);
	return DefWindowProc(hWnd, msg, wp, lp);
}

void MainWindow::Create()
{
	RegisterWindowClass();
	if (!hWnd)
	{
		hWnd = CreateWindowEx(
				0,
				classname,
				"Configuration",
				WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_SYSMENU & ~WS_VISIBLE,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				350,
				200,
				NULL,
				NULL,
				GetModuleHandle(NULL),
				NULL
		);
		SetWindowLong(hWnd, GWL_USERDATA, (long)this);
		UpdateWindow(hWnd);

		// タスクトレイ再描画できるようにする
		msgTaskbarRestart = RegisterWindowMessage("TaskbarCreated");

		// ホットキー設定
		next_hotkey = GlobalAddAtom("NEXT_HOTKEY");
		RegisterHotKey(hWnd, next_hotkey, pref.GetHotkeyNext().modifiers, pref.GetHotkeyNext().vkeys);
		escape_hotkey = GlobalAddAtom("ESCAPE_HOTKEY");
		RegisterHotKey(hWnd, escape_hotkey, pref.GetHotkeyQuit().modifiers, pref.GetHotkeyQuit().vkeys);

		// タスクトレイアイコン
		nicon.cbSize = sizeof(nicon);
		nicon.hWnd = hWnd;
		nicon.uID = 0;
		nicon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nicon.uCallbackMessage = WM_NOTIFYICON;
		nicon.hIcon = (HICON)LoadImage(
				NULL,
				MAKEINTRESOURCE(IDI_APPLICATION),
				IMAGE_ICON,
				0,
				0,
				LR_DEFAULTSIZE | LR_SHARED
		);
		strcpy(nicon.szTip, "multiple-cursors");
		Shell_NotifyIcon(NIM_ADD, &nicon);

		// ポップアップ表示するメニュー
		MENUITEMINFO mi;
		hTrayMenu = CreatePopupMenu();
		mi.cbSize = sizeof(mi);
		mi.fMask = MIIM_STRING | MIIM_FTYPE | MIIM_ID;
		mi.fType = MFT_STRING;

		mi.wID = IDM_QUIT;
		mi.dwTypeData = "Quit(&Q)";
		InsertMenuItem(hTrayMenu, 1, FALSE, &mi);
	}
}

