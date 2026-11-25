// DevMenu.cpp - RT3.exe 0x462000-0x468000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "game/calendar.h"
#include "core/crt.h"
#include "core/imports.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"

#undef near
#undef far
#undef small

struct HWND__;

struct HMENU__;

typedef HMENU__* HMENU;

namespace rt3 {

::HWND__* __fastcall Engine_Window(void* e);
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
uint8_t* __fastcall Obj_RegionAt(void* o);
void __fastcall Obj_AddFloat103(void* o, int, int32_t i, float d);
void __fastcall Load_Unlink(uint32_t* o);
void* __fastcall Obj_Ref2a8(void* o);
int32_t __fastcall Obj_Array590(void* o, int, int32_t i);
uint8_t* __fastcall Obj_Region(void* o);
int32_t __fastcall Game_EffectiveVersion();
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void* __fastcall Obj_Building(void* o);
uint8_t __fastcall Obj_Flag3d4(void* o);
int32_t __fastcall Obj_ListEntry(uint8_t* o, int, int32_t i, int32_t list, int32_t* out);
float __fastcall Obj_NodeCenterY(void* o);
float __fastcall Obj_NodeCenterX(void* o);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void* __fastcall Obj_CityRef(void* o);
int32_t __fastcall Pairs_Total(uint8_t* o);
uint8_t* __fastcall Pairs_At(uint8_t* o, int, int32_t list, int32_t i);
void __fastcall Building_ChargeUpkeep(uint8_t* b);
int32_t __fastcall Station_ConnectedTo(uint8_t* s, int, uint32_t id);
void __fastcall Lot_UpdateLinked(uint8_t* o, int, int32_t);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void __fastcall Lot_AttachTo(uint32_t* o, int, uint32_t id);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
int32_t __fastcall Sound_GetSpeakers(uint8_t* s);
uint32_t __fastcall Engine_IsFullscreen(void* e);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
int32_t __fastcall Company_OwnerFlag1e1(void* c);
HWND __fastcall Engine_Window(void* e);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_MarkDirty(uint8_t* s);
void __fastcall Scene_ApplyLighting(uint8_t* s);
int32_t __fastcall Engine_FreeTextureMem(void* e);
int32_t __fastcall Collection_Count(Collection* c);
void __fastcall Scene_ApplyLighting(uint8_t* r);
void __fastcall Scene_MarkDirty(uint8_t* r);
uint32_t __fastcall Sound_ActiveProvider(uint8_t* s);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

void __fastcall Tracks_UpdateColor(Collection* c);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void* __fastcall Obj_Ref25(void* o);
int32_t __fastcall Obj_Get25(void* o);
void __fastcall Obj_Incc60(uint8_t* o);
uint32_t __fastcall News_Add(Collection* t, int, const char* title, const char* text, int32_t a, int32_t b);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
void __fastcall Player_Init(uint8_t* p, int, int32_t id, int32_t kind, const char* name, const char* desc, int32_t ai, int32_t clearHistory);
int32_t __fastcall Sound_ActiveProviderIndex(uint8_t* s);
void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind);
void __fastcall Obj_Decc60(uint8_t* o);
void __fastcall Menu_UpdateChecks(HMENU* m, int, char relight);
void __fastcall Options_ApplyGraphics(uint8_t* o, int, int32_t which, int32_t rebuild);
int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute);
Collection* __fastcall Collection_SetVtbl(Collection* c);

void __fastcall Collection_Init(Collection* c, int, int32_t inlineRecords, int32_t stride, int32_t initialCapacity, int32_t growBy, int32_t list1,
                                int32_t list2, int32_t list3);

void __fastcall Textures_Manage(int32_t now, int32_t statsOnly);
void __fastcall Camera_SetZoom(uint8_t* c, int, uint32_t zoom);
uint8_t* __fastcall Vehicles_AddNamed(Collection* t, int, const char* name);
void __fastcall Obj_MarkDone(uint8_t* o);
void* __fastcall Obj_Company21(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);
void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled);
void __stdcall Options_SetWeatherOverride(int32_t on, int32_t level);
void __fastcall Obj_SetPlayer(void* o, int, uint32_t player);
void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
void __fastcall World_SetEndMode(uint8_t* w, int, int32_t mode);
void __fastcall Map_RescaleHeights(uint8_t* m, int, float scale, int32_t mode);
void __fastcall Overview_Close(uint8_t* t);
void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
void __fastcall CargoTypes_Recount(Collection* t);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
uint32_t __fastcall Vehicles_MaybeSpawnFlight(Collection* t, int, float chance, int32_t force);
RT3_NOINLINE void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
void __fastcall World_EconomyTick(uint8_t* w, int, int32_t force);
RT3_NOINLINE void __fastcall CargoTypes_MarkAvailable(Collection* c);
Collection* __fastcall EngineTypes_Construct(Collection* t);
RT3_NOINLINE void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled);
RT3_NOINLINE void __fastcall CargoTypes_Recount(Collection* t);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);

 // namespace

void __fastcall App_SetScreen(uint8_t* a, int, int32_t screen, int32_t closeWorld);

RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
void __fastcall Options_SetDisplayMode(uint8_t* o, int, int32_t m);
RT3_NOINLINE void __fastcall Options_ApplyGraphics(uint8_t* o, int, int32_t which, int32_t rebuild);
RT3_NOINLINE void __fastcall Menu_UpdateChecks(HMENU* m, int, char relight);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
RT3_NOINLINE void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);
uint32_t __fastcall ScenarioText_Import(const char* name, int32_t report);
int32_t __fastcall ScenarioText_Export(const char* name, int32_t report);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
void __fastcall Trains_AddAndShow(const char* name, int32_t follow);
void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);
int32_t __fastcall Sound_Restart(uint8_t* snd, int, const char* name);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall World_Advance(uint8_t* w, int, int32_t minutes, int32_t nested);
void __fastcall CargoTypes_MarkAvailable(Collection* c);
void __fastcall Obj_ClearGrids(void* o);
void __fastcall TrainObj_Clicked(uint8_t* o);
void __fastcall Players_AdjustAiCash(int32_t amount);
void __fastcall Companies_AdjustCash(int32_t amount);
void __stdcall Game_SetFixedTime(int32_t on, int32_t hour);
void __fastcall Options_SetDefaults(uint8_t* o, int, int32_t preset);
void __fastcall Options_AutoDetail(uint8_t* o);
int32_t Global6cfc8cSet();
void Tank_SpawnDemo();
void CursorMarker_Free();
void __fastcall Maps_BatchScenarioText(int32_t import);
void __fastcall Renderer_ResetSettings(uint8_t* o, int, int32_t kind);
void __fastcall Engine_SetFreeTexMemOverride(void* e, int, int32_t v);
void __fastcall Engine_SetTotalTexMemOverride(void* e, int, int32_t v);
void __fastcall Engine_SetTexMemOverride3(void* e, int, int32_t v);
void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);
void __fastcall Ui_ReloadImages(uint8_t* ui);
double __fastcall Render_ScaleByUnit(uint8_t*, int, float v);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall World_Advance(uint8_t* w, int, int32_t minutes, int32_t nested);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall Map_RescaleHeights(uint8_t* m, int, float scale, int32_t mode);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall TrainObj_Clicked(uint8_t* o);
RT3_NOINLINE void __fastcall EngineTypes_Load(Collection* t);
RT3_NOINLINE Collection* __fastcall EngineTypes_Construct(Collection* t);
uint32_t* __fastcall Pair_Init(uint32_t* p);
void __fastcall GdiPair_Release(void** p);
RT3_NOINLINE void __fastcall Menu_UpdateChecks(HMENU* m, int, char relight);
void __fastcall GdiPair_Release2(void** p);
RT3_NOINLINE void __fastcall Menu_Attach(HMENU* m);
RT3_NOINLINE void __fastcall Trains_AddAndShow(const char* name, int32_t follow);
RT3_NOINLINE void __fastcall Players_AdjustAiCash(int32_t amount);
RT3_NOINLINE void __fastcall Companies_AdjustCash(int32_t amount);
void __fastcall DevMenu_Command(HMENU* m, int, uint32_t cmd);
int32_t __fastcall Load_Age(uint8_t* l);
int32_t __fastcall Station_LoadShare(uint8_t* o, int, uint8_t* from, uint8_t* to, uint8_t* city, float* out, int32_t apply);
RT3_NOINLINE void __fastcall Route_ChargeRandomBuilding(uint8_t* o, int, uint8_t* r, int32_t kind);
void __fastcall Lot_UpdateLinked(uint8_t* o, int, int32_t);
RT3_NOINLINE void __fastcall Load_Unlink(uint32_t* o);
RT3_NOINLINE void __fastcall Lot_AttachTo(uint32_t* o, int, uint32_t id);
void __fastcall Load_AttachToTrain(uint32_t* load, int, uint32_t id);
void __fastcall Lot_Create(uint32_t src, uint32_t dst, int32_t kind);
RT3_NOINLINE void __fastcall App_SetScreen(uint8_t* a, int, int32_t screen, int32_t closeWorld);
RT3_NOINLINE void __fastcall Options_SetDefaults(uint8_t* o, int, int32_t preset);
RT3_NOINLINE void __fastcall Options_AutoDetail(uint8_t* o);
RT3_NOINLINE void CursorMarker_Free();
RT3_NOINLINE int32_t __fastcall ScenarioText_Export(const char* name, int32_t report);
RT3_NOINLINE uint32_t __fastcall ScenarioText_Import(const char* name, int32_t report);
RT3_NOINLINE void __fastcall Maps_BatchScenarioText(int32_t import);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);
RT3_NOINLINE void __fastcall Scene_ApplyLighting(uint8_t* s);

