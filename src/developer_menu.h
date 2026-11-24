#pragma once
#include <windows.h>

namespace developer_menu {

// Opt-in extension, independent of the differential-tested game rewrites.
// Enabled by RT3RE_DEV_MENU=1 or [developer] menu=1 in rt3re.ini.
void Install(const char* iniPath);

// Native menu helpers, also exercised by tools/test_developer_menu.cpp.
HMENU LoadOriginalMenu(HMODULE gameModule);
bool AttachPreservingClient(HWND window, HMENU menu);

} // namespace developer_menu
