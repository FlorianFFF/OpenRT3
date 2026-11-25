// GameWindows.cpp - RT3.exe 0x4dc000-0x4e2000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <initializer_list>
#include <windows.h>
#include <math.h>
#include <string.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall TrackPicker_SetTab(uint8_t* w, int, int32_t tab, int32_t value);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void* __fastcall Engine_Renderer(void* e);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t ClockMs();
uint32_t __fastcall TreeView_NodeValue(uint8_t* w, int, int32_t i);
int32_t __fastcall Obj_Get21(void* o);
void* __fastcall Obj_Company21(void* o);
int32_t __fastcall Company_NameIndex(void* c);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
int32_t __fastcall HasString(int32_t id);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Widget_SetVtbl(uint8_t* w);
uint32_t __fastcall Input_Flag800();
uint32_t __fastcall Input_Flag2000();
void* __fastcall Engine_Input(void* e);
int32_t __fastcall Cursor_InWindow();
void __fastcall Window_HighlightViewModes(uint8_t* w);
void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
uint32_t __fastcall Sound_Is3D(uint8_t* s);
uint8_t* __fastcall SaveList_Init(uint8_t* list, int, int32_t kind);
uint32_t IsEnglish();
void __fastcall Music_Close(uint8_t* m);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Window_SendEvent4(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
void* __fastcall Obj_City41(void* o);

// Adds a small image button (Control) with `def` filled by `fill` to `win`.
template <typename Fill> static void AddControlButton(uint8_t* win, Fill fill) {
    uint8_t def[0xa5];
    memset(def, 0, sizeof def);
    fill(def);
    uint8_t* c = (uint8_t*)crt::operator_new(0xe9);
    if (c) c = Control_Construct(c, 0, def);
    Window_AddChild(win, 0, c, 0);
    crt::free(F<void*>(def, 0x43));
    if (F<void*>(def, 0x3f)) crt::free(F<void*>(def, 0x3f));
}

void* __fastcall Obj_CityRef(void* o);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint8_t* __fastcall Obj_NamedEntry40(uint8_t* o);
void __fastcall Table_MarkStale(Collection* t);
void* __fastcall Table_Oldest(Collection* t, int, uint8_t all);
void* __fastcall Table_FindField4Next(Collection* t, int, int32_t v);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
uint8_t* __fastcall Obj_NamedEntry20(uint8_t* o);
int32_t __fastcall Widget_TextLines(uint8_t* w);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
char* __fastcall Date_Format(char* buf, const uint8_t* d, int32_t mode);
void __fastcall PortraitWindow_Layout(uint8_t* w);
uint8_t* __fastcall Windows_FindChild(uint8_t* o, int, int32_t id);
int32_t __fastcall Engine_IsForeground();
void __fastcall Camera_Pan(uint8_t* c, int, float dx, float dz);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Models_DisplayName(const char* name, char* out);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
uint32_t __fastcall Sound_Get2c(uint8_t* s);
int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd);
const char* __fastcall Options_SaveFolder(void* opt);
float __fastcall Grid_Darkness(uint8_t* g, int, int32_t x, int32_t y, int32_t layer);
int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall City_CargoQuota(uint8_t* o, int, int32_t i);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
int32_t __fastcall Table_LiveRankOf(Collection* t, int, uint32_t id);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void* __fastcall Obj_Ref25(void* o);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void* __fastcall Map_RegionRecordAt(void* m, int, float x, float y);
void __fastcall TrimSpaces(char* s, int32_t alsoNewlines);
int32_t Global6d07b4Set2();
int32_t IsMultiplayer();
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall StreamClose(int32_t s);
uint8_t* __fastcall FileRequest_Construct(uint8_t* w, int, int32_t kind);
void __fastcall Obj_FreeBuffer8(void* o);
int32_t __fastcall Options_Check1bf(void* self);
int32_t __fastcall File_Exists(const char* path);
void* __fastcall Table_At(Collection* t, int, int32_t i);
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
int32_t __fastcall FileExists(const char* path);
void HighScores_Load();
void __fastcall SetupWindow_FillLabels(uint8_t* w);
int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
int32_t __stdcall Net_AllHaveVersion(float v);
void __fastcall Camera_ZoomStep(uint8_t* c, int, int32_t dir);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Map_CellCity(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Session_Shutdown(uint8_t* s);
void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall Train_PromptRename(uint32_t id, uint32_t b);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
int32_t __fastcall Map_CellHigh(uint8_t* m, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall StreamClose(Stream s);
uint32_t __fastcall FileDialog_Run(char* name, int32_t kind);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
uint8_t* __fastcall FileOptions_Construct(uint8_t* w);
int32_t Global6d1350Set();
void OptionsDialog_Open();
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
void __fastcall Ledger_Open(int32_t company);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Status_SetMessage(uint8_t* s, int, const char* text);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
void __fastcall Sprite_DrawFirstGlyph(uint8_t* o, int, float x, float y, int32_t, int32_t);
void __fastcall Sprite_DrawSecondGlyph(uint8_t* o, int, float x, float y, int32_t, int32_t);
void __fastcall CompanyWindow_Draw(uint8_t* p, int, float x, float y, float w, float h);
uint32_t __stdcall Scenario_CheckEditable();
uint32_t __fastcall Player_CheckHasCompany(uint8_t* p);
void __stdcall World_SetEditorButtons(uint32_t modeArg, uint32_t disabledArg);
uint32_t __stdcall StockMarket_IsAllowed();
uint32_t __fastcall Player_CheckStockAllowed(uint8_t* p);
uint32_t __fastcall Player_CheckCanRunTrains(uint8_t* p);
void __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t);
void Companies_Clash();
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
int32_t __fastcall Music_IsPlaying(uint8_t* m);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall MapScroll_Tick(uint8_t* w);
int32_t __fastcall Table_ActiveIndexOf(Collection* t, int, uint32_t id);
void __stdcall SetGlobal6d19f4(int32_t v);
void __fastcall CompanyList_Select(uint8_t* w, int, int32_t idx);
void __fastcall OptionsDialog_SetLightingSet(uint8_t* d, int, uint8_t on, uint8_t apply);
uint8_t* __fastcall Grid_RegionAt(void* g, int, int32_t x, int32_t y);
float __fastcall Station_TotalAvailable(void* o);
void __fastcall Camera_SnapTurn(uint8_t* c, int, int32_t dir);
void* __fastcall Map_RegionAt(uint8_t* map, int, float x, float y);
uint32_t __fastcall Player_CheckCanFound(uint8_t* p);
void Player_FoundCompanyDialog();
void FileMenu_Run();
uint8_t* __fastcall Map_CellAtQuarter(uint8_t* m, int, int32_t x, int32_t y);
int32_t __fastcall Screen_CanClose(uint8_t* s, int, int32_t);
int32_t __fastcall Overlay_ValueAt(uint8_t* o, int, float x, float y);
int32_t __fastcall Scene_FocusCell(uint8_t* s, int, float* x, float* y);
void __fastcall Dialog_ShowAndSelect(uint8_t* w, int, const char* name, int32_t select);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall View_DestroyToolForMode(uint8_t* view, int, uint32_t mode);
RT3_NOINLINE void __stdcall World_SetEditorButtons(uint32_t mode, uint32_t disabled);

// Functions of this file, in exe order.
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE void __stdcall Camera_FollowObject(uint8_t* o, int32_t mode);
RT3_NOINLINE uint32_t __stdcall Scenario_CheckEditable();
RT3_NOINLINE uint32_t __fastcall Player_CheckHasCompany(uint8_t* p);
RT3_NOINLINE uint32_t __fastcall Player_CheckCanRunTrains(uint8_t* p);
RT3_NOINLINE void __stdcall World_SetEditorButtons(uint32_t modeArg, uint32_t disabledArg);
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE uint32_t __stdcall StockMarket_IsAllowed();
RT3_NOINLINE uint32_t __fastcall Player_CheckStockAllowed(uint8_t* p);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall World_PrepareTestGame(uint8_t* w, int, int32_t fill);
RT3_NOINLINE int32_t __fastcall World_Load(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE void __fastcall World_Save(uint8_t* w, int, const char* name, uint32_t flags, int32_t* done);
RT3_NOINLINE uint32_t __fastcall Player_CheckCanFound(uint8_t* p);
RT3_NOINLINE void Player_FoundCompanyDialog();
RT3_NOINLINE void __fastcall Lighting_Rebuild(uint8_t* opts);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
void __fastcall Window_HighlightViewModes(uint8_t* w);
void __fastcall Rules_DrawCountCb(void*, int, float x, float y, float w, int32_t, int32_t);
void __fastcall TrainDetail_AddPictures(uint8_t* win, int, float x, float y, float, float);
int32_t __fastcall Train_FollowCityCb(void*, const uint32_t* ev);
RT3_NOINLINE void __fastcall Train_PromptRename(uint32_t id, uint32_t b);
uint8_t* __fastcall FileOptions_Construct(uint8_t* w);
int32_t Global6d0800Set();
void SetGlobal6d07fc();
int32_t __fastcall GameMenu_HandleEvent(void*, int, uint32_t* msg);
RT3_NOINLINE void FileMenu_Run();
RT3_NOINLINE int32_t Global6d0810Set();
uint8_t* __fastcall FileRequest_Construct(uint8_t* w, int, int32_t kind);
RT3_NOINLINE void __fastcall Dialog_ShowAndSelect(uint8_t* w, int, const char* name, int32_t select);
int32_t __fastcall SaveDialog_HandleEvent(uint8_t* w, int, uint32_t* msg);
void __fastcall ListDialog_PickRow(int32_t row);
RT3_NOINLINE uint32_t __fastcall FileDialog_Run(char* name, int32_t kind);
RT3_NOINLINE void __fastcall OptionsDialog_SetLightingSet(uint8_t* d, int, uint8_t on, uint8_t apply);
void __fastcall Window_HighlightMapModes(uint8_t* w);
void __fastcall EditTool_UpdateCursor(uint8_t* t);
RT3_NOINLINE void __fastcall StatusBar_ShowCash(uint8_t* w);
RT3_NOINLINE void __fastcall Panel_SetValue90(uint8_t* o, int, int32_t v);
RT3_NOINLINE uint8_t* __fastcall Window_EnableControl7f9(uint8_t* w, int, uint8_t on);
RT3_NOINLINE uint8_t* __fastcall Map_CellAtQuarter(uint8_t* m, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall View_DestroyToolForMode(uint8_t* view, int, uint32_t mode);
RT3_NOINLINE void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
RT3_NOINLINE int32_t __fastcall Screen_CanClose(uint8_t* s, int, int32_t);
int32_t __fastcall GameView_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall StatusBar_ShowDate(uint8_t* w);
void __fastcall Year_TooltipCb(void*, int, int32_t, int32_t);
RT3_NOINLINE uint8_t* __fastcall GameWindow_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall View_Destroy(uint8_t* view);
RT3_NOINLINE void __fastcall View_ResetEditorTool(uint8_t* view);
void GameMessagePanel_Open();
int32_t __fastcall NewsList_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall MessagePanel_Refresh(uint8_t* w);
void Panel6d081c_Close();
RT3_NOINLINE void __fastcall MessagePanel_RefreshOpen();
void __fastcall Obj_TakeListCommand(uint8_t* o);
RT3_NOINLINE void __fastcall MapScroll_Tick(uint8_t* w);
RT3_NOINLINE void __fastcall SetupWindow_FillLabels(uint8_t* w);
void __fastcall EditButton_SetCursor(int32_t id, int, int32_t active, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE uint8_t* __fastcall SetupWindow_Construct(uint8_t* w);
int32_t __fastcall SetupWindow_HandleEvent(uint8_t* d, int, uint32_t* msg);
void Panel6d0820_Close();
void SetupWindow_Open();
RT3_NOINLINE void __fastcall HighScores_Draw(void*, int, float, float y, float w, float, int32_t);
int32_t __fastcall Jukebox_HandleEvent(uint8_t* j, int, uint32_t* msg);
RT3_NOINLINE int32_t __fastcall HighScores_Add(int32_t list, const char* name, const char* scenario, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t quiet);
RT3_NOINLINE void HighScores_Load();
uint8_t* __fastcall HighScoresDialog_Construct(uint8_t* w);
void __fastcall HighScores_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall HighScores_Show(int32_t value, int32_t extra, int32_t keepHighlight);
void __fastcall Sprite_DrawFirstGlyphCb(void*, int, float x, float y, int32_t a, int32_t b, int32_t);
void __fastcall Sprite_DrawSecondGlyphCb(void*, int, float x, float y, int32_t a, int32_t b, int32_t);
void __fastcall CompanyWindow_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall PortraitWindow_Layout(uint8_t* w);
int32_t Global6d10a8Set();
void SetGlobal6d10a4();
void __fastcall CompanyWindow_SelectCb(int32_t v, int, int32_t on, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall DetailDialog_AddHeader(uint8_t* win, int, float x, float y, float, float);
RT3_NOINLINE void __fastcall Ledger_Open(int32_t company);
RT3_NOINLINE void __fastcall CompanyWindow_Draw(uint8_t* p, int, float x, float y, float w, float h);
RT3_NOINLINE void OptionsDialog_Open();
RT3_NOINLINE void __fastcall ConsistEditor_Run(uint8_t* owner, uint8_t* train, uint8_t* orders, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Date_Format(char* buf, const uint8_t* d, int32_t mode);
RT3_NOINLINE void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
RT3_NOINLINE void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
int32_t __fastcall Window_SendEvent4(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE int32_t __fastcall Widget_TextLines(uint8_t* w);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }



static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }


// Company finance statistics. Ids below 0x2b are kept per year: +0x1c47 + id * 8
// (this year so far) and +0xd7f + (id * 11 + n) * 8 (n years ago, 0 = live); larger
// ids are sums / ratios of those. Years: 0x2329 = live value, 0x232a = this year so
// far. World 0x6cec78: year +0xd, month +0xf, day +0x11, tick +0x15, difficulty +0x2d.
static inline uint8_t* WorldG() { return g_world; }

// Closes the panel held in `slot`: out of the window manager (0x6d401c), back to
// the base vtable and deleted.
static void Panel_Close(uint32_t slot, uint32_t vtbl, bool clearFirst) {
    uint8_t*& p = hook::Global<uint8_t*>(slot);
    if (!p) return;
    Windows_Remove(g_windowManager, 0, p);
    if (uint8_t* w = p) {
        F<uint32_t>(w, 0) = vtbl;
        if (clearFirst) p = nullptr;
        Widget_SetVtbl(w);
        crt::operator_delete(w);
    }
    p = nullptr;
}

// Opens a pack-defined panel once: window of `size` bytes with vtable `vtbl`,
// loaded from `name` at (0, 0), kept in `slot` and inserted into the window
// manager (the insert's select flag `select`).
static uint8_t* Panel_Open(uint32_t slot, size_t size, uint32_t vtbl, const char* name, int32_t select, bool highlight) {
    if (hook::Global<uint8_t*>(slot)) return nullptr;
    uint8_t* w = (uint8_t*)crt::operator_new(size);
    if (w) {
        Widget_SetVtblRet(w);
        F<uint32_t>(w, 0) = vtbl;
        Window_Load(w, 0, 0.0f, 0.0f, name);
        if (highlight) Window_HighlightViewModes(w);
    }
    hook::Global<uint8_t*>(slot) = w;
    WidgetList_Insert(hook::As<uint8_t**>(g_windowManager), 0, w, select);
    return w;
}

static inline float Prod_Bits(uint32_t b) { return *(const float*)&b; }

namespace {
int32_t Companies_ClashCall(const void* d) {
    int32_t r;
    __asm {
        push ebx
        mov ebx, d
        call Companies_Clash
        pop ebx
        mov r, eax
    }
    return r;
}
}

namespace {
uint32_t Message474(const char* text, int32_t type) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

namespace {
typedef int(__cdecl* Sprintf480)(char*, const char*, ...);
// Dialog_RunMessage at the screen spot (0.48, -1).
uint32_t Message480(uint32_t text, int32_t type) {
    return Dialog_RunMessage(GetString(text), type, 0x3ef5c28f, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

static void Method(uint32_t address, void* self) {
    hook::Orig<void(__fastcall*)(void*)>(address)(self);
}

static void Call(uint32_t address) { hook::Orig<void(*)()>(address)(); }

static void Event(void* self, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    Window_SendEvent5((uint8_t*)(self), 0, type, id, a, b, c);
}

// 0x4dc0d0 (draw callback). Draws the rule count 0x6d07e0 (string 0xc7b)
// in the rules dialog font, 0.26 high.
void __fastcall Rules_DrawCountCb(void*, int, float x, float y, float w, int32_t, int32_t) {
    char num[0x38], text[0x200];
    const char* n = Num_Format(num, 1, (double)g_6d07e0, 10, 1, 0, 0);
    FormatText(text, GetString(0xc7b), n, 0, 0, 0, 0, 0, 0, 0);
    Font_DrawTextBox(g_6d07e4, 0, text, x, (float)((double)y + Fl(0x5d0db8)), w, BitsF(0x3e851eb8), 0xffffffff, 0, 1, 2, 1.0f);
}
RT3_REPLACE(0x4dc0d0, Rules_DrawCountCb);

// 0x4dc180. Train detail window: a frame, the locomotive picture of the
// selected train (0x6cfcbc, id 0x6d07dc) and the "TrainDetail.imb" button
// 0xb9f5 at (x, y). The picture is "<engine type name +0x82>_NE.imb" (engine
// types 0x62ba88), replaced while the territory 0x6d07e8 has its override flag
// (+0x3cc) by the override table entry (pack +0x429b8) named like the
// territory (+0x3ac, format at +0x80); missing pictures fall back to the
// generic diesel / electric / steam one by the engine's power (+0x10).
void __fastcall TrainDetail_AddPictures(uint8_t* win, int, float x, float y, float, float) {
    uint8_t def[0x89];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + Fl(0x5d0e18));
    F<uint32_t>(def, 0xf) = 0x3e800000;
    F<uint32_t>(def, 0x13) = 0x3df5c28f;
    F<float>(def, 0xb) = (float)((double)y + D(0x5d0820));
    F<int32_t>(def, 0x61) = 3;
    F<int32_t>(def, 0x75) = 9;
    F<int32_t>(def, 0x2f) = 0xb0;
    uint8_t* frame = (uint8_t*)crt::operator_new(0x111);
    frame = frame ? Scrollbar_Construct(frame, 0, def) : nullptr;
    Window_AddChild(win, 0, frame, 0);
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + Fl(0x5d0e14));
    F<uint32_t>(def, 0xf) = 0x3e75c28f;
    F<uint32_t>(def, 0x13) = 0x3dda740e;
    F<float>(def, 0xb) = (float)((double)y + D(0x5ca2f8));
    uint8_t* eng = (uint8_t*)Obj_CityRef(Collection_Get(g_trains, 0, g_6d07dc));
    uint8_t* type = F<uint32_t>(eng, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(eng, 0x72)) : nullptr;
    char name[0x100];
    FormatText(name, (const char*)0x5d0e08, (const char*)(type + 0x82), 0, 0, 0, 0, 0, 0, 0);
    if (g_6d07e8 && ((uint8_t*)Obj_City41(g_6d07e8))[0x3cc]) {
        for (int32_t i = 0; i < Collection_Count(F<Collection*>(g_pack, 0x429b8)); ++i) {
            Collection* t = F<Collection*>(g_pack, 0x429b8);
            const char* entry = (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
            if (!crt::stricmp((const char*)Obj_City41(g_6d07e8) + 0x3ac, entry)) {
                FormatText(name, entry + 0x80, 0, 0, 0, 0, 0, 0, 0, 0);
                break;
            }
        }
    }
    if (!NameTable_ContainsName(g_pack, 0, name, 0, 0)) {
        int32_t power = F<int32_t>(eng, 0x10);
        const char* generic = power == 1 ? (const char*)0x5d0df0 : power == 2 ? (const char*)0x5d0dd8 : (const char*)0x5d0dc0;
        FormatText(name, generic, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    String_Assign(&F<char*>(def, 0x43), name);
    F<uint16_t>(def, 0x5f) = 0;
    F<int32_t>(def, 0x2f) = 0xb0;
    uint8_t* pic = (uint8_t*)crt::operator_new(0x111);
    pic = pic ? Scrollbar_Construct(pic, 0, def) : nullptr;
    Window_AddChild(win, 0, pic, 0);
    crt::free(F<void*>(def, 0x43));
    uint8_t cd[0xa5];
    memset(cd, 0, sizeof cd);
    F<float>(cd, 7) = (float)((double)x + D(0x5c8988));
    F<float>(cd, 0xb) = (float)((double)y + Fl(0x5d0dbc));
    F<uint32_t>(cd, 0xf) = 0x3d59999a;
    F<uint32_t>(cd, 0x13) = 0x3d11eb85;
    F<uint16_t>(cd, 0x5f) = 0x79;
    F<uint16_t>(cd, 0x61) = 0x7a;
    cd[0x89] = 2;
    cd[0x8a] = 3;
    F<uint16_t>(cd, 0) = 0xb9f5;
    String_Assign(&F<char*>(cd, 0x43), (const char*)0x5c9ee8);
    uint8_t* c = (uint8_t*)crt::operator_new(0xe9);
    c = c ? Control_Construct(c, 0, cd) : nullptr;
    Window_AddChild(win, 0, c, 0);
    crt::free(F<void*>(cd, 0x43));
}
RT3_REPLACE(0x4dc180, TrainDetail_AddPictures);

// 0x4dc4e0 (event callback). Release of 0xb9f5 makes the camera follow the
// selected train's (0x6d07dc) city object. Returns 0 then, else 1.
int32_t __fastcall Train_FollowCityCb(void*, const uint32_t* ev) {
    if (ev[0] != 0xcb || ev[1] != 0xb9f5) return 1;
    if (uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, g_6d07dc))
        if (Obj_City41(t)) Camera_FollowObject((uint8_t*)Obj_City41(t), 0);
    return 0;
}
RT3_REPLACE(0x4dc4e0, Train_FollowCityCb);

// 0x4dc540 (train in ECX, b in EDX). With train naming on (options +0x273)
// asks for a new name for train `id` of 0x6cfcbc (text box 9, up to 127
// characters, with its portrait panel 0x4dc180 / 0x4dc0d0 / 0x4dc4e0 and
// font 2 held in 0x6d07e4) and stores it (+0x69, 30 characters).
RT3_NOINLINE void __fastcall Train_PromptRename(uint32_t id, uint32_t b) {
    if (!F<uint32_t>(g_app, 0x273)) return;
    g_6d07dc = id;
    hook::As<uint32_t>(g_6d07e0) = b;
    g_6d07e4 = Pack_AcquireFont(g_pack, 0, 2);
    if (F<uint32_t>(g_app, 0x273)) {
        uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, g_6d07dc);
        g_6d07e8 = t;
        if (t) {
            char buf[0x12c];
            strcpy(buf, Train_DisplayName(t, 0, 0));
            Dialog_RunMessage((const char*)0x5c87a8, 9, 0xbf800000, 0xbf800000, 0x4dc4e0,
                              (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4dc180, 0x4dc0d0, Prod_Bits(0x3eddddde),
                              (uint32_t)(uintptr_t)buf, 0x7f, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            t = (uint8_t*)Collection_Get(g_trains, 0, g_6d07dc);
            g_6d07e8 = t;
            if (t) StrCopyN((char*)t + 0x69, buf, 0x1e);
        }
    }
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d07e4));
    hook::As<uint32_t>(g_6d07e4) = 0;
    g_6d07dc = 0;
}
RT3_REPLACE(0x4dc540, Train_PromptRename);

// 0x4dc670. File options dialog constructor ("fileopt.win" at (0.334, 0.07),
// vtable 0x5d0e1c): clears 0x6d07ec / 0x6d07f0 / 0x6d07f8; outside English the
// fields 0x1b5a..0x1b60 get a length limit (18; language 1: 1 and the field
// widened); in a scenario (0x6cec7c +0xc5) field 0x1b5e explains (0xbc4 / 0xbc5,
// or 0xbc6 / 0xbc7 when world +0x4a77 is clear); string 0xf83 labels 0x1b5d.
RT3_NOINLINE uint8_t* __fastcall FileOptions_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0e1c;
    g_6d07f8 = 0;
    g_6d07ec = 0;
    g_6d07f0 = 0;
    Window_Load(w, 0, BitsF(0x3eaae148u), BitsF(0x3d8f5c29u), (const char*)0x5d0e28);
    if (!(uint8_t)IsEnglish()) {
        for (uint32_t id = 0x1b5a; id <= 0x1b60; ++id) {
            uint32_t limit = 0x12;
            if (g_language == 1) {
                if (uint8_t* f = Widget_FindChild(w, 0, id)) {
                    F<float>(f, 0xb2) = (float)(F<float>(f, 0xb2) - D(0x5d0280));
                    F<float>(f, 0xba) = (float)(F<float>(f, 0xba) + D(0x5ce8c8));
                }
                limit = 1;
            }
            Window_SendEvent5(w, 0, 0x78, id, limit, 0, 0);
        }
    }
    uint8_t* ng = g_newGame;
    if (ng && ng[0xc5]) {
        bool locked = F<int32_t>(WorldG(), 0x4a77) != 0;
        Window_SendEvent5(w, 0, 0x6f, 0x1b5e, (uint32_t)(uintptr_t)GetString(locked ? 0xbc4 : 0xbc6), 0, 0);
        Window_SendEvent5(w, 0, 0x6e, 0x1b5e, (uint32_t)(uintptr_t)GetString(locked ? 0xbc5 : 0xbc7), 0, 0);
    }
    if (HasString(0xf83)) Window_SendEvent5(w, 0, 0x6f, 0x1b5d, (uint32_t)(uintptr_t)GetString(0xf83), 0, 0);
    return w;
}
RT3_REPLACE(0x4dc670, FileOptions_Construct);

int32_t Global6d0800Set() { return hook::As<void*>(g_6d0800) ? 1 : 0; }
RT3_REPLACE(0x4dc7d0, Global6d0800Set);

void SetGlobal6d07fc() { g_6d07fc = 1; }
RT3_REPLACE(0x4dc7e0, SetGlobal6d07fc);

// 0x4dc7f0 (event handler). Game menu buttons 0x1b5a..0x1b60: save (not in
// multiplayer, string 0xbc8; 0x6d07f8), load (not in the campaign 0xe8c; in
// multiplayer only for the host with v1.04 peers, 0xbc9 / 0x1100; 0x6d07ec),
// the scenario goals (string 0xf3a for tutorials, else the description
// +0x5ae9 / +0x4f30 or 0xfe), options (0x6d07f0), quit to menu (confirm
// 0xbca in a campaign scenario, marking it played; else 0xbcb; event 0xcc
// with 7 / 2) and quit the game (0xbcc, event 0xcd); 0x1b60 just closes. Idle closes it when
// 0x6d07fc asks.
int32_t __fastcall GameMenu_HandleEvent(void*, int, uint32_t* msg) {
    if (msg[0] == 0xc9) return 1;
    if (msg[0] == 0) {
        if (g_6d07fc) {
            g_6d07fc = 0;
            msg[0] = 0xc9;
        }
        return 1;
    }
    if (msg[0] != 0xcb) return 1;
    switch (msg[1]) {
    case 0x1b5a:
        if (g_world && IsMultiplayer()) {
            Message474(GetString(0xbc8), 1);
            return 0;
        }
        msg[0] = 0xc9;
        g_6d07f8 = 1;
        return 1;
    case 0x1b5b:
        if (g_6d3b4c) {
            Message474(GetString(0xe8c), 1);
            return 0;
        }
        if (g_world && IsMultiplayer()) {
            if (!Net_AllHaveVersion(BitsF(0x3f851eb8))) {
                Message474(GetString(0xbc9), 1);
                return 0;
            }
            if (!g_6d40dc) {
                Message474(GetString(0x1100), 1);
                return 0;
            }
        }
        msg[0] = 0xc9;
        g_6d07ec = 1;
        return 1;
    case 0x1b5c: {
        if (g_newGame[0x82]) {
            Message474(GetString(0xf3a), 1);
            return 0;
        }
        const bool mp = IsMultiplayer() != 0;
        uint8_t* w = g_world;
        const char* text = nullptr;
        if (mp && strlen((const char*)w + 0x5ae9) > 1)
            text = (const char*)w + 0x5ae9;
        else if (strlen((const char*)w + 0x4f30) > 1)
            text = (const char*)w + 0x4f30;
        Message474(text ? text : GetString(0xfe), 1);
        return 0;
    }
    case 0x1b5d:
        msg[0] = 0xc9;
        g_6d07f0 = 1;
        return 1;
    case 0x1b5e: {
        uint8_t* sc = g_newGame;
        int32_t campaign = 0;
        if (sc && sc[0xc5]) {
            campaign = 1;
            if (!F<int32_t>(g_world, 0x4a77)) {
                if (Message474(GetString(0xbca), 3) != 0x3f2) return 0;
                F<int32_t>(g_world, 0x4a77) = 1;
                F<int32_t>(g_world, 0x4a73) = 1;
                sc = g_newGame;
                g_6cfd24 = sc[0xc4] + 1;
                uint8_t* played = sc + 0xc6 + sc[0xc4];
                if (*played < 1) *played = 1;
            }
        } else if (Message474(GetString(0xbcb), 3) != 0x3f2) {
            return 0;
        }
        g_6d07f4 = 1;
        View_SetToolMode(g_gameWindow, 0, 0, -1);
        Game_PostInputEvent(0xcc, 0, campaign ? 7 : 2, 0, 0);
        msg[0] = 0xc9;
        return 1;
    }
    case 0x1b5f:
        if (Message474(GetString(0xbcc), 3) != 0x3f2) return 0;
        Game_PostInputEvent(0xcd, 0, 0, 0, 0);
        msg[0] = 0xc9;
        return 1;
    case 0x1b60:
        msg[0] = 0xc9;
        return 1;
    default:
        return 1;
    }
}
RT3_REPLACE(0x4dc7f0, GameMenu_HandleEvent);

// 0x4dcbd0. The file menu (FileOptions, 0x7b bytes in 0x6d0800) runs
// modally with the game paused and the arrow cursor; the view's cursor comes
// back unless the view mode changed or it is the busy cursor (6). Its
// choices: quit a multiplayer session (0x6d07f4), load (0x6d07f8; a map in
// the editor then starts a test game), save (0x6d07ec), and the options
// dialog (0x6d07f0, unless one is open).
RT3_NOINLINE void FileMenu_Run() {
    const int32_t cursor = F<int32_t>(g_cursorManager, 4);
    const int32_t mode = F<int32_t>(g_gameWindow, 0x8c);
    Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    g_6d07f4 = 0;
    uint8_t* mem = ((uint8_t*)crt::operator_new(0x7b));
    g_6d0800 = mem ? FileOptions_Construct(mem) : nullptr;
    PauseLock_Push(hook::As<int32_t*>(g_app));
    Ui_RunModal(hook::As<uint8_t**>(g_windowManager), 0, g_6d0800, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::As<int32_t*>(g_app));
    Windows_Remove(g_windowManager, 0, g_6d0800);
    if (uint8_t* win = g_6d0800) {
        F<uint32_t>(win, 0) = 0x5d0e1c;
        Widget_SetVtbl(win);
        crt::operator_delete(win);
    }
    g_6d0800 = nullptr;
    if (g_6d07f4) {
        if (uint8_t* s = g_session) {
            Session_Shutdown(s);
            crt::operator_delete(s);
            g_session = nullptr;
        }
    }
    if (F<int32_t>(g_gameWindow, 0x8c) == mode && F<int32_t>(g_cursorManager, 4) != 6)
        Cursor_Set(g_cursorManager, 0, cursor, 0, 0);
    if (g_6d07f8) {
        if (World_Load(g_world, 0, nullptr, 0, nullptr) && Options_Check1bf(g_app))
            World_PrepareTestGame(g_world, 0, 0);
    } else if (g_6d07ec) {
        World_Save(g_world, 0, nullptr, 0, nullptr);
    }
    if (g_6d07f0 && !Global6d1350Set()) OptionsDialog_Open();
}
RT3_REPLACE(0x4dcbd0, FileMenu_Run);

RT3_NOINLINE int32_t Global6d0810Set() { return hook::As<void*>(g_6d0810) ? 1 : 0; }
RT3_REPLACE(0x4dcd40, Global6d0810Set);

// 0x4dcd50. File request dialog constructor ("filerqst.win" at (0.222, 0.04),
// vtable 0x5d0e34): scans the saves of `kind` into 0x6d0814 and lists their names
// (+0x12d of 0x25a-byte entries) in 0x1f45; language 1 limits the name fields
// 0x1f42 / 0x1f43 to 18 characters.
RT3_NOINLINE uint8_t* __fastcall FileRequest_Construct(uint8_t* w, int, int32_t kind) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0e34;
    Window_Load(w, 0, BitsF(0x3e633333u), BitsF(0x3d23d70au), (const char*)0x5d0e40);
    uint8_t* list = (uint8_t*)crt::operator_new(0xc);
    if (list) list = SaveList_Init(list, 0, kind);
    g_6d0814 = list;
    for (int32_t i = 0; i < F<int32_t>(g_6d0814, 0); ++i)
        Window_SendEvent5(w, 0, 0x88, 0x1f45, (uint32_t)(uintptr_t)(F<uint8_t*>(g_6d0814, 8) + i * 0x25a + 0x12d), 0, 0);
    if (g_language == 1) {
        Window_SendEvent5(w, 0, 0x78, 0x1f42, 0x12, 0, 0);
        Window_SendEvent5(w, 0, 0x78, 0x1f43, 0x12, 0, 0);
    }
    return w;
}
RT3_REPLACE(0x4dcd50, FileRequest_Construct);

// 0x4dce10 (dialog fill). Shows text `name` (label 0x1f44); with `select`
// the list 0x1f45 selects the last row whose value equals it (-1 none).
RT3_NOINLINE void __fastcall Dialog_ShowAndSelect(uint8_t* w, int, const char* name, int32_t select) {
    Window_SendEvent5(w, 0, 0x6f, 0x1f44, (uint32_t)(uintptr_t)name, 0, 0);
    if (!select) return;
    uint8_t* list = Widget_FindChild(w, 0, 0x1f45);
    int32_t sel = -1;
    for (int32_t i = 0; i < F<int32_t>(list, 0x1d2); ++i)
        if (!crt::stricmp(name, (const char*)(uintptr_t)TreeView_NodeValue(list, 0, i))) sel = i;
    Window_SendEvent5(w, 0, 0x66, 0x1f45, (uint32_t)sel, 0, 0);
}
RT3_REPLACE(0x4dce10, Dialog_ShowAndSelect);

// 0x4dcea0 (event handler). The save name dialog w: typing in the name field
// 0x1f44 copies it to 0x6d0808 (shown again, trimmed; 0x6d0804 = not empty);
// picking 0x1f45 clears 0x6d080c; OK (0x1f42 or the pick) closes with a
// name, else tells 0xf7 / 0xf8 (by the save slot kind 0x6d0814 +4);
// Cancel (0x1f43) closes without one.
int32_t __fastcall SaveDialog_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    auto confirm = [&]() {
        if (g_6d0804) {
            msg[0] = 0xc9;
            return 1;
        }
        const int32_t kind = F<int32_t>(g_6d0814, 4);
        Dialog_RunMessage(GetString(kind < 0 || kind > 3 ? 0xf8 : 0xf7), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0,
                          nullptr, 0);
        return 0;
    };
    if (msg[0] == 0xca) {
        if (msg[1] == 0x1f44) {
            char* name = g_6d0808;
            strcpy(name, F<const char*>(F<uint8_t*>(msg, 0x2c), 0xba));
            Dialog_ShowAndSelect(w, 0, g_6d0808, 1);
            TrimSpaces(g_6d0808, 0);
            g_6d0804 = strlen(g_6d0808) != 0;
            return 0;
        }
        if (msg[1] == 0x1f45) {
            g_6d080c = 0;
            return confirm();
        }
    } else if (msg[0] == 0xcb) {
        if (msg[1] == 0x1f42) return confirm();
        if (msg[1] == 0x1f43) {
            g_6d0804 = 0;
            msg[0] = 0xc9;
        }
    }
    return 1;
}
RT3_REPLACE(0x4dcea0, SaveDialog_HandleEvent);

// 0x4dcfb0. Picks row `row` of the list 0x1f45 in dialog 0x6d0810: copies its text
// to the buffer 0x6d0808, shows it in field 0x1f44 (event 0x6f) and flags the pick
// (0x6d0804). Nothing for -1.
void __fastcall ListDialog_PickRow(int32_t row) {
    if (row == -1) return;
    g_6d0804 = 1;
    const char* text = (const char*)(uintptr_t)TreeView_NodeValue(Widget_FindChild(g_6d0810, 0, 0x1f45), 0, row);
    strcpy(g_6d0808, text);
    Window_SendEvent5(g_6d0810, 0, 0x6f, 0x1f44, (uint32_t)(uintptr_t)g_6d0808, 0, 0);
}
RT3_REPLACE(0x4dcfb0, ListDialog_PickRow);

// 0x4dd010 (name buffer in ECX, file kind in EDX). Runs the save / load file
// dialog (0x7b bytes, 0x6d0810, list 0x6d0814; saving for list kinds 0..3
// and 0xb: captions 0xf9 / 0xfa, focus 0x1f44; else 0xfb..0xfd) into `name`
// (0x6d0808) and returns its result 0x6d0804. A chosen name gets the kind's
// extension (0x5f3d58; ".smp" with option +0x1bf); when loading a missing
// file whose ".gmp" twin exists in the save folder, that is used instead.
RT3_NOINLINE uint32_t __fastcall FileDialog_Run(char* name, int32_t kind) {
    hook::As<uint32_t>(g_6d0804) = 0;
    g_6d0808 = name;
    hook::As<uint32_t>(g_6d080c) = 1;
    int32_t mode = g_gameWindow ? F<int32_t>(g_gameWindow, 0x8c) : 0;
    int32_t cursor = F<int32_t>(g_cursorManager, 4);
    Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    uint8_t* w = (uint8_t*)crt::operator_new(0x7b);
    w = w ? FileRequest_Construct(w, 0, kind) : nullptr;
    g_6d0810 = w;
    int32_t lk = F<int32_t>(g_6d0814, 4);
    bool save = lk == 3 || lk == 0 || lk == 1 || lk == 2 || lk == 0xb;
    Window_SendEvent5(w, 0, 0x79, 0x1f45, 0x4dcfb0, 0, 0);
    if (save) {
        Window_SendEvent5(g_6d0810, 0, 0x6f, 0x1f42, (uint32_t)(uintptr_t)GetString(0xf9), 0, 0);
        Window_SendEvent5(g_6d0810, 0, 0x6e, 0x1f42, (uint32_t)(uintptr_t)GetString(0xfa), 0, 0);
    } else {
        Window_SendEvent5(g_6d0810, 0, 0x6f, 0x1f42, (uint32_t)(uintptr_t)GetString(0xfb), 0, 0);
        Window_SendEvent5(g_6d0810, 0, 0x6e, 0x1f42, (uint32_t)(uintptr_t)GetString(0xfc), 0, 0);
        Window_SendEvent5(g_6d0810, 0, 0x67, 0x1f45, 0, 0, 0);
        Window_SendEvent5(g_6d0810, 0, 0xa4, 0x1f44, 0, 0, 0);
        Window_SendEvent5(g_6d0810, 0, 0x6e, 0x1f44, (uint32_t)(uintptr_t)GetString(0xfd), 0, 0);
    }
    PauseLock_Push(hook::As<int32_t*>(g_app));
    Ui_RunModal(hook::As<uint8_t**>(g_windowManager), 0, g_6d0810, 1, 0, 0x14, save ? 0x1f44 : 0);
    PauseLock_Pop(hook::As<int32_t*>(g_app));
    Windows_Remove(g_windowManager, 0, g_6d0810);
    if (uint8_t* p = g_6d0810) {
        F<uint32_t>(p, 0) = 0x5d0e34;
        if (uint8_t* list = g_6d0814) {
            Obj_FreeBuffer8(list);
            crt::operator_delete(list);
        }
        g_6d0814 = nullptr;
        Widget_SetVtbl(p);
        crt::operator_delete(p);
    }
    uint8_t* view = g_gameWindow;
    g_6d0810 = nullptr;
    if (!view || (mode == F<int32_t>(view, 0x8c) && F<int32_t>(g_cursorManager, 4) != 6))
        Cursor_Set(g_cursorManager, 0, cursor, 0, 0);
    if (!hook::As<uint32_t>(g_6d0804)) return 0;
    if (Options_Check1bf(g_app)) {
        strcat(name, (const char*)0x5ca9a4);
        return hook::As<uint32_t>(g_6d0804);
    }
    char base[0x310], path[0x320];
    strcpy(base, name);
    strcat(name, ((const char**)0x5f3d58)[kind]);
    if (!save) {
        FormatText(path, (const char*)0x5c9d00, Options_SaveFolder(g_app), name, 0, 0, 0, 0, 0, 0);
        if (!File_Exists(path)) {
            FormatText(path, (const char*)0x5d0e50, Options_SaveFolder(g_app), base, 0, 0, 0, 0, 0, 0);
            if (File_Exists(path)) {
                strcpy(name, base);
                strcat(name, (const char*)0x5ca9c4);
            }
        }
    }
    return hook::As<uint32_t>(g_6d0804);
}
RT3_REPLACE(0x4dd010, FileDialog_Run);

// 0x4dd390. Options dialog: sets the lighting set toggle (+0x95, button 0x833,
// refreshing 0x837); with `apply` it is stored in options +0x2c3, saved, and
// the day lighting rebuilt.
RT3_NOINLINE void __fastcall OptionsDialog_SetLightingSet(uint8_t* d, int, uint8_t on, uint8_t apply) {
    d[0x95] = on;
    Window_SendEvent5(d, 0, on ? 0x6d : 0x86, 0x833, 1, 0, 0);
    Window_SendEvent5(d, 0, 0xb8, 0x837, 0xc4, 0, 0);
    if (!apply) return;
    F<int32_t>(g_app, 0x2c3) = d[0x95];
    Options_Save(g_app, 0, 1);
    Lighting_Rebuild(g_app);
}
RT3_REPLACE(0x4dd390, OptionsDialog_SetLightingSet);

// 0x4dd410. Highlights the map-mode buttons of window w by its mode +0x8c (field
// 0x20): 0x7db 12, 0x7e0 9, 0x7de 11, 0x7f3 8 (only for the player's own company,
// +0x90), 0x7dc 6, 0x7e1 7, 0x7e2 4, 0x7e3 1, 0x7ef 13, 0x7f0 14..19, 0x7f1 20,
// 0x7f2 21.
void __fastcall Window_HighlightMapModes(uint8_t* w) {
    auto t = [w](int32_t x) { return F<int32_t>(w, 0x8c) == x ? 0x65u : 0x87u; };
    Window_SendEvent5(w, 0, t(0xc), 0x7db, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(9), 0x7e0, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0xb), 0x7de, 0x20, 0, 0);
    bool own = F<int32_t>(w, 0x8c) == 8 && F<int32_t>(w, 0x90) == Obj_Get21(g_world);
    Window_SendEvent5(w, 0, own ? 0x65 : 0x87, 0x7f3, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(6), 0x7dc, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(7), 0x7e1, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(4), 0x7e2, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(1), 0x7e3, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0xd), 0x7ef, 0x20, 0, 0);
    int32_t m = F<int32_t>(w, 0x8c);
    Window_SendEvent5(w, 0, m >= 0xe && m <= 0x13 ? 0x65 : 0x87, 0x7f0, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0x14), 0x7f1, 0x20, 0, 0);
    Window_SendEvent5(w, 0, t(0x15), 0x7f2, 0x20, 0, 0);
}
RT3_REPLACE(0x4dd410, Window_HighlightMapModes);

// 0x4dd630. Cursor for edit tool +0x8c: 6 -> 2, 13 -> 0x15, 14..19 -> 9, 20 -> 0x16,
// 21 / 23 / 26 -> 8, 22 -> 0xc, 12 keeps the cursor, anything else -> 1.
void __fastcall EditTool_UpdateCursor(uint8_t* t) {
    int32_t idx;
    switch (F<int32_t>(t, 0x8c)) {
    case 6: idx = 2; break;
    case 12: return;
    case 13: idx = 0x15; break;
    case 14: case 15: case 16: case 17: case 18: case 19: idx = 9; break;
    case 20: idx = 0x16; break;
    case 21: case 23: case 26: idx = 8; break;
    case 22: idx = 0xc; break;
    default: idx = 1; break;
    }
    Cursor_Set(g_cursorManager, 0, idx, 0, 0);
}
RT3_REPLACE(0x4dd630, EditTool_UpdateCursor);

// 0x4dd710. Status bar field 0x805: the time of day in sandbox mode (+0x4aeb),
// the scene clock in the editor, else the player's company cash (red at
// -500 or less; blank without a company or while editing +0x4aef).
RT3_NOINLINE void __fastcall StatusBar_ShowCash(uint8_t* w) {
    char text[0x130];
    uint32_t color = 0xff2cba24;
    uint8_t* world = g_world;
    if (F<int32_t>(world, 0x4aeb)) {
        uint8_t d[8] = {};
        *(uint16_t*)d = 0x726;
        d[2] = 1;
        d[4] = 1;
        int32_t h, m, s;
        Clock_SplitD((uint8_t*)Engine_Renderer(g_engine), 0, &h, &m, &s);
        d[5] = (uint8_t)h;
        d[6] = (uint8_t)m;
        Date_Format(text, d, 4);
    } else if (g_newGame[0x82]) {
        int32_t h, m, s;
        Clock_SplitD((uint8_t*)Engine_Renderer(g_engine), 0, &h, &m, &s);
        crt::sprintf(text, (const char*)0x5d0e68, h, m, s);
    } else if (Obj_Company21(world) && !F<int32_t>(g_world, 0x4aef)) {
        double cash = Company_Stat((uint8_t*)Obj_Company21(g_world), 0, 0xd, 0x2329);
        Num_FormatSimple2(text, 0xb, cash, 9);
        if (cash < D(0x5d0e60)) color = 0xffff6565;
    } else {
        FormatText(text, (const char*)0x5c87a8, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    Window_SendEvent5(w, 0, 0x6f, 0x805, (uint32_t)(uintptr_t)text, 0, 0);
    Window_SendEvent5(w, 0, 0xa7, 0x805, color, 0, 0);
}
RT3_REPLACE(0x4dd710, StatusBar_ShowCash);

// 0x4dd8a0. Stores `v` at +0x90 and tells the attached window (+0x88), if any
// (event 0x3ea).
RT3_NOINLINE void __fastcall Panel_SetValue90(uint8_t* o, int, int32_t v) {
    F<int32_t>(o, 0x90) = v;
    if (uint8_t* w = F<uint8_t*>(o, 0x88)) Window_SendEvent5(w, 0, 0x3ea, 0, 0, 0, 0);
}
RT3_REPLACE(0x4dd8a0, Panel_SetValue90);

// 0x4dd8d0. Enables (0x6d) or disables (0x86) control 0x7f9 of window w; returns
// its child 0x808.
RT3_NOINLINE uint8_t* __fastcall Window_EnableControl7f9(uint8_t* w, int, uint8_t on) {
    Window_SendEvent5(w, 0, on ? 0x6d : 0x86, 0x7f9, 3, 0, 0);
    return Widget_FindChild(w, 0, 0x808);
}
RT3_REPLACE(0x4dd8d0, Window_EnableControl7f9);

// 0x4dd910. Map cell of the quarter-resolution coordinates (x/4, y/4).
RT3_NOINLINE uint8_t* __fastcall Map_CellAtQuarter(uint8_t* m, int, int32_t x, int32_t y) {
    return F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * (y / 4) + x / 4) * 0x1d7;
}
RT3_REPLACE(0x4dd910, Map_CellAtQuarter);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4dd950. Unregister the active tool, dispatch its mode-specific destructor,
// and free it with the game's delete. Modes 3/24/25 and unknown modes only
// clear the slot. Unregister may change +0x88, so reread it before destruction.
RT3_NOINLINE void __fastcall View_DestroyToolForMode(uint8_t* view, int, uint32_t mode) {
    void* tool = F<void*>(view, 0x88);
    if (!tool) return;
    Windows_Remove((uint8_t*)(hook::As<void*>(g_windowManager)), 0, (uint8_t*)(tool));
    static const uint32_t destroyers[27] = {
        0x53f7d0, 0x5157c0, 0x512f10, 0, 0x506a80, 0x505bf0, 0x507b50,
        0x4c6ae0, 0x4c2320, 0x4f3b30, 0x4b3520, 0x50bac0, 0x50db10, 0x4b8440,
        0x4bc1d0, 0x4bc1d0, 0x4bc1d0, 0x4bc1d0, 0x4bc1d0, 0x4bc1d0,
        0x4bcbf0, 0x4f5ee0, 0x4fcb40, 0x4f50d0, 0, 0, 0x4f5750
    };
    if (mode < 27 && destroyers[mode]) {
        tool = F<void*>(view, 0x88);
        if (tool) {
            // Exe-address calls use the installed replacements when enabled.
            Method(destroyers[mode], tool);
            crt::operator_delete(tool);
        }
    }
    F<void*>(view, 0x88) = nullptr;
}
RT3_REPLACE(0x4dd950, View_DestroyToolForMode);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4ddbd0. Change the view's tool and selection. Construction/destruction
// ordering differs for track and terrain tools; callbacks can change the mode,
// so reread +0x8c at the same decision points as the original.
RT3_NOINLINE void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested) {
    if (F<uint32_t>(view, 0x8c) == 25 && mode != 25) World_SetEditorButtons(0, 0);
    int32_t oldSelection = F<int32_t>(view, 0x90);
    bool selectionChanged = oldSelection != requested;
    int32_t selection = requested;
    uint32_t initialMode = F<uint32_t>(view, 0x8c);
    if (initialMode == mode && requested == -1) selection = oldSelection;
    F<int32_t>(view, 0x90) = selection;
    if (initialMode == mode && mode == 8 && selection != oldSelection && hook::As<void*>(g_6cfe4c))
        Method(0x4c2ca0, hook::As<void*>(g_6cfe4c));
    uint32_t oldMode = F<uint32_t>(view, 0x8c);
    if (oldMode == mode) {
        Method(0x4dd410, view);
        Call(0x4d44f0);
        if (F<uint32_t>(view, 0x8c) == 2) Call(0x514410);
        else if (F<uint32_t>(view, 0x8c) == 9) {
            TrackPicker_SetTab((uint8_t*)(hook::As<void*>(g_6d12f0)), 0, selection, -1);
            if (hook::As<uint32_t>(g_6d3b4c))
                TrackPicker_SetTab((uint8_t*)(hook::As<void*>(g_6d12f0)), 0, 0, -1);
        }
        if (selectionChanged && F<void*>(view, 0x88)) Event(F<void*>(view, 0x88), 0x3ea, 0, 0, 0, 0);
        return;
    }
    F<uint32_t>(view, 0x8c) = mode;
    switch (mode) {
    case 2: F<int32_t>(view, 0x78) = selection; break;
    case 5: F<int32_t>(view, 0x80) = selection; break;
    case 8: F<int32_t>(view, 0x7c) = selection; break;
    case 10: F<int32_t>(view, 0x84) = selection; break;
    }
    bool destroyFirst = oldMode == 5 || oldMode == 6 || oldMode == 10 || oldMode == 12 ||
                        (oldMode >= 14 && oldMode <= 19) || (mode >= 14 && mode <= 19);
    if (destroyFirst) View_DestroyToolForMode(view, 0, oldMode);
    if (F<uint32_t>(view, 0x8c) == 9 && selection != -1) g_6d12d8 = selection;
    struct Constructor { uint32_t bytes, address; };
    static const Constructor constructors[27] = {
        {0, 0}, {0x7c, 0x515f50}, {0x7c, 0x514420}, {0, 0},
        {0x7c, 0x5074c0}, {0x84, 0x5068c0}, {0x98, 0x509d80},
        {0x7c, 0x4c7200}, {0x7c, 0x4c5540}, {0x80, 0x4f5020},
        {0x7d, 0x4b81d0}, {0x78, 0x50c290}, {0xb0, 0x50e400},
        {0x80, 0x4b8720}, {0x8c, 0x4bcb50}, {0x8c, 0x4bcb50},
        {0x8c, 0x4bcb50}, {0x8c, 0x4bcb50}, {0x8c, 0x4bcb50},
        {0x8c, 0x4bcb50}, {0x98, 0x4bcdd0}, {0x200, 0x4f7ce0},
        {0x84, 0x4fcf30}, {0x84, 0x4f54d0}, {0, 0}, {0, 0}, {0x90, 0x4f5670}
    };
    uint8_t* tool = nullptr;
    uint32_t current = F<uint32_t>(view, 0x8c);
    if (current < 27 && constructors[current].address) {
        const Constructor& ctor = constructors[current];
        void* memory = crt::operator_new(ctor.bytes);
        if (memory) tool = hook::Orig<uint8_t*(__fastcall*)(void*)>(ctor.address)(memory);
    }
    if (!destroyFirst) View_DestroyToolForMode(view, 0, oldMode);
    Call(0x4557b0);
    F<uint8_t*>(view, 0x88) = tool;
    if (tool) WidgetList_Insert((uint8_t**)(hook::As<void*>(g_windowManager)), 0, tool, 0x1e);
    Method(0x4dd410, view);
    Call(0x4d44f0);
    Method(0x4dd630, view);
    if (oldMode == 12 || F<uint32_t>(view, 0x8c) == 12) Method(0x493150, hook::As<void*>(g_tracks));
}
RT3_REPLACE(0x4ddbd0, View_SetToolMode);

// 0x4de0c0. Whether the screen (+0x8c) may close: map-type screens close
// at once; others first let the renderer's active overlay (+0x366e) finish.
RT3_NOINLINE int32_t __fastcall Screen_CanClose(uint8_t* s, int, int32_t) {
    uint32_t i = (uint32_t)(F<int32_t>(s, 0x8c) - 6);
    if (i <= 0x14 && !(i >= 1 && i <= 5) && i != 18 && i != 19) return 1;
    uint8_t* r = (uint8_t*)Engine_Renderer(hook::As<void*>(g_engine));
    void* p = F<void*>(r, 0x366e);
    if (p && ((int32_t(__thiscall*)(void*))(*(void***)p)[0x1c / 4])(p)) {
        ((void(__thiscall*)(void*))(*(void***)p)[0x5c / 4])(p);
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4de0c0, Screen_CanClose);

 // namespace

// 0x4de140 (event handler). The main game view d (tool mode +0x8c).
// Buttons (0xca): the screen close 0x7d6, the tool toggles (stock 0x7dc,
// company 0x7db, stock market 0x7de, 0x7e0 .. 0x7e3, 0x7ef, the company
// detail 0x7f3, founding one when the player has none (string 0xbcd) and the
// main view report modes 0x7f0 .. 0x7f2), the game speeds 0x80c..0x811 (the
// buttons reflect the world speed +0x19 when refused); in a tutorial
// (0x6d3b4c) only the button it waits for (0x622b3c) works, others tell
// 0xe03. Buttons (0xcb): the train buyer 0x7dd, the file menu 0x7df, the
// ledger 0x7e5, camera turns / zooms 0x7fb..0x7fe and the lighting toggle
// 0x835. Escape leaves the tool or opens the file menu. Mouse moves over
// the map (0x7d6) show the cell tooltip of the overlay (heights, colours,
// height, city / region names, cargo supply and demand, the region with
// several trains) and, for the cheat / debug views (+0x70 / +0x184 of the
// options, 0x6d07b4), the cell position and the terrain / region details.
int32_t __fastcall GameView_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    uint8_t* w = g_world;
    auto mode = [&](uint32_t m, int32_t req) {
        View_SetToolMode(d, 0, m, req);
        return 0;
    };
    auto keep = [&]() { return mode(F<uint32_t>(d, 0x8c), -1); };
    // Toggles tool m: back to 0 when already in it.
    auto toggle = [&](uint32_t m) { return mode(F<uint32_t>(d, 0x8c) == m ? 0 : m, -1); };
    auto speedButtons = [&](uint32_t a, uint32_t b) {
        Window_SendEvent5(d, 0, 0x74, F<uint32_t>(g_world, 0x19) + 0x80c, 1, a, b);
    };
    switch (msg[0]) {
    case 0xca: {
        const uint32_t id = msg[1];
        if (g_6d3b4c &&
            (id == 0x7db || id == 0x7dc || id == 0x7de || (id >= 0x7e0 && id <= 0x7e3) || id == 0x7f3 || (id >= 0x80c && id <= 0x811)) &&
            id != g_622b3c) {
            View_SetToolMode(d, 0, F<uint32_t>(d, 0x8c), -1);
            speedButtons(0, 0);
            Message480(0xe03, 1);
            return 0;
        }
        switch (id) {
        case 0x7d6: return Screen_CanClose(d, 0, (int32_t)(uintptr_t)msg);
        case 0x7db:
            if (F<uint32_t>(d, 0x8c) == 0xc) return mode(0, -1);
            if (g_6cd8e8) return keep(), 1;
            return Player_CheckHasCompany(g_world) ? mode(0xc, -1) : keep();
        case 0x7dc:
            if (F<uint32_t>(d, 0x8c) == 6) return mode(0, -1);
            if (g_6cd8e8) return keep(), 1;
            return Player_CheckStockAllowed(g_world) ? mode(6, -1) : keep();
        case 0x7de:
            if (F<uint32_t>(d, 0x8c) == 0xb) return mode(0, -1);
            return StockMarket_IsAllowed() ? mode(0xb, -1) : keep();
        case 0x7e0: return toggle(9);
        case 0x7e1:
            if (F<uint32_t>(d, 0x8c) == 7) return mode(0, -1);
            return Scenario_CheckEditable() ? mode(7, -1) : keep();
        case 0x7e2: return toggle(4);
        case 0x7e3: return toggle(1);
        case 0x7ef: return toggle(0xd);
        case 0x7f0: {
            uint8_t* view = g_gameWindow;
            const uint32_t m = F<uint32_t>(view, 0x8c);
            View_SetToolMode(view, 0, m >= 0xe && m <= 0x13 ? 0 : 0xe, -1);
            return 0;
        }
        case 0x7f1:
        case 0x7f2: {
            uint8_t* view = g_gameWindow;
            const uint32_t m = id == 0x7f1 ? 0x14 : 0x15;
            View_SetToolMode(view, 0, F<uint32_t>(view, 0x8c) == m ? 0 : m, -1);
            return 0;
        }
        case 0x7f3: {
            if (F<uint32_t>(d, 0x8c) == 8 && F<int32_t>(d, 0x90) == Obj_Get21(g_world)) return mode(0, -1);
            if (!Scenario_CheckEditable()) return keep();
            if (!Obj_Get21(g_world)) {
                if (Player_CheckCanFound((uint8_t*)Obj_Ref25(g_world)) && Message480(0xbcd, 3) == 0x3f2)
                    Player_FoundCompanyDialog();
                if (!Obj_Get21(g_world)) return keep();
            }
            return mode(8, Obj_Get21(g_world));
        }
        case 0x80c:
        case 0x80d:
        case 0x80e:
        case 0x80f:
        case 0x810:
        case 0x811:
            if (g_6cd924) {
                speedButtons(0, 0);
                return 0;
            }
            if (!World_SetSpeed(g_world, 0, (int32_t)id - 0x80c, 1, 1)) speedButtons(0, 0);
            return 0;
        }
        return 1;
    }
    case 0xcb: {
        const uint32_t id = msg[1];
        if (g_6d3b4c && id == 0x7dd && g_622b3c != 0x7dd) {
            View_SetToolMode(d, 0, F<uint32_t>(d, 0x8c), -1);
            Message480(0xe03, 1);
            return 0;
        }
        switch (id) {
        case 0x7dd:
            if (g_6cd8e8) return keep(), 1;
            if (Player_CheckCanRunTrains(g_world)) ConsistEditor_Run(nullptr, nullptr, nullptr, 0xffffffff, 0xffffffff, 0, 0, 0, 0xffffffff);
            return 0;
        case 0x7df:
            FileMenu_Run();
            return 0;
        case 0x7e5:
            Ledger_Open(1);
            return 0;
        case 0x7fb:
        case 0x7fc:
            Camera_SnapTurn(g_62be68, 0, id == 0x7fb ? -1 : 1);
            return 0;
        case 0x7fd:
        case 0x7fe:
            Camera_ZoomStep(g_62be68, 0, id == 0x7fd ? -1 : 1);
            return 0;
        case 0x835:
            OptionsDialog_SetLightingSet(d, 0, (uint8_t)(1 - d[0x95]), 1);
            return 1;
        }
        return 1;
    }
    case 3:
        if (msg[2] != 0x1b) return 1;
        if (F<uint32_t>(d, 0x8c)) {
            if (!g_6d3b4c) View_SetToolMode(d, 0, 0, -1);
            return 0;
        }
        if (F<int32_t>(g_windowManager, 0xc64) > 0) return 1;
        FileMenu_Run();
        return 0;
    case 2:
        break;
    default:
        return 1;
    }
    // Mouse move: the overlay kinds of the report mode 0x15 (0x6d1304).
    const uint32_t tool = F<uint32_t>(d, 0x8c);
    uint8_t* overlay = g_6d1304;
    const bool heights = tool == 0x15 && overlay && F<int32_t>(overlay, 0x8c) == 1;
    const bool layers = tool == 0x15 && overlay && F<int32_t>(overlay, 0x90) == 3;
    const bool colours = tool == 0x15 && overlay && F<int32_t>(overlay, 0x90) == 1;
    if (msg[1] != 0x7d6 || !hook::As<void*>(g_engine)) return 1;
    float y, x;
    const int32_t focus = Scene_FocusCell((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, &x, &y);
    uint8_t* map = g_map;
    const bool inside = focus == 1 && x >= 0.0f && y >= 0.0f && x < (float)F<int32_t>(map, 0x2155) && y < (float)F<int32_t>(map, 0x2159);
    const bool here = focus && inside;
    // The truncated cell of the focus (y converted first, as the original).
    auto ix = [&]() { return (int32_t)x; };
    auto iy = [&]() { return (int32_t)y; };
    char text[0x7d0], a[0x100], b[0x100], c[0x100], e[0x100], s1[0x40], s2[0x40];
    auto tooltip = [&]() { Ui_ShowTooltip(g_windowManager, 0, text, 1, 0, -1); };
    bool tip = true;
    if (layers) {
        if (here) {
            char dd[0x100];
            auto layer = [&](char* buf, int32_t k) {
                const int32_t cy = iy(), cx = ix();
                Num_Format(buf, 5, Grid_Darkness(g_map, 0, cx, cy, k), 0xa, 1, 0, 0);
            };
            layer(a, 3);
            layer(b, 2);
            layer(c, 1);
            layer(dd, 0);
            FormatText(text, GetString(heights ? 0xb32 : 0xafa), dd, c, b, a, nullptr, nullptr, nullptr, nullptr);
        } else {
            text[0] = 0;
        }
    } else if (colours) {
        if (here) {
            char dd[0x100];
            const uint32_t v = (uint32_t)Overlay_ValueAt(g_6d1304, 0, x, y);
            Num_Format(b, 1, (double)(v & 0xff), 0xa, 1, 0, 0);
            Num_Format(c, 1, (double)(v >> 8 & 0xff), 0xa, 1, 0, 0);
            Num_Format(dd, 1, (double)(v >> 16 & 0xff), 0xa, 1, 0, 0);
            FormatText(text, GetString(heights ? 0xb33 : 0xafb), dd, c, b, nullptr, nullptr, nullptr, nullptr, nullptr);
        } else {
            text[0] = 0;
        }
    } else if (F<int32_t>(map, 0x2171) == 6) {
        if (here) {
            Num_Format(b, 1, Terrain_HeightAtTile(map, 0, x, y), 0xa, 1, 0, 0);
            FormatText(text, GetString(0x203), b, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        } else {
            text[0] = 0;
        }
    } else if (F<int32_t>(map, 0x2175) == 0x10 || F<int32_t>(map, 0x2175) == 0x1e) {
        crt::sprintf(text, (const char*)0x5c87a8);
        if (here) {
            const int32_t ky = iy(), kx = ix();
            if (!Terrain_KindIs1Or3to5(g_map, 0, kx, ky)) {
                if (F<int32_t>(map, 0x2175) == 0x10) {
                    const int32_t cy = iy(), cx = ix();
                    const uint16_t city = (uint16_t)Map_CellCity(g_map, 0, cx, cy);
                    if (uint8_t* ct = (uint8_t*)Collection_Get(g_cities, 0, city)) crt::sprintf(text, (const char*)ct + 0x356);
                } else {
                    crt::sprintf(text, (const char*)Map_RegionAt(g_map, 0, x, y) + 4);
                }
            }
        }
    } else if (F<int32_t>(map, 0x2175) == 3) {
        const int32_t cargo = F<int32_t>(map, 0x2179);
        if (here) {
            const int32_t cy = iy(), cx = ix();
            uint8_t* cell = Map_CellAtQuarter(map, 0, cx, cy);
            if (cargo && !F<int32_t>(g_world, 0x4af7)) {
                if (F<int32_t>(hook::As<uint8_t*>(g_cargoTypes), 0x9a + cargo * 4) != -1) {
                    crt::sprintf(s1, (const char*)0x5d0eb0, (double)Station_TotalAvailable(cell));
                    FormatText(text, GetString(0x205), s1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                    crt::sprintf(s1, (const char*)0x5d0eb0, (double)F<float>(cell, 0x103 + cargo * 4));
                    uint8_t* cg = (uint8_t*)Collection_Get(g_cargoTypes, 0, (uint32_t)cargo);
                    const char* name = LocalizeName((const char*)cg + 4);
                    FormatText(text, GetString(0x206), name, s1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                    strcat(text, GetString(0xbce));
                } else {
                    const float quota = (float)City_CargoQuota(cell, 0, cargo);
                    crt::sprintf(s1, (const char*)0x5d0eb0, (double)F<float>(cell, 0x103 + cargo * 4));
                    uint8_t* cg = (uint8_t*)Collection_Get(g_cargoTypes, 0, (uint32_t)cargo);
                    const char* name = LocalizeName((const char*)cg + 4);
                    Num_FormatSimple(e, 9, (double)(int32_t)(quota / BitsF(0x447a0000)), 0xb);
                    FormatText(text, GetString(0x207), name, e, s1, nullptr, nullptr, nullptr, nullptr, nullptr);
                }
            } else {
                crt::sprintf(s1, (const char*)0x5d0eb0, (double)Station_TotalAvailable(cell));
                FormatText(text, GetString(0x204), s1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            }
        } else {
            text[0] = 0;
        }
    } else {
        // Region names only when several trains are on the map.
        tip = false;
        if (!hook::As<int32_t>(g_trackTool) && !hook::As<int32_t>(g_buildingDialog)) {
            int32_t running = 0;
            for (int32_t i = 0; i < Collection_Count(g_territories); ++i)
                if (F<int32_t>(Table_At(g_territories, 0, i), 0x31) &&
                    F<int32_t>(Table_At(g_territories, 0, i), 0x3d))
                    ++running;
            if (running > 1) {
                tip = true;
                crt::sprintf(text, (const char*)0x5c87a8);
                if (here) {
                    const int32_t ky = iy(), kx = ix();
                    if (!Terrain_KindIs1Or3to5(g_map, 0, kx, ky))
                        crt::sprintf(text, (const char*)Map_RegionRecordAt(g_map, 0, x, y) + 4);
                }
            }
        }
    }
    if (tip) tooltip();
    // The status line of the cheat / debug views.
    uint8_t* opts = g_app;
    auto status = [&]() {
        Status_SetMessage(g_windowManager, 0, text);
        return 1;
    };
    static const int32_t* const kDx = (const int32_t*)0x624b28;
    static const int32_t* const kDy = (const int32_t*)0x624b48;
    if (!F<int32_t>(opts, 0x70) && !F<int32_t>(opts, 0x184)) {
        if (!Global6d07b4Set2()) return 1;
        if (!here) {
            text[0] = 0;
            return status();
        }
        crt::sprintf(s1, (const char*)0x5c9cd0, (int32_t)x);
        crt::sprintf(s2, (const char*)0x5c9cd0, (int32_t)y);
        FormatText(text, (const char*)0x5d0ea0, s1, s2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        // The nearest cell with terrain, rings of radius 0..4.
        for (int32_t k = 0; k <= 4; ++k)
            for (int32_t j = 0; j < 8; ++j) {
                const int32_t cx = (int32_t)((float)(kDx[j] * k) + x);
                const int32_t cy = (int32_t)((float)(kDy[j] * k) + y);
                uint8_t* m = g_map;
                if (cx < 0 || cy < 0 || cx >= F<int32_t>(m, 0x2155) || cy >= F<int32_t>(m, 0x2159)) continue;
                if ((int32_t)Terrain_CellCount(m, 0, cx, cy) <= 0) continue;
                // QUIRK: the high byte is compared with -1 after zero extension (never true).
                const uint32_t high = (uint8_t)Map_CellHigh(g_map, 0, cx, cy);
                FormatText(e, (const char*)0x5d0e9c, GetString(*(const int32_t*)(0x5ef3e8 + high * 0xb0)), nullptr, nullptr, nullptr, nullptr, nullptr,
                           nullptr, nullptr);
                strcat(text, e);
                return status();
            }
        return status();
    }
    if (!focus) {
        crt::sprintf(text, GetString(0x208));
        return status();
    }
    if (!inside) {
        crt::sprintf(s2, (const char*)0x5d0e94, (double)x);
        crt::sprintf(s1, (const char*)0x5d0e94, (double)y);
        FormatText(text, GetString(0x209), s2, s1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        return status();
    }
    if (F<int32_t>(opts, 0x184)) {
        // Region grid cell (4 cells each) and its eight neighbour counts.
        const float qx = x / BitsF(0x40800000);
        const int32_t bx = (int32_t)qx;
        const int32_t by = (int32_t)(y / BitsF(0x40800000));
        crt::sprintf(s2, (const char*)0x5d0e94, (double)qx);
        crt::sprintf(s1, (const char*)0x5d0e94, (double)(y / BitsF(0x40800000)));
        FormatText(text, (const char*)0x5d0e8c, s2, s1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        uint8_t* m = g_map;
        if (bx < 0 || by < 0 || bx >= F<int32_t>(m, 0x2145) || by >= F<int32_t>(m, 0x2149)) return status();
        for (int32_t n = 0; n < 8; ++n) {
            const int32_t ax = kDx[n] + bx, ay = kDy[n] + by;
            m = g_map;
            if (ax < 0 || ay < 0 || ax >= F<int32_t>(m, 0x2145) || ay >= F<int32_t>(m, 0x2149)) continue;
            // QUIRK: reads the centre cell's counts for every direction.
            crt::sprintf(e, (const char*)0x5d0e80, n, (uint32_t)F<uint16_t>(Grid_RegionAt(m, 0, bx, by), 0xf3 + n * 2));
            strcat(text, e);
        }
        return status();
    }
    crt::sprintf(s2, (const char*)0x5d0e94, (double)x);
    crt::sprintf(s1, (const char*)0x5d0e94, (double)y);
    FormatText(text, (const char*)0x5d0e78, s2, s1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    auto kind = [&](int32_t(__fastcall * f)(uint8_t*, int, int32_t, int32_t)) {
        const int32_t ky = iy(), kx = ix();
        return f(g_map, 0, kx, ky) != 0;
    };
    uint32_t sid;
    if (kind(Terrain_KindIs1Or4) && kind(Terrain_KindIs2Or4Or5))
        sid = 0x20a;
    else if (kind(Terrain_KindIs3Or5) && kind(Terrain_KindIs2Or4Or5))
        sid = 0x20b;
    else if (kind(Terrain_KindIs1Or4))
        sid = 0x20c;
    else if (kind(Terrain_KindIs3Or5))
        sid = 0x20d;
    else if (kind(Terrain_KindIs2Or4Or5))
        sid = 0x20e;
    else
        sid = 0x20f;
    strcat(text, GetString(sid));
    return status();
}
RT3_REPLACE(0x4de140, GameView_HandleEvent);

// 0x4df9e0. Status bar date (field 0x804) by options +0xa4: 2 the scene clock
// "hh:mm:ss", 1 the full date, else in sandbox mode (+0x4aeb) a calendar of
// 12 x 28 days running at half speed from the start year (+5) with the day
// offset +0x4b43 (a new day from 6 o'clock), otherwise "month year".
RT3_NOINLINE void __fastcall StatusBar_ShowDate(uint8_t* w) {
    char text[0x130];
    int32_t mode = F<int32_t>(g_app, 0xa4);
    uint8_t* world = g_world;
    if (mode == 2) {
        int32_t h, m, s;
        Clock_SplitD((uint8_t*)Engine_Renderer(g_engine), 0, &h, &m, &s);
        crt::sprintf(text, (const char*)0x5d0e68, h, m, s);
    } else if (mode == 1) {
        Date_Format(text, world + 0xd, 5);
    } else if (F<int32_t>(world, 0x4aeb)) {
        uint8_t d[8] = {};
        uint16_t start = F<uint16_t>(world, 5);
        int32_t day = (F<uint16_t>(world, 0xd) - start) / 2 + F<int32_t>(world, 0x4b43);
        *(uint16_t*)d = start;
        int32_t h, m, s;
        Clock_SplitD((uint8_t*)Engine_Renderer(g_engine), 0, &h, &m, &s);
        if (h <= 5) ++day;
        if (day > 0x150) {
            uint32_t n = (uint32_t)(day - 0x151) / 0x150 + 1;
            day -= (int32_t)n * 0x150;
            *(uint16_t*)d = (uint16_t)(*(uint16_t*)d + n);
        }
        d[2] = (uint8_t)(day / 0x1c + 1);
        d[4] = (uint8_t)(day % 0x1c + 1);
        Date_Format(text, d, 5);
    } else {
        Date_Format(text, world + 0xd, 1);
    }
    Window_SendEvent5(w, 0, 0x6f, 0x804, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4df9e0, StatusBar_ShowDate);

// 0x4dfb50 (hover callback). Shows the year (world +5) with string 0x210
// as a tooltip (not in the editor).
void __fastcall Year_TooltipCb(void*, int, int32_t, int32_t) {
    if (F<int32_t>(g_app, 0x68)) return;
    char num[0x40], text[0x190];
    const int32_t year = F<uint16_t>(g_world, 5);
    Num_Format(num, 8, (double)year, 10, 1, 0, 0);
    FormatText(text, GetString(0x210), num, 0, 0, 0, 0, 0, 0, 0);
    Ui_ShowTooltip(g_windowManager, 0, text, 1, 1, -1);
}
RT3_REPLACE(0x4dfb50, Year_TooltipCb);

// 0x4dfbe0. Main game window constructor ("Game.win", vtable 0x5d0eb8, kept in
// 0x6d0818): game speed button 0x80c + speed ticked, view state +0x78..+0x88 and
// +0x8c cleared, +0x94 = 1, +0x95 = options +0x2c3 (button 0x833), 0x837 gets
// 0xc4, the scenario buttons 0x7d1 / 0x7d2 by new-game +0x82, 0x7f9 by options
// +0x283, handlers 0x4dfb50 (0x804) and 0x43f640 (0x7d7), and an empty tooltip on
// 0x805 in scenarios or when world +0x4aeb is set.
RT3_NOINLINE uint8_t* __fastcall GameWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0eb8;
    w[0x94] = 1;
    F<int32_t>(w, 0x8c) = 0;
    F<int32_t>(w, 0x88) = 0;
    g_gameWindow = w;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d0ec4);
    Window_SendEvent5(w, 0, 0x74, F<uint32_t>(WorldG(), 0x19) + 0x80c, 1, 0, 0);
    F<int32_t>(w, 0x78) = 0;
    F<int32_t>(w, 0x7c) = 0;
    F<int32_t>(w, 0x80) = 0;
    F<int32_t>(w, 0x84) = 0;
    uint8_t opt = g_app[0x2c3];
    w[0x95] = opt;
    Window_SendEvent5(w, 0, opt ? 0x6d : 0x86, 0x833, 1, 0, 0);
    Window_SendEvent5(w, 0, 0xb8, 0x837, 0xc4, 0, 0);
    bool scen = g_newGame[0x82] != 0;
    Window_SendEvent5(w, 0, scen ? 0x6d : 0x86, 0x7d2, 3, 0, 0);
    Window_SendEvent5(w, 0, scen ? 0x86 : 0x6d, 0x7d1, 3, 0, 0);
    Window_SendEvent5(w, 0, F<int32_t>(g_app, 0x283) ? 0x6d : 0x86, 0x7f9, 3, 0, 0);
    Window_SendEvent5(w, 0, 0x82, 0x804, 0x4dfb50, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x7d7, 0x43f640, 0, 0);
    if (g_newGame[0x82] || F<int32_t>(WorldG(), 0x4aeb))
        Window_SendEvent5(w, 0, 0x6e, 0x805, 0x5c87a8, 0, 0);
    return w;
}
RT3_REPLACE(0x4dfbe0, GameWindow_Construct);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4dfd70. Destroy the main view: clear its global reference, dispose of the
// active mode's tool, then destroy the base window.
RT3_NOINLINE void __fastcall View_Destroy(uint8_t* view) {
    F<uint32_t>(view, 0) = 0x5d0eb8;
    hook::As<uint32_t>(g_gameWindow) = 0;
    View_DestroyToolForMode(view, 0, F<uint32_t>(view, 0x8c));
    Method(0x53f7d0, view);
}
RT3_REPLACE(0x4dfd70, View_Destroy);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4dfda0. Clear editor/terrain/track tools, retaining non-editor view modes.
RT3_NOINLINE void __fastcall View_ResetEditorTool(uint8_t* view) {
    uint32_t mode = F<uint32_t>(view, 0x8c);
    if (mode == 0 || (mode >= 13 && mode <= 23) || mode == 26) View_SetToolMode(view, 0, 0, -1);
}
RT3_REPLACE(0x4dfda0, View_ResetEditorTool);

// 0x4dfdf0. Opens the game message panel ("GameMessage.win", 0x6d081c).
void GameMessagePanel_Open() { Panel_Open(0x6d081c, 0x94, 0x5d0ed0, (const char*)0x5d0edc, 1, false); }
RT3_REPLACE(0x4dfdf0, GameMessagePanel_Open);

 // namespace

// 0x4dfe50 (event handler). News list d: a click on item 0x8ca8..0x8cae
// (+0x78 items) plays the click and jumps to its subject by kind (+0x20e,
// ids +0x212 / +0x216): trains (0 / 1, train view and its lead car), sites
// (7, follow), buildings (9 / 17, building view), cities (10 / 16), stock
// events (14 / 15, the company list on that player's company), else just
// the click (18).
int32_t __fastcall NewsList_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    if (msg[0] != 0xca) return 1;
    const int32_t i = (int32_t)msg[1] - 0x8ca8;
    if (i >= 7 || i < 0) return 1;
    auto item = [&]() { return F<uint8_t*>(d, 0x78 + i * 4); };
    auto click = []() { Ui_PlaySound(0x21, 0, 0x3f000000); };
    switch (F<uint32_t>(item(), 0x20e)) {
    case 0:
    case 1: {
        click();
        const uint32_t id = F<uint32_t>(item(), 0x212);
        if (!Collection_IsValid(g_trains, 0, id)) return 0;
        uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, id);
        View_SetToolMode(g_gameWindow, 0, 2, (int32_t)id);
        if (Obj_City41(t)) Camera_FollowObject((uint8_t*)Obj_City41(t), 0);
        return 0;
    }
    case 7: {
        click();
        const uint32_t id = F<uint32_t>(item(), 0x212);
        if (Collection_IsValid(g_62b244, 0, id)) World_FollowEntity(g_world, 0, 1, 3, (int32_t)id);
        return 0;
    }
    case 9:
    case 17: {
        click();
        const uint32_t id = F<uint32_t>(item(), F<uint32_t>(item(), 0x20e) == 9 ? 0x212 : 0x216);
        if (!Collection_IsValid(g_buildings, 0, id)) return 0;
        uint8_t* b = (uint8_t*)Collection_Get(g_buildings, 0, id);
        View_SetToolMode(g_gameWindow, 0, 0xa, (int32_t)id);
        Camera_FollowObject(b, 0);
        return 0;
    }
    case 10:
    case 16: {
        click();
        const uint32_t id = F<uint32_t>(item(), F<uint32_t>(item(), 0x20e) == 10 ? 0x212 : 0x216);
        if (Collection_IsValid(g_cities, 0, id))
            Camera_FollowObject((uint8_t*)Collection_Get(g_cities, 0, id), 0);
        return 0;
    }
    case 14:
    case 15: {
        if (!Collection_IsValid(g_players, 0, F<uint32_t>(item(), 0x212))) return 0;
        if (!F<uint32_t>(Collection_Get(g_players, 0, F<uint32_t>(item(), 0x212)), 4)) return 0;
        if (!Collection_IsValid(g_companies, 0, F<uint32_t>(item(), 0x216))) return 0;
        if (!F<uint32_t>(Collection_Get(g_companies, 0, F<uint32_t>(item(), 0x216)), 0x3f)) return 0;
        click();
        View_SetToolMode(g_gameWindow, 0, 0xb, -1);
        if (!g_stockWindow) return 0;
        SetGlobal6d19f4(Table_ActiveIndexOf(g_players, 0, F<uint32_t>(item(), 0x212)));
        CompanyList_Select(g_stockWindow, 0, Table_LiveRankOf(g_companies, 0, F<uint32_t>(item(), 0x216)));
        return 0;
    }
    case 18:
        click();
        return 0;
    default:
        return 0;
    }
}
RT3_REPLACE(0x4dfe50, NewsList_HandleEvent);

// 0x4e0210. Refreshes the message panel `w`: up to 7 log entries (0x6acd34,
// oldest first) whose kind (+0x20e) the options show (+0x18c + kind) fill the
// slots +0x78 (first shown time +8 stamped). Each gets a text widget 0x8ca1 +
// i (text +0xd, colour by kind) stacked by its line count, and an icon 0x8ca8
// + i by kind: eye / note / smiley / computer images, the player's portrait
// (+0x212 in 0x6ceb9c; custom face 0x29 by name) or the company's logo
// (+0x212 in 0x62be10, else its name entry in pack +0x429b0). Unused slots
// are disabled. Items start lower when the camera +0x60 is set, further right
// with options +0x68.
RT3_NOINLINE void __fastcall MessagePanel_Refresh(uint8_t* w) {
    Table_MarkStale(g_messageLog);
    int32_t n = 0;
    uint8_t** slots = (uint8_t**)(w + 0x78);
    for (uint8_t* e = (uint8_t*)Table_Oldest(g_messageLog, 0, 0); e;) {
        slots[n] = e;
        if (g_app[0x18c + F<int32_t>(e, 0x20e)]) {
            if (!F<int32_t>(e, 8)) F<int32_t>(e, 8) = ClockMs();
            ++n;
        }
        e = (uint8_t*)Table_FindField4Next(g_messageLog, 0, F<int32_t>(e, 4));
        if (n >= 7) break;
    }
    float y = F<int32_t>(g_62be68, 0x60) ? BitsF(0x3dcccccd) : BitsF(0x3c088889);
    static const uint8_t colorIdx[0x12] = { 0, 1, 2, 3, 2, 1, 1, 1, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1 };
    static const uint32_t colors[4] = { 0xffff2200, 0xff00ff00, 0xffffff00, 0xffffffff };
    static const uint8_t iconIdx[0x13] = { 0, 0, 0, 1, 5, 2, 5, 0, 5, 0, 0, 5, 5, 5, 3, 3, 4, 4, 4 };
    static const uint32_t images[6] = { 0x5d0f2c, 0x5d0f14, 0x5d0f20, 0, 0, 0x5d0eec };
    for (int32_t j = 0; j < n; ++j) {
        uint8_t* e = slots[j];
        uint32_t id = 0x8ca8 + (uint32_t)j;
        uint8_t* tw = Widget_FindChild(w, 0, (int32_t)id - 7);
        Widget_SetFlags(tw, 0, 3, 0, 0);
        String_Assign(&F<char*>(tw, 0xba), (const char*)(e + 0xd));
        uint32_t k = (uint32_t)F<int32_t>(slots[j], 0x20e) - 1;
        F<uint32_t>(tw, 0x38) = k <= 0x11 ? colors[colorIdx[k]] : 0xffffffff;
        F<float>(tw, 0xf) = y;
        F<uint32_t>(tw, 0xb) = F<int32_t>(g_app, 0x68) ? 0x3e300000 : 0x3d400000;
        uint8_t* ic = Widget_FindChild(w, 0, (int32_t)id);
        Widget_SetFlags(ic, 0, 3, 0, 0);
        F<float>(ic, 0xf) = (float)((double)y + Fl(0x5d073c));
        F<uint32_t>(ic, 0xb) = F<int32_t>(g_app, 0x68) ? 0x3e033333 : 0x3b4ccccd;
        F<uint32_t>(ic, 0x100) = 0x3f800000;
        F<uint32_t>(ic, 0x104) = 0x3f800000;
        F<uint16_t>(ic, 0xae) = 0;
        F<uint32_t>(ic, 0x13) = 0x3d333333;
        F<uint32_t>(ic, 0x17) = 0x3ccccccd;
        uint32_t kind = (uint32_t)F<int32_t>(slots[j], 0x20e);
        int32_t which = kind <= 0x12 ? iconIdx[kind] : 5;
        if (which == 3) {
            uint8_t* p = nullptr;
            if (Collection_IsValid(g_players, 0, F<uint32_t>(slots[j], 0x212)))
                p = (uint8_t*)Collection_Get(g_players, 0, F<uint32_t>(slots[j], 0x212));
            Window_SendEvent5(w, 0, 0x75, id, 0x5d0f04, 0, 0);
            if (p) {
                F<uint16_t>(ic, 0xae) = p[0x15c];
                if (p[0x15c] == 0x29) {
                    F<uint16_t>(ic, 0xae) = 0;
                    Window_SendEvent5(w, 0, 0x75, id, (uint32_t)(uintptr_t)Obj_NamedEntry20(p), 0, 0);
                }
            }
            F<uint32_t>(ic, 0x100) = 0x3eb33333;
            F<uint32_t>(ic, 0x104) = 0x3ea8f5c3;
            F<uint32_t>(ic, 0x13) = 0x3d266666;
            F<float>(ic, 0xb) = (float)((double)F<float>(ic, 0xb) + Fl(0x5d0f00));
            F<uint32_t>(ic, 0x17) = 0x3cda740e;
            F<float>(ic, 0xf) = (float)((double)F<float>(ic, 0xf) - Fl(0x5d073c));
        } else if (which == 4) {
            Window_SendEvent5(w, 0, 0x75, id, 0x5c9f98, 0, 0);
            if (Collection_IsValid(g_companies, 0, F<uint32_t>(slots[j], 0x212)))
                F<uint16_t>(ic, 0xae) = F<uint16_t>(Collection_Get(g_companies, 0, F<uint32_t>(slots[j], 0x212)), 0x37);
            if (F<uint16_t>(ic, 0xae) == 0xffff) {
                F<uint16_t>(ic, 0xae) = 0;
                int32_t idx = Company_NameIndex(Collection_Get(g_companies, 0, F<uint32_t>(slots[j], 0x212)));
                Collection* names = F<Collection*>(g_pack, 0x429b0);
                void* name = Collection_Get(names, 0, Collection_IdAt(names, 0, (uint32_t)idx, 0));
                Window_SendEvent5(w, 0, 0x75, id, (uint32_t)(uintptr_t)name, 0, 0);
            }
            F<uint32_t>(ic, 0x100) = 0x3ea3d70a;
            F<uint32_t>(ic, 0x104) = 0x3e99999a;
            F<uint32_t>(ic, 0x13) = 0x3d266666;
            F<uint32_t>(ic, 0x17) = 0x3cda740e;
            F<float>(ic, 0xb) = (float)((double)F<float>(ic, 0xb) + Fl(0x5d0efc));
            F<float>(ic, 0xf) = (float)((double)F<float>(ic, 0xf) - Fl(0x5d082c));
        } else {
            Window_SendEvent5(w, 0, 0x75, id, images[which], 0, 0);
        }
        float th = (float)(Widget_TextLines(tw) * 0x21);
        th = (float)((double)th / Fl(0x5ca164));
        F<float>(tw, 0x17) = th;
        y = (float)((double)th + y);
    }
    for (int32_t j = n; j < 7; ++j) {
        Window_SendEvent5(w, 0, 0x87, 0x8ca1 + (uint32_t)j, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x87, 0x8ca8 + (uint32_t)j, 3, 0, 0);
    }
}
RT3_REPLACE(0x4e0210, MessagePanel_Refresh);

// 0x4e06d0 / 0x4e0df0. Close the panels in 0x6d081c / 0x6d0820.
void Panel6d081c_Close() { Panel_Close(0x6d081c, 0x5d0ed0, false); }
RT3_REPLACE(0x4e06d0, Panel6d081c_Close);

// 0x4e0720. Refreshes the message panel 0x6d081c when it is open.
RT3_NOINLINE void __fastcall MessagePanel_RefreshOpen() {
    if (uint8_t* p = g_6d081c) MessagePanel_Refresh(p);
}
RT3_REPLACE(0x4e0720, MessagePanel_RefreshOpen);

// 0x4e0730. Takes the selected list item's command ID (0x7918..0x7921) from
// the dialog 0x6d401c (+0xc6d) into +0x78 and marks the item.
void __fastcall Obj_TakeListCommand(uint8_t* o) {
    F<int32_t>(o, 0x78) = 0;
    uint8_t* item = F<uint8_t*>(g_windowManager, 0xc6d);
    if (!item) return;
    uint16_t id = F<uint16_t>(item, 4);
    if (!id || id < 0x7918 || id > 0x7921) return;
    F<int32_t>(o, 0x78) = id;
    F<uint32_t>(F<uint8_t*>(g_windowManager, 0xc6d), 0x33) |= 0x20;
}
RT3_REPLACE(0x4e0730, Obj_TakeListCommand);

// 0x4e0780. Scrolls the map while one of the arrow buttons 0x7918..0x7921 is
// held under the mouse (the focused widget 0x6d401c +0xc6d; +0x78 is a button
// to ignore, released when the focus moves on): every >5 ms by the elapsed
// time (at most 1 s) times the scroll speed option +0x114, in the button's
// direction (0x6066c8 / 0x6066e8; 0x7920 / 0x7921 act as 0x791c / 0x791e).
RT3_NOINLINE void __fastcall MapScroll_Tick(uint8_t* w) {
    if (F<int32_t>(w, 0x78) && F<uint16_t>(F<uint8_t*>(g_windowManager, 0xc6d), 4) != F<int32_t>(w, 0x78)) {
        if (uint8_t* b = Windows_FindChild(g_windowManager, 0, F<int32_t>(w, 0x78))) F<uint32_t>(b, 0x33) &= ~0x20u;
        F<int32_t>(w, 0x78) = 0;
    }
    if (!F<int32_t>(g_app, 0x114) || !g_engine[0x114226] ||
        !F<uint8_t*>(g_windowManager, 0xc6d))
        return;
    Engine_Input(hook::As<void*>(g_engine));
    if (Input_Flag800()) return;
    Engine_Input(hook::As<void*>(g_engine));
    if (Input_Flag2000()) return;
    uint16_t id = F<uint16_t>(F<uint8_t*>(g_windowManager, 0xc6d), 4);
    if (id < 0x7918 || id > 0x7921 || id == F<int32_t>(w, 0x78)) return;
    int32_t dt = ClockMs() - F<int32_t>(F<uint8_t*>(g_windowManager, 0xc6d), 0xd2);
    if (dt <= 5) return;
    F<int32_t>(F<uint8_t*>(g_windowManager, 0xc6d), 0xd2) = ClockMs();
    if (dt > 1000) dt = 1000;
    int32_t speed = F<int32_t>(g_app, 0x114);
    float v = (float)((double)dt / D(0x5c87f8) * D(0x5c8c18));
    v = (float)((double)v * hook::Global<float>(0x5f34b0 + speed * 4));
    int32_t dir = F<uint16_t>(F<uint8_t*>(g_windowManager, 0xc6d), 4);
    if (dir == 0x7920) dir = 0x791c;
    else if (dir == 0x7921) dir = 0x791e;
    if (!Engine_IsForeground() || !Cursor_InWindow()) return;
    float dz = (float)hook::Global<int32_t>(0x6066e8 + dir * 4);
    dz = (float)((double)dz * v);
    float dx = (float)hook::Global<int32_t>(0x6066c8 + dir * 4);
    dx = (float)((double)dx * v);
    Camera_Pan(g_62be68, 0, dx, dz);
}
RT3_REPLACE(0x4e0780, MapScroll_Tick);

// 0x4e0960. Fills the setup window's option labels 0x7918..0x7921 (event 0x65 in
// windowed full-screen-capable mode with option +0x114, else 0x87) and its title
// 0x7923: string 0x711, plus string 0xbcf with the display name of model 0x5d0f48
// when there is one.
RT3_NOINLINE void __fastcall SetupWindow_FillLabels(uint8_t* w) {
    uint32_t ev = g_engine[0x114226] && F<int32_t>(g_app, 0x114) ? 0x65 : 0x87;
    for (uint32_t id = 0x7918; id <= 0x7921; ++id) Window_SendEvent5(w, 0, ev, id, 3, 0, 0);
    char text[0x1f4], name[0x12c], extra[0x190];
    strcpy(text, GetString(0x711));
    if (Models_DisplayName((const char*)0x5d0f48, name)) {
        FormatText(extra, GetString(0xbcf), name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        strcat(text, extra);
    }
    Window_SendEvent5(w, 0, 0x6e, 0x7923, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4e0960, SetupWindow_FillLabels);

// 0x4e0a70. Cursor for edit button `id` (0x7920 / 0x7921 share 0x791c / 0x791e's):
// cursor id - 0x790b while `active` and no modifier (input flags 0x800 / 0x2000) is
// held, remembering a normal cursor (not 0, not 13..20) in 0x622078; otherwise a
// 13..20 cursor goes back to the remembered one.
void __fastcall EditButton_SetCursor(int32_t id, int, int32_t active, int32_t, int32_t, int32_t, int32_t) {
    if (id == 0x7920) id = 0x791c;
    else if (id == 0x7921) id = 0x791e;
    int32_t cur = F<int32_t>(g_cursorManager, 4);
    bool tool = cur >= 0xd && cur <= 0x14;
    if (active) {
        Engine_Input(hook::As<void*>(g_engine));
        if (!Input_Flag800()) {
            Engine_Input(hook::As<void*>(g_engine));
            if (!Input_Flag2000()) {
                if (cur && !tool) g_622078 = cur;
                Cursor_Set(g_cursorManager, 0, id - 0x790b, 0, 0);
                return;
            }
        }
    }
    if (tool) Cursor_Set(g_cursorManager, 0, g_622078, 0, 0);
}
RT3_REPLACE(0x4e0a70, EditButton_SetCursor);

// 0x4e0b20. Setup window constructor (vtable 0x5d0f3c, kept in 0x6d0820, layout
// 0x5d0f68): edit buttons 0x7918..0x7921 get the cursor handler 0x4e0a70, the
// labels are filled and control 0x7922 gets style 3.
RT3_NOINLINE uint8_t* __fastcall SetupWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0f3c;
    g_setupWindow = w;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d0f68);
    for (uint32_t id = 0x7918; id <= 0x7921; ++id) Window_SendEvent5(w, 0, 0x7b, id, 0x4e0a70, 0, 0);
    SetupWindow_FillLabels(w);
    Window_SendEvent5(w, 0, 0x87, 0x7922, 3, 0, 0);
    F<int32_t>(w, 0x78) = 0;
    return w;
}
RT3_REPLACE(0x4e0b20, SetupWindow_Construct);

// 0x4e0ba0 (event handler). Setup window d: sizes its follow button 0x7922
// (wide in the editor with a free camera); idle scrolls the map, 0xac
// refills the labels, 0x7922 stops following, 0x7923 / 0x7924 switch the
// editor buttons (or, in the campaign, refuse with string 0xe8c).
int32_t __fastcall SetupWindow_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    uint8_t* follow = Widget_FindChild(d, 0, 0x7922);
    uint8_t* cam = g_62be68;
    if (cam && F<uint32_t>(cam, 0x60) && F<int32_t>(g_app, 0x68))
        F<uint32_t>(follow, 0xb) = 0x3e133333;
    else
        F<uint32_t>(follow, 0xb) = 0x3c99999a;
    switch (msg[0]) {
    case 0:
        MapScroll_Tick(d);
        return 1;
    case 0xac:
        SetupWindow_FillLabels(d);
        return 1;
    case 0xca:
        if (msg[1] != 0x7922) return 1;
        World_SetFollow(g_world, 0, 0, nullptr);
        return 0;
    case 0xcb: {
        const uint32_t id = msg[1];
        if (id != 0x7923 && id != 0x7924) return 1;
        if (g_6d3b4c) {
            Dialog_RunMessage(GetString(0xe8c), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            return 0;
        }
        World_SetEditorButtons(id == 0x7923 ? 1 : 0, 0);
        Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 1, 0, -1);
        return 0;
    }
    default:
        return 1;
    }
}
RT3_REPLACE(0x4e0ba0, SetupWindow_HandleEvent);

void Panel6d0820_Close() { Panel_Close(0x6d0820, 0x5d0f3c, true); }
RT3_REPLACE(0x4e0df0, Panel6d0820_Close);

// 0x4e0e40. Opens the setup window (0x6d0820) once and puts it on screen.
void SetupWindow_Open() {
    if (g_setupWindow) return;
    uint8_t* w = (uint8_t*)crt::operator_new(0xae);
    if (w) w = SetupWindow_Construct(w);
    g_setupWindow = w;
    WidgetList_Insert(hook::As<uint8_t**>(g_windowManager), 0, w, 1);
}
RT3_REPLACE(0x4e0e40, SetupWindow_Open);

// 0x4e0e90. The high score window: two tables (0x6d0828 counts, records of 0x6a
// bytes at 0x6d0830, ten per table), the second (scenario scores, at most 5)
// drawn first above the first (at most 10). Each has a title (0x48d / 0x48e),
// a header row (0x48f..0x494) and per record: rank and name, the scenario
// (+0x1f), the score (table 1: three numbers +0x46.. with 0x495; table 0: the
// medal +0x42 0x496..0x498), the difficulty / result +0x3e and +0x52. The just
// added record (0x6d1088, table 0x6d108c, row 0x6d1090) is white.
RT3_NOINLINE void __fastcall HighScores_Draw(void*, int, float, float y, float w, float, int32_t) {
    static const int32_t headX[5] = { 0xf, 0x1d1, 0x384, 0x47b, 0x55f }, headW[5] = { 0x1a4, 0x1a4, 0xe1, 0xc3, 0xc3 };
    static const int32_t rowX[5] = { 0x1e, 0x1d1, 0x384, 0x474, 0x55f }, rowW[5] = { 0x1a4, 0x1a4, 0x96, 0x96, 0x78 };
    char buf[0x400];
    for (int32_t t = 1; t >= 0; --t) {
        float rowY = (float)((double)y + (t ? Fl(0x5d09a8) : Fl(0x5cb5cc)));
        Font_DrawTextBox(g_6d1084, 0, GetString(t ? 0x48d : 0x48e), 0.0f, rowY, w, BitsF(0x3d4ccccd), 0xfffff799, 0, 0, 0, 1.0f);
        rowY = (float)((double)rowY + Fl(0x5d01c4));
        int32_t n = hook::Global<int32_t>(0x6d0828 + t * 4);
        if (n >= 10) n = 10;
        if (t && n >= 5) n = 5;
        for (int32_t i = -1; i < n; ++i) {
            for (int32_t k = 0; k < 5; ++k) {
                int32_t x, cw;
                if (i == -1) {
                    static const int32_t heads[5] = { 0x48f, 0x490, 0, 0x493, 0x494 };
                    FormatText(buf, GetString(k == 2 ? (t ? 0x491 : 0x492) : heads[k]), 0, 0, 0, 0, 0, 0, 0, 0);
                    x = headX[k];
                    cw = headW[k];
                } else {
                    uint8_t* rec = (uint8_t*)0x6d0830 + (t * 10 + i) * 0x6a;
                    uint8_t* rec0 = (uint8_t*)0x6d0830 + i * 0x6a;
                    char num[0x100];
                    switch (k) {
                    case 0:
                        FormatText(buf, (const char*)0x5d06e8, Num_Format(num, 1, (double)(i + 1), 10, 1, 0, 0), (const char*)rec, 0, 0, 0, 0, 0, 0);
                        break;
                    case 1: FormatText(buf, (const char*)(rec + 0x1f), 0, 0, 0, 0, 0, 0, 0, 0); break;
                    case 2:
                        if (t) {
                            FormatText(buf, (const char*)0x5c87a8, 0, 0, 0, 0, 0, 0, 0, 0);
                            char n1[0x20], n2[0x20], n3[0x20], line[0x400];
                            const char* a = Num_Format(n1, 1, (double)F<int32_t>(rec, 0x46), 10, 1, 0, 0);
                            const char* b = Num_Format(n2, 1, (double)F<int32_t>(rec, 0x4a), 10, 1, 0, 0);
                            const char* c = Num_Format(n3, 1, (double)F<int32_t>(rec, 0x4e), 10, 1, 0, 0);
                            FormatText(line, GetString(0x495), c, b, a, 0, 0, 0, 0, 0);
                            strcat(buf, line);
                        } else {
                            int32_t m = F<int32_t>(rec0, 0x42);
                            FormatText(buf, GetString(m == 0 ? 0x496 : m == 1 ? 0x497 : 0x498), 0, 0, 0, 0, 0, 0, 0, 0);
                        }
                        break;
                    case 3: {
                        int32_t m = F<int32_t>(t ? rec : rec0, 0x3e);
                        FormatText(buf, GetString(m == 0 ? 0x499 : m == 1 ? (t ? 0x22 : 0x7a8) : m == 2 ? 0x49a : 0x7ab), 0, 0, 0, 0, 0, 0, 0, 0);
                        break;
                    }
                    case 4: Num_Format(buf, 1, (double)F<int32_t>(rec, 0x52), 10, 1, 0, 0); break;
                    }
                    x = rowX[k];
                    cw = rowW[k];
                }
                if (k == 2) {
                    if (t) {
                        x = 0x1d1;
                        cw = 0x285;
                    }
                } else if (k == 1 && t)
                    continue;
                float fx = (float)x, fw = (float)cw;
                if (i == -1) {
                    Font_DrawTextBox(g_6d107c, 0, buf, (float)((double)fx / Fl(0x5ca198)), rowY, (float)((double)fw / Fl(0x5ca198)),
                                     BitsF(0x3ccccccd), 0xfffff799, 0, 0, 1, 1.0f);
                } else {
                    int32_t align = (k > 1) + 2;
                    if (k == 2 && t) align = 0;
                    uint32_t color = g_6d1088 && g_6d108c == t && g_6d1090 == i ? 0xffffffff
                                                                                                                                                    : 0xfffff799;
                    Font_DrawTextBox(g_6d107c, 0, buf, (float)((double)fx / Fl(0x5ca198)), rowY, (float)((double)fw / Fl(0x5ca198)),
                                     BitsF(0x3ccccccd), color, align, 0, 0, 1.0f);
                }
            }
            rowY = (float)((double)rowY + Fl(0x5cb660));
        }
    }
}
RT3_REPLACE(0x4e0e90, HighScores_Draw);

// 0x4e14e0 (event handler). Jukebox: messages pass (event 0xcb / 0xb799
// becomes 0xc9); idle ticks with the music player 0x6d1094 stopped and not
// waiting (0x6d1098) start a random track of the four (0x62207c names, file
// pattern 0x5d05a8) not played this round (bits +0x78, reset when all four
// are), at the music volume; while one plays it waits 20 s before allowing
// the next.
int32_t __fastcall Jukebox_HandleEvent(uint8_t* j, int, uint32_t* msg) {
    if (msg[0]) {
        if (msg[0] == 0xcb && msg[1] == 0xb799) msg[0] = 0xc9;
        return 1;
    }
    if (!g_6d1094) return 0;
    if (Music_IsPlaying(g_6d1094) || g_6d1098 != 1) {
        if (ClockMs() - g_6d109c > 0x4e20) g_6d1098 = 1;
        return 0;
    }
    if (F<int32_t>(j, 0x78) == 0xf) F<int32_t>(j, 0x78) = 0;
    int32_t r = Rand(0, 3);
    while (F<uint32_t>(j, 0x78) & (1u << (r & 31))) r = Rand(0, 3);
    F<uint32_t>(j, 0x78) |= 1u << (r & 31);
    char frame[0x204];
    char* file = frame + 4;
    FormatText(file, (const char*)0x5d05a8, *(const char* const*)(0x62207c + r * 4), 0, 0, 0, 0, 0, 0, 0);
    const float vol = (float)((double)F<float>(g_engine, 0x114702) * D(0x5c8640));
    Music_SetVolume(g_6d1094, 0, vol);
    void* digital = (void*)(uintptr_t)Sound_Get2c(g_sound);
    Music_Play(g_6d1094, 0, file, digital, nullptr);
    g_6d1098 = 0;
    g_6d109c = ClockMs();
    return 0;
}
RT3_REPLACE(0x4e14e0, Jukebox_HandleEvent);

// 0x4e1650 (list in ECX, name in EDX). Adds a high score to list `list` of
// the table 0x6d0828 (count per list, then 2 x 10 records of 0x6a bytes from
// 0x6d0830: name +0 and scenario +0x1f (30 characters each), the values
// +0x3e..+0x4e and the score +0x52). Scenario scores (list 1) are
// 5 * (a2 + 2 * (a3 + 2 * (a4 + 10 * a1))), campaign scores 10 * (a5 + 4 * a1
// + 1). Ranks below the list size (5 / 10) ask for the player name (0x49b)
// unless `quiet`, then the table is saved to data\other\highscores.dat.
// Returns the rank + 1 (0x6d108c / 0x6d1090 / 0x6d1088 remember it), or 0.
RT3_NOINLINE int32_t __fastcall HighScores_Add(int32_t list, const char* name, const char* scenario, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5,
                                  int32_t quiet) {
    int32_t score = list ? (a2 + (a3 + (a4 + a1 * 10) * 2) * 2) * 5 : (a5 + a1 * 4 + 1) * 10;
    HighScores_Load();
    int32_t* counts = (int32_t*)0x6d0828;
    uint8_t* recs = (uint8_t*)0x6d0830 + list * 0x424;
    int32_t rank = 0;
    if (counts[list] > 0)
        while (rank < counts[list] && score < F<int32_t>(recs + rank * 0x6a, 0x52)) ++rank;
    if (rank < 9) memmove(recs + (rank + 1) * 0x6a, recs + rank * 0x6a, (9 - rank) * 0x6a);
    if (rank >= (list ? 5 : 10)) return 0;
    char buf[0x20];
    strcpy(buf, name);
    if (!quiet)
        Dialog_RunMessage(GetString(0x49b), 9, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)buf, 0x18, 0, nullptr, nullptr, 0, 0,
                          nullptr, 0);
    uint8_t* r = recs + rank * 0x6a;
    StrCopyN((char*)r, buf, 0x1e);
    StrCopyN((char*)r + 0x1f, scenario, 0x1e);
    F<int32_t>(r, 0x3e) = a1;
    F<int32_t>(r, 0x42) = a5;
    F<int32_t>(r, 0x46) = a2;
    F<int32_t>(r, 0x4a) = a3;
    F<int32_t>(r, 0x4e) = a4;
    F<int32_t>(r, 0x52) = score;
    int32_t n = counts[list] + 1;
    counts[list] = n < 10 ? n : 10;
    int32_t s = StreamOpen((const char*)0x5d0fbc, 0, 200000, 0);
    if (s != -1) {
        StreamWrite(s, (const void*)0x6d0828, 0x850, 0);
        StreamClose(s);
    }
    g_6d108c = list;
    g_6d1090 = rank;
    g_6d1088 = 1;
    return rank + 1;
}
RT3_REPLACE(0x4e1650, HighScores_Add);

// 0x4e1840. Loads the high score table 0x6d0828 from data\other\highscores.dat;
// a missing file is created empty and seeded with the five default entries
// (scenario: Bismarck, Brunel; campaign: Gould, Huntington, Wheelwright).
RT3_NOINLINE void HighScores_Load() {
    const char* path = (const char*)0x5d0fbc;
    if (!FileExists(path)) {
        memset((void*)0x6d0828, 0, 0x850);
        int32_t s = StreamOpen(path, 0, 200000, 0);
        if (s != -1) {
            StreamWrite(s, (const void*)0x6d0828, 0x850, 0);
            StreamClose(s);
        }
        if (FileExists(path)) {
            HighScores_Add(1, (const char*)0x5d1024, (const char*)0x5d1038, 1, 9, 6, 1, 1, 1);
            HighScores_Add(1, (const char*)0x5d100c, (const char*)0x5d1038, 0, 7, 5, 4, 1, 1);
            HighScores_Add(0, (const char*)0x5d1000, GetString(0x49c), 1, 0, 0, 0, 2, 1);
            HighScores_Add(0, (const char*)0x5d0fec, GetString(0x49d), 0, 0, 0, 0, 1, 1);
            HighScores_Add(0, (const char*)0x5d0fd8, GetString(0x49e), 0, 0, 0, 0, 0, 1);
        }
    }
    memset((void*)0x6d0828, 0, 0x850);
    int32_t s = StreamOpen(path, 1, 200000, 0);
    if (s == -1) return;
    StreamRead(s, (void*)0x6d0828, 0x850, 0, 1);
    StreamClose(s);
}
RT3_REPLACE(0x4e1840, HighScores_Load);

// 0x4e19a0. High score dialog constructor ("HighScores.win", vtable 0x5d103c):
// fonts 4 / 3 / 2 (0x6d1084 / 0x6d107c / 0x6d1080), with 3D sound a fresh sound
// state (0x6d1094: first dword 0, volume +0x104 = 1), start time 0x6d109c, handler
// 0x4e0e90 on 0xb79a.
uint8_t* __fastcall HighScoresDialog_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d103c;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d1048);
    g_6d1084 = Pack_AcquireFont(g_pack, 0, 4);
    g_6d107c = Pack_AcquireFont(g_pack, 0, 3);
    g_6d1080 = Pack_AcquireFont(g_pack, 0, 2);
    F<int32_t>(w, 0x78) = 0;
    if (g_sound && Sound_Is3D(g_sound)) {
        uint8_t* s = (uint8_t*)crt::operator_new(0x108);
        if (s) {
            F<uint32_t>(s, 0) = 0;
            F<float>(s, 0x104) = 1.0f;
        }
        g_6d1094 = s;
    }
    g_6d109c = ClockMs();
    Window_SendEvent5(w, 0, 0x7a, 0xb79a, 0x4e0e90, 0, 0);
    return w;
}
RT3_REPLACE(0x4e19a0, HighScoresDialog_Construct);

// 0x4e1a70. High score window ("HighScores.win") destructor: releases its three
// images (0x6d1084 / 7c / 80) and closes its music 0x6d1094.
void __fastcall HighScores_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d103c;
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d1084));
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d107c));
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d1080));
    g_6d1098 = 0;
    if (uint8_t* m = g_6d1094) {
        Music_Close(m);
        crt::operator_delete(m);
        g_6d1094 = nullptr;
    }
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4e1a70, HighScores_Destruct);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4e1af0. Reload the scores, optionally add a result, then show and release
// the modal score dialog. Callbacks may replace the dialog and app globals.
RT3_NOINLINE void __fastcall HighScores_Show(int32_t value, int32_t extra, int32_t keepHighlight) {
    memset((void*)0x6d0828, 0, 0x850);
    HighScores_Load();
    if (!keepHighlight) g_6d1088 = 0;
    if (value >= 0) {
        g_6d1090 = -1;
        char name[300], scenario[300];
        auto company = hook::Orig<uint8_t*(__fastcall*)(uint8_t*)>(0x4348c0);
        uint8_t* world = g_world;
        if (!world || !company(world))
            strcpy(name, GetString(0xbd0));
        else
            strcpy(name, (const char*)company(g_world) + 8);
        uint8_t* options = g_newGame;
        int32_t list = 0, n2 = 0, n3 = 0, n4 = 0;
        if (options) {
            if (options[0xc5]) {
                list = 1;
                for (int32_t i = 0; i < 16; ++i) {
                    if (options[0xc6 + i] == 2) ++n2;
                    else if (options[0xc6 + i] == 3) ++n3;
                    else if (options[0xc6 + i] == 4) ++n4;
                }
            }
            strcpy(scenario, (const char*)options + 0x44);
        } else strcpy(scenario, "---");
        HighScores_Add(
            list, name, scenario, value, n2, n3, n4, extra, 0);
        if (g_6d4000) return;
    }
    void* view = hook::As<void*>(g_gameWindow);
    if (view) View_SetToolMode((uint8_t*)(view), 0, (uint32_t)(0), 0);
    uint8_t* mem = ((uint8_t*)crt::operator_new(0x7c));
    hook::As<uint8_t*>(g_6d1078) = mem ? HighScoresDialog_Construct(mem) : nullptr;
    PauseLock_Push((int32_t*)(hook::As<void*>(g_app)));
    Ui_RunModal((uint8_t**)(hook::As<void*>(g_windowManager)), 0, (uint8_t*)(g_6d1078), 1, 0, 20, 0);
    PauseLock_Pop((int32_t*)(hook::As<void*>(g_app)));
    Windows_Remove((uint8_t*)(hook::As<void*>(g_windowManager)), 0, (uint8_t*)(g_6d1078));
    uint8_t* dying = hook::As<uint8_t*>(g_6d1078);
    if (dying) {
        HighScores_Destruct(dying);
        crt::operator_delete(dying);
    }
    g_6d1078 = nullptr;
}
RT3_REPLACE(0x4e1af0, HighScores_Show);

