// Positional sound channel movement in block 0x55 (batch 190). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

struct PtrListNode;
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Channel_Command(uint8_t* c, int, uint8_t cmd, int32_t count);
void __fastcall Channel_ApplyTo(uint8_t* c, int, uint8_t* s);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Terrain_Remove1679(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
PtrListNode* __fastcall Terrain_Add1679(uint8_t* r, int, void* v, int32_t x, int32_t y);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
void* __fastcall Engine_Renderer(void* e);

static inline int32_t Cell16(float v) {
    double d = (double)v * *(const float*)0x5dcfe8;
    int32_t r;
    __asm {
        fld d
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r >> 4;
}

// 0x5545d0. Moves a positional sound channel to (x, y, z): updates its playing sample
// (+0x110; re-applied when +0x134 is set, else just moved; one-shots not looping
// +0x138 lose their loop), starts it when it enters an audible area of the terrain's
// 16-cell grid (+4, flags +0x47c) and stops it when it leaves; re-files the channel
// in the grid's per-cell lists (+0) when it changes cell.
void __fastcall Channel_MoveTo(uint8_t* c, int, float x, float y, float z) {
    int32_t ox = Cell16(F<float>(c, 0x114));
    int32_t oz = Cell16(F<float>(c, 0x11c));
    F<float>(c, 0x114) = x;
    F<float>(c, 0x118) = y;
    F<float>(c, 0x11c) = z;
    int32_t nx = Cell16(x);
    int32_t nz = Cell16(z);
    uint8_t* grid = F<uint8_t*>(c, 4);
    uint8_t was = grid[0x47c + ox * 64 + oz] & 1;
    uint8_t now = grid[0x47c + nx * 64 + nz] & 1;
    bool playing = false;
    if (F<uint32_t>(c, 0x110)) {
        uint8_t* s = (uint8_t*)Sound_LookupSample(hook::Global<uint8_t*>(0x6d402c), 0, F<uint32_t>(c, 0x110), 1);
        if (s) {
            if (F<int32_t>(c, 0x134) == 1) Channel_ApplyTo(c, 0, s);
            else {
                F<float>(s, 0x10) = x;
                F<float>(s, 0x14) = y;
                F<float>(s, 0x18) = z;
                F<uint32_t>(s, 0x44) |= 2;
            }
            if (!F<int32_t>(c, 0x138) && F<int32_t>(s, 0xc) == 1) {
                F<uint32_t>(s, 0x44) |= 8;
                F<int32_t>(s, 0xc) = 0;
            }
            playing = true;
        } else {
            F<uint32_t>(c, 0x110) = 0;
            F<int32_t>(c, 0x134) = 1;
        }
    }
    if (!playing) F<uint32_t>(c, 0x110) = 0;
    if (was != now || !F<uint32_t>(c, 0x110)) {
        if (now) Channel_Command(c, 0, 1, F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0xd));
        else {
            c[0x13c] = 0;
            if (F<uint32_t>(c, 0x110)) {
                Sound_Stop(hook::Global<uint8_t*>(0x6d402c), 0, F<uint32_t>(c, 0x110), 1);
                F<uint32_t>(c, 0x110) = 0;
            }
        }
    }
    if (ox == nx && oz == nz) return;
    int32_t w = ((Terrain_Rows(F<uint8_t*>(c, 4)) * 0x5c - 0x5c) / 0x5c) >> 4;
    int32_t h = ((Terrain_Cols(F<uint8_t*>(c, 4)) * 0x5c - 0x5c) / 0x5c) >> 4;
    if (F<PtrListNode*>(c, 0)) Terrain_Remove1679(F<uint8_t*>(c, 4), 0, F<PtrListNode*>(c, 0), ox, oz);
    if (nx >= 0 && nx < w && nz >= 0 && nz < h) F<PtrListNode*>(c, 0) = Terrain_Add1679(F<uint8_t*>(c, 4), 0, c, nx, nz);
    else F<PtrListNode*>(c, 0) = nullptr;
}
RT3_REPLACE(0x5545d0, Channel_MoveTo);

} // namespace rt3

