#include "preference.h"
#include <stdlib.h>

const struct Preference::Hotkey Preference::hotkey_next_default = {MOD_WIN | MOD_CONTROL, VK_TAB};
const struct Preference::Hotkey Preference::hotkey_quit_default = {MOD_WIN | MOD_CONTROL, VK_ESCAPE};

Preference::Preference()
{
	hotkey_next = hotkey_next_default;
	hotkey_quit = hotkey_quit_default;
}

Preference::Preference(const char* filename)
{
	Load(filename);
}

Preference::~Preference()
{
}

void Preference::Load(const char* filename)
{
	hotkey_next.modifiers = GetPrivateProfileInt("hotkey_next", "modifiers", hotkey_next_default.modifiers, filename);
	hotkey_next.vkeys = GetPrivateProfileInt("hotkey_next", "vkeys", hotkey_next_default.vkeys, filename);
	hotkey_quit.modifiers = GetPrivateProfileInt("hotkey_quit", "modifiers", hotkey_quit_default.modifiers, filename);
	hotkey_quit.vkeys = GetPrivateProfileInt("hotkey_quit", "vkeys", hotkey_quit_default.vkeys, filename);
}

void Preference::Save(const char* filename)
{
	char num[6];
	itoa(hotkey_next.modifiers, num, 10);
	WritePrivateProfileString("hotkey_next", "modifiers", num, filename);
	itoa(hotkey_next.vkeys, num, 10);
	WritePrivateProfileString("hotkey_next", "vkeys", num, filename);
	itoa(hotkey_quit.modifiers, num, 10);
	WritePrivateProfileString("hotkey_quit", "modifiers", num, filename);
	itoa(hotkey_quit.vkeys, num, 10);
	WritePrivateProfileString("hotkey_quit", "vkeys", num, filename);
}

void Preference::SetHotkeyNext(UINT modifiers, UINT vkeys)
{
	struct Hotkey hotkey;
	hotkey.modifiers = modifiers;
	hotkey.vkeys = vkeys;
	SetHotkeyNext(hotkey);
}

void Preference::SetHotkeyNext(const struct Preference::Hotkey& hk)
{
	hotkey_next.modifiers = hk.modifiers;
	hotkey_next.vkeys = hk.vkeys;
}

void Preference::SetHotkeyQuit(UINT modifiers, UINT vkeys)
{
	struct Hotkey hotkey;
	hotkey.modifiers = modifiers;
	hotkey.vkeys = vkeys;
	SetHotkeyQuit(hotkey);
}

void Preference::SetHotkeyQuit(const struct Preference::Hotkey& hk)
{
	hotkey_quit.modifiers = hk.modifiers;
	hotkey_quit.vkeys = hk.vkeys;
}

void Preference::SetPoints(const std::vector<POINT> new_points)
{
}

