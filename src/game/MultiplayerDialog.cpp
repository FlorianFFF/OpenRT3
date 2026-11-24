// MultiplayerDialog.cpp - RT3.exe 0x4ea000-0x4f2000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <initializer_list>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "hook/keepregs.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t Net_LockQueue();
void Net_UnlockQueue();
void __fastcall Widget_SetVtbl(uint8_t* w);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
void __fastcall Obj_FreeBuffer8(void* o);
void __fastcall NameDialog_SetName(uint8_t* d, int, const char* name);
void __fastcall Music_Close(uint8_t* m);
void __fastcall Thread_Destruct(uint8_t* t);
int32_t __fastcall Game_EffectiveVersion();
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall String_Assign(char** s, const char* src);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Table_CountField4(Collection* t);
int32_t __fastcall NewGameConfig_ClaimAiSlot(int32_t mode);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Obj_Get25(void* o);
void GameSpy_Pump();
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Thread_Start(uint8_t* t);
void __fastcall Obj_Incc60(uint8_t* o);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
void* __fastcall Obj_City41(void* o);
uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Font_GetLine(uint8_t* f, int, const char* text, float width, int32_t line, char* out);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Company_HasAssets(void* c);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
int32_t __fastcall Company_LiveIndex(void* c);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void MapSizeDialog_Show();
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
double __stdcall SpeedFactor(float v);
float __fastcall Cargo_DeliveryPayment(uint8_t* o, int32_t year, float v, uint32_t company, uint32_t region);
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
int32_t __fastcall Collection_Count(Collection* c);
void __fastcall Session_Shutdown(uint8_t* s);
uint8_t* __fastcall Session_Init(uint8_t* s);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial);
void __fastcall Obj_ApplySetup(uint8_t* o, int, uint8_t* src);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Widget_SetPicture(uint8_t* g, int, const void* pixels, int32_t w, int32_t h);
void __fastcall NewGameDialog_ShowSetting77(uint8_t* w);
void __fastcall Window_SetMode1(uint8_t* w, int, int32_t mode);
int32_t Global6d40e4();
uint8_t* __stdcall Textures_Find(int32_t key);
void __stdcall Net_UnlockQueueArg(int32_t);
char* __stdcall Net_LocalAddress(char* out);
void __fastcall Lobby_AddChatText(const char* text);
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
int32_t ClockMs();
int32_t __fastcall EditBox_ActivateClick(uint8_t* w, int, const uint32_t* ev);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __stdcall Game_PostInputEvent(uint32_t type, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
int32_t __fastcall Table_CountField3f(Collection* t);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
int32_t Global6d07b4Set2();
int32_t IsMultiplayer();
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
int32_t __fastcall Str_IsBlank(const char* s);
uint8_t* __fastcall Lobby_FreshList(uint8_t* lobby);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
int32_t __fastcall Company_FreeShares(void* c);
void __fastcall Obj_Decc60(uint8_t* o);
uint32_t __fastcall Company_Logo(uint8_t* c);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

uint32_t IsEnglish();
int32_t __stdcall Net_AllHaveVersion(float v);
void* __fastcall Obj_Company21(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void __fastcall SetSeed(int32_t seed);
void __fastcall MpDialog_SelectMode(uint8_t* dialog, int, int32_t choice);
void __fastcall MpScenario_SelectSource(uint8_t* dialog, int, int32_t choice);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
void __fastcall CompanyReport_Draw(uint8_t* w, int, float x, float y, float wd, float h, int32_t mode);
void __fastcall CompanyOverview_Draw(uint8_t* p, int, float x, float y, float w, float h);
void __fastcall CompanyRanking_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
void __fastcall PlayerRanking_Draw(uint8_t* p, int, float x, float y, float w, float, int32_t sortKey);
void __fastcall CompanyWindow_DrawStockPage(uint8_t* w, int, float x, float y, float wd, float h);
void __fastcall PlayerDetail_Draw(uint8_t* w, int, float x, float y, float, float);
void __fastcall CompanyTrains_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
void __fastcall CompanyStations_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
void __fastcall CargoRanking_Draw(uint8_t* p, int, float x, float y, float w, float, int32_t sortKey);
void __fastcall CompanyIndustries_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
float __fastcall Company_BookValue(uint8_t* c);
uint8_t* __fastcall Net_QueueText(uint8_t* s, int, const char* text, int32_t a, int32_t b, int32_t c, int32_t d);
float __fastcall Obj_Sum12a7Scaled(void* o);
void* Engine_FirstListEntry();
uint32_t __fastcall FileDialog_Run(char* name, int32_t kind);
void __fastcall StripExtension(char* s);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE void* Engine_FirstListEntry();
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall CompanyWindow_Draw(uint8_t* p, int, float x, float y, float w, float h);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
void __fastcall Company_Absorb(uint8_t* c, int, uint32_t id, int32_t price);
float __fastcall Game_VersionNumber();
void Merger_ResolveVote();
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall MpDialog_Refresh(int32_t reselect);
float __fastcall Obj_Sum12a7(void* o);
int32_t __fastcall Session_Open(uint8_t* s, int, int32_t gamespy, const char* nick);
int32_t __stdcall Peer_JoinNamed(const char* name);
void NewGameConfig_ClearSlots();
void __fastcall Lobby_HostGame(uint8_t* lobby, int, const char* game, const char* name, const char* password, int32_t ms);
int32_t __stdcall Peer_Think(int32_t);
void __fastcall NewGame_RefreshAiSlots(uint8_t* o);
void __fastcall Lobby_JoinGame(uint8_t* lobby, int, const char* host, const char* name, const char* password);
uint32_t __fastcall MapSize_Prompt(uint32_t* width, uint32_t* height);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall Session_Restart(uint8_t* w, int, int32_t host);
void __fastcall NameDialog_ShowName(uint8_t* d, int, const char* name);
const char* Net_SocketErrorText();
const char* __fastcall Net_ErrorText(int32_t code);
int32_t MpSession_ReportEvent();
void __fastcall MpScenarioPanel_Update(uint8_t* d, int, const char* path, const char* name, int32_t force);
void __fastcall Lobby_RefreshPlayers(uint8_t* w);
void __fastcall GameList_Fill(uint8_t* w);
void __fastcall NewGameDialog_ShowSetting7b(uint8_t* w);
void __fastcall MpDialog_Join(uint8_t* w, int, const char* address);
void __fastcall MpDialog_Connect(uint8_t* w);
void Net_SendReady();
void __fastcall NewGameDialog_ShowDifficulty(uint8_t* w);
uint8_t Net_AllPlayersFlag1();
void Net_OfferResync();
template <typename T> static inline T& F_dialog_flow(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
template <typename T> static inline T& F_dialog_selection(void* p, uint32_t off) { return *(T*)((uint8_t*)p + off); }
const char* __fastcall Obj_NextBufferedLine(void* o);
uint32_t __fastcall TreeView_NodeValue(uint8_t* w, int, int32_t i);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Company_Absorb(uint8_t* c, int, uint32_t id, int32_t price);
RT3_NOINLINE int32_t __fastcall Lobby_ListGames(uint8_t* lobby);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE uint8_t* __fastcall Lobby_FreshList(uint8_t* lobby);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE uint32_t __fastcall FileDialog_Run(char* name, int32_t kind);
RT3_NOINLINE void __fastcall EventPanel_Draw(uint8_t* p, int, float x, float y, float w, float h);
RT3_NOINLINE void __fastcall CompanyWindow_Draw(uint8_t* p, int, float x, float y, float w, float h);
RT3_NOINLINE uint8_t* __fastcall LoadScreen_Construct(uint8_t* w);
int32_t __fastcall Return1_4ea700(void*, int, int32_t);
RT3_NOINLINE void __fastcall Obj_Set78(uint8_t* o, int, int32_t v);
int32_t Global6d10b0Set();
void __fastcall TitleScreen_Destruct(uint8_t* w);
RT3_NOINLINE void MapSizeDialog_Show();
void MapSizeDialog_ReadSliders();
RT3_NOINLINE void __fastcall MapSizePanel_Build(uint8_t* w, int, float x, float y, float width, float);
RT3_NOINLINE uint32_t __fastcall MapSize_Prompt(uint32_t* width, uint32_t* height);
RT3_NOINLINE void __fastcall Money_PromptTransfer(int32_t* a, int32_t* b);
int32_t __fastcall Value_IsSet(void*, const int32_t* v);
RT3_NOINLINE void __fastcall Obj_CopyTo6d10d8(const uint8_t* o);
void __fastcall Company_PriceShares(void*, int32_t d);
void __fastcall ShareDialog_SelectCompany(uint32_t id);
void __fastcall ShareDialog_SelectCompanyCb(void*, uint32_t id);
void __fastcall CompanyList_DrawLogoCb(void*, uint32_t id, float x, float y, float w, float h, int32_t);
void __fastcall CompanyOverview_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
void __fastcall MergerWindow_DrawVotes(void*, int, float x, float y, float w, float, int32_t);
RT3_NOINLINE void Merger_ResolveVote();
void __fastcall CompanyPicker_Build(uint8_t* win, int, float x, float y, float, float);
RT3_NOINLINE void Merger_OfferDialog();
RT3_NOINLINE void NewGameConfig_ClearSlots();
RT3_NOINLINE int32_t __fastcall NewGameConfig_ClaimAiSlot(int32_t mode);
RT3_NOINLINE int32_t __fastcall Lobby_CountFreeEntries(uint8_t* l);
RT3_NOINLINE void __fastcall Lobby_SetMode(int32_t kind, int, int32_t, int32_t);
RT3_NOINLINE void __fastcall NameDialog_SetName(uint8_t* d, int, const char* name);
RT3_NOINLINE void __fastcall Setup_AddPortraitPicker(uint8_t* win, int, float x, float y, int32_t, int32_t);
RT3_NOINLINE int32_t __fastcall Count40_StepCb(uint8_t* w, const uint32_t* ev);
void __fastcall Dialog_AddTwoEntryFields(uint8_t* win, int, float x, float y, float w, int32_t);
int32_t __fastcall NameFields_CopyCb(uint8_t* w, const uint32_t* ev);
void __fastcall Dialog_ClearChildrenE61(uint8_t* w, int, int32_t);
RT3_NOINLINE void __fastcall NameDialog_ShowName(uint8_t* d, int, const char* name);
RT3_NOINLINE int32_t __fastcall GameList_Refresh(uint8_t* w);
RT3_NOINLINE void __fastcall NewGameDialog_ShowSetting77(uint8_t* w);
RT3_NOINLINE void __fastcall NewGameDialog_ShowSetting7b(uint8_t* w);
RT3_NOINLINE void __fastcall NewGameDialog_ShowDifficulty(uint8_t* w);
uint8_t* __fastcall WorkerA_Delete(uint8_t* t, int, uint32_t flags);
RT3_NOINLINE uint8_t* __fastcall Session_Init(uint8_t* s);
uint8_t* __fastcall WorkerB_Delete(uint8_t* t, int, uint32_t flags);
void Session_StartWorker();
RT3_NOINLINE void __fastcall NewGame_RefreshAiSlots(uint8_t* o);
RT3_NOINLINE int32_t __fastcall NewGame_ClaimSlot(int, int32_t mode);
RT3_NOINLINE void NewGame_CapAiSlots();
RT3_NOINLINE void __fastcall Session_Restart(uint8_t* w, int, int32_t host);
RT3_NOINLINE void __fastcall NameDialog_SetNameIfOpen(const char* name);
void __fastcall Lobby_AddPlayerButton(int32_t, int32_t slot, int32_t key, float x, float y, float w, float);
void __fastcall Lobby_DrawPlayerLabel(int32_t id, int32_t key, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall MpDialog_Connect(uint8_t* w);
RT3_NOINLINE void __fastcall MpDialog_Join(uint8_t* w, int, const char* address);
void __fastcall OptionsScreen_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall Window_SetMode1(uint8_t* w, int, int32_t mode);
RT3_NOINLINE void Net_SendReady();
RT3_NOINLINE uint8_t Net_AllPlayersFlag1();
RT3_NOINLINE void __fastcall Lobby_RefreshPlayers(uint8_t* w);
RT3_NOINLINE void __fastcall GameList_Fill(uint8_t* w);
RT3_NOINLINE void __fastcall Lobby_AddChatText(const char* text);
RT3_NOINLINE void __fastcall MpScenarioPanel_Update(uint8_t* d, int, const char* path, const char* name, int32_t force);
void __fastcall MpScenarioList_Fill(uint8_t* d);
RT3_NOINLINE void __fastcall MpScenario_SelectSource(uint8_t* dialog, int, int32_t choice);
RT3_NOINLINE void __fastcall MpDialog_SelectMode(uint8_t* dialog, int, int32_t choice);
RT3_NOINLINE int32_t MpSession_ReportEvent();
uint32_t __fastcall SessionWorker_Run(uint8_t* thr);
uint8_t* __fastcall MpDialog_Construct(uint8_t* d);
int32_t __fastcall MpDialog_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE void __fastcall MpDialog_Refresh(int32_t reselect);
int32_t __fastcall Picker_ChooseCb(uint8_t* w, const uint32_t* ev);
void __fastcall LocoDetail_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t);
void __fastcall LocoWindow_Build(uint8_t* w, int, float x, float y, float wd, float);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void* Engine_FirstListEntry();
RT3_NOINLINE uint8_t* __stdcall Textures_Find(int32_t key);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
void __fastcall Font_GetLine(uint8_t* f, int, const char* text, float width, int32_t line, char* out);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall EditBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);

// The network player list (head from Net_LockQueue): nodes {next, ?, name +8, flags +0x5c}.
static inline uint8_t* PlayerHead() { return *(uint8_t**)Net_LockQueue(); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline uint8_t* Options() { return g_app; }

static inline size_t Len(const char* s) { size_t n = 0; while (s[n]) ++n; return n; }

static inline void Copy(char* d, const char* s) { while ((*d++ = *s++) != 0) {} }

// Replaces the text (+0xba) of child `id` with a heap copy of s (only if it had one).
static inline void SetChildText(uint8_t* dlg, int32_t id, const char* s) {
    uint8_t* c = Widget_FindChild(dlg, 0, id);
    if (!F<void*>(c, 0xba)) return;
    Free(F<void*>(c, 0xba));
    char* p = (char*)Malloc(Len(s) + 2);
    F<char*>(c, 0xba) = p;
    Copy(p, s);
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

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

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline SprintfFn ExeSprintf() { return (SprintfFn)0x5a19c4; }

// 0x4ed890 (mode in EDI). Multiplayer: shows the wait cursor, opens a modal
// count and starts the session worker thread 0x6d1294 (vtable 0x5d1250, the old
// one deleted) for `mode` (0x6d127c); modes 1 and 4 also clear 0x6cd8dc.
static void __cdecl Session_StartWorker_Impl(int32_t mode) {
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    Obj_Incc60(g_windowManager);
    g_6d1274 = 1;
    g_6d1278 = 0;
    g_6d127c = mode;
    g_6d1280 = 0;
    g_6d1288 = 0;
    g_6d128c = 0;
    if (mode == 1 || mode == 4) g_6cd8dc = 0;
    if (uint8_t* t = g_6d1294) {
        (*(void(__fastcall**)(uint8_t*, int, int32_t))F<void**>(t, 0))(t, 0, 1);
        g_6d1294 = nullptr;
    }
    uint32_t* t = (uint32_t*)ExeNew(0x10);
    if (t) {
        t[1] = 0;
        t[2] = 0;
        t[3] = 0;
        t[0] = 0x5d1250;
    }
    hook::As<uint32_t*>(g_6d1294) = t;
    Thread_Start((uint8_t*)t);
}

static inline float Prod_Bits(uint32_t b) { return *(const float*)&b; }

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

// Session_StartWorker (0x4ed890) takes its mode in EDI.
static void Session_StartWorkerMode(int32_t mode) {
    __asm {
        push edi
        mov edi, mode
        mov eax, 0x4ed890
        call eax
        pop edi
    }
}

static void Event(uint8_t* window, uint32_t message, uint32_t id, uint32_t value) {
    hook::Orig<void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)>(0x540120)(
        window, 0, message, id, value, 0, 0);
}

static void Event_dialog_selection(uint8_t* dialog, uint32_t message, uint32_t id, uint32_t data) {
    hook::Orig<void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)>(0x540120)(
        dialog, 0, message, id, data, 0, 0);
}

// 0x4ea060 (draw callback; x, y, w, h). Company window p draws page +0x78:
// its title (table 0x6220a4, 13 bytes a page), the two corner ornaments
// (+0xb31 entries 0xc / 0xd), for pages with a company (0x6220ac) its logo
// and name link (0x4b2, widget 0x3f13), a rule, and the page body below:
// overview, rankings (sorted by +0x75c), the three reports, stock, player
// pages, the event panel, trains, stations, cargo and industries; pages 0,
// 11 and 13 have none, unknown ones say 0x4b3.
RT3_NOINLINE void __fastcall CompanyWindow_Draw(uint8_t* p, int, float x, float y, float w, float h) {
    const int32_t page = F<int32_t>(p, 0x78);
    Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, GetString(F<int32_t>((void*)0x6220a4, page * 0xd)), x, y, w, 0.05f, 0xff000000, 0, 1, 2, 1.0f);
    uint8_t* glyphs = F<uint8_t*>(p, 0xb31);
    float top = (float)((double)y - *(const double*)0x5d1178);
    double left = (double)x + *(const double*)0x5c8590;
    float gx = (float)left;
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(glyphs, 0, 0xc), 0, gx, top, 0x3f800000, 1.0f, 1.0f, 0x2fffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    double right = (double)x + w - *(const double*)0x5c8590;
    float rx = (float)(right - *(const double*)0x5d1170);
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(p, 0xb31), 0, 0xd), 0, rx, top, 0x3f800000, 1.0f, 1.0f, 0x2fffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    if (((const uint8_t*)0x6220ac)[page * 0xd] && Collection_IsValid(g_companies, 0, F<uint32_t>(p, 0x88))) {
        uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(p, 0x88));
        float lx = (float)(left - *(const double*)0x5d1170 - *(const double*)0x5c86b0);
        Glyph_Queue((uint8_t*)(uintptr_t)Company_Logo(co), 0, lx, top, 0x3f800000, 0.5f, 0.5f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
        char text[0x100];
        const char* fmt = GetString(0x4b2);
        FormatText(text, fmt, (const char*)co + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        Window_SendEvent5(p, 0, 0x6e, 0x3f13, (uint32_t)(uintptr_t)text, 0, 0);
        Window_SendEvent5(p, 0, 0x7c, 0x3f13, F<uint32_t>(p, 0x88), 0, 0);
    }
    float rule = (float)((double)y + *(const double*)0x5d10f8);
    Ui_Line(0x1f000000, 0, gx, rule, (float)right, rule, 2.0f);
    float body = (float)((double)rule + *(const double*)0x5c9bc0);
    float bh = (float)((double)h - ((double)body - y));
    int32_t sortKey = F<int32_t>(p, 0x75c);
    switch (page) {
    case 0: case 0xb: case 0xd: return;
    case 1: CompanyOverview_Draw(p, 0, x, body, w, bh); return;
    case 2: CompanyRanking_Draw(p, 0, x, body, w, bh, sortKey); return;
    case 3: CompanyReport_Draw(p, 0, x, body, w, bh, 0); return;
    case 4: CompanyReport_Draw(p, 0, x, body, w, bh, 1); return;
    case 5: CompanyReport_Draw(p, 0, x, body, w, bh, 2); return;
    case 6: CompanyWindow_DrawStockPage(p, 0, x, body, w, bh); return;
    case 7: PlayerRanking_Draw(p, 0, x, body, w, bh, sortKey); return;
    case 8: PlayerDetail_Draw(p, 0, x, body, w, bh); return;
    case 9: EventPanel_Draw(p, 0, x, body, w, bh); return;
    case 10: CompanyTrains_Draw(p, 0, x, body, w, bh, sortKey); return;
    case 0xc: CompanyStations_Draw(p, 0, x, body, w, bh, sortKey); return;
    case 0xe: CargoRanking_Draw(p, 0, x, body, w, bh, sortKey); return;
    case 0xf: CompanyIndustries_Draw(p, 0, x, body, w, bh, sortKey); return;
    }
    Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, GetString(0x4b3), x, body, w, bh, 0xff000000, 0, 1, 2, 1.0f);
}
RT3_REPLACE(0x4ea060, CompanyWindow_Draw);

