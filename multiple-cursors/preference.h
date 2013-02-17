#ifndef _H_PREFERENCE_
#define _H_PREFERENCE_

#include <windows.h>
#include <vector>

class Preference
{
public:
	struct Hotkey
	{
		UINT modifiers;
		UINT vkeys;
	};
private:
	static const struct Hotkey hotkey_next_default;
	static const struct Hotkey hotkey_quit_default;
	std::vector<POINT> points;
	Hotkey hotkey_next;
	Hotkey hotkey_quit;
public:
	Preference();
	Preference(const char*);
	virtual ~Preference();
	void Load(const char*);
	void Save(const char*);
	const struct Hotkey& GetHotkeyNext() { return hotkey_next; }
	void SetHotkeyNext(UINT, UINT);
	void SetHotkeyNext(const struct Hotkey&);
	const struct Hotkey& GetHotkeyQuit() { return hotkey_quit; }
	void SetHotkeyQuit(UINT, UINT);
	void SetHotkeyQuit(const struct Hotkey&);
	const std::vector<POINT>& GetPoints() { return points; }
	void SetPoints(const std::vector<POINT>);
};

#endif
