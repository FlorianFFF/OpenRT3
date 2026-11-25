// Track.cpp - RT3.exe 0x48a000-0x494000.
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
#include "game/calendar.h"
#include "core/containers.h"
#include "core/ptrlist.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

   // 0x5c8700
static const float kTwoCells = 184.0f;

uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y);
uint32_t __fastcall Map_GridByte(void* m, int, float x, float y);
void __fastcall Company_CountTrack(uint8_t* c, int, uint8_t* t, int32_t add);
double __fastcall Obj_Float18(void* o);
void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
void __fastcall Nop48a780(void*);
void __fastcall Obj_FreeList98(uint8_t* o);
void __fastcall Obj_FreeColumns(uint8_t* o);
void __fastcall Camera_UpdateZoomFlags(uint8_t* o);
double __stdcall AngleOf(float x0, float y0, float x1, float y1);
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
void* __fastcall Node_Company(void* n);
void __fastcall World_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
void __fastcall Tile_UpdateBlocked(uint8_t* o);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

// The four body corners (Effect_GetVectors: front-left +0x14, front-right +0x20,
// rear-left +0x2c, rear-right +0x38), each {x, y, z}.
struct Corners { float a[3], b[3], c[3], d[3]; };

void __fastcall Vehicle_PushState(uint8_t* o, int, uint32_t state, uint8_t mode);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
int32_t __fastcall Routes_SplitAt(Collection* routes, int, uint32_t id);
int32_t __fastcall Pair_Contains(void* p, int, int32_t v);
void __fastcall Light_SetMode(uint8_t* o, int, uint8_t mode);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
void __fastcall Vehicle_RearMid(uint8_t* o, int, float* x, float* z);
void __fastcall Vehicle_FrontMid(uint8_t* o, int, float* x, float* z);
float __stdcall Route_GradeFactor(float x2, float z2, float x1, float h1, float z1);
int32_t __fastcall Routes_SameComponent(Collection* c, int, int32_t a, int32_t b);
uint32_t __stdcall HeatColor(float t);
void __fastcall Obj_SetRgbValue(uint8_t* o, int, uint32_t v);
void __fastcall Track_ColorSlope(uint8_t* o);
void __fastcall Track_ColorWear(uint8_t* o, int, float full);
void __fastcall Obj_SetArgb(uint8_t* o, int, uint8_t a, uint8_t r, uint8_t g, uint8_t b);
int32_t __fastcall Obj_Get21(void* o);
void __fastcall Obj_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add);
void __fastcall Trains_Clear1a3(Collection* t);
void __fastcall Track_UpdateColor(uint8_t* o);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
uint8_t* __fastcall NamedObj_Construct(uint8_t* o);

void __fastcall WorldModel_Init(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y,
                                int32_t freeStanding);

void __fastcall LerpPercent(int32_t pct, float* ox, float x0, float y0, float x1, float y1, float* oy);
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
int32_t __fastcall Grid_NearestObject(Collection* c, int, float x, float y, int32_t skip1, int32_t skip2, int32_t freeOnly);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void* __fastcall Engine_Get1C(void* e);
void* __fastcall Obj_Node206(void* o);
void* __fastcall Obj_Node20a(void* o);
int32_t __fastcall Effect_PointOnSide(uint8_t* e, int, int32_t forward, float x, float y);
int32_t IsMultiplayer();
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
float __fastcall Obj_MidAboveGround(uint8_t* o);
float __fastcall Vehicle_Tilt(uint8_t* o, int, int32_t absolute);
int32_t __stdcall CellBuffer_Get(int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
float __fastcall Obj_DifficultyBlend(void* o);
void __fastcall Group_MoveAllToHeight(uint8_t* g, int, float y);
double __fastcall Effect_Blend(uint8_t* e, int, float t);
int32_t __fastcall Map_RemoveTrackRef(uint8_t* o, int, uint8_t* t);
void __fastcall Grid_AddObject(uint8_t* g, int, uint8_t* t);
void __fastcall Obj_UpdateHeightLight(uint8_t* o);
int32_t __fastcall Node_IsRouteEnd(uint8_t* n);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall Obj_SetRgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void* __fastcall Obj_CityRef(void* o);
int32_t __fastcall Routes_Component(Collection* c, int, int32_t id);
int32_t __fastcall Camera_FlagA4(uint8_t* o, int, int32_t);
int32_t __fastcall Camera_FlagAc(uint8_t* o, int, int32_t);
int32_t __fastcall Camera_FlagA0(uint8_t* o, int, int32_t);
int32_t __fastcall Camera_FlagA8(uint8_t* o, int, int32_t);
int32_t __fastcall Track_DebugTooltip(uint8_t* t, int, char* dst, int32_t n, int32_t, int32_t, int32_t, int32_t);
uint32_t __stdcall HeightColor(float t);
void* __fastcall Train_Company(void* t);
int32_t __fastcall Train_OrderTarget(uint8_t* t, int, int32_t* dist, int32_t preferStop);
int32_t __fastcall Train_CurrentOrderNode(uint8_t* t);
void* __fastcall Obj_Train390(void* o);
void __fastcall Vehicle_PointAlong(uint8_t* o, int, float t, float* x, float* z);
int32_t __fastcall Collection_Count(Collection* c);
void __fastcall Nop53a580(void*, int, int32_t);
void __fastcall Collection_Save(Collection* c, int, Stream s, int32_t recordSize);
double __fastcall Terrain_HeightAt(uint8_t* t, int, float x, float z);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall Routes_AddRef(Collection* t, int, uint32_t id, int32_t delta);
void __fastcall Routes_TryMergeAt(Collection* t, int, uint32_t id);
void __fastcall Track_Reverse(uint8_t* t);
float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
void __fastcall Group_Add(uint8_t* g, int, uint8_t* o, float lift);
void __fastcall Group_SetAngles(uint8_t* g, int, uint32_t a, uint32_t b);
void __fastcall Company_Add7680(void* c, int, int32_t d);
void __fastcall Vehicles_PushState(Collection* c, int, uint32_t id, uint8_t mode);
void __fastcall Effect_SetStyle(uint8_t* e, int, uint8_t kind, uint8_t on);
void __fastcall Track_BuildGeometry(uint8_t* e);
uint8_t __fastcall Track_SetState(uint8_t* t, int, uint8_t state, int32_t on, int32_t notify, int32_t spread);
void __fastcall Track_Resolve(uint8_t* t, int, int32_t on);
void __fastcall Track_Propagate(Collection* nodes, int, uint8_t* t, uint8_t state, int32_t on, uint8_t old, int32_t notify);
void Track_Reshape();
void __fastcall Track_Refresh(uint8_t* t);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
void __fastcall Route_UpdateFlag(uint8_t* r);
int32_t __fastcall Track_Cost2(uint8_t* t, int, uint32_t company);
void __fastcall Tracks_ApplyUpgrades(Collection* c, int, uint32_t company, int32_t amount);
int32_t __fastcall Track_Unlink(uint8_t* t, int, int32_t id);
int32_t __fastcall Route_ReplaceEnd(uint8_t* r, int, int32_t node, int32_t old);
void __fastcall NamedObj_Destructor(uint8_t* o);
void __fastcall SceneObj_Teardown(uint8_t* o);
uint32_t __fastcall Track_MarkSpecial(uint8_t* t, int, int32_t v);
uint32_t __fastcall Track_MarkSpecial13(uint8_t* t, int, int32_t v);
void __fastcall Track_UnmarkCells(uint8_t* t, int, int32_t remark);
int32_t __fastcall Track_AfterLoad(uint8_t* e, int, int32_t);
int32_t __fastcall Obj_CloseCurrent(void* o, int, int32_t a);
void __fastcall Map_SetCoarseByte(void* m, int, int32_t x, int32_t y, uint8_t v);
int32_t __fastcall StreamSkip4(int32_t s, int tag, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, int32_t s, int32_t recordSize);
uint8_t* __fastcall TrackLight_Construct(uint8_t* o);
void __fastcall Effect489e80_Release(uint8_t* e);
void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b);
void __fastcall Track_MakeEnd(uint8_t* t, int, int32_t flip);
void __fastcall Track_PairWith(uint8_t* t, int, int32_t id, int32_t mutual);

void __fastcall Track_SetMode(uint8_t* t, int, int32_t kind, const char* model, uint32_t a, uint32_t b, float y, int32_t, int32_t v219, int32_t v251,
                              int32_t bit5, int32_t keep);

void __fastcall Obj_FreeList244(uint8_t* o);
void __fastcall Company_PostStat(void* c, int, int32_t stat, float amount, int32_t withCash);
void __fastcall Effect_Destroy(uint8_t* e, int, int32_t really);
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side);
RT3_NOINLINE float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
int32_t __fastcall TrackNet_Load(Collection* net, int, int32_t s, int32_t recordSize);
RT3_NOINLINE void __fastcall Track_UpdateColor(uint8_t* o);
void __fastcall PtrList_Clear(PtrList* l);
void __fastcall PtrList_Destroy(PtrList* l);
void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share);
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall SceneObj_Teardown(uint8_t* o);
RT3_NOINLINE float __fastcall Obj_MidAboveGround(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Track_Link(uint8_t* t, int, int32_t second);
RT3_NOINLINE int32_t __fastcall Node_HasLink(void* n, int, int32_t any);
RT3_NOINLINE int32_t __fastcall Node_HasFreeLink(void* n, int, int32_t any);
RT3_NOINLINE int32_t __fastcall Track_HasLink(uint8_t* t, int, uint8_t end);
RT3_NOINLINE void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
RT3_NOINLINE float __fastcall Obj_CenterCellX(void* o);
RT3_NOINLINE float __fastcall Obj_CenterCellY(void* o);
void __fastcall Group_ReplaceModel(uint8_t* g, int, const char* model);
void __fastcall Track_SetProps(uint8_t* t, int, int32_t cls, uint8_t a, uint8_t b, uint8_t flag);
uint32_t __fastcall Track_MarkSpecial(uint8_t* t, int, int32_t v);
uint32_t __fastcall Track_MarkSpecial13(uint8_t* t, int, int32_t v);
void __fastcall Node_ResetKind(void* n);
int32_t __fastcall Track_Link2(uint8_t* t, int, int32_t second);
RT3_NOINLINE void __fastcall Vehicle_PointAlong(uint8_t* o, int, float t, float* x, float* z);
void __fastcall Obj_ClearFlag59_48a560(uint8_t* o);
void __fastcall Obj_SetFlag59_48a570(uint8_t* o);
int32_t __fastcall Obj_Flag59(uint8_t* o);
RT3_NOINLINE void __fastcall Track_AddWear(uint8_t* t);
RT3_NOINLINE void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b);
RT3_NOINLINE void __fastcall Node_SetLinkPair(void* n, int, int32_t a, int32_t b, int32_t first);
int32_t __fastcall Node_GetLink(void* n, int, int32_t first);
int32_t __fastcall Track_Field238(uint8_t* t, int, int32_t flag);
RT3_NOINLINE void __fastcall Container_Save(uint8_t* o, int, int32_t s);
void __fastcall Nop48a780(void*);
RT3_NOINLINE float __fastcall Vehicle_Tilt(uint8_t* o, int, int32_t absolute);
RT3_NOINLINE void __fastcall Track_ColorSlope(uint8_t* o);
int32_t __fastcall Effect_PointOnSide(uint8_t* e, int, int32_t forward, float x, float y);
void __fastcall ObjList_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b);
RT3_NOINLINE uint32_t __fastcall Obj_CenterGridByte(void* o);
RT3_NOINLINE uint32_t __fastcall Obj_CenterRegionType(void* o);
float __fastcall Obj_Mid1ed(uint8_t* o);
int32_t __fastcall Vehicle_CornerBelowGround(uint8_t* o, int, float off);
RT3_NOINLINE void __fastcall Node_Set222(void* n, int, int32_t v);
void __fastcall Nop48abd0(void*);
int32_t __fastcall Track_IsKind2(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Track_BuildCost(uint8_t* t, int, uint32_t company, int32_t inner);
int32_t __fastcall Track_Cost(uint8_t* t, int, uint32_t company);
int32_t __fastcall Track_Cost2(uint8_t* t, int, uint32_t company);
void __fastcall Track_MatchNeighbourHeights(uint8_t* t);
void __fastcall Node_UpdateLightFromNeighbors(uint8_t* n);
RT3_NOINLINE int32_t Option2bfActive();
RT3_NOINLINE void __fastcall Track_UpdateColor(uint8_t* o);
RT3_NOINLINE void __fastcall Track_SetKind(uint8_t* o, int, uint8_t kind);
RT3_NOINLINE int32_t __fastcall Node_IsRouteEnd(uint8_t* n);
void __fastcall Track_Refresh(uint8_t* t);
void __fastcall Vehicle_FrontMid(uint8_t* o, int, float* x, float* z);
void __fastcall Vehicle_RearMid(uint8_t* o, int, float* x, float* z);
double __fastcall Vehicle_Length(uint8_t* o);
float __fastcall Vehicle_Heading(uint8_t* o, int, int32_t forward);
RT3_NOINLINE float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
float __fastcall Vehicle_Pitch(uint8_t* o, int, int32_t forward);
void __fastcall Track_ClearProps(uint8_t* t);
RT3_NOINLINE void __fastcall Track_MarkCells(uint8_t* t);
RT3_NOINLINE void __fastcall Track_UnmarkCells(uint8_t* t, int, int32_t remark);
RT3_NOINLINE uint8_t* __fastcall Node_NextTowards(uint8_t* n, int, int32_t target, int32_t car, int32_t prev, uint8_t* train, int32_t anyTrack);
int32_t __fastcall Track_FindOverlap(uint8_t* t, int, int32_t* hit);
RT3_NOINLINE int32_t __fastcall Track_InfoText(uint8_t* t, int, char* dst, int32_t n, uint32_t* color, float* offset, char* dst2, int32_t extra);
void* __fastcall Node_Company(void* n);
RT3_NOINLINE int32_t __fastcall Track_LoadModels(uint8_t* t, int, int32_t s);
void __fastcall Track_ClearModels(uint8_t* t, int, int32_t keepSelected);
void __fastcall Track_ColorWear(uint8_t* o, int, float full);
int32_t __fastcall Track_LinksWithin(uint8_t* n, int, int32_t target, int32_t depth, int32_t from);
RT3_NOINLINE void* __fastcall Obj_Node206(void* o);
RT3_NOINLINE void* __fastcall Obj_Node20a(void* o);
void* __fastcall Track_Node20e(uint8_t* t);
int32_t __fastcall Track_EndLinksBack(uint8_t* t);
int32_t __fastcall Track_EndBackLink(uint8_t* t);
void __fastcall Track_SetHighlight(uint8_t* t, int, int32_t on);
RT3_NOINLINE void __fastcall Track_SetBlocked(uint8_t* o, int, uint8_t v);
RT3_NOINLINE void __fastcall Track_SetOwner(uint8_t* t, int, uint16_t owner);
RT3_NOINLINE int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id);
void __fastcall Tile_UpdateBlocked(uint8_t* o);
void __fastcall Track_DropAndSettle(uint8_t* t, int, int32_t, int32_t neighbours);
RT3_NOINLINE void __fastcall Track_Reshape_Impl(uint8_t* t, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d, int32_t neighbours, int32_t rebuild, int32_t newPiece);
void Track_Reshape();
void __fastcall Track_SetFlatAndLift(uint8_t* t, int, uint32_t h, int32_t neighbours);
RT3_NOINLINE void __fastcall Track_PairWith(uint8_t* t, int, int32_t id, int32_t mutual);
void __fastcall Track_Reverse(uint8_t* t);
RT3_NOINLINE void __fastcall Track_MakeEnd(uint8_t* t, int, int32_t flip);
RT3_NOINLINE int32_t __fastcall Track_DropModel(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Node_Advance(uint8_t* n, int, float pos, float dist, float* x, float* z, float* outPos, int32_t* dir, int32_t* count, float* accum, int32_t car, int32_t, int32_t front);
uint32_t __fastcall Tile_IsBlocked(uint8_t* o);
RT3_NOINLINE uint8_t __fastcall Track_SetState(uint8_t* t, int, uint8_t state, int32_t on, int32_t notify, int32_t spread);
void __fastcall Track_MoveEnds(uint8_t* t, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d, int32_t unused, int32_t force);
RT3_NOINLINE int32_t __fastcall Track_Connect(uint8_t* t, int, int32_t id);
RT3_NOINLINE void __fastcall Track_Resolve(uint8_t* t, int, int32_t on);
RT3_NOINLINE int32_t __fastcall Track_Unlink(uint8_t* t, int, int32_t id);
RT3_NOINLINE void __fastcall Track_SetMode(uint8_t* t, int, int32_t kind, const char* model, uint32_t a, uint32_t b, float y, int32_t, int32_t v219, int32_t v251, int32_t bit5, int32_t keep);
void __fastcall Track_InitRotated(uint8_t* t, int, int32_t id, int32_t group, float cx, float cy, float angle, int32_t kind, int32_t owner, int32_t style);
RT3_NOINLINE void __fastcall Track_Remove(uint8_t* t);
void __fastcall Track_PlaceRotated(uint8_t* t, int, float cx, float cy, float angle);
RT3_NOINLINE int32_t __fastcall Track_ConnectAndResolve(uint8_t* t, int, int32_t id);
RT3_NOINLINE void __fastcall Track_AddCatenary(uint8_t* t, int, int32_t side);
RT3_NOINLINE void __fastcall Track_PopUndo(uint8_t* t);
RT3_NOINLINE void __fastcall Track_Electrify(uint8_t* t, int, int32_t catenary, int32_t notify, int32_t side);
RT3_NOINLINE void __fastcall Obj_DecA0(void* o);
void __fastcall Obj_DecA4(uint8_t* o);
void __fastcall Obj_DecAc(uint8_t* o);
RT3_NOINLINE float __stdcall Route_GradeFactor(float x2, float z2, float x1, float h1, float z1);
RT3_NOINLINE Collection* __fastcall Tracks_Construct(Collection* c, int, int32_t columns, int32_t);
RT3_NOINLINE void __fastcall Tracks_RebuildGeometry(Collection* c);
RT3_NOINLINE void __fastcall ContainerList_Save(Collection* c, int, Stream s, int32_t recordSize);
void __fastcall Vehicles_PushState(Collection* c, int, uint32_t id, uint8_t mode);
void __fastcall Table_FillCursor(Collection* t, int, uint8_t* out);
RT3_NOINLINE int32_t __fastcall TrackNet_LinkPair(Collection* net, int, uint32_t a, uint32_t b, int32_t na, int32_t nb);
RT3_NOINLINE void __fastcall Tracks_MoveEnds(Collection* c, int, uint32_t id, const uint32_t* a, const uint32_t* b, const uint32_t* d, const uint32_t* e);
void __fastcall Grid_AddObject(uint8_t* g, int, uint8_t* t);
RT3_NOINLINE int32_t __fastcall Map_RemoveTrackRef(uint8_t* o, int, uint8_t* t);
void __fastcall Obj_FreeColumns(uint8_t* o);
void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y);
int32_t __fastcall Grid_NearestNode(Collection* g, int, float px, float py, int32_t cx, int32_t cy, float* second, int32_t exclude, int32_t byDegree, float* best, int32_t skipLinked);
void __fastcall Nodes_RemarkAround(Collection* nodes, int, int32_t fx, int32_t fy);
int32_t __fastcall Grid_NearestObject(Collection* c, int, float x, float y, int32_t skip1, int32_t skip2, int32_t freeOnly);
int32_t __stdcall Track_FindClearance(uint8_t* p, int32_t from, float* height);
RT3_NOINLINE void __fastcall TrackNet_GradeRun(Collection* net, int, uint8_t* t, int32_t from, float rise, uint8_t force);
int32_t __fastcall Node_TurnsLeft(Collection* nodes, int, uint32_t id);
void __fastcall Tracks_ColorSlope(Collection* c);
void __fastcall Objects_HighlightGroup(Collection* t, int, int32_t group);
void __fastcall Tracks_ColorWear(Collection* c);
RT3_NOINLINE void __fastcall Tracks_UpdateColor(Collection* c);
void __fastcall Table_DecayWear(Collection* t);
void __fastcall Track_Validate(Collection* t);
RT3_NOINLINE void __fastcall Camera_BeginA0(uint8_t* o);
void __fastcall Camera_BeginA4(uint8_t* o);
void __fastcall Camera_BeginAc(uint8_t* o);
void __fastcall Camera_UpdateZoomFlags(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Camera_FlagA0(uint8_t* o, int, int32_t);
RT3_NOINLINE int32_t __fastcall Camera_FlagA4(uint8_t* o, int, int32_t);
RT3_NOINLINE int32_t __fastcall Camera_FlagA8(uint8_t* o, int, int32_t);
RT3_NOINLINE int32_t __fastcall Camera_FlagAc(uint8_t* o, int, int32_t);
int32_t __fastcall Map_TrackOwnerAt(void* o, int, int32_t x, int32_t y, int32_t* owner, int32_t* flag);
RT3_NOINLINE float __stdcall Route_GradeFactorHere(float x, float z);
RT3_NOINLINE void __fastcall Obj_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add);
RT3_NOINLINE int32_t __fastcall Table_CountFlag201_40(Collection* t);
RT3_NOINLINE void __fastcall Tracks_ClearHighlights(Collection* c);
RT3_NOINLINE void __fastcall Tracks_HighlightRun(Collection* c, int, uint32_t id, int32_t on, int32_t follow);
RT3_NOINLINE void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share);
void __fastcall Tracks_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall TrackNet_Load(Collection* net, int, int32_t s, int32_t recordSize);
RT3_NOINLINE void __fastcall Route_UpdateFlag(uint8_t* r);
RT3_NOINLINE int32_t __fastcall Route_ReplaceEnd(uint8_t* r, int, int32_t node, int32_t old);
RT3_NOINLINE void __fastcall Track_Propagate(Collection* nodes, int, uint8_t* t, uint8_t state, int32_t on, uint8_t old, int32_t notify);
RT3_NOINLINE void __fastcall TrackNet_IdleModels(Collection* net, int, uint8_t* t);
RT3_NOINLINE void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side);
RT3_NOINLINE void __fastcall Routes_TryMergeAt(Collection* t, int, uint32_t id);
RT3_NOINLINE int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
RT3_NOINLINE void __fastcall Trains_DropWaypoint(Collection* trains, int, int32_t node);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE void __fastcall SceneObj_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b, int32_t deep);
RT3_NOINLINE void __fastcall Group_Add(uint8_t* g, int, uint8_t* o, float lift);
void __fastcall Track_BuildGeometry(uint8_t* e);
RT3_NOINLINE int32_t __fastcall Track_DebugTooltip(uint8_t* t, int, char* dst, int32_t n, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall Group_SetObject(uint8_t* g, int, uint8_t* o);
void __fastcall Track_SetEnds(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d);
void __fastcall Track_DropOnTerrain(uint8_t* e, int, float lift);
void __fastcall Track_SetFlatHeight(uint8_t* e, int, uint32_t h);

static inline void* RefNotMinus1(Collection* t, int32_t id) { return id != -1 ? Collection_Get(t, 0, id) : nullptr; }

// 0x48e1a0 / 0x48e220. Whether / through which link one of this piece's two
// end nodes leads back to it (+0x202); the node's flag +0x201 bit 1 says which side.
static uint8_t* EndNode(uint8_t* t, int end) {
    int32_t id = end ? F<int32_t>(t, 0x20a) : F<int32_t>(t, 0x206);
    return id != -1 ? (uint8_t*)Collection_Get(g_tracks, 0, id) : nullptr;
}

// 0x4932a0 / 0x493300. Starts (counter +0xa0 / +0xa4) a camera action; the
// first one records whether the camera height is below 2000. Always stores
// the camera value +0x18.
static void CameraBegin(uint8_t* o, uint32_t counter) {
    uint8_t* cam = g_62be68;
    if (!F<int32_t>(o, 0xa0) && !F<int32_t>(o, 0xa4))
        F<int32_t>(o, 0xb0) = F<float>(cam, 0x66) < *(const float*)0x5c88e0 ? 1 : 0;
    F<float>(o, 0xb8) = (float)Obj_Float18(hook::As<void*>(g_62be68));
    F<int32_t>(o, counter)++;
}

// 0x4934a0..0x493560. Zoom flag of a camera action (+0xb0 for +0xa0/+0xa4,
// +0xb4 for +0xa8/+0xac), refreshed when the camera value moved; 0 if inactive.
static int32_t CameraFlag(uint8_t* o, uint32_t active, uint32_t flag) {
    if (!F<int32_t>(o, active)) return 0;
    if (!(Obj_Float18(hook::As<void*>(g_62be68)) == F<float>(o, 0xb8))) Camera_UpdateZoomFlags(o);
    return F<int32_t>(o, flag);
}

static inline Collection* Coll(uint32_t a) { return hook::Global<Collection*>(a); }

static inline uint8_t* Map() { return g_map; }

static inline void GetCorners(uint8_t* o, Corners& k) {
    Effect_GetVectors(o, 0, (uint32_t*)k.a, (uint32_t*)k.b, (uint32_t*)k.c, (uint32_t*)k.d);
}


// Wraps an angle into [0, 2 pi) (0x5c96b8) in float steps.
static inline float WrapAngle(float a) {
    const double twoPi = *(const double*)0x5c96b8, zero = *(const double*)0x5c8608;
    while ((double)a >= twoPi) a = (float)((double)a - twoPi);
    while ((double)a < zero) a = (float)((double)a + twoPi);
    return a;
}


static inline Collection* Routes() { return g_routes; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl_204(uint32_t a) { return *(const float*)a; }

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

// Midpoint of track piece t's end points in world units ((a + b) / 184 x 92).
static inline void Track_Mid(uint8_t* t, float* x, float* y) {
    float mx = (float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl_205(0x5cfcfc));
    float my = (float)(((double)F<float>(t, 0x34) + F<float>(t, 0x1c)) / Fl_205(0x5cfcfc));
    *x = (float)((double)mx * Fl_205(0x5c8700));
    *y = (float)((double)my * Fl_205(0x5c8700));
}

// Whether `of`'s neighbours (+0x206, else +0x20a) lie on another side of `judge`
// than `of`'s own midpoint (side code `side`).
static bool Track_NeighbourCrosses(uint8_t* judge, uint8_t* of, int32_t side, bool firstByMid) {
    if (firstByMid) {
        if (F<int32_t>(of, 0x206) != -1 && Collection_Get(g_tracks, 0, F<int32_t>(of, 0x206))) {
            float x, y;
            Track_Mid((uint8_t*)Obj_Node206(of), &x, &y);
            if (Effect_PointOnSide(judge, 0, 1, x, y) != side) return true;
        }
    } else if (Obj_Node206(of)) {
        float x, y;
        Obj_CenterCell(Obj_Node206(of), 0, &x, &y);
        x = (float)((double)x * Fl_205(0x5c8700));
        y = (float)((double)y * Fl_205(0x5c8700));
        if (Effect_PointOnSide(judge, 0, 1, x, y) != side) return true;
    }
    if (!Obj_Node20a(of)) return false;
    float x, y;
    Obj_CenterCell(Obj_Node20a(of), 0, &x, &y);
    x = (float)((double)x * Fl_205(0x5c8700));
    y = (float)((double)y * Fl_205(0x5c8700));
    return Effect_PointOnSide(judge, 0, 1, x, y) != side;
}


// Lifts track piece t's scenery object (+0x248) to the mean corner height less one.
static void Track_LiftScenery(uint8_t* t) {
    uint8_t* o = F<uint8_t*>(t, 0x248);
    if (!o) return;
    uint32_t x, y, z;
    SceneObj_GetPos(o, 0, &x, &y, &z);
    float ny = (float)((((((double)F<float>(t, 0x3c) + F<float>(t, 0x30)) + F<float>(t, 0x24)) + F<float>(t, 0x18)) /
                        D(0x5c87b8)) - D(0x5c8770));
    Obj_MoveTo(F<uint8_t*>(t, 0x248), 0, *(float*)&x, ny, *(float*)&z);
}

// Settles a dead-end piece's attached scenery (+8) to the open end's height.
static void Track_SettleGroup(uint8_t* t) {
    if (!F<void*>(t, 8) || F<int32_t>(t, 0x226)) return;
    float end;
    if (F<int32_t>(t, 0x206) == -1) end = 0.0f;
    else if (F<int32_t>(t, 0x20a) == -1) end = 1.0f;
    else return;
    Group_MoveAllToHeight(t, 0, (float)Effect_Blend(t, 0, end));
}

static inline bool Track_IsRunKind(uint32_t k) { return k == 2 || k == 1 || k == 3 || k == 0xc || k == 0xd; }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

// The state neighbour n shows towards t: its own, or for an open piece the
// side t is on (4 from +0x206, 1 from +0x20a, by +0x201 bit 1 from +0x20e).
static uint8_t Track_FacingState(uint8_t* n, uint8_t* t) {
    uint8_t s = n[0x218];
    if (s != 2) return s;
    if (F<int32_t>(n, 0x206) == F<int32_t>(t, 0x202)) return 4;
    if (F<int32_t>(n, 0x20a) == F<int32_t>(t, 0x202)) return 1;
    if (F<int32_t>(n, 0x20e) == F<int32_t>(t, 0x202)) return n[0x201] & 2 ? 4 : 1;
    return 2;
}

// Whether corners p and q coincide (x / z within 0.009).
static bool Track_CornersMeet(const float* p, const float* q) {
    return crt::fabs((double)p[0] - q[0]) < D(0x5cfdd8) && crt::fabs((double)p[2] - q[2]) < D(0x5cfdd8);
}

namespace {
// A track piece end vector (x, y, z), copied by bits like the original's MOVs.
union TrackVec {
    uint32_t u[3];
    float f[3];
};
inline void SwapVec(TrackVec& x, TrackVec& y) {
    TrackVec t = x;
    x = y;
    y = t;
}
}

static void ReshapeVecs(uint8_t* p, const TrackVec& a, const TrackVec& b, const TrackVec& c, const TrackVec& d) {
    ((void(__fastcall*)(uint8_t*, int, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*, int32_t, int32_t, int32_t))(void*)&Track_Reshape)(
        p, 0, a.u, b.u, c.u, d.u, 1, 0, 0);
}

// 0x48a020. Height of the object's middle (+0x1ed/+0x1f1) above the terrain at (+0xc, +0x10).
RT3_NOINLINE float __fastcall Obj_MidAboveGround(uint8_t* o) {
    float h = (float)Terrain_HeightAt(Map(), 0, F<float>(o, 0xc), F<float>(o, 0x10));
    return (float)(((double)F<float>(o, 0x1f1) + F<float>(o, 0x1ed)) / *(const double*)0x5c85d0 - h);
}
RT3_REPLACE(0x48a020, Obj_MidAboveGround);

// thunk: jmp 0x53a3a0
RT3_REPLACE(0x48a060, Obj_SetRgb);

// 0x48a070 / 0x48a130. Track piece end links (+0x206, +0x20a).
RT3_NOINLINE int32_t __fastcall Track_Link(uint8_t* t, int, int32_t second) { return second ? F<int32_t>(t, 0x20a) : F<int32_t>(t, 0x206); }
RT3_REPLACE(0x48a070, Track_Link);

// 0x48a090. Track node has a link: (+0x206 or +0x20a) set; with `any`, +0x20e counts too.
RT3_NOINLINE int32_t __fastcall Node_HasLink(void* n, int, int32_t any) {
    if (F<int32_t>(n, 0x206) != -1) return 1;
    if (F<int32_t>(n, 0x20a) != -1) return 1;
    if (any && F<int32_t>(n, 0x20e) != -1) return 1;
    return 0;
}
RT3_REPLACE(0x48a090, Node_HasLink);

// 0x48a0e0. Track node has a free link slot (+0x206/+0x20a; with `any`, +0x20e too).
RT3_NOINLINE int32_t __fastcall Node_HasFreeLink(void* n, int, int32_t any) {
    if (F<int32_t>(n, 0x206) == -1) return 1;
    if (F<int32_t>(n, 0x20a) == -1) return 1;
    if (any && F<int32_t>(n, 0x20e) == -1) return 1;
    return 0;
}
RT3_REPLACE(0x48a0e0, Node_HasFreeLink);

RT3_NOINLINE int32_t __fastcall Track_HasLink(uint8_t* t, int, uint8_t end) {
    if (end == 1) return F<int32_t>(t, 0x206) != -1 ? 1 : 0;
    if (end == 2) return F<int32_t>(t, 0x20a) != -1 ? 1 : 0;
    return 0;
}
RT3_REPLACE(0x48a130, Track_HasLink);

// 0x48a170. Centre of a two-corner object, in cells.
RT3_NOINLINE void __fastcall Obj_CenterCell(void* o, int, float* x, float* y) {
    *x = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / kTwoCells);
    *y = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / kTwoCells);
}
RT3_REPLACE(0x48a170, Obj_CenterCell);

