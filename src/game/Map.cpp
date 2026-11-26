// Map.cpp - RT3.exe 0x448000-0x454000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <initializer_list>
#include <windows.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/crt.h"
#include "game/calendar.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall Terrain_Destructor(uint8_t* r);

void PreviewModels_Free();

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
void __fastcall Terrain_DeleteMarkers(uint8_t* r);
void __fastcall Obj_FreeKeyList(void* o);
void __fastcall Station_Reset(void* o, int, uint16_t x, uint16_t y);
uint8_t* __fastcall Obj_Region(void* o);
void __fastcall Map_FreeCellKeyLists(uint8_t* m);
void __fastcall Tracks_Destruct(Collection* c);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Scene_FocusCell(uint8_t* s, int, float* x, float* y);

void __fastcall Render_SetFog(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                              uint32_t f, uint32_t g);

void __fastcall Render_DisableFog(uint8_t* r);
uint32_t __fastcall Render_Get1554(uint8_t* r);
uint32_t __fastcall Terrain_Layer(uint8_t* r, int, uint8_t i);
void __fastcall Terrain_Save(uint8_t* t, int, int32_t s, const char* name, int32_t quality);
void __fastcall Save_LogProgress(Stream s, const char* label);
void __fastcall Region_SaveCargo(uint8_t* r, int, Stream st);
void __fastcall Map_Free(uint8_t* m);
void __fastcall Map_AllocGrids(uint8_t* m);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
uint32_t __fastcall Terrain_RecBit9(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Site_ClassifyWater(uint8_t* s);
void __fastcall Station_UpdateEdgeFlag(uint8_t* o);
uint32_t __fastcall Terrain_CellByte(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t Option2bbActive();
int32_t __fastcall Map_ClampRect(void* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
void __fastcall Map_MarkTileDirty(uint8_t* m, int, int32_t x, int32_t y);
static const int32_t* const kDirX = (const int32_t*)0x624b28;
static const int32_t* const kDirY = (const int32_t*)0x624b48;
void __fastcall Map_MarkNearKind245(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Map_UpdateBlockMask(uint8_t* m, int, int32_t x, int32_t y);
Collection* __fastcall Tracks_Construct(Collection* c, int, int32_t columns, int32_t);
void __fastcall Map_ResetCells(uint8_t* m);
void __fastcall City_UpdateCargoFlags(uint8_t* c);
void __fastcall City_Refresh(uint8_t* c);
int32_t __fastcall Obj_Bit(void* o, int, int32_t i);
int32_t __fastcall Industry_HandlesCargo(uint8_t* ind, int, int32_t cargo);
int32_t __fastcall Obj_CheckActive(void* o);
float __stdcall DarknessOf(uint8_t b);
int32_t __stdcall BrightnessOf(float f);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);

void __fastcall Terrain_PaintLayers(uint8_t* t, int, int32_t x, int32_t y, float s, int32_t mode, const float* w,
                                    const float* p);

int32_t Global6d07b4Set2();
int32_t __fastcall Cities_AssignBlocks(Collection* cities);
int32_t __fastcall Map_BlockCity(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Station_CapStocks(void* o);
void __fastcall SetSeed(int32_t seed);
void PushSeed();
void PopSeed();
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void* __fastcall Engine_Get1C(void* e);
uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Map_CountShoreNeighbours(uint8_t* m, int, int32_t x, int32_t y);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall Track_UpdateColor(uint8_t* o);
void __fastcall Map_SetCoarseByte(void* m, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Obj_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add);
void __fastcall Trains_Clear1a3(Collection* t);

void __fastcall Track_Setup(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d,
                            void* terrain, uint8_t style);

void __fastcall Map_DrainCell(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Water_FloodFrom(uint8_t* m, int, int32_t x, int32_t y);
void* __fastcall Obj_Building(void* o);
uint8_t* __fastcall Obj_BuildingRegion(void* o);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
float __fastcall Fade_Step_Impl();

void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                 uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex,
                                 uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16,
                                 uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20);

double One();
void __fastcall Fog_ParamsAt(uint8_t*, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color, float* strength);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Fade_Save();
void __fastcall Fade_Restore();
void __stdcall Fade_Set(uint32_t v);
int32_t __fastcall Camera_InBounds(uint8_t* c, int, float x, float y);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
double __fastcall Obj_BuildingCellY(void* o);
double __fastcall Obj_BuildingCellX(void* o);

void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap,
                                  float* outCount, float* outAmount, float* outFlagged);

float __fastcall Station_TotalAvailable(void* o);
float __fastcall City_CargoOutput(uint8_t* c, int, int32_t cargo);
double __fastcall Grid_Float1605(uint8_t* g, int, int32_t x, int32_t y);
double __stdcall Distance3D(float x1, float y1, float z1, float x2, float y2, float z2);
int32_t Tutorial_Blink348();
int32_t Tutorial_Blink352();
int32_t __fastcall Chain_Contains(void* o, int, uint16_t k);
int32_t __fastcall Chain_AnyActive(void* o);
void* __fastcall Obj_FirstActiveInChain(void* o);
int32_t __fastcall Table_NearestValue23a(Collection* t, int, float x, float y);
double __fastcall Camera_TargetAboveGround(uint8_t* c);
uint8_t* __fastcall Grid_RegionAt(void* g, int, int32_t x, int32_t y);
float __fastcall Obj_Level(void* o, int, int32_t i);
void __fastcall Obj_GetDims(uint8_t* o, int, int32_t* a, int32_t* b, float* c);
void __fastcall Map_DrawCellOverlay(uint8_t* m, int, float x, float y, uint32_t fade, int32_t cx, int32_t cy);
uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h);
void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count);
void __fastcall Map_SetCellLow(uint8_t* m, int, int32_t x, int32_t y, uint8_t lo);
void __fastcall Terrain_RefreshPointGrids(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
const char* __fastcall GetString(int32_t id);
void __fastcall Map_UpdateQuotaRange(uint8_t* m);
void __fastcall Render_ColourMap(uint8_t* r, int, uint32_t** pix, uint32_t* w, uint32_t* h);
void __fastcall Render_DestroyColourMap(uint8_t* r);
uint32_t* __fastcall Overview_Pixels(uint8_t* r, int, int32_t w, int32_t h);
float __fastcall Grid_Darkness(uint8_t* g, int, int32_t x, int32_t y, int32_t layer);
uint32_t __stdcall HeatColor(float t);
uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y);
uint32_t __fastcall Map_CellKeyColour(uint8_t* m, int, int32_t x, int32_t y, int32_t key);
double __fastcall Terrain_HeightAt(uint8_t* t, int, float x, float z);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
int32_t __fastcall Grid_CellMarked(uint8_t* g, int, int32_t x, int32_t y);
uint8_t __fastcall Terrain_GetFlag(uint8_t* t, int, int32_t x, int32_t y);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
void __fastcall Render_Set159a(uint8_t* r, int, uint8_t v);
void __fastcall Overview_Refresh(uint8_t* r);
void __fastcall Render_SetRect15a4(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
int32_t __fastcall Render_Enable159b(uint8_t* r);
int32_t __fastcall Map_TidyWater(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
uint32_t __fastcall Render_Get1588(uint8_t* r);
uint32_t __fastcall Render_Get1584(uint8_t* r);
void __fastcall Map_CountLand(uint8_t* m);
void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Map_ReclassifySites(uint8_t* m);
void __fastcall Territories_Recount(Collection* c);
void __fastcall Terrain_RefreshWaterFlow(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void* __fastcall Obj_Ref3cc(void* o);
const char* __fastcall LocalizeName(const char* name);
void __fastcall Table_RefreshAll(Collection* t);
void __fastcall Buildings_RefreshHighlight(Collection* t, int, int32_t key);
void __stdcall Buildings_UpdateFor(int32_t cargo);
void __fastcall Map_ClearFogMode(uint8_t* m);
void __fastcall Render_Disable159b(uint8_t* r);
int32_t ClockMs();
uint32_t __fastcall BlendColor(uint32_t a, uint32_t b, float t);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

void __fastcall Overview_SetOption(uint8_t* t, int, int32_t v);
void __fastcall Overview_SetHighlight(uint8_t* t, int, int32_t key);
void __fastcall Overview_SetCargo(uint8_t* t, int, int32_t cargo);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t IsMultiplayer();
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
int32_t CountHumanPlayers();
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void __fastcall Options_ApplyGraphics(uint8_t* o, int, int32_t which, int32_t rebuild);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
void World_RefreshColors();
void __fastcall Terrain_RefreshRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Terrain_RefreshQuads(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Objs_DropInRect(Collection* c, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Objs_DropAllInRect(Collection* c, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
int32_t __fastcall Map_NearFeature(uint8_t* m, int, int32_t x, int32_t y);
int32_t __fastcall StreamSkip4(int32_t s, int tag, int32_t, int32_t count);
float __fastcall Obj_CenterCellY(void* o);
float __fastcall Obj_CenterCellX(void* o);
void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
uint32_t __fastcall Obj_GridByte(void* o);
void __fastcall Tracks_ClearHighlights(Collection* c);
void __fastcall Tracks_HighlightRun(Collection* c, int, uint32_t id, int32_t on, int32_t follow);
int32_t __fastcall Obj_NodeNear8(uint8_t* o, int, int32_t id);
int32_t __fastcall Obj_NodeNear356(uint8_t* o, int, int32_t id);
void __fastcall Terrain_PaintPoints(uint8_t* t, int, float x, float y, float r, int32_t erase, int32_t kind, int32_t variant, float density, int32_t force);
float __fastcall Building_Price(uint8_t* b, int, uint32_t company);
int32_t __fastcall Track_BuildCost(uint8_t* t, int, uint32_t company, int32_t inner);
int32_t __fastcall Terrain_ImportHeightmap(uint8_t* t, int, const char* name, float scale, float curve, int32_t smooth);
void __fastcall Ambient_Start(void*);
void __fastcall Map_PaintShallows(uint8_t* m);
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t GetSeed();
void Vegetation_BuildTables();
void __fastcall Map_RaiseAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float amount, uint8_t keepFloor);
void __fastcall Map_SmoothAround(uint8_t* m, int, int32_t cx, int32_t cy, int32_t r, float strength);
void __fastcall Map_LevelAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float useCentre, float strength);
void __fastcall Map_RescaleHeights(uint8_t* m, int, float scale, int32_t mode);
Collection* __fastcall Collection5ce044_Construct(Collection* c);
Collection* __fastcall Collection5ce4a8_Construct(Collection* c);
Collection* __fastcall Collection5c9ae0_Construct(Collection* c);
Collection* __fastcall Collection5cf10c_Construct(Collection* c);
Collection* __fastcall Collection5c9810_Construct(Collection* c);
Collection* __fastcall Collection5c9414_Construct(Collection* c);
Collection* __fastcall Collection5ce130_Construct(Collection* c);
Collection* __fastcall Collection5ce90c_Construct(Collection* c);
Collection* __fastcall Routes_Construct(Collection* c);
Collection* __fastcall Collection5c9cec_Construct(Collection* c);
uint8_t* __fastcall Camera_ConstructView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
void __fastcall Map_InitRuntimeState(uint8_t* map);
void __fastcall Map_UnloadWorld(uint8_t* map);
void __fastcall Terrain_Create(uint8_t* t, int, int32_t w, int32_t h, const char* tex);
uint8_t* __fastcall Terrain_Construct(uint8_t* t);
void ClearGlobals6ceba4();
int32_t __fastcall Terrain_Load(uint8_t* t, int, int32_t s, const char* tex);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
void __fastcall Terrain_LoadLayers(uint8_t* r, int, int32_t s);
void __fastcall Site_Load(uint8_t* o, int, int32_t s, int16_t x, int16_t y);
void __fastcall Routes_BuildCosts(uint8_t* t);
void __fastcall Overview_Close(uint8_t* t);
void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
RT3_NOINLINE int32_t GetSeed();
RT3_NOINLINE int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
uint8_t* __fastcall Map_Construct(uint8_t* m, int, int32_t w, int32_t h, const char* tex);
int32_t __fastcall Map_ImportHeightmap(uint8_t* m, int, const char* name, float scale, float curve, int32_t smooth);
RT3_NOINLINE void __fastcall Map_UnloadWorld(uint8_t* map);
RT3_NOINLINE void __fastcall Ambient_Start(void*);
uint8_t* __fastcall Map_ConstructLoad(uint8_t* m, int, int32_t s, const char* tex);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
RT3_NOINLINE uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Terrain_LoadLayers(uint8_t* r, int, Stream s);
RT3_NOINLINE uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
RT3_NOINLINE void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
RT3_NOINLINE void __fastcall Territories_Recount(Collection* c);
RT3_NOINLINE int32_t __fastcall Cities_AssignBlocks(Collection* cities);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Options_ApplyGraphics(uint8_t* o, int, int32_t which, int32_t rebuild);
RT3_NOINLINE void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund);
void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Overview_ShowPlainOnce(uint8_t* t);
void __fastcall Overview_EndPlain(uint8_t* t);
int32_t __fastcall Map_Bulldoze(uint8_t* map, int, float x, float y, float r, uint32_t company, int32_t checkOnly, float* cost, char* msg);
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall Map_ClearCellFlag10(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

template <size_t N> static void Clear(uint8_t* map, const uint32_t (&offsets)[N]) {
    for (uint32_t off : offsets) F<uint32_t>(map, off) = 0;
}

template <typename T> static inline T& F_runtime_wrappers(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }

// Functions of this file, in exe order.
void __fastcall Tracks_RefreshArea(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Region_MonthlyEconomy(uint8_t* r);
RT3_NOINLINE void __fastcall Map_InitRuntimeState(uint8_t* map);
RT3_NOINLINE void __fastcall Map_Free(uint8_t* m);
RT3_NOINLINE void __fastcall Map_AllocGrids(uint8_t* m);
RT3_NOINLINE void __fastcall World_CreateTables(uint8_t* m, int, int32_t sx, int32_t sy);
void __fastcall Map_SetCoarseByte(void* m, int, int32_t x, int32_t y, uint8_t v);
RT3_NOINLINE double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);
RT3_NOINLINE int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
RT3_NOINLINE double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
RT3_NOINLINE double __fastcall Grid_Float1605(uint8_t* g, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Terrain_MarkFlooded(uint8_t* t, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Terrain_MarkDrained(uint8_t* t, int, int32_t x, int32_t y);
RT3_NOINLINE uint32_t __fastcall Terrain_PaintVertex(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Grid_MarkCell(uint8_t* g, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Map_ClearCellFlag10(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE uint8_t __fastcall Grid_CellLow(uint8_t* g, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Grid_SetCellLow(uint8_t* g, int, int32_t x, int32_t y, uint8_t v);
RT3_NOINLINE void __fastcall Grid_SetByte1661(uint8_t* g, int, int32_t x, int32_t y, uint8_t v);
RT3_NOINLINE void __fastcall Grid_SetByte1665(uint8_t* g, int, int32_t x, int32_t y, uint8_t v);
RT3_NOINLINE uint8_t __fastcall Grid_Byte1661(uint8_t* g, int, int32_t x, int32_t y);
RT3_NOINLINE uint8_t __fastcall Grid_Byte1665(uint8_t* g, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Grid_CellMarked(uint8_t* g, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Terrain_LoadLayers(uint8_t* r, int, Stream s);
RT3_NOINLINE void __fastcall Terrain_SaveLayers(uint8_t* r, int, Stream s);
float __stdcall DarknessOf(uint8_t b);
int32_t __stdcall BrightnessOf(float f);
int32_t __fastcall Map_PointCount(void* m);
float __fastcall Grid_Roughness(uint8_t* g);
RT3_NOINLINE void __fastcall Map_Create(uint8_t* m, int, int32_t w, int32_t h, const char* tex);
RT3_NOINLINE void __fastcall Map_UnloadWorld(uint8_t* map);
void __fastcall Map_Resize(uint8_t* m, int, int32_t w, int32_t h);
RT3_NOINLINE void __fastcall Map_RuntimeDestructor(uint8_t* map);
RT3_NOINLINE void __fastcall Map_Save(uint8_t* w, int, Stream s, const char* name);
RT3_NOINLINE void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
RT3_NOINLINE int32_t __fastcall Terrain_IsSpecial(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Map_NearFeature(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Map_WaterWithin(uint8_t* m, int, int32_t x, int32_t y, int32_t range);
int32_t __fastcall Map_Bit9NearWater(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE float __fastcall Map_DistanceToOtherKind(uint8_t* m, int, float x, float y);
int32_t __fastcall Map_ClampRect(void* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
int32_t __stdcall Map_ClampRectInclusive(int32_t*, int32_t*, int32_t*, int32_t*);
RT3_NOINLINE void __fastcall Map_CountLand(uint8_t* m);
RT3_NOINLINE void __fastcall Map_ReclassifySites(uint8_t* m);
RT3_NOINLINE float __fastcall Map_TotalCargoValue(uint8_t* m, int, int32_t);
RT3_NOINLINE void __fastcall Map_MarkBlocks3(uint8_t* m, int, float x, float y, float r, uint8_t v);
RT3_NOINLINE void __fastcall Map_MarkBlocks0(uint8_t* m, int, float x, float y, float r, uint8_t v);
RT3_NOINLINE void __fastcall Map_MarkTiles(uint8_t* m, int, float x, float y, float r, uint8_t v);
int32_t __fastcall Map_BlockCity(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Map_CellCity(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE uint16_t __stdcall Regions_NearestValue(int32_t x, int32_t y);
RT3_NOINLINE float __fastcall Grid_Darkness(uint8_t* g, int, int32_t x, int32_t y, int32_t layer);
RT3_NOINLINE void __fastcall Terrain_PaintLayers(uint8_t* t, int, int32_t x, int32_t y, float s, int32_t mode, const float* w, const float* p);
RT3_NOINLINE uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
RT3_NOINLINE void* __fastcall Map_RegionAt(uint8_t* map, int, float x, float y);
RT3_NOINLINE void* __fastcall Map_RegionRecordAt(void* m, int, float x, float y);
double __fastcall Map_NearestLandHeight(uint8_t* m, int, int32_t x, int32_t y, int32_t r);
int32_t __stdcall Map_FogEnd(int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Map_Bulldoze(uint8_t* map, int, float x, float y, float r, uint32_t company, int32_t checkOnly, float* cost, char* msg);
int32_t __fastcall Map_TerrainKindAt(void* m, int, float x, float y);
int32_t __fastcall Map_CountShoreNeighbours(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Water_FloodFill(uint8_t* m, int, int32_t x, int32_t y, int32_t dir, int32_t flag);
void __fastcall Water_FloodFrom(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Map_UpdateCargoFlags(uint8_t* m);
RT3_NOINLINE void __fastcall Map_MarkUntypedRegions(uint8_t* m);
void __fastcall Map_MarkTileDirty(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Terrain_AgeCorners(uint8_t* t);
RT3_NOINLINE void __fastcall Map_FeatureBounds(uint8_t* m);
RT3_NOINLINE uint8_t* __fastcall Map_Construct(uint8_t* m, int, int32_t w, int32_t h, const char* tex);
RT3_NOINLINE void __fastcall Map_Load(uint8_t* m, int, int32_t s, const char* tex);
RT3_NOINLINE void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __stdcall Map_CellsChanged(int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall Map_SmoothAround(uint8_t* m, int, int32_t cx, int32_t cy, int32_t r, float strength);
RT3_NOINLINE void __fastcall Map_SmoothHeights(uint8_t* m, int, int32_t cx, int32_t cy, int32_t r, int32_t refresh);
RT3_NOINLINE void __fastcall Map_LevelAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float useCentre, float strength);
RT3_NOINLINE void __fastcall Map_RaiseAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float amount, uint8_t keepFloor);
RT3_NOINLINE void __fastcall Map_SetCellHeight(uint8_t* m, int, int32_t x, int32_t y, uint32_t v, int32_t refresh);
void __fastcall Map_MarkNearKind245(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Map_FloodCell(uint8_t* m, int, int32_t x, int32_t y, int32_t);
RT3_NOINLINE void __fastcall Map_DrainCell(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Terrain_PaintBrush(uint8_t* t, int, float cx, float cy, float r, float strength, int32_t hard, int32_t mode, const float* w, const float* p, uint8_t* mask);
uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y);
RT3_NOINLINE void __fastcall Map_RescaleHeights(uint8_t* m, int, float scale, int32_t mode);
void __fastcall Track_Init(uint8_t* t, int, int32_t id, int32_t group, float x0, float z0, float x1, float z1, float x2, float z2, float x3, float z3, int32_t kind, int32_t owner, int32_t style);
int32_t __fastcall Map_TidyWater(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Map_MarkDirtyAround(uint8_t* m, int, float x, float y);
RT3_NOINLINE uint8_t* __fastcall Map_ConstructLoad(uint8_t* m, int, int32_t s, const char* tex);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
RT3_NOINLINE void __fastcall Map_PaintShallows(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Map_ImportHeightmap(uint8_t* m, int, const char* name, float scale, float curve, int32_t smooth);
RT3_NOINLINE void __fastcall Routes_BuildCosts(uint8_t* t);
void __fastcall Map_FreeCellKeyLists(uint8_t* m);
void __fastcall Map_ResetCells(uint8_t* m);
RT3_NOINLINE void __fastcall Map_RefreshCells(uint8_t* m);
void __fastcall Map_MonthlyEconomy(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Obj_Byte21c5(void* o);
void __fastcall Map_UpdateQuotaRange(uint8_t* m);
uint32_t __fastcall Map_CellKeyColour(uint8_t* m, int, int32_t x, int32_t y, int32_t key);
RT3_NOINLINE void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
RT3_NOINLINE void __fastcall Map_DrawCellOverlay(uint8_t* m, int, float x, float y, uint32_t fade, int32_t cx, int32_t cy);
void __fastcall Map_DrawCellLabel(uint8_t* m, int, float x, float y, float fade, int32_t cx, int32_t cy);
void __fastcall Map_SetFogMode(uint8_t* m, int, int32_t mode);
void __fastcall Map_ClearFogMode(uint8_t* m);
RT3_NOINLINE void __fastcall Overview_SetOption(uint8_t* t, int, int32_t v);
RT3_NOINLINE void __fastcall Overview_SetHighlight(uint8_t* t, int, int32_t key);
void __stdcall Buildings_UpdateFor(int32_t cargo);
RT3_NOINLINE void __fastcall Obj_UpdateThreshold(uint8_t* o);
RT3_NOINLINE void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Overview_SetCargo(uint8_t* t, int, int32_t cargo);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_ShowPlainOnce(uint8_t* t);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall Overview_EndPlain(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Map_CellHigh(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE uint32_t __fastcall Map_CellLow(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Map_SetCellHigh(uint8_t* m, int, int32_t x, int32_t y, uint8_t hi);
RT3_NOINLINE void __fastcall Map_SetCellLow(uint8_t* m, int, int32_t x, int32_t y, uint8_t lo);
RT3_NOINLINE void FreeBuffers6acd20();
int32_t __fastcall Map_TreeSpot(uint8_t* m, int, int32_t x, int32_t y, int32_t blk, uint8_t spot, uint32_t* outX, uint32_t* outY, uint32_t* outV, uint8_t* outFlags);
void __fastcall Map_DropMarkers(uint8_t* m);
RT3_NOINLINE void Vegetation_BuildTables();
RT3_NOINLINE void __fastcall TreeTypes_Load(uint8_t* r);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void __fastcall Track_DropAndSettle(uint8_t* t, int, int32_t, int32_t neighbours);
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Terrain_Resize(uint8_t* r, int, int32_t w, int32_t h);
RT3_NOINLINE int32_t __fastcall Terrain_ImportHeightmap(uint8_t* t, int, const char* name, float scale, float curve, int32_t smooth);
void __fastcall Track_Setup(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d, void* terrain, uint8_t style);
RT3_NOINLINE void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);

static inline uint8_t* Cell165d(uint8_t* g, int32_t x, int32_t y) {
    return F<uint8_t*>(g, 0x165d) + (F<int32_t>(g, 0x15dd) * y + x) * 3 + 2;
}

// 0x449e90. Clamps the cell rectangle to the map; 1 if it is then non-empty.
static int32_t __fastcall Map_ClampRectInclusive_Impl(uint8_t* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1) {
    if (*x0 < 0) *x0 = 0;
    if (*y0 < 0) *y0 = 0;
    int32_t w = F<int32_t>(m, 0x2155), h = F<int32_t>(m, 0x2159);
    *x1 = w < *x1 ? w : *x1;
    *y1 = h < *y1 ? h : *y1;
    if (*x0 < 0 || *y0 < 0 || *x1 > w || *y1 > h) return 0;
    return *x1 >= *x0 && *y1 >= *y0 ? 1 : 0;
}


static inline uint8_t* Renderer() { return (uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)); }

static inline uint32_t VertexCount(uint8_t* r) {
    return (uint32_t)((F<int32_t>(r, 0x15d9) + 1) * (F<int32_t>(r, 0x15d5) + 1));
}

static inline uint8_t* Map() { return g_map; }

// Eight neighbour directions (dx 0x624b28, dy 0x624b48).
static inline int32_t DirX(int32_t d) { return ((const int32_t*)0x624b28)[d]; }

static inline int32_t DirY(int32_t d) { return ((const int32_t*)0x624b48)[d]; }

// Terrain vertex record (+0x165d, 3 bytes, stride +0x15dd).
static inline uint8_t* Rec(uint8_t* r, int32_t x, int32_t y) {
    return F<uint8_t*>(r, 0x165d) + (F<int32_t>(r, 0x15dd) * y + x) * 3;
}

static inline bool OutsideGrid(int32_t cx, int32_t cy) {
    return cx < 0 || cx >= F<int32_t>(Map(), 0x15d9) - 1 || cy < 0 || cy >= F<int32_t>(Map(), 0x15d5) - 1;
}

// 0x448c20 / 0x448cb0. Marks a land vertex as flooded (kinds 1/4) or drained (3/5):
// clears its layer +0x1655, sets the kind bits, drops its counters and notifies the camera.
static inline void MarkWater(uint8_t* t, int32_t x, int32_t y, uint32_t keep2, uint32_t set2, uint32_t keep, uint32_t set) {
    F<uint8_t*>(t, 0x1655)[F<int32_t>(t, 0x15dd) * y + x] = 0;
    uint16_t* rec = (uint16_t*)(F<uint8_t*>(t, 0x165d) + (F<int32_t>(t, 0x15dd) * y + x) * 3);
    uint32_t w = *rec;
    w = (w & 7) == 2 ? ((w & keep2) | set2) : ((w & keep) | set);
    *rec = (uint16_t)w;
    Terrain_DecCellCount(t, 0, x, y, 6);
    void* cam = hook::As<void*>(g_62be68);
    ((void(__thiscall*)(void*))(*(void***)cam)[0])(cam);
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline Collection* Cargos() { return g_cargoTypes; }

// Cell centre (n + 0.5) as a float.
static inline float Centre(int32_t n) { return (float)((double)n + D(0x5c8628)); }

// Cell centre in world units.
static inline float CentreWorld(int32_t n) { return (float)((double)Centre(n) * Fl(0x5c8700)); }

// Cell word (3-byte stride) / byte grids of the map at (x, y).
static inline uint16_t* CellWord(uint8_t* m, int32_t x, int32_t y) {
    return (uint16_t*)(F<uint8_t*>(m, 0x165d) + (F<int32_t>(m, 0x15dd) * y + x) * 3);
}

static inline uint8_t& CellByte(uint8_t* m, uint32_t grid, int32_t x, int32_t y) {
    return F<uint8_t*>(m, grid)[F<int32_t>(m, 0x15dd) * y + x];
}

static inline uint8_t& ShadeByte(uint8_t* m, int32_t x, int32_t y) {
    return F<uint8_t*>(m, 0x162d)[((F<int32_t>(m, 0x15d9) >> (uint8_t)F<int32_t>(m, 0x15e1)) + 1) * y + x];
}

static inline void SetBit(uint8_t* bits, int32_t idx, int32_t on) {
    uint8_t b = (uint8_t)(1 << (idx & 7));
    if (on) bits[idx >> 3] |= b;
    else bits[idx >> 3] &= (uint8_t)~b;
}

static inline uint8_t* ZeroedAlloc(size_t n) {
    uint8_t* p = (uint8_t*)crt::malloc(n);
    memset(p, 0, n);
    return p;
}

// Share of the radius left at the centre of cell (x, y).
static inline float Falloff(int32_t x, int32_t y, float cx, float cy, float r) {
    float py = (float)((double)y + D(0x5c8628));
    float px = (float)((double)x + D(0x5c8628));
    return (float)(((double)r - Distance2D(px, py, cx, cy)) / r);
}

// Sets byte `byteOff` of the 4x4 blocks (+0x212d) inside the circle (world units);
// radius 0 marks the whole bounding range.
static void MarkBlocks(uint8_t* m, float x, float y, float r, uint8_t v, int32_t byteOff) {
    float k = Fl(0x5c8568);
    float cx = (float)((double)x / k), cy = (float)((double)y / k), rr = (float)((double)r / k);
    int32_t x0, x1, y0, y1;
    float t = (float)((double)cx - rr - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    x0 = (int32_t)t;
    t = (float)((double)cy - rr - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    y0 = (int32_t)t;
    float mx = (float)(F<int32_t>(m, 0x214d) - 1);
    float u = (float)((double)rr + cx + Fl(0x5c85b4));
    if (mx < u) u = mx;
    x1 = (int32_t)u;
    float my = (float)(F<int32_t>(m, 0x2151) - 1);
    u = (float)((double)rr + cy + Fl(0x5c85b4));
    if (my < u) u = my;
    y1 = (int32_t)u;
    for (int32_t by = y0; by <= y1; ++by)
        for (int32_t bx = x0; bx <= x1; ++bx) {
            if (!(rr == Fl(0x5c8598)) && Falloff(bx, by, cx, cy, rr) <= Fl(0x5c8598)) continue;
            F<uint8_t*>(m, 0x212d)[(F<int32_t>(m, 0x214d) * by + bx) * 4 + byteOff] = v;
        }
}

static inline float Bits(uint32_t u) { return *(float*)&u; }


static inline double Fl_194(uint32_t a) { return *(const float*)a; }

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

static inline double Fl_195(uint32_t a) { return *(const float*)a; }

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }


static inline double Fl_207(uint32_t a) { return *(const float*)a; }

// Checkerboard base colour: a, plus b on odd cells.
static inline uint32_t Overview_Check(int32_t x, int32_t y, uint32_t odd, uint32_t base) { return (((x + y) & 1) ? odd : 0) + base; }

// Colour of map t's overview cell (x, y) (sx, sy: in quarter cells on big
// maps) for overview mode t +0x2175 and its key +0x2179.
static uint32_t Overview_CellColour(uint8_t* t, const void* arg, int32_t x, int32_t y, int32_t sx, int32_t sy, uint32_t prev) {
    uint8_t* M = g_map;
    int32_t mode = F<int32_t>(t, 0x2175);
    switch (mode) {
    case 0x1f: {
        float p = 1.0f;
        for (int32_t i = 0; i < 3; ++i) p = (float)((D(0x5c8770) - Grid_Darkness(t, 0, x, y, i)) * p);
        float q = (float)(D(0x5c8770) - p);
        return HeatColor((float)(Grid_Darkness(t, 0, x, y, 3) * D(0x5c8c20) + q));
    }
    case 0x20: case 0x21: case 0x22: case 0x23: return HeatColor(Grid_Darkness(t, 0, x, y, mode - 0x20));
    case 0x24: {
        uint32_t c;
        uint32_t reg = F<uint8_t*>(M, 0x2131)[F<int32_t>(M, 0x2155) * y + x] >> 3;
        if (reg == F<uint32_t>(t, 0x2179)) c = 0xc35ae343;
        else if (!F<int32_t>(g_app, 0x68) &&
                 Map_RegionTypeAt(g_map, 0, (float)x, (float)y) == F<uint32_t>(t, 0x2179))
            c = 0xc35ae343;
        else c = 0;
        return Terrain_KindIs1Or3to5(g_map, 0, x, y) ? 0 : c;
    }
    case 0x1e: {
        uint32_t sel = arg ? *(const uint32_t*)arg : 0;
        uint32_t reg = F<uint8_t*>(M, 0x2131)[F<int32_t>(M, 0x2155) * y + x] >> 3;
        uint32_t c = reg == sel ? 0xa8ffff00 : hook::Global<uint32_t>(0x5ee840 + (reg & 0x1f) * 4);
        return Terrain_KindIs1Or3to5(g_map, 0, x, y) ? 0 : c;
    }
    case 0x17: return ((const uint8_t*)arg)[F<int32_t>(t, 0x2155) * y + x] ? 0x38ffffff : 0;
    case 3: {
        if (int32_t key = F<int32_t>(t, 0x2181))
            if (uint32_t c = Map_CellKeyColour(t, 0, x, y, key)) return c;
        uint8_t* cell = F<uint8_t*>(t, 0x2129) + (F<int32_t>(t, 0x2145) * y + x) * 0x1d7;
        if ((cell[0xe6] & 1) && !(cell[0xe6] & 4)) return 0;
        int32_t k = F<int32_t>(t, 0x2179);
        if (!k || F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), k * 4 + 0x9a) != -1 || F<int32_t>(g_world, 0x4af7)) return 0x8fcccccc;
        float q = (float)City_CargoQuota(cell, 0, k % 0x35);
        return HeatColor((float)(((double)q - F<float>(t, 0x2195)) / F<float>(t, 0x2199)));
    }
    case 2: {
        uint8_t* cell = F<uint8_t*>(t, 0x2129) + (F<int32_t>(t, 0x2145) * y + x) * 0x1d7;
        if ((cell[0xe6] & 1) && !(cell[0xe6] & 4)) return 0;
        float v;
        int32_t k = F<int32_t>(t, 0x2179);
        if (!k) {
            float s = (float)F<uint16_t>(cell, 0xf3);
            for (int32_t i = 1; i < 8; ++i) {
                float w = (float)F<uint16_t>(cell, 0xf3 + i * 2);
                s = w + s;
            }
            v = (float)(s / Fl_207(0x5c8898));
        } else {
            v = (float)F<uint16_t>(cell, 0xf3 + ((k - 1) % 8) * 2);
        }
        float a = (float)(v / Fl_207(0x5caacc));
        a = (float)(a * Fl_207(0x5c9aa8));
        a = (float)(Fl_207(0x5ca0ec) - a);
        return HeatColor((float)(a / D(0x5ca110)));
    }
    case 4: {
        uint8_t* cell = F<uint8_t*>(t, 0x2129) + (F<int32_t>(t, 0x2145) * y + x) * 0x1d7;
        if ((cell[0xe6] & 1) && !(cell[0xe6] & 4)) return 0;
        float v = (float)(F<float>(cell, 0x103 + (F<int32_t>(t, 0x2179) % 0x35) * 4) * Fl_207(0x5c85f8));
        return HeatColor((float)(v / D(0x5ca110)));
    }
    case 5: {
        uint8_t* cell = F<uint8_t*>(t, 0x2129) + (F<int32_t>(t, 0x2145) * y + x) * 0x1d7;
        if ((cell[0xe6] & 1) && !(cell[0xe6] & 4)) return 0;
        double w = F<uint16_t>(cell, 0x6a + (F<int32_t>(t, 0x2179) % 0x35) * 2);
        float a = (float)(w / D(0x5c9c40));
        a = (float)(a * Fl_207(0x5c85f8));
        return HeatColor((float)(a / D(0x5ca110)));
    }
    case 6: return Map_CellKeyColour(t, 0, x, y, F<int32_t>(t, 0x2179));
    case 7: return 0x8fcccccc;
    case 0x10: {
        uint32_t sel = arg ? *(const uint32_t*)arg : 0;
        const uint8_t* p = F<uint8_t*>(M, 0x212d) + (F<int32_t>(M, 0x214d) * y + x) * 4;
        uint32_t c = p[0] == sel ? 0xa8ffff00 : hook::Global<uint32_t>(0x5ee840 + (p[0] & 0x1f) * 4);
        if (*(const uint16_t*)(p + 1)) c = 0x480000ff;
        return c;
    }
    case 0x11: {
        int32_t cy = Ftol(((double)y + D(0x5c8628)) * D(0x5c87b8));
        int32_t cx = Ftol(((double)x + D(0x5c8628)) * D(0x5c87b8));
        if (Terrain_KindIs1Or3to5(g_map, 0, cx, cy)) return 0;
        uint8_t* m2 = g_map;
        uint32_t v = F<uint8_t*>(m2, 0x212d)[(F<int32_t>(m2, 0x214d) * y + x) * 4 + 3];
        return F<uint32_t>(t, 0x2179) == v ? 0x7fffff00 : hook::Global<uint32_t>(0x5ee82c + v * 4);
    }
    case 0x12: {
        uint32_t w = *(const uint16_t*)(F<uint8_t*>(M, 0x212d) + (F<int32_t>(M, 0x214d) * y + x) * 4 + 1);
        return w != F<uint32_t>(t, 0x2179) ? 0 : 0x4800ff00;
    }
    case 0x18:
        if (!Terrain_RecBit9(g_map, 0, x, y)) return Overview_Check(x, y, 0xff000001, 0x8ffffff);
        if (!Terrain_KindIs2Or4Or5(g_map, 0, x, y)) return Overview_Check(x, y, 0x7b9f4a4, 0x209414c4);
        if (Terrain_Rec(g_map, 0, x, y)[2] & 0x20) return 0x2fff0000;
        return Overview_Check(x, y, 0xac9f4a4, 0x24a4a4e4);
    case 0x19:
        return Terrain_KindIs1Or4(g_map, 0, x, y) ? Overview_Check(x, y, 0x1fb9f4a4, 0xcf9414c4)
                                                                             : Overview_Check(x, y, 0xff000001, 0x18ffffff);
    case 0x1a:
        return Map_CellLow3(g_map, 0, x, y) ? Overview_Check(x, y, 0xff000001, 0x18ffffff)
                                                                        : Overview_Check(x, y, 0x1fb9f4a4, 0xcf9414c4);
    case 0x1b:
        return (uint32_t)(F<uint8_t*>(t, 0x162d)[((F<int32_t>(t, 0x15d9) >> (F<uint32_t>(t, 0x15e1) & 0x1f)) + 1) * y + x] | 0x7f00) << 16;
    case 0x1c:
        return Terrain_KindIs3Or5(g_map, 0, x, y) ? Overview_Check(x, y, 0x1fb9f4a4, 0xcf9414c4)
                                                                             : Overview_Check(x, y, 0xff000001, 0x18ffffff);
    case 0x1d:
        return Grid_CellMarked(g_map, 0, x, y) ? Overview_Check(x, y, 0x1fb9f4a4, 0xcf9414c4)
                                                                          : Overview_Check(x, y, 0xff000001, 0x18ffffff);
    case 0x15: case 0x25: case 0x26: case 0x27:
        return (((x >> 2) + (y >> 2)) & 1) ? Overview_Check(x, y, 0xff708f71, 0x168fff8f) : Overview_Check(x, y, 0xff000001, 0xeffffff);
    case 0x16: {
        float fy = (float)(y * 0x5c), fx = (float)(x * 0x5c);
        float h = (float)Terrain_HeightAt(t, 0, fx, fy);
        h = (float)(h * D(0x5c86b8));
        if (h < Fl_207(0x5c8598)) h = 0;
        else if (h > Fl_207(0x5ca0ec)) h = 255.0f;
        uint32_t b = (uint8_t)Ftol(h);
        return ((b | 0xffffff00) << 8 | b) << 8 | b;
    }
    case 0x28: case 0x2e: case 0x2f: case 0x30: {
        uint32_t c = Overview_Check(x, y, 0xff000001, 0x18ffffff);
        if (((sy >> 2) + (sx >> 2)) & 1) c = (c | 0xff) + 0x10000000;
        return c;
    }
    case 0x2d: {
        uint8_t bits = Terrain_GetFlag(g_map, 0, sx >> 2, sy >> 2);
        uint8_t bit = (uint8_t)(1 << (((sy >> 1) & 1) * 2 + ((sx >> 1) & 1)));
        uint32_t c = (bit & bits) ? Overview_Check(x, y, 0x30000000, 0xbfff0000) : Overview_Check(x, y, 0xff000001, 0x18ffffff);
        if (((sx >> 2) + (sy >> 2)) & 1) c = (c | 0xff) + 0x10000000;
        return c;
    }
    case 0x29: case 0x2a: case 0x2b: case 0x2c: {
        if (mode == 0x29 && Map_CellFlag(t, 0, sx, sy, 2) && Map_CellFlag(t, 0, sx, sy, 1)) return Overview_Check(x, y, 0xe0000000, 0xcfff1f1f);
        mode = F<int32_t>(t, 0x2175);
        if ((mode == 0x29 || mode == 0x2b) && Map_CellFlag(t, 0, sx, sy, 2)) return Overview_Check(x, y, 0xe0000000, 0x9fcf1f1f);
        mode = F<int32_t>(t, 0x2175);
        if ((mode == 0x29 || mode == 0x2a) && Map_CellFlag(t, 0, sx, sy, 1)) return ~((uint32_t)(-(x + y)) << 29) & 0xef1f1fff | 0xcf1f1fff;
        mode = F<int32_t>(t, 0x2175);
        if ((mode == 0x29 || mode == 0x2c) && Map_CellFlag(t, 0, sx, sy, 3)) return Overview_Check(x, y, 0x1fb9f4a4, 0xcf9414c4);
        uint32_t c = Overview_Check(x, y, 0xff000001, 0x18ffffff);
        if (((sy >> 2) + (sx >> 2)) & 1) c += 0x40000000;
        return c;
    }
    default:
        if (mode - 1u > 0x2f) return prev;
        return Overview_Check(x, y, 0xff000001, 0x18ffffff);
    }
}

static inline uint8_t* Coast_Rec(uint8_t* m, int32_t x, int32_t y) { return F<uint8_t*>(m, 0x165d) + (F<int32_t>(m, 0x15dd) * y + x) * 3; }

static inline int32_t Coast_IsLand(uint8_t* m, int32_t x, int32_t y) { return (Coast_Rec(m, x, y)[0] & 7) != 0; }

// Coast reach in cells of vertex (x, y): 4 for kinds 1/4, 3 for 3/5, else 2.
static inline int32_t Coast_Reach(uint8_t* m, int32_t x, int32_t y) {
    if (Terrain_KindIs1Or4(m, 0, x, y)) return 4;
    if (Terrain_KindIs3Or5(m, 0, x, y)) return 3;
    Terrain_KindIs2Or4Or5(m, 0, x, y);
    return 2;
}

// Sets the low nibble of the +2 record byte of edge vertex (x, y) (and of
// the duplicated border records past the last column/row) to v.
static inline void Coast_SetEdgeNibble(uint8_t* m, int32_t x, int32_t y, uint8_t v) {
    uint8_t* p = Coast_Rec(m, x, y) + 2;
    *p = (*p & 0xf0) | v;
    if (x == F<int32_t>(m, 0x2155) - 1) {
        p = Coast_Rec(m, x, y) + 5;
        *p = (*p & 0xf0) | v;
    }
    if (y == F<int32_t>(m, 0x2159) - 1) {
        p = Coast_Rec(m, x, y + 1) + 2;
        *p = (*p & 0xf0) | v;
    }
    if (x == F<int32_t>(m, 0x2155) - 1 && y == F<int32_t>(m, 0x2159) - 1) {
        p = Coast_Rec(m, x, y + 1) + 5;
        *p = (*p & 0xf0) | v;
    }
}

// Engine counters of the overview modes (engine +0x11425a..).
static inline int32_t& Overview_Count(uint32_t off) { return F<int32_t>(g_engine, off); }

// Sets the overlay flag +0x219d, counting it while +0x21a1 is set.
static inline void Overview_SetOverlay(uint8_t* t, int32_t on) {
    if (on) {
        if (F<int32_t>(t, 0x219d) == 1) return;
        F<int32_t>(t, 0x219d) = 1;
        if (!F<int32_t>(t, 0x21a1)) return;
        ++Overview_Count(0x114272);
        ++Overview_Count(0x114266);
    } else {
        if (!F<int32_t>(t, 0x219d)) return;
        F<int32_t>(t, 0x219d) = 0;
        if (!F<int32_t>(t, 0x21a1)) return;
        --Overview_Count(0x114272);
        --Overview_Count(0x114266);
    }
}

// Shared tail of 0x453370 / 0x453510: leaves the current mode of map m and
// shows the plain mode 0x15 (whole map, all filters off).
static void Overview_ShowPlain(uint8_t* m) {
    Render_DestroyColourMap(m);
    if (F<int32_t>(m, 0x2175) == 6) Table_RefreshAll(g_buildings);
    if (F<int32_t>(m, 0x2175) == 7) {
        --Overview_Count(0x11425a);
        --Overview_Count(0x114262);
        Overview_SetOverlay(m, 0);
    }
    if (F<int32_t>(m, 0x2175) == 3) {
        --Overview_Count(0x11425e);
        --Overview_Count(0x114262);
        Buildings_UpdateFor(-1);
        Overview_SetOverlay(m, 0);
    }
    const uint32_t* modes = (const uint32_t*)0x5ee5e8;
    if (modes[F<int32_t>(m, 0x2175) * 3]) --Overview_Count(0x11426e);
    if (modes[0x15 * 3]) ++Overview_Count(0x11426e);
    F<int32_t>(m, 0x2175) = 0x15;
    F<int32_t>(m, 0x2179) = 0;
    F<int32_t>(m, 0x217d) = 0;
    F<int32_t>(m, 0x2181) = 0;
    F<int32_t>(m, 0x2185) = 0;
    F<int32_t>(m, 0x2189) = 0;
    F<int32_t>(m, 0x218d) = 99999;
    F<int32_t>(m, 0x2191) = 99999;
    Overview_Redraw(m, 0, nullptr);
    if (F<int32_t>(m, 0x2175) == 3 && F<int32_t>(m, 0x2181)) Buildings_RefreshHighlight(g_buildings, 0, F<int32_t>(m, 0x2181));
    Map_ClearFogMode(m);
}

static void Pump() {
    Loading_Pump((uint8_t*)(hook::As<void*>(g_app)), 0, 0, 0);
}

static void Method(uint32_t address, void* self) { hook::Orig<void(__fastcall*)(void*)>(address)(self); }

static void DeleteGlobal(uint32_t slot, uint32_t destructor, bool clearEmpty = true) {
    void* p = hook::Global<void*>(slot);
    if (p) {
        Method(destructor, p);
        crt::operator_delete(p);
    }
    if (p || clearEmpty) hook::Global<void*>(slot) = nullptr;
}

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x448260. Initialize runtime overlays and object lists in four observable
// loading-pump phases, install the global map reference, then init the terrain.
RT3_NOINLINE void __fastcall Map_InitRuntimeState(uint8_t* map) {
    const uint32_t first[] = {0x2125, 0x2165, 0x2205, 0x2169, 0x21a5, 0x2195};
    Clear(map, first);
    F<uint32_t>(map, 0x2199) = 0x3f800000;
    const uint32_t rest[] = {0x219d, 0x21a1, 0x2175, 0x2179, 0x217d, 0x2181, 0x2171};
    Clear(map, rest);
    Pump();
    const uint32_t second[] = {
        0x2159, 0x2155, 0x2149, 0x2145, 0x2151, 0x214d, 0x2161, 0x215d,
        0x21a9, 0x21fd, 0x21c1, 0x21c6, 0x21ca, 0x21ce, 0x21d2, 0x21d6
    };
    Clear(map, second);
    map[0x21c5] = 0;
    Pump();
    F<uint32_t>(map, 0x2191) = F<uint32_t>(map, 0x218d) =
        F<uint32_t>(map, 0x2189) = F<uint32_t>(map, 0x2185) = 0xffffffff;
    const uint32_t third[] = {0x2141, 0x2139, 0x213d, 0x2131, 0x212d, 0x2129, 0x2135};
    Clear(map, third);
    F<uint32_t>(map, 0x2201) = 1;
    Pump();
    memset(map + 0x21da, 0, 35);
    g_map = map;
    Pump();
    Method(0x532590, map);
}
RT3_REPLACE(0x448260, Map_InitRuntimeState);

// 0x4483d0. Frees the map's buffers and the track collection (once, flag +0x2165).
RT3_NOINLINE void __fastcall Map_Free(uint8_t* m) {
    if (!F<int32_t>(m, 0x2165)) return;
    F<int32_t>(m, 0x2165) = 0;
    Map_FreeCellKeyLists(m);
    if (Collection* t = g_tracks) {
        Tracks_Destruct(t);
        crt::operator_delete(t);
        g_tracks = nullptr;
    }
    static const uint32_t kBuffers[] = { 0x2141, 0x2139, 0x213d, 0x212d, 0x2129, 0x2135, 0x2131 };
    for (uint32_t off : kBuffers) crt::free(F<void*>(m, off));
}
RT3_REPLACE(0x4483d0, Map_Free);

// 0x448470. Allocates the map's grids once (+0x2165) from the vertex size
// (+0x15d5/+0x15d9): tile counts, 4x4 block counts, the track table (0x6cfca8),
// three bit planes, the tile flags (all 8), the vertex grid, the block grid (enabled)
// and the region cells (0x1d7 bytes each), then resets the cells.
RT3_NOINLINE void __fastcall Map_AllocGrids(uint8_t* m) {
    if (F<int32_t>(m, 0x2165)) return;
    int32_t h = F<int32_t>(m, 0x15d9) - 1, w = F<int32_t>(m, 0x15d5) - 1;
    int32_t bh = (h + 3) / 4, bw = (w + 3) / 4;
    F<int32_t>(m, 0x2159) = w;
    F<int32_t>(m, 0x2151) = bw;
    F<int32_t>(m, 0x2149) = bw;
    F<int32_t>(m, 0x2165) = 1;
    F<int32_t>(m, 0x2155) = h;
    F<int32_t>(m, 0x214d) = bh;
    F<int32_t>(m, 0x2145) = bh;
    F<int32_t>(m, 0x215d) = h * 4;
    F<int32_t>(m, 0x2161) = w * 4;
    F<int32_t>(m, 0x21a9) = w * h;
    void* p = crt::operator_new(0x20a);
    g_tracks =
        p ? Tracks_Construct((Collection*)p, 0, F<int32_t>(m, 0x2155), F<int32_t>(m, 0x2159)) : nullptr;
    int32_t bits = (F<int32_t>(m, 0x215d) * F<int32_t>(m, 0x2161) + 7) / 8;
    int32_t tiles = F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155);
    int32_t blocks = F<int32_t>(m, 0x214d) * F<int32_t>(m, 0x2151) * 4;
    int32_t verts = (F<int32_t>(m, 0x2159) + 1) * (F<int32_t>(m, 0x2155) + 1);
    int32_t cells = F<int32_t>(m, 0x2145) * F<int32_t>(m, 0x2149) * 0x1d7;
    F<uint8_t*>(m, 0x2139) = ZeroedAlloc(bits);
    F<uint8_t*>(m, 0x213d) = ZeroedAlloc(bits);
    F<uint8_t*>(m, 0x2141) = ZeroedAlloc(bits);
    F<uint8_t*>(m, 0x2131) = ZeroedAlloc(tiles);
    for (int32_t i = 0; i < F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155); ++i) {
        uint8_t* t = F<uint8_t*>(m, 0x2131) + i;
        *t = (*t & 7) | 8;
    }
    F<uint8_t*>(m, 0x2135) = ZeroedAlloc(verts);
    F<uint8_t*>(m, 0x212d) = ZeroedAlloc(blocks);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2151); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x214d); ++x) {
            F<uint8_t*>(m, 0x212d)[(F<int32_t>(m, 0x214d) * y + x) * 4] = 1;
            F<uint8_t*>(m, 0x212d)[(F<int32_t>(m, 0x214d) * y + x) * 4 + 3] = 0;
        }
    F<uint8_t*>(m, 0x2129) = (uint8_t*)crt::malloc(cells);
    Map_ResetCells(m);
}
RT3_REPLACE(0x448470, Map_AllocGrids);

// 0x448740. Map m (sx x sy cells) loads the tree types, then creates the
// world tables 0x6ada80, 0x6cea50, 0x62bae0, 0x6cfc9c, 0x62ba84, 0x62b26c,
// 0x6ada90, 0x6cec20, 0x6cfcb4 (routes) and 0x62be18, pumping the loading
// screen around each, and the camera 0x62be68 looking at cell
// (min(sx / 2 + 10, 146), min(sy / 2 + 10, 128)) from 768 units.
RT3_NOINLINE void __fastcall World_CreateTables(uint8_t* m, int, int32_t sx, int32_t sy) {
    TreeTypes_Load(m);
    auto pump = [] { Loading_Pump(g_app, 0, 0, 0); };
    auto make = [&](size_t size, Collection* (__fastcall* ctor)(Collection*), uint32_t global) {
        pump();
        Collection* c = (Collection*)crt::operator_new(size);
        hook::Global<Collection*>(global) = c ? ctor(c) : nullptr;
    };
    make(0x8c, Collection5ce044_Construct, 0x6ada80);
    make(0x8c, Collection5ce4a8_Construct, 0x6cea50);
    make(0x90, Collection5c9ae0_Construct, 0x62bae0);
    make(0x8c, Collection5cf10c_Construct, 0x6cfc9c);
    make(0x8c, Collection5c9810_Construct, 0x62ba84);
    make(0x8c, Collection5c9414_Construct, 0x62b26c);
    make(0x8c, Collection5ce130_Construct, 0x6ada90);
    make(0x8c, Collection5ce90c_Construct, 0x6cec20);
    make(0x9c, Routes_Construct, 0x6cfcb4);
    make(0x8c, Collection5c9cec_Construct, 0x62be18);
    pump();
    double a = (double)sx * *(const double*)0x5c8628 + *(const double*)0x5c85f0;
    double x = *(const double*)0x5ca9f0 < a ? 146.0 : a;
    int32_t cx = (int32_t)x;
    double b = (double)sy * *(const double*)0x5c8628 + *(const double*)0x5c85f0;
    double y = *(const double*)0x5ca9e8 < b ? 128.0 : b;
    uint8_t* cam = (uint8_t*)crt::operator_new(0x3ba);
    if (cam) {
        float fz = (float)((int32_t)y * 0x5c), fx = (float)(cx * 0x5c);
        cam = Camera_ConstructView(cam, 0, (uint32_t)(uintptr_t)m, *(uint32_t*)&fx, 0, *(uint32_t*)&fz, 0x44400000);
    }
    g_62be68 = cam;
    pump();
}
RT3_REPLACE(0x448740, World_CreateTables);

// 0x448a70. Writes a byte in the coarse grid (+0x162d; row length (width >> shift) + 1).
RT3_NOINLINE void __fastcall Map_SetCoarseByte(void* m, int, int32_t x, int32_t y, uint8_t v) {
    int32_t row = (F<int32_t>(m, 0x15d9) >> (F<int32_t>(m, 0x15e1) & 31)) + 1;
    F<uint8_t*>(m, 0x162d)[row * y + x] = v;
}
RT3_REPLACE(0x448a70, Map_SetCoarseByte);

// 0x448aa0. Float grid (+0x1605, row length +0x15d9 + 1) at (x, y).
RT3_NOINLINE double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y) {
    return F<float*>(m, 0x1605)[(F<int32_t>(m, 0x15d9) + 1) * y + x];
}
RT3_REPLACE(0x448aa0, Map_GridFloat);

RT3_NOINLINE int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y) { return Scene_FocusCell(Renderer(), 0, x, y) == 1 ? 1 : 0; }
RT3_REPLACE(0x448ac0, Scene_FocusFlagIs1);

// 0x448af0. Map cell flags: which 0 = A or B, 1 = B, 2 = A, 3 = C (bitsets
// +0x2139/+0x213d/+0x2141). Outside the map counts as set.
RT3_NOINLINE int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which) {
    int32_t stride = F<int32_t>(m, 0x215d);
    int32_t idx = stride * y + x;
    if (idx < 0 || idx >= F<int32_t>(m, 0x2161) * stride) return 1;
    auto bit = [&](uint32_t off) { return F<uint8_t*>(m, off)[idx >> 3] & (1 << (idx & 7)); };
    int32_t a = 0, b = 0, c = 0;
    if (which == 0 || which == 2) a = bit(0x2139);
    if (which == 0 || which == 1) b = bit(0x213d);
    if (which == 3) c = bit(0x2141);
    return (a || b || c) ? 1 : 0;
}
RT3_REPLACE(0x448af0, Map_CellFlag);

// 0x448bd0. Terrain height at tile coordinates (x 0x5c8700 world units).
RT3_NOINLINE double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z) {
    const float k = *(const float*)0x5c8700;
    float wz = (float)((double)z * k);
    float wx = (float)((double)x * k);
    return Terrain_HeightAt(r, 0, wx, wz);
}
RT3_REPLACE(0x448bd0, Terrain_HeightAtTile);

// 0x448c00..0x448f30. Grid accessors: float grid +0x1605 (stride
// +0x15d9 + 1), 3-byte cells +0x165d and byte grids +0x1661 / +0x1665
// (stride +0x15dd).
RT3_NOINLINE double __fastcall Grid_Float1605(uint8_t* g, int, int32_t x, int32_t y) {
    return F<float*>(g, 0x1605)[(F<int32_t>(g, 0x15d9) + 1) * y + x];
}
RT3_REPLACE(0x448c00, Grid_Float1605);

RT3_NOINLINE void __fastcall Terrain_MarkFlooded(uint8_t* t, int, int32_t x, int32_t y) {
    if (!Terrain_KindIs1Or4(t, 0, x, y)) MarkWater(t, x, y, 0xfffc, 4, 0xfff9, 1);
}
RT3_REPLACE(0x448c20, Terrain_MarkFlooded);

RT3_NOINLINE void __fastcall Terrain_MarkDrained(uint8_t* t, int, int32_t x, int32_t y) {
    if (!Terrain_KindIs3Or5(t, 0, x, y)) MarkWater(t, x, y, 0xfffd, 5, 0xfffb, 3);
}
RT3_REPLACE(0x448cb0, Terrain_MarkDrained);

// 0x448d40. Paints terrain vertex (x, y) with the current brush (+0x1554) in layer
// +0x1655, clears the low 3 bits of its +0x165d record and notifies the main camera.
RT3_NOINLINE uint32_t __fastcall Terrain_PaintVertex(uint8_t* r, int, int32_t x, int32_t y) {
    uint8_t v = (uint8_t)Render_Get1554(r);
    int32_t i = F<int32_t>(r, 0x15dd) * y + x;
    F<uint8_t*>(r, 0x1655)[i] = v;
    F<uint8_t*>(r, 0x165d)[(F<int32_t>(r, 0x15dd) * y + x) * 3] &= 0xf8;
    void* cam = hook::As<void*>(g_62be68);
    return ((uint32_t(__thiscall*)(void*))(*(void***)cam)[0])(cam);
}
RT3_REPLACE(0x448d40, Terrain_PaintVertex);

RT3_NOINLINE void __fastcall Grid_MarkCell(uint8_t* g, int, int32_t x, int32_t y) {
    *Cell165d(g, x, y) |= 0x10;
    void* o = hook::As<void*>(g_62be68);
    ((void(__thiscall*)(void*))(*(void***)o)[0])(o);
}
RT3_REPLACE(0x448d90, Grid_MarkCell);

// 0x448dc0. Clears flag 0x10 of cell (x, y)'s third byte (3-byte cells
// +0x165d, row +0x15dd) and tells the camera (0x62be68 slot 0).
RT3_NOINLINE void __fastcall Map_ClearCellFlag10(uint8_t* m, int, int32_t x, int32_t y) {
    (F<uint8_t*>(m, 0x165d) + (F<int32_t>(m, 0x15dd) * y + x) * 3)[2] &= 0xef;
    uint8_t* c = g_62be68;
    ((void(__fastcall*)(uint8_t*))(*(void***)c)[0])(c);
}
RT3_REPLACE(0x448dc0, Map_ClearCellFlag10);

RT3_NOINLINE uint8_t __fastcall Grid_CellLow(uint8_t* g, int, int32_t x, int32_t y) { return *Cell165d(g, x, y) & 0xf; }
RT3_REPLACE(0x448df0, Grid_CellLow);

RT3_NOINLINE void __fastcall Grid_SetCellLow(uint8_t* g, int, int32_t x, int32_t y, uint8_t v) {
    uint8_t* c = Cell165d(g, x, y);
    *c ^= (uint8_t)((*c ^ v) & 0xf);
}
RT3_REPLACE(0x448e20, Grid_SetCellLow);

RT3_NOINLINE void __fastcall Grid_SetByte1661(uint8_t* g, int, int32_t x, int32_t y, uint8_t v) {
    F<uint8_t*>(g, 0x1661)[F<int32_t>(g, 0x15dd) * y + x] = v;
}
RT3_REPLACE(0x448e60, Grid_SetByte1661);

RT3_NOINLINE void __fastcall Grid_SetByte1665(uint8_t* g, int, int32_t x, int32_t y, uint8_t v) {
    F<uint8_t*>(g, 0x1665)[F<int32_t>(g, 0x15dd) * y + x] = v;
}
RT3_REPLACE(0x448e90, Grid_SetByte1665);

RT3_NOINLINE uint8_t __fastcall Grid_Byte1661(uint8_t* g, int, int32_t x, int32_t y) {
    return F<uint8_t*>(g, 0x1661)[F<int32_t>(g, 0x15dd) * y + x];
}
RT3_REPLACE(0x448ec0, Grid_Byte1661);

RT3_NOINLINE uint8_t __fastcall Grid_Byte1665(uint8_t* g, int, int32_t x, int32_t y) {
    return F<uint8_t*>(g, 0x1665)[F<int32_t>(g, 0x15dd) * y + x];
}
RT3_REPLACE(0x448ee0, Grid_Byte1665);

// 0x448f00. Low 3 bits of the cell's 3-byte record (+0x165d, stride +0x15dd) set.
RT3_NOINLINE int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y) {
    int32_t i = F<int32_t>(m, 0x15dd) * y + x;
    return (F<uint8_t*>(m, 0x165d)[i * 3] & 7) ? 1 : 0;
}
RT3_REPLACE(0x448f00, Map_CellLow3);

RT3_NOINLINE int32_t __fastcall Grid_CellMarked(uint8_t* g, int, int32_t x, int32_t y) { return (*Cell165d(g, x, y) & 0x10) ? 1 : 0; }
RT3_REPLACE(0x448f30, Grid_CellMarked);

// 0x448f60. Loads the four terrain layers (chunks 0x9471..0x9472).
RT3_NOINLINE void __fastcall Terrain_LoadLayers(uint8_t* r, int, Stream s) {
    StreamSkip4(s, 0x9471, 0, 0);
    uint32_t n = VertexCount(r);
    void* layer[4];
    for (int k = 0; k < 4; ++k) layer[k] = (void*)Terrain_Layer(r, 0, (uint8_t)k);
    for (int k = 0; k < 4; ++k) StreamRead(s, layer[k], n, 0, 1);
    StreamSkip4(s, 0x9472, 0, 0);
}
RT3_REPLACE(0x448f60, Terrain_LoadLayers);

// 0x448fe0. Saves the four terrain layers.
RT3_NOINLINE void __fastcall Terrain_SaveLayers(uint8_t* r, int, Stream s) {
    StreamWriteU32(s, 0x9471);
    uint32_t n = VertexCount(r);
    void* layer[4];
    for (int k = 0; k < 4; ++k) layer[k] = (void*)Terrain_Layer(r, 0, (uint8_t)k);
    for (int k = 0; k < 4; ++k) StreamWrite(s, layer[k], n, 0);
    StreamWriteU32(s, 0x9472);
}
RT3_REPLACE(0x448fe0, Terrain_SaveLayers);

// 0x449050. Darkness of a brightness byte: (255 - b - 90) / 165, 0 when b >= 165.
RT3_NOINLINE float __stdcall DarknessOf(uint8_t b) {
    int32_t d = 0xff - b;
    if (d <= 0x5a) return 0.0f;
    float f = (float)(d - 0x5a);
    return (float)((double)f / *(const float*)0x5ca9f8);
}
RT3_REPLACE(0x449050, DarknessOf);

// 0x4490a0. Brightness byte of a darkness fraction: 0xff for <= 0, 0 for >= 1,
// else 165 - 165 f (AL only on the first two paths).
RT3_NOINLINE int32_t __stdcall BrightnessOf(float f) {
    if (!(f > *(const float*)0x5c8598)) return 0xff;
    if (!((double)f < *(const double*)0x5c8770)) return 0;
    const float k = *(const float*)0x5ca9f8;
    int32_t v = (int32_t)((double)k - (double)f * k);
    return v < 0 ? 0 : v;
}
RT3_REPLACE(0x4490a0, BrightnessOf);

int32_t __fastcall Map_PointCount(void* m) { return (F<int32_t>(m, 0x15d9) + 1) * (F<int32_t>(m, 0x15d5) + 1); }
RT3_REPLACE(0x4490f0, Map_PointCount);

// 0x449110. Terrain roughness (cached in +0x21c1): mean absolute height step
// along the rows of the height grid +0x1605, divided by 15.
float __fastcall Grid_Roughness(uint8_t* g) {
    if (!(F<float>(g, 0x21c1) == *(const float*)0x5c8598)) return F<float>(g, 0x21c1);
    float sum = 0.0f;
    for (int32_t y = 0; y < F<int32_t>(g, 0x15d5); ++y) {
        float prev = 0.0f;
        for (int32_t x = 0; x < F<int32_t>(g, 0x15d9); ++x) {
            float v = F<float*>(g, 0x1605)[(F<int32_t>(g, 0x15d9) + 1) * y + x];
            double d = crt::fabs((double)prev - v);
            sum = (float)(d + sum);
            prev = v;
        }
    }
    int32_t n = F<int32_t>(g, 0x15d9) * F<int32_t>(g, 0x15d5);
    if (n < 1) n = 1;
    float fn = (float)n;
    float avg = (float)((double)sum / fn / *(const double*)0x5c8798);
    F<float>(g, 0x21c1) = avg;
    return avg;
}
RT3_REPLACE(0x449110, Grid_Roughness);

// 0x449200. Map m creates a new w x h terrain with texture set tex: resets
// its runtime state, builds the terrain and the world tables (for w - 1 x
// h - 1 cells), counts a loading step (0xccb9e8 += 4), allocates the grids
// (+0x2125 = 1), clears 0x6ceba4.. and reapplies the graphics options.
RT3_NOINLINE void __fastcall Map_Create(uint8_t* m, int, int32_t w, int32_t h, const char* tex) {
    Map_InitRuntimeState(m);
    Loading_Pump(g_app, 0, 0, 0);
    Terrain_Create(m, 0, w, h, tex);
    World_CreateTables(m, 0, w - 1, h - 1);
    g_skipCount += 4;
    F<int32_t>(m, 0x2125) = 1;
    Map_AllocGrids(m);
    ClearGlobals6ceba4();
    Options_ApplyGraphics(g_app, 0, -2, 0);
}
RT3_REPLACE(0x449200, Map_Create);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x449270. Unload the world in the original dependency order. Pump callbacks
// and destructors can change global slots; load each when needed and free the
// cached object before clearing its slot. Preserve all nine pump boundaries.
RT3_NOINLINE void __fastcall Map_UnloadWorld(uint8_t* map) {
    if (!F<uint32_t>(map, 0x2125)) return;
    Pump();
    PreviewModels_Free();
    DeleteGlobal(0x6ada80, 0x461560);
    DeleteGlobal(0x6cea50, 0x474480);
    Pump();
    DeleteGlobal(0x62ba84, 0x41d320);
    Pump();
    DeleteGlobal(0x6ada90, 0x468140);
    DeleteGlobal(0x62b26c, 0x413260);
    DeleteGlobal(0x6cec20, 0x4811f0);
    DeleteGlobal(0x62bae0, 0x421470);
    Pump();
    DeleteGlobal(0x6cfca8, 0x493ba0, false);
    DeleteGlobal(0x6cfcb4, 0x4a41a0, false);
    DeleteGlobal(0x6cfc9c, 0x487c00);
    DeleteGlobal(0x62be18, 0x432e80, false);
    Pump();
    DeleteGlobal(0x62be68, 0x43a560);
    Method(0x4539c0, map);
    Pump();
    map[0x21c5] = 0;
    Method(0x453510, map);
    Method(0x537330, map);
    Pump();
    Method(0x4483d0, map);
    Pump();
    hook::Orig<void(*)()>(0x40b7d0)();
    Pump();
    F<uint32_t>(map, 0x2125) = 0;
}
RT3_REPLACE(0x449270, Map_UnloadWorld);

// 0x4494c0. Resizes the map to w x h cells when the size changes: frees the
// map grids, resizes the terrain and allocates the grids again.
void __fastcall Map_Resize(uint8_t* m, int, int32_t w, int32_t h) {
    if (w == F<int32_t>(m, 0x15d9) && h == F<int32_t>(m, 0x15d5)) return;
    Map_Free(m);
    Terrain_Resize(m, 0, w, h);
    Map_AllocGrids(m);
}
RT3_REPLACE(0x4494c0, Map_Resize);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x449500. Restore the map class, unload its runtime state, then destroy its base.
RT3_NOINLINE void __fastcall Map_RuntimeDestructor(uint8_t* map) {
    F_runtime_wrappers<uint32_t>(map, 0) = 0x5ca9fc;
    Map_UnloadWorld(map);
    Terrain_Destructor(map);
}
RT3_REPLACE(0x449500, Map_RuntimeDestructor);

// 0x449520. Saves map w to stream s: tag 0x2ee2 and the terrain (picture
// `name`, quality 200), the grid sizes (+0x2145..+0x2161, +0x2201), the
// region bit maps (+0x213d, +0x2141), cell and corner maps (+0x2131,
// +0x2135), region index map +0x212d, the terrain layers and every
// region's cargo, with section tags and progress log lines.
RT3_NOINLINE void __fastcall Map_Save(uint8_t* w, int, Stream s, const char* name) {
    uint32_t tag = 0x2ee2;
    StreamWrite(s, &tag, 4, 0);
    Terrain_Save(w, 0, s, name, 0xc8);
    Save_LogProgress(s, (const char*)0x5caa50);
    StreamWriteU32(s, 0x2ee2);
    static const uint32_t sizes[] = { 0x2145, 0x2149, 0x214d, 0x2151, 0x2155, 0x2159, 0x215d, 0x2161, 0x2201 };
    for (uint32_t off : sizes) StreamWrite(s, w + off, 4, 0);
    StreamWriteU32(s, 0x2ee3);
    int32_t bits = (F<int32_t>(w, 0x2161) * F<int32_t>(w, 0x215d) + 7) / 8;
    int32_t cells = F<int32_t>(w, 0x2155) * F<int32_t>(w, 0x2159);
    int32_t corners = (F<int32_t>(w, 0x2155) + 1) * (F<int32_t>(w, 0x2159) + 1);
    uint32_t regions = (uint32_t)(F<int32_t>(w, 0x2151) * F<int32_t>(w, 0x214d)) << 2;
    StreamWriteU32(s, 0x2ef4);
    StreamWrite(s, F<void*>(w, 0x213d), (uint32_t)bits, 0);
    StreamWriteU32(s, 0x2ef5);
    StreamWriteU32(s, 0x2ef6);
    StreamWrite(s, F<void*>(w, 0x2131), (uint32_t)cells, 0);
    StreamWrite(s, F<void*>(w, 0x2135), (uint32_t)corners, 0);
    StreamWriteU32(s, 0x2ee4);
    Save_LogProgress(s, (const char*)0x5caa40);
    StreamWrite(s, F<void*>(w, 0x2141), (uint32_t)bits, 0);
    StreamWriteU32(s, 0x2ee5);
    StreamWrite(s, F<void*>(w, 0x212d), regions, 0);
    Save_LogProgress(s, (const char*)0x5caa34);
    StreamWriteU32(s, 0x2f43);
    Terrain_SaveLayers(w, 0, s);
    Save_LogProgress(s, (const char*)0x5caa18);
    StreamWriteU32(s, 0x2f44);
    for (int32_t y = 0; y < F<int32_t>(w, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(w, 0x2145); ++x) Region_SaveCargo(F<uint8_t*>(w, 0x2129) + (F<int32_t>(w, 0x2145) * y + x) * 0x1d7, 0, s);
    Save_LogProgress(s, (const char*)0x5caa0c);
    StreamWriteU32(s, 0x2ee5);
}
RT3_REPLACE(0x449520, Map_Save);

// 0x4497a0. Sets or clears fine cell (x, y) in bit grid `which` (0 = +0x2139 and
// +0x213d, 1 = +0x213d, 2 = +0x2139, 3 = +0x2141); with `update` refreshes the
// block mask of the cell's block and of the neighbours it borders.
RT3_NOINLINE void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update) {
    int32_t w = F<int32_t>(m, 0x215d);
    int32_t idx = w * y + x;
    if (idx < 0 || idx >= F<int32_t>(m, 0x2161) * w) return;
    if (which == 0 || which == 2) SetBit(F<uint8_t*>(m, 0x2139), idx, on);
    if (which == 0 || which == 1) SetBit(F<uint8_t*>(m, 0x213d), idx, on);
    if (which == 3) SetBit(F<uint8_t*>(m, 0x2141), idx, on);
    if (!update) return;
    int32_t bx = x >> 2, by = y >> 2;
    if (bx <= 0 || by <= 0 || bx >= F<int32_t>(m, 0x2155) || by >= F<int32_t>(m, 0x2159)) return;
    Map_UpdateBlockMask(m, 0, bx, by);
    if (bx >= F<int32_t>(m, 0x2155) - 1 || by >= F<int32_t>(m, 0x2159) - 1) return;
    int32_t xm = x & 3, ym = y & 3;
    if (xm == 0 || xm == 3) {
        int32_t nx = xm == 0 ? bx - 1 : bx + 1;
        Map_UpdateBlockMask(m, 0, nx, by);
        if (ym == 0) {
            Map_UpdateBlockMask(m, 0, bx, by - 1);
            Map_UpdateBlockMask(m, 0, nx, by - 1);
        }
        if (ym == 3) {
            Map_UpdateBlockMask(m, 0, bx, by + 1);
            Map_UpdateBlockMask(m, 0, nx, by + 1);
        }
        return;
    }
    if (ym == 0) Map_UpdateBlockMask(m, 0, bx, by - 1);
    else if (ym == 3) Map_UpdateBlockMask(m, 0, bx, by + 1);
}
RT3_REPLACE(0x4497a0, Map_SetFineBit);

// 0x449980. Whether the vertex is not plain land (kind != 0 or bit 9).
RT3_NOINLINE int32_t __fastcall Terrain_IsSpecial(uint8_t* r, int, int32_t x, int32_t y) {
    if (Rec(r, x, y)[0] & 7) return 1;
    return Terrain_RecBit9(r, 0, x, y) ? 1 : 0;
}
RT3_REPLACE(0x449980, Terrain_IsSpecial);

// 0x4499c0. Whether cell (x, y) or (near the map edge) any of its neighbours
// has water/road bits (low 3 bits of the 3-byte cell grid +0x165d).
RT3_NOINLINE int32_t __fastcall Map_NearFeature(uint8_t* m, int, int32_t x, int32_t y) {
    int edge = !(x > 0 && y > 0 && x < F<int32_t>(m, 0x2155) - 1 && y < F<int32_t>(m, 0x2159) - 1);
    int32_t stride = F<int32_t>(m, 0x15dd);
    const uint8_t* cells = F<uint8_t*>(m, 0x165d);
    if (cells[(stride * y + x) * 3] & 7) return 1;
    const int32_t* dx = (const int32_t*)0x624b28;
    const int32_t* dy = (const int32_t*)0x624b48;
    for (int d = 0; d < 8; ++d) {
        int32_t ny = dy[d] + y, nx = dx[d] + x;
        if (edge) {
            uint8_t* map = g_map;
            if (nx < 0 || nx >= F<int32_t>(map, 0x15d9) - 1) continue;
            if (ny < 0 || ny >= F<int32_t>(map, 0x15d5) - 1) continue;
        }
        if (cells[((dy[d] + y) * stride + dx[d] + x) * 3] & 7) return 1;
    }
    return 0;
}
RT3_REPLACE(0x4499c0, Map_NearFeature);

// 0x449ab0. Whether water (kind 1 / 3..5) lies within `range` steps of (x, y) in
// any of the eight directions (bounds checked only near the map edge).
RT3_NOINLINE int32_t __fastcall Map_WaterWithin(uint8_t* m, int, int32_t x, int32_t y, int32_t range) {
    bool edge = true;
    if ((int32_t)((uint32_t)x - range) >= 0 && (int32_t)((uint32_t)y - range) >= 0 &&
        x + range <= F<int32_t>(m, 0x2155) - 1 && y + range <= F<int32_t>(m, 0x2159) - 1)
        edge = false;
    if (Terrain_KindIs1Or3to5(m, 0, x, y)) return 1;
    for (int32_t d = 0; d < 8; ++d) {
        for (int32_t s = 1; s <= range; ++s) {
            int32_t cx = DirX(d) * s + x, cy = DirY(d) * s + y;
            if (edge && OutsideGrid(cx, cy)) continue;
            if (Terrain_KindIs1Or3to5(m, 0, cx, cy)) return 1;
        }
    }
    return 0;
}
RT3_REPLACE(0x449ab0, Map_WaterWithin);

// 0x449bd0. For a bit-9 vertex: whether it or an orthogonal neighbour is water.
int32_t __fastcall Map_Bit9NearWater(uint8_t* m, int, int32_t x, int32_t y) {
    if (!Terrain_RecBit9(m, 0, x, y)) return 0;
    bool edge = !(x > 0 && y > 0 && x < F<int32_t>(m, 0x2155) - 1 && y < F<int32_t>(m, 0x2159) - 1);
    if (Terrain_KindIs1Or3to5(m, 0, x, y)) return 1;
    for (int32_t d = 0; d < 8; d += 2) {
        int32_t cx = DirX(d) + x, cy = DirY(d) + y;
        if (edge && OutsideGrid(cx, cy)) continue;
        if (Terrain_KindIs2Or4Or5(m, 0, cx, cy)) return 1;
    }
    return 0;
}
RT3_REPLACE(0x449bd0, Map_Bit9NearWater);

// 0x449cb0. Distance (at most 4) from (x, y) to the nearest cell within 4 whose
// terrain kind is not 1 or 4.
RT3_NOINLINE float __fastcall Map_DistanceToOtherKind(uint8_t* m, int, float x, float y) {
    float best = 4.0f;
    int32_t ix = (int32_t)x, iy = (int32_t)y;
    int32_t x0 = ix - 4 < 0 ? 0 : ix - 4;
    int32_t y0 = iy - 4 < 0 ? 0 : iy - 4;
    int32_t x1 = F<int32_t>(m, 0x2155) - 1;
    if (!(x1 < ix + 4)) x1 = ix + 4;
    int32_t y1 = F<int32_t>(m, 0x2159) - 1;
    if (!(y1 < iy + 4)) y1 = iy + 4;
    for (int32_t j = y0; j <= y1; ++j)
        for (int32_t i = x0; i <= x1; ++i) {
            if (Terrain_KindIs1Or4(m, 0, i, j)) continue;
            float d = (float)(Distance2D(x, y, Centre(i), Centre(j)) - D(0x5c8628));
            if (!(best < d)) best = d;
        }
    return best;
}
RT3_REPLACE(0x449cb0, Map_DistanceToOtherKind);

// 0x449df0. Clamps a rect to the map grid (w +0x2155, h +0x2159); true if it is non-empty.
RT3_NOINLINE int32_t __fastcall Map_ClampRect(void* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1) {
    if (*x0 < 0) *x0 = 0;
    if (*y0 < 0) *y0 = 0;
    int32_t maxX = F<int32_t>(m, 0x2155) - 1;
    *x1 = maxX < *x1 ? maxX : *x1;
    int32_t maxY = F<int32_t>(m, 0x2159) - 1;
    *y1 = maxY < *y1 ? maxY : *y1;
    if (*x0 < 0 || *y0 < 0) return 0;
    if (*x1 > F<int32_t>(m, 0x2155) - 1 || *y1 > F<int32_t>(m, 0x2159) - 1) return 0;
    if (*x1 < *x0 || *y1 < *y0) return 0;
    return 1;
}
RT3_REPLACE(0x449df0, Map_ClampRect);

// 0x44d410 keeps using ECX (the map) after the call.
__declspec(naked) int32_t __stdcall Map_ClampRectInclusive(int32_t*, int32_t*, int32_t*, int32_t*) { RT3_KEEP_ECX_EDX(Map_ClampRectInclusive_Impl, 4) }
RT3_REPLACE(0x449e90, Map_ClampRectInclusive);

// 0x449f20. Counts the land vertices into +0x21a9.
RT3_NOINLINE void __fastcall Map_CountLand(uint8_t* m) {
    F<int32_t>(m, 0x21a9) = 0;
    for (int32_t y = 0; y < F<int32_t>(m, 0x2159); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2155); ++x)
            if (!Terrain_KindIs1Or3to5(m, 0, x, y)) F<int32_t>(m, 0x21a9)++;
}
RT3_REPLACE(0x449f20, Map_CountLand);

// 0x449f80. Reclassifies every site (+0x2129, 0x1d7 bytes, +0x2145 x +0x2149),
// then updates every site's edge flag.
RT3_NOINLINE void __fastcall Map_ReclassifySites(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Site_ClassifyWater(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Station_UpdateEdgeFlag(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
}
RT3_REPLACE(0x449f80, Map_ReclassifySites);

// 0x44a030. Sum over all cities (+0x2129, 0x1d7 bytes) with +0xeb set of the quota
// and stock (+0x103) of every enabled cargo (0x62ba8c +0x16e).
RT3_NOINLINE float __fastcall Map_TotalCargoValue(uint8_t* m, int, int32_t) {
    float sum = 0.0f;
    uint32_t n = (uint32_t)(F<int32_t>(m, 0x2149) * F<int32_t>(m, 0x2145));
    uint8_t* city = F<uint8_t*>(m, 0x2129);
    for (; n; --n, city += 0x1d7) {
        if (!F<int32_t>(city, 0xeb)) continue;
        for (int32_t i = 0x34; i >= 0; --i) {
            if (!F<int32_t>(Cargos(), 0x16e + i * 4)) continue;
            float q = (float)City_CargoQuota(city, 0, i);
            sum = (float)((double)q + sum);
            sum = (float)((double)sum + F<float>(city, 0x103 + i * 4));
        }
    }
    return sum;
}
RT3_REPLACE(0x44a030, Map_TotalCargoValue);

// 0x44a0e0. Sets block byte 3 inside the circle.
RT3_NOINLINE void __fastcall Map_MarkBlocks3(uint8_t* m, int, float x, float y, float r, uint8_t v) { MarkBlocks(m, x, y, r, v, 3); }
RT3_REPLACE(0x44a0e0, Map_MarkBlocks3);

// 0x44a320. Sets block byte 0 inside the circle.
RT3_NOINLINE void __fastcall Map_MarkBlocks0(uint8_t* m, int, float x, float y, float r, uint8_t v) { MarkBlocks(m, x, y, r, v, 0); }
RT3_REPLACE(0x44a320, Map_MarkBlocks0);

// 0x44a560. Sets the upper bits (v << 3) of the tile flags (+0x2131) inside the
// circle (tile units); radius 0 marks just the centre tile.
RT3_NOINLINE void __fastcall Map_MarkTiles(uint8_t* m, int, float x, float y, float r, uint8_t v) {
    int32_t x0, x1, y0, y1;
    float t = (float)((double)x - r - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    x0 = (int32_t)t;
    t = (float)((double)y - r - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    y0 = (int32_t)t;
    float mx = (float)(F<int32_t>(m, 0x2155) - 1);
    float u = (float)((double)x + r + Fl(0x5c85b4));
    if (mx < u) u = mx;
    x1 = (int32_t)u;
    float my = (float)(F<int32_t>(m, 0x2159) - 1);
    u = (float)((double)y + r + Fl(0x5c85b4));
    if (my < u) u = my;
    y1 = (int32_t)u;
    for (int32_t ty = y0; ty <= y1; ++ty)
        for (int32_t tx = x0; tx <= x1; ++tx) {
            if (r == Fl(0x5c8598)) {
                if (tx != (int32_t)x || ty != (int32_t)y) continue;
            } else if (Falloff(tx, ty, x, y, r) <= Fl(0x5c8598)) continue;
            uint8_t* p = F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * ty + tx;
            *p = (uint8_t)((v << 3) | (*p & 7));
        }
}
RT3_REPLACE(0x44a560, Map_MarkTiles);

// 0x44a7a0. City owning 4x4 block (x, y) (its region byte when unclaimed); the
// assignment is redone first when marked stale (+0x2201) in the editor.
RT3_NOINLINE int32_t __fastcall Map_BlockCity(uint8_t* m, int, int32_t x, int32_t y) {
    if (Global6d07b4Set2() && F<int32_t>(m, 0x2201)) Cities_AssignBlocks(g_cities);
    uint8_t* g = g_map;
    uint8_t* b = F<uint8_t*>(g, 0x212d) + (F<int32_t>(g, 0x214d) * y + x) * 4;
    int32_t id = F<uint16_t>(b, 1);
    return id ? id : b[0];
}
RT3_REPLACE(0x44a7a0, Map_BlockCity);

// 0x44a800. City owning cell (x, y) (its 4x4 block).
RT3_NOINLINE int32_t __fastcall Map_CellCity(uint8_t* m, int, int32_t x, int32_t y) { return Map_BlockCity(m, 0, x / 4, y / 4); }
RT3_REPLACE(0x44a800, Map_CellCity);

// 0x44a830. Value +0x23a of the untyped region nearest to cell (x, y) (u16, AX only).
RT3_NOINLINE uint16_t __stdcall Regions_NearestValue(int32_t x, int32_t y) {
    Collection* regions = g_cities;
    float best = 4294967296.0f;
    uint16_t sel = 0;
    for (int32_t i = 0; i < Collection_Count(regions); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(regions, 0, Collection_IdAt(regions, 0, i, 0));
        if (F<int32_t>(r, 0x23e)) continue;
        int32_t dx = (int32_t)Obj_CellX(r) - x;
        if (dx < 0) dx = -dx;
        int32_t dy = (int32_t)Obj_CellY(r) - y;
        if (dy < 0) dy = -dy;
        float d = (float)crt::sqrt((double)(dy * dy + dx * dx));
        if (d < best) {
            sel = F<uint16_t>(r, 0x23a);
            best = d;
        }
    }
    return sel;
}
RT3_REPLACE(0x44a830, Regions_NearestValue);

// 0x44a930. Darkness (see 0x449050) of the brightness grid `layer` (+0x1631) at (x, y).
RT3_NOINLINE float __fastcall Grid_Darkness(uint8_t* g, int, int32_t x, int32_t y, int32_t layer) {
    const uint8_t* grid = F<uint8_t*>(g, 0x1631 + layer * 4);
    int32_t d = 0xff - grid[(F<int32_t>(g, 0x15d9) + 1) * y + x];
    if (d <= 0x5a) return 0.0f;
    float f = (float)(d - 0x5a);
    return (float)((double)f / *(const float*)0x5ca9f8);
}
RT3_REPLACE(0x44a930, Grid_Darkness);

// 0x44a9a0. Paints the four ground-texture layers (+0x1631, one byte per vertex) at
// vertex (x, y) (not on the map edge) with strength s: mode 3 fades all (to 0 under
// a floor), mode 2 boosts all (to 1 at the cap), mode 0 blends them toward `w`;
// mode 1 picks one layer by a hashed random draw over the shares `p` (the 4th by its
// own draw) and blends it toward its `w` (all layers with `p` summing >= 1).
RT3_NOINLINE void __fastcall Terrain_PaintLayers(uint8_t* t, int, int32_t x, int32_t y, float s, int32_t mode, const float* w,
                                    const float* p) {
    int32_t W = F<int32_t>(t, 0x15d9);
    int32_t idx = (W + 1) * y + x;
    if (x <= 0 || y <= 0 || x >= W - 1 || y >= F<int32_t>(t, 0x15d5) - 1) return;
    auto layer = [&](int32_t j) { return F<uint8_t*>(t, 0x1631 + j * 4) + idx; };
    if (mode == 3) {
        double k = D(0x5c8770) - s;
        for (int32_t j = 0; j < 4; ++j) {
            uint8_t* q = layer(j);
            float v = DarknessOf(*q);
            v = (float)(v * k);
            *q = (uint8_t)BrightnessOf(v > D(0x5c9b80) || v != v ? v : 0.0f);
        }
        return;
    }
    if (mode == 2) {
        double k = (double)s + D(0x5c8770);
        for (int32_t j = 0; j < 4; ++j) {
            uint8_t* q = layer(j);
            float v = DarknessOf(*q);
            v = (float)(v * k);
            *q = (uint8_t)BrightnessOf(v < D(0x5c8650) || v != v ? v : 1.0f);
        }
        return;
    }
    if (mode == 0) {
        double k = D(0x5c8770) - s;
        for (int32_t j = 0; j < 4; ++j) {
            uint8_t* q = layer(j);
            float v = DarknessOf(*q);
            *q = (uint8_t)BrightnessOf((float)((double)s * w[j] + v * k));
        }
        return;
    }
    if (mode != 1) return;
    float sum = (float)((double)p[0] + p[1]);
    sum = (float)((double)sum + p[2]);
    int32_t all = sum < D(0x5caa68) || sum != sum ? 0 : 1;
    int32_t range = (x * 0x8d + y * 7) % 1000 + 1000;
    if (Fl_194(0x5c85b4) > sum) sum = 1.0f;
    float rnd = (float)HashRand(y * 0x8dad + x * 0xaf1, 0, range);
    float draw = (float)((double)rnd / (float)range * sum);
    int32_t done = 0;
    for (int32_t j = 0; j < 4; ++j) {
        float cur = DarknessOf(*layer(j));
        float target = 0.0f;
        if (j == 3) {
            if (!(p[3] > D(0x5c85b8)) && p[3] == p[3] && !all) continue;
            double r = (double)HashRand(x * 0x1c13 + y * 0x3b, 0, 0x7d0) / D(0x5caa60);
            if (p[3] > r) target = w[3];
        } else if (!done) {
            draw = (float)((double)draw - p[j]);
            if (!(draw > Fl_194(0x5c8598)) && draw == draw) {
                target = w[j];
                done = 1;
            }
        }
        if (!(target > D(0x5c85b8)) && target == target && !all) continue;
        *layer(j) = (uint8_t)BrightnessOf((float)((D(0x5c8770) - s) * cur + (double)target * s));
    }
}
RT3_REPLACE(0x44a9a0, Terrain_PaintLayers);

// 0x44ad60. Map byte grid (+0x2131, row stride +0x2155) at (x, y), top 5 bits.
RT3_NOINLINE uint32_t __fastcall Map_GridByte(void* m, int, float x, float y) {
    int32_t row = (int32_t)y;
    uint8_t* p = F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * row;
    return p[(int32_t)x] >> 3;
}
RT3_REPLACE(0x44ad60, Map_GridByte);

// 0x44ada0. Region record (0x6cfc9c) of the cell at (x, y).
RT3_NOINLINE void* __fastcall Map_RegionAt(uint8_t* map, int, float x, float y) {
    int32_t yi = (int32_t)y;
    const uint8_t* row = F<uint8_t*>(map, 0x2131) + F<int32_t>(map, 0x2155) * yi;
    int32_t xi = (int32_t)x;
    return Collection_Get(g_territories, 0, row[xi] >> 3);
}
RT3_REPLACE(0x44ada0, Map_RegionAt);

// 0x44ade0. Region type record at (x, y), following its +0x35 alias unless +0x31 is set.
RT3_NOINLINE void* __fastcall Map_RegionRecordAt(void* m, int, float x, float y) {
    int32_t row = (int32_t)y;
    int32_t col = (int32_t)x;
    uint32_t type = (F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * row)[col] >> 3;
    uint8_t* rec = (uint8_t*)Collection_Get(g_territories, 0, type);
    if (F<int32_t>(rec, 0x31) != 0) return rec;
    return Collection_Get(g_territories, 0, F<uint32_t>(rec, 0x35));
}
RT3_REPLACE(0x44ade0, Map_RegionRecordAt);

// 0x44ae40. Terrain height at the cell (x, y) if its kind is 1 or 3..5, else at the
// first such cell found in the 4 directions within r rings; 0 when none.
double __fastcall Map_NearestLandHeight(uint8_t* m, int, int32_t x, int32_t y, int32_t r) {
    if (Terrain_KindIs1Or3to5(m, 0, x, y)) return Terrain_HeightAt(m, 0, CentreWorld(x), CentreWorld(y));
    for (int32_t k = 1; k <= r; ++k)
        for (int j = 0; j < 4; ++j) {
            int32_t nx = kDirX[j * 2] * k + x;
            int32_t ny = kDirY[j * 2] * k + y;
            if (nx < 0 || nx >= F<int32_t>(m, 0x2155) || ny < 0 || ny >= F<int32_t>(m, 0x2159)) continue;
            if (Terrain_KindIs1Or3to5(m, 0, nx, ny))
                return Terrain_HeightAt(m, 0, CentreWorld(nx), CentreWorld(ny));
        }
    return Fl(0x5c8598);
}
RT3_REPLACE(0x44ae40, Map_NearestLandHeight);

// 0x44afa0. Fog end distance at cell (x, y): 0x8c plus a far-camera term (cell
// distance to the view target 50..200 -> 0x5c8880 scale) plus the rise of the
// terrain around the cell, with a zoom term added above 0x5ca190 (+0x20), +50
// with option 0x2bb.
int32_t __stdcall Map_FogEnd(int32_t x, int32_t y) {
    uint8_t* cam = g_62be68;
    float tz = F<float>(cam, 0x24), tx = F<float>(cam, 0x1c);
    int32_t cellZ, cellX;
    __asm {
        fld tz
        fdiv dword ptr ds:[0x5c8700]
        mov eax, 0x5a10d0
        call eax
        mov cellZ, eax
        fld tx
        fdiv dword ptr ds:[0x5c8700]
        mov eax, 0x5a10d0
        call eax
        mov cellX, eax
    }
    int32_t dist = (uint16_t)DistanceInt(x, y, cellX, cellZ);
    int32_t fog = 0;
    if (dist > 0x32) {
        int32_t d = (dist >= 0xc8 ? 0xc8 : dist) - 0x32;
        __asm {
            fild d
            fmul qword ptr ds:[0x5c8880]
            mov eax, 0x5a10d0
            call eax
            mov fog, eax
        }
    }
    int32_t w = F<int32_t>(Map(), 0x15d9) + 1;
    const float* grid = F<const float*>(Map(), 0x1605);
    int32_t base = (int32_t)grid[w * y + x];
    int32_t top = base;
    for (int32_t k = 1; k <= 2; ++k)
        for (int j = 0; j < 4; ++j) {
            int32_t nx = kDirX[j * 2] * k * 7 + x;
            int32_t ny = kDirY[j * 2] * k * 7 + y;
            if (nx < 0 || ny < 0 || nx >= F<int32_t>(Map(), 0x2155) || ny >= F<int32_t>(Map(), 0x2159)) continue;
            int32_t v = (int32_t)grid[w * ny + nx];
            if (!(top > v)) top = v;
        }
    int32_t r = fog + (top - base);
    float zoom = F<float>(g_62be68, 0x20);
    if (zoom > Fl(0x5ca190)) {
        // The zoom term adds to the whole sum: the original keeps r in fog's
        // stack slot (0x44b107) and reloads that slot here.
        __asm {
            fld zoom
            fsub dword ptr ds:[0x5ca190]
            fmul qword ptr ds:[0x5c86b0]
            fiadd r
            mov eax, 0x5a10d0
            call eax
            mov r, eax
        }
    }
    if (Option2bbActive()) r += 0x32;
    return r + 0x8c;
}
RT3_REPLACE(0x44afa0, Map_FogEnd);

// 0x44b160 (map method, the map unused). Bulldozes the circle (x, y, r) for
// company `company` (0 in the editor, 0x6cec74+0x68): pass 0 prices it and
// finds what stops it, pass 1 (unless checkOnly) charges the company (stat
// 0xc) and clears it - land paint (in sandbox / editor, 0x6d07b4 or
// 0x6cec7c+0x82, once 0x62c124 has passed), trees, unowned industries and
// effects in sandbox / editor, buildings and the track inside. Buildings of
// others cost their price (Building_Price, the 0x65 effect multiplier, x2
// and x2 again near others' track, x the type's year factor and x2 more with
// several humans) plus 0x5c85b0 each for own ones; track costs 0x5ca06c a
// piece. Stopped by: no such company (0x1a3), another company's building
// (0x1a5) or track (0x1a6), a train on the track (0x1a7), a station on it
// (0x1a8, unless the station itself is being cleared), lack of cash (0x1a4).
// Returns 1 when it may go ahead; *cost gets the price, msg the reason.
RT3_NOINLINE int32_t __fastcall Map_Bulldoze(uint8_t* map, int, float x, float y, float r, uint32_t company, int32_t checkOnly, float* cost, char* msg) {
    msg[0] = 0;
    *(int32_t*)cost = 0;
    World_RefreshColors();
    if (!Collection_IsValid(g_companies, 0, company) && !F<int32_t>(g_app, 0x68)) {
        strcpy(msg, GetString(0x1a3));
        return 0;
    }
    uint8_t* co = nullptr;
    if (F<int32_t>(g_app, 0x68)) company = 0;
    else co = (uint8_t*)Collection_Get(g_companies, 0, company);
    msg[0] = 0;
    float total = 0.0f;
    int32_t ok = 1;
    float nearOwn = 999.0f, nearOther = 999.0f;
    int32_t step = IsMultiplayer() ? 1 : 0xc;
    Collection* pieces = g_tracks;
    int32_t n = Collection_Count(pieces);
    for (int32_t i = 0; i < n; i += step) {
        uint8_t* p = (uint8_t*)Collection_Get(g_tracks, 0, Collection_IdAt(g_tracks, 0, (uint32_t)i, 0));
        if (!p[0x216]) continue;
        float px, py;
        Obj_CenterCell(p, 0, &px, &py);
        double dx = crt::fabs((double)(x - px));
        float d = (float)(crt::fabs((double)(y - py)) + dx);
        if (!(d < Fl_207(0x5c8d4c))) continue;
        if ((uint32_t)F<uint16_t>(p, 0x22e) == company) {
            if (!(nearOwn < d)) nearOwn = d;
        } else if (!(nearOther < d)) {
            nearOther = d;
        }
    }
    float factor = 1.0f;
    if (nearOther < Fl_207(0x5c873c)) factor = 2.0f;
    if (nearOther < Fl_207(0x5c9ac8) && (double)nearOther + Fl_207(0x5c873c) < nearOwn) factor = factor + factor;
    uint32_t keep1 = 0, keep2 = 0;
    for (int32_t pass = 0; pass < 2; ++pass) {
        if (pass == 1) {
            if (checkOnly || !ok) continue;
            if (ClockMs() >= g_62c124 && (Global6d07b4Set2() || g_newGame[0x82]))
                Terrain_PaintPoints(g_map, 0, x, y, r, 1, 0, 0, 9999.0f, 0);
            if (!g_newGame[0x82] && total > D(0x5c86b8)) {
                if (!co) goto clear;
                double want = total;
                if (Company_Stat(co, 0, 0xd, 0x2329) < want) {
                    char have[0x130], need[0x130];
                    const char* sh = Num_FormatSimple(have, 9, Company_Stat(co, 0, 0xd, 0x2329), 0xb);
                    const char* sn = Num_FormatSimple(need, 9, (double)total, 0xb);
                    FormatText(msg, GetString(0x1a4), sn, sh, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                    ok = 0;
                    continue;
                }
            }
            if (co) Company_PostStat(co, 0, 0xc, -total, 1);
        }
    clear:
        if (Global6d07b4Set2()) {
            static const uint32_t kTables[3] = { 0x6ada80, 0x62bae0, 0x6cea50 };
            for (int tb = 0; tb < 3; ++tb) {
                Collection* t = hook::Global<Collection*>(kTables[tb]);
                int32_t j = Collection_Count(t) - 1;
                if (j < 0) continue;
                float cx = x, cy = y;
                if (tb != 1) {
                    cy = (float)((double)y + D(0x5c8628));
                    cx = (float)((double)x + D(0x5c8628));
                }
                int32_t removed = 0;
                for (; j >= 0; --j) {
                    t = hook::Global<Collection*>(kTables[tb]);
                    uint8_t* o = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)j, 0));
                    float oy = (float)Obj_CellY(o);
                    float ox = (float)Obj_CellX(o);
                    if (!(Distance2D(cx, cy, ox, oy) < r)) continue;
                    if (tb == 1 && F<int32_t>(o, 0x23e)) continue;
                    if (pass == 0) {
                        (*(void(__thiscall**)(uint8_t*, uint32_t, int32_t, int32_t))(F<uint8_t*>(o, 0) + 0x58))(o, 0xff, 0, 0);
                    } else {
                        t = hook::Global<Collection*>(kTables[tb]);
                        CollectionVtbl* vt = t->vtbl;
                        vt->release(hook::Global<Collection*>(kTables[tb]), Collection_IdAt(t, 0, (uint32_t)j, 0));
                        ++removed;
                    }
                }
                if (tb == 1 && removed) Cities_AssignBlocks(g_cities);
            }
        }
        keep1 = 0;
        keep2 = 0;
        for (int32_t k = Collection_Count(g_buildings) - 1; k >= 0; --k) {
            Collection* bs = g_buildings;
            uint8_t* b = (uint8_t*)Collection_Get(bs, 0, Collection_IdAt(bs, 0, (uint32_t)k, 0));
            float by = (float)Obj_CellY(b);
            float bx = (float)Obj_CellX(b);
            if (!(Distance2D(x, y, bx, by) < r)) continue;
            auto vcall = [&](int off) { return (*(int32_t(__thiscall**)(uint8_t*))(F<uint8_t*>(b, 0) + off))(b); };
            auto mark = [&] { (*(void(__thiscall**)(uint8_t*, uint32_t, int32_t, int32_t))(F<uint8_t*>(b, 0) + 0x58))(b, 0xff, 0, 0); };
            if (pass != 0) {
                F<int32_t>(hook::As<uint8_t*>(g_buildingTypes), 0x98) = 0;
                bs = g_buildings;
                CollectionVtbl* vt = bs->vtbl;
                vt->release(g_buildings, Collection_IdAt(bs, 0, (uint32_t)k, 0));
                F<int32_t>(hook::As<uint8_t*>(g_buildingTypes), 0x98) = 1;
                g_62c124 = IsMultiplayer() ? ClockMs() : ClockMs() + 0x320;
                continue;
            }
            if (F<uint32_t>(b, 0x276) == company || Global6d07b4Set2() || g_newGame[0x82]) {
                vcall(0x70);
                total = (float)(total + Fl_207(0x5c85b0));
                mark();
                if (vcall(0x70)) {
                    if (!keep1) keep1 = F<uint32_t>(b, 0x2a8);
                    else keep2 = F<uint32_t>(b, 0x2a8);
                }
                continue;
            }
            if (F<uint32_t>(b, 0x276) && Collection_IsValid(g_companies, 0, F<uint32_t>(b, 0x276))) {
                ok = 0;
                FormatText(msg, GetString(0x1a5), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                continue;
            }
            float v = Building_Price(b, 0, company);
            uint32_t chair = co ? F<uint32_t>(co, 0x3b) : 0;
            v = (float)(Effects_Multiplier(g_world, 0, 0x65, company, chair, Obj_GridByte(b)) * v);
            v = (float)(v * factor);
            if (!((uint8_t*)(uintptr_t)vcall(0x80))[0x32]) {
                float year = (float)F<int32_t>(Obj_Ref3cc(b), 0x141);
                v = (float)(year * v);
                if (factor > D(0x5c85a8) && CountHumanPlayers() > 1) v = v + v;
            }
            total = v + total;
            mark();
        }
        Tracks_ClearHighlights(g_tracks);
        for (int32_t i = Collection_Count(g_tracks) - 1; i >= 0; --i) {
            Collection* ps = g_tracks;
            uint8_t* p = (uint8_t*)Collection_Get(ps, 0, Collection_IdAt(ps, 0, (uint32_t)i, 0));
            float py = Obj_CenterCellY(p);
            float px = Obj_CenterCellX(p);
            if (Distance2D(x, y, px, py) < r)
                Tracks_HighlightRun(g_tracks, 0, Collection_IdAt(g_tracks, 0, (uint32_t)i, 0), 1, 1);
        }
        for (int32_t i = Collection_Count(g_tracks) - 1; i >= 0; --i) {
            Collection* ps = g_tracks;
            uint8_t* p = (uint8_t*)Collection_Get(ps, 0, Collection_IdAt(ps, 0, (uint32_t)i, 0));
            if (!(p[0x201] & 0x20)) continue;
            if (pass != 0) {
                Tracks_Delete(g_tracks, 0, Collection_IdAt(g_tracks, 0, (uint32_t)i, 0), 0);
                continue;
            }
            if ((uint32_t)F<uint16_t>(p, 0x22e) != company && !Global6d07b4Set2() && !g_newGame[0x82]) {
                ok = 0;
                strcpy(msg, GetString(0x1a6));
            }
            float px = Obj_CenterCellX(p);
            float py = Obj_CenterCellY(p);
            for (int32_t c = Collection_Count(g_62ba84) - 1; c >= 0; --c) {
                Collection* cs = g_62ba84;
                uint8_t* car = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)c, 0));
                float cy = (float)Obj_CellY(car);
                float cx = (float)Obj_CellX(car);
                if (Distance2D(px, py, cx, cy) > Fl_207(0x5c8900)) continue;
                if (!Obj_NodeNear356(car, 0, (int32_t)Collection_IdAt(g_tracks, 0, (uint32_t)i, 0))) continue;
                ok = 0;
                strcpy(msg, GetString(0x1a7));
            }
            for (int32_t s = Collection_Count(g_stations) - 1; s >= 0; --s) {
                Collection* ss = g_stations;
                uint8_t* st = (uint8_t*)Collection_Get(ss, 0, Collection_IdAt(ss, 0, (uint32_t)s, 0));
                float sy = (float)Obj_BuildingCellY(st);
                float sx = (float)Obj_BuildingCellX(st);
                if (Distance2D(px, py, sx, sy) > Fl_207(0x5c89a4)) continue;
                if (F<uint32_t>(st, 0) == keep1 || F<uint32_t>(st, 0) == keep2) continue;
                if (!Obj_NodeNear8(st, 0, (int32_t)Collection_IdAt(g_tracks, 0, (uint32_t)i, 0))) continue;
                ok = 0;
                uint8_t* o1 = (uint8_t*)Obj_Building(st);
                uint8_t* o2 = (uint8_t*)Obj_Building(st);
                uint8_t* n1 = (uint8_t*)(uintptr_t)(*(int32_t(__thiscall**)(uint8_t*))(F<uint8_t*>(o1, 0) + 0x80))(o1);
                uint8_t* n2 = (uint8_t*)(uintptr_t)(*(int32_t(__thiscall**)(uint8_t*))(F<uint8_t*>(o2, 0) + 0x80))(o2);
                const char* a1 = LocalizeName((const char*)n2 + 0x4f);
                const char* a2 = LocalizeName((const char*)n1 + 0x4f);
                FormatText(msg, GetString(0x1a8), a1, a2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
            total = (float)(total + Fl_207(0x5ca06c));
            Track_BuildCost(p, 0, company, 0);
        }
    }
    g_world[0x89] = 0;
    *cost = total;
    return ok;
}
RT3_REPLACE(0x44b160, Map_Bulldoze);

// 0x44bd10. Terrain kind (+0x272 of the 0x62bae0 record, 5 if none) of the
// coarse tile (4 cells) at world cell (x, y), clamped to the tile grid.
int32_t __fastcall Map_TerrainKindAt(void* m, int, float x, float y) {
    int32_t cx = (int32_t)((double)x / 4.0f);
    int32_t cy = (int32_t)((double)y / 4.0f);
    int32_t w = F<int32_t>(m, 0x214d), h = F<int32_t>(m, 0x2151);
    cx = cx <= 0 ? 0 : cx;
    if (cx >= w - 1) cx = w - 1;
    cy = cy <= 0 ? 0 : cy;
    if (cy >= h - 1) cy = h - 1;
    uint8_t* t = F<uint8_t*>(m, 0x212d) + (w * cy + cx) * 4;
    uint32_t id = *(uint16_t*)(t + 1) ? *(uint16_t*)(t + 1) : t[0];
    uint8_t* r = (uint8_t*)Collection_Get(g_cities, 0, id);
    return r ? F<int32_t>(r, 0x272) : 5;
}
RT3_REPLACE(0x44bd10, Map_TerrainKindAt);

// 0x44bdb0. Number of the eight neighbours of kind 2 / 4 / 5.
RT3_NOINLINE int32_t __fastcall Map_CountShoreNeighbours(uint8_t* m, int, int32_t x, int32_t y) {
    int32_t n = 0;
    for (int32_t d = 0; d < 8; ++d) {
        int32_t cx = DirX(d) + x, cy = DirY(d) + y;
        if (cx < 0 || cx >= F<int32_t>(m, 0x2155) || cy < 0 || cy >= F<int32_t>(m, 0x2159)) continue;
        if (Terrain_KindIs2Or4Or5(m, 0, cx, cy)) ++n;
    }
    return n;
}
RT3_REPLACE(0x44bdb0, Map_CountShoreNeighbours);

// 0x44be20. Flood-fills a body of water from (x, y), entered from direction `dir`
// (flag: the other flow sense): grows the box +0x21ad..+0x21b9, marks visited cells
// (0x62c128), stores each cell's flow direction (record bits 3-5, by table
// 0x5ee5cc) and spreads to the 8 neighbours of a water kind (2 / 4 / 5) unless a
// shore cell (record bit 0x20) meets a neighbour with 3+ shore neighbours (not when
// +0x21bd). Afterwards marks dead ends (one water neighbour, not on the map edge)
// and shore cells with their bank side (record +1 bits 4 / 8), turning a shore
// cell's direction toward its first diagonal-adjacent pair of orthogonal water
// neighbours.
void __fastcall Water_FloodFill(uint8_t* m, int, int32_t x, int32_t y, int32_t dir, int32_t flag) {
    const int32_t* dx = (const int32_t*)0x624b28;
    const int32_t* dy = (const int32_t*)0x624b48;
    const uint8_t* codes = (const uint8_t*)0x5ee5cc;
    if (F<int32_t>(m, 0x21ad) >= x) F<int32_t>(m, 0x21ad) = x;
    if (F<int32_t>(m, 0x21b1) >= y) F<int32_t>(m, 0x21b1) = y;
    if (F<int32_t>(m, 0x21b5) <= x) F<int32_t>(m, 0x21b5) = x;
    if (F<int32_t>(m, 0x21b9) <= y) F<int32_t>(m, 0x21b9) = y;
    auto visited = [&](int32_t cx, int32_t cy) -> uint8_t& { return g_62c128[F<int32_t>(m, 0x2155) * cy + cx]; };
    visited(x, y) = 1;
    auto setDir = [&](uint32_t idx) {
        uint16_t* r = (uint16_t*)Terrain_Rec(m, 0, x, y);
        *r = (uint16_t)(((codes[idx & 7] & 7) << 3) | (*r & 0xffc7));
    };
    setDir(flag ? (uint32_t)(dir - 4) : (uint32_t)dir);
    int32_t n = 0, nOrth = 0;
    uint8_t present[8] = {}, spread[8] = {};
    for (int32_t i = 0; i < 8; ++i) {
        int32_t nx = dx[i] + x, ny = dy[i] + y;
        if (nx < 0 || nx >= F<int32_t>(m, 0x2155) || ny < 0 || ny >= F<int32_t>(m, 0x2159)) continue;
        if (!Terrain_KindIs2Or4Or5(m, 0, nx, ny)) continue;
        ++n;
        if (!(i & 1)) ++nOrth;
        present[i] = 1;
        if (visited(nx, ny)) continue;
        if (!F<int32_t>(m, 0x21bd)) {
            if ((Terrain_Rec(m, 0, x, y)[2] & 0x20) && Map_CountShoreNeighbours(m, 0, nx, ny) >= 3) continue;
            if (!F<int32_t>(m, 0x21bd) && (Terrain_Rec(m, 0, nx, ny)[2] & 0x20) && Map_CountShoreNeighbours(m, 0, x, y) >= 3) continue;
        }
        visited(nx, ny) = 1;
        spread[i] = 1;
    }
    for (int32_t i = 0; i < 8; ++i)
        if (spread[i]) Water_FloodFill(m, 0, dx[i] + x, dy[i] + y, i, flag);
    Terrain_Rec(m, 0, x, y)[1] &= 0xf7;
    Terrain_Rec(m, 0, x, y)[1] &= 0xfb;
    if (n == 1 && x != 0 && x != F<int32_t>(m, 0x2155) - 1 && y != 0 && y != F<int32_t>(m, 0x2159) - 1)
        Terrain_Rec(m, 0, x, y)[1] |= flag ? 8 : 4;
    if (!(Terrain_Rec(m, 0, x, y)[2] & 0x20)) return;
    int32_t k = (dir + 4) % 8;
    if (Map_CountShoreNeighbours(m, 0, dx[k] + x, dy[k] + y) > 2)
        Terrain_Rec(m, 0, x, y)[1] |= flag ? 4 : 8;
    else
        Terrain_Rec(m, 0, x, y)[1] |= flag ? 8 : 4;
    if (nOrth < 2) return;
    Terrain_Rec(m, 0, x, y);
    for (int32_t e = 0; e < 8; e += 2) {
        if (!present[e]) continue;
        if (!present[(e + 1) % 8] && !present[(e + 7) % 8]) continue;
        int32_t diff = e - dir;
        int32_t a = diff + 8 < 0 ? -(diff + 8) : diff + 8;
        int32_t b = diff < 0 ? -diff : diff;
        int32_t d = b < a ? b : a;
        uint32_t off = d <= 1 ? (flag ? 4u : 0u) : (flag ? 0u : 4u);
        setDir(off + (uint32_t)e);
        return;
    }
}
RT3_REPLACE(0x44be20, Water_FloodFill);

// 0x44c200. Floods the water body at (x, y) (kinds 2 / 4 / 5): sets up the box and
// a visited grid (0x62c128, freed afterwards; shore rule off with option 0xa8c & 4),
// takes the cell's flow direction (table 0x5ee5d4 by record bits 3-5), fills every
// qualifying neighbour in turn (the first one fixing the main direction), then the
// cell itself, entered from opposite the last neighbour.
void __fastcall Water_FloodFrom(uint8_t* m, int, int32_t x, int32_t y) {
    const int32_t* dx = (const int32_t*)0x624b28;
    const int32_t* dy = (const int32_t*)0x624b48;
    if (!Terrain_KindIs2Or4Or5(m, 0, x, y)) return;
    F<int32_t>(m, 0x21bd) = F<int32_t>(g_input, 0xa8c) & 4;
    int32_t cells = F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155);
    F<int32_t>(m, 0x21ad) = x;
    F<int32_t>(m, 0x21b1) = y;
    F<int32_t>(m, 0x21b5) = x;
    F<int32_t>(m, 0x21b9) = y;
    uint8_t* visited = (uint8_t*)crt::malloc(cells);
    g_62c128 = visited;
    memset(visited, 0, cells);
    g_62c128[F<int32_t>(m, 0x2155) * y + x] = 1;
    int32_t dir = ((const uint8_t*)0x5ee5d4)[((Terrain_Rec(m, 0, x, y)[0] >> 3) - 4) & 7];
    int32_t last = dir, flag = 0, first = 1;
    for (int32_t i = 0; i < 8; ++i) {
        int32_t nx = dx[i] + x, ny = dy[i] + y;
        if (nx < 0 || nx >= F<int32_t>(m, 0x2155) || ny < 0 || ny >= F<int32_t>(m, 0x2159)) continue;
        if (!Terrain_KindIs2Or4Or5(m, 0, nx, ny)) continue;
        if (!F<int32_t>(m, 0x21bd)) {
            if ((Terrain_Rec(m, 0, x, y)[2] & 0x20) && Map_CountShoreNeighbours(m, 0, nx, ny) >= 3) continue;
            if (!F<int32_t>(m, 0x21bd) && (Terrain_Rec(m, 0, nx, ny)[2] & 0x20) && Map_CountShoreNeighbours(m, 0, x, y) >= 3) continue;
        }
        int32_t diff = i - dir;
        int32_t a = diff + 8 < 0 ? -(diff + 8) : diff + 8;
        int32_t b = diff < 0 ? -diff : diff;
        flag = (b < a ? b : a) > 2;
        if (first) {
            first = 0;
            dir = flag ? (i - 4) & 7 : i;
        }
        last = i;
        Water_FloodFill(m, 0, nx, ny, i, flag);
    }
    Water_FloodFill(m, 0, x, y, (last + 4) % 8, flag == 0);
    crt::free(g_62c128);
    g_62c128 = nullptr;
}
RT3_REPLACE(0x44c200, Water_FloodFrom);

// 0x44c450. Recomputes the cargo flags of every region cell.
RT3_NOINLINE void __fastcall Map_UpdateCargoFlags(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            City_UpdateCargoFlags(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
}
RT3_REPLACE(0x44c450, Map_UpdateCargoFlags);

// 0x44c4b0. Recomputes flag 0x10 of every map cell: set only on the cells
// of regions without a type (+0x23e).
RT3_NOINLINE void __fastcall Map_MarkUntypedRegions(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            (F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7)[0xe6] &= 0xef;
    Collection* regions = g_cities;
    for (int32_t i = 0; i < Collection_Count(regions); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(regions, 0, Collection_IdAt(regions, 0, i, 0));
        if (F<int32_t>(r, 0x23e)) continue;
        Obj_Region(r)[0xe6] |= 0x10;
    }
}
RT3_REPLACE(0x44c4b0, Map_MarkUntypedRegions);

// 0x44c570. Marks a free land tile (+0x2135 flags) as changed (bits 0xc3 kept,
// value 2) and grows the dirty rectangle +0x21c6..+0x21d2 (count +0x21d6).
RT3_NOINLINE void __fastcall Map_MarkTileDirty(uint8_t* m, int, int32_t x, int32_t y) {
    if (x < 0 || x >= F<int32_t>(m, 0x15d9) - 1) return;
    if (y < 0 || y >= F<int32_t>(m, 0x15d5) - 1) return;
    if (F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x] & 0x3e) return;
    if (Terrain_KindIs1Or3to5(m, 0, x, y)) return;
    int32_t n = F<int32_t>(m, 0x21d6);
    if (!n) {
        F<int32_t>(m, 0x21c6) = x;
        F<int32_t>(m, 0x21ca) = y;
        F<int32_t>(m, 0x21ce) = x;
        F<int32_t>(m, 0x21d2) = y;
    } else {
        if (x < F<int32_t>(m, 0x21c6)) F<int32_t>(m, 0x21c6) = x;
        if (y < F<int32_t>(m, 0x21ca)) F<int32_t>(m, 0x21ca) = y;
        if (x > F<int32_t>(m, 0x21ce)) F<int32_t>(m, 0x21ce) = x;
        if (y > F<int32_t>(m, 0x21d2)) F<int32_t>(m, 0x21d2) = y;
    }
    F<int32_t>(m, 0x21d6) = n + 1;
    uint8_t* cell = F<uint8_t*>(m, 0x2135) + (F<int32_t>(m, 0x2155) + 1) * y + x;
    *cell = (uint8_t)((*cell & 0xc3) | 2);
}
RT3_REPLACE(0x44c570, Map_MarkTileDirty);

// 0x44c670. Ages the corner counters (bits 1..5 of +0x2135, 1..29) in map
// t's dirty rect +0x21c6..+0x21d2 when the game allows it (0x6cec78
// +0x4af3): each step past 5 may sprout scenery (Terrain_AddPoints, kind
// by height), and the corner's cell gets the matching growth level 0..6.
// Then, for the area of corners at 4..13, darkens texture pixels near them
// at random (closer = likelier) and refreshes the textures; every 7th
// region type 3 also redraws two map overlays.
RT3_NOINLINE void __fastcall Terrain_AgeCorners(uint8_t* t) {
    if (!F<int32_t>(g_world, 0x4af3)) return;
    int32_t minX = 9999, minY = 9999, maxX = -1, maxY = -1;
    for (int32_t y = F<int32_t>(t, 0x21ca), seed = y * 0x11; y <= F<int32_t>(t, 0x21d2); ++y, seed += 0x11) {
        for (int32_t x = F<int32_t>(t, 0x21c6); x <= F<int32_t>(t, 0x21ce); ++x) {
            uint8_t* p = F<uint8_t*>(t, 0x2135) + (F<int32_t>(t, 0x2155) + 1) * y + x;
            uint8_t v = (uint8_t)((*p >> 1) & 0x1f);
            if (!v || v >= 0x1e) continue;
            *p ^= (uint8_t)(((*p & 0xfe) + 2) ^ *p) & 0x3e;
            uint8_t lvl = (uint8_t)((F<uint8_t*>(t, 0x2135)[(F<int32_t>(t, 0x2155) + 1) * y + x] >> 1) & 0x1f);
            if (lvl >= 4 && lvl <= 0xd) {
                if (x < minX) minX = x;
                if (y < minY) minY = y;
                if (x > maxX) maxX = x;
                if (y > maxY) maxY = y;
            }
            if (Terrain_RecBit9(t, 0, x, y)) continue;
            if (lvl >= 5 && !HashRand(lvl + seed + x, 0, 3)) {
                float h = F<float*>(t, 0x1605)[(F<int32_t>(t, 0x15d9) + 1) * y + x];
                Terrain_AddPoints(t, 0, x, y, h > Fl_207(0x5c9af4) ? 1 : 4, 1);
            }
            uint8_t stage = lvl <= 2 ? 0 : lvl <= 4 ? 1 : lvl <= 8 ? 2 : lvl <= 0xd ? 3 : lvl <= 0x11 ? 4 : lvl <= 0x16 ? 5 : 6;
            Map_SetCellLow(t, 0, x, y, stage);
        }
    }
    Terrain_RefreshPointGrids(t, 0, F<int32_t>(t, 0x21c6), F<int32_t>(t, 0x21ca), F<int32_t>(t, 0x21ce) + 2, F<int32_t>(t, 0x21d2) + 2);
    if (maxY == -1 || maxY - minY < 4 || maxX - minX < 4) return;
    uint8_t* map = g_map;
    int32_t x0 = (minX - 4 < 0 ? 0 : minX - 4) + 4;
    if (F<int32_t>(map, 0x2155) - 1 < x0) x0 = F<int32_t>(map, 0x2155) - 1;
    int32_t y0 = (minY - 4 < 0 ? 0 : minY - 4) + 4;
    if (F<int32_t>(map, 0x2159) - 1 < y0) y0 = F<int32_t>(map, 0x2159) - 1;
    float tw = (float)F<int32_t>(t, 0x2155), th = (float)F<int32_t>(t, 0x2159);
    float fx0 = (float)x0;
    float u0 = fx0 / tw;
    float fy0 = (float)y0;
    float v0 = fy0 / th;
    float fx1 = (float)(maxX + 1);
    float u1 = fx1 / tw;
    double u1d = u1 > 1.0 ? 1.0 : (double)u1;
    u1 = (float)u1d;
    float fy1 = (float)(maxY + 1);
    float v1 = fy1 / th;
    double v1d = v1 > D(0x5c8770) ? 1.0 : (double)v1;
    v1 = (float)v1d;
    int32_t outW, outH;
    uint32_t* px = Terrain_ExtractTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, u0, v0, u1, v1, &outW, &outH);
    float fw = (float)outW;
    float sx = (float)(((double)u1 - u0) * tw / fw);
    float fh = (float)outH;
    float sy = (float)(((double)v1 - v0) * th / fh);
    int32_t sizeW, sizeH;
    Render_GetSize4((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, &sizeW, &sizeH);
    for (int32_t j = 0; j < outH; ++j) {
        if (outW <= 0) continue;
        float fj = (float)j;
        float rowY = (float)((double)fj * sy + fy0);
        for (int32_t i = 0; i < outW; ++i, ++px) {
            float best = 0;
            float fi = (float)i;
            float colX = (float)((double)fi * sx + fx0);
            bool hit = false;
            for (int32_t pass = 0; pass <= 3 && !hit; ++pass) {
                for (uint32_t k = 0; k < 0x20; k += 4) {
                    if (pass == 0 && k != 0) continue;
                    float fdx = (float)hook::Global<int32_t>(0x624b28 + k);
                    int32_t cx = Ftol((double)fdx + colX);
                    float fdy = (float)hook::Global<int32_t>(0x624b48 + k);
                    int32_t cy = Ftol((double)fdy + rowY);
                    if (cx < 0 || cx >= F<int32_t>(t, 0x15d9) - 1 || cy < 0 || cy >= F<int32_t>(t, 0x15d5) - 1) continue;
                    uint8_t v = (uint8_t)((F<uint8_t*>(t, 0x2135)[(F<int32_t>(t, 0x2155) + 1) * cy + cx] >> 1) & 0x1f);
                    if (v < 4 || v > 0xd) continue;
                    if (pass == 0) {
                        best = 1.0f;
                        hit = true;
                        break;
                    }
                    float fcx = (float)cx, fcy = (float)cy, fcx1 = (float)(cx + 1), fcy1 = (float)(cy + 1);
                    float a = (float)Distance2D(colX, rowY, fcx, fcy);
                    float b = (float)Distance2D(colX, rowY, fcx1, fcy);
                    float c = (float)Distance2D(colX, rowY, fcx, fcy1);
                    float d = (float)Distance2D(colX, rowY, fcx1, fcy1);
                    float m = a < b ? a : b;
                    if (!(m < c)) m = c;
                    if (!(m < d)) m = d;
                    double tt = (double)m - D(0x5c8628);
                    if (!(D(0x5c86b8) <= tt)) tt = 0.1;
                    float tf = (float)tt;
                    float score = (float)(D(0x5c8770) - (double)tf / D(0x5ca140));
                    if (score > best) best = score;
                }
            }
            if (!hit && !(best > Fl_207(0x5c8598))) continue;
            float r = (float)Rand(0, 100);
            if (!((double)best * Fl_207(0x5c8c28) > r)) continue;
            uint32_t c = *px;
            uint32_t o = (uint32_t)Ftol((double)(int32_t)((c >> 16) & 0xff) * D(0x5caa88) + D(0x5caa80)) << 8;
            o = (o | (uint32_t)Ftol((double)(int32_t)((c >> 8) & 0xff) * D(0x5caa88) + D(0x5caa78))) << 8;
            o |= (uint32_t)Ftol((double)(int32_t)(c & 0xff) * D(0x5caa88) + D(0x5caa70));
            *px = o | (c & 0xff000000);
        }
    }
    Terrain_CommitTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, 0);
    if (g_world[0x11] % 7 == 3) {
        Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x808, 0, 0, 0);
        Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x659d, 0, 0, 0);
    }
}
RT3_REPLACE(0x44c670, Terrain_AgeCorners);

// 0x44ce60. Bounding box (+0x21c6..+0x21d2) and count (+0x21d6) of the
// cells with feature bits 0x3e in the byte grid +0x2135 (if world +0x4af3).
RT3_NOINLINE void __fastcall Map_FeatureBounds(uint8_t* m) {
    for (int k = 0; k < 5; ++k) F<int32_t>(m, 0x21c6 + k * 4) = 0;
    if (!F<int32_t>(g_world, 0x4af3)) return;
    int32_t w = F<int32_t>(m, 0x2155);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2159); ++y) {
        for (int32_t x = 0; x < w && x < F<int32_t>(m, 0x2155); ++x) {
            if (!(F<uint8_t*>(m, 0x2135)[(w + 1) * y + x] & 0x3e)) continue;
            if (F<int32_t>(m, 0x21d6) == 0) {
                F<int32_t>(m, 0x21c6) = x;
                F<int32_t>(m, 0x21ca) = y;
                F<int32_t>(m, 0x21ce) = x;
                F<int32_t>(m, 0x21d2) = y;
            } else {
                if (x < F<int32_t>(m, 0x21c6)) F<int32_t>(m, 0x21c6) = x;
                if (y < F<int32_t>(m, 0x21ca)) F<int32_t>(m, 0x21ca) = y;
                if (x > F<int32_t>(m, 0x21ce)) F<int32_t>(m, 0x21ce) = x;
                if (y > F<int32_t>(m, 0x21d2)) F<int32_t>(m, 0x21d2) = y;
            }
            F<int32_t>(m, 0x21d6)++;
        }
    }
}
RT3_REPLACE(0x44ce60, Map_FeatureBounds);

// 0x44cf70. Constructs map m (vtable 0x5ca9fc, the global map 0x62c120) as a
// new w x h terrain. Returns m.
RT3_NOINLINE uint8_t* __fastcall Map_Construct(uint8_t* m, int, int32_t w, int32_t h, const char* tex) {
    Terrain_Construct(m);
    F<uint32_t>(m, 0) = 0x5ca9fc;
    F<int32_t>(m, 0x2125) = 0;
    g_map = m;
    Map_Create(m, 0, w, h, tex);
    return m;
}
RT3_REPLACE(0x44cf70, Map_Construct);

// 0x44cfb0. Map m loads from stream s (texture set tex): unloads the old
// world, reads the format version, the terrain, creates the world tables,
// starts the ambience, then the chunks 0x2ee2 (grid sizes +0x2145.. and the
// track table 0x6cfca8), 0x2ee3 / 0x2ef4..0x2ef6 (the bit planes +0x2139
// cleared, +0x213d read, +0x2141 cleared), the cell bytes +0x2131 and the
// corner bytes +0x2135 (bit 0 cleared; zero before version 0x2ee2), 0x2ee4
// (+0x2141 read), 0x2ee5 (+0x212d), 0x2f43 (terrain layers), 0x2f44 (the
// 0x1d7-byte sites +0x2129 per cell), and finishes the route costs, the land
// count and the graphics options.
RT3_NOINLINE void __fastcall Map_Load(uint8_t* m, int, int32_t s, const char* tex) {
    Map_UnloadWorld(m);
    Map_InitRuntimeState(m);
    int32_t version;
    F<int32_t>(m, 0x2125) = 1;
    StreamRead(s, &version, 4, 0, 1);
    Loading_Pump(g_app, 0, 0, 0);
    Terrain_Load(m, 0, s, tex);
    Loading_Pump(g_app, 0, 0, 0);
    World_CreateTables(m, 0, F<int32_t>(m, 0x15d9) - 1, F<int32_t>(m, 0x15d5) - 1);
    Ambient_Start(hook::As<void*>(g_world));
    m[0x21c5] = 0;
    StreamSkip4(s, 0x2ee2, 0, 0);
    static const uint16_t kFields[] = { 0x2145, 0x2149, 0x214d, 0x2151, 0x2155, 0x2159, 0x215d, 0x2161, 0x2201 };
    for (uint16_t off : kFields) StreamRead(s, m + off, 4, 0, 1);
    Collection* tracks = (Collection*)crt::operator_new(0x20a);
    g_tracks = tracks ? Tracks_Construct(tracks, 0, F<int32_t>(m, 0x2155), F<int32_t>(m, 0x2159)) : nullptr;
    StreamSkip4(s, 0x2ee3, 0, 0);
    int32_t bits = (F<int32_t>(m, 0x2161) * F<int32_t>(m, 0x215d) + 7) / 8;
    int32_t cells = F<int32_t>(m, 0x2155) * F<int32_t>(m, 0x2159);
    int32_t corners = (F<int32_t>(m, 0x2155) + 1) * (F<int32_t>(m, 0x2159) + 1);
    int32_t layers = F<int32_t>(m, 0x2151) * F<int32_t>(m, 0x214d) * 4;
    int32_t sites = F<int32_t>(m, 0x2149) * F<int32_t>(m, 0x2145) * 0x1d7;
    F<int32_t>(m, 0x2165) = 1;
    Loading_Pump(g_app, 0, 0, 0);
    F<void*>(m, 0x2139) = crt::malloc(bits);
    memset(F<void*>(m, 0x2139), 0, (uint32_t)bits);
    StreamSkip4(s, 0x2ef4, 0, 0);
    F<void*>(m, 0x213d) = crt::malloc(bits);
    StreamRead(s, F<void*>(m, 0x213d), (uint32_t)bits, 0, 1);
    StreamSkip4(s, 0x2ef5, 0, 0);
    F<void*>(m, 0x2141) = crt::malloc(bits);
    memset(F<void*>(m, 0x2141), 0, (uint32_t)bits);
    StreamSkip4(s, 0x2ef6, 0, 0);
    Loading_Pump(g_app, 0, 0, 0);
    F<void*>(m, 0x2131) = crt::malloc(cells);
    StreamRead(s, F<void*>(m, 0x2131), (uint32_t)cells, 0, 1);
    F<void*>(m, 0x2135) = crt::malloc(corners);
    if (version >= 0x2ee2) {
        StreamRead(s, F<void*>(m, 0x2135), (uint32_t)corners, 0, 1);
        for (int32_t i = 0; i < corners; ++i) F<uint8_t*>(m, 0x2135)[i] &= 0xfe;
    } else {
        memset(F<void*>(m, 0x2135), 0, (uint32_t)corners);
    }
    StreamSkip4(s, 0x2ee4, 0, 0);
    Loading_Pump(g_app, 0, 0, 0);
    StreamRead(s, F<void*>(m, 0x2141), (uint32_t)bits, 0, 1);
    StreamSkip4(s, 0x2ee5, 0, 0);
    F<void*>(m, 0x212d) = crt::malloc(layers);
    StreamRead(s, F<void*>(m, 0x212d), (uint32_t)layers, 0, 1);
    StreamSkip4(s, 0x2f43, 0, 0);
    Terrain_LoadLayers(m, 0, s);
    StreamSkip4(s, 0x2f44, 0, 0);
    F<void*>(m, 0x2129) = crt::malloc(sites);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Site_Load(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7, 0, s, (int16_t)x, (int16_t)y);
    Routes_BuildCosts(m);
    Loading_Pump(g_app, 0, 0, 0);
    Map_CountLand(m);
    Options_ApplyGraphics(g_app, 0, -2, 1);
    StreamSkip4(s, 0x2ee5, 0, 0);
}
RT3_REPLACE(0x44cfb0, Map_Load);

// 0x44d410 body. Map m: after a change to the cell rectangle (x0, y0) -
// (x1, y1) (clamped; nothing while +0x2205 or when empty) refreshes the
// terrain, its point grids and quads, the view (0x62be68), drops the objects
// in it grown by one cell (0x62bae0 / 0x6ada80) and re-lays the track.
RT3_NOINLINE void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    if (F<int32_t>(m, 0x2205)) return;
    if (!((int32_t(__fastcall*)(uint8_t*, int, int32_t*, int32_t*, int32_t*, int32_t*))0x449e90)(m, 0, &x0, &y0, &x1, &y1)) return;
    Terrain_RefreshRect(m, 0, x0, y0, x1, y1);
    Terrain_RefreshPointGrids(m, 0, x0, y0, x1, y1);
    Terrain_RefreshQuads(m, 0, x0, y0, x1, y1);
    uint8_t* view = g_62be68;
    ((void(__fastcall*)(uint8_t*))(*(void***)view)[0])(view);
    if (g_cities) Objs_DropInRect(g_cities, 0, x0 - 1, y0 - 1, x1 + 1, y1 + 1);
    if (g_6ada80) Objs_DropAllInRect(g_6ada80, 0, x0 - 1, y0 - 1, x1 + 1, y1 + 1);
    Tracks_RefreshArea(x0 - 1, y0 - 1, x1, y1);
}

// 0x44d410. Returns with ECX still the map (POP ECX).
__declspec(naked) void __stdcall Map_CellsChanged(int32_t, int32_t, int32_t, int32_t) { RT3_KEEP_ECX_EDX(Map_CellsChangedImpl, 4) }
RT3_REPLACE(0x44d410, Map_CellsChanged);

// 0x44d4e0. Map m: smooths the heights within r cells of (cx, cy) (clamped
// to the inner map): five passes moving each point (in place) towards the
// average of its four neighbours by half the falloff (1 - distance / r)
// times `strength`, except fixed cells (flag bit 0). Announces the
// unclamped rectangle.
RT3_NOINLINE void __fastcall Map_SmoothAround(uint8_t* m, int, int32_t cx, int32_t cy, int32_t r, float strength) {
    int32_t ux0 = cx - r, uy0 = cy - r, ux1 = cx + r, uy1 = r + cy;
    int32_t x0 = ux0 < 1 ? 1 : ux0, y0 = uy0 < 1 ? 1 : uy0;
    int32_t x1 = F<int32_t>(m, 0x15d9) - 2 < ux1 ? F<int32_t>(m, 0x15d9) - 2 : ux1;
    int32_t y1 = F<int32_t>(m, 0x15d5) - 2 < uy1 ? F<int32_t>(m, 0x15d5) - 2 : uy1;
    for (int32_t pass = 5; pass; --pass)
        for (int32_t y = y0; y <= y1; ++y)
            for (int32_t x = x0; x <= x1; ++x) {
                float w;
                if (!r) w = 1.0f;
                else {
                    float rf = (float)r;
                    float d = (float)(int32_t)(uint16_t)DistanceInt(x, y, cx, cy);
                    w = (float)(((double)rf - d) / rf);
                    if (w <= 0.0f) continue;
                }
                float* hs = F<float*>(m, 0x1605);
                int32_t row = F<int32_t>(m, 0x15d9) + 1;
                float* c = hs + row * y + x;
                double avg = (((double)hs[row * (y - 1) + x] + hs[row * (y + 1) + x]) + c[-1]) + c[1];
                float d = (float)((double)*c - avg / Fl_207(0x5c8568));
                d = -d;
                d = (float)((double)d * w);
                d = (float)((double)d * D(0x5c8628));
                d = (float)((double)d * strength);
                uint8_t flags = F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x];
                if (!(flags & 1)) *c = (float)((double)d + *c);
            }
    Map_CellsChangedImpl(m, 0, ux0, uy0, ux1, uy1);
}
RT3_REPLACE(0x44d4e0, Map_SmoothAround);

// 0x44d6e0. Map m: smooths the heights around (cx, cy) within r cells
// (clamped to the inner map): five passes moving each point (in place)
// 0.65 of the way to the average of its four neighbours, except fixed cells
// (flag bit 0, +0x2135) and cells near a feature. With `refresh` announces
// the unclamped rectangle.
RT3_NOINLINE void __fastcall Map_SmoothHeights(uint8_t* m, int, int32_t cx, int32_t cy, int32_t r, int32_t refresh) {
    int32_t ux0 = cx - r, uy0 = cy - r, ux1 = cx + r, uy1 = r + cy;
    int32_t x0 = ux0 < 1 ? 1 : ux0, y0 = uy0 < 1 ? 1 : uy0;
    int32_t x1 = F<int32_t>(m, 0x15d9) - 2 >= ux1 ? ux1 : F<int32_t>(m, 0x15d9) - 2;
    int32_t y1 = F<int32_t>(m, 0x15d5) - 2 < uy1 ? F<int32_t>(m, 0x15d5) - 2 : uy1;
    for (int32_t pass = 5; pass; --pass)
        for (int32_t y = y0; y <= y1; ++y)
            for (int32_t x = x0; x <= x1; ++x) {
                float* hs = F<float*>(m, 0x1605);
                int32_t row = F<int32_t>(m, 0x15d9) + 1;
                float* c = hs + row * y + x;
                double avg = (((double)hs[row * (y - 1) + x] + hs[row * (y + 1) + x]) + c[-1]) + c[1];
                float d = (float)((double)*c - avg / Fl_207(0x5c8568));
                d = -d;
                d = (float)((double)d * D(0x5c8da8));
                uint8_t flags = F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x];
                if (!(flags & 1) && !Map_NearFeature(m, 0, x, y)) *c = (float)((double)d + *c);
            }
    if (refresh) Map_CellsChangedImpl(m, 0, ux0, uy0, ux1, uy1);
}
RT3_REPLACE(0x44d6e0, Map_SmoothHeights);

// 0x44d880. Map m: levels the heights within r cells of (x, y) towards a
// base (the centre's height, or 0 when `useCentre` is 0.0): each point moves
// by half the difference times the falloff (1 - distance / r) and
// `strength`. Announces the unclamped rectangle.
RT3_NOINLINE void __fastcall Map_LevelAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float useCentre, float strength) {
    if (x < 0 || x >= Terrain_Rows(m)) return;
    if (y < 0 || y >= Terrain_Cols(m)) return;
    float base = useCentre == 0.0f ? 0.0f : F<float*>(m, 0x1605)[(F<int32_t>(m, 0x15d9) + 1) * y + x];
    int32_t ux0 = x - r, uy0 = y - r, ux1 = x + r, uy1 = y + r;
    int32_t x0 = ux0 < 0 ? 0 : ux0, y0 = uy0 < 0 ? 0 : uy0;
    int32_t x1 = F<int32_t>(m, 0x15d9) - 1 < ux1 ? F<int32_t>(m, 0x15d9) - 1 : ux1;
    int32_t y1 = F<int32_t>(m, 0x15d5) - 1 < uy1 ? F<int32_t>(m, 0x15d5) - 1 : uy1;
    for (int32_t cy = y0; cy <= y1; ++cy)
        for (int32_t cx = x0; cx <= x1; ++cx) {
            float w;
            if (!r) w = 1.0f;
            else {
                float rf = (float)r;
                float d = (float)(int32_t)(uint16_t)DistanceInt(cx, cy, x, y);
                w = (float)(((double)rf - d) / rf);
                if (w <= 0.0f) continue;
            }
            float* h = F<float*>(m, 0x1605) + (F<int32_t>(m, 0x15d9) + 1) * cy + cx;
            float delta = (float)((double)*h - base);
            delta = -delta;
            delta = (float)((double)delta * w);
            delta = (float)((double)delta * D(0x5c8628));
            delta = (float)((double)delta * strength);
            *h = (float)((double)delta + *h);
        }
    Map_CellsChangedImpl(m, 0, ux0, uy0, ux1, uy1);
}
RT3_REPLACE(0x44d880, Map_LevelAround);

// 0x44da70. Map m: raises (or with a negative amount lowers) the heights
// within r cells of (x, y) by amount times the falloff (1 - distance / r),
// rounded down to whole units. With `keepFloor`, when the centre is water
// or a lake (kind 3 / 5) a point is not lowered below that cell's whole
// height. Announces the unclamped rectangle.
// QUIRK: the floor test reads the centre's kind but each point's own height.
RT3_NOINLINE void __fastcall Map_RaiseAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float amount, uint8_t keepFloor) {
    int32_t ux0 = x - r, uy0 = y - r, ux1 = x + r, uy1 = y + r;
    int32_t x0 = ux0 < 0 ? 0 : ux0, y0 = uy0 < 0 ? 0 : uy0;
    int32_t x1 = F<int32_t>(m, 0x15d9) - 1 < ux1 ? F<int32_t>(m, 0x15d9) - 1 : ux1;
    int32_t y1 = F<int32_t>(m, 0x15d5) - 1 < uy1 ? F<int32_t>(m, 0x15d5) - 1 : uy1;
    for (int32_t cy = y0; cy <= y1; ++cy)
        for (int32_t cx = x0; cx <= x1; ++cx) {
            float w;
            if (!r) w = 1.0f;
            else {
                float rf = (float)r;
                float d = (float)(int32_t)(uint16_t)DistanceInt(cx, cy, x, y);
                w = (float)(((double)rf - d) / rf);
                if (w <= 0.0f) continue;
            }
            int32_t row = F<int32_t>(m, 0x15d9) + 1;
            int32_t v = Ftol((double)w * amount + F<float*>(m, 0x1605)[row * cy + cx]);
            if (keepFloor) {
                int32_t floor = 0;
                if (x >= 0 && x < F<int32_t>(m, 0x15d9) - 1 && y >= 0 && y < F<int32_t>(m, 0x15d5) - 1 && Terrain_KindIs3Or5(m, 0, x, y))
                    floor = Ftol((double)F<float*>(m, 0x1605)[(F<int32_t>(m, 0x15d9) + 1) * cy + cx]);
                if (v < floor) {
                    float* h = F<float*>(m, 0x1605) + (F<int32_t>(m, 0x15d9) + 1) * cy + cx;
                    v = *h < (float)floor ? Ftol((double)*h) : floor;
                }
            }
            F<float*>(m, 0x1605)[(F<int32_t>(m, 0x15d9) + 1) * cy + cx] = (float)v;
        }
    Map_CellsChangedImpl(m, 0, ux0, uy0, ux1, uy1);
}
RT3_REPLACE(0x44da70, Map_RaiseAround);

// 0x44dca0. Map m: sets cell (x, y)'s height entry (+0x1605, rows of +0x15d9
// + 1) to v; with `refresh` announces the change (Map_CellsChanged).
RT3_NOINLINE void __fastcall Map_SetCellHeight(uint8_t* m, int, int32_t x, int32_t y, uint32_t v, int32_t refresh) {
    F<uint32_t*>(m, 0x1605)[(F<int32_t>(m, 0x15d9) + 1) * y + x] = v;
    if (refresh) Map_CellsChangedImpl(m, 0, x, y, x, y);
}
RT3_REPLACE(0x44dca0, Map_SetCellHeight);

// 0x44dcf0. Sets bit 9 of each cell word (+0x165d, 3 bytes, stride +0x15dd) in the
// clamped rect when it or one of its 8 neighbours is of kind 2/4/5 (neighbours
// off the inner map are skipped near the edge).
RT3_NOINLINE void __fastcall Map_MarkNearKind245(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    Map_ClampRect(m, 0, &x0, &y0, &x1, &y1);
    int32_t edge = 1;
    if (x0 > 2 && y0 > 2 && x1 < F<int32_t>(m, 0x2155) - 3 && y1 < F<int32_t>(m, 0x2159) - 3) edge = 0;
    for (int32_t x = x0; x <= x1; ++x)
        for (int32_t y = y0; y <= y1; ++y) {
            int32_t v = Terrain_KindIs2Or4Or5(m, 0, x, y);
            for (int j = 0; j < 8; ++j) {
                int32_t nx = kDirX[j] + x, ny = kDirY[j] + y;
                if (edge && !(nx > 0 && ny > 0 && nx < F<int32_t>(m, 0x2155) - 1 && ny < F<int32_t>(m, 0x2159) - 1))
                    continue;
                if (Terrain_KindIs2Or4Or5(m, 0, nx, ny)) v = 1;
            }
            uint16_t* cell = (uint16_t*)(F<uint8_t*>(m, 0x165d) + (F<int32_t>(m, 0x15dd) * y + x) * 3);
            *cell ^= (uint16_t)(((uint32_t)v << 9 ^ *cell) & 0x200);
        }
}
RT3_REPLACE(0x44dcf0, Map_MarkNearKind245);

// 0x44de30. Floods cell (x, y) unless already water: kind 1 -> 4, 3 -> 5, else 2;
// flags both water grids, updates the counts and camera, shades it, re-marks shores.
RT3_NOINLINE void __fastcall Map_FloodCell(uint8_t* m, int, int32_t x, int32_t y, int32_t) {
    if (Terrain_KindIs2Or4Or5(m, 0, x, y)) return;
    uint16_t* w = CellWord(m, x, y);
    uint32_t v = *w;
    uint32_t k = v & 7;
    if (k == 1) v = (v & 0xfffc) | 4;
    else if (k == 3) v = (v & 0xfffd) | 5;
    else v = (v & 0xfffa) | 2;
    *w = (uint16_t)v;
    CellByte(m, 0x1661, x, y) = 1;
    CellByte(m, 0x1665, x, y) = 1;
    Terrain_DecCellCount(m, 0, x, y, 6);
    uint8_t* cam = g_62be68;
    ((void(__thiscall*)(void*))(*(void***)cam)[0])(cam);
    ShadeByte(m, x, y) = 0xc4;
    Map_MarkNearKind245(m, 0, x - 1, y - 1, x + 1, y + 1);
}
RT3_REPLACE(0x44de30, Map_FloodCell);

// 0x44df10. Drains water cell (x, y): kind 4 -> 1, 5 -> 3, else 0; clears the
// shade and water grids and re-marks shores.
RT3_NOINLINE void __fastcall Map_DrainCell(uint8_t* m, int, int32_t x, int32_t y) {
    if (!Terrain_KindIs2Or4Or5(m, 0, x, y)) return;
    ShadeByte(m, x, y) = 0;
    CellByte(m, 0x1661, x, y) = 0;
    CellByte(m, 0x1665, x, y) = 0;
    uint16_t* w = CellWord(m, x, y);
    uint32_t v = *w;
    uint32_t k = v & 7;
    if (k == 4) v = (v & 0xfff9) | 1;
    else if (k == 5) v = (v & 0xfffb) | 3;
    else v &= 0xfff8;
    *w = (uint16_t)v;
    Map_MarkNearKind245(m, 0, x - 1, y - 1, x + 1, y + 1);
}
RT3_REPLACE(0x44df10, Map_DrainCell);

// 0x44dfe0. Ground-texture brush at (cx, cy) of radius r (min 0.1): each vertex
// within it is painted (Terrain_PaintLayers, also the far map edge) by the part of
// the brush falloff (or full with `hard`) that exceeds the stroke's coverage so far
// in `mask` (byte per vertex, 255 = done), which is raised to it.
RT3_NOINLINE void __fastcall Terrain_PaintBrush(uint8_t* t, int, float cx, float cy, float r, float strength, int32_t hard,
                                   int32_t mode, const float* w, const float* p, uint8_t* mask) {
    strength = (float)crt::fabs(strength);
    int32_t x0 = Ftol((double)cx - r - Fl_195(0x5c85b4));
    int32_t x1 = Ftol((double)cx + r + Fl_195(0x5c85b4));
    int32_t y0 = Ftol((double)cy - r - Fl_195(0x5c85b4));
    int32_t y1 = Ftol((double)cy + r + Fl_195(0x5c85b4));
    if (x0 < 0) x0 = 0;
    if (F<int32_t>(t, 0x2155) < x1) x1 = F<int32_t>(t, 0x2155);
    if (y0 < 0) y0 = 0;
    if (F<int32_t>(t, 0x2159) < y1) y1 = F<int32_t>(t, 0x2159);
    r = (float)(r < D(0x5c86b8) ? 0.1 : (double)r);
    for (int32_t y = y0; y < y1; ++y) {
        for (int32_t x = x0; x < x1; ++x) {
            int32_t i = F<int32_t>(t, 0x2155) * y + x;
            if (mask[i] >= 0xff) continue;
            float d = (float)Distance2D((float)x, (float)y, cx, cy);
            if (!(d < r) && d == d) continue;
            float k = (float)(hard ? 1.0 : D(0x5c8770) - (double)d / r);
            float cur = (float)((double)mask[i] / D(0x5ca110));
            if (!(k > cur)) continue;
            float diff = (float)((double)k - cur);
            mask[i] = (uint8_t)Ftol((double)k * Fl_195(0x5ca0ec));
            float s = (float)((double)diff * strength);
            Terrain_PaintLayers(t, 0, x, y, s, mode, w, p);
            if (x == F<int32_t>(t, 0x2155) - 1) Terrain_PaintLayers(t, 0, x + 1, y, s, mode, w, p);
            if (y == F<int32_t>(t, 0x2159) - 1) Terrain_PaintLayers(t, 0, x, y + 1, s, mode, w, p);
        }
    }
}
RT3_REPLACE(0x44dfe0, Terrain_PaintBrush);

// 0x44e270. Region type at (x, y): the type record's +0x35 unless it has +0x31, else the raw type.
RT3_NOINLINE uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y) {
    int32_t row = (int32_t)y;
    int32_t col = (int32_t)x;
    uint32_t type = (F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * row)[col] >> 3;
    uint8_t* rec = (uint8_t*)Collection_Get(g_territories, 0, type);
    if (F<int32_t>(rec, 0x31) == 0) return F<uint32_t>(rec, 0x35);
    return (F<uint8_t*>(m, 0x2131) + F<int32_t>(m, 0x2155) * row)[col] >> 3;
}
RT3_REPLACE(0x44e270, Map_RegionTypeAt);

// 0x44e2e0. Map m: rescales all heights. Mode 0 multiplies by 1 + scale;
// modes 1 / 2 by 1 + 0.85 * scale times the height's ratio to the mean
// (clamped to 0..2; mode 2 uses 2 - ratio), at least 0.35; other modes
// leave them. Heights from 4000 up become 4000. Announces the whole map.
RT3_NOINLINE void __fastcall Map_RescaleHeights(uint8_t* m, int, float scale, int32_t mode) {
    float top = 0.0f, sum = 0.0f;
    int32_t w = F<int32_t>(m, 0x15d9);
    for (int32_t y = 0; y < F<int32_t>(m, 0x15d5); ++y)
        for (int32_t x = 0; x < w; ++x) {
            float v = F<float*>(m, 0x1605)[(w + 1) * y + x];
            sum = (float)((double)v + sum);
            if (v > top) top = v;
        }
    float n = (float)(F<int32_t>(m, 0x15d9) * F<int32_t>(m, 0x15d5));
    float mean = (float)((double)sum / n);
    if (Fl_207(0x5c85b4) > mean) mean = 1.0f;
    float f = (float)((double)scale + D(0x5c8770));
    for (int32_t y = 0; y < F<int32_t>(m, 0x15d5); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x15d9); ++x) {
            float* p = F<float*>(m, 0x1605) + (F<int32_t>(m, 0x15d9) + 1) * y + x;
            float v = *p;
            if (mode == 0) v = (float)((double)f * v);
            else if (mode > 0 && mode <= 2) {
                f = (float)((double)v / mean);
                if ((double)f > D(0x5c85d0)) f = 2.0f;
                if ((double)f < D(0x5c8608)) f = 0.0f;
                double k = mode == 1 ? (double)f : D(0x5c85d0) - f;
                f = (float)(k * scale * D(0x5c85a0) + D(0x5c8770));
                if ((double)f < D(0x5c8618)) f = (float)D(0x5c8618);
                v = (float)((double)f * v);
            }
            *p = v < Fl_207(0x5ca1c8) ? v : 4000.0f;
        }
    Map_CellsChangedImpl(m, 0, 0, 0, 9999, 9999);
}
RT3_REPLACE(0x44e2e0, Map_RescaleHeights);

// 0x48f2b0. Initialises track piece t: id, owner group +0x240, corners given in
// cells (x, z pairs, times 92), style (+0x218, also the mesh style), owner company
// (+0x22e); all links cleared, built on the engine's terrain, its map cell marked
// (0x80) unless the table is loading (+0xed), dropped onto the terrain; becoming a
// built piece (kind 2) counts it in the table and clears the trains' route caches;
// then kind +0x216 is set and the piece recoloured. NOT INSTALLED: the original
// passes uninitialized corner heights to Track_Setup, whose mesh fields
// (+0x12f..+0x1e3) keep values derived from them after the drop, so its result
// depends on stack residue. The exe's code stays; this decode uses zero heights
// and is verified outside those fields.
void __fastcall Track_Init(uint8_t* t, int, int32_t id, int32_t group, float x0, float z0, float x1, float z1, float x2, float z2,
                           float x3, float z3, int32_t kind, int32_t owner, int32_t style) {
    float v[4][3] = {};
    const float k = Fl_205(0x5c8700);
    const float xs[4] = { x0, x1, x2, x3 }, zs[4] = { z0, z1, z2, z3 };
    for (int i = 0; i < 4; ++i) {
        v[i][0] = (float)((double)xs[i] * k);
        v[i][2] = (float)((double)zs[i] * k);
    }
    F<int32_t>(t, 0x202) = id;
    t[0x218] = (uint8_t)style;
    const uint32_t cleared[9] = { 0x206, 0x20a, 0x20e, 0x212, 0x21a, 0x21e, 0x238, 0x23c, 0x222 };
    for (uint32_t off : cleared) F<int32_t>(t, off) = -1;
    t[0x219] = 0xff;
    t[0x251] = 0xff;
    F<int32_t>(t, 0x253) = -1;
    F<int32_t>(t, 0x240) = group;
    F<int32_t>(t, 0x226) = 0;
    F<int32_t>(t, 0x22a) = 0;
    F<uint16_t>(t, 0x22e) = (uint16_t)owner;
    F<int32_t>(t, 0x244) = 0;
    t[0x217] = 0x80;
    t[0x201] = (uint8_t)((t[0x201] & 2) | 1);
    F<int32_t>(t, 0x248) = 0;
    F<int32_t>(t, 0x24d) = 0;
    t[0x252] = 0;
    t[0x24c] &= 8;
    Track_Setup(t, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3], Engine_Get1C(hook::As<void*>(g_engine)),
                (uint8_t)style);
    F<int32_t>(t, 0x230) = 0;
    if (!F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed)) {
        float cxf = (float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl_205(0x5cfcfc));
        float cyf = (float)(((double)F<float>(t, 0x34) + F<float>(t, 0x1c)) / Fl_205(0x5cfcfc));
        int32_t cy = Ftol(cyf);
        Map_SetCoarseByte(hook::As<void*>(g_map), 0, Ftol(cxf), cy, 0x80);
    }
    Track_DropAndSettle(t, 0, 0x40400000, 1);
    uint8_t newKind = (uint8_t)kind;
    if (newKind == 2 && t[0x216] != 2) {
        Obj_CountTrack(hook::As<uint8_t*>(g_tracks), 0, t, 1);
        Trains_Clear1a3(g_trains);
    }
    t[0x216] = newKind;
    Track_UpdateColor(t);
}

// 0x44e500. Tidies the water in a map rectangle (clamped; nothing when empty), in
// two passes over every water cell (kinds 2/4/5): a cell with one water neighbour
// (out of 8, not at the map edge, not yet marked) gets its flow mark (+1 bit 3 when
// that neighbour lies in its flow direction, rec bits 3-5, else bit 2); a cell
// with all four orthogonal or all four diagonal neighbours wet, or six or more,
// is drained; otherwise it is drained once per wet orthogonal neighbour that has a
// wet neighbour 90 degrees on, while it has at most two wet orthogonal
// neighbours. Then a flood queued in 0x6d1304 (+0x78 / +0x7c) is run and cleared;
// returns whether one was.
RT3_NOINLINE int32_t __fastcall Map_TidyWater(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    if (!Map_ClampRect(m, 0, &x0, &y0, &x1, &y1)) return 0;
    for (int pass = 0; pass < 2; ++pass) {
        for (int32_t x = x0; x <= x1; ++x) {
            for (int32_t y = y0; y <= y1; ++y) {
                if (!Terrain_KindIs2Or4Or5(m, 0, x, y)) continue;
                uint8_t wet[8] = {};
                int32_t n = 0, orth = 0, diag = 0;
                for (int k = 0; k < 8; ++k) {
                    int32_t nx = ((const int32_t*)0x624b28)[k] + x, ny = ((const int32_t*)0x624b48)[k] + y;
                    if (nx < 0 || nx >= F<int32_t>(m, 0x2155) || ny < 0 || ny >= F<int32_t>(m, 0x2159)) continue;
                    wet[k] = (uint8_t)Terrain_KindIs2Or4Or5(m, 0, nx, ny);
                    if (!wet[k]) continue;
                    ++n;
                    if (k & 1) ++diag;
                    else ++orth;
                }
                if (!n) continue;
                if (n == 1) {
                    if (Terrain_Rec(m, 0, x, y)[1] & 8) continue;
                    if (Terrain_Rec(m, 0, x, y)[1] & 4) continue;
                    if (x <= 0 || y <= 0 || x >= F<int32_t>(m, 0x2155) - 1 || y >= F<int32_t>(m, 0x2159) - 1) continue;
                    int dir = (Terrain_Rec(m, 0, x, y)[0] >> 3) & 7;
                    Terrain_Rec(m, 0, x, y)[1] |= wet[dir] ? 8 : 4;
                    continue;
                }
                if (orth == 4 || diag == 4 || n >= 6) {
                    Map_DrainCell(m, 0, x, y);
                    continue;
                }
                for (int d = 0; d < 8; d += 2)
                    if (wet[d] && (wet[(d + 2) % 8] || wet[(d + 6) % 8]) && orth <= 2) Map_DrainCell(m, 0, x, y);
            }
        }
    }
    uint8_t* q = g_6d1304;
    if (!q || F<int32_t>(q, 0x78) == -1) return 0;
    Water_FloodFrom(m, 0, F<int32_t>(q, 0x78), F<int32_t>(q, 0x7c));
    F<int32_t>(g_6d1304, 0x7c) = -1;
    F<int32_t>(g_6d1304, 0x78) = -1;
    return 1;
}
RT3_REPLACE(0x44e500, Map_TidyWater);

// 0x44e7d0. Marks dirty every tile whose centre is within 0x5c8dc0 of (x, y).
void __fastcall Map_MarkDirtyAround(uint8_t* m, int, float x, float y) {
    if (!Map_Contains(m, 0, x, y)) return;
    int32_t x0, x1, y0, y1;
    __asm {
        fld x
        fsub qword ptr ds:[0x5c8dc0]
        mov eax, 0x5a10d0
        call eax
        mov x0, eax
        fld x
        fadd qword ptr ds:[0x5c8c30]
        mov eax, 0x5a10d0
        call eax
        mov x1, eax
        fld y
        fsub qword ptr ds:[0x5c8dc0]
        mov eax, 0x5a10d0
        call eax
        mov y0, eax
        fld y
        fadd qword ptr ds:[0x5c8c30]
        mov eax, 0x5a10d0
        call eax
        mov y1, eax
    }
    if (x0 < 0) x0 = 0;
    if (x1 > F<int32_t>(m, 0x2155) - 1) x1 = F<int32_t>(m, 0x2155) - 1;
    if (y0 < 0) y0 = 0;
    if (y1 > F<int32_t>(m, 0x2159) - 1) y1 = F<int32_t>(m, 0x2159) - 1;
    for (int32_t j = y0; j <= y1; ++j) {
        float fy = Centre(j);
        for (int32_t i = x0; i <= x1; ++i)
            if (Distance2D(Centre(i), fy, x, y) < D(0x5c8dc0)) Map_MarkTileDirty(m, 0, i, j);
    }
}
RT3_REPLACE(0x44e7d0, Map_MarkDirtyAround);

// 0x44e910. Constructs map m (vtable 0x5ca9fc) from stream s.
RT3_NOINLINE uint8_t* __fastcall Map_ConstructLoad(uint8_t* m, int, int32_t s, const char* tex) {
    Terrain_Construct(m);
    F<uint32_t>(m, 0) = 0x5ca9fc;
    Map_InitRuntimeState(m);
    Map_Load(m, 0, s, tex);
    return m;
}
RT3_REPLACE(0x44e910, Map_ConstructLoad);

// 0x44e940. Rebuilds the shoreline of map m over vertex rect (x0, y0)-(x1, y1)
// after the land/water layout changed: tidies the water, flags vertices whose
// whole coast reach is one side, then for every texel of the terrain texture
// (tex, or the engine's own texture extracted for the rect when null) sets
// its alpha from the distance to the nearest differing vertex (land texels
// also shade toward sand on a fresh unsaved map), writes the edge depth
// nibbles, pastes the texture back, refreshes sites, territories and the
// water flow, and finally marks shore cells (0xc4) in the +0x162d grid.
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex) {
    if (!Map_ClampRect(m, 0, &x0, &y0, &x1, &y1)) return;
    int32_t tidied = Map_TidyWater(m, 0, x0, y0, x1, y1);
    int32_t recolour = 0;
    uint8_t* eng = g_engine;
    if (!F<int32_t>(g_map, 0x21a5) && !eng[0x1146eb]) {
        F<int32_t>(m, 0x21a5) = 1;
        recolour = 1;
    }
    int32_t extracted = 0;
    if (!tex) {
        if (Render_Get1588((uint8_t*)Engine_Get1C(g_engine)))
            tex = (uint32_t*)(uintptr_t)Render_Get1584((uint8_t*)Engine_Get1C(g_engine));
        else {
            tex = Terrain_ExtractTexture((uint8_t*)Engine_Get1C(g_engine), 0, 0.0f, 0.0f, 1.0f, 1.0f, nullptr, nullptr);
            extracted = 1;
        }
    }
    Map_CountLand(m);
    int32_t texW, texH;
    Render_GetSize4((uint8_t*)Engine_Get1C(g_engine), 0, &texW, &texH);
    int32_t W = F<int32_t>(m, 0x2155), H = F<int32_t>(m, 0x2159);
    float sx = (float)texW / (float)W;
    float sy = (float)texH / (float)H;
    float fx0 = (float)x0 * sx;
    int32_t tx0 = Ftol(fx0);
    float fy0 = (float)y0 * sy;
    int32_t ty0 = Ftol(fy0);
    float fx1 = (float)(x1 + 1) * sx;
    int32_t tx1 = Ftol(fx1);
    float fy1 = (float)(y1 + 1) * sy;
    int32_t ty1 = Ftol(fy1);
    if (tx1 >= texW - 1) tx1 = texW - 1;
    if (ty1 >= texH - 1) ty1 = texH - 1;
    uint8_t* flat = (uint8_t*)crt::malloc(H * W);
    memset(flat, 0, F<int32_t>(m, 0x2155) * F<int32_t>(m, 0x2159));
    // Vertices whose reach holds only their own side.
    for (int32_t y = y0; y <= y1; ++y)
        for (int32_t x = x0; x <= x1; ++x) {
            int32_t land = Coast_IsLand(m, x, y);
            if (Terrain_KindIs1Or3to5(m, 0, x, y)) {
                int32_t o = F<int32_t>(m, 0x15dd) * y + x;
                F<uint8_t*>(m, 0x1631)[o] = 0xff;
                F<uint8_t*>(m, 0x1635)[o] = 0xff;
                F<uint8_t*>(m, 0x1639)[o] = 0xff;
                F<uint8_t*>(m, 0x163d)[o] = 0xff;
            }
            int32_t r = Coast_Reach(m, x, y);
            int32_t ax0 = x - r - 1, ay0 = y - r - 1, ax1 = r + x + 1, ay1 = y + 1 + r;
            Map_ClampRect(m, 0, &ax0, &ay0, &ax1, &ay1);
            bool mixed = false;
            for (int32_t i = ax0; i <= ax1 && !mixed; ++i)
                for (int32_t j = ay0; j <= ay1; ++j)
                    if (Coast_IsLand(m, i, j) != land) {
                        mixed = true;
                        break;
                    }
            if (!mixed) flat[F<int32_t>(m, 0x2155) * y + x] = 1;
        }
    for (int32_t ty = ty0; ty <= ty1; ++ty) {
        if (tx0 > tx1) continue;
        float fy = (float)ty / sy;
        int32_t cy = Ftol(fy);
        for (int32_t tx = tx0; tx <= tx1; ++tx) {
            float fx = (float)tx / sx;
            int32_t cx = Ftol(fx);
            int32_t land = Coast_IsLand(m, cx, cy);
            int32_t k14 = Terrain_KindIs1Or4(m, 0, cx, cy);
            int32_t k35 = Terrain_KindIs3Or5(m, 0, cx, cy);
            int32_t r = Coast_Reach(m, cx, cy);
            float rf = (float)r;
            float dist = rf;
            float t;
            if (flat[F<int32_t>(m, 0x2155) * cy + cx] == 1)
                t = 1.0f;
            else {
                float best = (float)(r * r);
                int32_t bx0 = Ftol((double)fx - rf - Fl_207(0x5c85b4));
                int32_t by0 = Ftol((double)fy - rf - Fl_207(0x5c85b4));
                int32_t bx1 = Ftol((double)rf + fx + Fl_207(0x5c85b4));
                int32_t by1 = Ftol((double)rf + fy + Fl_207(0x5c85b4));
                if (!Map_ClampRect(m, 0, &bx0, &by0, &bx1, &by1)) continue;
                for (int32_t i = bx0; i <= bx1; ++i)
                    for (int32_t j = by0; j <= by1; ++j) {
                        if (Coast_IsLand(m, i, j) == land && Terrain_KindIs1Or4(m, 0, i, j) == k14 && Terrain_KindIs3Or5(m, 0, i, j) == k35)
                            continue;
                        float fi = (float)i, nx;
                        if (fi < fx) {
                            float fi1 = (float)(i + 1);
                            nx = fi1 < fx ? fi1 : fx;
                        } else
                            nx = fi;
                        float fj = (float)j, ny;
                        if (fj < fy) {
                            float fj1 = (float)(j + 1);
                            ny = fj1 < fy ? fj1 : fy;
                        } else
                            ny = fj;
                        float ddy = (float)((double)ny - fy);
                        float ddx = (float)((double)nx - fx);
                        float d = (float)((double)ddx * ddx + (double)ddy * ddy);
                        if (d < best) best = d;
                    }
                dist = (float)crt::sqrt(best);
                t = (float)((double)dist / rf);
                if (!(t <= Fl_207(0x5c85b4))) continue;
            }
            uint32_t* px = tex + (ty * texW + tx);
            if (land) {
                double td = t;
                double fresh;
                if (!Terrain_KindIs1Or3to5(m, 0, cx, cy))
                    fresh = (D(0x5c8770) - td) * D(0x5caaa0) + (double)t * Fl_207(0x5caa9c);
                else {
                    int32_t k = Terrain_KindIs3Or5(m, 0, cx, cy);
                    fresh = (D(0x5c8770) - td) * D(0x5caaa0) + (double)t * (k ? Fl_207(0x5c8700) : Fl_207(0x5caa98));
                }
                uint32_t a = (uint8_t)Ftol(fresh);
                uint32_t old = *px;
                *px = a << 24 | (old & 0xffffff);
                if (recolour) {
                    uint32_t b2 = ((uint8_t*)px)[2], b1 = ((uint8_t*)px)[1], b0 = old & 0xff;
                    int32_t c2, c1, c0;
                    if (Terrain_KindIs3Or5(m, 0, cx, cy)) {
                        c2 = 0x1f;
                        c1 = 0x3b;
                        c0 = 0x56;
                    } else {
                        c2 = 0x29;
                        c1 = 0x40;
                        c0 = 0x60;
                    }
                    float f = (float)(td * D(0x5caa90));
                    double g = D(0x5c8770) - f;
                    uint32_t v = ((uint32_t)Ftol((double)(float)c2 * f + (double)b2 * g) | a << 8) << 8;
                    v = (v | (uint32_t)Ftol((double)(float)c1 * f + (double)b1 * g)) << 8;
                    *px = v | (uint32_t)Ftol((double)(float)c0 * f + (double)b0 * g);
                }
                if (Terrain_KindIs2Or4Or5(m, 0, cx, cy)) continue;
                if (cx && cy && cx != F<int32_t>(m, 0x2155) - 1 && cy != F<int32_t>(m, 0x2159) - 1) continue;
                Coast_SetEdgeNibble(m, cx, cy, (uint8_t)(Ftol((double)t * Fl_207(0x5c873c)) & 0xf));
            } else {
                if (dist < Fl_207(0x5c878c) && (dist < Fl_207(0x5c85b4) || ((uint8_t*)px)[3] < 0x80)) ((uint8_t*)px)[3] = 0x80;
                if (Terrain_KindIs2Or4Or5(m, 0, cx, cy)) continue;
                int32_t lastX = F<int32_t>(m, 0x2155) - 1;
                if (!cx || !cy || cx == lastX || cy == F<int32_t>(m, 0x2159) - 1) Coast_Rec(m, cx, cy)[2] &= 0xf0;
                if (cx == F<int32_t>(m, 0x2155) - 1) Coast_Rec(m, cx, cy)[5] &= 0xf0;
                if (cy == F<int32_t>(m, 0x2159) - 1) Coast_Rec(m, cx, cy + 1)[2] &= 0xf0;
                if (cx == F<int32_t>(m, 0x2155) - 1 && cy == F<int32_t>(m, 0x2159) - 1) Coast_Rec(m, cx, cy + 1)[5] &= 0xf0;
            }
        }
    }
    float q0 = Fl_207(0x5c8598) > fx0 ? 0.0f : fx0;
    float q1 = Fl_207(0x5c8598) > fy0 ? 0.0f : fy0;
    float tw = (float)texW;
    float q2 = tw < fx1 ? tw : fx1;
    float th = (float)texH;
    float q3 = th < fy1 ? th : fy1;
    int32_t py1 = Ftol((double)q3 + Fl_207(0x5c85b4));
    int32_t px1 = Ftol((double)q2 + Fl_207(0x5c85b4));
    int32_t py0 = Ftol(q1);
    int32_t px0 = Ftol(q0);
    Terrain_PasteTexture((uint8_t*)Engine_Get1C(g_engine), 0, px0, py0, px1, py1);
    if (extracted) Terrain_CommitTexture((uint8_t*)Engine_Get1C(g_engine), 0, 0);
    void* cam = hook::As<void*>(g_62be68);
    ((void(__thiscall*)(void*))(*(void***)cam)[0])(cam);
    Map_ReclassifySites(m);
    Territories_Recount(g_territories);
    crt::free(flat);
    int32_t rx0 = x0, ry0 = y0, rx1 = x1, ry1 = y1;
    if (tidied) {
        if (F<int32_t>(m, 0x21ad) - 1 < x0) rx0 = F<int32_t>(m, 0x21ad) - 1;
        if (F<int32_t>(m, 0x21b1) - 1 < y0) ry0 = F<int32_t>(m, 0x21b1) - 1;
        if (F<int32_t>(m, 0x21b5) + 1 > x1) rx1 = F<int32_t>(m, 0x21b5) + 1;
        if (F<int32_t>(m, 0x21b9) + 1 > y1) ry1 = F<int32_t>(m, 0x21b9) + 1;
    }
    Terrain_RefreshWaterFlow(m, 0, rx0, ry0, rx1, ry1);
    if (x0 < 1) x0 = 1;
    if (y0 < 1) y0 = 1;
    if (F<int32_t>(m, 0x2155) - 2 < x1) x1 = F<int32_t>(m, 0x2155) - 2;
    if (F<int32_t>(m, 0x2159) - 2 < y1) y1 = F<int32_t>(m, 0x2159) - 2;
    const int32_t* dx = (const int32_t*)0x624b28;
    const int32_t* dy = (const int32_t*)0x624b48;
    for (int32_t y = y0; y <= y1; ++y)
        for (int32_t x = x0; x <= x1; ++x) {
            int32_t stride = F<int32_t>(m, 0x15dd);
            bool water = F<uint8_t*>(m, 0x1655)[stride * y + x] == 0;
            bool shore = false;
            for (int32_t k = 0; k < 8; k += 2)
                if ((F<uint8_t*>(m, 0x1655)[(dy[k] + y) * stride + dx[k] + x] != 0) == water) shore = true;
            if (shore) F<uint8_t*>(m, 0x162d)[((F<int32_t>(m, 0x15d9) >> (F<uint32_t>(m, 0x15e1) & 0x1f)) + 1) * y + x] = 0xc4;
        }
}
RT3_REPLACE(0x44e940, Map_RebuildCoast);

// 0x44f840. Repaints the whole terrain texture of map m from its vertex
// kinds: texels over kinds 1 / 4 get one of five shades from 0x5f040566 to
// 0x96586131 by the distance to water within 4 cells (the search radius
// follows the latest water distance found), others 0x96586131; then the
// coast is rebuilt over the full map and the texture committed.
RT3_NOINLINE void __fastcall Map_PaintShallows(uint8_t* m) {
    uint32_t* tex = Terrain_ExtractTexture(m, 0, 0.0f, 0.0f, 1.0f, 1.0f, nullptr, nullptr);
    int32_t w, h;
    Render_GetSize4(m, 0, &w, &h);
    uint32_t shades[5];
    for (int32_t i = 0; i < 5; ++i) shades[i] = BlendColor(0x5f040566, 0x96586131, (float)((double)i / D(0x5c8d68)));
    for (int32_t ty = 0; ty < h; ++ty) {
        float fty = (float)ty, fh = (float)h, fc = (float)(Terrain_Cols(m) - 1);
        int32_t cy = Ftol((double)fty / fh * fc);
        for (int32_t tx = 0; tx < w; ++tx) {
            float ftx = (float)tx, fw = (float)w, fr = (float)(Terrain_Rows(m) - 1);
            int32_t cx = Ftol((double)ftx / fw * fr);
            int32_t idx = ty * w + tx;
            uint16_t kind = *(const uint16_t*)Coast_Rec(m, cx, cy) & 7;
            if (kind != 1 && kind != 4) {
                tex[idx] = 0x96586131;
                continue;
            }
            int32_t r = 5;
            for (int32_t y = cy - 4; y <= cy + r - 1; ++y) {
                if (y < 0 || y >= F<int32_t>(m, 0x2159)) continue;
                for (int32_t x = cx - r + 1; x <= cx + r - 1; ++x) {
                    if (x < 0 || x >= F<int32_t>(m, 0x2155)) continue;
                    if (!(Coast_Rec(m, x, y)[0] & 7)) r = Ftol(Distance2D((float)cx, (float)cy, (float)x, (float)y));
                }
            }
            tex[idx] = r < 5 ? shades[r] : 0x5f040566;
        }
    }
    Map_RebuildCoast(m, 0, 0, 0, 0x270f, 0x270f, tex);
    Terrain_CommitTexture((uint8_t*)Engine_Get1C(g_engine), 0, 0);
}
RT3_REPLACE(0x44f840, Map_PaintShallows);

// 0x44faf0. Map m imports a heightmap (Terrain_ImportHeightmap), restarts
// the ambience and, unless it failed, paints the shallows; then the sites are
// reclassified, the territories recounted and the graphics options
// reapplied, with loading-screen steps. Returns the import result.
RT3_NOINLINE int32_t __fastcall Map_ImportHeightmap(uint8_t* m, int, const char* name, float scale, float curve, int32_t smooth) {
    int32_t ok = Terrain_ImportHeightmap(m, 0, name, scale, curve, smooth);
    Ambient_Start(hook::As<void*>(g_world));
    Loading_Pump(g_app, 0, 1, 0);
    if (!ok) Map_PaintShallows(m);
    Map_ReclassifySites(m);
    Territories_Recount(g_territories);
    Loading_Pump(g_app, 0, 1, 0);
    Options_ApplyGraphics(g_app, 0, -2, 1);
    return ok;
}
RT3_REPLACE(0x44faf0, Map_ImportHeightmap);

// 0x44fb70. Builds the route-planner costs: for every 4x4 block record (0x1d7
// bytes, +0x2129, W x H blocks +0x2145 / +0x2149) and each of the 8 directions
// (0x624b28 / 0x624b48) inside the map (+0x2155 / +0x2159), walks the 4 cells from
// the block centre: each step costs (1 + slope^2 / k) x a base (0.09, 0.072 into
// blocks flagged +0xe6 & 8, x0.8 out of them; 0.4 on water), diagonals scaled, plus
// a shore penalty; the total is scaled by the blocks' +0xe6 & 2/1 flags and stored
// as u16 (max 65000) in the record's +0xf3 table.
RT3_NOINLINE void __fastcall Routes_BuildCosts(uint8_t* t) {
    const int32_t* dxs = (const int32_t*)0x624b28;
    const int32_t* dys = (const int32_t*)0x624b48;
    for (int32_t by = 0, cy = 2; by < F<int32_t>(t, 0x2149); ++by, cy += 4) {
        for (int32_t bx = 0, cx = 2; bx < F<int32_t>(t, 0x2145); ++bx, cx += 4) {
            uint8_t* rec = F<uint8_t*>(t, 0x2129) + (F<int32_t>(t, 0x2145) * by + bx) * 0x1d7;
            uint16_t* slot = (uint16_t*)(rec + 0xf3);
            for (int32_t dir = 0; dir < 8; ++dir, ++slot) {
                int32_t ddx = dxs[dir], ddy = dys[dir];
                if (cx + ddx * 4 < 0 || cx + ddx * 4 >= F<int32_t>(t, 0x2155)) continue;
                if (cy + ddy * 4 < 0 || cy + ddy * 4 >= F<int32_t>(t, 0x2159)) continue;
                uint8_t* nrec = F<uint8_t*>(t, 0x2129) + ((ddy + by) * F<int32_t>(t, 0x2145) + ddx + bx) * 0x1d7;
                float sum = 0.0f;
                int32_t x = cx, y = cy;
                int32_t water0 = Terrain_KindIs1Or4(t, 0, cx, cy) == 0;
                float base = nrec[0xe6] & 8 ? 0.072f : 0.09f;
                if (rec[0xe6] & 8) base = (float)(base * D(0x5c8640));
                int32_t diag = dir & 1;
                for (int32_t step = 0; step < 4; ++step) {
                    int32_t nx = dxs[dir] + x, ny = dys[dir] + y;
                    int32_t water = Terrain_KindIs1Or4(t, 0, nx, ny) == 0;
                    Terrain_KindIs1Or4(t, 0, x, y);
                    float hn = (float)Map_GridFloat(t, 0, nx, ny);
                    float d = (float)((double)hn - Map_GridFloat(t, 0, x, y));
                    float k = (float)(water | water0 ? 0.4 : (double)base);
                    if (diag) {
                        d = (float)(d / D(0x5caab0));
                        k = (float)(k * D(0x5caab0));
                    }
                    d = (float)(d / D(0x5c8798));
                    if (d < Fl_194(0x5c8598)) d = (float)(crt::fabs(d) / D(0x5c85c8));
                    d = (float)((double)d * d);
                    sum = (float)(((double)d + D(0x5c8698)) / D(0x5c8698) * k + sum);
                    if (water0 != water) sum = (float)(sum + D(0x5c8640));
                    x = nx;
                    y = ny;
                }
                uint8_t fa = rec[0xe6], fb = nrec[0xe6];
                int32_t n = (fa & 2 ? 1 : 0) + (fb & 2 ? 1 : 0);
                if (n == 0) sum = (float)(sum * D(0x5c87f0));
                else if (n == 1) {
                    if (!(fa & 1) && !(fb & 1)) sum = (float)(sum * D(0x5c8578));
                } else sum = (float)(sum * D(0x5c88e8));
                float v = (float)(sum * D(0x5c9a00));
                if (v > Fl_194(0x5caaa8)) v = 65000.0f;
                *slot = (uint16_t)Ftol(v);
            }
        }
    }
}
RT3_REPLACE(0x44fb70, Routes_BuildCosts);

// 0x44ff10 / 0x44ff70. Per map cell: free its key list / reset it.
RT3_NOINLINE void __fastcall Map_FreeCellKeyLists(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Obj_FreeKeyList(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
}
RT3_REPLACE(0x44ff10, Map_FreeCellKeyLists);

RT3_NOINLINE void __fastcall Map_ResetCells(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Station_Reset(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7, 0, (uint16_t)x, (uint16_t)y);
}
RT3_REPLACE(0x44ff70, Map_ResetCells);

// 0x44ffd0. Refreshes every region cell's cargo and flags.
RT3_NOINLINE void __fastcall Map_RefreshCells(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            City_Refresh(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
}
RT3_REPLACE(0x44ffd0, Map_RefreshCells);

// 0x450030. Monthly map economy (not in sandbox / scenario-war modes): every 4th
// block record (+0x2129, 0x1d7 bytes; from the day & 3) whose phase (a fixed
// table 0x62c150 of 0..13 drawn once with seed 0x61) matches the day % 14 runs its
// economy; then all blocks cap their stocks.
void __fastcall Map_MonthlyEconomy(uint8_t* m) {
    if (g_newGame[0x82]) return;
    uint8_t* world = g_world;
    if (F<int32_t>(world, 0x4af7) || F<int32_t>(world, 0x4aef)) return;
    if (!g_62c254) {
        PushSeed();
        SetSeed(0x61);
        for (int32_t i = 0; i < 0x100; ++i) hook::Global<uint8_t>(0x62c150 + i) = (uint8_t)SimRand(0, 0xd);
        PopSeed();
        world = g_world;
        g_62c254 = 1;
    }
    int32_t phase = world[0x11] % 14;
    int32_t n = F<int32_t>(m, 0x2149) * F<int32_t>(m, 0x2145);
    for (int32_t i = world[0x14] & 3; i < n; i += 4)
        if (hook::Global<uint8_t>(0x62c150 + (i & 0xff)) == phase) Region_MonthlyEconomy(F<uint8_t*>(m, 0x2129) + i * 0x1d7);
    for (int32_t i = 0; i < n; ++i) Station_CapStocks(F<uint8_t*>(m, 0x2129) + i * 0x1d7);
}
RT3_REPLACE(0x450030, Map_MonthlyEconomy);

RT3_NOINLINE int32_t __fastcall Obj_Byte21c5(void* o) { return F<uint8_t>(o, 0x21c5); }
RT3_REPLACE(0x450150, Obj_Byte21c5);

// 0x450160. For the cargo overlay (mode +0x2175 == 3, cargo +0x2179 % 53) works out
// the colour range (+0x2195 base, +0x2199 span) of the region quotas over land cells
// (200 without a cargo): from min to max, lowered towards the lowest supplying cell
// when that is far below the average, widened to at least a minimum span.
RT3_NOINLINE void __fastcall Map_UpdateQuotaRange(uint8_t* m) {
    int32_t cargo = F<int32_t>(m, 0x2179) % 0x35;
    if (F<int32_t>(m, 0x2175) != 3) return;
    float lo = 1e10f, hi = Bits(0xd01502f9), supply = 1e10f, sum = 0.0f;
    int32_t count = 0, found = 0, any = 0;
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x) {
            uint8_t* cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7;
            uint8_t f = cell[0xe6];
            if ((f & 1) && !(f & 4)) continue;
            float val = cargo ? (float)City_CargoQuota(cell, 0, cargo) : 200.0f;
            if (val > hi) hi = val;
            if (val < lo) lo = val;
            if (val < supply && Obj_Bit(cell, 0, cargo)) {
                supply = val;
                found = 1;
            }
            sum = (float)((double)val + sum);
            ++count;
            any = 1;
        }
    if (!any) lo = hi = 10000.0f;
    float avg = (float)((double)sum / (float)(count < 1 ? 1 : count));
    float t;
    if (avg < Fl(0x5c8db0)) t = 15000.0f;
    else if (avg < Fl(0x5caac8)) t = 20000.0f;
    else if (avg < Fl(0x5caac4)) t = 25000.0f;
    else if (avg < Fl(0x5caac0)) t = 30000.0f;
    else t = avg < Fl(0x5c88bc) ? 35000.0f : 40000.0f;
    if (found) {
        float s = (float)((double)supply - t);
        if (lo < s && (double)avg - (double)t * D(0x5c8750) > lo) lo = s;
    }
    if ((float)((double)hi - lo) < Fl(0x5c99c8)) {
        float mid = (float)(((double)hi + lo) * D(0x5c8628));
        hi = (float)(mid + D(0x5caab8));
        lo = (float)(mid - D(0x5caab8));
        if (lo < Fl(0x5c8598)) {
            hi = (float)(crt::fabs(lo) + hi);
            lo = 0.0f;
        }
    }
    F<float>(m, 0x2195) = lo;
    float span = (float)((double)hi - lo);
    F<float>(m, 0x2199) = span;
    if (span < Fl(0x5c85b4)) F<float>(m, 0x2199) = 1.0f;
}
RT3_REPLACE(0x450160, Map_UpdateQuotaRange);

// 0x450490. If the cell's key list has `key` (or any entry for key 0):
// world +0x66ba | 0x2c000000 (a colour); else 0.
RT3_NOINLINE uint32_t __fastcall Map_CellKeyColour(uint8_t* m, int, int32_t x, int32_t y, int32_t key) {
    uint8_t* cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7;
    const uint8_t* e = F<uint8_t*>(cell, 0xef);
    for (int32_t i = 0; i < F<int32_t>(cell, 0xeb); ++i, e += 6)
        if (F<uint16_t>((void*)e, 0) == key || key == 0)
            return F<uint32_t>(g_world, 0x66ba) | 0x2c000000;
    return 0;
}
RT3_REPLACE(0x450490, Map_CellKeyColour);

// 0x450520. Redraws editor map t's overview texture for the dirty rect
// +0x2185..+0x2191 (clipped to the mode's grid from table 0x5ee5e0 and the
// screen, at most 2048): sizes the texture to the smallest halving still
// covering it, then fills each cell's block with its colour for the
// overview mode +0x2175 (a 4-int rect `arg` limits the area for the
// rect-based modes, otherwise clears first; some modes read arg as a key or
// a byte map), then sets the mode's flag (0x5ee5e4), refreshes and places
// the texture at the rect scaled by the grid's cell size.
RT3_NOINLINE void __fastcall Overview_Redraw(uint8_t* t, int, void* arg) {
    int32_t mode = F<int32_t>(t, 0x2175);
    if (!mode) return;
    if (mode == 3) Map_UpdateQuotaRange(t);
    int32_t gw = 0, gh = 0;
    float scale = 0;
    switch (hook::Global<uint32_t>(0x5ee5e0 + F<int32_t>(t, 0x2175) * 0xc)) {
    case 0: gw = F<int32_t>(t, 0x2145); gh = F<int32_t>(t, 0x2149); scale = 4.0f; break;
    case 1: gw = F<int32_t>(t, 0x2155) / 16; gh = F<int32_t>(t, 0x2159) / 16; scale = 16.0f; break;
    case 2: gw = F<int32_t>(t, 0x214d); gh = F<int32_t>(t, 0x2151); scale = 4.0f; break;
    case 3: gw = F<int32_t>(t, 0x2155); gh = F<int32_t>(t, 0x2159); scale = 1.0f; break;
    case 4:
        gw = F<int32_t>(t, 0x215d);
        gh = F<int32_t>(t, 0x2161);
        scale = 0.25f;
        if (F<int32_t>(t, 0x2155) > 0x200 || F<int32_t>(t, 0x2159) > 0x200) {
            gw /= 2;
            gh /= 2;
            scale = 0.5f;
        }
        break;
    }
    int32_t x1 = F<int32_t>(t, 0x218d) < gw - 1 ? F<int32_t>(t, 0x218d) : gw - 1;
    F<int32_t>(t, 0x218d) = x1;
    int32_t y1 = F<int32_t>(t, 0x2191) >= gh - 1 ? gh - 1 : F<int32_t>(t, 0x2191);
    F<int32_t>(t, 0x2191) = y1;
    uint8_t* e = g_engine;
    int32_t texW = F<int32_t>(e, 0x11423b) > 0x800 ? 0x800 : F<int32_t>(e, 0x11423b);
    int32_t texH = F<int32_t>(e, 0x11423f) > 0x800 ? 0x800 : F<int32_t>(e, 0x11423f);
    if (x1 >= F<int32_t>(t, 0x2185) + texW - 1) x1 = F<int32_t>(t, 0x2185) + texW - 1;
    F<int32_t>(t, 0x218d) = x1;
    if (!(y1 < F<int32_t>(t, 0x2189) + texH - 1)) y1 = F<int32_t>(t, 0x2189) + texH - 1;
    F<int32_t>(t, 0x2191) = y1;
    int32_t cellsW = x1 - F<int32_t>(t, 0x2185) + 1, cellsH = y1 - F<int32_t>(t, 0x2189) + 1;
    int32_t tw = texW, th = texH;
    for (int32_t v = texW >> 1; v >= 4 && v >= cellsW; v >>= 1) tw = v;
    for (int32_t v = texH >> 1; v >= 4 && v >= cellsH; v >>= 1) th = v;
    int32_t pxW = tw / cellsW, pxH = th / cellsH;
    uint32_t* tex = nullptr;
    uint32_t cw, ch;
    Render_ColourMap(t, 0, &tex, &cw, &ch);
    if (!tex || cw != (uint32_t)tw || ch != (uint32_t)th) {
        if (tex) Render_DestroyColourMap(t);
        tex = Overview_Pixels(t, 0, tw, th);
    }
    int32_t ry0 = F<int32_t>(t, 0x2189), rx1 = F<int32_t>(t, 0x218d), rx0 = F<int32_t>(t, 0x2185), ry1 = F<int32_t>(t, 0x2191);
    if (arg) {
        int32_t m = F<int32_t>(t, 0x2175);
        if (m == 0x2c || m == 0x18 || m == 0x1a || m == 0x19 || m == 0x1d || m == 0x1c) {
            const int32_t* r = (const int32_t*)arg;
            int32_t a = r[0], b = r[1], c = r[2], d = r[3];
            if (m == 0x2c && (F<int32_t>(t, 0x2155) > 0x200 || F<int32_t>(t, 0x2159) > 0x200)) {
                a >>= 1;
                b >>= 1;
                c >>= 1;
                d >>= 1;
            }
            if (!(rx0 > a)) rx0 = a;
            if (!(ry0 > b)) ry0 = b;
            if (!(rx1 < c)) rx1 = c;
            if (!(ry1 < d)) ry1 = d;
        } else {
            memset(tex, 0, (size_t)th * tw * 4);
        }
    }
    bool big = F<int32_t>(t, 0x2155) > 0x200 || F<int32_t>(t, 0x2159) > 0x200;
    uint32_t colour = 0;
    for (int32_t y = ry0; y <= ry1; ++y) {
        for (int32_t x = rx0; x <= rx1; ++x) {
            int32_t sx = big ? x * 2 : x, sy = big ? y * 2 : y;
            colour = Overview_CellColour(t, arg, x, y, sx, sy, colour);
            for (int32_t j = 0; j < pxH; ++j) {
                if (pxW <= 0) continue;
                uint32_t* p = tex + (y * pxH + j) * tw + x * pxW;
                for (int32_t i = 0; i < pxW; ++i) p[i] = colour;
            }
        }
    }
    Render_Set159a(t, 0, hook::Global<uint8_t>(0x5ee5e4 + F<int32_t>(t, 0x2175) * 0xc));
    Overview_Refresh(t);
    int32_t oy = F<int32_t>(t, 0x2189), ox = F<int32_t>(t, 0x2185);
    float f1 = (float)(oy + th);
    int32_t a1 = Ftol((double)f1 * scale);
    float f2 = (float)(ox + tw);
    int32_t a2 = Ftol((double)f2 * scale);
    float f3 = (float)oy;
    int32_t a3 = Ftol((double)f3 * scale);
    float f4 = (float)ox;
    int32_t a4 = Ftol((double)f4 * scale);
    Render_SetRect15a4(t, 0, (uint32_t)a4, (uint32_t)a3, (uint32_t)a2, (uint32_t)a1);
    Render_Enable159b(t);
}
RT3_REPLACE(0x450520, Overview_Redraw);

// 0x4515c0. Map overlay for the 4x4 block of cell (cx, cy) at screen (x, y).
// The block record is 0x1d7 bytes, rows of +0x2145 blocks from +0x2129. It
// draws a stack of icons for the block's amount: everything (0x42be20) when
// no cargo +0x2179 is picked, else that cargo's amount (+0x103 + c % 53 * 4).
// Each icon is up to a unit. With a cargo it also draws two bars, the
// block's demand (+0x6a word / 0x5c9c40) and its output (0x42bf20). The
// icons and bars are sized by the camera distance, and they fade with the
// view height 0x62be68 +0x14 and the camera's +0x20. Below height 0x5cab10
// everything is drawn at fade `fade`.
RT3_NOINLINE void __fastcall Map_DrawCellOverlay(uint8_t* m, int, float x, float y, uint32_t fade, int32_t cx, int32_t cy) {
    uint8_t* cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * (cy / 4) + cx / 4) * 0x1d7;
    uint32_t color = 0xff000000;
    float h = F<float>(g_62be68, 0x14);
    if (h < D(0x5c8618)) return;
    float k = 1.0f;
    if (h < D(0x5cab18)) {
        k = (float)((h - D(0x5c8618)) / D(0x5c86b0));
        color = (uint32_t)Ftol((double)k * Fl_205(0x5ca0ec)) << 24;
    }
    int32_t mode = F<int32_t>(m, 0x2179);
    float count, demand, output;
    if (mode == 0) {
        count = Station_TotalAvailable(cell);
        demand = 0.0f;
        output = 0.0f;
    } else {
        double d = (double)F<uint16_t>(cell, 0x6a + mode * 2);
        demand = (float)(d / D(0x5c9c40));
        count = F<float>(cell, 0x103 + mode % 0x35 * 4);
        output = City_CargoOutput(cell, 0, mode);
    }
    uint8_t* view = g_62be68;
    uint32_t color2 = color;
    if (F<float>(view, 0x14) < D(0x5cab10)) {
        Fade_Save();
        Fade_Set(fade);
        view = g_62be68;
    }
    if (F<int32_t>(m, 0x2179) == 0) {
        int32_t t = Ftol(F<float>(view, 0x20));
        if (t >= 10000) {
            if (t < 15000) {
                float f = (float)(t - 10000);
                k = (float)((D(0x5c8770) - (double)f / Fl_205(0x5c85b0)) * k);
                color2 = (uint32_t)Ftol((double)k * Fl_205(0x5ca0ec)) << 24 | (color & 0xffffff);
            } else
                color2 = 0;
        }
    }
    float alpha;
    {
        int32_t t = Ftol(F<float>(view, 0x20));
        if (t < 3000)
            alpha = 0.0f;
        else if (t < 0x1194) {
            float f = (float)(t - 3000);
            alpha = (float)(D(0x5c8770) - (double)f / Fl_205(0x5ca1a4));
        } else
            alpha = 1.0f;
    }
    if (count > Fl_205(0x5c8598) || demand > D(0x5c85b8) || output > D(0x5c85b8)) {
        float fy = (float)(cy * 0x5c);
        float gh = (float)Grid_Float1605(g_map, 0, cx, cy);
        float fx = (float)(cx * 0x5c);
        float dist = (float)Distance3D(F<float>(view, 0x1c), F<float>(view, 0x20), F<float>(view, 0x24), fx, gh, fy);
        if (dist > Fl_205(0x5ca1c8)) dist = (float)(((double)dist - Fl_205(0x5ca1c8)) * D(0x5c8c18) + D(0x5cab08));
        if (!(dist > Fl_205(0x5c873c)) || !(dist < Fl_205(0x5c8d00))) goto done;
        {
            float scale = (float)(D(0x5cab00) / dist);
            if (scale > D(0x5c86b8)) goto done;
            if (Fl_205(0x5c8598) < count) {
                int32_t i = 0;
                float fi = 0.0f;
                do {
                    float rem = count - fi;
                    if (!(rem < D(0x5c9ba0))) {
                        uint32_t c = color;
                        if (rem < D(0x5c8758)) c = color2;
                        if (!(rem < D(0x5c8758)) || c) {
                            float sz = !(rem >= Fl_205(0x5c85b4)) ? (float)((double)rem * Fl_205(0x5c8d70)) : 36.0f;
                            int32_t icon = Ftol(sz);
                            float w = (float)(scale / D(0x5caaf8));
                            uint8_t* glyph = (uint8_t*)Table_Entry(F<uint8_t*>(g_world, 0x46a90), 0, icon);
                            float hh;
                            if (rem < Fl_205(0x5c85b4)) {
                                double n = (double)(icon + 4);
                                hh = (float)(n / D(0x5ca2d0) * w);
                            } else
                                hh = (float)(w * D(0x5caaf0));
                            if (!Tutorial_Blink348()) {
                                if (!(rem < D(0x5c8758))) {
                                    float gx = (float)(x - scale * D(0x5c8628));
                                    Glyph_Queue(glyph, 0, gx, y - hh, 0x3f800000, w, w, color, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
                                } else {
                                    if (alpha > D(0x5c9ba0)) {
                                        uint32_t a = (uint32_t)Ftol((double)alpha * k * Fl_205(0x5ca0ec));
                                        float gx = (float)(x - scale * D(0x5c8628));
                                        Ui_FillRect(a << 24 | (color & 0xffffff), 1, gx, y - hh, scale, hh, 1);
                                    }
                                    if (alpha < D(0x5c8650)) {
                                        uint32_t a = (uint32_t)Ftol((D(0x5c8770) - alpha) * k * D(0x5ca110));
                                        float gx = (float)(x - scale * D(0x5c8628));
                                        Glyph_Queue(glyph, 0, gx, y - hh, 0x3f800000, w, w, a << 24 | (color & 0xffffff), 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
                                    }
                                }
                            }
                            y = (float)(y - hh * D(0x5c87e8));
                        }
                    }
                    ++i;
                    fi = (float)i;
                } while (fi < count);
            }
            float gapX = BitsF(0x3aa3d70au), gapY = BitsF(0x3ada740eu);
            if (dist < Fl_205(0x5ca1c8)) {
                float d = dist > Fl_205(0x5ca06c) ? dist : 500.0f;
                float r = Fl_205(0x5ca1c8) / d;
                gapX = r * Fl_205(0x5caaec);
                gapY = r * Fl_205(0x5caae8);
            }
            if (!(demand > D(0x5c85b8)) && !(output > D(0x5c85b8))) goto done;
            y = (float)(y - D(0x5caae0));
            auto lines = [] { return *(int32_t*)(F<uint8_t*>(g_engine, 0x34) + F<int32_t>(g_engine, 0x11421e) * 0x15 + 4); };
            float m1 = (float)(D(0x5caad8) / dist);
            double minLen = D(0x5c87b8) / (double)lines();
            float cap = (float)(m1 > minLen ? (double)m1 : minLen);
            float halfW = Fl_205(0x5c8780) / dist;
            if (Tutorial_Blink352()) goto done;
            // One bar: fill from y up by its length, with a white outline
            // (`outlineUp` decides on which side the outline's base sits).
            auto bar = [&](float v, uint32_t tint, uint32_t solid, bool first) {
                float n = (float)lines();
                float snapped = (float)Ftol((double)n * y);
                y = (float)((double)snapped / n + D(0x5c88a0));
                float len = (float)((double)v * Fl_205(0x5c8d4c) / dist);
                uint32_t c;
                if (len < cap) {
                    float f = len / cap;
                    if (f < D(0x5c8618)) f = BitsF(0x3eb33333u);
                    c = (uint32_t)Ftol((double)f * Fl_205(0x5ca0ec)) << 24 | tint;
                    len = cap;
                } else {
                    c = solid;
                    if (v > D(0x5c8628)) {
                        if (first) {
                            float e = (float)((double)y - len - gapY);
                            float fd = Fade_Step_Impl();
                            Engine_QueueQuad(hook::As<void*>(g_engine), 0, FBits((float)((double)halfW + gapX + x)), FBits(e), FBits(x), FBits((float)((double)gapY + y)),
                                             FBits((float)((double)x - halfW - gapX)), FBits(e), FBits(fd), 0xffffffff, 0xffffffff, 0xffffffff, 0, 0, 0, 0x3f800000,
                                             0x3f800000, 0x3f800000, 0, 0x3f800000, 0x3f800000, 0x3f800000);
                        } else {
                            float e = (float)((double)gapY + y);
                            float fd = Fade_Step_Impl();
                            Engine_QueueQuad(hook::As<void*>(g_engine), 0, FBits(x), FBits((float)((double)y - len - gapY)), FBits((float)((double)halfW + gapX + x)), FBits(e),
                                             FBits((float)((double)x - halfW - gapX)), FBits(e), FBits(fd), 0xffffffff, 0xffffffff, 0xffffffff, 0, 0, 0, 0x3f800000,
                                             0x3f800000, 0x3f800000, 0, 0x3f800000, 0x3f800000, 0x3f800000);
                        }
                    }
                }
                float fd = Fade_Step_Impl();
                if (first) {
                    float e = y - len;
                    Engine_QueueQuad(hook::As<void*>(g_engine), 0, FBits((float)((double)halfW + x)), FBits(e), FBits(x), FBits(y), FBits((float)((double)x - halfW)), FBits(e),
                                     FBits(fd), c, c, c, 0, 0, 0, 0x3f800000, 0x3f800000, 0x3f800000, 0, 0x3f800000, 0x3f800000, 0x3f800000);
                    y = (float)(y - (len - D(0x5caad0)));
                } else
                    Engine_QueueQuad(hook::As<void*>(g_engine), 0, FBits(x), FBits(y - len), FBits((float)((double)halfW + x)), FBits(y), FBits((float)((double)x - halfW)), FBits(y),
                                     FBits(fd), c, c, c, 0, 0, 0, 0x3f800000, 0x3f800000, 0x3f800000, 0, 0x3f800000, 0x3f800000, 0x3f800000);
            };
            if (demand > D(0x5c85b8)) bar(demand, 0xa0ffd0, 0xffa0ffd0, true);
            if (output > D(0x5c85b8)) bar(output, 0xffc0c0, 0xffffc0c0, false);
        }
    }
done:
    if (F<float>(g_62be68, 0x14) < D(0x5cab10)) Fade_Restore();
}
RT3_REPLACE(0x4515c0, Map_DrawCellOverlay);

// 0x452100. Text label of map overlay mode +0x2171 for cell (cx, cy) at screen
// (x, y); `fade` is the depth to draw it at.
//   1: the cargo overlay (icons by 0x4515c0). With a station +0x2181 picked,
//      label the building of the cell (or its city) with the cargo's demand
//      there against the station's supply (+n / -n), or, for a carried cargo,
//      the price a train would get (+rate (amount)). The station's own block
//      shows its name, drawn above the building.
//   2: block value +0xf3[cargo % 8] / 0x5caacc.  3: quota and level.
//   4: level (+0x103).  5: Obj_Level.
//   6: the placement preview 0x6cfe18 (X in the middle, o around, dots at
//      the edge of its radius).
// Labels fade out from view height 0x5c8880 (plus a bonus near the station).
void __fastcall Map_DrawCellLabel(uint8_t* m, int, float x, float y, float fade, int32_t cx, int32_t cy) {
    if (!Camera_InBounds(g_62be68, 0, x, y)) return;
    uint8_t* cell = nullptr;
    float lift = 0.0f, quota = 0.0f;
    int32_t forced = 0;
    int32_t hasPax = 0, dist = 0;
    char text[0x130], buf[0x190], num[0x20];
    text[0] = 0;
    if (F<int32_t>(m, 0x2171) == 1) {
        Map_DrawCellOverlay(m, 0, x, y, FBits(fade), cx, cy);
        if (F<int32_t>(m, 0x2179) <= 0 || F<int32_t>(m, 0x2181) <= 0) return;
        cell = F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * (cy / 4) + cx / 4) * 0x1d7;
        if (Chain_Contains(cell, 0, F<uint16_t>(m, 0x2181))) {
            forced = 1;
            lift = 100.0f;
        } else {
            if ((!(cell[0xe6] & 0x10) || F<int32_t>(cell, 0xeb) > 0) && !Chain_AnyActive(cell)) return;
            if (!Collection_IsValid(g_stations, 0, F<uint32_t>(m, 0x2181))) return;
            uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(m, 0x2181));
            if (!st) return;
            int32_t q = City_CargoQuota(Obj_BuildingRegion(st), 0, F<int32_t>(m, 0x2179));
            lift = BitsF(0x3e4ccccdu);
            quota = (float)q;
            hasPax = F<int16_t>(cell, 0xd4) != 0;
            int32_t sy = Ftol(Obj_BuildingCellY(st));
            int32_t sx = Ftol(Obj_BuildingCellX(st));
            dist = (uint16_t)DistanceInt(cx, cy, sx, sy);
            if (dist < 0x46)
                lift = 100.0f;
            else if (dist < 0x82) {
                double d = (double)(dist - 0x46);
                lift = (float)((D(0x5c8770) - d / D(0x5c88a8)) * D(0x5c8758) + D(0x5cab80));
            }
        }
    }
    float h = F<float>(g_62be68, 0x14) + lift;
    float zOff = 0.0f;
    uint32_t color = 0xffcf1f00;
    int32_t flags = 2;
    int32_t mode = F<int32_t>(m, 0x2171);
    if (mode != 6) {
        if (h < D(0x5c8880)) return;
        if (h < D(0x5cab78)) {
            h = (float)((h - D(0x5c8880)) / D(0x5cab70));
            color = (uint32_t)Ftol((double)h * Fl_205(0x5ca0ec)) << 24 | 0xcf1f00;
        }
    }
    switch (mode) {
    case 1: {
        uint8_t* b = nullptr;
        uint8_t* site = (uint8_t*)Obj_FirstActiveInChain(cell);
        if (site)
            b = (uint8_t*)Obj_Building(site);
        else if (cell[0xe6] & 0x10) {
            float fy = (float)cy, fx = (float)cx;
            int32_t id = Table_NearestValue23a(g_cities, 0, fx, fy);
            if (!id) return;
            b = (uint8_t*)Collection_Get(g_cities, 0, (uint32_t)id);
        }
        bool own = forced != 0;
        if (own && Collection_Get(g_stations, 0, F<uint32_t>(m, 0x2181)))
            b = (uint8_t*)Obj_Building(Collection_Get(g_stations, 0, F<uint32_t>(m, 0x2181)));
        if (b) {
            float bz = F<float>(b, 0x1e6);
            double t = Camera_TargetAboveGround(g_62be68) / Fl_205(0x5c8c28) + Fl_205(0x5cab50);
            double dd = (double)dist;
            bz = (float)(t + dd * D(0x5c8578) + bz);
            Scene_ProjectNormalised((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, F<float>(b, 0x1e2), bz, F<float>(b, 0x1ea), &x, &y,
                                    &fade);
        }
        if (own) {
            uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(m, 0x2181));
            if (st) FormatText(text, (const char*)st + 0x46b, 0, 0, 0, 0, 0, 0, 0, 0);
            color |= 0xffffff;
        } else if (F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x9a + F<int32_t>(m, 0x2179) * 4) == -1) {
            int32_t k = F<int32_t>(m, 0x2179) % 0x35;
            int32_t q = City_CargoQuota(Grid_RegionAt(m, 0, cx / 4, cy / 4), 0, k);
            float qf = (float)q;
            int32_t a = Ftol((double)qf / Fl_205(0x5cab40));
            int32_t diff = Ftol((double)quota / Fl_205(0x5cab40)) - a;
            int32_t mag = diff < 0 ? -diff : diff;
            Num_FormatSimple(buf, 9, (double)mag, 0xb);
            color &= 0xff000000;
            if (diff == 0) {
                FormatText(text, buf, 0, 0, 0, 0, 0, 0, 0, 0);
                color |= 0xdfdfdf;
            } else if (diff > 0) {
                FormatText(text, (const char*)0x5cab3c, buf, 0, 0, 0, 0, 0, 0, 0);
                color |= 0xbfffbf;
            } else {
                FormatText(text, (const char*)0x5cab38, buf, 0, 0, 0, 0, 0, 0, 0);
                color |= 0xffbfbf;
            }
        } else {
            uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(m, 0x2181));
            if (!st || !site) return;
            float cnt, amt;
            Train_GatherLoads(st, 0, 1, nullptr, site, F<int32_t>(m, 0x2179), BitsF(0x4479c000u), &cnt, &amt, nullptr);
            float rate = 0.0f;
            if (!(cnt == Fl_205(0x5c8598))) rate = amt / cnt;
            rate = (float)((double)rate / Fl_205(0x5c86a0) + D(0x5c8628));
            if (cnt < D(0x5c8628))
                text[0] = 0;
            else {
                char count[0x40];
                Num_Format(count, 4, (double)cnt, 10, 1, 0, 0);
                FormatText(text, (const char*)0x5cab44, Num_FormatSimple(buf, 9, (double)rate, 0xb), count, 0, 0, 0, 0, 0, 0);
            }
            color &= 0xff000000;
            color |= !(rate >= Fl_205(0x5c85b4)) ? 0xdfdfdf : 0xbfffbf;
        }
        flags = hasPax ? 0x80 : 0;
        zOff = -2.0f;
        break;
    }
    case 2: {
        uint8_t* r = Grid_RegionAt(m, 0, cx / 4, cy / 4);
        float v = (float)F<uint16_t>(r, 0xf3 + F<int32_t>(m, 0x2179) % 8 * 2);
        v = v / Fl_205(0x5caacc);
        crt::sprintf(text, (const char*)0x5cab54, (double)v);
        break;
    }
    case 3: {
        int32_t k = F<int32_t>(m, 0x2179) % 0x35;
        float qf = (float)City_CargoQuota(Grid_RegionAt(m, 0, cx / 4, cy / 4), 0, k);
        Num_FormatSimple(text, 9, (double)qf, 0xb);
        uint8_t* r = Grid_RegionAt(m, 0, cx / 4, cy / 4);
        float v = F<float>(r, 0x103 + F<int32_t>(m, 0x2179) % 0x35 * 4);
        crt::sprintf(buf, (const char*)0x5cab30, (double)v);
        strcat(text, buf);
        break;
    }
    case 4: {
        uint8_t* r = Grid_RegionAt(m, 0, cx / 4, cy / 4);
        float v = F<float>(r, 0x103 + F<int32_t>(m, 0x2179) % 0x35 * 4);
        crt::sprintf(text, (const char*)0x5cab54, (double)v);
        break;
    }
    case 5: {
        int32_t k = F<int32_t>(m, 0x2179) % 0x35;
        float v = Obj_Level(Grid_RegionAt(m, 0, cx / 4, cy / 4), 0, k);
        crt::sprintf(text, (const char*)0x5cab20, (double)v);
        break;
    }
    case 6: {
        uint8_t* p = g_6cfe18;
        if (!p) return;
        float fa, fb, fr;
        Obj_GetDims(p, 0, (int32_t*)&fa, (int32_t*)&fb, &fr);
        fa = (float)(fa + D(0x5c8628));
        fb = (float)(fb + D(0x5c8628));
        int32_t px = Ftol(fa), py = Ftol(fb), r = Ftol(fr);
        if ((px - cx < 0 ? cx - px : px - cx) + (py - cy < 0 ? cy - py : py - cy) > r * 2 + 4) return;
        int32_t lx = px - r - 2, ly = py - r - 2;
        if (lx < 0) lx = 0;
        if (ly < 0) ly = 0;
        int32_t hx = r + px + 2, hy = r + py + 2;
        if (F<int32_t>(m, 0x15d9) - 1 < hx) hx = F<int32_t>(m, 0x15d9) - 1;
        if (F<int32_t>(m, 0x15d5) - 1 < hy) hy = F<int32_t>(m, 0x15d5) - 1;
        if (cx < lx || cx > hx || cy < ly || cy > hy) return;
        if (r == 0)
            lift = cx == px && cy == py ? 1.0f : 0.0f;
        else {
            float rf = (float)r;
            float d = (float)(uint16_t)DistanceInt(px, py, cx, cy);
            lift = (float)(((double)rf - d) / rf);
        }
        double t = lift < D(0x5c86b8) ? BitsD(0x3fb999999999999aull) : (double)lift;
        color = 0xffcf1f00;
        zOff = (float)-t;
        if (lift >= D(0x5caa68)) {
            text[0] = 'X';
            text[1] = 0;
        } else if (lift > Fl_205(0x5c8598)) {
            text[0] = 'o';
            text[1] = 0;
        } else {
            color = 0xffcccccc;
            x = (float)(x + D(0x5cab68));
            text[0] = '.';
            text[1] = 0;
            y = (float)(y - D(0x5cab60));
        }
        fade = Fade_Step_Impl();
        break;
    }
    }
    float fogScale, fogStrength;
    uint32_t fogFont;
    Fog_ParamsAt((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, cx * 0x5c, cy * 0x5c, zOff, &fogScale, &fogFont, &fogStrength);
    Fade_Save();
    if (F<int32_t>(g_windowManager, 0xc64) <= 0 && (y < D(0x5cab28) || y == D(0x5cab28)))
        fade = BitsF(0x3a9d4952u);
    else if (F<float>(g_62be68, 0x14) >= D(0x5c8da8))
        fade = (float)(fade - D(0x5c85b8));
    Fade_Set(FBits(fade));
    float ty = (float)(y - D(0x5c86b8));
    float tx = (float)(x - D(0x5c88c8));
    Font_DrawTextBox((uint8_t*)(uintptr_t)fogFont, 0, text, tx, ty, BitsF(0x3f19999au), BitsF(0x3e4ccccdu), color, 0, 0, (uint32_t)flags,
                     fogScale);
    Fade_Restore();
}
RT3_REPLACE(0x452100, Map_DrawCellLabel);

// 0x452ca0. Sets the fog mode (+0x2171): range 0x40 (none / > 6), 4 (1..5) or 1 (6),
// with the map size (0x62c120 +0x2155/+0x2159). Mode 1 (+0x219d) counts as an
// extra engine user (+0x114272 / +0x114266) while +0x21a1 is set.
void __fastcall Map_SetFogMode(uint8_t* m, int, int32_t mode) {
    if (mode == 1 && F<int32_t>(m, 0x219d) != 1) {
        int32_t active = F<int32_t>(m, 0x21a1);
        F<int32_t>(m, 0x219d) = 1;
        if (active) {
            F<int32_t>(g_engine, 0x114272)++;
            F<int32_t>(g_engine, 0x114266)++;
        }
    }
    F<int32_t>(m, 0x2171) = mode;
    int32_t v = 0x40;
    if (mode > 0) {
        if (mode <= 5) v = 4;
        else if (mode == 6) v = 1;
    }
    uint8_t* map = g_map;
    Render_SetFog(m, 0, v / 2, v / 2, F<uint32_t>(map, 0x2155), F<uint32_t>(map, 0x2159), v, v, 9999);
}
RT3_REPLACE(0x452ca0, Map_SetFogMode);

// 0x452d30. Clears the fog mode (undoing the mode-1 engine count).
RT3_NOINLINE void __fastcall Map_ClearFogMode(uint8_t* m) {
    if (F<int32_t>(m, 0x2171) == 1 && F<int32_t>(m, 0x219d)) {
        int32_t active = F<int32_t>(m, 0x21a1);
        F<int32_t>(m, 0x219d) = 0;
        if (active) {
            F<int32_t>(g_engine, 0x114272)--;
            F<int32_t>(g_engine, 0x114266)--;
        }
    }
    F<int32_t>(m, 0x2171) = 0;
    Render_DisableFog(m);
}
RT3_REPLACE(0x452d30, Map_ClearFogMode);

// 0x452d80. Sets the overview option +0x217d, redrawing while a mode is shown.
RT3_NOINLINE void __fastcall Overview_SetOption(uint8_t* t, int, int32_t v) {
    F<int32_t>(t, 0x217d) = v;
    if (F<int32_t>(t, 0x2175)) Overview_Redraw(t, 0, nullptr);
}
RT3_REPLACE(0x452d80, Overview_SetOption);

// 0x452db0. Sets the highlighted building (+0x2181) of the buildings mode (3):
// clears the old highlight and marks the new one, then redraws.
RT3_NOINLINE void __fastcall Overview_SetHighlight(uint8_t* t, int, int32_t key) {
    if (F<int32_t>(t, 0x2175) == 3 && F<int32_t>(t, 0x2181)) Table_RefreshAll(g_buildings);
    F<int32_t>(t, 0x2181) = key;
    if (F<int32_t>(t, 0x2175) == 3 && key) Buildings_RefreshHighlight(g_buildings, 0, key);
    if (F<int32_t>(t, 0x2175)) Overview_Redraw(t, 0, nullptr);
}
RT3_REPLACE(0x452db0, Overview_SetHighlight);

// 0x452e10. Updates every building: those whose industry handles `cargo` (all with
// -1) or that are active take the vtable +0xc update, the rest +0x10.
RT3_NOINLINE void __stdcall Buildings_UpdateFor(int32_t cargo) {
    typedef void(__thiscall * Fn)(void*);
    for (int32_t i = 0; i < Collection_Count(g_buildings); ++i) {
        Collection* c = g_buildings;
        uint8_t* b = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        void** vt = *(void***)b;
        bool active = cargo == -1;
        if (!active && cargo) {
            uint8_t* ind = ((uint8_t*(__thiscall*)(void*))vt[0x80 / 4])(b);
            active = Industry_HandlesCargo(ind, 0, cargo) != 0;
        }
        if (!active) active = Obj_CheckActive(b) != 0;
        ((Fn)vt[(active ? 0xc : 0x10) / 4])(b);
    }
}
RT3_REPLACE(0x452e10, Buildings_UpdateFor);

// 0x452ea0. Tracks whether the value at (*0x62be68)+0x20 exceeds 0x5ca1c0
// (+0x21a1); while +0x219d is set, each change adjusts two engine counters.
RT3_NOINLINE void __fastcall Obj_UpdateThreshold(uint8_t* o) {
    int32_t on = F<float>(g_62be68, 0x20) > *(const float*)0x5ca1c0 ? 1 : 0;
    if (on == F<int32_t>(o, 0x21a1)) return;
    F<int32_t>(o, 0x21a1) = on;
    if (!F<int32_t>(o, 0x219d)) return;
    int32_t d = on ? 1 : -1;
    F<int32_t>(g_engine, 0x114272) += d;
    F<int32_t>(g_engine, 0x114266) += d;
}
RT3_REPLACE(0x452ea0, Obj_UpdateThreshold);

// 0x452f20. Redraws the cell rect (x0, y0, x1, y1) of the overview.
RT3_NOINLINE void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    int32_t r[4] = { x0, y0, x1, y1 };
    Overview_Redraw(t, 0, r);
}
RT3_REPLACE(0x452f20, Overview_RedrawRect);

