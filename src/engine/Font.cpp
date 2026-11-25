// Font.cpp - RT3.exe 0x550000-0x554000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <initializer_list>
#include <intrin.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "core/crt.h"
#include "core/imports.h"
#include "core/jpeglib.h"
#include "game/collection.h"
#include "core/stream.h"
#include "hook/keepregs.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
double __fastcall Font_ScaleY(uint8_t* f, int, float v);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Font_GlyphAdvance(uint8_t* f, int, uint8_t ch);
void __fastcall Terrain_InitMaps(uint8_t* t, int, char init, int32_t, int32_t);
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
void __fastcall StreamWriteU32(int32_t s, uint32_t value);
void __fastcall StripExtension(char* s);
void __fastcall BlockGrid_Save(uint8_t* g, int, int32_t s);
int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
void __fastcall Terrain_Destroy(uint8_t* r);
void __fastcall Terrain_AllocMips(uint8_t* t);
uint8_t* __fastcall BlockGrid_Init(uint8_t* g, int, float w, float h);
void __fastcall BlockGrid_Load(uint8_t* g, int, Stream s);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
void __fastcall Vec_Cross(float* x, float* y, float ax, float ay, float az, float bx, float by, float bz, float* z);
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall NameTable_PushPos(uint8_t* t);
void __fastcall NameTable_PopPosition(uint8_t* t);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
double __fastcall Slider_StepB(uint8_t* s);
int32_t __fastcall HasString(int32_t id);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
uint8_t* __fastcall Font_Construct(uint8_t* t, int, int32_t key);
void __fastcall Font_Create(uint8_t* t);
float __fastcall Fade_Step_Impl();

void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                 uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex,
                                 uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16,
                                 uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20);

void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12);

int32_t __fastcall Texture_Width(uint8_t* t);
int32_t __fastcall Texture_Height(uint8_t* t);
double __fastcall Font_ScaleX(uint8_t* f, int, float v);
float __fastcall Font_TextWidth(uint8_t* f, int, const uint8_t* s);
double __fastcall Slider_HeightScaled(uint8_t* s);

void __fastcall Font_FitText(uint8_t* font, int, const char* text, char* out, float maxW, int32_t ellipsis, float* width,
                             const char** next, int32_t clipFront);

void __fastcall Engine_QueueText(void* e, int, uint32_t p1, float x, float y, uint32_t p4, float w, float h, uint32_t p7, uint32_t p8,
                                 uint32_t p9);

double __fastcall Render_ScaleByUnit(uint8_t*, int, float v);
void __fastcall Font_GetLine(uint8_t* f, int, const char* text, float width, int32_t line, char* out);
void ReleaseResourceD93990();
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
typedef int32_t(__fastcall* StreamFn)(uint8_t* buf, int32_t n);
int32_t __fastcall IsLeadByte(uint16_t c);
int32_t __fastcall IsSingleByte(uint16_t c);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align,
                                 int32_t valign, uint32_t flags, float scale);

