#ifndef _H_CURSORMANAGER_
#define _H_CURSORMANAGER_

#include "dummycursor.h"

class CursorManager
{
private:
	unsigned int nowCursor;
	DummyCursor cursors[2];
public:
	CursorManager();
	virtual ~CursorManager();
	void Next();
	void Previous();
	void Use(unsigned int);
};
#endif

