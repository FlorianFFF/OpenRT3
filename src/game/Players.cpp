// Players.cpp - RT3.exe 0x476000-0x478000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <initializer_list>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/stream.h"
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t __fastcall Table_CountField4(Collection* t);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
int32_t __fastcall Obj_Get21(void* o);
void* __fastcall Obj_Ref25(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void* SelectedTrain();
double __fastcall Obj_Rounded29(void* o);
int32_t __fastcall Obj_CargoBonus(void* o, int, int32_t i, uint32_t company, uint32_t player, uint32_t region);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void __fastcall NameTable_PushPos(uint8_t* t);
void __fastcall NameTable_PopPosition(uint8_t* t);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t ClaimRandomSlot();
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
double __fastcall Company_FloatD7b(void* c);
double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
double __fastcall Player_HoldingsCollateral(uint8_t* p, int, int32_t stored);
float __fastcall Fade_Step_Impl();
double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Fade_Save();
void __fastcall Fade_Restore();
void __fastcall Scene_GetVectors(uint8_t* s, int, uint32_t* a, uint32_t* b, uint32_t* c);

void __fastcall Engine_QueueGradientQuad(uint32_t c0, uint32_t c1, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                                         uint32_t x3, uint32_t y3, uint32_t c2, uint32_t c3, int32_t clip, uint32_t cx0, uint32_t cy0,
                                         uint32_t cx1, uint32_t cy1);

void __fastcall Obj_CellPos(void* o, int, float* x, float* y);
double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
int32_t __fastcall Collection_Count(Collection* c);
uint32_t __fastcall Collection_IdAt(Collection* c, int, uint32_t pos, int list);
int32_t World_AnyCondition();
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
int32_t IsMultiplayer();
int32_t __fastcall Obj_Get25(void* o);
void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
int32_t __fastcall Company_FreeShares(void* c);
int32_t __fastcall Company_SpareShares(int32_t* c, int, int32_t flag);
int32_t __fastcall Player_TradeBlock(uint8_t* p, int, uint32_t company, int32_t buying);
int32_t __fastcall Player_CanBuyShares(uint8_t* p, int, uint32_t company, int32_t margin, int32_t n);
int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
int32_t __fastcall MinutesFromParts(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute);
void CompanyList_ClearRows();
void World_ProcessMarginCalls();
void __fastcall Player_MarginCheck(uint8_t* p);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE void __fastcall World_AddNamedEntry(void* w, int, const char* name, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE int32_t __fastcall World_SetSpeed(uint8_t* w, int, int32_t speed, int32_t request, int32_t announce);
uint32_t __fastcall Player_CheckCanFound(uint8_t* p);
void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force);
int32_t __fastcall Player_CanSellShares(uint8_t* p, int, uint32_t company, int32_t shortOk, int32_t n, int32_t flag);
void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE float __fastcall Company_SharePrice(uint8_t* c, int, int32_t issued, int32_t store);
RT3_NOINLINE void World_ProcessMarginCalls();
RT3_NOINLINE void __fastcall Obj_AddDouble154(void* o, int, int32_t amount);
void __fastcall Nop476080(void*);
RT3_NOINLINE void __fastcall Nop476090(void*);
void __fastcall Obj_SetSlot2ab(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add);
int32_t ClaimRandomSlot();
RT3_NOINLINE void __fastcall Player_Init(uint8_t* p, int, int32_t id, int32_t kind, const char* name, const char* desc, int32_t ai, int32_t clearHistory);
RT3_NOINLINE double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
double __fastcall Player_HoldingsCollateral(uint8_t* p, int, int32_t stored);
RT3_NOINLINE void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t);
void __fastcall Company_UpdateCreditRate(uint8_t* o);
RT3_NOINLINE double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
RT3_NOINLINE double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind);
RT3_NOINLINE void __fastcall Player_RecordWealth(uint8_t* p);
RT3_NOINLINE int32_t __fastcall Player_OwnsAllOf(uint8_t* p, int, uint32_t id);
RT3_NOINLINE int32_t __fastcall Player_CompanyAgePenalty(uint8_t* p);
float __fastcall Player_Aggressiveness(uint8_t* p);
RT3_NOINLINE int32_t World_AnyCondition();
RT3_NOINLINE uint8_t* __fastcall Obj_NamedEntry40(uint8_t* o);
RT3_NOINLINE uint8_t* __fastcall Obj_NamedEntry20(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_NameIndex(void* o);
RT3_NOINLINE double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
RT3_NOINLINE int32_t __fastcall Player_CanSellShares(uint8_t* p, int, uint32_t company, int32_t shortOk, int32_t n, int32_t flag);
RT3_NOINLINE uint32_t __fastcall Player_CheckCanFound(uint8_t* p);
RT3_NOINLINE int32_t __fastcall Player_TradeBlock(uint8_t* p, int, uint32_t company, int32_t buying);
RT3_NOINLINE int32_t __fastcall Player_CanBuyShares(uint8_t* p, int, uint32_t company, int32_t margin, int32_t n);
RT3_NOINLINE void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force);
RT3_NOINLINE void __fastcall Player_MarginCheck(uint8_t* p);
RT3_NOINLINE Collection* __fastcall Players_Construct(Collection* c);
RT3_NOINLINE void __fastcall Players_Destruct(Collection* c);
RT3_NOINLINE int32_t __fastcall Players_Load(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE void __fastcall Players_Save(Collection* t, int, Stream s, int32_t recordSize);
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
RT3_NOINLINE uint32_t __fastcall Table_NthActive(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t __fastcall Table_ActiveIndexOf(Collection* t, int, uint32_t id);
void __fastcall MapView_LoadTrainFont(uint8_t* v);
RT3_NOINLINE double __fastcall Obj_MapX(uint8_t* o, int, float x);
RT3_NOINLINE float __fastcall Obj_MapY(uint8_t* o, int, float y);
RT3_NOINLINE float __fastcall Obj_ScaledExtent(uint8_t* o);
void* __fastcall Obj_SelectedTrainIfFlag(uint8_t* o);
void __fastcall MapView_Init(uint8_t* v);
RT3_NOINLINE void __fastcall Minimap_WorldToPixel(uint8_t* o, int, float x, float y, float* outX, float* outY);
RT3_NOINLINE void __fastcall MiniMap_DrawView(uint8_t* m);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE void __fastcall Scene_GetVectors(uint8_t* s, int, uint32_t* a, uint32_t* b, uint32_t* c);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);


// 0x476ab0 / 0x476b30. Field +0x40 / +0x20 of the (*0x6d4020)+0x429b4 entry
// named like this +0xc47. QUIRK: without a match the last entry is used,
// and with no entries this object itself.
static uint8_t* FindNamedEntry(uint8_t* o) {
    auto list = []() { return F<Collection*>(g_pack, 0x429b4); };
    uint8_t* e = o;
    for (int32_t i = 0; i < Collection_Count(list()); ++i) {
        Collection* l = list();
        e = (uint8_t*)Collection_Get(l, 0, Collection_IdAt(l, 0, i, 0));
        if (crt::stricmp((const char*)e, (const char*)o + 0xc47) == 0) break;
    }
    return e;
}

static inline double D(uint32_t a) { return *(const double*)rt3::Rdata(a); }

static inline double Bits(uint64_t u) { return *(double*)&u; }

static inline double Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

// A centred (-1, -1) message box of `type` showing `text` with the default
// options (0x1e) and `timeout`.
static uint32_t Message_Show(const char* text, int32_t type, int32_t timeout) {
    return Dialog_RunMessage(text, type, 0xbf800000, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, timeout);
}

// News entry (world log) for player p's forced trade when p is the local player.
static void Margin_Report(uint8_t* p, int32_t fmt, int32_t amount, uint32_t id, bool nameFirst) {
    if (F<int32_t>(p, 0) != Obj_Get25(g_world)) return;
    const char* company = (const char*)Collection_Get(g_companies, 0, id) + 4;
    char num[0x1f8], text[0x2c0];
    const char* s = Num_Format(num, 1, (double)amount, 10, 1, 0, 0);
    if (nameFirst) FormatText(text, GetString(fmt), company, s, 0, 0, 0, 0, 0, 0);
    else FormatText(text, GetString(fmt), s, company, 0, 0, 0, 0, 0, 0);
    World_AddNamedEntry(g_world, 0, text, 0, 1, -1, -1, -1, -1);
}

// 0x476050. +0x154 (double) += whole amount.
RT3_NOINLINE void __fastcall Obj_AddDouble154(void* o, int, int32_t amount) {
    double& d = F<double>(o, 0x154);
    d = (double)amount + d;
}
RT3_REPLACE(0x476050, Obj_AddDouble154);

void __fastcall Nop476080(void*) { __asm nop }
RT3_REPLACE(0x476080, Nop476080);

RT3_NOINLINE void __fastcall Nop476090(void*) { __asm nop }
RT3_REPLACE(0x476090, Nop476090);

// 0x4760a0. As 0x424050 for the slots +0x2ab with last key +0xc43.
void __fastcall Obj_SetSlot2ab(void* o, int, int32_t idx, int32_t value, int32_t key, int32_t add) {
    if (key == F<int32_t>(o, 0xc43)) return;
    if (add) F<int32_t>(o, 0x2ab + idx * 4) += value;
    else F<int32_t>(o, 0x2ab + idx * 4) = value;
    if (key) F<int32_t>(o, 0xc43) = key;
}
RT3_REPLACE(0x4760a0, Obj_SetSlot2ab);

// 0x4760f0. Claims a random free slot of the 41-entry table 0x5f2d28
// (stride 0x27); after 100 probes the probed slot is taken anyway.
RT3_NOINLINE int32_t ClaimRandomSlot() {
    int32_t r = SimRand(0, 0x28);
    uint8_t* t = (uint8_t*)0x5f2d28;
    for (int32_t probes = 0;; ++probes) {
        if (probes > 100) t[r * 0x27] = 0;
        if (!t[r * 0x27]) break;
        r = (r + 1) % 0x29;
    }
    t[r * 0x27] = 1;
    return r;
}
RT3_REPLACE(0x4760f0, ClaimRandomSlot);

// 0x476140. Sets up player record p: id, kind (+0x15c), name (copied, max 30; empty
// gives "Unassigned N", string 0xb87), description (+0x27, max 300), AI flag
// (+0x291); the flags +0x292 / +0x293 are set unless this is the only player; a
// random portrait slot is claimed; the first decision delay (+0x1e5) is a random
// 1..3 days with option +0x2ab, else 45..270 (x 0x5a0 ticks); the stat tables are
// cleared (and +0x15d with `clearHistory`).
RT3_NOINLINE void __fastcall Player_Init(uint8_t* p, int, int32_t id, int32_t kind, const char* name, const char* desc, int32_t ai,
                            int32_t clearHistory) {
    int32_t players = Table_CountField4(g_players);
    p[0x294] = 1;
    if (players == 1) {
        p[0x292] = 0;
        p[0x293] = 0;
    } else {
        p[0x292] = 1;
        p[0x293] = 1;
    }
    p[0x291] = (uint8_t)ai;
    p[0x295] = 1;
    p[0xc47] = 0;
    F<int32_t>(p, 0) = id;
    F<int32_t>(p, 4) = 1;
    F<int32_t>(p, 0x296) = 0;
    F<int32_t>(p, 0x29a) = 0;
    F<int32_t>(p, 0x2a7) = 0;
    p[0x29e] = 0;
    F<int32_t>(p, 0x154) = 0;
    F<uint32_t>(p, 0x158) = 0x40f86a00;
    memset(p + 0x1e9, 0, 0xa0);
    F<int32_t>(p, 0x289) = 0;
    F<int32_t>(p, 0x28d) = 0;
    F<int32_t>(p, 0x1e1) = id == 1 ? 1 : 0;
    ClaimRandomSlot();
    p[0x15c] = (uint8_t)kind;
    if (!*name) {
        char num[0x100];
        Num_Format(num, 1, (double)F<int32_t>(p, 0), 10, 1, 0, 0);
        FormatText((char*)p + 8, GetString(0xb87), num, 0, 0, 0, 0, 0, 0, 0);
    } else {
        StrCopyN((char*)p + 8, name, 0x1e);
    }
    StrCopyN((char*)p + 0x27, desc, 0x12c);
    F<int32_t>(p, 0x1dd) = 0;
    F<int32_t>(p, 0x2a3) = 0;
    int32_t days = F<int32_t>(g_app, 0x2ab) ? SimRand(1, 3) : SimRand(0x2d, 0x10e);
    F<int32_t>(p, 0x1e5) = days * 0x5a0;
    F<int32_t>(p, 0xc43) = 1;
    if (clearHistory) memset(p + 0x15d, 0, 0x80);
    memset(p + 0x2ab, 0, 0x998);
    memset(p + 0xc66, 0, 0x45);
}
RT3_REPLACE(0x476140, Player_Init);

// 0x476320. Value of a player's holdings (+0x15d: shares of companies 0..31): at
// the stored price +0xd7b (`stored`) or the current share price; each share count
// is rounded through a float.
RT3_NOINLINE double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored) {
    double total = 0;
    for (int32_t i = 0; i < 0x20; ++i) {
        int32_t n = F<int32_t>(p, 0x15d + i * 4);
        if (!n) continue;
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, i);
        double price = stored ? Company_FloatD7b(c) : (double)Company_SharePrice(c, 0, 0, 0);
        total = price * (float)n + total;
    }
    return total;
}
RT3_REPLACE(0x476320, Player_HoldingsValue);

