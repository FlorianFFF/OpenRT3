// CompanyWindow.cpp - RT3.exe 0x4be000-0x4c6000.
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

// Global tables (Collection*) referenced by id from objects.

const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
float __fastcall Obj_CenterCellX(void* o);
float __fastcall Obj_CenterCellY(void* o);
void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Obj_Get25(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void* __fastcall Array33_At(void* a, int, int32_t i);
void* __fastcall Obj_Company21(void* o);
int32_t __fastcall Obj_NameIndex(void* o);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
void* __fastcall Obj_City41(void* o);
void* __fastcall Obj_Ref388(void* o);
uint32_t __fastcall Car_SkinTexture(uint8_t* car);
double One();
void* __fastcall Obj_City352(void* o);

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

void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
int32_t Global6d07b4Set2();
uint8_t* __fastcall Obj_NamedEntry20(uint8_t* o);
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
uint32_t __fastcall Company_Logo(uint8_t* c);

void __fastcall TextLink_DrawPlain(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align,
                                   int32_t valign, uint32_t flags, const char* tip, int32_t id);

char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);

// RT3_REPLACE(0x55ed9a -- the tail of Movie_Stop above, not a separate function.

int32_t Global6d3b30();

void __fastcall List_RowPos(uint8_t* w, int, int32_t row, float* x, float* y);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
void* SelectedCompany();
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
int32_t __fastcall Company_NthHolder(int32_t* o, int, int32_t n, int32_t flag);
void __fastcall CompanyWindow_Refresh(uint8_t* win);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t Global6d40e4();
void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
int32_t ClockMs();
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
uint8_t* __fastcall Windows_FindChild(uint8_t* o, int, int32_t id);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Widget_SetVtbl(uint8_t* w);
int32_t __fastcall Table_LiveRankOf(Collection* t, int, uint32_t id);
int32_t __fastcall Table_CountField3f(Collection* t);
uint32_t __fastcall Table_NthIdField3f(Collection* t, int, int32_t n);
void __fastcall Panel_SetValue90(uint8_t* o, int, int32_t v);
void __fastcall Window_HighlightMapModes(uint8_t* w);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

int32_t __fastcall Company_EntryOpen(void* c, int, int32_t i);
float __fastcall Obj_RoundedCost(uint8_t* o, int, uint32_t company);
int32_t IsMultiplayer();
void __fastcall Obj_Set4b(void* o, int, int32_t v);
float __fastcall Company_CreditRating(uint8_t* c, int, int32_t extraDebt);
int32_t __fastcall Company_IssueBond(uint8_t* c, int, int32_t amount, int32_t years, int32_t commission, float rate);
float __fastcall Company_BondRateWithDebt(uint8_t* c, int, int32_t extraDebt, int32_t);
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
void __fastcall Company_IssueShares(uint8_t* c, int, int32_t n, int32_t atPrice, int32_t price);
uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
int32_t __fastcall Company_FreeShares(void* c);
void __fastcall Company_BuyBackShares(uint8_t* c, int, int32_t n);
void* __fastcall Obj_CityRef(void* o);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

double __fastcall Company_FloatD7b(void* c);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

void __fastcall Train_SetRunning(uint8_t* train, int, int32_t running);
int32_t SelectedTrainId();
void __fastcall Company_SetEntryFlag(void* c, int, int32_t i, uint8_t v);
void __fastcall Company_Dissolve(uint8_t* c);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
void __fastcall CompanyReport_Show(uint32_t a, uint8_t quick, uint32_t b);
void __fastcall Overview_Close(uint8_t* t);
void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
void __fastcall CompanyWindow_IssueBond(uint8_t* w);
void __fastcall CompanyWindow_IssueStock(uint8_t* w);
void __fastcall CompanyWindow_BuyBackStock(uint8_t* w);
void __fastcall CompanyWindow_EditDividend(uint8_t* w);
void __fastcall ProxyFight_Start(uint32_t company);
void __fastcall Company_DropChairman(int32_t* c, int, int32_t player);
void __fastcall Company_GoBankrupt(uint8_t* c);
uint32_t* __fastcall Obj_GetEntry5f(uint8_t* o, int, uint32_t* out, int32_t i);
void __fastcall Company_RemoveEntry(void* c, int, int32_t idx, int32_t post);
void* Engine_FirstListEntry();
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE int32_t __fastcall Company_EntryFlag(void* c, int, int32_t i);
RT3_NOINLINE int32_t Global6d07b4Set2();
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE int32_t IsMultiplayer();
int32_t __stdcall DateDiffMinutes(GameDate a, GameDate b);
RT3_NOINLINE void __fastcall Net_Send(uint8_t* s, int, const char* to, uint32_t type, int32_t size, const uint8_t* data, uint32_t flags, int32_t serial);
RT3_NOINLINE void* Engine_FirstListEntry();
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void* __fastcall Obj_Company21(void* o);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall CompanyView_HoverTerritory(int32_t id);
void __fastcall Car_StatusText(uint8_t* car, int, char* out);
uint32_t __fastcall TreeView_NodeData(uint8_t* w, int, int32_t i);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
void Profiles_IndexOf();
void __fastcall CompanyCard_Draw(uint8_t* co, int, float x, float y, float w, float h);

// Event callbacks: in a scenario (0x6d3b4c), releasing button `id` shows
// string 0xe8c (returns 0); otherwise 1.
#define RT3_SCENARIO_LOCK(addr, name, id) \
    int32_t __fastcall name(void*, const uint32_t* ev) { \
        if (ev[0] != 0xcb || ev[1] != id || !g_6d3b4c) return 1; \
        Message_Dialog(GetString(0xe8c), 0x3ef5c28f); \
        return 0; \
    } \
    RT3_REPLACE(addr, name);

void __stdcall CompanyView_Next();
void __stdcall CompanyView_Prev();
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
float __fastcall Company_MaxDividend(uint8_t* c);
double __fastcall Obj_WeightedAverage(void* o);
float __fastcall Obj_ValueWithBonus(uint32_t* o);
void __fastcall Stop_Name(uint8_t* stop, char* out);
void* __fastcall Train_NthCarOther(uint8_t* t, int, int32_t n);
void __fastcall TrainPanel_TooltipCb(int32_t id, int, int32_t, int32_t);
int32_t __fastcall Train_ReportCb(void*, int, int32_t cmd, uint32_t id);
int32_t __fastcall TrainRunMode_StepCb(uint8_t* w, int32_t button, int32_t ev, uint32_t id);
void TrainPanel_RefreshTooltip();
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Car_StatusText(uint8_t* car, int, char* out);
RT3_NOINLINE void __fastcall Company_Dissolve(uint8_t* c);
RT3_NOINLINE void __fastcall Company_DropChairman(int32_t* c, int, int32_t player);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE float __fastcall Train_CargoRate(uint8_t* t, int, int32_t onlyLocal);
RT3_NOINLINE char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
RT3_NOINLINE void __fastcall Stop_Name(uint8_t* stop, char* out);
RT3_NOINLINE void __fastcall StopList_DrawLabel(void* stops, int32_t idx, float x, float y, int32_t, float h);
RT3_NOINLINE void __fastcall BuildingRow_AddCargoButtons(uint8_t* w, uint8_t* list, int32_t type, float x, float y, int32_t, int32_t);
RT3_NOINLINE void __fastcall TrainPanel_TooltipCb(int32_t id, int, int32_t, int32_t);
RT3_NOINLINE void TrainPanel_RefreshTooltip();
RT3_NOINLINE void __fastcall TrainRow_Build(uint8_t* win, uint32_t trainId, int32_t compact, int32_t slot, int32_t rebuild);
RT3_NOINLINE int32_t __fastcall Train_ReportCb(void*, int, int32_t cmd, uint32_t id);
RT3_NOINLINE int32_t __fastcall TrainRunMode_StepCb(uint8_t* w, int32_t button, int32_t ev, uint32_t id);
RT3_NOINLINE void __fastcall CompanyCard_Draw(uint8_t* co, int, float x, float y, float w, float h);
RT3_NOINLINE void __fastcall Map_ShowPickTooltip(uint8_t* o);
RT3_NOINLINE void __fastcall CompanyStock_DrawChart(uint8_t* win, uint32_t linkBase, float x, float y, float, float, int32_t compact, uint32_t coId, uint8_t* faces, uint8_t** custom, int32_t, const uint8_t* show);
void __fastcall Dialog_ClearChildren94e8(uint8_t* w, int, int32_t);
RT3_NOINLINE uint32_t* __fastcall Obj_GetEntry5f(uint8_t* o, int, uint32_t* out, int32_t i);
RT3_NOINLINE int32_t Screen90();
RT3_NOINLINE void* SelectedCompany();
void __fastcall CompanyView_DrawIncomeTable(void*, int, float x, float y, float, float, int32_t);
void __fastcall CompanyView_DrawCardCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall CompanyView_HoverTerritory(int32_t id);
void __fastcall CompanyView_HoverRowCb(int32_t id, int, int32_t, int32_t, int32_t, int32_t row, int32_t);
void __fastcall TerritoryList_DrawRowCb(void*, int32_t id, float x, float y, float w, float h, int32_t);
void __fastcall BondList_DrawRow(int32_t, int32_t idx, float x, float y, int32_t, float h, int32_t);
void __fastcall FinanceList_AddRowButton(uint8_t* list, int, int32_t idx, float x, float y, int32_t, int32_t);
void __fastcall CompanyView_HoverTerritoryCb(void*, int, int32_t on, int32_t, int32_t, int32_t, int32_t);
void __fastcall CompanyWindow_Destruct(uint8_t* w);
void __fastcall CompanyCredit_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
void __fastcall CompanyStock_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall CompanyWindow_Refresh(uint8_t* win);
RT3_NOINLINE void __fastcall CompanyView_Step(int32_t delta);
RT3_NOINLINE void __stdcall CompanyView_Next();
RT3_NOINLINE void __stdcall CompanyView_Prev();
void __fastcall BondPanel_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall CompanyWindow_IssueBond(uint8_t* w);
void __fastcall StockIssuePanel_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
RT3_NOINLINE void __fastcall CompanyWindow_IssueStock(uint8_t* w);
void __fastcall CompanyView_DrawTerms(void*, int, float x, float y, float w, float, int32_t);
RT3_NOINLINE void __fastcall CompanyWindow_BuyBackStock(uint8_t* w);
void __fastcall CompanyDetail_AddArrowButtons(uint8_t* win, int, float x, float y, int32_t, int32_t);
void __fastcall DividendPanel_DrawCb(void*, int, float x, float y, float w, float h, int32_t);
int32_t __fastcall DividendDialog_HandleEvent(void*, uint32_t* msg);
RT3_NOINLINE void __fastcall CompanyWindow_EditDividend(uint8_t* w);
RT3_NOINLINE uint8_t* __fastcall CompanyWindow_Construct(uint8_t* w);
int32_t __fastcall CompanyWindow_HandleEvent(uint8_t* w, int, int32_t* ev);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void Merger_OfferDialog();
RT3_NOINLINE void __fastcall CompanyReport_Show(uint32_t a, uint8_t quick, uint32_t b);
RT3_NOINLINE void __fastcall ProxyFight_Start(uint32_t company);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
RT3_NOINLINE double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall TextLink_Draw(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign, uint32_t flags, const char* tip, int32_t id, uint32_t color, uint32_t hover);
RT3_NOINLINE void __fastcall TextLink_DrawPlain(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, const char* tip, int32_t id);

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


static inline float Prod_Bits(uint32_t b) { return *(const float*)&b; }

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

