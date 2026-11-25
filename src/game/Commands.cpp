// Commands.cpp - RT3.exe 0x440000-0x444000.
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
#include "core/stream.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t __fastcall StreamTell(Stream s);
int32_t ClockMs();
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                         int32_t f, int32_t g, int32_t h, int32_t i);

int32_t __fastcall Obj_Get21(void* o);
void* SelectedTrain();
void __fastcall Texture_Destroy(uint8_t* t);
void* __fastcall Obj_City41(void* o);
void* __fastcall Obj_Ref388(void* o);
void __fastcall Str_Replace(char* str, int32_t size, const char* find, const char* repl, int32_t all);
int32_t __fastcall Game_EffectiveVersion();
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void __fastcall Territories_Recount(Collection* c);
void __fastcall City_LayStreets(uint8_t* c);
int32_t __fastcall Cities_AssignBlocks(Collection* cities);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
int32_t __fastcall Collection_Count(Collection* c);
void __fastcall Terrain_RefreshPointGrids(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
const char* __fastcall Options_SaveFolder(void* opt);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Obj_DecA0(void* o);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall Camera_BeginA0(uint8_t* o);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall TrimSpaces(char* s, int32_t alsoNewlines);
int32_t Global6d07b4Set2();
int32_t IsMultiplayer();
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall StreamClose(int32_t s);
void __fastcall Obj_Incc60(uint8_t* o);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
int32_t CountHumanPlayers();
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
void __fastcall Obj_Decc60(uint8_t* o);
void __fastcall World_UpdateSpeedScale(uint8_t* o);
void __fastcall Table_RefreshNamedFlags(Collection* t);
int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute);
void __fastcall Engine_ClearRenderLists(uint8_t* e);
void AppendOptionHistory();
void __fastcall InfoWindow_ClearHighlights(uint8_t* w);
int32_t __fastcall StreamSkip4(int32_t s, int tag, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, int32_t s, int32_t recordSize);
int32_t OptionTable10c();
void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
void* __fastcall Obj_Company21(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

void __fastcall Weather_ApplyIntensity(uint8_t* w);
void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled);
void __stdcall Options_SetWeatherOverride(int32_t on, int32_t level);
uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
void MarkKind2Buildings();
void __fastcall Obj_SumBuildingWeights(void* o);
void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
void __fastcall Train_Crash(uint8_t* t, int, int32_t forced);
void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7);
void __fastcall App_SetViewMode(uint8_t* app, int, int32_t mode);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall NewGameConfig_Reset(uint8_t* c);
void __fastcall Cursor_PopBusy(uint8_t* c);
int32_t Global6cfeb4Positive();
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
void __fastcall Industries_BuildCargoLists(Collection* c);
void World_UpdateEraFactor();
void __fastcall CargoTypes_Recount(Collection* t);
void __fastcall Lots_RebuildDemand(Collection* c);
int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
void __fastcall World_SetupPlayers(uint8_t* w);
void PopOptionHistory();
int32_t __fastcall BuildingDialog_Retrack(uint8_t* dlg);
void Game_PauseTools();
void __fastcall Cursor_PushBusy(uint8_t* c);
void __fastcall App_Redraw(uint8_t* a);
void __fastcall Map_WritePreview(uint8_t* m, int, uint8_t* dst);
void __fastcall StreamClose(Stream s);
void __fastcall TrackTool_Suspend(uint8_t* w);
uint8_t* __fastcall Net_CountMessagePart(uint8_t* s, int, int32_t count);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
RT3_NOINLINE int32_t Global6d07b4Set2();
uint32_t __fastcall Vehicles_MaybeSpawnFlight(Collection* t, int, float chance, int32_t force);
RT3_NOINLINE void __fastcall World_SetTime(uint8_t* w, int, int32_t minutes);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE uint64_t __fastcall MakeDate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
RT3_NOINLINE int32_t __fastcall DateToMinutes(const GameDate* d);
RT3_NOINLINE int32_t __fastcall Company_TrainCount(void* c);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void __fastcall CargoTypes_MarkAvailable(Collection* c);
void __fastcall World_Reset(uint8_t* w, int, int32_t setup);
RT3_NOINLINE void __fastcall Cursor_PopBusy(uint8_t* c);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE int32_t Global6cfeb4Positive();
void* __fastcall Table_Oldest14(Collection* t, int, uint8_t all);
void __fastcall Newspaper_Show(uint8_t* rec);
int32_t __fastcall World_TogglePause(uint8_t* w, int, int32_t announce);
void __fastcall Ledger_Open(int32_t company);
void __fastcall Obj_Set78(uint8_t* o, int, int32_t v);
void __fastcall World_Teardown(uint8_t* w);
int32_t CountUsedSlots6ce2d8();
void __fastcall Save_UnpackToTemp(const char* path);
char* __fastcall CopyBaseName(const char* src, char* dst);
void Block_ExtractImage();
RT3_NOINLINE int32_t __fastcall Collection_Load(Collection* c, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Templates_Normalize(uint8_t* self);
RT3_NOINLINE void __fastcall Industries_BuildCargoLists(Collection* c);
RT3_NOINLINE void __fastcall World_UpdateSpeedScale(uint8_t* o);
RT3_NOINLINE void __fastcall Economy_UpdateEraFactors(uint8_t* o);
RT3_NOINLINE void __fastcall Weather_ScheduleNext(uint8_t* w, int, int32_t scaled);
RT3_NOINLINE void __fastcall CargoTypes_Recount(Collection* t);
RT3_NOINLINE void __fastcall NewGameConfig_Reset(uint8_t* c);
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t);
uint8_t* __fastcall Map_ConstructLoad(uint8_t* m, int, int32_t s, const char* tex);
int32_t __fastcall Collection_LoadTagged6979(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall CargoTypes_Reload(Collection* c);
int32_t __stdcall Sites_LoadTable(Stream s);
void __fastcall BtyTable_Reload(Collection* t);
int32_t __fastcall Buildings_Load(Collection* t, int, Stream s, int32_t recordSize);
int32_t __fastcall Collection_LoadWithArrays(Collection* c, int, Stream s, int32_t);
int32_t __fastcall PartModels41aae0_Load(Collection* t, int, Stream s, int32_t recordSize);
int32_t __fastcall Trains_Load(Collection* c, int, int32_t st, int32_t recordSize);
int32_t __fastcall Companies_Load(Collection* t, int, Stream s, int32_t recordSize);
int32_t __fastcall TrackNet_Load(Collection* net, int, int32_t s, int32_t recordSize);
int32_t __fastcall Routes_Load(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Buildings_RestoreQueued(Collection* t);
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Tracks_RebuildGeometry(Collection* c);
int32_t __fastcall Players_Load(Collection* t, int, Stream s, int32_t recordSize);
int32_t __fastcall NamedObjs4611e0_Load(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Names_ApplyRegionVariants(Collection* c, int, char* mapName);
int32_t __fastcall NamedObjs474110_Load(Collection* t, int, Stream s, int32_t recordSize);
int32_t __fastcall NamedObjs41f590_Load(Collection* t, int, Stream s, int32_t recordSize);
int32_t __fastcall Stations_Load(Collection* t, int, Stream s, int32_t recordSize);
int32_t __fastcall NamedObjs456900_Load(Collection* t, int, Stream s, int32_t recordSize);
int32_t __fastcall RuleSets_Load(Collection* c, int, Stream st, int32_t recordSize);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
void __fastcall Territories_BuildBorders(Collection* c);
RT3_NOINLINE void MarkKind2Buildings();
void __fastcall Cities_LayStreets(Collection* cities);
void __fastcall Map_MarkUntypedRegions(uint8_t* m);
void __fastcall Map_UpdateCargoFlags(uint8_t* m);
RT3_NOINLINE void __fastcall Lots_RebuildDemand(Collection* c);
void __fastcall Scenario_ApplyFixes(uint8_t* scn, int, const char* name);
RT3_NOINLINE void __fastcall World_SetupPlayers(uint8_t* w);
RT3_NOINLINE void __fastcall Obj_SumBuildingWeights(void* o);
int32_t __fastcall Camera_RecallBookmark(uint8_t* c, int, int32_t slot);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void FreeBuffer62b2f8();
void __fastcall Map_FeatureBounds(uint8_t* m);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall Companies_BuildStationLinks(Collection* cos);
void Tutorial_Start();
RT3_NOINLINE void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);

 // namespace

void __fastcall App_SetScreen(uint8_t* a, int, int32_t screen, int32_t closeWorld);

RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
void __fastcall Options_SetDisplayMode(uint8_t* o, int, int32_t m);
int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
void __fastcall Obj_FillEntry10e(uint8_t* o, int, int32_t i);
RT3_NOINLINE int32_t __fastcall Models_DisplayName(const char* name, char* out);

// 0x440b00..0x440b90 / 0x440ba0..0x440c30 (hotkey callbacks). Save / load
// camera bookmark 1..9, then 0; the callback's argument goes along unused.
#define RT3_BOOKMARK(addr, verb, slot) \
    int32_t __fastcall Camera_##verb##Bookmark##slot(void* p) { return Camera_##verb##BookmarkCmd(slot, p); } \
    RT3_REPLACE(addr, Camera_##verb##Bookmark##slot);

void __fastcall World_CheatPrompt(uint8_t* w);
uint32_t __fastcall World_ReopenLastEvent(uint8_t* w);
void* FocusedOwnTrain();
void* __fastcall Table_Oldest(Collection* t, int, uint8_t all);

// Dev commands opening a ledger page (with a world). Return 0.
#define RT3_LEDGER(addr, page) \
    int32_t Cmd_Ledger##page() { \
        if (g_world) Ledger_Open(page); \
        return 0; \
    } \
    RT3_REPLACE(addr, Cmd_Ledger##page);

uint32_t __stdcall Scenario_CheckEditable();
uint32_t __fastcall Player_CheckHasCompany(uint8_t* p);
void __stdcall World_SetEditorButtons(uint32_t modeArg, uint32_t disabledArg);
int32_t __fastcall World_StepSpeed(uint8_t* w, int, int32_t delta);
uint32_t __stdcall StockMarket_IsAllowed();
uint32_t __fastcall Player_CheckStockAllowed(uint8_t* p);
int32_t __fastcall Obj_Byte21c5(void* o);
void __fastcall Overview_ShowPlainOnce(uint8_t* t);
void __fastcall Overview_EndPlain(uint8_t* t);
void __fastcall Trains_AddAndShow(const char* name, int32_t follow);
int32_t __fastcall Camera_FlagA0(uint8_t* o, int, int32_t);

#define RT3_TOOL9(addr, name, sub) \
    int32_t name() { \
        if (uint8_t* v = g_gameWindow) { \
            if (F<int32_t>(v, 0x8c) == 9 && F<int32_t>(v, 0x90) == sub) View_SetToolMode(v, 0, 0, -1); \
            else View_SetToolMode(v, 0, 9, sub); \
        } \
        return 0; \
    } \
    RT3_REPLACE(addr, name);

uint32_t __fastcall Player_CheckCanRunTrains(uint8_t* p);
int32_t RendererMode15Zero();
void __fastcall Obj_Set65(void* o);
void* __fastcall Table_Newest14(Collection* t);
void* __fastcall Table_FindField14Prev(Collection* t, int, int32_t v);
int32_t Option1df();
void __fastcall Companies_BuildStationLinks(Collection* cos);
void __fastcall Obj_Reset455310(void* o);
void* __fastcall Table_Newest(Collection* t);
void* __fastcall Table_FindField4Prev(Collection* t, int, int32_t v);
void* __fastcall Table_FindField4Next(Collection* t, int, int32_t v);
void __fastcall Train_BreakDown(uint8_t* t, int, int32_t forced);
int32_t __fastcall Company_TrainCount(void* c);
uint32_t __fastcall Company_NthTrain(int32_t* o, int, int32_t n);
uint32_t __fastcall Company_NthSite(int32_t* o, int, int32_t n, int32_t activeOnly);
void __fastcall Placement_RotateSteps(void*, int, int32_t steps);
int32_t __fastcall Models_DisplayName(const char* name, char* out);

// 0x440c80 / 0x440d40 (hotkeys). Rotate the building being placed (0x6d1720)
// by -1 / +1 step, or log the key's display name (0x176 / 0x177, 0x178 /
// 0x179) when nothing is placed.
#define RT3_ROTATE_HOTKEY(addr, name, steps, keyName, msg) \
    int32_t name() { \
        if (!g_world) return 0; \
        if (g_buildingDialog) { \
            Placement_RotateSteps(hook::As<void*>(g_buildingDialog), 0, steps); \
            return 0; \
        } \
        char key[0x190], shown[0x190], text[0x190]; \
        FormatText(key, GetString(keyName), 0, 0, 0, 0, 0, 0, 0, 0); \
        if (Models_DisplayName(key, shown)) { \
            FormatText(text, GetString(msg), shown, 0, 0, 0, 0, 0, 0, 0); \
            Log_Add(g_messageLog, 0, text, 4, 0, 0); \
        } \
        return 0; \
    } \
    RT3_REPLACE(addr, name);

int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
void Buildings_SelectNextFwd();
void Buildings_SelectNextBack();
void Trains_SelectNextFwd();
void Trains_SelectNextBack();
void __stdcall CompanyView_Next();
void __stdcall CompanyView_Prev();
void Vehicles_SelectNextFwd();
void Vehicles_SelectNextBack();

// 0x440e00 / 0x440f20 (hotkeys). Select the next / previous building, train,
// company (unless 0x6cfe6c or 0x6cfeb4) or vehicle of the open view, or
// log the key's name (0x17a / 0x17b, 0x17c / 0x17d).
#define RT3_SELECT_HOTKEY(addr, name, buildings, trains, company, vehicles, key, msg) \
    int32_t name() { \
        if (hook::As<int32_t>(g_stationWindow)) return buildings(), 0; \
        if (hook::As<int32_t>(g_industryWindow)) return trains(), 0; \
        if (hook::As<int32_t>(g_6cfe4c)) { \
            if (!hook::As<int32_t>(g_6cfe6c) && !Global6cfeb4Positive()) company(); \
            return 0; \
        } \
        if (hook::As<int32_t>(g_trainWindow)) return vehicles(), 0; \
        if (g_world) Log_KeyName(key, msg); \
        return 0; \
    } \
    RT3_REPLACE(addr, name);

int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
int32_t __fastcall DateToMinutes(const GameDate* d);
void __fastcall Obj_ClearClock346(void* o);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall CargoTypes_MarkAvailable(Collection* c);
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
void __fastcall StreamWriteU32(int32_t s, uint32_t value);
void __fastcall NameTable_Rescan(uint8_t* t);
typedef void*(__cdecl* B49Fopen)(const char*, const char*);
typedef size_t(__cdecl* B49Fio)(void*, size_t, size_t, void*);
int32_t __fastcall Options_Check1bf(void* self);
uint8_t* __fastcall Jpeg_Load(const char* path, int32_t* width, int32_t* height, int32_t offset);
void __fastcall Input_RemoveHandler(uint8_t* in, int, uint32_t h);
void __fastcall StripExtension(char* s);
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize);
void __fastcall Nop552ff0();
typedef int(__cdecl* B60Fseek)(void*, long, int);
typedef long(__cdecl* B60Ftell)(void*);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Types_RefreshAvailable(Collection* c);
RT3_NOINLINE int32_t __fastcall Buildings_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Buildings_RestoreQueued(Collection* t);
RT3_NOINLINE void __fastcall BtyTable_Reload(Collection* t);
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
RT3_NOINLINE uint32_t __stdcall Scenario_CheckEditable();
RT3_NOINLINE uint32_t __fastcall Player_CheckHasCompany(uint8_t* p);
RT3_NOINLINE uint32_t __fastcall Player_CheckCanRunTrains(uint8_t* p);
RT3_NOINLINE void __fastcall World_Teardown(uint8_t* w);
RT3_NOINLINE uint32_t __stdcall StockMarket_IsAllowed();
RT3_NOINLINE uint32_t __fastcall Player_CheckStockAllowed(uint8_t* p);
RT3_NOINLINE void __fastcall World_SetupPlayers(uint8_t* w);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall World_CheatPrompt(uint8_t* w);
RT3_NOINLINE void __fastcall World_PrepareNewGame(uint8_t* w);
RT3_NOINLINE uint32_t __fastcall World_ReopenLastEvent(uint8_t* w);
RT3_NOINLINE void __fastcall World_Startup(uint8_t* w, int, int32_t setup, int32_t withMap);
RT3_NOINLINE int32_t __fastcall Camera_RecallBookmark(uint8_t* c, int, int32_t slot);
int32_t Cmd_ToolModeStock();
int32_t Cmd_ToolMode1();
int32_t Cmd_ToolMode4();
int32_t Cmd_ToolModeEditor();
int32_t Cmd_ToolModeCompanyEditor();
int32_t Cmd_ToolModeCompany();
int32_t Cmd_ToolModeStockPlayer();
int32_t Cmd_ConsistEditor();
int32_t Cmd_TogglePause();
int32_t Cmd_SaveWorld();
int32_t Cmd_SaveWorld1();
RT3_NOINLINE int32_t __fastcall Camera_SaveBookmarkCmd(int32_t slot, void*);
RT3_NOINLINE int32_t __fastcall Camera_LoadBookmarkCmd(int32_t slot, void*);
int32_t Cmd_DisplayModeByOption();
int32_t Hotkey_ShowTrain();
int32_t Hotkey_ShowStation();
int32_t Cmd_TogglePlainOverview();
int32_t Cmd_ToggleTrackA0();
int32_t Cmd_ToggleEngine114276();
int32_t Cmd_Release9858();
int32_t Cmd_Toggle62be84();
int32_t Cmd_EditorButtons();
int32_t Cmd_EditorButtonsDisabled();
int32_t Cmd_NextWeather();
int32_t Cmd_CheatPrompt();
int32_t Cmd_SpawnFlight();
int32_t Cmd_AddTrain();
int32_t Cmd_ToggleEngine1146be();
int32_t Cmd_ResetLogEntries();
int32_t Cmd_MarkOldestLog();
int32_t Cmd_ShowPreviousNewspaper();
int32_t Cmd_ReopenLastEvent();
int32_t Cities_FindByPrompt();
int32_t Hotkey_Chat();
RT3_NOINLINE void* FocusedOwnTrain();
int32_t Cmd_CrashTrain();
int32_t Cmd_FlagTrainCity();
int32_t Cmd_BreakDownTrain();
int32_t Hotkey_SpeedUp();
int32_t Hotkey_SpeedDown();
int32_t Hotkey_QuickLoad();
int32_t Hotkey_QuickLoadAlt();
int32_t Cmd_ToggleMap168d();
int32_t Editor_ToggleCb();
int32_t Cmd_TimeOfDayUp();
int32_t Cmd_TimeOfDayDown();
RT3_NOINLINE const char* __stdcall Save_ProgressText(int32_t total, float done);
void Block3c2_Save();
void Block_AppendImage();
void Block_ExtractImage();
RT3_NOINLINE void AppendOptionHistory();
RT3_NOINLINE void PopOptionHistory();
RT3_NOINLINE void Game_PauseTools();
RT3_NOINLINE void Options_Restore();
RT3_NOINLINE void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial);
RT3_NOINLINE int32_t __fastcall Save_LoadOptions(const char* name);
RT3_NOINLINE void __fastcall Save_LogProgress(Stream s, const char* label);
RT3_NOINLINE void __stdcall Preview_SaveJpeg(const char* path, uint8_t* o);
RT3_NOINLINE void __fastcall Save_UnpackToTemp(const char* path);
void Save_Pack();
RT3_NOINLINE int32_t __cdecl AnyHasFixedName();
RT3_NOINLINE void __fastcall Map_WritePreview(uint8_t* m, int, uint8_t* dst);
RT3_NOINLINE void __fastcall Scenario_ApplyFixes(uint8_t* scn, int, const char* name);
RT3_NOINLINE void __fastcall World_LoadGame(uint8_t* w, int, const char* name, int32_t show);
RT3_NOINLINE int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE uint8_t* __fastcall Map_ConstructLoad(uint8_t* m, int, int32_t s, const char* tex);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
RT3_NOINLINE void __fastcall Overview_ShowPlainOnce(uint8_t* t);
RT3_NOINLINE void __fastcall Overview_EndPlain(uint8_t* t);
float __fastcall Model_FrameTime(uint8_t* o);
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t);
RT3_NOINLINE void __fastcall Trains_AddAndShow(const char* name, int32_t follow);
RT3_NOINLINE void __fastcall Minimap_Render(uint32_t* dst, int32_t w, int32_t h, uint32_t flags, int32_t player, int32_t haveTex, uint32_t* tex, int32_t srcW, int32_t srcH, float* u0, float* u1, float* v0, float* v1);
RT3_NOINLINE void __fastcall App_Redraw(uint8_t* a);
RT3_NOINLINE void __fastcall App_SetScreen(uint8_t* a, int, int32_t screen, int32_t closeWorld);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE int32_t __fastcall TrackNet_Load(Collection* net, int, int32_t s, int32_t recordSize);
RT3_NOINLINE void Trains_SelectNextFwd();
RT3_NOINLINE void Trains_SelectNextBack();
RT3_NOINLINE void __stdcall CompanyView_Next();
RT3_NOINLINE void __stdcall CompanyView_Prev();
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall Ledger_Open(int32_t company);
RT3_NOINLINE void __fastcall Newspaper_Show(uint8_t* rec);
RT3_NOINLINE void Buildings_SelectNextFwd();
RT3_NOINLINE void Buildings_SelectNextBack();
RT3_NOINLINE void __fastcall InfoWindow_ClearHighlights(uint8_t* w);
RT3_NOINLINE int32_t __fastcall BuildingDialog_Retrack(uint8_t* dlg);
RT3_NOINLINE void __fastcall TrackTool_Suspend(uint8_t* w);
RT3_NOINLINE void __fastcall TrackTool_Resume(uint8_t* t);
RT3_NOINLINE void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7);
RT3_NOINLINE void Vehicles_SelectNextFwd();
RT3_NOINLINE void Vehicles_SelectNextBack();
RT3_NOINLINE void Tutorial_Start();
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE int32_t __fastcall Texture_AppendJpeg(uint8_t* t, int, const char* path, uint32_t withAlpha, int32_t quality);
RT3_NOINLINE uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag);


