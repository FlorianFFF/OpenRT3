// Trains.cpp - RT3.exe 0x4b2000-0x4b6000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/stream.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall Train_Step(uint8_t* t, int, float dt, int32_t force);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Obj_ForCityChain(void* o, int, int32_t a, int32_t b, int32_t c);
void __fastcall Array33_Save(uint32_t* o, int, Stream s);
void __fastcall Train_Clear1a3(uint8_t* t);
void __fastcall Collection_ReleaseAll(Collection* c);
void __fastcall Collection_ReleaseAll2(Collection* c);
void __fastcall Nop4a7350(void*);
double __fastcall Obj_Float2c0(void* o, int, int32_t i);
double __fastcall Obj_Float2e8(void* o, int, int32_t i);
double __fastcall Obj_Float310(void* o, int, int32_t i);
double __fastcall Obj_Float338(void* o, int, int32_t i);
double __fastcall Obj_Float360(void* o, int, int32_t i);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
float __fastcall Obj_YearlyRatio(uint8_t* o);
uint32_t __stdcall HeatColor(float t);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, int32_t s, int32_t recordSize);
int32_t __fastcall Obj_BuildingStateActive(void* o);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
void* __fastcall Obj_CityRef(void* o);
double __fastcall Building_Total2e8(uint8_t* b);
double __fastcall Building_Total2c0(uint8_t* b);
double __fastcall Building_Total310(uint8_t* b);
double __fastcall Building_Total338(uint8_t* b);
double __fastcall Building_Total360(uint8_t* b);
double __fastcall Building_TotalRemaining(uint8_t* b);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void* __fastcall Train_Company(void* t);
void* Get6d3ae8();
void __fastcall TrainList_Refill(uint8_t* w);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Obj_Get21(void* o);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void __fastcall String_Assign(char** s, const char* src);
void* __fastcall Obj_Ref3cc(void* o);
uint32_t __fastcall Obj_TypeQuery(void* o, int, int32_t a);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t __fastcall Widget_IsVisible(uint8_t* w);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
const char* __fastcall LocalizeName(const char* name);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Widget_SetVtbl(uint8_t* w);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t IsMultiplayer();
void* __fastcall Array33_Current(void* a);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
void* __fastcall Obj_City41(void* o);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
void __fastcall Route_LinkStations(uint8_t* o);
void __fastcall Table_MoveRef(Collection* t, int, uint32_t from, uint32_t to);
void __fastcall TrainOrders_RefreshOwner(uint8_t* o);
uint8_t* __fastcall Train_FirstCarRegion(uint8_t* t);
uint8_t* __fastcall TrainOrders_NextStop(uint8_t* list, int, int32_t move, int32_t skipCurrent);
void __fastcall Train_RetargetRegion(uint8_t* t);
int32_t __fastcall Tutorial_OnBuildingPlaced(int32_t id);
int32_t __fastcall Train_StateIs7(uint8_t* t);
void __fastcall Train_StopAndGhost(uint8_t* train);
void* __fastcall Identity(void* o);
void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
void __fastcall Overview_Close(uint8_t* t);
float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall Industry_DrawProduction(uint8_t* o, int, float x, float y, float w, int32_t, int32_t);
void __fastcall Obj_SetHighlight(uint8_t* o, int, int32_t v);
uint32_t __fastcall Trains_Create(Collection* c, int, int32_t owner);
void __fastcall Train_PlaceInitialEngine(uint8_t* t, int, uint32_t typeId, uint8_t* node);
void __fastcall Train_Route(uint8_t* t);
int32_t __fastcall Train_OnOwnTrack(uint8_t* t);
int32_t __fastcall App_DispatchEvent5(uint8_t* app, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE int32_t __fastcall App_DispatchEvent5(uint8_t* app, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
RT3_NOINLINE int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
void __fastcall Train_SetGhost(uint8_t* train, int, int32_t ghost, int32_t force);
RT3_NOINLINE void __fastcall Train_RefreshWindowIfShown(uint8_t* t);
int32_t __fastcall Train_LoadExtra(uint8_t* train, int, int32_t st);
RT3_NOINLINE float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
RT3_NOINLINE int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
RT3_NOINLINE void __fastcall Train_RetargetRegion(uint8_t* t);
void __fastcall Train_Teardown(uint8_t* t);
RT3_NOINLINE void __fastcall Train_Route(uint8_t* t);
RT3_NOINLINE void __fastcall Train_DropStation(uint8_t* t, int, int32_t station);
RT3_NOINLINE void __fastcall Train_DropWaypoint(uint8_t* t, int, int32_t node);
RT3_NOINLINE uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company);
RT3_NOINLINE Collection* __fastcall Collection5d00c8_Construct(Collection* c);
void __fastcall Collection_ReleaseAll2(Collection* c);
RT3_NOINLINE void __fastcall Collection5d00c8_Destruct(Collection* c);
void __fastcall Trains_Remove(Collection* c, int, uint32_t id);
void __fastcall Trains_Advance(Collection* c, int, float dt);
RT3_NOINLINE int32_t __fastcall Trains_ResolveClash(Collection* objs, int, uint32_t a, uint32_t b);
RT3_NOINLINE int32_t __fastcall Trains_Load(Collection* c, int, int32_t st, int32_t recordSize);
RT3_NOINLINE void __fastcall Trains_Save(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Trains_DropStation(Collection* trains, int, int32_t station);
RT3_NOINLINE void __fastcall Trains_DropWaypoint(Collection* trains, int, int32_t node);
void __fastcall Trains_Clear1a3(Collection* t);
void __fastcall Trains_UpdateRatioColors(Collection* c);
void __fastcall Trains_ColorOwner(Collection* c, int, int32_t company);
void __fastcall Table_ForAllCityChains(Collection* t);
void __fastcall Table_Hook4a7350(Collection* t);
RT3_NOINLINE void __fastcall Trains_ReapplyGhosts(Collection* c);
RT3_NOINLINE void __fastcall OrderList_Init(uint8_t* o);
RT3_NOINLINE void* __fastcall Array33_At(void* a, int, int32_t i);
RT3_NOINLINE void* __fastcall Identity(void* o);
RT3_NOINLINE int32_t __fastcall OrderList_CountPlain(uint8_t* o);
RT3_NOINLINE void* __fastcall Cursor33_Init(uint8_t* c);
RT3_NOINLINE void* __fastcall Array33_Current(void* a);
uint8_t* __fastcall OrderList_Current(uint8_t* o);
RT3_NOINLINE uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric, int32_t companyId);
RT3_NOINLINE void __fastcall TrainOrders_RefreshOwner(uint8_t* o);
RT3_NOINLINE void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark);
RT3_NOINLINE void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx);
RT3_NOINLINE void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
RT3_NOINLINE uint8_t* __fastcall TrainOrders_NextStop(uint8_t* list, int, int32_t move, int32_t skipCurrent);
RT3_NOINLINE int32_t __fastcall Obj_NameTableIndex(uint8_t* o);
void __fastcall IndustryWindow_Destruct(uint8_t* w);
int32_t __fastcall Return0_4b35a0(void*);
RT3_NOINLINE double __fastcall Building_Total2c0(uint8_t* b);
RT3_NOINLINE double __fastcall Building_Total2e8(uint8_t* b);
RT3_NOINLINE double __fastcall Building_Total310(uint8_t* b);
RT3_NOINLINE double __fastcall Building_Total338(uint8_t* b);
RT3_NOINLINE double __fastcall Building_Total360(uint8_t* b);
RT3_NOINLINE double __fastcall Building_TotalRemaining(uint8_t* b);
void __fastcall CargoList_ClickCb(int32_t id, int, int32_t down, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall BuildingView_DrawIncomeTable(uint8_t* b, int32_t kind, float x, float y, float, float, int32_t);
RT3_NOINLINE void __fastcall BuildingPanel_DrawLine(uint8_t* b, int32_t kind, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __stdcall View_DrawSelectionLabel(float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall Industry_DrawProduction(uint8_t* o, int, float x, float y, float w, int32_t, int32_t);
RT3_NOINLINE void __fastcall TrainList_Refill(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Tutorial_OnBuildingPlaced(int32_t id);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

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

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }


static inline double Fl_207(uint32_t a) { return *(const float*)a; }

static inline float Prod_Bits(uint32_t b) { return *(const float*)&b; }

static inline uint8_t* Prod_Win() { return g_industryWindow; }

static inline void Prod_Text(const char* text, float x, float y, float w, int32_t align) {
    Font_DrawTextBox(g_6cfcf8, 0, text, x, y, w, Prod_Bits(0x3ccccccd), 0xffffffff, align, 0, 0, 1.0f);
}

static inline char* Prod_Format(char* dst, const char* fmt, const char* a1, const char* a2) { return FormatText(dst, fmt, a1, a2, 0, 0, 0, 0, 0, 0); }

static inline char* Prod_Num(char* buf, int32_t style, double v) { return Num_Format(buf, style, v, 10, 1, 0, 0); }

// div() of the exe's CRT: quotient in the low, remainder in the high half.
static inline int32_t Prod_Rem(int32_t a, int32_t b) { return (int32_t)(crt::div(a, b) >> 32); }

static inline bool Prod_KindIs(uint8_t* o, uint32_t name) { return !crt::stricmp((const char*)Obj_Ref3cc(o) + 4, (const char*)name); }

// A text label (0xf0 widget) of the production panel.
static void Prod_Label(uint32_t id, float x, float y, uint32_t h, uint8_t b27, uint8_t b33, const char* text) {
    uint8_t d[0x91];
    memset(d, 0, sizeof d);
    F<float>(d, 7) = x;
    F<float>(d, 0xb) = y;
    F<uint32_t>(d, 0xf) = h;
    F<uint32_t>(d, 0x13) = 0x3d75c28f;
    d[0x27] = b27;
    d[0x28] = 0;
    F<int32_t>(d, 0x34) = -1;
    String_Assign(&F<char*>(d, 0x77), text);
    d[0x33] = b33;
    F<uint16_t>(d, 0) = (uint16_t)(id + 0xbd42);
    uint8_t* e = (uint8_t*)crt::operator_new(0xf0);
    e = e ? EditBox_Construct(e, 0, d) : nullptr;
    Window_AddChild(Prod_Win(), 0, e, 0);
    crt::free(F<void*>(d, 0x77));
}

// A cargo picture (0x111 widget) from `imb` (the default one when the name
// table lacks it), captioned tip (or string tipId), drawn at `scale`.
static void Prod_Image(uint32_t id, float x, float y, const char* imb, const char* tip, int32_t tipId, uint32_t scale) {
    uint8_t d[0x89];
    memset(d, 0, sizeof d);
    F<float>(d, 7) = x;
    F<float>(d, 0xb) = y;
    F<uint32_t>(d, 0xf) = 0x3d4ccccd;
    F<uint32_t>(d, 0x13) = 0x3d75c28f;
    F<uint16_t>(d, 0) = (uint16_t)(id + 0xbd42);
    F<uint32_t>(d, 0x2f) = 0x800;
    F<uint16_t>(d, 0x5f) = 0;
    int32_t found = NameTable_ContainsName(g_pack, 0, imb, 0, nullptr);
    String_Assign(&F<char*>(d, 0x43), found ? imb : (const char*)0x5d0194);
    String_Assign(&F<char*>(d, 0x3f), tip ? tip : GetString(tipId));
    uint8_t* w = (uint8_t*)crt::operator_new(0x111);
    w = w ? Scrollbar_Construct(w, 0, d) : nullptr;
    F<uint32_t>(w, 0x100) = scale;
    F<uint32_t>(w, 0x104) = scale;
    Window_AddChild(Prod_Win(), 0, w, 0);
    crt::free(F<void*>(d, 0x43));
    crt::free(F<void*>(d, 0x3f));
}

// The hover box (0x111 widget, style 9, link `link`) showing string 0xb8e
// around `text`, hooked to the panel callback 0x4b3670.
static void Prod_Tip(uint32_t id, float x, float y, uint32_t h, uint32_t w13, uint32_t link, const char* text) {
    uint8_t d[0x89];
    memset(d, 0, sizeof d);
    F<float>(d, 7) = x;
    F<uint32_t>(d, 0xf) = h;
    F<float>(d, 0xb) = y;
    F<uint32_t>(d, 0x13) = w13;
    F<uint32_t>(d, 0x4b) = link;
    F<uint16_t>(d, 0) = (uint16_t)(id + 0xbd74);
    F<uint32_t>(d, 0x61) = 9;
    char buf[0x200];
    Prod_Format(buf, GetString(0xb8e), text, nullptr);
    String_Assign(&F<char*>(d, 0x3f), buf);
    uint8_t* t = (uint8_t*)crt::operator_new(0x111);
    t = t ? Scrollbar_Construct(t, 0, d) : nullptr;
    Widget_ClearFlags(t, 0, 1);
    Window_AddChild(Prod_Win(), 0, t, 0);
    crt::free(F<void*>(d, 0x3f));
    Window_SendEvent5(Prod_Win(), 0, 0x7b, F<uint16_t>(d, 0), 0x4b3670, 0, 0);
}

// The station of order entry i (0x33 bytes each; flag +0x28 bit 7 = a plain
// track node, station id +0x29), 0 for a node or a negative index.
static uint32_t TrainOrders_StationAt(const uint8_t* list, int32_t i) {
    if (i < 0) return 0;
    const uint8_t* e = F<uint8_t*>((void*)list, 0) + i * 0x33;
    if (!e || (int8_t)e[0x28] < 0) return 0;
    return F<uint16_t>((void*)e, 0x29);
}

// 0x4b2140. Creates a train of engine type `engine` for company `company`
// (-1: the player's) with orders (0x25 bytes, 0x33-byte stops; null for
// none), marking 0x6cfcb8 meanwhile: the current stop when its station is
// active, else the next stop from there with an active station (QUIRK: or a
// station that no longer exists); its node (+0x2b of a waypoint, else the
// station's +8, highlighted) or the first track node (0x6cfca8) places the
// engine; the orders are copied, routed and the train advanced five steps.
// 0 when there is no node.
RT3_NOINLINE uint32_t __fastcall Train_Create(uint32_t engine, uint8_t* orders, int32_t, int32_t company) {
    if (company == -1) company = Obj_Get21(g_world);
    g_6cfcb8 = 1;
    uint32_t node = 0;
    auto usable = [](uint8_t* e) {
        if ((int8_t)e[0x28] < 0) return 2; // a waypoint: never picked
        if (!Collection_IsValid(g_stations, 0, F<uint16_t>(e, 0x29))) return 0;
        return Obj_BuildingStateActive(Collection_Get(g_stations, 0, F<uint16_t>(e, 0x29))) ? 1 : 3;
    };
    if (orders) {
        const int32_t cur = F<int32_t>(orders, 8);
        bool search = false;
        if (cur < F<int32_t>(orders, 4)) {
            const int32_t u = usable(F<uint8_t*>(orders, 0) + cur * 0x33);
            search = u == 2 || u == 3;
        }
        if (search) {
            for (int32_t k = 0; k < F<int32_t>(orders, 4); ++k) {
                const int32_t idx = (F<int32_t>(orders, 8) + k) % F<int32_t>(orders, 4);
                const int32_t u = usable(F<uint8_t*>(orders, 0) + idx * 0x33);
                if (u == 0 || u == 1) {
                    F<int32_t>(orders, 8) = idx;
                    break;
                }
            }
        }
        if (uint8_t* stop = (uint8_t*)Array33_Current(orders)) {
            if ((int8_t)stop[0x28] < 0) {
                node = F<uint32_t>(stop, 0x2b);
            } else {
                uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>(stop, 0x29));
                node = F<uint32_t>(st, 8);
                Obj_SetHighlight(st, 0, 1);
            }
        }
    }
    if (!node && Collection_Count(g_tracks) > 0) node = Collection_IdAt(g_tracks, 0, 0, 0);
    if (!node) {
        g_6cfcb8 = 0;
        return 0;
    }
    const uint32_t id = Trains_Create(g_trains, 0, company);
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, id);
    Train_PlaceInitialEngine(t, 0, engine, (uint8_t*)Collection_Get(g_tracks, 0, node));
    F<uint32_t>(orders, 0xd) = F<uint32_t>(t, 0x25);
    memcpy(Identity(t), orders, 0x25);
    Train_Route(t);
    Route_LinkStations(t);
    for (int32_t k = 0; k < 5; ++k) ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t))0x4b10d0)(t, 0, 0, 1);
    g_6cfcb8 = 0;
    return id;
}
RT3_REPLACE(0x4b2140, Train_Create);

