// TrackTool.cpp - RT3.exe 0x50c000-0x510000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <initializer_list>
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

uint8_t* __fastcall Net_QueueText(uint8_t* s, int, const char* text, int32_t a, int32_t b, int32_t c, int32_t d);

int32_t __fastcall Map_Bulldoze(uint8_t* map, int, float x, float y, float r, uint32_t company, int32_t checkOnly, float* cost, char* msg);

int32_t __fastcall Obj_Byte21c5(void* o);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
int32_t __fastcall Obj_Get25(void* o);
int32_t __fastcall Obj_Get21(void* o);
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player);
int32_t IsMultiplayer();
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall Track_UpdateColor(uint8_t* o);
void __fastcall String_Assign(char** s, const char* src);
void __fastcall Editor_SelectionCost(int32_t* total, int32_t* count);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Obj_Ref25(void* o);
int32_t __fastcall Table_CountField3f(Collection* t);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall Reset_625a94();
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
double One();
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t __stdcall Net_AllHaveVersion(float v);
int32_t __fastcall Obj_Has98(uint8_t* o);
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Scene_PushBusy(uint8_t* s);
void __fastcall Scene_PopBusy(uint8_t* s);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
double __stdcall SpeedFactor(float v);
float __fastcall Cargo_DeliveryPayment(uint8_t* o, int32_t year, float v, uint32_t company, uint32_t region);
void UpgradeDialog_UpdateCostText();
int32_t __fastcall Collection_Count(Collection* c);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t Global6d40e4();
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Tracks_UpdateColor(Collection* c);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

void __fastcall Camera_BeginA0(uint8_t* o);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
int32_t __fastcall Company_FreeShares(void* c);
int32_t __fastcall Player_TradeBlock(uint8_t* p, int, uint32_t company, int32_t buying);
void __fastcall CompanyList_Refill(uint8_t* w);
void __fastcall Track_SetKind(uint8_t* o, int, uint8_t kind);
int32_t __fastcall Company_Get7680(void* c);
void __fastcall Tracks_ApplyPendingKinds(Collection* c);
int32_t __fastcall Node_HasLink(void* n, int, int32_t any);
void __fastcall Obj_FreeList1c9(void* o);
float __stdcall Route_GradeFactorHere(float x, float z);
int32_t __fastcall Node_HasFreeLink(void* n, int, int32_t any);
void __fastcall Obj_SetText1c(uint8_t* o, int, const char* s, uint32_t v);
void World_RefreshColors();
void Tracks_UndoPendingEdit();
void __fastcall Train_ResetMotion(uint8_t* t);
void __fastcall TrackPanel_Init(uint8_t* w);
int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);
void* __fastcall Obj_Company21(void* o);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
void __fastcall ProxyFight_Start(uint32_t company);
int32_t __fastcall Obj_PushIdNode(void* o, int, int32_t a, int32_t b);
void __fastcall TrackNet_ShowStatus(uint8_t* t, int, uint32_t piece);
void __fastcall Obj_SetRgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b);
void __fastcall Obj_DecayCounters(uint8_t* o);
void __fastcall Track_ColorSlope(uint8_t* o);

int32_t __fastcall TrackNet_FindOrAddNode(Collection* net, int, float x, float y, float radius, int32_t kind, int32_t* missing, int32_t add, int32_t owner,
                                          int32_t byDegree, int32_t exclude, int32_t skipLinked);

void __fastcall Obj_CopyCorners(uint8_t* o, int, uint8_t* body);
void __fastcall TrackTool_Bulldoze(uint8_t* tool, int, uint32_t, uint32_t);
void __fastcall TrackTool_Suspend(uint8_t* w);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
void __fastcall TrackNet_Undo(Collection* net, int, uint32_t stamp);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE int32_t __stdcall Net_AllHaveVersion(float v);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
RT3_NOINLINE void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
RT3_NOINLINE void __fastcall TrackPanel_Refresh(uint8_t* w);
void __fastcall TrackTool_FinishBuild(uint8_t* w);
void __fastcall Net_SendSettings(uint8_t* w);
RT3_NOINLINE void __fastcall TrackTool_Cancel(uint8_t* w);
int32_t __stdcall Company_TrackBounds(int32_t* out, int32_t company);
RT3_NOINLINE void __fastcall TrackNet_EndBuild(Collection* net);
void __fastcall Table_FillCursor(Collection* t, int, uint8_t* out);
void __fastcall TrackNet_ElectrifyQuote(Collection* net, int, uint32_t company, float* cost, float* share);
RT3_NOINLINE void __fastcall TrackNet_RemovePiece(Collection* net, int, uint32_t id, int32_t force, int32_t from);
RT3_NOINLINE void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
void __fastcall Obj_CameraByOptions(uint8_t* o);
RT3_NOINLINE void __fastcall Tracks_ApplyPendingKinds(Collection* c);
RT3_NOINLINE void __fastcall Obj_FreeList1c9(void* o);
RT3_NOINLINE void __fastcall Track_UpdateColor(uint8_t* o);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
uint8_t __stdcall Obj_Mode1Or4(int32_t);
int32_t __fastcall Tracks_AddPieceRotated(Collection* c, int, float cx, float cy, float angle, int32_t kind, int32_t owner, int32_t style, int32_t force);
void __fastcall Widget_DeleteGeneratedRows(uint8_t* w, int, int32_t);
void __fastcall BuildingRow_AddCargoButtons(uint8_t* w, uint8_t* list, int32_t type, float x, float y, int32_t, int32_t);
void __fastcall StopList_DrawLabel(void* stops, int32_t idx, float x, float y, int32_t, float h);
void __fastcall Map_ShowPickTooltip(uint8_t* o);
void __fastcall Packet_BuyStock(uint32_t* pkt);
void __fastcall Packet_SellStock(uint32_t* pkt);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);

// Dialog callbacks clearing a range of their window's child widgets.
#define RT3_CLEAR_CHILDREN(addr, name, lo, hi) \
    void __fastcall name(uint8_t* w, int, int32_t) { Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, lo, hi, (uint32_t)(uintptr_t)w); } \
    RT3_REPLACE(addr, name);

void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share);
void ProxyFight_Resolve();
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force);
void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t);
uint32_t TrainList_Selected();
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);
void __fastcall TrackList_ResetBuildState(uint8_t* tracks);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Company_InstallChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t);
RT3_NOINLINE int32_t __fastcall Player_TradeBlock(uint8_t* p, int, uint32_t company, int32_t buying);
RT3_NOINLINE void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force);
RT3_NOINLINE void __fastcall TrackNet_ElectrifyCompany(Collection* net, int, uint32_t company, int32_t dryRun, float* cost, float* share);
RT3_NOINLINE void __fastcall TrackNet_ElectrifyQuote(Collection* net, int, uint32_t company, float* cost, float* share);
RT3_NOINLINE int32_t __fastcall TrackNet_FindOrAddNode(Collection* net, int, float x, float y, float radius, int32_t kind, int32_t* missing, int32_t add, int32_t owner, int32_t byDegree, int32_t exclude, int32_t skipLinked);
void __fastcall Tracks_RestoreEdit(Collection* c, int, uint8_t* t);
void __fastcall Tracks_ReplayEdit(Collection* c);
RT3_NOINLINE void __fastcall TrackNet_ShowStatus(uint8_t* t, int, uint32_t piece);
RT3_NOINLINE void __fastcall TrackNet_RemovePiece(Collection* net, int, uint32_t id, int32_t force, int32_t from);
RT3_NOINLINE void __fastcall TrackNet_EndBuild(Collection* net);
RT3_NOINLINE int32_t __fastcall TrackNet_ExtendTo(Collection* net, int, float x, float y, int32_t mode, int32_t company, int32_t kind, float radius, int32_t* out);
RT3_NOINLINE void __fastcall TrackList_ResetBuildState(uint8_t* tracks);
RT3_NOINLINE void __fastcall TrackNet_Undo(Collection* net, int, uint32_t stamp);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
void __fastcall CompanyList_Refill(uint8_t* w);
void __fastcall CompanyList_Reset();
RT3_NOINLINE void __fastcall Packet_BuyStock(uint32_t* pkt);
RT3_NOINLINE void __fastcall Packet_SellStock(uint32_t* pkt);
uint8_t* __fastcall StockBuy_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall Obj_CopyTo6d1a08(const uint32_t* o);
void __fastcall StockWindow_DrawHolders(void*, int, float x, float y, float w, float, int32_t);
RT3_NOINLINE void ProxyFight_Resolve();
RT3_NOINLINE void __fastcall ProxyFight_Start(uint32_t company);
void __fastcall Company_DrawValueCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE uint8_t Global622b0cIs64();
int32_t __stdcall Company_TrackBounds(int32_t* out, int32_t company);
RT3_NOINLINE void __fastcall TrackTool_Cancel(uint8_t* w);
RT3_NOINLINE void __fastcall TrackPanel_Refresh(uint8_t* w);
RT3_NOINLINE void __fastcall TrackTool_Bulldoze(uint8_t* tool, int, uint32_t, uint32_t);
int32_t __fastcall Obj_Flag7dOrTracks118(uint8_t* o);
void __fastcall Net_SendSettings(uint8_t* w);
RT3_NOINLINE void Tracks_UndoPendingEdit();
RT3_NOINLINE void __fastcall TrackTool_Destroy(uint8_t* tool);
void __fastcall TrackPanel_Init(uint8_t* w);
RT3_NOINLINE void __fastcall TrackTool_FinishBuild(uint8_t* w);
RT3_NOINLINE void __fastcall TrackTool_Update(uint8_t* t, int, int32_t active, float release);
RT3_NOINLINE void __fastcall TrackTool_Suspend(uint8_t* w);
RT3_NOINLINE void __fastcall TrackTool_Resume(uint8_t* t);
uint8_t* __fastcall TrackTool_Construct(uint8_t* w);
RT3_NOINLINE int32_t __fastcall TrackTool_HandleEvent(uint8_t* t, int, uint32_t* ev);
RT3_NOINLINE void* Get6d3ae8();
RT3_NOINLINE void* Get6d3ae8b();
void __fastcall TrainList_PickTooltipCb(void*, int, int32_t on, int32_t, int32_t, int32_t, int32_t);
void __fastcall Thunk50f640(uint8_t* p0, int p1, int32_t p2);
void __fastcall BuildingRow_AddCargoButtonsTo6d3ae8(uint8_t* w, int32_t type, int32_t, float x, float y, int32_t a, int32_t b);
void __fastcall StopList_DrawLabelCb(void*, int32_t idx, float x, float y, int32_t a, float h, int32_t);
void __fastcall TrainList_ClearChildren(void*, int, int32_t);
RT3_NOINLINE void __fastcall Panel_ReleaseImages(uint8_t* w);
RT3_NOINLINE void __stdcall SetGlobal6d1aa4(int32_t v);
int32_t Global6d3ae4();
void SetGlobal6d3adc();
RT3_NOINLINE uint32_t TrainList_Selected();
RT3_NOINLINE void __fastcall Editor_SelectionCost(int32_t* total, int32_t* count);
RT3_NOINLINE void UpgradeDialog_UpdateCostText();
void __stdcall List_AddArrowButtons(uint32_t id, float x, float y, int32_t, float h);
void __fastcall LocoList_DrawRow(void*, uint32_t id, float x, float y, float w, float h, int32_t);
void __fastcall TrainBuy_EngineDraw(uint8_t* p, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Scene_PushBusy(uint8_t* s);
RT3_NOINLINE void __fastcall Scene_PopBusy(uint8_t* s);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
RT3_NOINLINE uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);

static inline double D(uint32_t a) { return *(const double*)rt3::Rdata(a); }

static inline double Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }


static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline double BitsD(uint64_t b) { return *(double*)&b; }