// 0x4763b0. Collateral value of a player's holdings for margin: half of each
// position's value, but 1.5x for short (negative) positions.
RT3_NOINLINE double __fastcall Player_HoldingsCollateral(uint8_t* p, int, int32_t stored) {
    double total = 0;
    for (int32_t i = 0; i < 0x20; ++i) {
        int32_t n = F<int32_t>(p, 0x15d + i * 4);
        if (!n) continue;
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, i);
        double price = stored ? Company_FloatD7b(c) : (double)Company_SharePrice(c, 0, 0, 0);
        double v = price * (double)n;
        double part = v / D(0x5c85d0);
        if (!(v >= D(0x5c8608))) part = part + v;
        total = part + total;
    }
    return total;
}
RT3_REPLACE(0x4763b0, Player_HoldingsCollateral);

// 0x476460. Player p: sells n shares of `company` (when holding none, short
// up to the spare shares; otherwise at most the holding) at the price the
// sale moves the market to, crediting 0x5c8928 of the proceeds; then
// re-checks margin calls and clears the company list rows.
RT3_NOINLINE void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t) {
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, company);
    int32_t& held = F<int32_t>(p, 0x15d + company * 4);
    int32_t shortSale = held <= 0;
    if (shortSale) {
        if (n >= Company_SpareShares((int32_t*)co, 0, 0)) n = Company_SpareShares((int32_t*)co, 0, 0);
    } else {
        if (n >= held) n = held;
        if (n <= 0 || n > held) return;
    }
    if (n > held && !shortSale) n = held;
    int32_t total = Ftol(Company_SharePrice(co, 0, -n, 1)) * n;
    held -= n;
    F<double>(p, 0x154) = (double)Ftol((double)total * D(0x5c8928)) + F<double>(p, 0x154);
    World_ProcessMarginCalls();
    CompanyList_ClearRows();
}
RT3_REPLACE(0x476460, Player_SellShares);