// 0x452f60. Sets the overview cargo (+0x2179), re-marking the buildings in mode 3.
RT3_NOINLINE void __fastcall Overview_SetCargo(uint8_t* t, int, int32_t cargo) {
    if (F<int32_t>(t, 0x2175) == 3) Buildings_UpdateFor(cargo);
    F<int32_t>(t, 0x2179) = cargo;
    if (F<int32_t>(t, 0x2175)) Overview_Redraw(t, 0, nullptr);
}
RT3_REPLACE(0x452f60, Overview_SetCargo);

// 0x452fa0. Switches the overview to `mode` (with cargo, options +0x217d,
// highlight +0x2181 and +0x2185..+0x2191): leaves the old mode (the fog mode 7
// and buildings mode 3 keep engine counters and the building marks), stores
// the settings, redraws the rect `rect` (null: all), re-marks the buildings
// and sets the fog style +0x2171 for the new mode.
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9) {
    Render_DestroyColourMap(t);
    int32_t old = F<int32_t>(t, 0x2175);
    if ((old == 6 && mode != 6) || (old == 3 && highlight && mode != 3)) Table_RefreshAll(g_buildings);
    if (mode == 7) {
        if (F<int32_t>(t, 0x2175) != 7) {
            ++Overview_Count(0x11425a);
            ++Overview_Count(0x114262);
            Overview_SetOverlay(t, 1);
        }
        if (F<int32_t>(t, 0x2175) == 3) {
            --Overview_Count(0x11425e);
            --Overview_Count(0x114262);
            Buildings_UpdateFor(-1);
        }
    } else {
        if (F<int32_t>(t, 0x2175) == 7) {
            --Overview_Count(0x11425a);
            --Overview_Count(0x114262);
            Overview_SetOverlay(t, 0);
        }
        if (mode == 3) {
            if (F<int32_t>(t, 0x2175) == 3) {
                Buildings_UpdateFor(cargo);
                F<int32_t>(t, 0x2179) = cargo;
                if (F<int32_t>(t, 0x2175)) Overview_Redraw(t, 0, nullptr);
            } else {
                ++Overview_Count(0x11425e);
                ++Overview_Count(0x114262);
                Buildings_UpdateFor(cargo);
                Overview_SetOverlay(t, 1);
            }
        } else if (F<int32_t>(t, 0x2175) == 3) {
            --Overview_Count(0x11425e);
            --Overview_Count(0x114262);
            Buildings_UpdateFor(-1);
            Overview_SetOverlay(t, 0);
        }
    }
    const uint32_t* modes = (const uint32_t*)0x5ee5e8;
    if (modes[F<int32_t>(t, 0x2175) * 3]) --Overview_Count(0x11426e);
    if (modes[mode * 3]) ++Overview_Count(0x11426e);
    F<int32_t>(t, 0x217d) = opt;
    F<int32_t>(t, 0x2185) = a6;
    F<int32_t>(t, 0x2189) = a7;
    F<int32_t>(t, 0x2181) = highlight;
    F<int32_t>(t, 0x2191) = a9;
    F<int32_t>(t, 0x2175) = mode;
    F<int32_t>(t, 0x2179) = cargo;
    F<int32_t>(t, 0x218d) = a8;
    Overview_Redraw(t, 0, rect);
    if (mode == 6) Buildings_RefreshHighlight(g_buildings, 0, F<int32_t>(t, 0x2179));
    if (F<int32_t>(t, 0x2175) == 3 && F<int32_t>(t, 0x2181)) Buildings_RefreshHighlight(g_buildings, 0, F<int32_t>(t, 0x2181));
    if (F<int32_t>(g_app, 0x12c)) {
        if (mode < 2 || mode > 5) {
            Map_ClearFogMode(t);
            return;
        }
        F<int32_t>(t, 0x2171) = mode;
    } else {
        if (mode != 3) {
            Map_ClearFogMode(t);
            return;
        }
        Overview_SetOverlay(t, 1);
        F<int32_t>(t, 0x2171) = 1;
    }
    uint8_t* map = g_map;
    Render_SetFog(t, 0, 2, 2, F<uint32_t>(map, 0x2155), F<uint32_t>(map, 0x2159), 4, 4, 0x270f);
}
RT3_REPLACE(0x452fa0, Overview_SetMode);

