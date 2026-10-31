// Effect dispatch / save compatibility / era economy factors / modifier product in block 0x43 (batch 125). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Obj_SetSlot35b(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
void __fastcall Obj_SetSlot3b5(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
void __fastcall Obj_SetSlot2ab(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
void __fastcall Obj_SetSlot8a(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
int32_t __fastcall Options_Check1bf(void* self);
const char* __fastcall Options_SaveFolder(void* opt);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
int32_t __fastcall Game_EffectiveVersion();

static inline double D(uint32_t a) { return *(const double*)a; }
static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

// 0x4315d0. Applies effect record rec ({id, value, kind byte}) through scope `slot`
// (+0x7fb kinds, +0x7ff keyed): to the company (a2, slots 35b), player (a3, 2ab),
// region (a1, 3b5) or the world (8a), per the effect's flags (table 0x6103fd, 0x6e bytes).
void __fastcall Effects_Apply(uint8_t* fx, int, const int32_t* rec, uint8_t* region, uint8_t* company,
                              uint8_t* player, int32_t slot) {
    int32_t id = rec[0];
    int32_t value = rec[1];
    int32_t add = ((const uint8_t*)rec)[8] == 2 ? 1 : 0;
    if ((id == 0x1c6 || id == 0x1c7 || id == 0x1c8 || (id >= 0xfb && id <= 0x15f)) && value == 0) value = -1;
    int32_t key = 0;
    if (F<int32_t>(fx, 0x7ff + slot * 4))
        key = F<int32_t>(World(), 0x15) * 1000 + (int32_t)(F<uint32_t>(fx, 0x14) % 1000) + (id << 16);
    uint32_t kind = fx[0x7fb + slot];
    if (id >= 0x27 && id <= 0x2a) kind = 1;
    if (kind > 8) return;
    uint8_t flags = ((const uint8_t*)0x6103fd)[id * 0x6e];
    switch (kind) {
    case 0:
        if (company && (flags & 1)) return Obj_SetSlot35b(company, 0, id, value, key, add);
        if (player && (((const uint8_t*)0x6103fd)[id * 0x6e] & 2)) return Obj_SetSlot2ab(player, 0, id, value, key, add);
        if (region && (((const uint8_t*)0x6103fd)[id * 0x6e] & 4)) return Obj_SetSlot3b5(region, 0, id, value, key, add);
        if (((const uint8_t*)0x6103fd)[id * 0x6e] & 8) Obj_SetSlot8a(World(), 0, id, value, key, add);
        return;
    case 1:
        if (flags & 8) Obj_SetSlot8a(World(), 0, id, value, key, add);
        return;
    case 2: case 3: case 4:
        if (company) Obj_SetSlot35b(company, 0, id, value, key, add);
        return;
    case 5:
        if (region) Obj_SetSlot3b5(region, 0, id, value, key, add);
        return;
    default:
        if (player) Obj_SetSlot2ab(player, 0, id, value, key, add);
        return;
    }
}
RT3_REPLACE(0x4315d0, Effects_Apply);

// 0x433260. Whether a save in the save folder can be loaded in this game mode
// (+4): version 0x2583..0x620e98 and the header flags (+0x20, +0x34, +0x35, +0x3b6)
// matching the mode; always for option 0x1bf.
int32_t __fastcall Save_IsCompatible(uint8_t* g, int, const char* name) {
    if (Options_Check1bf(hook::Global<void*>(0x6cec74))) return 1;
    char path[0x12c];
    FormatText(path, (const char*)0x5c9d00, Options_SaveFolder(hook::Global<void*>(0x6cec74)), name, nullptr, nullptr,
               nullptr, nullptr, nullptr, nullptr);
    Stream s = StreamOpen(path, 1, 0x30d40, 1);
    int32_t ver;
    StreamRead(s, &ver, 4, 0, 1);
    int32_t ok = ver >= 0x2583 && ver <= hook::Global<int32_t>(0x620e98) ? 1 : 0;
    int32_t mode = F<int32_t>(g, 4);
    if (mode == 0xc && !((const char*(__cdecl*)(const char*, const char*))0x5a1ff0)(name, (const char*)0x5c9cf8)) {
        ok = 0;
    } else if (ok) {
        uint8_t hdr[0x3c4];
        StreamSkip4(s, 0x2ee0, 1, 1);
        StreamRead(s, hdr, 0x3c2, 0, 1);
        mode = F<int32_t>(g, 4);
        uint8_t a = hdr[0x34], b = hdr[0x20];
        if ((mode == 0xc || mode == 8 || mode == 5 || mode == 0 || mode == 7) && (b || a)) ok = 0;
        else if ((mode == 9 || mode == 1) && (!b || a)) ok = 0;
        else if ((mode == 0xa || mode == 2) && !a) ok = 0;
        if (mode == 6 && hdr[0x35]) ok = 0;
        if (mode == 7 && F<int32_t>(hdr, 0x3b6) <= 1) ok = 0;
        StreamSkip4(s, 0x2ee1, 1, 1);
    }
    StreamClose(s);
    return ok;
}
RT3_REPLACE(0x433260, Save_IsCompatible);

// 0x433bd0. Era economy factors: the base value interpolated from the year table
// (0x5f3980: 21 x {year, value}) for the year +0xd, scaled into +0x65..+0x6d and
// the derived rates +0x71.. (1 - rate) and +0x7d...
void __fastcall Economy_UpdateEraFactors(uint8_t* o) {
    int32_t year = F<uint16_t>(o, 0xd);
    const int32_t* tab = (const int32_t*)0x5f3978; // tab[i*2] = year of entry i-1, tab[i*2+1] = value
    double v;
    int32_t i = 0;
    while (i < 0x15 && !(year <= tab[i * 2 + 2])) ++i;
    if (i == 0x15) {
        v = (float)hook::Global<int32_t>(0x5f3a24);
    } else if (i == 0) {
        v = (float)hook::Global<int32_t>(0x5f3984);
    } else {
        int32_t y0 = tab[i * 2];
        int32_t span = tab[i * 2 + 2] - y0;
        if (span < 1) span = 1;
        float t = (float)((double)(float)(year - y0) / (float)span);
        if (t < *(const float*)0x5c8598 || (double)t > D(0x5c8770)) t = 0.0f;
        double prev = (double)tab[i * 2 + 1];
        float next = (float)tab[i * 2 + 3];
        v = (D(0x5c8770) - t) * prev + (double)next * t;
    }
    float base = (float)v;
    F<float>(o, 0x69) = (float)((double)base * D(0x5c9ed8));
    F<float>(o, 0x65) = (float)((double)base * D(0x5c8888));
    F<float>(o, 0x6d) = (float)((double)base * D(0x5c8680));
    for (int k = 0; k < 3; ++k) {
        float q = (float)(F<float>(o, 0x65 + k * 4) / D(0x5c8a20));
        float w = (float)(((double)q * D(0x5c8da8) + D(0x5c8618)) * D(0x5c9ed0));
        if (w < D(0x5c88a0)) *(uint32_t*)&w = 0x38d1b717;
        if (Game_EffectiveVersion() >= 0x67) w = (float)((double)w * D(0x5c8878));
        if ((double)w > D(0x5c8988)) *(uint32_t*)&w = 0x3e19999a;
        F<float>(o, 0x71 + k * 4) = (float)(D(0x5c8770) - w);
        float r = (float)(D(0x5c8a20) / F<float>(o, 0x65 + k * 4));
        F<float>(o, 0x7d + k * 4) = (float)(((double)r * D(0x5c8680) + D(0x5c88c8)) * D(0x5c9ec8));
    }
}
RT3_REPLACE(0x433bd0, Economy_UpdateEraFactors);

// 0x436590. Product of the percentage modifiers for effect idx (world +0x8a, then
// company +0x35b, player +0x2ab (the company's own when none) and region +0x3b5),
// each at least -99%; at least 0x5c85b8.
RT3_NOINLINE float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region) {
    auto pct = [](int32_t v) { return (double)(v > -99 ? v : -99) / D(0x5c85d8) + D(0x5c8770); };
    float f = (float)pct(F<int32_t>(w, 0x8a + idx * 4));
    uint32_t pl = player;
    if (company && Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, company)) {
        uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, company);
        f = (float)(pct(F<int32_t>(c, 0x35b + idx * 4)) * f);
        if (!player) pl = F<uint32_t>(c, 0x3b);
    }
    if (pl && Collection_IsValid(hook::Global<Collection*>(0x6ceb9c), 0, pl)) {
        uint8_t* p = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ceb9c), 0, pl);
        f = (float)(pct(F<int32_t>(p, 0x2ab + idx * 4)) * f);
    }
    if (region && Collection_IsValid(hook::Global<Collection*>(0x6cfc9c), 0, region)) {
        uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, region);
        f = (float)(pct(F<int32_t>(r, 0x3b5 + idx * 4)) * f);
    }
    if ((double)f < D(0x5c85b8)) *(uint32_t*)&f = 0x3c23d70a;
    return f;
}
RT3_REPLACE(0x436590, Effects_Multiplier);

} // namespace rt3