// 0x4b2340 / 0x4b2370. Collection subclass 0x5d00c8 (inline 0x1d5-byte records, 50 + 20, list 1).
RT3_NOINLINE Collection* __fastcall Collection5d00c8_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5d00c8;
    Collection_Init(c, 0, 1, 0x1d5, 0x32, 0x14, 1, 0, 0);
    return c;
}
RT3_REPLACE(0x4b2340, Collection5d00c8_Construct);

RT3_NOINLINE void __fastcall Collection_ReleaseAll2(Collection* c) { Collection_ReleaseAll(c); }
RT3_REPLACE(0x4b2370, Collection_ReleaseAll2);

RT3_NOINLINE void __fastcall Collection5d00c8_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5d00c8;
    Collection_ReleaseAll2(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x4b23b0, Collection5d00c8_Destruct);

// 0x4b2430 (train collection vtable). Removes train id: the camera stops
// following it when it follows its lead car (0x62be68 +0x60), it is torn
// down and freed, and the app hears of it (event 0x3e9).
void __fastcall Trains_Remove(Collection* c, int, uint32_t id) {
    uint8_t* t = (uint8_t*)Collection_Get(c, 0, id);
    if (Obj_City41(t) == F<void*>(g_62be68, 0x60)) World_SetFollow(g_world, 0, 0, nullptr);
    Train_Teardown(t);
    Collection_Release(c, 0, id);
    App_DispatchEvent5(g_app, 0, 0x3e9, 0xffffffff, id, 0, 0);
}
RT3_REPLACE(0x4b2430, Trains_Remove);

