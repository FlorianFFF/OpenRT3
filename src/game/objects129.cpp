// Fine-bit lists / playlist tick / slot setup / cargo stock / collection load / night factor / rounded cost in blocks 0x41..0x48 (batch 132). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
void __fastcall Playlist_PlayCurrent(uint8_t* p);
void __fastcall Obj_AssignRandomSlots(uint8_t* o);
int32_t __fastcall Obj_YearReached(void* o);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
int32_t __fastcall Obj_LoadArrays(uint8_t* o, int, Stream s);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);

static inline double D(uint32_t a) { return *(const double*)a; }

// 0x414270. Sets fine bits (grid 2, with block refresh) for a -1-terminated list
// of packed cells (x | y << 16, 15 bits each).
void __stdcall Map_SetFineBitList(const uint32_t* cells, int32_t on) {
    for (; *cells != 0xffffffff; ++cells)
        Map_SetFineBit(hook::Global<uint8_t*>(0x62c120), 0, *cells & 0x7fff, (*cells >> 16) & 0x7fff, on, 2, 1);
}
RT3_REPLACE(0x414270, Map_SetFineBitList);

// 0x468960. Plays the current track when the playlist is active (+0x110 == 1).
void __fastcall Playlist_Tick(uint8_t* p) {
    if (F<int32_t>(p, 0x110) == 1) Playlist_PlayCurrent(p);
}
RT3_REPLACE(0x468960, Playlist_Tick);

// 0x47be50. Takes over setup src's settings (+0x14, +0x3b2, +0x3ba words, except
// in multiplayer, and the +0x20 byte) and assigns its random slots.
void __fastcall Obj_ApplySetup(uint8_t* o, int, uint8_t* src) {
    if (!src || !src[0]) return;
    if (!hook::Global<void*>(0x6cd8d8)) {
        F<uint16_t>(o, 0x77) = F<uint16_t>(src, 0x14);
        F<uint16_t>(o, 0x79) = F<uint16_t>(src, 0x3b2);
        F<uint16_t>(o, 0x7b) = F<uint16_t>(src, 0x3ba);
    }
    o[0xc5] = src[0x20];
    Obj_AssignRandomSlots(src);
}
RT3_REPLACE(0x47be50, Obj_ApplySetup);

// 0x47e240. For cargo i: the stock (+0x103) summed over o's cells (+0x38 x 12
// bytes at +0x34, {x, y}) and the quota of o's region city; 0 / 1.0 and 0 when
// the cargo is not available yet.
int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota) {
    if (!Obj_YearReached(Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, i))) {
        if (stock) *stock = 0.0f;
        if (quota) *quota = 1.0f;
        return 0;
    }
    void* owner = F<uint32_t>(o, 4) ? Collection_Get(hook::Global<Collection*>(0x62b26c), 0, F<uint32_t>(o, 4)) : nullptr;
    float sum = 0.0f;
    float q = (float)City_CargoQuota(Obj_Region(owner), 0, i);
    int32_t n = F<int32_t>(o, 0x38);
    if (n > 0) {
        uint8_t* m = hook::Global<uint8_t*>(0x62c120);
        const int32_t* c = F<const int32_t*>(o, 0x34);
        for (; n; --n, c += 3) {
            uint8_t* city = F<uint8_t*>(m, 0x2129) + (c[1] * F<int32_t>(m, 0x2145) + c[0]) * 0x1d7;
            sum = (float)((double)sum + F<float>(city, 0x103 + i * 4));
        }
    }
    if (stock) *stock = sum;
    if (quota) *quota = q;
    return 1;
}
RT3_REPLACE(0x47e240, Obj_CargoStockAndQuota);

// 0x481210. Loads the collection (chunks 0x4a39..0x4a3b) and each element's arrays;
// old saves (version 0x4a39 with 0x465-byte records) get their +0x2ba block moved
// to +0x462 and the new fields cleared.
int32_t __fastcall Collection_LoadWithArrays(Collection* c, int, Stream s, int32_t) {
    int32_t ver;
    StreamRead(s, &ver, 4, 0, 1);
    Collection_Load(c, 0, s, ver == 0x4a39 ? 0x465 : 0);
    int32_t r = StreamSkip4(s, 0x4a3a, 0, 0);
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (ver == 0x4a39) {
            ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(e + 0x462, e + 0x2ba, 0x168);
            for (int k = 0; k < 0x35; ++k) F<int32_t>(e, 0x2ba + k * 4) = 0;
            for (int k = 0; k < 0x35; ++k) F<int32_t>(e, 0x38e + k * 4) = 0;
            for (int k = 0; k < 0x43; ++k) e[0x5ca + k] = 0;
        }
        Obj_LoadArrays(e, 0, s);
    }
    return StreamSkip4(s, 0x4a3b, 0, 0) + r;
}
RT3_REPLACE(0x481210, Collection_LoadWithArrays);

// 0x482690. Night factor (0 day .. 1 night) for the renderer clock: full night
// before 4:20 and after 20:51, ramps through dawn (to 6:51) and dusk (from 18:25).
float __cdecl Sky_NightFactor() {
    void* eng = hook::Global<void*>(0x6d4024);
    if (!eng || !Engine_Renderer(eng)) return *(const float*)0x5c8598;
    int32_t h, m, s;
    Clock_SplitD((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, &h, &m, &s);
    float t;
    __asm {
        fild m
        fdiv qword ptr ds:[0x5c88a8]
        fild s
        fdiv qword ptr ds:[0x5c9f30]
        faddp st(1), st
        fiadd h
        fstp t
    }
    float r;
    if (t < D(0x5c8978)) {
        if (t < D(0x5ced38)) return 1.0f;
        if (t < D(0x5cecf0)) r = (float)((D(0x5cecf0) - t) / D(0x5ced30) * D(0x5c88e8) + D(0x5c8880));
        else if (t < D(0x5ced28)) r = (float)((D(0x5ced28) - t) / D(0x5ced20) * D(0x5c8880));
        else return 0.0f;
    } else {
        if (t < D(0x5ced18)) return 0.0f;
        if (t < D(0x5ced10)) r = (float)((t - D(0x5ced18)) / D(0x5ced08) * D(0x5c88c8));
        else if (t < D(0x5ced00)) r = (float)((t - D(0x5ced10)) / D(0x5cecf8) * D(0x5c8680) + D(0x5c88c8));
        else return 1.0f;
    }
    if (*(const float*)0x5c8598 > r) return 0.0f;
    if (*(const float*)0x5c85b4 < r) return 1.0f;
    return r;
}
RT3_REPLACE(0x482690, Sky_NightFactor);

// 0x4879f0. o's cost (+0x39) scaled by effect 0x1c5 for company and o's region
// (+0), rounded to the nearest 1000.
float __fastcall Obj_RoundedCost(uint8_t* o, int, uint32_t company) {
    float m = Effects_Multiplier(hook::Global<uint8_t*>(0x6cec78), 0, 0x1c5, company, 0, F<uint32_t>(o, 0));
    float base = (float)F<int32_t>(o, 0x39);
    int32_t v;
    __asm {
        fld m
        fmul base
        mov eax, 0x5a10d0
        call eax
        mov v, eax
    }
    return (float)((v + 500) / 1000 * 1000);
}
RT3_REPLACE(0x4879f0, Obj_RoundedCost);

} // namespace rt3
