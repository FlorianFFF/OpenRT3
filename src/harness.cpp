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
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "core/log.h"

namespace rt3 { struct Collection; void __fastcall Collection_Save(Collection*, int, int32_t, int32_t);
uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len); extern FILE* g_writeTrace; void __fastcall StreamWrite(int32_t, const void*, uint32_t, int32_t); }

namespace drawtrace { void Install(); void FrameEnd(long frame); }

namespace harness {

static char g_load[MAX_PATH], g_out[MAX_PATH];
static int g_frames = 600;
static volatile LONG g_frame;
static volatile DWORD g_mainTid; // thread running the frames (for the stall dump)
static int g_state, g_loadFrame;
// Multiplayer ("mp=1"): no save is loaded; the actions count from the first
// frame (they drive the lobby), and the run starts when the game world exists.
// The generators are left alone: the host hands out the seeds.
static int g_mp, g_actionBase;
// "menu=1": no save either; the script drives the menus from the first frame
// (main menu, map editor, a new game), timed like multiplayer, and the run
// starts once a world exists.
static int g_menu;
static bool Driven() { return g_mp || g_menu; }  // actions from the first frame, on g_ticks
static int g_mpPort;  // "mpport=<n>": the client's own (device) port, see SetPort
// "quit=1": after saving, the game is closed as by the window's close box
// (input event 0xcd, its quit question answered by the modal Enter), so its
// whole shutdown runs; coverage is written again when the DLL unloads.
static int g_quit;
static char g_coverage[MAX_PATH];
static volatile LONG g_ticks;  // main-loop frames and modal-loop passes (multiplayer action timing)
static volatile LONG g_fakeMs = 1000000;
static volatile LONG g_modalPasses; // modal loop passes since the last main-loop frame

static void*& Game() { return hook::As<void*>(g_world); }

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
static float g_mouseX = 0.5f, g_mouseY = 0.5f;  // the scripted cursor, fractions of the client area
static LONG g_actionShift;  // added to the remaining actions' times by a finished wait
static HWND GameWindow();
static BOOL WINAPI FakeGetCursorPos(POINT* p) {
    // The scripted mouse (see RunActions), in screen coordinates.
    HWND w = GameWindow();
    RECT r = {};
    if (w) GetClientRect(w, &r);
    p->x = w ? (LONG)(g_mouseX * (float)(r.right - r.left)) : GetSystemMetrics(SM_CXSCREEN) / 2;
    p->y = w ? (LONG)(g_mouseY * (float)(r.bottom - r.top)) : GetSystemMetrics(SM_CYSCREEN) / 2;
    if (w) ClientToScreen(w, p);
    return TRUE;
}
static BOOL WINAPI FakeSetCursorPos(int, int) { return TRUE; }
static BOOL WINAPI FakeGetKeyboardState(BYTE* k) { memset(k, 0, 256); return TRUE; }
static HWND WINAPI FakeGetForegroundWindow() {
    uint8_t* engine = g_engine;
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

static void RunActions();

// Multiplayer: the client's weekly state checks against the host (0x46b360):
// 0x6cd908 holds the categories compared, and a check entry (session +0x8f28
// list, checksum function +4) whose category differed has +0x98 set. Logged
// as "S <date> compared <mask> mismatch <checksum functions>" when it changes.
static void LogSyncChecks() {
    static uint32_t lastMask, lastDate;
    uint32_t mask = *(volatile uint32_t*)0x6cd908;
    uint8_t* g = (uint8_t*)Game();
    if (!mask || !g) return;
    uint32_t date = *(uint32_t*)(g + 0xd);
    if (mask == lastMask && date == lastDate) return;
    lastMask = mask;
    lastDate = date;
    char line[256];
    int n = wsprintfA(line, "S %u-%u-%u compared %08lx mismatch", *(uint16_t*)(g + 0xd), g[0xf], g[0x11], mask);
    uint8_t* s = g_session;
    if (void** list = s ? *(void***)(s + 0x8f28) : nullptr)
        for (uint8_t* e = (uint8_t*)*list; e && n < 230; e = *(uint8_t**)e)
            if (*(int32_t*)(e + 0x98)) n += wsprintfA(line + n, " %06lx", *(uint32_t*)(e + 4));
    rt3::Log("%s", line);
}

static int __fastcall FrameHook(void* ctx) {
    // Fake clocks from the first frame (startup busy-waits on the real clock).
    if (g_frame == 0) UseFakeClocks();
    if (g_frame == 0 && g_menu) {
        // Startup seeded the generators from the real clock: fixed, as for a
        // loaded save (a new map, a new game). Not in multiplayer, where the
        // host hands out the seeds.
        g_seed = 0x13572468;
        g_lcg = 0x2468ace0;
        ((void(__cdecl*)(unsigned))0x5a299e)(0x1357);
    }
    InterlockedExchangeAdd(&g_fakeMs, 33); // ~30 fps of game time per frame
    int r = ((int(__fastcall*)(void*))0x483f70)(ctx);
    g_mainTid = GetCurrentThreadId();
    InterlockedIncrement(&g_frame);
    InterlockedIncrement(&g_ticks);
    { // per-frame fingerprint, to find where two runs part
        uint8_t* g = (uint8_t*)Game();
        rt3::Log("F %ld lcg %08x simrand %08x world %08x cam %08x modal %ld ms %ld", g_frame,
                 g_lcg, g_seed,
                 g ? rt3::Crc32(g + 4, 0x4c00) : 0, g ? rt3::Crc32(g + 0x4c9c, 0x14) : 0, g_modalPasses, g_fakeMs);
    }
    // Frames with a modal loop also hold that loop's draws, as many as the
    // wall clock allowed: marked so simcheck can leave them out.
    drawtrace::FrameEnd(g_modalPasses ? -g_frame : g_frame);
    g_modalPasses = 0;
    if (g_mp) {
        // The weekly state check (0x46b360) is the game's own, but switched off
        // in the shipped game (0x6cd8ec is only ever cleared): on for the check.
        *(volatile int32_t*)0x6cd8ec = 1;
        LogSyncChecks();
    }
    if (Driven() && g_state == 0) {
        RunActions();
        uint8_t* g = (uint8_t*)Game();
        if (g && *(int32_t*)g) {  // a world that runs (World_Frame's own test)
            g_state = 1;
            g_loadFrame = g_frame;
            rt3::Log("harness: frame %ld, %s running", g_frame, g_mp ? "multiplayer game" : "world");
        }
    } else if (g_state == 0 && g_frame == 100) {
        // As if a saved game were started from the menu: new-game config mode 3
        // ("load save", name at +0x11), then enter screen 1, whose world init
        // (0x438890) loads it. ctx is the app/screen manager (+0x08 = screen).
        uint8_t* cfg = g_newGame;
        *(int32_t*)(cfg + 1) = 3;
        strncpy((char*)cfg + 0x11, g_load, 0x32);
        g_state = 1;
        g_loadFrame = g_actionBase = g_frame;
        // The Setup screen seeds SimRand (0x62431c) with Rand + GetTickCount and
        // loading does not restore it, so fix both generators before loading.
        g_seed = 0x13572468;
        // Multiplayer saves touch the network layer's locks, which only exist
        // after a network session was set up (0x5236a0); create them if missing.
        static const uint32_t kLocks[] = { 0x6d4090, 0x6d406c, 0x6d4048, 0x6d40b4 };
        for (uint32_t cs : kLocks)
            if (((CRITICAL_SECTION*)cs)->DebugInfo == nullptr && ((CRITICAL_SECTION*)cs)->LockCount == 0)
                InitializeCriticalSection((CRITICAL_SECTION*)cs);
        g_lcg = 0x2468ace0;
        // And the C runtime's rand() (effects, particles), whatever startup seeded it with.
        ((void(__cdecl*)(unsigned))0x5a299e)(0x1357);
        rt3::Log("harness: frame %ld, starting %s", g_frame, g_load);
        ((void(__fastcall*)(void*, int, int32_t, int32_t))0x482ec0)(ctx, 0, 1, 0);
        rt3::Log("harness: screen %d, game=%p", *(int32_t*)((uint8_t*)ctx + 8), Game());
    } else if (g_state == 1 && g_frame - g_loadFrame < g_frames) {
        RunActions();
    } else if (g_state == 1) {
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
        sprintf(g_coverage, "%s.coverage", g_out);
        hook::DumpCoverage(g_coverage);
        if (!g_quit) ExitProcess(0);
        g_state = 2;
        rt3::Log("harness: closing the game");
        if (uint8_t* engine = g_engine) PostMessageA(*(HWND*)engine, WM_CLOSE, 0, 0);
    }
    return r;
}

// Two instances on one machine (the multiplayer check): the game binds both the
// host and the joining client to port 9000 (0x521620, the DirectPlay "port"
// component; 9000 when 0), and the second bind fails. In the client
// ("mpport=<n>") its own device address (session +0x18e, +0xc) gets port n;
// the host address keeps 9000. Otherwise exactly 0x521620: the port is kept
// in +0x216 and set on the address object (component 0x5dcf70, 4 bytes, DWORD).
static int32_t __fastcall SetPort(uint8_t* e, int, void* addr, int32_t port) {
    if (!port) port = 9000;
    if (g_mpPort && addr == *(void**)(*(uint8_t**)(e + 0x18e) + 0xc)) port = g_mpPort;
    *(int32_t*)(e + 0x216) = port;
    return ((HRESULT(__stdcall*)(void*, const void*, int32_t*, int32_t, int32_t))(*(void***)addr)[0x48 / 4])(
               addr, (const void*)0x5dcf70, &port, 4, 2) == 0 ? 1 : 0;
}

// The idle callback 0x4840d0 runs a frame (its CALL at 0x4840d6) from loops
// that wait on the clock, such as a button's press animation: there the fake
// clock advances too, or the loop never ends.
static int __fastcall IdleFrame(void* ctx) {
    InterlockedExchangeAdd(&g_fakeMs, 33);
    InterlockedIncrement(&g_ticks);
    if (Driven()) RunActions();
    return ((int(__fastcall*)(void*))0x483f70)(ctx);
}

// Modal dialogs (scenario briefing, news) run their own event loop (0x538f10),
// so main-loop frames stall. To dismiss them deterministically, the modal
// loop's event fetch is wrapped: after a fixed number of loop passes since the
// last main-loop frame, Enter is posted to the game window.
static void PostEnter() {
    if (uint8_t* engine = g_engine) {
        HWND w = *(HWND*)engine;
        PostMessageA(w, WM_KEYDOWN, VK_RETURN, 0x001c0001);
        PostMessageA(w, WM_KEYUP, VK_RETURN, 0xc01c0001);
    }
}
// Scripted player actions ("do=<frame>:<action>,..."), run that many frames
// after loading:
//   <hex addr>            the command at that exe address, as when its hotkey is
//                         pressed (the key table's commands take no arguments);
//                         through the exe address, so the rewrite run gets the rewrite
//   m:<x>:<y>             the mouse moves to (x, y), fractions of the window
//   ld / lu / rd / ru:<x>:<y>  left / right button down / up there
//   k:<vk>                a key pressed and released (virtual key, hex)
//   c:<ch>                a typed character (WM_CHAR, hex)
//   x:<period>:<count>    explores the UI: every period frames, count times, clicks
//                         a visible button picked by the harness's own generator
//                         (fixed seed, so both runs pick alike while they agree;
//                         every 8th time Escape instead). Exit, the file menu and
//                         the train buyer (modal screens that Enter cannot close)
//                         are skipped.
//   w:<n>                 waits until the network session lists n players (0x6d408c);
//                         the actions after it move by however long that took
//   t:<id>:<text>         sets the text of widget id (event 0x6f to every window,
//                         as the game sets an edit box's text)
//   i:<id>                clicks the centre of the visible widget with that id; waits
//                         (like w:) until there is one, at most 3000 frames
//   dump                  logs every window and widget (id, type, rectangle as
//                         fractions of the screen, visible): where to click
// Mouse and keys go in as window messages, as from the real devices, and the
// faked cursor position (GetCursorPos) follows the scripted mouse.
enum ActionKind { kCall, kMove, kLeftDown, kLeftUp, kRightDown, kRightUp, kKey, kDump, kChar, kSetText, kWaitPlayers, kExplore, kClickId };
struct Action { int frame; ActionKind kind; uint32_t addr; float x, y; char text[64]; };
static Action g_actions[1024];
static int g_numActions, g_nextAction;


static HWND GameWindow() {
    uint8_t* engine = g_engine;
    return engine ? *(HWND*)engine : nullptr;
}

static LPARAM MousePoint(HWND w) {
    RECT r = {};
    GetClientRect(w, &r);
    int x = (int)(g_mouseX * (float)(r.right - r.left)), y = (int)(g_mouseY * (float)(r.bottom - r.top));
    return MAKELPARAM(x, y);
}

// The window manager's windows (first +4, next +0x54; rectangle +5 / +9 /
// +0xd / +0x11) and their widgets (first +0x74, next +0x8a; id +4, type +6,
// rectangle +0xb / +0xf / +0x13 / +0x17 within the window).
static void DumpWidgets() {
    uint8_t* m = g_windowManager;
    if (!m) return;
    auto visible = [](uint8_t* w) { return ((int32_t(__fastcall*)(uint8_t*))0x558670)(w); };
    rt3::Log("harness: frame %ld, widgets", g_frame);
    for (uint8_t* w = *(uint8_t**)(m + 4); w; w = *(uint8_t**)(w + 0x54)) {
        float wx = *(float*)(w + 5), wy = *(float*)(w + 9);
        rt3::Log("W window %p at %.4f %.4f size %.4f %.4f", w, wx, wy, *(float*)(w + 0xd), *(float*)(w + 0x11));
        for (uint8_t* c = *(uint8_t**)(w + 0x74); c; c = *(uint8_t**)(c + 0x8a)) {
            float x = wx + *(float*)(c + 0xb), y = wy + *(float*)(c + 0xf), cw = *(float*)(c + 0x13), ch = *(float*)(c + 0x17);
            char text[48] = "";
            const uint16_t type = *(uint16_t*)(c + 6);  // a label's text (+0xba), a control's caption (+0xce, else +0xb2)
            const char* t = type == 2 ? *(const char**)(c + 0xba) : type == 1 ? (*(const char**)(c + 0xce) ? *(const char**)(c + 0xce) : *(const char**)(c + 0xb2)) : nullptr;
            if (t && !IsBadReadPtr(t, 1)) {
                lstrcpynA(text, t, sizeof text);
                for (char* q = text; *q; ++q)
                    if (*q == 10 || *q == 13) *q = ' ';
            }
            rt3::Log("W   id %u type %u at %.4f %.4f size %.4f %.4f centre %.4f %.4f visible %d %s", *(uint16_t*)(c + 4),
                     *(uint16_t*)(c + 6), x, y, cw, ch, x + cw / 2, y + ch / 2, visible(c), text);
        }
    }
}

// The exploring clicks (kExplore): the next one's frame, how many are left.
static LONG g_exploreNext, g_explorePeriod, g_exploreLeft;
static uint32_t g_exploreSeed = 0x2468ace1;

static void Explore(HWND w) {
    uint32_t& r = g_exploreSeed;
    r = r * 1103515245u + 12345u;
    if ((r >> 16) % 8 == 7) {  // Escape: out of windows and dialogs
        PostMessageA(w, WM_KEYDOWN, VK_ESCAPE, 0x00010001);
        PostMessageA(w, WM_KEYUP, VK_ESCAPE, 0xc0010001);
        rt3::Log("harness: frame %ld, explore escape", g_frame);
        return;
    }
    static const uint16_t kSkip[] = { 3106, 2015, 2013 };  // Exit, file menu, train buyer
    float cx[256], cy[256];
    uint16_t ids[256];
    int n = 0;
    auto visible = [](uint8_t* c) { return ((int32_t(__fastcall*)(uint8_t*))0x558670)(c); };
    if (uint8_t* m = g_windowManager)
        for (uint8_t* win = *(uint8_t**)(m + 4); win; win = *(uint8_t**)(win + 0x54))
            for (uint8_t* c = *(uint8_t**)(win + 0x74); c && n < 256; c = *(uint8_t**)(c + 0x8a)) {
                uint16_t id = *(uint16_t*)(c + 4);
                if (*(uint16_t*)(c + 6) != 1 || !visible(c)) continue;
                bool skip = false;
                for (uint16_t s : kSkip) skip |= id == s;
                if (skip) continue;
                cx[n] = *(float*)(win + 5) + *(float*)(c + 0xb) + *(float*)(c + 0x13) / 2;
                cy[n] = *(float*)(win + 9) + *(float*)(c + 0xf) + *(float*)(c + 0x17) / 2;
                ids[n++] = id;
            }
    if (!n) return;
    int i = (int)((r >> 8) % (uint32_t)n);
    g_mouseX = cx[i];
    g_mouseY = cy[i];
    LPARAM pt = MousePoint(w);
    rt3::Log("harness: frame %ld, explore button %u at %.3f %.3f", g_frame, ids[i], cx[i], cy[i]);
    PostMessageA(w, WM_MOUSEMOVE, 0, pt);
    PostMessageA(w, WM_LBUTTONDOWN, MK_LBUTTON, pt);
    PostMessageA(w, WM_LBUTTONUP, 0, pt);
}

// The centre of the visible widget `id` (fractions of the screen), if any.
static bool FindWidget(uint16_t id, float* x, float* y) {
    auto visible = [](uint8_t* c) { return ((int32_t(__fastcall*)(uint8_t*))0x558670)(c); };
    if (uint8_t* m = g_windowManager)
        for (uint8_t* win = *(uint8_t**)(m + 4); win; win = *(uint8_t**)(win + 0x54))
            for (uint8_t* c = *(uint8_t**)(win + 0x74); c; c = *(uint8_t**)(c + 0x8a))
                if (*(uint16_t*)(c + 4) == id && visible(c)) {
                    *x = *(float*)(win + 5) + *(float*)(c + 0xb) + *(float*)(c + 0x13) / 2;
                    *y = *(float*)(win + 9) + *(float*)(c + 0xf) + *(float*)(c + 0x17) / 2;
                    return true;
                }
    return false;
}

static void RunActions() {
    const LONG now = (Driven() ? g_ticks : g_frame - g_actionBase) - g_actionShift;  // (the lobby runs in modal loops)
    if (g_exploreLeft > 0 && now >= g_exploreNext) {
        if (HWND w = GameWindow()) Explore(w);
        --g_exploreLeft;
        g_exploreNext = now + g_explorePeriod;
    }
    while (g_nextAction < g_numActions && now >= g_actions[g_nextAction].frame) {
        const Action& a = g_actions[g_nextAction];
        if (a.kind == kClickId) {
            float x, y;
            HWND cw = GameWindow();
            if (!cw || !FindWidget((uint16_t)a.addr, &x, &y)) {
                if (now - a.frame < 3000) return;  // again next time
                rt3::Log("harness: frame %ld, widget %u not found", g_frame, a.addr);
            } else {
                g_mouseX = x;
                g_mouseY = y;
                LPARAM pt = MousePoint(cw);
                rt3::Log("harness: frame %ld, click widget %u at %.3f %.3f", g_frame, a.addr, x, y);
                PostMessageA(cw, WM_MOUSEMOVE, 0, pt);
                PostMessageA(cw, WM_LBUTTONDOWN, MK_LBUTTON, pt);
                PostMessageA(cw, WM_LBUTTONUP, 0, pt);
            }
            g_actionShift += now - a.frame;
            ++g_nextAction;
            return;
        }
        if (a.kind == kWaitPlayers) {
            int n = 0;
            if (void** l = *(void***)0x6d408c)
                for (uint8_t* e = (uint8_t*)*l; e && n < 64; e = *(uint8_t**)e) ++n;
            if (n < (int)a.addr) return;  // again next time
            rt3::Log("harness: frame %ld, %d players", g_frame, n);
            g_actionShift += now - a.frame;
            ++g_nextAction;
            return;
        }
        ++g_nextAction;
        HWND w = GameWindow();
        if (a.kind == kExplore) {
            g_explorePeriod = (LONG)a.x;
            g_exploreLeft = (LONG)a.y;
            g_exploreNext = now;
            continue;
        }
        if (a.kind == kDump) {
            DumpWidgets();
            continue;
        }
        if (a.kind == kCall) {
            rt3::Log("harness: frame %ld, action %06x", g_frame, a.addr);
            ((int32_t(__cdecl*)())a.addr)();
            continue;
        }
        if (!w) continue;
        if (a.kind == kSetText) {
            rt3::Log("harness: frame %ld, text of %u = %s", g_frame, a.addr, a.text);
            if (uint8_t* m = g_windowManager)
                for (uint8_t* win = *(uint8_t**)(m + 4); win; win = *(uint8_t**)(win + 0x54))
                    ((int32_t(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))0x540120)(
                        win, 0, 0x6f, a.addr, (uint32_t)(uintptr_t)a.text, 0, 0);
            continue;
        }
        if (a.kind == kChar) {
            rt3::Log("harness: frame %ld, char %02x", g_frame, a.addr);
            PostMessageA(w, WM_CHAR, a.addr, 1);
            continue;
        }
        if (a.kind == kKey) {
            rt3::Log("harness: frame %ld, key %02x", g_frame, a.addr);
            UINT scan = MapVirtualKeyA(a.addr, 0);
            PostMessageA(w, WM_KEYDOWN, a.addr, 1 | (scan << 16));
            PostMessageA(w, WM_KEYUP, a.addr, 1 | (scan << 16) | 0xc0000000);
            continue;
        }
        g_mouseX = a.x;
        g_mouseY = a.y;
        LPARAM pt = MousePoint(w);
        static const UINT msg[] = { 0, WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP };
        static WPARAM buttons;  // MK_ bits held
        if (a.kind == kLeftDown) buttons |= MK_LBUTTON;
        if (a.kind == kLeftUp) buttons &= ~(WPARAM)MK_LBUTTON;
        if (a.kind == kRightDown) buttons |= MK_RBUTTON;
        if (a.kind == kRightUp) buttons &= ~(WPARAM)MK_RBUTTON;
        rt3::Log("harness: frame %ld, mouse %d at %d,%d", g_frame, (int)a.kind, LOWORD(pt), HIWORD(pt));
        if (a.kind != kMove) PostMessageA(w, WM_MOUSEMOVE, buttons, pt);
        PostMessageA(w, msg[a.kind], buttons, pt);
    }
}

static void ParseActions(char* list) {
    for (char* p = list; *p && g_numActions < 1024;) {
        Action a = {};
        a.frame = strtol(p, &p, 10);
        if (*p++ != ':') break;
        static const struct { const char* name; ActionKind kind; } kinds[] = {
            { "m:", kMove }, { "ld:", kLeftDown }, { "lu:", kLeftUp }, { "rd:", kRightDown }, { "ru:", kRightUp },
            { "x:", kExplore } };
        bool done = false;
        if (!strncmp(p, "dump", 4)) {
            p += 4;
            a.kind = kDump;
            done = true;
        }
        for (auto& k : kinds) {
            if (done) break;
            size_t n = strlen(k.name);
            if (!strncmp(p, k.name, n)) {
                p += n;
                a.kind = k.kind;
                a.x = (float)strtod(p, &p);
                if (*p == ':') ++p;
                a.y = (float)strtod(p, &p);
                done = true;
                break;
            }
        }
        if (!done) {
            if (!strncmp(p, "i:", 2)) {
                p += 2;
                a.kind = kClickId;
                a.addr = strtoul(p, &p, 10);
                g_actions[g_numActions++] = a;
                if (*p == ',') ++p;
                continue;
            } else if (!strncmp(p, "w:", 2)) {
                p += 2;
                a.kind = kWaitPlayers;
                a.addr = strtoul(p, &p, 10);
                g_actions[g_numActions++] = a;
                if (*p == ',') ++p;
                continue;
            } else if (!strncmp(p, "t:", 2)) {
                p += 2;
                a.kind = kSetText;
                a.addr = strtoul(p, &p, 10);
                if (*p == ':') ++p;
                int n = 0;
                while (*p && *p != ',' && n < 63) a.text[n++] = *p++;
                a.text[n] = 0;
                g_actions[g_numActions++] = a;
                if (*p == ',') ++p;
                continue;
            } else if (!strncmp(p, "c:", 2)) {
                p += 2;
                a.kind = kChar;
            } else if (!strncmp(p, "k:", 2)) {
                p += 2;
                a.kind = kKey;
            } else {
                a.kind = kCall;
            }
            a.addr = strtoul(p, &p, 16);
        }
        g_actions[g_numActions++] = a;
        if (*p == ',') ++p;
    }
}

static void* __fastcall ModalGetEvent(void* input, int, void* out) {
    LONG n = InterlockedIncrement(&g_modalPasses);
    if (Driven()) {
        InterlockedIncrement(&g_ticks);
        RunActions();
    }
    InterlockedExchangeAdd(&g_fakeMs, 33); // dialogs wait on the clock before taking input
    if ((g_state >= 1 || (Driven() && Game())) && (n == 3 || n == 30 || n == 100 || n == 300 || n == 1000 || n == 3000)) {
        rt3::Log("harness: modal pass %ld at frame %ld, posting Enter", n, g_frame);
        PostEnter();
    }
    return ((void*(__fastcall*)(void*, int, void*))0x54f4d0)(input, 0, out);
}

// Last resort if something else blocks frames: logged, since it is not deterministic.
// While stalled after loading, press Enter in the game window.
// The process's oldest thread (its main thread, before any frame ran).
static DWORD OldestThread() {
    DWORD best = 0;
    ULONGLONG bestTime = ~0ull;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;
    THREADENTRY32 te = { sizeof te };
    for (BOOL ok = Thread32First(snap, &te); ok; ok = Thread32Next(snap, &te)) {
        if (te.th32OwnerProcessID != GetCurrentProcessId()) continue;
        if (HANDLE t = OpenThread(THREAD_QUERY_LIMITED_INFORMATION, FALSE, te.th32ThreadID)) {
            FILETIME c, e, k, u;
            if (GetThreadTimes(t, &c, &e, &k, &u)) {
                ULONGLONG v = ((ULONGLONG)c.dwHighDateTime << 32) | c.dwLowDateTime;
                if (v < bestTime) bestTime = v, best = te.th32ThreadID;
            }
            CloseHandle(t);
        }
    }
    CloseHandle(snap);
    return best;
}

static DWORD WINAPI Watchdog(void*) {
    LONG seen = -1;
    for (;;) {
        Sleep(20000);
        LONG f = Driven() ? g_ticks : g_frame;
        if ((g_state == 1 || Driven()) && f == seen) {
            uint8_t* engine = g_engine;
            HWND w = engine ? *(HWND*)engine : nullptr;
            rt3::Log("harness: STALLED at frame %ld (non-deterministic Enter)", f);
            // Where is the frame thread? EIP and the code addresses on its stack.
            if (!g_mainTid) g_mainTid = OldestThread();  // no frame yet: the process's first thread
            if (HANDLE t = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, FALSE, g_mainTid)) {
                // Copy everything while suspended (no CRT calls: the thread may hold its locks).
                static uint32_t copy[4096];
                CONTEXT c = {};
                c.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
                SuspendThread(t);
                BOOL ok = GetThreadContext(t, &c);
                int words = 0;
                if (ok) {
                    const uint32_t* sp = (const uint32_t*)c.Esp;
                    MEMORY_BASIC_INFORMATION mi;
                    if (VirtualQuery(sp, &mi, sizeof mi)) {
                        uintptr_t end = (uintptr_t)mi.BaseAddress + mi.RegionSize;
                        words = (int)((end - (uintptr_t)sp) / 4);
                        if (words > 4096) words = 4096;
                        for (int k = 0; k < words; ++k) copy[k] = sp[k];
                    }
                }
                ResumeThread(t);
                CloseHandle(t);
                if (ok) {
                    // Win32 only from here: the stalled thread may hold CRT locks.
                    HMODULE self = nullptr;
                    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&Watchdog, &self);
                    uintptr_t lo = (uintptr_t)self, hi = lo + 0x400000;
                    char line[2048];
                    int n = wsprintfA(line, "eip %08lx esp %08lx ecx %08lx base %08lx stack:", c.Eip, c.Esp, c.Ecx, (DWORD)lo);
                    int found = 0;
                    for (int k = 0; k < words && found < 60 && n < 1900; ++k) {
                        uint32_t v = copy[k];
                        if ((v >= 0x401000 && v < 0x5c8000) || (v >= lo && v < hi)) {
                            n += wsprintfA(line + n, " %08lx", v);
                            ++found;
                        }
                    }
                    line[n++] = 13;
                    line[n++] = 10;
                    char stallName[32];
                    wsprintfA(stallName, "stall_%lu.txt", GetCurrentProcessId());
                    HANDLE fh = CreateFileA(stallName, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, 0, nullptr);
                    if (fh != INVALID_HANDLE_VALUE) {
                        DWORD wr;
                        WriteFile(fh, line, (DWORD)n, &wr, nullptr);
                        CloseHandle(fh);
                    }
                }
            }
            if (w && !Driven()) {  // (multiplayer: only reported; the lobby is scripted)
                PostMessageA(w, WM_KEYDOWN, VK_RETURN, 0x001c0001);
                PostMessageA(w, WM_KEYUP, VK_RETURN, 0xc01c0001);
            }
        }
        seen = f;
    }
}

