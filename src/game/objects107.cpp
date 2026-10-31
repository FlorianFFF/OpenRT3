// List resets / string trim / pool holder / music stream / widget basics in blocks 0x47..0x55 (batch 110). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall ListPair_Reset(uint8_t* o);
char* __fastcall StrCopy(char* dst, const char* src);
void __fastcall Holder_Free(uint8_t* h);
const char* __fastcall GetString(int32_t id);
void __fastcall String_Assign(char** s, const char* src);

static inline bool IsSpace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

void __fastcall Group_ResetLists(uint8_t* g) {
    for (int32_t i = 0; i < F<int32_t>(g, 8); ++i) ListPair_Reset(F<uint8_t*>(g, 0xc + i * 4));
}
RT3_REPLACE(0x474970, Group_ResetLists);

void __fastcall SceneObj_ResetAttachmentLists(uint8_t* o) {
    if (!F<PtrList*>(o, 0x79)) return;
    PtrList_Rewind(F<PtrList*>(o, 0x79));
    for (uint8_t* e = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79)); e; e = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79)))
        ListPair_Reset(e);
}
RT3_REPLACE(0x52eb90, SceneObj_ResetAttachmentLists);

// 0x557280. Trims leading whitespace in place.
void __fastcall Str_TrimLeft(char* s) {
    int32_t n = 0;
    while (s[n]) ++n;
    if (n <= 0) return;
    char* p = s;
    if (!*p) return;
    while (IsSpace(*p)) {
        if (!*++p) break;
    }
    if (p == s) return;
    if (p < s + n) StrCopy(s, p);
    else *s = 0;
}
RT3_REPLACE(0x557280, Str_TrimLeft);

// 0x557550. (Re)allocates the holder's pools: +4 of +0xc records (0x114 bytes),
// +0 of +0x10 records (0x204 bytes), and the 12-byte list +8.
void __fastcall Holder_Alloc(uint8_t* h) {
    if (F<void*>(h, 8)) Holder_Free(h);
    h[0x14] = 0;
    F<int32_t>(h, 0x114) = 0;
    void* a = Malloc(F<int32_t>(h, 0xc) * 0x114);
    void* b = Malloc(F<int32_t>(h, 0x10) * 0x204);
    uint32_t* p = (uint32_t*)ExeNew(0x1c);
    if (p) for (int k = 0; k < 7; ++k) p[k] = 0;
    F<uint32_t*>(h, 4) = p;
    Pool_Init((NodePool*)p, 0, a, 0x114, F<int32_t>(h, 0xc), 0);
    p = (uint32_t*)ExeNew(0x1c);
    if (p) for (int k = 0; k < 7; ++k) p[k] = 0;
    F<uint32_t*>(h, 0) = p;
    Pool_Init((NodePool*)p, 0, b, 0x204, F<int32_t>(h, 0x10), 0);
    p = (uint32_t*)ExeNew(0xc);
    if (p) p[0] = p[1] = p[2] = 0;
    F<uint32_t*>(h, 8) = p;
}
RT3_REPLACE(0x557550, Holder_Alloc);

// Music stream (+0, Miles HSTREAM; volume +0x104).
static inline int32_t StreamStatus(void* s) { return Import<int32_t(__stdcall*)(void*)>(0x5c83c0)(s); }
static inline void StreamPause(void* s, int32_t on) { Import<void(__stdcall*)(void*, int32_t)>(0x5c8464)(s, on); }
static inline void StreamClose(void* s) { Import<void(__stdcall*)(void*)>(0x5c83bc)(s); }

void __fastcall Music_Close(uint8_t* m) {
    if (!F<void*>(m, 0)) return;
    StreamPause(F<void*>(m, 0), 1);
    StreamClose(F<void*>(m, 0));
}
RT3_REPLACE(0x557c70, Music_Close);
int32_t __fastcall Music_IsPlaying(uint8_t* m) { return F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 4 ? 1 : 0; }
RT3_REPLACE(0x557c90, Music_IsPlaying);
int32_t __fastcall Music_IsPaused(uint8_t* m) { return F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 8 ? 1 : 0; }
RT3_REPLACE(0x557cb0, Music_IsPaused);

