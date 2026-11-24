// Minimap.cpp - RT3.exe 0x478000-0x47c000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/stream.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t __fastcall Rand(int32_t lo, int32_t hi);
float __fastcall Obj_NodeCenterX(void* o);
float __fastcall Obj_NodeCenterY(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
void __fastcall Obj_AssignRandomSlots(uint8_t* o);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Obj_BuildingValue(void* o);
double __fastcall Obj_BuildingCellX(void* o);
double __fastcall Obj_BuildingCellY(void* o);
float __fastcall Obj_ScaledExtent(uint8_t* o);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
float __fastcall SceneObj_Angle2(uint8_t* o);
void* __fastcall Obj_Building(void* o);
void* __fastcall Obj_Ref25(void* o);
float __fastcall Player_Aggressiveness(uint8_t* p);
void* __fastcall Array33_At(void* a, int, int32_t i);
void __fastcall Texture_Destroy(uint8_t* t);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
void __fastcall MapView_Init(uint8_t* v);
void __fastcall Engine_ResetTextureQueue(void* e);
float __fastcall Obj_MapY(uint8_t* o, int, float y);
uint8_t* __fastcall MapView_Construct(uint8_t* w, int, const uint8_t* def);
void* __fastcall Obj_City41(void* o);
void __fastcall Widget_Destruct(uint8_t* w);
void* SelectedTrain();
void* __fastcall Identity(void* o);
void* Get6d3ae8b();
double __fastcall Obj_MapX(uint8_t* o, int, float x);
int32_t __fastcall Obj_ByteF9(uint8_t* o);
void __fastcall Train_RearPoint(uint8_t* t, int, float* x, float* y);
void __fastcall MoneySlider_Setup(int32_t reset);
int32_t __fastcall Collection_Count(Collection* c);
void* __fastcall Engine_Get1C(void* e);
uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h);
void __fastcall Mem_Fill32(uint32_t* dst, uint32_t v, uint32_t count);
void* SelectedCompany();
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Map_Bit9NearWater(uint8_t* m, int, int32_t x, int32_t y);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
int32_t __fastcall Map_TrackOwnerAt(void* o, int, int32_t x, int32_t y, int32_t* owner, int32_t* flag);
void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

int32_t Global6d40e4();
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t __fastcall Widget_IsVisible(uint8_t* w);
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
int32_t __fastcall Texture_SetPixels(uint8_t* t, int, void* pixels);
int32_t ClockMs();
void Engine_Nop();
struct InputEvent;
InputEvent* __fastcall Input_NextEvent(uint8_t* q, int, InputEvent* out);
void __fastcall Input_Block(uint8_t* in);
void __fastcall Input_Unblock(uint8_t* in);
int32_t __fastcall Widget_HandleEvent(uint8_t* w, int, uint8_t* e);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag);
void __fastcall Minimap_WorldToPixel(uint8_t* o, int, float x, float y, float* outX, float* outY);
void* __fastcall Map_RegionRecordAt(void* m, int, float x, float y);
int32_t __fastcall Minimap_PickNearest(uint8_t* o, int, float x, float y, uint32_t* building, uint32_t* other);
void __fastcall Widget_OnHover(uint8_t* w, int, float x, float y, int32_t moved, int32_t track);
int32_t IsMultiplayer();
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
void* __fastcall Train_Company(void* t);
int32_t __fastcall Widget_IsEnabled(uint8_t* w);
void __fastcall ModelObj_Destructor(uint8_t* o);
void __fastcall ModelObj_Release(uint8_t* o);
uint8_t* __fastcall ModelObj_Construct(uint8_t* o);
void __fastcall Building_Init(uint8_t* o, int, const char* name, const char* tex, float x, float z, int32_t place, int32_t a6, int32_t a7, uint16_t a8);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
void __fastcall Obj_MoveToCell(uint8_t* o, int, float cx, float cz, float y);
void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);

struct InputEvent {
    uint32_t v[12];
};

void __fastcall Camera_MoveToCell(uint8_t* c, int, float x, float z);
void __fastcall MinimapWidget_Refresh(uint8_t* m, int, int32_t haveTex, uint32_t* tex, int32_t srcW, int32_t srcH);
void __fastcall MapPointer_Place(int32_t slot, int32_t kind, float x, float z, float lift);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Obj_CityKindIs2(void* o);
int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
int32_t SelectedTrainId();
uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric, int32_t companyId);
void* __fastcall Obj_SelectedTrainIfFlag(uint8_t* o);
void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
RT3_NOINLINE int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
RT3_NOINLINE void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
RT3_NOINLINE uint8_t __fastcall Orders_Drivable(uint32_t* orders, int, uint8_t* train, int32_t start, int32_t forceElectric, int32_t noElectric, int32_t companyId);
int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall Minimap_AddOrder(uint8_t* o, int, float x, float y);
void __fastcall Minimap_DragCamera(uint8_t* o);
uint8_t* __fastcall MinimapWidget_Texture(uint8_t* m, int, int32_t w, int32_t h);
void __fastcall Overview_FitArea(uint8_t* o);
void __fastcall Engine_QueueType5(void* e, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Overview_UpdateView(uint8_t* o);
void Fade_Step();

void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12);

