// Effects.cpp - RT3.exe 0x474000-0x476000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/crt.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
int32_t Global6d07b4Set2();
uint8_t* __fastcall NamedObj_Construct(uint8_t* o);
void __fastcall NamedObj_Destructor(uint8_t* o);
void __fastcall Effect_Free(uint32_t* e);
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

struct Collection;
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
void __fastcall ListPair_Reset(uint8_t* o);


void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);
void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
void __fastcall Obj_SetPair1ed(uint8_t* o, int, uint32_t v);
uint8_t* __fastcall EmitterLibrary_Get(uint8_t* lib, int, uint32_t key, uint32_t color, const char* name, uint8_t* obj, int32_t spawn);
void* __fastcall Engine_Get1C(void* e);
uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);

uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                      uint32_t flags);

PtrListNode* __fastcall Camera_AddListener(uint8_t* c, int, void* item);
void* __fastcall FindRecordByCrc(const char* name);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
void __fastcall WorldModel_Init(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y, int32_t freeStanding);
uint8_t* __fastcall NamedObj474110_Construct(uint8_t* o);
void __fastcall NamedObj474110_Destructor(uint8_t* o);
void __cdecl Emitters_Load();
void Particles_LoadTable();
void __fastcall Effects_RegisterEmitters();
struct NodePool;
void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
void __fastcall Ambience_Tick(uint8_t* a, int, uint32_t flag);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void* __fastcall PtrList_CurrentValue(PtrList* l);
void* __fastcall PtrList_RemoveCurrent(PtrList* l);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall SceneObj_Teardown(uint8_t* o);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall WorldModel_Load(uint8_t* o, int, int32_t s);
void __fastcall NamedObj_Save(uint8_t* o, int, Stream s);
void __fastcall Obj_PlaceAtCell(uint8_t* o, int, float cx, float cz, int32_t lift);
void __fastcall EffectObj_Teardown(uint8_t* o);

// Collection callbacks deleting record `id`: its teardown, then the slot.
#define RT3_DELETE_RECORD(addr, name, teardown, type) \
    void __fastcall name(Collection* c, int, uint32_t id) { \
        teardown((type)Collection_Get(c, 0, id)); \
        Collection_Release(c, 0, id); \
    } \
    RT3_REPLACE(addr, name);