// Fresh heap blocks start zeroed: which freed block malloc reuses depends on
// allocations by the Direct3D driver's threads, so leftover heap contents
// differ between runs, and code that reads a field before writing it (the
// original does, here and there) would make two runs of the same game differ.
// The same for the original and the rewrite, and only under the harness.
static uint8_t* g_mallocBody;  // malloc's first instruction (push [abs], 6 bytes), then a jmp back
static void* __cdecl ZeroMalloc(size_t n) {
    void* p = ((void*(__cdecl*)(size_t))(void*)g_mallocBody)(n);
    if (p) memset(p, 0, n);
    return p;
}
static void ZeroFreshHeap() {
    uint8_t* m = (uint8_t*)0x5a125d;
    if (m[0] != 0xFF || m[1] != 0x35) return;  // not the expected first instruction
    g_mallocBody = (uint8_t*)VirtualAlloc(nullptr, 16, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(g_mallocBody, m, 6);
    g_mallocBody[6] = 0xE9;
    *(int32_t*)(g_mallocBody + 7) = (int32_t)((uintptr_t)(m + 6) - (uintptr_t)(g_mallocBody + 11));
    DWORD o;
    VirtualProtect(m, 6, PAGE_EXECUTE_READWRITE, &o);
    m[0] = 0xE9;
    *(int32_t*)(m + 1) = (int32_t)((uintptr_t)&ZeroMalloc - (uintptr_t)(m + 5));
    m[5] = 0x90;
    VirtualProtect(m, 6, o, &o);
    FlushInstructionCache(GetCurrentProcess(), m, 6);
}

void Install() {
    static char spec[8192];
    if (!GetEnvironmentVariableA("RT3RE_HARNESS", spec, sizeof spec)) return;
    for (char* tok = strtok(spec, ";"); tok; tok = strtok(nullptr, ";")) {
        if (!strncmp(tok, "load=", 5)) strcpy(g_load, tok + 5);
        else if (!strncmp(tok, "out=", 4)) strcpy(g_out, tok + 4);
        else if (!strncmp(tok, "frames=", 7)) g_frames = atoi(tok + 7);
        else if (!strncmp(tok, "mp=", 3)) g_mp = atoi(tok + 3);
        else if (!strncmp(tok, "menu=", 5)) g_menu = atoi(tok + 5);
        else if (!strncmp(tok, "mpport=", 7)) g_mpPort = atoi(tok + 7);
        else if (!strncmp(tok, "quit=", 5)) g_quit = atoi(tok + 5);
        else if (!strncmp(tok, "do=@", 4)) {  // from a file (long scripts)
            if (FILE* f = fopen(tok + 4, "rb")) {
                static char buf[1 << 20];
                size_t n = fread(buf, 1, sizeof buf - 1, f);
                fclose(f);
                buf[n] = 0;
                ParseActions(buf);
            }
        } else if (!strncmp(tok, "do=", 3)) ParseActions(tok + 3);
    }
    CreateThread(nullptr, 0, Watchdog, nullptr, 0, nullptr);

    ZeroFreshHeap();
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
    hook::Unredirect((void*)&rt3::StreamWrite);  // (switched off: it would jump back here)
    hook::Unredirect((void*)&rt3::Collection_Save);
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
    if (g_mpPort) {  // the port setter, for the exe's callers and the rewrite's
        force(0x521620, (void*)&SetPort);
        if (void* r = hook::Find(0x521620)) force((uint32_t)(uintptr_t)r, (void*)&SetPort);
    }
    // The idle callback's call of the frame step (CALL rel32 at 0x4840d6): only
    // when the script drives the menus (a lobby button's press loop waits on
    // the clock). Loading a save, the clock advances with main-loop frames only,
    // as the scripted checks were made with.
    if (Driven()) {
        uint8_t* c = (uint8_t*)0x4840d6;
        DWORD o;
        VirtualProtect(c, 5, PAGE_EXECUTE_READWRITE, &o);
        *(int32_t*)(c + 1) = (int32_t)((uintptr_t)&IdleFrame - (uintptr_t)(c + 5));
        VirtualProtect(c, 5, o, &o);
    }
    // Redirect the main loop's call to the per-frame step (CALL rel32 at 0x48439d).
    uint8_t* call = (uint8_t*)0x48439d;
    DWORD old;
    VirtualProtect(call, 5, PAGE_EXECUTE_READWRITE, &old);
    *(int32_t*)(call + 1) = (int32_t)((uintptr_t)&FrameHook - (uintptr_t)(call + 5));
    VirtualProtect(call, 5, old, &old);
    // Per-frame CRC of the draw calls (src/drawtrace.cpp); not in multiplayer,
    // where the sync checks and the saves compare, and menus lock whole buffers.
    if (!Driven()) drawtrace::Install();
    rt3::Log("harness: load=%s frames=%d out=%s", g_load, g_frames, g_out);
}

// DLL_PROCESS_DETACH: after a "quit=1" shutdown, the coverage again (with
// what the shutdown ran).
void Shutdown() {
    if (g_state == 2 && g_coverage[0]) {
        rt3::Log("harness: game closed");
        hook::DumpCoverage(g_coverage);
    }
}

} // namespace harness
