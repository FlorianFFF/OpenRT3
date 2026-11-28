// Reports.cpp - RT3.exe 0x4e2000-0x4ea000.
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

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
double __fastcall Obj_Sum3(void* o, int, int32_t i);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
const char* __fastcall LocalizeName(const char* name);
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Obj_Building(void* o);
int32_t __fastcall Obj_Get21(void* o);
int32_t __fastcall Obj_Get25(void* o);
void* __fastcall Obj_Ref25(void* o);
int32_t __fastcall Obj_StateKindIs4(void* o);
int32_t __fastcall Table_CountField3f(Collection* t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
void* __fastcall Obj_Company21(void* o);
int32_t __fastcall Obj_NameIndex(void* o);
int32_t __fastcall Company_NameIndex(void* c);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
int32_t __fastcall Company_TrainCount(void* c);
double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
void __fastcall Widget_SetVtbl(uint8_t* w);
float __fastcall Company_BondRate(uint8_t* c);
float __fastcall Company_CashRate(uint8_t* c);
double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
void __fastcall Reset_625a94();
double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
void* __fastcall Obj_City41(void* o);
void* __fastcall Obj_Ref388(void* o);
uint32_t __fastcall Car_SkinTexture(uint8_t* car);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
int32_t __fastcall Company_CountCargoSites(void* c, int, int32_t activeOnly);
void* __fastcall Company_Owner(void* c);
double __fastcall Obj_Remaining(void* o, int, int32_t i);
double __fastcall Obj_Float2e8(void* o, int, int32_t i);
double __fastcall Obj_Float2c0(void* o, int, int32_t i);
uint32_t __fastcall Company_Logo(uint8_t* c);
int32_t __fastcall Obj_YearOrNow(void* o);

void __fastcall TextLink_DrawPlain(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align,
                                   int32_t valign, uint32_t flags, const char* tip, int32_t id);

int32_t __fastcall Obj_YearReached(void* o);
int32_t World_AnyCondition();
uint32_t __fastcall Table_NthActive(Collection* t, int, int32_t n);
double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
double __fastcall Cargo_DemandWeight(void* c);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
uint32_t __fastcall Company_NthSite(int32_t* o, int, int32_t n, int32_t activeOnly);
double __fastcall Obj_Float4ec(void* o, int, int32_t i);
double __fastcall Obj_Float4c4(void* o, int, int32_t i);
double __fastcall Obj_Float53c(void* o, int, int32_t i);
double __fastcall Obj_Float514(void* o, int, int32_t i);
int32_t __fastcall Company_CountIdleBuildings(void* c);
void __fastcall Company_CacheBuildings(int32_t* o);
void __stdcall SetGlobal62bae8(uint8_t v);
uint32_t __fastcall Company_NthBuilding(int32_t* o, int, int32_t n);
int32_t __fastcall Company_NthHolder(int32_t* o, int, int32_t n, int32_t flag);
void __fastcall PortraitWindow_Layout(uint8_t* w);
int32_t __fastcall Table_LiveRankOf(Collection* t, int, uint32_t id);
uint32_t __fastcall Table_NthIdField3f(Collection* t, int, int32_t n);
uint32_t __fastcall Company_NthTrain(int32_t* o, int, int32_t n);
int32_t __fastcall Player_OwnsAllOf(uint8_t* p, int, uint32_t id);
int32_t __fastcall Obj_Level34b(void* o);
int32_t __fastcall Collection_Count(Collection* c);
int32_t Global6d40e4();

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void __fastcall PauseLock_Push(int32_t* p);
void __fastcall PauseLock_Pop(int32_t* p);
void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
void __fastcall CompanyWindow_ShowTab(uint8_t* w, int, int32_t tab, int32_t page, int32_t sel);
void __fastcall Ledger_Init(uint8_t* w, int, int32_t tab, int32_t year);
void __fastcall PortraitWindow_Destruct(uint8_t* w);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t IsMultiplayer();
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);

void __fastcall TextLink_Draw(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign, uint32_t flags,
                              const char* tip, int32_t id, uint32_t color, uint32_t hover);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

uint32_t __fastcall Collection_PositionOf(Collection* c, int, uint32_t id, int list);
void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __fastcall View_SetToolMode(uint8_t* view, int, uint32_t mode, int32_t requested);
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE int32_t IsMultiplayer();
void __fastcall Ledger_Open(int32_t company);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void __fastcall StrCopyN(char* dst, const char* src, size_t n);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void* __fastcall Collection_RecordAt2(Collection* t, int, uint32_t pos);
void __fastcall Obj_SetName8(uint8_t* o, int, const char* s);
int32_t __fastcall Net_HasPlayer(void*, int, const char* name);
void __fastcall CompanyWindow_StepCompany(uint8_t* w, int, int32_t delta);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE float __fastcall Company_BondRate(uint8_t* c);
RT3_NOINLINE float __fastcall Company_CashRate(uint8_t* c);
RT3_NOINLINE double __fastcall Company_Stat(uint8_t* c, int, int32_t id, int32_t year);
RT3_NOINLINE void __fastcall Events_Fire(Collection* events, int, uint32_t trigger, char* out, int32_t outSize);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
RT3_NOINLINE double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
RT3_NOINLINE double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
RT3_NOINLINE double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
RT3_NOINLINE char* __fastcall Train_DisplayName(uint8_t* train, int, int32_t numberOnly);
RT3_NOINLINE void __fastcall CompanyStock_DrawChart(uint8_t* win, uint32_t linkBase, float x, float y, float, float, int32_t compact, uint32_t coId, uint8_t* faces, uint8_t** custom, int32_t, const uint8_t* show);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall PortraitWindow_Layout(uint8_t* w);
RT3_NOINLINE int32_t __fastcall FacePicker_OnClick(uint8_t* w, uint32_t* ev);
void __fastcall PortraitWindow_Destruct(uint8_t* w);
RT3_NOINLINE void __fastcall CompanyWindow_ShowTab(uint8_t* w, int, int32_t tab, int32_t page, int32_t sel);
RT3_NOINLINE int32_t __fastcall Obj_YearOrNow(void* o);
RT3_NOINLINE void __fastcall CompanyWindow_StepCompany(uint8_t* w, int, int32_t delta);
int32_t __fastcall CompanyWindow_OnEvent(uint8_t* w, int, uint32_t* msg);
void __fastcall Ledger_Init(uint8_t* w, int, int32_t tab, int32_t year);
RT3_NOINLINE void __fastcall Ledger_Open(int32_t company);
RT3_NOINLINE void __fastcall Sprite_DrawFirstGlyph(uint8_t* o, int, float x, float y, int32_t, int32_t);
RT3_NOINLINE void __fastcall Sprite_DrawSecondGlyph(uint8_t* o, int, float x, float y, int32_t, int32_t);
RT3_NOINLINE void __fastcall CompanyWindow_DrawStockPage(uint8_t* w, int, float x, float y, float wd, float h);
RT3_NOINLINE void __fastcall EventPanel_Draw(uint8_t* p, int, float x, float y, float w, float h);
double __fastcall Train_LifetimeTotal(uint8_t* t);
RT3_NOINLINE void __fastcall PlayerDetail_Draw(uint8_t* w, int, float x, float y, float, float);
RT3_NOINLINE void __fastcall CompanyOverview_Draw(uint8_t* p, int, float x, float y, float w, float h);
RT3_NOINLINE void __fastcall CompanyRanking_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
RT3_NOINLINE void __fastcall PlayerRanking_Draw(uint8_t* p, int, float x, float y, float w, float, int32_t sortKey);
RT3_NOINLINE void __fastcall CompanyTrains_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
RT3_NOINLINE void __fastcall CompanyIndustries_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
RT3_NOINLINE void __fastcall CompanyStations_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey);
RT3_NOINLINE void __fastcall CargoRanking_Draw(uint8_t* p, int, float x, float y, float w, float, int32_t sortKey);
RT3_NOINLINE void __fastcall CompanyReport_Draw(uint8_t* w, int, float x, float y, float wd, float h, int32_t mode);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
RT3_NOINLINE double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
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


// Report rows (0x622188, 0x16 bytes, 20 per mode; row 0 is the header): stat
// +0xa (-1: blank / heading), extra gap +0xe, flags +0x12 (underline) / +0x13
// (has a total), label string +0x14, number style +0x18, tooltip +0x1c.
static inline int32_t ReportRowInt(int32_t row, uint32_t off) { return *(const int32_t*)(0x622188 + row * 0x16 + off); }

static inline uint8_t ReportRowByte(int32_t row, uint32_t off) { return *(const uint8_t*)(0x622188 + row * 0x16 + off); }