// Refreshes the company list window (0x6d19f0) after a stock trade.
static void StockTrade_Refresh() {
    Reset_625a94();
    if (g_stockWindow) Window_DeleteChildren(g_stockWindow, 0, 0x3b60, 0x3e7f, 0);
    Reset_625a94();
    if (g_stockWindow) CompanyList_Refill(g_stockWindow);
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
inline float BlCell(float v) { return (float)((double)v / Fl(0x5c8700)); }
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
    Ui_SendEvent5((uint8_t**)(manager), 0, type, 0x808, 0, 0, 0);
}

// 0x50c130. Clears 0x625a94 and refills the company list window when it is open.
void __fastcall CompanyList_Reset() {
    Reset_625a94();
    if (uint8_t* w = g_stockWindow) CompanyList_Refill(w);
}
RT3_REPLACE(0x50c130, CompanyList_Reset);

// 0x50c150. Stock buy packet {player, company}: the player buys a trade block
// on margin (the local player hears 0x36 and gets the default cursor).
RT3_NOINLINE void __fastcall Packet_BuyStock(uint32_t* pkt) {
    uint8_t* p = (uint8_t*)Collection_Get(g_players, 0, pkt[0]);
    int32_t n = Player_TradeBlock(p, 0, pkt[1], 1);
    if ((uint32_t)Obj_Get25(g_world) == pkt[0]) {
        Ui_PlaySound(0x36, 0, 0x3f000000);
        Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    }
    Player_BuyShares(p, 0, pkt[1], n, 1, 0);
    StockTrade_Refresh();
}
RT3_REPLACE(0x50c150, Packet_BuyStock);

// 0x50c1f0. Stock sell packet {player, company}: as 0x50c150 for a sale
// (sound 0x37).
RT3_NOINLINE void __fastcall Packet_SellStock(uint32_t* pkt) {
    uint8_t* p = (uint8_t*)Collection_Get(g_players, 0, pkt[0]);
    int32_t n = Player_TradeBlock(p, 0, pkt[1], 0);
    if ((uint32_t)Obj_Get25(g_world) == pkt[0]) {
        Ui_PlaySound(0x37, 0, 0x3f000000);
        Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    }
    Player_SellShares(p, 0, pkt[1], n, 1, 0);
    StockTrade_Refresh();
}
RT3_REPLACE(0x50c1f0, Packet_SellStock);

// 0x50c290. Stock buying window constructor ("StockBuy.win", vtable 0x5d18f0):
// remembers the current player's index (0x6d19f4), resets the 64 share cells
// (0x6d18d8, 1000 each), becomes 0x6d19f0, loads its images (0x6d19d8..e0), the
// portraits (0x6d1748, one per pack portrait) and fonts 3 / 2 / 1, installs the
// handlers for buttons 0x3a9f / 0x3aa0 / 0x3aa4, ticks the six filter toggles
// 0x3aad.. from 0x622b04, shows / hides button 0x3a9b for the selection 0x622b00
// (reset when out of range) and fills the company list.
uint8_t* __fastcall StockBuy_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d18f0;
    g_6d19f4 = 0;
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i) {
        void* p = Table_NthField4(g_players, 0, i);
        if (p == Obj_Ref25(g_world)) g_6d19f4 = i;
    }
    for (uint32_t a = 0x6d18d8; a < 0x6d19d8; a += 4) hook::Global<int32_t>(a) = 1000;
    g_stockWindow = w;
    g_6d19d8 = Pack_AcquireNamed(g_pack, 0, (const char*)0x5d190c);
    g_6d19dc = Pack_AcquireNamed(g_pack, 0, (const char*)0x5d0f04);
    hook::As<uint8_t*>(g_6d19e0) = Pack_AcquireNamed(g_pack, 0, (const char*)0x5c9f98);
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(g_pack, 0x429b4)); ++i) {
        Collection* c = F<Collection*>(g_pack, 0x429b4);
        const char* name = (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0)) + 0x20;
        ((uint8_t**)0x6d1748)[i] = Pack_AcquireNamed(g_pack, 0, name);
    }
    g_6d19e4 = Pack_AcquireFont(g_pack, 0, 3);
    g_6d19e8 = Pack_AcquireFont(g_pack, 0, 2);
    g_6d19ec = Pack_AcquireFont(g_pack, 0, 1);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d18fc);
    Window_SendEvent5(w, 0, 0x7a, 0x3a9f, 0x50a730, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x3aa0, 0x50a570, 0, 0);
    Window_SendEvent5(w, 0, 0x82, 0x3aa4, 0x50b910, 0, 0);
    for (int32_t k = 0; k < 6; ++k) Window_SendEvent5(w, 0, 0x74, 0x3aad + k, ((const uint8_t*)0x622b04)[k], 0, 0);
    int32_t sel = g_622b00;
    if (sel >= Table_CountField3f(g_companies)) sel = -1;
    g_622b00 = sel;
    Window_SendEvent5(w, 0, sel != -1 ? 0x6d : 0x86, 0x3a9b, 3, 0, 0);
    Window_DeleteChildren(w, 0, 0x3b60, 0x3e7f, 0);
    CompanyList_Refill(w);
    return w;
}
RT3_REPLACE(0x50c290, StockBuy_Construct);

RT3_NOINLINE void __fastcall Obj_CopyTo6d1a08(const uint32_t* o) {
    for (int k = 0; k < 10; ++k) ((uint32_t*)0x6d1a08)[k] = o[k];
}
RT3_REPLACE(0x50c4e0, Obj_CopyTo6d1a08);

// 0x50c500. Stock window: the share table of company 0x6d1a0c at (x, y, w) in
// font 0x6d1a34: a title (0x272 when more than half the shares 0x6d1a30 of
// its +0x47 are in the first group, else 0x273), then two groups (0x274 /
// 0x275: players with / without the flag 0x6d1a20) listing every player
// holding shares (+0x15d + company x 4) with the count, the group's total
// (string 0x276; 0x6d1a00, or 0x6d1a04 - 0x6d1a00 for the second group,
// right-aligned to 8 places over a blank template) and its shares (0x6d1a30,
// or the rest of +0x47).
void __fastcall StockWindow_DrawHolders(void*, int, float x, float y, float w, float, int32_t) {
    float xs = (float)((double)x + D(0x5d0110));
    float ws = (float)((double)w - D(0x5c9b50));
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, g_6d1a0c);
    const float h = BitsF(0x3ce81b4f);
    char title[0x200], num[0x130];
    strcpy(title, GetString(g_6d1a30 > F<int32_t>(co, 0x47) / 2 ? 0x272 : 0x273));
    Font_DrawTextBox(g_6d1a34, 0, title, xs, y, ws, h, 0xffffffff, 0, 1, 0, 1.0f);
    float cy = (float)((double)y + Fl(0x5d11fc));
    for (int32_t k = 0; k < 2; ++k) {
        if (k == 0) {
            strcpy(title, GetString(0x274));
        } else {
            cy = (float)((double)cy + Fl(0x5d11fc));
            strcpy(title, GetString(0x275));
        }
        Font_DrawTextBox(g_6d1a34, 0, title, xs, cy, ws, h, 0xffffffff, 0, 1, 0, 1.0f);
        cy = (float)((double)cy + Fl(0x5d11fc));
        for (int32_t i = 0; i < Table_CountField4(g_players); ++i) {
            uint8_t* p = (uint8_t*)Table_NthField4(g_players, 0, i);
            int32_t n = F<int32_t>(p, 0x15d + g_6d1a0c * 4);
            if (n <= 0) continue;
            bool flagged = ((const uint8_t*)0x6d1a20)[i] != 0;
            if (k == 0 ? !flagged : flagged) continue;
            Num_Format(num, 1, (double)n, 10, 1, 0, 0);
            Font_DrawTextBox(g_6d1a34, 0, (const char*)(p + 8), xs, cy, ws, h, 0xffffffff, 2, 1, 0, 1.0f);
            Font_DrawTextBox(g_6d1a34, 0, num, xs, cy, ws, h, 0xffffffff, 3, 1, 0, 1.0f);
            cy = (float)((double)cy + Fl(0x5ca2a8));
        }
        int32_t total = k == 0 ? g_6d1a00 : g_6d1a04 - g_6d1a00;
        Num_Format(num, 1, (double)total, 10, 1, 0, 0);
        Font_DrawTextBox(g_6d1a34, 0, GetString(0x276), xs, cy, ws, h, 0xffffffff, 2, 1, 0, 1.0f);
        if (strlen(num) < 8) {
            char padded[0x1c];
            memcpy(padded, (const void*)0x5d11e0, sizeof padded);
            crt::sprintf(padded + 8 - strlen(num), num);
            strcpy(num, padded);
        }
        Font_DrawTextBox(g_6d1a34, 0, num, xs, cy, ws, h, 0xffffffff, 3, 1, 1, 1.0f);
        cy = (float)((double)cy + Fl(0x5ca2a8));
        int32_t shares = k == 0 ? g_6d1a30 : F<int32_t>(co, 0x47) - g_6d1a30;
        Num_Format(num, 1, (double)shares, 10, 1, 0, 0);
        Font_DrawTextBox(g_6d1a34, 0, num, xs, cy, ws, h, 0xffffffff, 3, 1, 0, 1.0f);
    }
}
RT3_REPLACE(0x50c500, StockWindow_DrawHolders);

// 0x50c940. Proxy fight for company 0x6d1a0c by player 0x6d1a0e, once every
// player is ready (0x6d1a10): the challenger's pull is 0x5c8c20 less the
// company's rating (/ 0x5c88a8) plus his own (/ 0x5cab00) and SimRand
// -10..10 %, clamped to 0..1; it wins him that share of the free stock (in
// thousands, 0x6d1a00), and each AI holder not chairing the company sides
// with him (0x6d1a20) at another -20..20 % swing; with more than half of all
// shares (0x6d1a30 vs +0x47 / 2) he becomes chairman, else the try is dated
// (+0x289). The result is shown (0xbf4 / 0xbf5 in 0xc0a in multiplayer).
RT3_NOINLINE void ProxyFight_Resolve() {
    int32_t waiting = 0;
    for (int32_t i = 0; i < Collection_Count(g_players); ++i)
        if (!*(uint8_t*)(0x6d1a10 + i)) waiting = 1;
    if (waiting) return;
    g_6d1a30 = 0;
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, g_6d1a0c);
    if (!F<int32_t>(co, 0x3f)) return;
    uint8_t* pl = (uint8_t*)Collection_Get(g_players, 0, g_6d1a0e);
    g_6d1a04 = Company_FreeShares(co);
    float pull = (float)(D(0x5c8c20) - (double)F<int32_t>(co, 0x34b) / D(0x5c88a8));
    pull = (float)((double)F<int32_t>(pl, 0x28d) / D(0x5cab00) + pull);
    pull = (float)((double)SimRand(-10, 10) / D(0x5c85d8) + pull);
    if (pull > D(0x5c8770)) pull = 1.0f;
    else if (pull < g_5c8598) pull = 0.0f;
    int32_t won = Ftol((double)(float)g_6d1a04 * pull) / 1000 * 1000;
    g_6d1a30 += won;
    g_6d1a00 = won;
    for (int32_t i = 0; i < Table_CountField4(g_players); ++i) {
        uint8_t* p = (uint8_t*)Table_NthField4(g_players, 0, i);
        uint8_t* side = (uint8_t*)(0x6d1a20 + i);
        if (!F<int32_t>(p, 0x1e1) && F<int32_t>(p, 0x15d + g_6d1a0c * 4) > 0) {
            if (F<int32_t>(p, 0x1dd) != F<int32_t>(co, 0)) {
                float x = (float)((double)SimRand(-20, 20) / D(0x5c85d8) + pull);
                float roll = (float)SimRand(0, 100);
                *side = (double)x * Fl(0x5c8c28) > roll ? 1 : 0;
            } else {
                *side = 0;
            }
        }
        if (*side) {
            int32_t held = F<int32_t>(p, 0x15d + g_6d1a0c * 4);
            if (held > 0) g_6d1a30 += held;
        }
    }
    int32_t win = g_6d1a30 > F<int32_t>(co, 0x47) / 2 ? 1 : 0;
    if (!IsMultiplayer()) {
        static const uint32_t kExtraH = 0x3ed55555;
        Dialog_RunMessage((const char*)0x5c87a8, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0x50c500, *(const float*)&kExtraH, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    } else {
        const char* verdict = GetString(win ? 0xbf4 : 0xbf5);
        uint8_t* who = (uint8_t*)Collection_Get(g_players, 0, g_6d1a0e);
        uint8_t* what = (uint8_t*)Collection_Get(g_companies, 0, g_6d1a0c);
        char buf[0x100];
        FormatText(buf, GetString(0xc0a), (const char*)what + 4, (const char*)who + 8, verdict, nullptr, nullptr, nullptr, nullptr, nullptr);
        Net_QueueText(g_session, 0, buf, 1, 0, 0, 0);
    }
    if (win) {
        Company_InstallChairman((int32_t*)co, 0, g_6d1a0e);
        return;
    }
    int32_t today = F<int32_t>(g_world, 0x15);
    F<int32_t>(Collection_Get(g_players, 0, g_6d1a0e), 0x289) = today;
}
RT3_REPLACE(0x50c940, ProxyFight_Resolve);

