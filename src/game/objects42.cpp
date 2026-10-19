// Functions across blocks 0x40..0x49 (batch 45). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Obj_Has78(void* o);
int32_t ClockMs();
float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m);
float* __fastcall Vec3_Normalize(float* v);
void __fastcall Vec3_CrossInPlace(float* a, int, const float* b);
double __fastcall Vec3_Length(const float* v);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
void* __fastcall Pool_Alloc(int32_t* p);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);

static inline void AddInto(uint8_t* o, uint32_t sum, uint32_t cur) {
    F<float>(o, sum) = (float)((double)F<float>(o, sum) + F<float>(o, cur));
}
// Shifts five 10-entry histories (stride 0x28 from `first`) by one and clears entry 0.
static void ShiftHistories(uint8_t* o, uint32_t first) {
    for (int k = 9; k >= 1; --k)
        for (int a = 0; a < 5; ++a) F<uint32_t>(o, first + a * 0x28 + k * 4) = F<uint32_t>(o, first + a * 0x28 + (k - 1) * 4);
    for (int a = 0; a < 5; ++a) F<uint32_t>(o, first + a * 0x28) = 0;
}

// 0x40c9a0. Building year roll-over: adds this year's five stats (+0x2c0,
// +0x2e8, ... first entries) to the totals +0x2ac.. and shifts the histories.
void __fastcall Building_RollStats(uint8_t* o) {
    AddInto(o, 0x2ac, 0x2c0);
    F<float>(o, 0x2b0) = (float)((double)F<float>(o, 0x2e8) + F<float>(o, 0x2b0));
    AddInto(o, 0x2b4, 0x310);
    AddInto(o, 0x2b8, 0x338);
    AddInto(o, 0x2bc, 0x360);
    ShiftHistories(o, 0x2c0);
}
RT3_REPLACE(0x40c9a0, Building_RollStats);

// 0x47d700. Same for the site stats (+0x4c4..; the fifth history +0x564 is an integer).
void __fastcall Site_RollStats(uint8_t* o) {
    AddInto(o, 0x4b0, 0x4c4);
    F<int32_t>(o, 0x4c0) += F<int32_t>(o, 0x564);
    F<float>(o, 0x4b4) = (float)((double)F<float>(o, 0x4ec) + F<float>(o, 0x4b4));
    AddInto(o, 0x4b8, 0x514);
    AddInto(o, 0x4bc, 0x53c);
    ShiftHistories(o, 0x4c4);
}
RT3_REPLACE(0x47d700, Site_RollStats);

// 0x4348e0. Whether the UI is busy (blocks game input). Some states keep it
// busy for another 1.5 s (deadline 0x62be38).
int32_t UiBusy() {
    if (hook::Global<int32_t>(0x6d4000)) return 1;
    bool stamp = false;
    if (F<int32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc64) > 0) {
        stamp = true;
    } else {
        int32_t screen = F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x8c);
        if (screen == 0xc) {
            if (F<uint32_t>(hook::Global<uint8_t*>(0x6d4018), 0xa8c) & 0x400) stamp = true;
            else if (void* d = hook::Global<void*>(0x6d1a8c)) stamp = Obj_Has78(d) != 0;
        }
        if (!stamp && screen == 6) {
            uint8_t* w = hook::Global<uint8_t*>(0x6d1720);
            if (w && F<int32_t>(w, 0x7c)) stamp = true;
        }
    }
    if (stamp) {
        hook::Global<int32_t>(0x62be38) = ClockMs() + 0x5dc;
        return 1;
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x62be68), 0x38e)) return 1;
    if (hook::Global<int32_t>(0x62be38) && hook::Global<int32_t>(0x62be38) > ClockMs()) return 1;
    hook::Global<int32_t>(0x62be38) = 0;
    return 0;
}
RT3_REPLACE(0x4348e0, UiBusy);