// 0x453370. First time only (+0x21c5): shows the plain overview mode on the
// map when it has none.
RT3_NOINLINE void __fastcall Overview_ShowPlainOnce(uint8_t* t) {
    if (t[0x21c5]) return;
    t[0x21c5] = 1;
    uint8_t* m = g_map;
    if (F<int32_t>(m, 0x2175)) return;
    Overview_ShowPlain(m);
}
RT3_REPLACE(0x453370, Overview_ShowPlainOnce);

// 0x453510. Turns the overview off: leaves the fog / buildings mode, drops the
// fog and colour map, and (after the first showing, +0x21c5) switches the map
// to the plain mode.
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t) {
    if (F<int32_t>(t, 0x2175) == 7) {
        --Overview_Count(0x11425a);
        --Overview_Count(0x114262);
        Overview_SetOverlay(t, 0);
    } else if (F<int32_t>(t, 0x2175) == 3) {
        --Overview_Count(0x11425e);
        --Overview_Count(0x114262);
        Buildings_UpdateFor(-1);
        Overview_SetOverlay(t, 0);
    }
    if (((const uint32_t*)0x5ee5e8)[F<int32_t>(t, 0x2175) * 3]) --Overview_Count(0x11426e);
    if (F<int32_t>(t, 0x2171) == 1) Overview_SetOverlay(t, 0);
    F<int32_t>(t, 0x2171) = 0;
    Render_DisableFog(t);
    Render_Disable159b(t);
    Render_DestroyColourMap(t);
    uint8_t shown = t[0x21c5];
    F<int32_t>(t, 0x2175) = 0;
    if (!shown) return;
    Overview_ShowPlain(g_map);
}
RT3_REPLACE(0x453510, Overview_Close);