// 0x50ccc0 (company id in ECX). The local player challenges company
// `company`'s chairman: with at most 0x5c86b8 of its shares he is told his
// stake (0x26f, below 10 %); within 0x76200 of his last try he must wait
// (0x270); otherwise, once confirmed (0x271 answered 0x3f2), the vote runs
// at once in a single-player game (everyone ready, ProxyFight_Resolve) or
// is sent to the host (type 0x4a; 0xc09 logged). Two fonts are held over it.
// QUIRK: the 10-byte message's last bytes are never written by the original
// (6..7 keep the high bytes of the stake test's double, 8..9 stack residue,
// sent as 0 here).
RT3_NOINLINE void __fastcall ProxyFight_Start(uint32_t company) {
    uint8_t* me = (uint8_t*)Obj_Ref25(hook::As<void*>(g_world));
    int32_t held = F<int32_t>(me, 0x15d + company * 4);
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, company);
    double dheld = (double)held;
    if ((double)F<int32_t>(co, 0x47) * D(0x5c86b8) > dheld) {
        int32_t pct = held * 100 / F<int32_t>(co, 0x47);
        if (pct >= 10) pct = 9;
        char num[0x40], text[0x400];
        const char* s = Num_Format(num, 1, (double)pct, 0xa, 1, 0, 0);
        FormatText(text, GetString(0x26f), s, (const char*)co + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        return;
    }
    if (F<int32_t>(Obj_Ref25(hook::As<void*>(g_world)), 0x289)) {
        int32_t now = F<int32_t>(g_world, 0x15);
        if (now - F<int32_t>(Obj_Ref25(hook::As<void*>(g_world)), 0x289) < 0x76200) {
            Dialog_RunMessage(GetString(0x270), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
            return;
        }
    }
    if (Dialog_RunMessage(GetString(0x271), 3, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0) != 0x3f2) return;
    g_6d1a34 = Pack_AcquireFont(g_pack, 0, 2);
    g_6d1a38 = Pack_AcquireFont(g_pack, 0, 1);
    if (!IsMultiplayer()) {
        memset((void*)0x6d1a08, 0, 0x28);
        g_6d1a0c = (uint16_t)company;
        g_6d1a0e = (uint16_t)Obj_Get25(hook::As<void*>(g_world));
        for (int i = 0; i < 0x10; ++i) *(uint8_t*)(0x6d1a10 + i) = 1, *(uint8_t*)(0x6d1a20 + i) = 1;
        ProxyFight_Resolve();
    } else {
        uint8_t m[0xa];
        F<int32_t>(m, 0) = Global6d40e4();
        m[4] = (uint8_t)company;
        m[5] = (uint8_t)Obj_Get25(hook::As<void*>(g_world));
        memcpy(m + 6, (const uint8_t*)&dheld + 6, 2);
        m[8] = 0, m[9] = 0;
        uint8_t* s = g_session;
        Net_Send(s, 0, (const char*)s + 0x8f48, 0x4a, 0xa, m, 5, 0);
        Log_Add(g_messageLog, 0, GetString(0xc09), 0xb, 0, 0);
    }
    Pack_Release(g_pack, 0, g_6d1a34);
    Pack_Release(g_pack, 0, g_6d1a38);
}
RT3_REPLACE(0x50ccc0, ProxyFight_Start);

// 0x50cf90 (draw callback). Draws the player's company's +0x7680 value
// (string 0x266) in the world's font (+0x46a98 in modes 1 / 4, else
// +0x46a94).
void __fastcall Company_DrawValueCb(void*, int, float x, float y, float w, float h, int32_t) {
    uint8_t* world = g_world;
    if (!world || !g_companies) return;
    if (!Collection_IsValid(g_companies, 0, Obj_Get21(world))) return;
    const int32_t v = Company_Get7680(Obj_Company21(g_world));
    char num[0x40], text[0x12c];
    const char* s = Num_Format(num, 1, (double)v, 10, 1, 0, 0);
    FormatText(text, GetString(0x266), s, 0, 0, 0, 0, 0, 0, 0);
    const int32_t mode = g_language;
    uint8_t* font = mode == 1 || mode == 4 ? F<uint8_t*>(g_world, 0x46a98) : F<uint8_t*>(g_world, 0x46a94);
    Font_DrawTextBox(font, 0, text, x, y, w, h, 0xffffffff, 0, 0, 0, 1.0f);
}
RT3_REPLACE(0x50cf90, Company_DrawValueCb);

RT3_NOINLINE uint8_t Global622b0cIs64() { return g_622b0c == 0x40 ? 1 : 0; }
RT3_REPLACE(0x50d0a0, Global622b0cIs64);

// 0x50d0b0. Cell bounds {minX, maxX, minY, maxY} of the company's plain
// track pieces (+0x216 == 0); returns how many there are.
RT3_NOINLINE int32_t __stdcall Company_TrackBounds(int32_t* out, int32_t company) {
    Collection* nodes = g_tracks;
    float minX = 1000000.0f, maxX = -1000000.0f, minY = 1000000.0f, maxY = -1000000.0f;
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(nodes); ++i) {
        uint8_t* t = (uint8_t*)Collection_Get(nodes, 0, Collection_IdAt(nodes, 0, i, 0));
        if (t[0x216] || F<uint16_t>(t, 0x22e) != company) continue;
        ++n;
        float x, y;
        Obj_CenterCell(t, 0, &x, &y);
        if (minX > x) minX = x;
        if (maxX < x) maxX = x;
        if (minY > y) minY = y;
        if (maxY < y) maxY = y;
    }
    if (n) {
        out[0] = (int32_t)minX;
        out[1] = (int32_t)maxX;
        out[2] = (int32_t)minY;
        out[3] = (int32_t)maxY;
    }
    return n;
}
RT3_REPLACE(0x50d0b0, Company_TrackBounds);

// 0x50d1e0. Track tool w: cancels the build. Applies the pending kinds, frees
// the network's list, removes the tool's piece +0x78 (when unplaced) - or,
// while the network keeps a build (+0x118), every unplaced piece - and pops
// the scene's busy state when set (+0x3741).
RT3_NOINLINE void __fastcall TrackTool_Cancel(uint8_t* w) {
    Tracks_ApplyPendingKinds(g_tracks);
    Obj_FreeList1c9(g_tracks);
    Collection* net = g_tracks;
    if (!F<int32_t>(net, 0x118)) {
        int32_t id = F<int32_t>(w, 0x78);
        if (id != -1 && !((uint8_t*)Collection_Get(net, 0, (uint32_t)id))[0x216]) {
            Collection* c = g_tracks;
            ((void(__fastcall*)(Collection*, int, uint32_t))(*(void***)c)[2])(c, 0, F<uint32_t>(w, 0x78));
        }
    } else {
        for (int32_t i = 0; i < Collection_Count(g_tracks); ++i) {
            Collection* c = g_tracks;
            if (((uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0)))[0x216]) continue;
            uint32_t id = Collection_IdAt(g_tracks, 0, (uint32_t)i, 0);
            TrackNet_RemovePiece(g_tracks, 0, id, 1, -1);
        }
    }
    F<int32_t>(w, 0x78) = -1;
    if (F<int32_t>((uint8_t*)Engine_Renderer(g_engine), 0x3741))
        Scene_PopBusy((uint8_t*)Engine_Renderer(g_engine));
}
RT3_REPLACE(0x50d1e0, TrackTool_Cancel);