// 0x476560. Drifts the company's credit rate (+0x154, double) towards the market
// rate plus its bonus (stat 0x39): the gap is capped by the current band (0.01 above
// k1 .. 0.04 above k4) and a k-th of it is applied, truncated. Off in sandbox mode.
void __fastcall Company_UpdateCreditRate(uint8_t* o) {
    uint8_t* w = g_world;
    if (F<int32_t>(w, 0x46c38)) return;
    float a = (float)Obj_Rounded29(w);
    int32_t n = Obj_CargoBonus(hook::As<void*>(g_world), 0, 0x39, 0, F<uint32_t>(o, 0), 0);
    a = (float)((double)n * D(0x5c85b8) + a);
    double rate = F<double>(o, 0x154);
    if (rate > D(0x5c8608)) {
        double b = (double)a - D(0x5c9b98);
        double q = D(0x5c8608) > b ? 0.0 : b;
        a = (float)q;
        double v;
        bool set = true;
        if (rate > D(0x5ce820)) v = a < D(0x5c85b8) ? (double)a : Bits(0x3f847ae147ae147bull);
        else if (rate > D(0x5ce818)) v = a < D(0x5c8918) ? (double)a : Bits(0x3f947ae147ae147bull);
        else if (rate > D(0x5ce810)) v = a < D(0x5c9b80) ? (double)a : Bits(0x3f9eb851eb851eb8ull);
        else if (rate > D(0x5c8810)) v = a < D(0x5c9b98) ? (double)a : Bits(0x3fa47ae147ae147bull);
        else set = false;
        if (set) a = (float)v;
    }
    double s = (double)a * F<double>(o, 0x154) / D(0x5c8978);
    int32_t step;
    __asm {
        fld s
        call Rt3_Ftol
        mov step, eax
    }
    F<double>(o, 0x154) = (double)step + F<double>(o, 0x154);
}
RT3_REPLACE(0x476560, Company_UpdateCreditRate);

// 0x476780 / 0x476c20. A player's cash (+0x154) plus the value / collateral value
// of the holdings.
RT3_NOINLINE double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored) { return Player_HoldingsValue(p, 0, stored) + F<double>(p, 0x154); }
RT3_REPLACE(0x476780, Player_NetWorth);

// 0x4767a0. Player wealth by kind: 0 cash, 1 holdings at market price, 3 both.
RT3_NOINLINE double __fastcall Player_Wealth(uint8_t* p, int, int32_t kind) {
    switch (kind) {
    case 0: return F<double>(p, 0x154);
    case 1: return Player_HoldingsValue(p, 0, 0);
    case 3: {
        double h = Player_Wealth(p, 0, 1);
        return Player_Wealth(p, 0, 0) + h;
    }
    default: return D(0x5c8608);
    }
}
RT3_REPLACE(0x4767a0, Player_Wealth);

