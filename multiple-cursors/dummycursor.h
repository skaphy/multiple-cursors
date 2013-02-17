#ifndef _H_DUMMYCURSOR_
#define _H_DUMMYCURSOR_

#include <windows.h>

class DummyCursor
{
private:
	static const char classname[];
	static void RegisterWindowClass();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	void CreateCursorWindow();
	void MoveCursorToCenter();
	HWND hWnd;
	int x;
	int y;
public:
	DummyCursor();
	virtual ~DummyCursor();
	void Move(int, int);
	void Show();
	void Hide();
	int GetX() const { return x; }
	int GetY() const { return y; }
};

#endif
