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
#include <mmdeviceapi.h>
#include <audiopolicy.h>
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
// "sound=1": the effects play (volume 0.6 through the game's own setter once a
// world runs; the settings the tests start from have them at 0), so the sound
// code runs too. The process's audio session is muted first: nothing is heard.
static int g_soundOn;
static char g_coverage[MAX_PATH];
static volatile LONG g_ticks;  // main-loop frames and modal-loop passes (multiplayer action timing)
static volatile LONG g_fakeMs = 1000000;
static volatile LONG g_modalPasses; // modal loop passes since the last main-loop frame
// Enter pressed in modal loops (dialogs answered with their default); a
// script that works a dialog itself switches it off ("autoenter 0").
static int g_autoEnter = 1;

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

// "sound=1": mutes this process's default audio session (Core Audio), so the
// effects that now play are not heard.
static void MuteProcessAudio() {
    HRESULT init = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    IMMDeviceEnumerator* en = nullptr;
    IMMDevice* dev = nullptr;
    IAudioSessionManager* mgr = nullptr;
    ISimpleAudioVolume* vol = nullptr;
    bool muted = false;
    if (SUCCEEDED(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&en)) &&
        SUCCEEDED(en->GetDefaultAudioEndpoint(eRender, eConsole, &dev)) &&
        SUCCEEDED(dev->Activate(__uuidof(IAudioSessionManager), CLSCTX_ALL, nullptr, (void**)&mgr)) &&
        SUCCEEDED(mgr->GetSimpleAudioVolume(nullptr, FALSE, &vol)))
        muted = SUCCEEDED(vol->SetMute(TRUE, nullptr));
    if (vol) vol->Release();
    if (mgr) mgr->Release();
    if (dev) dev->Release();
    if (en) en->Release();
    if (SUCCEEDED(init)) CoUninitialize();
    rt3::Log("harness: audio session %s", muted ? "muted" : "NOT muted (no audio device?)");
}

// Effects and music volume 0.6: the engine's settings (+0x1146fa / +0x1146fe, which the game applies
// itself) and the sound system's (0x531560, kind 2 = effects).
static void SoundUp() {
    uint8_t* e = g_engine;
    uint8_t* s = g_sound;
    if (!e || !s) return;
    *(float*)(e + 0x1146fa) = 0.6f;
    ((void(__fastcall*)(uint8_t*, int, float, int32_t))0x531560)(s, 0, 0.6f, 2);
    // And the music (+0x1146fe, applied x 0x5c9c80 as the game does, kind 0).
    *(float*)(e + 0x1146fe) = 0.6f;
    ((void(__fastcall*)(uint8_t*, int, float, int32_t))0x531560)(s, 0, (float)(0.6 * *(const double*)0x5c9c80), 0);
    rt3::Log("harness: frame %ld, effects and music volume 0.6", g_frame);
}