// 0x476810. Shifts the player's wealth history (+0x1e9, 18 doubles) back by two
// and stores the current cash and holdings value in front.
RT3_NOINLINE void __fastcall Player_RecordWealth(uint8_t* p) {
    crt::memmove(p + 0x1f9, p + 0x1e9, 0x90);
    F<double>(p, 0x1e9) = F<double>(p, 0x154);
    F<double>(p, 0x1f1) = Player_HoldingsValue(p, 0, 0);
}
RT3_REPLACE(0x476810, Player_RecordWealth);

// 0x4768c0. Whether this player's holding +0x15d[id] equals company id's total +0x47 (sole owner).
RT3_NOINLINE int32_t __fastcall Player_OwnsAllOf(uint8_t* p, int, uint32_t id) {
    if (!id) return 0;
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, id);
    if (!c) return 0;
    return F<int32_t>(c, 0x47) == F<int32_t>(p, 0x15d + id * 4) ? 1 : 0;
}
RT3_REPLACE(0x4768c0, Player_OwnsAllOf);

// 0x476900. Penalty -7 per year (up to 10) since company +0x1dd's founding (+0x43) short of 10.
RT3_NOINLINE int32_t __fastcall Player_CompanyAgePenalty(uint8_t* p) {
    uint32_t id = F<uint32_t>(p, 0x1dd);
    if (!id) return 0;
    uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, id);
    int32_t d = F<uint16_t>(g_world, 0xd) - F<int32_t>(c, 0x43);
    if (d <= 0) d = 0;
    if (d >= 10) d = 10;
    return (d - 10) * 7;
}
RT3_REPLACE(0x476900, Player_CompanyAgePenalty);

// 0x476950. AI player aggressiveness: (+0x28d + company age penalty + 100)
// / 10, at least 0.5, capped at 15; humans get 10.
float __fastcall Player_Aggressiveness(uint8_t* p) {
    if (!F<int32_t>(p, 0x1e1)) return *(const float*)rt3::Rdata(0x5c85f8);
    int32_t pen = 0;
    if (uint32_t id = F<uint32_t>(p, 0x1dd)) {
        uint8_t* c = (uint8_t*)Collection_Get(g_companies, 0, id);
        int32_t d = F<uint16_t>(g_world, 0xd) - F<int32_t>(c, 0x43);
        if (d <= 0) d = 0;
        if (d >= 10) d = 10;
        pen = (d - 10) * 7;
    }
    float v = (float)((double)(F<int32_t>(p, 0x28d) + pen + 100) / *(const double*)rt3::Rdata(0x5c85d8) * *(const double*)rt3::Rdata(0x5c85f0));
    double dv = (double)v > *(const double*)rt3::Rdata(0x5c8628) ? (double)v : 0.5;
    float f = (float)dv;
    if ((double)f < *(const double*)rt3::Rdata(0x5c8798)) return f;
    return 15.0f;
}
RT3_REPLACE(0x476950, Player_Aggressiveness);

// 0x476a60. World condition flags: +0x4acb with +0x21, +0x4ad7 with its player's
// +0x2a7 >= 1, or +0x4ad3.
RT3_NOINLINE int32_t World_AnyCondition() {
    if (F<int32_t>(g_world, 0x4acb) && Obj_Get21(g_world)) return 1;
    if (F<int32_t>(g_world, 0x4ad7) && F<int32_t>(Obj_Ref25(g_world), 0x2a7) >= 1) return 1;
    return F<int32_t>(g_world, 0x4ad3) ? 1 : 0;
}
RT3_REPLACE(0x476a60, World_AnyCondition);

RT3_NOINLINE uint8_t* __fastcall Obj_NamedEntry40(uint8_t* o) { return FindNamedEntry(o) + 0x40; }
RT3_REPLACE(0x476ab0, Obj_NamedEntry40);

RT3_NOINLINE uint8_t* __fastcall Obj_NamedEntry20(uint8_t* o) { return FindNamedEntry(o) + 0x20; }
RT3_REPLACE(0x476b30, Obj_NamedEntry20);

// 0x476bb0. Index (not ID) of this object's name (+0xc47) in the name table
// of object 0x6d4020 (+0x429b4), or -1.
RT3_NOINLINE int32_t __fastcall Obj_NameIndex(void* o) {
    auto table = [] { return F<Collection*>(g_pack, 0x429b4); };
    for (int32_t i = 0; i < Collection_Count(table()); ++i) {
        Collection* t = table();
        const char* name = (const char*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (crt::stricmp((const char*)o + 0xc47, name) == 0) return i;
    }
    return -1;
}
RT3_REPLACE(0x476bb0, Obj_NameIndex);

RT3_NOINLINE double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored) {
    return Player_HoldingsCollateral(p, 0, stored) + F<double>(p, 0x154);
}
RT3_REPLACE(0x476c20, Player_CollateralWorth);

// 0x476c40. Player p: can sell n shares of `company` (n = 0: the trade
// block). Holding them is enough; short selling (`shortOk`, off in
// scenario rule +0x4a83) needs a company not run by p, n below its spare
// shares (Company_SpareShares(flag)), a price not under 0x5c8554 and the
// collateral (Player_CollateralWorth) covering n at the price.
RT3_NOINLINE int32_t __fastcall Player_CanSellShares(uint8_t* p, int, uint32_t company, int32_t shortOk, int32_t n, int32_t flag) {
    if (F<int32_t>(g_world, 0x4a83)) shortOk = 0;
    if (!n) n = Player_TradeBlock(p, 0, company, 0);
    if (F<int32_t>(p, 0x15d + company * 4) >= n) return 1;
    if (!shortOk) return 0;
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, company);
    if (F<uint32_t>(co, 0x3b) == F<uint32_t>(p, 0)) return 0;
    if (n >= Company_SpareShares((int32_t*)co, 0, flag)) return 0;
    if (Company_SharePrice(co, 0, 0, 0) < Fl(0x5c8554)) return 0;
    double worth = Player_CollateralWorth(p, 0, 1);
    float fn = (float)n;
    return (double)Company_SharePrice(co, 0, 0, 0) * fn <= worth ? 1 : 0;
}
RT3_REPLACE(0x476c40, Player_CanSellShares);

