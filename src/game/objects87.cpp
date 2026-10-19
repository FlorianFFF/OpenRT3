// Site sweeps / camera setup / sound object colour in blocks 0x44..0x53 (batch 90). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Site_ClassifyWater(uint8_t* s);
void __fastcall Station_UpdateEdgeFlag(uint8_t* o);
void __fastcall Obj_Decc60(uint8_t* o);
uint32_t __fastcall Camera_Init(uint8_t* c);
void __fastcall Camera_KeepAboveGround(uint8_t* c);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Terrain_Remove1675(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);

// (int)(v * 0x5dcfe8) with the x87 product and the exe's _ftol.
static inline int32_t CellOf(float v) {
    int32_t r;
    __asm {
        fld v
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x449f80. Reclassifies every site (+0x2129, 0x1d7 bytes, +0x2145 x +0x2149),
// then updates every site's edge flag.
void __fastcall Map_ReclassifySites(uint8_t* m) {
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Site_ClassifyWater(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
    for (int32_t y = 0; y < F<int32_t>(m, 0x2149); ++y)
        for (int32_t x = 0; x < F<int32_t>(m, 0x2145); ++x)
            Station_UpdateEdgeFlag(F<uint8_t*>(m, 0x2129) + (F<int32_t>(m, 0x2145) * y + x) * 0x1d7);
}
RT3_REPLACE(0x449f80, Map_ReclassifySites);

// 0x456180. Marks the object done (+0x23e) once, releasing a busy count (0x6d401c +0xc60).
void __fastcall Obj_MarkDone(uint8_t* o) {
    if (o[0x23e]) return;
    o[0x23e] = 1;
    Obj_Decc60(hook::Global<uint8_t*>(0x6d401c));
}
RT3_REPLACE(0x456180, Obj_MarkDone);

uint8_t* __fastcall Camera_Construct(uint8_t* c) {
    F<uint32_t>(c, 0) = 0x5dd1c8;
    Camera_Init(c);
    return c;
}
RT3_REPLACE(0x52d6d0, Camera_Construct);

// 0x52d6f0. Attaches the camera to a terrain with eye (x, y, z) and distance.
void __fastcall Camera_Setup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist) {
    F<uint32_t>(c, 0x4) = x;
    F<uint32_t>(c, 0x6e) = terrain;
    F<uint32_t>(c, 0x8) = y;
    F<uint32_t>(c, 0xc) = z;
    F<uint32_t>(c, 0x18) = dist;
    F<uint32_t>(c, 0x4c) = dist;
    F<uint32_t>(c, 0x6a) = F<uint32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114222);
    F<int32_t>(c, 0x28) = (int32_t)F<float>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 0x22b67);
    Camera_KeepAboveGround(c);
}
RT3_REPLACE(0x52d6f0, Camera_Setup);

int32_t __fastcall Obj_Idlec60(uint8_t* o) { return F<int32_t>(o, 0xc60) == 0 ? 1 : 0; }
RT3_REPLACE(0x538a60, Obj_Idlec60);

// 0x5391b0. Destroys a placed effect: removes it from its terrain block list
// (+0x55; unless the engine is shutting down), frees +0x1e3 and calls vtbl[0].
void __fastcall Effect_Destroy(uint8_t* e, int, int32_t really) {
    if (!really) return;
    if (!F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114256)) {
        if (uint8_t* t = F<uint8_t*>(e, 4)) {
            int32_t y = CellOf(F<float>(e, 0x10)) >> 4;
            int32_t x = CellOf(F<float>(e, 0xc)) >> 4;
            Terrain_Remove1675(t, 0, F<PtrListNode*>(e, 0x55), x, y);
        }
    }
    if (F<void*>(e, 0x1e3)) {
        Free(F<void*>(e, 0x1e3));
        F<void*>(e, 0x1e3) = nullptr;
    }
    ((void(__thiscall*)(void*))(*(void***)e)[0])(e);
}
RT3_REPLACE(0x5391b0, Effect_Destroy);