namespace {
uint32_t Message479(const char* text) {
    return Dialog_RunMessage(text, 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// The name prompt (string 0x5c87a8, the colour / portrait picker 0x4e2320 /
// 0x4e1fa0) editing name in place.
void NamePrompt479(char* name) {
    Dialog_RunMessage((const char*)0x5c87a8, 9, 0xbf800000, 0xbf800000, 0x4e2320,
                      (void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float))0x4e1fa0, 0, BitsF(0x3e2aaaab),
                      (uint32_t)(uintptr_t)name, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// A rename request to the host: our peer, the record's position, the name.
void SendName479(uint32_t type, Collection* table, uint32_t id, const char* name) {
    uint32_t req[0x22] = {};
    req[1] = Collection_PositionOf(table, 0, id, 0);
    req[0] = (uint32_t)Global6d40e4();
    strcpy((char*)&req[2], name);
    Net_SendToHost(g_session, 0, type, 0x88, (const uint8_t*)req);
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    ++g_6d10a0;
}
// A colour / portrait change request to the host (our peer, position, value).
void SendValue479(uint32_t type, uint32_t pos, uint32_t value) {
    uint32_t req[3];
    req[1] = pos, req[2] = value;
    req[0] = (uint32_t)Global6d40e4();
    Net_SendToHost(g_session, 0, type, 0xc, (const uint8_t*)req);
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    ++g_6d10a0;
}
}

// 0x4e2320. Click handler (0xcb) of the face / logo picker's next (0x3f20) and
// previous (0x3f21) buttons for the dialog 0x6d10a8: in mode 8 it steps the player
// +0x8c's portrait (+0x15c: 0..40 built in, 41 = the pack portrait named at
// +0xc47; pack table +0x429b4) past the ones other players use, showing
// "portraitL%02d" or the pack picture (event 0x70 on 0x3f1f); otherwise the
// company +0x88's logo (+0x37: 0..35 built in, -1 = the pack logo named at +0xd65;
// pack table +0x429b0; events 0x6c / 0x75). Sic: pack names are used as sprintf
// formats. 0 if handled.
RT3_NOINLINE int32_t __fastcall FacePicker_OnClick(uint8_t* w, uint32_t* ev) {
    if (ev[0] != 0xcb || (ev[1] != 0x3f20 && ev[1] != 0x3f21)) return 1;
    int32_t step = ev[1] == 0x3f20 ? 1 : -1;
    uint8_t* dlg = g_6d10a8;
    auto pack = []() { return g_pack; };
    uint8_t avail[0x8d];
    if (F<int32_t>(dlg, 0x78) == 8) {
        auto table = [&]() { return F<Collection*>(pack(), 0x429b4); };
        memset(avail, 0, sizeof avail);
        memset(avail, 1, 0x29);
        for (int32_t i = 0; i < Collection_Count(table()); ++i) avail[0x29 + i] = 1;
        for (int32_t i = 0; i < Collection_Count(g_players); ++i) {
            Collection* pc = g_players;
            uint8_t* p = (uint8_t*)Collection_Get(pc, 0, Collection_IdAt(pc, 0, (uint32_t)i, 0));
            if (p[0x15c] != 0x29) avail[p[0x15c]] = 0;
            else avail[0x29 + Obj_NameIndex(p)] = 0;
        }
        uint8_t* p = (uint8_t*)Collection_Get(g_players, 0, F<uint32_t>(g_6d10a8, 0x8c));
        int32_t idx = p[0x15c];
        if (idx == 0x29) idx = Obj_NameIndex(p) + 0x29;
        do {
            idx += step;
            if (step < 0 && idx == -1) idx = Collection_Count(table()) + 0x28;
            if (step > 0 && idx == Collection_Count(table()) + 0x29) idx = 0;
        } while (!avail[idx]);
        p[0x15c] = (uint8_t)idx;
        char buf[0x80];
        if (idx >= 0x29) {
            p[0x15c] = 0x29;
            const char* name = (const char*)Collection_Get(table(), 0, Collection_IdAt(table(), 0, (uint32_t)(idx - 0x29), 0));
            StrCopyN((char*)p + 0xc47, name, 0x1e);
            crt::sprintf(buf, name + 0x40);
        } else crt::sprintf(buf, (const char*)0x5d0ab0, idx);
        Window_SendEvent5(w, 0, 0x70, 0x3f1f, (uint32_t)(uintptr_t)buf, 0, 0);
        return 0;
    }
    auto table = [&]() { return F<Collection*>(pack(), 0x429b0); };
    memset(avail, 0, 0x88);
    memset(avail, 1, 0x24);
    for (int32_t i = 0; i < Collection_Count(table()); ++i) avail[0x24 + i] = 1;
    for (int32_t i = 0; i < Collection_Count(g_companies); ++i) {
        Collection* cc = g_companies;
        uint8_t* c = (uint8_t*)Collection_Get(cc, 0, Collection_IdAt(cc, 0, (uint32_t)i, 0));
        if (F<int32_t>(c, 0x37) != -1) avail[F<int32_t>(c, 0x37)] = 0;
        else avail[0x24 + Company_NameIndex(c)] = 0;
    }
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(g_6d10a8, 0x88));
    int32_t idx = F<int32_t>(c, 0x37);
    if (idx == -1) idx = Company_NameIndex(c) + 0x24;
    do {
        idx += step;
        if (step < 0 && idx == -1) idx = Collection_Count(table()) + 0x23;
        if (step > 0 && idx == Collection_Count(table()) + 0x24) idx = 0;
    } while (!avail[idx]);
    if (idx >= 0x24) {
        Window_SendEvent5(w, 0, 0x6c, 0x3f1f, 0, 0, 0);
        idx -= 0x24;
        Window_SendEvent5(w, 0, 0x75, 0x3f1f, (uint32_t)(uintptr_t)Collection_Get(table(), 0, Collection_IdAt(table(), 0, (uint32_t)idx, 0)), 0, 0);
        F<int32_t>(c, 0x37) = -1;
        StrCopyN((char*)c + 0xd65, (const char*)Collection_Get(table(), 0, Collection_IdAt(table(), 0, (uint32_t)idx, 0)), 0x14);
    } else {
        Window_SendEvent5(w, 0, 0x6c, 0x3f1f, (uint32_t)idx, 0, 0);
        Window_SendEvent5(w, 0, 0x75, 0x3f1f, 0x5ce8b0, 0, 0);
        F<int32_t>(c, 0x37) = idx;
    }
    return 0;
}
RT3_REPLACE(0x4e2320, FacePicker_OnClick);

// 0x4e2ab0. Destructor of the portrait selection window (vtable 0x5d1058): drops
// its buttons 0x3fac..0x464f and releases its images: +0xe55 / +0xb29 / +0xb25,
// the portraits (+0x761, one per pack portrait plus 0x29; +0x995, one per
// portrait), one per entry of 0x62ba8c (+0xb35) and the sprite tables +0xb2d / +0xb31.
RT3_NOINLINE void __fastcall PortraitWindow_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d1058;
    Reset_625a94();
    Window_DeleteChildren(w, 0, 0x3fac, 0x464f, 0);
    uint8_t* pack = g_pack;
    Pack_Release(pack, 0, F<void*>(w, 0xe55));
    Pack_Release(g_pack, 0, F<void*>(w, 0xb29));
    Pack_Release(g_pack, 0, F<void*>(w, 0xb25));
    auto portraits = []() { return Collection_Count(F<Collection*>(g_pack, 0x429b4)); };
    for (int32_t i = 0; i < portraits() + 0x29; ++i) Pack_Release(g_pack, 0, F<void*>(w, 0x761 + i * 4));
    for (int32_t i = 0; i < portraits(); ++i) Pack_Release(g_pack, 0, F<void*>(w, 0x995 + i * 4));
    for (int32_t i = 0; i < Collection_Count(g_cargoTypes); ++i)
        Pack_Release(g_pack, 0, F<void*>(w, 0xb35 + i * 4));
    if (F<void*>(w, 0xb2d)) Pack_Release(g_pack, 0, F<void*>(w, 0xb2d));
    if (F<void*>(w, 0xb31)) Pack_Release(g_pack, 0, F<void*>(w, 0xb31));
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4e2ab0, PortraitWindow_Destruct);

// 0x4e2c10. Switches the company window to tab `tab` page `page` (-1 keeps
// them) with selection `sel` (a company, or for tab 8 a player; -1 keeps it):
// clicks (0x33 to another kind of tab, else 0x34) unless nothing changes,
// drops the old content (0x3fac..0x464f), works out the page count +0x84 and
// the rows shown (list frame 0x3f0c, row buttons 0x3f48..) for the list tabs
// (2 companies, 7 players, 0xa trains, 0xc stations, 0xe cargo, 0xf
// industries), the holders of tab 6 (0x3f0f..0x3f11), records the step in the
// back / forward history (+0xa0, 10 entries of tab / page / selection, +0x118
// the position) unless 0x6d10ac, lays the window out and sets the tab and
// navigation buttons. Per-tab flags are in 0x6220a0 (13 bytes per tab).
RT3_NOINLINE void __fastcall CompanyWindow_ShowTab(uint8_t* w, int, int32_t tab, int32_t page, int32_t sel) {
    if (tab == -1) tab = F<int32_t>(w, 0x78);
    if (page == -1) page = F<int32_t>(w, 0x7c);
    int32_t cur = F<int32_t>(w, 0x78);
    auto info = [](int32_t t) { return (uint8_t*)(0x6220a0 + t * 0xd); };
    if (tab == cur && page == F<int32_t>(w, 0x7c) && sel == -1) return;
    bool click = true;
    if (tab == cur || cur == -1) {
        int32_t p = F<int32_t>(w, 0x7c);
        if ((page == p || p == -1) && sel <= 0) click = false;
    }
    if (click) Ui_PlaySound(cur != -1 && F<int32_t>(info(cur), 0) != F<int32_t>(info(tab), 0) ? 0x33 : 0x34, 0, 0x3f000000);
    if (tab == 8) {
        uint8_t* pl = (uint8_t*)Collection_Get(g_players, 0, F<uint32_t>(w, 0x8c));
        F<uint32_t>(w, 0x88) = F<uint32_t>(pl, 0x1dd);
        if (sel != -1) F<int32_t>(w, 0x8c) = sel;
    } else if (sel != -1)
        F<int32_t>(w, 0x88) = sel;
    else if (!Collection_IsValid(g_companies, 0, F<uint32_t>(w, 0x88)))
        F<int32_t>(w, 0x88) = Obj_Get21(g_world);
    Reset_625a94();
    Window_DeleteChildren(w, 0, 0x3fac, 0x464f, 0);
    bool changed = true;
    if (F<int32_t>(w, 0x78) == tab) changed = false;
    else F<int32_t>(w, 0x75c) = 0;
    F<int32_t>(w, 0x78) = tab;
    F<int32_t>(w, 0x7c) = page;
    F<int32_t>(w, 0x84) = -1;
    Collection* companies = g_companies;
    auto company = [&] { return Collection_Get(g_companies, 0, F<uint32_t>(w, 0x88)); };
    // Pages of 9: the count, the page count (+0x84, one less when the last
    // page is exactly full) and the rows on this page.
    auto pages = [&](auto count) {
        int32_t rows = count() % 9;
        int32_t q = count() / 9;
        F<int32_t>(w, 0x84) = q;
        if (page < q) return 9;
        if (rows) return rows;
        F<int32_t>(w, 0x84) = q - 1;
        return count() ? 9 : rows;
    };
    auto frame = [&](int32_t rows) {
        uint8_t* f = Widget_FindChild(w, 0, 0x3f0c);
        float h = (float)((double)rows * D(0x5ce8c0));
        F<float>(f, 0xf) = (float)((double)h + Fl(0x5d1090));
        F<float>(f, 0x17) = (float)(Fl(0x5d108c) - (double)h);
    };
    auto rowButtons = [&](int32_t rows) {
        for (int32_t i = 0; i < 9; ++i) Window_SendEvent5(w, 0, i >= rows ? 0x87 : 0x65, 0x3f48 + i, 3, 0, 0);
    };
    if (tab == 0xe) {
        Collection* cargo = g_cargoTypes;
        F<int32_t>(w, 0x84) = Collection_Count(cargo) / 9;
        if (Collection_Count(g_cargoTypes) % 9 == 0) --F<int32_t>(w, 0x84);
    } else if (tab == 0xa || tab == 0xc) {
        if (!Collection_IsValid(companies, 0, F<uint32_t>(w, 0x88))) F<int32_t>(w, 0x84) = 1;
        else {
            int32_t rows = tab == 0xa ? pages([&] { return Company_TrainCount(company()); })
                                      : pages([&] { return Company_CountCargoSites(company(), 0, 1); });
            frame(rows);
            rowButtons(rows);
        }
    } else if (tab == 0xf) {
        if (!Collection_IsValid(companies, 0, F<uint32_t>(w, 0x88))) F<int32_t>(w, 0x84) = 1;
        else {
            int32_t n = Company_CountIdleBuildings(company()) >= 400 ? 400 : Company_CountIdleBuildings(company());
            int32_t rows = n % 9, q = n / 9;
            F<int32_t>(w, 0x84) = q;
            if (page < q) rows = 9;
            else if (!rows) {
                F<int32_t>(w, 0x84) = q - 1;
                if (n) rows = 9;
            }
            frame(rows);
            rowButtons(rows);
        }
    } else if (tab == 7) {
        int32_t rows = pages([&] { return Table_CountField4(g_players); });
        frame(rows);
        if (changed) w[0x760] = 1;
        rowButtons(rows);
    } else if (tab == 2) {
        frame(pages([&] { return Table_CountField3f(g_companies); }));
    } else if (tab == 6) {
        Window_SendEvent5(w, 0, 0x6e, 0x3ef9, 0x5c87a8, 0, 0);
        int32_t* co = (int32_t*)company();
        int32_t shown = 0, id = 0x3f0f;
        char buf[0x200];
        int32_t k = Company_NthHolder(co, 0, 1, 0);
        while (k > -1) {
            uint8_t* pl = (uint8_t*)Table_NthField4(g_players, 0, k);
            FormatText(buf, GetString(0x4b1), (const char*)pl + 8, 0, 0, 0, 0, 0, 0, 0);
            Window_SendEvent5(w, 0, 0x65, id, 3, 0, 0);
            Window_SendEvent5(w, 0, 0x6e, id, (uint32_t)(uintptr_t)buf, 0, 0);
            Window_SendEvent5(w, 0, 0x7c, id, F<uint32_t>(pl, 0), 0, 0);
            ++shown;
            ++id;
            k = Company_NthHolder(co, 0, id - 0x3f0e, 0);
            if (id >= 0x3f12) break;
        }
        for (; shown < 3; ++shown) Window_SendEvent5(w, 0, 0x87, 0x3f0f + shown, 3, 0, 0);
    }
    if (!g_6d10ac && tab) {
        int32_t n = ++F<int32_t>(w, 0x118);
        if (n == 10) {
            memmove(w + 0xa0, w + 0xac, 0x6c);
            --F<int32_t>(w, 0x118);
        } else if (n < 10)
            for (int32_t i = n; i < 10; ++i) F<int32_t>(w, 0xa0 + i * 12) = F<int32_t>(w, 0xa4 + i * 12) = F<int32_t>(w, 0xa8 + i * 12) = -1;
        n = F<int32_t>(w, 0x118);
        F<int32_t>(w, 0xa0 + n * 12) = tab;
        F<int32_t>(w, 0xa4 + n * 12) = page;
        F<int32_t>(w, 0xa8 + n * 12) = sel;
    }
    PortraitWindow_Layout(w);
    int32_t t = F<int32_t>(w, 0x78);
    Window_SendEvent5(w, 0, t == 2 || t == 7 || t == 0xc || t == 0xf || t == 0xa ? 0x65 : 0x87, 0x3f0c, 3, 0, 0);
    if (F<int32_t>(info(F<int32_t>(w, 0x78)), 0)) {
        Window_SendEvent5(w, 0, F<int32_t>(info(F<int32_t>(w, 0x78)), 8) ? 0x65 : 0x87, 0x3ef8, 3, 0, 0);
        Window_SendEvent5(w, 0, F<int32_t>(w, 0x7c) >= F<int32_t>(w, 0x84) ? 0x87 : 0x65, 0x3efb, 3, 0, 0);
        Window_SendEvent5(w, 0, F<int32_t>(w, 0x7c) <= 0 ? 0x87 : 0x65, 0x3efc, 3, 0, 0);
        Window_SendEvent5(w, 0, F<int32_t>(w, 0x118) <= 0 ? 0x87 : 0x65, 0x3ef6, 3, 0, 0);
        int32_t n = F<int32_t>(w, 0x118);
        Window_SendEvent5(w, 0, n + 1 == 10 || F<int32_t>(w, 0xac + n * 12) == -1 ? 0x87 : 0x65, 0x3ef7, 3, 0, 0);
        t = F<int32_t>(w, 0x78);
        if (t == 0xa || t == 0xf || t == 0xc) Window_SendEvent5(w, 0, 0x87, 0x3f0e, 3, 0, 0);
    }
    auto sel_ = [&](bool on, int32_t id) { Window_SendEvent5(w, 0, on ? 0x6d : 0x86, id, 3, 0, 0); };
    sel_(F<int32_t>(w, 0x78) == 6, 0x3efd);
    sel_(F<int32_t>(w, 0x78) == 1, 0x3f0a);
    sel_(F<int32_t>(w, 0x78) == 8, 0x3f0b);
    sel_(F<int32_t>(w, 0x78) == 0xe, 0x3f0d);
    t = F<int32_t>(w, 0x78);
    sel_(t == 7 || t == 0xc || t == 0xf || t == 0xa, 0x3f12);
    sel_(info(F<int32_t>(w, 0x78))[0xc] != 0, 0x3ef5);
    sel_(F<int32_t>(w, 0x78) == 6, 0x3efd);
    t = F<int32_t>(w, 0x78);
    sel_(t == 3 || t == 5, 0x3f19);
    sel_(F<int32_t>(w, 0x78) == 4, 0x3f1c);
    uint8_t* co = F<uint32_t>(w, 0x88) ? (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(w, 0x88)) : nullptr;
    bool old = co && F<uint16_t>(g_world, 0xd) - F<uint16_t>(co, 0x157) + 1 > 3;
    for (int32_t id : { 0x3f1a, 0x3f1b, 0x3f1d, 0x3f1d }) Window_SendEvent5(w, 0, old ? 0x65 : 0x87, id, 3, 0, 0);
    bool any = Table_CountField3f(g_companies) > 0;
    for (int32_t id : { 0x3f07, 0x3f05, 0x3f06, 0x3f04, 0x3f13 }) Window_SendEvent5(w, 0, any ? 0x65 : 0x87, id, 3, 0, 0);
}
RT3_REPLACE(0x4e2c10, CompanyWindow_ShowTab);

// 0x4e39e0. +0x9c if set, else the current year.
RT3_NOINLINE int32_t __fastcall Obj_YearOrNow(void* o) {
    int32_t v = F<int32_t>(o, 0x9c);
    return v ? v : F<uint16_t>(g_world, 0xd);
}
RT3_REPLACE(0x4e39e0, Obj_YearOrNow);

// 0x4e3a00. Steps the company window to the company `delta` places on
// (wrapping), on the same tab, page 0.
RT3_NOINLINE void __fastcall CompanyWindow_StepCompany(uint8_t* w, int, int32_t delta) {
    Collection* cos = g_companies;
    int32_t n = Table_CountField3f(cos);
    if (n <= 0) return;
    int32_t at = 0;
    if (Collection_IsValid(g_companies, 0, F<uint32_t>(w, 0x88)))
        at = Table_LiveRankOf(g_companies, 0, F<uint32_t>(w, 0x88));
    uint32_t id = Table_NthIdField3f(g_companies, 0, (at + n + delta) % n);
    F<uint32_t>(w, 0x88) = id;
    CompanyWindow_ShowTab(w, 0, F<int32_t>(w, 0x78), 0, (int32_t)id);
}
RT3_REPLACE(0x4e3a00, CompanyWindow_StepCompany);

