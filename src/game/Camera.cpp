// Camera.cpp - RT3.exe 0x438000-0x440000.
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
#include "game/collection.h"
#include "core/crt.h"
#include "game/calendar.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t ClockMs();
void* __fastcall Engine_Renderer(void* e);
double __fastcall Obj_Scene36f9(uint8_t* o);
void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void* __fastcall Engine_Input(void* e);
uint32_t __fastcall Input_Flag400();
uint32_t __fastcall Input_Flag2000();
uint32_t __fastcall Input_Flag800();
int32_t __fastcall Obj_Flag7dOrTracks118(uint8_t* o);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Camera_Detach(void* c);
void __fastcall Camera_Destruct(uint32_t* c);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

// 0x43a750. The camera pitch (+0x14) for its zoom level per camera mode (options
// +0x10c): a free camera is clamped to [-0.4, max]; mode 0 follows a power
// curve, mode 1 a two-segment line; mode 2 is fixed. vtbl +0xc updates the camera.
static float Camera_PitchForZoomMode(uint8_t* c, float zoom, int32_t mode);

double __fastcall Scene_FogFactor(uint8_t* s, int, uint8_t* r);
float __fastcall Render_Get14e0(uint8_t* r);
int32_t __fastcall Obj_ArrayAc(void* o, int, int32_t i);
int32_t __fastcall Obj_ArrayAc2(void* o, int, int32_t i);
double __stdcall Render_Scaled1540(uint8_t* r);
void __fastcall Render_Set1534(uint8_t* r, int, uint32_t v);
void __fastcall Render_Set1544(uint8_t* r, int, uint32_t v);
void __fastcall View_AccumulateMotion(uint8_t* o);
double __fastcall SceneObj_GroundHeight(uint8_t* o);
void __fastcall Render_SetLayerValue(uint8_t* r, int, int32_t i, float v);
void __fastcall Camera_UpdateHaze(uint8_t* cam, int, float h);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Camera_KeepAboveGround(uint8_t* c);
void __fastcall Camera_ClampToMap(uint8_t* c, int, int32_t keepTarget);
void __fastcall Camera_SmoothHeight(uint8_t* c);
void __fastcall Camera_ClampToGround(uint8_t* c);
void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z);
void __fastcall Camera_UpdateEye(uint8_t* c);
int32_t Options_Get10c();
void __fastcall Camera_UpdateTarget(uint8_t* c);
void __fastcall Camera_Setup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
float __fastcall Obj_ScaledSpanOffset(uint8_t* o, int, float t);
int32_t __fastcall Camera_PickGround(uint8_t* cam, int, float sx, float sy, float height, float* cx, float* cz);
int32_t __stdcall Map_TileToScreen(float tx, float ty, float h, float* ox, float* oy);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
uint8_t* __fastcall Camera_ConstructBase(uint8_t* c);
void __fastcall Camera_Reset(uint8_t* c);
int32_t __fastcall Tool_RemoveModifier();
int32_t __fastcall Tool_AddModifier(uint8_t* t);
void __fastcall Camera_UpdateFocus(uint8_t* c);
void __fastcall Camera_Drag(uint8_t* c, int, float dx, float dy, int32_t);
void __fastcall Camera_DragToMouse(uint8_t* c);
float __fastcall Camera_PitchForZoom(uint8_t* c, int, float zoom);
float __fastcall Camera_TargetGround(uint8_t* c);
void __fastcall Camera_FollowGround(uint8_t* c);
void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
int32_t __fastcall Buckets_Find(int32_t key, int32_t* value, uint32_t* flags);
void __fastcall Sound_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol);
uint32_t __fastcall Camera_Init(uint8_t* c);
void __fastcall Caps_Load(uint8_t* o);
void __fastcall Scene_SetDetail(uint8_t* s, int, int32_t v);
uint32_t __fastcall Sound_Play2D(uint8_t* snd, int, const char* name, float vol, uint32_t pan, int32_t mode, int32_t paused, void* data);
void* __fastcall Obj_FirstBuilding(void* o);
void __fastcall Obj_ShowConstructCursor(uint8_t* o, int, float scale);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
int32_t __fastcall Scene_FocusCell(uint8_t* s, int, float* x, float* y);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
double __stdcall Distance3D(float x1, float y1, float z1, float x2, float y2, float z2);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);
float __fastcall Scene_GetLightValue0(uint8_t* s, int, int32_t i);
void __fastcall Scene_SetPair0(uint8_t* s, int, float a, float b);
void __fastcall Scene_SetPair1(uint8_t* s, int, float a, float b);
int32_t __fastcall Fx_Intensity(uint8_t* o);
void __fastcall Renderer_SetTimeOfDay(uint8_t* r, int, int32_t secs, int32_t night);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

using SprintfFn = int(__cdecl*)(char*, const char*, ...);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Obj_BuildingValue(void* o);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
int32_t __fastcall Obj_CargoStockAndQuota(uint8_t* o, int, int32_t i, float* stock, float* quota);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
uint32_t IsEnglish();
void __fastcall Fade_Save();
void __fastcall Fade_Restore();
void __stdcall Fade_Set(uint32_t v);
int32_t Global6d07b4Set2();
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void* __fastcall Collection_RecordAt(Collection* t, int, uint32_t pos);

void __fastcall Train_GatherLoads(uint8_t* o, int, int32_t simulate, uint8_t* city, uint8_t* to, int32_t cargo, float cap,
                                  float* outCount, float* outAmount, float* outFlagged);

int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
double __fastcall Terrain_HeightAt(uint8_t* t, int, float x, float z);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t __fastcall Table_CountField3f(Collection* t);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
void __fastcall FreeAndNull(void** p);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
int32_t __fastcall Obj_Idlec60(uint8_t* o);
void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
void* __fastcall Obj_Ref72Plus45(void* o);
void __fastcall Mouse_Poll(uint8_t* m);
void __fastcall Camera_SetMode(uint8_t* c, int, int32_t mode);
uint32_t __fastcall Input_Key(uint8_t* in, int, int32_t k);
void __fastcall Camera_Turn(uint8_t* c, int, float d);
int32_t __fastcall Engine_IsForeground();
void __fastcall Camera_Pan(uint8_t* c, int, float dx, float dz);
void __fastcall Mouse_GetPos3(uint8_t* m, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Camera_ZoomStep(uint8_t* c, int, int32_t dir);
int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);
void __fastcall Obj_TakeListCommand(uint8_t* o);
int32_t OptionTable10c();
void __fastcall Camera_MouseInput(uint8_t* c, int, float dx, float dy, float wheel);
void __fastcall Camera_MouseInputMomentum(uint8_t* c, int, float dx, float dy, float wheel);
void __fastcall Camera_Step(uint8_t* c, int, float dt, int32_t move);
void __fastcall Camera_ApplyMomentum(uint8_t* c, int, float dt, int32_t move);
void __fastcall Camera_UpdateShake(uint8_t* c);
void __fastcall Camera_SetAngles(uint8_t* c, int, uint32_t yaw, float pitch, uint32_t zoom);
void __fastcall Camera_Update(uint8_t* c);
int32_t Global6d10a8Set();

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void __fastcall SetSeed(int32_t seed);
void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v);
void __fastcall Ambient_Start(void*);
void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
void __fastcall Map_RuntimeDestructor(uint8_t* map);
void __fastcall Cursor_PopBusy(uint8_t* c);
int32_t Global6cfeb4Positive();
void SetGlobal6cfeac();
int32_t UiBusy();
void __fastcall Map_UnloadWorld(uint8_t* map);
void __fastcall Routes_BuildCosts(uint8_t* t);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
void __fastcall World_SetupPlayers(uint8_t* w);
void __fastcall World_RunAhead(uint8_t* w);
void __fastcall Cities_UpdateAll(Collection* t, int, int32_t a, int32_t b);
void __fastcall Obj_Clear1c4f(void* o);
void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void World_MarkKind2Types();
void __fastcall CargoTypes_UpdateAverages(Collection* c, int, int32_t all);
void __fastcall Cities_AddTestRow(Collection* t);
void __fastcall Territories_AddTestSet(Collection* t);
void* Engine_FirstListEntry();
void __fastcall Cursor_PushBusy(uint8_t* c);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
RT3_NOINLINE void __fastcall CargoTypes_UpdateAverages(Collection* c, int, int32_t all);
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
uint64_t __fastcall DateAddMinutes(int32_t minutes, GameDate d);
int32_t Global6d1350Set();
void __fastcall World_Reset(uint8_t* w, int, int32_t setup);
void Clear6acd60();
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall NameTable_AddPath(uint8_t* t, int, const char* path);
RT3_NOINLINE void __fastcall Cursor_PopBusy(uint8_t* c);
Collection* __fastcall CargoTypes_Construct(Collection* c);
Collection* __fastcall BuildingTypes_Construct(Collection* t);
void __fastcall Obj_Nop411d90(void*);
Collection* __fastcall BtyTable_Construct(Collection* t);
RT3_NOINLINE uint8_t* __fastcall CarTypes_ConstructRuntime(uint8_t* table);
Collection* __fastcall EngineTypes_Construct(Collection* t);
RT3_NOINLINE void* __fastcall Obj_Ref72Plus45(void* o);
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
uint8_t* __fastcall Map_Construct(uint8_t* m, int, int32_t w, int32_t h, const char* tex);
Collection* __fastcall Collection5d00c8_Construct(Collection* c);
Collection* __fastcall Companies_Construct(Collection* c);
Collection* __fastcall Players_Construct(Collection* c);
Collection* __fastcall Collection5ce418_Construct(Collection* c);
Collection* __fastcall Collection5cae10_Construct(Collection* c);
uint8_t* __fastcall Airplanes_Construct(uint8_t* o);
int32_t __fastcall Map_ImportHeightmap(uint8_t* m, int, const char* name, float scale, float curve, int32_t smooth);
RT3_NOINLINE void __fastcall Map_UnloadWorld(uint8_t* map);
RT3_NOINLINE void __fastcall Map_RuntimeDestructor(uint8_t* map);
RT3_NOINLINE void __fastcall Terrain_BuildTiles(uint8_t* t, int, int32_t w, int32_t h, uint8_t* src);
RT3_NOINLINE void __fastcall Ambient_Start(void*);
void Ambient_Update();
RT3_NOINLINE void __fastcall Camera_UpdateShake(uint8_t* c);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_Set372c(uint8_t* s, int, uint32_t v);
RT3_NOINLINE void* __fastcall Engine_Input(void* e);
RT3_NOINLINE void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
void __fastcall Scene_Set3720(uint8_t* s, int, uint32_t a, uint32_t b);
void __fastcall StatusBar_ShowCash(uint8_t* w);
void __fastcall StatusBar_ShowDate(uint8_t* w);
RT3_NOINLINE void* Engine_FirstListEntry();
int32_t __fastcall Net_MessageComplete(uint8_t* s);
void __fastcall Train_UpdateSound(uint8_t* t);
void __fastcall MessagePanel_RefreshOpen();
RT3_NOINLINE int32_t Global6cfeb4Positive();
RT3_NOINLINE void SetGlobal6cfeac();
void SetGlobal6d10a4();
int32_t Global6d12bcSet();
void SetGlobal6d12cc();
void SetGlobal6d1360();
int32_t Global6d0800Set();
void SetGlobal6d07fc();
int32_t Global6cfe00Set();
void SetGlobal6cfe10();
int32_t Global6d3ae4();
void SetGlobal6d3adc();
RT3_NOINLINE void __fastcall World_ProcessEvents(uint8_t* w);
RT3_NOINLINE int32_t UiBusy();
int32_t __fastcall Table_AnyPending(Collection* t);
void* __fastcall Table_Oldest14(Collection* t, int, uint8_t all);
void __fastcall Newspaper_Show(uint8_t* rec);
int32_t __fastcall World_TogglePause(uint8_t* w, int, int32_t announce);
void __fastcall Ledger_Open(int32_t company);
RT3_NOINLINE void __fastcall Camera_MouseUpdate(uint8_t* c, int, int32_t forced);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void __fastcall World_SetupPlayers(uint8_t* w);
int32_t __fastcall Camera_RecallBookmark(uint8_t* c, int, int32_t slot);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall Territories_AddTestSet(Collection* t);
RT3_NOINLINE void __fastcall Cities_AddTestRow(Collection* t);
RT3_NOINLINE int32_t __fastcall Engine_IsForeground();
int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
uint32_t __fastcall World_ReopenLastEvent(uint8_t* w);
void __fastcall Camera_UpdateViewSettings(uint8_t* c);
void __fastcall Obj_UpdateThreshold(uint8_t* o);
void __fastcall Obj_SetByte64_52ce60(uint8_t* o);
void __fastcall Camera_SetModeClampZoom(uint8_t* c, int, int32_t mode, int32_t minZoom, int32_t maxZoom);
void __fastcall Camera_Save(uint8_t* c, int, Stream s);
void __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t);
int32_t __fastcall Camera_Load(uint8_t* c, int, Stream s);
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall World_Advance(uint8_t* w, int, int32_t minutes, int32_t nested);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall World_Advance(uint8_t* w, int, int32_t minutes, int32_t nested);
RT3_NOINLINE Collection* __fastcall BtyTable_Construct(Collection* t);
RT3_NOINLINE void __fastcall Cities_AddTestRow(Collection* t);
RT3_NOINLINE void __fastcall Cities_UpdateAll(Collection* t, int, int32_t a, int32_t b);
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE void __fastcall Scenario_End(uint8_t* w);
RT3_NOINLINE void __fastcall World_SetupPlayers(uint8_t* w);
RT3_NOINLINE void __fastcall World_CreatePlayers(uint8_t* w);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall World_RunAhead(uint8_t* w);
RT3_NOINLINE int32_t __fastcall World_OpenEventItem(uint8_t* w, int, uint8_t* e);
RT3_NOINLINE void __fastcall World_PrepareNewGame(uint8_t* w);
RT3_NOINLINE void __fastcall World_PrepareTestGame(uint8_t* w, int, int32_t fill);
RT3_NOINLINE void __fastcall World_ProcessEvents(uint8_t* w);
RT3_NOINLINE uint32_t __fastcall World_ReopenLastEvent(uint8_t* w);
RT3_NOINLINE void __fastcall World_Startup(uint8_t* w, int, int32_t setup, int32_t withMap);
RT3_NOINLINE int32_t __fastcall World_Frame(uint8_t* w);
RT3_NOINLINE void __fastcall Camera_ClampToGround(uint8_t* c);
RT3_NOINLINE void __fastcall Obj_Set65(void* o);
RT3_NOINLINE void __fastcall Obj_CellPos(void* o, int, float* x, float* y);
void __fastcall Camera_SaveWithBookmarks(uint8_t* c, int, Stream s);
RT3_NOINLINE int32_t __stdcall Map_TileToScreen(float tx, float ty, float h, float* ox, float* oy);
RT3_NOINLINE void __fastcall Obj_Nudge92(void* o);
RT3_NOINLINE int32_t Options_Get10c();
RT3_NOINLINE int32_t OptionTable10c();
RT3_NOINLINE double __fastcall Obj_Float18(void* o);
RT3_NOINLINE void __fastcall View_AccumulateMotion(uint8_t* o);
RT3_NOINLINE double __fastcall Camera_TargetAboveGround(uint8_t* c);
int32_t __fastcall Camera_DrawDistance(uint8_t* c);
void __fastcall Caps_Load(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_SetEntry10e(uint8_t* o, int, const uint32_t* src, int32_t i);
void __fastcall Camera_SetFullView(uint8_t* c, int, int32_t full);
float __fastcall Obj_FadedFraction(uint8_t* o);
void __fastcall Obj_StampClock346(void* o);
RT3_NOINLINE void __fastcall Obj_ClearClock346(void* o);
void __fastcall Camera_MoveEyeTo(uint8_t* c, int, float x, float z);
RT3_NOINLINE void __fastcall Camera_UpdateShake(uint8_t* c);
void __fastcall FollowCamera_Destruct(uint32_t* c);
RT3_NOINLINE void __fastcall Camera_Reset(uint8_t* c);
RT3_NOINLINE float __fastcall Camera_PitchForZoom(uint8_t* c, int, float zoom);
RT3_NOINLINE void __fastcall Camera_UpdateHaze(uint8_t* cam, int, float h);
int32_t __fastcall Camera_LoadWithBookmarks(uint8_t* c, int, Stream s);
void __fastcall Camera_ApplyZoom(uint8_t* c);
RT3_NOINLINE int32_t __fastcall Camera_FogAlpha(uint8_t* c, int, int32_t x, int32_t y);
RT3_NOINLINE float __fastcall Obj_ScaledSpanOffset(uint8_t* o, int, float t);
int32_t __fastcall Tool_AddModifier(uint8_t* t);
int32_t __fastcall Tool_RemoveModifier();
RT3_NOINLINE void __fastcall Camera_ClampToMap(uint8_t* c, int, int32_t keepTarget);
RT3_NOINLINE void __fastcall Obj_FillEntry10e(uint8_t* o, int, int32_t i);
RT3_NOINLINE void __fastcall Camera_UpdateViewSettings(uint8_t* c);
void __fastcall Camera_UpdateDetail(uint8_t* c);
RT3_NOINLINE void __fastcall Camera_StartShake(uint8_t* c, int, float strength);
RT3_NOINLINE void __fastcall Camera_Update(uint8_t* c);
void __fastcall Camera_RefreshView(uint8_t* c);
RT3_NOINLINE void __fastcall Camera_SetAngles(uint8_t* c, int, uint32_t yaw, float pitch, uint32_t zoom);
void __fastcall Camera_FollowGround(uint8_t* c);
void __fastcall Camera_Turn(uint8_t* c, int, float d);
RT3_NOINLINE void __fastcall Camera_Orbit(uint8_t* c, int, float d);
RT3_NOINLINE void __fastcall Camera_SetZoom(uint8_t* c, int, uint32_t zoom);
RT3_NOINLINE void __fastcall Camera_SetModeClampZoom(uint8_t* c, int, int32_t mode, int32_t minZoom, int32_t maxZoom);
RT3_NOINLINE int32_t __fastcall Camera_RecallBookmark(uint8_t* c, int, int32_t slot);
RT3_NOINLINE void __fastcall Camera_SnapTurn(uint8_t* c, int, int32_t dir);
RT3_NOINLINE void __fastcall Camera_ZoomStep(uint8_t* c, int, int32_t dir);
RT3_NOINLINE void __fastcall Camera_ResetView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
void __fastcall Camera_UpdateGround(uint8_t* c);
RT3_NOINLINE void __fastcall Camera_SmoothHeight(uint8_t* c);
RT3_NOINLINE void __fastcall Camera_MoveTo(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall Camera_MoveToCell(uint8_t* c, int, float x, float z);
RT3_NOINLINE void __fastcall Camera_Pan(uint8_t* c, int, float dx, float dz);
RT3_NOINLINE uint8_t* __fastcall Camera_ConstructView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
int32_t __fastcall Return0_43d2a0(void*);
int32_t __fastcall Return0_43d2b0(void*);
int32_t __fastcall Return0_43d2c0(void*);
int32_t __fastcall Return0_43d2d0(void*);
int32_t __fastcall Return0_43d2e0(void*);
int32_t __fastcall Return0_43d2f0(void*);
int32_t __fastcall Return0_43d300(void*);
int32_t __fastcall Return0_43d310(void*);
RT3_NOINLINE int32_t __fastcall Camera_PickGround(uint8_t* cam, int, float sx, float sy, float height, float* cx, float* cz);
RT3_NOINLINE void __fastcall Camera_UpdateFocus(uint8_t* c);
RT3_NOINLINE void __fastcall Camera_KeyControl(uint8_t* c, int, uint32_t* zoom);
RT3_NOINLINE void __fastcall Camera_MouseUpdate(uint8_t* c, int, int32_t forced);
void __fastcall Camera_ApplyMomentum(uint8_t* c, int, float dt, int32_t move);
void __fastcall Camera_MouseInputMomentum(uint8_t* c, int, float dx, float dy, float wheel);
RT3_NOINLINE void __fastcall Camera_Drag(uint8_t* c, int, float dx, float dy, int32_t);
RT3_NOINLINE void __fastcall Camera_DragPick(uint8_t* c, int, uint32_t x0, uint32_t y0, float x1, float y1, int32_t nested);
void __fastcall Camera_Step(uint8_t* c, int, float dt, int32_t move);
RT3_NOINLINE void __fastcall Camera_DragToMouse(uint8_t* c);
void __fastcall Camera_MouseInput(uint8_t* c, int, float dx, float dy, float wheel);
void ResetPair5ee4fc();
void __stdcall SetGlobals5ee4fc(int32_t a, int32_t b);
RT3_NOINLINE void __fastcall StationCargo_DrawCompare(void*, int, float, float, float, float, int32_t);
RT3_NOINLINE int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE uint8_t* __fastcall Map_Construct(uint8_t* m, int, int32_t w, int32_t h, const char* tex);
RT3_NOINLINE void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE int32_t __fastcall Map_ImportHeightmap(uint8_t* m, int, const char* name, float scale, float curve, int32_t smooth);
RT3_NOINLINE Collection* __fastcall EngineTypes_Construct(Collection* t);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE void __fastcall Territories_AddTestSet(Collection* t);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall Ledger_Open(int32_t company);
RT3_NOINLINE void __fastcall Newspaper_Show(uint8_t* rec);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
uint8_t __fastcall Scene_RayToHeight(uint8_t* s, int, uint8_t* o, float sx, float sy, float height, float* out);
RT3_NOINLINE void __fastcall Obj_SetByte64_52ce60(uint8_t* o);
RT3_NOINLINE void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
RT3_NOINLINE double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);

static inline uint8_t* Renderer() { return (uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)); }

static inline double ExeFabs(double v) { return ((double(__cdecl*)(double))0x5a152e)(v); }

static inline uint8_t* Engine() { return g_engine; }

// The flag getters ignore their `this` (the engine's input object); the original
// still fetches it before each call.
static inline uint32_t Flag400() { Engine_Input(Engine()); return Input_Flag400(); }

static inline uint32_t Flag2000() { Engine_Input(Engine()); return Input_Flag2000(); }

static inline uint32_t Flag800() { Engine_Input(Engine()); return Input_Flag800(); }

static inline uint32_t InputButtons() { return F<uint32_t>(g_input, 0xa8c); }

static inline int32_t SwapModifiers() { return F<int32_t>(g_app, 0x120); }

static inline uint8_t* SoundSys() { return g_sound; }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

static inline float FromBits(uint32_t u) { return *(float*)&u; }

