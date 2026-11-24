// Cars.cpp - RT3_mp.exe 0x41a000-0x41e000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/containers.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

  // industries/stations/houses
static Collection*& g_table62ba88 = hook::Global<Collection*>(0x62ba88);

static Collection*& g_cities     = hook::Global<Collection*>(0x62ba84);
void* __fastcall Obj_Ref388(void* o);
static Collection*& g_trains     = hook::Global<Collection*>(0x6cfcbc);
static Collection*& g_trackNodes  = hook::Global<Collection*>(0x6cfca8);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b);
void __fastcall Obj_UnlinkFromNode(void* o);
int32_t __fastcall Train_State1Or2(uint8_t* t);
void __fastcall Node_SetLinkPair(void* n, int, int32_t a, int32_t b, int32_t first);
int32_t __fastcall Track_Field238(uint8_t* t, int, int32_t flag);
void __fastcall Model_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
float __fastcall SceneObj_Angle2(uint8_t* o);
void __fastcall Model_SetPartFrame(uint8_t* o, int, int32_t part, int32_t frame);
void __fastcall Model_ShowOnlyPart(uint8_t* o, int, int32_t sel);
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);

// A collection of scene objects loaded from a save between the frame tags
// `t0` / `t1` / `t2`: the records get the vtable of a freshly constructed
// object of their class, then load their own extra data (vtbl +0x40).
template <typename Ctor, typename Dtor>
static int32_t LoadSceneObjects(Collection* t, Stream s, int32_t recordSize, int32_t t0, int32_t t1, int32_t t2, Ctor ctor, Dtor dtor) {
    int32_t n = StreamSkip4(s, t0, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, t1, 0, 0);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        uint8_t tmp[0x400];
        ctor(tmp);
        F<uint32_t>(r, 0) = F<uint32_t>(tmp, 0);
        n += ((int32_t(__thiscall*)(uint8_t*, Stream))(*(void***)tmp)[0x40 / 4])(r, s);
        dtor(tmp);
    }
    return StreamSkip4(s, t2, 0, 0) + n;
}

uint8_t* __fastcall PartModel41aae0_Construct(uint8_t* o);
struct Collection;
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
uint8_t* __fastcall PartModel_Construct(uint8_t* o);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t IsMultiplayer();
float __fastcall Net_CommonVersion();
void* __fastcall Obj_Train390(void* o);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
void __fastcall Obj_SetAmount39c(void* o, int, float v);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
uint8_t* __fastcall Train_FirstCarRegion(uint8_t* t);
void* __fastcall Train_Company(void* t);
void __fastcall Obj_AddRevenue(void* o, int, float a, float b, float c, int32_t flag, int32_t idx);
void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);
void __fastcall Car_ResolveType(uint8_t* o);
void __fastcall Car_ShowCargoModel(uint8_t* c);
void __fastcall PartModel41aae0_Destructor(uint8_t* o);
int32_t __fastcall Game_EffectiveVersion();

void __fastcall Station_PickUp(uint8_t* o, int, uint8_t* city, uint8_t* to, int32_t cargo, float cap, float* outCount,
                               float* outValue, float* outAmount);

void* __fastcall Obj_BuildingKind(void* o);
double __fastcall Company_QualityRaw(void* c);
int32_t __fastcall Obj_Get21(void* o);
int __fastcall ToUpper(char c);
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void __fastcall String_Assign(char** s, const char* src);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
void* __fastcall Obj_CityRef(void* o);
uint32_t __fastcall Company_Logo(uint8_t* c);
Collection* __fastcall Collection_SetVtbl(Collection* c);

void __fastcall Collection_Init(Collection* c, int, int32_t inlineRecords, int32_t stride, int32_t initialCapacity, int32_t growBy, int32_t list1,
                                int32_t list2, int32_t list3);

void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void __fastcall Collection_Destruct(Collection* c);
void __fastcall SceneObj_Destructor(uint8_t* o);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size, uint32_t key2, uint32_t key3);
double __fastcall Obj_Rate(void* o);
void* __fastcall Obj_CargoType(void* o);

Collection* __fastcall Collection_Construct(Collection* c, int, int32_t inlineRecords, int32_t stride, int32_t initialCapacity, int32_t growBy, int32_t list1,
                                            int32_t list2, int32_t list3);

void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
RT3_NOINLINE int __fastcall ToUpper(char c);
RT3_NOINLINE void __fastcall Obj_UnlinkFromNode(void* o);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
Collection* __fastcall BtyTable_Construct(Collection* t);
void __fastcall BtyTable_Reload(Collection* t);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall Car_Teardown(uint8_t* o);

// Collection callbacks deleting record `id`: its teardown, then the slot.
#define RT3_DELETE_RECORD(addr, name, teardown, type) \
    void __fastcall name(Collection* c, int, uint32_t id) { \
        teardown((type)Collection_Get(c, 0, id)); \
        Collection_Release(c, 0, id); \
    } \
    RT3_REPLACE(addr, name);