// 0x48a1a0 / 0x48a1c0. Centre x / y in cells, rounded to float.
RT3_NOINLINE float __fastcall Obj_CenterCellX(void* o) { float r = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / kTwoCells); return r; }
RT3_REPLACE(0x48a1a0, Obj_CenterCellX);

RT3_NOINLINE float __fastcall Obj_CenterCellY(void* o) { float r = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / kTwoCells); return r; }
RT3_REPLACE(0x48a1c0, Obj_CenterCellY);

// 0x48a1e0. Replaces the group's model by a new infrastructure world model
// (vtable 0x5cfd00) of `model`: with several objects in the group, the first
// one's angles and position carry over to it after clearing the group (slot
// 0x18) and re-adding; with one, it just becomes the group's object.
void __fastcall Group_ReplaceModel(uint8_t* g, int, const char* model) {
    PtrList* l = F<PtrList*>(g, 8);
    if (!l) return;
    if (F<int32_t>(l, 0xc) > 1) {
        PtrList_Rewind(l);
        uint32_t a = 0, b = 0, c = 0, x, y, z;
        uint8_t* first = (uint8_t*)PtrList_Next(F<PtrList*>(g, 8));
        SceneObj_GetAngles(first, 0, &a, &b, &c);
        SceneObj_GetPos(first, 0, &x, &y, &z);
        ((void(__thiscall*)(uint8_t*, int32_t))(*(void***)g)[0x18 / 4])(g, 0);
        uint8_t* o = (uint8_t*)crt::operator_new(0x23a);
        if (o) {
            NamedObj_Construct(o);
            *(uint32_t*)o = 0x5cfd00;
        }
        WorldModel_Init(o, 0, (const char*)0x5c87a8, model, (const char*)0x5cfd74, 0xbf800000, 0xbf800000, 0);
        Group_Add(g, 0, o, 0.0f);
        SceneObj_SetAngles(o, 0, a, b, c);
        Obj_MoveTo(o, 0, *(float*)&x, *(float*)&y, *(float*)&z);
        return;
    }
    uint8_t* o = (uint8_t*)crt::operator_new(0x23a);
    if (o) {
        NamedObj_Construct(o);
        *(uint32_t*)o = 0x5cfd00;
    }
    WorldModel_Init(o, 0, (const char*)0x5c87a8, model, (const char*)0x5cfd74, 0xbf800000, 0xbf800000, 0);
    Group_SetObject(g, 0, o);
}
RT3_REPLACE(0x48a1e0, Group_ReplaceModel);

// 0x48a340. Sets class +0x226, bytes +0x219 / +0x251 and flag bit 5 of +0x24c.
void __fastcall Track_SetProps(uint8_t* t, int, int32_t cls, uint8_t a, uint8_t b, uint8_t flag) {
    F<int32_t>(t, 0x226) = cls;
    t[0x219] = a;
    t[0x251] = b;
    t[0x24c] ^= (uint8_t)(((flag << 5) ^ t[0x24c]) & 0x20);
}
RT3_REPLACE(0x48a340, Track_SetProps);

// 0x48a380. A fully linked piece without +0x24d becomes class 12 with +0x24d = v.
RT3_NOINLINE uint32_t __fastcall Track_MarkSpecial(uint8_t* t, int, int32_t v) {
    if (F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x20a) == -1) return 0xffffffff;
    if (F<int32_t>(t, 0x24d)) return F<uint32_t>(t, 0x24d);
    F<int32_t>(t, 0x226) = 0xc;
    F<int32_t>(t, 0x24d) = v;
    return (uint32_t)v;
}
RT3_REPLACE(0x48a380, Track_MarkSpecial);

// 0x48a3c0. As 0x48a380 with class 13.
RT3_NOINLINE uint32_t __fastcall Track_MarkSpecial13(uint8_t* t, int, int32_t v) {
    if (F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x20a) == -1) return 0xffffffff;
    if (F<int32_t>(t, 0x24d)) return F<uint32_t>(t, 0x24d);
    F<int32_t>(t, 0x226) = 0xd;
    F<int32_t>(t, 0x24d) = v;
    return (uint32_t)v;
}
RT3_REPLACE(0x48a3c0, Track_MarkSpecial13);

void __fastcall Node_ResetKind(void* n) {
    int32_t k = F<int32_t>(n, 0x226);
    if (k == 12 || k == 13) {
        F<int32_t>(n, 0x226) = 0;
        F<int32_t>(n, 0x24d) = 0;
    }
}
RT3_REPLACE(0x48a400, Node_ResetKind);

int32_t __fastcall Track_Link2(uint8_t* t, int, int32_t second) { return second ? F<int32_t>(t, 0x20a) : F<int32_t>(t, 0x206); }
RT3_REPLACE(0x48a420, Track_Link2);

// 0x48a440. Point at fraction t / 0x5c97e0 between the front and rear axle
// midpoints (x into *x, z into *z), in tile units.
RT3_NOINLINE void __fastcall Vehicle_PointAlong(uint8_t* o, int, float t, float* x, float* z) {
    Corners k;
    GetCorners(o, k);
    const float two = *(const float*)0x5c878c;
    const double K = *(const double*)0x5c8c48;
    float m0 = (float)(((double)k.b[0] + k.a[0]) / two);
    float m1 = (float)(((double)k.d[0] + k.c[0]) / two);
    double tt = (double)t / *(const double*)0x5c97e0;
    if (m1 > m0) *x = (float)((((double)m1 - m0) * tt + m0) / K);
    else *x = (float)(((double)m0 - ((double)m0 - m1) * tt) / K);
    float n0 = (float)(((double)k.b[2] + k.a[2]) / two);
    float n1 = (float)(((double)k.d[2] + k.c[2]) / two);
    if (n1 > n0) *z = (float)((((double)n1 - n0) * tt + n0) / K);
    else *z = (float)(((double)n0 - ((double)n0 - n1) * tt) / K);
}
RT3_REPLACE(0x48a440, Vehicle_PointAlong);

// 0x48a560 / 0x48a570 (vtable stubs). Clear / set flag 0x20 of +0x59.
void __fastcall Obj_ClearFlag59_48a560(uint8_t* o) { o[0x59] &= 0xdf; }
RT3_REPLACE(0x48a560, Obj_ClearFlag59_48a560);

void __fastcall Obj_SetFlag59_48a570(uint8_t* o) { o[0x59] |= 0x20; }
RT3_REPLACE(0x48a570, Obj_SetFlag59_48a570);

// 0x48a580 / 0x48abe0 (vtables). Flag 0x20 of +0x59; kind +0x226 is 2.
int32_t __fastcall Obj_Flag59(uint8_t* o) { return o[0x59] >> 5 & 1; }
RT3_REPLACE(0x48a580, Obj_Flag59);

// 0x48a590. Wear +0x22a grows by 0x5c8d48 for kind 4, else by 0x5c85b4.
RT3_NOINLINE void __fastcall Track_AddWear(uint8_t* t) {
    float inc = t[0x218] == 4 ? *(const float*)0x5c8d48 : *(const float*)0x5c85b4;
    F<float>(t, 0x22a) = (float)((double)F<float>(t, 0x22a) + inc);
}
RT3_REPLACE(0x48a590, Track_AddWear);

// 0x48a5c0. Clears the node's link pairs (+0x21a/+0x238 and +0x21e/+0x23c)
// that match (a, b); -1 matches anything.
RT3_NOINLINE void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b) {
    if ((a == -1 || F<int32_t>(n, 0x21a) == a) && (b == -1 || F<int32_t>(n, 0x238) == b)) {
        F<int32_t>(n, 0x21a) = -1;
        F<int32_t>(n, 0x238) = -1;
    }
    if ((a == -1 || F<int32_t>(n, 0x21e) == a) && (b == -1 || F<int32_t>(n, 0x23c) == b)) {
        F<int32_t>(n, 0x21e) = -1;
        F<int32_t>(n, 0x23c) = -1;
    }
}
RT3_REPLACE(0x48a5c0, Node_ClearLinks);

// 0x48a620 / 0x48a660. Link pair slot: the second (+0x21e/+0x23c) unless `first`
// or the node is one-sided (+0x218 == 1).
RT3_NOINLINE void __fastcall Node_SetLinkPair(void* n, int, int32_t a, int32_t b, int32_t first) {
    if (first == 0 && F<uint8_t>(n, 0x218) != 1) {
        F<int32_t>(n, 0x21e) = a;
        F<int32_t>(n, 0x23c) = b;
    } else {
        F<int32_t>(n, 0x21a) = a;
        F<int32_t>(n, 0x238) = b;
    }
}
RT3_REPLACE(0x48a620, Node_SetLinkPair);

int32_t __fastcall Node_GetLink(void* n, int, int32_t first) {
    return (first == 0 && F<uint8_t>(n, 0x218) != 1) ? F<int32_t>(n, 0x21e) : F<int32_t>(n, 0x21a);
}
RT3_REPLACE(0x48a660, Node_GetLink);

int32_t __fastcall Track_Field238(uint8_t* t, int, int32_t flag) {
    return flag || t[0x218] == 1 ? F<int32_t>(t, 0x238) : F<int32_t>(t, 0x23c);
}
RT3_REPLACE(0x48a690, Track_Field238);

// 0x48a6c0. Saves a container: the child count (+8 list, its word +0xc), the
// index of the selected child (+0x248; 0xff for none) and then each child via its
// save method (+0x44) - all to stream s.
RT3_NOINLINE void __fastcall Container_Save(uint8_t* o, int, int32_t s) {
    Nop53a580(o, 0, s);
    PtrList* l = F<PtrList*>(o, 8);
    uint32_t n = l ? *(uint16_t*)((uint8_t*)l + 0xc) : 0;
    StreamWrite(s, &n, 2, 0);
    uint32_t sel = (uint32_t)s;
    *(uint8_t*)&sel = 0xff;
    if (F<void*>(o, 0x248)) {
        uint8_t idx = 0;
        PtrList_Rewind(F<PtrList*>(o, 8));
        for (void* c = PtrList_Next(F<PtrList*>(o, 8)); c; c = PtrList_Next(F<PtrList*>(o, 8)), ++idx)
            if (c == F<void*>(o, 0x248)) {
                *(uint8_t*)&sel = idx;
                break;
            }
    }
    StreamWrite(s, &sel, 1, 0);
    if (!F<PtrList*>(o, 8)) return;
    PtrList_Rewind(F<PtrList*>(o, 8));
    for (void* c = PtrList_Next(F<PtrList*>(o, 8)); c; c = PtrList_Next(F<PtrList*>(o, 8)))
        ((void(__fastcall*)(void*, int, int32_t))(*(void***)c)[0x44 / 4])(c, 0, s);
}
RT3_REPLACE(0x48a6c0, Container_Save);

RT3_NOINLINE void __fastcall Nop48a780(void*) { __asm nop }
RT3_REPLACE(0x48a780, Nop48a780);

// 0x48a790. Height difference front-left minus rear-left, scaled (absolute when asked).
RT3_NOINLINE float __fastcall Vehicle_Tilt(uint8_t* o, int, int32_t absolute) {
    Corners k;
    GetCorners(o, k);
    float v = (float)((double)k.a[1] - k.c[1]);
    v = (float)((double)v * *(const double*)0x5cfd88);
    if (!absolute) return v;
    return (float)crt::fabs(v);
}
RT3_REPLACE(0x48a790, Vehicle_Tilt);

// 0x48a7f0. Colours a track piece by its slope (height difference of its ends).
RT3_NOINLINE void __fastcall Track_ColorSlope(uint8_t* o) {
    uint32_t a[3], b[3], c[3], d[3];
    Effect_GetVectors(o, 0, a, b, c, d);
    float h = (float)((double)*(float*)&a[1] - *(float*)&c[1]);
    h = (float)(h * D(0x5cfd88));
    h = (float)crt::fabs(h);
    float t = (float)(D(0x5c8770) - (double)h * Fl(0x5c8c28) / D(0x5c8610));
    Obj_SetRgbValue(o, 0, HeatColor(t) | 0xff000000);
}
RT3_REPLACE(0x48a7f0, Track_ColorSlope);

// 0x48a880. Whether (x, y) lies on the positive side of the line through the
// midpoints of the effect's edges (vectors a+b and c+d; reversed unless `forward`).
RT3_NOINLINE int32_t __fastcall Effect_PointOnSide(uint8_t* e, int, int32_t forward, float x, float y) {
    float a[3], b[3], c[3], d[3];
    Effect_GetVectors(e, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)c, (uint32_t*)d);
    const float k = *(const float*)0x5c878c;
    float m1x = (float)(((double)b[0] + a[0]) / k), m1z = (float)(((double)b[2] + a[2]) / k);
    float m2x = (float)(((double)d[0] + c[0]) / k), m2z = (float)(((double)d[2] + c[2]) / k);
    float ax, az, bx, bz;
    if (forward) { ax = m1x; az = m1z; bx = m2x; bz = m2z; }
    else { bx = m1x; bz = m1z; ax = m2x; az = m2z; }
    float dx = (float)((double)bx - ax);
    float dz = (float)((double)bz - az);
    const double zero = *(const double*)0x5c8608;
    if ((double)dx == zero) {
        if ((double)dz > *(const float*)0x5c8598) return x > ax ? 1 : 0;
        return x < ax ? 1 : 0;
    }
    float slope = (float)((double)dz / dx);
    float yl = (float)(((double)x - ax) * slope + az);
    if ((double)dx > zero) return y < yl ? 1 : 0;
    if ((double)dx < zero) return y > yl ? 1 : 0;
    return 0;
}
RT3_REPLACE(0x48a880, Effect_PointOnSide);

// 0x48a9e0. Tints every object in o's list (+8), with children.
void __fastcall ObjList_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b) {
    PtrList* l = F<PtrList*>(o, 8);
    if (!l) return;
    PtrList_Rewind(l);
    for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 8)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 8)))
        SceneObj_SetTint(c, 0, r, g, b, 1);
}
RT3_REPLACE(0x48a9e0, ObjList_SetTint);

// 0x48aa30. Region-grid byte at this object's centre.
RT3_NOINLINE uint32_t __fastcall Obj_CenterGridByte(void* o) {
    const float twoCells = 184.0f;
    float y = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / twoCells);
    float x = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / twoCells);
    return Map_GridByte(hook::As<void*>(g_map), 0, x, y);
}
RT3_REPLACE(0x48aa30, Obj_CenterGridByte);

// 0x48aa70. Region type at this object's centre.
RT3_NOINLINE uint32_t __fastcall Obj_CenterRegionType(void* o) {
    const float twoCells = 184.0f;
    float y = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / twoCells);
    float x = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / twoCells);
    return Map_RegionTypeAt(hook::As<void*>(g_map), 0, x, y);
}
RT3_REPLACE(0x48aa70, Obj_CenterRegionType);

float __fastcall Obj_Mid1ed(uint8_t* o) {
    return (float)(((double)F<float>(o, 0x1f1) + F<float>(o, 0x1ed)) / *(const double*)0x5c85d0);
}
RT3_REPLACE(0x48aab0, Obj_Mid1ed);

// 0x48aad0. Whether any corner, lowered by `off`, lies below the terrain.
int32_t __fastcall Vehicle_CornerBelowGround(uint8_t* o, int, float off) {
    Corners k;
    GetCorners(o, k);
    float* c[4] = { k.a, k.b, k.c, k.d };
    for (int i = 0; i < 4; ++i) {
        float v = (float)((double)c[i][1] - off);
        if (Terrain_HeightAt(Map(), 0, c[i][0], c[i][2]) > v) return 1;
    }
    return 0;
}
RT3_REPLACE(0x48aad0, Vehicle_CornerBelowGround);