void __fastcall Minimap_DrawRouteMarkers(uint8_t* m);
void __fastcall MapWidget_DrawCityMarker(uint8_t* w);
void __fastcall MiniMap_DrawView(uint8_t* m);
void __fastcall Engine_QueueType6(void* e);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void __fastcall ModelObj_Release(uint8_t* o);
RT3_NOINLINE void __fastcall MapWidget_DrawCityMarker(uint8_t* w);
RT3_NOINLINE void __fastcall Minimap_DrawRouteMarkers(uint8_t* m);
RT3_NOINLINE void __fastcall Minimap_DragCamera(uint8_t* o);
RT3_NOINLINE void __fastcall Overview_FitArea(uint8_t* o);
RT3_NOINLINE void __fastcall Overview_UpdateView(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Minimap_PickNearest(uint8_t* o, int, float x, float y, uint32_t* building, uint32_t* other);
RT3_NOINLINE void __fastcall Minimap_AddOrder(uint8_t* o, int, float x, float y);
RT3_NOINLINE void __fastcall Minimap_Render(uint32_t* dst, int32_t w, int32_t h, uint32_t flags, int32_t player, int32_t haveTex, uint32_t* tex, int32_t srcW, int32_t srcH, float* u0, float* u1, float* v0, float* v1);
uint8_t* __fastcall MapView_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall MapView_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall MapWidget_Destruct(uint8_t* w);
void __fastcall MinimapWidget_Draw(uint8_t* w);
RT3_NOINLINE void __fastcall MinimapWidget_Refresh(uint8_t* m, int, int32_t haveTex, uint32_t* tex, int32_t srcW, int32_t srcH);
uint8_t* __fastcall MapWidget_DeletingDestructor(uint8_t* w, int, uint32_t flags);
RT3_NOINLINE uint8_t* __fastcall MinimapWidget_Texture(uint8_t* m, int, int32_t w, int32_t h);
void __fastcall MinimapWidget_OnHover(uint8_t* m, int, float x, float y, int32_t moved, int32_t track);
int32_t __fastcall MinimapWidget_HandleEvent(uint8_t* w, int, uint32_t* msg);
RT3_NOINLINE void ClearGlobals6ceba4();
RT3_NOINLINE void __fastcall MapPointer_Place(int32_t slot, int32_t kind, float x, float z, float lift);
RT3_NOINLINE void PreviewModels_Free();
void* __fastcall ReturnThis47bbd0(void* o);
void __fastcall Nop47bbe0(void*);
RT3_NOINLINE void __fastcall NewGameConfig_Reset(uint8_t* c);
int32_t NewGameConfig_FirstOpenSlot();
RT3_NOINLINE void __fastcall Obj_AssignRandomSlots(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_ApplySetup(uint8_t* o, int, uint8_t* src);
RT3_NOINLINE void __fastcall MoneySlider_Setup(int32_t reset);
RT3_NOINLINE int32_t __fastcall Train_SetState(uint8_t* t, int, int32_t state, float v);
RT3_NOINLINE void __fastcall TrainOrders_Insert(uint8_t* list, int, int32_t idx, uint8_t* e);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
void __fastcall Widget_LoadBase(uint8_t* w, int, const uint8_t* rec);
RT3_NOINLINE void __fastcall Widget_Destruct(uint8_t* w);
RT3_NOINLINE uint8_t* __fastcall Widget_Construct(uint8_t* w, int, const uint8_t* def);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

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

// Clamps a [lo, hi] cell span into [0, size - 1], sliding it (and splitting any
// leftover overshoot) as the original does.
static void ClampSpan(int32_t& lo, int32_t& hi, int32_t size) {
    if (lo < 0) {
        hi = (lo < 0 ? -lo : lo) + hi;
        lo = 0;
    }
    if (hi > size - 1) {
        lo += size - hi - 1;
        hi = size - 1;
    }
    if (lo < 0) {
        int32_t a = (lo < 0 ? -lo : lo) / 2;
        hi += a;
        lo = a + lo;
    }
}

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline void* ExeMalloc(size_t n) { return ((void*(__cdecl*)(size_t))0x5a125d)(n); }

// Pack-loaded widgets (the constructors take an unused argument): a bare base, the
// kind's init, then an `n`-byte record read from the pack's current position.
static uint8_t* Widget_ReadRecord(uint8_t* w, int32_t n) {
    uint8_t* rec = (uint8_t*)ExeMalloc(n);
    NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, rec, n);
    Widget_LoadBase(w, 0, rec);
    return rec;
}

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

namespace {
// Widget slot 0x20: whether (x, y) hits it (third argument 1).
int32_t WidgetHit453(uint8_t* w, uint32_t x, uint32_t y) {
    return ((int32_t(__thiscall*)(uint8_t*, uint32_t, uint32_t, int32_t))(*(void***)w)[0x20 / 4])(w, x, y, 1);
}
int32_t WidgetSlot453(uint8_t* w, uint32_t off) { return ((int32_t(__thiscall*)(uint8_t*))(*(void***)w)[off / 4])(w); }
// The click callback +0x9a gets the widget id (ECX), itself (EDX) and the
// click relative to the widget.
void WidgetClickCb453(uint8_t* w, uint32_t* msg) {
    auto cb = F<void(__fastcall*)(uint32_t, void*, float, float)>(w, 0x9a);
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const float ry = (float)((double)*(const float*)&msg[10] - ((double)F<float>(p, 9) + F<float>(w, 0xf)));
    const float rx = (float)((double)*(const float*)&msg[9] - ((double)F<float>(p, 5) + F<float>(w, 0xb)));
    cb(F<uint16_t>(w, 4), (void*)cb, rx, ry);
}
}

// 0x478200. Draws the marker glyph (2 of +0xaf) of city +0x11b on the map widget,
// centred on the city: x maps the city's cell x over [+0xc3, +0xcb] onto the
// width +0x13, y goes through Obj_MapY.
RT3_NOINLINE void __fastcall MapWidget_DrawCityMarker(uint8_t* w) {
    Collection* cities = hook::Global<Collection*>(0x62bae0);
    if (!Collection_IsValid(cities, 0, F<uint32_t>(w, 0x11b))) return;
    void* city = Collection_Get(hook::Global<Collection*>(0x62bae0), 0, F<uint32_t>(w, 0x11b));
    float cx = (float)Obj_CellX(city);
    float cy = (float)Obj_CellY(city);
    uint8_t* g = (uint8_t*)Table_Entry(F<uint8_t*>(w, 0xaf), 0, 2);
    float lo = (float)F<int32_t>(w, 0xc3);
    float span = (float)(F<int32_t>(w, 0xcb) - F<int32_t>(w, 0xc3));
    float x = (float)(((double)cx - lo) * F<float>(w, 0x13) / span + F<float>(F<uint8_t*>(w, 0x86), 5) + F<float>(w, 0xb));
    double y = Obj_MapY(w, 0, cy);
    float gy = (float)(y - Slider_StepB(g) * D(0x5c8628));
    float gx = (float)((double)x - Slider_StepA(g) * D(0x5c8628));
    Glyph_Queue(g, 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x478200, MapWidget_DrawCityMarker);

// 0x478330. Draws the route markers on the minimap for the order list being
// edited (the selected train's +0x10000, or the train list's +0x20000): the
// marker size follows the map scale and zoom (+0x13). In six passes: stations
// that are not / are active (+ again, ordered ones in passes 3 / 4), the
// order nodes (numbered images 0x3d..0x44), and the window's own station
// (+0x10f, double size); then every running train's arrow by heading (the
// selected one highlighted) and the marked node +0x113. Only markers inside
// the minimap's pixel box (+0xc3.., inset by 2) are drawn.
RT3_NOINLINE void __fastcall Minimap_DrawRouteMarkers(uint8_t* m) {
    void* list;
    uint32_t flags = F<uint32_t>(m, 0x33);
    if (flags & 0x10000) {
        if (!SelectedTrain()) return;
        list = Identity(SelectedTrain());
    } else if (flags & 0x20000)
        list = Get6d3ae8b();
    else
        return;
    if (!list) return;
    Engine_ResetTextureQueue(hook::Global<void*>(0x6d4024));
    int32_t x0 = F<int32_t>(m, 0xc3), y0 = F<int32_t>(m, 0xc7);
    float bx0 = (float)(x0 + 2), by0 = (float)(y0 + 2), bx1 = (float)(F<int32_t>(m, 0xcb) - 2), by1 = (float)(F<int32_t>(m, 0xcf) - 2);
    float wf = (float)(F<int32_t>(m, 0xcb) - x0);
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    float mw = (float)F<int32_t>(map, 0x2155);
    float sx = (float)((double)wf / mw);
    float hf = (float)(F<int32_t>(m, 0xcf) - y0);
    float mh = (float)F<int32_t>(map, 0x2159);
    float sy = (float)((double)hf / mh);
    float scale = sx < sy ? sx : sy;
    float zoom = (float)((double)F<float>(m, 0x13) / D(0x5c8640));
    double k = scale < D(0x5c85b8) ? BitsD(0x3f847ae147ae147bull) : (double)scale;
    float r = (float)(D(0x5c8770) / k);
    r = (float)((double)r * zoom);
    if (r < D(0x5c8770)) r = (float)((D(0x5c8770) - r) * D(0x5c8c20) + r);
    else r = (float)(((double)r - D(0x5c8770)) * D(0x5c8628) + D(0x5c8770));
    uint32_t rb = FBits(r);
    auto inBox = [&](float px, float pz) { return !(px < bx0) && !(pz < by0) && !(px > bx1) && !(pz > by1); };
    auto marker = [&](uint8_t* img, uint32_t tint, float px, float pz) {
        double gyd = Obj_MapY(m, 0, pz);
        float gy = (float)(gyd - Slider_StepB(img) * r * D(0x5c8628));
        double gxd = Obj_MapX(m, 0, px);
        float gx = (float)(gxd - Slider_StepA(img) * r * D(0x5c8628));
        Glyph_Queue(img, 0, gx, gy, tint, BitsF(rb), BitsF(rb), 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    };
    auto image = [&](int32_t i) { return (uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(m, 0xaf), 0, i); };
    for (int32_t pass = 0; pass < 6; ++pass) {
        if (pass == 2) {
            Engine_ResetTextureQueue(hook::Global<void*>(0x6d4024));
            for (int32_t i = 0; i < F<int32_t>(list, 4); ++i) {
                if (!(((int8_t*)Array33_At(list, 0, i))[0x28] < 0)) continue;
                void* node = Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(Array33_At(list, 0, i), 0x2b));
                float px = Obj_CenterCellX(node);
                float pz = Obj_CenterCellY(node);
                if (!inBox(px, pz)) continue;
                marker(image((i < 7 ? i : 7) + 0x3d), 0x3f800000, px, pz);
            }
            continue;
        }
        Collection* stations = hook::Global<Collection*>(0x6cec20);
        for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++j) {
            stations = hook::Global<Collection*>(0x6cec20);
            uint8_t* st = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, j, 0));
            if ((pass == 0 || pass == 3) && Obj_BuildingStateActive(st)) continue;
            if ((pass == 1 || pass == 4) && !Obj_BuildingStateActive(st)) continue;
            bool own = F<uint32_t>(st, 0) == F<uint32_t>(m, 0x10f);
            if (pass == 5 ? !own : own) continue;
            float px = Obj_NodeCenterX(st);
            float pz = Obj_NodeCenterY(st);
            if (!inBox(px, pz)) continue;
            int32_t id = 0x14;
            int32_t mine = Obj_Get21(hook::Global<uint8_t*>(0x6cec78));
            if (Obj_BuildingValue(st) != mine) id = 0x28;
            bool hit = false;
            for (int32_t o = 0; o < F<int32_t>(list, 4); ++o)
                if (F<uint16_t>(Array33_At(list, 0, o), 0x29) == F<uint32_t>(st, 0)) {
                    id += (o < 7 ? o : 7) + 1;
                    hit = true;
                    break;
                }
            if (!Obj_BuildingStateActive(st)) {
                uint8_t* b = (uint8_t*)Obj_Building(st);
                uint8_t* type = ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x20])(b);
                id = (type[0x8c] != 3) * 2 + 0x96;
                if (hit) ++id;
            }
            if ((pass == 0 || pass == 1) && hit) continue;
            if ((pass == 3 || pass == 4) && !hit) continue;
            marker(image(id), own ? 0x40000000 : 0x3f800000, px, pz);
        }
    }
    for (int32_t j = 0; j < Collection_Count(hook::Global<Collection*>(0x6cfcbc)); ++j) {
        Collection* trains = hook::Global<Collection*>(0x6cfcbc);
        uint8_t* t = (uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, j, 0));
        if (Obj_ByteF9(t) <= 0) continue;
        float px, pz;
        Train_RearPoint(t, 0, &px, &pz);
        if (!inBox(px, pz)) continue;
        double a = SceneObj_Angle2((uint8_t*)Obj_City41(t));
        int32_t dir = Ftol(((double)a / Fl(0x5c8768) + Fl(0x5c85b4)) * Fl(0x5c8898) + D(0x5c8c30)) & 7;
        void* sel = F<uint32_t>(m, 0x33) & 0x10000 ? SelectedTrain() : nullptr;
        dir += t == sel ? 0x50 : 0x5a;
        uint8_t* img = image(dir);
        float left = (float)F<int32_t>(m, 0xc3);
        float width = (float)(F<int32_t>(m, 0xcb) - F<int32_t>(m, 0xc3));
        float gxT = (float)(((double)px - left) * F<float>(m, 0x13) / width + F<float>(F<uint8_t*>(m, 0x86), 5) + F<float>(m, 0xb));
        double gyd = Obj_MapY(m, 0, pz);
        float gy = (float)(gyd - Slider_StepB(img) * r * D(0x5c8628));
        float gx = (float)((double)gxT - Slider_StepA(img) * r * D(0x5c8628));
        Glyph_Queue(img, 0, gx, gy, 0x3f800000, BitsF(rb), BitsF(rb), 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    }
    if (!F<uint32_t>(m, 0x113) || !Collection_IsValid(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(m, 0x113))) return;
    float px, pz;
    Obj_CenterCell(Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(m, 0x113)), 0, &px, &pz);
    if (!inBox(px, pz)) return;
    marker(image(0x3c), 0x40000000, px, pz);
}
RT3_REPLACE(0x478330, Minimap_DrawRouteMarkers);

