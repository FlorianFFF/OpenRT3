// Building cargo lot spawning / world sound restart / heat colour ramp / renderer settings reset in blocks 0x40..0x51 (batch 179). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall SimRand(int32_t lo, int32_t hi);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Obj_SharesRegionKey(void* o, int, uint32_t bid);
void __fastcall Lot_Create(uint32_t src, uint32_t dst, int32_t kind);
void __fastcall Ambient_StopSlots();
void __fastcall Ambient_Start(void*);
void ResetGlobals4686d0();
void* __fastcall Engine_Renderer(void* e);
void __fastcall Channels_PlayAll(uint8_t* o);
double __fastcall Render_ScaleByUnit(uint8_t*, int, float v);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
static inline double Fabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
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

// 0x40fef0. Sends a cargo lot of `kind` from this building to a random other ready
// building that wants it (+0x27e per kind), within a random range (50..400 tiles, or
// anywhere, times `factor`, at least 50) but over 15 away: starting at a random
// index, the best of the random-weighted wants (scaled by k when it has
// stations) wins; a strong want or scanning past a tenth of the buildings ends the
// search. No lot if the two share a region key.
void __fastcall Building_SpawnLot(uint8_t* b, int, int32_t kind, float factor) {
    Collection* blds = hook::Global<Collection*>(0x62b26c);
    int32_t n = Collection_Count(blds);
    int32_t start = SimRand(0, n - 1);
    float best = -1.0f;
    int32_t bestId = -1;
    int32_t r = SimRand(0, 100), range;
    if (r < 0x14) range = 0x32;
    else if (r < 0x1e) range = 0x46;
    else if (r < 0x28) range = 0x64;
    else if (r < 0x32) range = 0x8c;
    else if (r < 0x3c) range = 0xbe;
    else if (r < 0x46) range = 0xfa;
    else if (r < 0x50) range = 0x140;
    else range = r >= 0x5a ? 0x270f : 0x190;
    float fr = (float)((double)(float)range * factor);
    if (Fl(0x5c8780) > fr) fr = 50.0f;
    range = (int32_t)fr;
    int32_t lim = n / 10 + 1;
    bool found = false;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* o = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62b26c), 0,
                                              Collection_IdAt(hook::Global<Collection*>(0x62b26c), 0, (i + start) % n, 0));
        if (F<float>(o, 0x27e + kind * 4) <= Fl(0x5c8598)) continue;
        float oy = (float)Obj_CellY(o);
        double dy = Fabs(oy - Obj_CellY(b));
        float ox = (float)Obj_CellX(o);
        int32_t d = Ftol(Fabs(ox - Obj_CellX(b)) + dy);
        if (d <= 0xf || d > range) continue;
        if (Obj_Flag3d4(o) != 1 || o == b) continue;
        float v = F<float>(o, 0x27e + kind * 4);
        float rnd = (float)SimRand(1, 0x14);
        v = (float)((double)rnd * v);
        if (F<int32_t>(Obj_Region(o), 0xeb) > 0) v = (float)(v * D(0x5c88f8));
        if (Ftol(v) >= Ftol(best)) {
            bestId = F<int32_t>(o, 0x2a4);
            if (!(v < Fl(0x5c8d4c))) {
                found = true;
                break;
            }
            best = v;
        }
        if (i > lim) {
            found = true;
            break;
        }
    }
    if (!found && bestId == -1) return;
    if (!Obj_SharesRegionKey(b, 0, bestId)) Lot_Create(F<uint32_t>(b, 0x2a4), bestId, kind);
}
RT3_REPLACE(0x40fef0, Building_SpawnLot);

// 0x4fe160. Restarts the world's ambient sounds, resets the per-session globals
// when the options say so (+0x14) and replays the renderer's sound channels.
void __fastcall Game_RestartSounds() {
    if (hook::Global<void*>(0x6cec78)) {
        Ambient_StopSlots();
        Ambient_Start(hook::Global<void*>(0x6cec78));
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x14)) ResetGlobals4686d0();
    if (Engine_Renderer(hook::Global<void*>(0x6d4024)))
        Channels_PlayAll((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)));
}
RT3_REPLACE(0x4fe160, Game_RestartSounds);