// 0x462570 / 0x464130. Releases a GDI pair {object, dc}.
static void ReleaseGdiPair(void** p) {
    if (!p[1]) return;
    p[1] = nullptr;
    if (void* e = hook::As<void*>(g_engine))
        (imp::SetMenu)(Engine_Window(e), nullptr);
    if (p[0]) (imp::DestroyMenu)(p[0]);
    p[0] = nullptr;
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(float f) {
    int32_t r;
    __asm {
        fld f
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static inline Collection* Stations() { return g_stations; }

static inline uint8_t* Cargos() { return hook::As<uint8_t*>(g_cargoTypes); }

static inline int32_t Ftol_174(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static inline int32_t Ftol_205(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static inline int32_t Ftol_207(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static inline float BitsF(uint32_t b) { return *(float*)&b; }

// 0x462520. Engine type table t: an inline collection of 0xad-byte records
// (vtable 0x5ce068) filled from the .lco files.
RT3_NOINLINE Collection* __fastcall EngineTypes_Construct(Collection* t) {
    Collection_SetVtbl(t);
    F<uint32_t>(t, 0) = 0x5ce068;
    Collection_Init(t, 0, 1, 0xad, 0xa, 5, 1, 0, 0);
    EngineTypes_Load(t);
    return t;
}
RT3_REPLACE(0x462520, EngineTypes_Construct);

uint32_t* __fastcall Pair_Init(uint32_t* p) {
    p[0] = 0;
    p[1] = 0;
    return p;
}
RT3_REPLACE(0x462560, Pair_Init);

void __fastcall GdiPair_Release(void** p) { ReleaseGdiPair(p); }
RT3_REPLACE(0x462570, GdiPair_Release);

// 0x4625b0. Brings the main window's menu check marks (and a few enabled states) in
// line with the engine, display and game options; relight also reapplies the scene
// lighting before the scene is marked dirty.
RT3_NOINLINE void __fastcall Menu_UpdateChecks(HMENU* m, int, char relight) {
    typedef BOOL(WINAPI * CheckFn)(HMENU, UINT, UINT);
    typedef BOOL(WINAPI * EnableFn)(HMENU, UINT, UINT);
    typedef int(WINAPI * GetStrFn)(HMENU, UINT, LPSTR, int, UINT);
    if (!g_engine || !g_windowManager) return;
    CheckFn checkFn = ((CheckFn)imp::CheckMenuItem);
    auto E = [] { return g_engine; };
    auto O = [] { return g_app; };
    auto check = [&](UINT id, UINT flag) {
        if (*m) checkFn(*m, id, flag);
    };
    auto on = [](bool b) -> UINT { return b ? 8 : 0; };
    auto eb = [&](uint32_t off) { return on(E()[off] != 0); };
    auto ei = [&](uint32_t off) { return on(F<int32_t>(E(), off) != 0); };
    auto oi = [&](uint32_t off) { return on(F<int32_t>(O(), off) != 0); };
    check(0x9c41, ei(0x114206));
    check(0x9c42, eb(0x1146a3));
    check(0x9c6f, eb(0x1146a4));
    check(0x9c52, eb(0x1146e1));
    check(0x9c7f, eb(0x1146e8));
    check(0x9c49, eb(0x1146ab));
    check(0x9c4a, eb(0x1146ac));
    check(0x9c50, eb(0x1146ad));
    check(0x9c51, eb(0x1146ae));
    check(0x9c7a, eb(0x1146b7));
    check(0x9c80, eb(0x1146a9));
    check(0x9c72, on(F<int32_t>(E(), 0x114212) == 0));
    check(0xe101, ei(0x11471a));
    check(0xe102, ei(0x11471e));
    check(0xe103, ei(0x11472a));
    check(0xe104, ei(0x11472e));
    bool half = F<float>(E(), 0x1146af) == D(0x5c8628);
    EnableFn enableFn = ((EnableFn)imp::EnableMenuItem);
    check(0x9c4b, on(half));
    check(0x9c62, eb(0x1146e4));
    check(0x9c4c, eb(0x1146a5));
    check(0x9c4e, eb(0x1146a6));
    check(0x9c4f, eb(0x1146a7));
    check(0x9c79, eb(0x1146e5));
    if (*m) enableFn(*m, 0x9c53, 1);
    check(0x9c9c, eb(0x1146e9));
    check(0x9c54, eb(0x1146eb));
    check(0x8a, eb(0x1146ed));
    check(0x8b, eb(0x1146ea));
    check(0x9cca, oi(0x263));
    // Some options need enough free texture memory.
    if (Engine_FreeTextureMem(E()) < 0x800000) {
        if (*m) enableFn(*m, 0x9c6e, 1);
    } else check(0x9c6e, eb(0x1146e2));
    check(0x9c6d, eb(0x1146e3));
    check(0x9c7e, eb(0x1146e7));
    check(0x9ca2, eb(0x1146e6));
    check(0x9c56, eb(0x1146a8));
    check(0x9c57, eb(0x1146a2));
    check(0x9c94, 0);
    check(0x9c95, 0);
    check(0x9c64, 0);
    switch (F<int32_t>(E(), 0x1146b3)) {
    case 0: check(0x9c94, 8); break;
    case 1: check(0x9c95, 8); break;
    case 2: check(0x9c64, 8); break;
    }
    check(0x9c71, eb(0x1146d7));
    check(0x9c9e, eb(0x1146d8));
    check(0x9c9b, oi(0x108));
    check(0x9c9a, oi(0x104));
    check(0x87, eb(0x1146ef));
    check(0x88, eb(0x1146f0));
    check(0x9c7c, eb(0x1146b8));
    check(0x9c97, oi(0xa0));
    for (UINT id = 0xe2f4; id <= 0xe2f7; ++id) check(id, 0);
    switch (F<int32_t>(O(), 0x1eb)) {
    case 0: check(0xe2f4, 8); break;
    case 1: check(0xe2f5, 8); break;
    case 2: check(0xe2f6, 8); break;
    case 3: check(0xe2f7, 8); break;
    }
    if (E()) {
        for (UINT id = 58000; id <= 0xe293; ++id) check(id, 0);
        if (F<int32_t>(E(), 0x114722) == 0) check(58000, 8);
        else {
            switch (F<int32_t>(E(), 0x114726)) {
            case 0x46: check(0xe291, 8); break;
            case 0x78: check(0xe292, 8); break;
            case 0xf0: check(0xe293, 8); break;
            }
        }
        check(0x84, 0);
        check(0x85, 0);
        check(0x86, 0);
        switch (F<int32_t>(E(), 0x11420a)) {
        case 0: check(0x84, 8); break;
        case 1: check(0x85, 8); break;
        case 2: check(0x86, 8); break;
        }
    }
    bool three = F<int32_t>(E(), 0x114228) == 3;
    check(0x9c83, on(three));
    check(0x9c82, on(!three));
    if (E() && Engine_Renderer(E())) {
        if (relight) Scene_ApplyLighting((uint8_t*)Engine_Renderer(E()));
        Scene_MarkDirty((uint8_t*)Engine_Renderer(E()));
    }
    for (int32_t k = 0; k < 10; ++k)
        for (UINT id = k + 51000, n = 0; n < 4; ++n, id += 100) {
            check(id - 1000, 0);
            check(id, 0);
            check(id + 1000, 0);
        }
    uint8_t* ui = g_windowManager;
    for (UINT id = 0xcf6c; id <= 0xcf72; ++id) check(id, on(((1u << (id - 0xcf6c)) & F<uint32_t>(g_windowManager, 0xc79)) != 0));
    (void)ui;
    check(0xcf76, (F<uint32_t>(g_windowManager, 0xc79) >> 9) & 8);
    check(0xcfd0, on(g_6d3ff8 != 0));
    static const uint32_t kSel[][2] = { { 0x6d3de8, 50000 }, { 0x6d3dec, 0xc3b4 }, { 0x6d3df0, 0xc418 }, { 0x6d3df4, 0xc47c },
                                        { 0x6d3e30, 51000 }, { 0x6d3e34, 0xc79c }, { 0x6d3e38, 0xc800 }, { 0x6d3e3c, 0xc864 },
                                        { 0x6d3f20, 52000 }, { 0x6d3f24, 0xcb84 }, { 0x6d3f28, 0xcbe8 }, { 0x6d3f2c, 0xcc4c } };
    for (auto& s : kSel) {
        int32_t v = hook::Global<int32_t>(s[0]);
        if (v <= 10) check(v + s[1], 8);
    }
    auto sel = [&](UINT base, int32_t n, int32_t cur) {
        for (int32_t k = 0; k < n; ++k) check(base + k, on(cur == k));
    };
    sel(0xd048, 5, F<int32_t>(O(), 0x10c));
    check(0xd09a, oi(0x70));
    check(0xd09b, oi(0x184));
    check(0x9c99, oi(0xf0));
    sel(0x9d26, 3, F<int32_t>(O(), 0x178));
    static const uint32_t kOpt1[][2] = { { 0x9cb9, 0x174 }, { 0x9cd4, 0x2c8 }, { 0x9cb7, 0x16c }, { 0x9cb4, 0x14c }, { 0x9cb3, 0x148 } };
    for (auto& o : kOpt1) check(o[0], oi(o[1]));
    check(0x9c63, ei(0x11469c));
    static const uint32_t kOpt2[][2] = { { 0x9c60, 0x74 },   { 0x9c70, 0x78 },   { 0x9c61, 0x7c },   { 0x9c8d, 0x8c },   { 0x9c8e, 0x90 },
                                         { 0x9cb6, 0x168 },  { 0x9c9d, 0x110 },  { 0x9ca0, 0x130 },  { 0x9cc6, 0x24f },  { 0x9ca1, 0x134 },
                                         { 0x9cb8, 0x170 },  { 0x9c9f, 0x1bf },  { 0x9cd1, 0x2b3 } };
    for (auto& o : kOpt2) check(o[0], oi(o[1]));
    check(0x9c98, eb(0x1146bd));
    static const uint32_t kOpt3[][2] = { { 0x9cd0, 0x2af }, { 0x9ccc, 0x27f }, { 0x9c8f, 0x80 }, { 0x9ccf, 0x293 }, { 0x9c96, 0x84 },
                                         { 0x9ca8, 0x88 },  { 0x9c90, 0x94 },  { 0x9c92, 0x98 }, { 0x9c91, 0x9c },  { 0x9cb2, 0x144 },
                                         { 0x9c93, 0xa8 },  { 0x9e23, 0xfc } };
    for (auto& o : kOpt3) check(o[0], oi(o[1]));
    check(0x9c5f, ei(0x114716));
    sel(0x9d1c, 3, F<int32_t>(O(), 0x253));
    sel(0x9d12, 3, F<int32_t>(O(), 0x22f));
    sel(0x9d30, 3, F<int32_t>(O(), 0x2bb));
    sel(0x9d3a, 3, F<int32_t>(O(), 0x2bf));
    sel(0x9d08, 3, F<int32_t>(E(), 0x1146d9));
    sel(0x9d6c, 6, F<int32_t>(E(), 0x1146dd));
    // Three 0..10 sliders, as the scaled settings round to.
    for (int32_t k = 0; k <= 10; ++k) {
        if (!*m) continue;
        auto level = [&](uint32_t off) { return Ftol_205(((double)F<float>(E(), off) + D(0x5c85b8)) * D(0x5c85f0)); };
        check(k + 0xd160, on(level(0x1146fa) == k));
        if (!*m) continue;
        check(k + 0xd1c4, on(level(0x1146fe) == k));
        if (!*m) continue;
        check(k + 0xd228, on(level(0x114702) == k));
    }
    sel(0xdf0c, 6, Sound_GetSpeakers(g_sound));
    // Sound provider items: checked when the active provider's name contains the item text.
    GetStrFn getStr = ((GetStrFn)imp::GetMenuStringA);
    for (UINT id = 0xdea9; id < 0xdea9 + 8; ++id) {
        if (!*m) continue;
        char text[256];
        getStr(*m, id, text, 0xff, 0);
        const char* prov = (const char*)(uintptr_t)Sound_ActiveProvider(g_sound);
        bool has = ((const char*)crt::strstr(prov, text)) != nullptr;
        check(id, on(has));
    }
    check(0x9cb5, ei(0x114712));
    for (int32_t k = 0; k < 10; ++k) {
        check(k + 0xd28c, on(F<int32_t>(E(), 0x1146c6) == k));
        check(k + 0xd354, on(F<int32_t>(E(), 0x1146ca) == k));
        check(k + 54000, on(F<int32_t>(E(), 0x1146ce) == k));
    }
    for (int32_t k = 0; k < 7; ++k) check(k + 0xd480, on(F<int32_t>(E(), 0x1146c2) + 1 == k));
    check(0x9cbb, oi(0x17c));
    check(0x9cbc, oi(0x180));
    check(0x9cc9, oi(0x25f));
    check(0x9ccb, oi(0x267));
    check(0x9cd5, on(O()[0x2d0] != 0));
    static const uint32_t kOpt4[][2] = { { 0xdf70, 0x150 }, { 0xdf71, 0x154 }, { 0xdf72, 0x158 }, { 0xdf73, 0x15c }, { 0xdf74, 0x160 },
                                         { 0xdf75, 0x164 }, { 0xd14c, 0x12c }, { 0x9cbd, 0x188 }, { 0x9e24, 0x1b3 }, { 0x9e25, 0x287 } };
    for (auto& o : kOpt4) check(o[0], oi(o[1]));
    check(0x9e26, on(g_62be84 != 0));
    static const uint32_t kOpt5[][2] = { { 0x9cc4, 0x1db }, { 0xe09e, 0x1c3 }, { 0xe09f, 0x1c7 }, { 0xe0a0, 0x1cb }, { 0xe0a1, 0x1cf },
                                         { 0xe0a2, 0x1d3 }, { 0x9cc5, 0x1e7 }, { 0x9ccd, 0x28f } };
    for (auto& o : kOpt5) check(o[0], oi(o[1]));
    check(0x9cce, ei(0x114732));
    static const uint32_t kOpt6[][2] = { { 0x9cd3, 0x2b7 }, { 0x9cc7, 0x257 }, { 0x9cc8, 0x25b }, { 0x9f26, 0x26b }, { 0x9f27, 0x26f },
                                         { 0x9f28, 0x273 }, { 0x9f29, 0x277 }, { 0x9f2a, 0x27b }, { 0x9e0c, 0x297 }, { 0x9e0d, 0x29b },
                                         { 0x9e0e, 0x29f }, { 0x9e0f, 0x2a3 }, { 0x9e10, 0x2a7 }, { 0x9e18, 0x2ab } };
    for (auto& o : kOpt6) check(o[0], oi(o[1]));
    for (UINT k = 0; k < 0x18; ++k) check(k + 0xdc50, 0);
    check(0xdc82, on(F<int32_t>(O(), 0x124) == 0));
    if (F<int32_t>(O(), 0x124)) check(F<int32_t>(O(), 0x128) + 0xdc50, 8);
    for (UINT k = 0; k < 10; ++k) check(k + 0xe167, 0);
    check(0xe165, on(F<int32_t>(O(), 0x1df) == 0));
    if (F<int32_t>(O(), 0x1df)) check(F<int32_t>(O(), 0x1e3) + 0xe167, 8);
    sel(0xdfd4, 3, F<int32_t>(O(), 0xa4));
    for (UINT k = 0; k < 0x13; ++k) check(k + 0xe038, on(O()[0x18c + k] != 0));
    sel(0x9cbe, 3, F<int32_t>(E(), 0x1146be));
}
RT3_REPLACE(0x4625b0, Menu_UpdateChecks);

void __fastcall GdiPair_Release2(void** p) { ReleaseGdiPair(p); }
RT3_REPLACE(0x464130, GdiPair_Release2);

// 0x464170. Attaches the main menu to the game window (windowed mode only): detaches
// first when there is none, then sets it, redraws the bar and updates the checks.
RT3_NOINLINE void __fastcall Menu_Attach(HMENU* m) {
    typedef BOOL(WINAPI * SetMenuFn)(HWND, HMENU);
    if (!hook::As<void*>(g_engine) || Engine_IsFullscreen(hook::As<void*>(g_engine))) return;
    SetMenuFn setMenu = ((SetMenuFn)imp::SetMenu);
    ((int32_t*)m)[1] = 1;
    if (!*m) {
        setMenu(Engine_Window(hook::As<void*>(g_engine)), nullptr);
        if (!*m) return;
    }
    setMenu(Engine_Window(hook::As<void*>(g_engine)), *m);
    Menu_UpdateChecks(m, 0, 0);
}
RT3_REPLACE(0x464170, Menu_Attach);

// 0x4641d0. Adds train `name` (0x62b244). With `follow` the camera follows it
// at zoom 300; otherwise message 0x1b7 is shown first and the train acts as
// clicked.
RT3_NOINLINE void __fastcall Trains_AddAndShow(const char* name, int32_t follow) {
    if (!follow) Dialog_RunMessage(GetString(0x1b7), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    uint8_t* o = Vehicles_AddNamed(g_62b244, 0, name);
    if (!o) return;
    if (!follow) {
        TrainObj_Clicked(o);
        return;
    }
    Obj_MarkDone(o);
    World_FollowEntity(g_world, 0, 1, 3, F<int32_t>(o, 0x23a));
    Camera_SetZoom(g_62be68, 0, 0x43960000);
}
RT3_REPLACE(0x4641d0, Trains_AddAndShow);

// 0x464260. Gives every AI player `amount` cash, or (amount 0) zeroes it.
RT3_NOINLINE void __fastcall Players_AdjustAiCash(int32_t amount) {
    Collection* players = g_players;
    if (!players) return;
    for (int32_t i = 0; i < Table_CountField4(players); ++i) {
        if (F<int32_t>(Table_NthField4(players, 0, i), 0x1e1)) continue;
        double v = amount ? (double)amount : -F<double>(Table_NthField4(players, 0, i), 0x154);
        Obj_AddDouble154(Table_NthField4(players, 0, i), 0, (int32_t)v);
    }
}
RT3_REPLACE(0x464260, Players_AdjustAiCash);

// 0x464310. Gives every company whose owner flag (0x425b90) is clear `amount` in
// cash (stat 4 when positive, else 0xc); amount 0 takes its whole cash away.
RT3_NOINLINE void __fastcall Companies_AdjustCash(int32_t amount) {
    Collection* cos = g_companies;
    if (!cos) return;
    for (int32_t i = 0; i < Collection_Count(g_companies); ++i) {
        Collection* t = g_companies;
        uint8_t* c = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (Company_OwnerFlag1e1(c)) continue;
        double v;
        if (amount) {
            v = amount;
        } else {
            t = g_companies;
            v = -Company_Stat((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, amount)), 0, 0xd, 0x2329);
        }
        int32_t n = Ftol_205(v);
        t = g_companies;
        Company_PostStat(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, n > 0 ? 4 : 0xc, (float)n, 1);
    }
}
RT3_REPLACE(0x464310, Companies_AdjustCash);

// 0x464410 (thiscall). Runs developer menu command cmd of the menu owner m
// (+0: the HMENU): renderer presets (0x7530..), scenario text export/import,
// demo vehicles to follow, end modes, the cheat screens, the date, the many
// option and engine debug toggles (0x9c41.., 0xdf70.., 0xe09c..), sound
// volumes / speakers / providers, overview modes, texture memory overrides,
// money for the player, companies and AI, players, news and log entries and
// the height rescales (0x9f60..). Then saves the options and refreshes the
// menu's checks.
void __fastcall DevMenu_Command(HMENU* m, int, uint32_t cmd) {
    auto app = []() { return g_app; };
    auto eng = []() { return g_engine; };
    auto game = []() { return g_world; };
    auto map = []() { return g_map; };
    auto snd = []() { return g_sound; };
    auto ui = []() { return g_windowManager; };
    auto flip = [](uint8_t* o, uint32_t off) { F<int32_t>(o, off) = F<int32_t>(o, off) == 0; };
    auto flipB = [](uint8_t* o, uint32_t off) { F<uint8_t>(o, off) = F<uint8_t>(o, off) == 0; };
    auto msg = [](int32_t id) {
        Dialog_RunMessage(GetString(id), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    };
    auto vehicles = []() { return g_62b244; };
    // A demo vehicle by name, then the camera follows it (zoomed out).
    auto follow = [&](const char* name) {
        if (uint8_t* v = Vehicles_AddNamed(vehicles(), 0, name)) {
            Obj_MarkDone(v);
            World_FollowEntity(game(), 0, 1, 3, F<int32_t>(v, 0x23a));
            Camera_SetZoom(g_62be68, 0, 0x43960000);
        }
    };
    auto spawn = [&](const char* name) {
        msg(0x1b7);
        if (uint8_t* v = Vehicles_AddNamed(vehicles(), 0, name)) TrainObj_Clicked(v);
    };
    auto checkZoom = [&](uint32_t on) {
        static const uint32_t kItems[5] = { 0x9c77, 0x9c76, 0x9c74, 0x9c75, 0x9c78 };
        auto check = ((uint32_t(__stdcall*)(HMENU, uint32_t, uint32_t))imp::CheckMenuItem);
        for (uint32_t id : kItems) {
            if (!*m) return;
            check(*m, id, id == on ? 8 : 0);
        }
    };
    auto setVolume = [&](uint32_t off, int32_t steps, double scale, int32_t kind) {
        F<float>(eng(), off) = (float)((double)steps / D(0x5c85f0));
        Sound_SetVolume(snd(), 0, (float)(F<float>(eng(), off) * scale), kind);
    };
    auto company = [&]() { return Obj_Company21(game()); };
    auto addStat = [&](int32_t stat, uint32_t bits) {
        if (company()) Company_PostStat(company(), 0, stat, BitsF(bits), 1);
    };
    if (cmd == 0x7530 || cmd == 0x7531 || cmd == 0x7532) {
        // A renderer preset (keeping +0x1146f2) with its option defaults.
        const uint32_t keep = F<uint32_t>(eng(), 0x1146f2);
        const int32_t kind = cmd == 0x7531 ? 1 : cmd == 0x7532 ? 2 : 0;
        Renderer_ResetSettings(eng(), 0, kind);
        F<uint32_t>(eng(), 0x1146f2) = keep;
        Options_SetDefaults(app(), 0, kind);
        Options_AutoDetail(app());
    } else if (cmd == 0x7541) {
        if (strlen((const char*)0x62bee0)) ScenarioText_Export((const char*)0x62bee0, 1);
    } else if (cmd == 0x7540) {
        if (strlen((const char*)0x62bee0)) ScenarioText_Import((const char*)0x62bee0, 1);
    } else if (cmd == 0x7542 || cmd == 0x7543) {
        Maps_BatchScenarioText(cmd == 0x7542);
    } else if (cmd == 0x7533) {
        if (Global6cfc8cSet()) {
            msg(0xb6f);
        } else {
            Obj_Incc60(ui());
            msg(0xb70);
            Tank_SpawnDemo();
        }
    } else if (cmd == 0x7534) {
        if (Global6cfc8cSet()) {
            Obj_Decc60(ui());
            CursorMarker_Free();
        } else {
            msg(0x1b8);
        }
    } else if (cmd == 0x753d) {
        if (vehicles() && game())
            if (const uint32_t id = Vehicles_MaybeSpawnFlight(vehicles(), 0, 0.0f, 1)) World_FollowEntity(game(), 0, 1, 3, (int32_t)id);
    } else if (cmd == 0x753e) {
        follow((const char*)0x5ce128);
    } else if (cmd == 0x7539) {
        follow((const char*)0x5ce120);
    } else if (cmd == 0x753a) {
        follow((const char*)0x5ce110);
    } else if (cmd == 0x753b) {
        follow((const char*)0x5ca310);
    } else if (cmd == 0x753c) {
        follow((const char*)0x5ce104);
    } else if (cmd == 0x7535) {
        spawn((const char*)0x5ce120);
    } else if (cmd == 0x7536) {
        spawn((const char*)0x5ce110);
    } else if (cmd == 0x7537) {
        spawn((const char*)0x5ca310);
    } else if (cmd == 0x7538) {
        spawn((const char*)0x5ce104);
    } else if (cmd >= 0x9e02 && cmd <= 0x9e05) {
        if (game()) World_SetEndMode(game(), 0, 0x9e06 - (int32_t)cmd);
    } else if (cmd >= 0x9e99 && cmd <= 0x9ea9) {
        // A cheat screen, only from the game screen.
        if (F<int32_t>(app(), 8) == 2) {
            g_624308 = (int32_t)cmd - 0x9e8e;
            App_SetScreen(app(), 0, 3, 0);
        } else {
            msg(0x1b9);
        }
    } else if (cmd == 0x753f) {
        // Jump to a year (1800..2999), asked with ten years ahead filled in.
        if (uint8_t* g = game()) {
            const uint32_t ymd = F<uint32_t>(g, 0xd), hm = F<uint32_t>(g, 0x11);
            char buf[0x100];
            Num_Format(buf, 8, (double)((ymd & 0xffff) + 10), 10, 1, 0, 0);
            Dialog_RunMessage(GetString(0x1ba), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)buf, 0x1e, 0, nullptr, nullptr, 0,
                              0, nullptr, 0);
            int32_t year = crt::atoi(buf);
            if (year < 0x708)
                year = 0x708;
            else if (year > 0xbb7)
                year = 0xbb7;
            const int32_t minutes =
                MinutesFromParts(year, (uint8_t)(ymd >> 16), (uint8_t)hm, (uint8_t)(hm >> 8), (uint8_t)(hm >> 16));
            World_SetTime(game(), 0, minutes);
            CargoTypes_Recount(g_cargoTypes);
            World_Advance(game(), 0, 1, 0);
        }
    } else if (cmd == 0x9c41) {
        flip(eng(), 0x114206);
    } else if (cmd >= 0xd160 && cmd <= 0xd16a) {
        setVolume(0x1146fa, (int32_t)cmd - 0xd160, 1.0, 2);
    } else if (cmd >= 0xd1c4 && cmd <= 0xd1ce) {
        setVolume(0x1146fe, (int32_t)cmd - 0xd1c4, D(0x5c9c80), 0);
    } else if (cmd >= 0xd228 && cmd <= 0xd232) {
        setVolume(0x114702, (int32_t)cmd - 0xd228, D(0x5c8640), 1);
    } else if (cmd >= 0xdf0c && cmd <= 0xdf11) {
        Sound_Speakers(snd(), 0, (int32_t)cmd - 0xdf0c, 0);
        F<int32_t>(eng(), 0x11470a) = (int32_t)cmd - 0xdf0c;
    } else if (cmd >= 0xdea9 && cmd <= 0xdeb0) {
        // A sound provider by its menu text.
        if (*m) {
            char name[0x100];
            (((int(__stdcall*)(HMENU, uint32_t, char*, int, uint32_t))imp::GetMenuStringA))(*m, cmd, name, 0xff, 0);
            Sound_Restart(snd(), 0, name);
            F<int32_t>(eng(), 0x114706) = Sound_ActiveProviderIndex(snd());
        }
    } else if (cmd == 0xe101) {
        F<uint32_t>(eng(), 0x11471a) ^= 1;
    } else if (cmd == 0xe102) {
        F<uint32_t>(eng(), 0x11471e) ^= 1;
        if (!F<int32_t>(eng(), 0x11471e)) F<int32_t>(eng(), 0x11472a) = 0;
    } else if (cmd == 0xe103 || cmd == 0xe104) {
        const uint32_t off = cmd == 0xe103 ? 0x11472a : 0x11472e;
        F<uint32_t>(eng(), off) ^= 1;
        if (F<int32_t>(eng(), off)) F<int32_t>(eng(), 0x11471e) = 1;
    } else if (cmd == 0x9cb5) {
        F<uint32_t>(eng(), 0x114712) ^= 1;
    } else if (cmd >= 0xdf70 && cmd <= 0xdf75) {
        flip(app(), 0x150 + (cmd - 0xdf70) * 4);
    } else if (cmd >= 0x9d08 && cmd <= 0x9d0a) {
        F<int32_t>(eng(), 0x1146d9) = (int32_t)cmd - 0x9d08;
        Ui_ReloadImages(ui());
    } else if (cmd >= 0x9d6c && cmd <= 0x9d71) {
        F<int32_t>(eng(), 0x1146dd) = (int32_t)cmd - 0x9d6c;
        Ui_ReloadImages(ui());
    } else if (cmd >= 0x9d12 && cmd <= 0x9d14) {
        F<int32_t>(app(), 0x22f) = (int32_t)cmd - 0x9d12;
    } else if (cmd >= 0x9d1c && cmd <= 0x9d1e) {
        F<int32_t>(app(), 0x253) = (int32_t)cmd - 0x9d1c;
    } else if (cmd >= 0x9d30 && cmd <= 0x9d32) {
        F<int32_t>(app(), 0x2bb) = (int32_t)cmd - 0x9d30;
    } else if (cmd >= 0x9d3a && cmd <= 0x9d3c) {
        F<int32_t>(app(), 0x2bf) = (int32_t)cmd - 0x9d3a;
        if (Collection* tracks = g_tracks) Tracks_UpdateColor(tracks);
    } else if (cmd == 0xd0fc) {
        if (map()) Overview_Close(map());
    } else if (cmd == 0xd14c) {
        flip(app(), 0x12c);
    } else if (cmd >= 0xd0fd && cmd <= 0xd12c) {
        if (map()) Overview_SetMode(map(), 0, (int32_t)cmd - 0xd0fc, 0, 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
    } else if (cmd == 0xd09b) {
        flip(app(), 0x184);
    } else if (cmd == 0xd09a) {
        flip(app(), 0x70);
    } else if (cmd == 0x9cc5) {
        flip(app(), 0x1e7);
        Options_ApplyGraphics(app(), 0, -2, 1);
    } else if (cmd >= 0x9cbe && cmd <= 0x9cc0) {
        F<int32_t>(eng(), 0x1146be) = (int32_t)cmd - 0x9cbe;
    } else if (cmd == 0x9c8f || cmd == 0x9cd0 || cmd == 0x9cd1 || cmd == 0x9ccc || cmd == 0x9ccf || cmd == 0x9c96 || cmd == 0x9ca8) {
        // Mutually exclusive debug views (one on, the others off).
        uint8_t* a = app();
        switch (cmd) {
        case 0x9c8f:
            flip(a, 0x80);
            F<int32_t>(app(), 0x84) = 0, F<int32_t>(app(), 0x27f) = 0, F<int32_t>(app(), 0x293) = 0, F<int32_t>(app(), 0x2af) = 0, F<int32_t>(app(), 0x2b3) = 0;
            break;
        case 0x9cd0:
            F<int32_t>(a, 0x27f) = 0, F<int32_t>(app(), 0x80) = 0, F<int32_t>(app(), 0x84) = 0, F<int32_t>(app(), 0x293) = 0;
            flip(app(), 0x2af);
            F<int32_t>(app(), 0x2b3) = 0;
            break;
        case 0x9cd1:
            flip(a, 0x2b3);
            F<int32_t>(app(), 0x80) = 0, F<int32_t>(app(), 0x84) = 0, F<int32_t>(app(), 0x293) = 0, F<int32_t>(app(), 0x2af) = 0, F<int32_t>(app(), 0x27f) = 0;
            break;
        case 0x9ccc:
            flip(a, 0x27f);
            F<int32_t>(app(), 0x80) = 0, F<int32_t>(app(), 0x84) = 0, F<int32_t>(app(), 0x293) = 0, F<int32_t>(app(), 0x2af) = 0, F<int32_t>(app(), 0x2b3) = 0;
            break;
        case 0x9ccf:
            flip(a, 0x293);
            F<int32_t>(app(), 0x80) = 0, F<int32_t>(app(), 0x84) = 0, F<int32_t>(app(), 0x27f) = 0, F<int32_t>(app(), 0x2af) = 0, F<int32_t>(app(), 0x2b3) = 0;
            break;
        case 0x9c96:
            flip(a, 0x84);
            F<int32_t>(app(), 0x88) = 0, F<int32_t>(app(), 0x80) = 0, F<int32_t>(app(), 0x27f) = 0, F<int32_t>(app(), 0x293) = 0, F<int32_t>(app(), 0x2af) = 0,
            F<int32_t>(app(), 0x2b3) = 0;
            break;
        default:  // 0x9ca8
            flip(a, 0x88);
            F<int32_t>(app(), 0x84) = 0, F<int32_t>(app(), 0x80) = 0, F<int32_t>(app(), 0x293) = 0, F<int32_t>(app(), 0x2af) = 0, F<int32_t>(app(), 0x2b3) = 0;
            break;
        }
    } else if (cmd >= 0xdfd4 && cmd <= 0xdfd6) {
        F<int32_t>(app(), 0xa4) = (int32_t)cmd - 0xdfd4;
    } else if (cmd == 0x9cc9) {
        flip(app(), 0x25f);
        CargoTypes_MarkAvailable(g_cargoTypes);
    } else if (cmd >= 0x9d26 && cmd <= 0x9d28) {
        F<int32_t>(app(), 0x178) = (int32_t)cmd - 0x9d26;
    } else if (cmd >= 0xc350 && cmd < 0xccb0) {
        // Debug values, a hundred commands each.
        static const struct { uint32_t base, global; } kValues[] = {
            { 0xc350, 0x6d3de8 }, { 0xc3b4, 0x6d3dec }, { 0xc418, 0x6d3df0 }, { 0xc47c, 0x6d3df4 }, { 0xc738, 0x6d3e30 }, { 0xc79c, 0x6d3e34 },
            { 0xc800, 0x6d3e38 }, { 0xc864, 0x6d3e3c }, { 0xcb20, 0x6d3f20 }, { 0xcb84, 0x6d3f24 }, { 0xcbe8, 0x6d3f28 }, { 0xcc4c, 0x6d3f2c },
        };
        for (const auto& v : kValues)
            if (cmd >= v.base && cmd < v.base + 100) hook::Global<int32_t>(v.global) = (int32_t)(cmd - v.base);
    } else if (cmd == 0x9c4b) {
        F<uint32_t>(eng(), 0x1146af) = F<float>(eng(), 0x1146af) == D(0x5c8628) ? 0 : 0x3f000000;
    } else if (cmd == 0x9cca) {
        // The grid overlay; in screen mode 0xc it also moves the cursor row.
        flip(app(), 0x263);
        if (uint8_t* root = g_gameWindow) {
            const int32_t mode12 = F<int32_t>(root, 0x8c) == 0xc;
            if (F<int32_t>(app(), 0x263)) {
                if (mode12) F<int32_t>(map(), 0x168d) = F<int32_t>(map(), 0x1689) + 1;
            } else if (mode12) {
                F<int32_t>(map(), 0x168d) = F<int32_t>(map(), 0x1689);
            }
        }
    } else if (cmd == 0x9c97) {
        flip(app(), 0xa0);
        flip(eng(), 0x114216);
    } else if (cmd == 0x9c82) {
        if (F<int32_t>(eng(), 0x114228) == 3) {
            F<int32_t>(eng(), 0x114228) = 2;
            Engine_ResetDisplay(eng(), 0, 1, -1);
        }
    } else if (cmd == 0x9c83) {
        if (F<int32_t>(eng(), 0x114228) == 2) {
            F<int32_t>(eng(), 0x114228) = 3;
            Engine_ResetDisplay(eng(), 0, F<uint8_t>(eng(), 0x114226), -1);
        }
    } else if (cmd == 0x9c94 || cmd == 0x9c95 || cmd == 0x9c64) {
        F<int32_t>(eng(), 0x1146b3) = cmd == 0x9c94 ? 0 : cmd == 0x9c95 ? 1 : 2;
    } else if (cmd >= 0x9c74 && cmd <= 0x9c78) {
        // The camera field of view, checked in the menu.
        static const uint32_t kFov[5] = { 0, 0x42c80000, 0x42a00000, 0x42700000, 0x42dc0000 };
        if (cmd == 0x9c74)
            F<uint32_t>(eng(), 0x1146d3) = 0x3fc90fdb;
        else
            F<float>(eng(), 0x1146d3) = (float)Render_ScaleByUnit(nullptr, 0, BitsF(kFov[cmd - 0x9c74]));
        checkZoom(cmd);
    } else if (cmd >= 0xd048 && cmd < 0xd052) {
        Options_SetDisplayMode(app(), 0, (int32_t)cmd - 0xd048);
    } else if (cmd >= 0xdc50 && cmd < 0xdc68) {
        Game_SetFixedTime(1, (int32_t)cmd - 0xdc50);
    } else if (cmd == 0xdc82) {
        Game_SetFixedTime(0, 0);
    } else if (cmd >= 0xe167 && cmd < 0xe171) {
        Options_SetWeatherOverride(1, (int32_t)cmd - 0xe167);
    } else if (cmd == 0xe165 || cmd == 0xe166) {
        // Back to the real weather (0xe165 only when overridden).
        if (cmd == 0xe166 || F<int32_t>(app(), 0x1df)) {
            if (game()) Weather_ScheduleNext(game(), 0, cmd == 0xe166);
            Options_SetWeatherOverride(0, 0);
        }
    } else if (cmd >= 0xe290 && cmd <= 0xe293) {
        static const int32_t kMinutes[4] = { 0, 0x46, 0x78, 0xf0 };
        if (cmd == 0xe290) {
            F<int32_t>(eng(), 0x114722) = 0;
        } else {
            F<int32_t>(eng(), 0x114722) = 1;
            F<int32_t>(eng(), 0x114726) = kMinutes[cmd - 0xe290];
        }
    } else if (cmd >= 0xe2f4 && cmd <= 0xe2f7) {
        F<int32_t>(app(), 0x1eb) = (int32_t)cmd - 0xe2f4;
    } else if (cmd >= 0xd480 && cmd <= 0xd486) {
        F<int32_t>(eng(), 0x1146c2) = (int32_t)cmd - 0xd480;
    } else if (cmd >= 0xe038 && cmd < 0xe09c) {
        if (cmd - 0xe038 < 0x13) flipB(app(), 0x18c + (cmd - 0xe038));
    } else if (cmd == 0xe09c || cmd == 0xe09d) {
        for (int32_t k = 0; k < 5; ++k) F<int32_t>(app(), 0x1c3 + k * 4) = cmd == 0xe09c;
    } else if (cmd >= 0x9f1a && cmd <= 0x9f1e) {
        if (game()) World_EconomyTick(game(), 0, (int32_t)cmd - 0x9f1a);
    } else if (cmd == 0x9f24 || cmd == 0x9f25) {
        for (int32_t k = 0; k < 5; ++k) F<int32_t>(app(), 0x26b + k * 4) = cmd == 0x9f24;
    } else if (cmd >= 0x9f26 && cmd <= 0x9f2a) {
        flip(app(), 0x26b + (cmd - 0x9f26) * 4);
    } else if (cmd >= 0xe09e && cmd <= 0xe0a2) {
        flip(app(), 0x1c3 + (cmd - 0xe09e) * 4);
    } else if (cmd >= 0x9e0c && cmd <= 0x9e10) {
        flip(app(), 0x297 + (cmd - 0x9e0c) * 4);
    } else if (cmd == 0x9e18) {
        flip(app(), 0x2ab);
    } else if (cmd == 0x9e16 || cmd == 0x9e17) {
        for (int32_t k = 0; k < 5; ++k) F<int32_t>(app(), 0x297 + k * 4) = cmd == 0x9e16;
    } else if (cmd == 0xcf08 || cmd == 0xcf09) {
        F<int32_t>(ui(), 0xc79) = cmd == 0xcf08 ? 0 : -1;
    } else if (cmd >= 0xcf6c && cmd <= 0xcf72) {
        F<uint32_t>(ui(), 0xc79) ^= 1u << (cmd - 0xcf6c);
    } else if (cmd == 0xcf76) {
        F<uint32_t>(ui(), 0xc79) ^= 0x1000;
    } else if (cmd == 0xcfd0) {
        g_6d3ff8 = g_6d3ff8 == 0;
    } else if (cmd >= 0xd28c && cmd <= 0xd295) {
        Engine_SetTotalTexMemOverride(eng(), 0, (int32_t)cmd - 0xd28c);
        Textures_Manage(1, 0);
    } else if (cmd >= 0xd2f0 && cmd <= 0xd2f9) {
        Engine_SetTexMemOverride3(eng(), 0, (int32_t)cmd - 0xd2f0);
        Textures_Manage(1, 0);
    } else if (cmd >= 0xd354 && cmd <= 0xd35d) {
        Engine_SetFreeTexMemOverride(eng(), 0, (int32_t)cmd - 0xd354);
    } else if (cmd == 0x9df9) {
        // The player's cash to zero.
        const int32_t back = Ftol_207(-F<double>(Obj_Ref25(game()), 0x154));
        Obj_AddDouble154(Obj_Ref25(game()), 0, back);
    } else if (cmd == 0x9dfa) {
        if (company()) {
            const double cash = Company_Stat((uint8_t*)company(), 0, 0xd, 0x2329);
            Company_PostStat(company(), 0, 4, (float)-cash, 1);
        }
    } else if (cmd == 0x9dfe) {
        // Set the company's cash to an entered amount.
        if (company()) {
            char buf[0x100];
            const double now = Company_Stat((uint8_t*)company(), 0, 0xd, 0x2329);
            Num_Format(buf, 8, now, 10, 1, 0, 0);
            Dialog_RunMessage(GetString(0xe33), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)buf, 0x1e, 0, nullptr, nullptr, 0,
                              0, nullptr, 0);
            const int32_t want = crt::atoi(buf);
            const double cash = Company_Stat((uint8_t*)company(), 0, 0xd, 0x2329);
            Company_PostStat(company(), 0, 4, (float)-cash, 1);
            Company_PostStat(company(), 0, 4, (float)want, 1);
        }
    } else if (cmd == 0x9dfd) {
        if (company()) Obj_ClearGrids(company());
    } else if (cmd >= 0x9dd1 && cmd <= 0x9dda) {
        static const int32_t kCash[10] = { 10000, 100000, 1000000, 10000000, 100000000, -10000, -100000, -1000000, -10000000, -100000000 };
        Obj_AddDouble154(Obj_Ref25(game()), 0, kCash[cmd - 0x9dd1]);
    } else if (cmd >= 0x9de5 && cmd <= 0x9de9) {
        static const uint32_t kGain[5] = { 0x461c4000, 0x47c35000, 0x49742400, 0x4b189680, 0x4cbebc20 };
        addStat(4, kGain[cmd - 0x9de5]);
    } else if (cmd >= 0x9dea && cmd <= 0x9dee) {
        static const uint32_t kLoss[5] = { 0xc61c4000, 0xc7c35000, 0xc9742400, 0xcb189680, 0xccbebc20 };
        addStat(0xc, kLoss[cmd - 0x9dea]);
    } else if ((cmd >= 0xe359 && cmd <= 0xe362) || cmd == 0x9dfb) {
        static const int32_t kCash[10] = { 10000, 100000, 1000000, 10000000, 100000000, -10000, -100000, -1000000, -10000000, -100000000 };
        Players_AdjustAiCash(cmd == 0x9dfb ? 0 : kCash[cmd - 0xe359]);
    } else if ((cmd >= 0xe36d && cmd <= 0xe376) || cmd == 0x9dfc) {
        static const int32_t kCash[10] = { 10000, 100000, 1000000, 10000000, 100000000, -10000, -100000, -1000000, -10000000, -100000000 };
        Companies_AdjustCash(cmd == 0x9dfc ? 0 : kCash[cmd - 0xe36d]);
    } else if (cmd == 0x9e27) {
        if (uint8_t* g = game()) {
            F<int32_t>(g, 0x4aaf) = 1;
            Collection* companies = g_companies;
            for (int32_t i = 0; i < Collection_Count(companies); ++i) {
                Collection* c = g_companies;
                F<int32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0)), 0x7680) = 0x32;
            }
        }
    } else if (cmd == 0x9e21) {
        // A new player, numbered after the existing ones.
        Collection* players = g_players;
        const uint32_t id = ((uint32_t(__thiscall*)(Collection*, int32_t, int32_t, int32_t, int32_t))(*(void***)players)[1])(players, 0, 0, 0, 0);
        uint8_t* p = (uint8_t*)Collection_Get(g_players, 0, id);
        const int32_t n = Collection_Count(g_players) - 1;
        char num[0x100], name[0x2bc], desc[0x100];
        const char* digits = Num_Format(num, 1, (double)n, 10, 1, 0, 0);
        FormatText(name, GetString(0x1bb), digits, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        digits = Num_Format(num, 1, (double)n, 10, 1, 0, 0);
        FormatText(desc, GetString(0x1bc), digits, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        Player_Init(p, 0, (int32_t)id, n, name, desc, n, 1);
    } else if (cmd == 0x9e22) {
        // Play as the next player.
        F<int32_t>(Obj_Ref25(game()), 0x1e1) = 0;
        const int32_t me = Obj_Get25(game());
        const uint32_t pos = Collection_PositionOf(g_players, 0, (uint32_t)me, 0);
        const int32_t n = Collection_Count(g_players);
        Obj_SetPlayer(game(), 0, Collection_IdAt(g_players, 0, (uint32_t)(((int32_t)pos + 1) % n), 0));
        F<int32_t>(Obj_Ref25(game()), 0x1e1) = 1;
    } else if (cmd == 0x9cc1) {
        if (Collection* news = g_news) {
            const char* text = GetString(0x1be);
            News_Add(news, 0, GetString(0x1bd), text, 0, 0);
        }
    } else if (cmd == 0x9cc2) {
        if (Collection* log = g_messageLog) Log_Add(log, 0, GetString(0x1bf), 4, 0, 0);
    } else if (cmd >= 0x84 && cmd <= 0x86) {
        F<int32_t>(eng(), 0x11420a) = (int32_t)cmd - 0x84;
    } else if (cmd >= 0x9f60 && cmd <= 0x9f77) {
        // Scale the heights (mode by group of eight).
        static const uint32_t kScale[8] = { 0x3d4ccccd, 0x3dcccccd, 0x3e800000, 0x3f000000, 0xbd4ccccd, 0xbdcccccd, 0xbe800000, 0xbf000000 };
        if (uint8_t* w = map()) Map_RescaleHeights(w, 0, BitsF(kScale[(cmd - 0x9f60) % 8]), (int32_t)(cmd - 0x9f60) / 8);
    } else {
        // The plain toggles.
        static const struct { uint16_t cmd; uint8_t engine, byte; uint32_t off; } kToggles[] = {
            { 0x9c42, 1, 1, 0x1146a3 }, { 0x9c63, 1, 0, 0x11469c }, { 0x9c5f, 1, 0, 0x114716 }, { 0x9cce, 1, 0, 0x114732 },
            { 0x9ccd, 0, 0, 0x28f }, { 0x9cd3, 0, 0, 0x2b7 }, { 0x9c60, 0, 0, 0x74 }, { 0x9c70, 0, 0, 0x78 }, { 0x9c61, 0, 0, 0x7c },
            { 0x9c98, 1, 1, 0x1146bd }, { 0x9c99, 0, 0, 0xf0 }, { 0x9cc7, 0, 0, 0x257 }, { 0x9cc8, 0, 0, 0x25b }, { 0x9c90, 0, 0, 0x94 },
            { 0x9c92, 0, 0, 0x98 }, { 0x9cb2, 0, 0, 0x144 }, { 0x9c91, 0, 0, 0x9c }, { 0x9cc4, 0, 0, 0x1db }, { 0x9c93, 0, 0, 0xa8 },
            { 0x9c8d, 0, 0, 0x8c }, { 0x9ca0, 0, 0, 0x130 }, { 0x9cd4, 0, 0, 0x2c8 }, { 0x9cc6, 0, 0, 0x24f }, { 0x9c8e, 0, 0, 0x90 },
            { 0x9cb6, 0, 0, 0x168 }, { 0x9ca1, 0, 0, 0x134 }, { 0x9c9f, 0, 0, 0x1bf }, { 0x9cbb, 0, 0, 0x17c }, { 0x9cbc, 0, 0, 0x180 },
            { 0x9ccb, 0, 0, 0x267 }, { 0x9cd5, 0, 1, 0x2d0 }, { 0x9cb3, 0, 0, 0x148 }, { 0x9cb8, 0, 0, 0x170 }, { 0x9cb4, 0, 0, 0x14c },
            { 0x9cbd, 0, 0, 0x188 }, { 0x9cb7, 0, 0, 0x16c }, { 0x9cb9, 0, 0, 0x174 }, { 0x9c9d, 0, 0, 0x110 }, { 0x9c49, 1, 1, 0x1146ab },
            { 0x9c57, 1, 1, 0x1146a2 }, { 0x9c4a, 1, 1, 0x1146ac }, { 0x9c50, 1, 1, 0x1146ad }, { 0x9c72, 1, 0, 0x114212 },
            { 0x9c51, 1, 1, 0x1146ae }, { 0x9c62, 1, 1, 0x1146e4 }, { 0x9c4c, 1, 1, 0x1146a5 }, { 0x9c4e, 1, 1, 0x1146a6 },
            { 0x9c4f, 1, 1, 0x1146a7 }, { 0x9c79, 1, 1, 0x1146e5 }, { 0x9c52, 1, 1, 0x1146e1 }, { 0x9c54, 1, 1, 0x1146eb },
            { 0x9c9c, 1, 1, 0x1146e9 }, { 0x8a, 1, 1, 0x1146ed }, { 0x8b, 1, 1, 0x1146ea }, { 0x9c56, 1, 1, 0x1146a8 },
            { 0x9c7a, 1, 1, 0x1146b7 }, { 0x9c6e, 1, 1, 0x1146e2 }, { 0x9c6d, 1, 1, 0x1146e3 }, { 0x9c6f, 1, 1, 0x1146a4 },
            { 0x9c71, 1, 1, 0x1146d7 }, { 0x9c9b, 0, 0, 0x108 }, { 0x9c9a, 0, 0, 0x104 }, { 0x9c9e, 1, 1, 0x1146d8 },
            { 0x9ca2, 1, 1, 0x1146e6 }, { 0x9c7c, 1, 1, 0x1146b8 }, { 0x9c7e, 1, 1, 0x1146e7 }, { 0x9c7f, 1, 1, 0x1146e8 },
            { 0x87, 1, 1, 0x1146ef }, { 0x88, 1, 1, 0x1146f0 }, { 0x9c80, 1, 1, 0x1146a9 }, { 0x9e23, 0, 0, 0xfc }, { 0x9e24, 0, 0, 0x1b3 },
            { 0x9e25, 0, 0, 0x287 },
        };
        for (const auto& t : kToggles)
            if (t.cmd == cmd) {
                uint8_t* o = t.engine ? eng() : app();
                if (t.byte)
                    flipB(o, t.off);
                else
                    flip(o, t.off);
                break;
            }
        if (cmd == 0x9e26) g_62be84 = g_62be84 == 0;
    }
    Options_Save(app(), 0, 1);
    Menu_UpdateChecks(m, 0, 1);
}
RT3_REPLACE(0x464410, DevMenu_Command);

// 0x4675a0. Ages a cargo load (+0x11): on a train it ripens up to its
// cargo's limit (0x5f2adc) in season 2; waiting at a building it decays
// unless the site still wants it. 1 when it expired (flag 1 of +0x12).
int32_t __fastcall Load_Age(uint8_t* l) {
    uint8_t* w = g_world;
    if (F<uint16_t>(l, 8)) {
        int32_t limit = ((const int32_t*)0x5f2adc)[l[0x10]];
        if ((int8_t)l[0x11] < limit && w[0x10] == 2) l[0x11]++;
        return 0;
    }
    l[0x11]--;
    if (l[0x10] == 1 && (w[0x10] & 1)) {
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, F<uint16_t>(l, 0xa));
        if (b) {
            if (((int32_t(__thiscall*)(void*))(*(void***)b)[0x70 / 4])(b)) {
                if (Obj_Array590(Obj_Ref2a8(b), 0, 3) > 0) l[0x11]++;
            } else {
                uint8_t* r = Obj_Region(b);
                for (int32_t i = 0; i < F<int32_t>(r, 0xeb); ++i) {
                    void* s = Collection_Get(g_stations, 0, F<uint16_t>(F<uint8_t*>(r, 0xef), i * 6));
                    if (Obj_Array590(s, 0, 3) > 0) {
                        l[0x11]++;
                        break;
                    }
                }
            }
        }
    }
    if ((int8_t)l[0x11] > 0) return 0;
    l[0x12] |= 1;
    return 1;
}
RT3_REPLACE(0x4675a0, Load_Age);

// 0x4676a0. How much of the cargo waiting at the station (o: building +0xe, cargo
// +0x10, amount +0x18, rate +0x13) the train picks up for the route from `from` to
// `to`: 0 unless one of the region's stations on the route is closer to the cargo's
// destination than `from`; the share is the distance ratio towards that station
// times (1 - waiting / k), capped. Scaled by the difficulty table for city cargos.
// With `apply` records the pickup (+0x17) and adds it to the amount.
int32_t __fastcall Station_LoadShare(uint8_t* o, int, uint8_t* from, uint8_t* to, uint8_t* city, float* out, int32_t apply) {
    uint8_t* reg = Obj_RegionAt(Collection_Get(g_buildings, 0, F<uint16_t>(o, 0xe)));
    *out = 0.0f;
    float bestFrom = 4.83e8f, bestTo = 4.83e8f;
    uint8_t* via = nullptr;
    if (F<int32_t>(reg, 0xeb) <= 0) return 0;
    for (int32_t i = 0; i < F<int32_t>(reg, 0xeb); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(F<uint8_t*>(reg, 0xef), i * 6));
        if (Obj_Flag3d4(Obj_Building(s)) != 1) continue;
        float d;
        if (Obj_ListEntry(from, 0, F<int32_t>(s, 0), o[0x10], (int32_t*)&d) && d < bestFrom) bestFrom = d;
        if (s == to) {
            bestTo = 0.0f;
            via = s;
        } else if (Obj_ListEntry(to, 0, F<int32_t>(s, 0), o[0x10], (int32_t*)&d) && d < bestTo) {
            bestTo = d;
            via = s;
        }
    }
    if (!(bestTo < bestFrom)) return 0;
    float ratio;
    if (via == to) ratio = 1.0f;
    else {
        int32_t d1 = (uint16_t)DistanceInt(Ftol(Obj_NodeCenterX(from)), Ftol(Obj_NodeCenterY(from)), Ftol(Obj_NodeCenterX(to)),
                                           Ftol(Obj_NodeCenterY(to)));
        int32_t d2 = (uint16_t)DistanceInt(Ftol(Obj_NodeCenterX(to)), Ftol(Obj_NodeCenterY(to)), Ftol(Obj_NodeCenterX(via)),
                                           Ftol(Obj_NodeCenterY(via)));
        if (d1 < 1) d1 = 1;
        if (d2 < 1) d2 = 1;
        ratio = (float)((double)(float)d1 / (float)(d2 + d1));
    }
    int32_t have = o[0x18];
    float q = (float)((double)have / D(0x5ca110));
    if (q > D(0x5c8650)) q = (float)0.95;
    float f = (float)((D(0x5c8770) - q) * ratio);
    double fk = (double)f * Fl(0x5ca0ec);
    int32_t n;
    __asm {
        fld fk
        mov eax, 0x5a10d0
        call eax
        mov n, eax
    }
    if (n < 1) n = 1;
    if (have + n > 0xff) {
        n = 0xff - have;
        if (n < 1) n = 1;
    }
    *out = (float)((double)n / D(0x5ca110) * F<float>(o, 0x13));
    if (!o[0x10] && city) {
        uint8_t* p = (uint8_t*)Obj_CityRef(city);
        if (p) {
            const float* tab = (const float*)(Game_EffectiveVersion() >= 0x67 ? 0x5f349c : 0x5f348c);
            *out = tab[F<int32_t>(p, 0x14)] * *out;
        }
        if (city[0x1a1]) *out = (float)(*out * D(0x5c8698));
    }
    if (apply) {
        o[0x17] = (uint8_t)n;
        int32_t t = o[0x18] + (uint8_t)n;
        o[0x18] = (uint8_t)(t > 0xff ? 0xff : t);
    }
    return 1;
}
RT3_REPLACE(0x4676a0, Station_LoadShare);

// 0x467a10. For a non-city cargo picks a random list of the route's buildings by the
// weights at 0x5f2ae8[kind] and charges the upkeep of its cheapest active building
// (cost +4 + k, times a random 1..1000), if under the limit.
RT3_NOINLINE void __fastcall Route_ChargeRandomBuilding(uint8_t* o, int, uint8_t* r, int32_t kind) {
    if (o[0x10]) return;
    if (!Pairs_Total(r)) return;
    float v = (float)((double)SimRand(1, 0x63) / D(0x5c85d8));
    const float* w = (const float*)0x5f2ae8 + kind;
    int32_t list = 0;
    for (;; ++list, w += 3) {
        if (list >= 5) return;
        v = (float)((double)v - *w);
        if (v < Fl(0x5c8598)) break;
    }
    if (!Obj_Array590(r, 0, list)) return;
    float best = 9999999.0f;
    int32_t pick = -1;
    Collection* blds = g_buildings;
    if (Obj_Array590(r, 0, list) == 1) {
        if (Obj_Flag3d4(Collection_Get(blds, 0, F<uint32_t>(Pairs_At(r, 0, list, 1), 0))) == 1) pick = 0;
        float c = (float)((double)F<float>(Pairs_At(r, 0, list, 1), 4) + Fl(0x5c8900));
        float rnd = (float)SimRand(1, 1000);
        best = (float)((double)c * rnd);
    } else {
        if (Obj_Array590(r, 0, list) <= 0) return;
        for (int32_t j = 0; j < Obj_Array590(r, 0, list); ++j) {
            if (Obj_Flag3d4(Collection_Get(g_buildings, 0, F<uint32_t>(Pairs_At(r, 0, list, j + 1), 0))) != 1)
                continue;
            float c = (float)((double)F<float>(Pairs_At(r, 0, list, j + 1), 4) + Fl(0x5c8900));
            float rnd = (float)SimRand(1, 1000);
            float s = (float)((double)c * rnd);
            if (s < best) {
                best = s;
                pick = j;
            }
        }
    }
    if (pick == -1 || !(best < Fl(0x5c85b0))) return;
    Building_ChargeUpkeep((uint8_t*)Collection_Get(g_buildings, 0,
                                                   F<uint32_t>(Pairs_At(r, 0, list, pick + 1), 0)));
}
RT3_REPLACE(0x467a10, Route_ChargeRandomBuilding);

// 0x467c30. Recomputes whether a cargo lot's source and destination buildings
// (+0xa/+0xe) have track-connected ready stations (+0x12 bit 1); when that flips,
// the source region's demand for its cargo class moves by +-0.2.
RT3_NOINLINE void __fastcall Lot_UpdateLinked(uint8_t* o, int, int32_t) {
    if (F<uint16_t>(o, 8)) return;
    void* b1 = Collection_Get(g_buildings, 0, F<uint16_t>(o, 0xa));
    if (!b1) return;
    void* b2 = Collection_Get(g_buildings, 0, F<uint16_t>(o, 0xe));
    if (!b2) return;
    int32_t old = (o[0x12] >> 1) & 1;
    o[0x12] &= 0xfd;
    uint8_t* r1 = Obj_Region(b1);
    uint8_t* r2 = Obj_Region(b2);
    for (int32_t i = 0; i < F<int32_t>(r1, 0xeb); ++i) {
        uint8_t* s = (uint8_t*)Collection_Get(Stations(), 0, F<uint16_t>(F<uint8_t*>(r1, 0xef), i * 6));
        if (Obj_Flag3d4(Obj_Building(s)) != 1) continue;
        for (int32_t j = 0; j < F<int32_t>(r2, 0xeb); ++j) {
            uint16_t id2 = F<uint16_t>(F<uint8_t*>(r2, 0xef), j * 6);
            if (F<uint16_t>(F<uint8_t*>(r1, 0xef), i * 6) == id2) continue;
            if (Obj_Flag3d4(Obj_Building(Collection_Get(Stations(), 0, id2))) != 1) continue;
            if (Station_ConnectedTo(s, 0, F<uint16_t>(F<uint8_t*>(r2, 0xef), j * 6))) o[0x12] |= 2;
        }
    }
    int32_t now = (o[0x12] >> 1) & 1;
    if (old == now) return;
    float d = (float)(o[0x12] & 2 ? 0.2 : -0.2);
    Obj_AddFloat103(r1, 0, F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x8e + o[0x10] * 4), d);
}
RT3_REPLACE(0x467c30, Lot_UpdateLinked);

// 0x467df0. Unlinks a cargo load (ID +0, next +4) from its holder: a train
// (+8, list head +0x29) or a building (+0xa, head +0x272; for flag 2 of
// +0x12 the cargo +0x10 is also returned to the region stock, -0.2).
RT3_NOINLINE void __fastcall Load_Unlink(uint32_t* o) {
    uint32_t* head;
    if (uint16_t train = F<uint16_t>(o, 8)) {
        head = (uint32_t*)((uint8_t*)Collection_Get(g_trains, 0, train) + 0x29);
    } else if (uint16_t b = F<uint16_t>(o, 0xa)) {
        uint8_t* bld = (uint8_t*)Collection_Get(g_buildings, 0, b);
        if (F<uint8_t>(o, 0x12) & 2) {
            uint8_t* ct = hook::As<uint8_t*>(g_cargoTypes);
            Obj_AddFloat103(Obj_RegionAt(bld), 0, F<int32_t>(ct, 0x8e + F<uint8_t>(o, 0x10) * 4), -0.2f);
        }
        head = (uint32_t*)(bld + 0x272);
    } else {
        return;
    }
    if (*head == o[0]) {
        *head = o[1];
    } else {
        uint32_t cur = *head;
        uint8_t* r;
        do {
            r = (uint8_t*)Collection_Get(g_6ada90, 0, cur);
            cur = F<uint32_t>(r, 4);
        } while (cur != o[0]);
        F<uint32_t>(r, 4) = o[1];
    }
    F<uint16_t>(o, 0xa) = 0;
    F<uint16_t>(o, 8) = 0;
}
RT3_REPLACE(0x467df0, Load_Unlink);

// 0x467eb0. Moves the lot to building id (head of its +0x272 lot chain), refreshes
// its link flag and, if linked, raises the building region's demand for its class.
RT3_NOINLINE void __fastcall Lot_AttachTo(uint32_t* o, int, uint32_t id) {
    Load_Unlink(o);
    uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
    F<uint16_t>(o, 0xa) = (uint16_t)id;
    o[1] = F<uint32_t>(b, 0x272);
    F<uint32_t>(b, 0x272) = o[0];
    Lot_UpdateLinked((uint8_t*)o, 0, 0);
    if (!(((uint8_t*)o)[0x12] & 2)) return;
    int32_t cls = F<int32_t>(Cargos(), 0x8e + ((uint8_t*)o)[0x10] * 4);
    Obj_AddFloat103(Obj_RegionAt(b), 0, cls, 0.2f);
}
RT3_REPLACE(0x467eb0, Lot_AttachTo);

// 0x467f20. Moves a cargo load onto train `id` (front of its list +0x29).
void __fastcall Load_AttachToTrain(uint32_t* load, int, uint32_t id) {
    Load_Unlink(load);
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, id);
    F<uint16_t>(load, 8) = (uint16_t)id;
    load[1] = F<uint32_t>(t, 0x29);
    F<uint32_t>(t, 0x29) = load[0];
}
RT3_REPLACE(0x467f20, Load_AttachToTrain);

// 0x467f50. Creates a cargo lot of the given kind at building `src` bound for
// `dst`: its rate (+0x13) grows with the distance (world factor +0x7d per kind, plus
// k, times the kind's table 0x5f2ad0); counts it in the cargo type's totals.
void __fastcall Lot_Create(uint32_t src, uint32_t dst, int32_t kind) {
    uint8_t lot[0x19];
    memset(lot, 0, sizeof lot);
    F<uint16_t>(lot, 0xa) = 0;
    F<uint32_t>(lot, 4) = 0;
    lot[0x11] = ((const uint8_t*)0x5f2adc)[kind * 4];
    uint8_t flags = lot[0x12] & 0xfc;
    F<uint16_t>(lot, 0xc) = (uint16_t)src;
    F<uint16_t>(lot, 0xe) = (uint16_t)dst;
    lot[0x10] = (uint8_t)kind;
    lot[0x17] = 0;
    lot[0x18] = 0;
    lot[0x12] = flags;
    void* b1 = Collection_Get(g_buildings, 0, src);
    void* b2 = Collection_Get(g_buildings, 0, dst);
    int32_t y2 = Ftol_174(Obj_CellY(b2)), x2 = Ftol_174(Obj_CellX(b2));
    int32_t y1 = Ftol_174(Obj_CellY(b1)), x1 = Ftol_174(Obj_CellX(b1));
    int32_t d = (uint16_t)DistanceInt(x1, y1, x2, y2);
    float k = F<float>(g_world, 0x7d + kind * 4);
    float rate = (float)((double)(float)d * k + Fl(0x5c86a0));
    F<float>(lot, 0x13) = (float)((double)rate * ((const float*)0x5f2ad0)[kind]);
    Collection* lots = g_6ada90;
    typedef uint32_t(__thiscall * AddFn)(Collection*, void*, int32_t, int32_t, int32_t);
    uint32_t id = (*(AddFn*)(*(uint8_t**)lots + 4))(lots, lot, 0, 0, 0);
    uint32_t* o = (uint32_t*)Collection_Get(g_6ada90, 0, id);
    o[0] = id;
    Lot_AttachTo(o, 0, src);
    uint8_t* ct = (uint8_t*)Collection_Get(g_cargoTypes, 0,
                                           F<uint32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x8e + kind * 4));
    F<float>(ct, 0x5a) = (float)(F<float>(ct, 0x5a) + D(0x5c8c20));
    F<float>(ct, 0x5e) = (float)(F<float>(ct, 0x5e) + D(0x5c8c20));
    Lot_UpdateLinked((uint8_t*)o, 0, 1);
}
RT3_REPLACE(0x467f50, Lot_Create);

}  // namespace rt3