// 0x478cb0. Minimap o: drags the camera with the mouse until the button is
// released (event 7): every mouse move (type 2, starting at the current
// position) stops following and moves the camera to the minimap point (the
// first move also refreshes the view, slot 4); the frame callback
// (0x6d4004) runs every 20 ms or when the queue is empty.
RT3_NOINLINE void __fastcall Minimap_DragCamera(uint8_t* o) {
    uint8_t* in = hook::Global<uint8_t*>(0x6d4018);
    InputEvent e;
    e.v[9] = F<uint32_t>(in, 0xa94);
    e.v[10] = F<uint32_t>(in, 0xa98);
    uint32_t type = 2;
    Input_Block(in);
    int32_t last = ClockMs();
    int32_t first = 1;
    do {
        if (type == 2) {
            float ox, oy;
            Minimap_WorldToPixel(o, 0, *(float*)&e.v[9], *(float*)&e.v[10], &ox, &oy);
            World_SetFollow(hook::Global<uint8_t*>(0x6cec78), 0, 0, nullptr);
            Camera_MoveToCell(hook::Global<uint8_t*>(0x62be68), 0, ox, oy);
            if (first) {
                uint8_t* cam = hook::Global<uint8_t*>(0x62be68);
                first = 0;
                ((void(__fastcall*)(uint8_t*))(*(void***)cam)[4])(cam);
            }
        }
        Engine_Nop();
        InputEvent tmp;
        e = *Input_NextEvent(hook::Global<uint8_t*>(0x6d4018), 0, &tmp);
        type = e.v[0];
        if (ClockMs() - last > 20 || !type) {
            if (auto fn = hook::Global<void(__cdecl*)()>(0x6d4004)) fn();
            last = ClockMs();
        }
    } while (type != 7);
    Input_Unblock(hook::Global<uint8_t*>(0x6d4018));
}
RT3_REPLACE(0x478cb0, Minimap_DragCamera);

// 0x478dc0. Fits the overview map (+0x33 flags 0x2000000 whole map / 0x30000 own
// area) to the cells marked 2 in the map grid (+0x2131): bounds +0xb3..+0xbf
// (widened by a margin, more when one of the player's stations is near the
// north-west corner of the area), centre +0xdf / +0xe3, fit zoom +0xd3 for the
// window (+0x11f cells), minimum zoom +0xd7 (snapped to 1 / 0.5 / 0.25) and the
// current zoom +0xdb kept between them.
RT3_NOINLINE void __fastcall Overview_FitArea(uint8_t* o) {
    uint32_t flags = F<uint32_t>(o, 0x33);
    if (!(flags & 0x2030000)) return;
    auto map = []() { return hook::Global<uint8_t*>(0x62c120); };
    int32_t& x0 = F<int32_t>(o, 0xb3);
    int32_t& y0 = F<int32_t>(o, 0xb7);
    int32_t& x1 = F<int32_t>(o, 0xbb);
    int32_t& y1 = F<int32_t>(o, 0xbf);
    if (flags & 0x2000000) {
        x0 = 0;
        y0 = 0;
        x1 = F<int32_t>(map(), 0x2155) - 1;
        y1 = F<int32_t>(map(), 0x2159) - 1;
        F<int32_t>(o, 0xdf) = Ftol((double)F<int32_t>(map(), 0x2155) * D(0x5c8628));
        F<int32_t>(o, 0xe3) = Ftol((double)F<int32_t>(map(), 0x2159) * D(0x5c8628));
    } else {
        x0 = F<int32_t>(map(), 0x2155) - 1;
        y0 = F<int32_t>(map(), 0x2159) - 1;
        x1 = 0;
        y1 = 0;
        for (int32_t y = F<int32_t>(map(), 0x2159) - 1; y >= 0; --y)
            for (int32_t x = F<int32_t>(map(), 0x2155) - 1; x >= 0; --x) {
                if (!(F<uint8_t*>(map(), 0x2131)[F<int32_t>(map(), 0x2155) * y + x] & 2)) continue;
                if (x < x0) x0 = x;
                if (y < y0) y0 = y;
                if (x > x1) x1 = x;
                if (y > y1) y1 = y;
            }
        int32_t near = 0;
        Collection* stations = hook::Global<Collection*>(0x6cec20);
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++i) {
            stations = hook::Global<Collection*>(0x6cec20);
            uint8_t* st = (uint8_t*)Collection_Get(stations, 0, Collection_IdAt(stations, 0, i, 0));
            int32_t me = Obj_Get21(hook::Global<uint8_t*>(0x6cec78));
            if (Obj_BuildingValue(st) != me) continue;
            float d = (float)Distance2D((float)Obj_BuildingCellX(st), (float)Obj_BuildingCellY(st), (float)x0, (float)y0);
            if (d < Fl(0x5c8898)) near = 1;
            else {
                d = (float)Distance2D((float)Obj_BuildingCellX(st), (float)Obj_BuildingCellY(st), (float)x0, (float)y1);
                if (d < Fl(0x5c8898)) near = 1;
            }
        }
        float grow = 1.0f;
        int32_t extra = 0;
        if (near) {
            float dy = (float)(y1 - y0);
            float d = (float)Distance2D((float)x0, (float)y0, (float)x1, (float)y1);
            if (d > Fl(0x5c8c28)) F<uint32_t>(&grow, 0) = 0x3fcccccd;
            else if (d > Fl(0x5c8990)) F<uint32_t>(&grow, 0) = 0x3fb33333;
            else if (d > Fl(0x5c8d4c)) F<uint32_t>(&grow, 0) = 0x3f99999a;
            if (dy > Fl(0x5c9aa8)) extra = Ftol((double)grow * Fl(0x5c8568));
        }
        float m = (float)((double)grow * Fl(0x5c873c));
        float v = (float)((double)(float)x0 - m - (float)extra);
        if (Fl(0x5c8598) > v) v = 0.0f;
        int32_t nx0 = Ftol(v);
        x0 = nx0;
        v = (float)((double)(float)y0 - m);
        if (Fl(0x5c8598) > v) v = 0.0f;
        int32_t ny0 = Ftol(v);
        y0 = ny0;
        float lim = (float)(F<int32_t>(map(), 0x2155) - 1);
        v = (float)((double)(float)x1 + m);
        int32_t nx1 = Ftol(lim < v ? lim : v);
        x1 = nx1;
        lim = (float)(F<int32_t>(map(), 0x2159) - 1);
        v = (float)((double)(float)y1 + m);
        int32_t ny1 = Ftol(lim < v ? lim : v);
        y1 = ny1;
        F<int32_t>(o, 0xdf) = Ftol((double)(nx1 + nx0) * D(0x5c8628));
        F<int32_t>(o, 0xe3) = Ftol((double)(ny1 + ny0) * D(0x5c8628));
    }
    int32_t mx = F<int32_t>(map(), 0x2159);
    if (F<int32_t>(map(), 0x2155) > mx) mx = F<int32_t>(map(), 0x2155);
    int32_t w = x1 - x0 + 1, h = y1 - y0 + 1, win = F<int32_t>(o, 0x11f);
    float fm = (float)mx;
    float z1 = (float)((double)fm / (float)(win > w ? win : w));
    float z2 = (float)((double)fm / (float)(win > h ? win : h));
    float fit = z1 < z2 ? z1 : z2;
    F<float>(o, 0xd3) = fit;
    float fw = (float)win;
    float z3 = (float)((double)fw / (float)(w < 1 ? 1 : w));
    float z4 = (float)((double)fw / (float)(h < 1 ? 1 : h));
    float zz = z3 > z4 ? z3 : z4;
    F<float>(o, 0xd7) = zz;
    zz = (float)(zz > D(0x5c8758) ? (double)zz : 0.25);
    F<float>(o, 0xd7) = zz;
    double cap = D(0x5ca9e8) / (double)mx;
    float mn = (float)(zz < cap ? (double)zz : cap);
    F<float>(o, 0xd7) = mn;
    if (mn > D(0x5c8888)) F<uint32_t>(o, 0xd7) = 0x3f800000;
    else if (mn > D(0x5c8c18) && mn < D(0x5c96e0)) F<uint32_t>(o, 0xd7) = 0x3f000000;
    else if (mn > D(0x5c8c20) && mn < D(0x5c88c8)) F<uint32_t>(o, 0xd7) = 0x3e800000;
    float cur = F<float>(o, 0xdb) < fit ? F<float>(o, 0xdb) : fit;
    F<float>(o, 0xdb) = cur;
    if (!(cur > F<float>(o, 0xd7))) F<float>(o, 0xdb) = F<float>(o, 0xd7);
}
RT3_REPLACE(0x478dc0, Overview_FitArea);

