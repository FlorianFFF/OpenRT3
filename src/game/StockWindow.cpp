// StockWindow.cpp - RT3.exe 0x50a000-0x50c000.
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
#include "hook/keepregs.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "game/calendar.h"
#include "core/crt.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

void __fastcall String_Assign(char** s, const char* src);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t __fastcall Table_CountField4(Collection* t);
void __fastcall FreeAndNull(void** p);
void* __fastcall Obj_Ref25(void* o);
int32_t __fastcall Table_CountField3f(Collection* t);
void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
int32_t __fastcall Obj_NameIndex(void* o);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
void __fastcall Widget_SetVtbl(uint8_t* w);
double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
void __fastcall Reset_625a94();
double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
void* __fastcall Table_NthField4(Collection* t, int, int32_t n);

void __fastcall TextLink_DrawPlain(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align,
                                   int32_t valign, uint32_t flags, const char* tip, int32_t id);

double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
int32_t __fastcall Collection_Count(Collection* c);
int32_t __fastcall Obj_Get21(void* o);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
int32_t __fastcall Table_LiveRankOf(Collection* t, int, uint32_t id);
uint32_t __fastcall Table_NthIdField3f(Collection* t, int, int32_t n);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
int32_t IsMultiplayer();
void __fastcall Net_SendToHost(uint8_t* s, int, uint32_t type, int32_t size, const uint8_t* data);
int32_t __fastcall Player_CanBuyShares(uint8_t* p, int, uint32_t company, int32_t margin, int32_t n);
uint32_t __fastcall Company_Logo(uint8_t* c);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

uint32_t IsEnglish();
double __fastcall Company_FloatD7b(void* c);
void* __fastcall Company_Owner(void* c);

void __fastcall TextLink_Draw(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign, uint32_t flags,
                              const char* tip, int32_t id, uint32_t color, uint32_t hover);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

void __fastcall Obj_SetPlayer(void* o, int, uint32_t player);
int32_t __fastcall BuildingDialog_Retrack(uint8_t* dlg);
RT3_NOINLINE void* __fastcall Company_Owner(void* c);
RT3_NOINLINE void* __fastcall Table_NthField4(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t __fastcall Table_CountField3f(Collection* t);
RT3_NOINLINE void* __fastcall Table_NthField3f(Collection* t, int, int32_t n);
RT3_NOINLINE int32_t IsMultiplayer();
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
void __fastcall CompanyStock_DrawChart(uint8_t* win, uint32_t linkBase, float x, float y, float, float, int32_t compact, uint32_t coId, uint8_t* faces, uint8_t** custom, int32_t, const uint8_t* show);

// Event callbacks: in a scenario (0x6d3b4c), releasing button `id` shows
// string 0xe8c (returns 0); otherwise 1.
#define RT3_SCENARIO_LOCK(addr, name, id) \
    int32_t __fastcall name(void*, const uint32_t* ev) { \
        if (ev[0] != 0xcb || ev[1] != id || !g_6d3b4c) return 1; \
        Message_Dialog(GetString(0xe8c), 0x3ef5c28f); \
        return 0; \
    } \
    RT3_REPLACE(addr, name);

int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
void __fastcall CompanyList_Select(uint8_t* w, int, int32_t idx);
int32_t __fastcall BuildingDialog_TrackMouse(uint8_t* dlg, int, uint32_t* ev);
void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force);
void Dialog_Clear3b60IfOpen();
int32_t __fastcall Player_CanSellShares(uint8_t* p, int, uint32_t company, int32_t shortOk, int32_t n, int32_t flag);
void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t);
uint32_t __fastcall Table_NthActive(Collection* t, int, int32_t n);
void* __fastcall Collection_Get(Collection* c, int, uint32_t id);