void Effect_PlaceScenery();
int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall SceneObj_Teardown(uint8_t* o);
void Effect_PlaceScenery();
uint8_t* __fastcall NamedObj474110_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall SceneryEffect_Init(uint8_t* o, int, uint32_t id, float x, float z, uint32_t kind);
void __fastcall EffectObj_PlaceAt(uint8_t* o, int, float x, float z, int32_t lift);
int32_t __fastcall EffectObj_Load(uint8_t* o, int, int32_t s);
void __fastcall Thunk4742d0(uint8_t* p0, int p1, Stream p2);
int32_t Tooltip_Show123();
void __fastcall Nop474300(void*);
int32_t __fastcall EffectObj_Tooltip(uint8_t* o, int, char* out, int32_t n, int32_t a, int32_t b, int32_t c);
void __fastcall NamedObj474110_Destructor(uint8_t* o);
RT3_NOINLINE void __fastcall EffectObj_Teardown(uint8_t* o);
int32_t __fastcall Obj_Visible474430(uint8_t* o);
RT3_NOINLINE Collection* __fastcall Collection5ce4a8_Construct(Collection* c);
void __fastcall Collection5ce4a8_Destruct(Collection* c);
uint32_t __fastcall SceneryEffects_Add(Collection* t, int, float x, float z, uint32_t kind);
RT3_NOINLINE int32_t __fastcall NamedObjs474110_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Collection_SaveWithRecords61a9(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Effects_RegisterEmitters();
void Particles_LoadTable();
void __cdecl Emitters_Load();
void* __fastcall FindRecordByCrc(const char* name);
uint8_t* __fastcall Emitters_NextFile(const char* prefix, int32_t restart);
void __fastcall Group_ResetLists(uint8_t* g);
int32_t __fastcall Obj_NameIs5ce4f4(uint8_t* o);
void Emitter_Apply();
RT3_NOINLINE void Effects_ReapplyAll();
void Effects_Reset();
void FreeBuffers6cea54();
RT3_NOINLINE uint8_t* __fastcall Effect_Spawn(const char* name, uint8_t* parent, uint8_t* def, uint32_t flags, float scale);
void __fastcall EmitterGroup_Tick(uint8_t* g);
RT3_NOINLINE void __fastcall Obj_SetAllBc(void* o, int32_t v);
RT3_NOINLINE void __fastcall Store3Globals(int32_t* v);
RT3_NOINLINE void __fastcall Obj_ApplyStored3(void* o, int32_t idx);
RT3_NOINLINE int32_t __fastcall Obj_SumEntrySizes(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_SumEntries6c(uint8_t* o);
RT3_NOINLINE void __fastcall Effect_Free(uint32_t* e);
void __fastcall Obj_SetElementFlag1d8(void* o, int32_t mode, int32_t idx);
int32_t __fastcall Obj_EntryClear1d8(uint8_t* o, int32_t i);
uint8_t* __stdcall EffectGroup_BrokenDownCrash(uint8_t* parent, float scale, uint32_t flags, int32_t);
uint8_t* __stdcall EffectGroup_Volcano(uint8_t* parent, float scale, uint32_t flags);
uint8_t* __stdcall EffectGroup_BrokenDown(uint8_t* parent, float scale, uint32_t flags);
void EffectGroup_ServiceTower();
void EffectGroup_Rocket();
void EffectGroup_Fireworks();
void EffectGroup_CargoMoney();
uint8_t* __stdcall EffectGroup_SelectObject(uint8_t* parent, float scale, uint32_t flags);
void EffectGroup_SpendMoney();
uint8_t* __stdcall EffectGroup_Construct(uint8_t* parent, float scale, uint32_t flags);
void EffectGroup_Mist();
uint8_t* __stdcall EffectGroup_Vent(uint8_t* parent, float scale, uint32_t flags);
RT3_NOINLINE uint8_t* __fastcall Effect_Create(uint8_t* parent, int32_t kind, float scale, uint32_t flags, int32_t listed);
RT3_NOINLINE void __fastcall Emitter_Tick(uint8_t* e, int, int32_t force);
RT3_NOINLINE void __fastcall Texture_SetD9399c(const char* name);
void __fastcall Model_Release(uint8_t* m);
RT3_NOINLINE int32_t __fastcall EmitterLibrary_Register(uint8_t* lib, int, uint32_t key, int32_t perPoint, const float* rect, uint32_t flags);

static inline uint8_t* Packs() { return g_pack; }

static inline float Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

// 0x4749d0 body. Applies emitter settings src to effect dst at scale: the six
// angles +0x9c..+0xb0 times 0x5c9b68 (+0xb0 also scaled) to +0x70..+0x84; with
// flag 0x1000 the spin +0xec to +0x88 / +0x90; colours +0x84 / +0xc4 (24 bits);
// the budget, view sizes and lifetimes (+0x88..+0x98, scaled), the texture record
// named +0x44 from table 0x6cea5c (0xa0 bytes) to +0x10c; flag 0x40000 the pair
// +0xc0, flags 0x20080000 the rate +0xc8; the tail +0xcc..+0xe4 to +0xf0..+0x108.
static void __cdecl Emitter_ApplyImpl(uint8_t* src, uint8_t* dst, uint32_t flags, float scale) {
    const double k = Fl(0x5c9b68);
    float a0 = (float)((double)F<float>(src, 0xa0) * k);
    float a4 = (float)((double)F<float>(src, 0xa4) * k);
    float a9c = (float)((double)F<float>(src, 0x9c) * k);
    float aac = (float)((double)F<float>(src, 0xac) * k);
    float ab0 = (float)((double)scale * F<float>(src, 0xb0) * k);
    float aa8 = (float)((double)F<float>(src, 0xa8) * k);
    F<float>(dst, 0x70) = a9c;
    F<float>(dst, 0x74) = a4;
    F<float>(dst, 0x78) = a0;
    F<float>(dst, 0x7c) = aa8;
    F<float>(dst, 0x80) = ab0;
    F<float>(dst, 0x84) = aac;
    F<uint32_t>(dst, 0x88) = 0;
    F<uint32_t>(dst, 0x8c) = 0;
    F<uint32_t>(dst, 0x90) = 0;
    if (flags & 0x1000) {
        F<uint32_t>(dst, 0x88) = F<uint32_t>(src, 0xec);
        F<uint32_t>(dst, 0x8c) = 0;
        F<uint32_t>(dst, 0x90) = F<uint32_t>(src, 0xec);
    }
    F<uint32_t>(dst, 0x48) = F<uint32_t>(src, 0x84) & 0xffffff;
    F<uint32_t>(dst, 0x4c) = F<uint32_t>(src, 0xc4) & 0xffffff;
    Timer_SetBudget(dst, 0, (uint32_t)Ftol((double)scale * F<float>(src, 0x90) * Fl(0x5c86a0)));
    F<uint32_t>(dst, 0x98) = F<uint32_t>(src, 0xb4);
    F<uint32_t>(dst, 0xd4) = F<uint32_t>(src, 0xe8);
    float u = (float)((double)scale * F<float>(src, 0x98));
    float v = (float)((double)scale * F<float>(src, 0x94));
    float u2 = (float)((double)u * Fl(0x5ce500));
    float v2 = (float)((double)v * Fl(0x5ce500));
    int32_t life2 = Ftol((double)scale * F<float>(src, 0x8c) * Fl(0x5c86a0));
    int32_t life1 = Ftol((double)scale * F<float>(src, 0x88) * Fl(0x5c86a0));
    View_SetParams(dst, 0, life1, (uint32_t)life2, *(uint32_t*)&v, *(uint32_t*)&v2, *(uint32_t*)&u, *(uint32_t*)&u2);
    uint32_t key = Crc32String((const char*)src + 0x44, 1);
    if (uint8_t* t = hook::As<uint8_t*>(g_6cea5c)) {
        uint8_t* e = t + 0xc;
        for (int32_t i = 0; i < F<int32_t>(t, 8); ++i, e += 0xa0)
            if (F<uint32_t>(e, 0) == key) {
                memcpy(dst + 0x10c, e, 0xa0);
                break;
            }
    }
    F<uint32_t>(dst, 0x1ac) = 0;
    if (flags & 0x40000) Obj_SetPair1ed(dst, 0, (uint32_t)Ftol(F<float>(src, 0xc0)));
    if (flags & 0x20080000) F<float>(dst, 0x94) = (float)((double)F<float>(src, 0xc8) * Fl(0x5ce4fc));
    F<uint32_t>(dst, 0xf0) = F<uint32_t>(src, 0xcc);
    F<uint32_t>(dst, 0xf4) = F<uint32_t>(src, 0xd0);
    F<uint32_t>(dst, 0xf8) = F<uint32_t>(src, 0xd4);
    F<uint32_t>(dst, 0x108) = F<uint32_t>(src, 0xe4);
    F<uint32_t>(dst, 0xfc) = F<uint32_t>(src, 0xd8);
    F<uint32_t>(dst, 0x100) = F<uint32_t>(src, 0xdc);
    F<uint32_t>(dst, 0x104) = F<uint32_t>(src, 0xe0);
}

namespace {
void CallEmitterApply(uint8_t* def, uint8_t* e, uint32_t flags, float scale) {
    void* fn = (void*)&Emitter_Apply;
    __asm {
        push scale
        push flags
        push e
        mov eax, def
        call fn
    }
}
}

namespace {
// A node from the effect pool 0x6cea58 (a new one while the pool is off).
uint8_t* EffectNode() {
    uint32_t* pool = g_6cea58;
    if (!pool[3]) return (uint8_t*)crt::operator_new(pool[4]);
    uint8_t** list = (uint8_t**)pool[0];
    uint8_t* node = list[0];
    if (!node) return nullptr;
    uint8_t* next = *(uint8_t**)node;
    list[0] = next;
    if (node == list[1]) list[1] = next;
    --((int32_t*)list)[2];
    return node;
}
// The effect of emitters.dat 0x6cea60 named `name` (by CRC), or null.
uint8_t* EffectDef(const char* name) {
    uint8_t* r = g_effects + 0xc;
    uint32_t key = Crc32String(name, 1);
    for (int32_t i = 0; i < F<int32_t>(g_effects, 8); ++i, r += 0x140)
        if (F<uint32_t>(r, 0) == key) return r;
    return nullptr;
}
// Spawns the effects named in `defs` into node slots +0xc.. on `parent` (for
// a group, the first one's parent when null).
uint8_t* EffectGroup(uint8_t* parent, float scale, uint32_t flags, const char* const* defs, const char* const* names, int32_t n) {
    uint8_t* node = EffectNode();
    for (int32_t k = 0; k < n; ++k) {
        uint8_t* def = EffectDef(defs[k]);
        uint8_t* e = Effect_Spawn(names ? names[k] : nullptr, parent, def, flags, scale);
        F<uint8_t*>(node, 0xc + k * 4) = e;
        if (!k && !parent && n > 1) parent = F<uint8_t*>(e, 0xc);
    }
    F<int32_t>(node, 8) = n;
    return node;
}
}

// 0x475650 (flags in EAX). A service tower's three water jets.
static uint8_t* __stdcall EffectGroup_ServiceTowerImpl(uint32_t flags, uint8_t* parent, float scale) {
    static const char* const defs[] = { (const char*)0x5ce69c, (const char*)0x5ce668, (const char*)0x5ce634 };
    static const char* const names[] = { (const char*)0x5ce67c, (const char*)0x5ce648, (const char*)0x5ce614 };
    return EffectGroup(parent, scale, flags | 0x1000000, defs, names, 3);
}

// 0x4757b0 (flags in EAX). A rocket's flame, smoke and sparkle.
static uint8_t* __stdcall EffectGroup_RocketImpl(uint32_t flags, uint8_t* parent, float scale) {
    static const char* const defs[] = { (const char*)0x5ce724, (const char*)0x5ce6f8, (const char*)0x5ce6cc };
    static const char* const names[] = { (const char*)0x5ce708, (const char*)0x5ce6dc, (const char*)0x5ce6b0 };
    return EffectGroup(parent, scale, flags | 0x1000000, defs, names, 3);
}

// 0x475910 (flags in EAX). One of the three fireworks, never the one picked
// last time (0x5f2c2c), from the LCG 0x624324; +0xbc = 60.
static uint8_t* __stdcall EffectGroup_FireworksImpl(uint32_t flags, uint8_t* parent, float scale) {
    uint8_t* node = EffectNode();
    uint32_t s = g_lcg;
    int32_t pick;
    do {
        s = s * 0x19660d + 0x3c6ef35f;
        pick = (int32_t)(s & 0x7fff) % 3;
    } while (pick == g_5f2c2c);
    g_lcg = s;
    g_5f2c2c = pick;
    static const char* const defs[] = { (const char*)0x5ce768, (const char*)0x5ce75c, (const char*)0x5ce750 };
    uint8_t* e = Effect_Spawn((const char*)0x5ce734, parent, (uint8_t*)FindRecordByCrc(defs[pick]), flags | 0x1000000, scale);
    F<uint8_t*>(node, 0xc) = e;
    F<uint32_t>(e, 0xbc) = 0x3c;
    F<int32_t>(node, 8) = 1;
    return node;
}

// 0x4759f0 (flags in EAX). The dollar sign over a cargo delivery.
static uint8_t* __stdcall EffectGroup_CargoMoneyImpl(uint32_t flags, uint8_t* parent, float scale) {
    static const char* const defs[] = { (const char*)0x5ce504 };
    return EffectGroup(parent, scale, flags | 0xc000, defs, nullptr, 1);
}

// 0x475b30 (flags in EAX). The dollar sign over money spent.
static uint8_t* __stdcall EffectGroup_SpendMoneyImpl(uint32_t flags, uint8_t* parent, float scale) {
    static const char* const defs[] = { (const char*)0x5ce784 };
    return EffectGroup(parent, scale, flags | 0xc000, defs, nullptr, 1);
}

// 0x475dc0 (flags in EDI, parent in EBX). Mist, the small kind unless the
// scale is 1.
static uint8_t* __stdcall EffectGroup_MistImpl(uint32_t flags, uint8_t* parent, float scale) {
    uint8_t* node = EffectNode();
    const char* def = *(uint32_t*)&scale == 0x3f800000 ? (const char*)0x5ce7fc : (const char*)0x5ce7f0;
    F<uint8_t*>(node, 0xc) = Effect_Spawn(nullptr, parent, (uint8_t*)FindRecordByCrc(def), flags, scale);
    F<int32_t>(node, 8) = 1;
    return node;
}

// 0x474030 (kind in EAX). A free-standing scenery effect at (x, y, z):
// volcanoes of sizes 0.5 / 2 / - / 0.5 / 0.75 / 1 / 1.5 / 2 for kinds 0..7,
// mist 0.5 / 1 for 8 / 9, a vent for 10; else none.
RT3_NOINLINE static uint8_t* __stdcall Effect_PlaceSceneryImpl(uint32_t kind, float x, float y, float z) {
    static const int32_t kinds[] = { 4, 4, 0, 4, 4, 4, 4, 4, 13, 13, 14 };
    static const uint32_t scales[] = { 0x3f000000, 0x40000000, 0, 0x3f000000, 0x3f400000, 0x3f800000, 0x3fc00000, 0x40000000, 0x3f000000, 0x3f800000,
                                       0x3f800000 };
    if (kind > 10 || kind == 2) return nullptr;
    uint8_t* n = Effect_Create(nullptr, kinds[kind], *(const float*)&scales[kind], 0, 0);
    if (n) Obj_MoveTo(F<uint8_t*>(n, 4), 0, x, y, z);
    return n;
}

static inline double Fl_205(uint32_t a) { return *(const float*)rt3::Rdata(a); }


// Unlinks `node` from the singly linked list {head, tail, count}; a node that
// isn't found (or is the head) makes it drop the head instead, as the original.
static void SList_Unlink(uint32_t* head, uint32_t* tail, int32_t* count, uint32_t* node) {
    for (uint32_t* p = (uint32_t*)(uintptr_t)*head; p; p = (uint32_t*)(uintptr_t)p[0]) {
        if ((uint32_t*)(uintptr_t)p[0] == node) {
            p[0] = node[0];
            node[0] = 0;
            --*count;
            if ((uint32_t)(uintptr_t)node == *tail) *tail = (uint32_t)(uintptr_t)p;
            return;
        }
    }
    if (uint32_t* first = (uint32_t*)(uintptr_t)*head) {
        *head = first[0];
        if ((uint32_t)(uintptr_t)first == *tail) *tail = *head;
        --*count;
    }
}

// Gives `node` back to the block pool {free list, block start, block end, active}
// when it came from the pool's block, else deletes it.
static void Pool_Return(uint8_t* pool, uint32_t* node) {
    if (F<int32_t>(pool, 0xc) && (uintptr_t)node >= F<uint32_t>(pool, 4) && (uintptr_t)node < F<uint32_t>(pool, 8)) {
        uint32_t* free = F<uint32_t*>(pool, 0);
        node[0] = free[0];
        ++free[2];
        free[0] = (uint32_t)(uintptr_t)node;
        if (!free[1]) free[1] = (uint32_t)(uintptr_t)node;
    } else {
        crt::operator_delete(node);
    }
}

static inline double Fl_207(uint32_t a) { return *(const float*)rt3::Rdata(a); }

__declspec(naked) void Effect_PlaceScenery() {
    __asm {
        push [esp + 12]
        push [esp + 12]
        push [esp + 12]
        push eax
        call Effect_PlaceSceneryImpl
        ret 12
    }
}
RT3_REPLACE(0x474030, Effect_PlaceScenery);

RT3_NOINLINE uint8_t* __fastcall NamedObj474110_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<int32_t>(o, 0x242) = 0;
    F<int32_t>(o, 0x23a) = 0;
    F<uint32_t>(o, 0) = 0x5ce428;
    return o;
}
RT3_REPLACE(0x474110, NamedObj474110_Construct);

// 0x474130. Sets up scenery effect object o (id +0x23e, kind +0x242) at tile
// (x, z) on the ground: its effect (+0x23a, Effect_PlaceScenery) and a model-
// less world object named Marker04.
RT3_NOINLINE void __fastcall SceneryEffect_Init(uint8_t* o, int, uint32_t id, float x, float z, uint32_t kind) {
    float pz = (float)((double)z * Fl(0x5c8700));
    F<uint32_t>(o, 0x23e) = id;
    F<uint32_t>(o, 0x246) = 0;
    F<uint32_t>(o, 0x24a) = 0;
    F<uint16_t>(o, 0x24e) = 0;
    F<uint32_t>(o, 0x242) = kind;
    float h = (float)Terrain_HeightAtTile(g_map, 0, x, z);
    float px = (float)((double)x * Fl(0x5c8700));
    F<uint8_t*>(o, 0x23a) = Effect_PlaceSceneryImpl(kind, px, h, pz);
    char name[9];
    memcpy(name, (const char*)0x5ce49c, sizeof name);
    WorldModel_Init(o, 0, name, nullptr, nullptr, *(uint32_t*)&x, *(uint32_t*)&z, 0);
}
RT3_REPLACE(0x474130, SceneryEffect_Init);

// 0x474200 (vtable). Moves effect object o to cell (x, z): its scene node
// (+0x23a +4) to (x, ground, z) x 0x5c8700, then the cell placement.
void __fastcall EffectObj_PlaceAt(uint8_t* o, int, float x, float z, int32_t lift) {
    const float sz = (float)((double)z * Fl_207(0x5c8700));
    const float h = (float)Terrain_HeightAtTile(g_map, 0, x, z);
    Obj_MoveTo(F<uint8_t*>(F<uint8_t*>(o, 0x23a), 4), 0, (float)((double)x * Fl_207(0x5c8700)), h, sz);
    Obj_PlaceAtCell(o, 0, x, z, lift);
}
RT3_REPLACE(0x474200, EffectObj_PlaceAt);

// 0x474260 (vtable). Loads effect object o, then places its scenery effect
// (+0x242) at its cell (EAX kind, three stack arguments).
int32_t __fastcall EffectObj_Load(uint8_t* o, int, int32_t s) {
    const int32_t r = WorldModel_Load(o, 0, s);
    const float a = (float)(Obj_CellY(o) * Fl_207(0x5c8700));
    const float b = (float)Obj_CellY(o);
    const float c = (float)Obj_CellX(o);
    const float h = (float)Terrain_HeightAtTile(g_map, 0, c, b);
    const float e = (float)(Obj_CellX(o) * Fl_207(0x5c8700));
    const int32_t kind = F<int32_t>(o, 0x242);
    uint32_t placed;
    __asm {
        push a
        push h
        push e
        mov eax, kind
        call Effect_PlaceScenery
        mov placed, eax
    }
    F<uint32_t>(o, 0x23a) = placed;
    return r;
}
RT3_REPLACE(0x474260, EffectObj_Load);

// 0x4742d0 (vtable stub).
// Forwards to NamedObj_Save (0x4559d0).
void __fastcall Thunk4742d0(uint8_t* p0, int p1, Stream p2) { NamedObj_Save(p0, p1, p2); }
RT3_REPLACE(0x4742d0, Thunk4742d0);

int32_t Tooltip_Show123() { return Ui_ShowTooltip(g_windowManager, 0, GetString(0x123), 3, 0, -1); }
RT3_REPLACE(0x4742e0, Tooltip_Show123);

// 0x474300 (vtable stub).
void __fastcall Nop474300(void*) { __asm nop }
RT3_REPLACE(0x474300, Nop474300);

// 0x474310 (vtable). Tooltip text of effect object o into out (n bytes): in
// the editor its kind (0x124..0x126 by +0x242), else the scene object's.
int32_t __fastcall EffectObj_Tooltip(uint8_t* o, int, char* out, int32_t n, int32_t a, int32_t b, int32_t c) {
    if (!Global6d07b4Set2()) return SceneObj_DebugTooltip(o, 0, out, n, a, b, c);
    const int32_t kind = F<int32_t>(o, 0x242);
    char text[0x130];
    FormatText(text, GetString(kind == 0 ? 0x124 : kind == 1 ? 0x125 : 0x126), 0, 0, 0, 0, 0, 0, 0, 0);
    StrCopyN(out, text, (size_t)(n - 1));
    return 1;
}
RT3_REPLACE(0x474310, EffectObj_Tooltip);

// 0x4743d0. Destructor of the 0x474110 object: frees its effect (+0x23a) first.
RT3_NOINLINE void __fastcall NamedObj474110_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5ce428;
    if (F<uint32_t*>(o, 0x23a)) Effect_Free(F<uint32_t*>(o, 0x23a));
    F<uint32_t*>(o, 0x23a) = nullptr;
    NamedObj_Destructor(o);
}
RT3_REPLACE(0x4743d0, NamedObj474110_Destructor);