// 0x479510. Overview viewport: the visible cell rectangle (+0xc3..+0xcf) of the
// current zoom around the centre (+0xdf / +0xe3), kept on the map; its texture
// coordinates (+0xef..+0xfb within the map image span +0xff..+0x10b, kept off the
// edges); and the scroll limits for the centre (+0xe7 / +0xeb). Without an
// overview area the full image is shown.
RT3_NOINLINE void __fastcall Overview_UpdateView(uint8_t* o) {
    if (!(F<uint32_t>(o, 0x33) & 0x2030000)) {
        F<uint32_t>(o, 0xef) = 0;
        F<uint32_t>(o, 0xf3) = 0x3f800000;
        F<uint32_t>(o, 0xf7) = 0;
        F<uint32_t>(o, 0xfb) = 0x3f800000;
        return;
    }
    int32_t x0 = F<int32_t>(o, 0xb3), y0 = F<int32_t>(o, 0xb7), x1 = F<int32_t>(o, 0xbb), y1 = F<int32_t>(o, 0xbf);
    F<int32_t>(o, 0xcf) = y1;
    F<int32_t>(o, 0xcb) = x1;
    F<int32_t>(o, 0xc3) = x0;
    F<int32_t>(o, 0xc7) = y0;
    int32_t dw = x1 - x0, dh = y1 - y0;
    int32_t sz = Ftol(Obj_ScaledExtent(o));
    if (dw < sz) {
        double half = (double)(sz - dw) * D(0x5c8628);
        F<int32_t>(o, 0xc3) = Ftol((double)x0 - half);
        F<int32_t>(o, 0xcb) = Ftol((double)x1 + half + D(0x5c8880));
    }
    if (dh < sz) {
        double half = (double)(sz - dh) * D(0x5c8628);
        F<int32_t>(o, 0xc7) = Ftol((double)y0 - half);
        F<int32_t>(o, 0xcf) = Ftol((double)y1 + half + D(0x5c8880));
    }
    double cx = (double)F<int32_t>(o, 0xdf), half = (double)sz * D(0x5c8628);
    int32_t vx0 = Ftol(cx - half), vx1 = Ftol(half + cx);
    double cy = (double)F<int32_t>(o, 0xe3);
    int32_t vy0 = Ftol(cy - half), vy1 = Ftol(half + cy);
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    ClampSpan(vx0, vx1, F<int32_t>(m, 0x2155));
    ClampSpan(vy0, vy1, F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159));
    F<int32_t>(o, 0xc3) = vx0;
    F<int32_t>(o, 0xcb) = vx1;
    F<int32_t>(o, 0xc7) = vy0;
    F<int32_t>(o, 0xcf) = vy1;
    m = hook::Global<uint8_t*>(0x62c120);
    float sx = (float)(((double)F<float>(o, 0x103) - F<float>(o, 0xff)) / (float)F<int32_t>(m, 0x2155));
    float sy = (float)(((double)F<float>(o, 0x10b) - F<float>(o, 0x107)) / (float)F<int32_t>(m, 0x2159));
    float u0 = (float)((double)(float)vx0 * sx + F<float>(o, 0xff));
    F<float>(o, 0xef) = u0;
    float u1 = (float)((double)(float)vx1 * sx + F<float>(o, 0xff));
    F<float>(o, 0xf3) = u1;
    F<float>(o, 0xfb) = (float)((double)(float)(F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159) - vy0 - 1) * sy + F<float>(o, 0x107));
    F<float>(o, 0xf7) = (float)((double)(float)(F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2159) - vy1 - 1) * sy + F<float>(o, 0x107));
    F<float>(o, 0xef) = (float)(D(0x5c8678) > u0 ? 0.001 : (double)u0);
    F<float>(o, 0xf3) = (float)(D(0x5caa68) < u1 ? 0.999 : (double)u1);
    F<float>(o, 0xf7) = (float)(F<float>(o, 0xf7) < D(0x5c8678) ? 0.001 : (double)F<float>(o, 0xf7));
    F<float>(o, 0xfb) = (float)(F<float>(o, 0xfb) > D(0x5caa68) ? 0.999 : (double)F<float>(o, 0xfb));
    double lo = (double)vx0 + half - D(0x5c8770);
    F<int32_t>(o, 0xe7) = Ftol(cx > lo ? cx : lo);
    double hi = (double)vx1 - half + D(0x5c8770);
    F<int32_t>(o, 0xe7) = Ftol(cx < hi ? cx : hi);
    double ey = (double)F<int32_t>(o, 0xeb);
    lo = (double)vy0 + half - D(0x5c8770);
    int32_t e = Ftol(ey > lo ? ey : lo);
    F<int32_t>(o, 0xeb) = e;
    double ed = (double)e;
    hi = (double)vy1 - half + D(0x5c8770);
    F<int32_t>(o, 0xeb) = Ftol(ed < hi ? ed : hi);
}
RT3_REPLACE(0x479510, Overview_UpdateView);

// 0x479b50. Picks the building (0x6cec20; active ones at 0x5c8880 of their
// distance) nearest to (x, y) within the minimap's pick radius (+0xcb - +0xc3);
// failing that and with option 4 (0x6d4018 +0xa8c), the nearest 0x6cfca8 object
// within half of it (*other = its +0x202).
RT3_NOINLINE int32_t __fastcall Minimap_PickNearest(uint8_t* o, int, float x, float y, uint32_t* building, uint32_t* other) {
    *building = 0;
    *other = 0;
    float r = (float)((double)(F<int32_t>(o, 0xcb) - F<int32_t>(o, 0xc3)) / D(0x5cab00) * D(0x5c85c8));
    int32_t all = F<int32_t>(hook::Global<uint8_t*>(0x6d4018), 0xa8c) & 4;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++i) {
        Collection* c = hook::Global<Collection*>(0x6cec20);
        uint8_t* s = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        float cx = Obj_NodeCenterX(s);
        float cy = Obj_NodeCenterY(s);
        float d = (float)Distance2D(x, y, cx, cy);
        if (Obj_BuildingStateActive(s)) d = (float)((double)d * D(0x5c8880));
        else if (!all) continue;
        if (d < r) {
            r = d;
            *building = F<uint32_t>(s, 0);
        }
    }
    if (*building) return 1;
    r = (float)((double)r * D(0x5c8750));
    if (all) {
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfca8)); ++i) {
            Collection* c = hook::Global<Collection*>(0x6cfca8);
            uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
            float cx, cy;
            Obj_CenterCell(t, 0, &cx, &cy);
            float d = (float)Distance2D(x, y, cx, cy);
            if (d < r) {
                r = d;
                *other = F<uint32_t>(t, 0x202);
            }
        }
    }
    return *other != 0 ? 1 : 0;
}
RT3_REPLACE(0x479b50, Minimap_PickNearest);

