// StationTypes.cpp - RT3.exe 0x486000-0x488000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <math.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

void __fastcall StationType_Init(uint8_t* s);
uint8_t* __fastcall Station_Construct(uint8_t* s);
void __fastcall Station_InitWithId(uint8_t* s, int, int32_t id);
void __fastcall Nop487630(void*);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
uint8_t* __fastcall Camera_Construct(uint8_t* c, int, const char* name, uint32_t owner);
PtrListNode* __fastcall Camera_AddListener(uint8_t* c, int, void* item);

uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                      uint32_t flags);

void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);

uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6, int32_t a7,
                                  float vol);

void* __fastcall Engine_Get1C(void* e);
void __fastcall Obj_GetXYZ4(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
void __fastcall Terrain_FreeCell167d(uint8_t* m);
int32_t __fastcall Terrain_IsSpecial(uint8_t* r, int, int32_t x, int32_t y);
struct PtrListNode;

PtrListNode* __fastcall Terrain_AddQuad(uint8_t* t, int, float x0, float y0, float z0, float x1, float y1, float z1,
                                        float x2, float y2, float z2, float x3, float y3, float z3, uint8_t raise);

uint32_t __fastcall Obj_BuildingRegionIndex(void* o);
uint32_t __fastcall Obj_BuildingMapQuery(void* o);
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
void* __fastcall Obj_Node8(void* o);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
void __fastcall SceneObj_Destructor(uint8_t* o);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
void __fastcall Nop487640(void*);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void CursorMarker_Free();

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE uint8_t* __fastcall Tank_Construct(uint8_t* t, int, const char* name, uint8_t* scene, float x, float y, float z);
int32_t __fastcall Return0_486220(void*);
int32_t __fastcall Return0_486230(void*);
RT3_NOINLINE void Tank_SpawnDemo();
RT3_NOINLINE void CursorMarker_Free();
void __fastcall StationType_Init(uint8_t* s);
RT3_NOINLINE uint8_t* __fastcall Station_Construct(uint8_t* s);
RT3_NOINLINE void __fastcall Nop487630(void*);
RT3_NOINLINE void __fastcall Nop487640(void*);
RT3_NOINLINE void __fastcall Station_InitWithId(uint8_t* s, int, int32_t id);
int32_t __fastcall Return0_487670(void*, int, int32_t);
void __fastcall Nop487680(void*, int, int32_t);
RT3_NOINLINE void __fastcall Station_RollStats(uint8_t* o);
void __fastcall Obj_SetSlot3b5(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
void __fastcall Region_PickOther(uint8_t* o);
void __fastcall Station_AddStat45(uint8_t* o, int, float a, uint8_t* company, int32_t cargo);
void __fastcall Station_AddStat49(uint8_t* o, int, float a, uint8_t* company, int32_t cargo);
void __fastcall Station_AddStat4d(uint8_t* o, int, float a, uint8_t* company);
void __fastcall Station_AddStat51(uint8_t* o, int, float a, uint8_t* company);
void __fastcall World_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add);
RT3_NOINLINE float __fastcall Obj_RoundedCost(uint8_t* o, int, uint32_t company);
int32_t __fastcall Region_StationsLinked(uint32_t* o, int, int32_t company);
RT3_NOINLINE Collection* __fastcall Collection5cf10c_Construct(Collection* c);
void __fastcall Collection5cf10c_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall Stations_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Stations_Save(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE uint32_t __fastcall StationTypes_Add(Collection* t, int, const char* name);
void __fastcall Collection5cf114_Remove(Collection* c, int, uint32_t id);
RT3_NOINLINE void __fastcall Territories_BuildBorders(Collection* c);
RT3_NOINLINE void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);

// 0x487820..0x487920. Station stat bookkeeping: adds to this year's totals
// (and per-cargo slot), and to the company's per-region block (+0 is the region).
static inline void AddF(uint8_t* p, uint32_t off, float a) { F<float>(p, off) = (float)((double)a + F<float>(p, off)); }

namespace {
inline int32_t PackHas(const char* path) { return NameTable_ContainsName(g_pack, 0, path, 0, nullptr); }
// A scene object of model `path` (texture `tex`), or none when out of memory.
inline uint8_t* NewSceneObj(const char* path, const char* tex) {
    uint8_t* m = (uint8_t*)crt::operator_new(0x206);
    return m ? SceneObj_Construct(m, 0, 0, path, tex) : nullptr;
}
// A light of model `path` on `owner`, shown (flags 0x10).
inline uint8_t* NewLight(const char* path, uint8_t* owner) {
    uint8_t* m = (uint8_t*)crt::operator_new(0x35);
    uint8_t* c = m ? Camera_Construct(m, 0, path, (uint32_t)(uintptr_t)owner) : nullptr;
    return c;
}
inline void ShowLight(uint8_t* c) { c[0x24] = (uint8_t)((c[0x24] & 0xf) | 0x10); }
inline uint8_t* NewSound(const char* path, uint8_t* v, float vol) {
    uint8_t* m = (uint8_t*)crt::operator_new(0x141);
    return m ? Channel_Setup(m, 0, path, g_map, F<float>(v, 0x1e2), F<float>(v, 0x1e6), F<float>(v, 0x1ea), -1,
                             0, vol)
             : nullptr;
}
inline uint8_t* NewExhaust(const char* path, uint32_t color, int32_t perPoint, const float* rect, uint8_t* v, uint32_t flags,
                           uint32_t fade) {
    uint8_t* m = (uint8_t*)crt::operator_new(0x1fd);
    uint8_t* e = m ? Emitter_Construct(m, 0, path, color, perPoint, rect, v, flags) : nullptr;
    F<uint32_t>(e, 0x88) = 0;
    F<uint32_t>(e, 0x8c) = 0;
    F<uint32_t>(e, 0x90) = 0;
    F<uint32_t>(e, 0x98) = fade;
    Timer_SetBudget(e, 0, 0x3c);
    Camera_AddListener(v, 0, e);
    return e;
}
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline uint8_t* Map() { return g_map; }

static inline Collection* Stations() { return g_stations; }

static inline bool Usable(uint8_t* s, int32_t company) {
    return Obj_BuildingStateActive(s) && Obj_Flag3d4(Obj_Building(s)) == 1 &&
           (!company || company == Obj_BuildingValue(s));
}

// 0x486220 (vtable stub).
int32_t __fastcall Return0_486220(void*) { return 0; }
RT3_REPLACE(0x486220, Return0_486220);

// 0x486230 (vtable stub).
int32_t __fastcall Return0_486230(void*) { return 0; }
RT3_REPLACE(0x486230, Return0_486230);

// 0x487450. Spawns the demo tank ("TigerTank", 0x6cfc8c) 32 above the camera's
// point when there is none yet.
RT3_NOINLINE void Tank_SpawnDemo() {
    if (g_6cfc8c) return;
    Engine_Get1C(hook::As<void*>(g_engine));
    float p[3];
    Obj_GetXYZ4(F<uint8_t*>(g_engine, 0x28), 0, (uint32_t*)&p[2], (uint32_t*)&p[0], (uint32_t*)&p[1]);
    p[0] = (float)((double)p[0] + *(const float*)0x5c8c40);
    uint8_t* m = (uint8_t*)crt::operator_new(0x434);
    if (!m) {
        g_6cfc8c = nullptr;
        return;
    }
    g_6cfc8c =
        Tank_Construct(m, 0, (const char*)0x5cf100, (uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), p[2], p[0], p[1]);
}
RT3_REPLACE(0x487450, Tank_SpawnDemo);

// 0x4874f0. Deletes the cursor marker object (0x6cfc8c): back to the scene
// object vtable, unfollowed by the camera when it was the target.
RT3_NOINLINE void CursorMarker_Free() {
    uint8_t* o = g_6cfc8c;
    if (!o) return;
    F<uint32_t>(o, 0) = 0x5cf048;
    if (F<uint8_t*>(g_62be68, 0x60) == o) World_SetFollow(g_world, 0, 0, nullptr);
    SceneObj_Destructor(o);
    crt::operator_delete(o);
    hook::As<uint32_t>(g_6cfc8c) = 0;
}
RT3_REPLACE(0x4874f0, CursorMarker_Free);

// 0x487540. Initialises a station record (name "-", stats cleared).
RT3_NOINLINE void __fastcall StationType_Init(uint8_t* s) {
    F<int32_t>(s, 0) = 0;
    F<int32_t>(s, 0x2d) = 0;
    F<int32_t>(s, 0x31) = 1;
    F<int32_t>(s, 0x35) = 1;
    F<int32_t>(s, 0x39) = 100000;
    FormatText((char*)s + 4, (const char*)0x5ca2b4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    static const uint32_t kZero[] = { 0x3d, 0x41, 0x45, 0x49, 0x1fd, 0x201, 0x4d, 0x51, 0x205, 0x209 };
    for (uint32_t off : kZero) F<int32_t>(s, off) = 0;
    for (int32_t i = 0; i < 0x35; ++i) {
        F<int32_t>(s, 0x55 + i * 4) = 0;
        F<int32_t>(s, 0x129 + i * 4) = 0;
        F<int32_t>(s, 0x20d + i * 4) = 0;
        F<int32_t>(s, 0x2e1 + i * 4) = 0;
    }
    for (int k = 0; k < 7; ++k) F<int32_t>(s, 0xd51 + k * 4) = 0;
    F<int32_t>(s, 0xd4d) = 1;
    for (int k = 0; k < 0x266; ++k) F<int32_t>(s, 0x3b5 + k * 4) = 0;
    for (int k = 0; k < 0x19; ++k) F<int32_t>(s, 0xd6d + k * 4) = 0;
}
RT3_REPLACE(0x487540, StationType_Init);

RT3_NOINLINE uint8_t* __fastcall Station_Construct(uint8_t* s) {
    StationType_Init(s);
    return s;
}
RT3_REPLACE(0x487620, Station_Construct);

RT3_NOINLINE void __fastcall Nop487630(void*) { __asm nop }
RT3_REPLACE(0x487630, Nop487630);

// 0x487640 (vtable stub).
RT3_NOINLINE void __fastcall Nop487640(void*) { __asm nop }
RT3_REPLACE(0x487640, Nop487640);

RT3_NOINLINE void __fastcall Station_InitWithId(uint8_t* s, int, int32_t id) {
    StationType_Init(s);
    F<int32_t>(s, 0) = id;
}
RT3_REPLACE(0x487650, Station_InitWithId);

int32_t __fastcall Return0_487670(void*, int, int32_t) { return 0; }
RT3_REPLACE(0x487670, Return0_487670);

void __fastcall Nop487680(void*, int, int32_t) { __asm nop }
RT3_REPLACE(0x487680, Nop487680);

// 0x487690. Station year roll-over: this year's four totals (+0x45..) and
// two 53-entry cargo arrays (+0x55, +0x129) are added to the sums and cleared.
RT3_NOINLINE void __fastcall Station_RollStats(uint8_t* o) {
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

// 0x487780. Picks a region (+0x35) other than this one with +0x31 set
// (keeps the current one if still valid); 1 if none. Only while own +0x31 is 0.
void __fastcall Region_PickOther(uint8_t* o) {
    if (F<int32_t>(o, 0x31)) return;
    Collection* regions = g_territories;
    int32_t cur = F<int32_t>(o, 0x35);
    if (cur != F<int32_t>(o, 0) && Collection_IsValid(regions, 0, cur) &&
        F<int32_t>(Collection_Get(regions, 0, F<uint32_t>(o, 0x35)), 0x31))
        return;
    for (int32_t i = 0; i < Collection_Count(regions); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(regions, 0, Collection_IdAt(regions, 0, i, 0));
        if (F<int32_t>(r, 0) != F<int32_t>(o, 0) && F<int32_t>(r, 0x31)) {
            F<int32_t>(o, 0x35) = F<int32_t>(r, 0);
            return;
        }
    }
    F<int32_t>(o, 0x35) = 1;
}
RT3_REPLACE(0x487780, Region_PickOther);

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

// 0x487960. As 0x426c20 for the world-level counters +0xd51.. (kind) and +0xd61.. (class).
void __fastcall World_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add) {
    int32_t d = add ? 1 : -1;
    uint8_t kind = t[0x218];
    if (kind == 1) F<int32_t>(o, 0xd51) += d;
    else if (kind == 2) F<int32_t>(o, 0xd59) += d;
    else F<int32_t>(o, 0xd55) += d;
    int32_t cls = F<int32_t>(t, 0x226);
    if (cls == 3) F<int32_t>(o, 0xd61) += d;
    else if (cls == 1) F<int32_t>(o, 0xd65) += d;
    else if (((int32_t(__thiscall*)(void*))(*(void***)t)[0x14 / 4])(t)) F<int32_t>(o, 0xd69) += d;
}
RT3_REPLACE(0x487960, World_CountTrack);

// 0x4879f0. o's cost (+0x39) scaled by effect 0x1c5 for company and o's region
// (+0), rounded to the nearest 1000.
RT3_NOINLINE float __fastcall Obj_RoundedCost(uint8_t* o, int, uint32_t company) {
    float m = Effects_Multiplier(g_world, 0, 0x1c5, company, 0, F<uint32_t>(o, 0));
    float base = (float)F<int32_t>(o, 0x39);
    int32_t v;
    __asm {
        fld m
        fmul base
        call Rt3_Ftol
        mov v, eax
    }
    return (float)((v + 500) / 1000 * 1000);
}
RT3_REPLACE(0x4879f0, Obj_RoundedCost);

// 0x487a60. Whether a usable station (optionally of the company) in this region
// is track-connected to one in another map area.
int32_t __fastcall Region_StationsLinked(uint32_t* o, int, int32_t company) {
    for (int32_t i = 0; i < Collection_Count(Stations()); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(Stations(), 0, Collection_IdAt(Stations(), 0, i, 0));
        if (Obj_BuildingRegionIndex(s) != o[0] || !Usable(s, company)) continue;
        uint32_t area = Obj_BuildingMapQuery(s);
        uint32_t node = F<uint32_t>(s, 8);
        for (int32_t j = 0; j < Collection_Count(Stations()); ++j) {
            uint8_t* s2 = (uint8_t*)Collection_Get(Stations(), 0, Collection_IdAt(Stations(), 0, j, 0));
            if (Obj_BuildingMapQuery(s2) == area || !Usable(s2, company)) continue;
            if (Node_SameComponent((uint8_t*)Obj_Node8(s2), 0, node)) return 1;
        }
    }
    return 0;
}
RT3_REPLACE(0x487a60, Region_StationsLinked);

// 0x487bd0 / 0x487c00. Collection subclass 0x5cf10c (heap records, 10 + 20).
RT3_NOINLINE Collection* __fastcall Collection5cf10c_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5cf10c;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x487bd0, Collection5cf10c_Construct);

void __fastcall Collection5cf10c_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5cf10c;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x487c00, Collection5cf10c_Destruct);

// 0x487c20 / 0x487cc0. Station collection load / save framed by
// 0x61a9/0x61aa/0x61ab with the (empty) per-record hooks 0x487670 / 0x487680.
RT3_NOINLINE int32_t __fastcall Stations_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x61a9, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, 0x61aa, 0, 0);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        n += ((int32_t(__thiscall*)(void*, Stream))&Return0_487670)(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), s);
    return StreamSkip4(s, 0x61ab, 0, 0) + n;
}
RT3_REPLACE(0x487c20, Stations_Load);

