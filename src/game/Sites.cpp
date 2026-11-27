// Sites.cpp - RT3.exe 0x414000-0x41a000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/imports.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall TrackNet_RelayRect(Collection* net, int, uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Points_Bounds(void*, int, const uint32_t* p, int32_t* out);
int __fastcall ToUpper(char c);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
void __stdcall RotateNegated(float x, float y, float angle, float* outX, float* outY);
int32_t __fastcall Obj_GetCell(void* o, int, uint16_t* x, uint16_t* y);
void* __fastcall Engine_Get1C(void* e);
void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h);
uint8_t __fastcall Terrain_GetFlag(uint8_t* t, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellByte(uint8_t* t, int, int32_t x, int32_t y);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
int32_t __fastcall Map_NearFeature(uint8_t* m, int, int32_t x, int32_t y);
int32_t __stdcall Site_FootprintFlat(const int32_t* pos, int32_t shape);
void* __fastcall FindStaticRecord(void*, int, uint8_t* obj);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

const char* __fastcall GetString(int32_t id);
void __fastcall Fatal_Exit(const char* msg);
void __stdcall Map_PixelsPerTile(float* sx, float* sy);

uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW,
                                            int32_t* outH);

void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Sites_DestroyAll(Collection* c);
int32_t __fastcall Map_GatherMasked(void*, int, float* out, int32_t max);
int32_t __fastcall Game_EffectiveVersion();
int32_t __fastcall Track_RectIsFree(void* t, int, int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t allowSpecial);
int32_t __stdcall Site_ChooseShape(uint8_t* obj);
void __fastcall Terrain_SnapshotFlags();
struct Collection;
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void ResetGlobals62b308();
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
void DirtyCells_Add();
void DirtyCells_Apply();
void __fastcall Map_SetCoarseByte(void* m, int, int32_t x, int32_t y, uint8_t v);
void __fastcall List_DestroyThunk(MaxList* l);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
void __fastcall Type_RotatedCell(Collection* t, int, uint32_t id, float* outX, float* outY, float angle);
float __fastcall Vehicle_ScreenHeading(uint8_t* o, int, int32_t forward);
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
uint32_t __fastcall Map_RegionTypeAt(void* m, int, float x, float y);
void* __fastcall Obj_Ref3cc(void* o);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t Global6d07b4Set2();
int32_t IsMultiplayer();
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
int32_t __fastcall Terrain_IsSpecial(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Company_Get7680(void* c);

int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused, int32_t byDegree,
                                          int32_t skipLinked);

void __fastcall Map_ApplyTexturePatch(uint8_t* o, int, const uint32_t* pts, uint32_t* pixels);
void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
void __stdcall Site_ReleaseCells(const int32_t* rect, HashMap* hash, uint32_t* pool, uint8_t* site);
void __stdcall Map_SetFineBitList(const uint32_t* cells, int32_t on);
void __fastcall Map_MaskRect(void*, int, const int32_t* r, const uint32_t* values, uint8_t bit);
void __fastcall Sites_Free(uint8_t* mgr, int, uint8_t* site);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall Collection_Release(Collection* c, int, uint32_t id);
int32_t __cdecl Map_RectHasFlag1Impl(const int32_t* r);
int32_t __cdecl Map_BlockHasFlag2Impl(int32_t cx, int32_t cy);
int32_t __cdecl Map_NearestFlag2Impl(float* out, int32_t cx, int32_t cy);
void __fastcall Obj_Reground(uint8_t* o);
void __cdecl Site_LevelGroundImpl(int32_t id, const int32_t* r, float h, int32_t mode, int32_t refresh);
int32_t __fastcall Obj_TypeKind1Class3(void* o);
int32_t __fastcall Obj_TypeByteBa(void* o);
int32_t __fastcall Obj_TypeKindIs1(void* o);
int32_t __stdcall Site_CheckFlatness(int32_t allowSteep, int32_t ignoreFlags);
int32_t __fastcall Map_Contains(void* m, int, float x, float y);
int32_t __cdecl DirtyCells_AddImpl(int32_t a, int32_t x, int32_t y);
int32_t __cdecl Site_CheckPlacementImpl(uint8_t* type, uint32_t a, uint32_t b);

int32_t __fastcall Station_SnapToTrack(Collection* types, int, int32_t company, uint32_t type, float x, float y, float angle, float* outX, float* outY,
                                       float* outAngle, int32_t update, int32_t* outNode);

void __fastcall Obj_AttachmentPoint(Collection* c, int, int32_t mode, uint32_t id, float* x, float* z, float angle);
float __fastcall Obj_Mid1ed(uint8_t* o);
float __fastcall Vehicle_Tilt(uint8_t* o, int, int32_t absolute);
int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key);
void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Collection_FindByName4(Collection* c, int, const char* name);
int32_t __fastcall Site_CheckFootprint(uint8_t* types, int, uint32_t id, float x, float y, float angle, int32_t commit, int32_t trackOnly);
void __cdecl Footprint_StampImpl(uint8_t* o, int32_t owner);
uint32_t* __fastcall Map_ScorchTexture(uint8_t* o, int, const uint32_t* pts, int32_t listedOnly, int32_t innerOnly);
int32_t __cdecl DirtyCells_ApplyImpl(uint8_t* list, uint32_t** cursor);
uint32_t __fastcall Collection_Allocate(Collection* c, int, const void* data, uint32_t size, uint32_t key2, uint32_t key3);
int32_t __fastcall BuildingType_LoadFootprint(void*, int, uint8_t* t, const char* path);
void __fastcall BuildingTypes_RebuildPortsWarehouses(Collection* c);
void __fastcall TerrainTool_Flatten(uint8_t* t, int, int32_t x, int32_t y, int32_t r);
void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count);
uint8_t* __fastcall Sites_Create(uint8_t* mgr, int, const char* name, float x, float y, float angle, uint8_t* o, uint32_t* height);
RT3_NOINLINE int32_t __fastcall Game_EffectiveVersion();
RT3_NOINLINE int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
RT3_NOINLINE int32_t __fastcall DateToMinutes(const GameDate* d);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Models_DisplayName(const char* name, char* out);
int32_t __fastcall Models_DisplayName(const char* name, char* out);
ListNode* __fastcall List_Push(MaxList* l, int, int32_t key, int32_t value);
int32_t __fastcall List_Contains(MaxList* l, int, int32_t value);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
int32_t __fastcall DateToMinutes(const GameDate* d);

int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg,
                                  int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);

void __stdcall Map_ClearFineBits(const uint32_t* cells);
void __stdcall Map_SetFineBits(const uint32_t* cells);
uint32_t __fastcall Records_FindComposedName(Collection* c, int, int32_t a, int32_t b);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Terrain_SetCellCount(uint8_t* t, int, int32_t x, int32_t y, uint8_t n);
void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Building_MoveTo(uint8_t* o, int, float x, float z, int32_t lift, float level);
RT3_NOINLINE void __fastcall SList_PushFront(void** l, int, void** node);
void* __fastcall SList_PopFront(void** l);
RT3_NOINLINE void __stdcall Map_SetFineBitList(const uint32_t* cells, int32_t on);
RT3_NOINLINE void __stdcall Map_ClearFineBits(const uint32_t* cells);
RT3_NOINLINE void __stdcall Map_SetFineBits(const uint32_t* cells);
void __fastcall Points_Bounds(void*, int, const uint32_t* p, int32_t* out);
RT3_NOINLINE void __stdcall Map_SetHeightsInRect(const uint32_t* values, const int32_t* r);
void* __fastcall Pool_Alloc(int32_t* p);
void __fastcall Pool_Release(int32_t* p, int, void** node);
RT3_NOINLINE int32_t __fastcall Shape_Bounds(void* self, int, uint8_t* shape, int32_t* out);
void __stdcall SetGlobal5ee030(int32_t v);
RT3_NOINLINE int32_t GetQueueCount62ba64();
RT3_NOINLINE int32_t __fastcall BuildingType_LoadFootprint(void*, int, uint8_t* t, const char* path);
RT3_NOINLINE int32_t __fastcall Map_Contains(void* m, int, float x, float y);
int32_t __cdecl Map_BlockHasFlag2Impl(int32_t cx, int32_t cy);
void Map_BlockHasFlag2();
int32_t __cdecl Map_NearestFlag2Impl(float* out, int32_t cx, int32_t cy);
void Map_NearestFlag2();
void __fastcall Terrain_SnapshotFlags();
void Terrain_RestoreRect();
int32_t __fastcall Map_GatherMasked(void*, int, float* out, int32_t);
int32_t __stdcall Site_CheckFlatness(int32_t allowSteep, int32_t ignoreFlags);
RT3_NOINLINE void __cdecl Tracks_RefreshCellsImpl(const int32_t* r);
void Tracks_RefreshCells();
void __fastcall Tracks_RefreshArea(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __stdcall Map_PixelsPerTile(float* sx, float* sy);
int32_t __stdcall Site_FootprintFlat(const int32_t* pos, int32_t shape);
int32_t __stdcall Site_ChooseShape(uint8_t* obj);
void __cdecl Footprint_StampImpl(uint8_t* o, int32_t owner);
void Footprint_Stamp();
int32_t __cdecl DirtyCells_ApplyImpl(uint8_t* list, uint32_t** cursor);
void DirtyCells_Apply();
int32_t __cdecl Map_RectHasFlag1Impl(const int32_t* r);
void Map_RectHasFlag1();
RT3_NOINLINE void __cdecl Site_LevelGroundImpl(int32_t id, const int32_t* r, float h, int32_t mode, int32_t refresh);
void Site_LevelGround();
RT3_NOINLINE void __fastcall Sites_SpreadLevel(uint8_t* n, int32_t cx, float h, int32_t cy, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
RT3_NOINLINE void __stdcall Sites_LevelFrom(uint8_t* n, float h, int32_t cx, int32_t cy);
RT3_NOINLINE void __cdecl Sites_AddToCellHashImpl(HashMap* hash, const int32_t* r, uint8_t* p, uint8_t* s, int32_t noLevel);
void Sites_AddToCellHash();
void __stdcall Site_ReleaseCells(const int32_t* rect, HashMap* hash, uint32_t* pool, uint8_t* site);
void ResetGlobals62b308();
int32_t __cdecl Site_CheckPlacementImpl(uint8_t* type, uint32_t a, uint32_t b);
int32_t Site_CheckPlacement();
int32_t __fastcall Chain_AddToSortedList(uint8_t* o, const int32_t* mode);
void __fastcall Records_DeleteWithBuffers(Collection* c, int, uint32_t id);
void __fastcall Sites_DestroyAll(Collection* c);
RT3_NOINLINE void __fastcall Sites_SaveTable(uint8_t* c, int, Stream s);
uint32_t __fastcall Buildings_AddByTypeName(Collection* t, int, uint8_t* rec, uint32_t a, uint32_t b, uint32_t c);
int32_t __fastcall Collection_FindByName4(Collection* c, int, const char* name);
int32_t __fastcall Site_CheckFootprint(uint8_t* types, int, uint32_t id, float x, float y, float angle, int32_t commit, int32_t trackOnly);
int32_t __fastcall Table_CountNamedWithBit(Collection* t, int, const char* name, int32_t bit);
uint32_t __fastcall Table_NthNamedWithBit(Collection* t, int, const char* name, int32_t bit, int32_t nth);
void __stdcall RotateNegated(float x, float y, float angle, float* outX, float* outY);
RT3_NOINLINE void __fastcall Type_RotatedCell(Collection* t, int, uint32_t id, float* outX, float* outY, float angle);
RT3_NOINLINE void __fastcall Obj_AttachmentPoint(Collection* c, int, int32_t mode, uint32_t id, float* x, float* z, float angle);
RT3_NOINLINE void __fastcall Type_RotatedOffset(Collection* t, int, uint32_t id, int32_t i, float* outX, float* outY, float angle);
int32_t __fastcall Station_SnapToTrack(Collection* types, int, int32_t company, uint32_t type, float x, float y, float angle, float* outX, float* outY, float* outAngle, int32_t update, int32_t* outNode);
RT3_NOINLINE uint32_t* __stdcall Map_ExtractTextureRect(const int32_t* in, int32_t* out);
uint32_t* __fastcall Map_ScorchTexture(uint8_t* o, int, const uint32_t* pts, int32_t listedOnly, int32_t innerOnly);
void __fastcall Map_ApplyTexturePatch(uint8_t* o, int, const uint32_t* pts, uint32_t* pixels);
void __fastcall Map_MaskRect(void*, int, const int32_t* r, const uint32_t* values, uint8_t bit);
RT3_NOINLINE int32_t __fastcall Sites_PrepareGround(uint8_t* mgr, int, uint32_t id, int32_t level, int32_t mask, int32_t force, uint32_t* height);
RT3_NOINLINE uint8_t* __fastcall Sites_Restore(uint8_t* mgr, int, uint32_t id, uint8_t* o, int32_t);
RT3_NOINLINE uint8_t* __fastcall Sites_Create(uint8_t* mgr, int, const char* name, float x, float y, float angle, uint8_t* o, uint32_t* height);
RT3_NOINLINE void __fastcall Sites_Free(uint8_t* mgr, int, uint8_t* s);
RT3_NOINLINE void __fastcall TerrainTool_Flatten(uint8_t* t, int, int32_t x, int32_t y, int32_t r);
RT3_NOINLINE void __fastcall Map_RefreshBuildingsAround(uint8_t* o, int, int32_t x, int32_t y, int32_t r);
int32_t __stdcall CellBuffer_Get(int32_t x, int32_t y);
RT3_NOINLINE void __fastcall BuildingTypes_RebuildPortsWarehouses(Collection* c);
RT3_NOINLINE uint32_t __fastcall Records_FindComposedName(Collection* c, int, int32_t a, int32_t b);
RT3_NOINLINE void __fastcall Sites_Destruct(Collection* c);
RT3_NOINLINE void __fastcall BuildingTypes_LoadFiles(Collection* t);
RT3_NOINLINE int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg, int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap);
RT3_NOINLINE void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Map_SmoothHeights(uint8_t* m, int, int32_t cx, int32_t cy, int32_t r, int32_t refresh);
RT3_NOINLINE void __fastcall Map_SetCellHeight(uint8_t* m, int, int32_t x, int32_t y, uint32_t v, int32_t refresh);
RT3_NOINLINE void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE int32_t __fastcall Grid_NearestNodeWithin(Collection* g, int, float px, float py, int32_t exclude, float radius, int32_t unused, int32_t byDegree, int32_t skipLinked);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);


static inline uint8_t* Map() { return g_map; }

static inline double D(uint32_t a) { return *(const double*)a; }

// Formats string id with one argument and exits with it.
static void FatalFormat(int32_t id, const char* arg) {
    char msg[0x12c];
    FormatText(msg, GetString(id), arg, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Fatal_Exit(msg);
}

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}


// The placement message 0x62b270 = string id.
static void Site_SetMessage(int32_t id) {
    const char* s = GetString(id);
    char* d = (char*)0x62b270;
    while ((*d++ = *s++) != 0) {}
}

