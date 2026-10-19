// Named object constructors / fog mode / renderer buffers in blocks 0x41..0x53 (batch 83). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

uint8_t* __fastcall NamedObj_Construct(uint8_t* o);
int32_t __fastcall NamedObj_Reset(uint8_t* o);
void __fastcall Render_SetFog(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                              uint32_t f, uint32_t g);
void __fastcall Render_DisableFog(uint8_t* r);

// Named object subclasses: construct the base, then set the vtable and own fields.
uint8_t* __fastcall NamedObj41f590_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5c9a28;
    return o;
}
RT3_REPLACE(0x41f590, NamedObj41f590_Construct);
uint8_t* __fastcall NamedObj456900_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5cb250;
    return o;
}
RT3_REPLACE(0x456900, NamedObj456900_Construct);

// 0x45b680. Model object: same state fields as ModelObj_ResetState.
uint8_t* __fastcall ModelObj_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5cb4c0;
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
    NamedObj_Reset(o);
    return o;
}
RT3_REPLACE(0x45b680, ModelObj_Construct);

// 0x45c890. Multi-part model (parts +0x316, count +0x31a).
uint8_t* __fastcall PartModel_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5cb5e8;
    F<int32_t>(o, 0x316) = 0;
    F<int32_t>(o, 0x31a) = 0;
    F<int32_t>(o, 0x32e) = 0;
    F<int32_t>(o, 0x332) = 0;
    F<int32_t>(o, 0x336) = 0;
    NamedObj_Reset(o);
    return o;
}
RT3_REPLACE(0x45c890, PartModel_Construct);

uint8_t* __fastcall NamedObj4611e0_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5cbc98;
    F<uint32_t>(o, 0x2a2) = 0x787878;
    o[0x23e] = 0;
    return o;
}
RT3_REPLACE(0x4611e0, NamedObj4611e0_Construct);
uint8_t* __fastcall NamedObj474110_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<int32_t>(o, 0x242) = 0;
    F<int32_t>(o, 0x23a) = 0;
    F<uint32_t>(o, 0) = 0x5ce428;
    return o;
}
RT3_REPLACE(0x474110, NamedObj474110_Construct);

// 0x452ca0. Sets the fog mode (+0x2171): range 0x40 (none / > 6), 4 (1..5) or 1 (6),
// with the map size (0x62c120 +0x2155/+0x2159). Mode 1 (+0x219d) counts as an
// extra engine user (+0x114272 / +0x114266) while +0x21a1 is set.
void __fastcall Map_SetFogMode(uint8_t* m, int, int32_t mode) {
    if (mode == 1 && F<int32_t>(m, 0x219d) != 1) {
        int32_t active = F<int32_t>(m, 0x21a1);
        F<int32_t>(m, 0x219d) = 1;
        if (active) {
            F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114272)++;
            F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114266)++;
        }
    }
    F<int32_t>(m, 0x2171) = mode;
    int32_t v = 0x40;
    if (mode > 0) {
        if (mode <= 5) v = 4;
        else if (mode == 6) v = 1;
    }
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    Render_SetFog(m, 0, v / 2, v / 2, F<uint32_t>(map, 0x2155), F<uint32_t>(map, 0x2159), v, v, 9999);
}
RT3_REPLACE(0x452ca0, Map_SetFogMode);

// 0x452d30. Clears the fog mode (undoing the mode-1 engine count).
void __fastcall Map_ClearFogMode(uint8_t* m) {
    if (F<int32_t>(m, 0x2171) == 1 && F<int32_t>(m, 0x219d)) {
        int32_t active = F<int32_t>(m, 0x21a1);
        F<int32_t>(m, 0x219d) = 0;
        if (active) {
            F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114272)--;
            F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114266)--;
        }
    }
    F<int32_t>(m, 0x2171) = 0;
    Render_DisableFog(m);
}
RT3_REPLACE(0x452d30, Map_ClearFogMode);

