// TrackNet.cpp - RT3.exe 0x494000-0x4a6000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "game/calendar.h"
#include "core/crt.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Obj_DecA4(uint8_t* o);
void __fastcall Obj_DecAc(uint8_t* o);
uint32_t __fastcall Obj_CenterRegionType(void* o);
int32_t __fastcall Map_ClampRect(void* m, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
void* __fastcall Obj_CellEntry(void* o, int, uint16_t x, uint16_t y);
void __fastcall Camera_BeginA4(uint8_t* o);
void __fastcall Camera_BeginAc(uint8_t* o);
uint8_t* __fastcall Route_Init(uint8_t* r);
void __fastcall Nop494e80(void*);
int32_t __fastcall Route_SetOwner(uint8_t* r, int, int32_t v);
int32_t __fastcall Route_Add10(uint8_t* r, int, int32_t v);
void* __fastcall Obj_Node206(void* o);
void* __fastcall Obj_Node20a(void* o);
uint32_t __fastcall Obj_CenterGridByte(void* o);
void __fastcall Routes_Recount(Collection* routes, int, int32_t rid);
void __fastcall Route_Upgrade(uint8_t* r, int, int32_t);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
uint32_t __fastcall Routes_Add(Collection* t);
void __fastcall Node_Set212(void* n, int, int32_t v);
void __fastcall Routes_AddRef(Collection* t, int, uint32_t id, int32_t delta);
int32_t __fastcall Route_AddNode(uint8_t* r, int, uint32_t id);
void __fastcall Node_UpdateLightFromNeighbors(uint8_t* n);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
int32_t __fastcall Routes_SplitAt(Collection* routes, int, uint32_t id);
void __fastcall Track_SetKind(uint8_t* o, int, uint8_t kind);
struct Collection;
int32_t IsMultiplayer();
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
int32_t __fastcall Vehicle_CornerBelowGround(uint8_t* o, int, float off);
int32_t __fastcall Track_EndLinksBack(uint8_t* t);
int32_t __fastcall Track_EndBackLink(uint8_t* t);
void Track_Reshape();
void* __fastcall Map_RegionRecordAt(void* m, int, float x, float y);
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
double __stdcall DistanceSq2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_IsSpecial(uint8_t* r, int, int32_t x, int32_t y);
int32_t __stdcall Map_LineOnLand(float x0, float z0, float x1, float z1);
void __fastcall Obj_FreeList1c9(void* o);
uint32_t __fastcall Terrain_RecBit9(uint8_t* r, int, int32_t x, int32_t y);

struct PathNode {
    float x, y;
    PathNode* next;
};

void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
int32_t __fastcall Node_IsRouteEnd(uint8_t* n);

// One link step of Route_FindPlain: the link must be a live node on another route
// (unless `checkRoute` is off: the third link skips that test), plain track
// unless anyTrack, and pass the needFlag / company filters. Costs 1, or 2 (plus
// one foreign) on another owner's node.
static void RoutePlain_Link(Collection* routes, uint8_t* nd, int32_t id, bool checkRoute, int32_t cur, int32_t target,
                            uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack,
                            uint32_t owner, uint32_t& newCost, uint32_t& newForeign, int32_t& r);

int32_t __fastcall Route_FindPlain(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev,
                                   int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);

// RT3_REPLACE(0x48f7f0, Track_ReshapeCells); -- see above

int32_t __fastcall Node_HasFreeLink(void* n, int, int32_t any);

void __fastcall Node_ResetKind(void* n);
double __stdcall AngleOf(float x0, float y0, float x1, float y1);
float __fastcall Vehicle_Heading(uint8_t* o, int, int32_t forward);
double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
int32_t __fastcall Track_HasLink(uint8_t* t, int, uint8_t end);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

double __fastcall Terrain_HeightAt(uint8_t* t, int, float x, float z);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Tracks_UpdateColor(Collection* c);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
int32_t __fastcall Pair_Contains(void* p, int, int32_t v);
void __fastcall Routes_TryMergeAt(Collection* t, int, uint32_t id);
int32_t __fastcall Game_EffectiveVersion();
void __fastcall Routes_SplitBlock(Collection* c, int, uint32_t id);

int32_t __fastcall Route_NextStep(Collection* routes, int, int32_t from, int32_t to, uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign,
                                  int32_t anyTrack);

void __fastcall Group_ReplaceModel(uint8_t* g, int, const char* model);
void __fastcall Track_SetProps(uint8_t* t, int, int32_t cls, uint8_t a, uint8_t b, uint8_t flag);
float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
double __fastcall Effect_Blend(uint8_t* e, int, float t);
void __fastcall Company_Add7680(void* c, int, int32_t d);
uint8_t __fastcall Track_SetState(uint8_t* t, int, uint8_t state, int32_t on, int32_t notify, int32_t spread);
void __fastcall Track_Resolve(uint8_t* t, int, int32_t on);
void __fastcall Track_Propagate(Collection* nodes, int, uint8_t* t, uint8_t state, int32_t on, uint8_t old, int32_t notify);
float __fastcall Vehicle_Pitch(uint8_t* o, int, int32_t forward);
void __fastcall Track_SetFlatAndLift(uint8_t* t, int, uint32_t h, int32_t neighbours);
int32_t __fastcall Company_Get7680(void* c);
int32_t __fastcall Track_Cost(uint8_t* t, int, uint32_t company);
void __fastcall Tracks_ApplyPendingKinds(Collection* c);
int32_t __fastcall Node_HasLink(void* n, int, int32_t any);
int32_t __fastcall Tracks_SettleGroup(Collection* c, int, uint32_t id);
void __fastcall Track_DropAndSettle(uint8_t* t, int, int32_t, int32_t neighbours);
void __fastcall Route_UpdateFlag(uint8_t* r);
int32_t __fastcall Track_Cost2(uint8_t* t, int, uint32_t company);
void __fastcall Tracks_ApplyUpgrades(Collection* c, int, uint32_t company, int32_t amount);

int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused, int32_t byDegree,
                                          int32_t skipLinked);

float __stdcall Route_GradeFactorHere(float x, float z);
uint8_t* __fastcall Node_NextTowards(uint8_t* n, int, int32_t target, int32_t car, int32_t prev, uint8_t* train, int32_t anyTrack);
void __fastcall Obj_SetArgb(uint8_t* o, int, uint8_t a, uint8_t r, uint8_t g, uint8_t b);
void* __fastcall Node_Company(void* n);
void* __fastcall Track_Node20e(uint8_t* t);
void __fastcall Tracks_ReplayEdit(Collection* c);
void __fastcall Obj_SetText1c(uint8_t* o, int, const char* s, uint32_t v);
void __fastcall Track_LevelEnds(Collection* c, int, uint8_t* t);
int32_t __stdcall Track_OnPlainLine(uint8_t* t);
float __fastcall Track_RampGrade(uint8_t* self, int, float x0, float y0, float ang, float tx, float ty);
int32_t __fastcall TrackNet_LayOverpass(Collection* net, int, uint8_t* t, int32_t, int32_t);

int32_t __fastcall Route_ProbeLine(uint8_t* self, int, float x0, float y0, float angle, float x1, float y1, uint32_t company, int32_t minH, float* peak,
                                   int32_t* wet, float* dist);

int32_t __fastcall TrackBuilder_LayPiece(uint8_t* b, int, uint8_t* t, float x, float z, int32_t bridge, float pitch, int32_t, float minY, int32_t isLong,
                                         float span);

void __fastcall Midpoint(float* ox, float* oy, float x0, float y0, float x1, float y1);
float __fastcall AngleAdd(int32_t* wrapped, int, float a, float b);
void __fastcall Tracks_MoveEnds(Collection* c, int, uint32_t id, const uint32_t* a, const uint32_t* b, const uint32_t* d, const uint32_t* e);
double __fastcall Vehicle_Length(uint8_t* o);
int32_t __fastcall Track_Unlink(uint8_t* t, int, int32_t id);
int32_t __fastcall Route_ReplaceEnd(uint8_t* r, int, int32_t node, int32_t old);
int32_t __fastcall Routes_Merge(Collection* t, int, uint32_t a, uint32_t b);
int32_t __stdcall Track_FindClearance(uint8_t* p, int32_t from, float* height);
void __fastcall Track_ClearProps(uint8_t* t);
void __fastcall Track_SmoothHeights(Collection* tracks, int, uint32_t start, int32_t dir, int32_t loop, uint8_t force);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void __fastcall Train_ResetMotion(uint8_t* t);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
void __fastcall Track_MakeEnd(uint8_t* t, int, int32_t flip);
float __fastcall Obj_CenterCellY(void* o);
float __fastcall Obj_CenterCellX(void* o);
int32_t __fastcall Track_FindOverlap(uint8_t* t, int, int32_t* hit);
void __fastcall Track_PairWith(uint8_t* t, int, int32_t id, int32_t mutual);

void __fastcall Track_SetMode(uint8_t* t, int, int32_t kind, const char* model, uint32_t a, uint32_t b, float y, int32_t, int32_t v219, int32_t v251,
                              int32_t bit5, int32_t keep);

void __fastcall Track_MarkCells(uint8_t* t);
void __fastcall Track_RelightChain(Collection* c, int, int32_t start, int32_t dir, int32_t);
int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);
void* __fastcall Obj_Company21(void* o);
int32_t __fastcall Track_LinksWithin(uint8_t* n, int, int32_t target, int32_t depth, int32_t from);
uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
void RunDebugValidation();
int32_t __fastcall Map_RemoveTrackRef(uint8_t* o, int, uint8_t* t);
int32_t __fastcall Track_BuildCost(uint8_t* t, int, uint32_t company, int32_t inner);
struct PathNode;
int32_t __fastcall Track_PlanRoute(uint8_t* self, int, float x0, float y0, float x1, float y1, PathNode** out, PathNode** pending);
int32_t __fastcall Track_PlanCurve(uint8_t* t, int, uint32_t aId, uint32_t bId, uint32_t p4, uint32_t p5, uint32_t** out);
int32_t __fastcall Nodes_FindAlongLine(Collection* nodes, int, float x0, float y0, float x1, float y1, int32_t, int32_t exclude, int32_t* found);
float __stdcall Map_SlopeBetween(float ax0, float ay0, float ax1, float ay1, float bx0, float by0, float bx1, float by1);
float __fastcall Net_CommonVersion();
int32_t __fastcall Obj_PushIdNode(void* o, int, int32_t a, int32_t b);
float __fastcall Tracks_TotalCost(Collection* c, int, uint32_t company);

void __fastcall Track_Reshape_Impl(uint8_t* t, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d, int32_t neighbours,
                                   int32_t rebuild, int32_t newPiece);

void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side);
int32_t __fastcall TrackNet_AddPieceTurned(Collection* net, int, float cx, float cy, float angle, int32_t kind, int32_t owner, int32_t style);
void __fastcall TrackNet_ShowStatus(uint8_t* t, int, uint32_t piece);

int32_t __fastcall TrackNet_FindOrAddNode(Collection* net, int, float x, float y, float radius, int32_t kind, int32_t* missing, int32_t add, int32_t owner,
                                          int32_t byDegree, int32_t exclude, int32_t skipLinked);

RT3_NOINLINE int32_t __fastcall Game_EffectiveVersion();
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
RT3_NOINLINE float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void __fastcall TrackNet_RelayRect(Collection* net, int, uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1);
void __fastcall TrackNet_Undo(Collection* net, int, uint32_t stamp);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
RT3_NOINLINE void __fastcall TrackNet_EndBuild(Collection* net);
void __fastcall TrackNet_ElectrifyQuote(Collection* net, int, uint32_t company, float* cost, float* share);
RT3_NOINLINE void __fastcall TrackNet_RemovePiece(Collection* net, int, uint32_t id, int32_t force, int32_t from);
RT3_NOINLINE void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
RT3_NOINLINE void __fastcall Tracks_ApplyPendingKinds(Collection* c);
RT3_NOINLINE void __fastcall Obj_FreeList1c9(void* o);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
uint8_t __stdcall Obj_Mode1Or4(int32_t);
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund);
void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share);
void __fastcall Nop4950e0(void*);

// Functions of this file, in exe order.
RT3_NOINLINE int32_t __fastcall Track_BuildCost(uint8_t* t, int, uint32_t company, int32_t inner);
void __fastcall Track_MatchNeighbourHeights(uint8_t* t);
void __fastcall Track_DropAndSettle(uint8_t* t, int, int32_t, int32_t neighbours);
RT3_NOINLINE void __fastcall Track_Reshape_Impl(uint8_t* t, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d, int32_t neighbours, int32_t rebuild, int32_t newPiece);
RT3_NOINLINE void __fastcall Track_MakeEnd(uint8_t* t, int, int32_t flip);
RT3_NOINLINE int32_t __fastcall Track_DropModel(uint8_t* t);
RT3_NOINLINE uint8_t __fastcall Track_SetState(uint8_t* t, int, uint8_t state, int32_t on, int32_t notify, int32_t spread);
RT3_NOINLINE void __fastcall Track_Resolve(uint8_t* t, int, int32_t on);
RT3_NOINLINE int32_t __fastcall Track_Unlink(uint8_t* t, int, int32_t id);
RT3_NOINLINE void __fastcall Track_SetMode(uint8_t* t, int, int32_t kind, const char* model, uint32_t a, uint32_t b, float y, int32_t, int32_t v219, int32_t v251, int32_t bit5, int32_t keep);
RT3_NOINLINE void __fastcall Track_Remove(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Track_ConnectAndResolve(uint8_t* t, int, int32_t id);
RT3_NOINLINE void __fastcall Track_AddCatenary(uint8_t* t, int, int32_t side);
RT3_NOINLINE void __fastcall Track_PopUndo(uint8_t* t);
RT3_NOINLINE void __fastcall Track_Electrify(uint8_t* t, int, int32_t catenary, int32_t notify, int32_t side);
RT3_NOINLINE int32_t __fastcall TrackNet_LinkPair(Collection* net, int, uint32_t a, uint32_t b, int32_t na, int32_t nb);
int32_t __fastcall Grid_NearestNode(Collection* g, int, float px, float py, int32_t cx, int32_t cy, float* second, int32_t exclude, int32_t byDegree, float* best, int32_t skipLinked);
RT3_NOINLINE void __fastcall TrackNet_GradeRun(Collection* net, int, uint8_t* t, int32_t from, float rise, uint8_t force);
RT3_NOINLINE void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share);
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund);
void __fastcall Tracks_DeleteRefunded(Collection* c, int, uint32_t id);
int32_t __fastcall Track_RectIsFree(void* t, int, int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t allowSpecial);
RT3_NOINLINE void __fastcall TrackNet_RelayRect(Collection* net, int, uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1);
RT3_NOINLINE int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused, int32_t byDegree, int32_t skipLinked);
RT3_NOINLINE void __fastcall TrackNet_ElectrifyQuote(Collection* net, int, uint32_t company, float* cost, float* share);
uint8_t* __fastcall Route_Init(uint8_t* r);
void __fastcall Nop494e80(void*);
int32_t __fastcall Route_SetOwner(uint8_t* r, int, int32_t v);
int32_t __fastcall Route_Add10(uint8_t* r, int, int32_t v);
RT3_NOINLINE int32_t __fastcall Pair_Contains(void* p, int, int32_t v);
void __fastcall Nop494ef0(void*, int, int32_t);
RT3_NOINLINE int32_t __fastcall Route_AddNode(uint8_t* r, int, uint32_t id);
RT3_NOINLINE void __fastcall Route_UpdateFlag(uint8_t* r);
void __fastcall Route_Upgrade(uint8_t* r, int, int32_t);
RT3_NOINLINE void __fastcall Nop4950e0(void*);
RT3_NOINLINE int32_t __fastcall Route_ReplaceEnd(uint8_t* r, int, int32_t node, int32_t old);
void __fastcall Midpoint(float* ox, float* oy, float x0, float y0, float x1, float y1);
void __fastcall LerpPercent(int32_t pct, float* ox, float x0, float y0, float x1, float y1, float* oy);
RT3_NOINLINE float __stdcall Map_SlopeBetween(float ax0, float ay0, float ax1, float ay1, float bx0, float by0, float bx1, float by1);
RT3_NOINLINE double __stdcall AngleOf(float x0, float y0, float x1, float y1);
RT3_NOINLINE double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b);
float __fastcall AngleAdd(int32_t* wrapped, int, float a, float b);
RT3_NOINLINE void __fastcall Train_ResetMotion(uint8_t* t);
RT3_NOINLINE void __fastcall Obj_FreeList1c9(void* o);
RT3_NOINLINE void __fastcall Obj_CopyCorners(uint8_t* o, int, uint8_t* body);
uint8_t __stdcall Obj_Mode1Or4(int32_t);
RT3_NOINLINE void __fastcall Obj_DecayCounters(uint8_t* o);
void __fastcall Obj_CameraByOptions(uint8_t* o);
RT3_NOINLINE int32_t __fastcall TrackNet_AddPieceTurned(Collection* net, int, float cx, float cy, float angle, int32_t kind, int32_t owner, int32_t style);
RT3_NOINLINE void __fastcall TrackNet_MarkOverpasses(Collection* net, int, int32_t start, uint8_t mode, int32_t dir);
RT3_NOINLINE void __fastcall TrackNet_FinishStructures(Collection* net, int, int32_t start, int32_t force, int32_t dir);
void __fastcall Track_SmoothHeights(Collection* tracks, int, uint32_t start, int32_t dir, int32_t loop, uint8_t force);
void __fastcall Track_RelightChain(Collection* c, int, int32_t start, int32_t dir, int32_t);
RT3_NOINLINE void __fastcall TrackNet_FinishRun(Collection* net, int, int32_t start, int32_t mode);
int32_t __fastcall Tracks_SettleGroup(Collection* c, int, uint32_t id);
int32_t __stdcall Map_LineOnLand(float x0, float z0, float x1, float z1);
RT3_NOINLINE int32_t __fastcall Track_PlanCurve(uint8_t* t, int, uint32_t aId, uint32_t bId, uint32_t p4, uint32_t p5, uint32_t** out);
RT3_NOINLINE void __fastcall TrackNet_SmoothJoin(Collection* net, int, int32_t from, int32_t to, int32_t mode, int32_t a4, int32_t a5, float maxTurn);
RT3_NOINLINE int32_t __fastcall TrackNet_FindOrAddNode(Collection* net, int, float x, float y, float radius, int32_t kind, int32_t* missing, int32_t add, int32_t owner, int32_t byDegree, int32_t exclude, int32_t skipLinked);
RT3_NOINLINE void __fastcall Tracks_ApplyPendingKinds(Collection* c);
RT3_NOINLINE void __fastcall Tracks_ApplyUpgrades(Collection* c, int, uint32_t company, int32_t amount);
RT3_NOINLINE float __fastcall Tracks_TotalCost(Collection* c, int, uint32_t company);
void __fastcall Track_ReshapeCells(uint8_t* t, int, float x0, float z0, float x1, float z1, float x2, float z2, float x3, float z3, int32_t drop, int32_t neighbours);
void __fastcall Tracks_RestoreEdit(Collection* c, int, uint8_t* t);
void __fastcall Tracks_ReplayEdit(Collection* c);
RT3_NOINLINE void __fastcall TrackNet_ShowStatus(uint8_t* t, int, uint32_t piece);
RT3_NOINLINE void __fastcall TrackNet_RemovePiece(Collection* net, int, uint32_t id, int32_t force, int32_t from);
RT3_NOINLINE int32_t __fastcall TrackNet_WalkLine(Collection* net, int, int32_t start, float x0, float y0, float x1, float y1, int32_t mode, int32_t company, int32_t kind, int32_t* cost, int32_t* ok, int32_t* changed);
RT3_NOINLINE int32_t __fastcall Nodes_FindAlongLine(Collection* nodes, int, float x0, float y0, float x1, float y1, int32_t, int32_t exclude, int32_t* found);
RT3_NOINLINE int32_t __fastcall Track_PlanRoute(uint8_t* self, int, float x0, float y0, float x1, float y1, PathNode** out, PathNode** pending);
RT3_NOINLINE void __fastcall TrackNet_DropPreview(Collection* net);
RT3_NOINLINE int32_t __fastcall TrackNet_BuildLine(Collection* net, int, float x0, float y0, int32_t start, float x1, float y1, int32_t mode, int32_t company, int32_t kind, int32_t p9, float radius, int32_t end, int32_t* out);
RT3_NOINLINE void __fastcall TrackNet_EndBuild(Collection* net);
RT3_NOINLINE int32_t __fastcall TrackNet_LayLine(Collection* net, int, float x0, float y0, float x1, float y1, int32_t company, int32_t mode, int32_t doubleKind, int32_t* from, int32_t* to, float positive);
RT3_NOINLINE int32_t __fastcall TrackNet_ExtendTo(Collection* net, int, float x, float y, int32_t mode, int32_t company, int32_t kind, float radius, int32_t* out);
float __fastcall Track_RampGrade(uint8_t* self, int, float x0, float y0, float ang, float tx, float ty);
RT3_NOINLINE void __stdcall Track_ElectrifyRunPiece(uint8_t* t, int32_t n);
int32_t __stdcall Track_OnPlainLine(uint8_t* t);
RT3_NOINLINE int32_t __fastcall TrackNet_LayOverpass(Collection* net, int, uint8_t* t, int32_t, int32_t);
RT3_NOINLINE int32_t __fastcall TrackNet_LayOverpassRun(Collection* net, int, uint8_t* t, int32_t level);
RT3_NOINLINE int32_t __fastcall TrackBuilder_LayPiece(uint8_t* b, int, uint8_t* t, float x, float z, int32_t bridge, float pitch, int32_t, float minY, int32_t isLong, float span);
RT3_NOINLINE int32_t __fastcall TrackNet_LayBridgeRun(Collection* net, int, uint8_t* t);
int32_t __fastcall Route_ProbeLine(uint8_t* self, int, float x0, float y0, float angle, float x1, float y1, uint32_t company, int32_t minH, float* peak, int32_t* wet, float* dist);
void __fastcall Track_LevelEnds(Collection* c, int, uint8_t* t);
RT3_NOINLINE void __fastcall Track_Propagate(Collection* nodes, int, uint8_t* t, uint8_t state, int32_t on, uint8_t old, int32_t notify);
RT3_NOINLINE void __fastcall TrackNet_IdleModels(Collection* net, int, uint8_t* t);
RT3_NOINLINE int32_t __fastcall TrackNet_ShapePiece(Collection* net, int, uint8_t* t, float x, float y, int32_t n, uint32_t company, float peak, int32_t isLong);
void __fastcall Obj_Stash98(uint8_t* o);
int32_t __fastcall Obj_Unstash98(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_Has98(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_PushIdNode(void* o, int, int32_t a, int32_t b);
void __fastcall Obj_FreeList98(uint8_t* o);
RT3_NOINLINE void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
RT3_NOINLINE void __fastcall TrackNet_Undo(Collection* net, int, uint32_t stamp);
RT3_NOINLINE Collection* __fastcall Routes_Construct(Collection* c);
void __fastcall Routes_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall Routes_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Routes_Save(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE uint32_t __fastcall Routes_Add(Collection* t);
void __fastcall Collection5d0048_Remove(Collection* c, int, uint32_t id);
RT3_NOINLINE void __fastcall Routes_AddRef(Collection* t, int, uint32_t id, int32_t delta);
void __fastcall Routes_SplitBlock(Collection* c, int, uint32_t id);
RT3_NOINLINE void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side);
int32_t __fastcall Routes_CanMerge(Collection* routes, int, uint8_t* a, uint8_t* b);
RT3_NOINLINE int32_t __fastcall Routes_Merge(Collection* t, int, uint32_t a, uint32_t b);
RT3_NOINLINE int32_t __fastcall Routes_SplitAt(Collection* routes, int, uint32_t id);
void __fastcall Routes_Recount(Collection* routes, int, int32_t rid);
int32_t __fastcall Route_Find(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
int32_t __fastcall Route_FindPlain(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack);
void __fastcall Routes_NumberComponents(Collection* c);
RT3_NOINLINE void __fastcall Routes_TryMergeAt(Collection* t, int, uint32_t id);

static inline void Free53(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x4955b0. 1 if mode == 1, or +0x1e1 != -1 and kind +0x1fd == 3; else 4. (AL only.)
static uint8_t __fastcall Obj_Mode1Or4_Impl(void* o, int, int32_t mode) {
    if (mode == 1) return 1;
    if ((double)F<float>(o, 0x1e1) != *(const double*)0x5c9bf0 && F<uint8_t>(o, 0x1fd) == 3) return 1;
    return 4;
}

// 0x4a1440. Whether an unjunctioned piece sits on plain track: walking up to
// 4 pieces each way, no junction and no special piece within 3 steps.
static int WalkClear(uint8_t* start, uint8_t* prev, bool via206) {
    uint8_t* cur = start;
    for (int32_t i = 0; cur && i < 4; ++i) {
        if (F<int32_t>(cur, 0x20e) != -1) return 0;
        int32_t cls = F<int32_t>(cur, 0x226);
        bool special = cls == 1 || ((int32_t(__thiscall*)(void*))(*(void***)cur)[0x14 / 4])(cur) || cls == 3;
        if (special && i < 3) return 0;
        uint8_t* next;
        if (via206) next = (uint8_t*)Obj_Node206(cur) == prev ? (uint8_t*)Obj_Node20a(cur) : (uint8_t*)Obj_Node206(cur);
        else next = (uint8_t*)Obj_Node20a(cur) == prev ? (uint8_t*)Obj_Node206(cur) : (uint8_t*)Obj_Node20a(cur);
        prev = cur;
        cur = next;
    }
    return 1;
}

static inline uint8_t* Map() { return g_map; }

// Walks the track nodes from `from` (coming from `prev`) while they belong to
// route r, moving them to newId; counts them and returns the last node's id in *last.
static void Reassign(uint8_t* prev, uint8_t* cur, int32_t r, int32_t newId, int32_t* count, int32_t* count2,
                     uint32_t* last) {
    while (cur && F<int32_t>(cur, 0x212) == r) {
        Node_Set212(cur, 0, newId);
        ++*count;
        ++*count2;
        *last = F<uint32_t>(cur, 0x202);
        uint8_t* next = (uint8_t*)Obj_Node206(cur) == prev ? (uint8_t*)Obj_Node20a(cur) : (uint8_t*)Obj_Node206(cur);
        prev = cur;
        cur = next;
    }
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline double Bits(uint64_t u) { return *(double*)&u; }

static inline double Fabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }

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

static inline bool LandAt(float x, float z) {
    if (!Map_Contains(Map(), 0, x, z)) return false;
    float k = Fl(0x5c8568);
    int32_t iz = Ftol((double)z * k);
    return !Map_CellFlag(Map(), 0, Ftol((double)x * k), iz, 2);
}

static inline Collection* Nodes() { return g_tracks; }

static inline uint8_t* Node(uint32_t id) { return (uint8_t*)Collection_Get(Nodes(), 0, id); }

// Gives block g component `comp` and queues it, if it exists and is unnumbered.
static inline void Visit(Collection* c, uint32_t g, int32_t comp) {
    if (!Collection_IsValid(c, 0, g)) return;
    uint8_t* e = (uint8_t*)Collection_Get(c, 0, g);
    if (F<int32_t>(e, 0xc) != -1) return;
    F<int32_t>(e, 0xc) = comp;
    F<uint32_t*>(c, 0x94)[F<int32_t>(c, 0x98)] = g;
    ++F<int32_t>(c, 0x98);
}

namespace {
inline void* CrtAlloc(size_t n) { return ((void*(__cdecl*)(size_t))0x5a125d)(n); }
inline void CrtRelease(void* p) { ((void(__cdecl*)(void*))0x5a1145)(p); }
inline uint8_t* Piece(Collection* c, uint32_t id) { return (uint8_t*)Collection_Get(c, 0, id); }
inline uint8_t* Tracks() { return hook::As<uint8_t*>(g_tracks); }
// Track_Reshape through its register-keeping entry.
inline void Reshape(uint8_t* t, const float* a, const float* b, const float* c, const float* d) {
    ((void(__fastcall*)(uint8_t*, int, const float*, const float*, const float*, const float*, int32_t, int32_t, int32_t))(void*)&Track_Reshape)(
        t, 0, a, b, c, d, 1, 0, 0);
}
inline double Ground(float x, float z) { return Terrain_HeightAt(g_map, 0, x, z); }
// The higher of the ground at (x, z) plus 1.5 and `floor` (the ground is read again when it wins).
inline float GroundOrFloor(float x, float z, double floor) {
    if (Ground(x, z) + *(const double*)0x5c8578 < floor) return (float)floor;
    return (float)(Ground(x, z) + *(const double*)0x5c8578);
}
struct Corners {
    float a[3], b[3], c[3], d[3];
};
inline void GetCorners(uint8_t* t, Corners* k) {
    Effect_GetVectors(t, 0, (uint32_t*)k->a, (uint32_t*)k->b, (uint32_t*)k->c, (uint32_t*)k->d);
}
}

static inline double Fl_198(uint32_t a) { return *(const float*)a; }

static inline double ExeCos(double v) { return ((double(__cdecl*)(double))0x5a13e4)(v); }

static inline double ExeSin(double v) { return ((double(__cdecl*)(double))0x5a1494)(v); }

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static inline double Cos(double v) { return ((double(__cdecl*)(double))0x5a13e4)(v); }

static inline double Sin(double v) { return ((double(__cdecl*)(double))0x5a1494)(v); }

static inline double Atan(double v) { return ((double(__cdecl*)(double))0x5a1270)(v); }

// Probe steps along a leg: one per 0.362 of distance, plus one.
static inline float LegSteps(float x0, float y0, float x1, float y1) {
    float d = (float)Distance2D(x0, y0, x1, y1);
    return (float)((double)d / D(0x5c97e0) + D(0x5c8770));
}

static inline int32_t Blocked(float x, float y) {
    float k = Fl_205(0x5c8568);
    int32_t iy = Ftol((double)y * k);
    return Map_CellFlag(Map(), 0, Ftol((double)x * k), iy, 2);
}

// Terrain height at a position given in tiles (92 world units each).
static inline float HeightAtTile(float x, float y) {
    float k = Fl_205(0x5c8700);
    return (float)Terrain_HeightAt(Map(), 0, (float)((double)x * k), (float)((double)y * k));
}

static void RoutePlain_Link(Collection* routes, uint8_t* nd, int32_t id, bool checkRoute, int32_t cur, int32_t target,
                            uint32_t* cost, int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack,
                            uint32_t owner, uint32_t& newCost, uint32_t& newForeign, int32_t& r) {
    Collection* nodes = g_tracks;
    if (checkRoute) {
        if (F<int32_t>(Collection_Get(nodes, 0, id), 0x212) == F<int32_t>(nd, 0x212) ||
            (!anyTrack && F<uint8_t>(Collection_Get(nodes, 0, id), 0x216) != 2)) {
            newCost = 0xffffffff;
            return;
        }
        if (newCost == 0xffffffff) return;
    } else if (!anyTrack && F<uint8_t>(Collection_Get(nodes, 0, id), 0x216) != 2) {
        newCost = 0xffffffff;
        return;
    }
    if ((needFlag && !F<uint8_t>(Collection_Get(routes, 0, F<int32_t>(Collection_Get(nodes, 0, id), 0x212)), 0x1c)) ||
        (company && !Company_EntryFlag(company, 0, F<int32_t>(Collection_Get(routes, 0, F<int32_t>(Collection_Get(nodes, 0, id), 0x212)), 0x18)))) {
        newCost = 0xffffffff;
        return;
    }
    if (owner && F<uint16_t>(Collection_Get(nodes, 0, id), 0x22e) != owner) {
        newCost += 2;
        newForeign += 1;
    } else {
        newCost += 1;
    }
    r = Route_FindPlain(routes, 0, id, target, &newCost, cur, needFlag, company, &newForeign, anyTrack);
}

static inline double CrtFabs(double v) { return ((double(__cdecl*)(double))0x5a152e)(v); }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

static inline uint8_t* TrackNode(int32_t id) { return (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)id); }

// Moves `count` nodes' length from route `from` to route `to` (+0x14), dropping
// routes whose reference count (+0x10) reaches zero.
static void Routes_MoveLength(Collection* t, uint32_t from, uint32_t to, int32_t count) {
    auto route = [&](uint32_t id) { return (uint8_t*)Collection_Get(t, 0, id); };
    auto remove = [&](uint32_t id) { ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)t)[2])(t, 0, id); };
    F<int32_t>(route(from), 0x14) -= count;
    if (Collection_IsValid(t, 0, from) && !Route_Add10(route(from), 0, -count)) remove(from);
    F<int32_t>(route(to), 0x14) += count;
    if (Collection_IsValid(t, 0, to) && !Route_Add10(route(to), 0, count)) remove(to);
}

// The turned model (h, blend) for a crossing end (+0x20a side unless that
// neighbour is of the same piece kind, then turned around).
static void Track_EndModel(uint8_t* t, int32_t kind, const char* model) {
    uint8_t* n = (uint8_t*)Obj_Node20a(t);
    float h, arg;
    if (n && F<int32_t>((uint8_t*)Obj_Node20a(t), 0x226) != kind) {
        h = Vehicle_ScreenHeading(t, 0, 1);
        arg = 0.0f;
    } else {
        h = (float)((double)Vehicle_ScreenHeading(t, 0, 1) + Fl_207(0x5c8760));
        arg = 1.0f;
    }
    float b = (float)Effect_Blend(t, 0, arg);
    uint32_t hb;
    memcpy(&hb, &h, 4);
    Track_SetMode(t, 0, 0xa, model, hb, 0, b, 1, -1, -1, 0, 0);
}

// Bridge pieces by their span byte +0x252 (10..18, even only).
static const char* Track_BridgeModel(uint8_t span) {
    switch ((uint32_t)span - 10) {
    case 0: return (const char*)0x5cb048;
    case 1: return (const char*)0x5cff00;
    case 2: return (const char*)0x5d0000;
    case 4: return (const char*)0x5cffc0;
    case 6: return (const char*)0x5cff80;
    case 8: return (const char*)0x5cff40;
    }
    return nullptr;
}

// Neighbour piece n (id) of t: sets the corner heights of n that touch t
// (corners a / b on its +0x206 side, d / c on its +0x20a side) to t's end
// heights p / q and reshapes it.
static void TrackNet_FitNeighbour(Collection* net, uint8_t* t, uint32_t id, uint32_t p, uint32_t q) {
    uint8_t* n = (uint8_t*)Collection_Get(net, 0, id);
    uint32_t w[4][3];
    Effect_GetVectors(n, 0, w[0], w[1], w[2], w[3]);
    if (F<int32_t>(n, 0x206) == F<int32_t>(t, 0x202)) {
        w[0][1] = p;
        w[1][1] = q;
    } else if (F<int32_t>(n, 0x20a) == F<int32_t>(t, 0x202)) {
        w[3][1] = q;
        w[2][1] = p;
    }
    ((void(__fastcall*)(uint8_t*, int, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*, int32_t, int32_t, int32_t))(void*)&Track_Reshape)(
        n, 0, w[0], w[1], w[2], w[3], 1, 0, 0);
}

// Unpowered bridge pieces by their span byte +0x252 (10..18, even only).
static const char* Track_BridgeModelOff(uint8_t span) {
    switch ((uint32_t)span - 10) {
    case 0: return (const char*)0x5cb0e8;
    case 1: return (const char*)0x5cfee0;
    case 2: return (const char*)0x5cffe0;
    case 4: return (const char*)0x5cffa0;
    case 6: return (const char*)0x5cff60;
    case 8: return (const char*)0x5cff20;
    }
    return nullptr;
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

namespace {
// The original's frame: its slots are reused with other types (ints, floats,
// doubles, a text buffer), so the translation keeps them at their offsets.
struct BuildFrame {
    alignas(8) uint8_t b[0x198];
    int32_t& i(uint32_t o) { return *(int32_t*)(b + o); }
    float& f(uint32_t o) { return *(float*)(b + o); }
    double& d(uint32_t o) { return *(double*)(b + o); }
    uint8_t& u8(uint32_t o) { return b[o]; }
    uint32_t* p(uint32_t o) { return (uint32_t*)(b + o); }
};
inline double BlCos(double x) { return ((double(__cdecl*)(double))0x5a13e4)(x); }
inline double BlSin(double x) { return ((double(__cdecl*)(double))0x5a1494)(x); }
inline double BlAbs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
inline double BlAtan(double x) { return ((double(__cdecl*)(double))0x5a1270)(x); }
inline float BlCell(float v) { return (float)((double)v / Fl_207(0x5c8700)); }
inline uint32_t BlBits(float v) { return *(uint32_t*)&v; }
inline uint8_t* BlTrack(Collection* c, int32_t id) { return (uint8_t*)Collection_Get(c, 0, (uint32_t)id); }
inline int32_t BlVirt5(uint8_t* o) { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[0x14 / 4])(o); }
inline void BlVirt2(Collection* net, int32_t id) { ((void(__fastcall*)(Collection*, int, int32_t))(*(void***)net)[8 / 4])(net, 0, id); }
}

namespace {
// EAX as the exe's Obj_Mode1Or4 (0x4955b0) leaves it for a caller that
// pushed `kind` just before: AL the style (1 or 4), AH the FPU status of its
// compare of +0x1e1 with -1, the upper half `kind`'s.
int32_t ExtendStyle(uint8_t* n, int32_t kind) {
    if (kind == 1) return (int32_t)(((uint32_t)kind & 0xffffff00u) | 1);
    const float v = F<float>(n, 0x1e1);
    const uint32_t ah = v != v ? 0x45 : (double)v == D(0x5c9bf0) ? 0x40 : (double)v < D(0x5c9bf0) ? 0x01 : 0;
    const uint32_t al = (double)v != D(0x5c9bf0) && n[0x1fd] == 3 ? 1 : 4;
    return (int32_t)(((uint32_t)kind & 0xffff0000u) | ah << 8 | al);
}
}

static void Method(uint32_t address, void* self) {
    hook::Orig<void(__fastcall*)(void*)>(address)(self);
}

static void Notify(void* manager, uint32_t type) {
    hook::Orig<void(__fastcall*)(void*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)>(0x538e00)(
        manager, 0, type, 0x808, 0, 0, 0);
}

// 0x4941a0. Track piece table: deletes piece id. With `refund`, a company
// piece (+0x22e) in a running game counts for its company (Company_Add7680: 2
// for kind 4, +0x218, else 1). A built piece, or any with the table's +0xe9,
// first leaves the map; then it is removed and released.
RT3_NOINLINE void __fastcall Tracks_Delete(Collection* pieces, int, uint32_t id, int32_t refund) {
    if (!Collection_IsValid(pieces, 0, id)) return;
    uint8_t* t = (uint8_t*)Collection_Get(pieces, 0, id);
    if (F<uint16_t>(t, 0x22e) && hook::As<uint32_t>(g_companies) && refund) Company_Add7680(Node_Company(t), 0, t[0x218] == 4 ? 2 : 1);
    if (t[0x216] || F<int32_t>(pieces, 0xe9)) Map_RemoveTrackRef((uint8_t*)pieces, 0, t);
    Track_Remove(t);
    Collection_Release(pieces, 0, id);
}
RT3_REPLACE(0x4941a0, Tracks_Delete);

// 0x494230. Deletes track piece `id` with a refund.
void __fastcall Tracks_DeleteRefunded(Collection* c, int, uint32_t id) { Tracks_Delete(c, 0, id, 1); }
RT3_REPLACE(0x494230, Tracks_DeleteRefunded);

// 0x494240. Whether the cell rectangle holds only plain track (class 0), or
// also classes 12/13 when `allowSpecial`. QUIRK: the clamp is passed &x0 for
// both x0 and y0, so y0 is never clamped.
int32_t __fastcall Track_RectIsFree(void* t, int, int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t allowSpecial) {
    Map_ClampRect(hook::As<void*>(g_map), 0, &x0, &x0, &x1, &y1);
    for (int32_t x = x0; (int32_t)(uint16_t)x <= x1; ++x) {
        for (int32_t y = y0; (int32_t)(uint16_t)y <= y1; ++y) {
            uint8_t* e = (uint8_t*)Obj_CellEntry(t, 0, (uint16_t)x, (uint16_t)y);
            if (!e) continue;
            for (uint32_t* s = F<uint32_t*>(e, 2); s; s = (uint32_t*)s[1]) {
                int32_t cls = F<int32_t>(Collection_Get((Collection*)t, 0, s[0]), 0x226);
                if (cls == 0) continue;
                if ((cls == 0xc || cls == 0xd) && allowSpecial) continue;
                return 0;
            }
        }
    }
    return 1;
}
RT3_REPLACE(0x494240, Track_RectIsFree);

// 0x494310. Track network net: re-lays the track in cell rectangle (x0, y0) -
// (x1, y1) (x1 / y1 clamped to the map) after the ground changed. While the
// network defers it (+0xf5) the rectangle is only merged into the pending
// one (+0xf6..+0xfc). Otherwise, per cell (pieces marked with bit 3 of
// +0x24c as handled): plain pieces settle onto the ground; bridges (kind 3)
// re-grade their approaches (TrackNet_GradeRun) when both ends find
// clearance, else they are cleared and settled; on the border cells a
// plain or special (kind > 4) piece whose neighbour lies outside its cell
// has its shared ends rejoined to that neighbour. Finally the route of each
// piece nearest the centre (by distance, +10 per junction, +20 per open
// end, +15 / +5 when the next / second neighbour is on another route, +10 /
// +3 for their junctions) is height-smoothed once.
RT3_NOINLINE void __fastcall TrackNet_RelayRect(Collection* net, int, uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1) {
    uint8_t* m = g_map;
    int32_t cx1 = (uint16_t)x1, cy1 = (uint16_t)y1;
    if (F<int32_t>(m, 0x2155) - 1 < cx1) cx1 = F<int32_t>(m, 0x2155) - 1;
    if (F<int32_t>(m, 0x2159) - 1 < cy1) cy1 = F<int32_t>(m, 0x2159) - 1;
    uint8_t* b = (uint8_t*)net;
    uint16_t wx0 = (uint16_t)x0, wy0 = (uint16_t)y0, wx1 = (uint16_t)cx1, wy1 = (uint16_t)cy1;
    if (b[0xf5]) {
        if (wx0 < F<uint16_t>(b, 0xf6)) F<uint16_t>(b, 0xf6) = wx0;
        if (wy0 < F<uint16_t>(b, 0xf8)) F<uint16_t>(b, 0xf8) = wy0;
        if (wx1 > F<uint16_t>(b, 0xfa)) F<uint16_t>(b, 0xfa) = wx1;
        if (wy1 > F<uint16_t>(b, 0xfc)) F<uint16_t>(b, 0xfc) = wy1;
        return;
    }
    struct Node {
        int32_t id;
        Node* next;
    };
    auto cell = [&](uint32_t x, uint32_t y) { return (uint8_t*)Obj_CellEntry(net, 0, (uint16_t)x, (uint16_t)y); };
    auto clearMarks = [&](uint32_t x, uint32_t y) {
        if (uint8_t* c = cell(x, y))
            for (Node* n = F<Node*>(c, 2); n; n = n->next) ((uint8_t*)Collection_Get(net, 0, (uint32_t)n->id))[0x24c] &= 0xf7;
    };
    // Plain pieces settle onto the ground.
    for (uint32_t x = x0; (uint16_t)x <= wx1; ++x)
        for (uint32_t y = y0; (uint16_t)y <= wy1; ++y) {
            clearMarks(x, y);
            for (uint8_t* c = cell(x, y); c; c = cell(x, y)) {
                uint8_t* p = nullptr;
                for (Node* n = F<Node*>(c, 2); n; n = n->next) {
                    uint8_t* q = (uint8_t*)Collection_Get(net, 0, (uint32_t)n->id);
                    if (!(q[0x24c] & 8) && !F<int32_t>(q, 0x226)) {
                        p = q;
                        break;
                    }
                }
                if (!p) break;
                p[0x24c] |= 8;
                Track_DropAndSettle(p, 0, 0x40400000, 1);
            }
        }
    // Bridges re-grade their approaches.
    for (uint32_t x = x0; (uint16_t)x <= wx1; ++x)
        for (uint32_t y = y0; (uint16_t)y <= wy1; ++y) {
            clearMarks(x, y);
            for (uint8_t* c = cell(x, y); c; c = cell(x, y)) {
                uint8_t* p = nullptr;
                for (Node* n = F<Node*>(c, 2); n; n = n->next) {
                    uint8_t* q = (uint8_t*)Collection_Get(net, 0, (uint32_t)n->id);
                    if (q[0x24c] & 8) continue;
                    q[0x24c] |= 8;
                    if (F<int32_t>(q, 0x226) == 3) {
                        p = q;
                        break;
                    }
                }
                if (!p) break;
                float h1, h2;
                if (Track_FindClearance(p, F<int32_t>(p, 0x206), &h1) && Track_FindClearance(p, F<int32_t>(p, 0x20a), &h2)) {
                    TrackNet_GradeRun(net, 0, p, F<int32_t>(p, 0x206), h1, 2);
                    TrackNet_GradeRun(net, 0, p, F<int32_t>(p, 0x20a), h2, 2);
                } else {
                    Track_ClearProps(p);
                    Track_DropAndSettle(p, 0, 0x40400000, 1);
                }
            }
        }
    // Border pieces rejoin their neighbours outside the rectangle.
    for (uint32_t x = x0; (uint16_t)x <= wx1; ++x)
        for (uint32_t y = y0; (uint16_t)y <= wy1; ++y) {
            if ((uint16_t)x != wx0 && (uint16_t)x != wx1 && (uint16_t)y != wy0 && (uint16_t)y != wy1) continue;
            clearMarks(x, y);
            for (uint8_t* c = cell(x, y); c; c = cell(x, y)) {
                Node* n = F<Node*>(c, 2);
                for (; n; n = n->next) {
                    uint8_t* p = (uint8_t*)Collection_Get(net, 0, (uint32_t)n->id);
                    if (p[0x24c] & 8) continue;
                    p[0x24c] |= 8;
                    int32_t kind = F<int32_t>(p, 0x226);
                    if (kind != 0 && kind <= 4) continue;
                    float fx, fz;
                    uint8_t* q = nullptr;
                    bool side206 = false;
                    if (F<int32_t>(p, 0x206) != -1) {
                        q = (uint8_t*)Collection_Get(net, 0, F<uint32_t>(p, 0x206));
                        Obj_CenterCell(q, 0, &fx, &fz);
                        if ((uint16_t)Ftol((double)fx) != (uint16_t)x || (uint16_t)Ftol((double)fz) != (uint16_t)y) side206 = true;
                    }
                    if (!side206) {
                        if (F<int32_t>(p, 0x20a) == -1) continue;
                        q = (uint8_t*)Collection_Get(net, 0, F<uint32_t>(p, 0x20a));
                        Obj_CenterCell(q, 0, &fx, &fz);
                        if ((uint16_t)Ftol((double)fx) == (uint16_t)x && (uint16_t)Ftol((double)fz) == (uint16_t)y) continue;
                    }
                    TrackVec pa, pb, pc, pd, qa, qb, qc, qd;
                    Effect_GetVectors(p, 0, pa.u, pb.u, pc.u, pd.u);
                    Effect_GetVectors(q, 0, qa.u, qb.u, qc.u, qd.u);
                    if (side206) {
                        if (F<int32_t>(q, 0x206) == n->id) ReshapeVecs(p, qb, qa, pc, pd);
                        else if (F<int32_t>(q, 0x20a) == n->id) ReshapeVecs(p, qd, qc, pc, pd);
                    } else {
                        if (F<int32_t>(q, 0x206) == n->id) ReshapeVecs(p, pa, pb, qb, qa);
                        else if (F<int32_t>(q, 0x20a) == n->id) ReshapeVecs(p, pa, pb, qd, qc);
                    }
                    break;
                }
                if (!n) break;
            }
        }
    // Smooth the route of each piece nearest the centre.
    int32_t cX = Ftol((double)((int32_t)wx1 + (int32_t)wx0) * D(0x5c8628));
    int32_t cY = Ftol((double)((int32_t)wy1 + (int32_t)wy0) * D(0x5c8628));
    int32_t found = 0;
    uint8_t seen[2000];
    int32_t best[2000], who[2000];
    memset(seen, 0, sizeof seen);
    for (uint32_t x = x0, xs = wx0; (uint16_t)x <= wx1; ++x, ++xs)
        for (uint32_t y = y0, ys = (uint16_t)y0; (uint16_t)y <= wy1; ++y, ++ys) {
            uint8_t* c = cell(x, y);
            if (!c) continue;
            for (Node* n = F<Node*>(c, 2); n; n = n->next) {
                uint8_t* p = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)n->id);
                if (!p) continue;
                int32_t r = F<int32_t>(p, 0x212);
                if (r >= 2000 || r < 0) continue;
                float score = (float)(int32_t)(uint16_t)DistanceInt((int32_t)xs, (int32_t)ys, cX, cY);
                if (F<int32_t>(p, 0x20e) != -1) score = (float)((double)score + Fl_207(0x5c85f8));
                for (int32_t s = 0; s < 2; ++s) {
                    uint32_t link = s ? 0x20a : 0x206;
                    if (F<int32_t>(p, link) == -1) {
                        score = (float)((double)score + Fl_207(0x5c8784));
                        continue;
                    }
                    uint8_t* q = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(p, link));
                    if (F<int32_t>(q, 0x20e) != -1) score = (float)((double)score + Fl_207(0x5c85f8));
                    if (F<int32_t>(q, 0x212) != F<int32_t>(p, 0x212)) score = (float)((double)score + Fl_207(0x5c873c));
                    if (F<int32_t>(q, link) == -1) continue;
                    uint8_t* q2 = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(q, link));
                    if (F<int32_t>(q2, 0x20e) != -1) score = (float)((double)score + Fl_207(0x5c8900));
                    if (F<int32_t>(q2, 0x212) != F<int32_t>(p, 0x212)) score = (float)((double)score + Fl_207(0x5c8554));
                }
                if (seen[r]) {
                    float was = (float)best[r];
                    if (!(score < was)) continue;
                } else {
                    ++found;
                    seen[r] = 1;
                }
                best[r] = Ftol((double)score);
                who[r] = n->id;
            }
        }
    if (!found) return;
    for (int32_t i = 0; i < 2000; ++i) {
        if (!seen[i]) continue;
        Track_SmoothHeights(net, 0, (uint32_t)who[i], -1, 1, 2);
        if (--found <= 0) return;
    }
}
RT3_REPLACE(0x494310, TrackNet_RelayRect);

// 0x494cb0. Nearest track node to (px, py) within `radius` cells (none for a
// negative radius: -1): searches the point's own grid cell, then every cell of
// the square around it; the radius shrinks to the second-best distance found as
// the search goes (Grid_NearestNode updates it). `unused` is never read.
RT3_NOINLINE int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused,
                                          int32_t byDegree, int32_t skipLinked) {
    if (radius < 0.0f) return -1;
    float best = 999999.0f;
    int32_t ix = Ftol(px), iy = Ftol(py);
    int32_t found = Grid_NearestNode(g, 0, px, py, ix, iy, &radius, exclude, byDegree, &best, skipLinked);
    for (int32_t x = Ftol((double)px - radius); x <= Ftol((double)radius + px); ++x) {
        if (x < 0 || x >= F<int32_t>(g_map, 0x2155)) continue;
        for (int32_t y = Ftol((double)py - radius); y <= Ftol((double)radius + py); ++y) {
            if (y < 0 || y >= F<int32_t>(g_map, 0x2159)) continue;
            if (x == ix && y == iy) continue;
            int32_t r = Grid_NearestNode(g, 0, px, py, x, y, &radius, exclude, byDegree, &best, skipLinked);
            if (r != -1) found = r;
        }
    }
    return found;
}
RT3_REPLACE(0x494cb0, Grid_NearestNodeWithin);

// 0x494e20. Track network net: prices electrifying company's track (dry run of
// TrackNet_ElectrifyCompany): the cost and the share still unpowered.
RT3_NOINLINE void __fastcall TrackNet_ElectrifyQuote(Collection* net, int, uint32_t company, float* cost, float* share) {
    TrackNet_ElectrifyCompany(net, 0, (uint16_t)company, 1, cost, share);
}
RT3_REPLACE(0x494e20, TrackNet_ElectrifyQuote);

// 0x494e40. Initialises a 0x42-byte route record; returns it.
RT3_NOINLINE uint8_t* __fastcall Route_Init(uint8_t* r) {
    F<int32_t>(r, 4) = -1;
    F<int32_t>(r, 8) = -1;
    F<int32_t>(r, 0x18) = -1;
    F<int32_t>(r, 0x1e) = -1;
    F<int32_t>(r, 0xc) = -1;
    r[0x1c] = 0;
    F<int32_t>(r, 0x10) = 0;
    F<int32_t>(r, 0x14) = 0;
    r[0x1d] = 1;
    F<uint16_t>(r, 0x22) = 0;
    for (int k = 0; k < 0x1e; ++k) r[0x24 + k] = 0;
    return r;
}
RT3_REPLACE(0x494e40, Route_Init);

RT3_NOINLINE void __fastcall Nop494e80(void*) { __asm nop }
RT3_REPLACE(0x494e80, Nop494e80);

RT3_NOINLINE int32_t __fastcall Route_SetOwner(uint8_t* r, int, int32_t v) {
    F<int32_t>(r, 0) = v;
    F<int32_t>(r, 0x42) = 0;
    F<int32_t>(r, 0x46) = 0;
    return 0;
}
RT3_REPLACE(0x494e90, Route_SetOwner);

RT3_NOINLINE int32_t __fastcall Route_Add10(uint8_t* r, int, int32_t v) { return F<int32_t>(r, 0x10) += v; }
RT3_REPLACE(0x494eb0, Route_Add10);

RT3_NOINLINE int32_t __fastcall Pair_Contains(void* p, int, int32_t v) { return (v == F<int32_t>(p, 4) || v == F<int32_t>(p, 8)) ? 1 : 0; }
RT3_REPLACE(0x494ed0, Pair_Contains);

void __fastcall Nop494ef0(void*, int, int32_t) { __asm nop }
RT3_REPLACE(0x494ef0, Nop494ef0);

// 0x494f00. Extends a route with track node `id` if it matches the route's
// region type, owner and flag (the first node defines them). 1 if accepted.
RT3_NOINLINE int32_t __fastcall Route_AddNode(uint8_t* r, int, uint32_t id) {
    uint32_t type = Obj_CenterRegionType(Collection_Get(g_tracks, 0, id));
    int32_t owner = F<uint16_t>(Collection_Get(g_tracks, 0, id), 0x22e);
    uint8_t flag = F<uint8_t>(Collection_Get(g_tracks, 0, id), 0x44) ? 1 : 0;
    if (F<int32_t>(r, 4) == -1 && F<int32_t>(r, 8) == -1) {
        F<uint32_t>(r, 4) = id;
        F<uint32_t>(r, 8) = id;
        F<uint32_t>(r, 0x18) = type;
        F<int32_t>(r, 0x1e) = owner;
        r[0x1c] = flag;
        return 1;
    }
    if ((flag != 0) != (r[0x1c] != 0)) return 0;
    if (type != F<uint32_t>(r, 0x18) || owner != F<int32_t>(r, 0x1e)) return 0;
    if (F<int32_t>(r, 4) == -1) F<uint32_t>(r, 4) = id;
    else F<uint32_t>(r, 8) = id;
    return 1;
}
RT3_REPLACE(0x494f00, Route_AddNode);

// 0x494fb0. Route r: its flag +0x1c is set when both end nodes (+4, +8) have
// +0x44 (left alone while the node table is frozen, +0xf1); a change tries
// to merge the route with its neighbours.
RT3_NOINLINE void __fastcall Route_UpdateFlag(uint8_t* r) {
    uint8_t old = r[0x1c];
    if (!F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xf1)) r[0x1c] = TrackNode(F<int32_t>(r, 4))[0x44] && TrackNode(F<int32_t>(r, 8))[0x44] ? 1 : 0;
    if (old != r[0x1c]) Routes_TryMergeAt(g_routes, 0, F<uint32_t>(r, 0));
}
RT3_REPLACE(0x494fb0, Route_UpdateFlag);

// 0x495020. Upgrades a route record from older save versions (world
// +0x46c34) and resets its scratch fields; single-piece routes are recounted.
// QUIRK: the owner fix-up writes +0x18, not +0x1e.
RT3_NOINLINE void __fastcall Route_Upgrade(uint8_t* r, int, int32_t) {
    auto firstNode = [&]() -> int32_t {
        if (F<int32_t>(r, 4) != -1) return F<int32_t>(r, 4);
        return F<int32_t>(r, 8);
    };
    if (F<int32_t>(g_world, 0x46c34) <= 0x2533) {
        F<int32_t>(r, 0x18) = 1;
        int32_t id = firstNode();
        if (id != -1) F<uint32_t>(r, 0x18) = Obj_CenterGridByte(Collection_Get(g_tracks, 0, id));
    }
    if (F<int32_t>(g_world, 0x46c34) <= 0x2535) {
        F<int32_t>(r, 0x1e) = 1;
        int32_t id = firstNode();
        if (id != -1) F<int32_t>(r, 0x18) = F<uint16_t>(Collection_Get(g_tracks, 0, id), 0x22e);
    }
    for (int k = 0; k < 0x1e; ++k) r[0x24 + k] = 0;
    F<int32_t>(r, 0x42) = 0;
    F<int32_t>(r, 0x46) = 0;
    if (F<int32_t>(r, 4) == F<int32_t>(r, 8) && F<int32_t>(r, 0x10) > 1)
        Routes_Recount(g_routes, 0, F<int32_t>(r, 0));
}
RT3_REPLACE(0x495020, Route_Upgrade);

// 0x4950e0 (vtable stub).
RT3_NOINLINE void __fastcall Nop4950e0(void*) { __asm nop }
RT3_REPLACE(0x4950e0, Nop4950e0);

// 0x4950f0. Route r: replaces end node `old` (+4, else +8) by `node` when
// that node lies in the route's region type (+0x18) and group (+0x1e);
// then refreshes the flag (also when nothing matched). 1 when replaced.
RT3_NOINLINE int32_t __fastcall Route_ReplaceEnd(uint8_t* r, int, int32_t node, int32_t old) {
    if (F<uint32_t>(r, 0x18) != Obj_CenterRegionType(TrackNode(node)) || F<uint32_t>(r, 0x1e) != F<uint16_t>(TrackNode(node), 0x22e)) return 0;
    int32_t done = 0;
    if (F<int32_t>(r, 4) == old) {
        F<int32_t>(r, 4) = node;
        Route_UpdateFlag(r);
        return 1;
    }
    if (F<int32_t>(r, 8) == old) {
        F<int32_t>(r, 8) = node;
        done = 1;
    }
    Route_UpdateFlag(r);
    return done;
}
RT3_REPLACE(0x4950f0, Route_ReplaceEnd);

// 0x495170. Midpoint of (x0, y0) and (x1, y1).
RT3_NOINLINE void __fastcall Midpoint(float* ox, float* oy, float x0, float y0, float x1, float y1) {
    const double two = *(const double*)0x5c85d0;
    *ox = (float)(((double)x0 + x1) / two);
    *oy = (float)(((double)y0 + y1) / two);
}
RT3_REPLACE(0x495170, Midpoint);

// 0x4951a0. Point `pct` percent of the way from (x0, y0) to (x1, y1).
void __fastcall LerpPercent(int32_t pct, float* ox, float x0, float y0, float x1, float y1, float* oy) {
    float t = (float)pct;
    const float hundred = *(const float*)0x5c8c28;
    *ox = (float)(((double)x1 - x0) * t / hundred + x0);
    *oy = (float)(((double)y1 - y0) * t / hundred + y0);
}
RT3_REPLACE(0x4951a0, LerpPercent);

// 0x4951f0. Slope angle |atan(dh / dist)| between the midpoints of two segments
// (tile units scaled by 0x5c8c48); 0 when they coincide.
RT3_NOINLINE float __stdcall Map_SlopeBetween(float ax0, float ay0, float ax1, float ay1, float bx0, float by0, float bx1,
                                 float by1) {
    const double half = *(const double*)0x5c85d0, k = *(const double*)0x5c8c48;
    float p0 = (float)(((double)ax0 + ax1) / half * k);
    float p1 = (float)(((double)ay0 + ay1) / half * k);
    float p2 = (float)(((double)bx0 + bx1) / half * k);
    float p3 = (float)(((double)by0 + by1) / half * k);
    float dist = (float)Distance2D(p0, p1, p2, p3);
    float h1 = (float)Terrain_HeightAt(Map(), 0, p0, p1);
    float h2 = (float)Terrain_HeightAt(Map(), 0, p2, p3);
    if ((double)dist == *(const double*)0x5c8608) return *(const float*)0x5c8598;
    double a = ((double(__cdecl*)(double))0x5a1270)(((double)h2 - h1) / dist);
    return (float)((double(__cdecl*)(double))0x5a152e)(a);
}
RT3_REPLACE(0x4951f0, Map_SlopeBetween);

// 0x4952f0. Angle (radians, [0, 2 pi)) of the direction from (x1, y1) to
// (x0, y0) via atan; returned unrounded.
RT3_NOINLINE double __stdcall AngleOf(float x0, float y0, float x1, float y1) {
    if (x0 == x1) return y0 > y1 ? (float)4.71238898038 : (float)1.5707963267948966;
    float a = (float)((double(__cdecl*)(double))0x5a1270)(((double)y0 - y1) / ((double)x0 - x1));
    if (x0 > x1) return (double)a + *(const float*)0x5c8760;
    if (y0 > y1) return (double)a + *(const float*)0x5c8768;
    return a;
}
RT3_REPLACE(0x4952f0, AngleOf);

// 0x4953c0. a - b (as float) wrapped into [-pi, pi]; *wrapped (optional) = 1 if it was.
RT3_NOINLINE double __fastcall AngleDelta(int32_t* wrapped, int, float a, float b) {
    const double pi = *(const double*)0x5c96a8, twoPi = *(const double*)0x5c96b8, negPi = *(const double*)0x5cfe30;
    float d = (float)((double)a - b);
    if (wrapped) *wrapped = 0;
    if (d > pi) {
        d = (float)(d - twoPi);
    } else if (d < negPi) {
        d = (float)(d + twoPi);
    } else {
        return d;
    }
    if (wrapped) *wrapped = 1;
    return d;
}
RT3_REPLACE(0x4953c0, AngleDelta);

// 0x495420. a + b wrapped once into [0, 2 pi]; *wrapped tells whether it was.
RT3_NOINLINE float __fastcall AngleAdd(int32_t* wrapped, int, float a, float b) {
    float s = (float)((double)a + b);
    if (wrapped) *wrapped = 0;
    const double twoPi = *(const double*)0x5c96b8;
    if (s < *(const float*)0x5c8598) {
        s = (float)((double)s + twoPi);
    } else if ((double)s > twoPi) {
        s = (float)((double)s - twoPi);
    } else {
        return s;
    }
    if (wrapped) *wrapped = 1;
    return s;
}
RT3_REPLACE(0x495420, AngleAdd);

// 0x495480. Resets a train's movement state.
RT3_NOINLINE void __fastcall Train_ResetMotion(uint8_t* t) {
    F<int32_t>(t, 0x118) = 0;
    t[0x11c] = 0;
    F<int32_t>(t, 0x125) = -1;
    F<int32_t>(t, 0x129) = -1;
    F<int32_t>(t, 0x12d) = -1;
    F<int32_t>(t, 0x131) = -1;
    F<int32_t>(t, 0x135) = 0;
    F<uint32_t>(t, 0x169) = 0xbf800000;  // -1.0f
    F<int32_t>(t, 0x1c9) = 0;
    F<int32_t>(t, 0x1cd) = 0;
    F<uint32_t>(t, 0x1d9) = 0xbf800000;
    F<int32_t>(t, 0x1dd) = 0;
    F<uint32_t>(t, 0x1e1) = 0xbf800000;
    F<int32_t>(t, 0x1e5) = 0;
    F<int32_t>(t, 0x1ed) = 0;
    F<int32_t>(t, 0x1f5) = -1;
    F<int32_t>(t, 0x1f9) = -1;
    F<int32_t>(t, 0x1fe) = 0;
    F<int32_t>(t, 0x202) = 0;
    F<int32_t>(t, 0x1d1) = 0;
    F<int32_t>(t, 0xdd) = 0;
    F<int32_t>(t, 0xe1) = 0;
    for (int k = 0; k < 5; ++k) t[0xd8 + k] = 0;
}
RT3_REPLACE(0x495480, Train_ResetMotion);

// 0x495540. Frees the singly linked list at +0x1c9 (next at +8).
RT3_NOINLINE void __fastcall Obj_FreeList1c9(void* o) {
    uint8_t*& head = F<uint8_t*>(o, 0x1c9);
    for (uint8_t* n = head; n;) {
        uint8_t* next = *(uint8_t**)(n + 8);
        Free53(head);
        head = next;
        n = next;
    }
    head = nullptr;
}
RT3_REPLACE(0x495540, Obj_FreeList1c9);

// 0x495580. Copies a body's corners into this object (+0x139..+0x15d).
RT3_NOINLINE void __fastcall Obj_CopyCorners(uint8_t* o, int, uint8_t* body) {
    Effect_GetVectors(body, 0, (uint32_t*)(o + 0x139), (uint32_t*)(o + 0x145), (uint32_t*)(o + 0x151),
                      (uint32_t*)(o + 0x15d));
}
RT3_REPLACE(0x495580, Obj_CopyCorners);

// 0x4a06c7 keeps using ECX after the call.
__declspec(naked) uint8_t __stdcall Obj_Mode1Or4(int32_t) { RT3_KEEP_ECX_EDX(Obj_Mode1Or4_Impl, 1) }
RT3_REPLACE(0x4955b0, Obj_Mode1Or4);

// 0x4955e0. Counter decay per the options +0x25b / +0x257.
RT3_NOINLINE void __fastcall Obj_DecayCounters(uint8_t* o) {
    if (F<int32_t>(g_app, 0x25b)) Obj_DecA4(o);
    if (F<int32_t>(g_app, 0x257)) Obj_DecAc(o);
}
RT3_REPLACE(0x4955e0, Obj_DecayCounters);

// 0x495620. Camera actions per the options +0x25b / +0x257.
void __fastcall Obj_CameraByOptions(uint8_t* o) {
    if (F<int32_t>(g_app, 0x25b)) Camera_BeginA4(o);
    if (F<int32_t>(g_app, 0x257)) Camera_BeginAc(o);
}
RT3_REPLACE(0x495620, Obj_CameraByOptions);

// 0x495660. Track network net: adds a turned piece (0x493cf0, not forced).
// With the world's track limit (+0x4aab) a company with pieces pending
// (+0x7664 / +0x7668 / +0x766c) or a busy network (+0x98) gets -1. In an
// electrified world (+0x4c74) the new piece is electrified. Its id.
RT3_NOINLINE int32_t __fastcall TrackNet_AddPieceTurned(Collection* net, int, float cx, float cy, float angle, int32_t kind, int32_t owner, int32_t style) {
    if (F<int32_t>(g_world, 0x4aab)) {
        uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, (uint32_t)owner);
        if (F<int32_t>(co, 0x766c) + F<int32_t>(co, 0x7668) + F<int32_t>(co, 0x7664) > 0 || F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x98)) return -1;
    }
    int32_t id = ((int32_t(__fastcall*)(Collection*, int, float, float, float, int32_t, int32_t, int32_t, int32_t))0x493cf0)(net, 0, cx, cy, angle, kind, owner,
                                                                                                                       style, 0);
    if (F<int32_t>(g_world, 0x4c74) && Collection_IsValid(g_tracks, 0, (uint32_t)id))
        Track_Electrify((uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)id), 0, 0, 0, 1);
    return id;
}
RT3_REPLACE(0x495660, TrackNet_AddPieceTurned);

// 0x495720. Track network net: walks the new run from piece `start` (side
// +0x206, or +0x20a with `dir`) over plain pieces built in this pass
// (+0x240 >= +0x1d5) and handles the track each one crosses. Without
// overpasses (options +0x144) the crossing pieces are paired. Otherwise:
// a crossing of a different route (or an unknown one) is flagged (+0xd8);
// one sharing a neighbour or a junction flags +0xdc, pairs and stops; a
// nearly parallel one (within 0.6 rad either way) pairs and stops, flagging
// +0xda when far (3.5) from both build ends (+0x11d / +0xdd); else the piece
// becomes an overpass section at a common height (the crossed piece's
// average + 31) - flag +0xd9 - and, when building (mode 1 / 2), the crossed
// piece's pole model goes.
RT3_NOINLINE void __fastcall TrackNet_MarkOverpasses(Collection* net, int, int32_t start, uint8_t mode, int32_t dir) {
    uint8_t* b = (uint8_t*)net;
    uint8_t* s = (uint8_t*)Collection_Get(net, 0, (uint32_t)start);
    int32_t prev = F<int32_t>(s, 0x202);
    float lift = -1.0f;
    int32_t cur = dir ? F<int32_t>(s, 0x20a) : F<int32_t>(s, 0x206);
    b[0xd8] = b[0xd9] = b[0xda] = b[0xdb] = b[0xdc] = 0;
    while (cur != -1) {
        uint8_t* p = (uint8_t*)Collection_Get(net, 0, (uint32_t)cur);
        if (F<int32_t>(p, 0x226)) return;
        if (F<int32_t>(p, 0x20e) != prev && F<uint32_t>(p, 0x240) >= F<uint32_t>(b, 0x1d5)) {
            float cx, cz;
            Obj_CenterCell(p, 0, &cx, &cz);
            int32_t other;
            int32_t found = Track_FindOverlap(p, 0, &other);
            if (!F<int32_t>(g_app, 0x144)) {
                if (found) Track_PairWith(p, 0, other, 1);
                lift = -1.0f;
            } else if (!found)
                lift = -1.0f;
            else {
                if (!Collection_IsValid(g_tracks, 0, F<uint32_t>(b, 0x125)) || !Collection_IsValid(g_tracks, 0, (uint32_t)other))
                    b[0xd8] = 1;
                else {
                    int32_t r = F<int32_t>(Collection_Get(g_tracks, 0, (uint32_t)other), 0x212);
                    if (r != F<int32_t>(Collection_Get(g_tracks, 0, F<uint32_t>(b, 0x125)), 0x212)) b[0xd8] = 1;
                }
                uint8_t* q = (uint8_t*)Collection_Get(net, 0, (uint32_t)other);
                if (F<int32_t>(q, 0x20e) == F<int32_t>(p, 0x202) || F<int32_t>(q, 0x206) == F<int32_t>(p, 0x206) ||
                    F<int32_t>(q, 0x206) == F<int32_t>(p, 0x20a) || F<int32_t>(q, 0x20a) == F<int32_t>(p, 0x206) ||
                    F<int32_t>(q, 0x20a) == F<int32_t>(p, 0x20a)) {
                    b[0xdc] = 1;
                    Track_PairWith(p, 0, other, 1);
                    return;
                }
                auto fabs_ = (double(__cdecl*)(double))0x5a152e;
                float hp = Vehicle_ScreenHeading(p, 0, 1);
                float hq = Vehicle_ScreenHeading(q, 0, 1);
                bool parallel = fabs_(AngleDelta(nullptr, 0, hq, hp)) < D(0x5c8880);
                if (!parallel) {
                    hp = Vehicle_ScreenHeading(p, 0, 0);
                    hq = Vehicle_ScreenHeading(q, 0, 1);
                    parallel = fabs_(AngleDelta(nullptr, 0, hq, hp)) < D(0x5c8880);
                }
                if (parallel) {
                    float y = Obj_CenterCellY(p);
                    float x = Obj_CenterCellX(p);
                    if (Distance2D(x, y, F<float>(b, 0x11d), F<float>(b, 0x121)) > D(0x5c8dc0)) {
                        y = Obj_CenterCellY(p);
                        x = Obj_CenterCellX(p);
                        if (Distance2D(x, y, F<float>(b, 0xdd), F<float>(b, 0xe1)) > D(0x5c8dc0)) b[0xda] = 1;
                    }
                    Track_PairWith(p, 0, other, 1);
                    return;
                }
                TrackVec va, vb, vc, vd;
                Effect_GetVectors(p, 0, va.u, vb.u, vc.u, vd.u);
                if (lift == -1.0f) {
                    TrackVec qa, qb, qc, qd;
                    Effect_GetVectors(q, 0, qa.u, qb.u, qc.u, qd.u);
                    lift = (float)(((((double)qd.f[1] + qc.f[1]) + qb.f[1]) + qa.f[1]) / D(0x5c87b8) + D(0x5cfe38));
                }
                va.f[1] = vb.f[1] = vc.f[1] = vd.f[1] = lift;
                ReshapeVecs(p, va, vb, vc, vd);
                p[0x59] &= 0xef;
                b[0xd9] = 1;
                float h = (float)((double)Vehicle_ScreenHeading(p, 0, 1) + Fl_207(0x5c8760));
                uint32_t hb;
                memcpy(&hb, &h, 4);
                Track_SetMode(p, 0, 3, (const char*)(p[0x218] == 4 ? 0x5cb168 : 0x5cb180), hb, 0, lift, 1, -1, -1, 0, 0);
                if (mode == 1 || mode == 2) {
                    Track_MarkCells(p);
                    if (q[0x44] && F<void*>(q, 0x248)) Track_DropModel(q);
                }
            }
        }
        int32_t next = -1;
        if (F<int32_t>(p, 0x20e) == -1) next = F<int32_t>(p, 0x206) == prev ? F<int32_t>(p, 0x20a) : F<int32_t>(p, 0x206);
        prev = F<int32_t>(p, 0x202);
        if (prev == start) return;
        cur = next;
    }
}
RT3_REPLACE(0x495720, TrackNet_MarkOverpasses);

// 0x495b80. Track network net: walks the new run from piece `start` (side
// +0x206, or +0x20a with `dir`) over pieces built in this pass and finishes
// its bridges and tunnels. A bridge (kind 3) with clearance at both ends
// grades both approaches and gives each non-bridge neighbour a bridge-end
// model (single / double by the bridge's state); without clearance it is
// flagged (+0xdb), cleared and settled, together with the bridge / pier
// pieces (kinds 3, 12, 13) chained to it. A tunnel piece (kind 1) grades
// and gets a portal model towards each non-tunnel neighbour.
RT3_NOINLINE void __fastcall TrackNet_FinishStructures(Collection* net, int, int32_t start, int32_t force, int32_t dir) {
    uint8_t* b = (uint8_t*)net;
    uint8_t* s = (uint8_t*)Collection_Get(net, 0, (uint32_t)start);
    int32_t prev = F<int32_t>(s, 0x202);
    int32_t cur = dir ? F<int32_t>(s, 0x20a) : F<int32_t>(s, 0x206);
    if (cur == -1) return;
    for (;;) {
        uint8_t* p = (uint8_t*)Collection_Get(net, 0, (uint32_t)cur);
        if (F<int32_t>(p, 0x20e) != prev && F<uint32_t>(p, 0x240) >= F<uint32_t>(b, 0x1d5)) {
            int32_t kind = F<int32_t>(p, 0x226);
            if (kind == 3) {
                float h1, h2;
                if (Track_FindClearance(p, F<int32_t>(p, 0x206), &h1) && Track_FindClearance(p, F<int32_t>(p, 0x20a), &h2)) {
                    TrackNet_GradeRun(net, 0, p, F<int32_t>(p, 0x206), h1, (uint8_t)force);
                    TrackNet_GradeRun(net, 0, p, F<int32_t>(p, 0x20a), h2, (uint8_t)force);
                    for (int32_t side = 0; side < 2; ++side) {
                        uint8_t* q = (uint8_t*)Collection_Get(net, 0, side ? F<uint32_t>(p, 0x20a) : F<uint32_t>(p, 0x206));
                        if (F<int32_t>(q, 0x226) == 3) continue;
                        float h, at;
                        if (F<int32_t>(q, 0x206) == F<int32_t>(p, 0x202)) {
                            h = (float)((double)Vehicle_ScreenHeading(q, 0, 1) + Fl_207(0x5c8760));
                            at = 1.0f;
                        } else {
                            h = Vehicle_ScreenHeading(q, 0, 1);
                            at = 0.0f;
                        }
                        float y = (float)Effect_Blend(p, 0, at);
                        uint32_t hb;
                        memcpy(&hb, &h, 4);
                        Track_SetMode(q, 0, 0xa, (const char*)(p[0x218] == 4 ? 0x5cb138 : 0x5cb150), hb, 0, y, 1, -1, -1, 0, 0);
                    }
                } else {
                    b[0xdb] = 1;
                    p[0x59] |= 0x10;
                    Track_ClearProps(p);
                    Track_DropAndSettle(p, 0, 0x40400000, 1);
                    for (int32_t side = 0; side < 2; ++side) {
                        uint8_t* from = p;
                        for (int32_t n = side ? F<int32_t>(p, 0x20a) : F<int32_t>(p, 0x206); n != -1;) {
                            uint8_t* q = (uint8_t*)Collection_Get(net, 0, (uint32_t)n);
                            int32_t k = F<int32_t>(q, 0x226);
                            if (k != 3 && k != 0xc && k != 0xd) break;
                            q[0x59] |= 0x10;
                            Track_ClearProps(q);
                            Track_SetProps(q, 0, 0, 0xff, 0xff, 0);
                            Track_DropAndSettle(q, 0, 0x40400000, 1);
                            n = F<int32_t>(q, 0x206) == F<int32_t>(from, 0x202) ? F<int32_t>(q, 0x20a) : F<int32_t>(q, 0x206);
                            from = q;
                        }
                    }
                }
            } else if (kind == 1) {
                for (int32_t side = 0; side < 2; ++side) {
                    uint32_t near = side ? 0x20a : 0x206, far = side ? 0x206 : 0x20a;
                    if (F<int32_t>(p, near) == -1 || F<int32_t>(Collection_Get(net, 0, F<uint32_t>(p, near)), 0x226) == 1) continue;
                    float hc;
                    Track_FindClearance(p, F<int32_t>(p, far), &hc);
                    TrackNet_GradeRun(net, 0, p, F<int32_t>(p, far), hc, (uint8_t)force);
                    uint8_t* q = (uint8_t*)Collection_Get(net, 0, F<uint32_t>(p, near));
                    float h, at;
                    if (F<int32_t>(q, 0x206) == F<int32_t>(p, 0x202)) {
                        h = Vehicle_ScreenHeading(q, 0, 1);
                        at = 0.0f;
                    } else {
                        h = (float)((double)Vehicle_ScreenHeading(q, 0, 1) + Fl_207(0x5c8760));
                        at = 1.0f;
                    }
                    float y = (float)Effect_Blend(q, 0, at);
                    uint32_t ab;
                    if (p[0x219] == 2 && F<float>(b, 0x206) >= 0.0f) ab = F<uint32_t>(b, 0x206);
                    else memcpy(&ab, &h, 4);
                    uint32_t yb;
                    if (F<int32_t>(p, 0x226) == 1 && F<PtrList*>(p, 8)) {
                        PtrList_Rewind(F<PtrList*>(p, 8));
                        yb = F<uint32_t>(PtrList_Next(F<PtrList*>(p, 8)), 0x1e6);
                    } else
                        memcpy(&yb, &y, 4);
                    Track_SetMode(p, 0, 0xa, (const char*)(p[0x218] == 4 ? 0x5cb138 : 0x5cb150), ab, 0, BitsF(yb), 1, -1, -1, 0, 0);
                }
            }
        }
        int32_t next = -1;
        if (F<int32_t>(p, 0x20e) == -1) next = F<int32_t>(p, 0x206) == prev ? F<int32_t>(p, 0x20a) : F<int32_t>(p, 0x206);
        prev = F<int32_t>(p, 0x202);
        if (prev == start || next == -1) return;
        cur = next;
    }
}
RT3_REPLACE(0x495b80, TrackNet_FinishStructures);

 // namespace

// 0x496070. Smooths the heights along the chain of track pieces from `start`
// (in direction `dir`; with `loop`, first walks back up to 0x2b pieces and
// limits the chain to 0x2c) when not in a multiplayer game (unless `force`):
// lays each piece's ends over the ground (sampled along it, plus 4.5),
// relaxes them with three neighbour passes, flattens spikes and relaxes
// three more, then blends the result into each piece's corners by its weight
// (placed pieces fade out after 2..10 in a row, loop ends after 25..35;
// bridges, locked and station pieces keep their heights) and refreshes the
// lighting around changed pieces.
RT3_NOINLINE void __fastcall Track_SmoothHeights(Collection* tracks, int, uint32_t start, int32_t dir, int32_t loop, uint8_t force) {
    if (F<int32_t>(Tracks(), 0xed)) return;
    if (IsMultiplayer() && !force) return;
    uint8_t* p = Piece(tracks, start);
    uint8_t** chain = nullptr;
    int32_t back = 0;
    if (loop) {
        uint32_t next = F<uint32_t>(p, 0x20a);
        uint32_t self = F<uint32_t>(p, 0x202);
        dir = 1;
        while (next != 0xffffffff) {
            uint8_t* q = Piece(tracks, next);
            ++back;
            if (F<int32_t>(q, 0x20e) == -1) {
                next = F<uint32_t>(q, 0x206);
                if (next == self) next = F<uint32_t>(q, 0x20a);
                if (next != 0xffffffff) {
                    uint8_t* r = Piece(tracks, next);
                    if (F<uint32_t>(r, 0x206) == F<uint32_t>(q, 0x202)) dir = 0;
                    else if (F<uint32_t>(r, 0x20a) == F<uint32_t>(q, 0x202)) dir = 1;
                    else next = 0xffffffff;
                }
            } else {
                next = 0xffffffff;
            }
            self = F<uint32_t>(q, 0x202);
            if (self == start) next = 0xffffffff;
            if (back >= 0x2b) break;
        }
        start = self;
        p = Piece(tracks, self);
    }
    int32_t count = 0;
    for (int32_t pass = 0; pass < 2; ++pass) {
        uint32_t prev = F<uint32_t>(p, 0x202);
        uint32_t id = dir ? F<uint32_t>(p, 0x20a) : F<uint32_t>(p, 0x206);
        count = 1;
        if (id != 0xffffffff) {
            uint8_t** out = chain;
            int32_t limit = 3 - back;
            do {
                uint8_t* q = Piece(tracks, id);
                if (pass == 1) *out = q;
                ++count;
                ++out;
                id = 0xffffffff;
                ++limit;
                if (F<int32_t>(q, 0x20e) == -1) {
                    id = F<uint32_t>(q, 0x206);
                    if (id == prev) id = F<uint32_t>(q, 0x20a);
                }
                prev = F<uint32_t>(q, 0x202);
                if (prev == start) id = 0xffffffff;
            } while ((!loop || limit < 0x2c) && id != 0xffffffff);
        }
        if (pass == 0) {
            if (count <= 2) return;
            chain = (uint8_t**)CrtAlloc(count * 4 - 4);
        }
    }
    int32_t n1 = count - 1;
    float* h[9];
    for (int k = 0; k < 9; ++k) h[k] = (float*)CrtAlloc(count * 4);
    uint8_t* locked = (uint8_t*)CrtAlloc(count);
    float* avg = (float*)CrtAlloc(count * 4);
    float* weight = (float*)CrtAlloc(count * 4);
    uint8_t* forward = (uint8_t*)CrtAlloc(count);
    memset(locked, 0, count);
    for (int32_t k = 0; k < count; ++k) *(uint32_t*)&weight[k] = 0x3f800000;
    for (int32_t i = 0; i < n1; ++i) {
        if (i == 0) {
            forward[0] = dir != 0;
            continue;
        }
        uint32_t prevId = F<uint32_t>(chain[i - 1], 0x202);
        uint8_t* c = chain[i];
        if (F<uint32_t>(c, 0x206) == prevId) forward[i] = 1;
        else if (F<uint32_t>(c, 0x20a) == prevId || F<uint32_t>(c, 0x20e) != prevId) forward[i] = 0;
        else forward[i] = (c[0x201] >> 1) & 1;
    }
    int32_t run = 0;
    Corners k;
    for (int32_t i = 0; i < n1; ++i) {
        uint8_t* c = chain[i];
        uint8_t placed = c[0x216];
        if (placed && (placed != 1 || (c[0x201] & 0x10)) && !loop) {
            ++run;
            if (run > 2) {
                if (run <= 10) {
                    weight[i] = (float)(*(const double*)0x5c8770 - (double)(run - 2) / *(const double*)0x5c8970);
                } else {
                    locked[i] = 1;
                    if (i == n1 - 1) locked[i + 1] = 1;
                }
            }
        }
        if (c[0x216] == 1) c[0x201] |= 0x10;
        int32_t kind = F<int32_t>(c, 0x226);
        if (kind && (F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88) > 0 || kind != 0xc)) {
            uint32_t q = 0x3ee66666;
            if (Vehicle_CornerBelowGround(c, 0, *(float*)&q) && !((int32_t(__thiscall*)(uint8_t*))(*(void***)c)[0x14 / 4])(c) &&
                F<int32_t>(c, 0x226) != 1) {
                locked[i] = 0;
                locked[i + 1] = 0;
            } else {
                uint8_t was1 = locked[i + 1], was0 = locked[i];
                locked[i] = 1;
                locked[i + 1] = 1;
                if (F<int32_t>(c, 0x226) == 0xd) {
                    if (Collection_IsValid((Collection*)Tracks(), 0, F<uint32_t>(c, 0x206)) &&
                        F<int32_t>(Piece((Collection*)Tracks(), F<uint32_t>(c, 0x206)), 0x226) == 0xc) {
                        if (forward[i]) locked[i] = was0;
                        else locked[i + 1] = was1;
                    }
                    if (F<int32_t>(c, 0x226) == 0xd && Collection_IsValid((Collection*)Tracks(), 0, F<uint32_t>(c, 0x20a)) &&
                        F<int32_t>(Piece((Collection*)Tracks(), F<uint32_t>(c, 0x20a)), 0x226) == 0xc) {
                        if (forward[i]) locked[i + 1] = was1;
                        else locked[i] = was0;
                    }
                }
            }
        }
        if (Track_EndLinksBack(c)) {
            locked[i + 1] = 1;
            locked[i] = 1;
            uint8_t* o = Piece((Collection*)Tracks(), (uint32_t)Track_EndBackLink(c));
            if (o) {
                // A bridge end: lift both pieces' corners over the ground and match them.
                Corners m;
                GetCorners(c, &k);
                GetCorners(o, &m);
                float t1 = (float)(((double)m.b[1] + m.a[1]) * *(const double*)0x5c8628);
                float t2 = (float)(((double)m.d[1] + m.c[1]) * *(const double*)0x5c8628);
                float ya = GroundOrFloor(m.a[0], m.a[2], t1);
                float p0 = GroundOrFloor(m.b[0], m.b[2], ya);
                float yc = GroundOrFloor(m.c[0], m.c[2], t2);
                float q0 = GroundOrFloor(m.d[0], m.d[2], yc);
                float ra = (float)(Ground(k.a[0], k.a[2]) + *(const double*)0x5c8578);
                float r = GroundOrFloor(k.b[0], k.b[2], ra);
                float sc = (float)(Ground(k.c[0], k.c[2]) + *(const double*)0x5c8578);
                float s = GroundOrFloor(k.d[0], k.d[2], sc);
                bool flip = false;
                if ((F<int32_t>(o, 0x206) != -1 && F<int32_t>(o, 0x206) == F<int32_t>(c, 0x20a)) ||
                    (F<int32_t>(o, 0x20a) != -1 && F<int32_t>(o, 0x20a) == F<int32_t>(c, 0x206))) {
                    float tmp = p0;
                    p0 = q0;
                    q0 = tmp;
                    flip = true;
                }
                float e0 = p0 > r ? p0 : r;
                float e1 = q0 > s ? q0 : s;
                k.b[1] = e0;
                k.a[1] = e0;
                k.d[1] = e1;
                k.c[1] = e1;
                Reshape(c, k.a, k.b, k.c, k.d);
                float f0 = flip ? e1 : e0;
                float f1 = flip ? e0 : e1;
                m.b[1] = f0;
                m.a[1] = f0;
                m.d[1] = f1;
                m.c[1] = f1;
                Reshape(o, m.a, m.b, m.c, m.d);
            }
        }
        if (loop) {
            int32_t a = i - back;
            if (a < 0) a = -a;
            if (a >= 0x23) {
                weight[i + 1] = 0.0f;
                weight[i] = 0.0f;
            } else if (a >= 0x19) {
                float fa = (float)(a - 0x19);
                float v = (float)(*(const double*)0x5c8770 - (double)fa / *(const float*)0x5c85f8);
                weight[i + 1] = v;
                weight[i] = v;
            }
        }
        GetCorners(c, &k);
        double e0 = forward[i] ? (double)k.a[1] + k.b[1] : (double)k.c[1] + k.d[1];
        avg[i] = (float)(e0 * *(const double*)0x5c8628);
        double e1 = forward[i] ? (double)k.c[1] + k.d[1] : (double)k.a[1] + k.b[1];
        avg[i + 1] = (float)(e1 * *(const double*)0x5c8628);
    }
    for (int32_t j = 0; j < count; ++j)
        if (locked[j]) weight[j] = 0.0f;
    // Ground profile along each piece: the highest point above its straight line.
    for (int32_t i = 0; i < n1; ++i) {
        GetCorners(chain[i], &k);
        float dx = (float)((double)k.a[0] - k.d[0]);
        float dz = (float)((double)k.a[2] - k.d[2]);
        if (dir == 1) {
            dx = -dx;
            dz = -dz;
        }
        const float* p0 = dir ? k.a : k.d;
        const float* p1 = dir ? k.b : k.c;
        float x0 = p0[0], z0 = p0[2], x1 = p1[0], z1 = p1[2];
        bool found = false;
        float best = 0.0f, startH = 0.0f, endH = 0.0f;
        for (int32_t s = -2; s <= 16; ++s) {
            float t;
            if (s == -2) t = 0.0f;
            else if (s == -1) t = 1.0f;
            else if (s == 0 || s == 16) continue;
            else t = (float)((double)(float)s / *(const float*)0x5c8948);
            float tdz = (float)((double)t * dz);
            float tdx = (float)((double)t * dx);
            float h1 = (float)Ground((float)((double)tdx + x0), (float)((double)tdz + z0));
            float h2 = (float)Ground((float)((double)tdx + x1), (float)((double)tdz + z1));
            float h3 = (float)Ground((float)(((double)x0 + k.b[0]) * *(const double*)0x5c8628 + tdx),
                                     (float)(((double)z0 + k.b[2]) * *(const double*)0x5c8628 + tdz));
            float hi, lo;
            if (h2 > h1) hi = h2 > h3 ? h2 : h3;
            else hi = h1 > h3 ? h1 : h3;
            if (h2 < h1) lo = h2 < h3 ? h2 : h3;
            else lo = h1 < h3 ? h1 : h3;
            float peak = (float)(((double)hi - lo) * *(const double*)0x5c8758 + hi);
            if (s == -2) {
                startH = peak;
            } else if (s == -1) {
                endH = peak;
            } else {
                float base = (float)(((double)endH - startH) * t + startH);
                float d = (float)((double)peak - base);
                if (!found || !(d < best)) {
                    best = d;
                    found = true;
                }
            }
        }
        if (found && best > *(const float*)0x5c8598) {
            startH = (float)((double)best + startH);
            endH = (float)((double)best + endH);
        }
        if (i == 0) h[0][0] = startH;
        else h[0][i] = h[0][i] > startH ? h[0][i] : startH;
        h[0][i + 1] = endH;
    }
    for (int32_t j = 0; j < count; ++j) h[1][j] = locked[j] ? avg[j] : (float)((double)h[0][j] + *(const double*)0x5c8c30);
    int32_t level = 1;
    // Three relaxing passes, floored at the ground plus 1.5.
    for (int32_t pass = 0; pass < 3; ++pass) {
        ++level;
        const float* src = h[level - 1];
        float* dst = h[level];
        for (int32_t j = 0; j < count; ++j) {
            if (j == 0 || j == n1 || locked[j]) {
                dst[j] = src[j];
                continue;
            }
            float v = (float)(((double)src[j - 1] + src[j + 1]) * *(const double*)0x5c8758 + (double)src[j] * *(const double*)0x5c8628);
            double floor = (double)h[0][j] + *(const double*)0x5c8578;
            dst[j] = (double)v < floor ? (float)floor : v;
        }
    }
    ++level;
    memcpy(h[level], h[level - 1], count * 4);
    // Flatten spikes: share a peak more than 1 above both neighbours with three on each side.
    {
        float* a = h[level];
        for (int32_t j = 0; j < count; ++j) {
            if (j == 0 || j == n1 || locked[j - 1] || locked[j] || locked[j + 1]) continue;
            if (!((double)a[j - 1] + *(const float*)0x5c85b4 < a[j])) continue;
            if (!((double)a[j + 1] + *(const float*)0x5c85b4 < a[j])) continue;
            float d = (float)((double)a[j] - a[j + 1]);
            double dd = d;
            a[j + 1] = (float)(dd * *(const double*)0x5c8628 + a[j + 1]);
            if (j + 2 >= 0 && j + 2 < count) a[j + 2] = (float)(dd * *(const double*)0x5c8758 + a[j + 2]);
            if (j + 3 >= 0 && j + 3 < count) a[j + 3] = (float)(dd * *(const double*)0x5c86b8 + a[j + 3]);
            float d2 = (float)((double)a[j] - a[j - 1]);
            double dd2 = d2;
            a[j - 1] = (float)(dd2 * *(const double*)0x5c8628 + a[j - 1]);
            if (j - 2 >= 0 && j - 2 < count) a[j - 2] = (float)(dd2 * *(const double*)0x5c8758 + a[j - 2]);
            if (j - 3 >= 0 && j - 3 < count) a[j - 3] = (float)(dd2 * *(const double*)0x5c86b8 + a[j - 3]);
        }
    }
    // Three more passes towards the neighbours' middle, keeping the ground clearance.
    for (int32_t pass = 0; pass < 3; ++pass) {
        ++level;
        memcpy(h[level], h[level - 1], count * 4);
        const float* src = h[level - 1];
        float* dst = h[level];
        for (int32_t j = 0; j < count; ++j) {
            if (j == 0 || j == n1 || locked[j]) continue;
            float mid = (float)((double)(float)((double)src[j + 1] - src[j - 1]) * *(const double*)0x5c8628 + src[j - 1]);
            if (mid > src[j]) {
                dst[j] = (float)(((double)mid - src[j]) * *(const double*)0x5c88e8 + dst[j]);
                continue;
            }
            float d = (float)((double)src[j] - mid);
            double t = (double)d * *(const double*)0x5c8c20;
            float now = (float)((double)dst[j] - t);
            dst[j] = now;
            float lift = (float)((double)(float)((double)h[0][j] + *(const double*)0x5c8578) - now);
            if (lift > *(const float*)0x5c8598) {
                dst[j] = (float)((double)now + lift);
                if (!locked[j - 1]) dst[j - 1] = (float)((double)lift + dst[j - 1]);
                if (!locked[j + 1]) dst[j + 1] = (float)((double)lift + dst[j + 1]);
            }
            if (!locked[j - 1]) dst[j - 1] = (float)((double)dst[j - 1] + t);
            if (!locked[j + 1]) dst[j + 1] = (float)((double)dst[j + 1] + t);
        }
    }
    // The original blends from the next-to-last level.
    const float* fin = h[level - 1];
    for (int32_t i = 0; i < n1; ++i) {
        uint8_t* c = chain[i];
        if (locked[i] || locked[i + 1]) continue;
        if (!(weight[i] > 0.0f || weight[i] != weight[i]) && !(weight[i + 1] > 0.0f || weight[i + 1] != weight[i + 1])) continue;
        GetCorners(c, &k);
        float v0 = (float)((*(const double*)0x5c8770 - weight[i]) * avg[i] + (double)fin[i] * weight[i]);
        float v1 = (float)((*(const double*)0x5c8770 - weight[i + 1]) * avg[i + 1] + (double)fin[i + 1] * weight[i + 1]);
        float e0 = forward[i] ? v0 : v1;
        float e1 = forward[i] ? v1 : v0;
        k.b[1] = e0;
        k.a[1] = e0;
        k.d[1] = e1;
        k.c[1] = e1;
        Reshape(c, k.a, k.b, k.c, k.d);
    }
    for (int32_t i = 0; i < n1; ++i) {
        uint8_t* c = chain[i];
        auto live = [&](int32_t j) { return weight[j] > 0.0f || weight[j] != weight[j]; };
        if (live(i) || live(i + 1) || (i != 0 && live(i - 1)) || (i != n1 - 1 && live(i + 2))) Node_UpdateLightFromNeighbors(c);
    }
    for (int k2 = 0; k2 < 9; ++k2) CrtRelease(h[k2]);
    CrtRelease(forward);
    CrtRelease(weight);
    CrtRelease(locked);
    CrtRelease(avg);
    CrtRelease(chain);
}
RT3_REPLACE(0x496070, Track_SmoothHeights);

// 0x497ea0. Walks the track from node `start` in direction dir (+0x20a ahead, else
// +0x206) until a junction (+0x20e), the end or back at the start, relighting the
// nodes whose stamp (+0x240) is current (collection +0x1d5).
RT3_NOINLINE void __fastcall Track_RelightChain(Collection* c, int, int32_t start, int32_t dir, int32_t) {
    uint8_t* n = (uint8_t*)Collection_Get(c, 0, start);
    int32_t prev = F<int32_t>(n, 0x202);
    int32_t next = dir ? F<int32_t>(n, 0x20a) : F<int32_t>(n, 0x206);
    while (next != -1) {
        uint8_t* m = (uint8_t*)Collection_Get(c, 0, next);
        if (F<uint32_t>(m, 0x240) >= F<uint32_t>(c, 0x1d5)) Node_UpdateLightFromNeighbors(m);
        if (F<int32_t>(m, 0x20e) != -1) next = -1;
        else next = F<int32_t>(m, 0x206) == prev ? F<int32_t>(m, 0x20a) : F<int32_t>(m, 0x206);
        prev = F<int32_t>(m, 0x202);
        if (prev == start) break;
    }
}
RT3_REPLACE(0x497ea0, Track_RelightChain);

// 0x497f30. Track network net: finishes the run laid from piece `start` in
// both directions - overpasses, bridges / tunnels, height smoothing and the
// signal lights.
RT3_NOINLINE void __fastcall TrackNet_FinishRun(Collection* net, int, int32_t start, int32_t mode) {
    if (start == -1) return;
    for (int32_t dir = 0; dir < 2; ++dir) {
        TrackNet_MarkOverpasses(net, 0, start, (uint8_t)mode, dir);
        TrackNet_FinishStructures(net, 0, start, mode, dir);
        Track_SmoothHeights(net, 0, (uint32_t)start, dir, 0, (uint8_t)mode);
        Track_RelightChain(net, 0, start, mode, dir);
    }
}
RT3_REPLACE(0x497f30, TrackNet_FinishRun);

// 0x497f80. Re-settles the scenery group (+0x24d) of track piece `id`: walks from
// it in both directions (+0x206, then +0x20a) over the pieces of the same group,
// then drops each piece of that stretch onto the terrain and resets its kind,
// first found to last. Returns whether any stretch was found.
RT3_NOINLINE int32_t __fastcall Tracks_SettleGroup(Collection* c, int, uint32_t id) {
    uint8_t* d = (uint8_t*)Collection_Get(c, 0, id);
    int32_t g = F<int32_t>(d, 0x24d);
    int32_t changed = 0;
    if (g == -1 || g == 0) return 0;
    for (int dir = 0; dir < 2; ++dir) {
        int32_t first = -1, last = -1;
        int32_t cur = F<int32_t>(d, dir ? 0x20a : 0x206);
        if (cur == -1) continue;
        int32_t prev = (int32_t)id;
        for (;;) {
            uint8_t* n = (uint8_t*)Collection_Get(c, 0, (uint32_t)cur);
            if (F<int32_t>(n, 0x24d) != g) break;
            if (first == -1) first = cur;
            last = cur;
            int32_t next = F<int32_t>(n, 0x206);
            if (next == -1 || next == prev) {
                next = F<int32_t>(n, 0x20a);
                if (next == -1 || next == prev) break;
            }
            prev = F<int32_t>(n, 0x202);
            cur = next;
        }
        if (first == -1) continue;
        changed = 1;
        prev = (int32_t)id;
        for (int32_t at = first; at != -1;) {
            uint8_t* p = (uint8_t*)Collection_Get(c, 0, (uint32_t)at);
            Track_DropAndSettle(p, 0, 0x40400000, 1);
            Node_ResetKind(p);
            if (at == last) break;
            int32_t next = F<int32_t>(p, 0x206);
            if (next == -1 || next == prev) {
                next = F<int32_t>(p, 0x20a);
                if (next == -1 || next == prev) break;
            }
            prev = at;
            at = next;
        }
    }
    return changed;
}
RT3_REPLACE(0x497f80, Tracks_SettleGroup);

// 0x4980c0. Whether the straight line from (x0, z0) to (x1, z1) stays on the map and
// off water, sampled every tenth of a tile along the major axis.
RT3_NOINLINE int32_t __stdcall Map_LineOnLand(float x0, float z0, float x1, float z1) {
    float dx = (float)((double)x1 - x0), dz = (float)((double)z1 - z0);
    double major = Fabs(Fabs(dz) < Fabs(dx) ? dx : dz);
    int32_t n = Ftol(major / D(0x5c86b8));
    float slope = x1 == x0 ? 1e8f : (float)Fabs((double)dz / dx);
    float sx, sz;
    if (slope < D(0x5c8770)) {
        sx = (float)(x1 > x0 ? Bits(0x3fb999999999999aull) : Bits(0xbfb999999999999aull));
        sz = (float)((double)slope * (z1 > z0 ? D(0x5c86b8) : D(0x5cfe40)));
    } else {
        double inv = D(0x5c8770) / slope;
        sx = (float)(inv * (x1 > x0 ? D(0x5c86b8) : D(0x5cfe40)));
        sz = (float)(z1 > z0 ? Bits(0x3fb999999999999aull) : Bits(0xbfb999999999999aull));
    }
    for (int32_t i = 0; i <= n; ++i) {
        float fi = (float)i;
        float px = (float)((double)fi * sx + x0);
        float pz = (float)((double)fi * sz + z0);
        if (!LandAt(px, pz)) return 0;
    }
    return LandAt(x1, z1) ? 1 : 0;
}
RT3_REPLACE(0x4980c0, Map_LineOnLand);

// 0x498320. Plans a curve from the builder's current piece (ends +0x139 and
// +0x15d, centre their mean, direction +0x145) to piece `bId` of collection
// `t` (its ends from 0x5394b0). Each end's travel direction comes from its
// free links, or from which end lies toward the other piece. Unless the two
// directions already agree, it puts circles of radius 3.62 on both sides of
// each end and takes the shortest of the four tangent paths (outer left,
// outer right and the two crossings, arcs included). *out gets the list
// {start of tangent} -> {end of tangent} -> {p4, p5} (12-byte nodes, next at
// +8). If that is longer than twice the straight distance it tries once more
// with radius 1.81. Returns 0 only when both tries are too long (the list is
// then freed); 1 otherwise, also when no curve is needed.
RT3_NOINLINE int32_t __fastcall Track_PlanCurve(uint8_t* t, int, uint32_t aId, uint32_t bId, uint32_t p4, uint32_t p5, uint32_t** out) {
    if (aId == bId) return 1;
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    auto atan_ = (double(__cdecl*)(double))0x5a1270;
    auto fabs_ = (double(__cdecl*)(double))0x5a152e;
    auto asin_ = (double(__cdecl*)(double))0x5a2f84;
    auto malloc_ = (void*(__cdecl*)(size_t))0x5a125d;
    auto free_ = (void(__cdecl*)(void*))0x5a1145;
    int32_t pass = 0;
    uint32_t* prev = nullptr;
    float r = BitsF(0x4067ae14u);
    uint8_t* A = (uint8_t*)Collection_Get((Collection*)t, 0, aId);
    uint8_t* B = (uint8_t*)Collection_Get((Collection*)t, 0, bId);
    const float k = Fl_205(0x5c8700);
    float cx = (float)((((double)F<float>(t, 0x151) + F<float>(t, 0x139)) / Fl_205(0x5c878c)) / k);
    float cz = (float)((((double)F<float>(t, 0x159) + F<float>(t, 0x141)) / Fl_205(0x5c878c)) / k);
    float bx, bz;
    Obj_CenterCell(B, 0, &bx, &bz);
    float d0 = (float)Distance2D(cx, cz, bx, bz);
    float p0x = F<float>(t, 0x139), p0z = F<float>(t, 0x141), dirx = F<float>(t, 0x145), dirz = F<float>(t, 0x14d);
    float p1x = F<float>(t, 0x15d), p1z = F<float>(t, 0x165);
    uint32_t qa[3], qd[3], qb[3];
    Effect_GetVectors(B, 0, qa, qd, nullptr, qb);
    float q0x = BitsF(qa[0]), q0z = BitsF(qa[2]), dqx = BitsF(qd[0]), dqz = BitsF(qd[2]), q1x = BitsF(qb[0]), q1z = BitsF(qb[2]);
    int32_t aF1 = Track_HasLink(A, 0, 1), aF2 = Track_HasLink(A, 0, 2);
    int32_t bF1 = Track_HasLink(B, 0, 1), bF2 = Track_HasLink(B, 0, 2);
    if (!aF1 && !aF2) {
        float dA = (float)DistanceSq2D((float)(p0x / k), (float)(p0z / k), bx, bz);
        if (!(DistanceSq2D((float)(p1x / k), (float)(p1z / k), bx, bz) > dA)) aF1 = 1;
        else aF2 = 1;
    }
    if (!bF1 && !bF2) {
        float dA = (float)DistanceSq2D((float)(q0x / k), (float)(q0z / k), cx, cz);
        if (!(DistanceSq2D((float)(q1x / k), (float)(q1z / k), cx, cz) > dA)) bF1 = 1;
        else bF2 = 1;
    }
    auto slope = [](float dz, float dx) {
        if (dz == Fl_205(0x5c8598)) dz = BitsF(0x38d1b717u);
        if (dx == Fl_205(0x5c8598)) dx = BitsF(0x38d1b717u);
        return (float)(Fl_205(0x5c9b08) / ((double)dz / dx));
    };
    auto nz = [](float& v) {
        if (v == Fl_205(0x5c8598)) v = BitsF(0x38d1b717u);
    };
    for (;;) {
        float angA;
        if (!aF1)
            angA = (float)AngleOf(p1x, p1z, p0x, p0z);
        else if (!aF2)
            angA = (float)AngleOf(p0x, p0z, p1x, p1z);
        else {
            float m = slope(p1z - p0z, p1x - p0x);
            bool below = (double)p0z / k < ((double)p0x / k - cx) * m + cz;
            bool gt = ((double)bx - cx) * m + cz > bz;
            angA = below == gt ? (float)AngleOf(p1x, p1z, p0x, p0z) : (float)AngleOf(p0x, p0z, p1x, p1z);
        }
        float angB;
        if (!bF1)
            angB = (float)AngleOf(q0x, q0z, q1x, q1z);
        else if (!bF2)
            angB = (float)AngleOf(q1x, q1z, q0x, q0z);
        else {
            float m = slope(q1z - q0z, q1x - q0x);
            bool below = (double)q0z / k < ((double)q0x / k - bx) * m + bz;
            bool gt = ((double)cx - bx) * m + bz > cz;
            angB = below == gt ? (float)AngleOf(q0x, q0z, q1x, q1z) : (float)AngleOf(q1x, q1z, q0x, q0z);
        }
        float diff = (float)fabs_(AngleDelta(nullptr, 0, angA, angB));
        if (diff < D(0x5c88e8)) {
            float mid = (float)AngleOf((float)(((double)dirx + p0x) / Fl_205(0x5c878c)), (float)(((double)dirz + p0z) / Fl_205(0x5c878c)),
                                       (float)(((double)dqx + q0x) / Fl_205(0x5c878c)), (float)(((double)dqz + q0z) / Fl_205(0x5c878c)));
            diff = (float)fabs_(AngleDelta(nullptr, 0, angA, mid));
            if (diff < D(0x5c88e8)) return 1;
        }
        // Circle centres beside each end (left = -90 degrees, right = +90).
        auto side = [&](float ang, double off, float ox, float oz, float& x, float& z) {
            float a = (float)(ang + off);
            x = (float)(sin_(a) * r + ox);
            z = (float)(cos_(a) * r + oz);
        };
        float aLx, aLz, aRx, aRz, bLx, bLz, bRx, bRz;
        side(angA, -(double)Fl_205(0x5c8764), cx, cz, aLx, aLz);
        side(angA, Fl_205(0x5c8764), cx, cz, aRx, aRz);
        side(angB, -(double)Fl_205(0x5c8764), bx, bz, bLx, bLz);
        side(angB, Fl_205(0x5c8764), bx, bz, bRx, bRz);
        float d1 = bLx - aLx, d2 = bLx - aRx, d3 = bRx - aLx, d4 = bRx - aRx;
        nz(d1);
        nz(d2);
        nz(d3);
        nz(d4);
        float s1 = (float)(((double)bLz - aLz) / d1), s2 = (float)(((double)bLz - aRz) / d2);
        float s3 = (float)(((double)bRz - aLz) / d3), s4 = (float)(((double)bRz - aRz) / d4);
        float dLL = (float)Distance2D(aLx, aLz, bLx, bLz), dRL = (float)Distance2D(aRx, aRz, bLx, bLz);
        float dLR = (float)Distance2D(aLx, aLz, bRx, bRz), dRR = (float)Distance2D(aRx, aRz, bRx, bRz);
        float ang1 = (float)AngleOf(aLx, aLz, bLx, bLz), ang2 = (float)AngleOf(aRx, aRz, bRx, bRz);
        nz(s1);
        nz(s4);
        float t1 = (float)atan_(D(0x5c9bf0) / s1), t2 = (float)atan_(D(0x5c9bf0) / s4);
        // Outer tangents: points r along the normal of the centre line.
        float T1sx, T1sz, T1ex, T1ez, T2sx, T2sz, T2ex, T2ez;
        if (ang1 < Fl_205(0x5c8760) && !(ang1 <= D(0x5c8608))) {
            T1sx = (float)((double)aLx - sin_(t1) * r);
            T1sz = (float)((double)aLz - cos_(t1) * r);
            T1ex = (float)((double)bLx - sin_(t1) * r);
            T1ez = (float)((double)bLz - cos_(t1) * r);
        } else {
            T1sx = (float)(sin_(t1) * r + aLx);
            T1sz = (float)(cos_(t1) * r + aLz);
            T1ex = (float)(sin_(t1) * r + bLx);
            T1ez = (float)(cos_(t1) * r + bLz);
        }
        if (ang2 < Fl_205(0x5c8760) && !(ang2 <= D(0x5c8608))) {
            T2sx = (float)(sin_(t2) * r + aRx);
            T2sz = (float)(cos_(t2) * r + aRz);
            T2ex = (float)(sin_(t2) * r + bRx);
            T2ez = (float)(cos_(t2) * r + bRz);
        } else {
            T2sx = (float)((double)aRx - sin_(t2) * r);
            T2sz = (float)((double)aRz - cos_(t2) * r);
            T2ex = (float)((double)bRx - sin_(t2) * r);
            T2ez = (float)((double)bRz - cos_(t2) * r);
        }
        float twoR = r + r;
        // Crossing tangents (left of A to right of B and back); x = -1 when
        // the circles overlap.
        float T3sx = 0.0f, T3sz = 0.0f, T3ex = 0.0f, T3ez = 0.0f, T4sx = 0.0f, T4sz = 0.0f, T4ex = 0.0f, T4ez = 0.0f;
        if (dLR < twoR)
            T3sx = -1.0f;
        else {
            float dd = dLR;
            if (dd == Fl_205(0x5c8598)) dd = BitsF(0x38d1b717u);
            double as = asin_((double)twoR / dd);
            float base = (float)(atan_(s3) + as);
            float other = (float)(base + Fl_205(0x5c8760));
            float sa = bRx < aLx ? other : base, ea = bRx < aLx ? base : other;
            T3sx = (float)(sin_(sa) * r + aLx);
            T3sz = (float)(cos_(sa) * r + aLz);
            T3ex = (float)(sin_(ea) * r + bRx);
            T3ez = (float)(cos_(ea) * r + bRz);
        }
        if (dRL < twoR)
            T4sx = -1.0f;
        else {
            float dd = dRL;
            if (dd == Fl_205(0x5c8598)) dd = BitsF(0x38d1b717u);
            double at = atan_(s2);
            float base = (float)(at - asin_((double)twoR / dd));
            float other = (float)(base + Fl_205(0x5c8760));
            float sa = bLx < aRx ? other : base, ea = bLx < aRx ? base : other;
            T4sx = (float)(sin_(sa) * r + aRx);
            T4sz = (float)(cos_(sa) * r + aRz);
            T4ex = (float)(sin_(ea) * r + bLx);
            T4ez = (float)(cos_(ea) * r + bLz);
        }
        // Arc lengths (as angles) on each circle, wrapped to [0, 2 pi).
        auto wrap = [](float v) { return v < D(0x5c8608) ? (float)(v + Fl_205(0x5c8768)) : v; };
        float hA = (float)AngleOf(aLx, aLz, cx, cz);
        float aL1 = wrap((float)(hA - (float)AngleOf(aLx, aLz, T1sx, T1sz)));
        float aL3 = wrap((float)(hA - (float)AngleOf(aLx, aLz, T3sx, T3sz)));
        float hR = (float)AngleOf(aRx, aRz, cx, cz);
        float aR2 = wrap((float)((float)AngleOf(aRx, aRz, T2sx, T2sz) - hR));
        float aR4 = wrap((float)((float)AngleOf(aRx, aRz, T4sx, T4sz) - hR));
        float hBL = (float)AngleOf(bLx, bLz, bx, bz);
        float bL1 = wrap((float)((float)AngleOf(bLx, bLz, T1ex, T1ez) - hBL));
        float bL4 = wrap((float)((float)AngleOf(bLx, bLz, T4ex, T4ez) - hBL));
        float hBR = (float)AngleOf(bRx, bRz, bx, bz);
        float bR3 = wrap((float)(hBR - (float)AngleOf(bRx, bRz, T3ex, T3ez)));
        float bR2 = wrap((float)(hBR - (float)AngleOf(bRx, bRz, T2ex, T2ez)));
        float len1 = (float)(((double)bL1 * r + (double)aL1 * r) + dLL);
        float len2 = (float)(((double)bR2 * r + (double)aR2 * r) + dRR);
        float len3 = T3sx < D(0x5c8608) ? BitsF(0x4e6e6b28u) : (float)((Distance2D(T3sx, T3sz, T3ex, T3ez) + (double)bR3 * r) + (double)aL3 * r);
        float len4 = T4sx < D(0x5c8608) ? BitsF(0x4e6e6b28u) : (float)((Distance2D(T4sx, T4sz, T4ex, T4ez) + (double)bL4 * r) + (double)aR4 * r);
        uint32_t* n1 = (uint32_t*)malloc_(0xc);
        *out = n1;
        uint32_t* n2 = (uint32_t*)malloc_(0xc);
        n1[2] = (uint32_t)(uintptr_t)n2;
        uint32_t* n3 = (uint32_t*)malloc_(0xc);
        n2[2] = (uint32_t)(uintptr_t)n3;
        n3[0] = p4;
        n3[1] = p5;
        n3[2] = 0;
        float best;
        auto set = [&](float sx, float sz, float ex, float ez, float len) {
            n1[0] = FBits(sx);
            n1[1] = FBits(sz);
            n2[0] = FBits(ex);
            n2[1] = FBits(ez);
            best = len;
        };
        if (len1 < len2 && len1 < len3 && len1 < len4)
            set(T1sx, T1sz, T1ex, T1ez, len1);
        else if (len2 < len3 && len2 < len4)
            set(T2sx, T2sz, T2ex, T2ez, len2);
        else if (len3 < len4)
            set(T3sx, T3sz, T3ex, T3ez, len3);
        else
            set(T4sx, T4sz, T4ex, T4ez, len4);
        auto freeList = [&](uint32_t* l) {
            while (l) {
                uint32_t* next = (uint32_t*)(uintptr_t)l[2];
                free_(l);
                l = next;
            }
        };
        bool tooLong = (double)d0 + d0 < best;
        if (pass == 0) {
            if (!tooLong) return 1;
            pass = 1;
            r = BitsF(0x3fe7ae14u);
            prev = *out;
            continue;
        }
        if (pass != 1) return 1;
        if (!tooLong) {
            freeList(prev);
            return 1;
        }
        freeList(*out);
        *out = nullptr;
        freeList(prev);
        return 0;
    }
}
RT3_REPLACE(0x498320, Track_PlanCurve);

// 0x4996e0. Track network net: smooths the curve where a new run joins at
// piece `from` towards `to` (two passes, one per side of `from`). Walking the
// route from `from` (when its next step is that side's neighbour), each
// following piece whose heading turns by at least `maxTurn` (and not by
// nearly a half turn) is bent towards the run by maxTurn: its ends are
// recomputed from the midpoints of its sides and its neighbours' shared
// ends follow. When the walk ends at `to` the last piece's free ends are
// re-aimed. Building (mode 1 / 2) then, for the last bent piece, either
// splits a long piece into pieces of the standard length (Tracks_AddPiece,
// registering them on its route) or drops a too-short one, relinking its
// neighbours (and merging their routes).
// QUIRK: the side of a piece not linked back to the previous one stays
// that of the previous piece (stack residue for the very first, 0 here);
// the piece count and the last piece carry over into the second pass when
// that pass does not walk; dropping a short piece releases a route
// reference by the piece's id rather than its route's.
RT3_NOINLINE void __fastcall TrackNet_SmoothJoin(Collection* net, int, int32_t from, int32_t to, int32_t mode, int32_t a4, int32_t a5, float maxTurn) {
    auto fabs_ = (double(__cdecl*)(double))0x5a152e;
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    Collection* routes = g_routes;
    uint8_t mb = (uint8_t)mode;
    bool build = mb == 1 || mb == 2;
    uint32_t cost;
    int32_t next = Route_NextStep(routes, 0, from, to, &cost, 0, nullptr, nullptr, 0);
    TrackVec a, b, c, d, v78, v84, v90, v9c, va8, vb4, vc0, vd0;
    uint8_t* n = nullptr;  // ESI: the last neighbour, kept across passes
    int32_t side = 0, count = 0;
    for (int32_t pass = 0; pass < 2; ++pass) {
        uint8_t* cur = (uint8_t*)Collection_Get(net, 0, (uint32_t)from);
        int32_t prevId = from, target = to;
        int32_t first = pass == 0;
        bool done = false;
        int32_t nb = first ? F<int32_t>(cur, 0x206) : F<int32_t>(cur, 0x20a);
        int32_t prev = F<int32_t>(cur, 0x202);
        if (nb == next && (int32_t)cost != -1) {
            for (int32_t steps = 0; !done; ++steps) {
                if (!Collection_IsValid(net, 0, (uint32_t)nb) || F<int32_t>(Collection_Get(net, 0, (uint32_t)nb), 0x226) || steps > 200) {
                    target = prev;
                    done = true;
                }
                if (nb == to) done = true;
                else if (!done) {
                    int32_t e = F<int32_t>(Collection_Get(net, 0, (uint32_t)nb), 0x206);
                    if (e == prev) e = F<int32_t>(Collection_Get(net, 0, (uint32_t)nb), 0x20a);
                    prev = nb;
                    nb = e;
                }
            }
            done = false;
            count = 0;
        } else
            done = true;
        while (!done) {
            nb = first ? F<int32_t>(cur, 0x206) : F<int32_t>(cur, 0x20a);
            if (nb == target) {
                uint8_t* t = (uint8_t*)Collection_Get(net, 0, (uint32_t)nb);
                Effect_GetVectors(t, 0, a.u, b.u, c.u, d.u);
                Effect_GetVectors(cur, 0, v78.u, vc0.u, vb4.u, v84.u);
                float m1x, m1y, m2x, m2y;
                if (first) {
                    if (F<int32_t>(t, 0x206) == prevId) v78 = b, vc0 = a;
                    else v78 = d, vc0 = c;
                    Midpoint(&m1x, &m1y, vb4.f[0], vb4.f[2], v84.f[0], v84.f[2]);
                    Midpoint(&m2x, &m2y, v78.f[0], v78.f[2], vc0.f[0], vc0.f[2]);
                    float ref = (float)AngleOf(m1x, m1y, m2x, m2y);
                    float t1 = (float)AngleOf(v84.f[0], v84.f[2], v78.f[0], v78.f[2]);
                    float t2 = (float)AngleOf(v84.f[0], v84.f[2], vc0.f[0], vc0.f[2]);
                    double d2 = fabs_(AngleDelta(nullptr, 0, t2, ref));
                    if (fabs_(AngleDelta(nullptr, 0, t1, ref)) > d2) SwapVec(v78, vc0);
                    Tracks_MoveEnds(net, 0, F<uint32_t>(cur, 0x202), v78.u, vc0.u, nullptr, nullptr);
                } else {
                    if (F<int32_t>(t, 0x206) == prevId) vb4 = b, v84 = a;
                    else vb4 = d, v84 = c;
                    Midpoint(&m1x, &m1y, v78.f[0], v78.f[2], vc0.f[0], vc0.f[2]);
                    Midpoint(&m2x, &m2y, v84.f[0], v84.f[2], vb4.f[0], vb4.f[2]);
                    float ref = (float)AngleOf(m1x, m1y, m2x, m2y);
                    float t1 = (float)AngleOf(v78.f[0], v78.f[2], v84.f[0], v84.f[2]);
                    float t2 = (float)AngleOf(v78.f[0], v78.f[2], vb4.f[0], vb4.f[2]);
                    double d2 = fabs_(AngleDelta(nullptr, 0, t2, ref));
                    if (fabs_(AngleDelta(nullptr, 0, t1, ref)) > d2) SwapVec(vb4, v84);
                    Tracks_MoveEnds(net, 0, F<uint32_t>(cur, 0x202), nullptr, nullptr, vb4.u, v84.u);
                }
                break;
            }
            if (nb == -1) break;
            n = (uint8_t*)Collection_Get(net, 0, (uint32_t)nb);
            Effect_GetVectors(cur, 0, v78.u, nullptr, nullptr, v84.u);
            float heading = first ? (float)AngleOf(v84.f[0], v84.f[2], v78.f[0], v78.f[2]) : (float)AngleOf(v78.f[0], v78.f[2], v84.f[0], v84.f[2]);
            if (F<int32_t>(n, 0x206) == prevId) side = 0;
            else if (F<int32_t>(n, 0x20a) == prevId) side = 1;
            else if (F<int32_t>(n, 0x20e) == prevId) side = (uint8_t)~n[0x201] >> 1 & 1;
            Effect_GetVectors(n, 0, a.u, b.u, c.u, d.u);
            if (count > 0) {
                if (side) {
                    if (d.f[0] == v90.f[0] && d.f[2] == v90.f[2]) d = v9c, c = va8;
                    else if (c.f[0] == v90.f[0] && c.f[2] == v90.f[2]) c = v9c, d = va8;
                } else {
                    if (a.f[0] == v90.f[0] && a.f[2] == v90.f[2]) a = v9c, b = va8;
                    else if (b.f[0] == v90.f[0] && b.f[2] == v90.f[2]) b = v9c, a = va8;
                }
                Tracks_MoveEnds(net, 0, F<uint32_t>(n, 0x202), a.u, b.u, c.u, d.u);
            }
            float m1x, m1z, m2x, m2z;
            Midpoint(&m1x, &m1z, a.f[0], a.f[2], b.f[0], b.f[2]);
            Midpoint(&m2x, &m2z, c.f[0], c.f[2], d.f[0], d.f[2]);
            float along = side ? (float)AngleOf(m2x, m2z, m1x, m1z) : (float)AngleOf(m1x, m1z, m2x, m2z);
            int32_t wrapped;
            float turn = (float)fabs_(AngleDelta(&wrapped, 0, heading, along));
            if (turn < maxTurn) break;
            if (D(0x5c8c08) - (double)maxTurn < (double)turn) break;
            float angle;
            if (!wrapped) angle = heading < along ? (float)((double)heading + maxTurn) : (float)((double)heading - maxTurn);
            else angle = AngleAdd(nullptr, 0, heading, heading < along ? -maxTurn : maxTurn);
            if ((double)turn > D(0x5c85d0)) angle = heading;
            float e = (float)((double)angle + Fl_207(0x5c8764));
            if (e > Fl_207(0x5c8760)) e = (float)((double)e - Fl_207(0x5c8760));
            float ce = (float)cos_((double)e), se = (float)sin_((double)e);
            if (side) {
                float ox = (float)((double)m2x / Fl_207(0x5c8700)), oz = (float)((double)m2z / Fl_207(0x5c8700));
                float px = (float)(sin_((double)angle) * D(0x5c97e0) + ox);
                float pz = (float)(cos_((double)angle) * D(0x5c97e0) + oz);
                v90.u[0] = a.u[0];
                v90.u[2] = a.u[2];
                double hx = (double)se * D(0x5c97d8);
                a.f[0] = (float)(((double)px - hx) * D(0x5c8c48));
                b.f[0] = (float)(((double)px + hx) * D(0x5c8c48));
                double hz = (double)ce * D(0x5c97d8);
                a.f[2] = (float)(((double)pz - hz) * D(0x5c8c48));
                b.f[2] = (float)(((double)pz + hz) * D(0x5c8c48));
                float t1 = (float)AngleOf(d.f[0], d.f[2], a.f[0], a.f[2]);
                float t2 = (float)AngleOf(d.f[0], d.f[2], b.f[0], b.f[2]);
                double d2 = fabs_(AngleDelta(nullptr, 0, t2, angle));
                if (fabs_(AngleDelta(nullptr, 0, t1, angle)) > d2) SwapVec(a, b);
                v9c = a;
                va8 = b;
            } else {
                float ox = (float)((double)m1x / Fl_207(0x5c8700)), oz = (float)((double)m1z / Fl_207(0x5c8700));
                float px = (float)(sin_((double)angle) * D(0x5c97e0) + ox);
                float pz = (float)(cos_((double)angle) * D(0x5c97e0) + oz);
                v90 = d;
                double hx = (double)se * D(0x5c97d8);
                d.f[0] = (float)(((double)px + hx) * D(0x5c8c48));
                c.f[0] = (float)(((double)px - hx) * D(0x5c8c48));
                double hz = (double)ce * D(0x5c97d8);
                d.f[2] = (float)(((double)pz + hz) * D(0x5c8c48));
                c.f[2] = (float)(((double)pz - hz) * D(0x5c8c48));
                float t1 = (float)AngleOf(a.f[0], a.f[2], d.f[0], d.f[2]);
                float t2 = (float)AngleOf(a.f[0], a.f[2], c.f[0], c.f[2]);
                double d2 = fabs_(AngleDelta(nullptr, 0, t2, angle));
                if (fabs_(AngleDelta(nullptr, 0, t1, angle)) > d2) SwapVec(d, c);
                v9c = d;
                va8 = c;
            }
            Tracks_MoveEnds(net, 0, F<uint32_t>(n, 0x202), a.u, b.u, c.u, d.u);
            ++count;
            prevId = nb;
            cur = n;
            first = side;
        }
        if (!build || count < 1 || !n || F<int32_t>(n, 0x226)) continue;
        float len = (float)Vehicle_Length(n);
        if ((double)len > D(0x5cfe50)) {
            int32_t k = Ftol(((double)len + D(0x5c97d8)) / D(0x5cfe48));
            int32_t extra = -1 - k;
            float seg = (float)(-k);
            seg = (float)((double)len / seg);
            float step = (float)((double)seg * Fl_207(0x5c8700));
            int32_t far = side ? F<int32_t>(n, 0x206) : F<int32_t>(n, 0x20a);
            if (far == -1) continue;
            Effect_GetVectors(n, 0, a.u, b.u, c.u, d.u);
            float dir = side ? (float)AngleOf(d.f[0], d.f[2], a.f[0], a.f[2]) : (float)AngleOf(a.f[0], a.f[2], d.f[0], d.f[2]);
            float s = (float)sin_((double)dir);
            float co = (float)cos_((double)dir);
            Track_Unlink(n, 0, far);
            Track_Unlink((uint8_t*)Collection_Get(net, 0, (uint32_t)far), 0, F<int32_t>(n, 0x202));
            float dx = (float)((double)s * step);
            float dz = (float)((double)co * step);
            if (side) {
                v9c = a;
                va8 = b;
                a.f[0] = (float)((double)dx + d.f[0]);
                a.f[2] = (float)((double)dz + d.f[2]);
                b.f[0] = (float)((double)dx + c.f[0]);
                b.f[2] = (float)((double)dz + c.f[2]);
                v90 = a;
                vd0 = b;
                Tracks_MoveEnds(net, 0, F<uint32_t>(n, 0x202), a.u, b.u, nullptr, nullptr);
            } else {
                v9c = d;
                va8 = c;
                d.f[0] = (float)((double)dx + a.f[0]);
                d.f[2] = (float)((double)dz + a.f[2]);
                c.f[0] = (float)((double)dx + b.f[0]);
                c.f[2] = (float)((double)dz + b.f[2]);
                v90 = d;
                vd0 = c;
                Tracks_MoveEnds(net, 0, F<uint32_t>(n, 0x202), nullptr, nullptr, c.u, d.u);
            }
            for (int32_t left = extra; left > 0; --left) {
                a = v90;
                b = vd0;
                c.f[0] = (float)((double)vd0.f[0] + dx);
                c.f[2] = (float)((double)vd0.f[2] + dz);
                d.f[0] = (float)((double)dx + v90.f[0]);
                d.f[2] = (float)((double)dz + v90.f[2]);
                const double sc = Fl_207(0x5c8700);
                int32_t id = ((int32_t(__fastcall*)(Collection*, int, float, float, float, float, float, float, float, float, int32_t, int32_t,
                                                     int32_t))0x493ec0)(net, 0, (float)(v90.f[0] / sc), (float)(v90.f[2] / sc), (float)(vd0.f[0] / sc),
                                                                        (float)(vd0.f[2] / sc), (float)(c.f[0] / sc), (float)(c.f[2] / sc),
                                                                        (float)(d.f[0] / sc), (float)(d.f[2] / sc), mode, a4, a5);
                Track_ConnectAndResolve(n, 0, id);
                uint8_t* p = (uint8_t*)Collection_Get(net, 0, (uint32_t)id);
                F<int32_t>(p, 0x206) = F<int32_t>(n, 0x202);
                if (build) {
                    Node_Set212(p, 0, F<int32_t>(n, 0x212));
                    Routes_AddRef(routes, 0, F<uint32_t>(n, 0x212), 1);
                    ++F<int32_t>(Collection_Get(routes, 0, F<uint32_t>(n, 0x212)), 0x14);
                    if (Pair_Contains(Collection_Get(routes, 0, F<uint32_t>(n, 0x212)), 0, F<int32_t>(n, 0x202)))
                        Route_ReplaceEnd((uint8_t*)Collection_Get(routes, 0, F<uint32_t>(n, 0x212)), 0, id, F<int32_t>(n, 0x202));
                }
                vd0.u[0] = c.u[0];
                vd0.u[1] = c.u[1];
                vd0.u[2] = c.u[2];
                v90.u[0] = d.u[0];
                v90.u[1] = d.u[1];
                v90.u[2] = d.u[2];
                n = p;
            }
            Tracks_MoveEnds(net, 0, F<uint32_t>(n, 0x202), nullptr, nullptr, va8.u, v9c.u);
            Track_ConnectAndResolve(n, 0, far);
            Track_ConnectAndResolve((uint8_t*)Collection_Get(net, 0, (uint32_t)far), 0, F<int32_t>(n, 0x202));
            continue;
        }
        if (!((double)len < D(0x5c97d8))) continue;
        int32_t far = side ? F<int32_t>(n, 0x206) : F<int32_t>(n, 0x20a);
        if (far == -1) {
            Track_Unlink(cur, 0, F<int32_t>(n, 0x202));
            ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)net)[2])(net, 0, F<uint32_t>(n, 0x202));
            continue;
        }
        uint8_t* o = (uint8_t*)Collection_Get(net, 0, side ? F<uint32_t>(n, 0x20a) : F<uint32_t>(n, 0x206));
        if (first) F<int32_t>(o, 0x206) = far;
        else F<int32_t>(o, 0x20a) = far;
        Effect_GetVectors(n, 0, a.u, b.u, c.u, d.u);
        if (side) {
            if (first) Tracks_MoveEnds(net, 0, F<uint32_t>(o, 0x202), a.u, b.u, nullptr, nullptr);
            else Tracks_MoveEnds(net, 0, F<uint32_t>(o, 0x202), nullptr, nullptr, b.u, a.u);
        } else {
            if (first) Tracks_MoveEnds(net, 0, F<uint32_t>(o, 0x202), d.u, c.u, nullptr, nullptr);
            else Tracks_MoveEnds(net, 0, F<uint32_t>(o, 0x202), nullptr, nullptr, c.u, d.u);
        }
        if (build) {
            Routes_AddRef(routes, 0, F<uint32_t>(n, 0x202), -1);
            --F<int32_t>(Collection_Get(routes, 0, F<uint32_t>(n, 0x212)), 0x14);
            if (Pair_Contains(Collection_Get(routes, 0, F<uint32_t>(n, 0x212)), 0, F<int32_t>(n, 0x202)))
                Route_ReplaceEnd((uint8_t*)Collection_Get(routes, 0, F<uint32_t>(n, 0x212)), 0, F<int32_t>(o, 0x202), F<int32_t>(n, 0x202));
        }
        ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)net)[2])(net, 0, F<uint32_t>(n, 0x202));
        n = (uint8_t*)Collection_Get(net, 0, (uint32_t)far);
        Track_ConnectAndResolve(n, 0, F<int32_t>(o, 0x202));
        if (build) Routes_Merge(routes, 0, F<uint32_t>(o, 0x212), F<uint32_t>(n, 0x212));
    }
}
RT3_REPLACE(0x4996e0, TrackNet_SmoothJoin);

// 0x49ace0. Track network net: the node nearest (x, y) within radius (see
// Grid_NearestNodeWithin). None found: with `add`, places a plain piece
// there for owner (style 1 for kind 1 or a kind-3 network that is not
// flat, else 4) and sets *missing; found: clears it. Returns the node found,
// the piece placed or -1 (left in EAX; the track tool 0x50dce0 uses it).
// QUIRK: the style only replaces the low byte of skipLinked's stack slot,
// so the upper bytes of skipLinked reach TrackNet_AddPieceTurned.
RT3_NOINLINE int32_t __fastcall TrackNet_FindOrAddNode(Collection* net, int, float x, float y, float radius, int32_t kind, int32_t* missing, int32_t add, int32_t owner,
                                       int32_t byDegree, int32_t exclude, int32_t skipLinked) {
    uint8_t* b = (uint8_t*)net;
    int32_t id = Grid_NearestNodeWithin(net, 0, x, y, exclude, radius, 1, byDegree, skipLinked);
    if (id != -1) {
        if (missing) *missing = 0;
        return id;
    }
    if (add) {
        uint8_t style = kind == 1 || ((double)F<float>(b, 0x1e1) != D(0x5c9bf0) && b[0x1fd] == 3) ? 1 : 4;
        id = TrackNet_AddPieceTurned(net, 0, x, y, 0.0f, 0, owner, (int32_t)(((uint32_t)skipLinked & 0xffffff00u) | style));
    }
    if (missing) *missing = 1;
    return id;
}
RT3_REPLACE(0x49ace0, TrackNet_FindOrAddNode);

// 0x49ad90. Applies and frees the queued track kind changes (+0x1cd list of
// {id, kind, next} nodes).
RT3_NOINLINE void __fastcall Tracks_ApplyPendingKinds(Collection* c) {
    while (uint8_t* p = F<uint8_t*>(c, 0x1cd)) {
        uint8_t* next = F<uint8_t*>(p, 5);
        if (Collection_IsValid(c, 0, F<uint32_t>(F<uint8_t*>(c, 0x1cd), 0))) {
            uint8_t* t = (uint8_t*)Collection_Get(c, 0, F<uint32_t>(F<uint8_t*>(c, 0x1cd), 0));
            Track_SetKind(t, 0, F<uint8_t*>(c, 0x1cd)[4]);
        }
        Free(F<uint8_t*>(c, 0x1cd));
        F<uint8_t*>(c, 0x1cd) = next;
    }
}
RT3_REPLACE(0x49ad90, Tracks_ApplyPendingKinds);

// 0x49ae00. Charges `company` for upgrading the collection's track pieces to their
// planned kind (+0x218) and signal state (+0x44): `amount` goes to the collection's
// total (+0x9c) and, outside multiplayer, to the company's stats (0xf, minus 0xd);
// each changed piece adds its build cost to +0x230 and takes the new state. Not in
// the editor (new game +0x82) or while the world is busy (+0x4aef).
RT3_NOINLINE void __fastcall Tracks_ApplyUpgrades(Collection* c, int, uint32_t company, int32_t amount) {
    if (g_newGame[0x82] || F<int32_t>(g_world, 0x4aef)) return;
    if (!company) return;
    uint8_t* rec = (uint8_t*)Collection_Get(g_companies, 0, company);
    float f = (float)amount;
    F<float>(c, 0x9c) = (float)((double)f + F<float>(c, 0x9c));
    if (!IsMultiplayer()) {
        Company_AddStat(rec, 0, 0xf, f);
        Company_AddStat(rec, 0, 0xd, (float)-amount);
    }
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (t[0x217] == t[0x218] && (int32_t)((t[0x24c] >> 4) & 1) == (t[0x44] != 0)) continue;
        F<int32_t>(t, 0x230) += Track_BuildCost(t, 0, company, 0);
        t[0x217] = t[0x218];
        t[0x24c] = (uint8_t)((t[0x24c] & ~0x10) | ((t[0x44] != 0) << 4));
    }
}
RT3_REPLACE(0x49ae00, Tracks_ApplyUpgrades);

// 0x49af50. Total build cost for `company` of the collection's track pieces from the
// current era (+0x240 >= +0x88 - 1), skipping kind 2 (+0x216).
RT3_NOINLINE float __fastcall Tracks_TotalCost(Collection* c, int, uint32_t company) {
    float sum = 0.0f;
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        if (F<uint32_t>(t, 0x240) < F<uint32_t>(c, 0x88) - 1 || t[0x216] == 2) continue;
        float cost = (float)Track_BuildCost(t, 0, company, 0);
        sum = (float)((double)cost + sum);
    }
    return sum;
}
RT3_REPLACE(0x49af50, Tracks_TotalCost);

// 0x48f7f0. Re-shapes track piece t to four corners given in cells (x, z pairs,
// times 92). Without `drop` the heights are the current ones; with it the original
// passes uninitialized stack as heights and then drops the piece onto the terrain.
// `neighbours` then matches the neighbouring pieces. NOT INSTALLED: Track_Reshape
// sets the light mode (+0x5a) from those stale heights before the drop, so with
// `drop` the original's result depends on stack residue. The exe's own code stays
// in place; this is the decode (zero heights instead), verified without `drop`.
void __fastcall Track_ReshapeCells(uint8_t* t, int, float x0, float z0, float x1, float z1, float x2, float z2, float x3,
                                   float z3, int32_t drop, int32_t neighbours) {
    float v[4][3] = {};
    if (!drop) Effect_GetVectors(t, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3]);
    const float k = Fl_205(0x5c8700);
    const float xs[4] = { x0, x1, x2, x3 }, zs[4] = { z0, z1, z2, z3 };
    for (int i = 0; i < 4; ++i) {
        v[i][0] = (float)((double)xs[i] * k);
        v[i][2] = (float)((double)zs[i] * k);
    }
    Track_Reshape_Impl(t, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3], 1, 0, 0);
    if (drop) Track_DropAndSettle(t, 0, 0x40400000, 1);
    if (neighbours) Track_MatchNeighbourHeights(t);
}

// 0x49afe0. Restores a track edit (this = the piece collection with its undo
// record): while the remembered piece (+0x129) still exists and `t` has no free
// link, re-sets that piece to the saved corners +0x199..; otherwise re-sets `t`
// to the saved corners +0x139.. (new piece).
void __fastcall Tracks_RestoreEdit(Collection* c, int, uint8_t* t) {
    uint8_t* u = (uint8_t*)c;
    if (Collection_IsValid(c, 0, F<uint32_t>(u, 0x129)) && !Node_HasFreeLink(t, 0, 0)) {
        uint8_t* p = (uint8_t*)Collection_Get(c, 0, F<uint32_t>(u, 0x129));
        Track_Reshape_Impl(p, 0, (uint32_t*)(u + 0x199), (uint32_t*)(u + 0x1a5), (uint32_t*)(u + 0x1b1), (uint32_t*)(u + 0x1bd), 1, 0, 1);
        return;
    }
    Track_Reshape_Impl(t, 0, (uint32_t*)(u + 0x139), (uint32_t*)(u + 0x145), (uint32_t*)(u + 0x151), (uint32_t*)(u + 0x15d), 0, 0, 1);
}
RT3_REPLACE(0x49afe0, Tracks_RestoreEdit);

// 0x49b070. Replays the collection's track edit (its undo record, see
// Tracks_RestoreEdit) when one is pending (+0x12d, with a non-negative +0x169):
// while the edited piece (+0x12d) and the remembered one (+0x129) both exist and
// the edited piece has no free link, the remembered piece takes the corners
// +0x199..; otherwise the piece +0x131 (or else +0x12d) takes the corners +0x169...
RT3_NOINLINE void __fastcall Tracks_ReplayEdit(Collection* c) {
    uint8_t* u = (uint8_t*)c;
    if (F<int32_t>(u, 0x12d) == -1 || !(F<float>(u, 0x169) >= 0.0f)) return;
    if (Collection_IsValid(c, 0, F<uint32_t>(u, 0x12d)) && Collection_IsValid(c, 0, F<uint32_t>(u, 0x129)) &&
        !Node_HasFreeLink(Collection_Get(c, 0, F<uint32_t>(u, 0x12d)), 0, 0)) {
        Track_Reshape_Impl((uint8_t*)Collection_Get(c, 0, F<uint32_t>(u, 0x129)), 0, (uint32_t*)(u + 0x199), (uint32_t*)(u + 0x1a5),
                           (uint32_t*)(u + 0x1b1), (uint32_t*)(u + 0x1bd), 1, 0, 1);
        return;
    }
    uint32_t id;
    if (Collection_IsValid(c, 0, F<uint32_t>(u, 0x131))) id = F<uint32_t>(u, 0x131);
    else if (Collection_IsValid(c, 0, F<uint32_t>(u, 0x12d))) id = F<uint32_t>(u, 0x12d);
    else return;
    Track_Reshape_Impl((uint8_t*)Collection_Get(c, 0, id), 0, (uint32_t*)(u + 0x169), (uint32_t*)(u + 0x175), (uint32_t*)(u + 0x181),
                       (uint32_t*)(u + 0x18d), 1, 0, 1);
}
RT3_REPLACE(0x49b070, Tracks_ReplayEdit);

// 0x49b180. Track network t: the status line (0xccbb20) and tooltip while
// laying track (+0x118 building) or pointing at piece `piece`. Without a
// build it first reports a cell that cannot take track - no ground under the
// cursor, special terrain (water before v1.895), a region the player's
// company may not build in, a structure - and stops there. Otherwise it
// lists a crossing / junction (+0x216 of the piece, or of the start piece
// +0x125 or the pieces +0x129..+0x131 in a build; a build also counts a
// junction where the run +0x12d rejoins the start's neighbours within 200
// pieces), the build cost (+0x1d1, red when the company cannot pay it) with
// the bridge / tunnel note (+0xd8..+0xdb) and, for a player's company
// with track but no build on an empty line, the hint 0xf28 / 0xf29.
RT3_NOINLINE void __fastcall TrackNet_ShowStatus(uint8_t* t, int, uint32_t piece) {
    char m1[0x1f4], m2[0x1f4];
    m1[0] = 0, m2[0] = 0;
    float fx, fy;
    int32_t focus = Scene_FocusFlagIs1(&fx, &fy);
    auto simple = [&](int32_t id, const char* a1) {
        FormatText(m2, GetString(id), a1, 0, 0, 0, 0, 0, 0, 0);
        Obj_SetText1c(g_cursorManager, 0, m1, 0xffff5f5f);
        Ui_ShowTooltip(g_windowManager, 0, m2, 1, 0, -1);
    };
    if (!focus) {
        if (!F<int32_t>(t, 0x118)) {
            FormatText(m1, GetString(0x2e6), 0, 0, 0, 0, 0, 0, 0, 0);
            return simple(0x2e7, nullptr);
        }
    } else {
        uint8_t* rec = (uint8_t*)Map_RegionRecordAt(hook::As<void*>(g_map), 0, fx, fy);
        int32_t yi = Ftol(fy);
        if (Terrain_IsSpecial(g_map, 0, Ftol(fx), yi)) {
            FormatText(m1, GetString(0x2de), 0, 0, 0, 0, 0, 0, 0, 0);
            if (F<uint16_t>(g_world, 0xd) < 0x767) {
                int32_t yj = Ftol(fy);
                if (Terrain_KindIs1Or3to5(g_map, 0, Ftol(fx), yj)) {
                    strcat(m1, GetString(0x2df));
                    char num[0x40];
                    return simple(0x2e0, Num_Format(num, 8, 1895.0, 10, 1, 0, 0));
                }
            }
            return simple(0x2e1, nullptr);
        }
        if (!F<int32_t>(t, 0x118)) {
            if (Collection_IsValid(g_companies, 0, (uint32_t)Obj_Get21(hook::As<void*>(g_world))) &&
                !Company_EntryFlag(Obj_Company21(hook::As<void*>(g_world)), 0, F<int32_t>(rec, 0))) {
                FormatText(m1, GetString(0x2e2), (const char*)rec + 4, 0, 0, 0, 0, 0, 0, 0);
                return simple(0x2e3, (const char*)rec + 4);
            }
            if (!F<int32_t>(t, 0x118)) {
                int32_t cy = Ftol(fy * Fl_207(0x5c8568));
                if (Map_CellFlag(g_map, 0, Ftol(fx * Fl_207(0x5c8568)), cy, 2)) {
                    FormatText(m1, GetString(0x2e4), 0, 0, 0, 0, 0, 0, 0, 0);
                    return simple(0x2e5, nullptr);
                }
            }
        }
    }
    Collection* net = (Collection*)t;
    int32_t count = 0, changed = 0;
    uint8_t* start = nullptr;
    uint32_t from = piece;
    if (F<int32_t>(t, 0x118)) {
        if (Collection_IsValid(net, 0, F<uint32_t>(t, 0x125))) {
            start = (uint8_t*)Collection_Get(net, 0, F<uint32_t>(t, 0x125));
            if (start[0x216]) count = 1;
        }
    } else if (Collection_IsValid(net, 0, piece) && ((uint8_t*)Collection_Get(net, 0, piece))[0x216])
        count = 1;
    char crossing[0x12c];
    bool linked = false;
    if (F<int32_t>(t, 0x118)) {
        if (!count) {
            for (uint32_t off = 0x129; off <= 0x131; off += 4) {
                int32_t id = F<int32_t>(t, off);
                if (id != -1 && Collection_IsValid(net, 0, (uint32_t)id) && F<int32_t>(Collection_Get(net, 0, (uint32_t)id), 0x20e) != -1) {
                    linked = true;
                    break;
                }
            }
            if (linked) count = 1, changed = 1;
        }
        int32_t run = F<int32_t>(t, 0x12d);
        if (!linked && run != -1 && Collection_IsValid(net, 0, (uint32_t)run) && ((uint8_t*)Collection_Get(net, 0, (uint32_t)run))[0x216] && start) {
            uint8_t* s = start;
            if (F<int32_t>(t, 0x129) != -1 && Collection_IsValid(net, 0, F<uint32_t>(t, 0x129))) {
                from = F<uint32_t>(t, 0x129);
                s = (uint8_t*)Collection_Get(net, 0, from);
            }
            int32_t next = -1;
            for (uint32_t off = 0x206; off <= 0x20e; off += 4) {
                int32_t id = F<int32_t>(s, off);
                if (id != -1 && Collection_IsValid(net, 0, (uint32_t)id) &&
                    !((uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)id))[0x216]) {
                    next = id;
                    break;
                }
            }
            if (next != -1) {
                uint8_t* o = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)next);
                if (o && Track_LinksWithin(o, 0, run, 200, (int32_t)from)) ++count, changed = 1;
            }
        }
    }
    if (count > 0) strcpy(crossing, GetString(0x2e8));
    else crossing[0] = 0;
    if (count > 1) memcpy(crossing + strlen(crossing), (const void*)0x5cfe58, 5);
    int32_t cost = Ftol(F<float>(t, 0x1d1));
    char note[0x4b0], costText[0x64], line[0x12c], tip[0x7c4], extra[0x190];
    note[0] = 0, costText[0] = 0;
    bool haveTip = false;
    if (F<int32_t>(t, 0x118) && cost > 0) {
        Num_FormatSimple2(costText, 0xb, (double)cost, 0xb);
        if (!F<uint8_t>(g_newGame, 0x82) && !F<int32_t>(g_world, 0x4aef))
            FormatText(line, GetString(0x2e9), costText, 0, 0, 0, 0, 0, 0, 0);
        else {
            FormatText(line, (const char*)0x5c87a8, 0, 0, 0, 0, 0, 0, 0, 0);
            FormatText(costText, (const char*)0x5c87a8, 0, 0, 0, 0, 0, 0, 0, 0);
        }
        extra[0] = 0;
        if (t[0xd8]) {
            if (changed) t[0xda] = 0;
            if (t[0xda] || t[0xdb]) {
                strcat(note, GetString(0x2ea));
                strcpy(extra, GetString(t[0xda] ? 0x2eb : 0x2ec));
            }
        }
        strcpy(tip, line);
        strcat(tip, strlen(extra) > 1 ? extra : crossing);
        strcat(line, crossing);
        haveTip = true;
    }
    uint32_t colour = 0xffffffff;
    if (F<int32_t>(t, 0x118)) {
        FormatText(line, costText, 0, 0, 0, 0, 0, 0, 0, 0);
        if (Collection_IsValid(g_companies, 0, (uint32_t)Obj_Get21(hook::As<void*>(g_world)))) {
            uint8_t* co = (uint8_t*)Obj_Company21(hook::As<void*>(g_world));
            if (Company_Stat(co, 0, 0xd, 0x2329) <= (double)cost) colour = 0xffff5f5f;
        }
    } else
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(line, costText);
    strcat(line, crossing);
    strcat(line, note);
    const char* text = haveTip ? tip : (const char*)0x5c87a8;
    if (focus && !line[0] && F<int32_t>(g_world, 0x4aab) && !F<int32_t>(t, 0x118) &&
        Collection_IsValid(g_companies, 0, (uint32_t)Obj_Get21(hook::As<void*>(g_world))) &&
        Obj_Company21(hook::As<void*>(g_world))) {
        uint8_t* co = (uint8_t*)Obj_Company21(hook::As<void*>(g_world));
        if (F<int32_t>(co, 0x766c) + F<int32_t>(co, 0x7668) + F<int32_t>(co, 0x7664) > 0 || F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x98)) {
            strcat(line, GetString(0xf28));
            strcpy(tip, GetString(0xf29));
            colour = 0xffff5f5f;
            text = tip;
        }
    }
    Ui_ShowTooltip(g_windowManager, 0, text, 1, 0, -1);
    Obj_SetText1c(g_cursorManager, 0, line, colour);
}
RT3_REPLACE(0x49b180, TrackNet_ShowStatus);

// 0x49bbb0. Track network net: removes piece id (reached from piece
// `from`, -1 for the first, which first applies the pending kinds) with its
// unplaced (+0x216) neighbours that hang on it alone (Node_HasLink). A first
// piece is only removed with `force` when unplaced and routeless. Settling
// the ground (kinds 0xc / 0xd: dropped by 3 and reset), unlinking it from
// `from` and the table removal (method 2) follow; +0x1e5 is cleared.
RT3_NOINLINE void __fastcall TrackNet_RemovePiece(Collection* net, int, uint32_t id, int32_t force, int32_t from) {
    if (!Collection_IsValid(net, 0, id)) return;
    uint8_t* p = (uint8_t*)Collection_Get(net, 0, id);
    uint8_t* parent = nullptr;
    if (from == -1) Tracks_ApplyPendingKinds(net);
    else parent = (uint8_t*)Collection_Get(net, 0, (uint32_t)from);
    if (Node_HasLink(p, 0, 1)) {
        static const uint32_t sides[] = { 0x206, 0x20a, 0x20e };
        for (uint32_t side : sides) {
            int32_t nb = F<int32_t>(p, side);
            if (nb == -1) continue;
            uint8_t* q = (uint8_t*)Collection_Get(net, 0, (uint32_t)nb);
            if (q && q != parent && !q[0x216]) TrackNet_RemovePiece(net, 0, (uint32_t)nb, 0, (int32_t)id);
        }
    }
    if (from == -1 && (!force || p[0x216] || F<int32_t>(p, 0x212) != -1)) return;
    if (Tracks_SettleGroup(net, 0, id)) {
        int32_t kind = F<int32_t>(Collection_Get(g_tracks, 0, id), 0x226);
        if (kind == 0xc || F<int32_t>(Collection_Get(g_tracks, 0, id), 0x226) == 0xd) {
            Track_DropAndSettle(p, 0, 0x40400000, 1);
            Node_ResetKind(p);
        }
    }
    if (from != -1) Track_Unlink(parent, 0, (int32_t)id);
    ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)net)[2])(net, 0, id);
    F<int32_t>(net, 0x1e5) = 0;
}
RT3_REPLACE(0x49bbb0, TrackNet_RemovePiece);

// 0x49bd40. Track network net: walks the track from piece `start` along the
// straight line (x0, y0) - (x1, y1) in 0.181-tile steps on the major axis,
// following the route towards the piece nearest the end (0x494cb0) when the
// track is not much longer than the line (route cost x 0.362 below the
// distance x 1.4, 1.8 for double / electric). Every piece reached is tinted
// and queued in the network's touched list (+0x1cd, 9-byte nodes: id,
// placed flag); upgrading (kind 4) single pieces of the company to double
// track (cost Track_Cost into *cost) and, in an electrified world, powering
// unpowered ones (Track_Cost2, a pole every sixth) - both only with `mode`
// 1 / 2, which also re-resolves, splits and merges the routes at the ends
// and sets *changed. *ok is cleared when the walk reached the end piece.
// Returns the last piece the walk stood on. QUIRK: `mode` doubles as the
// first candidate id; the end-split side is stack residue unless a second
// piece was powered (0 here). QUIRK: a followed walk whose route gives out
// with no piece nearby stops before its first piece; with a powered end piece
// the original then reads the null first piece's route (+0x212) and crashes.
RT3_NOINLINE int32_t __fastcall TrackNet_WalkLine(Collection* net, int, int32_t start, float x0, float y0, float x1, float y1, int32_t mode, int32_t company,
                                     int32_t kind, int32_t* cost, int32_t* ok, int32_t* changed) {
    uint8_t* nb = (uint8_t*)net;
    if (F<void*>(nb, 0x1cd)) Tracks_ApplyPendingKinds(net);
    *ok = 1;
    *changed = 0;
    uint8_t* p0 = (uint8_t*)Collection_Get(net, 0, (uint32_t)start);
    if (!p0 || Node_HasFreeLink(p0, 0, 0)) return start;
    float dx = (float)((double)x1 - x0), dy = (float)((double)y1 - y0);
    if (dx == 0.0f && dy == 0.0f) return start;
    uint8_t mb = (uint8_t)mode;
    bool build = mb == 1 || mb == 2;
    int32_t end = Grid_NearestNodeWithin(net, 0, x1, y1, -1, BitsF(0x3eb95810), 1, 0, 0);
    int32_t cur = start, prev = -1, cand = mode, count = 0, startOn = 0, flip = 0;
    float sumX = 0, sumY = 0, curX = x0, curY = y0;
    uint8_t* tail = F<uint8_t*>(nb, 0x1cd);
    uint8_t *first = nullptr, *last = nullptr, *piece = nullptr;
    bool follow = false;
    if (!F<int32_t>(nb, 0xe9)) {
        float factor = (float)(kind == 4 || F<int32_t>(g_world, 0x4c74) ? 1.8 : 1.4);
        if (start != -1 && end != -1) {
            float dist = (float)Distance2D(x0, y0, x1, y1);
            uint32_t routeCost;
            int32_t step = Route_NextStep(g_routes, 0, start, end, &routeCost, 0, nullptr, nullptr, 1);
            if (step != -1) {
                double c = (double)(int32_t)routeCost;
                if ((int32_t)routeCost < 0) c += Fl_207(0x5c872c);
                float scaled = (float)((double)(float)c * D(0x5c97e0));
                if ((double)dist * factor > scaled) {
                    follow = true;
                    piece = (uint8_t*)Collection_Get(net, 0, (uint32_t)start);
                    prev = F<int32_t>(piece, 0x206) == step ? F<int32_t>(piece, 0x20a) : F<int32_t>(piece, 0x206);
                    cand = start;
                }
            }
        }
    }
    auto fabs_ = (double(__cdecl*)(double))0x5a152e;
    float stepX, stepY;
    if (fabs_((double)dy) < fabs_((double)dx)) {
        if (dx == 0.0f) dx = BitsF(0x38d1b717);
        stepX = (float)(dx < 0.0f ? D(0x5cfde0) : D(0x5c97d8));
        double r = fabs_((double)dy / dx);
        stepY = (float)(r * (dy < 0.0f ? D(0x5cfde0) : D(0x5c97d8)));
    } else {
        if (dy == 0.0f) dy = BitsF(0x38d1b717);
        double r = fabs_((double)dx / dy);
        stepX = (float)(r * (dx < 0.0f ? D(0x5cfde0) : D(0x5c97d8)));
        stepY = (float)(dy < 0.0f ? D(0x5cfde0) : D(0x5c97d8));
    }
    bool go = true;
    auto nearest = [&](int32_t exclude) {
        float rad = (float)((double)Route_GradeFactorHere(curX, curY) * D(0x5cfe60));
        int32_t r = Grid_NearestNodeWithin(net, 0, curX, curY, exclude, rad, 1, 0, 0);
        return r == -1 ? -1 : r;
    };
    if (!(fabs_((double)dx) < fabs_(0.0))) {
        while (!(fabs_((double)dy) < fabs_((double)sumY)) && go) {
            bool reached = false;
            for (;;) {
                if (F<int32_t>(nb, 0xe9) && cur != -1 && Node_HasFreeLink(Collection_Get(net, 0, (uint32_t)cur), 0, 1)) go = false;
                else {
                    curX = (float)((double)stepX + curX);
                    curY = (float)((double)stepY + curY);
                    sumX = (float)((double)stepX + sumX);
                    sumY = (float)((double)stepY + sumY);
                }
                bool attach = true;
                if (!follow) cand = nearest(-1);
                else {
                    if (F<int32_t>(piece, 0x20e) == -1) {
                        int32_t old = cand;
                        cand = F<int32_t>(piece, 0x206) == prev ? F<int32_t>(piece, 0x20a) : F<int32_t>(piece, 0x206);
                        prev = old;
                    } else {
                        int32_t from = cand;
                        uint32_t c2;
                        cand = Route_NextStep(g_routes, 0, cand, end, &c2, 0, nullptr, nullptr, 1);
                        if (cand == -1) {
                            follow = false;
                            cand = nearest(-1);
                        }
                        prev = from;
                    }
                    last = (uint8_t*)Collection_Get(net, 0, (uint32_t)prev);
                    if (follow) attach = false;
                }
                if (attach && piece && cand != prev && end != -1 && !Node_HasFreeLink(piece, 0, 1)) {
                    if (cand == -1) goto done;
                    int32_t alt = nearest(cand);
                    if (alt != -1) {
                        int32_t other = F<int32_t>(piece, 0x206) == prev ? F<int32_t>(piece, 0x20a) : F<int32_t>(piece, 0x206);
                        uint8_t* q = Node_NextTowards(piece, 0, end, -1, other, nullptr, 1);
                        if (q && F<int32_t>(q, 0x202) == alt) cand = alt;
                    }
                }
                if (cand == -1) goto done;
                if (cand == prev) break;
                piece = (uint8_t*)Collection_Get(net, 0, (uint32_t)cand);
                if (prev != -1 && prev != F<int32_t>(piece, 0x206) && prev != F<int32_t>(piece, 0x20a) && prev != F<int32_t>(piece, 0x20e)) {
                    int32_t r = nearest(cand);
                    cand = r;
                    if (r == prev) continue;
                    if (r == -1) goto done;
                    piece = (uint8_t*)Collection_Get(net, 0, (uint32_t)r);
                    if (prev != F<int32_t>(piece, 0x206) && prev != F<int32_t>(piece, 0x20a) && prev != F<int32_t>(piece, 0x20e)) goto done;
                }
                reached = true;
                break;
            }
            if (reached) {
                if (!count) startOn = piece[0x44] != 0;
                Obj_SetArgb(piece, 0, 0xff, 0xff, 0xff, 0);
                if (kind == 4 && (piece[0x218] == 1 || piece[0x218] == 2) &&
                    (!Collection_IsValid(g_companies, 0, (uint32_t)company) ||
                     Company_Get7680(Collection_Get(g_companies, 0, (uint32_t)company)) > 0)) {
                    *cost += Track_Cost(piece, 0, (uint32_t)company);
                    if (build) {
                        Track_SetState(piece, 0, 4, piece[0x44] != 0, 1, 1);
                        Track_SetKind(piece, 0, 1);
                        *changed = 1;
                        if (F<uint16_t>(piece, 0x22e)) Company_Add7680(Node_Company(piece), 0, -1);
                        if (last) {
                            if (last != first) Track_Resolve(last, 0, 1);
                            if ((uint8_t*)Track_Node20e(last) == piece) {
                                if (Obj_Node206(last)) Track_Resolve((uint8_t*)Obj_Node206(last), 0, 0);
                                if (Obj_Node20a(last)) Track_Resolve((uint8_t*)Obj_Node20a(last), 0, 0);
                            }
                            if ((uint8_t*)Track_Node20e(piece) == last) {
                                if (Obj_Node206(piece)) Track_Resolve((uint8_t*)Obj_Node206(piece), 0, 0);
                                if (Obj_Node20a(piece)) Track_Resolve((uint8_t*)Obj_Node20a(piece), 0, 0);
                            }
                        }
                    }
                }
                int32_t n = count;
                bool firstNow;
                if (F<int32_t>(g_world, 0x4c74) && !piece[0x44]) {
                    if (!n) {
                        Track_SetKind(p0, 0, 1);
                        Obj_SetArgb(p0, 0, 0xff, 0xff, 0xff, 0);
                        *cost += Track_Cost2(p0, 0, (uint32_t)company);
                        if (build) Track_Electrify(p0, 0, 1, 1, (uint8_t*)Obj_Node20a(p0) == piece ? 0 : 1);
                        count = n = 1;
                    }
                    Track_SetKind(piece, 0, 1);
                    Obj_SetArgb(piece, 0, 0xff, 0xff, 0xff, 0);
                    *cost += Track_Cost2(piece, 0, (uint32_t)company);
                    if (build) {
                        Track_Electrify(piece, 0, n % 6 == 0, 1, 1);
                        *changed = 1;
                    }
                    if (build && n == 2) {
                        flip = F<int32_t>(first, 0x206) == cand;
                        firstNow = false;
                    } else
                        firstNow = n == 1;
                } else
                    firstNow = n == 0;
                if (firstNow) first = piece;
                count = n + 1;
                last = piece;
                uint8_t* node = (uint8_t*)Malloc(9);
                if (!tail) F<uint8_t*>(nb, 0x1cd) = node;
                else F<uint8_t*>(tail, 5) = node;
                tail = node;
                F<int32_t>(node, 0) = F<int32_t>(piece, 0x202);
                node[4] = piece[0x216];
                F<uint32_t>(node, 5) = 0;
                if (cand == end) {
                    cur = cand;
                    goto done;
                }
                if (Node_HasFreeLink(Collection_Get(net, 0, (uint32_t)cand), 0, 1) && !F<int32_t>(Collection_Get(net, 0, (uint32_t)cand), 0x226)) cur = cand;
                if (!follow) prev = cand;
            }
            if (fabs_((double)dx) < fabs_((double)sumX)) break;
        }
    }
done:
    if (end != -1 && cur == end) *ok = 0;
    if (first && first[0x218] == 4 && build) Track_Resolve(first, 0, 0);
    if (last && last[0x218] == 4 && build) Track_Resolve(last, 0, 0);
    if (!startOn && first && first[0x44]) Routes_SplitAtNode(g_routes, 0, first, flip);
    if (last && last[0x44] && build) {
        Track_AddCatenary(last, 0, F<int32_t>(last, 0x20a) == prev ? 0 : 1);
        auto on = [&](uint32_t side) {
            return F<int32_t>(last, side) != -1 && ((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(last, side)))[0x44];
        };
        bool openA = F<int32_t>(last, 0x206) != -1 && !on(0x206), openB = F<int32_t>(last, 0x20a) != -1 && !on(0x20a);
        if (openA || openB) Routes_SplitAtNode(g_routes, 0, last, on(0x206) ? 1 : 0);
        Routes_TryMergeAt(g_routes, 0, F<uint32_t>(last, 0x212));
        Routes_TryMergeAt(g_routes, 0, F<uint32_t>(first, 0x212));
    }
    return cur;
}
RT3_REPLACE(0x49bd40, TrackNet_WalkLine);

// 0x49c900. Walks from (x0, y0) towards (x1, y1) in steps of 0.181 cells along the
// major axis and looks, around each step, for the nearest placed track node (not
// `exclude`) whose heading runs along the walk (within pi/8, either way); stores it
// in *found (-1 if none) and returns whether one was found.
RT3_NOINLINE int32_t __fastcall Nodes_FindAlongLine(Collection* nodes, int, float x0, float y0, float x1, float y1, int32_t, int32_t exclude,
                                       int32_t* found) {
    float dx = (float)((double)x1 - x0), dy = (float)((double)y1 - y0);
    float tx = 0.0f, ty = 0.0f, cx = x0, cy = y0;
    float angle = (float)AngleOf(x0, y0, x1, y1);
    *found = -1;
    float stepX, stepY;
    uint32_t stepBits[2] = { 0x0c49ba5f, 0 };
    if (CrtFabs(dy) < CrtFabs(dx)) {
        if (dx == 0.0f) dx = 0.0001f;
        stepBits[1] = dx < 0.0f ? 0xbfc72b02 : 0x3fc72b02;
        stepX = (float)*(double*)stepBits;
        double r = CrtFabs((double)dy / dx);
        stepY = (float)(dy < 0.0f ? r * D(0x5cfde0) : r * D(0x5c97d8));
    } else {
        if (dy == 0.0f) dy = 0.0001f;
        double r = CrtFabs((double)dx / dy);
        stepX = (float)(dx < 0.0f ? r * D(0x5cfde0) : r * D(0x5c97d8));
        stepBits[1] = dy < 0.0f ? 0xbfc72b02 : 0x3fc72b02;
        stepY = (float)*(double*)stepBits;
    }
    int32_t searching = 1;
    if (!(CrtFabs(dx) > CrtFabs(0.0))) return *found != -1;
    for (;;) {
        if (!(CrtFabs(dy) > CrtFabs(ty)) || !searching) break;
        int32_t r = Grid_NearestNodeWithin(nodes, 0, cx, cy, -1, 0.181f, 1, 0, 0);
        if (r != -1 && r != exclude) {
            uint8_t* n = (uint8_t*)Collection_Get(nodes, 0, (uint32_t)r);
            float heading = Vehicle_Heading(n, 0, 1);
            float d = (float)CrtFabs(AngleDelta(nullptr, 0, heading, angle));
            if ((d < D(0x5cfe70) || d > D(0x5cfe68)) && n[0x216]) {
                *found = r;
                searching = 0;
            }
        }
        cx = (float)((double)stepX + cx);
        cy = (float)((double)stepY + cy);
        tx = (float)((double)stepX + tx);
        ty = (float)((double)stepY + ty);
        if (!(CrtFabs(dx) > CrtFabs(tx))) break;
    }
    return *found != -1;
}
RT3_REPLACE(0x49c900, Nodes_FindAlongLine);

// 0x49cc40. Plans a route from (x0, y0) to (x1, y1) as a list of waypoints in *out.
// A clear straight line gives a single waypoint. Otherwise the object's cached
// route (+0x1c9) is reused from its last node that sees the target, unless a
// replan is forced (+0x135, set by pending waypoints or +0x1cd). Failing that, the
// legs are walked in 0.362 steps; where a leg crosses blocked cells, the midpoint
// of the blocked stretch is pushed sideways, alternating sides in half-cell steps
// (up to 98 tries), until a free point is found. That point becomes the leg's end,
// and the leg is walked again (up to 20 detours per leg, 100 legs). Pending
// waypoints (*pending) are consumed as the next leg ends. Finally, if nothing was
// pending, nodes that a later node can see straight are cut out (only the first
// skipped node is freed). Returns whether the last leg ended on free ground.
RT3_NOINLINE int32_t __fastcall Track_PlanRoute(uint8_t* self, int, float x0, float y0, float x1, float y1,
                                   PathNode** out, PathNode** pending) {
    if (x0 == x1 && y0 == y1) x1 = (float)((double)x1 + D(0x5c85b8));
    if (*pending == nullptr && Map_LineOnLand(x0, y0, x1, y1)) {
        Obj_FreeList1c9(self);
        PathNode* n = (PathNode*)ExeNew(0xc);
        *out = n;
        n->x = x1;
        n->y = y1;
        n->next = nullptr;
        return 1;
    }
    float steps = LegSteps(x0, y0, x1, y1);
    float px = x0, py = y0, lastX = x0, lastY = y0;
    float dx = (float)(((double)x1 - x0) / steps);
    float dy = (float)(((double)y1 - y0) / steps);
    PathNode* p = *pending;
    float curX = p ? p->x : x1;
    float curY = p ? p->y : y1;
    float startX = x0, startY = y0;
    int32_t done = 0;
    if (p) F<int32_t>(self, 0x135) = 1;
    if (F<int32_t>(self, 0x1cd)) F<int32_t>(self, 0x135) = 1;
    PathNode* c = F<PathNode*>(self, 0x1c9);
    if (c && !F<int32_t>(self, 0x135)) {
        for (; c->next; c = c->next) {
            if (!Map_LineOnLand(c->x, c->y, x1, y1)) continue;
            *out = F<PathNode*>(self, 0x1c9);
            PathNode* k = c->next;
            PathNode* rest = k->next;
            k->x = x1;
            k->y = y1;
            k->next = nullptr;
            while (rest) {
                PathNode* nx = rest->next;
                ExeDelete(rest);
                rest = nx;
            }
            return 1;
        }
    }
    F<int32_t>(self, 0x135) = 0;
    Obj_FreeList1c9(self);
    PathNode* node = (PathNode*)ExeNew(0xc);
    *out = node;
    node->x = curX;
    node->y = curY;
    node->next = nullptr;
    int32_t blocked = 0;
    for (int32_t leg = 0;;) {
        blocked = 0;
        int32_t retries = 0;
        if (Fl_205(0x5c8598) < steps) {
            for (int32_t i = 0;;) {
                if (Blocked(px, py)) {
                    blocked = 1;
                } else if (!blocked) {
                    lastX = px;
                    lastY = py;
                } else {
                    float midX = (float)(((double)lastX + px) / D(0x5c85d0));
                    float midY = (float)(((double)lastY + py) / D(0x5c85d0));
                    float rise = (float)((double)midY - startY);
                    if (rise == Fl_205(0x5c8598)) rise = BitsF(0x38d1b717);
                    float slope = (float)-(((double)midX - startX) / rise);
                    float ox, oy;
                    if (fabs((double)slope) < D(0x5c8770)) {
                        oy = (float)((double)slope * D(0x5c8628));
                        ox = 0.5f;
                    } else {
                        oy = 0.5f;
                        ox = (float)(D(0x5c8770) / slope * D(0x5c8628));
                    }
                    int32_t found = 0;
                    for (int32_t j = 2;;) {
                        bool up = slope > D(0x5c8608);
                        bool plus;
                        float t;
                        if ((j & 1) == 0) {
                            t = (float)(j / 2);
                            plus = up;
                        } else {
                            t = (float)((j - 1) / 2);
                            plus = !up;
                        }
                        if (plus) {
                            curX = (float)((double)t * ox + midX);
                            curY = (float)((double)t * oy + midY);
                        } else {
                            curX = (float)(midX - (double)t * ox);
                            curY = (float)(midY - (double)t * oy);
                        }
                        if (!Blocked(curX, curY)) {
                            found = 1;
                            node->x = curX;
                            node->y = curY;
                        }
                        if (++j > 0x63) goto legEnd;
                        if (found) break;
                    }
                    steps = LegSteps(startX, startY, curX, curY);
                    px = startX;
                    py = startY;
                    blocked = 0;
                    i = -1;
                    dx = (float)(((double)curX - startX) / steps);
                    dy = (float)(((double)curY - startY) / steps);
                    if (++retries > 0x14) goto legEnd;
                    goto next;
                }
                px = (float)((double)px + dx);
                py = (float)((double)py + dy);
            next:
                ++i;
                if (!((float)i < steps)) break;
            }
        }
    legEnd:
        if (blocked) {
            node->x = lastX;
            node->y = lastY;
            done = 1;
        } else if (curX == x1 && curY == y1) {
            done = 1;
        } else {
            if (*pending) {
                PathNode* nx = (*pending)->next;
                Free(*pending);
                *pending = nx;
            }
            PathNode* n = (PathNode*)ExeNew(0xc);
            n->x = *pending ? (*pending)->x : x1;
            n->y = *pending ? (*pending)->y : y1;
            n->next = nullptr;
            node->next = n;
            node = n;
            startX = curX;
            startY = curY;
            curX = n->x;
            curY = n->y;
            steps = LegSteps(startX, startY, curX, curY);
            px = startX;
            py = startY;
            dx = (float)(((double)curX - startX) / steps);
            dy = (float)(((double)curY - startY) / steps);
        }
        if (++leg > 0x64 || done) break;
    }
    if (*pending == nullptr) {
    restart:
        for (PathNode* a = *out; a->next; a = a->next) {
            for (PathNode* b = a->next->next; b; b = b->next) {
                if (!Map_LineOnLand(a->x, a->y, b->x, b->y)) continue;
                PathNode* skipped = a->next;
                a->next = b;
                ExeDelete(skipped);
                goto restart;
            }
        }
    } else {
        PathNode* q = *pending;
        do {
            PathNode* nx = q->next;
            Free(q);
            q = nx;
        } while (q);
        *pending = nullptr;
    }
    return blocked == 0;
}
RT3_REPLACE(0x49cc40, Track_PlanRoute);

// 0x49d320. Track network net: drops the preview piece +0x129 - restoring
// its saved corners (+0x199 / +0x1a5 / +0x1b1 / +0x1bd) and removing it.
RT3_NOINLINE void __fastcall TrackNet_DropPreview(Collection* net) {
    uint8_t* b = (uint8_t*)net;
    int32_t id = F<int32_t>(b, 0x129);
    if (id == -1) return;
    ((void(__fastcall*)(uint8_t*, int, const uint8_t*, const uint8_t*, const uint8_t*, const uint8_t*, int32_t, int32_t, int32_t))(void*)&Track_Reshape)(
        (uint8_t*)Collection_Get(net, 0, (uint32_t)id), 0, b + 0x199, b + 0x1a5, b + 0x1b1, b + 0x1bd, 1, 0, 1);
    TrackNet_RemovePiece(net, 0, F<uint32_t>(b, 0x129), 1, -1);
    F<int32_t>(b, 0x129) = -1;
}
RT3_REPLACE(0x49d320, TrackNet_DropPreview);

 // namespace

// 0x49d380 (thiscall, RET 0x30). Track network net lays track for `company`
// from (x0, y0) / piece `start` towards (x1, y1), or the node `end` (-1: the
// nearest node within radius x 0x5cfe60 of the target, or one met along the
// line). In `mode` 1 / 2 (build) it is real track with routes (0x6cfcb4),
// otherwise a preview. A company without pieces left (+0x7680) gets
// nothing. The walk (0x49bd40) first follows existing track; the rest is
// planned (0x49cc40 / 0x498320) into a path of points, and along each leg
// pieces of about 0x5c97e0 are added (0x493ec0, kept original) whose
// corners follow the heading, bent by at most the turn the leg allows,
// checked against water / rough cells (0x448af0 / 0x449980), the slope
// (0x4951f0) and the company's territories, shaped (0x4a3810), linked and
// put on a route. A run that ends on the node `end` is matched to it
// (Tracks_MoveEnds, links, route merge or split, 0x4996e0). Too sharp a
// turn, a blocked cell or a failed piece undoes the run (preview) or the
// whole step (build: 0x4a3d00 / 0x4a3ec0). The cost goes to net +0x1d1 and
// in build mode is charged (0x49ae00). Returns the last piece (or `start`),
// *out tells whether the run reached `end`.
RT3_NOINLINE int32_t __fastcall TrackNet_BuildLine(Collection* net, int, float x0, float y0, int32_t start, float x1, float y1, int32_t mode, int32_t company,
                                      int32_t kind, int32_t p9, float radius, int32_t end, int32_t* out) {
    uint8_t* const n = (uint8_t*)net;
    Collection* const tracks = g_tracks;
    Collection* const routes = g_routes;
    const bool build = (uint8_t)mode == 1 || (uint8_t)mode == 2;
    BuildFrame fr;
    memset(fr.b, 0, sizeof fr.b);
    int32_t esi, ebx;
    int32_t at = start; // the caller's start slot, later the last piece
    float* pth;
    // Turn between the walked piece's direction and the line from its centre
    // to (tx, ty).
    auto turn = [&](float tx, float ty) {
        float a1 = (float)AngleOf(fr.f(0x88), fr.f(0x80), tx, ty);
        float a2 = (float)AngleOf(BlCell(fr.f(0x170)), BlCell(fr.f(0x178)), BlCell(fr.f(0x164)), BlCell(fr.f(0x16c)));
        return BlAbs(AngleDelta(nullptr, 0, a2, a1));
    };
    auto restore = [&] {
        memcpy(n + 0x139, fr.b + 0x124, 0xc);
        memcpy(n + 0x145, fr.b + 0x13c, 0xc);
        memcpy(n + 0x151, fr.b + 0x118, 0xc);
        memcpy(n + 0x15d, fr.b + 0x130, 0xc);
    };
    auto map = [] { return g_map; };
    auto cellFlag = [&](double x, double y) { return Map_CellFlag(map(), 0, Ftol(x * Fl_207(0x5c8568)), Ftol(y * Fl_207(0x5c8568)), 2); };
    fr.i(0x188) = 0;
    fr.i(0x78) = 1;
    RunDebugValidation();
    fr.i(0x94) = 0;
    fr.i(0xdc) = 0;
    fr.i(0x20) = 0;
    F<int32_t>(n, 0x1d5) = F<int32_t>(n, 0x90);
    fr.f(0xd4) = x0;
    fr.f(0xe4) = y0;
    fr.i(0x1c) = start;
    fr.i(0x10) = -1;
    esi = start;
    if (Collection_IsValid(g_companies, 0, (uint32_t)company) &&
        Company_Get7680(Collection_Get(g_companies, 0, (uint32_t)company)) <= 0) {
        F<float>(n, 0x1d1) = (float)fr.i(0xdc);
        ebx = esi;
        goto done;
    }
    {
        uint8_t* tool = g_6d1a8c;
        if (tool && !F<int32_t>(tool, 0x83) && !F<int32_t>(n, 0xe9)) {
            fr.i(0xf4) = 1;
            Cursor_Set(g_cursorManager, 0, 4, 0, 0);
        } else {
            fr.i(0xf4) = 0;
        }
    }
    esi = TrackNet_WalkLine(net, 0, esi, fr.f(0xd4), fr.f(0xe4), x1, y1, mode, company, kind, &fr.i(0xdc), &fr.i(0xc8), &fr.i(0x28));
    at = esi;
    if (F<int32_t>(g_world, 0x4aab) && esi != -1 && F<uint16_t>(BlTrack(net, esi), 0x22e) != company) goto walked;
    if (fr.i(0xc8)) goto onTrack;
    if (!fr.i(0x28)) goto fail;
walked:
    ebx = esi;
leave:
    if (ebx != fr.i(0x1c)) goto finishRun;
    F<float>(n, 0x1d1) = (float)fr.i(0xdc);
    goto done;

onTrack:
    // The walk ended on track: when it already points at the target, stop.
    Obj_CenterCell(BlTrack(tracks, esi), 0, &fr.f(0x88), &fr.f(0x80));
    Effect_GetVectors(BlTrack(tracks, esi), 0, fr.p(0x170), nullptr, nullptr, fr.p(0x164));
    if (Distance2D(fr.f(0x88), fr.f(0x80), x1, y1) < D(0x5cfea0) && Node_HasLink(BlTrack(tracks, esi), 0, 1) && turn(x1, y1) > D(0x5ca1b0) &&
        turn(x1, y1) < D(0x5cfe98)) {
        F<float>(n, 0x1d1) = (float)fr.i(0xdc);
        ebx = fr.i(0x1c);
        goto done;
    }
    fr.f(0xc8) = (float)((double)radius * D(0x5cfe60));
    ebx = end;
    if (ebx == -1) {
        int32_t r = Grid_NearestNodeWithin(net, 0, x1, y1, esi, fr.f(0xc8), 1, 1, 0);
        if (r == -1) {
            end = -1;
            goto searched;
        }
        ebx = end = r;
    }
    if (!F<int32_t>(n, 0xe9) || (IsMultiplayer() && Net_CommonVersion() <= Fl_207(0x5cfe90))) {
        if (Nodes_FindAlongLine(net, 0, fr.f(0x88), fr.f(0x80), x1, y1, company, esi, &fr.i(0xc8))) ebx = end = fr.i(0xc8);
    }
searched:
    if (esi != fr.i(0x1c)) {
        int32_t cur = F<int32_t>(n, 0x129);
        if (cur != esi) {
            if (cur != -1)
                Track_Reshape_Impl(BlTrack(net, cur), 0, (uint32_t*)(n + 0x199), (uint32_t*)(n + 0x1a5), (uint32_t*)(n + 0x1b1), (uint32_t*)(n + 0x1bd), 1,
                                   0, 1);
            F<int32_t>(n, 0x129) = esi;
            Effect_GetVectors(BlTrack(net, esi), 0, (uint32_t*)(n + 0x199), (uint32_t*)(n + 0x1a5), (uint32_t*)(n + 0x1b1), (uint32_t*)(n + 0x1bd));
        }
        Obj_CenterCell(BlTrack(net, esi), 0, &fr.f(0xd4), &fr.f(0xe4));
        if (ebx != -1) {
            memcpy(fr.b + 0x124, n + 0x139, 0xc);
            memcpy(fr.b + 0x13c, n + 0x145, 0xc);
            memcpy(fr.b + 0x118, n + 0x151, 0xc);
            memcpy(fr.b + 0x130, n + 0x15d, 0xc);
            fr.i(0x20) = 1;
            Effect_GetVectors(BlTrack(net, esi), 0, (uint32_t*)(n + 0x139), (uint32_t*)(n + 0x145), (uint32_t*)(n + 0x151), (uint32_t*)(n + 0x15d));
            ebx = end;
        }
        fr.i(0x78) = 0;
    }
    if (esi == ebx) {
        // The walk reached the end node.
        if (fr.i(0x20)) restore();
        if (fr.i(0xdc) > 0 && build) Tracks_ApplyUpgrades(net, 0, (uint32_t)company, fr.i(0xdc));
        F<float>(n, 0x1d1) = (float)fr.i(0xdc);
        ebx = fr.i(0x1c);
        goto done;
    }
    if (ebx != -1) {
        Obj_CenterCell(BlTrack(net, ebx), 0, &x1, &y1);
        if (!Track_PlanCurve(n, 0, (uint32_t)esi, (uint32_t)ebx, BlBits(x1), BlBits(y1), (uint32_t**)fr.p(0x94))) {
            if (fr.i(0x20)) restore();
            if (fr.i(0xf4)) Cursor_Set(g_cursorManager, 0, 5, 0, 0);
            F<float>(n, 0x1d1) = (float)fr.i(0xdc);
            ebx = fr.i(0x1c);
            goto done;
        }
    }
    if (fr.i(0x20)) restore();
    fr.i(0x7c) = 0;
    {
        int32_t r = Track_PlanRoute(n, 0, fr.f(0xd4), fr.f(0xe4), x1, y1, (PathNode**)fr.p(0x7c), (PathNode**)fr.p(0x94));
        ebx = fr.i(0x7c);
        F<int32_t>(n, 0x1c9) = ebx;
        if (!r && (p9 || end != -1)) {
            F<float>(n, 0x1d1) = (float)fr.i(0xdc);
            ebx = fr.i(0x1c);
            goto done;
        }
    }
    pth = (float*)(uintptr_t)ebx;
    if (!(x1 == pth[0] && y1 == pth[1])) {
        if (Distance2D(fr.f(0x88), fr.f(0x80), pth[0], pth[1]) < D(0x5cfea0) && Node_HasLink(BlTrack(tracks, esi), 0, 1) &&
            turn(pth[0], pth[1]) > D(0x5ca1b0) && turn(pth[0], pth[1]) < D(0x5cfe98))
            goto fail;
    }
    if (F<int32_t>(g_app, 0x9c) && build) {
        // Debug: number the planned points on the map.
        fr.i(0x58) = 1;
        Num_Format((char*)fr.b + 0x98, 1, 1.0, 10, 1, 0, 0);
        Markers_Add(g_6ada80, 0, fr.p(0xd4)[0], fr.p(0xe4)[0], (const char*)fr.b + 0x98, 1);
        for (uint8_t* q = (uint8_t*)(uintptr_t)fr.i(0x7c); q; q = F<uint8_t*>(q, 8)) {
            fr.i(0x58) += 1;
            fr.d(0xa0) = (double)fr.i(0x58);
            Num_Format((char*)fr.b + 0x98, 1, fr.d(0xa0), 10, 1, 0, 0);
            Markers_Add(g_6ada80, 0, F<uint32_t>(q, 0), F<uint32_t>(q, 4), (const char*)fr.b + 0x98, 1);
        }
    }
    {
        pth = (float*)(uintptr_t)fr.i(0x7c);
        fr.f(0x3c) = (float)AngleOf(fr.f(0xd4), fr.f(0xe4), pth[0], pth[1]);
        fr.f(0x18) = (float)BlCos(fr.f(0x3c));
        fr.f(0x60) = (float)BlSin(fr.f(0x3c));
        fr.i(0x38) = 0;
        fr.i(0x94) = esi;
        uint8_t bl;
        if (!Node_HasLink(BlTrack(tracks, esi), 0, 0)) bl = BlTrack(tracks, esi)[0x216] ? 3 : 0;
        else bl = Node_HasFreeLink(BlTrack(tracks, esi), 0, 0) ? 1 : 2;
        fr.u8(0x17) = bl;
        fr.i(0x24) = 0;
        fr.i(0x180) = 0;
        fr.i(0xd8) = Track_HasLink(BlTrack(tracks, esi), 0, 1);
        fr.i(0x84) = Track_HasLink(BlTrack(tracks, esi), 0, 2);
        if (bl == 3) {
            fr.i(0xd8) = 1;
            fr.i(0x84) = 1;
        } else if (bl == 0 && !p9) {
            // A loose piece is replaced: the run starts beside its centre.
            BlVirt2(net, esi);
            fr.d(0xc8) = (double)fr.f(0x60) * D(0x5c97d8);
            fr.d(0xa0) = (double)fr.f(0xd4) - fr.d(0xc8);
            fr.d(0x98) = (double)fr.f(0x18) * D(0x5c97d8);
            fr.f(0x8c) = (float)(fr.d(0xa0) - fr.d(0x98));
            fr.d(0x28) = (double)fr.f(0xe4) - fr.d(0x98);
            fr.f(0x90) = (float)(fr.d(0x28) + fr.d(0xc8));
            fr.f(0xb4) = (float)(fr.d(0x98) + fr.d(0xa0));
            fr.f(0xb8) = (float)(fr.d(0x28) - fr.d(0xc8));
            if (build) {
                fr.i(0x70) = (int32_t)Routes_Add(routes);
                fr.i(0x38) = (int32_t)(uintptr_t)Collection_Get(routes, 0, (uint32_t)fr.i(0x70));
            }
            goto laying;
        }
        ebx = 0;
        fr.i(0x180) = 1;
        Effect_GetVectors(BlTrack(tracks, esi), 0, fr.p(0x100), fr.p(0x154), fr.p(0x148), fr.p(0xbc));
        if (fr.i(0xd8) == fr.i(0x84)) {
            // Both ends free (or both taken): start from the end nearer the target.
            fr.i(0x78) = 0;
            fr.f(0xc8) = (float)DistanceSq2D(fr.f(0x100), fr.f(0x108), (float)((double)x1 * Fl_207(0x5c8700)), (float)((double)y1 * Fl_207(0x5c8700)));
            if (DistanceSq2D(fr.f(0xbc), fr.f(0xc4), (float)((double)x1 * Fl_207(0x5c8700)), (float)((double)y1 * Fl_207(0x5c8700))) > fr.f(0xc8)) {
                fr.i(0xd8) = 0;
                fr.f(0x110) = BlCell(fr.f(0x154));
                fr.f(0x20) = BlCell(fr.f(0x15c));
                fr.f(0xfc) = BlCell(fr.f(0x100));
                fr.f(0x5c) = BlCell(fr.f(0x108));
            } else {
                fr.i(0x84) = 0;
                fr.f(0x10c) = BlCell(fr.f(0x148));
                fr.f(0x114) = BlCell(fr.f(0x150));
                fr.f(0xec) = BlCell(fr.f(0xbc));
                fr.f(0xe0) = BlCell(fr.f(0xc4));
            }
            ebx = 1;
        }
        if (fr.i(0x84)) {
            float h = fr.i(0x78) ? (float)AngleOf(F<float>(n, 0x15d), F<float>(n, 0x165), F<float>(n, 0x139), F<float>(n, 0x141))
                                 : (float)AngleOf(fr.f(0xbc), fr.f(0xc4), fr.f(0x100), fr.f(0x108));
            fr.f(0x84) = h;
            fr.i(0x24) = 1;
            ++ebx;
            fr.f(0x10c) = BlCell(fr.f(0x148));
            fr.f(0x114) = BlCell(fr.f(0x150));
            fr.i(0xb4) = fr.i(0x10c);
            fr.i(0xb8) = fr.i(0x114);
            fr.f(0xec) = BlCell(fr.f(0xbc));
            fr.i(0x8c) = fr.i(0xec);
            fr.f(0xe0) = BlCell(fr.f(0xc4));
            fr.i(0x90) = fr.i(0xe0);
        }
        if (fr.i(0xd8)) {
            float h = fr.i(0x78) ? (float)AngleOf(F<float>(n, 0x139), F<float>(n, 0x141), F<float>(n, 0x15d), F<float>(n, 0x165))
                                 : (float)AngleOf(fr.f(0x100), fr.f(0x108), fr.f(0xbc), fr.f(0xc4));
            fr.f(0x84) = h;
            fr.i(0x24) = 1;
            ++ebx;
            fr.f(0x110) = BlCell(fr.f(0x154));
            fr.i(0x8c) = fr.i(0x110);
            fr.f(0x20) = BlCell(fr.f(0x15c));
            fr.i(0x90) = fr.i(0x20);
            fr.f(0xfc) = BlCell(fr.f(0x100));
            fr.i(0xb4) = fr.i(0xfc);
            fr.f(0x5c) = BlCell(fr.f(0x108));
            fr.i(0xb8) = fr.i(0x5c);
        }
        if (build) {
            if (ebx == 2 && fr.u8(0x17) != 3) {
                ebx = (int32_t)Routes_Add(routes);
                fr.i(0x70) = ebx;
                Routes_SplitAtNode(routes, 0, BlTrack(tracks, esi), fr.i(0xd8));
            } else {
                if (F<int32_t>(BlTrack(tracks, esi), 0x222) != -1 && fr.u8(0x17) != 3) ebx = (int32_t)Routes_Add(routes);
                else if (F<int32_t>(g_world, 0x4c74) != (BlTrack(tracks, esi)[0x44] != 0 ? 1 : 0))
                    ebx = (int32_t)Routes_Add(routes);
                else ebx = F<int32_t>(BlTrack(tracks, esi), 0x212);
                fr.i(0x70) = ebx;
            }
            if (ebx != -1) fr.i(0x38) = (int32_t)(uintptr_t)Collection_Get(routes, 0, (uint32_t)ebx);
        }
    }
laying:
    // Lay pieces along each leg of the path.
    ebx = fr.i(0xd4);
    fr.i(0x160) = fr.i(0xe4);
    fr.i(0x58) = 0;
    fr.i(0x78) = 0;
    F<uint32_t>(n, 0x1d9) = 0xbf800000;
    F<uint32_t>(n, 0x1e1) = 0xbf800000;
leg:
    fr.i(0xf8) = ebx;
    fr.i(0xf0) = fr.i(0x160);
    pth = (float*)(uintptr_t)fr.i(0x7c);
    fr.f(0x44) = (float)Distance2D(*(float*)&ebx, fr.f(0x160), pth[0], pth[1]);
    fr.i(0x98) = Ftol((double)fr.f(0x44) / D(0x5c97e0) * D(0x5c85c8) + D(0x5c85c8));
    if (fr.i(0x180)) {
        fr.i(0x74) = fr.i(0x84);
        fr.i(0x180) = 0;
    } else if (!fr.i(0x78)) {
        fr.i(0x74) = fr.i(0x3c);
    }
    fr.f(0x3c) = (float)AngleOf(*(float*)&ebx, fr.f(0x160), pth[0], pth[1]);
    BlCos(fr.f(0x3c));
    BlSin(fr.f(0x3c));
    fr.f(0x54) = (float)BlAbs(AngleDelta(nullptr, 0, fr.f(0x3c), fr.f(0x74)));
    if (fr.f(0x54) == D(0x5c8608) || fr.f(0x44) == D(0x5c8608)) {
        fr.i(0xe8) = 0;
    } else {
        // Radius of the arc that turns by the leg's heading change.
        fr.f(0xe8) = (float)((double)fr.f(0x54) + fr.f(0x54));
        fr.f(0x40) = (float)BlCos(fr.f(0xe8));
        if (fr.f(0x40) == Fl_207(0x5c8598)) fr.i(0x40) = 0x38d1b717;
        fr.f(0x54) = (float)(BlCos(D(0x5c96f0) - fr.f(0x54)) * fr.f(0xe8) * fr.f(0x44) / fr.f(0x40));
        if (fr.f(0x54) == Fl_207(0x5c8598)) fr.i(0x54) = 0x38d1b717;
        fr.f(0xe8) = (float)((double)fr.f(0xe8) * D(0x5c97e0) / fr.f(0x54));
    }
    if (fr.f(0x3c) == fr.f(0x74)) {
        fr.i(0x54) = 1;
        fr.i(0x4c) = 0;
    } else {
        // Turn per piece: the arc's, at least 0.1 unless the whole turn is less.
        fr.d(0xa0) = fr.f(0xe8) < D(0x5c86b8) ? 0.1 : (double)fr.f(0xe8);
        fr.f(0x4c) = (float)fr.d(0xa0);
        int32_t h = fr.i(0x3c);
        fr.d(0x28) = (double)fr.f(0x4c);
        if (BlAbs(AngleDelta(&fr.i(0x184), 0, *(float*)&h, fr.f(0x74))) > fr.d(0x28)) fr.d(0x28) = (double)fr.f(0x4c);
        else fr.d(0x28) = BlAbs(AngleDelta(&fr.i(0x184), 0, *(float*)&h, fr.f(0x74)));
        fr.f(0x4c) = (float)fr.d(0x28);
        if (fr.f(0x3c) < fr.f(0x74)) fr.f(0x4c) = -fr.f(0x4c);
        if (fr.i(0x184)) fr.f(0x4c) = -fr.f(0x4c);
        fr.i(0x24) = 1;
        fr.i(0x54) = 0;
    }
    fr.i(0x40) = 0;
piece:
    if (!(fr.f(0x44) > D(0x5c97e0)) && fr.i(0x40)) goto nextLeg;
    if (fr.i(0x40) > fr.i(0x98) && fr.i(0x10) != -1 && F<int32_t>(BlTrack(net, fr.i(0x10)), 0x226) != 1) goto tooLong;
    if (kind == 1) fr.u8(0xf4) = 1;
    else fr.u8(0xf4) = F<float>(n, 0x1e1) != D(0x5c9bf0) && n[0x1fd] == 3 ? 1 : 4;
    if (!fr.i(0x54) && !(fr.f(0x74) == fr.f(0x3c))) {
        // Bend the heading towards the leg by one step.
        int32_t prev = fr.i(0x74), h = fr.i(0x3c);
        fr.d(0x28) = BlAbs(AngleDelta(nullptr, 0, *(float*)&h, *(float*)&prev));
        if (BlAbs(fr.f(0x4c)) >= fr.d(0x28)) {
            fr.i(0x74) = h;
            fr.i(0x54) = 1;
        } else {
            fr.f(0x74) = AngleAdd(nullptr, 0, *(float*)&prev, fr.f(0x4c));
        }
        fr.f(0x18) = (float)BlCos(fr.f(0x74));
        fr.f(0x60) = (float)BlSin(fr.f(0x74));
        fr.i(0x24) = 1;
    } else {
        fr.f(0x18) = (float)BlCos(fr.f(0x3c));
        fr.f(0x60) = (float)BlSin(fr.f(0x3c));
    }
    {
        // The piece's far corners: one step along the heading.
        const int32_t bent = fr.i(0x24);
        fr.d(0x28) = (double)fr.f(0x60) * D(0x5c97e0);
        fr.f(0x48) = (float)((double)fr.f(0x8c) + fr.d(0x28));
        fr.d(0x60) = (double)fr.f(0x18) * D(0x5c97e0);
        fr.f(0x50) = (float)((double)fr.f(0x90) + fr.d(0x60));
        fr.f(0x18) = (float)((double)fr.f(0xb4) + fr.d(0x28));
        fr.f(0x60) = (float)((double)fr.f(0xb8) + fr.d(0x60));
        if (bent) {
            // Square the far edge to the step.
            fr.f(0x24) = (float)((double)fr.f(0x48) - fr.f(0x8c));
            fr.f(0x60) = (float)((double)fr.f(0x50) - fr.f(0x90));
            fr.i(0x17c) = fr.i(0x60);
            if (fr.f(0x24) == Fl_207(0x5c8598)) fr.i(0x24) = 0x38d1b717;
            if (fr.f(0x17c) == Fl_207(0x5c8598)) fr.i(0x17c) = 0x38d1b717;
            fr.f(0x24) = (float)BlAtan(D(0x5c9bf0) / ((double)fr.f(0x17c) / fr.f(0x24)));
            if (fr.f(0x60) < D(0x5c8608)) fr.f(0x24) = (float)((double)fr.f(0x24) - D(0x5c96a8));
            fr.f(0x18) = (float)(BlSin(fr.f(0x24)) * D(0x5c97e0) + fr.f(0x48));
            double c = BlCos(fr.f(0x24));
            fr.i(0x24) = 0;
            fr.f(0x60) = (float)(c * D(0x5c97e0) + fr.f(0x50));
        }
    }
    if (!fr.i(0x40) && !fr.i(0x78) && fr.u8(0x17)) {
        // The first piece reshapes the piece the run starts from.
        if (fr.u8(0x17) == 1) {
            float a = fr.i(0xd8) ? (float)AngleOf(fr.f(0xfc), fr.f(0x5c), fr.f(0x18), fr.f(0x60)) : (float)AngleOf(fr.f(0xec), fr.f(0xe0), fr.f(0x48), fr.f(0x50));
            fr.f(0x28) = a;
            if (BlAbs(AngleDelta(nullptr, 0, fr.f(0x28), fr.f(0x84))) > D(0x5c8880)) goto fail;
            esi = fr.i(0x94);
            if (build) {
                Effect_GetVectors(BlTrack(tracks, esi), 0, fr.p(0x124), fr.p(0x13c), fr.p(0x118), fr.p(0x130));
                if (esi == F<int32_t>(n, 0x125))
                    Track_Reshape_Impl(BlTrack(tracks, esi), 0, (uint32_t*)(n + 0x139), (uint32_t*)(n + 0x145), (uint32_t*)(n + 0x151),
                                       (uint32_t*)(n + 0x15d), 1, 0, 0);
                else if (esi == F<int32_t>(n, 0x129))
                    Track_Reshape_Impl(BlTrack(tracks, esi), 0, (uint32_t*)(n + 0x199), (uint32_t*)(n + 0x1a5), (uint32_t*)(n + 0x1b1),
                                       (uint32_t*)(n + 0x1bd), 1, 0, 0);
                Track_Reshape_Impl(BlTrack(tracks, esi), 0, fr.p(0x124), fr.p(0x13c), fr.p(0x118), fr.p(0x130), 1, 0, 0);
            }
            auto reshape = hook::Orig<void(__fastcall*)(uint8_t*, int, float, float, float, float, float, float, float, float, int32_t, int32_t)>(0x48f7f0);
            if (fr.i(0xd8))
                reshape(BlTrack(tracks, esi), 0, fr.f(0xfc), fr.f(0x5c), fr.f(0x110), fr.f(0x20), fr.f(0x48), fr.f(0x50), fr.f(0x18), fr.f(0x60), 0, 1);
            else
                reshape(BlTrack(tracks, esi), 0, fr.f(0x48), fr.f(0x50), fr.f(0x18), fr.f(0x60), fr.f(0x10c), fr.f(0x114), fr.f(0xec), fr.f(0xe0), 0, 1);
            esi = at;
            ebx = company;
            goto placed;
        }
        // A dead end or a fresh piece: continue from its end nearer the target.
        fr.i(0x24) = 1;
        fr.f(0x28) = (float)DistanceSq2D(fr.f(0xfc), fr.f(0x5c), x1, y1);
        if (DistanceSq2D(fr.f(0xec), fr.f(0xe0), x1, y1) > fr.f(0x28)) {
            fr.i(0x48) = fr.i(0xfc);
            fr.i(0x50) = fr.i(0x5c);
            fr.i(0x18) = fr.i(0x110);
            fr.i(0x60) = fr.i(0x20);
        } else {
            fr.i(0x18) = fr.i(0xec);
            fr.i(0x60) = fr.i(0xe0);
            fr.i(0x48) = fr.i(0x10c);
            fr.i(0x50) = fr.i(0x114);
        }
        esi = at;
        ebx = company;
        goto placed;
    }
    if (cellFlag(fr.f(0x48), fr.f(0x50))) goto blockedA;
    if (cellFlag(fr.f(0x18), fr.f(0x60))) goto blockedB;
    esi = fr.i(0x10);
    if (esi != -1 && F<int32_t>(BlTrack(net, esi), 0x226) != 1 && !BlVirt5(BlTrack(net, esi))) {
        if (cellFlag(((double)fr.f(0x48) + fr.f(0x8c)) / Fl_207(0x5c878c), ((double)fr.f(0x50) + fr.f(0x90)) / Fl_207(0x5c878c))) goto blockedC;
        if (cellFlag(((double)fr.f(0x18) + fr.f(0xb4)) / Fl_207(0x5c878c), ((double)fr.f(0x60) + fr.f(0xb8)) / Fl_207(0x5c878c))) goto blockedA;
    }
    if (Terrain_IsSpecial(map(), 0, Ftol(((double)fr.f(0x18) + fr.f(0x48)) / Fl_207(0x5c878c)), Ftol(((double)fr.f(0x60) + fr.f(0x50)) / Fl_207(0x5c878c))) &&
        Terrain_IsSpecial(map(), 0, Ftol(x1), Ftol(y1)))
        goto abandon;
    ebx = company;
    esi = hook::Orig<int32_t(__fastcall*)(Collection*, int, float, float, float, float, float, float, float, float, int32_t, int32_t, int32_t)>(0x493ec0)(
        net, 0, fr.f(0x48), fr.f(0x50), fr.f(0x18), fr.f(0x60), fr.f(0xb4), fr.f(0xb8), fr.f(0x8c), fr.f(0x90), mode, ebx, fr.i(0xf4));
    if (esi == -1) goto abandon;
    fr.i(0xdc) += Track_BuildCost(BlTrack(net, esi), 0, (uint32_t)ebx, 0);
placed:
    if (Collection_IsValid(g_companies, 0, (uint32_t)ebx)) {
        fr.i(0x28) = hook::As<int32_t>(g_tracks);
        uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, (uint32_t)ebx);
        if (!Company_EntryFlag(co, 0, (int32_t)Obj_CenterRegionType(BlTrack((Collection*)(uintptr_t)fr.i(0x28), esi)))) goto noTerritory;
    }
    if (fr.i(0x10) != -1 && fr.i(0x10) != esi) TrackNet_LinkPair(tracks, 0, (uint32_t)esi, (uint32_t)fr.i(0x10), fr.i(0x10) != at ? 1 : 0, 1);
    else at = esi;
    ebx = fr.i(0x40) < 2 && !fr.i(0x78) && fr.u8(0x17) ? fr.i(0x40) - 1 : fr.i(0x40);
    pth = (float*)(uintptr_t)fr.i(0x7c);
    if (!TrackNet_ShapePiece(net, 0, BlTrack(net, esi), pth[0], pth[1], ebx, (uint32_t)company, fr.f(0x54), p9)) goto shapeFailed;
    Obj_CenterCell(BlTrack(net, esi), 0, &fr.f(0xf8), &fr.f(0xf0));
    if (Terrain_IsSpecial(map(), 0, Ftol(fr.f(0xf8)), Ftol(fr.f(0xf0))) && !Terrain_IsSpecial(map(), 0, Ftol(x1), Ftol(y1)) &&
        F<float>(n, 0x1e1) < D(0x5c8608) && F<float>(n, 0x1d9) < D(0x5c8608))
        goto special;
    if (!BlVirt5(BlTrack(net, esi)) && F<int32_t>(BlTrack(net, esi), 0x226) != 1) {
        ebx = fr.i(0x10);
        if ((ebx == -1 || (!BlVirt5(BlTrack(net, ebx)) && F<int32_t>(BlTrack(net, ebx), 0x226) != 1)) && fr.i(0x40) &&
            Map_SlopeBetween(fr.f(0x48), fr.f(0x50), fr.f(0x18), fr.f(0x60), fr.f(0xb4), fr.f(0xb8), fr.f(0x8c), fr.f(0x90)) > D(0x5c8640))
            goto tooSteep;
    }
    if (build && F<int32_t>(BlTrack(net, esi), 0x212) == -1) {
        // Put the piece on the run's route.
        uint8_t* r = (uint8_t*)(uintptr_t)fr.i(0x38);
        bool extend = true;
        if (fr.i(0x40) || fr.i(0x78)) {
            const int32_t a = F<int32_t>(r, 4), b = F<int32_t>(r, 8);
            if (a != -1 && b != -1 && a != b) {
                extend = false;
                if (Pair_Contains(r, 0, fr.i(0x10))) {
                    Routes_AddRef(routes, 0, (uint32_t)fr.i(0x70), 1);
                    F<int32_t>(r, 0x14) += 1;
                    if (!Route_ReplaceEnd(r, 0, esi, fr.i(0x10))) {
                        Routes_AddRef(routes, 0, (uint32_t)fr.i(0x70), -1);
                        F<int32_t>(r, 0x14) -= 1;
                        fr.i(0x70) = (int32_t)Routes_Add(routes);
                        r = (uint8_t*)Collection_Get(routes, 0, (uint32_t)fr.i(0x70));
                        fr.i(0x38) = (int32_t)(uintptr_t)r;
                        Route_AddNode(r, 0, (uint32_t)esi);
                        Routes_AddRef(routes, 0, (uint32_t)fr.i(0x70), 1);
                        F<int32_t>(r, 0x14) += 1;
                    }
                }
            }
        }
        if (extend) {
            Routes_AddRef(routes, 0, (uint32_t)fr.i(0x70), 1);
            F<int32_t>(r, 0x14) += 1;
            if (!Route_AddNode(r, 0, (uint32_t)esi)) {
                Routes_AddRef(routes, 0, (uint32_t)fr.i(0x70), -1);
                F<int32_t>(r, 0x14) -= 1;
                fr.i(0x70) = (int32_t)Routes_Add(routes);
                r = (uint8_t*)Collection_Get(routes, 0, (uint32_t)fr.i(0x70));
                fr.i(0x38) = (int32_t)(uintptr_t)r;
                Route_AddNode(r, 0, (uint32_t)esi);
            }
        }
        Node_Set212(BlTrack(net, esi), 0, fr.i(0x70));
        Track_MarkCells(BlTrack(net, esi));
        if (!F<int32_t>(n, 0xe9) && (!IsMultiplayer() || Obj_Get21(g_world) == company)) Ui_PlaySound(0x2e, 0, 0x3f000000);
    }
    fr.i(0xc8) = fr.i(0x44);
    fr.i(0x10) = esi;
    Obj_CenterCell(BlTrack(net, esi), 0, &fr.f(0xf8), &fr.f(0xf0));
    pth = (float*)(uintptr_t)fr.i(0x7c);
    fr.f(0x44) = (float)Distance2D(fr.f(0xf8), fr.f(0xf0), pth[0], pth[1]);
    if (end != -1) fr.f(0x44) = (float)((double)fr.f(0x44) - D(0x5cfe88));
    if (fr.f(0x44) > D(0x5c97e0) || F<int32_t>(BlTrack(net, esi), 0x226) == 1 || F<int32_t>(BlTrack(net, esi), 0x226) == 0xc) {
        // The next piece starts at this one's far edge.
        fr.i(0x8c) = fr.i(0x48);
        fr.i(0x90) = fr.i(0x50);
        fr.i(0xb4) = fr.i(0x18);
        fr.i(0xb8) = fr.i(0x60);
    }
    if (!fr.i(0x54)) fr.f(0x3c) = (float)AngleOf(fr.f(0xf8), fr.f(0xf0), pth[0], pth[1]);
    {
        const int32_t count = ++fr.i(0x40);
        if (fr.f(0x44) < fr.f(0xc8) && !fr.i(0x58) && count > 2) {
            fr.i(0x58) = 1;
        } else if (fr.f(0xc8) < fr.f(0x44) && (fr.i(0x58) || (fr.i(0x54) && count > 1))) {
            fr.i(0x44) = 0; // moving away from the point: on to the next leg
        }
    }
    if (fr.f(0x44) < D(0x5c97e0) && (F<int32_t>(BlTrack(net, esi), 0x226) == 1 || F<int32_t>(BlTrack(net, esi), 0x226) == 0xc)) fr.i(0x44) = 0x461c4000;
    goto piece;

nextLeg:
    fr.i(0x7c) = F<int32_t>((void*)(uintptr_t)fr.i(0x7c), 8);
    if (!fr.i(0x7c)) goto reached;
    fr.i(0x10c) = fr.i(0xb4);
    fr.i(0xfc) = fr.i(0xb4);
    fr.i(0x114) = fr.i(0xb8);
    fr.i(0x5c) = fr.i(0xb8);
    fr.i(0xec) = fr.i(0x8c);
    fr.i(0x110) = fr.i(0x8c);
    fr.i(0x160) = fr.i(0xf0);
    {
        const int32_t o90 = fr.i(0x90);
        fr.i(0x90) = fr.i(0x50);
        fr.i(0xe0) = o90;
        fr.i(0x20) = o90;
    }
    fr.i(0xb4) = fr.i(0x18);
    fr.i(0x8c) = fr.i(0x48);
    fr.i(0x78) += 1;
    ebx = fr.i(0xf8);
    fr.i(0x58) = 0;
    at = esi;
    fr.i(0x94) = esi;
    fr.u8(0x17) = 1;
    fr.i(0xb8) = fr.i(0x60);
    Obj_CenterCell(BlTrack(tracks, esi), 0, &fr.f(0x88), &fr.f(0x80));
    Effect_GetVectors(BlTrack(tracks, esi), 0, fr.p(0x170), nullptr, nullptr, fr.p(0x164));
    if (F<int32_t>(n, 0x12d) != -1) goto leg;
    pth = (float*)(uintptr_t)fr.i(0x7c);
    if (!(Distance2D(fr.f(0x88), fr.f(0x80), pth[0], pth[1]) < D(0x5cfea0))) goto leg;
    fr.f(0x28) = (float)AngleOf(BlCell(fr.f(0x170)), BlCell(fr.f(0x178)), BlCell(fr.f(0x164)), BlCell(fr.f(0x16c)));
    fr.f(0x98) = (float)AngleOf(fr.f(0x88), fr.f(0x80), x1, y1);
    fr.f(0x28) = (float)AngleDelta(nullptr, 0, fr.f(0x28), fr.f(0x98));
    if (!(BlAbs(fr.f(0x28)) > D(0x5ca1b0))) goto leg;
    if (!(BlAbs(fr.f(0x28)) < D(0x5cfe98))) goto leg;
    ebx = fr.i(0x1c);
    goto removeRun;

tooLong:
    if (p9) goto undoA;
    F<int32_t>(n, 0x135) = 1;
abandon:
    ebx = fr.i(0x10);
    goto leave;
blockedA:
    if (p9 || !(BlAbs(fr.f(0x4c)) < D(0x5c8590))) goto undoB;
    ebx = fr.i(0x10);
    F<int32_t>(n, 0x135) = 1;
    goto leave;
blockedB:
    if (p9 || !(BlAbs(fr.f(0x4c)) < D(0x5c8590))) {
        ebx = fr.i(0x1c);
        if (build) goto undoAll;
        goto removeRun;
    }
    ebx = fr.i(0x10);
    F<int32_t>(n, 0x135) = 1;
    goto leave;
blockedC:
    if (p9 || !(BlAbs(fr.f(0x4c)) < D(0x5c8590))) goto undoA;
    ebx = fr.i(0x10);
    F<int32_t>(n, 0x135) = 1;
    goto leave;
undoA:
undoB:
    ebx = fr.i(0x1c);
    if (build) goto undoAll;
removeRun:
    TrackNet_RemovePiece(net, 0, (uint32_t)fr.i(0x10), 1, -1);
    F<float>(n, 0x1d1) = (float)fr.i(0xdc);
    goto done;
undoAll:
    Obj_PushIdNode(net, 0, 0, Obj_Get21(g_world));
    TrackNet_Undo(net, 0, 0xffffffff);
    F<float>(n, 0x1d1) = (float)fr.i(0xdc);
    goto done;
noTerritory:
    ebx = fr.i(0x10);
    if (F<uint16_t>(BlTrack(net, esi), 0x22e) == company) BlVirt2(net, esi);
    goto leave;
shapeFailed:
    ebx = fr.i(0x10);
    BlVirt2(net, esi);
    goto leave;
special:
    ebx = fr.i(0x10);
    BlVirt2(net, esi);
    Tracks_ApplyPendingKinds(net);
    goto leave;
tooSteep:
    F<int32_t>(n, 0x135) = 1;
    BlVirt2(net, esi);
    ebx = fr.i(0x10);
    goto leave;

reached:
    // The path is laid: finish the last piece and join the end node.
    ebx = end;
    esi = fr.i(0x10);
    if (F<int32_t>(g_world, 0x4c74) && esi != ebx && esi != at) {
        if (F<int32_t>(BlTrack(net, esi), 0x248)) Track_DropModel(BlTrack(net, esi));
        Track_AddCatenary(BlTrack(net, esi), 0, 0);
    }
    if (F<int32_t>(BlTrack(net, esi), 0x226) == 1 || F<int32_t>(BlTrack(net, esi), 0x226) == 0xc) {
        ebx = fr.i(0x1c);
        TrackNet_RemovePiece(net, 0, (uint32_t)ebx, 0, -1);
        F<float>(n, 0x1d1) = (float)fr.i(0xdc);
        goto done;
    }
    if (uint8_t* r = (uint8_t*)(uintptr_t)fr.i(0x38)) {
        if (F<int32_t>(r, 8) == -1 && !Route_AddNode(r, 0, (uint32_t)esi))
            Route_AddNode((uint8_t*)Collection_Get(routes, 0, Routes_Add(routes)), 0, (uint32_t)esi);
    }
    if (ebx != -1 && ebx != at) {
        uint8_t* E = BlTrack(net, ebx);
        fr.i(0xc8) = (int32_t)(uintptr_t)E;
        uint8_t* P = BlTrack(net, esi);
        fr.i(0x94) = (int32_t)(uintptr_t)P;
        if (F<int32_t>(E, 0x206) != esi && F<int32_t>(E, 0x20a) != esi) {
            Effect_GetVectors(E, 0, fr.p(0xa0), fr.p(0x18c), fr.p(0x148), fr.p(0x28));
            Effect_GetVectors(P, 0, fr.p(0x60), fr.p(0x100), fr.p(0x154), fr.p(0xbc));
            fr.i(0x98) = Track_HasLink(P, 0, 1);
            fr.i(0x58) = F<int32_t>(E, 0x20e) == -1 ? 1 : 0;
            const int32_t freeA = Track_HasLink(E, 0, 1) ? 0 : 1;
            const int32_t freeB = Track_HasLink(E, 0, 2) ? 0 : 1;
            fr.i(0x5c) = 0;
            const uint32_t id = (uint32_t)fr.i(0x10);
            // The last piece's free end meets the end node's nearer free end.
            auto check = [&](int32_t freeEnd, float ax, float ay, float bx, float by) {
                if (!fr.i(0x58)) {
                    if (!freeEnd) return false;
                } else if (!freeEnd) {
                    fr.i(0x5c) = 1;
                }
                fr.f(0x38) = (float)AngleOf(ax, ay, bx, by);
                return !(BlAbs(AngleDelta(nullptr, 0, fr.f(0x20), fr.f(0x38))) > D(0x5c8880));
            };
            auto same = [&](uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
                return (fr.f(a) > fr.f(b) && fr.f(c) > fr.f(d)) || (fr.f(a) < fr.f(b) && fr.f(c) < fr.f(d));
            };
            if (fr.i(0x98)) {
                fr.f(0x20) = (float)AngleOf(fr.f(0x60), fr.f(0x68), fr.f(0xbc), fr.f(0xc4));
                fr.f(0x98) = (float)DistanceSq2D(fr.f(0x60), fr.f(0x68), fr.f(0xa0), fr.f(0xa8));
                if (DistanceSq2D(fr.f(0x60), fr.f(0x68), fr.f(0x28), fr.f(0x30)) > fr.f(0x98)) {
                    if (!check(freeA, fr.f(0xa0), fr.f(0xa8), fr.f(0x28), fr.f(0x30))) goto rejoinFailed;
                    if (same(0x68, 0x108, 0xa8, 0x18c)) Tracks_MoveEnds(net, 0, id, fr.p(0x60), fr.p(0x100), fr.p(0xa0), fr.p(0x18c));
                    else Tracks_MoveEnds(net, 0, id, fr.p(0x60), fr.p(0x100), fr.p(0x18c), fr.p(0xa0));
                } else {
                    if (!check(freeB, fr.f(0x28), fr.f(0x30), fr.f(0xa0), fr.f(0xa8))) goto rejoinFailed;
                    if (same(0x68, 0x108, 0x30, 0x150)) Tracks_MoveEnds(net, 0, id, fr.p(0x60), fr.p(0x100), fr.p(0x28), fr.p(0x148));
                    else Tracks_MoveEnds(net, 0, id, fr.p(0x60), fr.p(0x100), fr.p(0x148), fr.p(0x28));
                }
            } else {
                fr.f(0x20) = (float)AngleOf(fr.f(0xbc), fr.f(0xc4), fr.f(0x60), fr.f(0x68));
                fr.f(0x98) = (float)DistanceSq2D(fr.f(0xbc), fr.f(0xc4), fr.f(0xa0), fr.f(0xa8));
                if (DistanceSq2D(fr.f(0xbc), fr.f(0xc4), fr.f(0x28), fr.f(0x30)) > fr.f(0x98)) {
                    if (!check(freeA, fr.f(0xa0), fr.f(0xa8), fr.f(0x28), fr.f(0x30))) goto rejoinFailed;
                    Tracks_MoveEnds(net, 0, id, fr.p(0x18c), fr.p(0xa0), fr.p(0x154), fr.p(0xbc));
                } else {
                    if (!check(freeB, fr.f(0x28), fr.f(0x30), fr.f(0xa0), fr.f(0xa8))) goto rejoinFailed;
                    Tracks_MoveEnds(net, 0, id, fr.p(0x28), fr.p(0x148), fr.p(0x154), fr.p(0xbc));
                }
            }
            uint8_t* e = (uint8_t*)(uintptr_t)fr.i(0xc8);
            uint8_t* p = (uint8_t*)(uintptr_t)fr.i(0x94);
            TrackNet_LinkPair(tracks, 0, F<uint32_t>(p, 0x202), F<uint32_t>(e, 0x202), 1, 1);
            if (build) {
                if (!fr.i(0x5c)) {
                    Routes_Merge(routes, 0, F<uint32_t>(p, 0x212), F<uint32_t>(e, 0x212));
                    goto smooth;
                }
                Routes_SplitAtNode(routes, 0, e, 0);
            }
            if (!fr.i(0x5c)) {
            smooth:
                TrackNet_SmoothJoin(net, 0, end, fr.i(0x1c), mode, company, fr.i(0xf4), 0.15f);
            }
        }
    }
    fr.i(0x188) = 1;
rejoinFailed:
    ebx = fr.i(0x10);
    goto leave;

finishRun:
    if (fr.i(0x10) != -1) TrackNet_FinishRun(net, 0, fr.i(0x10), mode);
    {
        const int32_t cost = Ftol(Tracks_TotalCost(net, 0, (uint32_t)company));
        fr.i(0xdc) = cost;
        if (build) {
            DebugLog((const char*)0x5cfe78, cost, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234);
            Tracks_ApplyUpgrades(net, 0, (uint32_t)company, cost);
        }
        fr.f(0x98) = (float)fr.i(0xdc);
        const int32_t bits = fr.i(0x98);
        F<int32_t>(n, 0x1d1) = bits;
        RunDebugValidation();
        F<int32_t>(n, 0x1d1) = bits;
    }
    goto done;
fail:
    F<float>(n, 0x1d1) = (float)fr.i(0xdc);
    ebx = fr.i(0x1c);
done:
    if (out) *out = fr.i(0x188);
    return ebx;
}
RT3_REPLACE(0x49d380, TrackNet_BuildLine);

// 0x4a00b0. Track network net: ends the current build - forgets a vanished
// build piece (+0x125, also dropping +0x118), replays the edit, removes the
// build piece and the preview, gives back the build's counters (+0xa4 /
// +0xac, per the options) and clears the tooltip, list and hover state.
RT3_NOINLINE void __fastcall TrackNet_EndBuild(Collection* net) {
    uint8_t* b = (uint8_t*)net;
    if (F<int32_t>(b, 0x125) != -1 && !Collection_IsValid(net, 0, F<uint32_t>(b, 0x125))) {
        F<int32_t>(b, 0x125) = -1;
        F<int32_t>(b, 0x118) = 0;
    }
    Tracks_ReplayEdit(net);
    if (F<int32_t>(b, 0x125) != -1) TrackNet_RemovePiece(net, 0, F<uint32_t>(b, 0x125), 1, -1);
    F<int32_t>(b, 0x125) = -1;
    TrackNet_DropPreview(net);
    b[0x11c] = 0;
    if (F<int32_t>(b, 0x118)) {
        if (F<int32_t>(g_app, 0x25b)) Obj_DecA4(b);
        if (F<int32_t>(g_app, 0x257)) Obj_DecAc(b);
    }
    F<int32_t>(b, 0x118) = 0;
    Obj_SetText1c(g_cursorManager, 0, nullptr, 0xffffffff);
    Obj_FreeList1c9(net);
    F<uint32_t>(b, 0x169) = 0xbf800000;
    F<int32_t>(b, 0x131) = -1;
    Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 1, 0, -1);
}
RT3_REPLACE(0x4a00b0, TrackNet_EndBuild);

// 0x4a01a0 (thiscall, RET 0x28). Track network net lays a line for `company`
// from (x0, y0), or the centre of piece *from, to (x1, y1), or piece *to's
// centre. Nothing on special terrain (0x449980) at either end. Without a
// start piece a turned piece is placed first (0x493cf0, kept original,
// facing away from the target) and in build mode (`mode` 1 / 2) gets a
// route of its own. The builder is reset (0x495480) and TrackNet_BuildLine
// runs (kind 4 with `doubleKind`, else 1; radius +-1 by `positive`); when
// it reached the end *from / *to get the pieces. The planned path is
// freed and pending kinds applied. Returns whether it reached the end.
RT3_NOINLINE int32_t __fastcall TrackNet_LayLine(Collection* net, int, float x0, float y0, float x1, float y1, int32_t company, int32_t mode, int32_t doubleKind,
                                    int32_t* from, int32_t* to, float positive) {
    uint8_t* const n = (uint8_t*)net;
    int32_t reached = 0;
    int32_t a = from ? *from : -1;
    const int32_t b = to ? *to : -1;
    if (a != -1) Obj_CenterCell(Collection_Get(g_tracks, 0, (uint32_t)a), 0, &x0, &y0);
    if (b != -1) Obj_CenterCell(Collection_Get(g_tracks, 0, (uint32_t)b), 0, &x1, &y1);
    if (Terrain_IsSpecial(g_map, 0, Ftol(x0), Ftol(y0))) return 0;
    if (Terrain_IsSpecial(g_map, 0, Ftol(x1), Ftol(y1))) return 0;
    float facing = (float)(Fl_207(0x5c8768) - AngleOf(x1, y1, x0, y0));
    const bool fresh = a == -1;
    if (fresh) {
        a = hook::Orig<int32_t(__fastcall*)(Collection*, int, float, float, float, int32_t, int32_t, int32_t, int32_t)>(0x493cf0)(net, 0, x0, y0, facing, 0,
                                                                                                                                 company, 1, 0);
        if (a == -1) goto cleanup;
    }
    {
        uint8_t* p = (uint8_t*)Collection_Get(net, 0, (uint32_t)a);
        uint32_t v[4][3];
        Effect_GetVectors(p, 0, v[0], v[1], v[2], v[3]);
        memcpy(n + 0x139, v[0], 0xc);
        memcpy(n + 0x145, v[1], 0xc);
        memcpy(n + 0x151, v[2], 0xc);
        memcpy(n + 0x15d, v[3], 0xc);
        F<float>(n, 0x11d) = x0;
        F<int32_t>(n, 0x118) = 1;
        F<float>(n, 0x121) = y0;
        float cx, cy;
        Obj_CenterCell(p, 0, &cx, &cy);
        F<int32_t>(n, 0x125) = F<int32_t>(p, 0x202);
        if (fresh && ((uint8_t)mode == 1 || (uint8_t)mode == 2)) {
            Collection* routes = g_routes;
            const uint32_t id = Routes_Add(routes);
            uint8_t* r = (uint8_t*)Collection_Get(routes, 0, id);
            Node_Set212(p, 0, (int32_t)id);
            Route_AddNode(r, 0, (uint32_t)a);
            Route_AddNode(r, 0, (uint32_t)a);
            Routes_AddRef(routes, 0, id, 1);
            F<int32_t>(r, 0x14) += 1;
            Track_MarkCells(p);
        }
        Train_ResetMotion(n);
        float radius = (float)(*(const uint32_t*)&positive ? 1.0 : -1.0);
        const int32_t end = TrackNet_BuildLine(net, 0, x0, y0, a, x1, y1, mode, company, doubleKind ? 4 : 1, 0, radius, b, &reached);
        if (reached) {
            if (from) *from = a;
            if (to) *to = end;
        }
    }
cleanup:
    Obj_FreeList1c9(n);
    Tracks_ApplyPendingKinds(net);
    return reached;
}
RT3_REPLACE(0x4a01a0, TrackNet_LayLine);

 // namespace

// 0x4a04c0 (thiscall, RET 0x1c). Track tool drag on network net to (x, y)
// for `company`: the anchor is the last piece +0x125 at (+0x11d, +0x121);
// +0xdd / +0xe1 keep the cursor. The node under the cursor (+0x12d, within
// radius x 0x5cfe60, not the anchor) is snapped to:
// - none: the trial pieces (+0x129, the anchor +0x125, a reshaped +0x131
//   with its saved corners +0x169..) are undone and replaced by a fresh
//   anchor where needed (0x495660), then a line is laid to the node met on
//   the way or to (x, y); a run that only got back to the anchor restores
//   its corners;
// - the same node again (without `mode`) or the anchor: nothing;
// - a node without a free end: forgotten (unless `mode`);
// - otherwise the line is laid from that node back to the anchor (p9 set)
//   with the anchor's corners saved around it, or, with a placed anchor
//   (+0x11c), from the anchor to it unless they are linked already.
// Returns the last piece.
RT3_NOINLINE int32_t __fastcall TrackNet_ExtendTo(Collection* net, int, float x, float y, int32_t mode, int32_t company, int32_t kind, float radius, int32_t* out) {
    uint8_t* const n = (uint8_t*)net;
    auto piece = [&](int32_t id) { return (uint8_t*)Collection_Get(net, 0, (uint32_t)id); };
    auto valid = [&](int32_t id) { return Collection_IsValid(net, 0, (uint32_t)id) != 0; };
    auto vec = [&](uint32_t o) { return (uint32_t*)(n + o); };
    auto newAnchor = [&] {
        F<int32_t>(n, 0x125) = TrackNet_AddPieceTurned(net, 0, F<float>(n, 0x11d), F<float>(n, 0x121), 0.0f, 0, company, ExtendStyle(n, kind));
    };
    // Undo the reshape of +0x131 (when its saved corners are set) and
    // replace the anchor.
    auto undoReshape = [&] {
        const int32_t r = F<int32_t>(n, 0x131);
        if (r != -1 && r != F<int32_t>(n, 0x125) && F<float>(n, 0x169) >= D(0x5c8608)) {
            Track_Reshape_Impl(piece(r), 0, vec(0x169), vec(0x175), vec(0x181), vec(0x18d), 1, 0, 1);
            F<uint32_t>(n, 0x169) = 0xbf800000;
            TrackNet_RemovePiece(net, 0, F<uint32_t>(n, 0x125), 1, -1);
            newAnchor();
        }
    };
    auto dropTrial = [&] {
        TrackNet_RemovePiece(net, 0, F<uint32_t>(n, 0x129), 1, -1);
        if (valid(F<int32_t>(n, 0x129))) Track_Reshape_Impl(piece(F<int32_t>(n, 0x129)), 0, vec(0x199), vec(0x1a5), vec(0x1b1), vec(0x1bd), 1, 0, 1);
        F<int32_t>(n, 0x129) = -1;
    };
    const float r2 = (float)((double)radius * D(0x5cfe60));
    F<int32_t>(n, 0x131) = F<int32_t>(n, 0x12d);
    const uint8_t placed = n[0x11c];
    F<float>(n, 0xdd) = x;
    F<float>(n, 0xe1) = y;
    n[0xd8] = 0, n[0xd9] = 0, n[0xda] = 0, n[0xdb] = 0, n[0xdc] = 0;
    {
        const int32_t hit = Grid_NearestNodeWithin(net, 0, x, y, placed ? F<int32_t>(n, 0x125) : -1, r2, 1, 1, 0);
        F<int32_t>(n, 0x12d) = hit;
        if (hit == F<int32_t>(n, 0x125)) F<int32_t>(n, 0x12d) = -1;
    }
    const int32_t hit = F<int32_t>(n, 0x12d);
    if (hit == -1) {
        if (F<int32_t>(n, 0x129) != -1) {
            dropTrial();
            if (!valid(F<int32_t>(n, 0x125))) newAnchor();
        }
        TrackNet_RemovePiece(net, 0, F<uint32_t>(n, 0x125), 0, -1);
        undoReshape();
        int32_t last = -1;
        uint32_t saved[4][3] = {};
        if (valid(F<int32_t>(n, 0x125))) Effect_GetVectors(piece(F<int32_t>(n, 0x125)), 0, saved[0], saved[1], saved[2], saved[3]);
        int32_t found;
        Nodes_FindAlongLine(net, 0, F<float>(n, 0x11d), F<float>(n, 0x121), x, y, company, F<int32_t>(n, 0x125), &found);
        uint8_t* f = found != -1 ? piece(found) : nullptr;
        const int32_t anchor = F<int32_t>(n, 0x125);
        if (f && anchor != F<int32_t>(f, 0x206) && anchor != F<int32_t>(f, 0x20a) && anchor != F<int32_t>(f, 0x20e)) {
            float fx, fy;
            Obj_CenterCell(piece(found), 0, &fx, &fy);
            last = TrackNet_BuildLine(net, 0, F<float>(n, 0x11d), F<float>(n, 0x121), F<int32_t>(n, 0x125), fx, fy, mode, company, kind, 0, radius, -1, out);
        } else if (valid(F<int32_t>(n, 0x125))) {
            uint32_t v[4][3];
            Effect_GetVectors(piece(F<int32_t>(n, 0x125)), 0, v[0], v[1], v[2], v[3]);
            last = TrackNet_BuildLine(net, 0, F<float>(n, 0x11d), F<float>(n, 0x121), F<int32_t>(n, 0x125), x, y, mode, company, kind, 0, radius, -1, out);
        }
        if (last == F<int32_t>(n, 0x125) && valid(F<int32_t>(n, 0x125)))
            Track_Reshape_Impl(piece(F<int32_t>(n, 0x125)), 0, saved[0], saved[1], saved[2], saved[3], 1, 0, 0);
        return last;
    }
    if ((hit == F<int32_t>(n, 0x131) && !(uint8_t)mode) || hit == F<int32_t>(n, 0x125)) return F<int32_t>(n, 0x125);
    if (!Node_HasFreeLink(piece(hit), 0, 1) && !(uint8_t)mode) {
        F<int32_t>(n, 0x12d) = -1;
        return F<int32_t>(n, 0x125);
    }
    if (!n[0x11c]) {
        // Build back from the node to the anchor point, around a new anchor.
        undoReshape();
        TrackNet_RemovePiece(net, 0, F<uint32_t>(n, 0x125), 1, -1);
        uint8_t keep[0x30];
        memcpy(keep, n + 0x139, 0x30);
        Effect_GetVectors(piece(F<int32_t>(n, 0x12d)), 0, vec(0x169), vec(0x175), vec(0x181), vec(0x18d));
        F<int32_t>(n, 0x131) = -1;
        Effect_GetVectors(piece(F<int32_t>(n, 0x12d)), 0, vec(0x139), vec(0x145), vec(0x151), vec(0x15d));
        float cx, cy;
        Obj_CenterCell(piece(F<int32_t>(n, 0x12d)), 0, &cx, &cy);
        int32_t last = TrackNet_BuildLine(net, 0, cx, cy, F<int32_t>(n, 0x12d), F<float>(n, 0x11d), F<float>(n, 0x121), mode, company, kind, 1, radius, -1, out);
        if (last == F<int32_t>(n, 0x12d)) {
            Track_Reshape_Impl(piece(F<int32_t>(n, 0x12d)), 0, vec(0x139), vec(0x145), vec(0x151), vec(0x15d), 1, 0, 0);
            last = -1;
            F<uint32_t>(n, 0x169) = 0xbf800000;
        }
        memcpy(n + 0x139, keep, 0x30);
        return last;
    }
    uint8_t* a = piece(F<int32_t>(n, 0x125));
    TrackNet_RemovePiece(net, 0, F<uint32_t>(n, 0x125), 0, -1);
    const int32_t h = F<int32_t>(n, 0x12d);
    if (F<int32_t>(a, 0x206) == h || F<int32_t>(a, 0x20a) == h || F<int32_t>(a, 0x20e) == h) {
        if (out) *out = 0;
        return F<int32_t>(n, 0x125);
    }
    if (F<int32_t>(n, 0x129) != -1) dropTrial();
    float cx, cy;
    Obj_CenterCell(piece(F<int32_t>(n, 0x12d)), 0, &cx, &cy);
    return TrackNet_BuildLine(net, 0, F<float>(n, 0x11d), F<float>(n, 0x121), F<int32_t>(n, 0x125), cx, cy, mode, company, kind, 0, radius, -1, out);
}
RT3_REPLACE(0x4a04c0, TrackNet_ExtendTo);

// 0x4a0d20. Grade of a ramp that ends at (x0, y0) heading `ang`, approaching from
// (tx, ty). Walks back from 1.448 tiles behind the end in 0.362 steps until the
// ground drops below the end height (and is level there, |atan(dh / 33.304)| <= 0.1)
// or it passes the approach point, failing on special cells (record bit 9).
// Fails (1.5) too unless the ground somewhere rose 175 above the end. From the
// spot found (or the approach point) it then slides along while the ground's
// grade is below the world's limit (table 0x5f3404 by +0x140), reversing once it
// moves away; failing when it comes closer than 1.81 or after 290 steps. Stores the
// ramp's foot height in +0x1f1 and returns atan((foot - end) / length).
RT3_NOINLINE float __fastcall Track_RampGrade(uint8_t* self, int, float x0, float y0, float ang, float tx, float ty) {
    double c = Cos(ang), s = Sin(ang);
    float h0 = HeightAtTile(x0, y0);
    float bx = (float)((double)x0 - c * D(0x5cfed0));
    float by = (float)((double)y0 - s * D(0x5cfed0));
    if (HeightAtTile(bx, by) < h0) return Fl_205(0x5cb468);
    float px = bx, py = by;
    float sx = -1.0f, sy = 0.0f;
    float reach = (float)DistanceSq2D(bx, by, tx, ty);
    float maxRise = 0.0f;
    for (;;) {
        px = (float)((double)px - c * D(0x5cfec8));
        py = (float)((double)py - s * D(0x5cfec8));
        int32_t iy = Ftol(py);
        if (Terrain_RecBit9(Map(), 0, Ftol(px), iy)) return Fl_205(0x5cb468);
        float h = HeightAtTile(px, py);
        float rise = (float)((double)h - h0);
        if (rise > maxRise) maxRise = rise;
        if (DistanceSq2D(bx, by, px, py) > reach) break;
        if (!(h < h0)) continue;
        sx = px;
        sy = py;
        float ha = HeightAtTile((float)((double)px - c * D(0x5cfec0)), (float)((double)py - s * D(0x5cfec0)));
        float hc = HeightAtTile((float)(c * D(0x5cfec0) + px), (float)(s * D(0x5cfec0) + py));
        float level = (float)Atan(((double)hc - ha) / D(0x5cfeb8));
        if (!(fabs((double)level) > D(0x5c86b8))) break;
        sx = -1.0f;
    }
    if (maxRise < D(0x5cfeb0)) return Fl_205(0x5cb468);
    float mx, my, ax, ay, cx, cy;
    if (sx < D(0x5c8608)) {
        ax = tx;
        ay = ty;
        cx = (float)(c * D(0x5cfec0) + tx);
        cy = (float)(s * D(0x5cfec0) + ty);
        mx = (float)(((double)cx + tx) / D(0x5c85d0));
        my = (float)(((double)cy + ty) / D(0x5c85d0));
    } else {
        mx = sx;
        my = sy;
        ax = (float)((double)sx - c * D(0x5cfec0));
        ay = (float)((double)sy - s * D(0x5cfec0));
        cx = (float)(c * D(0x5cfec0) + sx);
        cy = (float)(s * D(0x5cfec0) + sy);
    }
    float ha = HeightAtTile(ax, ay);
    float hc = HeightAtTile(cx, cy);
    float grade = (float)Atan(((double)hc - ha) / D(0x5cfeb8));
    float prev = (float)Distance2D(bx, by, mx, my);
    float dir = 1.0f;
    auto limit = [] { return ((const float*)0x5f3404)[F<int32_t>(hook::As<void*>(g_app), 0x140)]; };
    if (grade < limit()) {
        for (int32_t i = 0;;) {
            if (i++ >= 300) break;
            if (i >= 0x122) return Fl_205(0x5cb468);
            ha = hc;
            ax = cx;
            ay = cy;
            cx = (float)(c * dir * D(0x5cfec8) + cx);
            cy = (float)(s * dir * D(0x5cfec8) + cy);
            hc = HeightAtTile(cx, cy);
            mx = (float)(((double)cx + ax) / D(0x5c85d0));
            my = (float)(((double)cy + ay) / D(0x5c85d0));
            float d = (float)Distance2D(bx, by, mx, my);
            if (d > prev) dir = -1.0f;
            prev = d;
            if (d < D(0x5cfea8)) return Fl_205(0x5cb468);
            grade = (float)Atan(((double)hc - ha) / D(0x5cfeb8));
            if (!(grade < limit())) break;
        }
    }
    float ex = (float)(c * D(0x5cfed0) + mx);
    float ey = (float)(s * D(0x5cfed0) + my);
    F<float>(self, 0x1f1) = HeightAtTile(mx, my);
    float len = (float)(Distance2D(bx, by, ex, ey) * Fl_205(0x5c8700));
    if (len == D(0x5c8608)) return Fl_205(0x5c8598);
    return (float)Atan(((double)F<float>(self, 0x1f1) - h0) / len);
}
RT3_REPLACE(0x4a0d20, Track_RampGrade);

// 0x4a1410. Electrifies piece t (right side), with a catenary pole on every
// sixth piece n of a run.
RT3_NOINLINE void __stdcall Track_ElectrifyRunPiece(uint8_t* t, int32_t n) { Track_Electrify(t, 0, n % 6 == 0, 0, 1); }
RT3_REPLACE(0x4a1410, Track_ElectrifyRunPiece);

RT3_NOINLINE int32_t __stdcall Track_OnPlainLine(uint8_t* t) {
    if (F<int32_t>(t, 0x20e) != -1) return 0;
    if (!Obj_Node206(t) && !Obj_Node20a(t)) return 0;
    if (!WalkClear((uint8_t*)Obj_Node206(t), t, true)) return 0;
    return WalkClear((uint8_t*)Obj_Node20a(t), t, false);
}
RT3_REPLACE(0x4a1440, Track_OnPlainLine);

// 0x4a1550. Track network net: lays overpass piece t level at +0x1d9 (an
// open end takes the higher ground under its two corners), fits both
// neighbours to it and gives it the overpass model (variant by the year,
// 1920 on, 0x621a94 for a crossing, else 0x621a64), 6 / 12 below the deck.
// Returns 1.
RT3_NOINLINE int32_t __fastcall TrackNet_LayOverpass(Collection* net, int, uint8_t* t, int32_t, int32_t) {
    float v[4][3];
    Effect_GetVectors(t, 0, (uint32_t*)v[0], (uint32_t*)v[1], (uint32_t*)v[2], (uint32_t*)v[3]);
    auto ground = [](float x, float z) { return (float)Terrain_HeightAt(g_map, 0, x, z); };
    if (F<int32_t>(t, 0x206) == -1) {
        float a = ground(v[0][0], v[0][2]), b = ground(v[1][0], v[1][2]);
        float y = a > b ? a : b;
        F<float>(net, 0x1d9) = y;
        v[0][1] = y;
        v[1][1] = y;
    }
    if (F<int32_t>(t, 0x20a) == -1) {
        float a = ground(v[2][0], v[2][2]), b = ground(v[3][0], v[3][2]);
        F<float>(net, 0x1d9) = a > b ? a : b;
    }
    float y = F<float>(net, 0x1d9);
    v[1][1] = y;
    v[3][1] = y;
    v[2][1] = y;
    v[0][1] = y;
    ((void(__fastcall*)(uint8_t*, int, const float*, const float*, const float*, const float*, int32_t, int32_t, int32_t))(void*)&Track_Reshape)(
        t, 0, v[0], v[1], v[2], v[3], 1, 0, 0);
    uint32_t y0, y1, y2, y3;
    memcpy(&y0, &v[0][1], 4);
    memcpy(&y1, &v[1][1], 4);
    memcpy(&y2, &v[2][1], 4);
    memcpy(&y3, &v[3][1], 4);
    if (F<int32_t>(t, 0x206) != -1) TrackNet_FitNeighbour(net, t, F<uint32_t>(t, 0x206), y1, y0);
    if (F<int32_t>(t, 0x20a) != -1) TrackNet_FitNeighbour(net, t, F<uint32_t>(t, 0x20a), y3, y2);
    int32_t late = F<uint16_t>(g_world, 0xd) >= 0x780;
    float deck = (float)((double)F<float>(net, 0x1d9) - (t[0x218] == 4 ? Fl_207(0x5c89a4) : Fl_207(0x5c8788)));
    float h = Vehicle_ScreenHeading(t, 0, 1);
    uint32_t hb;
    memcpy(&hb, &h, 4);
    const char* model = ((const char* const*)(t[0x218] == 4 ? 0x621a94 : 0x621a64))[late];
    Track_SetMode(t, 0, 2, model, hb, 0, deck, 1, -1, late, 1, 0);
    F<int32_t>(net, 0x1e5) = 0;
    F<int32_t>(net, 0x1e9) = 0;
    return 1;
}
RT3_REPLACE(0x4a1550, TrackNet_LayOverpass);

// 0x4a1820. Track network net: lays the next piece t of an overpass run.
// While above the ground under t's centre (from 4 pieces on), or with
// `level`, the deck stays level at +0x1d9 (overpass model, which also ends
// the run: +0x1d9 = -1). Otherwise t ramps: flattened to +0x1d9, then (from
// 4 pieces on) the height steps by tan(+0x1dd) x 33.304 on the open side,
// stopping the ramp counter (+0x1e5 = -9999) once the start height +0x1f1
// is nearer than the previous step; t gets the ramp model of its pitch while
// still 60 above the ground (returning 1), else none (returning 0). The run
// counters +0x1e5 / +0x1e9 advance.
RT3_NOINLINE int32_t __fastcall TrackNet_LayOverpassRun(Collection* net, int, uint8_t* t, int32_t level) {
    float cx, cz;
    Obj_CenterCell(t, 0, &cx, &cz);
    float g = (float)Terrain_HeightAt(g_map, 0, (float)((double)cx * Fl_207(0x5c8700)), (float)((double)cz * Fl_207(0x5c8700)));
    auto reshape = (void(__fastcall*)(uint8_t*, int, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*, int32_t, int32_t, int32_t))(void*)&Track_Reshape;
    auto late = []() { return (int32_t)(F<uint16_t>(g_world, 0xd) >= 0x780); };
    if ((g < F<float>(net, 0x1d9) && F<int32_t>(net, 0x1e9) >= 4) || level) {
        uint32_t y = F<uint32_t>(net, 0x1d9);
        uint32_t v[4][3];
        Effect_GetVectors(t, 0, v[0], v[1], v[2], v[3]);
        if (F<int32_t>(t, 0x206) != -1) {
            v[0][1] = F<uint32_t>(net, 0x1d9);
            v[1][1] = F<uint32_t>(net, 0x1d9);
            v[2][1] = y;
            v[3][1] = y;
        }
        if (F<int32_t>(t, 0x20a) != -1) {
            v[2][1] = F<uint32_t>(net, 0x1d9);
            v[3][1] = F<uint32_t>(net, 0x1d9);
            v[0][1] = y;
            v[1][1] = y;
        }
        reshape(t, 0, v[0], v[1], v[2], v[3], 1, 0, 0);
        int32_t l = late();
        float deck = (float)((double)F<float>(net, 0x1d9) - (t[0x218] == 4 ? Fl_207(0x5c89a4) : Fl_207(0x5c8788)));
        float a = (float)((double)Vehicle_ScreenHeading(t, 0, 1) + Fl_207(0x5c8760));
        uint32_t ab;
        memcpy(&ab, &a, 4);
        const char* model = ((const char* const*)(t[0x218] == 4 ? 0x621a94 : 0x621a64))[l];
        Track_SetMode(t, 0, 2, model, ab, 0, deck, 1, -1, l, 1, 0);
        F<uint32_t>(net, 0x1d9) = 0xbf800000;
        return 1;
    }
    int32_t forward = 1;
    Track_SetFlatAndLift(t, 0, F<uint32_t>(net, 0x1d9), 1);
    if (F<int32_t>(net, 0x1e5) >= 4) {
        float before = F<float>(net, 0x1d9);
        double slope = ((double(__cdecl*)(double))0x5a3054)((double)F<float>(net, 0x1dd));
        F<float>(net, 0x1d9) = (float)(slope * D(0x5cfeb8) + F<float>(net, 0x1d9));
        uint32_t v[4][3];
        Effect_GetVectors(t, 0, v[0], v[1], v[2], v[3]);
        if (F<int32_t>(t, 0x206) == -1) {
            v[0][1] = F<uint32_t>(net, 0x1d9);
            v[1][1] = F<uint32_t>(net, 0x1d9);
            forward = 0;
        }
        if (F<int32_t>(t, 0x20a) == -1) {
            v[2][1] = F<uint32_t>(net, 0x1d9);
            v[3][1] = F<uint32_t>(net, 0x1d9);
            forward = 1;
        }
        reshape(t, 0, v[0], v[1], v[2], v[3], 1, 0, 0);
        auto fabs_ = (double(__cdecl*)(double))0x5a152e;
        double stepped = fabs_((double)F<float>(net, 0x1d9) - before);
        if (fabs_((double)F<float>(net, 0x1d9) - F<float>(net, 0x1f1)) < stepped) F<int32_t>(net, 0x1e5) = -9999;
    }
    float pitch = Vehicle_Pitch(t, 0, forward);
    t[0x59] &= 0xef;
    int32_t l = late();
    int32_t r = 0;
    if ((double)g < (double)F<float>(net, 0x1d9) + D(0x5c88a8)) {
        float y = (float)((double)F<float>(net, 0x1d9) - (t[0x218] == 4 ? Fl_207(0x5c89a4) : Fl_207(0x5c8788)));
        float a = (float)((double)Vehicle_ScreenHeading(t, 0, 1) + Fl_207(0x5c8760));
        uint32_t ab, pb;
        memcpy(&ab, &a, 4);
        memcpy(&pb, &pitch, 4);
        const char* model = ((const char* const*)(t[0x218] == 4 ? 0x621a9c : 0x621a6c))[l];
        Track_SetMode(t, 0, 2, model, ab, pb, y, 1, -1, l, 0, 0);
        r = 1;
    } else
        Track_SetMode(t, 0, 2, nullptr, 0, 0, 0.0f, 1, -1, l, 0, 0);
    ++F<int32_t>(net, 0x1e5);
    ++F<int32_t>(net, 0x1e9);
    return r;
}
RT3_REPLACE(0x4a1820, TrackNet_LayOverpassRun);

// 0x4a1bd0. Track builder b: lays piece t at tile (x, z) (x 92 world units):
// the build kind +0x1fd is 2 for a long span (`span` beyond 7.24 with
// `long`), else option +0x138; the height +0x1e1 is the ground (plus the
// clearance 0x5f3414 of the kind for a bridge, `bridge`) or at least `minY`.
// The piece is flattened to it, gets its pitch from the open ends, the
// model of its kind (0x621a74 for a crossing, else 0x621a44) and kind 1;
// the builder's heading +0x206 and run fields are reset. Returns 1.
RT3_NOINLINE int32_t __fastcall TrackBuilder_LayPiece(uint8_t* b, int, uint8_t* t, float x, float z, int32_t bridge, float pitch, int32_t, float minY, int32_t isLong,
                                         float span) {
    if (bridge) F<int32_t>(b, 0x1ed) = 1;
    if (isLong && (double)span > D(0x5cfed8)) b[0x1fd] = 2;
    else b[0x1fd] = g_app[0x138];
    float v[4][3];
    Effect_GetVectors(t, 0, (uint32_t*)v[3], (uint32_t*)v[2], (uint32_t*)v[1], (uint32_t*)v[0]);
    z = (float)((double)z * Fl_207(0x5c8700));
    x = (float)((double)x * Fl_207(0x5c8700));
    const float* clear = (const float*)0x5f3414;
    float y;
    if (bridge) {
        if (Terrain_HeightAt(g_map, 0, x, z) + clear[b[0x1fd]] < minY) y = minY;
        else y = (float)(Terrain_HeightAt(g_map, 0, x, z) + clear[b[0x1fd]]);
    } else {
        if (Terrain_HeightAt(g_map, 0, x, z) < minY) y = minY;
        else y = (float)Terrain_HeightAt(g_map, 0, x, z);
    }
    F<float>(b, 0x1e1) = y;
    v[2][1] = y;
    v[1][1] = y;
    v[3][1] = y;
    v[0][1] = y;
    ((void(__fastcall*)(uint8_t*, int, const float*, const float*, const float*, const float*, int32_t, int32_t, int32_t))(void*)&Track_Reshape)(
        t, 0, v[3], v[2], v[1], v[0], 1, 0, 0);
    if (F<int32_t>(t, 0x206) == -1) pitch = Vehicle_Pitch(t, 0, 1);
    if (F<int32_t>(t, 0x20a) == -1) pitch = Vehicle_Pitch(t, 0, 0);
    t[0x59] &= 0xef;
    float a = Vehicle_ScreenHeading(t, 0, 1);
    uint32_t ab, pb;
    memcpy(&ab, &a, 4);
    memcpy(&pb, &pitch, 4);
    F<uint32_t>(b, 0x206) = ab;
    const char* const* models = (const char* const*)(t[0x218] == 4 ? 0x621a74 : 0x621a44);
    Track_SetMode(t, 0, 1, models[b[0x1fd]], ab, pb, F<float>(b, 0x1e1), 1, b[0x1fd], -1, 1, 0);
    if (b[0x1fd] == 2) t[0x252] = 0xa;
    F<int32_t>(b, 0x1e5) = 0;
    F<int32_t>(b, 0x1e9) = 0;
    F<int32_t>(b, 0x1f9) = -1;
    F<int32_t>(b, 0x1f5) = -1;
    F<int32_t>(b, 0x1fe) = 0;
    return 1;
}
RT3_REPLACE(0x4a1bd0, TrackBuilder_LayPiece);

// 0x4a1df0. Track network net: lays the next piece t of a bridge run at
// deck height +0x1e1. The run ends here (returning 1, the deck reset to -1
// and +0x1ed cleared) when it is low over the ground ahead (0.724 tiles
// along the heading, within 10) or forced (+0x1ed / +0x1fe), neither cell
// is special (water), the step parity +0x1e5 fits the kind (+0x1fd: long
// spans end on even steps) and at least 4 pieces were laid: t gets the end
// model, or for a long span the last pier pair (+0x1f9 / +0x1f5) gets the
// span model by the step (+0x1e5 mod 10) and t none (no pier: 0, deck
// reset). Otherwise t is flattened onto the deck and gets a pier model on
// odd steps (long spans: on step 9 mod 10, remembering the pier), the run
// ends forced from 4 pieces on over low ground; the first piece fits its
// neighbours. The counters +0x1e5 / +0x1e9 advance, forcing the end past
// +0x202. 1 when t got a model.
RT3_NOINLINE int32_t __fastcall TrackNet_LayBridgeRun(Collection* net, int, uint8_t* t) {
    uint8_t* b = (uint8_t*)net;
    float cx, cz;
    Obj_CenterCell(t, 0, &cx, &cz);
    Terrain_HeightAt(g_map, 0, (float)((double)cx * Fl_207(0x5c8700)), (float)((double)cz * Fl_207(0x5c8700)));
    int32_t water = Terrain_IsSpecial(g_map, 0, Ftol(cx), Ftol(cz));
    float h = 0;
    if (F<int32_t>(t, 0x206) == -1) h = Vehicle_ScreenHeading(t, 0, 0);
    else if (F<int32_t>(t, 0x20a) == -1) h = Vehicle_ScreenHeading(t, 0, 1);
    float nx = (float)((double)cx - ((double(__cdecl*)(double))0x5a13e4)((double)h) * D(0x5d0020));
    float nz = (float)((double)cz - ((double(__cdecl*)(double))0x5a1494)((double)h) * D(0x5d0020));
    float g2 = (float)Terrain_HeightAt(g_map, 0, (float)((double)nx * Fl_207(0x5c8700)), (float)((double)nz * Fl_207(0x5c8700)));
    int32_t water2 = Terrain_IsSpecial(g_map, 0, Ftol(nx), Ftol(nz));
    double low = (double)g2 + D(0x5c85f0);
    int32_t shortKind = b[0x1fd] != 2;
    auto reshape = (void(__fastcall*)(uint8_t*, int, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*, int32_t, int32_t, int32_t))(void*)&Track_Reshape;
    auto heading = [&](uint8_t* p, uint32_t angleAt) {
        float a = (float)((double)(angleAt ? F<float>(b, 0x206) : Vehicle_ScreenHeading(p, 0, 1)) + Fl_207(0x5c8760));
        uint32_t ab;
        memcpy(&ab, &a, 4);
        return ab;
    };
    if (((double)F<float>(b, 0x1e1) < low || F<int32_t>(b, 0x1ed) || F<int32_t>(b, 0x1fe)) && !water && !water2 && F<int32_t>(b, 0x1e5) % 2 == shortKind &&
        F<int32_t>(b, 0x1e5) >= 4) {
        uint32_t v[4][3];
        Effect_GetVectors(t, 0, v[0], v[1], v[2], v[3]);
        uint32_t y = F<uint32_t>(b, 0x1e1);
        v[1][1] = y;
        v[2][1] = y;
        v[0][1] = y;
        v[3][1] = y;
        reshape(t, 0, v[0], v[1], v[2], v[3], 1, 0, 0);
        t[0x59] &= 0xef;
        if (b[0x1fd] != 2) {
            const char* const* models = (const char* const*)(t[0x218] == 4 ? 0x621a74 : 0x621a44);
            uint32_t a = heading(t, 0);
            Track_SetMode(t, 0, 1, models[b[0x1fd]], a, 0, F<float>(b, 0x1e1), 1, b[0x1fd], -1, 1, 0);
        } else {
            if (F<int32_t>(b, 0x1f9) == -1) {
                F<uint32_t>(b, 0x1e1) = 0xbf800000;
                return 0;
            }
            uint8_t* P = (uint8_t*)Collection_Get(net, 0, F<uint32_t>(b, 0x1f9));
            uint8_t* Q = (uint8_t*)Collection_Get(net, 0, F<uint32_t>(b, 0x1f5));
            int32_t m = F<int32_t>(b, 0x1e5) % 10;
            char model[0x20];
            uint8_t* span = P;
            if (m < 8) {
                ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)P)[6])(P, 0, 0);
                ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)Q)[6])(Q, 0, 0);
                uint32_t src;
                uint8_t len;
                if (m < 2) src = P[0x218] == 4 ? 0x5d0000 : 0x5cffe0, len = 0xc;
                else if (m < 4) src = P[0x218] == 4 ? 0x5cffc0 : 0x5cffa0, len = 0xe;
                else if (m < 6) src = P[0x218] == 4 ? 0x5cff80 : 0x5cff60, len = 0x10;
                else src = P[0x218] == 4 ? 0x5cff40 : 0x5cff20, len = 0x12;
                memcpy(model, (const void*)(uintptr_t)src, 0x1d);
                if (b[0x1fd] == 2) P[0x252] = len;
                Track_SetMode(Q, 0, 1, nullptr, 0, 0, 0.0f, 1, b[0x1fd], -1, 0, 0);
                Q[0x252] = 0;
            } else {
                ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)Q)[6])(Q, 0, 0);
                span = Q;
                memcpy(model, (const void*)(uintptr_t)(Q[0x218] == 4 ? 0x5cff00 : 0x5cfee0), 0x1e);
                if (b[0x1fd] == 2) Q[0x252] = 0xb;
            }
            uint32_t a = heading(span, 1);
            Track_SetMode(span, 0, 1, model, a, 0, F<float>(b, 0x1e1), 1, b[0x1fd], -1, 1, 0);
            Track_SetMode(t, 0, 1, nullptr, 0, 0, 0.0f, 1, b[0x1fd], -1, 0, 0);
            t[0x252] = 0;
        }
        F<uint32_t>(b, 0x1e1) = 0xbf800000;
        F<int32_t>(b, 0x1ed) = 0;
        return 1;
    }
    if (((double)F<float>(b, 0x1e1) < low || F<int32_t>(b, 0x1ed)) && !water && F<int32_t>(b, 0x1e5) >= 4) F<int32_t>(b, 0x1fe) = 1;
    Track_SetFlatAndLift(t, 0, F<uint32_t>(b, 0x1e1), 1);
    int32_t r = 0;
    if (b[0x1fd] != 2) {
        if (F<int32_t>(b, 0x1e5) % 2 == 1) {
            const char* const* models = (const char* const*)(t[0x218] == 4 ? 0x621a84 : 0x621a54);
            uint32_t a = heading(t, 0);
            Track_SetMode(t, 0, 1, models[b[0x1fd]], a, 0, F<float>(b, 0x1e1), 1, b[0x1fd], -1, 0, 0);
            r = 1;
        } else
            Track_SetMode(t, 0, 1, nullptr, 0, 0, 0.0f, 1, b[0x1fd], -1, 0, 0);
    } else if (F<int32_t>(b, 0x1e5) % 10 != 9)
        Track_SetMode(t, 0, 1, nullptr, 0, 0, 0.0f, 1, 2, -1, 0, 0);
    else {
        uint32_t a = heading(t, 1);
        const char* model = *(const char* const*)(uintptr_t)(t[0x218] == 4 ? 0x621a8c : 0x621a5c);
        Track_SetMode(t, 0, 1, model, a, 0, F<float>(b, 0x1e1), 1, 2, -1, 0, 0);
        r = 1;
        if (b[0x1fd] == 2) t[0x252] = 0xa;
        F<int32_t>(b, 0x1f9) = F<int32_t>(b, 0x1f5);
        F<int32_t>(b, 0x1f5) = F<int32_t>(t, 0x202);
    }
    if (!F<int32_t>(b, 0x1e5)) {
        uint32_t v[4][3];
        Effect_GetVectors(t, 0, v[0], v[1], v[2], v[3]);
        if (F<int32_t>(t, 0x206) != -1) TrackNet_FitNeighbour(net, t, F<uint32_t>(t, 0x206), v[1][1], v[0][1]);
        if (F<int32_t>(t, 0x20a) != -1) TrackNet_FitNeighbour(net, t, F<uint32_t>(t, 0x20a), v[3][1], v[2][1]);
    }
    t[0x59] &= 0xef;
    ++F<int32_t>(b, 0x1e5);
    ++F<int32_t>(b, 0x1e9);
    if (F<int32_t>(b, 0x1e5) > F<int32_t>(b, 0x202)) F<int32_t>(b, 0x1fe) = 1;
    return r;
}
RT3_REPLACE(0x4a1df0, TrackNet_LayBridgeRun);

// 0x4a2640. Walks from (x0, y0) along `angle` in small steps toward (x1, y1) while
// it gets closer (company `company` must be allowed in both end regions; the walk
// fails off the map): tracks the highest ground (*peak) and whether it crosses
// water (*wet). The first dry, ordinary point below `minH` is the landing; failing
// that, a point rising to the start height that is not special ends the walk.
// Water crossings need 1895+. *dist gets the distance to the chosen point.
RT3_NOINLINE int32_t __fastcall Route_ProbeLine(uint8_t* self, int, float x0, float y0, float angle, float x1, float y1,
                                   uint32_t company, int32_t minH, float* peak, int32_t* wet, float* dist) {
    uint8_t* m = g_map;
    int32_t* ra = (int32_t*)Map_RegionRecordAt(m, 0, x0, y0);
    int32_t* rb = (int32_t*)Map_RegionRecordAt(g_map, 0, x1, y1);
    Collection* companies = g_companies;
    if (ra != rb && Collection_IsValid(companies, 0, company)) {
        if (!Company_EntryFlag(Collection_Get(companies, 0, company), 0, *ra)) return 0;
        if (!Company_EntryFlag(Collection_Get(companies, 0, company), 0, *rb)) return 0;
    }
    auto map = []() { return g_map; };
    float cx = x0, cy = y0;
    float h0 = (float)Terrain_HeightAt(map(), 0, (float)((double)x0 * Fl_198(0x5c8700)), (float)((double)y0 * Fl_198(0x5c8700)));
    float d1 = (float)DistanceSq2D(x0, y0, x1, y1);
    float savedPeak = *peak;
    *(uint32_t*)peak = 0xc7c34f80;
    *wet = 0;
    int32_t special = 0, landed = 0;
    F<int32_t>(self, 0x202) = 0;
    float lx = x0, ly = y0;
    if (!(Fl_198(0x5c8598) < d1)) return 0;
    float d2;
    for (;;) {
        d1 = (float)DistanceSq2D(cx, cy, x1, y1);
        cx = (float)((double)cx - ExeCos(angle) * D(0x5cfec8));
        double sy = cy;
        double sn = ExeSin(angle) * D(0x5cfec8);
        ++F<int32_t>(self, 0x202);
        cy = (float)(sy - sn);
        d2 = (float)DistanceSq2D(cx, cy, x1, y1);
        if (d2 < D(0x5d0030)) {
            cx = x1;
            cy = y1;
        }
        if (d2 < d1) {
            if (cx < Fl_198(0x5c8598) || cx > (float)F<int32_t>(map(), 0x2155)) return 0;
            if (cy < Fl_198(0x5c8598) || cy > (float)F<int32_t>(map(), 0x2159)) return 0;
            float h = (float)Terrain_HeightAt(map(), 0, (float)((double)cx * Fl_198(0x5c8700)), (float)((double)cy * Fl_198(0x5c8700)));
            if (h > *peak) *peak = h;
            bool check;
            if (!*wet && Terrain_KindIs1Or3to5(map(), 0, Ftol(cx), Ftol(cy))) {
                *wet = 1;
                special = 1;
                check = true;
            } else if (special) check = true;
            else if (Terrain_IsSpecial(map(), 0, Ftol(cx), Ftol(cy))) {
                special = 1;
                check = true;
            } else check = false;
            if (check && !landed && h < (float)minH) {
                int32_t ix = Ftol(cx), iy = Ftol(cy);
                if (!Terrain_KindIs1Or3to5(map(), 0, ix, iy) && !Terrain_IsSpecial(map(), 0, ix, iy)) {
                    landed = 1;
                    lx = cx;
                    ly = cy;
                    savedPeak = *peak;
                }
            }
            if (h >= h0 && !Terrain_IsSpecial(map(), 0, Ftol(cx), Ftol(cy))) {
                if (*wet && F<uint16_t>(g_world, 0xd) < 0x767) return 0;
                float d = (float)Distance2D(x0, y0, cx, cy);
                *dist = d;
                return d < D(0x5d0028) ? 0 : 1;
            }
        }
        if (!(d2 < d1)) break;
    }
    if (!landed) return 0;
    if (*wet && F<uint16_t>(g_world, 0xd) < 0x767) return 0;
    *peak = savedPeak;
    *dist = (float)Distance2D(x0, y0, lx, ly);
    return 1;
}
RT3_REPLACE(0x4a2640, Route_ProbeLine);

// 0x4a2aa0. Levels track piece t's ends: each end's two corner heights become the
// higher one (the second corner's on a tie or NaN), the piece is re-set; then each
// end linked to a non-plain neighbour (+0x226) takes that neighbour's heights at
// the shared end (matched by which of its links points back) and is re-set again
// with neighbour matching.
RT3_NOINLINE void __fastcall Track_LevelEnds(Collection* c, int, uint8_t* t) {
    uint32_t v[4][3];
    Effect_GetVectors(t, 0, v[0], v[1], v[2], v[3]);
    auto level = [](uint32_t& a, uint32_t& b) {
        if (*(float*)&a <= *(float*)&b || *(float*)&a != *(float*)&a || *(float*)&b != *(float*)&b) a = b;
        else b = a;
    };
    level(v[0][1], v[1][1]);
    level(v[2][1], v[3][1]);
    Track_Reshape_Impl(t, 0, v[0], v[1], v[2], v[3], 0, 0, 0);
    int32_t id = F<int32_t>(t, 0x202);
    if (F<int32_t>(t, 0x206) != -1) {
        uint8_t* n = (uint8_t*)Collection_Get(c, 0, F<uint32_t>(t, 0x206));
        if (F<int32_t>(n, 0x226)) {
            uint32_t m[4][3];
            Effect_GetVectors(n, 0, m[0], m[1], m[2], m[3]);
            id = F<int32_t>(t, 0x202);
            if (F<int32_t>(n, 0x206) == id) {
                v[0][1] = m[1][1];
                v[1][1] = m[0][1];
            } else if (F<int32_t>(n, 0x20a) == id) {
                v[0][1] = m[3][1];
                v[1][1] = m[2][1];
            }
            Track_Reshape_Impl(t, 0, v[0], v[1], v[2], v[3], 1, 0, 0);
        }
    }
    if (F<int32_t>(t, 0x20a) != -1) {
        uint8_t* n = (uint8_t*)Collection_Get(c, 0, F<uint32_t>(t, 0x20a));
        if (F<int32_t>(n, 0x226)) {
            uint32_t m[4][3];
            Effect_GetVectors(n, 0, m[0], m[1], m[2], m[3]);
            id = F<int32_t>(t, 0x202);
            if (F<int32_t>(n, 0x206) == id) {
                v[2][1] = m[1][1];
                v[3][1] = m[0][1];
            } else if (F<int32_t>(n, 0x20a) == id) {
                v[2][1] = m[3][1];
                v[3][1] = m[2][1];
            }
            Track_Reshape_Impl(t, 0, v[0], v[1], v[2], v[3], 1, 0, 0);
        }
    }
}
RT3_REPLACE(0x4a2aa0, Track_LevelEnds);

// 0x4a2c70. Track network: spreads piece t's new state along the run of
// pieces of its kind (+0x226: 1 tunnel-like, 2 bridge, 3 overpass) in both
// directions, giving each its model for the state (tables 0x621a74 ..
// 0x621a9c by variant +0x219 / +0x251; bridges by span) and properties, and
// setting it to `state`; the first piece of another kind is re-resolved and
// ends that direction. A tunnel turning into a crossing (old 1, state 4)
// re-models t itself first, with end models where the run stops. QUIRK: the
// second direction does not skip pieces without a variant (-1), and the end
// count +0x28 carries over from the first.
RT3_NOINLINE void __fastcall Track_Propagate(Collection* nodes, int, uint8_t* t, uint8_t state, int32_t on, uint8_t old, int32_t notify) {
    if (!F<int32_t>(t, 0x226)) return;
    // QUIRK: the original only loads the run's kind inside the "old 1 / state 4"
    // block; otherwise the walk compares against stack residue. Modelled as a
    // value no piece kind has, so the walk resolves the first neighbour and stops.
    int32_t kind = old == 1 && state == 4 ? F<int32_t>(t, 0x226) : -1;
    const char* const* tA74 = (const char* const*)0x621a74;
    const char* const* tA84 = (const char* const*)0x621a84;
    const char* const* tA94 = (const char* const*)0x621a94;
    const char* const* tA9c = (const char* const*)0x621a9c;
    int32_t ends = 0;
    auto mark = [&](uint8_t* n) {
        if (Game_EffectiveVersion() >= 0x67 && notify) Track_SetKind(n, 0, 1);
    };
    if (old == 1 && state == 4) {
        int32_t v219 = (int8_t)t[0x219], v251 = (int8_t)t[0x251];
        uint8_t flag = (t[0x24c] >> 5) & 1;
        if (kind == 2) {
            if (flag) {
                Group_ReplaceModel(t, 0, tA94[v251]);
                uint8_t* n = (uint8_t*)Obj_Node206(t);
                if (n && F<int32_t>((uint8_t*)Obj_Node206(t), 0x226) != 2) Track_Resolve((uint8_t*)Obj_Node206(t), 0, 1);
                n = (uint8_t*)Obj_Node20a(t);
                if (n && F<int32_t>((uint8_t*)Obj_Node20a(t), 0x226) != 2) Track_Resolve((uint8_t*)Obj_Node20a(t), 0, 1);
            } else
                Group_ReplaceModel(t, 0, tA9c[v251]);
            Track_SetProps(t, 0, 2, 0xff, (uint8_t)v251, flag);
        } else if (kind == 3) {
            Group_ReplaceModel(t, 0, (const char*)0x5cb168);
            uint8_t* n = (uint8_t*)Obj_Node206(t);
            if (n && F<int32_t>((uint8_t*)Obj_Node206(t), 0x226) != 3) Group_ReplaceModel((uint8_t*)Obj_Node206(t), 0, (const char*)0x5cb138);
            n = (uint8_t*)Obj_Node20a(t);
            if (n && F<int32_t>((uint8_t*)Obj_Node20a(t), 0x226) != 3) Group_ReplaceModel((uint8_t*)Obj_Node20a(t), 0, (const char*)0x5cb138);
        } else if (kind == 1) {
            if (flag) {
                if (v219 != 2) Group_ReplaceModel(t, 0, tA74[v219]);
                else if (const char* m = Track_BridgeModel(t[0x252])) Group_ReplaceModel(t, 0, m);
                ends = 1;
                Track_EndModel(t, 1, (const char*)0x5cb138);
            }
            Track_SetProps(t, 0, 1, (uint8_t)v219, 0xff, flag);
            mark(t);
        }
    }
    for (int32_t dir = 0; dir < 2; ++dir) {
        auto step = [&](uint8_t* n) { return (uint8_t*)(dir ? Obj_Node20a(n) : Obj_Node206(n)); };
        uint8_t* prev = t;
        uint8_t* n = step(t);
        bool done = false;
        while (n && !done) {
            if (F<int32_t>(n, 0x226) != kind) {
                Track_Resolve(n, 0, 0);
                done = true;
            } else {
                int32_t v219 = (int8_t)n[0x219], v251 = (int8_t)n[0x251];
                uint8_t flag = (n[0x24c] >> 5) & 1;
                if (kind == 2) {
                    if (dir || v251 != -1) {
                        Group_ReplaceModel(n, 0, flag ? tA94[v251] : tA9c[v251]);
                        Track_SetProps(n, 0, kind, 0xff, (uint8_t)v251, flag);
                    }
                } else if (kind == 3) {
                    Group_ReplaceModel(n, 0, (const char*)0x5cb168);
                    uint8_t* m = (uint8_t*)Obj_Node206(n);
                    if (m && F<int32_t>((uint8_t*)Obj_Node206(n), 0x226) != 3) Group_ReplaceModel((uint8_t*)Obj_Node206(n), 0, (const char*)0x5cb138);
                    m = (uint8_t*)Obj_Node20a(n);
                    if (m && F<int32_t>((uint8_t*)Obj_Node20a(n), 0x226) != 3) Group_ReplaceModel((uint8_t*)Obj_Node20a(n), 0, (const char*)0x5cb138);
                } else if (kind == 1 && (dir || v219 != -1)) {
                    if (flag) {
                        if (v219 != 2) {
                            Group_ReplaceModel(n, 0, tA74[v219]);
                            Track_EndModel(n, 1, (const char*)0x5cb138);
                        } else {
                            if (const char* m = Track_BridgeModel(n[0x252])) Group_ReplaceModel(n, 0, m);
                            ++ends;
                        }
                    } else if (ends < 2)
                        Group_ReplaceModel(n, 0, tA84[v219]);
                    Track_SetProps(n, 0, kind, (uint8_t)v219, 0xff, flag);
                }
                Track_SetState(n, 0, state, on, notify, 0);
            }
            mark(n);
            if ((uint8_t*)Obj_Node206(n) != prev) {
                prev = n;
                n = (uint8_t*)Obj_Node206(n);
            } else if ((uint8_t*)Obj_Node20a(n) != prev) {
                prev = n;
                n = (uint8_t*)Obj_Node20a(n);
            } else
                done = true;
        }
    }
    F<int32_t>(g_world, 0x66a2) = 0;
}
RT3_REPLACE(0x4a2c70, Track_Propagate);

// 0x4a34e0. Track network: puts piece t's models back to their idle look
// (tables 0x621a44 / 0x621a54 by variant +0x219, overpasses 0x621a64 /
// 0x621a6c by +0x251) and re-resolves the neighbours of other kinds that
// joined its run.
RT3_NOINLINE void __fastcall TrackNet_IdleModels(Collection* net, int, uint8_t* t) {
    const char* const* tA44 = (const char* const*)0x621a44;
    const char* const* tA54 = (const char* const*)0x621a54;
    int32_t kind = F<int32_t>(t, 0x226);
    if (kind == 3) {
        Group_ReplaceModel(t, 0, (const char*)0x5cb180);
        uint8_t* n = (uint8_t*)Obj_Node206(t);
        if (n && F<int32_t>((uint8_t*)Obj_Node206(t), 0x226) != 3) {
            Group_ReplaceModel((uint8_t*)Obj_Node206(t), 0, (const char*)0x5cb150);
            Track_Resolve((uint8_t*)Obj_Node206(t), 0, 0);
        }
        n = (uint8_t*)Obj_Node20a(t);
        if (n && F<int32_t>((uint8_t*)Obj_Node20a(t), 0x226) != 3) {
            Group_ReplaceModel((uint8_t*)Obj_Node20a(t), 0, (const char*)0x5cb150);
            Track_Resolve((uint8_t*)Obj_Node20a(t), 0, 0);
        }
        return;
    }
    if (kind == 1) {
        int8_t v = (int8_t)t[0x219];
        if (!(t[0x24c] & 0x20)) {
            if (v != 2) Group_ReplaceModel(t, 0, tA54[v]);
            else if (Obj_Node206(t) && F<int32_t>((uint8_t*)Obj_Node206(t), 0x226) == 1 && Obj_Node20a(t) &&
                     F<int32_t>((uint8_t*)Obj_Node20a(t), 0x226) == 1)
                Group_ReplaceModel(t, 0, tA54[(int8_t)t[0x219]]);
            return;
        }
        if (v != 2) Group_ReplaceModel(t, 0, tA44[v]);
        else if (const char* m = Track_BridgeModelOff(t[0x252])) Group_ReplaceModel(t, 0, m);
        float h, b;
        uint8_t* n = (uint8_t*)Obj_Node20a(t);
        if (n && F<int32_t>((uint8_t*)Obj_Node20a(t), 0x226) != 1) {
            h = Vehicle_ScreenHeading(t, 0, 1);
            b = (float)Effect_Blend(t, 0, 0.0f);
            Track_Resolve((uint8_t*)Obj_Node20a(t), 0, 0);
        } else {
            h = (float)((double)Vehicle_ScreenHeading(t, 0, 1) + Fl_207(0x5c8760));
            b = (float)Effect_Blend(t, 0, 1.0f);
            n = (uint8_t*)Obj_Node206(t);
            if (n && F<int32_t>((uint8_t*)Obj_Node206(t), 0x226) != 1) Track_Resolve((uint8_t*)Obj_Node206(t), 0, 0);
        }
        if (t[0x219] != 2) {
            uint32_t hb;
            memcpy(&hb, &h, 4);
            Track_SetMode(t, 0, 0xa, (const char*)0x5cb150, hb, 0, b, 1, -1, -1, 0, 0);
        }
        return;
    }
    if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)t)[5])(t)) return;
    if (t[0x24c] & 0x20) {
        Group_ReplaceModel(t, 0, ((const char* const*)0x621a64)[(int8_t)t[0x251]]);
        uint8_t* n = (uint8_t*)Obj_Node206(t);
        if (n && F<int32_t>((uint8_t*)Obj_Node206(t), 0x226) != 2) Track_Resolve((uint8_t*)Obj_Node206(t), 0, 1);
        n = (uint8_t*)Obj_Node20a(t);
        if (n && F<int32_t>((uint8_t*)Obj_Node20a(t), 0x226) != 2) Track_Resolve((uint8_t*)Obj_Node20a(t), 0, 1);
    } else
        Group_ReplaceModel(t, 0, ((const char* const*)0x621a6c)[(int8_t)t[0x251]]);
}
RT3_REPLACE(0x4a34e0, TrackNet_IdleModels);

// 0x4a3810. Track network net: shapes piece t of a run being laid towards
// (x, y) (n: its index, negative for a preview). Within reach of the target
// on flat ground (+0x1d9 / +0x1e1 at the base height) the piece is only
// levelled (and powered every sixth piece in an electrified world). An
// overpass (+0x1d9) or bridge (+0x1e1) height lays that run instead. Else,
// by the piece's pitch against the detail options' limits (+0x140 / +0x13c)
// and special terrain: a steep end piece gets a ramp grade (+0x1dd) and an
// overpass; a bridge-worthy one is probed (Route_ProbeLine) and laid as a
// bridge piece; otherwise the piece is levelled. Laid pieces are powered in
// an electrified world. Whether laying worked.
// QUIRK: `peak` is tested by its bits (-0.0 counts as set); Route_ProbeLine
// writes its outputs over the peak / isLong / n argument slots, which then
// feed TrackBuilder_LayPiece (n left as its integer bits when not written).
RT3_NOINLINE int32_t __fastcall TrackNet_ShapePiece(Collection* net, int, uint8_t* t, float x, float y, int32_t n, uint32_t company, float peak, int32_t isLong) {
    uint8_t* b = (uint8_t*)net;
    float cx, cy;
    Obj_CenterCell(t, 0, &cx, &cy);
    float tx = x, ty = y;
    float dist = (float)Distance2D(cx, cy, x, y);
    int32_t neg = n < 0;
    int32_t result = 1, overpass = 0;
    if (!((double)dist > D(0x5d0038))) {
        if ((double)F<float>(b, 0x1d9) == D(0x5c9bf0) && (double)F<float>(b, 0x1e1) == D(0x5c9bf0)) {
            if (F<int32_t>(g_world, 0x4c74) && !neg) Track_ElectrifyRunPiece(t, n);
            Track_LevelEnds(net, 0, t);
            return 1;
        }
        overpass = 1;
    } else {
        float a = (float)AngleOf(x, y, cx, cy);
        tx = (float)(((double(__cdecl*)(double))0x5a1494)((double)a) * D(0x5d0020) + (double)x);
        ty = (float)(((double(__cdecl*)(double))0x5a13e4)((double)a) * D(0x5d0020) + (double)y);
    }
    auto power = [&] {
        if (F<int32_t>(g_world, 0x4c74) && !neg) Track_Electrify(t, 0, 0, 0, 1);
    };
    if (!((double)F<float>(b, 0x1d9) == D(0x5c9bf0))) {
        TrackNet_LayOverpassRun(net, 0, t, overpass);
        power();
        return result;
    }
    if (!((double)F<float>(b, 0x1e1) == D(0x5c9bf0))) {
        TrackNet_LayBridgeRun(net, 0, t);
        power();
        return result;
    }
    float pitch = 0, heading = 0, px, py;
    if (F<int32_t>(t, 0x206) == -1) {
        Obj_CenterCell(t, 0, &px, &py);
        pitch = Vehicle_Pitch(t, 0, 0);
        heading = Vehicle_ScreenHeading(t, 0, 0);
    } else if (F<int32_t>(t, 0x20a) == -1) {
        Obj_CenterCell(t, 0, &px, &py);
        pitch = Vehicle_Pitch(t, 0, 1);
        heading = Vehicle_ScreenHeading(t, 0, 1);
    } else
        Obj_CenterCell(t, 0, &px, &py);
    int32_t iy = Ftol((double)py);
    int32_t special = Terrain_IsSpecial(g_map, 0, Ftol((double)px), iy);
    uint8_t* opt = g_app;
    int32_t peakSet = *(int32_t*)&peak;
    if (pitch > ((const float*)0x5f3404)[F<int32_t>(opt, 0x140)] && !neg && peakSet) {
        if (Track_OnPlainLine(t)) {
            float grade = Track_RampGrade(b, 0, px, py, heading, tx, ty);
            F<float>(b, 0x1dd) = grade;
            if ((double)grade < D(0x5c8628)) result = TrackNet_LayOverpass(net, 0, t, *(int32_t*)&px, *(int32_t*)&py);
            else F<int32_t>(b, 0x1dd) = 0;
        }
        power();
        return result;
    }
    if (((pitch < ((const float*)0x5f33f4)[F<int32_t>(opt, 0x13c)]) || special) && !neg && peakSet) {
        if (Track_OnPlainLine(t)) {
            int32_t ok = Route_ProbeLine(b, 0, px, py, heading, x, y, company, isLong ? 0x3c : 0x28, &peak, &isLong, (float*)&n);
            peak = (float)((double)peak + D(0x5c85f0));
            if (!ok) {
                if (special) result = 0;
            } else if (!overpass)
                result = TrackBuilder_LayPiece(b, 0, t, px, py, special, pitch, *(int32_t*)&heading, peak, isLong, *(float*)&n);
        }
        power();
        return result;
    }
    if (F<int32_t>(g_world, 0x4c74) && !neg) Track_ElectrifyRunPiece(t, n);
    Track_LevelEnds(net, 0, t);
    return result;
}
RT3_REPLACE(0x4a3810, TrackNet_ShapePiece);

// 0x4a3ca0 / 0x4a3cd0. Save / restore +0x98 in +0xfe (with +0x90 -> +0x102).
void __fastcall Obj_Stash98(uint8_t* o) {
    F<int32_t>(o, 0xfe) = F<int32_t>(o, 0x98);
    F<int32_t>(o, 0x98) = 0;
    F<int32_t>(o, 0x102) = F<int32_t>(o, 0x90);
}
RT3_REPLACE(0x4a3ca0, Obj_Stash98);

int32_t __fastcall Obj_Unstash98(uint8_t* o) {
    F<int32_t>(o, 0x98) = F<int32_t>(o, 0xfe);
    F<int32_t>(o, 0xfe) = 0;
    F<int32_t>(o, 0x102) = 0;
    return 0;
}
RT3_REPLACE(0x4a3cd0, Obj_Unstash98);

RT3_NOINLINE int32_t __fastcall Obj_Has98(uint8_t* o) { return F<int32_t>(o, 0x98) ? 1 : 0; }
RT3_REPLACE(0x4a3cf0, Obj_Has98);

// 0x4a3d00. Pushes {id, a, b, next} onto the list at +0x98 and returns the id:
// the reserved id at +0x88, or else the next one from the counter +0x90.
RT3_NOINLINE int32_t __fastcall Obj_PushIdNode(void* o, int, int32_t a, int32_t b) {
    int32_t* n = (int32_t*)((void*(__cdecl*)(size_t))0x5a125d)(0x10);
    if (F<int32_t>(o, 0x88) == -1) {
        int32_t c = F<int32_t>(o, 0x90);
        F<int32_t>(o, 0x88) = c + 1;
        F<int32_t>(o, 0x90) = c + 2;
    }
    n[0] = F<int32_t>(o, 0x88);
    n[1] = a;
    n[2] = b;
    n[3] = F<int32_t>(o, 0x98);
    F<int32_t*>(o, 0x98) = n;
    F<int32_t>(o, 0x88) = -1;
    return n[0];
}
RT3_REPLACE(0x4a3d00, Obj_PushIdNode);

// 0x4a3d70. Frees the list +0x98 (next +0xc); +0x88 = -1.
void __fastcall Obj_FreeList98(uint8_t* o) {
    while (uint8_t* n = F<uint8_t*>(o, 0x98)) {
        F<uint8_t*>(o, 0x98) = F<uint8_t*>(n, 0xc);
        Free(n);
    }
    F<int32_t>(o, 0x88) = -1;
}
RT3_REPLACE(0x4a3d70, Obj_FreeList98);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4a3db0. Finish the track-build preview: snapshot its generation, finalize
// new type-1 nodes, refresh every node, free the pending list, notify the UI,
// and rebuild company/traffic data if the build was dirty.
RT3_NOINLINE void __fastcall TrackList_ResetBuildState(uint8_t* tracks) {
    F<uint32_t>(tracks, 0x94) = F<uint32_t>(tracks, 0x90);
    bool dirty = F<uint32_t>(tracks, 0xed) != 0;
    if (F<uint32_t>(hook::As<void*>(g_app), 0x94)) {
        int32_t count = hook::Orig<int32_t(__fastcall*)(void*)>(0x517cf0)(tracks);
        for (int32_t i = count - 1; i >= 0; --i) {
            uint32_t id = hook::Orig<uint32_t(__fastcall*)(void*, int, int32_t, uint32_t)>(0x518380)(tracks, 0, i, 0);
            uint8_t* node = (uint8_t*)hook::Orig<void*(__fastcall*)(void*, int, uint32_t)>(0x518140)(tracks, 0, id);
            if (F<uint32_t>(node, 0x240) > F<uint32_t>(tracks, 0x102) && node[0x216] == 1) {
                hook::Orig<void(__fastcall*)(void*, int, uint32_t)>(0x48b830)(node, 0, 2);
                dirty = true;
            }
            Method(0x489f40, node);
        }
    }
    for (void* p = F<void*>(tracks, 0x98); p; p = F<void*>(tracks, 0x98)) {
        F<void*>(tracks, 0x98) = F<void*>(p, 0xc);
        hook::Orig<void(__cdecl*)(void*)>(0x5a1145)(p);
    }
    F<uint32_t>(tracks, 0x88) = 0xffffffff;
    void* manager = hook::As<void*>(g_windowManager);
    if (manager && !F<uint32_t>(tracks, 0xed)) Notify(manager, 0xbb);
    if (dirty) {
        Method(0x4358d0, hook::As<void*>(g_world));
        // This callee is in a separate unfinished call cycle.
        hook::Orig<void(__fastcall*)(void*, int, uint32_t, uint32_t, uint32_t)>(0x432f40)(
            hook::As<void*>(g_events), 0, 6, 0, 0);
    }
    Method(0x4682c0, hook::As<void*>(g_6ada90));
}
RT3_REPLACE(0x4a3db0, TrackList_ResetBuildState);

// 0x4a3ec0. Track network net: undoes the newest build step (the undo stack
// +0x98: stamp, cost, company, next): every piece touched since the stamp
// (or `stamp`, unless -1) pops its undo records (up to 51 while building,
// +0xe9) and gets fresh ends where open; pieces newer than it are removed
// (settling their group first when they hung off older track). Then
// re-resolves pieces between differing neighbours, refunds the cost
// (company stats 0xf / 0xd, unless the scenario forbids), pops the stack
// (resetting the motion when empty) and refreshes the track window and
// colours. Nesting counter +0xe5.
RT3_NOINLINE void __fastcall TrackNet_Undo(Collection* net, int, uint32_t stamp) {
    uint8_t* b = (uint8_t*)net;
    if (!F<uint8_t*>(b, 0x98)) return;
    ++F<int32_t>(b, 0xe5);
    if (stamp == 0xffffffff) stamp = F<uint32_t>(F<uint8_t*>(b, 0x98), 0);
    for (int32_t i = Collection_Count(net) - 1; i >= 0; --i) {
        uint8_t* p = (uint8_t*)Collection_Get(net, 0, Collection_IdAt(net, 0, (uint32_t)i, 0));
        for (int32_t k = 0;;) {
            if (F<uint32_t>(p, 0x240) < stamp) break;
            if (!F<void*>(p, 0x244)) {
                // Newer than the stamp with no record left: the piece goes.
                if (p[0x216] == 2) break;
                if (F<int32_t>(p, 0x24d)) {
                    bool older = false;
                    if (F<int32_t>(p, 0x206) != -1 && F<uint32_t>(Collection_Get(net, 0, F<uint32_t>(p, 0x206)), 0x240) < stamp) older = true;
                    else if (F<int32_t>(p, 0x20a) != -1 && F<uint32_t>(Collection_Get(net, 0, F<uint32_t>(p, 0x20a)), 0x240) < stamp) older = true;
                    if (older && Tracks_SettleGroup(net, 0, F<uint32_t>(p, 0x202))) Node_ResetKind(p);
                }
                ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)net)[2])(net, 0, Collection_IdAt(net, 0, (uint32_t)i, 0));
                break;
            }
            Track_PopUndo(p);
            if (F<int32_t>(p, 0x206) == -1) {
                Node_ResetKind(p);
                Track_MakeEnd(p, 0, 1);
            }
            if (F<int32_t>(p, 0x20a) == -1) {
                Node_ResetKind(p);
                Track_MakeEnd(p, 0, 0);
            }
            if (!F<int32_t>(b, 0xe9) || F<uint32_t>(p, 0x240) < stamp) break;
            if (k++ >= 0x32) break;
        }
    }
    for (int32_t i = Collection_Count(net) - 1; i >= 0; --i) {
        uint8_t* p = (uint8_t*)Collection_Get(net, 0, Collection_IdAt(net, 0, (uint32_t)i, 0));
        if (p[0x218] == 2) {
            Track_Resolve(p, 0, 0);
            continue;
        }
        uint8_t* a = (uint8_t*)Obj_Node206(p);
        uint8_t* c = (uint8_t*)Obj_Node20a(p);
        if (a && c && a[0x218] != c[0x218]) Track_Resolve(p, 0, 0);
    }
    uint8_t* top = F<uint8_t*>(b, 0x98);
    if (!g_newGame[0x82] && F<uint32_t>(top, 8)) {
        void* co = Collection_Get(g_companies, 0, F<uint32_t>(top, 8));
        Company_AddStat(co, 0, 0xf, -F<float>(F<uint8_t*>(b, 0x98), 4));
        Company_AddStat(co, 0, 0xd, F<float>(F<uint8_t*>(b, 0x98), 4));
    }
    top = F<uint8_t*>(b, 0x98);
    F<uint8_t*>(b, 0x98) = F<uint8_t*>(top, 0xc);
    Free(top);
    if (!F<uint8_t*>(b, 0x98)) Train_ResetMotion(b);
    F<int32_t>(b, 0x88) = -1;
    if (hook::As<uint8_t**>(g_windowManager) && !F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xed))
        Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x808, 0, 0, 0);
    --F<int32_t>(b, 0xe5);
    Tracks_UpdateColor(net);
}
RT3_REPLACE(0x4a3ec0, TrackNet_Undo);

// 0x4a4150 / 0x4a41a0. Routes collection (vtbl 0x5d0040, inline 0x4a-byte records, 100 + 50).
RT3_NOINLINE Collection* __fastcall Routes_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5d0040;
    Collection_Init(c, 0, 1, 0x4a, 100, 50, 0, 0, 0);
    F<int32_t>(c, 0x88) = 0;
    F<int32_t>(c, 0x94) = 0;
    F<int32_t>(c, 0x98) = 0;
    F<int32_t>(c, 0x90) = 0;
    F<int32_t>(c, 0x8c) = 1;
    return c;
}
RT3_REPLACE(0x4a4150, Routes_Construct);

void __fastcall Routes_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5d0040;
    Collection_Destruct(c);
}
RT3_REPLACE(0x4a41a0, Routes_Destruct);

// 0x4a41b0. Routes load: tag 0x396d, records (each upgraded by 0x495020), tag 0x396e.
RT3_NOINLINE int32_t __fastcall Routes_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x396d, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    F<int32_t>(t, 0x8c) = 1;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        Route_Upgrade((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0, (int32_t)s);
    return StreamSkip4(s, 0x396e, 0, 0) + n;
}
RT3_REPLACE(0x4a41b0, Routes_Load);

// 0x4a4240. Routes save: tag 0x396d, records (with the empty hook 0x494ef0), tag 0x396e.
RT3_NOINLINE void __fastcall Routes_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x396d);
    Collection_Save(t, 0, s, recordSize);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        ((void(__thiscall*)(void*, Stream))0x494ef0)(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), s);
    StreamWriteU32(s, 0x396e);
}
RT3_REPLACE(0x4a4240, Routes_Save);

// 0x4a42b0. Adds a fresh route; returns its ID.
RT3_NOINLINE uint32_t __fastcall Routes_Add(Collection* t) {
    uint8_t local[0x4c] = {};
    Route_Init(local);
    F<int32_t>(t, 0x8c) = 1;
    uint32_t id = Collection_Allocate(t, 0, local, 0, 0, 0);
    Route_SetOwner((uint8_t*)Collection_Get(t, 0, id), 0, (int32_t)id);
    Nop494e80(local);
    return id;
}
RT3_REPLACE(0x4a42b0, Routes_Add);

void __fastcall Collection5d0048_Remove(Collection* c, int, uint32_t id) {
    if (!Collection_IsValid(c, 0, id)) return;
    Nop4950e0(Collection_Get(c, 0, id));
    Collection_Release(c, 0, id);
    F<int32_t>(c, 0x8c) = 1;
}
RT3_REPLACE(0x4a4300, Collection5d0048_Remove);

// 0x4a4340. Adjusts route id's reference count (+0x10); releases it at 0.
RT3_NOINLINE void __fastcall Routes_AddRef(Collection* t, int, uint32_t id, int32_t delta) {
    if (!Collection_IsValid(t, 0, id)) return;
    if (Route_Add10((uint8_t*)Collection_Get(t, 0, id), 0, delta) == 0) {
        void** vt = *(void***)t;
        ((void(__thiscall*)(void*, uint32_t))vt[2])(t, id);
    }
}
RT3_REPLACE(0x4a4340, Routes_AddRef);

// 0x4a4380. Walks block `id` from its start node (+4) towards its end node (+8) and
// splits it wherever the track changes kind (+0x44), owner (+0x22e) or region type:
// the old block ends before the change and a new block (owner set, start node
// there) takes the rest; nodes after a split are re-tagged (+0x212) with the new id.
RT3_NOINLINE void __fastcall Routes_SplitBlock(Collection* c, int, uint32_t id) {
    if (!Collection_IsValid(c, 0, id)) return;
    uint8_t* b = (uint8_t*)Collection_Get(c, 0, id);
    int32_t kind = b[0x1c], region = F<int32_t>(b, 0x18), owner = F<int32_t>(b, 0x1e);
    uint8_t* endP = Node(F<uint32_t>(b, 8));
    int32_t len = F<int32_t>(b, 0x10), endId = F<int32_t>(b, 8);
    int32_t start = F<int32_t>(b, 4);
    int32_t n = 0, total = 0, split = 0;
    uint8_t* p = Node(start);
    uint8_t* prev;
    if (Obj_Node206(p) && F<uint32_t>(Obj_Node206(p), 0x212) == id) {
        if (Obj_Node20a(p) && F<uint32_t>(Obj_Node20a(p), 0x212) == id) return;
        prev = (uint8_t*)Obj_Node20a(p);
    } else prev = (uint8_t*)Obj_Node206(p);
    if (prev == endP) return;
    do {
        if (total >= len || !p) break;
        bool same = !prev || ((p[0x44] != 0) == kind && F<uint16_t>(p, 0x22e) == owner && (int32_t)Obj_CenterRegionType(p) == region);
        if (!same) {
            F<int32_t>(b, 8) = F<int32_t>(prev, 0x202);
            F<int32_t>(b, 4) = start;
            F<int32_t>(b, 0x10) = n;
            F<int32_t>(b, 0x14) = n;
            uint8_t tmp[0x4c];
            Route_Init(tmp);
            F<int32_t>(c, 0x8c) = 1;
            uint32_t nid = Collection_Allocate(c, 0, tmp, 0, 0, 0);
            Route_SetOwner((uint8_t*)Collection_Get(c, 0, nid), 0, nid);
            Nop494e80(tmp);
            id = nid;
            b = (uint8_t*)Collection_Get(c, 0, nid);
            start = F<int32_t>(p, 0x202);
            Route_AddNode(b, 0, start);
            kind = b[0x1c];
            region = F<int32_t>(b, 0x18);
            owner = F<int32_t>(b, 0x1e);
            F<int32_t>(b, 4) = start;
            F<int32_t>(b, 8) = endId;
            n = 0;
            split = 1;
        }
        if (split) Node_Set212(p, 0, id);
        bool back = Obj_Node206(p) == prev;
        prev = p;
        p = (uint8_t*)(back ? Obj_Node20a(p) : Obj_Node206(p));
        ++total;
        ++n;
    } while (prev != endP);
    if (split) {
        F<int32_t>(b, 0x10) = n;
        F<int32_t>(b, 0x14) = n;
    }
}
RT3_REPLACE(0x4a4380, Routes_SplitBlock);

// 0x4a45f0. Route table t: splits route +0x212 of node P off at P (frozen
// +0x90 meanwhile, changed flag +0x8c). A route end node starts a new route
// of its own; otherwise a new route takes P and one neighbour (+0x206, or
// +0x20a by `side` and the one-way flags) and grows along that direction
// while the nodes still belong to the old route - for a loop route only up to
// half its length - and both routes' ends are fixed up. Empty routes are
// removed (via 0x6cfcb4). QUIRK: for a node without neighbours, or a route of
// a single node, the original passes the node pointer as the route ids it
// checks at the end.
RT3_NOINLINE void __fastcall Routes_SplitAtNode(Collection* t, int, uint8_t* P, int32_t side) {
    auto route = [&](uint32_t id) { return (uint8_t*)Collection_Get(t, 0, id); };
    F<int32_t>(t, 0x90) = 1;
    uint32_t R = 0, N = (uint32_t)(uintptr_t)P;
    do {
        if (F<int32_t>(P, 0x206) == -1 && F<int32_t>(P, 0x20a) == -1) {
            R = (uint32_t)(uintptr_t)P;
            break;
        }
        if (Game_EffectiveVersion() >= 100) Routes_SplitBlock(t, 0, F<uint32_t>(P, 0x212));
        R = F<uint32_t>(P, 0x212);
        if (F<int32_t>(route(R), 0x10) == 1) break;
        F<int32_t>(t, 0x8c) = 1;
        if (Node_IsRouteEnd(P)) {
            N = Routes_Add(t);
            Route_AddNode(route(N), 0, F<uint32_t>(P, 0x202));
            Node_Set212(P, 0, (int32_t)N);
            Routes_MoveLength(t, R, N, 1);
            if (F<int32_t>(P, 0x206) != -1 && F<uint32_t>(TrackNode(F<int32_t>(P, 0x206)), 0x212) == R)
                Route_ReplaceEnd(route(R), 0, F<int32_t>(P, 0x206), F<int32_t>(P, 0x202));
            else if (F<int32_t>(P, 0x20a) != -1 && F<uint32_t>(TrackNode(F<int32_t>(P, 0x20a)), 0x212) == R)
                Route_ReplaceEnd(route(R), 0, F<int32_t>(P, 0x20a), F<int32_t>(P, 0x202));
            break;
        }
        int32_t prev = F<int32_t>(P, 0x202);
        int32_t loop = 0, moved = 2;
        int32_t total = F<int32_t>(route(R), 0x10);
        uint8_t* A = TrackNode(F<int32_t>(route(R), 4));
        int32_t a206 = F<int32_t>(A, 0x206);
        if (a206 == F<int32_t>(route(R), 8)) loop = 1;
        else {
            int32_t a20a = F<int32_t>(A, 0x20a);
            if (a20a == F<int32_t>(route(R), 8)) loop = 1;
        }
        bool first = F<int32_t>(P, 0x20e) == -1 ? side != 0 : !(P[0x201] & 2);
        if (!first && F<int32_t>(P, 0x20a) == -1) first = true;
        uint32_t near = first ? 0x206 : 0x20a, far = first ? 0x20a : 0x206;
        N = Routes_Add(t);
        Route_AddNode(route(N), 0, F<uint32_t>(P, 0x202));
        Route_AddNode(route(N), 0, F<uint32_t>(P, near));
        uint8_t* Q = TrackNode(F<int32_t>(P, near));
        int32_t other = F<int32_t>(P, far);
        Node_Set212(P, 0, (int32_t)N);
        Node_Set212(Q, 0, (int32_t)N);
        Routes_MoveLength(t, R, N, 2);
        route(R);
        route(N);
        for (;;) {
            uint8_t* next;
            if (F<int32_t>(Q, 0x206) != -1 && F<int32_t>(Q, 0x206) != prev) {
                prev = F<int32_t>(Q, 0x202);
                next = TrackNode(F<int32_t>(Q, 0x206));
            } else if (F<int32_t>(Q, 0x20a) != -1 && F<int32_t>(Q, 0x20a) != prev) {
                prev = F<int32_t>(Q, 0x202);
                next = TrackNode(F<int32_t>(Q, 0x20a));
            } else
                break;
            if (F<uint32_t>(next, 0x212) != R) {
                Q = TrackNode(prev);
                break;
            }
            Q = next;
            Node_Set212(Q, 0, (int32_t)N);
            Routes_MoveLength(t, R, N, 1);
            Route_ReplaceEnd(route(N), 0, F<int32_t>(Q, 0x202), prev);
            ++moved;
            if (loop && moved > total / 2) break;
        }
        if (!loop) {
            if (Collection_IsValid(t, 0, R)) Route_ReplaceEnd(route(R), 0, other, F<int32_t>(Q, 0x202));
        } else {
            int32_t end0 = F<int32_t>(route(R), 4);
            Route_ReplaceEnd(route(R), 0, other, end0);
            int32_t x = F<int32_t>(Q, 0x206) != prev ? F<int32_t>(Q, 0x206) : F<int32_t>(Q, 0x20a);
            int32_t old = F<int32_t>(route(R), 8);
            Route_ReplaceEnd(route(R), 0, x, old);
        }
    } while (false);
    if (Collection_IsValid(t, 0, R) && !F<int32_t>(route(R), 0x10)) {
        Collection* g = g_routes;
        ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)g)[2])(g, 0, R);
    }
    if (Collection_IsValid(t, 0, R)) {
        uint8_t* r = route(R);
        TrackNode(F<int32_t>(r, 4));
        TrackNode(F<int32_t>(r, 8));
    }
    if (Collection_IsValid(t, 0, N)) {
        uint8_t* r = route(N);
        TrackNode(F<int32_t>(r, 4));
        TrackNode(F<int32_t>(r, 8));
    }
    F<int32_t>(t, 0x90) = 0;
}
RT3_REPLACE(0x4a45f0, Routes_SplitAtNode);

// 0x4a4c00. Whether two track pieces' routes (+0x212) can be merged: both
// open (+0x1d) with equal region type, owner and flag, and neither piece
// has +0x20e / +0x222 links.
int32_t __fastcall Routes_CanMerge(Collection* routes, int, uint8_t* a, uint8_t* b) {
    if (F<int32_t>(routes, 0x90)) return 0;
    if (!Collection_IsValid(routes, 0, F<uint32_t>(a, 0x212))) return 0;
    if (!Collection_IsValid(routes, 0, F<uint32_t>(b, 0x212))) return 0;
    uint8_t* ra = (uint8_t*)Collection_Get(routes, 0, F<uint32_t>(a, 0x212));
    uint8_t* rb = (uint8_t*)Collection_Get(routes, 0, F<uint32_t>(b, 0x212));
    if (!ra || !rb || !ra[0x1d] || !rb[0x1d]) return 0;
    if (F<int32_t>(rb, 0x18) != F<int32_t>(ra, 0x18) || F<int32_t>(rb, 0x1e) != F<int32_t>(ra, 0x1e)) return 0;
    if (rb[0x1c] != ra[0x1c]) return 0;
    if (F<int32_t>(a, 0x20e) != -1 || F<int32_t>(b, 0x20e) != -1) return 0;
    if (F<int32_t>(a, 0x222) != -1 || F<int32_t>(b, 0x222) != -1) return 0;
    return 1;
}
RT3_REPLACE(0x4a4c00, Routes_CanMerge);

// 0x4a4ce0. Route table t: merges route a into route b when both are valid
// (+0x1d) and agree in group, region type and flag: finds where they touch
// (a node next to an end of a whose neighbour is in b and is a route end;
// the first end prefers plain nodes, the second one-way ones (+0x201 bit 1)),
// moves a's nodes from there to its far end over to b (+0x212), moves the
// length (+0x14) and references, extends b's end and deletes a. 1 when merged
// or nothing to do (frozen tables +0x90 / nodes +0xf1, a == b).
RT3_NOINLINE int32_t __fastcall Routes_Merge(Collection* t, int, uint32_t a, uint32_t b) {
    if (F<int32_t>(t, 0x90) || F<int32_t>(hook::As<uint8_t*>(g_tracks), 0xf1)) return 1;
    if (b == a) return 1;
    uint8_t* A = (uint8_t*)Collection_Get(t, 0, a);
    uint8_t* B = (uint8_t*)Collection_Get(t, 0, b);
    if (!A[0x1d] || !B[0x1d] || F<int32_t>(A, 0x1e) != F<int32_t>(B, 0x1e) || F<int32_t>(A, 0x18) != F<int32_t>(B, 0x18) || A[0x1c] != B[0x1c]) return 0;
    auto edge = [&](uint8_t* p, uint32_t side, bool oneWay) {
        if (!Pair_Contains(B, 0, F<int32_t>(p, side))) return false;
        if (F<int32_t>(p, 0x20e) != -1 && ((p[0x201] & 2) != 0) != oneWay) return false;
        return F<int32_t>(TrackNode(F<int32_t>(p, side)), 0x20e) == -1;
    };
    uint8_t* P = TrackNode(F<int32_t>(A, 4));
    int32_t touch;
    if (edge(P, 0x206, false)) touch = F<int32_t>(P, 0x206);
    else if (edge(P, 0x20a, false)) touch = F<int32_t>(P, 0x20a);
    else {
        P = TrackNode(F<int32_t>(A, 8));
        if (edge(P, 0x206, true)) touch = F<int32_t>(P, 0x206);
        else if (edge(P, 0x20a, true)) touch = F<int32_t>(P, 0x20a);
        else return 0;
    }
    uint8_t* Q = TrackNode(touch);
    F<int32_t>(t, 0x8c) = 1;
    int32_t qid = F<int32_t>(Q, 0x202);
    int32_t prev = qid;
    int32_t farEnd = F<int32_t>(A, 4) != F<int32_t>(P, 0x202) ? F<int32_t>(A, 4) : F<int32_t>(A, 8);
    Node_Set212(P, 0, (int32_t)b);
    int32_t moved = 1, steps = 1;
    while (F<int32_t>(P, 0x202) != farEnd) {
        int32_t next = F<int32_t>(P, 0x206);
        if (next != -1 && next != prev) {
            prev = F<int32_t>(P, 0x202);
            P = TrackNode(next);
        } else {
            next = F<int32_t>(P, 0x20a);
            if (next != -1 && next != prev) {
                prev = F<int32_t>(P, 0x202);
                P = TrackNode(next);
            }
        }
        Node_Set212(P, 0, (int32_t)b);
        ++moved;
        ++steps;
    }
    F<int32_t>(A, 0x14) -= steps;
    Routes_AddRef(t, 0, a, -moved);
    F<int32_t>(B, 0x14) += steps;
    Routes_AddRef(t, 0, b, moved);
    Route_ReplaceEnd(B, 0, farEnd, qid);
    ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)t)[2])(t, 0, a);
    return 1;
}
RT3_REPLACE(0x4a4ce0, Routes_Merge);

// 0x4a4ff0. Splits track node id's chain off into a new route when its route
// (+0x212) no longer exists: renumbers the connected nodes of the old route both
// ways (+0x206 / +0x20a), sets the new route's node count (+0x14, refs) and ends.
RT3_NOINLINE int32_t __fastcall Routes_SplitAt(Collection* routes, int, uint32_t id) {
    if (!Collection_IsValid(g_tracks, 0, id)) return 0;
    uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, id);
    if (!n[0x216]) return 0;
    int32_t r = F<int32_t>(n, 0x212);
    if (Collection_IsValid(routes, 0, r)) return 0;
    int32_t newId = (int32_t)Routes_Add(routes);
    int32_t count = 1, count2 = 1;
    uint32_t endA = id, endB = id;
    Node_Set212(n, 0, newId);
    Reassign(n, (uint8_t*)Obj_Node206(n), r, newId, &count, &count2, &endA);
    Reassign(n, (uint8_t*)Obj_Node20a(n), r, newId, &count, &count2, &endB);
    F<int32_t>(Collection_Get(routes, 0, newId), 0x14) += count2;
    Routes_AddRef(routes, 0, newId, count);
    Route_AddNode((uint8_t*)Collection_Get(routes, 0, newId), 0, endA);
    Route_AddNode((uint8_t*)Collection_Get(routes, 0, newId), 0, endB);
    return 1;
}
RT3_REPLACE(0x4a4ff0, Routes_SplitAt);

// 0x4a51a0. Recomputes route rid's end (+8) and length (+0x10/+0x14) by
// walking its pieces from the start piece (+4), unless that start lies inside.
RT3_NOINLINE void __fastcall Routes_Recount(Collection* routes, int, int32_t rid) {
    if (!Collection_IsValid(routes, 0, rid)) return;
    uint8_t* r = (uint8_t*)Collection_Get(routes, 0, rid);
    uint32_t start = F<uint32_t>(r, 4);
    int32_t count = 1;
    uint8_t* cur = (uint8_t*)Collection_Get(g_tracks, 0, start);
    uint8_t* prev;
    uint8_t* e206 = (uint8_t*)Obj_Node206(cur);
    if (e206 && F<int32_t>(Obj_Node206(cur), 0x212) == rid) {
        uint8_t* e20a = (uint8_t*)Obj_Node20a(cur);
        if (e20a && F<int32_t>(Obj_Node20a(cur), 0x212) == rid) return;
        prev = (uint8_t*)Obj_Node20a(cur);
    } else {
        prev = (uint8_t*)Obj_Node206(cur);
    }
    for (;;) {
        uint8_t* next = (uint8_t*)Obj_Node206(cur) == prev ? (uint8_t*)Obj_Node20a(cur) : (uint8_t*)Obj_Node206(cur);
        prev = cur;
        if (!next || F<int32_t>(next, 0x212) != rid) break;
        ++count;
        cur = next;
    }
    F<uint32_t>(r, 4) = start;
    F<int32_t>(r, 8) = F<int32_t>(cur, 0x202);
    F<int32_t>(r, 0x10) = count;
    F<int32_t>(r, 0x14) = count;
}
RT3_REPLACE(0x4a51a0, Routes_Recount);

// 0x4a5280. Depth-first branch-and-bound route search over track routes (this =
// route collection; node table 0x6cfca8) from node `cur` to `target`, arriving from
// `prev` (-1 at the top, which sets up the visit costs 0x6cfcac and the bound
// 0x6cfcb0). Candidates are the far end of cur's route and cur's three links,
// tried nearest to the target first (squared distance, unusable ones last); a step
// costs 1, or the route length when it spans the route (remembered per route),
// doubled on another company's track (counted in *foreign). Options: only plain
// track unless `anyTrack`, only routes with +0x1c set (`needFlag`), only routes
// the company may use (`company`, Company_EntryFlag). Updates *cost / *foreign
// and returns the first node to take (top level) or cur. Quirk: the foreign-cost
// scratch is only rewritten on foreign track, so own-track candidates pass on the
// previous candidate's value.
int32_t __fastcall Route_Find(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev,
                              int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack) {
    Collection* nodes = g_tracks;
    uint32_t*& visit = g_6cfcac;
    uint32_t& bound = g_6cfcb0;
    int32_t best = -1;
    uint8_t* nd = (uint8_t*)Collection_Get(nodes, 0, cur);
    int32_t routeId = F<int32_t>(nd, 0x212);
    uint8_t* route = (uint8_t*)Collection_Get(routes, 0, routeId);
    uint32_t dummy = 0;
    if (!foreign) foreign = &dummy;
    uint32_t owner = company ? *company : 0;
    if (prev == -1) {
        *cost = 0;
        *foreign = 0;
        bound = 0xffffffff;
        uint32_t last = Collection_IdAt(routes, 0, Collection_Count(routes) - 1, 0);
        size_t bytes = (last + 1) * 4;
        visit = (uint32_t*)((void*(__cdecl*)(size_t))0x5a125d)(bytes);
        memset(visit, 0, bytes);
    }
    if (*cost > bound || (cur != target && ((!anyTrack && nd[0x216] != 2) || !Node_IsRouteEnd(nd)))) {
        *cost = 0xffffffff;
        return cur;
    }
    if (cur == target) {
        bound = *cost;
        return target;
    }
    uint32_t len = F<uint32_t>(route, 0x14);
    uint32_t newCost = 0, newForeign = 0, bestForeign = 0;
    if (visit[routeId] && visit[routeId] < *cost) {
        *cost = 0xffffffff;
        return cur;
    }
    float tx, ty;
    Obj_CenterCell(Collection_Get(nodes, 0, target), 0, &tx, &ty);
    struct Cand {
        int32_t id, spans;
        float dist;
    } c[4];
    c[0].id = F<int32_t>(route, 4);
    if (c[0].id == cur) c[0].id = F<int32_t>(route, 8);
    if (c[0].id != prev && c[0].id != cur) {
        float x, y;
        Obj_CenterCell(Collection_Get(nodes, 0, c[0].id), 0, &x, &y);
        c[0].spans = 1;
        c[0].dist = (float)DistanceSq2D(x, y, tx, ty);
    } else {
        c[0].id = -1;
    }
    for (int k = 1; k < 4; ++k) {
        c[k].id = F<int32_t>(nd, 0x206 + (k - 1) * 4);
        if (c[k].id == -1) continue;
        bool ok = false;
        if (c[k].id != prev) {
            uint8_t* n2 = (uint8_t*)Collection_Get(nodes, 0, c[k].id);
            uint8_t* r2 = F<int32_t>(n2, 0x212) == -1 ? nullptr : (uint8_t*)Collection_Get(routes, 0, F<int32_t>(n2, 0x212));
            ok = (anyTrack || n2[0x216] == 2) && route != r2 && (!needFlag || (r2 && r2[0x1c])) &&
                 (!company || (r2 && Company_EntryFlag(company, 0, F<int32_t>(r2, 0x18))));
            if (ok) {
                float x, y;
                Obj_CenterCell(n2, 0, &x, &y);
                c[k].spans = 0;
                c[k].dist = (float)DistanceSq2D(x, y, tx, ty);
            }
        }
        if (!ok) c[k].id = -1;
    }
    int order[4] = { 0, 1, 2, 3 };
    for (int k = 1; k < 4; ++k) {
        if (c[k].id == -1) continue;
        for (int p = k; p > 0; --p) {
            const Cand& before = c[order[p - 1]];
            if (before.id != -1 && !(c[k].dist < before.dist)) break;
            order[p] = order[p - 1];
            order[p - 1] = k;
        }
    }
    uint32_t bestCost = 0xffffffff;
    int32_t r = 0;
    for (int i = 0; i < 4; ++i) {
        const Cand& e = c[order[i]];
        if (e.id == -1) break;
        uint8_t* n2 = (uint8_t*)Collection_Get(nodes, 0, e.id);
        if (owner && F<uint16_t>(n2, 0x22e) != owner) {
            if (e.spans) {
                newForeign = *foreign + len;
                newCost = *cost + len * 2;
                visit[routeId] = newCost;
            } else {
                newForeign = *foreign + 1;
                newCost = *cost + 2;
            }
        } else if (e.spans) {
            newCost = *cost + len;
            visit[routeId] = newCost;
        } else {
            newCost = *cost + 1;
        }
        if (newCost < bound)
            r = Route_Find(routes, 0, e.id, target, &newCost, cur, needFlag, company, &newForeign, anyTrack);
        else
            newCost = 0xffffffff;
        if (newCost < bestCost) {
            bestCost = newCost;
            bestForeign = newForeign;
            best = r;
        }
    }
    *cost = bestCost;
    *foreign = bestForeign;
    int32_t next = best;
    if (best != -1 && Collection_IsValid(g_tracks, 0, best)) {
        int32_t ra = F<int32_t>(Collection_Get(g_tracks, 0, cur), 0x212);
        if (ra == F<int32_t>(Collection_Get(g_tracks, 0, best), 0x212)) {
            next = F<int32_t>(nd, 0x206);
            if (next == -1 || F<int32_t>(Collection_Get(g_tracks, 0, next), 0x212) != routeId)
                next = F<int32_t>(nd, 0x20a);
        }
    }
    if (prev != -1) return cur;
    Free(visit);
    return next;
}
RT3_REPLACE(0x4a5280, Route_Find);

// 0x4a5900. The plain variant of Route_Find (same arguments, visit costs and
// bound): tries the far end of cur's route, then its three links in slot order,
// with no distance ordering and no bound test before recursing. Spanning a route
// owned by someone else (route +0x1e, not the node owner) costs double. Quirks:
// when the far end is unusable the best node starts out as the *address* of
// `cost` (so a top-level search with no route returns a pointer as the node id),
// and the third link skips the same-route and dead-cost tests of the other two.
int32_t __fastcall Route_FindPlain(Collection* routes, int, int32_t cur, int32_t target, uint32_t* cost, int32_t prev,
                                   int32_t needFlag, uint32_t* company, uint32_t* foreign, int32_t anyTrack) {
    Collection* nodes = g_tracks;
    uint32_t& bound = g_6cfcb0;
    uint32_t dummy = 0;
    if (!foreign) foreign = &dummy;
    uint32_t owner = company ? *company : 0;
    if (prev == -1) {
        *cost = 0;
        *foreign = 0;
        bound = 0xffffffff;
        uint32_t last = Collection_IdAt(routes, 0, Collection_Count(routes) - 1, 0);
        size_t bytes = (last + 1) * 4;
        uint32_t* v = (uint32_t*)((void*(__cdecl*)(size_t))0x5a125d)(bytes);
        g_6cfcac = v;
        memset(v, 0, bytes);
    }
    if (*cost > bound) {
        *cost = 0xffffffff;
        return cur;
    }
    if (cur == target) {
        bound = *cost;
        return target;
    }
    uint8_t* nd = (uint8_t*)Collection_Get(nodes, 0, cur);
    if ((!anyTrack && nd[0x216] != 2) || !Node_IsRouteEnd(nd)) {
        *cost = 0xffffffff;
        return cur;
    }
    uint8_t* route = (uint8_t*)Collection_Get(routes, 0, F<int32_t>(nd, 0x212));
    uint32_t newCost = 0, newForeign = 0;
    uint32_t* visit = g_6cfcac;
    uint32_t seen = visit[F<int32_t>(nd, 0x212)];
    if (seen && seen < *cost) {
        *cost = 0xffffffff;
        return cur;
    }
    int32_t r;
    int32_t other = F<int32_t>(route, 4);
    if (other == cur) other = F<int32_t>(route, 8);
    if (other != prev && other != cur) {
        uint32_t len = F<uint32_t>(route, 0x14);
        if (owner && F<uint32_t>(route, 0x1e) != owner) {
            newCost = *cost + len * 2;
            newForeign = *foreign + len;
        } else {
            newCost = *cost + len;
        }
        visit[F<int32_t>(nd, 0x212)] = newCost;
        r = Route_FindPlain(routes, 0, other, target, &newCost, cur, needFlag, company, &newForeign, anyTrack);
    } else {
        r = (int32_t)(uintptr_t)cost;
        newCost = 0xffffffff;
    }
    uint32_t bestCost = newCost, bestForeign = newForeign;
    int32_t best = r;
    for (int k = 0; k < 3; ++k) {
        newCost = *cost;
        newForeign = *foreign;
        int32_t id = F<int32_t>(nd, 0x206 + k * 4);
        if (id == -1 || id == prev) {
            if (k == 2) break;
            newCost = 0xffffffff;
        } else {
            RoutePlain_Link(routes, nd, id, k < 2, cur, target, cost, needFlag, company, foreign, anyTrack, owner, newCost, newForeign, r);
        }
        if (newCost < bestCost) {
            bestCost = newCost;
            bestForeign = newForeign;
            best = r;
        }
    }
    *cost = bestCost;
    *foreign = bestForeign;
    if (prev != -1) return cur;
    int32_t next = best;
    if (Collection_IsValid(nodes, 0, cur) && Collection_IsValid(nodes, 0, best) &&
        F<int32_t>(Collection_Get(nodes, 0, cur), 0x212) == F<int32_t>(Collection_Get(nodes, 0, best), 0x212)) {
        next = F<int32_t>(Collection_Get(nodes, 0, cur), 0x206);
        if (next == -1 || F<int32_t>(Collection_Get(nodes, 0, cur), 0x212) != F<int32_t>(Collection_Get(nodes, 0, next), 0x212))
            next = F<int32_t>(Collection_Get(nodes, 0, cur), 0x20a);
    }
    Free(g_6cfcac);
    return next;
}
RT3_REPLACE(0x4a5900, Route_FindPlain);

// 0x4a5fc0. Numbers the connected components of the track blocks (+0xc) by flood
// fill through the nodes at both ends; empty blocks are handed to the manager at
// 0x6cfcb4, and nodes not yet in a block get one first.
void __fastcall Routes_NumberComponents(Collection* c) {
    F<int32_t>(c, 0x8c) = 0;
    if (!Collection_Count(c)) return;
    F<uint32_t*>(c, 0x94) = (uint32_t*)Malloc(Collection_Count(c) * 4 + 0x14);
    F<int32_t>(c, 0x98) = 0;
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        F<int32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0xc) = -1;
    int32_t comp = 1;
    Visit(c, Collection_IdAt(c, 0, 0, 0), comp);
    for (;;) {
        int32_t cur = -1;
        if (F<int32_t>(c, 0x98)) cur = F<int32_t*>(c, 0x94)[--F<int32_t>(c, 0x98)];
        if (cur == -1) {
            bool found = false;
            for (int32_t i = 0; i < Collection_Count(c); ++i) {
                if (F<int32_t>(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)), 0xc) != -1) continue;
                ++comp;
                Visit(c, Collection_IdAt(c, 0, i, 0), comp);
                found = true;
                break;
            }
            if (found) continue;
            Free(F<uint32_t*>(c, 0x94));
            return;
        }
        uint8_t* b = (uint8_t*)Collection_Get(c, 0, cur);
        if (F<int32_t>(b, 0x10) <= 0) {
            void* mgr = hook::As<void*>(g_routes);
            int32_t save = F<int32_t>(c, 0x8c);
            (*(void(__thiscall**)(void*, int32_t))(*(uint8_t**)mgr + 8))(mgr, cur);
            F<int32_t>(c, 0x8c) = save;
            continue;
        }
        for (int32_t j = 0; j < 2; ++j) {
            int32_t pid = j ? F<int32_t>(b, 8) : F<int32_t>(b, 4);
            if (pid == -1) continue;
            uint8_t* p = Node(pid);
            for (int32_t k = 0; k < 3; ++k) {
                int32_t nb = k == 0 ? F<int32_t>(p, 0x20e) : k == 1 ? F<int32_t>(p, 0x206) : F<int32_t>(p, 0x20a);
                if (nb == -1) continue;
                uint8_t* q = Node(nb);
                int32_t g = F<int32_t>(q, 0x212);
                if (g == cur) continue;
                if (Collection_IsValid(c, 0, g)) {
                    Visit(c, g, comp);
                    continue;
                }
                Routes_SplitAt(c, 0, nb);
                Visit(c, F<int32_t>(q, 0x212), comp);
                F<int32_t>(c, 0x8c) = 0;
            }
        }
    }
}
RT3_REPLACE(0x4a5fc0, Routes_NumberComponents);

}  // namespace rt3