 // namespace

// 0x4e3a80 (event handler). The company / player window w (tab +0x78, page
// +0x7c, company +0x88, player +0x8c, year +0x9c, a 10-step history +0xa0 /
// +0x118 of tab / page / selection). Idle closes it when 0x6d10a4 asks;
// Escape closes, Enter goes up from the sub pages (+0x80). Clicks: the sort
// columns (+0x75c, repeat flips +0x760) rebuild the rows, the company /
// player rows open their pages, the check boxes 0x3efe..0x3f03 (0x622098..),
// list rows follow trains / stations / buildings on the map, the tab buttons
// (return 1), the scenario goals and the player / company rename prompts
// (string 0xb83 / 0xb85 for a taken name; in multiplayer requests 0x5d /
// 0x61 for names and 0x5f / 0x63 for the colour or portrait, put back until
// the host answers). Buttons: history back / forward, parent tab, page
// steps, the company / player steps and the year steps.
int32_t __fastcall CompanyWindow_OnEvent(uint8_t* w, int, uint32_t* msg) {
    auto tab = [&](int32_t t, int32_t page, int32_t sel) {
        CompanyWindow_ShowTab(w, 0, t, page, sel);
        return 0;
    };
    auto close = [&]() {
        msg[0] = 0xc9;
        return 1;
    };
    switch (msg[0]) {
    case 0:
        if (g_6d10a4) {
            g_6d10a4 = 0;
            return close();
        }
        return 1;
    case 3:
        if (msg[2] == 0x1b) return close();
        if (msg[2] != 0xd) return 1;
        switch (F<uint32_t>(w, 0x80)) {
        case 0: return tab(1, -1, -1);
        case 1: return tab(2, -1, -1);
        case 2: return tab(7, 0, -1);
        case 3: return tab(9, -1, -1);
        case 4: return close();
        }
        return 0;
    case 0xca:
        break;
    case 0xcb: {
        const uint32_t id = msg[1];
        // History: back (0x3ef6) / forward (0x3ef7) without recording.
        auto history = [&](int32_t n) {
            g_6d10ac = 1;
            CompanyWindow_ShowTab(w, 0, F<int32_t>(w, 0xa0 + n * 0xc), F<int32_t>(w, 0xa4 + n * 0xc), F<int32_t>(w, 0xa8 + n * 0xc));
            g_6d10ac = 0;
            return 0;
        };
        switch (id) {
        case 0x270f: return 0;
        case 0x3ef6:
            if (F<int32_t>(w, 0x118) <= 0) return 0;
            --F<int32_t>(w, 0x118);
            return history(F<int32_t>(w, 0x118));
        case 0x3ef7: {
            const int32_t n = F<int32_t>(w, 0x118);
            if (n >= 0xa || F<int32_t>(w, 0xac + n * 0xc) <= -1) return 0;
            ++F<int32_t>(w, 0x118);
            return history(F<int32_t>(w, 0x118));
        }
        case 0x3ef8:
            if (const int32_t parent = *(const int32_t*)(0x6220a8 + F<int32_t>(w, 0x78) * 0xd)) tab(parent, 0, -1);
            return 0;
        case 0x3efb: return tab(F<int32_t>(w, 0x78), F<int32_t>(w, 0x7c) + 1, -1);
        case 0x3efc: return tab(F<int32_t>(w, 0x78), F<int32_t>(w, 0x7c) - 1, -1);
        case 0x3f04:
        case 0x3f06:
            CompanyWindow_StepCompany(w, 0, -1);
            return 0;
        case 0x3f05:
        case 0x3f07:
            CompanyWindow_StepCompany(w, 0, 1);
            return 0;
        case 0x3f08:
        case 0x3f09: {
            Collection* players = g_players;
            int32_t pos = (int32_t)Collection_PositionOf(players, 0, F<uint32_t>(w, 0x8c), 0);
            if (id == 0x3f09) {
                ++pos;
                if (pos >= Table_CountField4(g_players)) pos = 0;
            } else if (--pos < 0) {
                pos = Table_CountField4(g_players) - 1;
            }
            const uint32_t player = Collection_IdAt(g_players, 0, (uint32_t)pos, 0);
            F<uint32_t>(w, 0x8c) = player;
            return tab(F<int32_t>(w, 0x78), 0, (int32_t)player);
        }
        case 0x3f1a:
        case 0x3f1d: {
            int32_t year = Obj_YearOrNow(w);
            uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(w, 0x88));
            if (!c) return 0;
            if (year - 3 > (int32_t)F<uint16_t>(c, 0x157) - 1 && year - 4 > (int32_t)F<uint16_t>(g_world, 0xd) - 0xc) --year;
            F<int32_t>(w, 0x9c) = year;
            return 0;
        }
        case 0x3f1b:
        case 0x3f1e: {
            int32_t year = Obj_YearOrNow(w);
            if (year < (int32_t)F<uint16_t>(g_world, 0xd)) ++year;
            F<int32_t>(w, 0x9c) = year;
            return 0;
        }
        }
        return 1;
    }
    default:
        return 1;
    }
    const int32_t id = (int32_t)msg[1];
    // Sort columns: a click on the sorted column flips the order.
    static const int32_t kSort[6][3] = { { 0x3fd4, 0x3fde, 0x3fd4 }, { 0x3fde, 0x3fe8, 0x3fdd }, { 0x3fe8, 0x3ff2, 0x3fe8 },
                                         { 0x3ff2, 0x3ffc, 0x3ff2 }, { 0x3ffc, 0x406a, 0x3ffc }, { 0x406a, 0x4074, 0x406a } };
    for (const auto& s : kSort) {
        if (id < s[0] || id >= s[1]) continue;
        const int32_t old = F<int32_t>(w, 0x75c);
        Ui_PlaySound(0x35, 0, 0x3f000000);
        const int32_t col = (int32_t)msg[1] - s[2];
        F<int32_t>(w, 0x75c) = col;
        w[0x760] = old == col ? w[0x760] == 0 : 0;
        Window_DeleteChildren(w, 0, 0x3fac, 0x464f, 0);
        return 0;
    }
    // Company rows of the four company lists.
    static const int32_t kRows[4][2] = { { 0x413c, 3 }, { 0x416e, 3 }, { 0x41a0, 4 }, { 0x41d2, 5 } };
    for (const auto& r : kRows) {
        if (id < r[0] || id >= r[0] + 0x32) continue;
        F<int32_t>(w, 0x88) = id - r[0];
        return tab(r[1], 0, -1);
    }
    if (id >= 0x3efe && id < 0x3f04) {
        *(uint8_t*)(0x61e19a + id) = (uint8_t)msg[2];
        return 0;
    }
    if (id >= 0x4074 && id <= 0x40a5) return tab(1, -1, id - 0x4074);
    if (id >= 0x3f48 && id < 0x3f52) {
        switch (F<int32_t>(w, 0x78)) {
        case 7: return tab(8, -1, F<int32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
        case 0xf:
            if (g_6d3b4c) return 0;
            View_SetToolMode(g_gameWindow, 0, 0xa, F<int32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
            World_FollowEntity(g_world, 0, 1, 2, F<int32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
            return 0;
        case 0xc: {
            if (g_6d3b4c) return 0;
            void* st = Collection_Get(g_stations, 0, F<uint32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
            View_SetToolMode(g_gameWindow, 0, 5, F<int32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
            World_FollowEntity(g_world, 0, 1, 2, F<int32_t>((uint8_t*)Obj_Building(st), 0x2a4));
            return 0;
        }
        case 0xa: {
            if (g_6d3b4c) return 0;
            void* t = Collection_Get(g_trains, 0, F<uint32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
            View_SetToolMode(g_gameWindow, 0, 2, F<int32_t>(F<uint8_t*>(msg, 0x2c), 0x4f));
            if (!Obj_City41(t)) return 0;
            World_FollowEntity(g_world, 0, 1, 1, F<int32_t>((uint8_t*)Obj_City41(t), 0x34a));
            return 0;
        }
        }
        return 0;
    }
    switch (id) {
    case 0x3ee6:
    case 0x3ee7: return close();
    case 0x3eee: return tab(1, -1, -1), 1;
    case 0x3eef: return tab(2, 0, -1), 1;
    case 0x3ef0: return tab(7, 0, -1), 1;
    case 0x3ef1: return tab(9, -1, -1), 1;
    case 0x3fad: return tab(3, -1, -1), 1;
    case 0x3fae: return tab(4, -1, -1), 1;
    case 0x3faf:
    case 0x3fb4: return tab(5, -1, -1), 1;
    case 0x3fb0:
    case 0x3fb7: return tab(6, -1, -1), 1;
    case 0x3fb1: return tab(0xa, 0, -1), 1;
    case 0x3fb2: return tab(0xc, 0, -1), 1;
    case 0x3fb3: return tab(0xe, 0, -1), 1;
    case 0x3fb5: return tab(0xf, -1, -1), 1;
    case 0x3f0f:
    case 0x3f10:
    case 0x3f11: {
        const int32_t player = F<int32_t>(F<uint8_t*>(msg, 0x2c), 0x4f);
        F<int32_t>(w, 0x8c) = player;
        return tab(8, 0, player);
    }
    case 0x3f13: {
        const int32_t company = F<int32_t>(F<uint8_t*>(msg, 0x2c), 0x4f);
        F<int32_t>(w, 0x88) = company;
        return tab(1, 0, company);
    }
    case 0x3fba: {
        uint8_t* world = g_world;
        if (IsMultiplayer() && strlen((const char*)world + 0x5ae9) > 1) {
            Message479((const char*)world + 0x5ae9);
            return 0;
        }
        Message479(strlen((const char*)world + 0x4f30) > 1 ? (const char*)world + 0x4f30 : GetString(0xfe));
        return 0;
    }
    case 0x3fb8: {
        if (g_6d10a0) return 0;
        uint8_t* p = (uint8_t*)Collection_Get(g_players, 0, F<uint32_t>(w, 0x8c));
        hook::As<uint32_t>(g_622090) = p[0x15c];
        char name[0x1e8];
        strcpy(name, (const char*)p + 8);
        NamePrompt479(name);
        if (name[0] && crt::stricmp(name, (const char*)p + 8) && strlen(name) <= 0x1e) {
            if (!IsMultiplayer())
                Obj_SetName8(p, 0, name);
            else if (Net_HasPlayer(g_session, 0, name) == 1)
                Message479(GetString(0xb83));
            else
                SendName479(0x5d, g_players, F<uint32_t>(w, 0x8c), name);
        }
        if (hook::As<uint32_t>(g_622090) == p[0x15c] || !IsMultiplayer()) return 0;
        const uint32_t pos = Collection_PositionOf(g_players, 0, F<uint32_t>(w, 0x8c), 0);
        const uint32_t picked = p[0x15c];
        p[0x15c] = hook::As<uint8_t>(g_622090);
        SendValue479(0x5f, pos, picked);
        return 0;
    }
    case 0x3fb9: {
        if (g_6d10a0) return 0;
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(w, 0x88));
        hook::As<uint32_t>(g_622094) = F<uint32_t>(c, 0x37);
        char name[0x1e8];
        strcpy(name, (const char*)c + 4);
        NamePrompt479(name);
        if (name[0] && crt::stricmp(name, (const char*)c + 4) && strlen(name) <= 0x32) {
            if (!IsMultiplayer()) {
                StrCopyN((char*)c + 4, name, 0x32);
            } else {
                bool taken = false;
                for (int32_t i = 0; i < Collection_Count(g_companies); ++i)
                    if (!crt::stricmp((const char*)Collection_RecordAt2(g_companies, 0, (uint32_t)i) + 4, name)) {
                        taken = true;
                        break;
                    }
                if (taken)
                    Message479(GetString(0xb85));
                else
                    SendName479(0x61, g_companies, F<uint32_t>(w, 0x88), name);
            }
        }
        if (hook::As<uint32_t>(g_622094) == F<uint32_t>(c, 0x37) || !IsMultiplayer()) return 0;
        const uint32_t pos = Collection_PositionOf(g_companies, 0, F<uint32_t>(w, 0x88), 0);
        const uint32_t picked = F<uint32_t>(c, 0x37);
        F<uint32_t>(c, 0x37) = hook::As<uint32_t>(g_622094);
        SendValue479(0x63, pos, picked);
        return 0;
    }
    }
    return 1;
}
RT3_REPLACE(0x4e3a80, CompanyWindow_OnEvent);

// 0x4e4b10. Opens the ledger (Ledger.win) on tab `tab`: resets the state and
// the history, picks the player's company and player, loads the font, the
// logo / frame / default images, the 41 portraits (portraitL%02d01), the
// two images per company type (0x6d4020 +0x429b4: +0x40, +0x20) and the cargo
// icons (default.imb when missing), installs the draw callbacks and tab
// flags, shows the tab, and on tab 0 shows the four digits of `year`.
RT3_NOINLINE void __fastcall Ledger_Init(uint8_t* w, int, int32_t tab, int32_t year) {
    F<int32_t>(w, 0x9c) = year;
    F<int32_t>(w, 0x80) = F<int32_t>(w, 0x78) = F<int32_t>(w, 0x7c) = F<int32_t>(w, 0x98) = -1;
    F<int32_t>(w, 0xb2d) = F<int32_t>(w, 0xb31) = 0;
    F<int32_t>(w, 0x94) = F<int32_t>(w, 0x90) = F<int32_t>(w, 0x8c) = F<int32_t>(w, 0x88) = 0;
    for (int32_t k = 0; k < 10; ++k) F<int32_t>(w, 0xa0 + k * 12) = F<int32_t>(w, 0xa4 + k * 12) = -1;
    F<int32_t>(w, 0x118) = -1;
    F<int32_t>(w, 0x75c) = 0;
    w[0x760] = 0;
    F<int32_t>(w, 0x84) = -1;
    if (g_world) {
        F<int32_t>(w, 0x88) = Obj_Get21(g_world);
        F<int32_t>(w, 0x8c) = Obj_Get25(g_world);
    }
    auto pack = [] { return g_pack; };
    F<uint8_t*>(w, 0xe55) = Pack_AcquireFont(pack(), 0, 8);
    F<uint8_t*>(w, 0xb29) = Pack_AcquireNamed(pack(), 0, (const char*)0x5c9f98);
    F<uint8_t*>(w, 0xb31) = Pack_AcquireNamed(pack(), 0, (const char*)0x5d10ac);
    F<uint8_t*>(w, 0xb25) = Pack_AcquireNamed(pack(), 0, (const char*)0x5d0f04);
    char buf[0x100];
    for (int32_t i = 0; i < 0x29; ++i) {
        crt::sprintf(buf, (const char*)0x5d0ab0, i);
        F<uint8_t*>(w, 0x761 + i * 4) = Pack_AcquireNamed(pack(), 0, buf);
    }
    for (int32_t j = 0; j < Collection_Count(F<Collection*>(pack(), 0x429b4)); ++j) {
        Collection* types = F<Collection*>(pack(), 0x429b4);
        uint8_t* rec = (uint8_t*)Collection_Get(types, 0, Collection_IdAt(types, 0, j, 0));
        F<uint8_t*>(w, 0x805 + j * 4) = Pack_AcquireNamed(pack(), 0, (const char*)rec + 0x40);
        F<uint8_t*>(w, 0x995 + j * 4) = Pack_AcquireNamed(pack(), 0, (const char*)rec + 0x20);
    }
    for (int32_t k = 0; k < Collection_Count(g_cargoTypes); ++k) {
        Collection* cargo = g_cargoTypes;
        uint8_t* rec = (uint8_t*)Collection_Get(cargo, 0, Collection_IdAt(cargo, 0, k, 0));
        FormatText(buf, (const char*)0x5d01bc, (const char*)rec + 4, 0, 0, 0, 0, 0, 0, 0);
        if (!NameTable_ContainsName(pack(), 0, buf, 0, 0)) FormatText(buf, (const char*)0x5d10a0, 0, 0, 0, 0, 0, 0, 0, 0);
        F<uint8_t*>(w, 0xb35 + k * 4) = Pack_AcquireNamed(pack(), 0, buf);
    }
    Window_Load(w, 0, BitsF(0x3deccccd), BitsF(0x3c8f5c29), (const char*)0x5d1094);
    Window_SendEvent5(w, 0, 0x7a, 0x3ee4, 0x4e1cd0, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x3ef4, 0x4e1d00, 0, 0);
    Window_SendEvent5(w, 0, 0x7a, 0x3ee5, 0x4e1d30, 0, 0);
    for (int32_t i = 0; i < 9; ++i) Window_SendEvent5(w, 0, 0x7b, 0x3f48 + i, 0x4e1f70, 0, 0);
    for (int32_t i = 0; i < 6; ++i) Window_SendEvent5(w, 0, 0x74, 0x3efe + i, hook::Global<uint8_t>(0x622098 + i), 0, 0);
    ++g_ccba14;
    CompanyWindow_ShowTab(w, 0, tab, 0, F<int32_t>(w, 0x88));
    --g_ccba14;
    if (tab) return;
    char num[0x100], digit[0x10];
    Num_Format(num, 8, (double)F<int32_t>(w, 0x9c), 10, 1, 0, 0);
    for (int32_t i = 0; i < 4; ++i) {
        crt::strncpy(digit, num + i, 1);
        digit[1] = 0;
        Window_SendEvent5(w, 0, 0x6c, 0x3f14 + i, crt::atoi(digit) + 2, 0, 0);
    }
}
RT3_REPLACE(0x4e4b10, Ledger_Init);

// 0x4e4ee0 (company in ECX). Opens the stock market / ledger window (0xe59
// bytes, 0x6d10a8) on `company` (0: the whole market for last year), or just
// switches the open one; message 0xf3b when the market is disabled (0x6cec7c
// +0x82). Plays its open / close sounds 0x31 / 0x32.
RT3_NOINLINE void __fastcall Ledger_Open(int32_t company) {
    if (g_newGame[0x82]) {
        Dialog_RunMessage(GetString(0xf3b), 1, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
        return;
    }
    if (uint8_t* open = g_6d10a8) {
        if (company) CompanyWindow_ShowTab(open, 0, company, 0, -1);
        return;
    }
    Ui_PlaySound(0x31, 0, 0x3f000000);
    hook::As<uint32_t>(g_6d10a4) = 0;
    int32_t mode = F<int32_t>(g_gameWindow, 0x8c);
    int32_t cursor = F<int32_t>(g_cursorManager, 4);
    Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    uint8_t* w = (uint8_t*)crt::operator_new(0xe59);
    if (w) {
        Widget_SetVtblRet(w);
        F<uint32_t>(w, 0) = 0x5d1058;
    }
    g_6d10a8 = w;
    Ledger_Init(w, 0, company, company ? 0 : F<uint16_t>(g_world, 0xd) - 1);
    PauseLock_Push(hook::As<int32_t*>(g_app));
    Ui_RunModal(hook::As<uint8_t**>(g_windowManager), 0, g_6d10a8, 1, 0, 0x14, 0);
    PauseLock_Pop(hook::As<int32_t*>(g_app));
    Windows_Remove(g_windowManager, 0, g_6d10a8);
    if (uint8_t* p = g_6d10a8) {
        PortraitWindow_Destruct(p);
        crt::operator_delete(p);
    }
    g_6d10a8 = nullptr;
    if (mode == F<int32_t>(g_gameWindow, 0x8c) && F<int32_t>(g_cursorManager, 4) != 6)
        Cursor_Set(g_cursorManager, 0, cursor, 0, 0);
    Ui_PlaySound(0x32, 0, 0x3f000000);
}
RT3_REPLACE(0x4e4ee0, Ledger_Open);

// 0x4e5080. Draws the first glyph of the sprite table +0xb2d at (x, y), unscaled
// and snapped (the other two arguments are unused).
RT3_NOINLINE void __fastcall Sprite_DrawFirstGlyph(uint8_t* o, int, float x, float y, int32_t, int32_t) {
    Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(o, 0xb2d), 0, 0), 0, x, y, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x4e5080, Sprite_DrawFirstGlyph);

// 0x4e50d0. Draws the second glyph of the sprite table +0xb2d at (x + 0.005, y).
RT3_NOINLINE void __fastcall Sprite_DrawSecondGlyph(uint8_t* o, int, float x, float y, int32_t, int32_t) {
    float gx = (float)((double)x + D(0x5d10c0));
    Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(o, 0xb2d), 0, 1), 0, gx, y, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x4e50d0, Sprite_DrawSecondGlyph);

// 0x4e5130. Company window stock page in the box (x, y, w, h). For a valid
// company +0x88 it draws the stock chart in compact mode (links from 0x40d8,
// faces +0xb25, custom faces +0x995 / +0xb29, all series shown). Otherwise it
// shows text 0x513 in font +0xe55 and hides control 0x3efd.
RT3_NOINLINE void __fastcall CompanyWindow_DrawStockPage(uint8_t* w, int, float x, float y, float wd, float h) {
    if (Collection_IsValid(g_companies, 0, F<uint32_t>(w, 0x88))) {
        CompanyStock_DrawChart(w, 0x40d8, x, y, wd, h, 1, F<uint32_t>(w, 0x88), F<uint8_t*>(w, 0xb25), (uint8_t**)(w + 0x995), F<int32_t>(w, 0xb29),
                               (const uint8_t*)0x622098);
        return;
    }
    Font_DrawTextBox(F<uint8_t*>(w, 0xe55), 0, GetString(0x513), x, y, wd, h, 0xff000000, 0, 0, 2, 1.0f);
    Window_SendEvent5(w, 0, 0x86, 0x3efd, 3, 0, 0);
}
RT3_REPLACE(0x4e5130, CompanyWindow_DrawStockPage);

// 0x4e51f0 (draw callback; x, y, w, h). Panel p shows the text of the
// trigger-9 events (0x62be18, up to 1000 bytes) in its font (+0xe55), black,
// and a help link (0x6ec, tip 0x6ed, id 0x3fba) at its bottom right.
RT3_NOINLINE void __fastcall EventPanel_Draw(uint8_t* p, int, float x, float y, float w, float h) {
    char text[0x3e8];
    Events_Fire(g_events, 0, 9, text, 0x3e8);
    Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, text, x, y, w, h, 0xff000000, 0, 1, 2, 1.0f);
    const char* tip = GetString(0x6ed);
    float ly = (float)((double)y + h - *(const double*)rt3::Rdata(0x5d0238));
    float lx = (float)((double)x + w - *(const double*)rt3::Rdata(0x5c9688));
    const char* label = GetString(0x6ec);
    uint32_t lw = 0x3e400000, lh = 0x3d19999a;
    TextLink_Draw(p, F<uint8_t*>(p, 0xe55), label, lx, ly, *(float*)&lw, *(float*)&lh, 3, 4, 3, tip, 0x3fba, 0xff605e18, 0xff908e38);
}
RT3_REPLACE(0x4e51f0, EventPanel_Draw);

// 0x4e52e0. Train lifetime total of the three stats (this year + totals), unrounded.
double __fastcall Train_LifetimeTotal(uint8_t* t) {
    double v = Obj_Sum3(t, 0, 0) + F<float>(t, 0x105);
    v = v + F<float>(t, 0x101);
    return v + F<float>(t, 0xfd);
}
RT3_REPLACE(0x4e52e0, Train_LifetimeTotal);

// 0x4e5300. The player detail page: the face (+0x761 list; custom faces
// 0x29 + name index), the name, an "edit face" link (0x795) for the local
// player, a two-column money table (cash, stock holdings, total, collateral;
// labels 0x279 / 0x4b4 / 0x4b5 / 0x27c), the title +0x27 and, under header
// 0x4b6, every company the player holds shares in (name link with tooltip
// 0x4b1, share count 0x4b7 and value at the share price 0x4b8) or "none"
// (0x4b9).
RT3_NOINLINE void __fastcall PlayerDetail_Draw(uint8_t* w, int, float x, float y, float, float) {
    uint8_t* p = (uint8_t*)Collection_Get(g_players, 0, F<uint32_t>(w, 0x8c));
    int32_t face = p[0x15c];
    if (face == 0x29) {
        if (Obj_NameIndex(p) != -1) face = Obj_NameIndex(p) + 0x29;
        else {
            face = 0;
            p[0x15c] = 0;
        }
    }
    uint8_t* font = F<uint8_t*>(w, 0xe55);
    float gx = (float)((double)x + D(0x5d1108));
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x761 + face * 4), 0, 0), 0, gx, y, 0x3f800000, BitsF(0x3f3ae148), BitsF(0x3f3ae148),
                0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    float nameX = (float)((double)x + D(0x5c8988));
    Font_DrawTextBox(font, 0, (const char*)(p + 8), nameX, y, BitsF(0x3ee147ae), BitsF(0x3d9374bc), 0xff000000, 2, 1, 2, 1.0f);
    float tw;
    Font_TextHeight(font, 0, (const char*)(p + 8), 0.5f, &tw);
    if (p == Obj_Ref25(g_world)) {
        const char* tip = GetString(0xbd5);
        float bw = (float)(Slider_StepA((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x761 + face * 4), 0, 0)) * D(0x5d1100));
        float by = (float)(Slider_StepB((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x761 + face * 4), 0, 0)) * D(0x5d1100) + y);
        TextLink_Draw(w, font, GetString(0x795), gx, by, bw, BitsF(0x3d1374bc), 0, 0, 3, tip, 0x3fb8, 0xff605e18, 0xff908e38);
    }
    float colY = (float)((double)y + D(0x5d10f8));
    float cur = colY;
    char buf[0x100];
    for (int32_t c = 0; c < 2; ++c) {
        cur = colY;
        int32_t align = (c != 0) + 2;
        auto row = [&](int32_t str, double v, uint32_t flags) {
            if (!c) strcpy(buf, GetString(str));
            else Num_FormatSimple2(buf, 0xb, v, 0xb);
            Font_DrawTextBox(font, 0, buf, nameX, cur, BitsF(0x3e89999a), BitsF(0x3d9374bc), 0xff000000, align, 1, flags, 1.0f);
            cur = (float)((double)cur + D(0x5d10f0));
        };
        row(0x279, c ? F<double>(p, 0x154) : 0.0, 2);
        row(0x4b4, c ? Player_Wealth(p, 0, 1) : 0.0, c == 1 ? 3 : 2);
        cur = (float)((double)cur + D(0x5caad0));
        row(0x4b5, c ? Player_Wealth(p, 0, 1) + F<double>(p, 0x154) : 0.0, 2);
        cur = (float)((double)cur + D(0x5d10e8));
        row(0x27c, c ? Player_CollateralWorth(p, 0, 1) : 0.0, 2);
    }
    cur = (float)((double)cur + D(0x5c9b50));
    float x2 = (float)((double)x + D(0x5c9b98));
    Font_DrawTextBox(font, 0, (const char*)(p + 0x27), x2, cur, BitsF(0x3f0ccccd), BitsF(0x3e19999a), 0xff000000, 2, 1, 2, 1.0f);
    cur = (float)((double)cur + D(0x5c8988));
    Font_DrawTextBox(font, 0, GetString(0x4b6), x, cur, BitsF(0x3f170a3d), BitsF(0x3d9374bc), 0xff000000, 0, 1, 3, 1.0f);
    cur = (float)((double)cur + D(0x5d10e0));
    int32_t shown = 0;
    Collection* cos = g_companies;
    char tip[0x100], num[0x40];
    for (int32_t i = 0; i < Table_CountField3f(cos); ++i) {
        uint8_t* co = (uint8_t*)Table_NthField3f(cos, 0, i);
        if (!F<int32_t>(p, 0x15d + F<int32_t>(co, 0) * 4)) continue;
        FormatText(tip, GetString(0x4b1), (const char*)(co + 4), 0, 0, 0, 0, 0, 0, 0);
        TextLink_Draw(w, font, (const char*)(co + 4), x2, cur, BitsF(0x3e47ae14), BitsF(0x3d1374bc), 2, 1, 2, tip, F<int32_t>(co, 0) + 0x4074, 0xff000000,
                      0xff6d6b6e);
        int32_t held = F<int32_t>(p, 0x15d + F<int32_t>(co, 0) * 4);
        const char* n = Num_Format(num, 1, (double)held, 9, 1, 0, 0);
        FormatText(tip, GetString(0x4b7), n, 0, 0, 0, 0, 0, 0, 0);
        float sx = (float)((double)x2 + D(0x5d10d8));
        Font_DrawTextBox(font, 0, tip, sx, cur, BitsF(0x3e4ccccd), BitsF(0x3d9374bc), 0xff000000, 3, 1, 2, 1.0f);
        held = F<int32_t>(p, 0x15d + F<int32_t>(co, 0) * 4);
        double price = Company_Stat(co, 0, 0x1c, 0x2329);
        Num_FormatSimple2(num, 0xb, price * (double)held, 9);
        FormatText(tip, GetString(0x4b8), num, 0, 0, 0, 0, 0, 0, 0);
        float vx = (float)((double)x2 + D(0x5d10d0));
        Font_DrawTextBox(font, 0, tip, vx, cur, BitsF(0x3e428f5c), BitsF(0x3d9374bc), 0xff000000, 3, 1, 2, 1.0f);
        cur = (float)((double)cur + D(0x5d10f0));
        ++shown;
    }
    if (!shown) {
        float ny = (float)((double)cur + D(0x5d10f0));
        float nx = (float)((double)x + D(0x5d10c8));
        Font_DrawTextBox(font, 0, GetString(0x4b9), nx, ny, BitsF(0x3e4ccccd), BitsF(0x3d9374bc), 0xff000000, 0, 1, 2, 1.0f);
    }
}
RT3_REPLACE(0x4e5300, PlayerDetail_Draw);