// 0x4b24a0 (thiscall on the train collection). Advances every train by dt
// (whole ms in multiplayer): first those on their own track, then the rest
// (+0x1be marks the done ones); a finished train (state 0xd) is removed
// (QUIRK: removing the first one skips the next).
void __fastcall Trains_Advance(Collection* c, int, float dt) {
    if (g_world && IsMultiplayer()) dt = (float)(int32_t)dt;
    for (int32_t pass = 0; pass < 2; ++pass) {
        for (int32_t i = 0; i < Collection_Count(c); ++i) {
            uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
            if (!t) continue;
            if (pass == 0) {
                if (!Train_OnOwnTrack(t)) {
                    t[0x1be] = 0;
                    continue;
                }
            } else if (t[0x1be]) {
                continue;
            }
            t[0x1be] = 1;
            Train_Step(t, 0, dt, 0);
            if (F<int32_t>(t, 0xe8) == 0xd) {
                ((void(__fastcall**)(Collection*, int, uint32_t))*(void***)c)[2](c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
                if (i >= 1) --i;
            }
        }
    }
}
RT3_REPLACE(0x4b24a0, Trains_Advance);

// 0x4b2580. Two trains a and b (ids in 0x6cfcbc; their objects in this table)
// meet: one is stopped and ghosted. Always 1 with the world flag
// 0x6cec74+0x1b3. Higher priority (+0xd8, -1 off their own company's track:
// track +0x49's owner +0x22e vs +0x51) wins; on a tie a train in state 7
// yields to one that is not, then the one carrying the bigger cargo rate
// (Train_CargoRate, when they differ by more than 0x5c87f8) wins, then the
// higher +0xcc (b on a tie). Returns 1 when b was stopped, 0 for a.
RT3_NOINLINE int32_t __fastcall Trains_ResolveClash(Collection* objs, int, uint32_t a, uint32_t b) {
    if (F<int32_t>(g_app, 0x1b3)) return 1;
    int32_t pa = F<int32_t>(Collection_Get(g_trains, 0, a), 0xd8);
    int32_t pb = F<int32_t>(Collection_Get(g_trains, 0, b), 0xd8);
    uint8_t* A = (uint8_t*)Collection_Get(objs, 0, a);
    uint8_t* B = (uint8_t*)Collection_Get(objs, 0, b);
    if ((int32_t)F<uint16_t>(Collection_Get(g_tracks, 0, F<uint32_t>(A, 0x49)), 0x22e) != F<int32_t>(A, 0x51)) pa = -1;
    if ((int32_t)F<uint16_t>(Collection_Get(g_tracks, 0, F<uint32_t>(B, 0x49)), 0x22e) != F<int32_t>(B, 0x51)) pb = -1;
    bool stopB;
    if (pa > pb) stopB = true;
    else if (pa < pb) stopB = false;
    else {
        if (Train_StateIs7(A) && !Train_StateIs7(B)) return 0;
        if (Train_StateIs7(B) && !Train_StateIs7(A)) return 1;
        float ra = Train_CargoRate(A, 0, 0);
        float rb = Train_CargoRate(B, 0, 0);
        if (fabs((double)ra - rb) > D(0x5c87f8)) stopB = !(rb > ra);
        else stopB = !(F<float>(A, 0xcc) < F<float>(B, 0xcc));
    }
    Train_StopAndGhost(stopB ? B : A);
    return stopB ? 1 : 0;
}
RT3_REPLACE(0x4b2580, Trains_ResolveClash);

// 0x4b2700. Loads the train table: tag 0x5209, the records, tag 0x520a, each
// train's extra block, tag 0x520b. Returns the bytes read.
RT3_NOINLINE int32_t __fastcall Trains_Load(Collection* c, int, int32_t st, int32_t recordSize) {
    int32_t n = StreamSkip4(st, 0x5209, 0, 0);
    n += Collection_Load(c, 0, st, recordSize);
    n += StreamSkip4(st, 0x520a, 0, 0);
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        n += Train_LoadExtra((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, st);
    return n + StreamSkip4(st, 0x520b, 0, 0);
}
RT3_REPLACE(0x4b2700, Trains_Load);

// 0x4b27a0. Trains save: tags 0x5209/0x520a/0x520b with each train's order array (0x4a7030).
RT3_NOINLINE void __fastcall Trains_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x5209);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x520a);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Array33_Save((uint32_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, s);
    StreamWriteU32(s, 0x520b);
}
RT3_REPLACE(0x4b27a0, Trains_Save);

// 0x4b2820. Train table: every train drops its orders for `station`.
RT3_NOINLINE void __fastcall Trains_DropStation(Collection* trains, int, int32_t station) {
    for (int32_t i = 0; i < Collection_Count(trains); ++i)
        Train_DropStation((uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, (uint32_t)i, 0)), 0, station);
}
RT3_REPLACE(0x4b2820, Trains_DropStation);

// 0x4b2860. Train table: every train drops its waypoint orders for `node`.
RT3_NOINLINE void __fastcall Trains_DropWaypoint(Collection* trains, int, int32_t node) {
    for (int32_t i = 0; i < Collection_Count(trains); ++i)
        Train_DropWaypoint((uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, (uint32_t)i, 0)), 0, node);
}
RT3_REPLACE(0x4b2860, Trains_DropWaypoint);

void __fastcall Trains_Clear1a3(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) Train_Clear1a3((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x4b28a0, Trains_Clear1a3);

// 0x4b28e0. Tints each train by its yearly ratio r (1 - ratio): red-ish channel full
// below 0.5 fading above, the other rising to full at 0.5 (alpha 0x33).
void __fastcall Trains_UpdateRatioColors(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        float r = (float)(D(0x5c8770) - Obj_YearlyRatio(e));
        float a = (float)(r < D(0x5c8628) ? 1.0 : D(0x5c8770) - ((double)r - D(0x5c8628) + ((double)r - D(0x5c8628))));
        float b = (float)(r > D(0x5c8628) ? 1.0 : (double)r + r);
        int32_t ia = Ftol((double)a * Fl(0x5ca0ec));
        int32_t ib = Ftol((double)b * Fl(0x5ca0ec));
        Obj_ForCityChain(e, 0, ib, ia, 0x33);
    }
}
RT3_REPLACE(0x4b28e0, Trains_UpdateRatioColors);

// 0x4b29e0. Colours the trains: hot for the company's, cold for the rest.
void __fastcall Trains_ColorOwner(Collection* c, int, int32_t company) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        uint32_t col = HeatColor((float)(F<int32_t>(e, 0x51) == company ? 1 : 0));
        Obj_ForCityChain(e, 0, col >> 16, col >> 8, col);
    }
}
RT3_REPLACE(0x4b29e0, Trains_ColorOwner);

// 0x4b2a50. Runs Obj_ForCityChain(0, 0, 0) for every record of the table.
void __fastcall Table_ForAllCityChains(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Obj_ForCityChain(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, 0, 0, 0);
}
RT3_REPLACE(0x4b2a50, Table_ForAllCityChains);

void __fastcall Table_Hook4a7350(Collection* t) {
    if (!F<int32_t>(g_app, 0x1cf)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i) Nop4a7350(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
}
RT3_REPLACE(0x4b2a90, Table_Hook4a7350);

// 0x4b2ae0. Re-applies the ghost state of every ghosted train (count from this
// collection, records from the train table 0x6cfcbc).
RT3_NOINLINE void __fastcall Trains_ReapplyGhosts(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        Collection* trains = g_trains;
        uint8_t* t = (uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, i, 0));
        if (t[0xf4]) Train_SetGhost(t, 0, 1, 1);
    }
}
RT3_REPLACE(0x4b2ae0, Trains_ReapplyGhosts);

// 0x4b2b30. Initialises an order list header.
RT3_NOINLINE void __fastcall OrderList_Init(uint8_t* o) {
    F<int32_t>(o, 0) = 0;
    F<int32_t>(o, 4) = 0;
    F<int32_t>(o, 8) = -1;
    F<int32_t>(o, 0xd) = 0;
    o[0xc] = 0;
    for (int k = 0; k < 5; ++k) F<int32_t>(o, 0x11 + k * 4) = 0;
}
RT3_REPLACE(0x4b2b30, OrderList_Init);

RT3_NOINLINE void* __fastcall Array33_At(void* a, int, int32_t i) { return F<uint8_t*>(a, 0) + i * 0x33; }
RT3_REPLACE(0x4b2b60, Array33_At);

RT3_NOINLINE void* __fastcall Identity(void* o) { return o; }
RT3_REPLACE(0x4b2b70, Identity);

// 0x4b2b80 / 0x4b2bf0. Order list (0x33-byte entries, count +4, cursor +8):
// count of unflagged orders (+0x28 >= 0); current order (0 if none).
RT3_NOINLINE int32_t __fastcall OrderList_CountPlain(uint8_t* o) {
    int32_t n = 0;
    const uint8_t* e = F<uint8_t*>(o, 0) + 0x28;
    for (int32_t i = 0; i < F<int32_t>(o, 4); ++i, e += 0x33)
        if ((int8_t)*e >= 0) ++n;
    return n;
}
RT3_REPLACE(0x4b2b80, OrderList_CountPlain);

