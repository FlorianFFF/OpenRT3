// Input.cpp - RT3.exe 0x54e000-0x550000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include "core/imports.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

struct HWND__;

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
::HWND__* __fastcall Engine_Window(void* e);
void __fastcall PumpMessages();
int32_t ClockMs();
void __fastcall Input_PushEvent(uint8_t* in, int, const uint32_t* ev);
uint32_t __fastcall Engine_IsFullscreen(void* e);
void __fastcall Keyboard_SetScrollLock(int32_t on);
float __fastcall Fade_Step_Impl();

void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                 uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex,
                                 uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16,
                                 uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20);

void __fastcall Engine_QueueType5(void* e, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Engine_QueueType6(void* e);

struct InputEvent {
    uint32_t v[12];
};

void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12);

void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
int32_t __fastcall Input_OnMouseMessage(uint8_t* in, int, uint32_t hwnd, uint32_t msg, uint32_t wparam, uint32_t lparam);
int32_t Global6d10b0Set();
void __fastcall Engine_KeyDown(uint8_t* e, int, uint8_t k);
void __fastcall Engine_KeyUp(uint8_t* e, int, uint8_t k);
int32_t __fastcall Input_OnKey(uint8_t* in, int, uint32_t hwnd, uint32_t msg, uint32_t vk, uint32_t lparam);
int32_t __fastcall Input_PostChar(uint8_t* in, int, uint32_t ch);
void __fastcall Input_PostEvent(uint8_t* in, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Input_ClearKeys(uint8_t* in);
void __fastcall Input_UpdateCtrl(uint8_t* in);
void __fastcall Engine_PlaceWindow(uint8_t* e, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Input_PostEvent(uint8_t* in, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);
int32_t __stdcall Game_WndProc(uint32_t hwnd, uint32_t msg, uint32_t wparam, uint32_t lparam);
RT3_NOINLINE void __fastcall PumpMessages();
RT3_NOINLINE int32_t __fastcall Engine_IsForeground();
RT3_NOINLINE void __fastcall Keyboard_SetScrollLock(int32_t on);
void __fastcall Keyboard_SaveScrollLock();
void __fastcall Keyboard_RestoreScrollLock();
uint8_t* __fastcall Input_Construct(uint8_t* in);
RT3_NOINLINE void __fastcall Nop54e780();
void __fastcall Input_RemoveHandler(uint8_t* in, int, uint32_t h);
uint32_t __fastcall Input_Key(uint8_t* in, int, int32_t k);
void __fastcall Input_Block(uint8_t* in);
void __fastcall Input_Unblock(uint8_t* in);
void __fastcall Input_ClearKeys(uint8_t* in);
void __fastcall Input_UpdateCtrl(uint8_t* in);
RT3_NOINLINE void __fastcall Input_DetectDoubleClick(uint8_t* in, int, const uint32_t* ev);
RT3_NOINLINE void __fastcall Input_PushEvent(uint8_t* in, int, const uint32_t* ev);
InputEvent* __fastcall Input_PopEvent(uint8_t* q, int, InputEvent* out);
int32_t __fastcall Input_PostChar(uint8_t* in, int, uint32_t ch);
RT3_NOINLINE void __fastcall Input_OnMouse(uint8_t* in, int, float x, float y, float wheel, int32_t left, int32_t middle, int32_t right);
int32_t __fastcall Input_OnMouseMessage(uint8_t* in, int, uint32_t, uint32_t msg, uint32_t wparam, uint32_t lparam);
int32_t __fastcall Input_OnKey(uint8_t* in, int, uint32_t, uint32_t msg, uint32_t vk, uint32_t lparam);
InputEvent* __fastcall Input_NextEvent(uint8_t* q, int, InputEvent* out);
RT3_NOINLINE int32_t __fastcall Cursor_InWindow();
RT3_NOINLINE void __fastcall Input_PostEvent(uint8_t* in, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
float __fastcall Fade_Step_Impl();
void Fade_Step();
void __fastcall Fade_StartSlow();
void __fastcall Fade_StartFast();
void __fastcall Fade_StartVerySlow();
void __fastcall Fade_Save();
void __fastcall Fade_Restore();
void __stdcall Fade_Set(uint32_t v);
RT3_NOINLINE void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
RT3_NOINLINE void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
RT3_NOINLINE void __fastcall Fx_QueueFadedQuad(uint32_t a, uint32_t b, uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3, uint32_t s4, uint32_t clip, uint32_t s6, uint32_t, uint32_t s8, uint32_t s9, uint32_t s10, uint32_t s11);
RT3_NOINLINE void __fastcall Engine_QueueGradientQuad(uint32_t c0, uint32_t c1, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t c2, uint32_t c3, int32_t clip, uint32_t cx0, uint32_t cy0, uint32_t cx1, uint32_t cy1);

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline uint8_t* Engine() { return g_engine; }

static inline void KeyboardState(uint8_t* buf) { imp::GetKeyboardState(buf); }

namespace {
void InputQueue(uint8_t* in, const uint32_t* ev) {
    if (F<int32_t>(in, 0xa88) == 0x32) F<int32_t>(in, 0xa88) = 0x31;
    memcpy(in + 0x128 + F<int32_t>(in, 0xa88) * 0x30, ev, 0x30);
    ++F<int32_t>(in, 0xa88);
    Input_DetectDoubleClick(in, 0, ev);
}
}

namespace {
// Button capture follows the buttons in a window unless a button of the
// global input (0x6d4018) is already down or the game is full screen.
inline bool InputMayCapture(uint8_t* e) {
    uint8_t* g = g_input;
    return e && g && !(F<uint32_t>(g, 0xa8c) & 0x2c00) && !Engine_IsFullscreen(e);
}
}

static inline void* Engine_103() { return hook::As<void*>(g_engine); }

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

static inline double D(uint32_t a) { return *(const double*)rt3::Rdata(a); }

static inline double Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static inline const int32_t* ScreenMode() {
    uint8_t* e = g_engine;
    return (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
}

// An untextured, white-tinted screen rectangle at the next UI depth.
static void Ui_QueueRect(float x1, float y1, float x2, float y2, uint32_t color) {
    float depth = Fade_Step_Impl();
    Engine_QueueSprite(hook::As<void*>(g_engine), 0, FBits(x1), FBits(y1), FBits(x2), FBits(y2), FBits(depth), color, 0, 0, 0,
                       0x3f800000, 0x3f800000, 0x3f800000);
}

// 0x54e3a0. The game window procedure: moves (WM_MOVE) and focus changes
// reach the engine and input 0x6d4018; WM_SIZE is swallowed; WM_CLOSE posts
// input event 0xcd; key messages update the key table and input (scroll lock
// is ignored while 0xd93840); WM_COMMAND posts the character; mouse messages
// go to the input; maximise toggles full screen (unless 0x6d10b0) and the
// screen saver / monitor power commands are dropped. Everything else goes to
// DefWindowProc.
int32_t __stdcall Game_WndProc(uint32_t hwnd, uint32_t msg, uint32_t wparam, uint32_t lparam) {
    uint8_t* in = g_input;
    switch (msg) {
    case 3:
        if (uint8_t* e = g_engine) Engine_PlaceWindow(e, 0, lparam & 0xffff, lparam >> 16);
        return 0;
    case 5:
        return 0;
    case 7:
        if (!in) break;
        Input_UpdateCtrl(in);
        break;
    case 8:
        if (!in) break;
        Input_ClearKeys(in);
        break;
    case 0x10:
        if (in) Input_PostEvent(in, 0, 0xcd, 0, 0, 0, 0);
        return 0;
    case 0x100:
    case 0x101:
        if (g_d93840 && wparam == 0x91) return 0;
        if (msg == 0x100) Engine_KeyDown(g_engine, 0, (uint8_t)wparam);
        else Engine_KeyUp(g_engine, 0, (uint8_t)wparam);
        if (uint8_t* i = g_input) Input_OnKey(i, 0, hwnd, msg, wparam, lparam);
        return 0;
    case 0x111:
        if (in && Input_PostChar(in, 0, wparam & 0xffff)) return 0;
        break;
    case 0x112:
        if (wparam == 0xf030 || wparam == 0xf032) {
            if (!Global6d10b0Set())
                if (uint8_t* e = g_engine) Engine_ResetDisplay(e, 0, e[0x114226] == 0, -1);
            return 0;
        }
        if (wparam == 0xf140 || wparam == 0xf170) return 0;
        break;
    default:
        if ((msg >= 0x200 && msg <= 0x208) || msg == 0x20a) {
            if (in && !Input_OnMouseMessage(in, 0, hwnd, msg, wparam, lparam)) return 0;
        }
        break;
    }
    return imp::DefWindowProcA(hwnd, msg, wparam, lparam);
}
RT3_REPLACE(0x54e3a0, Game_WndProc);

// 0x54e5d0. Dispatches all pending window messages.
RT3_NOINLINE void __fastcall PumpMessages() {
    uint8_t msg[0x1c];
    auto peek = imp::PeekMessageA;
    if (!peek(msg, nullptr, 0, 0, 1)) return;
    auto translate = imp::TranslateMessage;
    auto dispatch = imp::DispatchMessageA;
    do {
        translate(msg);
        dispatch(msg);
    } while (peek(msg, nullptr, 0, 0, 1));
}
RT3_REPLACE(0x54e5d0, PumpMessages);

// 0x54e630. Whether the game window is in the foreground.
RT3_NOINLINE int32_t __fastcall Engine_IsForeground() {
    if (!Engine()) return 0;
    ::HWND__* w = Engine_Window(Engine());
    return w == ((::HWND__*(__stdcall*)())imp::GetForegroundWindow)() ? 1 : 0;
}
RT3_REPLACE(0x54e630, Engine_IsForeground);

// 0x54e660. Sets the scroll-lock toggle (by simulating a key press).
RT3_NOINLINE void __fastcall Keyboard_SetScrollLock(int32_t on) {
    uint8_t keys[0x100];
    KeyboardState(keys);
    if ((int32_t)(keys[0x91] & 1) == on) return;
    g_d93840++;
    auto keybd = imp::keybd_event;
    keybd(0x91, 0x45, 1, 0);
    keybd(0x91, 0x45, 3, 0);
    PumpMessages();
    g_d93840--;
}
RT3_REPLACE(0x54e660, Keyboard_SetScrollLock);

// 0x54e6d0. Remembers the scroll-lock toggle (0xd93844).
void __fastcall Keyboard_SaveScrollLock() {
    uint8_t keys[0x100];
    KeyboardState(keys);
    g_d93844 = keys[0x91] & 1;
}
RT3_REPLACE(0x54e6d0, Keyboard_SaveScrollLock);

void __fastcall Keyboard_RestoreScrollLock() { Keyboard_SetScrollLock(hook::As<int32_t>(g_d93844)); }
RT3_REPLACE(0x54e700, Keyboard_RestoreScrollLock);

// 0x54e710. Input object constructor: key table, buttons swapped (SM_SWAPBUTTON),
// cursor at the centre, double-click time.
uint8_t* __fastcall Input_Construct(uint8_t* in) {
    for (int k = 0; k < 0x40; ++k) F<int32_t>(in, k * 4) = 0;
    F<int32_t>(in, 0xa88) = 0;
    F<int32_t>(in, 0xa8c) = 0;
    F<int32_t>(in, 0xaa4) = imp::GetSystemMetrics(0x17);
    F<uint32_t>(in, 0xa94) = 0x3f000000;
    F<uint32_t>(in, 0xa98) = 0x3f000000;
    F<int32_t>(in, 0xa9c) = 0;
    F<int32_t>(in, 0xa90) = 0;
    F<int32_t>(in, 0x100) = 0;
    F<uint32_t>(in, 0xaa0) = imp::GetDoubleClickTime();
    for (int k = 0; k < 9; ++k) F<int32_t>(in, 0x104 + k * 4) = 0;
    return in;
}
RT3_REPLACE(0x54e710, Input_Construct);

RT3_NOINLINE void __fastcall Nop54e780() { __asm nop }
RT3_REPLACE(0x54e780, Nop54e780);

// 0x54e790. Clears handler h from the input bindings (+0x128, 0x30 bytes, count +0xa88).
void __fastcall Input_RemoveHandler(uint8_t* in, int, uint32_t h) {
    for (int32_t i = 0; i < F<int32_t>(in, 0xa88); ++i)
        if (F<uint32_t>(in, 0x128 + i * 0x30) == h) F<uint32_t>(in, 0x128 + i * 0x30) = 0;
}
RT3_REPLACE(0x54e790, Input_RemoveHandler);

uint32_t __fastcall Input_Key(uint8_t* in, int, int32_t k) { return in[k]; }
RT3_REPLACE(0x54e7d0, Input_Key);

void __fastcall Input_Block(uint8_t* in) { F<int32_t>(in, 0xa90)++; }
RT3_REPLACE(0x54e7e0, Input_Block);

void __fastcall Input_Unblock(uint8_t* in) {
    if (--F<int32_t>(in, 0xa90) < 0) F<int32_t>(in, 0xa90) = 0;
}
RT3_REPLACE(0x54e7f0, Input_Unblock);

// 0x54e810. Releases all keys and the modifier/button bits 0x27.
RT3_NOINLINE void __fastcall Input_ClearKeys(uint8_t* in) {
    for (int k = 0; k < 0x40; ++k) F<int32_t>(in, k * 4) = 0;
    F<uint32_t>(in, 0xa8c) &= 0xffffffd8;
}
RT3_REPLACE(0x54e810, Input_ClearKeys);

// 0x54e830. Refreshes the ctrl bit (4) of +0xa8c from the keyboard.
RT3_NOINLINE void __fastcall Input_UpdateCtrl(uint8_t* in) {
    uint8_t keys[0x100];
    KeyboardState(keys);
    if (keys[0x11] & 0x80) F<uint32_t>(in, 0xa8c) |= 4;
    else F<uint32_t>(in, 0xa8c) &= ~4u;
}
RT3_REPLACE(0x54e830, Input_UpdateCtrl);

// 0x54e880. Turns a second button-down (types 6 / 10 / 8 for buttons 0-2)
// close to the last one (distance in screen metric units below 0x5c8554)
// within the double-click time (+0xaa0) into a double-click event (0x1e /
// 0x22 / 0x20); otherwise remembers where and when (+0x104 + 12 b).
RT3_NOINLINE void __fastcall Input_DetectDoubleClick(uint8_t* in, int, const uint32_t* ev) {
    int32_t b;
    switch (ev[0]) {
    case 6: b = 0; break;
    case 8: b = 2; break;
    case 10: b = 1; break;
    default: return;
    }
    uint8_t* e = g_engine;
    if (!e) return;
    const int32_t* metric = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    double dx = fabs((double)*(const float*)&ev[9] - F<float>(in, 0x104 + b * 0xc)) * metric[0];
    double dy = fabs((double)*(const float*)&ev[10] - F<float>(in, 0x108 + b * 0xc)) * metric[1];
    double dist = sqrt(dy * dy + dx * dx + *(const double*)rt3::Rdata(0x5c85b8));
    if (dist < *(const float*)rt3::Rdata(0x5c8554) && F<int32_t>(in, 0x10c + b * 0xc) && F<int32_t>(in, 0x10c + b * 0xc) < ClockMs() + F<int32_t>(in, 0xaa0)) {
        uint32_t dbl[12];
        memcpy(dbl, ev, sizeof dbl);
        static const uint32_t kinds[] = { 0x1e, 0x22, 0x20 };
        dbl[0] = kinds[b];
        Input_PushEvent(in, 0, dbl);
        F<int32_t>(in, 0x10c + b * 0xc) = 0;
        return;
    }
    F<uint32_t>(in, 0x104 + b * 0xc) = ev[9];
    F<uint32_t>(in, 0x108 + b * 0xc) = ev[10];
    F<int32_t>(in, 0x10c + b * 0xc) = ClockMs();
}
RT3_REPLACE(0x54e880, Input_DetectDoubleClick);

// 0x54e9c0. Queues input event ev (0x30 bytes at +0x128, 50 at most: a full
// queue overwrites the last), then checks it for a double click.
RT3_NOINLINE void __fastcall Input_PushEvent(uint8_t* in, int, const uint32_t* ev) {
    if (F<int32_t>(in, 0xa88) == 0x32) F<int32_t>(in, 0xa88) = 0x31;
    memcpy(in + 0x128 + F<int32_t>(in, 0xa88) * 0x30, ev, 0x30);
    ++F<int32_t>(in, 0xa88);
    Input_DetectDoubleClick(in, 0, ev);
}
RT3_REPLACE(0x54e9c0, Input_PushEvent);

// 0x54ea20. Pops the oldest input event (queue +0x128, 0x30 bytes each, count
// +0xa88; nothing while locked, +0xa90). Mouse moves (type 2) are coalesced unless
// +0x100: later queued moves replace the popped one.
InputEvent* __fastcall Input_PopEvent(uint8_t* q, int, InputEvent* out) {
    InputEvent e;
    if (!F<int32_t>(q, 0xa88) || F<int32_t>(q, 0xa90)) {
        memset(&e, 0, sizeof e);
    } else {
        memcpy(&e, q + 0x128, sizeof e);
        int32_t n = F<int32_t>(q, 0xa88);
        if (n > 1) memmove(q + 0x128, q + 0x158, (n * 3 - 3) << 4);
        --F<int32_t>(q, 0xa88);
        if (e.v[0] == 2 && !F<int32_t>(q, 0x100)) {
            while (F<int32_t>(q, 0xa88) && !F<int32_t>(q, 0xa90)) {
                if (F<uint32_t>(q, 0x128) != 2) break;
                InputEvent next;
                memcpy(&e, Input_PopEvent(q, 0, &next), sizeof e);
            }
        }
    }
    *out = e;
    return out;
}
RT3_REPLACE(0x54ea20, Input_PopEvent);

// 0x54eb10. Queues a character event {1, 0, ch}; returns 1.
RT3_NOINLINE int32_t __fastcall Input_PostChar(uint8_t* in, int, uint32_t ch) {
    uint32_t ev[12] = { 1, 0, ch };
    if (F<int32_t>(in, 0xa88) == 0x32) F<int32_t>(in, 0xa88) = 0x31;
    memcpy(in + 0x128 + F<int32_t>(in, 0xa88) * 0x30, ev, 0x30);
    ++F<int32_t>(in, 0xa88);
    Input_DetectDoubleClick(in, 0, ev);
    return 1;
}
RT3_REPLACE(0x54eb10, Input_PostChar);

 // namespace

// 0x54eb90. A mouse update at (x, y) with wheel delta `wheel` and buttons
// left / middle / right (left and right swapped with +0xaa4): keeps the
// position (+0xa94 / +0xa98) and wheel sum (+0xa9c) and queues, in order, a
// move (2, only when it moved since 0x625458 / 0x625454), left down / up (6
// / 7, state bit 0x400), middle (10 / 11, 0x2000), right (8 / 9, 0x800) and
// wheel (5, ticks = wheel x 1.01 x 4.1666) event, each with the state flags.
RT3_NOINLINE void __fastcall Input_OnMouse(uint8_t* in, int, float x, float y, float wheel, int32_t left, int32_t middle, int32_t right) {
    if (F<int32_t>(in, 0xaa4)) {
        int32_t t = left;
        left = right;
        right = t;
    }
    F<float>(in, 0xa98) = y;
    F<float>(in, 0xa9c) = (float)((double)wheel + F<float>(in, 0xa9c));
    F<float>(in, 0xa94) = x;
    uint32_t type = *(uint32_t*)&x;
    for (int32_t i = 0; i < 8; ++i) {
        int32_t ticks = 0;
        uint32_t& st = F<uint32_t>(in, 0xa8c);
        switch (i) {
        case 0:
            if (x == g_625458 && y == g_625454) continue;
            g_625458 = x;
            g_625454 = y;
            type = 2;
            break;
        case 1:
            if (!left || (st & 0x400)) continue;
            st |= 0x400;
            type = 6;
            break;
        case 2:
            if (left || !(st & 0x400)) continue;
            st &= ~0x400u;
            type = 7;
            break;
        case 3:
            if (!middle || (st & 0x2000)) continue;
            st |= 0x2000;
            type = 0xa;
            break;
        case 4:
            if (middle || !(st & 0x2000)) continue;
            st &= ~0x2000u;
            type = 0xb;
            break;
        case 5:
            if (!right || (st & 0x800)) continue;
            st |= 0x800;
            type = 8;
            break;
        case 6:
            if (right || !(st & 0x800)) continue;
            st &= ~0x800u;
            type = 9;
            break;
        case 7:
            if (wheel == *(const float*)rt3::Rdata(0x5c8598)) continue;
            type = 5;
            ticks = Ftol((double)wheel * *(const double*)rt3::Rdata(0x5c9b10) * *(const double*)rt3::Rdata(0x5ddb70));
            break;
        }
        uint32_t ev[12] = {};
        ev[9] = *(uint32_t*)&x;
        ev[8] = F<uint32_t>(in, 0xa8c);
        ev[0] = type;
        ev[10] = *(uint32_t*)&y;
        ev[2] = (uint32_t)ticks;
        InputQueue(in, ev);
    }
}
RT3_REPLACE(0x54eb90, Input_OnMouse);

 // namespace

// 0x54ee50. A window mouse message (WM_MOUSEMOVE .. WM_MOUSEWHEEL; 0 for the
// others, double clicks included): keeps the cursor (in screen units,
// clamped to 0..1, +0xa94 / +0xa98; the real cursor position when full
// screen, the last one for the wheel) and queues a move (2, skipped when
// unchanged since 0x625460 / 0x62545c), left / right / middle down / up (6 /
// 7, 8 / 9, 10 / 11, with button capture and state bits 0x400 / 0x800 /
// 0x2000) or wheel (5, ticks of 120, sum +0xa9c x 0.24) event; 1 then.
RT3_NOINLINE int32_t __fastcall Input_OnMouseMessage(uint8_t* in, int, uint32_t, uint32_t msg, uint32_t wparam, uint32_t lparam) {
    if (msg == 0x200 && wparam == g_625460 && lparam == g_62545c) return 1;
    uint32_t ev[12] = {};
    int32_t x = (int16_t)lparam, y = (int16_t)(lparam >> 16);
    if (msg == 0x20a) {
        x = g_d9384c;
        y = g_d93848;
    }
    uint8_t* e = g_engine;
    if (e && Engine_IsFullscreen(e)) {
        int32_t pt[2];
        imp::GetCursorPos(pt);
        x = pt[0];
        y = pt[1];
    }
    e = g_engine;
    g_d9384c = x;
    g_d93848 = y;
    const int32_t* metric = e ? (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15) : nullptr;
    float fx = (float)((double)x / (e ? metric[0] : 1));
    float fy = (float)((double)y / (e ? metric[1] : 1));
    if (fx < *(const float*)rt3::Rdata(0x5c8598))
        fx = 0.0f;
    else if (fx > *(const float*)rt3::Rdata(0x5c85b4))
        fx = 1.0f;
    if (fy < *(const float*)rt3::Rdata(0x5c8598))
        fy = 0.0f;
    else if (fy > *(const float*)rt3::Rdata(0x5c85b4))
        fy = 1.0f;
    F<float>(in, 0xa94) = fx;
    F<float>(in, 0xa98) = fy;
    ev[9] = *(uint32_t*)&fx;  // the scaled position is kept in the event itself
    ev[10] = *(uint32_t*)&fy;
    uint32_t& st = F<uint32_t>(in, 0xa8c);
    switch (msg) {
    case 0x200:
        ev[0] = 2;
        g_625460 = wparam;
        g_62545c = lparam;
        break;
    case 0x201:
    case 0x204:
    case 0x207: {
        ev[0] = msg == 0x201 ? 6 : msg == 0x204 ? 8 : 0xa;
        if (InputMayCapture(g_engine))
            imp::SetCapture((void*)Engine_Window(hook::As<void*>(g_engine)));
        st |= msg == 0x201 ? 0x400 : msg == 0x204 ? 0x800 : 0x2000;
        break;
    }
    case 0x202:
    case 0x205:
    case 0x208:
        st &= ~(msg == 0x202 ? 0x400u : msg == 0x205 ? 0x800u : 0x2000u);
        ev[0] = msg == 0x202 ? 7 : msg == 0x205 ? 9 : 0xb;
        if (InputMayCapture(g_engine)) imp::ReleaseCapture();
        break;
    case 0x20a: {
        int32_t ticks = (int16_t)(wparam >> 16) / 120;
        ev[0] = 5;
        ev[2] = (uint32_t)ticks;
        F<float>(in, 0xa9c) = (float)((double)ticks * *(const double*)rt3::Rdata(0x5c9c80) + F<float>(in, 0xa9c));
        break;
    }
    default:
        return 0;
    }
    ev[8] |= F<uint32_t>(in, 0xa8c);
    Input_PushEvent(in, 0, ev);
    return 1;
}
RT3_REPLACE(0x54ee50, Input_OnMouseMessage);

// 0x54f290. A WM_KEYDOWN / WM_KEYUP message: queues a key down (3) / up (4)
// event with the virtual key and its character (ToAscii), keeps the key
// table (+0..+0xff) and the Ctrl / Shift / Alt bits (4 / 2, 1 / 0x20) by scan
// code; 1 when handled.
RT3_NOINLINE int32_t __fastcall Input_OnKey(uint8_t* in, int, uint32_t, uint32_t msg, uint32_t vk, uint32_t lparam) {
    uint32_t ev[12] = {};
    bool down;
    if (msg == 0x100)
        down = true;
    else if (msg == 0x101)
        down = false;
    else
        return 0;
    uint8_t state[0x100];
    uint16_t ch[2];
    ev[0] = down ? 3 : 4;
    ev[2] = vk;
    if (imp::GetKeyboardState(state) &&
        imp::ToAscii(vk, (lparam >> 16) & 0xff, state, ch, 0) == 1)
        ev[3] = ch[0] & 0xff;
    if ((int32_t)vk >= 0 && (int32_t)vk < 0x100) in[vk] = down ? 1 : 0;
    uint32_t scan = (lparam >> 16) & 0xff;
    uint32_t bit = scan == 0x1d ? 4 : scan == 0x2a ? 2 : scan == 0x36 ? 1 : scan == 0x38 ? 0x20 : 0;
    if (bit) {
        if (down)
            F<uint32_t>(in, 0xa8c) |= bit;
        else
            F<uint32_t>(in, 0xa8c) &= ~bit;
    }
    ev[8] |= F<uint32_t>(in, 0xa8c);
    Input_PushEvent(in, 0, ev);
    return 1;
}
RT3_REPLACE(0x54f290, Input_OnKey);

// 0x54f4d0. Next input event that passes the filter 0x6d4008 (if any), popping
// even while the queue is locked (+0xa90 is lifted meanwhile).
InputEvent* __fastcall Input_NextEvent(uint8_t* q, int, InputEvent* out) {
    int32_t locked = F<int32_t>(q, 0xa90);
    F<int32_t>(q, 0xa90) = 0;
    InputEvent e;
    for (;;) {
        InputEvent tmp;
        e = *Input_PopEvent(q, 0, &tmp);
        void* filter = g_6d4008;
        if (!filter || ((int32_t(__fastcall*)(InputEvent*))filter)(&e)) break;
    }
    F<int32_t>(q, 0xa90) = locked;
    *out = e;
    return out;
}
RT3_REPLACE(0x54f4d0, Input_NextEvent);

// 0x54f540. Whether the cursor is inside the client area (always when fullscreen).
RT3_NOINLINE int32_t __fastcall Cursor_InWindow() {
    if (Engine()[0x114226]) return 1;
    int32_t pt[2];
    imp::GetCursorPos(pt);
    ::HWND__* w = Engine_Window(Engine());
    ((int32_t(__stdcall*)(::HWND__*, int32_t*))imp::ScreenToClient)(w, pt);
    const int32_t* mode = (const int32_t*)(F<uint8_t*>(Engine(), 0x34) + F<int32_t>(Engine(), 0x11421e) * 0x15);
    if (pt[0] < 0 || pt[0] >= mode[0]) return 0;
    if (pt[1] < 0 || pt[1] >= mode[1]) return 0;
    return 1;
}
RT3_REPLACE(0x54f540, Cursor_InWindow);

// 0x54f5b0. Queues an input event {type, a, b, c, d} (as Input_PushEvent)
// and checks it for a double click.
RT3_NOINLINE void __fastcall Input_PostEvent(uint8_t* in, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint32_t ev[12] = { type, a, b, c, d };
    if (F<int32_t>(in, 0xa88) == 0x32) F<int32_t>(in, 0xa88) = 0x31;
    memcpy(in + 0x128 + F<int32_t>(in, 0xa88) * 0x30, ev, 0x30);
    ++F<int32_t>(in, 0xa88);
    Input_DetectDoubleClick(in, 0, ev);
}
RT3_REPLACE(0x54f5b0, Input_PostEvent);

// 0x54f640. Steps the fade value 0xd93850 down by 0x5ddb80; returns it, or
// 0x5ddb78 once it is negative (or above 0x5c85b8 when 0x625464 is unset).
RT3_NOINLINE float __fastcall Fade_Step_Impl() {
    float& g = hook::As<float>(g_d93850);
    double d = (double)g - *(const double*)rt3::Rdata(0x5ddb80);
    g = (float)d;
    if (!(d >= *(const float*)rt3::Rdata(0x5c8598))) return *(const float*)rt3::Rdata(0x5ddb78);
    if ((double)g <= *(const double*)rt3::Rdata(0x5c85b8)) return g;
    if (*(const float*)0x625464 == *(const float*)rt3::Rdata(0x5c9b08)) return *(const float*)rt3::Rdata(0x5ddb78);
    return g;
}

// 0x54fd20 passes ECX through to the next call.
__declspec(naked) void Fade_Step() { RT3_KEEP_ECX_EDX(Fade_Step_Impl, 0) }
RT3_REPLACE(0x54f640, Fade_Step);

void __fastcall Fade_StartSlow() { g_d93850 = 0x3b30f27c; }
RT3_REPLACE(0x54f6a0, Fade_StartSlow);

void __fastcall Fade_StartFast() { g_d93850 = 0x3baa64c3; }
RT3_REPLACE(0x54f6b0, Fade_StartFast);

// Fade value 0xd93850 and its saved copy 0x625464 (-1 = none).
void __fastcall Fade_StartVerySlow() { g_d93850 = 0x3951b717; }
RT3_REPLACE(0x54f6c0, Fade_StartVerySlow);

void __fastcall Fade_Save() { g_625464 = g_d93850; }
RT3_REPLACE(0x54f6d0, Fade_Save);

void __fastcall Fade_Restore() {
    g_d93850 = g_625464;
    g_625464 = 0xbf800000;
}
RT3_REPLACE(0x54f6e0, Fade_Restore);

void __stdcall Fade_Set(uint32_t v) { g_d93850 = v; }
RT3_REPLACE(0x54f700, Fade_Set);

// 0x54f710. Fills screen rectangle (x, y, w, h) (fractions of the screen) with
// `color`. With `snap` the size is snapped to whole pixels; the left-over part of
// a pixel on the right (flags 2) / bottom (flags 1) either widens the box by a
// pixel (more than 0.9 of one) or becomes a 1.01-pixel strip with the alpha scaled
// by the coverage (at least 0.1).
RT3_NOINLINE void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags) {
    if (snap) {
        const int32_t* mode = ScreenMode();
        int32_t width = mode[0];
        float fw = (float)width;
        double wd = (double)width;
        int32_t px = Ftol((double)fw * w);
        float wq = (float)(((double)px + D(0x5c8678)) / wd);
        int32_t height = mode[1];
        double hd = (double)height;
        int32_t py = Ftol((double)h * hd);
        float hq = (float)(((double)py + D(0x5c8678)) / height);
        float dw = (float)(((double)w - wq) * fw);
        float dh = (float)(((double)h - hq) * hd);
        if (flags & 2) {
            if (dw > D(0x5c8888)) {
                wq = (float)(D(0x5c8770) / wd + wq);
            } else if (!(dw < D(0x5c86b8))) {
                float xa = (float)((double)x + wq);
                uint32_t c = (uint32_t)Ftol((double)(color >> 24) * dw) << 24 | (color & 0xffffff);
                Ui_QueueRect(xa, y, (float)(D(0x5c9b10) / width + xa), (float)((double)y + hq), c);
            }
        }
        if (flags & 1) {
            if (dh > D(0x5c8888)) {
                hq = (float)(D(0x5c8770) / (double)ScreenMode()[1] + hq);
            } else if (!(dh < D(0x5c86b8))) {
                float ya = (float)((double)y + hq);
                int32_t height2 = ScreenMode()[1];
                uint32_t c = (uint32_t)Ftol((double)(color >> 24) * dh) << 24 | (color & 0xffffff);
                Ui_QueueRect(x, ya, (float)((double)x + wq), (float)(D(0x5c9b10) / height2 + ya), c);
            }
        }
        w = wq;
        h = hq;
    }
    Ui_QueueRect(x, y, (float)((double)x + w), (float)((double)y + h), color);
}
RT3_REPLACE(0x54f710, Ui_FillRect);

// 0x54f9f0. Draws a rectangle outline `thick` pixels (+0.02) wide: top, bottom
// (nudged down 0.05 pixel), left and right edges; nearly opaque colours (alpha
// above 0xc8) let the side edges run the full height.
RT3_NOINLINE void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags) {
    const int32_t* mode = ScreenMode();
    double t = (double)thick + D(0x5c8918);
    float tw = (float)(t / mode[0]);
    float th = (float)(t / mode[1]);
    Ui_FillRect(color, snap, x, y, w, th, flags);
    Ui_FillRect(color, snap, x, (float)(D(0x5c9ba0) / ScreenMode()[1] + ((double)y + h - th)), w, th, flags);
    double inset = (color & 0xff000000) > 0xc8000000 ? (double)Fl(0x5c8598) : (double)th;
    float h2 = (float)((double)h - (inset + inset));
    float y2 = (float)(inset + y);
    Ui_FillRect(color, snap, x, y2, tw, h2, flags);
    Ui_FillRect(color, snap, (float)((double)x + w - tw), y2, tw, h2, flags);
}
RT3_REPLACE(0x54f9f0, Ui_FrameRect);

// 0x54fb10. Draws a line `width` pixels wide: axis-aligned ones as snapped boxes,
// others as two quads around the segment; `glow` first draws it half a pixel
// thinner, then half a pixel wider at half the alpha.
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width) {
    if (!(width > Fl(0x5c8598)) && width == width) return;
    float lenX = (float)((double)x2 - x1);
    float lenY = (float)((double)y2 - y1);
    for (;;) {
        const int32_t* mode = ScreenMode();
        double t = (double)width + D(0x5c8918);
        float tw = (float)(t / mode[0]);
        float th = (float)(t / mode[1]);
        if (lenX == Fl(0x5c8598)) {
            Ui_FillRect(color, 1, (float)((double)x1 - (double)tw * D(0x5c8628)), y1, tw, lenY, 3);
            return;
        }
        if (lenY == Fl(0x5c8598)) {
            Ui_FillRect(color, 1, x1, (float)((double)y1 - (double)th * D(0x5c8628)), lenX, th, 3);
            return;
        }
        if (!glow) {
            // Corners around the segment, exactly as the x87 code computes them.
            float k = Fl(0x5c8764);
            double half = D(0x5c8628);
            double ox, oy;
            float sinA, ax1, ay1, bx2, by2;
            __asm {
                fld lenY
                fdiv lenX
                fld1
                fpatan
                fsub k
                fld st(0)
                fcos
                fxch st(1)
                fsin
                fstp sinA
                fmul tw
                fmul half
                fst ox
                fld x1
                fsub st(0), st(1)
                fstp ax1
                fld sinA
                fmul th
                fmul half
                fst oy
                fld y1
                fsub st(0), st(1)
                fstp ay1
                fld x2
                fadd st(0), st(2)
                fstp bx2
                fld y2
                fadd st(0), st(1)
                fstp by2
                fstp st(0)
                fstp st(0)
            }
            float d1 = Fade_Step_Impl();
            Engine_QueueQuad(hook::As<void*>(g_engine), 0, FBits((float)(x1 + ox)), FBits((float)(y1 + oy)), FBits(ax1), FBits(ay1), FBits(bx2),
                             FBits(by2), FBits(d1), color, color, color, 0, 0, 0, 0x3f800000, 0x3f800000, 0x3f800000, 0, 0x3f800000,
                             0x3f800000, 0x3f800000);
            float d2 = Fade_Step_Impl();
            Engine_QueueQuad(hook::As<void*>(g_engine), 0, FBits(ax1), FBits(ay1), FBits((float)(x2 - ox)), FBits((float)(y2 - oy)), FBits(bx2),
                             FBits(by2), FBits(d2), color, color, color, 0, 0, 0, 0x3f800000, 0x3f800000, 0x3f800000, 0, 0x3f800000,
                             0x3f800000, 0x3f800000);
            return;
        }
        Ui_Line(color, 0, x1, y1, x2, y2, (float)((double)width - D(0x5c8628)));
        width = (float)((double)width + D(0x5c8628));
        color = (color & 0xffffff) | ((color >> 1) & 0x7f000000);
        glow = 0;
        if (!(width > Fl(0x5c8598)) && width == width) return;
    }
}
RT3_REPLACE(0x54fb10, Ui_Line);

// 0x54fdd0. Queues a quad with the current fade as its alpha (p7); with `clip`
// (s5) it is wrapped in a clip rectangle (s8..s11).
RT3_NOINLINE void __fastcall Fx_QueueFadedQuad(uint32_t a, uint32_t b, uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3,
                                  uint32_t s4, uint32_t clip, uint32_t s6, uint32_t, uint32_t s8, uint32_t s9,
                                  uint32_t s10, uint32_t s11) {
    const uint32_t one = 0x3f800000;
    if (clip) Engine_QueueType5(Engine_103(), 0, s8, s9, s10, s11);
    float fade = Fade_Step_Impl();
    Engine_QueueQuad(Engine_103(), 0, s0, s1, s2, s3, s4, clip, *(uint32_t*)&fade, a, b, s6, 0, 0, 0, one, one, one, 0,
                     one, one, one);
    if (clip) Engine_QueueType6(Engine_103());
}
RT3_REPLACE(0x54fdd0, Fx_QueueFadedQuad);

// 0x54fed0. Queues a gradient quad (x, y) x 4 with corner colours (c0, c1, c2, c3)
// as two faded triangles; `clip` brackets them with the clip rect (cx0..cy1).
RT3_NOINLINE void __fastcall Engine_QueueGradientQuad(uint32_t c0, uint32_t c1, uint32_t x0, uint32_t y0, uint32_t x1,
                                         uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3,
                                         uint32_t c2, uint32_t c3, int32_t clip, uint32_t cx0, uint32_t cy0,
                                         uint32_t cx1, uint32_t cy1) {
    const uint32_t one = 0x3f800000;
    if (clip) Engine_QueueType5(hook::As<void*>(g_engine), 0, cx0, cy0, cx1, cy1);
    float f = Fade_Step_Impl();
    Engine_QueueQuad(hook::As<void*>(g_engine), 0, x0, y0, x1, y1, x2, y2, Bits(f), c0, c1, c2, 0, 0, 0, one,
                     one, one, 0, one, one, one);
    f = Fade_Step_Impl();
    Engine_QueueQuad(hook::As<void*>(g_engine), 0, x2, y2, x3, y3, x0, y0, Bits(f), c2, c3, c0, 0, 0, 0, one,
                     one, one, 0, one, one, one);
    if (clip) Engine_QueueType6(hook::As<void*>(g_engine));
}
RT3_REPLACE(0x54fed0, Engine_QueueGradientQuad);

}  // namespace rt3