// 0x476d10. Whether player p may start a company: not while a scenario
// condition blocks it (0xb88), nor with less than 100,000 in cash and
// holdings (0x10f, showing the date record's player's worth).
RT3_NOINLINE uint32_t __fastcall Player_CheckCanFound(uint8_t* p) {
    if (World_AnyCondition()) {
        Message_Show(GetString(0xb88), 1, 0);
        return 0;
    }
    if (!(Player_HoldingsCollateral(p, 0, 1) + F<double>(p, 0x154) < D(0x5c8830))) return 1;
    uint8_t* q = (uint8_t*)Obj_Ref25(g_world);
    char have[0x2c0], need[0x2c0], text[0x400];
    const char* sh = Num_FormatSimple(have, 9, Player_HoldingsCollateral(q, 0, 1) + F<double>(q, 0x154), 0xb);
    const char* sn = Num_FormatSimple(need, 9, 100000.0, 0xb);
    FormatText(text, GetString(0x10f), sn, sh, 0, 0, 0, 0, 0, 0);
    Message_Show(text, 1, 0);
    return 0;
}
RT3_REPLACE(0x476d10, Player_CheckCanFound);

// 0x476e50. Player p: the share block to trade in `company` (1000s, at
// least 1000; 1000 unless p trades in blocks, +0x1e1). Buying starts from
// 0x5c9b98 of the holding (rounded to 1000s) and shrinks by 1000 until
// affordable (on margin when the first 1000 are not), capped at the free
// shares; selling starts from the same share of |holding| and shrinks until
// it can be sold (short when holding less than 1000).
RT3_NOINLINE int32_t __fastcall Player_TradeBlock(uint8_t* p, int, uint32_t company, int32_t buying) {
    if (!F<int32_t>(p, 0x1e1)) return 1000;
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, company);
    int32_t n;
    if (buying) {
        n = Ftol((double)F<int32_t>(p, 0x15d + company * 4) * D(0x5c9b98) + D(0x5c87f8)) / 1000 * 1000;
        int32_t margin = Player_CanBuyShares(p, 0, company, 0, 1000) == 0;
        for (int32_t ok = 0; n > 1000 && !ok;) {
            n -= 1000;
            ok = Player_CanBuyShares(p, 0, company, margin, n);
        }
        if (n > Company_FreeShares(co)) n = Company_FreeShares(co) < 1000 ? 1000 : Company_FreeShares(co);
    } else {
        int32_t held = F<int32_t>(p, 0x15d + company * 4);
        int32_t shortSell = held < 1000;
        int32_t a = held < 0 ? -held : held;
        n = Ftol((double)a * D(0x5c9b98) + D(0x5c87f8)) / 1000 * 1000;
        for (int32_t ok = 0; n > 1000 && !ok;) {
            n -= 1000;
            ok = Player_CanSellShares(p, 0, company, shortSell, n, 0);
        }
    }
    n = (n + 500) / 1000 * 1000;
    return n < 1000 ? 1000 : n;
}
RT3_REPLACE(0x476e50, Player_TradeBlock);

// 0x476ff0. Player p: can buy n shares of `company` (n = 0: the trade
// block; at least 1 must be free). Covering a short position needs nothing;
// otherwise cash +0x154 must pay n at the price, or - on `margin` (off in
// scenario rule +0x4a83) - cash plus Player_HoldingsCollateral.
RT3_NOINLINE int32_t __fastcall Player_CanBuyShares(uint8_t* p, int, uint32_t company, int32_t margin, int32_t n) {
    if (F<int32_t>(g_world, 0x4a83)) margin = 0;
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, company);
    if (!n) n = Player_TradeBlock(p, 0, company, 1);
    int32_t need = n < 1 ? 1 : n;
    if (Company_FreeShares(co) < need) return 0;
    if (F<int32_t>(p, 0x15d + company * 4) <= -n) return 1;
    float fn = (float)n;
    if ((double)Company_SharePrice(co, 0, 0, 0) * fn <= F<double>(p, 0x154)) {
        double limit = Player_HoldingsCollateral(p, 0, 1) + F<double>(p, 0x154);
        if ((double)Company_SharePrice(co, 0, 0, 0) * fn <= limit) return 1;
    }
    if (!margin) return 0;
    double limit = Player_HoldingsCollateral(p, 0, 1) + F<double>(p, 0x154);
    return (double)Company_SharePrice(co, 0, 0, 0) * fn <= limit ? 1 : 0;
}
RT3_REPLACE(0x476ff0, Player_CanBuyShares);

// 0x477110. Player p: buys n shares of `company` (unless `force`, only when
// it can buy the trade block, on margin by `margin`): moves the price,
// pays 0x5c8930 of the cost at the old price; then re-checks margin calls and
// clears the company list rows.
RT3_NOINLINE void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force) {
    if (!force && !Player_CanBuyShares(p, 0, company, margin, 0)) return;
    Company_SharePrice((uint8_t*)Collection_Get(g_companies, 0, company), 0, n, 1);
    float price = Company_SharePrice((uint8_t*)Collection_Get(g_companies, 0, company), 0, 0, 0);
    float fn = (float)n;
    int32_t cost = Ftol((double)price * fn);
    F<int32_t>(p, 0x15d + company * 4) += n;
    F<double>(p, 0x154) = F<double>(p, 0x154) - (double)Ftol((double)cost * D(0x5c8930));
    World_ProcessMarginCalls();
    CompanyList_ClearRows();
}
RT3_REPLACE(0x477110, Player_BuyShares);

