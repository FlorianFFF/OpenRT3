// Functions across blocks 0x41..0x4a (batch 51). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_trains     = hook::Global<Collection*>(0x6cfcbc);
static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);

int32_t __fastcall Train_State1Or2(uint8_t* t);
int32_t __fastcall StreamTell(Stream s);
int32_t ClockMs();
const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                         int32_t f, int32_t g, int32_t h, int32_t i);
void __fastcall Obj_SetElementFlag1d8(void* o, int32_t mode, int32_t idx);
int32_t __fastcall Node_GetLink(void* n, int, int32_t first);
void* __fastcall Array33_Current(void* a);

static inline int ExeSprintf(char* d, const char* f, int32_t v) { return ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(d, f, v); }

// 0x41bee0. Scales a loaded car's value +0x3a0 by its cargo's price factor
// (world table for mapped cargo, else the cargo type's +0x42); halfway
// towards 1 while the train is in state 1 or 2.
void __fastcall Car_ApplyPriceFactor(uint8_t* car) {
    uint32_t ct = F<uint32_t>(car, 0x38c);
    if (!ct || !(F<float>(car, 0x398) > *(const float*)0x5c8598)) return;
    uint8_t* types = hook::Global<uint8_t*>(0x62ba8c);
    int32_t map = F<int32_t>(types, 0x9a + ct * 4);
    float v = map != -1 ? F<float>(hook::Global<uint8_t*>(0x6cec78), 0x71 + map * 4)
                        : F<float>(Collection_Get((Collection*)types, 0, ct), 0x42);
    int32_t tid = F<int32_t>(car, 0x390);
    uint8_t* train = tid != -1 ? (uint8_t*)Collection_Get(g_trains, 0, tid) : nullptr;
    if (Train_State1Or2(train))
        F<float>(car, 0x3a0) = (float)(((double)v + *(const double*)0x5c8770) / *(const double*)0x5c85d0 * F<float>(car, 0x3a0));
    else
        F<float>(car, 0x3a0) = (float)((double)v * F<float>(car, 0x3a0));
}
RT3_REPLACE(0x41bee0, Car_ApplyPriceFactor);

// 0x442660. Debug log line (string 0x198) with the KB written to stream s
// since the last call and the elapsed ms since 0x62bed0.
void __fastcall Save_LogProgress(Stream s, const char* label) {
    if (!s && !label) return;
    char kb[0x20], ms[0x20], line[0x1ec];
    int32_t pos = StreamTell(s);
    ExeSprintf(kb, (const char*)0x5ca350, (pos - hook::Global<int32_t>(0x62c11c)) / 1024);
    ExeSprintf(ms, (const char*)0x5ca348, ClockMs() - hook::Global<int32_t>(0x62bed0));
    FormatText(line, GetString(0x198), kb, ms, label, nullptr, nullptr, nullptr, nullptr, nullptr);
    DebugLog(line, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234);
    hook::Global<int32_t>(0x62c11c) = pos;
}
RT3_REPLACE(0x442660, Save_LogProgress);

// Flags entry of `blk` in the current block iteration (0x6acd50), then calls
// its virtual slot 4 (flag 0x800 set) or 3 (unless 0x2000).
static void UpdateBlockEntry(uint8_t* blk, bool set) {
    uint8_t* it = hook::Global<uint8_t*>(0x6acd50);
    uint8_t* e = it + 0x10;
    for (int32_t i = 0; i < F<int32_t>(it, 0xc); ++i, e += 0xc) {
        if (F<uint8_t*>(e, 0) != blk) continue;
        uint32_t& f = F<uint32_t>(e, 4);
        f = set ? (f | 0x800) : (f & ~0x800u);
        if (f & 0x800) ((void(__thiscall*)(void*))(*(void***)blk)[0x10 / 4])(blk);
        else if (!(f & 0x2000)) ((void(__thiscall*)(void*))(*(void***)blk)[0xc / 4])(blk);
        return;
    }
}

// 0x45a000. Clears flag 0x800 of the four blocks +0xa8.. and sets it on the four +0xb8..
void __fastcall Obj_SwapBlockFlags(void*, int, uint8_t* o) {
    for (int k = 0; k < 4; ++k)
        if (uint8_t* b = F<uint8_t*>(o, 0xa8 + k * 4)) UpdateBlockEntry(b, false);
    for (int k = 0; k < 4; ++k)
        if (uint8_t* b = F<uint8_t*>(o, 0xb8 + k * 4)) UpdateBlockEntry(b, true);
}
RT3_REPLACE(0x45a000, Obj_SwapBlockFlags);