RT3_NOINLINE void __fastcall Node_Set222(void* n, int, int32_t v) { F<int32_t>(n, 0x222) = v; }
RT3_REPLACE(0x48abc0, Node_Set222);

// 0x48abd0 (vtable stub).
void __fastcall Nop48abd0(void*) { __asm nop }
RT3_REPLACE(0x48abd0, Nop48abd0);

int32_t __fastcall Track_IsKind2(uint8_t* o) { return F<int32_t>(o, 0x226) == 2; }
RT3_REPLACE(0x48abe0, Track_IsKind2);

// 0x48abf0. Cost for `company` to build track piece t. Mixed pieces (kind +0x217 1 /
// 2 / 4 with a different kind or a bridge flag) cost 1.5 x the difference between
// pricing them as either. Otherwise: by piece type +0x226 a base from height above
// ground ^1.2 and tilt ^1.13 (plain track), a bridge (+0x219 table 0x621a34), tunnel
// (+0x251) or flat surcharge; non-double track is scaled by 1.666, cells already
// built on cost 2000 more, else 250 per terrain cell count; signalled pieces (+0x44)
// add 600 / 999.6. Then the region / company multipliers (Effects 0x3d / 0x3e /
// 0x40 / 0x41), the difficulty table (0x5f3390) and the company's own blend.
RT3_NOINLINE int32_t __fastcall Track_BuildCost(uint8_t* t, int, uint32_t company, int32_t inner) {
    uint8_t kind = t[0x217];
    if (!inner && (kind == 1 || kind == 4 || kind == 2) &&
        ((t[0x218] == 4 && kind == 1) || (!(t[0x24c] & 0x10) && t[0x44]))) {
        uint8_t old218 = t[0x218], old44 = t[0x44];
        t[0x218] = kind;
        t[0x44] = (uint8_t)((t[0x24c] >> 4) & 1);
        float c1 = (float)Track_BuildCost(t, 0, company, 1);
        t[0x218] = old218;
        t[0x44] = old44;
        float c2 = (float)Track_BuildCost(t, 0, company, 1);
        float d = (float)((double)c2 - c1);
        return Ftol((float)((double)d * D(0x5c8578)));
    }
    uint8_t* rec = nullptr;
    uint32_t owner = 0;
    if (Collection_IsValid(g_companies, 0, company)) {
        rec = (uint8_t*)Collection_Get(g_companies, 0, company);
        owner = F<uint32_t>(rec, 0x3b);
    }
    float my = (float)(((double)F<float>(t, 0x1c) + F<float>(t, 0x34)) / Fl_205(0x5cfcfc));
    float mx = (float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl_205(0x5cfcfc));
    uint32_t region = Map_GridByte(g_map, 0, mx, my);
    uint8_t* w = g_world;
    float f3d = Effects_Multiplier(w, 0, 0x3d, company, owner, region);
    float f3e = Effects_Multiplier(g_world, 0, 0x3e, company, owner, region);
    Effects_Multiplier(g_world, 0, 0x3f, company, owner, region);
    float f40 = Effects_Multiplier(g_world, 0, 0x40, company, owner, region);
    float f41 = Effects_Multiplier(g_world, 0, 0x41, company, owner, region);
    int32_t single = t[0x218] == 1;
    float extra = 0.0f;
    int32_t cost = 1000;
    switch (F<uint32_t>(t, 0x226)) {
    case 0:
    case 0xc:
    case 0xd: {
        float h = Obj_MidAboveGround(t);
        h = (float)((double)h - Fl_205(0x5c8900));
        if (h < Fl_205(0x5c8598)) h = 0.0f;
        float p1 = (float)crt::pow(h, 1.2);
        float tilt = Vehicle_Tilt(t, 0, 1);
        float lk = (float)((double)tilt * Fl_205(0x5c8c28));
        float p2 = (float)crt::pow(lk, 1.13);
        float base = (float)Ftol((double)p1 * f3e * Fl_205(0x5c8c28) + Fl_205(0x5c86a0));
        cost = Ftol((double)p2 * f3e * Fl_205(0x5ca068) + base);
        break;
    }
    case 1:
        if (t[0x219] != 0xff)
            extra = (float)(((double)((const float*)0x621a34)[(int8_t)t[0x219]] - D(0x5c8770)) * ((double)f40 * Fl_205(0x5c86a0)));
        break;
    case 2:
        if (t[0x251] != 0xff) extra = (float)((double)f41 * Fl_205(0x5c86a0) * D(0x5c9ab8));
        break;
    case 3:
        extra = (float)((double)f40 * Fl_205(0x5c86a0) * D(0x5c87b8));
        break;
    case 10:
    case 11:
        cost = 0;
        break;
    }
    int32_t cx = Ftol((float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl_205(0x5cfcfc)));
    int32_t cy = Ftol((float)(((double)F<float>(t, 0x1c) + F<float>(t, 0x34)) / Fl_205(0x5cfcfc)));
    if (!single && cost > 0) {
        cost = Ftol((double)(cost - 1000) * D(0x5cfd98) + D(0x5cfd90));
        extra = (float)((double)extra * D(0x5cfd98));
    }
    if (CellBuffer_Get(cx, cy)) {
        cost += 2000;
    } else {
        float y = (float)(((double)F<float>(t, 0x1c) + F<float>(t, 0x34)) / Fl_205(0x5cfcfc));
        float x = (float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl_205(0x5cfcfc));
        int32_t iy = Ftol(y);
        cost += (int32_t)Terrain_CellCount(g_map, 0, Ftol(x), iy) * 0xfa;
    }
    if (t[0x44]) cost = Ftol((double)cost + (single ? 600.0 : BitsD(0x408f3cccccccccccull)));
    if (extra < Fl_205(0x5c8598)) extra = 0.0f;
    int32_t r = Ftol((double)(float)cost + extra);
    r = Ftol((double)(float)r * f3d);
    r = Ftol((double)(float)r * ((const float*)0x5f3390)[F<int32_t>(g_world, 0x2d)]);
    if (rec) {
        double blend = Obj_DifficultyBlend(rec);
        r = Ftol(blend * (float)r);
    }
    return r;
}
RT3_REPLACE(0x48abf0, Track_BuildCost);

// 0x48b100 / 0x48b110. Track_BuildCost for `company` (two identical entry points).
int32_t __fastcall Track_Cost(uint8_t* t, int, uint32_t company) { return Track_BuildCost(t, 0, company, 0); }
RT3_REPLACE(0x48b100, Track_Cost);

RT3_NOINLINE int32_t __fastcall Track_Cost2(uint8_t* t, int, uint32_t company) { return Track_BuildCost(t, 0, company, 0); }
RT3_REPLACE(0x48b110, Track_Cost2);

// 0x48b120. Carries track piece t's end heights over to its neighbours (+0x206 /
// +0x20a / +0x20e), so joints line up: the neighbour end that links back gets t's
// matching corner heights and is re-set (Track_SetEnds); through the first two links
// the change keeps spreading along switches whose branch side (+0x201 bit 1)
// matches. Skipped while loading (node table +0xed) and, in multiplayer, unless
// +0x216.
void __fastcall Track_MatchNeighbourHeights(uint8_t* t) {
    if (F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed)) return;
    if (IsMultiplayer() && !t[0x216]) return;
    float own[4][3];
    Effect_GetVectors(t, 0, (uint32_t*)own[0], (uint32_t*)own[1], (uint32_t*)own[2], (uint32_t*)own[3]);
    int32_t self = F<int32_t>(t, 0x202);
    // Returns -1 when unchanged, else the branch flag (1 = linked by +0x206).
    auto match = [&](uint8_t* nb, float (*v)[3], const float* h0, const float* h1) -> int {
        uint32_t* y0;
        uint32_t* y1;
        int branch;
        if (F<int32_t>(nb, 0x206) == self) {
            y0 = (uint32_t*)&v[1][1];
            y1 = (uint32_t*)&v[0][1];
            branch = 1;
        } else if (F<int32_t>(nb, 0x20a) == self) {
            y0 = (uint32_t*)&v[3][1];
            y1 = (uint32_t*)&v[2][1];
            branch = 0;
        } else {
            return -1;
        }
        if (*(float*)y0 == *h0 && *(float*)y1 == *h1) return -1;
        *y0 = *(const uint32_t*)h0;
        *y1 = *(const uint32_t*)h1;
        Track_SetEnds(nb, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3]);
        return branch;
    };
    for (int k = 0; k < 2; ++k) {
        int32_t id = F<int32_t>(t, 0x206 + k * 4);
        if (id == -1) continue;
        uint8_t* nb = (uint8_t*)Collection_Get(g_tracks, 0, id);
        float v[4][3];
        Effect_GetVectors(nb, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3]);
        int branch = match(nb, v, &own[k * 2][1], &own[k * 2 + 1][1]);
        if (branch >= 0 && F<int32_t>(nb, 0x20e) != -1 && ((nb[0x201] >> 1) & 1) == branch) Track_MatchNeighbourHeights(nb);
    }
    if (F<int32_t>(t, 0x20e) == -1) return;
    uint8_t* nb = (uint8_t*)Collection_Get(g_tracks, 0, F<int32_t>(t, 0x20e));
    float v[4][3];
    Effect_GetVectors(nb, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3]);
    int e = (t[0x201] & 2) ? 0 : 2;
    match(nb, v, &own[e][1], &own[e + 1][1]);
}
RT3_REPLACE(0x48b120, Track_MatchNeighbourHeights);

// 0x48b4b0. Matches the node's light mode (+0x5a) to its lit neighbours (+0x206
// back, +0x20a ahead; flag +0x59 & 0x10, mode not 0xff): mode 2/3 (slopes) when
// they differ, their mode when they agree; neighbours already on a slope win.
void __fastcall Node_UpdateLightFromNeighbors(uint8_t* n) {
    Collection* nodes = g_tracks;
    auto lit = [&](uint32_t off) {
        int32_t id = F<int32_t>(n, off);
        return id != -1 && (((uint8_t*)Collection_Get(nodes, 0, id))[0x59] & 0x10) ? 1 : 0;
    };
    auto mode = [&](uint32_t off) { return ((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(n, off)))[0x5a]; };
    int32_t a = lit(0x206), b = lit(0x20a);
    if (a && mode(0x206) == 0xff) a = 0;
    if (b && mode(0x20a) == 0xff) b = 0;
    if (a) {
        uint8_t ma = mode(0x206);
        if (b) {
            uint8_t mb = mode(0x20a);
            if (ma == 2 || ma == 3 || mb == 2 || mb == 3) return;
            if (ma == mb) {
                if (n[0x5a] != ma) Light_SetMode(n, 0, ma);
                return;
            }
            Light_SetMode(n, 0, ma ? 2 : 3);
            return;
        }
        if (ma == 2 || ma == 3 || ma == n[0x5a]) return;
        Light_SetMode(n, 0, ma ? 2 : 3);
        return;
    }
    if (!b) return;
    uint8_t mb = mode(0x20a);
    if (mb == 2 || mb == 3 || mb == n[0x5a]) return;
    Light_SetMode(n, 0, mb ? 3 : 2);
}
RT3_REPLACE(0x48b4b0, Node_UpdateLightFromNeighbors);

// 0x48b630. Option +0x2bf: 0 off, 2 on, else on in screen mode 12 (0x6d0818 +0x8c).
RT3_NOINLINE int32_t Option2bfActive() {
    int32_t m = F<int32_t>(g_app, 0x2bf);
    if (m == 0) return 0;
    if (m == 2) return 1;
    uint8_t* s = g_gameWindow;
    return s && F<int32_t>(s, 0x8c) == 0xc ? 1 : 0;
}
RT3_REPLACE(0x48b630, Option2bfActive);

// 0x48b660. Sets a track piece's colour: the slope overlay when it is plain track
// and no overlay is forced (nodes +0xe9); red for blocked pieces (world +0x4ae3,
// +0x201 bit 6); with owner colours on (option +0x2bf: 2, or 1 in the track view
// 0x6d0818 mode 12) by owner (the player's red tones, others' pale) and kind
// (+0x44 double, +0x218 == 4 electric); else grey (dimmed for +0x216 == 1 with
// option +0x98).
RT3_NOINLINE void __fastcall Track_UpdateColor(uint8_t* o) {
    uint8_t* opts = g_app;
    int32_t owners;
    int32_t m = F<int32_t>(opts, 0x2bf);
    if (!m) owners = 0;
    else if (m == 2) owners = 1;
    else {
        uint8_t* view = g_gameWindow;
        owners = view && F<int32_t>(view, 0x8c) == 0xc ? 1 : 0;
    }
    uint8_t kind = o[0x216];
    if (!kind && !F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xe9)) {
        Track_ColorSlope(o);
        return;
    }
    if (F<int32_t>(g_world, 0x4ae3) && (o[0x201] & 0x40)) {
        Obj_SetArgb(o, 0, 0xff, 0x5f, 0x5f, 0xff);
        return;
    }
    if (owners) {
        bool elec = o[0x218] == 4;
        bool mine = F<uint16_t>(o, 0x22e) == (uint32_t)Obj_Get21(hook::As<void*>(g_world));
        bool dbl = o[0x44] != 0;
        if (mine) {
            if (dbl) {
                if (elec) Obj_SetArgb(o, 0, 0xff, 0x7f, 0x7f, 0xff);
                else Obj_SetArgb(o, 0, 0xff, 0x5f, 0x5f, 0xdf);
            } else if (elec) Obj_SetArgb(o, 0, 0xff, 0x20, 0x28, 0x88);
            else Obj_SetArgb(o, 0, 0xff, 0, 0, 0x5f);
        } else if (dbl) {
            if (elec) Obj_SetArgb(o, 0, 0xff, 0xff, 0xcf, 0xcf);
            else Obj_SetArgb(o, 0, 0xff, 0xff, 0xbf, 0xbf);
        } else if (elec) Obj_SetArgb(o, 0, 0xff, 0xef, 0x7f, 0x7f);
        else Obj_SetArgb(o, 0, 0xff, 0xdf, 0x6f, 0x6f);
        return;
    }
    if (kind == 1 && F<int32_t>(opts, 0x98)) Obj_SetArgb(o, 0, 0x70, 0x7f, 0x7f, 0x7f);
    else Obj_SetArgb(o, 0, 0xff, 0x7f, 0x7f, 0x7f);
}
RT3_REPLACE(0x48b660, Track_UpdateColor);

// 0x48b830. Sets the track piece's kind (+0x216); becoming kind 2 counts it in the
// track totals and clears the trains' +0x1a3 flags. Recolours it.
RT3_NOINLINE void __fastcall Track_SetKind(uint8_t* o, int, uint8_t kind) {
    if (kind == 2 && o[0x216] != kind) {
        Obj_CountTrack(hook::As<uint8_t*>(g_tracks), 0, o, 1);
        Trains_Clear1a3(g_trains);
    }
    o[0x216] = kind;
    Track_UpdateColor(o);
}
RT3_REPLACE(0x48b830, Track_SetKind);

// 0x48b870. Whether track node n is an end of its route (+0x212), splitting the
// route off first when it no longer exists.
RT3_NOINLINE int32_t __fastcall Node_IsRouteEnd(uint8_t* n) {
    if (!Collection_IsValid(Routes(), 0, F<uint32_t>(n, 0x212)) && !Routes_SplitAt(Routes(), 0, F<uint32_t>(n, 0x202)))
        return 0;
    return Pair_Contains(Collection_Get(Routes(), 0, F<uint32_t>(n, 0x212)), 0, F<int32_t>(n, 0x202));
}
RT3_REPLACE(0x48b870, Node_IsRouteEnd);

// 0x48b8d0. Virtual slot 6 (1) for an unclassed track piece with +8, unless the node table's +0xed is set.
RT3_NOINLINE void __fastcall Track_Refresh(uint8_t* t) {
    if (F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed)) return;
    if (F<int32_t>(t, 0x226) || !F<int32_t>(t, 8)) return;
    ((void(__thiscall*)(void*, int32_t))(*(void***)t)[0x18 / 4])(t, 1);
}
RT3_REPLACE(0x48b8d0, Track_Refresh);

// 0x48b900 / 0x48b950. Front / rear axle midpoint in tile units.
RT3_NOINLINE void __fastcall Vehicle_FrontMid(uint8_t* o, int, float* x, float* z) {
    Corners k;
    GetCorners(o, k);
    const float two = *(const float*)0x5c878c, tile = *(const float*)0x5c8700;
    *x = (float)(((double)k.b[0] + k.a[0]) / two / tile);
    *z = (float)(((double)k.b[2] + k.a[2]) / two / tile);
}
RT3_REPLACE(0x48b900, Vehicle_FrontMid);

RT3_NOINLINE void __fastcall Vehicle_RearMid(uint8_t* o, int, float* x, float* z) {
    Corners k;
    GetCorners(o, k);
    const float two = *(const float*)0x5c878c, tile = *(const float*)0x5c8700;
    *x = (float)(((double)k.d[0] + k.c[0]) / two / tile);
    *z = (float)(((double)k.d[2] + k.c[2]) / two / tile);
}
RT3_REPLACE(0x48b950, Vehicle_RearMid);

// 0x48b9a0. Axle distance in tile units (unrounded).
double __fastcall Vehicle_Length(uint8_t* o) {
    Corners k;
    GetCorners(o, k);
    const float two = *(const float*)0x5c878c, tile = *(const float*)0x5c8700;
    float p0 = (float)(((double)k.b[0] + k.a[0]) / two / tile);
    float p1 = (float)(((double)k.b[2] + k.a[2]) / two / tile);
    float p2 = (float)(((double)k.d[0] + k.c[0]) / two / tile);
    float p3 = (float)(((double)k.d[2] + k.c[2]) / two / tile);
    return Distance2D(p0, p1, p2, p3);
}
RT3_REPLACE(0x48b9a0, Vehicle_Length);

// 0x48ba40. Heading of the side front-left -> rear-right (+ pi when reversed).
float __fastcall Vehicle_Heading(uint8_t* o, int, int32_t forward) {
    Corners k;
    Effect_GetVectors(o, 0, (uint32_t*)k.a, nullptr, nullptr, (uint32_t*)k.d);
    float dx = (float)((double)k.a[0] - k.d[0]);
    if (dx == *(const float*)0x5c8598) *(uint32_t*)&dx = 0xb8d1b717;
    float a = (float)crt::atan(((double)k.a[2] - k.d[2]) / dx);
    const double pi = *(const double*)0x5c96a8;
    if (k.d[0] < k.a[0]) a = (float)((double)a + pi);
    if (!forward) a = (float)((double)a + pi);
    return WrapAngle(a);
}
RT3_REPLACE(0x48ba40, Vehicle_Heading);

// 0x48bb40. Same, as a screen angle (0x5cfda0 - (atan + 0x5c96f0)).
RT3_NOINLINE float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward) {
    Corners k;
    Effect_GetVectors(o, 0, (uint32_t*)k.a, nullptr, nullptr, (uint32_t*)k.d);
    float dx = (float)((double)k.a[0] - k.d[0]);
    if (dx == *(const float*)0x5c8598) *(uint32_t*)&dx = 0xb8d1b717;
    float a = (float)(crt::atan(((double)k.a[2] - k.d[2]) / dx) + *(const double*)0x5c96f0);
    a = (float)(*(const double*)0x5cfda0 - a);
    const double pi = *(const double*)0x5c96a8;
    if (k.d[0] < k.a[0]) a = (float)((double)a + pi);
    if (!forward) a = (float)((double)a + pi);
    return WrapAngle(a);
}
RT3_REPLACE(0x48bb40, Vehicle_ScreenHeading);

// 0x48bc50. Pitch atan(dh / length) between the axle midpoints (signed by direction).
float __fastcall Vehicle_Pitch(uint8_t* o, int, int32_t forward) {
    Corners k;
    GetCorners(o, k);
    const double half = *(const double*)0x5c85d0;
    float fx = (float)(((double)k.b[0] + k.a[0]) / half);
    float fz = (float)(((double)k.b[2] + k.a[2]) / half);
    float fy = (float)(((double)k.b[1] + k.a[1]) / half);
    float rx = (float)(((double)k.d[0] + k.c[0]) / half);
    float rz = (float)(((double)k.d[2] + k.c[2]) / half);
    float ry = (float)(((double)k.d[1] + k.c[1]) / half);
    float dist = (float)Distance2D(fx, fz, rx, rz);
    if ((double)dist == *(const double*)0x5c8608) return *(const float*)0x5c8598;
    if (forward) return (float)crt::atan(((double)ry - fy) / dist);
    return (float)crt::atan(((double)fy - ry) / dist);
}
RT3_REPLACE(0x48bc50, Vehicle_Pitch);

// 0x48bd70. Clears a track piece's class and properties (after virtual slot 6 with 0).
void __fastcall Track_ClearProps(uint8_t* t) {
    ((void(__thiscall*)(void*, int32_t))(*(void***)t)[0x18 / 4])(t, 0);
    t[0x219] = 0xff;
    t[0x251] = 0xff;
    F<int32_t>(t, 0x226) = 0;
    t[0x24c] &= 0xdf;
}
RT3_REPLACE(0x48bd70, Track_ClearProps);

// 0x48bdb0. Marks the map under a track piece (unless the node table is bulk
// loading, +0xed): six points sampled along its two curve segments (control points
// from Effect_GetVectors, in cells) flag their cell (in town pieces bit 4, with the
// fine-grid street bit when +8; otherwise fine-grid layer 0/1 by kind +0x226 and
// cell bit 2).
RT3_NOINLINE void __fastcall Track_MarkCells(uint8_t* t) {
    if (F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed)) return;
    int32_t town = (*(int32_t(__fastcall**)(void*))(*(uint8_t**)t + 0x14))(t);
    int32_t street = F<int32_t>(t, 8) != 0;
    int32_t layer = (F<int32_t>(t, 0x226) == 1 || F<int32_t>(t, 0x226) == 3) ? 0 : 1;
    float a[3], b[3], c[3], d[3];
    Effect_GetVectors(t, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)c, (uint32_t*)d);
    const float cell = *(const float*)0x5c8700;
    float* pts[4] = { a, b, c, d };
    for (float* v : pts) {
        v[0] = (float)((double)v[0] / cell);
        v[2] = (float)((double)v[2] / cell);
    }
    float px[6], py[6];
    LerpPercent(0x14, &px[0], a[0], a[2], b[0], b[2], &py[0]);
    LerpPercent(0x50, &px[1], a[0], a[2], b[0], b[2], &py[1]);
    LerpPercent(0x14, &px[2], c[0], c[2], d[0], d[2], &py[2]);
    LerpPercent(0x50, &px[3], c[0], c[2], d[0], d[2], &py[3]);
    LerpPercent(0x32, &px[4], px[0], py[0], px[3], py[3], &py[4]);
    LerpPercent(0x32, &px[5], px[1], py[1], px[2], py[2], &py[5]);
    for (int32_t k = 0; k < 6; ++k) {
        int32_t xi = Ftol((double)px[k] * Fl_204(0x5c8568)), yi = Ftol((double)py[k] * Fl_204(0x5c8568));
        auto grid = [&]() -> uint8_t& {
            uint8_t* m = g_map;
            int32_t row = Ftol(py[k]);
            return F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * row + Ftol(px[k])];
        };
        if (town) {
            grid() |= 4;
            if (street) Map_SetFineBit(g_map, 0, xi, yi, 1, 2, 1);
        } else {
            Map_SetFineBit(g_map, 0, xi, yi, 1, layer, 1);
            grid() |= 2;
        }
    }
}
RT3_REPLACE(0x48bdb0, Track_MarkCells);