static inline int32_t FtolD(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

namespace {
int32_t CallDirtyApply() {
    int32_t r;
    void* fn = (void*)&DirtyCells_Apply;
    __asm {
        xor eax, eax
        call fn
        mov r, eax
    }
    return r;
}
// The original passes the cell as (ECX = y, EDX = x).
void CallDirtyAdd(int32_t flag, int32_t inEcx, int32_t inEdx) {
    void* fn = (void*)&DirtyCells_Add;
    __asm {
        mov eax, flag
        mov ecx, inEcx
        mov edx, inEdx
        call fn
    }
}
int32_t CallCheckPlacement(uint8_t* rec, uint32_t a, uint32_t b) {
    int32_t r;
    void* fn = (void*)&Site_CheckPlacement;
    __asm {
        push b
        push a
        mov eax, rec
        call fn
        mov r, eax
    }
    return r;
}
inline int32_t Quarter(int32_t v) { return (v + (v < 0 ? 3 : 0)) >> 2; }
}


static inline uint8_t* World() { return g_world; }

static inline double Fl_205(uint32_t a) { return *(const float*)a; }



// Returns whether track node `id` is a plain piece usable for company `company`:
// no junction link and no other company's claim (+0x222).
static bool Station_PlainFor(uint8_t* n, int32_t company) {
    return F<int32_t>(n, 0x20e) == -1 && (F<int32_t>(n, 0x222) == -1 || F<int32_t>(n, 0x222) == company);
}

// The piece beyond neighbour `nb` (seen from `from`) must not be a junction that
// leads back into `nb` through its branch.
static bool Station_FarSideOk(uint8_t* farNode, int32_t nb, int32_t company) {
    if (F<int32_t>(farNode, 0x222) != -1 && F<int32_t>(farNode, 0x222) != company) return false;
    int32_t j = F<int32_t>(farNode, 0x20e);
    if (j == -1) return true;
    if (j == nb) return false;
    bool flipped = (farNode[0x201] >> 1) & 1;
    if (flipped && F<int32_t>(farNode, 0x206) == nb) return false;
    if (!flipped && F<int32_t>(farNode, 0x20a) == nb) return false;
    return true;
}

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

// 0x414ee0 takes its first source in EAX (Terrain_RestoreRect, regargs.cpp).
static void Terrain_RestoreRectAt(const uint8_t* src, const int32_t* r, const uint8_t* src2) {
    __asm {
        push src2
        push r
        mov eax, src
        mov ecx, 0x414ee0
        call ecx
    }
}

namespace {
uint32_t Message_Dialog(const char* text, uint32_t px) {
    return Dialog_RunMessage(text, 1, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

namespace {
uint32_t Message447(const char* text, int32_t type, uint32_t buttonText, uint32_t a8) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, buttonText, a8, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// Logs the display name of key name string `key` (message `msg`).
void Log_KeyName(int32_t key, int32_t msg) {
    char name[0x190], shown[0x190], text[0x190];
    FormatText(name, GetString(key), 0, 0, 0, 0, 0, 0, 0, 0);
    if (!Models_DisplayName(name, shown)) return;
    FormatText(text, GetString(msg), shown, 0, 0, 0, 0, 0, 0, 0);
    Log_Add(g_messageLog, 0, text, 4, 0, 0);
}
// The session's minute follows the next queued part (0 without one).
void Session_StampNextPart() {
    uint8_t* part = Net_CountMessagePart(g_session, 0, 1);
    uint8_t* s = g_session;
    F<int32_t>(s, 0x8f38) = part ? DateToMinutes((const GameDate*)part) : 0;
}
}

// 0x414ee0 body. Restores the terrain cells of rect { x0, x1, y0, y1 } (row by row)
// from saved bytes: flags (low nibble) and counts (high nibble) from src, the cell
// byte from src2; 0xff entries and cells outside the map are skipped. Marks the
// scene dirty.
static void __cdecl Terrain_RestoreRectImpl(const uint8_t* src, const int32_t* r, const uint8_t* src2) {
    for (int32_t y = r[2]; y <= r[3]; ++y)
        for (int32_t x = r[0]; x <= r[1]; ++x, ++src, ++src2) {
            uint8_t b = *src;
            uint8_t* m = g_map;
            if (b == 0xff || x < 0 || x >= F<int32_t>(m, 0x15d9) - 1 || y < 0 || y >= F<int32_t>(m, 0x15d5) - 1) continue;
            Terrain_SetFlag(m, 0, x, y, (uint8_t)(b & 0x0f));
            Terrain_SetCellByte(g_map, 0, x, y, *src2);
            Terrain_SetCellCount(g_map, 0, x, y, (uint8_t)(*src >> 4));
        }
    ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x374a] = 1;
}

// 0x414230 / 0x414250. Singly linked list {head, tail, count}: push / pop at the front.
RT3_NOINLINE void __fastcall SList_PushFront(void** l, int, void** node) {
    *node = l[0];
    ((int32_t*)l)[2]++;
    void* tail = l[1];
    l[0] = node;
    if (!tail) l[1] = node;
}
RT3_REPLACE(0x414230, SList_PushFront);

void* __fastcall SList_PopFront(void** l) {
    void** n = (void**)l[0];
    if (!n) return nullptr;
    l[0] = *n;
    if (n == (void**)l[1]) l[1] = *n;
    ((int32_t*)l)[2]--;
    return n;
}
RT3_REPLACE(0x414250, SList_PopFront);

// 0x414270. Sets fine bits (grid 2, with block refresh) for a -1-terminated list
// of packed cells (x | y << 16, 15 bits each).
RT3_NOINLINE void __stdcall Map_SetFineBitList(const uint32_t* cells, int32_t on) {
    for (; *cells != 0xffffffff; ++cells)
        Map_SetFineBit(g_map, 0, *cells & 0x7fff, (*cells >> 16) & 0x7fff, on, 2, 1);
}
RT3_REPLACE(0x414270, Map_SetFineBitList);

// 0x4142c0 / 0x4142d0. Clear / set the fine map bits of `cells`.
RT3_NOINLINE void __stdcall Map_ClearFineBits(const uint32_t* cells) { Map_SetFineBitList(cells, 0); }
RT3_REPLACE(0x4142c0, Map_ClearFineBits);

RT3_NOINLINE void __stdcall Map_SetFineBits(const uint32_t* cells) { Map_SetFineBitList(cells, 1); }
RT3_REPLACE(0x4142d0, Map_SetFineBits);

// 0x4142e0. Bounding box {minX, maxX, minY, maxY} of a -1-terminated list of
// packed points (x = low 15 bits, y = bits 16..30).
RT3_NOINLINE void __fastcall Points_Bounds(void*, int, const uint32_t* p, int32_t* out) {
    int32_t minX = 99999, minY = 99999, maxX = -99999, maxY = -99999;
    for (; *p != 0xffffffffu; ++p) {
        int32_t x = *p & 0x7fff, y = (*p >> 16) & 0x7fff;
        if (minX > x) minX = x;
        if (minY > y) minY = y;
        if (maxX < x) maxX = x;
        if (maxY < y) maxY = y;
    }
    out[3] = maxY;
    out[2] = minY;
    out[1] = maxX;
    out[0] = minX;
}
RT3_REPLACE(0x4142e0, Points_Bounds);

// 0x414350. Writes the heights `values` (row by row, 999999 = keep) over the
// cell rectangle r = { x0, x1, y0, y1 } of the map, then announces it.
RT3_NOINLINE void __stdcall Map_SetHeightsInRect(const uint32_t* values, const int32_t* r) {
    int32_t k = 0;
    for (int32_t y = r[2]; y <= r[3]; ++y)
        for (int32_t x = r[0]; x <= r[1]; ++x, ++k)
            if (values[k] != 0x497423f0) Map_SetCellHeight(g_map, 0, x, y, values[k], 0);
    Map_CellsChangedImpl(g_map, 0, r[0], r[2], r[1], r[3]);
}
RT3_REPLACE(0x414350, Map_SetHeightsInRect);

// 0x4143e0 / 0x414410. Node pool alloc / release (see core/containers.cpp).
void* __fastcall Pool_Alloc(int32_t* p) {
    if (p[3] == 0) return crt::operator_new(p[4]);
    void** f = (void**)p[0];
    void** n = (void**)f[0];
    if (!n) return nullptr;
    f[0] = *n;
    if (n == (void**)f[1]) f[1] = *n;
    ((int32_t*)f)[2]--;
    return n;
}
RT3_REPLACE(0x4143e0, Pool_Alloc);

void __fastcall Pool_Release(int32_t* p, int, void** node) {
    if (p[3] == 0 || (uint8_t*)node < (uint8_t*)p[1] || (uint8_t*)node >= (uint8_t*)p[2]) {
        crt::operator_delete(node);
        return;
    }
    void** f = (void**)p[0];
    *node = f[0];
    ((int32_t*)f)[2]++;
    void* tail = f[1];
    f[0] = node;
    if (!tail) f[1] = node;
}
RT3_REPLACE(0x414410, Pool_Release);

// 0x414450. Bounds of the point list at +0x10 of `shape`; always 1.
RT3_NOINLINE int32_t __fastcall Shape_Bounds(void* self, int, uint8_t* shape, int32_t* out) {
    Points_Bounds(self, 0, (const uint32_t*)(shape + 0x10), out);
    return 1;
}
RT3_REPLACE(0x414450, Shape_Bounds);

void __stdcall SetGlobal5ee030(int32_t v) { g_5ee030 = v; }
RT3_REPLACE(0x414470, SetGlobal5ee030);

RT3_NOINLINE int32_t GetQueueCount62ba64() { return g_62ba64; }
RT3_REPLACE(0x414480, GetQueueCount62ba64);

// 0x414490. Loads a building footprint file (`path`, "rb", exe CRT stdio) into `t`:
// version, name and 5 more strings, grid size (+0xb8 w, +0xb9 h, +0xba cell
// resolution), header fields, version-dependent extras (1001+ / 1003+ 12 slots /
// 1004+ name / flags), renaming ServiceTower / Maintenance (+0xcd = 0x81 / 0x9d);
// then decodes the nibble-packed occupancy (classes 1-4 per sub-cell) into four
// bitmasks (+0xcf..+0xdb, 32 bytes per cell; freed when unused, +0x177..). Returns
// 1 when the file opened.
RT3_NOINLINE int32_t __fastcall BuildingType_LoadFootprint(void*, int, uint8_t* t, const char* path) {
    FILE* f = ((FILE*)crt::fopen(path, (const char*)0x5c9678));
    int32_t ok = 0;
    if (f) {
        auto rd = [&](void* p, size_t n) { crt::fread(p, 1, n, f); };
        rd(t, 4);
        static const uint16_t texts[] = { 4, 0x22, 0x40, 0x5e, 0x7c, 0x9a };
        for (uint16_t o : texts) rd(t + o, 0x1e);
        rd(t + 0xb8, 1);
        rd(t + 0xb9, 1);
        rd(t + 0xba, 1);
        rd(t + 0xbb, 4);
        rd(t + 0xbf, 4);
        rd(t + 0xc3, 4);
        rd(t + 0xc7, 4);
        rd(t + 0xcb, 2);
        rd(t + 0xcd, 2);
        if (!crt::stricmp((const char*)t + 4, (const char*)0x5c871c)) F<uint16_t>(t, 0xcd) = 0x81;
        if (!crt::stricmp((const char*)t + 4, (const char*)0x5c8710)) F<uint16_t>(t, 0xcd) = 0x9d;
        int32_t size = t[0xb8] * t[0xb9] * t[0xba] * t[0xba] / 2;
        uint8_t* buf = (uint8_t*)crt::operator_new(size);
        rd(buf, size);
        if (F<int32_t>(t, 0) >= 0x3e9) {
            rd(t + 0xdf, 4);
            rd(t + 0xe3, 4);
            if (F<int32_t>(t, 0) >= 0x3eb)
                for (int32_t i = 0; i < 0xc; ++i) {
                    rd(t + 0xe7 + i, 1);
                    rd(t + 0xf3 + i * 2, 2);
                    rd(t + 0x10b + i * 2, 2);
                }
            if (F<int32_t>(t, 0) >= 0x3ec) rd(t + 0x123, 0x1e);
            rd(t + 0x141, 4);
            if (!F<int32_t>(t, 0x141)) F<int32_t>(t, 0x141) = 1;
            rd(t + 0x145, 0x2e);
        }
        crt::fclose(f);
        for (int32_t k = 0; k < 4; ++k) {
            uint32_t n = t[0xb8] * t[0xb9] * 32;
            uint8_t* m = (uint8_t*)crt::operator_new(n);
            F<uint8_t*>(t, 0xcf + k * 4) = m;
            memset(m, 0, n);
        }
        int32_t q = t[0xba] >> 4;
        int32_t state = 2, src = 0, dst = 0, bit = 7;
        uint8_t bytes[3];
        for (int32_t y = 0; y < t[0xb9]; ++y) {
            for (int32_t x = 0; x < t[0xb8]; ++x) {
                for (int32_t a = 0; a < 4; ++a) {
                    for (int32_t b = 0; b < 4; ++b) {
                        for (int32_t c = 0; c < 4; ++c) {
                            for (int32_t d = 0; d < 4; ++d) {
                                uint8_t cls[4] = {};
                                for (int32_t i = 0; i < q;) {
                                    bool high;
                                    if (state == 2) {
                                        int32_t half = t[0xba] >> 1;
                                        bytes[0] = buf[src];
                                        bytes[1] = buf[src + half];
                                        bytes[2] = buf[src + half * 2];
                                        ++src;
                                        high = true;
                                    } else if (state == 0) high = true;
                                    else if (state == 1) high = false;
                                    else continue;
                                    if (high) {
                                        for (int32_t k = 0; k < 3; ++k) {
                                            uint32_t v = (uint32_t)(bytes[k] >> 4) - 1;
                                            if (v <= 3) cls[v] = 1;
                                        }
                                        state = 1;
                                        ++i;
                                        if (i >= q) break;
                                    }
                                    for (int32_t k = 0; k < 3; ++k) {
                                        uint32_t v = (uint32_t)(bytes[k] & 0xf) - 1;
                                        if (v <= 3) cls[v] = 1;
                                    }
                                    state = 2;
                                    ++i;
                                }
                                for (int32_t k = 0; k < 4; ++k)
                                    if (cls[k]) {
                                        F<uint8_t*>(t, 0xcf + k * 4)[dst] |= (uint8_t)(1 << bit);
                                        F<int32_t>(t, 0x177 + k * 4) = 1;
                                    }
                                if (--bit < 0) {
                                    ++dst;
                                    bit = 7;
                                }
                            }
                            src += (t[0xba] * 3 - q * 4) / 2;
                        }
                        src += -((t[0xba] * 4 - 4) * q / 2);
                    }
                    src += t[0xba] * (q * 4 - 1) / 2;
                }
            }
        }
        crt::operator_delete(buf);
        F<uint32_t>(t, 0x1a3) = 0x3f800000;
        ok = 1;
    }
    for (int32_t k = 0; k < 4; ++k)
        if (!F<int32_t>(t, 0x177 + k * 4)) {
            crt::operator_delete(F<void*>(t, 0xcf + k * 4));
            F<void*>(t, 0xcf + k * 4) = nullptr;
        }
    return ok;
}
RT3_REPLACE(0x414490, BuildingType_LoadFootprint);

// 0x414bd0. (x, y) lies inside the map grid [0, w-1] x [0, h-1].
RT3_NOINLINE int32_t __fastcall Map_Contains(void* m, int, float x, float y) {
    const float zero = *(const float*)0x5c8598;
    if (x < zero) return 0;
    float maxX = (float)(F<int32_t>(m, 0x15d9) - 1);
    if (!(x <= maxX)) return 0;
    if (y < zero) return 0;
    float maxY = (float)(F<int32_t>(m, 0x15d5) - 1);
    if (!(y <= maxY)) return 0;
    return 1;
}
RT3_REPLACE(0x414bd0, Map_Contains);

// 0x414c50 body. Whether any cell of the 4x4 block (cx, cy) has map flag 2; stops
// (false) at the map edge.
RT3_NOINLINE int32_t __cdecl Map_BlockHasFlag2Impl(int32_t cx, int32_t cy) {
    uint32_t y = (uint32_t)cy * 4;
    for (int32_t dy = 0; dy < 4; ++dy, ++y) {
        uint32_t x = (uint32_t)cx * 4;
        for (int32_t dx = 0; dx < 4; ++dx, ++x) {
            uint8_t* m = g_map;
            if (x >= F<uint32_t>(m, 0x215d) || y >= F<uint32_t>(m, 0x2161)) return 0;
            if (Map_CellFlag(m, 0, (int32_t)x, (int32_t)y, 2)) return 1;
        }
    }
    return 0;
}

// 0x414c50. Block x in EAX, y in EDX. Returns with ECX = x * 4 (the original
// pops its x * 4 local into ECX).
__declspec(naked) void Map_BlockHasFlag2() {
    __asm {
        push eax
        push edx
        push eax
        call Map_BlockHasFlag2Impl
        add esp, 8
        pop ecx
        shl ecx, 2
        ret
    }
}
RT3_REPLACE(0x414c50, Map_BlockHasFlag2);

// 0x414cd0 body. Distance (in blocks, cell distance / 4) from the centre of 4x4
// block (cx, cy) to the nearest cell around it (-4..3) with map flag 2, into *out
// (9999 when none); whether one was found. Cells on the map's first row or column
// are skipped.
RT3_NOINLINE int32_t __cdecl Map_NearestFlag2Impl(float* out, int32_t cx, int32_t cy) {
    *out = 9999.0f;
    int32_t found = 0;
    for (int32_t dy = -4; dy < 4; ++dy)
        for (int32_t dx = -4; dx < 4; ++dx) {
            uint8_t* m = g_map;
            int32_t x = dx + cx * 4, y = dy + cy * 4;
            if (x <= 0 || y <= 0 || x >= F<int32_t>(m, 0x215d) || y >= F<int32_t>(m, 0x2161)) continue;
            if (!Map_CellFlag(m, 0, x, y, 2)) continue;
            found = 1;
            float fy = (float)((double)dy + D(0x5c8628));
            float fx = (float)((double)dx + D(0x5c8628));
            double best = *out;
            double d = Distance2D(0.0f, 0.0f, fx, fy) / D(0x5c87b8);
            double v = d > best ? (double)*out : Distance2D(0.0f, 0.0f, fx, fy) / D(0x5c87b8);
            *out = (float)v;
        }
    return found;
}

// 0x414cd0. Output in EBX, block on the stack.
__declspec(naked) void Map_NearestFlag2() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push ebx
        call Map_NearestFlag2Impl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x414cd0, Map_NearestFlag2);

// 0x414e10. Snapshots the terrain flags (low nibble | counter << 4) and cell bytes
// of the rectangle 0x62b7a8..0x62b7b4 into 0x62b96c / 0x62b9d1 (count 0x62ba36).
RT3_NOINLINE void __fastcall Terrain_SnapshotFlags() {
    uint8_t* flags = (uint8_t*)0x62b96c;
    uint8_t* bytes = (uint8_t*)0x62b9d1;
    int32_t& n = g_62ba36;
    n = 0;
    for (int32_t y = g_62b7b0; y <= g_62b7b4; ++y)
        for (int32_t x = g_62b7a8; x <= g_62b7ac; ++x, ++flags, ++bytes) {
            if (x < 0 || x >= F<int32_t>(Map(), 0x15d9) - 1 || y < 0 || y >= F<int32_t>(Map(), 0x15d5) - 1) {
                ++n;
                *flags = 0;
                *bytes = 0;
                continue;
            }
            *flags = Terrain_GetFlag(Map(), 0, x, y);
            *flags |= (uint8_t)(Terrain_CellCount(Map(), 0, x, y) << 4);
            *bytes = (uint8_t)Terrain_CellByte(Map(), 0, x, y);
            ++n;
        }
}
RT3_REPLACE(0x414e10, Terrain_SnapshotFlags);

// 0x414ee0. Source in EAX, rect and second source on the stack.
__declspec(naked) void Terrain_RestoreRect() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call Terrain_RestoreRectImpl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x414ee0, Terrain_RestoreRect);

// 0x414fa0. Gathers the float grid values inside the marked bounding box
// (0x62b7a8..0x62b7b4) whose mask bit is set; returns how many.
RT3_NOINLINE int32_t __fastcall Map_GatherMasked(void*, int, float* out, int32_t) {
    int32_t n = 0;
    for (int32_t y = g_62b7b0; y <= g_62b7b4; ++y) {
        for (int32_t x = g_62b7a8; x <= g_62b7ac; ++x) {
            uint8_t* map = g_map;
            if (!(F<uint8_t*>(map, 0x2135)[(F<int32_t>(map, 0x2155) + 1) * y + x] & 1)) continue;
            out[n++] = (float)Map_GridFloat(map, 0, x, y);
        }
    }
    return n;
}
RT3_REPLACE(0x414fa0, Map_GatherMasked);

// 0x415020. Checks the placement rect (0x62b7a8 x0, 0x62b7ac x1, 0x62b7b0 y0,
// 0x62b7b4 y1) for flatness: heights must stay within 40 (4 for ports, 0x62b2fc
// +0x9c) of the masked mean (0x62b7c4) unless lenient (allowSteep) or the cell is
// flagged; min/max to 0x62b7b8/0x62b7bc, middle to 0x62b7c0. Failure sets the
// message 0x62b270 (string 0xb8) and returns 0.
RT3_NOINLINE int32_t __stdcall Site_CheckFlatness(int32_t allowSteep, int32_t ignoreFlags) {
    float x1f = (float)g_62b7ac;
    if (*(uint32_t*)&x1f == 0xc97423f0) return 1;
    g_62b7bc = 0xc97423f0;
    g_62b7b8 = 0x497423f0;
    g_62b7c4 = 0x497423f0;
    g_62b7c0 = 0;
    uint8_t* types = hook::As<uint8_t*>(g_buildingTypes);
    float tol = F<int32_t>(types, 0x9c) ? 4.0f : 40.0f;
    float buf[0x80];
    int32_t n = Map_GatherMasked(nullptr, 0, buf, 0x80);
    if (n > 0) {
        float sum = 0.0f;
        for (int32_t i = 0; i < n; ++i) sum = (float)((double)sum + buf[i]);
        hook::As<float>(g_62b7c4) = (float)((double)sum / (float)n);
    }
    float& lo = hook::As<float>(g_62b7b8);
    float& hi = hook::As<float>(g_62b7bc);
    for (int32_t y = g_62b7b0; y <= g_62b7b4; ++y)
        for (int32_t x = g_62b7a8; x <= g_62b7ac; ++x) {
            uint8_t* m = g_map;
            float h = (float)Map_GridFloat(m, 0, x, y);
            m = g_map;
            bool check = (F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x] & 1) ||
                         (!ignoreFlags && !F<int32_t>(hook::As<uint8_t*>(g_buildingTypes), 0x9c) &&
                          Game_EffectiveVersion() >= 0x68);
            if (check) {
                if (g_62b7c4 == 0x497423f0) {
                    hook::As<float>(g_62b7c4) = h;
                } else if (fabs((double)h - hook::As<float>(g_62b7c4)) > tol && !allowSteep) {
                    const char* s = GetString(0xb8);
                    char* d = (char*)0x62b270;
                    while ((*d++ = *s++) != 0) {}
                    return 0;
                }
            }
            if (lo > h) lo = h;
            if (hi < h) hi = h;
        }
    if (!F<int32_t>(g_app, 0x4fd) && F<int32_t>(hook::As<uint8_t*>(g_buildingTypes), 0x9c)) {
        double diff = (double)hi - lo;
        double lim = g_62ba3a != -1 ? D(0x5c9680) : (double)Fl(0x5c8c28);
        if (diff > lim && !allowSteep) return 0;
    }
    hook::As<float>(g_62b7c0) = (float)(((double)lo + hi) * Fl(0x5c8d48));
    return 1;
}
RT3_REPLACE(0x415020, Site_CheckFlatness);