// 0x4b2ba0. Initialises a 0x25-byte cursor record; returns it.
RT3_NOINLINE void* __fastcall Cursor33_Init(uint8_t* c) {
    F<int32_t>(c, 0) = 0;
    F<int32_t>(c, 4) = 0;
    F<int32_t>(c, 8) = -1;
    F<int32_t>(c, 0xd) = 0;
    F<uint8_t>(c, 0xc) = 0;
    for (int i = 0; i < 5; ++i) F<int32_t>(c, 0x11 + i * 4) = 0;
    return c;
}
RT3_REPLACE(0x4b2ba0, Cursor33_Init);

// 0x4b2bd0 / 0x4b2b60. Elements of a 0x33-byte record array {base, ?, current}.
RT3_NOINLINE void* __fastcall Array33_Current(void* a) {
    int32_t i = F<int32_t>(a, 8);
    return i >= 0 ? F<uint8_t*>(a, 0) + i * 0x33 : nullptr;
}
RT3_REPLACE(0x4b2bd0, Array33_Current);

uint8_t* __fastcall OrderList_Current(uint8_t* o) {
    int32_t i = F<int32_t>(o, 8);
    if (i < 0) return nullptr;
    if (i == 0) i = F<int32_t>(o, 4);
    return F<uint8_t*>(o, 0) + (i - 1) * 0x33;
}
RT3_REPLACE(0x4b2bf0, OrderList_Current);

// 0x4b2c10. Whether a train's order list (this: records of 0x33 bytes, count
// +4) is drivable: every order's node (+0x2b, or its station's node) reaches the
// next one (cyclically) and the first order reaches `start` (unless -1), on the
// company's track (company `companyId` if > 0 and valid, none for 0, else the
// train's), electrified when the engine is electric or `forceElectric`, not with
// `noElectric`; and at least two different active stations are visited.
RT3_NOINLINE uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric,
                                                int32_t companyId) {
    uint8_t ok = 1;
    uint32_t st1 = 0, st2 = 0;
    int32_t need = 0;
    uint32_t* co = nullptr;
    if (train) co = (uint32_t*)Train_Company(train);
    if (!companyId) co = nullptr;
    else if (companyId > 0 && Collection_IsValid(g_companies, 0, (uint32_t)companyId))
        co = (uint32_t*)Collection_Get(g_companies, 0, (uint32_t)companyId);
    if (train && F<int32_t>(Obj_CityRef(train), 0x10) == 2) need = 1;
    if (forceElectric) need = 1;
    if (noElectric) need = 0;
    Collection* stations = g_stations;
    Collection* nodes = g_tracks;
    for (int32_t i = 0; i < (int32_t)orders[1]; ++i) {
        uint8_t* base = (uint8_t*)orders[0];
        uint8_t* cur = base + i * 0x33;
        uint8_t* next = i == (int32_t)orders[1] - 1 ? base : base + i * 0x33 + 0x33;
        int32_t from;
        if ((int8_t)cur[0x28] < 0) from = F<int32_t>(cur, 0x2b);
        else {
            if (!Collection_IsValid(stations, 0, F<uint16_t>(cur, 0x29))) return 0;
            from = F<int32_t>(Collection_Get(stations, 0, F<uint16_t>(cur, 0x29)), 8);
        }
        if (!Collection_IsValid(nodes, 0, (uint32_t)from)) return 0;
        int32_t to;
        if ((int8_t)next[0x28] < 0) to = F<int32_t>(next, 0x2b);
        else {
            if (!Collection_IsValid(stations, 0, F<uint16_t>(next, 0x29))) return 0;
            uint8_t* st = (uint8_t*)Collection_Get(stations, 0, F<uint16_t>(next, 0x29));
            to = F<int32_t>(st, 8);
            if (Obj_BuildingStateActive(st)) {
                if (!st1) st1 = F<uint16_t>(next, 0x29);
                else if (st1 != F<uint16_t>(next, 0x29)) st2 = F<uint16_t>(next, 0x29);
            }
        }
        if (!Collection_IsValid(nodes, 0, (uint32_t)to)) return 0;
        uint32_t cost;
        Route_NextStep(g_routes, 0, from, to, &cost, need, co, nullptr, 0);
        if (cost == 0xffffffff) {
            ok = 0;
            goto done;
        }
    }
    if (start != -1 && (int32_t)orders[1] > 0) {
        uint8_t* first = (uint8_t*)orders[0];
        int32_t from;
        if ((int8_t)first[0x28] < 0) from = F<int32_t>(first, 0x2b);
        else {
            if (!Collection_IsValid(stations, 0, F<uint16_t>(first, 0x29))) return 0;
            from = F<int32_t>(Collection_Get(stations, 0, F<uint16_t>(first, 0x29)), 8);
        }
        if (!Collection_IsValid(nodes, 0, (uint32_t)from) || !Collection_IsValid(nodes, 0, (uint32_t)start)) return 0;
        uint32_t cost;
        Route_NextStep(g_routes, 0, from, start, &cost, need, co, nullptr, 0);
        if (cost == 0xffffffff) ok = 0;
    }
done:
    if (!st1 || !st2) ok = 0;
    return ok;
}
RT3_REPLACE(0x4b2c10, Orders_Drivable);

// 0x4b2ea0. Refreshes what shows this train order list's train (+0xd): the
// train's window, or without a train the train list 0x6d3ac8 when it is this.
RT3_NOINLINE void __fastcall TrainOrders_RefreshOwner(uint8_t* o) {
    Collection* trains = g_trains;
    if (F<uint32_t>(o, 0xd) && Collection_Get(trains, 0, F<uint32_t>(o, 0xd))) {
        Train_RefreshWindowIfShown((uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(o, 0xd)));
        return;
    }
    uint8_t* list = g_6d3ac8;
    if (list && Get6d3ae8() == o) TrainList_Refill(g_6d3ac8);
}
RT3_REPLACE(0x4b2ea0, TrainOrders_RefreshOwner);

// 0x4b2f00. Train order list (+0 entries, +4 count, +8 current, +0xd the
// train in 0x6cfcbc): makes entry `idx` (0 past the end) current, moving the
// stations' train references; for a live train (marked +0x1b9 with `mark`)
// it re-targets the region and re-routes. Then the owner display refreshes.
RT3_NOINLINE void __fastcall TrainOrders_SetCurrent(uint8_t* list, int, int32_t idx, int32_t mark) {
    if (idx >= F<int32_t>(list, 4)) idx = 0;
    if (F<int32_t>(list, 8) == idx) return;
    uint32_t from = TrainOrders_StationAt(list, F<int32_t>(list, 8));
    F<int32_t>(list, 8) = idx;
    uint32_t to = TrainOrders_StationAt(list, idx);
    Table_MoveRef(g_stations, 0, from, to);
    auto train = [&]() -> uint8_t* {
        return F<uint32_t>(list, 0xd) ? (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(list, 0xd)) : nullptr;
    };
    if (F<uint32_t>(list, 0xd) && Collection_Get(g_trains, 0, F<uint32_t>(list, 0xd))) {
        if (mark) train()[0x1b9] = 1;
        Train_RetargetRegion(train());
        Train_Route(train());
    }
    TrainOrders_RefreshOwner(list);
}
RT3_REPLACE(0x4b2f00, TrainOrders_SetCurrent);

// 0x4b3000. Train order list: removes entry idx (ignored out of range). When
// it was current the list first moves to the following entry (marking the
// train) - QUIRK: only when the current index is above 1, not the count - and
// the current index shifts down with it (0 when it falls below 0 while more
// than one order is left). A live train re-targets, and re-routes when the
// current entry was removed.
RT3_NOINLINE void __fastcall TrainOrders_Remove(uint8_t* list, int, int32_t idx) {
    int32_t n = F<int32_t>(list, 4);
    if (idx >= n || idx < 0) return;
    int32_t wasCurrent = 0;
    int32_t cur = F<int32_t>(list, 8);
    if (idx == cur) {
        wasCurrent = 1;
        if (cur > 1) TrainOrders_SetCurrent(list, 0, (cur + 1) % n, 1);
    }
    cur = F<int32_t>(list, 8);
    if (idx <= cur) {
        F<int32_t>(list, 8) = --cur;
        if (cur < 0 && F<int32_t>(list, 4) > 1) F<int32_t>(list, 8) = 0;
    }
    n = F<int32_t>(list, 4);
    if (n > 1) {
        uint8_t* buf = ((uint8_t*)crt::malloc((size_t)((n - 1) * 0x33)));
        if (idx > 0) memcpy(buf, F<uint8_t*>(list, 0), idx * 0x33);
        n = F<int32_t>(list, 4);
        if (idx + 1 < n) memcpy(buf + idx * 0x33, F<uint8_t*>(list, 0) + idx * 0x33 + 0x33, (n - idx - 1) * 0x33);
        crt::free(F<uint8_t*>(list, 0));
        F<uint8_t*>(list, 0) = buf;
    } else {
        crt::free(F<uint8_t*>(list, 0));
        F<uint8_t*>(list, 0) = nullptr;
    }
    --F<int32_t>(list, 4);
    TrainOrders_RefreshOwner(list);
    auto train = [&]() -> uint8_t* {
        return F<uint32_t>(list, 0xd) ? (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(list, 0xd)) : nullptr;
    };
    if (!F<uint32_t>(list, 0xd) || !Collection_Get(g_trains, 0, F<uint32_t>(list, 0xd))) return;
    Train_RetargetRegion(train());
    if (wasCurrent) Train_Route(train());
}
RT3_REPLACE(0x4b3000, TrainOrders_Remove);