// 0x474400. Effect object o: frees its effect (+0x23a), then tears down the
// scene object.
RT3_NOINLINE void __fastcall EffectObj_Teardown(uint8_t* o) {
    if (F<uint32_t*>(o, 0x23a)) Effect_Free(F<uint32_t*>(o, 0x23a));
    F<void*>(o, 0x23a) = nullptr;
    SceneObj_Teardown(o);
}
RT3_REPLACE(0x474400, EffectObj_Teardown);

int32_t __fastcall Obj_Visible474430(uint8_t* o) { return Global6d07b4Set2() ? o[0x42] : 1; }
RT3_REPLACE(0x474430, Obj_Visible474430);

// 0x474450 / 0x474480. Collection subclass 0x5ce4a8 (heap records, 10 + 20).
RT3_NOINLINE Collection* __fastcall Collection5ce4a8_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce4a8;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x474450, Collection5ce4a8_Construct);

void __fastcall Collection5ce4a8_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce4a8;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x474480, Collection5ce4a8_Destruct);

// 0x4744a0. Adds a scenery effect object (0x250-byte records) of kind `kind`
// at tile (x, z) to table t and returns its id.
uint32_t __fastcall SceneryEffects_Add(Collection* t, int, float x, float z, uint32_t kind) {
    uint8_t tmp[0x250];
    NamedObj474110_Construct(tmp);
    uint32_t id = Collection_Allocate(t, 0, tmp, 0x250, 0, 0);
    SceneryEffect_Init((uint8_t*)Collection_Get(t, 0, id), 0, id, x, z, kind);
    NamedObj474110_Destructor(tmp);
    return id;
}
RT3_REPLACE(0x4744a0, SceneryEffects_Add);