// 0x479d30. Minimap o: a click at (x, y) adds an order to the selected
// train's list (flag +0x33 0x10000) or to the consist editor's (0x20000):
// the station picked there (Minimap_PickNearest), else a waypoint at its
// track node. For a train the stop must be on track connected to the train
// (else message 0xc12 for a station, 0xc13 for a waypoint) and the train must
// belong to the player (else 0x250 with its company's name). The new order
// (wait 900, load 4, +0x2f 0x3e395810) goes to the host in multiplayer when
// this is not the host's consist editor (0x6d3ae4); otherwise it is inserted
// at the end, or at the selected row of the order list (0x2aff in the editor,
// 0x65a3/0x65a4 in the train dialog) with shift/ctrl (0x6d4018+0xa8c). If the
// train can't drive the orders any more, it stops (state 10, or 19 at a
// station it can still leave). Then the click sounds (0x38).
RT3_NOINLINE void __fastcall Minimap_AddOrder(uint8_t* o, int, float x, float y) {
    float px, py;
    Minimap_WorldToPixel(o, 0, x, y, &px, &py);
    uint32_t station, node;
    if (!Minimap_PickNearest(o, 0, px, py, &station, &node)) return;
    auto sel = [&]() -> uint8_t* { return (F<uint32_t>(o, 0x33) & 0x10000) ? (uint8_t*)SelectedTrain() : nullptr; };
    uint32_t* list;
    if (F<uint32_t>(o, 0x33) & 0x10000) {
        if (!SelectedTrain()) return;
        list = (uint32_t*)Identity(SelectedTrain());
    } else if (F<uint32_t>(o, 0x33) & 0x20000) {
        list = (uint32_t*)Get6d3ae8b();
    } else {
        return;
    }
    if (!list) return;
    Collection* pieces = hook::Global<Collection*>(0x6cfca8);
    if ((F<uint32_t>(o, 0x33) & 0x10000) && SelectedTrain()) {
        uint32_t at = node;
        if (station) at = F<uint32_t>(Collection_Get(hook::Global<Collection*>(0x6cec20), 0, station), 8);
        uint32_t trk = F<uint32_t>(sel(), 0x49);
        if (Collection_IsValid(pieces, 0, at) && Collection_IsValid(pieces, 0, trk) && !Node_SameComponent((uint8_t*)Collection_Get(pieces, 0, at), 0, trk)) {
            Dialog_RunMessage(GetString(station ? 0xc12 : 0xc13), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            return;
        }
    }
    uint8_t e[0x33] = {};
    F<int32_t>(e, 0x24) = 0x384;
    e[0x23] = 4;
    e[0x22] = 0;
    uint8_t flag = e[0x28];
    if (station) {
        flag &= 0x7f;
        F<uint16_t>(e, 0x29) = (uint16_t)station;
    } else {
        flag |= 0x80;
        F<uint32_t>(e, 0x2b) = node;
    }
    e[0x28] = flag;
    F<uint32_t>(e, 0x2f) = 0x3e395810;
    if ((F<uint32_t>(o, 0x33) & 0x10000) && SelectedTrain()) {
        int32_t company = F<int32_t>(sel(), 0x51);
        if (company != Obj_Get21(hook::Global<void*>(0x6cec78))) {
            uint8_t* co = (uint8_t*)Train_Company(sel());
            char buf[0x200];
            FormatText(buf, GetString(0x250), co ? (const char*)co + 4 : (const char*)0x5c87a8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            Dialog_RunMessage(buf, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            return;
        }
    }
    if (IsMultiplayer() && !hook::Global<uint32_t>(0x6d3ae4)) {
        uint8_t* msg = ((uint8_t*(__cdecl*)(size_t))0x5a125d)(0x3b);
        F<int32_t>(msg, 0) = Global6d40e4();
        F<int32_t>(msg, 4) = SelectedTrainId();
        memcpy(msg + 8, e, 0x33);
        Net_SendToHost(hook::Global<uint8_t*>(0x6cd8d8), 0, 0x41, 0x3b, msg);
        ((void(__cdecl*)(void*))0x5a1145)(msg);
    } else {
        int32_t idx = (int32_t)list[1];
        if (hook::Global<uint8_t*>(0x6d4018)[0xa8c] & 3) {
            uint8_t* w = hook::Global<uint32_t>(0x6d3ae4)
                             ? Widget_FindChild(hook::Global<uint8_t*>(0x6d3ac8), 0, 0x2aff)
                             : Widget_FindChild(hook::Global<uint8_t*>(0x6d3b20), 0, hook::Global<uint32_t>(0x6d3b24) ? 0x65a3 : 0x65a4);
            idx = F<int32_t>(w, 0x1d6);
        }
        if (idx < 0) idx = 0;
        TrainOrders_Insert((uint8_t*)list, 0, idx, e);
        int32_t trk = (F<uint32_t>(o, 0x33) & 0x10000) && SelectedTrain() ? F<int32_t>(sel(), 0x49) : -1;
        if (!Orders_Drivable(list, 0, sel(), trk, 0, 0, -1) && (F<uint32_t>(o, 0x33) & 0x10000) && SelectedTrain()) {
            int32_t state = 0xa;
            if ((F<uint32_t>(o, 0x33) & 0x10000) && SelectedTrain() && Obj_CityKindIs2(Obj_SelectedTrainIfFlag(o))) {
                int32_t start = F<int32_t>(Obj_SelectedTrainIfFlag(o), 0x49);
                if (Orders_Drivable(list, 0, (uint8_t*)Obj_SelectedTrainIfFlag(o), start, 0, 1, -1)) state = 0x13;
            }
            Train_SetState((uint8_t*)Obj_SelectedTrainIfFlag(o), 0, state, 0.0f);
        }
    }
    Ui_PlaySound(0x38, 0, 0x3f000000);
}
RT3_REPLACE(0x479d30, Minimap_AddOrder);

// 0x47a120. Renders the minimap into the w x h pixels `dst`, the map
// (0x62c120) centred and scaled to fit (its extent in texture coordinates
// to u0..v1 when asked): terrain from the map texture `tex` (srcW x srcH;
// taken from the terrain when `haveTex` is 0, then committed back) shaded
// by the height slope, water 41/64/96, coast 31/59/86, greyed toward the
// average for flags 0x2030000; or with flag 0x400000 the political map,
// each region in its owning territory's colour (lighter for the player
// `player`, owners in the selected company's sphere tinted) with black
// borders. Then the track layer (0x6cfca8: own / rival, double), joined
// up with lines when zoomed in, and the stations (0x62bae0) as white
// crosses. In the editor (0x6ceba0) the map is kept and only tracks are
// drawn over it.
RT3_NOINLINE void __fastcall Minimap_Render(uint32_t* dst, int32_t w, int32_t h, uint32_t flags, int32_t player, int32_t haveTex, uint32_t* tex, int32_t srcW,
                               int32_t srcH, float* u0, float* u1, float* v0, float* v1) {
    int32_t madeTex = 0;
    bool editor = hook::Global<int32_t>(0x6ceba0) != 0;
    if (editor) {
        Render_GetSize4((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024)), 0, &srcW, &srcH);
    } else if (!haveTex) {
        haveTex = 1;
        madeTex = 1;
        tex = Terrain_ExtractTexture((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024)), 0, 0.0f, 0.0f, 1.0f, 1.0f, nullptr, nullptr);
        Render_GetSize4((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024)), 0, &srcW, &srcH);
    }
    if (!tex) {
        if (!hook::Global<int32_t>(0x6ceba0)) return;
    } else if (!hook::Global<int32_t>(0x6ceba0)) {
        Mem_Fill32(dst, 0xff000000, (uint32_t)(w * h));
    }
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if (!map) return;
    uint32_t political = flags & 0x400000;
    uint32_t colours[100], owners[100];
    if (political) {
        memset(colours, 0, sizeof colours);
        memset(owners, 0, sizeof owners);
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfc9c)); ++i) {
            uint32_t* r = (uint32_t*)Collection_Get(hook::Global<Collection*>(0x6cfc9c), 0, Collection_IdAt(hook::Global<Collection*>(0x6cfc9c), 0, (uint32_t)i, 0));
            owners[r[0]] = F<int32_t>(r, 0x31) ? r[0] : F<uint32_t>(r, 0x35);
        }
        void* mine = SelectedCompany();
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfc9c)); ++i) {
            uint32_t id = Collection_IdAt(hook::Global<Collection*>(0x6cfc9c), 0, (uint32_t)i, 0);
            uint32_t mask = owners[id] != (uint32_t)player ? 0 : 0x323232;
            if (mine && Company_EntryFlag(mine, 0, (int32_t)owners[id])) colours[id] = mask + 0xff28b111;
            else colours[id] = mask + 0xffc7808d;
        }
    }
    int32_t tw = F<int32_t>(map, 0x2155), th = F<int32_t>(map, 0x2159);
    float ftw = (float)tw;
    float fsrcW = (float)srcW;
    float sx = fsrcW / ftw;
    float fth = (float)th;
    float fsrcH = (float)srcH;
    float sy = fsrcH / fth;
    float big = (float)(tw > th ? tw : th);
    float fw = (float)w;
    float stepX = big / fw;
    float fh = (float)h;
    float stepY = big / fh;
    int32_t offX = (w - Ftol((double)ftw / stepX)) / 2;
    int32_t offY = (h - Ftol((double)fth / stepY)) / 2;
    float innerW = (float)(w - offX * 2);
    float scaleX = innerW / ftw;
    float innerH = (float)(h - offY * 2);
    float scaleY = innerH / fth;
    if (u0) {
        float fo = (float)offX;
        float a = fo / fw;
        *u0 = a;
        *u1 = (float)((double)ftw / stepX / fw + a);
        fo = (float)offY;
        float b = fo / fh;
        *v0 = b;
        *v1 = (float)((double)fth / stepY / fh + b);
    }
    float fx = 0, fy = 0, cb = 0, cg = 0, cr = 0;
    if (!haveTex) {
        if (!hook::Global<int32_t>(0x6ceba0)) goto done;
    } else if (!hook::Global<int32_t>(0x6ceba0)) {
        int32_t y = h - offY - 1;
        if (y > offY) {
            int32_t row = y * w;
            do {
                int32_t my = Ftol(fy);
                if (my >= th) break;
                for (int32_t x = offX; x < w; ++x) {
                    int32_t mx = Ftol(fx);
                    if (mx >= tw) break;
                    if (political) {
                        if (hook::Global<int32_t>(0x6ceba0)) goto colour;
                        uint32_t c;
                        if (Terrain_KindIs1Or4(hook::Global<uint8_t*>(0x62c120), 0, mx, my)) c = 0xff8fa9ce;
                        else if (Terrain_KindIs3Or5(hook::Global<uint8_t*>(0x62c120), 0, mx, my)) c = 0xff7b95ba;
                        else {
                            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
                            int32_t mw = F<int32_t>(m, 0x2155);
                            const uint8_t* cell = F<uint8_t*>(m, 0x2131) + mw * my;
                            uint32_t o = owners[cell[mx] >> 3];
                            if ((mx > 0 && owners[cell[mx - 1] >> 3] != o) || (my > 0 && owners[cell[mx - mw] >> 3] != o)) c = 0xff000000;
                            else c = colours[o];
                        }
                        dst[row + x] = c;
                        goto next;
                    }
                    if (hook::Global<int32_t>(0x6ceba0)) goto colour;
                    if (Terrain_KindIs1Or4(hook::Global<uint8_t*>(0x62c120), 0, mx, my)) {
                        cr = 41.0f;
                        cg = 64.0f;
                        cb = 96.0f;
                    } else if (Terrain_KindIs3Or5(hook::Global<uint8_t*>(0x62c120), 0, mx, my) ||
                               Map_Bit9NearWater(hook::Global<uint8_t*>(0x62c120), 0, mx, my)) {
                        cr = 31.0f;
                        cg = 59.0f;
                        cb = 86.0f;
                    } else {
                        int32_t h0 = Ftol(Map_GridFloat(hook::Global<uint8_t*>(0x62c120), 0, mx, my));
                        int32_t d;
                        if (mx > 0 && my > 0) d = Ftol(Map_GridFloat(hook::Global<uint8_t*>(0x62c120), 0, mx - 1, my - 1)) - h0 + 0x7f;
                        else d = h0 - Ftol(Map_GridFloat(hook::Global<uint8_t*>(0x62c120), 0, mx + 1, my + 1)) + 0x7f;
                        if (d < 0) d = 0;
                        else if (d > 0xff) d = 0xff;
                        float shade = (float)((double)d / D(0x5ca9e8));
                        shade = (float)(shade * D(0x5c8880) + D(0x5c88e8));
                        float fmy = (float)my;
                        int32_t idx = Ftol((double)fmy * sy) * srcW;
                        float fmx = (float)mx;
                        idx += Ftol((double)fmx * sx);
                        uint32_t px = tex[idx];
                        cr = (float)(int32_t)((px >> 16) & 0xff);
                        cg = (float)(int32_t)((px >> 8) & 0xff);
                        cb = (float)(int32_t)(px & 0xff);
                        cr = shade * cr;
                        cg = shade * cg;
                        cb = shade * cb;
                    }
                colour:
                    if (flags & 0x2030000) {
                        float avg = (float)(((double)cb + cg + cr) / Fl(0x5c8900));
                        cr = (float)(((double)cr - avg) * Fl(0x5ce83c) + avg);
                        cg = (float)(((double)cg - avg) * Fl(0x5ce83c) + avg);
                        cb = (float)(((double)cb - avg) * Fl(0x5ce83c) + avg);
                        if (cr < Fl(0x5c8598)) cr = 0;
                        if (cg < Fl(0x5c8598)) cg = 0;
                        if (cb < Fl(0x5c8598)) cb = 0;
                    }
                    if (cr > Fl(0x5ca0ec)) cr = 255.0f;
                    if (cg > Fl(0x5ca0ec)) cg = 255.0f;
                    if (cb > Fl(0x5ca0ec)) cb = 255.0f;
                    {
                        uint32_t c = ((uint32_t)Ftol(cr) | 0xffffff00) << 8;
                        c = (c | (uint32_t)Ftol(cg)) << 8;
                        dst[row + x] = c | (uint32_t)Ftol(cb);
                    }
                next:
                    fx = (float)(fx + stepX);
                }
                fy = (float)(fy + stepY);
                row -= w;
                --y;
                fx = 0;
            } while (y > offY);
        }
    }
    if (political) goto done;
    {
        bool zoomed = scaleX > Fl(0x5c85b4) || scaleY > Fl(0x5c85b4);
        int32_t total = w * h;
        uint8_t* m = hook::Global<uint8_t*>(0x62c120);
        int32_t mh = F<int32_t>(m, 0x2159), mw = F<int32_t>(m, 0x2155);
        int32_t lastRow = mh - 1;
        uint8_t* buf = nullptr;
        if (zoomed) {
            buf = ((uint8_t*(__cdecl*)(uint32_t))0x5a125d)((uint32_t)(mw * mh));
            memset(buf, 0, (size_t)(mw * mh));
        }
        const uint8_t* cell = F<uint8_t*>(hook::Global<uint8_t*>(0x62c120), 0x2131);
        if (mh > 0) {
            int32_t flip = mh - 1;
            uint8_t* brow = buf + (mh - 1) * mw;
            for (int32_t y = 0; y < mh; ++y, brow -= mw, --flip) {
                for (int32_t x = 0; x < mw; ++x, ++cell) {
                    int32_t owner, dbl;
                    if (!(*cell & 6) || !Map_TrackOwnerAt(hook::Global<void*>(0x6cfca8), 0, x, y, &owner, &dbl)) continue;
                    int32_t me = Obj_Get21(hook::Global<void*>(0x6cec78));
                    int32_t kind = (owner == me ? 0 : 2) + (dbl != 0 ? 1 : 0) + 1;
                    if (zoomed) {
                        brow[x] = (uint8_t)kind;
                        continue;
                    }
                    float ff = (float)flip;
                    float rowStart = (float)((Ftol((double)ff * scaleY) + offY) * w);
                    float fxx = (float)x;
                    float fo = (float)offX;
                    dst[Ftol((double)fxx * scaleX + rowStart + fo)] = hook::Global<uint32_t>(0x5f2c34 + kind * 4);
                }
            }
        }
        if (zoomed) {
            const uint8_t* p = buf;
            for (int32_t y = 0; y < mh; ++y) {
                for (int32_t x = 0; x < mw; ++x, ++p) {
                    uint8_t k = *p;
                    if (!k) continue;
                    float fox = (float)offX, fxx = (float)x;
                    int32_t px0 = Ftol((double)fxx * scaleX + fox);
                    float foy = (float)offY, fyy = (float)y;
                    int32_t py0 = Ftol((double)fyy * scaleY + foy);
                    dst[py0 * w + px0] = hook::Global<uint32_t>(0x5f2c34 + k * 4);
                    if (!(y > 0 && y < lastRow && x > 0 && x < mw - 1)) continue;
                    for (uint32_t n = 0; n < 0x10; n += 4) {
                        int32_t dy = hook::Global<int32_t>(0x624b48 + n), dx = hook::Global<int32_t>(0x624b28 + n);
                        if (!p[dy * mw + dx]) continue;
                        float fnx = (float)(dx + x);
                        int32_t ddx = Ftol((double)fnx * scaleX + fox) - px0;
                        float fny = (float)(dy + y);
                        int32_t ddy = Ftol((double)fny * scaleY + foy) - py0;
                        if (ddx <= 1 && ddy <= 1) continue;
                        int32_t steps = ddx > ddy ? ddx : ddy;
                        float fsteps = (float)steps;
                        float fdx = (float)ddx;
                        float stx = fdx / fsteps;
                        float fdy = (float)ddy;
                        float sty = fdy / fsteps;
                        float bx = (float)px0, by = (float)py0;
                        for (int32_t i = 1; i < steps; ++i) {
                            float fi = (float)i;
                            int32_t py = Ftol((double)fi * sty + by);
                            int32_t pxx = Ftol((double)fi * stx + bx);
                            dst[py * w + pxx] = hook::Global<uint32_t>(0x5f2c34 + *p * 4);
                        }
                    }
                }
            }
        }
        if (buf) ((void(__cdecl*)(void*))0x5a1145)(buf);
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62bae0)); ++i) {
            uint8_t* s = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, Collection_IdAt(hook::Global<Collection*>(0x62bae0), 0, (uint32_t)i, 0));
            if (F<int32_t>(s, 0x23e)) continue;
            float fo = (float)offX;
            int32_t px = Ftol(Obj_CellX(s) * scaleX + fo);
            float fl = (float)lastRow;
            int32_t idx = (Ftol(((double)fl - Obj_CellY(s)) * scaleY) + offY) * w + px;
            if (idx >= 0 && idx < total) dst[idx] = 0xffffffff;
            if (idx + 1 >= 0 && idx + 1 < total && px < w - 1) dst[idx + 1] = 0xffffffff;
            if (idx - 1 >= 0 && idx - 1 < total && px > 0) dst[idx - 1] = 0xffffffff;
            if (idx + w >= 0 && idx + w < total) dst[idx + w] = 0xffffffff;
            if (idx - w >= 0 && idx - w < total) dst[idx - w] = 0xffffffff;
        }
    }