// Functions of this file, in exe order.
int32_t __fastcall IsLeadByte(uint16_t c);
int32_t __fastcall IsSingleByte(uint16_t c);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE void __fastcall Terrain_SaveTexture(uint8_t* t, int, const char* path, int32_t mode, int32_t quality);
RT3_NOINLINE void __fastcall Terrain_Create(uint8_t* t, int, int32_t w, int32_t h, const char* tex);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
RT3_NOINLINE void __fastcall Ui_Line(uint32_t color, int32_t glow, float x1, float y1, float x2, float y2, float width);
RT3_NOINLINE double __fastcall Scroll_Total(uint8_t* s);
RT3_NOINLINE double __fastcall Scroll_Visible(uint8_t* s);
double __fastcall Font_GlyphAdvance(uint8_t* f, int, uint8_t ch);
float __fastcall Font_TextWidth(uint8_t* f, int, const uint8_t* s);
void __fastcall Font_FitText(uint8_t* font, int, const char* text, char* out, float maxW, int32_t ellipsis, float* width, const char** next, int32_t clipFront);
void __fastcall Font_ReleaseHandles(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE int32_t __fastcall Text_HitTest(void** font, int, float px, float py, const char* text, float x, float y, float w, float h, int32_t, int32_t, uint32_t flags, float* lineH, int32_t target, float* outX, float* outY);
RT3_NOINLINE double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Font_LoadFromPack(uint8_t* t);
void __fastcall Font_DrawLine(uint8_t* f, int, const uint8_t* text, float x, float y, float width, int32_t, uint32_t color, int32_t align, int32_t snap, uint8_t flags, int32_t* link);
void __fastcall Texture_ReleaseHandles(uint8_t* t);
void __fastcall Font_Create(uint8_t* t);
void __fastcall Font_DrawText(uint8_t* f, int, const char* text, float x, float y, float width, float height, uint32_t color, int32_t align, int32_t valign, uint32_t flags, int32_t measure, int32_t* lines, float* maxWidth, char* copy, int32_t copyLine);
uint8_t* __fastcall Font_Construct(uint8_t* t, int, int32_t key);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE int32_t __fastcall Font_HitTest(uint8_t* f, int, float px, float py, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
void __fastcall Font_GetLine(uint8_t* f, int, const char* text, float width, int32_t line, char* out);
int32_t __fastcall Font_MoveCaretLines(uint8_t* f, int, int32_t pos, int32_t dir, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
uint8_t* __fastcall Slider_Construct(uint8_t* s, int, float x0, float y0, float x1, float y1, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t font);
void __fastcall Nop552130();
double __fastcall Slider_HeightScaled(uint8_t* s);
RT3_NOINLINE double __fastcall Slider_StepA(uint8_t* s);
RT3_NOINLINE double __fastcall Slider_StepB(uint8_t* s);
void __fastcall Glyph_SnapToPixels(uint8_t* g, int, int32_t mode, float* x, float* y, float* du, float* dv);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
RT3_NOINLINE void __fastcall Glyph_QueueRotated(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, float angle, float px, float py);
uint8_t* __fastcall Jpeg_Load(const char* path, int32_t* width, int32_t* height, int32_t offset);
int32_t __fastcall PackSrc_Read(uint8_t* buf, int32_t n);
int32_t __fastcall PackDst_Write(uint8_t* buf, int32_t n);
void __fastcall Nop552ff0();
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize);
RT3_NOINLINE int32_t __fastcall Terrain_Load(uint8_t* t, int, Stream s, const char* tex);
void __fastcall Terrain_Save(uint8_t* t, int, int32_t s, const char* name, int32_t quality);
void __fastcall Sprite_ReleaseImage(uint8_t* s);
RT3_NOINLINE void __fastcall Flags24_Set1(uint8_t* o);
RT3_NOINLINE void __fastcall Flags24_Clear(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_SetUnit25(uint8_t* o, int, float v);
uint8_t __fastcall Flags24_Low(uint8_t* o);
uint32_t __fastcall Get_d93990();
RT3_NOINLINE void ReleaseResourceD93990();
RT3_NOINLINE uint8_t* __fastcall Camera_Construct(uint8_t* c, int, const char* name, uint32_t owner);
void __fastcall Particle_EmitQuads(uint8_t* pd, int, float* out, int32_t* count);
RT3_NOINLINE int32_t __fastcall Lzss_Unpack(StreamFn read, StreamFn write);
RT3_NOINLINE int32_t __fastcall Lzss_Pack(StreamFn read, StreamFn write);
RT3_NOINLINE int32_t __fastcall Rle_Pack(StreamFn read, StreamFn write);
RT3_NOINLINE int32_t __fastcall Rle_Unpack(StreamFn read, StreamFn write);

static inline void Terrain_Pump() {
    if (void (*pump)() = hook::Global<void (*)()>(0x6d4014)) pump();
}

// Tag check (StreamSkip4 with the tag in EDX).
static inline int32_t Terrain_Tag(Stream s, int32_t tag) { return StreamSkip4(s, tag, 0, 0); }

static inline bool Over(float w, float lim) { return w > lim; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline double Sqrt87(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }

static inline double D(uint32_t a) { return *(const double*)a; }

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


// Font table (0x625468, 40 bytes per font): +0 localized face string id, +4 default
// face, +8 pack bitmap-font name, +0xc point size, +0x10 weight, +0x20 extra spacing.
static inline uint8_t* FontRec(int32_t idx) { return (uint8_t*)0x625468 + idx * 40; }

// 0x553aa0 / 0x5549b0 / 0x565da0. Release the pack resources held in 0xd93990 /
// 0xd9399c / 0xd97ab0.
static void ReleaseResourceIn(uint32_t slot) {
    void*& r = hook::Global<void*>(slot);
    if (!r) return;
    Pack_Release(g_pack, 0, r);
    r = nullptr;
}

static inline const int32_t* ScreenMode() {
    uint8_t* e = g_engine;
    return (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
}

// Glyph scales that draw pixel-exact: about 1, 2, 4 or 8 screen pixels per texel.
static bool Glyph_ExactScale(float s) {
    return (s >= D(0x5ca288) && s <= D(0x5c9b10)) || (s >= D(0x5ddca0) && s <= D(0x5dd1b8)) ||
           (s >= D(0x5ddc98) && s <= D(0x5ddc90)) || (s >= D(0x5c8860) && s <= D(0x5ddc88));
}

// ID3DXFont::DrawTextA (vtable slot 6) on the GDI font object at +0.
static int32_t Font_D3dxDrawText(uint8_t* f, const char* text, RECT* rect, uint32_t format, uint32_t color) {
    void* font = F<void*>(f, 0);
    return (*(int32_t(__stdcall**)(void*, const char*, int32_t, RECT*, uint32_t, uint32_t))(*(uint8_t**)font + 0x18))(font, text, -1, rect, format,
                                                                                                                  color);
}

static double FpTan(double v) {
    double r;
    __asm {
        fld v
        fptan
        fstp st(0)
        fstp r
    }
    return r;
}

namespace {
struct HitRect {
    int32_t left, top, right, bottom;
};
struct HitChar {        // one per character position
    int32_t width, line;
    char s[3];
    uint8_t len;
};
struct HitLine {
    int32_t left, top, right, bottom, height;
};
inline int32_t HitFtol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}
inline void HitMeasure(void** font, const char* s, HitRect* r, uint32_t format) {
    ((int32_t(__stdcall*)(void*, const char*, int32_t, HitRect*, uint32_t, uint32_t))(*(void***)*font)[0x18 / 4])(*font, s, -1, r, format, 0);
}
inline uint16_t HitChr(char c) { return (uint16_t)(int16_t)(int8_t)c; }
}

// 0x550090 / 0x5500b0. Scrollbar extents: (+0x28 + +0x1c) or +0x1c, scaled by +0x14;
// +8 x +0x14 when not scrolling (+0x10 == 0).
RT3_NOINLINE double __fastcall Scroll_Total(uint8_t* s) {
    if (F<int32_t>(s, 0x10)) return ((double)F<float>(s, 0x28) + F<float>(s, 0x1c)) * F<float>(s, 0x14);
    return (double)F<float>(s, 0x14) * F<float>(s, 8);
}
RT3_REPLACE(0x550090, Scroll_Total);

RT3_NOINLINE double __fastcall Scroll_Visible(uint8_t* s) {
    if (F<int32_t>(s, 0x10)) return (double)F<float>(s, 0x1c) * F<float>(s, 0x14);
    return (double)F<float>(s, 0x14) * F<float>(s, 8);
}
RT3_REPLACE(0x5500b0, Scroll_Visible);

// 0x5500d0. Advance of glyph ch in the font (style +0xc, table 0x625480, 40 bytes):
// fixed-width digits when the style says so; 0 for control characters and '|'.
RT3_NOINLINE double __fastcall Font_GlyphAdvance(uint8_t* f, int, uint8_t ch) {
    if (ch == 0xf || ch == 0x7c || ch == 0xa || ch == 0xd) return *(const float*)0x5c8598;
    const uint8_t* style = (const uint8_t*)0x625480 + F<int32_t>(f, 0xc) * 40;
    if (F<int32_t>((void*)style, 0) && ch >= '0' && ch <= '9')
        return ((double)F<float>((void*)style, 4) + F<float>(f, 0x30)) * F<float>(f, 0x14);
    double w = Slider_StepA((uint8_t*)Table_Entry(F<uint8_t*>(f, 0x18), 0, ch));
    return (w + F<float>((void*)((const uint8_t*)0x625488 + F<int32_t>(f, 0xc) * 40), 0)) * F<float>(f, 0x14);
}
RT3_REPLACE(0x5500d0, Font_GlyphAdvance);

// 0x550150. Width of a string (sum of glyph advances, float steps).
RT3_NOINLINE float __fastcall Font_TextWidth(uint8_t* f, int, const uint8_t* s) {
    float w = 0.0f;
    for (; *s; ++s) w = (float)(Font_GlyphAdvance(f, 0, *s) + w);
    return w;
}
RT3_REPLACE(0x550150, Font_TextWidth);

// 0x5501a0. Copies as much of `text` (up to a newline) as fits in `maxW` into
// `out`, setting *width and *next (after the copied part). Normally it breaks at the
// last space / '-' / '\' / '/' that fits (or where the width first fitted), dropping
// a trailing space; with `ellipsis` it makes room for "..." and appends it; with
// `clipFront` it drops characters from the front instead.
RT3_NOINLINE void __fastcall Font_FitText(uint8_t* font, int, const char* text, char* out, float maxW, int32_t ellipsis, float* width,
                             const char** next, int32_t clipFront) {
    int32_t wrapped = 0, shrunk = 0, dots = 0, trailing = 0;
    int32_t more = 1, lastFit = 0, i = 0;
    *width = 0.0f;
    do {
        char c = text[i];
        if (c && c != '\n' && (!Over(*width, maxW) || clipFront)) {
            lastFit = i;
            *width = (float)(Font_GlyphAdvance(font, 0, (uint8_t)c) + *width);
        } else {
            more = 0;
            if (Over(*width, maxW)) i = lastFit - 1;
            else --i;
        }
        ++i;
    } while (more);
    float full = *width;
    int32_t end = i;
    int32_t front = 0;
    int32_t len;
    const char* src = text;
    if (!Over(*width, maxW)) goto fits;
search:
    if (clipFront) {
        while (Over(*width, maxW)) {
            *width = (float)((double)*width - Font_GlyphAdvance(font, 0, (uint8_t)text[front]));
            ++front;
        }
        goto fits;
    }
    {
        --i;
        int32_t fitAt = 0;
        bool broke = false;
        while (i > 0) {
            char c = text[i];
            if (c == ' ' || c == '-' || c == 0x5c || c == '/') {
                broke = true;
                break;
            }
            double d = (double)*width - Font_GlyphAdvance(font, 0, (uint8_t)c);
            *width = (float)d;
            if (d < maxW && !fitAt) fitAt = i;
            --i;
        }
        if (!broke) {
            wrapped = 1;
            i = fitAt;
            *width = full;
        }
    }
    if (text[i] == ' ') {
        *width = (float)((double)*width - Font_GlyphAdvance(font, 0, ' '));
        trailing = 1;
    }
    if (ellipsis) {
        double e = (double)F<float>(font, 0x2c) * F<float>(font, 0x14);
        if (!(e + *width > maxW) || shrunk) {
            dots = 1;
            len = i + 1;
            goto copy;
        }
        shrunk = 1;
        maxW = (float)((double)maxW - e);
        if (Over(*width, maxW)) goto search;
        goto fits;
    }
    len = i + 1;
    goto copy;
fits:
    if (clipFront) {
        len = end - front + 1;
        src = text + front;
    } else len = i + 1;
copy:
    StrCopyN(out, src, len);
    if (trailing) {
        size_t n = strlen(out);
        if (n > 2 && out[n - 1] == ' ') out[n - 1] = 0;
    }
    if (dots) {
        *(uint32_t*)(out + i) = 0x2e2e2e;
        *width = (float)(Font_GlyphAdvance(font, 0, (uint8_t)text[i]) * *(const float*)0x5c8900 + *width);
    }
    *next = text + i + 1;
    (void)wrapped;
}
RT3_REPLACE(0x5501a0, Font_FitText);

// 0x550420. Releases a font's handles: a bitmap font (+0x10) drops its pack font
// (+0x18); a GDI one releases its D3DX font (+0) and deletes the HFONT (+4).
void __fastcall Font_ReleaseHandles(uint8_t* t) {
    if (F<int32_t>(t, 0x10)) {
        Pack_Release(g_pack, 0, F<void*>(t, 0x18));
        F<void*>(t, 0x18) = nullptr;
        return;
    }
    if (void* f = F<void*>(t, 0)) ((void(__stdcall*)(void*))(*(void***)f)[2])(f);
    F<void*>(t, 0) = nullptr;
    if (void* h = F<void*>(t, 4)) imp::DeleteObject(h);
    F<void*>(t, 4) = nullptr;
}
RT3_REPLACE(0x550420, Font_ReleaseHandles);

// 0x550470. Number of lines `text` takes wrapped to `width` (at least 1 for GDI
// fonts, measured with DT_CALCRECT in a box 1.5 screens high); *textWidth gets the
// widest line.
RT3_NOINLINE int32_t __fastcall Font_CountLines(uint8_t* f, int, const char* text, float width, float* textWidth) {
    if (F<int32_t>(f, 0x10)) {
        int32_t lines;
        Font_DrawText(f, 0, text, 0.0f, 0.0f, width, BitsF(0x461c3c00), 0xffffffff, 2, 1, 0, 1, &lines, textWidth, nullptr, 0);
        return lines;
    }
    RECT r;
    r.left = 0;
    r.top = 0;
    r.right = Ftol((double)ScreenMode()[0] * width);
    r.bottom = Ftol((double)ScreenMode()[1] * D(0x5c8d68));
    Font_D3dxDrawText(f, text, &r, 0x40d10, 0xffffffff);
    double h = (double)r.bottom / ScreenMode()[1];
    if (textWidth) *textWidth = (float)((double)r.right / ScreenMode()[0]);
    int32_t n = Ftol((F<float>(f, 8) * D(0x5c8628) + h) / F<float>(f, 8));
    return n < 1 ? 1 : n;
}
RT3_REPLACE(0x550470, Font_CountLines);

 // namespace

// 0x550580. Lays `text` out in the box (x, y, w, h; screen units of the font
// metrics) character by character, wrapping where a line grows taller than
// one line, and either finds the character nearest the point (px, py)
// (target < 0; returns its index, -1 for none) or reports the right edge and
// middle height of character `target` in *outX / *outY (returns it). A line
// starting below y + h returns -1 - index. *lineH gets the line height.
// Indexes on a DBCS trail byte step back one.
RT3_NOINLINE int32_t __fastcall Text_HitTest(void** font, int, float px, float py, const char* text, float x, float y, float w, float h, int32_t,
                                int32_t, uint32_t flags, float* lineH, int32_t target, float* outX, float* outY) {
    const int32_t* metric = (const int32_t*)(F<int32_t>(g_engine, 0x34) +
                                             F<int32_t>(g_engine, 0x11421e) * 0x15);
    int32_t best = -1, bestDist = 99999;
    int32_t boxW = HitFtol((double)metric[0] * w);
    int32_t boxH = HitFtol((double)h * metric[1]);
    uint32_t format = (flags & 0x200) ? 0x40d10 : 0x40d11 + ((flags & 0x400) != 0);
    int32_t len = (int32_t)strlen(text);
    int32_t top = HitFtol((double)metric[1] * y);
    if (len <= 0) return best;
    char lineBuf[0xbb8];
    HitChar chars[3000];
    HitLine lines[3000];
    HitLine* line = lines;
    char* out = lineBuf;
    const char* cur = text;
    int32_t lineStart = 0, lineNo = 0, oneLine = 0;
    int32_t i = 0;
    do {
        HitChar& ch = chars[i];
        int32_t n = IsLeadByte(HitChr(*cur)) + 1;
        char* d = ch.s;
        for (int32_t k = 0; k < n; ++k) *d++ = cur[k];
        *d = 0;
        int32_t step = IsLeadByte(HitChr(*cur)) + 1;
        char* o = out;
        for (int32_t k = 0; k < step; ++k) *o++ = cur[k];
        *o = 0;
        ch.len = (uint8_t)step;
        HitRect r = { 0, 0, boxW, boxH };
        HitMeasure(font, lineBuf, &r, format);
        int32_t tall = r.bottom - r.top;
        if (i == lineStart) {
            HitRect s = { 0, 0, boxW, boxH };
            HitMeasure(font, (const char*)0x5cbbb8, &s, format);
            oneLine = s.bottom - s.top;
        }
        uint8_t wrap = 0;
        if (tall > oneLine) {
            *out = 0;
            wrap = 1;
        }
        int32_t next = i + step;
        if (next >= len) wrap = 2;
        if (wrap) {
            r.left = 0;
            r.top = 0;
            r.right = boxW;
            r.bottom = boxH;
            HitMeasure(font, lineBuf, &r, format);
            int32_t offX = HitFtol((double)metric[0] * x);
            r.top += top;
            r.bottom += top;
            r.right += offX;
            r.left += offX;
            line->right = r.right;
            line->bottom = r.bottom;
            line->top = r.top;
            line->left = r.left;
            line->height = r.bottom - r.top;
            metric = (const int32_t*)(F<int32_t>(g_engine, 0x34) +
                                      F<int32_t>(g_engine, 0x11421e) * 0x15);
            if (lineH) *lineH = (float)((double)lines[0].height / metric[1]);
            metric = (const int32_t*)(F<int32_t>(g_engine, 0x34) +
                                      F<int32_t>(g_engine, 0x11421e) * 0x15);
            int32_t tx = HitFtol((double)metric[0] * px);
            int32_t ty = HitFtol((double)metric[1] * py);
            int32_t j = lineStart, xAcc = 0;
            for (; j <= i; ) {
                HitRect c = { 0, 0, boxW, boxH };
                HitMeasure(font, chars[j].s, &c, 0x40d10);
                chars[j].width = c.right - c.left;
                int32_t cl = r.left + xAcc;
                int32_t cr = chars[j].width + xAcc + r.left;
                if (target < 0) {
                    int32_t dx = tx - ((cl + cr) >> 1);
                    int32_t dy = ty - ((r.bottom + r.top) >> 1);
                    if (dx < 0) dx = -dx;
                    if (dy < 0) dy = -dy;
                    int32_t dist = HitFtol(Distance2D(0.0f, 0.0f, (float)dx, (float)dy));
                    if (dist < bestDist) {
                        bestDist = dist;
                        best = j;
                    }
                } else if (j == target) {
                    const int32_t* m2 = (const int32_t*)(F<int32_t>(g_engine, 0x34) +
                                                         F<int32_t>(g_engine, 0x11421e) * 0x15);
                    if (outX) *outX = (float)((double)cr / m2[0]);
                    if (outY) {
                        int32_t mid = (r.bottom - r.top) / 2 + r.top;
                        m2 = (const int32_t*)(F<int32_t>(g_engine, 0x34) +
                                              F<int32_t>(g_engine, 0x11421e) * 0x15);
                        *outY = (float)((double)mid / m2[1]);
                    }
                    if (j > 0 && IsSingleByte(HitChr(text[j]))) --j;
                    return j;
                }
                chars[j].line = lineNo;
                int32_t cw = chars[j].width;
                j += chars[j].len;
                xAcc += cw;
            }
            lineStart = i;
            metric = (const int32_t*)(F<int32_t>(g_engine, 0x34) +
                                      F<int32_t>(g_engine, 0x11421e) * 0x15);
            if ((double)y + h < (double)top / metric[1]) return -1 - j;
            top += lines[0].height;
            if (wrap == 1) {
                ++lineNo;
                ++line;
                out = lineBuf - step;
            }
        }
        cur += step;
        out += step;
        i = next;
    } while (i < len);
    if (best > 0 && IsSingleByte(HitChr(text[best]))) --best;
    return best;
}
RT3_REPLACE(0x550580, Text_HitTest);

// 0x550b50. Height of `text` wrapped to `width` (screen fraction).
RT3_NOINLINE double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth) {
    if (F<int32_t>(f, 0x10)) {
        int32_t lines;
        Font_DrawText(f, 0, text, 0.0f, 0.0f, width, BitsF(0x461c3c00), 0xffffffff, 2, 1, 0, 1, &lines, textWidth, nullptr, 0);
        return (((double)F<float>(f, 0x1c) + F<float>(f, 0x28)) * lines - F<float>(f, 0x28)) * F<float>(f, 0x14);
    }
    return (double)Font_CountLines(f, 0, text, width, textWidth) * F<float>(f, 8);
}
RT3_REPLACE(0x550b50, Font_TextHeight);

// 0x550bd0. Sets up a font from the pack's bitmap fonts: "<name>_<width>.imb" for the
// screen width (1600 unless the mode is one of the five standard ones), then the line
// height ('A' metrics, nudged for fonts 0x11 / 0x15), the space and tab widths ('M'
// and '.') and the widest digit (0.001 at least).
void __fastcall Font_LoadFromPack(uint8_t* t) {
    uint8_t* pack = g_pack;
    NameTable_PushPos(pack);
    const int32_t sizes[5][2] = { { 1600, 1200 }, { 1280, 1024 }, { 1024, 768 }, { 800, 600 }, { 640, 480 } };
    int32_t width = 1600;
    uint8_t* g = g_engine;
    uint8_t* mode = F<uint8_t*>(g, 0x34) + F<int32_t>(g, 0x11421e) * 0x15;
    for (int k = 0; k < 5; ++k)
        if (sizes[k][0] == F<int32_t>(mode, 0) && sizes[k][1] == F<int32_t>(mode, 4)) width = F<int32_t>(mode, 0);
    char num[0x10], name[0x20];
    const char* numStr = Num_Format(num, 8, (double)width, 10, 1, 0, 0);
    uint8_t* rec = FontRec(F<int32_t>(t, 0xc));
    FormatText(name, (const char*)0x5ddc68, F<const char*>(rec, 8), numStr, 0, 0, 0, 0, 0, 0);
    uint8_t* bf = Pack_AcquireNamed(pack, 0, name);
    F<uint8_t*>(t, 0x18) = bf;
    auto glyph = [&](int32_t c) { return (uint8_t*)Table_Entry(F<uint8_t*>(t, 0x18), 0, c); };
    double h = Slider_StepB(glyph(0x41));
    F<float>(t, 0x1c) = (float)h;
    if (F<int32_t>(t, 0xc) == 0x11) F<float>(t, 0x1c) = (float)(h - g_5d06f8);
    if (F<int32_t>(t, 0xc) == 0x15) F<float>(t, 0x1c) = (float)(F<float>(t, 0x1c) - g_5c9bc0);
    double v = Slider_StepA(glyph(0x4d));
    F<float>(t, 0x24) = (float)((v + F<float>(rec, 0x20)) * F<float>(t, 0x14) * g_5cf0f0);
    F<float>(t, 0x20) = (float)(F<float>(t, 0x1c) * g_5cf0f0);
    F<float>(t, 0x28) = (float)(F<float>(t, 0x1c) * g_5c8608);
    v = Slider_StepA(glyph(0x2e));
    F<uint32_t>(t, 0x30) = 0x3a83126f; // 0.001f
    F<float>(t, 0x2c) = (float)((v + F<float>(rec, 0x20)) * F<float>(t, 0x14) * g_5c8900);
    for (int32_t c = 0x30; c < 0x3a; ++c) {
        double d = Slider_StepA(glyph(c));
        // (sic) the digit width is fetched again when it isn't smaller.
        F<float>(t, 0x30) = d < F<float>(t, 0x30) ? F<float>(t, 0x30) : (float)Slider_StepA(glyph(c));
    }
    NameTable_PopPosition(pack);
}
RT3_REPLACE(0x550bd0, Font_LoadFromPack);

// 0x550dd0. Draws one line of text with font `f` at (x, y), aligned in `width`
// (`align` 0 centred, 3 right). Text between '[' and ']' is a link (*link, kept
// across lines) and turns green with flags 0x40; '|' is drawn 0.002 to the left.
// Fonts of a turned style (0x62547c) draw each glyph turned 270 degrees around
// +0x7c / +0x80. Instead of drawing, +0x68 records where character +0x70 sits
// (+0x44 / +0x48, once) and +0x40 finds the character nearest the point +0x44 /
// +0x48 (distance +0x4c, index +0x54 counted from +0x58, +0x5c = the point's y
// +0x64 is on the line). Afterwards flags 0x80 frame the text and 0x1 / 0x4 / 0x8
// underline it (0x4 white, 0x8 black).
void __fastcall Font_DrawLine(uint8_t* f, int, const uint8_t* text, float x, float y, float width, int32_t, uint32_t color, int32_t align,
                              int32_t snap, uint8_t flags, int32_t* link) {
    double tw = Font_TextWidth(f, 0, text);
    if (align == 0)
        x = (float)(((double)width - tw) * D(0x5c8628) + x);
    else if (align == 3)
        x = (float)((double)width - tw + x);
    float minX = 999999.0f, maxX = -999999.0f;
    float ascent = (float)Slider_HeightScaled((uint8_t*)Table_Entry(F<uint8_t*>(f, 0x18), 0, 0));
    for (const uint8_t* p = text; *p; ++p) {
        uint8_t ch = *p;
        if (ch == '[') *link = 1;
        float cx = x;
        float cy = (float)((double)y - ascent);
        uint32_t c = color;
        if (*link && (flags & 0x40)) c = 0xff00ff00;
        if (ch == '|') cx = (float)((double)x - D(0x5ca2a0));
        if (*(const int32_t*)(0x62547c + F<int32_t>(f, 0xc) * 0x28)) {
            cy = y;
            float rx = (float)(((double)y - F<float>(f, 0x80)) * D(0x5c8750) + F<float>(f, 0x7c));
            float ry = (float)((double)F<float>(f, 0x80) - ((double)cx - F<float>(f, 0x7c)) * D(0x5ddc78));
            if (!F<int32_t>(f, 0x40) && !F<int32_t>(f, 0x68))
                Glyph_QueueRotated((uint8_t*)Table_Entry(F<uint8_t*>(f, 0x18), 0, ch), 0, rx, ry, 0x3f800000, F<float>(f, 0x14), F<float>(f, 0x14), c,
                                   BitsF(0x4096cbe4), 0.0f, 0.0f);
        } else if (!F<int32_t>(f, 0x40) && !F<int32_t>(f, 0x68)) {
            Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(f, 0x18), 0, ch), 0, cx, cy, 0x3f800000, F<float>(f, 0x14), F<float>(f, 0x14), c, 0,
                        (snap != 0) + 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
        }
        int32_t index = F<int32_t>(f, 0x58) - (int32_t)(uintptr_t)text + (int32_t)(uintptr_t)p;
        if (F<int32_t>(f, 0x68) && index >= F<int32_t>(f, 0x70) && !F<int32_t>(f, 0x6c)) {
            F<int32_t>(f, 0x6c) = 1;
            F<float>(f, 0x44) = cx;
            F<float>(f, 0x48) = (float)((double)cy - F<float>(f, 0x1c) * D(0x5c8628));
        }
        if (F<int32_t>(f, 0x40)) {
            float dxAbs = (float)fabs((double)cx - F<float>(f, 0x44));
            float lineH = (float)(((double)F<float>(f, 0x28) + F<float>(f, 0x1c)) * F<float>(f, 0x14));
            float top = (float)((double)cy - F<float>(f, 0x1c));
            float py = F<float>(f, 0x48);
            double dy;
            if (py <= top) {
                dy = fabs((double)top - py);
            } else {
                double bottom = (double)top + lineH;
                dy = !(bottom >= py) ? fabs(bottom - py) : (double)Fl(0x5c8598);
            }
            double score = dy * Fl(0x5c9f00) + dxAbs;
            if (score < F<double>(f, 0x4c)) {
                F<double>(f, 0x4c) = score;
                F<int32_t>(f, 0x54) = index;
                float hy = F<float>(f, 0x64);
                F<int32_t>(f, 0x5c) = hy >= top && (double)top + lineH >= hy && dxAbs <= D(0x5ce8c8);
            }
        }
        if (ch == ']') *link = 0;
        if (x < minX) minX = x;
        double nx = Font_GlyphAdvance(f, 0, ch) + x;
        x = (float)nx;
        if (nx > maxX) maxX = x;
    }
    if ((flags & 0x80) && minX < Fl(0x5c8d90)) {
        double onePx = D(0x5c85c8) / ScreenMode()[1];
        double size = (double)F<float>(f, 0x1c) * F<float>(f, 0x14);
        double half = D(0x5c85b8) * size;
        double pad = onePx > half ? onePx : half;
        double baseD = (double)y - (double)ascent * F<float>(f, 0x14);
        float base = (float)baseD;
        float top = (float)(baseD - size * D(0x5c8880) - pad);
        if (!F<int32_t>(f, 0x40) && !F<int32_t>(f, 0x68))
            Ui_FrameRect(color, 0, (float)((double)minX - D(0x5d0280)), (float)((double)top - D(0x5caae0)), (float)((double)maxX - minX + D(0x5ce8c8)),
                         (float)(pad + base - top + D(0x5c9bc0)), 1.0f, 3);
    }
    if ((flags & 0xd) && minX < Fl(0x5c8d90)) {
        double onePx = D(0x5c85c8) / ScreenMode()[1];
        double half = (double)F<float>(f, 0x1c) * F<float>(f, 0x14) * D(0x5c85b8);
        double pad = onePx > half ? onePx : half;
        float ly = (float)((double)y - (double)ascent * F<float>(f, 0x14) + pad);
        uint32_t c = color;
        if (flags & 4) c = 0xffffffff;
        if (flags & 8) c = 0xff000000;
        if (!F<int32_t>(f, 0x40) && !F<int32_t>(f, 0x68)) Ui_Line(c, 0, minX, ly, maxX, ly, 1.0f);
    }
}
RT3_REPLACE(0x550dd0, Font_DrawLine);

// 0x5512a0. Releases a texture's handles: a pack-owned one (+0x10) drops its pack
// reference (+0x18); otherwise the COM object +0 is released and the GDI bitmap +4
// deleted.
void __fastcall Texture_ReleaseHandles(uint8_t* t) {
    if (F<int32_t>(t, 0x10)) {
        Pack_Release(g_pack, 0, F<void*>(t, 0x18));
        F<void*>(t, 0x18) = nullptr;
        return;
    }
    if (void* obj = F<void*>(t, 0)) ((void(__stdcall*)(void*))(*(void***)obj)[2])(obj);
    F<void*>(t, 0) = nullptr;
    if (void* bmp = F<void*>(t, 4)) imp::DeleteObject(bmp);
    F<void*>(t, 4) = nullptr;
}
RT3_REPLACE(0x5512a0, Texture_ReleaseHandles);

// 0x5512f0. Creates font +0xc: from the pack's bitmap fonts (+0x10), or as a GDI font
// handed to D3DX: the localized face (string id +0 of the record) is tried first and
// kept only when GDI really picked it, else the default face; strings 0xfc8 / 0xfc9
// override the face and the charset. +8 is the text height relative to the screen.
void __fastcall Font_Create(uint8_t* t) {
    F<uint32_t>(t, 0) = 0;
    if (F<int32_t>(t, 0x10)) {
        Font_LoadFromPack(t);
        return;
    }
    uint8_t* rec = FontRec(F<int32_t>(t, 0xc));
    char face[0x178], picked[0x1f4];
    for (int32_t i = 0; i < 2; ++i) {
        strcpy(face, F<const char*>(rec, 4));
        int32_t charset = 1;
        if (HasString(0xfc8)) strcpy(face, GetString(0xfc8));
        if (HasString(0xfc9)) charset = crt::atoi(GetString(0xfc9));
        if (i == 0) {
            if (!HasString(F<int32_t>(rec, 0))) continue;
            strcpy(face, GetString(F<int32_t>(rec, 0)));
        }
        double size = F<float>(rec, 0xc);
        if (g_language == 0xc && F<int32_t>(t, 0xc) == 8) size *= g_5ddc80;
        uint8_t* g = g_engine;
        int32_t screenH = F<int32_t>(F<uint8_t*>(g, 0x34) + F<int32_t>(g, 0x11421e) * 0x15, 4);
        int32_t height = Ftol(size * ((double)screenH * g_5d0770));
        F<void*>(t, 4) = imp::CreateFontA(
            height, 0, 0, 0, F<int32_t>(rec, 0x10), 0, 0, 0, (uint32_t)charset, 0, 0, 0, 0, face);
        void* dc = imp::CreateCompatibleDC(nullptr);
        imp::SelectObject(dc, F<void*>(t, 4));
        int32_t tm[0xe];
        imp::GetTextMetricsA(dc, tm);
        F<float>(t, 8) = (float)((double)tm[0] / F<int32_t>(F<uint8_t*>(g, 0x34) + F<int32_t>(g, 0x11421e) * 0x15, 4));
        imp::GetTextFaceA(dc, 0x1f4, picked);
        if (i == 0) {
            if (crt::stricmp(picked, face)) {
                imp::DeleteDC(dc);
                imp::DeleteObject(F<void*>(t, 4));
                F<void*>(t, 4) = nullptr;
                continue;
            }
            i = 999;
        }
        imp::DeleteDC(dc);
    }
    uint8_t* g = g_engine;
    ((int32_t(__stdcall*)(void*, void*, uint8_t*))0x5711b5)(F<void*>(g, 8), F<void*>(t, 4), t);
}
RT3_REPLACE(0x5512f0, Font_Create);

// 0x5514f0. Draws `text` word-wrapped to `width` in the box (x, y, width, height)
// with font `f`. Each line goes to Font_DrawLine (`align`; 5 / 6 switch to right /
// left aligned once a line no longer fits). Unless `measure` is set, a first pass
// counts the lines and drops the ones that don't fit `height`. For `valign` 0 they
// are dropped from top and bottom by turns and the rest centred; for 4 they are
// dropped from the top and the rest bottom aligned; otherwise from the bottom.
// Lines that run past the box get "..." where the alignment asks for it. Fonts
// with a shadow (+0x20) and shadows switched on (engine +0x114697) draw each line
// first in the colour's alpha only, offset by +0x24 pixels (at least 1, at most the
// style's 0x62548c). Flags 2 turn off the shadow, 0x100 the "...". *lines and
// *maxWidth get the line count and the widest line, and line `copyLine` is
// formatted into `copy` (the line itself is the format string).
void __fastcall Font_DrawText(uint8_t* f, int, const char* text, float x, float y, float width, float height, uint32_t color, int32_t align,
                              int32_t valign, uint32_t flags, int32_t measure, int32_t* lines, float* maxWidth, char* copy, int32_t copyLine) {
    union { int32_t i; float f; } lineW;
    lineW.i = (int32_t)strlen(text);
    int32_t firstLine = 0, lastLine = 9999;
    float yOff = 0.0f;
    if (!measure) {
        int32_t n;
        Font_DrawText(f, 0, text, 0.0f, 0.0f, width, BitsF(0x461c3c00), 0xffffffff, 2, 1, 0, 1, &n, nullptr, nullptr, 0);
        float lineH = (float)((double)F<float>(f, 0x1c) + F<float>(f, 0x28));
        int32_t last = n - 1, first = 0;
        double h = ((double)n * lineH - F<float>(f, 0x28)) * F<float>(f, 0x14);
        int32_t k = n;
        while (k > 1 && h > height) {
            --k;
            if (valign == 4 || (valign == 0 && !(k & 1)))
                ++first;
            else
                --last;
            h = ((double)k * lineH - F<float>(f, 0x28)) * F<float>(f, 0x14);
        }
        lastLine = last;
        firstLine = first;
        if (valign == 0)
            yOff = (float)(((double)height - h) * Fl(0x5c8d48));
        else if (valign == 4)
            yOff = (float)((double)height - h);
    }
    const char* p = text;
    float widest = 0.0f;
    int32_t line = 0;
    float shadowX = 0.0f, shadowY = 0.0f;
    if (!(flags & 2)) {
        const int32_t* scr = ScreenMode();
        int32_t sw = scr[0];
        double w = (double)sw;
        int32_t px = Ftol(((double)F<float>(f, 0x24) * F<float>(f, 0x14) + D(0x5c9b10) / w * D(0x5c8628)) * w);
        if (px < 1) px = 1;
        int32_t cap = *(const int32_t*)(0x62548c + F<int32_t>(f, 0xc) * 0x28);
        if (cap > 1 && sw <= 0x400) --cap;
        if (px > cap) px = cap;
        double nn = (double)px + Fl(0x5c8d48);
        shadowX = (float)(nn / w);
        shadowY = (float)(nn / scr[1]);
    }
    const char* end = text + lineW.i;
    int32_t link = 0, shadowLink = 0;
    char buf[1000];
    while (p < end && line <= lastLine) {
        double next2 = (((double)F<float>(f, 0x1c) + F<float>(f, 0x28)) * 2 - F<float>(f, 0x28)) * F<float>(f, 0x14) + yOff;
        int32_t overflow = (double)height + D(0x5c8678) < next2;
        if (flags & 0x100) overflow = 0;
        if ((align == 5 || align == 6) && overflow) flags |= 0x10;
        F<int32_t>(f, 0x58) = (int32_t)(p - text);
        const char* cur = p;
        Font_FitText(f, 0, cur, buf, width, overflow, &lineW.f, &p, flags & 0x10);
        if (!measure && line >= firstLine) {
            int32_t a = align;
            if ((align == 5 || align == 6) && Font_TextWidth(f, 0, (const uint8_t*)buf) >= width) {
                if (align == 5) a = (overflow != 0) + 2;
                if (a == 6) a = overflow ? 3 : 0;
            }
            if (!(flags & 2) && !(F<float>(f, 0x20) == Fl(0x5c8598)) && F<int32_t>(g_engine, 0x114697))
                Font_DrawLine(f, 0, (const uint8_t*)buf, (float)((double)shadowX + x), (float)((double)shadowY + yOff + y), width, 0,
                              color & 0xff000000, a, 1, (flags & 0xd) != 0, &shadowLink);
            Font_DrawLine(f, 0, (const uint8_t*)buf, x, (float)((double)yOff + y), width, 0, color, a, 0, (uint8_t)(flags & 0xcd), &link);
        }
        if (!(widest > lineW.f)) widest = lineW.f;
        if (line >= firstLine) yOff = (float)(((double)F<float>(f, 0x1c) + F<float>(f, 0x28)) * F<float>(f, 0x14) + yOff);
        if (copy && line == copyLine) crt::sprintf(copy, buf);
        ++line;
    }
    if (maxWidth) *maxWidth = widest;
    if (lines) *lines = line;
}
RT3_REPLACE(0x5514f0, Font_DrawText);

// 0x551970. Font constructor for pack key `key` (font 0x754d4c0 + n): bitmap font
// unless 0x6d4034 asks for GDI fonts; the name goes to 0xd93858 (sic: as a format).
uint8_t* __fastcall Font_Construct(uint8_t* t, int, int32_t key) {
    int32_t idx = key - 0x754d4c0;
    F<int32_t>(t, 0x10) = 1;
    F<float>(t, 0x14) = 1.0f;
    F<int32_t>(t, 0xc) = idx;
    if (g_6d4034) F<int32_t>(t, 0x10) = 0;
    crt::sprintf((char*)0xd93858, F<const char*>(FontRec(idx), 8));
    F<uint32_t>(t, 0x68) = 0;
    for (int32_t o = 0x40; o <= 0x5c; o += 4) F<uint32_t>(t, o) = 0;
    F<float>(t, 0x60) = -1.0f;
    F<float>(t, 0x64) = -1.0f;
    Font_Create(t);
    return t;
}
RT3_REPLACE(0x551970, Font_Construct);

// 0x5519f0. Draws `text` in the box (x, y, w, h) at `scale` (kept in +0x14 for the
// call; +0x7c / +0x80 = the box corner turned styles rotate around). Bitmap fonts
// (+0x10) go through Font_DrawText. GDI fonts queue an engine text item instead:
// `valign` 0 / 4 first measure the text to centre / bottom-align it, styles 9-11
// keep only the colour's alpha, and flags 0x1 / 0x4 / 0x8 add an underline under
// the measured text. A debug switch (0x6d3ff8) frames the box in purple. The
// original also reads the mouse position here and ignores it.
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align,
                                 int32_t valign, uint32_t flags, float scale) {
    if (!*text) return;
    F<float>(f, 0x14) = scale;
    F<float>(f, 0x7c) = x;
    F<float>(f, 0x80) = y;
    bool turned = *(const int32_t*)(0x62547c + F<int32_t>(f, 0xc) * 0x28) != 0;
    if (g_6d3ff8 && !F<int32_t>(f, 0x40) && !F<int32_t>(f, 0x68)) {
        if (turned)
            Ui_FrameRect(0xff00ff80, 0, x, (float)((double)y - w), h, w, 1.0f, 3);
        else
            Ui_FrameRect(0xff00ff80, 0, x, y, w, h, 1.0f, 3);
    }
    if (*(const int32_t*)(0x62547c + F<int32_t>(f, 0xc) * 0x28)) {
        w = (float)(w * D(0x5c8750));
        h = (float)(h * D(0x5ddc78));
    }
    if (F<int32_t>(f, 0x10)) {
        Font_DrawText(f, 0, text, x, y, w, h, color, align, valign, flags, 0, nullptr, nullptr, nullptr, 0);
        F<float>(f, 0x14) = 1.0f;
        return;
    }
    if (F<int32_t>(f, 0x40) || F<int32_t>(f, 0x68)) return;
    int32_t style = F<int32_t>(f, 0xc);
    if (style == 9 || style == 10 || style == 11) color &= 0xff000000;
    uint32_t underline = flags & 0xd;
    if (valign != 1 || underline) {
        RECT r;
        r.left = 0;
        r.top = 0;
        r.right = Ftol((double)ScreenMode()[0] * w);
        r.bottom = Ftol((double)ScreenMode()[1] * h);
        float boxH = (float)r.bottom;
        Font_D3dxDrawText(f, text, &r, 0x40d10, 0xffffffff);
        double off = Fl(0x5c8598);
        if (valign == 0)
            off = ((double)boxH - r.bottom) * D(0x5c8628) / ScreenMode()[1];
        else if (valign == 4)
            off = ((double)boxH - r.bottom) / ScreenMode()[1];
        y = (float)(off + y);
        if (underline) {
            double th = (double)r.bottom / ScreenMode()[1];
            double tw = (double)r.right / ScreenMode()[0];
            float len = (float)(D(0x5d0250) + tw);
            double x0;
            if (align == 2)
                x0 = (double)x + D(0x5d0250);
            else if (align == 0)
                x0 = ((double)w - tw) * D(0x5c8628) + x - D(0x5c8678);
            else
                x0 = (double)w - tw + x - D(0x5c9bc0);
            float lx = (float)x0;
            float ly = (float)(th + y);
            if (style == 1 || style == 2) ly = (float)(F<float>(f, 8) * D(0x5c9ba0) + ly);
            uint32_t c = color;
            if (flags & 4) c = 0xffffffff;
            if (flags & 8) c = 0xff000000;
            if (!F<int32_t>(f, 0x40) && !F<int32_t>(f, 0x68)) Ui_Line(c, 0, lx, ly, (float)((double)lx + len), ly, 1.0f);
        }
    }
    if (align == 2)
        flags |= 0x200;
    else if (align == 3)
        flags |= 0x400;
    if (valign == 1)
        flags |= 0x800;
    else if (valign == 4)
        flags |= 0x1000;
    Engine_QueueText(hook::As<void*>(g_engine), 0, (uint32_t)(uintptr_t)text, x, y, 0, w, h, color, (uint32_t)(uintptr_t)f, flags);
    F<float>(f, 0x14) = 1.0f;
}
RT3_REPLACE(0x5519f0, Font_DrawTextBox);

// 0x551d80. Index of the character of `text` (laid out as Font_DrawTextBox
// would) nearest the point (px, py): through Text_HitTest for a font without
// a surface (+0x10), else by drawing it in hit-test mode (+0x40, best
// distance +0x4c, result +0x54).
RT3_NOINLINE int32_t __fastcall Font_HitTest(uint8_t* f, int, float px, float py, const char* text, float x, float y, float w, float h, uint32_t color,
                                int32_t align, int32_t valign, uint32_t flags, float scale) {
    F<float>(f, 0x44) = px;
    F<float>(f, 0x48) = py;
    if (!F<int32_t>(f, 0x10))
        return Text_HitTest((void**)f, 0, px, py, text, x, y, w, h, align, valign, flags, nullptr, -1, nullptr, nullptr);
    F<int32_t>(f, 0x40) = 1;
    F<int32_t>(f, 0x5c) = 0;
    F<uint32_t>(f, 0x4c) = 0xe0000000;
    F<uint32_t>(f, 0x50) = 0x416312cf;
    F<int32_t>(f, 0x54) = 0;
    Font_DrawTextBox(f, 0, text, x, y, w, h, color, align, valign, flags, scale);
    int32_t r = F<int32_t>(f, 0x54);
    F<int32_t>(f, 0x40) = 0;
    return r;
}
RT3_REPLACE(0x551d80, Font_HitTest);

// 0x551e50. Copies line `line` (1-based) of `text` wrapped to `width` into `out`;
// GDI fonts don't wrap here and copy the whole text.
void __fastcall Font_GetLine(uint8_t* f, int, const char* text, float width, int32_t line, char* out) {
    if (F<int32_t>(f, 0x10)) {
        Font_DrawText(f, 0, text, 0.0f, 0.0f, width, BitsF(0x461c3c00), 0xffffffff, 2, 1, 0, 1, nullptr, nullptr, out, line - 1);
        return;
    }
    strcpy(out, text);
}
RT3_REPLACE(0x551e50, Font_GetLine);

// 0x551ea0. Moves caret `pos` of `text` (laid out as Font_DrawTextBox would)
// `dir` lines up (< 0) or down: through Text_HitTest for a font without a
// surface, stepping a line height at a time from the caret's position until
// the hit character changes (stopping at the box edges or, moving the wrong
// way, at the old position); else by a hit-test draw that records the caret
// line (+0x68.. +0x78) and a hit test one line further. -1 for dir 0.
int32_t __fastcall Font_MoveCaretLines(uint8_t* f, int, int32_t pos, int32_t dir, const char* text, float x, float y, float w, float h, uint32_t color,
                                       int32_t align, int32_t valign, uint32_t flags, float scale) {
    if (!dir) return -1;
    if (F<int32_t>(f, 0x10)) {
        F<int32_t>(f, 0x74) = dir;
        float lineH = (float)(((double)F<float>(f, 0x28) + F<float>(f, 0x1c)) * F<float>(f, 0x14));
        F<int32_t>(f, 0x68) = 1;
        F<int32_t>(f, 0x6c) = 0;
        F<int32_t>(f, 0x70) = pos;
        F<int32_t>(f, 0x78) = pos;
        Font_DrawTextBox(f, 0, text, x, y, w, h, color, align, valign, flags, scale);
        float py = (float)((double)F<int32_t>(f, 0x74) * lineH + F<float>(f, 0x48));
        F<uint32_t>(f, 0x60) = F<uint32_t>(f, 0x44);
        F<uint32_t>(f, 0x64) = F<uint32_t>(f, 0x48);
        F<float>(f, 0x48) = py;
        F<int32_t>(f, 0x68) = 0;
        int32_t r = Font_HitTest(f, 0, F<float>(f, 0x44), py, text, x, y, w, h, color, align, valign, flags, scale);
        return F<int32_t>(f, 0x5c) ? pos : r;
    }
    // The caret position comes back in the pos / flags argument slots.
    uint32_t fl = flags;
    float lineH;
    int32_t first = Text_HitTest((void**)f, 0, F<float>(f, 0x44), F<float>(f, 0x48), text, x, y, w, h, align, valign, fl, &lineH, pos, (float*)&flags,
                                 (float*)&pos);
    if (first < 0) return first;
    int32_t r = first;
    while (r == first) {
        float cy = (float)((double)dir * lineH + *(float*)&pos);
        *(float*)&pos = cy;
        if ((double)y + h < cy) return first;
        if (y > (double)cy + *(const double*)0x5d0770) return first;
        r = Text_HitTest((void**)f, 0, *(float*)&flags, cy, text, x, y, w, h, align, valign, fl, nullptr, -1, nullptr, nullptr);
        if (dir < 0) {
            if (r > first) return first;
            dir = -1;
        } else if (dir > 0) {
            if (r < first) return first;
            dir = 1;
        }
        if (r < 0) return r;
    }
    return r;
}
RT3_REPLACE(0x551ea0, Font_MoveCaretLines);

// 0x5520a0. Slider constructor: track (x0, y0)-(x1, y1), extra values, font (+0);
// +0x24 = 0 when the track runs diagonally down-right or up-left, else 1.
uint8_t* __fastcall Slider_Construct(uint8_t* s, int, float x0, float y0, float x1, float y1, uint32_t a,
                                     uint32_t b, uint32_t c, uint32_t d, uint32_t font) {
    F<float>(s, 4) = x0;
    F<float>(s, 0xc) = x1;
    F<uint32_t>(s, 0x14) = a;
    F<uint32_t>(s, 0x1c) = c;
    F<float>(s, 8) = y0;
    F<float>(s, 0x10) = y1;
    F<uint32_t>(s, 0) = font;
    F<uint32_t>(s, 0x18) = b;
    F<uint32_t>(s, 0x20) = d;
    bool straight = (x0 < x1 && y0 < y1) || (x0 > x1 && y0 > y1);
    s[0x24] = straight ? 0 : 1;
    return s;
}
RT3_REPLACE(0x5520a0, Slider_Construct);

void __fastcall Nop552130() { __asm nop }
RT3_REPLACE(0x552130, Nop552130);

RT3_NOINLINE double __fastcall Slider_HeightScaled(uint8_t* s) {
    return Font_ScaleY(F<uint8_t*>(s, 0), 0, (float)((double)F<float>(s, 8) - F<float>(s, 0x18)));
}
RT3_REPLACE(0x552140, Slider_HeightScaled);

// 0x552160 / 0x5521a0. Absolute slider step along / across the track.
RT3_NOINLINE double __fastcall Slider_StepA(uint8_t* s) {
    uint8_t* f = F<uint8_t*>(s, 0);
    double v = s[0x24] ? ((double)F<float>(s, 8) - F<float>(s, 0x10)) * F<float>(f, 0x32) * *(const double*)0x5c8750
                       : ((double)F<float>(s, 4) - F<float>(s, 0xc)) * F<float>(f, 0x2e);
    v *= F<float>(s, 0x1c);
    if (v < *(const float*)0x5c8598) v = -v;
    return v;
}
RT3_REPLACE(0x552160, Slider_StepA);

RT3_NOINLINE double __fastcall Slider_StepB(uint8_t* s) {
    uint8_t* f = F<uint8_t*>(s, 0);
    double v = s[0x24] ? ((double)F<float>(s, 4) - F<float>(s, 0xc)) * F<float>(f, 0x2e) * *(const double*)0x5ddc78
                       : ((double)F<float>(s, 8) - F<float>(s, 0x10)) * F<float>(f, 0x32);
    v *= F<float>(s, 0x20);
    if (v < *(const float*)0x5c8598) v = -v;
    return v;
}
RT3_REPLACE(0x5521a0, Slider_StepB);

// 0x5521e0. Snaps glyph `g`'s screen position (*x, *y; fractions of the screen) to
// whole pixels when its scale (frame size / texel size x +0x1c / +0x20) is exact,
// for `mode` 2 rounded to the pixel centre minus half a pixel; *du / *dv get the
// matching texture-coordinate corrections when the glyph has its own offsets
// (+4 / +8 above 0.01 with +0xc / +0x10 under 0.99).
void __fastcall Glyph_SnapToPixels(uint8_t* g, int, int32_t mode, float* x, float* y, float* du, float* dv) {
    *du = 0.0f;
    *dv = 0.0f;
    if (!mode) return;
    uint8_t* e = g_engine;
    const int32_t* scr = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    float fw = (float)scr[0];
    uint8_t* f = F<uint8_t*>(g, 0);
    int32_t n1 = Ftol((double)F<int32_t>(f, 8) / F<float>(f, 0x2e) + D(0x5c85b8));
    float sx = (float)((double)n1 * F<float>(g, 0x1c) / fw);
    int32_t n2 = Ftol((double)F<int32_t>(f, 0xc) / F<float>(f, 0x32) + D(0x5c85b8));
    float sy = (float)((double)n2 * F<float>(g, 0x20) / scr[1]);
    if (Glyph_ExactScale(sx)) {
        double px = (double)fw * *x;
        if (mode == 2) {
            double q = (double)Ftol(px + D(0x5d1470)) / fw;
            *x = (float)q;
            const int32_t* s2 = ScreenMode();
            *x = (float)(q - D(0x5c8628) / s2[0]);
        } else {
            *x = (float)((double)Ftol(px + D(0x5c85b8)) / fw);
        }
        if (F<float>(g, 4) > D(0x5c85b8) && F<float>(g, 0xc) < D(0x5ca288)) {
            int32_t w = Texture_Width(F<uint8_t*>(f, 0x2a));
            float n = (float)Ftol((double)w * F<float>(g, 4) + D(0x5c85b8));
            int32_t w2 = Texture_Width(F<uint8_t*>(F<uint8_t*>(g, 0), 0x2a));
            *du = (float)((double)n / w2 - F<float>(g, 4));
        }
    }
    if (Glyph_ExactScale(sy)) {
        const int32_t* s2 = ScreenMode();
        double hd = (double)s2[1];
        if (mode == 2) {
            double q = (double)Ftol(hd * *y + D(0x5d1470)) / hd;
            *y = (float)q;
            *y = (float)(q - D(0x5c8628) / (double)ScreenMode()[1]);
        } else {
            *y = (float)((double)Ftol(hd * *y + D(0x5c85b8)) / hd);
        }
        if (F<float>(g, 8) > D(0x5c85b8) && F<float>(g, 0x10) < D(0x5ca288)) {
            int32_t h = Texture_Height(F<uint8_t*>(F<uint8_t*>(g, 0), 0x2a));
            float n = (float)Ftol((double)h * F<float>(g, 8) + D(0x5c85b8));
            int32_t h2 = Texture_Height(F<uint8_t*>(F<uint8_t*>(g, 0), 0x2a));
            *dv = (float)((double)n / h2 - F<float>(g, 8));
        }
    }
}
RT3_REPLACE(0x5521e0, Glyph_SnapToPixels);

// 0x552560. Queues glyph slider `g` (texture corners +4..+0x10, origin offsets
// +0x14 / +0x18, scale +0x1c / +0x20, rotated +0x24; frame +0) at (x, y) with
// `color`, `tint` and the size factors sx / sy, the position snapped by `snap`
// (Glyph_SnapToPixels) and nudged by -0.49 pixel. With `clip` the quad is cut to
// the rectangle (left, top, right, bottom) and its texture range follows.
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t,
                            int32_t snap, int32_t clip, float left, float top, float right, float bottom) {
    float x0 = x, y0 = y;
    uint8_t* f = F<uint8_t*>(g, 0);
    x = (float)(Font_ScaleX(f, 0, (float)((double)F<float>(g, 4) - F<float>(g, 0x14))) + x);
    y = (float)(Font_ScaleY(F<uint8_t*>(g, 0), 0, (float)((double)F<float>(g, 8) - F<float>(g, 0x18))) + y);
    float du, dv;
    Glyph_SnapToPixels(g, 0, snap, &x, &y, &du, &dv);
    const int32_t* scr = ScreenMode();
    float pxw = (float)(D(0x5ddca8) / scr[0]);
    float pxh = (float)(D(0x5ddca8) / scr[1]);
    bool rot = g[0x24] != 0;
    f = F<uint8_t*>(g, 0);
    double wSrc = rot ? ((double)F<float>(g, 8) - F<float>(g, 0x10)) * F<float>(f, 0x32) * D(0x5c8750) : ((double)F<float>(g, 4) - F<float>(g, 0xc)) * F<float>(f, 0x2e);
    double hSrc = rot ? ((double)F<float>(g, 4) - F<float>(g, 0xc)) * F<float>(f, 0x2e) * D(0x5ddc78) : ((double)F<float>(g, 8) - F<float>(g, 0x10)) * F<float>(f, 0x32);
    uint8_t* tex = F<uint8_t*>(f, 0x2a);
    if (!clip) {
        float hh = (float)(hSrc * F<float>(g, 0x20));
        if (hh < Fl(0x5c8598)) hh = -hh;
        float ww = (float)(wSrc * F<float>(g, 0x1c));
        if (ww < Fl(0x5c8598)) ww = -ww;
        float npxh = -pxh, npxw = -pxw;
        float v2 = (float)((double)dv + F<float>(g, 0x10)), u2 = (float)((double)du + F<float>(g, 0xc));
        float v1 = (float)((double)dv + F<float>(g, 8)), u1 = (float)((double)du + F<float>(g, 4));
        float depth = Fade_Step_Impl();
        float y2 = (float)((double)hh * sy + npxh + y), x2 = (float)((double)ww * sx + npxw + x);
        float y1 = (float)((double)npxh + y), x1 = (float)((double)npxw + x);
        Engine_QueueSprite(hook::As<void*>(g_engine), 0, FBits(x1), FBits(y1), FBits(x2), FBits(y2), FBits(depth), color,
                           (uint32_t)(uintptr_t)tex, FBits(u1), FBits(v1), FBits(u2), FBits(v2), tint);
        return;
    }
    float ww = (float)(wSrc * F<float>(g, 0x1c));
    if (ww < Fl(0x5c8598)) ww = -ww;
    double hh = hSrc * F<float>(g, 0x20);
    if (hh < Fl(0x5c8598)) hh = -hh;
    double xr = (double)ww + x;
    if (!(right < xr)) right = (float)xr;
    double yb = y + hh;
    if (!(bottom < yb)) bottom = (float)yb;
    left = (float)((double)x0 - x + left);
    top = (float)((double)y0 - y + top);
    if (!(left > x)) left = x;
    if (!(top > y)) top = y;
    double u1 = ((double)left - x) / ((double)F<float>(f, 0x2e) * F<float>(g, 0x1c)) + F<float>(g, 4);
    double v1 = ((double)top - y) / ((double)F<float>(f, 0x32) * F<float>(g, 0x20)) + F<float>(g, 8);
    double u2 = ((double)right - left) / ww * ((double)F<float>(g, 0xc) - F<float>(g, 4)) + u1;
    double v2 = ((double)bottom - top) / hh * ((double)F<float>(g, 0x10) - F<float>(g, 8)) + v1;
    float npxh = -pxh, npxw = -pxw;
    float fv2 = (float)(v2 + dv), fu2 = (float)(u2 + du), fv1 = (float)(v1 + dv), fu1 = (float)(u1 + du);
    float depth = Fade_Step_Impl();
    float y2 = (float)((double)npxh + bottom), x2 = (float)((double)npxw + right);
    float y1 = (float)((double)npxh + top), x1 = (float)((double)npxw + left);
    Engine_QueueSprite(hook::As<void*>(g_engine), 0, FBits(x1), FBits(y1), FBits(x2), FBits(y2), FBits(depth), color,
                       (uint32_t)(uintptr_t)tex, FBits(fu1), FBits(fv1), FBits(fu2), FBits(fv2), tint);
}
RT3_REPLACE(0x552560, Glyph_Queue);