void __fastcall Car_StatusText(uint8_t* car, int, char* out);
uint8_t Global622b0cIs64();
void Companies_Clash();
int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);
void __fastcall Car_Save(uint8_t* c, int, Stream s);
uint8_t* __fastcall Model_NextFrame(uint8_t* o);
void __fastcall Obj_SetTexture(uint8_t* o, int, const char* name);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall BuildingTypes_LoadFiles(Collection* t);
RT3_NOINLINE void __fastcall BtyTable_Reload(Collection* t);
void __fastcall BtyTable_Setup(Collection* t);
RT3_NOINLINE Collection* __fastcall BtyTable_Construct(Collection* t);
uint8_t* __fastcall PartModel41aae0_Construct(uint8_t* o);
void __fastcall PartModel41aae0_Destructor(uint8_t* o);
int32_t Tooltip_Clear41ab10();
void __fastcall Obj_Toggle35a(void* o);
int32_t __fastcall Return26_41ab50(void*);
int32_t __fastcall Return22_41ab60(void*);
void __fastcall Thunk41ab70(uint8_t* p0, int p1, Stream p2);
RT3_NOINLINE float __fastcall Car_TiltValue(uint8_t* c);
RT3_NOINLINE void __fastcall Obj_SetTexture(uint8_t* o, int, const char* name);
int32_t __fastcall Return0_41ac00(void*);
int32_t __fastcall Return0_41ac10(void*);
int32_t __fastcall Return0_41ac20(void*);
int32_t __fastcall Returna_41ac30(void*);
int32_t __fastcall Return1_41ac40(void*);
int32_t __fastcall Return1_41ac50(void*);
void __fastcall Car_ResolveType(uint8_t* o);
void __fastcall Car_OpenTrain(uint8_t* c);
RT3_NOINLINE void* __fastcall Obj_City352(void* o);
RT3_NOINLINE void* __fastcall Obj_Ref388(void* o);
void __fastcall Car_RearPoint(uint8_t* c, int, float* x, float* y);
RT3_NOINLINE void __fastcall Car_LinkToNode(uint8_t* car, int, int32_t id, int32_t side);
RT3_NOINLINE void __fastcall Obj_UnlinkFromNode(void* o);
RT3_NOINLINE void* __fastcall Obj_Train390(void* o);
double __fastcall Obj_Ref388Float10d(void* o);
uint32_t __fastcall Obj_NodeVirtual5(void* o);
RT3_NOINLINE void* __fastcall Obj_CargoType(void* o);
void __fastcall Obj_SetAmount39c(void* o, int, float v);
void __fastcall Car_SetCargoValue(uint8_t* car, int, uint8_t* city);
void __fastcall Car_LoadAt(uint8_t* car, int, uint8_t* at, uint8_t* to);
void __fastcall Car_ApplyPriceFactor(uint8_t* car);
RT3_NOINLINE double __fastcall Obj_Rate(void* o);
RT3_NOINLINE int32_t __fastcall Obj_NodeNear356(uint8_t* o, int, int32_t id);
RT3_NOINLINE uint32_t __fastcall Car_SkinTexture(uint8_t* car);
RT3_NOINLINE void __fastcall Car_StatusText(uint8_t* car, int, char* out);
RT3_NOINLINE void __fastcall Car_Teardown(uint8_t* o);
void __fastcall Car_ShowCargoModel(uint8_t* c);
void __fastcall Car_ShowStatusTooltip(uint8_t* c);
void __fastcall Obj_DetachChain(void* o);
int32_t __fastcall Car_Tooltip(uint8_t* o, int, char* dst, int32_t n, int32_t a, int32_t b, int32_t logo);
float __fastcall Load_PayOut(uint8_t* o);
int32_t __fastcall Car_LoadState(uint8_t* c, int, int32_t s);
RT3_NOINLINE void __fastcall Car_Init(uint8_t* c, int, uint32_t id, uint8_t* type, uint32_t owner, float x, float z, uint32_t a6);
RT3_NOINLINE Collection* __fastcall Collection5c9810_Construct(Collection* c);
void __fastcall Collection5c9810_Destruct(Collection* c);
uint32_t __fastcall Cars_Add(Collection* t, int, uint8_t* type, uint32_t owner, float x, float z, uint32_t a6);
RT3_NOINLINE int32_t __fastcall PartModels41aae0_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Collection_SaveWithRecords(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE double One();
RT3_NOINLINE int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player);
RT3_NOINLINE void __fastcall Collection5c9824_Destruct(Collection* c);
void __fastcall CarTypes_LoadAll(Collection* t);
void __fastcall BuildingTypes_Delete(Collection* c, int, uint32_t id);
uint32_t __fastcall BuildingTypes_Add(Collection* c, int, uint8_t* rec, uint32_t size, uint32_t key2, uint32_t key3);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall PartModel_Destructor(uint8_t* o);
RT3_NOINLINE void __fastcall Car_Save(uint8_t* c, int, Stream s);
RT3_NOINLINE void __fastcall Car_LoadModel(uint8_t* c, int, const char* name, const char* tex, float x, float z, int32_t reset);
RT3_NOINLINE void __fastcall Vehicle_Teardown(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Car_Load(uint8_t* c, int, int32_t s, char* texSet, const char* name, uint8_t* type);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
int32_t __fastcall Track_LinksWithin(uint8_t* n, int, int32_t target, int32_t depth, int32_t from);
RT3_NOINLINE float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
RT3_NOINLINE uint8_t* __fastcall Model_NextFrame(uint8_t* o);

static inline void* RefNotMinus1(Collection* t, int32_t id) { return id != -1 ? Collection_Get(t, 0, id) : nullptr; }

static inline void Virtual(void* o, uint32_t slot) { ((void(__thiscall*)(void*))(*(void***)o)[slot / 4])(o); }

static inline double ExeSin(double v) { return ((double(__cdecl*)(double))0x5a1494)(v); }

static inline double ExeCos(double v) { return ((double(__cdecl*)(double))0x5a13e4)(v); }

static inline Collection* Coll(uint32_t a) { return hook::Global<Collection*>(a); }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline Collection* Cargos() { return hook::Global<Collection*>(0x62ba8c); }

// Formats string id with one argument and exits with it.
static void FatalFormat(int32_t id, const char* arg) {
    char msg[0x12c];
    FormatText(msg, GetString(id), arg, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Fatal_Exit(msg);
}

static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

static inline double Fl_193(uint32_t a) { return *(const float*)a; }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

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

namespace {
void Callback_Delete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
int32_t Tooltip_Clear() { return Ui_ShowTooltip(hook::Global<uint8_t*>(0x6d401c), 0, (const char*)0x5c87a8, 3, 0, -1); }
// A network message's handler gets the packet (+8) when it has one (+0xc).
template <typename P, typename Fn> int32_t NetMsg_Forward(uint8_t* m, Fn handler) {
    if (F<int32_t>(m, 0xc) > 0) handler(F<P>(m, 8));
    return 1;
}
}

namespace {
void Free445(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
int Stricmp445(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
int32_t Companies_ClashCall(const void* d) {
    int32_t r;
    __asm {
        push ebx
        mov ebx, d
        call Companies_Clash
        pop ebx
        mov r, eax
    }
    return r;
}
}

namespace {
inline double Fabs482(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
}

// 0x41a950. Building type table t: releases every type, then loads the
// type files again.
RT3_NOINLINE void __fastcall BtyTable_Reload(Collection* t) {
    while (Collection_Count(t)) {
        CollectionVtbl* vt = t->vtbl;
        vt->release(t, Collection_IdAt(t, 0, 0, 0));
    }
    BuildingTypes_LoadFiles(t);
}
RT3_REPLACE(0x41a950, BtyTable_Reload);

// 0x41a990. Building type table t: creates its name hash (+0x88, 0x65
// buckets) and two node pools (+0x90 of 8-byte, +0x8c of 0x74e-byte nodes),
// then loads the type files. (0x62b304 is cleared first.)
void __fastcall BtyTable_Setup(Collection* t) {
    hook::Global<int32_t>(0x62b304) = 0;
    auto alloc = (uint8_t*(__cdecl*)(size_t))0x53b070;
    uint8_t* h = alloc(0x14);
    if (h) memset(h, 0, 0x14);
    F<uint8_t*>(t, 0x88) = h;
    Hash_Init((HashMap*)h, 0, 0x65, 0);
    uint8_t* p1 = alloc(0x1c);
    if (p1) memset(p1, 0, 0x1c);
    F<uint8_t*>(t, 0x90) = p1;
    Pool_Init((NodePool*)p1, 0, nullptr, 8, 0, 0);
    uint8_t* p2 = alloc(0x1c);
    if (p2) memset(p2, 0, 0x1c);
    F<uint8_t*>(t, 0x8c) = p2;
    Pool_Init((NodePool*)p2, 0, nullptr, 0x74e, 0, 0);
    BuildingTypes_LoadFiles(t);
}
RT3_REPLACE(0x41a990, BtyTable_Setup);

// 0x41aa50. Constructs building type table t (vtable 0x5c9718, inline
// 0x1a7-byte records, room for 10, growing by 5): clears the port and
// warehouse lists (0x62b2f0..0x62b2f8), the year range (+0xa0 / +0xa4 =
// +/-999999) and 0x6cec74+0x4fd, then sets it up. Returns t.
RT3_NOINLINE Collection* __fastcall BtyTable_Construct(Collection* t) {
    Collection_SetVtbl(t);
    F<uint32_t>(t, 0) = 0x5c9718;
    hook::Global<int32_t>(0x62b2f0) = 0;
    hook::Global<int32_t>(0x62b2f4) = 0;
    hook::Global<int32_t>(0x62b2f8) = 0;
    F<int32_t>(t, 0x98) = 1;
    F<int32_t>(t, 0x88) = 0;
    F<int32_t>(t, 0x90) = 0;
    F<int32_t>(t, 0x8c) = 0;
    F<int32_t>(t, 0x9c) = 0;
    F<uint32_t>(t, 0xa0) = 0x497423f0;
    F<uint32_t>(t, 0xa4) = 0xc97423f0;
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x4fd) = 0;
    Collection_Init(t, 0, 1, 0x1a7, 0xa, 5, 1, 0, 0);
    BtyTable_Setup(t);
    return t;
}
RT3_REPLACE(0x41aa50, BtyTable_Construct);

RT3_NOINLINE uint8_t* __fastcall PartModel41aae0_Construct(uint8_t* o) {
    PartModel_Construct(o);
    F<uint32_t>(o, 0) = 0x5c9748;
    return o;
}
RT3_REPLACE(0x41aae0, PartModel41aae0_Construct);

RT3_NOINLINE void __fastcall PartModel41aae0_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5c9748;
    PartModel_Destructor(o);
}
RT3_REPLACE(0x41ab00, PartModel41aae0_Destructor);

// 0x41ab10 / 0x4566f0 / 0x4742e0 / 0x41f7b0 (callbacks). Clear the tooltip
// (0x4742e0 shows string 0x123; 0x41f7b0 also closes the overview of a
// map in mode 0x12).
int32_t Tooltip_Clear41ab10() { return Tooltip_Clear(); }
RT3_REPLACE(0x41ab10, Tooltip_Clear41ab10);

void __fastcall Obj_Toggle35a(void* o) { F<int32_t>(o, 0x35a) = F<int32_t>(o, 0x35a) == 0 ? 1 : 0; }
RT3_REPLACE(0x41ab30, Obj_Toggle35a);

// 0x41ab50 (vtable stub).
int32_t __fastcall Return26_41ab50(void*) { return 0x26; }
RT3_REPLACE(0x41ab50, Return26_41ab50);

// 0x41ab60 (vtable stub).
int32_t __fastcall Return22_41ab60(void*) { return 0x22; }
RT3_REPLACE(0x41ab60, Return22_41ab60);

// 0x41ab70 (vtable stub).
// Forwards to Car_Save (0x45c6f0).
void __fastcall Thunk41ab70(uint8_t* p0, int p1, Stream p2) { Car_Save(p0, p1, p2); }
RT3_REPLACE(0x41ab70, Thunk41ab70);

// 0x41ab80. First model angle scaled by 0.18.
RT3_NOINLINE float __fastcall Car_TiltValue(uint8_t* c) {
    float a;
    Model_GetAngles(c, 0, (uint32_t*)&a, nullptr, nullptr);
    return (float)((double)a * *(const double*)0x5c97c0);
}
RT3_REPLACE(0x41ab80, Car_TiltValue);

// 0x41abb0 (vtable). Sets o's texture name (+0x332, +0x20e) and reloads its
// texture (+0x1c) from the pack 0x6d4020.
RT3_NOINLINE void __fastcall Obj_SetTexture(uint8_t* o, int, const char* name) {
    String_Assign((char**)(o + 0x332), name);
    String_Assign((char**)(o + 0x20e), name);
    if (void* t = F<void*>(o, 0x1c)) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, t);
    F<uint8_t*>(o, 0x1c) = TextureCache_Get(hook::Global<uint8_t*>(0x6d4020), 0, name, 0, 0, 0x7f);
}
RT3_REPLACE(0x41abb0, Obj_SetTexture);

// 0x41ac00 (vtable stub).
int32_t __fastcall Return0_41ac00(void*) { return 0; }
RT3_REPLACE(0x41ac00, Return0_41ac00);

// 0x41ac10 (vtable stub).
int32_t __fastcall Return0_41ac10(void*) { return 0; }
RT3_REPLACE(0x41ac10, Return0_41ac10);

// 0x41ac20 (vtable stub).
int32_t __fastcall Return0_41ac20(void*) { return 0; }
RT3_REPLACE(0x41ac20, Return0_41ac20);

// 0x41ac30 (vtable stub).
int32_t __fastcall Returna_41ac30(void*) { return 0xa; }
RT3_REPLACE(0x41ac30, Returna_41ac30);

// 0x41ac40 (vtable stub).
int32_t __fastcall Return1_41ac40(void*) { return 0x1; }
RT3_REPLACE(0x41ac40, Return1_41ac40);

// 0x41ac50 (vtable stub).
int32_t __fastcall Return1_41ac50(void*) { return 0x1; }
RT3_REPLACE(0x41ac50, Return1_41ac50);

// 0x41ac60. Resolves the car's type name (+0x36a) in 0x62ba88 (names +0x82) into
// +0x388; fatal (string 0xc1) if unknown.
RT3_NOINLINE void __fastcall Car_ResolveType(uint8_t* o) {
    F<uint32_t>(o, 0x388) = 0;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62ba88)); ++i) {
        Collection* c = hook::Global<Collection*>(0x62ba88);
        if (Stricmp((const char*)o + 0x36a, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)) + 0x82)) continue;
        F<uint32_t>(o, 0x388) = Collection_IdAt(hook::Global<Collection*>(0x62ba88), 0, i, 0);
        break;
    }
    if (!F<uint32_t>(o, 0x388)) FatalFormat(0xc1, (const char*)o + 0x36a);
}
RT3_REPLACE(0x41ac60, Car_ResolveType);