// Body of 0x43a750 for the fixed camera modes. Kept separate: inline, MSVC 2019
// /O2 drops the whole mode 0 branch after the early return above.
static __declspec(noinline) float Camera_PitchForZoomMode(uint8_t* c, float zoom, int32_t mode) {
    float pitch = F<float>(c, 0x14);
    auto powf_ = (double(__cdecl*)(double, double))0x5a1b60;
    static const uint64_t kExpo = 0x3fd6666666666666ull, kDenom = 0x40d090a000000000ull; // 0.35, 16962.5
    if (mode == 0) {
        pitch = (float)((double)zoom - D(0x5ca0d0));
        pitch = (float)powf_(pitch, *(const double*)&kExpo);
        float top = (float)powf_(*(const double*)&kDenom, *(const double*)&kExpo);
        pitch = (float)((double)pitch / top * D(0x5ca0c0) + D(0x5c86b8));
        if ((double)pitch > D(0x5ca028)) pitch = FromBits(0x3fbc4303);
        if ((double)pitch < D(0x5c86b8)) pitch = FromBits(0x3dcccccd);
    } else if (mode == 2) {
        pitch = FromBits(0x3f47ae14);
    } else if (mode == 1) {
        if (zoom < Fl(0x5ca0b8))
            pitch = (float)(((double)zoom - D(0x5ca0d0)) / D(0x5ca0b0) * D(0x5ca0a8) + D(0x5c86b8));
        else if (zoom < Fl(0x5ca0a4))
            pitch = FromBits(0x3f47ae14);
        else if (zoom < Fl(0x5ca0a0))
            pitch = (float)(((double)zoom - Fl(0x5ca0a4)) / Fl(0x5c86a0) * D(0x5ca098) + D(0x5c8920));
        else
            pitch = FromBits(0x3fbc4303);
    }
    ((void(__thiscall*)(void*))(*(void***)c)[0xc / 4])(c);
    ((double(__cdecl*)(double))0x5a13e4)(pitch);
    return pitch;
}

static inline int32_t CamFtol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static inline double CamFabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }

static inline double CamBitsD(uint64_t b) { return *(double*)&b; }

// The common tail of the camera setters: a zoom (+0x18) below the minimum
// becomes 37.5, the pitch follows the zoom, and the camera is lifted above the
// ground.
static void Camera_ApplyZoom_124(uint8_t* c) {
    if (F<float>(c, 0x18) < D(0x5ca0d0)) F<uint32_t>(c, 0x18) = 0x42160000;
    F<float>(c, 0x14) = Camera_PitchForZoom(c, 0, F<float>(c, 0x18));
    Camera_KeepAboveGround(c);
}

// Zoom (+0x18) floor, pitch for it (+0x14), then keep the eye above ground.
static void ApplyZoom(uint8_t* c) {
    if (F<float>(c, 0x18) < D(0x5ca0d0)) F<float>(c, 0x18) = 37.5f;
    F<float>(c, 0x14) = Camera_PitchForZoom(c, 0, F<float>(c, 0x18));
    Camera_KeepAboveGround(c);
}

static inline uint32_t U(float f) { return *(uint32_t*)&f; }

static inline double Sqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }

static inline double Cos(double x) { return ((double(__cdecl*)(double))0x5a13e4)(x); }

static inline double Fabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }

static inline double Fl_203(uint32_t a) { return *(const float*)a; }

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

static inline double ExeSqrt(double v) { return ((double(__cdecl*)(double))0x5a1634)(v); }

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

static inline double Atof(const char* s) { return ((double(__cdecl*)(const char*))0x5a2076)(s); }

namespace {
// The first open modal (in this order) is told to close; false when none is.
bool World_NudgeOpenDialog() {
    if (Global6cfeb4Positive()) SetGlobal6cfeac();
    else if (Global6d10a8Set()) SetGlobal6d10a4();
    else if (Global6d12bcSet()) SetGlobal6d12cc();
    else if (Global6d1350Set()) SetGlobal6d1360();
    else if (Global6d0800Set()) SetGlobal6d07fc();
    else if (Global6cfe00Set()) SetGlobal6cfe10();
    else if (Global6d3ae4()) SetGlobal6d3adc();
    else return false;
    return true;
}
}

// 0x438080. World w finishes a new game with map refreshes held off (map
// 0x62c120 +0x2205). Outside the editor: with random city sizes (0x6cec74
// +0x174; reseeded from Rand 100..5000 in single player) every city in
// 0x62bae0 not marked +0x23e has its size +0x25a (and +0x25e) scaled by
// 50..200 % (SimRand within a spread of 22, or 9.9 with w+0x66de, 0.6x in
// multiplayer, smaller for big cities and larger below 0x5c8780, at least
// 3); with run-ahead years (+0x178) the world runs ahead (0x437b20). Then
// the players are set up, single player creates the companies (0x47d440,
// kept original) and with auto companies (0x6cec7c +0x82) clears 49 ints at
// world +0x4a7f and opens every territory to the player's company. Every
// company is reset (0x4241c0), the loading screen shows 0x142 and the whole
// map is refreshed.
RT3_NOINLINE void __fastcall World_PrepareNewGame(uint8_t* w) {
    ++F<int32_t>(g_map, 0x2205);
    auto status = [](int32_t id, int32_t step) {
        uint8_t* win = F<uint8_t*>(g_app, 0xc);
        Window_SendEvent5(win, 0, 0x6f, 0x84d2, (uint32_t)(uintptr_t)GetString(id), 0, 0);
        Loading_Pump(g_app, 0, step, 1);
    };
    if (!F<int32_t>(g_app, 0x68)) {
        if (F<int32_t>(g_app, 0x174)) {
            if (!g_session) SetSeed(Rand(0x64, 0x1388));
            F<int32_t>(w, 0x46c3c) = 1;
            status(0x140, 0);
            for (int32_t i = 0; i < Collection_Count(g_cities); ++i) {
                Collection* cities = g_cities;
                uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, (uint32_t)i, 0));
                if (F<int32_t>(c, 0x23e)) continue;
                float spread = w[0x66de] ? 9.9f : 22.0f;
                if (g_session) spread = (float)((double)spread * *(const double*)0x5c8578);
                const float size = F<float>(c, 0x25a);
                if (size > *(const float*)0x5c9f84) spread = (float)((double)spread * *(const double*)0x5c8988);
                else if (size > *(const float*)0x5c8904) spread = (float)((double)spread * *(const double*)0x5c8618);
                else if (size > *(const float*)0x5c87c0) spread = (float)((double)spread * *(const double*)0x5c8da8);
                else if (size > *(const float*)0x5c9f00) spread = (float)((double)spread * *(const double*)0x5c8640);
                if (F<float>(c, 0x25a) < *(const float*)0x5c8780) spread = (float)((double)spread * *(const double*)0x5c8660);
                if (spread < *(const float*)0x5c8900) spread = 3.0f;
                int32_t hi, lo;
                __asm {
                    fld spread
                    mov eax, 0x5a10d0
                    call eax
                    mov hi, eax
                    fld spread
                    fchs
                    mov eax, 0x5a10d0
                    call eax
                    mov lo, eax
                }
                float pct = (float)SimRand(lo, hi);
                if (pct < *(const float*)0x5c9f80) pct = -50.0f;
                else if (pct > *(const float*)0x5c8c28) pct = 100.0f;
                float v = (float)(((double)pct + *(const float*)0x5c8c28) / *(const double*)0x5c85d8 * F<float>(c, 0x25a));
                F<float>(c, 0x25a) = v;
                F<float>(c, 0x25e) = v;
            }
            Cities_UpdateAll(g_cities, 0, 0x3f800000, 1);
            F<int32_t>(w, 0x46c3c) = 0;
        }
        if (!F<int32_t>(g_app, 0x68) && F<int32_t>(g_app, 0x178) > 0) {
            status(0x141, 0);
            World_RunAhead(w);
        }
    }
    World_SetupPlayers(w);
    if (!g_session) {
        if (g_newGame[0x82] ||
            (F<int32_t>(g_app, 0x14c) && !F<int32_t>(g_app, 0x68)))
            hook::Orig<void (*)()>(0x47d440)();
        if (g_newGame[0x82])
            for (int32_t off = 0; off < 0xc4; off += 4) F<int32_t>(g_world, 0x4a7f + off) = 0;
        if (g_newGame[0x82] && F<int32_t>(g_world, 0x21) > 0 &&
            Collection_Get(g_companies, 0, F<uint32_t>(g_world, 0x21))) {
            for (int32_t i = 0; i < Collection_Count(g_territories); ++i) {
                int32_t mine = F<int32_t>(g_world, 0x21);
                void* co = mine > 0 ? Collection_Get(g_companies, 0, (uint32_t)mine) : nullptr;
                Company_SetEntryFlag(co, 0, (int32_t)Collection_IdAt(g_territories, 0, (uint32_t)i, 0), 1);
            }
        }
    }
    for (int32_t i = 0; i < Table_CountField3f(g_companies); ++i)
        Obj_Clear1c4f(Table_NthField3f(g_companies, 0, i));
    status(0x142, 1);
    --F<int32_t>(g_map, 0x2205);
    Map_CellsChangedImpl(g_map, 0, -1, -1, 0x1869f, 0x1869f);
    Loading_Pump(g_app, 0, 1, 1);
}
RT3_REPLACE(0x438080, World_PrepareNewGame);

// 0x4384d0 (thiscall, RET 4). World w starts a generated test game with map
// refreshes held off (0x62c120 +0x2205): +0xbde takes +0xbe2, with `fill`
// the kind-2 types are marked and a test city row and territory set added
// (0x421b60 / 0x4882e0) after the route costs and cargo averages; random
// city sizes and run-ahead years as in World_PrepareNewGame (no resizing
// here, just the city update), the lobby's +0x79 set in single player, at
// least one year in +0x6a6c, scenario slot 0 in use, the players created
// (World_CreatePlayers) and the whole map refreshed.
RT3_NOINLINE void __fastcall World_PrepareTestGame(uint8_t* w, int, int32_t fill) {
    ++F<int32_t>(g_map, 0x2205);
    F<int32_t>(w, 0xbde) = F<int32_t>(w, 0xbe2);
    if (fill) World_MarkKind2Types();
    auto status = [](int32_t id, int32_t step) {
        uint8_t* win = F<uint8_t*>(g_app, 0xc);
        Window_SendEvent5(win, 0, 0x6f, 0x84d2, (uint32_t)(uintptr_t)GetString(id), 0, 0);
        Loading_Pump(g_app, 0, step, 1);
    };
    status(0x13e, 0);
    Routes_BuildCosts(g_map);
    CargoTypes_UpdateAverages(g_cargoTypes, 0, 1);
    if (fill) {
        Cities_AddTestRow(g_cities);
        Territories_AddTestSet(g_territories);
    }
    status(0x13f, 0);
    if (!F<int32_t>(g_app, 0x68)) {
        if (F<int32_t>(g_app, 0x174)) {
            F<int32_t>(w, 0x46c3c) = 1;
            status(0x140, 0);
            Cities_UpdateAll(g_cities, 0, 0x3f800000, 1);
            F<int32_t>(w, 0x46c3c) = 0;
        }
        if (!F<int32_t>(g_app, 0x68) && F<int32_t>(g_app, 0x178) > 0) {
            status(0x141, 0);
            World_RunAhead(w);
        }
    }
    if (!g_session) F<uint16_t>(g_newGame, 0x79) = 1;
    if (F<int32_t>(w, 0x6a6c) < 1) F<int32_t>(w, 0x6a6c) = 1;
    w[0x69db] = 1;
    World_CreatePlayers(w);
    status(0x142, 1);
    --F<int32_t>(g_map, 0x2205);
    Map_CellsChangedImpl(g_map, 0, -1, -1, 0x1869f, 0x1869f);
    Loading_Pump(g_app, 0, 1, 1);
}
RT3_REPLACE(0x4384d0, World_PrepareTestGame);

// 0x438710. World w works through its pending event items (+0x66a6, linked
// by +0; +0xc = 1 marks an urgent one). While a message box is up
// (0x6cfeb4) an urgent item with 0x62be80 set only raises 0x6cfeac.
// Otherwise, once at a time (0x62be3c), each item is opened (0x437c00)
// unless it must wait: kind 6 while the consist editor runs (0x6d3ac8), or
// any non-urgent item while the UI is busy. Opened items that stay
// readable become the last event (+0x66aa, replacing and freeing the old
// one with its text); used-up ones are freed. Waiting items stay queued in
// order.
RT3_NOINLINE void __fastcall World_ProcessEvents(uint8_t* w) {
    // QUIRK: the loop meant to find an urgent item rereads only the first
    // item's flag, so only the head counts.
    if (Global6cfeb4Positive() && F<uint8_t*>(w, 0x66a6) && F<int32_t>(F<uint8_t*>(w, 0x66a6), 0xc) == 1 && g_62be80) {
        SetGlobal6cfeac();
        return;
    }
    if (g_62be3c) return;
    g_62be3c = 1;
    int32_t busy = UiBusy();
    uint8_t** head = &F<uint8_t*>(w, 0x66a6);
    uint8_t *kept = nullptr, *last = nullptr;
    if (*head) {
        uint8_t* next;
        do {
            uint8_t* e = *head;
            next = F<uint8_t*>(e, 0);
            bool wait = busy && !F<int32_t>(e, 0xc);
            if ((F<int32_t>(e, 8) == 6 && hook::As<int32_t>(g_6d3ac8)) || wait) {
                if (last) F<uint8_t*>(last, 0) = e;
                if (!kept) kept = *head;
                last = *head;
                F<uint8_t*>(last, 0) = nullptr;
            } else if (World_OpenEventItem(w, 0, e)) {
                if (uint8_t* old = F<uint8_t*>(w, 0x66aa)) {
                    FreeAndNull((void**)(old + 4));
                    FreeAndNull((void**)&F<uint8_t*>(w, 0x66aa));
                }
                F<uint8_t*>(w, 0x66aa) = *head;
            } else {
                FreeAndNull((void**)(*head + 4));
                FreeAndNull((void**)head);
            }
            *head = next;
        } while (next);
        if (kept) *head = kept;
    }
    --g_62be3c;
}
RT3_REPLACE(0x438710, World_ProcessEvents);

// 0x438840. World w reopens its last event item (+0x66aa), or says there is
// none (0x153). Returns what the opener or the message box returned.
RT3_NOINLINE uint32_t __fastcall World_ReopenLastEvent(uint8_t* w) {
    if (uint8_t* e = F<uint8_t*>(w, 0x66aa)) return (uint32_t)World_OpenEventItem(w, 0, e);
    return Dialog_RunMessage(GetString(0x153), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
RT3_REPLACE(0x438840, World_ReopenLastEvent);

// 0x438890 (RET 8). World w starts up (`setup` for World_Reset; `withMap`
// creates a 33 x 33 map first): the loading screen shows 0x137, the UI
// image packs (CargoCarOverlay, CityStars, TrainIcons, CompanyLogo,
// SolidColors and every name in the pack table +0x429b0) and fonts 3, 2, 1
// and 17 load. For a heightmap game (lobby +1 == 2) the map's file name
// (0x6d1370, without folder and extension) and three values are asked for
// (0x138 scale, default 1.0; 0x139 curve, 1.0; 0x13a smoothing, 1). Then the
// collections are built with loading steps between them: cargo, building
// and car types, the BTY table, engines (each engine's named flag set in the
// world), trains, companies, players, two more lists, the news log and the
// airplanes. With `setup` the start mode (lobby +1) picks: 1 / 7 the
// tutorial (Tutorial_1 / _2.gmp), 2 the heightmap import (scale x 0.7, then
// mode 3), 4 an empty map of 0x6d14cc + 1 by 0x6d14d0 + 1 cells with ambient
// sound, 3 / 5 / 6 the lobby's file (+0x11; 6 marks the editor's +0x6c).
// A loaded file or a new map then goes on as a test game (filled for new
// maps); w is marked ready (+0).
// QUIRK: a file name without a folder loses its first character.
RT3_NOINLINE void __fastcall World_Startup(uint8_t* w, int, int32_t setup, int32_t withMap) {
    auto ed = [] { return g_app; };
    auto status = [&](int32_t id) {
        uint8_t* win = F<uint8_t*>(ed(), 0xc);
        Window_SendEvent5(win, 0, 0x6f, 0x84d2, (uint32_t)(uintptr_t)GetString(id), 0, 0);
    };
    auto pack = [] { return g_pack; };
    auto alloc = [](size_t n) { return ((uint8_t*(__cdecl*)(size_t))0x53b070)(n); };
    F<int32_t>(w, 0) = 0;
    F<int32_t>(w, 0x4c80) = 0;
    World_Reset(w, 0, setup);
    status(0x137);
    Loading_Pump(ed(), 0, 0, 1);
    Clear6acd60();
    F<uint8_t*>(w, 0x46a90) = Pack_AcquireNamed(pack(), 0, (const char*)0x5c9fc8);
    F<uint8_t*>(w, 0x46a88) = Pack_AcquireNamed(pack(), 0, (const char*)0x5c9fb8);
    F<uint8_t*>(w, 0x46a84) = Pack_AcquireNamed(pack(), 0, (const char*)0x5c9fa8);
    F<uint8_t*>(w, 0x46a80) = Pack_AcquireNamed(pack(), 0, (const char*)0x5c9f98);
    F<uint8_t*>(w, 0x46a8c) = Pack_AcquireNamed(pack(), 0, (const char*)0x5c9f88);
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(pack(), 0x429b0)); ++i) {
        Collection* names = F<Collection*>(pack(), 0x429b0);
        const char* name = (const char*)Collection_Get(names, 0, Collection_IdAt(names, 0, (uint32_t)i, 0));
        F<uint8_t*>(w, 0x46aa4 + 4 * i) = Pack_AcquireNamed(pack(), 0, name);
    }
    Loading_Pump(ed(), 0, 1, 0);
    F<uint8_t*>(w, 0x46a94) = Pack_AcquireFont(pack(), 0, 3);
    F<uint8_t*>(w, 0x46a98) = Pack_AcquireFont(pack(), 0, 2);
    F<uint8_t*>(w, 0x46a9c) = Pack_AcquireFont(pack(), 0, 1);
    F<uint8_t*>(w, 0x46aa0) = Pack_AcquireFont(pack(), 0, 0x11);
    Loading_Pump(ed(), 0, 1, 0);
    float scale = 1.0f, curve = 1.0f;
    int32_t smooth = 1;
    char name[0x80];
    memcpy(name, "XXX", 4);
    ++g_skipCount;
    if (F<int32_t>(g_newGame, 1) == 2) {
        NameTable_AddPath(pack(), 0, (const char*)0x6d1370);
        strcpy(name, (const char*)0x6d1370);
        const int32_t n = (int32_t)strlen(name);
        if (n > 0) {
            int32_t i = n;
            while (name[i] != '\\' && --i > 0) {
            }
            if (i != n) {
                ++i;
                ((void*(__cdecl*)(void*, const void*, size_t))0x5a20b0)(name, name + i, (size_t)(n - i + 1));
            }
        }
        const int32_t len = (int32_t)strlen(name);
        if (len >= 4 && name[len - 4] == '.') name[len - 4] = 0;
        Cursor_PopBusy(g_cursorManager);
        char text[0x12c];
        auto ask = [&](int32_t id) {
            Dialog_RunMessage(GetString(id), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)text, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        };
        memcpy(text, "1.0", 4);
        ask(0x138);
        scale = (float)Atof(text);
        memcpy(text, "1.0", 4);
        ask(0x139);
        curve = (float)Atof(text);
        memcpy(text, "1", 2);
        ask(0x13a);
        smooth = ((int(__cdecl*)(const char*))0x5a1ea5)(text);
        Cursor_PushBusy(g_cursorManager);
    }
    Loading_Pump(ed(), 0, 1, 0);
    Cursor_PushBusy(g_cursorManager);
    status(0x13b);
    Loading_Pump(ed(), 0, 0, 1);
    {
        uint8_t* m = alloc(0x31e);
        g_cargoTypes = m ? CargoTypes_Construct((Collection*)m) : nullptr;
    }
    Loading_Pump(ed(), 0, 1, 0);
    {
        uint8_t* m = alloc(0xa0);
        g_62b268 = m ? BuildingTypes_Construct((Collection*)m) : nullptr;
    }
    Obj_Nop411d90(nullptr);
    Loading_Pump(ed(), 0, 1, 0);
    {
        uint8_t* m = alloc(0xb8);
        g_buildingTypes = m ? BtyTable_Construct((Collection*)m) : nullptr;
    }
    Loading_Pump(ed(), 0, 1, 0);
    {
        uint8_t* m = alloc(0x8c);
        hook::As<uint8_t*>(g_carTypes) = m ? CarTypes_ConstructRuntime(m) : nullptr;
    }
    Loading_Pump(ed(), 0, 1, 0);
    {
        uint8_t* m = alloc(0x8c);
        Collection* engines = m ? EngineTypes_Construct((Collection*)m) : nullptr;
        g_6ada84 = engines;
        for (int32_t i = 0; i < Collection_Count(engines); ++i) {
            Collection* all = g_6ada84;
            void* e = Collection_Get(all, 0, Collection_IdAt(all, 0, (uint32_t)i, 0));
            World_SetNamedFlag(g_world, 0, (const char*)Obj_Ref72Plus45(e), 1);
            engines = g_6ada84;
        }
    }
    Loading_Pump(ed(), 0, 1, 0);
    status(0x13c);
    Loading_Pump(ed(), 0, 0, 1);
    if (withMap) {
        uint8_t* m = alloc(0x2209);
        g_map = m ? Map_Construct(m, 0, 0x21, 0x21, nullptr) : nullptr;
    }
    Loading_Pump(ed(), 0, 1, 0);
    status(0x13d);
    Loading_Pump(ed(), 0, 0, 1);
    {
        uint8_t* m = alloc(0x8c);
        g_trains = m ? Collection5d00c8_Construct((Collection*)m) : nullptr;
    }
    Loading_Pump(ed(), 0, 1, 0);
    {
        uint8_t* m = alloc(0x8c);
        g_companies = m ? Companies_Construct((Collection*)m) : nullptr;
    }
    Loading_Pump(ed(), 0, 1, 0);
    {
        uint8_t* m = alloc(0x8c);
        g_players = m ? Players_Construct((Collection*)m) : nullptr;
    }
    Loading_Pump(ed(), 0, 1, 0);
    {
        uint8_t* m = alloc(0x8c);
        g_6cea4c = m ? Collection5ce418_Construct((Collection*)m) : nullptr;
    }
    {
        uint8_t* m = alloc(0x8c);
        g_messageLog = m ? Collection5cae10_Construct((Collection*)m) : nullptr;
    }
    {
        uint8_t* m = alloc(0xc10);
        hook::As<uint8_t*>(g_62b244) = m ? Airplanes_Construct(m) : nullptr;
    }
    int32_t fresh = 0, loaded = 0;
    if (setup) {
        const int32_t mode = F<int32_t>(g_newGame, 1);
        switch (mode) {
        case 1:
        case 7:
            F<int32_t>(ed(), 0x6c) = 2;
            World_Load(w, 0, (const char*)(F<int32_t>(g_newGame, 1) == 1 ? 0x5c9f68 : 0x5c9f58), 4, &loaded);
            F<int32_t>(ed(), 0x6c) = 0;
            break;
        case 2: {
            if (!g_map) {
                uint8_t* m = alloc(0x2209);
                g_map = m ? Map_Construct(m, 0, 0x21, 0x21, nullptr) : nullptr;
            }
            scale = (float)(scale * D(0x5c8680));
            g_ccbb58 = 1;
            Map_ImportHeightmap(g_map, 0, name, scale, curve, smooth);
            g_ccbb58 = 0;
            F<int32_t>(g_newGame, 1) = 3;
            fresh = 1;
            break;
        }
        case 4: {
            if (uint8_t* old = g_map) {
                Map_UnloadWorld(old);
                if (uint8_t* again = g_map) {
                    Map_RuntimeDestructor(again);
                    ((void(__cdecl*)(void*))0x53b080)(again);
                }
            }
            uint8_t* m = alloc(0x2209);
            uint8_t* map = m ? Map_Construct(m, 0, g_6d14cc + 1, g_6d14d0 + 1, nullptr) : nullptr;
            g_map = map;
            Terrain_BuildTiles(map, 0, 0x270f, 0x270f, nullptr);
            Ambient_Start(w);
            fresh = 1;
            break;
        }
        case 6:
            F<int32_t>(ed(), 0x6c) = 1;
            World_Load(w, 0, (const char*)g_newGame + 0x11, 4, &loaded);
            break;
        case 3:
        case 5:
            World_Load(w, 0, (const char*)g_newGame + 0x11, 4, &loaded);
            break;
        }
    }
    Loading_Pump(ed(), 0, 1, 0);
    if (loaded || fresh) World_PrepareTestGame(w, 0, fresh);
    Cursor_PopBusy(g_cursorManager);
    F<int32_t>(w, 0) = 1;
    Loading_Pump(ed(), 0, 1, 0);
}
RT3_REPLACE(0x438890, World_Startup);

 // namespace