// 0x552900. Queues glyph `g` at (x, y) scaled by sx / sy and turned by `angle`
// (plus a fixed offset) around the pivot at fractions (px, py) of its size, as two
// textured triangles (vertex colour `color`, `tint` in the last three slots); the
// first corner is snapped like Glyph_Queue and the whole quad moved with it. The
// original also measures the distance from the pivot to all four corners
// (Distance2D) and throws the results away; those pure calls are left out.
RT3_NOINLINE void __fastcall Glyph_QueueRotated(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, float angle,
                                   float px, float py) {
    angle = (float)((double)angle + D(0x5c8748));
    float du = 0.0f, dv = 0.0f;
    bool rot = g[0x24] != 0;
    uint8_t* f = F<uint8_t*>(g, 0);
    double w = (rot ? ((double)F<float>(g, 8) - F<float>(g, 0x10)) * F<float>(f, 0x32) * D(0x5c8750) : ((double)F<float>(g, 4) - F<float>(g, 0xc)) * F<float>(f, 0x2e)) *
               F<float>(g, 0x1c);
    if (w < Fl(0x5c8598)) w = -w;
    double h = (rot ? ((double)F<float>(g, 4) - F<float>(g, 0xc)) * F<float>(f, 0x2e) * D(0x5ddc78) : ((double)F<float>(g, 8) - F<float>(g, 0x10)) * F<float>(f, 0x32)) *
               F<float>(g, 0x20);
    if (h < Fl(0x5c8598)) h = -h;
    float right = (float)(w * sx + x);
    float bottom = (float)(h * sy + y);
    float pivX = (float)(w * sx * px + x);
    float pivY = (float)(h * sy * py + y);
    double k = Fl(0x5d006c), aspect = D(0x5ddc78);
    float pl = (float)((double)pivX - x), pr = (float)((double)pivX - right);
    float qt = (float)(((double)pivY - y) * k), qb = (float)(((double)pivY - bottom) * k);
    double qbFull = ((double)pivY - bottom) * k;
    double c, s;
    __asm {
        fld angle
        fcos
        fstp c
        fld angle
        fsin
        fstp s
    }
    float r1x = (float)((double)pivX - ((double)pl * s + (double)qt * c));
    float r1y = (float)(((double)pl * c - (double)qt * s) * aspect + pivY);
    float r2x = (float)((double)pivX - ((double)pr * s + qbFull * c));
    float r2y = (float)(((double)pr * c - qbFull * s) * aspect + pivY);
    float depth = Fade_Step_Impl();
    float snapX = r1x, snapY = r1y;
    Glyph_SnapToPixels(g, 0, 1, &snapX, &snapY, &du, &dv);
    const int32_t* scr = ScreenMode();
    float ox = (float)((double)snapX - r1x - D(0x5ddca8) / scr[0]);
    float oy = (float)((double)snapY - r1y - D(0x5ddca8) / scr[1]);
    float v1 = (float)((double)dv + F<float>(g, 8)), u2 = (float)((double)du + F<float>(g, 0xc));
    float u1 = (float)((double)du + F<float>(g, 4)), v2 = (float)((double)dv + F<float>(g, 0x10));
    float bx = (float)((double)r2x + ox), by = (float)((double)r2y + oy);
    float ax = (float)((double)r1x + ox), ay = (float)((double)r1y + oy);
    uint32_t tex = (uint32_t)(uintptr_t)F<uint8_t*>(F<uint8_t*>(g, 0), 0x2a);
    float x3 = (float)((double)pivX - ((double)pr * s + (double)qt * c) + ox);
    float y3 = (float)(((double)pr * c - (double)qt * s) * aspect + pivY + oy);
    Engine_QueueQuad(hook::As<void*>(g_engine), 0, FBits(ax), FBits(ay), FBits(bx), FBits(by), FBits(x3), FBits(y3), FBits(depth), color, color, color, tex,
                     FBits(u1), FBits(v1), FBits(u2), FBits(v2), FBits(u2), FBits(v1), tint, tint, tint);
    float x4 = (float)((double)pivX - ((double)pl * s + (double)qb * c) + ox);
    float y4 = (float)(((double)pl * c - (double)qb * s) * aspect + pivY + oy);
    Engine_QueueQuad(hook::As<void*>(g_engine), 0, FBits(ax), FBits(ay), FBits(x4), FBits(y4), FBits(bx), FBits(by), FBits(depth), color, color, color, tex,
                     FBits(u1), FBits(v1), FBits(u1), FBits(v2), FBits(u2), FBits(v2), tint, tint, tint);
}
RT3_REPLACE(0x552900, Glyph_QueueRotated);

