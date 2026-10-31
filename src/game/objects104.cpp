// Ticker / overview target / control text / glyph width / sound channel in blocks 0x4c..0x55 (batch 107). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Rand(int32_t lo, int32_t hi);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
void __fastcall Render_DestroyCurrentTexture(uint8_t* r);
uint8_t* __fastcall Texture_ConstructDefault(uint8_t* t);
int32_t __fastcall Texture_CreateTarget(uint8_t* t, int, int32_t w, int32_t h, void* dev);
const char* __fastcall GetString(int32_t id);
void __fastcall String_Assign(char** s, const char* src);
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }

// 0x4c7ad0. Picks the next two ticker entries (+0x78 values, +0x7c cool-downs of
// 31 slots) avoiding recent ones; the second is dropped when it would not fit.
RT3_NOINLINE void __fastcall Ticker_PickNext(uint8_t* t) {
    for (int k = 0; k < 0x1f; ++k)
        if (F<int32_t>(t, 0x7c + k * 8) > 0) F<int32_t>(t, 0x7c + k * 8)--;
    int32_t prev = -1;
    for (int n = 0; n < 2; ++n) {
        int32_t r;
        do {
            r = Rand(0, 0x1e);
        } while (F<int32_t>(t, 0x7c + r * 8) || r == prev);
        F<uint32_t>(t, 0x17c + n * 4) = F<uint32_t>(t, 0x78 + r * 8);
        prev = r;
        F<int32_t>(t, 0x7c + r * 8) = 5;
    }
    float room = (float)((double)*(const float*)0x5d0998 -
                         Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0)));
    double w = Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x180), 0, 0));
    if (w > room || F<int32_t>(t, 0x178) == 1) F<int32_t>(t, 0x180) = 0;
    if (++F<int32_t>(t, 0x178) == 3) F<int32_t>(t, 0x178) = 0;
}
RT3_REPLACE(0x4c7ad0, Ticker_PickNext);

int32_t ClockMs();
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t,
                            float, float, float, float);

static inline float Kf(uint32_t a) { return *(const float*)a; }

// 0x4c7c80. Draws the ticker at (x, y): picks new entries every 8 s (10 s with
// two), the first fading in over 2 s, holding to 6 s and out by 8 s; a second
// entry (+0x180) fades in at 2..4 s and out after 8 s, sitting below the first.
// Phase 2 (+0x178) swaps the two fades. Entries are centred on 0.375 and drawn
// from 0.95 up.
void __fastcall Ticker_Draw(uint8_t* t, int, float x, float y, float, float) {
    int32_t el = ClockMs() - F<int32_t>(t, 0x170);
    if (el > 8000 && (!F<int32_t>(t, 0x180) || el > 10000)) {
        Ticker_PickNext(t);
        F<int32_t>(t, 0x170) = ClockMs();
    }
    el = ClockMs() - F<int32_t>(t, 0x170);
    float fel = (float)el;
    float a1 = fel;
    if (!F<int32_t>(t, 0x180)) a1 = (float)(el < 8000 ? el : 8000);
    if (a1 < Kf(0x5c88e0)) a1 = (float)((double)a1 / Kf(0x5c88e0));
    else if (a1 < Kf(0x5c9b0c)) a1 = 1.0f;
    else if (a1 <= Kf(0x5ca1c4)) a1 = (float)(*(const double*)0x5c8770 - ((double)a1 - Kf(0x5c9b0c)) / Kf(0x5c88e0));
    else a1 = 0.0f;
    float a2 = fel;
    if (F<int32_t>(t, 0x180)) {
        if (a2 < Kf(0x5c88e0)) a2 = 0.0f;
        else if (!(a2 < Kf(0x5ca1c8)) && a2 < Kf(0x5ca1c4)) a2 = 1.0f;
        else if (a2 < Kf(0x5ca1c8)) a2 = (float)(((double)a2 - Kf(0x5c88e0)) / Kf(0x5c88e0));
        else a2 = (float)(*(const double*)0x5c8770 - ((double)a2 - Kf(0x5ca1c4)) / Kf(0x5c88e0));
    }
    float alpha = F<int32_t>(t, 0x178) == 2 && F<int32_t>(t, 0x180) ? a2 : a1;
    float x1 = (float)(((double)Kf(0x5d09b4) - Slider_StepA((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0))) / Kf(0x5c878c) + x);
    float top = (float)((double)Kf(0x5d09b0) - Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0)));
    if (F<int32_t>(t, 0x180))
        top = (float)((double)top - (Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x180), 0, 0)) - Kf(0x5d09ac)));
    y = (float)((double)top / Kf(0x5c878c) + y);
    Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0), 0, x1, y, *(uint32_t*)&alpha, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    if (!F<int32_t>(t, 0x180)) return;
    float alpha2 = F<int32_t>(t, 0x178) == 2 ? a1 : a2;
    float x2 = (float)(((double)Kf(0x5d09b4) - Slider_StepA((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x180), 0, 0))) / Kf(0x5c878c) + x);
    double h1 = Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x17c), 0, 0));
    float y2 = (float)(h1 + ((double)top - Kf(0x5d09a8)) / Kf(0x5c878c) + y);
    Glyph_Queue((uint8_t*)Table_Entry(F<uint8_t*>(t, 0x180), 0, 0), 0, x2, y2, *(uint32_t*)&alpha2, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f,
                1.0f);
}
RT3_REPLACE(0x4c7c80, Ticker_Draw);

