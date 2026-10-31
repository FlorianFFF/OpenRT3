// Track overlay colours (owner / signals / slope / wear) and train owner colours in blocks 0x48..0x4b (batch 181). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Track_ColorSlope(uint8_t* o);
void __fastcall Track_ColorWear(uint8_t* o, int, float full);
void __fastcall Obj_SetArgb(uint8_t* o, int, uint8_t a, uint8_t r, uint8_t g, uint8_t b);
int32_t __fastcall Obj_Get21(void* o);
uint32_t __stdcall HeatColor(float t);
void __fastcall Obj_ForCityChain(void* o, int, int32_t a, int32_t b, int32_t c);

static inline float Fl(uint32_t a) { return *(const float*)a; }

// 0x48b660. Sets a track piece's colour: the slope overlay when it is plain track
// and no overlay is forced (nodes +0xe9); red for blocked pieces (world +0x4ae3,
// +0x201 bit 6); with owner colours on (option +0x2bf: 2, or 1 in the track view
// 0x6d0818 mode 12) by owner (the player's red tones, others' pale) and kind
// (+0x44 double, +0x218 == 4 electric); else grey (dimmed for +0x216 == 1 with
// option +0x98).
void __fastcall Track_UpdateColor(uint8_t* o) {
    uint8_t* opts = hook::Global<uint8_t*>(0x6cec74);
    int32_t owners;
    int32_t m = F<int32_t>(opts, 0x2bf);
    if (!m) owners = 0;
    else if (m == 2) owners = 1;
    else {
        uint8_t* view = hook::Global<uint8_t*>(0x6d0818);
        owners = view && F<int32_t>(view, 0x8c) == 0xc ? 1 : 0;
    }
    uint8_t kind = o[0x216];
    if (!kind && !F<int32_t>(hook::Global<uint8_t*>(0x6cfca8), 0xe9)) {
        Track_ColorSlope(o);
        return;
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4ae3) && (o[0x201] & 0x40)) {
        Obj_SetArgb(o, 0, 0xff, 0x5f, 0x5f, 0xff);
        return;
    }
    if (owners) {
        bool elec = o[0x218] == 4;
        bool mine = F<uint16_t>(o, 0x22e) == (uint32_t)Obj_Get21(hook::Global<void*>(0x6cec78));
        bool dbl = o[0x44] != 0;
        if (mine) {
            if (dbl) {
                if (elec) Obj_SetArgb(o, 0, 0xff, 0x7f, 0x7f, 0xff);
                else Obj_SetArgb(o, 0, 0xff, 0x5f, 0x5f, 0xdf);
            } else if (elec) Obj_SetArgb(o, 0, 0xff, 0x20, 0x28, 0x88);
            else Obj_SetArgb(o, 0, 0xff, 0, 0, 0x5f);
        } else if (dbl) {
            if (elec) Obj_SetArgb(o, 0, 0xff, 0xff, 0xcf, 0xcf);
            else Obj_SetArgb(o, 0, 0xff, 0xff, 0xbf, 0xbf);
        } else if (elec) Obj_SetArgb(o, 0, 0xff, 0xef, 0x7f, 0x7f);
        else Obj_SetArgb(o, 0, 0xff, 0xdf, 0x6f, 0x6f);
        return;
    }
    if (kind == 1 && F<int32_t>(opts, 0x98)) Obj_SetArgb(o, 0, 0x70, 0x7f, 0x7f, 0x7f);
    else Obj_SetArgb(o, 0, 0xff, 0x7f, 0x7f, 0x7f);
}
RT3_REPLACE(0x48b660, Track_UpdateColor);

// 0x492fc0. Colours every track piece by slope.
void __fastcall Tracks_ColorSlope(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Track_ColorSlope((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x492fc0, Tracks_ColorSlope);

// 0x493080. Colours every track piece by wear relative to the most worn (at least
// k, else 12).
void __fastcall Tracks_ColorWear(Collection* c) {
    float top = 0.0f;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        float v = F<float>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0x22a);
        if (!(top > v)) top = F<float>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0x22a);
    }
    if (Fl(0x5c8788) > top) top = 12.0f;
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Track_ColorWear((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, top);
}
RT3_REPLACE(0x493080, Tracks_ColorWear);

// 0x4b29e0. Colours the trains: hot for the company's, cold for the rest.
void __fastcall Trains_ColorOwner(Collection* c, int, int32_t company) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        uint32_t col = HeatColor((float)(F<int32_t>(e, 0x51) == company ? 1 : 0));
        Obj_ForCityChain(e, 0, col >> 16, col >> 8, col);
    }
}
RT3_REPLACE(0x4b29e0, Trains_ColorOwner);

} // namespace rt3