// 0x439140. World w's frame (nothing before it is ready, +0: returns 1):
// ambient sound, camera shake, the scene's mouse position and the status
// bar. The game time moves by the speed's rate (0x5ee2a0[+0x19]; above speed
// 3 with world +0x4af3 blended 0.6 / 0.4 towards speed 3) x the frame's
// milliseconds (at most 500) x 2 (a tenth with the editor's +0x110), with the
// fraction carried (0x62be28). In a running multiplayer game (+0x66ae) a
// client holds while it is ahead of the host's clock (0x2584 of the first
// engine entry; 250 ms) and catches up 1440 minutes when more than 2880
// behind (every 100 ms); the host holds until a new day's message is
// complete (0x46a3d0) or while +0x9054. Multiplayer advances in steps of
// 0x5f2b38 minutes (+0x4c80 keeps the rest; a client never passes the host:
// it logs and holds), single player by the whole minutes. Then train sounds,
// a pending close of the open dialog (0x6d4114) and, once the scenario is
// decided (+0x4a73), its end after 50 frames or when no dialog is open.
// Unless the game is over (+0x4a7b: returns 0) events run, the oldest news
// shows (not over the end screen or a busy UI), the ledger opens when due
// (+0x4d; the game paused around it in single player), the camera follows
// the mouse and the 3D selection under the cursor is highlighted (when the
// view's widget 0x7d6 allows) after the camera moved. Returns 1.
RT3_NOINLINE int32_t __fastcall World_Frame(uint8_t* w) {
    if (!F<int32_t>(w, 0)) return 1;
    Ambient_Update();
    if (uint8_t* cam = g_62be68) Camera_UpdateShake(cam);
    void* const engine = hook::As<void*>(g_engine);
    Scene_Set372c((uint8_t*)Engine_Renderer(engine), 0, 0);
    uint32_t mx, my;
    Mouse_GetPos((uint8_t*)Engine_Input(hook::As<void*>(g_engine)), 0, &mx, &my);
    Scene_Set3720((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, mx, my);
    if (uint8_t* view = g_gameWindow) {
        StatusBar_ShowCash(view);
        StatusBar_ShowDate(g_gameWindow);
    }
    int32_t ms = ClockMs() - g_62be48;
    if (!g_62be48) ms = 0;
    g_62be48 = ClockMs();
    if (ms >= 0x1f4) ms = 0x1f4;
    float rate = hook::Global<float>(0x5ee2a0 + 4 * F<int32_t>(w, 0x19));
    if (F<int32_t>(g_world, 0x4af3) && F<int32_t>(w, 0x19) > 3)
        rate = (float)(Fl_207(0x5ee2ac) * D(0x5c88e8) + rate * D(0x5c8880));
    float minutes = (float)((double)rate * (double)ms * 2.0);
    if (F<int32_t>(g_app, 0x110)) minutes = (float)(minutes * D(0x5c86b8));
    int32_t run = 1;
    uint8_t* const session = g_session;
    if (session && F<int32_t>(w, 0x66ae)) {
        if (!g_6d40dc) {
            const uint32_t now = F<uint32_t>(w, 0x15);
            uint8_t* first = (uint8_t*)Engine_FirstListEntry();
            const uint32_t host = first ? F<uint32_t>(first, 0x2584) : 0;
            const int32_t t = ClockMs();
            if (const int32_t held = g_6cd8fc) {
                run = 0;
                if ((uint32_t)(t - held) > 0xfa) g_6cd8fc = 0;
            } else if (now > host) {
                g_6cd8fc = t;
                run = 0;
            } else if (host - now > 0xb40 && t - g_6cd8f8 > 0x64) {
                g_6cd8f8 = t;
                F<int32_t>(w, 0x4c80) += 0x5a0;
            }
        } else if (F<int32_t>(session, 0x9054)) {
            run = 0;
        } else {
            const int32_t m = Ftol(minutes);
            const uint64_t to = DateAddMinutes(m + F<int32_t>(w, 0x4c80), *(GameDate*)(w + 0xd));
            if ((uint8_t)(to >> 32) != w[0x11] && !Net_MessageComplete(g_session)) run = 0;
        }
    }
    if (F<int32_t>(w, 0x19) > 0) minutes = (float)((double)g_62be28 + minutes);
    const int32_t whole = Ftol(minutes);
    g_62be28 = (float)((double)minutes - (float)whole);
    if (run == 1) {
        if (g_session && F<int32_t>(w, 0x66ae)) {
            const int32_t step = g_5f2b38;
            uint8_t* first = (uint8_t*)Engine_FirstListEntry();
            const uint32_t host = first ? F<uint32_t>(first, 0x2584) : 0;
            g_6cd8f4 = 0;
            int32_t pending = F<int32_t>(w, 0x4c80) + whole;
            if (pending < g_5f2b38) {
                F<int32_t>(w, 0x4c80) = pending;
            } else {
                for (;;) {
                    if (!g_6d40dc && (uint32_t)(g_5f2b38 + F<int32_t>(w, 0x15)) > host) {
                        char note[0x100];
                        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(note, (const char*)0x5c9fdc, F<int32_t>(w, 0x15), host,
                                                                           g_5f2b38);
                        g_6cd8fc = ClockMs();
                        F<int32_t>(w, 0x4c80) = 0;
                        break;
                    }
                    World_Advance(w, 0, g_5f2b38, 0);
                    ++g_6cd8f4;
                    ++g_6cd92c;
                    if (step != g_5f2b38) {
                        F<int32_t>(w, 0x4c80) = pending - step;
                        break;
                    }
                    pending -= g_5f2b38;
                    if (pending < g_5f2b38) {
                        F<int32_t>(w, 0x4c80) = pending;
                        break;
                    }
                }
            }
        } else if (!(minutes == Fl_207(0x5c8598))) {
            World_Advance(w, 0, whole, 0);
        }
    }
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        Collection* trains = g_trains;
        Train_UpdateSound((uint8_t*)Collection_Get(trains, 0, Collection_IdAt(trains, 0, (uint32_t)i, 0)));
    }
    MessagePanel_RefreshOpen();
    if (g_6d4114) {
        World_NudgeOpenDialog();
        --g_6d4114;
    }
    if (F<int32_t>(w, 0x4a73) && !F<int32_t>(w, 0x4a77)) {
        if (++g_62be34 >= 0x32 || !World_NudgeOpenDialog()) {
            g_62be34 = 0;
            Scenario_End(w);
        }
    }
    if (F<int32_t>(w, 0x4a7b)) return 0;
    World_ProcessEvents(w);
    if (!Global6d12bcSet()) {
        bool show = true;
        if (F<int32_t>(g_gameWindow, 0x8c) == 0xc && (F<uint32_t>(g_input, 0xa8c) & 0x400))
            show = F<int32_t>(w, 0x4a73) && !F<int32_t>(g_world, 0x4a77);
        if (show && (!UiBusy() || Table_AnyPending(g_6cea4c))) {
            if (uint8_t* rec = (uint8_t*)Table_Oldest14(g_6cea4c, 0, 0)) Newspaper_Show(rec);
        }
    }
    if (!Global6d12bcSet() && !Global6d10a8Set() && !g_6d4000 && F<int32_t>(w, 0x4d) &&
        (!UiBusy() || Table_AnyPending(g_6cea4c))) {
        F<int32_t>(w, 0x4d) = 0;
        if (!Global6d10a8Set()) {
            if (F<int32_t>(w, 0x19) && !g_session) World_TogglePause(w, 0, 0);
            Ledger_Open(0);
            if (!F<int32_t>(w, 0x19) && !g_session) World_TogglePause(w, 0, 0);
        }
    }
    Camera_MouseUpdate(g_62be68, 0, 0);
    int32_t moved = 0;
    {
        uint8_t* o = g_input;
        if (!(g_62be2c == F<float>(o, 0xa94)) || !(g_62be30 == F<float>(o, 0xa98))) {
            hook::As<uint32_t>(g_62be2c) = F<uint32_t>(o, 0xa94);
            hook::As<uint32_t>(g_62be30) = F<uint32_t>(o, 0xa98);
            moved = 1;
        }
    }
    uint8_t* const under = F<uint8_t*>(Engine_Renderer(hook::As<void*>(g_engine)), 0x366e);
    uint8_t* const lit = F<uint8_t*>(w, 0x66a2);
    if (under != lit && (moved || lit)) {
        if (lit) ((void(__fastcall*)(uint8_t*))(*(void***)lit)[0x64 / 4])(lit);
        F<uint8_t*>(w, 0x66a2) = nullptr;
        w[0x89] = 0;
        if (under && ((int32_t(__fastcall*)(uint8_t*))(*(void***)under)[0x1c / 4])(under)) {
            if (uint8_t* view = g_gameWindow) {
                uint8_t* c = Widget_FindChild(view, 0, 0x7d6);
                if (c && (c[0x6a] & 4)) {
                    F<uint8_t*>(w, 0x66a2) = under;
                    ((void(__fastcall*)(uint8_t*))(*(void***)under)[0x60 / 4])(under);
                    w[0x89] = 1;
                }
            }
        }
    }
    return 1;
}
RT3_REPLACE(0x439140, World_Frame);

// 0x439820. Keeps the camera (+0x1c/+0x20/+0x24) above the ground: the highest of
// five terrain samples around it plus a margin, eased in by +0x34e unless option
// +0x1146d8; in free mode (+0x60) the ease restarts. Also capped by the view limit
// (vtable +0xc).
RT3_NOINLINE void __fastcall Camera_ClampToGround(uint8_t* c) {
    uint8_t* r = F<uint8_t*>(c, 0x6e);
    float x = F<float>(c, 0x1c), z = F<float>(c, 0x24);
    float h0 = (float)Terrain_HeightAt(r, 0, x, z);
    float h1 = (float)Terrain_HeightAt(r, 0, (float)(x + D(0x5ca020)), z);
    float h2 = (float)Terrain_HeightAt(r, 0, (float)(x - D(0x5ca020)), z);
    float h3 = (float)Terrain_HeightAt(r, 0, x, (float)(z + D(0x5ca020)));
    float h4 = (float)Terrain_HeightAt(r, 0, x, (float)(z - D(0x5ca020)));
    float m = h0;
    if (!(m > h1)) m = h1;
    if (!(m > h2)) m = h2;
    if (!(m > h3)) m = h3;
    if (!(m > h4)) m = h4;
    m = (float)(m + Fl(0x5c85f8));
    float g = (float)(Camera_TargetGround(c) + Fl(0x5ca01c));
    if (!(g > m)) g = m;
    float t = F<float>(c, 0x20);
    if (F<int32_t>(c, 0x60)) {
        if (t < m) t = (float)((double)m - t + t);
        F<uint32_t>(c, 0x34a) = F<uint32_t>(c, 8);
        F<uint32_t>(c, 0x34e) = 0;
    } else {
        float g2 = (float)(((double)g - m) * F<float>(c, 0x34e) + m);
        if (g_engine[0x1146d8]) g2 = m;
        if (t < g2) {
            float d = (float)((double)g2 - t);
            float s = (float)((double)d * F<float>(c, 0x34e) + F<float>(c, 8));
            Camera_SetEye(c, 0, F<uint32_t>(c, 4), *(uint32_t*)&s, F<uint32_t>(c, 0xc));
            t = (float)((double)d + t);
        }
    }
    typedef int32_t(__thiscall * LimitFn)(void*);
    float lim = (float)(*(LimitFn*)(*(uint8_t**)c + 0xc))(c);
    if (t > lim) t = (float)(*(LimitFn*)(*(uint8_t**)c + 0xc))(c);
    if (!(t == F<float>(c, 0x20))) Camera_SetTarget(c, 0, F<float>(c, 0x1c), t, F<float>(c, 0x24));
}
RT3_REPLACE(0x439820, Camera_ClampToGround);

RT3_NOINLINE void __fastcall Obj_Set65(void* o) { F<uint8_t>(o, 0x65) = 1; }
RT3_REPLACE(0x439a80, Obj_Set65);

// 0x439a90. Position (+4, +0xc) in cells (92 units).
RT3_NOINLINE void __fastcall Obj_CellPos(void* o, int, float* x, float* y) {
    const float cell = *(const float*)0x5c8700;
    *x = (float)((double)F<float>(o, 4) / cell);
    *y = (float)((double)F<float>(o, 0xc) / cell);
}
RT3_REPLACE(0x439a90, Obj_CellPos);

// 0x439ac0 (vtable). Saves camera c: the base camera, its ten bookmarks
// (+0x10e, 0x30 bytes each) and 20 zero bytes.
void __fastcall Camera_SaveWithBookmarks(uint8_t* c, int, Stream s) {
    Camera_Save(c, 0, s);
    for (int32_t k = 0; k < 10; ++k) StreamWrite(s, c + 0x10e + k * 0x30, 0x30, 0);
    uint32_t pad[5] = {};
    StreamWrite(s, pad, 0x14, 0);
}
RT3_REPLACE(0x439ac0, Camera_SaveWithBookmarks);

// 0x439b20. Normalised screen position of a tile point (tile x, tile y, height).
RT3_NOINLINE int32_t __stdcall Map_TileToScreen(float tx, float ty, float h, float* ox, float* oy) {
    const float k = *(const float*)0x5c8700;
    float wz = (float)((double)ty * k);
    float wx = (float)((double)tx * k);
    return Scene_ProjectNormalised((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, wx, h, wz, ox, oy, nullptr);
}
RT3_REPLACE(0x439b20, Map_TileToScreen);

RT3_NOINLINE void __fastcall Obj_Nudge92(void* o) {
    const double eps = *(const double*)0x5c88a0;
    F<float>(o, 0x96) = (float)((double)F<float>(o, 0x96) + eps);
    F<float>(o, 0x92) = (float)((double)F<float>(o, 0x92) + eps);
}
RT3_REPLACE(0x439b70, Obj_Nudge92);

RT3_NOINLINE int32_t Options_Get10c() { return F<int32_t>(g_app, 0x10c); }
RT3_REPLACE(0x439ba0, Options_Get10c);

// 0x439bb0. Byte of table 0x5ee4d0 for option +0x10c.
RT3_NOINLINE int32_t OptionTable10c() { return ((const uint8_t*)0x5ee4d0)[F<int32_t>(g_app, 0x10c)]; }
RT3_REPLACE(0x439bb0, OptionTable10c);

RT3_NOINLINE double __fastcall Obj_Float18(void* o) { return F<float>(o, 0x18); }
RT3_REPLACE(0x439bd0, Obj_Float18);

// 0x439be0. Accumulates how far the camera moved since the last snapshot
// (+0x352..: positions +0x4.. and +0x1c..) into +0x34e, capped at 1.
RT3_NOINLINE void __fastcall View_AccumulateMotion(uint8_t* o) {
    double t = ExeFabs((double)F<float>(o, 0x366) - F<float>(o, 0x24));
    t = ExeFabs((double)F<float>(o, 0x35e) - F<float>(o, 0x1c)) + t;
    t = ExeFabs((double)F<float>(o, 0x352) - F<float>(o, 0x4)) + t;
    float a = (float)((ExeFabs((double)F<float>(o, 0x35a) - F<float>(o, 0xc)) + t) / *(const double*)0x5c8c48);
    t = ExeFabs((double)F<float>(o, 0x362) - F<float>(o, 0x20));
    float b = (float)(ExeFabs((double)F<float>(o, 0x356) - F<float>(o, 0x8)) + t);
    const float zero = *(const float*)0x5c8598;
    if (b == zero && a == zero) return;
    float v = (float)((double)a / *(const double*)0x5c8798);
    F<uint32_t>(o, 0x352) = F<uint32_t>(o, 0x4);
    F<uint32_t>(o, 0x35e) = F<uint32_t>(o, 0x1c);
    F<uint32_t>(o, 0x356) = F<uint32_t>(o, 0x8);
    F<uint32_t>(o, 0x35a) = F<uint32_t>(o, 0xc);
    F<uint32_t>(o, 0x362) = F<uint32_t>(o, 0x20);
    F<uint32_t>(o, 0x366) = F<uint32_t>(o, 0x24);
    float s = (float)((double)v + F<float>(o, 0x34e));
    F<float>(o, 0x34e) = s;
    if ((double)s > *(const double*)0x5c8770) F<uint32_t>(o, 0x34e) = 0x3f800000;
}
RT3_REPLACE(0x439be0, View_AccumulateMotion);

// 0x439d30. Height of the camera target (+0x1c/+0x20/+0x24) above the terrain (+0x6e).
RT3_NOINLINE double __fastcall Camera_TargetAboveGround(uint8_t* c) {
    return (double)F<float>(c, 0x20) - Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 0x1c), F<float>(c, 0x24));
}
RT3_REPLACE(0x439d30, Camera_TargetAboveGround);

// 0x439d50 (vtable). Camera c's draw distance: +0x28 for display modes with
// a fixed distance (0x5ee4d0, not mode 2), else scaled down with the zoom
// (+0x14) and kept at least ground height + 0x5c88e0 when zoomed in.
int32_t __fastcall Camera_DrawDistance(uint8_t* c) {
    const int32_t mode = F<int32_t>(g_app, 0x10c);
    if (mode != 2 && *(const uint8_t*)(0x5ee4d0 + mode)) return F<int32_t>(c, 0x28);
    float t = (float)((D(0x5ca028) - (double)F<float>(c, 0x14)) / D(0x5c8660));
    if ((double)t > D(0x5c8888)) t = BitsF(0x3f666666);
    const int32_t n = Ftol((D(0x5c8770) - (double)t) * (double)F<int32_t>(c, 0x28));
    if (!((double)t > D(0x5c8628))) return n;
    const float h = (float)Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 0x1c), F<float>(c, 0x24));
    const float fn = (float)n;
    const float lim = (float)((double)h + Fl_207(0x5c88e0));
    return Ftol(fn > lim ? fn : lim);
}
RT3_REPLACE(0x439d50, Camera_DrawDistance);

// 0x439e40. Reads the eight render capability pairs (keys 0x43d2a0..0x43d310, value
// +0xa6, flags +0xaa, 8 bytes each) and remaps the flags 2 -> 0x10, 0x20 -> 0x12,
// 4 -> 0x11.
RT3_NOINLINE void __fastcall Caps_Load(uint8_t* o) {
    for (uint32_t off = 0xa6; off < 0xe6; off += 4) F<int32_t>(o, off) = 0;
    for (int32_t i = 0; i < 8; ++i)
        Buckets_Find(0x43d2a0 + i * 0x10, &F<int32_t>(o, 0xa6 + i * 8), &F<uint32_t>(o, 0xaa + i * 8));
    static const uint32_t from[] = { 2, 0x20, 4 }, to[] = { 0x10, 0x12, 0x11 };
    uint32_t v[8];
    for (int32_t i = 0; i < 8; ++i) v[i] = F<uint32_t>(o, 0xaa + i * 8);
    for (int32_t k = 0; k < 3; ++k)
        for (int32_t i = 0; i < 8; ++i)
            if (v[i] == from[k]) v[i] = to[k];
    for (int32_t i = 0; i < 8; ++i) F<uint32_t>(o, 0xaa + i * 8) = v[i];
}
RT3_REPLACE(0x439e40, Caps_Load);

// 0x43a070. Copies a 48-byte entry into slot i of the array +0x10e.
RT3_NOINLINE void __fastcall Obj_SetEntry10e(uint8_t* o, int, const uint32_t* src, int32_t i) {
    uint32_t* d = (uint32_t*)(o + 0x10e + i * 0x30);
    for (int k = 0; k < 12; ++k) d[k] = src[k];
}
RT3_REPLACE(0x43a070, Obj_SetEntry10e);

// 0x43a0a0. Switches the camera between full and 70% height bounds;
// engine +0x114216 = !full; refreshes the main camera (virtual slot 0) and flags it.
void __fastcall Camera_SetFullView(uint8_t* c, int, int32_t full) {
    F<int32_t>(g_engine, 0x114216) = full == 0;
    Camera_SetBounds(c, 0, 0, 0, 0x3f800000, full ? 0x3f800000 : 0x3f333333);
    void* cam = hook::As<void*>(g_62be68);
    ((void(__thiscall*)(void*))(*(void***)cam)[0])(cam);
    g_62be68[0x65] = 1;
}
RT3_REPLACE(0x43a0a0, Camera_SetFullView);

// 0x43a0f0 (vtable). Remaining fraction like 0x52d3b0 (0x5c8578 - +0x20 /
// +0x28 x 0x5ca048, at least 0, as a float), faded out above +0x14 >=
// 0x5ca040 towards 0 at 0x5ca038.
float __fastcall Obj_FadedFraction(uint8_t* o) {
    const float n = (float)F<int32_t>(o, 0x28);
    float r = (float)(D(0x5c8578) - (double)F<float>(o, 0x20) / (double)n * D(0x5ca048));
    if ((double)r < Fl_207(0x5c8598)) r = 0.0f;
    const double t = F<float>(o, 0x14);
    if (t < D(0x5ca040)) return r;
    if (!(t < D(0x5ca038))) return 0.0f;
    return (float)((D(0x5c8770) - (t - D(0x5ca040)) / D(0x5ca030)) * (double)r);
}
RT3_REPLACE(0x43a0f0, Obj_FadedFraction);