namespace {
const uint32_t kMsgNeg1 = 0xbf800000;
int32_t HandleEventMsg(const char* text, int32_t type, uint32_t px, int32_t p4 = 0, uint32_t buttonText = 0) {
    return (int32_t)Dialog_RunMessage(text, type, px, kMsgNeg1, p4, nullptr, 0, 0.0f, buttonText, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// The "not in multiplayer" / "not your turn" notes, shown a little left.
int32_t HandleEventNote(int32_t id) { return HandleEventMsg(GetString(id), 1, 0x3ef5c28f); }
// Magnitude as CDQ / XOR / SUB, so INT_MIN stays INT_MIN.
int32_t IAbs(int32_t v) { return v < 0 ? (int32_t)(0u - (uint32_t)v) : v; }
int32_t CompanyViewId() { return F<int32_t>(g_gameWindow, 0x90); }
}

namespace {
uint32_t Message_Dialog(const char* text, uint32_t px) {
    return Dialog_RunMessage(text, 1, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

namespace {
uint32_t Message451(const char* text, uint32_t px, uint32_t py) {
    return Dialog_RunMessage(text, 1, px, py, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
int32_t MyProfileIndex451() {
    const char* me = (const char*)Engine_FirstListEntry() + 8;
    int32_t k;
    __asm {
        push ebx
        mov ebx, me
        call Profiles_IndexOf
        pop ebx
        mov k, eax
    }
    return k;
}
}

// 0x4be6f0. Name of a route stop into `out`: a waypoint (+0x28 bit 7) is
// "Waypoint (x, y)" (string 0x38a) from its track node (+0x2b), cell coordinates
// truncated; otherwise the name of city +0x29 (0x6cec20, +0x46b).
RT3_NOINLINE void __fastcall Stop_Name(uint8_t* stop, char* out) {
    if ((int8_t)stop[0x28] < 0) {
        uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(stop, 0x2b));
        char ys[0x124], xs[0x134];
        Num_Format(ys, 1, (double)Ftol(Obj_CenterCellY(n)), 10, 1, 0, 0);
        Num_Format(xs, 1, (double)Ftol(Obj_CenterCellX(n)), 10, 1, 0, 0);
        FormatText(out, GetString(0x38a), xs, ys, 0, 0, 0, 0, 0, 0);
        return;
    }
    strcpy(out, (const char*)Collection_Get(g_stations, 0, F<uint16_t>(stop, 0x29)) + 0x46b);
}
RT3_REPLACE(0x4be6f0, Stop_Name);

// 0x4be7f0. Draws the label "<n>. <stop name>" (format 0x5d06e8) of stop `idx` of
// the list `stops` at (x + 0.004, y), 0.205 wide and half of `h` high, in the
// world's label font (+0x46a98).
RT3_NOINLINE void __fastcall StopList_DrawLabel(void* stops, int32_t idx, float x, float y, int32_t, float h) {
    char name[0x12c], text[0x12c], num[0x40];
    Stop_Name((uint8_t*)Array33_At(stops, 0, idx), name);
    const char* n = Num_Format(num, 1, (double)(idx + 1), 10, 1, 0, 0);
    FormatText(text, (const char*)0x5d06e8, n, name, 0, 0, 0, 0, 0, 0);
    float hh = (float)(h * D(0x5c8628));
    float lx = (float)((double)x + D(0x5d0720));
    Font_DrawTextBox(F<uint8_t*>(g_world, 0x46a98), 0, text, lx, y, BitsF(0x3e51eb85), hh, 0xffffffff, 2, 1, 0, 1.0f);
}
RT3_REPLACE(0x4be7f0, StopList_DrawLabel);

// 0x4be8d0. Adds a building type's row of cargo buttons to the window at +0x86:
// the row button 0x6608 + (type - +0x1e2) (shifted for windows 0x65a4 / 0x2aff;
// tooltip 0x38b, pressed unless it is the list's current type +8), then for the
// type's record (0x33 bytes, skipped when +0x28 has the top bit) up to 8 cargo
// icons 0x6612.. stepping right (the cargo list +1 with flag 0x40, else the one
// cargo +0x24 when +0x23 allows; icon "<cargo name>" from the cargo table
// 0x62ba8c when it exists in the image table, else a default; the specials
// 0x384..0x386 use fixed icons and strings 0x38c..0x38e), the optional icons
// for flags 0x10 (0x38f) and 0x20 (0x390), and the count box 0x661a (0x391).
RT3_NOINLINE void __fastcall BuildingRow_AddCargoButtons(uint8_t* w, uint8_t* list, int32_t type, float x, float y, int32_t, int32_t) {
    int32_t base = type - F<int32_t>(w, 0x1e2);
    if (F<uint16_t>(w, 4) == 0x65a4) base += 100;
    if (F<uint16_t>(w, 4) == 0x2aff) base -= 0x3afc;
    uint8_t def[0x89];
    auto addButton = [&] {
        uint8_t* b = (uint8_t*)crt::operator_new(0x111);
        b = b ? Scrollbar_Construct(b, 0, def) : nullptr;
        Window_AddChild(F<uint8_t*>(w, 0x86), 0, b, 0);
        crt::free(F<void*>(def, 0x43));
        crt::free(F<void*>(def, 0x3f));
    };
    memset(def, 0, sizeof def);
    F<float>(def, 0xb) = (float)((double)y + D(0x5d0288));
    F<uint32_t>(def, 7) = *(uint32_t*)&x;
    F<uint16_t>(def, 0) = (uint16_t)(base + 0x6608);
    F<uint32_t>(def, 0x2f) = 0x800;
    F<uint16_t>(def, 0x5f) = F<int32_t>(list, 8) != type;
    String_Assign(&F<char*>(def, 0x3f), GetString(0x38b));
    String_Assign(&F<char*>(def, 0x43), (const char*)0x5c9ee8);
    addButton();
    uint8_t* e = (uint8_t*)Array33_At(list, 0, type);
    if ((int8_t)e[0x28] < 0) return;
    float rowY = (float)((double)y + Fl(0x5d0758));
    float acc = 0.0f;
    int32_t last = 0;
    uint8_t* cargoes = e + 1;
    for (int32_t k = 0; k < 8; ++k) {
        acc = (float)((double)acc + Fl(0x5d0100));
        memset(def, 0, sizeof def);
        F<float>(def, 7) = (float)((double)acc + x);
        F<float>(def, 0xb) = rowY;
        F<uint16_t>(def, 0) = (uint16_t)(base + k + 0x6612);
        F<uint32_t>(def, 0x2f) = 0x810;
        int32_t cargo;
        if (e[0x28] & 0x40) cargo = F<int32_t>(cargoes, 0);
        else {
            if (e[0x23] < k + 1) break;
            cargo = F<int32_t>(e, 0x24);
        }
        char icon[0x100], tip[0x200];
        const char* fmt;
        if (Collection_IsValid(g_cargoTypes, 0, cargo)) {
            FormatText(icon, (const char*)0x5d01bc, (const char*)Collection_Get(g_cargoTypes, 0, cargo) + 4, 0, 0, 0, 0, 0, 0, 0);
            fmt = LocalizeName((const char*)Collection_Get(g_cargoTypes, 0, cargo) + 4);
        } else {
            int32_t str;
            if (cargo == 0x384) {
                FormatText(icon, (const char*)0x5d0640, 0, 0, 0, 0, 0, 0, 0, 0);
                str = 0x38c;
            } else if (cargo == 0x385) {
                FormatText(icon, (const char*)0x5d0630, 0, 0, 0, 0, 0, 0, 0, 0);
                str = 0x38d;
            } else if (cargo == 0x386) {
                FormatText(icon, (const char*)0x5d0620, 0, 0, 0, 0, 0, 0, 0, 0);
                str = 0x38e;
            } else
                break;
            fmt = GetString(str);
        }
        FormatText(tip, fmt, 0, 0, 0, 0, 0, 0, 0, 0);
        String_Assign(&F<char*>(def, 0x43), NameTable_ContainsName(g_pack, 0, icon, 0, 0) ? icon : (const char*)0x5d0194);
        String_Assign(&F<char*>(def, 0x3f), tip);
        addButton();
        last = k;
        cargoes += 4;
    }
    auto extra = [&](int32_t str, uint32_t image) {
        memset(def, 0, sizeof def);
        F<float>(def, 7) = (float)((double)acc + x);
        F<uint16_t>(def, 0) = (uint16_t)(last + base + 0x6612);
        F<float>(def, 0xb) = rowY;
        F<uint32_t>(def, 0x2f) = 0x810;
        String_Assign(&F<char*>(def, 0x3f), GetString(str));
        String_Assign(&F<char*>(def, 0x43), (const char*)image);
        addButton();
    };
    if (e[0x28] & 0x10) {
        extra(0x38f, 0x5d074c);
        acc = (float)((double)acc + Fl(0x5d0100));
        ++last;
    }
    if (e[0x28] & 0x20) extra(0x390, 0x5d0740);
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)((double)x + Fl(0x5d073c));
    F<uint16_t>(def, 0) = (uint16_t)(base + 0x661a);
    F<float>(def, 0xb) = (float)((double)y + Fl(0x5d0738));
    F<uint32_t>(def, 0x2f) = 0x810;
    String_Assign(&F<char*>(def, 0x43), (const char*)0x5d0728);
    String_Assign(&F<char*>(def, 0x3f), GetString(0x391));
    F<uint16_t>(def, 0x5f) = (e[0x28] & 0x40) ? e[0x21] : e[0x22];
    addButton();
}
RT3_REPLACE(0x4be8d0, BuildingRow_AddCargoButtons);

// 0x4beea0 (tooltip callback). Train panel widget `id` (0x332c.. per train,
// or 0x65c2.. for the selected train 0x6d0818): speed (+0xcc, string 0x392),
// cargo rate (0x393), cars and route leg (+4 / +8 of the stop list, strings
// 0x394..0x397), the lead car's status, or car n's status.
RT3_NOINLINE void __fastcall TrainPanel_TooltipCb(int32_t id, int, int32_t, int32_t) {
    uint8_t* w = Windows_FindChild(g_windowManager, 0, id);
    if (!w) return;
    char frame[0xfa8];
    char* text = frame + 0x1fc;
    char* num = frame + 8;
    text[0] = 0;
    const bool selected = id >= 0x65c2 && id <= 0x65d4;
    int32_t k = id + (selected ? -0x3296 : 0);
    uint32_t train = F<uint32_t>(w, 0x4f);
    k -= (k - 0x332c) / 18 * 18;
    if (selected) train = (uint32_t)SelectedTrainId();
    if (!Collection_IsValid(g_trains, 0, train)) return;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, train);
    if (!t) return;
    switch (k - 0x332c) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 0x11:
        return;
    case 6: {
        Num_Format(num, 1, (double)Ftol(F<float>(t, 0xcc)), 10, 1, 0, 0);
        FormatText(text, GetString(0x392), num, 0, 0, 0, 0, 0, 0, 0);
        memcpy(text + strlen(text), (const void*)0x5c9ce8, 2);
        strcat(text, GetString(0xf2e));
        break;
    }
    case 5: {
        Num_FormatSimple2(num, 0xb, (double)Train_CargoRate(t, 0, 0), 0xb);
        FormatText(text, GetString(0x393), num, 0, 0, 0, 0, 0, 0, 0);
        memcpy(text + strlen(text), (const void*)0x5c9ce8, 2);
        strcat(text, GetString(0xf2e));
        break;
    }
    case 7:
        if (Obj_City41(t)) Car_StatusText((uint8_t*)Obj_City41(t), 0, text);
        break;
    case 4: {
        const char* cars = GetString(F<int32_t>(t, 4) == 1 ? 0x395 : 0x396);
        Num_Format(num, 1, (double)F<int32_t>(t, 4), 10, 1, 0, 0);
        FormatText(text, GetString(0x394), num, cars, 0, 0, 0, 0, 0, 0);
        if (F<int32_t>(t, 4) >= 2) {
            char* from = frame + 0xdb4;
            char* to = frame + 0x9cc;
            char* leg = frame + 0xbc0;
            Stop_Name((uint8_t*)Array33_At(t, 0, F<int32_t>(t, 8)), from);
            Stop_Name((uint8_t*)Array33_At(t, 0, (F<int32_t>(t, 8) + 1) % F<int32_t>(t, 4)), to);
            FormatText(leg, GetString(0x397), from, to, 0, 0, 0, 0, 0, 0);
            strcat(text, leg);
        }
        break;
    }
    case 0x10:
        break;
    default:
        if (k - 0x332c >= 8 && k - 0x332c <= 0xf)
            if (uint8_t* car = (uint8_t*)Train_NthCarOther(t, 0, k - 0x3333)) Car_StatusText(car, 0, text);
        break;
    }
    Ui_ShowTooltip(g_windowManager, 0, text, 1, 1, -1);
}
RT3_REPLACE(0x4beea0, TrainPanel_TooltipCb);

// 0x4bf270. At most every 150 ms (0x6cfe24) refreshes the tooltip of the
// hovered train panel control (0x6d401c +0xc6d: ids 0x332c..0x36b0 and
// 0x65c6..0x65d1).
RT3_NOINLINE void TrainPanel_RefreshTooltip() {
    if (ClockMs() - g_6cfe24 <= 0x96) return;
    g_6cfe24 = ClockMs();
    uint8_t* hover = F<uint8_t*>(g_windowManager, 0xc6d);
    if (!hover) return;
    const uint16_t id = F<uint16_t>(hover, 4);
    if ((id >= 0x332c && id <= 0x36b0) || (id >= 0x65c6 && id <= 0x65d1)) TrainPanel_TooltipCb(id, 0, 0, 0);
}
RT3_REPLACE(0x4bf270, TrainPanel_RefreshTooltip);

// 0x4bf320. One train's row in the train list (or, `compact`, the train
// window's header at id 0x65c2): four order buttons (ids base..base+3, images
// 0x14.. / tooltips 0x621f20), the train name box (base+0x11, tooltip 0x39c),
// then (only the timer cells when `rebuild`) the cells base+4..: the name
// width cell, two fixed cells and one cell per car (consist from +0x41, car
// width from its skin texture, skipping kind-3 cars); buttons are enabled by
// the train's state (the button of the current order +0xd8 is held, +0x1a1)
// and the game mode.
RT3_NOINLINE void __fastcall TrainRow_Build(uint8_t* win, uint32_t trainId, int32_t compact, int32_t slot, int32_t rebuild) {
    Collection* trains = g_trains;
    if (!Collection_IsValid(trains, 0, trainId)) return;
    int32_t base;
    float ox, oy;
    if (compact) {
        base = 0x65c2;
        ox = BitsF(0x3ed0a3d7);
        oy = BitsF(0x3f6258bf);
    } else {
        oy = 0.0f;
        ox = 0.0f;
        uint8_t* list = Widget_FindChild(win, 0, Global6d3b30() ? 0x32c9 : 0x32ca);
        base = slot * 18 + 0x332c;
        List_RowPos(list, 0, slot, &ox, &oy);
    }
    uint8_t* t = (uint8_t*)Collection_Get(trains, 0, trainId);
    if (!rebuild || !Widget_FindChild(win, 0, base)) {
        for (int32_t i = 0; i < 4; ++i) {
            uint8_t def[0xa5];
            memset(def, 0, sizeof def);
            F<float>(def, 7) = (float)((double)ox + hook::Global<float>(0x6cfe28 + i * 8) + D(0x5ce8d0));
            if (compact) F<float>(def, 7) = (float)((double)F<float>(def, 7) + Fl(0x5d0100));
            F<uint16_t>(def, 0x5f) = (uint16_t)(i * 2 + 0x14);
            F<uint16_t>(def, 0x61) = (uint16_t)(i * 2 + 0x15);
            F<uint32_t>(def, 0x4b) = trainId;
            F<float>(def, 0xb) = (float)((double)oy + hook::Global<float>(0x6cfe2c + i * 8) + D(0x5d0770));
            F<uint32_t>(def, 0xf) = 0x3cc28f5c;
            F<uint32_t>(def, 0x13) = 0x3cf5c28f;
            F<uint16_t>(def, 0) = (uint16_t)(base + i);
            def[0x89] = 6;
            def[0x8a] = 7;
            String_Assign(&F<char*>(def, 0x43), (const char*)0x5c9fa8);
            String_Assign(&F<char*>(def, 0x3f), GetString(hook::Global<int32_t>(0x621f20 + i * 4)));
            uint8_t* b = (uint8_t*)crt::operator_new(0xe9);
            b = b ? Control_Construct(b, 0, def) : nullptr;
            Window_AddChild(win, 0, b, 0);
            crt::free(F<void*>(def, 0x43));
            crt::free(F<void*>(def, 0x3f));
        }
        uint8_t def[0x89];
        memset(def, 0, sizeof def);
        F<float>(def, 7) = (float)((double)g_6cfe40 + ox + D(0x5d0768));
        F<float>(def, 0xb) = (float)((double)g_6cfe44 + oy + D(0x5d0770));
        if (compact) F<float>(def, 7) = (float)((double)F<float>(def, 7) + Fl(0x5d0100));
        F<uint32_t>(def, 0xf) = 0x3ca3d70a;
        F<uint32_t>(def, 0x13) = 0x3cda740e;
        F<uint16_t>(def, 0x5f) = 0x8e;
        String_Assign(&F<char*>(def, 0x43), (const char*)0x5c9ee8);
        char text[0x200];
        const char* name = Train_DisplayName(t, 0, 0);
        FormatText(text, GetString(0x39c), name, 0, 0, 0, 0, 0, 0, 0);
        String_Assign(&F<char*>(def, 0x3f), text);
        F<uint16_t>(def, 0) = (uint16_t)(base + 0x11);
        F<uint32_t>(def, 0x4b) = trainId;
        uint8_t* s = (uint8_t*)crt::operator_new(0x111);
        s = s ? Scrollbar_Construct(s, 0, def) : nullptr;
        Window_AddChild(win, 0, s, 0);
        crt::free(F<void*>(def, 0x43));
        crt::free(F<void*>(def, 0x3f));
    }
    for (int32_t i = 0; i < 4; ++i) Window_SendEvent5(win, 0, F<int32_t>(t, 0xd8) != i ? 0x87 : 0x65, base + i, 3, 0, 0);
    Window_SendEvent5(win, 0, t[0x1a1] ? 0x65 : 0x87, base + 0x11, 3, 0, 0);
    if (rebuild) Window_DeleteChildren(win, 0, base + 7, base + 0xf, 0);
    for (int32_t k = 0x3330; k <= 0x333b; ++k) {
        if (rebuild && (k < 0x3333 || k > 0x333b)) continue;
        float x, y, w, h;
        if (k == 0x3332) {
            x = compact ? BitsF(0x3f0c0000) : BitsF(0x3f04cccd);
            y = BitsF(0x3ccccccd);
            w = BitsF(0x3d07ae14);
            h = BitsF(0x3cd3a06d);
        } else if (k == 0x3331) {
            x = BitsF(0x3ea28f5c);
            y = BitsF(0x3ada740e);
            w = BitsF(0x3e875c29);
            h = BitsF(0x3cd3a06d);
        } else if (k == 0x3330) {
            float tw = 0.0f;
            if ((t[0x1ce] & 1) || t[0x1a1]) {
                const char* nm = Train_DisplayName(t, 0, 0);
                Font_TextHeight(F<uint8_t*>(g_world, 0x46a98), 0, nm, 1.0f, &tw);
                tw = (float)((double)tw + D(0x5c85b8));
            }
            if (!t[0x1a1]) {
                w = BitsF(0x3df851ec);
                x = (float)((double)tw + D(0x5d0760));
            } else {
                w = BitsF(0x3dcf5c29);
                x = (float)((double)tw + Fl(0x5d075c));
            }
            if (compact) x = (float)((double)x + Fl(0x5d0100));
            y = BitsF(0x3ada740e);
            h = BitsF(0x3cbf258c);
        } else {
            y = BitsF(0x3cc5f92c);
            h = BitsF(0x3ccccccd);
            int32_t left = k - 0x3333;
            void* c = Obj_City41(t);
            x = compact ? BitsF(0x3ceb851f) : BitsF(0x3b75c28f);
            bool found = false;
            for (; c; c = Obj_City352(c)) {
                uint8_t* kind = (uint8_t*)Obj_Ref388(c);
                if (F<int32_t>(kind, 0x41) == 3) continue;
                uint32_t skin = Car_SkinTexture((uint8_t*)c);
                float f = (float)One();
                float cw = (float)(Slider_StepA((uint8_t*)(uintptr_t)skin) * f);
                if (k == 0x3333 || (F<int32_t>(kind, 0x41) != 2 && --left <= 0)) {
                    w = cw;
                    found = true;
                    break;
                }
                x = (float)((double)cw + x);
            }
            if (!found) continue;
        }
        uint8_t def[0x8b];
        memset(def, 0, sizeof def);
        F<float>(def, 7) = (float)((double)x + ox);
        F<float>(def, 0xf) = w;
        F<float>(def, 0x13) = h;
        F<float>(def, 0xb) = (float)((double)y + oy);
        F<uint16_t>(def, 0) = (uint16_t)(base + k - 0x332c);
        F<uint32_t>(def, 0x4b) = trainId;
        if (k != 0x3333) F<uint32_t>(def, 0x2f) = 0x90;
        uint8_t* tw = (uint8_t*)crt::operator_new(0xda);
        tw = tw ? TimerWidget_Construct(tw, 0, def) : nullptr;
        Window_AddChild(win, 0, tw, 0);
        Window_SendEvent5(win, 0, 0x82, F<uint16_t>(def, 0), 0x4beea0, 0, 0);
    }
    int32_t type = g_newGame[0x82] || F<int32_t>(g_world, 0x4aef) ? 0x87 : 0x65;
    Window_SendEvent5(win, 0, type, base + 5, 3, 0, 0);
}
RT3_REPLACE(0x4bf320, TrainRow_Build);

// 0x4bf9d0 (callback). Command 0x3333 opens the company report of train
// `id`'s city owner.
RT3_NOINLINE int32_t __fastcall Train_ReportCb(void*, int, int32_t cmd, uint32_t id) {
    if (Collection_IsValid(g_trains, 0, id)) {
        uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, id);
        if (cmd == 0x3333) CompanyReport_Show(*(const uint32_t*)Obj_CityRef(t), 0, (uint32_t)(uintptr_t)t);
    }
    return 0;
}
RT3_REPLACE(0x4bf9d0, Train_ReportCb);

// 0x4bfa20 (button callback). The four run-mode buttons (0x332c..0x332f) of
// train `id` step its mode (+0xd8, 0..3; multiplayer sends {peer, train,
// mode} as 0x3b), highlight the new one and show its tooltip (0x621f20).
RT3_NOINLINE int32_t __fastcall TrainRunMode_StepCb(uint8_t* w, int32_t button, int32_t ev, uint32_t id) {
    if (!Collection_IsValid(g_trains, 0, id)) return 0;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, id);
    if (ev < 0x332c || ev > 0x332f) return 0;
    int32_t mode = F<int32_t>(t, 0xd8) + 1;
    if (mode == 4) mode = 0;
    if (!IsMultiplayer()) {
        Train_SetRunning(t, 0, mode);
    } else {
        int32_t pkt[3];
        pkt[0] = Global6d40e4();
        pkt[1] = (int32_t)id;
        pkt[2] = mode;
        uint8_t* s = g_session;
        Net_Send(s, 0, (const char*)(s + 0x8f48), 0x3b, 0xc, (const uint8_t*)pkt, 5, 0);
    }
    const int32_t base = button - ev + 0x332c;
    for (int32_t i = 0; i < 4; ++i) Window_SendEvent5(w, 0, mode != i ? 0x87 : 0x65, (uint32_t)(base + i), 3, 0, 0);
    Ui_ShowTooltip(g_windowManager, 0, GetString(((const int32_t*)0x621f20)[mode]), 1, 1, -1);
    return 0;
}
RT3_REPLACE(0x4bfa20, TrainRunMode_StepCb);

// 0x4bfb30. Company card `co` at (x, y, w, h) (green frame for the player's
// own, world +0x25): its logo, its name (prefixed "<id> " by string 0xe00 with
// options +0x68) in the world font (+0x46a98), and three figures across the
// lower half: 0x279 cash (stat 0xd), 0x25f revenue (0x2c), 0x39d profit (0x2b),
// red when negative.
RT3_NOINLINE void __fastcall CompanyCard_Draw(uint8_t* co, int, float x, float y, float w, float h) {
    if (!co) return;
    if (F<int32_t>(co, 0x3b) == Obj_Get25(g_world)) Ui_FrameRect(0x5f00ff00, 0, x, y, w, h, 4.0f, 3);
    float ly = (float)((double)y + D(0x5d0250));
    float lx = (float)((double)x + D(0x5c9bc0));
    Glyph_Queue((uint8_t*)(uintptr_t)Company_Logo(co), 0, lx, ly, 0x3f800000, BitsF(0x3f0ccccd), BitsF(0x3f0ccccd), 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f,
                1.0f);
    char name[0x12c];
    strcpy(name, (const char*)(co + 4));
    if (F<int32_t>(g_app, 0x68)) {
        char num[0x100], text[0x12c];
        const char* n = Num_Format(num, 1, (double)F<int32_t>(co, 0), 10, 1, 0, 0);
        FormatText(text, GetString(0xe00), n, 0, 0, 0, 0, 0, 0, 0);
        strcat(text, name);
        strcpy(name, text);
    }
    uint8_t* font = F<uint8_t*>(g_world, 0x46a98);
    float ny = (float)((double)y + D(0x5d0790));
    double half = (double)h * D(0x5c8628);
    float hh = (float)half;
    float nw = (float)((double)w - D(0x5c86b8));
    float nx = (float)((double)x + D(0x5c9ba0));
    Font_DrawTextBox(font, 0, name, nx, ny, nw, hh, 0xffffffff, 2, 0, 0, 1.0f);
    float ry = (float)((double)y + half - D(0x5d0790));
    float rx = (float)((double)x + D(0x5d0788));
    char text[0x12c];
    static const int32_t labels[3] = { 0x279, 0x25f, 0x39d };
    static const int32_t stats[3] = { 0xd, 0x2c, 0x2b };
    for (int k = 0; k < 3; ++k) {
        if (k) rx = (float)((double)rx + D(0x5d0778));
        Font_DrawTextBox(F<uint8_t*>(g_world, 0x46a98), 0, GetString(labels[k]), rx, ry, BitsF(0x3dcccccd), hh, 0xffffffff, 3, 0, 0,
                         1.0f);
        Num_FormatSimple2(text, 0xb, Company_Stat(co, 0, stats[k], 0x2329), 0xb);
        uint32_t color = Company_Stat(co, 0, stats[k], 0x2329) < D(0x5c8608) ? 0xffff5f5f : 0xffffffff;
        float vx = (float)((double)rx + D(0x5d0780));
        Font_DrawTextBox(F<uint8_t*>(g_world, 0x46a98), 0, text, vx, ry, BitsF(0x3db851ec), hh, color, 2, 0, 0, 1.0f);
    }
    (void)font;
}
RT3_REPLACE(0x4bfb30, CompanyCard_Draw);

// 0x4bffc0. Map tooltip for picked object o: a station (+0x10f: "name" twice
// through string 0x39e, then 0xfdc when its building is active, else 0xfdd), a
// track node (+0x113: its cell x / y through 0x39f, then 0xfde) or string 0xbad.
// The second format reads and writes the same buffer.
RT3_NOINLINE void __fastcall Map_ShowPickTooltip(uint8_t* o) {
    if (!o) return;
    char cy[0x10], cx[0x10], text[0x1f0];
    if (F<uint32_t>(o, 0x10f)) {
        uint8_t* st = (uint8_t*)Collection_Get(g_stations, 0, F<uint32_t>(o, 0x10f));
        const char* name = (const char*)st + 0x46b;
        FormatText(text, GetString(0x39e), name, name, 0, 0, 0, 0, 0, 0);
        FormatText(text, GetString(Obj_BuildingStateActive(st) ? 0xfdc : 0xfdd), text, 0, 0, 0, 0, 0, 0, 0);
    } else if (F<uint32_t>(o, 0x113)) {
        uint8_t* n = (uint8_t*)Collection_Get(g_tracks, 0, F<uint32_t>(o, 0x113));
        char* sy = Num_Format(cy, 8, (double)Ftol(Obj_CenterCellY(n)), 10, 1, 0, 0);
        char* sx = Num_Format(cx, 8, (double)Ftol(Obj_CenterCellX(n)), 10, 1, 0, 0);
        FormatText(text, GetString(0x39f), sx, sy, 0, 0, 0, 0, 0, 0);
        FormatText(text, GetString(0xfde), text, 0, 0, 0, 0, 0, 0, 0);
    } else {
        strcpy(text, GetString(0xbad));
    }
    Ui_ShowTooltip(g_windowManager, 0, text, 1, 0, -1);
}
RT3_REPLACE(0x4bffc0, Map_ShowPickTooltip);

// 0x4c0160. The company's share panel at (x, y) (`compact` for the small
// version in white): the logo, the top three holders (faces, names, shares,
// the focused one highlighted) and everyone else, the six stat links
// (linkBase + 0..5, values of stats 0x1c..0x21), a frame with three lines, and
// a line chart of the chosen series (`show`[6]) over the last ten years
// (to yesterday on 1 Jan), each series scaled to a round top (pairs 0/1, 2/3,
// 3/4 share their axis), with year labels and two value axes.
RT3_NOINLINE void __fastcall CompanyStock_DrawChart(uint8_t* win, uint32_t linkBase, float x, float y, float, float, int32_t compact, uint32_t coId,
                                       uint8_t* faces, uint8_t** custom, int32_t, const uint8_t* show) {
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, coId);
    auto font = [] { return F<uint8_t*>(g_world, 0x46a98); };
    auto font2 = [] { return F<uint8_t*>(g_world, 0x46a9c); };
    if (!compact) {
        float gy = (float)((double)y - D(0x5d0810));
        float gx = (float)((double)x + D(0x5d0808));
        Glyph_Queue((uint8_t*)(uintptr_t)Company_Logo(co), 0, gx, gy, 0x3f800000, 0.5f, 0.5f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    }
    int32_t edi = compact ? 2 : 0;
    uint32_t color = compact ? 0xff000000 : 0xffffffff;
    float rowY = (float)((double)y + D(0x5c85b8));
    float colX = (float)((double)x + D(0x5d0800));
    uint32_t frameColor = compact ? 0xff000000 : 0xffaaa3a3;
    uint32_t lineColor = compact ? 0xffaaa3a3 : 0xff8a8383;
    int32_t edi1 = edi | 1;
    Font_DrawTextBox(font(), 0, GetString(0x3a0), colX, rowY, BitsF(0x3e89999a), BitsF(0x3cf5c28f), color, 2, 1, edi1, 1.0f);
    Font_DrawTextBox(font(), 0, GetString(0x3a1), colX, rowY, BitsF(0x3e89999a), BitsF(0x3cf5c28f), color, 3, 1, edi1, 1.0f);
    int32_t k = Company_NthHolder((int32_t*)co, 0, 1, 0);
    int32_t held = 0;
    int32_t i = 1;
    float row0 = (float)((double)y + D(0x5c9ba0));
    float row = row0;
    char buf[0x200], num[0x40];
    while (k > -1) {
        uint32_t c = color;
        if (compact) {
            uint8_t* f = F<uint8_t*>(g_windowManager, 0xc6d);
            if (f && F<uint16_t>(f, 4) == i + 0x3f0e) c = 0xff6d6b6e;
        }
        uint8_t* pl = (uint8_t*)Table_NthField4(g_players, 0, k);
        uint8_t face = pl[0x15c];
        float fy = (float)((double)row - D(0x5ca2d8));
        uint32_t img;
        if (face != 0x29) img = Table_Entry(faces, 0, face);
        else if (Obj_NameIndex(pl) > -1) img = Table_Entry(custom[Obj_NameIndex(pl)], 0, 0);
        else {
            pl[0x15c] = 0;
            img = Table_Entry(faces, 0, 0);
        }
        Glyph_Queue((uint8_t*)(uintptr_t)img, 0, colX, fy, 0x3f800000, BitsF(0x3f0f5c29), BitsF(0x3f0f5c29), 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
        float ny = (float)((double)row - D(0x5c8918));
        float nx = (float)((double)x + D(0x5d07f8));
        Font_DrawTextBox(font(), 0, (const char*)pl + 8, nx, ny, BitsF(0x3e3147ae), BitsF(0x3d8f5c29), c, 2, 0, edi, 1.0f);
        int32_t shares = F<int32_t>(pl, 0x15d + coId * 4);
        Num_Format(num, shares < 10000000 ? 1 : 0xd, (double)shares, 10, 1, 0, 0);
        held += F<int32_t>(pl, 0x15d + coId * 4);
        Font_DrawTextBox(font(), 0, num, colX, row, BitsF(0x3e89999a), BitsF(0x3cf5c28f), c, 3, 1, edi, 1.0f);
        ++i;
        k = Company_NthHolder((int32_t*)co, 0, i, 0);
        row = (float)((double)row + D(0x5d07f0));
        if (i - 1 >= 3) break;
    }
    float oy = (float)((double)y + D(0x5d07e8));
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(faces, 0, 0), 0, colX, (float)((double)oy - D(0x5ce8c8)), 0x3f800000, BitsF(0x3f147ae1), BitsF(0x3f147ae1),
                0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    Font_DrawTextBox(font(), 0, GetString(0x276), (float)((double)x + D(0x5d07f8)), (float)((double)oy - D(0x5c8918)), BitsF(0x3e3147ae),
                     BitsF(0x3d8f5c29), color, 2, 0, edi, 1.0f);
    int32_t others = F<int32_t>(co, 0x47) - held;
    Num_Format(num, others < 10000000 ? 1 : 0xd, (double)others, 10, 1, 0, 0);
    Font_DrawTextBox(font(), 0, num, colX, oy, BitsF(0x3e89999a), BitsF(0x3cf5c28f), color, 3, 1, edi, 1.0f);
    float lx = (float)((double)x + D(0x5c85b8));
    Font_DrawTextBox(font(), 0, GetString(0x3a2), lx, rowY, 0.2f, BitsF(0x3cf5c28f), color, 2, 1, edi1, 1.0f);
    uint32_t flags = edi | 0x20;
    float r3 = row0;
    float vx = 0.0f;
    static const int32_t stats[6] = { 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21 };
    static const int32_t styles[6] = { 9, 9, 0xa, 0xa, 0xa, 5 };
    for (int32_t n = 0; n < 6; ++n) {
        if (n) r3 = (float)((double)r3 + D(0x5c9b80));
        const char* tip = GetString(0x3a3 + n);
        TextLink_DrawPlain(win, font(), GetString(0x3a9 + n), lx, r3, BitsF(0x3e766666), BitsF(0x3cf5c28f), color, 2, 1, flags, tip, linkBase + n);
        Num_FormatSimple2(num, styles[n], Company_Stat(co, 0, stats[n], 0x2329), 0xb);
        if (!n) vx = (float)((double)x + D(0x5d07e0));
        Font_DrawTextBox(font(), 0, num, vx, r3, BitsF(0x3d99999a), BitsF(0x3cf5c28f), color, 3, 1, edi, 1.0f);
    }
    r3 = (float)((double)r3 + D(0x5c9ba0));
    float y2 = (float)((double)r3 + D(0x5c9ba0));
    float a = (float)((double)x + D(0x5d07d8));
    double bd = (double)x + D(0x5c86b0);
    float bf = (float)bd;
    Ui_Line(lineColor, 0, bf, y2, a, y2, 2.0f);
    float y3 = (float)((double)r3 + D(0x5c86b8));
    Ui_Line(lineColor, 0, bf, y3, a, y3, 2.0f);
    float y4 = (float)((double)r3 + D(0x5c8988));
    Ui_Line(lineColor, 0, bf, y4, a, y4, 2.0f);
    Ui_FrameRect(frameColor, 0, bf, r3, BitsF(0x3eee147b), BitsF(0x3e4ccccd), 2.0f, 3);
    uint32_t colors[6] = { compact ? 0xff12a019u : 0xff22c029u, 0xfff97908, 0xff0808c3, 0xffc2061c, 0xff0acbdc, 0xff9414c4 };
    int32_t full[6] = { 1, 1, 1, 1, 1, 1 };
    int32_t today = 1, axisA = -1, axisB = -1;
    uint8_t* world = g_world;
    int32_t from = F<uint16_t>(co, 0x157) - 1;
    int32_t to = F<uint16_t>(world, 0xd);
    if (to - from > 10) from = to - 10;
    if (world[0xf] == 1 && world[0x11] == 1 && to - 1 > from) {
        to = to - 1;
        today = 0;
    }
    int32_t count = to - from + 1;
    float bottom = (float)((double)r3 + D(0x5d07d0));
    bool labelled = false;
    float lo[6], hi[6], range[6];
    for (int32_t s = 0; s < 6; ++s) {
        if (!show[s]) continue;
        float top = 0.0f;
        for (int32_t yr = from; yr <= to; ++yr) {
            double prev = top;
            double v = crt::fabs(Company_Stat(co, 0, stats[s], yr));
            double nt = v < prev ? (double)top : crt::fabs(Company_Stat(co, 0, stats[s], yr));
            top = (float)nt;
        }
        top = (float)((double)top * D(0x5c85a8));
        if (top < D(0x5c85b8)) top = BitsF(0x3c23d70a);
        if (s < 2) top = (float)((1 - Ftol((double)top / Fl(0x5d07cc))) * 20);
        else if (s < 5) top = (float)((1 - Ftol((double)top / Fl(0x5d07c8))) * 2);
        else top = (float)((double)(1 - Ftol((double)top / D(0x5d07c0))) * D(0x5c9ba0));
        float low = s <= 1 ? 0.0f : -top;
        hi[s] = top;
        range[s] = (float)((double)top - low);
        lo[s] = low;
        int32_t with = -1;
        if (s == 1) with = axisA == 0 ? 0 : -1;
        else if (s == 3) with = axisA == 2 || axisB == 2 ? 2 : -1;
        else if (s == 4) with = axisA == 2 || axisB == 2 ? 2 : axisA == 3 || axisB == 3 ? 3 : -1;
        if (with < 0) {
            if (axisA == -1) axisA = s;
            else if (axisB == -1) axisB = s;
            continue;
        }
        float nl = lo[with] < low ? lo[with] : low;
        lo[s] = lo[with] = nl;
        float nh = hi[with] > top ? hi[with] : top;
        hi[s] = hi[with] = nh;
        float rg = (float)((double)hi[s] - lo[s]);
        range[s] = range[with] = rg;
        if (s == 4 && show[2] && show[3]) {
            lo[3] = lo[4];
            hi[3] = hi[4];
            range[3] = range[4];
        }
    }
    for (int32_t s = 0; s < 6; ++s) {
        if (!show[s]) continue;
        int32_t n = count - 1 < 1 ? 1 : count - 1;
        float dx = (float)(D(0x5d07b8) / (double)n);
        for (int32_t yr = from; yr <= to - 1; ++yr) {
            if (yr == from && !full[s]) continue;
            float t0 = 0.0f, t1 = 1.0f;
            if (yr == from) {
                uint32_t d = F<uint32_t>(co, 0x157);
                int32_t day = (int32_t)((uint8_t)(d >> 16)) * 0x1c + (uint8_t)F<uint32_t>(co, 0x15b) - 0x1d;
                float df = (float)day;
                t0 = (float)((double)df / Fl(0x5c99fc));
            }
            if (yr == to - 1 && today) {
                int32_t day = world[0xf] * 0x1c + world[0x11] - 0x1d;
                float df = (float)day;
                t1 = (float)((double)df / Fl(0x5c99fc));
            }
            double minT = (double)t0 + D(0x5c9b80);
            t1 = (float)(t1 > minT ? (double)t1 : minT);
            float v0 = (float)Company_Stat(co, 0, stats[s], yr);
            float v1 = (float)Company_Stat(co, 0, stats[s], yr + 1);
            float yrF = (float)yr, fromF = (float)from;
            float x0 = (float)((((double)yrF + t0) - fromF) * dx + x + D(0x5d07b0));
            float x1 = (float)((((double)yrF + t1) - fromF) * dx + x + D(0x5d07b0));
            float py0 = (float)(bottom - ((double)v0 - lo[s]) * Fl(0x5d07a8) / range[s]);
            float py1 = (float)(bottom - ((double)v1 - lo[s]) * Fl(0x5d07a8) / range[s]);
            Ui_Line(colors[s], 0, x0, py0, x1, py1, 2.0f);
        }
        if (!labelled) {
            if (count > 0) {
                double bdd = bottom;
                float tickY = (float)(bdd + D(0x5ca1a8));
                float textY = (float)(bdd + D(0x5d07a0));
                for (int32_t t = 0; t < count; ++t) {
                    float tf = (float)t;
                    float gx = (float)((double)tf * dx + x + D(0x5d07b0));
                    Ui_Line(frameColor, 0, gx, bottom, gx, tickY, 1.0f);
                    if (!t || (t == count && today)) FormatText(buf, (const char*)0x5c87a8, 0, 0, 0, 0, 0, 0, 0, 0);
                    else if (!show[0] && !show[1]) Num_Format(buf, 8, (double)(t + from), 10, 1, 0, 0);
                    else {
                        Num_Format(num, 1, (double)((t + from) % 100), 10, 1, 0, 0);
                        FormatText(buf, GetString(0x3af), num, 0, 0, 0, 0, 0, 0, 0);
                    }
                    Font_DrawTextBox(font2(), 0, buf, (float)((double)gx - D(0x5c9b50)), textY, BitsF(0x3d4ccccd), BitsF(0x3cf5c28f), color, 0, 1, edi, 1.0f);
                }
            }
            labelled = true;
        }
        if (s != axisA && s != axisB) continue;
        float step = (float)((double)range[s] / Fl(0x5c8568));
        bool left = axisA == s;
        for (int32_t q = 0; q < 5; ++q) {
            float qf = (float)q;
            float ly = (float)(bottom - (double)qf * Fl(0x5d0798));
            double val = (double)qf * step + lo[s];
            char lab[0x40];
            if (s < 2) Num_FormatSimple(lab, 9, val, 0xb);
            else if (s < 5) Num_FormatSimple(lab, 0xa, val, 0xb);
            else Num_Format(lab, 5, val, 10, 1, 0, 0);
            double off = left ? BitsD(0xbfac28f5c28f5c29ull) : BitsD(0x3fde147ae147ae14ull);
            float xx = (float)(bd + off);
            float ly2 = (float)((double)ly - D(0x5ca1a8));
            Font_DrawTextBox(font2(), 0, lab, xx, ly2, BitsF(0x3d4ccccd), BitsF(0x3cf5c28f), color, 0, 1, edi, 1.0f);
        }
    }
}
RT3_REPLACE(0x4c0160, CompanyStock_DrawChart);

RT3_SCENARIO_LOCK(0x4c15b0, Scenario_LockCb3f2, 0x3f2)

void __fastcall Dialog_ClearChildren94e8(uint8_t* w, int, int32_t) {
    if (F<uint16_t>(w, 4) == 0x947e) Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, 0x94e8, 0x950f, (uint32_t)(uintptr_t)w);
    else Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, 0x9510, 0x9537, (uint32_t)(uintptr_t)w);
}
RT3_REPLACE(0x4c1610, Dialog_ClearChildren94e8);