// 0x48c400. Undoes Track_MarkCells (unless the node table is bulk loading): clears
// the fine-grid layer bit (layer 0 for kinds 1..3) and the cell bit (4 for town
// pieces, else 2) at the same six samples; asking the piece whether it is a town
// piece again for each. With `remark`, re-marks the nodes over the samples'
// fine-grid box (the last row is left out).
RT3_NOINLINE void __fastcall Track_UnmarkCells(uint8_t* t, int, int32_t remark) {
    if (F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed)) return;
    int32_t kind = F<int32_t>(t, 0x226);
    int32_t layer = (kind == 1 || kind == 3 || kind == 2) ? 0 : 1;
    int32_t minX = 99999, maxX = 0, minY = 99999, maxY = 0;
    float a[3], b[3], c[3], d[3];
    Effect_GetVectors(t, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)c, (uint32_t*)d);
    const float cell = *(const float*)0x5c8700;
    float* pts[4] = { a, b, c, d };
    for (float* v : pts) {
        v[0] = (float)((double)v[0] / cell);
        v[2] = (float)((double)v[2] / cell);
    }
    float px[6], py[6];
    LerpPercent(0x14, &px[0], a[0], a[2], b[0], b[2], &py[0]);
    LerpPercent(0x50, &px[1], a[0], a[2], b[0], b[2], &py[1]);
    LerpPercent(0x14, &px[2], c[0], c[2], d[0], d[2], &py[2]);
    LerpPercent(0x50, &px[3], c[0], c[2], d[0], d[2], &py[3]);
    LerpPercent(0x32, &px[4], px[0], py[0], px[3], py[3], &py[4]);
    LerpPercent(0x32, &px[5], px[1], py[1], px[2], py[2], &py[5]);
    for (int32_t k = 0; k < 6; ++k) {
        int32_t xi = Ftol((double)px[k] * Fl_204(0x5c8568)), yi = Ftol((double)py[k] * Fl_204(0x5c8568));
        Map_SetFineBit(g_map, 0, xi, yi, 0, layer, 1);
        if (xi <= minX) minX = xi;
        if (xi >= maxX) maxX = xi;
        if (yi <= minY) minY = yi;
        if (yi >= maxY) maxY = yi;
        int32_t town = (*(int32_t(__fastcall**)(void*))(*(uint8_t**)t + 0x14))(t);
        uint8_t* m = g_map;
        int32_t row = Ftol(py[k]);
        uint8_t& g = F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * row + Ftol(px[k])];
        g &= town ? 0xfb : 0xfd;
    }
    if (!remark) return;
    for (int32_t x = minX; x <= maxX; ++x)
        for (int32_t y = minY; y < maxY; ++y) Nodes_RemarkAround(g_tracks, 0, x, y);
}
RT3_REPLACE(0x48c400, Track_UnmarkCells);

// 0x48cad0. The node a train moves to from this node towards `target`: arriving
// from `prev`, a train on route r (car `car` +0x356) takes the third link +0x20e
// when the switch bit (+0x201 bit 1, flipped by direction) points there and it
// stays on r; going back onto r gives null. Otherwise the route search's first
// step (null when there is none or it stays here). No target: null (after a
// random draw).
RT3_NOINLINE uint8_t* __fastcall Node_NextTowards(uint8_t* n, int, int32_t target, int32_t car, int32_t prev, uint8_t* train, int32_t anyTrack) {
    Collection* nodes = g_tracks;
    uint8_t bits = n[0x201] >> 1;
    int32_t sw = (prev == F<int32_t>(n, 0x206) ? bits : (uint8_t)~bits) & 1;
    if (car != -1) {
        int32_t at = F<int32_t>(Collection_Get(g_62ba84, 0, (uint32_t)car), 0x356);
        if (at != -1) {
            int32_t route = F<int32_t>(Collection_Get(nodes, 0, (uint32_t)at), 0x212);
            if (route != -1) {
                if (F<int32_t>(n, 0x20e) != -1 && sw && F<int32_t>(Collection_Get(nodes, 0, F<uint32_t>(n, 0x20e)), 0x212) == route)
                    return (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(n, 0x20e));
                if (prev != -1 && F<int32_t>(Collection_Get(nodes, 0, (uint32_t)prev), 0x212) == route) return nullptr;
            }
        }
    }
    if (target == -1) {
        Rand(0, 100);
        return nullptr;
    }
    int32_t need = 0;
    uint32_t* co = nullptr;
    if (train) {
        if (F<int32_t>(Obj_CityRef(train), 0x10) == 2) need = 1;
        co = (uint32_t*)Train_Company(train);
    }
    uint32_t cost;
    int32_t step = Route_NextStep(g_routes, 0, F<int32_t>(n, 0x202), target, &cost, need, co, nullptr, anyTrack);
    if (step == -1 || step == F<int32_t>(n, 0x202)) return nullptr;
    return (uint8_t*)Collection_Get(nodes, 0, (uint32_t)step);
}
RT3_REPLACE(0x48cad0, Node_NextTowards);

// 0x48cc20. Whether the track piece overlaps another one: 17 probe points (its four
// control points, the centre, the edge midpoints and points 0x5c88f0 of the way
// along the edges) are looked up in the node grid, skipping the piece's own
// neighbours (+0x202 with +0x206 or +0x20a); a hit on a non-bridge piece (+0x226 == 0)
// answers 1 and stores its id in *hit.
int32_t __fastcall Track_FindOverlap(uint8_t* t, int, int32_t* hit) {
    float a[3], b[3], c[3], d[3];
    Effect_GetVectors(t, 0, (uint32_t*)a, (uint32_t*)b, (uint32_t*)c, (uint32_t*)d);
    const double cell = *(const float*)0x5c8700, four = *(const float*)0x5c8568;
    auto probe = [&](float x, float y, uint32_t other) {
        Collection* nodes = g_tracks;
        int32_t id = Grid_NearestObject(nodes, 0, x, y, F<int32_t>(t, 0x202), F<int32_t>(t, other), 1);
        if (id == -1) return false;
        if (F<int32_t>(Collection_Get(g_tracks, 0, id), 0x226)) return false;
        if (hit) *hit = id;
        return true;
    };
    auto fine = [&](double v) { return (float)(v / cell * four); };
    auto at = [&](const float* p, uint32_t other) { return probe(fine(p[0]), fine(p[2]), other); };
    auto mid = [&](const float* p, const float* q, uint32_t other) {
        auto m = [&](double u, double v) { return (float)((u + v) / D(0x5c85d0) / D(0x5c8c48) * D(0x5c87b8)); };
        return probe(m(p[0], q[0]), m(p[2], q[2]), other);
    };
    auto along = [&](const float* from, const float* to, uint32_t other) {
        float px = (float)(((double)to[0] - from[0]) * D(0x5c88f0) + from[0]);
        float pz = (float)(((double)to[2] - from[2]) * D(0x5c88f0) + from[2]);
        return probe(fine(px), fine(pz), other);
    };
    if (at(a, 0x206) || at(b, 0x206) || at(c, 0x20a) || at(d, 0x20a)) return 1;
    float cx = (float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl_204(0x5cfcfc));
    float cz = (float)(((double)F<float>(t, 0x34) + F<float>(t, 0x1c)) / Fl_204(0x5cfcfc));
    if (probe((float)((double)cx * four), (float)((double)cz * four), 0x20a)) return 1;
    if (mid(b, a, 0x206) || mid(c, b, 0x206) || mid(d, c, 0x20a) || mid(d, a, 0x20a)) return 1;
    if (along(d, a, 0x206) || along(c, b, 0x206) || along(b, c, 0x20a) || along(a, d, 0x20a)) return 1;
    if (along(b, a, 0x206) || along(a, b, 0x206) || along(d, c, 0x20a) || along(c, d, 0x20a)) return 1;
    return 0;
}
RT3_REPLACE(0x48cc20, Track_FindOverlap);

// 0x48d540. The hover text of a track piece: one of the debug overlays chosen
// in the options (route id +0x212, signal +0x226 / +0x219, route component,
// block +0x202, the +0x202..+0x20a triple, the +0x21a / +0x21e pair or the
// +0x238 / +0x23c ranges) copied into `dst` (returns 1); otherwise, with the
// grade / speed overlays of the camera (0x6cfca8) on, the grade (percent,
// coloured green..red into *color) and / or the speed limit +0x230 (returns how
// many texts were written, *offset = -0.5); else the normal track tooltip.
RT3_NOINLINE int32_t __fastcall Track_InfoText(uint8_t* t, int, char* dst, int32_t n, uint32_t* color, float* offset, char* dst2, int32_t extra) {
    uint8_t* opts = g_app;
    char text[0x100], a[0x40], b[0x40], c[0x40], d[0x40];
    auto u = [](uint32_t v) {
        double r = (double)(int32_t)v;
        if ((int32_t)v < 0) r += D(0x5c8910);
        return r;
    };
    auto done = [&] {
        StrCopyN(dst, text, n - 1);
        return 1;
    };
    if (F<int32_t>(opts, 0x80)) {
        Num_Format(text, 8, u(F<uint32_t>(t, 0x212)), 10, 1, 0, 0);
        return done();
    }
    if (F<int32_t>(opts, 0x293)) {
        Num_Format(text, 8, (double)F<int32_t>(t, 0x226), 10, 1, 0, 0);
        char tmp[0x40];
        crt::sprintf(tmp, (const char*)0x5cfdd0, (int32_t)(int8_t)t[0x219]);
        strcat(text, tmp);
        return done();
    }
    if (F<int32_t>(opts, 0x2b3)) {
        int32_t route = F<int32_t>(t, 0x212);
        int32_t v = route == -1 ? -1 : Routes_Component(g_routes, 0, route);
        Num_Format(text, 8, (double)v, 10, 1, 0, 0);
        return done();
    }
    if (F<int32_t>(opts, 0x27f)) {
        Num_Format(text, 8, u(F<uint32_t>(t, 0x202)), 10, 1, 0, 0);
        return done();
    }
    if (F<int32_t>(opts, 0x2af)) {
        const char* n1 = Num_Format(a, 1, u(F<uint32_t>(t, 0x20a)), 10, 1, 0, 0);
        const char* n2 = Num_Format(b, 1, u(F<uint32_t>(t, 0x206)), 10, 1, 0, 0);
        const char* n3 = Num_Format(c, 1, u(F<uint32_t>(t, 0x202)), 10, 1, 0, 0);
        FormatText(text, (const char*)0x5cfdc4, n3, n2, n1, 0, 0, 0, 0, 0);
        return done();
    }
    int32_t lo = F<int32_t>(t, 0x21a), hi = F<int32_t>(t, 0x21e);
    if (F<int32_t>(opts, 0x84)) {
        if (lo != -1 && hi != -1) {
            const char* nb = Num_Format(c, 1, (double)hi, 10, 1, 0, 0);
            const char* na = Num_Format(a, 1, (double)F<int32_t>(t, 0x21a), 10, 1, 0, 0);
            FormatText(text, (const char*)0x5cfdbc, na, nb, 0, 0, 0, 0, 0, 0);
            return done();
        }
        if (lo != -1) {
            Num_Format(text, 1, (double)lo, 10, 1, 0, 0);
            return done();
        }
        if (hi != -1) {
            Num_Format(text, 1, (double)hi, 10, 1, 0, 0);
            return done();
        }
    } else if (F<int32_t>(opts, 0x88)) {
        if (lo != -1 && F<int32_t>(t, 0x21e) != -1) {
            const char* n1 = Num_Format(c, 1, (double)F<int32_t>(t, 0x23c), 10, 1, 0, 0);
            const char* n2 = Num_Format(a, 1, (double)F<int32_t>(t, 0x21e), 10, 1, 0, 0);
            const char* n3 = Num_Format(b, 1, (double)F<int32_t>(t, 0x238), 10, 1, 0, 0);
            const char* n4 = Num_Format(d, 1, (double)F<int32_t>(t, 0x21a), 10, 1, 0, 0);
            FormatText(text, (const char*)0x5cfdb0, n4, n3, n2, n1, 0, 0, 0, 0);
            return done();
        }
        if (lo != -1) {
            const char* n1 = Num_Format(d, 1, (double)F<int32_t>(t, 0x238), 10, 1, 0, 0);
            const char* n2 = Num_Format(c, 1, (double)F<int32_t>(t, 0x21a), 10, 1, 0, 0);
            FormatText(text, (const char*)0x5cfda8, n2, n1, 0, 0, 0, 0, 0, 0);
            return done();
        }
        if (F<int32_t>(t, 0x21e) != -1) {
            const char* n1 = Num_Format(d, 1, (double)F<int32_t>(t, 0x23c), 10, 1, 0, 0);
            const char* n2 = Num_Format(c, 1, (double)F<int32_t>(t, 0x21e), 10, 1, 0, 0);
            FormatText(text, (const char*)0x5cfda8, n2, n1, 0, 0, 0, 0, 0, 0);
            return done();
        }
    }
    uint8_t* cam = hook::As<uint8_t*>(g_tracks);
    int32_t grade, speed;
    if (!t[0x216]) {
        grade = Camera_FlagA4(cam, 0, 1);
        speed = Camera_FlagAc(hook::As<uint8_t*>(g_tracks), 0, 1);
        grade |= Camera_FlagA0(hook::As<uint8_t*>(g_tracks), 0, 1);
        speed |= Camera_FlagA8(hook::As<uint8_t*>(g_tracks), 0, 1);
    } else {
        grade = Camera_FlagA0(cam, 0, 1);
        speed = Camera_FlagA8(hook::As<uint8_t*>(g_tracks), 0, 1);
    }
    if (!grade && !speed) return Track_DebugTooltip(t, 0, dst, n, (int32_t)(uintptr_t)color, (int32_t)(uintptr_t)offset, (int32_t)(uintptr_t)dst2, extra);
    *(uint32_t*)offset = 0xbf000000;
    float limit = (float)u(F<uint32_t>(t, 0x230));
    float tilt = (float)((double)Vehicle_Tilt(t, 0, 1) * Fl_205(0x5c8c28));
    int32_t ret = 0;
    if (grade) {
        Num_Format(dst, 1, (double)Ftol((double)tilt + D(0x5c8628)), 10, 1, 0, 0);
        double q = (double)tilt / D(0x5c8610);
        double cl = D(0x5c8770) < q ? 1.0 : q;
        float f = (float)cl;
        float g = (float)(D(0x5c8770) - f);
        *color = HeightColor(g);
        ret = 1;
    }
    if (speed) {
        Num_Format(grade ? dst2 : dst, 1, (double)Ftol(((double)limit + Fl_205(0x5ca06c)) / D(0x5c87f8)), 10, 1, 0, 0);
        ++ret;
    }
    return ret;
}
RT3_REPLACE(0x48d540, Track_InfoText);

// 0x48dcb0. Owning company (u16 +0x22e) of a track node, if valid.
RT3_NOINLINE void* __fastcall Node_Company(void* n) {
    if (!g_companies) return nullptr;
    if (!Collection_IsValid(g_companies, 0, F<uint16_t>(n, 0x22e))) return nullptr;
    return Collection_Get(g_companies, 0, F<uint16_t>(n, 0x22e));
}
RT3_REPLACE(0x48dcb0, Node_Company);

// 0x48dcf0. Track piece t: loads its decoration models from stream s (a
// 16-bit count, and from save version 0x2535 the attached model's index):
// each named model is read and grouped with the piece at its saved pose
// (beyond the sixth they are read and dropped); the attached one becomes
// +0x248. Then marks the piece's coarse map cell, counts a placed piece
// (state 1 -> 2) and recolours it. Returns Track_AfterLoad's result.
// QUIRK: older saves take the attached index from the low byte of the
// stream handle; the index matches dropped models too (left dangling).
RT3_NOINLINE int32_t __fastcall Track_LoadModels(uint8_t* t, int, int32_t s) {
    int32_t ret = Track_AfterLoad(t, 0, s);
    uint16_t count;
    F<void*>(t, 8) = nullptr;
    StreamRead(s, &count, 2, 0, 1);
    int32_t selSlot = s;
    if (F<int32_t>(g_world, 0x46c34) >= 0x2535) StreamRead(s, &selSlot, 1, 0, 1);
    for (int32_t i = 0; i < count; ++i) {
        uint8_t* mem = ((uint8_t*)crt::operator_new(0x23a));
        uint8_t* m = nullptr;
        if (mem) {
            NamedObj_Construct(mem);
            m = mem;
            F<uint32_t>(m, 0) = 0x5cfd00;
        }
        Obj_CloseCurrent(m, 0, s);
        if (i > 5) {
            SceneObj_Teardown(m);
            if (m) {
                NamedObj_Destructor(m);
                crt::operator_delete(m);
            }
        } else {
            uint32_t px, py, pz, aa, ab, ac;
            SceneObj_GetPos(m, 0, &px, &py, &pz);
            SceneObj_GetAngles(m, 0, &aa, &ab, &ac);
            Group_Add(t, 0, m, 0.0f);
            Obj_MoveTo(m, 0, BitsF(px), BitsF(py), BitsF(pz));
            SceneObj_SetAngles(m, 0, aa, ab, ac);
        }
        if (F<void*>(t, 0x248) && (int8_t)selSlot == i) F<uint8_t*>(t, 0x248) = m;
    }
    F<int32_t>(t, 0x244) = 0;
    float cy = (float)(((double)F<float>(t, 0x34) + F<float>(t, 0x1c)) / Fl_207(0x5cfcfc));
    float cx = (float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl_207(0x5cfcfc));
    int32_t iy = Ftol((double)cy);
    Map_SetCoarseByte(g_map, 0, Ftol((double)cx), iy, 0x80);
    if (t[0x216] == 1) {
        Obj_CountTrack(hook::As<uint8_t*>(g_tracks), 0, t, 1);
        Trains_Clear1a3(g_trains);
        t[0x216] = 2;
        Track_UpdateColor(t);
    }
    Track_UpdateColor(t);
    return ret;
}
RT3_REPLACE(0x48dcf0, Track_LoadModels);

// 0x48dec0. Track piece t: deletes the models in its list (+0x8) - all of
// them, or with `keepSelected` all but the attached one (+0x248, which is
// then kept along with the list) - after unmarking the cells of a placed
// bridge / tunnel / elevated piece, and resets its kind (+0x226), styles
// (+0x219 / +0x251) and model flag (+0x24c bit 5).
// QUIRK: a kept model must be the list's first: further down the cursor
// stays on it and the loop never ends.
void __fastcall Track_ClearModels(uint8_t* t, int, int32_t keepSelected) {
    PtrList* l = F<PtrList*>(t, 8);
    if (!l) return;
    int32_t kept = 0;
    PtrList_Rewind(l);
    if ((t[0x216] == 1 || t[0x216] == 2) && (F<int32_t>(t, 0x226) == 1 || F<int32_t>(t, 0x226) == 3 || F<int32_t>(t, 0x226) == 2))
        Track_UnmarkCells(t, 0, 0);
    int32_t i = 0;
    for (uint8_t* v = (uint8_t*)PtrList_CurrentValue(F<PtrList*>(t, 8)); v; ++i) {
        uint8_t* sel = F<uint8_t*>(t, 0x248);
        if (keepSelected && v == sel) {
            if (!i) PtrList_Next(F<PtrList*>(t, 8));
            v = (uint8_t*)PtrList_CurrentValue(F<PtrList*>(t, 8));
            kept = 1;
            continue;
        }
        if (!keepSelected && v == sel) F<void*>(t, 0x248) = nullptr;
        SceneObj_Teardown(v);
        NamedObj_Destructor(v);
        crt::operator_delete(v);
        PtrList_RemoveCurrent(F<PtrList*>(t, 8));
        v = (uint8_t*)PtrList_Next(F<PtrList*>(t, 8));
    }
    F<int32_t>(t, 0x226) = 0;
    t[0x219] = 0xff;
    t[0x251] = 0xff;
    t[0x24c] &= 0xdf;
    if (kept) return;
    PtrList_Clear(F<PtrList*>(t, 8));
    if (PtrList* d = F<PtrList*>(t, 8)) {
        PtrList_Destroy(d);
        crt::operator_delete(d);
    }
    F<void*>(t, 8) = nullptr;
}
RT3_REPLACE(0x48dec0, Track_ClearModels);

// 0x48dfe0. Colours a track piece by its wear (+0x22a) relative to `full`
// (at least 1).
RT3_NOINLINE void __fastcall Track_ColorWear(uint8_t* o, int, float full) {
    if (Fl(0x5c85b4) > full) full = 1.0f;
    float r = (float)((double)F<float>(o, 0x22a) / full);
    float t = (float)(D(0x5c8770) - r);
    Obj_SetRgbValue(o, 0, HeatColor(t) | 0xff000000);
}
RT3_REPLACE(0x48dfe0, Track_ColorWear);

// 0x48e050. Whether track node n links to node `target` within `depth` steps (not
// going back through `from`; children are searched with n's own id as `from`).
int32_t __fastcall Track_LinksWithin(uint8_t* n, int, int32_t target, int32_t depth, int32_t from) {
    static const uint32_t links[3] = { 0x206, 0x20a, 0x20e };
    for (uint32_t l : links)
        if (F<int32_t>(n, l) == target) return 1;
    if (depth == 1) return 0;
    for (uint32_t l : links) {
        int32_t id = F<int32_t>(n, l);
        if (id == -1 || id == from) continue;
        uint8_t* c = (uint8_t*)Collection_Get(g_tracks, 0, id);
        if (Track_LinksWithin(c, 0, target, depth - 1, F<int32_t>(n, 0x202))) return 1;
    }
    return 0;
}
RT3_REPLACE(0x48e050, Track_LinksWithin);

RT3_NOINLINE void* __fastcall Obj_Node206(void* o) { return RefNotMinus1(g_tracks, F<int32_t>(o, 0x206)); }
RT3_REPLACE(0x48e140, Obj_Node206);

RT3_NOINLINE void* __fastcall Obj_Node20a(void* o) { return RefNotMinus1(g_tracks, F<int32_t>(o, 0x20a)); }
RT3_REPLACE(0x48e160, Obj_Node20a);

void* __fastcall Track_Node20e(uint8_t* t) {
    int32_t id = F<int32_t>(t, 0x20e);
    return id != -1 ? Collection_Get(g_tracks, 0, id) : nullptr;
}
RT3_REPLACE(0x48e180, Track_Node20e);

int32_t __fastcall Track_EndLinksBack(uint8_t* t) {
    for (int end = 0; end < 2; ++end) {
        uint8_t* n = EndNode(t, end);
        if (!n || F<int32_t>(n, 0x20e) == -1) continue;
        int32_t self = F<int32_t>(t, 0x202);
        if (F<int32_t>(n, 0x20e) == self) return 1;
        int c = (n[0x201] >> 1) & 1;
        if (c && F<int32_t>(n, 0x206) == self) return 1;
        if (!c && F<int32_t>(n, 0x20a) == self) return 1;
    }
    return 0;
}
RT3_REPLACE(0x48e1a0, Track_EndLinksBack);

int32_t __fastcall Track_EndBackLink(uint8_t* t) {
    for (int end = 0; end < 2; ++end) {
        uint8_t* n = EndNode(t, end);
        if (!n || F<int32_t>(n, 0x20e) == -1) continue;
        int32_t self = F<int32_t>(t, 0x202);
        if (F<int32_t>(n, 0x20e) == self) return (n[0x201] & 2) ? F<int32_t>(n, 0x206) : F<int32_t>(n, 0x20a);
        int c = (n[0x201] >> 1) & 1;
        if (c && F<int32_t>(n, 0x206) == self) return F<int32_t>(n, 0x20e);
        if (!c && F<int32_t>(n, 0x20a) == self) return F<int32_t>(n, 0x20e);
    }
    return -1;
}
RT3_REPLACE(0x48e220, Track_EndBackLink);

// 0x48e2c0. Sets track piece t's highlight bit (+0x201 bit 5); turning it on also
// tints the piece and its scenery red.
void __fastcall Track_SetHighlight(uint8_t* t, int, int32_t on) {
    t[0x201] ^= ((uint8_t)(on << 5) ^ t[0x201]) & 0x20;
    if (on) {
        Obj_SetRgb(t, 0, 0xff, 0, 0);
        ObjList_SetTint(t, 0, 0xff, 0, 0);
    }
}
RT3_REPLACE(0x48e2c0, Track_SetHighlight);

// 0x48e310. Sets the track piece's blocked flag (+0x201 bit 6), recolouring on change.
RT3_NOINLINE void __fastcall Track_SetBlocked(uint8_t* o, int, uint8_t v) {
    uint8_t f = o[0x201];
    if (((f >> 6) & 1) == v) return;
    o[0x201] = (uint8_t)((((v << 6) ^ f) & 0x40) ^ f);
    Track_UpdateColor(o);
}
RT3_REPLACE(0x48e310, Track_SetBlocked);