RT3_DELETE_RECORD(0x474510, EffectObjs_Delete, EffectObj_Teardown, uint8_t*)

// 0x474540. Loads the 0x474110 objects (frame 0x61a9 / 0x61aa / 0x61ab).
RT3_NOINLINE int32_t __fastcall NamedObjs474110_Load(Collection* t, int, Stream s, int32_t recordSize) {
    return LoadSceneObjects(t, s, recordSize, 0x61a9, 0x61aa, 0x61ab, [](uint8_t* o) { NamedObj474110_Construct(o); },
                            [](uint8_t* o) { NamedObj474110_Destructor(o); });
}
RT3_REPLACE(0x474540, NamedObjs474110_Load);

// 0x474610. As 0x40b6a0 with tags 0x61a9/0x61aa/0x61ab.
RT3_NOINLINE void __fastcall Collection_SaveWithRecords61a9(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x61a9);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x61aa);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, 0x61ab);
}
RT3_REPLACE(0x474610, Collection_SaveWithRecords61a9);

// 0x474690. Registers the emitter templates of the effect table 0x6cea60
// (count +8, 0x140-byte entries from +0x10: name, rate +0x84 / period +0x8c,
// enabled +0xb4, flags +0xb8) in the model library 0x6d4030, keyed by the
// name's CRC, with rate / period + 2 particles per point.
RT3_NOINLINE void __fastcall Effects_RegisterEmitters() {
    for (int32_t i = 0; i < F<int32_t>(g_effects, 8); ++i) {
        uint8_t* e = g_effects + 0x10 + i * 0x140;
        if (F<int32_t>(e, 0xb4) != 1) continue;
        int32_t n = Ftol((double)F<float>(e, 0x84) / F<float>(e, 0x8c) + Fl_205(0x5c878c));
        uint32_t flags = F<uint32_t>(e, 0xb8) | 0x10000;
        EmitterLibrary_Register(g_models, 0, Crc32String((const char*)e, 1), n, nullptr, flags);
    }
}
RT3_REPLACE(0x474690, Effects_RegisterEmitters);