void __fastcall Sprite_DrawFirstGlyphCb(void*, int, float x, float y, int32_t a, int32_t b, int32_t) {
    Sprite_DrawFirstGlyph(g_6d10a8, 0, x, y, a, b);
}
RT3_REPLACE(0x4e1cd0, Sprite_DrawFirstGlyphCb);

void __fastcall Sprite_DrawSecondGlyphCb(void*, int, float x, float y, int32_t a, int32_t b, int32_t) {
    Sprite_DrawSecondGlyph(g_6d10a8, 0, x, y, a, b);
}
RT3_REPLACE(0x4e1d00, Sprite_DrawSecondGlyphCb);

void __fastcall CompanyWindow_DrawCb(void*, int, float x, float y, float w, float h, int32_t) { CompanyWindow_Draw(g_6d10a8, 0, x, y, w, h); }
RT3_REPLACE(0x4e1d30, CompanyWindow_DrawCb);

// 0x4e1d60. Portrait window: when the number of choices for the current entry
// (table 0x6220a0, 13 bytes per +0x78) changes (+0x80), swaps the frame image
// (0x5d1078 for none, 0x5d1064 otherwise) and the visibility of the arrow
// buttons 0x3e80 / 0x3e81, then lays out the four slots 0x3eee..0x3ef1 in a
// ring so the last choice sits in front: position (event 0x99) and scale
// (event 0xa9: 1.2 / 1.0 / 0.9 / 0.8, 1.03 for the front slot when there is none).
RT3_NOINLINE void __fastcall PortraitWindow_Layout(uint8_t* w) {
    int32_t n = *(const int32_t*)(0x6220a0 + F<int32_t>(w, 0x78) * 0xd);
    if (F<int32_t>(w, 0x80) == n) return;
    auto swapFrame = [&](uint32_t name) {
        if (F<void*>(w, 0xb2d)) Pack_Release(g_pack, 0, F<void*>(w, 0xb2d));
        F<void*>(w, 0xb2d) = nullptr;
        F<uint8_t*>(w, 0xb2d) = Pack_AcquireNamed(g_pack, 0, (const char*)(uintptr_t)name);
    };
    if (n == 0) {
        swapFrame(0x5d1078);
        Window_SendEvent5(w, 0, 0x6d, 0x3e81, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x86, 0x3e80, 3, 0, 0);
    } else if (F<int32_t>(w, 0x80) <= 0) {
        swapFrame(0x5d1064);
        Window_SendEvent5(w, 0, 0x86, 0x3e81, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x6d, 0x3e80, 3, 0, 0);
    }
    F<int32_t>(w, 0x80) = n;
    int32_t last = (n < 1 ? 1 : n) - 1;
    const uint32_t scales[4] = { 0x3f99999a, 0x3f800000, 0x3f666666, 0x3f4ccccd };
    int32_t px = 0x49;
    for (int32_t i = 0; i < 4; ++i, px += 0x7e) {
        int32_t slot = (4 + i - last) % 4;
        float fx = (float)((double)px / D(0x5ca2d0));
        float fy = (float)((double)(slot * 3 + 0x45b) / D(0x5ca2c8));
        Window_SendEvent4(w, 0, 0x99, 0x3eee + i, FBits(fy), FBits(fx));
        uint32_t scale = scales[slot];
        if (n == 0 && slot == 0) scale = 0x3f83d70a;
        Window_SendEvent4(w, 0, 0xa9, 0x3eee + i, scale, 0);
    }
}
RT3_REPLACE(0x4e1d60, PortraitWindow_Layout);

