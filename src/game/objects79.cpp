// Named object / renderer setting accessors in blocks 0x45..0x53 (batch 82). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint8_t* __fastcall SceneObj_Construct(uint8_t* o);
int32_t __fastcall SceneObj_Reset_Impl(uint8_t* o);
int32_t __fastcall NamedObj_Reset(uint8_t* o);
void __fastcall NamedObj_Save(uint8_t* o, int, Stream s);

// 0x455b20. Named scene object constructor.
uint8_t* __fastcall NamedObj_Construct(uint8_t* o) {
    SceneObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5cb1c0;
    F<int32_t>(o, 0x206) = 0;
    F<int32_t>(o, 0x20a) = 0;
    F<int32_t>(o, 0x20e) = 0;
    F<int32_t>(o, 0x22e) = 0;
    F<int32_t>(o, 0x232) = 0;
    for (int k = 0; k < 7; ++k) F<int32_t>(o, 0x212 + k * 4) = 0;
    SceneObj_Reset_Impl(o);
    return o;
}
RT3_REPLACE(0x455b20, NamedObj_Construct);

// 0x456630. Saves a model: chunk 0x6979, the named object, its own data (vtbl +0x4c)
// as chunk 0x697a, closing with 0x697b.
void __fastcall ModelObj_Save(uint8_t* o, int, Stream s) {
    StreamWriteU32(s, 0x6979);
    NamedObj_Save(o, 0, s);
    StreamWriteU32(s, 0x697a);
    ((void(__thiscall*)(void*, Stream))(*(void***)o)[0x4c / 4])(o, s);
    StreamWriteU32(s, 0x697b);
}
RT3_REPLACE(0x456630, ModelObj_Save);

// 0x45b050. Clears the model state fields, also resetting the named object when asked.
int32_t __fastcall ModelObj_ResetState(uint8_t* o, int, int32_t full) {
    F<int32_t>(o, 0x24e) = 0;
    F<int32_t>(o, 0x23e) = 0;
    F<int32_t>(o, 0x242) = 0;
    F<int32_t>(o, 0x246) = 0;
    F<int32_t>(o, 0x24a) = 0;
    F<int32_t>(o, 0x262) = -1;
    F<int32_t>(o, 0x266) = 0;
    F<int32_t>(o, 0x26a) = 0;
    F<uint16_t>(o, 0x23b) = 0;
    o[0x23a] = 0;
    o[0x23d] = 0;
    if (!full) return 0;
    return NamedObj_Reset(o);
}
RT3_REPLACE(0x45b050, ModelObj_ResetState);

// 0x532310 / 0x532360 / 0x532370 / 0x532380. Seven-value setting at +0x15b5
// (enabled flag +0x15b4).
void __fastcall Render_SetFog(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                              uint32_t f, uint32_t g) {
    F<uint32_t>(r, 0x15b5) = a;
    F<uint32_t>(r, 0x15b9) = b;
    F<uint32_t>(r, 0x15bd) = c;
    F<uint32_t>(r, 0x15c1) = d;
    F<uint32_t>(r, 0x15c5) = e;
    r[0x15b4] = 1;
    F<uint32_t>(r, 0x15c9) = f;
    F<uint32_t>(r, 0x15cd) = g;
}
RT3_REPLACE(0x532310, Render_SetFog);
void __fastcall Render_DisableFog(uint8_t* r) { r[0x15b4] = 0; }
RT3_REPLACE(0x532360, Render_DisableFog);
uint8_t __fastcall Render_FogEnabled(uint8_t* r) { return r[0x15b4]; }
RT3_REPLACE(0x532370, Render_FogEnabled);
void __fastcall Render_GetFog(uint8_t* r, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d, uint32_t* e,
                              uint32_t* f, uint32_t* g) {
    *a = F<uint32_t>(r, 0x15b5);
    *c = F<uint32_t>(r, 0x15bd);
    *b = F<uint32_t>(r, 0x15b9);
    *d = F<uint32_t>(r, 0x15c1);
    *e = F<uint32_t>(r, 0x15c5);
    *f = F<uint32_t>(r, 0x15c9);
    *g = F<uint32_t>(r, 0x15cd);
}
RT3_REPLACE(0x532380, Render_GetFog);

// 0x5324e0 / 0x532500 / 0x532510. Flag +0x159b (only set when +0x478 exists) and byte +0x159a.
int32_t __fastcall Render_Enable159b(uint8_t* r) {
    int32_t v = F<int32_t>(r, 0x478);
    if (v) r[0x159b] = 1;
    return v;
}
RT3_REPLACE(0x5324e0, Render_Enable159b);
void __fastcall Render_Disable159b(uint8_t* r) { r[0x159b] = 0; }
RT3_REPLACE(0x532500, Render_Disable159b);
void __fastcall Render_Set159a(uint8_t* r, int, uint8_t v) { r[0x159a] = v; }
RT3_REPLACE(0x532510, Render_Set159a);

// 0x532520 / 0x532550. Four-value setting at +0x15a4.
void __fastcall Render_SetRect15a4(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    F<uint32_t>(r, 0x15a4) = a;
    F<uint32_t>(r, 0x15a8) = b;
    F<uint32_t>(r, 0x15ac) = c;
    F<uint32_t>(r, 0x15b0) = d;
}
RT3_REPLACE(0x532520, Render_SetRect15a4);
void __fastcall Render_GetRect15a4(uint8_t* r, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
    *a = F<uint32_t>(r, 0x15a4);
    *b = F<uint32_t>(r, 0x15a8);
    *c = F<uint32_t>(r, 0x15ac);
    *d = F<uint32_t>(r, 0x15b0);
}
RT3_REPLACE(0x532550, Render_GetRect15a4);

// 0x532960. Converts (*a, *b) between grid units of the size W x H (+0x15d5 / +0x15d9):
// *a = (*a * 4 / (H / 16)) * (H / 4), *b = (*b * 4 / (W / 16)) * (W / 4).
void __fastcall Render_SnapToGrid(uint8_t* r, int, int32_t* a, int32_t* b) {
    int32_t w = F<int32_t>(r, 0x15d5), h = F<int32_t>(r, 0x15d9);
    *a = ((int32_t)((uint32_t)*a << 2) / (h / 16)) * (h / 4);
    *b = ((int32_t)((uint32_t)*b << 2) / (w / 16)) * (w / 4);
}
RT3_REPLACE(0x532960, Render_SnapToGrid);

// 0x5329e0. 1-based grid cell index of (x, y) in a 4-wide grid.
int32_t __fastcall Render_GridCell(uint8_t* r, int, int32_t x, int32_t y) {
    int32_t row = (int32_t)((uint32_t)y << 2) / (F<int32_t>(r, 0x15d5) / 16);
    int32_t col = (int32_t)((uint32_t)x << 2) / (F<int32_t>(r, 0x15d9) / 16);
    return col + row * 4 + 1;
}
RT3_REPLACE(0x5329e0, Render_GridCell);

} // namespace rt3