// 0x534fc0. (Re)creates the current overview render target of size x size.
void __fastcall Render_CreateOverviewTarget(uint8_t* r, int, int32_t size) {
    if (!Engine()[0x11422e]) return;
    if (F<int32_t>(r, 0x1558)) {
        Render_DestroyCurrentTexture(r);
    } else {
        F<int32_t>(r, 0x1558) = F<int32_t>(r, 4);
        F<int32_t>(r, 4)++;
    }
    if (F<void*>(r, 8 + F<int32_t>(r, 0x1558) * 4)) return;
    uint8_t* p = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0xec);
    F<uint8_t*>(r, 8 + F<int32_t>(r, 0x1558) * 4) = p ? Texture_ConstructDefault(p) : nullptr;
    if (!Texture_CreateTarget(F<uint8_t*>(r, 8 + F<int32_t>(r, 0x1558) * 4), 0, size, size, F<void*>(Engine(), 8))) {
        Engine()[0x1146ef] = 0;
        Engine()[0x1146f0] = 0;
    }
}
RT3_REPLACE(0x534fc0, Render_CreateOverviewTarget);

// 0x540170. Sizes the control from its image (+0x92, entry +0xa2) when unset and
// loads its caption string (+0xd2) into +0xce.
void __fastcall Control_Refresh(uint8_t* c) {
    const float zero = *(const float*)0x5c8598;
    if ((F<float>(c, 0x13) == zero || F<float>(c, 0x17) == zero) && F<uint8_t*>(c, 0x92)) {
        F<float>(c, 0x13) = (float)Slider_StepA((uint8_t*)Table_Entry(F<uint8_t*>(c, 0x92), 0, F<int16_t>(c, 0xa2)));
        F<float>(c, 0x17) = (float)Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(c, 0x92), 0, F<int16_t>(c, 0xa2)));
    }
    if (uint16_t id = F<uint16_t>(c, 0xd2)) String_Assign((char**)(c + 0xce), GetString(id));
}
RT3_REPLACE(0x540170, Control_Refresh);

// 0x5500d0. Advance of glyph ch in the font (style +0xc, table 0x625480, 40 bytes):
// fixed-width digits when the style says so; 0 for control characters and '|'.
double __fastcall Font_GlyphAdvance(uint8_t* f, int, uint8_t ch) {
    if (ch == 0xf || ch == 0x7c || ch == 0xa || ch == 0xd) return *(const float*)0x5c8598;
    const uint8_t* style = (const uint8_t*)0x625480 + F<int32_t>(f, 0xc) * 40;
    if (F<int32_t>((void*)style, 0) && ch >= '0' && ch <= '9')
        return ((double)F<float>((void*)style, 4) + F<float>(f, 0x30)) * F<float>(f, 0x14);
    double w = Slider_StepA((uint8_t*)Table_Entry(F<uint8_t*>(f, 0x18), 0, ch));
    return (w + F<float>((void*)((const uint8_t*)0x625488 + F<int32_t>(f, 0xc) * 40), 0)) * F<float>(f, 0x14);
}
RT3_REPLACE(0x5500d0, Font_GlyphAdvance);