namespace rt3 {

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x5554f0. Colour of particle p from emitter e (colours +0x48 / +0x4c, tint
// +0x3c..+0x44): mode 1 picks a random blend of the two (shared LCG 0x624324) and
// stores it in p +0x74; with e +0xac bit 27 the blend follows p's age (+0x48 of
// +0x4c); else p keeps its colour. Quirk: mode 1 leaves the random factor on the x87
// stack, as the original does.
uint32_t __fastcall Particle_Color(uint8_t* e, int, uint8_t* p, int32_t mode) {
    int32_t r, g, b;
    uint32_t ca = F<uint32_t>(e, 0x48), cb = F<uint32_t>(e, 0x4c);
    if (mode == 1) {
        uint32_t& seed = hook::Global<uint32_t>(0x624324);
        seed = seed * 0x19660d + 0x3c6ef35f;
        double t = (double)(int32_t)(seed & 0x7fff) * *(const float*)0x5ddcb8;
        double u = (double)*(const float*)0x5c85b4 - t;
        r = Ftol((double)(int32_t)((ca >> 16) & 0xff) * u + (double)(int32_t)((cb >> 16) & 0xff) * t);
        g = Ftol((double)(int32_t)((ca >> 8) & 0xff) * u + (double)(int32_t)((cb >> 8) & 0xff) * t);
        b = Ftol((double)(int32_t)(ca & 0xff) * u + (double)(int32_t)(cb & 0xff) * t);
        F<uint32_t>(p, 0x74) = (((uint32_t)r << 8 | (uint32_t)g) << 8 | (uint32_t)b) & 0xffffff;
    } else if (F<uint32_t>(e, 0xac) & 0x8000000) {
        double u = (double)*(const float*)0x5c85b4 - (double)F<int32_t>(p, 0x48) / F<int32_t>(p, 0x4c);
        double v = (double)*(const float*)0x5c85b4 - u;
        r = Ftol((double)(int32_t)((cb >> 16) & 0xff) * u + (double)(int32_t)((ca >> 16) & 0xff) * v);
        g = Ftol((double)(int32_t)((cb >> 8) & 0xff) * u + (double)(int32_t)((ca >> 8) & 0xff) * v);
        b = Ftol((double)(int32_t)(cb & 0xff) * u + (double)(int32_t)(ca & 0xff) * v);
        F<uint32_t>(p, 0x74) = (((uint32_t)r << 8 | (uint32_t)g) << 8 | (uint32_t)b) & 0xffffff;
    } else {
        uint32_t c = F<uint32_t>(p, 0x74);
        r = (c >> 16) & 0xff;
        g = (c >> 8) & 0xff;
        b = c & 0xff;
    }
    uint32_t out = (uint32_t)Ftol((double)r * F<float>(e, 0x3c)) << 8;
    out = (out | (uint32_t)Ftol((double)g * F<float>(e, 0x40))) << 8;
    out |= (uint32_t)Ftol((double)b * F<float>(e, 0x44));
    return out;
}
RT3_REPLACE(0x5554f0, Particle_Color);

} // namespace rt3

#include "core/ptrlist.h"

namespace rt3 {

PtrList* __fastcall Terrain_Cell1671(uint8_t* r, int, int32_t x, int32_t y);
PtrList* __fastcall Terrain_Cell1679(uint8_t* r, int, int32_t x, int32_t y);

// 0x563f20. Rebuilds the audible area (16-cell units) around (x, y) with radius r
// in the terrain's sound grid: clears the audible flags (keeping bit 7), gathers the
// ambient sources of the cells (+0x1671) into +0x79, marks the cells audible, starts
// the positional channels there (+0x1679; new ones join the active list +0x8d) and
// stops the active channels of other sessions (+0xc != +5).
void __fastcall Sound_RefreshArea(uint8_t* o, int, int32_t x, int32_t y, int32_t r, uint8_t* grid) {
    x >>= 4;
    y >>= 4;
    r >>= 4;
    for (int32_t k = 0; k < 0x1000; ++k) grid[0x47c + k] &= 0x80;
    int32_t x0 = x - r <= 0 ? 0 : x - r;
    int32_t y0 = y - r <= 0 ? 0 : y - r;
    int32_t x1 = x + r < Terrain_Rows(grid) >> 4 ? x + r : Terrain_Rows(grid) >> 4;
    int32_t y1 = y + r < Terrain_Cols(grid) >> 4 ? y + r : Terrain_Cols(grid) >> 4;
    PtrList_Clear(F<PtrList*>(o, 0x79));
    for (int32_t cy = y0; cy < y1; ++cy)
        for (int32_t cx = x0; cx < x1; ++cx) {
            PtrList* l = Terrain_Cell1671(grid, 0, cx, cy);
            PtrList_Rewind(l);
            while (void* v = PtrList_Next(l)) PtrList_Append(F<PtrList*>(o, 0x79), 0, v);
            grid[0x47c + cx * 64 + cy] |= 1;
            PtrList* ch = Terrain_Cell1679(grid, 0, cx, cy);
            PtrList_Rewind(ch);
            for (uint8_t* c = (uint8_t*)PtrList_Next(ch); c; c = (uint8_t*)PtrList_Next(ch)) {
                if (!F<void*>(c, 0x13d)) {
                    Channel_Command(c, 0, 1, F<int32_t>(o, 5));
                    F<PtrListNode*>(c, 0x13d) = PtrList_Append(F<PtrList*>(o, 0x8d), 0, c);
                } else Channel_Command(c, 0, 1, F<int32_t>(o, 5));
            }
        }
    PtrList_Rewind(F<PtrList*>(o, 0x8d));
    for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x8d)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x8d))) {
        if (F<int32_t>(c, 0xc) == F<int32_t>(o, 5)) continue;
        Channel_Command(c, 0, 2, 0);
        PtrList_RemoveNode(F<PtrList*>(o, 0x8d), 0, F<PtrListNode*>(c, 0x13d));
        F<PtrListNode*>(c, 0x13d) = nullptr;
    }
}
RT3_REPLACE(0x563f20, Sound_RefreshArea);

} // namespace rt3