// 0x4152d0 body. Re-lays the track over the cell rectangle r = { x0, x1, y0,
// y1 } grown by one cell (clamped to the map), unless the world is loading
// (0x6ce9a8).
RT3_NOINLINE void __cdecl Tracks_RefreshCellsImpl(const int32_t* r) {
    if (g_6ce9a8) return;
    uint8_t* m = g_map;
    int32_t y0 = r[2] - 1 < 0 ? 0 : r[2] - 1;
    int32_t h = F<int32_t>(m, 0x2159);
    int32_t y1 = h >= r[3] + 1 ? r[3] + 1 : h - 1;
    int32_t x0 = r[0] - 1 < 0 ? 0 : r[0] - 1;
    int32_t w = F<int32_t>(m, 0x2155);
    int32_t x1 = w >= r[1] + 1 ? r[1] + 1 : w - 1;
    TrackNet_RelayRect(g_tracks, 0, (uint32_t)(x0), (uint32_t)(x1), (uint32_t)(y0), (uint32_t)(y1));
}

// 0x4152d0. Rectangle in EAX.
__declspec(naked) void Tracks_RefreshCells() {
    __asm {
        push eax
        call Tracks_RefreshCellsImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x4152d0, Tracks_RefreshCells);

// 0x415340. Tracks_RefreshCells over (x0, y0) - (x1, y1).
void __fastcall Tracks_RefreshArea(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    int32_t r[4] = { x0, x1, y0, y1 };
    Tracks_RefreshCellsImpl(r);
}
RT3_REPLACE(0x415340, Tracks_RefreshArea);

// 0x415370. Renderer size in pixels per map tile (x: +0x215d, y: +0x2161).
RT3_NOINLINE void __stdcall Map_PixelsPerTile(float* sx, float* sy) {
    int32_t w, h;
    Render_GetSize4((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, &w, &h);
    float fw = (float)w, mw = (float)F<int32_t>(Map(), 0x215d);
    *sx = (float)((double)fw / mw);
    float fh = (float)h, mh = (float)F<int32_t>(Map(), 0x2161);
    *sy = (float)((double)fh / mh);
}
RT3_REPLACE(0x415370, Map_PixelsPerTile);

// 0x4153f0. Whether the 8x8 footprint (mask 0x5edd30 for `shape`) around pos is
// buildable and flat enough: every masked cell free of features and objects
// (0x62b300), and the height range within 1 (5 without regions) x 0x5c9680.
RT3_NOINLINE int32_t __stdcall Site_FootprintFlat(const int32_t* pos, int32_t shape) {
    float lo = 1000000.0f, hi = -1000000.0f;
    for (int32_t j = 0; j < 8; ++j) {
        uint32_t y = (uint32_t)(pos[2] + j - 3);
        if (y >= (uint32_t)(F<int32_t>(Map(), 0x2159) - 1)) return 0;
        for (int32_t i = 0; i < 8; ++i) {
            uint32_t x = (uint32_t)(pos[0] + i - 3);
            int32_t w = F<int32_t>(Map(), 0x2155);
            if (x >= (uint32_t)(w - 1)) return 0;
            if (((const uint8_t*)0x5edd30)[(j + shape * 8) * 8 + i]) {
                uint32_t* occ = hook::As<uint32_t*>(g_cellOwners);
                uint32_t o = occ ? occ[w * (int32_t)y + (int32_t)x] : 0;
                if (Map_CellFlag(Map(), 0, (int32_t)x * 4, (int32_t)y * 4, 0)) return 0;
                if (Map_NearFeature(Map(), 0, (int32_t)x, (int32_t)y)) return 0;
                if (o) return 0;
            }
            float h = (float)Map_GridFloat(Map(), 0, (int32_t)x, (int32_t)y);
            if (lo > h) lo = h;
            if (hi < h) hi = h;
        }
    }
    float k = F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88) > 0 ? 1.0f : 5.0f;
    double limit = (double)k * D(0x5c9680);
    return !((double)hi - lo > limit) ? 1 : 0;
}
RT3_REPLACE(0x4153f0, Site_FootprintFlat);

// 0x415570. Chooses a footprint shape (0x62ba3a) for placing obj at 0x62b7a8: the
// preferred one (0x5ee030, consumed) or a random one, else the first that fits.
RT3_NOINLINE int32_t __stdcall Site_ChooseShape(uint8_t* obj) {
    if (!hook::As<uint32_t*>(g_cellOwners)) {
        uint8_t* m = g_map;
        uint32_t n = (uint32_t)(F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155)) << 2;
        uint8_t* p = (uint8_t*)crt::malloc(n);
        hook::As<uint8_t*>(g_cellOwners) = p;
        for (uint32_t k = 0; k < n; ++k) p[k] = 0;
    }
    g_62ba3a = -1;
    void* rec = FindStaticRecord(nullptr, 0, obj);
    g_62ba3e = rec;
    if (!rec) return 0;
    const int32_t* pos = (const int32_t*)0x62b7a8;
    int32_t pref = g_5ee030;
    if (pref != -1) {
        if (!Site_FootprintFlat(pos, pref)) {
            for (uint32_t s = 0; s < 0xc; ++s)
                if (Site_FootprintFlat(pos, s) == 1) {
                    g_5ee030 = -1;
                    g_62ba3a = (int32_t)s;
                    return 1;
                }
        }
        int32_t p = g_5ee030;
        g_5ee030 = -1;
        g_62ba3a = p;
        return 1;
    }
    int32_t s = SimRand(0, 0xb);
    if (Site_FootprintFlat(pos, s)) {
        g_62ba3a = s;
        return 1;
    }
    for (uint32_t t = 0; t < 0xc; ++t)
        if (Site_FootprintFlat(pos, t) == 1) {
            g_62ba3a = (int32_t)t;
            return 1;
        }
    return 0;
}
RT3_REPLACE(0x415570, Site_ChooseShape);

// 0x4156b0 body. Stamps a building footprint into the terrain: for each cell of the
// 8x8 shape (0x5edd30, by shape +0x732; -1 none) at block (+0x4a0, +0x4a8) that is
// not map flag 0, records owner in the cell-owner map 0x62b300 (allocated zeroed on
// first use) and adds terrain points of kind (type index of +0x736) * 7 + 6, 4 of
// them for type kind 3, else 999. Then marks the scene dirty and refreshes the view.
RT3_NOINLINE void __cdecl Footprint_StampImpl(uint8_t* o, int32_t owner) {
    if (!g_cellOwners) {
        uint8_t* m = g_map;
        size_t n = (size_t)(F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155) * 4);
        void* p = crt::malloc(n);
        hook::As<void*>(g_cellOwners) = p;
        memset(p, 0, n);
    }
    if (!o) o = (uint8_t*)0x62b308;
    if (F<int32_t>(o, 0x732) == -1) return;
    int32_t kind = (int32_t)(F<uint32_t>(o, 0x736) - 0x5ef3e0) / 0xb0 * 7 + 6;
    for (int32_t dy = 0; dy < 8; ++dy) {
        int32_t y = F<int32_t>(o, 0x4a8) + dy;
        for (int32_t dx = 0; dx < 8; ++dx) {
            int32_t x = F<int32_t>(o, 0x4a0) + dx;
            if (!*(uint8_t*)(0x5edd30 + (dy + F<int32_t>(o, 0x732) * 8) * 8 + dx)) continue;
            if (Map_CellFlag(hook::As<void*>(g_map), 0, x * 4, y * 4, 0)) continue;
            uint8_t* m = g_map;
            g_cellOwners[F<int32_t>(m, 0x2155) * y + x] = owner;
            Terrain_AddPoints(g_map, 0, x, y, (uint8_t)kind, F<int32_t>(F<uint8_t*>(o, 0x736), 4) == 3 ? 4 : 0x3e7);
        }
    }
    ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x374a] = 1;
    void* view = hook::As<void*>(g_62be68);
    ((void(__fastcall*)(void*))(*(void***)view)[0])(view);
}

// 0x4156b0. Object in EAX, owner on the stack.
__declspec(naked) void Footprint_Stamp() {
    __asm {
        push [esp + 4]
        push eax
        call Footprint_StampImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x4156b0, Footprint_Stamp);

// 0x415820 body. Applies a dirty-cell list (list +0x10, or the global 0x62b318 for
// null; entries as in 0x413df0, -1 ended): sets each cell's fine bit (1 for flagged
// entries, else 2), marks each changed 4x4 block for redraw (terrain flag 0xf and
// block byte bit 1), counts the pass (0x62ba78) and marks the scene dirty. Returns
// the list end in *cursor (the original leaves it in ECX).
RT3_NOINLINE int32_t __cdecl DirtyCells_ApplyImpl(uint8_t* list, uint32_t** cursor) {
    int32_t lastX = -1, lastY = -1;
    ++g_62ba78;
    uint32_t* e = list ? (uint32_t*)(list + 0x10) : (uint32_t*)0x62b318;
    *cursor = e;
    for (; *e != 0xffffffff; *cursor = ++e) {
        uint32_t v = *e;
        int32_t x = (int32_t)(v & 0x7fff), y = (int32_t)(v >> 16 & 0x7fff);
        Map_SetFineBit(g_map, 0, x, y, 1, (int32_t)v < 0 ? 1 : 2, 0);
        int32_t bx = x >> 2, by = y >> 2;
        if (bx == lastX && by == lastY) continue;
        Terrain_SetFlag(g_map, 0, bx, by, 0xf);
        uint8_t* m = g_map;
        F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * by + bx] |= 1;
        lastX = bx;
        lastY = by;
    }
    ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x374a] = 1;
    return 1;
}

// 0x415820. List in EAX.
__declspec(naked) void DirtyCells_Apply() {
    __asm {
        push ecx
        push esp
        push eax
        call DirtyCells_ApplyImpl
        add esp, 8
        pop ecx
        ret
    }
}
RT3_REPLACE(0x415820, DirtyCells_Apply);

// 0x415900 body. Whether any cell of the 4x4 blocks of rect { x0, x1, y0, y1 },
// grown by one block and clamped to the map, has map flag 1.
RT3_NOINLINE int32_t __cdecl Map_RectHasFlag1Impl(const int32_t* r) {
    uint8_t* m = g_map;
    int32_t bx0 = r[0] - 1 <= 0 ? 0 : r[0] - 1;
    int32_t bx1 = r[1] + 1 < F<int32_t>(m, 0x2155) - 1 ? r[1] + 1 : F<int32_t>(m, 0x2155) - 1;
    int32_t by0 = r[2] - 1 <= 0 ? 0 : r[2] - 1;
    int32_t by1 = r[3] + 1 < F<int32_t>(m, 0x2159) - 1 ? r[3] + 1 : F<int32_t>(m, 0x2159) - 1;
    for (int32_t by = by0; by <= by1; ++by)
        for (int32_t bx = bx0; bx <= bx1; ++bx)
            for (int32_t dy = 0; dy < 4; ++dy)
                for (int32_t dx = 0; dx < 4; ++dx)
                    if (Map_CellFlag(hook::As<void*>(g_map), 0, dx + bx * 4, dy + by * 4, 1)) return 1;
    return 0;
}

// 0x415900. Rect in EAX.
__declspec(naked) void Map_RectHasFlag1() {
    __asm {
        push eax
        call Map_RectHasFlag1Impl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x415900, Map_RectHasFlag1);

// 0x4159e0 body. Levels the ground of site `id` over cell rect r towards
// height h (skipped while loading when the site touches fine cells). Mode 1
// eases each cell (with any fine structure nearby, from version 0x68 by its
// distance) towards h, rising at most upK (5, or per site type, 0x5ee038)
// and falling at most downK (9) scaled by steepness; others set h outright
// on unfixed cells. Multiplayer games with v1.02 peers round to whole
// heights. With `refresh` announces the rect.
RT3_NOINLINE void __cdecl Site_LevelGroundImpl(int32_t id, const int32_t* r, float h, int32_t mode, int32_t refresh) {
    int32_t touches = Map_RectHasFlag1Impl(r);
    if (g_6ce9a8 && touches) return;
    int32_t steep = 0, maint = 0;
    float upK = 5.0f, downK = 9.0f;
    if (Collection_IsValid(g_buildingTypes, 0, (uint32_t)id)) {
        uint8_t* site = (uint8_t*)Collection_Get(g_buildingTypes, 0, (uint32_t)id);
        uint8_t* type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(site, 0x173));
        steep = type[0x32] == 1;
        const char* name = (const char*)site + 4;
        for (const uint32_t* e = (const uint32_t*)0x5ee038; e < (const uint32_t*)0x5ee0f8; e += 2)
            if (!crt::stricmp(name, (const char*)(uintptr_t)e[1])) memcpy(&upK, e, 4);
        if (!crt::stricmp(name, (const char*)0x5c8710)) maint = 1;
    }
    if (F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88)) {
        int32_t k = (touches ? 1 : 0) + 1;
        float a = (float)(k * 5);
        upK = (float)((double)a + upK);
        float c = (float)(k * 9);
        downK = (float)((double)c + Fl_207(0x5c9690));
    }
    if (F<int32_t>(g_world, 0x46c38)) {
        upK = 2.5f;
        downK = 4.5f;
    }
    if (g_newGame[0x82] && steep) {
        upK = (float)((double)upK * D(0x5c8880));
        downK = (float)((double)downK * D(0x5c8880));
    }
    if (maint) downK = (float)((double)downK * D(0x5c8600));
    auto store = [&](int32_t x, int32_t y, float v) {
        if (IsMultiplayer() && Net_AllHaveVersion(BitsF(0x3f828f5c))) v = (float)Ftol((double)v);
        Map_SetCellHeight(g_map, 0, x, y, *(uint32_t*)&v, 0);
    };
    if (mode == 1) {
        for (int32_t y = r[2]; y <= r[3]; ++y)
            for (int32_t x = r[0]; x <= r[1]; ++x) {
                float s = 0.0f;
                int32_t ok;
                if (Game_EffectiveVersion() >= 0x68) ok = Map_NearestFlag2Impl(&s, x, y);
                else if (g_cellOwners && g_cellOwners[F<int32_t>(g_map, 0x2155) * y + x])
                    ok = Map_BlockHasFlag2Impl(x, y);
                else
                    ok = 1;
                if (!ok) continue;
                float v = h;
                if (Game_EffectiveVersion() >= 0x68) {
                    float g = (float)Map_GridFloat(g_map, 0, x, y);
                    bool up;
                    if (!((double)s > D(0x5c9688)) && s == s) s = 0.0f;
                    else if (steep && (double)g > (double)h) {
                        double t = (double)s - D(0x5c88c8);
                        s = (float)(D(0x5c9688) > t ? 0.1875 : t);
                    }
                    up = (double)g > (double)h;
                    double f = (double)s > D(0x5c8650) ? 0.95 : (double)s;
                    float k = (float)(D(0x5c8770) / (D(0x5c8770) - f) * (up ? upK : downK) * D(0x5c85a8));
                    if ((double)s < D(0x5c85b8)) k = 0.0f;
                    if (up) {
                        float lim = (float)((double)k + h);
                        v = g < lim ? g : lim;
                    } else {
                        float lim = (float)((double)h - k);
                        v = g > lim ? g : lim;
                    }
                }
                store(x, y, v);
            }
    } else {
        for (int32_t y = r[2]; y <= r[3]; ++y)
            for (int32_t x = r[0]; x <= r[1]; ++x) {
                uint8_t* m = g_map;
                if (F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x] & 1) continue;
                if (g_cellOwners && g_cellOwners[F<int32_t>(m, 0x2155) * y + x] && !Map_BlockHasFlag2Impl(x, y))
                    continue;
                store(x, y, h);
            }
    }
    if (refresh) Map_CellsChangedImpl(g_map, 0, r[0], r[2], r[1], r[3]);
}

