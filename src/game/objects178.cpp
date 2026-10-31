// Track kind / blocked flag setters, track colour refresh and the two-tone overlay colour in blocks 0x48..0x4b (batch 182). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Obj_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add);
void __fastcall Trains_Clear1a3(Collection* t);
void __fastcall Track_UpdateColor(uint8_t* o);
uint32_t __stdcall HeatColor(float t);

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

// 0x48b830. Sets the track piece's kind (+0x216); becoming kind 2 counts it in the
// track totals and clears the trains' +0x1a3 flags. Recolours it.
void __fastcall Track_SetKind(uint8_t* o, int, uint8_t kind) {
    if (kind == 2 && o[0x216] != kind) {
        Obj_CountTrack(hook::Global<uint8_t*>(0x6cfca8), 0, o, 1);
        Trains_Clear1a3(hook::Global<Collection*>(0x6cfcbc));
    }
    o[0x216] = kind;
    Track_UpdateColor(o);
}
RT3_REPLACE(0x48b830, Track_SetKind);

// 0x48e310. Sets the track piece's blocked flag (+0x201 bit 6), recolouring on change.
void __fastcall Track_SetBlocked(uint8_t* o, int, uint8_t v) {
    uint8_t f = o[0x201];
    if (((f >> 6) & 1) == v) return;
    o[0x201] = (uint8_t)((((v << 6) ^ f) & 0x40) ^ f);
    Track_UpdateColor(o);
}
RT3_REPLACE(0x48e310, Track_SetBlocked);

// 0x493150. Recolours every track piece.
void __fastcall Tracks_UpdateColor(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Track_UpdateColor((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x493150, Tracks_UpdateColor);

// 0x4bd360. Overlay colour for t in 0..1: the heat ramp in special pixel formats,
// else red fading to green through yellow (opaque, blue 0x33).
uint32_t __stdcall OverlayColor(float t) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11474e)) return HeatColor(t) | 0xff000000;
    if (t < Fl(0x5c8598)) t = 0.0f;
    else if (t > Fl(0x5c85b4)) t = 1.0f;
    float a = (float)(t < D(0x5c8628) ? 1.0 : D(0x5c8770) - (((double)t - D(0x5c8628)) + ((double)t - D(0x5c8628))));
    float b = (float)(t > D(0x5c8628) ? 1.0 : (double)t + t);
    uint32_t r = Ftol((double)a * Fl(0x5ca0ec));
    uint32_t g = Ftol((double)b * Fl(0x5ca0ec));
    return (((r << 8) | g) << 8) | 0xff000033;
}
RT3_REPLACE(0x4bd360, OverlayColor);

} // namespace rt3