// 0x539380. Applies effect style `kind` (1/2/4 -> 0/1/2) from the tables at
// 0x624d68.., with +0x44 = style + 1 when enabled.
void __fastcall Effect_SetStyle(uint8_t* e, int, uint8_t kind, uint8_t on) {
    uint8_t k;
    if (kind == 1) k = 0;
    else if (kind == 2) k = 1;
    else if (kind == 4) k = 2;
    else k = kind;
    uint32_t i = k;
    F<uint32_t>(e, 0x1f5) = ((const uint32_t*)0x624d68)[i];
    F<uint32_t>(e, 0x1f9) = ((const uint32_t*)0x624d78)[i];
    e[0x44] = on ? (uint8_t)(k + 1) : 0;
    const uint32_t* a = (const uint32_t*)(0x624d88 + i * 20);
    F<uint32_t>(e, 0x17b) = a[0];
    F<uint32_t>(e, 0x17f) = a[1];
    F<uint32_t>(e, 0x163) = a[2];
    F<uint32_t>(e, 0x167) = a[1];
    F<uint32_t>(e, 0x133) = a[2];
    F<uint32_t>(e, 0x137) = a[3];
    F<uint32_t>(e, 0x14b) = a[0];
    F<uint32_t>(e, 0x14f) = a[3];
    const uint32_t* b = (const uint32_t*)(0x624dc4 + i * 20);
    F<uint32_t>(e, 0x1db) = b[0];
    F<uint32_t>(e, 0x1df) = b[1];
    F<uint32_t>(e, 0x1c3) = b[2];
    F<uint32_t>(e, 0x1c7) = b[1];
    F<uint32_t>(e, 0x193) = b[2];
    F<uint32_t>(e, 0x197) = b[3];
    F<uint32_t>(e, 0x1ab) = b[0];
    F<uint32_t>(e, 0x1af) = b[3];
    F<uint32_t>(e, 0x1fd) = b[4];
}
RT3_REPLACE(0x539380, Effect_SetStyle);

// 0x5394b0. Copies the four vectors at +0x14/+0x20/+0x2c/+0x38 to the non-null outputs.
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
    uint32_t* out[4] = { a, b, c, d };
    for (int k = 0; k < 4; ++k)
        if (out[k])
            for (int j = 0; j < 3; ++j) out[k][j] = F<uint32_t>(e, 0x14 + k * 0xc + j * 4);
}
RT3_REPLACE(0x5394b0, Effect_GetVectors);

// 0x539530. Sets the angles (b, a, 0) of every object in the list +8.
void __fastcall Group_SetAngles(uint8_t* g, int, uint32_t a, uint32_t b) {
    if (!F<PtrList*>(g, 8)) return;
    PtrList_Rewind(F<PtrList*>(g, 8));
    for (uint8_t* o = (uint8_t*)PtrList_Next(F<PtrList*>(g, 8)); o; o = (uint8_t*)PtrList_Next(F<PtrList*>(g, 8)))
        SceneObj_SetAngles(o, 0, b, a, 0);
}
RT3_REPLACE(0x539530, Group_SetAngles);

// 0x539f90. Linear blend (1 - t) * +0x1f1 + t * +0x1ed (unrounded).
double __fastcall Effect_Blend(uint8_t* e, int, float t) {
    return ((double)*(const float*)0x5c85b4 - t) * F<float>(e, 0x1f1) + (double)t * F<float>(e, 0x1ed);
}
RT3_REPLACE(0x539f90, Effect_Blend);

// Colour +0x5f (ARGB); low nibble of +0x59 = "tinted" (rgb != 7f7f7f).
static inline void SetTinted(uint8_t* o, uint8_t on) { o[0x59] = (uint8_t)((o[0x59] & 0xf0) | on); }

void __fastcall Obj_SetArgb(uint8_t* o, int, uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    SetTinted(o, r == 0x7f && g == 0x7f && b == 0x7f ? 0 : 1);
    F<uint32_t>(o, 0x5f) = ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
RT3_REPLACE(0x53a350, Obj_SetArgb);
void __fastcall Obj_SetRgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b) {
    SetTinted(o, r == 0x7f && g == 0x7f && b == 0x7f ? 0 : 1);
    F<uint32_t>(o, 0x5f) = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (F<uint32_t>(o, 0x5f) & 0xff000000) | b;
}
RT3_REPLACE(0x53a3a0, Obj_SetRgb);
void __fastcall Obj_SetRgbValue(uint8_t* o, int, uint32_t v) {
    SetTinted(o, (v & 0x7f7f7f) != 0x7f7f7f ? 1 : 0);
    F<uint32_t>(o, 0x5f) = (F<uint32_t>(o, 0x5f) & 0xff000000) | v;
}
RT3_REPLACE(0x53a400, Obj_SetRgbValue);
void __fastcall Obj_SetAlpha5b(uint8_t* o, int, uint8_t a) { F<uint32_t>(o, 0x5b) = (uint32_t)a << 24; }
RT3_REPLACE(0x53a440, Obj_SetAlpha5b);

uint32_t __fastcall Get_ccba20() { return hook::Global<uint32_t>(0xccba20); }
RT3_REPLACE(0x53a510, Get_ccba20);
void __fastcall Nop53a580(void*, int, int32_t) {}
RT3_REPLACE(0x53a580, Nop53a580);

} // namespace rt3