// 0x41ad30. Car c (train +0x390) is clicked: a sound (0x21), the train
// window opens on its train and the camera follows the train's lead car.
void __fastcall Car_OpenTrain(uint8_t* c) {
    if (F<int32_t>(c, 0x390) == -1) return;
    Ui_PlaySound(0x21, 0, 0x3f000000);
    ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4ddbd0)(hook::Global<uint8_t*>(0x6d0818), 0, 2, F<int32_t>(c, 0x390));
    int32_t lead = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(c, 0x390)), 0x41);
    World_FollowEntity(hook::Global<uint8_t*>(0x6cec78), 0, 1, 1, lead);
}
RT3_REPLACE(0x41ad30, Car_OpenTrain);

RT3_NOINLINE void* __fastcall Obj_City352(void* o) { return RefNotMinus1(g_cities, F<int32_t>(o, 0x352)); }
RT3_REPLACE(0x41ad90, Obj_City352);

// 0x41adb0. The table-0x62ba88 record referenced at +0x388.
RT3_NOINLINE void* __fastcall Obj_Ref388(void* o) { return Collection_Get(g_table62ba88, 0, F<uint32_t>(o, 0x388)); }
RT3_REPLACE(0x41adb0, Obj_Ref388);

// 0x41add0. The car's rear coupling point: its cell position moved back by
// half its type length (+0x10d) along its heading.
void __fastcall Car_RearPoint(uint8_t* c, int, float* x, float* y) {
    float half = (float)((double)F<float>(Obj_Ref388(c), 0x10d) * *(const double*)0x5c8628);
    *x = (float)Obj_CellX(c);
    *y = (float)Obj_CellY(c);
    float a = SceneObj_Angle2(c);
    float h = (float)((double(__cdecl*)(double, double))0x5a1390)((double)a + *(const double*)0x5c8740, 6.2831854820251465);
    *x = (float)((double)*x - ExeSin(h) * half);
    *y = (float)((double)*y - ExeCos(h) * half);
}
RT3_REPLACE(0x41add0, Car_RearPoint);

// 0x41aea0. Links this vehicle (+0x390, +0x34a) into track node `id` and,
// where still free, into its two neighbours (on the matching side).
RT3_NOINLINE void __fastcall Car_LinkToNode(uint8_t* car, int, int32_t id, int32_t side) {
    if (id == -1) return;
    uint8_t* n = (uint8_t*)Collection_Get(g_trackNodes, 0, id);
    Node_SetLinkPair(n, 0, F<int32_t>(car, 0x390), F<int32_t>(car, 0x34a), side);
    if (F<int32_t>(n, 0x206) != -1) {
        uint8_t* m = (uint8_t*)Collection_Get(g_trackNodes, 0, F<uint32_t>(n, 0x206));
        int32_t s = side;
        if (F<int32_t>(m, 0x206) == id || (F<int32_t>(m, 0x20e) == id && (m[0x201] & 2))) s = side == 0;
        if (Track_Field238(m, 0, s) == -1) Node_SetLinkPair(m, 0, F<int32_t>(car, 0x390), F<int32_t>(car, 0x34a), s);
    }
    if (F<int32_t>(n, 0x20a) != -1) {
        uint8_t* m = (uint8_t*)Collection_Get(g_trackNodes, 0, F<uint32_t>(n, 0x20a));
        int32_t s = side;
        if (F<int32_t>(m, 0x20a) == id || (F<int32_t>(m, 0x20e) == id && !(m[0x201] & 2))) s = side == 0;
        if (Track_Field238(m, 0, s) == -1) Node_SetLinkPair(m, 0, F<int32_t>(car, 0x390), F<int32_t>(car, 0x34a), s);
    }
}
RT3_REPLACE(0x41aea0, Car_LinkToNode);

// 0x41afc0. Removes this vehicle's link (+0x390, +0x34a) from its track node
// (+0x356) and that node's three neighbours.
RT3_NOINLINE void __fastcall Obj_UnlinkFromNode(void* o) {
    if (F<int32_t>(o, 0x356) == -1) return;
    uint8_t* n = (uint8_t*)Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 0x356));
    Node_ClearLinks(n, 0, F<int32_t>(o, 0x390), F<int32_t>(o, 0x34a));
    static const uint32_t kNeighbours[3] = { 0x206, 0x20a, 0x20e };
    for (uint32_t off : kNeighbours) {
        int32_t id = F<int32_t>(n, off);
        if (id == -1) continue;
        Node_ClearLinks(Collection_Get(g_trackNodes, 0, id), 0, F<int32_t>(o, 0x390), F<int32_t>(o, 0x34a));
    }
}
RT3_REPLACE(0x41afc0, Obj_UnlinkFromNode);

RT3_NOINLINE void* __fastcall Obj_Train390(void* o) {
    int32_t id = F<int32_t>(o, 0x390);
    return id != -1 ? Collection_Get(g_trains, 0, id) : nullptr;
}
RT3_REPLACE(0x41b8f0, Obj_Train390);

double __fastcall Obj_Ref388Float10d(void* o) {
    return F<float>(Collection_Get(g_table62ba88, 0, F<uint32_t>(o, 0x388)), 0x10d);
}
RT3_REPLACE(0x41b910, Obj_Ref388Float10d);