// 0x557cd0. Stops and closes the stream, forgets it in the active table
// (0xd973b8, 10) and reports the end (0xd973e0) when asked.
void __fastcall Music_Stop(uint8_t* m, int, uint32_t notify) {
    void* s = F<void*>(m, 0);
    if (!s) return;
    Import<void(__stdcall*)(void*, void*)>(0x5c83c4)(s, nullptr);
    StreamPause(F<void*>(m, 0), 1);
    StreamClose(F<void*>(m, 0));
    for (int i = 0; i < 10; ++i)
        if (((void**)0xd973b8)[i] == F<void*>(m, 0)) {
            ((void**)0xd973b8)[i] = nullptr;
            break;
        }
    F<void*>(m, 0) = nullptr;
    auto cb = hook::Global<void(__fastcall*)(uint32_t)>(0xd973e0);
    if (cb && notify) cb(notify);
}
RT3_REPLACE(0x557cd0, Music_Stop);

// 0x557d90. Sets the music volume: 0 pauses a playing stream, non-zero resumes
// a paused one; levels scaled by 0x5ddcc8.
void __fastcall Music_SetVolume(uint8_t* m, int, float vol) {
    void* s = F<void*>(m, 0);
    if (!s) {
        F<float>(m, 0x104) = vol;
        return;
    }
    if (vol == *(const float*)0x5c8598) {
        if (StreamStatus(s) == 4) {
            if ((F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 4) ||
                (F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 8))
                StreamPause(F<void*>(m, 0), 1);
        }
    } else if (StreamStatus(s) == 8) {
        if ((F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 4) ||
            (F<void*>(m, 0) && StreamStatus(F<void*>(m, 0)) == 8))
            StreamPause(F<void*>(m, 0), 0);
    }
    float v = (float)((double)vol * *(const float*)0x5ddcc8);
    Import<void(__stdcall*)(void*, float, float)>(0x5c83fc)(F<void*>(m, 0), v, v);
    F<float>(m, 0x104) = vol;
}
RT3_REPLACE(0x557d90, Music_SetVolume);

void __fastcall Mem_Fill32(uint32_t* dst, uint32_t v, uint32_t count) {
    for (uint32_t k = 0; k < count; ++k) dst[k] = v;
}
RT3_REPLACE(0x557f50, Mem_Fill32);

// 0x558080. Widget defaults.
void __fastcall Widget_Init(uint8_t* w) {
    static const uint8_t zero32[] = { 0xb, 0xf, 0x13, 0x17, 0x43, 0x47, 0x63, 0x4b, 0x33, 0x1b, 0x1f, 0x23, 0x27,
                                      0x4f, 0x6c, 0x70, 0x74, 0x78 };
    F<int32_t>(w, 0x86) = 0;
    F<int32_t>(w, 0x8a) = 0;
    F<int32_t>(w, 0x8e) = 0;
    F<uint16_t>(w, 4) = 0;
    F<uint16_t>(w, 0x7c) = 0;
    F<uint16_t>(w, 9) = 0;
    for (uint8_t off : zero32) F<int32_t>(w, off) = 0;
    w[0x2b] = w[0x2c] = 0;
    F<int32_t>(w, 0x92) = 0;
    w[0x37] = 0;
    w[0x67] = 0;
    F<uint16_t>(w, 6) = 0;
    F<int32_t>(w, 0x38) = -1;
    F<uint16_t>(w, 0x6a) = 3;
    F<uint16_t>(w, 0x68) = 3;
    F<uint16_t>(w, 0x40) = 0;
    w[0x42] = 0;
    w[0x2d] = w[0x2e] = w[0x2f] = w[0x30] = 0;
    F<uint16_t>(w, 0x7e) = 0xffff;
    F<uint16_t>(w, 0x80) = 0xffff;
    F<int32_t>(w, 0x96) = 0;
    F<int32_t>(w, 0x9a) = 0;
    F<int32_t>(w, 0x9e) = 0;
}
RT3_REPLACE(0x558080, Widget_Init);

// 0x558130. Attaches the widget to its parent window: caption string (+0x40 ->
// +0x43), default id, then vtbl +0x14 and +0x3c.
void __fastcall Widget_Attach(uint8_t* w, int, uint8_t* parent) {
    F<uint8_t*>(w, 0x86) = parent;
    if (uint16_t id = F<uint16_t>(w, 0x40)) String_Assign((char**)(w + 0x43), GetString(id));
    if (!F<int32_t>(w, 0x38)) F<int32_t>(w, 0x38) = -1;
    ((void(__thiscall*)(void*))(*(void***)w)[0x14 / 4])(w);
    ((void(__thiscall*)(void*))(*(void***)w)[0x3c / 4])(w);
}
RT3_REPLACE(0x558130, Widget_Attach);