done:
    if (madeTex) Terrain_CommitTexture((uint8_t*)Engine_Get1C(hook::Global<void*>(0x6d4024)), 0, 1);
}

// The original is fastcall with dst in ECX and w in EDX.
RT3_REPLACE(0x47a120, Minimap_Render);

// 0x47b070 / 0x47b0d0. Map view widget (vtable 0x5ce840): one byte at +0xa2.
uint8_t* __fastcall MapView_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5ce840;
    MapView_Init(w);
    uint8_t* rec = Widget_ReadRecord(w, 0x60);
    w[0xa2] = rec[0x5f];
    Free(rec);
    return w;
}
RT3_REPLACE(0x47b070, MapView_Load);

uint8_t* __fastcall MapView_Construct(uint8_t* w, int, const uint8_t* def) {
    Widget_Construct(w, 0, def);
    F<uint32_t>(w, 0) = 0x5ce840;
    MapView_Init(w);
    w[0xa2] = def[0x5f];
    F<uint16_t>(w, 6) = 0x1000;
    return w;
}
RT3_REPLACE(0x47b0d0, MapView_Construct);

// 0x47b110. Map widget destructor: releases its pack image (+0xaf) and frees its
// texture (+0xab).
void __fastcall MapWidget_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5ce840;
    if (F<void*>(w, 0xaf)) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, F<void*>(w, 0xaf));
    F<void*>(w, 0xaf) = nullptr;
    if (uint8_t* t = F<uint8_t*>(w, 0xab)) {
        Texture_Destroy(t);
        ExeDelete(t);
        F<void*>(w, 0xab) = nullptr;
    }
    Widget_Destruct(w);
}
RT3_REPLACE(0x47b110, MapWidget_Destruct);

// 0x47b170 (vtable). Draws minimap widget w (with a map): clips its box to
// the unit square into a scissor (0x6d4024 type 5) when it is not empty,
// updates the view, draws its picture (+0xab, uv +0xef..+0xfb, faded), then
// the route markers (modes 0x30000), the city marker (0x2000000) or the view
// box unless locked (0x400000), the debug frames, and ends the scissor.
void __fastcall MinimapWidget_Draw(uint8_t* w) {
    if (!Widget_IsVisible(w) || !hook::Global<uint8_t*>(0x62c120)) return;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    float v = (float)((double)F<float>(w, 0xb) + F<float>(p, 5));
    const float x0 = Fl(0x5c8598) > v ? 0.0f : v;
    v = (float)((double)F<float>(p, 9) + F<float>(w, 0xf));
    const float y0 = Fl(0x5c8598) > v ? 0.0f : v;
    v = (float)((double)F<float>(w, 0xb) + F<float>(p, 5) + F<float>(w, 0x13));
    const float x1 = Fl(0x5c85b4) < v ? 1.0f : v;
    v = (float)((double)F<float>(p, 9) + F<float>(w, 0x17) + F<float>(w, 0xf));
    const float y1 = Fl(0x5c85b4) < v ? 1.0f : v;
    bool clip = false;
    if (!(x0 >= x1) && !(y0 >= y1)) {
        Engine_QueueType5(hook::Global<void*>(0x6d4024), 0, Bits(x0), Bits(y0), Bits(x1), Bits(y1));
        clip = true;
    }
    Overview_UpdateView(w);
    p = F<uint8_t*>(w, 0x86);
    const float fade = ((float(__cdecl*)())(void*)&Fade_Step)();
    const float sy1 = (float)((double)F<float>(p, 9) + F<float>(w, 0x17) + F<float>(w, 0xf));
    const float sx1 = (float)((double)F<float>(w, 0xb) + F<float>(p, 5) + F<float>(w, 0x13));
    const float sy0 = (float)((double)F<float>(p, 9) + F<float>(w, 0xf));
    const float sx0 = (float)((double)F<float>(w, 0xb) + F<float>(p, 5));
    Engine_QueueSprite(hook::Global<void*>(0x6d4024), 0, Bits(sx0), Bits(sy0), Bits(sx1), Bits(sy1), Bits(fade), 0xffffffff, F<uint32_t>(w, 0xab),
                       F<uint32_t>(w, 0xef), F<uint32_t>(w, 0xf7), F<uint32_t>(w, 0xf3), F<uint32_t>(w, 0xfb), 0x3f800000);
    const uint32_t style = F<uint32_t>(w, 0x33);
    if (!(style & 0x400000)) {
        if (style & 0x30000)
            Minimap_DrawRouteMarkers(w);
        else if (style & 0x2000000)
            MapWidget_DrawCityMarker(w);
        else
            MiniMap_DrawView(w);
    }
    if (F<uint32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc79) & F<uint16_t>(w, 6)) {
        p = F<uint8_t*>(w, 0x86);
        Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0xb)), (float)((double)F<float>(p, 9) + F<float>(w, 0xf)), F<float>(w, 0x13),
                     F<float>(w, 0x17), 1.0f, 3);
        if (!(F<float>(w, 0x23) == Fl(0x5c8598))) {
            p = F<uint8_t*>(w, 0x86);
            Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0x1b)), (float)((double)F<float>(p, 9) + F<float>(w, 0x1f)),
                         F<float>(w, 0x23), F<float>(w, 0x27), 1.0f, 3);
        }
    }
    if (clip) Engine_QueueType6(hook::Global<void*>(0x6d4024));
}
RT3_REPLACE(0x47b170, MinimapWidget_Draw);