// 0x4b3160. Train order list: inserts a copy of order entry e (0x33 bytes) at
// `idx` (0 when out of range). With the list's flag +0xc the entry first takes
// the last order's settings (all but +0x25..+0x27, the station +0x29.. and the
// top flag bits 7 and 0..3). During the tutorial (0x6d3b4c) an active station
// it refuses leaves the list unchanged. The first order becomes current; a
// later one at or before the current one moves it on. A live train re-targets.
RT3_NOINLINE void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e) {
    int32_t n = F<int32_t>(list, 4);
    if (list[0xc] && n) {
        uint8_t* last = F<uint8_t*>(list, 0) + (n - 1) * 0x33;
        F<uint32_t>(e, 0x24) = F<uint32_t>(last, 0x24);
        e[0x28] ^= (e[0x28] ^ last[0x28]) & 0x40;
        memcpy(e + 1, last + 1, 0x20);
        e[0x23] = last[0x23], e[0x22] = last[0x22], e[0x21] = last[0x21];
        e[0] = last[0];
        e[0x28] ^= (e[0x28] ^ last[0x28]) & 0x20;
        e[0x28] ^= (e[0x28] ^ last[0x28]) & 0x10;
    }
    if ((int8_t)e[0x28] >= 0 && g_6d3b4c) {
        uint32_t id = F<uint16_t>(e, 0x29);
        if (Obj_BuildingStateActive(Collection_Get(g_stations, 0, id)) && !Tutorial_OnBuildingPlaced((int32_t)id)) return;
    }
    if (idx < 0 || idx > F<int32_t>(list, 4)) idx = 0;
    uint8_t* buf = ((uint8_t*)crt::malloc((size_t)((F<int32_t>(list, 4) + 1) * 0x33)));
    if (idx > 0) memcpy(buf, F<uint8_t*>(list, 0), idx * 0x33);
    memcpy(buf + idx * 0x33, e, 0x33);
    if (idx < F<int32_t>(list, 4)) memcpy(buf + idx * 0x33 + 0x33, F<uint8_t*>(list, 0) + idx * 0x33, (F<int32_t>(list, 4) - idx) * 0x33);
    if (uint8_t* old = F<uint8_t*>(list, 0)) crt::free(old);
    F<uint8_t*>(list, 0) = buf;
    if (F<int32_t>(list, 4)) {
        ++F<int32_t>(list, 4);
        if (F<int32_t>(list, 8) >= idx) ++F<int32_t>(list, 8);
    } else {
        F<int32_t>(list, 4) = 1;
        TrainOrders_SetCurrent(list, 0, 0, 0);
    }
    if (F<uint32_t>(list, 0xd) && Collection_Get(g_trains, 0, F<uint32_t>(list, 0xd)))
        Train_RetargetRegion(F<uint32_t>(list, 0xd) ? (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(list, 0xd)) : nullptr);
    TrainOrders_RefreshOwner(list);
}
RT3_REPLACE(0x4b3160, TrainOrders_Insert);

// 0x4b3360. Train order list: the next order station that is active
// (Obj_BuildingStateActive), from the current entry (or the one after it with
// skipCurrent), going round once; 0 if none. With `move`, a live train that
// is not at that station already (its region differs and +0xa0 is another
// station) stops - state 8 without orders, 9 with one, else runs (0) - and
// the list advances to the next entry; then 0 is returned.
RT3_NOINLINE uint8_t* __fastcall TrainOrders_NextStop(uint8_t* list, int, int32_t move, int32_t skipCurrent) {
    uint8_t* st = nullptr;
    int32_t n = F<int32_t>(list, 4);
    if (n) {
        int32_t i = F<int32_t>(list, 8);
        if (skipCurrent) i = (i + 1) % n;
        for (int32_t k = 0;; ) {
            const uint8_t* e = F<uint8_t*>(list, 0) + i * 0x33;
            if ((int8_t)e[0x28] >= 0) {
                st = (uint8_t*)Collection_Get(g_stations, 0, F<uint16_t>((void*)e, 0x29));
                if (Obj_BuildingStateActive(st)) break;
                st = nullptr;
            }
            n = F<int32_t>(list, 4);
            i = (i + 1) % n;
            if (k++ > n) break;
        }
    }
    if (!move || !F<uint32_t>(list, 0xd) || !Collection_Get(g_trains, 0, F<uint32_t>(list, 0xd))) return st;
    auto train = [&]() -> uint8_t* {
        return F<uint32_t>(list, 0xd) ? (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(list, 0xd)) : nullptr;
    };
    if (st) {
        uint8_t* region = Train_FirstCarRegion(train());
        if (Obj_BuildingRegion(st) != region) {
            int32_t id = F<int32_t>(st, 0);
            if (id != F<int32_t>(train(), 0xa0)) return st;
        }
    }
    int32_t count = F<int32_t>(list, 4);
    Train_SetState(train(), 0, count == 0 ? 8 : count == 1 ? 9 : 0, 0.0f);
    if (int32_t c = F<int32_t>(list, 4)) TrainOrders_SetCurrent(list, 0, (F<int32_t>(list, 8) + 1) % c, 0);
    return nullptr;
}
RT3_REPLACE(0x4b3360, TrainOrders_NextStop);

// 0x4b34e0. Index (last match, 0 if none) of name +4 in the 23-entry table 0x621ac0.
RT3_NOINLINE int32_t __fastcall Obj_NameTableIndex(uint8_t* o) {
    int32_t found = 0;
    for (int32_t i = 0; i < 0x17; ++i)
        if (crt::stricmp(((const char**)0x621ac0)[i], (const char*)o + 4) == 0) found = i;
    return found;
}
RT3_REPLACE(0x4b34e0, Obj_NameTableIndex);

// 0x4b3520. Industry window destructor: releases its five pack images
// (0x6cfcf4 .. 0x6cfd00, 0x6cfd18), closes the overview mode and forgets the
// window (0x6cfd04).
void __fastcall IndustryWindow_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d00dc;
    static const uint32_t images[] = { 0x6cfcf4, 0x6cfcf8, 0x6cfcfc, 0x6cfd00, 0x6cfd18 };
    for (uint32_t a : images) Pack_Release(g_pack, 0, hook::Global<void*>(a));
    Overview_Close(g_map);
    hook::As<uint32_t>(g_industryWindow) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4b3520, IndustryWindow_Destruct);

// 0x4b35a0 (vtable stub).
int32_t __fastcall Return0_4b35a0(void*) { return 0; }
RT3_REPLACE(0x4b35a0, Return0_4b35a0);

RT3_NOINLINE double __fastcall Building_Total2c0(uint8_t* b) { return Obj_Float2c0(b, 0, 0) + F<float>(b, 0x2ac); }
RT3_REPLACE(0x4b35b0, Building_Total2c0);

// 0x4b35d0..0x4b3650. Building stats: this year's slot 0 plus the totals.
RT3_NOINLINE double __fastcall Building_Total2e8(uint8_t* b) { return Obj_Float2e8(b, 0, 0) + F<float>(b, 0x2b0); }
RT3_REPLACE(0x4b35d0, Building_Total2e8);

RT3_NOINLINE double __fastcall Building_Total310(uint8_t* b) { return Obj_Float310(b, 0, 0) + F<float>(b, 0x2b4); }
RT3_REPLACE(0x4b35f0, Building_Total310);

RT3_NOINLINE double __fastcall Building_Total338(uint8_t* b) { return Obj_Float338(b, 0, 0) + F<float>(b, 0x2b8); }
RT3_REPLACE(0x4b3610, Building_Total338);

RT3_NOINLINE double __fastcall Building_Total360(uint8_t* b) { return Obj_Float360(b, 0, 0) + F<float>(b, 0x2bc); }
RT3_REPLACE(0x4b3630, Building_Total360);

RT3_NOINLINE double __fastcall Building_TotalRemaining(uint8_t* b) {
    double r = Obj_Remaining(b, 0, 0);
    double t = (double)F<float>(b, 0x2b4) - F<float>(b, 0x2b8);
    t = t - F<float>(b, 0x2bc);
    return r + t;
}
RT3_REPLACE(0x4b3650, Building_TotalRemaining);