uint32_t __fastcall Render_Get1554(uint8_t* r) { return F<uint32_t>(r, 0x1554); }
RT3_REPLACE(0x532a30, Render_Get1554);
uint32_t __fastcall Render_Get1578(uint8_t* r) { return F<uint32_t>(r, 0x1578); }
RT3_REPLACE(0x532a90, Render_Get1578);

// 0x532a40. Current frame of animated texture set A (+0x1568, +0x156c frames) from
// the system time x 0x5dd258 (x87 with the exe's _CIfmod / _ftol); index kept in +0x1570.
uint32_t __fastcall Render_AnimFrameA(uint8_t* r) {
    int32_t t = (int32_t)Import<uint32_t(__stdcall*)()>(0x5c8068)();
    int32_t n = F<int32_t>(r, 0x156c);
    int32_t idx;
    __asm {
        fild t
        mov eax, t
        test eax, eax
        jge positive
        fadd dword ptr ds:[0x5c872c]
    positive:
        fmul dword ptr ds:[0x5dd258]
        fild n
        mov eax, 0x5a139a
        call eax
        mov eax, 0x5a10d0
        call eax
        mov idx, eax
    }
    F<int32_t>(r, 0x1570) = idx;
    return F<uint32_t>(r, (F<int32_t>(r, 0x1568) + idx) * 4 + 8);
}
RT3_REPLACE(0x532a40, Render_AnimFrameA);

// 0x532aa0. Current frame of animated texture set B (+0x1560, +0x1564 frames): time / 12.
uint32_t __fastcall Render_AnimFrameB(uint8_t* r) {
    uint32_t t = Import<uint32_t(__stdcall*)()>(0x5c8068)();
    uint32_t idx = (t / 12) % F<uint32_t>(r, 0x1564);
    return F<uint32_t>(r, (F<int32_t>(r, 0x1560) + idx) * 4 + 8);
}
RT3_REPLACE(0x532aa0, Render_AnimFrameB);

// 0x532c80. (Re)allocates the terrain grid buffers for (rows + 1) x (cols + 1)
// vertices: heights +0x1605 (zeroed), four layers +0x1631.., +0x1655 (2s),
// +0x1659 (1s) and +0x165d (3 bytes each, zeroed).
void __fastcall Render_AllocGrid(uint8_t* r, int, int32_t rows, int32_t cols) {
    F<int32_t>(r, 0x15e1) = 0;
    if (rows && cols) {
        F<int32_t>(r, 0x15d9) = rows;
        F<int32_t>(r, 0x15d5) = cols;
        F<int32_t>(r, 0x15dd) = rows + 1;
    }
    uint32_t n = (uint32_t)((F<int32_t>(r, 0x15d9) + 1) * (F<int32_t>(r, 0x15d5) + 1));
    uint8_t* h = (uint8_t*)Malloc(n * 4);
    F<uint8_t*>(r, 0x1605) = h;
    for (uint32_t k = 0; k < n * 4; ++k) h[k] = 0;
    for (int k = 0; k < 4; ++k) F<void*>(r, 0x1631 + k * 4) = Malloc(n);
    F<void*>(r, 0x1655) = Malloc(n);
    F<void*>(r, 0x1659) = Malloc(n);
    F<void*>(r, 0x165d) = Malloc(n * 3);
    for (uint32_t k = 0; k < n; ++k) F<uint8_t*>(r, 0x1655)[k] = 2;
    for (uint32_t k = 0; k < n; ++k) F<uint8_t*>(r, 0x1659)[k] = 1;
    for (uint32_t k = 0; k < n * 3; ++k) F<uint8_t*>(r, 0x165d)[k] = 0;
}
RT3_REPLACE(0x532c80, Render_AllocGrid);

void __fastcall Render_SetLayerValue(uint8_t* r, int, int32_t i, float v) { F<float>(r, 0x14e4 + i * 4) = v; }
RT3_REPLACE(0x533100, Render_SetLayerValue);

} // namespace rt3