// 0x552e20. Loads the JPEG in file path (from byte offset when non-zero) with the
// bundled libjpeg: *width / *height get its size and the 24-bit pixels come back
// in a new buffer (null when the file can't be opened).
uint8_t* __fastcall Jpeg_Load(const char* path, int32_t* width, int32_t* height, int32_t offset) {
    void* f = crt::fopen(path, (const char*)0x5c9678);
    if (!f) return nullptr;
    if (offset) crt::fseek(f, offset, 0);
    struct {
        uint8_t* row;
        uint8_t cinfo[0x1a0];
        uint8_t err[0x84];
    } l;
    F<void*>(l.cinfo, 0) = jpeg::std_error(l.err);
    jpeg::CreateDecompress(l.cinfo, 0x3e, 0x19f);
    jpeg::stdio_src(l.cinfo, f);
    jpeg::read_header(l.cinfo, 1);
    jpeg::start_decompress(l.cinfo);
    uint32_t w = F<uint32_t>(l.cinfo, 0x52), h = F<uint32_t>(l.cinfo, 0x56);
    *width = (int32_t)w;
    *height = (int32_t)h;
    uint8_t* buf = ((uint8_t*)crt::malloc(h * w * 3));
    while (F<uint32_t>(l.cinfo, 0x6e) < F<uint32_t>(l.cinfo, 0x56)) {
        l.row = buf + F<uint32_t>(l.cinfo, 0x6e) * F<uint32_t>(l.cinfo, 0x52) * 3;
        jpeg::read_scanlines(l.cinfo, &l.row, 1);
    }
    jpeg::finish_decompress(l.cinfo);
    jpeg::destroy_decompress(l.cinfo);
    crt::fclose(f);
    return buf;
}
RT3_REPLACE(0x552e20, Jpeg_Load);

