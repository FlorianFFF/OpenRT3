// Blink timers / fades / text width / pooled lists / vertex transforms / sound tick in blocks 0x45..0x55 (batch 108). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Flags24_Set1(uint8_t* o);
void __fastcall Flags24_Clear(uint8_t* o);
void __fastcall Obj_SetUnit25(uint8_t* o, int, float v);
void __fastcall Render_CreateOverviewTarget(uint8_t* r, int, int32_t size);
double __fastcall Font_GlyphAdvance(uint8_t* f, int, uint8_t ch);
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x4565b0. Advances the two blink timers (+0x376) of the indicator lights (+0x36e):
// off below 0x5cb2e0, on below 0x5cb2d8, then on and restart.
void __fastcall Vehicle_BlinkLights(uint8_t* v, int, float dt) {
    for (int k = 0; k < 2; ++k) {
        uint8_t* light = F<uint8_t*>(v, 0x36e + k * 4);
        if (!light) continue;
        float t = (float)((double)dt + F<float>(v, 0x376 + k * 4));
        F<float>(v, 0x376 + k * 4) = t;
        if ((double)t < *(const double*)0x5cb2e0) {
            Flags24_Clear(light);
        } else if ((double)t < *(const double*)0x5cb2d8) {
            Flags24_Set1(light);
        } else {
            Flags24_Set1(light);
            F<int32_t>(v, 0x376 + k * 4) = 0;
        }
    }
}
RT3_REPLACE(0x4565b0, Vehicle_BlinkLights);

// 0x45c8d0. Sets the opacity of every mesh (+0x44.., count +8) of every part and
// of the attachments.
void __fastcall Model_SetOpacity(uint8_t* m, int, float a) {
    for (int32_t i = 0; i < F<int32_t>(m, 0x31a); ++i) {
        uint8_t* part = F<uint8_t*>(m, 0x316) + i * 0xc0;
        for (int32_t j = 0; j < F<int32_t>(part, 8); ++j) Obj_SetUnit25(F<uint8_t*>(part, 0x44 + j * 4), 0, a);
    }
    if (!F<PtrList*>(m, 0x7d)) return;
    PtrList_Rewind(F<PtrList*>(m, 0x7d));
    for (uint8_t* e = (uint8_t*)PtrList_Next(F<PtrList*>(m, 0x7d)); e; e = (uint8_t*)PtrList_Next(F<PtrList*>(m, 0x7d)))
        Obj_SetUnit25(e, 0, a);
}
RT3_REPLACE(0x45c8d0, Model_SetOpacity);

void __fastcall Render_SetOverviewSize(uint8_t* r, int, int32_t size) {
    F<int32_t>(r, 0x1548) = size;
    Render_CreateOverviewTarget(r, 0, size);
}
RT3_REPLACE(0x536400, Render_SetOverviewSize);

// 0x550150. Width of a string (sum of glyph advances, float steps).
float __fastcall Font_TextWidth(uint8_t* f, int, const uint8_t* s) {
    float w = 0.0f;
    for (; *s; ++s) w = (float)(Font_GlyphAdvance(f, 0, *s) + w);
    return w;
}
RT3_REPLACE(0x550150, Font_TextWidth);

// 0x554450. Sets the channel volume (+0x128, sample +0x28 flag 1).
void __fastcall Channel_SetVolume(uint8_t* c, int, uint32_t v) {
    if (F<uint32_t>(c, 0x110)) {
        uint8_t* s = (uint8_t*)Sound_LookupSample(hook::Global<uint8_t*>(0x6d402c), 0, F<uint32_t>(c, 0x110), 1);
        if (s) {
            F<uint32_t>(s, 0x28) = v;
            F<uint32_t>(s, 0x44) |= 1;
            F<uint32_t>(c, 0x128) = v;
            return;
        }
        F<int32_t>(c, 0x110) = 0;
        F<int32_t>(c, 0x134) = 1;
    }
    F<uint32_t>(c, 0x128) = v;
}
RT3_REPLACE(0x554450, Channel_SetVolume);

void __fastcall Nop5548c0(void*, int, int32_t, int32_t) {}
RT3_REPLACE(0x5548c0, Nop5548c0);
void __fastcall Clear_d939a0(void*, int, int32_t, int32_t) { hook::Global<int32_t>(0xd939a0) = 0; }
RT3_REPLACE(0x5548d0, Clear_d939a0);