RT3_SCENARIO_LOCK(0x4c1650, Scenario_LockCb3f4, 0x3f4)

// 0x4c16b0. Copies entry i of the 12-byte array +0x5f.
RT3_NOINLINE uint32_t* __fastcall Obj_GetEntry5f(uint8_t* o, int, uint32_t* out, int32_t i) {
    const uint32_t* e = (const uint32_t*)(o + 0x5f + i * 12);
    out[0] = e[0];
    out[1] = e[1];
    out[2] = e[2];
    return out;
}
RT3_REPLACE(0x4c16b0, Obj_GetEntry5f);

RT3_NOINLINE int32_t Screen90() { return F<int32_t>(g_gameWindow, 0x90); }
RT3_REPLACE(0x4c16e0, Screen90);

// 0x4c16f0. Company selected in the game window (0x6d0818 +0x90), if valid.
RT3_NOINLINE void* SelectedCompany() {
    uint8_t* w = g_gameWindow;
    if (!w || F<int32_t>(w, 0x90) <= 0) return nullptr;
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(w, 0x90))) return nullptr;
    return Collection_Get(g_companies, 0, F<uint32_t>(w, 0x90));
}
RT3_REPLACE(0x4c16f0, SelectedCompany);

// 0x4c1730. Company view: the income table of the selected company (0x62be10,
// main view +0x90) at (x, y), in font 0x6cfe50, like the train one: labels
// (strings 3, 0x25f, 0x3b0, 0x3b1, 0x262), up to 3 years back plus "total"
// (year code 0x232a) of the stats 0x2c / 0x2e / 0x2f / 0x2b. With 0x6d07b4 at
// 2 a link 0xfb4 (tooltip 0xfb5, id 0x9494) goes above it in window 0x6cfe4c.
void __fastcall CompanyView_DrawIncomeTable(void*, int, float x, float y, float, float, int32_t) {
    uint8_t* view = g_gameWindow;
    if (!view || F<int32_t>(view, 0x90) <= 0 || !Collection_IsValid(g_companies, 0, F<uint32_t>(view, 0x90))) return;
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (!co) return;
    if (Global6d07b4Set2()) {
        const char* tip = GetString(0xfb5);
        float ly = (float)((double)y + D(0x5d0820));
        float lx = (float)((double)x + D(0x5d0110));
        TextLink_Draw(g_6cfe4c, F<uint8_t*>(g_world, 0x46a9c), GetString(0xfb4), lx, ly, 0.1875f, BitsF(0x3ccccccd), 2,
                      4, 0x101, tip, 0x9494, 0, 0);
    }
    static const int32_t labels[5] = { 3, 0x25f, 0x3b0, 0x3b1, 0x262 };
    static const int32_t stats[5] = { 0, 0x2c, 0x2e, 0x2f, 0x2b };
    static const uint32_t colX[6] = { 0x3c656042, 0x3dd2f1aa, 0x3e560419, 0x3ea147ae, 0x3ed78d50, 0x3f06e979 };
    int32_t cols = F<uint16_t>(g_world, 0xd) - F<uint16_t>(co, 0x157) + 3;
    if (cols > 5) cols = 5;
    float top = (float)((double)y + D(0x5caad0));
    char text[0x184];
    for (int32_t c = 0; c < cols; ++c) {
        float rowY = top;
        float cx = (float)((double)x + BitsF(colX[c]));
        float cw = (float)((double)BitsF(colX[c + 1]) - BitsF(colX[c]));
        int32_t align = (c != 0) + 2;
        for (int32_t r = 0; r < 5; ++r) {
            int32_t flags = 0;
            uint32_t color = 0xffffffff;
            if (c == 0) {
                strcpy(text, GetString(labels[r]));
            } else if (r == 0) {
                if (c == cols - 1) strcpy(text, GetString(0x263));
                else Num_Format(text, 8, (double)(F<uint16_t>(g_world, 0xd) - (c - 1)), 10, 1, 0, 0);
            } else {
                int32_t year = c == cols - 1 ? 0x232a : F<uint16_t>(g_world, 0xd) - (c - 1);
                float v = (float)Company_Stat(co, 0, stats[r], year);
                if (v < Fl(0x5c8598)) color = 0xffff5f5f;
                Num_FormatSimple2(text, 0xb, (double)v, 0xb);
                if (r == 3) flags = 4;
            }
            Font_DrawTextBox(g_6cfe50, 0, text, cx, rowY, cw, BitsF(0x3ca3d70a), color, align, 0, (uint32_t)flags, 1.0f);
            rowY = (float)((double)rowY + D(0x5c8918));
            if (r == 0) rowY = (float)((double)rowY + D(0x5c85b8));
            else if (r == 3) rowY = (float)((double)rowY + D(0x5d0818));
        }
    }
}
RT3_REPLACE(0x4c1730, CompanyView_DrawIncomeTable);