// 0x4e5a70. The company overview page of the company window (company +0x88;
// without one a notice 0xf30 / 0xbe3 / 0x4ba and the trade buttons off): the
// logo (a "your company" link 0x795 for the player's own), five stats (labels
// 0x4bb..0x4bf: two company stats, revenue per mile, their ratio and one with
// 0x4c0), who controls it (0xbe5..0xbe9), the player's / owner's stake
// (0xbea..0xbef), this year's change of control (0xbf0 / 0xbf1), the
// difficulty (0x4c2, at the top) and two columns of action links
// (0x3fad..0x3fb5, the fourth only without world +0x4a7f).
RT3_NOINLINE void __fastcall CompanyOverview_Draw(uint8_t* p, int, float x, float y, float w, float h) {
    uint8_t* font = F<uint8_t*>(p, 0xe55);
    uint8_t* co = F<uint32_t>(p, 0x88) ? (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(p, 0x88)) : nullptr;
    char buf[0x400], line[0x400], num[0x40], num2[0x40], tip[0x400];
    auto world = [] { return g_world; };
    if (!co) {
        Obj_Ref25(world());
        const char* s;
        if (World_AnyCondition()) s = GetString(0xf30);
        else s = F<int32_t>(Obj_Ref25(world()), 0x2a7) ? GetString(0xbe3) : GetString(0x4ba);
        strcpy(buf, s);
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, x, y, w, h, 0xff000000, 0, 0, 2, 1.0f);
        Window_SendEvent5(p, 0, 0x87, 0x3f06, 3, 0, 0);
        Window_SendEvent5(p, 0, 0x87, 0x3f07, 3, 0, 0);
        return;
    }
    (void)font;
    Window_SendEvent5(p, 0, 0x65, 0x3f06, 3, 0, 0);
    Window_SendEvent5(p, 0, 0x65, 0x3f07, 3, 0, 0);
    float gx = (float)((double)x + D(0x5c9b98));
    static const int32_t labels[5] = { 0x4bb, 0x4bc, 0x4bd, 0x4be, 0x4bf };
    Glyph_Queue((uint8_t*)(uintptr_t)Company_Logo(co), 0, gx, y, 0x3f800000, BitsF(0x3fa66666), BitsF(0x3fa66666), 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    if (co == Obj_Company21(world())) {
        const char* t = GetString(0xbe4);
        float lw = (float)(Slider_StepA((uint8_t*)(uintptr_t)Company_Logo(co)) * D(0x5c8660));
        float ly = (float)(Slider_StepB((uint8_t*)(uintptr_t)Company_Logo(co)) * D(0x5c8660) + y);
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), GetString(0x795), gx, ly, lw, BitsF(0x3d1374bc), 0, 0, 3, t, 0x3fb9, 0xff605e18, 0xff908e38);
    }
    float colX = (float)((double)x + D(0x5c8758));
    float colW = (float)((double)w - ((double)colX - x));
    float rowY = y;
    for (int32_t pass = 0; pass < 2; ++pass) {
        rowY = y;
        for (int32_t i = 0; i < 5; ++i) {
            if (!pass) {
                FormatText(line, (const char*)0x5d1128, GetString(labels[i]), 0, 0, 0, 0, 0, 0, 0);
                Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, line, colX, rowY, colW, BitsF(0x3cd4fdf4), 0xff000000, 2, 1, 2, 1.0f);
            } else {
                switch (i) {
                case 0: Num_Format(line, 0xb, Company_Stat(co, 0, 0x2c, Obj_YearOrNow(p)), 10, 1, 0, 0); break;
                case 1: Num_Format(line, 0xb, Company_Stat(co, 0, 0x2b, Obj_YearOrNow(p)), 10, 1, 0, 0); break;
                case 2: Num_Format(line, 1, Company_Stat(co, 0, 0x17, Obj_YearOrNow(p)), 10, 1, 0, 0); break;
                case 3: {
                    double miles = Company_Stat(co, 0, 0x17, Obj_YearOrNow(p)) < D(0x5c8770) ? 1.0 : Company_Stat(co, 0, 0x17, Obj_YearOrNow(p));
                    float v = (float)(Company_Stat(co, 0, 0x2c, Obj_YearOrNow(p)) / miles);
                    Num_Format(line, 9, (double)v, 10, 1, 0, 0);
                    break;
                }
                default:
                    Num_Format(num, 1, Company_Stat(co, 0, 0x26, Obj_YearOrNow(p)), 10, 1, 0, 0);
                    FormatText(line, GetString(0x4c0), num, 0, 0, 0, 0, 0, 0, 0);
                    break;
                }
                float vw = (float)((double)colW - D(0x5c8918));
                Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, line, colX, rowY, vw, BitsF(0x3cd4fdf4), 0xff000000, 3, 1, 2, 1.0f);
            }
            rowY = (float)((double)rowY + D(0x5d10f0));
        }
    }
    rowY = (float)((double)rowY + D(0x5c8918));
    auto para = [&]() {
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, x, rowY, w, 1.0f, 0xff000000, 0, 1, 2, 1.0f);
        double th = Font_TextHeight(F<uint8_t*>(p, 0xe55), 0, buf, w, nullptr);
        rowY = (float)(th + rowY + D(0x5d01d8));
    };
    if (!F<int32_t>(co, 0x3b)) {
        strcpy(buf, GetString(0xbe5));
        para();
    } else {
        uint32_t first = F<uint32_t>(co, 0);
        int32_t all = Player_OwnsAllOf((uint8_t*)Company_Owner(co), 0, first);
        int32_t owner = F<int32_t>(co, 0x3b);
        if (all) {
            if (owner == Obj_Get25(world())) FormatText(buf, GetString(0xbe6), 0, 0, 0, 0, 0, 0, 0, 0);
            else {
                const char* n = (const char*)Company_Owner(co) + 8;
                FormatText(buf, GetString(0xbe7), n, 0, 0, 0, 0, 0, 0, 0);
            }
        } else {
            bool mine = owner == Obj_Get25(world());
            const char* lvl = GetString(hook::Global<int32_t>(0x622170 + Obj_Level34b(co) * 4));
            FormatText(buf, GetString(mine ? 0xbe8 : 0xbe9), lvl, 0, 0, 0, 0, 0, 0, 0);
        }
        para();
        buf[0] = 0;
        int32_t shares = F<int32_t>(co, 0x14f);
        int32_t diff = shares - F<int32_t>(co, 0xd59);
        Num_FormatSimple(num, 9, (double)shares, 0xb);
        int32_t ad = diff < 0 ? -diff : diff;
        Num_FormatSimple(num2, 9, (double)ad, 0xb);
        bool mine = F<int32_t>(co, 0x3b) == Obj_Get25(world());
        int32_t sid;
        if (diff < 0) sid = mine ? 0xbea : 0xbed;
        else if (diff > 0) sid = mine ? 0xbeb : 0xbee;
        else sid = mine ? 0xbec : 0xbef;
        if (diff) FormatText(line, GetString(sid), num2, num, 0, 0, 0, 0, 0, 0);
        else FormatText(line, GetString(sid), num, 0, 0, 0, 0, 0, 0, 0);
        strcat(buf, line);
        para();
        if (F<int32_t>(co, 0x34f) == Obj_YearOrNow(p)) {
            buf[0] = 0;
            Num_FormatSimple(num, 9, (double)F<int32_t>(co, 0x353), 0xb);
            bool me = F<int32_t>(co, 0x3b) == Obj_Get25(world());
            FormatText(line, GetString(me ? 0xbf0 : 0xbf1), num, 0, 0, 0, 0, 0, 0, 0);
            strcat(buf, line);
            para();
        }
    }
    const char* diffName = GetString(hook::Global<int32_t>(0x620cc0 + F<int32_t>(world(), 0x2d) * 4));
    FormatText(line, GetString(0x4c2), diffName, 0, 0, 0, 0, 0, 0, 0);
    Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, line, x, y, w, h, 0xff000000, 0, 4, 2, 1.0f);
    double a = (double)rowY - D(0x5d01d8);
    double b = (double)y - D(0x5d1120) + D(0x5d1118);
    float top = (float)(a > b ? a : b);
    rowY = top;
    float lx = (float)((double)x + D(0x5d01b0));
    const char* name = (const char*)co + 4;
    auto link = [&](float lxv, float base, int32_t tipId, int32_t textId, int32_t id, bool plainTip) {
        if (!plainTip) FormatText(tip, GetString(tipId), name, 0, 0, 0, 0, 0, 0, 0);
        const char* t = plainTip ? GetString(tipId) : tip;
        float ty = (float)((double)base + D(0x5d01e0));
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), GetString(textId), lxv, ty, BitsF(0x3e400000), BitsF(0x3ccccccd), 2, 0, 3, t, id, 0xff605e18, 0xff908e38);
    };
    link(lx, rowY, 0x4c3, 0x4c4, 0x3fad, false);
    rowY = (float)((double)rowY + D(0x5d1110));
    link(lx, rowY, 0x4c5, 0x4c6, 0x3fae, false);
    rowY = (float)((double)rowY + D(0x5d1110));
    link(lx, rowY, 0x4c7, 0x4c8, 0x3faf, false);
    rowY = (float)((double)rowY + D(0x5d1110));
    if (!F<int32_t>(world(), 0x4a7f)) link(lx, rowY, 0x4c9, 0x4ca, 0x3fb0, false);
    float lx2 = (float)((double)x + D(0x5d1118));
    link(lx2, top, 0x4cb, 0x4cc, 0x3fb1, false);
    rowY = (float)((double)top + D(0x5d1110));
    link(lx2, rowY, 0x4cd, 0x4ce, 0x3fb2, false);
    rowY = (float)((double)rowY + D(0x5d1110));
    link(lx2, rowY, 0x4cf, 0x4d0, 0x3fb5, false);
    rowY = (float)((double)rowY + D(0x5d1110));
    link(lx2, rowY, 0x4d2, 0x4d1, 0x3fb3, true);
}
RT3_REPLACE(0x4e5a70, CompanyOverview_Draw);