// 0x552f30. Pack_Compress's read callback: the next up to n bytes of the source
// (context 0xd93984: data +0, position +4, size +8). When the progress callback
// 0xd93988 is set it gets position / size * [0x5c8c28] (x87, then _ftol). -1 at
// the end.
int32_t __fastcall PackSrc_Read(uint8_t* buf, int32_t n) {
    int32_t* c = g_d93984;
    int32_t left = c[2] - c[1];
    if (n >= left) n = left;
    if (!n) return -1;
    memcpy(buf, (const uint8_t*)(uintptr_t)c[0] + c[1], (size_t)n);
    int32_t pos = c[1] + n;
    c[1] = pos;
    if (void* progress = hook::As<void*>(g_d93988)) {
        int32_t* total = &c[2];
        int32_t pct;
        __asm {
            fild pos
            mov eax, total
            fidiv dword ptr [eax]
            mov eax, 0x5c8c28
            fmul dword ptr [eax]
            mov eax, 0x5a10d0
            call eax
            mov pct, eax
        }
        ((void(__fastcall*)(int32_t))progress)(pct);
    }
    return n;
}
RT3_REPLACE(0x552f30, PackSrc_Read);

// 0x552fa0. Pack_Compress's write callback: appends n bytes at the output
// (context: buffer +0xc, used +0x10, capacity +0x14) when they fit; the used
// count grows either way. n when copied, else 0.
int32_t __fastcall PackDst_Write(uint8_t* buf, int32_t n) {
    if (!n) return 0;
    int32_t* c = g_d93984;
    int32_t r = 0;
    if (n + c[4] <= c[5]) {
        memcpy((uint8_t*)(uintptr_t)c[3] + c[4], buf, (size_t)n);
        r = n;
    }
    c[4] += n;
    return r;
}
RT3_REPLACE(0x552fa0, PackDst_Write);