// 0x50d2d0. Refreshes the track-laying panel `w` for the tool 0x622b0c: the
// button 0x9873 (handler 0x50cf90) by world +0x4aaf; the track type radios
// (0x9861.. before 1865, 0x9863.. later; options +0x138 falls back to 3 when
// the kind isn't offered); 0x9858 (off in multiplayer and for tool 0x40, else
// by 0x6cfca8 +0x98); the tool radios 0x985e..0x9860 / 0x9875 / 0x9876 by tool
// (1, 4, 0x40) and whether everyone runs version 1.03 in multiplayer; the
// option labels 0x986e / 0x986d / 0x9870 / 0x9871; the cursor (0x15 with the
// brush for tool 0x40, else 4) and the scene's busy state.
RT3_NOINLINE void __fastcall TrackPanel_Refresh(uint8_t* w) {
    Window_SendEvent5(w, 0, 0x7a, 0x9873, 0x50cf90, 0, 0);
    Window_SendEvent5(w, 0, F<int32_t>(g_world, 0x4aaf) ? 0x65 : 0x87, 0x9873, 3, 0, 0);
    uint32_t radio;
    uint8_t* opts = g_app;
    if (F<uint16_t>(g_world, 0xd) < 0x749) {
        radio = 0x9861;
        int32_t m = F<int32_t>(opts, 0x138);
        if (m == 0 || m == 2) F<int32_t>(opts, 0x138) = 3;
    } else {
        radio = 0x9863;
        if (F<int32_t>(opts, 0x138) == 2) F<int32_t>(opts, 0x138) = 3;
    }
    if (IsMultiplayer() || g_622b0c == 0x40) {
        Window_SendEvent5(w, 0, 0x87, 0x9858, 3, 0, 0);
    } else {
        Window_SendEvent5(w, 0, 0x65, 0x9858, 1, 0, 0);
        uint32_t t = Obj_Has98(hook::As<uint8_t*>(g_tracks)) ? 0x65 : 0x87;
        Window_SendEvent5(w, 0, t, 0x9858, 2, 0, 0);
    }
    Window_SendEvent5(w, 0, F<int32_t>(g_app, 0x138) == 3 ? 0x65 : 0x87, radio, 1, 0, 0);
    Window_SendEvent5(w, 0, F<int32_t>(g_app, 0x138) == 1 ? 0x65 : 0x87, radio + 1, 1, 0, 0);
    if (F<uint16_t>(g_world, 0xd) >= 0x749)
        Window_SendEvent5(w, 0, F<int32_t>(g_app, 0x138) == 0 ? 0x65 : 0x87, radio + 2, 1, 0, 0);
    auto send = [&](uint32_t type, uint32_t id) { Window_SendEvent5(w, 0, type, id, 1, 0, 0); };
    switch (g_622b0c) {
    case 0x40:
        send(0x87, 0x985e);
        send(0x87, 0x985f);
        send(0x65, 0x9860);
        break;
    case 4:
        if (IsMultiplayer() && !Net_AllHaveVersion(BitsF(0x3f83d70a))) {
            send(0x87, 0x9875);
            send(0x65, 0x9876);
        } else {
            send(0x87, 0x985e);
            send(0x65, 0x985f);
            send(0x87, 0x9860);
        }
        break;
    case 1:
        if (IsMultiplayer() && !Net_AllHaveVersion(BitsF(0x3f83d70a))) {
            send(0x65, 0x9875);
            send(0x87, 0x9876);
        } else {
            send(0x65, 0x985e);
            send(0x87, 0x985f);
            send(0x87, 0x9860);
        }
        break;
    }
    auto level = [](int32_t v) { return v < 1 ? 0x267 : v < 2 ? 0x268 : v == 2 ? 0x269 : 0x26a; };
    Widget_FindChild(w, 0, 0x986e);
    Window_SendEvent5(w, 0, 0x6f, 0x986e, (uint32_t)(uintptr_t)GetString(F<int32_t>(g_app, 0x144) ? 7 : 8), 0, 0);
    Widget_FindChild(w, 0, 0x986d);
    Window_SendEvent5(w, 0, 0x6f, 0x986d, (uint32_t)(uintptr_t)GetString(F<int32_t>(g_world, 0x4c74) ? 7 : 8), 0, 0);
    Widget_FindChild(w, 0, 0x9870);
    Window_SendEvent5(w, 0, 0x6f, 0x9870, (uint32_t)(uintptr_t)GetString(level(F<int32_t>(g_app, 0x140))), 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x9871, (uint32_t)(uintptr_t)GetString(level(F<int32_t>(g_app, 0x13c))), 0, 0);
    if (g_622b0c == 0x40) {
        Cursor_Set(g_cursorManager, 0, 0x15, 0, 0);
        if (!F<int32_t>(Engine_Renderer(g_engine), 0x3741)) Scene_PushBusy((uint8_t*)Engine_Renderer(g_engine));
        Scene_SetBrushRadius((uint8_t*)Engine_Renderer(g_engine), 0, 0.5f);
    } else {
        Cursor_Set(g_cursorManager, 0, 4, 0, 0);
        if (F<int32_t>(Engine_Renderer(g_engine), 0x3741)) Scene_PopBusy((uint8_t*)Engine_Renderer(g_engine));
    }
}
RT3_REPLACE(0x50d2d0, TrackPanel_Refresh);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x50d740. Price the hovered bulldoze area, or apply/send a drag operation,
// update the track-dirty flag, and show its cost/error tooltip. RET 8 cleans
// two unused caller arguments omitted by Ghidra's inferred signature.
RT3_NOINLINE void __fastcall TrackTool_Bulldoze(uint8_t* tool, int, uint32_t, uint32_t) {
    if (F<uint32_t>(tool, 0x83)) return;
    float x, y;
    Scene_FocusFlagIs1(&x, &y);
    if (tool[0x7d]) {
        float distance = (float)Distance2D(
            x, y, hook::As<float>(g_622b1c), hook::As<float>(g_622b20));
        if ((double)distance <= *(const double*)rt3::Rdata(0x5c97c0)) return;
        // The original copies these with MOV, preserving signaling NaN bits.
        g_622b20 = *(uint32_t*)&y;
        g_622b1c = *(uint32_t*)&x;
    } else {
        g_622b20 = 0xbf800000;
        g_622b1c = 0xbf800000;
    }
    bool preview = tool[0x7d] == 0;
    if (!preview && !IsMultiplayer()) {
        TrackList_ResetBuildState(hook::As<uint8_t*>(g_tracks));
        Train_ResetMotion((uint8_t*)(hook::As<void*>(g_tracks)));
    }
    int32_t count = Collection_Count((Collection*)(hook::As<void*>(g_tracks)));
    int32_t result = 0;
    float cost;
    char message[1300];
    if (preview || !IsMultiplayer()) {
        uint32_t company = ((uint32_t)Obj_Get21(hook::As<void*>(g_world)));
        result = Map_Bulldoze((uint8_t*)(hook::As<void*>(g_map)), 0, x, y, 0.5f, company, preview, &cost, message);
    } else {
        F<uint32_t>(tool, 0x83) = 1;
        Cursor_Set((uint8_t*)(hook::As<void*>(g_cursorManager)), 0, 6, 0, 0);
        uint32_t company = ((uint32_t)Obj_Get21(hook::As<void*>(g_world))) & 0xff;
        uint32_t frame = ((uint32_t)Global6d40e4());
        uint32_t packet[4] = { frame, company, *(uint32_t*)&x, *(uint32_t*)&y };
        uint8_t* session = g_session;
        Net_Send(
            session, 0, (const char*)session + 0x8f48, 0x77, 16, (const uint8_t*)packet, 5, 0);
        tool[0x7d] = 0;
        message[0] = 0;
    }
    if (!preview && count != Collection_Count((Collection*)(hook::As<void*>(g_tracks))))
        g_6d1a90 = 1;
    if (result) {
        const char* format = (const char*)0x5c87a8;
        const char* number = nullptr;
        char buffer[256];
        if (preview && !g_newGame[0x82]) {
            number = Num_FormatSimple(buffer, 9, (double)cost, 11);
            format = GetString(0x26e);
        }
        FormatText(
            message, format, number, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    Ui_ShowTooltip((uint8_t*)(hook::As<void*>(g_windowManager)), 0, message, 1, 0, -1);
}
RT3_REPLACE(0x50d740, TrackTool_Bulldoze);

int32_t __fastcall Obj_Flag7dOrTracks118(uint8_t* o) {
    return o[0x7d] || F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x118) ? 1 : 0;
}
RT3_REPLACE(0x50d9b0, Obj_Flag7dOrTracks118);

// 0x50d9d0. Marks window w sent (+0x83), shows the busy cursor and sends the
// game settings record 0x6d1a40 (0x4b bytes: engine value 0x6d40e4, world flag
// +0x21, 0x622b0c, three values of 0x62be68 +0x1c.., four options of 0x6cec74,
// the world byte +0x4c74) to the player named at +0x8f48, reliably, as type 0x14.
RT3_NOINLINE void __fastcall Net_SendSettings(uint8_t* w) {
    F<int32_t>(w, 0x83) = 1;
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    g_6d1a44 = (uint8_t)Obj_Get21(hook::As<void*>(g_world));
    g_6d1a45 = hook::As<uint8_t>(g_622b0c);
    g_6d1a40 = Global6d40e4();
    uint8_t* v = g_62be68;
    g_6d1a62 = F<uint32_t>(v, 0x1c);
    g_6d1a66 = F<uint32_t>(v, 0x20);
    g_6d1a6a = F<uint32_t>(v, 0x24);
    uint8_t* opt = g_app;
    g_6d1a86 = F<uint8_t>(opt, 0x140);
    g_6d1a87 = F<uint8_t>(opt, 0x13c);
    g_6d1a88 = F<uint8_t>(opt, 0x138);
    g_6d1a89 = F<uint8_t>(opt, 0x144);
    g_6d1a8a = F<uint8_t>(g_world, 0x4c74);
    uint8_t* s = g_session;
    g_6d1a82 = 0;
    Net_Send(s, 0, (const char*)s + 0x8f48, 0x14, 0x4b, (const uint8_t*)0x6d1a40, 5, 0);
}
RT3_REPLACE(0x50d9d0, Net_SendSettings);

// 0x50dab0. Undoes the pending track edit (piece table 0x6cfca8 with +0x118 set and
// a live piece +0x125): restores that piece, recolours it and replays the record.
RT3_NOINLINE void Tracks_UndoPendingEdit() {
    Collection* c = g_tracks;
    if (!F<int32_t>(c, 0x118) || !Collection_IsValid(c, 0, F<uint32_t>(c, 0x125))) return;
    uint8_t* p = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x125));
    Tracks_RestoreEdit(g_tracks, 0, p);
    Track_UpdateColor(p);
    Tracks_ReplayEdit(g_tracks);
}
RT3_REPLACE(0x50dab0, Tracks_UndoPendingEdit);

// Reverse engineered by GPT6.1 SOL MEDIUM.
// 0x50db10. Destroy the track tool: stop its gesture, clear preview tracks,
// restore map/tool flags and overlays, then run the base window destructor.
RT3_NOINLINE void __fastcall TrackTool_Destroy(uint8_t* tool) {
    F<uint32_t>(tool, 0) = 0x5d191c;
    Method(0x50d1e0, tool);
    TrackList_ResetBuildState(hook::As<uint8_t*>(g_tracks));
    hook::As<uint32_t>(g_trackTool) = 0;
    Obj_SetText1c((uint8_t*)(hook::As<void*>(g_cursorManager)), 0, (const char*)0x5c87a8, (uint32_t)(-1));
    if (F<uint32_t>(hook::As<void*>(g_app), 0x263) && !g_6d1a94) {
        void* map = hook::As<void*>(g_map);
        F<uint32_t>(map, 0x168d) = F<uint32_t>(map, 0x1689);
    }
    if (F<uint32_t>(hook::As<void*>(g_app), 0x2cc) &&
        F<uint32_t>(hook::As<void*>(g_map), 0x2175) == 0x15) {
        if (!Obj_Byte21c5(hook::As<void*>(g_map)))
            Method(0x453510, hook::As<void*>(g_map));
    }
    Method(0x4361d0, hook::As<void*>(g_world));
    F<uint32_t>(tool, 0x83) = 0;
    if (F<uint32_t>(hook::As<void*>(g_app), 0x309))
        Method(0x491880, hook::As<void*>(g_tracks));
    if (g_6d1a90) Notify(hook::As<void*>(g_windowManager), 0xba);
    Method(0x53f7d0, tool);
}
RT3_REPLACE(0x50db10, TrackTool_Destroy);

// 0x50dc00. Track panel setup: of the track type buttons 0x986a..0x986c only
// the era's one is shown (0x986a before 1865, 0x986b later; 0x6d shows, 0x86
// hides), 0x9872 from 1904; then the panel is refreshed.
RT3_NOINLINE void __fastcall TrackPanel_Init(uint8_t* w) {
    int32_t keep = F<uint16_t>(g_world, 0xd) >= 0x749 ? 1 : 0;
    for (int32_t i = 0; i < 3; ++i) Window_SendEvent5(w, 0, i != keep ? 0x86 : 0x6d, 0x986a + (uint32_t)i, 3, 0, 0);
    Window_SendEvent5(w, 0, F<uint16_t>(g_world, 0xd) < 0x770 ? 0x86 : 0x6d, 0x9872, 3, 0, 0);
    TrackPanel_Refresh(w);
}
RT3_REPLACE(0x50dc00, TrackPanel_Init);

// 0x50dc80. Track tool w: finishes a pending build (network +0x118): undoes
// the pending edit, then either ends the build or - when the tool keeps it
// (+0x82, cleared) - only drops the build flag and the status text.
RT3_NOINLINE void __fastcall TrackTool_FinishBuild(uint8_t* w) {
    if (!F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x118)) return;
    Tracks_UndoPendingEdit();
    if (!w[0x82]) {
        TrackNet_EndBuild(g_tracks);
        return;
    }
    w[0x82] = 0;
    F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x118) = 0;
    Obj_SetText1c(g_cursorManager, 0, nullptr, 0xffffffff);
}
RT3_REPLACE(0x50dc80, TrackTool_FinishBuild);