// 0x4e68e0. The company ranking list: with no active companies a message (the
// scenario's 0xf30 / 0x4ba / 0xbe3). Else four sortable column headers
// (0x4d3..0x4d6: book value 0x2c, revenue 0x2b, cash 0xd, 0x25 of the shown
// year), then the companies ordered by the chosen column (descending, or
// ascending with +0x760), a page of 9 from row +0x7c * 9: the player's own
// company highlighted, its logo, its name link (0x4b2) and the four figures
// as links (tooltips 0x4db..0x4dd), separated by lines. The shown company ids
// go to +0x11c (200 slots).
RT3_NOINLINE void __fastcall CompanyRanking_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey) {
    char buf[0x200], tip[0x400];
    if (!Table_CountField3f(g_companies)) {
        Obj_Ref25(g_world);
        int32_t str;
        if (World_AnyCondition()) str = 0xf30;
        else str = F<int32_t>(Obj_Ref25(g_world), 0x2a7) ? 0xbe3 : 0x4ba;
        strcpy(tip, GetString(str));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, tip, x, y, w, h, 0xff000000, 0, 0, 2, 1.0f);
        return;
    }
    uint8_t used[200];
    memset(used, 0, sizeof used);
    memset(p + 0x11c, 0, 0x640);
    int32_t year = Obj_YearOrNow(p);
    struct Column { int32_t title; float width; int32_t stat, style, tip, cellTip; };
    const Column cols[4] = {
        { 0x4d3, BitsF(0x3dae147b), 0x2c, 0xb, 0x4d7, 0x4db },
        { 0x4d4, BitsF(0x3dae147b), 0x2b, 0xb, 0x4d8, 0x4db },
        { 0x4d5, BitsF(0x3de147ae), 0xd, 0xb, 0x4d9, 0x4dc },
        { 0x4d6, BitsF(0x3dae147b), 0x25, 1, 0x4da, 0x4dd },
    };
    float colX = (float)((double)x + D(0x5c9c50));
    float cx = colX;
    for (int32_t i = 0; i < 4; ++i) {
        const char* t = GetString(cols[i].tip);
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), GetString(cols[i].title), cx, y, cols[i].width, BitsF(0x3d9374bc), 3, 4, 3, t, i + 0x3fd4, 0xff000000,
                      0xff6d6b6e);
        cx = (float)((double)cx + cols[i].width);
    }
    float rowY = (float)((double)y + D(0x5d1140));
    int32_t rows = 0, seen = 0;
    int32_t* slot = (int32_t*)(p + 0x11c);
    for (int32_t r = 0; r < 200; ++r, ++slot) {
        int32_t best = -1;
        float hi = BitsF(0xc97423f0), lo = BitsF(0x4e6e6b28);
        for (int32_t j = 0; j < Table_CountField3f(g_companies); ++j) {
            if (used[j]) continue;
            int32_t stat = sortKey == 1 ? 0x2b : sortKey == 2 ? 0xd : sortKey == 3 ? 0x25 : 0x2c;
            float v = (float)Company_Stat((uint8_t*)Table_NthField3f(g_companies, 0, j), 0, stat, year);
            if (!p[0x760]) {
                if (v > hi) {
                    hi = v;
                    best = j;
                }
            } else if (v < lo) {
                lo = v;
                best = j;
            }
        }
        if (best == -1) return;
        used[best] = 1;
        if (++seen <= F<int32_t>(p, 0x7c) * 9) continue;
        uint8_t* c = (uint8_t*)Table_NthField3f(g_companies, 0, best);
        *slot = F<int32_t>(c, 0);
        if (F<int32_t>(c, 0x3b) == Obj_Get25(g_world)) Ui_FillRect(0x18000000, 0, x, rowY, w, BitsF(0x3d2c0831), 3);
        float gy = (float)((double)rowY + D(0x5c9bc0));
        float gx = (float)((double)x + D(0x5c9bc0));
        Glyph_Queue((uint8_t*)(uintptr_t)Company_Logo(c), 0, gx, gy, 0x3f800000, BitsF(0x3ecccccd), BitsF(0x3ecccccd), 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f,
                    1.0f);
        FormatText(buf, GetString(0x4b2), (const char*)(c + 4), 0, 0, 0, 0, 0, 0, 0);
        float ny = (float)((double)rowY + D(0x5d1138));
        float nx = (float)((double)x + D(0x5d1130));
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), (const char*)(c + 4), nx, ny, BitsF(0x3e2f1aa0), BitsF(0x3d2c0831), 2, 1, 2, buf, F<int32_t>(c, 0) + 0x4074,
                      0xff000000, 0xff6d6b6e);
        cx = colX;
        for (int32_t i = 0, off = 0x413c; off < 0x4204; ++i, off += 0x32) {
            double v = Company_Stat(c, 0, cols[i].stat, Obj_YearOrNow(p));
            Num_Format(buf, cols[i].style, v, 10, 1, 0, 0);
            FormatText(tip, GetString(cols[i].cellTip), (const char*)(c + 4), 0, 0, 0, 0, 0, 0, 0);
            TextLink_Draw(p, F<uint8_t*>(p, 0xe55), buf, cx, rowY, cols[i].width, BitsF(0x3d2c0831), 3, 0, 2, tip, F<int32_t>(c, 0) + off, 0xff000000,
                          0xff6d6b6e);
            cx = (float)((double)cx + cols[i].width);
        }
        if (rows > 0) Ui_Line(0x2f000000, 0, x, rowY, (float)((double)x + w), rowY, 2.0f);
        if (++rows >= 9) return;
        rowY = (float)((double)rowY + D(0x5ce8c0));
    }
}
RT3_REPLACE(0x4e68e0, CompanyRanking_Draw);