// 0x41b930. Virtual slot 5 of this vehicle's track node (+0x356); 0 if none.
uint32_t __fastcall Obj_NodeVirtual5(void* o) {
    if (F<int32_t>(o, 0x356) == -1) return 0;
    void* n = Collection_Get(g_trackNodes, 0, F<uint32_t>(o, 0x356));
    return ((uint32_t(__thiscall*)(void*))(*(void***)n)[0x14 / 4])(n);
}
RT3_REPLACE(0x41b930, Obj_NodeVirtual5);

// 0x41b960. Cargo type (+0x38c) record, or 0.
RT3_NOINLINE void* __fastcall Obj_CargoType(void* o) {
    uint32_t id = F<uint32_t>(o, 0x38c);
    if (!id) return nullptr;
    return Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, id);
}
RT3_REPLACE(0x41b960, Obj_CargoType);

// 0x41b980. Sets +0x39c; a value <= 0 becomes 1000..8000 (HashRand on +0x34a)
// when option +0x130 is on.
RT3_NOINLINE void __fastcall Obj_SetAmount39c(void* o, int, float v) {
    F<float>(o, 0x39c) = v;
    if (!(v <= *(const float*)0x5c8598)) return;
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x130)) return;
    F<float>(o, 0x39c) = (float)(HashRand(F<int32_t>(o, 0x34a), 1, 8) * 1000);
}
RT3_REPLACE(0x41b980, Obj_SetAmount39c);

// 0x41b9e0. Sets the car's cargo value (+0x39c) on arrival at city: the quota for
// its cargo (+0x38c) x amount (+0x398) less the paid price (+0x3a4); nothing when
// it is the region it came from (train +0x390, building +0x9c) or the cargo is not
// produced (multiplayer games of version 0x5c9800+ / single player). Then scales
// +0x3a0 by the mode's factor.
void __fastcall Car_SetCargoValue(uint8_t* car, int, uint8_t* city) {
    int32_t mode;
    if (IsMultiplayer()) mode = !(Net_CommonVersion() >= Fl(0x5c9800)) ? 0 : 1;
    else mode = 2;
    if (!F<uint32_t>(car, 0x38c)) return;
    if (!(F<float>(car, 0x398) > Fl(0x5c8598))) return;
    float value = 0.0f;
    if (city) {
        uint8_t* from = nullptr;
        int32_t tid = F<int32_t>(car, 0x390);
        uint8_t* t = tid != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, tid) : nullptr;
        if (Collection_IsValid(hook::Global<Collection*>(0x6cec20), 0, F<uint32_t>(t, 0x9c)))
            from = Obj_BuildingRegion(Collection_Get(hook::Global<Collection*>(0x6cec20), 0,
                                                     F<uint32_t>(Obj_Train390(car), 0x9c)));
        if (mode < 1 || (city != from && F<int32_t>(Cargos(), 0x9a + F<int32_t>(car, 0x38c) * 4) == -1)) {
            float q = (float)City_CargoQuota(city, 0, F<int32_t>(car, 0x38c));
            value = (float)((double)q * F<float>(car, 0x398) - F<float>(car, 0x3a4));
        }
    }
    Obj_SetAmount39c(car, 0, value);
    static const uint32_t factor[3] = { 0x5c8750, 0x5c85a0, 0x5c97f8 };
    F<float>(car, 0x3a0) = (float)((double)F<float>(car, 0x3a0) * D(factor[mode]));
}
RT3_REPLACE(0x41b9e0, Car_SetCargoValue);

// 0x41bb40. Loads a not-yet-full car (+0x38c cargo, +0x390 train, fill +0x398) at
// station `at` bound for `to`: picks up cargo for its free room, raising the fill
// (max 1) and averaging the load age (+0x3a0); for cargo without a city quota the
// unmet demand at `to` is booked (+0x3a4). The car's waiting time (+0x39c) is then
// re-rolled (1..20 s, in some game modes) and grows by the unmet amount x both
// stations' kind factors x the company's quality blend for the difficulty.
void __fastcall Car_LoadAt(uint8_t* car, int, uint8_t* at, uint8_t* to) {
    if (!(F<float>(car, 0x398) < D(0x5c9808)) && !(F<float>(car, 0x398) != F<float>(car, 0x398))) return;
    if (F<float>(car, 0x39c) < Fl_193(0x5c8598)) F<float>(car, 0x39c) = 0.0f;
    int32_t cargo = F<int32_t>(car, 0x38c);
    float fill = F<float>(car, 0x398);
    uint8_t* train = F<int32_t>(car, 0x390) != -1
                         ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(car, 0x390))
                         : nullptr;
    float room = (float)(D(0x5c8770) - fill);
    float got, value, amount;
    Station_PickUp(at, 0, train, to, cargo, room, &got, &value, &amount);
    float unmet = 0.0f;
    if (got != 0.0f || got != got) {
        float t = (float)((double)got + F<float>(car, 0x398));
        float nf = Fl_193(0x5c85b4) < t ? 1.0f : t;
        auto denom = [&]() { return nf < D(0x5c8678) ? 0.001 : (double)nf; };
        if (F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + F<int32_t>(car, 0x38c) * 4) != -1) {
            double dv = denom();
            unmet = amount;
            F<float>(car, 0x3a0) = (float)(((double)F<float>(car, 0x3a0) * F<float>(car, 0x398) + got) / dv);
        } else {
            float quota = (float)City_CargoQuota(Obj_BuildingRegion(to), 0, F<int32_t>(car, 0x38c));
            float w = (float)((double)quota * got - value);
            float u = w < Fl_193(0x5c8598) ? 0.0f : w;
            double dv = denom();
            unmet = u;
            F<float>(car, 0x3a0) = (float)(((double)F<float>(car, 0x3a0) * F<float>(car, 0x398) + got) / dv);
            F<float>(car, 0x3a4) = (float)((double)value + F<float>(car, 0x3a4));
        }
        F<float>(car, 0x398) = nf;
    }
    if (F<float>(car, 0x39c) < Fl_193(0x5c8598) || F<float>(car, 0x39c) == Fl_193(0x5c8598)) {
        if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x130) || hook::Global<uint8_t*>(0x6cec7c)[0x82] ||
            F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4aef))
            F<float>(car, 0x39c) = (float)(HashRand(F<int32_t>(car, 0x34a), 4, 0x14) * 1000);
    }
    uint8_t* ka = at ? (uint8_t*)Obj_BuildingKind(at) : nullptr;
    uint8_t* kb = to ? (uint8_t*)Obj_BuildingKind(to) : nullptr;
    float m = 1.0f;
    if (ka) m = (float)(((double)F<float>(ka, 0x31b) + D(0x5c8770)) / D(0x5c85d0));
    if (kb) m = (float)(((double)F<float>(kb, 0x31b) + D(0x5c8770)) / D(0x5c85d0) * m);
    float x = (float)((double)m * unmet);
    train = F<int32_t>(car, 0x390) != -1
                ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(car, 0x390))
                : nullptr;
    float q = (float)Company_QualityRaw(Train_Company(train));
    uint32_t idx = hook::Global<uint8_t*>(0x6cec7c)[0x7d] * 4;
    uint32_t ta = 0x5f3cb8, tb = 0x5f3cc8;
    if (Game_EffectiveVersion() >= 0x67) {
        ta = 0x5f3c98;
        tb = 0x5f3ca8;
    }
    float blend = (float)(Fl_193(ta + idx) * (D(0x5c8770) - q) + (double)q * Fl_193(tb + idx));
    x = (float)((double)blend * x);
    F<float>(car, 0x39c) = (float)((double)x + F<float>(car, 0x39c));
}
RT3_REPLACE(0x41bb40, Car_LoadAt);

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

// 0x41bf90. Rate +0x39c * +0x3a0 (as float); x1.2 when this is the special
// kind (u16 +0x88 of object 0x62ba8c) and its train (+0x390) has flag +0x198.
RT3_NOINLINE double __fastcall Obj_Rate(void* o) {
    float f = (float)((double)F<float>(o, 0x3a0) * F<float>(o, 0x39c));
    if (F<int32_t>(o, 0x38c) != F<uint16_t>(hook::Global<uint8_t*>(0x62ba8c), 0x88)) return f;
    int32_t id = F<int32_t>(o, 0x390);
    volatile uint8_t* t = id != -1 ? (uint8_t*)Collection_Get(g_trains, 0, id) : nullptr;
    if (t[0x198]) f = (float)(f * *(const double*)0x5c8698); // QUIRK: no train reads address 0x198
    return f;
}
RT3_REPLACE(0x41bf90, Obj_Rate);

