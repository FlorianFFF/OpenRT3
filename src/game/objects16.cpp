// Block 0x42xxxx, functions (batch 19). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static uint8_t*& g_world          = hook::Global<uint8_t*>(0x6cec78);
static Collection*& g_players     = hook::Global<Collection*>(0x6ceb9c);
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_cargoTypes  = hook::Global<Collection*>(0x62ba8c);

static inline double D(uint32_t va) { return *(const double*)va; }
static inline size_t StrLen(const char* s) { size_t n = 0; while (s[n]) ++n; return n; }

uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Obj_HasKey(void* o, int, uint16_t k);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
int32_t __stdcall Bracket5(int32_t value);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
int32_t __fastcall Company_FreeShares(void* c);
int32_t __fastcall Company_PlayerDebt(int32_t* o);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute);

// 0x420280. ID of the first 0x6cec20 site in this object's region that is
// active (or `anyState`) and whose building is flagged (or `anyFlag` is 0).
uint32_t __fastcall Obj_FindRegionSite(void* o, int, int32_t requireActive, int32_t requireFlag) {
    for (int32_t i = 0; i < Collection_Count(g_table6cec20); ++i) {
        uint32_t id = Collection_IdAt(g_table6cec20, 0, i, 0);
        if (!Obj_HasKey(Obj_Region(o), 0, (uint16_t)id)) continue;
        void* r = Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (!Obj_BuildingStateActive(r) && requireActive) continue;
        r = Collection_Get(g_table6cec20, 0, Collection_IdAt(g_table6cec20, 0, i, 0));
        if (Obj_Flag3d4(Obj_Building(r)) == 1 || !requireFlag) return Collection_IdAt(g_table6cec20, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x420280, Obj_FindRegionSite);

// 0x4234e0. Production rate of slot i, from the type tables 0x5ee1b0 /
// 0x5ee138, the era tables 0x5f33e0 / 0x5f33cc and size +0x25e.
float __fastcall Obj_ProductionRate(void* o, int, int32_t i) {
    const float* typeTbl = (const float*)0x5ee1b0;
    int32_t era = F<int32_t>(g_world, 0x2d);
    int32_t type = F<int32_t>(o, 0x23e);
    if (type) {
        double v = (double)typeTbl[type + i * 2] * ((const float*)0x5f33e0)[era];
        v = (double)F<float>(o, 0x25e) / D(0x5c85d8) * v;
        v = v * F<float>(g_world, 0xbde);
        v = v * (double)F<int32_t>(o, 0x242);
        return (float)(v * D(0x5c8918));
    }
    int32_t b = Bracket5((int32_t)F<float>(o, 0x25e));
    double v = (double)((const float*)0x5ee138)[b + i * 5] * typeTbl[F<int32_t>(o, 0x23e) + i * 2];
    v = v * ((const float*)0x5f33cc)[era];
    v = (double)F<float>(o, 0x25e) / D(0x5c85d8) * v;
    v = v * F<float>(g_world, 0xbde);
    return (float)(v * D(0x5c8d68));
}
RT3_REPLACE(0x4234e0, Obj_ProductionRate);

// 0x423d70. Removes entry idx of the 12-byte array +0x5f (count +0x5b),
// booking its amount (stats 0x12 / 0xd, and 0xc with cash when `post`).
void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post) {
    if (idx < 0 || idx >= F<int32_t>(c, 0x5b)) return;
    uint8_t* e = (uint8_t*)c + 0x5f + idx * 12;
    Company_AddStat(c, 0, 0x12, (float)F<int32_t>(e, 0));
    Company_AddStat(c, 0, 0xd, (float)(int32_t)(0u - F<uint32_t>(e, 0)));
    if (post) {
        double neg = (double)(int32_t)(0u - F<uint32_t>(e, 0));
        Company_PostStat(c, 0, 0xc, (float)(neg * D(0x5c8918)), 1);
    }
    int32_t n = F<int32_t>(c, 0x5b);
    if (n > idx + 1) {
        uint8_t* src = e + 12;
        for (int32_t k = 0; k < (n - idx - 1) * 12; ++k) e[k] = src[k];
    }
    F<int32_t>(c, 0x5b) = n - 1;
}
RT3_REPLACE(0x423d70, Company_RemoveEntry);

// 0x4245e0. Index of the player with the n-th largest positive holding
// +0x15d[*o]; -1 if none. QUIRK: with `flag` set it always returns -1.
int32_t __fastcall Company_NthHolder(int32_t* o, int, int32_t n, int32_t flag) {
    if (!F<int32_t>(o, 0x3f)) return -1;
    int32_t count = Table_CountField4(g_players);
    int32_t used[16] = {};
    int32_t bestIdx = 0;
    if (--n < 0) return -1;
    for (;;) {
        int32_t best = flag ? -2000000 : 0;
        for (int32_t j = 0; j < count; ++j) {
            if (used[j]) continue;
            int32_t v = F<int32_t>(Table_NthField4(g_players, 0, j), 0x15d + o[0] * 4);
            if (v == 0) used[j] = 1;
            else if (v > best) { best = v; bestIdx = j; }
        }
        if (best <= 0 || flag) return -1;
        if (n == 0) return bestIdx;
        used[bestIdx] = 1;
        if (--n < 0) return -1;
    }
}
RT3_REPLACE(0x4245e0, Company_NthHolder);

// 0x4268e0. Clamps +0x157 to [world +5, max(world +5, year)] and +0xd07 to
// [start-of-year minutes, max(that, now)]; clears +0xd3a / +0xd3e.
int32_t __fastcall Company_ClampDates(void* c, int, int32_t) {
    F<int32_t>(c, 0xd3a) = 0;
    F<int32_t>(c, 0xd3e) = 0;
    uint16_t lo = F<uint16_t>(g_world, 5);
    uint16_t v = F<uint16_t>(c, 0x157);
    if (v < lo) v = lo;
    F<uint16_t>(c, 0x157) = v;
    uint16_t a = F<uint16_t>(g_world, 5), y = F<uint16_t>(g_world, 0xd);
    uint16_t hi = a > y ? a : y;
    F<uint16_t>(c, 0x157) = v < hi ? v : hi;
    int32_t m = MinutesFromParts(F<uint16_t>(g_world, 5), 1, 1, 0, 0);
    int32_t t = F<int32_t>(c, 0xd07);
    if (t <= m) t = m;
    F<int32_t>(c, 0xd07) = t;
    int32_t now = F<int32_t>(g_world, 0x15);
    int32_t top = m > now ? m : now;
    F<int32_t>(c, 0xd07) = t < top ? t : top;
    return 0;
}
RT3_REPLACE(0x4268e0, Company_ClampDates);

// 0x429910. Free shares (a quarter with `flag`, else half) minus player debt.
int32_t __fastcall Company_SpareShares(int32_t* c, int, int32_t flag) {
    int32_t s = Company_FreeShares(c);
    s = flag ? s >> 2 : s >> 1;
    return s - Company_PlayerDebt(c);
}
RT3_REPLACE(0x429910, Company_SpareShares);

// 0x42a850. Copies `src` into `dst`, drops its last character, keeps only the
// part after the last '\' and cuts at the first '.'. QUIRK: no '\' crashes.
void __fastcall PathToBaseName(char* dst, const char* src) {
    char* d = dst;
    while ((*d++ = *src++) != 0) {}
    dst[StrLen(dst) - 1] = 0;
    const char* s = ((const char*(__cdecl*)(const char*, int))0x5a1df0)(dst, '\\') + 1;
    d = dst;
    while ((*d++ = *s++) != 0) {}
    if (StrLen(dst) == 0) return;
    uint16_t i = 0;
    do {
        if (dst[i] == '.') { dst[i] = 0; return; }
        ++i;
    } while (i < StrLen(dst));
}
RT3_REPLACE(0x42a850, PathToBaseName);

// 0x42a970. "Open file" dialog (title 0x5c9c04) into `file` (MAX_PATH) with
// the given filter and initial directory; restores the current directory.
// Returns whether a file was chosen.
int32_t __fastcall OpenFileDialog(char* file, const char* filter, const char* initialDir) {
    char cwd[0x100];
    ((uint32_t(__stdcall*)(uint32_t, char*))hook::Global<void*>(0x5c807c))(0x100, cwd);
    uint32_t ofn[0x13] = {};
    file[0] = 0;
    ofn[0]  = 0x4c;
    ofn[1]  = 0;
    ofn[3]  = (uint32_t)filter;
    ofn[4]  = 0;
    ofn[5]  = 0;
    ofn[6]  = 0;
    ofn[7]  = (uint32_t)file;
    ofn[8]  = 0x104;
    ofn[11] = (uint32_t)initialDir;
    ofn[9]  = 0;
    ofn[10] = 0x104;
    ofn[12] = 0x5c9c04;
    ofn[15] = 0;
    ofn[13] = 0x1000;
    ((int32_t(__stdcall*)(void*))hook::Global<void*>(0x5c8390))(ofn);
    ((int32_t(__stdcall*)(const char*))hook::Global<void*>(0x5c80d8))(cwd);
    return StrLen(file) != 0 ? 1 : 0;
}
RT3_REPLACE(0x42a970, OpenFileDialog);

// 0x42ae20. Frees the key list +0xef.
void __fastcall Obj_FreeKeyList(void* o) {
    if (F<void*>(o, 0xef)) Free(F<void*>(o, 0xef));
    F<void*>(o, 0xef) = nullptr;
}
RT3_REPLACE(0x42ae20, Obj_FreeKeyList);

// 0x42b140. Adds f (scaled by 0x5c9c40, rounded via +0x5c8628) to the u16
// level +0x6a[i], capped at 65000.
void __fastcall Obj_AddLevel(void* o, int, int32_t i, float f) {
    double v = (double)f * D(0x5c9c40) + D(0x5c8628);
    v = v + (double)F<uint16_t>(o, 0x6a + i * 2);
    int32_t n = (int32_t)v;
    if (n > 65000) n = 65000;
    F<uint16_t>(o, 0x6a + i * 2) = (uint16_t)n;
}
RT3_REPLACE(0x42b140, Obj_AddLevel);

// 0x42b190 / 0x42b230. Key list +0xef of 6-byte {u16 key, i32 value}
// entries, count +0xeb, reallocated on each change. Remove drops every
// entry with `key` but shrinks the count by one (QUIRK).
void __fastcall Obj_AddKey(void* o, int, uint16_t key, int32_t value) {
    int32_t n = F<int32_t>(o, 0xeb);
    uint8_t* p = (uint8_t*)Malloc((n + 1) * 6);
    if (n > 0) {
        const uint8_t* old = F<uint8_t*>(o, 0xef);
        for (int32_t k = 0; k < n * 6; ++k) p[k] = old[k];
    }
    F<uint16_t>(p, n * 6) = key;
    F<int32_t>(p, n * 6 + 2) = value;
    if (F<void*>(o, 0xef)) Free(F<void*>(o, 0xef));
    F<uint8_t*>(o, 0xef) = p;
    F<int32_t>(o, 0xeb) = n + 1;
}
RT3_REPLACE(0x42b190, Obj_AddKey);
void __fastcall Obj_RemoveKey(void* o, int, uint16_t key) {
    uint8_t* p = nullptr;
    int32_t n = F<int32_t>(o, 0xeb);
    if (n > 1) {
        p = (uint8_t*)Malloc((n - 1) * 6);
        uint8_t* d = p;
        for (int32_t k = 0; k < F<int32_t>(o, 0xeb); ++k) {
            uint8_t* e = F<uint8_t*>(o, 0xef) + k * 6;
            if (F<uint16_t>(e, 0) == key) continue;
            F<uint32_t>(d, 0) = F<uint32_t>(e, 0);
            F<uint16_t>(d, 4) = F<uint16_t>(e, 4);
            d += 6;
        }
    }
    Free(F<void*>(o, 0xef));
    F<uint8_t*>(o, 0xef) = p;
    F<int32_t>(o, 0xeb) = F<int32_t>(o, 0xeb) - 1;
}
RT3_REPLACE(0x42b230, Obj_RemoveKey);

// 0x42b350. Bit i of the bitmap +0xdf (the masked byte, not 0/1).
int32_t __fastcall Obj_Bit(void* o, int, int32_t i) {
    return (1 << (i & 7)) & F<uint8_t>(o, 0xdf + (i >> 3));
}
RT3_REPLACE(0x42b350, Obj_Bit);

float __fastcall Obj_Level(void* o, int, int32_t i) {
    return (float)((double)F<uint16_t>(o, 0x6a + i * 2) / D(0x5c9c40));
}
RT3_REPLACE(0x42b380, Obj_Level);

// 0x42c080. Lowers the u16 stock this[id] towards other[id] minus the
// consumption (cargo weight +0x52 x stock, scaled) x f, never below its current value.
void __fastcall Stock_Consume(uint16_t* o, int, const uint16_t* other, uint32_t id, float f) {
    uint8_t* ct = (uint8_t*)Collection_Get(g_cargoTypes, 0, id);
    int32_t a = o[id];
    int32_t s = other[id];
    float t = (float)(((double)F<float>(ct, 0x52) * (double)a * D(0x5c99e8) + D(0x5c8db8)) / D(0x5c9b78));
    s -= (int32_t)((double)t * f);
    o[id] = (uint16_t)(a > s ? a : s);
}
RT3_REPLACE(0x42c080, Stock_Consume);

// 0x42c110. Smallest (min 4000) 16 x level of cargo i over this chain's
// sites (u16 links +0xd4 / +0x3c) visited within the last 0xec400 minutes.
int32_t __fastcall Chain_MinLevel(void* o, int, int32_t i) {
    int32_t best = 999999999;
    for (uint32_t id = F<uint16_t>(o, 0xd4); id; ) {
        uint8_t* r = (uint8_t*)Collection_Get(g_table6cec20, 0, id);
        if (F<int32_t>(g_world, 0x15) - F<int32_t>(r, 0x38e + i * 4) < 0xec400 &&
            Collection_IsValid(g_table6cec20, 0, F<uint32_t>(r, 0x2ba + i * 4))) {
            uint8_t* reg = Obj_BuildingRegion(Collection_Get(g_table6cec20, 0, F<uint32_t>(r, 0x2ba + i * 4)));
            int32_t v = F<uint16_t>(reg, i * 2) << 4;
            if (v <= 4000) v = 4000;
            if (v < best) best = v;
        }
        id = F<uint16_t>(r, 0x3c);
    }
    return best;
}
RT3_REPLACE(0x42c110, Chain_MinLevel);

} // namespace rt3
