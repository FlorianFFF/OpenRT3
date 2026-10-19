// Version gates / visit grid setup / particles / font draw / mesh free / 3D provider in blocks 0x46..0x56 (batch 116). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

float __fastcall Net_CommonVersion();
float __fastcall Game_VersionNumber();
int32_t CountHumanPlayers();
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
void __fastcall Render_FixTileSeams(uint8_t* r);
uint8_t* __fastcall VisitGrid_Init(uint8_t* g, int, int32_t n);
void __fastcall VisitGrid_Free(uint8_t* g);
void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12);

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// Version as an integer: (v + 0x5c88a0) x 0x5c85d8 through the exe's _ftol.
static inline int32_t VersionInt(float v) {
    int32_t r;
    __asm {
        fld v
        fadd qword ptr ds:[0x5c88a0]
        fmul qword ptr ds:[0x5c85d8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x46d2b0. Whether every session player (0x6cd8d8) has at least version v.
int32_t __stdcall Net_AllHaveVersion(float v) {
    if (!hook::Global<void*>(0x6cd8d8)) return 1;
    float cv = Net_CommonVersion();
    float need = (float)VersionInt(v);
    return cv < need || cv != cv ? 0 : 1;
}
RT3_REPLACE(0x46d2b0, Net_AllHaveVersion);

// 0x46eec0. Whether a train is in state 1 or 2 (only for sessions at version 0x5c9800+).
int32_t __fastcall Trains_AnyActive() {
    if (hook::Global<void*>(0x6cd8d8)) {
        float cv = Net_CommonVersion();
        if (cv < *(const float*)0x5c9800 || cv != cv) return 0;
    }
    Collection* c = hook::Global<Collection*>(0x6cfcbc);
    if (!c) return 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        c = hook::Global<Collection*>(0x6cfcbc);
        int32_t st = F<int32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0xe8);
        if (st == 1 || st == 2) return 1;
        c = hook::Global<Collection*>(0x6cfcbc);
    }
    return 0;
}
RT3_REPLACE(0x46eec0, Trains_AnyActive);

// 0x482e00. The effective game version integer: the session's common version
// online or with several human players, else our own.
int32_t __fastcall Game_EffectiveVersion() {
    if (!hook::Global<void*>(0x6cd8d8) && !(hook::Global<void*>(0x6cec78) && CountHumanPlayers() > 1))
        return VersionInt(Game_VersionNumber());
    float v = Net_CommonVersion();
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}
RT3_REPLACE(0x482e00, Game_EffectiveVersion);

// 0x527ce0. (Re)builds the visit grid (+0x65) for the terrain size, after fixing
// the overview tile seams; nothing to do when the terrain is unchanged.
void __fastcall Scene_SetupVisitGrid(uint8_t* s, int, uint8_t* t) {
    int32_t rows = Terrain_Rows(t);
    int32_t cols = Terrain_Cols(t);
    if (F<void*>(s, 0x65)) {
        if (hook::Global<int32_t>(0x624d40) == rows && hook::Global<int32_t>(0x624d3c) == cols &&
            hook::Global<int32_t>(0x624d38) == F<int32_t>(t, 0x2121))
            return;
        Render_FixTileSeams(t);
        F<int32_t>(s, 5) = 0;
        int32_t v = F<int32_t>(t, 0x2121);
        uint8_t* old = F<uint8_t*>(s, 0x65);
        hook::Global<int32_t>(0x624d40) = rows;
        hook::Global<int32_t>(0x624d3c) = cols;
        hook::Global<int32_t>(0x624d38) = v;
        if (old) {
            VisitGrid_Free(old);
            ExeDelete(old);
        }
    } else {
        Render_FixTileSeams(t);
        hook::Global<int32_t>(0x624d40) = rows;
        hook::Global<int32_t>(0x624d3c) = cols;
        hook::Global<int32_t>(0x624d38) = F<int32_t>(t, 0x2121);
    }
    int32_t n = cols > rows ? cols : rows;
    s[0x3749] = 1;
    uint8_t* g = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x3e83f);
    F<uint8_t*>(s, 0x65) = g ? VisitGrid_Init(g, 0, n + 1) : nullptr;
    uint32_t* a = (uint32_t*)0x8f2520;
    for (uint32_t k = 0; k < 0x80400; ++k) a[k] = 0;
    *(uint16_t*)(a + 0x80400) = 0;
    uint32_t* b = (uint32_t*)0xb33530;
    for (uint32_t k = 0; k < 0x40200; ++k) b[k] = 0;
    *(uint8_t*)(b + 0x40200) = 0;
}
RT3_REPLACE(0x527ce0, Scene_SetupVisitGrid);