namespace rt3 {

// 0x564660. Expands a screen sprite (+1/+5 left/top, +9/+0xd right/bottom, depth
// +0x19, colour +0x1d faded by +0x29 / 2, texture rect +0x45..+0x51) into two
// triangles (6 vertices of x, y, z, colour, u, v); the off-diagonal texture corners
// are swapped for mirrored rects so the image stays upright.
void __stdcall Sprite_ToTriangles(const uint8_t* s, float* out, int32_t* count) {
    float a = F<float>((void*)s, 1), b = F<float>((void*)s, 5);
    uint32_t c = F<uint32_t>((void*)s, 9), d = F<uint32_t>((void*)s, 0xd), z = F<uint32_t>((void*)s, 0x19);
    float u0 = F<float>((void*)s, 0x45), v0 = F<float>((void*)s, 0x49), u1 = F<float>((void*)s, 0x4d), v1 = F<float>((void*)s, 0x51);
    uint32_t col = F<uint32_t>((void*)s, 0x1d);
    float c1u, c1v, c2u, c2v;
    bool same = (u0 < u1 && v0 < v1) || (u0 > u1 && v0 > v1);
    if (same) {
        c1u = u1;
        c1v = v0;
        c2u = u0;
        c2v = v1;
    } else {
        c1u = u0;
        c1v = v1;
        c2u = u1;
        c2v = v0;
    }
    float rf = (float)(int32_t)((col >> 16) & 0xff), gf = (float)(int32_t)((col >> 8) & 0xff);
    double bf = (double)(int32_t)(col & 0xff);
    double k = (double)F<float>((void*)s, 0x29) * *(const double*)0x5c8628;
    if (k > *(const double*)0x5c8770) k = *(const float*)0x5c85b4;
    else if (k < *(const float*)0x5c8598) k = *(const float*)0x5c8598;
    uint32_t nr = (uint32_t)Ftol((double)rf * k);
    uint32_t ng = (uint32_t)Ftol((double)gf * k);
    uint32_t nb = (uint32_t)Ftol(k * bf);
    uint32_t color = ((((nr << 8) | ng) << 8) | nb) | (col & 0xff000000);
    uint32_t* v = (uint32_t*)out;
    auto put = [&](int32_t i, uint32_t x, uint32_t y, float u, float w) {
        v[i * 6] = x;
        v[i * 6 + 1] = y;
        v[i * 6 + 2] = z;
        v[i * 6 + 3] = color;
        F<float>(v, i * 0x18 + 0x10) = u;
        F<float>(v, i * 0x18 + 0x14) = w;
    };
    uint32_t ab = *(uint32_t*)&a, bb = *(uint32_t*)&b;
    put(0, c, bb, c1u, c1v);
    put(1, ab, bb, u0, v0);
    put(2, ab, d, c2u, c2v);
    put(3, c, bb, c1u, c1v);
    put(4, ab, d, c2u, c2v);
    put(5, c, d, u1, v1);
    *count = 6;
}
RT3_REPLACE(0x564660, Sprite_ToTriangles);

} // namespace rt3

