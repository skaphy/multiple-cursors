#ifndef _H_MAINWINDOW_
#define _H_MAINWINDOW_

#include <windows.h>
#include "cursormanager.h"
#include "preference.h"

#if defined(__MINGW32__)
#undef  MAKEINTRESOURCE
#define MAKEINTRESOURCE(i) ((LPSTR)((ULONG_PTR)(i)))
#endif

class MainWindow
{
	static const char classname[];
	static const int WM_NOTIFYICON = WM_APP + 1;
	static const int IDM_CONFIG = 0;
	static const int IDM_QUIT = 1;
	void RegisterWindowClass();
	LRESULT CALLBACK RealWndProc(UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	const char* ConfigFilename();
	void LoadPreference();
	HWND hWnd;
	ATOM escape_hotkey;
	ATOM next_hotkey;
	CursorManager cursors;
	NOTIFYICONDATA nicon;
	UINT msgTaskbarRestart;
	HMENU hTrayMenu;
	HFONT hFont;
	Preference pref;
public:
	MainWindow();
	virtual ~MainWindow();
	void Create();
};

#endif