// 0x565c90. Draws particle i (16-byte records +0x18..: scale, size, sprite, ARGB)
// at (x + dx * scale, y + dy * scale), faded by +0xd8 (sprite UVs 0x625a98).
void __fastcall Particles_Draw(uint8_t* p, int, int32_t i, float x, float y, float dx, float dy) {
    uint8_t* e = p + i * 16;
    uint32_t argb = F<uint32_t>(e, 0x24);
    int32_t alpha = (int32_t)(argb >> 24) - p[0xd8];
    if (alpha <= 0) return;
    double cx = (double)dx * F<float>(e, 0x18) + x;
    const float* uv = (const float*)(0x625a98 + F<int32_t>(p, (i + 2) * 16) * 16);
    double cy = (double)dy * F<float>(e, 0x18) + y;
    float size = F<float>(e, 0x1c);
    uint32_t col = (argb & 0xffffff) | ((uint32_t)alpha << 24);
    float bottom = (float)(size + cy), right = (float)(size + cx), top = (float)(cy - size), left = (float)(cx - size);
    Engine_QueueSprite(hook::Global<void*>(0x6d4024), 0, Bits(left), Bits(top), Bits(right), Bits(bottom), 0x3e800000,
                       col, hook::Global<uint32_t>(0xd97ab0), Bits(uv[0]), Bits(uv[1]), Bits(uv[2]), Bits(uv[3]),
                       0x3f800000);
}
RT3_REPLACE(0x565c90, Particles_Draw);

void __fastcall Nop565e20() {}
RT3_REPLACE(0x565e20, Nop565e20);

// 0x565e30. Emitter defaults.
void __fastcall Emitter_Init(uint8_t* e) {
    for (int k = 0; k < 0x13; ++k) F<int32_t>(e, k * 4) = 0;
    F<int32_t>(e, 0x4c) = 1;
    F<int32_t>(e, 0x70) = 0;
    F<int32_t>(e, 0x74) = 0;
    F<int32_t>(e, 0x54) = 0;
    F<int32_t>(e, 0x58) = 0;
}
RT3_REPLACE(0x565e30, Emitter_Init);

// 0x565e80. Starts an emitter: `count` particles over `life` from position `from`
// toward `to`; the colour comes from 0x625bc4.. unless fixed (flag 0x40000000).
void __fastcall Emitter_Start(uint8_t* e, int, int32_t count, const uint32_t* from, const uint32_t* to, uint32_t start,
                              float life) {
    if (!count) return;
    F<uint32_t>(e, 0x44) = start;
    uint32_t fl = F<uint32_t>(e, 0x5c);
    F<int32_t>(e, 0x4c) = count;
    F<int32_t>(e, 0x48) = count;
    F<float>(e, 0x68) = (float)((double)life * count + *(const float*)&start);
    double k = (int8_t)fl < 0 ? *(const float*)0x5cb490 : *(const float*)0x5ddb0c;
    F<float>(e, 0x50) = (float)(k / count);
    if (!(fl & 0x40000000)) {
        F<uint32_t>(e, 0x18) = hook::Global<uint32_t>(0x625bc4);
        F<uint32_t>(e, 0x1c) = hook::Global<uint32_t>(0x625bc8);
        F<uint32_t>(e, 0x20) = hook::Global<uint32_t>(0x625bcc);
        F<uint32_t>(e, 0x24) = hook::Global<uint32_t>(0x625bac);
    }
    for (int k2 = 0; k2 < 3; ++k2) F<uint32_t>(e, 0xc + k2 * 4) = to[k2];
    for (int k2 = 0; k2 < 3; ++k2) F<uint32_t>(e, k2 * 4) = from[k2];
    if (fl & 0x400000) F<float>(e, 0x64) = (float)((double)life + life);
    else F<float>(e, 0x64) = life;
}
RT3_REPLACE(0x565e80, Emitter_Start);

