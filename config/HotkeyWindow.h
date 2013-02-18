#ifndef _H_HOTKEYWINDOW_
#define _H_HOTKEYWINDOW_

#include <windows.h>

#define WM_HOTKEY_PRESSED WM_APP + 1

HWND CreateHotkeyWindow(HWND);

#endif
