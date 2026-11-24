// Native smoke test using the actual menu resource; does not start the game.
// tools\vc.bat cl /nologo /EHsc /std:c++17 /Isrc tools\test_developer_menu.cpp
//   src\developer_menu.cpp /Fe:out\test_developer_menu.exe /link user32.lib
#include "developer_menu.h"
#include <stdio.h>
#include <stdarg.h>

namespace rt3 {
void Log(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}
}

static int g_lastCommand;
static LRESULT CALLBACK TestWindow(HWND w, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_COMMAND) { g_lastCommand = LOWORD(wp); return 0; }
    return DefWindowProcA(w, msg, wp, lp);
}

static bool HasCommand(HMENU menu, UINT id) {
    MENUITEMINFOA item = { sizeof item };
    item.fMask = MIIM_ID;
    return GetMenuItemInfoA(menu, id, FALSE, &item) && item.wID == id;
}

int main(int argc, char** argv) {
    const char* exe = argc > 1 ? argv[1] : "..\\Railroad Tycoon 3 Full Game\\RT3_mp.exe";
    HMODULE module = LoadLibraryExA(exe, nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if (!module) { fprintf(stderr, "cannot read game resources (%lu)\n", GetLastError()); return 1; }
    HMENU menu = developer_menu::LoadOriginalMenu(module);
    bool ok = menu && GetMenuItemCount(menu) == 10 &&
              HasCommand(menu, 0x7533) && HasCommand(menu, 0x9ccd) && HasCommand(menu, 0x9e0c);

    WNDCLASSA cls = {};
    cls.lpfnWndProc = TestWindow;
    cls.hInstance = GetModuleHandleA(nullptr);
    cls.lpszClassName = "RT3DeveloperMenuSmokeTest";
    RegisterClassA(&cls);
    // Hidden native window: exercise menu layout without opening the game.
    HWND window = CreateWindowExA(0, cls.lpszClassName, "menu test", WS_OVERLAPPEDWINDOW,
                                  0, 0, 800, 600, nullptr, nullptr, cls.hInstance, nullptr);
    RECT before = {}, after = {};
    GetClientRect(window, &before);
    ok = window && ok && developer_menu::AttachPreservingClient(window, menu);
    GetClientRect(window, &after);
    ok = ok && GetMenu(window) == menu && before.right == after.right && before.bottom == after.bottom;
    SendMessageA(window, WM_COMMAND, MAKEWPARAM(0x9ccd, 0), 0);
    ok = ok && g_lastCommand == 0x9ccd;

    // Rechecking an already attached menu must not keep growing the window.
    ok = ok && developer_menu::AttachPreservingClient(window, menu);
    GetClientRect(window, &after);
    ok = ok && before.right == after.right && before.bottom == after.bottom;

    // A recreated game window must be able to attach a fresh copy as well.
    SetMenu(window, nullptr);
    DestroyMenu(menu);
    DestroyWindow(window);
    window = CreateWindowExA(0, cls.lpszClassName, "menu test", WS_OVERLAPPEDWINDOW,
                              0, 0, 800, 600, nullptr, nullptr, cls.hInstance, nullptr);
    menu = developer_menu::LoadOriginalMenu(module);
    ok = ok && menu && developer_menu::AttachPreservingClient(window, menu);
    GetClientRect(window, &after);
    ok = ok && before.right == after.right && before.bottom == after.bottom;
    SetMenu(window, nullptr);
    DestroyMenu(menu);
    DestroyWindow(window);
    FreeLibrary(module);
    puts(ok ? "PASS: original menu, command IDs, WM_COMMAND, reattachment, preserved client size"
            : "FAIL: developer menu smoke test");
    return ok ? 0 : 1;
}