static int __fastcall FrameHook(void* ctx) {
    // Fake clocks from the first frame (startup busy-waits on the real clock).
    if (g_frame == 0) UseFakeClocks();
    if (g_soundOn && g_frame == 0) MuteProcessAudio();
    if (g_soundOn && g_state == 1 && g_frame == g_loadFrame + 10) SoundUp();
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
//   shot                  a screenshot of the game window (Screenshot)
//   kd / ku:<vk>          a key down / up (held in between)
//   wh:<ticks>            the mouse wheel at the mouse position
//   waitgame              waits until the game world runs (like w:)
//   ae:<0|1>              Enter in dialogs (g_autoEnter) off / on
// The game itself (a world): the in-game camera reads the mouse through
// DirectInput, so these use the game's own calls, and map cells instead of
// screen positions:
//   look:<x>:<y>          centres the view on map cell (x, y) (0x43d0c0)
//   zoom:<n>              n steps along the zoom ladder, negative in (0x43cc30)
//   cm / cd / cu:<x>:<y>  mouse move / left down / left up over map cell (x, y),
//                         found on screen with the camera's ground pick (ScreenOfCell)
//   cities / tracks       logs every city ("C <id> <x> <y> <name>") / track piece
//                         ("T <id> <x> <y> <owner>") with its map cell
// Mouse and keys go in as window messages, as from the real devices, and the
// faked cursor position (GetCursorPos) follows the scripted mouse.
enum ActionKind { kCall, kMove, kLeftDown, kLeftUp, kRightDown, kRightUp, kKey, kDump, kChar, kSetText, kWaitPlayers, kExplore, kClickId, kShot, kWheel, kKeyDown, kKeyUp, kZoom, kLook, kCities, kCell, kTracks, kAutoEnter, kWaitGame, kCallFn };
struct Action { int frame; ActionKind kind; uint32_t addr; float x, y; char text[160]; uint32_t objColl; int32_t objIndex; };  // (objColl: x, y are that object's cell)
static Action g_actions[1024];
static int g_numActions, g_nextAction;

// The map cell of the action's object (the objIndex-th of the collection whose
// pointer is the global objColl; 0x455800 / 0x455810, its position in cells).
static void* CollectionObject(uint32_t global, int32_t index, bool wantId, uint32_t* id);
static bool ObjectCell(const Action& a, float* x, float* y) {
    uint32_t id = 0;
    void* o = CollectionObject(a.objColl, a.objIndex, false, &id);
    if (!o) {
        rt3::Log("harness: frame %ld, no object %x.%d", g_frame, a.objColl, a.objIndex);
        return false;
    }
    *x = (float)((double(__fastcall*)(void*))0x455800)(o);
    *y = (float)((double(__fastcall*)(void*))0x455810)(o);
    return true;
}


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
// "shot": the game window's client area, to <out>_<tick>.bmp in the game
// folder (tools/shot.py makes PNGs of them), to look at what a script does;
// nothing compares them. PrintWindow renders the game window itself, never
// what covers it on the desktop.
static void Screenshot(HWND w) {
    RECT rc, wr;
    if (!w || !GetClientRect(w, &rc) || !GetWindowRect(w, &wr)) return;
    POINT o = { 0, 0 };
    ClientToScreen(w, &o);
    o.x -= wr.left;  // the client area within the whole window's image
    o.y -= wr.top;
    const int cx = rc.right, cy = rc.bottom;
    HDC screen = GetDC(nullptr), dc = CreateCompatibleDC(screen), whole = CreateCompatibleDC(screen);
    HBITMAP wbmp = CreateCompatibleBitmap(screen, wr.right - wr.left, wr.bottom - wr.top);
    HGDIOBJ wold = SelectObject(whole, wbmp);
    PrintWindow(w, whole, 2 /* PW_RENDERFULLCONTENT */);
    BITMAPINFO bi = {};
    bi.bmiHeader.biSize = sizeof bi.bmiHeader;
    bi.bmiHeader.biWidth = cx;
    bi.bmiHeader.biHeight = cy;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 24;
    void* bits = nullptr;
    HBITMAP bmp = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (bmp) {
        HGDIOBJ old = SelectObject(dc, bmp);
        BitBlt(dc, 0, 0, cx, cy, whole, o.x, o.y, SRCCOPY);
        SelectObject(dc, old);
        char name[MAX_PATH];
        sprintf(name, "%s_%05ld.bmp", g_out, g_ticks);
        if (FILE* f = fopen(name, "wb")) {
            const DWORD row = (cx * 3 + 3) & ~3u, size = row * cy;
            BITMAPFILEHEADER fh = { 0x4d42, (DWORD)(sizeof fh + sizeof bi.bmiHeader + size), 0, 0,
                                    (DWORD)(sizeof fh + sizeof bi.bmiHeader) };
            fwrite(&fh, sizeof fh, 1, f);
            fwrite(&bi.bmiHeader, sizeof bi.bmiHeader, 1, f);
            fwrite(bits, size, 1, f);
            fclose(f);
            rt3::Log("harness: frame %ld, screenshot %s", g_frame, name);
        }
        DeleteObject(bmp);
    }
    SelectObject(whole, wold);
    DeleteObject(wbmp);
    DeleteDC(whole);
    DeleteDC(dc);
    ReleaseDC(nullptr, screen);
}

// The window position (fractions) over map cell (tx, ty): the camera's own
// ground pick (0x43d320, at the terrain height of the cell, 0x448bd0) solved
// by Newton steps from the centre. False when the cell is not in view.
static bool ScreenOfCell(float tx, float ty, float* ox, float* oy) {
    uint8_t* cam = g_62be68;
    uint8_t* map = g_map;
    if (!cam || !map) return false;
    const float h = (float)((double(__fastcall*)(uint8_t*, int, float, float))0x448bd0)(map, 0, tx, ty);
    auto pick = [&](float sx, float sy, float* cx, float* cy) {
        return ((int32_t(__fastcall*)(uint8_t*, int, float, float, float, float*, float*))0x43d320)(cam, 0, sx, sy, h, cx, cy) != 0;
    };
    float sx = 0.5f, sy = 0.4f;
    for (int it = 0; it < 40; ++it) {
        float cx, cy, ax, ay, bx, by;
        const float d = 0.01f;
        if (!pick(sx, sy, &cx, &cy) || !pick(sx + d, sy, &ax, &ay) || !pick(sx, sy + d, &bx, &by)) return false;
        const float ex = tx - cx, ey = ty - cy;
        if (ex * ex + ey * ey < 0.04f) {
            *ox = sx;
            *oy = sy;
            return sx > 0.0f && sx < 1.0f && sy > 0.0f && sy < 1.0f;
        }
        // Jacobian of the cell over the screen position, by differences.
        const float j11 = (ax - cx) / d, j21 = (ay - cy) / d, j12 = (bx - cx) / d, j22 = (by - cy) / d;
        const float det = j11 * j22 - j12 * j21;
        if (det == 0.0f) return false;
        float dx = (ex * j22 - ey * j12) / det, dy = (ey * j11 - ex * j21) / det;
        const float m = 0.15f;  // a step at most this far (the pick is far from linear near the horizon)
        if (dx > m) dx = m; else if (dx < -m) dx = -m;
        if (dy > m) dy = m; else if (dy < -m) dy = -m;
        sx += dx;
        sy += dy;
        if (sx < 0.01f) sx = 0.01f; else if (sx > 0.99f) sx = 0.99f;
        if (sy < 0.01f) sy = 0.01f; else if (sy > 0.99f) sy = 0.99f;
    }
    return false;
}

// "fn:<spec>": calls a game function by its exe address, the same in every run
// (the original run gets the original, the rewrite run the rewrite through the
// replacement JMP), and logs what it returned: "R <addr> <eax> <st0 bits>
// <crc of the scratch buffers>". simcheck compares these lines between the
// runs (and the save catches what the call changed). Spec, '/'-separated:
//   <hex addr>/<cc>/<ret>/<arg>/...
//   cc   f: the first two args in ECX / EDX, the rest on the stack (thiscall
//           methods are f with a dummy EDX); s / c: all on the stack
//   ret  i: EAX, b: AL, f: ST0 (float or double), v: nothing
//   arg  n<dec> / x<hex> integer, f<float>, d<double> (two words),
//        o<coll>.<i> the i-th object and i<coll>.<i> its id of the collection
//        whose pointer is the global <coll> (hex), g<hex> the dword global,
//        p<hex>.<off>[.<off>..] the dword at +off (hex) of the object global
//        <hex> points to (each further .<off> follows that pointer again),
//        a<hex> the address itself, b<n> scratch buffer n (0..3, 1 KB, zeroed
//        before the call: out parameters), s<text> a string
// A fault in the call is caught and logged as "R <addr> fault".
static uint8_t g_scratch[4][1024];
static char g_callStrings[8][64];

static void* CollectionObject(uint32_t global, int32_t index, bool wantId, uint32_t* id) {
    rt3::Collection* c = *(rt3::Collection**)global;
    if (!c) return nullptr;
    const int32_t n = ((int32_t(__fastcall*)(rt3::Collection*))0x517cf0)(c);
    if (n <= 0) return nullptr;
    index = index % n;
    const uint32_t i = ((uint32_t(__fastcall*)(rt3::Collection*, int, uint32_t, int))0x518380)(c, 0, (uint32_t)index, 0);
    if (wantId) *id = i;
    return ((void*(__fastcall*)(rt3::Collection*, int, uint32_t))0x518140)(c, 0, i);
}

// Pushes stack[0..n) (stack[0] nearest), sets ECX / EDX and calls fn; ESP is
// put back afterwards whatever the callee popped (stdcall or cdecl).
static int CallRaw(uint32_t fn, uint32_t ecxv, uint32_t edxv, const uint32_t* stack, int n, uint32_t* eaxOut,
                   uint64_t* st0Out, int fret) {
    uint32_t r = 0, saved = 0;
    uint64_t f = 0;
    __try {
        __asm {
            mov saved, esp
            mov ecx, n
            mov esi, stack
        push_loop:
            test ecx, ecx
            jz pushed
            dec ecx
            push dword ptr [esi + ecx * 4]
            jmp push_loop
        pushed:
            mov ecx, ecxv
            mov edx, edxv
            call fn
            mov esp, saved
            mov r, eax
            cmp fret, 0
            je no_float
            fstp qword ptr f
        no_float:
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return 0;
    }
    *eaxOut = r;
    *st0Out = f;
    return 1;
}

static void CallFunction(const char* spec) {
    char buf[160];
    strncpy(buf, spec, sizeof buf - 1);
    buf[sizeof buf - 1] = 0;
    char* tok[16];
    int nt = 0;
    for (char* s = strtok(buf, "/"); s && nt < 16; s = strtok(nullptr, "/")) tok[nt++] = s;
    if (nt < 3) return;
    const uint32_t fn = strtoul(tok[0], nullptr, 16);
    const bool fast = tok[1][0] == 'f';
    const char ret = tok[2][0];
    uint32_t words[16];
    int nw = 0, strings = 0;
    memset(g_scratch, 0, sizeof g_scratch);
    for (int i = 3; i < nt && nw < 14; ++i) {
        const char* t = tok[i];
        uint32_t v = 0;
        switch (t[0]) {
        case 'n': v = (uint32_t)strtol(t + 1, nullptr, 10); break;
        case 'x': v = strtoul(t + 1, nullptr, 16); break;
        case 'f': { float x = (float)atof(t + 1); memcpy(&v, &x, 4); break; }
        case 'd': {
            double x = atof(t + 1);
            uint32_t w[2];
            memcpy(w, &x, 8);
            words[nw++] = w[0];
            v = w[1];
            break;
        }
        case 'o': case 'i': {
            char* dot = nullptr;
            const uint32_t g = strtoul(t + 1, &dot, 16);
            uint32_t id = 0;
            void* o = CollectionObject(g, dot && *dot == '.' ? atoi(dot + 1) : 0, true, &id);
            v = t[0] == 'o' ? (uint32_t)(uintptr_t)o : id;
            break;
        }
        case 'g': v = *(uint32_t*)strtoul(t + 1, nullptr, 16); break;
        case 'p': {  // p<global>.<off>[.<off>...]: follows pointers: *(*(global) + off) ...
            char* q = nullptr;
            v = *(uint32_t*)strtoul(t + 1, &q, 16);
            while (v && q && *q == '.') v = *(uint32_t*)(v + strtoul(q + 1, &q, 16));
            break;
        }
        case 'a': v = strtoul(t + 1, nullptr, 16); break;
        case 'b': v = (uint32_t)(uintptr_t)g_scratch[atoi(t + 1) & 3]; break;
        case 's':
            if (strings < 8) {
                strncpy(g_callStrings[strings], t + 1, 63);
                v = (uint32_t)(uintptr_t)g_callStrings[strings++];
            }
            break;
        }
        words[nw++] = v;
    }
    uint32_t ecxv = 0, edxv = 0;
    const uint32_t* stack = words;
    int ns = nw;
    if (fast) {
        if (ns > 0) { ecxv = stack[0]; ++stack; --ns; }
        if (ns > 0) { edxv = stack[0]; ++stack; --ns; }
    }
    uint32_t eax = 0;
    uint64_t st0 = 0;
    if (!CallRaw(fn, ecxv, edxv, stack, ns, &eax, &st0, ret == 'f')) {
        rt3::Log("R %06x fault", fn);
        return;
    }
    if (ret == 'b') eax &= 0xff;
    if (ret == 'v' || ret == 'f') eax = 0;  // (a float result leaves EAX as it was)
    rt3::Log("R %06x %08x %016llx %08x", fn, eax, ret == 'f' ? st0 : 0ull, rt3::Crc32(&g_scratch[0][0], sizeof g_scratch));
}

struct Action;
static bool ObjectCell(const Action& a, float* x, float* y);

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
        if (a.kind == kWaitGame) {  // until a world runs (World_Frame's own test), then the next actions shift
            uint8_t* g = (uint8_t*)Game();
            if (!g || !*(int32_t*)g) return;  // again next time
            rt3::Log("harness: frame %ld, game running", g_frame);
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
        if (a.kind == kShot) {
            Screenshot(w);
            continue;
        }
        if (a.kind == kCallFn) {
            CallFunction(a.text);
            continue;
        }
        if (a.kind == kAutoEnter) {
            g_autoEnter = (int)a.addr;
            rt3::Log("harness: frame %ld, Enter in dialogs %s", g_frame, g_autoEnter ? "on" : "off");
            continue;
        }
        // The in-game camera reads the mouse through DirectInput, not the
        // window messages the script posts: these move it with the game's own
        // calls instead (the view only, the world is untouched).
        if (a.kind == kZoom || a.kind == kLook) {
            uint8_t* cam = g_62be68;
            if (!cam || !Game()) continue;
            float lx = a.x, ly = a.y;
            if (a.objColl && !ObjectCell(a, &lx, &ly)) continue;
            if (a.kind == kZoom) {  // 0x43cc30: steps along the zoom ladder (negative: in)
                rt3::Log("harness: frame %ld, zoom %d", g_frame, (int32_t)a.addr);
                ((void(__fastcall*)(uint8_t*, int, int32_t))0x43cc30)(cam, 0, (int32_t)a.addr);
            } else {  // 0x43d0c0: centres the view on map cell (x, y)
                rt3::Log("harness: frame %ld, look at %.1f %.1f", g_frame, lx, ly);
                ((void(__fastcall*)(uint8_t*, int, float, float))0x43d0c0)(cam, 0, lx, ly);
            }
            continue;
        }
        if (a.kind == kTracks) {  // "T <id> <cell x> <cell y> <owner>" for every track piece
            if (rt3::Collection* t = g_tracks) {
                const int32_t n = ((int32_t(__fastcall*)(rt3::Collection*))0x517cf0)(t);
                rt3::Log("harness: frame %ld, %d track pieces", g_frame, n);
                for (int32_t i = 0; i < n && i < 5000; ++i) {
                    uint32_t id = ((uint32_t(__fastcall*)(rt3::Collection*, int, uint32_t, int))0x518380)(t, 0, i, 0);
                    uint8_t* c = (uint8_t*)((void*(__fastcall*)(rt3::Collection*, int, uint32_t))0x518140)(t, 0, id);
                    // The centre of its four corners (+0x14, x y z each, world units; 0x5c8700 per cell).
                    double x = 0, y = 0;
                    for (int k = 0; k < 4; ++k) {
                        x += *(float*)(c + 0x14 + k * 0xc);
                        y += *(float*)(c + 0x1c + k * 0xc);
                    }
                    x /= 4 * *(const float*)0x5c8700;
                    y /= 4 * *(const float*)0x5c8700;
                    rt3::Log("T %u %.2f %.2f %d", id, x, y, *(int16_t*)(c + 0x22e));
                }
            }
            continue;
        }
        if (a.kind == kCities) {  // "C <id> <cell x> <cell y> <name>" for every city
            if (rt3::Collection* t = g_cities) {
                const int32_t n = ((int32_t(__fastcall*)(rt3::Collection*))0x517cf0)(t);
                for (int32_t i = 0; i < n; ++i) {
                    uint32_t id = ((uint32_t(__fastcall*)(rt3::Collection*, int, uint32_t, int))0x518380)(t, 0, i, 0);
                    uint8_t* c = (uint8_t*)((void*(__fastcall*)(rt3::Collection*, int, uint32_t))0x518140)(t, 0, id);
                    double x = ((double(__fastcall*)(void*))0x455800)(c), y = ((double(__fastcall*)(void*))0x455810)(c);
                    rt3::Log("C %u %.1f %.1f %s", id, x, y, (const char*)c + 0x356);
                }
            }
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
        if (a.kind == kWheel) {  // (the game takes the wheel at the last mouse position)
            rt3::Log("harness: frame %ld, wheel %d", g_frame, (int32_t)a.addr);
            PostMessageA(w, WM_MOUSEWHEEL, (WPARAM)(uint32_t)((int32_t)a.addr * WHEEL_DELTA) << 16, MousePoint(w));
            continue;
        }
        if (a.kind == kKeyDown || a.kind == kKeyUp) {  // a key held over frames (camera keys)
            rt3::Log("harness: frame %ld, key %02x %s", g_frame, a.addr, a.kind == kKeyDown ? "down" : "up");
            UINT scan = MapVirtualKeyA(a.addr, 0);
            if (a.kind == kKeyDown) PostMessageA(w, WM_KEYDOWN, a.addr, 1 | (scan << 16));
            else PostMessageA(w, WM_KEYUP, a.addr, 1 | (scan << 16) | 0xc0000000);
            continue;
        }
        if (a.kind == kKey) {
            rt3::Log("harness: frame %ld, key %02x", g_frame, a.addr);
            UINT scan = MapVirtualKeyA(a.addr, 0);
            PostMessageA(w, WM_KEYDOWN, a.addr, 1 | (scan << 16));
            PostMessageA(w, WM_KEYUP, a.addr, 1 | (scan << 16) | 0xc0000000);
            continue;
        }
        ActionKind kind = a.kind;
        float mx = a.x, my = a.y;
        if (kind == kCell) {  // a mouse action (move / left down / left up) over map cell (x, y)
            float cx = a.x, cy = a.y;
            if (a.objColl && !ObjectCell(a, &cx, &cy)) continue;
            if (!Game() || !ScreenOfCell(cx, cy, &mx, &my)) {
                rt3::Log("harness: frame %ld, cell %.1f %.1f not in view", g_frame, a.x, a.y);
                continue;
            }
            kind = (ActionKind)a.addr;
            rt3::Log("harness: frame %ld, cell %.1f %.1f at %.4f %.4f", g_frame, a.x, a.y, mx, my);
        }
        g_mouseX = mx;
        g_mouseY = my;
        LPARAM pt = MousePoint(w);
        static const UINT msg[] = { 0, WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP };
        static WPARAM buttons;  // MK_ bits held
        if (kind == kLeftDown) buttons |= MK_LBUTTON;
        if (kind == kLeftUp) buttons &= ~(WPARAM)MK_LBUTTON;
        if (kind == kRightDown) buttons |= MK_RBUTTON;
        if (kind == kRightUp) buttons &= ~(WPARAM)MK_RBUTTON;
        rt3::Log("harness: frame %ld, mouse %d at %d,%d", g_frame, (int)kind, LOWORD(pt), HIWORD(pt));
        if (kind != kMove) PostMessageA(w, WM_MOUSEMOVE, buttons, pt);
        PostMessageA(w, msg[kind], buttons, pt);
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
        } else if (!strncmp(p, "shot", 4)) {
            p += 4;
            a.kind = kShot;
            done = true;
        } else if (!strncmp(p, "cities", 6)) {
            p += 6;
            a.kind = kCities;
            done = true;
        } else if (!strncmp(p, "fn:", 3)) {  // a call (CallFunction), its spec up to the next ','
            p += 3;
            a.kind = kCallFn;
            int n = 0;
            while (*p && *p != ',' && n < (int)sizeof a.text - 1) a.text[n++] = *p++;
            a.text[n] = 0;
            done = true;
        } else if (!strncmp(p, "waitgame", 8)) {
            p += 8;
            a.kind = kWaitGame;
            done = true;
        } else if (!strncmp(p, "ae:", 3)) {
            p += 3;
            a.kind = kAutoEnter;
            a.addr = strtoul(p, &p, 10);
            done = true;
        } else if (!strncmp(p, "tracks", 6)) {
            p += 6;
            a.kind = kTracks;
            done = true;
        } else if (!strncmp(p, "look:", 5)) {
            p += 5;
            a.kind = kLook;
            a.x = (float)strtod(p, &p);
            if (*p == ':') ++p;
            a.y = (float)strtod(p, &p);
            done = true;
        } else if (!strncmp(p, "ol:", 3) || !strncmp(p, "ocm:", 4) || !strncmp(p, "ocd:", 4) || !strncmp(p, "ocu:", 4)) {
            // the same by object: <coll>.<i>, the i-th object of the collection global <coll>
            if (p[1] == 'l') {
                a.kind = kLook;
                p += 3;
            } else {
                a.kind = kCell;
                a.addr = p[2] == 'm' ? kMove : p[2] == 'd' ? kLeftDown : kLeftUp;
                p += 4;
            }
            a.objColl = strtoul(p, &p, 16);
            if (*p == '.') a.objIndex = strtol(p + 1, &p, 10);
            done = true;
        } else if (!strncmp(p, "cm:", 3) || !strncmp(p, "cd:", 3) || !strncmp(p, "cu:", 3)) {
            a.addr = p[1] == 'm' ? kMove : p[1] == 'd' ? kLeftDown : kLeftUp;  // what to do at the cell
            p += 3;
            a.kind = kCell;
            a.x = (float)strtod(p, &p);
            if (*p == ':') ++p;
            a.y = (float)strtod(p, &p);
            done = true;
        } else if (!strncmp(p, "zoom:", 5)) {
            p += 5;
            a.kind = kZoom;
            a.addr = (uint32_t)strtol(p, &p, 10);
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
            } else if (!strncmp(p, "wh:", 3)) {
                p += 3;
                a.kind = kWheel;
                a.addr = (uint32_t)strtol(p, &p, 10);
                g_actions[g_numActions++] = a;
                if (*p == ',') ++p;
                continue;
            } else if (!strncmp(p, "kd:", 3) || !strncmp(p, "ku:", 3)) {
                a.kind = p[1] == 'd' ? kKeyDown : kKeyUp;
                p += 3;
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
    if (g_autoEnter && (g_state >= 1 || (Driven() && Game())) && (n == 3 || n == 30 || n == 100 || n == 300 || n == 1000 || n == 3000)) {
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
        else if (!strncmp(tok, "sound=", 6)) g_soundOn = atoi(tok + 6);
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