// 0x453780. Ends the plain overview of t's first showing (+0x21c5), closing
// mode 0x15 on the map.
RT3_NOINLINE void __fastcall Overview_EndPlain(uint8_t* t) {
    if (!t[0x21c5]) return;
    t[0x21c5] = 0;
    if (F<int32_t>(g_map, 0x2175) == 0x15) Overview_Close(g_map);
}
RT3_REPLACE(0x453780, Overview_EndPlain);

// 0x4537b0 / 0x4537e0 / 0x453800 / 0x453840. The cell byte holds two base-7 digits.
RT3_NOINLINE int32_t __fastcall Map_CellHigh(uint8_t* m, int, int32_t x, int32_t y) {
    return (int32_t)(uint8_t)Terrain_CellByte(m, 0, x, y) / 7;
}
RT3_REPLACE(0x4537b0, Map_CellHigh);

RT3_NOINLINE uint32_t __fastcall Map_CellLow(uint8_t* m, int, int32_t x, int32_t y) {
    int32_t v = (uint8_t)Terrain_CellByte(m, 0, x, y);
    return (uint32_t)((v / 7) & 0xffffff00) | (uint32_t)(v % 7);
}
RT3_REPLACE(0x4537e0, Map_CellLow);

RT3_NOINLINE void __fastcall Map_SetCellHigh(uint8_t* m, int, int32_t x, int32_t y, uint8_t hi) {
    int32_t v = (uint8_t)Terrain_CellByte(m, 0, x, y);
    Terrain_SetCellByte(m, 0, x, y, (uint8_t)(v % 7 + (uint8_t)(hi * 7)));
}
RT3_REPLACE(0x453800, Map_SetCellHigh);