// 0x4c1ab0 (draw callback). Draws the card of the focused company.
void __fastcall CompanyView_DrawCardCb(void*, int, float x, float y, float w, float h, int32_t) {
    auto focused = [] {
        uint8_t* v = g_gameWindow;
        return v && F<int32_t>(v, 0x90) > 0 && Collection_IsValid(g_companies, 0, F<uint32_t>(v, 0x90));
    };
    if (!focused() || !Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    uint8_t* c = focused() ? (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90)) : nullptr;
    CompanyCard_Draw(c, 0, x, y, w, h);
}
RT3_REPLACE(0x4c1ab0, CompanyView_DrawCardCb);

// 0x4c1b60. Company view: the mouse moved onto territory `id` of the map
// (0x6cfe5c): shows overview mode 0x24 for it, points the map tooltip box
// 0x948c of the company window at it, and sets the tooltip: owned (0x3b2),
// for sale at its price (0x3b3, format 9) or closed (0x3b4) to the company.
RT3_NOINLINE void __fastcall CompanyView_HoverTerritory(int32_t id) {
    uint8_t* view = g_gameWindow;
    if (!view || F<int32_t>(view, 0x90) <= 0) return;
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    if (!Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    if (id == g_6cfe5c) return;
    g_6cfe5c = id;
    char text[0x3e4];
    text[0] = 0;
    uint8_t* tip = Widget_FindChild(g_6cfe4c, 0, 0x948c);
    Overview_SetMode(g_map, 0, 0x24, id, 0, 0, nullptr, 0, 0, 99999, 99999);
    if (tip) {
        F<int32_t>(tip, 0x117) = id;
        Window_SendEvent5(g_6cfe4c, 0, 0xba, 0x948c, 0, 0, 0);
    }
    if (id) {
        uint8_t* terr = (uint8_t*)Collection_Get(g_territories, 0, id);
        const char* tname = (const char*)terr + 4;
        if (Company_EntryFlag(SelectedCompany(), 0, id)) {
            const char* cname = (const char*)SelectedCompany() + 4;
            FormatText(text, GetString(0x3b2), tname, cname, tname, 0, 0, 0, 0, 0);
        } else if (Company_EntryOpen(SelectedCompany(), 0, id)) {
            uint32_t cid = F<uint32_t>(g_gameWindow, 0x90);
            const char* cname = (const char*)SelectedCompany() + 4;
            char num[0x200];
            double price = Obj_RoundedCost(terr, 0, cid);
            const char* sp = Num_FormatSimple(num, 9, price, 0xb);
            FormatText(text, GetString(0x3b3), tname, cname, sp, 0, 0, 0, 0, 0);
        } else {
            const char* cname = (const char*)SelectedCompany() + 4;
            FormatText(text, GetString(0x3b4), tname, cname, 0, 0, 0, 0, 0, 0);
        }
    }
    Ui_ShowTooltip(g_windowManager, 0, text, 3, 0, -1);
}
RT3_REPLACE(0x4c1b60, CompanyView_HoverTerritory);

// 0x4c1d30 (hover callback). The company view hovers the territory of row
// `row` of tree child `id` (none for a negative row).
void __fastcall CompanyView_HoverRowCb(int32_t id, int, int32_t, int32_t, int32_t, int32_t row, int32_t) {
    uint32_t t = 0;
    if (row >= 0)
        if (uint8_t* w = Widget_FindChild(g_6cfe4c, 0, id)) t = TreeView_NodeData(w, 0, row);
    CompanyView_HoverTerritory((int32_t)t);
}
RT3_REPLACE(0x4c1d30, CompanyView_HoverRowCb);

// 0x4c1d70 (draw callback). Territory list row `id` (EDX): with a player
// company (0x6d0818 +0x90), the territory's name (+4) and its access glyph
// (0x1a when the company holds the territory, else 0x19); the selected row
// (0x6cfe5c) gets the highlight box.
void __fastcall TerritoryList_DrawRowCb(void*, int32_t id, float x, float y, float w, float h, int32_t) {
    x = (float)((double)x + D(0x5d0830));
    uint8_t* player = g_gameWindow;
    if (!player || F<int32_t>(player, 0x90) <= 0) return;
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(player, 0x90))) return;
    uint8_t* company = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (!company) return;
    uint8_t* t = (uint8_t*)Collection_Get(g_territories, 0, (uint32_t)id);
    const float ty = (float)((double)y + Fl(0x5d082c));
    const float tx = (float)((double)x + D(0x5cab68));
    Font_DrawTextBox(g_6cfe50, 0, (const char*)t + 4, tx, ty, BitsF(0x3e800000), h, 0xffffffff, 2, 0, 0, 1.0f);
    const int32_t glyph = Company_EntryFlag(company, 0, id) ? 0x1a : 0x19;
    const float gx = (float)((double)x + Fl(0x5d0828));
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(g_6cfe58, 0, glyph), 0, gx, ty, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    if (g_6cfe5c == id) Ui_FillRect(0x18000000, 0, x, y, w, h, 3);
}
RT3_REPLACE(0x4c1d70, TerritoryList_DrawRowCb);

