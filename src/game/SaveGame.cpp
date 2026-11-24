// SaveGame.cpp - RT3.exe 0x444000-0x448000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <math.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "game/calendar.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include "core/containers.h"

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count);
void __fastcall Map_SetCellLow(uint8_t* m, int, int32_t x, int32_t y, uint8_t lo);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

int32_t Global6d40e4();
const char* __fastcall Options_SaveFolder(void* opt);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Grid_CellMarked(uint8_t* g, int, int32_t x, int32_t y);
void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Territories_Recount(Collection* c);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t ClockMs();
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Cities_AssignBlocks(Collection* cities);
void __fastcall Terrain_RebuildWaterMask(uint8_t* t);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
int32_t Global6d07b4Set2();
int32_t IsMultiplayer();
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall StreamClose(int32_t s);
int32_t __fastcall Options_Check1bf(void* self);
int32_t __fastcall File_Exists(const char* path);
void __fastcall Obj_NumberPorts(void* o, int, int32_t n);
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
void __fastcall Table_RefreshNamedFlags(Collection* t);
int32_t __fastcall FileExists(const char* path);
void* __fastcall Obj_Ref72Plus45(void* o);
void AppendOptionHistory();
void __fastcall InfoWindow_ClearHighlights(uint8_t* w);
void __fastcall Obj_UnlinkFromNode(void* o);
int32_t __fastcall StreamSkip4(int32_t s, int tag, int32_t, int32_t count);
void __fastcall Terrain_MarkFlooded(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_MarkDrained(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
uint32_t __fastcall Cities_Add(Collection* t, int, uint32_t x, uint32_t y, const char* name, int32_t rural);
void __fastcall Ambient_Start(void*);
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
void* __fastcall Table_GetByName(Collection* t, int, const char* name);
void __fastcall Obj_AssignRandomSlots(uint8_t* o);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall Cursor_PopBusy(uint8_t* c);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
void __fastcall Terrain_LoadLayers(uint8_t* r, int, int32_t s);
void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Cities_AddTestRow(Collection* t);
void __fastcall Territories_AddTestSet(Collection* t);
void PopOptionHistory();
int32_t __fastcall BuildingDialog_Retrack(uint8_t* dlg);
void Game_PauseTools();
void __fastcall Cursor_PushBusy(uint8_t* c);
void __fastcall App_Redraw(uint8_t* a);
void __fastcall Map_WritePreview(uint8_t* m, int, uint8_t* dst);
void __fastcall Terrain_SaveLayers(uint8_t* r, int, Stream s);
uint8_t __fastcall Grid_Byte1661(uint8_t* g, int, int32_t x, int32_t y);
uint8_t __fastcall Grid_Byte1665(uint8_t* g, int, int32_t x, int32_t y);
int32_t __fastcall Map_CellHigh(uint8_t* m, int, int32_t x, int32_t y);
uint32_t __fastcall Map_CellLow(uint8_t* m, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall RuleSets_Save(Collection* c, int, Stream st, int32_t recordSize);
void __fastcall StreamWriteU32(Stream s, uint32_t value);
void __fastcall StreamClose(Stream s);
int32_t __cdecl AnyHasFixedName();
void __fastcall TrackTool_Suspend(uint8_t* w);
void __fastcall World_MarkBuildingTypes(uint8_t* w);
void __fastcall Save_LogProgress(Stream s, const char* label);
const char* __stdcall Save_ProgressText(int32_t total, float done);
void __fastcall Collection_Save(Collection* c, int, Stream s, int32_t recordSize);
void __fastcall Map_Save(uint8_t* w, int, Stream s, const char* name);
void __fastcall Collection_SaveTagged6979(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Sites_SaveTable(uint8_t* c, int, Stream s);
void __fastcall Buildings_Save(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Sites_Save(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Collection_SaveWithRecords(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Trains_Save(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Companies_Save(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall ContainerList_Save(Collection* c, int, Stream s, int32_t recordSize);
void __fastcall Routes_Save(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Players_Save(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Collection_SaveWithRecords5209b(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Collection_SaveWithRecords61a9(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Collection_SaveWithRecords5209(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Stations_Save(Collection* t, int, Stream s, int32_t recordSize);
void __fastcall Collection_SaveWithRecords2af9(Collection* t, int, Stream s, int32_t recordSize);
void __stdcall Preview_SaveJpeg(const char* path, uint8_t* o);
void __fastcall NameTable_Rescan(uint8_t* t);
int32_t Global6d0810Set();
uint32_t __fastcall FileDialog_Run(char* name, int32_t kind);
void __fastcall StripExtension(char* s);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void __fastcall Obj_UnlinkFromNode(void* o);
RT3_NOINLINE void __fastcall Car_LinkToNode(uint8_t* car, int, int32_t id, int32_t side);
RT3_NOINLINE void __fastcall Track_AddWear(uint8_t* t);
RT3_NOINLINE void __fastcall Track_SetBlocked(uint8_t* o, int, uint8_t v);
int32_t __fastcall Save_LoadOptions(const char* name);
RT3_NOINLINE void __fastcall Cursor_PopBusy(uint8_t* c);
RT3_NOINLINE void* __fastcall Obj_Ref72Plus45(void* o);
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
uint8_t* __fastcall Map_Construct(uint8_t* m, int, int32_t w, int32_t h, const char* tex);
RT3_NOINLINE void __fastcall Terrain_BuildTiles(uint8_t* t, int, int32_t w, int32_t h, uint8_t* src);
RT3_NOINLINE void __fastcall Ambient_Start(void*);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
void __fastcall World_Teardown(uint8_t* w);
RT3_NOINLINE void __fastcall Templates_Normalize(uint8_t* self);
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t);
int32_t __fastcall RuleSets_Load(Collection* c, int, Stream st, int32_t recordSize);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
void __fastcall Cities_LayStreets(Collection* cities);
void __fastcall Map_MarkUntypedRegions(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Grid_SetByte1661(uint8_t* g, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Grid_SetByte1665(uint8_t* g, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Grid_MarkCell(uint8_t* g, int, int32_t x, int32_t y);
void __fastcall Map_FloodCell(uint8_t* m, int, int32_t x, int32_t y, int32_t);
RT3_NOINLINE uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n);
void __fastcall Map_SetCellHigh(uint8_t* m, int, int32_t x, int32_t y, uint8_t hi);
RT3_NOINLINE void __fastcall Terrain_LoadLayers(uint8_t* r, int, Stream s);
RT3_NOINLINE void __fastcall Obj_AssignRandomSlots(uint8_t* o);
RT3_NOINLINE uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
RT3_NOINLINE void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
RT3_NOINLINE void __fastcall Territories_AddTestSet(Collection* t);
uint32_t __fastcall StationTypes_Add(Collection* t, int, const char* name);
RT3_NOINLINE void __fastcall Territories_Recount(Collection* c);
RT3_NOINLINE void __fastcall Cities_AddTestRow(Collection* t);
RT3_NOINLINE void* __fastcall Table_GetByName(Collection* t, int, const char* name);
RT3_NOINLINE uint32_t __fastcall Cities_Add(Collection* t, int, uint32_t x, uint32_t y, const char* name, int32_t rural);
void __fastcall Obj_UpdatePortFlags(void* o);
RT3_NOINLINE uint32_t __fastcall Markers_Add(Collection* t, int, uint32_t x, uint32_t y, const char* label, int32_t bodied);
RT3_NOINLINE void __fastcall Terrain_RebuildWaterMask(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Cities_AssignBlocks(Collection* cities);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall World_SaveGame(uint8_t* w, int, const char* name, uint32_t flags);
void __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t);
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Types_RefreshAvailable(Collection* c);
RT3_NOINLINE void __fastcall Cities_AddTestRow(Collection* t);
RT3_NOINLINE void __fastcall World_Teardown(uint8_t* w);
RT3_NOINLINE void __fastcall World_SetNamedFlag(uint8_t* w, int, const char* name, int32_t value);
RT3_NOINLINE void __fastcall World_Startup(uint8_t* w, int, int32_t setup, int32_t withMap);
RT3_NOINLINE void Game_PauseTools();
RT3_NOINLINE void Options_Restore();
RT3_NOINLINE void __fastcall Map_WritePreview(uint8_t* m, int, uint8_t* dst);
RT3_NOINLINE void __fastcall World_SaveGame(uint8_t* w, int, const char* name, uint32_t flags);
void __fastcall Car_SetPiece(uint8_t* car, int, int32_t piece, int32_t pos);
RT3_NOINLINE int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall World_ExportMap(uint8_t* w, int, const char* name);
RT3_NOINLINE void __fastcall World_LoadMap(uint8_t* w, int, const char* name);
RT3_NOINLINE uint8_t* __fastcall Map_Construct(uint8_t* m, int, int32_t w, int32_t h, const char* tex);
RT3_NOINLINE void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Map_SetCellHeight(uint8_t* m, int, int32_t x, int32_t y, uint32_t v, int32_t refresh);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
RT3_NOINLINE void __fastcall Table_RefreshNamedFlags(Collection* t);
RT3_NOINLINE void __fastcall App_Redraw(uint8_t* a);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE void __fastcall Territories_AddTestSet(Collection* t);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE uint32_t __fastcall FileDialog_Run(char* name, int32_t kind);
RT3_NOINLINE void __fastcall InfoWindow_ClearHighlights(uint8_t* w);
RT3_NOINLINE int32_t __fastcall BuildingDialog_Retrack(uint8_t* dlg);
RT3_NOINLINE void __fastcall TrackTool_Suspend(uint8_t* w);
RT3_NOINLINE void __fastcall TrackTool_Resume(uint8_t* t);

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

namespace {
// 0x441ec0 takes the file in ESI, the block in EBX and the stream in EDI.
void SaveHeaderBlock(void* file, uint8_t* block, Stream s) {
    __asm {
        push ebx
        push esi
        push edi
        mov esi, file
        mov ebx, block
        mov edi, s
        mov eax, 0x441ec0
        call eax
        pop edi
        pop esi
        pop ebx
    }
}
// 0x441f70 / 0x442900 take the block in EAX and the path on the stack.
void BlockCall(uint32_t fn, uint8_t* block, const char* path) {
    __asm {
        push path
        mov eax, block
        mov ecx, fn
        call ecx
    }
}
}

namespace {
// strcat of the five bytes (extension and terminator) at a.
void AppendExtension(char* name, uint32_t a) { memcpy(name + strlen(name), (const void*)a, 5); }
}

namespace {
// The AI route builder's stack frame, kept as in the original: several slots
// change meaning on the way (see the QUIRK below).
struct RouteFrame {
    alignas(8) uint8_t b[0x1f0];
    int32_t& i(uint32_t o) { return *(int32_t*)(b + o); }
    float& f(uint32_t o) { return *(float*)(b + o); }
    double& d(uint32_t o) { return *(double*)(b + o); }
    uint8_t*& p(uint32_t o) { return *(uint8_t**)(b + o); }
};
inline double RtSin(double x) { return ((double(__cdecl*)(double))0x5a1494)(x); }
inline double RtCos(double x) { return ((double(__cdecl*)(double))0x5a13e4)(x); }
inline double RtAbs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }
inline double RtMod(double x, double y) { return ((double(__cdecl*)(double, double))0x5a1390)(x, y); }
inline int RtStricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
inline float RtBits(uint32_t v) { return *(const float*)&v; }
inline uint8_t* RtGet(uint32_t global, int32_t id) { return (uint8_t*)Collection_Get(hook::Global<Collection*>(global), 0, (uint32_t)id); }
inline uint8_t* RtMap() { return g_map; }
}

namespace {
// Empties collection c through its virtual remove (slot 2).
template <typename Get> void Map_ClearCollection(Get c) {
    while (Collection_Count(c())) {
        Collection* t = c();
        auto remove = (void(__fastcall*)(Collection*, int, uint32_t))(*(void***)t)[2];
        const uint32_t id = Collection_IdAt(t, 0, 0, 0);
        remove(c(), 0, id);
    }
}
// Adds a record through the collection's virtual add (slot 1).
void Map_AddRecord(Collection* c, void* rec) { ((void(__fastcall*)(Collection*, int, void*, int32_t, int32_t, int32_t))(*(void***)c)[1])(c, 0, rec, 0, 0, 0); }
}

 // namespace

// 0x444dd0 (thiscall, RET 8). World w saves the game as `name` in the save
// folder with the cursor and the tools parked (format 0x2649 when a fixed
// name is in use, else 0x25e5). Unless `flags` bit 1 is set, a file that
// cannot be created reports 0xfda. The file holds the header block (+0x66be
// with the preview), the world record (+4, +0x4f30, +0x5ae9), the name
// tables, lobby, map (with its "_A.tga" picture), view and every table, each
// section tagged and logged, the progress shown in 0x15 (+1 for an image,
// +0x15 to pack) steps; then the preview JPEG, the image appended (+0x66c8,
// then packed with +0x66c9) or the header rewritten in place.
RT3_NOINLINE void __fastcall World_SaveGame(uint8_t* w, int, const char* name, uint32_t flags) {
    hook::As<int32_t>(g_620e94) = AnyHasFixedName() ? 0x2649 : 0x25e5;
    g_62bec8 = 0x15;
    g_62becc = 0;
    g_62bed0 = ClockMs();
    const int32_t cursor = F<int32_t>(g_cursorManager, 4);
    Cursor_Set(g_cursorManager, 0, 0, 0, 0);
    AppendOptionHistory();
    if (uint8_t* tool = g_6d1a8c) TrackTool_Suspend(tool);
    if (uint8_t* dlg = g_6d1720) InfoWindow_ClearHighlights(dlg);
    auto restoreUi = [&] {
        PopOptionHistory();
        if (uint8_t* dlg = g_6d1720) BuildingDialog_Retrack(dlg);
        if (uint8_t* tool = g_6d1a8c) TrackTool_Resume(tool);
        Cursor_Set(g_cursorManager, 0, cursor, 0, 0);
        Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1);
        g_62bec8 = 0;
    };
    char path[0x130];
    {
        const char* folder = Options_SaveFolder(g_app);
        FormatText(path, (const char*)0x5c9d00, folder, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    const Stream s = StreamOpen(path, 4, 200000, 1);
    if (s == -1) {
        if (!(flags & 2))
            Dialog_RunMessage(GetString(0xfda), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        restoreUi();
        return;
    }
    if (F<int32_t>(g_app, 0x68)) World_MarkBuildingTypes(w);
    uint8_t* const block = w + 0x66be;
    Map_WritePreview(w, 0, block);
    SaveHeaderBlock(nullptr, block, s);
    Save_LogProgress(s, (const char*)0x5ca998);
    if (w[0x66c8]) ++g_62bec8;
    if (w[0x66c9]) g_62bec8 += 0x15;
    auto step = [] {
        const float done = (float)++g_62becc;
        Ui_ShowTooltip(g_windowManager, 0, Save_ProgressText(g_62bec8, done), 3, 0, -1);
        App_Redraw(g_app);
    };
    auto tag = [s](uint32_t v) { StreamWriteU32(s, v); };
    auto log = [s](uint32_t label) { Save_LogProgress(s, (const char*)label); };
    tag(0x32c8);
    StreamWrite(s, w + 4, 0x4f2c, 0);
    tag(0x32c9);
    StreamWrite(s, w + 0x4f30, 0xbb9, 0);
    tag(0x332d);
    StreamWrite(s, w + 0x5ae9, 0xbb9, 0);
    tag(0x332e);
    step();
    Loading_Pump(g_app, 0, 0, 0);
    Collection_Save(F<Collection*>(w, 0x66b2), 0, s, 0);
    step();
    log(0x5ca978);
    tag(0x32dc);
    tag(0x3714);
    StreamWrite(s, g_newGame, 0x108, 0);
    tag(0x3715);
    Collection_Save(F<Collection*>(w, 0x66b6), 0, s, 0);
    tag(0x32dd);
    char picture[0x130];
    {
        const char* folder = Options_SaveFolder(g_app);
        FormatText(picture, (const char*)0x5c9d00, folder, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        // The extension (last four characters) becomes "_A.tga".
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(picture + strlen(picture) - 4, (const char*)0x5ca320);
    }
    Map_Save(g_map, 0, s, picture);
    log(0x5ca96c);
    tag(0x32ca);
    step();
    {
        uint8_t* view = g_62be68;
        ((void(__fastcall*)(uint8_t*, int, Stream))(*(void***)view)[8 / 4])(view, 0, s);
    }
    log(0x5ca95c);
    tag(0x32cb);
    step();
    Collection_SaveTagged6979(g_cargoTypes, 0, s, 0);
    tag(0x332b);
    log(0x5ca94c);
    step();
    Sites_SaveTable(hook::As<uint8_t*>(g_buildingTypes), 0, s);
    tag(0x3315);
    log(0x5ca938);
    step();
    Buildings_Save(g_buildings, 0, s, 0);
    log(0x5ca928);
    tag(0x32cc);
    step();
    Collection_Save(g_6ada90, 0, s, 0);
    log(0x5ca918);
    tag(0x332a);
    step();
    Sites_Save(g_stations, 0, s, 0);
    log(0x5ca908);
    tag(0x332b);
    step();
    Collection_SaveWithRecords(g_62ba84, 0, s, 0);
    log(0x5ca8fc);
    tag(0x32cd);
    step();
    Trains_Save(g_trains, 0, s, 0);
    log(0x5ca8f0);
    tag(0x32ce);
    step();
    Companies_Save(g_companies, 0, s, 0);
    log(0x5ca8e0);
    tag(0x32cf);
    step();
    ContainerList_Save(g_tracks, 0, s, 0);
    log(0x5ca8d4);
    tag(0x32d3);
    step();
    Routes_Save(g_routes, 0, s, 0x24);
    log(0x5ca8c4);
    tag(0x32d0);
    step();
    Players_Save(g_players, 0, s, 0);
    log(0x5ca8b8);
    tag(0x32d1);
    step();
    Collection_SaveWithRecords5209b(g_6ada80, 0, s, 0);
    log(0x5ca8ac);
    tag(0x332c);
    step();
    Collection_SaveWithRecords61a9(g_6cea50, 0, s, 0);
    log(0x5ca898);
    tag(0x332d);
    step();
    Collection_SaveWithRecords5209(g_cities, 0, s, 0);
    log(0x5ca88c);
    tag(0x32d2);
    step();
    Stations_Save(g_territories, 0, s, 0);
    log(0x5ca87c);
    tag(0x32d3);
    Collection_SaveWithRecords2af9(g_62b244, 0, s, 0);
    log(0x5ca86c);
    tag(0x32dc);
    step();
    RuleSets_Save(g_events, 0, s, 0);
    log(0x5ca860);
    tag(0x32e6);
    step();
    StreamWrite(s, w + 0x19, 4, 0);
    {
        uint8_t pad[0x1f4] = {};
        StreamWrite(s, pad, 0x1f4, 0);
    }
    tag(0x32cf);
    StreamClose(s);
    Preview_SaveJpeg(path, block);
    step();
    if (w[0x66c8]) {
        BlockCall(0x441f70, block, path);
        step();
        if (w[0x66c9]) {
            BlockCall(0x442900, block, path);
            step();
        }
    } else {
        void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5ca330);
        SaveHeaderBlock(f, block, 0);
        ((int(__cdecl*)(void*))0x5a1740)(f);
    }
    NameTable_Rescan(g_pack);
    restoreUi();
}
RT3_REPLACE(0x444dd0, World_SaveGame);

// 0x41c700 (RET 8). Car car moves onto track piece `piece` (-1: off the
// track) at position `pos` (+0x35e): it is unlinked and relinked on its side
// (+0x35a); entering a new piece wears it and blocks it.
void __fastcall Car_SetPiece(uint8_t* car, int, int32_t piece, int32_t pos) {
    uint8_t* const p = piece != -1 ? (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)piece) : nullptr;
    Obj_UnlinkFromNode(car);
    Car_LinkToNode(car, 0, piece, F<int32_t>(car, 0x35a));
    if (F<int32_t>(car, 0x356) != piece) {
        F<int32_t>(car, 0x356) = piece;
        if (piece != -1) {
            Track_AddWear(p);
            Track_SetBlocked(p, 0, 1);
        }
    }
    F<int32_t>(car, 0x35e) = pos;
}

// 0x445ac0 (RET 0xc). Loads a game: `name` (formatted), else with flag 1
// the Quicksave (extension by mode: .gmp editor, .gmx auto companies, .gmc
// campaign, else .gms; confirmed by 0x181 when it exists, else 0x182 says
// there is none), else a file picked in the load dialog (kind 4 paused, 9
// campaign, 10 auto companies, else 8; a saved options file loads them and
// opens the options event 0xcc / 7 instead). A map (option 0x1bf or flag 8)
// loads as .smp (0x446d40, *done = 1), anything else as a game (0x443a50,
// flag 4: keep the view). Loading holds 0x62be6c, then the screen refreshes
// (0x62be68), the editor's +0x501 is set and 0x6d4018 +0xa8c cleared.
// Returns 1 when loaded (or the options opened), else 0.
RT3_NOINLINE int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done) {
    if (done) *done = 0;
    char file[0x12c];
    if (name) {
        FormatText(file, name, 0, 0, 0, 0, 0, 0, 0, 0);
    } else if (flags & 1) {
        memcpy(file, (const void*)0x5ca9cc, 10);
        uint8_t* ed = g_app;
        uint8_t* lobby = g_newGame;
        const uint32_t ext = F<int32_t>(ed, 0x68) ? 0x5ca9c4 : lobby[0x82] ? 0x5ca9bc : lobby[0xc5] ? 0x5ca9b4 : 0x5ca9ac;
        memcpy(file + strlen(file), (const void*)ext, 5);
        char path[0x11c];
        const char* folder = Options_SaveFolder(g_app);
        FormatText(path, (const char*)0x5c9d00, folder, file, 0, 0, 0, 0, 0, 0);
        if (!FileExists(path)) {
            Dialog_RunMessage(GetString(0x182), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            return 0;
        }
        if (Dialog_RunMessage(GetString(0x181), 2, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0) != 0x3f2) return 0;
    } else {
        int32_t kind;
        if (Global6d07b4Set2()) kind = 4;
        else if (g_newGame[0xc5]) kind = 9;
        else kind = g_newGame[0x82] ? 10 : 8;
        if (Global6d0810Set() || !FileDialog_Run(file, kind)) return 0;
        if (Save_LoadOptions(file)) {
            Game_PostInputEvent(0xcc, 0, 7, 0, 0);
            return 1;
        }
    }
    ++g_62be6c;
    if (Options_Check1bf(g_app) || (flags & 8)) {
        StripExtension(file);
        memcpy(file + strlen(file), (const void*)0x5ca9a4, 5);
        ((void(__fastcall*)(uint8_t*, int, char*))0x446d40)(g_world, 0, file);
        if (done) *done = 1;
    } else {
        ((void(__fastcall*)(uint8_t*, int, char*, uint32_t))0x443a50)(g_world, 0, file, ~(flags >> 2) & 1);
    }
    --g_62be6c;
    {
        void* screen = hook::As<void*>(g_62be68);
        ((void(__fastcall*)(void*))(*(void***)screen)[0])(screen);
    }
    F<int32_t>(g_app, 0x501) = 1;
    F<int32_t>(g_input, 0xa8c) = 0;
    return 1;
}
RT3_REPLACE(0x445ac0, World_Load);

 // namespace

// 0x445de0 (thiscall, RET 0xc). World w saves: under `name`, as the
// quicksave (flags bit 0, 0x5ca9cc), as the autosave (bit 1, 0x183, no
// thumbnail id) or by the file dialog (unless one is open, 0x6d0810) with
// the kind of game (editor map 3, auto companies 2, scenario 1,
// multiplayer 11). The editor's map export (0x4839b0) goes to .smp
// (World_ExportMap, *done set). Otherwise unnamed saves get the kind's
// extension (.gmp / .gmx / .gmc / .gms); a picked file that exists is
// confirmed (0xf4d, with the overwrite check 0x6d080c). Multiplayer asks
// the host (message 0x7d: id, flags, name), else World_SaveGame. In the
// editor a map with towns (+0x66de) whose name is none of the sixteen
// standard maps (0x621cf0, "%s.gmp") warns 0xfd9.
RT3_NOINLINE void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done) {
    if (done) *done = 0;
    g_62bec4 = Options_Check1bf(g_app) ? F<int32_t>(g_engine, 0x11472a)
                                                                                         : F<int32_t>(g_engine, 0x11472e);
    char file[0x12c];
    int32_t picked = 0, plain = 1;
    if (name) {
        plain = 0;
        FormatText(file, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    } else if (flags & 1) {
        FormatText(file, (const char*)0x5ca9cc, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    } else if (flags & 2) {
        g_62bec4 = 0;
        FormatText(file, GetString(0x183), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    } else {
        if (Global6d0810Set()) return;
        plain = 0;
        int32_t kind;
        if (F<int32_t>(g_app, 0x68)) kind = 3;
        else if (g_newGame[0x82]) kind = 2;
        else if (g_newGame[0xc5]) kind = 1;
        else kind = IsMultiplayer() ? 0xb : 0;
        if (!FileDialog_Run(file, kind)) return;
        picked = 1;
    }
    if (Options_Check1bf(g_app)) {
        StripExtension(file);
        AppendExtension(file, 0x5ca9a4);
        World_ExportMap(w, 0, file);
        if (done) *done = 1;
    } else {
        if (plain) {
            if (F<int32_t>(g_app, 0x68)) AppendExtension(file, 0x5ca9c4);
            else if (g_newGame[0x82]) AppendExtension(file, 0x5ca9bc);
            else if (g_newGame[0xc5]) AppendExtension(file, 0x5ca9b4);
            else AppendExtension(file, 0x5ca9ac);
        }
        if (g_6d080c && picked) {
            char path[0x12c];
            const char* folder = Options_SaveFolder(g_app);
            FormatText(path, (const char*)0x5c9d00, folder, file, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            if (File_Exists(path)) {
                char base[0x408], text[0x3bc];
                strcpy(base, file);
                StripExtension(base);
                const char* fmt = GetString(0xf4d);
                FormatText(text, fmt, base, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                if (Dialog_RunMessage(text, 3, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0) != 0x3f2) return;
            }
        }
        if (IsMultiplayer()) {
            uint32_t msg[0x12] = {};
            msg[0] = (uint32_t)Global6d40e4();
            msg[1] = flags;
            strcpy((char*)&msg[2], file);
            uint8_t* sess = g_session;
            Net_Send(sess, 0, (const char*)(sess + 0x8f48), 0x7d, 0x48, (const uint8_t*)msg, 5, 0);
        } else {
            World_SaveGame(w, 0, file, flags);
        }
    }
    if (F<int32_t>(g_app, 0x68) && g_world[0x66de]) {
        bool standard = false;
        for (uint32_t p = 0x621cf0; p < 0x621d30; p += 4) {
            char std_[0x408];
            ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(std_, (const char*)0x5ca9d8, *(const char**)p);
            if (!((int(__cdecl*)(const char*, const char*))0x5a57cf)(file, std_)) standard = true;
        }
        if (!standard) Dialog_RunMessage(GetString(0xfd9), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    }
}
RT3_REPLACE(0x445de0, World_Save);

// 0x446240 (thiscall, RET 4). World w exports its map as `name` in the save
// folder (format 0x400) with the tools paused and the cursor busy: the
// preview (chunk 0x2ee0..0x2ee1), the size, terrain layers, the height of
// every grid point, per cell the kind flags, the two grid bytes, the mark
// and five bits of the cell record, the twelve region records (+0xfe7,
// 0x4e1 each), the terrain texture, the cell shades, the territories, the
// two name tables (+0x66b2 / +0x66b6), the build options (+0xbe2..), the
// cities (name, cell, kind, size, industry plan), the scenario text
// (+0x4f30), the map labels, 49 ints at +0x4a7f, +0x4b43, the water mask,
// the overlay cells, per cell its high / low type and the count, the rule
// sets (chunk 0x4e21..0x4e22) and +0x4c78 / +0x4c7c.
// QUIRK: a label is written as 41 bytes of a buffer that only its own name
// fills, so longer earlier names (or, the first time, stack residue) follow
// it in the file; here the buffer starts zeroed. For a cell whose high
// type has a table entry (0x5ef3e4), the "low" byte written is the one left
// from the last cell that had none (or from the cell flags pass).
RT3_NOINLINE void __fastcall World_ExportMap(uint8_t* w, int, const char* name) {
    int32_t version = 0x400;
    if (uint8_t* v = g_gameWindow) View_SetToolMode(v, 0, 0, -1);
    Cursor_PushBusy(g_cursorManager);
    Game_PauseTools();
    Ui_ShowTooltip(g_windowManager, 0, GetString(0x199), 3, 0, -1);
    App_Redraw(g_app);
    char path[0x200];
    {
        const char* folder = Options_SaveFolder(g_app);
        FormatText(path, (const char*)0x5c9d00, folder, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    const Stream s = StreamOpen(path, 4, 200000, 1);
    StreamWrite(s, &version, 4, 0);
    if (version >= 0x3f1) {
        Map_WritePreview(w, 0, w + 0x66be);
        StreamWriteU32(s, 0x2ee0);
        StreamWrite(s, w + 0x66be, 0x403c2, 0);
        StreamWriteU32(s, 0x2ee1);
    }
    uint8_t* const map = g_map;
    int32_t width = F<int32_t>(map, 0x2155), height = F<int32_t>(map, 0x2159);
    StreamWrite(s, &width, 4, 0);
    StreamWrite(s, &height, 4, 0);
    Terrain_SaveLayers(g_map, 0, s);
    for (int32_t y = 0; y < height + 1; ++y)
        for (int32_t x = 0; x < width + 1; ++x) {
            float h = (float)Map_GridFloat(g_map, 0, x, y);
            StreamWrite(s, &h, 4, 0);
        }
    uint8_t lowByte = 0; // shared by the cell passes, see the QUIRK
    auto put8 = [&](uint8_t v) { StreamWrite(s, &v, 1, 0); };
    for (int32_t y = 0; y < height; ++y)
        for (int32_t x = 0; x < width; ++x) {
            auto m = [] { return g_map; };
            put8((uint8_t)Terrain_KindIs1Or4(m(), 0, x, y));
            put8((uint8_t)Terrain_KindIs2Or4Or5(m(), 0, x, y));
            put8((uint8_t)Terrain_KindIs3Or5(m(), 0, x, y));
            put8(Grid_Byte1661(m(), 0, x, y));
            put8(Grid_Byte1665(m(), 0, x, y));
            put8((uint8_t)Grid_CellMarked(m(), 0, x, y));
            put8((Terrain_Rec(m(), 0, x, y)[2] >> 5) & 1);
            put8((uint8_t)((F<uint16_t>(Terrain_Rec(m(), 0, x, y), 0) >> 6) & 7));
            put8((Terrain_Rec(m(), 0, x, y)[0] >> 3) & 7);
            lowByte = (uint8_t)((F<uint16_t>(Terrain_Rec(m(), 0, x, y), 0) >> 0xb) & 1);
            put8(lowByte);
            put8((uint8_t)((F<uint16_t>(Terrain_Rec(m(), 0, x, y), 0) >> 0xa) & 1));
        }
    for (int32_t r = 0; r < 12; ++r) {
        uint8_t* reg = w + 0xfe7 + r * 0x4e1;
        StreamWrite(s, reg + 0x3ed, 4, 0);
        StreamWrite(s, reg, 0x3d, 0);
        for (int32_t k = 0; k < 5; ++k) {
            uint8_t* p = reg + 0x3f5 + k * 0x30;
            StreamWrite(s, p - 4, 4, 0);
            StreamWrite(s, p, 4, 0);
            StreamWrite(s, p + 4, 0x14, 0);
            StreamWrite(s, p + 0x18, 0x14, 0);
        }
    }
    {
        int32_t tw, th;
        uint32_t* pixels = Terrain_ExtractTexture(g_map, 0, 0.0f, 0.0f, 1.0f, 1.0f, &tw, &th);
        StreamWrite(s, &tw, 4, 0);
        StreamWrite(s, &th, 4, 0);
        StreamWrite(s, pixels, (uint32_t)(tw * th * 4), 0);
        Terrain_CommitTexture(g_map, 0, 1);
    }
    for (int32_t y = 0; y < height; ++y)
        for (int32_t x = 0; x < width; ++x) {
            uint8_t* m = g_map;
            put8(F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * y + x] >> 3);
        }
    // A table's records written as fixed fields.
    auto table = [&](Collection* c, auto&& each) {
        int32_t n = Collection_Count(c);
        StreamWrite(s, &n, 4, 0);
        for (int32_t i = 0; i < n; ++i) each((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0)));
    };
    table(g_territories, [&](uint8_t* t) {
        StreamWrite(s, t + 4, 0x29, 0);
        StreamWrite(s, t + 0x2d, 4, 0);
        StreamWrite(s, t + 0x31, 4, 0);
        StreamWrite(s, t + 0x35, 4, 0);
        StreamWrite(s, t + 0x39, 4, 0);
    });
    table(F<Collection*>(w, 0x66b2), [&](uint8_t* e) {
        StreamWrite(s, e, 0x1e, 0);
        StreamWrite(s, e + 0x1e, 4, 0);
    });
    table(F<Collection*>(w, 0x66b6), [&](uint8_t* e) {
        StreamWrite(s, e, 0x3d, 0);
        StreamWrite(s, e + 0x3d, 4, 0);
    });
    StreamWrite(s, w + 0xbe2, 4, 0);
    for (int32_t k = 0; k < 5; ++k) StreamWrite(s, w + 0xbe6 + k * 4, 4, 0);
    table(g_cities, [&](uint8_t* c) {
        int32_t plan = F<int32_t>(c, 0x272), kind = F<int32_t>(c, 0x23e);
        float cx = (float)Obj_CellX(c), cy = (float)Obj_CellY(c);
        StreamWrite(s, c + 0x356, 0x29, 0);
        StreamWrite(s, &cx, 4, 0);
        StreamWrite(s, &cy, 4, 0);
        StreamWrite(s, &kind, 4, 0);
        StreamWrite(s, &plan, 4, 0);
        StreamWrite(s, c + 0x25a, 4, 0);
        int32_t v = F<int32_t>(c, 0x2f2);
        StreamWrite(s, &v, 4, 0);
        table(F<Collection*>(c, 0x37f), [&](uint8_t* e) {
            StreamWrite(s, e, 0x1e, 0);
            StreamWrite(s, e + 0x1e, 4, 0);
        });
    });
    StreamWrite(s, w + 0x4f30, 0x3e9, 0);
    {
        char label[0x100] = {};
        Collection* labels = g_6ada80;
        int32_t n = Collection_Count(labels);
        StreamWrite(s, &n, 4, 0);
        for (int32_t i = 0; i < n; ++i) {
            auto at = [&] { return (uint8_t*)Collection_Get(g_6ada80, 0,
                                                           Collection_IdAt(g_6ada80, 0, (uint32_t)i, 0)); };
            strcpy(label, (const char*)at() + 0x2a2);
            float lx = (float)Obj_CellX(at());
            float ly = (float)Obj_CellY(at());
            StreamWrite(s, label, 0x29, 0);
            StreamWrite(s, &lx, 4, 0);
            StreamWrite(s, &ly, 4, 0);
        }
    }
    for (int32_t k = 0; k < 0x31; ++k) StreamWrite(s, w + 0x4a7f + k * 4, 4, 0);
    StreamWrite(s, w + 0x4b43, 4, 0);
    {
        uint8_t* m = g_map;
        StreamWrite(s, F<uint8_t*>(m, 0x2141), (uint32_t)((F<int32_t>(m, 0x2161) * F<int32_t>(m, 0x215d) + 7) / 8), 0);
    }
    for (int32_t y = 0; y < F<int32_t>(g_map, 0x2151); ++y)
        for (int32_t x = 0; x < F<int32_t>(g_map, 0x214d); ++x) {
            uint8_t* m = g_map;
            int32_t a = F<uint8_t*>(m, 0x212d)[(F<int32_t>(m, 0x214d) * y + x) * 4];
            StreamWrite(s, &a, 4, 0);
            m = g_map;
            int32_t b = F<uint8_t*>(m, 0x212d)[(F<int32_t>(m, 0x214d) * y + x) * 4 + 3];
            StreamWrite(s, &b, 4, 0);
        }
    for (int32_t y = 0; y < F<int32_t>(g_map, 0x2159); ++y)
        for (int32_t x = 0; x < F<int32_t>(g_map, 0x2155); ++x) {
            const uint8_t high = (uint8_t)Map_CellHigh(g_map, 0, x, y);
            uint8_t count;
            if (!F<int32_t>((void*)0x5ef3e4, high * 0xb0)) {
                lowByte = (uint8_t)Map_CellLow(g_map, 0, x, y);
                count = (uint8_t)Terrain_CellCount(g_map, 0, x, y);
            } else {
                count = 0;
            }
            put8(high);
            put8(lowByte);
            put8(count);
        }
    StreamWriteU32(s, 0x4e21);
    RuleSets_Save(g_events, 0, s, 0);
    StreamWriteU32(s, 0x4e22);
    StreamWrite(s, w + 0x4c78, 4, 0);
    StreamWrite(s, w + 0x4c7c, 4, 0);
    Options_Restore();
    Cursor_PopBusy(g_cursorManager);
    Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1);
    StreamClose(s);
}
RT3_REPLACE(0x446240, World_ExportMap);

 // namespace

// 0x446d40 (thiscall, RET 4). World w loads the map file `name` (.smp) from
// the save folder, after a teardown and a fresh startup. The file version
// (0x3e9 .. 0x400) gates each part: the header block (+0x66be, from 0x3f1;
// before 0x3f7 shorter and its tail cleared), the size, terrain layers
// (0x3ec), corner heights, per cell water / drain / two grid bytes / flood /
// mark (0x3eb, 0x3ef; flooding from 0x3ff, or when a grid byte is set) and
// the terrain record bits (0x400; older maps get the defaults), 12 groups of
// region data (0x3f5), the texture (resampled when the terrain's size
// differs), cell territory bits and territories (0x3ee), templates and
// names (0x3f0; without names every engine's flag is set), five camera
// values (0x3f2), cities with their buildings (sizes from 0x5f3a28 before
// 0x3f3, ports 0x3f5), the intro text (0x3fa), markers (0x3f4), the 49
// region flags (0x3fc), the coast bits (0x3e9; cleared up to 0x3f5),
// per-quad bytes (one more from 0x3f8), cell heights (boosted by 2 from 19
// up to 0x3f8) and points, rules (0x3fd) and difficulty (0x3fe). Then the
// derived data is rebuilt; up to 0x3fa the 16 limits (+0x69dc, every 9
// bytes) become 100000.
// QUIRK: the cell loops pump the loading screen on rows where (y & 0x15) == 0.
RT3_NOINLINE void __fastcall World_LoadMap(uint8_t* w, int, const char* name) {
    auto ed = [] { return g_app; };
    auto map = [] { return g_map; };
    auto status = [&](int32_t id) {
        uint8_t* win = F<uint8_t*>(ed(), 0xc);
        Window_SendEvent5(win, 0, 0x6f, 0x84d2, (uint32_t)(uintptr_t)GetString(id), 0, 0);
        Loading_Pump(ed(), 0, 0, 1);
    };
    if (uint8_t* view = g_gameWindow) View_SetToolMode(view, 0, 0, -1);
    World_Teardown(w);
    Loading_Pump(ed(), 0, 0, 0);
    World_Startup(w, 0, 0, 0);
    char path[0x12c];
    {
        const char* folder = Options_SaveFolder(ed());
        FormatText(path, (const char*)0x5c9d00, folder, name, 0, 0, 0, 0, 0, 0);
    }
    const Stream s = StreamOpen(path, 6, 0x30d40, 1);
    auto read = [&](void* dst, uint32_t size) { StreamRead(s, dst, size, 0, 1); };
    int32_t version;
    read(&version, 4);
    status(0x19a);
    if (version >= 0x3f1) {
        StreamSkip4(s, 0x2ee0, 0, 0);
        if (version >= 0x3f7) {
            read(w + 0x66be, 0x403c2);
        } else {
            read(w + 0x66be, 0x402c9);
            memset(w + 0x6987, 0, 0x400f9);
        }
        StreamSkip4(s, 0x2ee1, 0, 0);
        Obj_AssignRandomSlots(w + 0x66be);
    }
    int32_t width, height;
    read(&width, 4);
    read(&height, 4);
    {
        uint8_t* m = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x2209);
        g_map = m ? Map_Construct(m, 0, width + 1, height + 1, nullptr) : nullptr;
    }
    if (version >= 0x3ec) Terrain_LoadLayers(map(), 0, s);
    for (int32_t y = 0; y < height + 1; ++y)
        for (int32_t x = 0; x < width + 1; ++x) {
            uint32_t v;
            read(&v, 4);
            Map_SetCellHeight(map(), 0, x, y, v, 0);
        }
    status(0x19b);
    for (int32_t y = 0; y < height; ++y) {
        if (!(y & 0x15)) Loading_Pump(ed(), 0, 0, 1);
        for (int32_t x = 0; x < width; ++x) {
            uint8_t water, flood, drain, a, b, mark;
            read(&water, 1);
            if (water) Terrain_MarkFlooded(map(), 0, x, y);
            if (version < 0x3eb) continue;
            read(&flood, 1);
            if (version >= 0x3ef) {
                read(&drain, 1);
                if (drain) Terrain_MarkDrained(map(), 0, x, y);
                read(&a, 1);
                Grid_SetByte1661(map(), 0, x, y, a);
                read(&b, 1);
                Grid_SetByte1665(map(), 0, x, y, b);
                if (flood && (version >= 0x3ff || a || b)) Map_FloodCell(map(), 0, x, y, 1);
            }
            read(&mark, 1);
            if (mark) Grid_MarkCell(map(), 0, x, y);
            if (version >= 0x400) {
                uint8_t b5, hi, mid, b11, b10;
                read(&b5, 1);
                read(&hi, 1);
                read(&mid, 1);
                read(&b11, 1);
                read(&b10, 1);
                uint8_t* r = Terrain_Rec(map(), 0, x, y);
                r[2] ^= (uint8_t)(((b5 << 5) ^ r[2]) & 0x20);
                r = Terrain_Rec(map(), 0, x, y);
                F<uint16_t>(r, 0) = (uint16_t)((F<uint16_t>(r, 0) & 0xfe3f) | (hi & 7) << 6);
                r = Terrain_Rec(map(), 0, x, y);
                F<uint16_t>(r, 0) = (uint16_t)((F<uint16_t>(r, 0) & 0xffc7) | (mid & 7) << 3);
                r = Terrain_Rec(map(), 0, x, y);
                F<uint16_t>(r, 0) = (uint16_t)((F<uint16_t>(r, 0) & 0xf7ff) | (b11 & 1) << 0xb);
                r = Terrain_Rec(map(), 0, x, y);
                F<uint16_t>(r, 0) = (uint16_t)((F<uint16_t>(r, 0) & 0xfbff) | (b10 & 1) << 0xa);
            } else {
                Terrain_Rec(map(), 0, x, y)[2] &= 0xdf;
                uint8_t* r = Terrain_Rec(map(), 0, x, y);
                F<uint16_t>(r, 0) = (uint16_t)((F<uint16_t>(r, 0) & 0xfeff) | 0xc0);
                Terrain_Rec(map(), 0, x, y)[0] &= 0xc7;
                Terrain_Rec(map(), 0, x, y)[1] &= 0xf7;
                Terrain_Rec(map(), 0, x, y)[1] &= 0xfb;
            }
        }
    }
    if (version >= 0x3f5) {
        for (int32_t g = 0; g < 0xc; ++g) {
            uint8_t* grp = w + 0xfe7 + g * 0x4e1;
            read(grp + 0x3ed, 4);
            read(grp, 0x3d);
            for (int32_t k = 0; k < 5; ++k) {
                uint8_t* e = grp + 0x3f5 + k * 0x30;
                read(e - 4, 4);
                read(e, 4);
                read(e + 4, 0x14);
                read(e + 0x18, 0x14);
            }
        }
    }
    Loading_Pump(ed(), 0, 1, 1);
    Map_CellsChangedImpl(map(), 0, 0, 0, width, height);
    Loading_Pump(ed(), 0, 1, 1);
    Terrain_RebuildWaterMask(map());
    Loading_Pump(ed(), 0, 1, 1);
    status(0x19c);
    {
        int32_t texW, texH;
        read(&texW, 4);
        read(&texH, 4);
        Terrain_BuildTiles(map(), 0, texW, texH, nullptr);
        uint32_t* const src = (uint32_t*)((void*(__cdecl*)(size_t))0x5a125d)((size_t)(texH * texW * 4));
        read(src, (uint32_t)(texW * texH * 4));
        int32_t outW, outH;
        uint32_t* const dst = Terrain_ExtractTexture(map(), 0, 0.0f, 0.0f, 1.0f, 1.0f, &outW, &outH);
        if (outW == texW && outH == texH) {
            memcpy(dst, src, (size_t)(outW * outH * 4));
        } else {
            const float sx = (float)((double)(float)texW / (float)outW);
            const float sy = (float)((double)(float)texH / (float)outH);
            for (int32_t row = 0; row < outH; ++row) {
                if (outW <= 0) continue;
                const int32_t fy = Ftol((double)(float)row * sy);
                for (int32_t col = 0; col < outW; ++col) {
                    const int32_t fx = Ftol((double)(float)col * sx);
                    dst[row * outW + col] = src[fy * texW + fx];
                }
            }
        }
        Terrain_PasteTexture(map(), 0, 0, 0, outW - 1, outH - 1);
        Terrain_CommitTexture(map(), 0, 0);
        ((void(__cdecl*)(void*))0x5a1145)(src);
    }
    Territories_AddTestSet(g_territories);
    if (version >= 0x3ee) {
        for (int32_t y = 0; y < height; ++y)
            for (int32_t x = 0; x < width; ++x) {
                uint8_t t;
                read(&t, 1);
                uint8_t* c = F<uint8_t*>(map(), 0x2131) + F<int32_t>(map(), 0x2155) * y + x;
                *c = (uint8_t)((*c & 7) | (uint8_t)(t << 3));
            }
        int32_t count;
        read(&count, 4);
        Map_ClearCollection([] { return g_territories; });
        for (int32_t i = 0; i < count; ++i) {
            Collection* terr = g_territories;
            const uint32_t id = StationTypes_Add(terr, 0, (const char*)0x5c87a8);
            uint8_t* r = (uint8_t*)Collection_Get(g_territories, 0, id);
            read(r + 4, 0x29);
            read(r + 0x2d, 4);
            read(r + 0x31, 4);
            read(r + 0x35, 4);
            read(r + 0x39, 4);
        }
    }
    Territories_Recount(g_territories);
    status(0x19d);
    Cities_AddTestRow(g_cities);
    Map_ClearCollection([&] { return F<Collection*>(w, 0x66b2); });
    uint8_t rec[0x44];
    int32_t templates;
    read(&templates, 4);
    for (int32_t i = 0; i < templates; ++i) {
        memset(rec, 0, 0x22);
        read(rec, 0x1e);
        read(rec + 0x1e, 4);
        if (RtStricmp((const char*)rec, (const char*)0x5c8f94) && RtStricmp((const char*)rec, (const char*)0x5c8e4c) &&
            Table_GetByName(g_62b268, 0, (const char*)rec))
            Map_AddRecord(F<Collection*>(w, 0x66b2), rec);
    }
    if (version >= 0x3f0) {
        Map_ClearCollection([&] { return F<Collection*>(w, 0x66b6); });
        int32_t names;
        read(&names, 4);
        for (int32_t i = 0; i < names; ++i) {
            memset(rec, 0, 0x41);
            read(rec, 0x3d);
            read(rec + 0x3d, 4);
            Map_AddRecord(F<Collection*>(w, 0x66b6), rec);
        }
    }
    if (Collection_Count(F<Collection*>(w, 0x66b6)) <= 0) {
        for (int32_t i = 0; i < Collection_Count(g_6ada84); ++i) {
            Collection* all = g_6ada84;
            void* e = Collection_Get(all, 0, Collection_IdAt(all, 0, (uint32_t)i, 0));
            World_SetNamedFlag(g_world, 0, (const char*)Obj_Ref72Plus45(e), 1);
        }
    }
    if (version >= 0x3f2) {
        read(w + 0xbe2, 4);
        F<uint32_t>(w, 0xbde) = F<uint32_t>(w, 0xbe2);
        for (int32_t k = 0; k < 5; ++k) read(w + 0xbe6 + 4 * k, 4);
    }
    status(0x19e);
    Map_ClearCollection([] { return g_cities; });
    int32_t cities;
    read(&cities, 4);
    for (int32_t i = 0; i < cities; ++i) {
        char cname[0x2c];
        int32_t cx, cy, rural, kind, sizeClass;
        uint32_t size;
        read(cname, 0x29);
        read(&cx, 4);
        read(&cy, 4);
        read(&rural, 4);
        read(&kind, 4);
        if (version >= 0x3f3) {
            read(&size, 4);
        } else {
            read(&sizeClass, 4);
            size = hook::Global<uint32_t>(0x5f3a28 + 4 * sizeClass);
        }
        const uint32_t id = Cities_Add(g_cities, 0, (uint32_t)cx, (uint32_t)cy, cname, rural);
        uint8_t* city = (uint8_t*)Collection_Get(g_cities, 0, id);
        F<int32_t>(city, 0x272) = kind;
        F<uint32_t>(city, 0x25a) = size;
        F<uint32_t>(city, 0x25e) = size;
        int32_t ports = 0;
        if (version >= 0x3f5) read(&ports, 4);
        Obj_NumberPorts(city, 0, ports);
        Map_ClearCollection([&] { return F<Collection*>(city, 0x37f); });
        int32_t buildings;
        read(&buildings, 4);
        for (int32_t j = 0; j < buildings; ++j) {
            memset(rec, 0, 0x22);
            read(rec, 0x1e);
            read(rec + 0x1e, 4);
            if (RtStricmp((const char*)rec, (const char*)0x5ca9e0) && RtStricmp((const char*)rec, (const char*)0x5c8f94) &&
                RtStricmp((const char*)rec, (const char*)0x5c8e4c) && F<float>(rec, 0x1e) > Fl(0x5c8598) &&
                Table_GetByName(g_62b268, 0, (const char*)rec))
                Map_AddRecord(F<Collection*>(city, 0x37f), rec);
        }
        Obj_UpdatePortFlags(city);
    }
    if (version >= 0x3fa) read(w + 0x4f30, 0x3e9);
    else w[0x4f30] = 0;
    if (version >= 0x3f4) {
        Map_ClearCollection([] { return g_6ada80; });
        int32_t markers;
        read(&markers, 4);
        for (int32_t i = 0; i < markers; ++i) {
            char label[0x2c];
            int32_t mx, my;
            read(label, 0x29);
            read(&mx, 4);
            read(&my, 4);
            Markers_Add(g_6ada80, 0, (uint32_t)mx, (uint32_t)my, label, 0);
        }
    }
    if (version >= 0x3fc) {
        for (int32_t k = 0; k < 0x31; ++k) read(w + 0x4a7f + 4 * k, 4);
        read(w + 0x4b43, 4);
    }
    if (version >= 0x3e9) {
        status(0x19f);
        const int32_t bytes = (F<int32_t>(map(), 0x2161) * F<int32_t>(map(), 0x215d) + 7) / 8;
        uint8_t* coast = F<uint8_t*>(map(), 0x2141);
        read(coast, (uint32_t)bytes);
        if (version <= 0x3f5) memset(F<uint8_t*>(map(), 0x2141), 0, (size_t)bytes);
    }
    status(0x1a0);
    for (int32_t y = 0; y < F<int32_t>(map(), 0x2151); ++y)
        for (int32_t x = 0; x < F<int32_t>(map(), 0x214d); ++x) {
            uint32_t q;
            read(&q, 4);
            F<uint8_t*>(map(), 0x212d)[(F<int32_t>(map(), 0x214d) * y + x) * 4] = (uint8_t)q;
            if (version >= 0x3f8) {
                uint32_t q3 = 0;
                read(&q3, 4);
                F<uint8_t*>(map(), 0x212d)[(F<int32_t>(map(), 0x214d) * y + x) * 4 + 3] = (uint8_t)q3;
            }
        }
    status(0x1a1);
    for (int32_t y = 0; y < F<int32_t>(map(), 0x2159); ++y) {
        if (!(y & 0x15)) Loading_Pump(ed(), 0, 0, 1);
        for (int32_t x = 0; x < F<int32_t>(map(), 0x2155); ++x) {
            uint8_t hi, lo, points;
            read(&hi, 1);
            read(&lo, 1);
            read(&points, 1);
            if (version <= 0x3f8 && hi >= 0x13) hi = (uint8_t)(hi + 2);
            Terrain_DecCellCount(map(), 0, x, y, 0x3e7);
            if (points) {
                Terrain_AddPoints(map(), 0, x, y, 1, points);
                Map_SetCellHigh(map(), 0, x, y, hi);
                Map_SetCellLow(map(), 0, x, y, lo);
            }
        }
    }
    if (version >= 0x3fd) {
        StreamSkip4(s, 0x4e21, 0, 0);
        RuleSets_Load(g_events, 0, s, 0);
        StreamSkip4(s, 0x4e22, 0, 0);
    }
    if (version >= 0x3fe) {
        read(w + 0x4c78, 4);
        read(w + 0x4c7c, 4);
    }
    StreamClose(s);
    Loading_Pump(ed(), 0, 1, 1);
    Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x808, 0, 0, 0);
    Loading_Pump(ed(), 0, 1, 1);
    if (Collection* bt = g_62b268) Types_RefreshAvailable(bt);
    if (Collection* engines = g_6ada84) Table_RefreshNamedFlags(engines);
    Loading_Pump(ed(), 0, 1, 1);
    Map_RebuildCoast(map(), 0, 0, 0, 0x270f, 0x270f, nullptr);
    Loading_Pump(ed(), 0, 1, 1);
    {
        void* cam = hook::As<void*>(g_62be68);
        ((void(__fastcall*)(void*))(*(void***)cam)[0])(cam);
    }
    Loading_Pump(ed(), 0, 1, 1);
    Cities_AssignBlocks(g_cities);
    Templates_Normalize(w);
    Types_RefreshAvailable(g_62b268);
    ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x3748] = 1;
    Loading_Pump(ed(), 0, 1, 1);
    Ambient_Start(g_world);
    Cities_LayStreets(g_cities);
    Map_MarkUntypedRegions(map());
    for (int32_t y = 0; y < F<int32_t>(map(), 0x2159); ++y)
        for (int32_t x = 0; x < F<int32_t>(map(), 0x2155); ++x) {
            if (!Terrain_KindIs1Or3to5(map(), 0, x, y)) continue;
            for (int32_t sy = 0; sy < 4; ++sy)
                for (int32_t sx = 0; sx < 4; ++sx) Map_SetFineBit(map(), 0, sx + x * 4, sy + y * 4, 0, 3, 1);
        }
    if (version <= 0x3fa)
        for (int32_t k = 0; k < 0x10; ++k) F<int32_t>(w, 0x69dc + 9 * k) = 0x186a0;
    F<int32_t>(ed(), 0x6c) = 0;
}
RT3_REPLACE(0x446d40, World_LoadMap);

}  // namespace rt3