RT3_NOINLINE int32_t __fastcall Obj_NodeNear356(uint8_t* o, int, int32_t id) {
    int32_t n = F<int32_t>(o, 0x356);
    if (n == -1) return 0;
    if (id == n) return 1;
    return Track_LinksWithin((uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, n), 0, id, 2, -1) ? 1 : 0;
}
RT3_REPLACE(0x41c010, Obj_NodeNear356);

// 0x41c060. The car's skin texture: a named livery (+0x3ac, while +0x3cc) from
// its type's list (+0x196 / +0x19a), else the type's base skin (+0x3d) shifted by
// 4 per cargo load level (cargo +0x20, 1..8).
RT3_NOINLINE uint32_t __fastcall Car_SkinTexture(uint8_t* car) {
    Collection* types = hook::Global<Collection*>(0x62ba88);
    Collection* cargos = hook::Global<Collection*>(0x62ba8c);
    if (car[0x3cc]) {
        for (int32_t i = 0; i < Collection_Count(F<Collection*>(Collection_Get(types, 0, F<uint32_t>(car, 0x388)), 0x196)); ++i) {
            Collection* names = F<Collection*>(Collection_Get(types, 0, F<uint32_t>(car, 0x388)), 0x196);
            const char* name = (const char*)Collection_Get(names, 0, Collection_IdAt(names, 0, i, 0));
            if (!Stricmp((const char*)car + 0x3ac, name)) {
                uint8_t* t = (uint8_t*)Collection_Get(types, 0, F<uint32_t>(car, 0x388));
                return Table_Entry(F<uint8_t*>(t, 0x19a + i * 4), 0, 0);
            }
        }
        car[0x3cc] = 0;
    }
    int32_t idx = F<int32_t>(Collection_Get(types, 0, F<uint32_t>(car, 0x388)), 0x3d);
    if (F<uint32_t>(car, 0x38c) && Collection_Get(cargos, 0, F<uint32_t>(car, 0x38c))) {
        int8_t lvl = (int8_t)((uint8_t*)Collection_Get(cargos, 0, F<uint32_t>(car, 0x38c)))[0x20];
        if (lvl > 0 && lvl <= 8) idx += lvl * 4;
    }
    uint8_t* t = (uint8_t*)Collection_Get(types, 0, F<uint32_t>(car, 0x388));
    return Table_Entry(F<uint8_t*>(t, 0x12a), 0, idx);
}
RT3_REPLACE(0x41c060, Car_SkinTexture);

// 0x41c1d0. Car's status text for its train (+0x390) into out, by its cargo
// class (0x62ba88 +0x388, +0x41): 1 freight - cargo, value (+0x398 x
// 0x5c8c28) and rate (0xc4), the train's bonus note (0xdbb) for local cargo,
// with 0x6cec7c+0x82 just the cargo type's name; 0 empty (0xc5); 4
// passengers - the rate minus its share 0x5c8698 (0xc6 + 0xdbc), with
// 0x6cec7c+0x82 0x38f; 5 (0xb5b); 2/3 the train's city cargo (0xc3, its
// age in days and +0x20); else "". Without a train, or a 2/3 car of a
// train without a city, out is left alone.
RT3_NOINLINE void __fastcall Car_StatusText(uint8_t* car, int, char* out) {
    if (F<int32_t>(car, 0x390) == -1) return;
    uint8_t* train = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(car, 0x390));
    if (!train) return;
    int32_t kind = F<int32_t>(Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(car, 0x388)), 0x41);
    char a[0x40], b[0x40];
    if (kind == 3 || kind == 2) {
        uint8_t* city = (uint8_t*)Obj_CityRef(train);
        if (!city) return;
        uint32_t pr = F<uint32_t>(city, 0x72);
        int32_t age = F<int32_t>(train, 0xd4);
        const char* nm = LocalizeName((const char*)(pr ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, pr) : nullptr) + 0x45);
        int32_t days = (int32_t)F<uint16_t>(hook::Global<uint8_t*>(0x6cec78), 0xd) - age;
        const char* sd = Num_Format(a, 8, (double)days, 0xa, 1, 0, 0);
        const char* sv = Num_Format(b, 1, (double)Ftol((double)F<float>(city, 0x20)), 0xa, 1, 0, 0);
        FormatText(out, GetString(0xc3), nm, sv, sd, nullptr, nullptr, nullptr, nullptr, nullptr);
        return;
    }
    if (kind == 1) {
        float rate = Obj_Rate(car) < hook::Global<float>(0x5c8598) ? 0.0f : (float)Obj_Rate(car);
        float value = F<float>(car, 0x398);
        uint32_t ck = F<uint32_t>(car, 0x38c);
        const char* cargo = LocalizeName((const char*)(ck ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba8c), 0, ck) : nullptr) + 4);
        char r[0x40];
        const char* sr = Num_FormatSimple2(r, 0xb, (double)rate, 0xb);
        const char* sv = Num_Format(b, 8, (double)Ftol((double)value * Fl_207(0x5c8c28)), 0xa, 1, 0, 0);
        FormatText(out, GetString(0xc4), cargo, sv, sr, nullptr, nullptr, nullptr, nullptr, nullptr);
        if (F<int32_t>(car, 0x38c) == F<uint16_t>(hook::Global<uint8_t*>(0x62ba8c), 0x88) && train[0x198]) strcat(out, GetString(0xdbb));
        if (!hook::Global<uint8_t*>(0x6cec7c)[0x82]) return;
        strcpy(out, LocalizeName((const char*)Obj_CargoType(car) + 4));
        return;
    }
    if (kind == 0) {
        strcpy(out, GetString(0xc5));
        return;
    }
    if (kind == 4) {
        strcpy(out, GetString(0xc6));
        double rate = Train_CargoRate(train, 0, 1);
        float v = (float)(rate - rate / D(0x5c8698));
        if (!(v > hook::Global<float>(0x5c8598))) v = 0.0f;
        char n[0x40], line[0x100];
        const char* s = Num_FormatSimple(n, 9, (double)v, 0xb);
        FormatText(line, GetString(0xdbc), s, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        strcat(out, line);
        if (hook::Global<uint8_t*>(0x6cec7c)[0x82]) strcpy(out, GetString(0x38f));
        return;
    }
    if (kind == 5) {
        strcpy(out, GetString(0xb5b));
        return;
    }
    out[0] = 0;
}
RT3_REPLACE(0x41c1d0, Car_StatusText);

// 0x41c5d0. Car o: leaves its node (+0x356) and tears the vehicle down.
RT3_NOINLINE void __fastcall Car_Teardown(uint8_t* o) {
    if (F<int32_t>(o, 0x356) != -1) Obj_UnlinkFromNode(o);
    Vehicle_Teardown(o);
}
RT3_REPLACE(0x41c5d0, Car_Teardown);

// 0x41c5f0. Shows the car's cargo load: the cargo type's sprite frame (+0x3a)
// on part 0 when the car type (+0x388) wants it (+0x12e), and model part +0x20.
RT3_NOINLINE void __fastcall Car_ShowCargoModel(uint8_t* c) {
    uint32_t id = F<uint32_t>(c, 0x38c);
    if (id) {
        int32_t frame = F<int32_t>(Collection_Get(Coll(0x62ba8c), 0, id), 0x3a);
        if (frame) {
            uint8_t* t = (uint8_t*)Collection_Get(Coll(0x62ba88), 0, F<uint32_t>(c, 0x388));
            if (t[0x12e]) Model_SetPartFrame(c, 0, 0, frame);
        }
    }
    if (!F<uint32_t>(c, 0x38c) || !Collection_Get(Coll(0x62ba8c), 0, F<uint32_t>(c, 0x38c))) return;
    uint8_t* cargo = (uint8_t*)Collection_Get(Coll(0x62ba8c), 0, F<uint32_t>(c, 0x38c));
    if ((int8_t)cargo[0x20] <= 0) return;
    cargo = (uint8_t*)Collection_Get(Coll(0x62ba8c), 0, F<uint32_t>(c, 0x38c));
    Model_ShowOnlyPart(c, 0, (int8_t)cargo[0x20]);
}
RT3_REPLACE(0x41c5f0, Car_ShowCargoModel);

