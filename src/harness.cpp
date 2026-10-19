// Determinism harness. With RT3RE_HARNESS="load=<save>;frames=<n>;out=<file>"
// set, the game loads <save> shortly after startup, runs <n> frames of game
// time, saves to <file> (silently) and exits.
//
// For runs to be comparable, time must not depend on the wall clock: from the
// moment the save loads, the game's time imports (GetTickCount, QPC, local
// time) are redirected to fake clocks that only advance once per frame.
// Running once with every replacement off and once with them on, then
// comparing the two saves byte by byte, checks that the rewrite behaves
// exactly like the original (tools/simcheck.py).
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hook/hook.h"
#include "core/log.h"

namespace rt3 { struct Collection; void __fastcall Collection_Save(Collection*, int, int32_t, int32_t);
uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len); extern FILE* g_writeTrace; void __fastcall StreamWrite(int32_t, const void*, uint32_t, int32_t); }

namespace harness {

static char g_load[MAX_PATH], g_out[MAX_PATH];
static int g_frames = 600;
static volatile LONG g_frame;
static int g_state, g_loadFrame;
static volatile LONG g_fakeMs = 1000000;
static volatile LONG g_modalPasses; // modal loop passes since the last main-loop frame

static void*& Game() { return hook::Global<void*>(0x6cec78); }

// --- fake clocks -----------------------------------------------------------

static DWORD WINAPI FakeGetTickCount() { return (DWORD)g_fakeMs; }
static BOOL WINAPI FakeQPC(LARGE_INTEGER* c) { c->QuadPart = (LONGLONG)g_fakeMs * 1000; return TRUE; }
static BOOL WINAPI FakeQPF(LARGE_INTEGER* f) { f->QuadPart = 1000000; return TRUE; }
static void WINAPI FakeLocalTime(SYSTEMTIME* t) {
    memset(t, 0, sizeof *t);
    t->wYear = 2004; t->wMonth = 9; t->wDay = 8; t->wHour = 12;
}
static void WINAPI FakeSystemTimeAsFileTime(FILETIME* ft) {
    ULONGLONG v = 127394208000000000ULL + (ULONGLONG)g_fakeMs * 10000; // 2004-09-08
    ft->dwLowDateTime = (DWORD)v;
    ft->dwHighDateTime = (DWORD)(v >> 32);
}

// Mouse/keyboard state the game polls: fixed, so a resting real mouse (edge
// scrolling, hover) cannot change the camera or UI between runs.
static BOOL WINAPI FakeGetCursorPos(POINT* p) {
    p->x = GetSystemMetrics(SM_CXSCREEN) / 2;
    p->y = GetSystemMetrics(SM_CYSCREEN) / 2;
    return TRUE;
}
static BOOL WINAPI FakeSetCursorPos(int, int) { return TRUE; }
static BOOL WINAPI FakeGetKeyboardState(BYTE* k) { memset(k, 0, 256); return TRUE; }
static HWND WINAPI FakeGetForegroundWindow() {
    uint8_t* engine = hook::Global<uint8_t*>(0x6d4024);
    return engine ? *(HWND*)engine : nullptr;
}

static void PatchIat(uint32_t slot, void* fn) {
    DWORD old;
    VirtualProtect((void*)slot, 4, PAGE_READWRITE, &old);
    *(void**)slot = fn;
    VirtualProtect((void*)slot, 4, old, &old);
}

static void UseFakeClocks() {
    PatchIat(0x5c8068, (void*)&FakeGetTickCount);
    PatchIat(0x5c81e0, (void*)&FakeQPC);
    PatchIat(0x5c81dc, (void*)&FakeQPF);
    PatchIat(0x5c8200, (void*)&FakeLocalTime);
    PatchIat(0x5c814c, (void*)&FakeSystemTimeAsFileTime);
    PatchIat(0x5c8270, (void*)&FakeGetCursorPos);
    PatchIat(0x5c8264, (void*)&FakeSetCursorPos);
    PatchIat(0x5c8280, (void*)&FakeGetKeyboardState);
    PatchIat(0x5c8284, (void*)&FakeGetForegroundWindow);
}

// --- frame hook -------------------------------------------------------------

static int __fastcall FrameHook(void* ctx) {
    // Fake clocks from the first frame (startup busy-waits on the real clock).
    if (g_frame == 0) UseFakeClocks();
    InterlockedExchangeAdd(&g_fakeMs, 33); // ~30 fps of game time per frame
    int r = ((int(__fastcall*)(void*))0x483f70)(ctx);
    InterlockedIncrement(&g_frame);
    { // per-frame fingerprint, to find where two runs part
        uint8_t* g = (uint8_t*)Game();
        rt3::Log("F %ld lcg %08x simrand %08x world %08x cam %08x modal %ld", g_frame,
                 hook::Global<uint32_t>(0x624324), hook::Global<uint32_t>(0x62431c),
                 g ? rt3::Crc32(g + 4, 0x4c00) : 0, g ? rt3::Crc32(g + 0x4c9c, 0x14) : 0, g_modalPasses);
    }
    g_modalPasses = 0;
    if (g_state == 0 && g_frame == 100) {
        // As if a saved game were started from the menu: new-game config mode 3
        // ("load save", name at +0x11), then enter screen 1, whose world init
        // (0x438890) loads it. ctx is the app/screen manager (+0x08 = screen).
        uint8_t* cfg = hook::Global<uint8_t*>(0x6cec7c);
        *(int32_t*)(cfg + 1) = 3;
        strncpy((char*)cfg + 0x11, g_load, 0x32);
        g_state = 1;
        g_loadFrame = g_frame;
        // The Setup screen seeds SimRand (0x62431c) with Rand + GetTickCount and
        // loading does not restore it, so fix both generators before loading.
        hook::Global<uint32_t>(0x62431c) = 0x13572468;
        // Multiplayer saves touch the network layer's locks, which only exist
        // after a network session was set up (0x5236a0); create them if missing.
        static const uint32_t kLocks[] = { 0x6d4090, 0x6d406c, 0x6d4048, 0x6d40b4 };
        for (uint32_t cs : kLocks)
            if (((CRITICAL_SECTION*)cs)->DebugInfo == nullptr && ((CRITICAL_SECTION*)cs)->LockCount == 0)
                InitializeCriticalSection((CRITICAL_SECTION*)cs);
        hook::Global<uint32_t>(0x624324) = 0x2468ace0;
        rt3::Log("harness: frame %ld, starting %s", g_frame, g_load);
        ((void(__fastcall*)(void*, int, int32_t, int32_t))0x482ec0)(ctx, 0, 1, 0);
        rt3::Log("harness: screen %d, game=%p", *(int32_t*)((uint8_t*)ctx + 8), Game());
    } else if (g_state == 1 && g_frame - g_loadFrame >= g_frames) {
        uint8_t* g = (uint8_t*)Game();
        rt3::Log("harness: %d frames done, date %u-%u-%u, saving %s", g_frames,
                 g ? *(uint16_t*)(g + 0xd) : 0, g ? g[0xf] : 0, g ? g[0x11] : 0, g_out);
        char trace[MAX_PATH];
        sprintf(trace, "%s.trace", g_out);
        rt3::g_writeTrace = fopen(trace, "w");
        ((void(__fastcall*)(void*, int, const char*, uint32_t))0x444dd0)(g, 0, g_out, 2);
        fclose(rt3::g_writeTrace);
        rt3::g_writeTrace = nullptr;
        rt3::Log("harness: saved, exiting");
        char cov[MAX_PATH];
        sprintf(cov, "%s.coverage", g_out);
        hook::DumpCoverage(cov);
        ExitProcess(0);
    }
    return r;
}

// Modal dialogs (scenario briefing, news) run their own event loop (0x538f10),
// so main-loop frames stall. To dismiss them deterministically, the modal
// loop's event fetch is wrapped: after a fixed number of loop passes since the
// last main-loop frame, Enter is posted to the game window.
static void PostEnter() {
    if (uint8_t* engine = hook::Global<uint8_t*>(0x6d4024)) {
        HWND w = *(HWND*)engine;
        PostMessageA(w, WM_KEYDOWN, VK_RETURN, 0x001c0001);
        PostMessageA(w, WM_KEYUP, VK_RETURN, 0xc01c0001);
    }
}
static void* __fastcall ModalGetEvent(void* input, int, void* out) {
    LONG n = InterlockedIncrement(&g_modalPasses);
    InterlockedExchangeAdd(&g_fakeMs, 33); // dialogs wait on the clock before taking input
    if (g_state == 1 && (n == 3 || n == 30 || n == 100 || n == 300 || n == 1000 || n == 3000)) {
        rt3::Log("harness: modal pass %ld at frame %ld, posting Enter", n, g_frame);
        PostEnter();
    }
    return ((void*(__fastcall*)(void*, int, void*))0x54f4d0)(input, 0, out);
}

// Last resort if something else blocks frames: logged, since it is not deterministic.
// While stalled after loading, press Enter in the game window.
static DWORD WINAPI Watchdog(void*) {
    LONG seen = -1;
    for (;;) {
        Sleep(20000);
        LONG f = g_frame;
        if (g_state == 1 && f == seen) {
            uint8_t* engine = hook::Global<uint8_t*>(0x6d4024);
            HWND w = engine ? *(HWND*)engine : nullptr;
            rt3::Log("harness: STALLED at frame %ld (non-deterministic Enter)", f);
            if (w) {
                PostMessageA(w, WM_KEYDOWN, VK_RETURN, 0x001c0001);
                PostMessageA(w, WM_KEYUP, VK_RETURN, 0xc01c0001);
            }
        }
        seen = f;
    }
}

void Install() {
    char spec[512];
    if (!GetEnvironmentVariableA("RT3RE_HARNESS", spec, sizeof spec)) return;
    for (char* tok = strtok(spec, ";"); tok; tok = strtok(nullptr, ";")) {
        if (!strncmp(tok, "load=", 5)) strcpy(g_load, tok + 5);
        else if (!strncmp(tok, "out=", 4)) strcpy(g_out, tok + 4);
        else if (!strncmp(tok, "frames=", 7)) g_frames = atoi(tok + 7);
    }
    CreateThread(nullptr, 0, Watchdog, nullptr, 0, nullptr);
    // Always use the (verified) rewritten StreamWrite and Collection_Save so
    // saves can be traced (Collection_Save marks its heap-pointer writes).
    auto force = [](uint32_t at, void* fn) {
        uint8_t* p = (uint8_t*)at;
        DWORD o;
        VirtualProtect(p, 5, PAGE_EXECUTE_READWRITE, &o);
        p[0] = 0xE9;
        *(int32_t*)(p + 1) = (int32_t)((uintptr_t)fn - (uintptr_t)(p + 5));
        VirtualProtect(p, 5, o, &o);
    };
    force(0x531030, (void*)&rt3::StreamWrite);
    force(0x517d90, (void*)&rt3::Collection_Save);
    // The modal loop's event fetch (CALL rel32 at 0x538fbe).
    {
        uint8_t* c = (uint8_t*)0x538fbe;
        DWORD o;
        VirtualProtect(c, 5, PAGE_EXECUTE_READWRITE, &o);
        *(int32_t*)(c + 1) = (int32_t)((uintptr_t)&ModalGetEvent - (uintptr_t)(c + 5));
        VirtualProtect(c, 5, o, &o);
    }
    // Redirect the main loop's call to the per-frame step (CALL rel32 at 0x48439d).
    uint8_t* call = (uint8_t*)0x48439d;
    DWORD old;
    VirtualProtect(call, 5, PAGE_EXECUTE_READWRITE, &old);
    *(int32_t*)(call + 1) = (int32_t)((uintptr_t)&FrameHook - (uintptr_t)(call + 5));
    VirtualProtect(call, 5, old, &old);
    rt3::Log("harness: load=%s frames=%d out=%s", g_load, g_frames, g_out);
}

} // namespace harness
