#include "cursormanager.h"

CursorManager::CursorManager()
{
	nowCursor = 0;
	cursors[0].Hide();
	cursors[1].Show();
}

CursorManager::~CursorManager()
{
}

void CursorManager::Next()
{
	POINT point;
	if (nowCursor == 0)
	{
		GetCursorPos(&point);
		cursors[0].Move(point.x, point.y);
		cursors[0].Show();
		SetCursorPos(cursors[1].GetX(), cursors[1].GetY());
		cursors[1].Hide();
		nowCursor = 1;
	}
	else if (nowCursor == 1)
	{
		GetCursorPos(&point);
		cursors[1].Move(point.x, point.y);
		cursors[1].Show();
		SetCursorPos(cursors[0].GetX(), cursors[0].GetY());
		cursors[0].Hide();
		nowCursor = 0;
	}
}

void CursorManager::Previous()
{
	// あとで実装する
}

void CursorManager::Use(unsigned int id)
{
	// あとで実装する
}