// 0x474700. Reads the particle definitions (pack file "particles.dat") into
// 0x6cea5c (the old copy freed) and loads the particle texture.
RT3_NOINLINE void Particles_LoadTable() {
    if (void* old = g_6cea5c) {
        crt::free(old);
        g_6cea5c = nullptr;
    }
    if (NameTable_ContainsName(g_pack, 0, (const char*)0x5ce4c8, 0, nullptr)) {
        int32_t id = NameTable_Hash(g_pack, 0, (const char*)0x5ce4c8);
        int32_t n = NameTable_EntrySize(g_pack, 0, id);
        if (n > 0) {
            NameTable_Seek(g_pack, 0, id, 0);
            void* buf = crt::malloc((size_t)n);
            g_6cea5c = buf;
            NameTable_Read(g_pack, 0, buf, n);
        }
    }
    Texture_SetD9399c((const char*)0x5ce4b4);
}
RT3_REPLACE(0x474700, Particles_LoadTable);

// 0x474790. (Re)loads emitters.dat (0x6cea60) from the packs.
RT3_NOINLINE void __cdecl Emitters_Load() {
    if (hook::As<void*>(g_effects)) {
        crt::free(hook::As<void*>(g_effects));
        hook::As<void*>(g_effects) = nullptr;
    }
    if (!NameTable_ContainsName(Packs(), 0, (const char*)0x5ce4d8, 0, nullptr)) return;
    int32_t id = NameTable_Hash(Packs(), 0, (const char*)0x5ce4d8);
    int32_t size = NameTable_EntrySize(Packs(), 0, id);
    if (size <= 0) return;
    NameTable_Seek(Packs(), 0, id, 0);
    void* p = crt::malloc(size);
    hook::As<void*>(g_effects) = p;
    NameTable_Read(Packs(), 0, p, size);
}
RT3_REPLACE(0x474790, Emitters_Load);

// 0x474820. Record (0x140 bytes, from +0xc of 0x6cea60) whose key is the
// case-insensitive CRC of `name`; NULL if none.
RT3_NOINLINE void* __fastcall FindRecordByCrc(const char* name) {
    uint8_t* t = g_effects;
    uint32_t crc = Crc32String(name, 1);
    uint8_t* r = t + 0xc;
    for (int32_t i = 0; i < F<int32_t>(t, 8); ++i, r += 0x140)
        if (*(uint32_t*)r == crc) return r;
    return nullptr;
}
RT3_REPLACE(0x474820, FindRecordByCrc);

// 0x474860. Iterates the emitter model files "<prefix><emitter name><n>.3dp" (n
// from 1, up to 254; emitters 0x140 bytes from emitters.dat +0xc, count +8):
// returns 0x6cea90 = {emitter*, file name} for the next one present (only pack
// entries when 0x6cea64), or 0 at the end. restart == 1 starts over.
uint8_t* __fastcall Emitters_NextFile(const char* prefix, int32_t restart) {
    if (restart == 1) {
        g_5f2c30 = restart;
        g_6ceb94 = 0;
    }
    int32_t n = g_5f2c30;
    int32_t idx = g_6ceb94;
    uint8_t* data = g_effects;
    uint8_t* e = data + 0xc + idx * 0x140;
    char buf[0x100];
    for (; idx < F<int32_t>(data, 8); ++idx, e += 0x140) {
        if (n >= 0xff) continue;
        crt::sprintf(buf, (const char*)0x5ce4e8, prefix, (const char*)e + 4, n);
        int32_t r = NameTable_ContainsName(Packs(), 0, buf, 0, nullptr);
        if ((!g_6cea64 || r == 2) && r) {
            g_6ceb94 = idx;
            g_5f2c30 = n + 1;
            g_6cea90 = e;
            char* d = (char*)0x6cea94;
            for (const char* s = buf; (*d++ = *s++) != 0;) {}
            return (uint8_t*)0x6cea90;
        }
        data = g_effects;
        n = 1;
        g_5f2c30 = 1;
    }
    g_5f2c30 = 1;
    g_6ceb94 = 0;
    return nullptr;
}
RT3_REPLACE(0x474860, Emitters_NextFile);