// 0x4b3670 (click callback). Cargo list (0x6cfd04) row `id` pressed, unless
// the mouse moved (0x6cfd0c / 0x6cfd10 vs 0x6d4018 +0xa94 / +0xa98) less
// than 0x5d00e8 with the button down (+0xa8c bit 0x400): relabels the old
// row (0x6cfd14, string 0xb8b) and the new one (string 0xb8c) with the cargo
// name, moves the highlight and shows the cargo overlay (mode 3).
void __fastcall CargoList_ClickCb(int32_t id, int, int32_t down, int32_t, int32_t, int32_t, int32_t) {
    if (!down) return;
    uint8_t* in = g_input;
    if (!(F<uint32_t>(in, 0xa8c) & 0x400)) return;
    if (!(Distance2D(hook::As<float>(g_6cfd0c), hook::As<float>(g_6cfd10), F<float>(in, 0xa94), F<float>(in, 0xa98)) >= D(0x5d00e8))) return;
    g_6cfd10 = 0xbf800000;
    g_6cfd0c = 0xbf800000;
    uint8_t* row = Widget_FindChild(g_industryWindow, 0, id);
    uint8_t* old = g_6cfd14 ? Widget_FindChild(g_industryWindow, 0, g_6cfd14) : row;
    uint8_t* cargo = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(row, 0x4f));
    if (g_6cfd14) cargo = (uint8_t*)Collection_Get(g_cargoTypes, 0, F<uint32_t>(old, 0x4f));
    const char* name = (const char*)cargo + 4;
    char text[0x200];
    {
        const char* shown = LocalizeName(name);
        FormatText(text, GetString(0xb8b), shown, 0, 0, 0, 0, 0, 0, 0);
    }
    Window_SendEvent5(g_industryWindow, 0, 0x6e, hook::As<uint32_t>(g_6cfd14), (uint32_t)(uintptr_t)text, 0, 0);
    {
        const char* shown = LocalizeName(name);
        FormatText(text, GetString(0xb8c), shown, 0, 0, 0, 0, 0, 0, 0);
    }
    Window_SendEvent5(g_industryWindow, 0, 0x6e, (uint32_t)id, (uint32_t)(uintptr_t)text, 0, 0);
    Widget_ClearFlags(old, 0, 1);
    Widget_SetFlags(row, 0, 1, 0, 0);
    uint8_t* sel = Widget_FindChild(g_industryWindow, 0, id);
    Overview_SetMode(g_map, 0, 3, F<int32_t>(sel, 0x4f), 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
    g_6cfd14 = id;
}
RT3_REPLACE(0x4b3670, CargoList_ClickCb);