// 0x4ea620. Load screen constructor ("LoadScreen.win", vtable 0x5d1194, kept in
// 0x6d10b0): picks a picture 1..5 other than last time's (options +0x2c7, saved
// right away) and shows "LoadScreen%d.imb" in images 0x84d3..0x84d5.
RT3_NOINLINE uint8_t* __fastcall LoadScreen_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d1194;
    g_6d10b0 = w;
    F<int32_t>(w, 0x78) = 0;
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d11a0);
    int32_t pic;
    do {
        pic = Rand(1, 5);
    } while (pic == g_app[0x2c7]);
    g_app[0x2c7] = (uint8_t)pic;
    // Through the original address: the tests stub the options file write.
    ((void(__fastcall*)(uint8_t*, int, int32_t))0x484910)(g_app, 0, 1);
    char name[0x80];
    ExeSprintf()(name, (const char*)0x5d1180, pic);
    for (int32_t i = 0; i < 3; ++i) Window_SendEvent5(w, 0, 0x75, 0x84d3 + i, (uint32_t)(uintptr_t)name, 0, 0);
    return w;
}
RT3_REPLACE(0x4ea620, LoadScreen_Construct);

// 0x4ea700 (vtable stub).
int32_t __fastcall Return1_4ea700(void*, int, int32_t) { return 0x1; }
RT3_REPLACE(0x4ea700, Return1_4ea700);

RT3_NOINLINE void __fastcall Obj_Set78(uint8_t* o, int, int32_t v) { F<int32_t>(o, 0x78) = v; }
RT3_REPLACE(0x4ea710, Obj_Set78);

int32_t Global6d10b0Set() { return hook::As<void*>(g_6d10b0) ? 1 : 0; }
RT3_REPLACE(0x4ea720, Global6d10b0Set);

// 0x4ea730. Title screen destructor: closes the menu music (0x6cfd34).
void __fastcall TitleScreen_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1194;
    hook::As<int32_t>(g_6d10b0) = 0;
    if (uint8_t* m = g_6cfd34) {
        Music_Close(m);
        ExeDelete(m);
    }
    hook::As<void*>(g_6cfd34) = nullptr;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4ea730, TitleScreen_Destruct);

