// App.cpp - RT3.exe 0x482000-0x486000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <windows.h>
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

struct HWND__;

typedef HWND__* HWND;

struct HMENU__;

typedef HMENU__* HMENU;

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Engine_SaveConfig(uint8_t* e);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                            const char* a6, const char* a7, const char* a8);

int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
uint8_t* __fastcall Camera_Construct(uint8_t* c, int, const char* name, uint32_t owner);
PtrListNode* __fastcall SceneObj_AddChild(uint8_t* o, int, uint8_t* child);
PtrListNode* __fastcall SceneObj_AddAttachment(uint8_t* o, int, uint8_t* item);
PtrListNode* __fastcall Camera_AddListener(uint8_t* c, int, void* item);

uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                      uint32_t flags);

void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);

uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6, int32_t a7,
                                  float vol);

void __fastcall Obj_AttachChannel(uint8_t* o, int, uint8_t* ch);
uint8_t* __fastcall SceneObj_ConstructDefault(uint8_t* o);
void __fastcall SceneObj_Init(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
void __fastcall Model_PrepareTint(uint8_t* m, int, int32_t tint);
void __fastcall Model_AddTintRect(uint8_t* m, int, float x0, float y0, float x1, float y1, float tint);
void __fastcall SceneObj_GetPos2(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
const char* __fastcall SysInfo_DisplayName();
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);
uint32_t __fastcall File_Version(const char* path);
float __fastcall Net_CommonVersion();
float __fastcall Game_VersionNumber();
int32_t CountHumanPlayers();
float __fastcall Grid_Roughness(uint8_t* g);
float __fastcall Render_Get14e0(uint8_t* r);
void __fastcall Render_Set14e0(uint8_t* r, int, uint32_t v);
void __fastcall Render_SetLayerValue(uint8_t* r, int, int32_t i, float v);
void __fastcall Render_Set153c(uint8_t* r, int, uint32_t v);
void __fastcall Render_Set1534(uint8_t* r, int, uint32_t v);
void __fastcall Render_Set1540(uint8_t* r, int, uint32_t v);
void __fastcall Render_SetOverviewSize(uint8_t* r, int, int32_t size);
void __fastcall Terrain_RefreshPointGrids(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Terrain_InitMaps(uint8_t* t, int, char init, int32_t, int32_t);
float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m);
int32_t IsMultiplayer();
const char* __fastcall GetString(int32_t id);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
int32_t __fastcall World_LookupNamed1(void* w, int, const char* name);
void __fastcall Scene_SetLightPair1(uint8_t* s, int, int32_t i, float a, float b);
void __fastcall Scene_SetLightPair0(uint8_t* s, int, int32_t i, float a, float b);
void __fastcall Scene_SetLightRange(uint8_t* s, int, int32_t i, float v);
void __fastcall Scene_ApplyLighting(uint8_t* s);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
int32_t __fastcall Collection_Count(Collection* c);
void __fastcall Scene_ApplyLighting(uint8_t* r);
void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
void __fastcall Obj_UnlinkFromNode(void* o);
void __fastcall Car_LinkToNode(uint8_t* car, int, int32_t id, int32_t side);
void __fastcall Track_AddWear(uint8_t* t);
void __fastcall Track_SetBlocked(uint8_t* o, int, uint8_t v);
void __fastcall CargoTypes_MarkAvailable(Collection* c);

// Kept original (not hooked): with this rewrite the simcheck run diverges late
// (vehicle smoke emitters launch fewer particles, shifting the shared LCG 0x624324),
// although the code matches the original instruction for instruction; the cause
// looks like stack residue read by original render code. See dumpready's skip list.
// RT3_REPLACE(0x412c10, IndustryTypes_MarkAvailable);

double __fastcall Vec3_Length(const float* v);

void __fastcall Vec3_CrossInPlace(float* a, int, const float* b);
float* __fastcall Vec3_Normalize(float* v);

// Kept original (not hooked): with this rewrite the simcheck run diverges late
// (vehicle smoke emitters launch fewer particles, shifting the shared LCG 0x624324),
// although the code matches the original instruction for instruction; the cause
// looks like stack residue read by original render code. See dumpready's skip list.
// RT3_REPLACE(0x458140, Camera_WorldCross);

int32_t __fastcall Models_ValueByName(const char* name);

void __fastcall Keyboard_SetScrollLock(int32_t on);
void __fastcall Obj_Nudge92(void* o);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t ClockMs();
struct InputEvent;
void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
int32_t __fastcall Ui_Dispatch(uint8_t** mgr, int, uint32_t* ev);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
int32_t __fastcall Table_CountField3f(Collection* t);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall PumpMessages();
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall StreamClose(int32_t s);
HWND __fastcall Engine_Window(void* e);
uint32_t __fastcall Obj_Get18(uint8_t* o);
void __fastcall Engine_LoadConfig(uint8_t* o);
uint32_t __fastcall SysInfo_TotalMemory();
void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind);
void __fastcall Input_PostEvent(uint8_t* in, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Menu_UpdateChecks(HMENU* m, int, char relight);
void __fastcall Lighting_Rebuild(uint8_t* opts);
int32_t __fastcall FileExists(const char* path);
void __fastcall Options_ApplyGraphics(uint8_t* o, int, int32_t which, int32_t rebuild);
int32_t __fastcall Obj_ArrayAc2(void* o, int, int32_t i);
int32_t __fastcall Obj_Idlec60(uint8_t* o);
int32_t __fastcall Adapter_KnownIndex();
void __fastcall Engine_SetCursorObject(void* e, int, uint32_t v);
uint8_t __fastcall SysInfo_IsWin9x();
int32_t __fastcall Engine_TotalTextureMem(void* e);
void Models_Shutdown();
void __fastcall Engine_RequestRepaint(void* e);
void __fastcall WindowManager_Draw(uint8_t* m);
void __fastcall Lobby_DrawQueue(uint8_t* lobby);

struct InputEvent {
    uint32_t v[12];
};

void __fastcall Cursor_UseSystem(uint8_t* c);
int32_t __fastcall Engine_IsForeground();
void* __fastcall Obj_Company21(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

void __fastcall Engine_DrawFrame(uint8_t* e);
void __fastcall Session_Shutdown(uint8_t* s);
void* __fastcall World_Construct(uint8_t* w);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall Cursor_PopBusy(uint8_t* c);
void __fastcall Cursor_PushBusy(uint8_t* c);
void __fastcall App_Redraw(uint8_t* a);
void __fastcall StreamClose(Stream s);
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t Global6d1350Set();
void OptionsDialog_Open();
RT3_NOINLINE void __fastcall Cursor_PopBusy(uint8_t* c);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall Obj_Set78(uint8_t* o, int, int32_t v);
void __fastcall World_Teardown(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);

 // namespace

void __fastcall App_SetScreen(uint8_t* a, int, int32_t screen, int32_t closeWorld);

void ViewModesPanel_Close();
RT3_NOINLINE void __fastcall View_Destroy(uint8_t* view);
void Panel6d0820_Close();
void Panel6d081c_Close();
void __fastcall Setup_Destruct(uint8_t* w);
void __fastcall VideoWindow_Destruct(uint8_t* w);
void __fastcall TitleScreen_Destruct(uint8_t* w);
void __fastcall OptionsScreen_Destruct(uint8_t* w);
void __fastcall Credits_Destruct(uint8_t* w);
void __fastcall CampaignWindow_Destruct(uint8_t* w);
void __fastcall Nop433730(void*);
void __fastcall Obj_Setc5c(uint8_t* o, int, uint32_t v);
uint8_t* __fastcall LoadScreen_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
RT3_NOINLINE void* __fastcall World_Construct(uint8_t* w);
void __fastcall Sound_StartFadeTimer(uint8_t* s, int, int32_t mode);
uint8_t* __fastcall GameWindow_Construct(uint8_t* w);
void GameMessagePanel_Open();
void SetupWindow_Open();
void EditorPanel_Open();
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
uint8_t* __fastcall SetupScreen_Construct(uint8_t* s);
uint8_t* __fastcall VideoWindow_Construct(uint8_t* w);
uint8_t* __fastcall CreditsWindow_Construct(uint8_t* w);
uint8_t* __fastcall CampaignWindow_Construct(uint8_t* w);
int32_t __fastcall Chain_ForEach(uint8_t* o, int, uint32_t arg);
const char* SysInfo_Report();
void __fastcall Status_SetMessage(uint8_t* s, int, const char* text);
int32_t __fastcall Records_Dispatch(uint8_t* r);
RT3_NOINLINE int32_t __stdcall View_OnModeInput(const uint32_t* event);
int32_t __fastcall World_IsHostOrLocal(void* w);
RT3_NOINLINE void __fastcall PumpMessages();
RT3_NOINLINE int32_t __fastcall Engine_IsForeground();
InputEvent* __fastcall Input_PopEvent(uint8_t* q, int, InputEvent* out);
RT3_NOINLINE void __fastcall Input_PostEvent(uint8_t* in, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Playlist_Tick(uint8_t* p);
void __fastcall Sound_Pause(uint8_t* s);
void __fastcall EmitterGroup_Tick(uint8_t* g);
void __fastcall Ambience_Tick(uint8_t* a, int, uint32_t flag);
RT3_NOINLINE int32_t __fastcall Session_Update(uint8_t* s);
uint32_t* __fastcall Pair_Init(uint32_t* p);
void __fastcall Options_Load(uint8_t* o, int, int32_t withEngine);
void __fastcall Options_SetDisplayMode(uint8_t* o, int, int32_t m);
uint8_t* __fastcall Engine_Construct(uint8_t* e);
void __fastcall Engine_Init(uint8_t* e, int, void* inst, int32_t w, int32_t h);
void __fastcall Hotkeys_Init(int32_t load);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall Options_ApplyGraphics(uint8_t* o, int, int32_t which, int32_t rebuild);
RT3_NOINLINE void __fastcall Menu_UpdateChecks(HMENU* m, int, char relight);
RT3_NOINLINE void __fastcall Lighting_Rebuild(uint8_t* opts);
uint8_t* __fastcall Music_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
RT3_NOINLINE void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);
void Effects_Reset();
RT3_NOINLINE HWND __fastcall Engine_Window(void* e);
void FreeBuffers6cea54();
void __fastcall Obj_Free108(void* o);
void __fastcall Net_StopListener();
void __fastcall Engine_Destroy(uint8_t* e);
void __fastcall GdiPair_Release2(void** p);
void StartClock();
void SysInfo_Init();
void ResetRand();
void __fastcall Cmdline_Parse(const char* p);
void __fastcall Set_ccba28();
void __fastcall Keyboard_SaveScrollLock();
uint8_t* __fastcall Game_InitStart(uint8_t* g);
void* __fastcall ReturnThis47bbd0(void* o);
void __fastcall Nop47bbe0(void*);
void Mem_Shutdown();
void __fastcall SysInfo_Nop();
void __fastcall Keyboard_RestoreScrollLock();
int32_t __fastcall Engine_StepSetting(int32_t delta);
int32_t __fastcall Options_StepDetail(int32_t dir);
void __fastcall Options_StepResolution(int32_t dir);
void __fastcall Options_SetDetail(uint8_t* o, int, int32_t which, int32_t level, int32_t rebuild);
void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);
uint32_t __fastcall Engine_IsFullscreen(void* e);
void __fastcall Cursor_UseGameDrawn(uint8_t* c);
void __fastcall Cursor_UseWindows(uint8_t* c);
void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
uint8_t __fastcall Engine_PosterScreenshot(uint8_t* e);
int32_t Global6d0818Is25();
uint8_t __fastcall Engine_Screenshot(uint8_t* e, int, int32_t hideCursor);
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
int32_t Options_ToggleFullscreen();
int32_t __fastcall App_DispatchEvent(uint8_t* a, int, uint32_t* ev);
int32_t __fastcall App_DispatchEvent5(uint8_t* app, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void __fastcall Options_SetDefaults(uint8_t* o, int, int32_t preset);
void __fastcall Options_AutoDetail(uint8_t* o);
void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);

// Functions of this file, in exe order.
void __fastcall World_RefreshTime(uint8_t* w);
RT3_NOINLINE void __fastcall World_Teardown(uint8_t* w);
RT3_NOINLINE void __fastcall Weather_ApplyIntensity(uint8_t* w);
RT3_NOINLINE int32_t __fastcall World_TogglePause(uint8_t* w, int, int32_t announce);
RT3_NOINLINE void __fastcall World_Startup(uint8_t* w, int, int32_t setup, int32_t withMap);
RT3_NOINLINE int32_t __fastcall World_Frame(uint8_t* w);
float __fastcall Model_FrameTime(uint8_t* o);
RT3_NOINLINE void __fastcall Hotkeys_Init(int32_t load);
RT3_NOINLINE void __fastcall Menu_UpdateChecks(HMENU* m, int, char relight);
RT3_NOINLINE int32_t __fastcall Session_Update(uint8_t* s);
RT3_NOINLINE void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
RT3_NOINLINE void __fastcall App_Redraw(uint8_t* a);
RT3_NOINLINE void __fastcall Lighting_Rebuild(uint8_t* opts);
RT3_NOINLINE float SceneClock_MinutesSinceDawn();
RT3_NOINLINE float __cdecl Sky_NightFactor();
RT3_NOINLINE int32_t Option1df();
RT3_NOINLINE void __stdcall Options_SetWeatherOverride(int32_t on, int32_t level);
RT3_NOINLINE void __stdcall Game_SetFixedTime(int32_t on, int32_t hour);
int32_t Cmd_CycleCursor();
RT3_NOINLINE int32_t __fastcall Engine_StepSetting(int32_t delta);
int32_t Hotkey_SettingUp();
int32_t Hotkey_SettingDown();
RT3_NOINLINE int32_t __fastcall Options_StepDetail(int32_t dir);
int32_t Hotkey_DetailUp();
int32_t Hotkey_DetailDown();
int32_t Hotkey_Options();
int32_t Hotkey_Screenshot();
int32_t Cmd_PosterScreenshot();
int32_t Cmd_ToggleSysInfo();
int32_t Cmd_ToggleEngineText1();
int32_t Cmd_ToggleEngineText2();
void __fastcall Car_MoveToSegment(uint8_t* car, int, int32_t seg, int32_t dir);
void __fastcall IndustryTypes_MarkAvailable(Collection* t);
float* __fastcall Camera_WorldCross(uint8_t* cam, int, float* out, float vx, float vy, float vz, float wx, float wy, float wz, float* len);
RT3_NOINLINE void __fastcall Options_SetDisplayMode(uint8_t* o, int, int32_t m);
RT3_NOINLINE float __fastcall Game_VersionNumber();
RT3_NOINLINE const char* __fastcall Game_VersionText();
RT3_NOINLINE int32_t __fastcall Game_EffectiveVersion();
RT3_NOINLINE void __fastcall App_SetViewMode(uint8_t* app, int, int32_t mode);
RT3_NOINLINE void __fastcall App_SetScreen(uint8_t* a, int, int32_t screen, int32_t closeWorld);
RT3_NOINLINE void __fastcall PauseLock_Push(int32_t* p);
RT3_NOINLINE void __fastcall PauseLock_Pop(int32_t* p);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE int32_t Options_ToggleFullscreen();
RT3_NOINLINE void __fastcall Options_StepResolution(int32_t dir);
int32_t Hotkey_ResolutionUp();
int32_t Hotkey_ResolutionDown();
RT3_NOINLINE int32_t __fastcall Options_Check1bf(void* self);
RT3_NOINLINE const char* __fastcall Options_SaveFolder(void* opt);
RT3_NOINLINE int32_t __fastcall App_HandleEvent(uint8_t* a, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall App_DispatchEvent(uint8_t* a, int, uint32_t* ev);
RT3_NOINLINE void __fastcall App_PumpInput(uint8_t* a);
RT3_NOINLINE int32_t __fastcall App_DispatchEvent5(uint8_t* app, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
RT3_NOINLINE int32_t __fastcall App_Frame(uint8_t* a);
RT3_NOINLINE void __fastcall App_Run(uint8_t* a, int, void* inst);
int __stdcall Game_WinMain(void* inst, void* prev, char* cmdLine, int show);
RT3_NOINLINE void __fastcall Options_SetDefaults(uint8_t* o, int, int32_t preset);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
RT3_NOINLINE void __fastcall Options_Load(uint8_t* o, int, int32_t withEngine);
int32_t __fastcall Adapter_KnownIndex();
int32_t __fastcall Obj_ArrayAc(void* o, int, int32_t i);
RT3_NOINLINE int32_t __fastcall Obj_ArrayAc2(void* o, int, int32_t i);
RT3_NOINLINE float __fastcall Map_ViewDistance(uint8_t* o);
RT3_NOINLINE void __fastcall Options_ApplyGraphics(uint8_t* o, int, int32_t which, int32_t rebuild);
RT3_NOINLINE void __fastcall Options_SetDetail(uint8_t* o, int, int32_t which, int32_t level, int32_t rebuild);
RT3_NOINLINE void __fastcall Options_ResetDetail(uint8_t* o);
RT3_NOINLINE void __fastcall Options_AutoDetail(uint8_t* o);
RT3_NOINLINE void __fastcall Train_ShiftGear(uint8_t* o, int, int32_t delta);
RT3_NOINLINE int32_t Global6cfc8cSet();
RT3_NOINLINE uint8_t* __fastcall Tank_Construct(uint8_t* t, int, const char* name, uint8_t* scene, float x, float y, float z);
void __fastcall CampaignWindow_Destruct(uint8_t* w);
uint8_t* __fastcall CampaignWindow_Construct(uint8_t* w);
uint8_t* __fastcall CreditsWindow_Construct(uint8_t* w);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall View_ResetEditorTool(uint8_t* view);
RT3_NOINLINE void OptionsDialog_Open();
uint8_t* __fastcall SetupScreen_Construct(uint8_t* s);
uint8_t* __fastcall VideoWindow_Construct(uint8_t* w);
void __fastcall VideoWindow_Destruct(uint8_t* w);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);
RT3_NOINLINE void __fastcall Engine_DrawFrame(uint8_t* e);
RT3_NOINLINE void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
RT3_NOINLINE uint8_t __fastcall Engine_PosterScreenshot(uint8_t* e);
RT3_NOINLINE uint8_t __fastcall Engine_Screenshot(uint8_t* e, int, int32_t hideCursor);
void __fastcall Engine_Init(uint8_t* e, int, void* inst, int32_t w, int32_t h);
RT3_NOINLINE void __fastcall Scene_ApplyLighting(uint8_t* s);
RT3_NOINLINE void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
RT3_NOINLINE void __fastcall Input_PostEvent(uint8_t* in, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
RT3_NOINLINE void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
RT3_NOINLINE void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);

namespace {
inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
inline int32_t PackHas(const char* path) { return NameTable_ContainsName(g_pack, 0, path, 0, nullptr); }
// A scene object of model `path` (texture `tex`), or none when out of memory.
inline uint8_t* NewSceneObj(const char* path, const char* tex) {
    uint8_t* m = (uint8_t*)ExeNew(0x206);
    return m ? SceneObj_Construct(m, 0, 0, path, tex) : nullptr;
}
// A light of model `path` on `owner`, shown (flags 0x10).
inline uint8_t* NewLight(const char* path, uint8_t* owner) {
    uint8_t* m = (uint8_t*)ExeNew(0x35);
    uint8_t* c = m ? Camera_Construct(m, 0, path, (uint32_t)(uintptr_t)owner) : nullptr;
    return c;
}
inline void ShowLight(uint8_t* c) { c[0x24] = (uint8_t)((c[0x24] & 0xf) | 0x10); }
inline uint8_t* NewSound(const char* path, uint8_t* v, float vol) {
    uint8_t* m = (uint8_t*)ExeNew(0x141);
    return m ? Channel_Setup(m, 0, path, g_map, F<float>(v, 0x1e2), F<float>(v, 0x1e6), F<float>(v, 0x1ea), -1,
                             0, vol)
             : nullptr;
}
inline uint8_t* NewExhaust(const char* path, uint32_t color, int32_t perPoint, const float* rect, uint8_t* v, uint32_t flags,
                           uint32_t fade) {
    uint8_t* m = (uint8_t*)ExeNew(0x1fd);
    uint8_t* e = m ? Emitter_Construct(m, 0, path, color, perPoint, rect, v, flags) : nullptr;
    F<uint32_t>(e, 0x88) = 0;
    F<uint32_t>(e, 0x8c) = 0;
    F<uint32_t>(e, 0x90) = 0;
    F<uint32_t>(e, 0x98) = fade;
    Timer_SetBudget(e, 0, 0x3c);
    Camera_AddListener(v, 0, e);
    return e;
}
}

namespace {
inline float Bits(uint32_t b) { return *(float*)&b; }
}

// Version as an integer: (v + 0x5c88a0) x 0x5c85d8 through the exe's _ftol.
static inline int32_t VersionInt(float v) {
    int32_t r;
    __asm {
        fld v
        fadd qword ptr ds:[0x5c88a0]
        fmul qword ptr ds:[0x5c85d8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline double Bits_165(uint64_t u) { return *(double*)&u; }

static inline double Sqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }

// One view distance band: when the map is larger than `minArea` and the distance is
// above `top` - roughness, scales it down to area / sqrt(area / div), capped at that.
static inline float Band(float v, float area, float rough, uint32_t minArea, uint32_t top, uint32_t div) {
    if (!(area > Fl(minArea))) return v;
    double lim = D(top) - rough;
    if (!(v > lim)) return v;
    float s = (float)Sqrt((double)area / D(div));
    v = (float)((double)area / s);
    return (float)(v > lim ? lim : (double)v);
}

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline double Sin(double v) { return ((double(__cdecl*)(double))0x5a1494)(v); }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

// Detail level for setting `which` implied by a group level v: settings 10..13
// map it through their own tables (0x6212a8, 0x621300, 0x621250, 0x621358).
static int32_t Options_DetailFor(int32_t which, int32_t v) {
    if (which == 0xc) return ((const int32_t*)0x621250)[v];
    if (which == 0xa) return ((const int32_t*)0x6212a8)[v];
    if (which == 0xb) return ((const int32_t*)0x621300)[v];
    if (which == 0xd) return ((const int32_t*)0x621358)[v];
    return v;
}

namespace {
// The music and effects volumes from the options (+0x1146fa, +0x1146fe x 0.24).
void Scenario_RestoreVolumes() {
    Sound_SetVolume(g_sound, 0, F<float>(g_engine, 0x1146fa), 2);
    Sound_SetVolume(g_sound, 0, (float)((double)F<float>(g_engine, 0x1146fe) * D(0x5c9c80)), 0);
}
}

namespace {
uint8_t* App_New(size_t n) { return ((uint8_t*(__cdecl*)(size_t))0x53b070)(n); }
void App_Delete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
// Destroys screen window w of kind `screen` (1 game view .. 7 campaign).
void App_DestroyScreen(int32_t screen, uint8_t* w) {
    switch (screen) {
    case 1:
        ViewModesPanel_Close();
        if (uint8_t* v = w) {
            View_Destroy(v);
            App_Delete(v);
        }
        Panel6d0820_Close();
        Panel6d081c_Close();
        return;
    case 2:
        if (w) Setup_Destruct(w), App_Delete(w);
        return;
    case 3:
        if (w) VideoWindow_Destruct(w), App_Delete(w);
        return;
    case 4:
        if (w) TitleScreen_Destruct(w), App_Delete(w);
        return;
    case 5:
        if (w) OptionsScreen_Destruct(w), App_Delete(w);
        return;
    case 6:
        if (w) Credits_Destruct(w), App_Delete(w);
        return;
    case 7:
        if (w) CampaignWindow_Destruct(w), App_Delete(w);
        return;
    }
}
}

namespace {
void App_CloseSession() {
    if (uint8_t* s = g_session) {
        Session_Shutdown(s);
        App_Delete(s);
        g_session = nullptr;
    }
}
}

namespace {
void Callback_Free(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
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

static void Method(uint32_t address, void* self) { hook::Orig<void(__fastcall*)(void*)>(address)(self); }

static void Call(uint32_t address) { hook::Orig<void(*)()>(address)(); }

// 0x482150. Input_PostEvent on the game's input (0x6d4018).
RT3_NOINLINE void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    Input_PostEvent(g_input, 0, type, a, b, c, d);
}
RT3_REPLACE(0x482150, Game_PostInputEvent);

// 0x482160. App a: redraws (nesting counter +0x64): asks for a world
// repaint, draws the windows (unless +0xa0) and, in a multiplayer game,
// the lobby queue (0x6cd8d8), then the engine frame.
RT3_NOINLINE void __fastcall App_Redraw(uint8_t* a) {
    ++F<int32_t>(a, 0x64);
    if (hook::As<void*>(g_world)) Engine_RequestRepaint(hook::As<void*>(g_engine));
    if (g_windowManager) {
        if (!F<int32_t>(a, 0xa0)) WindowManager_Draw(g_windowManager);
        if (hook::As<void*>(g_world) && IsMultiplayer() && g_session) Lobby_DrawQueue(g_session);
    }
    Engine_DrawFrame(g_engine);
    --F<int32_t>(a, 0x64);
}
RT3_REPLACE(0x482160, App_Redraw);

// 0x4821d0. Rebuilds the scene's 144-step day lighting from the keyframe set
// (options +0x2c3: 0 / 1; 17 keys of 0x18 bytes at 0x620ea0: time, ambient,
// diffuse, range and two extras), interpolated per step, blended towards the
// set's fixed light (+0x620f64..) by options +0x319 / 0x5c85f0 (at most 1),
// the diffuse scaled by (|sin| of the time of day + 2) / 3, clamped to 0..1.
// A step past the last key keeps (and blends again) the previous step's
// values, as in the original.
RT3_NOINLINE void __fastcall Lighting_Rebuild(uint8_t* opts) {
    int32_t set = F<int32_t>(opts, 0x2c3) != 0;
    const uint8_t* keys = (const uint8_t*)0x620ea0 + set * 0x198;
    const uint8_t* fixed = (const uint8_t*)0x620f64 + set * 0x198;
    auto K = [](const uint8_t* p, uint32_t off) { return *(const float*)(p + off); };
    float amb = 0.0f, ambTop = 0.0f, dif = 0.0f, difTop = 0.0f, range = 0.0f;
    for (int32_t i = 0; i < 0x90; ++i) {
        float fi = (float)i;
        float t = (float)((double)fi / Fl_205(0x5cece8) * Fl_205(0x5c8968));
        int32_t k = 0;
        while (k < 0x11 && !(t < K(keys, k * 0x18))) ++k;
        if (k < 0x11) {
            const uint8_t* cur = keys + k * 0x18;
            const uint8_t* prev = cur - 0x18;
            if (k > 0) {
                float f = (float)(((double)t - K(prev, 0)) / ((double)K(cur, 0) - K(prev, 0)));
                double g = D(0x5c8770) - f;
                amb = (float)(K(prev, 4) * g + (double)f * K(cur, 4));
                ambTop = (float)(((double)K(prev, 0x10) + K(prev, 4)) * g + ((double)K(cur, 0x10) + K(cur, 4)) * f);
                dif = (float)(K(prev, 8) * g + (double)f * K(cur, 8));
                difTop = (float)(((double)K(prev, 0x14) + K(prev, 8)) * g + ((double)K(cur, 0x14) + K(cur, 8)) * f);
                range = (float)(K(prev, 0xc) * g + (double)f * K(cur, 0xc));
            } else {
                *(uint32_t*)&amb = *(const uint32_t*)(cur + 4);
                *(uint32_t*)&dif = *(const uint32_t*)(cur + 8);
                ambTop = (float)((double)K(cur, 0x10) + K(cur, 4));
                difTop = (float)((double)K(cur, 0x14) + K(cur, 8));
                *(uint32_t*)&range = *(const uint32_t*)(cur + 0xc);
            }
        }
        double d = (double)F<int32_t>(g_app, 0x319) / D(0x5c85f0);
        double sd = D(0x5c8770) < d ? D(0x5c8770) : d;
        float s = (float)sd;
        float tmp = (float)((double)s * K(fixed, 0));
        double om = D(0x5c8770) - s;
        amb = (float)(amb * om + tmp);
        ambTop = (float)(ambTop * om + tmp + K(fixed, 0xc));
        tmp = (float)((double)s * K(fixed, 4));
        dif = (float)(dif * om + tmp);
        difTop = (float)(difTop * om + tmp + K(fixed, 0x10));
        range = (float)((double)s * K(fixed, 8) + range * om);
        float ang = (float)(((double)t - Fl_205(0x5c89a4)) / D(0x5c8978) * D(0x5cece0));
        t = (float)((double(__cdecl*)(double))0x5a152e)(Sin(ang));
        float u = (float)(((double)t + D(0x5c85d0)) / D(0x5c85c8));
        float p = (float)((double)u * dif);
        float q = (float)((double)u * difTop);
        if (Fl_205(0x5c8598) > p) p = 0.0f;
        else if (Fl_205(0x5c85b4) < p) p = 1.0f;
        float qc;
        if (Fl_205(0x5c8598) > q) qc = 0.0f;
        else qc = Fl_205(0x5c85b4) < q ? 1.0f : q;
        if (Fl_205(0x5c8598) > amb) amb = 0.0f;
        else if (Fl_205(0x5c85b4) < amb) amb = 1.0f;
        float r = (float)((double)range * D(0x5cecd8));
        p = (float)((double)p * D(0x5cecd0));
        Scene_SetLightPair1((uint8_t*)Engine_Renderer(g_engine), 0, i, ambTop, qc);
        Scene_SetLightPair0((uint8_t*)Engine_Renderer(g_engine), 0, i, amb, p);
        Scene_SetLightRange((uint8_t*)Engine_Renderer(g_engine), 0, i, r);
    }
    Scene_ApplyLighting((uint8_t*)Engine_Renderer(g_engine));
}
RT3_REPLACE(0x4821d0, Lighting_Rebuild);

// 0x4825d0. Minutes of the scene clock since 5:15 (wrapping past midnight); 0 without a renderer.
RT3_NOINLINE float SceneClock_MinutesSinceDawn() {
    void* e = hook::As<void*>(g_engine);
    if (!e || !Engine_Renderer(e)) return *(const float*)0x5c8598;
    int32_t h, m, s;
    Clock_SplitD((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, &h, &m, &s);
    double v = (double)m / *(const double*)0x5c88a8 + (double)s / *(const double*)0x5c9f30;
    float f = (float)(v + (double)h);
    if ((double)f < *(const double*)0x5cecf0) f = (float)((double)f + *(const float*)0x5c8968);
    return (float)(((double)f - *(const double*)0x5cecf0) * *(const double*)0x5c88a8);
}
RT3_REPLACE(0x4825d0, SceneClock_MinutesSinceDawn);

// 0x482690. Night factor (0 day .. 1 night) for the renderer clock: full night
// before 4:20 and after 20:51, ramps through dawn (to 6:51) and dusk (from 18:25).
RT3_NOINLINE float __cdecl Sky_NightFactor() {
    void* eng = hook::As<void*>(g_engine);
    if (!eng || !Engine_Renderer(eng)) return *(const float*)0x5c8598;
    int32_t h, m, s;
    Clock_SplitD((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, &h, &m, &s);
    float t;
    __asm {
        fild m
        fdiv qword ptr ds:[0x5c88a8]
        fild s
        fdiv qword ptr ds:[0x5c9f30]
        faddp st(1), st
        fiadd h
        fstp t
    }
    float r;
    if (t < D(0x5c8978)) {
        if (t < D(0x5ced38)) return 1.0f;
        if (t < D(0x5cecf0)) r = (float)((D(0x5cecf0) - t) / D(0x5ced30) * D(0x5c88e8) + D(0x5c8880));
        else if (t < D(0x5ced28)) r = (float)((D(0x5ced28) - t) / D(0x5ced20) * D(0x5c8880));
        else return 0.0f;
    } else {
        if (t < D(0x5ced18)) return 0.0f;
        if (t < D(0x5ced10)) r = (float)((t - D(0x5ced18)) / D(0x5ced08) * D(0x5c88c8));
        else if (t < D(0x5ced00)) r = (float)((t - D(0x5ced10)) / D(0x5cecf8) * D(0x5c8680) + D(0x5c88c8));
        else return 1.0f;
    }
    if (*(const float*)0x5c8598 > r) return 0.0f;
    if (*(const float*)0x5c85b4 < r) return 1.0f;
    return r;
}
RT3_REPLACE(0x482690, Sky_NightFactor);

RT3_NOINLINE int32_t Option1df() { return F<int32_t>(g_app, 0x1df); }
RT3_REPLACE(0x482840, Option1df);

// 0x482850. Sets the fixed weather override (options +0x1df on/off, level +0x1e3)
// and reapplies the weather intensity.
RT3_NOINLINE void __stdcall Options_SetWeatherOverride(int32_t on, int32_t level) {
    F<int32_t>(g_app, 0x1df) = on;
    F<int32_t>(g_app, 0x1e3) = level;
    if (uint8_t* w = g_world) Weather_ApplyIntensity(w);
}
RT3_REPLACE(0x482850, Options_SetWeatherOverride);

// 0x482880. Fixes the game's time of day (+0x124 on/off, +0x128 hour) and
// refreshes the world clock.
RT3_NOINLINE void __stdcall Game_SetFixedTime(int32_t on, int32_t hour) {
    F<int32_t>(g_app, 0x124) = on;
    F<int32_t>(g_app, 0x128) = hour;
    if (uint8_t* w = g_world) World_RefreshTime(w);
}
RT3_REPLACE(0x482880, Game_SetFixedTime);

// 0x4828b0 (dev command). Cycles the cursor between game drawn, Windows
// (engine +0x11473a) and system, saving the options.
int32_t Cmd_CycleCursor() {
    uint8_t* c = g_cursorManager;
    if (!Obj_Get18(c)) Cursor_UseGameDrawn(g_cursorManager);
    else if (F<int32_t>(g_engine, 0x11473a)) Cursor_UseWindows(g_cursorManager);
    else Cursor_UseSystem(g_cursorManager);
    Options_Save(g_app, 0, 1);
    return 0;
}
RT3_REPLACE(0x4828b0, Cmd_CycleCursor);

// 0x482920. Steps the engine setting +0x1146c2 by delta, wrapping -1..4 (-1 shows
// as "off"), reports the new value on the status line for 3 s, refreshes the menu
// checks and saves the options.
RT3_NOINLINE int32_t __fastcall Engine_StepSetting(int32_t delta) {
    F<int32_t>(g_engine, 0x1146c2) += delta;
    if (F<int32_t>(g_engine, 0x1146c2) > 4) F<int32_t>(g_engine, 0x1146c2) = -1;
    if (F<int32_t>(g_engine, 0x1146c2) < -1) F<int32_t>(g_engine, 0x1146c2) = 4;
    int32_t v = F<int32_t>(g_engine, 0x1146c2);
    char num[0x12c], msg[0x12c];
    const char* text = v == -1 ? GetString(0x2f8) : Num_Format(num, 1, (double)v, 10, 1, 0, 0);
    FormatText(msg, GetString(0x2f7), text, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    StatusLine_Set(g_windowManager, 0, msg, 3000);
    Menu_UpdateChecks(hook::As<HMENU*>(g_6ada8c), 0, 0);
    Options_Save(g_app, 0, 1);
    return 0;
}
RT3_REPLACE(0x482920, Engine_StepSetting);

// 0x482a10 / 0x482a20, 0x482ae0 / 0x482af0, 0x483990 / 0x4839a0 (hotkeys).
// Step the engine setting, detail level and resolution up / down.
int32_t Hotkey_SettingUp() { return Engine_StepSetting(1), 0; }
RT3_REPLACE(0x482a10, Hotkey_SettingUp);

int32_t Hotkey_SettingDown() { return Engine_StepSetting(-1), 0; }
RT3_REPLACE(0x482a20, Hotkey_SettingDown);

// 0x482a30. Steps the overall detail level by `dir` (wrapping over 21
// levels), shows it for three seconds (0x2fa), updates the menu and saves
// the options.
RT3_NOINLINE int32_t __fastcall Options_StepDetail(int32_t dir) {
    int32_t level = (Obj_ArrayAc2(hook::As<void*>(g_app), 0, 0) + dir + 0x15) % 0x15;
    Options_SetDetail(g_app, 0, 0, level, 1);
    char num[0x40], text[0x12c];
    const char* s = Num_Format(num, 1, (double)level, 10, 1, 0, 0);
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(text, GetString(0x2fa), s);
    StatusLine_Set(g_windowManager, 0, text, 0xbb8);
    Menu_UpdateChecks(hook::As<HMENU*>(g_6ada8c), 0, 0);
    Options_Save(g_app, 0, 1);
    return 0;
}
RT3_REPLACE(0x482a30, Options_StepDetail);

int32_t Hotkey_DetailUp() { return Options_StepDetail(1), 0; }
RT3_REPLACE(0x482ae0, Hotkey_DetailUp);

int32_t Hotkey_DetailDown() { return Options_StepDetail(-1), 0; }
RT3_REPLACE(0x482af0, Hotkey_DetailDown);

// 0x482b00 (hotkey). Opens the options dialog unless it is open.
int32_t Hotkey_Options() {
    if (!Global6d1350Set()) OptionsDialog_Open();
    return 0;
}
RT3_REPLACE(0x482b00, Hotkey_Options);

// 0x482b20 (hotkey). Takes a screenshot (sound 0x3e in full screen; the
// cursor hidden in view mode 0x25); failure shows 0x2fb.
int32_t Hotkey_Screenshot() {
    if (!g_engine) return 0;
    if (Engine_IsFullscreen(hook::As<void*>(g_engine))) Ui_PlaySound(0x3e, 0, 0x3f000000);
    const int32_t hide = g_world && Global6d0818Is25() ? 1 : 0;
    if (!Engine_Screenshot(g_engine, 0, hide)) Message_Dialog444(GetString(0x2fb));
    return 0;
}
RT3_REPLACE(0x482b20, Hotkey_Screenshot);

// 0x482bb0 (dev command). Takes a poster screenshot (sound 0x3e) in full
// screen with a view; else says why (0x2fc / 0x2fd).
int32_t Cmd_PosterScreenshot() {
    if (!Engine_IsFullscreen(hook::As<void*>(g_engine))) {
        Message_Dialog(GetString(0x2fc), 0xbf800000);
        return 0;
    }
    if (!g_gameWindow) {
        Message_Dialog(GetString(0x2fd), 0xbf800000);
        return 0;
    }
    Ui_PlaySound(0x3e, 0, 0x3f000000);
    Engine_PosterScreenshot(g_engine);
    return 0;
}
RT3_REPLACE(0x482bb0, Cmd_PosterScreenshot);

int32_t Cmd_ToggleSysInfo() { return g_6cec60 = !g_6cec60, 0; }
RT3_REPLACE(0x482c30, Cmd_ToggleSysInfo);

int32_t Cmd_ToggleEngineText1() { return g_6cec64 = !g_6cec64, 0; }
RT3_REPLACE(0x482c50, Cmd_ToggleEngineText1);

int32_t Cmd_ToggleEngineText2() { return g_6cec68 = !g_6cec68, 0; }
RT3_REPLACE(0x482c70, Cmd_ToggleEngineText2);

// 0x41c700. Moves a train car onto track segment seg (-1: off the track) facing
// dir: relinks it, and when the segment changes marks the new one worn and blocked.
void __fastcall Car_MoveToSegment(uint8_t* car, int, int32_t seg, int32_t dir) {
    uint8_t* s = seg != -1 ? (uint8_t*)Collection_Get(g_tracks, 0, (uint32_t)seg) : nullptr;
    Obj_UnlinkFromNode(car);
    Car_LinkToNode(car, 0, seg, F<int32_t>(car, 0x35a));
    if (F<int32_t>(car, 0x356) != seg) {
        F<int32_t>(car, 0x356) = seg;
        if (seg != -1) {
            Track_AddWear(s);
            Track_SetBlocked(s, 0, 1);
        }
    }
    F<int32_t>(car, 0x35e) = dir;
}

// Kept original (not hooked): with this rewrite the simcheck run diverges late
// (vehicle smoke emitters launch fewer particles, shifting the shared LCG 0x624324),
// although the code matches the original instruction for instruction; the cause
// looks like stack residue read by original render code. See dumpready's skip list.
// RT3_REPLACE(0x41c700, Car_MoveToSegment);

// 0x412c10. Marks which industry types are available: processing types (+0x32
// == 2, not flagged +0xba / +0xbb) by the scenario's named list, all others always;
// then updates the cargo types.
void __fastcall IndustryTypes_MarkAvailable(Collection* t) {
    if (!hook::As<void*>(g_world)) return;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (!r[0xba] && !r[0xbb] && r[0x32] == 2) F<int32_t>(r, 0x7ac) = World_LookupNamed1(hook::As<void*>(g_world), 0, (const char*)r + 4);
        else F<int32_t>(r, 0x7ac) = 1;
    }
    if (g_cargoTypes) CargoTypes_MarkAvailable(g_cargoTypes);
}

// 0x458140. Turns a view-space direction v into world space through the inverse of
// the camera matrix (+0x18e), crosses it with w, and returns the normalized result
// in out with its length before normalizing in *len. Works on its stack
// arguments in place, as the original does.
float* __fastcall Camera_WorldCross(uint8_t* cam, int, float* out, float vx, float vy, float vz, float wx, float wy, float wz, float* len) {
    float inv[16], det, t[3];
    ((void*(__stdcall*)(void*, float*, const void*))0x56d8d9)(inv, &det, cam + 0x18e);
    float* r = Vec3_Transform(&vx, 0, t, inv);
    memcpy(&vx, r, 12);
    Vec3_CrossInPlace(&vx, 0, &wx);
    *len = (float)Vec3_Length(&vx);
    Vec3_Normalize(&vx);
    memcpy(out, &vx, 12);
    return out;
}

// 0x482c90. Selects display mode m (option +0x10c; also kept as the last
// windowed +0x1bb or full-screen +0x1b7 choice by the mode's kind, table 0x5ee4d0),
// saves the options and refreshes the menu and the view; the Scroll Lock light
// follows full-screen modes when the keyboard model value is 0x91.
RT3_NOINLINE void __fastcall Options_SetDisplayMode(uint8_t* o, int, int32_t m) {
    if (m != F<int32_t>(o, 0x10c)) {
        if (*(uint8_t*)(0x5ee4d0 + m)) F<int32_t>(o, 0x1b7) = m;
        else F<int32_t>(o, 0x1bb) = m;
        F<int32_t>(o, 0x10c) = m;
        Options_Save(o, 0, 1);
        if (hook::As<HMENU*>(g_6ada8c)) Menu_UpdateChecks(hook::As<HMENU*>(g_6ada8c), 0, 0);
        if (hook::As<void*>(g_62be68)) Obj_Nudge92(hook::As<void*>(g_62be68));
    }
    if (Models_ValueByName((const char*)0x5ced40) == 0x91) Keyboard_SetScrollLock(*(uint8_t*)(0x5ee4d0 + m) != 0);
}
RT3_REPLACE(0x482c90, Options_SetDisplayMode);

// 0x482d10. The game version as a number (major + minor / 0x5c8c28), cached.
RT3_NOINLINE float __fastcall Game_VersionNumber() {
    if (!g_6cec84) {
        g_6cec84 = 1;
        uint32_t v = File_Version((const char*)0x5cec18);
        float lo = (float)(int32_t)(v & 0xffff);
        float hi = (float)(int32_t)(v >> 16);
        g_6cec5c = (float)((double)lo / *(const float*)0x5c8c28 + hi);
    }
    return g_6cec5c;
}
RT3_REPLACE(0x482d10, Game_VersionNumber);

// 0x482d80. The localised version text (string 0xf54 with "major.minor"), cached in 0x6cec24.
RT3_NOINLINE const char* __fastcall Game_VersionText() {
    if (!g_6cec85) {
        g_6cec85 = 1;
        char buf[0x34];
        uint32_t lo = File_Version((const char*)0x5cec18) & 0xffff;
        uint32_t hi = File_Version((const char*)0x5cec18) >> 16;
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(buf, (const char*)0x5ced5c, hi, lo);
        FormatText((char*)0x6cec24, GetString(0xf54), buf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    return (const char*)0x6cec24;
}
RT3_REPLACE(0x482d80, Game_VersionText);

// 0x482e00. The effective game version integer: the session's common version
// online or with several human players, else our own.
RT3_NOINLINE int32_t __fastcall Game_EffectiveVersion() {
    if (!hook::As<void*>(g_session) && !(hook::As<void*>(g_world) && CountHumanPlayers() > 1))
        return VersionInt(Game_VersionNumber());
    float v = Net_CommonVersion();
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}
RT3_REPLACE(0x482e00, Game_EffectiveVersion);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x482e50. Toggle the application's view mode, its panel and route-cost
// overlay, then apply graphics option 13. Repeated values are a no-op.
RT3_NOINLINE void __fastcall App_SetViewMode(uint8_t* app, int, int32_t mode) {
    if (F<int32_t>(app, 0x68) == mode) return;
    F<int32_t>(app, 0x68) = mode;
    if (!mode) {
        if (hook::As<void*>(g_gameWindow)) {
            Call(0x4d44a0);
            View_ResetEditorTool(g_gameWindow);
        }
        if (hook::As<void*>(g_map)) Method(0x44fb70, hook::As<void*>(g_map));
    } else if (hook::As<void*>(g_gameWindow)) Call(0x4d4500);
    hook::Orig<void(__fastcall*)(void*, int, int32_t, int32_t)>(0x484d70)(app, 0, 13, 1);
}
RT3_REPLACE(0x482e50, App_SetViewMode);

 // namespace

// 0x482ec0 (thiscall, RET 8). App a switches to screen `screen` (+8: 1 game
// view, 2 setup, 3 video, 4 title, 5 options, 6 credits, 7 campaign; +0x60
// marks none), unless the UI is missing or it is already shown. The busy
// cursor goes up, the old window (+0xc) is removed and destroyed (the game
// view also closes its panels); `closeWorld` tears the world down. The game
// view first needs a world: without one the title screen shows (98 / 118 /
// 212 progress by lobby mode and editor) while a new world starts up, then
// the menu music fades (unless a campaign with 0x6cfd20), multiplayer tells
// the peers (0x68), the view and its panels open and the tool mode becomes
// 0 in a multiplayer game with companies, else 7 (editor: the editor panel;
// not for a company-less world 0x434870, auto companies or 0x6d3b4c). The
// other screens are built at 0.95 opacity. Finally the music and effect
// volumes are restored and the cursor comes back.
RT3_NOINLINE void __fastcall App_SetScreen(uint8_t* a, int, int32_t screen, int32_t closeWorld) {
    if (!g_windowManager || screen == F<int32_t>(a, 8)) return;
    F<int32_t>(a, 0x60) = screen == 0;
    if (F<int32_t>(a, 8)) {
        Cursor_PushBusy(g_cursorManager);
        App_Redraw(g_app);
    }
    if (uint8_t* old = F<uint8_t*>(a, 0xc)) {
        Windows_Remove(g_windowManager, 0, old);
        App_DestroyScreen(F<int32_t>(a, 8), F<uint8_t*>(a, 0xc));
        F<uint8_t*>(a, 0xc) = nullptr;
    }
    if (closeWorld && g_world) {
        World_Teardown(g_world);
        if (uint8_t* w = g_world) {
            Nop433730(w);
            App_Delete(w);
        }
        g_world = nullptr;
    }
    auto open = [&](uint8_t* win) {
        F<uint8_t*>(a, 0xc) = win;
        WidgetList_Insert(hook::As<uint8_t**>(g_windowManager), 0, win, 1);
    };
    auto dim = [] { Obj_Setc5c(g_windowManager, 0, 0x3f733333); };
    switch (screen) {
    case 1: {
        if (!g_world) {
            dim();
            uint8_t* m = App_New(0x7c);
            open(m ? LoadScreen_Construct(m) : nullptr);
            F<int32_t>(a, 8) = 4;
            g_skipCount = 0;
            const int32_t mode = F<int32_t>(g_newGame, 1);
            const int32_t progress = mode == 5 || mode != 3 ? 0x43540000 : F<int32_t>(g_app, 0x68) ? 0x42c40000 : 0x42ec0000;
            Obj_Set78(F<uint8_t*>(a, 0xc), 0, progress);
            uint8_t* wm = App_New(0x46c40);
            uint8_t* w = wm ? (uint8_t*)World_Construct(wm) : nullptr;
            g_world = w;
            World_Startup(w, 0, 1, 0);
            Windows_Remove(g_windowManager, 0, F<uint8_t*>(a, 0xc));
            uint8_t* win = F<uint8_t*>(a, 0xc);
            if (F<int32_t>(a, 8) == 7) {
                if (win) CampaignWindow_Destruct(win), App_Delete(win);
            } else if (F<int32_t>(a, 8) == 4) {
                if (win) TitleScreen_Destruct(win), App_Delete(win);
            } else if (win) {
                View_Destroy(win), App_Delete(win);
            }
            F<int32_t>(a, 8) = 4;
        }
        if (F<int32_t>(a, 8) == screen) break;
        uint8_t* w = g_world;
        if (!w[0x66de] || !g_6cfd20) Sound_StartFadeTimer(g_sound, 0, 1);
        if (IsMultiplayer()) Net_Send(g_session, 0, nullptr, 0x68, 0, nullptr, 1, 0);
        Obj_Setc5c(g_windowManager, 0, 0x3e800000);
        uint8_t* m = App_New(0x9a);
        open(m ? GameWindow_Construct(m) : nullptr);
        F<int32_t>(a, 8) = 1;
        GameMessagePanel_Open();
        SetupWindow_Open();
        Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x808, 0, 0, 0);
        if (F<int32_t>(g_app, 0x68)) {
            EditorPanel_Open();
        } else if (g_world && g_gameWindow && !Obj_Company21(g_world) &&
                   !g_newGame[0x82] && !g_6d3b4c) {
            if (IsMultiplayer() && g_companies && Table_CountField3f(g_companies) > 0)
                View_SetToolMode(g_gameWindow, 0, 0, -1);
            else
                View_SetToolMode(g_gameWindow, 0, 7, -1);
        }
        break;
    }
    case 2: {
        dim();
        uint8_t* m = App_New(0x7f);
        open(m ? SetupScreen_Construct(m) : nullptr);
        break;
    }
    case 3: {
        dim();
        uint8_t* m = App_New(0x7b);
        open(m ? VideoWindow_Construct(m) : nullptr);
        break;
    }
    case 4: {
        dim();
        uint8_t* m = App_New(0x7c);
        open(m ? LoadScreen_Construct(m) : nullptr);
        break;
    }
    case 5: {
        dim();
        uint8_t* m = App_New(0x84);
        open(m ? ((uint8_t*(__fastcall*)(uint8_t*))0x4efe80)(m) : nullptr);
        break;
    }
    case 6: {
        dim();
        uint8_t* m = App_New(0x3ed74);
        open(m ? CreditsWindow_Construct(m) : nullptr);
        break;
    }
    case 7: {
        dim();
        uint8_t* m = App_New(0xa1);
        open(m ? CampaignWindow_Construct(m) : nullptr);
        break;
    }
    }
    Scenario_RestoreVolumes();
    if (F<int32_t>(a, 8)) Cursor_PopBusy(g_cursorManager);
    F<int32_t>(a, 8) = screen;
}
RT3_REPLACE(0x482ec0, App_SetScreen);

// 0x483450 / 0x4834a0. Nested pause while a modal window is open (count +0,
// +4 = the game was already paused); not in multiplayer.
RT3_NOINLINE void __fastcall PauseLock_Push(int32_t* p) {
    uint8_t* w = g_world;
    if (w && IsMultiplayer()) return;
    if (!p[0]) {
        p[1] = w ? F<int32_t>(w, 0x19) == 0 : 0;
        w = g_world;
        if (w && F<int32_t>(w, 0x19)) World_TogglePause(w, 0, 0);
    }
    ++p[0];
}
RT3_REPLACE(0x483450, PauseLock_Push);

RT3_NOINLINE void __fastcall PauseLock_Pop(int32_t* p) {
    uint8_t* w = g_world;
    if (w && IsMultiplayer()) return;
    if (--p[0]) return;
    if (p[1]) return;
    w = g_world;
    if (w && !F<int32_t>(w, 0x19)) World_TogglePause(w, 0, 0);
}
RT3_REPLACE(0x4834a0, PauseLock_Pop);

// 0x4834e0. Loading screen l (unless +0x60): counts a step (`step`, into
// 0xccb9e8) and, at most every 16 ms unless `force` (0x6cfc88), pumps
// messages, moves the progress bar (widget 0x84d1 +0x13: steps / expected
// +0x78, plus 0x5caa9c in multiplayer, capped at 1, x 0x5ced68) and redraws.
// In multiplayer every 8th update (0x6cec80) also runs the session
// (0x46b780); one that fails is shut down and freed.
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force) {
    if (F<int32_t>(l, 0x60)) return;
    if (step) ++g_skipCount;
    if (!g_6cfc88) g_6cfc88 = ClockMs();
    if (ClockMs() - g_6cfc88 <= 0xf && !force) return;
    PumpMessages();
    if (F<uint8_t*>(l, 0xc) && Widget_FindChild(F<uint8_t*>(l, 0xc), 0, 0x84d1)) {
        float expected = F<float>(F<uint8_t*>(l, 0xc), 0x78);
        if (g_world && IsMultiplayer()) expected = (float)(expected + Fl_207(0x5caa9c));
        if (expected < Fl_207(0x5c85b4)) expected = 1.0f;
        float frac = (float)((double)(float)g_skipCount / expected);
        if (frac > D(0x5c8770)) frac = 1.0f;
        float v = (float)(frac * D(0x5ced68));
        F<float>(Widget_FindChild(F<uint8_t*>(l, 0xc), 0, 0x84d1), 0x13) = v;
        WindowManager_Draw(g_windowManager);
        Engine_DrawFrame(g_engine);
    }
    if (g_world && IsMultiplayer() && !F<int32_t>(g_world, 0x66ae) && g_session) {
        int32_t n = ++g_6cec80;
        if (!(n & 7) && !((int32_t(__fastcall*)(uint8_t*))0x46b780)(g_session)) {
            if (uint8_t* s = g_session) {
                Session_Shutdown(s);
                ((void(__cdecl*)(void*))0x53b080)(s);
                g_session = nullptr;
            }
        }
    }
    g_6cfc88 = ClockMs();
}
RT3_REPLACE(0x4834e0, Loading_Pump);

// 0x483680. Toggles full screen. Leaving it with a mode at least the desktop
// size (+0x38 / +0x3c) picks the largest standard size (0x620e64 /
// 0x620e7c) below the desktop, else 512 x 384, and saves it; otherwise just
// switches. Updates the menu checks and saves the options; 0xe8d tells about
// a mode that stayed full screen.
RT3_NOINLINE int32_t Options_ToggleFullscreen() {
    uint8_t* eng = g_engine;
    uint8_t fs = eng[0x114226];
    int32_t dw = F<int32_t>(eng, 0x38), dh = F<int32_t>(eng, 0x3c);
    uint8_t* cur = F<uint8_t*>(eng, 0x34) + F<int32_t>(eng, 0x11421e) * 0x15;
    if (fs && F<int32_t>(cur, 0) >= dw && F<int32_t>(cur, 4) >= dh) {
        int32_t i = 5;
        for (; i >= 0; --i)
            if (((const int32_t*)0x620e64)[i] < dw && ((const int32_t*)0x620e7c)[i] < dh) break;
        if (i < 0) Engine_SetDisplayMode(eng, 0, 0x200, 0x180, 0, 1);
        else Engine_SetDisplayMode(eng, 0, ((const int32_t*)0x620e64)[i], ((const int32_t*)0x620e7c)[i], 0, 1);
    } else
        Engine_ResetDisplay(eng, 0, fs == 0, -1);
    Menu_UpdateChecks(hook::As<HMENU*>(g_6ada8c), 0, 0);
    Options_Save(g_app, 0, 1);
    if (fs && g_engine[0x114226]) Message_Show(GetString(0xe8d), 1, 0);
    return 0;
}
RT3_REPLACE(0x483680, Options_ToggleFullscreen);

// 0x483780. Steps the display resolution by `dir` through the six standard
// sizes (0x620e64 / 0x620e7c), skipping sizes bigger than the screen (full
// screen 1600 x 1280, else the desktop less the frame; at most 50 steps).
// Not in mode 3 or without an engine. Saves the size, shows "w x h" for
// three seconds, updates the menu, saves the options and rebuilds the
// lighting. QUIRK: landing on index 0 restarts from (dir + 6) % 6 (during the
// first 20 steps).
RT3_NOINLINE void __fastcall Options_StepResolution(int32_t dir) {
    if (F<int32_t>(g_app, 8) == 3) return;
    uint8_t* eng = g_engine;
    if (!eng) return;
    int32_t cw = F<int32_t>(F<uint8_t*>(eng, 0x34) + F<int32_t>(eng, 0x11421e) * 0x15, 0);
    int32_t idx = cw <= 0x200 ? 0 : cw <= 0x280 ? 1 : cw <= 0x320 ? 2 : cw <= 0x400 ? 3 : (cw > 0x500) + 4;
    int32_t k = (idx + dir + 6) % 6;
    if (!k) k = (dir + 6) % 6;
    int32_t maxW = 0x640, maxH = 0x500;
    if (!eng[0x114226]) {
        maxW = F<int32_t>(eng, 0x38) - 0x14;
        maxH = F<int32_t>(eng, 0x3c) - 0x1e;
    }
    const int32_t* tw = (const int32_t*)0x620e64;
    const int32_t* th = (const int32_t*)0x620e7c;
    for (int32_t tries = 0; tw[k] > maxW || th[k] > maxH;) {
        if (tries++ >= 0x32) break;
        k = (dir + 6 + k) % 6;
        if (!k && tries < 0x14) k = (dir + 6) % 6;
    }
    Engine_SetDisplayMode(eng, 0, tw[k], th[k], -1, 1);
    uint8_t* m = F<uint8_t*>(g_engine, 0x34) + F<int32_t>(g_engine, 0x11421e) * 0x15;
    int32_t mw = F<int32_t>(m, 0), mh = F<int32_t>(m, 4);
    char nh[0x40], nw[0x40], text[0x128];
    const char* sh = Num_Format(nh, 8, (double)mh, 10, 1, 0, 0);
    const char* sw = Num_Format(nw, 8, (double)mw, 10, 1, 0, 0);
    FormatText(text, (const char*)0x5ced70, sw, sh, 0, 0, 0, 0, 0, 0);
    StatusLine_Set(g_windowManager, 0, text, 0xbb8);
    if (hook::As<HMENU*>(g_6ada8c)) Menu_UpdateChecks(hook::As<HMENU*>(g_6ada8c), 0, 0);
    Options_Save(g_app, 0, 1);
    if (uint8_t* o = g_app) Lighting_Rebuild(o);
}
RT3_REPLACE(0x483780, Options_StepResolution);

int32_t Hotkey_ResolutionUp() { return Options_StepResolution(1), 0; }
RT3_REPLACE(0x483990, Hotkey_ResolutionUp);

int32_t Hotkey_ResolutionDown() { return Options_StepResolution(-1), 0; }
RT3_REPLACE(0x4839a0, Hotkey_ResolutionDown);

// 0x4839b0. Option +0x1bf on, and (option +0x68 or this +0x6c == 1).
RT3_NOINLINE int32_t __fastcall Options_Check1bf(void* self) {
    uint8_t* opt = g_app;
    if (F<int32_t>(opt, 0x1bf) == 0) return 0;
    return (F<int32_t>(opt, 0x68) != 0 || F<int32_t>(self, 0x6c) == 1) ? 1 : 0;
}
RT3_REPLACE(0x4839b0, Options_Check1bf);

// 0x4839e0. Folder for saves/maps from the options object.
RT3_NOINLINE const char* __fastcall Options_SaveFolder(void* opt) {
    int32_t kind = F<int32_t>(opt, 0x6c);
    if (kind == 2) return (const char*)0x5ca338;                     // "data\tutorial"
    if (F<int32_t>(opt, 0x68) != 0 || kind == 1) return (const char*)0x5ced78; // "maps"
    return (const char*)0x5ced80;                                     // "saved games"
}
RT3_REPLACE(0x4839e0, Options_SaveFolder);

 // namespace

// 0x483a10 (thiscall, RET 4). App a's own event handling: an open UI first
// gets the event through its handler chain (mouse buttons, types 3 / 4,
// only while it is idle) and keeps it unless that returns 1. Type 0 shows
// the debug texts (system info 0x6cec60, two engine texts 0x6cec64 /
// 0x6cec68), 1 runs menu command ev[2] (0x464410), 3 / 4 go to the record
// handlers while the UI is idle, 0xcc switches to screen ev[2] (closing the
// world) and 0xcd quits (+0x10). Returns 0 when handled, else 1.
RT3_NOINLINE int32_t __fastcall App_HandleEvent(uint8_t* a, int, uint32_t* ev) {
    int32_t r = 1;
    if (uint8_t* ui = g_windowManager) {
        if (Obj_Idlec60(ui) || (ev[0] != 3 && ev[0] != 4)) {
            r = Chain_ForEach(g_windowManager, 0, (uint32_t)(uintptr_t)ev);
            if (r != 1) return r;
        }
    }
    uint8_t* const ui = g_windowManager;
    switch (ev[0]) {
    case 0:
        if (!ui) return r;
        if (g_6cec60 == 1) {
            const char* info = SysInfo_Report();
            Status_SetMessage(g_windowManager, 0, info);
        }
        if (g_6cec64 == 1)
            Status_SetMessage(g_windowManager, 0, F<const char*>(g_engine, 0xc) + 0x3c);
        if (g_6cec68 == 1)
            Status_SetMessage(g_windowManager, 0, F<const char*>(g_engine, 0xc) + 0x43c);
        return r;
    case 1:
        if (uint8_t* menu = g_6ada8c) ((void(__fastcall*)(uint8_t*, int, uint32_t))0x464410)(menu, 0, ev[2]);
        return 0;
    case 3:
    case 4:
        if (Obj_Idlec60(ui)) r = Records_Dispatch((uint8_t*)ev);
        return r;
    case 0xcc:
        App_SetScreen(a, 0, (int32_t)ev[2], 1);
        return 0;
    case 0xcd:
        F<int32_t>(a, 0x10) = 1;
        return 1;
    }
    return r;
}
RT3_REPLACE(0x483a10, App_HandleEvent);

// 0x483dd0 (thiscall, RET 4). App a passes event ev to the app, the UI (not
// while the world refuses input, 0x434060) and the world's view mode, in
// turn; a handler returning 0 (or a quit, +0x10) ends it (returns 0); one
// returning 1 that changed the event restarts it from the app. Returns 1
// when nobody handled it.
RT3_NOINLINE int32_t __fastcall App_DispatchEvent(uint8_t* a, int, uint32_t* ev) {
restart:
    uint32_t seen[12];
    memcpy(seen, ev, sizeof seen);
    for (int32_t stage = 0; stage < 3; ++stage) {
        int32_t r;
        if (stage == 0) {
            r = App_HandleEvent(a, 0, ev);
        } else if (stage == 1) {
            if (!g_windowManager) r = 1;
            else if (g_world && !World_IsHostOrLocal(g_world)) r = 1;
            else r = Ui_Dispatch(hook::As<uint8_t**>(g_windowManager), 0, ev);
        } else {
            r = g_world ? View_OnModeInput(ev) : 1;
        }
        if (F<int32_t>(a, 0x10) || r == 0) return 0;
        if (r == 1 && memcmp(ev, seen, sizeof seen)) goto restart;
    }
    return 1;
}
RT3_REPLACE(0x483dd0, App_DispatchEvent);

// 0x483ea0. App a handles queued input for up to 30 ms (waiting 20 ms
// whenever the game is in the background), until the queue is empty.
RT3_NOINLINE void __fastcall App_PumpInput(uint8_t* a) {
    if (!g_input) return;
    const int32_t start = ClockMs();
    auto sleepEx = (uint32_t(__stdcall*)(uint32_t, int32_t))g_5c8064;
    uint32_t ev[12];
    do {
        PumpMessages();
        if (!Engine_IsForeground()) sleepEx(0x14, 1);
        InputEvent out;
        memcpy(ev, Input_PopEvent(g_input, 0, &out), sizeof ev);
        App_DispatchEvent(a, 0, ev);
    } while (ev[0] && ClockMs() - start <= 0x1e);
}
RT3_REPLACE(0x483ea0, App_PumpInput);

// 0x483f20. Dispatches the event (type, a, b, c, d) to app a at once.
RT3_NOINLINE int32_t __fastcall App_DispatchEvent5(uint8_t* app, int, uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint32_t ev[0xc] = {};
    ev[0] = type, ev[1] = a, ev[2] = b, ev[3] = c, ev[4] = d;
    return App_DispatchEvent(app, 0, ev);
}
RT3_REPLACE(0x483f20, App_DispatchEvent5);

// 0x483f70. App a's frame: a pending start (0x6cd920) posts the start event
// (0xcc / 5) once, input is handled and the world runs a frame (not under
// 0x6d1298; its result is returned, else 1). Without a world the music
// fades once (+0x14 +0x114). The engine gets the camera (+0x28) when it has
// none; music, sound, emitters and ambience tick; a dropped multiplayer
// session is closed (unless a dialog closes, 0x6d4114); the screen redraws
// and +0x501 marks the scene for a full refresh.
RT3_NOINLINE int32_t __fastcall App_Frame(uint8_t* a) {
    if (g_6cd920 && !g_6ae3c4) {
        g_6ae3c4 = 1;
        Input_PostEvent(g_input, 0, 0xcc, 0, 5, 0, 0);
    }
    App_PumpInput(a);
    int32_t r = 1;
    if (g_world) {
        if (!g_6d1298) r = World_Frame(g_world);
    }
    if (g_world) {
        F<int32_t>(F<uint8_t*>(a, 0x14), 0x114) = 1;
    } else if (F<int32_t>(F<uint8_t*>(a, 0x14), 0x114)) {
        Sound_StartFadeTimer(g_sound, 0, 1);
        F<int32_t>(F<uint8_t*>(a, 0x14), 0x114) = 0;
    }
    if (!F<void*>(g_engine, 0x28)) F<void*>(g_engine, 0x28) = hook::As<void*>(g_62be68);
    Playlist_Tick(F<uint8_t*>(a, 0x14));
    Sound_Pause(g_sound);
    EmitterGroup_Tick(nullptr);
    Ambience_Tick(g_models, 0, 0);
    if (g_session && !Session_Update(g_session) && !g_6d4114) App_CloseSession();
    App_Redraw(a);
    if (F<int32_t>(a, 0x501)) {
        F<int32_t>(a, 0x501) = 0;
        ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x3748] = 1;
        ((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)))[0x3747] = 1;
    }
    return r;
}
RT3_REPLACE(0x483f70, App_Frame);

// 0x4840e0 (thiscall, RET 4). App a runs the game: window callbacks, the
// menu (0x6ada8c), options, display mode and engine (640 x 480 the first
// time, editor flag +0xa8 unset with intro mode 0x6cec70 0 or 9), save
// format 0x2648 (0x26ac with the 0x5cecbc pack), hotkeys and the localized
// names of the two tables 0x6103a0 (0x6e-byte records, id +0x62) and
// 0x5f3d9c (0x81, +0x69), graphics, menu checks, lighting and music. The
// first run saves the options and plays the intro video (screen 3); later
// runs show the setup screen with the arrow cursor, the music (when it has
// tracks) and two 600 ms pauses around the speaker setup. Then frames run
// until quit and everything is torn down.
RT3_NOINLINE void __fastcall App_Run(uint8_t* a, int, void* inst) {
    F<int32_t>(a, 0x10) = 0, F<int32_t>(a, 0xc) = 0, F<int32_t>(a, 8) = 0, F<int32_t>(a, 0x60) = 0;
    F<int32_t>(a, 0x68) = 0, F<int32_t>(a, 0x6c) = 0, F<int32_t>(a, 0) = 0, F<int32_t>(a, 4) = 0;
    hook::As<uint32_t>(g_6d4004) = 0x4840d0, hook::As<uint32_t>(g_6d4008) = 0x483dc0;
    g_6d400c = 0x482130, g_6d4010 = 0x482140;
    {
        uint8_t* m = App_New(8);
        hook::As<uint32_t*>(g_6ada8c) = m ? Pair_Init((uint32_t*)m) : nullptr;
    }
    int32_t w = -1, h = -1;
    Options_Load(a, 0, 0);
    Options_SetDisplayMode(a, 0, F<int32_t>(a, 0x10c));
    if (uint8_t* m = App_New(0x11486e)) Engine_Construct(m);
    auto firstRun = [] {
        const int32_t intro = g_6cec70;
        return !F<int32_t>(g_app, 0xa8) && (intro == 0 || intro == 9);
    };
    if (firstRun()) {
        ++F<int32_t>(g_engine, 0x11427a);
        w = 0x280, h = 0x1e0;
    }
    Engine_Init(g_engine, 0, inst, w, h);
    F<int32_t>(g_engine, 0x114216) = F<int32_t>(g_app, 0xa0) == 0;
    g_620e98 = 0x2648;
    if (NameTable_ContainsName(g_pack, 0, (const char*)0x5cecbc, 0, nullptr)) g_620e98 = 0x26ac;
    Hotkeys_Init(1);
    for (uint8_t* r = (uint8_t*)0x6103a0; r < (uint8_t*)0x620b74; r += 0x6e) StrCopyN((char*)r, GetString(F<int32_t>(r, 0x62)), 0x46);
    for (uint8_t* r = (uint8_t*)0x5f3d9c; r < (uint8_t*)0x6103a1; r += 0x81) StrCopyN((char*)r, GetString(F<int32_t>(r, 0x69)), 0x46);
    Options_ApplyGraphics(a, 0, -1, 1);
    Menu_UpdateChecks(hook::As<HMENU*>(g_6ada8c), 0, 0);
    Lighting_Rebuild(a);
    {
        uint8_t* m = App_New(0x118);
        F<uint8_t*>(a, 0x14) = m ? Music_Construct(m) : nullptr;
    }
    if (firstRun()) {
        F<int32_t>(g_app, 0xa8) = 1;
        Options_Save(a, 0, 1);
        --F<int32_t>(g_engine, 0x11427a);
        g_624308 = 1;
        App_SetScreen(a, 0, 3, 0);
    } else {
        Cursor_Set(g_cursorManager, 0, 1, 0, 0);
        App_SetScreen(a, 0, 2, 0);
        if (F<int32_t>(F<uint8_t*>(a, 0x14), 0x100) > 0) F<int32_t>(F<uint8_t*>(a, 0x14), 0x110) = 1;
        Sound_StartFadeTimer(g_sound, 0, 0);
        for (const int32_t until = ClockMs() + 0x258; ClockMs() < until;) {
        }
        Sound_Speakers(g_sound, 0, F<int32_t>(g_engine, 0x11470a), 0);
        for (const int32_t until = ClockMs() + 0x258; ClockMs() < until;) {
        }
    }
    Effects_Reset();
    hook::As<uint32_t>(g_6d4014) = 0x483c30;
    if (uint8_t* e = g_engine) {
        if (e[0x114226]) ((void*(__stdcall*)(HWND))g_5c823c)(Engine_Window(e));
    }
    // Through the call site, which the simcheck harness redirects.
    while (!F<int32_t>(a, 0x10) && hook::CallTarget<int32_t(__fastcall*)(uint8_t*)>(0x48439d)(a)) {
    }
    App_SetScreen(a, 0, 0, 1);
    FreeBuffers6cea54();
    if (uint8_t* music = F<uint8_t*>(a, 0x14)) {
        Obj_Free108(music);
        App_Delete(music);
    }
    F<uint8_t*>(a, 0x14) = nullptr;
    App_CloseSession();
    Net_StopListener();
    Models_Shutdown();
    if (uint8_t* e = g_engine) {
        Engine_Destroy(e);
        App_Delete(e);
    }
    g_engine = nullptr;
    if (void** menu = hook::As<void**>(g_6ada8c)) {
        GdiPair_Release2(menu);
        App_Delete(menu);
    }
    hook::As<void*>(g_6ada8c) = nullptr;
}
RT3_REPLACE(0x4840e0, App_Run);

// 0x484440. The game's WinMain: COM, the clock, system info, the random
// seed and the command line are set up, the window title copied
// ("Railroad Tycoon 3"), the scroll lock saved; the app (0x505 bytes,
// 0x6cec74) and lobby (0x109, 0x6cec7c) are created and run, then freed with
// the session and the memory system, and the scroll lock restored.
int __stdcall Game_WinMain(void* inst, void* prev, char* cmdLine, int show) {
    g_6cec58 = show;
    ((int32_t(__stdcall*)(void*, uint32_t))g_5c8494)(nullptr, 0);
    StartClock();
    SysInfo_Init();
    ResetRand();
    Cmdline_Parse(cmdLine);
    Set_ccba28();
    memcpy((void*)0x624b68, (const void*)0x5ced8c, 0x12);
    hook::As<int32_t>(g_6d4038) = 0x82;
    Keyboard_SaveScrollLock();
    {
        uint8_t* m = App_New(0x505);
        g_app = m ? Game_InitStart(m) : nullptr;
    }
    {
        uint8_t* m = App_New(0x109);
        g_newGame = m ? (uint8_t*)ReturnThis47bbd0(m) : nullptr;
    }
    App_Run(g_app, 0, inst);
    if (uint8_t* app = g_app) App_Delete(app);
    if (uint8_t* lobby = g_newGame) {
        Nop47bbe0(lobby);
        App_Delete(lobby);
    }
    App_CloseSession();
    Mem_Shutdown();
    SysInfo_Nop();
    Keyboard_RestoreScrollLock();
    return 0;
}
RT3_REPLACE(0x484440, Game_WinMain);

// 0x484590. Options o: back to the defaults (+0x70..+0x4fc cleared, the
// default detail level, then the fixed values), followed by the values of
// preset 0 (normal), 1 or 2.
RT3_NOINLINE void __fastcall Options_SetDefaults(uint8_t* o, int, int32_t preset) {
    memset(o + 0x70, 0, 0x123 * 4 + 1);
    Options_ResetDetail(o);
    struct Value {
        uint16_t off;
        uint32_t v;
    };
    static const Value common[] = { { 0x110, 0 }, { 0x134, 1 }, { 0x1e7, 1 }, { 0xf0, 1 },   { 0x100, 2 }, { 0x108, 0 }, { 0x104, 1 }, { 0x10c, 0 },
                                    { 0x1b7, 0 }, { 0x1bb, 3 }, { 0x120, 1 }, { 0x28b, 0 },  { 0x283, 0 }, { 0x168, 0 }, { 0x114, 9 }, { 0x118, 9 },
                                    { 0x11c, 9 } };
    static const Value common2[] = { { 0x13c, 2 }, { 0x140, 2 }, { 0x154, 0 }, { 0x158, 0 }, { 0x15c, 0 }, { 0x160, 0 }, { 0x164, 0 }, { 0x130, 0 },
                                     { 0x1b3, 0 }, { 0x1db, 0 }, { 0x1bf, 0 }, { 0x2c8, 0 }, { 0x94, 1 },  { 0x144, 1 }, { 0x22f, 1 }, { 0x253, 1 },
                                     { 0x2bb, 2 }, { 0x2bf, 1 }, { 0x233, 0 }, { 0x237, 0xc }, { 0x23b, 0 }, { 0x23f, 2 }, { 0x243, 1 }, { 0x247, 0 },
                                     { 0x24b, 0 }, { 0x257, 0 }, { 0x25b, 0 }, { 0x25f, 0 }, { 0x263, 0 }, { 0x267, 0 }, { 0x17c, 0 }, { 0x180, 0 } };
    for (const Value& v : common) F<uint32_t>(o, v.off) = v.v;
    hook::As<uint32_t>(g_6cec6c) = 0;
    for (const Value& v : common2) F<uint32_t>(o, v.off) = v.v;
    o[0x2d0] = 0;
    for (uint32_t k = 0x297; k <= 0x2ab; k += 4) F<uint32_t>(o, k) = 0;
    F<uint32_t>(o, 0x14c) = 0;
    F<uint32_t>(o, 0x30d) = 1;
    for (uint32_t k = 0x18c; k <= 0x198; k += 4) F<uint32_t>(o, k) = 0x01010101;
    F<uint16_t>(o, 0x19c) = 0x0101;
    o[0x19e] = 1;
    static const uint16_t flagBytes[] = { 0x192, 0x193, 0x18f, 0x195, 0x198, 0x199 };
    if (preset == 0) {
        static const Value normal[] = { { 0x178, 2 }, { 0x1eb, 2 }, { 0xf4, 3 }, { 0x24f, 0 }, { 0x150, 0 }, { 0x90, 0 },  { 0xf8, 0 },
                                        { 0x16c, 0 }, { 0xfc, 0 },  { 0x148, 0 }, { 0xa8, 0 },  { 0x170, 0 }, { 0x174, 1 }, { 0x188, 0 } };
        for (const Value& v : normal) F<uint32_t>(o, v.off) = v.v;
        for (uint16_t b : flagBytes) o[b] = 0;
        o[0x194] = 0;
        for (uint32_t k = 0x26b; k <= 0x27b; k += 4) F<uint32_t>(o, k) = 1;
        F<uint32_t>(o, 0x28f) = 0;
        return;
    }
    if (preset == 2) {
        F<uint32_t>(o, 0x90) = 1;
        F<uint32_t>(o, 0x148) = 1;
        F<uint32_t>(o, 0x174) = 0;
        F<uint32_t>(o, 0x178) = 0;
        F<uint32_t>(o, 0x1eb) = 0;
    } else if (preset == 1) {
        F<uint32_t>(o, 0x90) = 0;
        F<uint32_t>(o, 0x148) = 0;
        F<uint32_t>(o, 0x174) = 1;
        F<uint32_t>(o, 0x178) = 1;
        F<uint32_t>(o, 0x1eb) = 2;
    } else
        return;
    F<uint32_t>(o, 0x2c8) = 1;
    F<uint32_t>(o, 0x16c) = 1;
    for (uint32_t k = 0x26b; k <= 0x27b; k += 4) F<uint32_t>(o, k) = 0;
    static const Value tail[] = { { 0xf8, 1 }, { 0x24f, 1 }, { 0x170, 1 }, { 0x150, 1 }, { 0xfc, 1 }, { 0xa8, 1 }, { 0x188, 1 }, { 0x1df, 1 }, { 0x1e3, 0 }, { 0x2b7, 1 } };
    for (const Value& v : tail) F<uint32_t>(o, v.off) = v.v;
    for (uint16_t b : flagBytes) o[b] = 1;
    static const Value tail2[] = { { 0x1c3, 1 }, { 0x1c7, 1 }, { 0x1cb, 0 }, { 0x1cf, 1 }, { 0x1d3, 1 }, { 0x28f, 1 } };
    for (const Value& v : tail2) F<uint32_t>(o, v.off) = v.v;
}
RT3_REPLACE(0x484590, Options_SetDefaults);

// 0x484910. Saves the options (version 0x429, 0x48d bytes from +0x70) and,
// on request, the engine configuration.
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine) {
    if (withEngine && hook::As<void*>(g_engine)) Engine_SaveConfig(g_engine);
    Stream s = StreamOpen((const char*)0x5ceda0, 0, 0x30d40, 1);
    if (s == (Stream)-1) return;
    uint32_t version = 0x429;
    StreamWrite(s, &version, 4, 0);
    StreamWrite(s, o + 0x70, 0x48d, 0);
    StreamClose(s);
}
RT3_REPLACE(0x484910, Options_Save);

// 0x484980. Options o: loads the engine config (when withEngine) and the
// options file 0x5ceda0 (version 0x429, 0x48d bytes at +0x70, detail +0xd4 at
// most 4). A missing file, or one of another version, gets the defaults and is
// saved. QUIRK: the version mismatch saves with the engine config either way;
// the missing file saves it only withEngine. With 0x6cec6c set the file is
// not read.
RT3_NOINLINE void __fastcall Options_Load(uint8_t* o, int, int32_t withEngine) {
    if (withEngine && g_engine) Engine_LoadConfig(g_engine);
    if (FileExists((const char*)0x5ceda0) && !g_6cec6c) {
        int32_t s = StreamOpen((const char*)0x5ceda0, 1, 200000, 1);
        int32_t ver;
        StreamRead(s, &ver, 4, 0, 1);
        if (ver != 0x429) {
            StreamClose(s);
            Options_SetDefaults(o, 0, 0);
            Options_Save(o, 0, 1);
            return;
        }
        StreamRead(s, o + 0x70, 0x48d, 0, 1);
        StreamClose(s);
        int32_t d = F<int32_t>(o, 0xd4);
        F<int32_t>(o, 0xd4) = d > 4 ? 4 : d;
        return;
    }
    Options_SetDefaults(o, 0, 0);
    Options_Save(o, 0, withEngine);
}
RT3_REPLACE(0x484980, Options_Load);

// 0x484a60. Index of the known-adapter entry (44 x {name, extra} at 0x621428,
// 0x18 bytes) matching the display adapter name; -1 if none.
RT3_NOINLINE int32_t __fastcall Adapter_KnownIndex() {
    char name[0x6c] = {};
    ((char*(__cdecl*)(char*, const char*, size_t))0x5a18a0)(name, SysInfo_DisplayName(), 100);
    int32_t i = 0;
    for (const char* const* e = (const char* const*)0x62142c; e < (const char* const*)0x62184c;
         e = (const char* const*)((const uint8_t*)e + 0x18), ++i) {
        if (!StrFind(name, e[-1], 0)) continue;
        const char* extra = e[0];
        if (!*extra || StrFind(name, extra, 0)) return i;
    }
    return -1;
}
RT3_REPLACE(0x484a60, Adapter_KnownIndex);

int32_t __fastcall Obj_ArrayAc(void* o, int, int32_t i) { return F<int32_t>(o, 0xac + i * 4); }
RT3_REPLACE(0x484ae0, Obj_ArrayAc);

RT3_NOINLINE int32_t __fastcall Obj_ArrayAc2(void* o, int, int32_t i) { return F<int32_t>(o, 0xac + i * 4); }
RT3_REPLACE(0x484af0, Obj_ArrayAc2);

// 0x484b00. The view distance for detail level +0xb8 (table 0x6219b8), reduced on
// big maps (more so on smooth ones).
RT3_NOINLINE float __fastcall Map_ViewDistance(uint8_t* o) {
    float v = ((const float*)0x6219b8)[F<int32_t>(o, 0xb8)];
    float area = 262144.0f, rough = 1.0f;
    uint8_t* m = g_map;
    if (m) {
        area = (float)(F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155));
        rough = Grid_Roughness(m);
    }
    rough = (float)(((double)rough - D(0x5c8770)) * D(0x5c8988));
    if (Fl(0x5c8598) > rough) rough = 0.0f;
    rough = (float)(rough > D(0x5c88c8) ? Bits_165(0x3fd3333333333333ull) : (double)rough);
    v = Band(v, area, rough, 0x5cef10, 0x5c88f8, 0x5cef08);
    v = Band(v, area, rough, 0x5cef00, 0x5c8818, 0x5ceef8);
    v = Band(v, area, rough, 0x5ceef0, 0x5c87e8, 0x5ceee8);
    return v;
}
RT3_REPLACE(0x484b00, Map_ViewDistance);

// 0x484d70. Applies graphics option `which` (-2 = all; also any marked
// changed in +0x1c.. while +0x18 is set) of the 17 at +0xac to the map
// renderer 0x62c120: view distance (rebuilding the terrain maps with
// `rebuild` when it moves more than 1%), the two layer distances, the detail
// levels from tables 0x621848.. 0x621960, overview size, sky and water
// options. Without an engine or map the option is only marked changed;
// otherwise all marks are cleared and the listener 0x62be68 told.
RT3_NOINLINE void __fastcall Options_ApplyGraphics(uint8_t* o, int, int32_t which, int32_t rebuild) {
    for (int32_t i = 0; i < 0x11; ++i) {
        if (which != -2 && i != which && (!F<int32_t>(o, 0x18) || !F<int32_t>(o, 0x1c + i * 4))) continue;
        uint8_t* map = g_map;
        if (!g_engine || !map) {
            F<int32_t>(o, 0x18) = 1;
            F<int32_t>(o, 0x1c + i * 4) = 1;
            continue;
        }
        int32_t v = F<int32_t>(o, 0xac + i * 4);
        switch (i) {
        case 3: {
            float d = Map_ViewDistance(g_app);
            double cur = Render_Get14e0(g_map);
            if ((double)d * D(0x5ca288) <= cur) {
                double cur2 = Render_Get14e0(g_map);
                if ((double)d * D(0x5c9b10) >= cur2) break;
            }
            Render_Set14e0(g_map, 0, *(uint32_t*)&d);
            if (rebuild) Terrain_InitMaps(g_map, 0, 1, 0, 0);
            break;
        }
        case 4:
            Render_SetLayerValue(map, 0, 0, (float)((double)((const float*)0x621960)[v] * D(0x5c8578)));
            break;
        case 5:
            Render_SetLayerValue(map, 0, 10, (float)((double)((const float*)0x621960)[v] * D(0x5c8578)));
            break;
        case 6:
            Render_Set153c(map, 0, (uint32_t)(int32_t)((double)((const float*)0x6218a0)[v] * Fl(0x5c85f8)));
            break;
        case 7:
            Render_Set1534(map, 0, (uint32_t)(int32_t)((double)((const float*)0x6218a0)[v] * Fl(0x5ca128)));
            break;
        case 8:
            Render_Set1540(map, 0, (uint32_t)(int32_t)((double)((const float*)0x621848)[v] * Fl(0x5cef14)));
            break;
        case 9:
            F<int32_t>(map, 0x1538) = (int32_t)((double)((const float*)0x6218f8)[v] * Fl(0x5ca128));
            break;
        case 10:
            if (v >= 4) v = 4;
            else if (v == 0) {
                g_engine[0x1146f0] = 0;
                g_engine[0x1146ef] = 0;
                break;
            }
            g_engine[0x1146f0] = 1;
            g_engine[0x1146ef] = 1;
            Render_SetOverviewSize(g_map, 0, (1 << ((v - 1) & 31)) << 7);
            break;
        case 11:
            if (g_models) F<uint32_t>(g_models, 0x28) = ((const uint32_t*)0x62194c)[v];
            break;
        case 12:
            g_engine[0x1146ee] = (uint8_t)v;
            break;
        case 13:
            if (F<int32_t>(o, 0x68)) v = 0;
            if (F<int32_t>(map, 0x21fd) != v) {
                F<int32_t>(map, 0x21fd) = v;
                Terrain_RefreshPointGrids(g_map, 0, 0, 0, 9999, 9999);
            }
            break;
        }
    }
    if (!g_engine || !g_map) return;
    memset(o + 0x1c, 0, 0x11 * 4);
    F<int32_t>(o, 0x18) = 0;
    if (void** l = hook::As<void**>(g_62be68)) ((void(__thiscall*)(void*))(*(void***)l)[0])(l);
}
RT3_REPLACE(0x484d70, Options_ApplyGraphics);

// 0x485060. Options o: sets graphics detail `which` (+0xac + which * 4) to
// `level`. The overall level (0) sets all sixteen settings and the engine's
// texture mode (+0x1146b3: 2 above 6) and resets +0x1146a8 / +0x1146af; the
// groups 2 (settings 3..6) and 1 (7..9, 13) set their members; setting 3
// switches engine +0x1146eb above 8. A group or the overall level no longer
// matching becomes custom (-1). Then the setting is applied and, with an
// engine, the options saved.
RT3_NOINLINE void __fastcall Options_SetDetail(uint8_t* o, int, int32_t which, int32_t level, int32_t rebuild) {
    F<int32_t>(o, 0xac + which * 4) = level;
    if (which == 0) {
        if (level != -1) {
            for (int32_t s = 1; s < 0x11; ++s) Options_SetDetail(o, 0, s, Options_DetailFor(s, level), rebuild);
            if (uint8_t* e = g_engine) {
                F<int32_t>(e, 0x1146b3) = (level > 6) + 1;
                g_engine[0x1146a8] = 0;
                F<int32_t>(g_engine, 0x1146af) = 0;
            }
        }
    } else if (which == 2) {
        if (level != -1)
            for (int32_t s = 3; s <= 6; ++s) Options_SetDetail(o, 0, s, level, rebuild);
    } else if (which == 1) {
        if (level != -1) {
            for (int32_t s = 7; s <= 9; ++s) Options_SetDetail(o, 0, s, level, rebuild);
            Options_SetDetail(o, 0, 0xd, ((const int32_t*)0x621358)[level], rebuild);
        }
    } else if (which == 3) {
        if (uint8_t* e = g_engine) e[0x1146eb] = level > 8;
    }
    if (level != Options_DetailFor(which, F<int32_t>(o, 0xac))) F<int32_t>(o, 0xac) = -1;
    if (which >= 3 && which <= 6 && level != Options_DetailFor(which, F<int32_t>(o, 0xb4))) F<int32_t>(o, 0xb4) = -1;
    if (((which >= 7 && which <= 9) || which == 0xd) && level != Options_DetailFor(which, F<int32_t>(o, 0xb0))) F<int32_t>(o, 0xb0) = -1;
    Options_ApplyGraphics(o, 0, which, rebuild);
    if (g_engine) Options_Save(o, 0, 1);
}
RT3_REPLACE(0x485060, Options_SetDetail);

// 0x4852e0. Options o: back to the default overall detail level 0x6211dc
// (as Options_SetDetail(0, level), but applied once for all settings).
RT3_NOINLINE void __fastcall Options_ResetDetail(uint8_t* o) {
    int32_t level = g_6211dc;
    F<int32_t>(o, 0xac) = level;
    if (level != -1) {
        for (int32_t s = 1; s < 0x11; ++s) Options_SetDetail(o, 0, s, Options_DetailFor(s, level), 1);
        if (uint8_t* e = g_engine) {
            F<int32_t>(e, 0x1146b3) = (level > 6) + 1;
            g_engine[0x1146a8] = 0;
            F<int32_t>(g_engine, 0x1146af) = 0;
        }
    }
    if (level != F<int32_t>(o, 0xac)) F<int32_t>(o, 0xac) = -1;
    Options_ApplyGraphics(o, 0, 0, 1);
    if (g_engine) Options_Save(o, 0, 1);
}
RT3_REPLACE(0x4852e0, Options_ResetDetail);

// 0x4853c0. Options o: picks the detail settings for this machine. The
// speed factor (engine +0x1146f2, scaled by the known adapter's 0x62143c)
// selects one of ten classes (0x6213b0, 12 bytes: threshold, width, height);
// below 0x5c8678 class 2 is forced; outside modes 0 / 9 (0x6cec70) the class
// drops one (0..4). The class's overall level (0x6211d0) is applied as by
// Options_SetDetail(0). In mode 0 also the cursor mode (+0x11473e /
// +0x11473a), the display size of the class and the engine mode +0x114736
// by texture and system memory (0 without hardware support +0x11422f).
RT3_NOINLINE void __fastcall Options_AutoDetail(uint8_t* o) {
    int32_t cls;
    float speed;
    if (F<float>(g_engine, 0x1146f2) < D(0x5c8678)) {
        cls = 2;
    } else {
        int32_t a = Adapter_KnownIndex();
        speed = a == -1 ? 1.0f : ((const float*)(0x62143c + a * 0x18))[0];
        speed = (float)((double)speed * F<float>(g_engine, 0x1146f2));
        a = Adapter_KnownIndex();
        if (a != -1) {
            Engine_SetCursorObject(hook::As<void*>(g_engine), 0, *(const uint32_t*)(0x621430 + a * 0x18));
            if (*(const int32_t*)(0x621438 + a * 0x18)) F<int32_t>(g_engine, 0x114742) = 1;
        }
        cls = 0;
        for (int32_t k = 0; k < 10; ++k) {
            cls = k;
            if (((const float*)0x6213b0)[k * 3] > speed) break;
        }
        int32_t mode = g_6cec70;
        if (mode && mode != 9) {
            cls = cls - 1 <= 0 ? 0 : cls - 1;
            if (cls >= 4) cls = 4;
        }
    }
    int32_t level = ((const int32_t*)0x6211d0)[cls];
    F<int32_t>(o, 0xac) = level;
    if (level != -1) {
        for (int32_t s = 1; s < 0x11; ++s) Options_SetDetail(o, 0, s, Options_DetailFor(s, level), 1);
        if (uint8_t* e = g_engine) {
            F<int32_t>(e, 0x1146b3) = (level > 6) + 1;
            g_engine[0x1146a8] = 0;
            F<int32_t>(g_engine, 0x1146af) = 0;
        }
    }
    if (level != F<int32_t>(o, 0xac)) F<int32_t>(o, 0xac) = -1;
    Options_ApplyGraphics(o, 0, 0, 1);
    if (g_engine) Options_Save(o, 0, 1);
    uint8_t* e = g_engine;
    if (!g_6cec70) {
        F<int32_t>(e, 0x11473e) = 0;
        F<int32_t>(g_engine, 0x11473a) = SysInfo_IsWin9x() == 0;
        int32_t a = Adapter_KnownIndex();
        if (a != -1) F<int32_t>(g_engine, 0x11473a) = *(const int32_t*)(0x621434 + a * 0x18);
        F<int32_t>(g_engine, 0x11468a) = ((const int32_t*)0x6213b4)[cls * 3];
        F<int32_t>(g_engine, 0x11468e) = ((const int32_t*)0x6213b8)[cls * 3];
        e = g_engine;
    }
    if (!e[0x11422f]) {
        F<int32_t>(e, 0x114736) = 0;
        return;
    }
    if (SysInfo_TotalMemory() < 0x9600000) {
        F<int32_t>(g_engine, 0x114736) = 0;
        return;
    }
    Engine_TotalTextureMem(hook::As<void*>(g_engine));
    if (Engine_TotalTextureMem(hook::As<void*>(g_engine)) < 0x3200000 && SysInfo_TotalMemory() < 0xb400000) {
        F<int32_t>(g_engine, 0x114736) = 0;
        return;
    }
    if (g_engine[0x11422d] && Engine_TotalTextureMem(hook::As<void*>(g_engine)) >= 0x5000000 && SysInfo_TotalMemory() > 0xd200000) {
        F<int32_t>(g_engine, 0x114736) = 2;
        return;
    }
    F<int32_t>(g_engine, 0x114736) = 1;
}
RT3_REPLACE(0x4853c0, Options_AutoDetail);

// 0x4856b0. Shifts the driven train's gear (+0x430) by `delta`, clamped to -1..4,
// and logs "Now in gear N" (string 0x2f0, log kind 0xb).
RT3_NOINLINE void __fastcall Train_ShiftGear(uint8_t* o, int, int32_t delta) {
    int32_t gear = F<int32_t>(o, 0x430) + delta;
    if (gear > 4) gear = 4;
    else if (gear < -1) gear = -1;
    F<int32_t>(o, 0x430) = gear;
    char num[0x20], msg[0x190];
    Num_Format(num, 1, (double)gear, 10, 1, 0, 0);
    FormatText(msg, GetString(0x2f0), num, 0, 0, 0, 0, 0, 0, 0);
    Log_Add(g_messageLog, 0, msg, 0xb, 0, 0);
}
RT3_REPLACE(0x4856b0, Train_ShiftGear);

RT3_NOINLINE int32_t Global6cfc8cSet() { return hook::As<void*>(g_6cfc8c) ? 1 : 0; }
RT3_REPLACE(0x485750, Global6cfc8cSet);

// 0x485760. Constructs tank t (vtable 0x5cf048) of model `name` in scene
// `scene` at (x, y, z): body (texture "<name>1"), the turret / mantlet /
// cannon / muzzle-flash chain with the mantlet light, both tracks (tinted),
// drive and idler wheels, up to 0x30 road wheels per side (+0x24b / +0x30c
// with their rest positions), exhaust and muzzle smoke emitters +0x216 /
// +0x21a and five sounds +0x3cc..+0x3dc; +0x3e8 is the creation time.
RT3_NOINLINE uint8_t* __fastcall Tank_Construct(uint8_t* t, int, const char* name, uint8_t* scene, float x, float y, float z) {
    char buf[0x50], tex[0x40];
    char* path = buf;
    char* num = buf + 0x40;
    SceneObj_ConstructDefault(t);
    *(uint32_t*)t = 0x5cf048;
    F<uint8_t*>(t, 0x206) = scene;
    F<uint32_t>(t, 0x3ec) = 0;
    F<uint32_t>(t, 0x3f0) = 0;
    F<uint32_t>(t, 0x424) = 0;
    F<uint32_t>(t, 0x428) = 0;
    F<uint32_t>(t, 0x42c) = 0;
    F<uint32_t>(t, 0x418) = 0;
    F<uint32_t>(t, 0x41c) = 0;
    F<uint32_t>(t, 0x420) = 0;
    F<uint32_t>(t, 0x3f4) = 0;
    F<uint32_t>(t, 0x3f8) = 0;
    F<uint32_t>(t, 0x3e0) = 0x43fa0000;
    F<uint32_t>(t, 0x3e4) = 0;
    F<uint32_t>(t, 0x430) = 1;
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(tex, (const char*)0x5cf088, name);
    FormatText(path, (const char*)0x5cadc8, name, 0, 0, 0, 0, 0, 0, 0);
    SceneObj_Init(t, 0, (uint32_t)(uintptr_t)scene, path, tex);
    FormatText(path, (const char*)0x5cb3bc, name, 0, 0, 0, 0, 0, 0, 0);
    F<uint8_t*>(t, 0x20a) = t;
    F<uint8_t*>(t, 0x20e) = NewSceneObj(path, tex);
    FormatText(path, (const char*)0x5cb3ac, name, 0, 0, 0, 0, 0, 0, 0);
    F<uint8_t*>(t, 0x212) = NewSceneObj(path, tex);
    FormatText(path, (const char*)0x5cb39c, name, 0, 0, 0, 0, 0, 0, 0);
    F<uint8_t*>(t, 0x21e) = NewSceneObj(path, tex);
    FormatText(path, (const char*)0x5cb364, name, 0, 0, 0, 0, 0, 0, 0);
    uint8_t* flash = NewSceneObj(path, name);
    F<uint8_t*>(t, 0x22e) = flash;
    SceneObj_AddChild(F<uint8_t*>(t, 0x21e), 0, flash);
    FormatText(path, (const char*)0x5cf02c, name, 0, 0, 0, 0, 0, 0, 0);
    SceneObj_AddAttachment(F<uint8_t*>(t, 0x212), 0, NewLight(path, F<uint8_t*>(t, 0x212)));
    SceneObj_AddChild(F<uint8_t*>(t, 0x212), 0, F<uint8_t*>(t, 0x21e));
    SceneObj_AddChild(F<uint8_t*>(t, 0x20e), 0, F<uint8_t*>(t, 0x212));
    SceneObj_AddChild(t, 0, F<uint8_t*>(t, 0x20e));
    SceneObj_GetPos2(F<uint8_t*>(t, 0x21e), 0, (uint32_t*)(t + 0x222), (uint32_t*)(t + 0x226), (uint32_t*)(t + 0x22a));
    F<uint8_t*>(t, 0x232) = nullptr;
    FormatText(path, (const char*)0x5cf01c, name, 0, 0, 0, 0, 0, 0, 0);
    F<uint8_t*>(t, 0x232) = NewSceneObj(path, name);
    Model_PrepareTint(F<uint8_t*>(t, 0x232), 0, 0x3f000000);
    FormatText(path, (const char*)0x5cf00c, name, 0, 0, 0, 0, 0, 0, 0);
    F<uint8_t*>(t, 0x236) = NewSceneObj(path, name);
    Model_PrepareTint(F<uint8_t*>(t, 0x236), 0, 0x3f000000);
    static const uint32_t tracks[2] = { 0x232, 0x236 };
    for (uint32_t track : tracks) {
        Model_AddTintRect(F<uint8_t*>(t, track), 0, Bits(0xc1f00000), Bits(0x410ccccd), Bits(0x41f00000), Bits(0x4110cccd), Bits(0x4135999a));
        Model_AddTintRect(F<uint8_t*>(t, track), 0, Bits(0xc1f00000), Bits(0x41133333), Bits(0x41f00000), Bits(0x41173333), Bits(0x4141999a));
    }
    SceneObj_AddChild(t, 0, F<uint8_t*>(t, 0x232));
    SceneObj_AddChild(t, 0, F<uint8_t*>(t, 0x236));
    static const uint32_t wheels[4][2] = { { 0x5ceff8, 0x23a }, { 0x5cefe4, 0x23e }, { 0x5cefd0, 0x242 }, { 0x5cefbc, 0x246 } };
    for (const auto& w : wheels) {
        FormatText(path, (const char*)w[0], name, 0, 0, 0, 0, 0, 0, 0);
        uint8_t* o = NewSceneObj(path, name);
        F<uint8_t*>(t, w[1]) = o;
        SceneObj_AddChild(t, 0, o);
    }
    // Road wheels: count +0x24a / +0x30b, objects +0x24b / +0x30c, rest positions +0x27b / +0x33c.
    static const uint32_t sides[2][4] = { { 0x5cefa8, 0x24a, 0x24b, 0x27b }, { 0x5cef94, 0x30b, 0x30c, 0x33c } };
    for (const auto& sd : sides) {
        t[sd[1]] = 0;
        for (int32_t i = 1; i < 0xff; ++i) {
            FormatText(path, (const char*)sd[0], name, Num_Format(num, 1, (double)i, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
            if (!PackHas(path)) break;
            F<uint8_t*>(t, sd[2] + t[sd[1]] * 4) = NewSceneObj(path, name);
            SceneObj_AddChild(t, 0, F<uint8_t*>(t, sd[2] + t[sd[1]] * 4));
            uint8_t* rest = t + sd[3] + t[sd[1]] * 0xc;
            SceneObj_GetPos2(F<uint8_t*>(t, sd[2] + t[sd[1]] * 4), 0, (uint32_t*)rest, (uint32_t*)(rest + 4), (uint32_t*)(rest + 8));
            ++t[sd[1]];
        }
    }
    FormatText(path, (const char*)0x5cef80, name, 0, 0, 0, 0, 0, 0, 0);
    if (PackHas(path)) {
        uint32_t r[4] = { 0, 0, 0x3f000000, 0x3f000000 };
        uint8_t* m = (uint8_t*)ExeNew(0x1fd);
        uint8_t* e = m ? Emitter_Construct(m, 0, path, 0xff1f1f1f, 0x40, (const float*)r, F<uint8_t*>(t, 0x20a), 0x120) : nullptr;
        F<uint8_t*>(t, 0x216) = e;
        F<uint32_t>(e, 0x70) = 0;
        F<uint32_t>(e, 0x74) = 0x3ac49ba6;
        F<uint32_t>(e, 0x78) = 0;
        F<uint32_t>(e, 0x80) = 0x3a03126f;
        F<uint32_t>(e, 0x7c) = 0x3a83126f;
        F<uint32_t>(e, 0x84) = 0x3a83126f;
        e = F<uint8_t*>(t, 0x216);
        F<uint32_t>(e, 0x88) = 0x3c23d70a;
        F<uint32_t>(e, 0x8c) = 0x3c23d70a;
        F<uint32_t>(e, 0x90) = 0x3c23d70a;
        View_SetParams(F<uint8_t*>(t, 0x216), 0, 2000, 0, 0x3f800000, 0x3e19999a, 0x41000000, 0x3f99999a);
        Timer_SetBudget(F<uint8_t*>(t, 0x216), 0, 0x96);
        Camera_AddListener(t, 0, F<uint8_t*>(t, 0x216));
    }
    FormatText(path, (const char*)0x5cef6c, name, 0, 0, 0, 0, 0, 0, 0);
    if (PackHas(path)) {
        uint32_t r[4] = { 0, 0x3f000000, 0x3f000000, 0x3f800000 };
        uint8_t* m = (uint8_t*)ExeNew(0x1fd);
        uint8_t* e = m ? Emitter_Construct(m, 0, path, 0xff7f7f7f, 0x40, (const float*)r, F<uint8_t*>(t, 0x21e), 0x20) : nullptr;
        F<uint8_t*>(t, 0x21a) = e;
        F<uint32_t>(e, 0x70) = 0;
        F<uint32_t>(e, 0x78) = 0;
        F<uint32_t>(e, 0x74) = 0x3b03126f;
        F<uint32_t>(e, 0x7c) = 0x3b03126f;
        F<uint32_t>(e, 0x80) = 0x3b03126f;
        F<uint32_t>(e, 0x84) = 0x3b03126f;
        e = F<uint8_t*>(t, 0x21a);
        F<uint32_t>(e, 0x88) = 0x3c23d70a;
        F<uint32_t>(e, 0x8c) = 0x3c23d70a;
        F<uint32_t>(e, 0x90) = 0x3c23d70a;
        View_SetParams(F<uint8_t*>(t, 0x21a), 0, 3000, 0, 0x3f800000, 0x3e19999a, 0x41800000, 0x4019999a);
        Timer_SetBudget(F<uint8_t*>(t, 0x21a), 0, 0x32);
        Camera_AddListener(F<uint8_t*>(t, 0x21e), 0, F<uint8_t*>(t, 0x21a));
        F<uint8_t*>(t, 0x21a)[0x1d8] = 1;
    }
    Obj_MoveTo(t, 0, x, y, z);
    static const int32_t sounds[5][3] = { { 0x5cef5c, -1, 0 }, { 0x5cef4c, -1, 0 }, { 0x5cef38, 0, 0 }, { 0x5cef28, -1, 1 }, { 0x5cef18, -1, 1 } };
    for (int k = 0; k < 5; ++k) {
        uint8_t* m = (uint8_t*)ExeNew(0x141);
        F<uint8_t*>(t, 0x3cc + k * 4) =
            m ? Channel_Setup(m, 0, (const char*)sounds[k][0], scene, x, y, z, sounds[k][1], sounds[k][2], Bits(0xbf800000)) : nullptr;
    }
    for (int k = 0; k < 5; ++k) Obj_AttachChannel(t, 0, F<uint8_t*>(t, 0x3cc + k * 4));
    F<float>(t, 0x400) = x;
    F<float>(t, 0x404) = y;
    F<float>(t, 0x408) = z;
    F<uint32_t>(t, 0x3e8) = ((uint32_t(__stdcall*)())g_5c8068)();
    return t;
}
RT3_REPLACE(0x485760, Tank_Construct);

}  // namespace rt3