// 0x4e6ef0. The player ranking list: four sortable headers (0x4d5 / 0x4de /
// 0x44d / 0x4df: cash +0x154, holdings, net worth, collateral; unsorted by
// table order), then the players ordered by the chosen figure (descending, or
// ascending with +0x760), a page of 9 from row +0x7c * 9: the local player
// highlighted, the face (+0x15c, or the custom face list +0x995 for face 0x29;
// a missing custom face falls back to face 0), the name (selected row +0x98 in
// grey) and the four figures, separated by lines; each row gets tooltip 0x4b1
// and the player id as events 0x6e / 0x7c on id 0x3f48 + row.
RT3_NOINLINE void __fastcall PlayerRanking_Draw(uint8_t* p, int, float x, float y, float w, float, int32_t sortKey) {
    float colX = (float)((double)x + D(0x5c9bd0));
    uint8_t used[200];
    memset(used, 0, sizeof used);
    memset(p + 0x11c, 0, 0x640);
    const int32_t titles[4] = { 0x4d5, 0x4de, 0x44d, 0x4df }, tips[4] = { 0x4d9, 0x4e0, 0x4e1, 0x4e2 };
    float cx = colX;
    for (int32_t i = 0; i < 4; ++i) {
        double off = i == 3 ? BitsD(0x3f7cac083126e979ull) : 0.0;
        const char* tip = GetString(tips[i]);
        float hx = (float)((double)cx + off);
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), GetString(titles[i]), hx, y, BitsF(0x3dcccccd), BitsF(0x3d9374bc), 3, 4, 3, tip, i + 0x3fde, 0xff000000,
                      0xff6d6b6e);
        cx = (float)((double)cx + (i == 3 ? BitsD(0x3fb999999999999aull) : BitsD(0x3fb5c28f5c28f5c3ull)));
    }
    Collection* players = g_players;
    auto figure = [&](uint8_t* pl, int32_t k) -> double {
        switch (k) {
        case 0: return F<double>(pl, 0x154);
        case 1: return Player_HoldingsValue(pl, 0, 1);
        case 2: return Player_NetWorth(pl, 0, 1);
        default: return Player_CollateralWorth(pl, 0, 1);
        }
    };
    float rowY = (float)((double)y + D(0x5d1140));
    int32_t rows = 0, seen = 0;
    int32_t* slot = (int32_t*)(p + 0x11c);
    char buf[0x200];
    for (int32_t r = 0; r < 200; ++r, ++slot) {
        int32_t best = -1;
        float hi = BitsF(0xc97423f0), lo = BitsF(0x4e6e6b28);
        for (int32_t j = 0; j < Table_CountField4(players); ++j) {
            if (used[j]) continue;
            float v;
            if (sortKey >= 1 && sortKey <= 4)
                v = (float)figure((uint8_t*)Table_NthField4(players, 0, j), sortKey - 1);
            else {
                uint32_t id = Table_NthActive(players, 0, j);
                double d = (double)(int32_t)id;
                if ((int32_t)id < 0) d += Fl(0x5c872c);
                v = (float)d;
            }
            if (!p[0x760]) {
                if (v > hi) {
                    hi = v;
                    best = j;
                }
            } else if (v < lo) {
                lo = v;
                best = j;
            }
        }
        if (best == -1) return;
        used[best] = 1;
        if (++seen <= F<int32_t>(p, 0x7c) * 9) continue;
        uint8_t* pl = (uint8_t*)Table_NthField4(players, 0, best);
        if (F<int32_t>(pl, 0) == Obj_Get25(g_world)) {
            float rw = (float)((double)w - D(0x5d0810));
            float rx = (float)((double)x + D(0x5d0810));
            Ui_FillRect(0x18000000, 0, rx, rowY, rw, BitsF(0x3d2c0831), 3);
        }
        *slot = F<int32_t>(pl, 0);
        double faceOff = (rows & 1) ? BitsD(0xbf916872b020c49cull) : BitsD(0x3f689374bc6a7efaull);
        auto glyph = [&](uint8_t* table, int32_t idx, bool custom) {
            float gy = (float)((double)rowY - D(0x5caad0));
            float gx = (float)((double)x + faceOff + D(0x5d1150));
            if (custom) table = F<uint8_t*>(p, 0x995 + Obj_NameIndex(pl) * 4);
            Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(table, 0, idx), 0, gx, gy, 0x3f800000, BitsF(0x3f1eb852), BitsF(0x3f1eb852), 0xffffffff, 0, 1, 0,
                        0.0f, 0.0f, 1.0f, 1.0f);
        };
        uint8_t face = pl[0x15c];
        if (face != 0x29) glyph(F<uint8_t*>(p, 0xb25), face, false);
        else if (Obj_NameIndex(pl) != -1) glyph(nullptr, 0, true);
        else {
            pl[0x15c] = 0;
            glyph(F<uint8_t*>(p, 0xb25), 0, false);
        }
        int32_t id = rows + 0x3f48;
        uint32_t color = F<int32_t>(p, 0x98) == id ? 0xff6d6b6e : 0xff000000;
        float nx = (float)((double)x + D(0x5d1148));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, (const char*)(pl + 8), nx, rowY, BitsF(0x3e5c28f6), BitsF(0x3d2c0831), color, 2, 0, 2, 1.0f);
        cx = colX;
        for (int32_t k = 0; k < 4; ++k) {
            float v = (float)figure(pl, k);
            Num_Format(buf, 0xb, (double)v, 9, 1, 0, 0);
            Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, cx, rowY, BitsF(0x3dcccccd), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
            cx = (float)((double)cx + (k == 3 ? BitsD(0x3fb999999999999aull) : BitsD(0x3fb5c28f5c28f5c3ull)));
        }
        if (rows > 0) {
            float ex = (float)((double)x + w);
            float sx = (float)((double)x + D(0x5d0810));
            Ui_Line(0x2f000000, 0, sx, rowY, ex, rowY, 2.0f);
        }
        FormatText(buf, GetString(0x4b1), (const char*)(pl + 8), 0, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(p, 0, 0x6e, (uint32_t)id, (uint32_t)(uintptr_t)buf, 0, 0);
        Window_SendEvent5(p, 0, 0x7c, (uint32_t)id, F<uint32_t>(pl, 0), 0, 0);
        if (++rows >= 9) return;
        rowY = (float)((double)rowY + D(0x5ce8c0));
    }
}
RT3_REPLACE(0x4e6ef0, PlayerRanking_Draw);