// 0x50dce0 (thiscall, RET 8). Track tool t, each frame while the player's
// company exists: the hover node shown last frame (+0x78, its kind +0x7c)
// is put back. While `active`, at the focused cell: before a drag (network
// 0x6cfca8 +0x118 clear) the node under the cursor (or a new piece, radius
// by the grade there x 0x5d1928) is shown with the network's link mode
// +0x11c and highlighted; during a drag a lost anchor is re-placed (when
// flagged, +0x82) and the line extended (TrackNet_ExtendTo); a failed one
// ends the drag (0x4955e0, cursor text, 0x50dab0), a live one shades the
// anchor by slope. Inactive, `release` cancels a drag's trial pieces.
RT3_NOINLINE void __fastcall TrackTool_Update(uint8_t* t, int, int32_t active, float release) {
    Collection* const tracks = g_tracks;
    uint8_t* const net = (uint8_t*)tracks;
    if (!Collection_IsValid(g_companies, 0, (uint32_t)Obj_Get21(g_world))) return;
    if (F<int32_t>(t, 0x78) != -1) {
        if (!t[0x7c]) BlVirt2(g_tracks, F<int32_t>(t, 0x78));
        else Track_SetKind((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x78)), 0, t[0x7c]);
        F<int32_t>(t, 0x78) = -1;
    }
    if (!active) {
        Obj_SetText1c(g_cursorManager, 0, nullptr, 0xffffffff);
        if (*(uint32_t*)&release && F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x118)) {
            TrackNet_RemovePiece(g_tracks, 0, F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x125), 1, -1);
            TrackNet_RemovePiece(g_tracks, 0, F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x129), 1, -1);
            t[0x82] = 1;
        }
        return;
    }
    float x, y;
    if (Scene_FocusFlagIs1(&x, &y)) {
        if (!F<int32_t>(net, 0x118)) {
            const float r = (float)((double)Route_GradeFactorHere(x, y) * Fl(0x5d1928));
            const int32_t id = TrackNet_FindOrAddNode(tracks, 0, x, y, r, g_622b0c, &active, 1,
                                                      Obj_Get21(g_world), 1, -1, 0);
            F<int32_t>(t, 0x78) = id;
            if (id == -1) {
                TrackNet_ShowStatus(net, 0, 0xffffffff);
                return;
            }
            uint8_t* p = (uint8_t*)Collection_Get(tracks, 0, (uint32_t)id);
            if (!Node_HasFreeLink(p, 0, 1)) active = 1;
            t[0x7c] = p[0x216];
            if (!active) {
                if (!Node_HasLink(p, 0, 0)) net[0x11c] = p[0x216] ? 3 : 0;
                else net[0x11c] = Node_HasFreeLink(p, 0, 0) ? 1 : 2;
                Obj_SetRgb(p, 0, 0, 0xff, 0);
            } else {
                net[0x11c] = 0;
            }
            TrackNet_ShowStatus(net, 0, F<uint32_t>(t, 0x78));
            return;
        }
        if (!Collection_IsValid(tracks, 0, F<uint32_t>(net, 0x125))) t[0x82] = 1;
        if (t[0x82]) {
            t[0x82] = 0;
            if (!Collection_IsValid(tracks, 0, F<uint32_t>(net, 0x125))) {
                const float ax = F<float>(net, 0x11d), ay = F<float>(net, 0x121);
                const int32_t style = ExtendStyle(net, g_622b0c);
                F<int32_t>(net, 0x125) = Tracks_AddPieceRotated(
                    tracks, 0, ax, ay, 0.0f, 0, Obj_Get21(g_world), style, 0);
            }
        }
        const float r = Route_GradeFactorHere(x, y);
        const int32_t kind = g_622b0c;
        if (TrackNet_ExtendTo(g_tracks, 0, x, y, 0, Obj_Get21(g_world), kind, r, nullptr) == -1) {
            if (F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x118)) Obj_DecayCounters(hook::As<uint8_t*>(g_tracks));
            Obj_SetText1c(g_cursorManager, 0, nullptr, 0xffffffff);
            Tracks_UndoPendingEdit();
            F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x118) = 0;
        }
        uint8_t* n2 = hook::As<uint8_t*>(g_tracks);
        if (F<int32_t>(n2, 0x118) && Collection_IsValid(g_tracks, 0, F<uint32_t>(n2, 0x125)))
            Track_ColorSlope((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x125)));
    }
    TrackNet_ShowStatus(hook::As<uint8_t*>(g_tracks), 0, F<uint32_t>(t, 0x78));
}
RT3_REPLACE(0x50dce0, TrackTool_Update);

// 0x50e070. Track tool w: suspends the build (nested: only the first call,
// counter +0x87). In the world editor (0x622b0c 0x40) only refreshes the
// colours. Otherwise saves the network's build state (+0x94 / +0x98 /
// +0x88 into +0x9f / +0xa3 / +0x9b, the hovered piece +0x12d when valid,
// +0x11c, +0x118, the build piece +0x125 when placed, +0x11d / +0x121),
// resets it, undoes the pending edit and cancels the tool.
RT3_NOINLINE void __fastcall TrackTool_Suspend(uint8_t* w) {
    if (++F<int32_t>(w, 0x87) > 1) return;
    if (g_622b0c == 0x40) {
        World_RefreshColors();
        return;
    }
    F<uint32_t>(w, 0x9f) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x94);
    F<uint32_t>(w, 0xa3) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x98);
    F<uint32_t>(w, 0x9b) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x88);
    F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x94) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x90);
    F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x98) = 0;
    F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x88) = -1;
    F<int32_t>(w, 0xab) = -1;
    if (Collection_IsValid(g_tracks, 0, F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x12d)))
        F<uint32_t>(w, 0xab) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x12d);
    w[0xaf] = hook::As<uint8_t*>(g_tracks)[0x11c];
    Tracks_UndoPendingEdit();
    F<uint32_t>(w, 0x8b) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x118);
    F<uint32_t>(w, 0x8f) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x125);
    if (Collection_IsValid(g_tracks, 0, F<uint32_t>(w, 0x8f)) &&
        !((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(w, 0x8f)))[0x216])
        F<int32_t>(w, 0x8f) = -1;
    F<uint32_t>(w, 0x93) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x11d);
    F<uint32_t>(w, 0x97) = F<uint32_t>(hook::As<uint8_t*>(g_tracks), 0x121);
    TrackTool_Cancel(w);
}
RT3_REPLACE(0x50e070, TrackTool_Suspend);

// 0x50e1e0. Track tool t resumes after a pause (+0x87 counts down): the
// bulldozer (tool kind 0x622b0c 0x40) refocuses and bulldozes again (by the
// pending action +0x7e: 0x7d6 one way, others the other); otherwise the
// network's builder is reset and, for a saved drag (+0x8b), its anchor
// (+0x8f, or a new node at (+0x93, +0x97) within 0x622b18 x 0x5c8888, kept
// only when new) restored with the drag state and link mode (+0xaf); in
// multiplayer the width goes to 0x6d1a56. The saved build options (+0x9f,
// and single-player +0xa3 / +0x9b) are restored and the tool updated.
RT3_NOINLINE void __fastcall TrackTool_Resume(uint8_t* t) {
    if (--F<int32_t>(t, 0x87) > 0) return;
    if (g_622b0c == 0x40) {
        g_6d1a98 = Scene_FocusFlagIs1((float*)0x622b10, (float*)0x622b14);
        const int32_t a = F<int32_t>(t, 0x7e);
        TrackTool_Bulldoze(t, 0, a == 0x7d6 ? 1 : 0, a != 0x7d6 && a != 0 ? 1 : 0);
        return;
    }
    Train_ResetMotion(hook::As<uint8_t*>(g_tracks));
    F<int32_t>(t, 0x78) = -1;
    if (F<int32_t>(t, 0x8b)) {
        if (Collection_IsValid(g_tracks, 0, F<uint32_t>(t, 0x8f))) {
            F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x125) = F<int32_t>(t, 0x8f);
        } else {
            int32_t missing;
            const float r = (float)((double)g_622b18 * D(0x5c8888));
            F<int32_t>(t, 0x8f) = TrackNet_FindOrAddNode(g_tracks, 0, F<float>(t, 0x93), F<float>(t, 0x97), r,
                                                         g_622b0c, &missing, 1, Obj_Get21(g_world), 1, -1, 0);
            if (!missing) F<int32_t>(t, 0x8f) = -1;
        }
        if (Collection_IsValid(g_tracks, 0, F<uint32_t>(t, 0x8f))) {
            uint8_t* n = hook::As<uint8_t*>(g_tracks);
            F<int32_t>(n, 0x118) = F<int32_t>(t, 0x8b);
            F<uint32_t>(n, 0x11d) = F<uint32_t>(t, 0x93);
            F<uint32_t>(n, 0x121) = F<uint32_t>(t, 0x97);
            Obj_CopyCorners(hook::As<uint8_t*>(g_tracks), 0, (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x8f)));
        }
        F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x125) = F<int32_t>(t, 0x8f);
        hook::As<uint8_t*>(g_tracks)[0x11c] = t[0xaf];
        if (IsMultiplayer()) g_6d1a56 = hook::As<uint32_t>(g_622b18);
    }
    F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x94) = F<int32_t>(t, 0x9f);
    if (!IsMultiplayer()) {
        F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x98) = F<int32_t>(t, 0xa3);
        F<int32_t>(hook::As<uint8_t*>(g_tracks), 0x88) = F<int32_t>(t, 0x9b);
    }
    TrackTool_Update(t, 0, 1, 0.0f);
}
RT3_REPLACE(0x50e1e0, TrackTool_Resume);

// 0x50e400. Constructs the track laying tool window w (TrackLay.win, 0x6d1a8c):
// stops following, resets its state and the network's motion, sets the
// button states (multiplayer games with an older peer get the classic set,
// a world-editor screen there falls back to 1), initialises the panel,
// remembers the map's track allowance (0x6d1a94; unlimited with options
// +0x263), shows the track overview (+0x2cc, unless already +0x2175) and
// starts the camera mode (+0x309).
uint8_t* __fastcall TrackTool_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d191c;
    g_trackTool = w;
    g_6d1a90 = 0;
    World_SetFollow(g_world, 0, 0, nullptr);
    F<int32_t>(w, 0x78) = -1;
    w[0x7d] = 0;
    w[0x82] = 0;
    F<int32_t>(w, 0x87) = 0;
    Train_ResetMotion(hook::As<uint8_t*>(g_tracks));
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d192c);
    if (IsMultiplayer() && Net_AllHaveVersion(BitsF(0x3f83d70a))) {
        Window_SendEvent5(w, 0, 0x86, 0x9874, 3, 0, 0);
        for (int32_t i = 0; i < 3; ++i) Window_SendEvent5(w, 0, 0x65, 0x985e + i, 3, 0, 0);
    } else {
        if (IsMultiplayer() && g_622b0c == 0x40) g_622b0c = 1;
        Window_SendEvent5(w, 0, IsMultiplayer() ? 0x6d : 0x86, 0x9874, 3, 0, 0);
        for (int32_t i = 0; i < 3; ++i) Window_SendEvent5(w, 0, IsMultiplayer() ? 0x87 : 0x65, 0x985e + i, 3, 0, 0);
    }
    TrackPanel_Init(w);
    uint8_t* m = g_map;
    hook::As<int32_t>(g_6d1a94) = F<int32_t>(m, 0x168d) - F<int32_t>(m, 0x1689);
    if (F<int32_t>(g_app, 0x263)) F<int32_t>(m, 0x168d) = F<int32_t>(m, 0x1689) + 1;
    m = g_map;
    if (F<int32_t>(g_app, 0x2cc) && !F<int32_t>(m, 0x2175)) Overview_SetMode(m, 0, 0x15, 0, 0, 0, nullptr, 0, 0, 99999, 99999);
    F<int32_t>(w, 0x83) = 0;
    if (F<int32_t>(g_app, 0x309)) Camera_BeginA0(hook::As<uint8_t*>(g_tracks));
    return w;
}
RT3_REPLACE(0x50e400, TrackTool_Construct);