// 0x4c1ec0. Bond list row (draw callback for bond `idx` of the selected company,
// main view +0x90 in 0x62be10; bonds at +0x5f, 12 bytes {amount, year, rate}):
// "issued <year>" (string 0x3b5), the amount (money style 0xb) and the rate
// (style 5, right aligned). Columns sit 0x30 further right in the finance
// window layout (0x6d3c84 = 4, font world +0x46a9c; else font 0x6cfe50).
void __fastcall BondList_DrawRow(int32_t, int32_t idx, float x, float y, int32_t, float h, int32_t) {
    x = (float)((double)x + D(0x5d0830));
    uint8_t* view = g_gameWindow;
    if (!view || F<int32_t>(view, 0x90) <= 0) return;
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(view, 0x90))) return;
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (!c) return;
    const uint8_t* bond = c + 0x5f + idx * 12;
    int32_t amount = F<int32_t>((void*)bond, 0), year = F<int32_t>((void*)bond, 4);
    float rate = F<float>((void*)bond, 8);
    bool finance = g_language == 4;
    uint8_t* font = finance ? F<uint8_t*>(g_world, 0x46a9c) : g_6cfe50;
    int32_t shift = finance ? 0x30 : 0;
    char num[0x20], text[0xf4];
    FormatText(text, GetString(0x3b5), Num_Format(num, 8, (double)year, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0, 0);
    float w1 = (float)((double)(shift + 0x7d) / D(0x5ca2c8));
    float x1 = (float)((double)x + D(0x5cab68));
    Font_DrawTextBox(font, 0, text, x1, y, w1, h, 0xffffffff, 2, 0, 0, 1.0f);
    Num_FormatSimple2(text, 0xb, (double)amount, 0xb);
    float x2 = (float)((double)(shift + 0x7f) / D(0x5ca2c8) + x);
    Font_DrawTextBox(font, 0, text, x2, y, BitsF(0x3d800000), h, 0xffffffff, 0, 0, 0, 1.0f);
    Num_Format(text, 5, (double)rate, 10, 1, 0, 0);
    int32_t col3 = (g_language == 4 ? 5 : 0) + 0xe3;
    float x3 = (float)((double)col3 / D(0x5ca2c8) + x);
    Font_DrawTextBox(font, 0, text, x3, y, BitsF(0x3d266666), h, 0xffffffff, 3, 0, 0, 1.0f);
}
RT3_REPLACE(0x4c1ec0, BondList_DrawRow);

// 0x4c2110. Finance window row of the player's own company (not while +0x4a8b
// locks it): a "repay" button 0x9510 + idx (0x94e8 + idx in the bond list, list
// 0x947e), style 0x947b / 0x947c, tooltip string 0x3b6, at (x + 0x5d0850,
// y + 0x5ce8c8) in window 0x6cfe4c.
void __fastcall FinanceList_AddRowButton(uint8_t* list, int, int32_t idx, float x, float y, int32_t, int32_t) {
    uint8_t* view = g_gameWindow;
    if (!view || F<int32_t>(view, 0x90) <= 0) return;
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(view, 0x90))) return;
    if (!Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    uint8_t* sel = nullptr;
    view = g_gameWindow;
    if (view && F<int32_t>(view, 0x90) > 0 && Collection_IsValid(g_companies, 0, F<uint32_t>(view, 0x90)))
        sel = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    if (Obj_Company21(g_world) != sel) return;
    if (F<int32_t>(g_world, 0x4a8b)) return;
    bool bonds = F<uint16_t>(list, 4) == 0x947e;
    AddControlButton(g_6cfe4c, [&](uint8_t* d) {
        F<float>(d, 7) = (float)((double)x + D(0x5d0850));
        F<float>(d, 0xb) = (float)((double)y + D(0x5ce8c8));
        F<uint32_t>(d, 0xf) = 0x3c99999a;
        F<uint32_t>(d, 0x13) = 0x3c9eb852;
        d[0x89] = 6;
        d[0x8a] = 7;
        F<uint16_t>(d, 0x5f) = 0xd;
        F<uint16_t>(d, 5) = (uint16_t)(0x947b + (bonds ? 1 : 0));
        F<uint16_t>(d, 0x61) = 0xe;
        F<uint16_t>(d, 0) = (uint16_t)(idx + (bonds ? 0x94e8 : 0x9510));
        String_Assign(&F<char*>(d, 0x43), (const char*)0x5d0838);
        String_Assign(&F<char*>(d, 0x3f), GetString(0x3b6));
    });
}
RT3_REPLACE(0x4c2110, FinanceList_AddRowButton);

// 0x4c22e0 (hover callback). The company view hovers the territory of
// child 0x948c (+0x117) while the first argument is set, else none.
void __fastcall CompanyView_HoverTerritoryCb(void*, int, int32_t on, int32_t, int32_t, int32_t, int32_t) {
    uint8_t* w = Widget_FindChild(g_6cfe4c, 0, 0x948c);
    CompanyView_HoverTerritory(w && on ? F<int32_t>(w, 0x117) : 0);
}
RT3_REPLACE(0x4c22e0, CompanyView_HoverTerritoryCb);

// 0x4c2320. Company window destructor: releases and clears its three pack
// images (0x6cfe54, 0x6cfe50, 0x6cfe58), forgets the window (0x6cfe4c) and
// leaves overview mode 0x24.
void __fastcall CompanyWindow_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0858;
    static const uint32_t images[] = { 0x6cfe54, 0x6cfe50, 0x6cfe58 };
    for (uint32_t a : images) {
        Pack_Release(g_pack, 0, hook::Global<void*>(a));
        hook::Global<uint32_t>(a) = 0;
    }
    hook::As<uint32_t>(g_6cfe4c) = 0;
    if (F<int32_t>(g_map, 0x2175) == 0x24) Overview_Close(g_map);
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4c2320, CompanyWindow_Destruct);

// 0x4c23a0 (draw callback). Credit panel of the player's company: its stat
// 0x12 for this year (absolute, string 0x3b7), the credit rating's word
// (0x620d00, red at 4 and below, string 0x3b8), the weighted average when
// that stat is positive (0x3b9) and the bonus value (0x3ba).
void __fastcall CompanyCredit_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    auto company = []() -> uint8_t* {
        uint8_t* sel = g_gameWindow;
        if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_companies, 0, F<uint32_t>(sel, 0x90))) return nullptr;
        return (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    };
    if (!company()) return;
    y = (float)((double)y + D(0x5cab60));
    uint8_t* c = company();
    const float tw = (float)((double)w - D(0x5d0700));
    const float stat = (float)Company_Stat(c, 0, 0x12, F<uint16_t>(g_world, 0xd));
    const float mag = (float)crt::fabs((double)stat);
    char frame[0x108];
    char* num = frame + 8;
    Num_FormatSimple2(num, 0xb, (double)mag, 0xb);
    const float rh = BitsF(0x3cd3a06d);
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3b7), x, y, tw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Font_DrawTextBox(g_6cfe50, 0, num, x, y, tw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    y = (float)((double)y + D(0x5d0868));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3b8), x, y, tw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    const uint32_t color = Ftol(Company_CreditRating(c, 0, 0)) > 4 ? 0xffffffff : 0xffff5f5f;
    const char* word = *(const char* const*)(0x620d00 + Ftol(Company_CreditRating(c, 0, 0)) * 4);
    Font_DrawTextBox(g_6cfe50, 0, word, x, y, tw, rh, color, 3, 1, 0, 1.0f);
    y = (float)((double)y + D(0x5d0868));
    if (mag > D(0x5c8678)) {
        Num_Format(num, 6, Obj_WeightedAverage(c), 10, 1, 0, 0);
        Font_DrawTextBox(g_6cfe50, 0, GetString(0x3b9), x, y, tw, rh, 0xffffffff, 2, 1, 0, 1.0f);
        Font_DrawTextBox(g_6cfe50, 0, num, x, y, tw, rh, 0xffffffff, 3, 1, 0, 1.0f);
        y = (float)((double)y + D(0x5d0868));
    }
    Num_Format(num, 6, (double)Obj_ValueWithBonus((uint32_t*)company()), 10, 1, 0, 0);
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3ba), x, y, tw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Font_DrawTextBox(g_6cfe50, 0, num, x, y, tw, rh, 0xffffffff, 3, 1, 0, 1.0f);
}
RT3_REPLACE(0x4c23a0, CompanyCredit_DrawCb);

// 0x4c2720 (draw callback). Stock panel of the player's company: share
// price (+0xd7b, string 0x3a9), shares (+0x47, 0x2d4), market value
// (0x3bb), and after a gap this year's stat 0x20 per share (0x3bc) and in
// total (0x3bd); rows of 0x5d0868, values right aligned.
void __fastcall CompanyStock_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    auto company = []() -> uint8_t* {
        uint8_t* sel = g_gameWindow;
        if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_companies, 0, F<uint32_t>(sel, 0x90))) return nullptr;
        return (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    };
    if (!company()) return;
    y = (float)((double)y + D(0x5d0878));
    company();
    const float tw = (float)((double)w - D(0x5d0700));
    const float rh = BitsF(0x3cd3a06d);
    char frame[0x1fc];
    char* num = frame + 8;
    auto label = [&](int32_t id) { Font_DrawTextBox(g_6cfe50, 0, GetString(id), x, y, tw, rh, 0xffffffff, 2, 1, 0, 1.0f); };
    auto value = [&]() { Font_DrawTextBox(g_6cfe50, 0, num, x, y, tw, rh, 0xffffffff, 3, 1, 0, 1.0f); };
    label(0x3a9);
    Num_FormatSimple(num, 9, Company_FloatD7b(company()), 0xb);
    value();
    y = (float)((double)y + D(0x5d0868));
    label(0x2d4);
    Num_Format(num, 1, (double)F<int32_t>(company(), 0x47), 10, 1, 0, 0);
    value();
    y = (float)((double)y + D(0x5d0868));
    label(0x3bb);
    {
        uint8_t* a = company();
        const int32_t shares = F<int32_t>(company(), 0x47);
        const double price = Company_FloatD7b(a);
        const float fs = (float)shares;
        Num_FormatSimple2(num, 0xb, price * fs, 0xb);
    }
    value();
    y = (float)((double)y + D(0x5d0868));
    y = (float)((double)y + D(0x5d0870));
    label(0x3bc);
    Num_FormatSimple2(num, 0xa, Company_Stat(company(), 0, 0x20, F<uint16_t>(g_world, 0xd)), 0xb);
    value();
    y = (float)((double)y + D(0x5d0868));
    label(0x3bd);
    {
        uint8_t* a = company();
        const int32_t shares = F<int32_t>(company(), 0x47);
        const double stat = Company_Stat(a, 0, 0x20, F<uint16_t>(g_world, 0xd));
        Num_FormatSimple(num, 9, stat * (double)shares, 0xb);
    }
    value();
}
RT3_REPLACE(0x4c2720, CompanyStock_DrawCb);

// 0x4c2ca0. Refreshes the company window for the selected company (view
// +0x90) and tab 0x6cfe60: tab buttons (0x9472.. / 0x9476..), then per tab the
// chairman's face and caption (0x3be / 0x3bf; custom faces by image name),
// the list fillers of the bond / stock lists (0x947d / 0x947e, one row per
// bond), the history graph, or the station lists (0x948d / 0x948e, stations
// that are served). Action buttons 0x94d4..0x9537 are enabled only for the
// player's own company, 0x9538..0x959b only for other companies' chairmen,
// and the scenario's disabled actions (world +0x4a87..) stay off.
RT3_NOINLINE void __fastcall CompanyWindow_Refresh(uint8_t* win) {
    Collection* cos = g_companies;
    auto viewCompany = [&]() -> uint8_t* {
        uint8_t* view = g_gameWindow;
        if (!view || F<int32_t>(view, 0x90) <= 0 || !Collection_IsValid(cos, 0, F<uint32_t>(g_gameWindow, 0x90))) return nullptr;
        return (uint8_t*)Collection_Get(cos, 0, F<uint32_t>(g_gameWindow, 0x90));
    };
    if (!viewCompany()) return;
    auto send = [&](uint32_t type, uint32_t id, uint32_t a, uint32_t b) { Window_SendEvent5(win, 0, type, id, a, b, 0); };
    for (int32_t i = 0; i < 4; ++i) {
        send(i != g_6cfe60 ? 0x87 : 0x65, 0x9472 + i, 1, 0);
        send(i != g_6cfe60 ? 0x86 : 0x6d, 0x9476 + i, 3, 0);
    }
    send(g_6cfe60 != 1 ? 0x86 : 0x6d, 0x947b, 3, 0);
    send(0x86, 0x947c, 3, 0);
    send(g_6cfe60 != 3 ? 0x86 : 0x6d, 0x948f, 3, 0);
    send(0x86, 0x9490, 3, 0);
    Collection* pls = g_players;
    char buf[0x100];
    switch (g_6cfe60) {
    case 0: {
        uint32_t chair = F<uint32_t>(SelectedCompany(), 0x3b);
        int32_t face;
        if (chair) {
            uint8_t* p = (uint8_t*)Collection_Get(pls, 0, chair);
            FormatText(buf, GetString(0x3be), (const char*)(p + 8), 0, 0, 0, 0, 0, 0, 0);
            face = p[0x15c];
        } else {
            face = 0;
            FormatText(buf, GetString(0x3bf), 0, 0, 0, 0, 0, 0, 0, 0);
        }
        send(face ? 0x65 : 0x87, 0x9480, 3, 0);
        if (face != 0x29) {
            send(0x6c, 0x9480, (uint32_t)face, 0);
            send(0x70, 0x9480, 0x5d0880, 0);
        } else {
            send(0x6c, 0x9480, 0, 0);
            const char* name = (const char*)Obj_NamedEntry20((uint8_t*)Collection_Get(pls, 0, chair));
            if (NameTable_ContainsName(g_pack, 0, name, 0, 0))
                send(0x70, 0x9480, (uint32_t)(uintptr_t)Obj_NamedEntry20((uint8_t*)Collection_Get(pls, 0, chair)), 0);
            else {
                send(0x70, 0x9480, 0x5d0880, 0);
                ((uint8_t*)Collection_Get(pls, 0, chair))[0x15c] = 0;
            }
        }
        F<uint32_t>(Widget_FindChild(win, 0, 0x9480), 0x100) = 0x3f0ccccd;
        F<uint32_t>(Widget_FindChild(win, 0, 0x9480), 0x104) = 0x3f0ccccd;
        send(0x6f, 0x947f, (uint32_t)(uintptr_t)buf, 0);
        break;
    }
    case 1: {
        send(0x7a, 0x9488, 0x4c23a0, 0);
        send(0x7e, 0x947d, 0x4c2110, 0);
        send(0x7f, 0x947d, 0x4c1610, 0);
        send(0x7e, 0x947e, 0x4c2110, 0);
        send(0x7f, 0x947e, 0x4c1610, 0);
        Window_DeleteChildren(win, 0, 0x94e8, 0x950f, 0);
        Window_DeleteChildren(win, 0, 0x9510, 0x9537, 0);
        send(0x8b, 0x947d, 0, 0);
        send(0x8b, 0x947e, 0, 0);
        uint8_t* co = (uint8_t*)SelectedCompany();
        for (int32_t i = 0; i < F<int32_t>(co, 0x5b); ++i) {
            send(0x88, 0x947d, 0x5c87a8, (uint32_t)i);
            send(0x88, 0x947e, 0x5c87a8, (uint32_t)i);
        }
        break;
    }
    case 2: send(0x7a, 0x948a, 0x4c2720, 0); break;
    case 3: {
        send(0x8b, 0x948d, 0, 0);
        send(0x8b, 0x948e, 0, 0);
        Collection* sts = g_territories;
        for (int32_t j = 0; j < Collection_Count(sts); ++j) {
            if (!F<int32_t>(Collection_Get(sts, 0, Collection_IdAt(sts, 0, j, 0)), 0x31)) continue;
            if (!F<int32_t>(Collection_Get(sts, 0, Collection_IdAt(sts, 0, j, 0)), 0x3d)) continue;
            send(0x88, 0x948d, 0x5c87a8, Collection_IdAt(sts, 0, j, 0));
            send(0x88, 0x948e, 0x5c87a8, Collection_IdAt(sts, 0, j, 0));
        }
        break;
    }
    }
    send(g_6cfe60 != 1 ? 0x86 : 0x6d, 0x947b, 3, 0);
    send(0x86, 0x947c, 3, 0);
    uint8_t* co2 = viewCompany();
    uint32_t own = Obj_Company21(g_world) != co2 ? 0x87 : 0x65;
    for (uint32_t id = 0x94d4; id <= 0x9537; ++id) send(own, id, 3, 0);
    int32_t chair = F<int32_t>(viewCompany(), 0x3b);
    uint32_t other = Obj_Get25(g_world) != chair ? 0x65 : 0x87;
    for (uint32_t id = 0x9538; id <= 0x959b; ++id) send(other, id, 3, 0);
    uint8_t* world = g_world;
    if (F<int32_t>(world, 0x4a87)) {
        send(0x87, 0x94d8, 3, 0);
        send(0x87, 0x94d9, 3, 0);
    }
    if (F<int32_t>(g_world, 0x4a93)) {
        send(0x87, 0x94da, 3, 0);
        send(0x87, 0x94d5, 3, 0);
    }
    if (F<int32_t>(g_world, 0x4a8b)) {
        send(0x87, 0x94d7, 3, 0);
        send(0x87, 0x94d5, 3, 0);
    }
    if (F<int32_t>(g_world, 0x4a8f)) {
        send(0x87, 0x94d6, 3, 0);
        send(0x87, 0x94d4, 3, 0);
    }
    if (F<int32_t>(g_world, 0x4adb)) send(0x87, 0x94db, 3, 0);
    if (F<int32_t>(g_world, 0x4acb)) send(0x87, 0x94dc, 3, 0);
    if (F<int32_t>(g_world, 0x4acf)) send(0x87, 0x9538, 3, 0);
    if (!F<int32_t>(g_app, 0x68)) send(0x87, 0x9493, 3, 0);
}
RT3_REPLACE(0x4c2ca0, CompanyWindow_Refresh);