// 0x48e340. Changes the owning company (+0x22e) and moves the piece between the companies' track counters.
RT3_NOINLINE void __fastcall Track_SetOwner(uint8_t* t, int, uint16_t owner) {
    uint16_t cur = F<uint16_t>(t, 0x22e);
    if (cur == owner) return;
    if (Collection_IsValid(g_companies, 0, cur))
        Company_CountTrack((uint8_t*)Collection_Get(g_companies, 0, F<uint16_t>(t, 0x22e)), 0, t, 0);
    if (Collection_IsValid(g_companies, 0, owner))
        Company_CountTrack((uint8_t*)Collection_Get(g_companies, 0, owner), 0, t, 1);
    F<uint16_t>(t, 0x22e) = owner;
}
RT3_REPLACE(0x48e340, Track_SetOwner);

// 0x48e3c0. Whether track node id is in the same block component as this node.
RT3_NOINLINE int32_t __fastcall Node_SameComponent(uint8_t* n, int, uint32_t id) {
    int32_t b = F<int32_t>(Collection_Get(g_tracks, 0, id), 0x212);
    return Routes_SameComponent(g_routes, 0, F<int32_t>(n, 0x212), b);
}
RT3_REPLACE(0x48e3c0, Node_SameComponent);

// 0x48e3f0. Computes once (flag 1 of +0x24c) whether the tile is blocked (flag 2):
// by a connected track node (+0x202, +0x206) or a nearby region centre.
RT3_NOINLINE void __fastcall Tile_UpdateBlocked(uint8_t* o) {
    uint8_t fl = o[0x24c];
    if (fl & 1) return;
    float cx = (float)(((double)F<float>(o, 0x2c) + F<float>(o, 0x14)) / *(const float*)0x5cfcfc);
    o[0x24c] = (uint8_t)((fl & 0xfd) | 1);
    float cy = (float)(((double)F<float>(o, 0x34) + F<float>(o, 0x1c)) / *(const float*)0x5cfcfc);
    for (int32_t i = 0; i < 3; ++i) {
        uint32_t id = i == 0 ? F<uint32_t>(o, 0x202) : F<uint32_t>(o, 0x206);
        if (id == 0xffffffff) continue;
        uint8_t* n = (uint8_t*)Collection_Get(Coll(0x6cfca8), 0, id);
        if (!n) continue;
        if (F<int32_t>(n, 0x226) || F<int32_t>(n, 0x20e) != -1 || F<int32_t>(n, 0x222) != -1) o[0x24c] |= 2;
    }
    if (o[0x24c] & 2) return;
    for (int32_t j = 0; j < Collection_Count(Coll(0x62bae0)); ++j) {
        Collection* c = Coll(0x62bae0);
        uint8_t* r = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, j, 0));
        if (F<int32_t>(r, 0x23e)) continue;
        float ry = (float)Obj_CellY(r);
        float rx = (float)Obj_CellX(r);
        float d = (float)DistanceSq2D(cx, cy, rx, ry);
        if (d < *(const float*)0x5cfdd4) {
            o[0x24c] |= 2;
            return;
        }
    }
}
RT3_REPLACE(0x48e3f0, Tile_UpdateBlocked);

// 0x48e540. Drops track piece t onto the terrain (3 above it); a plain piece with
// scenery attached (+8) at a dead end settles that scenery to the open end's
// height; then lifts the piece's own scenery (+0x248) and optionally matches the
// neighbours.
void __fastcall Track_DropAndSettle(uint8_t* t, int, int32_t, int32_t neighbours) {
    Track_DropOnTerrain(t, 0, 3.0f);
    if (F<void*>(t, 8) && !F<int32_t>(t, 0x226)) {
        float end = -1.0f;
        if (F<int32_t>(t, 0x206) == -1) end = 0.0f;
        else if (F<int32_t>(t, 0x20a) == -1) end = 1.0f;
        if (end >= 0.0f) Group_MoveAllToHeight(t, 0, (float)Effect_Blend(t, 0, end));
    }
    Track_LiftScenery(t);
    if (neighbours) Track_MatchNeighbourHeights(t);
}
RT3_REPLACE(0x48e540, Track_DropAndSettle);

// 0x48e600. Re-sets track piece t's corners: a fully linked piece moved without
// `rebuild` / `newPiece` only takes the new heights (keeping x / z); otherwise it is
// re-registered in the node grid (when placed, +0x216, or the table is live +0xe9)
// around the full corner update. Then refreshes its lighting, settles dead-end
// scenery, lifts its own scenery and optionally matches the neighbours.
RT3_NOINLINE void __fastcall Track_Reshape_Impl(uint8_t* t, int, const uint32_t* a, const uint32_t* b, const uint32_t* c,
                                   const uint32_t* d, int32_t neighbours, int32_t rebuild, int32_t newPiece) {
    if (!newPiece && !rebuild && F<int32_t>(t, 0x206) != -1 && F<int32_t>(t, 0x20a) != -1) {
        uint32_t v[4][3];
        Effect_GetVectors(t, 0, v[0], v[1], v[2], v[3]);
        v[0][1] = a[1];
        v[1][1] = b[1];
        v[2][1] = c[1];
        v[3][1] = d[1];
        Track_SetEnds(t, 0, v[0], v[1], v[2], v[3]);
    } else {
        if (t[0x216] || F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xe9)) Map_RemoveTrackRef(hook::As<uint8_t*>(g_tracks), 0, t);
        Track_SetEnds(t, 0, a, b, c, d);
        if (t[0x216] || F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xe9)) Grid_AddObject(hook::As<uint8_t*>(g_tracks), 0, t);
    }
    Obj_UpdateHeightLight(t);
    Track_SettleGroup(t);
    Track_LiftScenery(t);
    if (neighbours) Track_MatchNeighbourHeights(t);
}

// 0x4a17a4 / 0x4a1942 keep using ECX after the call.
__declspec(naked) void Track_Reshape() { RT3_KEEP_ECX_EDX(Track_Reshape_Impl, 7) }
RT3_REPLACE(0x48e600, Track_Reshape);

// 0x48e7a0. Sets a flat track piece to height h, lifts its scenery object (+0x248)
// to the piece's mean corner height less one, and optionally matches neighbours.
void __fastcall Track_SetFlatAndLift(uint8_t* t, int, uint32_t h, int32_t neighbours) {
    Track_SetFlatHeight(t, 0, h);
    if (uint8_t* o = F<uint8_t*>(t, 0x248)) {
        uint32_t x, y, z;
        SceneObj_GetPos(o, 0, &x, &y, &z);
        float ny = (float)((((((double)F<float>(t, 0x3c) + F<float>(t, 0x30)) + F<float>(t, 0x24)) + F<float>(t, 0x18)) /
                            D(0x5c87b8)) - D(0x5c8770));
        Obj_MoveTo(F<uint8_t*>(t, 0x248), 0, *(float*)&x, ny, *(float*)&z);
    }
    if (neighbours) Track_MatchNeighbourHeights(t);
}
RT3_REPLACE(0x48e7a0, Track_SetFlatAndLift);

// 0x48e820. Pairs track piece t with piece `id` (+0x253; -1 unpairs, also undoing
// the partner's link when `mutual`). The pair holds only if the pieces share no
// neighbour and each one's neighbours lie across the other; with `mutual` the
// partner is then paired back.
RT3_NOINLINE void __fastcall Track_PairWith(uint8_t* t, int, int32_t id, int32_t mutual) {
    if (id == -1) {
        if (mutual && Collection_IsValid(g_tracks, 0, F<uint32_t>(t, 0x253))) {
            uint8_t* o = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x253));
            if (F<int32_t>(o, 0x253) == F<int32_t>(t, 0x202))
                Track_PairWith((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x253)), 0, -1, 0);
        }
        F<int32_t>(t, 0x253) = -1;
        return;
    }
    for (;;) {
        if (!Collection_IsValid(g_tracks, 0, (uint32_t)id)) return;
        uint8_t* o = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)id);
        for (uint32_t l = 0x206; l <= 0x20a; l += 4) {
            int32_t v = F<int32_t>(o, l);
            if (v != -1 && (v == F<int32_t>(t, 0x206) || v == F<int32_t>(t, 0x20a))) return;
        }
        float x, y;
        Track_Mid(o, &x, &y);
        if (!Track_NeighbourCrosses(t, o, Effect_PointOnSide(t, 0, 1, x, y), true)) return;
        Track_Mid(t, &x, &y);
        if (!Track_NeighbourCrosses(o, t, Effect_PointOnSide(o, 0, 1, x, y), false)) return;
        F<int32_t>(t, 0x253) = id;
        if (!mutual) return;
        id = F<int32_t>(t, 0x202);
        mutual = 0;
        t = o;
        if (id == -1) {
            F<int32_t>(t, 0x253) = -1;
            return;
        }
    }
}
RT3_REPLACE(0x48e820, Track_PairWith);

// 0x48eb80. Reverses track piece t: swaps its two end links (+0x206 / +0x20a),
// flips the direction bit (+0x201 bit 1) when it has a third link, re-sets the
// corners in reverse order (full rebuild, matching neighbours) and swaps the two
// halves of every attached scenery record (+0x244 list, next at +0xe).
RT3_NOINLINE void __fastcall Track_Reverse(uint8_t* t) {
    uint32_t v[4][3];
    Effect_GetVectors(t, 0, v[0], v[1], v[2], v[3]);
    int32_t a = F<int32_t>(t, 0x206);
    F<int32_t>(t, 0x206) = F<int32_t>(t, 0x20a);
    F<int32_t>(t, 0x20a) = a;
    if (F<int32_t>(t, 0x20e) != -1) t[0x201] ^= 2;
    Track_Reshape_Impl(t, 0, v[2], v[3], v[0], v[1], 1, 1, 0);
    for (uint8_t* s = F<uint8_t*>(t, 0x244); s; s = F<uint8_t*>(s, 0xe)) {
        uint32_t tmp[6];
        memcpy(tmp, s + 0x12, 12);
        memcpy(s + 0x12, s + 0x36, 12);
        memcpy(s + 0x36, tmp, 12);
        memcpy(tmp, s + 0x1e, 12);
        memcpy(s + 0x1e, s + 0x2a, 12);
        memcpy(s + 0x2a, tmp, 12);
    }
}
RT3_REPLACE(0x48eb80, Track_Reverse);

// 0x48ec70. Track piece t without a piece kind (and while the network isn't
// frozen, +0xed): redraws (method 6, when shown) and becomes an end piece
// (0xb, model 0x5cb198 for a crossing, else 0x5cb1ac), turned around when
// `flip`.
RT3_NOINLINE void __fastcall Track_MakeEnd(uint8_t* t, int, int32_t flip) {
    if (F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed) || F<int32_t>(t, 0x226)) return;
    if (F<int32_t>(t, 8)) ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)t)[6])(t, 0, 1);
    float h;
    if (flip) h = (float)((double)Vehicle_ScreenHeading(t, 0, 1) + Fl_207(0x5c8760));
    else h = Vehicle_ScreenHeading(t, 0, 1);
    float b = (float)Effect_Blend(t, 0, flip ? 0.0f : 1.0f);
    uint32_t hb;
    memcpy(&hb, &h, 4);
    Track_SetMode(t, 0, 0xb, (const char*)(t[0x218] == 4 ? 0x5cb198 : 0x5cb1ac), hb, 0, b, 1, -1, -1, 0, 0);
}
RT3_REPLACE(0x48ec70, Track_MakeEnd);

// 0x48ed30. Track piece t: deletes its attached model (+0x248) from its model
// list (+0x8, destroyed once empty). Whether it was found.
RT3_NOINLINE int32_t __fastcall Track_DropModel(uint8_t* t) {
    if (!F<void*>(t, 0x248)) return 0;
    int32_t found = 0;
    if (!F<PtrList*>(t, 8)) return 0;
    PtrList_Rewind(F<PtrList*>(t, 8));
    for (uint8_t* m = (uint8_t*)PtrList_CurrentValue(F<PtrList*>(t, 8)); m && !found; m = (uint8_t*)PtrList_CurrentValue(F<PtrList*>(t, 8))) {
        if (m != F<uint8_t*>(t, 0x248)) {
            PtrList_Next(F<PtrList*>(t, 8));
            continue;
        }
        F<void*>(t, 0x248) = nullptr;
        SceneObj_Teardown(m);
        NamedObj_Destructor(m);
        crt::operator_delete(m);
        PtrList_RemoveCurrent(F<PtrList*>(t, 8));
        found = 1;
    }
    PtrList* l = F<PtrList*>(t, 8);
    if (F<int32_t>(l, 0xc)) return found;
    PtrList_Clear(l);
    if (PtrList* d = F<PtrList*>(t, 8)) {
        PtrList_Destroy(d);
        crt::operator_delete(d);
    }
    F<void*>(t, 8) = nullptr;
    return found;
}
RT3_REPLACE(0x48ed30, Track_DropModel);

// 0x48edf0. Moves a car `dist` along the track from position `pos` (0..1) on
// this node in direction *dir (0: towards +0x206, 1: towards +0x20a), crossing
// nodes as needed: at a route end with a junction the car's train picks the
// branch towards its order target (front car) or follows the front car (rear
// car `front` == 0). Writes the new position (*outPos, the point *x / *z),
// counts the nodes crossed (*count) and adds the distance moved (*accum).
// Returns the node id it stops on, -1 at a track end or off plain track.
RT3_NOINLINE int32_t __fastcall Node_Advance(uint8_t* n, int, float pos, float dist, float* x, float* z, float* outPos, int32_t* dir, int32_t* count,
                                             float* accum, int32_t car, int32_t, int32_t front) {
    Collection* nodes = g_tracks;
    for (;;) {
        if (*dir) {
            double rem = D(0x5c97e0) - pos;
            if (dist < rem) {
                float np = (float)((double)pos + dist);
                *outPos = np;
                Vehicle_PointAlong(n, 0, np, x, z);
                if (accum) *accum = (float)((double)dist + *accum);
                return F<int32_t>(n, 0x202);
            }
            *outPos = (float)((double)dist - rem);
            if (accum) *accum = (float)((double)*accum + rem);
        } else {
            if (dist <= pos) {
                float np = (float)((double)pos - dist);
                *outPos = np;
                Vehicle_PointAlong(n, 0, np, x, z);
                if (accum) *accum = (float)((double)dist + *accum);
                return F<int32_t>(n, 0x202);
            }
            *outPos = (float)((double)dist - pos);
            if (accum) *accum = (float)((double)pos + *accum);
        }
        bool back = *dir == 0;
        int32_t exitLink = back ? F<int32_t>(n, 0x206) : F<int32_t>(n, 0x20a);
        if (exitLink == -1) return -1;
        uint8_t* next = (uint8_t*)Collection_Get(nodes, 0, (uint32_t)exitLink);
        if (next[0x216] != 2) return -1;
        if (car != -1 && Node_IsRouteEnd(n) && F<int32_t>(n, 0x20e) != -1) {
            uint8_t* c = (uint8_t*)Collection_Get(g_62ba84, 0, (uint32_t)car);
            int32_t target = Train_OrderTarget((uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(c, 0x390)), 0, nullptr, 0);
            int32_t linkA = back ? 0x206 : 0x20a, linkB = back ? 0x20a : 0x206;
            if (front) {
                uint8_t* r = Node_NextTowards(n, 0, target, F<int32_t>(c, 0x34e), F<int32_t>(n, linkA), (uint8_t*)Obj_Train390(c), 0);
                if (r) next = r;
            } else {
                int32_t target2 = Train_CurrentOrderNode((uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(c, 0x390)));
                int32_t follow = F<int32_t>(c, 0x352);
                uint8_t* r = Node_NextTowards(n, 0, target2, follow, F<int32_t>(n, linkB), (uint8_t*)Obj_Train390(c), 0);
                if (r) {
                    if (target2 == target) {
                        if (F<int32_t>(n, 0x206) == F<int32_t>(r, 0x202) && Collection_IsValid(nodes, 0, F<uint32_t>(n, 0x20a)))
                            r = (uint8_t*)Obj_Node20a(n);
                        else if (F<int32_t>(n, 0x20a) == F<int32_t>(r, 0x202) && Collection_IsValid(nodes, 0, F<uint32_t>(n, 0x206)))
                            r = (uint8_t*)Obj_Node206(n);
                    }
                    if (r) next = r;
                }
            }
        }
        int32_t id = F<int32_t>(n, 0x202);
        uint32_t newPos;
        if (back) {
            newPos = 0x3eb95810;
            if (F<int32_t>(next, 0x206) == id || (F<int32_t>(next, 0x20e) == id && (next[0x201] & 2))) {
                *dir = 1;
                newPos = 0;
            }
        } else {
            newPos = 0;
            if (F<int32_t>(next, 0x20a) == id || (F<int32_t>(next, 0x20e) == id && !(next[0x201] & 2))) {
                *dir = 0;
                newPos = 0x3eb95810;
            }
        }
        if (count) ++*count;
        dist = *outPos;
        pos = BitsF(newPos);
        n = next;
    }
}
RT3_REPLACE(0x48edf0, Node_Advance);

// 0x48f290. Whether the tile is blocked (computed on first use).
uint32_t __fastcall Tile_IsBlocked(uint8_t* o) {
    if (!(o[0x24c] & 1)) Tile_UpdateBlocked(o);
    return (o[0x24c] >> 1) & 1;
}
RT3_REPLACE(0x48f290, Tile_IsBlocked);

// 0x48f4c0. Track piece t: sets the signal state +0x218 (1 on, 2 open, 4
// crossing...) with the "on" flag +0x44 = `on` (unless both are unchanged;
// a crossing of variant 3 never), crediting the owner company (+0x22e) when
// a crossing goes, telling the vehicles (`notify`, not for an open piece
// with +0x244), recounting junction pieces (+0x216 == 2) and rebuilding the
// geometry. A crossing that loses a side becomes an end. With `spread` the
// change runs along the piece (Track_Propagate) and to its neighbours. The
// old state.
RT3_NOINLINE uint8_t __fastcall Track_SetState(uint8_t* t, int, uint8_t state, int32_t on, int32_t notify, int32_t spread) {
    uint8_t old = t[0x218];
    if (old == state && (t[0x44] != 0) == (on != 0)) return old;
    if (state == 4 && t[0x219] == 3) return old;
    int32_t wasOn = t[0x44] != 0;
    if (Collection_IsValid(g_companies, 0, F<uint16_t>(t, 0x22e)) && t[0x218] == 4 && state != 4)
        Company_Add7680(Collection_Get(g_companies, 0, F<uint16_t>(t, 0x22e)), 0, 1);
    if (notify && !(state == 2 && F<int32_t>(t, 0x244)))
        Vehicles_PushState(g_tracks, 0, F<uint32_t>(t, 0x202), state);
    if (t[0x216] == 2) Obj_CountTrack(hook::As<uint8_t*>(g_tracks), 0, t, 0);
    Effect_SetStyle(t, 0, state, (uint8_t)on);
    t[0x218] = state;
    Track_BuildGeometry(t);
    if (t[0x216] == 2) {
        Obj_CountTrack(hook::As<uint8_t*>(g_tracks), 0, t, 1);
        Trains_Clear1a3(g_trains);
    }
    if (old == 1 && state == 4 && (F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x20a) == -1)) Track_MakeEnd(t, 0, F<int32_t>(t, 0x206) == -1);
    if (!spread) return old;
    auto vis = [](uint8_t* n) { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)n)[5])(n); };
    bool go = (old == 1 && state == 4) || (!wasOn && on == 1);
    if (go && (vis(t) || F<int32_t>(t, 0x226) == 1 || F<int32_t>(t, 0x226) == 3))
        Track_Propagate(g_tracks, 0, t, state, on, old, notify);
    if (!go || vis(t)) return old;
    uint8_t* n = (uint8_t*)Obj_Node206(t);
    if (n && vis(n) && ((n[0x218] == 1 && state == 4) || (n[0x44] && on == 1))) {
        Track_SetState(n, 0, state, on, 1, 0);
        Track_Propagate(g_tracks, 0, n, state, on, old, notify);
    }
    n = (uint8_t*)Obj_Node20a(t);
    if (n && vis(n) && ((n[0x218] == 1 && state == 4) || (n[0x44] && on == 1))) {
        Track_SetState(n, 0, state, on, 1, 0);
        Track_Propagate(g_tracks, 0, n, state, on, old, notify);
    }
    return old;
}
RT3_REPLACE(0x48f4c0, Track_SetState);

// 0x48f740. Moves track piece t's ends to a..d: skipped for a fully linked piece
// unless `force` (re-registering it in the node grid around the move when placed
// or the table is live), then drops it onto the terrain, refreshes its lighting
// and with `force` matches the neighbours. `unused` is never read.
void __fastcall Track_MoveEnds(uint8_t* t, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d,
                               int32_t unused, int32_t force) {
    if (force || F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x20a) == -1) {
        uint8_t* nodes = hook::As<uint8_t*>(g_tracks);
        if (t[0x216] || F<int32_t>(nodes, 0xe9)) Map_RemoveTrackRef(nodes, 0, t);
        Track_SetEnds(t, 0, a, b, c, d);
        nodes = hook::As<uint8_t*>(g_tracks);
        if (t[0x216] || F<int32_t>(nodes, 0xe9)) Grid_AddObject(nodes, 0, t);
    }
    Track_DropAndSettle(t, 0, 0x40400000, 1);
    Obj_UpdateHeightLight(t);
    if (force) Track_MatchNeighbourHeights(t);
}
RT3_REPLACE(0x48f740, Track_MoveEnds);

// 0x48f8e0. Track piece t: links neighbour id on the side whose corners
// (a / b: +0x206, c / d: +0x20a) meet one of id's. A free side (or one
// already linked to id) takes it - the +0x206 side redraws a new piece
// (method 6, unless frozen, +0xed) and an end piece is made of a piece with
// one link; a taken side makes id the branch +0x20e (side in +0x201 bit 1)
// and lifts id's touching corners to t's. 0 when nothing meets or the
// branch is taken.
RT3_NOINLINE int32_t __fastcall Track_Connect(uint8_t* t, int, int32_t id) {
    float v[4][3], w[4][3];
    Effect_GetVectors(t, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3]);
    uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)id);
    Effect_GetVectors(n, 0, (uint32_t*)w[0], (uint32_t*)w[1], (uint32_t*)w[2], (uint32_t*)w[3]);
    auto meets = [&](const float* p) {
        for (int k = 0; k < 4; ++k)
            if (Track_CornersMeet(p, w[k])) return true;
        return false;
    };
    // Lifts the pair of n's corners that s1 meets to s1 / s2.
    auto fit = [&](const float* s1, const float* s2) {
        for (int k = 0; k < 4; ++k)
            if (Track_CornersMeet(s1, w[k])) {
                int pair = k & ~1;
                w[pair][1] = (k & 1) ? s2[1] : s1[1];
                w[pair + 1][1] = (k & 1) ? s1[1] : s2[1];
                break;
            }
        ((void(__fastcall*)(uint8_t*, int, const float*, const float*, const float*, const float*, int32_t, int32_t, int32_t))(void*)&Track_Reshape)(
            n, 0, w[0], w[1], w[2], w[3], 1, 0, 0);
    };
    if (meets(v[0]) || meets(v[1])) {
        if (F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x206) == id) {
            F<int32_t>(t, 0x206) = id;
            if (!F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed) && !F<int32_t>(t, 0x226) && F<int32_t>(t, 8))
                ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)t)[6])(t, 0, 1);
            if (F<int32_t>(t, 0x20a) == -1) Track_MakeEnd(t, 0, 0);
            return 1;
        }
        if (F<int32_t>(t, 0x20e) != -1) return 0;
        F<int32_t>(t, 0x20e) = id;
        t[0x201] |= 2;
        fit(v[0], v[1]);
        return 1;
    }
    if (!meets(v[2]) && !meets(v[3])) return 0;
    if (F<int32_t>(t, 0x20a) == -1 || F<int32_t>(t, 0x20a) == id) {
        F<int32_t>(t, 0x20a) = id;
        Track_Refresh(t);
        if (F<int32_t>(t, 0x206) == -1) Track_MakeEnd(t, 0, 1);
        return 1;
    }
    if (F<int32_t>(t, 0x20e) != -1) return 0;
    F<int32_t>(t, 0x20e) = id;
    t[0x201] &= 0xfd;
    fit(v[2], v[3]);
    return 1;
}
RT3_REPLACE(0x48f8e0, Track_Connect);