// 0x4771e0. Player p: margin check. When collateral plus cash fall below
// 0x5c8608 and p holds shares: a block trader (+0x1e1) first gets until the
// deadline +0x296 (end of the month; the local player is warned, 0x11f, and
// the game slowed to speed 4), after that - or right away for others -
// positions are closed 1000 shares at a time (other companies first, then
// its own), reported with 0x120 / 0x121. Short positions in companies with
// no spare shares are always covered (0x122).
RT3_NOINLINE void __fastcall Player_MarginCheck(uint8_t* p) {
    auto broke = [&]() { return Player_HoldingsCollateral(p, 0, 1) + F<double>(p, 0x154) < D(0x5c8608); };
    if (broke()) {
        int32_t any = 0;
        for (int32_t i = 0; i < 0x20; ++i)
            if (F<int32_t>(p, 0x15d + i * 4)) any = 1;
        if (!any) return;
        uint8_t* w = g_world;
        if (F<int32_t>(p, 0x1e1) && (!F<int32_t>(p, 0x296) || F<int32_t>(p, 0x296) > F<int32_t>(w, 0x15))) {
            if (F<int32_t>(p, 0) == Obj_Get25(g_world) && !F<int32_t>(p, 0x296)) {
                int32_t month = g_world[0xf] % 12;
                char text[0x2c0];
                const char* m = GetString(((const int32_t*)0x624390)[month]);
                FormatText(text, GetString(0x11f), m, 0, 0, 0, 0, 0, 0, 0);
                World_AddNamedEntry(g_world, 0, text, 0, 1, -1, -1, -1, -1);
                if (!IsMultiplayer() && F<int32_t>(g_world, 0x19) > 4)
                    World_SetSpeed(g_world, 0, 4, 1, 1);
            }
            if (!F<int32_t>(p, 0x296)) {
                uint8_t* wd = g_world;
                F<int32_t>(p, 0x296) = MinutesFromParts(F<uint16_t>(wd, 0xd), wd[0xf], 0x1c, 0x17, 0x3b) + 0x9d7f;
            }
        } else {
            for (int32_t pass = 0; pass < 2; ++pass)
                for (int32_t c = 0; c < Collection_Count(g_companies); ++c) {
                    Collection* t = g_companies;
                    if (!F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)c, 0)), 0x3f)) continue;
                    if (!pass && F<uint32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)c, 0)), 0x3b) == F<uint32_t>(p, 0)) continue;
                    uint32_t id = F<uint32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)c, 0)), 0);
                    int32_t sold = 0, bought = 0;
                    if (F<int32_t>(p, 0x15d + id * 4) > 0) {
                        while (broke()) {
                            sold += 1000;
                            Player_SellShares(p, 0, id, 1000, 0, 1);
                            if (F<int32_t>(p, 0x15d + id * 4) <= 0) break;
                        }
                        if (sold > 0) Margin_Report(p, 0x120, sold, id, false);
                    }
                    if (F<int32_t>(p, 0x15d + id * 4) < 0) {
                        while (broke()) {
                            bought += 1000;
                            Player_BuyShares(p, 0, id, 1000, 0, 1);
                            if (F<int32_t>(p, 0x15d + id * 4) >= 0) break;
                        }
                        if (bought > 0) Margin_Report(p, 0x121, bought, id, false);
                    }
                }
            F<int32_t>(p, 0x296) = 0;
        }
    } else
        F<int32_t>(p, 0x296) = 0;
    for (uint32_t i = 0; i < 0x20; ++i) {
        if (F<int32_t>(p, 0x15d + i * 4) >= 0) continue;
        int32_t bought = 0;
        while (Company_SpareShares((int32_t*)Collection_Get(g_companies, 0, i), 0, 0) < 0) {
            bought += 1000;
            Player_BuyShares(p, 0, i, 1000, 0, 1);
            if (F<int32_t>(p, 0x15d + i * 4) >= 0) break;
        }
        if (bought > 0) Margin_Report(p, 0x122, bought, i, true);
    }
}
RT3_REPLACE(0x4771e0, Player_MarginCheck);

// 0x477740 / 0x477770. Players collection (vtbl 0x5ce828, inline 0xcab-byte records, 4 + 2).
RT3_NOINLINE Collection* __fastcall Players_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5ce828;
    Collection_Init(c, 0, 1, 0xcab, 4, 2, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x477740, Players_Construct);

RT3_NOINLINE void __fastcall Players_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5ce828;
    Collection_Destruct(c);
}
RT3_REPLACE(0x477770, Players_Destruct);

// 0x477780. Players load framed by 0x5209/0x520a/0x520b.
RT3_NOINLINE int32_t __fastcall Players_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x5209, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, 0x520a, 0, 0);
    return StreamSkip4(s, 0x520b, 0, 0) + n;
}
RT3_REPLACE(0x477780, Players_Load);

RT3_NOINLINE void __fastcall Players_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x5209);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x520a);
    StreamWriteU32(s, 0x520b);
}
RT3_REPLACE(0x4777e0, Players_Save);

// 0x477820. Count of records with a nonzero dword at +4.
RT3_NOINLINE int32_t __fastcall Table_CountField4(Collection* t) {
    int32_t total = Collection_Count(t), n = 0;
    for (int32_t i = 0; i < total; ++i)
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 4) != 0) ++n;
    return n;
}
RT3_REPLACE(0x477820, Table_CountField4);

// 0x477860. The n-th (0-based) record with a nonzero dword at +4, or NULL.
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n) {
    int32_t total = Collection_Count(t);
    for (int32_t i = 0; i < total; ++i) {
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 4) == 0) continue;
        if (n == 0) return Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        --n;
    }
    return nullptr;
}
RT3_REPLACE(0x477860, Table_NthField4);

// 0x4778c0 / 0x477920. Active (+4) players: ID of the n-th (0-based) / index of `id` among them.
RT3_NOINLINE uint32_t __fastcall Table_NthActive(Collection* t, int, int32_t n) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (!F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 4)) continue;
        if (n == 0) return Collection_IdAt(t, 0, i, 0);
        --n;
    }
    return 0;
}
RT3_REPLACE(0x4778c0, Table_NthActive);

RT3_NOINLINE int32_t __fastcall Table_ActiveIndexOf(Collection* t, int, uint32_t id) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (Collection_IdAt(t, 0, i, 0) == id) return n;
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 4)) ++n;
    }
    return 0;
}
RT3_REPLACE(0x477920, Table_ActiveIndexOf);