void __fastcall Nop552ff0() { __asm nop }
RT3_REPLACE(0x552ff0, Nop552ff0);

// 0x553000. Packs n bytes at src with the packer's method (+0: 0x91 LZSS, 0x92
// RLE) into a new buffer of n + 0x10 bytes (the stream context is 0xd93984,
// read/write callbacks 0x552f30 / 0x552fa0). On success *out / *outSize get the
// buffer and packed size (also stored at +4, with the original size at +8) and
// the packer's 1 comes back; otherwise both are cleared and the error returned
// (-5: output overflow, -6: no memory, -7: unknown method).
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize) {
    uint8_t* buf = ((uint8_t*)crt::malloc((size_t)(n + 0x10)));
    if (!buf) return -6;
    int32_t ctx[6] = { (int32_t)(uintptr_t)src, 0, n, (int32_t)(uintptr_t)buf, 0, n };
    g_d93984 = ctx;
    int32_t r;
    bool failed = false;
    if (p[0] == 0x91) r = Lzss_Pack((StreamFn)0x552f30, (StreamFn)0x552fa0);
    else if (p[0] == 0x92) r = Rle_Pack((StreamFn)0x552f30, (StreamFn)0x552fa0);
    else {
        r = -7;
        failed = true;
    }
    if (r != 1) failed = true;
    int32_t* c = g_d93984;
    if (c[4] > c[5]) {
        r = -5;
        failed = true;
    }
    if (failed) {
        *out = nullptr;
        *outSize = 0;
        crt::free(buf);
        return r;
    }
    F<int32_t>((void*)(uintptr_t)ctx[3], 8) = c[2];
    F<int32_t>((void*)(uintptr_t)ctx[3], 4) = c[4];
    *out = buf;
    *outSize = c[4];
    return r;
}
RT3_REPLACE(0x553000, Pack_Compress);