// 0x490200. Track piece t: settles its state from its neighbours A (+0x206),
// B (+0x20a) and the branch C (+0x20e, joining A's side with +0x201 bit 1,
// else B's): each side's state, a both-way side (5) opening the neighbour
// that leads into it; equal sides set t to that state, differing ones open t
// (a 1 / 4 pair also reversing it). An open t then opens through to an open
// A (or B) when both are junction-aware (+0x216), carrying C along.
RT3_NOINLINE void __fastcall Track_Resolve(uint8_t* t, int, int32_t on) {
    uint8_t* C = F<int32_t>(t, 0x20e) != -1 ? (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x20e)) : nullptr;
    uint8_t sa = 0, sb = 0, want = 0, got = 0;
    uint8_t* A = nullptr;
    uint8_t* B = nullptr;
    if (F<int32_t>(t, 0x206) != -1) {
        A = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x206));
        if (A && (!t[0x216] || A[0x216])) sa = Track_FacingState(A, t);
    }
    if (F<int32_t>(t, 0x20a) != -1) {
        B = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x20a));
        if (B && (!t[0x216] || B[0x216])) sb = Track_FacingState(B, t);
    }
    if (C && (!t[0x216] || C[0x216])) {
        uint8_t v = Track_FacingState(C, t);
        if (t[0x201] & 2) sa |= v;
        else sb |= v;
    }
    auto open = [&](uint8_t* n, uint32_t side) {
        Track_SetState(n, 0, 2, n[0x44] != 0, on, 1);
        if (F<int32_t>(n, side) == F<int32_t>(t, 0x202)) Track_Reverse(n);
    };
    bool checkB = true;
    if (!sa) {
        if (!sb) return;
        want = t[0x218] == 2 ? 4 : t[0x218];
    } else if (t[0x218] == sa) {
        want = t[0x218];
    } else if (sa == 1 || sa == 4) {
        want = sa;
    } else if (sa == 5) {
        if (sb == 1) {
            want = 1;
            if (A[0x218] == 4) open(A, 0x206);
            else open(C, 0x206);
            checkB = false;
        } else {
            want = 4;
            if (A[0x218] == 1) open(A, 0x20a);
            else open(C, 0x20a);
        }
    }
    if (checkB && !sb) {
        got = t[0x218] == 2 ? 1 : t[0x218];
    } else if (t[0x218] == sb) {
        got = t[0x218];
    } else if (sb == 1 || sb == 4) {
        got = sb;
    } else if (sb == 5) {
        if (sa == 1) {
            got = 1;
            if (B[0x218] == 4) open(B, 0x206);
            else open(C, 0x206);
        } else {
            got = 4;
            if (B[0x218] == 1) open(B, 0x20a);
            else open(C, 0x20a);
        }
    }
    if (want == got) {
        if (want != t[0x218]) Track_SetState(t, 0, want, t[0x44] != 0, on, 1);
    } else {
        Track_SetState(t, 0, 2, t[0x44] != 0, 0, 1);
        if (want == 1 && got == 4) {
            Track_Reverse(t);
            uint8_t* s = A;
            A = B;
            B = s;
        }
    }
    if (t[0x218] != 2) return;
    if (A && A[0x218] == 2) {
        if (!t[0x216] || !A[0x216]) return;
        Track_SetState(t, 0, 1, t[0x44] != 0, on, 1);
        Track_SetState(A, 0, 1, A[0x44] != 0, on, 1);
        if (!C || !(t[0x201] & 2)) return;
        if (C[0x218] == 2) Track_SetState(C, 0, 1, C[0x44] != 0, on, 1);
        else if (C[0x218] == 4) open(C, 0x20a);
        return;
    }
    if (!B || B[0x218] != 2 || !t[0x216] || !B[0x216]) return;
    Track_SetState(t, 0, 4, t[0x44] != 0, on, 1);
    Track_SetState(B, 0, 4, B[0x44] != 0, on, 1);
    if (!C || (t[0x201] & 2)) return;
    if (C[0x218] == 2) Track_SetState(C, 0, 4, C[0x44] != 0, on, 1);
    else if (C[0x218] == 1) open(C, 0x206);
}
RT3_REPLACE(0x490200, Track_Resolve);

// 0x490850. Track piece t: drops neighbour id. A main link (+0x206 / +0x20a)
// is taken over by the branch +0x20e on its side (+0x201 bit 1 for +0x206),
// re-merging the piece's route; a piece left with one link becomes an end.
// 1 when id was a neighbour.
RT3_NOINLINE int32_t __fastcall Track_Unlink(uint8_t* t, int, int32_t id) {
    if (F<int32_t>(t, 0x206) == id) {
        int32_t branch = F<int32_t>(t, 0x20e);
        F<int32_t>(t, 0x206) = -1;
        if (branch != -1 && (t[0x201] & 2)) {
            F<int32_t>(t, 0x206) = branch;
            F<int32_t>(t, 0x20e) = -1;
            Routes_TryMergeAt(g_routes, 0, F<uint32_t>(t, 0x212));
        }
        if (F<int32_t>(t, 0x20a) != -1 && F<int32_t>(t, 0x206) == -1) Track_MakeEnd(t, 0, 1);
        return 1;
    }
    if (F<int32_t>(t, 0x20a) == id) {
        int32_t branch = F<int32_t>(t, 0x20e);
        F<int32_t>(t, 0x20a) = -1;
        if (branch != -1 && !(t[0x201] & 2)) {
            F<int32_t>(t, 0x20a) = branch;
            F<int32_t>(t, 0x20e) = -1;
            Routes_TryMergeAt(g_routes, 0, F<uint32_t>(t, 0x212));
        }
        if (F<int32_t>(t, 0x206) != -1 && F<int32_t>(t, 0x20a) == -1) Track_MakeEnd(t, 0, 0);
        return 1;
    }
    if (F<int32_t>(t, 0x20e) == id) {
        F<int32_t>(t, 0x20e) = -1;
        Routes_TryMergeAt(g_routes, 0, F<uint32_t>(t, 0x212));
        return 1;
    }
    return 0;
}
RT3_REPLACE(0x490850, Track_Unlink);

// 0x490960. Track piece t: switches to piece kind `kind` (below 4 also
// storing the variant bytes +0x219 / +0x251 and the flag bit 5 of +0x24c,
// redrawing via method 6) and, with a model, adds that model (a 0x5cfd00
// world model, turned by angles a / b and lifted to y) to the group, kept in
// +0x248 when `keep`. A crossing (state 4) being given the 0x5cafac model
// first turns on (state 1) and re-resolves both neighbours.
RT3_NOINLINE void __fastcall Track_SetMode(uint8_t* t, int, int32_t kind, const char* model, uint32_t a, uint32_t b, float y, int32_t, int32_t v219, int32_t v251,
                              int32_t bit5, int32_t keep) {
    if (t[0x218] == 4 && model && !memcmp((const void*)0x5cafac, model, 0x19)) {
        Track_SetState(t, 0, 1, t[0x44] != 0, 1, 1);
        if (F<int32_t>(t, 0x206) != -1)
            Track_Resolve((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x206)), 0, 1);
        if (F<int32_t>(t, 0x20a) != -1)
            Track_Resolve((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x20a)), 0, 1);
    }
    if (kind < 4) {
        ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)t)[6])(t, 0, 0);
        t[0x219] = (uint8_t)v219;
        t[0x251] = (uint8_t)v251;
        t[0x24c] ^= (uint8_t)((((uint8_t)bit5 << 5) ^ t[0x24c]) & 0x20);
        F<int32_t>(t, 0x226) = kind;
        t[0x201] |= 0xc;
    }
    if (!model) return;
    uint8_t* o = (uint8_t*)crt::operator_new(0x23a);
    if (o) {
        NamedObj_Construct(o);
        F<uint32_t>(o, 0) = 0x5cfd00;
    }
    WorldModel_Init(o, 0, (const char*)0x5c87a8, model, (const char*)0x5cfd74, 0xbf800000, 0xbf800000, 0);
    Group_Add(t, 0, o, 0.0f);
    Group_SetAngles(t, 0, a, b);
    Group_MoveAllToHeight(t, 0, y);
    if (keep) F<uint8_t*>(t, 0x248) = o;
}
RT3_REPLACE(0x490960, Track_SetMode);

// 0x490ac0. Initialises track piece t (as Track_Init, the exe's own code) as a
// square of half-size 0.181 cells around cell (cx, cy) turned by -angle.
void __fastcall Track_InitRotated(uint8_t* t, int, int32_t id, int32_t group, float cx, float cy, float angle, int32_t kind,
                                  int32_t owner, int32_t style) {
    angle = -angle;
    float c = (float)crt::sin((double)angle);
    float s = (float)crt::cos((double)angle);
    double s1 = (double)s * D(0x5cfde0), c1 = (double)c * D(0x5cfde0);
    double c2 = (double)c * D(0x5c97d8), s2 = (double)s * D(0x5c97d8);
    float x0 = (float)((s1 - c1) + cx), z0 = (float)((c1 + s1) + cy);
    float x1 = (float)((s1 - c2) + cx), z1 = (float)((s2 + c1) + cy);
    float x2 = (float)((s2 - c2) + cx), z2 = (float)((c2 + s2) + cy);
    float x3 = (float)((s2 - c1) + cx), z3 = (float)((c2 + s1) + cy);
    ((void(__fastcall*)(uint8_t*, int, int32_t, int32_t, float, float, float, float, float, float, float, float, int32_t, int32_t,
                        int32_t))0x48f2b0)(t, 0, id, group, x0, z0, x1, z1, x2, z2, x3, z3, kind, owner, style);
}
RT3_REPLACE(0x490ac0, Track_InitRotated);

// 0x490bf0. Removes placed track piece t (+0x201 bit 0) from the world:
// unpairs it, uncounts it (built track, +0x216 == 2) and clears its coarse map
// byte (unless 0x6cfca8+0xed), frees its list, books its cost back (+0x230,
// stats 0xf / 0xc) for a company build (+0x22e) in a running game, unmarks its
// cells. Its route (+0x212 in 0x6cfcb4) is split first if it does not end at
// the piece (unless +0xf1), loses a node and a reference, moves its end to a
// same-route neighbour when the piece was one, and goes when unreferenced
// (+0x10). The neighbours (+0x206 / +0x20a / +0x20e) drop it, the piece's
// vtbl[6](0) runs and trains drop it as a waypoint. Then the effect is
// destroyed (really only when it was placed).
RT3_NOINLINE void __fastcall Track_Remove(uint8_t* t) {
    Collection* pieces = g_tracks;
    Collection* routes = g_routes;
    if (t[0x201] & 1) {
        Track_PairWith(t, 0, -1, 1);
        if (t[0x216] == 2) Obj_CountTrack(hook::As<uint8_t*>(g_tracks), 0, t, 0);
        if (!F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed)) {
            float fx = (float)(((double)F<float>(t, 0x2c) + F<float>(t, 0x14)) / Fl_207(0x5cfcfc));
            float fy = (float)(((double)F<float>(t, 0x34) + F<float>(t, 0x1c)) / Fl_207(0x5cfcfc));
            int32_t y = Ftol(fy);
            Map_SetCoarseByte(hook::As<void*>(g_map), 0, Ftol(fx), y, 0);
        }
        Obj_FreeList244(t);
        if (t[0x216] == 2 && F<uint16_t>(t, 0x22e) && hook::As<uint32_t>(g_companies) && !g_newGame[0x82]) {
            float cost = -(float)(double)F<uint32_t>(t, 0x230);
            Company_AddStat(Node_Company(t), 0, 0xf, cost);
            Company_PostStat(Node_Company(t), 0, 0xc, cost, 0);
        }
        if (t[0x216] == 1 || t[0x216] == 2) Track_UnmarkCells(t, 0, 1);
        uint8_t* route = nullptr;
        if (Collection_IsValid(routes, 0, F<uint32_t>(t, 0x212)) && F<int32_t>(t, 0x212) != -1) route = (uint8_t*)Collection_Get(routes, 0, F<uint32_t>(t, 0x212));
        bool keep = true;
        if (!F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xf1)) {
            if (!route) keep = false;
            else if (!Pair_Contains(route, 0, F<int32_t>(t, 0x202))) {
                Routes_SplitAtNode(routes, 0, t, 0);
                route = (uint8_t*)Collection_Get(routes, 0, F<uint32_t>(t, 0x212));
            }
        }
        if (keep && route) {
            --F<int32_t>(route, 0x14);
            Routes_AddRef(routes, 0, F<uint32_t>(t, 0x212), -1);
            if (Pair_Contains(route, 0, F<int32_t>(t, 0x202))) {
                auto sameRoute = [&](int32_t id) { return F<int32_t>(Collection_Get(pieces, 0, (uint32_t)id), 0x212) == F<int32_t>(t, 0x212); };
                int32_t a = F<int32_t>(t, 0x206);
                if (a != -1 && F<int32_t>(t, 0x20a) != -1 && sameRoute(a) && sameRoute(F<int32_t>(t, 0x20a))) {
                    if (Pair_Contains(Collection_Get(routes, 0, F<uint32_t>(t, 0x212)), 0, F<int32_t>(t, 0x206)))
                        Route_ReplaceEnd(route, 0, F<int32_t>(t, 0x20a), F<int32_t>(t, 0x202));
                    else
                        Route_ReplaceEnd(route, 0, F<int32_t>(t, 0x206), F<int32_t>(t, 0x202));
                } else if (F<int32_t>(t, 0x206) != -1 && sameRoute(F<int32_t>(t, 0x206))) {
                    Route_ReplaceEnd(route, 0, F<int32_t>(t, 0x206), F<int32_t>(t, 0x202));
                } else if (F<int32_t>(t, 0x20a) != -1 && sameRoute(F<int32_t>(t, 0x20a))) {
                    Route_ReplaceEnd(route, 0, F<int32_t>(t, 0x20a), F<int32_t>(t, 0x202));
                }
            }
            if (!F<int32_t>(route, 0x10)) routes->vtbl->release(routes, F<uint32_t>(t, 0x212));
        }
        static const int32_t kLinks[] = { 0x206, 0x20a, 0x20e };
        for (int32_t off : kLinks)
            if (F<int32_t>(t, off) != -1) Track_Unlink((uint8_t*)Collection_Get(pieces, 0, F<uint32_t>(t, off)), 0, F<int32_t>(t, 0x202));
        F<int32_t>(t, 0x206) = -1;
        F<int32_t>(t, 0x20a) = -1;
        F<int32_t>(t, 0x20e) = -1;
        (*(void(__thiscall**)(uint8_t*, int32_t))(F<uint8_t*>(t, 0) + 0x18))(t, 0);
        if (t[0x216] == 2 && g_trains) Trains_DropWaypoint(g_trains, 0, F<int32_t>(t, 0x202));
    }
    Effect_Destroy(t, 0, t[0x201] & 1);
}
RT3_REPLACE(0x490bf0, Track_Remove);

// 0x490f80. Re-places track piece t as a square of half-size 0.181 cells around
// (cx, cy), turned by `angle`: unless fully linked its ends are moved there
// (re-registering it in the node grid when placed or the table is live); then it
// is dropped onto the terrain, relit, its neighbours matched and dropped again.
// The original leaves the corner heights uninitialized (zero here): the drop
// overwrites them before anything reads them.
void __fastcall Track_PlaceRotated(uint8_t* t, int, float cx, float cy, float angle) {
    float c = (float)crt::sin((double)angle);
    float s = (float)crt::cos((double)angle);
    Engine_Get1C(hook::As<void*>(g_engine));
    double s1 = (double)s * D(0x5cfde0), c1 = (double)c * D(0x5cfde0);
    double c2 = (double)c * D(0x5c97d8), s2 = (double)s * D(0x5c97d8);
    const double k = D(0x5c8c48);
    float v[4][3] = {};
    v[0][0] = (float)(((s1 - c1) + cx) * k);
    v[0][2] = (float)(((c1 + s1) + cy) * k);
    v[1][0] = (float)(((s1 - c2) + cx) * k);
    v[1][2] = (float)(((s2 + c1) + cy) * k);
    v[2][0] = (float)(((s2 - c2) + cx) * k);
    v[2][2] = (float)(((c2 + s2) + cy) * k);
    v[3][0] = (float)(((s2 - c1) + cx) * k);
    v[3][2] = (float)(((c2 + s1) + cy) * k);
    if (F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x20a) == -1) {
        uint8_t* nodes = hook::As<uint8_t*>(g_tracks);
        if (t[0x216] || F<int32_t>(nodes, 0xe9)) Map_RemoveTrackRef(nodes, 0, t);
        Track_SetEnds(t, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3]);
        nodes = hook::As<uint8_t*>(g_tracks);
        if (t[0x216] || F<int32_t>(nodes, 0xe9)) Grid_AddObject(nodes, 0, t);
    }
    Track_DropAndSettle(t, 0, 0x40400000, 1);
    Obj_UpdateHeightLight(t);
    Track_MatchNeighbourHeights(t);
    Track_DropAndSettle(t, 0, 0x40400000, 1);
}
RT3_REPLACE(0x490f80, Track_PlaceRotated);

// 0x491140. Track piece t: links neighbour id and, when that worked,
// re-resolves its signals (telling the vehicles). Whether it linked.
RT3_NOINLINE int32_t __fastcall Track_ConnectAndResolve(uint8_t* t, int, int32_t id) {
    int32_t r = Track_Connect(t, 0, id);
    if (r) Track_Resolve(t, 0, 1);
    return r;
}
RT3_REPLACE(0x491140, Track_ConnectAndResolve);

// 0x491170. Electrified track piece t (+0x44) without a catenary yet (+0x248)
// and without variants: adds the catenary pole model "%cCatenary%cT_body"
// (R / L by which neighbour leaves its route - none: `side` - and S / D for
// a single / crossing piece) at the piece's mean corner height, turned with
// the piece, and keeps it.
RT3_NOINLINE void __fastcall Track_AddCatenary(uint8_t* t, int, int32_t side) {
    if (!t[0x44] || F<void*>(t, 0x248) || t[0x219] != 0xff || t[0x251] != 0xff) return;
    int32_t left;
    if (F<int32_t>(t, 0x206) != -1 &&
        F<uint32_t>(Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x206)), 0x212) != F<uint32_t>(t, 0x212))
        left = 0;
    else if (F<int32_t>(t, 0x20a) != -1 &&
             F<uint32_t>(Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x20a)), 0x212) != F<uint32_t>(t, 0x212))
        left = 1;
    else
        left = side;
    char model[0x30];
    crt::sprintf(model, (const char*)0x5cfde8, left ? 0x4c : 0x52, t[0x218] != 4 ? 0x53 : 0x44);
    float y = (float)(((((double)F<float>(t, 0x30) + F<float>(t, 0x24)) + F<float>(t, 0x3c)) + F<float>(t, 0x18)) / D(0x5c87b8) - D(0x5c8770));
    y = (float)(((double)F<float>(t, 0x18) - F<float>(t, 0x3c)) * D(0x5c8628) + y);
    float a = (float)((double)Vehicle_ScreenHeading(t, 0, 1) + Fl_207(0x5c8760));
    uint32_t ab;
    memcpy(&ab, &a, 4);
    Track_SetMode(t, 0, 0xe, model, ab, 0, y, 1, -1, -1, 0, 1);
    float h = Vehicle_ScreenHeading(t, 0, 1);
    uint32_t hb;
    memcpy(&hb, &h, 4);
    SceneObj_SetAngles(F<uint8_t*>(t, 0x248), 0, 0, hb, 0);
}
RT3_REPLACE(0x491170, Track_AddCatenary);

// 0x4912d0. Track piece t: restores the newest undo record (+0x244, a list
// via +0xe): state +0x218, placement +0x216, stamp +0x240, +0x230, +0x217,
// the flag bit 4 of +0x24c and the corner shape. Then re-resolves it and
// its neighbours as needed (signals on a placed piece, bridges, special
// pieces), credits back the company for a double track undone (state 4 ->
// 1), restyles, re-adds the catenary or drops the pole model, rebuilds the
// geometry and remerges its route. Frees the record.
RT3_NOINLINE void __fastcall Track_PopUndo(uint8_t* t) {
    uint8_t* rec = F<uint8_t*>(t, 0x244);
    if (!rec) return;
    F<uint8_t*>(t, 0x244) = F<uint8_t*>(rec, 0xe);
    uint8_t old = t[0x218];
    t[0x218] = rec[4];
    t[0x216] = rec[5];
    F<uint32_t>(t, 0x240) = F<uint32_t>(rec, 0);
    F<uint32_t>(t, 0x230) = F<uint32_t>(rec, 0xa);
    t[0x217] = rec[8];
    t[0x24c] ^= (t[0x24c] ^ (uint8_t)(rec[9] << 4)) & 0x10;
    ((void(__fastcall*)(uint8_t*, int, const uint8_t*, const uint8_t*, const uint8_t*, const uint8_t*, int32_t, int32_t, int32_t))(void*)&Track_Reshape)(
        t, 0, rec + 0x12, rec + 0x1e, rec + 0x2a, rec + 0x36, 1, 0, 0);
    if (t[0x44] && !rec[6] && t[0x216] && Node_IsRouteEnd(t))
        ((uint8_t*)Collection_Get(g_routes, 0, F<uint32_t>(t, 0x212)))[0x1c] = 0;
    if (t[0x218] == 2) Track_Resolve(t, 0, 0);
    auto at = [&](uint32_t side) -> uint8_t* {
        if (F<int32_t>(t, side) == -1 || !Collection_Get(g_tracks, 0, F<uint32_t>(t, side))) return nullptr;
        return F<int32_t>(t, side) == -1 ? nullptr : (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, side));
    };
    if (F<int32_t>(t, 0x226) == 1) {
        if (uint8_t* n = at(0x206))
            if (F<int32_t>(n, 0x226) != 1) Track_Resolve((uint8_t*)Obj_Node206(t), 0, 0);
        if (uint8_t* n = at(0x20a))
            if (F<int32_t>(n, 0x226) != 1) Track_Resolve((uint8_t*)Obj_Node20a(t), 0, 0);
    }
    if (((int32_t(__fastcall*)(uint8_t*))(*(void***)t)[5])(t)) {
        if (uint8_t* n = at(0x206))
            if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)n)[5])(n)) Track_Resolve((uint8_t*)Obj_Node206(t), 0, 0);
        if (uint8_t* n = at(0x20a))
            if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)n)[5])(n)) Track_Resolve((uint8_t*)Obj_Node20a(t), 0, 0);
    }
    if (Collection_IsValid(g_companies, 0, F<uint16_t>(t, 0x22e)) && t[0x218] == 1 && old == 4)
        Company_Add7680(Collection_Get(g_companies, 0, F<uint16_t>(t, 0x22e)), 0, 1);
    Effect_SetStyle(t, 0, t[0x218], rec[6]);
    if (old == 4 && t[0x218] == 1 && F<int32_t>(t, 0x226)) TrackNet_IdleModels(g_tracks, 0, t);
    if (rec[7]) Track_AddCatenary(t, 0, 1);
    else if (F<void*>(t, 0x248)) Track_DropModel(t);
    Track_BuildGeometry(t);
    if (t[0x216] && Node_IsRouteEnd(t)) Routes_TryMergeAt(g_routes, 0, F<uint32_t>(t, 0x212));
    crt::free(rec);
}
RT3_REPLACE(0x4912d0, Track_PopUndo);