// 0x4159e0. Site id in EAX, the rest on the stack.
__declspec(naked) void Site_LevelGround() {
    __asm {
        push dword ptr [esp + 0x10]
        push dword ptr [esp + 0x10]
        push dword ptr [esp + 0x10]
        push dword ptr [esp + 0x10]
        push eax
        call Site_LevelGroundImpl
        add esp, 0x14
        ret 0x10
    }
}
RT3_REPLACE(0x4159e0, Site_LevelGround);

// 0x415f20. Site list node n (next, site) at cell (cx, cy): stamps its site
// as visited (0x62ba80), grows the bounding rect (*x0 / *y0 / *x1 / *y1) by
// the site's rect and recurses into the other unvisited sites sharing any of
// its cells (hash 0x62ba7c by y << 16 | x); a shapeless site (+0x732 -1)
// levels its ground to h. Then regrounds the site's object (+0x742).
RT3_NOINLINE void __fastcall Sites_SpreadLevel(uint8_t* n, int32_t cx, float h, int32_t cy, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1) {
    uint8_t* s = F<uint8_t*>(n, 4);
    F<uint32_t>(s, 0x73e) = g_62ba80;
    if (*y0 >= F<int32_t>(F<uint8_t*>(n, 4), 0x4a8)) *y0 = F<int32_t>(F<uint8_t*>(n, 4), 0x4a8);
    if (*y1 <= F<int32_t>(F<uint8_t*>(n, 4), 0x4ac)) *y1 = F<int32_t>(F<uint8_t*>(n, 4), 0x4ac);
    if (*x0 >= F<int32_t>(F<uint8_t*>(n, 4), 0x4a0)) *x0 = F<int32_t>(F<uint8_t*>(n, 4), 0x4a0);
    if (*x1 <= F<int32_t>(F<uint8_t*>(n, 4), 0x4a4)) *x1 = F<int32_t>(F<uint8_t*>(n, 4), 0x4a4);
    for (int32_t y = F<int32_t>(F<uint8_t*>(n, 4), 0x4a8); y <= F<int32_t>(F<uint8_t*>(n, 4), 0x4ac); ++y)
        for (int32_t x = F<int32_t>(F<uint8_t*>(n, 4), 0x4a0); x <= F<int32_t>(F<uint8_t*>(n, 4), 0x4a4); ++x) {
            if (x == cx && y == cy) continue;
            uint8_t* m = g_map;
            if ((uint32_t)x >= (uint32_t)(F<int32_t>(m, 0x2155) - 1) || (uint32_t)y >= (uint32_t)(F<int32_t>(m, 0x2159) - 1)) continue;
            uint32_t key = (uint32_t)y << 16 | ((uint32_t)x & 0xffff);
            for (uint8_t* e = (uint8_t*)(uintptr_t)Hash_Get(g_62ba7c, 0, key); e; e = F<uint8_t*>(e, 0)) {
                if (F<uint8_t*>(e, 4) == F<uint8_t*>(n, 4)) continue;
                if (F<uint32_t>(F<uint8_t*>(e, 4), 0x73e) == g_62ba80) continue;
                Sites_SpreadLevel(e, x, h, y, x0, y0, x1, y1);
            }
        }
    if (F<int32_t>(F<uint8_t*>(n, 4), 0x732) == -1) Site_LevelGroundImpl(0, (const int32_t*)(F<uint8_t*>(n, 4) + 0x4a0), h, 1, 0);
    Obj_Reground(F<uint8_t*>(F<uint8_t*>(n, 4), 0x742));
}
RT3_REPLACE(0x415f20, Sites_SpreadLevel);

// 0x4160c0. Levels the ground of the site in list node n and of every site
// sharing cells with it (Sites_SpreadLevel from cell (cx, cy) to height h) -
// only for version 1.04+ games (Game_EffectiveVersion > 0x67) unless
// 0x6cec78 +0x46c38 - with the map's change count (+0x2205) held up meanwhile,
// then announces the touched rect grown by one cell (two at the far side).
RT3_NOINLINE void __stdcall Sites_LevelFrom(uint8_t* n, float h, int32_t cx, int32_t cy) {
    ++F<int32_t>(g_map, 0x2205);
    int32_t y1 = -1, x1 = -1, y0 = 999999, x0 = 999999;
    if (Game_EffectiveVersion() <= 0x67 || F<int32_t>(g_world, 0x46c38)) Sites_SpreadLevel(n, cx, h, cy, &x0, &y0, &x1, &y1);
    --F<int32_t>(g_map, 0x2205);
    Map_CellsChangedImpl(g_map, 0, x0 - 1, y0 - 1, x1 + 2, y1 + 2);
}
RT3_REPLACE(0x4160c0, Sites_LevelFrom);

// 0x416170 body. Adds site s to the cell hash (in EAX, kept in 0x62ba7c) for
// every cell of rect r { x0, x1, y0, y1 }: a list node { next, site } from
// pool p (its free list when +0xc, else new +0x10 bytes) goes in front of
// the cell's list. A shapeless site (+0x732 -1, not a port type, no editor
// option +0x4fd, unless noLevel) first levels each site already there to its
// height (+0x4bc); an empty cell gets coarse flag 0x80. Bumps the visit
// stamp 0x62ba80.
RT3_NOINLINE void __cdecl Sites_AddToCellHashImpl(HashMap* hash, const int32_t* r, uint8_t* p, uint8_t* s, int32_t noLevel) {
    g_62ba7c = hash;
    ++hook::As<int32_t>(g_62ba80);
    for (int32_t y = r[2]; y <= r[3]; ++y)
        for (int32_t x = r[0]; x <= r[1]; ++x) {
            uint8_t* node;
            if (F<int32_t>(p, 0xc)) {
                uint8_t* list = F<uint8_t*>(p, 0);
                node = F<uint8_t*>(list, 0);
                if (node) {
                    uint8_t* next = F<uint8_t*>(node, 0);
                    bool last = node == F<uint8_t*>(list, 4);
                    F<uint8_t*>(list, 0) = next;
                    if (last) F<uint8_t*>(list, 4) = next;
                    --F<int32_t>(list, 8);
                }
            } else
                node = (uint8_t*)crt::operator_new(F<uint32_t>(p, 0x10));
            uint32_t key = (uint32_t)y << 16 | ((uint32_t)x & 0xffff);
            F<uint8_t*>(node, 4) = s;
            int32_t e = Hash_Get(g_62ba7c, 0, key);
            if (e) {
                if (F<int32_t>(s, 0x732) == -1 && !F<int32_t>(hook::As<uint8_t*>(g_buildingTypes), 0x9c) && !F<int32_t>(g_app, 0x4fd) &&
                    !noLevel)
                    for (uint8_t* n = (uint8_t*)(uintptr_t)e; n; n = F<uint8_t*>(n, 0)) Sites_LevelFrom(n, F<float>(s, 0x4bc), x, y);
                F<int32_t>(node, 0) = e;
                Hash_Remove(g_62ba7c, 0, key);
            } else {
                F<int32_t>(node, 0) = 0;
                Map_SetCoarseByte(hook::As<void*>(g_map), 0, x, y, 0x80);
            }
            Hash_Insert(g_62ba7c, 0, key, (int32_t)(uintptr_t)node);
        }
}

// 0x416170. Hash in EAX, the rest on the stack.
__declspec(naked) void Sites_AddToCellHash() {
    __asm {
        push dword ptr [esp + 0x10]
        push dword ptr [esp + 0x10]
        push dword ptr [esp + 0x10]
        push dword ptr [esp + 0x10]
        push eax
        call Sites_AddToCellHashImpl
        add esp, 0x14
        ret 0x10
    }
}
RT3_REPLACE(0x416170, Sites_AddToCellHash);

// 0x4162d0. Releases site's claim on the cells of rect {x0, x1, y0, y1}: unlinks it
// from each cell's chain in the cell hash (key y << 16 | x; nodes {next, site}),
// handing its per-cell data (+0x4c8 / +0x664 / +0x6c9, k-th cell) to the site
// before it, clears the placement grid (0x62b300) where it was the owner, and
// returns the node to the pool (or frees it).
RT3_NOINLINE void __stdcall Site_ReleaseCells(const int32_t* rect, HashMap* hash, uint32_t* pool, uint8_t* site) {
    int32_t k = 0;
    for (int32_t y = rect[2]; y <= rect[3]; ++y)
        for (int32_t x = rect[0]; x <= rect[1]; ++x, ++k) {
            uint32_t key = ((uint32_t)y << 16) | (uint32_t)(x & 0xffff);
            uint32_t* n = (uint32_t*)Hash_Get(hash, 0, key);
            if (!n) continue;
            int32_t count = 0;
            for (uint32_t* p = n; p; p = (uint32_t*)p[0]) ++count;
            uint32_t* node = n;
            uint8_t* m = g_map;
            if (count > 1) {
                if ((uint8_t*)n[1] == site) {
                    Hash_Remove(hash, 0, key);
                    Hash_Insert(hash, 0, key, (int32_t)n[0]);
                } else {
                    uint32_t* prev;
                    do {
                        prev = node;
                        node = (uint32_t*)node[0];
                    } while ((uint8_t*)node[1] != site);
                    uint8_t* other = (uint8_t*)prev[1];
                    int32_t j = 0, found = 0;
                    for (int32_t yy = F<int32_t>(other, 0x4a8); yy <= F<int32_t>(other, 0x4ac) && !found; ++yy)
                        for (int32_t xx = F<int32_t>(other, 0x4a0); xx <= F<int32_t>(other, 0x4a4); ++xx, ++j)
                            if (yy == y && xx == x) {
                                found = 1;
                                break;
                            }
                    if (!found) j = 0;
                    prev[0] = node[0];
                    uint8_t* s = (uint8_t*)node[1];
                    F<uint32_t>(other, 0x4c8 + j * 4) = F<uint32_t>(s, 0x4c8 + k * 4);
                    other[0x664 + j] = s[0x664 + k];
                    other[0x6c9 + j] = s[0x6c9 + k];
                }
                uint8_t* s = (uint8_t*)node[1];
                F<uint32_t>(s, 0x4c8 + k * 4) = 0x497423f0;
                uint32_t* occ = hook::As<uint32_t*>(g_cellOwners);
                if (F<int32_t>(s, 0x732) != -1 && (uint8_t*)(occ ? occ[F<int32_t>(m, 0x2155) * y + x] : 0) == s) {
                    occ[F<int32_t>(m, 0x2155) * y + x] = 0;
                } else {
                    s[0x664 + k] = 0xff;
                    s[0x6c9 + k] = 0xff;
                }
            } else {
                Hash_Remove(hash, 0, key);
                if (F<int32_t>((uint8_t*)n[1], 0x732) != -1)
                    hook::As<uint32_t*>(g_cellOwners)[F<int32_t>(g_map, 0x2155) * y + x] = 0;
                Map_SetCoarseByte(hook::As<void*>(g_map), 0, x, y, 0);
            }
            if (pool[3] && (uint32_t)node >= pool[1] && (uint32_t)node < pool[2]) {
                uint32_t* l = (uint32_t*)pool[0];
                node[0] = l[0];
                l[0] = (uint32_t)node;
                ++l[2];
                if (!l[1]) l[1] = (uint32_t)node;
            } else {
                crt::operator_delete(node);
            }
        }
}
RT3_REPLACE(0x4162d0, Site_ReleaseCells);

// 0x416590. Resets the global block 0x62b308 (and allocates the per-cell
// buffer 0x62b300 if missing).
RT3_NOINLINE void ResetGlobals62b308() {
    uint8_t* b = (uint8_t*)0x62b308;
    for (int i = 0; i < 0x1d3 * 4 + 2; ++i) b[i] = 0;
    g_62b318 = -1;
    g_62ba3a = -1;
    g_62b7ac = (int32_t)0xfff0bdc1;
    g_62b7b4 = (int32_t)0xfff0bdc1;
    g_62b7a8 = 0xf423f;
    g_62b7b0 = 0xf423f;
    g_62b270 = 0;
    if (hook::As<void*>(g_cellOwners)) return;
    uint8_t* map = g_map;
    uint32_t n = F<int32_t>(map, 0x2159) * F<int32_t>(map, 0x2155) * 4;
    uint8_t* p = (uint8_t*)crt::malloc(n);
    hook::As<void*>(g_cellOwners) = p;
    for (uint32_t i = 0; i < n; ++i) p[i] = 0;
}
RT3_REPLACE(0x416590, ResetGlobals62b308);

// 0x416620 body. Checks that building type can go on the placement rect
// (0x62b7a8..0x62b7b4, grown by one cell right / down within the map) at a, b
// (kept in 0x62b30c / 0x62b310): no track around it (0xb9); a type with a shape
// (+0x123) must find one (0xba, rect then 3 cells out each way, 8 wide); ports
// (0x62b2fc +0x9c) must not cover occupied cells of grid 0x62b300 (0xbb). Success
// takes the terrain flag snapshot. An unset rect (x1 = -1000000) always passes.
RT3_NOINLINE int32_t __cdecl Site_CheckPlacementImpl(uint8_t* type, uint32_t a, uint32_t b) {
    float x1f = (float)g_62b7ac;
    g_62b308 = hook::As<uint32_t>(g_62b304);
    g_62b30c = a;
    g_62b310 = b;
    if (*(uint32_t*)&x1f == 0xc97423f0) return 1;
    int32_t& x0 = g_62b7a8;
    int32_t& x1 = g_62b7ac;
    int32_t& y0 = g_62b7b0;
    int32_t& y1 = g_62b7b4;
    uint8_t* map = g_map;
    if (x1 < F<int32_t>(map, 0x2155) - 1) ++x1;
    if (y1 < F<int32_t>(map, 0x2159) - 1) ++y1;
    ((int32_t*)0x62b318)[g_62b7a4] = -1;
    int32_t special = F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88) <= 0 ? 1 : 0;
    if (!Track_RectIsFree(hook::As<void*>(g_tracks), 0, x0 - 1, x1 + 1, y0 - 1, y1 + 1, special)) {
        Site_SetMessage(0xb9);
        return 0;
    }
    if (type[0x123]) {
        if (Site_ChooseShape(type) == 1) {
            x0 -= 3;
            x1 = x0 + 8;
            y0 -= 3;
            y1 = y0 + 8;
            Terrain_SnapshotFlags();
            return 1;
        }
        Site_SetMessage(0xba);
        return 0;
    }
    int32_t ok = 1;
    if (F<int32_t>(hook::As<uint8_t*>(g_buildingTypes), 0x9c) == 1) {
        for (int32_t y = y0; y <= y1; ++y) {
            if (ok != 1) continue;
            for (int32_t x = x0; x <= x1; ++x) {
                int32_t* grid = g_cellOwners;
                if (grid && grid[F<int32_t>(map, 0x2155) * y + x]) {
                    Site_SetMessage(0xbb);
                    ok = 0;
                    break;
                }
            }
        }
    }
    if (ok == 1) Terrain_SnapshotFlags();
    return ok;
}

// 0x416620. Type in EAX, a and b on the stack.
__declspec(naked) int32_t Site_CheckPlacement() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call Site_CheckPlacementImpl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x416620, Site_CheckPlacement);

// 0x416830 (list callback). Adds the records of o's chain (+4) not yet in
// the list 0x62ba58: mode 1 keyed 0x7fffffff - value, mode 2 by value
// (count 0x62ba64). Returns 0.
int32_t __fastcall Chain_AddToSortedList(uint8_t* o, const int32_t* mode) {
    uint8_t* head = F<uint8_t*>(o, 4);
    for (uint8_t* p = head; p; p = F<uint8_t*>(p, 0)) {
    }
    int32_t count = 0;
    for (uint8_t* p = head; p; p = F<uint8_t*>(p, 0)) ++count;
    (void)count;
    if (*mode != 1 && *mode != 2) return 0;
    for (uint8_t* p = head; p; p = F<uint8_t*>(p, 0)) {
        if (List_Contains(g_62ba58, 0, F<int32_t>(p, 4))) continue;
        const int32_t* rec = F<const int32_t*>(p, 4);
        List_Push(g_62ba58, 0, *mode == 1 ? 0x7fffffff - *rec : *rec, (int32_t)(uintptr_t)rec);
        ++g_62ba64;
    }
    return 0;
}
RT3_REPLACE(0x416830, Chain_AddToSortedList);