// 0x5661b0. Rotates four points by `angle` about the axis 0xd97b98 (D3DX).
void __stdcall Points_RotateAxis(float angle, float* pts) {
    float m[16] = {};
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    ((void*(__stdcall*)(float*, const void*, float))0x56df7d)(m, (const void*)0xd97b98, angle);
    for (int k = 0; k < 4; ++k) {
        float in[3] = { pts[k * 3], pts[k * 3 + 1], pts[k * 3 + 2] }, out[3];
        ((void*(__stdcall*)(float*, const float*, const float*))0x56cd3c)(out, in, m);
        pts[k * 3] = out[0];
        pts[k * 3 + 1] = out[1];
        pts[k * 3 + 2] = out[2];
    }
}
RT3_REPLACE(0x5661b0, Points_RotateAxis);

// 0x5669d0. Draws text with a D3DX font in the rectangle given as fractions of the
// back buffer (+0x25c7 x +0x25cb); flags 0x200 / 0x400 pick the alignment.
void __fastcall Render_DrawFontText(uint8_t* r, int, const char* text, float x0, float y0, float x1, float y1,
                                   uint32_t colour, void** font, uint32_t flags) {
    float w = (float)F<int32_t>(r, 0x25c7), h = (float)F<int32_t>(r, 0x25cb);
    int32_t rc[4] = { (int32_t)((double)x0 * w), (int32_t)((double)h * y0), (int32_t)((double)w * x1),
                      (int32_t)((double)h * y1) };
    if (!font) return;
    void* f = *font;
    ((int32_t(__stdcall*)(void*))(*(void***)f)[0x14 / 4])(f);
    uint32_t fmt = (flags & 0x200) ? 0x40810 : 0x40811 + ((flags & 0x400) ? 1 : 0);
    ((int32_t(__stdcall*)(void*, const char*, int32_t, int32_t*, uint32_t, uint32_t))(*(void***)f)[0x18 / 4])(
        f, text, -1, rc, fmt, colour);
    ((int32_t(__stdcall*)(void*))(*(void***)f)[0x20 / 4])(f);
}
RT3_REPLACE(0x5669d0, Render_DrawFontText);

// 0x567150. Invalidates the cached state (+0x31/+0x39/+0x3d) of all registered meshes (0xd97bb8).
void __fastcall Meshes_InvalidateAll() {
    PtrList* l = hook::Global<PtrList*>(0xd97bb8);
    if (!l) return;
    PtrList_Rewind(l);
    for (uint8_t* m = (uint8_t*)PtrList_Next(hook::Global<PtrList*>(0xd97bb8)); m;
         m = (uint8_t*)PtrList_Next(hook::Global<PtrList*>(0xd97bb8))) {
        F<int32_t>(m, 0x39) = -1;
        F<int32_t>(m, 0x3d) = -1;
        F<int32_t>(m, 0x31) = -1;
    }
}
RT3_REPLACE(0x567150, Meshes_InvalidateAll);