void __fastcall Group_ResetLists(uint8_t* g) {
    for (int32_t i = 0; i < F<int32_t>(g, 8); ++i) ListPair_Reset(F<uint8_t*>(g, 0xc + i * 4));
}
RT3_REPLACE(0x474970, Group_ResetLists);

// 0x4749a0. Name +0x44 equals the string 0x5ce4f4 (case-insensitive).
int32_t __fastcall Obj_NameIs5ce4f4(uint8_t* o) { return crt::stricmp((const char*)o + 0x44, (const char*)0x5ce4f4) == 0 ? 1 : 0; }
RT3_REPLACE(0x4749a0, Obj_NameIs5ce4f4);

// 0x4749d0. Settings in EAX; effect, flags and scale on the stack.
__declspec(naked) void Emitter_Apply() {
    __asm {
        push [esp + 12]
        push [esp + 12]
        push [esp + 12]
        push eax
        call Emitter_ApplyImpl
        add esp, 16
        ret 12
    }
}
RT3_REPLACE(0x4749d0, Emitter_Apply);

// 0x474c60. Re-applies every active (+0xb8 == 1) emitter definition of table
// 0x6cea60 (0x140-byte records) to its library effect (by the CRC of its name
// +4), with its flags +0xbc | 0x10004 and scale 1.
RT3_NOINLINE void Effects_ReapplyAll() {
    uint8_t* t = g_effects;
    uint8_t* e = t + 0xc;
    for (int32_t i = 0; i < F<int32_t>(t, 8); ++i, e += 0x140) {
        if (F<int32_t>(e, 0xb8) != 1) continue;
        uint8_t* fx = EmitterLibrary_Get(g_models, 0, Crc32String((const char*)e + 4, 1), 0, nullptr,
                                         nullptr, 0);
        uint32_t flags = F<uint32_t>(e, 0xbc) | 0x10004;
        F<uint32_t>(fx, 0xac) = flags;
        void* fn = (void*)&Emitter_Apply;
        float one = 1.0f;
        __asm {
            push one
            push flags
            push fx
            mov eax, e
            call fn
        }
        t = g_effects;
    }
}
RT3_REPLACE(0x474c60, Effects_ReapplyAll);

// 0x474ce0. Resets the effects system: reloads emitters.dat and the particle
// table; the first time also registers the emitters and creates the
// 10000-node effect pool 0x6cea58 (0x38-byte nodes); clears the effect
// counters, re-applies the active emitters and ticks the ambience.
void Effects_Reset() {
    Emitters_Load();
    Particles_LoadTable();
    if (!g_6ceb98) {
        Effects_RegisterEmitters();
        uint32_t* p = (uint32_t*)crt::operator_new(0x1c);
        if (p) memset(p, 0, 0x1c);
        g_6cea58 = p;
        void* storage = crt::malloc(0x88b80);
        g_6cea54 = storage;
        Pool_Init((NodePool*)g_6cea58, 0, storage, 0x38, 0x2710, 1);
        g_6ceb98 = 1;
    }
    hook::As<uint32_t>(g_6cea74) = 0;
    g_6cea78 = 0;
    hook::As<uint32_t>(g_6cea7c) = 0;
    Effects_ReapplyAll();
    hook::As<uint32_t>(g_6cea68) = 0;
    hook::As<uint32_t>(g_6cea6c) = 0;
    hook::As<uint32_t>(g_6cea70) = 0;
    g_6cea80 = 0;
    g_6cea84 = 0;
    g_6cea88 = 0;
    Ambience_Tick(g_models, 0, 1);
}
RT3_REPLACE(0x474ce0, Effects_Reset);

// 0x474db0. Frees the buffers 0x6cea54/0x6cea5c/0x6cea60 and the map 0x6cea58.
void FreeBuffers6cea54() {
    static const uint32_t kBuffers[] = { 0x6cea54, 0x6cea5c, 0x6cea60 };
    for (uint32_t va : kBuffers) {
        void*& p = hook::Global<void*>(va);
        if (p) {
            crt::free(p);
            p = nullptr;
        }
    }
    void*& m = hook::As<void*>(g_6cea58);
    if (m) {
        void* p = m;
        ((void(__thiscall*)(void*))&Pool_Free)(p);
        crt::operator_delete(p);
        m = nullptr;
    }
}
RT3_REPLACE(0x474db0, FreeBuffers6cea54);

 // namespace

// 0x474e20. Spawns effect `def` (name +0x4, colour +0x84) as `name` on
// scene object `parent` (a new one owned by the engine scene when null):
// from the emitter library with flag 0x400 (and +0xb8 set), else a new
// emitter with rate +0x88 / +0x90 (+0x5c878c) set up by Emitter_Apply.
// Effects named CargoMoney_DollarSign drop def flags 0xe0 (after they are
// read). The emitter is attached to the parent, remembers its def (+0x60)
// and with flag 0x2000000 is marked +0xbc.
RT3_NOINLINE uint8_t* __fastcall Effect_Spawn(const char* name, uint8_t* parent, uint8_t* def, uint32_t flags, float scale) {
    uint32_t base = F<uint32_t>(def, 0xbc) | 4;
    if (!parent) {
        uint8_t* p = (uint8_t*)crt::operator_new(0x206);
        parent = p ? SceneObj_Construct(p, 0, (uint32_t)(uintptr_t)Engine_Get1C(hook::As<void*>(g_engine)), nullptr, nullptr) : nullptr;
    }
    if (((const char*)crt::strstr((const char*)def + 4, (const char*)0x5ce504))) F<uint32_t>(def, 0xbc) &= 0xffffff1f;
    if (!F<int32_t>(def, 0xb8)) flags &= 0xfffffbff;
    uint8_t* e;
    if (flags & 0x400) {
        uint32_t key = Crc32String((const char*)def + 4, 1);
        e = EmitterLibrary_Get(g_models, 0, key, F<uint32_t>(def, 0x84), name, parent, 1);
    } else {
        int32_t rate = Ftol((double)F<float>(def, 0x88) / F<float>(def, 0x90) + Fl(0x5c878c));
        uint8_t* p = (uint8_t*)crt::operator_new(0x1fd);
        e = p ? Emitter_Construct(p, 0, name, F<uint32_t>(def, 0x84), rate, nullptr, parent, base | flags) : nullptr;
        CallEmitterApply(def, e, base | flags, scale);
    }
    if (!e) return e;
    Camera_AddListener(parent, 0, e);
    F<uint8_t*>(e, 0x60) = def;
    if ((base | flags) & 0x2000000) F<uint32_t>(e, 0xbc) = 1;
    return e;
}
RT3_REPLACE(0x474e20, Effect_Spawn);