void __fastcall Obj_StampClock346(void* o) { F<int32_t>(o, 0x346) = ClockMs(); }
RT3_REPLACE(0x43a180, Obj_StampClock346);

RT3_NOINLINE void __fastcall Obj_ClearClock346(void* o) { F<int32_t>(o, 0x346) = 0; }
RT3_REPLACE(0x43a190, Obj_ClearClock346);

// 0x43a1a0. Moves the camera eye to (x, z) scaled by k (keeping its height), with
// no easing, and keeps it above ground.
void __fastcall Camera_MoveEyeTo(uint8_t* c, int, float x, float z) {
    float k = Fl(0x5c8700);
    Camera_SetEye(c, 0, U((float)((double)x * k)), F<uint32_t>(c, 0xc), U((float)((double)z * k)));
    F<float>(c, 0x34e) = 1.0f;
    Camera_ClampToGround(c);
}
RT3_REPLACE(0x43a1a0, Camera_MoveEyeTo);

// 0x43a1e0. Camera shake (+0x38e on, strength +0x392, start ms +0x396): a cosine-
// decaying offset (+0x3a6..+0x3ae, scaled down near the ground) added to the base eye
// (+0x39a..+0x3a2), and the rumble sound (+0x3b2) volume follows it; when it runs out
// the sound stops. Quirk: the remembered volume +0x3b6 is reset to -1 every frame.
RT3_NOINLINE void __fastcall Camera_UpdateShake(uint8_t* c) {
    if (!F<int32_t>(c, 0x38e)) return;
    float a = (float)((Sqrt(F<float>(c, 0x392)) + F<float>(c, 0x392)) / D(0x5c85d0));
    float lim1 = (float)((double)a * Fl(0x5ca074));
    float lim2 = (float)((double)a * Fl(0x5ca070));
    float tf = (float)ClockMs();
    float e = (float)((double)tf - F<float>(c, 0x396));
    if (!(e > lim2) && !(e < Fl(0x5c8598))) {
        float q = (float)((double)e * Fl(0x5c8760));
        float cr = (float)Cos((double)q / lim2);
        float amp = (float)((double)cr * a);
        double vv = amp < D(0x5c85b8) ? 0.01 : (double)amp;
        amp = (float)vv;
        float vol = (float)((Sqrt(amp) + amp) / D(0x5c85d0));
        if (Fabs((double)vol - F<float>(c, 0x3b6)) > D(0x5c9b98)) {
            F<float>(c, 0x3b6) = vol;
            Sound_SetSampleVolume(SoundSys(), 0, F<uint32_t>(c, 0x3b2), vol);
        }
        F<uint32_t>(c, 0x3b6) = 0xbf800000;
        float d = (float)(F<float>(c, 0x20) - Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 0x1c), F<float>(c, 0x24)));
        uint32_t f;
        if (d < Fl(0x5c8c28)) f = 0x3e99999a;
        else if (d < Fl(0x5c87c0)) f = 0x3ecccccd;
        else if (d < Fl(0x5ca06c)) f = 0x3f000000;
        else if (d < Fl(0x5ca068)) f = 0x3f19999a;
        else if (d < Fl(0x5c86a0)) f = 0x3f400000;
        else if (d < Fl(0x5c88e0)) f = 0x3f59999a;
        else f = d < Fl(0x5c85b0) ? 0x3f800000 : 0x3f99999a;
        float fac = *(float*)&f;
        float s = 0.0f;
        if (e < lim1) s = (float)(Cos((double)q / lim1) * ((double)fac * F<float>(c, 0x392)));
        float e2 = (float)((double)e * Fl(0x5c8768));
        F<float>(c, 0x3a6) = (float)(Cos((double)e2 / Fl(0x5c8c28)) * s * D(0x5ca060));
        F<float>(c, 0x3aa) = (float)(Cos((double)e2 / Fl(0x5ca05c)) * s * D(0x5c9ab8));
        F<float>(c, 0x3ae) = (float)(Cos((double)e2 / Fl(0x5ca058)) * s * D(0x5ca050));
    } else {
        F<int32_t>(c, 0x38e) = 0;
        F<int32_t>(c, 0x3a6) = 0;
        F<int32_t>(c, 0x3aa) = 0;
        F<int32_t>(c, 0x3ae) = 0;
        Sound_Stop(SoundSys(), 0, F<uint32_t>(c, 0x3b2), 0);
        F<int32_t>(c, 0x3b2) = 0;
    }
    float z = (float)((double)F<float>(c, 0x3a2) + F<float>(c, 0x3ae));
    float y = (float)((double)F<float>(c, 0x39e) + F<float>(c, 0x3aa));
    float x = (float)((double)F<float>(c, 0x39a) + F<float>(c, 0x3a6));
    Camera_SetEye(c, 0, U(x), U(y), U(z));
    F<float>(c, 0x34e) = 1.0f;
}
RT3_REPLACE(0x43a1e0, Camera_UpdateShake);

// 0x43a560. Follow camera destructor: stops its sound (+0x3b2), then the camera's.
void __fastcall FollowCamera_Destruct(uint32_t* c) {
    c[0] = 0x5ca078;
    if (uint32_t key = F<uint32_t>(c, 0x3b2)) Sound_Stop(SoundSys(), 0, key, 0);
    F<uint32_t>(c, 0x3b2) = 0;
    Camera_Detach(c);
    Camera_Destruct(c);
}
RT3_REPLACE(0x43a560, FollowCamera_Destruct);

// 0x43a5a0. Resets the camera state and reloads the render capabilities.
RT3_NOINLINE void __fastcall Camera_Reset(uint8_t* c) {
    Camera_Init(c);
    for (int32_t i = 0; i < 10; ++i) F<int32_t>(c, 0x10e + i * 0x30) = 0;
    F<int32_t>(c, 0x9e) = 1;
    static const uint16_t zero[] = { 0xa2, 0x2ee, 0x346, 0x36a, 0x386, 0x36e, 0x372, 0x376, 0x37a, 0x37e, 0x382,
                                     0x38e, 0x3b2, 0x92, 0x8e, 0x8a, 0x9a, 0x96 };
    for (uint16_t off : zero) F<int32_t>(c, off) = 0;
    F<uint32_t>(c, 0x38a) = 0xc479c000;
    for (uint32_t off = 0x2f2; off <= 0x342; off += 4)
        if (off == 0x2f2 || off >= 0x302) F<int32_t>(c, off) = 0;
    for (uint32_t off = 0x352; off <= 0x366; off += 4) F<int32_t>(c, off) = 0;
    F<float>(c, 0x34e) = 1.0f;
    for (uint32_t off = 0xe6; off <= 0x10a; off += 4) F<int32_t>(c, off) = 0;
    for (uint32_t off = 0xa6; off < 0xe6; off += 4) F<int32_t>(c, off) = 0;
    Caps_Load(c);
}
RT3_REPLACE(0x43a5a0, Camera_Reset);

RT3_NOINLINE float __fastcall Camera_PitchForZoom(uint8_t* c, int, float zoom) {
    if (zoom < D(0x5ca0d0)) zoom = 37.5f;
    float pitch = F<float>(c, 0x14);
    int32_t mode = F<int32_t>(g_app, 0x10c);
    if (!hook::Global<uint8_t>(0x5ee4d0 + mode) && g_engine[0x11422e]) {
        if (pitch < D(0x5ca0c8)) pitch = FromBits(0xbecccccd);
        if ((double)pitch > D(0x5ca028)) pitch = FromBits(0x3fbc4303);
        return pitch;
    }
    return Camera_PitchForZoomMode(c, zoom, mode);
}
RT3_REPLACE(0x43a750, Camera_PitchForZoom);