// 0x477980. Map views showing trains (+0x33 flags 0x2030000) load the
// "TrainDetail.imb" bitmap font into +0xaf.
void __fastcall MapView_LoadTrainFont(uint8_t* v) {
    if (!(F<uint32_t>(v, 0x33) & 0x2030000)) return;
    NameTable_PushPos(g_pack);
    F<uint8_t*>(v, 0xaf) = Pack_AcquireNamed(g_pack, 0, (const char*)0x5c9ee8);
    NameTable_PopPosition(g_pack);
}
RT3_REPLACE(0x477980, MapView_LoadTrainFont);

// 0x4779c0. Linear map of x from [+0xc3, +0xcb] scaled by +0x13, plus the
// offsets (+0x86)->+5 and +0xb; returned unrounded.
RT3_NOINLINE double __fastcall Obj_MapX(uint8_t* o, int, float x) {
    float lo = (float)F<int32_t>(o, 0xc3);
    float span = (float)(F<int32_t>(o, 0xcb) - F<int32_t>(o, 0xc3));
    double v = ((double)x - lo) * F<float>(o, 0x13);
    v = v / span;
    v = v + F<float>(F<uint8_t*>(o, 0x86), 5);
    return v + F<float>(o, 0xb);
}
RT3_REPLACE(0x4779c0, Obj_MapX);

// 0x477a10. Inverse linear map of y over [+0xc7, +0xcf] scaled by +0x17 plus the offsets (+0x86)->+9 and +0xf.
RT3_NOINLINE float __fastcall Obj_MapY(uint8_t* o, int, float y) {
    float lo = (float)F<int32_t>(o, 0xc7);
    float span = (float)(F<int32_t>(o, 0xcf) - F<int32_t>(o, 0xc7));
    double v = ((double)y - lo) / span;
    v = (*(const double*)rt3::Rdata(0x5c8770) - v) * F<float>(o, 0x17);
    double off = (double)F<float>(F<uint8_t*>(o, 0x86), 9) + F<float>(o, 0xf);
    return (float)(v + off);
}
RT3_REPLACE(0x477a10, Obj_MapY);

// 0x477a70. Largest extent (+0xb3..+0xbf, at least +0x11f) times the scale +0xdb, truncated.
RT3_NOINLINE float __fastcall Obj_ScaledExtent(uint8_t* o) {
    int32_t w = F<int32_t>(o, 0xbb) - F<int32_t>(o, 0xb3);
    int32_t h = F<int32_t>(o, 0xbf) - F<int32_t>(o, 0xb7);
    if (w <= h) w = h;
    if (w < F<int32_t>(o, 0x11f)) w = F<int32_t>(o, 0x11f);
    float fw = (float)w;
    return (float)(int32_t)((double)fw * F<float>(o, 0xdb));
}
RT3_REPLACE(0x477a70, Obj_ScaledExtent);

void* __fastcall Obj_SelectedTrainIfFlag(uint8_t* o) { return (F<uint32_t>(o, 0x33) & 0x10000) ? SelectedTrain() : nullptr; }
RT3_REPLACE(0x477ad0, Obj_SelectedTrainIfFlag);

// 0x477af0. Initialises the minimap/graph view settings; scale +0x11f is
// the smaller map side, at most 160.
void __fastcall MapView_Init(uint8_t* v) {
    F<uint16_t>(v, 6) = 0x1000;
    v[0xa2] = 0;
    uint8_t* map = g_map;
    int32_t w = F<int32_t>(map, 0x2155), h = F<int32_t>(map, 0x2159);
    int32_t m = w < h ? w : h;
    F<int32_t>(v, 0x11f) = m > 0xa0 ? 0xa0 : m;
    for (uint32_t off : { 0xdfu, 0xe3u, 0xe7u, 0xebu }) F<int32_t>(v, off) = 5;
    for (uint32_t off : { 0xbbu, 0xbfu, 0xcbu, 0xcfu }) F<int32_t>(v, off) = 10;
    for (uint32_t off : { 0xafu, 0xa3u, 0xa7u, 0xabu, 0xb3u, 0xb7u, 0xc3u, 0xc7u }) F<int32_t>(v, off) = 0;
    for (uint32_t off : { 0xdbu, 0xd3u, 0xd7u }) F<float>(v, off) = 1.0f;
    for (uint32_t k = 0; k < 5; ++k) {
        F<int32_t>(v, 0xef + k * 8) = 0;
        F<float>(v, 0xf3 + k * 8) = 1.0f;
    }
    F<int32_t>(v, 0x113) = 0;
    F<int32_t>(v, 0x117) = 0;
    F<int32_t>(v, 0x11b) = 0;
}
RT3_REPLACE(0x477af0, MapView_Init);

// 0x477c10. Converts world (x, y) to the minimap's pixel position: zoomed views
// (+0x33 & 0x2030000) map the view rect (+0xb/+0xf origin plus the scroll at
// +0x86 +5/+9, size +0x13/+0x17) onto pixels +0xc3..+0xcb / +0xc7..+0xcf (y flipped);
// otherwise the whole map, letterboxed to its larger side and flipped in y.
RT3_NOINLINE void __fastcall Minimap_WorldToPixel(uint8_t* o, int, float x, float y, float* outX, float* outY) {
    if (F<uint32_t>(o, 0x33) & 0x2030000) {
        uint8_t* s = F<uint8_t*>(o, 0x86);
        float w = (float)(F<int32_t>(o, 0xcb) - F<int32_t>(o, 0xc3));
        *outX = (float)(((double)x - ((double)F<float>(s, 5) + F<float>(o, 0xb))) / F<float>(o, 0x13) * w +
                        (float)F<int32_t>(o, 0xc3));
        *outY = (float)((D(0x5c8770) - ((double)y - ((double)F<float>(s, 9) + F<float>(o, 0xf))) / F<float>(o, 0x17)) *
                            (double)(F<int32_t>(o, 0xcf) - F<int32_t>(o, 0xc7)) +
                        (double)F<int32_t>(o, 0xc7));
        return;
    }
    uint8_t* m = g_map;
    int32_t w = F<int32_t>(m, 0x2155), h = F<int32_t>(m, 0x2159);
    int32_t big = w > h ? w : h;
    float size = (float)big;
    *outX = (float)(((double)x - F<float>(o, 0xb)) / F<float>(o, 0x13) * size);
    float t = (float)(((double)y - F<float>(o, 0xf)) / F<float>(o, 0x17) * size);
    *outY = t;
    if (w > h) *outY = (float)((double)t - (double)(big - h) * D(0x5c8628));
    else *outX = (float)((double)*outX - (double)(big - w) * D(0x5c8628));
    *outY = (float)((double)(float)h - *outY);
}
RT3_REPLACE(0x477c10, Minimap_WorldToPixel);