// 0x47b3d0. Re-renders the minimap widget m into its texture (+0xab): size
// +0xa3 x +0xa7, flags +0x33, player +0x117, source texture (haveTex, tex,
// srcW, srcH) and the UV rect +0xff..+0x10b.
RT3_NOINLINE void __fastcall MinimapWidget_Refresh(uint8_t* m, int, int32_t haveTex, uint32_t* tex, int32_t srcW, int32_t srcH) {
    if (!F<uint8_t*>(m, 0xab)) return;
    uint32_t* px = Texture_Pixels32(F<uint8_t*>(m, 0xab), 0, nullptr, nullptr, 0);
    Minimap_Render(px, F<int32_t>(m, 0xa3), F<int32_t>(m, 0xa7), F<uint32_t>(m, 0x33), F<int32_t>(m, 0x117), haveTex, tex, srcW, srcH,
                   (float*)(m + 0xff), (float*)(m + 0x103), (float*)(m + 0x107), (float*)(m + 0x10b));
    Texture_SetPixels(F<uint8_t*>(m, 0xab), 0, px);
}
RT3_REPLACE(0x47b3d0, MinimapWidget_Refresh);

// 0x47b450. Deleting destructor of the map widget.
uint8_t* __fastcall MapWidget_DeletingDestructor(uint8_t* w, int, uint32_t flags) {
    MapWidget_Destruct(w);
    if (flags & 1) ExeDelete(w);
    return w;
}
RT3_REPLACE(0x47b450, MapWidget_DeletingDestructor);

// 0x47b470. The minimap widget m's texture (+0xab), created on first use at
// w x h, or (h -1) at the smallest power of two from 64 up to the device
// limits (engine +0x11423b / +0x11423f) covering the widget's share
// (+0x13 / +0x17) of the screen (times 0x5c8640); then rendered.
RT3_NOINLINE uint8_t* __fastcall MinimapWidget_Texture(uint8_t* m, int, int32_t w, int32_t h) {
    if (F<uint8_t*>(m, 0xab)) return F<uint8_t*>(m, 0xab);
    if (h == -1) {
        uint8_t* eng = hook::Global<uint8_t*>(0x6d4024);
        uint8_t* vp = F<uint8_t*>(eng, 0x34) + F<int32_t>(eng, 0x11421e) * 0x15;
        float fw = (float)F<int32_t>(vp, 0);
        int32_t pw = Ftol((double)fw * F<float>(m, 0x13));
        float fh = (float)F<int32_t>(vp, 4);
        int32_t ph = Ftol((double)fh * F<float>(m, 0x17));
        double needH = (double)ph * D(0x5c8640);
        h = 0x40;
        w = 0x40;
        if (D(0x5ce880) < needH) {
            int32_t maxH = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11423f);
            while (h < maxH) {
                h <<= 1;
                if (!((double)h < needH)) break;
            }
        }
        double needW = (double)pw * D(0x5c8640);
        if (D(0x5ce880) < needW) {
            int32_t maxW = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11423b);
            while (w < maxW) {
                w <<= 1;
                if (!((double)w < needW)) break;
            }
        }
    }
    F<int32_t>(m, 0xa3) = w;
    F<int32_t>(m, 0xa7) = h;
    uint8_t* t = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0xec);
    t = t ? Texture_InitMemory(t, 0, nullptr, w, h, 1) : nullptr;
    F<uint8_t*>(m, 0xab) = t;
    MinimapWidget_Refresh(m, 0, 0, nullptr, 0, 0);
    return F<uint8_t*>(m, 0xab);
}
RT3_REPLACE(0x47b470, MinimapWidget_Texture);

// 0x47b5e0. Mouse over minimap widget m at (x, y): with +0x33 0x400000 it
// tracks the region under the cell (+0x117, redrawing on change; none over
// water / kinds 1, 3, 4, 5), otherwise the nearest building / other object
// (+0x10f / +0x113); then the widget hover handling.
void __fastcall MinimapWidget_OnHover(uint8_t* m, int, float x, float y, int32_t moved, int32_t track) {
    union {
        float f;
        uint32_t u;
        int32_t i;
    } px, py;
    Minimap_WorldToPixel(m, 0, x, y, &px.f, &py.f);
    if (F<uint32_t>(m, 0x33) & 0x400000) {
        int32_t cx = Ftol(px.f), cy = Ftol(py.f);
        uint32_t region = 0;
        uint8_t* map = hook::Global<uint8_t*>(0x62c120);
        if (cx >= 0 && cx < F<int32_t>(map, 0x2155) && cy >= 0 && cy < F<int32_t>(map, 0x2159) && !Terrain_KindIs1Or4(map, 0, cx, cy) &&
            !Terrain_KindIs3Or5(hook::Global<uint8_t*>(0x62c120), 0, cx, cy))
            region = *(uint32_t*)Map_RegionRecordAt(hook::Global<uint8_t*>(0x62c120), 0, (float)cx, (float)cy);
        if (region != F<uint32_t>(m, 0x117)) {
            F<uint32_t>(m, 0x117) = region;
            MinimapWidget_Refresh(m, 0, 0, nullptr, 0, 0);
        }
    } else {
        int32_t hit = Minimap_PickNearest(m, 0, px.f, py.f, &py.u, &px.u);
        F<uint32_t>(m, 0x113) = 0;
        F<uint32_t>(m, 0x10f) = 0;
        if (hit) {
            F<uint32_t>(m, 0x10f) = py.u;
            F<uint32_t>(m, 0x113) = px.u;
        }
    }
    Widget_OnHover(m, 0, x, y, moved, track);
}
RT3_REPLACE(0x47b5e0, MinimapWidget_OnHover);

 // namespace

// 0x47b720 (vtable). Minimap widget events: clicks inside run the click
// callback, or (no callback) add a route order (route modes 0x30000) or drag
// the camera, unless locked (0x400000); 0xba / 0xbb rebuild its picture
// (0x6ceba0 marks 0xbb) and refit; 0xbc pans the view (+0xdf / +0xe3 from
// +0xe7 / +0xeb) by the given steps x the scaled extent (returning 0); 0xad
// zooms (+0xdb) within +0xd7..+0xd3. All other events, and these, go to the
// widget default.
int32_t __fastcall MinimapWidget_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    switch (msg[0]) {
    case 6:
        if (!WidgetSlot453(w, 0x30) || !Widget_IsEnabled(w) || !WidgetHit453(w, msg[9], msg[10])) break;
        if (F<void*>(w, 0x9a)) {
            WidgetClickCb453(w, msg);
        } else if (!(F<uint32_t>(w, 0x33) & 0x400000)) {
            if (F<uint32_t>(w, 0x33) & 0x30000)
                Minimap_AddOrder(w, 0, *(const float*)&msg[9], *(const float*)&msg[10]);
            else
                Minimap_DragCamera(w);
        }
        break;
    case 0xba:
    case 0xbb:
        if (msg[1] != F<uint16_t>(w, 4)) break;
        hook::Global<int32_t>(0x6ceba0) = msg[0] == 0xbb ? 1 : 0;
        if (!F<uint32_t>(w, 0xab))
            MinimapWidget_Texture(w, 0, -1, -1);
        else
            MinimapWidget_Refresh(w, 0, 0, nullptr, 0, 0);
        hook::Global<int32_t>(0x6ceba0) = 0;
        Overview_FitArea(w);
        break;
    case 0xbc: {
        if (msg[1] != F<uint16_t>(w, 4)) break;
        const int32_t baseX = F<int32_t>(w, 0xe7);
        F<int32_t>(w, 0xdf) = baseX;
        const int32_t baseY = F<int32_t>(w, 0xeb);
        F<int32_t>(w, 0xe3) = baseY;
        const float sx = Obj_ScaledExtent(w);
        const float dx = (float)(int32_t)msg[2];
        F<int32_t>(w, 0xdf) = Ftol((double)sx * dx * D(0x5c8c20) + (double)baseX);
        const float sy = Obj_ScaledExtent(w);
        const float dy = (float)(int32_t)msg[3];
        F<int32_t>(w, 0xe3) = Ftol((double)sy * dy * D(0x5c8c20) + (double)baseY);
        return 0;
    }
    case 0xad: {
        if (msg[1] != F<uint16_t>(w, 4)) break;
        const float z = (float)(((double)(int32_t)msg[2] / D(0x5c8d18) + D(0x5c8770)) * F<float>(w, 0xdb));
        F<float>(w, 0xdb) = z;
        if (z < F<float>(w, 0xd7)) F<float>(w, 0xdb) = F<float>(w, 0xd7);
        if (F<float>(w, 0xdb) > F<float>(w, 0xd3)) F<float>(w, 0xdb) = F<float>(w, 0xd3);
        break;
    }
    }
    return Widget_HandleEvent(w, 0, (uint8_t*)msg);
}
RT3_REPLACE(0x47b720, MinimapWidget_HandleEvent);

RT3_NOINLINE void ClearGlobals6ceba4() {
    for (int i = 0; i < 10; ++i) ((int32_t*)0x6ceba4)[i] = 0;
}
RT3_REPLACE(0x47ba60, ClearGlobals6ceba4);