// 0x4c3470. Steps the company detail view (selection +0x90 of 0x6d0818) by
// `delta` companies through 0x62be10 in ranking order (wrapping), refreshing
// the company window and map-mode buttons; closes overview mode 0x24.
RT3_NOINLINE void __fastcall CompanyView_Step(int32_t delta) {
    int32_t id = F<int32_t>(g_gameWindow, 0x90);
    if (id <= 0) return;
    Collection* t = g_companies;
    if (!Collection_IsValid(t, 0, id)) return;
    int32_t rank = Table_LiveRankOf(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    int32_t n = Table_CountField3f(g_companies) + rank + delta;
    int32_t pos = n % Table_CountField3f(g_companies);
    uint32_t next = Table_NthIdField3f(g_companies, 0, pos);
    Panel_SetValue90(g_gameWindow, 0, (int32_t)next);
    CompanyWindow_Refresh(g_6cfe4c);
    Window_HighlightMapModes(g_gameWindow);
    Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xba, 0x948c, 0, 0, 0);
    if (F<int32_t>(g_map, 0x2175) == 0x24) Overview_Close(g_map);
}
RT3_REPLACE(0x4c3470, CompanyView_Step);

// 0x4c3540 / 0x4c3550. Next / previous company in the company view.
RT3_NOINLINE void __stdcall CompanyView_Next() { CompanyView_Step(1); }
RT3_REPLACE(0x4c3540, CompanyView_Next);

RT3_NOINLINE void __stdcall CompanyView_Prev() { CompanyView_Step(-1); }
RT3_REPLACE(0x4c3550, CompanyView_Prev);

// 0x4c3560 (draw callback). Bond panel of the player's company: title
// (string 0x3c8), the bond size 500000 (0x3c9), its interest rate with that
// much more debt (0x3ca), the 2% fee (0x3cb) and the footnote (0x3cc).
void __fastcall BondPanel_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    uint8_t* sel = g_gameWindow;
    if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_companies, 0, F<uint32_t>(sel, 0x90))) return;
    if (!Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    sel = g_gameWindow;
    uint8_t* c = nullptr;
    if (sel && F<int32_t>(sel, 0x90) > 0 && Collection_IsValid(g_companies, 0, F<uint32_t>(sel, 0x90)))
        c = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    const float rate = Company_BondRateWithDebt(c, 0, 0x7a120, 0x1e);
    const float tx = (float)((double)x + D(0x5d0830));
    const float ty = (float)((double)y + D(0x5d01d8));
    const float tw = (float)((double)w - D(0x5d0700));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3c8), tx, ty, tw, BitsF(0x3d7258bf), 0xffffffff, 2, 1, 0, 1.0f);
    float ry = (float)((double)ty + D(0x5d08a8));
    const float rx = (float)((double)tx + D(0x5d0180));
    const float rw = (float)((double)tw - D(0x5d08a0));
    const float rh = BitsF(0x3cda740e);
    char frame[0x204];
    char* num = frame + 0x10;
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3c9), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_FormatSimple2(num, 0xb, 500000.0, 0xb);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d0898));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3ca), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_Format(num, 5, (double)rate, 10, 1, 0, 0);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d0898));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3cb), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    const uint64_t kFee = 0x3f947ae147ae147bull;
    Num_Format(num, 5, *(const double*)&kFee, 10, 1, 0, 0);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d0898));
    const float nx = (float)((double)rx - D(0x5d0180));
    const float nw = (float)((double)rw + D(0x5d08a0));
    const float ny = (float)((double)ry + D(0x5d0890));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3cc), nx, ny, nw, rh, 0xffffffff, 2, 1, 0, 1.0f);
}
RT3_REPLACE(0x4c3560, BondPanel_DrawCb);

// 0x4c3890. Company window w: issue a bond for the selected company. Refused
// at 20 bonds (0x3cd) or, outside the editor, with a credit rating under 5
// (0x3ce with the rating's name); otherwise the bond dialog (panels 0x4c1650
// / 0x4c3560, left aligned when 0x6d3b4c is set) and on 0x3f2 a $500,000
// 30-year bond, or in multiplayer packet 0x23 with its rate.
RT3_NOINLINE void __fastcall CompanyWindow_IssueBond(uint8_t* w) {
    auto company = []() -> uint8_t* {
        uint8_t* v = g_gameWindow;
        if (!v || F<int32_t>(v, 0x90) <= 0) return nullptr;
        if (!Collection_IsValid(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return nullptr;
        return (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    };
    if (!company()) return;
    char text[0x200];
    if (F<int32_t>(company(), 0x5b) >= 0x14) {
        char num[0x200];
        const char* s = Num_Format(num, 1, 20.0, 10, 1, 0, 0);
        FormatText(text, GetString(0x3cd), s, 0, 0, 0, 0, 0, 0, 0);
        Message_Show(text, 1, 0);
        return;
    }
    int32_t rating = Ftol(Company_CreditRating((uint8_t*)SelectedCompany(), 0, 0));
    if (rating < 5 && !Global6d07b4Set2()) {
        FormatText(text, GetString(0x3ce), ((const char**)0x620d00)[rating], g_620d14, 0, 0, 0, 0, 0, 0);
        Message_Show(text, 1, 0);
        return;
    }
    uint32_t x = hook::As<uint32_t>(g_6d3b4c) ? 0x3ef5c28f : 0xbf800000;
    uint32_t r = Dialog_RunMessage((const char*)0x5c87a8, 2, x, 0xbf800000, 0x4c1650, nullptr, 0x4c3560, Prod_Bits(0x3e800000), 0, 0x1e, 0, nullptr, nullptr, 0,
                                   0, nullptr, 0);
    if (r != 0x3f2 || !SelectedCompany()) return;
    if (!IsMultiplayer()) {
        Company_IssueBond((uint8_t*)SelectedCompany(), 0, 500000, 30, 1, 0.0f);
        CompanyWindow_Refresh(w);
        return;
    }
    uint32_t pkt[5];
    pkt[0] = (uint32_t)Obj_Get25(g_world);
    pkt[1] = F<uint32_t>(g_gameWindow, 0x90);
    pkt[2] = 500000;
    pkt[3] = 30;
    float rate = Company_BondRateWithDebt((uint8_t*)SelectedCompany(), 0, 500000, 30);
    pkt[4] = *(uint32_t*)&rate;
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    Net_SendToHost(g_session, 0, 0x23, 0x14, (const uint8_t*)pkt);
}
RT3_REPLACE(0x4c3890, CompanyWindow_IssueBond);

// 0x4c3b50 (draw callback). Stock issue panel: title (string 0x3cf), the
// shares to issue (0x6cfe64, 0x3d0) and their price (0x6cfe68, 0x2d3), the
// 5% fee (0x3cb), the proceeds (shares x price x 0x5c8650, rounded to the
// thousand, 0x3d1) and the footnote (0x3d2).
void __fastcall StockIssuePanel_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    uint8_t* sel = g_gameWindow;
    if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_companies, 0, F<uint32_t>(sel, 0x90))) return;
    if (!Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    const float tx = (float)((double)x + D(0x5d0830));
    const float ty = (float)((double)y + D(0x5d01d8));
    const float tw = (float)((double)w - D(0x5d0700));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3cf), tx, ty, tw, BitsF(0x3d7258bf), 0xffffffff, 2, 1, 0, 1.0f);
    float ry = (float)((double)ty + D(0x5d08a8));
    const float rx = (float)((double)tx + D(0x5d0180));
    const float rw = (float)((double)tw - D(0x5d08a0));
    const float rh = BitsF(0x3cda740e);
    char frame[0x208];
    char* num = frame + 0x14;
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3d0), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_Format(num, 1, (double)g_issueShareCount, 10, 1, 0, 0);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d0898));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x2d3), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_FormatSimple(num, 9, (double)g_issueSharePrice, 0xb);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d0898));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3cb), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    const uint64_t kFee = 0x3fa999999999999aull;
    Num_Format(num, 5, *(const double*)&kFee, 10, 1, 0, 0);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d0898));
    ry = (float)((double)ry + D(0x5d08b0));
    const int32_t cost = Ftol((double)(g_issueSharePrice * g_issueShareCount) * D(0x5c8650));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3d1), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    const int32_t rounded = (cost + 500) / 1000 * 1000;
    Num_FormatSimple2(num, 0xb, (double)rounded, 0xb);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d0898));
    const float nx = (float)((double)rx - D(0x5d0180));
    const float nw = (float)((double)rw + D(0x5d08a0));
    const float ny = (float)((double)ry + D(0x5d0890));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3d2), nx, ny, nw, rh, 0xffffffff, 2, 1, 0, 1.0f);
}
RT3_REPLACE(0x4c3b50, StockIssuePanel_DrawCb);

// 0x4c3f30. Company window w: issue stock for the selected company. The block
// (0x6cfe64) is a twentieth of its shares (+0x47) in thousands, at least
// 1000, at the price the market would pay for it (0x6cfe68). Refused outside
// the editor under $5 a share (0x3d3, format 0xb) or when both earlier issues
// (+0x16b, +0x173) are less than a year old (0x3d4); otherwise the issue
// dialog (panels 0x4c1650 / 0x4c3b50) and on 0x3f2 the issue, or in
// multiplayer packet 0x29 (player, company, block, price).
RT3_NOINLINE void __fastcall CompanyWindow_IssueStock(uint8_t* w) {
    auto company = []() -> uint8_t* {
        uint8_t* v = g_gameWindow;
        if (!v || F<int32_t>(v, 0x90) <= 0) return nullptr;
        if (!Collection_IsValid(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return nullptr;
        return (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    };
    if (!company()) return;
    int32_t block = F<int32_t>(company(), 0x47) / 20 / 1000 * 1000;
    if (block < 1000) block = 1000;
    g_issueShareCount = block;
    uint8_t* co = company();
    int32_t issued = Ftol((double)-g_issueShareCount * D(0x5c8680));
    g_issueSharePrice = Ftol(Company_SharePrice(co, 0, issued, 0));
    char num[0x100], text[0x200];
    Num_FormatSimple2(num, 0xb, (double)g_issueSharePrice, 0xb);
    if (g_issueSharePrice < 5 && !Global6d07b4Set2()) {
        FormatText(text, GetString(0x3d3), num, 0, 0, 0, 0, 0, 0, 0);
        Message_Show(text, 1, 0);
        return;
    }
    uint8_t* d = g_world;
    if (DateDiffMinutes(*(GameDate*)(d + 0xd), *(GameDate*)((uint8_t*)SelectedCompany() + 0x16b)) < 0x76200 &&
        DateDiffMinutes(*(GameDate*)(g_world + 0xd), *(GameDate*)((uint8_t*)SelectedCompany() + 0x173)) < 0x76200 &&
        !Global6d07b4Set2()) {
        FormatText(text, GetString(0x3d4), 0, 0, 0, 0, 0, 0, 0, 0);
        Message_Show(text, 1, 0);
        return;
    }
    uint32_t x = hook::As<uint32_t>(g_6d3b4c) ? 0x3ef5c28f : 0xbf800000;
    uint32_t r = Dialog_RunMessage((const char*)0x5c87a8, 2, x, 0xbf800000, 0x4c1650, nullptr, 0x4c3b50, Prod_Bits(0x3e800000), 0, 0x1e, 0, nullptr, nullptr, 0,
                                   0, nullptr, 0);
    if (r != 0x3f2 || !SelectedCompany()) return;
    if (!IsMultiplayer()) {
        int32_t price = g_issueSharePrice, n = g_issueShareCount;
        Company_IssueShares((uint8_t*)SelectedCompany(), 0, n, 0, price);
        CompanyWindow_Refresh(w);
        return;
    }
    uint32_t pkt[4];
    pkt[0] = (uint32_t)Obj_Get25(g_world);
    pkt[2] = hook::As<uint32_t>(g_issueShareCount);
    pkt[1] = F<uint32_t>(g_gameWindow, 0x90);
    pkt[3] = hook::As<uint32_t>(g_issueSharePrice);
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    uint8_t* session = g_session;
    Net_Send(session, 0, (const char*)(session + 0x8f48), 0x29, 0x10, (const uint8_t*)pkt, 5, 0);
}
RT3_REPLACE(0x4c3f30, CompanyWindow_IssueStock);

// 0x4c4300. Company view (selected company, main view +0x90): the terms box at
// (x, y, w) in font 0x6cfe50: a title (0x3d5), then label / value rows: 0x3d0
// with the count 0x6cfe64, 0x2d3 with the amount 0x6cfe68 (style 9), 0x3d6
// with 5%, and 0x3d7 with count x amount x 0x5c8930 rounded to thousands; a
// footer line (0x3d8) under it.
void __fastcall CompanyView_DrawTerms(void*, int, float x, float y, float w, float, int32_t) {
    uint8_t* view = g_gameWindow;
    if (!view || F<int32_t>(view, 0x90) <= 0 || !Collection_IsValid(g_companies, 0, F<uint32_t>(view, 0x90))) return;
    if (!Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    const float rowH = BitsF(0x3cda740e);
    float tx = (float)((double)x + D(0x5d0830));
    float ty = (float)((double)y + D(0x5d01d8));
    float tw = (float)((double)w - D(0x5d0700));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3d5), tx, ty, tw, BitsF(0x3d7258bf), 0xffffffff, 2, 1, 0, 1.0f);
    float cy = (float)((double)ty + D(0x5d08a8));
    float cx = (float)((double)tx + D(0x5d0180));
    float cw = (float)((double)tw - D(0x5d08a0));
    char num[0x40];
    {
        const char* label = GetString(0x3d0);
        Font_DrawTextBox(g_6cfe50, 0, label, cx, cy, cw, rowH, 0xffffffff, 2, 1, 0, 1.0f);
        Num_Format(num, 1, (double)g_issueShareCount, 10, 1, 0, 0);
        Font_DrawTextBox(g_6cfe50, 0, num, cx, cy, cw, rowH, 0xffffffff, 3, 1, 0, 1.0f);
    }
    cy = (float)((double)cy + Fl(0x5d0898));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x2d3), cx, cy, cw, rowH, 0xffffffff, 2, 1, 0, 1.0f);
    Num_FormatSimple(num, 9, (double)g_issueSharePrice, 0xb);
    Font_DrawTextBox(g_6cfe50, 0, num, cx, cy, cw, rowH, 0xffffffff, 3, 1, 0, 1.0f);
    cy = (float)((double)cy + Fl(0x5d0898));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3d6), cx, cy, cw, rowH, 0xffffffff, 2, 1, 0, 1.0f);
    Num_Format(num, 5, BitsD(0x3fa999999999999aull), 10, 1, 0, 0);
    Font_DrawTextBox(g_6cfe50, 0, num, cx, cy, cw, rowH, 0xffffffff, 3, 1, 0, 1.0f);
    cy = (float)((double)cy + Fl(0x5d0898));
    cy = (float)((double)cy + D(0x5d08b0));
    double v = (double)g_issueSharePrice * (double)g_issueShareCount;
    v = v * D(0x5c8930);
    int32_t total = Ftol((v + D(0x5d0298)) / D(0x5c87f8)) * 1000;
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3d7), cx, cy, cw, rowH, 0xffffffff, 2, 1, 0, 1.0f);
    Num_FormatSimple2(num, 0xb, (double)total, 0xb);
    Font_DrawTextBox(g_6cfe50, 0, num, cx, cy, cw, rowH, 0xffffffff, 3, 1, 0, 1.0f);
    cy = (float)((double)cy + Fl(0x5d0898));
    cx = (float)((double)cx - D(0x5d0180));
    cw = (float)((double)cw + D(0x5d08a0));
    cy = (float)((double)cy + D(0x5d0890));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3d8), cx, cy, cw, rowH, 0xffffffff, 2, 1, 0, 1.0f);
}
RT3_REPLACE(0x4c4300, CompanyView_DrawTerms);