// 0x4e7670. The company's train list (company +0x88; 0x4ba when it is gone,
// 0x4e3 when it has no trains): nine sortable headers (0x4e4, 0xb36, 0x4e6..
// 0x4e9, the current and last year's income 0x4ea with the year, 0xb34;
// widths depend on the language), then the trains ordered by the chosen
// column (largest first, or smallest with +0x760; default the engine's +0x126),
// a page of 9 from row +0x7c * 9: the engine picture, age (0 when world
// +0x4aeb), three percentages (+0xdc, +0xe0, +0xe4), +0x18e, the income of this
// and last year and the lifetime total, a tooltip (0x4f3 with the train's
// name) and the train id per row. The shown train ids go to +0x11c.
RT3_NOINLINE void __fastcall CompanyTrains_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey) {
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(p, 0x88))) {
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, GetString(0x4ba), x, y, w, h, 0xff000000, 0, 0, 2, 1.0f);
        return;
    }
    int32_t* co = (int32_t*)Collection_Get(g_companies, 0, F<uint32_t>(p, 0x88));
    if (!Company_TrainCount(co)) {
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, GetString(0x4e3), x, y, w, h, 0xff000000, 0, 0, 2, 1.0f);
        return;
    }
    uint8_t used[200];
    memset(used, 0, sizeof used);
    memset(p + 0x11c, 0, 0x640);
    int32_t year = Obj_YearOrNow(p);
    int32_t yearsBack = F<uint16_t>(g_world, 0xd) - year;
    struct Col { int32_t title; uint32_t width; int32_t tip; } cols[9] = {
        { 0x4e4, 0x3dd70a3d, 0x4eb }, { 0xb36, 0x3d23d70a, 0xb37 }, { 0x4e6, 0x3d54fdf4, 0x4ed }, { 0x4e7, 0x3d6147ae, 0x4ee }, { 0x4e8, 0x3d54fdf4, 0x4ef },
        { 0x4e9, 0x3d50e560, 0x4f0 }, { 0x4ea, 0x3d9374bc, 0x4f1 }, { 0x4ea, 0x3d9374bc, 0x4f1 }, { 0xb34, 0x3da7ef9e, 0xb35 } };
    int32_t lang = g_language;
    if (lang == 1) {
        cols[2].width = 0x3d483127;
        cols[3].width = 0x3d75c28f;
        cols[5].width = 0x3d6f9db2;
    } else if (lang == 4)
        cols[6].width = cols[7].width = 0x3d99db22;
    else if (lang == 3) {
        cols[0].width = 0x3dd0a3d7;
        cols[5].width = 0x3d2a7efa;
        cols[6].width = cols[7].width = 0x3d960418;
        cols[8].width = 0x3dcbc6a8;
    } else if (lang == 2) {
        cols[5].width = 0x3d6a7efa;
        cols[8].width = 0x3db4bc6b;
    }
    char buf[0x110], num[0x10];
    float cx = x;
    for (int32_t i = 0; i < 9; ++i) {
        double pad = 0.0;
        if (i <= 5 || i == 8) {
            FormatText(buf, GetString(cols[i].title), 0, 0, 0, 0, 0, 0, 0, 0);
            if (i == 1) pad = BitsD(0x3f947ae147ae147bull);
        } else if (i == 6) {
            Num_Format(num, 8, (double)year, 10, 1, 0, 0);
            FormatText(buf, GetString(0x4ea), num, 0, 0, 0, 0, 0, 0, 0);
        } else {
            Num_Format(num, 8, (double)(year - 1), 10, 1, 0, 0);
            FormatText(buf, GetString(cols[i].title), num, 0, 0, 0, 0, 0, 0, 0);
        }
        float padf = (float)pad;
        const char* tip = GetString(cols[i].tip);
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), buf, (float)((double)cx - padf), y, (float)((double)padf + BitsF(cols[i].width)), BitsF(0x3d9374bc),
                      i ? 3 : 0, 4, 3, tip, 0x3fe8 + i, 0xff000000, 0xff6d6b6e);
        cx = (float)((double)cx + BitsF(cols[i].width));
    }
    float rowY = (float)((double)y + D(0x5d1140));
    int32_t seen = 0, rowId = 0x3f48;
    int32_t* slot = (int32_t*)(p + 0x11c);
    Collection* trains = g_trains;
    auto train = [&](int32_t n) { return (uint8_t*)Collection_Get(g_trains, 0, Company_NthTrain(co, 0, n)); };
    (void)trains;
    for (int32_t r = 0; r < 200; ++r, ++slot) {
        int32_t best = -1;
        float hi = BitsF(0xc97423f0), lo = BitsF(0x4e6e6b28);
        if (Company_TrainCount(co) <= 0) return;
        int32_t n = 1;
        do {
            if (!used[n - 1]) {
                float v;
                switch (sortKey) {
                case 1: v = (float)F<int32_t>(train(n), 0xd4); break;
                case 2: v = F<float>(train(n), 0xdc); break;
                case 3: v = F<float>(train(n), 0xe4); break;
                case 4: v = F<float>(train(n), 0xe0); break;
                case 5: v = F<float>(train(n), 0x18e); break;
                case 6: v = (float)Obj_Sum3(train(n), 0, yearsBack); break;
                case 7: v = (float)Obj_Sum3(train(n), 0, yearsBack + 1); break;
                case 8: {
                    uint8_t* t = train(n);
                    v = (float)(Obj_Sum3(t, 0, 0) + F<float>(t, 0x105) + F<float>(t, 0x101) + F<float>(t, 0xfd));
                    break;
                }
                default: v = (float)F<int32_t>(Obj_Ref388(Obj_City41(train(n))), 0x126); break;
                }
                if (!p[0x760]) {
                    if (v > hi) {
                        hi = v;
                        best = n - 1;
                    }
                } else if (v < lo) {
                    lo = v;
                    best = n - 1;
                }
            }
            ++n;
        } while (n - 1 < Company_TrainCount(co));
        if (best == -1) return;
        used[best] = 1;
        if (++seen <= F<int32_t>(p, 0x7c) * 9) continue;
        int32_t nth = best + 1;
        uint8_t* t = train(nth);
        *slot = (int32_t)Company_NthTrain(co, 0, nth);
        uint32_t icon = Car_SkinTexture((uint8_t*)Obj_City41(t));
        float gy = (float)((double)rowY + D(0x5c9bc0));
        float gx = (float)((double)x + D(0x5c9bc0));
        Glyph_Queue((uint8_t*)(uintptr_t)icon, 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
        uint32_t color = F<int32_t>(p, 0x98) == rowId ? 0xff6d6b6e : 0xff000000;
        int32_t age = year - F<int32_t>(t, 0xd4);
        float colX = (float)((double)BitsF(cols[0].width) + x);
        if (F<int32_t>(g_world, 0x4aeb)) age = 0;
        auto cell = [&](float cxv, uint32_t wd) {
            Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, cxv, rowY, BitsF(wd), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        };
        auto percent = [&](uint32_t off) {
            crt::sprintf(buf, (const char*)0x5d0b18, Ftol((double)F<float>(t, off) * D(0x5c85d8)));
        };
        Num_Format(buf, 8, (double)age, 10, 1, 0, 0);
        cell(colX, 0x3d23d70a);
        percent(0xdc);
        colX = (float)((double)colX + Fl(0x5cb660));
        cell(colX, cols[2].width);
        percent(0xe0);
        colX = (float)((double)colX + BitsF(cols[2].width));
        cell(colX, cols[3].width);
        percent(0xe4);
        colX = (float)((double)colX + BitsF(cols[3].width));
        cell(colX, 0x3d54fdf4);
        Num_Format(buf, 1, (double)Ftol(F<float>(t, 0x18e)), 10, 1, 0, 0);
        colX = (float)((double)colX + Fl(0x5d1158));
        cell(colX, cols[5].width);
        Num_Format(buf, 0xb, Obj_Sum3(t, 0, yearsBack), 10, 1, 0, 0);
        colX = (float)((double)colX + BitsF(cols[5].width));
        cell(colX, cols[6].width);
        Num_Format(buf, 0xb, Obj_Sum3(t, 0, yearsBack + 1), 10, 1, 0, 0);
        colX = (float)((double)colX + BitsF(cols[6].width));
        cell(colX, cols[7].width);
        Num_Format(buf, 0xb, Obj_Sum3(t, 0, 0) + F<float>(t, 0x105) + F<float>(t, 0x101) + F<float>(t, 0xfd), 10, 1, 0, 0);
        colX = (float)((double)colX + BitsF(cols[7].width));
        cell(colX, cols[8].width);
        if (rowId - 0x3f48 > 0) Ui_Line(0x2f000000, 0, x, rowY, (float)((double)x + w), rowY, 2.0f);
        const char* name = Train_DisplayName(t, 0, 0);
        FormatText(buf, GetString(0x4f3), name, 0, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(p, 0, 0x6e, rowId, (uint32_t)(uintptr_t)buf, 0, 0);
        Window_SendEvent5(p, 0, 0x7c, rowId, Company_NthTrain(co, 0, nth), 0, 0);
        if (++rowId - 0x3f48 >= 9) return;
        rowY = (float)((double)rowY + D(0x5ce8c0));
    }
}
RT3_REPLACE(0x4e7670, CompanyTrains_Draw);

// 0x4e8270. The company's industry list (company +0x88; 0x4ba when it is gone,
// 0x4f4 when it owns none; at most 400): six sortable headers (0x4e4, 0x4f5,
// 0xae9, 0x4d4, 0x4f7, 0x4f8; widths depend on the language 0x6d3c84), then
// the industries (building cache on) ordered by the chosen column (largest
// first, or smallest with +0x760), a page of 9 from row +0x7c * 9: type name,
// +0x3d5, +0x27a, Obj_Remaining and two figures ("-" / 0xe4 for idle ones), a
// tooltip (0x4fe) and +0x2a4 per row (ids 0x3f48 + row, the selected row
// +0x98 grey). The shown building ids go to +0x11c.
RT3_NOINLINE void __fastcall CompanyIndustries_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey) {
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(p, 0x88))) {
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, GetString(0x4ba), x, y, w, h, 0xff000000, 0, 0, 2, 1.0f);
        return;
    }
    int32_t* co = (int32_t*)Collection_Get(g_companies, 0, F<uint32_t>(p, 0x88));
    int32_t count;
    if (Company_CountIdleBuildings(co) >= 400) count = 400;
    else {
        count = Company_CountIdleBuildings(co);
        if (!count) {
            Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, GetString(0x4f4), x, y, w, h, 0xff000000, 0, 0, 2, 1.0f);
            return;
        }
    }
    uint8_t used[400];
    memset(used, 0, sizeof used);
    memset(p + 0x11c, 0, 0x640);
    Obj_YearOrNow(p);
    uint32_t widths[6] = { 0x3e3851ec, 0x3d4ccccd, 0x3db851ec, 0x3d75c28f, 0x3dcccccd, 0x3dcccccd };
    int32_t lang = g_language;
    if (lang == 1) {
        widths[0] = 0x3e3147af;
        widths[1] = 0x3d828f5c;
        widths[2] = 0x3ddeb852;
        widths[3] = 0x3d851eb8;
        widths[5] = 0x3dab851f;
    } else if (lang == 4) {
        widths[1] = 0x3d933333;
        widths[3] = 0x3d9ae148;
        widths[4] = 0x3dc51eb9;
        widths[5] = 0x3db33333;
        widths[2] = 0x3dab851f;
    } else if (lang == 3) {
        widths[1] = 0x3dc66666;
        widths[3] = 0x3d851eb8;
        widths[4] = 0x3db9999a;
        widths[5] = 0x3d95c290;
    } else if (lang == 2) {
        widths[1] = 0x3d666667;
        widths[4] = widths[5] = 0x3dd9999a;
        widths[2] = 0x3dab851f;
    }
    static const int32_t titles[6] = { 0x4e4, 0x4f5, 0xae9, 0x4d4, 0x4f7, 0x4f8 }, tips[6] = { 0x4f9, 0x4fa, 0xaea, 0x4fb, 0x4fc, 0x4fd };
    float cx = x;
    for (int32_t i = 0; i < 6; ++i) {
        const char* tip = GetString(tips[i]);
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), GetString(titles[i]), cx, y, BitsF(widths[i]), BitsF(0x3d9374bc), 3, 4, 3, tip, i + 0x3ff2, 0xff000000,
                      0xff6d6b6e);
        cx = (float)((double)cx + BitsF(widths[i]));
    }
    float rowY = (float)((double)y + D(0x5d1140));
    Company_CacheBuildings(co);
    SetGlobal62bae8(1);
    Collection* inds = g_buildings;
    auto ind = [&](int32_t n) { return (uint8_t*)Collection_Get(inds, 0, Company_NthBuilding(co, 0, n)); };
    auto typeName = [](uint8_t* b) {
        uint8_t* type = ((uint8_t*(__fastcall*)(uint8_t*))(*(void***)b)[0x20])(b);
        return LocalizeName((const char*)type + 0x4f);
    };
    int32_t seen = 0, rowId = 0x3f48;
    int32_t* slot = (int32_t*)(p + 0x11c);
    char buf[0x200];
    for (int32_t r = 0; r < 400; ++r, ++slot) {
        int32_t best = -1;
        float hi = BitsF(0xc97423f0), lo = BitsF(0x4e6e6b28);
        if (count <= 0) break;
        for (int32_t n = 1; n - 1 < count; ++n) {
            if (used[n - 1]) continue;
            float v;
            switch (sortKey) {
            case 1: v = (float)F<int32_t>(ind(n), 0x3d5); break;
            case 2: v = F<float>(ind(n), 0x27a); break;
            case 3: v = (float)Obj_Remaining(ind(n), 0, 1); break;
            case 4: v = (float)Obj_Float2c0(ind(n), 0, 1); break;
            case 5: v = (float)Obj_Float2e8(ind(n), 0, 1); break;
            default: v = (float)F<uint32_t>(ind(n), 0x3cc); break;
            }
            if (!p[0x760]) {
                if (v > hi) {
                    hi = v;
                    best = n - 1;
                }
            } else if (v < lo) {
                lo = v;
                best = n - 1;
            }
        }
        if (best == -1) break;
        used[best] = 1;
        if (++seen <= F<int32_t>(p, 0x7c) * 9) continue;
        uint32_t color = F<int32_t>(p, 0x98) == rowId ? 0xff6d6b6e : 0xff000000;
        uint8_t* b = ind(best + 1);
        *slot = (int32_t)Company_NthBuilding(co, 0, best + 1);
        FormatText(buf, typeName(b), 0, 0, 0, 0, 0, 0, 0, 0);
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, x, rowY, BitsF(widths[0]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        float colX = (float)((double)BitsF(widths[0]) + x);
        Num_Format(buf, 8, (double)F<int32_t>(b, 0x3d5), 10, 1, 0, 0);
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[1]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        Num_FormatSimple2(buf, 0xb, (double)F<float>(b, 0x27a), 0xb);
        colX = (float)((double)colX + BitsF(widths[1]));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[2]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        Num_FormatSimple2(buf, 0xb, Obj_Remaining(b, 0, 1), 0xb);
        colX = (float)((double)colX + BitsF(widths[2]));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[3]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        auto figure = [&](double(__fastcall * get)(void*, int, int32_t)) {
            if (Obj_StateKindIs4(b))
                FormatText(buf, g_language == 1 ? (const char*)0x5ca2b4 : GetString(0xe4), 0, 0, 0, 0, 0, 0, 0, 0);
            else
                Num_Format(buf, 1, (double)Ftol(get(b, 0, 1)), 10, 1, 0, 0);
        };
        figure(&Obj_Float2c0);
        colX = (float)((double)colX + BitsF(widths[3]));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[4]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        figure(&Obj_Float2e8);
        colX = (float)((double)colX + BitsF(widths[4]));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[5]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        if (rowId - 0x3f48 > 0) Ui_Line(0x2f000000, 0, x, rowY, (float)((double)x + w), rowY, 2.0f);
        const char* name = typeName(b);
        FormatText(buf, GetString(0x4fe), name, 0, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(p, 0, 0x6e, rowId, (uint32_t)(uintptr_t)buf, 0, 0);
        Window_SendEvent5(p, 0, 0x7c, rowId, F<uint32_t>(b, 0x2a4), 0, 0);
        if (++rowId - 0x3f48 >= 9) break;
        rowY = (float)((double)rowY + D(0x5ce8c0));
    }
    SetGlobal62bae8(0);
}
RT3_REPLACE(0x4e8270, CompanyIndustries_Draw);

// 0x4e8bb0. The company's station list (company +0x88; 0x4ba when it is gone,
// 0x4ff when it has no stations): six sortable headers (0x48f, 0x4f5,
// 0x500..0x503; widths depend on the language 0x6d3c84), then the stations
// ordered by the chosen column (name ascending, or descending with +0x760; the
// numbers largest first), a page of 9 from row +0x7c * 9: name, the building's
// +0x3d5, the four station figures, a tooltip (0x4f3) and the station id per
// row (ids 0x3f48 + row, the selected row +0x98 grey). The shown site ids go
// to +0x11c. With +0x760 a numeric column never updates its bound, so only
// values below -1e6 qualify and the list stays empty.
RT3_NOINLINE void __fastcall CompanyStations_Draw(uint8_t* p, int, float x, float y, float w, float h, int32_t sortKey) {
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(p, 0x88))) {
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, GetString(0x4ba), x, y, w, h, 0xff000000, 0, 0, 2, 1.0f);
        return;
    }
    int32_t* co = (int32_t*)Collection_Get(g_companies, 0, F<uint32_t>(p, 0x88));
    if (!Company_CountCargoSites(co, 0, 1)) {
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, GetString(0x4ff), x, y, w, h, 0xff000000, 0, 0, 2, 1.0f);
        return;
    }
    uint8_t used[200];
    memset(used, 0, sizeof used);
    memset(p + 0x11c, 0, 0x640);
    Obj_YearOrNow(p);
    uint32_t widths[6] = { 0x3e4ccccd, 0x3d8f5c29, 0x3d8f5c29, 0x3d8f5c29, 0x3da3d70a, 0x3da3d70a };
    int32_t lang = g_language;
    if (lang == 4) {
        widths[0] = 0x3e366667;
        widths[1] = 0x3ddc28f6;
        widths[5] = 0x3dbae147;
    } else if (lang == 3) {
        widths[0] = 0x3e333333;
        widths[1] = 0x3dcf5c29;
        widths[2] = 0x3d933333;
        widths[3] = 0x3da51eb8;
        widths[4] = widths[5] = 0x3daa3d70;
    } else if (lang == 2) {
        widths[2] = widths[3] = 0x3da66666;
        widths[0] = 0x3e466667;
        widths[1] = 0x3d9c28f6;
        widths[4] = widths[5] = 0x3d9eb852;
    }
    static const int32_t titles[6] = { 0x48f, 0x4f5, 0x500, 0x501, 0x502, 0x503 }, tips[6] = { 0x504, 0x4fa, 0x505, 0x506, 0x507, 0x508 };
    float cx = x;
    for (int32_t i = 0; i < 6; ++i) {
        const char* tip = GetString(tips[i]);
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), GetString(titles[i]), cx, y, BitsF(widths[i]), BitsF(0x3d9374bc), 3, 4, 3, tip, i + 0x3ffc, 0xff000000,
                      0xff6d6b6e);
        cx = (float)((double)cx + BitsF(widths[i]));
    }
    Collection* stations = g_stations;
    auto site = [&](int32_t n) { return (uint8_t*)Collection_Get(stations, 0, Company_NthSite(co, 0, n, 1)); };
    float rowY = (float)((double)y + D(0x5d1140));
    int32_t seen = 0, rowId = 0x3f48;
    int32_t* slot = (int32_t*)(p + 0x11c);
    char buf[0x200];
    float v = 0.0f;
    for (int32_t r = 0; r < 200; ++r, ++slot) {
        int32_t best = -1;
        float bestVal = BitsF(0xc97423f0);
        const char* bestName = nullptr;
        if (Company_CountCargoSites(co, 0, 1) <= 0) return;
        int32_t n = 1;
        do {
            if (!used[n - 1]) {
                const char* name = nullptr;
                switch (sortKey) {
                case 0: name = (const char*)site(n) + 0x46b; break;
                case 1: v = (float)F<int32_t>(Obj_Building(site(n)), 0x3d5); break;
                case 2: v = (float)Obj_Float4ec(site(n), 0, 1); break;
                case 3: v = (float)Obj_Float4c4(site(n), 0, 1); break;
                case 4: v = (float)Obj_Float53c(site(n), 0, 1); break;
                case 5: v = (float)Obj_Float514(site(n), 0, 1); break;
                default: v = (float)F<uint32_t>(Obj_Building(site(n)), 0x3cc); break;
                }
                bool take;
                if (!p[0x760]) {
                    if (sortKey) take = v > bestVal;
                    else take = !bestName || crt::stricmp(name, bestName) < 0;
                    if (take) bestVal = v;
                } else if (sortKey)
                    take = v < bestVal;
                else
                    take = !bestName || crt::stricmp(name, bestName) > 0;
                if (take) {
                    best = n - 1;
                    bestName = name;
                }
            }
            ++n;
        } while (n - 1 < Company_CountCargoSites(co, 0, 1));
        if (best == -1) return;
        used[best] = 1;
        if (++seen <= F<int32_t>(p, 0x7c) * 9) continue;
        uint32_t color = F<int32_t>(p, 0x98) == rowId ? 0xff6d6b6e : 0xff000000;
        uint8_t* st = site(best + 1);
        *slot = (int32_t)Company_NthSite(co, 0, best + 1, 1);
        FormatText(buf, (const char*)st + 0x46b, 0, 0, 0, 0, 0, 0, 0, 0);
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, x, rowY, BitsF(widths[0]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        float colX = (float)((double)BitsF(widths[0]) + x);
        Num_Format(buf, 8, (double)F<int32_t>(Obj_Building(st), 0x3d5), 10, 1, 0, 0);
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[1]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        Num_Format(buf, 1, (double)Ftol(Obj_Float4ec(st, 0, 1)), 10, 1, 0, 0);
        colX = (float)((double)colX + BitsF(widths[1]));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[2]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        Num_Format(buf, 1, (double)Ftol(Obj_Float4c4(st, 0, 1)), 10, 1, 0, 0);
        colX = (float)((double)colX + BitsF(widths[2]));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[3]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        Num_FormatSimple2(buf, 0xb, Obj_Float53c(st, 0, 1), 0xb);
        colX = (float)((double)colX + BitsF(widths[3]));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[4]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        Num_FormatSimple2(buf, 0xb, Obj_Float514(st, 0, 1), 0xb);
        colX = (float)((double)colX + BitsF(widths[4]));
        Font_DrawTextBox(F<uint8_t*>(p, 0xe55), 0, buf, colX, rowY, BitsF(widths[5]), BitsF(0x3d2c0831), color, 3, 0, 2, 1.0f);
        if (rowId - 0x3f48 > 0) Ui_Line(0x2f000000, 0, x, rowY, (float)((double)x + w), rowY, 2.0f);
        FormatText(buf, GetString(0x4f3), (const char*)st + 0x46b, 0, 0, 0, 0, 0, 0, 0);
        Window_SendEvent5(p, 0, 0x6e, rowId, (uint32_t)(uintptr_t)buf, 0, 0);
        Window_SendEvent5(p, 0, 0x7c, rowId, F<uint32_t>(st, 0), 0, 0);
        if (++rowId - 0x3f48 >= 9) return;
        rowY = (float)((double)rowY + D(0x5ce8c0));
    }
}
RT3_REPLACE(0x4e8bb0, CompanyStations_Draw);