int32_t Global6d10a8Set() { return hook::As<void*>(g_6d10a8) ? 1 : 0; }
RT3_REPLACE(0x4e1f50, Global6d10a8Set);

void SetGlobal6d10a4() { g_6d10a4 = 1; }
RT3_REPLACE(0x4e1f60, SetGlobal6d10a4);

// 0x4e1f70 (callback). Sets the company window's selection (+0x98) to v or
// -1 by the first stack argument.
void __fastcall CompanyWindow_SelectCb(int32_t v, int, int32_t on, int32_t, int32_t, int32_t, int32_t) {
    if (uint8_t* w = g_6d10a8) F<int32_t>(w, 0x98) = on ? v : -1;
}
RT3_REPLACE(0x4e1f70, CompanyWindow_SelectCb);

// 0x4e1fa0. Detail dialog (0x6d10a8) header at (x, y): the picture (0x3f1f) of
// the shown player (mode 8, +0x8c in 0x6ceb9c: portrait +0x15c, 0x29 = custom
// named one) or company (+0x88 in 0x62be10: its logo +0x37, else its picture
// +0xd65), and the next / previous buttons 0x3f20 / 0x3f21 (tooltips
// 0xbd1 / 0xbd3 for players, 0xbd2 / 0xbd4 for companies).
RT3_NOINLINE void __fastcall DetailDialog_AddHeader(uint8_t* win, int, float x, float y, float, float) {
    uint8_t def[0x89];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + D(0x5c8590));
    F<uint32_t>(def, 0xf) = 0x3dcccccd;
    F<uint32_t>(def, 0x13) = 0x3e19999a;
    F<float>(def, 0xb) = (float)((double)y + D(0x5c8918));
    F<uint16_t>(def, 0x5f) = 0;
    F<uint16_t>(def, 0) = 0x3f1f;
    const char* pic;
    char portrait[0x40];
    uint8_t* dlg = g_6d10a8;
    if (F<int32_t>(dlg, 0x78) == 8) {
        uint8_t* p = (uint8_t*)Collection_Get(g_players, 0, F<uint32_t>(dlg, 0x8c));
        if (p[0x15c] != 0x29) {
            p = (uint8_t*)Collection_Get(g_players, 0, F<uint32_t>(g_6d10a8, 0x8c));
            crt::sprintf(portrait, (const char*)0x5d0ab0, p[0x15c]);
            pic = portrait;
        } else {
            pic = (const char*)Obj_NamedEntry40(
                (uint8_t*)Collection_Get(g_players, 0, F<uint32_t>(g_6d10a8, 0x8c)));
        }
    } else {
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(dlg, 0x88));
        if (F<int32_t>(c, 0x37) != -1) {
            c = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_6d10a8, 0x88));
            F<uint16_t>(def, 0x5f) = F<uint16_t>(c, 0x37);
            pic = (const char*)0x5c9f98;
        } else {
            F<uint16_t>(def, 0x5f) = 0;
            pic = (const char*)Collection_Get(g_companies, 0, F<uint32_t>(g_6d10a8, 0x88)) + 0xd65;
        }
    }
    String_Assign(&F<char*>(def, 0x43), pic);
    uint8_t* img = (uint8_t*)crt::operator_new(0x111);
    img = img ? Scrollbar_Construct(img, 0, def) : nullptr;
    if (F<int32_t>(g_6d10a8, 0x78) == 8) {
        F<uint32_t>(img, 0x100) = 0x3f3ae148;
        F<uint32_t>(img, 0x104) = 0x3f3851ec;
    } else {
        F<uint32_t>(img, 0x100) = 0x3fcccccd;
        F<uint32_t>(img, 0x104) = 0x3fcccccd;
    }
    Window_AddChild(win, 0, img, 0);
    crt::free(F<void*>(def, 0x43));
    float bx = (float)((double)x + D(0x5c9c80));
    float by = (float)((double)y + D(0x5c86e0));
    AddControlButton(win, [&](uint8_t* d) {
        F<float>(d, 0xb) = by;
        F<float>(d, 7) = bx;
        F<uint32_t>(d, 0xf) = 0x3cdc28f6;
        F<uint32_t>(d, 0x13) = 0x3ce147ae;
        F<uint16_t>(d, 0x5f) = 2;
        F<uint16_t>(d, 0x61) = 3;
        F<uint16_t>(d, 0) = 0x3f20;
        d[0x89] = 6;
        d[0x8a] = 7;
        String_Assign(&F<char*>(d, 0x43), (const char*)0x5d08b8);
        String_Assign(&F<char*>(d, 0x3f), GetString(F<int32_t>(g_6d10a8, 0x78) == 8 ? 0xbd1 : 0xbd2));
    });
    float by2 = (float)((double)y + D(0x5c8648));
    AddControlButton(win, [&](uint8_t* d) {
        F<float>(d, 0xb) = by2;
        F<float>(d, 7) = bx;
        F<uint32_t>(d, 0xf) = 0x3cdc28f6;
        F<uint32_t>(d, 0x13) = 0x3ce147ae;
        F<uint16_t>(d, 0x5f) = 4;
        F<uint16_t>(d, 0x61) = 5;
        F<uint16_t>(d, 0) = 0x3f21;
        d[0x89] = 6;
        d[0x8a] = 7;
        String_Assign(&F<char*>(d, 0x43), (const char*)0x5d08b8);
        String_Assign(&F<char*>(d, 0x3f), GetString(F<int32_t>(g_6d10a8, 0x78) == 8 ? 0xbd3 : 0xbd4));
    });
}
RT3_REPLACE(0x4e1fa0, DetailDialog_AddHeader);

}  // namespace rt3