// 0x474f70. Ticks the emitters of group g ({next, ?, count +8, emitters +0xc..}), or
// of every group in the chain 0x6cea74 when g is null.
void __fastcall EmitterGroup_Tick(uint8_t* g) {
    if (g) {
        for (int32_t i = 0; i < F<int32_t>(g, 8); ++i) Emitter_Tick(F<uint8_t*>(g, 0xc + i * 4), 0, 0);
        return;
    }
    for (uint8_t* c = g_6cea74; c; c = F<uint8_t*>(c, 0))
        for (int32_t i = 0; i < F<int32_t>(c, 8); ++i) Emitter_Tick(F<uint8_t*>(c, 0xc + i * 4), 0, 0);
}
RT3_REPLACE(0x474f70, EmitterGroup_Tick);

RT3_NOINLINE void __fastcall Obj_SetAllBc(void* o, int32_t v) {
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i) F<int32_t>(F<uint8_t*>(o, 0xc + i * 4), 0xbc) = v;
}
RT3_REPLACE(0x474fe0, Obj_SetAllBc);

// 0x475010. Copies 3 dwords to globals 0x6cea68..0x6cea70.
RT3_NOINLINE void __fastcall Store3Globals(int32_t* v) {
    g_6cea68 = v[0];
    g_6cea6c = v[1];
    g_6cea70 = v[2];
}
RT3_REPLACE(0x475010, Store3Globals);

// 0x475030. Copies the 3 stored dwords (0x6cea68..) to +0x1b0 of element idx (-1 = all).
RT3_NOINLINE void __fastcall Obj_ApplyStored3(void* o, int32_t idx) {
    auto apply = [](uint8_t* e) {
        F<int32_t>(e, 0x1b0) = g_6cea68;
        F<int32_t>(e, 0x1b4) = g_6cea6c;
        F<int32_t>(e, 0x1b8) = g_6cea70;
    };
    if (idx == -1) {
        for (int32_t i = 0; i < F<int32_t>(o, 8); ++i) apply(F<uint8_t*>(o, 0xc + i * 4));
    } else {
        apply(F<uint8_t*>(o, 0xc + idx * 4));
    }
}
RT3_REPLACE(0x475030, Obj_ApplyStored3);

// 0x4750a0. Sum over the entries (+0xc, count +8) of their sizes: +0x38 when
// flag 0x400 of +0xac, otherwise (+4)->+8.
RT3_NOINLINE int32_t __fastcall Obj_SumEntrySizes(uint8_t* o) {
    int32_t sum = 0;
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i) {
        uint8_t* e = F<uint8_t*>(o, 0xc + i * 4);
        int32_t v = 0;
        if (F<uint32_t>(e, 0xac) & 0x400) v = F<int32_t>(e, 0x38);
        else if (uint8_t* s = F<uint8_t*>(e, 4)) v = F<int32_t>(s, 8);
        sum += v;
    }
    return sum;
}
RT3_REPLACE(0x4750a0, Obj_SumEntrySizes);

RT3_NOINLINE int32_t __fastcall Obj_SumEntries6c(uint8_t* o) {
    int32_t sum = 0;
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i) sum += F<int32_t>(F<uint8_t*>(o, 0xc + i * 4), 0x6c);
    return sum;
}
RT3_REPLACE(0x4750e0, Obj_SumEntries6c);

// 0x475100. Frees a scene effect: deletes its models (+0xc, count +8) from the
// scene +4's model list (+0x79), unlinks it from the active effects 0x6cea74
// when it is listed (+0x34 = 1) and returns it to the effect pool 0x6cea58.
RT3_NOINLINE void __fastcall Effect_Free(uint32_t* e) {
    if (!e) return;
    if (uint8_t* scene = (uint8_t*)(uintptr_t)e[1]) {
        if (PtrList* list = F<PtrList*>(scene, 0x79)) {
            for (int32_t i = 0; i < (int32_t)e[2]; ++i) {
                PtrList_Rewind(list);
                for (uint8_t* m; (m = (uint8_t*)PtrList_CurrentValue(list)) != nullptr;) {
                    if ((uint32_t)(uintptr_t)m == e[3 + i]) {
                        PtrList_RemoveCurrent(list);
                        Model_Release(m);
                        crt::operator_delete(m);
                        break;
                    }
                    PtrList_Next(list);
                }
            }
        }
    }
    if (e[0xd] == 1) SList_Unlink(&hook::As<uint32_t>(g_6cea74), &g_6cea78, &g_6cea7c, e);
    Pool_Return(hook::As<uint8_t*>(g_6cea58), e);
}
RT3_REPLACE(0x475100, Effect_Free);

// 0x475240. Sets elements' +0x1d8 flag: 0 when mode == 1, else 1; idx -1 = all.
void __fastcall Obj_SetElementFlag1d8(void* o, int32_t mode, int32_t idx) {
    uint8_t v = mode == 1 ? 0 : 1;
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i)
        if (idx == -1 || i == idx) F<uint8_t>(F<uint8_t*>(o, 0xc + i * 4), 0x1d8) = v;
}
RT3_REPLACE(0x475240, Obj_SetElementFlag1d8);

int32_t __fastcall Obj_EntryClear1d8(uint8_t* o, int32_t i) {
    if (i >= F<int32_t>(o, 8)) return 0;
    return F<uint8_t>(F<uint8_t*>(o, 0xc + i * 4), 0x1d8) == 0 ? 1 : 0;
}
RT3_REPLACE(0x4752b0, Obj_EntryClear1d8);

 // namespace

// 0x4752d0. A broken-down train: steam, flames and smoke.
uint8_t* __stdcall EffectGroup_BrokenDownCrash(uint8_t* parent, float scale, uint32_t flags, int32_t) {
    static const char* const defs[] = { (const char*)0x5ce538, (const char*)0x5ce52c, (const char*)0x5ce51c };
    return EffectGroup(parent, scale, flags | 0x8000, defs, nullptr, 3);
}
RT3_REPLACE(0x4752d0, EffectGroup_BrokenDownCrash);

