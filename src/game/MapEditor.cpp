// MapEditor.cpp - RT3.exe 0x4f6000-0x4fc000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <math.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/crt.h"
#include "game/calendar.h"
#include "core/ptrlist.h"
#include "core/containers.h"

namespace rt3 {

void __fastcall MapEditor_Stroke(uint8_t* d, int, float x, float y, int32_t first, int32_t erase);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static const int32_t* const kDirX = (const int32_t*)0x624b28;
static const int32_t* const kDirY = (const int32_t*)0x624b48;
void* __fastcall Engine_Get1C(void* e);
uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
double __fastcall Map_GridFloat(void* m, int, int32_t x, int32_t y);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

float __fastcall Grid_Darkness(uint8_t* g, int, int32_t x, int32_t y, int32_t layer);
int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Window_Load(uint8_t* win, int, float x, float y, const char* name);
void __fastcall Scene_PushBusy(uint8_t* s);

void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7,
                                 int32_t a8, int32_t a9);

int32_t __fastcall Screen_IsMode5_2(uint8_t* s);
int32_t __fastcall Widget_IsEnabled(uint8_t* w);
uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
void __fastcall Map_SetWaterPreset(uint8_t* o, int, int32_t idx);
int32_t __fastcall Map_PointCount(void* m);
void __fastcall Editor_PushUndo(uint8_t* o);
void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
int32_t __stdcall Scene_FocusFlagIs1(float* x, float* y);
double __fastcall Grid_Float1605(uint8_t* g, int, int32_t x, int32_t y);
void __fastcall Map_FloodFill(int32_t x, int32_t y, uint8_t* grid);
void __fastcall Terrain_PaintLayers(uint8_t* t, int, int32_t x, int32_t y, float s, int32_t mode, const float* w, const float* p);
void __fastcall MapEditor_ShowControls(uint8_t* w);
void __fastcall Terrain_MarkFlooded(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_MarkDrained(uint8_t* t, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_PaintVertex(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Image_AverageColor(uint8_t* o, int, int32_t x, int32_t y, uint8_t* outR, uint8_t* outG, uint8_t* outB);
void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
void __fastcall MinimapWidget_Refresh(uint8_t* m, int, int32_t haveTex, uint32_t* tex, int32_t srcW, int32_t srcH);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
void __fastcall Map_RaiseAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float amount, uint8_t keepFloor);
void __fastcall Overview_Close(uint8_t* t);
void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
uint8_t __fastcall Grid_Byte1661(uint8_t* g, int, int32_t x, int32_t y);
uint8_t __fastcall Grid_Byte1665(uint8_t* g, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
RT3_NOINLINE int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
void __fastcall Grid_SetByte1661(uint8_t* g, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Grid_SetByte1665(uint8_t* g, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Map_FloodCell(uint8_t* m, int, int32_t x, int32_t y, int32_t);
RT3_NOINLINE uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
RT3_NOINLINE void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Window_BalancePercentages(uint8_t* w);
void __fastcall Editor_UndoPaint(uint8_t* e);
int32_t __fastcall Overlay_ValueAt(uint8_t* o, int, float x, float y);

void __fastcall MapEditor_FillPaint(uint8_t* ed, int, float x, float y, float strength, int32_t colorMode, int32_t tex, uint32_t color,
                                    int32_t select, int32_t flood, uint8_t* mask, int32_t apply);

void __fastcall Map_SetStyle(uint8_t* o, int, int32_t style);
void __fastcall MapEditor_SetTool(uint8_t* w, int, int32_t tool);
void __fastcall MapEditor_ShowPageMode(uint8_t* w, int, int32_t close);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
int32_t __fastcall MapEditor_IsTool4Mode2(uint8_t* e);

void __fastcall Terrain_PaintBrush(uint8_t* t, int, float cx, float cy, float r, float strength, int32_t hard,
                                   int32_t mode, const float* w, const float* p, uint8_t* mask);

void __fastcall MapBrush_PaintFlag10(uint8_t* d, int, float x, float y);
void __fastcall MapBrush_PaintBit3(uint8_t* b, int, float x, float y);
uint8_t __fastcall Grid_CellLow(uint8_t* g, int, int32_t x, int32_t y);
float __fastcall Map_DistanceToOtherKind(uint8_t* m, int, float x, float y);
int32_t __fastcall Map_WaterWithin(uint8_t* m, int, int32_t x, int32_t y, int32_t range);
void __fastcall Map_DrainCell(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Grid_SetCellLow(uint8_t* g, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Editor_BlendTexel(uint8_t* o, int, int32_t x, int32_t y, float alpha, int32_t idx);
void __fastcall Image_SoftenPixel(uint8_t* img, int, int32_t x, int32_t y);

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Map_CellsChangedImpl(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __stdcall Map_CellsChanged(int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE void __fastcall Map_RaiseAround(uint8_t* m, int, int32_t x, int32_t y, int32_t r, float amount, uint8_t keepFloor);
RT3_NOINLINE void __fastcall Map_SetCellHeight(uint8_t* m, int, int32_t x, int32_t y, uint32_t v, int32_t refresh);
RT3_NOINLINE void __fastcall Map_RebuildCoast(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t* tex);
RT3_NOINLINE void __fastcall Overview_Redraw(uint8_t* t, int, void* arg);
RT3_NOINLINE void __fastcall Overview_RedrawRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall Overview_SetMode(uint8_t* t, int, int32_t mode, int32_t cargo, int32_t opt, int32_t highlight, void* rect, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
RT3_NOINLINE void __fastcall Overview_Close(uint8_t* t);
RT3_NOINLINE void __fastcall MinimapWidget_Refresh(uint8_t* m, int, int32_t haveTex, uint32_t* tex, int32_t srcW, int32_t srcH);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE int32_t __fastcall MapEditor_IsTool4Mode2(uint8_t* e);
void __fastcall ColourDialog_SliderCb(uint8_t* w, int32_t v);
void __fastcall MapEditor_SetStrengthCb(void*, int32_t v);
RT3_NOINLINE void __fastcall MapEditor_ShowPageMode(uint8_t* w, int, int32_t close);
RT3_NOINLINE void __fastcall MapEditor_ShowControls(uint8_t* w);
RT3_NOINLINE int32_t __fastcall Overlay_ValueAt(uint8_t* o, int, float x, float y);
void __fastcall PercentDialog_SliderCb(uint8_t* w, int32_t v);
int32_t __fastcall MapEditor_HandleKey(uint8_t* e, int, const uint32_t* ev);
RT3_NOINLINE void __fastcall MapEditor_SetTool(uint8_t* w, int, int32_t tool);
int32_t __fastcall MapEditor_HandleEvent(uint8_t* d, int, uint32_t* msg);
uint8_t* __fastcall MapEditorWindow_Construct(uint8_t* w);
RT3_NOINLINE void __fastcall Editor_BlendTexel(uint8_t* o, int, int32_t x, int32_t y, float alpha, int32_t idx);
RT3_NOINLINE void __fastcall Image_SoftenPixel(uint8_t* img, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Image_AverageColor(uint8_t* o, int, int32_t x, int32_t y, uint8_t* outR, uint8_t* outG, uint8_t* outB);
void __fastcall Map_FloodFill(int32_t x, int32_t y, uint8_t* grid);
RT3_NOINLINE void __fastcall MapEditor_PaintPath(uint8_t* d, int, int32_t cnt, float* xs, float* ys, float rad, float strength, int32_t colorMode, int32_t tex, uint32_t color, int32_t hard, int32_t erase);
RT3_NOINLINE void __fastcall MapEditor_FillPaint(uint8_t* ed, int, float x, float y, float strength, int32_t colorMode, int32_t tex, uint32_t color, int32_t select, int32_t flood, uint8_t* mask, int32_t apply);
RT3_NOINLINE void __fastcall MapBrush_PaintFlag10(uint8_t* d, int, float x, float y);
RT3_NOINLINE void __fastcall MapBrush_PaintBit3(uint8_t* b, int, float x, float y);

static inline double D(uint32_t a) { return *(const double*)rt3::Rdata(a); }

// One blended channel: ftol(src * alpha + dst * (1 - alpha)).
static inline uint8_t Blend(uint32_t s, uint32_t d, float alpha, double inv) {
    int32_t srcI = (int32_t)s, dstI = (int32_t)d, r;
    float sf = (float)srcI;
    __asm {
        fld inv
        fld sf
        fmul alpha
        fild dstI
        fmul st(0), st(2)
        faddp st(1), st(0)
        call Rt3_Ftol
        fstp st(0)
        mov r, eax
    }
    return (uint8_t)r;
}

static inline float Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline double Fl_195(uint32_t a) { return *(const float*)rt3::Rdata(a); }

// Average of two channel bytes (x 0.5 in float, truncated).
static inline uint8_t Avg(uint8_t a, uint8_t b) { return (uint8_t)Ftol(((double)(float)a + (float)b) * Fl_195(0x5c8d48)); }

static inline double Fl_207(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

// 0x4f6070 (slider callback). Colour dialog (0x6d1334) slider w (0x294c /
// 0x294e / 0x2950 = red / green / blue, strings 0xdf / 0xe0 / 0xe1) set to
// v: labels it with the number, stores the channel (0x6d1300..), the RGB
// colour (+0x80) and shows it on the swatch 0x296a.
void __fastcall ColourDialog_SliderCb(uint8_t* w, int32_t v) {
    if (!g_6d1334) return;
    char frame[0x90];
    char* num = frame;
    char* text = frame + 0x10;
    const uint16_t id = F<uint16_t>(w, 4);
    Num_Format(num, 1, (double)v, 10, 1, 0, 0);
    const int32_t str = id == 0x294c ? 0xdf : id == 0x294e ? 0xe0 : 0xe1;
    FormatText(text, GetString(str), num, 0, 0, 0, 0, 0, 0, 0);
    *(uint8_t*)(0x6d1300 + ((int32_t)F<uint16_t>(w, 4) - 0x294c) / 2) = (uint8_t)v;
    const uint32_t rgb = ((uint32_t)g_6d1300 << 16) | ((uint32_t)g_6d1301 << 8) | g_6d1302;
    F<uint32_t>(g_6d1334, 0x80) = rgb;
    Window_SendEvent5(g_6d1334, 0, 0x6f, (uint32_t)F<uint16_t>(w, 4) - 1, (uint32_t)(uintptr_t)text, 0, 0);
    Window_SendEvent5(g_6d1334, 0, 0xa8, 0x296a, F<uint32_t>(g_6d1334, 0x80) | 0xff000000, 0, 0);
}
RT3_REPLACE(0x4f6070, ColourDialog_SliderCb);

// 0x4f61c0 (slider callback). Map editor brush strength v + 1 (dialog
// +0xc4, 0x62275c) shown with string 0xe2 on label 0x290c.
void __fastcall MapEditor_SetStrengthCb(void*, int32_t v) {
    uint8_t* dlg = g_6d1334;
    if (!dlg) return;
    const int32_t n = v + 1;
    F<int32_t>(dlg, 0xc4) = n;
    g_62275c = n;
    char num[0x40], text[0x190];
    const char* s = Num_Format(num, 1, (double)n, 10, 1, 0, 0);
    FormatText(text, GetString(0xe2), s, 0, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(g_6d1334, 0, 0x6f, 0x290c, (uint32_t)(uintptr_t)text, 0, 0);
}
RT3_REPLACE(0x4f61c0, MapEditor_SetStrengthCb);

// 0x4f6250. Shows the overview mode of map editor page w (+0x90 / +0x94):
// 0x2c, 0x1a, 0x18, 0x19, 0x1d or 0x1c; otherwise closes it when `close`.
RT3_NOINLINE void __fastcall MapEditor_ShowPageMode(uint8_t* w, int, int32_t close) {
    int32_t page = F<int32_t>(w, 0x90);
    int32_t mode = 0;
    if (page == 6) mode = 0x2c;
    else if (page == 5 && F<int32_t>(w, 0x94) == 1) mode = 0x1a;
    else if (page == 4) mode = 0x18;
    else if (page == 5 && !F<int32_t>(w, 0x94)) mode = 0x19;
    else if (page == 7) mode = 0x1d;
    else if (Screen_IsMode5_2(w)) mode = 0x1c;
    if (mode) {
        Overview_SetMode(g_map, 0, mode, 0, 0, 0, nullptr, 0, 0, 99999, 99999);
        return;
    }
    if (close) Overview_Close(g_map);
}
RT3_REPLACE(0x4f6250, MapEditor_ShowPageMode);

// 0x4f6390. Map editor window w: shows the state of its option buttons.
// Each group gets 0x65 on its picked entry and 0x87 on the others: brushes
// 0x2711 (40, +0xa0), the pages 0x27d9 (+0x9c; when disabled only with
// 0x6d1331 off pages 4..7), 0x283d (+0x98) and 0x28a1 (+0x8c; disabled ones
// only with 0x6d1331), tools 0x28d3 (10, +0x90) and the small groups at
// +0xa4 / +0xb8 / +0xbc / +0xb4 / +0xb0 / +0xac / +0x94 / +0xc0. Then the
// overview buttons 0x2964.. (map mode 0x20 + i or 0x1f), the brush size
// 0x62278c in 0x291a (0xe3, or 0xe4 when at most 0x5d143c), the controls of
// page 1 (0x296b) and pages 4 / 5 (0x2a31), the colour 0x296a (+0x80), closes
// overview mode 0x17 off page 1, the three sliders 0x2a38 (+0xa8, positions
// from +0xd8) and the rows 0x2922..0x2940 by +0xbc.
RT3_NOINLINE void __fastcall MapEditor_ShowControls(uint8_t* w) {
    auto group = [&](uint32_t base, int32_t n, int32_t sel) {
        for (int32_t i = 0; i < n; ++i) Window_SendEvent5(w, 0, i == sel ? 0x65 : 0x87, base + i, 1, 0, 0);
    };
    group(0x2711, 0x28, F<int32_t>(w, 0xa0));
    for (int32_t i = 0; i < 7; ++i) {
        uint32_t t = 0x87;
        if (i == F<int32_t>(w, 0x9c)) {
            int32_t page = F<int32_t>(w, 0x90);
            if (Widget_IsEnabled(Widget_FindChild(w, 0, 0x27d9 + i)) ||
                (g_6d1331 && page != 6 && page != 7 && page != 4 && page != 5))
                t = 0x65;
        }
        Window_SendEvent5(w, 0, t, 0x27d9 + i, 1, 0, 0);
    }
    for (int32_t i = 0; i < 7; ++i) {
        uint32_t t = 0x87;
        if (i == F<int32_t>(w, 0x98) && (Widget_IsEnabled(Widget_FindChild(w, 0, 0x283d + i)) || g_6d1331)) t = 0x65;
        Window_SendEvent5(w, 0, t, 0x283d + i, 1, 0, 0);
    }
    for (int32_t i = 0; i < 6; ++i) {
        uint32_t t = 0x87;
        if (i == F<int32_t>(w, 0x8c) && (Widget_IsEnabled(Widget_FindChild(w, 0, 0x28a1 + i)) || g_6d1331)) t = 0x65;
        Window_SendEvent5(w, 0, t, 0x28a1 + i, 1, 0, 0);
    }
    group(0x28d3, 10, F<int32_t>(w, 0x90));
    group(0x29cd, 3, F<int32_t>(w, 0xa4));
    group(0x2911, 2, F<int32_t>(w, 0xb8));
    group(0x2954, 4, F<int32_t>(w, 0xbc));
    group(0x290d, 3, F<int32_t>(w, 0xb4));
    group(0x2913, 2, F<int32_t>(w, 0xb0));
    group(0x2915, 2, F<int32_t>(w, 0xac));
    group(0x2917, 3, F<int32_t>(w, 0x94));
    group(0x291f, 2, F<int32_t>(w, 0xc0));
    int32_t mode = F<int32_t>(g_map, 0x2175);
    for (uint32_t id = 0x2964; id < 0x2968; ++id)
        Window_SendEvent5(w, 0, mode == (int32_t)(id - 0x2944) || mode == 0x1f ? 0x65 : 0x87, id, 1, 0, 0);
    char num[0x1d0], text[0x1f4];
    const char* size;
    if (g_62278c > g_5d143c) size = Num_Format(num, 1, (double)g_62278c, 10, 1, 0, 0);
    else size = GetString(0xe4);
    FormatText(text, GetString(0xe3), size, 0, 0, 0, 0, 0, 0, 0);
    Window_SendEvent5(w, 0, 0x6f, 0x291a, (uint32_t)(uintptr_t)text, 0, 0);
    Window_SendEvent5(w, 0, F<int32_t>(w, 0x8c) == 1 ? 0x6d : 0x86, 0x296b, 3, 0, 0);
    Window_SendEvent5(w, 0, F<int32_t>(w, 0x8c) == 4 || F<int32_t>(w, 0x8c) == 5 ? 0x6d : 0x86, 0x2a31, 3, 0, 0);
    Window_SendEvent5(w, 0, 0xa8, 0x296a, F<uint32_t>(w, 0x80) | 0xff000000, 0, 0);
    if (F<int32_t>(w, 0x8c) != 1 && F<int32_t>(g_map, 0x2175) == 0x17) Overview_Close(g_map);
    for (int32_t i = 0; i < 3; ++i) {
        Window_SendEvent5(w, 0, i == F<int32_t>(w, 0xa8) ? 0x65 : 0x87, 0x2a38 + i, 1, 0, 0);
        int16_t pos = F<int16_t>(w, 0xd8 + i * 8);
        F<int16_t>(Widget_FindChild(w, 0, 0x2a3b + i), 0xae) = (int16_t)(pos + 10);
    }
    for (uint32_t id = 0x292c; id < 0x2930; ++id) {
        int32_t v = F<int32_t>(w, 0xbc);
        uint32_t t = v == 0 || v == 1 ? 0x65 : 0x87;
        Window_SendEvent5(w, 0, t, id - 10, 1, 0, 0);
        Window_SendEvent5(w, 0, t, id, 1, 0, 0);
        t = F<int32_t>(w, 0xbc) == 1 ? 0x65 : 0x87;
        Window_SendEvent5(w, 0, t, id + 10, 1, 0, 0);
        Window_SendEvent5(w, 0, t, id + 20, 1, 0, 0);
    }
}
RT3_REPLACE(0x4f6390, MapEditor_ShowControls);

// 0x4f6930. Value of the overlay grid (+0x114, +0xfc x +0x100) at map position (x, y); 0 outside.
RT3_NOINLINE int32_t __fastcall Overlay_ValueAt(uint8_t* o, int, float x, float y) {
    uint8_t* map = g_map;
    float w = (float)F<int32_t>(map, 0x2155);
    int32_t cols = F<int32_t>(o, 0xfc);
    float fc = (float)cols;
    int32_t ix = (int32_t)((double)x / w * fc);
    float h = (float)F<int32_t>(map, 0x2159);
    int32_t rows = F<int32_t>(o, 0x100);
    float fr = (float)rows;
    int32_t iy = (int32_t)((double)y / h * fr);
    if (ix < 0 || ix >= cols || iy < 0 || iy >= rows) return 0;
    return F<int32_t*>(o, 0x114)[cols * iy + ix];
}
RT3_REPLACE(0x4f6930, Overlay_ValueAt);

// 0x4f69e0 (slider callback). Percentage dialog (0x6d1334) slider w set to
// v: labels it (0x5d0b18) on its label (id + 3), stores v (+id x 8 -
// 0x150b4) and, unless locked (0x6d1330), rebalances the others.
void __fastcall PercentDialog_SliderCb(uint8_t* w, int32_t v) {
    if (!g_6d1334) return;
    char text[0x80];
    crt::sprintf(text, (const char*)0x5d0b18, v);
    Window_SendEvent5(g_6d1334, 0, 0x6f, F<uint16_t>(w, 4) + 3u, (uint32_t)(uintptr_t)text, 0, 0);
    *(int32_t*)(g_6d1334 + F<uint16_t>(w, 4) * 8 - 0x150b4) = v;
    if (!g_6d1330) Window_BalancePercentages(g_6d1334);
}
RT3_REPLACE(0x4f69e0, PercentDialog_SliderCb);

// 0x4f6a60 (map editor key handler). U undoes the last paint; in the paint
// tool (+0x90 3) A / R / D / G / C toggle overview modes 0x1f..0x23 (and
// update the controls). Returns 0 when handled.
int32_t __fastcall MapEditor_HandleKey(uint8_t* e, int, const uint32_t* ev) {
    if (ev[0] != 3) return 1;
    int32_t mode;
    switch (ev[2]) {
    case 'U':
        Editor_UndoPaint(e);
        return 0;
    case 'A': mode = 0x1f; break;
    case 'R': mode = 0x20; break;
    case 'D': mode = 0x21; break;
    case 'G': mode = 0x22; break;
    case 'C': mode = 0x23; break;
    default: return 1;
    }
    if (F<int32_t>(e, 0x90) != 3) return 1;
    uint8_t* m = g_map;
    if (F<int32_t>(m, 0x2175) == mode) Overview_Close(m);
    else Overview_SetMode(m, 0, mode, 0, 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
    MapEditor_ShowControls(e);
    return 0;
}
RT3_REPLACE(0x4f6a60, MapEditor_HandleKey);

// 0x4f6b50. Map editor window w: picks tool `tool` (+0x90) and shows its
// controls: the tool buttons 0x28dd.. (0x6d picked), then for the terrain
// tools 4..7 the brush rows (no brush shape +0x8c) and their value boxes, and
// for the others the shape rows (tools 3 and 8 only allow shapes 0..2), the
// paint rows and, for tool 0, the toggles 0x2777 / 0x2778 (0x622839 /
// 0x622838). Then the option states, the overview mode and the water preset
// +0x98.
RT3_NOINLINE void __fastcall MapEditor_SetTool(uint8_t* w, int, int32_t tool) {
    F<int32_t>(w, 0x90) = tool;
    auto send = [&](uint32_t type, uint32_t id, uint32_t a) { Window_SendEvent5(w, 0, type, id, a, 0, 0); };
    for (int32_t i = 0; i < 10; ++i) send(F<int32_t>(w, 0x90) != i ? 0x86 : 0x6d, 0x28dd + i, 3);
    int32_t t = F<int32_t>(w, 0x90);
    if (t == 6 || t == 7 || t == 4 || t == 5) {
        F<int32_t>(w, 0x8c) = 0;
        send(0x65, 0x28b4, 3);
        for (int32_t i = 0; i < 7; ++i) send(0x87, 0x27d9 + i, 3);
        for (uint32_t id = 0x28ab; id < 0x28b1; ++id) {
            send(0x87, id - 10, 3);
            send(0x65, id, 3);
        }
        send(0x65, 0x28a1, 3);
        send(0x87, 0x28ab, 3);
        send(0x87, 0x2775, 3);
        send(0x87, 0x2776, 3);
        if (F<int32_t>(w, 0x90) == 5) {
            send(0x65, 0x28a2, 3);
            send(0x87, 0x28ac, 3);
            send(0x74, 0x291d, F<uint32_t>(w, 0xc8));
            send(0x74, 0x291e, F<uint32_t>(w, 0xcc));
        }
        if (F<int32_t>(w, 0x90) == 4) {
            send(0x74, 0x291b, F<uint32_t>(w, 0xd0));
            send(0x74, 0x291c, F<uint32_t>(w, 0xd4));
        }
    } else {
        send(0x87, 0x28b4, 3);
        int32_t shape = F<int32_t>(w, 0x8c);
        if ((t == 8 || t == 3) && shape != 0 && shape != 2 && shape != 1) F<int32_t>(w, 0x8c) = 2;
        for (int32_t i = 0; i < 7; ++i) send(0x65, 0x27d9 + i, 3);
        for (uint32_t id = 0x28ab; id < 0x28ae; ++id) {
            send(0x65, id - 10, 3);
            send(0x87, id, 3);
        }
        for (uint32_t id = 0x28ae; id < 0x28b1; ++id) {
            send(0x87, id - 10, 3);
            send(0x65, id, 3);
        }
        send(0x65, 0x2775, 3);
        send(0x65, 0x2776, 3);
        send(0x65, 0x2775, 3);
        send(0x65, 0x2776, 3);
        if (F<int32_t>(w, 0x90) == 2) {
            send(0x65, 0x28a4, 3);
            send(0x87, 0x28ae, 3);
        }
        if (F<int32_t>(w, 0x90) == 1 || F<int32_t>(w, 0x90) == 0)
            for (uint32_t id = 0x28ae; id < 0x28b1; ++id) {
                send(0x65, id - 10, 3);
                send(0x87, id, 3);
            }
        if (!F<int32_t>(w, 0x90)) {
            send(0x74, 0x2777, g_622839);
            send(0x74, 0x2778, g_622838);
        }
    }
    MapEditor_ShowControls(w);
    MapEditor_ShowPageMode(w, 0, 1);
    Map_SetWaterPreset(w, 0, F<int32_t>(w, 0x98));
}
RT3_REPLACE(0x4f6b50, MapEditor_SetTool);

// 0x4f6f50 (event handler). The map editor d. Press 0xca: the controls set
// the editor state, each mirrored in a global (tool +0x90 / 0x6d1314.., brush
// +0x98, texture +0xa0 / 0x62274c, strength +0x9c / 0x622754, select +0xa4,
// slot +0xa8, the layer toggles +0xc8..+0xd4 and the overview modes); 0x7d6
// on the map starts a stroke (+0x84, the texture backed up in +0x118 and the
// brush mask 0x6d1338 cleared) or, with the pick key (msg +0x20 bit 2), picks
// the layer weights (tool 3) or the colour (tool 1) under the cursor. Mouse
// 2 with 0x7d6 continues the stroke while the button is down and paints by
// tool; 0xcb 0x7d6 ends it (refreshes the minimap 0x808, pushes the undo) and
// 0x2775 undoes. Returns 0 when handled.
int32_t __fastcall MapEditor_HandleEvent(uint8_t* d, int, uint32_t* msg) {
    uint8_t* const world = g_map;
    float x, y;
    int32_t first = 0;
    auto show = [&]() { MapEditor_ShowControls(d); return 0; };
    auto release = [&]() -> int32_t {
        uint8_t* app = g_input;
        if (!F<int32_t>(d, 0x84)) return 1;
        --F<int32_t>(app, 0x100);
        F<int32_t>(d, 0x84) = 0;
        if (uint8_t* root = g_gameWindow)
            if (uint8_t* map = Widget_FindChild(root, 0, 0x808))
                MinimapWidget_Refresh(map, 0, 1, F<uint32_t*>(d, 0x114), F<int32_t>(d, 0xfc), F<int32_t>(d, 0x100));
        if (F<int32_t>(d, 0xa0) >= 0 && F<int32_t>(d, 0xa0) < 0x20) Editor_PushUndo(d);
        return 0;
    };
    if (msg[0] == 0xcb) {
        if (msg[1] == 0x7d6) return release();
        if (msg[1] != 0x2775) return 1;
        Editor_UndoPaint(d);
        return 0;
    }
    if (msg[0] == 2) {
        if (msg[1] != 0x7d6) goto overview17;
        if (!F<int32_t>(d, 0x84)) {
            // A click with the fill tool (+0x8c 1) and select on: the
            // selection fill as a preview in overview 0x17.
            if (F<int32_t>(d, 0x8c) != 1 || !F<int32_t>(d, 0xa4)) return 1;
            if (!Scene_FocusFlagIs1(&x, &y)) goto overview17;
            uint8_t* buf = ((uint8_t*)crt::malloc((size_t)(F<int32_t>(world, 0x2159) * F<int32_t>(world, 0x2155))));
            MapEditor_FillPaint(d, 0, x, y, ((const float*)0x622790)[F<int32_t>(d, 0x9c)], F<int32_t>(d, 0x88), F<int32_t>(d, 0xa0),
                                F<uint32_t>(d, 0x80), g_622758, ~(msg[8] >> 2) & 1, buf, 0);
            uint8_t* w = g_map;
            if (F<int32_t>(w, 0x2175) == 0x17)
                Overview_Redraw(w, 0, buf);
            else
                Overview_SetMode(w, 0, 0x17, 0, 0, 0, buf, 0, 0, 0x1869f, 0x1869f);
            crt::free(buf);
            return 1;
        }
        Loading_Pump(g_app, 0, 0, 0);
        const uint32_t keys = F<uint32_t>(g_input, 0xa8c);
        if ((keys & 0x800) || !(keys & 0x400)) return release();
        if (MapEditor_IsTool4Mode2(d)) return 0;
        goto paint;
    }
    if (msg[0] != 0xca) return 1;
    {
        const int32_t id = (int32_t)msg[1];
        if (id >= 0x2711 && id <= 0x2730) {
            if (F<int32_t>(d, 0x8c) == 4 || F<int32_t>(d, 0x8c) == 5) {
                F<int32_t>(d, 0xd8 + F<int32_t>(d, 0xa8) * 8) = id - 0x2711;
                ((int32_t*)0x622770)[F<int32_t>(d, 0xa8) * 2] = (int32_t)msg[1] - 0x2711;
                return show();
            }
            F<int32_t>(d, 0x88) = 0;
            g_6d130c = 0;
            F<int32_t>(d, 0xa0) = (int32_t)msg[1] - 0x2711;
            g_62274c = (int32_t)msg[1] - 0x2711;
            return show();
        }
        if (id == 0x2777) {
            g_622839 = !g_622839;
            return 0;
        }
        if (id == 0x2778) {
            g_622838 = !g_622838;
            return 0;
        }
        if (id >= 0x27d9 && id <= 0x27df) {
            F<int32_t>(d, 0x9c) = id - 0x27d9;
            g_622754 = id - 0x27d9;
            return show();
        }
        if (id >= 0x283d && id <= 0x2843) {
            Map_SetWaterPreset(d, 0, id - 0x283d);
            g_622750 = id - 0x283d;
            return show();
        }
        if (id >= 0x28a1 && id <= 0x28a6) {
            g_622748 = id - 0x28a1;
            Map_SetStyle(d, 0, id - 0x28a1);
            return show();
        }
        if (id >= 0x28d3 && id <= 0x28dc) {
            g_6d1308 = id - 0x28d3;
            MapEditor_SetTool(d, 0, id - 0x28d3);
            return show();
        }
        if (id >= 0x2905 && id <= 0x2908) {
            g_6d130c = id - 0x2905;
            F<int32_t>(d, 0x88) = id - 0x2905;
            return show();
        }
        if (id >= 0x290d && id <= 0x290f) {
            g_6d1320 = id - 0x290d;
            F<int32_t>(d, 0xb4) = id - 0x290d;
            MapEditor_ShowControls(d);
            MapEditor_ShowPageMode(d, 0, 1);
            return 0;
        }
        if (id >= 0x2911 && id <= 0x2912) {
            g_6d1324 = id - 0x2911;
            F<int32_t>(d, 0xb8) = id - 0x2911;
            return show();
        }
        if (id >= 0x2913 && id <= 0x2914) {
            g_6d131c = id - 0x2913;
            F<int32_t>(d, 0xb0) = id - 0x2913;
            return show();
        }
        if (id >= 0x2915 && id <= 0x2916) {
            g_6d1318 = id - 0x2915;
            F<int32_t>(d, 0xac) = id - 0x2915;
            return show();
        }
        if (id >= 0x2917 && id <= 0x2919) {
            const int32_t v = id - 0x2917;
            g_6d1314 = v;
            F<int32_t>(d, 0x94) = v;
            Overview_SetMode(g_map, 0, v == 0 ? 0x19 : v == 1 ? 0x1a : 0x1c, 0, 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
            return show();
        }
        // The layer toggles.
        static const uint16_t toggles[4][3] = {
            { 0x291b, 0xd0, 0x2768 }, { 0x291c, 0xd4, 0x276c }, { 0x291d, 0xc8, 0x2760 }, { 0x291e, 0xcc, 0x2764 },
        };
        for (const auto& t : toggles)
            if (id == t[0]) {
                const int32_t v = F<int32_t>(d, t[1]) == 0;
                F<int32_t>(d, t[1]) = v;
                hook::Global<int32_t>(0x620000 + t[2]) = v;
                return 0;
            }
        if (id >= 0x291f && id <= 0x2920) {
            g_6d1310 = id - 0x291f;
            F<int32_t>(d, 0xc0) = id - 0x291f;
            return show();
        }
        if (id >= 0x2954 && id <= 0x2957) {
            g_6d1328 = id - 0x2954;
            F<int32_t>(d, 0xbc) = id - 0x2954;
            return show();
        }
        if (id >= 0x2964 && id <= 0x2967) {
            // The overview modes 0x20..0x23 toggle.
            uint8_t* w = g_map;
            if (F<int32_t>(w, 0x2175) == id - 0x2944)
                Overview_Close(w);
            else
                Overview_SetMode(w, 0, id - 0x2944, 0, 0, 0, nullptr, 0, 0, 0x1869f, 0x1869f);
            return show();
        }
        if (id >= 0x29cd && id <= 0x29cf) {
            F<int32_t>(d, 0xa4) = id - 0x29cd;
            g_622758 = id - 0x29cd;
            return show();
        }
        if (id >= 0x2a38 && id <= 0x2a3a) {
            F<int32_t>(d, 0xa8) = id - 0x2a38;
            return show();
        }
        if (id != 0x7d6) return 1;
    }
    // A press on the map.
    if (!Scene_FocusFlagIs1(&x, &y)) return 0;
    if (msg[8] & 0x800) return 0;
    if ((msg[8] & 4) && F<int32_t>(d, 0x8c) != 1 && F<int32_t>(d, 0x90) == 3) {
        // Pick the four layer weights (0x622728) under the cursor.
        g_6d1328 = 0;
        F<int32_t>(d, 0xbc) = 0;
        for (int32_t i = 0; i < 4; ++i) {
            const int32_t iy = Ftol(y);
            const float v = Grid_Darkness(g_map, 0, Ftol(x), iy, i);
            ((float*)0x622728)[i] = v;
            Window_SendEvent5(d, 0, 0x6a, i + 0x2922, (uint32_t)Ftol((double)v * Fl_207(0x5c8c28)), 0, 0);
        }
        return show();
    }
    if ((msg[8] & 4) && F<int32_t>(d, 0x8c) != 1 && F<int32_t>(d, 0x90) == 1) {
        // Pick the colour under the cursor.
        F<int32_t>(d, 0x80) = Overlay_ValueAt(d, 0, x, y);
        g_6d1300 = F<uint8_t>(d, 0x82);
        g_6d1301 = F<uint8_t>(d, 0x81);
        g_6d1302 = F<uint8_t>(d, 0x80);
        // QUIRK: the three sliders read bytes i / 2 (red, red, green), not i.
        for (int32_t i = 0; i < 3; ++i)
            Window_SendEvent5(d, 0, 0x6a, i + 0x294c, ((const uint8_t*)0x6d1300)[i / 2], 0, 0);
        return show();
    }
    if (F<int32_t>(d, 0x8c) == 1) {
        // The fill tool paints once, undoably.
        memcpy(F<void*>(d, 0x118), F<void*>(d, 0x114), (size_t)(F<int32_t>(d, 0xfc) * F<int32_t>(d, 0x100) * 4));
        MapEditor_FillPaint(d, 0, x, y, ((const float*)0x622790)[F<int32_t>(d, 0x9c)], F<int32_t>(d, 0x88), F<int32_t>(d, 0xa0),
                            F<uint32_t>(d, 0x80), g_622758, ~(msg[8] >> 2) & 1, nullptr, 1);
        Editor_PushUndo(d);
        return 0;
    }
    {
        uint8_t* app = g_input;
        if (F<int32_t>(d, 0x90) == 5 && ((F<uint8_t>(app, 0xa8c) & 3) || hook::As<uint32_t>(g_62278c) == 0xc61c3c00)) {
            // The height tool takes the height under the cursor (shift or
            // none yet, -9999).
            const int32_t iy = Ftol(y);
            g_62278c = (float)Map_GridFloat(g_map, 0, Ftol(x), iy);
            const int32_t iy2 = Ftol(y);
            Map_CellLow3(g_map, 0, Ftol(x), iy2);
            MapEditor_ShowControls(d);
            app = g_input;
            if (F<uint8_t>(app, 0xa8c) & 3) return 0;
        }
        // Start the stroke.
        ++F<int32_t>(app, 0x100);
        F<int32_t>(d, 0x84) = 1;
        F<int32_t>(d, 0xf0) = 0;
        if (F<void*>(d, 0xf4)) crt::free(F<void*>(d, 0xf4));
        if (F<void*>(d, 0xf8)) crt::free(F<void*>(d, 0xf8));
        F<void*>(d, 0xf4) = nullptr;
        F<void*>(d, 0xf8) = nullptr;
        memcpy(F<void*>(d, 0x118), F<void*>(d, 0x114), (size_t)(F<int32_t>(d, 0xfc) * F<int32_t>(d, 0x100) * 4));
        F<int32_t>(d, 0x110) = F<int32_t>(d, 0x10c) = F<int32_t>(d, 0x108) = F<int32_t>(d, 0x104) = 0;
        g_622844 = x;
        g_622848 = y;
        if (void* p = g_6d132c) crt::free(p);
        g_6d132c = nullptr;
        uint8_t* mask = g_6d1338;
        if (!mask) {
            uint8_t* w = g_map;
            mask = ((uint8_t*)crt::malloc((size_t)(F<int32_t>(w, 0x2159) * F<int32_t>(w, 0x2155))));
            g_6d1338 = mask;
        }
        uint8_t* w = g_map;
        memset(mask, 0, (size_t)(F<int32_t>(w, 0x2159) * F<int32_t>(w, 0x2155)));
        first = 1;
    }
paint:
    if (!Scene_FocusFlagIs1(&x, &y)) return 0;
    switch (F<int32_t>(d, 0x90)) {
    case 3: {
        const float r = ((const float*)0x6227c8)[F<int32_t>(d, 0x98)];
        Terrain_PaintBrush(g_map, 0, x, y, r, ((const float*)0x622790)[F<int32_t>(d, 0x9c)],
                           F<int32_t>(d, 0x8c) == 0, F<int32_t>(d, 0xbc), (const float*)0x622728, (const float*)0x622738,
                           g_6d1338);
        uint8_t* w = g_map;
        const int32_t mode = F<int32_t>(w, 0x2175);
        if (mode == 0x1f || mode == 0x20 || mode == 0x21 || mode == 0x22 || mode == 0x23) {
            const int32_t y1 = Ftol((double)r + y + Fl_207(0x5c85b4));
            const int32_t x1 = Ftol((double)r + x + Fl_207(0x5c85b4));
            const int32_t y0 = Ftol((double)y - r - Fl_207(0x5c85b4));
            const int32_t x0 = Ftol((double)x - r - Fl_207(0x5c85b4));
            Overview_RedrawRect(w, 0, x0, y0, x1, y1);
        }
        return 0;
    }
    case 7:
        MapBrush_PaintFlag10(d, 0, x, y);
        return 0;
    case 6:
        MapBrush_PaintBit3(d, 0, x, y);
        return 0;
    default:
        MapEditor_Stroke(d, 0, x, y, first, 0);
        return 0;
    }
overview17:
    if (F<int32_t>(g_map, 0x2175) == 0x17) Overview_Close(g_map);
    return 1;
}
RT3_REPLACE(0x4f6f50, MapEditor_HandleEvent);

// 0x4f7ce0. Map editor window constructor (0x6d1304, also 0x6d1334): takes
// the remembered settings (0x622748.. / 0x6d1308..), the water preset,
// layout 0x5d1454 and the last tool; the busy scene; the palette image
// 0x5d1440 (+0x1f8, pixels +0x1fc, 128 x 128 view), a copy of the whole
// terrain texture (+0x114, size +0xfc / +0x100) with a work copy (+0x118) and
// ten undo levels (texture +0x11c, four point buffers +0x144); the first undo
// step; the texture-to-map scale (0x6227ac, the smaller axis); and the slider,
// colour, height and brush controls (with their callbacks) before showing the
// option states. 0x6d1331 is set while it builds.
uint8_t* __fastcall MapEditorWindow_Construct(uint8_t* w) {
    Widget_SetVtblRet(w);
    F<uint32_t>(w, 0) = 0x5d1430;
    g_6d1304 = w;
    g_6d1331 = 1;
    static const uint32_t fields[][2] = { { 0x80, 0x622788 }, { 0x88, 0x6d130c }, { 0x90, 0x6d1308 }, { 0x8c, 0x622748 }, { 0xa0, 0x62274c },
                                          { 0x9c, 0x622754 }, { 0xa4, 0x622758 }, { 0xc0, 0x6d1310 }, { 0x94, 0x6d1314 }, { 0xac, 0x6d1318 },
                                          { 0xb0, 0x6d131c }, { 0xb4, 0x6d1320 }, { 0xb8, 0x6d1324 }, { 0xc4, 0x62275c }, { 0xc8, 0x622760 },
                                          { 0xcc, 0x622764 }, { 0xd0, 0x622768 } };
    for (const auto& f : fields) F<uint32_t>(w, f[0]) = hook::Global<uint32_t>(f[1]);
    F<uint32_t>(w, 0x84) = 0;
    F<uint32_t>(w, 0xd4) = g_62276c;
    F<uint32_t>(w, 0xbc) = hook::As<uint32_t>(g_6d1328);
    F<int32_t>(w, 0x78) = -1;
    F<int32_t>(w, 0x7c) = -1;
    for (uint32_t k = 0; k < 6; ++k) F<uint32_t>(w, 0xd8 + k * 4) = hook::Global<uint32_t>(0x622770 + k * 4);
    Map_SetWaterPreset(w, 0, g_622750);
    Window_Load(w, 0, 0.0f, 0.0f, (const char*)0x5d1454);
    g_6d1334 = w;
    MapEditor_SetTool(w, 0, g_6d1308);
    Scene_PushBusy((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)));
    uint8_t* tex = TextureCache_Get(g_pack, 0, (const char*)0x5d1440, 0, 0, 0);
    F<uint8_t*>(w, 0x1f8) = tex;
    F<uint32_t*>(w, 0x1fc) = Texture_Pixels32(tex, 0, (int32_t*)(w + 0x1e8), (int32_t*)(w + 0x1ec), 0);
    F<int32_t>(w, 0x1f0) = 0x80;
    F<int32_t>(w, 0x1f4) = 0x80;
    F<uint32_t*>(w, 0x114) = Terrain_ExtractTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, 0.0f, 0.0f, 1.0f, 1.0f, nullptr, nullptr);
    Render_GetSize4((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, (int32_t*)(w + 0xfc), (int32_t*)(w + 0x100));
    F<void*>(w, 0x118) = crt::malloc(F<int32_t>(w, 0xfc) * F<int32_t>(w, 0x100) * 4);
    F<int32_t>(w, 0x1e4) = -1;
    for (int32_t k = 0; k < 10; ++k) {
        F<void*>(w, 0x11c + k * 4) = crt::malloc(F<int32_t>(w, 0xfc) * F<int32_t>(w, 0x100) * 4);
        for (int32_t j = 0; j < 4; ++j) F<void*>(w, 0x144 + (k * 4 + j) * 4) = crt::malloc(Map_PointCount(hook::As<void*>(g_map)));
    }
    Editor_PushUndo(w);
    F<uint32_t>(w, 0xf0) = 0;
    F<uint32_t>(w, 0xf4) = 0;
    F<uint32_t>(w, 0xf8) = 0;
    F<uint32_t>(w, 0xa8) = 0;
    uint8_t* map = g_map;
    float mw = (float)F<int32_t>(map, 0x2155), tw = (float)F<int32_t>(w, 0xfc);
    double sx = (double)mw / tw * D(0x5c8628);
    float mh = (float)F<int32_t>(map, 0x2159), th = (float)F<int32_t>(w, 0x100);
    double sy = (double)mh / th * D(0x5c8628);
    double scale = sx < sy ? sx : sy;
    hook::As<uint32_t>(g_6d132c) = 0;
    g_6227ac = (float)scale;
    MapEditor_ShowPageMode(w, 0, 0);
    for (int32_t i = 0; i < 3; ++i) {
        Window_SendEvent5(w, 0, 0x84, 0x2a32 + i, 0x4f69e0, 0, 0);
        Window_SendEvent5(w, 0, 0x6a, 0x2a32 + i, F<uint32_t>(w, 0xdc + i * 8), 0, 0);
    }
    g_6d1300 = w[0x82];
    g_6d1301 = w[0x81];
    g_6d1302 = w[0x80];
    for (uint32_t id = 0x294c; id < 0x2951; id += 2) {
        Window_SendEvent5(w, 0, 0x84, id, 0x4f6070, 0, 0);
        Window_SendEvent5(w, 0, 0x6a, id, ((uint8_t*)0x6d1300)[(id - 0x294c) / 2], 0, 0);
    }
    for (int32_t k = 0; k < 4; ++k) {
        uint32_t lo = 0x2922 + k, hi = 0x2936 + k;
        Window_SendEvent5(w, 0, 0x84, lo, 0x4f5960, 0, 0);
        Window_SendEvent5(w, 0, 0x84, hi, 0x4f59f0, 0, 0);
        Window_SendEvent5(w, 0, 0x6a, lo, (uint32_t)Ftol((D(0x5c8770) - ((const float*)0x622728)[k]) * D(0x5c85d8)), 0, 0);
        Window_SendEvent5(w, 0, 0x6a, hi, (uint32_t)Ftol((D(0x5c8770) - ((const float*)0x622738)[k]) * D(0x5c85d8)), 0, 0);
    }
    Window_SendEvent5(w, 0, 0x84, 0x2921, 0x4f61c0, 0, 0);
    Window_SendEvent5(w, 0, 0x6a, 0x2921, F<uint32_t>(w, 0xc4) - 1, 0, 0);
    MapEditor_ShowControls(w);
    g_6d1331 = 0;
    return w;
}
RT3_REPLACE(0x4f7ce0, MapEditorWindow_Construct);

// 0x4f81e0. Blends texel (x, y) of the editor image (+0x114, stride +0xfc) with
// texture tile idx (8 per row; tile +0x1f0 wide, rows +0x1f4 apart, stride +0x1e8,
// data +0x1fc, wrapping) by alpha.
RT3_NOINLINE void __fastcall Editor_BlendTexel(uint8_t* o, int, int32_t x, int32_t y, float alpha, int32_t idx) {
    uint8_t* dst = F<uint8_t*>(o, 0x114) + (F<int32_t>(o, 0xfc) * y + x) * 4;
    int32_t w = F<int32_t>(o, 0x1f0);
    int32_t base = (F<int32_t>(o, 0x1f4) * (idx / 8) + y % w) * F<int32_t>(o, 0x1e8) + (idx % 8) * w;
    const uint8_t* src = F<uint8_t*>(o, 0x1fc) + (base + x % w) * 4;
    double inv = D(0x5c8770) - alpha;
    uint32_t v = (uint32_t)Blend(src[3], dst[3], alpha, inv) << 8;
    v = (v | Blend(src[2], dst[2], alpha, inv)) << 8;
    v = (v | Blend(src[1], dst[1], alpha, inv)) << 8;
    v |= Blend(src[0], dst[0], alpha, inv);
    *(uint32_t*)dst = v;
}
RT3_REPLACE(0x4f81e0, Editor_BlendTexel);

// 0x4f8370. Softens pixel (x, y) of the 32-bit image (+0x114, +0xfc x +0x100, not
// on the border): each channel is averaged with the right, then left, lower and
// upper neighbours in turn.
RT3_NOINLINE void __fastcall Image_SoftenPixel(uint8_t* img, int, int32_t x, int32_t y) {
    if (x <= 0) return;  // (before the image is read, as in the original)
    int32_t w = F<int32_t>(img, 0xfc);
    if (x >= w - 1 || y <= 0 || y >= F<int32_t>(img, 0x100) - 1) return;
    uint8_t* pix = F<uint8_t*>(img, 0x114);
    uint8_t* p = pix + (w * y + x) * 4;
    uint8_t c[4];
    static const int8_t order[4] = { 2, 1, 0, 3 };
    for (int8_t k : order) c[k] = Avg(p[4 + k], p[k]);
    for (int8_t k : order) c[k] = Avg(p[k - 4], c[k]);
    uint8_t* q = pix + ((y + 1) * w + x) * 4;
    for (int8_t k : order) c[k] = Avg(q[k], c[k]);
    uint8_t* r = pix + ((y - 1) * w + x) * 4;
    uint8_t a3 = Avg(r[3], c[3]), a2 = Avg(r[2], c[2]), a1 = Avg(r[1], c[1]), a0 = Avg(r[0], c[0]);
    *(uint32_t*)p = (uint32_t)a3 << 24 | (uint32_t)a2 << 16 | (uint32_t)a1 << 8 | a0;
}
RT3_REPLACE(0x4f8370, Image_SoftenPixel);

// 0x4f8770. Average colour of the image's current frame (+0x118, or frame +0x1e4,
// or +0x114) around (x, y) within the diamond of radius +0xc4, weighted by
// closeness; into the three byte outputs.
RT3_NOINLINE void __fastcall Image_AverageColor(uint8_t* o, int, int32_t x, int32_t y, uint8_t* outR, uint8_t* outG, uint8_t* outB) {
    const uint32_t* px;
    if (F<uint32_t>(o, 0x118)) px = F<const uint32_t*>(o, 0x118);
    else if (F<int32_t>(o, 0x1e4) > 0) px = F<const uint32_t*>(o, 0x118 + F<int32_t>(o, 0x1e4) * 4);
    else px = F<const uint32_t*>(o, 0x114);
    int32_t r = F<int32_t>(o, 0xc4);
    int32_t x0 = x - r < 0 ? 0 : x - r, y0 = y - r < 0 ? 0 : y - r;
    int32_t w = F<int32_t>(o, 0xfc);
    int32_t x1 = w - 1 >= r + x ? r + x : w - 1;
    int32_t y1 = F<int32_t>(o, 0x100) - 1 < r + y ? F<int32_t>(o, 0x100) - 1 : r + y;
    float sr = 0.0f, sg = 0.0f, sb = 0.0f, ws = 0.0f;
    if (y0 <= y1) {
        for (int32_t yy = y0, row = w * y0; yy <= y1; ++yy, row += w) {
            int32_t ady = yy - y < 0 ? y - yy : yy - y;
            for (int32_t xx = x0; xx <= x1; ++xx) {
                int32_t adx = xx - x < 0 ? x - xx : xx - x;
                int32_t dist = adx + ady;
                if (dist > r) continue;
                float wt = (float)(r - dist + 1);
                uint32_t p = px[xx + row];
                ws = (float)((double)wt + ws);
                float cr = (float)(int32_t)((p >> 16) & 0xff);
                sr = (float)((double)cr * wt + sr);
                float cg = (float)(int32_t)((p >> 8) & 0xff);
                sg = (float)((double)cg * wt + sg);
                float cb = (float)(int32_t)(p & 0xff);
                sb = (float)((double)cb * wt + sb);
            }
        }
        if (ws <= Fl(0x5c8598)) ws = 0.001f;
    } else ws = 0.001f;
    *outR = (uint8_t)Ftol((double)sr / ws);
    *outG = (uint8_t)Ftol((double)sg / ws);
    *outB = (uint8_t)Ftol((double)sb / ws);
}
RT3_REPLACE(0x4f8770, Image_AverageColor);

// 0x4f89b0. Flood fills the map-sized byte grid from (x, y) through cells marked 1
// (4-neighbours; filled cells end as 3), then clears the unreached 1s to 0.
RT3_NOINLINE void __fastcall Map_FloodFill(int32_t x, int32_t y, uint8_t* grid) {
    uint8_t* m = g_map;
    int32_t w = F<int32_t>(m, 0x2155), h = F<int32_t>(m, 0x2159);
    int32_t minX = x, maxX = x, minY = y, maxY = y;
    grid[w * y + x] = 2;
    int32_t changed;
    do {
        int32_t x0 = minX, x1 = maxX, y0 = minY, y1 = maxY;
        changed = 0;
        int32_t edge = !(x0 > 0 && y0 > 0 && x1 < w - 1 && y1 < h - 1);
        for (int32_t j = y0; j <= y1; ++j) {
            uint8_t* row = grid + w * j;
            for (int32_t i = x0; i <= x1; ++i) {
                if (row[i] != 2) continue;
                row[i] = 3;
                int32_t near = edge && !(i && j && i < w - 1 && j < h - 1);
                for (int k = 0; k < 4; ++k) {
                    int32_t nx = kDirX[k * 2] + i, ny = kDirY[k * 2] + j;
                    if (near && (nx < 0 || nx > w - 1 || ny < 0 || ny > h - 1)) continue;
                    uint8_t* p = grid + ny * w + nx;
                    if (*p != 1) continue;
                    *p = 2;
                    changed = 1;
                    if (nx < minX) minX = nx;
                    if (nx > maxX) maxX = nx;
                    if (ny < minY) minY = ny;
                    if (ny > maxY) maxY = ny;
                }
            }
        }
    } while (changed);
    for (int32_t j = 0; j < h; ++j)
        for (int32_t i = 0; i < w; ++i)
            if (grid[j * w + i] == 1) grid[j * w + i] = 0;
}
RT3_REPLACE(0x4f89b0, Map_FloodFill);

// 0x4f8bb0 (thiscall). Paints the stroke path (cnt points xs/ys in cells,
// brush radius rad, strength) of the map editor d into its texture +0x114
// (texW +0xfc x texH +0x100, sx/sy texels per cell) or, for the height tools,
// into the map. The path's texel box (+0x104..+0x110, grown by the brush and
// one step) is walked in steps (1..8 by radius); a step within the brush of
// a path point (exact when snapping: tiny brush, not the height tools; soft
// unless hard) paints its step x step texels with alpha falling off past half
// the radius (all of it when erasing):
//   tool 0: texture tex from the atlas +0x1fc (mode 4: a random slot of
//           +0xd8.., mode 5: the slot of the nearest random blob) and, with
//           0x622838, the four terrain layers per cell (0x5f3500 table);
//   tool 1: colour, 2: alpha (0 or 0xff by +0xb8, not near water), 8:
//           brighten (+0xc0 0) or darken, 9: smudge toward the average;
//   tool 5: the water/vertex marks (+0x94) per cell, and with +0xcc the
//           corners lowered back to sea level.
// Tools 0 and 1 fade near other terrain kinds (none with shift/ctrl). The
// height tools (4, 5 with +0x94 2) then raise the cells around each point
// (+0xb4 0: in rings 1..5 with +0xd4 lowering the corners, 1: drain the
// centre, 2: toggle the cell bit) or flood them (5/2), blend the changed
// cells' texels, and rebuild the coast; the others paste the texture box.
// Redraws the overview (and refreshes the cells of the height tools).
RT3_NOINLINE void __fastcall MapEditor_PaintPath(uint8_t* d, int, int32_t cnt, float* xs, float* ys, float rad, float strength, int32_t colorMode,
                                    int32_t tex, uint32_t color, int32_t hard, int32_t erase) {
    (void)colorMode;
    auto world = []() { return g_map; };
    if (!cnt) return;
    ++F<int32_t>(world(), 0x2205);
    int32_t snap = F<int32_t>(d, 0x98) == 0;
    if ((F<int32_t>(d, 0x90) == 4 && (F<int32_t>(d, 0xb4) == 0 || F<int32_t>(d, 0xb4) == 2 || F<int32_t>(d, 0xb4) == 1)) ||
        Screen_IsMode5_2(d))
        snap = 0;
    int32_t texCol = (tex % 8) * F<int32_t>(d, 0x1f0);
    int32_t texRow = (tex / 8) * F<int32_t>(d, 0x1f4);
    float minX = BitsF(0x47c34f80), minY = BitsF(0x47c34f80), maxX = BitsF(0xc7c34f80), maxY = BitsF(0xc7c34f80);
    const int32_t mode = F<int32_t>(d, 0x8c);
    // The smoothing modes only take the newest point.
    const int32_t first = mode == 3 || mode == 4 ? cnt - 1 : 0;
    for (int32_t i = first; i < cnt; ++i) {
        if (xs[i] < minX) minX = xs[i];
        if (xs[i] > maxX) maxX = xs[i];
        if (ys[i] < minY) minY = ys[i];
        if (ys[i] > maxY) maxY = ys[i];
    }
    const int32_t texW = F<int32_t>(d, 0xfc);
    const float W = (float)F<int32_t>(world(), 0x2155);
    const float uMin = minX / W, uMax = maxX / W;
    const float H = (float)F<int32_t>(world(), 0x2159);
    const float vMin = minY / H, vMax = maxY / H;
    float radU = rad / W;
    float radV = rad / H;
    {
        const double a = radU, b = D(0x5d1470) / (double)texW;
        radU = (float)(a > b ? a : b);
    }
    const int32_t texH = F<int32_t>(d, 0x100);
    {
        const double a = radV, b = D(0x5d1470) / (double)texH;
        radV = (float)(a > b ? a : b);
    }
    if (mode == 3 || mode == 4) strength = (float)(strength * D(0x5c8758));
    const float sx = (float)texW / W;
    const float sy = (float)texH / H;
    float rTexX, rTexY;
    {
        const double a = (double)sx * rad;
        rTexX = (float)(D(0x5d1468) > a ? D(0x5d1468) : a);
        const double b = (double)sy * rad;
        rTexY = (float)(D(0x5d1468) > b ? D(0x5d1468) : b);
    }
    const float invRX = (float)(D(0x5c8770) / rTexX);
    const float invRY = (float)(D(0x5c8770) / rTexY);
    float* us = (float*)crt::malloc((size_t)(cnt * 4));
    float* vs = (float*)crt::malloc((size_t)(cnt * 4));
    for (int32_t i = first; i < cnt; ++i) {
        us[i] = sx * xs[i];
        vs[i] = sy * ys[i];
    }
    int32_t step = 1;
    if (rad > Fl_207(0x5c9aa8))
        step = 8;
    else if (rad > Fl_207(0x5c85f8))
        step = 4;
    else if (rad > Fl_207(0x5c8554))
        step = 2;
    if (step == 1 || snap)
        for (int32_t i = 0; i < cnt; ++i) {
            us[i] = (float)Ftol((double)us[i] + (snap ? 0.0 : 0.5));
            vs[i] = (float)Ftol((double)vs[i] + (snap ? 0.0 : 0.5));
        }
    // The texel box of the path.
    const float texWf = (float)texW, stepf = (float)step;
    float v = (float)(((double)uMin - radU) * texWf - Fl_207(0x5c878c) - stepf);
    if (Fl_207(0x5c8598) > v) v = 0.0f;
    F<int32_t>(d, 0x104) = Ftol(v);
    const float texHf = (float)texH;
    v = (float)(((double)vMin - radV) * texHf - Fl_207(0x5c878c) - stepf);
    if (Fl_207(0x5c8598) > v) v = 0.0f;
    F<int32_t>(d, 0x108) = Ftol(v);
    float lim = (float)(texW - 1);
    v = (float)(((double)radU + uMax) * texWf + stepf + Fl_207(0x5c878c));
    if (lim < v) v = lim;
    F<int32_t>(d, 0x10c) = Ftol(v);
    lim = (float)(texH - 1);
    v = (float)(((double)radV + vMax) * texHf + stepf + Fl_207(0x5c878c));
    if (lim < v) v = lim;
    F<int32_t>(d, 0x110) = Ftol(v);
    const size_t cells = (size_t)(F<int32_t>(world(), 0x2159) * F<int32_t>(world(), 0x2155));
    uint8_t* seen = (uint8_t*)crt::malloc(cells);
    memset(seen, 0, cells);
    const int32_t tool = F<int32_t>(d, 0x90);
    uint8_t tR = 0, tG = 0, tB = 0, tA = 0;
    if (tool == 1)
        tR = (uint8_t)(color >> 16), tG = (uint8_t)(color >> 8), tB = (uint8_t)color;
    else if (tool == 2)
        tA = F<int32_t>(d, 0xb8) ? 0xff : 0;
    // Mode 5 scatters random blobs of the slot textures (+0xd8: texture,
    // +0xdc: weight, three slots).
    int32_t nBlobs = 0;
    int32_t *blobX = nullptr, *blobY = nullptr, *blobT = nullptr;
    if (mode == 5) {
        const int32_t bw = F<int32_t>(d, 0x10c) - F<int32_t>(d, 0x104), bh = F<int32_t>(d, 0x110) - F<int32_t>(d, 0x108);
        nBlobs = bh * bw / 12;
        if (nBlobs <= 1) nBlobs = 1;
        blobX = (int32_t*)crt::malloc((size_t)(nBlobs * 4));
        blobY = (int32_t*)crt::malloc((size_t)(nBlobs * 4));
        blobT = (int32_t*)crt::malloc((size_t)(nBlobs * 4));
        for (int32_t k = 0; k < nBlobs; ++k) {
            blobX[k] = SimRand(0, bw - 1);
            blobY[k] = SimRand(0, bh - 1);
            const int32_t r = SimRand(1, 0x65);
            blobT[k] = F<int32_t>(d, 0xd8);
            for (int32_t c = 0, acc = 0; c < 3; ++c) {
                acc += F<int32_t>(d, 0xdc + c * 8);
                if (r < acc) {
                    blobT[k] = F<int32_t>(d, 0xd8 + c * 8);
                    break;
                }
            }
        }
    }
    // QUIRK: modes above 5 paint the layers with the weights left over from
    // the last cell (stack residue in the original; here the last values).
    float weights[4] = {};
    for (int32_t ty = F<int32_t>(d, 0x108); ty <= F<int32_t>(d, 0x110); ty += step)
        for (int32_t tx = F<int32_t>(d, 0x104); tx <= F<int32_t>(d, 0x10c); tx += step) {
            // The nearest path point, as the squared distance in brush radii.
            float best = BitsF(0x4b18967f);
            for (int32_t i = 0; i < cnt; ++i) {
                float dd;
                if (snap) {
                    dd = (float)tx == us[i] && (float)ty == vs[i] ? 0.0f : BitsF(0x497423f0);
                } else {
                    const float dx = (float)crt::fabs((double)(float)tx - us[i]);
                    if (dx > rTexX) continue;
                    const float dy = (float)crt::fabs((double)(float)ty - vs[i]);
                    if (dy > rTexY) continue;
                    const float ex = dx * invRX, ey = dy * invRY;
                    dd = (float)((double)ey * ey + (double)ex * ex);
                }
                if (dd < best) {
                    best = dd;
                    if (hard && best < D(0x5c8770)) break;
                }
            }
            const double bestD = best;
            if (bestD > D(0x5c8770)) continue;
            for (int32_t oy = 0; oy < step; ++oy) {
                const int32_t py = oy + ty;
                for (int32_t ox = 0; ox < step; ++ox) {
                    const int32_t px = tx + ox;
                    if (px < 0 || px >= F<int32_t>(d, 0xfc) || py < 0 || py >= F<int32_t>(d, 0x100)) continue;
                    const float cx = (float)px / sx;
                    float alpha = strength;
                    const float cy = (float)py / sy;
                    uint8_t* pix = F<uint8_t*>(d, 0x114) + (F<int32_t>(d, 0xfc) * py + px) * 4;
                    const float dist = (float)crt::sqrt(bestD);
                    if (!hard) {
                        if (erase)
                            alpha = (float)((D(0x5c8770) - dist) * strength);
                        else if ((double)dist > D(0x5c8628))
                            alpha = (float)((D(0x5c8770) - (double)dist) / D(0x5c8628) * strength);
                    }
                    if (F<int32_t>(d, 0x8c) == 3 || F<int32_t>(d, 0x8c) == 4) alpha = (float)(alpha * D(0x5c8758));
                    uint8_t a = pix[3], r = pix[2], b = pix[0], g = pix[1];
                    auto blend = [&](uint8_t to, uint8_t from, double inv) {
                        return (uint8_t)Ftol((double)(float)to * alpha + (double)from * inv);
                    };
                    auto store = [&]() { *(uint32_t*)pix = (uint32_t)a << 24 | (uint32_t)r << 16 | (uint32_t)g << 8 | b; };
                    if (g_622838 && F<int32_t>(d, 0x90) == 0) {
                        // The four terrain layers of the cell, once per stroke.
                        const int32_t ix = Ftol(cx), iy = Ftol(cy);
                        uint8_t* w = world();
                        if (ix >= 0 && iy >= 0 && ix < F<int32_t>(w, 0x2155) && iy < F<int32_t>(w, 0x2159) && !seen[iy * F<int32_t>(w, 0x2155) + ix]) {
                            seen[iy * F<int32_t>(w, 0x2155) + ix] = 1;
                            int32_t layerMode = 0, paint = 1;
                            if ((uint32_t)F<int32_t>(d, 0x8c) <= 3) {
                                const float* t = (const float*)(0x5f3500 + F<int32_t>(d, 0xa0) * 0x24);
                                for (int32_t k = 0; k < 4; ++k) weights[k] = t[k];
                                layerMode = ((const int32_t*)t)[8];
                            } else if ((uint32_t)F<int32_t>(d, 0x8c) <= 5) {
                                SimRand(1, 0x65);
                                for (int32_t k = 0; k < 4; ++k) weights[k] = 0.0f;
                                int32_t sum = 0, top = 0;
                                for (int32_t c = 0; c < 3; ++c) {
                                    if (F<int32_t>(d, 0xd8 + c * 8) >= 0x1f) continue;
                                    const int32_t wt = F<int32_t>(d, 0xdc + c * 8);
                                    sum += wt;
                                    if (wt > top) {
                                        top = wt;
                                        layerMode = *(const int32_t*)(0x5f3520 + F<int32_t>(d, 0xd8 + c * 8) * 0x24);
                                    }
                                }
                                if (sum > 0) {
                                    for (int32_t c = 0; c < 3; ++c) {
                                        if (F<int32_t>(d, 0xd8 + c * 8) >= 0x1f) continue;
                                        const float share = (float)F<int32_t>(d, 0xdc + c * 8) / (float)sum;
                                        const float* t = (const float*)(0x5f3500 + F<int32_t>(d, 0xd8 + c * 8) * 0x24);
                                        for (int32_t k = 0; k < 4; ++k) weights[k] = (float)((double)share * t[k] + weights[k]);
                                    }
                                } else {
                                    paint = 0;
                                }
                            }
                            if (paint) Terrain_PaintLayers(world(), 0, ix, iy, alpha, layerMode, weights, (const float*)0x6d133c);
                        }
                    }
                    if (F<int32_t>(d, 0x90) == 5) {
                        // The water and vertex marks of the cells under the texel.
                        const double sxd = sx, cxd = cx, syd = sy, cyd = cy;
                        float oyf = 0.0f;
                        do {
                            const double oyd = oyf;
                            float oxf = 0.0f;
                            do {
                                const double ux = D(0x5c86b8) > sxd ? D(0x5c86b8) : sxd;
                                const int32_t gx = Ftol((float)(D(0x5c8770) / ux * oxf + cxd));
                                const double uy = D(0x5c86b8) > syd ? D(0x5c86b8) : syd;
                                const int32_t gy = Ftol((float)(D(0x5c8770) / uy * oyd + cyd));
                                uint8_t* w = world();
                                if (gx < 0 || gx >= F<int32_t>(w, 0x15d9) - 1 || gy < 0 || gy >= F<int32_t>(w, 0x15d5) - 1) {
                                    g_6d134c += gy + gx + gy + gx;
                                } else {
                                    switch (F<int32_t>(d, 0x94)) {
                                    case 0: Terrain_MarkFlooded(w, 0, gx, gy); break;
                                    case 1: Terrain_PaintVertex(w, 0, gx, gy); break;
                                    case 2: Terrain_MarkDrained(w, 0, gx, gy); break;
                                    }
                                    if (F<int32_t>(d, 0xcc) && !F<int32_t>(d, 0x94)) {
                                        static const int8_t kCorner[4][2] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
                                        for (const auto& k : kCorner) {
                                            const float h = (float)-Map_GridFloat(world(), 0, gx + k[0], gy + k[1]);
                                            if (h != 0.0f) Map_RaiseAround(world(), 0, gx + k[0], gy + k[1], 1, h, 0);
                                        }
                                    }
                                }
                                oxf = oxf + sx;
                            } while (oxf < Fl_207(0x5c85b4));
                            oyf = oyf + sy;
                        } while (oyf < Fl_207(0x5c85b4));
                    }
                    if (F<int32_t>(d, 0x90) == 0 || F<int32_t>(d, 0x90) == 1) {
                        const int32_t iy = Ftol(cy);
                        if (Terrain_KindIs1Or4(world(), 0, Ftol(cx), iy) && !(F<uint8_t>(g_input, 0xa8c) & 3)) {
                            // Fade out toward other terrain kinds.
                            const float k = Map_DistanceToOtherKind(world(), 0, cx, cy);
                            if (k >= Fl_207(0x5c8568)) continue;
                            alpha = (float)((D(0x5c8770) - k / D(0x5c87b8)) * alpha);
                        }
                    }
                    if (F<int32_t>(d, 0x90) == 1) {
                        const double inv = D(0x5c8770) - alpha;
                        r = blend(tR, r, inv);
                        g = blend(tG, g, inv);
                        b = blend(tB, b, inv);
                        store();
                    }
                    if (F<int32_t>(d, 0x90) == 2) {
                        const int32_t iy = Ftol(cy);
                        if (!Map_WaterWithin(world(), 0, Ftol(cx), iy, 2)) {
                            a = (uint8_t)Ftol((D(0x5c8770) - alpha) * (double)a + (double)(float)tA * alpha);
                            store();
                        }
                    }
                    if (F<int32_t>(d, 0x90) == 9) {
                        Image_AverageColor(d, 0, px, py, &tR, &tG, &tB);
                        const double inv = D(0x5c8770) - alpha;
                        r = blend(tR, r, inv);
                        g = blend(tG, g, inv);
                        b = blend(tB, b, inv);
                        store();
                    }
                    if (F<int32_t>(d, 0x90) == 8) {
                        if (!F<int32_t>(d, 0xc0)) {
                            const float add = alpha * Fl_207(0x5c8780);
                            const int32_t nr = Ftol((double)(float)r + add), ng = Ftol((double)(float)g + add), nb = Ftol((double)(float)b + add);
                            r = nr < 0xff ? (uint8_t)nr : 0xff;
                            g = ng < 0xff ? (uint8_t)ng : 0xff;
                            b = nb < 0xff ? (uint8_t)nb : 0xff;
                        } else {
                            const double f = (D(0x5c8770) - alpha) * D(0x5c8618) + D(0x5c8da8);
                            r = (uint8_t)Ftol((double)r * f);
                            g = (uint8_t)Ftol((double)g * f);
                            b = (uint8_t)Ftol((double)b * f);
                        }
                        store();
                    }
                    if (F<int32_t>(d, 0x90) == 0) {
                        if (F<int32_t>(d, 0x8c) == 4) {
                            const int32_t pick = SimRand(1, 0x65);
                            tex = F<int32_t>(d, 0xd8);
                            for (int32_t c = 0, acc = 0; c < 3; ++c) {
                                acc += F<int32_t>(d, 0xdc + c * 8);
                                if (pick < acc) {
                                    tex = F<int32_t>(d, 0xd8 + c * 8);
                                    break;
                                }
                            }
                            texCol = (tex & 7) * F<int32_t>(d, 0x1f0);
                            texRow = (tex >> 3) * F<int32_t>(d, 0x1f4);
                        }
                        if (F<int32_t>(d, 0x8c) == 5) {
                            int32_t nearest = 0, bestD2 = 99999999;
                            for (int32_t k = 0; k < nBlobs; ++k) {
                                const int32_t ey = py - blobY[k] - F<int32_t>(d, 0x108);
                                const int32_t ex = px - blobX[k] - F<int32_t>(d, 0x104);
                                const int32_t d2 = ey * ey + ex * ex;
                                if (d2 < bestD2) {
                                    bestD2 = d2;
                                    nearest = k;
                                }
                            }
                            tex = blobT[nearest];
                            texCol = (tex % 8) * F<int32_t>(d, 0x1f0);
                            texRow = (tex / 8) * F<int32_t>(d, 0x1f4);
                        }
                        if (tex == 0x1f) alpha = 0.0f;
                        const double inv = D(0x5c8770) - alpha;
                        const int32_t tile = F<int32_t>(d, 0x1f0);
                        const int32_t at = px % tile + texCol + (py % tile + texRow) * F<int32_t>(d, 0x1e8);
                        const uint8_t* src = F<uint8_t*>(d, 0x1fc) + at * 4;
                        tR = src[2], tA = src[3], tG = src[1], tB = src[0];
                        r = blend(tR, r, inv);
                        g = blend(tG, g, inv);
                        b = blend(tB, b, inv);
                        if (g_622839) {
                            const int32_t iy = Ftol(cy);
                            if (!Map_WaterWithin(world(), 0, Ftol(cx), iy, 2)) a = blend(tA, a, inv);
                        }
                        store();
                    }
                }
            }
        }
    if ((F<int32_t>(d, 0x90) == 4 && (F<int32_t>(d, 0xb4) == 0 || F<int32_t>(d, 0xb4) == 1 || F<int32_t>(d, 0xb4) == 2)) ||
        (F<int32_t>(d, 0x90) == 5 && F<int32_t>(d, 0x94) == 2)) {
        // The height tools: the cells around each path point (skipping
        // points under a quarter cell from the last).
        const float reach = rad + Fl_207(0x5c85b4);
        const int32_t ir = Ftol(reach);
        float lastX = BitsF(0xc2c60000), lastY = BitsF(0xc2c60000);
        for (int32_t i = 0; i < cnt; ++i) {
            const double dy = crt::fabs((double)lastY - ys[i]);
            if (crt::fabs((double)lastX - xs[i]) + dy < D(0x5c8758)) continue;
            lastX = xs[i];
            lastY = ys[i];
            const int32_t cx = Ftol(xs[i]);
            const int32_t cy = Ftol(ys[i]);
            for (int32_t yy = cy - ir; yy <= cy + ir; ++yy)
                for (int32_t xx = cx - ir; xx <= cx + ir; ++xx) {
                    const int32_t dist = (uint16_t)DistanceInt(xx, yy, cx, cy);
                    if (dist > ir || xx + 1 < 1 || xx >= F<int32_t>(world(), 0x15d9) - 1 || yy < 0 || yy >= F<int32_t>(world(), 0x15d5) - 1)
                        continue;
                    const int32_t t = F<int32_t>(d, 0x90);
                    if (t == 4 && F<int32_t>(d, 0xb4) == 2) {
                        if (!dist) {
                            const uint8_t* was = Terrain_Rec(world(), 0, xx, yy);
                            uint8_t* rec = Terrain_Rec(world(), 0, xx, yy);
                            rec[2] ^= ((uint8_t)(~(uint8_t)(was[2] >> 5) << 5) ^ rec[2]) & 0x20;
                        }
                    } else if (t == 5 && F<int32_t>(d, 0x94) == 2) {
                    } else if (t == 4 && F<int32_t>(d, 0xb4) == 1 && xx == cx && yy == cy) {
                        if (!dist) Map_DrainCell(world(), 0, xx, yy);
                    } else {
                        const int32_t centre = xx == cx && yy == cy;
                        if (!dist) {
                            const int32_t noKeys = (F<uint8_t>(g_input, 0xa8c) & 3) == 0;
                            const int32_t kind = Terrain_KindIs2Or4Or5(world(), 0, xx, yy);
                            Map_FloodCell(world(), 0, xx, yy, noKeys);
                            F<int32_t>(g_6d1304, 0x78) = xx;
                            F<int32_t>(g_6d1304, 0x7c) = yy;
                            const int32_t level = F<int32_t>(d, 0x98) > 6 ? 6 : F<int32_t>(d, 0x98);
                            uint16_t* w = (uint16_t*)Terrain_Rec(world(), 0, xx, yy);
                            *w ^= (uint16_t)(((uint16_t)(level << 6) ^ *w) & 0x1c0);
                            if (!kind) Terrain_Rec(world(), 0, xx, yy)[2] &= 0xdf;
                        }
                        if (centre && i) {
                            // The flow direction from the earlier points
                            // (the last one wins; none: the last stored, or +x).
                            const float fcx = (float)cx, fcy = (float)cy;
                            for (int32_t j = i - 1; j >= 0; --j) {
                                if (xs[j] == fcx && ys[j] == fcy) continue;
                                int32_t fx = Ftol((double)fcx - xs[j]);
                                int32_t fy = Ftol((double)fcy - ys[j]);
                                if (!fx && !fy) {
                                    fx = (int8_t)Grid_Byte1661(world(), 0, xx, yy);
                                    fy = (int8_t)Grid_Byte1665(world(), 0, xx, yy);
                                    if (!fx && !fy) {
                                        fx = g_62283c;
                                        fy = g_622840;
                                        if (!fx && !fy) fx = 1;
                                    }
                                } else {
                                    g_62283c = fx;
                                    g_622840 = fy;
                                }
                                Grid_SetByte1661(world(), 0, xx, yy, (uint8_t)fx);
                                Grid_SetByte1665(world(), 0, xx, yy, (uint8_t)fy);
                            }
                        }
                    }
                    // The cell's level: 1 at the rim up to 5 inside.
                    const int32_t inside = ir - dist;
                    const int32_t level = inside > 1 ? 5 : (inside > 0) + 1;
                    int32_t raise = level - Grid_CellLow(world(), 0, xx, yy);
                    if (xx + 1 == 1 || yy == 0 || xx == F<int32_t>(world(), 0x2155) - 1 || yy == F<int32_t>(world(), 0x2159) - 1) raise = 0;
                    if (F<int32_t>(d, 0x90) == 4) {
                        if (F<int32_t>(d, 0xb4) == 0 && raise > 0) {
                            if (F<int32_t>(d, 0xd4)) {
                                const float amount = (float)((double)(float)(raise * -15) * D(0x5c8758));
                                Map_RaiseAround(world(), 0, xx, yy, 1, amount, 1);
                                Map_RaiseAround(world(), 0, xx + 1, yy, 1, amount, 1);
                                Map_RaiseAround(world(), 0, xx, yy + 1, 1, amount, 1);
                                Map_RaiseAround(world(), 0, xx + 1, yy + 1, 1, amount, 1);
                            }
                            Grid_SetCellLow(world(), 0, xx, yy, (uint8_t)level);
                        }
                    } else if (F<int32_t>(d, 0x90) == 5 && F<int32_t>(d, 0x94) == 2) {
                        int32_t depth = dist - ir < 0 ? ir - dist : dist - ir;
                        if (depth > 3) depth = 3;
                        if (depth > Grid_CellLow(world(), 0, xx, yy)) Grid_SetCellLow(world(), 0, xx, yy, (uint8_t)depth);
                        if (depth >= 1 && F<int32_t>(d, 0xcc)) {
                            static const int8_t kCorner[4][2] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
                            for (const auto& k : kCorner) {
                                const float h = (float)(g_62278c - Map_GridFloat(world(), 0, xx + k[0], yy + k[1]));
                                if (h != 0.0f) Map_RaiseAround(world(), 0, xx + k[0], yy + k[1], 1, h, 0);
                            }
                        }
                    }
                }
        }
        // The changed cells blend their texels.
        minX = minX - reach;
        minY = minY - reach;
        maxX = (float)((double)maxX + rad + Fl_207(0x5c85b4));
        maxY = (float)((double)maxY + rad + Fl_207(0x5c85b4));
        v = sx * minX;
        if (Fl_207(0x5c8598) > v) v = 0.0f;
        F<int32_t>(d, 0x104) = Ftol(v);
        v = sy * minY;
        if (Fl_207(0x5c8598) > v) v = 0.0f;
        F<int32_t>(d, 0x108) = Ftol(v);
        lim = (float)(F<int32_t>(d, 0xfc) - 1);
        v = sx * maxX;
        if (lim < v) v = lim;
        F<int32_t>(d, 0x10c) = Ftol(v);
        lim = (float)(F<int32_t>(d, 0x100) - 1);
        v = sy * maxY;
        if (lim < v) v = lim;
        F<int32_t>(d, 0x110) = Ftol(v);
        for (int32_t tx = F<int32_t>(d, 0x104); tx <= F<int32_t>(d, 0x10c); ++tx)
            for (int32_t ty = F<int32_t>(d, 0x108); ty <= F<int32_t>(d, 0x110); ++ty) {
                const int32_t gx = Ftol((double)(float)tx / sx);
                const int32_t gy = Ftol((double)(float)ty / sy);
                const int32_t stride = F<int32_t>(world(), 0x15d9);
                const int32_t low = Grid_CellLow(world(), 0, gx, gy);
                const int32_t diff = low - hook::As<uint8_t*>(g_6d132c)[(stride + 1) * gy + gx];
                if (!diff) continue;
                float alpha;
                int32_t idx;
                if (F<int32_t>(d, 0x90) == 5 && F<int32_t>(d, 0x94) == 2) {
                    alpha = (float)((double)diff / D(0x5c85c8) * D(0x5c8618));
                    idx = 0x1c;
                } else if (Grid_CellLow(world(), 0, gx, gy) == 5) {
                    idx = 0, alpha = BitsF(0x3ea8f5c3);
                } else {
                    idx = 0xc, alpha = Grid_CellLow(world(), 0, gx, gy) == 2 ? BitsF(0x3e570a3d) : BitsF(0x3dcccccd);
                }
                if ((F<int32_t>(d, 0x90) == 5 && F<int32_t>(d, 0x94) == 2 && F<int32_t>(d, 0xc8)) ||
                    (F<int32_t>(d, 0x90) == 4 && F<int32_t>(d, 0xb4) == 0 && F<int32_t>(d, 0xd0)))
                    Editor_BlendTexel(d, 0, tx, ty, alpha, idx);
            }
    }
    if (F<int32_t>(d, 0x8c) == 5)
        for (int32_t x = F<int32_t>(d, 0x104); x <= F<int32_t>(d, 0x10c); ++x)
            for (int32_t y = F<int32_t>(d, 0x108); y <= F<int32_t>(d, 0x110); ++y) Image_SoftenPixel(d, 0, x, y);
    if (F<int32_t>(d, 0x90) == 4 || F<int32_t>(d, 0x90) == 5) {
        const float margin = 4.0f;
        const int32_t y1 = Ftol((double)(float)F<int32_t>(d, 0x110) / sy + margin);
        const int32_t x1 = Ftol((double)(float)F<int32_t>(d, 0x10c) / sx + margin);
        const int32_t y0 = Ftol((double)(float)F<int32_t>(d, 0x108) / sy - margin);
        const int32_t x0 = Ftol((double)(float)F<int32_t>(d, 0x104) / sx - margin);
        Map_RebuildCoast(world(), 0, x0, y0, x1, y1, F<uint32_t*>(d, 0x114));
    } else {
        Terrain_PasteTexture((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, F<int32_t>(d, 0x104), F<int32_t>(d, 0x108),
                             F<int32_t>(d, 0x10c) + 1, F<int32_t>(d, 0x110) + 1);
    }
    --F<int32_t>(world(), 0x2205);
    if (F<int32_t>(d, 0x90) == 5 || F<int32_t>(d, 0x90) == 4) {
        // Redraw the path's cells.
        float x0 = BitsF(0x461c3c00), y0 = BitsF(0x461c3c00), x1 = BitsF(0xbf800000), y1 = BitsF(0xbf800000);
        for (int32_t i = 0; i < cnt; ++i) {
            if (!(x0 < xs[i])) x0 = xs[i];
            if (!(y0 < ys[i])) y0 = ys[i];
            if (!(x1 > xs[i])) x1 = xs[i];
            if (!(y1 > ys[i])) y1 = ys[i];
        }
        const float top = y1 + rad, right = x1 + rad, bottom = y0 - rad, left = x0 - rad;
        {
            const int32_t ry1 = Ftol((double)top + Fl_207(0x5c85b4)), rx1 = Ftol((double)right + Fl_207(0x5c85b4));
            const int32_t ry0 = Ftol((double)bottom - Fl_207(0x5c85b4)), rx0 = Ftol((double)left - Fl_207(0x5c85b4));
            Overview_RedrawRect(world(), 0, rx0, ry0, rx1, ry1);
        }
        if ((F<int32_t>(d, 0x90) == 4 && (F<int32_t>(d, 0xb4) == 0 || F<int32_t>(d, 0xb4) == 2 || F<int32_t>(d, 0xb4) == 1)) ||
            (F<int32_t>(d, 0x90) == 5 && (F<int32_t>(d, 0x94) == 2 || F<int32_t>(d, 0x94) == 0))) {
            const int32_t ry1 = Ftol((double)top + Fl_207(0x5c878c)), rx1 = Ftol((double)right + Fl_207(0x5c878c));
            const int32_t ry0 = Ftol((double)bottom - Fl_207(0x5c878c)), rx0 = Ftol((double)left - Fl_207(0x5c878c));
            // Map_CellsChanged (keeps ECX/EDX; through its entry).
            ((void(__thiscall*)(uint8_t*, int32_t, int32_t, int32_t, int32_t))&Map_CellsChanged)(world(), rx0, ry0, rx1, ry1);
        }
    }
    crt::free(us);
    crt::free(vs);
    if (F<int32_t>(d, 0x8c) == 5) {
        crt::free(blobX);
        crt::free(blobY);
        crt::free(blobT);
    }
    crt::free(seen);
}
RT3_REPLACE(0x4f8bb0, MapEditor_PaintPath);

// 0x4fb5e0. Map editor ed: fills the area picked at (x, y). The cell mask
// (`mask`, else a temporary W x H one) selects every cell (select 0), the
// cells at least (1) / at most (other) as high as the ground there, and with
// `flood` only those connected to (x, y). With `apply` the tool +0x90 works on
// the selected cells: 3 (and 0 with 0x622838) paints the terrain layers (+0xbc
// / the preset +0xa0); 5 with +0xcc lowers the whole map by the height
// (+0x94 0) or levels the selection to it (+0x94 2, height in 0x62278c). Then,
// except for tool 3, every texel of the map texture (+0x114, +0xfc x +0x100)
// is blended by strength * the bilinear mask coverage: 0 with the texture
// tile `tex` (atlas +0x1fc, 8 per row; alpha with 0x622839), 1 / 9 with
// `color` / the image average, 8 brightens (+0xc0 0) or darkens, 5 marks the
// cell flooded / drained / painted, 4 leaves it, others set the alpha (0 for
// colorMode 1, 0xff for 2). Then the coast is rebuilt; the mini map
// (0x6d0818 child 0x808) gets the new texture either way.
// QUIRK: with tool 5 / +0x94 2 the mask is read one cell past each row and
// past its end. QUIRK: the alpha source is unset unless colorMode is 1 or
// 2 (0 here).
RT3_NOINLINE void __fastcall MapEditor_FillPaint(uint8_t* ed, int, float x, float y, float strength, int32_t colorMode, int32_t tex, uint32_t color,
                                    int32_t select, int32_t flood, uint8_t* mask, int32_t apply) {
    int32_t colOff = tex % 8 * F<int32_t>(ed, 0x1f0);
    int32_t rowOff = F<int32_t>(ed, 0x1f4) * (tex / 8);
    float h = (float)Terrain_HeightAtTile(g_map, 0, x, y);
    uint8_t* m = g_map;
    int32_t W = F<int32_t>(m, 0x2155), H = F<int32_t>(m, 0x2159);
    int32_t owned = 0;
    if (!mask) {
        mask = (uint8_t*)crt::malloc((size_t)(H * W));
        owned = 1;
    }
    memset(mask, 0, (size_t)(uint32_t)(H * W));
    if (!select) memset(mask, 1, (size_t)(uint32_t)(H * W));
    else {
        uint8_t* row = mask;
        for (int32_t cy = 0; cy < H; ++cy, row += W)
            for (int32_t cx = 0; cx < W; ++cx) {
                double v = Grid_Float1605(g_map, 0, cx, cy);
                row[cx] = select == 1 ? (v >= h ? 1 : 0) : (v <= h ? 1 : 0);
            }
        if (flood) {
            int32_t fy = Ftol(y);
            Map_FloodFill(Ftol(x), fy, mask);
        }
    }
    if (apply) {
        int32_t tool = F<int32_t>(ed, 0x90);
        if (tool == 3 || (tool == 0 && g_622838)) {
            uint8_t* row = mask;
            for (int32_t cy = 0; cy < H; ++cy, row += W)
                for (int32_t cx = 0; cx < W; ++cx) {
                    if (!row[cx]) continue;
                    if (F<int32_t>(ed, 0x90) == 3)
                        Terrain_PaintLayers(g_map, 0, cx, cy, strength, F<int32_t>(ed, 0xbc), (const float*)0x622728,
                                            (const float*)0x622738);
                    else {
                        uint32_t p = (uint32_t)F<int32_t>(ed, 0xa0) * 36;
                        Terrain_PaintLayers(g_map, 0, cx, cy, strength, *(int32_t*)(0x5f3520 + p), (const float*)(0x5f3500 + p),
                                            (const float*)(0x5f3510 + p));
                    }
                }
        }
        tool = F<int32_t>(ed, 0x90);
        if (tool != 3) {
            uint8_t sr = 0, sg = 0, sb = 0, sa = 0;
            if (colorMode == 3 || tool == 1) {
                sr = (uint8_t)(color >> 16);
                sg = (uint8_t)(color >> 8);
                sb = (uint8_t)color;
            } else if (colorMode == 1) sa = 0;
            else if (colorMode == 2) sa = 0xff;
            if (tool == 5 && !F<int32_t>(ed, 0x94) && F<int32_t>(ed, 0xcc)) {
                for (int32_t cy = 0; cy < F<int32_t>(g_map, 0x2159) + 1; ++cy)
                    for (int32_t cx = 0; cx < F<int32_t>(g_map, 0x2155) + 1; ++cx) {
                        float g = (float)Map_GridFloat(g_map, 0, cx, cy);
                        float d = g - h;
                        float v = g_5c8598 > d ? 0.0f : d;
                        Map_SetCellHeight(g_map, 0, cx, cy, *(uint32_t*)&v, 0);
                    }
                Map_CellsChangedImpl(g_map, 0, 0, 0, 0x270f, 0x270f);
            }
            if (F<int32_t>(ed, 0x90) == 5 && F<int32_t>(ed, 0x94) == 2 && F<int32_t>(ed, 0xcc)) {
                g_62278c = h;
                MapEditor_ShowControls(ed);
                uint32_t hb = *(uint32_t*)&h;
                for (int32_t cy = 0; cy < F<int32_t>(g_map, 0x2159) + 1; ++cy)
                    for (int32_t cx = 0; cx < F<int32_t>(g_map, 0x2155) + 1; ++cx) {
                        if (!mask[cy * W + cx]) continue;
                        Map_SetCellHeight(g_map, 0, cx, cy, hb, 0);
                        Map_SetCellHeight(g_map, 0, cx + 1, cy, hb, 0);
                        Map_SetCellHeight(g_map, 0, cx, cy + 1, hb, 0);
                        Map_SetCellHeight(g_map, 0, cx + 1, cy + 1, hb, 0);
                    }
                Map_CellsChangedImpl(g_map, 0, 0, 0, 0x270f, 0x270f);
            }
            m = g_map;
            float sx = (float)F<int32_t>(m, 0x2155) / (float)F<int32_t>(ed, 0xfc);
            float sy = (float)F<int32_t>(m, 0x2159) / (float)F<int32_t>(ed, 0x100);
            uint8_t* px = F<uint8_t*>(ed, 0x114);
            for (int32_t ty = 0; ty < F<int32_t>(ed, 0x100); ++ty) {
                if (F<int32_t>(ed, 0xfc) <= 0) continue;
                float fy = (float)ty * sy;
                int32_t cy = Ftol(fy);
                for (int32_t tx = 0; tx < F<int32_t>(ed, 0xfc); ++tx, px += 4) {
                    float fx = (float)tx * sx;
                    int32_t cx = Ftol(fx);
                    float v;
                    if (cx > 0 && cx < W - 1 && cy > 0 && cy < H - 1) {
                        float rx = fx - (float)cx;
                        const uint8_t* p0 = mask + cy * W + cx;
                        float ry = fy - (float)cy;
                        const uint8_t* p1 = mask + (cy + 1) * W + cx;
                        double a = D(0x5c8770) - ry, b = D(0x5c8770) - rx;
                        double sum = rx * (double)(p0[1] != 0) * a;
                        sum = sum + (double)(float)(p1[1] != 0) * ry * rx;
                        sum = sum + ry * (double)(p1[0] != 0) * b;
                        sum = sum + (double)(p0[0] != 0) * b * a;
                        v = (float)sum;
                        v = (float)(v * D(0x5c8750));
                    } else
                        v = mask[cy * W + cx] ? 1.0f : 0.0f;
                    v = v * strength;
                    if (!(v > D(0x5c85b8))) continue;
                    uint8_t pa = px[3], pr = px[2], pb = px[0], pg = px[1];
                    int32_t mode = F<int32_t>(ed, 0x90);
                    if (mode == 4) {
                    } else if (mode == 5) {
                        if (!F<int32_t>(ed, 0x94)) Terrain_MarkFlooded(g_map, 0, cx, cy);
                        else if (F<int32_t>(ed, 0x94) == 2) Terrain_MarkDrained(g_map, 0, cx, cy);
                        else Terrain_PaintVertex(g_map, 0, cx, cy);
                    } else if (mode == 0) {
                        double inv = D(0x5c8770) - v;
                        int32_t n = F<int32_t>(ed, 0x1f0);
                        int32_t idx = (ty % n + rowOff) * F<int32_t>(ed, 0x1e8) + colOff + tx % n;
                        const uint8_t* t = F<uint8_t*>(ed, 0x1fc) + idx * 4;
                        sr = t[2], sa = t[3], sg = t[1], sb = t[0];
                        pr = (uint8_t)Ftol((double)(float)sr * v + (double)pr * inv);
                        pg = (uint8_t)Ftol((double)(float)sg * v + (double)pg * inv);
                        pb = (uint8_t)Ftol((double)(float)sb * v + (double)pb * inv);
                        if (g_622839) pa = (uint8_t)Ftol((double)(float)sa * v + (double)pa * inv);
                    } else if (mode == 8) {
                        if (!F<int32_t>(ed, 0xc0)) {
                            float add = (float)(v * Fl_207(0x5c8780));
                            int32_t r = Ftol((double)(float)pr + add), g = Ftol((double)(float)pg + add), b = Ftol((double)(float)pb + add);
                            pr = r < 0xff ? (uint8_t)r : 0xff;
                            pg = g < 0xff ? (uint8_t)g : 0xff;
                            pb = b < 0xff ? (uint8_t)b : 0xff;
                        } else {
                            double f = (D(0x5c8770) - v) * D(0x5c8618) + D(0x5c8da8);
                            pr = (uint8_t)Ftol((double)pr * f);
                            pg = (uint8_t)Ftol((double)pg * f);
                            pb = (uint8_t)Ftol((double)pb * f);
                        }
                    } else if (mode == 1 || mode == 9) {
                        if (mode == 9) Image_AverageColor(ed, 0, tx, ty, &sr, &sg, &sb);
                        double inv = D(0x5c8770) - v;
                        pr = (uint8_t)Ftol((double)(float)sr * v + (double)pr * inv);
                        pg = (uint8_t)Ftol((double)(float)sg * v + (double)pg * inv);
                        pb = (uint8_t)Ftol((double)(float)sb * v + (double)pb * inv);
                    } else {
                        double inv = D(0x5c8770) - v;
                        pa = (uint8_t)Ftol(inv * (double)pa + (double)(float)sa * v);
                    }
                    *(uint32_t*)px = (uint32_t)pa << 24 | (uint32_t)pr << 16 | (uint32_t)pg << 8 | pb;
                }
            }
            Map_RebuildCoast(g_map, 0, 0, 0, 0x270f, 0x270f, F<uint32_t*>(ed, 0x114));
        }
    }
    if (owned) crt::free(mask);
    if (uint8_t* mm = g_gameWindow)
        if (uint8_t* w = Widget_FindChild(mm, 0, 0x808))
            MinimapWidget_Refresh(w, 0, 1, F<uint32_t*>(ed, 0x114), F<int32_t>(ed, 0xfc), F<int32_t>(ed, 0x100));
}
RT3_REPLACE(0x4fb5e0, MapEditor_FillPaint);

}  // namespace rt3