// 0x50e5c0 (thiscall, RET 4). Track tool t's event handler (0 frame, 2 / 3
// mouse button / key, 0xca press and 0xcb release of a button or the map).
// Nothing while paused (+0x83); a running multiplayer build (0x6cd8e8) or
// Esc finishes the build. A frame without the player's company leaves the
// tool; with a modal UI open (+0xc64) a drag ends or the hover node (+0x78)
// is dropped; otherwise, when the focus moved (0x6d1a98, more than 0.2
// away), and after a press (the button is remembered in +0x7e) the tool is
// updated (or bulldozes for kind 0x622b0c 0x40). Pressing the map starts a
// drag at the hover node unless its cell is blocked (multiplayer: width to
// 0x6d1a56); releasing it lays the line (TrackNet_ExtendTo; multiplayer
// sends it instead) or, when it costs more than the cash (and the game
// charges for it), ends with a message. The panel buttons pick the tool
// kind (1, 4, bulldozer 0x40: not with a pending order +0x4adf), the
// option +0x138 (0 / 1 / 3) / +0x144 / +0x13c / +0x140 (saved), the
// world flag +0x4c74, undo, and electrification (quote, confirm 0x3f2).
// Other releases drop the trial pieces. Returns 0 when handled, else 1.
RT3_NOINLINE int32_t __fastcall TrackTool_HandleEvent(uint8_t* t, int, uint32_t* ev) {
    uint8_t* const net = hook::As<uint8_t*>(g_tracks);
    auto netNow = [] { return hook::As<uint8_t*>(g_tracks); };
    auto company = [] { return Obj_Get21(g_world); };
    auto kind = [] { return g_622b0c; };
    auto message = [](const char* text, int32_t type) {
        return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
    };
    auto setOption = [&](uint32_t off, int32_t v) {
        F<int32_t>(g_app, off) = v;
        Options_Save(g_app, 0, 1);
        TrackPanel_Refresh(t);
        return 0;
    };
    auto setKind = [&](int32_t k) {
        g_622b0c = k;
        TrackPanel_Refresh(t);
        return 0;
    };
    auto endDrag = [&] {
        netNow()[0x11c] = 0;
        if (F<int32_t>(netNow(), 0x118)) Obj_DecayCounters(netNow());
        F<int32_t>(netNow(), 0x118) = 0;
        Obj_SetText1c(g_cursorManager, 0, nullptr, 0xffffffff);
    };
    (void)net;
    if (F<int32_t>(t, 0x83)) return 0;
    if (g_6cd8e8) {
        TrackTool_FinishBuild(t);
        return 1;
    }
    int32_t button = -1;
    switch (ev[0]) {
    case 0: {
        if (!Collection_IsValid(g_companies, 0, company())) {
            View_SetToolMode(g_gameWindow, 0, 0, -1);
            return 1;
        }
        if (F<int32_t>(g_windowManager, 0xc64) > 0) {
            if (F<int32_t>(netNow(), 0x118)) {
                TrackTool_FinishBuild(t);
                return 1;
            }
            if (F<int32_t>(t, 0x78) == -1) return 1;
            uint8_t* p = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x78));
            if (!p[0x216]) {
                uint8_t* n = netNow();
                ((void(__fastcall*)(uint8_t*, int, int32_t))(*(void***)n)[2])(n, 0, F<int32_t>(t, 0x78));
            }
            F<int32_t>(t, 0x78) = -1;
            Tracks_UpdateColor(g_tracks);
            return 1;
        }
        if (kind() == 0x40) return 1;
        float x, y;
        const int32_t focus = Scene_FocusFlagIs1(&x, &y);
        if (focus == g_6d1a98) {
            if (!focus) return 1;
            if (!(Distance2D(x, y, g_622b10, g_622b14) > D(0x5c8c20))) return 1;
        }
        button = F<int32_t>(t, 0x7e);
        break;
    }
    case 2:
        F<uint32_t>(t, 0x7e) = ev[1];
        button = (int32_t)ev[1];
        break;
    case 3:
        if (ev[2] != 0x1b) return 1;
        TrackTool_FinishBuild(t);
        return 0;
    case 0xca:
        switch (ev[1]) {
        case 0x7d6:
            break;
        case 0x9860:
            if (IsMultiplayer() && !Net_AllHaveVersion(1.03f)) return 0;
            if (F<int32_t>(g_world, 0x4adf)) {
                TrackPanel_Refresh(t);
                message(GetString(0x26b), 1);
                return 0;
            }
            return setKind(0x40);
        case 0x985e:
        case 0x9875:
            return setKind(1);
        case 0x985f:
        case 0x9876:
            return setKind(4);
        case 0x9861:
        case 0x9863:
        case 0x9866:
            return setOption(0x138, 3);
        case 0x9862:
        case 0x9864:
        case 0x9867:
            return setOption(0x138, 1);
        case 0x9865:
        case 0x9868:
            return setOption(0x138, 0);
        default:
            return 1;
        }
        if (kind() == 0x40) {
            float x, y;
            if (!Scene_FocusFlagIs1(&x, &y)) return 0;
            if (ev[8] & 0x800) return 0;
            if (!IsMultiplayer()) Ui_PlaySound(0x39, 0, 0x3f000000);
            t[0x7d] = 1;
            break;
        }
        {
            float x, y;
            if (!Scene_FocusFlagIs1(&x, &y)) return 0;
            if (F<int32_t>(netNow(), 0x118)) return 0;
            if (F<int32_t>(t, 0x78) == -1) return 0;
            const int32_t iy = (int32_t)y;
            if (Map_CellLow3(hook::As<void*>(g_map), 0, (int32_t)x, iy)) return 0;
            uint8_t* p = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(t, 0x78));
            F<int32_t>(t, 0x78) = -1;
            F<int32_t>(netNow(), 0x118) = 1;
            float cx, cy;
            Obj_CenterCell(p, 0, &cx, &cy);
            F<float>(netNow(), 0x11d) = cx;
            F<float>(netNow(), 0x121) = cy;
            F<uint32_t>(netNow(), 0x125) = F<uint32_t>(p, 0x202);
            Obj_CopyCorners(netNow(), 0, p);
            Obj_CameraByOptions(netNow());
            if (IsMultiplayer()) hook::As<float>(g_6d1a56) = Route_GradeFactorHere(cx, cy);
        }
        return 0;
    case 0xcb: {
        if (kind() != 0x40) TrackNet_ShowStatus(netNow(), 0, F<uint32_t>(t, 0x78));
        switch (ev[1]) {
        case 0x985a:
            return setOption(0x144, !F<int32_t>(g_app, 0x144));
        case 0x985c:
        case 0x985d: {
            const uint32_t off = ev[1] == 0x985c ? 0x140 : 0x13c;
            uint8_t* app = g_app;
            if (--F<int32_t>(app, off) < 0) F<int32_t>(app, off) = 3;
            Options_Save(g_app, 0, 1);
            TrackPanel_Refresh(t);
            return 0;
        }
        case 0x9859: {
            uint8_t* w = g_world;
            F<int32_t>(w, 0x4c74) = !F<int32_t>(w, 0x4c74);
            TrackPanel_Refresh(t);
            return 0;
        }
        case 0x9858:
            if (!IsMultiplayer()) {
                TrackTool_Cancel(t);
                TrackNet_Undo(g_tracks, 0, 0xffffffff);
                F<int32_t>(netNow(), 0x12d) = -1;
                TrackPanel_Refresh(t);
            }
            return 0;
        case 0x985b: {
            float cost, share;
            TrackNet_ElectrifyQuote(g_tracks, 0, company(), &cost, &share);
            if (share == 0.0f) { // an unordered share (NaN) goes on
                message(GetString(0xc0b), 1);
                return 0;
            }
            const double cash = Company_Stat((uint8_t*)Obj_Company21(g_world), 0, 0xd, 0x2329);
            const uint8_t charged = !g_newGame[0x82];
            uint32_t r;
            char a[0x20], b[0x20], text[0x200];
            if ((double)cost > cash) {
                if (charged) {
                    const char* have = Num_FormatSimple(a, 9, Company_Stat((uint8_t*)Obj_Company21(g_world), 0, 0xd, 0x2329), 0xb);
                    const char* need = Num_FormatSimple(b, 9, (double)cost, 0xb);
                    FormatText(text, GetString(0xc0c), need, have, 0, 0, 0, 0, 0, 0);
                    message(text, 1);
                    return 0;
                }
                r = message(GetString(0xf3c), 3);
            } else if (!charged) {
                r = message(GetString(0xf3c), 3);
            } else {
                const char* price = Num_FormatSimple(a, 9, (double)cost, 0xb);
                const char* pct = Num_Format(b, 5, D(0x5c8770) - (double)share, 10, 1, 0, 0);
                FormatText(text, GetString(0xefd), pct, price, 0, 0, 0, 0, 0, 0);
                r = message(text, 3);
            }
            if (r != 0x3f2) return 0;
            if (!IsMultiplayer()) {
                TrackNet_ElectrifyCompany(g_tracks, 0, company(), 0, nullptr, nullptr);
                return 0;
            }
            int32_t id = company();
            Net_SendToHost(g_session, 0, 0x5b, 4, (const uint8_t*)&id);
            return 0;
        }
        case 0x7d6:
            break;
        default:
            if (F<int32_t>(netNow(), 0x125) == -1) return 1;
            Tracks_UndoPendingEdit();
            if (F<int32_t>(netNow(), 0x125) != -1 && Collection_IsValid(g_tracks, 0, F<uint32_t>(netNow(), 0x125)))
                TrackNet_RemovePiece(g_tracks, 0, F<uint32_t>(netNow(), 0x125), 0, -1);
            endDrag();
            Obj_FreeList1c9(netNow());
            return 1;
        }
        if (kind() == 0x40) {
            if (!t[0x7d]) return 1;
            t[0x7d] = 0;
            return 0;
        }
        if (F<int32_t>(t, 0x7e) != 0x7d6 && F<int32_t>(netNow(), 0x118)) {
            Tracks_UndoPendingEdit();
            if (!t[0x82]) {
                TrackNet_EndBuild(g_tracks);
                return 0;
            }
            t[0x82] = 0;
            F<int32_t>(netNow(), 0x118) = 0;
            Obj_SetText1c(g_cursorManager, 0, nullptr, 0xffffffff);
            return 0;
        }
        t[0x82] = 0;
        const uint32_t xb = hook::As<uint32_t>(g_622b10), yb = hook::As<uint32_t>(g_622b14);
        float x, y;
        memcpy(&x, &xb, 4), memcpy(&y, &yb, 4);
        if (!g_6d1a98 || !F<int32_t>(netNow(), 0x118)) return 1;
        const float cash = (float)Company_Stat((uint8_t*)Obj_Company21(g_world), 0, 0xd, 0x2329);
        const float cost = F<int32_t>(netNow(), 0x118) ? F<float>(netNow(), 0x1d1) : 1.0f;
        if (cost > cash && !g_newGame[0x82] && !F<int32_t>(g_world, 0x4aef)) {
            Tracks_UndoPendingEdit();
            TrackNet_EndBuild(g_tracks);
            char need[0x200], have[0x130], text[0x320];
            Num_FormatSimple2(need, 0xb, (double)cost, 0xb);
            Num_FormatSimple2(have, 0xb, (double)cash, 0xb);
            if (F<int32_t>(netNow(), 0x118)) FormatText(text, GetString(0xc6d), need, have, 0, 0, 0, 0, 0, 0);
            else FormatText(text, GetString(0xc6e), 0, 0, 0, 0, 0, 0, 0, 0);
            if (!F<int32_t>(g_world, 0x4a8b)) strcat(text, GetString(0xc71));
            message(text, 1);
            Tracks_UpdateColor(g_tracks);
        } else {
            if (F<int32_t>(netNow(), 0x125) != -1 && Collection_IsValid(g_tracks, 0, F<uint32_t>(netNow(), 0x125)))
                Track_UpdateColor((uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(netNow(), 0x125)));
            if (F<int32_t>(netNow(), 0x125) != -1 && !Collection_IsValid(g_tracks, 0, F<uint32_t>(netNow(), 0x125))) {
                const float cx = F<float>(netNow(), 0x11d), cy = F<float>(netNow(), 0x121);
                const uint8_t style = Obj_Mode1Or4(kind());
                const int32_t owner = company();
                F<int32_t>(netNow(), 0x125) = Tracks_AddPieceRotated(g_tracks, 0, cx, cy, 0.0f, 0, owner, style, 0);
            }
            if (IsMultiplayer()) {
                Table_FillCursor(g_tracks, 0, (uint8_t*)0x6d1a40);
                g_6d1a5a = xb;
                g_6d1a5e = yb;
                Company_TrackBounds((int32_t*)0x6d1a72, company());
                if (F<int32_t>(netNow(), 0x118)) {
                    Tracks_UndoPendingEdit();
                    TrackNet_EndBuild(g_tracks);
                }
                Net_SendSettings(t);
            } else {
                const int32_t before = F<int32_t>(netNow(), 0x125);
                // ExtendTo leaves `moved` alone on some paths; the original
                // reuses the slot that holds the cash there.
                int32_t moved;
                memcpy(&moved, &cash, 4);
                F<int32_t>(netNow(), 0x9c) = 0;
                const float radius = Route_GradeFactorHere(x, y);
                const int32_t last = TrackNet_ExtendTo(g_tracks, 0, x, y, 1, company(), kind(), radius, &moved);
                if (last != -1) F<int32_t>(netNow(), 0x125) = last;
                const int32_t run = F<int32_t>(netNow(), 0x9c);
                if (before != F<int32_t>(netNow(), 0x125) || moved) Obj_PushIdNode(netNow(), 0, run, company());
                endDrag();
                Tracks_ApplyPendingKinds(g_tracks);
                Obj_FreeList1c9(netNow());
                F<int32_t>(netNow(), 0x12d) = -1;
                F<uint32_t>(netNow(), 0x169) = 0xbf800000;
                Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xbb, 0x808, 0, 0, 0);
                TrackPanel_Refresh(t);
            }
        }
        Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 1, 0, -1);
        return 0;
    }
    default:
        return 1;
    }
    if (F<int32_t>(g_windowManager, 0xc64) > 0) return 1;
    if (!Collection_IsValid(g_companies, 0, company())) return 1;
    g_6d1a98 = Scene_FocusFlagIs1((float*)0x622b10, (float*)0x622b14);
    const uint32_t release = button != 0x7d6 && button != 0 ? 1 : 0;
    if (kind() == 0x40) {
        TrackTool_Bulldoze(t, 0, button == 0x7d6 ? 1 : 0, release);
    } else {
        float r;
        memcpy(&r, &release, 4);
        TrackTool_Update(t, 0, button == 0x7d6 ? 1 : 0, r);
    }
    return 1;
}
RT3_REPLACE(0x50e5c0, TrackTool_HandleEvent);