// One channel of the ramp: rises from k to 1 below 0.5, falls to 0 after.
static inline double Ramp(double v) {
    if (v < D(0x5dce78)) return D(0x5dce70) * v * D(0x5c88e8) + D(0x5c8880);
    if (v < D(0x5c8628)) return 1.0;
    if (v < D(0x5dce68)) return D(0x5c8770) - (v - D(0x5c8628)) * D(0x5dce60);
    return 0.0;
}

// 0x51dfc0. Colour for t in 0..1 (cold to hot) in the renderer's pixel format
// (+0x11474e: 0 = RGB, 4, 3, 2 variants, else grey), alpha 0xcf.
uint32_t __stdcall HeatColor(float t) {
    float v = t;
    if (v < Fl(0x5c8598)) v = Fl(0x5c8598);
    else if (v > Fl(0x5c85b4)) v = Fl(0x5c85b4);
    float a = (float)Ramp(v);
    float u = (float)((double)Fl(0x5c85b4) - v);
    double bv;
    if (u < D(0x5dce78)) bv = (double)u * D(0x5dce70) * D(0x5c88e8) + D(0x5c8880);
    else if (u < D(0x5c8628)) bv = Fl(0x5c85b4);
    else if (u < D(0x5dce68)) bv = D(0x5c8770) - ((double)u - D(0x5c8628)) * D(0x5dce60);
    else bv = Fl(0x5c8598);
    float c = (float)((double)Fl(0x5c85b4) - u);
    double k = Fl(0x5ca0ec);
    int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11474e);
    uint32_t px;
    if (mode == 0) {
        uint32_t ra = Ftol((double)a * k);
        px = ((ra << 8) | (uint32_t)Ftol(bv * k)) << 8;
    } else if (mode == 4) {
        uint32_t ra = Ftol((double)a * k);
        px = (ra << 16) | (uint32_t)Ftol(bv * k);
    } else if (mode == 3) {
        uint32_t g = Ftol(bv * k);
        px = (((g << 8) | g) << 8) | (uint32_t)Ftol((double)a * k);
    } else if (mode == 2) {
        uint32_t g = Ftol(bv * k);
        px = (g << 8) | (uint32_t)Ftol((double)a * k);
    } else {
        uint32_t g = Ftol((double)c * k);
        px = (((g << 8) | g) << 8) | g;
    }
    return px | 0xcf000000;
}
RT3_REPLACE(0x51dfc0, HeatColor);