// 0x43a940. Camera-height lighting: the camera's height above the ground (`h`, or
// measured when negative) and the daylight level of the current time (from the
// scene's light curve, 0..1) give a haze factor; when it changed by more than 0.01
// (cached in 0x5ee4d8) the scene's two light pairs and the time-of-day lighting
// are refreshed.
RT3_NOINLINE void __fastcall Camera_UpdateHaze(uint8_t* cam, int, float h) {
    if (h < Fl_203(0x5c8598))
        h = (float)((double)F<float>(cam, 0x20) -
                    Terrain_HeightAt(F<uint8_t*>(cam, 0x6e), 0, F<float>(cam, 0x1c), F<float>(cam, 0x24)));
    int32_t hh, mm, ss;
    Clock_SplitD((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, &hh, &mm, &ss);
    float t = (float)((double)mm / D(0x5c88a8) + (double)ss / D(0x5c9f30) + (double)hh);
    int32_t idx = Ftol((double)t / D(0x5c9ab8) * D(0x5ca0e0));
    if (idx < 0) idx = 0;
    else if (idx > 0x8f) idx = 0x8f;
    auto light = [](int32_t i) { return Scene_GetLightValue0((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, i); };
    float now = light(idx);
    float lo = light(0);
    float span = light(0x48);
    span = (float)((double)span - lo);
    double den = span < D(0x5c85b8) ? 0.01 : (double)span;
    float r = (float)(((double)now - lo) / den);
    if (Fl_203(0x5c8598) > r) r = 0.0f;
    r = (float)(r > D(0x5c8770) ? 1.0 : (double)r);
    float dusk = (float)(D(0x5c8770) - r);
    dusk = (float)(((double)dusk - D(0x5c88e8)) / D(0x5c8880));
    if (Fl_203(0x5c8598) > dusk) dusk = 0.0f;
    float hc = Fl_203(0x5c85b4) > h ? 1.0f : h;
    float sq = (float)ExeSqrt(hc);
    float q = (float)(((double)sq - Fl_203(0x5c8550)) / ExeSqrt(6500.0));
    if (q < D(0x5c85b8)) q = 0.0f;
    else if (q > Fl_203(0x5c85b4)) q = 1.0f;
    float v = (float)((double)q * dusk);
    float& cache = g_5ee4d8;
    if (!(ExeFabs((double)cache - v) > D(0x5c85b8))) return;
    cache = v;
    float w = (float)((double)v * D(0x5ca0d8) + D(0x5c8770));
    Scene_SetPair0((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, w, 1.0f);
    Scene_SetPair1((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, w, 1.0f);
    uint8_t* s = (uint8_t*)Engine_Renderer(hook::As<void*>(g_engine));
    int32_t night = Fx_Intensity(s);
    Renderer_SetTimeOfDay(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0xd), night);
}
RT3_REPLACE(0x43a940, Camera_UpdateHaze);

// 0x43ac30 (vtable). Loads camera c: stops its sound (+0x3b2), detaches and
// resets it, loads the base camera, its bookmarks (8, or 10 from save
// version 0x252e on, world +0x46c34) and 20 more bytes; sets the bounds and
// wraps the heading (+0x10) plus 0x5ca0e8. Returns the bytes read.
int32_t __fastcall Camera_LoadWithBookmarks(uint8_t* c, int, Stream s) {
    if (const uint32_t snd = F<uint32_t>(c, 0x3b2)) Sound_Stop(g_sound, 0, snd, 0);
    F<uint32_t>(c, 0x3b2) = 0;
    Camera_Detach(c);
    Camera_Reset(c);
    int32_t r = Camera_Load(c, 0, s);
    const int32_t n = (F<int32_t>(g_world, 0x46c34) > 0x252d ? 1 : 0) * 2 + 8;
    for (int32_t k = 0; k < n; ++k) r += StreamRead(s, c + 0x10e + k * 0x30, 0x30, 0, 1);
    uint8_t pad[0x14];
    r += StreamRead(s, pad, 0x14, 0, 1);
    Camera_SetBounds(c, 0, 0, 0, 0x3f800000, 0x3f333333);
    ((void(__fastcall*)(uint8_t*))(*(void***)c)[0x10 / 4])(c);
    const uint64_t wrapBits = 0x401921fb60000000ull;
    double wrap;
    memcpy(&wrap, &wrapBits, 8);
    F<float>(c, 0x10) = (float)((double(__cdecl*)(double, double))0x5a1390)((double)F<float>(c, 0x10) + Fl_207(0x5ca0e8), wrap);
    return r;
}
RT3_REPLACE(0x43ac30, Camera_LoadWithBookmarks);

// 0x43ad20. Re-applies the camera zoom.
RT3_NOINLINE void __fastcall Camera_ApplyZoom(uint8_t* c) { ApplyZoom(c); }
RT3_REPLACE(0x43ad20, Camera_ApplyZoom);

// 0x43ad50. Fog alpha (0..255) of point (x, y) for the camera: a height-based base
// (pitch +0x14 bands) plus a distance term from the target cell (+0x1c, +0x24).
RT3_NOINLINE int32_t __fastcall Camera_FogAlpha(uint8_t* c, int, int32_t x, int32_t y) {
    float v = F<float>(c, 0x14);
    int32_t base = 0;
    if (!((double)v <= D(0x5ca120))) return 0xff;
    if ((double)v > D(0x5ca118)) {
        __asm {
            fld v
            fsub qword ptr ds:[0x5ca118]
            fmul qword ptr ds:[0x5ca110]
            fdiv qword ptr ds:[0x5c8668]
            mov eax, 0x5a10d0
            call eax
            mov base, eax
        }
    } else {
        if ((double)v < D(0x5ca108)) return 0;
        if ((double)v < D(0x5ca100)) {
            __asm {
                fld v
                fsubr qword ptr ds:[0x5ca100]
                fmul qword ptr ds:[0x5ca0f8]
                fdiv qword ptr ds:[0x5c8668]
                mov eax, 0x5a10d0
                call eax
                mov base, eax
            }
        }
    }
    float r = (float)(((double)v - D(0x5c8630)) / D(0x5c8880));
    if ((double)r < D(0x5c85b8)) r = FromBits(0x3c23d70a);
    float tz = F<float>(c, 0x24), tx = F<float>(c, 0x1c);
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
    float d = (float)(uint16_t)DistanceInt(x, y, cellX, cellZ);
    float q = (float)((double)d / ((double)r + D(0x5c88e8)));
    q = (float)((double)q - Fl(0x5ca0f0));
    int32_t a;
    if ((double)q > Fl(0x5c873c)) {
        a = 0;
    } else if ((double)q > Fl(0x5c8598)) {
        __asm {
            fld dword ptr ds:[0x5c873c]
            fsub q
            fmul dword ptr ds:[0x5ca0ec]
            fdiv dword ptr ds:[0x5c873c]
            mov eax, 0x5a10d0
            call eax
            mov a, eax
        }
    } else {
        a = 0xff;
    }
    if (a <= 0) a = 0;
    if (a >= 0xff) a = 0xff;
    a += base;
    if (a <= 0) a = 0;
    if (a >= 0xff) a = 0xff;
    return a;
}
RT3_REPLACE(0x43ad50, Camera_FogAlpha);

// 0x43aef0. Base +0x14 plus twice the scene scale times t's offset from
// the span centre, relative to the span (+0x54..+0x5c).
RT3_NOINLINE float __fastcall Obj_ScaledSpanOffset(uint8_t* o, int, float t) {
    float d = (float)((double)F<float>(o, 0x5c) - F<float>(o, 0x54));
    float base = F<float>(o, 0x14);
    Obj_Scene36f9(Renderer());  // result discarded by the original
    float r = (float)(((double)t - (double)d * 0.5) / d);
    double v = Obj_Scene36f9(Renderer()) * r;
    v = v + v;
    return (float)(v + base);
}
RT3_REPLACE(0x43aef0, Obj_ScaledSpanOffset);

// 0x43af60. Whether the "add" modifier is active for this tool (+0xa2 forces it).
RT3_NOINLINE int32_t __fastcall Tool_AddModifier(uint8_t* t) {
    if (F<int32_t>(t, 0xa2)) return 1;
    uint32_t shift = Flag400();
    if (uint8_t* g = g_6d1a8c)
        if (Obj_Flag7dOrTracks118(g)) shift = 0;
    int32_t alt;
    if (SwapModifiers() && Flag2000()) alt = 1;
    else if (!SwapModifiers() && Flag800()) alt = 1;
    else alt = 0;
    uint32_t ctrl = Flag800();
    if (ctrl && (InputButtons() & 3)) return 1;
    if (alt) return 1;
    if (shift && ctrl) return 1;
    return 0;
}
RT3_REPLACE(0x43af60, Tool_AddModifier);

// 0x43b030. Whether the "remove" modifier is active.
RT3_NOINLINE int32_t __fastcall Tool_RemoveModifier() {
    uint32_t shift = Flag400();
    Flag2000();
    uint32_t ctrl = Flag800();
    if (uint8_t* g = g_6d1a8c)
        if (Obj_Flag7dOrTracks118(g)) shift = 0;
    if (ctrl && (InputButtons() & 3)) return 0;
    if (SwapModifiers()) return ctrl && !shift ? 1 : 0;
    return Flag2000() ? 1 : 0;
}
RT3_REPLACE(0x43b030, Tool_RemoveModifier);

// 0x43b0e0. Keeps the camera eye (+4/+0xc) on the map and, when asked (and not
// detached, +0x60), the target (+0x1c/+0x24) within a margin from the zoom (+0x20).
RT3_NOINLINE void __fastcall Camera_ClampToMap(uint8_t* c, int, int32_t keepTarget) {
    float w = (float)(Terrain_Rows(F<uint8_t*>(c, 0x6e)) * 0x5c);
    float h = (float)(Terrain_Cols(F<uint8_t*>(c, 0x6e)) * 0x5c);
    const float zero = Fl(0x5c8598);
    if (F<float>(c, 4) < zero) F<int32_t>(c, 4) = 0;
    if (F<float>(c, 0xc) < zero) F<int32_t>(c, 0xc) = 0;
    if (F<float>(c, 4) > w) F<float>(c, 4) = w;
    if (F<float>(c, 0xc) > h) F<float>(c, 0xc) = h;
    if (!keepTarget || F<int32_t>(c, 0x60)) return;
    float m = (float)(((double)F<float>(c, 0x20) / D(0x5c85e8) + D(0x5c8970)) * D(0x5c8c48));
    float lo = -m;
    auto shift = [&](uint32_t eye, uint32_t tgt, float d) {
        F<float>(c, eye) = (float)((double)d + F<float>(c, eye));
        F<float>(c, tgt) = (float)((double)d + F<float>(c, tgt));
    };
    if (F<float>(c, 0x1c) < lo) {
        F<int32_t>(c, 0x312) = 0;
        shift(4, 0x1c, (float)((double)lo - F<float>(c, 0x1c)));
    }
    if (F<float>(c, 0x24) < lo) {
        F<int32_t>(c, 0x312) = 0;
        shift(0xc, 0x24, (float)((double)lo - F<float>(c, 0x24)));
    }
    float hiX = (float)((double)(float)(Terrain_Rows(F<uint8_t*>(c, 0x6e)) * 0x5c - 0x5c) + m);
    if (F<float>(c, 0x1c) > hiX) {
        F<int32_t>(c, 0x312) = 0;
        shift(4, 0x1c, (float)((double)hiX - F<float>(c, 0x1c)));
    }
    float hiZ = (float)((double)(float)(Terrain_Cols(F<uint8_t*>(c, 0x6e)) * 0x5c - 0x5c) + m);
    if (F<float>(c, 0x24) > hiZ) {
        F<int32_t>(c, 0x312) = 0;
        shift(0xc, 0x24, (float)((double)hiZ - F<float>(c, 0x24)));
    }
}
RT3_REPLACE(0x43b0e0, Camera_ClampToMap);

// 0x43b2c0. Fills slot i of the 48-byte array +0x10e: in use, world +0x45 /
// +0x49 and nine of this object's fields.
RT3_NOINLINE void __fastcall Obj_FillEntry10e(uint8_t* o, int, int32_t i) {
    uint8_t* e = o + i * 0x30 + 0x10e;
    F<int32_t>(e, 0) = 1;
    F<uint32_t>(e, 4) = F<uint32_t>(g_world, 0x45);
    F<uint32_t>(e, 8) = F<uint32_t>(g_world, 0x49);
    static const uint8_t kSrc[9] = { 0x4, 0x8, 0xc, 0x18, 0x10, 0x14, 0x1c, 0x20, 0x24 };
    for (int k = 0; k < 9; ++k) F<uint32_t>(e, 0xc + k * 4) = F<uint32_t>(o, kSrc[k]);
}
RT3_REPLACE(0x43b2c0, Obj_FillEntry10e);

// 0x43b350. Per-frame view settings from the camera: its altitude over the
// terrain (+0x38a); the scene's detail level by the fog factor (scene
// +0x22b5b / +0x22b5f, full detail when the option is off or the renderer's
// +0x14e0 is high); the fog start by altitude, stretched by the view-distance
// option (scene +0x25, negated); the draw distance (option 7 table 0x6218a0,
// grown with altitude or by pitch) and the far plane by pitch (renderer +0x1534
// / +0x1544); the scene's +0x22b8b by pitch and option 8.
RT3_NOINLINE void __fastcall Camera_UpdateViewSettings(uint8_t* c) {
    uint8_t* dev = g_map;
    uint8_t* opts = g_app;
    float alt = (float)((double)F<float>(c, 0x20) -
                        ((double(__fastcall*)(uint8_t*, int, uint32_t, uint32_t))&Terrain_HeightAt)(F<uint8_t*>(c, 0x6e), 0, F<uint32_t>(c, 0x1c), F<uint32_t>(c, 0x24)));
    F<float>(c, 0x38a) = alt;
    uint8_t* s = (uint8_t*)Engine_Renderer(hook::As<void*>(g_engine));
    float fog = (float)Scene_FogFactor(s, 0, dev);
    float level;
    if (fog < D(0x5ca188)) level = FromBits(0x3e19999a);
    else if (fog < D(0x5c9f08)) level = FromBits(0x3e99999a);
    else if (fog < D(0x5ca180)) level = FromBits(0x3ee66666);
    else if (fog < D(0x5c9cb8)) level = FromBits(0x3f0ccccd);
    else if (fog < D(0x5ca178)) level = FromBits(0x3f266666);
    else level = fog < D(0x5ca170) ? 0.75f : 1.0f;
    if (!g_engine[0x1146eb]) level = 1.0f;
    if ((double)Render_Get14e0(dev) >= D(0x5ca168)) level = 1.0f;
    else if ((double)Render_Get14e0(dev) >= D(0x5c9ed8)) level = (float)(((double)level + D(0x5c8770)) / D(0x5c85d0));
    F<float>(s, 0x22b5b) = level;
    F<float>(s, 0x22b5f) = (float)(((double)level + D(0x5c8770)) / D(0x5c85d0));
    float d;
    if (alt < Fl(0x5c9ac0)) d = 0.0f;
    else if (alt < Fl(0x5ca164)) d = (float)(((double)alt - Fl(0x5c9ac0)) / D(0x5c85d8));
    else if (alt < Fl(0x5ca160)) d = (float)(((double)alt - Fl(0x5ca164)) / D(0x5c8a20) + D(0x5c85f0));
    else if (alt < Fl(0x5c85b0)) d = (float)(((double)alt - Fl(0x5ca160)) / D(0x5ca158) * D(0x5c9be0) + D(0x5c8a20));
    else d = 70.0f;
    auto grow = [&](double div, double cap, double over) {
        if (!(alt > Fl(0x5ca148))) return;
        double t = ((double)alt - Fl(0x5ca148)) / div;
        alt = (float)((double)alt + (t < cap ? t : over));
    };
    if (Obj_ArrayAc(opts, 0, 3) <= 3) {
        d = (float)((double)d * D(0x5ca150));
        grow(D(0x5c85f0), D(0x5c88a8), 60.0);
    } else if (Obj_ArrayAc(opts, 0, 3) <= 6) {
        d = (float)((double)d * D(0x5ca140));
        grow(D(0x5c8798), D(0x5c96c0), 40.0);
    } else if (Obj_ArrayAc(opts, 0, 3) <= 8) {
        d = (float)((double)d * D(0x5c8820));
    } else if (Obj_ArrayAc(opts, 0, 3) <= 0xb) {
        d = (float)((double)d * D(0x5c8698));
    }
    d = (float)-(double)d;
    F<float>((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0x25) = d;
    float dist = hook::Global<float>(0x6218a0 + Obj_ArrayAc2(opts, 0, 7) * 4);
    if (fog <= D(0x5c8608)) {
        float t = (float)((double)alt - Render_Scaled1540(dev));
        if (Fl(0x5c8598) > t) t = 0.0f;
        dist = (float)((double)t * D(0x5ca138) / D(0x5ca130) + dist);
    } else if (dist >= D(0x5c8770)) {
        float t = (float)(((double)F<float>(c, 0x14) - D(0x5c86b8) - D(0x5c8c20)) / D(0x5c85a0));
        if (t > Fl(0x5c8598) && t < D(0x5c8770)) {
            t = (float)((double)t * D(0x5c85a0) + D(0x5c8988));
            dist = (float)(((double)dist - D(0x5c8770)) * t + D(0x5c8770));
        }
    }
    Render_Set1534(dev, 0, (uint32_t)CamFtol((double)dist * Fl(0x5ca128)));
    float p = (float)(((double)F<float>(c, 0x14) - D(0x5c86b8)) / D(0x5c8618));
    p = (float)(p > D(0x5c8770) ? 1.0 : (double)p);
    p = (float)(p < D(0x5c8608) ? 0.0 : (double)p);
    p = (float)((double)p * D(0x5c8970) + D(0x5c9b78));
    Render_Set1544(dev, 0, (uint32_t)CamFtol(p));
    float pitch = (float)((double)F<float>(c, 0x14) - D(0x5c86b8));
    int32_t e = 0x14;
    if (pitch < D(0x5c8c20)) e = 0x28;
    else if (pitch < D(0x5c88c8)) e = 0x1e;
    int32_t q = Obj_ArrayAc2(opts, 0, 8);
    if (q < 10) e += 8;
    else if (q < 14) e += 4;
    F<int32_t>((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0x22b8b) = e;
}
RT3_REPLACE(0x43b350, Camera_UpdateViewSettings);

// 0x43b920. Picks the scene detail level from the camera's height above ground (2 low
// .. 32 high); when the camera moved more than a height-based distance since the last
// snapshot (+0x36e..+0x382) it drops to 1 and takes a new snapshot, except that after
// 12 small moves it only halves; forced to 1 once after +0x36a.
void __fastcall Camera_UpdateDetail(uint8_t* c) {
    float alt = (float)(F<float>(c, 0x20) - Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 0x1c), F<float>(c, 0x24)));
    double sum = Fabs((double)F<float>(c, 0x24) - F<float>(c, 0x382));
    sum = Fabs((double)F<float>(c, 0x20) - F<float>(c, 0x37e)) + sum;
    sum = Fabs((double)F<float>(c, 0x1c) - F<float>(c, 0x37a)) + sum;
    sum = Fabs((double)F<float>(c, 0xc) - F<float>(c, 0x376)) + sum;
    sum = Fabs((double)F<float>(c, 8) - F<float>(c, 0x372)) + sum;
    sum = Fabs((double)F<float>(c, 4) - F<float>(c, 0x36e)) + sum;
    int32_t moved;
    __asm {
        fld sum
        mov eax, 0x5a10d0
        call eax
        mov moved, eax
    }
    int32_t level;
    if (alt <= Fl(0x5c8cf8)) level = 2;
    else if (alt <= Fl(0x5ca19c)) level = 3;
    else if (alt <= Fl(0x5ca164)) level = 4;
    else if (alt <= Fl(0x5ca198)) level = 5;
    else if (alt <= Fl(0x5ca194)) level = 8;
    else if (alt <= Fl(0x5ca160)) level = 0xc;
    else level = alt <= Fl(0x5c85b0) ? 0x10 : 0x20;
    float th;
    if (alt < Fl(0x5ca06c)) th = 30.0f;
    else if (alt < Fl(0x5c86a0)) th = 80.0f;
    else if (alt < Fl(0x5c88e0)) th = 200.0f;
    else th = alt < Fl(0x5ca190) ? 350.0f : 700.0f;
    if (F<int32_t>(c, 0x36a)) {
        level = 1;
        F<int32_t>(c, 0x36a) = 0;
    }
    float mv = (float)moved;
    if (mv > th) {
        int32_t n = F<int32_t>(c, 0x386);
        if (n > 12 && !((double)th * Fl(0x5c8568) < mv)) {
            level >>= 1;
            F<int32_t>(c, 0x386) = n + 1;
        } else {
            F<uint32_t>(c, 0x36e) = F<uint32_t>(c, 4);
            F<uint32_t>(c, 0x372) = F<uint32_t>(c, 8);
            F<uint32_t>(c, 0x376) = F<uint32_t>(c, 0xc);
            F<uint32_t>(c, 0x37a) = F<uint32_t>(c, 0x1c);
            F<uint32_t>(c, 0x37e) = F<uint32_t>(c, 0x20);
            F<uint32_t>(c, 0x382) = F<uint32_t>(c, 0x24);
            level = 1;
            F<int32_t>(c, 0x386) = 0;
            F<int32_t>(c, 0x36a) = 1;
            ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x3748] = 1;
        }
    }
    Scene_SetDetail((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, level);
}
RT3_REPLACE(0x43b920, Camera_UpdateDetail);

// 0x43bc00. Starts (or strengthens) camera c's shake: strength at least the
// running one (at least 0x5c85b8, else 0.01), restarted unless it began
// within the last second, the rumble sound (0x5f0a2c, +0x3b2) started with
// a new shake, the base eye taken from the camera; then every placed
// building group (0x62bae0, no +0x23e, more than 15 parts) shows its
// construction cursors at strength x 0x5ca1a0.
RT3_NOINLINE void __fastcall Camera_StartShake(uint8_t* c, int, float strength) {
    if (F<int32_t>(c, 0x38e)) {
        if (!(strength > F<float>(c, 0x392))) strength = F<float>(c, 0x392);
        float since = (float)(ClockMs() - 1000);
        if (!(F<float>(c, 0x396) > since)) F<float>(c, 0x396) = (float)(ClockMs() - 1000);
    } else {
        F<float>(c, 0x396) = (float)ClockMs();
        F<uint32_t>(c, 0x3b2) =
            Sound_Play2D(g_sound, 0, g_5f0a2c, 0.001f, 0x3f000000, 1, 0, nullptr);
    }
    F<int32_t>(c, 0x38e) = 1;
    F<float>(c, 0x392) = (float)(strength < *(const double*)0x5c85b8 ? 0.01 : (double)strength);
    F<uint32_t>(c, 0x3a2) = F<uint32_t>(c, 0xc);
    F<uint32_t>(c, 0x39a) = F<uint32_t>(c, 4);
    F<uint32_t>(c, 0x39e) = F<uint32_t>(c, 8);
    Camera_UpdateShake(c);
    for (int32_t i = 0; i < Collection_Count(g_cities); ++i) {
        Collection* t = g_cities;
        uint8_t* g = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (F<int32_t>(g, 0x23e) || F<int32_t>(g, 0x286) <= 0xf) continue;
        float size = (float)((double)strength * Fl(0x5ca1a0));
        for (uint8_t* b = (uint8_t*)Obj_FirstBuilding(g); b;) {
            Obj_ShowConstructCursor(b, 0, size);
            if (!Collection_IsValid(g_buildings, 0, F<uint16_t>(b, 0x270))) break;
            b = (uint8_t*)Collection_Get(g_buildings, 0, F<uint16_t>(b, 0x270));
        }
    }
}
RT3_REPLACE(0x43bc00, Camera_StartShake);

// 0x43bde0. Per-frame camera update (unless frozen, +0x64): measures how far the
// camera still has to travel to its target (position +0x38.., yaw +0x44, pitch
// +0x48, zoom +0x4c against the current +0x4.., +0x10, +0x14, +0x18) in units
// of the per-frame step, which depends on the altitude over the ground and on
// how far the yaw has to turn; moves it a step (vtbl +0) while it is not
// there; refreshes the view settings when the altitude changed enough; sets
// the far fog distances from the altitude and the detail options; and flags
// +0x65 when the camera and its target are both over water.
RT3_NOINLINE void __fastcall Camera_Update(uint8_t* c) {
    View_AccumulateMotion(c);
    if (c[0x64]) return;
    float ground = (float)SceneObj_GroundHeight(c);
    float alt = (float)((double)F<float>(c, 0x20) - ground);
    float yawSpeed = FromBits(0x44b80000);
    float rate = FromBits(0x3e6147ae);
    if (alt > Fl(0x5c85b0)) rate = FromBits(0x3de147ae);
    else if (alt > Fl(0x5ca190)) rate = FromBits(0x3e0bac71);
    else if (alt > Fl(0x5c86a0)) rate = FromBits(0x3e28f5c2);
    float dx = (float)((double)F<float>(c, 0x38) - F<float>(c, 4));
    float dz = (float)((double)F<float>(c, 0x40) - F<float>(c, 0xc));
    if (dz == Fl(0x5c8598)) dz = FromBits(0x3a83126f);
    float yaw = F<float>(c, 0x10);
    float target = (float)((double(__cdecl*)(double))0x5a1270)((double)dx / dz);
    if ((dx > Fl(0x5c8598) && dz < Fl(0x5c8598)) || (dx < Fl(0x5c8598) && dz < Fl(0x5c8598))) target = (float)((double)target + Fl(0x5c8760));
    while (target > Fl(0x5c8760)) target = (float)((double)target - Fl(0x5c8768));
    while (yaw > Fl(0x5c8760)) yaw = (float)((double)yaw - Fl(0x5c8768));
    while (yaw < Fl(0x5ca1f0)) yaw = (float)((double)yaw + Fl(0x5c8768));
    float turn = (float)CamFabs((double)target - yaw);
    if (turn > Fl(0x5c8760)) turn = (float)(Fl(0x5c8768) - (double)turn);
    if (turn > D(0x5ca1e8)) yawSpeed = FromBits(0x4400cccd);
    else if (turn > D(0x5ca1e0)) yawSpeed = FromBits(0x4425999a);
    else if (turn > D(0x5ca1d8)) yawSpeed = FromBits(0x448a0000);
    else if (turn > D(0x5ca1d0)) yawSpeed = FromBits(0x44a5999a);
    float zoomSpeed = F<float>(c, 0x4c) > F<float>(c, 0x18) ? FromBits(0x44898000) : FromBits(0x443b8000);
    static const uint32_t bands[] = { 0x5ca068, 0x5c86a0, 0x5c88e0, 0x5ca190, 0x5ca1c8 };
    static const uint32_t scales[] = { 0x5c8590, 0x5c8c20, 0x5c88e8, 0x5c8880, 0x5c85a0 };
    bool scaled = false;
    for (int k = 0; k < 5 && !scaled; ++k)
        if (alt < Fl(bands[k])) {
            zoomSpeed = (float)((double)zoomSpeed * D(scales[k]));
            scaled = true;
        }
    if (!scaled && !(alt < Fl(0x5c9b0c))) {
        if (alt < Fl(0x5ca1c4)) zoomSpeed = (float)((double)zoomSpeed * D(0x5c8878));
        else if (alt < Fl(0x5ca1c0)) zoomSpeed = (float)((double)zoomSpeed * D(0x5c8818));
        else zoomSpeed = (float)((double)zoomSpeed * D(0x5ca1b8));
    }
    float posDiv = Fl(0x5c85b4) > yawSpeed ? 1.0f : yawSpeed;
    double moveD = CamFabs((double)F<float>(c, 0x3c) - F<float>(c, 8));
    moveD = CamFabs((double)F<float>(c, 0x40) - F<float>(c, 0xc)) + moveD;
    float steps = (float)((CamFabs((double)F<float>(c, 0x38) - F<float>(c, 4)) + moveD) / posDiv);
    float pitchRate = (float)(CamFabs((double)F<float>(c, 0x14) - D(0x5ca1b0)) * D(0x5c8c18) + D(0x5ca1a8));
    if (pitchRate > D(0x5c8618)) pitchRate = FromBits(0x3eb33333);
    double rateD = rate < D(0x5c8678) ? CamBitsD(0x3f50624dd2f1a9fcull) : (double)rate;
    steps = (float)(CamFabs((double)F<float>(c, 0x44) - F<float>(c, 0x10)) / rateD + steps);
    steps = (float)(CamFabs((double)F<float>(c, 0x48) - F<float>(c, 0x14)) / pitchRate + steps);
    steps = (float)(CamFabs((double)F<float>(c, 0x4c) - F<float>(c, 0x18)) / zoomSpeed + steps);
    int32_t mode = F<int32_t>(g_app, 0x10c);
    if (!hook::Global<uint8_t>(0x5ee4d0 + mode) && F<float>(c, 0x14) > D(0x5c8628) && F<float>(c, 0x18) < Fl(0x5c86a0)) {
        float q = (float)((double)F<float>(c, 0x18) / (((double)F<float>(c, 0x14) - D(0x5c8628)) * D(0x5c85c8) + D(0x5c8770)));
        steps = (float)((Fl(0x5ca1a4) - (double)q) / Fl(0x5ca06c) * steps);
    }
    if (!(steps < Fl(0x5c85b4)) || F<int32_t>(c, 0xa2)) ((void(__fastcall*)(uint8_t*))(*(void***)c)[0])(c);
    float limit;
    if (alt < Fl(0x5c9ac0)) limit = 30.0f;
    else if (alt < Fl(0x5ca06c)) limit = 80.0f;
    else limit = alt < Fl(0x5ca1a4) ? 160.0f : 350.0f;
    if ((double)limit < CamFabs((double)F<float>(c, 0x38a) - alt)) Camera_UpdateViewSettings(c);
    float z = (float)(((double(__cdecl*)(double, double))0x5a1b60)(alt, CamBitsD(0x3fd99999a0000000ull)) / D(0x5c87e0));
    z = (float)(z < D(0x5c8d68) ? (double)z : 5.0);
    z = (float)(z > D(0x5c8770) ? (double)z : 1.0);
    uint8_t* opts = g_app;
    float far1 = (float)((double)hook::Global<float>(0x621960 + Obj_ArrayAc2(opts, 0, 4) * 4) * z * D(0x5c8578));
    Render_SetLayerValue(g_map, 0, 0, far1);
    float far2 = (float)((double)hook::Global<float>(0x621960 + Obj_ArrayAc2(g_app, 0, 5) * 4) * z * D(0x5c8578));
    Render_SetLayerValue(g_map, 0, 0xa, far2);
    Camera_UpdateHaze(c, 0, FromBits(0xc479c000));
    if (!(steps > D(0x5c9ba0))) return;
    int32_t ix = CamFtol((double)F<float>(c, 0x1c) / Fl(0x5c8700));
    int32_t iy = CamFtol((double)F<float>(c, 0x24) / Fl(0x5c8700));
    int32_t jx = CamFtol((double)F<float>(c, 4) / Fl(0x5c8700));
    int32_t jy = CamFtol((double)F<float>(c, 0xc) / Fl(0x5c8700));
    uint8_t* map = g_map;
    if (ix >= 0 && ix < F<int32_t>(map, 0x2155) && iy >= 0 && iy < F<int32_t>(map, 0x2159) && !Terrain_KindIs1Or4(map, 0, ix, iy)) {
        map = g_map;
        if (jx >= 0 && jx < F<int32_t>(map, 0x2155) && jy >= 0 && jy < F<int32_t>(map, 0x2159) && !Terrain_KindIs1Or4(map, 0, jx, jy)) return;
    }
    // The original also computes these distances and drops them.
    CamFabs((double)F<float>(c, 0x82) - F<float>(c, 0xc));
    CamFabs((double)F<float>(c, 0x7e) - F<float>(c, 8));
    CamFabs((double)F<float>(c, 0x7a) - F<float>(c, 4));
    CamFabs((double)F<float>(c, 0x14) - D(0x5ca1b0));
    CamFabs((double)F<float>(c, 0x72) - F<float>(c, 0x10));
    CamFabs((double)F<float>(c, 0x76) - F<float>(c, 0x14));
    CamFabs((double)F<float>(c, 0x86) - F<float>(c, 0x18));
    if (F<float>(c, 0x7a) > D(0x5c9ba0)) c[0x65] = 1;
}
RT3_REPLACE(0x43bde0, Camera_Update);

// 0x43c5e0 (vtable). Camera c refreshes its view settings (and the map's
// threshold) unless 0x62be6c, then marks +0x65 and 0x52ce60.
void __fastcall Camera_RefreshView(uint8_t* c) {
    if (g_62be6c) return;
    if (uint8_t* m = g_map) Obj_UpdateThreshold(m);
    Camera_UpdateViewSettings(c);
    c[0x65] = 1;
    Obj_SetByte64_52ce60(c);
}
RT3_REPLACE(0x43c5e0, Camera_RefreshView);

// 0x43c610. Sets the camera's yaw, pitch (wrapped to pi and clamped to
// [0.1, 1.47]) and zoom (-1 keeps it; at least 37.5, at most vtbl +0xc); does
// nothing when all three are unchanged.
RT3_NOINLINE void __fastcall Camera_SetAngles(uint8_t* c, int, uint32_t yaw, float pitch, uint32_t zoom) {
    pitch = (float)((double(__cdecl*)(double, double))0x5a1390)(pitch, CamBitsD(0x400921fafc8b007aull));
    if (pitch < Fl(0x5ca200)) pitch = FromBits(0xbfc90fdb);
    else if (pitch >= D(0x5ca1f8)) pitch = FromBits(0x3fc8f5c3);
    if (pitch < D(0x5c86b8)) pitch = FromBits(0x3dcccccd);
    else if (pitch >= D(0x5ca028)) pitch = FromBits(0x3fbc4303);
    if (FromBits(yaw) == F<float>(c, 0x10) && pitch == F<float>(c, 0x14) && zoom == 0xbf800000) return;
    F<uint32_t>(c, 0x10) = yaw;
    F<float>(c, 0x14) = pitch;
    if (zoom != 0xbf800000) {
        F<uint32_t>(c, 0x18) = zoom;
        if (FromBits(zoom) < D(0x5ca0d0)) F<uint32_t>(c, 0x18) = 0x42160000;
        else {
            auto maxZoom = [&] { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)c)[3])(c); };
            float m = (float)maxZoom();
            if (F<float>(c, 0x18) > m) F<float>(c, 0x18) = (float)maxZoom();
        }
    }
    Camera_ApplyZoom_124(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43c610, Camera_SetAngles);

// 0x43c770. While ground following (+0x34e > 0) moves the eye height (+8) toward
// the terrain under it, blending from +0x34a by the factor, then re-applies the zoom.
RT3_NOINLINE void __fastcall Camera_FollowGround(uint8_t* c) {
    if ((double)F<float>(c, 0x34e) <= Fl(0x5c8598)) return;
    float h = (float)(Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 4), F<float>(c, 0xc)) + D(0x5ca208));
    if (!(F<float>(c, 0x34e) >= D(0x5c8770)))
        F<float>(c, 8) = (float)(((double)h - F<float>(c, 0x34a)) * F<float>(c, 0x34e) + F<float>(c, 0x34a));
    else
        F<float>(c, 8) = h;
    ApplyZoom(c);
}
RT3_REPLACE(0x43c770, Camera_FollowGround);