RT3_NOINLINE void* Get6d3ae8() { return (void*)0x6d3ae8; }
RT3_REPLACE(0x50f5f0, Get6d3ae8);

RT3_NOINLINE void* Get6d3ae8b() { return (void*)0x6d3ae8; }
RT3_REPLACE(0x50f600, Get6d3ae8b);

// 0x50f610 (callback). Shows the pick tooltip of train list child 0x2afc
// when the first stack argument is set.
void __fastcall TrainList_PickTooltipCb(void*, int, int32_t on, int32_t, int32_t, int32_t, int32_t) {
    if (on) Map_ShowPickTooltip(Widget_FindChild(g_6d3ac8, 0, 0x2afc));
}
RT3_REPLACE(0x50f610, TrainList_PickTooltipCb);

// 0x50f640 (vtable stub).
// Forwards to Widget_DeleteGeneratedRows (0x4bd460).
void __fastcall Thunk50f640(uint8_t* p0, int p1, int32_t p2) { Widget_DeleteGeneratedRows(p0, p1, p2); }
RT3_REPLACE(0x50f640, Thunk50f640);

// 0x50f650 (callback). Adds the cargo buttons of kind `type` (EDX) to the
// list 0x6d3ae8 of row w; the first stack argument is replaced.
void __fastcall BuildingRow_AddCargoButtonsTo6d3ae8(uint8_t* w, int32_t type, int32_t, float x, float y, int32_t a, int32_t b) {
    BuildingRow_AddCargoButtons(w, (uint8_t*)0x6d3ae8, type, x, y, a, b);
}
RT3_REPLACE(0x50f650, BuildingRow_AddCargoButtonsTo6d3ae8);

void __fastcall StopList_DrawLabelCb(void*, int32_t idx, float x, float y, int32_t a, float h, int32_t) { StopList_DrawLabel((void*)0x6d3ae8, idx, x, y, a, h); }
RT3_REPLACE(0x50f660, StopList_DrawLabelCb);

void __fastcall TrainList_ClearChildren(void*, int, int32_t) { Window_DeleteChildren(g_6d3ac8, 0, 0x28a0, 0x2968, 0); }
RT3_REPLACE(0x50f690, TrainList_ClearChildren);

RT3_CLEAR_CHILDREN(0x50f6b0, Dialog_ClearChildren2774, 0x2774, 0x27d8)

// 0x50f6d0. Drops the three pack resources 0x6d3abc..0x6d3ac4 and clears +0x78.
RT3_NOINLINE void __fastcall Panel_ReleaseImages(uint8_t* w) {
    for (uint32_t slot = 0x6d3abc; slot <= 0x6d3ac4; slot += 4) {
        Pack_Release(g_pack, 0, hook::Global<void*>(slot));
        hook::Global<void*>(slot) = nullptr;
    }
    F<int32_t>(w, 0x78) = 0;
}
RT3_REPLACE(0x50f6d0, Panel_ReleaseImages);

RT3_NOINLINE void __stdcall SetGlobal6d1aa4(int32_t v) { g_6d1aa4 = v; }
RT3_REPLACE(0x50f730, SetGlobal6d1aa4);

int32_t Global6d3ae4() { return g_6d3ae4; }
RT3_REPLACE(0x50f740, Global6d3ae4);

void SetGlobal6d3adc() { g_6d3adc = 1; }
RT3_REPLACE(0x50f750, SetGlobal6d3adc);

// 0x50f760. The train list's selected entry (child 0x2afe), 0 when none.
RT3_NOINLINE uint32_t TrainList_Selected() {
    uint8_t* list = g_6d3ac8;
    if (!list) return 0;
    uint8_t* w = Widget_FindChild(list, 0, 0x2afe);
    if (F<int32_t>(w, 0x1d6) < 0) return 0;
    return TreeView_SelectedData(w);
}
RT3_REPLACE(0x50f760, TrainList_Selected);

// 0x50f790. Cost of placing the editor's selected building (dialog 0x6d3ac8,
// tree 0x2afe) on every marked cell (0x6d1aa8, 0x2000 bytes): *count cells x the
// type's scaled price for the world's company / player.
RT3_NOINLINE void __fastcall Editor_SelectionCost(int32_t* total, int32_t* count) {
    *total = 0;
    *count = 0;
    uint8_t* dlg = g_6d3ac8;
    if (!dlg) return;
    uint8_t* tree = Widget_FindChild(dlg, 0, 0x2afe);
    if (F<int32_t>(tree, 0x1d6) < 0) return;
    uint32_t sel = TreeView_SelectedData(tree);
    if (!sel) return;
    int32_t n = 0;
    for (int k = 0; k < 0x2000; ++k)
        if (((const uint8_t*)0x6d1aa8)[k]) ++n;
    uint8_t* t = (uint8_t*)Collection_Get(g_6ada84, 0, sel);
    uint8_t* w = g_world;
    int32_t player = Obj_Get25(w);
    float price = Obj_EffectScale(t, 0, Obj_Get21(g_world), player);
    *count = n;
    *total = (int32_t)price * n;
}
RT3_REPLACE(0x50f790, Editor_SelectionCost);