// 0x5581e0. Whether (x, y) hits the widget: always for flag 2, never for 0x4000;
// the padded rectangle (+0x1b.., 0x5ddd10 margin) or, for types 1 and 4, +0xb..
int32_t __fastcall Widget_HitTest(uint8_t* w, int, float x, float y) {
    uint32_t fl = F<uint32_t>(w, 0x33);
    if (fl & 2) return 1;
    if (fl & 0x4000) return 0;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const double m = *(const double*)0x5ddd10;
    if (!(F<float>(w, 0x23) == *(const float*)0x5c8598)) {
        if (!(((double)F<float>(p, 5) + F<float>(w, 0x1b)) - m <= x)) return 0;
        if (!(((double)F<float>(w, 0x1f) + F<float>(p, 9)) - m <= y)) return 0;
        if (!((((double)F<float>(p, 5) + F<float>(w, 0x23)) + F<float>(w, 0x1b)) + m >= x)) return 0;
        return (((double)F<float>(w, 0x27) + F<float>(w, 0x1f)) + F<float>(p, 9)) + m >= y ? 1 : 0;
    }
    int16_t t = F<int16_t>(w, 6);
    if (t != 1 && t != 4) return 0;
    p = F<uint8_t*>(w, 0x86);
    if (!((double)F<float>(p, 5) + F<float>(w, 0xb) <= x)) return 0;
    if (!((double)F<float>(p, 9) + F<float>(w, 0xf) <= y)) return 0;
    if (!(((double)F<float>(w, 0x13) + F<float>(p, 5)) + F<float>(w, 0xb) >= x)) return 0;
    return (((double)F<float>(w, 0x17) + F<float>(p, 9)) + F<float>(w, 0xf)) >= y ? 1 : 0;
}
RT3_REPLACE(0x5581e0, Widget_HitTest);

void __fastcall Nop558460(void*, int, int32_t, int32_t, int32_t) {}
RT3_REPLACE(0x558460, Nop558460);

// 0x558470 / 0x5584a0. Set / clear state bits (+0x68), notifying (vtbl +0x1c) on change.
RT3_NOINLINE void __fastcall Widget_SetState(uint8_t* w, int, uint32_t mask) {
    uint32_t v = F<uint16_t>(w, 0x68);
    if ((v & mask) == mask) return;
    F<uint16_t>(w, 0x68) = (uint16_t)(v | mask);
    ((void(__thiscall*)(void*))(*(void***)w)[0x1c / 4])(w);
}
RT3_REPLACE(0x558470, Widget_SetState);
RT3_NOINLINE void __fastcall Widget_ClearState(uint8_t* w, int, uint32_t mask) {
    uint32_t v = F<uint16_t>(w, 0x68);
    if (!(mask & v)) return;
    F<uint16_t>(w, 0x68) = (uint16_t)(~mask & v);
    ((void(__thiscall*)(void*))(*(void***)w)[0x1c / 4])(w);
}
RT3_REPLACE(0x5584a0, Widget_ClearState);

// 0x5584d0. Horizontal position of the widget centre as a 0..1 pan value.
double __fastcall Widget_Pan_Impl(uint8_t* w) {
    uint8_t* p = F<uint8_t*>(w, 0x86);
    if (!p) return *(const float*)0x5c8d48;
    double v = (((double)F<float>(p, 5) + F<float>(w, 0xb)) + F<float>(w, 0x13) * *(const double*)0x5c8628) *
                   *(const double*)0x5c8680 + *(const double*)0x5c8988;
    if (v < *(const float*)0x5c8598) return *(const float*)0x5c8598;
    if (v > *(const float*)0x5c85b4) return *(const float*)0x5c85b4;
    return v;
}
// Callers (0x55876a, 0x55884a, 0x558fe6) keep a value in DL across the call.
__declspec(naked) void Widget_Pan() { RT3_KEEP_ECX_EDX(Widget_Pan_Impl, 0) }
RT3_REPLACE(0x5584d0, Widget_Pan);

} // namespace rt3