// 0x458090. For the unit direction v: dot of transformed +0x2d1 with v, and
// lengths of (transformed +0x2b9) x v and (transformed +0x2c5) x v.
void __fastcall Obj_AxisTerms(uint8_t* o, int, float vx, float vy, float vz, float* dot, float* lenC, float* lenA) {
    const float* m = (const float*)(o + 0x18e);
    float a[3], b[3], c[3], v[3] = { vx, vy, vz };
    Vec3_Transform((float*)(o + 0x2b9), 0, a, m);
    Vec3_Transform((float*)(o + 0x2d1), 0, b, m);
    Vec3_Transform((float*)(o + 0x2c5), 0, c, m);
    Vec3_Normalize(v);
    Vec3_CrossInPlace(a, 0, v);
    Vec3_CrossInPlace(c, 0, v);
    *lenA = (float)Vec3_Length(a);
    double d = (double)b[2] * v[2] + (double)b[1] * v[1];
    *dot = (float)(d + (double)b[0] * v[0]);
    *lenC = (float)Vec3_Length(c);
}
RT3_REPLACE(0x458090, Obj_AxisTerms);

// 0x464260. Gives every AI player `amount` cash, or (amount 0) zeroes it.
void __fastcall Players_AdjustAiCash(int32_t amount) {
    Collection* players = hook::Global<Collection*>(0x6ceb9c);
    if (!players) return;
    for (int32_t i = 0; i < Table_CountField4(players); ++i) {
        if (F<int32_t>(Table_NthField4(players, 0, i), 0x1e1)) continue;
        double v = amount ? (double)amount : -F<double>(Table_NthField4(players, 0, i), 0x154);
        Obj_AddDouble154(Table_NthField4(players, 0, i), 0, (int32_t)v);
    }
}
RT3_REPLACE(0x464260, Players_AdjustAiCash);

// 0x469930. Appends a new 0xa4-byte message node (pool +0x8f20) {a, b, c, text}
// to the list +0x8f28. QUIRK: an exhausted pool writes through null.
int32_t __fastcall Obj_AddMessage(uint8_t* o, int, int32_t a, int32_t b, int32_t c, const char* text) {
    uint32_t* n = (uint32_t*)Pool_Alloc(F<int32_t*>(o, 0x8f20));
    for (int k = 0; k < 0x29; ++k) n[k] = 0;
    n[1] = c;
    n[2] = b;
    n[5] = a;
    char* d = (char*)n + 0x18;
    while ((*d++ = *text++) != 0) {}
    void** l = F<void**>(o, 0x8f28);
    if (void** tail = (void**)l[1]) {
        n[0] = (uint32_t)tail[0];
        tail[0] = n;
        ((int32_t*)l)[2]++;
        l[1] = n;
    } else {
        n[0] = (uint32_t)l[0];
        l[0] = n;
        ((int32_t*)l)[2]++;
        if (!l[1]) l[1] = n;
    }
    return 1;
}
RT3_REPLACE(0x469930, Obj_AddMessage);

// 0x491fd0. Removes track piece t (+0x202) from the per-cell lists +0x8c
// (column -> u16-sorted cells -> id list); 1 if removed.
int32_t __fastcall Map_RemoveTrackRef(uint8_t* o, int, uint8_t* t) {
    float x, y;
    Obj_CenterCell(t, 0, &x, &y);
    int32_t xi = (int32_t)x;
    uint16_t yi = (uint16_t)(int32_t)y;
    uint8_t* e = F<uint8_t**>(o, 0x8c)[(uint16_t)xi];
    for (; e; e = F<uint8_t*>(e, 6)) {
        uint16_t key = F<uint16_t>(e, 0);
        if (key > yi) return 0;
        if (key == yi) break;
    }
    if (!e) return 0;
    uint8_t* prev = nullptr;
    for (uint8_t* n = F<uint8_t*>(e, 2); n; prev = n, n = F<uint8_t*>(n, 4)) {
        if (F<int32_t>(n, 0) != F<int32_t>(t, 0x202)) continue;
        if (prev) F<uint8_t*>(prev, 4) = F<uint8_t*>(n, 4);
        else F<uint8_t*>(e, 2) = F<uint8_t*>(n, 4);
        ((void(__cdecl*)(void*))0x53b080)(n);
        return 1;
    }
    return 0;
}
RT3_REPLACE(0x491fd0, Map_RemoveTrackRef);

} // namespace rt3