static inline bool OwnTrain(uint8_t* t) { return F<int32_t>(t, 0x51) == Obj_Get21(hook::As<void*>(g_world)); }

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

static inline float BitsF(uint32_t b) { return *(float*)&b; }

namespace {
// 0x4420f0 takes the path in ECX, the block in EAX and the output on the stack.
void LoadExtractImage(const char* path, uint8_t* block, char* out) {
    __asm {
        push out
        mov eax, block
        mov ecx, path
        mov edx, offset Block_ExtractImage
        call edx
    }
}
}

namespace {
uint32_t Message_Dialog(const char* text, uint32_t px) {
    return Dialog_RunMessage(text, 1, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

namespace {
uint32_t Message_Dialog444(const char* text) {
    return Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// The current frame time of model o (Model_FrameTime), 0 without a frame.
uint32_t Model_FrameTimeBits(uint8_t* o, int* has) {
    *has = 0;
    uint8_t* list = F<uint8_t*>(o, 0x14);
    if (!list) return 0;
    uint8_t* rec = F<uint8_t*>(list + 24 * F<int32_t>(o, 0x3a), 0xc) + F<int32_t>(o, 0x21) * 0x45;
    if (!rec) return 0;
    *has = 1;
    return F<uint32_t>(rec, 0x18);
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

namespace {
uint32_t Message449(const char* text, int32_t type) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

// 0x441ec0 body. Saves the 0x3c2-byte block data as: version (0x620e94), marker
// 0x2ee0, the block, marker 0x2ee1 - to file f from its start, or to stream s
// when f is null.
static void __cdecl Block3c2_SaveImpl(void* f, const uint8_t* data, int32_t s) {
    uint32_t v = g_620e94;
    if (f) {
        typedef size_t(__cdecl * FwriteFn)(const void*, size_t, size_t, void*);
        crt::fseek(f, 0, 0);
        crt::fwrite(&v, 4, 1, f);
        v = 0x2ee0;
        crt::fwrite(&v, 4, 1, f);
        crt::fwrite(data, 0x3c2, 1, f);
        v = 0x2ee1;
        crt::fwrite(&v, 4, 1, f);
        return;
    }
    StreamWrite(s, &v, 4, 0);
    StreamWriteU32(s, 0x2ee0);
    StreamWrite(s, data, 0x3c2, 0);
    StreamWriteU32(s, 0x2ee1);
}

// The image file next to path: its base name + "_A.jpg" (jpg) or "_A.tga".
static void B49ImageName(const char* path, char* out, bool jpg) {
    CopyBaseName(path, out);
    memcpy(out + strlen(out), (const void*)(jpg ? 0x5ca328 : 0x5ca320), 7);
}

// 0x441f70 body. Appends the scenario's image (base name of path + "_A.jpg" when
// the block's +9 says jpg, else "_A.tga") to the file path, records where it
// starts (+0x1c), deletes both image files and rewrites the block at the start.
static void __cdecl Block_AppendImageImpl(uint8_t* block, const char* path) {
    void* f = crt::fopen(path, (const char*)0x5ca330);
    crt::fseek(f, 0, 2);
    F<int32_t>(block, 0x1c) = ((int32_t)crt::ftell(f));
    char name[0x40];
    B49ImageName(path, name, block[9] != 0);
    void* in = crt::fopen(name, (const char*)0x5c9678);
    uint8_t buf[0x200];
    for (size_t n; (int32_t)(n = crt::fread(buf, 1, 0x200, in)) > 0;) crt::fwrite(buf, 1, n, f);
    crt::fclose(in);
    crt::remove(name);
    B49ImageName(path, name, block[9] == 0);
    crt::remove(name);
    Block3c2_SaveImpl(f, block, 0);
    crt::fclose(f);
}

// 0x4420f0 body. Extracts the image appended to the file path (from the block's
// +0x1c) into its "_A.jpg" / "_A.tga" file, rescans the resources and copies the
// file name to out. When that file can't be created the image goes into the
// buffer 0x62bed4 (0x402ee0 bytes; its length in 0x62bed8) instead.
static void __cdecl Block_ExtractImageImpl(const char* path, uint8_t* block, char* out) {
    void* f = crt::fopen(path, (const char*)0x5c9678);
    crt::fseek(f, F<int32_t>(block, 0x1c), 0);
    char name[0x44];
    B49ImageName(path, name, block[9] != 0);
    void* o = crt::fopen(name, (const char*)0x5ca334);
    uint8_t buf[0x200];
    if (!o) {
        if (void* old = hook::As<void*>(g_62bed4)) crt::free(old);
        g_62bed8 = 0x402ee0;
        g_62bed4 = ((uint8_t*)crt::malloc(0x402ee0));
        int32_t used = 0;
        for (int32_t n = (int32_t)crt::fread(buf, 1, 0x200, f); n > 0; n = (int32_t)crt::fread(buf, 1, 0x200, f)) {
            if (used + 0x200 >= g_62bed8) break;
            memcpy(g_62bed4 + used, buf, (size_t)n);
            used += n;
        }
        g_62bed8 = used;
        crt::fclose(f);
        return;
    }
    for (size_t n; (int32_t)(n = crt::fread(buf, 1, 0x200, f)) > 0;) crt::fwrite(buf, 1, n, o);
    crt::fflush(o);
    crt::fclose(o);
    crt::fclose(f);
    NameTable_Rescan(g_pack);
    strcpy(out, name);
}

// The whole of file path in a new heap buffer (*size bytes).
static uint8_t* B60ReadAll(const char* path, int32_t* size) {
    void* f = crt::fopen(path, (const char*)0x5c9678);
    crt::fseek(f, 0, 2);
    *size = crt::ftell(f);
    crt::fseek(f, 0, 0);
    uint8_t* buf = ((uint8_t*)crt::malloc((size_t)*size));
    crt::fread(buf, 1, (size_t)*size, f);
    crt::fclose(f);
    return buf;
}

// 0x442900 body. Packs save file path in place: the payload from +0x3ce to the
// block's data offset (+0x18) is LZSS-packed (progress to 0x441e20), the rest
// is kept, the offset moves to where the data now starts and the block header
// is rewritten at the start.
static void __cdecl Save_PackImpl(uint8_t* block, const char* path) {
    int32_t size;
    uint8_t* buf = B60ReadAll(path, &size);
    uint8_t* out;
    int32_t outSize;
    int32_t packer[3] = { 0x91, 0, 0 };
    g_d93988 = 0x441e20;
    Pack_Compress(packer, 0, buf + 0x3ce, F<int32_t>(block, 0x18) - 0x3ce, &out, &outSize);
    void* f = crt::fopen(path, (const char*)0x5ca334);
    crt::fwrite(buf, 1, 0x3ce, f);
    crt::fwrite(out, 1, (size_t)outSize, f);
    int32_t pos = crt::ftell(f);
    crt::fwrite(buf + F<int32_t>(block, 0x18), 1, (size_t)(F<int32_t>(block, 0x1c) - F<int32_t>(block, 0x18)), f);
    crt::fwrite(buf + F<int32_t>(block, 0x1c), 1, (size_t)(size - F<int32_t>(block, 0x1c)), f);
    F<int32_t>(block, 0x18) = pos;
    Block3c2_SaveImpl(f, block, 0);
    crt::fclose(f);
    crt::free(buf);
    crt::free(out);
    g_d93988 = 0;
    ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
}

// Dev commands switching the view's tool mode when allowed.
int32_t Cmd_ToolModeStock() {
    if (g_gameWindow && g_world && StockMarket_IsAllowed()) View_SetToolMode(g_gameWindow, 0, 0xb, -1);
    return 0;
}
RT3_REPLACE(0x440530, Cmd_ToolModeStock);

// 0x440560 / 0x440580 (dev commands). Tool modes 1 / 4.
int32_t Cmd_ToolMode1() {
    if (uint8_t* v = g_gameWindow) View_SetToolMode(v, 0, 1, -1);
    return 0;
}
RT3_REPLACE(0x440560, Cmd_ToolMode1);

int32_t Cmd_ToolMode4() {
    if (uint8_t* v = g_gameWindow) View_SetToolMode(v, 0, 4, -1);
    return 0;
}
RT3_REPLACE(0x440580, Cmd_ToolMode4);

int32_t Cmd_ToolModeEditor() {
    if (g_gameWindow && g_world && Scenario_CheckEditable()) View_SetToolMode(g_gameWindow, 0, 7, -1);
    return 0;
}
RT3_REPLACE(0x4405a0, Cmd_ToolModeEditor);

// 0x4405d0 (dev command). Editor tool mode 8 for the player's company
// (string 0x170 without one).
int32_t Cmd_ToolModeCompanyEditor() {
    if (!g_gameWindow) return 0;
    if (!Obj_Get21(g_world)) {
        Message_Dialog444(GetString(0x170));
        return 0;
    }
    if (g_gameWindow && g_world && Scenario_CheckEditable())
        View_SetToolMode(g_gameWindow, 0, 8, Obj_Get21(g_world));
    return 0;
}
RT3_REPLACE(0x4405d0, Cmd_ToolModeCompanyEditor);

int32_t Cmd_ToolModeCompany() {
    if (g_gameWindow && g_world && Player_CheckHasCompany(g_world))
        View_SetToolMode(g_gameWindow, 0, 0xc, -1);
    return 0;
}
RT3_REPLACE(0x440660, Cmd_ToolModeCompany);

int32_t Cmd_ToolModeStockPlayer() {
    if (g_gameWindow && g_world && Player_CheckStockAllowed(g_world))
        View_SetToolMode(g_gameWindow, 0, 6, -1);
    return 0;
}
RT3_REPLACE(0x440690, Cmd_ToolModeStockPlayer);

// 0x4406c0 (dev command). Opens the consist editor for a new train when the
// player can run trains.
int32_t Cmd_ConsistEditor() {
    if (g_gameWindow && g_world && Player_CheckCanRunTrains(g_world))
        ConsistEditor_Run(nullptr, nullptr, nullptr, 0xffffffff, 0xffffffff, 0, 0, 0, 0xffffffff);
    return 0;
}
RT3_REPLACE(0x4406c0, Cmd_ConsistEditor);

RT3_LEDGER(0x440700, 1)

RT3_LEDGER(0x440720, 3)

RT3_LEDGER(0x440740, 4)

RT3_LEDGER(0x440760, 5)

RT3_LEDGER(0x440780, 6)

RT3_LEDGER(0x4407a0, 10)

RT3_LEDGER(0x4407c0, 12)

RT3_LEDGER(0x4407e0, 15)

RT3_LEDGER(0x440800, 14)

RT3_LEDGER(0x440820, 2)

RT3_LEDGER(0x440840, 7)

RT3_LEDGER(0x440860, 9)

// 0x440880 (dev command). Toggles the pause (announced) with sound 0x2c.
int32_t Cmd_TogglePause() {
    if (uint8_t* w = g_world) {
        World_TogglePause(w, 0, 1);
        Ui_PlaySound(0x2c, 0, 0x3f000000);
    }
    return 0;
}
RT3_REPLACE(0x440880, Cmd_TogglePause);

// 0x4408b0 / 0x4408d0 (dev commands). Save the world (quick / flag 1).
int32_t Cmd_SaveWorld() {
    if (uint8_t* w = g_world) World_Save(w, 0, nullptr, 0, nullptr);
    return 0;
}
RT3_REPLACE(0x4408b0, Cmd_SaveWorld);

int32_t Cmd_SaveWorld1() {
    if (uint8_t* w = g_world) World_Save(w, 0, nullptr, 1, nullptr);
    return 0;
}
RT3_REPLACE(0x4408d0, Cmd_SaveWorld1);

// 0x4408f0. Dev command: stores the camera (0x62be68) in bookmark `slot` and
// logs it (0x172) under the display name of string 0x171 with the slot
// number. The second register argument is unused. Returns 0.
RT3_NOINLINE int32_t __fastcall Camera_SaveBookmarkCmd(int32_t slot, void*) {
    if (!g_62be68) return 0;
    Obj_FillEntry10e(g_62be68, 0, slot);
    char text[0x190], name[0x190], shown[0x190];
    const char* num = Num_Format(text, 1, (double)slot, 10, 1, 0, 0);
    FormatText(name, GetString(0x171), num, 0, 0, 0, 0, 0, 0, 0);
    if (Models_DisplayName(name, shown)) {
        FormatText(text, GetString(0x172), shown, 0, 0, 0, 0, 0, 0, 0);
        Log_Add(g_messageLog, 0, text, 4, 0, 0);
    }
    return 0;
}
RT3_REPLACE(0x4408f0, Camera_SaveBookmarkCmd);

// 0x4409c0. Dev command: moves the camera to bookmark `slot`; when it is set
// (0x43ca20 returns 0) and both display names (0x173 / 0x174 with the slot
// number) are known, logs 0x175. Returns 0.
// QUIRK: the message gets the second name twice and the first one third.
RT3_NOINLINE int32_t __fastcall Camera_LoadBookmarkCmd(int32_t slot, void*) {
    if (!g_62be68) return 0;
    if (Camera_RecallBookmark(g_62be68, 0, slot)) return 0;
    char text[0x190], first[0x190], second[0x190], shownFirst[0x190], shownSecond[0x190];
    const char* num = Num_Format(text, 1, (double)slot, 10, 1, 0, 0);
    FormatText(first, GetString(0x173), num, 0, 0, 0, 0, 0, 0, 0);
    num = Num_Format(text, 1, (double)slot, 10, 1, 0, 0);
    FormatText(second, GetString(0x174), num, 0, 0, 0, 0, 0, 0, 0);
    const int32_t a = Models_DisplayName(first, shownFirst);
    const int32_t b = Models_DisplayName(second, shownSecond);
    if (!a || !b) return 0;
    FormatText(text, GetString(0x175), shownSecond, shownSecond, shownFirst, 0, 0, 0, 0, 0);
    Log_Add(g_messageLog, 0, text, 4, 0, 0);
    return 0;
}
RT3_REPLACE(0x4409c0, Camera_LoadBookmarkCmd);

RT3_BOOKMARK(0x440b00, Save, 1)

RT3_BOOKMARK(0x440b10, Save, 2)

RT3_BOOKMARK(0x440b20, Save, 3)

RT3_BOOKMARK(0x440b30, Save, 4)

RT3_BOOKMARK(0x440b40, Save, 5)

RT3_BOOKMARK(0x440b50, Save, 6)

RT3_BOOKMARK(0x440b60, Save, 7)

RT3_BOOKMARK(0x440b70, Save, 8)

RT3_BOOKMARK(0x440b80, Save, 9)

RT3_BOOKMARK(0x440b90, Save, 0)

RT3_BOOKMARK(0x440ba0, Load, 1)

RT3_BOOKMARK(0x440bb0, Load, 2)

RT3_BOOKMARK(0x440bc0, Load, 3)

RT3_BOOKMARK(0x440bd0, Load, 4)

RT3_BOOKMARK(0x440be0, Load, 5)

RT3_BOOKMARK(0x440bf0, Load, 6)

RT3_BOOKMARK(0x440c00, Load, 7)

RT3_BOOKMARK(0x440c10, Load, 8)

RT3_BOOKMARK(0x440c20, Load, 9)

RT3_BOOKMARK(0x440c30, Load, 0)

// 0x440c40 (dev command). Display mode +0x1bb / +0x1b7 by option 0x10c.
int32_t Cmd_DisplayModeByOption() {
    if (g_62be68) {
        uint8_t* app = g_app;
        if (OptionTable10c()) Options_SetDisplayMode(app, 0, F<int32_t>(app, 0x1bb));
        else Options_SetDisplayMode(app, 0, F<int32_t>(app, 0x1b7));
    }
    return 0;
}
RT3_REPLACE(0x440c40, Cmd_DisplayModeByOption);

RT3_ROTATE_HOTKEY(0x440c80, Hotkey_RotateLeft, -1, 0x176, 0x177)

RT3_ROTATE_HOTKEY(0x440d40, Hotkey_RotateRight, 1, 0x178, 0x179)

RT3_SELECT_HOTKEY(0x440e00, Hotkey_SelectNext, Buildings_SelectNextFwd, Trains_SelectNextFwd, CompanyView_Next, Vehicles_SelectNextFwd, 0x17a, 0x17b)

RT3_SELECT_HOTKEY(0x440f20, Hotkey_SelectPrev, Buildings_SelectNextBack, Trains_SelectNextBack, CompanyView_Prev, Vehicles_SelectNextBack, 0x17c, 0x17d)

// 0x441040 / 0x4410d0 (hotkeys). Open the train (+0x78) / station (+0x80)
// view, or the company's first one when that is gone.
int32_t Hotkey_ShowTrain() {
    if (!g_world || !g_gameWindow) return 0;
    uint32_t id = F<uint32_t>(g_gameWindow, 0x78);
    if (!Collection_IsValid(g_trains, 0, id) && Obj_Company21(g_world) &&
        Company_TrainCount(Obj_Company21(g_world)) > 0)
        id = Company_NthTrain((int32_t*)Obj_Company21(g_world), 0, 1);
    if (Collection_IsValid(g_trains, 0, id)) View_SetToolMode(g_gameWindow, 0, 2, (int32_t)id);
    return 0;
}
RT3_REPLACE(0x441040, Hotkey_ShowTrain);

int32_t Hotkey_ShowStation() {
    if (!g_world || !g_gameWindow) return 0;
    uint32_t id = F<uint32_t>(g_gameWindow, 0x80);
    if (!Collection_IsValid(g_stations, 0, id) && Obj_Company21(g_world) &&
        Company_CountCargoSites(Obj_Company21(g_world), 0, 0) > 0)
        id = Company_NthSite((int32_t*)Obj_Company21(g_world), 0, 1, 0);
    if (Collection_IsValid(g_stations, 0, id)) View_SetToolMode(g_gameWindow, 0, 5, (int32_t)id);
    return 0;
}
RT3_REPLACE(0x4410d0, Hotkey_ShowStation);

// 0x441160 (dev command). Toggles the overview's plain mode.
int32_t Cmd_TogglePlainOverview() {
    if (g_world && g_map) {
        if (!Obj_Byte21c5(g_map)) Overview_ShowPlainOnce(g_map);
        else Overview_EndPlain(g_map);
    }
    return 0;
}
RT3_REPLACE(0x441160, Cmd_TogglePlainOverview);

// 0x4411a0 (dev command). Toggles the track network's A0 state.
int32_t Cmd_ToggleTrackA0() {
    if (g_world && hook::As<uint8_t*>(g_tracks)) {
        if (Camera_FlagA0(hook::As<uint8_t*>(g_tracks), 0, 0)) Obj_DecA0(hook::As<uint8_t*>(g_tracks));
        else Camera_BeginA0(hook::As<uint8_t*>(g_tracks));
    }
    return 0;
}
RT3_REPLACE(0x4411a0, Cmd_ToggleTrackA0);

int32_t Cmd_ToggleEngine114276() {
    if (uint8_t* e = g_engine) F<int32_t>(e, 0x114276) = !F<int32_t>(e, 0x114276);
    return 0;
}
RT3_REPLACE(0x4411e0, Cmd_ToggleEngine114276);

// 0x441200 (dev command). In single player, the UI gets release 0x9858.
int32_t Cmd_Release9858() {
    if (g_world && CountHumanPlayers() <= 1) Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xcb, 0x9858, 0, 0, 0);
    return 0;
}
RT3_REPLACE(0x441200, Cmd_Release9858);

// Dev commands toggling a flag. Return 0.
int32_t Cmd_Toggle62be84() { return g_62be84 = !g_62be84, 0; }
RT3_REPLACE(0x441240, Cmd_Toggle62be84);

// 0x441260 / 0x4412a0 (dev commands). Editor buttons (enabled / disabled)
// outside scenarios, mode from the engine's +0x114216.
int32_t Cmd_EditorButtons() {
    if (!g_6d3b4c && g_world && g_engine)
        // SETZ AL over the engine pointer: the upper bytes go along.
        World_SetEditorButtons(((uint32_t)(uintptr_t)g_engine & 0xffffff00) |
                                   (F<int32_t>(g_engine, 0x114216) == 1), 0);
    return 0;
}
RT3_REPLACE(0x441260, Cmd_EditorButtons);

int32_t Cmd_EditorButtonsDisabled() {
    if (!g_6d3b4c && g_world && g_engine)
        // SETZ AL over the engine pointer: the upper bytes go along.
        World_SetEditorButtons(((uint32_t)(uintptr_t)g_engine & 0xffffff00) |
                                   (F<int32_t>(g_engine, 0x114216) == 1), 1);
    return 0;
}
RT3_REPLACE(0x4412a0, Cmd_EditorButtonsDisabled);

// 0x4412e0 (dev command). In single player, schedules the next weather
// change and clears the weather override.
int32_t Cmd_NextWeather() {
    if (!g_world || CountHumanPlayers() > 1 || !g_world) return 0;
    if (!RendererMode15Zero() && !Option1df()) return 0;
    Weather_ScheduleNext(g_world, 0, 1);
    Options_SetWeatherOverride(0, 0);
    return 0;
}
RT3_REPLACE(0x4412e0, Cmd_NextWeather);

// 0x441340 / 0x4414d0 / 0x441460 / 0x441870 (dev commands, return 0).
int32_t Cmd_CheatPrompt() {
    if (uint8_t* w = g_world) World_CheatPrompt(w);
    return 0;
}
RT3_REPLACE(0x441340, Cmd_CheatPrompt);

// 0x441360 (dev command). In single player, spawns a flight (0x62b244) and
// follows it.
int32_t Cmd_SpawnFlight() {
    if (!g_world || CountHumanPlayers() > 1 || !g_62b244 || !g_world) return 0;
    if (const uint32_t id = Vehicles_MaybeSpawnFlight(g_62b244, 0, 0.0f, 1))
        World_FollowEntity(g_world, 0, 1, 3, (int32_t)id);
    return 0;
}
RT3_REPLACE(0x441360, Cmd_SpawnFlight);

// 0x4413b0 (dev command). With at most one human player, adds and follows
// train "0x5ca310".
int32_t Cmd_AddTrain() {
    if (g_world && CountHumanPlayers() <= 1 && g_world) Trains_AddAndShow((const char*)0x5ca310, 1);
    return 0;
}
RT3_REPLACE(0x4413b0, Cmd_AddTrain);

int32_t Cmd_ToggleEngine1146be() {
    if (uint8_t* e = g_engine) F<int32_t>(e, 0x1146be) = F<int32_t>(e, 0x1146be) != 1;
    return 0;
}
RT3_REPLACE(0x4413e0, Cmd_ToggleEngine1146be);

// 0x441400 (dev command). Resets the log entries (0x6acd34) from the one
// before the oldest unread (or the newest) onwards.
int32_t Cmd_ResetLogEntries() {
    if (g_world) {
        uint8_t* e = (uint8_t*)Table_Oldest(g_messageLog, 0, 0);
        uint8_t* r = e ? (uint8_t*)Table_FindField4Prev(g_messageLog, 0, F<int32_t>(e, 4))
                       : (uint8_t*)Table_Newest(g_messageLog);
        for (; r; r = (uint8_t*)Table_FindField4Next(g_messageLog, 0, F<int32_t>(r, 4))) Obj_Reset455310(r);
    }
    return 0;
}
RT3_REPLACE(0x441400, Cmd_ResetLogEntries);

int32_t Cmd_MarkOldestLog() {
    if (g_world)
        if (uint8_t* e = (uint8_t*)Table_Oldest(g_messageLog, 0, 0)) e[0xc] = 1;
    return 0;
}
RT3_REPLACE(0x441460, Cmd_MarkOldestLog);

// 0x441490 (dev command). Shows the newspaper before the oldest unread one
// (or the newest) of log 0x6cea4c.
int32_t Cmd_ShowPreviousNewspaper() {
    if (g_world) {
        Collection* log = g_news;
        uint8_t* e = (uint8_t*)Table_Oldest14(log, 0, 0);
        uint8_t* r = e ? (uint8_t*)Table_FindField14Prev(g_news, 0, F<int32_t>(e, 0x14))
                       : (uint8_t*)Table_Newest14(g_news);
        if (r) Newspaper_Show(r);
    }
    return 0;
}
RT3_REPLACE(0x441490, Cmd_ShowPreviousNewspaper);

int32_t Cmd_ReopenLastEvent() {
    if (uint8_t* w = g_world)
        if (F<int32_t>(g_windowManager, 0xc64) <= 0) World_ReopenLastEvent(w);
    return 0;
}
RT3_REPLACE(0x4414d0, Cmd_ReopenLastEvent);

// 0x441500 (hotkey). "Find city": asks for a name (string 0x16a, up to 30
// characters) and centres the camera on the first city (0x62bae0, not +0x23e)
// whose name (+0x356) starts with it, case-insensitively; else says so
// (string 0x16b).
int32_t Cities_FindByPrompt() {
    if (!g_world) return 0;
    char frame[0x190];
    char* name = frame;
    char* msg = frame + 0x64;
    name[0] = 0;
    Dialog_RunMessage(GetString(0x16a), 5, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)name, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    const int32_t len = (int32_t)strlen(name);
    if (len <= 0) return 0;
    for (int32_t i = 0; i < Collection_Count(g_cities); ++i) {
        Collection* cities = g_cities;
        uint8_t* c = (uint8_t*)Collection_Get(cities, 0, Collection_IdAt(cities, 0, (uint32_t)i, 0));
        if (F<int32_t>(c, 0x23e) || crt::strnicmp(name, (const char*)c + 0x356, (size_t)len)) continue;
        if (g_62be68) Obj_ClearClock346(g_62be68);
        g_62be70 = 1;
        Camera_FollowObject(c, 0);
        g_62be70 = 0;
        if (uint8_t* view = g_62be68) ((void(__thiscall*)(uint8_t*))(*(void***)view)[7])(view);
        return 0;
    }
    FormatText(msg, GetString(0x16b), name, 0, 0, 0, 0, 0, 0, 0);
    Dialog_RunMessage(msg, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    return 0;
}
RT3_REPLACE(0x441500, Cities_FindByPrompt);

// 0x441690 (hotkey). Multiplayer chat: asks for a line (string 0xb6d, 63
// characters, 0x62be80 set meanwhile) and sends a non-empty one (message 1).
// Returns 0, or 1 when not in a running multiplayer game.
int32_t Hotkey_Chat() {
    if (!g_world || !IsMultiplayer() || !F<int32_t>(g_world, 0x66ae)) return 1;
    char line[0x40] = {};
    Obj_Incc60(g_windowManager);
    g_62be80 = 1;
    Message447(GetString(0xb6d), 0xb, (uint32_t)(uintptr_t)line, 0x3f);
    if (line[0]) Net_Send(g_session, 0, nullptr, 1, (int32_t)strlen(line) + 1, (const uint8_t*)line, 1, 0);
    Obj_Decc60(g_windowManager);
    g_62be80 = 0;
    return 0;
}
RT3_REPLACE(0x441690, Hotkey_Chat);

// 0x441790. The player's own train under focus: the camera's followed
// vehicle (+0x60, if virtual slot 27 says so), else the selected train; 0 if not own.
RT3_NOINLINE void* FocusedOwnTrain() {
    uint8_t* cam = g_62be68;
    if (!cam) return nullptr;
    uint8_t* t = nullptr;
    if (void* v = F<void*>(cam, 0x60)) {
        if (((int32_t(__thiscall*)(void*))(*(void***)v)[0x6c / 4])(v)) {
            t = (uint8_t*)Collection_Get(g_trains, 0,
                                         F<uint32_t>(F<uint8_t*>(g_62be68, 0x60), 0x390));
            if (t && OwnTrain(t)) return t;
        }
    }
    t = (uint8_t*)SelectedTrain();
    if (t && !OwnTrain(t)) t = nullptr;
    return t;
}
RT3_REPLACE(0x441790, FocusedOwnTrain);

// 0x441810 / 0x4418a0 (dev commands). Crash / break down the focused own
// train in single player (string 0x16d / 0x16e without one).
int32_t Cmd_CrashTrain() {
    if (!g_world || CountHumanPlayers() > 1) return 0;
    if (!FocusedOwnTrain()) {
        Message_Dialog(GetString(0x16d), 0xbf800000);
        return 0;
    }
    Train_Crash((uint8_t*)FocusedOwnTrain(), 0, 1);
    return 0;
}
RT3_REPLACE(0x441810, Cmd_CrashTrain);

int32_t Cmd_FlagTrainCity() {
    if (FocusedOwnTrain()) F<int32_t>((uint8_t*)Obj_City41(FocusedOwnTrain()), 0x33a) = 1;
    return 0;
}
RT3_REPLACE(0x441870, Cmd_FlagTrainCity);

int32_t Cmd_BreakDownTrain() {
    if (!g_world || CountHumanPlayers() > 1) return 0;
    if (!FocusedOwnTrain()) {
        Message_Dialog(GetString(0x16e), 0xbf800000);
        return 0;
    }
    Train_BreakDown((uint8_t*)FocusedOwnTrain(), 0, 1);
    return 0;
}
RT3_REPLACE(0x4418a0, Cmd_BreakDownTrain);

RT3_TOOL9(0x441900, Cmd_ToolMode9_0, 0)

RT3_TOOL9(0x441940, Cmd_ToolMode9_1, 1)

RT3_TOOL9(0x441980, Cmd_ToolMode9_2, 2)

RT3_TOOL9(0x4419c0, Cmd_ToolMode9_3, 3)

RT3_TOOL9(0x441a00, Cmd_ToolMode9_4, 4)

// 0x441a40 / 0x441a80 (hotkeys). Game speed up / down, with sound 0x2c when
// it changed.
int32_t Hotkey_SpeedUp() {
    if (uint8_t* w = g_world) {
        const int32_t was = F<int32_t>(w, 0x19);
        World_StepSpeed(w, 0, 1);
        if (F<int32_t>(g_world, 0x19) != was) Ui_PlaySound(0x2c, 0, 0x3f000000);
    }
    return 0;
}
RT3_REPLACE(0x441a40, Hotkey_SpeedUp);

int32_t Hotkey_SpeedDown() {
    if (uint8_t* w = g_world) {
        const int32_t was = F<int32_t>(w, 0x19);
        World_StepSpeed(w, 0, -1);
        if (F<int32_t>(g_world, 0x19) != was) Ui_PlaySound(0x2c, 0, 0x3f000000);
    }
    return 0;
}
RT3_REPLACE(0x441a80, Hotkey_SpeedDown);

// 0x441ac0 / 0x441af0 (hotkeys). Quick load (flags 0 / 1) in a game unless
// a modal window is up (0x6d401c +0xc64).
int32_t Hotkey_QuickLoad() {
    if (g_world && F<int32_t>(g_windowManager, 0xc64) <= 0)
        World_Load(g_world, 0, nullptr, 0, nullptr);
    return 0;
}
RT3_REPLACE(0x441ac0, Hotkey_QuickLoad);

int32_t Hotkey_QuickLoadAlt() {
    if (g_world && F<int32_t>(g_windowManager, 0xc64) <= 0)
        World_Load(g_world, 0, nullptr, 1, nullptr);
    return 0;
}
RT3_REPLACE(0x441af0, Hotkey_QuickLoadAlt);

// 0x441b20 (dev command). Toggles the map's +0x168d between +0x1689 and one
// more.
int32_t Cmd_ToggleMap168d() {
    if (uint8_t* m = g_map) {
        const int32_t base = F<int32_t>(m, 0x1689);
        F<int32_t>(m, 0x168d) = base + (F<int32_t>(m, 0x168d) == base ? 1 : 0);
    }
    return 0;
}
RT3_REPLACE(0x441b20, Cmd_ToggleMap168d);

// 0x441b50 (hotkey). Toggles the editor (0x6cec74 +0x68) in a game with a
// player: leaving asks once (0x62be88, string 0x17e); entering is refused in
// multiplayer (string 0xb6e) and asks once (0x62be8c, string 0xb24). Only a
// confirmed 0x3f2 or an earlier confirmation switches the view mode.
int32_t Editor_ToggleCb() {
    if (!g_world || !g_gameWindow) return 0;
    if (F<int32_t>(g_app, 0x68)) {
        if (!g_62be88) {
            g_62be88 = 1;
            if (Message449(GetString(0x17e), 2) != 0x3f2) return 0;
        }
        if (!F<int32_t>(g_app, 0x68)) goto entering;
    } else {
    entering:
        if (CountHumanPlayers() > 1) {
            Message449(GetString(0xb6e), 1);
            return 0;
        }
        if (!F<int32_t>(g_app, 0x68) && !g_62be8c) {
            g_62be8c = 1;
            if (Message449(GetString(0xb24), 2) != 0x3f2) return 0;
        }
    }
    uint8_t* app = g_app;
    App_SetViewMode(app, 0, F<int32_t>(app, 0x68) == 0 ? 1 : 0);
    return 0;
}
RT3_REPLACE(0x441b50, Editor_ToggleCb);

// 0x441cb0 / 0x441d20 (dev commands). In single player, move the world's
// time of day (+0xbfa) by 0x5c8628 (with the clock options off).
int32_t Cmd_TimeOfDayUp() {
    if (!g_world || CountHumanPlayers() > 1 || !g_world) return 0;
    F<int32_t>(g_app, 0x233) = 0;
    F<int32_t>(g_app, 0x124) = 0;
    uint8_t* w = g_world;
    F<float>(w, 0xbfa) = (float)((double)F<float>(w, 0xbfa) + D(0x5c8628));
    World_SetTime(g_world, 0, F<int32_t>(g_world, 0x15));
    Obj_Set65(g_62be68);
    return 0;
}
RT3_REPLACE(0x441cb0, Cmd_TimeOfDayUp);

int32_t Cmd_TimeOfDayDown() {
    if (!g_world || CountHumanPlayers() > 1 || !g_world) return 0;
    F<int32_t>(g_app, 0x233) = 0;
    F<int32_t>(g_app, 0x124) = 0;
    uint8_t* w = g_world;
    F<float>(w, 0xbfa) = (float)((double)F<float>(w, 0xbfa) - D(0x5c8628));
    World_SetTime(g_world, 0, F<int32_t>(g_world, 0x15));
    Obj_Set65(g_62be68);
    return 0;
}
RT3_REPLACE(0x441d20, Cmd_TimeOfDayDown);

// 0x441d90. "Saving Game... N%" (string 0x180) into the static buffer 0x62c0d8, N
// being done / total in percent, truncated and capped at 100.
RT3_NOINLINE const char* __stdcall Save_ProgressText(int32_t total, float done) {
    float t = (float)total;
    int32_t pct = Ftol((double)done / t * (double)Fl(0x5c8c28));
    if (pct > 100) pct = 100;
    char num[16];
    Num_Format(num, 1, (double)pct, 10, 1, 0, 0);
    return FormatText((char*)0x62c0d8, GetString(0x180), num, 0, 0, 0, 0, 0, 0, 0);
}
RT3_REPLACE(0x441d90, Save_ProgressText);

// 0x441ec0. File in ESI, block in EBX, stream in EDI.
__declspec(naked) void Block3c2_Save() {
    __asm {
        push edi
        push ebx
        push esi
        call Block3c2_SaveImpl
        add esp, 12
        ret
    }
}
RT3_REPLACE(0x441ec0, Block3c2_Save);

// 0x441f70. Block in EAX, path on the stack.
__declspec(naked) void Block_AppendImage() {
    __asm {
        push [esp + 4]
        push eax
        call Block_AppendImageImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x441f70, Block_AppendImage);

// 0x4420f0. Path in ECX, block in EAX, out on the stack.
__declspec(naked) void Block_ExtractImage() {
    __asm {
        push [esp + 4]
        push eax
        push ecx
        call Block_ExtractImageImpl
        add esp, 12
        ret 4
    }
}
RT3_REPLACE(0x4420f0, Block_ExtractImage);

// 0x4422d0. Appends the current option bytes (+0x140, +0x13c, +0x138,
// +0x144) and world byte +0x4c74 to the 5-byte history 0x62be90.
RT3_NOINLINE void AppendOptionHistory() {
    int32_t n = g_62bedc;
    uint8_t* opt = g_app;
    uint8_t* d = (uint8_t*)0x62be90 + n * 5;
    d[0] = opt[0x140];
    d[1] = opt[0x13c];
    d[2] = opt[0x138];
    d[3] = opt[0x144];
    d[4] = g_world[0x4c74];
    g_62bedc = n + 1;
}
RT3_REPLACE(0x4422d0, AppendOptionHistory);

// 0x442330. Pops the last option history entry (see 0x4422d0) back into the options.
RT3_NOINLINE void PopOptionHistory() {
    int32_t n = --g_62bedc;
    const uint8_t* d = (const uint8_t*)0x62be90 + n * 5;
    uint8_t* opt = g_app;
    F<int32_t>(opt, 0x140) = (int8_t)d[0];
    F<int32_t>(g_app, 0x13c) = (int8_t)d[1];
    F<int32_t>(g_app, 0x138) = (int8_t)d[2];
    F<int32_t>(g_app, 0x144) = d[3];
    F<int32_t>(g_world, 0x4c74) = d[4];
}
RT3_REPLACE(0x442330, PopOptionHistory);

// 0x4423a0. Before leaving the map view: records the option history and
// suspends the track tool (0x6d1a8c) and the info window highlights
// (0x6d1720) when open.
RT3_NOINLINE void Game_PauseTools() {
    AppendOptionHistory();
    if (g_trackTool) TrackTool_Suspend(g_trackTool);
    if (g_buildingDialog) InfoWindow_ClearHighlights(g_buildingDialog);
}
RT3_REPLACE(0x4423a0, Game_PauseTools);

// 0x4423d0. Back from a modal screen: restores the saved options and lets
// an open building dialog (0x6d1720) and the track tool (0x6d1a8c) resume.
RT3_NOINLINE void Options_Restore() {
    PopOptionHistory();
    if (uint8_t* dlg = g_buildingDialog) BuildingDialog_Retrack(dlg);
    if (uint8_t* tool = g_trackTool) TrackTool_Resume(tool);
}
RT3_REPLACE(0x4423d0, Options_Restore);

// 0x442400. Loads the description block (0x403c2 bytes) of save / map name (from
// the tutorial folder when tutorial is set). With option 0x1bf the whole block is
// read from file version 0x3f1 on (before 0x3f7 only up to +0x2c9, the rest
// cleared); otherwise its 0x3c2-byte header is read and the preview (+0x3c2,
// 0xffRRGGBB) is decoded from the JPEG appended at the header's offset +0x18.
RT3_NOINLINE void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial) {
    memset(block, 0, 0x403c2);
    char path[0x120];
    const char* folder = tutorial ? (const char*)0x5ca338 : Options_SaveFolder(hook::As<void*>(g_app));
    FormatText(path, (const char*)0x5c9d00, folder, name, 0, 0, 0, 0, 0, 0);
    int32_t s = StreamOpen(path, 1, 0x30d40, 1);
    int32_t version;
    if (Options_Check1bf(hook::As<void*>(g_app))) {
        StreamRead(s, &version, 4, 0, 1);
        if (version >= 0x3f1) {
            StreamSkip4(s, 0x2ee0, 0, 0);
            if (version >= 0x3f7) {
                StreamRead(s, block, 0x403c2, 0, 1);
            } else {
                StreamRead(s, block, 0x402c9, 0, 1);
                memset(block + 0x2c9, 0, 0x400f9);
            }
            StreamSkip4(s, 0x2ee1, 0, 0);
        }
        StreamClose(s);
        return;
    }
    StreamRead(s, &version, 4, 0, 1);
    StreamSkip4(s, 0x2ee0, 1, 1);
    StreamRead(s, block, 0x3c2, 0, 1);
    StreamSkip4(s, 0x2ee1, 1, 1);
    StreamClose(s);
    int32_t w, h;
    uint8_t* pix = Jpeg_Load(path, &w, &h, F<int32_t>(block, 0x18));
    for (int32_t y = 0; y < h; ++y)
        for (int32_t x = 0; x < w; ++x) {
            int32_t i = y * w + x;
            const uint8_t* p = pix + i * 3;
            F<uint32_t>(block, 0x3c2 + i * 4) = 0xff000000u | (uint32_t)p[0] << 16 | (uint32_t)p[1] << 8 | p[2];
        }
    crt::free(pix);
}
RT3_REPLACE(0x442400, Block_Load);

// 0x4425d0. Takes the game options stored with save name (its description block,
// when its flag +0x21 is set): 0x6cec7c +0xc5 = 1, +0xc4 / +0x7d from +0x22 /
// +0x33 and the 16 bytes +0xc6.. from +0x23..; then drops input handler 0xcc.
// 1 when the save had them.
RT3_NOINLINE int32_t __fastcall Save_LoadOptions(const char* name) {
    uint8_t block[0x403c4];
    Block_Load(name, block, 0);
    if (!block[0x21]) return 0;
    F<uint8_t>(g_newGame, 0xc5) = 1;
    F<uint8_t>(g_newGame, 0xc4) = block[0x22];
    F<uint8_t>(g_newGame, 0x7d) = block[0x33];
    for (int32_t i = 0; i < 0x10; ++i) F<uint8_t>(g_newGame, 0xc6 + i) = block[0x23 + i];
    Input_RemoveHandler(g_input, 0, 0xcc);
    return 1;
}
RT3_REPLACE(0x4425d0, Save_LoadOptions);

// 0x442660. Debug log line (string 0x198) with the KB written to stream s
// since the last call and the elapsed ms since 0x62bed0.
RT3_NOINLINE void __fastcall Save_LogProgress(Stream s, const char* label) {
    if (!s && !label) return;
    char kb[0x20], ms[0x20], line[0x1ec];
    int32_t pos = StreamTell(s);
    crt::sprintf(kb, (const char*)0x5ca350, (pos - g_62c11c) / 1024);
    crt::sprintf(ms, (const char*)0x5ca348, ClockMs() - g_62bed0);
    FormatText(line, GetString(0x198), kb, ms, label, nullptr, nullptr, nullptr, nullptr, nullptr);
    DebugLog(line, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234, -1234);
    g_62c11c = pos;
}
RT3_REPLACE(0x442660, Save_LogProgress);

// 0x442740. Writes the 256 x 256 preview bitmap of o (+0x3c2) to the JPEG
// file `path` (quality 70) through a memory texture; the result goes to
// +0x18.
RT3_NOINLINE void __stdcall Preview_SaveJpeg(const char* path, uint8_t* o) {
    void* pix = crt::malloc(0x40000);
    memcpy(pix, o + 0x3c2, 0x40000);
    uint8_t* p = (uint8_t*)crt::operator_new(0xec);
    uint8_t* t = p ? Texture_InitMemory(p, 0, pix, 0x100, 0x100, 0) : nullptr;
    F<int32_t>(o, 0x18) = Texture_AppendJpeg(t, 0, path, 0, 0x46);
    if (!t) return;
    Texture_Destroy(t);
    crt::operator_delete(t);
}
RT3_REPLACE(0x442740, Preview_SaveJpeg);

// 0x4427c0. Unpacks save file path into path's base name + ".tmp": its 0x3ce-byte
// header, the LZSS payload at +0x3ce unpacked, then everything after the packed
// data. Nothing is written when unpacking fails.
RT3_NOINLINE void __fastcall Save_UnpackToTemp(const char* path) {
    char name[0x30];
    strcpy(name, path);
    StripExtension(name);
    memcpy(name + strlen(name), (const void*)0x5ca354, 5);
    int32_t size;
    uint8_t* buf = B60ReadAll(path, &size);
    uint8_t* out = nullptr;
    int32_t outSize = 0;
    int32_t packer[3] = { 0x91, 0, 0 };
    if (Pack_Decompress(packer, 0, (const int32_t*)(buf + 0x3ce), 0, &out, &outSize) == 1) {
        void* f = crt::fopen(name, (const char*)0x5ca334);
        crt::fwrite(buf, 1, 0x3ce, f);
        crt::fwrite(out, 1, (size_t)outSize, f);
        int32_t packed = F<int32_t>(buf + 0x3ce, 4);
        crt::fwrite(buf + 0x3ce + packed, 1, (size_t)(size - packed - 0x3ce), f);
        crt::fclose(f);
        crt::free(out);
    }
    crt::free(buf);
    ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
}
RT3_REPLACE(0x4427c0, Save_UnpackToTemp);

// 0x442900. Block in EAX, path on the stack.
__declspec(naked) void Save_Pack() {
    __asm {
        push [esp + 4]
        push eax
        call Save_PackImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x442900, Save_Pack);

// 0x442a40. Whether any element of 0x6cfcbc refers (city +0x41, then +0x388, name
// +0x82) to one of eight fixed names.
RT3_NOINLINE int32_t __cdecl AnyHasFixedName() {
    static const uint32_t names[8] = { 0x5ca3a4, 0x5ca39c, 0x5ca390, 0x5ca384, 0x5ca378, 0x5ca36c, 0x5ca364, 0x5ca35c };
    for (int32_t i = 0; i < Collection_Count(g_trains); ++i) {
        Collection* c = g_trains;
        void* x = Obj_City41(Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0)));
        for (int k = 0; k < 8; ++k)
            if (!crt::stricmp((const char*)Obj_Ref388(x) + 0x82, (const char*)names[k])) return 1;
    }
    return 0;
}
RT3_REPLACE(0x442a40, AnyHasFixedName);

// 0x442ba0. Writes the save-game preview of map m to dst: the 0x403c2-byte
// block +0x66be, tagged 1 with the map size, two engine settings and
// 0x62bec4, and a fresh 256x256 minimap at dst + 0x3c2.
RT3_NOINLINE void __fastcall Map_WritePreview(uint8_t* m, int, uint8_t* dst) {
    memcpy(dst, m + 0x66be, 0x100f0 * 4 + 2);
    dst[0] = 1;
    F<int32_t>(dst, 1) = F<int32_t>(g_map, 0x2155);
    F<int32_t>(dst, 5) = F<int32_t>(g_map, 0x2159);
    dst[9] = g_engine[0x11471a];
    dst[0xa] = g_engine[0x11471e];
    dst[0xb] = hook::As<uint8_t>(g_62bec4);
    Minimap_Render((uint32_t*)(dst + 0x3c2), 0x100, 0x100, 0, 0, 0, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr);
}
RT3_REPLACE(0x442ba0, Map_WritePreview);

// 0x442c30. Hand-made fixes applied when the scenario `name` is loaded (this +0x66de:
// a saved game rather than a fresh start): repairs to specific event rules (by id
// in 0x62be18, guarded by their name and expected values), city sizes (Go West!,
// Orient Express), a misplaced city and industry (Chicago to New York), a region
// patch on the map (France), texts (Central Pacific, New Beginnings, Tex-Mex), and
// for newer saves the post office cost (25000 -> 40000). Several fixes only take
// effect in the state they check for; one ("Setup P3 Welcome") writes back the value
// it just compared against.
RT3_NOINLINE void __fastcall Scenario_ApplyFixes(uint8_t* scn, int, const char* name) {
    auto rule = [](uint32_t id) { return (uint8_t*)Collection_Get(g_events, 0, id); };
    auto city = [](uint32_t id) { return (uint8_t*)Collection_Get(g_cities, 0, id); };
    auto named = [](uint8_t* r, uint32_t str) { return !crt::stricmp((const char*)(r + 0x18), (const char*)str); };
    auto is = [&](uint32_t str) { return !crt::stricmp(name, (const char*)str); };
    auto cond = [](uint8_t* r) { return F<uint8_t*>(r, 0); };
    if (is(0x5ca838) && !scn[0x66de]) { // Germany
        uint8_t* r = rule(7);
        if (r && named(r, 0x5ca82c)) {
            uint8_t* c = cond(r);
            if (c && F<int32_t>(c, 0) == 0x1b && !c[4] && r[0x7f9] == 1) r[0x7f9] = 2;
        }
    }
    auto nudge = [&](uint32_t id, uint32_t ref, uint32_t add) {
        uint8_t* c = city(id);
        if (!c || !(fabs((double)F<float>(c, 0x25a) - Fl(ref)) < D(0x5c8bf0))) return;
        float v = (float)((double)F<float>(c, 0x25e) + Fl(add));
        F<float>(c, 0x25e) = v;
        F<float>(c, 0x25a) = v;
    };
    if (is(0x5ca820) && scn[0x66de]) { // Go West!
        nudge(0x35, 0x5c9ac8, 0x5c9690);
        nudge(0x2f, 0x5c8674, 0x5c9690);
        nudge(0x21, 0x5c8674, 0x5c8554);
    }
    if (is(0x5ca810) && scn[0x66de]) { // Orient Express
        uint32_t ids[2] = { 0x64, 0x5f };
        for (uint32_t id : ids) {
            uint8_t* c = city(id);
            if (c && fabs((double)F<float>(c, 0x25a) - Fl(0x5c8784)) < D(0x5c8bf0)) {
                F<uint32_t>(c, 0x25e) = 0x42200000;
                F<uint32_t>(c, 0x25a) = 0x42200000;
            }
        }
    }
    if (is(0x5ca800) && scn[0x66de]) { // Central Pacific
        uint8_t* r = rule(5);
        if (r && named(r, 0x5ca7f8)) {
            static const uint32_t texts[9] = { 0x5ca7c4, 0x5ca78c, 0x5ca748, 0x5ca70c, 0x5ca6d0,
                                               0x5ca6a8, 0x5ca674, 0x5ca640, 0x5ca600 };
            for (uint32_t t : texts) Str_Replace((char*)(r + 0xe0), 0x320, (const char*)t, (const char*)0x5c87a8, 1);
        }
        r = rule(0x10);
        if (r && cond(r) && named(r, 0x5ca5ec)) {
            uint8_t* c = cond(r);
            if (F<int32_t>(c, 0) == 0x1b && F<int32_t>(c, 9) == 3) F<int32_t>(c, 9) = 3;
        }
    }
    if (is(0x5ca5d8) && !scn[0x66de]) { // Chicago to New York
        uint8_t* c = city(0x4b);
        if (Game_EffectiveVersion() >= 0x67 && c && Ftol(Obj_CellX(c)) == 0xd0 && Ftol(Obj_CellY(c)) == 0xd6) {
            Obj_MoveTo(c, 0, F<float>(c, 0x1e2), F<float>(c, 0x1e6), BitsF(0x468d9800));
            City_LayStreets(c);
            Cities_AssignBlocks(g_cities);
        }
        uint8_t* o = (uint8_t*)Collection_Get(g_6ada80, 0, 0xa);
        if (Game_EffectiveVersion() >= 0x67 && o && Ftol(Obj_CellX(o)) == 0xcb && Ftol(Obj_CellY(o)) == 0xc2)
            Obj_MoveTo(o, 0, BitsF(0x4684f800), F<float>(o, 0x1e6), BitsF(0x468e5000));
    }
    if (is(0x5ca5d0) && !scn[0x66de]) { // France
        uint8_t* r = rule(0x1b);
        uint8_t* m = g_map;
        if (Game_EffectiveVersion() >= 0x67 && r && named(r, 0x5ca5bc) && F<int32_t>(m, 0x2155) == 0x1c0 &&
            F<int32_t>(m, 0x2159) == 0x1c0) {
            const uint16_t* ends = (const uint16_t*)0x5ee508;
            for (int32_t row = 0; ends + row < (const uint16_t*)0x5ee5cc; ++row)
                for (int32_t col = 0x15e; col <= (int32_t)ends[row]; ++col) {
                    m = g_map;
                    uint8_t& cell = F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * (row + 0x115) + col];
                    if ((cell & 0xf8) == 0x28) cell = (uint8_t)((cell & 7) | 8);
                }
            Territories_Recount(g_territories);
        }
    }
    if (is(0x5ca5a8) && scn[0x66de]) { // State of Germany
        uint8_t* a = rule(0xf);
        uint8_t* b = rule(0x10);
        if (a && cond(a) && named(a, 0x5ca594) && F<int32_t>(cond(a), 0) == 0x1b && F<int32_t>(cond(a), 9) == 2 &&
            F<uint8_t*>(a, 4) && F<int32_t>(F<uint8_t*>(a, 4), 0) == 0x3b && b && cond(b) && named(b, 0x5ca5ec) &&
            F<int32_t>(cond(b), 0) == 0x1b && F<int32_t>(cond(b), 9) == 3 && F<uint8_t*>(b, 4) &&
            F<int32_t>(F<uint8_t*>(b, 4), 0) == 0x38) {
            uint32_t tmp[10];
            memcpy(tmp, F<void*>(a, 4), 0x28);
            memcpy(F<void*>(a, 4), F<void*>(b, 4), 0x28);
            memcpy(F<void*>(b, 4), tmp, 0x28);
        }
        uint8_t* r = rule(0x10);
        if (r && cond(r) && named(r, 0x5ca5ec) && F<int32_t>(cond(r), 0) == 0x1b && F<int32_t>(cond(r), 9) == 3)
            F<int32_t>(cond(r), 9) = 3;
        r = rule(0x14);
        if (r && cond(r) && named(r, 0x5ca584) && F<int32_t>(cond(r), 0) == 0x1b && F<int32_t>(cond(r), 9) == 2)
            F<int32_t>(cond(r), 9) = 3;
        r = rule(0x15);
        if (r && cond(r) && named(r, 0x5ca570) && F<int32_t>(cond(r), 0) == 0x1b && F<int32_t>(cond(r), 9) == 2)
            F<int32_t>(cond(r), 9) = 3;
        r = rule(0x16);
        if (r && cond(r) && F<void*>(cond(r), 0x1a) && named(r, 0x5ca560)) {
            uint8_t* n = F<uint8_t*>(cond(r), 0x1a);
            if (F<int32_t>(n, 0) == 0x1b && F<int32_t>(n, 9) == 2) F<int32_t>(n, 9) = 3;
        }
        r = rule(0x17);
        if (r && cond(r) && F<void*>(cond(r), 0x1a) && named(r, 0x5ca54c) && F<int32_t>(cond(r), 0) == 0x1b &&
            F<int32_t>(cond(r), 9) == 3)
            F<int32_t>(cond(r), 9) = 2;
        r = rule(0x18);
        if (r && cond(r) && F<void*>(cond(r), 0x1a) && named(r, 0x5ca538)) {
            uint8_t* n = F<uint8_t*>(cond(r), 0x1a);
            if (F<int32_t>(n, 0) == 0x1d && F<int32_t>(n, 9) == 1) {
                F<int32_t>(n, 0) = 0x1b;
                F<int32_t>(F<uint8_t*>(cond(r), 0x1a), 9) = 2;
            }
        }
        r = rule(0x19);
        if (r && cond(r) && F<void*>(cond(r), 0x1a) && named(r, 0x5ca524)) {
            uint8_t* n = F<uint8_t*>(cond(r), 0x1a);
            if (F<int32_t>(n, 0) == 0x1b && F<int32_t>(n, 9) == 3) F<int32_t>(n, 9) = 2;
        }
    }
    if (is(0x5ca518)) { // Dutchlantis
        uint8_t* r = rule(1);
        if (r && named(r, 0x5ca50c) && cond(r) && F<int32_t>(cond(r), 0) == 7 && cond(r)[4] == 5 && scn[0x66de] &&
            r[0x7ef] == 1)
            r[0x7ef] = 5;
    }
    if (is(0x5ca4fc)) { // New Beginnings
        Str_Replace((char*)(scn + 0x4f30), 0xbb8, (const char*)0x5ca4b4, (const char*)0x5ca4d4, 1);
        Str_Replace((char*)(scn + 0x4f30), 0xbb8, (const char*)0x5ca474, (const char*)0x5ca494, 1);
    }
    if (is(0x5ca468)) { // Germantown
        uint8_t* r = rule(0xd);
        if (r && named(r, 0x5ca460) && cond(r) && F<int32_t>(cond(r), 0) == 0x3c && cond(r)[4] == 4 && scn[0x66de] &&
            !r[0x7ef]) {
            uint8_t* e = F<uint8_t*>(r, 4);
            if (e && F<uint8_t*>(e, 0x24) && F<int32_t>(F<uint8_t*>(e, 0x24), 0) == 0x3d &&
                F<int32_t>(F<uint8_t*>(e, 0x24), 4) == -0xf) {
                r[0x7ef] = 2;
                F<int32_t>(F<uint8_t*>(e, 0x24), 4) = -0x14;
            }
        }
        r = rule(0xa);
        if (r && named(r, 0x5ca450) && r[0x7ef] == 7) {
            uint8_t* e = F<uint8_t*>(r, 8);
            if (e && F<int32_t>(e, 0) == 1 && F<int32_t>(e, 4) == 0x2bf20) F<int32_t>(e, 4) = 0x29810;
        }
    }
    if (is(0x5ca448)) { // Britain
        uint8_t* r = rule(7);
        if (r && named(r, 0x5ca434) && cond(r) && F<int32_t>(cond(r), 0xe) == 0x20 && F<int32_t>(cond(r), 0x12) == 0x38 &&
            cond(r)[4] == 4 && scn[0x66de] && r[0x7ef] == 6 && r[0x7f9] == 1)
            r[0x7f9] = 2;
        r = rule(4);
        if (r && named(r, 0x5ca428)) {
            uint8_t* e = F<uint8_t*>(r, 4);
            if (e && F<int32_t>(e, 0) == 0x92 && F<int32_t>(e, 4) == -5 && scn[0x66de] && r[0x7ef] == 2) F<int32_t>(e, 4) = -10;
        }
        r = rule(0xf);
        if (r && named(r, 0x5ca41c)) {
            uint8_t* e = F<uint8_t*>(r, 4);
            if (e && F<int32_t>(e, 0) == 0xce && F<int32_t>(e, 4) == 0x19 && scn[0x66de] && r[0x7ef] == 3) F<int32_t>(e, 0) = 0xcf;
        }
    }
    if (is(0x5ca404)) { // South East Australia
        uint8_t* r = rule(8);
        if (r && named(r, 0x5ca3f0) && F<uint8_t*>(r, 4)) {
            uint8_t* e = F<uint8_t*>(F<uint8_t*>(r, 4), 0x24);
            if (e && F<int32_t>(e, 0) == 0x2b && F<int32_t>(e, 4) == 2 && !scn[0x66de] && !r[0x7ef] && r[0x7f9] == 2)
                F<int32_t>(e, 4) = 3;
        }
    }
    if (is(0x5ca3e8)) { // Tex-Mex
        uint8_t* r = rule(0x10);
        if (r && named(r, 0x5ca3dc) && cond(r) && F<int32_t>(cond(r), 0) == -1 && cond(r)[4] == 4 && r[0x7f4] == 1 &&
            r[0x7ef] == 9)
            Str_Replace((char*)(r + 0xe0), (int32_t)strlen((const char*)(r + 0xe0)), (const char*)0x5ca3d4,
                        (const char*)0x5ca3d8, 1);
    }
    if (is(0x5ca4fc)) { // New Beginnings
        struct Win {
            uint32_t id, str;
            int32_t type, value;
            uint8_t sub, state;
        };
        static const Win wins[3] = { { 1, 0x5ca3c8, 0x1b, 1, 0, 5 }, { 2, 0x5ca3b8, 0x1a, 0x989680, 4, 1 },
                                     { 3, 0x5ca3a8, 0x1a, 0x4c4b40, 4, 1 } };
        for (const Win& w : wins) {
            uint8_t* r = rule(w.id);
            if (!r || !named(r, w.str) || !cond(r)) continue;
            uint8_t* c = cond(r);
            if (F<int32_t>(c, 0) == w.type && c[4] == w.sub && F<int32_t>(c, 9) == w.value && c[0xd] == 2 &&
                r[0x7ef] == w.state && r[0x7fa] == 1)
                r[0x7fa] = 2;
        }
    }
    if (Game_EffectiveVersion() >= 0x67) {
        for (int32_t i = 0; i < Collection_Count(g_62b268); ++i) {
            Collection* types = g_62b268;
            uint8_t* t = (uint8_t*)Collection_Get(types, 0, Collection_IdAt(types, 0, i, 0));
            if (!crt::stricmp((const char*)(t + 4), (const char*)0x5c8f88) && F<int32_t>(t, 0x22) == 25000) F<int32_t>(t, 0x22) = 40000;
        }
    }
}
RT3_REPLACE(0x442c30, Scenario_ApplyFixes);

// 0x443a50 (thiscall, RET 8). World w loads the saved game `name` from the
// save folder (`show`: with the progress screen, busy cursor and a cleared
// view). The name without extension goes to 0x62bee0, the tool mode is
// reset and the old world torn down, rebuilt (World_Startup) and reset.
// The file starts with the version (+0x46c34) and the header block (+0x66be);
// a file with an image (+0x66c8) is reopened past it (unpacked first with
// +0x66c9, the image extracted next to it). Then the world record (+4), the
// texts +0x4f30 (before 0x2588 copied from +0xbfe) and +0x5ae9 (from 0x2589),
// the templates (+0x66b2) and, from 0x2584, the lobby: a mode 1 / 7
// (tutorial) load keeps the current one; a scenario started fresh (+0x97)
// takes the saved one around its own player range (+0xc3 .. +0x107) and
// starts at the scenario's year (+0x77, a year earlier with the start
// option 1 or world +0x4af7, 3 with option 2); other loads take it whole
// (before 0x2587 reset) and an editor load in mode 3 keeps its year. The
// name table (+0x66b6; "H10 2-8-2 " loses its trailing space), the map ("%s" of the name,
// QUIRK: the name is the format), camera, every table (progress 0x185 ..
// 0x196 between them), the speed (+0x19, 0 in multiplayer) and a 500-byte
// tail follow, each section tagged. Temporary files are removed, the derived
// data rebuilt (coast for old saves or maps without it, borders, streets,
// demand, terrain grids), scenario fixes applied and a fresh scenario starts
// (World_PrepareNewGame, else 0x437220 when 0x6ce9bc); tutorials start, and
// a fresh single-player scenario shows its intro text (+0x5ae9 in
// multiplayer, else +0x4f30) and fires trigger 8.
RT3_NOINLINE void __fastcall World_LoadGame(uint8_t* w, int, const char* name, int32_t show) {
    auto ed = [] { return g_app; };
    auto lobby = [] { return g_newGame; };
    auto status = [&](int32_t id) {
        uint8_t* win = F<uint8_t*>(ed(), 0xc);
        Window_SendEvent5(win, 0, 0x6f, 0x84d2, (uint32_t)(uintptr_t)GetString(id), 0, 0);
        Loading_Pump(ed(), 0, 0, 1);
    };
    char* const base = (char*)0x62bee0;
    strcpy(base, name);
    if (strlen(base) > 4) base[strlen(base) - 4] = 0;
    g_62bed0 = ClockMs();
    if (uint8_t* view = g_gameWindow) View_SetToolMode(view, 0, 0, -1);
    if (show) g_skipCount = 0;
    App_SetScreen(ed(), 0, 4, 0);
    if (show) {
        Obj_Set78(F<uint8_t*>(ed(), 0xc), 0, 0x42ce0000);
        Cursor_PushBusy(g_cursorManager);
        App_Redraw(ed());
        Engine_ClearRenderLists(g_engine);
        App_Redraw(ed());
    }
    F<int32_t>(ed(), 0x4fd) = 1;
    World_Teardown(w);
    Loading_Pump(ed(), 0, 0, 0);
    World_Startup(w, 0, 0, 0);
    F<int32_t>(ed(), 0x4fd) = 1;
    if (g_6ce9c8 && CountUsedSlots6ce2d8())
        do Loading_Pump(ed(), 0, 0, 0);
        while (CountUsedSlots6ce2d8());
    Loading_Pump(ed(), 0, 0, 0);
    World_Reset(w, 0, 0);
    g_skipCount += 2;
    Loading_Pump(ed(), 0, 0, 0);
    char path[0x12c];
    {
        const char* folder = Options_SaveFolder(ed());
        FormatText(path, (const char*)0x5c9d00, folder, name, 0, 0, 0, 0, 0, 0);
    }
    Stream s = StreamOpen(path, 1, 0x30d40, 1);
    g_skipCount += 2;
    Loading_Pump(ed(), 0, 0, 0);
    StreamRead(s, w + 0x46c34, 4, 0, 1);
    Loading_Pump(ed(), 0, 0, 0);
    StreamSkip4(s, 0x2ee0, 1, 1);
    StreamRead(s, w + 0x66be, 0x3c2, 0, 1);
    StreamSkip4(s, 0x2ee1, 1, 1);
    char unpacked[0x104], image[0x100];
    unpacked[0] = 0;
    image[0] = 0;
    crt::tell(StreamFd(s));
    if (w[0x66c8]) {
        StreamClose(s);
        strcpy(image, path);
        if (w[0x66c9]) {
            Save_UnpackToTemp(path);
            FormatText(image, (const char*)0x5ca858, CopyBaseName(path, unpacked), 0, 0, 0, 0, 0, 0, 0);
        }
        LoadExtractImage(image, w + 0x66be, unpacked);
        s = StreamOpen(image, 6, 0x30d40, 1);
    } else {
        StreamClose(s);
        s = StreamOpen(path, 6, 0x30d40, 1);
    }
    StreamRead(s, w + 0x46c34, 4, 0, 1);
    StreamSkip4(s, 0x2ee0, 1, 1);
    StreamRead(s, w + 0x66be, 0x3c2, 0, 1);
    StreamSkip4(s, 0x2ee1, 1, 1);
    StreamSkip4(s, 0x32c8, 1, 1);
    StreamRead(s, w + 4, 0x4f2c, 0, 1);
    StreamSkip4(s, 0x32c9, 1, 1);
    const auto version = [&] { return F<int32_t>(w, 0x46c34); };
    if (version() <= 0x2587) {
        strcpy((char*)w + 0x4f30, (const char*)w + 0xbfe);
    } else {
        StreamRead(s, w + 0x4f30, 0xbb9, 0, 1);
        StreamSkip4(s, 0x332d, 1, 1);
    }
    if (version() <= 0x2588) {
        w[0x5ae9] = 0;
    } else {
        StreamRead(s, w + 0x5ae9, 0xbb9, 0, 1);
        StreamSkip4(s, 0x332e, 1, 1);
    }
    status(0x184);
    Collection_Load(F<Collection*>(w, 0x66b2), 0, s, 0);
    Templates_Normalize(w);
    if (Collection* bt = g_62b268) Types_RefreshAvailable(bt);
    Industries_BuildCargoLists(g_62b268);
    ((void(__fastcall*)(uint8_t*))&World_UpdateEraFactor)(w);
    World_UpdateSpeedScale(w);
    StreamSkip4(s, 0x32dc, 1, 1);
    const int32_t mode = F<int32_t>(lobby(), 1);
    if (version() >= 0x2584) {
        StreamSkip4(s, 0x3714, 1, 1);
        uint8_t saved[0x108];
        StreamRead(s, saved, 0x108, 0, 1);
        uint8_t* const l = lobby();
        if (F<int32_t>(l, 1) == 1 || F<int32_t>(l, 1) == 7) {
            l[0x97] = 0;
        } else if (l[0x97]) {
            const int32_t from = (int8_t)l[0xc3];
            const int32_t off = from - (int8_t)l[0];
            const uint32_t count = (uint32_t)((int8_t)l[0x107] - from);
            const uint8_t rank = l[0x7d];
            memcpy(l + off, saved + off, count);
            uint8_t* const l2 = lobby();
            if (l2[0xc5]) {
                l2[0x7d] = rank;
                if (!lobby()[0x82]) F<uint16_t>(lobby(), 0x77) = F<uint16_t>(w, 0x66d2);
            }
            int32_t year = F<uint16_t>(lobby(), 0x77);
            const uint64_t start = MakeDate((uint16_t)year, 1, 1, 0, 0);
            memcpy(w + 5, &start, 8);
            if (!F<int32_t>(ed(), 0x68)) {
                const int32_t opt = F<int32_t>(ed(), 0x178);
                if (opt == 1) --year;
                else if (F<int32_t>(g_world, 0x4af7)) --year;
                else if (opt == 2) year -= 3;
            }
            World_SetTime(w, 0, MinutesFromParts(year, 1, 1, 0, 0));
            Economy_UpdateEraFactors(w);
            Weather_ScheduleNext(w, 0, 0);
            memcpy(w + 0x35, w + 0xd, 8);
            if (Collection* cargo = g_cargoTypes) CargoTypes_Recount(cargo);
            if (Collection* bt = g_62b268) Industries_BuildCargoLists(bt);
            ((void(__fastcall*)(uint8_t*))&World_UpdateEraFactor)(w);
            World_UpdateSpeedScale(w);
            if (lobby()[0x82]) {
                lobby()[0xc5] = 0;
                w[0x66f2] = 1;
                w[0x66de] = 0;
            } else {
                const uint8_t campaign = lobby()[0xc5];
                w[0x66f2] = 0;
                w[0x66de] = campaign ? 1 : 0;
            }
        } else {
            int32_t year = -1;
            if (F<int32_t>(l, 1) == 3 && F<int32_t>(ed(), 0x68)) year = F<uint16_t>(l, 0x77);
            if (version() < 0x2587) NewGameConfig_Reset(l);
            else memcpy(l, saved, 0x108);
            lobby()[0x97] = 0;
            if (year != -1) {
                F<uint16_t>(lobby(), 0x77) = (uint16_t)year;
                F<int32_t>(w, 0x15) = MinutesFromParts(year, 1, 1, 0, 0);
            }
        }
        StreamSkip4(s, 0x3715, 1, 1);
    }
    F<int32_t>(lobby(), 1) = mode;
    Collection_Load(F<Collection*>(w, 0x66b6), 0, s, 0);
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(w, 0x66b6)); ++i) {
        Collection* names = F<Collection*>(w, 0x66b6);
        uint8_t* r = (uint8_t*)Collection_Get(names, 0, Collection_IdAt(names, 0, (uint32_t)i, 0));
        if (!memcmp((const void*)0x5ca84c, r, 0xb)) memcpy(r, (const void*)0x5ca840, 10);
    }
    if (Collection* engines = g_6ada84) Table_RefreshNamedFlags(engines);
    Loading_Pump(ed(), 0, 0, 0);
    StreamSkip4(s, 0x32dd, 1, 1);
    ++g_skipCount;
    World_SetTime(w, 0, F<int32_t>(w, 0x15));
    CargoTypes_Recount(g_cargoTypes);
    Loading_Pump(ed(), 0, 0, 0);
    char mapName[0x114];
    crt::sprintf(mapName, name);
    mapName[strlen(mapName) - 4] = 0;
    status(0x185);
    {
        uint8_t* m = ((uint8_t*)crt::operator_new(0x2209));
        g_map = m ? Map_ConstructLoad(m, 0, s, mapName) : nullptr;
    }
    StreamSkip4(s, 0x32ca, 1, 1);
    status(0x186);
    {
        void* cam = hook::As<void*>(g_62be68);
        ((void(__fastcall*)(void*, int, Stream))(*(void***)cam)[1])(cam, 0, s);
    }
    StreamSkip4(s, 0x32cb, 1, 1);
    auto fresh = [&] { return lobby()[0x97] || F<int32_t>(ed(), 0x68); };
    status(0x187);
    Collection_LoadTagged6979(g_cargoTypes, 0, s, 0);
    if (fresh()) CargoTypes_Reload(g_cargoTypes);
    StreamSkip4(s, 0x332b, 1, 1);
    status(0x188);
    Sites_LoadTable(s);
    if (fresh()) BtyTable_Reload(g_buildingTypes);
    StreamSkip4(s, 0x3315, 1, 1);
    status(0x189);
    Buildings_Load(g_buildings, 0, s, 0);
    StreamSkip4(s, 0x32cc, 1, 1);
    Collection_Load(g_6ada90, 0, s, 0);
    StreamSkip4(s, 0x332a, 1, 1);
    status(0x18a);
    Collection_LoadWithArrays(g_stations, 0, s, 0);
    StreamSkip4(s, 0x332b, 1, 1);
    status(0x18b);
    PartModels41aae0_Load(g_62ba84, 0, s, 0);
    StreamSkip4(s, 0x32cd, 1, 1);
    status(0x18c);
    Trains_Load(g_trains, 0, s, 0);
    StreamSkip4(s, 0x32ce, 1, 1);
    status(0x18d);
    Companies_Load(g_companies, 0, s, 0);
    StreamSkip4(s, 0x32cf, 1, 1);
    status(0x18e);
    TrackNet_Load(g_tracks, 0, s, 0);
    StreamSkip4(s, 0x32d3, 1, 1);
    status(0x18f);
    Routes_Load(g_routes, 0, s, 0x24);
    StreamSkip4(s, 0x32d0, 1, 1);
    Buildings_RestoreQueued(g_buildings);
    status(0x190);
    {
        uint8_t* map = g_map;
        for (int32_t y = 0; y < F<int32_t>(map, 0x2161); ++y) {
            for (int32_t x = 0; x < F<int32_t>(map, 0x215d); ++x) {
                if (Map_CellFlag(map, 0, x, y, 0) == 1) Terrain_SetFlag(g_map, 0, x >> 2, y >> 2, 0xf);
                map = g_map;
            }
        }
    }
    Tracks_RebuildGeometry(g_tracks);
    status(0x191);
    Players_Load(g_players, 0, s, 0);
    StreamSkip4(s, 0x32d1, 1, 1);
    status(0x192);
    NamedObjs4611e0_Load(g_6ada80, 0, s, 0);
    StreamSkip4(s, 0x332c, 1, 1);
    Names_ApplyRegionVariants(g_6ada80, 0, (char*)name);
    status(0x193);
    NamedObjs474110_Load(g_6cea50, 0, s, 0);
    StreamSkip4(s, 0x332d, 1, 1);
    status(0x194);
    NamedObjs41f590_Load(g_cities, 0, s, 0);
    StreamSkip4(s, 0x32d2, 1, 1);
    Loading_Pump(ed(), 0, 0, 1);
    Stations_Load(g_territories, 0, s, 0);
    StreamSkip4(s, 0x32d3, 1, 1);
    status(0x195);
    NamedObjs456900_Load(g_62b244, 0, s, 0);
    StreamSkip4(s, 0x32dc, 1, 1);
    status(0x196);
    RuleSets_Load(g_events, 0, s, 0);
    StreamSkip4(s, 0x32e6, 1, 1);
    StreamRead(s, w + 0x19, 4, 0, 1);
    if (hook::As<int32_t>(g_6d40d0)) F<int32_t>(w, 0x19) = 0;
    if (uint8_t* view = g_gameWindow) Window_SendEvent5(view, 0, 0x74, F<int32_t>(w, 0x19) + 0x80c, 1, 0, 0);
    Loading_Pump(ed(), 0, 0, 0);
    {
        uint8_t tail[0x1f4];
        StreamRead(s, tail, 0x1f4, 0, 1);
    }
    StreamSkip4(s, 0x32cf, 1, 1);
    StreamClose(s);
    Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x808, 0, 0, 0);
    {
        uint8_t* map = g_map;
        if ((!F<int32_t>(map, 0x21a5) && !g_engine[0x1146eb]) || version() <= 0x2584) Map_RebuildCoast(map, 0, 0, 0, 0x270f, 0x270f, nullptr);
    }
    if (unpacked[0]) crt::remove(unpacked);
    if (w[0x66c9]) crt::remove(image);
    status(0x197);
    Weather_ApplyIntensity(w);
    Territories_BuildBorders(g_territories);
    Templates_Normalize(w);
    CargoTypes_MarkAvailable(g_cargoTypes);
    Types_RefreshAvailable(g_62b268);
    MarkKind2Buildings();
    ++g_skipCount;
    Loading_Pump(ed(), 0, 0, 0);
    World_SetTime(w, 0, F<int32_t>(w, 0x15));
    Cities_LayStreets(g_cities);
    Map_MarkUntypedRegions(g_map);
    Map_UpdateCargoFlags(g_map);
    Lots_RebuildDemand(g_6ada90);
    Terrain_RefreshPointGrids(g_map, 0, 0, 0, 0x270f, 0x270f);
    Scenario_ApplyFixes(w, 0, name);
    if (lobby()[0x97]) World_PrepareNewGame(w);
    else if (g_6ce9bc) World_SetupPlayers(w);
    for (int32_t i = 0; i < Collection_Count(g_cities); ++i) {
        Collection* cities = g_cities;
        Obj_SumBuildingWeights(Collection_Get(cities, 0, Collection_IdAt(cities, 0, (uint32_t)i, 0)));
    }
    if (lobby()[0x97]) Camera_RecallBookmark(g_62be68, 0, 1);
    if (show) {
        Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1);
        Cursor_PopBusy(g_cursorManager);
    }
    Loading_Pump(ed(), 0, 0, 1);
    g_622ae4 = -1;
    g_622b2c = -1;
    FreeBuffer62b2f8();
    Map_FeatureBounds(g_map);
    App_SetScreen(ed(), 0, 1, 0);
    if (F<int32_t>(w, 0x45)) World_FollowEntity(w, 0, 1, F<int32_t>(w, 0x45), F<int32_t>(w, 0x49));
    ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x3748] = 1;
    ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x3747] = 1;
    F<int32_t>(ed(), 0x4fd) = 0;
    F<int32_t>(ed(), 0x6c) = 0;
    Companies_BuildStationLinks(g_companies);
    if (F<int32_t>(lobby(), 1) == 1 || F<int32_t>(lobby(), 1) == 7) {
        Tutorial_Start();
    } else if (!Global6d07b4Set2() && !lobby()[0x82] && lobby()[0x97]) {
        TrimSpaces((char*)w + 0x5ae9, 0);
        TrimSpaces((char*)w + 0x4f30, 0);
        if (IsMultiplayer() && strlen((const char*)w + 0x5ae9) > 1)
            World_AddNamedEntry(w, 0, (const char*)w + 0x5ae9, 0, 0, -1, -1, -1, -1);
        else if (strlen((const char*)w + 0x4f30) > 1)
            World_AddNamedEntry(w, 0, (const char*)w + 0x4f30, 0, 0, -1, -1, -1, -1);
    }
    if (lobby()[0x97]) Events_Fire(g_events, 0, 8, nullptr, 0);
    lobby()[0x97] = 0;
}
RT3_REPLACE(0x443a50, World_LoadGame);

}  // namespace rt3