// 0x50f890. The train upgrade dialog (0x6d3ac8) cost line (child 0x275c): with an
// engine picked in its list (child 0x2afe), "No trains currently selected to
// upgrade." (string 0x53a), or "Total Cost to Upgrade the N Selected Trains to
// <engine> is <cost>" (0x53b; 0x53c for one train), cost in money-K style.
RT3_NOINLINE void UpgradeDialog_UpdateCostText() {
    uint8_t* dlg = g_6d3ac8;
    if (!dlg) return;
    uint8_t* tree = Widget_FindChild(dlg, 0, 0x2afe);
    if (F<int32_t>(tree, 0x1d6) < 0) return;
    uint32_t sel = TreeView_SelectedData(tree);
    if (!sel) return;
    int32_t total, count;
    Editor_SelectionCost(&total, &count);
    uint8_t* engine = (uint8_t*)Collection_Get(g_6ada84, 0, sel);
    uint8_t* line = Widget_FindChild(g_6d3ac8, 0, 0x275c);
    char cost[0x80], num[0x20], text[0x200];
    Num_FormatSimple2(cost, 0xb, (double)total, 0xb);
    if (!count) {
        FormatText(text, GetString(0x53a), 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
        uint32_t type = F<uint32_t>(engine, 0x72);
        uint8_t* e = type ? (uint8_t*)Collection_Get(g_carTypes, 0, type) : nullptr;
        const char* name = LocalizeName((const char*)(e + 0x45));
        if (count > 1) {
            Num_Format(num, 1, (double)count, 10, 1, 0, 0);
            FormatText(text, GetString(0x53b), num, name, cost, 0, 0, 0, 0, 0);
        } else {
            FormatText(text, GetString(0x53c), name, cost, 0, 0, 0, 0, 0, 0);
        }
    }
    String_Assign((char**)(line + 0xba), text);
}
RT3_REPLACE(0x50f890, UpgradeDialog_UpdateCostText);

// 0x50fa00. Single player only: adds the two small arrow buttons 0x28a0 + `id`
// (top, at y) and 0x2904 + `id` (bottom, at y + h - 0x5d1948) left of x to the
// window 0x6d3ac8 for list `id` (0x6ada84, whose entry +0x72 in 0x62ba88 must
// have entries in +0x196); tooltip string 0x10d8.
void __stdcall List_AddArrowButtons(uint32_t id, float x, float y, int32_t, float h) {
    uint8_t* t = (uint8_t*)Collection_Get(g_6ada84, 0, id);
    uint8_t* r = F<uint32_t>(t, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(t, 0x72)) : nullptr;
    if (!Collection_Count(F<Collection*>(r, 0x196))) return;
    if (IsMultiplayer()) return;
    float bx = (float)((double)x - D(0x5cab68));
    auto add = [&](uint16_t bid, float by, uint16_t up, uint16_t down) {
        uint8_t def[0xa5];
        memset(def, 0, sizeof def);
        F<uint16_t>(def, 0) = bid;
        F<float>(def, 7) = bx;
        F<float>(def, 0xb) = by;
        F<uint32_t>(def, 0xf) = 0x3c428f5c;
        F<uint32_t>(def, 0x13) = 0x3c962fc9;
        def[0x89] = 6;
        def[0x8a] = 7;
        F<uint16_t>(def, 0x5f) = up;
        F<uint16_t>(def, 0x61) = down;
        String_Assign(&F<char*>(def, 0x43), (const char*)0x5d190c);
        String_Assign(&F<char*>(def, 0x3f), GetString(0x10d8));
        uint8_t* b = (uint8_t*)crt::operator_new(0xe9);
        if (b) b = Control_Construct(b, 0, def);
        Window_AddChild(g_6d3ac8, 0, b, 0);
        crt::free(F<void*>(def, 0x43));
        crt::free(F<void*>(def, 0x3f));
    };
    add((uint16_t)(id + 0x28a0), y, 0x6e, 0x6f);
    add((uint16_t)(id + 0x2904), (float)((double)y + h - Fl(0x5d1948)), 0x70, 0x71);
}
RT3_REPLACE(0x50fa00, List_AddArrowButtons);

// 0x50fc00. Draws row `id` of the locomotive list (records 0x6ada84, engine
// type +0x72 in 0x62ba88) at (x, y, w, h): the type's localized name (+0x45,
// font 0x6d3abc) and on the right (font 0x6d3ac0) its value +0x105, or with
// option +0x180 the range "+0x105-+0x109" plus a line of the record's tags
// (+0x30..+0x32: strings 0x53d..0x53f, last separator cut); the type's icon
// (variant +0x262, or its base image +0x12a by +0x3d) at the left.
void __fastcall LocoList_DrawRow(void*, uint32_t id, float x, float y, float w, float h, int32_t) {
    uint8_t* r = (uint8_t*)Collection_Get(g_6ada84, 0, id);
    auto type = [&]() -> uint8_t* {
        return F<uint32_t>(r, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(r, 0x72)) : nullptr;
    };
    uint8_t* t = type();
    float nx = (float)((double)x + D(0x5c85b8));
    float nw = (float)((double)w - D(0x5c8918));
    Font_DrawTextBox(g_6d3abc, 0, LocalizeName((const char*)(t + 0x45)), nx, y, nw, h, 0xffffffff, 2, 4, 0, 1.0f);
    uint8_t* t2 = type();
    char text[0x100], a[0x20], b[0x20];
    if (F<int32_t>(g_app, 0x180)) {
        int32_t hi = F<int32_t>(type(), 0x109);
        int32_t lo = F<int32_t>(type(), 0x105);
        const char* s1 = Num_Format(a, 8, (double)hi, 10, 1, 0, 0);
        const char* s0 = Num_Format(b, 8, (double)lo, 10, 1, 0, 0);
        FormatText(text, (const char*)0x5cfdbc, s0, s1, 0, 0, 0, 0, 0, 0);
    } else {
        Num_Format(text, 8, (double)F<int32_t>(type(), 0x105), 10, 1, 0, 0);
    }
    double right = (double)x + w - D(0x5c86b8);
    Font_DrawTextBox(g_6d3ac0, 0, text, (float)(right - D(0x5c85b8)), y, BitsF(0x3dcccccd), h, 0xffffffff, 3, 4, 0, 1.0f);
    if (F<int32_t>(g_app, 0x180)) {
        char tags[0x100];
        tags[0] = 0;
        if (r[0x30]) strcat(tags, GetString(0x53d));
        if (r[0x31]) strcat(tags, GetString(0x53e));
        if (r[0x32]) strcat(tags, GetString(0x53f));
        if (strlen(tags) > 2) tags[strlen(tags) - 2] = 0;
        float ty = (float)((double)y + D(0x5caad0));
        Font_DrawTextBox(g_6d3ac0, 0, tags, (float)(right - D(0x5c9ba0)), ty, BitsF(0x3e0f5c29), h, 0xffffffff, 3, 1, 0, 1.0f);
    }
    uint8_t* img;
    if (F<int32_t>(t2, 0x262) == -1) img = (uint8_t*)Table_Entry(F<uint8_t*>(t2, 0x12a), 0, F<int32_t>(type(), 0x3d));
    else img = (uint8_t*)Table_Entry(F<uint8_t*>(t2, 0x19a + F<int32_t>(t2, 0x262) * 4), 0, 0);
    float s = (float)One();
    Glyph_Queue(img, 0, nx, y, 0x3f800000, s, s, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x50fc00, LocoList_DrawRow);

// 0x50fff0. The engine panel of the train buying window (0x6d3ac8, the engine
// picked in its list 0x2afe): name, the seven stats (as the locomotive
// window, but the first two are the effect-scaled cost and the price with
// the multipliers; reliability and the two cargo lines hidden in sandbox /
// no-money games, world +0x4aef or 0x6cec7c +0x82), the stat labels as
// timers (created once, 0x6d3ae2), the speed step of the chosen unit
// (0x6d3aac by 0x622b28) and the payment table with cents (style 5) down the
// side.
void __fastcall TrainBuy_EngineDraw(uint8_t* p, int, float x, float y, float w, float h, int32_t) {
    auto list = [] { return Widget_FindChild(g_6d3ac8, 0, 0x2afe); };
    if (!g_6d3ac8) return;
    uint8_t* l0 = list();
    if (F<int32_t>(l0, 0x1d6) < 0 || !TreeView_SelectedData(l0)) return;
    uint32_t pick = 0;
    if (g_6d3ac8) {
        uint8_t* l = list();
        if (F<int32_t>(l, 0x1d6) >= 0) pick = TreeView_SelectedData(l);
    }
    uint8_t* rec = (uint8_t*)Collection_Get(g_6ada84, 0, pick);
    uint8_t* type = F<uint32_t>(rec, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(rec, 0x72)) : nullptr;
    int32_t base = Ftol(F<float>(type, 0xfd));
    if (rec[0x7a]) {
        uint8_t* second = F<uint32_t>(rec, 0x76) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(rec, 0x76)) : nullptr;
        float bf = (float)base;
        base = Ftol((double)bf + F<float>(second, 0xfd));
    }
    char stats[7][0x32];
    memset(stats, 0, sizeof stats);
    static const int32_t labels[7] = { 0x2bc, 0x2bd, 0x2be, 0x2bf, 0x2c0, 0x2c1, 0x2c2 };
    static const int32_t timers[7] = { 0xf02, 0xf03, 0xf04, 0xf05, 0xf06, 0xf07, 0xf08 };
    auto who = [&](uint32_t& company, uint32_t& player) {
        player = (uint32_t)Obj_Get25(g_world);
        company = (uint32_t)Obj_Get21(g_world);
    };
    uint32_t co, pl;
    who(co, pl);
    float scale = Obj_EffectScale(rec, 0, co, pl);
    Num_FormatSimple2(stats[0], 0xb, (double)scale, 0xb);
    float price = F<float>(rec, 8);
    who(co, pl);
    float f1 = Effects_Multiplier(g_world, 0, F<int32_t>(rec, 0x10) + 0x4e, co, pl, 0);
    who(co, pl);
    float f2 = Effects_Multiplier(g_world, 0, 0x4d, co, pl, 0);
    float cost = (float)((double)f2 * f1 * price);
    Num_FormatSimple2(stats[1], 0xb, (double)cost, 0xb);
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
    auto font = [](uint32_t a) { return hook::Global<uint8_t*>(a); };
    float rowY = (float)((double)y + D(0x5d0818));
    uint8_t* t2 = F<uint32_t>(rec, 0x72) ? (uint8_t*)Collection_Get(g_carTypes, 0, F<uint32_t>(rec, 0x72)) : nullptr;
    char text[0x200], num[0x40], line[0x200];
    FormatText(text, LocalizeName((const char*)t2 + 0x45), 0, 0, 0, 0, 0, 0, 0, 0);
    float ty = (float)((double)rowY - D(0x5d0820));
    Font_DrawTextBox(font(0x6d3abc), 0, text, x, ty, w, BitsF(0x3ccccccd), 0xffffffff, 0, 0, 0, 1.0f);
    rowY = (float)((double)rowY + Fl(0x5d0c60));
    rowY = (float)((double)rowY + D(0x5d1308));
    auto simple = [] { return g_newGame[0x82] || F<int32_t>(g_world, 0x4aef); };
    int32_t first = 0;
    if (simple()) {
        rowY = (float)((double)rowY + Fl(0x5d0c60));
        first = 3;
    }
    for (int32_t i = first; i < 7; ++i) {
        if (simple() && i == 5) continue;
        float wl = (float)((double)w - D(0x5c9b98));
        double lyd = (double)rowY - D(0x5d0820);
        float ly = (float)lyd;
        double lxd = (double)x + D(0x5c8918);
        float lx = (float)lxd;
        Font_DrawTextBox(font(0x6d3abc), 0, GetString(labels[i]), lx, ly, wl, BitsF(0x3ccccccd), 0xffffffff, 2, 0, 0, 1.0f);
        Font_DrawTextBox(font(0x6d3abc), 0, stats[i], lx, ly, wl, BitsF(0x3ccccccd), 0xffffffff, 3, 0, 0, 1.0f);
        if (!g_6d3ae2) {
            uint8_t def[0x8b];
            memset(def, 0, sizeof def);
            uint8_t* win = g_6d3ac8;
            F<float>(def, 7) = (float)(lxd - F<float>(win, 5));
            F<uint32_t>(def, 0x13) = 0x3ccccccd;
            F<float>(def, 0xb) = (float)(lyd - F<float>(win, 9));
            F<float>(def, 0xf) = (float)((double)w - D(0x5c8918));
            String_Assign(&F<char*>(def, 0x3f), GetString(timers[i]));
            uint8_t* tw = (uint8_t*)crt::operator_new(0xda);
            tw = tw ? TimerWidget_Construct(tw, 0, def) : nullptr;
            Window_AddChild(g_6d3ac8, 0, tw, 0);
            crt::free(F<void*>(def, 0x3f));
        }
        rowY = (float)((double)rowY + Fl(0x5d0c60));
    }
    if (simple()) rowY = (float)((double)rowY + Fl(0x5d1958));
    rowY = (float)((double)rowY + D(0x5d1300));
    Font_DrawTextBox(font(0x6d3ac0), 0, GetString(0x2c6), x, rowY, w, BitsF(0x3cb17e4b), 0xffffffff, 0, 0, 0, 1.0f);
    int32_t step = hook::Global<int32_t>(0x6d3aac + g_622b28 * 4);
    rowY = (float)((double)rowY + D(0x5d12f0));
    rowY = (float)((double)rowY + D(0x5d12e8));
    Num_Format(num, 1, (double)step, 10, 1, 0, 0);
    FormatText(line, GetString(0x2c7), num, 0, 0, 0, 0, 0, 0, 0);
    double xd = x;
    Font_DrawTextBox(font(0x6d3ac0), 0, line, (float)(xd + D(0x5d0170)), rowY, w, BitsF(0x3cb17e4b), 0xffffffff, 0, 0, 0, 1.0f);
    rowY = (float)((double)rowY + D(0x5d12e0));
    float cw = (float)((double)h * D(0x5c8618));
    float cy = (float)((double)y + h + D(0x5d1950));
    Font_DrawTextBox(font(0x6d3ac4), 0, GetString(0x2c8), x, cy, cw, BitsF(0x3cb17e4b), 0xffffffff, 0, 0, 0, 1.0f);
    rowY = (float)((double)rowY + D(0x5d12d0));
    float rowStart = (float)(xd + D(0x5c85b8));
    char cell[0x20];
    double inset = xd;
    for (int32_t row = 0; row < 5; ++row) {
        float colX = rowStart;
        for (int32_t col = 0; col < 9; ++col) {
            int32_t spd = hook::Global<int32_t>(0x6d3aac + g_622b28 * 4) * col + base;
            float spdF = (float)spd;
            if (!col) {
                if (row > 0) Num_Format(cell, 5, (double)(row * 2 - 2) / D(0x5c85d8), 10, 1, 0, 0);
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
            uint8_t* f = row && col ? font(0x6d3abc) : font(0x6d3ac0);
            inset = row ? 0.0 : BitsD(0x3f747ae147ae147bull);
            float cx = (float)((double)colX - inset);
            Font_DrawTextBox(f, 0, cell, cx, rowY, BitsF(0x3cf5c28f), BitsF(0x3cb17e4b), 0xffffffff, 3, 0, 0, 1.0f);
            colX = (float)((double)colX + Fl(0x5d12c8));
        }
        rowY = (float)((double)rowY + Fl(0x5d0c60));
    }
    g_6d3ae2 = 1;
}
RT3_REPLACE(0x50fff0, TrainBuy_EngineDraw);

}  // namespace rt3