namespace rt3 {

PtrList* __fastcall PtrList_Init(PtrList* l);
void __fastcall Obj_Set8(uint8_t* o, int, uint32_t v);
uint32_t __fastcall Channel_Init(uint8_t* c);

// 0x52da00. Attaches sound channel ch to the object (list +0x81, created on first
// use), puts it at the object's position (+0x1e2) and makes the object its owner.
void __fastcall Obj_AttachChannel(uint8_t* o, int, uint8_t* ch) {
    if (!F<PtrList*>(o, 0x81)) {
        void* p = ((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(o, 0x81) = p ? PtrList_Init((PtrList*)p) : nullptr;
    }
    PtrList_Append(F<PtrList*>(o, 0x81), 0, ch);
    Channel_MoveTo(ch, 0, F<float>(o, 0x1e2), F<float>(o, 0x1e6), F<float>(o, 0x1ea));
    Obj_Set8(ch, 0, (uint32_t)(uintptr_t)o);
}
RT3_REPLACE(0x52da00, Obj_AttachChannel);

// 0x52fd60. Moves all the object's sound channels to its position.
void __fastcall Obj_MoveChannels(uint8_t* o) {
    PtrList* l = F<PtrList*>(o, 0x81);
    if (!l || !l->count) return;
    PtrList_Rewind(l);
    for (uint8_t* ch = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x81)); ch; ch = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x81)))
        Channel_MoveTo(ch, 0, F<float>(o, 0x1e2), F<float>(o, 0x1e6), F<float>(o, 0x1ea));
}
RT3_REPLACE(0x52fd60, Obj_MoveChannels);

// 0x554830. Sets up a positional sound channel: sample `name` on terrain `grid` at
// (x, y, z) with flags a6 (+0x130) / a7 (+0x138) and, if positive, volume `vol`
// (+0x120, scaled +0x124). Returns the channel.
uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6,
                                  int32_t a7, float vol) {
    Channel_Init(ch);
    if (vol > *(const float*)0x5c8598) {
        F<float>(ch, 0x120) = vol;
        F<float>(ch, 0x124) = (float)((double)vol * *(const float*)0x5ddb68);
    }
    F<int32_t>(ch, 0x138) = a7;
    F<uint8_t*>(ch, 4) = grid;
    F<int32_t>(ch, 0x13d) = 0;
    F<int32_t>(ch, 0x130) = a6;
    F<int32_t>(ch, 8) = 0;
    strcpy((char*)ch + 0x10, name);
    Channel_MoveTo(ch, 0, x, y, z);
    return ch;
}
RT3_REPLACE(0x554830, Channel_Setup);

// 0x555740. Re-tints every live particle of the emitter (+4 list) into +0x70.
void __fastcall Emitter_RetintAll(uint8_t* e) {
    for (uint8_t** n = *F<uint8_t***>(e, 4); n; n = (uint8_t**)n[0])
        F<uint32_t>(n[1], 0x70) = Particle_Color(e, 0, n[1], 0) & 0xffffff;
}
RT3_REPLACE(0x555740, Emitter_RetintAll);

} // namespace rt3