RT3_NOINLINE void __fastcall Stations_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x61a9);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x61aa);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        ((void(__thiscall*)(void*, Stream))&Nop487680)(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), s);
    StreamWriteU32(s, 0x61ab);
}
RT3_REPLACE(0x487cc0, Stations_Save);

// 0x487d40. Adds a new station (0xdd1-byte record), optionally named; returns its ID.
RT3_NOINLINE uint32_t __fastcall StationTypes_Add(Collection* t, int, const char* name) {
    static uint8_t local[0xdd4];  // original: stack (initialised by 0x487620)
    for (auto& b : local) b = 0;
    Station_Construct(local);
    uint32_t id = Collection_Allocate(t, 0, local, 0xdd1, 0, 0);
    uint8_t* r = (uint8_t*)Collection_Get(t, 0, id);
    Station_InitWithId(r, 0, (int32_t)id);
    if (name) StrCopyN((char*)r + 4, name, 0x28);
    Nop487630(local);
    return id;
}
RT3_REPLACE(0x487d40, StationTypes_Add);

void __fastcall Collection5cf114_Remove(Collection* c, int, uint32_t id) {
    Nop487640(Collection_Get(c, 0, id));
    Collection_Release(c, 0, id);
}
RT3_REPLACE(0x487db0, Collection5cf114_Remove);