// 0x4168e0. Deletes record `id` of c with its four buffers (+0xcf..+0xdb).
void __fastcall Records_DeleteWithBuffers(Collection* c, int, uint32_t id) {
    uint8_t* r = (uint8_t*)Collection_Get(c, 0, id);
    for (uint32_t off = 0xcf; off <= 0xdb; off += 4)
        if (void* p = F<void*>(r, off)) crt::operator_delete(p);
    Collection_Release(c, 0, id);
}
RT3_REPLACE(0x4168e0, Records_DeleteWithBuffers);

// 0x416950. Removes every site of the collection (vtable slot 2), clears it and
// frees the placement grid (0x62b300), the name hash (+0x88), the two node pools
// (+0x90, +0x8c, with their blocks) and the site globals 0x62ba58..0x62ba74.
RT3_NOINLINE void __fastcall Sites_DestroyAll(Collection* c) {
    while (Collection_Count(c)) {
        void** vt = *(void***)c;
        ((void(__thiscall*)(Collection*, uint32_t))vt[2])(c, Collection_IdAt(c, 0, 0, 0));
    }
    Collection_Clear(c);
    if (hook::As<void*>(g_cellOwners)) {
        crt::free(hook::As<void*>(g_cellOwners));
        hook::As<void*>(g_cellOwners) = nullptr;
    }
    if (HashMap* h = F<HashMap*>(c, 0x88)) {
        Hash_DestroyThunk(h);
        crt::operator_delete(h);
        F<void*>(c, 0x88) = nullptr;
    }
    static const uint32_t pools[2] = { 0x90, 0x8c };
    for (uint32_t off : pools) {
        NodePool* p = F<NodePool*>(c, off);
        if (!p) continue;
        void* block = F<void*>(p, 4);
        Pool_Free(p);
        crt::operator_delete(p);
        F<void*>(c, off) = nullptr;
        if (block) crt::free(block);
    }
    if (hook::As<void*>(g_62ba6c)) crt::free(hook::As<void*>(g_62ba6c));
    static const uint32_t globals[] = { 0x62ba74, 0x62ba58, 0x62ba5c, 0x62ba60, 0x62ba64, 0x62ba6c, 0x62ba70 };
    for (uint32_t g : globals) hook::Global<int32_t>(g) = 0;
    g_5ee030 = -1;
}
RT3_REPLACE(0x416950, Sites_DestroyAll);

// 0x416a70. Saves the site table: the sites collected from the cell hash (+0x88,
// callback 0x416830 into the list 0x62ba58, count 0x62ba64) after a {0xbabe, 1,
// count} header, each record with its variable parts (see Sites_LoadTable).
RT3_NOINLINE void __fastcall Sites_SaveTable(uint8_t* c, int, Stream s) {
    int32_t one = 1;
    uint32_t* l = (uint32_t*)crt::operator_new(0xc);
    if (l) {
        l[0] = 0;
        l[1] = 0;
        l[2] = 0;
    }
    hook::As<uint32_t*>(g_62ba58) = l;
    List_Init((MaxList*)l, 0, 0);
    g_62ba64 = 0;
    Hash_ForEach(F<HashMap*>(c, 0x88), 0, 0, (HashVisit)0x416830, &one);
    int32_t hdr[3] = { 0xbabe, 1, g_62ba64 };
    StreamWrite(s, hdr, 0xc, 0);
    float sx, sy;
    Map_PixelsPerTile(&sx, &sy);
    for (uint8_t* e = (uint8_t*)List_PopMax(g_62ba58); e;
         e = (uint8_t*)List_PopMax(g_62ba58)) {
        static const uint16_t fixed[] = { 0x74a, 4, 8, 0xc, 0x4b8, 0x4bc, 0x49c };
        for (uint16_t off : fixed) StreamWrite(s, e + off, 4, 0);
        for (uint32_t k = 0; k < F<uint32_t>(e, 0x49c) + 1; ++k) StreamWrite(s, e + 0x10 + k * 4, 4, 0);
        StreamWrite(s, e + 0x4a0, 0x10, 0);
        StreamWrite(s, e + 0x4c4, 4, 0);
        if (F<int32_t>(e, 0x4c4) > 0) StreamWrite(s, F<void*>(e, 0x4c0), F<int32_t>(e, 0x4c4), 0);
        StreamWrite(s, e + 0x65c, 4, 0);
        for (uint32_t k = 0; k < F<uint32_t>(e, 0x65c); ++k) StreamWrite(s, e + 0x4c8 + k * 4, 4, 0);
        StreamWrite(s, e + 0x72e, 4, 0);
        for (uint32_t k = 0; k < F<uint32_t>(e, 0x72e); ++k) {
            StreamWrite(s, e + 0x664 + k, 1, 0);
            StreamWrite(s, e + 0x6c9 + k, 1, 0);
        }
        StreamWrite(s, e + 0x732, 4, 0);
    }
    if (MaxList* lst = g_62ba58) {
        List_DestroyThunk(lst);
        crt::operator_delete(lst);
    }
    hook::As<void*>(g_62ba58) = nullptr;
}
RT3_REPLACE(0x416a70, Sites_SaveTable);

// 0x416ce0. Adds building record rec (with fields a / b / c) to table t:
// "port" / "warehouse" type names (+0x22) become "Port00" / "Warehouse00",
// then the type (by name in 0x62b268) goes to +0x173. An unknown type is
// fatal (string 0xbc with the name and +4) and adds nothing.
uint32_t __fastcall Buildings_AddByTypeName(Collection* t, int, uint8_t* rec, uint32_t a, uint32_t b, uint32_t c) {
    char* type = (char*)rec + 0x22;
    if (!crt::strnicmp(type, (const char*)0x5c8f94, 4)) FormatText(type, (const char*)0x5c96a0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (!crt::strnicmp(type, (const char*)0x5c8e4c, 9)) FormatText(type, (const char*)0x5c9694, 0, 0, 0, 0, 0, 0, 0, 0);
    for (int32_t i = 0; i < Collection_Count(g_62b268); ++i) {
        Collection* types = g_62b268;
        if (crt::stricmp(type, (const char*)Collection_Get(types, 0, Collection_IdAt(types, 0, (uint32_t)i, 0)) + 4)) continue;
        F<uint32_t>(rec, 0x173) = Collection_IdAt(g_62b268, 0, (uint32_t)i, 0);
        return Collection_Allocate(t, 0, rec, a, b, c);
    }
    char frame[0x130];
    char* msg = frame + 4;
    FormatText(msg, GetString(0xbc), type, (const char*)rec + 4, 0, 0, 0, 0, 0, 0);
    Fatal_Exit(msg);
    return 0;
}
RT3_REPLACE(0x416ce0, Buildings_AddByTypeName);

// 0x416e20. Id of the element named `name` (+4); fatal (string 0xbd) and -1 if none.
RT3_NOINLINE int32_t __fastcall Collection_FindByName4(Collection* c, int, const char* name) {
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        if (!crt::stricmp(name, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)) + 4))
            return Collection_IdAt(c, 0, i, 0);
    FatalFormat(0xbd, name);
    return -1;
}
RT3_REPLACE(0x416e20, Collection_FindByName4);

 // namespace

// 0x416ec0. Checks (and with `commit` lays) the footprint of building type
// `id` at (x, y) turned by `angle`: each of its four masks (+0xdb track,
// +0xcf solid, +0xd3 water, +0xd7 shore; 16-bit 4 x 4 sub-cell words per
// tile, every other set bit) is rotated into a scratch grid around the site;
// cells off the map fail. Checking only marks the fine bits for preview;
// committing tests each cell (occupied, near a feature, water kind) and
// queues it as dirty. `trackOnly` restricts committing to the track mask.
// Ends with the site placement check; a repeated preview just reapplies
// the dirty cells.
RT3_NOINLINE int32_t __fastcall Site_CheckFootprint(uint8_t* types, int, uint32_t id, float x, float y, float angle, int32_t commit, int32_t trackOnly) {
    if (!commit && g_62b304 && g_62b304 == hook::As<int32_t>(g_62b308))
        return CallDirtyApply();
    uint8_t* rec = (uint8_t*)Collection_Get((Collection*)types, 0, id);
    float fx = (float)((double)x * Fl(0x5c8568));
    double dw = (double)(rec[0xb8] * 4) / D(0x5c85d0);
    float left = (float)(fx - dw);
    double dh = (double)(rec[0xb9] * 4) / D(0x5c85d0);
    float fy = (float)((double)y * Fl(0x5c8568));
    float bottom = (float)(fy + dh);
    while (angle < Fl(0x5c8598)) angle = (float)(angle + D(0x5c96b8));
    float halfW = (float)dw;
    float halfH = (float)dh;
    float cx = (float)((double)halfW + left);
    float cy = (float)((double)bottom - halfH);
    int32_t deg = FtolD((double)angle * Fl(0x5c96b0) / D(0x5c96a8)) % 0x168;
    uint8_t* eng = g_engine;
    float sn = *(float*)(eng + 0x1136b1 + deg * 4);
    float cs = *(float*)(eng + 0x113c51 + deg * 4);
    ResetGlobals62b308();
    uint8_t rows = rec[0xb9];
    uint8_t cols = rec[0xb8];
    int32_t n = (rows > cols ? rows : cols) * 4 + 2;
    float half = (float)(n >> 1);
    int32_t ox = FtolD((double)fx - half);
    int32_t oy = FtolD((double)fy - half);
    // Stack order of the original: track, water, solid, shore.
    uint8_t scratch[4][0x400];
    uint8_t* grid[4] = { scratch[0], scratch[2], scratch[1], scratch[3] };
    for (int k = 0; k < 4; ++k) memset(grid[k], 0, (size_t)(n * n));
    uint8_t* m = g_map;
    bool clip = !(ox >= 0 && oy >= 0 && ox + n < F<int32_t>(m, 0x215d) && oy + n < F<int32_t>(m, 0x2161));
    int32_t shoreKinds = (F<int32_t>(rec, 0x17b) | F<int32_t>(rec, 0x17f)) != 0;
    static const uint32_t flagOff[4] = { 0x183, 0x177, 0x17b, 0x17f };
    static const uint32_t maskOff[4] = { 0xdb, 0xcf, 0xd3, 0xd7 };
    for (int32_t pass = 0; pass < 4; ++pass) {
        if (!F<int32_t>(rec, flagOff[pass])) continue;
        const uint16_t* mask = F<uint16_t*>(rec, maskOff[pass]);
        uint8_t* buf = grid[pass];
        int32_t word = 0;
        for (int32_t r = 0; r < rows; ++r, rows = rec[0xb9]) {
            for (int32_t c = 0; c < cols; ++c, cols = rec[0xb8]) {
                for (int32_t j = 0; j < 16; ++j, ++word) {
                    const uint16_t bits = mask[word];
                    if (!bits) continue;
                    float bx = (float)(((double)(float)(c * 4 + (j & 3)) - halfW) * Fl(0x5c8568));
                    float by = (float)(((double)(float)((j >> 2) + r * 4) - halfW) * Fl(0x5c8568));
                    for (int32_t bit = 15, k = -16, s = 0; k <= -1; --bit, ++k, ++s) {
                        if (!(mask[word] & (1 << bit))) continue;
                        float px = (float)((double)(float)(k & 3) + bx);
                        float pz = (float)((double)(float)(s >> 2) + by);
                        float rx = (float)((double)px * cs - (double)pz * sn);
                        float rz = (float)((double)pz * cs + (double)px * sn);
                        int32_t X = FtolD((double)rx / D(0x5c87b8) + cx);
                        int32_t Y = FtolD((double)cy - (double)rz / D(0x5c87b8));
                        int32_t cell = (Y - oy) * n - ox + X;
                        if (buf[cell]) continue;
                        m = g_map;
                        if (clip && (X < 0 || X >= F<int32_t>(m, 0x215d) || Y < 0 || Y >= F<int32_t>(m, 0x2161))) return 0;
                        buf[cell] = 1;
                        uint8_t* map = g_map;
                        bool preview;
                        if (trackOnly && pass == 0) {
                            if (commit) {
                                --bit, ++k, ++s;
                                continue;
                            }
                            preview = true;
                        } else {
                            preview = !commit;
                        }
                        if (preview) {
                            if (grid[0][cell]) {
                                Map_SetFineBit(map, 0, X, Y, 1, 1, 1);
                            } else if (grid[1][cell] || grid[2][cell] || grid[3][cell]) {
                                Map_SetFineBit(g_map, 0, X, Y, 1, 2, 1);
                                CallDirtyAdd(0, Y, X);
                            }
                            m = g_map;
                            F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * (Y >> 2) + (X >> 2)] |= 1;
                        } else {
                            uint8_t track = grid[0][cell];
                            if (track) {
                                if (!trackOnly && Map_CellFlag(map, 0, X, Y, 2)) return 0;
                            } else if (grid[1][cell]) {
                                if (Map_CellFlag(map, 0, X, Y, 0)) return 0;
                                map = g_map;
                                int32_t bad = shoreKinds ? Terrain_KindIs1Or3to5(map, 0, Quarter(X), Quarter(Y)) : Map_NearFeature(map, 0, Quarter(X), Quarter(Y));
                                if (bad) return 0;
                                if (F<int32_t>(types, 0x9c) == 1 || F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88)) {
                                    if (Map_CellFlag(g_map, 0, X, Y, 3)) return 0;
                                }
                            } else if (grid[2][cell]) {
                                if (!Terrain_KindIs1Or3to5(map, 0, Quarter(X), Quarter(Y))) return 0;
                                if (Map_CellFlag(g_map, 0, X, Y, 0)) return 0;
                            } else if (grid[3][cell]) {
                                if (Map_CellFlag(map, 0, X, Y, 0)) return 0;
                            }
                            CallDirtyAdd(track, Y, X);
                        }
                        --bit, ++k, ++s;
                    }
                }
            }
        }
    }
    return CallCheckPlacement(rec, *(uint32_t*)&x, *(uint32_t*)&y);
}
RT3_REPLACE(0x416ec0, Site_CheckFootprint);

// 0x417610 / 0x4176d0. Records named `name` (+0x22) whose +0xc3 has `bit`
// (-1 = any): count / ID of the n-th (1-based).
int32_t __fastcall Table_CountNamedWithBit(Collection* t, int, const char* name, int32_t bit) {
    char first = (char)ToUpper(name[0]);
    int32_t mask = bit == -1 ? -1 : 1 << (bit & 31);
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if ((char)ToUpper((char)r[0x22]) != first) continue;
        if (!(F<int32_t>(r, 0xc3) & mask)) continue;
        if (crt::stricmp((const char*)r + 0x22, name) == 0) ++n;
    }
    return n;
}
RT3_REPLACE(0x417610, Table_CountNamedWithBit);

uint32_t __fastcall Table_NthNamedWithBit(Collection* t, int, const char* name, int32_t bit, int32_t nth) {
    char first = (char)ToUpper(name[0]);
    int32_t mask = bit == -1 ? -1 : 1 << (bit & 31);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if ((char)ToUpper((char)r[0x22]) != first) continue;
        if (!(F<int32_t>(r, 0xc3) & mask)) continue;
        if (crt::stricmp((const char*)r + 0x22, name) != 0) continue;
        if (--nth <= 0) return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x4176d0, Table_NthNamedWithBit);

// 0x417790. Rotates (x, y) by `angle` (radians, via the engine's per-degree
// sin/cos tables), negated: out = -(c*x - s*y, s*x + c*y).
RT3_NOINLINE void __stdcall RotateNegated(float x, float y, float angle, float* outX, float* outY) {
    const float zero = *(const float*)0x5c8598;
    while (angle < zero) angle = (float)(angle + *(const double*)0x5c96b8);
    int32_t deg = (int32_t)((double)angle * *(const float*)0x5c96b0 / *(const double*)0x5c96a8) % 360;
    uint8_t* e = g_engine;
    float s = F<float>(e, 0x1136b1 + deg * 4);
    float c = F<float>(e, 0x113c51 + deg * 4);
    *outX = (float)-((double)c * x - (double)s * y);
    *outY = (float)-((double)s * x + (double)c * y);
}
RT3_REPLACE(0x417790, RotateNegated);

// 0x417840. Cell of record `id` (if it has one) relative to its footprint
// centre, rotated by `angle`. QUIRK: without a cell the original uses
// uninitialised stack (x) and the low word of `id` (y); here x = 0.
RT3_NOINLINE void __fastcall Type_RotatedCell(Collection* t, int, uint32_t id, float* outX, float* outY, float angle) {
    uint8_t* r = (uint8_t*)Collection_Get(t, 0, id);
    uint16_t cx = 0, cy = (uint16_t)id;
    Obj_GetCell(r, 0, &cx, &cy);
    const double c48 = *(const double*)0x5c96c8, c2 = *(const double*)0x5c85d0;
    float y = (float)((double)(int16_t)cy / c48 - (double)F<uint8_t>(r, 0xb9) / c2);
    float x = (float)((double)(int16_t)cx / c48 - (double)F<uint8_t>(r, 0xb8) / c2);
    RotateNegated(x, y, angle, outX, outY);
}
RT3_REPLACE(0x417840, Type_RotatedCell);