// 0x5548e0. Sets the view parameters when the first (a) is positive.
void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f) {
    if (a <= 0) return;
    F<int32_t>(v, 0x54) = a;
    F<uint32_t>(v, 0x58) = b;
    F<uint32_t>(v, 0x9c) = c;
    F<uint32_t>(v, 0xa0) = d;
    F<uint32_t>(v, 0xa4) = e;
    F<uint32_t>(v, 0xa8) = f;
}
RT3_REPLACE(0x5548e0, View_SetParams);

// 0x554920. Frame time +0xb4: 0.05 s while dt is at most 0x5d0058, else dt when the
// frame budget (+0xb8) holds at least one frame of dt (x87 with the exe's _ftol).
void __fastcall Timer_SetFrameTime(uint8_t* t, int, float dt) {
    if (!(*(const float*)0x5d0058 < dt) && !(dt != dt)) {
        F<uint32_t>(t, 0xb4) = 0x3d4ccccd;
        return;
    }
    int32_t budget = F<int32_t>(t, 0xb8), n;
    __asm {
        fild budget
        fdiv dt
        mov eax, 0x5a10d0
        call eax
        mov n, eax
    }
    if (n > 0) F<float>(t, 0xb4) = dt;
}
RT3_REPLACE(0x554920, Timer_SetFrameTime);

void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v) { F<uint32_t>(t, 0xb8) = v; }
RT3_REPLACE(0x554970, Timer_SetBudget);
void __fastcall Obj_SetPair1ed(uint8_t* o, int, uint32_t v) {
    F<uint32_t>(o, 0x1ed) = v;
    F<uint32_t>(o, 0x1f5) = v;
}
RT3_REPLACE(0x554980, Obj_SetPair1ed);
uint32_t __fastcall Get_d9399c() { return hook::Global<uint32_t>(0xd9399c); }
RT3_REPLACE(0x5549a0, Get_d9399c);

// 0x5549d0. out = v rotated by the 3x3 part of m (row vector).
float* __fastcall Vec3_Rotate(const float* v, int, float* out, const float* m) {
    double z = ((double)m[10] * v[2] + (double)m[2] * v[0]) + (double)m[6] * v[1];
    double y = ((double)m[9] * v[2] + (double)m[1] * v[0]) + (double)m[5] * v[1];
    double x = ((double)m[8] * v[2] + (double)m[4] * v[1]) + (double)m[0] * v[0];
    out[0] = (float)x;
    out[1] = (float)y;
    out[2] = (float)z;
    return out;
}
RT3_REPLACE(0x5549d0, Vec3_Rotate);

// Node lists {head, tail, count}; nodes link through their first dword. The pool
// 0x6d4030 +0x14 = {free list*, lo, hi, active} recycles nodes inside its block.
static inline void PushFront(uint32_t* l, uint32_t* n) {
    *n = l[0];
    l[2]++;
    l[0] = (uint32_t)n;
    if (!l[1]) l[1] = (uint32_t)n;
}
static inline void FreeAll(uint32_t* l) {
    for (uint32_t* n = (uint32_t*)l[0]; n;) {
        uint8_t* pool = F<uint8_t*>(hook::Global<uint8_t*>(0x6d4030), 0x14);
        uint32_t* next = (uint32_t*)*n;
        if (F<int32_t>(pool, 0xc) && (uint32_t)n >= F<uint32_t>(pool, 4) && (uint32_t)n < F<uint32_t>(pool, 8))
            PushFront(F<uint32_t*>(pool, 0), n);
        else
            ExeDelete(n);
        n = next;
    }
}

// 0x554a30. Clears the list pair +0/+4 (count +0x34): mode 1 frees all nodes and
// both lists, otherwise the second list's nodes move to the first.
void __fastcall ListPair_Clear(uint8_t* o, int, int32_t mode) {
    if (F<int32_t>(o, 0x34) > 0) {
        if (mode == 1) {
            FreeAll(F<uint32_t*>(o, 0));
            FreeAll(F<uint32_t*>(o, 4));
            F<int32_t>(o, 0x34) = 0;
        } else {
            for (uint32_t* n = (uint32_t*)*F<uint32_t*>(o, 4); n;) {
                uint32_t* next = (uint32_t*)*n;
                PushFront(F<uint32_t*>(o, 0), n);
                n = next;
            }
            uint32_t* b = F<uint32_t*>(o, 4);
            b[0] = b[1] = b[2] = 0;
            return;
        }
    } else if (mode != 1) {
        return;
    }
    if (void* a = F<void*>(o, 0)) {
        ExeDelete(a);
        F<void*>(o, 0) = nullptr;
    }
    if (void* b = F<void*>(o, 4)) {
        ExeDelete(b);
        F<void*>(o, 4) = nullptr;
    }
}
RT3_REPLACE(0x554a30, ListPair_Clear);