// 0x4915e0. Track piece t: switches its power on (+0x44, unless already on
// or a variant-3 crossing) - as Track_SetState with the same state and
// on = 1, telling the vehicles when `notify` - then (with `catenary`) adds
// the catenary on `side`, updates a route end's flag and rebuilds the
// geometry and colour.
RT3_NOINLINE void __fastcall Track_Electrify(uint8_t* t, int, int32_t catenary, int32_t notify, int32_t side) {
    uint8_t s = t[0x218];
    if (!t[0x44] && !(s == 4 && t[0x219] == 3)) {
        uint8_t old = s;
        int32_t wasOn = t[0x44] != 0;
        if (Collection_IsValid(g_companies, 0, F<uint16_t>(t, 0x22e)) && t[0x218] == 4 && s != 4)
            Company_Add7680(Collection_Get(g_companies, 0, F<uint16_t>(t, 0x22e)), 0, 1);
        if (notify && !(s == 2 && F<int32_t>(t, 0x244))) Vehicles_PushState(g_tracks, 0, F<uint32_t>(t, 0x202), s);
        if (t[0x216] == 2) Obj_CountTrack(hook::As<uint8_t*>(g_tracks), 0, t, 0);
        Effect_SetStyle(t, 0, s, 1);
        t[0x218] = s;
        Track_BuildGeometry(t);
        if (t[0x216] == 2) {
            Obj_CountTrack(hook::As<uint8_t*>(g_tracks), 0, t, 1);
            Trains_Clear1a3(g_trains);
        }
        auto vis = [](uint8_t* n) { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)n)[5])(n); };
        bool go = old == 1 && s == 4;
        if (go) {
            if (F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x20a) == -1) Track_MakeEnd(t, 0, F<int32_t>(t, 0x206) == -1);
        }
        if (go || !wasOn) {
            if (vis(t) || F<int32_t>(t, 0x226) == 1 || F<int32_t>(t, 0x226) == 3) Track_Propagate(g_tracks, 0, t, s, 1, old, notify);
            if (!vis(t)) {
                for (int32_t k = 0; k < 2; ++k) {
                    uint8_t* n = (uint8_t*)(k ? Obj_Node20a(t) : Obj_Node206(t));
                    if (!n || !vis(n)) continue;
                    if (!((n[0x218] == 1 && s == 4) || n[0x44])) continue;
                    Track_SetState(n, 0, s, 1, 1, 0);
                    Track_Propagate(g_tracks, 0, n, s, 1, old, notify);
                }
            }
        }
    }
    if (catenary) Track_AddCatenary(t, 0, side);
    if (F<int32_t>(t, 0x212) != -1 && Node_IsRouteEnd(t))
        Route_UpdateFlag((uint8_t*)Collection_Get(g_routes, 0, F<uint32_t>(t, 0x212)));
    Track_BuildGeometry(t);
    Track_UpdateColor(t);
}
RT3_REPLACE(0x4915e0, Track_Electrify);

RT3_NOINLINE void __fastcall Obj_DecA0(void* o) {
    if (--F<int32_t>(o, 0xa0) < 0) F<int32_t>(o, 0xa0) = 0;
}
RT3_REPLACE(0x491880, Obj_DecA0);

void __fastcall Obj_DecA4(uint8_t* o) {
    if (--F<int32_t>(o, 0xa4) < 0) F<int32_t>(o, 0xa4) = 0;
}
RT3_REPLACE(0x4918a0, Obj_DecA4);

void __fastcall Obj_DecAc(uint8_t* o) {
    if (--F<int32_t>(o, 0xac) < 0) F<int32_t>(o, 0xac) = 0;
}
RT3_REPLACE(0x4918c0, Obj_DecAc);

// 0x4918e0. Speed/cost factor for a route step from tile (x1, z1) to (x2, z2) (world
// units, height h1): the slope to the terrain height at the end over the distance,
// from 0.7 (falling) through 1.0 to 4.2 (steep climb).
RT3_NOINLINE float __stdcall Route_GradeFactor(float x2, float z2, float x1, float h1, float z1) {
    float dist = (float)Distance2D(x1, z1, (float)((double)x2 * Fl(0x5c8700)), (float)((double)z2 * Fl(0x5c8700)));
    float h = (float)Terrain_HeightAtTile(Map(), 0, x2, z2);
    float s = (float)Distance2D(0.0f, h1, dist, h);
    static const uint32_t lo[] = { 0x5ca06c, 0x5ca068, 0x5c86a0 };
    static const uint32_t loV[] = { 0x3f333333, 0x3f4ccccd, 0x3f666666 };
    for (int32_t i = 0; i < 3; ++i)
        if (s <= Fl(lo[i])) return *(const float*)&loV[i];
    if (s <= Fl(0x5cfe10)) return 1.0f;
    static const uint32_t hi[] = { 0x5ca198, 0x5cfe0c, 0x5cfe08, 0x5cfe04, 0x5ca1c8, 0x5c85b0, 0x5cfe00, 0x5ca1c4 };
    static const uint32_t hiV[] = { 0x3f8ccccd, 0x3fa00000, 0x3fb9999a, 0x3fd9999a, 0x3ff9999a, 0x40133333, 0x40266666, 0x40400000 };
    for (int32_t i = 0; i < 8; ++i)
        if (s <= Fl(hi[i])) return *(const float*)&hiV[i];
    return s <= Fl(0x5ca1c0) ? 3.5f : 4.2f;
}
RT3_REPLACE(0x4918e0, Route_GradeFactor);

// 0x491af0. Track collection constructor (vtbl 0x5cfe18, heap records
// 1000 + 500) with a zeroed column array of `columns` entries (+0x8c).
RT3_NOINLINE Collection* __fastcall Tracks_Construct(Collection* c, int, int32_t columns, int32_t) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5cfe18;
    Collection_Init(c, 0, 0, 0, 1000, 500, 0, 0, 0);
    static const uint32_t kZero[] = { 0x98, 0x90, 0x94, 0xfe, 0x102, 0xa0, 0xa4, 0xa8, 0xac, 0x9c, 0xb0, 0xb4, 0xb8,
                                      0xbc, 0xc0, 0xc4, 0xc8, 0xcc, 0xd0, 0xd4, 0xe5, 0xe9, 0xed, 0xf1 };
    for (uint32_t off : kZero) F<int32_t>(c, off) = 0;
    F<uint8_t>(c, 0xf5) = 0;
    F<int32_t>(c, 0x106) = 0;
    F<uint8_t>(c, 0x10a) = 0;
    F<int32_t>(c, 0x88) = -1;
    F<uint32_t>(c, 0x206) = 0xbf800000;  // -1.0f
    for (int k = 0; k < 13; ++k) F<uint8_t>(c, 0x10b + k) = 0;
    F<int32_t>(c, 0x8c) = 0;
    uint8_t* cols = (uint8_t*)crt::malloc(columns * 4);
    F<uint8_t*>(c, 0x8c) = cols;
    for (int32_t k = 0; k < columns * 4; ++k) cols[k] = 0;
    return c;
}
RT3_REPLACE(0x491af0, Tracks_Construct);

// 0x491c20. Rebuilds the geometry of every piece in the collection.
RT3_NOINLINE void __fastcall Tracks_RebuildGeometry(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Track_BuildGeometry((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x491c20, Tracks_RebuildGeometry);

// 0x491c60. Saves a collection of containers: tag 0x38a5, the collection itself,
// tag 0x38a6, the word +0x90, each container, tag 0x38a7.
RT3_NOINLINE void __fastcall ContainerList_Save(Collection* c, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x38a5);
    Collection_Save(c, 0, s, recordSize);
    StreamWriteU32(s, 0x38a6);
    StreamWrite(s, (uint8_t*)c + 0x90, 4, 0);
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Container_Save((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0)), 0, s);
    StreamWriteU32(s, 0x38a7);
}
RT3_REPLACE(0x491c60, ContainerList_Save);

// 0x491cf0. Pushes a new undo state (+0x90 += 2) onto vehicle `id`; the first
// state is remembered in +0x88.
RT3_NOINLINE void __fastcall Vehicles_PushState(Collection* c, int, uint32_t id, uint8_t mode) {
    if (!Collection_IsValid(c, 0, id)) return;
    uint8_t* v = (uint8_t*)Collection_Get(c, 0, id);
    uint8_t* cc = (uint8_t*)c;
    F<int32_t>(cc, 0x90) += 2;
    Vehicle_PushState(v, 0, F<uint32_t>(cc, 0x90), mode);
    if (F<int32_t>(cc, 0x88) == -1) F<uint32_t>(cc, 0x88) = F<uint32_t>(v, 0x240);
}
RT3_REPLACE(0x491cf0, Vehicles_PushState);

// 0x491d50. Copies the cursor fields (+0x118..+0x125) into out and whether the
// selected record (+0x125) has byte +0x216 clear.
void __fastcall Table_FillCursor(Collection* t, int, uint8_t* out) {
    F<uint32_t>(out, 6) = F<uint32_t>(t, 0x118);
    F<uint32_t>(out, 0xa) = F<uint32_t>(t, 0x11d);
    F<uint32_t>(out, 0xe) = F<uint32_t>(t, 0x121);
    F<uint32_t>(out, 0x12) = F<uint32_t>(t, 0x125);
    F<int32_t>(out, 0x2e) = 0;
    if (!Collection_IsValid(t, 0, F<uint32_t>(t, 0x125))) return;
    F<int32_t>(out, 0x2e) = F<uint8_t>(Collection_Get(t, 0, F<uint32_t>(t, 0x125)), 0x216) == 0 ? 1 : 0;
}
RT3_REPLACE(0x491d50, Table_FillCursor);

// 0x491dc0. Track network net: links pieces a and b both ways, then
// re-resolves each (notifying per na / nb). 0 when either link failed.
RT3_NOINLINE int32_t __fastcall TrackNet_LinkPair(Collection* net, int, uint32_t a, uint32_t b, int32_t na, int32_t nb) {
    if (!Track_Connect((uint8_t*)Collection_Get(net, 0, a), 0, (int32_t)b)) return 0;
    if (!Track_Connect((uint8_t*)Collection_Get(net, 0, b), 0, (int32_t)a)) return 0;
    Track_Resolve((uint8_t*)Collection_Get(net, 0, a), 0, na);
    Track_Resolve((uint8_t*)Collection_Get(net, 0, b), 0, nb);
    return 1;
}
RT3_REPLACE(0x491dc0, TrackNet_LinkPair);

// 0x491e30. Track_MoveEnds on piece `id` of the collection (forced, with neighbours),
// if it exists.
RT3_NOINLINE void __fastcall Tracks_MoveEnds(Collection* c, int, uint32_t id, const uint32_t* a, const uint32_t* b, const uint32_t* d,
                                const uint32_t* e) {
    if (uint8_t* t = (uint8_t*)Collection_Get(c, 0, id)) Track_MoveEnds(t, 0, a, b, d, e, 1, 1);
}
RT3_REPLACE(0x491e30, Tracks_MoveEnds);

// 0x491e60. Adds object t (id +0x202) to the grid's column list for its cell
// (+0x8c: per x a y-sorted chain of {y, ids, next} 10-byte cells, ids a chain of
// {id, next}).
RT3_NOINLINE void __fastcall Grid_AddObject(uint8_t* g, int, uint8_t* t) {
    float fx, fy;
    Obj_CenterCell(t, 0, &fx, &fy);
    int32_t ix = (int32_t)fx;
    uint16_t iy = (uint16_t)(int32_t)fy;
    uint8_t** column = &F<uint8_t**>(g, 0x8c)[(uint16_t)ix];
    uint8_t* cur = *column;
    uint32_t* node = (uint32_t*)crt::operator_new(8);
    node[0] = F<uint32_t>(t, 0x202);
    node[1] = 0;
    if (!*column) {
        uint8_t* c = (uint8_t*)crt::operator_new(10);
        *column = c;
        F<uint16_t>(*column, 0) = iy;
        F<uint32_t>(*column, 6) = 0;
        F<uint32_t*>(*column, 2) = node;
        return;
    }
    uint8_t* prev = nullptr;
    for (; cur; prev = cur, cur = F<uint8_t*>(cur, 6)) {
        uint16_t y = F<uint16_t>(cur, 0);
        if (y == iy) {
            uint32_t* l = F<uint32_t*>(cur, 2);
            if (l) {
                while (l[1]) l = (uint32_t*)l[1];
                l[1] = (uint32_t)node;
            } else {
                F<uint32_t*>(cur, 2) = node;
            }
            return;
        }
        if (y > iy) {
            uint8_t* c = (uint8_t*)crt::operator_new(10);
            F<uint32_t*>(c, 2) = node;
            F<uint16_t>(c, 0) = iy;
            F<uint8_t*>(c, 6) = cur;
            if (prev) F<uint8_t*>(prev, 6) = c;
            else *column = c;
            return;
        }
    }
    uint8_t* c = (uint8_t*)crt::operator_new(10);
    F<uint32_t*>(c, 2) = node;
    F<uint16_t>(c, 0) = iy;
    F<uint32_t>(c, 6) = 0;
    if (prev) F<uint8_t*>(prev, 6) = c;
}
RT3_REPLACE(0x491e60, Grid_AddObject);

// 0x491fd0. Removes track piece t (+0x202) from the per-cell lists +0x8c
// (column -> u16-sorted cells -> id list); 1 if removed.
RT3_NOINLINE int32_t __fastcall Map_RemoveTrackRef(uint8_t* o, int, uint8_t* t) {
    float x, y;
    Obj_CenterCell(t, 0, &x, &y);
    int32_t xi = (int32_t)x;
    uint16_t yi = (uint16_t)(int32_t)y;
    uint8_t* e = F<uint8_t**>(o, 0x8c)[(uint16_t)xi];
    for (; e; e = F<uint8_t*>(e, 6)) {
        uint16_t key = F<uint16_t>(e, 0);
        if (key > yi) return 0;
        if (key == yi) break;
    }
    if (!e) return 0;
    uint8_t* prev = nullptr;
    for (uint8_t* n = F<uint8_t*>(e, 2); n; prev = n, n = F<uint8_t*>(n, 4)) {
        if (F<int32_t>(n, 0) != F<int32_t>(t, 0x202)) continue;
        if (prev) F<uint8_t*>(prev, 4) = F<uint8_t*>(n, 4);
        else F<uint8_t*>(e, 2) = F<uint8_t*>(n, 4);
        crt::operator_delete(n);
        return 1;
    }
    return 0;
}
RT3_REPLACE(0x491fd0, Map_RemoveTrackRef);

// 0x492090. Frees the per-column chains +0x8c (map width entries; each node
// +6 next, +2 sub-list with +4 next), then the column array.
RT3_NOINLINE void __fastcall Obj_FreeColumns(uint8_t* o) {
    int32_t w = F<int32_t>(g_map, 0x2155);
    auto free53b = [](void* p) { crt::operator_delete(p); };
    for (int32_t i = 0; i < w; ++i) {
        for (uint8_t* e = F<uint8_t**>(o, 0x8c)[i]; e; ) {
            uint8_t* next = F<uint8_t*>(e, 6);
            for (uint8_t* s = F<uint8_t*>(e, 2); s; ) {
                uint8_t* sn = F<uint8_t*>(s, 4);
                free53b(s);
                s = sn;
            }
            free53b(e);
            e = next;
        }
        F<uint8_t**>(o, 0x8c)[i] = nullptr;
    }
    crt::free(F<void*>(o, 0x8c));
    F<void*>(o, 0x8c) = nullptr;
}
RT3_REPLACE(0x492090, Obj_FreeColumns);

// 0x492130. Entry for cell (x, y) in per-column lists sorted by y (+0x8c; next at +6).
RT3_NOINLINE void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y) {
    uint8_t** columns = F<uint8_t**>(o, 0x8c);
    if (!columns) return nullptr;
    uint8_t* map = g_map;
    if ((int32_t)x >= F<int32_t>(map, 0x15d9) - 1 || (int32_t)y >= F<int32_t>(map, 0x15d5) - 1) return nullptr;
    for (uint8_t* n = columns[x]; n; n = *(uint8_t**)(n + 6)) {
        uint16_t k = *(uint16_t*)n;
        if (k > y) return nullptr;
        if (k == y) return n;
    }
    return nullptr;
}
RT3_REPLACE(0x492130, Obj_CellEntry);

// 0x4921a0. Nearest track node to (px, py) listed in grid cell (cx, cy) of node
// table g, not `exclude` (nor, with `skipLinked`, linked to it within 8 steps);
// with `byDegree` the distance is weighted by the node's link count. Stale entries
// are dropped on the way (unlinking the list head, as the original does). Updates
// *best and *second; returns the node id or -1.
int32_t __fastcall Grid_NearestNode(Collection* g, int, float px, float py, int32_t cx, int32_t cy, float* second,
                                    int32_t exclude, int32_t byDegree, float* best, int32_t skipLinked) {
    uint8_t* m = g_map;
    if (cx < 0 || cx >= F<int32_t>(m, 0x15d9) - 1 || cy < 0 || cy >= F<int32_t>(m, 0x15d5) - 1) return -1;
    uint8_t* e = (uint8_t*)Obj_CellEntry(g, 0, (uint16_t)cx, (uint16_t)cy);
    if (!e) return -1;
    int32_t found = -1;
    for (uint32_t* n = F<uint32_t*>(e, 2); n; n = (uint32_t*)n[1]) {
        int32_t id = (int32_t)n[0];
        if (id != exclude) {
            if (!Collection_IsValid(g, 0, (uint32_t)id)) {
                F<uint32_t*>(e, 2) = (uint32_t*)F<uint32_t*>(e, 2)[1];
                crt::free(n);
                n = F<uint32_t*>(e, 2);
                if (!n) break;
                continue;
            }
            uint8_t* o = (uint8_t*)Collection_Get(g, 0, (uint32_t)id);
            if (skipLinked && exclude != -1 && Track_LinksWithin(o, 0, exclude, 8, -1)) continue;
            float x, y;
            Obj_CenterCell(o, 0, &x, &y);
            float dist = (float)Distance2D(x, y, px, py);
            if (byDegree) {
                int32_t k = 0;
                if (F<int32_t>(o, 0x206) != -1) k = 1;
                if (F<int32_t>(o, 0x20a) != -1) ++k;
                if (F<int32_t>(o, 0x20e) != -1) ++k;
                double nd = k;
                dist = (float)((nd + D(0x5c8698)) / D(0x5c8698) * dist + nd * D(0x5cfe28));
            }
            if (dist < *best) {
                found = id;
                *second = *second < dist ? *second : dist;
                *best = dist;
            }
        }
    }
    return found;
}
RT3_REPLACE(0x4921a0, Grid_NearestNode);

// 0x492360. Re-marks the map under every track piece listed in the grid blocks
// around fine cell (fx, fy); stale entries are freed (only the list head is
// unlinked, as in the original).
void __fastcall Nodes_RemarkAround(Collection* nodes, int, int32_t fx, int32_t fy) {
    uint8_t* m = g_map;
    int32_t x0 = (fx - 1 < 0 ? 0 : fx - 1) / 4;
    int32_t xm = F<int32_t>(m, 0x2155) * 4 - 4;
    int32_t x1 = (xm < fx + 1 ? xm : fx + 1) / 4;
    int32_t y0 = (fy - 1 < 0 ? 0 : fy - 1) / 4;
    int32_t ym = F<int32_t>(m, 0x2159) * 4 - 4;
    int32_t y1 = (ym < fy + 1 ? ym : fy + 1) / 4;
    for (int32_t x = x0; x <= x1; ++x)
        for (int32_t y = y0; y <= y1; ++y) {
            uint8_t* e = (uint8_t*)Obj_CellEntry(nodes, 0, (uint16_t)x, (uint16_t)y);
            if (!e) continue;
            for (uint32_t* n = F<uint32_t*>(e, 2); n;) {
                if (Collection_IsValid(nodes, 0, n[0])) {
                    Track_MarkCells((uint8_t*)Collection_Get(nodes, 0, n[0]));
                    n = (uint32_t*)n[1];
                } else {
                    if (n == F<uint32_t*>(e, 2)) F<uint32_t*>(e, 2) = (uint32_t*)n[1];
                    uint32_t* dead = n;
                    n = (uint32_t*)n[1];
                    crt::operator_delete(dead);
                }
            }
        }
}
RT3_REPLACE(0x492360, Nodes_RemarkAround);

// 0x4924a0. The object of the collection nearest to (x, y) among those listed in
// the grid block of (x, y) and in the same fine cell, skipping ids skip1 / skip2
// and, with `freeOnly`, occupied ones (+0x226); -1 if none or the cell is not flagged.
RT3_NOINLINE int32_t __fastcall Grid_NearestObject(Collection* c, int, float x, float y, int32_t skip1, int32_t skip2, int32_t freeOnly) {
    int32_t blockX, blockY;
    __asm {
        fld y
        fdiv dword ptr ds:[0x5c8568]
        mov eax, 0x5a10d0
        call eax
        mov blockY, eax
        fld x
        fdiv dword ptr ds:[0x5c8568]
        mov eax, 0x5a10d0
        call eax
        mov blockX, eax
    }
    uint8_t* e = (uint8_t*)Obj_CellEntry(c, 0, (uint16_t)blockX, (uint16_t)blockY);
    if (!e) return -1;
    int32_t iy = (int32_t)y, ix = (int32_t)x;
    if (!Map_CellFlag(hook::As<void*>(g_map), 0, ix, iy, 1)) return -1;
    int32_t best = -1;
    float bestD = 9999999.0f;
    for (const uint32_t* n = F<const uint32_t*>(e, 2); n; n = (const uint32_t*)n[1]) {
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, n[0]);
        if (!t) continue;
        if (freeOnly && F<int32_t>(t, 0x226)) continue;
        if ((int32_t)n[0] == skip1 || (int32_t)n[0] == skip2) continue;
        float tx, ty;
        Obj_CenterCell(t, 0, &tx, &ty);
        tx = (float)((double)tx * *(const float*)0x5c8568);
        ty = (float)((double)ty * *(const float*)0x5c8568);
        if ((int32_t)tx != ix || (int32_t)ty != iy) continue;
        float d = (float)DistanceSq2D(tx, ty, x, y);
        if (d < bestD) {
            best = (int32_t)n[0];
            bestD = d;
        }
    }
    return best;
}
RT3_REPLACE(0x4924a0, Grid_NearestObject);

// 0x492610. Finds the clearance for a vehicle on track piece p leaving away from
// `from`: tries heights (k x a + b) x c for k = 0..3, following the pieces (via
// +0x206/+0x20a) until a piece's terrain rises above the track height minus it. A
// bridge end (kind 3) or a blocked piece within 20 steps ends the search early.
// Returns 0 if a junction (+0x20e) is met or no height above the minimum fits.
int32_t __stdcall Track_FindClearance(uint8_t* p, int32_t from, float* height) {
    uint32_t a[3], b[3], c[3], d[3];
    float result = -1.0f;
    Effect_GetVectors(p, 0, a, b, c, d);
    int32_t next = F<int32_t>(p, 0x206);
    float h0;
    if (next == from) {
        next = F<int32_t>(p, 0x20a);
        h0 = (float)(((double)*(float*)&d[1] + *(float*)&c[1]) / D(0x5c85d0));
    } else h0 = (float)(((double)*(float*)&b[1] + *(float*)&a[1]) / D(0x5c85d0));
    for (int32_t k = 0; k < 4; ++k) {
        *height = (float)(((double)k * D(0x5c8758) + D(0x5c8770)) * D(0x5c8d68));
        int32_t cur = next, prev = F<int32_t>(p, 0x202), steps = 0;
        while (cur != -1) {
            uint8_t* q = (uint8_t*)Collection_Get(g_tracks, 0, cur);
            ++steps;
            bool hit;
            int32_t after;
            if (F<int32_t>(q, 0x226) == 3) hit = true;
            else {
                if (F<int32_t>(q, 0x20e) != -1) return 0;
                float lim = (float)((double)h0 - *height);
                Effect_GetVectors(q, 0, a, b, c, d);
                float x, z;
                bool rear = F<int32_t>(q, 0x206) == prev;
                if (rear) Vehicle_RearMid(q, 0, &x, &z);
                else Vehicle_FrontMid(q, 0, &x, &z);
                float k7 = Fl(0x5c8700);
                hit = Terrain_HeightAt(Map(), 0, (float)((double)x * k7), (float)((double)z * k7)) > lim;
                after = rear ? F<int32_t>(q, 0x20a) : F<int32_t>(q, 0x206);
            }
            if (hit) {
                if (steps < 0x14) return 1;
                result = *height;
                break;
            }
            prev = cur;
            cur = after;
        }
    }
    if (!(result > D(0x5c8608))) return 0;
    *height = result;
    return 1;
}
RT3_REPLACE(0x492610, Track_FindClearance);