// 0x45a520. Starts a block iteration and marks slot idx (-1: all four) in
// the first block's +0xf4; with flag 1 also updates each entry's element (0x475240).
void __fastcall BlockIter_MarkSlot(uint8_t* o, int32_t idx, int32_t flag) {
    hook::Global<int32_t>(0x6acd44) = 0;
    hook::Global<int32_t>(0x6acd48) = 0;
    hook::Global<int32_t>(0x6acd54) = 0;
    hook::Global<uint8_t*>(0x6acd50) = o;
    hook::Global<int32_t>(0x6acd4c) = 0;
    int32_t n = F<int32_t>(o, 0xc);
    hook::Global<uint8_t*>(0x6acd4c) = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    for (int32_t i = 0; i < F<int32_t>(hook::Global<uint8_t*>(0x6acd50), 0xc); ++i) {
        void* el = F<void*>(o, 0x18 + i * 0xc);
        if (!el) continue;
        uint8_t* blk = hook::Global<uint8_t*>(0x6acd4c);
        if (idx == -1) {
            for (int k = 0; k < 4; ++k) hook::Global<uint8_t*>(0x6acd4c)[0xf4 + k] = 1;
        } else {
            blk[0xf4 + idx] = 1;
        }
        if (flag == 1) Obj_SetElementFlag1d8(el, 1, idx);
    }
}
RT3_REPLACE(0x45a520, BlockIter_MarkSlot);

// 0x4952f0. Angle (radians, [0, 2 pi)) of the direction from (x1, y1) to
// (x0, y0) via atan; returned unrounded.
double __stdcall AngleOf(float x0, float y0, float x1, float y1) {
    if (x0 == x1) return y0 > y1 ? (float)4.71238898038 : (float)1.5707963267948966;
    float a = (float)((double(__cdecl*)(double))0x5a1270)(((double)y0 - y1) / ((double)x0 - x1));
    if (x0 > x1) return (double)a + *(const float*)0x5c8760;
    if (y0 > y1) return (double)a + *(const float*)0x5c8768;
    return a;
}
RT3_REPLACE(0x4952f0, AngleOf);

// 0x4a7e10. Train id exists and is in none of the idle/stopped states.
int32_t __fastcall Train_IsRunning(void*, int, uint32_t id) {
    if (!Collection_IsValid(g_trains, 0, id)) return 0;
    switch (F<int32_t>(Collection_Get(g_trains, 0, id), 0xe8)) {
    case 0x13: case 0xa: case 9: case 8: case 7: case 0x10: case 1: case 2: case 3: case 5: case 4: case 6:
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4a7e10, Train_IsRunning);

// 0x4a80a0. Drops the pending node +0x1cf once its link (side from +0x1ce bit 2)
// is gone or points at +0x25. 0 while still pending.
int32_t __fastcall Train_CheckPendingNode(uint8_t* t) {
    if (!Collection_IsValid(g_trackNodes, 0, F<uint32_t>(t, 0x1cf))) return 1;
    void* n = Collection_Get(g_trackNodes, 0, F<uint32_t>(t, 0x1cf));
    int32_t link = Node_GetLink(n, 0, (t[0x1ce] >> 2) & 1);
    if (link != -1 && link != F<int32_t>(t, 0x25)) return 0;
    F<int32_t>(t, 0x1cf) = -1;
    return 1;
}
RT3_REPLACE(0x4a80a0, Train_CheckPendingNode);

// 0x4a83a0. Destination of the train's current order (+8 array): a track
// node for flagged orders, else the site's +8; `preferStop` picks +0x94
// when valid. *dist gets the order's +0x2f. -1 if none.
int32_t __fastcall Train_OrderTarget(uint8_t* t, int, int32_t* dist, int32_t preferStop) {
    if (F<int32_t>(t, 8) == -1) return -1;
    uint8_t* c = (uint8_t*)Array33_Current(t);
    if (!c) return -1;
    if ((int8_t)c[0x28] < 0) {
        if (preferStop && Collection_IsValid(g_trackNodes, 0, F<uint32_t>(t, 0x94))) return F<int32_t>(t, 0x94);
        if (dist) *dist = F<int32_t>(c, 0x2f);
        return F<int32_t>(c, 0x2b);
    }
    Collection* sites = hook::Global<Collection*>(0x6cec20);
    if (!Collection_IsValid(sites, 0, F<uint16_t>(c, 0x29))) return F<int32_t>(t, 0x94);
    uint8_t* s = (uint8_t*)Collection_Get(sites, 0, F<uint16_t>(c, 0x29));
    if (!s) return -1;
    if (dist) *dist = F<int32_t>(c, 0x2f);
    if (preferStop && F<int32_t>(t, 0x94) != -1) return F<int32_t>(t, 0x94);
    return F<int32_t>(s, 8);
}
RT3_REPLACE(0x4a83a0, Train_OrderTarget);

} // namespace rt3