// 0x554d70. Transforms the emitter points (+0x10, +0x30 of them) by the owner's
// world matrix (+0xc +0xce) into +0x1dd (pointer table, then xyz); without points
// the owner's position. Returns the table and the count.
uint8_t* __fastcall Emitter_WorldPoints(uint8_t* e, int, int32_t* count) {
    const float* src = F<float*>(e, 0x10);
    int32_t n = F<int32_t>(e, 0x30);
    uint32_t* tab = F<uint32_t*>(e, 0x1dd);
    float* out = (float*)(tab + n);
    if (src) {
        for (int32_t i = 0; i < F<int32_t>(e, 0x30); ++i, src += 3, out += 3) {
            tab[i] = (uint32_t)out;
            const float* m = (const float*)(F<uint8_t*>(e, 0xc) + 0xce);
            double x = ((double)m[8] * src[2] + (double)m[4] * src[1]) + (double)src[0] * m[0] + m[12];
            double y = ((double)m[9] * src[2] + (double)m[1] * src[0]) + (double)m[5] * src[1] + m[13];
            float z = (float)(((double)m[10] * src[2] + (double)m[2] * src[0]) + (double)m[6] * src[1] + m[14]);
            out[0] = (float)x;
            out[2] = z;
            out[1] = (float)y;
        }
    } else {
        tab[0] = (uint32_t)out;
        const float* m = (const float*)(F<uint8_t*>(e, 0xc) + 0xce);
        out[0] = m[12];
        out[2] = m[14];
        out[1] = m[13];
    }
    F<int32_t>(e, 0x1e1) = 1;
    *count = F<int32_t>(e, 0x30);
    return F<uint8_t*>(e, 0x1dd);
}
RT3_REPLACE(0x554d70, Emitter_WorldPoints);

// 0x556500. Ambient sound tick: fades the day/night blend +0x30 toward the ends
// (direction +0x34, speed 0x6257dc) and updates all sounds (Hash_ForEach 0x5563b0),
// twice when the 10 s refresh (0x6257d8) is due.
void __fastcall Ambience_Tick(uint8_t* a, int, uint32_t flag) {
    hook::Global<int32_t>(0xd97ab8) = 0;
    F<int32_t>(a, 0x20) = 0;
    F<int32_t>(a, 0x24) = 0;
    hook::Global<uint32_t>(0xd93998) = flag;
    int32_t due = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x2c) + 10000;
    if (!(F<float>(a, 0x30) > *(const float*)0x5c970c)) {
        F<uint32_t>(a, 0x34) = 0x3f800000;
        hook::Global<uint32_t>(0x6257dc) = 0x3951b717;
    } else if (!(F<float>(a, 0x30) < *(const float*)0x5c85b4) && F<float>(a, 0x30) == F<float>(a, 0x30)) {
        F<uint32_t>(a, 0x34) = 0xbf800000;
        hook::Global<uint32_t>(0x6257dc) = 0x39d1b717;
    }
    auto visit = (HashVisit)0x5563b0;
    hook::Global<int32_t>(0xd939a8) = 1;
    F<float>(a, 0x30) = (float)((double)hook::Global<float>(0x6257dc) * F<float>(a, 0x34) + F<float>(a, 0x30));
    if (due < hook::Global<int32_t>(0x6257d8)) {
        hook::Global<int32_t>(0x6257d8) = due - 10000;
        Hash_ForEach(F<HashMap*>(a, 8), 0, 0, visit, nullptr);
        hook::Global<int32_t>(0xd93998) = 1;
    }
    hook::Global<int32_t>(0x6257d8) = due;
    Hash_ForEach(F<HashMap*>(a, 8), 0, 0, visit, nullptr);
    F<int32_t>(a, 0x2c)++;
    hook::Global<int32_t>(0xd939a8) = 0;
}
RT3_REPLACE(0x556500, Ambience_Tick);

// 0x5565e0. Runs the sound visitor over all entries with context {a, b, c}.
void __fastcall Ambience_Visit(uint8_t* a, int, uint32_t x, uint32_t y, uint32_t z) {
    uint32_t ctx[3] = { x, y, z };
    Hash_ForEach(F<HashMap*>(a, 8), 0, 0, (HashVisit)0x5563b0, ctx);
}
RT3_REPLACE(0x5565e0, Ambience_Visit);

} // namespace rt3
