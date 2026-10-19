// Functions across blocks 0x41..0x4b (batch 52). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_cities      = hook::Global<Collection*>(0x62ba84);
static Collection*& g_table6cec20 = hook::Global<Collection*>(0x6cec20);
static Collection*& g_companies   = hook::Global<Collection*>(0x62be10);

void __fastcall Save_LogProgress(Stream s, const char* label);
void* __fastcall Obj_Ref388(void* o);
void* __fastcall Obj_City352(void* o);
uint8_t* __fastcall Obj_Region(void* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
int32_t __fastcall Obj_CityRef10(void* o);
float __fastcall Company_QualityFactor(void* c);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
void __fastcall Array33_Save(uint32_t* o, int, Stream s);
void __fastcall Train_Clear1a3(uint8_t* t);
void __fastcall Collection_ReleaseAll(Collection* c);

// 0x413440. Buildings save: tags 0x36b1/0x36b2/0x36b3, records saving
// themselves, with debug progress lines.
void __fastcall Buildings_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x36b1);
    Collection_Save(t, 0, s, recordSize);
    Save_LogProgress(s, (const char*)0x5c9438);
    StreamWriteU32(s, 0x36b2);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    Save_LogProgress(s, (const char*)0x5c9420);
    StreamWriteU32(s, 0x36b3);
}
RT3_REPLACE(0x413440, Buildings_Save);

static inline void* FirstCar(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x41);
    return id != -1 ? Collection_Get(g_cities, 0, id) : nullptr;
}

// 0x4a8880 / 0x4a88b0. Of the train's first car: its type's +0x126 / its region.
int32_t __fastcall Train_FirstCarType126(uint8_t* t) {
    void* c = FirstCar(t);
    return c ? F<int32_t>(Obj_Ref388(c), 0x126) : 0;
}
RT3_REPLACE(0x4a8880, Train_FirstCarType126);
uint8_t* __fastcall Train_FirstCarRegion(uint8_t* t) {
    void* c = FirstCar(t);
    return c ? Obj_Region(c) : nullptr;
}
RT3_REPLACE(0x4a88b0, Train_FirstCarRegion);

// 0x4a8ec0. Smooths the displayed speeds +0x18e (and +0x18a in state 0)
// towards the target +0xcc (snapping when below 0.001).
static void Smooth(uint8_t* t, uint32_t off) {
    if ((double)F<float>(t, off) < *(const double*)0x5c8678) {
        F<uint32_t>(t, off) = F<uint32_t>(t, 0xcc);
        return;
    }
    F<float>(t, off) = (float)((double)F<float>(t, 0xcc) * *(const double*)0x5c86b8 +
                               (double)F<float>(t, off) * *(const double*)0x5c8888);
}
void __fastcall Train_SmoothSpeed(uint8_t* t) {
    Smooth(t, 0x18e);
    if (F<int32_t>(t, 0xe8) == 0) Smooth(t, 0x18a);
}
RT3_REPLACE(0x4a8ec0, Train_SmoothSpeed);

// 0x4a9060. Flags whether the consist has a car of type kind 0 (+0x197) / kind 4 (+0x198).
void __fastcall Train_ScanCarKinds(uint8_t* t) {
    t[0x198] = 0;
    t[0x197] = 0;
    for (void* c = FirstCar(t); c; c = Obj_City352(c)) {
        if (F<int32_t>(Obj_Ref388(c), 0x41) == 0) t[0x197] = 1;
        if (F<int32_t>(Obj_Ref388(c), 0x41) == 4) t[0x198] = 1;
    }
}
RT3_REPLACE(0x4a9060, Train_ScanCarKinds);

// 0x4a92f0. Map grid byte at the position of car +0x45; 0 if none.
uint32_t __fastcall Train_GridByteAtCar45(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x45);
    if (id == -1) return 0;
    void* c = Collection_Get(g_cities, 0, id);
    if (!c) return 0;
    float y = (float)Obj_CellY(c);
    float x = (float)Obj_CellX(c);
    return Map_GridByte(hook::Global<void*>(0x62c120), 0, x, y);
}
RT3_REPLACE(0x4a92f0, Train_GridByteAtCar45);

// 0x4a9430. The track piece +0x49 belongs to the train's company (+0x51).
int32_t __fastcall Train_OnOwnTrack(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x49);
    if (id == -1) return 0;
    uint8_t* n = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, id);
    if (!n) return 0;
    return F<uint16_t>(n, 0x22e) == F<int32_t>(t, 0x51) ? 1 : 0;
}
RT3_REPLACE(0x4a9430, Train_OnOwnTrack);

// 0x4aadd0 / 0x4aae40. Leaves / enters the station site +0xa4 (its train counter +0x58c).
static uint8_t* StationSite(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0xa4);
    if (id == -1 || !Collection_IsValid(g_table6cec20, 0, id) || Obj_CityRef10(t)) return nullptr;
    return (uint8_t*)Collection_Get(g_table6cec20, 0, F<uint32_t>(t, 0xa4));
}
void __fastcall Train_LeaveSite(uint8_t* t) {
    uint8_t* s = StationSite(t);
    if (!s || F<int32_t>(s, 0x58c) <= 0) return;
    s = (uint8_t*)Collection_Get(g_table6cec20, 0, F<uint32_t>(t, 0xa4));
    if (F<int32_t>(s, 0x58c) > 0) F<int32_t>(s, 0x58c)--;
}
RT3_REPLACE(0x4aadd0, Train_LeaveSite);
void __fastcall Train_EnterSite(uint8_t* t) {
    if (uint8_t* s = StationSite(t)) F<int32_t>(s, 0x58c)++;
}
RT3_REPLACE(0x4aae40, Train_EnterSite);

// 0x4ac3f0. Books a train expense (scaled by the company's quality factor)
// into +0x159 and the company's stat 8 (not in the editor).
void __fastcall Train_AddExpense(uint8_t* t, int, float amount) {
    if (hook::Global<uint8_t*>(0x6cec7c)[0x82]) return;
    float v = (float)((double)Company_QualityFactor(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51))) * amount);
    F<float>(t, 0x159) = (float)((double)v + F<float>(t, 0x159));
    Company_PostStat(Collection_Get(g_companies, 0, F<uint32_t>(t, 0x51)), 0, 8, v, 1);
}
RT3_REPLACE(0x4ac3f0, Train_AddExpense);

// 0x4b2340 / 0x4b2370. Collection subclass 0x5d00c8 (inline 0x1d5-byte records, 50 + 20, list 1).
Collection* __fastcall Collection5d00c8_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5d00c8;
    Collection_Init(c, 0, 1, 0x1d5, 0x32, 0x14, 1, 0, 0);
    return c;
}
RT3_REPLACE(0x4b2340, Collection5d00c8_Construct);
void __fastcall Collection_ReleaseAll2(Collection* c) { Collection_ReleaseAll(c); }
RT3_REPLACE(0x4b2370, Collection_ReleaseAll2);

// 0x4b27a0. Trains save: tags 0x5209/0x520a/0x520b with each train's order array (0x4a7030).
void __fastcall Trains_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x5209);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x520a);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Array33_Save((uint32_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, s);
    StreamWriteU32(s, 0x520b);
}
RT3_REPLACE(0x4b27a0, Trains_Save);

void __fastcall Trains_Clear1a3(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) Train_Clear1a3((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x4b28a0, Trains_Clear1a3);

} // namespace rt3