// 0x553120. Unpacks a Pack_Compress buffer (header 0xbabe, packed size +4,
// original size +8, method word +0xe) of n bytes (n <= 0: the header's size) into
// a new buffer; *out / *outSize get it and its length and the unpacker's 1 comes
// back, else both are cleared and the error returned (-4: no header, -6: no
// memory, -7: unknown method). The packer's method +0 is restored afterwards.
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize) {
    if (src[0] != 0xbabe) return -4;
    uint8_t* buf = ((uint8_t*)crt::malloc((size_t)src[2]));
    if (!buf) return -6;
    int32_t ctx[6] = { (int32_t)(uintptr_t)src, 0, n > 0 ? n : src[1], (int32_t)(uintptr_t)buf, 0, src[2] };
    int32_t saved = p[0];
    g_d93984 = ctx;
    int32_t method = ((const uint16_t*)src)[7];
    p[0] = method;
    int32_t r;
    if (method == 0x91) r = Lzss_Unpack((StreamFn)0x552f30, (StreamFn)0x552fa0);
    else if (method == 0x92) r = Rle_Unpack((StreamFn)0x552f30, (StreamFn)0x552fa0);
    else r = -7;
    p[0] = saved;
    if (r != 1) {
        *out = nullptr;
        *outSize = 0;
        crt::free(buf);
        return r;
    }
    *out = buf;
    *outSize = g_d93984[4];
    return r;
}
RT3_REPLACE(0x553120, Pack_Decompress);

// 0x553240. Loads terrain t from stream s (texture `tex`): recreates it at
// the saved size, then heights +0x1605 (cell- or corner-sized by version
// tag 0x2afb, the missing last row copied), the two height layers +0x1655
// / +0x1659, the 3-byte cell flags +0x165d (packed from 1 or 2 bytes per
// cell in versions 0x2d49 / 0x2d50), maps +0x1661, +0x1665, +0x162d and
// the 16 x 16 cell block grids +0x1685. Returns the bytes read.
RT3_NOINLINE int32_t __fastcall Terrain_Load(uint8_t* t, int, Stream s, const char* tex) {
    int32_t total = Terrain_Tag(s, 0x2af9);
    Terrain_Pump();
    Terrain_Destroy(t);
    Terrain_Pump();
    uint32_t skip, version;
    total += StreamRead(s, &skip, 4, 0, 1);
    total += StreamRead(s, t + 0x15d9, 4, 0, 1);
    total += StreamRead(s, t + 0x15d5, 4, 0, 1);
    Terrain_Pump();
    Terrain_Create(t, 0, F<int32_t>(t, 0x15d9), F<int32_t>(t, 0x15d5), tex);
    total += Terrain_Tag(s, 0x2afa);
    Terrain_AllocMips(t);
    Terrain_Pump();
    StreamRead(s, &version, 4, 0, 1);
    int32_t n = version == 0x2afb ? F<int32_t>(t, 0x15d9) * F<int32_t>(t, 0x15d5) : (F<int32_t>(t, 0x15d9) + 1) * (F<int32_t>(t, 0x15d5) + 1);
    total += StreamRead(s, F<void*>(t, 0x1605), (uint32_t)n * 4, 0, 1);
    if (version == 0x2afb) {
        int32_t row = F<int32_t>(t, 0x15d9) + 1;
        uint32_t* h = F<uint32_t*>(t, 0x1605);
        memcpy(h + (F<int32_t>(t, 0x15d5) - 1) * row, h + (F<int32_t>(t, 0x15d5) - 2) * row, (size_t)row * 4);
    }
    total += Terrain_Tag(s, 0x2ced);
    total += StreamRead(s, F<void*>(t, 0x1655), (uint32_t)n, 0, 1);
    total += Terrain_Tag(s, 0x2cee);
    total += StreamRead(s, F<void*>(t, 0x1659), (uint32_t)n, 0, 1);
    StreamRead(s, &version, 4, 0, 1);
    uint32_t packed = (uint32_t)s;
    if (version == 0x2d49 || version == 0x2d50) {
        memset(F<void*>(t, 0x165d), 0, (size_t)n * 3);
        for (int32_t k = 0, o = 0; k < n; ++k, o += 3) {
            total += StreamRead(s, &packed, version == 0x2d49 ? 1 : 2, 0, 1);
            uint8_t b = (uint8_t)packed;
            uint8_t* p = F<uint8_t*>(t, 0x165d) + o;
            uint8_t hi = version == 0x2d49 ? (uint8_t)(b >> 3) : (uint8_t)((uint8_t)(packed >> 7) << 4);
            p[2] ^= (hi ^ p[2]) & 0x10;
            p = F<uint8_t*>(t, 0x165d) + o;
            p[2] ^= ((uint8_t)(b >> 3) ^ p[2]) & 0xf;
            uint16_t* w = (uint16_t*)(F<uint8_t*>(t, 0x165d) + o);
            *w = (uint16_t)((*w & 0xfff8) | (b & 7));
        }
    } else {
        total += StreamRead(s, F<void*>(t, 0x165d), (uint32_t)n * 3, 0, 1);
    }
    total += Terrain_Tag(s, 0x2d4a);
    total += Terrain_Tag(s, 0x2cf2);
    total += StreamRead(s, F<void*>(t, 0x1661), (uint32_t)n, 0, 1);
    total += Terrain_Tag(s, 0x2cf3);
    total += StreamRead(s, F<void*>(t, 0x1665), (uint32_t)n, 0, 1);
    total += Terrain_Tag(s, 0x2afc);
    StreamRead(s, F<void*>(t, 0x162d), (uint32_t)n, 0, 1); // not counted, as in the original
    Terrain_Pump();
    Terrain_InitMaps(t, 0, 1, 0, 0);
    total += Terrain_Tag(s, 0x2afd);
    Terrain_Pump();
    if ((int32_t)(F<uint32_t>(t, 0x15d9) & 0xfffffff0) > 0) {
        for (int32_t i = 0, x = 0; i < F<int32_t>(t, 0x15d9) >> 4; ++i, x += 0x5c0) {
            Terrain_Pump();
            if ((int32_t)(F<uint32_t>(t, 0x15d5) & 0xfffffff0) <= 0) continue;
            for (int32_t j = 0, y = 0; j < F<int32_t>(t, 0x15d5) >> 4; ++j, y += 0x5c0) {
                total += StreamRead(s, &packed, 1, 0, 1);
                if (!(uint8_t)packed) continue;
                uint8_t* g = ((uint8_t*)crt::operator_new(0x54));
                g = g ? BlockGrid_Init(g, 0, (float)x, (float)y) : nullptr;
                F<uint8_t**>(t, 0x1685)[(F<int32_t>(t, 0x15d9) >> 4) * j + i] = g;
                BlockGrid_Load(F<uint8_t**>(t, 0x1685)[(F<int32_t>(t, 0x15d9) >> 4) * j + i], 0, s);
            }
        }
    }
    total += Terrain_Tag(s, 0x2afe);
    return Terrain_Tag(s, 0x2b02) + total;
}
RT3_REPLACE(0x553240, Terrain_Load);