// 0x41c6b0 (vtable). Shows car c's status as a tooltip (not while the
// bulldozer tool runs).
void __fastcall Car_ShowStatusTooltip(uint8_t* c) {
    if (hook::Global<uint8_t*>(0x6d1a8c) && Global622b0cIs64()) return;
    char text[0x1f8];
    Car_StatusText(c, 0, text);
    Ui_ShowTooltip(hook::Global<uint8_t*>(0x6d401c), 0, text, 3, 0, -1);
}
RT3_REPLACE(0x41c6b0, Car_ShowStatusTooltip);

// 0x41c8c0. Detaches every city in the chain from +0x352 (virtual slot 4 and
// node unlink), clears this train's link in its node (+0x4d), then this's
// virtual slot 4.
void __fastcall Obj_DetachChain(void* o) {
    int32_t id = F<int32_t>(o, 0x352);
    if (id != -1) {
        for (uint8_t* c = (uint8_t*)Collection_Get(g_cities, 0, id); c; ) {
            Virtual(c, 0x10);
            Obj_UnlinkFromNode(c);
            int32_t next = F<int32_t>(c, 0x352);
            if (next == -1) break;
            c = (uint8_t*)Collection_Get(g_cities, 0, next);
        }
    }
    if (Collection_IsValid(g_trains, 0, F<uint32_t>(o, 0x390))) {
        // The train is looked up twice, as in the original (a -1 ID reads through null).
        auto trainNode = [&]() -> uint32_t {
            int32_t t = F<int32_t>(o, 0x390);
            uint8_t* tr = t != -1 ? (uint8_t*)Collection_Get(g_trains, 0, t) : nullptr;
            return F<uint32_t>(tr, 0x4d);
        };
        if (Collection_IsValid(g_trackNodes, 0, trainNode())) {
            void* node = Collection_Get(g_trackNodes, 0, trainNode());
            Node_ClearLinks(node, 0, F<int32_t>(o, 0x390), -3);
        }
    }
    Virtual(o, 0x10);
}
RT3_REPLACE(0x41c8c0, Obj_DetachChain);

// 0x41c9a0 (vtable). Tooltip of car o into dst (n bytes): in debug mode
// (0x6cec74 +0x7c) string 0xc2 with its type's name (0x62ba88 +0x45) and
// speed (angle 0 x 0x5c97c0 x 0x5c8c28); otherwise the owner's logo in *logo
// (returning 2) when ownership shows: map mode +0x22f, or the open finder
// (0x6d12f0) in mode 4, or mode 3 on this car's train (+0x7c), for a car of
// its train (+0x390 / +0x34a) with an owner on a visible piece (+0x356),
// skipping the player's own trains in mode 1 unless the finder asks; else
// the generic tooltip.
int32_t __fastcall Car_Tooltip(uint8_t* o, int, char* dst, int32_t n, int32_t a, int32_t b, int32_t logo) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x7c)) {
        char frame[0x158];
        char* num = frame + 8;
        char* text = frame + 0x28;
        float angle;
        Model_GetAngles(o, 0, (uint32_t*)&angle, nullptr, nullptr);
        const char* name = LocalizeName((const char*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(o, 0x388)) + 0x45);
        const float speed = (float)((double)angle * D(0x5c97c0));
        Num_Format(num, 3, (double)speed * Fl_207(0x5c8c28), 10, 1, 0, 0);
        FormatText(text, GetString(0xc2), name, num, 0, 0, 0, 0, 0, 0);
        StrCopyN(dst, text, (size_t)(n - 1));
        return 1;
    }
    int32_t show = 0, hide = 0;
    if (uint8_t* f = hook::Global<uint8_t*>(0x6d12f0)) {
        const int32_t mode = F<int32_t>(f, 0x78);
        if (mode == 4) show = 1;
        if (mode == 3) {
            uint8_t* t = F<int32_t>(o, 0x390) != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(o, 0x390)) : nullptr;
            f = hook::Global<uint8_t*>(0x6d12f0);
            if (F<int32_t>(t, 0x41) == F<int32_t>(o, 0x34a) && F<int32_t>(t, 0x51) == F<int32_t>(f, 0x7c))
                show = 1;
            else
                hide = 1;
        }
    }
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x22f) || show) {
        uint8_t* t = F<int32_t>(o, 0x390) != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(o, 0x390)) : nullptr;
        if (F<int32_t>(t, 0x41) == F<int32_t>(o, 0x34a) && F<int32_t>(t, 0x51)) {
            bool hidden = false;
            if (F<int32_t>(o, 0x356) != -1) {
                uint8_t* piece = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(o, 0x356));
                hidden = ((int32_t(__thiscall*)(uint8_t*))(*(void***)piece)[5])(piece) != 0;
            }
            if (!hidden && !hide) {
                if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x22f) != 1 || F<int32_t>(t, 0x51) != Obj_Get21(hook::Global<uint8_t*>(0x6cec78)) || show) {
                    *(uint32_t*)(uintptr_t)logo = Company_Logo((uint8_t*)Train_Company(t));
                    return 2;
                }
            }
        }
    }
    return SceneObj_DebugTooltip(o, 0, dst, n, a, b, logo);
}
RT3_REPLACE(0x41c9a0, Car_Tooltip);

// 0x41cbc0. Pays out a delivered load (+0x398 units of cargo +0x38c at rate +0x3a0
// x +0x39c, scaled for passengers on +0x198 trains): shares the revenue,
// books it to the train's company with the train's speed and reliability, adds
// region cargos to the region's stock, and clears the load. Returns the revenue.
float __fastcall Load_PayOut(uint8_t* o) {
    uint8_t* t = F<int32_t>(o, 0x390) != -1 ? (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfcbc), 0, F<uint32_t>(o, 0x390)) : nullptr;
    uint8_t* r = Train_FirstCarRegion(t);
    float rev = 0.0f;
    if (F<float>(o, 0x398) > *(const float*)0x5c8598) {
        uint8_t* cargos = hook::Global<uint8_t*>(0x62ba8c);
        rev = (float)((double)F<float>(o, 0x3a0) * F<float>(o, 0x39c));
        int32_t cargo = F<int32_t>(o, 0x38c);
        if (cargo == F<uint16_t>(cargos, 0x88) && t[0x198]) rev = (float)(rev * D(0x5c8698));
        int32_t kind = F<int32_t>(cargos, 0x9a + cargo * 4) != -1;
        // Through the call site: DesyncWatch hooks it (track use income).
        hook::CallTarget<void(__fastcall*)(uint8_t*, int, float, int32_t, float, int32_t)>(0x41cc70)(
            t, 0, rev, kind, F<float>(o, 0x398), cargo);
        float h = (float)(F<float>(t, 0x39) * D(0x5c8628));
        float rel = F<float>(t, 0x3d);
        void* c = Train_Company(t);
        if (c) Obj_AddRevenue(c, 0, F<float>(o, 0x398), (float)((double)h * F<float>(o, 0x398)), rel, kind, F<int32_t>(o, 0x38c));
        int32_t cg = F<int32_t>(o, 0x38c);
        if (F<int32_t>(hook::Global<uint8_t*>(0x62ba8c), 0x9a + cg * 4) == -1)
            F<float>(r, 0x103 + cg * 4) = (float)((double)F<float>(r, 0x103 + cg * 4) + F<float>(o, 0x398));
    }
    F<uint32_t>(o, 0x3a0) = 0x3f800000;
    F<int32_t>(o, 0x39c) = 0;
    F<int32_t>(o, 0x398) = 0;
    F<int32_t>(o, 0x3a4) = 0;
    return rev;
}
RT3_REPLACE(0x41cbc0, Load_PayOut);

// 0x41cd10. Loads car c from stream s with its type (+0x388 in 0x62ba88),
// texture set +0x3cc and name +0x3ac, then resolves its type and shows its
// cargo model.
int32_t __fastcall Car_LoadState(uint8_t* c, int, int32_t s) {
    uint8_t* type = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba88), 0, F<uint32_t>(c, 0x388));
    int32_t r = Car_Load(c, 0, s, (char*)c + 0x3cc, (const char*)c + 0x3ac, type);
    Car_ResolveType(c);
    Car_ShowCargoModel(c);
    return r;
}
RT3_REPLACE(0x41cd10, Car_LoadState);