// 0x477d80. Minimap `m` (box +0xb..+0x17, the map 0x62c120 fitted keeping its
// aspect): the camera's view on the map: the scene's four ground corners
// (cells of 92 units) joined by a line along the near edge and a translucent
// fill, cut back towards the near edge when the far edge reaches more than
// 0.12 away, and a cross at the camera target.
RT3_NOINLINE void __fastcall MiniMap_DrawView(uint8_t* m) {
    Fade_Step_Impl();
    Fade_Save();
    for (int k = 0; k < 20; ++k) Fade_Step_Impl();
    float far4[4], near4[4], mid4[4];
    Scene_GetVectors((uint8_t*)Engine_Renderer(g_engine), 0, (uint32_t*)far4, (uint32_t*)mid4, (uint32_t*)near4);
    float mx = F<float>(m, 0xb), my = F<float>(m, 0xf), mw = F<float>(m, 0x13), mh = F<float>(m, 0x17);
    int32_t W = F<int32_t>(g_map, 0x2155), H = F<int32_t>(g_map, 0x2159);
    float Wf, Hf;
    if (W < H) {
        Wf = (float)W;
        Hf = (float)H;
        mw = (float)((double)Wf / Hf * mw);
        mx = (float)(((double)F<float>(m, 0x13) - mw) / D(0x5c85d0) + mx);
    } else {
        Hf = (float)H;
        Wf = (float)W;
        mh = (float)((double)Hf / Wf * mh);
        my = (float)(((double)F<float>(m, 0x17) - mh) / D(0x5c85d0) + my);
    }
    float X[4], Y[4];
    for (int k = 0; k < 4; ++k) {
        float u = (float)((double)far4[k] / Fl(0x5c8700));
        float v = (float)((double)Hf - (double)near4[k] / Fl(0x5c8700));
        X[k] = (float)((double)u / Wf * mw + mx);
        Y[k] = (float)((double)v / Hf * mh + my);
    }
    float d1 = (float)Distance2D(X[2], Y[2], X[0], Y[0]);
    float d2 = (float)Distance2D(X[3], Y[3], X[1], Y[1]);
    Ui_Line(0x9fffffbf, 1, X[0], Y[0], X[1], Y[1], 2.0f);
    auto bits = [](float f) { return *(uint32_t*)&f; };
    if (!(d1 > D(0x5c8590)) && !(d2 > D(0x5c8590))) {
        Engine_QueueGradientQuad(0x64ffffcf, 0x64ffffcf, bits(X[0]), bits(Y[0]), bits(X[1]), bits(Y[1]), bits(X[3]), bits(Y[3]), bits(X[2]), bits(Y[2]),
                                 0xfffff8f, 0xfffff8f, 0, 0, 0, 0x3f800000, 0x3f800000);
    } else {
        float s1 = (float)((double)d1 / D(0x5c8590));
        if (s1 < D(0x5c9b10)) s1 = BitsF(0x3f8147ae);
        float pa = (float)(((double)X[2] - X[0]) / s1 + X[0]);
        float pb = (float)(((double)Y[2] - Y[0]) / s1 + Y[0]);
        float s2 = (float)((double)d2 / D(0x5c8590));
        if (s2 < D(0x5c9b10)) s2 = BitsF(0x3f8147ae);
        float pc = (float)(((double)X[3] - X[1]) / s2 + X[1]);
        float pd = (float)(((double)Y[3] - Y[1]) / s2 + Y[1]);
        Engine_QueueGradientQuad(0x64ffffcf, 0x64ffffcf, bits(X[0]), bits(Y[0]), bits(X[1]), bits(Y[1]), bits(pc), bits(pd), bits(pa), bits(pb), 0xfffff8f,
                                 0xfffff8f, 0, 0, 0, 0x3f800000, 0x3f800000);
        Engine_QueueGradientQuad(0xfffff8f, 0xfffff8f, bits(pa), bits(pb), bits(pc), bits(pd), bits(X[3]), bits(Y[3]), bits(X[2]), bits(Y[2]), 0xfffff8f,
                                 0xfffff8f, 0, 0, 0, 0x3f800000, 0x3f800000);
    }
    float cx, cz;
    Obj_CellPos(g_62be68, 0, &cx, &cz);
    float czf = (float)((double)Hf - cz);
    float px, py;
    if (W < H) {
        px = (float)(((double)(H - W) * D(0x5c8628) + cx) / (double)H * F<float>(m, 0x13) + F<float>(m, 0xb));
        py = (float)((double)czf / Hf * F<float>(m, 0x17) + F<float>(m, 0xf));
    } else {
        px = (float)((double)cx / Wf * F<float>(m, 0x13) + F<float>(m, 0xb));
        py = (float)(((double)(W - H) * D(0x5c8628) + czf) / (double)W * F<float>(m, 0x17) + F<float>(m, 0xf));
    }
    Ui_Line(0xcfffffbf, 1, (float)((double)px - Fl(0x5ce838)), py, (float)((double)px + Fl(0x5ce838)), py, 1.0f);
    Ui_Line(0xcfffffbf, 1, px, (float)((double)py - Fl(0x5ce834)), px, (float)((double)py + Fl(0x5ce834)), 1.0f);
    Fade_Restore();
}
RT3_REPLACE(0x477d80, MiniMap_DrawView);

}  // namespace rt3
