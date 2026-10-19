// Fog density / texture teardown / keyboard and input object in blocks 0x52..0x54 (batch 105). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "hook/keepregs.h"
#include <stdint.h>

struct HWND__;

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

int32_t __fastcall Fx_Intensity(uint8_t* o);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_RestoreTarget(uint8_t* s);
void __fastcall Texture_ReleaseTarget(uint8_t* t);
void __fastcall Texture_RemoveAllImages(uint8_t* t);
::HWND__* __fastcall Engine_Window(void* e);
void __fastcall PumpMessages();

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }

// 0x526590. Fog density 0..79 from the fog factor (+0x21), the effect intensity
// and, above 0x5dcff0, the height +0x368e (x87 with the exe's _ftol).
int32_t __fastcall Scene_FogDensity(uint8_t* s) {
    float f21 = F<float>(s, 0x21);
    int32_t v;
    __asm {
        fld dword ptr ds:[0x5c85b4]
        fsub f21
        fsub qword ptr ds:[0x5c8758]
        fmul qword ptr ds:[0x5dcff8]
        fsubr qword ptr ds:[0x5dcff8]
        mov eax, 0x5a10d0
        call eax
        mov v, eax
    }
    v -= Fx_Intensity(s) >> 2;
    float h = F<float>(s, 0x368e);
    if ((double)h > *(const double*)0x5dcff0) {
        int32_t base = v;
        __asm {
            fld dword ptr ds:[0x5ca164]
            fsub h
            fmul qword ptr ds:[0x5d0c70]
            fimul base
            mov eax, 0x5a10d0
            call eax
            mov v, eax
        }
    }
    if (v > 0x4f) return 0x4f;
    if (v < 0) return 0;
    return v;
}
RT3_REPLACE(0x526590, Scene_FogDensity);

// 0x542c90. Texture teardown: frees its lists, unlinks it from the global texture
// list, restores the back buffer if it was a render target and frees everything.
void __fastcall Texture_Destroy(uint8_t* t) {
    uint8_t* head = hook::Global<uint8_t*>(0xccbb2c);
    static const uint32_t lists[] = { 0xdc, 0xe4 };
    for (uint32_t off : lists) {
        if (PtrList* l = F<PtrList*>(t, off)) {
            PtrList_Destroy(l);
            ((void(__cdecl*)(void*))0x53b080)(l);
        }
    }
    F<int32_t>(t, 0xe0) = 0;
    if (hook::Global<uint8_t*>(0xccbb2c) == t) {
        hook::Global<uint8_t*>(0xccbb2c) = F<uint8_t*>(t, 0);
    } else {
        for (uint8_t* p = head; p; p = F<uint8_t*>(p, 0))
            if (F<uint8_t*>(p, 0) == t) {
                F<uint8_t*>(p, 0) = F<uint8_t*>(t, 0);
                break;
            }
    }
    if (t[0x93]) Scene_RestoreTarget((uint8_t*)Engine_Renderer(Engine()));
    Texture_ReleaseTarget(t);
    Texture_RemoveAllImages(t);
    F<int32_t>(t, 7) = 0;
    F<int32_t>(t, 0xc0) = 0;
}
RT3_REPLACE(0x542c90, Texture_Destroy);

// 0x54e630. Whether the game window is in the foreground.
int32_t __fastcall Engine_IsForeground() {
    if (!Engine()) return 0;
    ::HWND__* w = Engine_Window(Engine());
    return w == Import<::HWND__*(__stdcall*)()>(0x5c8284)() ? 1 : 0;
}
RT3_REPLACE(0x54e630, Engine_IsForeground);

static inline void KeyboardState(uint8_t* buf) { Import<int32_t(__stdcall*)(uint8_t*)>(0x5c8280)(buf); }

// 0x54e660. Sets the scroll-lock toggle (by simulating a key press).
void __fastcall Keyboard_SetScrollLock(int32_t on) {
    uint8_t keys[0x100];
    KeyboardState(keys);
    if ((int32_t)(keys[0x91] & 1) == on) return;
    hook::Global<int32_t>(0xd93840)++;
    auto keybd = Import<void(__stdcall*)(uint8_t, uint8_t, uint32_t, uint32_t)>(0x5c827c);
    keybd(0x91, 0x45, 1, 0);
    keybd(0x91, 0x45, 3, 0);
    PumpMessages();
    hook::Global<int32_t>(0xd93840)--;
}
RT3_REPLACE(0x54e660, Keyboard_SetScrollLock);