// 0x41cd60. Sets up car c of type `type` (model name +0x82, textures +0xa1)
// for owner `owner` at (x, z): ids a1 (+0x34a) / a3 (+0x38c) / a6 (+0x35a),
// state cleared, its model loaded afresh at scale 1, its name copied (+0x36a),
// then its type resolved and cargo model shown.
RT3_NOINLINE void __fastcall Car_Init(uint8_t* c, int, uint32_t id, uint8_t* type, uint32_t owner, float x, float z, uint32_t a6) {
    F<int32_t>(c, 0x34e) = -1;
    F<int32_t>(c, 0x352) = -1;
    F<int32_t>(c, 0x356) = -1;
    F<uint32_t>(c, 0x35a) = a6;
    F<int32_t>(c, 0x390) = -1;
    F<float>(c, 0x362) = x;
    F<float>(c, 0x366) = z;
    F<uint32_t>(c, 0x35e) = 0x3e395810;
    F<uint32_t>(c, 0x394) = F<uint32_t>(type, 0xfd);
    F<uint32_t>(c, 0x38c) = owner;
    F<uint32_t>(c, 0x34a) = id;
    F<uint32_t>(c, 0x398) = 0;
    F<uint32_t>(c, 0x3a4) = 0;
    F<uint32_t>(c, 0x39c) = 0;
    F<uint32_t>(c, 0x3a0) = 0x3f800000;
    F<uint32_t>(c, 0x3a8) = 0;
    F<uint32_t>(c, 0x33e) = 0;
    F<uint32_t>(c, 0x342) = 0;
    F<uint32_t>(c, 0x33a) = 0;
    c[0x3cc] = 0;
    memset(c + 0x3cd, 0, 7);
    Car_LoadModel(c, 0, (const char*)type + 0x82, (const char*)type + 0xa1, x, z, 1);
    F<uint32_t>(type, 0x192) = 0x3f800000;
    SceneObj_SetScale36(c, 0, 1.0f);
    strcpy((char*)c + 0x36a, (const char*)type + 0x82);
    Car_ResolveType(c);
    Car_ShowCargoModel(c);
}
RT3_REPLACE(0x41cd60, Car_Init);

// 0x41d2f0 / 0x41d320. Collection subclass 0x5c9810 (heap records, 10 + 20).
RT3_NOINLINE Collection* __fastcall Collection5c9810_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5c9810;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x41d2f0, Collection5c9810_Construct);

void __fastcall Collection5c9810_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9810;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x41d320, Collection5c9810_Destruct);

// 0x41d340. Adds a car (0x3d4-byte records) of type `type` to table t and
// sets it up (Car_Init); returns its id.
uint32_t __fastcall Cars_Add(Collection* t, int, uint8_t* type, uint32_t owner, float x, float z, uint32_t a6) {
    uint8_t tmp[0x3d4];
    PartModel41aae0_Construct(tmp);
    uint32_t id = Collection_Allocate(t, 0, tmp, 0x3d4, 0, 0);
    Car_Init((uint8_t*)Collection_Get(t, 0, id), 0, id, type, owner, x, z, a6);
    PartModel41aae0_Destructor(tmp);
    return id;
}
RT3_REPLACE(0x41d340, Cars_Add);

RT3_DELETE_RECORD(0x41d3c0, Cars_Delete, Car_Teardown, uint8_t*)

// 0x41d3f0. Loads the 0x41aae0 part models (frame 0x4a39 / 0x4a3a / 0x4a3b).
RT3_NOINLINE int32_t __fastcall PartModels41aae0_Load(Collection* t, int, Stream s, int32_t recordSize) {
    return LoadSceneObjects(t, s, recordSize, 0x4a39, 0x4a3a, 0x4a3b, [](uint8_t* o) { PartModel41aae0_Construct(o); },
                            [](uint8_t* o) { PartModel41aae0_Destructor(o); });
}
RT3_REPLACE(0x41d3f0, PartModels41aae0_Load);

// 0x41d4c0. Save of a collection whose records save themselves (virtual slot
// 17), framed by tags 0x4a39/0x4a3a/0x4a3b.
RT3_NOINLINE void __fastcall Collection_SaveWithRecords(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x4a39);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x4a3a);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, 0x4a3b);
}
RT3_REPLACE(0x41d4c0, Collection_SaveWithRecords);

RT3_NOINLINE double One() { return *(const float*)0x5c85b4; }

 // 1.0f
RT3_REPLACE(0x41d540, One);

// 0x41d550. Whether car type o (kind +0x41: 2/3 = cargo cars checked against the
// cargo's effects and era options) is available to company / player in this year
// (within +0x105..+0x109; a cargo car from its first year's July on).
RT3_NOINLINE int32_t __fastcall CarType_Available(uint8_t* o, int, uint32_t company, uint32_t player) {
    int32_t bonus = 0;
    int32_t k = F<int32_t>(o, 0x41);
    if (k == 2 || k == 3) {
        uint8_t* opts = hook::Global<uint8_t*>(0x6cec74);
        if (F<int32_t>(opts, 0x180)) return 1;
        if (World() && World()[0x4c8c]) return 1;
        if (hook::Global<uint8_t*>(0x6cec7c)[0x82] && F<int32_t>(opts, 0x247)) return 1;
        Collection* cargos = hook::Global<Collection*>(0x6ada84);
        if (!Collection_IsValid(cargos, 0, F<uint32_t>(o, 0x126))) return 0;
        uint8_t* t = (uint8_t*)Collection_Get(cargos, 0, F<uint32_t>(o, 0x126));
        int32_t id = F<int32_t>(o, 0x126) + 0xfb;
        int32_t allowed = 0;
        if (F<int32_t>(t, 0x10) == 0) bonus = Obj_CargoBonus(World(), 0, 0x1c6, company, player, 0);
        if (F<int32_t>(t, 0x10) == 1) bonus = Obj_CargoBonus(World(), 0, 0x1c7, company, player, 0);
        if (F<int32_t>(t, 0x10) == 2) bonus = Obj_CargoBonus(World(), 0, 0x1c8, company, player, 0);
        if (!Stricmp((const char*)t + 0x34, (const char*)0x5c981c)) bonus = 0;
        if (id >= 0xfb && id <= 0x15f) {
            int32_t b = Obj_CargoBonus(World(), 0, id, company, player, 0);
            if (b) {
                if (b < 0) return 0;
                return 1;
            }
        }
        if (bonus < 0) return 0;
        uint8_t* w = World();
        if (bonus > 0) {
            if (!w[0x4c97] && !w[0x4c98] && !w[0x4c99]) allowed = 1;
            else if ((t[0x30] && w[0x4c97]) || (t[0x31] && w[0x4c98]) || (t[0x32] && w[0x4c99])) allowed = 1;
        }
        if (!F<int32_t>(t, 0x7b) && !allowed) return 0;
        bonus = 1;
    }
    uint8_t* w = World();
    int32_t year = F<uint16_t>(w, 0xd);
    if (year < 0x726) year = 0x726;
    int32_t from = F<int32_t>(o, 0x105);
    bool fromOk;
    if (year == from) fromOk = w[0xf] >= 6 || !bonus;
    else fromOk = year > from;
    if (!fromOk) return 0;
    return year > F<int32_t>(o, 0x109) ? 0 : 1;
}
RT3_REPLACE(0x41d550, CarType_Available);

RT3_NOINLINE void __fastcall Collection5c9824_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9824;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x41d7a0, Collection5c9824_Destruct);