// 0x43c810. Turns the camera by `d` (yaw +0x10).
RT3_NOINLINE void __fastcall Camera_Turn(uint8_t* c, int, float d) {
    F<float>(c, 0x10) = (float)((double)d + F<float>(c, 0x10));
    Camera_ApplyZoom_124(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43c810, Camera_Turn);

// 0x43c860. Orbits the camera by `d` around its centre (+0xee.., radius +0xfa,
// angle +0x106, yaw offset +0x10a) when orbiting is on (+0xea) and the step is
// large enough (or NaN).
RT3_NOINLINE void __fastcall Camera_Orbit(uint8_t* c, int, float d) {
    if (CamFabs(d) <= D(0x5c88a0) || !F<int32_t>(c, 0xea)) return;
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    float a = (float)((double)d + F<float>(c, 0x106));
    F<float>(c, 0x106) = a;
    double s = sin_(a);
    float z = (float)(sin_(0.0) * s * F<float>(c, 0xfa) + F<float>(c, 0xf6));
    float y = (float)(cos_(0.0) * F<float>(c, 0xfa) + F<float>(c, 0xf2));
    double k = cos_(F<float>(c, 0x106));
    float x = (float)(sin_(0.0) * k * F<float>(c, 0xfa) + F<float>(c, 0xee));
    Camera_SetTarget(c, 0, x, y, z);
    F<float>(c, 0x10) = (float)((double)F<float>(c, 0x10a) + F<float>(c, 0x106));
    Camera_UpdateEye(c);
    Camera_ClampToGround(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43c860, Camera_Orbit);

// 0x43c980. Sets the camera's zoom, keeping yaw and pitch.
RT3_NOINLINE void __fastcall Camera_SetZoom(uint8_t* c, int, uint32_t zoom) {
    Camera_SetAngles(c, 0, F<uint32_t>(c, 0x10), F<float>(c, 0x14), zoom);
}
RT3_REPLACE(0x43c980, Camera_SetZoom);

// 0x43c9a0. Camera c: switches to mode `mode` with the zoom (+0x18) clamped to
// minZoom..maxZoom, updates it at once and returns to mode 0.
RT3_NOINLINE void __fastcall Camera_SetModeClampZoom(uint8_t* c, int, int32_t mode, int32_t minZoom, int32_t maxZoom) {
    Camera_SetMode(c, 0, mode);
    float hi = (float)maxZoom;
    if (F<float>(c, 0x18) > hi) Camera_SetAngles(c, 0, F<uint32_t>(c, 0x10), F<float>(c, 0x14), *(uint32_t*)&hi);
    else {
        float lo = (float)minZoom;
        if (F<float>(c, 0x18) < lo) Camera_SetAngles(c, 0, F<uint32_t>(c, 0x10), F<float>(c, 0x14), *(uint32_t*)&lo);
    }
    Camera_MouseUpdate(c, 0, 1);
    Camera_SetMode(c, 0, 0);
    View_AccumulateMotion(c);
    F<int32_t>(c, 0x34e) = 0;
}
RT3_REPLACE(0x43c9a0, Camera_SetModeClampZoom);

// 0x43ca20. Camera c: jumps to the bookmark `slot` (48 bytes at +0x10e: set
// flag, followed entity kind / id, position, zoom, angles, ...), the yaw
// turned by 0x5ca0e8 modulo 2 pi, following the entity again when there was
// one. Returns 0 for an unset bookmark.
RT3_NOINLINE int32_t __fastcall Camera_RecallBookmark(uint8_t* c, int, int32_t slot) {
    uint8_t* b = c + slot * 48;
    if (!F<int32_t>(b, 0x10e)) return 0;
    World_SetFollow(g_world, 0, 0, nullptr);
    F<uint32_t>(c, 4) = F<uint32_t>(b, 0x11a);
    F<uint32_t>(c, 8) = F<uint32_t>(b, 0x11e);
    F<uint32_t>(c, 0xc) = F<uint32_t>(b, 0x122);
    F<uint32_t>(c, 0x18) = F<uint32_t>(b, 0x126);
    F<uint32_t>(c, 0x10) = F<uint32_t>(b, 0x12a);
    F<uint32_t>(c, 0x14) = F<uint32_t>(b, 0x12e);
    F<uint32_t>(c, 0x1c) = F<uint32_t>(b, 0x132);
    F<uint32_t>(c, 0x20) = F<uint32_t>(b, 0x136);
    F<uint32_t>(c, 0x24) = F<uint32_t>(b, 0x13a);
    const uint64_t twoPi = 0x401921fb60000000ull;
    F<float>(c, 0x10) = (float)((double(__cdecl*)(double, double))0x5a1390)(F<float>(c, 0x10) + Fl_207(0x5ca0e8), *(const double*)&twoPi);
    if (int32_t kind = F<int32_t>(b, 0x112)) World_FollowEntity(g_world, 0, 1, kind, F<int32_t>(b, 0x116));
    Camera_MouseUpdate(c, 0, 1);
    Camera_Update(c);
    return 1;
}
RT3_REPLACE(0x43ca20, Camera_RecallBookmark);

// 0x43cb00. Snaps the camera's yaw to the next quarter turn in direction `dir`
// (rounding up a fraction past 0.1 / 0.9 first).
RT3_NOINLINE void __fastcall Camera_SnapTurn(uint8_t* c, int, int32_t dir) {
    float q = (float)((double)F<float>(c, 0x10) / D(0x5c8c08) * D(0x5c87b8));
    q = (float)((double)q + Fl(0x5ca210));
    float frac = (float)((double(__cdecl*)(double, double))0x5a1390)(q, 1.0);
    if (dir > 0 && frac > D(0x5c86b8)) q = (float)(CamFtol(q) + 1);
    if (dir < 0 && frac > D(0x5c8888)) q = (float)(CamFtol(q) + 1);
    float fd = (float)dir;
    q = (float)((double)q - fd);
    q = (float)((double)q + D(0x5c85b8));
    q = (float)(CamFtol(q) % 4);
    F<float>(c, 0x10) = (float)((double)q * D(0x5c8748));
    Camera_ApplyZoom_124(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43cb00, Camera_SnapTurn);

// 0x43cc30. Zooms one step in direction `dir` along the zoom ladder 0x5ee4dc
// (7 levels, compared by square root, 0x62be4c caches the roots): the current
// position on the ladder (fractions past 0.9 / below 0.1 snap), plus `dir`;
// the bottom is 37.5, the top 1e6.
RT3_NOINLINE void __fastcall Camera_ZoomStep(uint8_t* c, int, int32_t dir) {
    auto sqrt_ = (double(__cdecl*)(double))0x5a1634;
    float* roots = (float*)0x62be4c;
    for (int32_t i = 0; i < 7; ++i) roots[i] = (float)sqrt_((double)hook::Global<int32_t>(0x5ee4dc + i * 4));
    float s = Fl(0x5c85b4) > F<float>(c, 0x18) ? 1.0f : F<float>(c, 0x18);
    float q = (float)sqrt_(s);
    float pos = 6.0f;
    for (int32_t i = 0; i < 7; ++i)
        if (q < roots[i]) {
            if (!i) pos = 0.0f;
            else {
                float base = (float)(i - 1);
                pos = (float)(((double)q - roots[i - 1]) / ((double)roots[i] - roots[i - 1]) + base);
            }
            break;
        }
    float frac = (float)((double(__cdecl*)(double, double))0x5a1390)(pos, 1.0);
    if (frac > D(0x5ca218)) pos = (float)(CamFtol(pos) + 1);
    if (frac < D(0x5c9cb0)) pos = (float)CamFtol(pos);
    float d = (float)dir;
    int32_t idx = CamFtol((double)d + pos + D(0x5c85b8));
    if (idx < 0) idx = 0;
    uint32_t zoom;
    if (idx > 6 || idx == 6) zoom = 0x497423f0;
    else if (!idx) zoom = 0x42160000;
    else zoom = Bits((float)hook::Global<int32_t>(0x5ee4dc + idx * 4));
    Camera_SetAngles(c, 0, F<uint32_t>(c, 0x10), F<float>(c, 0x14), zoom);
    Camera_ClampToGround(c);
    Camera_Update(c);
}
RT3_REPLACE(0x43cc30, Camera_ZoomStep);

// 0x43cdf0. Puts the camera on a new map: sets it up, then the default view
// (yaw pi, pitch 0.2, zoom 2048) and bounds, and notifies it (vtbl +0x10).
RT3_NOINLINE void __fastcall Camera_ResetView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist) {
    Camera_Setup(c, 0, terrain, x, y, z, dist);
    Camera_ApplyZoom_124(c);
    Camera_SetAngles(c, 0, 0x40490fdb, FromBits(0x3e4ccccd), 0x45000000);
    Camera_SetBounds(c, 0, 0, 0, 0x3f800000, 0x3f333333);
    ((void(__fastcall*)(uint8_t*))(*(void***)c)[4])(c);
}
RT3_REPLACE(0x43cdf0, Camera_ResetView);

// 0x43ce80. Per-frame camera ground handling: free mode clamps above ground; else,
// with the per-mode flag (0x5ee4d0[option +0x10c]), follows the ground and remembers
// the eye height (+0x2ee).
void __fastcall Camera_UpdateGround(uint8_t* c) {
    if (F<int32_t>(c, 0x60)) {
        Camera_ClampToGround(c);
        return;
    }
    if (!((const uint8_t*)0x5ee4d0)[F<int32_t>(g_app, 0x10c)]) return;
    Camera_FollowGround(c);
    F<uint32_t>(c, 0x2ee) = F<uint32_t>(c, 8);
}
RT3_REPLACE(0x43ce80, Camera_UpdateGround);

// 0x43cec0. For attached cameras of modes with ground following (0x5ee4d0): follows
// the ground, then eases the remembered height (+0x2ee) toward the eye height (+8)
// by at most 0.2 (up) / 0.4 (down) of cos(pitch) x zoom per call, and below zoom
// 1250 blends the eye height between them (fully the eye's below zoom 1000).
RT3_NOINLINE void __fastcall Camera_SmoothHeight(uint8_t* c) {
    if (F<int32_t>(c, 0x60)) return;
    if (!((const uint8_t*)0x5ee4d0)[F<int32_t>(g_app, 0x10c)]) return;
    Camera_TargetGround(c);
    float reach = (float)(((double(__cdecl*)(double))0x5a13e4)(F<float>(c, 0x14)) * F<float>(c, 0x18));
    Camera_FollowGround(c);
    float y = F<float>(c, 8);
    if (F<float>(c, 0x18) < Fl(0x5c86a0)) {
        F<float>(c, 0x2ee) = y;
        return;
    }
    if (F<float>(c, 0x2ee) < y) {
        float diff = (float)((double)y - F<float>(c, 0x2ee));
        double lim = (double)reach * D(0x5c8c20);
        if ((double)diff > lim) F<float>(c, 0x2ee) = (float)((double)diff - lim + F<float>(c, 0x2ee));
    } else {
        float diff = (float)((double)F<float>(c, 0x2ee) - y);
        double lim = (double)reach * D(0x5c88e8);
        if ((double)diff > lim) F<float>(c, 0x2ee) = (float)((double)F<float>(c, 0x2ee) - ((double)diff - lim));
    }
    if (F<float>(c, 0x18) < D(0x5ca228)) {
        float t = (float)((D(0x5ca228) - F<float>(c, 0x18)) / D(0x5ca220));
        if (t > Fl(0x5c85b4)) t = 1.0f;
        else if (t < Fl(0x5c8598)) t = 0.0f;
        F<float>(c, 8) = (float)((D(0x5c8770) - t) * F<float>(c, 0x2ee) + (double)t * y);
    } else {
        F<uint32_t>(c, 8) = F<uint32_t>(c, 0x2ee);
    }
}
RT3_REPLACE(0x43cec0, Camera_SmoothHeight);

// 0x43d050. Moves the camera to (x, y, z) (+0x4..), keeps it on the map and
// over the ground, then updates it.
RT3_NOINLINE void __fastcall Camera_MoveTo(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z) {
    F<uint32_t>(c, 8) = y;
    F<uint32_t>(c, 4) = x;
    F<uint32_t>(c, 0xc) = z;
    Camera_ClampToMap(c, 0, 1);
    Camera_SmoothHeight(c);
    Camera_ApplyZoom_124(c);
    Camera_ClampToGround(c);
    Camera_ClampToMap(c, 0, 1);
    Camera_Update(c);
}
RT3_REPLACE(0x43d050, Camera_MoveTo);

// 0x43d0c0. Moves the camera to map cell (x, z), on the ground.
void __fastcall Camera_MoveToCell(uint8_t* c, int, float x, float z) {
    uint32_t h = 0;
    if (F<uint8_t*>(c, 0x6e)) {
        float wz = (float)((double)z * Fl(0x5c8700));
        float hh = (float)Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, (float)((double)x * Fl(0x5c8700)), wz);
        h = Bits(hh);
    }
    uint32_t wz = Bits((float)((double)z * Fl(0x5c8700)));
    Camera_MoveTo(c, 0, Bits((float)((double)x * Fl(0x5c8700))), h, wz);
    Camera_Update(c);
}
RT3_REPLACE(0x43d0c0, Camera_MoveToCell);

// 0x43d130. Pans the camera by (dx, dz) screen units, scaled with the square
// root of the zoom and turned by the yaw.
RT3_NOINLINE void __fastcall Camera_Pan(uint8_t* c, int, float dx, float dz) {
    auto sqrt_ = (double(__cdecl*)(double))0x5a1634;
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    float s = Fl(0x5c85b4) > F<float>(c, 0x18) ? 1.0f : F<float>(c, 0x18);
    double r = sqrt_((double)s + D(0x5c9680));
    float f = (float)(r / sqrt_(62.0));
    f = (float)((double)f * Fl(0x5c8900));
    dx = (float)((double)f * dx);
    dz = (float)((double)f * dz);
    float a = (float)((double)dz * Fl(0x5c8904));
    float t1 = (float)(cos_(F<float>(c, 0x10)) * a);
    float t2 = (float)(sin_(F<float>(c, 0x10)) * a);
    float b = (float)((double)dx * Fl(0x5c8904));
    float mx = (float)((double)t1 - sin_(F<float>(c, 0x10)) * b);
    float mz = (float)(cos_(F<float>(c, 0x10)) * b + t2);
    uint32_t z = Bits((float)((double)mz + F<float>(c, 0xc)));
    Camera_MoveTo(c, 0, Bits((float)((double)mx + F<float>(c, 4))), F<uint32_t>(c, 8), z);
    Camera_Update(c);
}
RT3_REPLACE(0x43d130, Camera_Pan);

// 0x43d260. Constructs the map camera (vtable 0x5ca078) and puts it on the
// map (terrain, position, distance).
RT3_NOINLINE uint8_t* __fastcall Camera_ConstructView(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist) {
    Camera_ConstructBase(c);
    F<uint32_t>(c, 0) = 0x5ca078;
    Camera_Reset(c);
    Camera_ResetView(c, 0, terrain, x, y, z, dist);
    return c;
}
RT3_REPLACE(0x43d260, Camera_ConstructView);

// 0x43d2a0 (vtable stub).
int32_t __fastcall Return0_43d2a0(void*) { return 0; }
RT3_REPLACE(0x43d2a0, Return0_43d2a0);

// 0x43d2b0 (vtable stub).
int32_t __fastcall Return0_43d2b0(void*) { return 0; }
RT3_REPLACE(0x43d2b0, Return0_43d2b0);

// 0x43d2c0 (vtable stub).
int32_t __fastcall Return0_43d2c0(void*) { return 0; }
RT3_REPLACE(0x43d2c0, Return0_43d2c0);

// 0x43d2d0 (vtable stub).
int32_t __fastcall Return0_43d2d0(void*) { return 0; }
RT3_REPLACE(0x43d2d0, Return0_43d2d0);

// 0x43d2e0 (vtable stub).
int32_t __fastcall Return0_43d2e0(void*) { return 0; }
RT3_REPLACE(0x43d2e0, Return0_43d2e0);

// 0x43d2f0 (vtable stub).
int32_t __fastcall Return0_43d2f0(void*) { return 0; }
RT3_REPLACE(0x43d2f0, Return0_43d2f0);

// 0x43d300 (vtable stub).
int32_t __fastcall Return0_43d300(void*) { return 0; }
RT3_REPLACE(0x43d300, Return0_43d300);

// 0x43d310 (vtable stub).
int32_t __fastcall Return0_43d310(void*) { return 0; }
RT3_REPLACE(0x43d310, Return0_43d310);

// 0x43d320. Ground point under screen position (sx, sy) for the camera: near
// the horizon (span offset below 0.0?) the vertical position is eased and the
// pick height scaled; a tilt (+0x5c - +0x54) shifts sy by the eye-height
// correction. Writes the cell (x, z) / cell size into *cx / *cz; false when the
// ray misses (no object 0x62be68 or no renderer).
RT3_NOINLINE int32_t __fastcall Camera_PickGround(uint8_t* cam, int, float sx, float sy, float height, float* cx, float* cz) {
    float tilt = (float)((double)F<float>(cam, 0x5c) - F<float>(cam, 0x54));
    float off = Obj_ScaledSpanOffset(cam, 0, sy);
    float eased = off;
    if (off < D(0x5c86b8)) {
        double num = D(0x5c86b8) - off;
        double den = D(0x5c86b8) - Obj_ScaledSpanOffset(cam, 0, 0.0f);
        float k = (float)(D(0x5c8770) - num / den);
        height = (float)((double)k * height);
        eased = (float)(k * D(0x5c9b50) + D(0x5ca230));
    }
    if (!(eased == off)) {
        double d = (double)eased - off;
        float s = (float)(d / (Obj_Scene36f9((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine))) * 2));
        float shift = (float)((double)s * tilt);
        sy = (float)((double)shift + sy);
    }
    *cz = 0.0f;
    *cx = 0.0f;
    void* e = hook::As<void*>(g_engine);
    if (!e || !Engine_Renderer(e) || !g_62be68) return 0;
    float out[3];
    if (!Scene_RayToHeight((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, g_62be68, sx, sy, height, out)) return 0;
    *cx = (float)(out[0] / Fl_205(0x5c8700));
    *cz = (float)(out[2] / Fl_205(0x5c8700));
    return 1;
}
RT3_REPLACE(0x43d320, Camera_PickGround);

// 0x43d480. Works out the camera's focus point (+0xee/+0xf2/+0xf6: the followed
// object, else the screen-centre cell, else the eye itself) with its screen position
// (+0xfe/+0x102), distance (+0xfa), heading (+0x106, mod 2 pi) and the heading
// relative to the camera yaw (+0x10a).
RT3_NOINLINE void __fastcall Camera_UpdateFocus(uint8_t* c) {
    F<int32_t>(c, 0xea) = 0;
    F<uint32_t>(c, 0xfe) = F<uint32_t>(g_input, 0xa94);
    F<uint32_t>(c, 0x102) = F<uint32_t>(g_input, 0xa98);
    float x = 0.0f, y = 0.0f;
    if (F<void*>(Renderer(), 0x366e)) {
        F<int32_t>(c, 0xea) = 1;
        x = (float)Obj_CellX(F<void*>(Renderer(), 0x366e));
        y = (float)Obj_CellY(F<void*>(Renderer(), 0x366e));
    }
    bool height = F<int32_t>(c, 0xea) != 0;
    if (!height) {
        int32_t ok = 0;
        if (Scene_FocusCell(Renderer(), 0, &x, &y) == 1 && !(x < Fl(0x5c8598)) && !(y < Fl(0x5c8598))) {
            uint8_t* m = g_map;
            if (x < (float)F<int32_t>(m, 0x2155) && y < (float)F<int32_t>(m, 0x2159)) ok = 1;
        }
        F<int32_t>(c, 0xea) = ok;
        height = ok != 0;
    }
    if (height) F<float>(c, 0xf2) = (float)Terrain_HeightAtTile(g_map, 0, x, y);
    float k = Fl(0x5c8700);
    F<float>(c, 0xee) = (float)((double)x * k);
    F<float>(c, 0xf6) = (float)((double)y * k);
    if (!F<int32_t>(c, 0xea)) {
        F<uint32_t>(c, 0xee) = F<uint32_t>(c, 4);
        F<uint32_t>(c, 0xf2) = F<uint32_t>(c, 8);
        F<uint32_t>(c, 0xf6) = F<uint32_t>(c, 0xc);
        F<int32_t>(c, 0xea) = 1;
        float tz = (float)((double)F<float>(c, 0xc) / k);
        float tx = (float)((double)F<float>(c, 4) / k);
        if (!Map_TileToScreen(tx, tz, F<float>(c, 8), &F<float>(c, 0xfe), &F<float>(c, 0x102))) {
            F<float>(c, 0xfe) = 0.5f;
            F<float>(c, 0x102) = (float)(((double)F<float>(c, 0x5c) - F<float>(c, 0x54)) * D(0x5c8628));
        }
    }
    F<uint32_t>(c, 0xf2) = F<uint32_t>(c, 0x20);
    F<float>(c, 0xfa) = (float)Distance3D(F<float>(c, 0xee), F<float>(c, 0x20), F<float>(c, 0xf6), F<float>(c, 0x1c),
                                          F<float>(c, 0x20), F<float>(c, 0x24));
    float dx = (float)((double)F<float>(c, 0xee) - F<float>(c, 0x1c));
    if (dx == Fl(0x5c8598)) dx = 0.1f;
    float a = (float)(D(0x5c8740) - ((double(__cdecl*)(double))0x5a1270)(((double)F<float>(c, 0xf6) - F<float>(c, 0x24)) / dx));
    F<float>(c, 0x106) = a;
    if (F<float>(c, 0xee) < F<float>(c, 0x1c)) F<float>(c, 0x106) = (float)((double)a - Fl(0x5c8760));
    float m = (float)((double(__cdecl*)(double, double))0x5a1390)(F<float>(c, 0x106), 6.2831854820251465);
    F<float>(c, 0x106) = m;
    F<float>(c, 0x10a) = (float)((double)F<float>(c, 0x10) - m);
}
RT3_REPLACE(0x43d480, Camera_UpdateFocus);

// 0x43d740. Camera c: applies the held camera keys (each a key and an
// optional modifier, +0xa6..+0xe2), at most every 8 ms with the elapsed
// time capped at 200 ms (0x62be74): turning (+0xd6 / +0xde) at a third of
// a turn per second; zooming (+0xc6 / +0xce) reports the zoom step in
// *zoom, a zoom-in beyond the close limit (height 42.5, nothing followed)
// pans instead; panning (+0xb6 / +0xbe sideways, +0xa6 / +0xae forward)
// at the options' scroll speed (+0x11c) stops following first and only
// moves while the game is in front. Nothing while the window manager is
// busy.
RT3_NOINLINE void __fastcall Camera_KeyControl(uint8_t* c, int, uint32_t* zoom) {
    if (g_windowManager && !Obj_Idlec60(g_windowManager)) return;
    int32_t zooming = 0, turning = 0;
    *zoom = 0;
    int32_t now = ClockMs();
    int32_t dt = now - g_62be74;
    if (dt < 8) return;
    if (dt > 200) dt = 200;
    g_62be74 = now;
    float dx = 0.0f, dz = 0.0f;
    auto held = [&](uint32_t key, uint32_t mod) {
        if (!Input_Key(g_input, 0, F<int32_t>(c, key))) return -1;
        return !F<int32_t>(c, mod) || Input_Key(g_input, 0, F<int32_t>(c, mod)) ? 1 : 0;
    };
    int32_t k = held(0xb6, 0xba);
    if (k == 1) dx = (float)((double)((const float*)0x5f34b0)[F<int32_t>(g_app, 0x11c)] * D(0x5c9bf0));
    else if (k < 0 && held(0xbe, 0xc2) == 1) dx = ((const float*)0x5f34b0)[F<int32_t>(g_app, 0x11c)];
    k = held(0xa6, 0xaa);
    if (k == 1) dz = (float)((double)((const float*)0x5f34b0)[F<int32_t>(g_app, 0x11c)] * D(0x5c9bf0));
    else if (k < 0 && held(0xae, 0xb2) == 1) dz = ((const float*)0x5f34b0)[F<int32_t>(g_app, 0x11c)];
    k = held(0xd6, 0xda);
    if (k == 1) {
        dx = -1.0f;
        turning = 1;
    } else if (k < 0 && held(0xde, 0xe2) == 1) {
        dx = 1.0f;
        turning = 1;
    }
    k = held(0xc6, 0xca);
    if (k == 1) {
        dz = -1.0f;
        zooming = 1;
    } else if (k < 0 && held(0xce, 0xd2) == 1) {
        dz = 1.0f;
        zooming = 1;
    }
    if (dx == 0.0f && dz == 0.0f) return;
    if (turning) {
        float t = (float)dt;
        t = (float)((double)t * Fl_207(0x5c8768) / D(0x5c87f8) / D(0x5c85c8));
        Camera_Turn(c, 0, (float)((double)t * dx));
        return;
    }
    if (zooming) {
        float step = (float)((double)dt / D(0x5c87f8) * ((double)dz * Fl_207(0x5ca164)));
        *(float*)zoom = step;
        if (!(step < 0.0f)) return;
        if (!((double)F<float>(c, 0x18) <= D(0x5ca238))) return;
        if (F<void*>(g_62be68, 0x60)) return;
        dz = -1.0f;
        *zoom = 0;
    }
    World_SetFollow(g_world, 0, 0, nullptr);
    float s = (float)((double)dt / D(0x5c87f8) * D(0x5c8628));
    if (Engine_IsForeground()) Camera_Pan(c, 0, (float)((double)s * dx), (float)((double)s * dz));
}
RT3_REPLACE(0x43d740, Camera_KeyControl);

// 0x43db00. Camera c, once per frame while a game runs (0x6cec74 +8 == 1 and a
// world): keys, then the mouse - its movement since the last frame (+0x2f6 /
// +0x2fa / wheel +0x2fe, scaled by 0x5ca248; the wheel by 0x5c9b08 unless the
// engine flag +0x1146d7) becomes the move (dx, dy) and zoom (dz, plus the
// keys' zoom). With stepped zooming (+0x28b) a big wheel turn zooms whole
// steps instead. Grabbing (Tool_AddModifier) with a move stops following, and
// on the first grab frame remembers the grabbed ground point (+0x312..+0x322)
// and view (+0x326). Ending a grab or rotation hands control to the list
// panel. The move, clamped to +-95, is applied with mouse or momentum input,
// then the camera steps in 0x5c878c slices of the smoothed frame time
// (0x5ee4f8 / 0x62be78); tiny speeds snap to 0. Without any input or speed
// the grab state is cleared instead. `forced` is held in +0xa2 meanwhile.
RT3_NOINLINE void __fastcall Camera_MouseUpdate(uint8_t* c, int, int32_t forced) {
    if (F<int32_t>(g_app, 8) != 1 || !*hook::As<int32_t*>(g_world)) return;
    F<int32_t>(c, 0xa2) = forced;
    float zoom = 0.0f;
    Camera_KeyControl(c, 0, (uint32_t*)&zoom);
    Mouse_Poll((uint8_t*)Engine_Input(hook::As<void*>(g_engine)));
    float x, y, z;
    Mouse_GetPos3((uint8_t*)Engine_Input(hook::As<void*>(g_engine)), 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    if (ClockMs() < F<int32_t>(c, 0x346) + 0x2bc) F<float>(c, 0x2fe) = z;
    int32_t first = 0;
    float dx, dy, dz;
    if (!F<int32_t>(c, 0x2f2)) {
        dz = 0.0f, dy = 0.0f, dx = 0.0f;
        F<int32_t>(c, 0x2f2) = 1;
        first = 1;
    } else {
        const float k = g_5ca248;
        dx = (x - F<float>(c, 0x2f6)) * k;
        dy = (y - F<float>(c, 0x2fa)) * k;
        dz = (z - F<float>(c, 0x2fe)) * k;
        if (!F<uint8_t>(g_engine, 0x1146d7)) dz = dz * g_5c9b08;
        if (F<int32_t>(g_app, 0x28b) && fabs((double)dz) > g_5c86b8) {
            int32_t up = dz > g_5c8598 ? 1 : 0;
            int32_t steps = Ftol(fabs((double)dz) / g_5c8908);
            if (steps <= 1) steps = 1;
            Camera_ZoomStep(c, 0, (up * 2 - 1) * steps);
            dz = 0.0f;
        }
        dz = dz + zoom;
    }
    if (g_62be70) dx = 0.0f, dy = 0.0f, dz = 0.0f;
    const float zero = g_5c8598;
    if (Tool_AddModifier(c) && (dx != zero || dy != zero || dz != zero)) World_SetFollow(g_world, 0, 0, nullptr);
    if (Tool_AddModifier(c) && !F<int32_t>(c, 0x336)) {
        F<int32_t>(c, 0x312) = 0;
        F<int32_t>(c, 0x316) = 0;
        F<int32_t>(c, 0x31a) = 0;
        F<uint32_t>(c, 0x326) = F<uint32_t>(g_input, 0xa94);
        F<uint32_t>(c, 0x32a) = F<uint32_t>(g_input, 0xa98);
        float fx, fy;
        if (Scene_FocusFlagIs1(&fx, &fy) && fx >= zero && fy >= zero) {
            uint8_t* m = g_map;
            float w = (float)F<int32_t>(m, 0x2155);
            if (fx < w) {
                float h = (float)F<int32_t>(m, 0x2159);
                if (fy < h) {
                    float g = (float)Terrain_HeightAtTile(m, 0, fx, fy);
                    F<float>(c, 0x316) = g;
                    F<float>(c, 0x31a) = g - F<float>(c, 0x20);
                    F<int32_t>(c, 0x312) = 1;
                    F<float>(c, 0x31e) = fx;
                    F<float>(c, 0x322) = fy;
                }
            }
        }
    }
    if ((!Tool_AddModifier(c) && F<int32_t>(c, 0x336)) || (!Tool_RemoveModifier() && F<int32_t>(c, 0x33a)))
        if (uint8_t* p = g_6d0820) Obj_TakeListCommand(p);
    F<int32_t>(c, 0x336) = Tool_AddModifier(c);
    F<int32_t>(c, 0x33a) = Tool_RemoveModifier();
    F<float>(c, 0x2fe) = z;
    F<float>(c, 0x2f6) = x;
    F<float>(c, 0x2fa) = y;
    if (dx > g_5ca244) dx = BitsF(0x42be0000);
    else if (dx < g_5ca240) dx = BitsF(0xc2be0000);
    if (dy > g_5ca244) dy = BitsF(0x42be0000);
    else if (dy < g_5ca240) dy = BitsF(0xc2be0000);
    int32_t elapsed = ClockMs() - g_62be7c;
    g_62be7c = ClockMs();
    if (elapsed > 200) elapsed = 200;
    g_5ee4f8 = (float)((double)elapsed / g_5c8a20 + g_5ee4f8 * g_5c8928);
    g_62be78 = g_62be78 + g_5ee4f8;
    const float slice = g_5c878c;
    if (g_62be78 < slice) g_62be78 = 2.0f;
    bool move;
    Engine_Input(hook::As<void*>(g_engine));
    bool keys = Input_Flag800();
    if (!keys) {
        Engine_Input(hook::As<void*>(g_engine));
        keys = Input_Flag400();
    }
    if (!keys) {
        Engine_Input(hook::As<void*>(g_engine));
        keys = Input_Flag2000();
    }
    if ((keys || dz != zero) && (dx != zero || dy != zero || dz != zero)) move = true;
    else
        move = F<float>(c, 0x8a) != zero || F<float>(c, 0x8e) != zero || F<float>(c, 0x92) != zero || F<float>(c, 0x96) != zero ||
               F<float>(c, 0x9a) != zero || first || F<int32_t>(c, 0x60) || F<int32_t>(c, 0xa2);
    if (!move) {
        if (!Tool_AddModifier(c)) F<int32_t>(c, 0x302) = 0;
        if (!Tool_RemoveModifier()) F<int32_t>(c, 0x306) = 0;
        hook::As<int32_t>(g_62be78) = 0;
    } else {
        if (OptionTable10c()) Camera_MouseInput(c, 0, dx, dy, dz);
        else Camera_MouseInputMomentum(c, 0, dx, dy, dz);
        while (g_62be78 > slice) {
            float dt = BitsF(0x3dcccccd);
            g_62be78 = g_62be78 - slice;
            int32_t last = g_62be78 > slice ? 0 : 1;
            if (Options_Get10c() != 4) {
                F<int32_t>(c, 0x9a) = 0;
                F<int32_t>(c, 0x96) = 0;
                hook::As<int32_t>(g_62be78) = 0;
                dt = 1.0f;
                last = 1;
            }
            if (OptionTable10c()) Camera_Step(c, 0, dt, last);
            else Camera_ApplyMomentum(c, 0, dt, last);
        }
        const double tiny = g_5c8678;
        for (int32_t off = 0x8a; off <= 0x9a; off += 4)
            if (fabs((double)F<float>(c, off)) < tiny) F<int32_t>(c, off) = 0;
    }
    F<uint32_t>(c, 0x32e) = F<uint32_t>(g_input, 0xa94);
    F<uint32_t>(c, 0x332) = F<uint32_t>(g_input, 0xa98);
    F<int32_t>(c, 0xe6) = Tool_RemoveModifier();
    F<int32_t>(c, 0xa2) = 0;
    Camera_UpdateShake(c);
}
RT3_REPLACE(0x43db00, Camera_MouseUpdate);

// 0x43e2c0. Applies the camera's momentum over `dt`: zoom (+0x18, rate +0x92,
// scaled with the altitude unless in camera mode 4), yaw (+0x10, +0x8a), pitch
// (+0x14, +0x8e, clamped; at least 0.1 unless options +0x104) and, with `move`,
// position (+0x4 / +0xc, +0x96 / +0x9a; followed onto the tracked object +0x60
// or the ground), then lets the rates decay.
RT3_NOINLINE void __fastcall Camera_ApplyMomentum(uint8_t* c, int, float dt, int32_t move) {
    if (Options_Get10c() == 4)
        F<float>(c, 0x18) = (float)((double)dt * F<float>(c, 0x92) + F<float>(c, 0x18));
    else {
        float k = F<float>(c, 0x18);
        if (k < D(0x5ca0d0)) k = FromBits(0x42160000);
        k = (float)((double)k - (F<int32_t>(c, 0x60) ? D(0x5c8d58) : D(0x5ca268)));
        k = (float)(((double(__cdecl*)(double))0x5a1634)((double)k / Fl(0x5c9ac4)) * D(0x5c88e8) + D(0x5c85b8));
        if (F<float>(c, 0x18) < Fl(0x5ca06c)) k = (float)((double)k * D(0x5c88c8));
        else if (F<float>(c, 0x18) < Fl(0x5ca1a4))
            k = (float)((((double)F<float>(c, 0x18) - Fl(0x5ca06c)) / D(0x5c87f8) * D(0x5c8680) + D(0x5c88c8)) * k);
        k = (float)((double)F<float>(c, 0x92) * k * dt * D(0x5c85a8));
        F<float>(c, 0x18) = (float)((double)k + F<float>(c, 0x18));
    }
    F<float>(c, 0x10) = (float)((double)dt * F<float>(c, 0x8a) + F<float>(c, 0x10));
    float pitch = (float)((double)dt * F<float>(c, 0x8e) + F<float>(c, 0x14));
    F<float>(c, 0x14) = pitch;
    if (F<float>(c, 0x18) < D(0x5ca0d0)) F<uint32_t>(c, 0x18) = 0x42160000;
    else if (F<float>(c, 0x18) > Fl(0x5ca260)) F<uint32_t>(c, 0x18) = 0x4703a400;
    if (pitch < D(0x5ca258)) F<uint32_t>(c, 0x14) = 0xbf8ccccd;
    else if (pitch > D(0x5ca028)) F<uint32_t>(c, 0x14) = 0x3fbc4303;
    if (!F<int32_t>(g_app, 0x104) && F<float>(c, 0x14) < D(0x5c86b8)) F<uint32_t>(c, 0x14) = 0x3dcccccd;
    F<float>(c, 4) = (float)((double)dt * F<float>(c, 0x96) + F<float>(c, 4));
    F<float>(c, 0xc) = (float)((double)dt * F<float>(c, 0x9a) + F<float>(c, 0xc));
    if (move) {
        uint8_t* follow = F<uint8_t*>(c, 0x60);
        if (follow) {
            ((void(__fastcall*)(uint8_t*, int, float*, float*, float*))(*(void***)follow)[2])(follow, 0, &F<float>(c, 4), &F<float>(c, 8), &F<float>(c, 0xc));
            Camera_ClampToMap(c, 0, 1);
        } else {
            Camera_ClampToMap(c, 0, 1);
            F<float>(c, 8) = (float)(Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 4), F<float>(c, 0xc)) + Fl(0x5c8898));
        }
        Camera_UpdateTarget(c);
        Camera_ClampToGround(c);
        Camera_Update(c);
    }
    if (Options_Get10c() == 4) {
        double k = D(0x5c8770) - (double)dt * D(0x5c86b8);
        F<float>(c, 0x8a) = (float)((double)F<float>(c, 0x8a) * k);
        F<float>(c, 0x8e) = (float)((double)F<float>(c, 0x8e) * k);
        F<float>(c, 0x92) = (float)((D(0x5c8770) - (double)dt * D(0x5ca250)) * F<float>(c, 0x92));
        k = D(0x5c8770) - (double)dt * D(0x5c8648);
        F<float>(c, 0x96) = (float)((double)F<float>(c, 0x96) * k);
        F<float>(c, 0x9a) = (float)((double)F<float>(c, 0x9a) * k);
    } else {
        F<float>(c, 0x8a) = (float)((D(0x5c8770) - (double)dt * D(0x5c88c8)) * F<float>(c, 0x8a));
        F<float>(c, 0x8e) = (float)((D(0x5c8770) - (double)dt * D(0x5c8758)) * F<float>(c, 0x8e));
        F<float>(c, 0x92) = (float)((D(0x5c8770) - (double)dt * D(0x5c8618)) * F<float>(c, 0x92));
        double k = D(0x5c8770) - (double)dt * D(0x5c8590);
        F<float>(c, 0x96) = (float)((double)F<float>(c, 0x96) * k);
        F<float>(c, 0x9a) = (float)((double)F<float>(c, 0x9a) * k);
    }
}
RT3_REPLACE(0x43e2c0, Camera_ApplyMomentum);