// 0x553760. Saves terrain t to stream s: its tile texture next to `name`
// (as .jpg with the engine's +0x11471a, quality `quality`, else .tga), then
// version 0xbba, the size, heights (+0x1605), land, +0x1659, cell bytes,
// flow (+0x1661 / +0x1665), +0x162d and each 16 x 16 block's grid (+0x1685,
// a presence byte first), between section tags.
void __fastcall Terrain_Save(uint8_t* t, int, int32_t s, const char* name, int32_t quality) {
    char path[0x130];
    strcpy(path, name);
    StripExtension(path);
    strcat(path, F<int32_t>(g_engine, 0x11471a) ? (const char*)0x5dd274 : (const char*)0x5dd27c);
    Terrain_SaveTexture(t, 0, path, F<int32_t>(g_engine, 0x11471a), quality);
    StreamWriteU32(s, 0x2af9);
    uint32_t version = 0xbba;
    StreamWrite(s, &version, 4, 0);
    StreamWrite(s, t + 0x15d9, 4, 0);
    StreamWrite(s, t + 0x15d5, 4, 0);
    StreamWriteU32(s, 0x2afa);
    uint32_t n = (uint32_t)((F<int32_t>(t, 0x15d5) + 1) * (F<int32_t>(t, 0x15d9) + 1));
    StreamWriteU32(s, 0x2afc);
    StreamWrite(s, F<void*>(t, 0x1605), n * 4, 0);
    StreamWriteU32(s, 0x2ced);
    StreamWrite(s, F<void*>(t, 0x1655), n, 0);
    StreamWriteU32(s, 0x2cee);
    StreamWrite(s, F<void*>(t, 0x1659), n, 0);
    StreamWriteU32(s, 0x2d51);
    StreamWrite(s, F<void*>(t, 0x165d), n * 3, 0);
    StreamWriteU32(s, 0x2d4a);
    StreamWriteU32(s, 0x2cf2);
    StreamWrite(s, F<void*>(t, 0x1661), n, 0);
    StreamWriteU32(s, 0x2cf3);
    StreamWrite(s, F<void*>(t, 0x1665), n, 0);
    StreamWriteU32(s, 0x2afc);
    StreamWrite(s, F<void*>(t, 0x162d), n, 0);
    StreamWriteU32(s, 0x2afd);
    for (int32_t i = 0; i < F<int32_t>(t, 0x15d9) >> 4; ++i)
        for (int32_t j = 0; j < F<int32_t>(t, 0x15d5) >> 4; ++j) {
            uint8_t* g = F<uint8_t**>(t, 0x1685)[(F<int32_t>(t, 0x15d9) >> 4) * j + i];
            uint8_t has = g != nullptr;
            StreamWrite(s, &has, 1, 0);
            if (uint8_t* g2 = F<uint8_t**>(t, 0x1685)[(F<int32_t>(t, 0x15d9) >> 4) * j + i]) BlockGrid_Save(g2, 0, s);
        }
    StreamWriteU32(s, 0x2afe);
    StreamWriteU32(s, 0x2b02);
}
RT3_REPLACE(0x553760, Terrain_Save);

// 0x553a00. Clears +0x20 and drops the pack resource +4 (kept in the field).
void __fastcall Sprite_ReleaseImage(uint8_t* s) {
    F<int32_t>(s, 0x20) = 0;
    Pack_Release(g_pack, 0, F<void*>(s, 4));
}
RT3_REPLACE(0x553a00, Sprite_ReleaseImage);

RT3_NOINLINE void __fastcall Flags24_Set1(uint8_t* o) { o[0x24] = (uint8_t)((o[0x24] & 0xf1) | 1); }
RT3_REPLACE(0x553a20, Flags24_Set1);

RT3_NOINLINE void __fastcall Flags24_Clear(uint8_t* o) { o[0x24] &= 0xf0; }
RT3_REPLACE(0x553a30, Flags24_Clear);

// 0x553a40. Stores v clamped to [0, 1] in +0x25.
RT3_NOINLINE void __fastcall Obj_SetUnit25(uint8_t* o, int, float v) {
    if (v < *(const float*)0x5c8598) {
        F<uint32_t>(o, 0x25) = 0;
        return;
    }
    if ((double)v > *(const double*)0x5c8770) {
        F<uint32_t>(o, 0x25) = 0x3f800000;
        return;
    }
    F<float>(o, 0x25) = v;
}
RT3_REPLACE(0x553a40, Obj_SetUnit25);

uint8_t __fastcall Flags24_Low(uint8_t* o) { return o[0x24] & 0xf; }
RT3_REPLACE(0x553a80, Flags24_Low);

uint32_t __fastcall Get_d93990() { return g_d93990; }
RT3_REPLACE(0x553a90, Get_d93990);

RT3_NOINLINE void ReleaseResourceD93990() { ReleaseResourceIn(0xd93990); }
RT3_REPLACE(0x553aa0, ReleaseResourceD93990);

// 0x553ac0. Sets up a camera from the collision resource `name` (frame `owner`):
// for a two-point shape, the view vector (+0x14..+0x1c) runs from point 0 to
// point 1, and the half extents +0xc / +0x10 come from its length and the tangents
// of the shape's angles (+0xc, +0x10 of the shape, scaled to radians).
RT3_NOINLINE uint8_t* __fastcall Camera_Construct(uint8_t* c, int, const char* name, uint32_t owner) {
    F<uint32_t>(c, 4) = 0;
    F<uint32_t>(c, 8) = 0;
    F<uint32_t>(c, 0) = owner;
    c[0x24] = 1;
    F<uint32_t>(c, 0x25) = 0x3f800000;
    F<uint32_t>(c, 0x29) = 0;
    F<uint32_t>(c, 0x2d) = 0;
    F<uint32_t>(c, 0x31) = 0;
    uint8_t* res = Pack_AcquireCollision(g_pack, 0, name);
    F<uint8_t*>(c, 4) = res;
    uint8_t* shape = F<uint8_t*>(F<uint8_t*>(res, 0), 0x10);
    F<uint8_t*>(c, 8) = shape;
    if (F<int32_t>(shape, 4) == 2) {
        const float* pt = F<const float*>(shape, 0);
        F<float>(c, 0x14) = (float)((double)pt[3] - pt[0]);
        pt = F<const float*>(F<uint8_t*>(c, 8), 0);
        F<float>(c, 0x18) = (float)((double)pt[4] - pt[1]);
        pt = F<const float*>(F<uint8_t*>(c, 8), 0);
        double dz = (double)pt[5] - pt[2];
        F<float>(c, 0x1c) = (float)dz;
        double x = F<float>(c, 0x14), y = F<float>(c, 0x18);
        float len = (float)sqrt((x * x + y * y) + dz * dz);
        shape = F<uint8_t*>(c, 8);
        F<float>(c, 0xc) = (float)(FpTan(Render_ScaleByUnit(shape, 0, F<float>(shape, 0xc))) * len);
        double t = FpTan(Render_ScaleByUnit(F<uint8_t*>(c, 8), 0, F<float>(F<uint8_t*>(c, 8), 0x10))) * len;
        F<float>(c, 0x10) = (float)(t * ((double)Fl(0x5c85b4) / F<float>(c, 0xc)));
    }
    F<int32_t>(c, 0x20) = 1;
    return c;
}
RT3_REPLACE(0x553ac0, Camera_Construct);

// 0x553ba0. Emits a particle (+8) of the batch as a camera-facing quad (6 vertices;
// corner offsets 0xd97abc.. x 10 x size +0x14 x scale +0x25) at its position (less
// the batch origin +0x29) in the object's frame (+0 -> +0xce), and unless it is a
// point (+4 == 1) a streak along its velocity (+0x14.., width +0xc, length factor
// +0x10, faded tail). Vertices are x, y, z, colour (+8), u, v; *count += 6 each.
void __fastcall Particle_EmitQuads(uint8_t* pd, int, float* out, int32_t* count) {
    uint8_t* part = F<uint8_t*>(pd, 8);
    double size = (double)F<float>(part, 0x14) * F<float>(pd, 0x25);
    if (!(size > Fl(0x5c8598)) && size == size) return;
    const float* pos = F<float*>(part, 0);
    double x = (double)pos[0] - F<float>(pd, 0x29), y = (double)pos[1] - F<float>(pd, 0x2d),
           z = (double)pos[2] - F<float>(pd, 0x31);
    float hs = (float)(size * Fl(0x5c85f8));
    const float* m = (const float*)(F<uint8_t*>(pd, 0) + 0xce);
    float wx = (float)((x * m[0] + z * m[8]) + y * m[4] + m[12]);
    float wy = (float)((x * m[1] + z * m[9]) + y * m[5] + m[13]);
    float wz = (float)((x * m[2] + z * m[10]) + y * m[6] + m[14]);
    *count += 6;
    uint32_t col = F<uint32_t>(part, 8);
    auto g = [](uint32_t a) { return (double)*(const float*)a; };
    auto put = [&](float* v, double cx, double cy, double cz, uint32_t c, uint32_t u, uint32_t w) {
        v[0] = (float)cx;
        v[1] = (float)cy;
        v[2] = (float)cz;
        ((uint32_t*)v)[3] = c;
        ((uint32_t*)v)[4] = u;
        ((uint32_t*)v)[5] = w;
    };
    auto corner = [&](float* v, uint32_t ax, uint32_t ay, uint32_t az, uint32_t u, uint32_t w) {
        put(v, g(ax) * hs + wx, g(ay) * hs + wy, g(az) * hs + wz, col, u, w);
    };
    corner(out, 0xd97ae0, 0xd97ac4, 0xd97ae8, 0, 0x3f000000);
    corner(out + 6, 0xd97ad4, 0xd97acc, 0xd97ad8, 0x3f800000, 0x3f000000);
    corner(out + 12, 0xd97ac0, 0xd97ae4, 0xd97abc, 0, 0x3f800000);
    corner(out + 18, 0xd97ad4, 0xd97acc, 0xd97ad8, 0x3f800000, 0x3f000000);
    corner(out + 24, 0xd97ad0, 0xd97adc, 0xd97ac8, 0x3f800000, 0x3f800000);
    corner(out + 30, 0xd97ac0, 0xd97ae4, 0xd97abc, 0, 0x3f800000);
    if (F<int32_t>(F<uint8_t*>(pd, 8), 4) == 1) return;
    double vx = F<float>(pd, 0x14), vy = F<float>(pd, 0x18), vz = F<float>(pd, 0x1c);
    float rx = (float)((vx * m[0] + vz * m[8]) + vy * m[4]);
    float ry = (float)((vx * m[1] + vz * m[9]) + vy * m[5]);
    float rz = (float)((vx * m[2] + vz * m[10]) + vy * m[6]);
    float cam[3];
    Obj_GetXYZ1c(F<uint8_t*>(g_engine, 0x28), 0, (uint32_t*)&cam[0], (uint32_t*)&cam[1],
                 (uint32_t*)&cam[2]);
    float dx = (float)((double)cam[0] - wx), dy = (float)((double)cam[1] - wy), dz = (float)((double)cam[2] - wz);
    float cx, cy, cz;
    Vec_Cross(&cx, &cy, rx, ry, rz, dx, dy, dz, &cz);
    double inv = *(const double*)0x5c8770 / Sqrt87((double)cx * cx + (double)cy * cy + (double)cz * cz);
    double k = (double)F<float>(pd, 0xc) * F<float>(pd, 0x25) * inv;
    *count += 6;
    double px = cx * k;
    float py = (float)(k * cy);
    float pz = (float)(cz * k);
    float hx = (float)((double)rx * F<float>(pd, 0x25) + wx);
    float hy = (float)((double)ry * F<float>(pd, 0x25) + wy);
    float hz = (float)((double)rz * F<float>(pd, 0x25) + wz);
    uint32_t half = ((col >> 1) & 0x7f000000) | (col & 0xffffff);
    float* v = out + 36;
    put(v, wx, wy, wz, col, 0, 0x3e800000);
    put(v + 6, hx + px, (double)hy + py, (double)hz + pz, col, 0x3f7d70a4, 0);
    put(v + 12, hx - px, (double)hy - py, (double)hz - pz, col, 0x3f7d70a4, 0x3f000000);
    double qx = px * F<float>(pd, 0x10);
    double qy = (double)py * F<float>(pd, 0x10);
    float qz = (float)((double)pz * F<float>(pd, 0x10));
    put(v + 18, wx, wy, wz, half, 0, 0x3e800000);
    put(v + 24, hx + qx, hy + qy, (double)hz + qz, half, 0x3f7d70a4, 0);
    put(v + 30, hx - qx, hy - qy, (double)hz - qz, half, 0x3f7d70a4, 0x3f000000);
}
RT3_REPLACE(0x553ba0, Particle_EmitQuads);

}  // namespace rt3