// 0x492840. Track network net: grades the run leaving piece t away from
// `from`. Starting at t's end height, each following piece (pass +0x106 as
// its mark) is set level, dropping by `rise` per piece after the first, but
// stops on the ground: a piece whose middle would sit below the terrain is
// raised to it and its next neighbour's shared end follows. Pieces claimed
// by another pass (+0x24d) - or, in multiplayer, placed ones unless `force`
// - end the run. Reaching a bridge or tunnel piece (kind 1 / 3) instead ramps
// evenly from t's height to that piece's and joins it. A powered first piece
// gets its catenary (dropping a stale pole model).
RT3_NOINLINE void __fastcall TrackNet_GradeRun(Collection* net, int, uint8_t* t, int32_t from, float rise, uint8_t force) {
    int32_t prev = F<int32_t>(t, 0x202);
    int32_t gen = ++F<int32_t>(net, 0x106);
    int32_t count = 0;
    TrackVec a, b, c, d, e, f, g, h;
    Effect_GetVectors(t, 0, a.u, b.u, c.u, d.u);
    int32_t first = F<int32_t>(t, 0x206);
    float h0;
    if (first == from) {
        first = F<int32_t>(t, 0x20a);
        h0 = (float)(((double)d.f[1] + c.f[1]) / D(0x5c85d0));
    } else
        h0 = (float)(((double)b.f[1] + a.f[1]) / D(0x5c85d0));
    float level = h0;
    for (int32_t cur = first; cur != -1;) {
        ++count;
        uint8_t* p = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)cur);
        int32_t kind = F<int32_t>(p, 0x226);
        if (kind == 3 || kind == 1) {
            int32_t next = F<int32_t>(t, 0x206);
            if (next == from) next = F<int32_t>(t, 0x20a);
            int32_t pr = F<int32_t>(t, 0x202);
            TrackVec xa, xb, xc, xd;
            Effect_GetVectors(p, 0, xa.u, xb.u, xc.u, xd.u);
            float y = F<int32_t>(p, 0x206) == pr ? (float)(((double)xb.f[1] + xa.f[1]) / D(0x5c85d0)) : (float)(((double)xd.f[1] + xc.f[1]) / D(0x5c85d0));
            float n = (float)count;
            float step = (float)(((double)y - h0) / n);
            if (next == -1) return;
            float acc = step;
            for (int32_t q = next; q != -1;) {
                uint8_t* s = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)q);
                int32_t k = F<int32_t>(s, 0x226);
                if (k == 3 || k == 1) {
                    uint8_t* r = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)pr);
                    TrackVec sa;
                    Effect_GetVectors(s, 0, sa.u, nullptr, nullptr, nullptr);
                    Effect_GetVectors(r, 0, a.u, b.u, c.u, d.u);
                    if (F<int32_t>(r, 0x206) == q) a.u[1] = b.u[1] = sa.u[1];
                    else c.u[1] = d.u[1] = sa.u[1];
                    ReshapeVecs(r, a, b, c, d);
                    return;
                }
                Effect_GetVectors(s, 0, a.u, b.u, c.u, d.u);
                float v = (float)((double)acc + h0);
                a.f[1] = b.f[1] = c.f[1] = d.f[1] = v;
                ReshapeVecs(s, a, b, c, d);
                Track_MarkSpecial(s, 0, gen);
                acc = (float)((double)acc + step);
                int32_t nx = F<int32_t>(s, 0x206);
                if (nx == pr) nx = F<int32_t>(s, 0x20a);
                pr = q;
                q = nx;
            }
            return;
        }
        if (F<int32_t>(p, 0x24d) && F<int32_t>(p, 0x24d) != gen) break;
        if (IsMultiplayer() && p[0x216] && !force) break;
        if (count > 1) level = (float)((double)level - rise);
        Effect_GetVectors(p, 0, a.u, b.u, c.u, d.u);
        float x, z;
        int32_t next;
        if (F<int32_t>(p, 0x206) == prev) {
            Vehicle_RearMid(p, 0, &x, &z);
            double ground = Terrain_HeightAt(g_map, 0, (float)((double)x * Fl_207(0x5c8700)), (float)((double)z * Fl_207(0x5c8700)));
            if (ground > (double)level) {
                b.f[1] = (float)((double)level + rise);
                a.u[1] = b.u[1];
                ReshapeVecs(p, a, b, c, d);
                Track_MarkSpecial(p, 0, gen);
                if (F<int32_t>(p, 0x20a) == -1) break;
                uint8_t* n2 = (uint8_t*)Collection_Get(net, 0, F<uint32_t>(p, 0x20a));
                Effect_GetVectors(n2, 0, e.u, f.u, g.u, h.u);
                if (F<int32_t>(n2, 0x206) == cur) ReshapeVecs(n2, d, c, g, h);
                else if (F<int32_t>(n2, 0x20a) == cur) ReshapeVecs(n2, e, f, d, c);
                break;
            }
            if (count > 1) {
                b.f[1] = (float)((double)level + rise);
                a.u[1] = b.u[1];
            } else
                a.f[1] = b.f[1] = level;
            d.f[1] = c.f[1] = level;
            next = F<int32_t>(p, 0x20a);
        } else {
            Vehicle_FrontMid(p, 0, &x, &z);
            double ground = Terrain_HeightAt(g_map, 0, (float)((double)x * Fl_207(0x5c8700)), (float)((double)z * Fl_207(0x5c8700)));
            if (ground > (double)level) {
                d.f[1] = (float)((double)level + rise);
                c.u[1] = d.u[1];
                ReshapeVecs(p, a, b, c, d);
                Track_MarkSpecial(p, 0, gen);
                if (F<int32_t>(p, 0x206) == -1) break;
                uint8_t* n2 = (uint8_t*)Collection_Get(net, 0, F<uint32_t>(p, 0x206));
                Effect_GetVectors(n2, 0, h.u, g.u, f.u, e.u);
                if (F<int32_t>(n2, 0x206) == cur) ReshapeVecs(n2, a, b, f, e);
                else if (F<int32_t>(n2, 0x20a) == cur) ReshapeVecs(n2, h, g, b, a);
                break;
            }
            a.f[1] = b.f[1] = level;
            if (count > 1) {
                d.f[1] = (float)((double)level + rise);
                c.u[1] = d.u[1];
            } else
                c.f[1] = d.f[1] = level;
            next = F<int32_t>(p, 0x206);
        }
        if (count < 1) Track_MarkSpecial(p, 0, gen);
        else Track_MarkSpecial13(p, 0, gen);
        ReshapeVecs(p, a, b, c, d);
        prev = cur;
        cur = next;
    }
    uint8_t* q = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)first);
    if (!q[0x44]) return;
    if (F<void*>(q, 0x248)) Track_DropModel(q);
    Track_AddCatenary(q, 0, 0);
}
RT3_REPLACE(0x492840, TrackNet_GradeRun);

// 0x492ec0. Whether the junction at node id turns left: the angle from its
// diverging neighbour to the node is below the angle to the straight
// neighbour (+0x20e). 1 for nodes without +0x20e.
int32_t __fastcall Node_TurnsLeft(Collection* nodes, int, uint32_t id) {
    uint8_t* n = (uint8_t*)Collection_Get(nodes, 0, id);
    if (F<int32_t>(n, 0x20e) == -1) return 1;
    uint8_t* a = (uint8_t*)Collection_Get(nodes, 0, F<uint32_t>(n, 0x20e));
    uint8_t* b = (uint8_t*)Collection_Get(nodes, 0, (n[0x201] & 2) ? F<uint32_t>(n, 0x206) : F<uint32_t>(n, 0x20a));
    float nx, ny, ax, ay, bx, by;
    Obj_CenterCell(n, 0, &nx, &ny);
    Obj_CenterCell(a, 0, &ax, &ay);
    Obj_CenterCell(b, 0, &bx, &by);
    float toNode = (float)AngleOf(bx, by, nx, ny);
    float toA = (float)AngleOf(bx, by, ax, ay);
    float d = (float)AngleDelta(nullptr, 0, toNode, toA);
    return (double)d < *(const double*)0x5c8608 ? 1 : 0;
}
RT3_REPLACE(0x492ec0, Node_TurnsLeft);

// 0x492fc0. Colours every track piece by slope.
void __fastcall Tracks_ColorSlope(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Track_ColorSlope((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x492fc0, Tracks_ColorSlope);

// 0x493000. Tints every object of table t: the heat colour for 1 when its group
// (+0x22e) is group, else for 0.
void __fastcall Objects_HighlightGroup(Collection* t, int, int32_t group) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* o = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        float on = (float)(F<uint16_t>(o, 0x22e) == group ? 1 : 0);
        uint32_t c = HeatColor(on);
        Obj_SetRgb(o, 0, (uint8_t)(c >> 16), (uint8_t)(c >> 8), (uint8_t)c);
    }
}
RT3_REPLACE(0x493000, Objects_HighlightGroup);

// 0x493080. Colours every track piece by wear relative to the most worn (at least
// k, else 12).
void __fastcall Tracks_ColorWear(Collection* c) {
    float top = 0.0f;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        float v = F<float>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0x22a);
        if (!(top > v)) top = F<float>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0x22a);
    }
    if (Fl(0x5c8788) > top) top = 12.0f;
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Track_ColorWear((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, top);
}
RT3_REPLACE(0x493080, Tracks_ColorWear);

// 0x493150. Recolours every track piece.
RT3_NOINLINE void __fastcall Tracks_UpdateColor(Collection* c) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        Track_UpdateColor((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
}
RT3_REPLACE(0x493150, Tracks_UpdateColor);

// 0x493190. Decays every track piece's wear +0x22a by 0.97.
void __fastcall Table_DecayWear(Collection* t) {
    const float k = *(const float*)0x5cfe14;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        F<float>(r, 0x22a) = (float)((double)F<float>(r, 0x22a) * k);
    }
}
RT3_REPLACE(0x493190, Table_DecayWear);

// 0x4931e0. Debug validation (option +0x1cb): per-record hook and a lookup of
// every track piece referenced from the cell lists. No effects.
void __fastcall Track_Validate(Collection* t) {
    if (!F<int32_t>(g_app, 0x1cb)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i) Nop48a780(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)));
    uint8_t* map = g_map;
    for (int32_t x = 0; x < F<int32_t>(map, 0x2155) - 1; ++x) {
        for (int32_t y = 0; y < F<int32_t>(map, 0x2159) - 1; ++y) {
            uint8_t* e = (uint8_t*)Obj_CellEntry(t, 0, (uint16_t)x, (uint16_t)y);
            if (!e) continue;
            for (uint32_t* s = F<uint32_t*>(e, 2); s; s = (uint32_t*)s[1]) Collection_IsValid(g_tracks, 0, s[0]);
            map = g_map;
        }
    }
}
RT3_REPLACE(0x4931e0, Track_Validate);

RT3_NOINLINE void __fastcall Camera_BeginA0(uint8_t* o) { CameraBegin(o, 0xa0); }
RT3_REPLACE(0x4932a0, Camera_BeginA0);

void __fastcall Camera_BeginA4(uint8_t* o) { CameraBegin(o, 0xa4); }
RT3_REPLACE(0x493300, Camera_BeginA4);

// 0x493360. As 0x4932a0 for the second camera pair (+0xa8/+0xac, flag +0xb4).
void __fastcall Camera_BeginAc(uint8_t* o) {
    uint8_t* cam = g_62be68;
    if (!F<int32_t>(o, 0xa8) && !F<int32_t>(o, 0xac))
        F<int32_t>(o, 0xb4) = F<float>(cam, 0x66) < *(const float*)0x5c88e0 ? 1 : 0;
    F<float>(o, 0xb8) = (float)Obj_Float18(hook::As<void*>(g_62be68));
    F<int32_t>(o, 0xac)++;
}
RT3_REPLACE(0x493360, Camera_BeginAc);

// 0x4933c0. Updates the camera zoom flags (+0xb0 / +0xb4, hysteresis around
// 2000) for the active camera actions and stores the camera value +0x18.
RT3_NOINLINE void __fastcall Camera_UpdateZoomFlags(uint8_t* o) {
    float cam66 = F<float>(g_62be68, 0x66);
    const float h = *(const float*)0x5c88e0;
    if (F<int32_t>(o, 0xa0) || F<int32_t>(o, 0xa4)) {
        float k = F<int32_t>(o, 0xb0) ? 1.05f : 0.95f;
        F<int32_t>(o, 0xb0) = (double)k * h > cam66 ? 1 : 0;
    }
    if (F<int32_t>(o, 0xa8) || F<int32_t>(o, 0xac)) {
        cam66 = F<float>(g_62be68, 0x66);
        float k = F<int32_t>(o, 0xb4) ? 1.1f : 0.9f;
        F<int32_t>(o, 0xb4) = (double)k * h > cam66 ? 1 : 0;
    }
    F<float>(o, 0xb8) = (float)Obj_Float18(hook::As<void*>(g_62be68));
}
RT3_REPLACE(0x4933c0, Camera_UpdateZoomFlags);

RT3_NOINLINE int32_t __fastcall Camera_FlagA0(uint8_t* o, int, int32_t) { return CameraFlag(o, 0xa0, 0xb0); }
RT3_REPLACE(0x4934a0, Camera_FlagA0);

RT3_NOINLINE int32_t __fastcall Camera_FlagA4(uint8_t* o, int, int32_t) { return CameraFlag(o, 0xa4, 0xb0); }
RT3_REPLACE(0x4934e0, Camera_FlagA4);

RT3_NOINLINE int32_t __fastcall Camera_FlagA8(uint8_t* o, int, int32_t) { return CameraFlag(o, 0xa8, 0xb4); }
RT3_REPLACE(0x493520, Camera_FlagA8);

RT3_NOINLINE int32_t __fastcall Camera_FlagAc(uint8_t* o, int, int32_t) { return CameraFlag(o, 0xac, 0xb4); }
RT3_REPLACE(0x493560, Camera_FlagAc);

// 0x4935a0. Owner (+0x22e) and flag (+0x44) of the first track piece at cell (x, y); 1 if found.
int32_t __fastcall Map_TrackOwnerAt(void* o, int, int32_t x, int32_t y, int32_t* owner, int32_t* flag) {
    uint8_t* e = (uint8_t*)Obj_CellEntry(o, 0, (uint16_t)x, (uint16_t)y);
    if (!e) return 0;
    uint32_t* list = F<uint32_t*>(e, 2);
    if (!list) return 0;
    uint8_t* t = (uint8_t*)Collection_Get(g_tracks, 0, list[0]);
    if (!t) return 0;
    *owner = F<uint16_t>(t, 0x22e);
    *flag = t[0x44] ? 1 : 0;
    return 1;
}
RT3_REPLACE(0x4935a0, Map_TrackOwnerAt);

// 0x493600. Route_GradeFactor from the camera focus (0x62be68 +0x1c..+0x24), or
// from the stored point (0x5f2b4c, 0x6cd988, 0x6ce6f8) when one is set.
RT3_NOINLINE float __stdcall Route_GradeFactorHere(float x, float z) {
    uint32_t sx = g_5f2b4c;
    if (sx == 0x49741cf0) {
        uint8_t* cam = g_62be68;
        return Route_GradeFactor(x, z, F<float>(cam, 0x1c), F<float>(cam, 0x20), F<float>(cam, 0x24));
    }
    return Route_GradeFactor(x, z, *(float*)&sx, hook::As<float>(g_6cd988), hook::As<float>(g_6ce6f8));
}
RT3_REPLACE(0x493600, Route_GradeFactorHere);

// 0x493660. Adds (or removes) a track piece to this object's counters, its
// company's (0x426c20) and its region's (0x487960).
RT3_NOINLINE void __fastcall Obj_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add) {
    int32_t d = add ? 1 : -1;
    uint8_t kind = t[0x218];
    if (kind == 1) F<int32_t>(o, 0xbc) += d;
    else if (kind == 2) F<int32_t>(o, 0xc4) += d;
    else F<int32_t>(o, 0xc0) += d;
    int32_t cls = F<int32_t>(t, 0x226);
    if (cls == 3) F<int32_t>(o, 0xcc) += d;
    else if (cls == 1) F<int32_t>(o, 0xd0) += d;
    else if (((int32_t(__thiscall*)(void*))(*(void***)t)[0x14 / 4])(t)) F<int32_t>(o, 0xd4) += d;
    if (t[0x44]) F<int32_t>(o, 0xc8) += d;
    if (Node_Company(t)) Company_CountTrack((uint8_t*)Node_Company(t), 0, t, add);
    Collection* regions = g_territories;
    if (!regions) return;
    int32_t y = (int32_t)Obj_CenterCellY(t);
    int32_t x = (int32_t)Obj_CenterCellX(t);
    uint8_t* map = g_map;
    uint32_t r = F<uint8_t*>(map, 0x2131)[F<int32_t>(map, 0x2155) * y + x] >> 3;
    if (!Collection_IsValid(g_territories, 0, r)) return;
    World_CountTrack((uint8_t*)Collection_Get(g_territories, 0, r), 0, t, add);
}
RT3_REPLACE(0x493660, Obj_CountTrack);

RT3_NOINLINE int32_t __fastcall Table_CountFlag201_40(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (F<uint8_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x201) & 0x40) ++n;
    return n;
}
RT3_REPLACE(0x493770, Table_CountFlag201_40);

// 0x4937b0. Clears the highlight of every piece in the collection, last first.
RT3_NOINLINE void __fastcall Tracks_ClearHighlights(Collection* c) {
    for (int32_t i = Collection_Count(c) - 1; i >= 0; --i)
        Track_SetHighlight((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0, 0);
}
RT3_REPLACE(0x4937b0, Tracks_ClearHighlights);

// 0x4937f0. Highlights (or not) track piece `id`; with `follow` while turning it on
// and the piece being of a run kind (+0x226: 1, 2, 3, 12, 13) also highlights the
// pieces along both directions (+0x206, then +0x20a) while they stay of a run kind,
// at most 1000 steps each way.
RT3_NOINLINE void __fastcall Tracks_HighlightRun(Collection* c, int, uint32_t id, int32_t on, int32_t follow) {
    uint8_t* t = (uint8_t*)Collection_Get(c, 0, id);
    Track_SetHighlight(t, 0, on);
    if (!follow || !on || !Track_IsRunKind(F<uint32_t>(t, 0x226))) return;
    for (int dir = 0; dir < 2; ++dir) {
        int32_t cur = F<int32_t>(t, dir ? 0x20a : 0x206), prev = F<int32_t>(t, 0x202);
        for (int32_t steps = 0; cur != -1 && steps < 1000; ++steps) {
            uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)cur);
            int32_t next = F<int32_t>(n, 0x206);
            if (next == -1 || next == prev) {
                next = F<int32_t>(n, 0x20a);
                if (next == -1 || next == prev) next = -1;
            }
            if (next != -1) prev = cur;
            cur = next;
            if (!Track_IsRunKind(F<uint32_t>(n, 0x226))) break;
            Track_SetHighlight(n, 0, 1);
        }
    }
}
RT3_REPLACE(0x4937f0, Tracks_HighlightRun);

// 0x493960. Track network net: electrifies company's track (or, with
// `dryRun`, only prices it): each unpowered piece costs Track_Cost2 (priced
// as if powered); unless dry the pieces are powered, the cost charged
// (Tracks_ApplyUpgrades) and catenary poles added at run ends and on every
// sixth plain piece. Reports the cost (*cost) and the share still unpowered
// (*share, at least 0.01; 0 when all already were).
RT3_NOINLINE void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share) {
    if (!Collection_IsValid(g_companies, 0, company)) return;
    int32_t n = Collection_Count(net);
    float total = 0;
    int32_t owned = 0, powered = 0;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* p = (uint8_t*)Collection_Get(net, 0, Collection_IdAt(net, 0, (uint32_t)i, 0));
        if (F<uint16_t>(p, 0x22e) != company) continue;
        ++owned;
        if (p[0x44]) {
            ++powered;
            continue;
        }
        p[0x44] = 1;
        int32_t c = Track_Cost2(p, 0, company);
        p[0x44] = 0;
        total = (float)((double)(float)c + total);
        if (!dryRun) Track_Electrify(p, 0, 0, 0, 1);
    }
    if (!dryRun) {
        Tracks_ApplyUpgrades(net, 0, company, Ftol((double)total));
        for (int32_t j = 0; j < n; ++j) {
            uint8_t* p = (uint8_t*)Collection_Get(net, 0, Collection_IdAt(net, 0, (uint32_t)j, 0));
            if (F<uint16_t>(p, 0x22e) != company || !p[0x44]) continue;
            if (F<int32_t>(p, 0x206) == -1 || !((uint8_t*)Collection_Get(net, 0, F<uint32_t>(p, 0x206)))[0x44]) {
                Track_AddCatenary(p, 0, 0);
                continue;
            }
            if (F<int32_t>(p, 0x20a) == -1 || !((uint8_t*)Collection_Get(net, 0, F<uint32_t>(p, 0x20a)))[0x44]) {
                Track_AddCatenary(p, 0, 1);
                continue;
            }
            if (((int32_t(__fastcall*)(uint8_t*))(*(void***)p)[5])(p)) continue;
            if (F<int32_t>(p, 0x226) == 1 || F<int32_t>(p, 0x226) == 3) continue;
            if (F<uint32_t>(p, 0x202) % 6) continue;
            Track_AddCatenary(p, 0, 1);
        }
    }
    if (cost) *cost = total;
    if (!share) return;
    if (owned == powered) {
        *(uint32_t*)share = 0;
        return;
    }
    int32_t d = owned >= 1 ? owned : 1;
    float frac = (float)(D(0x5c8770) - (double)(float)powered / (double)(float)d);
    *share = frac;
    if ((double)frac < D(0x5c85b8)) *share = (float)D(0x5c85b8);
}
RT3_REPLACE(0x493960, TrackNet_ElectrifyCompany);

// 0x493ba0. Track collection destructor (vtbl 0x5cfe18).
void __fastcall Tracks_Destruct(Collection* c) {
    F<int32_t>(c, 0xf1)++;
    c->vtbl = (CollectionVtbl*)0x5cfe18;
    Collection_Clear(c);
    Obj_FreeList98((uint8_t*)c);
    Obj_FreeColumns((uint8_t*)c);
    F<int32_t>(c, 0xf1)--;
    Collection_Destruct(c);
}
RT3_REPLACE(0x493ba0, Tracks_Destruct);

// 0x493be0. Track network net: loads from stream s (chunks 0x38a5..0x38a7):
// the pieces, the counter +0x90 (copied to +0x94), then each piece gets the
// track-light vtable and its models, joins the grid when placed (or while
// building, +0xe9) and loses its links. Returns the bytes read.
RT3_NOINLINE int32_t __fastcall TrackNet_Load(Collection* net, int, int32_t s, int32_t recordSize) {
    uint8_t* b = (uint8_t*)net;
    int32_t total = StreamSkip4(s, 0x38a5, 0, 0);
    total += Collection_Load(net, 0, s, recordSize);
    total += StreamSkip4(s, 0x38a6, 0, 0);
    StreamRead(s, b + 0x90, 4, 0, 1);
    F<uint32_t>(b, 0x94) = F<uint32_t>(b, 0x90);
    for (int32_t i = 0; i < Collection_Count(net); ++i) {
        uint8_t* t = (uint8_t*)Collection_Get(net, 0, Collection_IdAt(net, 0, (uint32_t)i, 0));
        uint8_t tmp[0x248];
        TrackLight_Construct(tmp);
        F<uint32_t>(t, 0) = F<uint32_t>(tmp, 0);
        total += Track_LoadModels(t, 0, s);
        if (t[0x216] || F<int32_t>(b, 0xe9)) Grid_AddObject(b, 0, t);
        Node_ClearLinks(t, 0, -1, -1);
        Effect489e80_Release(tmp);
    }
    return StreamSkip4(s, 0x38a7, 0, 0) + total;
}
RT3_REPLACE(0x493be0, TrackNet_Load);

}  // namespace rt3