// 0x43e610. Mouse input for the momentum cameras: a free drag adds (dx, dy)
// to the turn and pitch rates (finer in camera mode 4); with the grab
// modifier mode 4 pushes the position rates (+0x96 / +0x9a) along the view,
// scaled with the height over the ground, other modes drag the map to the
// mouse; the wheel adds to the zoom rate (scaled with the height in mode 4).
RT3_NOINLINE void __fastcall Camera_MouseInputMomentum(uint8_t* c, int, float dx, float dy, float wheel) {
    int32_t freeDrag = Tool_RemoveModifier();
    int32_t grab = Tool_AddModifier(c);
    if (!freeDrag && !grab && CamFabs(wheel) <= D(0x5c8678)) return;
    float ground = (float)Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, F<float>(c, 4), F<float>(c, 0xc));
    float k = (float)(CamFabs((double)F<float>(c, 0x20) - ground) / D(0x5c87f8) + D(0x5c9ba0));
    if (freeDrag) {
        if (Options_Get10c() == 4) {
            F<float>(c, 0x8a) = (float)((double)dx / Fl(0x5c86a0) + F<float>(c, 0x8a));
            F<float>(c, 0x8e) = (float)((double)dy / Fl(0x5c88e0) + F<float>(c, 0x8e));
        } else {
            F<float>(c, 0x8a) = (float)((double)dx / Fl(0x5ca19c) + F<float>(c, 0x8a));
            F<float>(c, 0x8e) = (float)((double)dy / Fl(0x5ca270) + F<float>(c, 0x8e));
        }
    } else if (grab) {
        if (Options_Get10c() == 4) {
            auto sin_ = (double(__cdecl*)(double))0x5a1494;
            auto cos_ = (double(__cdecl*)(double))0x5a13e4;
            float a = (float)((double)k * dy);
            double t = F<float>(c, 0x96);
            F<float>(c, 0x96) = (float)(t - cos_(F<float>(c, 0x10)) * a);
            t = F<float>(c, 0x9a);
            F<float>(c, 0x9a) = (float)(t - sin_(F<float>(c, 0x10)) * a);
            float b = (float)((double)k * dx);
            F<float>(c, 0x96) = (float)(sin_(F<float>(c, 0x10)) * b + F<float>(c, 0x96));
            t = F<float>(c, 0x9a);
            F<float>(c, 0x9a) = (float)(t - cos_(F<float>(c, 0x10)) * b);
        } else
            Camera_DragToMouse(c);
    }
    if (wheel == Fl(0x5c8598)) return;
    double v;
    if (Options_Get10c() == 4)
        v = (double)k * D(0x5c86b8) * wheel * hook::Global<float>(0x5f34b0 + F<int32_t>(g_app, 0x118) * 4);
    else {
        float s = hook::Global<float>(0x5f34b0 + F<int32_t>(g_app, 0x118) * 4);
        v = s * ((double)wheel * D(0x5c8628));
    }
    F<float>(c, 0x92) = (float)(v + v + F<float>(c, 0x92));
}
RT3_REPLACE(0x43e610, Camera_MouseInputMomentum);

// 0x43e830. Mouse drag of the camera: on the first move of a drag (+0x306 clear)
// decides which axes follow (+0x33e horizontal, +0x342 vertical) from the dominant
// direction; an axis left out joins once its accumulated motion dominates enough.
// Horizontal turns the yaw (+0x8a), vertical the pitch (+0x92) by the speed option.
RT3_NOINLINE void __fastcall Camera_Drag(uint8_t* c, int, float dx, float dy, int32_t) {
    const double k = D(0x5c85c8);
    if (!F<int32_t>(c, 0x306)) {
        F<int32_t>(c, 0x30a) = 0;
        F<int32_t>(c, 0x30e) = 0;
        F<int32_t>(c, 0x33e) = 0;
        F<int32_t>(c, 0x342) = 0;
        if (Fabs(dy) * k < Fabs(dx)) F<int32_t>(c, 0x33e) = 1;
        else if (Fabs(dx) * k < Fabs(dy)) F<int32_t>(c, 0x342) = 1;
        else {
            if (Fabs(dx) > D(0x5c85f0)) F<int32_t>(c, 0x33e) = 1;
            if (Fabs(dy) > D(0x5c85f0)) F<int32_t>(c, 0x342) = 1;
        }
    }
    float sx = (float)((double)dx + F<float>(c, 0x30a));
    F<float>(c, 0x30a) = sx;
    F<float>(c, 0x30e) = (float)((double)dy + F<float>(c, 0x30e));
    if (!F<int32_t>(c, 0x33e)) {
        double t = Fabs(sx) * k;
        if (Fabs(F<float>(c, 0x30e)) < t) F<int32_t>(c, 0x33e) = 1;
        F<float>(c, 0x30a) = dx;
    }
    if (!F<int32_t>(c, 0x342)) {
        double t = Fabs(F<float>(c, 0x30e)) * k;
        if (Fabs(F<float>(c, 0x30a)) < t) F<int32_t>(c, 0x342) = 1;
        F<float>(c, 0x30e) = dy;
    }
    if (F<int32_t>(c, 0x33e)) F<float>(c, 0x8a) = (float)((double)dx / Fl(0x5ca19c) + F<float>(c, 0x8a));
    if (F<int32_t>(c, 0x342)) {
        float speed = ((const float*)0x5f34b0)[F<int32_t>(g_app, 0x118)];
        F<float>(c, 0x92) = (float)(F<float>(c, 0x92) - (double)dy * speed * D(0x5ca278));
    }
    F<int32_t>(c, 0x306) = 1;
}
RT3_REPLACE(0x43e830, Camera_Drag);

// 0x43ea40. Drags the map: the mouse moved from (x0, y0) (kept in +0x32e /
// +0x332) to (x1, y1). Picks the ground under both points at a few heights
// (the drag plane +0x31a above the camera's +0x20, falling back to 50 below,
// one unit below, and the ground) along the screen's middle column and the
// row band (+0x326, +0x32a), blends the height-based shift (weight by the
// altitude) with the direct one, and moves the camera by it. With +0x312 set
// and the blend nearly direct, it checks that the grabbed tile (+0x31e /
// +0x322) is back under the mouse and, when not, recurses once (`nested`)
// with a correction of at most 0.0015 (or half the drag distance).
RT3_NOINLINE void __fastcall Camera_DragPick(uint8_t* c, int, uint32_t x0, uint32_t y0, float x1, float y1, int32_t nested) {
    F<uint32_t>(c, 0x32e) = x0;
    F<uint32_t>(c, 0x332) = y0;
    Obj_Scene36f9((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)));
    float lowY = F<float>(c, 0x332) < y1 ? F<float>(c, 0x332) : y1;
    float probe = lowY;
    if (F<float>(c, 0x32a) < lowY && F<float>(c, 0x31a) > Fl(0x5c9f80)) probe = F<float>(c, 0x32a);
    float alt = Obj_ScaledSpanOffset(c, 0, probe);
    float frac;
    if (alt < D(0x5c86b8)) frac = 0.0f;
    else if (alt < D(0x5c8988)) frac = (float)(((double)alt - D(0x5c86b8)) / D(0x5ca290));
    else frac = 1.0f;
    float shift[2] = { 0.0f, 0.0f }, direct[2] = { 0.0f, 0.0f };
    int32_t found = 0;
    float ax0 = 0.0f, ax1 = 0.0f, az0 = 0.0f, az1 = 0.0f;  // pick outputs (+0x2c / +0x34 for the start, +0x28 / +0x30 for the end)
    auto plane = [&](int32_t pass) {
        float lvl = Fl(0x5c9f80) < F<float>(c, 0x31a) ? FromBits(0xc2480000) : F<float>(c, 0x31a);
        float v = (float)((double)lvl + F<float>(c, 0x20));
        uint32_t h = v < Fl(0x5c8598) ? 0 : Bits(v);
        if (pass == 1) h = Bits((float)((double)F<float>(c, 0x20) - Fl(0x5c8780)));
        else if (pass == 2) h = 0;
        return FromBits(h);
    };
    for (int32_t pass = 0; pass < 3; ++pass) {
        float h = plane(pass);
        float half = (float)(((double)F<float>(c, 0x5c) - F<float>(c, 0x54)) * D(0x5c8628));
        float sy = (float)(pass ? (double)half : ((double)half + F<float>(c, 0x32a)) * D(0x5c8628));
        int32_t r1 = Camera_PickGround(c, 0, F<float>(c, 0x32e), sy, h, &ax0, &az0);
        sy = (float)(pass ? (double)half : ((double)half + F<float>(c, 0x32a)) * D(0x5c8628));
        int32_t r2 = Camera_PickGround(c, 0, x1, sy, h, &ax1, &az1);
        if (!r1 || !r2) continue;
        direct[0] = (float)((double)ax1 - ax0);
        direct[1] = (float)((double)az1 - az0);
        float h2 = plane(pass);
        r1 = Camera_PickGround(c, 0, F<float>(c, 0x326), half, h2, &ax0, &az0);
        float hp = (float)((double)half + D(0x5c85b8));
        r2 = Camera_PickGround(c, 0, F<float>(c, 0x326), hp, h2, &ax1, &az1);
        if (r1 && r2) {
            found = 1;
            float q = (float)(((double)y1 - F<float>(c, 0x332)) / D(0x5c85b8));
            direct[0] = (float)(((double)ax1 - ax0) * q + direct[0]);
            direct[1] = (float)(((double)az1 - az0) * q + direct[1]);
            break;
        }
    }
    float top = (float)((double)F<float>(c, 0x31a) + F<float>(c, 0x20));
    int32_t r1 = Camera_PickGround(c, 0, F<float>(c, 0x32e), F<float>(c, 0x332), top, &ax0, &az0);
    int32_t r2 = Camera_PickGround(c, 0, x1, y1, top, &ax1, &az1);
    if (r1 && r2) {
        shift[0] = (float)((double)ax1 - ax0);
        shift[1] = (float)((double)az1 - az0);
        if (!found) frac = 1.0f;
    } else {
        if (!found) return;
        frac = 0.0f;
    }
    double k = D(0x5c8770) - frac;
    float dx = (float)((double)direct[0] * k + (double)shift[0] * frac);
    float dz = (float)((double)direct[1] * k + (double)shift[1] * frac);
    uint32_t z = Bits((float)((double)F<float>(c, 0xc) - (double)dz * Fl(0x5c8700)));
    Camera_MoveTo(c, 0, Bits((float)((double)F<float>(c, 4) - (double)dx * Fl(0x5c8700))), F<uint32_t>(c, 8), z);
    float sx, sy;
    if (F<int32_t>(c, 0x312) && frac >= D(0x5ca288) && !nested && Map_TileToScreen(F<float>(c, 0x31e), F<float>(c, 0x322), top, &sx, &sy)) {
        float e1 = (float)((double)sx - x1);
        float e2 = (float)((double)sy - y1);
        if (!(CamFabs(e1) > D(0x5c8758)) && !(CamFabs(e2) > D(0x5c8758))) {
            float lim = FromBits(0x3ac49ba6);
            double d = (float)Distance2D(x1, y1, F<float>(c, 0x32e), F<float>(c, 0x332)) * D(0x5c8628);
            if (D(0x5ca280) > d) lim = (float)d;
            if (e1 > lim) e1 = lim;
            float nl = -lim;
            if (e1 < nl) e1 = nl;
            if (e2 > lim) e2 = lim;
            if (e2 < nl) e2 = nl;
            float nx = (float)((double)e1 + x1);
            F<float>(c, 0x32e) = nx;
            float nz = (float)((double)e2 + y1);
            F<float>(c, 0x332) = nz;
            Camera_DragPick(c, 0, Bits(nx), Bits(nz), x1, y1, 1);
            F<float>(c, 0x32e) = x1;
            F<float>(c, 0x332) = y1;
        }
    }
    Camera_Update(c);
}
RT3_REPLACE(0x43ea40, Camera_DragPick);