// 0x47ba80. Replaces map pointer model `slot` (0x6ceba4) with a new one of
// `kind` (0 "pointer", 1 "pointer2", else "pointer3") at (x, z), lifted by
// `lift` above the ground, scaled 0.8 and shown (+0x20 bit 5 cleared).
// QUIRK: a failed allocation still uses the null model (crash).
RT3_NOINLINE void __fastcall MapPointer_Place(int32_t slot, int32_t kind, float x, float z, float lift) {
    uint8_t** p = (uint8_t**)0x6ceba4 + slot;
    if (*p) {
        ModelObj_Release(*p);
        if (uint8_t* m = *p) {
            ModelObj_Destructor(m);
            ((void(__cdecl*)(void*))0x53b080)(m);
        }
        *p = nullptr;
    }
    uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x26e);
    uint8_t* m = mem ? ModelObj_Construct(mem) : nullptr;
    *p = m;
    int32_t h = Ftol((double)lift);
    if (kind == 0) Building_Init(m, 0, (const char*)0x5cb244, (const char*)0x5cb244, x, z, 1, 1, 0, (uint16_t)h);
    else if (kind == 1) Building_Init(m, 0, (const char*)0x5ce894, (const char*)0x5ce894, x, z, 1, 1, 0, (uint16_t)h);
    else Building_Init(m, 0, (const char*)0x5ce888, (const char*)0x5ce888, x, z, 1, 0, 1, (uint16_t)h);
    float ground = (float)Terrain_HeightAtTile(hook::Global<uint8_t*>(0x62c120), 0, x, z);
    Obj_MoveToCell(*p, 0, x, z, (float)((double)ground + lift));
    SceneObj_SetScale36(*p, 0, BitsF(0x3f4ccccd));
    (*p)[0x20] &= 0xdf;
}
RT3_REPLACE(0x47ba80, MapPointer_Place);

// 0x47bb90. Releases and deletes the ten preview models (0x6ceba4..0x6cebcc).
RT3_NOINLINE void PreviewModels_Free() {
    for (uint8_t** p = (uint8_t**)0x6ceba4; p < (uint8_t**)0x6cebcc; ++p) {
        if (!*p) continue;
        ModelObj_Release(*p);
        if (uint8_t* m = *p) {
            ModelObj_Destructor(m);
            ((void(__cdecl*)(void*))0x53b080)(m);
        }
        *p = nullptr;
    }
}
RT3_REPLACE(0x47bb90, PreviewModels_Free);

void* __fastcall ReturnThis47bbd0(void* o) { return o; }
RT3_REPLACE(0x47bbd0, ReturnThis47bbd0);

void __fastcall Nop47bbe0(void*) { __asm nop }
RT3_REPLACE(0x47bbe0, Nop47bbe0);

// 0x47bbf0. Resets the new-game config: start year 1830, month 1, a random
// map seed (Rand 0..32767) at +0x83 and the default flags.
RT3_NOINLINE void __fastcall NewGameConfig_Reset(uint8_t* c) {
    for (int i = 0; i < 0x42; ++i) ((uint32_t*)c)[i] = 0;
    c[0x7d] = 1;
    F<uint16_t>(c, 0x77) = 0x726;
    F<uint16_t>(c, 0x79) = 1;
    F<uint16_t>(c, 0x7b) = 0;
    int32_t seed = Rand(0, 0x7fff);
    c[0xc4] = 0;
    c[0xc5] = 0;
    c[0x82] = 0;
    F<int32_t>(c, 1) = 0;
    F<int32_t>(c, 0x83) = seed;
    c[0x87] = 1;
    c[0x97] = 1;
}
RT3_REPLACE(0x47bbf0, NewGameConfig_Reset);

// 0x47bc50. First of the 16 player slots (config +0xc6) whose state is below 2; 16 if none.
int32_t NewGameConfig_FirstOpenSlot() {
    const uint8_t* c = hook::Global<uint8_t*>(0x6cec7c);
    for (int32_t i = 0; i < 0x10; ++i)
        if (c[0xc6 + i] < 2) return i;
    return 0x10;
}
RT3_REPLACE(0x47bc50, NewGameConfig_FirstOpenSlot);

// 0x47bc80. Fills o's free slots (+0x3ae entries of 9 bytes at +0x31a, first byte
// = slot 1..40) with random free slots (seeded by 0x6cec7c +0x83, fixed at 7 in
// multiplayer), first making sure as many slots are enabled (+0x2c9 + k) as
// needed; 0x5f2d28 (41 x 0x27 bytes) marks slots taken. Not in the editor (+0x68).
RT3_NOINLINE void __fastcall Obj_AssignRandomSlots(uint8_t* o) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68)) return;
    uint8_t* taken = (uint8_t*)0x5f2d28;
    for (uint32_t a = 0x5f2d28; a < 0x5f3367; a += 0x27) *(uint8_t*)a = 0;
    for (int32_t i = 0; i < F<int32_t>(o, 0x3ae); ++i) {
        int8_t s = (int8_t)o[0x31a + i * 9];
        if (s) taken[s * 0x27] = 1;
    }
    int32_t nfree = 0;
    for (int k = 0; k < 40; ++k)
        if (o[0x2ca + k]) ++nfree;
    while (nfree < F<int32_t>(o, 0x3ae)) {
        for (int32_t k = 1; k < 0x29; ++k)
            if (!o[0x2c9 + k]) {
                o[0x2c9 + k] = 1;
                ++nfree;
                break;
            }
    }
    if (hook::Global<void*>(0x6cd8d8)) F<int32_t>(hook::Global<uint8_t*>(0x6cec7c), 0x83) = 7;
    for (int32_t i = 0; i < F<int32_t>(o, 0x3ae); ++i) {
        uint8_t* e = o + 0x31a + i * 9;
        if (*e) continue;
        int32_t n = HashRand(F<int32_t>(hook::Global<uint8_t*>(0x6cec7c), 0x83), 1, nfree);
        int32_t k = 0, tries = 0;
        if (n > 0) {
            while (nfree) {
                if (++k >= 0x29) k = 1;
                if (!taken[k * 0x27] && (o[0x2c9 + k] || tries > 200)) --n;
                if (++tries > 500) {
                    k = i + 10;
                    break;
                }
                if (n <= 0) break;
            }
        }
        *e = (uint8_t)k;
        --nfree;
        taken[k * 0x27] = 1;
    }
}
RT3_REPLACE(0x47bc80, Obj_AssignRandomSlots);

// 0x47be50. Takes over setup src's settings (+0x14, +0x3b2, +0x3ba words, except
// in multiplayer, and the +0x20 byte) and assigns its random slots.
RT3_NOINLINE void __fastcall Obj_ApplySetup(uint8_t* o, int, uint8_t* src) {
    if (!src || !src[0]) return;
    if (!hook::Global<void*>(0x6cd8d8)) {
        F<uint16_t>(o, 0x77) = F<uint16_t>(src, 0x14);
        F<uint16_t>(o, 0x79) = F<uint16_t>(src, 0x3b2);
        F<uint16_t>(o, 0x7b) = F<uint16_t>(src, 0x3ba);
    }
    o[0xc5] = src[0x20];
    Obj_AssignRandomSlots(src);
}
RT3_REPLACE(0x47be50, Obj_ApplySetup);

// 0x47bea0. Sets up the loan / bond amount slider 0x7150 of window 0x6cec18 from
// cash 0x6cec14: the top is cash * the player's aggressiveness (1e9 when that
// overflows), at least 200000 - cash, clamped to 50000..4000000 with a step that
// grows with it (1000 .. 100000); the bottom is half the top (at most 500000 - cash,
// at least 0) rounded up to a step. Range 0x6cebcc..0x6cebd0 (at least one step
// apart) in 0x5f2c50 (>= 3) notches; the knob goes to the top with `reset` or no
// current amount (0x6cec10 == -1), else to the current amount's notch.
RT3_NOINLINE void __fastcall MoneySlider_Setup(int32_t reset) {
    float aggr = Player_Aggressiveness((uint8_t*)Obj_Ref25(hook::Global<void*>(0x6cec78)));
    float cash = (float)hook::Global<int32_t>(0x6cec14);
    float v = (float)((double)cash * aggr);
    int32_t top = (double)v < *(const float*)0x5ce8a0 ? Ftol(v) : 1000000000;
    int32_t money = hook::Global<int32_t>(0x6cec14);
    if (top <= 200000 - money) top = 200000 - money;
    int32_t step;
    if (top <= 50000) {
        top = 50000;
        step = 1000;
    } else if (top >= 4000000) {
        top = 4000000;
        step = 100000;
    } else if (top < 100000) step = 1000;
    else if (top < 1000000) step = 10000;
    else if (top < 10000000) step = 100000;
    else step = top >= 100000000 ? 10000000 : 1000000;
    int32_t lo = top / 2;
    if (lo >= 500000 - money) lo = 500000 - money;
    if (lo < 0) lo = 0;
    hook::Global<int32_t>(0x6cebcc) = lo;
    if (lo % step) {
        lo += step - lo % step;
        hook::Global<int32_t>(0x6cebcc) = lo;
    }
    int32_t hi = top - top % step;
    hook::Global<int32_t>(0x6cebd0) = hi;
    if (hi <= lo + step) {
        hi = lo + step;
        hook::Global<int32_t>(0x6cebd0) = hi;
    }
    int32_t n = (hi - lo) / step + 1;
    hook::Global<int32_t>(0x5f2c50) = n;
    if (n < 3) {
        n = 3;
        hook::Global<int32_t>(0x5f2c50) = 3;
    }
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cec18), 0, 0x73, 0x7150, (uint32_t)n, 0, 0);
    int32_t cur = hook::Global<int32_t>(0x6cec10), pos;
    if (cur == -1 || reset) pos = hook::Global<int32_t>(0x5f2c50) - 1;
    else {
        int32_t last = hook::Global<int32_t>(0x5f2c50) - 1;
        int32_t notch = (hook::Global<int32_t>(0x6cebd0) - hook::Global<int32_t>(0x6cebcc)) / last;
        pos = (cur - hook::Global<int32_t>(0x6cebcc)) / notch;
        if (pos < 0) pos = 0;
        if (last < pos) pos = last;
    }
    Window_SendEvent5(hook::Global<uint8_t*>(0x6cec18), 0, 0x66, 0x7150, (uint32_t)pos, 0, 0);
}
RT3_REPLACE(0x47bea0, MoneySlider_Setup);

}  // namespace rt3