void __fastcall Flags24_Set1(uint8_t* o) { o[0x24] = (uint8_t)((o[0x24] & 0xf1) | 1); }
RT3_REPLACE(0x553a20, Flags24_Set1);
void __fastcall Flags24_Clear(uint8_t* o) { o[0x24] &= 0xf0; }
RT3_REPLACE(0x553a30, Flags24_Clear);
// 0x553a40. Stores v clamped to [0, 1] in +0x25.
void __fastcall Obj_SetUnit25(uint8_t* o, int, float v) {
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
uint32_t __fastcall Get_d93990() { return hook::Global<uint32_t>(0xd93990); }
RT3_REPLACE(0x553a90, Get_d93990);
void __fastcall Obj_Set8(uint8_t* o, int, uint32_t v) { F<uint32_t>(o, 8) = v; }
RT3_REPLACE(0x5540a0, Obj_Set8);

// Sound channel: sample key +0x110, position +0x114, volume +0x128, range
// +0x120/+0x124, rate +0x12c, dirty +0x134, loop +0x138. Samples come from the
// 3D sample table of the sound system 0x6d402c (+0x44 change flags).
uint32_t __fastcall Channel_Init(uint8_t* c) {
    F<int32_t>(c, 0xc) = 0;
    F<int32_t>(c, 0x110) = 0;
    F<uint32_t>(c, 0x128) = 0x3f800000;
    F<uint32_t>(c, 0x120) = 0x42400000;
    F<uint32_t>(c, 0x124) = 0x47000000;
    F<int32_t>(c, 0x134) = 0;
    F<int32_t>(c, 0x12c) = 0;
    F<int32_t>(c, 0x130) = 0;
    F<int32_t>(c, 0x138) = 0;
    F<int32_t>(c, 0) = 0;
    c[0x13c] = 0;
    F<int32_t>(c, 0x114) = 0;
    F<int32_t>(c, 0x118) = 0;
    F<int32_t>(c, 0x11c) = 0;
    return 0;
}
RT3_REPLACE(0x554140, Channel_Init);

// 0x5541a0. Copies all channel parameters to the sample, flagging them changed.
void __fastcall Channel_ApplyTo(uint8_t* c, int, uint8_t* s) {
    uint32_t fl = F<uint32_t>(s, 0x44);
    F<float>(s, 0x10) = F<float>(c, 0x114);
    F<float>(s, 0x14) = F<float>(c, 0x118);
    F<uint32_t>(s, 0x44) = fl | 2;
    F<float>(s, 0x18) = F<float>(c, 0x11c);
    F<uint32_t>(s, 0x3c) = F<uint32_t>(c, 0x12c);
    F<uint32_t>(s, 0x44) = fl | 0x82;
    F<uint32_t>(s, 0x44) = fl | 0x83;
    F<float>(s, 0x28) = F<float>(c, 0x128);
    F<float>(s, 0x30) = F<float>(c, 0x120);
    F<float>(s, 0x34) = F<float>(c, 0x124);
    F<uint32_t>(s, 0x44) = fl | 0x93;
    F<int32_t>(c, 0x134) = 0;
}
RT3_REPLACE(0x5541a0, Channel_ApplyTo);

// The channel's live sample, or 0 (forgetting it and marking the channel dirty).
static inline uint8_t* LiveSample(uint8_t* c) {
    uint8_t* s = (uint8_t*)Sound_LookupSample(hook::Global<uint8_t*>(0x6d402c), 0, F<uint32_t>(c, 0x110), 1);
    if (!s) {
        F<int32_t>(c, 0x110) = 0;
        F<int32_t>(c, 0x134) = 1;
    }
    return s;
}

int32_t __fastcall Channel_Check(uint8_t* c) {
    if (!F<uint32_t>(c, 0x110)) return 0;
    return (int32_t)LiveSample(c);
}
RT3_REPLACE(0x554230, Channel_Check);
uint8_t __fastcall Channel_Playing(uint8_t* c) { return c[0x13c] != 0; }
RT3_REPLACE(0x554270, Channel_Playing);

// 0x5542c0. Sets the playback rate (clamped to 8000..44100 Hz).
void __fastcall Channel_SetRate(uint8_t* c, int, int32_t rate) {
    if (F<int32_t>(c, 0x12c) == rate) return;
    int32_t r = rate < 0x1f40 ? 0x1f40 : rate > 0xac44 ? 0xac44 : rate;
    if (F<uint32_t>(c, 0x110)) {
        uint8_t* s = LiveSample(c);
        if (!s) {
            F<int32_t>(c, 0x12c) = r;
            return;
        }
        F<int32_t>(s, 0x3c) = r;
        F<uint32_t>(s, 0x44) |= 0x80;
    }
    F<int32_t>(c, 0x12c) = r;
}
RT3_REPLACE(0x5542c0, Channel_SetRate);

// 0x554340. Sets the min/max distance.
void __fastcall Channel_SetRange(uint8_t* c, int, uint32_t lo, uint32_t hi) {
    if (F<uint32_t>(c, 0x110)) {
        if (uint8_t* s = LiveSample(c)) {
            F<uint32_t>(s, 0x30) = lo;
            F<uint32_t>(s, 0x34) = hi;
            F<uint32_t>(s, 0x44) |= 0x10;
        }
    }
    F<uint32_t>(c, 0x120) = lo;
    F<uint32_t>(c, 0x124) = hi;
}
RT3_REPLACE(0x554340, Channel_SetRange);

// 0x5543a0. Sets the sample's value +0x50 (flag 0x400).
void __fastcall Channel_SetSample50(uint8_t* c, int, uint32_t v) {
    if (!F<uint32_t>(c, 0x110)) return;
    uint8_t* s = LiveSample(c);
    if (!s) return;
    F<uint32_t>(s, 0x50) = v;
    F<uint32_t>(s, 0x44) |= 0x400;
}
RT3_REPLACE(0x5543a0, Channel_SetSample50);

// 0x5543f0. Sets the loop count (+0x138, sample +0xc, flag 8).
void __fastcall Channel_SetLoop(uint8_t* c, int, uint32_t v) {
    if (F<uint32_t>(c, 0x138) == v) return;
    if (F<uint32_t>(c, 0x110)) {
        uint8_t* s = LiveSample(c);
        if (!s) {
            F<uint32_t>(c, 0x138) = v;
            return;
        }
        F<uint32_t>(s, 0xc) = v;
        F<uint32_t>(s, 0x44) |= 8;
    }
    F<uint32_t>(c, 0x138) = v;
}
RT3_REPLACE(0x5543f0, Channel_SetLoop);

} // namespace rt3
