// Fog render states in block 0x54 (batch 189). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Render_FogOn(uint8_t* r);

static inline void RenderState(uint8_t* r, uint32_t state, uint32_t value) {
    void* dev = F<void*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t))(*(uint8_t**)dev + 0xc8))(dev, state, value);
}
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
static inline int32_t Clamp255(int32_t v) { return v < 0 ? 0 : v > 0xff ? 0xff : v; }

// 0x54a280. Sets the fog (density `density`, colour +0x2510 scaled by r/g/b) when
// the device supports it (caps +0x83c +0x2e): off when r <= 0; table or vertex fog
// (caps +0x1f), linear from start to end (caps +0x20, from +0x2513) or exponential.
void __fastcall Renderer_SetFog(uint8_t* r, int, float density, float cr, float cg, float cb) {
    F<float>(r, 0x25b3) = density;
    if (!F<uint8_t*>(r, 0x83c)[0x2e]) return;
    if (!r[0x2549]) {
        r[0x2549] = 1;
        RenderState(r, 0x1c, 1);
    }
    if (cr <= *(const float*)0x5c8598) {
        if (r[0x2549]) {
            r[0x2549] = 0;
            RenderState(r, 0x1c, 0);
        }
        return;
    }
    if (!r[0x2549] && F<uint8_t*>(r, 0x83c)[0x2e]) Render_FogOn(r);
    int32_t red = Clamp255(Ftol((double)(int32_t)r[0x2510] * cr));
    int32_t green = Clamp255(Ftol((double)(int32_t)r[0x2511] * cg));
    int32_t blue = Clamp255(Ftol((double)(int32_t)r[0x2512] * cb));
    RenderState(r, 0x22, ((((uint32_t)red | 0xffffff00) << 8 | (uint32_t)green) << 8) | (uint32_t)blue);
    RenderState(r, 0x30, 1);
    uint32_t mode = F<uint8_t*>(r, 0x83c)[0x20] ? 3 : 2;
    if (F<uint8_t*>(r, 0x83c)[0x1f]) {
        RenderState(r, 0x23, 0);
        RenderState(r, 0x8c, mode);
    } else {
        RenderState(r, 0x8c, 0);
        RenderState(r, 0x23, mode);
    }
    if (F<uint8_t*>(r, 0x83c)[0x20]) {
        double x = 1.0 / ((double)density * F<float>(r, 0x2513));
        float start = (float)(*(const double*)0x5ddaf0 * x);
        double endD = x * *(const float*)0x5ddaec;
        float end = (float)endD;
        if (endD > *(const float*)0x5ddae8) end = 65535.0f;
        if (start > end) start = (float)((double)end * *(const double*)0x5c8750);
        RenderState(r, 0x24, *(uint32_t*)&start);
        RenderState(r, 0x25, *(uint32_t*)&end);
    } else {
        float d = (float)((double)density * *(const float*)0x5ddae4);
        RenderState(r, 0x26, *(uint32_t*)&d);
    }
}
RT3_REPLACE(0x54a280, Renderer_SetFog);

} // namespace rt3

namespace rt3 {

double __fastcall Font_GlyphAdvance(uint8_t* f, int, uint8_t ch);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);

static inline bool Over(float w, float lim) { return w > lim; }

// 0x5501a0. Copies as much of `text` (up to a newline) as fits in `maxW` into
// `out`, setting *width and *next (after the copied part). Normally it breaks at the
// last space / '-' / '\' / '/' that fits (or where the width first fitted), dropping
// a trailing space; with `ellipsis` it makes room for "..." and appends it; with
// `clipFront` it drops characters from the front instead.
void __fastcall Font_FitText(uint8_t* font, int, const char* text, char* out, float maxW, int32_t ellipsis, float* width,
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

} // namespace rt3