// 0x4c46d0. Company window w: buy back stock of the selected company. The
// block (0x6cfe64) is a twentieth of its shares (+0x47) in thousands, cut to
// what its cash (stat 0xd) buys at the price per share (0x6cfe68, at least
// 0x5c8770 / 0x5c8698), at least 1000 and at most the free shares. Refused
// without free shares (0x3d9), when the block reaches all shares (0x3da with
// their count) or, outside the editor, when the cash can't pay even 1000
// (0x3db); then shrinks the block by 1000s to the cash, runs the dialog
// (panel 0x4c4300) and on 0x3f2 buys back, or in multiplayer sends packet
// 0x2b (player, company, block).
RT3_NOINLINE void __fastcall CompanyWindow_BuyBackStock(uint8_t* w) {
    auto company = []() -> uint8_t* {
        uint8_t* v = g_gameWindow;
        if (!v || F<int32_t>(v, 0x90) <= 0) return nullptr;
        if (!Collection_IsValid(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return nullptr;
        return (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    };
    if (!company()) return;
    g_issueShareCount = F<int32_t>(company(), 0x47) / 20 / 1000 * 1000;
    uint8_t* co = company();
    int32_t issued = Ftol((double)g_issueShareCount * D(0x5c8680));
    g_issueSharePrice = Ftol(Company_SharePrice(co, 0, issued, 0));
    double per = (double)g_issueSharePrice * D(0x5c8698);
    if (D(0x5c8770) > per) per = 1.0;
    int32_t afford = Ftol(Company_Stat(company(), 0, 0xd, 0x2329) / per);
    if (afford < g_issueShareCount) g_issueShareCount = afford / 1000 * 1000;
    if (g_issueShareCount < 1000) g_issueShareCount = 1000;
    if (g_issueShareCount > Company_FreeShares(company())) g_issueShareCount = Company_FreeShares(SelectedCompany());
    char text[0x1f8];
    if (!g_issueShareCount) {
        FormatText(text, GetString(0x3d9), 0, 0, 0, 0, 0, 0, 0, 0);
        Message_Show(text, 1, 0);
        return;
    }
    if (g_issueShareCount >= F<int32_t>((uint8_t*)SelectedCompany(), 0x47)) {
        int32_t shares = F<int32_t>((uint8_t*)SelectedCompany(), 0x47);
        char num[0x1f0];
        const char* s = Num_Format(num, 1, (double)shares, 10, 1, 0, 0);
        FormatText(text, GetString(0x3da), s, 0, 0, 0, 0, 0, 0, 0);
        Message_Show(text, 1, 0);
        return;
    }
    issued = Ftol((double)g_issueShareCount * D(0x5c8680));
    g_issueSharePrice = Ftol(Company_SharePrice((uint8_t*)SelectedCompany(), 0, issued, 0));
    double cost = (double)g_issueSharePrice * (double)g_issueShareCount * D(0x5c8930);
    while (Company_Stat(company(), 0, 0xd, 0x2329) < cost && g_issueShareCount > 1000) {
        g_issueShareCount -= 1000;
        cost = (double)g_issueSharePrice * (double)g_issueShareCount * D(0x5c8930);
    }
    if (Company_Stat((uint8_t*)SelectedCompany(), 0, 0xd, 0x2329) < cost && !Global6d07b4Set2()) {
        Message_Show(GetString(0x3db), 1, 0);
        return;
    }
    uint32_t r = Dialog_RunMessage((const char*)0x5c87a8, 2, 0xbf800000, 0xbf800000, 0, nullptr, 0x4c4300, Prod_Bits(0x3e800000), 0, 0x1e, 0, nullptr, nullptr,
                                   0, 0, nullptr, 0);
    if (r != 0x3f2 || !SelectedCompany()) return;
    if (!IsMultiplayer()) {
        int32_t n = g_issueShareCount;
        Company_BuyBackShares((uint8_t*)SelectedCompany(), 0, n);
        CompanyWindow_Refresh(w);
        return;
    }
    uint32_t pkt[3];
    pkt[0] = (uint32_t)Obj_Get25(g_world);
    pkt[2] = hook::As<uint32_t>(g_issueShareCount);
    pkt[1] = F<uint32_t>(g_gameWindow, 0x90);
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    Net_SendToHost(g_session, 0, 0x2b, 0xc, (const uint8_t*)pkt);
}
RT3_REPLACE(0x4c46d0, CompanyWindow_BuyBackStock);

// 0x4c4c70. Company detail: the two small arrow buttons 0x99e8 / 0xc0f9
// ("Setup_Choose_Map_Buttons.imb") next to (x, y), for a valid selected company.
void __fastcall CompanyDetail_AddArrowButtons(uint8_t* win, int, float x, float y, int32_t, int32_t) {
    uint8_t* view = g_gameWindow;
    if (!view || F<int32_t>(view, 0x90) <= 0) return;
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(view, 0x90))) return;
    if (!Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return;
    x = (float)((double)x + D(0x5d0800));
    y = (float)((double)y + D(0x5d08d8));
    AddControlButton(win, [&](uint8_t* d) {
        F<float>(d, 0xb) = y;
        F<float>(d, 7) = x;
        F<uint32_t>(d, 0xf) = 0x3cdc28f6;
        F<uint32_t>(d, 0x13) = 0x3ce147ae;
        F<uint16_t>(d, 0x5f) = 2;
        F<uint16_t>(d, 0x61) = 3;
        d[0x89] = 2;
        d[0x8a] = 3;
        F<uint16_t>(d, 0) = 0x99e8;
        String_Assign(&F<char*>(d, 0x43), (const char*)0x5d08b8);
    });
    y = (float)((double)y + D(0x5caaf0));
    AddControlButton(win, [&](uint8_t* d) {
        F<float>(d, 0xb) = y;
        F<float>(d, 7) = x;
        F<uint32_t>(d, 0xf) = 0x3cdc28f6;
        F<uint32_t>(d, 0x13) = 0x3ceeeeef;
        F<uint16_t>(d, 0x5f) = 4;
        F<uint16_t>(d, 0x61) = 5;
        d[0x89] = 2;
        d[0x8a] = 3;
        F<uint16_t>(d, 0) = 0xc0f9;
        String_Assign(&F<char*>(d, 0x43), (const char*)0x5d08b8);
    });
}
RT3_REPLACE(0x4c4c70, CompanyDetail_AddArrowButtons);

 // namespace

// 0x4c4e30 (draw callback). Dividend panel of the player's company (0x6d0818
// +0x90): title (string 0x3dc), then shares (+0x47, string 0x2d4), the
// dividend per share 0x6cfe48 (0x3dd) and the total payout (0x3de), values
// right aligned.
void __fastcall DividendPanel_DrawCb(void*, int, float x, float y, float w, float h, int32_t) {
    auto company = []() -> uint8_t* {
        uint8_t* sel = g_gameWindow;
        if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_companies, 0, F<uint32_t>(sel, 0x90))) return nullptr;
        return (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    };
    if (!company()) return;
    const float tx = (float)((double)x + D(0x5d0830));
    const float ty = (float)((double)y + D(0x5d01d8));
    const float tw = (float)((double)w - D(0x5d0700));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3dc), tx, ty, tw, BitsF(0x3d7258bf), 0xffffffff, 0, 1, 0, 1.0f);
    float ry = (float)((double)ty + D(0x5d08a8));
    const float rx = (float)((double)tx + D(0x5d08f0));
    const float rw = (float)((double)tw - D(0x5d08e8));
    const float rh = BitsF(0x3d0f5c29);
    char frame[0x204];
    char* num = frame + 0x10;
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x2d4), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_Format(num, 1, (double)F<int32_t>(company(), 0x47), 10, 1, 0, 0);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d08e0));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3dd), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    Num_FormatSimple2(num, 0xa, (double)g_dividendPerShare, 0xb);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
    ry = (float)((double)ry + Fl(0x5d08e0));
    Font_DrawTextBox(g_6cfe50, 0, GetString(0x3de), rx, ry, rw, rh, 0xffffffff, 2, 1, 0, 1.0f);
    const float shares = (float)F<int32_t>(company(), 0x47);
    const float payout = (float)((double)shares * g_dividendPerShare);
    Num_FormatSimple2(num, 0xb, (double)payout, 0xb);
    Font_DrawTextBox(g_6cfe50, 0, num, rx, ry, rw, rh, 0xffffffff, 3, 1, 0, 1.0f);
}
RT3_REPLACE(0x4c4e30, DividendPanel_DrawCb);

// 0x4c5140 (event handler). Dividend dialog of the player's company: the
// step is 0x6cfe48 (per share, x 0x5c8c28) rounded to a multiple of 5 cents
// (at least 5); 0x99e8 raises the dividend (0x6cfe48) by it up to the
// company's maximum (or its current one if higher, +0x4b), refusing (string
// 0x3df) at the cap; 0xc0f9 lowers it by it, not below 0. Values keep the
// 0x5c85d8 granularity.
int32_t __fastcall DividendDialog_HandleEvent(void*, uint32_t* msg) {
    uint8_t* sel = g_gameWindow;
    if (!sel || F<int32_t>(sel, 0x90) <= 0 || !Collection_IsValid(g_companies, 0, F<uint32_t>(sel, 0x90))) return 1;
    if (!Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return 1;
    if (msg[0] != 0xcb) return 1;
    SelectedCompany();
    float step = (float)((double)g_dividendPerShare * Fl(0x5c8c28) / D(0x5c8610));
    step = (float)(Ftol(((double)step + D(0x5c87d8)) / D(0x5c8d68)) * 5);
    if (!(step > Fl(0x5c8554))) step = 5.0f;
    float cap = Company_MaxDividend((uint8_t*)SelectedCompany());
    if (!(cap > F<float>(SelectedCompany(), 0x4b))) cap = F<float>(SelectedCompany(), 0x4b);
    if (msg[1] == 0x99e8) {
        if (!(cap > (double)g_dividendPerShare + D(0x5d08f8))) {
            Message451(GetString(0x3df), 0xbf800000, 0xbf800000);
            return 0;
        }
        const int32_t v = Ftol((double)g_dividendPerShare * D(0x5c85d8) + step + D(0x5c8628));
        g_dividendPerShare = (float)((double)v / D(0x5c85d8));
        if (!(g_dividendPerShare < cap)) g_dividendPerShare = cap;
        return 0;
    }
    if (msg[1] == 0xc0f9) {
        const int32_t v = Ftol((double)g_dividendPerShare * D(0x5c85d8) - step + D(0x5c8628));
        g_dividendPerShare = (float)((double)v / D(0x5c85d8));
        if (g_dividendPerShare < Fl(0x5c8598)) hook::As<uint32_t>(g_dividendPerShare) = 0;
        return 0;
    }
    return 1;
}
RT3_REPLACE(0x4c5140, DividendDialog_HandleEvent);

// 0x4c5360. Company window w: the dividend dialog (panels 0x4c5140 /
// 0x4c4c70 / 0x4c4e30, value 0x6cfe48 from the company's +0x4b, 0x6cfe6c set
// while open) for the selected company; the new dividend is set directly, or
// in multiplayer (when changed) sent as packet 0x2d (player, company, value)
// with the busy cursor.
RT3_NOINLINE void __fastcall CompanyWindow_EditDividend(uint8_t* w) {
    auto company = []() -> uint8_t* {
        uint8_t* v = g_gameWindow;
        if (!v || F<int32_t>(v, 0x90) <= 0) return nullptr;
        if (!Collection_IsValid(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90))) return nullptr;
        return (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_gameWindow, 0x90));
    };
    if (!company()) return;
    uint32_t old = F<uint32_t>(company(), 0x4b);
    hook::As<uint32_t>(g_dividendPerShare) = old;
    g_6cfe6c = 1;
    Dialog_RunMessage((const char*)0x5c87a8, 1, 0xbf800000, 0xbf800000, 0x4c5140,
                      (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4c4c70, 0x4c4e30, Prod_Bits(0x3e333333), 0, 0x1e, 0, nullptr,
                      nullptr, 0, 0, nullptr, 0);
    g_6cfe6c = 0;
    if (!company()) return;
    if (!IsMultiplayer()) {
        uint32_t v = hook::As<uint32_t>(g_dividendPerShare);
        Obj_Set4b(SelectedCompany(), 0, (int32_t)v);
        CompanyWindow_Refresh(w);
        return;
    }
    if (Prod_Bits(old) == g_dividendPerShare) return;
    uint32_t pkt[3];
    pkt[0] = (uint32_t)Obj_Get25(g_world);
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    pkt[1] = F<uint32_t>(g_gameWindow, 0x90);
    pkt[2] = hook::As<uint32_t>(g_dividendPerShare);
    uint8_t* session = g_session;
    Net_Send(session, 0, (const char*)(session + 0x8f48), 0x2d, 0xc, (const uint8_t*)pkt, 5, 0);
}
RT3_REPLACE(0x4c5360, CompanyWindow_EditDividend);