// 0x54e6d0. Remembers the scroll-lock toggle (0xd93844).
void __fastcall Keyboard_SaveScrollLock() {
    uint8_t keys[0x100];
    KeyboardState(keys);
    hook::Global<uint32_t>(0xd93844) = keys[0x91] & 1;
}
RT3_REPLACE(0x54e6d0, Keyboard_SaveScrollLock);

// 0x54e710. Input object constructor: key table, buttons swapped (SM_SWAPBUTTON),
// cursor at the centre, double-click time.
uint8_t* __fastcall Input_Construct(uint8_t* in) {
    for (int k = 0; k < 0x40; ++k) F<int32_t>(in, k * 4) = 0;
    F<int32_t>(in, 0xa88) = 0;
    F<int32_t>(in, 0xa8c) = 0;
    F<int32_t>(in, 0xaa4) = Import<int32_t(__stdcall*)(int32_t)>(0x5c8274)(0x17);
    F<uint32_t>(in, 0xa94) = 0x3f000000;
    F<uint32_t>(in, 0xa98) = 0x3f000000;
    F<int32_t>(in, 0xa9c) = 0;
    F<int32_t>(in, 0xa90) = 0;
    F<int32_t>(in, 0x100) = 0;
    F<uint32_t>(in, 0xaa0) = Import<uint32_t(__stdcall*)()>(0x5c8278)();
    for (int k = 0; k < 9; ++k) F<int32_t>(in, 0x104 + k * 4) = 0;
    return in;
}
RT3_REPLACE(0x54e710, Input_Construct);

void __fastcall Nop54e780() {}
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
void __fastcall Input_ClearKeys(uint8_t* in) {
    for (int k = 0; k < 0x40; ++k) F<int32_t>(in, k * 4) = 0;
    F<uint32_t>(in, 0xa8c) &= 0xffffffd8;
}
RT3_REPLACE(0x54e810, Input_ClearKeys);

// 0x54e830. Refreshes the ctrl bit (4) of +0xa8c from the keyboard.
void __fastcall Input_UpdateCtrl(uint8_t* in) {
    uint8_t keys[0x100];
    KeyboardState(keys);
    if (keys[0x11] & 0x80) F<uint32_t>(in, 0xa8c) |= 4;
    else F<uint32_t>(in, 0xa8c) &= ~4u;
}
RT3_REPLACE(0x54e830, Input_UpdateCtrl);

// 0x54f540. Whether the cursor is inside the client area (always when fullscreen).
int32_t __fastcall Cursor_InWindow() {
    if (Engine()[0x114226]) return 1;
    int32_t pt[2];
    Import<int32_t(__stdcall*)(int32_t*)>(0x5c8270)(pt);
    ::HWND__* w = Engine_Window(Engine());
    Import<int32_t(__stdcall*)(::HWND__*, int32_t*)>(0x5c8268)(w, pt);
    const int32_t* mode = (const int32_t*)(F<uint8_t*>(Engine(), 0x34) + F<int32_t>(Engine(), 0x11421e) * 0x15);
    if (pt[0] < 0 || pt[0] >= mode[0]) return 0;
    if (pt[1] < 0 || pt[1] >= mode[1]) return 0;
    return 1;
}
RT3_REPLACE(0x54f540, Cursor_InWindow);

// 0x54f640. Steps the fade value 0xd93850 down by 0x5ddb80; returns it, or
// 0x5ddb78 once it is negative (or above 0x5c85b8 when 0x625464 is unset).
float __fastcall Fade_Step_Impl() {
    float& g = hook::Global<float>(0xd93850);
    double d = (double)g - *(const double*)0x5ddb80;
    g = (float)d;
    if (!(d >= *(const float*)0x5c8598)) return *(const float*)0x5ddb78;
    if ((double)g <= *(const double*)0x5c85b8) return g;
    if (*(const float*)0x625464 == *(const float*)0x5c9b08) return *(const float*)0x5ddb78;
    return g;
}
// 0x54fd20 passes ECX through to the next call.
__declspec(naked) void Fade_Step() { RT3_KEEP_ECX_EDX(Fade_Step_Impl, 0) }
RT3_REPLACE(0x54f640, Fade_Step);

void __fastcall Fade_StartSlow() { hook::Global<uint32_t>(0xd93850) = 0x3b30f27c; }
RT3_REPLACE(0x54f6a0, Fade_StartSlow);
void __fastcall Fade_StartFast() { hook::Global<uint32_t>(0xd93850) = 0x3baa64c3; }
RT3_REPLACE(0x54f6b0, Fade_StartFast);

} // namespace rt3