// 0x4e9460. The cargo ranking list: five sortable headers (0x509..0x50c,
// 0xafe; column widths depend on the language 0x6d3c84), then the cargo types
// already available (Obj_YearReached) ordered by the chosen column (table
// order, demand, +0x5e, +0x62, price +0x6a; descending, or ascending with
// +0x760), a page of 9 from row +0x7c * 9: icon (+0xb35 list), name and the
// four figures ("---" for the price of cargo flagged +0x46), separated by
// lines. The shown cargo ids go to +0x11c.
RT3_NOINLINE void __fastcall CargoRanking_Draw(uint8_t* p, int, float x, float y, float w, float, int32_t sortKey) {
    uint8_t used[200];
    memset(used, 0, sizeof used);
    memset(p + 0x11c, 0, 0x640);
    Obj_YearOrNow(p);
    uint32_t widths[5] = { 0x3e19999a, 0x3dcccccd, 0x3dcccccd, 0x3dcccccd, 0x3dcccccd };
    int32_t lang = g_language;
    if (lang == 1) {
        widths[1] = 0x3e01eb85;
        widths[2] = 0x3dc66667;
        widths[3] = 0x3dd1eb85;
        widths[4] = 0x3dd851ec;
    } else if (lang == 3)
        widths[3] = 0x3dc00000;
    else if (lang == 2) {
        widths[1] = widths[2] = 0x3dd9999a;
        widths[3] = 0x3de00000;
        widths[4] = 0x3db33333;
    }
    static const int32_t titles[5] = { 0x509, 0x50a, 0x50b, 0x50c, 0xafe }, tips[5] = { 0x50e, 0x50f, 0x510, 0x511, 0x512 };
    float cx = x;
    for (int32_t i = 0; i < 5; ++i) {
        const char* tip = GetString(tips[i]);
        TextLink_Draw(p, F<uint8_t*>(p, 0xe55), GetString(titles[i]), cx, y, BitsF(widths[i]), BitsF(0x3d9374bc), 3, 4, 3, tip, i + 0x406a, 0xff000000,
                      0xff6d6b6e);
        cx = (float)((double)cx + BitsF(widths[i]));
    }
    Collection* cargo = g_cargoTypes;
    auto at = [&](int32_t j) { return (uint8_t*)Collection_Get(cargo, 0, Collection_IdAt(cargo, 0, j, 0)); };
    float rowY = (float)((double)y + D(0x5d1140));
    int32_t rows = 0, seen = 0;
    int32_t* slot = (int32_t*)(p + 0x11c);
    char buf[0x200];
    for (int32_t r = 0; r < 200; ++r, ++slot) {
        int32_t best = -1;
        float hi = BitsF(0xc97423f0), lo = BitsF(0x4e6e6b28);
        for (int32_t j = 0; j < Collection_Count(cargo); ++j) {
            if (used[j]) continue;
            if (!Obj_YearReached(at(j))) continue;
            float v;
            switch (sortKey) {
            case 1: v = (float)Cargo_DemandWeight(at(j)); break;
            case 2: v = F<float>(at(j), 0x5e); break;
            case 3: v = F<float>(at(j), 0x62); break;
            case 4: v = at(j)[0x46] ? 0.0f : F<float>(at(j), 0x6a); break;
            default: v = (float)(Collection_Count(cargo) - j); break;
            }
            if (!p[0x760]) {
                if (v > hi) {
                    hi = v;
                    best = j;
                }
            } else if (v < lo) {
                lo = v;
                best = j;
            }
        }
        if (best == -1) return;
        used[best] = 1;
        if (++seen <= F<int32_t>(p, 0x7c) * 9) continue;
        uint8_t* c = at(best);
        *slot = (int32_t)Collection_IdAt(cargo, 0, best, 0);
        uint32_t icon = Table_Entry(F<uint8_t*>(p, 0xb35 + best * 4), 0, 0);
        float gy = (float)((double)rowY + D(0x5c9bc0));
        float gx = (float)((double)x + D(0x5c9bc0));
        Glyph_Queue((uint8_t*)(uintptr_t)icon, 0, gx, gy, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
        FormatText(buf, LocalizeName((const char*)(c + 4)), 0, 0, 0, 0, 0, 0, 0, 0);
        uint8_t* font = F<uint8_t*>(p, 0xe55);
        Font_DrawTextBox(font, 0, buf, x, rowY, BitsF(0x3e19999a), BitsF(0x3d2c0831), 0xff000000, 3, 0, 2, 1.0f);
        float colX = (float)((double)x + Fl(0x5ce500));
        Num_Format(buf, 1, Cargo_DemandWeight(c), 10, 1, 0, 0);
        Font_DrawTextBox(font, 0, buf, colX, rowY, BitsF(widths[1]), BitsF(0x3d2c0831), 0xff000000, 3, 0, 2, 1.0f);
        Num_Format(buf, 1, (double)F<float>(c, 0x5e), 10, 1, 0, 0);
        colX = (float)((double)colX + BitsF(widths[1]));
        Font_DrawTextBox(font, 0, buf, colX, rowY, BitsF(widths[2]), BitsF(0x3d2c0831), 0xff000000, 3, 0, 2, 1.0f);
        Num_Format(buf, 1, (double)F<float>(c, 0x62), 10, 1, 0, 0);
        colX = (float)((double)colX + BitsF(widths[2]));
        Font_DrawTextBox(font, 0, buf, colX, rowY, BitsF(widths[3]), BitsF(0x3d2c0831), 0xff000000, 3, 0, 2, 1.0f);
        Num_FormatSimple(buf, 9, (double)F<float>(c, 0x6a) / Fl(0x5c86a0), 0xb);
        if (c[0x46]) *(uint32_t*)buf = 0x2d2d2d;
        colX = (float)((double)colX + BitsF(widths[3]));
        Font_DrawTextBox(font, 0, buf, colX, rowY, BitsF(widths[4]), BitsF(0x3d2c0831), 0xff000000, 3, 0, 2, 1.0f);
        if (rows > 0) Ui_Line(0x2f000000, 0, x, rowY, (float)((double)x + w), rowY, 2.0f);
        if (++rows >= 9) return;
        rowY = (float)((double)rowY + D(0x5ce8c0));
    }
}
RT3_REPLACE(0x4e9460, CargoRanking_Draw);

// 0x4e9b20. Company report window `w` (company +0x88) at (x, y), `mode` 0 / 1
// (income statement / balance sheet): the label column and up to 3 / 2 year
// columns plus a total column (mode 0; year code 0x232a), each cell a text link
// with a tooltip (row tooltip string, filled with the bond and cash rates for
// stats 9 / 13) and id (0x10b3 + column x 5) x 4 + row. Without a company it
// shows string 0x4ba.
RT3_NOINLINE void __fastcall CompanyReport_Draw(uint8_t* w, int, float x, float y, float wd, float h, int32_t mode) {
    if (!Collection_IsValid(g_companies, 0, F<uint32_t>(w, 0x88))) {
        Font_DrawTextBox(F<uint8_t*>(w, 0xe55), 0, GetString(0x4ba), x, y, wd, h, 0xff000000, 0, 0, 2, 1.0f);
        return;
    }
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, F<uint32_t>(w, 0x88));
    float colX[6];
    float s = BitsF(0x3b83126f);
    for (int i = 0; i < 6; ++i) {
        colX[i] = s;
        double inc = i == 0 ? (mode == 1 ? BitsD(0x3fc999999999999aull) : BitsD(0x3fc5c28f5c28f5c3ull))
                            : (mode == 1 ? BitsD(0x3fbf7ced916872b0ull) : BitsD(0x3fba5e353f7ced91ull));
        s = (float)((double)s + inc);
    }
    int32_t founded = F<uint16_t>(co, 0x157);
    int32_t ncols = Obj_YearOrNow(w) + (1 - founded) + (mode != 1 ? 2 : 1);
    int32_t maxCols = mode == 1 ? 4 : 5;
    if (maxCols < ncols) ncols = maxCols;
    if (ncols <= 0) return;
    float y0 = (float)((double)y + D(0x5cfdd8));
    char text[0x184], tip[0x200], a[0x130], b[0x130];
    for (int32_t c = 0; c < ncols; ++c) {
        float rowY = y0;
        float cx = (float)((double)x + colX[c]);
        float cw = (float)((double)colX[c + 1] - colX[c]);
        int32_t idBase = (c * 5 + 0x10b3) * 4;
        for (int32_t r = 0; r < 0x15; ++r) {
            int32_t row = mode * 20 + r;
            int32_t yr = Obj_YearOrNow(w) - (c - 1);
            if (r == 0) {
                *(uint16_t*)tip = 0x20;
            } else {
                int32_t kind = ReportRowInt(row, 0xa);
                if (kind == 9 || kind == 0xd) {
                    Num_Format(a, 5, (double)Company_BondRate(co), 10, 1, 0, 0);
                    Num_Format(b, 5, (double)Company_CashRate(co), 10, 1, 0, 0);
                    FormatText(tip, GetString(ReportRowInt(row, 0x1c)), a, b, 0, 0, 0, 0, 0, 0);
                } else {
                    FormatText(tip, GetString(ReportRowInt(row, 0x1c)), 0, 0, 0, 0, 0, 0, 0, 0);
                }
            }
            if (mode != 1 && c == ncols - 1) yr = 0x232a;
            if (c == 0) {
                if (r == 0) {
                    text[0] = 0;
                } else {
                    strcpy(text, GetString(ReportRowInt(row, 0x14)));
                    if (!strlen(text) && ReportRowInt(row, 0xa) != -1) strcpy(text, GetString(((const int32_t*)0x620b88)[ReportRowInt(row, 0xa)]));
                }
            } else if (r == 0) {
                if (yr == 0x232a) strcpy(text, GetString(0x263));
                else Num_Format(text, 8, (double)(Obj_YearOrNow(w) - (c - 1)), 10, 1, 0, 0);
            } else {
                int32_t kind = ReportRowInt(row, 0xa);
                if (kind == -1) text[0] = 0;
                else if (yr == 0x232a && !ReportRowByte(row, 0x13)) strcpy(text, g_language == 1 ? (const char*)0x5ca2b4 : GetString(0xe4));
                else {
                    float v = (float)Company_Stat(co, 0, kind, yr);
                    Num_Format(text, ReportRowInt(row, 0x18), (double)v, 10, 1, 0, 0);
                }
            }
            uint32_t flags = 2;
            if (r > 0 && ReportRowByte(row, 0x12) && c > 0) flags = 10;
            float tw = cw;
            int32_t align = 3;
            if (c == 0) {
                if (r > 0 && ReportRowInt(row, 0xa) == -1) {
                    align = 0;
                    tw = (float)((double)tw - D(0x5d1168));
                } else {
                    align = 2;
                    tw = (float)((double)tw + D(0x5d1160));
                }
            }
            int32_t lang = g_language;
            if ((lang == 4 || lang == 3 || lang == 2) && c == 0) tw = (float)((double)tw + D(0x5d0110));
            TextLink_DrawPlain(w, F<uint8_t*>(w, 0xe55), text, cx, rowY, tw, BitsF(0x3ca3d70a), 0xff000000, align, 0, flags, tip, idBase + r);
            rowY = (float)((double)rowY + D(0x5d10f0));
            if (r > 0) rowY = (float)((double)rowY + *(const float*)(0x622188 + row * 0x16 + 0xe));
        }
    }
}
RT3_REPLACE(0x4e9b20, CompanyReport_Draw);

}  // namespace rt3