// 0x4c5540. Company window constructor (vtable 0x5d0858, the open window
// 0x6cfe4c): two images and a font from the pack, the layout "CompanyDetail"
// (0x5d0900), the drawing / list callbacks of its panels, then a refresh.
RT3_NOINLINE uint8_t* __fastcall CompanyWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d0858;
    g_6cfe4c = w;
    g_6cfe5c = 0;
    g_6cfe54 = Pack_AcquireNamed(g_pack, 0, (const char*)0x5c9f98);
    g_6cfe58 = Pack_AcquireNamed(g_pack, 0, (const char*)0x5d0838);
    g_6cfe50 = Pack_AcquireFont(g_pack, 0, 2);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d0900);
    static const uint32_t events[][3] = { { 0x7a, 0x9470, 0x4c1730 }, { 0x7a, 0x9471, 0x4c1ab0 }, { 0x7a, 0x947d, 0x4c1ec0 }, { 0x7a, 0x947e, 0x4c1ec0 },
                                          { 0x7b, 0x948c, 0x4c22e0 }, { 0xba, 0x948c, 0 },        { 0x7a, 0x948d, 0x4c1d70 }, { 0x7a, 0x948e, 0x4c1d70 },
                                          { 0x7b, 0x948d, 0x4c1d30 }, { 0x7b, 0x948e, 0x4c1d30 } };
    for (const auto& e : events) Window_SendEvent5(w, 0, e[0], e[1], e[2], 0, 0);
    CompanyWindow_Refresh(w);
    return w;
}
RT3_REPLACE(0x4c5540, CompanyWindow_Construct);

 // namespace

// 0x4c56a0 (event handler). Company window w: type 0xcb (button) repays
// bond 0x94e8.. / 0x9510.. of the shown company (+0x5f, 12 bytes each:
// the cash must cover the principal, 0xbae, else 0xbaf asks with the
// principal x 0x5c8918, a fixed 2 % and the principal; MP message 0x25),
// and runs the page buttons: tool mode, tab pairs, dissolve, bankruptcy
// (0x94d6: not within 5 years of the last one +0x163, 0x3c5, and only for
// a company over two years old whose stat 0x2b is not positive in either
// of the last two years unless 0x4338c0, else 0x3c6; MP 0x27), bonds,
// stock, buyback, dividend, merger, resigning (0x3c4, MP 0x21) and the
// proxy fight. Most refuse in multiplayer (0x6d3b4c) with 0xe8c, or 0xe03
// when it is another player's tab (0x622b38). Type 0xca (select) picks a
// tab 0x9472..0x9475 (0x6cfe60), sets the cash (0x9494, a 0xe33 prompt)
// or, for the player's company, takes a territory from list 0x948c..0x948e:
// owned 0x3c0, closed 0x3c3, too dear 0x3c2, else 0x3c1 buys it (MP 0x2f);
// in the editor (0x6cec74 +0x68) 0xfdf / 0xfe0 toggle it. QUIRK: the
// territory path clears +0x117 of list 0x948c even when it was not found.
// Returns 1 for events it does not handle.
int32_t __fastcall CompanyWindow_HandleEvent(uint8_t* w, int, int32_t* ev) {
    char num1[0x1f0], num2[0x1f8], num3[0x1f8], text[0x200];
    if (ev[0] == 0xcb) {
        int32_t id = ev[1], idx;
        if (id >= 0x94e8 && id <= 0x950f) idx = id - 0x94e8;
        else if (id >= 0x9510 && id <= 0x9537) idx = id - 0x9510;
        else idx = -1;
        if (idx != -1) {
            if (g_6d3b4c) return HandleEventNote(0xe8c), 0;
            uint32_t entry[3];
            memcpy(entry, (uint8_t*)SelectedCompany() + 0x5f + idx * 0xc, 0xc);
            double cash = Company_Stat((uint8_t*)SelectedCompany(), 0, 0xd, 0x2329);
            double principal = (double)(int32_t)entry[0];
            if (principal > cash) {
                const char* have = Num_FormatSimple2(num3, 0xb, Company_Stat((uint8_t*)SelectedCompany(), 0, 0xd, 0x2329), 0xb);
                const char* fmt = GetString(0xbae);
                FormatText(text, fmt, have, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                HandleEventMsg(text, 1, kMsgNeg1);
                return 0;
            }
            uint32_t outA[3], outB[3];
            uint32_t* a = Obj_GetEntry5f((uint8_t*)SelectedCompany(), 0, outA, idx);
            uint32_t* b = Obj_GetEntry5f((uint8_t*)SelectedCompany(), 0, outB, idx);
            double pa = (double)(int32_t)a[0];
            int32_t fee;
            __asm {
                fld pa
                fmul qword ptr ds:[0x5c8918]
                call Rt3_Ftol
                mov fee, eax
            }
            const char* s1 = Num_FormatSimple(num2, 9, (double)IAbs(fee), 0xb);
            const char* s2 = Num_Format(num1, 5, 0.02, 0xa, 1, 0, 0);
            const char* s3 = Num_FormatSimple(num3, 9, (double)IAbs((int32_t)b[0]), 0xb);
            const char* fmt = GetString(0xbaf);
            FormatText(text, fmt, s3, s2, s1, nullptr, nullptr, nullptr, nullptr, nullptr);
            if (HandleEventMsg(text, 3, kMsgNeg1) != 0x3f2) return 0;
            if (!IsMultiplayer()) {
                if (!SelectedCompany()) return 0;
                Company_RemoveEntry(SelectedCompany(), 0, idx, 1);
                CompanyWindow_Refresh(w);
                return 0;
            }
            int32_t msg[3];
            msg[0] = Obj_Get25(g_world);
            msg[1] = CompanyViewId();
            msg[2] = idx;
            Cursor_Set(g_cursorManager, 0, 6, 0, 0);
            Net_SendToHost(g_session, 0, 0x25, 0xc, (const uint8_t*)msg);
            return 0;
        }
        // Sends message `type` (8 bytes: player, company) to the host.
        auto sendCompany = [](uint32_t type) {
            int32_t msg[2];
            msg[0] = Obj_Get25(g_world);
            msg[1] = CompanyViewId();
            Cursor_Set(g_cursorManager, 0, 6, 0, 0);
            Net_SendToHost(g_session, 0, type, 8, (const uint8_t*)msg);
        };
        switch (id) {
        case 0x948b:
            if (g_6d3b4c) break;
            View_SetToolMode(g_gameWindow, 0, 0, -1);
            return 0;
        case 0x9491:
            Window_SendEvent5(w, 0, 0x86, 0x948f, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x6d, 0x9490, 3, 0, 0);
            return 0;
        case 0x9492:
            Window_SendEvent5(w, 0, 0x6d, 0x948f, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x86, 0x9490, 3, 0, 0);
            return 0;
        case 0x9493:
            Company_Dissolve((uint8_t*)SelectedCompany());
            CompanyWindow_Refresh(w);
            return 0;
        case 0x94c0:
            Window_SendEvent5(w, 0, 0x6d, 0x947b, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x86, 0x947c, 3, 0, 0);
            return 0;
        case 0x94c1:
            Window_SendEvent5(w, 0, 0x86, 0x947b, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x6d, 0x947c, 3, 0, 0);
            return 0;
        case 0x94d6: {
            const int32_t year = F<uint16_t>(g_world, 0xd);
            if (year - F<int32_t>(SelectedCompany(), 0x163) < 5 && !Global6d07b4Set2()) {
                int32_t until = F<int32_t>(SelectedCompany(), 0x163) + 5;
                int32_t last = F<int32_t>(SelectedCompany(), 0x163);
                const char* s1 = Num_Format(num1, 8, (double)until, 10, 1, 0, 0);
                const char* s2 = Num_Format(num3, 8, (double)last, 10, 1, 0, 0);
                const char* fmt = GetString(0x3c5);
                FormatText(text, fmt, s2, s1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                HandleEventMsg(text, 1, kMsgNeg1);
                return 0;
            }
            bool allowed = false;
            int32_t y2 = F<uint16_t>(g_world, 0xd) - 2;
            if (F<uint16_t>(SelectedCompany(), 0x157) <= y2 &&
                !(Company_Stat((uint8_t*)SelectedCompany(), 0, 0x2b, y2) > *(const double*)rt3::Rdata(0x5c8608))) {
                int32_t y1 = F<uint16_t>(g_world, 0xd) - 1;
                allowed = !(Company_Stat((uint8_t*)SelectedCompany(), 0, 0x2b, y1) > *(const double*)rt3::Rdata(0x5c8608));
            }
            if (!allowed && !Global6d07b4Set2()) {
                HandleEventMsg(GetString(0x3c6), 1, kMsgNeg1);
                return 0;
            }
            if (HandleEventMsg(GetString(0x3c7), 3, kMsgNeg1) != 0x3f2 || !SelectedCompany()) return 0;
            if (IsMultiplayer()) {
                sendCompany(0x27);
                return 0;
            }
            Company_GoBankrupt((uint8_t*)SelectedCompany());
            CompanyWindow_Refresh(w);
            return 0;
        }
        case 0x94d7:
            if (g_6d3b4c && hook::As<int32_t>(g_622b38) != id) return HandleEventNote(0xe03), 0;
            CompanyWindow_IssueBond(w);
            return 0;
        case 0x94d8:
            if (g_6d3b4c && hook::As<int32_t>(g_622b38) != id) return HandleEventNote(0xe03), 0;
            CompanyWindow_IssueStock(w);
            return 0;
        case 0x94d9:
            if (g_6d3b4c) break;
            CompanyWindow_BuyBackStock(w);
            return 0;
        case 0x94da:
            CompanyWindow_EditDividend(w);
            return 0;
        case 0x94db:
            if (g_6d3b4c) break;
            Merger_OfferDialog();
            CompanyWindow_Refresh(w);
            return 0;
        case 0x94dc: {
            if (g_6d3b4c) break;
            if (HandleEventMsg(GetString(0x3c4), 3, kMsgNeg1) != 0x3f2 || !SelectedCompany()) return 0;
            if (IsMultiplayer()) {
                sendCompany(0x21);
                return 0;
            }
            int32_t player = Obj_Get25(g_world);
            Company_DropChairman((int32_t*)SelectedCompany(), 0, player);
            CompanyWindow_Refresh(w);
            return 0;
        }
        case 0x9538:
            ProxyFight_Start((uint32_t)CompanyViewId());
            CompanyWindow_Refresh(w);
            return 0;
        default:
            return 1;
        }
        HandleEventNote(0xe8c);
        return 0;
    }
    if (ev[0] != 0xca) return 1;
    const int32_t id = ev[1];
    if (id > 0x948e) {
        if (id != 0x9494) return 1;
        double cash = Company_Stat((uint8_t*)SelectedCompany(), 0, 0xd, 0x2329);
        Num_Format(num1, 8, cash, 10, 1, 0, 0);
        Dialog_RunMessage(GetString(0xe33), 9, kMsgNeg1, kMsgNeg1, 0, nullptr, 0, 0.0f, (uint32_t)(uintptr_t)num1, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        int32_t v = crt::atoi(num1);
        if (!SelectedCompany()) return 0;
        F<double>(SelectedCompany(), 0x11f7) = (double)v;
        return 0;
    }
    if (id < 0x948c) {
        if (id == 0x3e7) return 0;
        if (id <= 0x9471 || id > 0x9475) return 1;
        if (g_6d3b4c && hook::As<int32_t>(g_622b38) != id) return HandleEventNote(0xe03), 0;
        if (id != 0x9475 && F<int32_t>(g_map, 0x2175) == 0x24) Overview_Close(g_map);
        g_6cfe60 = ev[1] - 0x9472;
        CompanyWindow_Refresh(w);
        return 0;
    }
    void* mine = Obj_Company21(g_world);
    if (mine != SelectedCompany()) return 0;
    uint8_t* list = Widget_FindChild(g_6cfe4c, 0, 0x948c);
    int32_t terr = 0;
    bool have = false;
    if (list && ev[1] == 0x948c) {
        terr = F<int32_t>(list, 0x117);
        have = true;
    } else if (ev[1] == 0x948d || ev[1] == 0x948e) {
        terr = (int32_t)TreeView_SelectedData(Widget_FindChild(w, 0, ev[1]));
        have = true;
    }
    if (have && terr) {
        Ui_PlaySound(0x20, 0, 0x3f000000);
        uint8_t* t = (uint8_t*)Collection_Get(g_territories, 0, (uint32_t)terr);
        if (!F<int32_t>(g_app, 0x68)) {
            if (Company_EntryFlag(SelectedCompany(), 0, terr)) {
                uint8_t* co = (uint8_t*)SelectedCompany();
                const char* fmt = GetString(0x3c0);
                FormatText(text, fmt, (const char*)co + 4, (const char*)t + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                HandleEventMsg(text, 1, kMsgNeg1);
            } else if (!Company_EntryOpen(SelectedCompany(), 0, terr)) {
                uint8_t* co = (uint8_t*)SelectedCompany();
                const char* fmt = GetString(0x3c3);
                FormatText(text, fmt, (const char*)t + 4, (const char*)co + 4, (const char*)t + 4, nullptr, nullptr, nullptr, nullptr, nullptr);
                HandleEventMsg(text, 1, kMsgNeg1);
            } else {
                // A NaN price counts as too dear.
                bool afford = Obj_RoundedCost(t, 0, (uint32_t)CompanyViewId()) <= *(const float*)rt3::Rdata(0x5c8598);
                if (!afford) {
                    double cash = Company_Stat((uint8_t*)SelectedCompany(), 0, 0xd, 0x2329);
                    afford = Obj_RoundedCost(t, 0, (uint32_t)CompanyViewId()) <= cash;
                }
                if (!afford) {
                    uint8_t* co = (uint8_t*)SelectedCompany();
                    uint32_t company = (uint32_t)CompanyViewId();
                    uint8_t* co2 = (uint8_t*)SelectedCompany();
                    const char* have = Num_FormatSimple(num3, 9, Company_Stat(co, 0, 0xd, 0x2329), 0xb);
                    const char* need = Num_FormatSimple(num1, 9, Obj_RoundedCost(t, 0, company), 0xb);
                    const char* fmt = GetString(0x3c2);
                    FormatText(text, fmt, (const char*)t + 4, (const char*)co2 + 4, (const char*)t + 4, need, have, nullptr, nullptr, nullptr);
                    HandleEventMsg(text, 1, kMsgNeg1);
                } else {
                    uint32_t company = (uint32_t)CompanyViewId();
                    uint8_t* co = (uint8_t*)SelectedCompany();
                    const char* price = Num_FormatSimple(num1, 9, Obj_RoundedCost(t, 0, company), 0xb);
                    const char* fmt = GetString(0x3c1);
                    FormatText(text, fmt, (const char*)t + 4, (const char*)co + 4, (const char*)t + 4, price, nullptr, nullptr, nullptr, nullptr);
                    if (HandleEventMsg(text, 3, kMsgNeg1, 0x4c15b0) == 0x3f2) {
                        if (!IsMultiplayer()) {
                            Company_SetEntryFlag(SelectedCompany(), 0, terr, 1);
                            float cost = Obj_RoundedCost(t, 0, (uint32_t)CompanyViewId());
                            Company_AddStat(SelectedCompany(), 0, 0xd, -cost);
                        } else {
                            int32_t msg[4];
                            msg[0] = Obj_Get25(g_world);
                            msg[1] = CompanyViewId();
                            msg[2] = terr;
                            float cost = Obj_RoundedCost(t, 0, (uint32_t)CompanyViewId());
                            int32_t c;
                            __asm {
                                fld cost
                                call Rt3_Ftol
                                mov c, eax
                            }
                            msg[3] = c;
                            Cursor_Set(g_cursorManager, 0, 6, 0, 0);
                            Net_SendToHost(g_session, 0, 0x2f, 0x10, (const uint8_t*)msg);
                        }
                    }
                }
            }
        } else {
            bool owned = Company_EntryFlag(SelectedCompany(), 0, terr) != 0;
            uint8_t* co = (uint8_t*)SelectedCompany();
            const char* fmt = GetString(owned ? 0xfdf : 0xfe0);
            FormatText(text, fmt, (const char*)t + 4, (const char*)co + 4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            if (HandleEventMsg(text, 3, kMsgNeg1, 0x4c15b0) == 0x3f2) Company_SetEntryFlag(SelectedCompany(), 0, terr, owned ? 0 : 1);
        }
    }
    F<int32_t>(list, 0x117) = 0;
    Window_SendEvent5(w, 0, 0xba, 0x948c, 0, 0, 0);
    return 0;
}
RT3_REPLACE(0x4c56a0, CompanyWindow_HandleEvent);

}  // namespace rt3
