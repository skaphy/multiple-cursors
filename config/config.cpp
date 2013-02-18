#include <windows.h>
#include "config.h"
#include "../multiple-cursors/preference.h"
#include "HotkeyWindow.h"

#if defined(__MINGW32__)
#undef  MAKEINTRESOURCE
#define MAKEINTRESOURCE(i) ((LPSTR)((ULONG_PTR)(i)))
#endif

#define BUTTON_HOTKEY_NEXT 0
#define BUTTON_HOTKEY_QUIT 1
#define BUTTON_SAVE 2
#define BUTTON_QUIT 3

#define CONFIGURING_HOTKEY_NEXT 1
#define CONFIGURING_HOTKEY_QUIT 2

HWND CreateConfigWindow(int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ReloadConfigProc(HWND, LPARAM);
const char* ConfigFilename();
void SetHotkeyText(HWND, UINT, UINT);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;

	HWND hWnd = CreateConfigWindow(nCmdShow);

	while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

HWND CreateConfigWindow(int nCmdShow)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
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
	wc.lpszClassName = (LPCSTR)"multiple-cursors-config";
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(
			0,
			"multiple-cursors-config",
			"Configuration",
			WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			350,
			200,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL
	);
	UpdateWindow(hWnd);
	ShowWindow(hWnd, nCmdShow);
	return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static Preference pref;
	static HFONT hFont;
	static HWND hk_next_btn;
	static HWND hk_quit_btn;
	static int now_configuring = 0;
	switch(msg)
	{
		case WM_CREATE:
			{
				LOGFONT lf;
				SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
				hFont = CreateFontIndirect(&lf);
				SendMessage(CreateWindow("STATIC", "Hotkey(next cursor): ", WS_CHILD|WS_VISIBLE,10,10,150,20,hWnd,NULL,GetModuleHandle(NULL),NULL), WM_SETFONT, (WPARAM)hFont, 1);
				SendMessage(CreateWindow("STATIC", "Hotkey(quit): ", WS_CHILD|WS_VISIBLE,10,50,150,20,hWnd,NULL,GetModuleHandle(NULL),NULL), WM_SETFONT, (WPARAM)hFont, 1);
				hk_next_btn = CreateWindow("BUTTON", "", WS_CHILD|WS_VISIBLE,160,10,150,20,hWnd,(HMENU)BUTTON_HOTKEY_NEXT,GetModuleHandle(NULL),NULL);
				SendMessage(hk_next_btn, WM_SETFONT, (WPARAM)hFont, 1);
				hk_quit_btn = CreateWindow("BUTTON", "", WS_CHILD|WS_VISIBLE,160,50,150,20,hWnd,(HMENU)BUTTON_HOTKEY_QUIT,GetModuleHandle(NULL),NULL);
				SendMessage(hk_quit_btn, WM_SETFONT, (WPARAM)hFont, 1);
				SendMessage(CreateWindow("BUTTON", "Save", WS_CHILD|WS_VISIBLE,10,80,150,30,hWnd,(HMENU)BUTTON_SAVE,GetModuleHandle(NULL),NULL), WM_SETFONT, (WPARAM)hFont, 1);
				SendMessage(CreateWindow("BUTTON", "Quit", WS_CHILD|WS_VISIBLE,170,80,150,30,hWnd,(HMENU)BUTTON_QUIT,GetModuleHandle(NULL),NULL), WM_SETFONT, (WPARAM)hFont, 1);
				pref.Load(ConfigFilename());
				SetHotkeyText(hk_next_btn, pref.GetHotkeyNext().modifiers, pref.GetHotkeyNext().vkeys);
				SetHotkeyText(hk_quit_btn, pref.GetHotkeyQuit().modifiers, pref.GetHotkeyQuit().vkeys);
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wp))
			{
				case BUTTON_HOTKEY_NEXT:
					now_configuring = CONFIGURING_HOTKEY_NEXT;
					SetWindowLong(hk_next_btn, GWL_STYLE, GetWindowLong(hk_next_btn, GWL_STYLE) | WS_DISABLED);
					SetWindowLong(hk_quit_btn, GWL_STYLE, GetWindowLong(hk_quit_btn, GWL_STYLE) | WS_DISABLED);
					CreateHotkeyWindow(hWnd);
					break;
				case BUTTON_HOTKEY_QUIT:
					now_configuring = CONFIGURING_HOTKEY_QUIT;
					SetWindowLong(hk_next_btn, GWL_STYLE, GetWindowLong(hk_next_btn, GWL_STYLE) | WS_DISABLED);
					SetWindowLong(hk_quit_btn, GWL_STYLE, GetWindowLong(hk_quit_btn, GWL_STYLE) | WS_DISABLED);
					CreateHotkeyWindow(hWnd);
					break;
				case BUTTON_SAVE:
					pref.Save(ConfigFilename());
					EnumWindows(ReloadConfigProc, 0);
					MessageBox(hWnd, "Saved", "Save", MB_OK);
					break;
				case BUTTON_QUIT:
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
			}
			break;
		case WM_HOTKEY_PRESSED:
			SetWindowLong(hk_next_btn, GWL_STYLE, GetWindowLong(hk_next_btn, GWL_STYLE) & ~WS_DISABLED);
			SetWindowLong(hk_quit_btn, GWL_STYLE, GetWindowLong(hk_quit_btn, GWL_STYLE) & ~WS_DISABLED);
			if (now_configuring == CONFIGURING_HOTKEY_NEXT)
			{
				pref.SetHotkeyNext(wp, lp);
				SetHotkeyText(hk_next_btn, wp, lp);
			}
			else if (now_configuring == CONFIGURING_HOTKEY_QUIT)
			{
				pref.SetHotkeyQuit(wp, lp);
				SetHotkeyText(hk_quit_btn, wp, lp);
			}
			now_configuring = 0;
			break;
		case WM_DESTROY:
			DeleteObject(hFont);
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

BOOL CALLBACK ReloadConfigProc(HWND hWnd, LPARAM lp)
{
	char cn[50];
	GetClassName(hWnd, cn, 50);
	if (strncmp(cn, "multiple-cursors-MainWindow", 28) == 0)
	{
		SendMessage(hWnd, WM_RELOAD_CONFIG, 0, 0);
	}
	return TRUE;
}

const char* ConfigFilename()
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

void SetHotkeyText(HWND hWnd, UINT modifiers, UINT vkeys)
{
	char text[128] = {0};
	if (modifiers & MOD_CONTROL) strcat(text, "Ctrl + ");
	if (modifiers & MOD_ALT) strcat(text, "Alt + ");
	if (modifiers & MOD_SHIFT) strcat(text, "Shift + ");
	if (modifiers & MOD_WIN) strcat(text, "Win + ");
	if (vkeys == VK_BACK) strcat(text, "Backspace");
	else if (vkeys == VK_TAB) strcat(text, "TAB");
	else if (vkeys == VK_RETURN) strcat(text, "Enter");
	else if (vkeys == VK_ESCAPE) strcat(text, "Esc");
	else if (vkeys == VK_F1) strcat(text, "F1");
	else if (vkeys == VK_F2) strcat(text, "F2");
	else if (vkeys == VK_F3) strcat(text, "F3");
	else if (vkeys == VK_F4) strcat(text, "F4");
	else if (vkeys == VK_F5) strcat(text, "F5");
	else if (vkeys == VK_F6) strcat(text, "F6");
	else if (vkeys == VK_F7) strcat(text, "F7");
	else if (vkeys == VK_F8) strcat(text, "F8");
	else if (vkeys == VK_F9) strcat(text, "F9");
	else if (vkeys == VK_F10) strcat(text, "F10");
	else if (vkeys == VK_F11) strcat(text, "F11");
	else if (vkeys == VK_F12) strcat(text, "F12");
	else
	{
		if (vkeys >= 0x30 && vkeys <= 0x5a)
		{
			text[strlen(text)] = vkeys;
			text[strlen(text)] = 0;
		}
		else
		{
			strcat(text, "???");
		}
	}
	SetWindowText(hWnd, text);
}

