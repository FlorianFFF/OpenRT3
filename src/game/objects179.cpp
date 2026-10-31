// Pending track kind changes / terrain height colour ramp / engine config load in blocks 0x41..0x51 (batch 183). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Obj_UnlinkFromNode(void* o);
void __fastcall Car_LinkToNode(uint8_t* car, int, int32_t id, int32_t side);
void __fastcall Track_AddWear(uint8_t* t);
void __fastcall Track_SetBlocked(uint8_t* o, int, uint8_t v);
void __fastcall Track_SetKind(uint8_t* o, int, uint8_t kind);
uint32_t __stdcall HeatColor(float t);
int32_t __fastcall FileExists(const char* path);
void __fastcall Renderer_ResetSettings(uint8_t* o, int, int32_t kind);
void __fastcall Engine_SaveConfig(uint8_t* e);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline float Fl(uint32_t a) { return *(const float*)a; }
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

// 0x49ad90. Applies and frees the queued track kind changes (+0x1cd list of
// {id, kind, next} nodes).
void __fastcall Tracks_ApplyPendingKinds(Collection* c) {
    while (uint8_t* p = F<uint8_t*>(c, 0x1cd)) {
        uint8_t* next = F<uint8_t*>(p, 5);
        if (Collection_IsValid(c, 0, F<uint32_t>(F<uint8_t*>(c, 0x1cd), 0))) {
            uint8_t* t = (uint8_t*)Collection_Get(c, 0, F<uint32_t>(F<uint8_t*>(c, 0x1cd), 0));
            Track_SetKind(t, 0, F<uint8_t*>(c, 0x1cd)[4]);
        }
        Free(F<uint8_t*>(c, 0x1cd));
        F<uint8_t*>(c, 0x1cd) = next;
    }
}
RT3_REPLACE(0x49ad90, Tracks_ApplyPendingKinds);

// One channel of the terrain ramp for v in 0..1 (rising in three linear pieces).
static inline double Rise(double v) {
    if (v < D(0x5c8c20)) return v * D(0x5c8d68) * D(0x5c8628);
    if (v < D(0x5c88e8)) return (v - D(0x5c8c20)) * D(0x5c8d68) * D(0x5c8618) + D(0x5c8628);
    if (v < D(0x5c8880)) return (v - D(0x5c88e8)) * D(0x5c8d68) * D(0x5c8988) + D(0x5c85a0);
    return Fl(0x5c85b4);
}

// 0x51e1d0. Terrain height overlay colour for t in 0..1 (heat ramp in special pixel
// formats): green rising with t, red with 1 - t, blue peaking off mid-range; the top
// end blends towards white. Alpha 0xcf.
uint32_t __stdcall HeightColor(float t) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11474e)) return HeatColor(t);
    if (t < Fl(0x5c8598)) t = 0.0f;
    else if (t > Fl(0x5c85b4)) t = 1.0f;
    double a = Rise(t);
    double u = (double)Fl(0x5c85b4) - t;
    float r;
    if (u < D(0x5c8c20)) r = (float)(D(0x5c8d68) * u * D(0x5c8628));
    else if (u < D(0x5c88e8)) r = (float)((u - D(0x5c8c20)) * D(0x5c8d68) * D(0x5c8618) + D(0x5c8628));
    else if (u < D(0x5c8880)) r = (float)((u - D(0x5c88e8)) * D(0x5c8d68) * D(0x5c8988) + D(0x5c85a0));
    else r = 1.0f;
    float b = 0.4f;
    double dd = D(0x5c8628) - u;
    if (dd < 0) dd = -dd;
    float df = (float)dd;
    if (dd < D(0x5c86e0)) b = 0.0f;
    else if (df < D(0x5dce88)) b = (float)(((double)df - D(0x5c86e0)) * D(0x5dce80));
    if (u > D(0x5c8880)) {
        double x = (D(0x5c8770) - (D(0x5c8770) - u) * D(0x5c87d8)) * D(0x5c8c20);
        b = (float)(b + x);
        a = a + x;
        if (a > Fl(0x5c85b4)) a = Fl(0x5c85b4);
        if (b > Fl(0x5c85b4)) b = 1.0f;
    }
    uint32_t px = ((uint32_t)Ftol((double)r * Fl(0x5ca0ec)) | 0xffffcf00) << 8;
    px = (px | (uint32_t)Ftol(a * Fl(0x5ca0ec))) << 8;
    return (uint32_t)Ftol((double)b * Fl(0x5ca0ec)) | px;
}
RT3_REPLACE(0x51e1d0, HeightColor);

// 0x51ef20. Loads the renderer settings from data\configuration\engine.cfg (version
// 0x41b; not in a forced screen mode), else resets them to defaults and saves.
void __fastcall Engine_LoadConfig(uint8_t* o) {
    const char* path = (const char*)0x5dcec8;
    if (FileExists(path) && !hook::Global<int32_t>(0x6d4028)) {
        Stream s = StreamOpen(path, 1, 0x30d40, 1);
        int32_t version;
        uint8_t rest[0x1ec];
        StreamRead(s, &version, 4, 0, 1);
        if (version == 0x41b) {
            StreamRead(s, o + 0x11468a, 0x1e4, 0, 1);
            StreamRead(s, rest, 0x1ec, 0, 1);
            StreamClose(s);
            if (F<int32_t>(o, 0x11474a)) hook::Global<uint8_t*>(0x6d4024)[0x11422e] = 0;
            return;
        }
        StreamClose(s);
    }
    Renderer_ResetSettings(o, 0, 0);
    Engine_SaveConfig(o);
}
RT3_REPLACE(0x51ef20, Engine_LoadConfig);

} // namespace rt3