RT3_NOINLINE void __fastcall Map_SetCellLow(uint8_t* m, int, int32_t x, int32_t y, uint8_t lo) {
    int32_t v = (uint8_t)Terrain_CellByte(m, 0, x, y);
    Terrain_SetCellByte(m, 0, x, y, (uint8_t)((uint8_t)(v / 7) * 7 + lo));
}
RT3_REPLACE(0x453840, Map_SetCellLow);

// 0x453890. Frees the four buffers 0x6acd20..0x6acd2c.
RT3_NOINLINE void FreeBuffers6acd20() {
    for (void** p = (void**)0x6acd20; p < (void**)0x6acd30; ++p) {
        if (*p) crt::free(*p);
        *p = nullptr;
    }
}
RT3_REPLACE(0x453890, FreeBuffers6acd20);

// 0x4538c0 (map query). Tree spot `spot` (byte) of cell (x, y) in block
// `blk`: unless hidden (0x62c258 by +0x21fd, or 0x675a58 with the engine's
// +0x11425a) its position from the spot table (0x6acd20 by 0x688058) into
// *outX / *outY and, with outV, the cell's value (0x688120 index, +0x1695)
// and its flags byte. Returns 1, 0 when hidden.
int32_t __fastcall Map_TreeSpot(uint8_t* m, int, int32_t x, int32_t y, int32_t blk, uint8_t spot, uint32_t* outX, uint32_t* outY, uint32_t* outV, uint8_t* outFlags) {
    const int32_t cell = (y * 8 | x) & 0x3f;
    if (const int32_t season = F<int32_t>(m, 0x21fd))
        if (*(const uint8_t*)((((season * 0xc4 + spot - 0xc4) * 6) + blk) * 64 + cell + 0x62c258)) return 0;
    if (F<int32_t>(g_engine, 0x11425a) && *(const uint8_t*)(((spot * 6) + blk) * 64 + cell + 0x675a58)) return 0;
    const uint8_t* table = ((const uint8_t* const*)0x6acd20)[*(const uint8_t*)(0x688058 + spot)];
    const uint8_t* rec = table + ((blk << 4 | y) << 4 | x) * 9;
    *outX = F<uint32_t>((uint8_t*)rec, 0);
    *outY = F<uint32_t>((uint8_t*)rec, 4);
    if (outV) {
        const uint32_t k = *(const uint16_t*)(0x688120 + (((spot * 6) + blk) * 64 + cell) * 2);
        *outV = F<uint32_t>(m, k * 9 + 0x1695);
        *outFlags = rec[8];
    }
    return 1;
}
RT3_REPLACE(0x4538c0, Map_TreeSpot);