// 0x51ebc0. Resets the renderer settings (+0x11468a..) to defaults for the screen
// mode (0x6d4028: 0/9 = 1024x768 windowed defaults, else 640x480; 3/4 no colour
// scale) and the session kind `kind` (0 = single player).
void __fastcall Renderer_ResetSettings(uint8_t* o, int, int32_t kind) {
    memset(o + 0x11468a, 0, 0x79 * 4);
    F<uint32_t>(o, 0x1146fa) = 0x3f800000;
    F<uint32_t>(o, 0x1146fe) = 0x3f800000;
    F<uint32_t>(o, 0x114702) = 0x3f800000;
    int32_t mode = hook::Global<int32_t>(0x6d4028);
    if (mode != 0 && mode != 9) {
        F<int32_t>(o, 0x11473e) = 0;
        F<int32_t>(o, 0x11473a) = 0;
        F<int32_t>(o, 0x11470e) = 1;
        F<int32_t>(o, 0x11468a) = 0x280;
        F<int32_t>(o, 0x11468e) = 0x1e0;
        mode = hook::Global<int32_t>(0x6d4028);
        o[0x114696] = (mode == 1 || mode == 3) ? 1 : 0;
        F<int32_t>(o, 0x114706) = 0xc;
        F<int32_t>(o, 0x11470a) = 0x63;
        mode = hook::Global<int32_t>(0x6d4028);
        if (mode == 4 || mode == 3) {
            F<int32_t>(o, 0x1146fa) = 0;
            F<int32_t>(o, 0x1146fe) = 0;
            F<int32_t>(o, 0x114702) = 0;
        }
    } else {
        F<int32_t>(o, 0x11470e) = 0;
        F<int32_t>(o, 0x11468a) = 0x400;
        F<int32_t>(o, 0x11468e) = 0x300;
        o[0x114696] = 1;
        F<int32_t>(o, 0x114706) = 3;
        F<int32_t>(o, 0x11470a) = 0x63;
        F<int32_t>(o, 0x11473e) = 0;
        F<int32_t>(o, 0x11473a) = 1;
    }
    F<int32_t>(o, 0x114692) = 0x20;
    static const uint32_t ones1[] = { 0x1146ec, 0x1146e9, 0x1146ea, 0x1146ef, 0x1146f0, 0x11469b, 0x1146a5, 0x1146a6, 0x1146a7 };
    for (uint32_t off : ones1) o[off] = 1;
    o[0x1146a8] = 0;
    o[0x1146a9] = 1;
    o[0x1146aa] = 1;
    F<int32_t>(o, 0x114697) = 1;
    F<int32_t>(o, 0x114736) = 1;
    F<int32_t>(o, 0x11469c) = 0;
    F<int32_t>(o, 0x1146be) = 0;
    F<int32_t>(o, 0x1146c2) = -1;
    F<int32_t>(o, 0x1146c6) = 0;
    F<int32_t>(o, 0x1146ca) = 0;
    F<int32_t>(o, 0x1146ce) = 0;
    o[0x1146d7] = 0;
    o[0x1146d8] = 0;
    o[0x1146bd] = 0;
    o[0x1146d2] = 0;
    F<float>(o, 0x1146d3) = (float)Render_ScaleByUnit(nullptr, 0, 60.0f);
    o[0x1146a4] = 0;
    o[0x1146a3] = 0;
    o[0x1146a2] = 0;
    o[0x1146a1] = 0;
    static const uint32_t ones2[] = { 0x1146e1, 0x1146e2, 0x1146e3, 0x1146e6, 0x1146e4, 0x1146e5, 0x1146e7,
                                      0x1146ed, 0x1146ee, 0x1146e8, 0x1146eb, 0x1146ab, 0x1146ac, 0x1146ad };
    for (uint32_t off : ones2) o[off] = 1;
    o[0x1146ae] = 0;
    F<int32_t>(o, 0x1146b3) = 2;
    F<int32_t>(o, 0x1146af) = 0;
    o[0x1146b7] = 1;
    o[0x1146b8] = 1;
    F<int32_t>(o, 0x1146f6) = 0;
    F<uint32_t>(o, 0x1146b9) = 0x3f800000;
    o[0x1146f1] = 1;
    F<int32_t>(o, 0x1146f2) = 0;
    F<int32_t>(o, 0x114732) = 0;
    F<int32_t>(o, 0x1146d9) = 0;
    F<int32_t>(o, 0x1146dd) = 0;
    F<int32_t>(o, 0x114742) = 0;
    hook::Global<int32_t>(0x6d4028) = 0;
    F<int32_t>(o, 0x114712) = 1;
    if (kind == 0) {
        F<int32_t>(o, 0x114716) = 1;
        F<int32_t>(o, 0x11472a) = 1;
        F<int32_t>(o, 0x11472e) = 0;
    } else if (kind == 2 || kind == 1) {
        o[0x114696] = 0;
        F<int32_t>(o, 0x114716) = 0;
        F<int32_t>(o, 0x11472a) = 0;
        F<int32_t>(o, 0x11472e) = 0;
    }
    F<int32_t>(o, 0x11471a) = 0;
    F<int32_t>(o, 0x114722) = 0;
    F<int32_t>(o, 0x11471e) = 1;
    F<int32_t>(o, 0x114726) = 0x96;
}
RT3_REPLACE(0x51ebc0, Renderer_ResetSettings);

} // namespace rt3