// 0x417910. Moves (*x, *z) to the attachment point of object `id`: its cell
// centre rotated by `angle`, then back (mode 0) or forward along the heading.
RT3_NOINLINE void __fastcall Obj_AttachmentPoint(Collection* c, int, int32_t mode, uint32_t id, float* x, float* z, float angle) {
    uint8_t* o = (uint8_t*)Collection_Get(c, 0, id);
    uint16_t cx, cy;
    Obj_GetCell(o, 0, &cx, &cy);
    const double k = D(0x5c96c8), two = D(0x5c85d0);
    float py = (float)((double)(int16_t)cy / k - (double)o[0xb9] / two);
    float px = (float)((double)(int16_t)cx / k - (double)o[0xb8] / two);
    RotateNegated(px, py, angle, x, z);
    if (!mode) {
        double t = *x;
        *x = (float)(t - crt::cos(angle) * D(0x5c96d8));
        t = *z;
        *z = (float)(t - crt::sin(angle) * D(0x5c96d8));
        return;
    }
    *x = (float)(crt::cos(angle) * D(0x5c96d0) + *x);
    *z = (float)(crt::sin(angle) * D(0x5c96d0) + *z);
}
RT3_REPLACE(0x417910, Obj_AttachmentPoint);

// 0x417a70. Footprint offset i of record `id`, rotated by `angle`:
// x = f3[i]/48 - b8/2, y = 10b[i]/48 - b9/2 (both floats), then RotateNegated.
RT3_NOINLINE void __fastcall Type_RotatedOffset(Collection* t, int, uint32_t id, int32_t i, float* outX, float* outY, float angle) {
    uint8_t* r = (uint8_t*)Collection_Get(t, 0, id);
    const double c48 = *(const double*)0x5c96c8, c2 = *(const double*)0x5c85d0;
    float y = (float)((double)F<int16_t>(r, 0x10b + i * 2) / c48 - (double)F<uint8_t>(r, 0xb9) / c2);
    float x = (float)((double)F<int16_t>(r, 0xf3 + i * 2) / c48 - (double)F<uint8_t>(r, 0xb8) / c2);
    RotateNegated(x, y, angle, outX, outY);
}
RT3_REPLACE(0x417a70, Type_RotatedOffset);

// 0x417b40. Snaps a station of type `type` (table this) for company `company` near
// (x, y) at `angle` onto the track: the nearest plain piece within 0.724 cells of
// the placement point (offset by the type's footprint) must be unclaimed or ours,
// with plain, unclaimed neighbours on both sides whose farNode pieces don't branch back.
// The station turns to the piece's heading (mod pi, flipped by pi when that is
// nearer the requested angle); *outX / *outY / *outAngle get the snapped placement
// (with `update` also used as the new reference). Accepted when the turn is under
// 0.25 rad and the move under 0.55 cells; *outNode (optional) gets the piece.
RT3_NOINLINE int32_t __fastcall Station_SnapToTrack(Collection* types, int, int32_t company, uint32_t type, float x, float y, float angle,
                                       float* outX, float* outY, float* outAngle, int32_t update, int32_t* outNode) {
    if (outNode) *outNode = -1;
    float ox, oy;
    Type_RotatedCell(types, 0, type, &ox, &oy, angle);
    Collection* nodes = g_tracks;
    int32_t id = Grid_NearestNodeWithin(nodes, 0, (float)((double)x - ox), (float)((double)oy + y), company, 0.724f, 1, 0, 0);
    if (id == -1) return 0;
    uint8_t* nd = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)id);
    if (F<int32_t>(nd, 0x226)) return 0;
    if (F<int32_t>(nd, 0x222) != -1 && F<int32_t>(nd, 0x222) != company) return 0;
    if (F<int32_t>(nd, 0x20e) != -1) return 0;
    for (int side = 0; side < 2; ++side) {
        int32_t nb = F<int32_t>(nd, side ? 0x20a : 0x206);
        if (nb == -1) return 0;
        uint8_t* a = (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)nb);
        if (!Station_PlainFor(a, company)) return 0;
        int32_t farNode = F<int32_t>(a, side ? 0x20a : 0x206);
        if (farNode == id) farNode = F<int32_t>(a, side ? 0x206 : 0x20a);
        if (farNode == -1) return 0;
        if (!Station_FarSideOk((uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)farNode), nb, company)) return 0;
    }
    float heading = (float)((double)Vehicle_ScreenHeading(nd, 0, 1) - D(0x5c8748));
    const double pi = 3.14159265359, twoPi = 6.2831854820251465;
    heading = (float)crt::fmod((double)heading + D(0x5c96f8), pi);
    float turn = (float)crt::fabs((double)angle - heading);
    if (turn > Fl_205(0x5c8760)) turn = (float)((double)Fl_205(0x5c8768) - turn);
    if (turn > D(0x5c96f0)) heading = (float)crt::fmod((double)heading + D(0x5c96a8), twoPi);
    Type_RotatedCell(types, 0, type, &ox, &oy, heading);
    *outX = (float)((double)Obj_CenterCellX(nd) + ox);
    *outY = (float)((double)Obj_CenterCellY(nd) - oy);
    *outAngle = heading;
    if (update) {
        x = *outX;
        y = *outY;
        angle = heading;
    }
    float d = (float)crt::fmod(((double)heading + Fl_205(0x5c96e8)) - angle, twoPi);
    if (d > Fl_205(0x5c8760)) d = (float)((double)Fl_205(0x5c8768) - d);
    if (d < 0.0f) d = 0.0f;
    if (d >= D(0x5c8758)) return 0;
    if (Distance2D(x, y, *outX, *outY) > D(0x5c96e0)) return 0;
    if (outNode) *outNode = id;
    return 1;
}
RT3_REPLACE(0x417b40, Station_SnapToTrack);

// 0x417f20. Scales the map cell rect in (x0, x1, y0, y1) to texture pixels (out)
// and extracts that part of the terrain texture.
RT3_NOINLINE uint32_t* __stdcall Map_ExtractTextureRect(const int32_t* in, int32_t* out) {
    float sx, sy;
    Map_PixelsPerTile(&sx, &sy);
    out[0] = (int32_t)((double)(float)in[0] * sx);
    out[1] = (int32_t)((double)(float)in[1] * sx);
    out[2] = (int32_t)((double)(float)in[2] * sy);
    out[3] = (int32_t)((double)(float)in[3] * sy);
    int32_t w, h;
    Render_GetSize4((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, &w, &h);
    float hf = (float)h, wf = (float)w;
    float y1 = (float)((double)(float)(out[3] + 1) / hf);
    float x1 = (float)((double)(float)(out[1] + 1) / wf);
    float y0 = (float)((double)(float)out[2] / hf);
    float x0 = (float)((double)(float)out[0] / wf);
    return Terrain_ExtractTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, x0, y0, x1, y1, nullptr,
                                  nullptr);
}
RT3_REPLACE(0x417f20, Map_ExtractTextureRect);

// 0x418040. Scorches the map texture area bounding points `pts` (tiles
// packed y << 16 | x, -1 ends; `listedOnly` limits it to those tiles),
// skipping tiles near features (0x62c120): each pixel, by its randomised
// distance from the centre, is pulled toward the inner (0x5edd24) or, unless
// `innerOnly`, the outer (0x5edd20) colour, less so for bright pixels.
// Returns a malloc'ed copy of the old pixels (size 0x62b7cc), or null.
RT3_NOINLINE uint32_t* __fastcall Map_ScorchTexture(uint8_t* o, int, const uint32_t* pts, int32_t listedOnly, int32_t innerOnly) {
    int32_t in[4], r[4];
    Points_Bounds(o, 0, pts, in);
    uint32_t* p = Map_ExtractTextureRect(in, r);
    if (!p) return nullptr;
    uint32_t size = (uint32_t)((r[1] - r[0] + 1) * (r[3] - r[2] + 1) * 4);
    if (size <= 0x10) {
        Terrain_CommitTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, 0);
        return nullptr;
    }
    g_62b7cc = size;
    uint32_t* copy = ((uint32_t*)crt::malloc(size));
    memcpy(copy, p, size);
    float sy, sx;
    Map_PixelsPerTile(&sx, &sy);
    int32_t x0 = r[0], x1 = r[1], y0 = r[2], y1 = r[3];
    int32_t cx = (x0 + x1) >> 1, cy = (y1 + y0) >> 1;
    float d1 = (float)((cy - y0) * (cy - y0) + (cx - x0) * (cx - x0));
    float d2 = (float)((cy - y1) * (cy - y1) + (cx - x1) * (cx - x1));
    float maxd = d1 > d2 ? d1 : d2;
    if (Fl(0x5c85b4) > maxd) maxd = 1.0f;
    for (int32_t y = y0, dy = y0 - cy; y <= y1; ++y, ++dy) {
        for (int32_t x = x0, dx = x0 - cx; x <= x1; ++x, ++dx, ++p) {
            float fy = (float)y, fx = (float)x;
            int32_t ty = Ftol(fy / sy);
            int32_t tx = Ftol(fx / sx);
            if (listedOnly == 1) {
                uint32_t key = (uint32_t)ty << 16 | (uint32_t)tx;
                const uint32_t* e = pts;
                while (*e != 0xffffffff && (*e & 0x7fffffff) != key) ++e;
                if (*e == 0xffffffff) continue;
            }
            if (Map_NearFeature(g_map, 0, (int32_t)((uint32_t)tx >> 2), (int32_t)((uint32_t)ty >> 2))) continue;
            int32_t dist2 = dx * dx + dy * dy;
            uint32_t seed = g_lcg * 0x19660d + 0x3c6ef35f;
            g_lcg = seed;
            int32_t rnd = (int32_t)(seed & 0x7fff) % 10 + 1;
            float fd = (float)dist2;
            float frnd = (float)rnd;
            float v = (float)(fd - Fl(0x5c85b4) / frnd * fd);
            float ratio = v / maxd;
            if (!(ratio < Fl(0x5c970c))) continue;
            uint32_t c = *p;
            int32_t b0 = c & 0xff, b1 = (c >> 8) & 0xff, b2 = (c >> 16) & 0xff, b3 = c >> 24;
            float sum = (float)(b0 + b1 + b2);
            float bright = (float)(Fl(0x5c85b4) - sum / Fl(0x5c9708));
            if (Fl(0x5c9704) > bright) bright = 0.25f;
            const uint8_t* tc;
            if (ratio < Fl(0x5c9700)) {
                tc = (const uint8_t*)0x5edd24;
            } else {
                if (innerOnly == 1) continue;
                tc = (const uint8_t*)0x5edd20;
            }
            float k = (float)(Fl(0x5c85b4) - fd / maxd);
            float o3 = (float)b3, e3 = (float)(b3 - tc[3]);
            uint32_t out = (uint32_t)Ftol(o3 - e3 * k * bright) << 8;
            float o2 = (float)b2, e2 = (float)(b2 - tc[0]);
            out = (out | (uint32_t)Ftol(o2 - e2 * k * bright)) << 8;
            float o1 = (float)b1, e1 = (float)(b1 - tc[1]);
            out = (out | (uint32_t)Ftol(o1 - e1 * k * bright)) << 8;
            float o0 = (float)b0, e0 = (float)(b0 - tc[2]);
            *p = out | (uint32_t)Ftol(o0 - e0 * k * bright);
        }
    }
    Terrain_CommitTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, 0);
    return copy;
}
RT3_REPLACE(0x418040, Map_ScorchTexture);

// 0x4184e0. Pastes picture `pixels` (freed here) over the map texture area
// bounding points `pts`, unless the game (0x6cec74 +0x10 / +0x4fd) or the
// world (+0x4c93) forbids it.
RT3_NOINLINE void __fastcall Map_ApplyTexturePatch(uint8_t* o, int, const uint32_t* pts, uint32_t* pixels) {
    if (!pixels) return;
    uint8_t* s = g_app;
    uint8_t* w = g_world;
    if (!F<int32_t>(s, 0x10) && !F<int32_t>(s, 0x4fd) && !(w && F<int32_t>(w, 0x4c93))) {
        int32_t in[4], out[4];
        Points_Bounds(o, 0, pts, in);
        if (uint32_t* dst = Map_ExtractTextureRect(in, out)) {
            int32_t n = (out[1] - out[0] + 1) * (out[3] - out[2] + 1);
            memcpy(dst, pixels, (size_t)(n * 4));
            Terrain_CommitTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, 0);
        }
    }
    crt::free(pixels);
}
RT3_REPLACE(0x4184e0, Map_ApplyTexturePatch);

// 0x4185a0. Sets bit 0 of the map's mask grid (+0x2135) to `bit` over a rect
// {x0, x1, y0, y1}, skipping cells whose value is 999999.0f (0x497423f0).
RT3_NOINLINE void __fastcall Map_MaskRect(void*, int, const int32_t* r, const uint32_t* values, uint8_t bit) {
    int32_t i = 0;
    for (int32_t y = r[2]; y <= r[3]; ++y) {
        for (int32_t x = r[0]; x <= r[1]; ++x, ++i) {
            if (values[i] == 0x497423f0) continue;
            uint8_t* map = *(uint8_t* volatile*)&g_map;  // read for every cell, as in the original
            uint8_t* cell = F<uint8_t*>(map, 0x2135) + (F<int32_t>(map, 0x2155) + 1) * y + x;
            *cell ^= (*cell ^ bit) & 1;
        }
    }
}
RT3_REPLACE(0x4185a0, Map_MaskRect);

// 0x418610. Site manager mgr: prepares the ground for site `id` on the pending
// footprint (0x62b7a8 { x0, x1, y0, y1 }). With `force` (or 0x62b304 clear) it
// must lie inside the map and pass Site_CheckFlatness (0 otherwise). With
// `level` the old heights are saved (0x62b7d0, count 0x62b964) and - except
// for steep class-3 or special (+0xba) types - the ground is levelled: by
// Site_LevelGround at the given height (0x62b7c4, else 0x62b7c0) when there
// is no shape (0x62ba3a -1), else every unfixed cell with structure gets the
// footprint centre's terrain height (rounded for v1.02 peers), with its
// right / lower neighbours. *height (when given) receives the height. Then
// the rect is announced; with `mask` it is masked with the saved heights.
// Returns 1 (also for an unset footprint).
RT3_NOINLINE int32_t __fastcall Sites_PrepareGround(uint8_t* mgr, int, uint32_t id, int32_t level, int32_t mask, int32_t force, uint32_t* height) {
    uint8_t* site = (uint8_t*)Collection_Get((Collection*)mgr, 0, id);
    if (height) *height = 0;
    int32_t* r = (int32_t*)0x62b7a8;
    auto unset = [&] {
        float f = (float)r[1];
        return *(uint32_t*)&f == 0xc97423f0;
    };
    if (force == 1 || !g_62b304) {
        if (unset()) return 1;
        if (r[0] <= 0 || r[1] <= 0 || r[2] <= 0 || r[3] <= 0) return 0;
        uint8_t* m = g_map;
        uint32_t w = (uint32_t)(F<int32_t>(m, 0x2155) - 1), hgt = (uint32_t)(F<int32_t>(m, 0x2159) - 1);
        if ((uint32_t)r[0] >= w || (uint32_t)r[1] >= w || (uint32_t)r[2] >= hgt || (uint32_t)r[3] >= hgt) return 0;
        int32_t alt = Obj_TypeKind1Class3(site) || Obj_TypeByteBa(site) ? 1 : 0;
        if (Site_CheckFlatness(alt, Obj_TypeKindIs1(site)) != 1) return 0;
    }
    if (unset()) return 1;
    if (level == 1) {
        uint32_t h = g_62b7c4 != 0x497423f0 ? g_62b7c4 : hook::As<uint32_t>(g_62b7c0);
        g_62b964 = 0;
        for (int32_t y = r[2]; y <= r[3]; ++y)
            for (int32_t x = r[0]; x <= r[1]; ++x)
                ((float*)0x62b7d0)[g_62b964++] = (float)Map_GridFloat(g_map, 0, x, y);
        uint8_t* type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(site, 0x173));
        bool skip = type[0x32] == 1 && type[0x8c] == 3;
        if (!skip) skip = ((uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(site, 0x173)))[0xba] != 0;
        if (!skip) {
            if (g_62ba3a == -1)
                Site_LevelGroundImpl((int32_t)id, r, BitsF(h), 1, 1);
            else if (!g_6ce9a8 || !Map_RectHasFlag1Impl(r)) {
                float cy = (float)((r[3] + r[2]) >> 1);
                float cx = (float)((r[0] + r[1]) >> 1);
                float g = (float)Terrain_HeightAtTile(g_map, 0, cx, cy);
                memcpy(&h, &g, 4);
                for (int32_t y = r[2]; y <= r[3]; ++y)
                    for (int32_t x = r[0]; x <= r[1]; ++x) {
                        uint8_t* m = g_map;
                        if (F<uint8_t*>(m, 0x2135)[(F<int32_t>(m, 0x2155) + 1) * y + x] & 1) continue;
                        if (Map_BlockHasFlag2Impl(x, y) != 1) continue;
                        int32_t right = 0;
                        if (IsMultiplayer() && Net_AllHaveVersion(BitsF(0x3f828f5c))) {
                            float v = (float)Ftol((double)BitsF(h));
                            memcpy(&h, &v, 4);
                        }
                        Map_SetCellHeight(g_map, 0, x, y, h, 0);
                        if (x <= r[1]) {
                            Map_SetCellHeight(g_map, 0, x + 1, y, h, 0);
                            right = 1;
                        }
                        if (y <= r[3]) {
                            Map_SetCellHeight(g_map, 0, x, y + 1, h, 0);
                            if (right == 1) Map_SetCellHeight(g_map, 0, x + 1, y + 1, h, 0);
                        }
                    }
            }
            if (height) *height = h;
        }
        Map_CellsChangedImpl(g_map, 0, r[0], r[2], r[1], r[3]);
    }
    g_62b968 = mask;
    if (mask == 1) Map_MaskRect(mgr, 0, r, (const uint32_t*)0x62b7d0, 1);
    return 1;
}
RT3_REPLACE(0x418610, Sites_PrepareGround);