// 0x4b3820. Building view: the income table of building `b` (panel `kind`) at
// (x, y), in font 0x6cfcf8 (needs a valid selected building): a label column
// (strings 3, 0x2ff, 0x300, 0x25f, 0x301, 0x302, 0x262; 0xdfe for row 1 of
// kinds 0x13 / 0x14) and one column per year since it was built (+0x3d5, at
// most 3) plus "total" (0x263). Rows 1-2 are "%5.1f" amounts (red below 0),
// rows 3-6 money (red at -500 or less; row 4 / 5 negated, row 5 underlined).
// Kinds 0x12 / 0x15 show only rows 0 and 2; money rows are hidden in the
// sandbox, the editor and for kinds 1 / 2.
RT3_NOINLINE void __fastcall BuildingView_DrawIncomeTable(uint8_t* b, int32_t kind, float x, float y, float, float, int32_t) {
    if (!Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    uint8_t* font = g_6cfcf8;
    float cx = (float)((double)x + D(0x5d0110));
    static const uint32_t widths[5] = { 0x43610000, 0x43110000, 0x43110000, 0x43110000, 0x431b0000 };
    static const int32_t labels[7] = { 3, 0x2ff, 0x300, 0x25f, 0x301, 0x302, 0x262 };
    int32_t years = F<uint16_t>(g_world, 0xd) - F<int32_t>(b, 0x3d5) + 1;
    char text[0x184];
    for (int32_t c = 0; c <= years && c < 5; ++c) {
        float colW = (float)((double)BitsF(widths[c]) / D(0x5ca2c8));
        float rowY = y;
        for (int32_t r = 0; r < 7; ++r) {
            if (r == 1 || r >= 3) {
                if (kind == 0x12 || kind == 0x15) continue;
                if (r >= 3 && (g_newGame[0x82] || F<int32_t>(g_world, 0x4aef) || kind == 1 || kind == 2))
                    continue;
            }
            uint32_t color = 0xffffffff;
            uint32_t flags = 0;
            if (c == 0) {
                strcpy(text, GetString(labels[r]));
                if (r == 1 && (kind == 0x13 || kind == 0x14)) strcpy(text, GetString(0xdfe));
            } else if (r == 0) {
                if (c == 4) crt::sprintf(text, GetString(0x263));
                else Num_Format(text, 8, (double)(F<uint16_t>(g_world, 0xd) - c + 1), 10, 1, 0, 0);
            } else {
                float v = 0.0f;
                if (c == 4) {
                    switch (r) {
                    case 1: v = (float)Building_Total2e8(b); break;
                    case 2: v = (float)Building_Total2c0(b); break;
                    case 3: v = (float)Building_Total310(b); break;
                    case 4: v = (float)-Building_Total338(b); break;
                    case 5: v = (float)-Building_Total360(b); break;
                    case 6: v = (float)Building_TotalRemaining(b); break;
                    }
                } else {
                    switch (r) {
                    case 1: v = (float)Obj_Float2e8(b, 0, c - 1); break;
                    case 2: v = (float)Obj_Float2c0(b, 0, c - 1); break;
                    case 3: v = (float)Obj_Float310(b, 0, c - 1); break;
                    case 4: v = (float)-Obj_Float338(b, 0, c - 1); break;
                    case 5: v = (float)-Obj_Float360(b, 0, c - 1); break;
                    case 6: v = (float)Obj_Remaining(b, 0, c - 1); break;
                    }
                }
                if (r <= 2) crt::sprintf(text, (const char*)0x5d0104, (double)v);
                else Num_FormatSimple(text, 0xb, (double)v, 0xb);
                bool red;
                if (r == 5) {
                    flags = 4;
                    red = v <= Fl_205(0x5cb45c);
                } else if (r <= 2) {
                    red = v < Fl_205(0x5c8598) || v <= Fl_205(0x5cb45c);
                } else {
                    red = v <= Fl_205(0x5cb45c);
                }
                if (red) color = 0xffff5f5f;
            }
            double pad = g_language == 1 && c == 0 ? BitsD(0x3f747ae147ae147bull) : 0.0;
            float padF = (float)pad;
            double extra = c == 0 ? BitsD(0x3fa3333333333333ull) : 0.0;
            float w = (float)((double)padF + colW + extra);
            Font_DrawTextBox(font, 0, text, cx, rowY, w, BitsF(0x3ccccccd), color, (c != 0) + 2, 0, flags, 1.0f);
            rowY = (float)((double)rowY + Fl_205(0x5d0100));
            if (r == 2) rowY = (float)((double)rowY + D(0x5d00f8));
            else if (r == 5) rowY = (float)((double)rowY + D(0x5d00f0));
        }
        cx = (float)((double)colW + cx + D(0x5d00e8));
    }
}
RT3_REPLACE(0x4b3820, BuildingView_DrawIncomeTable);

// 0x4b3ca0. Building info panel line `kind` (draw callback) for the selected
// building (main view +0x90 in 0x62b26c), text by the type index +0x3d9: kinds
// 3..9 use the string tables 0x621b1c.. (40 bytes apart) with formats 0x35e..0x364,
// kind 2 table 0x621c84 with format 0xaef, kind 10 the pair 0x621c34 / 0x621c5c
// (+0x3dd) in format 0x365, kind 22 a date-seeded 10..14 in format 0xb8d; drawn
// in font 0x6cfcf8.
RT3_NOINLINE void __fastcall BuildingPanel_DrawLine(uint8_t* b, int32_t kind, float x, float y, float w, float h, int32_t) {
    if (!Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    char text[0x1f4];
    text[0] = 0;
    auto fmt1 = [&](uint32_t table, int32_t format) {
        const char* s = GetString(*(const int32_t*)(table + F<int32_t>(b, 0x3d9) * 4));
        FormatText(text, GetString(format), s, 0, 0, 0, 0, 0, 0, 0);
    };
    switch (kind) {
    case 2: fmt1(0x621c84, 0xaef); break;
    case 3: fmt1(0x621b1c, 0x35e); break;
    case 4: fmt1(0x621b44, 0x35f); break;
    case 5: fmt1(0x621b6c, 0x360); break;
    case 6: fmt1(0x621b94, 0x361); break;
    case 7: fmt1(0x621bbc, 0x362); break;
    case 8: fmt1(0x621be4, 0x363); break;
    case 9: fmt1(0x621c0c, 0x364); break;
    case 10: {
        const char* a = GetString(*(const int32_t*)(0x621c5c + F<int32_t>(b, 0x3dd) * 4));
        const char* s = GetString(*(const int32_t*)(0x621c34 + F<int32_t>(b, 0x3d9) * 4));
        FormatText(text, GetString(0x365), s, a, 0, 0, 0, 0, 0, 0);
        break;
    }
    case 22: {
        uint8_t* world = g_world;
        int32_t seed = F<uint16_t>(world, 0xd) * 0x11 + world[0xf] * 3 + world[0x11];
        char num[0x64];
        const char* n = Num_Format(num, 1, (double)HashRand(seed, 10, 0xe), 10, 1, 0, 0);
        FormatText(text, GetString(0xb8d), n, 0, 0, 0, 0, 0, 0, 0);
        break;
    }
    default: break;
    }
    Font_DrawTextBox(g_6cfcf8, 0, text, x, y, w, h, 0xffffffff, 0, 0, 0, 1.0f);
}
RT3_REPLACE(0x4b3ca0, BuildingPanel_DrawLine);

// 0x4b3f80. Draws string 0x366 in the box (x, y, w, h) with font 0x6cfcf8 while the
// main view's selection (+0x90) is a valid entry of 0x62b26c.
RT3_NOINLINE void __stdcall View_DrawSelectionLabel(float x, float y, float w, float h, int32_t) {
    if (!Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    Font_DrawTextBox(g_6cfcf8, 0, GetString(0x366), x, y, w, h, 0xffffffff, 0, 0, 0, 1.0f);
}
RT3_REPLACE(0x4b3f80, View_DrawSelectionLabel);

// 0x4b3fe0. Draws the production panel of the selected industry o at
// (x, y, width w): headers for its outputs, inputs and stock, the farm or
// fertilizer note, and (once per opening, flag 0x6cfd09) the row widgets of
// window 0x6cfd04: per cargo entry of its recipe (vtable +0x80, 0xbc-byte
// entries) an amount label, the cargo picture and a hover box, the inputs'
// own ingredients, the troop counts and the barracks extras.
RT3_NOINLINE void __fastcall Industry_DrawProduction(uint8_t* o, int, float x, float y, float w, int32_t, int32_t) {
    if (!Collection_IsValid(g_buildings, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    uint8_t* type = ((uint8_t*(__thiscall*)(uint8_t*))(*(void***)o)[0x80 / 4])(o);
    uint8_t* kind = (uint8_t*)Obj_Ref3cc(o);
    int32_t nA = 0, nB = 0, nC = 0;
    for (int32_t i = 0; i < F<int32_t>(type, 0x33); ++i) {
        uint8_t* e = F<uint8_t*>(type, 0x37) + i * 0xbc;
        if (!Obj_TypeQuery(o, 0, (int32_t)(uintptr_t)e)) continue;
        int32_t n = F<int32_t>(e, 0x2c);
        if (F<int32_t>(e, 0) && n > 0)
            ++nC;
        else if (n)
            ++nA;
        else
            ++nB;
    }
    float cy = y;
    if (uint8_t* c = Widget_FindChild(Prod_Win(), 0, 0xbd1b))
        if (Widget_IsVisible(c)) cy = (float)((double)y + D(0x5ca1a8));
    float rowB = 0.0f, rowC = 0.0f, rowA = 0.0f;
    if (nB && type[0x32]) {
        float ww = w;
        if (nC && nA) ww = (float)((double)w / D(0x5c85d0));
        Prod_Text(GetString(0x367), x, cy, ww, 0);
        rowB = (float)((double)cy + D(0x5d0238));
        if (!nC || !nA) cy = (float)((double)cy + D(0x5d0230));
    }
    if (nC) {
        float ww = w, xx = x;
        if (nB && nA) {
            ww = (float)((double)w / D(0x5c85d0));
            xx = (float)((double)ww + x);
        }
        Prod_Text(GetString(0x368), xx, cy, ww, 0);
        if (!type[0x32]) {
            float yy = (float)((double)y + Fl_207(0x5d0228));
            if (nC < 0xf) yy = (float)((double)yy - Fl_207(0x5d0224));
            float x2 = (float)((double)x + D(0x5d0110));
            Font_DrawTextBox(g_6cfcf8, 0, GetString(0xdff), x2, yy, 0.125f, Prod_Bits(0x3ccccccd), 0xffffffff, 2, 0, 0, 1.0f);
        }
        rowC = (float)((double)cy + D(0x5d0238));
        if (!type[0x32] && nC > 0xe) rowC = (float)((double)rowC - Fl_207(0x5ce838));
        cy = (float)((double)cy + D(0x5c9b40));
    }
    bool farm = ((uint8_t*)Obj_Ref3cc(o))[0x123] || Prod_KindIs(o, 0x5d0218) || Prod_KindIs(o, 0x5d020c) || Prod_KindIs(o, 0x5d0200);
    auto fertilized = [] {
        uint32_t f = Table_FindByName(g_cargoTypes, 0, (const char*)0x5d01e8);
        return F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x16e + f * 4) != 0;
    };
    if (nA && !farm) {
        float step = Prod_Bits(0x3d19999a);
        if (nB || (nC && type[0x32])) {
            step = Prod_Bits(0x3cf5c28f);
            cy = (float)((double)cy + D(0x5d01f8));
        }
        float v = F<int32_t>(o, 0x246) ? F<float>(type, 0x47) : 0.0f;
        int32_t n = Ftol(((double)v + D(0x5c8770)) * F<float>(type, 0x2a));
        char num[0x100], text[0x200];
        const char* s = Prod_Num(num, 1, (double)n);
        Prod_Format(text, GetString(0x369), s, nullptr);
        Prod_Text(text, x, cy, w, 0);
        rowA = (float)((double)step + cy);
    } else if (farm) {
        char scratch[0x400];
        FormatText(scratch, (const char*)Obj_Ref3cc(o) + 0x123, 0, 0, 0, 0, 0, 0, 0, 0);
        int32_t str = 0x36a;
        if (!Prod_KindIs(o, 0x5d0218) && !Prod_KindIs(o, 0x5d020c) && !Prod_KindIs(o, 0x5d0200)) str = fertilized() ? 0x36b : 0;
        if (str) {
            float yy = (float)((double)cy + D(0x5d01e0));
            Prod_Text(GetString(str), x, yy, w, 0);
        }
    }
    if (Prod_KindIs(o, 0x5c9378)) {
        float yy = (float)((double)cy + D(0x5d01d8));
        Prod_Text(GetString(0xdff), x, yy, w, 0);
    }
    if (g_6cfd09) return;
    Window_DeleteChildren(Prod_Win(), 0, 0xbd42, 0xbda5, 0);
    double half = (double)w / D(0x5c85d0);
    g_6cfd09 = 1;
    double mid = (double)x + half;
    double offB = (double)nB * D(0x5c9ee0) / D(0x5c85d0);
    float colB = (float)(mid - offB - D(0x5d00f8));
    double offC = (double)nC * D(0x5c9ee0) / D(0x5c85d0);
    float colC = (float)(mid - offC - D(0x5d00f8));
    if (!type[0x32]) colC = x;
    if (nC && nB && nA) {
        colB = (float)((double)w / D(0x5c87b8) + x - offB - D(0x5d01d0));
        colC = (float)((double)w * D(0x5c8750) + x - offC - D(0x5d01c8));
    }
    int32_t inputs = 0;
    bool mixed = nA && (nB || nC);
    uint32_t id = 0;
    float tipY = 0.0f;
    float own = (float)F<int32_t>(kind, 0x141);
    char name[0x100], num[0x100], tip[0x200];
    for (int32_t i = 0; i < F<int32_t>(type, 0x33); ++i) {
        uint8_t* e = F<uint8_t*>(type, 0x37) + i * 0xbc;
        if (!Obj_TypeQuery(o, 0, (int32_t)(uintptr_t)e)) continue;
        float lx, ly;
        uint32_t link;
        if (F<int32_t>(e, 0)) {
            lx = colC;
            colC = (float)((double)colC + Fl_207(0x5d01c4));
            ly = rowC;
            Prod_Format(name, (const char*)0x5d01bc, (const char*)(e + 0x30), nullptr);
            link = F<uint32_t>(e, 0x44);
            double v = (double)own * F<float>(e, 0x48);
            if (!type[0x32]) {
                Prod_Num(num, 3, v);
                colC = (float)((double)colC + Fl_207(0x5d01b8));
                if ((double)x + w < (double)colC + D(0x5c9ee0)) {
                    colC = x;
                    rowC = (float)((double)rowC + D(0x5c9b98));
                    if (nC < 0xf) rowC = (float)((double)rowC + Fl_207(0x5cb5c0));
                }
            } else
                Prod_Num(num, 1, v);
            const char* nm = LocalizeName((const char*)(e + 0x30));
            Prod_Format(tip, GetString(0x36c), num, nm);
        } else if (int32_t n = F<int32_t>(e, 0x2c)) {
            if (farm) continue;
            int32_t k = n * 0xaa + 0xdc;
            float fk = (float)((double)k / D(0x5ca2c8));
            double q = (double)fk / D(0x5c85d0);
            lx = (float)((double)x + half + q - D(0x5d01b0));
            if ((nA > 3 && !mixed) || (mixed && nA == 2)) {
                lx = (float)(mid - D(0x5c8988));
                if (Prod_Rem(inputs, 2)) lx = (float)((double)lx + half);
            }
            if (mixed && nA > 2) {
                int32_t rem = Prod_Rem(inputs, nA);
                lx = (float)((double)x + q - D(0x5d01d0));
                float fa = (float)nA, fr = (float)rem;
                lx = (float)((double)w / fa * fr + lx);
                if (nA > 3) {
                    float t = (float)((nA - 3) * 0x41);
                    lx = (float)((double)lx - (double)t / Fl_207(0x5ca198));
                }
            }
            ly = rowA;
            if (!mixed) {
                if (Prod_Rem(inputs, 2) || nA < 4) rowA = (float)((double)rowA + Fl_207(0x5d0058));
                if (nA > 3 && inputs == 5) rowA = (float)((double)rowA - Fl_207(0x5ce500));
            } else if (inputs > 2)
                rowA = (float)((double)rowA + Fl_207(0x5d0058));
            Prod_Format(name, (const char*)0x5d01bc, (const char*)(e + 8), nullptr);
            link = F<uint32_t>(e, 0x1c);
            Prod_Num(num, 1, (double)own * F<float>(e, 4));
            const char* nm = LocalizeName((const char*)(e + 8));
            Prod_Format(tip, GetString(0x36c), num, nm);
            ++inputs;
        } else {
            if (!type[0x32]) continue;
            lx = colB;
            colB = (float)((double)colB + Fl_207(0x5d01c4));
            ly = rowB;
            Prod_Format(name, (const char*)0x5d01bc, (const char*)(e + 8), nullptr);
            link = F<uint32_t>(e, 0x1c);
            Prod_Num(num, 1, (double)own * F<float>(e, 4));
            uint8_t* recipe = F<uint8_t*>(type, 0x37);
            if (!farm) {
                const char* nm = LocalizeName((const char*)(e + 8));
                Prod_Format(tip, GetString(0x36c), num, nm);
            } else if (Prod_KindIs(o, 0x5d0218)) {
                char a[0x100], b[0x100];
                Prod_Num(a, 3, (double)F<float>(recipe, 0x238) * (double)F<int32_t>(kind, 0x141) * D(0x5c8578));
                Prod_Num(b, 3, (double)F<float>(recipe, 0x17c) * (double)F<int32_t>(kind, 0x141) * D(0x5c8578));
                Prod_Format(tip, GetString(0x36d), a, b);
            } else if (Prod_KindIs(o, 0x5d020c)) {
                char a[0x100];
                Prod_Num(a, 3, (double)F<float>(recipe, 0xc0) * (double)F<int32_t>(kind, 0x141) * D(0x5c8578));
                Prod_Format(tip, GetString(0x36e), a, nullptr);
            } else if (Prod_KindIs(o, 0x5d0200)) {
                char a[0x100];
                Prod_Num(a, 3, (double)F<float>(recipe, 0xc0) * (double)F<int32_t>(kind, 0x141) * D(0x5c8578));
                Prod_Format(tip, GetString(0x36f), a, nullptr);
            } else if (fertilized()) {
                Prod_Num(num, 3, (double)F<float>(e, 4) * (double)F<int32_t>(kind, 0x141) * D(0x5c8698));
                const char* nm = LocalizeName((const char*)(e + 8));
                Prod_Format(tip, GetString(0x370), num, nm);
            } else {
                const char* nm = LocalizeName((const char*)(e + 8));
                Prod_Format(tip, GetString(0x36c), num, nm);
            }
        }
        // Amount, picture and hover box of the entry.
        double off = type[0x32] ? 0.0 : 0.01;
        float labelY = (float)((double)ly - off);
        uint32_t h = 0x3d000000;
        float labelX = lx;
        if (!type[0x32]) {
            Prod_Num(num, 3, (double)own * F<float>(e, 0x48));
            h = 0x3d400000;
            lx = (float)((double)lx + Fl_207(0x5d01a8));
        } else
            Prod_Num(num, 1, (double)own * (F<int32_t>(e, 0) ? F<float>(e, 0x48) : F<float>(e, 4)));
        Prod_Label(id, labelX, labelY, h, 3, type[0x32] ? 3 : 2, num);
        lx = (float)((double)lx + D(0x5d01a0));
        ++id;
        float iy = (nA > 3 && mixed) ? (float)((double)ly + Fl_207(0x5d01b8)) : ly;
        uint32_t scale = !type[0x32] ? 0x3fbe6666 : nA < 2 ? 0x40000000 : (nA > 3 && mixed) ? 0x3fc00000 : 0x3feccccd;
        Prod_Image(id, lx, iy, name, tip, 0, scale);
        ++id;
        float tx = (float)((double)lx - (type[0x32] ? Fl_207(0x5d018c) : Fl_207(0x5d0190)));
        tipY = (float)((double)ly - Fl_207(0x5d0188));
        Prod_Tip(id, tx, tipY, 0x3da00000, type[0x32] ? 0x3d75c28f : 0x3d4ccccd, link, tip);
        if (F<int32_t>(e, 0) || F<int32_t>(e, 0x2c) <= 0 || farm) continue;
        // The input's own ingredients: "= a + b ..." to its left.
        lx = (float)((double)lx - D(0x5d0180));
        if (mixed && nA > 3) lx = (float)((double)lx + Fl_207(0x5d017c));
        Prod_Label(id, lx, ly, 0x3d000000, 0, 3, (const char*)0x5d0178);
        ++id;
        uint8_t* s = e + 0x30;
        for (int32_t j = 0; j < F<int32_t>(e, 0x2c); ++j, s += 0x1c) {
            if (j > 0) {
                lx = (float)((double)lx - D(0x5d0170));
                Prod_Label(id, lx, ly, 0x3d000000, 0, 3, (const char*)0x5ca2b8);
                ++id;
            }
            lx = (float)((double)lx - D(0x5c9ba0));
            if (mixed && nA > 3) lx = (float)((double)lx + Fl_207(0x5d0168));
            Prod_Format(name, (const char*)0x5d01bc, (const char*)s, nullptr);
            link = F<uint32_t>(s, 0x14);
            Prod_Num(num, 1, (double)own * F<float>(s, 0x18));
            const char* nm = LocalizeName((const char*)s);
            Prod_Format(tip, GetString(0x36c), num, nm);
            float sy = (nA > 3 && mixed) ? (float)((double)ly + Fl_207(0x5d01b8)) : ly;
            uint32_t sc = nA < 2 ? 0x40000000 : (nA > 3 && mixed) ? 0x3fc00000 : 0x3feccccd;
            Prod_Image(id, lx, sy, name, tip, 0, sc);
            lx = (float)((double)lx - D(0x5d0160));
            ++id;
            Prod_Num(num, 1, (double)(float)F<int32_t>(kind, 0x141) * F<float>(s, 0x18));
            Prod_Label(id, lx, ly, 0x3d000000, 3, 3, num);
            ++id;
            Prod_Tip(id, (float)((double)lx + Fl_207(0x5d01b8)), tipY, 0x3da00000, 0x3d75c28f, link, tip);
        }
    }
    if (!type[0x32]) {
        float py = (float)((double)y + Fl_207(0x5ce500));
        static const uint32_t imbs[2] = { 0x5d0150, 0x5d0144 };
        static const int32_t tips[2] = { 0xe8e, 0xe8f };
        float ly = (float)((double)y + Fl_207(0x5d0134));
        for (int32_t i = 0; i < 2; ++i) {
            float fi = (float)i;
            float bx = (float)((double)fi * Fl_207(0x5d0130) + x);
            float lx = (float)((double)bx + Fl_207(0x5d012c));
            float labelY = ly;
            if (nC < 0xf) labelY = (float)((double)labelY - Fl_207(0x5d0224));
            char a[0x100], b[0x100];
            Prod_Num(a, 1, (double)(float)F<int32_t>(kind, 0x141) * F<float>(o, 0x27e + i * 4));
            Prod_Num(b, 1, (double)(float)F<int32_t>(kind, 0x141) * F<float>(o, 0x28a + i * 4));
            Prod_Format(num, (const char*)0x5cfda8, a, b);
            Prod_Label(id, lx, labelY, 0x3d000000, 3, 2, num);
            ++id;
            float iy = py;
            if (nC < 0xf) iy = (float)((double)iy - Fl_207(0x5d0224));
            Prod_Image(id, (float)((double)bx + Fl_207(0x5d0128)), iy, (const char*)imbs[i], nullptr, tips[i], 0x3fbe6666);
            ++id;
            float ty = ly;
            if (nC < 0xf) ty = (float)((double)ty - Fl_207(0x5d0224));
            uint32_t link = F<uint16_t>(hook::As<uint8_t*>(g_cargoTypes), i ? 0x8a : 0x88);
            const char* t = GetString(tips[i]);
            Prod_Tip(id, lx, ty, 0x3da66666, 0x3d75c28f, link, t);
        }
    }
    if (Prod_KindIs(o, 0x5c9378)) {
        float lx = (float)((double)x + Fl_207(0x5d0124));
        float ly = (float)((double)y + Fl_207(0x5d0120));
        char a[0x100], b[0x100];
        Prod_Num(a, 1, (double)(float)F<int32_t>(kind, 0x141) * F<float>(o, 0x286));
        Prod_Num(b, 1, (double)(float)F<int32_t>(kind, 0x141) * F<float>(o, 0x292));
        Prod_Format(num, (const char*)0x5cfda8, a, b);
        Prod_Label(id, lx, ly, 0x3d000000, 3, 3, num);
        ++id;
        float ix = (float)((double)x + Fl_207(0x5d011c));
        float iy = (float)((double)y + Fl_207(0x5d0118));
        Prod_Image(id, ix, iy, (const char*)0x5d0138, nullptr, 0xe90, 0x40000000);
        ++id;
        uint32_t link = F<uint16_t>(hook::As<uint8_t*>(g_cargoTypes), 0x8c);
        const char* t = GetString(0xe90);
        Prod_Tip(id, lx, ly, 0x3da66666, 0x3d75c28f, link, t);
    }
}
RT3_REPLACE(0x4b3fe0, Industry_DrawProduction);

}  // namespace rt3