// 0x4539c0. Drops the map's markers once (flag +0x2169): frees the buffers
// 0x6acd20 and deletes the marker objects.
void __fastcall Map_DropMarkers(uint8_t* m) {
    if (!F<int32_t>(m, 0x2169)) return;
    F<int32_t>(m, 0x2169) = 0;
    FreeBuffers6acd20();
    Terrain_DeleteMarkers(m);
}
RT3_REPLACE(0x4539c0, Map_DropMarkers);

// 0x4539f0. Builds the vegetation tables (seed 0x7b, the old seed restored
// after; a loading step before and after). Each of the 28 climate rows
// (0x5ef3e0, 0xb0 bytes: weight total, then 20 species name / weight pairs
// from +0x10) gets its weight total. Then for every row, 6 latitudes and 64
// cells: a species by weight (SimRand), looked up by name in the 0x45-byte
// species records (0x5ee8e0, the first when absent); for each of its 7
// height bands a variant (HashRand by row / latitude / cell, weights
// 0x5ef250 or 0x5ef318 by the species' +8, only variants it has at +0x14),
// its model id (+0x1b) into 0x688120, four density flags into 0x62c258
// (+4 x 0x5ee8c0[variant] x level x 0x5c873c beat HashRand 100; the top
// level x 0x5c8878) and the species' +0xc == 0 into 0x675a58.
RT3_NOINLINE void Vegetation_BuildTables() {
    int32_t saved = GetSeed();
    SetSeed(0x7b);
    Loading_Pump(g_app, 0, 1, 0);
    for (uint8_t* row = (uint8_t*)0x5ef3e0; row < (uint8_t*)0x5f0720; row += 0xb0) {
        F<int32_t>(row, 0) = 0;
        for (int j = 0; j < 20; ++j) F<float>(row, 0) = (float)((double)F<float>(row, 0x14 + 8 * j) + F<float>(row, 0));
    }
    uint8_t* flags = (uint8_t*)0x62c258;
    for (int32_t r = 0; r < 28; ++r) {
        uint8_t* row = (uint8_t*)0x5ef3e0 + r * 0xb0;
        uint8_t* kept = (uint8_t*)0x675a58 + r * 0xa80;
        uint16_t* models = (uint16_t*)0x688120 + r * 0xa80;
        for (int32_t y = 0; y < 6; ++y) {
            int32_t keyA = r * 7 + y, keyB = r * 7 + y * 0x1b;
            for (int32_t x = 0; x < 0x40; ++x, keyA += 0x100, keyB += 0x29, ++kept, ++models) {
                float pick = (float)((double)SimRand(0, 0x2710) / D(0x5c8d18));
                if (!(pick < D(0x5caa68))) F<uint32_t>(&pick, 0) = 0x3f7fbe77;
                pick = (float)((double)pick * F<float>(row, 0));
                int32_t sp = 0;
                bool hit = false;
                for (int32_t j = 0; j < 20; ++j) {
                    pick = (float)((double)pick - F<float>(row, 0x14 + 8 * j));
                    if (pick < 0.0f || pick == 0.0f) {
                        sp = j;
                        hit = true;
                        break;
                    }
                }
                if (!hit || sp >= 20) sp = 0;
                const char* name = *(const char**)(0x5ef3f0 + (r * 0x16 + sp) * 8);
                int32_t idx = 0;
                {
                    int32_t i = 0;
                    uint8_t* p = (uint8_t*)0x5ee8e0;
                    for (; p < (uint8_t*)0x5ef24f; p += 0x45, ++i)
                        if (!crt::stricmp(name, *(const char**)p)) break;
                    idx = p < (uint8_t*)0x5ef24f ? i : 0;
                }
                uint8_t* rec = (uint8_t*)0x5ee8e0 + idx * 0x45;
                uint8_t* k8 = kept;
                uint16_t* m16 = models;
                for (int32_t band = 0; band < 7; ++band, k8 += 0x180, m16 += 0x180) {
                    float v = (float)((double)HashRand(keyA, 0, 0x2710) / D(0x5c8d18));
                    int32_t var = 0;
                    for (; var < 7; ++var) {
                        const float* w = F<int32_t>(rec, 8) ? (const float*)0x5ef318 : (const float*)0x5ef250;
                        v = (float)((double)v - w[band * 7 + var]);
                        if (v < 0.0f && rec[0x14 + var]) break;
                    }
                    if (var == 7)
                        for (var = 6; var >= 0 && !rec[0x14 + var]; --var) {}
                    int32_t k = 0;
                    uint8_t n = rec[0x14 + var];
                    if (n > 1) k = SimRand(0, n - 1);
                    *m16 = F<uint16_t>(rec, 0x1b + (k + var * 3) * 2);
                    for (int32_t d = 1; d < 5; ++d) {
                        float f = (float)((double)F<float>(rec, 4) * ((const float*)0x5ee8c0)[var]);
                        if (d == 4) f = (float)(f * D(0x5c8878));
                        float roll = (float)HashRand(keyB, 0, 100);
                        uint8_t on = (double)(float)d * f * Fl_207(0x5c873c) > roll ? 1 : 0;
                        flags[((((d - 1) * 28 + r) * 7 + band) * 6 + y) * 0x40 + x] = on;
                    }
                    *k8 = F<int32_t>(rec, 0xc) == 0 ? 1 : 0;
                }
            }
        }
    }
    SetSeed(saved);
    Loading_Pump(g_app, 0, 1, 0);
}
RT3_REPLACE(0x4539f0, Vegetation_BuildTables);

}  // namespace rt3