// 0x41d7c0. Car type table t: loads Data/EngineTypes/*.car in two passes -
// everything else first, then the 13 base models (u1l .. zephyrl). Each file
// is a version, a kind (+0x41), a dword, the version >= 0x3ea name (+0x45,
// else "XXX"), the cargo (+0xc0), the engine (+0x82) and model (+0xa1)
// names, a 0x3c-byte block (+0), and the numbers at +0x3d / +0x105 / +0x109
// / +0xfd (two single bytes are skipped twice). Kind 1 reads its cargo's
// .cgo for +0x101 and the 21-byte display name (+0x111, upper-cased); others
// copy +0xfd to +0x101. +0x192 = 1.0, +0x12e marks freight (not auto / pass
// / mail / stock / flat), +0x196 lists the engine's sounds (0x6d4020+0x429b8
// by name, their handles after it), +0x262 = -1; the record is added.
void __fastcall CarTypes_LoadAll(Collection* t) {
    auto Stricmp = (int(__cdecl*)(const char*, const char*))0x5a57cf;
    auto Strnicmp = (int(__cdecl*)(const char*, const char*, size_t))0x5a5f26;
    auto Fopen = (void*(__cdecl*)(const char*, const char*))0x5a1887;
    auto Fread = (size_t(__cdecl*)(void*, size_t, size_t, void*))0x5a1b05;
    auto Fclose = (int(__cdecl*)(void*))0x5a1740;
    auto FindFirst = (void*(__stdcall*)(const char*, void*))hook::Global<void*>(0x5c8190);
    auto FindNext = (int32_t(__stdcall*)(void*, void*))hook::Global<void*>(0x5c8194);
    auto FindClose_ = (int32_t(__stdcall*)(void*))hook::Global<void*>(0x5c819c);
    static const uint32_t kBase[13] = { 0x5c9934, 0x5c992c, 0x5c9920, 0x5c9910, 0x5c9900, 0x5c98f0, 0x5c98e0,
                                        0x5c98d0, 0x5c98c0, 0x5c98b0, 0x5c98a0, 0x5c9890, 0x5c9884 };
    uint8_t fd[0x140];
    char path[0x60];
    uint8_t rec[0x266];
    int32_t version, skip4;
    uint8_t b1, b2;
    for (int32_t pass = 0; pass < 2; ++pass) {
        void* h = FindFirst((const char*)0x5c993c, fd);
        if (h == (void*)-1) continue;
        do {
            const char* file = (const char*)fd + 0x2c;
            int32_t base = 0;
            for (uint32_t n : kBase)
                if (!Stricmp(file, (const char*)n)) {
                    base = 1;
                    break;
                }
            if ((pass == 0 && base) || (pass == 1 && !base)) continue;
            Loading_Pump(hook::Global<uint8_t*>(0x6cec74), 0, 0, 0);
            FormatText(path, (const char*)0x5c9870, file, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            void* f = Fopen(path, (const char*)0x5c9678);
            if (!f) continue;
            memset(rec, 0, sizeof rec);
            Fread(&version, 1, 4, f);
            Fread(rec + 0x41, 1, 4, f);
            Fread(&skip4, 1, 4, f);
            if (version >= 0x3ea) Fread(rec + 0x45, 1, 0x3c, f);
            else F<uint32_t>(rec, 0x45) = 0x585858;
            Fread(rec + 0xc0, 1, 0x3c, f);
            Fread(rec + 0x82, 1, 0x1e, f);
            Fread(rec + 0xa1, 1, 0x1e, f);
            Fread(rec, 1, 0x3c, f);
            Fread(rec + 0x3d, 1, 4, f);
            Fread(&b1, 1, 1, f);
            Fread(&b2, 1, 1, f);
            Fread(rec + 0x105, 1, 4, f);
            Fread(&b1, 1, 1, f);
            Fread(&b2, 1, 1, f);
            Fread(rec + 0x109, 1, 4, f);
            Fread(rec + 0xfd, 1, 4, f);
            if (F<int32_t>(rec, 0x41) == 1) {
                FormatText(path, (const char*)0x5c9858, (const char*)rec + 0xc0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                if (void* g = Fopen(path, (const char*)0x5c9678)) {
                    Fread(rec + 0x101, 1, 4, g);
                    Fread(rec + 0x111, 1, 0x15, g);
                    char* name = (char*)rec + 0x111;
                    for (int32_t i = (int32_t)strlen(name) - 1; i >= 0; --i) name[i] = (char)ToUpper(name[i]);
                    Fclose(g);
                }
            } else {
                F<uint32_t>(rec, 0x101) = F<uint32_t>(rec, 0xfd);
            }
            F<uint32_t>(rec, 0x192) = 0x3f800000;
            const char* cargo = (const char*)rec + 0xc0;
            rec[0x12e] = (!Strnicmp(cargo, (const char*)0x5c9850, 4) || !Strnicmp(cargo, (const char*)0x5c9848, 4) || !Strnicmp(cargo, (const char*)0x5c9840, 4) ||
                          !Strnicmp(cargo, (const char*)0x5c9838, 5) || !Strnicmp(cargo, (const char*)0x5c9830, 4))
                             ? 0
                             : 1;
            uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x88);
            Collection* sounds = mem ? Collection_Construct((Collection*)mem, 0, 1, 0x20, 0xa, 5, 0, 0, 0) : nullptr;
            F<Collection*>(rec, 0x196) = sounds;
            uint8_t* pack = hook::Global<uint8_t*>(0x6d4020);
            for (int32_t i = 0; i < Collection_Count(F<Collection*>(pack, 0x429b8)); ++i) {
                Collection* all = F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b8);
                uint8_t* e = (uint8_t*)Collection_Get(all, 0, Collection_IdAt(all, 0, (uint32_t)i, 0));
                if (Stricmp((const char*)rec + 0x82, (const char*)e + 0x20)) continue;
                sounds = F<Collection*>(rec, 0x196);
                sounds->vtbl->allocate(sounds, e, 0, 0, 0);
                uint8_t* hnd = Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)e + 0x40);
                F<uint8_t*>(rec, 0x196 + Collection_Count(F<Collection*>(rec, 0x196)) * 4) = hnd;
                pack = hook::Global<uint8_t*>(0x6d4020);
            }
            F<int32_t>(rec, 0x262) = -1;
            t->vtbl->allocate(t, rec, 0, 0, 0);
            Fclose(f);
        } while (FindNext(h, fd));
        FindClose_(h);
    }
}
RT3_REPLACE(0x41d7c0, CarTypes_LoadAll);

 // namespace

// 0x41ddc0. Deletes building type record `id`: releases its model (+0x12a)
// and the textures of its list (+0x196, entries +0x19a..), frees the list
// and the slot.
void __fastcall BuildingTypes_Delete(Collection* c, int, uint32_t id) {
    uint8_t* r = (uint8_t*)Collection_Get(c, 0, id);
    Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, F<void*>(r, 0x12a));
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(r, 0x196)); ++i) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, F<void*>(r, 0x19a + 4 * i));
    if (Collection* l = F<Collection*>(r, 0x196)) {
        Collection_Destruct(l);
        Free445(l);
    }
    F<Collection*>(r, 0x196) = nullptr;
    Collection_Release(c, 0, id);
}
RT3_REPLACE(0x41ddc0, BuildingTypes_Delete);

 // namespace

// 0x41de60 (building type collection vtable 0x5c9828). Adds type record rec:
// its height (+0x10d) is the rise of the first two frames of its length
// point model ("%1_LengthPoint.3dp" for the name +0x82) over 92, or 1 with
// a log line when the model is missing (0xca) or has no two frames (0xc9;
// QUIRK: that model is never freed); +0x12a gets its pack entry.
uint32_t __fastcall BuildingTypes_Add(Collection* c, int, uint8_t* rec, uint32_t size, uint32_t key2, uint32_t key3) {
    const char* name = (const char*)rec + 0x82;
    char path[0x12c], text[0x1e4];
    FormatText(path, (const char*)0x5c9954, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    uint32_t fail = 0xca;
    if (NameTable_ContainsName(hook::Global<uint8_t*>(0x6d4020), 0, path, 0, nullptr)) {
        uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x206);
        uint8_t* model = mem ? SceneObj_Construct(mem, 0, 0, path, nullptr) : nullptr;
        uint8_t* cur = nullptr;
        if (uint8_t* list = F<uint8_t*>(model, 0x14)) cur = F<uint8_t*>(list + 0x18 * F<int32_t>(model, 0x3a), 0xc) + F<int32_t>(model, 0x21) * 0x45;
        uint8_t* next = Model_NextFrame(model);
        if (cur && next) {
            const float from = F<float>(F<uint8_t*>(cur, 0), 8), to = F<float>(F<uint8_t*>(next, 0), 8);
            SceneObj_Destructor(model);
            ((void(__cdecl*)(void*))0x53b080)(model);
            F<float>(rec, 0x10d) = (float)(Fabs482(to - from) / D(0x5c8c48));
            fail = 0;
        } else {
            fail = 0xc9;
        }
    }
    if (fail) {
        FormatText(text, GetString(fail), name, path, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        DebugLog(text, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2, -0x4d2);
        F<uint32_t>(rec, 0x10d) = 0x3f800000;
    }
    F<uint32_t>(rec, 0x12a) = (uint32_t)(uintptr_t)Pack_AcquireNamed(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)rec);
    return Collection_Allocate(c, 0, rec, size, key2, key3);
}
RT3_REPLACE(0x41de60, BuildingTypes_Add);

}  // namespace rt3