// 0x5672e0. Frees a model's frames (+0xc, 0x45 bytes; or +0x10, 0x24 bytes) and
// unregisters their meshes. 1 for no model, else 0.
int32_t __stdcall Model_FreeFrames(uint8_t* m) {
    if (!m) return 1;
    for (int32_t i = 0; i < F<int32_t>(m, 0x14); ++i) {
        if (uint8_t* simple = F<uint8_t*>(m, 0x10)) {
            if (F<int32_t>(simple, i * 0x24 + 4) > 0) Free(F<void*>(simple, i * 0x24));
            continue;
        }
        uint8_t* fr = F<uint8_t*>(m, 0xc);
        if (!fr) continue;
        fr += i * 0x45;
        if (F<int32_t>(fr, 0xc) > 0) Free(F<void*>(fr, 4));
        PtrList_RemoveNode(hook::Global<PtrList*>(0xd97bb8), 0, F<PtrListNode*>(F<uint8_t*>(m, 0xc) + i * 0x45, 0x41));
        PtrList* l = hook::Global<PtrList*>(0xd97bb8);
        if (!F<int32_t>(l, 0xc)) {
            PtrList_Destroy(l);
            ExeDelete(l);
            hook::Global<PtrList*>(0xd97bb8) = nullptr;
        }
        fr = F<uint8_t*>(m, 0xc) + i * 0x45;
        F<void*>(fr, 0x41) = nullptr;
        Free(F<void*>(F<uint8_t*>(m, 0xc) + i * 0x45, 0));
        if (void* t = F<void*>(F<uint8_t*>(m, 0xc) + i * 0x45, 0x35)) Free(t);
    }
    if (void* fr = F<void*>(m, 0xc)) {
        Free(fr);
        return 0;
    }
    if (void* s = F<void*>(m, 0x10)) Free(s);
    return 0;
}
RT3_REPLACE(0x5672e0, Model_FreeFrames);

// 0x567530. Of the angles a and a + pi, the one closer (around the circle) to the
// reference angle idx (0x625be0).
float __fastcall Angle_PickFacing(uint8_t idx, float a) {
    const double pi = *(const float*)0x5c8760, twoPi = *(const float*)0x5c8768;
    float ref = ((const float*)0x625be0)[idx];
    double d1 = (double)ref - a;
    if (d1 < 0) d1 = -d1;
    float b = (float)((double)a + pi);
    double d2 = (double)ref - b;
    if (d2 < 0) d2 = -d2;
    if (d1 > pi) {
        d1 = twoPi - d1;
        if (d1 < 0) d1 = -d1;
    }
    if (d2 > pi) {
        d2 = twoPi - d2;
        if (d2 < 0) d2 = -d2;
    }
    return d1 < d2 ? a : b;
}
RT3_REPLACE(0x567530, Angle_PickFacing);

// 0x567bf0. Frees the items of the lists +0x20 and +0x1c and clears them.
static inline void FreeItems(PtrList* l) {
    if (!F<int32_t>(l, 0xc)) return;
    PtrList_Rewind(l);
    for (void* p = PtrList_Next(l); p; p = PtrList_Next(l)) Free(p);
    PtrList_Clear(l);
}
void __fastcall Lists_FreeItems(uint8_t* o) {
    FreeItems(F<PtrList*>(o, 0x20));
    FreeItems(F<PtrList*>(o, 0x1c));
}
RT3_REPLACE(0x567bf0, Lists_FreeItems);

// 0x569380. Opens the Miles 3D provider (+0x28): speaker type (once), room
// support (+0xc) and distance factor.
int32_t __fastcall Sound_Open3DProvider(uint8_t* s) {
    uint32_t* p = F<uint32_t*>(s, 0x28);
    if (!p) return 0;
    if (Import<int32_t(__stdcall*)(uint32_t)>(0x5c83e8)(p[0])) {
        F<void*>(s, 0x28) = nullptr;
        return 0;
    }
    if (hook::Global<int32_t>(0x625c7c) == -2)
        hook::Global<int32_t>(0x625c7c) = Import<int32_t(__stdcall*)(uint32_t)>(0x5c83e0)(F<uint32_t*>(s, 0x28)[0]);
    int32_t room = Import<int32_t(__stdcall*)(uint32_t)>(0x5c83dc)(F<uint32_t*>(s, 0x28)[0]);
    F<uint32_t*>(s, 0x28)[3] = room != -1 ? 1 : 0;
    Import<void(__stdcall*)(uint32_t, uint32_t)>(0x5c83d8)(F<uint32_t*>(s, 0x28)[0], 0x3e806d98);
    return 1;
}
RT3_REPLACE(0x569380, Sound_Open3DProvider);

} // namespace rt3