// 0x418a60. Site manager mgr: recreates the next saved site record (0x62b2f8
// array, index 0x62ba74 counted up) of type `id` for object o: a pool record
// copied from it (the saved one gets o in +0x742), its dirty cells applied,
// a shaped one restamped with its static record (+0x736), its ground mask
// redone, then the visit stamp cleared, the object / owner stored and the
// record added to the cell hash. Returns the record.
// QUIRK: o's position is read (scaled by 0x5c8700) but unused.
RT3_NOINLINE uint8_t* __fastcall Sites_Restore(uint8_t* mgr, int, uint32_t id, uint8_t* o, int32_t) {
    uint8_t* saved = hook::As<uint8_t*>(g_62b2f8) + g_62ba74 * 0x74e;
    float px, py, pz;
    SceneObj_GetPos(o, 0, (uint32_t*)&px, (uint32_t*)&pz, (uint32_t*)&py);
    px = px / Fl_207(0x5c8700);
    py = py / Fl_207(0x5c8700);
    if (!saved) return nullptr;
    uint8_t* pool = F<uint8_t*>(mgr, 0x8c);
    uint8_t* s;
    if (F<int32_t>(pool, 0xc)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        s = F<uint8_t*>(list, 0);
        if (s) {
            uint8_t* next = F<uint8_t*>(s, 0);
            bool last = s == F<uint8_t*>(list, 4);
            F<uint8_t*>(list, 0) = next;
            if (last) F<uint8_t*>(list, 4) = next;
            --F<int32_t>(list, 8);
        }
    } else
        s = (uint8_t*)crt::operator_new(F<uint32_t>(pool, 0x10));
    memcpy(s, saved, 0x74e);
    F<uint8_t*>(saved, 0x742) = o;
    uint32_t* cursor;
    DirtyCells_ApplyImpl(s, &cursor);
    if (F<int32_t>(s, 0x732) != -1) {
        F<void*>(s, 0x736) = FindStaticRecord(nullptr, 0, (uint8_t*)Collection_Get((Collection*)mgr, 0, id));
        Footprint_StampImpl(s, (int32_t)(uintptr_t)s);
    }
    Map_MaskRect(mgr, 0, (const int32_t*)(s + 0x4a0), (const uint32_t*)(s + 0x4c8), 1);
    F<int32_t>(s, 0x73e) = 0;
    F<uint8_t*>(s, 0x742) = o;
    F<uint32_t>(s, 0x74a) = F<uint32_t>(o, 0x2a4);
    uint8_t* type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(Collection_Get((Collection*)mgr, 0, id), 0x173));
    int32_t flag = type[0x32] == 1 && type[0x8c] == 3 ? 1 : 0;
    Sites_AddToCellHashImpl(F<HashMap*>(mgr, 0x88), (const int32_t*)(s + 0x4a0), F<uint8_t*>(mgr, 0x90), s, flag);
    ++g_62ba74;
    return s;
}
RT3_REPLACE(0x418a60, Sites_Restore);

// 0x418be0. Site manager mgr: creates the site of type `name` at (x, y)
// turned by `angle` for object o: a record from the pool (+0x8c) filled from
// the pending site 0x62b308 after checking the footprint, stamping a shaped
// one, preparing (levelling, masking) its ground (height to *height) and
// scorching the dirty cells (0x62b7c8); then the position, object (+0x742)
// and its owner (+0x74a) are stored and a placed site joins the cell hash
// (+0x88). Returns the record (0 for an unknown type).
RT3_NOINLINE uint8_t* __fastcall Sites_Create(uint8_t* mgr, int, const char* name, float x, float y, float angle, uint8_t* o, uint32_t* height) {
    if (height) *height = 0;
    uint8_t* s = nullptr;
    int32_t id = Collection_FindByName4((Collection*)mgr, 0, name);
    if (id == -1) return nullptr;
    uint8_t* pool = F<uint8_t*>(mgr, 0x8c);
    if (F<int32_t>(pool, 0xc)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        s = F<uint8_t*>(list, 0);
        if (s) {
            uint8_t* next = F<uint8_t*>(s, 0);
            bool last = s == F<uint8_t*>(list, 4);
            F<uint8_t*>(list, 0) = next;
            if (last) F<uint8_t*>(list, 4) = next;
            --F<int32_t>(list, 8);
        }
    } else
        s = (uint8_t*)crt::operator_new(F<uint32_t>(pool, 0x10));
    Site_CheckFootprint(mgr, 0, (uint32_t)id, x, y, angle, 0, 1);
    if (g_62ba3a != -1) Footprint_StampImpl(nullptr, (int32_t)(uintptr_t)s);
    Sites_PrepareGround(mgr, 0, (uint32_t)id, 1, 1, 0, height);
    if (g_62b7a4)
        g_62b7c8 = Map_ScorchTexture(mgr, 0, (const uint32_t*)0x62b318, 0, 0);
    memcpy(s, (const void*)0x62b308, 0x74e);
    F<float>(s, 0xc) = angle;
    F<float>(s, 8) = y;
    F<float>(s, 4) = x;
    F<uint8_t*>(s, 0x742) = o;
    F<uint32_t>(s, 0x74a) = F<uint32_t>(o, 0x2a4);
    float x1 = (float)F<int32_t>(s, 0x4a4);
    if (*(uint32_t*)&x1 != 0xc97423f0) {
        uint8_t* type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(Obj_Ref3cc(o), 0x173));
        int32_t flag = type[0x32] == 1 && type[0x8c] == 3 ? 1 : 0;
        Sites_AddToCellHashImpl(F<HashMap*>(mgr, 0x88), (const int32_t*)(s + 0x4a0), F<uint8_t*>(mgr, 0x90), s, flag);
    }
    return s;
}
RT3_REPLACE(0x418be0, Sites_Create);

// 0x418d40. Site manager mgr: frees site s. Closes the trace file
// (0x62b2f4); restores the ground texture under it (+0x49c / +0x4c0) and -
// unless editing / replaying (options +0x10 / +0x4fd, world +0x4c93) - its
// fine map bits (+0x10). A placed site (+0x4a4 != -999999) releases its
// cells and, unless editing, restores the masked rect (+0x660), the heights
// (mgr +0x98, +0x4c8) and terrain (+0x664 / +0x6c9, when shaped +0x732) and
// announces the change. The record is zeroed and returned to the pool
// (mgr +0x8c) when it came from it, else deleted. Redraws the overview
// when the map wants it (+0x159b).
RT3_NOINLINE void __fastcall Sites_Free(uint8_t* mgr, int, uint8_t* s) {
    if (g_62b2f4) {
        crt::fclose(g_62b2f4);
        hook::As<uint32_t>(g_62b2f4) = 0;
    }
    if (!s) return;
    auto quiet = [] {
        uint8_t* opt = g_app;
        return !F<int32_t>(opt, 0x10) && (!g_world || !F<int32_t>(g_world, 0x4c93)) &&
               !F<int32_t>(opt, 0x4fd);
    };
    if (F<int32_t>(s, 0x49c)) {
        Map_ApplyTexturePatch(mgr, 0, (const uint32_t*)(s + 0x10), F<uint32_t*>(s, 0x4c0));
        F<uint32_t>(s, 0x4c0) = 0;
    }
    if (quiet()) {
        Map_SetFineBitList((const uint32_t*)(s + 0x10), 0);
        F<int32_t>(s, 0x10) = -1;
        F<int32_t>(s, 0x49c) = 0;
    }
    float placed = (float)F<int32_t>(s, 0x4a4);
    if (*(uint32_t*)&placed != 0xc97423f0) {
        int32_t* r = (int32_t*)(s + 0x4a0);
        Site_ReleaseCells(r, F<HashMap*>(mgr, 0x88), F<uint32_t*>(mgr, 0x90), s);
        if (quiet()) {
            if (F<int32_t>(s, 0x660) == 1) Map_MaskRect(mgr, 0, r, (const uint32_t*)(s + 0x4c8), 0);
            if (F<int32_t>(mgr, 0x98) == 1) {
                Map_SetHeightsInRect((const uint32_t*)(s + 0x4c8), r);
                Terrain_RestoreRectAt(s + 0x664, r, s + 0x6c9);
            } else if (F<int32_t>(s, 0x732) != -1)
                Terrain_RestoreRectAt(s + 0x664, r, s + 0x6c9);
            Map_CellsChangedImpl(g_map, 0, r[0] - 1, r[2] - 1, r[1] - 1, r[3] - 1);
        }
        F<int32_t>(s, 0x4a4) = (int32_t)0xfff0bdc1;
    }
    memset(s, 0, 0x1d3 * 4 + 2);
    uint8_t* pool = F<uint8_t*>(mgr, 0x8c);
    if (F<int32_t>(pool, 0xc) && s >= F<uint8_t*>(pool, 4) && s < F<uint8_t*>(pool, 8)) {
        uint8_t* l = F<uint8_t*>(pool, 0);
        F<uint8_t*>(s, 0) = F<uint8_t*>(l, 0);
        F<uint8_t*>(l, 0) = s;
        ++F<int32_t>(l, 8);
        if (!F<void*>(l, 4)) F<uint8_t*>(l, 4) = s;
    } else
        crt::operator_delete(s);
    if (quiet() && g_map[0x159b]) Overview_Redraw(g_map, 0, nullptr);
}
RT3_REPLACE(0x418d40, Sites_Free);

// 0x418f60. Terrain tool t: flattens the square of radius r around (x, y)
// in up to twice the options' smoothing passes (+0x1eb): each pass tracks the
// height range (+0xa0 / +0xa4) and, while it spans more than 110, smooths
// every map cell of the square once and announces it.
// QUIRK: the range's maximum is also written to +0xa0 (the minimum), and
// the announced rectangle starts one row below the square.
RT3_NOINLINE void __fastcall TerrainTool_Flatten(uint8_t* t, int, int32_t x, int32_t y, int32_t r) {
    int32_t passes = F<int32_t>(g_app, 0x1eb) * 2;
    if (!passes) return;
    int32_t y0 = y - r, y1 = y + r;
    do {
        --passes;
        uint8_t* m = g_map;
        for (int32_t cy = y0; cy <= y1; ++cy)
            for (int32_t cx = x - r; cx <= x + r; ++cx) {
                if (cx < 0 || cy < 0 || cx >= F<int32_t>(m, 0x2155) - 1 || cy >= F<int32_t>(m, 0x2159) - 1) continue;
                float h = (float)Map_GridFloat(m, 0, cx, cy);
                if (h < F<float>(t, 0xa0)) F<float>(t, 0xa0) = h;
                if (h > F<float>(t, 0xa4)) F<float>(t, 0xa0) = h;
                m = g_map;
            }
        if (crt::fabs((double)F<float>(t, 0xa4) - F<float>(t, 0xa0)) <= D(0x5c9710)) return;
        m = g_map;
        for (int32_t cy = y0; cy <= y1; ++cy)
            for (int32_t cx = x - r; cx <= x + r; ++cx) {
                if (cx < 0 || cy < 0 || cx >= F<int32_t>(m, 0x2155) - 1 || cy >= F<int32_t>(m, 0x2159) - 1) continue;
                Map_SmoothHeights(m, 0, cx, cy, 1, 0);
                m = g_map;
            }
        Map_CellsChangedImpl(m, 0, x - r - 1, y0 + 1, x + r + 1, y1 + 1);
    } while (passes);
}
RT3_REPLACE(0x418f60, TerrainTool_Flatten);

// 0x419110. Re-places every building registered in the cell hash +0x88 (key
// y << 16 | x, nodes {next, record}; record +4 / +8 position, +0x742 building)
// within `r` cells of (x, y), clamped to the map.
RT3_NOINLINE void __fastcall Map_RefreshBuildingsAround(uint8_t* o, int, int32_t x, int32_t y, int32_t r) {
    uint8_t* map = g_map;
    int32_t x0 = x - r < 0 ? 0 : x - r;
    int32_t y0 = y - r < 0 ? 0 : y - r;
    int32_t x1 = F<int32_t>(map, 0x2155) - 1;
    if (x1 >= x + r) x1 = x + r;
    int32_t y1 = F<int32_t>(map, 0x2159) - 1;
    if (y1 >= y + r) y1 = y + r;
    for (int32_t yy = y0; yy <= y1; ++yy)
        for (int32_t xx = x0; xx <= x1; ++xx)
            for (uint32_t* n = (uint32_t*)(uintptr_t)Hash_Get(F<HashMap*>(o, 0x88), 0, (uint32_t)(yy << 16) | (uint32_t)(xx & 0xffff)); n;
                 n = (uint32_t*)(uintptr_t)n[0]) {
                uint8_t* rec = (uint8_t*)(uintptr_t)n[1];
                Building_MoveTo(F<uint8_t*>(rec, 0x742), 0, F<float>(rec, 4), F<float>(rec, 8), 0, 0.0f);
            }
}
RT3_REPLACE(0x419110, Map_RefreshBuildingsAround);

// 0x419200. Per-cell value from the buffer 0x62b300 (0 if absent).
int32_t __stdcall CellBuffer_Get(int32_t x, int32_t y) {
    int32_t* b = g_cellOwners;
    if (!b) return 0;
    return b[F<int32_t>(g_map, 0x2155) * y + x];
}
RT3_REPLACE(0x419200, CellBuffer_Get);

// 0x419230. Rebuilds the 12 port (pass 0) and warehouse (pass 1) building types
// (0x1a7 bytes; template +0x187, industry +0x173): reuses or creates (vtable slot 1)
// the one for each template by cloning the last of its kind with its four tile
// tables (+0xcf.. when +0x177.., w +0xb8 x h +0xb9 x 32 bytes), names it and links
// the industry of the same name.
RT3_NOINLINE void __fastcall BuildingTypes_RebuildPortsWarehouses(Collection* c) {
    if (!World() || !g_62b268) return;
    for (int32_t pass = 0; pass < 2; ++pass)
        for (int32_t k = 0; k < 0xc; ++k) {
            int32_t exact = -1, any = -1;
            for (int32_t i = 0; i < Collection_Count(c); ++i) {
                uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
                uint8_t* ind = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(e, 0x173));
                if (!(pass == 0 ? ind[0xba] : pass == 1 && ind[0xbb])) continue;
                any = Collection_IdAt(c, 0, i, 0);
                if (F<int32_t>(e, 0x187) == k) exact = Collection_IdAt(c, 0, i, 0);
            }
            if (exact == -1) {
                uint8_t tmp[0x1a8];
                memcpy(tmp, Collection_Get(c, 0, any), 0x1a7);
                exact = ((int32_t(__thiscall*)(Collection*, void*, int32_t, int32_t, int32_t))(*(void***)c)[1])(c, tmp, 0, 0, 0);
            }
            uint8_t* src = (uint8_t*)Collection_Get(c, 0, any);
            uint8_t* dst = (uint8_t*)Collection_Get(c, 0, exact);
            if (exact != any) {
                memcpy(dst, src, 0x1a7);
                for (int t = 0; t < 4; ++t) {
                    if (!F<int32_t>(src, 0x177 + t * 4)) continue;
                    uint32_t size = (uint32_t)dst[0xb9] * dst[0xb8] * 32;
                    void* p = crt::operator_new(size);
                    F<void*>(dst, 0xcf + t * 4) = p;
                    memcpy(p, F<void*>(src, 0xcf + t * 4), size);
                }
            }
            F<int32_t>(dst, 0x187) = k;
            crt::sprintf((char*)dst + 0x22, (const char*)(pass ? 0x5c93d8 : 0x5c93e8), k);
            strcpy((char*)dst + 4, (const char*)dst + 0x22);
            int32_t link = -1;
            for (int32_t i = 0; i < Collection_Count(g_62b268); ++i) {
                Collection* inds = g_62b268;
                if (!crt::stricmp((const char*)dst + 0x22, (const char*)Collection_Get(inds, 0, Collection_IdAt(inds, 0, i, 0)) + 4))
                    link = Collection_IdAt(g_62b268, 0, i, 0);
            }
            F<int32_t>(dst, 0x173) = link;
        }
}
RT3_REPLACE(0x419230, BuildingTypes_RebuildPortsWarehouses);

// 0x419590. The id of the record of c named after the composed name (format
// 0x5c8730 from the tables 0x5f3c80[a] and 0x5f3c6c[b]); 0 when none.
RT3_NOINLINE uint32_t __fastcall Records_FindComposedName(Collection* c, int, int32_t a, int32_t b) {
    char name[0x12c], composed[0x12c];
    FormatText(name, ((const char* const*)0x5f3c6c)[b], 0, 0, 0, 0, 0, 0, 0, 0);
    FormatText(composed, (const char*)0x5c8730, ((const char* const*)0x5f3c80)[a], name, 0, 0, 0, 0, 0, 0);
    FormatText(name, composed, 0, 0, 0, 0, 0, 0, 0, 0);
    for (int32_t i = 0; i < Collection_Count(c); ++i)
        if (!crt::stricmp(name, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0)) + 4)) return Collection_IdAt(c, 0, (uint32_t)i, 0);
    return 0;
}
RT3_REPLACE(0x419590, Records_FindComposedName);

// 0x419680. Destructor of the site table (vtable 0x5c9718): removes all sites, frees
// the grid 0x62b2f8, then the collection.
RT3_NOINLINE void __fastcall Sites_Destruct(Collection* c) {
    F<uint32_t>(c, 0) = 0x5c9718;
    Sites_DestroyAll(c);
    if (g_62b2f8) {
        crt::free(g_62b2f8);
        g_62b2f8 = nullptr;
    }
    Collection_Destruct(c);
}
RT3_REPLACE(0x419680, Sites_Destruct);