// Functions of this file, in exe order.
RT3_NOINLINE double __fastcall Player_HoldingsValue(uint8_t* p, int, int32_t stored);
RT3_NOINLINE void __fastcall Player_SellShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t, int32_t);
RT3_NOINLINE double __fastcall Player_NetWorth(uint8_t* p, int, int32_t stored);
RT3_NOINLINE double __fastcall Player_CollateralWorth(uint8_t* p, int, int32_t stored);
RT3_NOINLINE int32_t __fastcall Player_CanSellShares(uint8_t* p, int, uint32_t company, int32_t shortOk, int32_t n, int32_t flag);
RT3_NOINLINE int32_t __fastcall Player_CanBuyShares(uint8_t* p, int, uint32_t company, int32_t margin, int32_t n);
RT3_NOINLINE void __fastcall Player_BuyShares(uint8_t* p, int, uint32_t company, int32_t n, int32_t margin, int32_t force);
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE int32_t __fastcall BuildingDialog_TrackMouse(uint8_t* dlg, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall BuildingDialog_Retrack(uint8_t* dlg);
void __fastcall Stock_DrawChartCb(void*, int, float x, float y, float w, float h, int32_t);
void Dialog_Clear3b60();
RT3_NOINLINE void __stdcall SetGlobal6d19f4(int32_t v);
void CompanyList_ClearRows();
RT3_NOINLINE void Dialog_Clear3b60IfOpen();
RT3_NOINLINE void __fastcall CompanyList_Select(uint8_t* w, int, int32_t idx);
RT3_NOINLINE void __fastcall PlayerFinance_Draw(void*, int, float x, float y, float w, float, int32_t);
void __fastcall StockList_DrawRow(void*, uint32_t id, float x, float y, int32_t, float h, int32_t);
void __fastcall Stock_CompanyTooltipCb(void*, int, int32_t, int32_t);
void __fastcall CompanyList_Refill(uint8_t* w);
void __fastcall StockBuy_Destruct(uint8_t* w);
int32_t __fastcall StockWindow_HandleEvent(uint8_t* d, int, uint32_t* msg);
RT3_NOINLINE char* __fastcall Num_FormatSimple(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE char* __fastcall Num_FormatSimple2(char* buf, int32_t style, double v, int32_t width);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
RT3_NOINLINE uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall TextLink_DrawPlain(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, const char* tip, int32_t id);

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }


static inline float BitsF(uint32_t b) { return *(float*)&b; }

namespace {
uint32_t Message_Dialog(const char* text, uint32_t px) {
    return Dialog_RunMessage(text, 1, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

namespace {
uint32_t Message494(uint32_t text, int32_t type, uint32_t px, int32_t p4) {
    return Dialog_RunMessage(GetString(text), type, px, 0xbf800000, p4, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
// Shares per click: 1000, with Shift / Ctrl (flags 3) up to 5000, with Alt
// (flag 4) up to 25000 of the company's lot (by position, 1000 beyond 32).
int32_t Lot494(uint32_t table, int32_t company, uint32_t flags) {
    const int32_t lot = company < 0x20 ? *(const int32_t*)(table + company * 8) : 0x3e8;
    if (flags & 3) return lot < 0x1388 ? lot : 0x1388;
    if (flags & 4) return lot < 0x61a8 ? lot : 0x61a8;
    return 0x3e8;
}
// A trade request to the host: the player record and the company position.
void SendTrade494(uint32_t type, uint8_t* p, int32_t company) {
    uint32_t req[2];
    req[0] = F<uint32_t>(p, 0);
    Cursor_Set(g_cursorManager, 0, 6, 0, 0);
    req[1] = (uint32_t)company;
    Net_SendToHost(g_session, 0, type, 8, (const uint8_t*)req);
}
}

// 0x50a530. Building dialog dlg: unless a placement is pending (+0x94),
// re-tracks the mouse when the player has a company (or with option +0x68).
// Returns what it last computed (callers may pass it on).
RT3_NOINLINE int32_t __fastcall BuildingDialog_Retrack(uint8_t* dlg) {
    if (int32_t p = F<int32_t>(dlg, 0x94)) return p;
    if (!Collection_IsValid(g_companies, 0, (uint32_t)Obj_Get21(hook::As<void*>(g_world))) &&
        !F<int32_t>(g_app, 0x68))
        return (int32_t)(uintptr_t)g_app;
    return BuildingDialog_TrackMouse(dlg, 0, nullptr);
}
RT3_REPLACE(0x50a530, BuildingDialog_Retrack);

// 0x50a570 (draw callback). Draws the stock chart of the company at index
// 0x622b00 (among those with +0x3f) into 0x6d19f0.
void __fastcall Stock_DrawChartCb(void*, int, float x, float y, float w, float h, int32_t) {
    const int32_t idx = g_622b00;
    if (idx < 0 || idx >= Table_CountField3f(g_companies)) return;
    const int32_t a = hook::As<int32_t>(g_6d19dc), b = g_6d19e0;
    const uint32_t co = Table_NthIdField3f(g_companies, 0, g_622b00);
    CompanyStock_DrawChart(g_stockWindow, 0x3e4e, x, y, w, h, 0, co, (uint8_t*)(uintptr_t)a, (uint8_t**)0x6d1748, b,
                           (const uint8_t*)0x622b04);
}
RT3_REPLACE(0x50a570, Stock_DrawChartCb);

void Dialog_Clear3b60() {
    Reset_625a94();
    Window_DeleteChildren(g_stockWindow, 0, 0x3b60, 0x3e7f, 0);
}
RT3_REPLACE(0x50a5e0, Dialog_Clear3b60);

RT3_SCENARIO_LOCK(0x50a600, Scenario_LockCb3f4b, 0x3f4)

RT3_NOINLINE void __stdcall SetGlobal6d19f4(int32_t v) { g_6d19f4 = v; }
RT3_REPLACE(0x50a660, SetGlobal6d19f4);

// 0x50a670. Resets 0x625a94 and clears the generated rows 0x3b60..0x3e7f of the
// company list window 0x6d19f0.
void CompanyList_ClearRows() {
    Reset_625a94();
    if (uint8_t* w = g_stockWindow) Window_DeleteChildren(w, 0, 0x3b60, 0x3e7f, 0);
}
RT3_REPLACE(0x50a670, CompanyList_ClearRows);

RT3_NOINLINE void Dialog_Clear3b60IfOpen() {
    Reset_625a94();
    if (g_stockWindow) Window_DeleteChildren(g_stockWindow, 0, 0x3b60, 0x3e7f, 0);
}
RT3_REPLACE(0x50a6a0, Dialog_Clear3b60IfOpen);

// 0x50a6d0. Selects company row `idx` (0x622b00; -1 past the active companies),
// enabling (0x6d) or disabling (0x86) control 0x3a9b, and clears the generated rows.
RT3_NOINLINE void __fastcall CompanyList_Select(uint8_t* w, int, int32_t idx) {
    if (idx >= Table_CountField3f(g_companies)) idx = -1;
    g_622b00 = idx;
    Window_SendEvent5(w, 0, idx != -1 ? 0x6d : 0x86, 0x3a9b, 3, 0, 0);
    Window_DeleteChildren(w, 0, 0x3b60, 0x3e7f, 0);
}
RT3_REPLACE(0x50a6d0, CompanyList_Select);

// 0x50a730. The players panel of the finance window: a row of player faces
// (scaled down past 5 players; the selected player 0x6d19f4 framed by image
// 0x4c; custom faces from 0x6d1748), each with a hover cell 0x3afc + i (tooltip
// 0x277 with the name, plus 0x278 for the local player or 0xc08 for others
// when option +0x68 is set), then the selected player's cash, holdings, net
// worth and collateral (labels 0x279.., negative values red, a rule under the
// holdings).
RT3_NOINLINE void __fastcall PlayerFinance_Draw(void*, int, float x, float y, float w, float, int32_t) {
    Collection* players = g_players;
    int32_t n = Table_CountField4(players);
    float scale = BitsF(0x3f333333), frame = 1.0f;
    if (n > 5) {
        double q = (double)n / D(0x5c8d68);
        frame = (float)(D(0x5c8770) / q);
        scale = (float)(D(0x5d18d8) / q);
    }
    float cellW = (float)((double)scale * D(0x5c9ba0));
    double wd = (double)w;
    float margin = (float)(wd * D(0x5c85b8));
    int32_t cnt = n < 1 ? 1 : n;
    float slotW = (float)(((double)w - ((double)margin + margin)) / (float)cnt);
    if (n > 0) {
        float faceY = (float)((double)y + Fl(0x5d18d0));
        float base = (float)((double)margin + x);
        double half = (double)cellW * D(0x5c8628);
        for (int32_t i = 0; i < n; ++i) {
            uint8_t* pl = (uint8_t*)Table_NthField4(players, 0, i);
            double mid = ((double)i + D(0x5c8628)) * slotW;
            if (i == g_6d19f4) {
                float t1 = (float)((double)frame * D(0x5d18c8));
                float t2 = (float)((double)frame * D(0x5d18c0));
                float gy = (float)((double)faceY + t2);
                float gx = (float)(mid + ((double)base + t1) - half);
                Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(g_6d19d8, 0, 0x4c), 0, gx, gy, 0x3f800000, frame, frame, 0xffffffff, 1, 1,
                            0, 0.0f, 0.0f, 1.0f, 1.0f);
            }
            uint8_t* table = g_6d19dc;
            int32_t idx = 0;
            float gx = (float)((double)base + mid - half);
            if (pl[0x15c] != 0x29) idx = pl[0x15c];
            else if (Obj_NameIndex(pl) != -1) table = hook::Global<uint8_t*>(0x6d1748 + Obj_NameIndex(pl) * 4);
            else pl[0x15c] = 0;
            Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(table, 0, idx), 0, gx, faceY, 0x3f800000, scale, scale, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
            if (Widget_FindChild(g_stockWindow, 0, i + 0x3afc)) continue;
            uint8_t def[0x8b];
            memset(def, 0, sizeof def);
            F<float>(def, 7) = (float)(((double)slotW - cellW) * D(0x5ca180) + margin + x + mid - half);
            F<uint16_t>(def, 0) = (uint16_t)(i + 0x3afc);
            F<float>(def, 0xf) = slotW;
            F<uint32_t>(def, 0x13) = 0x3d75c28f;
            def[0x29] = 0x20;
            F<float>(def, 0xb) = (float)((double)y + D(0x5d18b8) - D(0x5d0250));
            char text[0x200];
            FormatText(text, GetString(0x277), (const char*)(pl + 8), 0, 0, 0, 0, 0, 0, 0);
            if (pl == Obj_Ref25(g_world)) strcat(text, GetString(0x278));
            if (F<int32_t>(g_app, 0x68) && pl != Obj_Ref25(g_world)) strcat(text, GetString(0xc08));
            String_Assign(&F<char*>(def, 0x3f), text);
            uint8_t* tw = (uint8_t*)crt::operator_new(0xda);
            tw = tw ? TimerWidget_Construct(tw, 0, def) : nullptr;
            Window_AddChild(g_stockWindow, 0, tw, 0);
            FreeAndNull(&F<void*>(def, 0x3f));
        }
    }
    float rowY = (float)((double)y + Fl(0x5d18b0) + D(0x5d18a8));
    static const int32_t labels[4] = { 0x279, 0x27a, 0x27b, 0x27c }, tips[4] = { 0x27d, 0x27e, 0x27f, 0x280 };
    float valueW = (float)(wd - D(0x5d0288));
    uint8_t* font = g_6d19e8;
    char buf[0x200];
    float v = 0.0f;
    for (int32_t k = 0; k < 4; ++k) {
        double hm = k == 3 ? 2.25 : 1.0;
        float h = (float)(hm * D(0x5d0bf8));
        Font_DrawTextBox(font, 0, GetString(labels[k]), x, rowY, BitsF(0x3dc8b439), h, 0xffffffff, 3, 0, 0, 1.0f);
        uint8_t* sel = (uint8_t*)Table_NthField4(players, 0, g_6d19f4);
        switch (k) {
        case 0: v = (float)F<double>(sel, 0x154); break;
        case 1: v = (float)Player_HoldingsValue(sel, 0, 1); break;
        case 2: v = (float)Player_NetWorth(sel, 0, 1); break;
        case 3: v = (float)Player_CollateralWorth(sel, 0, 1); break;
        }
        if (v >= Fl(0x5d18a0)) Num_FormatSimple(buf, 0xc, (double)v, 0xb);
        else Num_FormatSimple2(buf, 0xb, (double)v, 0xb);
        uint32_t color = v >= Fl(0x5c8598) ? 0xffffffff : 0xffff5f5f;
        const char* tip = GetString(tips[k]);
        float h2 = (float)(hm * D(0x5d0bf8));
        TextLink_DrawPlain(g_stockWindow, font, buf, x, rowY, valueW, h2, color, 3, 0, 0x20, tip, k + 0x3e1c);
        if (k == 1) {
            float ly = (float)((double)rowY + Fl(0x5d12c8));
            float right = (float)((double)x + w);
            float x2 = (float)((double)right - D(0x5d0830));
            float x1 = (float)((double)right - D(0x5d1898));
            Ui_Line(0xffffffff, 0, x1, ly, x2, ly, 2.0f);
            rowY = (float)((double)rowY + D(0x5d1890));
        } else if (k == 0)
            rowY = (float)((double)rowY + Fl(0x5d12c8));
        else
            rowY = (float)((double)rowY + D(0x5d1888));
    }
}
RT3_REPLACE(0x50a730, PlayerFinance_Draw);

// 0x50ae50. Stock market list (window 0x6d19f0): the row of company id at
// (x, y), height h, for player 0x6d19f4: the logo, then six columns (offsets
// by language 0x6d3c84): share price (0xad3, with the owner line 0xe0d mine
// / 0xe0e / 0xe0f none), p's holding (0xad4; millions as "%3.0f" / "%3.1f"
// + 0x282), its value (0xad5) - each with a tooltip widget (id (column +
// 0x98) x 100 + company) - and for the local player the buy (0x287 or "+"
// in language 4; on margin 0x285) and sell (0x289; short 0x28b) links with
// their block size (1000, 5000 or 25000 when affordable, single player
// only) kept in 0x6d18d8 / 0x6d18dc, and the select link (0x28d / 0x28f).
void __fastcall StockList_DrawRow(void*, uint32_t id, float x, float y, int32_t, float h, int32_t) {
    uint8_t* co = (uint8_t*)Collection_Get(g_companies, 0, id);
    uint8_t* p = (uint8_t*)Table_NthField4(g_players, 0, g_6d19f4);
    float gy = (float)((double)y + D(0x5d0250));
    float gx = (float)((double)x + D(0x5c9bc0));
    Glyph_Queue((uint8_t*)(uintptr_t)Company_Logo(co), 0, gx, gy, 0x3f800000, BitsF(0x3f0ccccd), BitsF(0x3f0ccccd), 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f,
                1.0f);
    uint8_t* font = IsEnglish() & 0xff ? g_6d19e8 : g_6d19ec;
    uint32_t col[7] = { 0, 0x3d9ba5e3, 0x3df9db23, 0x3e4ac083, 0x3e77ced9, 0x3e94fdf4, 0x3eaf9db2 };
    int32_t lang = g_language;
    if (lang == 0xc) {
        static const uint32_t c12[7] = { 0xba83126f, 0x3d978d50, 0x3df3b646, 0x3e439581, 0x3e800000, 0x3e9db22d, 0x3eb1a9fc };
        memcpy(col, c12, sizeof col);
    } else if (lang == 2) {
        col[6] = 0x3eb1a9fc, col[3] = 0x3e45a1cb, col[2] = 0x3df3b646, col[1] = 0x3d978d50;
    } else if (lang == 3) {
        col[6] = 0x3eb22d0e, col[5] = 0x3e93f7cf, col[4] = 0x3e7df3b6;
    }
    auto C = [&](int32_t k) { return (double)BitsF(col[k]); };
    auto fmt = [](char* dst, int32_t s, const char* a) { FormatText(dst, GetString(s), a, 0, 0, 0, 0, 0, 0, 0); };
    for (int32_t k = 0; k < 6; ++k) {
        char text[0x258], num[0x258];
        if (k < 3) {
            if (k == 0) {
                Num_FormatSimple(num, 9, Company_FloatD7b(co), 0xb);
                fmt(text, 0xad3, (const char*)co + 4);
                char line[0x1ec];
                uint32_t owner = F<uint32_t>(co, 0x3b);
                if (owner && owner == (uint32_t)Obj_Get21(g_world)) fmt(line, 0xe0d, (const char*)Company_Owner(co) + 8);
                else if (F<uint32_t>(co, 0x3b)) fmt(line, 0xe0e, (const char*)Company_Owner(co) + 8);
                else fmt(line, 0xe0f, nullptr);
                strcat(text, line);
            } else if (k == 1) {
                float f = (float)F<int32_t>(p, 0x15d + id * 4);
                if (!(crt::fabs((double)f) < D(0x5d18e8))) {
                    char m[0x20];
                    crt::sprintf(m, (const char*)0x5d0c48, (double)f / Fl(0x5c8d10));
                    FormatText(num, (const char*)0x5c8730, m, GetString(0x282), 0, 0, 0, 0, 0, 0);
                } else if (!(crt::fabs((double)f) < D(0x5d18e0))) {
                    char m[0x20];
                    crt::sprintf(m, (const char*)0x5ca2ac, (double)f / Fl(0x5c8d10));
                    FormatText(num, (const char*)0x5c8730, m, GetString(0x282), 0, 0, 0, 0, 0, 0);
                } else
                    Num_Format(num, 2, (double)f, 10, 1, 0, 0);
                fmt(text, 0xad4, (const char*)co + 4);
            } else {
                int32_t held = F<int32_t>(p, 0x15d + id * 4);
                double v = Company_FloatD7b(co);
                float val = (float)(v * (double)(float)held);
                Num_FormatSimple(num, val < Fl(0x5d18a0) ? 0xb : 0xc, (double)val, 0xb);
                fmt(text, 0xad5, (const char*)co + 4);
            }
            float w = (float)((C(k + 1) - C(k)) - D(0x5d0250));
            float cx = (float)((double)x + C(k));
            Font_DrawTextBox(font, 0, num, cx, y, w, h, 0xffffffff, 3, 0, 0, 1.0f);
            int32_t wid = (k + 0x98) * 0x64 + (int32_t)id;
            if (Widget_FindChild(g_stockWindow, 0, wid)) continue;
            uint8_t def[0x8b];
            memset(def, 0, sizeof def);
            F<float>(def, 0xb) = y;
            F<float>(def, 0x13) = h;
            F<float>(def, 7) = cx;
            F<float>(def, 0xf) = w;
            F<uint16_t>(def, 0) = (uint16_t)wid;
            String_Assign(&F<char*>(def, 0x3f), text);
            uint8_t* tw = (uint8_t*)crt::operator_new(0xda);
            tw = tw ? TimerWidget_Construct(tw, 0, def) : nullptr;
            Window_AddChild(g_stockWindow, 0, tw, 0);
            crt::free(F<void*>(def, 0x3f));
            continue;
        }
        if (k == 3) {
            if (Obj_Ref25(g_world) != p) continue;
            int32_t margin = 0;
            if (!Player_CanBuyShares(p, 0, id, 0, 1000)) {
                margin = 1;
                if (!Player_CanBuyShares(p, 0, id, 1, 1000)) continue;
                strcpy(num, GetString(0x285));
                fmt(text, 0xf16, (const char*)co + 4);
            } else {
                if (g_language == 4) F<uint16_t>(num, 0) = 0x2b;
                else strcpy(num, GetString(0x287));
                fmt(text, 0xf17, (const char*)co + 4);
            }
            int32_t amount = 1000;
            if (!IsMultiplayer()) {
                if (Player_CanBuyShares(p, 0, id, margin, 25000)) {
                    amount = 25000;
                    strcat(text, GetString(0xf33));
                } else if (Player_CanBuyShares(p, 0, id, margin, 5000)) {
                    amount = 5000;
                    strcat(text, GetString(0xf34));
                }
            }
            if (F<int32_t>(co, 0) < 0x20) *(int32_t*)(0x6d18d8 + F<int32_t>(co, 0) * 8) = amount;
        } else if (k == 4) {
            if (Obj_Ref25(g_world) != p) continue;
            int32_t amount = 1000;
            if (F<int32_t>(p, 0x15d + id * 4) > 0) {
                strcpy(num, GetString(0x289));
                fmt(text, 0xf18, (const char*)co + 4);
                if (!IsMultiplayer()) {
                    int32_t held = F<int32_t>(p, 0x15d + id * 4);
                    if (held > 25000) {
                        amount = 25000;
                        strcat(text, GetString(0xf35));
                    } else if (held > 5000) {
                        amount = 5000;
                        strcat(text, GetString(0xf36));
                    }
                }
            } else {
                if (!Player_CanSellShares(p, 0, id, 1, 1000, 0)) continue;
                strcpy(num, GetString(0x28b));
                fmt(text, 0xf19, (const char*)co + 4);
                if (!IsMultiplayer()) {
                    if (Player_CanSellShares(p, 0, id, 1, 25000, 0)) {
                        amount = 25000;
                        strcat(text, GetString(0xf37));
                    } else if (Player_CanSellShares(p, 0, id, 1, 5000, 0)) {
                        amount = 5000;
                        strcat(text, GetString(0xf38));
                    }
                }
            }
            if (F<int32_t>(co, 0) < 0x20) *(int32_t*)(0x6d18dc + F<int32_t>(co, 0) * 8) = amount;
        } else {
            int32_t sel = g_622b00;
            if (sel != -1 && Table_NthIdField3f(g_companies, 0, sel) == id) {
                strcpy(num, GetString(0x28d));
                fmt(text, 0x28e, nullptr);
            } else {
                strcpy(num, GetString(0x28f));
                fmt(text, 0x290, (const char*)co + 4);
            }
        }
        float w = (float)((C(k + 1) - C(k)) - D(0x5d0250));
        TextLink_Draw(g_stockWindow, font, num, (float)((double)x + C(k)), y, w, h, 3, 0, 1, text, (k + 0x98) * 0x64 + (int32_t)id, 0, 0);
    }
}
RT3_REPLACE(0x50ae50, StockList_DrawRow);

// 0x50b910 (hover callback). Tooltip for the company at stock index 0x622b00:
// its name with the owner (0xe10 the player's, 0xe11 another's) or 0xe12
// without one.
void __fastcall Stock_CompanyTooltipCb(void*, int, int32_t, int32_t) {
    const int32_t idx = g_622b00;
    if (idx < 0 || idx >= Table_CountField3f(g_companies)) return;
    uint8_t* c = (uint8_t*)Table_NthField3f(g_companies, 0, g_622b00);
    if (!c) return;
    int32_t id;
    const char* owner = nullptr;
    if (F<int32_t>(c, 0x3b) == Obj_Get21(g_world)) {
        owner = (const char*)Company_Owner(c) + 8;
        id = 0xe10;
    } else if (F<int32_t>(c, 0x3b)) {
        owner = (const char*)Company_Owner(c) + 8;
        id = 0xe11;
    } else {
        id = 0xe12;
    }
    char text[0x1f4];
    FormatText(text, GetString(id), (const char*)(c + 4), owner, 0, 0, 0, 0, 0, 0);
    Ui_ShowTooltip(g_windowManager, 0, text, 1, 1, -1);
}
RT3_REPLACE(0x50b910, Stock_CompanyTooltipCb);

// 0x50ba00. Refills the company list 0x3a9a with the active companies (data their
// id), re-selecting 0x622b00, with callbacks 0x50ae50 / 0x50a5e0.
void __fastcall CompanyList_Refill(uint8_t* w) {
    CompanyList_Select(w, 0, g_622b00);
    Window_DeleteChildren(w, 0, 0x3b60, 0x3e7f, 0);
    Window_SendEvent5(w, 0, 0x8b, 0x3a9a, 0, 0, 0);
    for (int32_t i = 0; i < Table_CountField3f(g_companies); ++i) {
        uint8_t* c = (uint8_t*)Table_NthField3f(g_companies, 0, i);
        Window_SendEvent5(w, 0, 0x88, 0x3a9a, 0, F<uint32_t>(c, 0), 0);
    }
    Window_SendEvent5(w, 0, 0x7a, 0x3a9a, 0x50ae50, 0, 0);
    Window_SendEvent5(w, 0, 0x83, 0x3a9a, 0x50a5e0, 0, 0);
}
RT3_REPLACE(0x50ba00, CompanyList_Refill);

// 0x50bac0. Stock buying window ("StockBuy.win") destructor: releases its six
// images (0x6d19d8..ec), the image of every entry of the pack's portrait table
// (+0x429b4, record +0x20) and its company / player buttons in 0x6d19f0.
void __fastcall StockBuy_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d18f0;
    for (uint32_t g : { 0x6d19e0u, 0x6d19dcu, 0x6d19e4u, 0x6d19e8u, 0x6d19ecu, 0x6d19d8u })
        Pack_Release(g_pack, 0, hook::Global<void*>(g));
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(g_pack, 0x429b4)); ++i) {
        Collection* c = F<Collection*>(g_pack, 0x429b4);
        uint8_t* rec = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        Pack_Release(g_pack, 0, rec + 0x20);
    }
    Window_DeleteChildren(g_stockWindow, 0, 0x3b60, 0x3e7f, 0);
    Window_DeleteChildren(g_stockWindow, 0, 0x3e4e, 0x3e7f, 0);
    g_stockWindow = nullptr;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x50bac0, StockBuy_Destruct);

 // namespace

// 0x50bbe0 (event handler). The stock market window d for player 0x6d19f4
// (by position): buy (0x3c8c.. by company position; on margin after the
// one-time warning 0x292, its own picker 0x50a600) or sell (0x3cf0..; short
// after the warning 0x293) a lot of shares (sound 0x36 / 0x37; in
// multiplayer a request 0x1d / 0x1f to the host), the company rows
// 0x3d54.. select / unselect a company (0x622b00), the page buttons 0x3aa1
// .. step it, the player buttons 0x3afc.. pick the player (in the editor
// with tool flag 4 also becoming them), the check boxes 0x3aad..0x3ab2
// (0x61f057 + id). In a tutorial (0x6d3b4c) only the awaited purchase
// (0x622b38, string 0xe03) and no sales (0xe8c) work.
int32_t __fastcall StockWindow_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    uint8_t* p = (uint8_t*)Table_NthField4(g_players, 0, g_6d19f4);
    const int32_t id = (int32_t)msg[1];
    if (msg[0] == 0xcb) {
        if (id == 0x3aa1) {
            CompanyList_Select(d, 0, -1);
            return 0;
        }
        if (id <= 0x3aa1 || id > 0x3aa3) return 1;
        const int32_t pages = Table_CountField3f(g_companies) > 1 ? Table_CountField3f(g_companies) : 1;
        const int32_t count = Table_CountField3f(g_companies);
        const int32_t step = id != 0x3aa2 ? 1 : -1;
        g_622b00 = (step + g_622b00 + count) % pages;
        return 0;
    }
    if (msg[0] != 0xca) return 1;
    if (id >= 0x3c8c && id <= 0x3cef) {
        const int32_t co = id - 0x3c8c;
        if (g_6d3b4c && (co != Obj_Get21(g_world) || hook::As<int32_t>(g_622b38) != id)) {
            Message494(0xe03, 1, 0x3ef5c28f, 0);
            return 0;
        }
        const int32_t margin = !Player_CanBuyShares(p, 0, (uint32_t)co, 0, 0x3e8) && Player_CanBuyShares(p, 0, (uint32_t)co, 1, 0x3e8);
        if (!g_6d19fc && margin) {
            g_6d19fc = 1;
            if (Message494(0x292, 3, 0xbf800000, 0x50a600) != 0x3f2) return 0;
        }
        if (IsMultiplayer()) {
            SendTrade494(0x1d, p, co);
            return 0;
        }
        const int32_t n = Lot494(0x6d18d8, co, msg[8]);
        Ui_PlaySound(0x36, 0, 0x3f000000);
        Player_BuyShares(p, 0, (uint32_t)co, n, 1, 0);
        Dialog_Clear3b60IfOpen();
        return 0;
    }
    if (id >= 0x3cf0 && id <= 0x3d53) {
        if (g_6d3b4c) {
            Message494(0xe8c, 1, 0x3ef5c28f, 0);
            return 0;
        }
        const int32_t co = id - 0x3cf0;
        const int32_t shortSale = F<int32_t>(p, 0x15d + co * 4) <= 0 && Player_CanSellShares(p, 0, (uint32_t)co, 1, 0x3e8, 0);
        if (!g_6d19f8 && shortSale) {
            g_6d19f8 = 1;
            if (Message494(0x293, 3, 0xbf800000, 0) != 0x3f2) return 0;
        }
        if (IsMultiplayer()) {
            SendTrade494(0x1f, p, co);
            return 0;
        }
        const int32_t n = Lot494(0x6d18dc, co, msg[8]);
        Ui_PlaySound(0x37, 0, 0x3f000000);
        Player_SellShares(p, 0, (uint32_t)co, n, 1, 0);
        Dialog_Clear3b60IfOpen();
        return 0;
    }
    if (id >= 0x3d54 && id <= 0x3db7) {
        const int32_t rank = Table_LiveRankOf(g_companies, 0, (uint32_t)(id - 0x3d54));
        if (g_622b00 != rank && g_622b00 < Table_CountField3f(g_companies)) {
            Ui_PlaySound(0x1a, 0, 0x3f000000);
            CompanyList_Select(d, 0, rank);
            return 0;
        }
        Ui_PlaySound(0x1c, 0, 0x3f000000);
        CompanyList_Select(d, 0, -1);
        return 0;
    }
    if (id >= 0x3afc && id <= 0x3b5f) {
        g_6d19f4 = id - 0x3afc;
        if (F<int32_t>(g_app, 0x68) && (F<uint8_t>(g_input, 0xa8c) & 4)) {
            F<int32_t>(Obj_Ref25(g_world), 0x1e1) = 0;
            Obj_SetPlayer(g_world, 0, Table_NthActive(g_players, 0, g_6d19f4));
            F<int32_t>(Obj_Ref25(g_world), 0x1e1) = 1;
        }
        Reset_625a94();
        Window_DeleteChildren(g_stockWindow, 0, 0x3b60, 0x3e7f, 0);
        return 0;
    }
    if (id >= 0x3aad && id < 0x3ab3) {
        *(uint8_t*)(0x61f057 + id) = (uint8_t)msg[2];
        return 0;
    }
    return id == 0x3a9a ? 0 : 1;
}
RT3_REPLACE(0x50bbe0, StockWindow_HandleEvent);

}  // namespace rt3