// 0x487de0. With more than one territory in use, walls off the borders between
// tiles of different territory groups (tile flags >> 3; grouped by +0x31 / +0x35)
// with thin quads along the west and north tile edges.
RT3_NOINLINE void __fastcall Territories_BuildBorders(Collection* c) {
    Terrain_FreeCell167d(Map());
    int32_t used = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<int32_t>(e, 0x31) && F<int32_t>(e, 0x41) + F<int32_t>(e, 0x3d) > 0) ++used;
    }
    if (used <= 1) return;
    int32_t w = F<int32_t>(Map(), 0x2155), h = F<int32_t>(Map(), 0x2159);
    int32_t group[100];
    memset(group, 0, sizeof group);
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        group[F<int32_t>(e, 0)] = F<int32_t>(e, 0x31) ? F<int32_t>(e, 0) : F<int32_t>(e, 0x35);
    }
    const double d = D(0x5cf118);
    for (int32_t y = 0; y < h; ++y)
        for (int32_t x = 0; x < w; ++x) {
            if (Terrain_IsSpecial(Map(), 0, x, y)) continue;
            for (int32_t k = 0; k < 2; ++k) {
                int32_t nx = x + (k ? 0 : -1), ny = y + (k ? -1 : 0);
                if (nx < 0 || ny < 0) continue;
                if (Terrain_IsSpecial(Map(), 0, nx, ny)) continue;
                uint8_t* m = Map();
                uint8_t* tiles = F<uint8_t*>(m, 0x2131);
                int32_t t1 = tiles[F<int32_t>(m, 0x2155) * y + x] >> 3;
                int32_t t2 = tiles[F<int32_t>(m, 0x2155) * ny + nx] >> 3;
                if (group[t1] == group[t2]) continue;
                int32_t px = x * 0x5c, pz = y * 0x5c;
                if (!k) {
                    float xa = (float)((double)px + d), xb = (float)((double)px - d);
                    float za = (float)pz, zb = (float)(pz + 0x5c);
                    Terrain_AddQuad(Map(), 0, xb, 0.0f, zb, xb, 0.0f, za, xa, 0.0f, za, xa, 0.0f, zb, 2);
                } else {
                    float za = (float)((double)pz + d), zb = (float)((double)pz - d);
                    float xa = (float)px, xb = (float)(px + 0x5c);
                    Terrain_AddQuad(Map(), 0, xb, 0.0f, zb, xa, 0.0f, zb, xa, 0.0f, za, xb, 0.0f, za, 2);
                }
            }
        }
}
RT3_REPLACE(0x487de0, Territories_BuildBorders);

}  // namespace rt3