// 0x43f050. The camera step of the follow / orbit modes over `dt`: tilts the
// view vector (+0x2c..+0x34) with the yaw rate, turns (orbiting when options
// +0x108), pitches (the pitch rate +0x8e is used up), zooms (scaled with the
// altitude, at least 37.5); with `move` follows the tracked object +0x60 (moving
// when it is more than 0.5 away) or drifts by the position rates; refreshes the
// view when anything moved or +0x9e asks for it; then lets the rates decay.
RT3_NOINLINE void __fastcall Camera_Step(uint8_t* c, int, float dt, int32_t move) {
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    double t = F<float>(c, 0x2c);
    double s14 = sin_(F<float>(c, 0x14));
    F<float>(c, 0x2c) = (float)(t - sin_(F<float>(c, 0x10)) * s14 * F<float>(c, 0x8a) * D(0x5c8628));
    t = F<float>(c, 0x34);
    double k = cos_(-(double)F<float>(c, 0x10));
    F<float>(c, 0x34) = (float)(t - sin_(F<float>(c, 0x14)) * k * F<float>(c, 0x8a) * D(0x5c8628));
    k = cos_(F<float>(c, 0x10));
    k = sin_(F<float>(c, 0x14)) * k;
    F<float>(c, 0x30) = (float)(D(0x5c8770) - sin_(F<float>(c, 0x10)) * k * F<float>(c, 0x8a));
    F<float>(c, 0x2c) = (float)((double)F<float>(c, 0x2c) * D(0x5c8680));
    F<float>(c, 0x34) = (float)((double)F<float>(c, 0x34) * D(0x5c8680));
    double turn = (double)dt * F<float>(c, 0x8a);
    if (F<int32_t>(g_app, 0x108)) Camera_Orbit(c, 0, (float)turn);
    else F<float>(c, 0x10) = (float)(turn + F<float>(c, 0x10));
    double pitch = (double)dt * F<float>(c, 0x8e);
    F<uint32_t>(c, 0x8e) = 0;
    F<float>(c, 0x14) = (float)(pitch + F<float>(c, 0x14));
    float l = F<float>(c, 0x18);
    if (l < D(0x5ca0d0)) l = FromBits(0x42160000);
    l = (float)((double)l - (F<int32_t>(c, 0x60) ? D(0x5c8d58) : D(0x5ca268)));
    l = (float)(((double(__cdecl*)(double))0x5a1634)((double)l / Fl(0x5c9ac4)) * D(0x5c88e8) + D(0x5c85b8));
    if (l > D(0x5c8820)) l = (float)((((double)l - D(0x5c8820)) * D(0x5c8680) + D(0x5c8770)) * l);
    float m = (float)((double)F<float>(c, 0x92) * l * dt * D(0x5c85a8));
    float z = (float)((double)m + F<float>(c, 0x18));
    F<float>(c, 0x18) = z;
    F<float>(c, 0x18) = (float)(z > D(0x5ca0d0) ? (double)z : 37.5);
    if (!move) {
        F<float>(c, 4) = (float)((double)dt * F<float>(c, 0x96) + F<float>(c, 4));
        F<float>(c, 0xc) = (float)((double)dt * F<float>(c, 0x9a) + F<float>(c, 0xc));
    } else {
        if (uint8_t* follow = F<uint8_t*>(c, 0x60)) {
            float fx, fy, fz;
            ((void(__fastcall*)(uint8_t*, int, float*, float*, float*))(*(void***)follow)[2])(follow, 0, &fx, &fy, &fz);
            fy = (float)((double)fy + Fl(0x5c873c));
            if (CamFabs((double)fx - F<float>(c, 4)) > D(0x5c85b8) || CamFabs((double)fy - F<float>(c, 8)) > D(0x5c85b8) ||
                CamFabs((double)fz - F<float>(c, 0xc)) > D(0x5c85b8))
                Camera_MoveTo(c, 0, Bits(fx), Bits(fy), Bits(fz));
            uint32_t a, b, d;
            SceneObj_GetAngles(F<uint8_t*>(c, 0x60), 0, &d, &b, &a);
        } else if (F<float>(c, 0x96) != Fl(0x5c8598) || F<float>(c, 0x9a) != Fl(0x5c8598)) {
            F<float>(c, 4) = (float)((double)dt * F<float>(c, 0x96) + F<float>(c, 4));
            F<float>(c, 0xc) = (float)((double)dt * F<float>(c, 0x9a) + F<float>(c, 0xc));
            Camera_ClampToMap(c, 0, 1);
        }
        const float zero = Fl(0x5c8598);
        if (F<int32_t>(c, 0x9e) || F<float>(c, 0x8a) != zero || F<float>(c, 0x8e) != zero || F<float>(c, 0x92) != zero || F<float>(c, 0x96) != zero ||
            F<float>(c, 0x9a) != zero) {
            F<int32_t>(c, 0x9e) = 0;
            Camera_SmoothHeight(c);
            Camera_SetAngles(c, 0, F<uint32_t>(c, 0x10), F<float>(c, 0x14), F<uint32_t>(c, 0x18));
            Camera_ClampToGround(c);
            Camera_Update(c);
        }
    }
    F<float>(c, 0x8a) = (float)((D(0x5c8770) - (double)dt * D(0x5c88c8)) * F<float>(c, 0x8a));
    F<float>(c, 0x8e) = (float)((D(0x5c8770) - (double)dt * D(0x5c8758)) * F<float>(c, 0x8e));
    F<float>(c, 0x92) = (float)((D(0x5c8770) - (double)dt * D(0x5c8618)) * F<float>(c, 0x92));
    double r = D(0x5c8770) - (double)dt * D(0x5c8590);
    F<float>(c, 0x96) = (float)((double)F<float>(c, 0x96) * r);
    F<float>(c, 0x9a) = (float)((double)F<float>(c, 0x9a) * r);
}
RT3_REPLACE(0x43f050, Camera_Step);

// 0x43f4c0. Continues a map drag to the current mouse position (0x6d4018
// +0xa94 / +0xa98).
RT3_NOINLINE void __fastcall Camera_DragToMouse(uint8_t* c) {
    uint8_t* m = g_input;
    Camera_DragPick(c, 0, F<uint32_t>(c, 0x32e), F<uint32_t>(c, 0x332), F<float>(m, 0xa94), F<float>(m, 0xa98), 0);
}
RT3_REPLACE(0x43f4c0, Camera_DragToMouse);

// 0x43f4f0. Applies the mouse to the camera: while the drag modifier is held
// (Tool_RemoveModifier) it is a free drag by (dx, dy) (slower unless the
// screen's +0x1146d7), otherwise orbiting stops and, with the grab modifier,
// the map follows the mouse; the wheel `wheel` adds to the zoom rate scaled
// by the zoom speed option +0x118.
RT3_NOINLINE void __fastcall Camera_MouseInput(uint8_t* c, int, float dx, float dy, float wheel) {
    int32_t freeDrag = Tool_RemoveModifier();
    int32_t grab = Tool_AddModifier(c);
    if (!freeDrag) {
        if (!grab && CamFabs(wheel) <= D(0x5c8678)) return;
        F<int32_t>(c, 0xea) = 0;
        F<int32_t>(c, 0x8a) = 0;
        F<int32_t>(c, 0x306) = 0;
    } else {
        if (!F<int32_t>(c, 0xe6)) Camera_UpdateFocus(c);
        if (!g_engine[0x1146d7]) dy = (float)((double)dy * Fl(0x5c9b08));
        Camera_Drag(c, 0, dx, dy, *(int32_t*)&wheel);
    }
    F<int32_t>(c, 0xe6) = freeDrag;
    if (grab) {
        uint8_t* m = g_input;
        Camera_DragPick(c, 0, F<uint32_t>(c, 0x32e), F<uint32_t>(c, 0x332), F<float>(m, 0xa94), F<float>(m, 0xa98), 0);
    } else
        F<int32_t>(c, 0x302) = 0;
    if (wheel == Fl(0x5c8598)) return;
    float k = hook::Global<float>(0x5f34b0 + F<int32_t>(g_app, 0x118) * 4);
    double v = k * ((double)wheel * D(0x5c8628));
    F<float>(c, 0x92) = (float)(v + v + F<float>(c, 0x92));
}
RT3_REPLACE(0x43f4f0, Camera_MouseInput);

void ResetPair5ee4fc() {
    hook::As<int32_t>(g_5ee4fc) = -1;
    g_5ee500 = -1;
}
RT3_REPLACE(0x43f610, ResetPair5ee4fc);

void __stdcall SetGlobals5ee4fc(int32_t a, int32_t b) {
    hook::As<int32_t>(g_5ee4fc) = a;
    g_5ee500 = b;
}
RT3_REPLACE(0x43f620, SetGlobals5ee4fc);

// 0x43f640. Draw handler of the station window's cargo panel (control 0x7d7).
// For the selected station 0x5ee4fc and cargo 0x5ee500 it shows a table with
// columns name, price, difference and amount. There are three sections: the
// station itself, up to 8 stations that are cheaper (they pay less than this
// one minus 0.5) and up to 8 that are dearer. For a "carried" cargo (cargo
// +0x46) the prices are what a train from here would get there (0x47e690
// both ways). Pass 0 counts the rows, pass 1 measures the text to size the
// backdrop, and pass 2 draws the backdrop, frame and text.
RT3_NOINLINE void __fastcall StationCargo_DrawCompare(void*, int, float, float, float, float, int32_t) {
    if (!g_world || !hook::As<int32_t>(g_gameWindow)) return;
    Collection* stations = g_stations;
    if (!stations || !g_cargoTypes) return;
    if (!Collection_IsValid(stations, 0, g_5ee4fc)) return;
    if (hook::As<uint32_t>(g_5ee500) && !Collection_IsValid(g_cargoTypes, 0, hook::As<uint32_t>(g_5ee500))) return;
    if (Global6d07b4Set2()) return;
    if (g_newGame[0x82]) return;
    if (F<int32_t>(g_world, 0x4aef)) return;
    if (!hook::As<uint32_t>(g_5ee500)) return;
    Fade_Save();
    uint8_t* ref = (uint8_t*)Collection_Get(g_stations, 0, g_5ee4fc);
    float refStock, refPrice;
    Obj_CargoStockAndQuota(ref, 0, g_5ee500, &refStock, &refPrice);
    refPrice = (float)(refPrice / D(0x5c87f8));
    uint8_t* cargo = nullptr;
    int32_t carried = 0;
    if (hook::As<uint32_t>(g_5ee500)) {
        cargo = (uint8_t*)Collection_Get(g_cargoTypes, 0, hook::As<uint32_t>(g_5ee500));
        if (cargo) {
            carried = cargo[0x46];
            if (carried) refPrice = 0.0f;
        }
    }
    float widths[4] = { 440.0f, 70.0f, 80.0f, 75.0f };
    float colX[5];
    colX[4] = 1580.0f;
    for (int k = 3; k >= 0; --k) colX[k] = colX[k + 1] - widths[k];
    int32_t align[4] = { 3, 3, 3, 3 };
    static uint8_t skip[500], skipSave[500];
    static uint8_t* list[500];
    static float price[500], amount[500], back[500], backAmount[500];
    memset(skip, 0, sizeof skip);
    int32_t count = Collection_Count(g_stations) > 500 ? 500 : Collection_Count(g_stations);
    for (int32_t i = 0; i < count; ++i) {
        uint8_t* st = (uint8_t*)Collection_RecordAt(g_stations, 0, i);
        list[i] = st;
        if (!Obj_BuildingStateActive(st))
            skip[i] = 1;
        else if (carried && st != ref) {
            Train_GatherLoads(ref, 0, 1, nullptr, st, g_5ee500, BitsF(0x4479c000u), &amount[i], &price[i], nullptr);
            if (!(amount[i] == Fl_205(0x5c8598))) price[i] = price[i] / amount[i];
            price[i] = (float)(((double)price[i] + Fl_205(0x5ca06c)) / D(0x5c87f8));
            Train_GatherLoads(st, 0, 1, nullptr, ref, g_5ee500, BitsF(0x4479c000u), &backAmount[i], &back[i], nullptr);
            if (!(backAmount[i] == Fl_205(0x5c8598))) back[i] = back[i] / backAmount[i];
            back[i] = (float)(((double)back[i] + Fl_205(0x5ca06c)) / D(0x5c87f8));
        } else {
            Obj_CargoStockAndQuota(st, 0, g_5ee500, &amount[i], &price[i]);
            price[i] = (float)(price[i] / D(0x5c87f8));
        }
    }
    float sectionY[4] = { 18.0f, 108.0f, 465.0f, 75.0f };
    float top = 1.0f, left = 1.0f, bottom = 0.0f, right = 0.0f;
    int32_t cheaperRows = 0, dearerRows = 0;
    memcpy(skipSave, skip, sizeof skip);
    char text[0x1f8], num[0x68];
    for (int32_t pass = 0; pass < 3; ++pass) {
        if (pass == 2) {
            if (F<int32_t>(g_windowManager, 0xc64) <= 0) Fade_Set(0x3a378034);
            memcpy(skip, skipSave, sizeof skip);
            if (right > left && bottom > top) {
                bottom = BitsF(0x3f2bbbbcu);
                double l = left < D(0x5ca308) ? (double)left : BitsD(0x3fe7000000000000ull);
                left = (float)l;
                float x = (float)(left - D(0x5ca300));
                float y = (float)(top - D(0x5ca2f8));
                float w = (float)(((double)right - left) + D(0x5ca2f0));
                float h = (float)(((double)Fl_205(0x5ca2e8) - top) + D(0x5ca2e0));
                float sx = (float)(w / D(0x5c9bc0));
                float sy = (float)(h / D(0x5ca2d8));
                Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(g_world, 0x46a8c), 0, g_6d3e34), 0, x, y,
                            0x3f800000, sx, sy, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
                Ui_FrameRect(0xff000000, 0, x, y, w, h, 2.5f, 3);
            }
        }
        for (int32_t sec = 0; sec < 3; ++sec) {
            if (sec == 2 && carried) memcpy(skip, skipSave, sizeof skip);
            float y = (float)(sectionY[sec] / D(0x5ca2d0));
            int32_t rowEnd = (sec != 0) * 8 + 1;
            int32_t diff = 0;
            for (int32_t row = -2; row < rowEnd; ++row) {
                int32_t idx = -1;
                if (row >= 0) {
                    if (sec == 0) {
                        idx = (int32_t)Collection_PositionOf(g_stations, 0, g_5ee4fc, 0);
                        if (idx >= 500) idx = -1;
                    } else {
                        float best = 0.0f;
                        for (int32_t i = 0; i < count; ++i) {
                            if (skip[i] || list[i] == ref) continue;
                            if (sec == 1) {
                                if (carried) {
                                    if (!(backAmount[i] > D(0x5c8628))) continue;
                                    if (idx != -1 && !(back[i] > best)) continue;
                                    best = back[i];
                                } else {
                                    if (!(amount[i] >= D(0x5c8628))) continue;
                                    if (!(refPrice - D(0x5c8578) > price[i])) continue;
                                    if (idx != -1 && !(price[i] < best)) continue;
                                    best = price[i];
                                }
                            } else if (sec == 2) {
                                if (carried && !(amount[i] > D(0x5c8628))) continue;
                                if (!(refPrice + D(0x5c8578) < price[i])) continue;
                                if (idx != -1 && !(price[i] > best)) continue;
                                best = price[i];
                            } else
                                continue;
                            idx = i;
                        }
                    }
                    if (idx == -1) break;
                }
                if (pass == 0) {
                    if (row >= 0) {
                        if (sec == 1) ++cheaperRows;
                        else if (sec == 2) ++dearerRows;
                    }
                    continue;
                }
                bool nextRow = false;
                for (int32_t col = 0; col < 4 && !nextRow; ++col) {
                    float x = (float)(colX[col] / D(0x5ca2c8));
                    uint8_t* font = F<uint8_t*>(g_world, 0x46a94);
                    int32_t al = align[col];
                    uint32_t color = 0xffffffff;
                    float w = (float)((((double)colX[col + 1] - colX[col]) - Fl_205(0x5c8554)) / D(0x5ca2c8));
                    if (row < 0) {
                        if (col > 0) continue;
                        x = BitsF(0x3f480000u);
                        al = 0;
                        w = (float)(((double)colX[3] + Fl_205(0x5ca01c)) / D(0x5ca2c8) - D(0x5ca2c0));
                        if (sec == 0) {
                            if (row != -2) {
                                nextRow = true;
                                break;
                            }
                            if (cargo) ExeSprintf()(text, LocalizeName((const char*)cargo + 4));
                            else ExeSprintf()(text, (const char*)0x5ca2bc);
                        } else if (row == -2) {
                            const char* a1;
                            uint32_t fmt;
                            if (carried) {
                                a1 = (const char*)cargo + 0x46b;
                                fmt = sec == 1 ? 0xf22 : 0xf23;
                            } else {
                                a1 = Num_FormatSimple(num, 9, (double)Ftol(refPrice), 0xb);
                                fmt = sec == 1 ? 0xf24 : 0xf25;
                            }
                            FormatText(text, GetString(fmt), a1, 0, 0, 0, 0, 0, 0, 0);
                            font = F<uint8_t*>(g_world, 0x46a98);
                        } else if (sec == 1) {
                            if (cheaperRows) {
                                nextRow = true;
                                break;
                            }
                            FormatText(text, GetString(0xf26), 0, 0, 0, 0, 0, 0, 0, 0);
                        } else {
                            if (sec != 2 || dearerRows) {
                                nextRow = true;
                                break;
                            }
                            FormatText(text, GetString(0xf26), 0, 0, 0, 0, 0, 0, 0, 0);
                        }
                    } else {
                        uint8_t* st = list[idx];
                        int32_t value = Obj_BuildingValue(st);
                        bool differs = value != Obj_Get21(g_world);
                        switch (col) {
                        case 0:
                            FormatText(text, (const char*)st + 0x46b, 0, 0, 0, 0, 0, 0, 0, 0);
                            break;
                        case 1:
                            if (carried) continue;
                            Num_FormatSimple(text, 9, (double)price[idx], 0xb);
                            break;
                        case 2: {
                            if (carried) {
                                if (sec == 0) continue;
                                float v = sec == 1 ? -back[idx] : price[idx];
                                diff = Ftol(v);
                            } else {
                                int32_t a = Ftol(price[idx]);
                                diff = a - Ftol(refPrice);
                            }
                            const char* sign = diff < 0 ? (const char*)0x5ca2b4 : (const char*)0x5ca2b8;
                            int32_t mag = diff < 0 ? -diff : diff;
                            FormatText(text, (const char*)0x5c8730, sign, Num_FormatSimple(num, 9, (double)mag, 0xb), 0, 0, 0, 0, 0, 0);
                            break;
                        }
                        case 3: {
                            float v = carried && sec == 1 ? backAmount[idx] : amount[idx];
                            ExeSprintf()(text, (const char*)0x5ca2ac, (double)v);
                            break;
                        }
                        }
                        if (sec == 0)
                            color = 0xffffff00;
                        else if (col == 2) {
                            uint32_t base = diff == 0 ? 0xdfdfdf : diff > 0 ? 0xbfffbf : 0xffbfbf;
                            color = ((differs ? 0xffcfcfd0u : 0u) + base) | 0xff000000;
                        } else if (differs)
                            color = 0xffbfbfbf;
                    }
                    if (pass == 1) {
                        float textW;
                        Font_TextHeight(font, 0, text, w, &textW);
                        double t = (double)y + D(0x5c9bc0);
                        top = (float)(top < t ? (double)top : t);
                        float yb = (float)(y + Fl_205(0x5ca2a8));
                        if (!(bottom > yb)) bottom = yb;
                        if (al == 3) {
                            x = (float)(w + x);
                            t = ((double)x - textW) - D(0x5ca2a0);
                            left = (float)(left < t ? (double)left : t);
                        } else if (al == 0) {
                            t = ((double)w - textW) * D(0x5c8628) + x;
                            left = (float)(left < t ? (double)left : t);
                            x = (float)(w + x);
                        } else {
                            if (!(left < x)) left = x;
                            x = (float)(w + x);
                        }
                        right = bottom > x ? bottom : x;
                    } else {
                        float pad = 0.0f;
                        if (row == -2 && !IsEnglish()) pad = BitsF(0x3ccccccdu);
                        Font_DrawTextBox(font, 0, text, x - pad, y, pad + w, BitsF(0x3ce81b4fu), color, al, 0, 0, 1.0f);
                    }
                }
                if (nextRow) continue;
                y = (float)(row == -2 && sec == 0 ? y + D(0x5ca298) : y + Fl_205(0x5ca2a8));
                if (idx >= 0 && idx < 500) skip[idx] = 1;
            }
        }
    }
    Fade_Restore();
}
RT3_REPLACE(0x43f640, StationCargo_DrawCompare);

}  // namespace rt3