// 0x4ea780. The map-size dialog (0x6d10bc, if open): width 0x6d10b4 and height
// 0x6d10b8 in fields 0xc032 / 0xc034 and "W*H of 409,600" style text (string 0xf3)
// in 0xc035.
RT3_NOINLINE void MapSizeDialog_Show() {
    if (!g_6d10bc) return;
    char buf[0x110], area[0x130], limit[0x130], text[0x320];
    Num_Format(buf, 8, (double)g_6d10b4, 10, 1, 0, 0);
    Window_SendEvent5(g_6d10bc, 0, 0x6f, 0xc032, (uint32_t)(uintptr_t)buf, 0, 0);
    Num_Format(buf, 8, (double)g_6d10b8, 10, 1, 0, 0);
    Window_SendEvent5(g_6d10bc, 0, 0x6f, 0xc034, (uint32_t)(uintptr_t)buf, 0, 0);
    char* l = Num_Format(limit, 1, 409600.0, 10, 1, 0, 0);
    char* a = Num_Format(area, 1, (double)(g_6d10b8 * g_6d10b4), 10, 1, 0, 0);
    FormatText(text, GetString(0xf3), a, l, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(g_6d10bc, 0, 0x6f, 0xc035, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4ea780, MapSizeDialog_Show);

// 0x4ea8c0 (callback). The map size dialog (0x6d10bc) reads its two sliders
// (children 0xc031 / 0xc033, +0x185 + 1 times 64 into 0x6d10b4 / 0x6d10b8)
// and is shown again.
void MapSizeDialog_ReadSliders() {
    if (g_6d10bc) {
        if (Widget_FindChild(g_6d10bc, 0, 0xc031))
            g_6d10b4 = (F<int32_t>(Widget_FindChild(g_6d10bc, 0, 0xc031), 0x185) + 1) << 6;
        if (g_6d10bc && Widget_FindChild(g_6d10bc, 0, 0xc033))
            g_6d10b8 = (F<int32_t>(Widget_FindChild(g_6d10bc, 0, 0xc033), 0x185) + 1) << 6;
    }
    MapSizeDialog_Show();
}
RT3_REPLACE(0x4ea8c0, MapSizeDialog_ReadSliders);

// 0x4ea940. Builds the map size panel into window `w` (also 0x6d10bc): the
// width and height captions (0xf4 / 0xf5) and their size lists (0xc031 /
// 0xc033, sizes 2..5 x 64 cells), value boxes 0xc032 / 0xc034 and the status
// box 0xc035. When resizing an existing map (0x6d10c0) the lists start at the
// current size (0x6d10c4 x 0x6d10c8, scaled so the larger side fits), else at
// the default; then refreshes the panel.
RT3_NOINLINE void __fastcall MapSizePanel_Build(uint8_t* w, int, float x, float y, float width, float) {
    g_6d10bc = w;
    float cy = (float)((double)y + D(0x5c9b80));
    if (g_6d10c0) cy = (float)((double)cy + D(0x5d11b8));
    auto editBox = [&](uint16_t id, float ex, float ey, float ew, uint32_t eh, uint8_t kind, const char* text) {
        uint8_t def[0x91];
        memset(def, 0, sizeof def);
        F<float>(def, 0xb) = ey;
        F<float>(def, 0xf) = ew;
        F<float>(def, 7) = ex;
        F<uint32_t>(def, 0x13) = eh;
        if (id) F<uint16_t>(def, 0) = id;
        def[0x27] = kind;
        def[0x28] = 0;
        F<int32_t>(def, 0x34) = -1;
        String_Assign(&F<char*>(def, 0x77), text);
        def[0x33] = kind == 3 ? 3 : 1;
        uint8_t* e = (uint8_t*)ExeNew(0xf0);
        e = e ? EditBox_Construct(e, 0, def) : nullptr;
        Window_AddChild(w, 0, e, 0);
        Free(F<void*>(def, 0x77));
    };
    auto list = [&](uint16_t id) {
        cy = (float)((double)cy + D(0x5c9b80));
        uint8_t def[0xe3];
        memset(def, 0, sizeof def);
        F<int32_t>(def, 0x63) = 1;
        F<int32_t>(def, 0x67) = 1;
        F<int32_t>(def, 0x97) = 1;
        F<float>(def, 7) = x;
        F<float>(def, 0xb) = cy;
        F<uint32_t>(def, 0xf) = 0x3e700000;
        F<uint32_t>(def, 0x13) = 0x3cb851ec;
        F<uint16_t>(def, 0) = id;
        F<int32_t>(def, 0x5f) = 0x10;
        F<int32_t>(def, 0x87) = 2;
        F<int32_t>(def, 0x8b) = 3;
        F<int32_t>(def, 0x8f) = 4;
        F<int32_t>(def, 0x93) = 5;
        F<int32_t>(def, 0x9b) = 0;
        F<uint32_t>(def, 0xab) = 0x3c088889;
        F<uint32_t>(def, 0xaf) = 0x3ceb851f;
        F<uint32_t>(def, 0xb3) = 0x3e500000;
        F<uint32_t>(def, 0xb7) = 0;
        F<uint32_t>(def, 0xbb) = 0;
        F<uint32_t>(def, 0xbf) = 0x3bcccccd;
        F<uint32_t>(def, 0xc3) = 0x3c088889;
        F<uint32_t>(def, 0xc7) = 0x3e51eb85;
        F<uint32_t>(def, 0xcb) = 0x3c088889;
        String_Assign(&F<char*>(def, 0x43), (const char*)0x5ce8d8);
        uint8_t* l = (uint8_t*)ExeNew(0x1b9);
        l = l ? ListBox_Construct(l, 0, def) : nullptr;
        Window_AddChild(w, 0, l, 0);
        Free(F<void*>(def, 0x43));
        Window_SendEvent5(w, 0, 0x84, id, 0x4ea8c0, 0, 0);
    };
    editBox(0, x, (float)((double)cy + D(0x5c8678)), width, 0x3cf5c28f, 0, GetString(0xf4));
    list(0xc031);
    float xr = (float)((double)x + D(0x5c9c80));
    float wr = (float)((double)width - D(0x5c9c80));
    editBox(0xc032, xr, cy, wr, 0x3cf5c28f, 3, (const char*)0x5c87a8);
    cy = (float)((double)cy + D(0x5c9b80));
    cy = (float)((double)cy + D(0x5ca1a8));
    editBox(0, x, (float)((double)cy + D(0x5c8678)), width, 0x3cf5c28f, 0, GetString(0xf5));
    list(0xc033);
    editBox(0xc034, xr, cy, wr, 0x3cf5c28f, 3, (const char*)0x5c87a8);
    cy = (float)((double)cy + D(0x5c9b80));
    cy = (float)((double)cy + D(0x5c8918));
    {
        uint8_t def[0x91];
        memset(def, 0, sizeof def);
        F<float>(def, 0xb) = cy;
        F<float>(def, 0xf) = width;
        F<float>(def, 7) = x;
        F<uint32_t>(def, 0x13) = 0x3dcccccd;
        F<uint16_t>(def, 0) = 0xc035;
        def[0x27] = 0;
        def[0x28] = 0;
        F<int32_t>(def, 0x34) = -1;
        String_Assign(&F<char*>(def, 0x77), (const char*)0x5c87a8);
        def[0x33] = 3;
        uint8_t* e = (uint8_t*)ExeNew(0xf0);
        e = e ? EditBox_Construct(e, 0, def) : nullptr;
        Window_AddChild(w, 0, e, 0);
        Free(F<void*>(def, 0x77));
    }
    if (g_6d10c0) {
        int32_t a = g_6d10c4, b = g_6d10c8;
        int32_t m = a > b ? a : b;
        float s = (float)(D(0x5d11b0) / (double)m);
        float wv = (float)((double)(float)a * s);
        if (Fl(0x5ca210) > wv) wv = 64.0f;
        int32_t iw = Ftol((double)wv);
        float hv = (float)((double)(float)b * s);
        if (Fl(0x5ca210) > hv) hv = 64.0f;
        Window_SendEvent5(w, 0, 0x66, 0xc031, (uint32_t)((iw - 0x40) / 64), 0, 0);
        Window_SendEvent5(w, 0, 0x66, 0xc033, (uint32_t)((Ftol((double)hv) - 0x40) / 64), 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x66, 0xc031, 4, 0, 0);
        Window_SendEvent5(w, 0, 0x66, 0xc033, 4, 0, 0);
    }
    MapSizeDialog_Show();
}
RT3_REPLACE(0x4ea940, MapSizePanel_Build);

// 0x4eb0b0. Asks for the map size (string 0xf6, panel 0x4ea940); stores the
// chosen width / height 0x6d10b4 / 0x6d10b8 and returns whether it was OK'd.
RT3_NOINLINE uint32_t __fastcall MapSize_Prompt(uint32_t* width, uint32_t* height) {
    uint32_t r = Dialog_RunMessage(GetString(0xf6), 2, 0xbf800000, 0xbf800000, 0, (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4ea940, 0, Prod_Bits(0x3e75c28f), 0, 0x1e, 0, nullptr,
                                   nullptr, 0, 0, nullptr, 0);
    *width = hook::As<uint32_t>(g_6d10b4);
    *height = hook::As<uint32_t>(g_6d10b8);
    return r == 0x3f2;
}
RT3_REPLACE(0x4eb0b0, MapSize_Prompt);

// 0x4eb120 (amounts in ECX / EDX). Multiplayer money transfer prompt: shows
// string 0xfe1 with both amounts (format 8) and the amount panel 0x4ea940
// (state 0x6d10c0..0x6d10c8), and returns the chosen pair 0x6d10b4 / 0x6d10b8.
RT3_NOINLINE void __fastcall Money_PromptTransfer(int32_t* a, int32_t* b) {
    g_6d10c0 = 1;
    g_6d10c4 = *a;
    g_6d10c8 = *b;
    char nb[0x200], na[0x200], text[0x400];
    const char* sb = Num_Format(nb, 8, (double)g_6d10c8, 10, 1, 0, 0);
    const char* sa = Num_Format(na, 8, (double)g_6d10c4, 10, 1, 0, 0);
    FormatText(text, GetString(0xfe1), sa, sb, 0, 0, 0, 0, 0, 0);
    Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4ea940, 0,
                      Prod_Bits(0x3ebbbbbc), 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    *a = g_6d10b4;
    *b = g_6d10b8;
    g_6d10c0 = 0;
}
RT3_REPLACE(0x4eb120, Money_PromptTransfer);

// 0x4eb220 (callback). The value (EDX) is not 999.
int32_t __fastcall Value_IsSet(void*, const int32_t* v) { return *v != 0x3e7; }
RT3_REPLACE(0x4eb220, Value_IsSet);

// 0x4eb230. Copies 0x2a bytes of this object into the global 0x6d10d8.
RT3_NOINLINE void __fastcall Obj_CopyTo6d10d8(const uint8_t* o) {
    uint8_t* d = (uint8_t*)0x6d10d8;
    for (int k = 0; k < 0x2a; ++k) d[k] = o[k];
}
RT3_REPLACE(0x4eb230, Obj_CopyTo6d10d8);

// 0x4eb250 (callback). Adds d (EDX) to the base 0x6d1110 (0x6d10cc) and
// prices it with the company 0x6d10d0's +0x47 (0x6d1114).
void __fastcall Company_PriceShares(void*, int32_t d) {
    const uint32_t id = g_6d10d0;
    g_6d10cc = g_6d1110 + d;
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, id);
    g_6d1114 = F<int32_t>(c, 0x47) * g_6d10cc;
}
RT3_REPLACE(0x4eb250, Company_PriceShares);

// 0x4eb280 / 0x4eb340. The share dialog (0x6d1118) shows company `id`
// (0x6d10d0): its button 0xbfce is enabled for public companies, the price
// (0x6d1110, 10 for private ones) goes on the slider.
void __fastcall ShareDialog_SelectCompany(uint32_t id) {
    if (id == g_6d10d0) return;
    g_6d10d0 = id;
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, id);
    int32_t price;
    if (!F<int32_t>(c, 0x3f)) {
        Window_SendEvent5(g_6d1118, 0, 0x87, 0xbfce, 3, 0, 0);
        price = 0xa;
    } else {
        Window_SendEvent5(g_6d1118, 0, 0x65, 0xbfce, 3, 0, 0);
        price = Ftol(Company_SharePrice((uint8_t*)Collection_Get(g_companies, 0, g_6d10d0), 0, 0, 0));
    }
    g_6d1110 = price;
    Window_SendEvent5(g_6d1118, 0, 0x73, 0xbfce, (uint32_t)(price + 1), 0, 0);
    Window_SendEvent5(g_6d1118, 0, 0x66, 0xbfce, 1, 0, 0);
    Window_SendEvent5(g_6d1118, 0, 0x66, 0xbfce, 0, 0, 0);
}
RT3_REPLACE(0x4eb280, ShareDialog_SelectCompany);

void __fastcall ShareDialog_SelectCompanyCb(void*, uint32_t id) { ShareDialog_SelectCompany(id); }
RT3_REPLACE(0x4eb340, ShareDialog_SelectCompanyCb);

// 0x4eb350 (draw callback). Draws company `id`'s logo (0.3 scale) and its
// name right-aligned next to it.
void __fastcall CompanyList_DrawLogoCb(void*, uint32_t id, float x, float y, float w, float h, int32_t) {
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, id);
    const float ly = (float)((double)y + D(0x5caae0));
    const float lx = (float)((double)x + D(0x5d0818));
    Glyph_Queue((uint8_t*)(uintptr_t)Company_Logo(c), 0, lx, ly, 0x3f800000, BitsF(0x3e99999a), BitsF(0x3e99999a), 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    const float tw = (float)((double)w - D(0x5d06d8));
    const float tx = (float)((double)x + D(0x5d06d8));
    Font_DrawTextBox(g_6d1120, 0, (const char*)(c + 4), tx, y, tw, h, 0xffffffff, 2, 0, 0, 1.0f);
}
RT3_REPLACE(0x4eb350, CompanyList_DrawLogoCb);

// 0x4eb420 (draw callback). Company overview (company 0x6d10d0): the
// player's company's stat 0xd (string 0x2d0), then for a company without
// shares (+0x3f) its two debt sums (string 0xfe2) in a box, else its price
// (0x6d10cc, 0x2d3), shares (+0x47, 0x2d4) and market value (0x6d1114,
// 0x2d5) with the footnote (0x2d6) below.
void __fastcall CompanyOverview_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    const float tx = (float)((double)x + D(0x5d0830));
    const float ty = (float)((double)y + D(0x5caae0));
    const float tw = (float)((double)w - D(0x5d0700));
    const float rw = (float)((double)tw - D(0x5d08e8));
    const float rx = (float)((double)tx + D(0x5d08f0));
    const float rh = BitsF(0x3cda740e);
    char frame[0xb6c];
    char* num = frame + 0x18;
    Font_DrawTextBox(g_6d111c, 0, GetString(0x2d0), rx, ty, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_FormatSimple(num, 9, Company_Stat((uint8_t*)Obj_Company21(g_world), 0, 0xd, 0x2329), 0xb);
    Font_DrawTextBox(g_6d111c, 0, num, tx, ty, tw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    const float ty2 = (float)((double)ty + D(0x5d11d0));
    Font_DrawTextBox(g_6d111c, 0, GetString(0x2d1), tx, ty2, tw, rh, 0xffffffff, 0, 1, 0, 1.0f);
    float row = (float)((double)y + D(0x5c9f28));
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, g_6d10d0);
    if (!F<int32_t>(c, 0x3f)) {
        char* a = frame + 0x2d4;
        char* b = frame + 0x20c;
        char* text = frame + 0x39c;
        Num_FormatSimple2(a, 0xb, (double)Obj_Sum12a7Scaled(c), 0xb);
        Num_FormatSimple2(b, 0xb, (double)Obj_Sum12a7(c), 0xb);
        FormatText(text, GetString(0xfe2), b, a, 0, 0, 0, 0, 0, 0);
        const float bh = (float)((double)h - ((double)row - y));
        Font_DrawTextBox(g_6d1120, 0, text, tx, row, tw, bh, 0xffffffff, 0, 1, 0, 1.0f);
        return;
    }
    Font_DrawTextBox(g_6d111c, 0, GetString(0x2d2), tx, row, tw, rh, 0xffffffff, 0, 1, 0, 1.0f);
    row = (float)((double)y + D(0x5d11c8));
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, g_6d10d0);
    Font_DrawTextBox(g_6d111c, 0, GetString(0x2d3), rx, row, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_FormatSimple(num, 9, (double)g_6d10cc, 0xb);
    Font_DrawTextBox(g_6d111c, 0, num, rx, row, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    row = (float)((double)row + Fl(0x5d0898));
    Font_DrawTextBox(g_6d111c, 0, GetString(0x2d4), rx, row, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_Format(num, 1, (double)F<int32_t>(co, 0x47), 10, 1, 0, 0);
    Font_DrawTextBox(g_6d111c, 0, num, rx, row, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    row = (float)((double)row + Fl(0x5d0898));
    Font_DrawTextBox(g_6d111c, 0, GetString(0x2d5), rx, row, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_FormatSimple2(num, 0xb, (double)g_6d1114, 0xb);
    Font_DrawTextBox(g_6d111c, 0, num, rx, row, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    row = (float)((double)row + Fl(0x5d0898));
    const float nx = (float)((double)rx - D(0x5d08f0));
    const float nw = (float)((double)rw + D(0x5d08e8));
    row = (float)((double)row + D(0x5d11c0));
    const float bh = (float)((double)h - ((double)row - y));
    Font_DrawTextBox(g_6d1120, 0, GetString(0x2d6), nx, row, nw, bh, 0xffffffff, 0, 1, 0, 1.0f);
}
RT3_REPLACE(0x4eb420, CompanyOverview_DrawCb);

// 0x4eb890. Merger window: the vote table of company 0x6d10de (offer from
// 0x6d10dc) at (x, y, w) in font 0x6d111c, like the stock window's holder
// table: title 0x2d9 / 0x2da (by the shares 0x6d110c against half of +0x47),
// two groups 0x2db / 0x2dc (players with / without the flag 0x6d10f2) with
// each holder's shares, the group total (0x2dd: 0x6d1108, or 0x6d1104 -
// 0x6d1108) padded to 8 places, and its share count a bit lower.
void __fastcall MergerWindow_DrawVotes(void*, int, float x, float y, float w, float, int32_t) {
    float xs = (float)((double)x + D(0x5d0110));
    float ws = (float)((double)w - D(0x5c9b50));
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, g_6d10de);
    Collection_Get(g_companies, 0, g_6d10dc);
    Company_LiveIndex(co);
    const float h = BitsF(0x3ce81b4f);
    char title[0x200], num[0x130];
    strcpy(title, GetString(g_6d110c > F<int32_t>(co, 0x47) / 2 ? 0x2d9 : 0x2da));
    Font_DrawTextBox(g_6d111c, 0, title, xs, y, ws, h, 0xffffffff, 0, 1, 0, 1.0f);
    float cy = (float)((double)y + Fl(0x5d11fc));
    for (int32_t k = 0; k < 2; ++k) {
        strcpy(title, GetString(k == 0 ? 0x2db : 0x2dc));
        Font_DrawTextBox(g_6d111c, 0, title, xs, cy, ws, h, 0xffffffff, 0, 1, 0, 1.0f);
        cy = (float)((double)cy + Fl(0x5d11fc));
        for (int32_t i = 0; i < Table_CountField4(g_players); ++i) {
            uint8_t* p = (uint8_t*)Table_NthField4(g_players, 0, i);
            int32_t n = F<int32_t>(p, 0x15d + g_6d10de * 4);
            if (n <= 0) continue;
            bool flagged = ((const uint8_t*)0x6d10f2)[i] != 0;
            if (k == 0 ? !flagged : flagged) continue;
            Num_Format(num, 1, (double)n, 10, 1, 0, 0);
            Font_DrawTextBox(g_6d111c, 0, (const char*)(p + 8), xs, cy, ws, h, 0xffffffff, 2, 1, 0, 1.0f);
            Font_DrawTextBox(g_6d111c, 0, num, xs, cy, ws, h, 0xffffffff, 3, 1, 0, 1.0f);
            cy = (float)((double)cy + Fl(0x5ca2a8));
        }
        int32_t total = k == 0 ? g_6d1108 : g_6d1104 - g_6d1108;
        Num_Format(num, 1, (double)total, 10, 1, 0, 0);
        Font_DrawTextBox(g_6d111c, 0, GetString(0x2dd), xs, cy, ws, h, 0xffffffff, 2, 1, 0, 1.0f);
        if (strlen(num) < 8) {
            char padded[0x1c];
            memcpy(padded, (const void*)0x5d11e0, sizeof padded);
            ExeSprintf()(padded + 8 - strlen(num), num);
            strcpy(num, padded);
        }
        Font_DrawTextBox(g_6d111c, 0, num, xs, cy, ws, h, 0xffffffff, 3, 1, 1, 1.0f);
        cy = (float)((double)cy + Fl(0x5ca2a8));
        int32_t shares = k == 0 ? g_6d110c : F<int32_t>(co, 0x47) - g_6d110c;
        Num_Format(num, 1, (double)shares, 10, 1, 0, 0);
        cy = (float)((double)cy + D(0x5d11d8));
        Font_DrawTextBox(g_6d111c, 0, num, xs, cy, ws, h, 0xffffffff, 3, 1, 0, 1.0f);
        cy = (float)((double)cy + Fl(0x5d11fc));
    }
}
RT3_REPLACE(0x4eb890, MergerWindow_DrawVotes);

// 0x4ebd10. Settles the merger offer in 0x6d10dc.. (buyer, target, price a
// share; single player copies it from the pending offer 0x6d10cc.. and
// marks all 16 players voted and in favour). Once every player has voted
// and both companies trade (+0x3f), the AI shareholders of the target vote:
// the premium of the price over the target's value (share price plus book
// value, halved, at least 1) scaled by the buyer's effect 0x3a, its stat
// 0x21 and +-0.1 noise, less 0.1 and 0.45 for the target's own chairman,
// +-0.2 noise, against a roll of 100. Their shares plus that appeal times
// the free float (rounded down to 1000s, 0x6d1108) carry it at more than
// half of the issued shares: the result window (0x4eb890), or the network
// message 0xbf3 with 0xbf4 / 0xbf5, then the takeover (0x427e20) or the
// buyer's last-offer time +0x15f.
RT3_NOINLINE void Merger_ResolveVote() {
    uint8_t* voted = (uint8_t*)0x6d10e2;
    uint8_t* yes = (uint8_t*)0x6d10f2;
    if (!IsMultiplayer()) {
        uint16_t price = F<uint16_t>((void*)0x6d10cc, 0), target = F<uint16_t>((void*)0x6d10d0, 0);
        memset((void*)0x6d10d8, 0, 0x2a);
        F<uint16_t>((void*)0x6d10dc, 0) = F<uint16_t>((void*)0x6d10d4, 0);
        F<uint16_t>((void*)0x6d10de, 0) = target;
        F<uint16_t>((void*)0x6d10e0, 0) = price;
        for (int32_t k = 0; k < 0x10; ++k) voted[k] = 1, yes[k] = 1;
    }
    bool waiting = false;
    for (int32_t i = 0; i < Collection_Count(g_players); ++i)
        if (!voted[i]) waiting = true;
    if (waiting) return;
    g_6d110c = 0;
    uint8_t* tgt = (uint8_t*)Collection_Get(g_companies, 0, F<uint16_t>((void*)0x6d10de, 0));
    uint8_t* buyer = (uint8_t*)Collection_Get(g_companies, 0, F<uint16_t>((void*)0x6d10dc, 0));
    if (!F<int32_t>(tgt, 0x3f) || !F<int32_t>(buyer, 0x3f)) return;
    Company_LiveIndex(tgt);
    g_6d1104 = Company_FreeShares(tgt);
    float value = Company_SharePrice(tgt, 0, 0, 0);
    value = (float)(((double)Company_BookValue(tgt) + value) / *(const float*)0x5c878c);
    if (value < *(const float*)0x5c85b4) value = 1.0f;
    float fprice = (float)(int32_t)F<uint16_t>((void*)0x6d10e0, 0);
    float premium = (float)((double)fprice / value - *(const double*)0x5c8770);
    float m = Effects_Multiplier(g_world, 0, 0x3a, F<uint16_t>((void*)0x6d10dc, 0), F<uint32_t>(buyer, 0x3b), 0);
    double scale = m < *(const double*)0x5c85b8 ? 0.01 : (double)m;
    premium = (float)((double)premium / scale);
    float appeal = (float)((double)premium * *(const double*)0x5c8680 + *(const double*)0x5c86b8);
    appeal = (float)((Company_Stat(buyer, 0, 0x21, 0x2329) - *(const double*)0x5c86b8) / *(const double*)0x5c85c8 + appeal);
    appeal = (float)((double)SimRand(-10, 10) / *(const double*)0x5c85d8 + appeal);
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i) {
        uint8_t* pl = (uint8_t*)Table_NthField4(g_players, 0, i);
        int32_t held = F<int32_t>(pl, F<uint16_t>((void*)0x6d10de, 0) * 4 + 0x15d);
        if (!F<int32_t>(pl, 0x1e1) && held > 0) {
            float will = (float)((double)appeal - *(const double*)0x5c86b8);
            if (F<int32_t>(pl, 0x1dd) == F<int32_t>(tgt, 0)) will = (float)((double)will - *(const double*)0x5c8c18);
            will = (float)((double)SimRand(-20, 20) / *(const double*)0x5c85d8 + will);
            float roll = (float)SimRand(0, 100);
            yes[i] = (double)will * *(const float*)0x5c8c28 > roll ? 1 : 0;
        }
        if (yes[i] && (held = F<int32_t>(pl, F<uint16_t>((void*)0x6d10de, 0) * 4 + 0x15d)) > 0) g_6d110c += held;
    }
    if (appeal < *(const float*)0x5c8598) appeal = 0.0f;
    if (appeal > *(const double*)0x5c8770) appeal = 1.0f;
    float free = (float)g_6d1104;
    int32_t extra = (int32_t)((double)free * appeal) / 0x3e8 * 0x3e8;
    g_6d1108 = extra;
    g_6d110c += extra;
    bool pass = g_6d110c > F<int32_t>(tgt, 0x47) / 2;
    if (!IsMultiplayer()) {
        uint32_t extraH = 0x3ed55555;
        Dialog_RunMessage((const char*)0x5c87a8, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0x4eb890, *(const float*)&extraH, 0, 0x1e, 0, nullptr, nullptr, 0, 0,
                          nullptr, 0);
    } else {
        const char* verdict = GetString(pass ? 0xbf4 : 0xbf5);
        uint8_t* t = (uint8_t*)Collection_Get(g_companies, 0, F<uint16_t>((void*)0x6d10de, 0));
        uint8_t* b = (uint8_t*)Collection_Get(g_companies, 0, F<uint16_t>((void*)0x6d10dc, 0));
        char text[0x100];
        const char* fmt = GetString(0xbf3);
        FormatText(text, fmt, (const char*)b + 4, (const char*)t + 4, verdict, nullptr, nullptr, nullptr, nullptr, nullptr);
        Net_QueueText(g_session, 0, text, 1, 0, 0, 0);
    }
    if (pass) {
        Company_Absorb(buyer, 0, F<uint16_t>((void*)0x6d10de, 0), F<uint16_t>((void*)0x6d10e0, 0));
        return;
    }
    F<int32_t>(Collection_Get(g_companies, 0, F<uint16_t>((void*)0x6d10dc, 0)), 0x15f) =
        F<int32_t>(g_world, 0x15);
}
RT3_REPLACE(0x4ebd10, Merger_ResolveVote);

// 0x4ec1d0. Company picker (window `win`, kept in 0x6d1118) at (x, y): a frame,
// the company list 0xbfcd (selection 0x4eb340, double click 0x4eb350) with
// every company but 0x6d10d4 that is active (+0x3f) or, from version 0x67,
// has assets, and the start button slider 0xbfce (handler 0x4eb250); the
// first listed company is selected. 0x6d10d0 is reset to -2.
void __fastcall CompanyPicker_Build(uint8_t* win, int, float x, float y, float, float) {
    float xa = (float)((double)x + D(0x5d1238));
    g_6d1118 = win;
    {
        uint8_t d[0x89];
        memset(d, 0, sizeof d);
        F<float>(d, 0xb) = (float)((double)y + D(0x5d1230));
        F<float>(d, 7) = xa;
        F<uint16_t>(d, 0x5f) = 1;
        String_Assign(&F<char*>(d, 0x43), (const char*)0x5d1210);
        uint8_t* f = (uint8_t*)ExeNew(0x111);
        f = f ? Scrollbar_Construct(f, 0, d) : nullptr;
        Window_AddChild(win, 0, f, 0);
        Free(F<void*>(d, 0x43));
    }
    {
        uint8_t t[0x12f];
        memset(t, 0, sizeof t);
        F<float>(t, 0xb) = (float)((double)y + D(0x5c86b0));
        F<float>(t, 7) = xa;
        F<uint32_t>(t, 0x8b) = 0x3e93851f;
        F<uint32_t>(t, 0x93) = 0x3e93851f;
        F<uint32_t>(t, 0xf) = 0x3e9e6666;
        F<uint32_t>(t, 0x13) = 0x3e6147ae;
        t[0x33] = 2;
        F<uint16_t>(t, 0) = 0xbfcd;
        static const uint32_t parts[10] = { 0x3ba3d70a, 0x3c3f258c, 0x3e8947ae, 0x3cfa4fa5, 0x3b23d70a,
                                            0x3b4ccccd, 0x3d088889, 0x3e41b4e8, 0x3e9428f6, 0x3ce147ae };
        for (int k = 0; k < 10; ++k) F<uint32_t>(t, 0x63 + k * 4) = parts[k];
        F<uint32_t>(t, 0x8f) = 0;
        F<uint32_t>(t, 0x97) = 0x3e451eb8;
        F<int32_t>(t, 0x9f) = 1;
        F<int32_t>(t, 0xa3) = 1;
        F<int32_t>(t, 0xc3) = 0;
        F<int32_t>(t, 0xc7) = 1;
        F<int32_t>(t, 0xcb) = 2;
        F<int32_t>(t, 0xcf) = 3;
        F<int32_t>(t, 0xd3) = 4;
        F<int32_t>(t, 0xd7) = 5;
        F<int32_t>(t, 0x5f) = 6;
        String_Assign(&F<char*>(t, 0x43), (const char*)0x5d0a10);
        uint8_t* tv = (uint8_t*)ExeNew(0x1ea);
        tv = tv ? TreeView_Construct(tv, 0, t) : nullptr;
        Window_AddChild(win, 0, tv, 0);
        Window_SendEvent5(win, 0, 0x79, 0xbfcd, 0x4eb340, 0, 0);
        Window_SendEvent5(win, 0, 0x7a, 0xbfcd, 0x4eb350, 0, 0);
        Free(F<void*>(t, 0x43));
    }
    hook::As<int32_t>(g_6d10d0) = -2;
    int32_t first = -1;
    for (int32_t i = 0; i < Collection_Count(g_companies); ++i) {
        Collection* cs = g_companies;
        uint8_t* co = (uint8_t*)Collection_Get(cs, 0, Collection_IdAt(cs, 0, (uint32_t)i, 0));
        if (!F<int32_t>(co, 0x3f)) {
            if (!Company_HasAssets(co)) continue;
            if (Game_EffectiveVersion() < 0x67) continue;
        }
        int32_t id = F<int32_t>(co, 0);
        if (id == hook::As<int32_t>(g_6d10d4)) continue;
        if (first == -1) first = id;
        Window_SendEvent5(win, 0, 0x88, 0xbfcd, 0, (uint32_t)id, 0);
    }
    {
        uint8_t s[0xe3];
        memset(s, 0, sizeof s);
        F<float>(s, 7) = (float)((double)x + D(0x5d1208));
        F<int32_t>(s, 0x63) = 1;
        F<int32_t>(s, 0x67) = 1;
        F<int32_t>(s, 0x97) = 1;
        F<float>(s, 0xb) = (float)((double)y + D(0x5d1200));
        F<uint32_t>(s, 0xf) = 0x3e700000;
        F<uint32_t>(s, 0x13) = 0x3cb851ec;
        F<uint16_t>(s, 0) = 0xbfce;
        F<int32_t>(s, 0x5f) = 0xa;
        F<int32_t>(s, 0x87) = 2;
        F<int32_t>(s, 0x8b) = 3;
        F<int32_t>(s, 0x8f) = 4;
        F<int32_t>(s, 0x93) = 5;
        F<int32_t>(s, 0x9b) = 0;
        F<uint32_t>(s, 0xab) = 0x3c088889;
        F<uint32_t>(s, 0xaf) = 0x3ceb851f;
        F<uint32_t>(s, 0xb3) = 0x3e500000;
        F<uint32_t>(s, 0xb7) = 0;
        F<uint32_t>(s, 0xbb) = 0;
        F<uint32_t>(s, 0xbf) = 0x3bcccccd;
        F<uint32_t>(s, 0xc3) = 0x3c088889;
        F<uint32_t>(s, 0xc7) = 0x3e51eb85;
        F<uint32_t>(s, 0xcb) = 0x3c088889;
        String_Assign(&F<char*>(s, 0x43), (const char*)0x5ce8d8);
        uint8_t* lb = (uint8_t*)ExeNew(0x1b9);
        lb = lb ? ListBox_Construct(lb, 0, s) : nullptr;
        Window_AddChild(win, 0, lb, 0);
        Free(F<void*>(s, 0x43));
    }
    Window_SendEvent5(win, 0, 0x84, 0xbfce, 0x4eb250, 0, 0);
    if (first != -1) Window_SendEvent5(win, 0, 0x68, 0xbfcd, (uint32_t)first, 0, 0);
}
RT3_REPLACE(0x4ec1d0, CompanyPicker_Build);

// 0x4ec640. The player's company (0x6d10d4) picks a takeover target in the
// merger dialog (layout 0x4eb220 / 0x4ec1d0 / 0x4eb420, fonts 2 and 1 in
// 0x6d111c / 0x6d1120; target 0x6d10d0, price 0x6d10cc). Not with a single
// company (0x2d7) or within a year of its last refused offer (+0x15f, 0x2d8,
// unless 0x4338c0); QUIRK: those return without releasing the fonts. A
// bankrupt target (no stock) is bought for 0x424200 if the cash (stat 0xd)
// covers it (else 0xfe3), a listed one is offered price x shares (else
// 0xf31) and put to the vote (0x4ebd10); in multiplayer both go to the host
// (0x79 / 0x46; QUIRK: the offer message leaves its last 4 bytes unset).
RT3_NOINLINE void Merger_OfferDialog() {
    g_6d111c = Pack_AcquireFont(g_pack, 0, 2);
    g_6d1120 = Pack_AcquireFont(g_pack, 0, 1);
    auto say = [](int32_t id) {
        Dialog_RunMessage(GetString(id), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    };
    if (Table_CountField3f(g_companies) <= 1) {
        say(0x2d7);
        return;
    }
    int32_t mine = Obj_Get21(g_world);
    hook::As<int32_t>(g_6d10d4) = mine;
    hook::As<int32_t>(g_6d10d0) = -1;
    g_6d10cc = 0;
    g_6d1114 = 0;
    g_6d1110 = 0;
    Collection* cos = g_companies;
    if (F<int32_t>(Collection_Get(cos, 0, (uint32_t)mine), 0x15f)) {
        int32_t now = F<int32_t>(g_world, 0x15);
        if (now - F<int32_t>(Collection_Get(g_companies, 0, g_6d10d4), 0x15f) < 0x76200 &&
            !Global6d07b4Set2()) {
            say(0x2d8);
            return;
        }
    }
    uint32_t extraBits = 0x3f0aaaab;
    uint32_t r = Dialog_RunMessage((const char*)0x5c87a8, 2, 0xbf800000, 0xbf800000, 0x4eb220,
                                   (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4ec1d0, 0x4eb420, *(const float*)&extraBits, 0, 0x1e,
                                   0, nullptr, nullptr, 0, 0, nullptr, 0);
    char num1[0x80], num2[0x80], num3[0x80], text[0x200];
    if (r == 0x3f2) {
        uint8_t* tgt = (uint8_t*)Collection_Get(g_companies, 0, g_6d10d0);
        if (!F<int32_t>(Collection_Get(g_companies, 0, g_6d10d0), 0x3f)) {
            float cost = Obj_Sum12a7Scaled(tgt);
            uint8_t* me = (uint8_t*)Collection_Get(g_companies, 0, g_6d10d4);
            if (Company_Stat(me, 0, 0xd, 0x2329) < (double)cost) {
                me = (uint8_t*)Collection_Get(g_companies, 0, g_6d10d4);
                const char* have = Num_FormatSimple(num1, 9, Company_Stat(me, 0, 0xd, 0x2329), 0xb);
                const char* need = Num_FormatSimple(num2, 9, cost, 0xb);
                const char* fmt = GetString(0xfe3);
                FormatText(text, fmt, need, have, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            } else if (!IsMultiplayer()) {
                Company_Absorb((uint8_t*)Collection_Get(g_companies, 0, g_6d10d4), 0,
                               g_6d10d0, 0);
            } else {
                int32_t msg[3];
                msg[0] = Global6d40e4();
                msg[1] = hook::As<int32_t>(g_6d10d4);
                msg[2] = hook::As<int32_t>(g_6d10d0);
                Net_SendToHost(g_session, 0, 0x79, 0xc, (const uint8_t*)msg);
                Cursor_Set(g_cursorManager, 0, 6, 0, 0);
            }
        } else {
            int32_t total = F<int32_t>(Collection_Get(g_companies, 0, g_6d10d0), 0x47) * g_6d10cc;
            float cost = (float)total;
            uint8_t* me = (uint8_t*)Collection_Get(g_companies, 0, g_6d10d4);
            if (Company_Stat(me, 0, 0xd, 0x2329) < (double)cost) {
                me = (uint8_t*)Collection_Get(g_companies, 0, g_6d10d4);
                const char* have = Num_FormatSimple(num1, 9, Company_Stat(me, 0, 0xd, 0x2329), 0xb);
                const char* need = Num_FormatSimple(num2, 9, cost, 0xb);
                const char* each = Num_FormatSimple(num3, 9, (double)g_6d10cc, 0xb);
                const char* fmt = GetString(0xf31);
                FormatText(text, fmt, each, need, have, nullptr, nullptr, nullptr, nullptr, nullptr);
                Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            } else if (!IsMultiplayer()) {
                Merger_ResolveVote();
            } else {
                uint8_t msg[0xc];
                F<int32_t>(msg, 0) = Global6d40e4();
                msg[4] = (uint8_t)hook::As<int32_t>(g_6d10d4);
                F<uint16_t>(msg, 6) = (uint16_t)g_6d10cc;
                msg[5] = (uint8_t)hook::As<int32_t>(g_6d10d0);
                Net_SendToHost(g_session, 0, 0x46, 0xc, msg);
                Log_Add(g_messageLog, 0, GetString(0xbf2), 0xb, 0, 0);
            }
        }
    }
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d111c));
    Pack_Release(g_pack, 0, hook::As<void*>(g_6d1120));
    hook::As<int32_t>(g_6d1118) = 0;
}
RT3_REPLACE(0x4ec640, Merger_OfferDialog);

// 0x4ecb20. Clears the new-game player slots and the lobby buffer 0x6d1270.
RT3_NOINLINE void NewGameConfig_ClearSlots() {
    uint8_t* c = g_newGame;
    c[0x97] = 1;
    F<uint16_t>(c, 0x79) = 0;
    F<uint16_t>(c, 0x7b) = 0;
    for (int k = 0; k < 16; ++k) c[0x87 + k] = 0;
    for (int k = 0; k < 0x33; ++k) c[0x44 + k] = 0;
    if (uint8_t* l = g_6d1270)
        for (int k = 0; k < 0x100f0 * 4 + 2; ++k) l[k] = 0;
}
RT3_REPLACE(0x4ecb20, NewGameConfig_ClearSlots);

// 0x4ecb90. Free AI slot check: mode 1 tests whether an unassigned slot with
// a lobby entry exists (0xff); otherwise claims the highest one (slot id
// k + 100, +0x7b++). 0 when none (or in mode 0x6cd914 == 1).
RT3_NOINLINE int32_t __fastcall NewGameConfig_ClaimAiSlot(int32_t mode) {
    if (g_6cd914 == 1) return 0;
    uint8_t* c = g_newGame;
    uint8_t* l = g_6d1270;
    if (mode == 1) {
        if (!l) return 0;
        int32_t n7b = F<uint16_t>(c, 0x7b), n79 = F<uint16_t>(c, 0x79);
        if (F<int32_t>(l, 0x3ae) <= n79 + n7b || F<int32_t>(l, 0x3be) <= n7b) return 0;
        for (int32_t k = 15; k >= 0; --k)
            if (!c[0x87 + k] && l[0x31c + k * 9]) return 0xff;
        return 0;
    }
    for (int32_t k = 15; k >= 0; --k) {
        if (c[0x87 + k] || !l[0x31c + k * 9]) continue;
        c[0x87 + k] = (uint8_t)(k + 100);
        F<uint16_t>(g_newGame, 0x7b)++;
        return 0xff;
    }
    return 0;
}
RT3_REPLACE(0x4ecb90, NewGameConfig_ClaimAiSlot);

// 0x4ecc50. Lobby entries (9-byte, from +0x31b, count +0x3ae) with both flags clear.
RT3_NOINLINE int32_t __fastcall Lobby_CountFreeEntries(uint8_t* l) {
    if (!l) l = g_6d1270;
    if (!l) return 0;
    int32_t n = 0;
    for (int32_t i = 0; i < F<int32_t>(l, 0x3ae); ++i)
        if (!l[0x31b + i * 9] && !l[0x31d + i * 9]) ++n;
    return n;
}
RT3_REPLACE(0x4ecc50, Lobby_CountFreeEntries);

// 0x4ecc90. Sets the lobby mode 0x6d1288 for kinds 5, 6 and 8.
RT3_NOINLINE void __fastcall Lobby_SetMode(int32_t kind, int, int32_t, int32_t) {
    if (kind == 5) g_6d1288 = 5;
    else if (kind == 6) g_6d1288 = 6;
    else if (kind == 8) g_6d1288 = 8;
}
RT3_REPLACE(0x4ecc90, Lobby_SetMode);

// 0x4eccd0. Stores the player name (options +0x1ef) and shows it in fields 0xe48/0xe5d.
RT3_NOINLINE void __fastcall NameDialog_SetName(uint8_t* d, int, const char* name) {
    if (!name) return;
    Copy((char*)Options() + 0x1ef, name);
    SetChildText(d, 0xe48, (const char*)Options() + 0x1ef);
    SetChildText(d, 0xe5d, (const char*)Options() + 0x1ef);
}
RT3_REPLACE(0x4eccd0, NameDialog_SetName);

// 0x4ecdd0. Player setup: the portrait picker at (x, y): the portrait
// "portraitL<nn>01.imb" for portrait 0x6226d0 in scrollbar-style frame 0x3f1f,
// and the previous / next buttons 0x3f20 / 0x3f21 ("Setup_Choose_Map_Buttons.imb",
// tooltips 0xe62 / 0xe65).
RT3_NOINLINE void __fastcall Setup_AddPortraitPicker(uint8_t* win, int, float x, float y, int32_t, int32_t) {
    uint8_t def[0x89];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + D(0x5c8590));
    char name[0x30];
    ExeSprintf()(name, (const char*)0x5d0ab0, g_6226d0);
    F<float>(def, 0xb) = (float)((double)y + D(0x5c8918));
    F<uint32_t>(def, 0xf) = 0x3dcccccd;
    F<uint32_t>(def, 0x13) = 0x3e19999a;
    F<uint16_t>(def, 0x5f) = 0;
    F<uint16_t>(def, 0) = 0x3f1f;
    String_Assign(&F<char*>(def, 0x43), name);
    uint8_t* pic = (uint8_t*)ExeNew(0x111);
    if (pic) pic = Scrollbar_Construct(pic, 0, def);
    F<uint32_t>(pic, 0x100) = 0x3f3ae148;
    F<uint32_t>(pic, 0x104) = 0x3f3851ec;
    Window_AddChild(win, 0, pic, 0);
    Free(F<void*>(def, 0x43));
    float bx = (float)((double)x + D(0x5c9c80));
    auto button = [&](uint16_t id, float by, uint16_t up, uint16_t down, int32_t tip) {
        uint8_t b[0xa5];
        memset(b, 0, sizeof b);
        F<float>(b, 0xb) = by;
        F<float>(b, 7) = bx;
        F<uint32_t>(b, 0xf) = 0x3cdc28f6;
        F<uint32_t>(b, 0x13) = 0x3ce147ae;
        F<uint16_t>(b, 0x5f) = up;
        F<uint16_t>(b, 0x61) = down;
        F<uint16_t>(b, 0) = id;
        b[0x89] = 6;
        b[0x8a] = 7;
        String_Assign(&F<char*>(b, 0x43), (const char*)0x5d08b8);
        String_Assign(&F<char*>(b, 0x3f), GetString(tip));
        uint8_t* c = (uint8_t*)ExeNew(0xe9);
        if (c) c = Control_Construct(c, 0, b);
        Window_AddChild(win, 0, c, 0);
        Free(F<void*>(b, 0x43));
        Free(F<void*>(b, 0x3f));
    };
    button(0x3f20, (float)((double)y + D(0x5c86e0)), 2, 3, 0xe62);
    button(0x3f21, (float)((double)y + D(0x5c8648)), 4, 5, 0xe65);
}
RT3_REPLACE(0x4ecdd0, Setup_AddPortraitPicker);

// 0x4ed050 (event callback). Releases of 0x3f20 / 0x3f21 step the count
// 0x6226d0 up / down through 1..40 and show it (format 0x5d0ab0) on
// 0x3f1f. Returns 0 then, else 1.
RT3_NOINLINE int32_t __fastcall Count40_StepCb(uint8_t* w, const uint32_t* ev) {
    if (ev[0] != 0xcb) return 1;
    int32_t v;
    if (ev[1] == 0x3f20) {
        v = g_6226d0 + 1;
        g_6226d0 = v;
        if (v == 0x29) g_6226d0 = v = 1;
    } else if (ev[1] == 0x3f21) {
        if (--g_6226d0 == 0) g_6226d0 = 0x28;
        v = g_6226d0;
    } else {
        return 1;
    }
    char text[0x40];
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(text, (const char*)0x5d0ab0, v);
    Window_SendEvent5(w, 0, 0x70, 0x3f1f, (uint32_t)(uintptr_t)text, 0, 0);
    return 0;
}
RT3_REPLACE(0x4ed050, Count40_StepCb);

// 0x4ed100. Adds two labelled text entry fields at (x, y, width w): labels
// 0xe63 / 0xe64 (y and y + 0.0833) each over a frame (+0.0333 / +0.1167) holding
// an editable box 0xe5f / 0xe60 of up to 27 characters on the buffers
// 0x6d1128 / 0x6d1228.
void __fastcall Dialog_AddTwoEntryFields(uint8_t* win, int, float x, float y, float w, int32_t) {
    auto addEdit = [&](auto fill) {
        uint8_t d[0x91];
        memset(d, 0, sizeof d);
        fill(d);
        uint8_t* e = (uint8_t*)ExeNew(0xf0);
        e = e ? EditBox_Construct(e, 0, d) : nullptr;
        Window_AddChild(win, 0, e, 0);
        Free(F<void*>(d, 0x77));
    };
    auto addFrame = [&](float fy) {
        uint8_t d[0x89];
        memset(d, 0, sizeof d);
        F<float>(d, 7) = x;
        F<float>(d, 0xb) = fy;
        F<float>(d, 0xf) = w;
        F<uint32_t>(d, 0x13) = 0x3d23d70a;
        F<int32_t>(d, 0x61) = 3;
        F<int32_t>(d, 0x75) = 6;
        uint8_t* f = (uint8_t*)ExeNew(0x111);
        f = f ? Scrollbar_Construct(f, 0, d) : nullptr;
        Window_AddChild(win, 0, f, 0);
    };
    auto label = [&](float ly, int32_t str) {
        addEdit([&](uint8_t* d) {
            F<float>(d, 0xb) = ly;
            F<float>(d, 7) = x;
            F<float>(d, 0xf) = w;
            F<uint32_t>(d, 0x13) = 0x3d088889;
            F<int32_t>(d, 0x34) = -1;
            F<int32_t>(d, 0x2f) = 0xb0;
            String_Assign(&F<char*>(d, 0x77), GetString(str));
            d[0x33] = 3;
            d[0x27] = 0;
        });
    };
    auto entry = [&](float ex, float ey, uint16_t id, uint32_t buf) {
        addEdit([&](uint8_t* d) {
            F<float>(d, 0xb) = ey;
            F<float>(d, 7) = ex;
            F<float>(d, 0xf) = w;
            F<uint32_t>(d, 0x13) = 0x3d23d70a;
            F<uint16_t>(d, 0) = id;
            d[0x27] = 0;
            d[0x28] = 0;
            F<int32_t>(d, 0x34) = -1;
            F<int32_t>(d, 0x5f) = 1;
            F<int32_t>(d, 0x63) = 1;
            F<int32_t>(d, 0x67) = 0x1b;
            String_Assign(&F<char*>(d, 0x77), (const char*)buf);
            d[0x33] = 3;
        });
    };
    label(y, 0xe63);
    float fy1 = (float)((double)y + D(0x5caaf0));
    addFrame(fy1);
    float ex = (float)((double)x - D(0x5ce8d0));
    entry(ex, fy1, 0xe5f, 0x6d1128);
    label((float)((double)y + D(0x5c9b40)), 0xe64);
    float fy2 = (float)((double)y + D(0x5d1240));
    addFrame(fy2);
    entry(ex, fy2, 0xe60, 0x6d1228);
}
RT3_REPLACE(0x4ed100, Dialog_AddTwoEntryFields);

// 0x4ed4c0 (event callback). Copies the two name fields (children 0xe5f /
// 0xe60, text +0xba, 27 characters) to 0x6d1128 / 0x6d1228 when edited or
// on release of 0x3f3. Always 1.
int32_t __fastcall NameFields_CopyCb(uint8_t* w, const uint32_t* ev) {
    if (ev[0] == 0xca) {
        if (ev[1] == 0xe5f) StrCopyN((char*)0x6d1128, F<const char*>(F<uint8_t*>((uint8_t*)ev, 0x2c), 0xba), 0x1b);
        else if (ev[1] == 0xe60) StrCopyN((char*)0x6d1228, F<const char*>(F<uint8_t*>((uint8_t*)ev, 0x2c), 0xba), 0x1b);
    } else if (ev[0] == 0xcb && ev[1] == 0x3f3) {
        StrCopyN((char*)0x6d1128, F<const char*>(Widget_FindChild(w, 0, 0xe5f), 0xba), 0x1b);
        StrCopyN((char*)0x6d1228, F<const char*>(Widget_FindChild(w, 0, 0xe60), 0xba), 0x1b);
    }
    return 1;
}
RT3_REPLACE(0x4ed4c0, NameFields_CopyCb);

void __fastcall Dialog_ClearChildrenE61(uint8_t* w, int, int32_t) { Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, 0xe61, 0xe6a, 0); }
RT3_REPLACE(0x4ed570, Dialog_ClearChildrenE61);

// 0x4ed590. Shows the name in both fields and stores it when it differs.
RT3_NOINLINE void __fastcall NameDialog_ShowName(uint8_t* d, int, const char* name) {
    SetChildText(d, 0xe48, name);
    SetChildText(d, 0xe5d, name);
    if (Stricmp((const char*)Options() + 0x1ef, name)) Copy((char*)Options() + 0x1ef, name);
}
RT3_REPLACE(0x4ed590, NameDialog_ShowName);

// 0x4ed670. Refreshes the game list window w: requests the list and posts each
// buffered line as event 0x88 / 0xe57; the number posted (0 while busy).
RT3_NOINLINE int32_t __fastcall GameList_Refresh(uint8_t* w) {
    int32_t n = 0;
    if (g_6cd920) return 0;
    Lobby_ListGames(g_session);
    for (const char* line; (line = Obj_NextBufferedLine(hook::As<void*>(g_session))) != nullptr;) {
        ++n;
        Window_SendEvent5(w, 0, 0x88, 0xe57, (uint32_t)(uintptr_t)line, 0, 0);
    }
    return n;
}
RT3_REPLACE(0x4ed670, GameList_Refresh);

// 0x4ed6d0 / 0x4ed730. Show a new-game setting (+0x77 / +0x7b) in field 0xe2f (style
// 8) / 0xe36 (style 1) of window w (event 0x6f).
RT3_NOINLINE void __fastcall NewGameDialog_ShowSetting77(uint8_t* w) {
    char num[0x24];
    Num_Format(num, 8, (double)F<uint16_t>(g_newGame, 0x77), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0xe2f, (uint32_t)(uintptr_t)num, 0, 0);
}
RT3_REPLACE(0x4ed6d0, NewGameDialog_ShowSetting77);

RT3_NOINLINE void __fastcall NewGameDialog_ShowSetting7b(uint8_t* w) {
    char num[0x1f8];
    Num_Format(num, 1, (double)F<uint16_t>(g_newGame, 0x7b), 10, 1, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0xe36, (uint32_t)(uintptr_t)num, 0, 0);
}
RT3_REPLACE(0x4ed730, NewGameDialog_ShowSetting7b);

// 0x4ed790. Highlights the new game's difficulty (+0x7d) among buttons 0xe26..0xe2a.
RT3_NOINLINE void __fastcall NewGameDialog_ShowDifficulty(uint8_t* w) {
    for (int32_t i = 0; i < 5; ++i)
        Window_SendEvent5(w, 0, g_newGame[0x7d] != i ? 0x87 : 0x65, 0xe26 + i, 0x20, 0, 0);
}
RT3_REPLACE(0x4ed790, NewGameDialog_ShowDifficulty);

// 0x4ed7e0 / 0x4ed870. Scalar deleting destructors of two worker thread classes.
uint8_t* __fastcall WorkerA_Delete(uint8_t* t, int, uint32_t flags) {
    Thread_Destruct(t);
    if (flags & 1) ((void(__cdecl*)(void*))0x53b080)(t);
    return t;
}
RT3_REPLACE(0x4ed7e0, WorkerA_Delete);

// 0x4ed800. Clears the network session object; returns it.
RT3_NOINLINE uint8_t* __fastcall Session_Init(uint8_t* s) {
    static const uint32_t kZero[] = { 0, 4, 8, 0xc, 0x8f18, 0x8f20, 0x8f24, 0x8f28, 0x8f2c, 0x8f30, 0x8f3c,
                                      0x8f40, 0x8f44, 0x9050, 0x9054, 0x985c };
    for (uint32_t off : kZero) F<int32_t>(s, off) = 0;
    F<int32_t>(s, 0x9860) = -1;
    F<int32_t>(s, 0x9878) = 0;
    F<int32_t>(s, 0x9884) = 0;
    return s;
}
RT3_REPLACE(0x4ed800, Session_Init);

uint8_t* __fastcall WorkerB_Delete(uint8_t* t, int, uint32_t flags) {
    Thread_Destruct(t);
    if (flags & 1) ((void(__cdecl*)(void*))0x53b080)(t);
    return t;
}
RT3_REPLACE(0x4ed870, WorkerB_Delete);

__declspec(naked) void Session_StartWorker() {
    __asm {
        push edi
        call Session_StartWorker_Impl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x4ed890, Session_StartWorker);

// 0x4ed940. Tidies the new-game AI setup (new game 0x6cec7c): the sixteen AI slot
// levels above 100 are cleared (one fewer AI, +0x7b, each); then for every player
// row of `o` (default 0x6d1270; +0x3ae rows of 9 bytes from +0x31b) beyond the
// configured AI count (+0x79) whose row is empty an AI slot is claimed (mode 1, then
// mode 0); the AI count is shown in field 0xe36 of window 0x6d1268.
RT3_NOINLINE void __fastcall NewGame_RefreshAiSlots(uint8_t* o) {
    if (!o) o = g_6d1270;
    int32_t left = F<uint16_t>(g_newGame, 0x79);
    for (int32_t k = 0xf; k >= 0; --k) {
        uint8_t* ng = g_newGame;
        if (ng[0x87 + k] > 100) {
            ng[0x87 + k] = 0;
            --F<uint16_t>(g_newGame, 0x7b);
        }
    }
    for (int32_t i = 0; i < F<int32_t>(o, 0x3ae); ++i, --left)
        if (left <= 0 && !o[0x31b + i * 9] && NewGameConfig_ClaimAiSlot(1)) NewGameConfig_ClaimAiSlot(0);
    char num[0x1f8];
    Num_Format(num, 1, (double)F<uint16_t>(g_newGame, 0x7b), 10, 1, 0, 0);
    Window_SendEvent5(g_6d1268, 0, 0x6f, 0xe36, (uint32_t)(uintptr_t)num, 0, 0);
}
RT3_REPLACE(0x4ed940, NewGame_RefreshAiSlots);

// 0x4eda30. New-game slot check (mode 1) / claim (other modes) for the setup
// screen 0x6d1270 against new game 0x6cec7c (+0x79 claimed, +0x7b AIs, levels
// +0x87.., rows of 9 bytes at +0x31b with the human flag at +0x31d).
// Mode 1: 7 without a setup screen; caps +0x3b6 at 4; 3 when no more players fit
// (+0x3b6 <= claimed, or rows <= claimed + empty rows), 0xff when a free or AI
// (level > 100) slot has a human row, else 3. Other modes take the first free slot
// with a human row (level 1, one more claimed) or else the first AI one (also one
// AI fewer, then refreshes the AI slots and pumps GameSpy): 0xff, or 0 if none.
RT3_NOINLINE int32_t __fastcall NewGame_ClaimSlot(int, int32_t mode) {
    uint8_t* ng = g_newGame;
    if (mode == 1) {
        uint8_t* p = g_6d1270;
        if (!p) return 7;
        if (F<int32_t>(p, 0x3b6) >= 4) F<int32_t>(p, 0x3b6) = 4;
        int32_t claimed = F<uint16_t>(ng, 0x79);
        if (F<int32_t>(p, 0x3b6) <= claimed) return 3;
        int32_t rows = F<int32_t>(p, 0x3ae), empty = 0;
        for (int32_t i = 0; i < rows; ++i)
            if (!p[0x31b + i * 9] && !p[0x31d + i * 9]) ++empty;
        if (rows <= claimed + empty) return 3;
        for (int32_t k = 0; k < 0x10; ++k) {
            uint8_t lvl = ng[0x87 + k];
            if (!lvl && p[0x31d + k * 9]) return 0xff;
            if (lvl > 100 && p[0x31d + k * 9]) return 0xff;
        }
        return 3;
    }
    uint8_t* rows = g_6d1270 + 0x31d;
    for (int32_t k = 0; k < 0x10; ++k)
        if (!ng[0x87 + k] && rows[k * 9]) {
            ng[0x87 + k] = 1;
            ++F<uint16_t>(g_newGame, 0x79);
            return 0xff;
        }
    for (int32_t k = 0; k < 0x10; ++k)
        if (ng[0x87 + k] > 100 && rows[k * 9]) {
            ng[0x87 + k] = 1;
            ++F<uint16_t>(g_newGame, 0x79);
            --F<uint16_t>(g_newGame, 0x7b);
            NewGame_RefreshAiSlots(nullptr);
            GameSpy_Pump();
            return 0xff;
        }
    return 0;
}
RT3_REPLACE(0x4eda30, NewGame_ClaimSlot);

// 0x4edbb0. With the new-game window open (0x6d1268): when more AI slots are in
// use (levels 1..99 at +0x87..) than the network allows (0x6d4100), frees the last
// one in use (one fewer configured AI, +0x79) and refreshes the AI setup.
RT3_NOINLINE void NewGame_CapAiSlots() {
    if (!hook::As<void*>(g_6d1268)) return;
    int32_t limit = g_6d4100;
    uint8_t* ng = g_newGame;
    int32_t used = 0;
    for (int32_t k = 0xf; k >= 0; --k)
        if (ng[0x87 + k] && ng[0x87 + k] < 100) ++used;
    if (used <= limit) return;
    for (int32_t k = 0xf; k >= 0; --k) {
        if (!ng[0x87 + k] || ng[0x87 + k] >= 100) continue;
        ng[0x87 + k] = 0;
        --F<uint16_t>(g_newGame, 0x79);
        NewGame_RefreshAiSlots(nullptr);
        return;
    }
}
RT3_REPLACE(0x4edbb0, NewGame_CapAiSlots);

// 0x4edc40. Starts a fresh network session (0x6cd8d8) from window w: the old one
// is shut down, the status text (child 0xe47) and 0x6d11a8 are cleared and the
// worker starts in mode 4 (host) or 1.
RT3_NOINLINE void __fastcall Session_Restart(uint8_t* w, int, int32_t host) {
    if (uint8_t* s = g_session) {
        Session_Shutdown(s);
        ExeDelete(s);
        g_session = nullptr;
    }
    uint8_t* s = (uint8_t*)ExeNew(0x9898);
    g_session = s ? Session_Init(s) : nullptr;
    if (uint8_t* c = Widget_FindChild(w, 0, 0xe47))
        if (char* text = F<char*>(c, 0xba)) *text = 0;
    g_6d11a8 = 0;
    Session_StartWorker_Impl(host ? 4 : 1);
}
RT3_REPLACE(0x4edc40, Session_Restart);

// Texture: image slots +0xa0 (8 x 50-byte images: w +8, h +0xc, format +0x14/+0x18,
// in video memory +0x22, 32-bit pixels +0x26, 16-bit pixels +0x2a, extra +0x2e),
// slot used flags +0x98, count +4.

// 0x4edce0. Passes the name to the open name dialog (0x6d1268), if any.
RT3_NOINLINE void __fastcall NameDialog_SetNameIfOpen(const char* name) {
    if (uint8_t* d = g_6d1268) NameDialog_SetName(d, 0, name);
}
RT3_REPLACE(0x4edce0, NameDialog_SetNameIfOpen);

// 0x4edd00. Adds a button for player key to the lobby panel 0x6d1268 (id 0xe61 +
// slot, parent 0xe14) at the right edge of (x, y, w): labelled with string 0xe66
// when the player's flags (+0x5c) have bit 0, else 0xe67. The panel's list 0xe14
// is then refreshed (event 0x86 on pages 0xe15 / 0xe16, else 0x6d).
void __fastcall Lobby_AddPlayerButton(int32_t, int32_t slot, int32_t key, float x, float y, float w, float) {
    if (!hook::As<void*>(g_6d40d0)) return;
    uint8_t* e = Textures_Find(key);
    if (!e) return;
    uint32_t flags = F<uint32_t>(e, 0x5c);
    Net_UnlockQueueArg(key);
    uint8_t d[0xa4];
    memset(d, 0, 0xa1);
    F<float>(d, 7) = (float)((double)x + w - (double)g_5d0100);
    F<float>(d, 0xb) = (float)((double)y + (double)g_5d0188);
    F<uint32_t>(d, 0xf) = 0x3c4ccccd;
    F<uint32_t>(d, 0x13) = 0x3c888889;
    d[0x29] = 0x20;
    F<int32_t>(d, 0x67) = 0x16;
    F<int32_t>(d, 0x6b) = 0x17;
    F<int32_t>(d, 0x4b) = key;
    F<uint16_t>(d, 0) = (uint16_t)(slot + 0xe61);
    F<uint16_t>(d, 5) = 0xe14;
    String_Assign(&F<char*>(d, 0x43), (const char*)0x5d1258);
    String_Assign(&F<char*>(d, 0x3f), GetString((flags & 1) ? 0xe66 : 0xe67));
    F<int32_t>(d, 0x89) = (flags & 1) ? 1 : 0;
    uint8_t* b = (uint8_t*)ExeNew(0xe8);
    if (b) b = Button_Construct(b, 0, d);
    Window_AddChild(g_6d1268, 0, b, 0);
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(d, 0x43));
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(d, 0x3f));
    uint8_t* panel = g_6d1268;
    int32_t page = F<int32_t>(panel, 0x78);
    Window_SendEvent5(panel, 0, page == 0xe15 || page == 0xe16 ? 0x86 : 0x6d, 0xe14, 3, 0, 0);
}
RT3_REPLACE(0x4edd00, Lobby_AddPlayerButton);

// 0x4ede80. Lobby: draws player `key`'s label (name, a star when marked
// +0x50, the ping +0x54 in ms when exactly one of marked / showing pings
// (0x6d40dc) holds, and the version +0x268c) in window `id`'s font, inset
// by 0.018 on the left, while the player list (0x6d40d0) is up.
void __fastcall Lobby_DrawPlayerLabel(int32_t id, int32_t key, float x, float y, float w, float h, int32_t) {
    if (!hook::As<void*>(g_6d40d0)) return;
    uint8_t* win = Widget_FindChild(g_6d1268, 0, id);
    uint8_t* p = Textures_Find(key);
    if (!p) return;
    int32_t ms = F<int32_t>(p, 0x54);
    int32_t marked = F<int32_t>(p, 0x50);
    char name[0x104], ver[0x40], ping[0x40], text[0xc0];
    strcpy(name, (const char*)p + 8);
    float version = F<float>(p, 0x268c);
    Net_UnlockQueueArg(key);
    int32_t pings = g_6d40dc;
    int32_t form = marked ? (pings ? 1 : 2) : (pings ? 3 : 0);
    const char* v = Num_Format(ver, 3, (double)version, 10, 1, 0, 0);
    if (form == 0) FormatText(text, (const char*)0x5d12a0, name, v, 0, 0, 0, 0, 0, 0);
    else if (form == 1) FormatText(text, (const char*)0x5d1294, name, v, 0, 0, 0, 0, 0, 0);
    else {
        const char* m = Num_Format(ping, 1, (double)ms, 10, 1, 0, 0);
        FormatText(text, (const char*)(form == 2 ? 0x5d1280 : 0x5d1270), name, m, v, 0, 0, 0, 0, 0);
    }
    float tw = (float)((double)w - D(0x5d1268));
    float tx = (float)((double)x + D(0x5d1268));
    Font_DrawTextBox(F<uint8_t*>(win, 0x63), 0, text, tx, y, tw, h, 0xffffffff, 2, 0, 0, 1.0f);
}
RT3_REPLACE(0x4ede80, Lobby_DrawPlayerLabel);

// 0x4ee0e0. Multiplayer game dialog w: takes the player name from box 0xe5d
// (host page 0xe13) or 0xe48 into the options (+0x1ef, saved), prepares the
// two entry fields (0x6d1128 from string 0x2ce, 0x6d1228 empty) for message
// 0x5c87a8 with the field panel 0x4ed100, then starts the session worker (2).
RT3_NOINLINE void __fastcall MpDialog_Connect(uint8_t* w) {
    uint8_t* box = Widget_FindChild(w, 0, F<int32_t>(w, 0x7c) == 0xe13 ? 0xe5d : 0xe48);
    strcpy((char*)g_app + 0x1ef, F<const char*>(box, 0xba));
    Options_Save(g_app, 0, 0);
    char line[0x80];
    FormatText(line, GetString(0x2ce), F<const char*>(box, 0xba), 0, 0, 0, 0, 0, 0, 0);
    strcpy((char*)0x6d1128, line);
    g_6d1228 = 0;
    Dialog_RunMessage((const char*)0x5c87a8, 1, 0xbf800000, 0xbf800000, 0x4ed4c0,
                      (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4ed100, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    __asm {
        push edi
        mov edi, 2
        mov eax, 0x4ed890
        call eax
        pop edi
    }
}
RT3_REPLACE(0x4ee0e0, MpDialog_Connect);

// 0x4ee1d0. Multiplayer dialog w: join the game at `address` (null: the typed
// address box 0xe47, or when blank the selected lobby entry of list 0xe56 /
// 0xe22, message 0x2cc without one) into 0x6d11a8, saving the player name
// (box 0xe5d / 0xe48, options +0x1ef) and the password (0x6d1228 from
// 0x6ae3cc when 0x6cd920 and at most one 0x6ae3c8); starts the session worker
// (3) for addresses longer than 5 characters, else clears the address.
RT3_NOINLINE void __fastcall MpDialog_Join(uint8_t* w, int, const char* address) {
    uint8_t* name = Widget_FindChild(w, 0, F<int32_t>(w, 0x7c) == 0xe13 ? 0xe5d : 0xe48);
    strcpy((char*)g_app + 0x1ef, F<const char*>(name, 0xba));
    Options_Save(g_app, 0, 0);
    char* target = (char*)0x6d11a8;
    if (!address) {
        uint8_t* box = Widget_FindChild(w, 0, 0xe47);
        if (Str_IsBlank(F<const char*>(box, 0xba))) {
            uint8_t* list = Widget_FindChild(w, 0, F<int32_t>(w, 0x7c) == 0xe13 ? 0xe56 : 0xe22);
            if (F<int32_t>(list, 0x1d6) < 0) {
                Message_Show(GetString(0x2cc), 1, 0);
                return;
            }
            uint8_t* entries = Lobby_FreshList(g_session);
            strcpy(target, (const char*)entries + F<int32_t>(list, 0x1d6) * 0x11c + 0x84);
        } else
            strcpy(target, F<const char*>(box, 0xba));
    } else
        strcpy(target, address);
    if (hook::As<uint32_t>(g_6cd920)) {
        if (g_6ae3c8 <= 1) {
            if (g_6ae3cc) {
                memset((void*)0x6d1228, 0, 0x40);
                ((char*(__cdecl*)(char*, const char*, size_t))0x5a18a0)((char*)0x6d1228, (const char*)0x6ae3cc, 0x3f);
            } else
                g_6d1228 = 0;
        }
    } else
        g_6d1228 = 0;
    if (strlen(target) > 5) {
        Session_StartWorkerMode(3);
        return;
    }
    *F<char*>(Widget_FindChild(w, 0, 0xe47), 0xba) = 0;
    target[0] = 0;
}
RT3_REPLACE(0x4ee1d0, MpDialog_Join);

// 0x4ee3a0. Options screen destructor: saves the options and frees its globals.
void __fastcall OptionsScreen_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d12ac;
    hook::As<int32_t>(g_6d1268) = 0;
    F<uint32_t>(Options(), 0x30d) = hook::As<uint32_t>(g_6226d0);
    Options_Save(Options(), 0, 0);
    if (void* p = hook::As<void*>(g_6d126c)) {
        Obj_FreeBuffer8(p);
        ExeDelete(p);
        hook::As<void*>(g_6d126c) = nullptr;
    }
    if (void* p = hook::As<void*>(g_6d1270)) {
        ExeDelete(p);
        hook::As<void*>(g_6d1270) = nullptr;
    }
    if (void* p = hook::As<void*>(g_6d1294)) {
        ((void(__thiscall*)(void*, int32_t))(*(void***)p)[0])(p, 1);
        hook::As<void*>(g_6d1294) = nullptr;
    }
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4ee3a0, OptionsScreen_Destruct);

// 0x4ee430. Mode 1 disables controls 0xe12 / 0xe18 (0x86) and lights 0xe73 if
// 0x6d40dc; otherwise enables them (0x6d) and dims 0xe73 (0x87).
RT3_NOINLINE void __fastcall Window_SetMode1(uint8_t* w, int, int32_t mode) {
    if (mode == 1) {
        Window_SendEvent5(w, 0, 0x86, 0xe12, 3, 0, 0);
        Window_SendEvent5(w, 0, 0x86, 0xe18, 3, 0, 0);
        if (g_6d40dc) Window_SendEvent5(w, 0, 0x65, 0xe73, 3, 0, 0);
        return;
    }
    Window_SendEvent5(w, 0, 0x6d, 0xe12, 3, 0, 0);
    Window_SendEvent5(w, 0, 0x6d, 0xe18, 3, 0, 0);
    Window_SendEvent5(w, 0, 0x87, 0xe73, 3, 0, 0);
}
RT3_REPLACE(0x4ee430, Window_SetMode1);

// 0x4ee4c0. Sends the empty message 0x69 to everyone (reliable from the host).
RT3_NOINLINE void Net_SendReady() {
    Net_Send(g_session, 0, nullptr, 0x69, 0, nullptr, g_6d40dc ? 3 : 1, 0);
}
RT3_REPLACE(0x4ee4c0, Net_SendReady);

// 0x4ee500. Whether every network player has flag 1 (AL only).
RT3_NOINLINE uint8_t Net_AllPlayersFlag1() {
    uint8_t ok = 1;
    for (uint8_t* e = PlayerHead(); e; e = *(uint8_t**)e)
        if (!(e[0x5c] & 1)) ok = 0;
    Net_UnlockQueue();
    return ok;
}
RT3_REPLACE(0x4ee500, Net_AllPlayersFlag1);

// 0x4ee540. Refills the lobby's player list (0xe57 in the game browser w+0x7c ==
// 0xe13, else 0xe35; guarded by 0x6d1290): the browser without a session asks
// GameSpy (GameList_Refresh), otherwise the connected players are listed. Empty
// without a session it restarts it; outside the browser it then sends this
// player's ready bit (engine entry +0x5c bit 0) as message 9.
RT3_NOINLINE void __fastcall Lobby_RefreshPlayers(uint8_t* w) {
    if (g_6d1290) return;
    g_6d1290 = 1;
    int32_t list = F<int32_t>(w, 0x7c) == 0xe13 ? 0xe57 : 0xe35;
    int32_t n = 0;
    if (F<int32_t>(Widget_FindChild(w, 0, list), 0x1d6) > -1) TreeView_SelectedData(Widget_FindChild(w, 0, list));
    if (F<int32_t>(Widget_FindChild(w, 0, list), 0x1d2)) Window_SendEvent5(w, 0, 0x8b, (uint32_t)list, 0, 0, 0);
    if (list == 0xe57 && !g_6cd920) {
        n = GameList_Refresh(w);
    } else {
        uint32_t names[0x10];
        memset(names, 0, sizeof names);
        for (uint8_t* p = *(uint8_t**)(uintptr_t)Net_LockQueue(); p; p = *(uint8_t**)p) names[n++] = F<uint32_t>(p, 4);
        Net_UnlockQueue();
        for (int32_t i = 0; i < n; ++i) Window_SendEvent5(w, 0, 0x88, (uint32_t)list, 0, names[i], 0);
    }
    if (!n && !g_6cd920) {
        Session_Restart(w, 0, F<int32_t>(g_session, 0x985c));
        g_6d1290 = 0;
        return;
    }
    if (F<int32_t>(w, 0x7c) != 0xe13) {
        uint8_t ready = F<uint8_t>(Engine_FirstListEntry(), 0x5c) & 1;
        Net_Send(g_session, 0, nullptr, 9, 1, &ready, 3, 0);
    }
    g_6d1290 = 0;
}
RT3_REPLACE(0x4ee540, Lobby_RefreshPlayers);

// 0x4ee6a0. Refills window w's game list (control 0xe56 on page 0xe13, else
// 0xe22) from a fresh server list, up to the first empty slot, keeping the
// selection on the game of the same name. Skipped while connected (0x6cd920),
// while it runs (0x6d1290) or when the status text (0xe47) isn't blank.
RT3_NOINLINE void __fastcall GameList_Fill(uint8_t* w) {
    if (g_6cd920 || hook::As<char>(g_6d1290)) return;
    hook::As<char>(g_6d1290) = 1;
    uint32_t list = F<int32_t>(w, 0x7c) == 0xe13 ? 0xe56 : 0xe22;
    char name[0x100];
    memset(name, 0, sizeof name);
    int32_t sel = -1;
    if (F<int32_t>(Widget_FindChild(w, 0, list), 0x1d6) > -1) {
        int32_t i = F<int32_t>(Widget_FindChild(w, 0, list), 0x1d6);
        strcpy(name, (const char*)(uintptr_t)TreeView_NodeValue(Widget_FindChild(w, 0, list), 0, i));
    }
    int32_t count = 0;
    Window_SendEvent5(w, 0, 0x8b, list, 0, 0, 0);
    uint8_t* status = Widget_FindChild(w, 0, 0xe47);
    uint8_t* games = Lobby_FreshList(g_session);
    if (Str_IsBlank(F<const char*>(status, 0xba))) {
        uint8_t* g = games + 4;
        for (int32_t k = 0; k < 0x80; ++k, g += 0x11c) {
            if (!((int(__cdecl*)(const char*, const char*))0x5a57cf)((const char*)g, name)) sel = count;
            if (!F<int32_t>(g, 0x100)) break;
            Window_SendEvent5(w, 0, 0x88, list, (uint32_t)(uintptr_t)g, 0, 0);
            ++count;
            if (sel != -1) {
                Window_SendEvent5(w, 0, 0x66, list, (uint32_t)sel, 0, 0);
                sel = -1;
            }
        }
    }
    hook::As<char>(g_6d1290) = 0;
}
RT3_REPLACE(0x4ee6a0, GameList_Fill);

// 0x4ee810. Multiplayer lobby chat: appends `text` to the chat list (0xe32, or
// 0xe5c in the in-game chat window 0xe13) of window 0x6d1268, wrapped to the
// list's width (one line only with 0x6d4034), lines without a '>' (system
// messages) in green, and scrolls to the end; null clears the list.
RT3_NOINLINE void __fastcall Lobby_AddChatText(const char* text) {
    uint8_t* win = g_6d1268;
    if (!win) return;
    int32_t id = F<int32_t>(win, 0x7c) == 0xe13 ? 0xe5c : 0xe32;
    if (!text) {
        uint8_t* lb = Widget_FindChild(win, 0, id);
        if (lb && F<int32_t>(lb, 0x1d2)) Window_SendEvent5(g_6d1268, 0, 0x8b, id, 0, 0, 0);
        return;
    }
    uint32_t color = 0;
    if (!strchr(text, '>')) color = 0xff00ff00;
    uint8_t* lb = Widget_FindChild(g_6d1268, 0, id);
    if (!lb) return;
    int32_t n = Font_CountLines(F<uint8_t*>(lb, 0x63), 0, text, F<float>(lb, 0xae), nullptr);
    if (g_6d4034 && n >= 1) n = 1;
    char line[0x400];
    for (int32_t i = 1; i <= n; ++i) {
        Font_GetLine(F<uint8_t*>(lb, 0x63), 0, text, F<float>(lb, 0xae), i, line);
        Window_SendEvent5(g_6d1268, 0, 0x88, id, (uint32_t)(uintptr_t)line, 0, 0);
        if (color) F<uint32_t>(F<uint8_t*>(lb, 0x176), F<int32_t>(lb, 0x1d2) * 0x20 - 0x14) = color;
    }
    Window_SendEvent5(g_6d1268, 0, 0x66, id, (uint32_t)(F<int32_t>(lb, 0x1d2) - 1), 0, 0);
}
RT3_REPLACE(0x4ee810, Lobby_AddChatText);

// 0x4ee950. Multiplayer setup counterpart of ScenarioPanel_Update: shows
// scenario `path` (title `name`) in dialog d unless already shown and not
// `force`; sets 0x6cec74 +0x6c when the path lacks string
// 0x5c9cf8, loads its header into 0x6d1270 (players +0x3b6 capped at 4,
// minimum +0x3b2 = 1; a GameSpy host keeps the header's start year +0xc),
// fills in year, title, description, size and the players line (string
// 0xe6c) and the preview, then pumps GameSpy and refreshes the dialog.
RT3_NOINLINE void __fastcall MpScenarioPanel_Update(uint8_t* d, int, const char* path, const char* name, int32_t force) {
    uint8_t* g = g_newGame;
    if (path) {
        if (!force && g[0x10] && !Stricmp((const char*)g + 0x11, path)) return;
        g_newGame[0x10] = 1;
        StrCopyN((char*)g_newGame + 0x11, path, 0x32);
        StrCopyN((char*)g_newGame + 0x44, name, 0x32);
    } else {
        if (!force && g[0x10]) return;
        g[0x10] = 0;
    }
    if (!g_newGame[0x10]) {
        static const uint32_t ids[] = { 0xe23, 0xe2e, 0xe4b };
        for (uint32_t id : ids) Window_SendEvent5(d, 0, 0x6f, id, 0x5c87a8, 0, 0);
        return;
    }
    const char* hit = ((const char*(__cdecl*)(const char*, const char*))0x5a1ff0)((const char*)g_newGame + 0x11, (const char*)0x5c9cf8);
    F<int32_t>(g_app, 0x6c) = !hit;
    Block_Load((const char*)g_newGame + 0x11, g_6d1270, 0);
    uint8_t* info = g_6d1270;
    int32_t players = F<int32_t>(info, 0x3b6);
    if (players >= 4) players = 4;
    F<int32_t>(info, 0x3b6) = players;
    F<int32_t>(g_6d1270, 0x3b2) = 1;
    if (g_6d40dc && g_6226d4)
        F<uint16_t>(g_newGame, 0x77) = F<uint16_t>(g_6d1270, 0xc);
    Obj_ApplySetup(g_newGame, 0, g_6d1270);
    char yearText[0x20], aText[0x20], bText[0x20], line[0x200];
    Num_Format(yearText, 8, (double)F<uint16_t>(g_newGame, 0x77), 10, 1, 0, 0);
    Window_SendEvent5(d, 0, 0x6f, 0xd4f, (uint32_t)(uintptr_t)yearText, 0, 0);
    Window_SendEvent5(d, 0, 0x6f, 0xe23, (uint32_t)(uintptr_t)(g_newGame + 0x44), 0, 0);
    Window_SendEvent5(d, 0, 0x6f, 0xe2e, (uint32_t)(uintptr_t)(g_6d1270 + 0x70), 0, 0);
    const char* ws = Num_Format(aText, 8, (double)F<int32_t>(g_6d1270, 5), 10, 1, 0, 0);
    const char* hs = Num_Format(bText, 8, (double)F<int32_t>(g_6d1270, 1), 10, 1, 0, 0);
    FormatText(line, GetString(0x2b2), hs, ws, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    info = g_6d1270;
    if (F<int32_t>(info, 5) * F<int32_t>(info, 1) > 0x51000) strcat(line, GetString(0xe0c));
    Window_SendEvent5(d, 0, 0x6f, 0xe4b, (uint32_t)(uintptr_t)line, 0, 0);
    const char* maxs = Num_Format(aText, 1, (double)F<int32_t>(g_6d1270, 0x3b6), 10, 1, 0, 0);
    const char* mins = Num_Format(bText, 1, (double)F<int32_t>(g_6d1270, 0x3b2), 10, 1, 0, 0);
    FormatText(line, GetString(0xe6c), mins, maxs, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    Window_SendEvent5(d, 0, 0x6f, 0xe50, (uint32_t)(uintptr_t)line, 0, 0);
    Widget_SetPicture(Widget_FindChild(d, 0, 0xe4a), 0, g_6d1270 + 0x3c2, 0x100, 0x100);
    if (g_6d40dc) GameSpy_Pump();
    NewGameDialog_ShowSetting77(d);
    Window_SetMode1(d, 0, g_6ce9bc);
}
RT3_REPLACE(0x4ee950, MpScenarioPanel_Update);

// 0x4eed30. Fills multiplayer setup dialog d's scenario list 0xe30 from the
// scenario table 0x6d126c (names +0x12d of 0x25a-byte entries), selects the
// current one (0x6cec7c +0x44) and shows it.
void __fastcall MpScenarioList_Fill(uint8_t* d) {
    Window_SendEvent5(d, 0, 0x8b, 0xe30, 0, 0, 0);
    for (int32_t i = 0; i < F<int32_t>(g_6d126c, 0); ++i) {
        Window_SendEvent5(d, 0, 0x88, 0xe30, (uint32_t)(uintptr_t)(F<uint8_t*>(g_6d126c, 8) + i * 0x25a + 0x12d), 0, 0);
        if (!Stricmp((const char*)F<uint8_t*>(g_6d126c, 8) + i * 0x25a + 0x12d, (const char*)g_newGame + 0x44))
            Window_SendEvent5(d, 0, 0x66, 0xe30, (uint32_t)i, 0, 0);
    }
    uint8_t* g = g_newGame;
    if (g[0x10]) MpScenarioPanel_Update(d, 0, (const char*)g + 0x11, (const char*)g + 0x44, 1);
    else MpScenarioPanel_Update(d, 0, nullptr, nullptr, 1);
}
RT3_REPLACE(0x4eed30, MpScenarioList_Fill);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4eee00. Select the multiplayer scenario source, update the controls and
// initialize/copy the saved-game choice. Preserve the original null-list fault.
RT3_NOINLINE void __fastcall MpScenario_SelectSource(uint8_t* dialog, int, int32_t choice) {
    F_dialog_flow<int32_t>(dialog, 0x78) = choice;
    for (int32_t id = 0xe15; id <= 0xe16; ++id) Event(dialog, id == choice ? 0x6d : 0x86, id, 3);
    Event(dialog, choice == -1 && F_dialog_flow<int32_t>(dialog, 0x7c) == 0xe12 ? 0x6d : 0x86, 0xe18, 3);
    if (choice == 0xe15) Event(dialog, 0x6d, 0xe17, 3);
    else if (choice == -1 && F_dialog_flow<int32_t>(dialog, 0x7c) != 0xe12) {
        Event(dialog, 0x86, 0xe17, 3);
        if (g_6ce9bc == 1) Event(dialog, 0x86, 0xe18, 3);
    }
    Event(dialog, choice == 0xe15 ? 0x65 : 0x87, 0xe24, 0x20);
    Event(dialog, choice == 0xe16 ? 0x65 : 0x87, 0xe25, 0x20);
    Event(dialog, choice == -1 && F_dialog_flow<int32_t>(dialog, 0x7c) == 0xe12 ? 0x6d : 0x86, 0xe14, 3);
    if (choice != 0xe15) {
        if (g_6d1291) {
            uint8_t* options = g_newGame;
            hook::Orig<void(__fastcall*)(uint8_t*, int, const char*, const char*, int32_t)>(0x4ee950)(
                dialog, 0, options[0x10] ? (const char*)options + 0x11 : nullptr,
                options[0x10] ? (const char*)options + 0x44 : nullptr, options[0x10] ? 1 : 0);
        }
        return;
    }
    char text[32];
    hook::Orig<char*(__fastcall*)(char*, int32_t, double, int32_t, int32_t, int32_t, int32_t)>(0x51b700)(
        text, 8, (double)F_dialog_flow<uint16_t>(hook::As<void*>(g_newGame), 0x77), 10, 1, 0, 0);
    Event(dialog, 0x6f, 0xe2f, (uint32_t)(uintptr_t)text);
    F_dialog_flow<int32_t>(hook::As<void*>(g_app), 0x6c) = 1;
    hook::Orig<void(__fastcall*)(void*, int, int32_t)>(0x482e50)(hook::As<void*>(g_app), 0, 0);
    uint8_t* list = g_6d126c;
    bool copy;
    if (!list) {
        uint8_t* mem = hook::Orig<uint8_t*(__cdecl*)(uint32_t)>(0x53b070)(12);
        list = mem ? hook::Orig<uint8_t*(__fastcall*)(uint8_t*, int, int32_t)>(0x4336a0)(mem, 0, 7) : nullptr;
        // The native CMP dereferences the result before publishing it.
        int32_t count = *(volatile int32_t*)list;
        g_6d126c = list;
        copy = count > 0;
    } else copy = F_dialog_flow<int32_t>(list, 0) > 0 && g_6d40dc;
    if (copy) {
        g_newGame[0x10] = 1;
        hook::Orig<void(__fastcall*)(char*, const char*, uint32_t)>(0x51d8a0)(
            (char*)hook::As<void*>(g_newGame) + 0x11, F_dialog_flow<const char*>(list, 8), 50);
        list = g_6d126c;
        hook::Orig<void(__fastcall*)(char*, const char*, uint32_t)>(0x51d8a0)(
            (char*)hook::As<void*>(g_newGame) + 0x44, F_dialog_flow<const char*>(list, 8) + 0x12d, 50);
    }
    if (!g_6d1291) hook::Orig<void(__fastcall*)(uint8_t*)>(0x4eed30)(dialog);
}
RT3_REPLACE(0x4eee00, MpScenario_SelectSource);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x4ef090. Select the multiplayer dialog mode, install scenario callbacks
// where required, reset the source selector and remember the current clock.
RT3_NOINLINE void __fastcall MpDialog_SelectMode(uint8_t* dialog, int, int32_t choice) {
    F_dialog_selection<int32_t>(dialog, 0x7c) = choice;
    for (int32_t id = 0xe10; id <= 0xe13; ++id) Event_dialog_selection(dialog, id == choice ? 0x6d : 0x86, id, 3);
    uint32_t state = 0x86;
    if (choice == 0xe11) {
        char buffer[128];
        char* address = hook::Orig<char*(__fastcall*)(void*, int, char*)>(0x5216f0)(hook::As<void*>(g_6d40d0), 0, buffer);
        Event_dialog_selection(dialog, 0x6f, 0xe46, (uint32_t)(uintptr_t)address);
    } else if (choice == 0xe12) {
        hook::Orig<void(__fastcall*)(uint8_t*)>(0x4ed730)(dialog);
        hook::Orig<void(__fastcall*)(uint8_t*)>(0x4ed790)(dialog);
        Event_dialog_selection(dialog, 0x7a, 0xe35, 0x4ede80);
        Event_dialog_selection(dialog, 0x7e, 0xe35, 0x4edd00);
        Event_dialog_selection(dialog, 0x7f, 0xe35, 0x4ed570);
        hook::Orig<void(__fastcall*)(uint8_t*, int, int32_t)>(0x4eee00)(dialog, 0, 0xe15);
        if (F_dialog_selection<int32_t>(hook::As<void*>(g_session), 0x985c)) state = 0x6d;
    } else if (choice == 0xe10) {
        if (!hook::Orig<uint8_t(__cdecl*)()>(0x51b6f0)()) Event_dialog_selection(dialog, 0x87, 0xe4c, 3);
    } else if (choice == 0xe13) state = 0x6d;
    Event_dialog_selection(dialog, state, 0xe55, 3);
    hook::Orig<void(__fastcall*)(uint8_t*, int, int32_t)>(0x4eee00)(dialog, 0, -1);
    uint32_t clock = hook::Orig<uint32_t(__cdecl*)()>(0x51d890)();
    F_dialog_selection<uint32_t>(dialog, 0x80) = clock;
}
RT3_REPLACE(0x4ef090, MpDialog_SelectMode);

// 0x4ef200. Reports a pending multiplayer session event (0x6d1278 set; the
// event 0x6d1280, the join result 0x6d1284, the lobby result 0x6d1288) with
// a message box: lost connections (with the socket / network error text)
// end the session and return to the menu (input event 0xcc), a taken player
// name on a GameSpy (0x6cd920) join of fewer than 3 tries (0x6ae3c8)
// proposes "RT3Player<n>" instead (0x6ae3c4 = 2), a lobby result 8 / 5 / 6
// goes back to the mode choice (0xe13). Returns 1, or 0 with nothing pending.
RT3_NOINLINE int32_t MpSession_ReportEvent() {
    if (g_6d1278 != 1) return 0;
    g_6d1278 = 0;
    char text[0x100], name[0x100];
    auto show = [&](const char* t) {
        Dialog_RunMessage(t, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    };
    auto say = [&](int32_t id) {
        strcpy(text, GetString(id));
        show(text);
    };
    auto endSession = []() {
        if (uint8_t* s = g_session) {
            Session_Shutdown(s);
            ((void(__cdecl*)(void*))0x53b080)(s);
            g_session = nullptr;
        }
    };
    auto toMenu = [&]() {
        show(text);
        Game_PostInputEvent(0xcc, 0, 2, 0, 0);
    };
    // A random name for the retry (the rand of 0x624324).
    auto randomName = [&]() {
        const uint32_t x = g_lcg * 0x19660d + 0x3c6ef35f;
        g_lcg = x;
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(name, (const char*)0x5ce1e4, (int32_t)(x & 0x7fff) % 1000);
    };
    auto gameSpyRetry = []() { return g_6cd920 && g_6ae3c8 < 3; };
    switch (g_6d1280) {
    case 1: {
        strcpy(text, GetString(0xe4f));
        const char* err = nullptr;
        if (g_6d4118)
            err = Net_SocketErrorText();
        else if (g_6d411c)
            err = Net_ErrorText(g_6d411c);
        if (err) {
            const char* what = GetString(0xe4f);
            FormatText(text, GetString(0xfe4), what, err, 0, 0, 0, 0, 0, 0);
        }
        endSession();
        toMenu();
        return 1;
    }
    case 5:
        g_6cd920 = 0;
        endSession();
        strcpy(text, GetString(0xe50));
        toMenu();
        return 1;
    case 0xa:
        if (g_6d1292) show(GetString(0xe51));
        return 1;
    case 0xb:
        if (!gameSpyRetry()) {
            show(GetString(0xe52));
            return 1;
        }
        randomName();
        NameDialog_ShowName(g_6d1268, 0, name);
        g_6ae3c4 = 2;
        return 1;
    case 0xc:
        show(GetString(0xe53));
        return 1;
    case 0xe:
        show(GetString(0xfb6));
        return 1;
    case 2:
        if (!gameSpyRetry())
            show(GetString(0x2cd));
        else
            g_6ae3c4 = 2;
        return 1;
    case 7:
        say(0xe54);
        Session_Restart(g_6d1268, 0, F<int32_t>(g_session, 0x985c));
        return 1;
    case 3: {
        Session_Restart(g_6d1268, 0, F<int32_t>(g_session, 0x985c));
        static const int32_t kJoin[9] = { 0xe55, 0xe56, 0xe57, 0xe58, 0xe92, 0xe92, 0xe59, 0xe92, 0xf53 };
        const int32_t r = g_6d1284;
        if (r == 4 && gameSpyRetry()) {
            randomName();
            NameDialog_ShowName(g_6d1268, 0, name);
            g_6ae3c4 = 2;
            return 1;
        }
        say((uint32_t)(r - 1) <= 8 ? kJoin[r - 1] : 0xe59);
        return 1;
    }
    case 6: {
        const int32_t r = g_6d1288;
        if (r == 8) {
            strcpy(text, GetString(0xe5a));
        } else if (r == 5) {
            if (gameSpyRetry()) {
                // QUIRK: the random name is made but the dialog gets 0x6ae2c4.
                randomName();
                NameDialog_ShowName(g_6d1268, 0, (const char*)0x6ae2c4);
                g_6d1288 = 0;
                g_6ae3c4 = 2;
                return 1;
            }
            strcpy(text, GetString(0xe5b));
        } else if (r == 6) {
            strcpy(text, GetString(0xe5c));
        } else {
            endSession();
            strcpy(text, GetString(0xe5d));
            toMenu();
            g_6d1288 = 0;
            return 1;
        }
        show(text);
        MpDialog_SelectMode(g_6d1268, 0, 0xe13);
        g_6cd8dc = 1;
        g_6d1288 = 0;
        return 1;
    }
    case 8:
        say(0xe5e);
        return 1;
    case 0xd:
        say(0xe5f);
        return 1;
    default:
        return 1;
    }
}
RT3_REPLACE(0x4ef200, MpSession_ReportEvent);

// 0x4ef960 (vtable 0x5d1250, the session worker thread; see
// Session_StartWorker). Runs step 0x6d127c and reports through 0x6d1278 (done)
// / 0x6d1280 (result), polling (SleepEx) for the network thread's answers:
// 1 opens the session (dialog mode 0xe11), 4 opens it with the player name
// (0xe13; result 6 when 0x6d1288 is set, 5 on failure), 6 joins a named peer,
// 2 hosts (mode 0xe12, message 0xe60), 3 joins (the connect state +0xc: 3
// announces our address and version (type 0x51) and waits for the host's
// answer, one retry on 3 / 7; else results 0xa / 0xb / 0xc / 0xe / 2), 5
// sends the scenario name (0x59) and waits (8 / 0xd: ready, type 0x11), else
// the scenario (type 2). Always ends with the arrow cursor, clearing the step
// and dropping the modal count. Returns 0x7f.
uint32_t __fastcall SessionWorker_Run(uint8_t* thr) {
    auto sleep = [] { hook::Global<uint32_t(__stdcall*)(uint32_t, int32_t)>(0x5c8064)(0, 0); };
    auto s = [] { return g_session; };
    auto name = [] { return (const char*)g_app + 0x1ef; };
    auto done = [](int32_t result) {
        g_6d1278 = 1;
        g_6d1280 = result;
    };
    auto selectMode = [](int32_t mode) {
        g_6d1291 = 1;
        MpDialog_SelectMode(g_6d1268, 0, mode);
        g_6d1291 = 0;
    };
    bool pump = false;
    switch (g_6d127c) {
    case 1:
        hook::Global<int32_t(__stdcall*)(void*, int32_t)>(0x5c8084)(F<void*>(thr, 4), 2);
        if (Session_Open(s(), 0, 0, nullptr)) {
            MpDialog_SelectMode(g_6d1268, 0, 0xe11);
            g_6cd8dc = 1;
        } else {
            done(1);
        }
        break;
    case 4:
        Cursor_Set(g_cursorManager, 0, 6, 0, 1);
        if (!Session_Open(s(), 0, 1, name())) {
            done(5);
        } else if (g_6d1288) {
            done(6);
        } else {
            MpDialog_SelectMode(g_6d1268, 0, 0xe13);
            g_6cd8dc = 1;
        }
        break;
    case 6:
        Peer_JoinNamed(name());
        if (g_6d1288) done(6);
        break;
    case 2: {
        hook::Global<int32_t(__stdcall*)(void*, int32_t)>(0x5c8084)(F<void*>(thr, 4), 2);
        NewGameConfig_ClearSlots();
        Lobby_HostGame(s(), 0, (const char*)0x6d1128, name(), (const char*)0x6d1228, 0);
        if (F<int32_t>(s(), 0x985c)) Peer_Think((int32_t)(uintptr_t)GetString(0xe60));
        F<uint32_t>(Engine_FirstListEntry(), 0x2690) = hook::As<uint32_t>(g_6226d0);
        g_newGame[0x87] = 1;
        ++F<uint16_t>(g_newGame, 0x79);
        Lobby_AddChatText(nullptr);
        selectMode(0xe12);
        NewGame_RefreshAiSlots(nullptr);
        pump = true;
        break;
    }
    case 3: {
        Lobby_JoinGame(s(), 0, (const char*)0x6d11a8, name(), (const char*)0x6d1228);
        int32_t state = F<int32_t>(s(), 0xc);
        ClockMs();
        if (state != 3) {
            for (;;) {
                sleep();
                state = F<int32_t>(s(), 0xc);
                if (state == 3) break;
                if (state == 1) continue;
                switch (state) {
                case 4: done(0xa); break;
                case 5: done(0xb); break;
                case 6: done(0xc); break;
                case 7: done(0xe); break;
                default: done(2); break;
                }
                goto end;
            }
        }
        selectMode(0xe12);
        g_6d1292 = 0;
        F<uint32_t>(Engine_FirstListEntry(), 0x2690) = hook::As<uint32_t>(g_6226d0);
        uint32_t hello[10] = {};
        Net_LocalAddress((char*)hello);
        hello[8] = g_5f2b70;
        const float version = Game_VersionNumber();
        memcpy(&hello[9], &version, 4);
        Net_Send(s(), 0, (const char*)s() + 0x8f48, 0x51, 0x28, (const uint8_t*)hello, 3, 0);
        for (;;) {
            while (!g_6d1280) sleep();
            if (g_6d1280 != 3) break;
            if (g_6d1284 != 7) {
                // The refusal stays the result (3).
                g_6d1278 = 1;
                goto end;
            }
            g_6d1280 = 0;
            g_6d1284 = 0xff;
            Net_Send(s(), 0, (const char*)s() + 0x8f48, 0x51, 0x28, (const uint8_t*)hello, 3, 0);
        }
        Lobby_AddChatText(nullptr);
        if (F<int32_t>(s(), 0x985c)) Peer_Think((int32_t)(uintptr_t)GetString(0xe61));
        break;
    }
    case 5:
        if (!g_6ce9bc) {
            const char* scenario = (const char*)g_newGame + 0x11;
            Net_Send(s(), 0, nullptr, 0x59, (int32_t)strlen(scenario) + 1, (const uint8_t*)scenario, 1, 0);
            while (!g_6d1280) sleep();
            if (g_6d1280 == 8 || g_6d1280 == 0xd) {
                g_6d1278 = 1;
                if (g_6d40dc)
                    Net_Send(s(), 0, nullptr, 0x11, 0, nullptr, 3, 0);
                else
                    Net_Send(s(), 0, (const char*)s() + 0x8f48, 0x11, 0, nullptr, 1, 0);
                break;
            }
        }
        NewGame_RefreshAiSlots(nullptr);
        Net_Send(s(), 0, nullptr, 2, 0x109, g_newGame, 3, 0);
        pump = true;
        break;
    default:
        break;
    }
    if (pump) GameSpy_Pump();
end:
    Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    g_6d1274 = 0;
    g_6d127c = 0;
    Obj_Decc60(g_windowManager);
    return 0x7f;
}
RT3_REPLACE(0x4ef960, SessionWorker_Run);

// 0x4efe80. Constructs the multiplayer dialog d (vtable 0x5d12ac): resets
// the session state globals and the 0x403c2-byte session buffer 0x6d1270,
// clamps the remembered game count (0x6226d0 from options +0x30d, 1..40),
// loads window 0x5d12b8 and points the two buttons at (444, 211) / (467,
// 234) and 0xe23 / 0xe24 to 0xe17, 0xe37 / 0xe38 to 0xe18. A dialog without
// the 0xe72 list gets it built (scrollbar, button 0xe73, list 0xe72). Then
// mode 0xe10, the 0xe73 page, 0xe2e hidden outside English, and the player
// name (GameSpy's 0x6ae2c4, or options +0x1ef).
uint8_t* __fastcall MpDialog_Construct(uint8_t* d) {
    auto exeNew = (uint8_t*(__cdecl*)(size_t))0x53b070;
    auto crtFree = (void(__cdecl*)(void*))0x5a1145;
    Widget_SetVtblRet(d);
    F<uint32_t>(d, 0) = 0x5d12ac;
    g_6d127c = 0;
    g_6d1274 = 0;
    g_6d1278 = 0;
    g_6d1280 = 0;
    g_6d1288 = 0;
    g_6cd90c = 0;
    g_6cd8dc = 1;
    g_6d1268 = d;
    uint8_t* buf = exeNew(0x403c2);
    g_6d1270 = buf;
    memset(buf, 0, 0x403c2);
    F<int32_t>(d, 0x80) = ClockMs();
    const int32_t games = F<int32_t>(g_app, 0x30d);
    g_6226d0 = games;
    if (games >= 0x29)
        g_6226d0 = 1;
    else if (games <= 0)
        g_6226d0 = 0x28;
    NewGameConfig_ClearSlots();
    Window_Load(d, 0, 0.0f, 0.0f, (const char*)0x5d12b8);
    for (uint8_t* w = F<uint8_t*>(d, 0x70); w; w = F<uint8_t*>(w, 0x8e)) {
        const uint16_t type = F<uint16_t>(w, 6);
        auto at = [&](int32_t px, int32_t py) {
            return type == 4 && !F<uint16_t>(w, 0xae) && Ftol(((double)F<float>(w, 0xb) + D(0x5c88a0)) * D(0x5ca2c8)) == px &&
                   Ftol(((double)F<float>(w, 0xf) + D(0x5c88a0)) * D(0x5ca2d0)) == py && F<uint16_t>(w, 9) == 0xe12;
        };
        if (at(0x1bc, 0xd3)) F<uint16_t>(w, 9) = 0xe17;
        if (at(0x1d3, 0xea)) F<uint16_t>(w, 9) = 0xe17;
        const uint16_t id = F<uint16_t>(w, 4);
        if (id == 0xe24) F<uint16_t>(w, 9) = 0xe17;
        if (id == 0xe23) F<uint16_t>(w, 9) = 0xe17;
        if (id == 0xe37) F<uint16_t>(w, 9) = 0xe18;
        if (id == 0xe38) F<uint16_t>(w, 9) = 0xe18;
    }
    if (!Widget_FindChild(d, 0, 0xe72)) {
        uint8_t bar[0x89];
        memset(bar, 0, sizeof bar);
        F<uint32_t>(bar, 7) = 0x3e9570a4;
        F<uint32_t>(bar, 0xb) = 0x3e85f92c;
        F<uint16_t>(bar, 0x5f) = 2;
        F<uint16_t>(bar, 5) = 0xe17;
        bar[0x3e] = 1;
        String_Assign(&F<char*>(bar, 0x43), (const char*)0x5d1210);
        uint8_t* w = exeNew(0x111);
        Window_AddChild(d, 0, w ? Scrollbar_Construct(w, 0, bar) : nullptr, 0);
        crtFree(F<char*>(bar, 0x43));
        uint8_t button[0xa1];
        memset(button, 0, sizeof button);
        F<uint32_t>(button, 7) = 0x3ecccccd;
        F<uint32_t>(button, 0xb) = 0x3e881b4f;
        F<uint32_t>(button, 0xf) = 0x3ea00000;
        F<uint32_t>(button, 0x13) = 0x3d12c5f9;
        F<uint16_t>(button, 0) = 0xe73;
        button[0x29] = 0x26;
        String_Assign(&F<char*>(button, 0x43), (const char*)0x5d0658);
        F<int32_t>(button, 0x6b) = 0x17;
        F<int32_t>(button, 0x67) = 0x18;
        String_Assign(&F<char*>(button, 0x6f), GetString(0x1101));
        button[0x33] = 3;
        F<int32_t>(button, 0x34) = -0x867;
        F<uint32_t>(button, 0x75) = 0x3d147ae1;
        F<uint32_t>(button, 0x79) = 0;
        F<uint32_t>(button, 0x7d) = 0x3e8d70a4;
        F<uint32_t>(button, 0x81) = 0x3d12c5f9;
        w = exeNew(0xe8);
        Window_AddChild(d, 0, w ? Button_Construct(w, 0, button) : nullptr, 0);
        crtFree(F<char*>(button, 0x43));
        crtFree(F<char*>(button, 0x6f));
        uint8_t list[0xa5];
        memset(list, 0, sizeof list);
        F<uint32_t>(list, 7) = 0x3e9570a4;
        F<uint32_t>(list, 0xb) = 0x3e881b4f;
        F<uint32_t>(list, 0xf) = 0x3db0a3d7;
        F<uint32_t>(list, 0x13) = 0x3d3851ec;
        F<uint32_t>(list, 0x6f) = 0x3c0f5c29;
        F<uint32_t>(list, 0x73) = 0x3b23d70a;
        F<uint32_t>(list, 0x77) = 0x3d87ae14;
        F<uint32_t>(list, 0x7b) = 0x3d0bf259;
        F<uint32_t>(list, 0x85) = 0x3aa3d70a;
        F<uint32_t>(list, 0x81) = 0x3ada740e;
        String_Assign(&F<char*>(list, 0x43), (const char*)0x5d1210);
        F<uint16_t>(list, 0x5f) = 3;
        F<uint16_t>(list, 0x61) = 4;
        list[0x33] = 0xb;
        F<int32_t>(list, 0x34) = -1;
        String_Assign(&F<char*>(list, 0x8b), GetString(0xfb));
        list[0x7f] = 1;
        F<uint16_t>(list, 0) = 0xe72;
        F<uint16_t>(list, 5) = 0xe17;
        list[0x89] = 8;
        list[0x8a] = 9;
        String_Assign(&F<char*>(list, 0x3f), GetString(0x6ea));
        w = exeNew(0xe9);
        Window_AddChild(d, 0, w ? Control_Construct(w, 0, list) : nullptr, 0);
        crtFree(F<char*>(list, 0x43));
        crtFree(F<char*>(list, 0x3f));
        crtFree(F<char*>(list, 0x8b));
    }
    F<int32_t>(g_app, 0x6c) = 1;
    MpDialog_SelectMode(d, 0, 0xe10);
    Window_SendEvent5(d, 0, 0x87, 0xe73, 3, 0, 0);
    if (!(uint8_t)IsEnglish()) Window_SendEvent5(d, 0, 0x78, 0xe2e, 1, 0, 0);
    if (g_6cd920)
        ((char*(__cdecl*)(char*, const char*, size_t))0x5a18a0)((char*)g_app + 0x1ef, (const char*)0x6ae2c4, 0x3f);
    const char* name = (const char*)g_app + 0x1ef;
    if (!*name) {
        char computer[0x10];
        uint32_t size = 0x10;
        if (!((int32_t(__stdcall*)(char*, uint32_t*))g_5c8080)(computer, &size)) return d;
        // QUIRK: this exe then takes the fixed name in its header (0x400b94),
        // not the computer name just read.
        name = (const char*)0x400b94;
    }
    NameDialog_ShowName(d, 0, name);
    return d;
}
RT3_REPLACE(0x4efe80, MpDialog_Construct);

// 0x4f03f0 (vtable +4; Ghidra lists its body as 0x4f0400). Event handler of
// the multiplayer dialog d (+0x7c page: 0xe11 games / 0xe12 lobby / 0xe13
// game setup, +0x80 next refresh). While 0x6cd90c counts the exit (to 5:
// back to the menu) nothing else runs; then the session events are reported
// (0x4ef200) and, on the lobby, the GameSpy name / retry steps run and
// once a second the player and game lists refresh. Keys: Enter in the chat
// boxes. Clicks (0xca): the ready toggles 0xe61..0xe6a (own entry: sent to
// everyone), the chat send buttons, the name and address boxes, the 0xe73
// toggle. Commands (0xcb): back, host / join / connect, the scenario and
// map pickers, the setup settings (+0x77 year, +0x7b / +0x79 AI and
// player counts, +0x7d difficulty) sent to everyone, start (seed and the
// resync offer). Returns 0 when handled.
int32_t __fastcall MpDialog_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    auto app = []() { return g_app; };
    auto cfg = []() { return g_newGame; };
    auto session = []() { return g_session; };
    auto text = [](uint8_t* w) { return F<char*>(w, 0xba); };
    auto strcmp_ = [](const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); };
    auto msgBox = [](const char* t, int32_t type) {
        Dialog_RunMessage(t, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    };
    auto say = [&](int32_t id) { msgBox(GetString(id), 1); };
    auto sendSetup = [&]() { Net_Send(session(), 0, nullptr, 2, 0x109, cfg(), 3, 0); };
    auto clearAddress = [&]() {
        text(Widget_FindChild(d, 0, 0xe47))[0] = 0;
        hook::As<uint8_t>(g_6d11a8) = 0;
        return 0;
    };
    // A new player name (renames the host directly, else asks the host).
    auto rename = [&](const char* name) {
        if (!strcmp_(name, (const char*)app() + 0x1ef)) return 0;
        if (F<int32_t>(session(), 0x985c)) {
            strcpy((char*)app() + 0x1ef, name);
            Session_StartWorkerMode(6);
        } else {
            NameDialog_SetName(d, 0, name);
        }
        return 0;
    };
    if (const int32_t leaving = g_6cd90c) {
        if (leaving == 5) {
            g_d973e8 = ClockMs();
            F<int32_t>(cfg(), 1) = 3;
            Game_PostInputEvent(0xcc, 0, 1, 0, 0);
            return 0;
        }
        g_6cd90c = leaving + 1;
        return 1;
    }
    if (MpSession_ReportEvent() == 1) {
        if (g_6d1280 == 0xa) {
            if (!g_6d1292) {
                Dialog_RunMessage(GetString(0xe68), 5, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0x6d1228, 0x1b, 0, nullptr, nullptr, 0, 0,
                                  nullptr, 0);
                g_6d1292 = 1;
                Session_StartWorkerMode(3);
            } else if (g_6d1280 == 0xa) {
                g_6d1292 = 0;
            }
        }
        if (g_6d1280 == 2) return clearAddress();
        return 0;
    }
    if (!g_6cd8dc || g_6d1274 == 1 || g_6d1288) return 0;
    if (g_6cd920) {
        // GameSpy: take the chosen name, then join the chosen server.
        const int32_t step = g_6ae3c4;
        if (step == 1) {
            if (g_6ae2c4) NameDialog_ShowName(d, 0, (const char*)0x6ae2c4);
            g_6ae3c4 = 2;
            Session_Restart(d, 0, 1);
            return 0;
        }
        if (step == 2) {
            g_6ae3c4 = 3;
            if (g_6ae1c0) {
                MpDialog_Connect(d);
                return 0;
            }
            if (g_6ae3c8 >= 3) return 0;
            ++g_6ae3c8;
            MpDialog_Join(d, 0, (const char*)0x6ae1c4);
            return 0;
        }
    }
    if (F<int32_t>(d, 0x80) < ClockMs()) {
        if (F<int32_t>(d, 0x7c) == 0xe12 || F<int32_t>(d, 0x7c) == 0xe13) Lobby_RefreshPlayers(d);
        if (F<int32_t>(d, 0x7c) == 0xe11 || F<int32_t>(d, 0x7c) == 0xe13) GameList_Fill(d);
        F<int32_t>(d, 0x80) = ClockMs() + 1000;
        NewGameDialog_ShowSetting7b(d);
        if (hook::As<int32_t>(g_6d40d0) && g_6d40dc) GameSpy_Pump();
        return 0;
    }
    const int32_t hosting = g_6d40dc;
    switch (msg[0]) {
    case 3:
        // Enter in a chat box.
        if (msg[2] != 0x20) return 1;
        if (F<int32_t>(d, 0x7c) == 0xe13) {
            EditBox_ActivateClick(Widget_FindChild(d, 0, 0xe5b), 0, msg);
            return 0;
        }
        if (F<int32_t>(d, 0x7c) == 0xe12) EditBox_ActivateClick(Widget_FindChild(d, 0, 0xe49), 0, msg);
        return 0;
    case 0xce: {
        // A pick in the scenario list.
        if (msg[1] != 0xe30 || (int32_t)msg[2] < 0) return 1;
        char before[0x100];
        strcpy(before, (const char*)cfg() + 0x11);
        uint8_t* e = F<uint8_t*>(g_6d126c, 8) + (int32_t)msg[2] * 0x25a;
        MpScenarioPanel_Update(d, 0, (const char*)e, (const char*)e + 0x12d, 0);
        if (strcmp_(before, (const char*)cfg() + 0x11)) {
            F<uint16_t>(cfg(), 0x77) = F<uint16_t>(g_6d1270, 0xc);
            g_6ce9bc = 0;
            Window_SetMode1(d, 0, 0);
        }
        NewGameDialog_ShowSetting77(d);
        return 1;
    }
    case 0xca: {
        const uint32_t id = msg[1];
        if (id >= 0xe61 && id <= 0xe6a) {
            // A player's ready toggle (only the own one changes and is sent).
            uint8_t* entry = Textures_Find(F<int32_t>(Widget_FindChild(d, 0, (int32_t)id), 0x4f));
            if (!entry) return 0;
            if (F<int32_t>(entry, 4) == Global6d40e4()) {
                F<uint32_t>(entry, 0x5c) ^= 1;
                Net_UnlockQueueArg(F<int32_t>(Widget_FindChild(d, 0, (int32_t)msg[1]), 0x4f));
                uint8_t ready = F<uint8_t>(entry, 0x5c) & 1;
                Window_SendEvent5(d, 0, 0x6e, msg[1], (uint32_t)(uintptr_t)GetString(ready ? 0xe69 : 0xe6a), 0, 0);
                Net_Send(session(), 0, nullptr, 9, 1, &ready, 3, 0);
                return 0;
            }
            Net_UnlockQueueArg(F<int32_t>(Widget_FindChild(d, 0, (int32_t)msg[1]), 0x4f));
            uint8_t* box = Widget_FindChild(d, 0, (int32_t)msg[1]);
            F<int32_t>(box, 0xcc) = F<int32_t>(box, 0xcc) == 0;
            return 0;
        }
        switch (id) {
        case 0xe73:
            g_6ce9c8 = g_6ce9c8 == 0;
            return 0;
        case 0xe49:
        case 0xe5b: {
            // Send the chat line.
            char* line = text((uint8_t*)(uintptr_t)msg[0xb]);
            if (!strlen(line)) return 0;
            Net_Send(session(), 0, nullptr, 1, (int32_t)strlen(line) + 1, (const uint8_t*)line, 1, 0);
            Window_SendEvent5(d, 0, 0x6f, msg[1], 0x5c87a8, 0, 0);
            return 0;
        }
        case 0xe48:
        case 0xe5d:
            return rename(text(Widget_FindChild(d, 0, (int32_t)id)));
        case 0xe47:
            strcpy((char*)0x6d11a8, text(Widget_FindChild(d, 0, 0xe47)));
            if (strlen((const char*)0x6d11a8) <= 5) return clearAddress();
            Session_StartWorkerMode(3);
            return 0;
        default:
            return id == 0xe45 ? 0 : 1;
        }
    }
    case 0xcb:
        switch (msg[1]) {
        case 0xe1d:
            Game_PostInputEvent(0xcc, 0, 2, 0, 0);
            if (uint8_t* s = session()) {
                Session_Shutdown(s);
                ((void(__cdecl*)(void*))0x53b080)(s);
                g_session = nullptr;
            }
            return 0;
        case 0xe1b:
            if (!g_6cd920) {
                Session_Restart(d, 0, 1);
                return 0;
            }
            say(0xf4f);
            return 0;
        case 0xe1c:
            Session_Restart(d, 0, 0);
            return 0;
        case 0xe20:
        case 0xe5a:
            MpDialog_SelectMode(d, 0, 0xe10);
            return 0;
        case 0xe1e:
        case 0xe58:
            MpDialog_Join(d, 0, nullptr);
            return 0;
        case 0xe1f:
        case 0xe59:
            MpDialog_Connect(d);
            return 0;
        case 0xe21:
        case 0xe5e: {
            // Ask for a new player name.
            char name[0x100];
            strcpy(name, (const char*)app() + 0x1ef);
            Dialog_RunMessage((const char*)0x5c87a8, 9, 0xbf800000, 0xbf800000, 0x4ed050,
                              (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4ecdd0, 0, BitsF(0x3e2aaaab),
                              (uint32_t)(uintptr_t)name, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            if (!name[0]) return 0;
            return rename(name);
        }
        case 0xe24:
        case 0xe25:
            if (hosting) {
                const int32_t source = (int32_t)msg[1] - 0xf;
                MpScenario_SelectSource(d, 0, F<int32_t>(d, 0x78) == source ? -1 : source);
                return 0;
            }
            Window_SendEvent5(d, 0, 0x87, 0xe24, 0x20, 0, 0);
            say(0xe21);
            return 0;
        case 0xe26:
        case 0xe27:
        case 0xe28:
        case 0xe29:
        case 0xe2a:
            if (hosting) {
                cfg()[0x7d] = (uint8_t)(msg[1] - 0x26);
                Net_SendReady();
                sendSetup();
            } else {
                say(0xe21);
            }
            NewGameDialog_ShowDifficulty(d);
            return 0;
        case 0xe2b:
            if (cfg()[0x10]) {
                g_6226d4 = 0;
                g_6ce9bc = 0;
                Window_SetMode1(d, 0, 0);
                MpScenarioPanel_Update(d, 0, (const char*)cfg() + 0x11, (const char*)cfg() + 0x44, 1);
                g_6226d4 = 1;
                Session_StartWorkerMode(5);
            } else {
                MpScenarioPanel_Update(d, 0, nullptr, nullptr, 1);
            }
            MpScenario_SelectSource(d, 0, -1);
            Net_SendReady();
            return 0;
        case 0xe2c: {
            int32_t year = F<uint16_t>(cfg(), 0x77) + 5;
            const int32_t last = F<int32_t>(g_6d1270, 0x10);
            if (last < year) year = last;
            F<uint16_t>(cfg(), 0x77) = (uint16_t)year;
            NewGameDialog_ShowSetting77(d);
            return 0;
        }
        case 0xe2d: {
            int32_t year = F<uint16_t>(cfg(), 0x77) - 5;
            const int32_t first = F<int32_t>(g_6d1270, 0xc);
            if (first > year) year = first;
            F<uint16_t>(cfg(), 0x77) = (uint16_t)year;
            NewGameDialog_ShowSetting77(d);
            return 0;
        }
        case 0xe31:
            MpScenario_SelectSource(d, 0, -1);
            Net_SendReady();
            return 0;
        case 0xe33:
            // Start: everyone ready, the setup and the seed to everyone.
            if (!hosting) {
                say(0x2cf);
                return 0;
            }
            if (!Net_AllPlayersFlag1()) {
                say(0xf15);
                return 0;
            }
            Net_Send(session(), 0, nullptr, 4, 0x48d, app() + 0x70, 3, 0);
            F<int32_t>(cfg(), 0x83) = Rand(0, 0x7fff);
            Net_Send(session(), 0, nullptr, 2, 0x109, cfg(), 3, 0);
            SetSeed(F<int32_t>(cfg(), 0x83));
            g_6d4110 = 1;
            if (g_6ce9c8) {
                Net_Send(session(), 0, nullptr, 0x83, 0, nullptr, 3, 0);
                Net_OfferResync();
            }
            Net_Send(session(), 0, nullptr, 3, 0, nullptr, 1, 0);
            return 0;
        case 0xe34:
            Window_SendEvent5(d, 0, 0x87, 0xe73, 3, 0, 0);
            Session_Restart(d, 0, F<int32_t>(session(), 0x985c));
            return 0;
        case 0xe37:
            if (!hosting) {
                say(0xe21);
                return 0;
            }
            if (!NewGameConfig_ClaimAiSlot(1)) {
                say(0x2b8);
                return 0;
            }
            NewGameConfig_ClaimAiSlot(0);
            NewGameDialog_ShowSetting7b(d);
            Net_SendReady();
            sendSetup();
            return 0;
        case 0xe38: {
            // Drop an AI player (when the slots allow it).
            if (!hosting) {
                say(0xe21);
                return 0;
            }
            uint8_t* c = cfg();
            const int32_t ai = F<uint16_t>(c, 0x7b);
            if (!ai) return 0;
            uint8_t* slots = g_6d1270;
            int32_t open = 0, empty = 0;
            if (slots && F<int32_t>(slots, 0x3ae) > 0)
                for (int32_t k = 0; k < F<int32_t>(slots, 0x3ae); ++k)
                    if (!slots[0x31b + k * 9] && !slots[0x31d + k * 9]) ++open;
            if (open >= ai) {
                say(0x2b9);
                return 0;
            }
            if (slots && F<int32_t>(slots, 0x3ae) > 0)
                for (int32_t k = 0; k < F<int32_t>(slots, 0x3ae); ++k)
                    if (!slots[0x31b + k * 9]) ++empty;
            if (empty >= F<uint16_t>(c, 0x79) + ai) {
                say(0x2b9);
                return 0;
            }
            for (int32_t k = 0xf; k >= 0; --k)
                if (c[0x87 + k] > 0x64) {
                    c[0x87 + k] = 0;
                    --F<uint16_t>(cfg(), 0x7b);
                    break;
                }
            NewGameDialog_ShowSetting7b(d);
            Net_SendReady();
            Net_Send(session(), 0, nullptr, 2, 0x109, cfg(), 3, 0);
            return 0;
        }
        case 0xe72: {
            // Pick a map file (players of version 1.04 on).
            if (!Net_AllHaveVersion(BitsF(0x3f851eb8))) {
                Window_SendEvent5(d, 0, 0x87, 0xe24, 0x20, 0, 0);
                say(0x10ff);
                return 0;
            }
            if (!hosting) {
                Window_SendEvent5(d, 0, 0x87, 0xe24, 0x20, 0, 0);
                say(0xe21);
                return 0;
            }
            char file[0x12c], base[0x100], label[0x100];
            memset(file, 0, sizeof file);
            g_6d1298 = 1;
            F<int32_t>(app(), 0x6c) = 0;
            if (FileDialog_Run(file, 0xc)) {
                g_6ce9bc = 1;
                strcpy(base, file);
                strcpy((char*)0x6ce630, file);
                StripExtension(base);
                FormatText(label, GetString(0x10da), base, 0, 0, 0, 0, 0, 0, 0);
                MpScenarioPanel_Update(d, 0, file, label, 1);
                Session_StartWorkerMode(5);
            }
            g_6d1298 = 0;
            Window_SendEvent5(d, 0, 0x87, 0xe72, 0x20, 0, 0);
            return 0;
        }
        default:
            return 1;
        }
    default:
        return 1;
    }
}
RT3_REPLACE(0x4f03f0, MpDialog_HandleEvent);

// 0x4f13e0 (fastcall). Refreshes the multiplayer game dialog 0x6d1268 for
// the host / client role (0x6ce9bc; 0x6cec74+0x6c = host). With reselect it
// replays the dialog's mode (+0x7c) and scenario source (+0x78) choices with
// 0x6d1291 held at 1 and, while hosting a GameSpy game (0x6d40d0,
// 0x6d40dc), pumps GameSpy; otherwise it clears 0x6cd8dc and sets 0x6d1278 =
// 1, 0x6d1280 = 7.
RT3_NOINLINE void __fastcall MpDialog_Refresh(int32_t reselect) {
    if (!g_6d1268) return;
    Window_SetMode1(g_6d1268, 0, g_6ce9bc);
    F<int32_t>(g_app, 0x6c) = g_6ce9bc == 0;
    if (!reselect) {
        g_6cd8dc = 0;
        g_6d1278 = 1;
        g_6d1280 = 7;
        return;
    }
    uint8_t* d = g_6d1268;
    int32_t mode = F<int32_t>(d, 0x7c), source = F<int32_t>(d, 0x78);
    uint8_t held = *(uint8_t*)0x6d1291;
    *(uint8_t*)0x6d1291 = 1;
    MpDialog_SelectMode(d, 0, mode);
    MpScenario_SelectSource(g_6d1268, 0, source);
    *(uint8_t*)0x6d1291 = held;
    if (hook::As<int32_t>(g_6d40d0) && g_6d40dc) GameSpy_Pump();
}
RT3_REPLACE(0x4f13e0, MpDialog_Refresh);

// 0x4f1490 (event callback). Presses of 0xb9f2..0xb9f4 pick choice 0..2
// (0x6226dc) and select that button (event 0x74). Returns 0 when handled.
int32_t __fastcall Picker_ChooseCb(uint8_t* w, const uint32_t* ev) {
    if (ev[0] != 0xca || (int32_t)ev[1] < 0xb9f2 || (int32_t)ev[1] > 0xb9f4) return 1;
    g_6226dc = (int32_t)ev[1] - 0xb9f2;
    Window_SendEvent5(w, 0, 0x74, ev[1], 1, 0, 0);
    return 0;
}
RT3_REPLACE(0x4f1490, Picker_ChooseCb);

// 0x4f14d0. The locomotive detail panel for the picked engine (0x6d129c in
// 0x6ada84; nothing when it is gone): an optional title (0x2bb), the engine's
// number (+0x105 of its type +0x72), its name and seven stats (labels 0x2bc..
// 0x2c2: cost with the company / player multipliers, +8, three graded ratings
// from 0x620d88 / 0x620d58 / 0x620d2c with the effect bonuses, 0x620db4 by
// +0x14 and 0x2c3.. by +0x10), the speed step for the era (10 / 20 / 40 / 80
// mph, the shown unit 0x6226dc), then a 5 x 9 payment table: speeds across,
// distances down, the payment per car (at least 1).
void __fastcall LocoDetail_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t) {
    if (!Collection_IsValid(g_6ada84, 0, g_6d129c)) return;
    auto font = [](uint32_t a) { return hook::Global<uint8_t*>(a); };
    if (g_6226d8) {
        float ty = (float)((double)y - D(0x5d0820));
        Font_DrawTextBox(font(0x6d12a4), 0, GetString(0x2bb), x, ty, w, BitsF(0x3ccccccd), 0xffffffff, 0, 0, 0, 1.0f);
    }
    uint8_t* rec = (uint8_t*)Collection_Get(g_6ada84, 0, g_6d129c);
    uint8_t* type = F<uint32_t>(rec, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(rec, 0x72)) : nullptr;
    char text[0x200], line[0x200];
    Num_Format(text, 8, (double)F<int32_t>(type, 0x105), 10, 1, 0, 0);
    float ny = (float)((double)y + Fl(0x5d1318));
    Font_DrawTextBox(font(0x6d12ac), 0, text, (float)((double)x + Fl(0x5d1314)), ny, w, BitsF(0x3ccccccd), 0xffcfcfcf, 2, 1, 0, 1.0f);
    float rowY = (float)((double)y + Fl(0x5d1310));
    int32_t base = Ftol(F<float>(type, 0xfd));
    if (rec[0x7a]) {
        uint8_t* second = F<uint32_t>(rec, 0x76) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(rec, 0x76)) : nullptr;
        float bf = (float)base;
        base = Ftol((double)bf + F<float>(second, 0xfd));
    }
    char stats[7][0x32];
    memset(stats, 0, sizeof stats);
    static const int32_t labels[7] = { 0x2bc, 0x2bd, 0x2be, 0x2bf, 0x2c0, 0x2c1, 0x2c2 };
    uint8_t* world = g_world;
    auto who = [&](uint32_t& company, uint32_t& player) {
        player = (uint32_t)Obj_Get25(g_world);
        company = (uint32_t)Obj_Get21(g_world);
    };
    uint32_t co, pl;
    who(co, pl);
    int32_t k1 = Ftol(Obj_EffectScale(rec, 0, co, pl));
    int32_t k2 = Ftol(F<float>(rec, 8));
    who(co, pl);
    float f1 = Effects_Multiplier(g_world, 0, F<int32_t>(rec, 0x10) + 0x4e, co, pl, 0);
    who(co, pl);
    float f2 = Effects_Multiplier(g_world, 0, 0x4d, co, pl, 0);
    float k1f = (float)k1;
    int32_t cost = Ftol((double)f2 * f1 * k1f);
    Num_FormatSimple2(stats[0], 0xb, (double)cost, 0xb);
    Num_FormatSimple2(stats[1], 0xb, (double)k2, 0xb);
    int32_t kinds[3] = { 0x2c3, 0x2c4, 0x2c5 };
    auto bonus = [&](int32_t idx) {
        uint32_t c, q;
        who(c, q);
        return Obj_CargoBonus(g_world, 0, idx, c, q, 0);
    };
    auto grade = [&](char* out, uint32_t table, int32_t idx) { FormatText(out, GetString(hook::Global<int32_t>(table + idx * 4)), 0, 0, 0, 0, 0, 0, 0, 0); };
    int32_t b = bonus(F<int32_t>(rec, 0x10) + 0x4a);
    int32_t g = F<int32_t>(rec, 0xc) + b + bonus(0x49);
    g = g < 1 ? 1 : g >= 10 ? 10 : g;
    grade(stats[2], 0x620d88, g);
    b = bonus(F<int32_t>(rec, 0x10) + 0x5e);
    g = F<int32_t>(rec, 0x1c) + b + bonus(0x5d);
    if (g < 0) g = 0;
    if (g >= 0xb) g = 0xb;
    grade(stats[3], 0x620d58, g);
    b = bonus(F<int32_t>(rec, 0x10) + 0x56);
    g = F<int32_t>(rec, 0x18) + b + bonus(0x55);
    if (g < 0) g = 0;
    if (g >= 0xa) g = 0xa;
    grade(stats[4], 0x620d2c, g);
    grade(stats[5], 0x620db4, F<int32_t>(rec, 0x14));
    FormatText(stats[6], GetString(kinds[F<int32_t>(rec, 0x10)]), 0, 0, 0, 0, 0, 0, 0, 0);
    uint8_t* t2 = F<uint32_t>(rec, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(rec, 0x72)) : nullptr;
    FormatText(text, LocalizeName((const char*)t2 + 0x45), 0, 0, 0, 0, 0, 0, 0, 0);
    float ty = (float)((double)rowY - D(0x5d0820));
    Font_DrawTextBox(font(0x6d12a4), 0, text, x, ty, w, BitsF(0x3ccccccd), 0xffffffff, 0, 0, 0, 1.0f);
    rowY = (float)((double)rowY + Fl(0x5d0c60));
    rowY = (float)((double)rowY + D(0x5d1308));
    float sw = (float)((double)w - D(0x5c9b98));
    float sx = (float)((double)x + D(0x5c8918));
    for (int32_t i = 0; i < 7; ++i) {
        float ly = (float)((double)rowY - D(0x5d0820));
        Font_DrawTextBox(font(0x6d12a4), 0, GetString(labels[i]), sx, ly, sw, BitsF(0x3ccccccd), 0xffffffff, 2, 0, 0, 1.0f);
        Font_DrawTextBox(font(0x6d12a4), 0, stats[i], sx, ly, sw, BitsF(0x3ccccccd), 0xffffffff, 3, 0, 0, 1.0f);
        rowY = (float)((double)rowY + Fl(0x5d0c60));
    }
    rowY = (float)((double)rowY + D(0x5d1300));
    uint16_t year = F<uint16_t>(world, 0xd);
    int32_t step[3];
    step[0] = year < 0x73a ? 10 : year < 0x76c ? 20 : year < 0x79e ? 40 : 80;
    step[1] = Ftol((double)step[0] * D(0x5d12f8) + D(0x5c8628));
    int32_t sum = step[1] + step[0];
    step[2] = Ftol((double)sum / D(0x5c85d0) + D(0x5c8628));
    Font_DrawTextBox(font(0x6d12ac), 0, GetString(0x2c6), x, rowY, w, BitsF(0x3cb17e4b), 0xffffffff, 0, 0, 0, 1.0f);
    rowY = (float)((double)rowY + D(0x5d12f0));
    rowY = (float)((double)rowY + D(0x5d12e8));
    char num[0x40];
    Num_Format(num, 1, (double)step[g_6226dc], 10, 1, 0, 0);
    FormatText(line, GetString(0x2c7), num, 0, 0, 0, 0, 0, 0, 0);
    Font_DrawTextBox(font(0x6d12ac), 0, line, (float)((double)x + D(0x5d0700)), rowY, w, BitsF(0x3cb17e4b), 0xffffffff, 0, 0, 0, 1.0f);
    rowY = (float)((double)rowY + D(0x5d12e0));
    double inset = x;
    float lw = (float)((double)h * D(0x5c8618));
    float lyy = (float)((double)y + h + D(0x5d12d8));
    float lx = (float)(inset + D(0x5c9b50));
    Font_DrawTextBox(font(0x6d12a8), 0, GetString(0x2c8), lx, lyy, lw, BitsF(0x3cb17e4b), 0xffffffff, 0, 0, 0, 1.0f);
    rowY = (float)((double)rowY + D(0x5d12d0));
    char cell[0x20];
    float rowStart = (float)(inset + D(0x5c9b98));
    for (int32_t row = 0; row < 5; ++row) {
        float colX = rowStart;
        for (int32_t col = 0; col < 9; ++col) {
            int32_t spd = step[g_6226dc] * col + base;
            float spdF = (float)spd;
            if (!col) {
                if (row > 0) ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(cell, (const char*)0x5d0b18, row * 2 - 2);
                else cell[0] = 0;
            } else if (!row)
                Num_Format(cell, 1, (double)col, 10, 1, 0, 0);
            else {
                float dist = (float)((double)(row - 1) * D(0x5c8918));
                dist = (float)(SpeedFactor(dist) * spdF);
                uint32_t company = (uint32_t)Obj_Get21(g_world);
                float pay = Cargo_DeliveryPayment(rec, F<uint16_t>(g_world, 0xd), dist, company, 0);
                if (pay < D(0x5c8770)) pay = 1.0f;
                Num_Format(cell, 1, (double)Ftol((double)pay + D(0x5c8628)), 10, 1, 0, 0);
            }
            uint8_t* f = row && col ? font(0x6d12a4) : font(0x6d12ac);
            inset = row ? 0.0 : BitsD(0x3f747ae147ae147bull);
            float cx = (float)((double)colX - inset);
            Font_DrawTextBox(f, 0, cell, cx, rowY, BitsF(0x3cf5c28f), BitsF(0x3cb17e4b), 0xffffffff, 3, 0, 0, 1.0f);
            colX = (float)((double)colX + Fl(0x5d12c8));
        }
        rowY = (float)((double)rowY + Fl(0x5d0c60));
    }
}
RT3_REPLACE(0x4f14d0, LocoDetail_Draw);

// 0x4f1f90. Builds the locomotive window's widgets at (x, y): a frame, the
// engine picture (%1_NE.imb of its type, or the region's variant from the
// pack's region list 0x429b8 when the picked city 0x6d12a0 has one, else a
// generic picture by the engine kind +0x10), three speed-unit buttons
// (0xb9f2.., the current unit 0x6226dc checked) whose tooltips give the
// era's step (10 / 20 / 40 / 80) in each unit, the seven stat labels
// 0xf02..0xf08 and the payment table caption 0x87c.
void __fastcall LocoWindow_Build(uint8_t* w, int, float x, float y, float wd, float) {
    uint8_t def[0x89];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + Fl(0x5d0e18));
    F<float>(def, 0xb) = (float)((double)y + D(0x5d1350));
    F<uint32_t>(def, 0xf) = 0x3e800000;
    F<uint32_t>(def, 0x13) = 0x3df5c28f;
    F<int32_t>(def, 0x61) = 3;
    F<int32_t>(def, 0x75) = 9;
    F<uint32_t>(def, 0x2f) = 0xb0;
    uint8_t* s = (uint8_t*)ExeNew(0x111);
    s = s ? Scrollbar_Construct(s, 0, def) : nullptr;
    Window_AddChild(w, 0, s, 0);
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + Fl(0x5d0e14));
    F<uint32_t>(def, 0xf) = 0x3e75c28f;
    F<uint32_t>(def, 0x13) = 0x3dda740e;
    F<float>(def, 0xb) = (float)((double)y + D(0x5d1348));
    uint8_t* rec = (uint8_t*)Collection_Get(g_6ada84, 0, g_6d129c);
    uint8_t* type = F<uint32_t>(rec, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(rec, 0x72)) : nullptr;
    char name[0x220];
    FormatText(name, (const char*)0x5d0e08, (const char*)type + 0x82, 0, 0, 0, 0, 0, 0, 0);
    if (g_6d12a0 && ((uint8_t*)Obj_City41(g_6d12a0))[0x3cc]) {
        for (int32_t i = 0; i < Collection_Count(F<Collection*>(g_pack, 0x429b8)); ++i) {
            Collection* regions = F<Collection*>(g_pack, 0x429b8);
            uint8_t* e = (uint8_t*)Collection_Get(regions, 0, Collection_IdAt(regions, 0, i, 0));
            if (!Stricmp((const char*)Obj_City41(g_6d12a0) + 0x3ac, (const char*)e)) {
                FormatText(name, (const char*)e + 0x80, 0, 0, 0, 0, 0, 0, 0, 0);
                break;
            }
        }
    }
    if (!NameTable_ContainsName(g_pack, 0, name, 0, 0)) {
        int32_t kind = F<int32_t>(rec, 0x10);
        FormatText(name, kind == 1 ? (const char*)0x5d0df0 : kind == 2 ? (const char*)0x5d0dd8 : (const char*)0x5d0dc0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    String_Assign(&F<char*>(def, 0x43), name);
    F<uint16_t>(def, 0x5f) = 0;
    F<uint32_t>(def, 0x2f) = 0xb0;
    s = (uint8_t*)ExeNew(0x111);
    s = s ? Scrollbar_Construct(s, 0, def) : nullptr;
    Window_AddChild(w, 0, s, 0);
    Free(F<void*>(def, 0x43));
    float by = (float)((double)y + D(0x5d1338));
    static const uint32_t bx[3] = { 0x5d1340, 0x5d1330, 0x5d132c };
    for (int32_t i = 0; i < 3; ++i) {
        uint8_t b[0xa1];
        memset(b, 0, sizeof b);
        F<float>(b, 7) = (float)((double)x + Fl(bx[i]));
        F<float>(b, 0xb) = by;
        F<uint32_t>(b, 0xf) = 0x3db33333;
        F<uint32_t>(b, 0x13) = 0x3d5a740e;
        F<uint16_t>(b, 0) = (uint16_t)(0xb9f2 + i);
        F<uint32_t>(b, 0x85) = 0xb9f1;
        b[0x29] = 0x25;
        String_Assign(&F<char*>(b, 0x43), (const char*)0x5d0a30);
        F<uint32_t>(b, 0x6b) = 0;
        F<uint32_t>(b, 0x67) = 1;
        String_Assign(&F<char*>(b, 0x6f), GetString(0x2c9 + i));
        b[0x33] = 1;
        F<int32_t>(b, 0x34) = -1;
        F<uint32_t>(b, 0x75) = 0x3cb851ec;
        F<uint32_t>(b, 0x79) = 0xbc5a740e;
        F<uint32_t>(b, 0x7d) = 0x3d800000;
        F<uint32_t>(b, 0x81) = 0x3d5a740e;
        uint8_t* bt = (uint8_t*)ExeNew(0xe8);
        bt = bt ? Button_Construct(bt, 0, b) : nullptr;
        Window_AddChild(w, 0, bt, 0);
        Free(F<void*>(b, 0x43));
        Free(F<void*>(b, 0x6f));
    }
    Window_SendEvent5(w, 0, 0x74, 0xb9f2 + g_6226dc, 1, 0, 0);
    uint16_t year = F<uint16_t>(g_world, 0xd);
    int32_t step[3];
    step[0] = year < 0x73a ? 10 : year < 0x76c ? 20 : year < 0x79e ? 40 : 80;
    step[1] = Ftol((double)step[0] * D(0x5d12f8) + D(0x5c8628));
    int32_t sum = step[1] + step[0];
    step[2] = Ftol((double)sum / D(0x5c85d0) + D(0x5c8628));
    char num[0x40], tip[0x220];
    for (int32_t i = 0; i < 3; ++i) {
        Num_Format(num, 1, (double)step[i], 10, 1, 0, 0);
        FormatText(tip, GetString(0x546 + i), num, 0, 0, 0, 0, 0, 0, 0);
        strcat(tip, GetString(0xf09));
        Window_SendEvent5(w, 0, 0x6e, 0xb9f2 + i, (uint32_t)(uintptr_t)tip, 0, 0);
    }
    static const int32_t labels[7] = { 0xf02, 0xf03, 0xf04, 0xf05, 0xf06, 0xf07, 0xf08 };
    float rowY = (float)((double)y + Fl(0x5d1328));
    rowY = (float)((double)rowY + D(0x5d1308));
    float lx = (float)((double)x + D(0x5c8918));
    float lw = (float)((double)wd - D(0x5c8918));
    auto timer = [&](uint32_t px, uint32_t py, uint32_t pw, uint32_t ph, const char* text) {
        uint8_t t[0x8b];
        memset(t, 0, sizeof t);
        F<uint32_t>(t, 7) = px;
        F<uint32_t>(t, 0xb) = py;
        F<uint32_t>(t, 0xf) = pw;
        F<uint32_t>(t, 0x13) = ph;
        String_Assign(&F<char*>(t, 0x3f), text);
        uint8_t* tw = (uint8_t*)ExeNew(0xda);
        tw = tw ? TimerWidget_Construct(tw, 0, t) : nullptr;
        Window_AddChild(w, 0, tw, 0);
        Free(F<void*>(t, 0x3f));
    };
    for (int32_t i = 0; i < 7; ++i) {
        float ly = (float)((double)rowY - D(0x5d0820));
        timer(FBits(lx), FBits(ly), FBits(lw), 0x3ccccccd, GetString(labels[i]));
        rowY = (float)((double)rowY + Fl(0x5d0c60));
    }
    float cx = (float)((double)x + D(0x5d0160));
    float cy = (float)((double)y + D(0x5d1320));
    timer(FBits(cx), FBits(cy), 0x3e933333, 0x3e01b4e8, GetString(0x87c));
}
RT3_REPLACE(0x4f1f90, LocoWindow_Build);

}  // namespace rt3