// 0x4196c0. Building type table t: loads every .\Data\BuildingTypes\*.bty
// (BuildingType_LoadFootprint into a 0x1a7-byte record, added when it
// parses), pumping the loading screen per file, then rebuilds the port and
// warehouse lists.
RT3_NOINLINE void __fastcall BuildingTypes_LoadFiles(Collection* t) {
    uint8_t rec[0x1a7] = {};
    char pattern[0x168], path[0xc8];
    uint8_t fd[0x140];
    FormatText(pattern, (const char*)0x5c9724, (const char*)0x5c93fc, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    void* h = (((void*(__stdcall*)(const char*, void*))imp::FindFirstFileA))(pattern, fd);
    if (h != (void*)-1) {
        int32_t more;
        do {
            FormatText(path, (const char*)0x5c8730, (const char*)0x5c93fc, (const char*)fd + 0x2c, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            memset(rec, 0, sizeof rec);
            if (BuildingType_LoadFootprint(t, 0, rec, path)) t->vtbl->allocate(t, rec, 0, 0, 0);
            more = (imp::FindNextFileA)(h, fd);
            Loading_Pump(g_app, 0, 0, 0);
        } while (more);
        (imp::FindClose)(h);
    }
    BuildingTypes_RebuildPortsWarehouses(t);
}
RT3_REPLACE(0x4196c0, BuildingTypes_LoadFiles);

// 0x4197e0. Site manager mgr: can building type `id` be placed at (x, y)
// turned by `angle`? (1 yes, 0 no; the reason goes to `msg` when given,
// the short form with `shortMsg`, else the pending site message 0x62b270.)
// Counts the check in 0x62b304. Checks, in order: a city's (`city`) region
// rules; every footprint cell of the four layers (+0x183 / +0x177 / +0x17b /
// +0x17f, 16-bit masks +0xdb.. of 4x4 sub cells, turned) - inside the map,
// free of track / water as the layer needs (with `editor` the base layer is
// not checked) - collecting them as dirty cells; Site_CheckPlacement;
// Sites_PrepareGround; for stations (kind 1): snapping to track (`snap`),
// the track piece's owner (`onTrack`, `trackId`) or the city's size and no
// track node near the three attachment points with clear ground around
// them; for kind 1 / class 3 (inside the map's 3-cell border) a level
// enough ground at the eight corners; for shore types (+0xba) land and water
// around the cell.
// QUIRK: after a sub cell is taken the next bit is skipped. QUIRK: both
// footprint axes are centred by the x half size.
// QUIRK: the 4N+2 square scratch masks do not hold every footprint (turned
// near 45 degrees, or taller than wide given the x centring); such cells
// index outside a mask - into the next one or the locals below the first.
RT3_NOINLINE int32_t __fastcall Sites_CanPlace(Collection* mgr, int, uint32_t id, float x, float y, float angle, int32_t city, int32_t editor, char* msg,
                                  int32_t shortMsg, int32_t onTrack, int32_t trackId, int32_t snap) {
    int32_t ret = 1;
    uint8_t* btype = (uint8_t*)Collection_Get(g_buildingTypes, 0, id);
    ++g_62b304;
    if (msg) *msg = 0;
    auto copy = [&](const char* s) { strcpy(msg, s); };
    auto fail = [&](int32_t sid) {
        ret = 0;
        if (msg) copy(GetString(sid));
        return 0;
    };
    auto failSite = [&](int32_t shortId, int32_t sid) {
        ret = 0;
        if (!msg) return 0;
        if (shortMsg) copy(GetString(shortId));
        else if (*(const char*)0x62b270) copy((const char*)0x62b270);
        else copy(GetString(sid));
        return 0;
    };
    auto failRegion = [&](uint32_t t) {
        ret = 0;
        if (msg)
            FormatText(msg, GetString(0xb54), (const char*)Collection_Get(g_territories, 0, t) + 4, 0, 0, 0, 0, 0, 0, 0);
        return 0;
    };
    uint8_t* cityObj = nullptr;
    // A city's region rules: the cell's region must be allowed (unless water).
    auto regionBlocks = [&](float px, float py, int32_t cxi, int32_t cyi, uint32_t* t) {
        if (Global6d07b4Set2() || F<uint8_t>(g_newGame, 0x82)) return false;
        if (!Map_Contains(g_map, 0, px, py)) return false;
        if (Terrain_KindIs1Or3to5(g_map, 0, cxi, cyi)) return false;
        *t = Map_RegionTypeAt(g_map, 0, px, py);
        return !Company_EntryFlag(cityObj, 0, (int32_t)*t);
    };
    if (city > 0 && (cityObj = (uint8_t*)Collection_Get(g_companies, 0, (uint32_t)city)) != nullptr) {
        if (!Global6d07b4Set2() && !F<uint8_t>(g_newGame, 0x82) && Map_Contains(g_map, 0, x, y)) {
            int32_t yi = Ftol(y);
            if (!Terrain_KindIs1Or3to5(g_map, 0, Ftol(x), yi)) {
                uint32_t t = Map_RegionTypeAt(g_map, 0, x, y);
                if (!Company_EntryFlag(cityObj, 0, (int32_t)t)) return failRegion(t);
            }
        }
    }
    float ang = angle;
    uint8_t* st = (uint8_t*)Collection_Get(mgr, 0, id);
    float fx = (float)(x * Fl_207(0x5c8568));
    double dW = (double)(st[0xb8] << 2) / D(0x5c85d0);
    float left = (float)(fx - dW);
    double dH = (double)(st[0xb9] << 2) / D(0x5c85d0);
    float fy = (float)(y * Fl_207(0x5c8568));
    float bottom = (float)(fy + dH);
    if (angle < g_5c8598) do
            ang = (float)(ang + D(0x5c96b8));
        while (ang < g_5c8598);
    // The scratch masks and the locals below them live in one block laid out
    // like the original frame (ESP offsets), so footprint cells that index
    // outside a mask hit the same values (see the QUIRK above).
    alignas(8) uint8_t fr[0x1090];
#define FR(T, off) (*(T*)(fr + (off)))
    FR(uint8_t*, 0x6c) = btype;
    FR(float, 0x68) = (float)dW;
    float halfH = (float)dH;
    FR(float, 0x64) = (float)(FR(float, 0x68) + left);
    FR(float, 0x40) = (float)(bottom - halfH);
    int32_t ai = Ftol(ang * Fl_207(0x5c96b0) / D(0x5c96a8)) % 0x168;
    uint8_t* eng = g_engine;
    FR(float, 0x74) = *(float*)(eng + 0x1136b1 + ai * 4);
    FR(float, 0x7c) = *(float*)(eng + 0x113c51 + ai * 4);
    FR(uint8_t*, 0x80) = st;
    FR(uint8_t*, 0x58) = cityObj;
    ResetGlobals62b308();
    uint8_t W = st[0xb8], H = st[0xb9];
    FR(int32_t, 0x78) = (H < W ? W : H) * 4 + 2;
    float half = (float)(FR(int32_t, 0x78) >> 1);
    FR(int32_t, 0x48) = Ftol(fx - half);
    FR(int32_t, 0x84) = Ftol(fy - half);
    static const uint32_t bufOff[4] = { 0x890, 0x90, 0x490, 0xc90 };
    for (int k = 0; k < 4; ++k) memset(fr + bufOff[k], 0, (size_t)(FR(int32_t, 0x78) * FR(int32_t, 0x78)));
    uint8_t* m = g_map;
    FR(int32_t, 0x5c) = FR(int32_t, 0x48) < 0 || FR(int32_t, 0x84) < 0 || FR(int32_t, 0x48) + FR(int32_t, 0x78) >= F<int32_t>(m, 0x215d) ||
                        FR(int32_t, 0x84) + FR(int32_t, 0x78) >= F<int32_t>(m, 0x2161);
    int32_t water = (F<int32_t>(FR(uint8_t*, 0x80), 0x17f) | F<int32_t>(FR(uint8_t*, 0x80), 0x17b)) != 0;
    // A cell outside the frame block (where the original would hit saved registers) reads 0 and is not written.
    auto cell = [&](int32_t layer, int32_t at) -> uint8_t* {
        int32_t o = (int32_t)bufOff[layer] + at;
        static uint8_t sink;
        if (o < 0 || o >= (int32_t)sizeof fr) return sink = 0, &sink;
        return fr + o;
    };
    // Layers: 0 base, 1 land, 2 water, 3 other.
    static const uint32_t flag[4] = { 0x183, 0x177, 0x17b, 0x17f }, maskOff[4] = { 0xdb, 0xcf, 0xd3, 0xd7 };
    for (int32_t layer = 0; layer < 4; ++layer) {
        uint8_t* s = FR(uint8_t*, 0x80);
        if (!F<int32_t>(s, flag[layer])) continue;
        FR(const uint16_t*, 0x60) = F<uint16_t*>(s, maskOff[layer]);
        int32_t word = 0;
        for (int32_t r = 0; r < FR(uint8_t*, 0x80)[0xb9]; ++r)
            for (FR(int32_t, 0x4c) = 0; FR(int32_t, 0x4c) < FR(uint8_t*, 0x80)[0xb8]; ++FR(int32_t, 0x4c)) {
                const uint16_t* p = FR(const uint16_t*, 0x60) + word;
                for (int32_t k = 0; k < 16; ++k, ++word, ++p) {
                    if (!*p) continue;
                    int32_t c = FR(int32_t, 0x4c);
                    FR(float, 0x8c) = (float)(((double)(float)((k & 3) + c * 4) - FR(float, 0x68)) * Fl_207(0x5c8568));
                    FR(float, 0x88) = (float)(((double)(float)((k >> 2) + r * 4) - FR(float, 0x68)) * Fl_207(0x5c8568));
                    for (int32_t j = 0, bit = 15; j < 16; ++j, --bit) {
                        if (!(*p & 1 << bit)) continue;
                        float sx = (float)((double)(float)(j & 3) + FR(float, 0x8c));
                        float sy = (float)((double)(float)(j >> 2) + FR(float, 0x88));
                        float rx = (float)((double)sx * FR(float, 0x7c) - (double)sy * FR(float, 0x74));
                        float ry = (float)((double)sy * FR(float, 0x7c) + (double)sx * FR(float, 0x74));
                        int32_t cx = Ftol(rx / D(0x5c87b8) + FR(float, 0x64));
                        int32_t cy = Ftol(FR(float, 0x40) - ry / D(0x5c87b8));
                        int32_t at = (cy - FR(int32_t, 0x84)) * FR(int32_t, 0x78) - FR(int32_t, 0x48) + cx;
                        if (*cell(layer, at)) continue;
                        m = g_map;
                        if (FR(int32_t, 0x5c) && (cx < 0 || cx >= F<int32_t>(m, 0x215d) || cy < 0 || cy >= F<int32_t>(m, 0x2161)))
                            return failSite(0xb55, 0xbe);
                        *cell(layer, at) = 1;
                        if (!editor || layer) {
                            uint8_t base = *cell(0, at);
                            int32_t check = -1;
                            if (base) {
                                if (!editor) check = 2;
                            } else if (*cell(1, at)) {
                                if (Map_CellFlag(g_map, 0, cx, cy, 0)) return failSite(0xb55, 0xbe);
                                int32_t bad = water ? Terrain_KindIs1Or3to5(g_map, 0, cx / 4, cy / 4)
                                                    : Map_NearFeature(g_map, 0, cx / 4, cy / 4);
                                if (bad) return failSite(0xb55, 0xbe);
                                if (F<int32_t>(mgr, 0x9c) == 1 || F<int32_t>(hook::As<uint8_t*>(g_cities), 0x88)) check = 3;
                            } else if (*cell(2, at)) {
                                if (!Terrain_KindIs1Or3to5(g_map, 0, cx / 4, cy / 4)) return failSite(0xb55, 0xbe);
                                check = 0;
                            } else if (*cell(3, at))
                                check = 0;
                            if (check >= 0 && Map_CellFlag(g_map, 0, cx, cy, check)) return failSite(0xb55, 0xbe);
                            DirtyCells_AddImpl(base, cy, cx);
                        }
                        ++j, --bit;
                    }
                }
            }
    }
    st = FR(uint8_t*, 0x80);
    btype = FR(uint8_t*, 0x6c);
    cityObj = FR(uint8_t*, 0x58);
#undef FR
    if (!Site_CheckPlacementImpl(st, *(uint32_t*)&x, *(uint32_t*)&y)) return failSite(0xb55, 0xbe);
    if (!Sites_PrepareGround((uint8_t*)mgr, 0, id, 0, 0, 1, nullptr)) return failSite(0xb56, 0xbf);
    uint8_t* type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(btype, 0x173));
    if (type[0x32] == 1) {
        float sx, sy, sa;
        if (snap && !Station_SnapToTrack(mgr, 0, 0, id, x, y, angle, &sx, &sy, &sa, 0, nullptr)) return 0;
        if (onTrack) {
            Collection* tr = g_tracks;
            if (Collection_IsValid(tr, 0, (uint32_t)trackId) && city &&
                F<uint16_t>(Collection_Get(g_tracks, 0, (uint32_t)trackId), 0x22e) != (uint32_t)city)
                return fail(0xb57);
        } else {
            if (city && Collection_IsValid(g_companies, 0, (uint32_t)city) &&
                Company_Get7680(Collection_Get(g_companies, 0, (uint32_t)city)) < 5)
                return fail(0xb58);
            for (int32_t pass = 0; pass < 3; ++pass) {
                float ox, oy;
                if (pass == 0) Type_RotatedCell(g_buildingTypes, 0, id, &ox, &oy, angle);
                else Obj_AttachmentPoint(g_buildingTypes, 0, pass - 1, id, &ox, &oy, angle);
                float py = (float)(oy + y);
                float px = (float)(x - ox);
                if (Grid_NearestNodeWithin(g_tracks, 0, px, py, -1, BitsF(0x3ecbe0df), 0, 0, 0) != -1) return fail(0xb59);
                if (cityObj) {
                    uint32_t t;
                    int32_t pyi = Ftol(py);
                    if (regionBlocks(px, py, Ftol(px), pyi, &t)) return failRegion(t);
                }
                for (int32_t ring = 1; ring <= 3; ++ring)
                    for (int32_t o = -4; o < 0x20; o += 4) {
                        float qx = px, qy = py;
                        uint8_t* mp = g_map;
                        if (o != -4) {
                            double d = (double)ring;
                            qx = (float)((double)*(int32_t*)(0x624b28 + o) * d * D(0x5c9740) + qx);
                            qy = (float)((double)*(int32_t*)(0x624b48 + o) * d * D(0x5c9740) + qy);
                        }
                        if (qx < g_5c8598 || qy < g_5c8598) return fail(0xbe);
                        float wf = (float)F<int32_t>(mp, 0x2155);
                        if (qx >= wf) return fail(0xbe);
                        float hf = (float)F<int32_t>(mp, 0x2159);
                        if (qy >= hf) return fail(0xbe);
                        int32_t qyi = Ftol(qy);
                        if (Terrain_IsSpecial(mp, 0, Ftol(qx), qyi)) return fail(0xbe);
                    }
            }
        }
    }
    type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(btype, 0x173));
    if (type[0x32] == 1 && type[0x8c] == 3 && x >= g_5c878c) {
        m = g_map;
        float xl = (float)(F<int32_t>(m, 0x2155) - 3);
        if (x < xl && y >= g_5c878c) {
            float yl = (float)(F<int32_t>(m, 0x2159) - 3);
            if (y < yl) {
                float tilt = 0.0f, base;
                if (onTrack && Collection_IsValid(g_tracks, 0, (uint32_t)trackId)) {
                    base = (float)((double)Obj_Mid1ed((uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)trackId)) - Fl_207(0x5c8738));
                    tilt = Vehicle_Tilt((uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)trackId), 0, 1);
                } else {
                    float ox, oy;
                    Type_RotatedCell(g_buildingTypes, 0, id, &ox, &oy, angle);
                    base = (float)Terrain_HeightAtTile(g_map, 0, (float)(x - ox), (float)(oy + y));
                }
                for (const uint32_t* p = (const uint32_t*)0x5ee0f8; p < (const uint32_t*)0x5ee138; p += 2) {
                    float rx, ry;
                    RotateNegated(BitsF(p[0]), BitsF(p[1]), angle, &rx, &ry);
                    float qx = (float)(x - rx), qy = (float)(y - ry);
                    float d = (float)(Terrain_HeightAtTile(g_map, 0, qx, qy) - base);
                    if (d > Fl_207(0x5c8788) || d < Fl_207(0x5c9738) || fabs((double)tilt) > D(0x5c9730)) return fail(0xb5a);
                }
            }
        }
    }
    type = (uint8_t*)Collection_Get(g_62b268, 0, F<uint32_t>(btype, 0x173));
    if (type[0xba]) {
        int32_t gx = Ftol(x / Fl_207(0x5c8568)), gy = Ftol(y / Fl_207(0x5c8568));
        int32_t land = 0, sea = 0;
        uint8_t* mp = g_map;
        for (int32_t o = 0; o < 0x20; o += 4) {
            int32_t tx = *(int32_t*)(0x624b28 + o) + gx, ty = *(int32_t*)(0x624b48 + o) + gy;
            if (tx < 0 || tx >= F<int32_t>(mp, 0x2145) || ty < 0 || ty >= F<int32_t>(mp, 0x2149)) continue;
            if (F<uint8_t*>(mp, 0x2129)[(F<int32_t>(mp, 0x2145) * ty + tx) * 0x1d7 + 0xe6] & 1) land = 1;
            else sea = 1;
        }
        if (!land || !sea) {
            ret = 0;
            if (msg) copy(GetString(0xc0));
        }
    }
    return ret;
}
RT3_REPLACE(0x4197e0, Sites_CanPlace);

}  // namespace rt3
