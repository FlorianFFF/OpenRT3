// Functions across blocks 0x43..0x48 (batch 40). See objects.cpp for conventions.
#include "hook/hook.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Table_CountClear82b(Collection* t);
void* __fastcall Collection_RecordAtList(Collection* t, int, uint32_t pos, int list);
float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m);

// 0x436c70. Recognises one specific scenario (map 384x448, 65 regions with
// region 1 named 0x5c9f48, 22 open slots and slot 7 kind 6) when v == 16.
int32_t __fastcall World_IsSpecialScenario(void*, int, int32_t v) {
    if (v != 0x10) return 0;
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if (F<int32_t>(map, 0x2155) != 0x180 || F<int32_t>(map, 0x2159) != 0x1c0) return 0;
    Collection* regions = hook::Global<Collection*>(0x62bae0);
    if (Collection_Count(regions) != 0x41) return 0;
    if (Stricmp((const char*)Collection_Get(regions, 0, 1) + 0x356, (const char*)0x5c9f48) != 0) return 0;
    Collection* slots = hook::Global<Collection*>(0x62be18);
    if (Table_CountClear82b(slots) != 0x16) return 0;
    return F<uint8_t>(Collection_RecordAtList(slots, 0, 7, 0), 0x7ef) == 6 ? 1 : 0;
}
RT3_REPLACE(0x436c70, World_IsSpecialScenario);

// 0x449e90. Clamps the cell rectangle to the map; 1 if it is then non-empty.
static int32_t __fastcall Map_ClampRectInclusive_Impl(uint8_t* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1) {
    if (*x0 < 0) *x0 = 0;
    if (*y0 < 0) *y0 = 0;
    int32_t w = F<int32_t>(m, 0x2155), h = F<int32_t>(m, 0x2159);
    *x1 = w < *x1 ? w : *x1;
    *y1 = h < *y1 ? h : *y1;
    if (*x0 < 0 || *y0 < 0 || *x1 > w || *y1 > h) return 0;
    return *x1 >= *x0 && *y1 >= *y0 ? 1 : 0;
}
// 0x44d410 keeps using ECX (the map) after the call.
__declspec(naked) int32_t __stdcall Map_ClampRectInclusive(int32_t*, int32_t*, int32_t*, int32_t*) { RT3_KEEP_ECX_EDX(Map_ClampRectInclusive_Impl, 4) }
RT3_REPLACE(0x449e90, Map_ClampRectInclusive);

// 0x450490. If the cell's key list has `key` (or any entry for key 0):
// world +0x66ba | 0x2c000000 (a colour); else 0.
uint32_t __fastcall Map_CellKeyColour(uint8_t* m, int, int32_t x, int32_t y, int32_t key) {
    uint8_t* cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7;
    const uint8_t* e = F<uint8_t*>(cell, 0xef);
    for (int32_t i = 0; i < F<int32_t>(cell, 0xeb); ++i, e += 6)
        if (F<uint16_t>((void*)e, 0) == key || key == 0)
            return F<uint32_t>(hook::Global<uint8_t*>(0x6cec78), 0x66ba) | 0x2c000000;
    return 0;
}
RT3_REPLACE(0x450490, Map_CellKeyColour);

// 0x457eb0. +0x2b9 through the matrix +0x18e, scaled by +0x27d then by +0x281.
void __fastcall Obj_ScaledPoint(uint8_t* o, int, float* out) {
    float r[3];
    float s = F<float>(o, 0x27d);
    float* t = Vec3_Transform((float*)(o + 0x2b9), 0, r, (const float*)(o + 0x18e));
    float a2 = (float)((double)s * t[2]);
    float a1 = (float)((double)s * t[1]);
    float a0 = (float)((double)s * t[0]);
    float s2 = F<float>(o, 0x281);
    out[2] = (float)((double)a2 * s2);
    out[1] = (float)((double)a1 * s2);
    out[0] = (float)((double)a0 * s2);
}
RT3_REPLACE(0x457eb0, Obj_ScaledPoint);

void __fastcall Nop487630(void*) {}
RT3_REPLACE(0x487630, Nop487630);
int32_t __fastcall Return0_487670(void*, int, int32_t) { return 0; }
RT3_REPLACE(0x487670, Return0_487670);
void __fastcall Nop487680(void*, int, int32_t) {}
RT3_REPLACE(0x487680, Nop487680);

// 0x487690. Station year roll-over: this year's four totals (+0x45..) and
// two 53-entry cargo arrays (+0x55, +0x129) are added to the sums and cleared.
void __fastcall Station_RollStats(uint8_t* o) {
    auto roll = [&](uint32_t cur, uint32_t sum) {
        float t = F<float>(o, cur);
        F<float>(o, cur) = 0.0f;
        F<float>(o, sum) = (float)((double)t + F<float>(o, sum));
    };
    roll(0x45, 0x1fd);
    roll(0x49, 0x201);
    roll(0x4d, 0x205);
    roll(0x51, 0x209);
    for (int32_t i = 0; i < 0x35; ++i) {
        roll(0x55 + i * 4, 0x20d + i * 4);
        roll(0x129 + i * 4, 0x2e1 + i * 4);
    }
}
RT3_REPLACE(0x487690, Station_RollStats);

// 0x487730. As 0x424050 for the slots +0x3b5 with last key +0xd4d.
void __fastcall Obj_SetSlot3b5(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add) {
    if (key == F<int32_t>(o, 0xd4d)) return;
    if (add) F<int32_t>(o, 0x3b5 + idx * 4) += value;
    else F<int32_t>(o, 0x3b5 + idx * 4) = value;
    if (key) F<int32_t>(o, 0xd4d) = key;
}
RT3_REPLACE(0x487730, Obj_SetSlot3b5);

// 0x487820..0x487920. Station stat bookkeeping: adds to this year's totals
// (and per-cargo slot), and to the company's per-region block (+0 is the region).
static inline void AddF(uint8_t* p, uint32_t off, float a) { F<float>(p, off) = (float)((double)a + F<float>(p, off)); }
void __fastcall Station_AddStat45(uint8_t* o, int, float a, uint8_t* company, int32_t cargo) {
    AddF(o, 0x45, a);
    AddF(o, 0x55 + cargo * 4, a);
    if (!company) return;
    uint8_t* b = company + F<int32_t>(o, 0) * 0x38d;
    AddF(b, 0x1da0, a);
    AddF(b, 0x1ddc + cargo * 4, a);
}
RT3_REPLACE(0x487820, Station_AddStat45);
void __fastcall Station_AddStat49(uint8_t* o, int, float a, uint8_t* company, int32_t cargo) {
    AddF(o, 0x49, a);
    AddF(o, 0x129 + cargo * 4, a);
    if (!company) return;
    uint8_t* b = company + F<int32_t>(o, 0) * 0x38d;
    AddF(b, 0x1da8, a);
    AddF(b, 0x1f84 + cargo * 4, a);
}
RT3_REPLACE(0x487880, Station_AddStat49);
void __fastcall Station_AddStat4d(uint8_t* o, int, float a, uint8_t* company) {
    AddF(o, 0x4d, a);
    if (company) AddF(company + F<int32_t>(o, 0) * 0x38d, 0x1db0, a);
}
RT3_REPLACE(0x4878e0, Station_AddStat4d);
void __fastcall Station_AddStat51(uint8_t* o, int, float a, uint8_t* company) {
    AddF(o, 0x51, a);
    if (company) AddF(company + F<int32_t>(o, 0) * 0x38d, 0x1db8, a);
}
RT3_REPLACE(0x487920, Station_AddStat51);

} // namespace rt3