// 0x475420. A volcano: black smoke (flag 0x10000, rate +0x94 from its def
// +0xc8) and lava (+0x1e9 = 0x371c0732), sized 0.5 / 0.75 / 1 / 1.5 / 2 (no
// effects for other scales).
uint8_t* __stdcall EffectGroup_Volcano(uint8_t* parent, float scale, uint32_t flags) {
    uint8_t* node = EffectNode();
    uint8_t* smoke = nullptr;
    uint8_t* lava = nullptr;
    uint32_t bits = *(uint32_t*)&scale;
    static const uint32_t sizes[] = { 0x3f000000, 0x3f400000, 0x3f800000, 0x3fc00000, 0x40000000 };
    static const char* const smokes[] = { (const char*)0x5ce5ec, (const char*)0x5ce5c8, (const char*)0x5ce5a4, (const char*)0x5ce580, (const char*)0x5ce55c };
    static const char* const lavas[] = { (const char*)0x5ce5dc, (const char*)0x5ce5b8, (const char*)0x5ce594, (const char*)0x5ce570, (const char*)0x5ce54c };
    for (int k = 0; k < 5; ++k)
        if (bits == sizes[k]) {
            smoke = (uint8_t*)FindRecordByCrc(smokes[k]);
            lava = (uint8_t*)FindRecordByCrc(lavas[k]);
            break;
        }
    F<uint8_t*>(node, 0xc) = Effect_Spawn(nullptr, parent, smoke, flags | 0x10000, scale);
    if (!parent) parent = F<uint8_t*>(F<uint8_t*>(node, 0xc), 0xc);
    F<uint32_t>(F<uint8_t*>(node, 0xc), 0x94) = F<uint32_t>(smoke, 0xc8);
    uint8_t* e = Effect_Spawn(nullptr, parent, lava, flags, scale);
    F<uint8_t*>(node, 0x10) = e;
    F<uint32_t>(e, 0x1e9) = 0x371c0732;
    F<int32_t>(node, 8) = 2;
    return node;
}
RT3_REPLACE(0x475420, EffectGroup_Volcano);

// 0x475550. A broken-down train: steam and smoke.
uint8_t* __stdcall EffectGroup_BrokenDown(uint8_t* parent, float scale, uint32_t flags) {
    static const char* const defs[] = { (const char*)0x5ce538, (const char*)0x5ce600 };
    return EffectGroup(parent, scale, flags | 0x8000, defs, nullptr, 2);
}
RT3_REPLACE(0x475550, EffectGroup_BrokenDown);

__declspec(naked) void EffectGroup_ServiceTower() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call EffectGroup_ServiceTowerImpl
        ret 8
    }
}
RT3_REPLACE(0x475650, EffectGroup_ServiceTower);

__declspec(naked) void EffectGroup_Rocket() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call EffectGroup_RocketImpl
        ret 8
    }
}
RT3_REPLACE(0x4757b0, EffectGroup_Rocket);

__declspec(naked) void EffectGroup_Fireworks() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call EffectGroup_FireworksImpl
        ret 8
    }
}
RT3_REPLACE(0x475910, EffectGroup_Fireworks);

__declspec(naked) void EffectGroup_CargoMoney() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call EffectGroup_CargoMoneyImpl
        ret 8
    }
}
RT3_REPLACE(0x4759f0, EffectGroup_CargoMoney);

// 0x475a90. The selection marker.
uint8_t* __stdcall EffectGroup_SelectObject(uint8_t* parent, float scale, uint32_t flags) {
    static const char* const defs[] = { (const char*)0x5ce774 };
    return EffectGroup(parent, scale, flags, defs, nullptr, 1);
}
RT3_REPLACE(0x475a90, EffectGroup_SelectObject);

__declspec(naked) void EffectGroup_SpendMoney() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call EffectGroup_SpendMoneyImpl
        ret 8
    }
}
RT3_REPLACE(0x475b30, EffectGroup_SpendMoney);

// 0x475bd0. The construction cursor: centre and the four directions.
uint8_t* __stdcall EffectGroup_Construct(uint8_t* parent, float scale, uint32_t flags) {
    static const char* const defs[] = { (const char*)0x5ce7e0, (const char*)0x5ce7d0, (const char*)0x5ce7c0, (const char*)0x5ce7ac, (const char*)0x5ce79c };
    return EffectGroup(parent, scale, flags | 0xc000, defs, nullptr, 5);
}
RT3_REPLACE(0x475bd0, EffectGroup_Construct);

__declspec(naked) void EffectGroup_Mist() {
    __asm {
        push [esp + 4]
        push ebx
        push edi
        call EffectGroup_MistImpl
        ret 4
    }
}
RT3_REPLACE(0x475dc0, EffectGroup_Mist);

// 0x475e30. A vent.
uint8_t* __stdcall EffectGroup_Vent(uint8_t* parent, float scale, uint32_t flags) {
    static const char* const defs[] = { (const char*)0x5ce804 };
    return EffectGroup(parent, scale, flags, defs, nullptr, 1);
}
RT3_REPLACE(0x475e30, EffectGroup_Vent);

// 0x475ed0. Creates effect group `kind` (1-3 crash smoke, 4 volcano, 5
// broken down, 6 service tower, 7 / 9 money, 8 construction, 10 rocket, 11
// fireworks, 12 selection, 13 mist, 14 vent; else none) on `parent`; the
// node keeps the first effect's parent (+4) and `listed` (+0x34), and listed
// ones go on the front of the effect list 0x6cea74 (tail +4, count +8).
RT3_NOINLINE uint8_t* __fastcall Effect_Create(uint8_t* parent, int32_t kind, float scale, uint32_t flags, int32_t listed) {
    uint8_t* n;
    switch (kind) {
    case 1: case 2: case 3: n = EffectGroup_BrokenDownCrash(parent, scale, flags, kind - 1); break;
    case 4: n = EffectGroup_Volcano(parent, scale, 8); break;
    case 5: n = EffectGroup_BrokenDown(parent, scale, flags); break;
    case 6: n = EffectGroup_ServiceTowerImpl(flags, parent, scale); break;
    case 7: n = EffectGroup_CargoMoneyImpl(flags, parent, scale); break;
    case 8: n = EffectGroup_Construct(parent, scale, flags); break;
    case 9: n = EffectGroup_SpendMoneyImpl(flags, parent, scale); break;
    case 10: n = EffectGroup_RocketImpl(flags, parent, scale); break;
    case 11: n = EffectGroup_FireworksImpl(flags, parent, scale); break;
    case 12: n = EffectGroup_SelectObject(parent, scale, flags); break;
    case 13: n = EffectGroup_MistImpl(flags, parent, scale); break;
    case 14: n = EffectGroup_Vent(parent, scale, flags); break;
    default: return nullptr;
    }
    F<uint8_t*>(n, 4) = F<uint8_t*>(F<uint8_t*>(n, 0xc), 0xc);
    F<int32_t>(n, 0x34) = listed;
    if (listed == 1) {
        F<uint8_t*>(n, 0) = g_6cea74;
        g_6cea74 = n;
        ++g_6cea7c;
        if (!hook::As<uint8_t*>(g_6cea78)) hook::As<uint8_t*>(g_6cea78) = n;
    }
    return n;
}
RT3_REPLACE(0x475ed0, Effect_Create);

}  // namespace rt3
