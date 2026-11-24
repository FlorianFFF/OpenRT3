#include "developer_menu.h"
#include "hook/hook.h"
#include "core/log.h"
#include <stdint.h>
#include <string.h>

namespace developer_menu {
namespace {

using CreateWindowFn = HWND(WINAPI*)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int,
                                    HWND, HMENU, HINSTANCE, LPVOID);
static CreateWindowFn g_createWindow;
static constexpr UINT_PTR kTimer = 0x5254334d; // RT3M; owned by the game window.

// The original menu owner at 0x6ada8c. Its destructor detaches and destroys
// the handle when active is set, so restoration uses the existing ownership.
struct MenuOwner {
    HMENU menu;
    int32_t active;
};
static_assert(sizeof(MenuOwner) == 8, "RT3 is a 32-bit game");

void CALLBACK MaintainMenu(HWND window, UINT, UINT_PTR, DWORD) {
    auto engine = hook::Global<uint8_t*>(0x6d4024);
    auto owner = hook::Global<MenuOwner*>(0x6ada8c);
    if (!engine || !owner || *(HWND*)engine != window) return;

    // The native menu belongs to windowed mode. Keep the handle for a later
    // return to windowed mode; recreated windows receive their own timer.
    if (engine[0x114226]) {
        if (owner->menu && GetMenu(window) == owner->menu) {
            SetMenu(window, nullptr);
            DrawMenuBar(window);
        }
        return;
    }

    // Do not call the game's menu-check routine until its UI, input and sound
    // managers exist. The timer runs on the game's UI thread after startup.
    if (!hook::Global<void*>(0x6d401c) || !hook::Global<void*>(0x6d4018) ||
        !hook::Global<void*>(0x6d402c)) return;

    // DestroyWindow may have invalidated a menu on a display-mode rebuild.
    if (owner->menu && !IsMenu(owner->menu)) owner->menu = nullptr;
    if (!owner->menu) {
        owner->menu = LoadOriginalMenu(GetModuleHandleA(nullptr));
        if (!owner->menu) {
            rt3::Log("developer menu: cannot load MNUDFLT (%lu)", GetLastError());
            KillTimer(window, kTimer);
            return;
        }
        owner->active = 1;
    }
    if (GetMenu(window) == owner->menu) return;
    if (!AttachPreservingClient(window, owner->menu)) {
        rt3::Log("developer menu: cannot attach to window (%lu)", GetLastError());
        KillTimer(window, kTimer);
        return;
    }
    owner->active = 1;
    // Both original and rewritten dispatchers already handle WM_COMMAND.
    // Refresh the original resource's checks from the game's current options.
    hook::Orig<void(__fastcall*)(HMENU*, int, char)>(0x4625b0)(&owner->menu, 0, 0);
    rt3::Log("developer menu: MNUDFLT attached to %p", window);
}

HWND WINAPI CreateGameWindow(DWORD exStyle, LPCSTR className, LPCSTR title, DWORD style,
                             int x, int y, int width, int height, HWND parent,
                             HMENU menu, HINSTANCE instance, LPVOID param) {
    HWND window = g_createWindow(exStyle, className, title, style, x, y, width, height,
                                 parent, menu, instance, param);
    // Class names can be integer atoms. Only the game's "World" windows need
    // a timer, including windows recreated by a resolution/fullscreen change.
    if (window && (uintptr_t)className > 0xffff && strcmp(className, "World") == 0) {
        if (!SetTimer(window, kTimer, 250, MaintainMenu))
            rt3::Log("developer menu: cannot schedule attachment (%lu)", GetLastError());
    }
    return window;
}

} // namespace

HMENU LoadOriginalMenu(HMODULE gameModule) {
    return LoadMenuA(gameModule, "MNUDFLT");
}

bool AttachPreservingClient(HWND window, HMENU menu) {
    RECT clientBefore, clientAfter, bounds;
    if (!GetClientRect(window, &clientBefore) || !GetWindowRect(window, &bounds)) return false;
    if (!SetMenu(window, menu)) return false;
    DrawMenuBar(window);
    if (!GetClientRect(window, &clientAfter)) return false;
    // Adding a non-client menu otherwise shrinks the rendering/input area.
    int dx = (clientBefore.right - clientBefore.left) - (clientAfter.right - clientAfter.left);
    int dy = (clientBefore.bottom - clientBefore.top) - (clientAfter.bottom - clientAfter.top);
    return SetWindowPos(window, nullptr, 0, 0, bounds.right - bounds.left + dx,
                        bounds.bottom - bounds.top + dy,
                        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE) != FALSE;
}

void Install(const char* iniPath) {
    if (g_createWindow) return;
    char enabled[16] = {};
    GetEnvironmentVariableA("RT3RE_DEV_MENU", enabled, sizeof enabled);
    if (strcmp(enabled, "1") != 0 && !GetPrivateProfileIntA("developer", "menu", 0, iniPath)) return;
    if (!FindResourceA(GetModuleHandleA(nullptr), "MNUDFLT", RT_MENU)) {
        rt3::Log("developer menu: this executable has no MNUDFLT resource");
        return;
    }

    // Hook the game's import, not the system DLL. Original and rewritten
    // Engine_CreateWindow both use this slot. No on-disk EXE bytes change.
    auto slot = (CreateWindowFn*)0x5c82b8;
    DWORD oldProtect;
    if (!VirtualProtect(slot, sizeof *slot, PAGE_READWRITE, &oldProtect)) {
        rt3::Log("developer menu: cannot hook window creation (%lu)", GetLastError());
        return;
    }
    g_createWindow = *slot;
    *slot = CreateGameWindow;
    VirtualProtect(slot, sizeof *slot, oldProtect, &oldProtect);
    rt3::Log("developer menu: enabled; waiting for a windowed game window");
}

} // namespace developer_menu
