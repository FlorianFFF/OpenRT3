// Utilities.cpp - RT3.exe 0x564000-0x568000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/imports.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Render_ScreenSpaceAll(uint8_t* r);
void __fastcall Render_CullNone(uint8_t* r);
int32_t __fastcall Render_LightingOff(uint8_t* r);
void __fastcall Render_FogOff(uint8_t* r);
void __fastcall Render_SetDefaultMaterial(uint8_t* r, int, uint8_t shiny);
int32_t __fastcall Render_BlendDefault(uint8_t* r);
void __fastcall Render_AlphaTestOn(uint8_t* r, int, uint32_t ref);
int32_t __fastcall Render_SetZEnable(uint8_t* r, int, uint32_t v);
void __fastcall Render_BindVb2647(uint8_t* r);

void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12);

int32_t __stdcall Model_FreeFrames(uint8_t* m);
void __fastcall Lists_FreeItems(uint8_t* o);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
int32_t __fastcall NameTable_Contains(uint8_t* t, int, int32_t id, int32_t fatal);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
void __fastcall Hash_DestroyThunk(HashMap* m);
void* __stdcall Sound_LoadWav(const char* name, int32_t, int32_t* size);
uint32_t __fastcall TimedQueue_Pop(uint8_t* q);
void* __fastcall Pool_Alloc(int32_t* p);
void __fastcall ModelSet_Free(uint8_t* s);
void __fastcall SoundCache_Clear(uint8_t* s);
template <typename T> static inline T& G(uint32_t a) { return *(T*)a; }
void __fastcall Obj_GetXY10(uint8_t* o, int, uint32_t* a, uint32_t* b);
void* __fastcall Engine_Get1C(void* e);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall Quad_FindJoin(uint8_t* a, uint8_t* b, uint8_t** outA, uint8_t** outB, float* best);
struct PtrList;
float __fastcall Angle_PickFacing(uint8_t idx, float a);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
struct PtrListNode;
PtrListNode* __fastcall PtrList_Append(PtrList* l, int, void* value);
void __fastcall PtrList_Clear(PtrList* l);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall Quad_ToVertices(uint8_t* out, uint8_t* q, float lift);
void __stdcall Points_RotateAxis(float angle, float* pts);
void __fastcall VisitGrid_Visit(uint8_t* g, int, int16_t x, int16_t y);
PtrList* __fastcall PtrList_Init(PtrList* l);
int32_t __stdcall Light_LoadPoints(uint8_t* rec);
void __fastcall NameTable_PushPos(uint8_t* t);
void __fastcall NameTable_PopPosition(uint8_t* t);
void __fastcall CollisionFile_Load(uint8_t* c, int, int32_t id);
const char* __fastcall GetString(int32_t id);

void __fastcall Fatal_Exit(const char* msg);

void ReleaseResourceD97ab0();
void __stdcall Overlay_DrawText(const char* text, int32_t shadow);
void __fastcall Renderer_SetTexture(uint8_t* r, int, int32_t stage, uint8_t* tex);
void* __fastcall Scene_BeginBatch(uint8_t* s, int, int32_t count);
void __fastcall Render_AlphaTestOff(uint8_t* r);
int32_t __fastcall Render_BlendOff(uint8_t* r);
void __fastcall Render_DrawFontText(uint8_t* r, int, const char* text, float x0, float y0, float x1, float y1, uint32_t colour, void** font, uint32_t flags);
void __fastcall Render_SetSubViewport(uint8_t* r, int, float x0, float y0, float x1, float y1);
void __fastcall Render_ScreenSpaceWorld(uint8_t* r);
void __stdcall Sprite_ToTriangles(const uint8_t* s, float* out, int32_t* count);
void __stdcall Tri_ToVertices(const uint8_t* s, float* out, int32_t* count);
void __fastcall Scene_EndBatch(uint8_t* s, int, uint8_t draw);
void* __fastcall PtrList_RemoveCurrent(PtrList* l);
uint8_t __fastcall Render_DeviceState(uint8_t* r);
void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
void __fastcall Display_DestroyRenderer(uint8_t* o);
void __fastcall Pack_RecreateFonts(uint8_t* pack);
void __fastcall SceneStats_Draw(uint8_t* s, int, float dt);
int32_t __fastcall Render_BeginFrame(uint8_t* r, int, int32_t clear);
void __fastcall Render_SetPixelViewport(uint8_t* r, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Scene_BeginOverlay(uint8_t* s);
void __fastcall Render_PushStages(uint8_t* r, int, const uint32_t* states);
void __fastcall Render_SetFiltering(uint8_t* r, int, uint8_t filtered, int8_t stage);
void __fastcall UiList_Draw(uint8_t* v, int, PtrList* items, int32_t count);
void __fastcall Render_ApplyFilterStates(uint8_t* r);
void __fastcall SceneList_Draw(uint8_t* v, int, uint8_t* tex, PtrList* items);
void __fastcall Render_PopStages(uint8_t* r, int, int32_t n);
void __fastcall Render_FogOn(uint8_t* r);
void __fastcall Render_ApplyStoredTransforms(uint8_t* r);
void __fastcall Render_EndFrame(uint8_t* r, int, int32_t noPresent);
void __fastcall PumpMessages();
int32_t __fastcall Render_LightingDefault(uint8_t* r);
void __fastcall Scene_CreateRenderer(uint8_t* s);
void __fastcall Scene_DrawOverlay(uint8_t* s, int, int32_t frame);
RT3_NOINLINE void __fastcall PumpMessages();

// Functions of this file, in exe order.
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Scene_CreateRenderer(uint8_t* s);
uint8_t* __fastcall KeyTable_Init(uint8_t* t);
RT3_NOINLINE void __fastcall Nop564170();
void __fastcall KeyTable_Set(uint8_t* t, int, uint8_t k);
void __fastcall KeyTable_Clear(uint8_t* t, int, uint8_t k);
uint8_t __fastcall KeyTable_Get(uint8_t* t, int, uint8_t k);
void __fastcall Obj_SetName24(uint8_t* o, int, const char* s);
int32_t __fastcall Obj_FreeChildBuffer(uint8_t* o, int32_t keep);
void* __stdcall Sound_LoadWav(const char* name, int32_t, int32_t* size);
void __fastcall SoundCache_Clear(uint8_t* s);
uint32_t __fastcall TimedQueue_Pop(uint8_t* q);
void* __fastcall SoundCache_Load(uint8_t* s, int, const char* name, void* info);
void __fastcall SoundCache_ClearThunk(uint8_t* s);
int32_t __fastcall SoundCache_Init(uint8_t* s, int, int32_t capacity);
void __stdcall Sprite_ToTriangles(const uint8_t* s, float* out, int32_t* count);
void __stdcall Tri_ToVertices(const uint8_t* s, float* out, int32_t* count);
void __fastcall Scene_BeginOverlay(uint8_t* s);
void __fastcall UiList_Draw(uint8_t* v, int, PtrList* items, int32_t count);
void __fastcall SceneList_Draw(uint8_t* v, int, uint8_t* tex, PtrList* items);
RT3_NOINLINE void __fastcall Scene_DrawOverlay(uint8_t* s, int, int32_t frame);
uint8_t* __fastcall VisitGrid_Init(uint8_t* g, int, int32_t n);
RT3_NOINLINE void __fastcall VisitGrid_Free(uint8_t* g);
void __fastcall VisitGrid_Visit(uint8_t* g, int, int16_t x, int16_t y);
RT3_NOINLINE int32_t __fastcall VisitGrid_Log2(uint8_t* g);
RT3_NOINLINE uint8_t* __fastcall VisitGrid_Flags(uint8_t* g);
void __fastcall TerrainPatch_Triangulate(uint8_t* g, int, int32_t x0, int32_t y0);
void __fastcall Nop565c10();
void __fastcall Obj_GetVec0(uint32_t* o, int, uint32_t* out);
void __fastcall Obj_SetPair10(uint8_t* o, int, uint32_t a, uint32_t b, uint8_t c);
uint8_t __fastcall Obj_GetPair10(uint8_t* o, int, uint32_t* a, uint32_t* b);
void __fastcall Particles_Draw(uint8_t* p, int, int32_t i, float x, float y, float dx, float dy);
void __fastcall Texture_SetD97ab0(const char* name);
RT3_NOINLINE void ReleaseResourceD97ab0();
uint8_t* __fastcall Object_InitFromCollision(uint8_t* o, int, const char* name, int32_t);
void __fastcall Nop565e20();
void __fastcall Emitter_Init(uint8_t* e);
void __fastcall Emitter_Start(uint8_t* e, int, int32_t count, const uint32_t* from, const uint32_t* to, uint32_t start, float life);
int32_t __fastcall Particle_Advance(uint8_t* p, int, float dt);
void __stdcall Points_RotateAxis(float angle, float* pts);
void __fastcall Particle_ToBillboard(uint8_t* p, int, uint32_t* out, int32_t* count, const float* at);
void __fastcall Particles_FrameSetup(uint8_t* cam);
RT3_NOINLINE void __stdcall Overlay_DrawText(const char* text, int32_t shadow);
void __fastcall Render_DrawFontText(uint8_t* r, int, const char* text, float x0, float y0, float x1, float y1, uint32_t colour, void** font, uint32_t flags);
int32_t __stdcall Light_LoadPoints(uint8_t* o);
int32_t __stdcall CollisionMesh_Load(uint8_t* m);
void __fastcall Meshes_InvalidateAll();
int32_t __fastcall CollisionBlock_Load(uint8_t*, int, uint32_t* out);
int32_t __stdcall Model_FreeFrames(uint8_t* m);
void __fastcall CollisionFile_Load(uint8_t* c, int, int32_t id);
void __fastcall ModelSet_Free(uint8_t* s);
uint8_t* __fastcall CollisionFile_Construct(uint8_t* c, int, int32_t id);
void __fastcall ModelSet_FreeThunk(uint8_t* s);
float __fastcall Angle_PickFacing(uint8_t idx, float a);
void __fastcall Quad_FindJoin(uint8_t* a, uint8_t* b, uint8_t** outA, uint8_t** outB, float* best);
RT3_NOINLINE void __fastcall QuadGrid_Weld(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Quad_ToVertices(uint8_t* out, uint8_t* q, float lift);
void __fastcall Lists_FreeItems(uint8_t* o);
void __fastcall Quads_EmitFlat(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t tint);
void __fastcall Quads_EmitRaised(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t tint, float lift);
void __fastcall Lists_Destroy(uint8_t* o);
RT3_NOINLINE void __fastcall Water_BuildQuads(uint8_t* g);

// The texture name from the main pack's cache (all mips, flag 0x7f).
static uint8_t* Texture_Acquire(const char* name) { return TextureCache_Get(g_pack, 0, name, 0, 0, 0x7f); }

// Replaces the texture held in global slot by name's (the old one released).
static void Texture_Replace(uint32_t slot, const char* name) {
    if (void* old = hook::Global<void*>(slot)) {
        Pack_Release(g_pack, 0, old);
        hook::Global<void*>(slot) = nullptr;
    }
    hook::Global<uint8_t*>(slot) = Texture_Acquire(name);
}


static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// 0x567bf0. Frees the items of the lists +0x20 and +0x1c and clears them.
static inline void FreeItems(PtrList* l) {
    if (!F<int32_t>(l, 0xc)) return;
    PtrList_Rewind(l);
    for (void* p = PtrList_Next(l); p; p = PtrList_Next(l)) crt::free(p);
    PtrList_Clear(l);
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }


static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline double Fl_190(uint32_t a) { return *(const float*)a; }

// Colour `col` with its RGB scaled by `fade` / 2 (clamped to 0..1); alpha kept.
static uint32_t Shade(uint32_t col, float fade) {
    float rf = (float)(int32_t)((col >> 16) & 0xff), gf = (float)(int32_t)((col >> 8) & 0xff);
    double bf = (double)(int32_t)(col & 0xff);
    double k = (double)fade * *(const double*)0x5c8628;
    if (k > *(const double*)0x5c8770) k = Fl_190(0x5c85b4);
    else if (k < Fl_190(0x5c8598)) k = Fl_190(0x5c8598);
    uint32_t c = (uint32_t)Ftol((double)rf * k);
    c = (c << 8) | (uint32_t)Ftol((double)gf * k);
    c = (c << 8) | (uint32_t)Ftol(bf * k);
    return c | (col & 0xff000000);
}

static inline double Fl_191(uint32_t a) { return *(const float*)a; }

namespace {

// Quad corner offsets: x/z of corners 0..2 and x of corner 3 as floats, z of
// corner 3 still unrounded, as the original keeps it on the FPU stack.
struct QuadDelta {
    float d0x, d0z, d1x, d1z, d2x, d2z, d3x;
    double d3z;
};

// Plain water: the cell's two heights a / -b turned by the four corner
// angles; q+0x3a gets the wave phase of the corner (cornerX, cornerZ).
void WaterCorners(float a, float nb, float cornerX, float cornerZ, QuadDelta* o, int32_t* phase) {
    double c0, s0, v;
    float d0x, d0z, d1x, d1z, d2x, d2z, d3x;
    int32_t ph;
    __asm {
        fld qword ptr ds:[0x5ca1b0]
        fcos
        fstp c0
        fld nb
        fld qword ptr ds:[0x5ca1b0]
        fsin
        fstp s0
        fld a
        fld s0
        fmul st, st(1)
        fld c0
        fmul st, st(3)
        faddp st(1), st
        fstp d0x
        fld c0
        fmul st, st(1)
        fld s0
        fmul st, st(3)
        fsubp st(1), st
        fstp d0z
        fld qword ptr ds:[0x5de428]
        fcos
        fld qword ptr ds:[0x5de428]
        fsin
        fld st(0)
        fmul st, st(3)
        fld st(2)
        fmul st, st(5)
        faddp st(1), st
        fstp d1x
        fxch st(1)
        fmul st, st(2)
        fxch st(1)
        fmul st, st(3)
        fsubp st(1), st
        fstp d1z
        fld qword ptr ds:[0x5de420]
        fcos
        fld qword ptr ds:[0x5de420]
        fsin
        fld st(0)
        fmul st, st(3)
        fld st(2)
        fmul st, st(5)
        faddp st(1), st
        fstp d2x
        fxch st(1)
        fmul st, st(2)
        fxch st(1)
        fmul st, st(3)
        fsubp st(1), st
        fstp d2z
        fld qword ptr ds:[0x5de418]
        fcos
        fld qword ptr ds:[0x5de418]
        fsin
        fld st(0)
        fmul st, st(3)
        fld st(2)
        fmul st, st(5)
        faddp st(1), st
        fstp d3x
        fxch st(1)
        fmul st, st(2)
        fxch st(1)
        fmul st, st(3)
        fsubp st(1), st
        fstp v
        fstp st(0)
        fstp st(0)
        fld cornerZ
        fmul qword ptr ds:[0x5d0790]
        fcos
        fld cornerX
        fmul qword ptr ds:[0x5d0790]
        fsin
        fmulp st(1), st
        fmul qword ptr ds:[0x5de410]
        call Rt3_Ftol
        mov ph, eax
    }
    o->d0x = d0x;
    o->d0z = d0z;
    o->d1x = d1x;
    o->d1z = d1z;
    o->d2x = d2x;
    o->d2z = d2z;
    o->d3x = d3x;
    o->d3z = v;
    *phase = ph;
}

// Shore: the corner offsets (e1, p1..p4 and o) turned by angle a.
void ShoreCorners(float a, float e1, float p1, float p2, float off, float p3, float p4, QuadDelta* o) {
    double ad, p1d = p1, v;
    float d0x, d0z, d1x, d1z, d2x, d2z, d3x;
    __asm {
        fld a
        fst ad
        fcos
        fld ad
        fsin
        fld e1
        fmul st, st(1)
        fld st(2)
        fmul p1d
        faddp st(1), st
        fstp d0x
        fld e1
        fmul st, st(2)
        fld st(1)
        fmul p1d
        fsubp st(1), st
        fstp d0z
        fld e1
        fld p2
        fld e1
        fmul st, st(3)
        fld st(4)
        fmul st, st(2)
        faddp st(1), st
        fstp d1x
        fxch st(1)
        fmul st, st(3)
        fld st(2)
        fmul st, st(2)
        fsubp st(1), st
        fstp d1z
        fstp st(0)
        fld off
        fld p3
        fld off
        fmul st, st(3)
        fld st(4)
        fmul st, st(2)
        faddp st(1), st
        fstp d2x
        fxch st(1)
        fmul st, st(3)
        fld st(2)
        fmul st, st(2)
        fsubp st(1), st
        fstp d2z
        fstp st(0)
        fld off
        fld p4
        fld off
        fmul st, st(3)
        fld st(4)
        fmul st, st(2)
        faddp st(1), st
        fstp d3x
        fxch st(1)
        fmulp st(3), st
        fxch st(1)
        fmul st, st(1)
        fsubp st(2), st
        fstp st(0)
        fstp v
    }
    o->d0x = d0x;
    o->d0z = d0z;
    o->d1x = d1x;
    o->d1z = d1z;
    o->d2x = d2x;
    o->d2z = d2z;
    o->d3x = d3x;
    o->d3z = v;
}

// |sin a * cos a| * 32 at full precision.
double SinCos32(float a) {
    double r;
    __asm {
        fld a
        fsin
        fld a
        fcos
        fmulp st(1), st
        fabs
        fmul qword ptr ds:[0x5de440]
        fstp r
    }
    return r;
}

// The exe CRT's fmod (_CIfmod) of a by 2 pi.
float Wrap2Pi(float a) {
    float r;
    __asm {
        fld a
        fld qword ptr ds:[0x5c8c08]
        mov eax, 0x5a139a
        call eax
        fstp r
    }
    return r;
}

inline int32_t QuadFtol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

inline bool Wet(uint8_t b) { return G<int32_t>(0x625c60 + (b & 7) * 4) != 0; }
}

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

// Copies a float through the x87 (FLD / FSTP), which quiets signalling NaNs as the
// original's copies do.
static inline void X87Copy(void* dst, const void* src) {
    __asm {
        mov eax, src
        mov ecx, dst
        fld dword ptr [eax]
        fstp dword ptr [ecx]
    }
}

static inline double Sqrt87(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }

// 0x553aa0 / 0x5549b0 / 0x565da0. Release the pack resources held in 0xd93990 /
// 0xd9399c / 0xd97ab0.
static void ReleaseResourceIn(uint32_t slot) {
    void*& r = hook::Global<void*>(slot);
    if (!r) return;
    Pack_Release(g_pack, 0, r);
    r = nullptr;
}

// Key table: 0x400 bytes, one flag per key code.
uint8_t* __fastcall KeyTable_Init(uint8_t* t) {
    for (int k = 0; k < 0x100; ++k) F<uint32_t>(t, k * 4) = 0;
    return t;
}
RT3_REPLACE(0x564150, KeyTable_Init);

RT3_NOINLINE void __fastcall Nop564170() { __asm nop }
RT3_REPLACE(0x564170, Nop564170);

void __fastcall KeyTable_Set(uint8_t* t, int, uint8_t k) { t[k] = 1; }
RT3_REPLACE(0x564180, KeyTable_Set);

void __fastcall KeyTable_Clear(uint8_t* t, int, uint8_t k) { t[k] = 0; }
RT3_REPLACE(0x564190, KeyTable_Clear);

uint8_t __fastcall KeyTable_Get(uint8_t* t, int, uint8_t k) { return t[k]; }
RT3_REPLACE(0x5641a0, KeyTable_Get);

void __fastcall Obj_SetName24(uint8_t* o, int, const char* s) {
    char* d = (char*)o + 0x24;
    while ((*d++ = *s++) != 0) {}
}
RT3_REPLACE(0x5641b0, Obj_SetName24);

// 0x5641d0 (vtable). Unless kept (EDX), frees the buffer +0x10 of +4.
int32_t __fastcall Obj_FreeChildBuffer(uint8_t* o, int32_t keep) {
    if (!keep) {
        uint8_t* s = F<uint8_t*>(o, 4);
        crt::free(F<void*>(s, 0x10));
        F<void*>(s, 0x10) = nullptr;
    }
    return 0;
}
RT3_REPLACE(0x5641d0, Obj_FreeChildBuffer);

// 0x5641f0. Loads WAV file `name` (or, when unknown, id `name` itself) from the
// packs into a new buffer, parsing its header into 0xd9798c; *size = its size.
RT3_NOINLINE void* __stdcall Sound_LoadWav(const char* name, int32_t, int32_t* size) {
    uint8_t* t = g_pack;
    int32_t id = NameTable_Contains(t, 0, NameTable_Hash(t, 0, name), 1)
                     ? NameTable_Hash(g_pack, 0, name)
                     : (int32_t)name;
    int32_t n = NameTable_EntrySize(g_pack, 0, id);
    NameTable_Seek(g_pack, 0, id, 0);
    void* p = crt::malloc(n);
    NameTable_Read(g_pack, 0, p, n);
    imp::AIL_WAV_info(p, (void*)0xd9798c);
    if (size) *size = n;
    return p;
}
RT3_REPLACE(0x5641f0, Sound_LoadWav);

// 0x564280. Empties the sound cache (+0 node pool, +4 name hash, +8 LRU heap, size
// +0xc of capacity +0x10, prefix +0x24, enabled +0x14), freeing every cached sound.
RT3_NOINLINE void __fastcall SoundCache_Clear(uint8_t* s) {
    if (!F<int32_t>(s, 0x10)) return;
    Hash_ForEach(F<HashMap*>(s, 4), 0, 0, (HashVisit)0x5641d0, nullptr);
    Hash_Destroy(F<HashMap*>(s, 4));
    crt::free(F<void*>(F<uint8_t*>(s, 0), 4));
    if (NodePool* p = F<NodePool*>(s, 0)) {
        Pool_Free(p);
        crt::operator_delete(p);
    }
    F<void*>(s, 0) = nullptr;
    if (HashMap* h = F<HashMap*>(s, 4)) {
        Hash_DestroyThunk(h);
        crt::operator_delete(h);
    }
    F<void*>(s, 4) = nullptr;
    if (MaxHeap* h = F<MaxHeap*>(s, 8)) {
        Heap_Destroy2(h);
        crt::operator_delete(h);
    }
    F<void*>(s, 8) = nullptr;
    F<int32_t>(s, 0x10) = 0;
    F<int32_t>(s, 0xc) = 0;
    s[0x24] = 0;
    F<int32_t>(s, 0x14) = 0;
}
RT3_REPLACE(0x564280, SoundCache_Clear);

// 0x564310. Pops the highest-priority entry (heap +8): frees its buffer, removes it
// from the lookup (+4), recycles the node in the pool (+0) and counts it (+0x20).
// Returns the entry's value (+4), read after the node is released, as in the original.
RT3_NOINLINE uint32_t __fastcall TimedQueue_Pop(uint8_t* q) {
    uint8_t* e = (uint8_t*)Heap_PopMax(F<MaxHeap*>(q, 8));
    crt::free(F<void*>(e, 0x10));
    F<void*>(e, 0x10) = nullptr;
    Hash_Remove(F<HashMap*>(q, 4), 0, F<uint32_t>(e, 0));
    uint8_t* pool = F<uint8_t*>(q, 0);
    if (F<int32_t>(pool, 0xc) && (uint32_t)e >= F<uint32_t>(pool, 4) && (uint32_t)e < F<uint32_t>(pool, 8)) {
        uint32_t* fl = F<uint32_t*>(pool, 0);
        F<uint32_t>(e, 0) = fl[0];
        fl[0] = (uint32_t)e;
        fl[2]++;
        if (!fl[1]) fl[1] = (uint32_t)e;
    } else {
        crt::operator_delete(e);
    }
    F<int32_t>(q, 0x20)++;
    return F<uint32_t>(e, 4);
}
RT3_REPLACE(0x564310, TimedQueue_Pop);

// 0x564390. Loads sound <prefix +0x24><name> into a new buffer, through the cache
// when enabled (+0x14): cached entries {crc, size, heap node, data, WAV info x 9}
// are reused (hits +0x1c of lookups +0x18); new ones evict the oldest until they
// fit (sounds larger than the cache are returned uncached). *info gets the WAV info.
void* __fastcall SoundCache_Load(uint8_t* s, int, const char* name, void* info) {
    char path[0x10c];
    char* d = path;
    if (s[0x24])
        for (const char* p = (const char*)s + 0x24; (*d = *p) != 0; ++d, ++p) {}
    for (const char* p = name; (*d++ = *p++) != 0;) {}
    int32_t size;
    if (!F<int32_t>(s, 0x14)) return Sound_LoadWav(path, (int32_t)name, &size);
    uint32_t key = Crc32String(path, 1);
    ++F<int32_t>(s, 0x18);
    uint8_t* e = (uint8_t*)Hash_Get(F<HashMap*>(s, 4), 0, key);
    if (!e) {
        void* data = Sound_LoadWav(path, (int32_t)name, &size);
        if (size > F<int32_t>(s, 0x10)) return data;
        while (F<int32_t>(s, 0xc) + size > F<int32_t>(s, 0x10)) F<int32_t>(s, 0xc) -= TimedQueue_Pop(s);
        F<int32_t>(s, 0xc) += size;
        e = (uint8_t*)Pool_Alloc(F<int32_t*>(s, 0));
        memcpy(e + 0x14, (const void*)0xd9798c, 36);
        F<uint32_t>(e, 0) = key;
        F<void*>(e, 0x10) = data;
        F<int32_t>(e, 4) = size;
        F<HeapNode*>(e, 8) = Heap_Push(F<MaxHeap*>(s, 8), 0, 0x7ffffffe, (int32_t)e);
        Hash_Insert(F<HashMap*>(s, 4), 0, key, (int32_t)e);
    } else {
        size = F<int32_t>(e, 4);
        ++F<int32_t>(s, 0x1c);
    }
    if (info) memcpy(info, e + 0x14, 36);
    Heap_DecrementKey(F<MaxHeap*>(s, 8), 0, F<HeapNode*>(e, 8));
    void* p = crt::malloc(size);
    memcpy(p, F<void*>(e, 0x10), size);
    return p;
}
RT3_REPLACE(0x564390, SoundCache_Load);

// 0x564560. Thunk to SoundCache_Clear.
void __fastcall SoundCache_ClearThunk(uint8_t* s) { SoundCache_Clear(s); }
RT3_REPLACE(0x564560, SoundCache_ClearThunk);

// 0x564570. (Re)creates the sound cache with `capacity` bytes (0 = off): a pool of
// 100 0x38-byte entries (+0), a 17-bucket name hash (+4) and an LRU heap (+8).
int32_t __fastcall SoundCache_Init(uint8_t* s, int, int32_t capacity) {
    if (F<int32_t>(s, 0x10)) SoundCache_Clear(s);
    if (!capacity) {
        F<void*>(s, 0) = nullptr;
        F<void*>(s, 4) = nullptr;
        F<void*>(s, 8) = nullptr;
        F<int32_t>(s, 0x10) = 0;
        F<int32_t>(s, 0x14) = 0;
        F<int32_t>(s, 0xc) = 0;
        s[0x24] = 0;
        return 0;
    }
    uint32_t* h = (uint32_t*)crt::operator_new(0x14);
    if (h) memset(h, 0, 0x14);
    F<uint32_t*>(s, 4) = h;
    uint32_t* p = (uint32_t*)crt::operator_new(0x1c);
    if (p) memset(p, 0, 0x1c);
    F<uint32_t*>(s, 0) = p;
    uint32_t* q = (uint32_t*)crt::operator_new(0x1c);
    if (q) memset(q, 0, 0x10);
    F<uint32_t*>(s, 8) = q;
    void* storage = crt::malloc(0x15e0);
    Pool_Init(F<NodePool*>(s, 0), 0, storage, 0x38, 100, 0);
    Heap_Init(F<MaxHeap*>(s, 8), 0, 100);
    Hash_Init(F<HashMap*>(s, 4), 0, 0x11, 100);
    F<int32_t>(s, 0x10) = capacity;
    F<int32_t>(s, 0x20) = 0;
    F<int32_t>(s, 0x1c) = 0;
    F<int32_t>(s, 0x18) = 0;
    F<int32_t>(s, 0xc) = 0;
    s[0x24] = 0;
    return 1;
}
RT3_REPLACE(0x564570, SoundCache_Init);

// A float's bits loaded and stored by the x87 (a signalling NaN comes out quiet).
static uint32_t ThroughX87(uint32_t bits) {
    float f;
    __asm {
        fld dword ptr bits
        fstp f
    }
    return *(uint32_t*)&f;
}

// 0x564660. Expands a screen sprite (+1/+5 left/top, +9/+0xd right/bottom, depth
// +0x19, colour +0x1d faded by +0x29 / 2, texture rect +0x45..+0x51) into two
// triangles (6 vertices of x, y, z, colour, u, v); the off-diagonal texture corners
// are swapped for mirrored rects so the image stays upright.
RT3_NOINLINE void __stdcall Sprite_ToTriangles(const uint8_t* s, float* out, int32_t* count) {
    // The coordinates as bits: the original copies most of them as integers
    // and only some through the x87 (below), and a float copy could change a
    // NaN's bits.
    const uint32_t a = F<uint32_t>((void*)s, 1), b = F<uint32_t>((void*)s, 5);
    uint32_t c = F<uint32_t>((void*)s, 9), d = F<uint32_t>((void*)s, 0xd), z = F<uint32_t>((void*)s, 0x19);
    const uint32_t u0 = F<uint32_t>((void*)s, 0x45), v0 = F<uint32_t>((void*)s, 0x49), u1 = F<uint32_t>((void*)s, 0x4d),
                   v1 = F<uint32_t>((void*)s, 0x51);
    uint32_t col = F<uint32_t>((void*)s, 0x1d);
    auto fl = [](const uint32_t& bits) { return *(const float*)&bits; };
    uint32_t c1u, c1v, c2u, c2v;
    bool same = (fl(u0) < fl(u1) && fl(v0) < fl(v1)) || (fl(u0) > fl(u1) && fl(v0) > fl(v1));
    if (same) {
        c1u = u1;
        c1v = v0;
        c2u = u0;
        c2v = v1;
    } else {
        c1u = u0;
        c1v = v1;
        c2u = u1;
        c2v = v0;
    }
    float rf = (float)(int32_t)((col >> 16) & 0xff), gf = (float)(int32_t)((col >> 8) & 0xff);
    double bf = (double)(int32_t)(col & 0xff);
    double k = (double)F<float>((void*)s, 0x29) * *(const double*)0x5c8628;
    if (k > *(const double*)0x5c8770) k = *(const float*)0x5c85b4;
    else if (k < *(const float*)0x5c8598) k = *(const float*)0x5c8598;
    uint32_t nr = (uint32_t)Ftol((double)rf * k);
    uint32_t ng = (uint32_t)Ftol((double)gf * k);
    uint32_t nb = (uint32_t)Ftol(k * bf);
    uint32_t color = ((((nr << 8) | ng) << 8) | nb) | (col & 0xff000000);
    uint32_t* v = (uint32_t*)out;
    auto put = [&](int32_t i, uint32_t x, uint32_t y, uint32_t u, uint32_t w) {
        v[i * 6] = x;
        v[i * 6 + 1] = y;
        v[i * 6 + 2] = z;
        v[i * 6 + 3] = color;
        v[i * 6 + 4] = u;
        v[i * 6 + 5] = w;
    };
    put(0, c, b, c1u, c1v);
    put(1, a, b, u0, v0);
    put(2, a, d, c2u, c2v);
    put(3, c, b, c1u, c1v);
    put(4, a, d, c2u, c2v);
    put(5, c, d, u1, v1);
    // The original moves these through the x87 (FLD / FST dword), which quiets
    // a signalling NaN; the other fields it copies as integers.
    static const uint8_t kX87[] = { 0, 1, 4, 5, 6, 7, 12, 19, 22, 23, 24 };
    for (uint8_t k : kX87) v[k] = ThroughX87(v[k]);
    *count = 6;
}
RT3_REPLACE(0x564660, Sprite_ToTriangles);

// 0x5648c0. Expands a screen triangle (corners +1/+5, +9/+0xd, +0x11/+0x15, depth
// +0x19, colours +0x1d/+0x21/+0x25 faded by +0x29/+0x2d/+0x31 / 2, texture
// coordinates +0x45..+0x59) into 3 vertices of x, y, z, colour, u, v.
RT3_NOINLINE void __stdcall Tri_ToVertices(const uint8_t* s, float* out, int32_t* count) {
    uint8_t* p = (uint8_t*)s;
    uint32_t* v = (uint32_t*)out;
    // Everything is read before the first write, as in the original (the
    // output may overlap the triangle).
    uint32_t z = F<uint32_t>(p, 0x19), in[18];
    for (int32_t i = 0; i < 3; ++i) {
        in[i * 6] = F<uint32_t>(p, 1 + i * 8);
        in[i * 6 + 1] = F<uint32_t>(p, 5 + i * 8);
        in[i * 6 + 2] = z;
        in[i * 6 + 3] = Shade(F<uint32_t>(p, 0x1d + i * 4), F<float>(p, 0x29 + i * 4));
        in[i * 6 + 4] = F<uint32_t>(p, 0x45 + i * 8);
        in[i * 6 + 5] = F<uint32_t>(p, 0x49 + i * 8);
    }
    for (int k = 0; k < 18; ++k) v[k] = in[k];
    // The original moves these three through the x87 (FLD / FSTP dword), which
    // quiets a signalling NaN; the rest it copies as integers.
    v[0] = ThroughX87(v[0]);
    v[16] = ThroughX87(v[16]);
    v[17] = ThroughX87(v[17]);
    *count = 3;
}
RT3_REPLACE(0x5648c0, Tri_ToVertices);

// 0x564c10. Render state for 2D overlays.
RT3_NOINLINE void __fastcall Scene_BeginOverlay(uint8_t* s) {
    Render_ScreenSpaceAll(F<uint8_t*>(s, 0x2d));
    Render_CullNone(F<uint8_t*>(s, 0x2d));
    Render_LightingOff(F<uint8_t*>(s, 0x2d));
    Render_FogOff(F<uint8_t*>(s, 0x2d));
    Render_SetDefaultMaterial(F<uint8_t*>(s, 0x2d), 0, 0);
    Render_BlendDefault(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_SetZEnable(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_BindVb2647(F<uint8_t*>(s, 0x2d));
}
RT3_REPLACE(0x564c10, Scene_BeginOverlay);

// 0x564c70. Draws the first `count` items of 2D draw list `items` for view
// v (z test off): sprites (1, batched as triangles) and triangles (4) with
// their texture (+0x41, else none or the white texture +0x365d), text (3,
// +0x3d, freed; with a shadow 0.0025 down-right unless flag 2), sub
// viewports (5) and screen space (6); blending per item (+0x5d low bits).
RT3_NOINLINE void __fastcall UiList_Draw(uint8_t* v, int, PtrList* items, int32_t count) {
    if (count <= 0) return;
    uint8_t* cur = nullptr;
    Renderer_SetTexture(F<uint8_t*>(v, 0x2d), 0, 0, nullptr);
    Render_SetZEnable(F<uint8_t*>(v, 0x2d), 0, 0);
    v[0x374e] = 0;
    PtrList_Rewind(items);
    uint8_t* it = (uint8_t*)PtrList_Next(items);
    do {
        uint8_t* tex = F<uint8_t*>(it, 0x41);
        if (it[0] == 1 || it[0] == 4) {
            if (tex) {
                if (tex != cur) {
                    Renderer_SetTexture(F<uint8_t*>(v, 0x2d), 0, 0, tex);
                    cur = tex;
                }
            } else if (!g_engine[0x114251] && !F<int32_t>(F<uint8_t*>(v, 0x59), 4)) {
                if (cur) {
                    Renderer_SetTexture(F<uint8_t*>(v, 0x2d), 0, 0, nullptr);
                    cur = nullptr;
                }
            } else if (F<uint8_t*>(v, 0x365d) != cur) {
                Renderer_SetTexture(F<uint8_t*>(v, 0x2d), 0, 0, F<uint8_t*>(v, 0x365d));
                cur = F<uint8_t*>(v, 0x365d);
            }
        }
        uint8_t mode = it[0x5d] & 0xf;
        if (mode != v[0x374d]) {
            v[0x374d] = mode;
            if (mode) {
                Render_AlphaTestOn(F<uint8_t*>(v, 0x2d), 0, 1);
                Render_BlendDefault(F<uint8_t*>(v, 0x2d));
            } else {
                Render_AlphaTestOff(F<uint8_t*>(v, 0x2d));
                Render_BlendOff(F<uint8_t*>(v, 0x2d));
            }
        }
        switch (it[0]) {
        case 1:
        case 4: {
            int32_t n = 0;
            float* vb = (float*)Scene_BeginBatch(v, 0, it[0] == 1 ? 8 : 4);
            if (it[0] == 1)
                Sprite_ToTriangles(it, vb, &n);
            else
                Tri_ToVertices(it, vb, &n);
            F<int32_t>(v, 0x71) += n;
            Scene_EndBatch(v, 0, 1);
            break;
        }
        case 3:
            if (const char* text = F<const char*>(it, 0x3d)) {
                if (!(F<uint32_t>(it, 0x35) & 2)) {
                    const double d = *(const double*)0x5caae0;
                    Render_DrawFontText(F<uint8_t*>(v, 0x2d), 0, text, (float)((double)F<float>(it, 1) + d), (float)((double)F<float>(it, 5) + d),
                                        (float)((double)F<float>(it, 9) + d), (float)((double)F<float>(it, 0xd) + d), F<uint32_t>(it, 0x1d) & 0xff000000,
                                        F<void**>(it, 0x39), F<uint32_t>(it, 0x35));
                }
                Render_DrawFontText(F<uint8_t*>(v, 0x2d), 0, F<const char*>(it, 0x3d), F<float>(it, 1), F<float>(it, 5), F<float>(it, 9), F<float>(it, 0xd),
                                    F<uint32_t>(it, 0x1d), F<void**>(it, 0x39), F<uint32_t>(it, 0x35));
                crt::free(F<void*>(it, 0x3d));
            }
            F<void*>(it, 0x3d) = nullptr;
            break;
        case 5:
            Render_SetSubViewport(F<uint8_t*>(v, 0x2d), 0, F<float>(it, 1), F<float>(it, 5), F<float>(it, 9), F<float>(it, 0xd));
            break;
        case 6:
            Render_ScreenSpaceWorld(F<uint8_t*>(v, 0x2d));
            break;
        }
        it = (uint8_t*)PtrList_Next(items);
    } while (--count);
    Render_SetZEnable(F<uint8_t*>(v, 0x2d), 0, 1);
}
RT3_REPLACE(0x564c70, UiList_Draw);

// 0x564ee0. Draws and empties draw list `items` for view v with texture
// `tex` (none: the white texture +0x365d when wanted): sprites (1) and
// triangles (4) batched up to 0x400 vertices (a batch also ends when the
// item's blend (+0x5d low) or z (+0x5d high) mode changes, which is applied
// first, or the shared buffer +0x71 nears its end), text (3, +0x3d, freed),
// then sub viewport (5) or screen space (6) items after flushing.
RT3_NOINLINE void __fastcall SceneList_Draw(uint8_t* v, int, uint8_t* tex, PtrList* items) {
    if (F<int32_t>((uint8_t*)items, 0xc) <= 0) return;
    if (!tex) {
        if (g_engine[0x114251] || F<int32_t>(F<uint8_t*>(v, 0x59), 4)) tex = F<uint8_t*>(v, 0x365d);
    }
    Renderer_SetTexture(F<uint8_t*>(v, 0x2d), 0, 0, tex);
    PtrList_Rewind(items);
    uint8_t* it = (uint8_t*)PtrList_RemoveCurrent(items);
    if (!it) return;
    do {
        uint8_t* vb = (uint8_t*)Scene_BeginBatch(v, 0, 0x400);
        int32_t stop = 0;
        uint8_t* last = nullptr;
        while (it) {
            uint8_t lo = it[0x5d] & 0xf;
            int32_t n = 0;
            if (lo != v[0x374d]) {
                v[0x374d] = lo;
                if (lo) {
                    Render_AlphaTestOn(F<uint8_t*>(v, 0x2d), 0, 1);
                    Render_BlendDefault(F<uint8_t*>(v, 0x2d));
                } else {
                    Render_AlphaTestOff(F<uint8_t*>(v, 0x2d));
                    Render_BlendOff(F<uint8_t*>(v, 0x2d));
                }
                break;
            }
            uint8_t hi = it[0x5d] >> 4;
            if (hi != v[0x374e]) {
                v[0x374e] = hi;
                Render_SetZEnable(F<uint8_t*>(v, 0x2d), 0, hi ? 1 : 0);
                break;
            }
            switch (it[0]) {
            case 1:
                Sprite_ToTriangles(it, (float*)vb, &n);
                break;
            case 3:
                if (F<const char*>(it, 0x3d)) {
                    Render_DrawFontText(F<uint8_t*>(v, 0x2d), 0, F<const char*>(it, 0x3d), F<float>(it, 1), F<float>(it, 5), F<float>(it, 9), F<float>(it, 0xd),
                                        F<uint32_t>(it, 0x1d), F<void**>(it, 0x39), F<uint32_t>(it, 0x35));
                    crt::free(F<void*>(it, 0x3d));
                }
                F<void*>(it, 0x3d) = nullptr;
                break;
            case 4:
                Tri_ToVertices(it, (float*)vb, &n);
                break;
            case 5:
            case 6:
                last = it;
                stop = it[0];
                break;
            }
            F<int32_t>(v, 0x71) += n;
            vb += n * 0x18;
            it = (uint8_t*)PtrList_RemoveCurrent(items);
            if (F<int32_t>(v, 0x71) > 0xfbe8 || stop) break;
        }
        Scene_EndBatch(v, 0, 1);
        if (stop == 5)
            Render_SetSubViewport(F<uint8_t*>(v, 0x2d), 0, F<float>(last, 1), F<float>(last, 5), F<float>(last, 9), F<float>(last, 0xd));
        else if (stop == 6)
            Render_ScreenSpaceWorld(F<uint8_t*>(v, 0x2d));
    } while (it);
}
RT3_REPLACE(0x564ee0, SceneList_Draw);

// 0x565110. Draws the 2D overlay of scene s for this frame. A lost device
// (state 1) only pumps messages until it comes back; a reset (2) rebuilds the
// renderer, releasing and recreating the pack fonts around it. With `frame`
// it also draws the stats and brackets the frame. The overlay goes into the
// current screen viewport (engine +0x34 table, entry +0x11421e): either the
// UI list (engine +0x11369d, +0x58 items, with 0x6d4034) or the scene's
// textured lists (+0x1136a5, then +0x1136a1 untextured), each in the scene's
// stage presets (+0x55 / +0x59), with the light flag of +0x29 dropped.
RT3_NOINLINE void __fastcall Scene_DrawOverlay(uint8_t* s, int, int32_t frame) {
    uint8_t state = Render_DeviceState(F<uint8_t*>(s, 0x2d));
    if (state == 1) {
        while (Render_DeviceState(F<uint8_t*>(s, 0x2d)) == 1) PumpMessages();
        return;
    }
    if (state == 2) {
        if (g_pack && hook::As<uint32_t>(g_6d4034)) Pack_ReleaseFontHandles(g_pack);
        Display_DestroyRenderer(s);
        Scene_CreateRenderer(s);
        if (g_pack && hook::As<uint32_t>(g_6d4034)) Pack_RecreateFonts(g_pack);
    }
    if ((uint8_t)frame) {
        SceneStats_Draw(s, 0, 0.0f);
        Render_BeginFrame(F<uint8_t*>(s, 0x2d), 0, 1);
    }
    uint32_t light = F<uint32_t>(F<uint8_t*>(s, 0x29), 0x29);
    auto viewport = [&] {
        uint8_t* eng = g_engine;
        uint8_t* vp = F<uint8_t*>(eng, 0x34) + F<int32_t>(eng, 0x11421e) * 0x15;
        Render_SetPixelViewport(F<uint8_t*>(s, 0x2d), 0, 0, 0, F<int32_t>(vp, 0), F<int32_t>(vp, 4));
        Scene_BeginOverlay(s);
        Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<uint32_t*>(s, 0x55));
        s[0x374d] = 1;
        s[0x374e] = 1;
        Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 1, -1);
        Render_CullNone(F<uint8_t*>(s, 0x2d));
    };
    int32_t stages;
    if (hook::As<uint32_t>(g_6d4034)) {
        uint8_t* eng = g_engine;
        if (F<int32_t>(eng, 0x58) <= 0) return;
        PtrList* items = F<PtrList*>(eng, 0x11369d);
        viewport();
        UiList_Draw(s, 0, items, F<int32_t>(g_engine, 0x58));
        stages = 1;
    } else {
        uint8_t* eng = g_engine;
        PtrList* plain = F<PtrList*>(eng, 0x1136a1);
        PtrList* textured = F<PtrList*>(eng, 0x1136a5);
        if (F<int32_t>(plain, 0xc) <= 0 && F<int32_t>(textured, 0xc) <= 0) return;
        F<uint32_t>(F<uint8_t*>(s, 0x29), 0x29) = 0;
        Render_ApplyFilterStates(F<uint8_t*>(s, 0x2d));
        viewport();
        PtrList_Rewind(textured);
        while (uint8_t* e = (uint8_t*)PtrList_Next(textured)) SceneList_Draw(s, 0, e, F<PtrList*>(e, 0xe4));
        Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<uint32_t*>(s, 0x59));
        SceneList_Draw(s, 0, nullptr, plain);
        stages = 2;
    }
    Render_PopStages(F<uint8_t*>(s, 0x2d), 0, stages);
    Render_FogOn(F<uint8_t*>(s, 0x2d));
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    Render_ApplyStoredTransforms(F<uint8_t*>(s, 0x2d));
    F<uint32_t>(F<uint8_t*>(s, 0x29), 0x29) = light;
    Render_ApplyFilterStates(F<uint8_t*>(s, 0x2d));
    if ((uint8_t)frame) Render_EndFrame(F<uint8_t*>(s, 0x2d), 0, 0);
}
RT3_REPLACE(0x565110, Scene_DrawOverlay);

// 0x565370. Visit grid for n cells per side: rounded up to a power of two (+0x1f418
// = log2, +0x3e833 = size), (size + 1)^2 flags (+0x1f414), 0x144 stamps (+0x1f40c, row stride 17).
uint8_t* __fastcall VisitGrid_Init(uint8_t* g, int, int32_t n) {
    F<int32_t>(g, 0x1f418) = n;
    g[0x3e832] = 0;
    F<int32_t>(g, 4) = 0;
    F<int32_t>(g, 0x1f41c) = 0;
    int32_t bits = 0;
    if (n - 1 != 0) {
        int32_t v = F<int32_t>(g, 0x1f418) - 1;
        do {
            ++bits;
        } while ((v >> (bits & 31)) != 0);
    }
    int32_t size = 1 << bits;
    F<int32_t>(g, 0x1f418) = bits;
    uint32_t cells = (uint32_t)((size + 1) * (size + 1));
    F<int32_t>(g, 0x3e833) = size;
    F<uint8_t*>(g, 0x1f414) = (uint8_t*)crt::malloc(cells);
    F<int32_t>(g, 0x1f408) = 1;
    uint32_t* st = (uint32_t*)crt::malloc(0x510);
    F<uint32_t*>(g, 0x1f40c) = st;
    for (int k = 0; k < 0x144; ++k) st[k] = 0;
    if (uint8_t* fl = F<uint8_t*>(g, 0x1f414)) {
        g[0x3e832] = 1;
        for (uint32_t k = 0; k < cells; ++k) fl[k] = 0;
    }
    F<uint32_t*>(g, 0) = F<uint32_t*>(g, 0x1f40c);
    F<int32_t>(g, 0x3e83b) = 1 << F<int32_t>(g, 0x1f418);
    return g;
}
RT3_REPLACE(0x565370, VisitGrid_Init);

RT3_NOINLINE void __fastcall VisitGrid_Free(uint8_t* g) {
    if (!g[0x3e832]) return;
    crt::free(F<void*>(g, 0x1f414));
    crt::free(F<void*>(g, 0x1f40c));
}
RT3_REPLACE(0x565440, VisitGrid_Free);

// 0x565470. Marks block (x, y) visited this pass (+0x1f408) and appends it to the
// output list (+0x3e820) once.
RT3_NOINLINE void __fastcall VisitGrid_Visit(uint8_t* g, int, int16_t x, int16_t y) {
    uint32_t* cell = F<uint32_t*>(g, 0x1f40c) + (y * 0x11 + x);
    uint32_t stamp = F<uint32_t>(g, 0x1f408);
    if (*cell == stamp) return;
    *cell = stamp;
    // The original's order of loads and stores: the list pointer is read
    // again after every store (a store may land on it).
    int16_t* volatile& out = *(int16_t* volatile*)(g + 0x3e820);
    *out = x;
    out = out + 1;
    *out = y;
    int16_t* next = out + 1;
    const int32_t n = *(volatile int32_t*)(g + 0x1f41c) + 1;
    out = next;
    *(volatile int32_t*)(g + 0x1f41c) = n;
}
RT3_REPLACE(0x565470, VisitGrid_Visit);

RT3_NOINLINE int32_t __fastcall VisitGrid_Log2(uint8_t* g) { return F<int32_t>(g, 0x1f418); }
RT3_REPLACE(0x5654f0, VisitGrid_Log2);

RT3_NOINLINE uint8_t* __fastcall VisitGrid_Flags(uint8_t* g) { return F<uint8_t*>(g, 0x1f414); }
RT3_REPLACE(0x565500, VisitGrid_Flags);

// 0x565510. Triangulates a 16x16 terrain patch at (x0, y0) from its level grid
// (+0x1f414, row shift +0x1f418, stride +0x3e83b): a breadth-first walk from the
// top-left block (queue +0x1f420, visited stamps +0x1f40c / +0x1f408) takes each
// block of size 2^(level-1) whose corners are present, queues its right and lower
// neighbours, and fans it into triangles from the top-left corner over the top and
// right edge vertices, then from the bottom-right corner over the bottom and left
// edge vertices (scratch lists 0xd979b0 / 0xd97a30). Writes 16-bit (x, y) vertex
// triples to +8.. (pointer +0x3e824), counting vertices in +4. Quirk: when the stamp
// wraps, only 81 of the 289 visited entries are cleared.
void __fastcall TerrainPatch_Triangulate(uint8_t* g, int, int32_t x0, int32_t y0) {
    F<int32_t>(g, 0x3e837) = (y0 << (F<int32_t>(g, 0x1f418) & 31)) + x0;
    uint32_t stamp = F<uint32_t>(g, 0x1f408) + 1;
    int16_t* q = &F<int16_t>(g, 0x1f420);
    F<uint32_t>(g, 0x1f408) = stamp;
    if (stamp > 0xfffffff0u) {
        F<uint32_t>(g, 0x1f408) = 1;
        memset(F<void*>(g, 0x1f40c), 0, 0x51 * 4);
    }
    F<int16_t*>(g, 0x3e820) = q;
    F<int16_t*>(g, 0x3e824) = &F<int16_t>(g, 8);
    uint8_t* grid = F<uint8_t*>(g, 0x1f414) + (0 << (F<int32_t>(g, 0x1f418) & 31)) + F<int32_t>(g, 0x3e837);
    F<int32_t>(g, 0x1f41c) = 0;
    VisitGrid_Visit(g, 0, 0, 0);
    F<uint8_t*>(g, 0x3e828) = grid;
    auto emit = [&](int32_t v) {
        int16_t*& o = F<int16_t*>(g, 0x3e824);
        *o = (int16_t)v;
        ++o;
    };
    int32_t* listA = (int32_t*)0xd979b0; // top / bottom edge
    int32_t* listB = (int32_t*)0xd97a30; // right / left edge
    int32_t done = 0;
    while (F<int32_t>(g, 0x1f41c) > done) {
        int32_t x = q[0], y = q[1];
        q += 2;
        ++done;
        uint8_t* cell = (y << (F<int32_t>(g, 0x1f418) & 31)) + F<uint8_t*>(g, 0x3e828) + x;
        int32_t sz = (int32_t)(1u << ((cell[0] - 1u) & 31));
        int32_t xs = x + sz, ys = y + sz;
        int32_t stride = F<int32_t>(g, 0x3e83b);
        int32_t down = stride * sz;
        if (!cell[down + sz] || !cell[sz]) {
            F<uint8_t*>(g, 0x3e828) = grid;
            continue;
        }
        if (xs < 16) VisitGrid_Visit(g, 0, (int16_t)xs, (int16_t)y);
        if (!cell[down]) {
            F<uint8_t*>(g, 0x3e828) = grid;
            continue;
        }
        if (ys < 16) VisitGrid_Visit(g, 0, (int16_t)x, (int16_t)ys);
        // Top-left fan over the top edge (x+1..x+s) and right edge (bottom up).
        int32_t nA = 0, nB = 0;
        uint8_t* top = cell;
        uint8_t* right = cell + down + sz;
        for (int32_t j = sz, k = x; j;) {
            ++top;
            right -= stride;
            --j;
            ++k;
            if (*right) listB[nB++] = j + y;
            if (*top) listA[nA++] = k;
        }
        int32_t ax = x + x0, topY = y + y0, rx = xs + x0, by = ys + y0;
        int32_t *pa = listA, *pb = listB;
        for (;;) {
            if (!nA && !nB) break;
            if (nB < nA) {
                if (nA) {
                    int32_t v = *pa++;
                    --nA;
                    emit(ax), emit(topY), emit(rx), emit(by), emit(x0 + v), emit(y + y0);
                    F<int32_t>(g, 4) += 3;
                    ax = v + x0;
                }
                if (nB < nA) continue;
            }
            if (!nB) continue;
            int32_t v = *pb++;
            --nB;
            emit(ax), emit(topY), emit(rx), emit(by), emit(rx), emit(v + y0);
            F<int32_t>(g, 4) += 3;
            by = v + y0;
        }
        // Bottom-right fan over the bottom edge (right to left) and left edge (top down).
        int32_t cx = xs + x0, ly = y + y0, botY = ys + y0, lx = x + x0;
        nA = nB = 0;
        uint8_t* left = cell;
        uint8_t* bottom = cell + down;
        for (int32_t j = sz, yy = y; j;) {
            left += stride;
            --j;
            ++yy;
            if (*left) listB[nB++] = yy;
            if (bottom[j]) listA[nA++] = j + x;
        }
        pa = listA;
        pb = listB;
        for (;;) {
            if (!nA && !nB) break;
            if (nB < nA) {
                if (nA) {
                    int32_t v = *pa++;
                    --nA;
                    emit(cx), emit(botY), emit(lx), emit(ly), emit(x0 + v), emit(botY);
                    F<int32_t>(g, 4) += 3;
                    cx = v + x0;
                }
                if (nB < nA) continue;
            }
            if (!nB) continue;
            int32_t v = *pb++;
            --nB;
            emit(cx), emit(botY), emit(lx), emit(ly), emit(lx), emit(v + y0);
            F<int32_t>(g, 4) += 3;
            ly = v + y0;
        }
    }
}
RT3_REPLACE(0x565510, TerrainPatch_Triangulate);

void __fastcall Nop565c10() { __asm nop }
RT3_REPLACE(0x565c10, Nop565c10);

void __fastcall Obj_GetVec0(uint32_t* o, int, uint32_t* out) {
    out[0] = o[0];
    out[1] = o[1];
    out[2] = o[2];
}
RT3_REPLACE(0x565c20, Obj_GetVec0);

void __fastcall Obj_SetPair10(uint8_t* o, int, uint32_t a, uint32_t b, uint8_t c) {
    F<uint32_t>(o, 0x10) = a;
    F<uint32_t>(o, 0x14) = b;
    o[0xd8] = c;
}
RT3_REPLACE(0x565c40, Obj_SetPair10);

uint8_t __fastcall Obj_GetPair10(uint8_t* o, int, uint32_t* a, uint32_t* b) {
    if (a) *a = F<uint32_t>(o, 0x10);
    if (b) *b = F<uint32_t>(o, 0x14);
    return o[0xd8];
}
RT3_REPLACE(0x565c60, Obj_GetPair10);

// 0x565c90. Draws particle i (16-byte records +0x18..: scale, size, sprite, ARGB)
// at (x + dx * scale, y + dy * scale), faded by +0xd8 (sprite UVs 0x625a98).
void __fastcall Particles_Draw(uint8_t* p, int, int32_t i, float x, float y, float dx, float dy) {
    uint8_t* e = p + i * 16;
    uint32_t argb = F<uint32_t>(e, 0x24);
    int32_t alpha = (int32_t)(argb >> 24) - p[0xd8];
    if (alpha <= 0) return;
    double cx = (double)dx * F<float>(e, 0x18) + x;
    const float* uv = (const float*)(0x625a98 + F<int32_t>(p, (i + 2) * 16) * 16);
    double cy = (double)dy * F<float>(e, 0x18) + y;
    float size = F<float>(e, 0x1c);
    uint32_t col = (argb & 0xffffff) | ((uint32_t)alpha << 24);
    float bottom = (float)(size + cy), right = (float)(size + cx), top = (float)(cy - size), left = (float)(cx - size);
    Engine_QueueSprite(hook::As<void*>(g_engine), 0, Bits(left), Bits(top), Bits(right), Bits(bottom), 0x3e800000,
                       col, g_d97ab0, Bits(uv[0]), Bits(uv[1]), Bits(uv[2]), Bits(uv[3]),
                       0x3f800000);
}
RT3_REPLACE(0x565c90, Particles_Draw);

void __fastcall Texture_SetD97ab0(const char* name) {
    if (name) Texture_Replace(0xd97ab0, name);
}
RT3_REPLACE(0x565d60, Texture_SetD97ab0);

RT3_NOINLINE void ReleaseResourceD97ab0() { ReleaseResourceIn(0xd97ab0); }
RT3_REPLACE(0x565da0, ReleaseResourceD97ab0);

// 0x565dc0. Takes the first vertex of collision file `name`'s mesh (when it has
// any) into +0..+8 and the default table 0x625ae8 into +0x18 (0xc0 bytes); the
// second argument is unused.
uint8_t* __fastcall Object_InitFromCollision(uint8_t* o, int, const char* name, int32_t) {
    uint8_t* cf = Pack_AcquireCollision(g_pack, 0, name);
    uint8_t* mesh = F<uint8_t*>(F<uint8_t*>(cf, 0), 0xc);
    if (F<int32_t>(mesh, 8)) memcpy(o, F<uint8_t*>(mesh, 0), 0xc);
    memcpy(o + 0x18, (const void*)0x625ae8, 0xc0);
    Pack_Release(g_pack, 0, cf);
    return o;
}
RT3_REPLACE(0x565dc0, Object_InitFromCollision);

void __fastcall Nop565e20() { __asm nop }
RT3_REPLACE(0x565e20, Nop565e20);

// 0x565e30. Emitter defaults.
void __fastcall Emitter_Init(uint8_t* e) {
    for (int k = 0; k < 0x13; ++k) F<int32_t>(e, k * 4) = 0;
    F<int32_t>(e, 0x4c) = 1;
    F<int32_t>(e, 0x70) = 0;
    F<int32_t>(e, 0x74) = 0;
    F<int32_t>(e, 0x54) = 0;
    F<int32_t>(e, 0x58) = 0;
}
RT3_REPLACE(0x565e30, Emitter_Init);

// 0x565e80. Starts an emitter: `count` particles over `life` from position `from`
// toward `to`; the colour comes from 0x625bc4.. unless fixed (flag 0x40000000).
void __fastcall Emitter_Start(uint8_t* e, int, int32_t count, const uint32_t* from, const uint32_t* to, uint32_t start,
                              float life) {
    if (!count) return;
    F<uint32_t>(e, 0x44) = start;
    uint32_t fl = F<uint32_t>(e, 0x5c);
    F<int32_t>(e, 0x4c) = count;
    F<int32_t>(e, 0x48) = count;
    F<float>(e, 0x68) = (float)((double)life * count + *(const float*)&start);
    double k = (int8_t)fl < 0 ? *(const float*)0x5cb490 : *(const float*)0x5ddb0c;
    F<float>(e, 0x50) = (float)(k / count);
    if (!(fl & 0x40000000)) {
        F<uint32_t>(e, 0x18) = g_625bc4;
        F<uint32_t>(e, 0x1c) = g_625bc8;
        F<uint32_t>(e, 0x20) = g_625bcc;
        F<uint32_t>(e, 0x24) = g_625bac;
    }
    for (int k2 = 0; k2 < 3; ++k2) F<uint32_t>(e, 0xc + k2 * 4) = to[k2];
    for (int k2 = 0; k2 < 3; ++k2) F<uint32_t>(e, k2 * 4) = from[k2];
    if (fl & 0x400000) F<float>(e, 0x64) = (float)((double)life + life);
    else F<float>(e, 0x64) = life;
}
RT3_REPLACE(0x565e80, Emitter_Start);

// 0x565f40. Advances a particle by `dt`: counts its life (+0x48, of +0x4c) down,
// returning false once spent; moves it by its velocity (+0xc..+0x14) scaled by the
// remaining / elapsed life fraction (flags +0x5c 0x100 / 0x10000000), blending
// towards the end velocity (+0x18..+0x20) over the late part of its life (0x40000000)
// or drifting with the wind (0x625ba8.., growing +0x24 toward its limits) unless
// paused; spins +0x40 (0x20080000), applies gravity (0x800, +0x6c) and grows its
// size +0x44 (by +0x64, at least 0.01).
int32_t __fastcall Particle_Advance(uint8_t* p, int, float dt) {
    if (!(Fl_190(0x5c8598) < dt) && !(dt != dt)) return 1;
    int32_t n = Ftol((double)F<int32_t>(p, 0x48) - dt);
    F<int32_t>(p, 0x48) = n;
    if (n <= 0) {
        F<int32_t>(p, 0x48) = 0;
        return 0;
    }
    uint32_t flags = F<uint32_t>(p, 0x5c);
    double s = dt;
    if (flags & 0x100) s = (double)n / F<int32_t>(p, 0x4c) * dt;
    else if (flags & 0x10000000) s = (Fl_190(0x5c85b4) - (double)n / F<int32_t>(p, 0x4c)) * dt;
    if (flags & 0x20080000) F<float>(p, 0x40) = (float)((double)F<float>(p, 0x3c) + F<float>(p, 0x40));
    bool done = false;
    if (flags & 0x40000000) {
        double a = (double)n / F<int32_t>(p, 0x4c);
        double b = Fl_190(0x5c85b4) - F<float>(p, 0x28);
        if (a < b || a == b) {
            double t = a / b, u = Fl_190(0x5c85b4) - t;
            F<float>(p, 0) = (float)((u * F<float>(p, 0xc) + t * F<float>(p, 0x18)) * s + F<float>(p, 0));
            F<float>(p, 4) = (float)((t * F<float>(p, 0x20) + u * F<float>(p, 0x10)) * s + F<float>(p, 4));
            F<float>(p, 8) = (float)((u * F<float>(p, 0x14) + t * F<float>(p, 0x1c)) * s + F<float>(p, 8));
            done = true;
        }
    }
    if (!done) {
        float a = (float)((double)n / F<int32_t>(p, 0x4c));
        float b = (float)(Fl_190(0x5c85b4) - F<float>(p, 0x28));
        if ((a < b || a == b) && !g_d93998 && !g_d97af4) {
            double w = Fl_190(0x5c85b4) - (double)a / b;
            const float wind = *(const float*)0x625ba8;
            F<float>(p, 0x18) = (float)((double)*(const float*)0x625bb8 * F<float>(p, 0x24));
            F<float>(p, 0x1c) = (float)((double)*(const float*)0x625bbc * F<float>(p, 0x24));
            F<float>(p, 0x20) = (float)((double)*(const float*)0x625bc0 * F<float>(p, 0x24));
            F<float>(p, 0) = (float)(((double)wind * F<float>(p, 0x18) * w + F<float>(p, 0xc)) * s + F<float>(p, 0));
            F<float>(p, 4) = (float)(((double)wind * w * F<float>(p, 0x1c) + F<float>(p, 0x10)) * s + F<float>(p, 4));
            F<float>(p, 8) = (float)(((double)wind * F<float>(p, 0x20) * w + F<float>(p, 0x14)) * s + F<float>(p, 8));
            const float gust = *(const float*)0x625bb0;
            if ((gust > Fl_190(0x5c8598) && F<float>(p, 0x24) < *(const float*)0x625bb4) ||
                (gust < Fl_190(0x5c8598) && F<float>(p, 0x24) > *(const float*)0xd97af0))
                F<float>(p, 0x24) = (float)((double)gust * Fl_190(0x5cadc0) + F<float>(p, 0x24));
        } else {
            F<float>(p, 0) = (float)(s * F<float>(p, 0xc) + F<float>(p, 0));
            F<float>(p, 4) = (float)(s * F<float>(p, 0x10) + F<float>(p, 4));
            F<float>(p, 8) = (float)(s * F<float>(p, 0x14) + F<float>(p, 8));
        }
    }
    if (flags & 0x800) F<float>(p, 0x10) = (float)((double)F<float>(p, 0x10) - (double)dt * F<float>(p, 0x6c));
    double size = (double)dt * F<float>(p, 0x64) + F<float>(p, 0x44);
    F<float>(p, 0x44) = (float)size;
    if (size < Fl_190(0x5cb5c0)) F<uint32_t>(p, 0x44) = 0x3c23d70a;
    return 1;
}
RT3_REPLACE(0x565f40, Particle_Advance);

// 0x5661b0. Rotates four points by `angle` about the axis 0xd97b98 (D3DX).
// D3DXVec3TransformCoord (0x56cd3c) stores a fourth float past its output, so the
// output buffer has a spare slot (as on the original's stack).
RT3_NOINLINE void __stdcall Points_RotateAxis(float angle, float* pts) {
    float m[16] = {};
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    ((void*(__stdcall*)(float*, const void*, float))0x56df7d)(m, (const void*)0xd97b98, angle);
    for (int k = 0; k < 4; ++k) {
        float in[3] = { pts[k * 3], pts[k * 3 + 1], pts[k * 3 + 2] }, out[4];
        ((void*(__stdcall*)(float*, const float*, const float*))0x56cd3c)(out, in, m);
        pts[k * 3] = out[0];
        pts[k * 3 + 1] = out[1];
        pts[k * 3 + 2] = out[2];
    }
}
RT3_REPLACE(0x5661b0, Points_RotateAxis);

// 0x5662a0. Emits particle p as a camera-facing quad (two triangles, 6 vertices of
// {xyz, colour, uv}) at `at` + its position: corners are the camera vectors
// 0xd97abc..0xd97ae8 scaled by its size (+0x44), spun by +0x40 (0x20080000) into
// the scratch quad 0xd97b38; alpha fades by age (+0x48 of +0x4c): flag 0x20 fades
// in over the first 30%, 0x40 / 0x80 scale by +0x50, else it fades in over 10%.
void __fastcall Particle_ToBillboard(uint8_t* p, int, uint32_t* out, int32_t* count, const float* at) {
    uint32_t fl = F<uint32_t>(p, 0x5c);
    double a;
    if (fl & 0x20) {
        double t = (double)F<int32_t>(p, 0x48) / F<int32_t>(p, 0x4c);
        a = (t <= Fl_205(0x5cadc0) ? t * Fl_205(0x5de3f8) * F<float>(p, 0x60) : (double)F<float>(p, 0x60)) * Fl_205(0x5ca0ec);
    } else if (fl & 0x40) {
        a = (double)(F<int32_t>(p, 0x4c) - F<int32_t>(p, 0x48)) * F<float>(p, 0x60) * F<float>(p, 0x50);
    } else if ((int8_t)fl < 0) {
        double life = F<int32_t>(p, 0x48);
        double t = life / F<int32_t>(p, 0x4c);
        if (t < Fl_205(0x5c8d48)) a = (double)F<float>(p, 0x60) * F<float>(p, 0x50) * life;
        else a = (double)(F<int32_t>(p, 0x4c) - F<int32_t>(p, 0x48)) * F<float>(p, 0x60) * F<float>(p, 0x50);
    } else {
        double t = (double)F<int32_t>(p, 0x48) / F<int32_t>(p, 0x4c);
        a = (t <= Fl_205(0x5cb4a8) ? t * Fl_205(0x5c85f8) * F<float>(p, 0x60) : (double)F<float>(p, 0x60)) * Fl_205(0x5ca0ec);
    }
    uint32_t color = (uint32_t)Ftol(a) << 24 | F<uint32_t>(p, 0x70);
    float c[3];
    for (int i = 0; i < 3; ++i) c[i] = (float)((double)at[i] + F<float>(p, i * 4));
    float* q = (float*)0xd97b38;
    const float* cam = (const float*)0xd97abc;
    // Corner k, component i comes from camera float idx[k][i] (0xd97abc + 4 idx).
    static const int idx[4][3] = { { 9, 2, 11 }, { 6, 4, 7 }, { 5, 8, 3 }, { 1, 10, 0 } };
    double s = F<float>(p, 0x44);
    for (int k = 0; k < 4; ++k)
        for (int i = 0; i < 3; ++i) q[k * 3 + i] = (float)(cam[idx[k][i]] * s);
    if ((fl & 0x20080000) && F<float>(p, 0x40) != Fl_205(0x5c8598)) Points_RotateAxis(F<float>(p, 0x40), q);
    for (int k = 0; k < 4; ++k)
        for (int i = 0; i < 3; ++i) q[k * 3 + i] = (float)((double)q[k * 3 + i] + c[i]);
    *count += 6;
    ++g_d97ab8;
    static const int corner[6] = { 0, 1, 3, 1, 2, 3 };
    static const uint32_t uv[6][2] = { { 0x2c, 0x30 }, { 0x34, 0x30 }, { 0x2c, 0x38 }, { 0x34, 0x30 }, { 0x34, 0x38 }, { 0x2c, 0x38 } };
    for (int v = 0; v < 6; ++v) {
        uint32_t* o = out + v * 6;
        for (int i = 0; i < 3; ++i) o[i] = F<uint32_t>(q, (corner[v] * 3 + i) * 4);
        o[4] = F<uint32_t>(p, uv[v][0]);
        o[5] = F<uint32_t>(p, uv[v][1]);
        o[3] = color;
    }
}
RT3_REPLACE(0x5662a0, Particle_ToBillboard);

// 0x566670. Per-frame particle setup from the camera's angles: stores its forward
// axis (0xd97b98), the billboard corner offsets (0xd97abc..0xd97ae8, half-size
// 0x5c9b08 along its right / up axes) and steps the wind gusts: every 32..1024
// frames (counter 0xd97aec, period 0x625bdc) the gust 0x625bb0 may reverse, and the
// wind strength 0x625bac is pushed by it up to 0x625bb4 / down to 0xd97af0.
void __fastcall Particles_FrameSetup(uint8_t* cam) {
    float yaw, pitch;
    float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    float r[16];
    Obj_GetXY10(cam, 0, (uint32_t*)&yaw, (uint32_t*)&pitch);
    auto mul = (void*(__stdcall*)(float*, const float*, const float*))0x56d3cd;
    ((void*(__stdcall*)(float*, float))0x56dda8)(r, -pitch);
    mul(m, m, r);
    ((void*(__stdcall*)(float*, float))0x56de44)(r, yaw);
    mul(m, m, r);
    const double z = Fl_191(0x5c8598), k = Fl_191(0x5c9b08);
    double a = m[8] * z;
    G<uint32_t>(0xd97b98) = ((uint32_t*)m)[8];
    G<uint32_t>(0xd97b9c) = ((uint32_t*)m)[9];
    G<uint32_t>(0xd97ba0) = ((uint32_t*)m)[10];
    double b = m[4] * k + a;
    double t = m[0] * k;
    float rx = (float)t;
    G<float>(0xd97ae0) = (float)(t + b);
    float l4 = (float)(m[9] * z);
    double c = m[5] * k + l4;
    t = m[1] * k;
    float ry = (float)t;
    G<float>(0xd97ac4) = (float)(t + c);
    float l0 = (float)(m[10] * z);
    float l8 = (float)(m[6] * k + l0);
    int32_t counter = G<int32_t>(0xd97aec);
    t = m[2] * k;
    float rz = (float)t;
    G<float>(0xd97ae8) = (float)(t + l8);
    G<float>(0xd97ad4) = (float)(m[0] + b);
    G<float>(0xd97acc) = (float)(m[1] + c);
    G<float>(0xd97ad8) = (float)(l8 + (double)m[2]);
    t = m[4] + a;
    l8 = (float)t;
    G<float>(0xd97ad0) = (float)(t + m[0]);
    t = (double)m[5] + l4;
    l4 = (float)t;
    G<float>(0xd97adc) = (float)(t + m[1]);
    t = (double)m[6] + l0;
    l0 = (float)t;
    G<float>(0xd97ac8) = (float)(t + m[2]);
    G<float>(0xd97ac0) = (float)((double)l8 + rx);
    G<float>(0xd97ae4) = (float)((double)l4 + ry);
    G<float>(0xd97abc) = (float)((double)l0 + rz);
    if (counter < 0) {
        uint32_t& seed = G<uint32_t>(0x624324);
        seed = seed * 0x19660d + 0x3c6ef35f;
        int32_t n = (int32_t)(seed & 0x7fff) % G<int32_t>(0x625bdc);
        G<int32_t>(0xd97aec) = n;
        if (n < 0x20) {
            G<int32_t>(0x625bdc) = 0x400;
            G<float>(0x625bb0) = -G<float>(0x625bb0);
        }
    } else {
        if (G<int32_t>(0x625bdc) > 0x20) --G<int32_t>(0x625bdc);
        --G<int32_t>(0xd97aec);
    }
    float gust = G<float>(0x625bb0);
    if ((gust > z && G<float>(0x625bac) < G<float>(0x625bb4)) || (gust < z && G<float>(0x625bac) > G<float>(0xd97af0)))
        G<float>(0x625bac) = (float)((double)G<float>(0x625bac) + G<float>(0x625bb0));
}
RT3_REPLACE(0x566670, Particles_FrameSetup);

// 0x566980. Draws `text` right-aligned near the top right of the screen in the
// engine's overlay font (+0x1141f5) at half size; `shadow` 0 turns the shadow off.
RT3_NOINLINE void __stdcall Overlay_DrawText(const char* text, int32_t shadow) {
    uint8_t* e = g_engine;
    if (!e || !F<uint8_t*>(e, 0x1141f5)) return;
    Font_DrawTextBox(F<uint8_t*>(e, 0x1141f5), 0, text, BitsF(0x3efccccd), BitsF(0x3bbf258c), 0.5f, 0.5f, 0xffffffff, 3, 1, shadow ? 0 : 2, 0.5f);
}
RT3_REPLACE(0x566980, Overlay_DrawText);

// 0x5669d0. Draws text with a D3DX font in the rectangle given as fractions of the
// back buffer (+0x25c7 x +0x25cb); flags 0x200 / 0x400 pick the alignment.
RT3_NOINLINE void __fastcall Render_DrawFontText(uint8_t* r, int, const char* text, float x0, float y0, float x1, float y1,
                                   uint32_t colour, void** font, uint32_t flags) {
    float w = (float)F<int32_t>(r, 0x25c7), h = (float)F<int32_t>(r, 0x25cb);
    int32_t rc[4] = { (int32_t)((double)x0 * w), (int32_t)((double)h * y0), (int32_t)((double)w * x1),
                      (int32_t)((double)h * y1) };
    if (!font) return;
    void* f = *font;
    ((int32_t(__stdcall*)(void*))(*(void***)f)[0x14 / 4])(f);
    uint32_t fmt = (flags & 0x200) ? 0x40810 : 0x40811 + ((flags & 0x400) ? 1 : 0);
    ((int32_t(__stdcall*)(void*, const char*, int32_t, int32_t*, uint32_t, uint32_t))(*(void***)f)[0x18 / 4])(
        f, text, -1, rc, fmt, colour);
    ((int32_t(__stdcall*)(void*))(*(void***)f)[0x20 / 4])(f);
}
RT3_REPLACE(0x5669d0, Render_DrawFontText);

// 0x566a80. Reads a light point set from the current pack: a 9-dword header
// ({?, count, ?, +0xc, +0x10, r, g, b, a}; colour packed to +8 from 0..1 floats)
// then count points of 12 bytes (+0) with y and z swapped.
RT3_NOINLINE int32_t __stdcall Light_LoadPoints(uint8_t* o) {
    int32_t h[9];
    NameTable_Read(g_pack, 0, h, 0x24);
    const float* hf = (const float*)h;
    F<int32_t>(o, 0x14) = h[5];
    F<int32_t>(o, 0x18) = h[6];
    F<int32_t>(o, 0x1c) = h[7];
    F<int32_t>(o, 4) = h[1];
    F<int32_t>(o, 0x20) = h[8];
    F<int32_t>(o, 0x10) = h[4];
    F<int32_t>(o, 0xc) = h[3];
    const float k = Fl(0x5ca0ec);
    uint32_t c = (uint32_t)(int32_t)((double)hf[5] * k) << 8;
    c = (c | (uint32_t)(int32_t)((double)hf[6] * k)) << 8;
    c = (c | (uint32_t)(int32_t)((double)hf[7] * k)) << 8;
    c |= (uint32_t)(int32_t)((double)hf[8] * k);
    F<uint32_t>(o, 8) = c;
    int32_t n = h[1];
    float* p = (float*)crt::malloc(n * 12);
    F<float*>(o, 0) = p;
    NameTable_Read(g_pack, 0, p, n * 12);
    for (int32_t i = 0; i < h[1]; ++i) {
        float* q = F<float*>(o, 0) + i * 3;
        float y = q[1];
        F<uint32_t>(q, 4) = F<uint32_t>(q, 8);
        q[2] = y;
    }
    return 0;
}
RT3_REPLACE(0x566a80, Light_LoadPoints);

// 0x566b80. Loads collision mesh m from the open pack (0x6d4020): registers it in
// the mesh list 0xd97bb8 (+0x41), reads {?, vertices, faces}, the vertices
// (y / z swapped, recentred by 0xd97ba8 / 0xd97bac / 0xd97bb4; bounds and radius
// +0x14 / top +0x18) and the faces (0x4c bytes: vertex pointers, three normals
// normalised unless shorter than 0.001, 0x18 + 4 more bytes). +0x10 gets
// 10 x (mean face "area" + bounding diagonal squared). Quirk: the face area uses a
// cross product with one sign flipped in each component (+bc instead of -bc).
int32_t __stdcall CollisionMesh_Load(uint8_t* m) {
    PtrList*& meshes = g_d97bb8;
    if (!meshes) {
        PtrList* l = (PtrList*)crt::operator_new(0x18);
        meshes = l ? PtrList_Init(l) : nullptr;
    }
    F<PtrListNode*>(m, 0x41) = PtrList_Append(meshes, 0, m);
    m[0x1c] = 0;
    uint8_t* pack = g_pack;
    int32_t hdr[3];
    NameTable_Read(pack, 0, hdr, 0xc);
    int32_t nv = hdr[1], nf = hdr[2];
    F<int32_t>(m, 8) = nv;
    F<int32_t>(m, 0xc) = nf;
    F<int32_t>(m, 0x39) = -1;
    F<int32_t>(m, 0x3d) = -1;
    float* verts = (float*)crt::malloc(nv * 0xc);
    F<float*>(m, 0) = verts;
    F<int32_t>(m, 0x35) = 0;
    F<int32_t>(m, 0x31) = -1;
    NameTable_Read(g_pack, 0, verts, nv * 0xc);
    double maxZ = Fl_205(0x5c8598);
    F<int32_t>(m, 0x14) = 0;
    F<int32_t>(m, 0x18) = 0;
    float maxX = 0.0f, minX = 0.0f, maxY = 0.0f, minY = 0.0f, minZ = 0.0f;
    for (int32_t i = 0; i < nv; ++i) {
        float* v = F<float*>(m, 0) + i * 3;
        float oldY;
        X87Copy(&oldY, &v[1]);
        F<uint32_t>(v, 4) = F<uint32_t>(v, 8);
        v[2] = oldY;
        v[0] = (float)((double)v[0] - g_d97ba8);
        v[1] = (float)((double)v[1] - g_d97bac);
        v[2] = (float)((double)v[2] - g_d97bb4);
        float x, y, z;
        X87Copy(&x, &v[0]);
        X87Copy(&y, &v[1]);
        X87Copy(&z, &v[2]);
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
        if (z < minZ) minZ = z;
        if (z > maxZ) maxZ = z;
        double r = Sqrt87(((double)z * z + (double)y * y) + (double)x * x);
        if (r > F<float>(m, 0x14)) F<float>(m, 0x14) = (float)r;
        if (F<float>(m, 0x18) < y) F<float>(m, 0x18) = y;
    }
    double dz = maxZ - minZ, dy = (double)maxY - minY, dx = (double)maxX - minX;
    float diag = (float)((dx * dx + dy * dy) + dz * dz);
    F<void*>(m, 4) = nullptr;
    if (nf <= 0) return 0;
    uint8_t* faces = (uint8_t*)crt::malloc(nf * 0x4c);
    float* base = F<float*>(m, 0);
    F<uint8_t*>(m, 4) = faces;
    float area = 0.0f;
    int32_t i = 0;
    for (; i < nf; ++i) {
        uint8_t* f = F<uint8_t*>(m, 4) + i * 0x4c;
        for (int k = 0; k < 3; ++k) {
            int32_t idx;
            NameTable_Read(g_pack, 0, &idx, 4);
            F<float*>(F<uint8_t*>(m, 4) + i * 0x4c, k * 4) = base + idx * 3;
        }
        NameTable_Read(g_pack, 0, f + 0xc, 0x24);
        for (int k = 0; k < 3; ++k) {
            float* n = (float*)(F<uint8_t*>(m, 4) + i * 0x4c + 0xc + k * 0xc);
            float oldY;
            X87Copy(&oldY, &n[1]);
            F<uint32_t>(n, 4) = F<uint32_t>(n, 8);
            n[2] = oldY;
        }
        for (int k = 0; k < 3; ++k) {
            float* n = (float*)(F<uint8_t*>(m, 4) + i * 0x4c + 0xc + k * 0xc);
            double len = Sqrt87(((double)n[2] * n[2] + (double)n[1] * n[1]) + (double)n[0] * n[0]);
            double inv = len < D(0x5c8678) ? (double)Fl_205(0x5c85b4) : Fl_205(0x5c85b4) / len;
            n[0] = (float)(inv * n[0]);
            n[1] = (float)(inv * n[1]);
            n[2] = (float)(inv * n[2]);
        }
        NameTable_Read(g_pack, 0, f + 0x30, 0x18);
        NameTable_Read(g_pack, 0, f + 0x48, 4);
        const float* a = F<float*>(f, 0);
        const float* b = F<float*>(f, 4);
        const float* c = F<float*>(f, 8);
        double a0 = a[0], a1 = a[1], a2 = a[2], b0 = b[0], b1 = b[1], b2 = b[2], c0 = c[0], c1 = c[1], c2 = c[2];
        float cx = (float)(((((b2 * a1 - c2 * a1) - b1 * a2) + c1 * b2) + c2 * b1) + c1 * a2);
        float cy = (float)(((((b0 * a2 - c0 * a2) - b2 * a0) + c0 * b2) + c2 * b0) + c2 * a0);
        double cz = ((((b1 * a0 - c1 * a0) - b0 * a1) + c0 * b1) + c1 * b0) + c0 * a1;
        double len = Sqrt87((cz * cz + (double)cy * cy) + (double)cx * cx);
        area = (float)(len * D(0x5c8628) + area);
    }
    F<float>(m, 0x10) = (float)(((double)area / i + diag) * Fl_205(0x5c85f8));
    return 0;
}
RT3_REPLACE(0x566b80, CollisionMesh_Load);

// 0x567150. Invalidates the cached state (+0x31/+0x39/+0x3d) of all registered meshes (0xd97bb8).
void __fastcall Meshes_InvalidateAll() {
    PtrList* l = g_d97bb8;
    if (!l) return;
    PtrList_Rewind(l);
    for (uint8_t* m = (uint8_t*)PtrList_Next(g_d97bb8); m;
         m = (uint8_t*)PtrList_Next(g_d97bb8)) {
        F<int32_t>(m, 0x39) = -1;
        F<int32_t>(m, 0x3d) = -1;
        F<int32_t>(m, 0x31) = -1;
    }
}
RT3_REPLACE(0x567150, Meshes_InvalidateAll);

// 0x567190. Loads a collision block into out: {?, count}, from format version 3
// (0xd97bb0) a recentring offset (y / z swapped; also the global offset
// 0xd97ba8 / 0xd97bac / 0xd97bb4 for the meshes), then a 12-byte tag peeked at the
// current position: 'I' = count collision meshes (+0xc, 0x45 each), 'L' = count
// light point records (+0x10, 0x24 each).
int32_t __fastcall CollisionBlock_Load(uint8_t*, int, uint32_t* out) {
    int32_t hdr[2];
    NameTable_Read(g_pack, 0, hdr, 8);
    if (g_d97bb0 > 2) {
        uint32_t off[3];
        NameTable_Read(g_pack, 0, off, 0xc);
        X87Copy(&out[0], &off[0]);
        out[1] = off[2];
        out[2] = off[1];
    } else {
        out[2] = out[1] = out[0] = 0;
    }
    hook::As<uint32_t>(g_d97ba8) = out[0];
    hook::As<uint32_t>(g_d97bac) = out[1];
    hook::As<uint32_t>(g_d97bb4) = out[2];
    out[5] = (uint32_t)hdr[1];
    NameTable_PushPos(g_pack);
    char tag[0xc];
    NameTable_Read(g_pack, 0, tag, 0xc);
    NameTable_PopPosition(g_pack);
    if (tag[0] == 'I') {
        out[4] = 0;
        out[3] = (uint32_t)crt::malloc(hdr[1] * 0x45);
        for (int32_t i = 0; i < hdr[1]; ++i) CollisionMesh_Load((uint8_t*)out[3] + i * 0x45);
    } else if (tag[0] == 'L') {
        out[3] = 0;
        out[4] = (uint32_t)crt::malloc(hdr[1] * 0x24);
        for (int32_t i = 0; i < hdr[1]; ++i) Light_LoadPoints((uint8_t*)out[4] + i * 0x24);
    }
    return 0;
}
RT3_REPLACE(0x567190, CollisionBlock_Load);

// 0x5672e0. Frees a model's frames (+0xc, 0x45 bytes; or +0x10, 0x24 bytes) and
// unregisters their meshes. 1 for no model, else 0.
RT3_NOINLINE int32_t __stdcall Model_FreeFrames(uint8_t* m) {
    if (!m) return 1;
    for (int32_t i = 0; i < F<int32_t>(m, 0x14); ++i) {
        if (uint8_t* simple = F<uint8_t*>(m, 0x10)) {
            if (F<int32_t>(simple, i * 0x24 + 4) > 0) crt::free(F<void*>(simple, i * 0x24));
            continue;
        }
        uint8_t* fr = F<uint8_t*>(m, 0xc);
        if (!fr) continue;
        fr += i * 0x45;
        if (F<int32_t>(fr, 0xc) > 0) crt::free(F<void*>(fr, 4));
        PtrList_RemoveNode(g_d97bb8, 0, F<PtrListNode*>(F<uint8_t*>(m, 0xc) + i * 0x45, 0x41));
        PtrList* l = g_d97bb8;
        if (!F<int32_t>(l, 0xc)) {
            PtrList_Destroy(l);
            crt::operator_delete(l);
            g_d97bb8 = nullptr;
        }
        fr = F<uint8_t*>(m, 0xc) + i * 0x45;
        F<void*>(fr, 0x41) = nullptr;
        crt::free(F<void*>(F<uint8_t*>(m, 0xc) + i * 0x45, 0));
        if (void* t = F<void*>(F<uint8_t*>(m, 0xc) + i * 0x45, 0x35)) crt::free(t);
    }
    if (void* fr = F<void*>(m, 0xc)) {
        crt::free(fr);
        return 0;
    }
    if (void* s = F<void*>(m, 0x10)) crt::free(s);
    return 0;
}
RT3_REPLACE(0x5672e0, Model_FreeFrames);

// 0x567400. Loads collision file `id` (pack 0x6d4020): checks the "3DPF" magic
// (fatal, text 0x46), takes the format version (0xd97bb0) and block count (+4),
// and loads each block (0x18 bytes, +0).
void __fastcall CollisionFile_Load(uint8_t* c, int, int32_t id) {
    F<int32_t>(c, 6) = id;
    NameTable_Seek(g_pack, 0, id, 0);
    uint8_t hdr[8];
    NameTable_Read(g_pack, 0, hdr, 8);
    if (hdr[0] != '3' || hdr[1] != 'D' || hdr[2] != 'P' || hdr[3] != 'F') Fatal_Exit(GetString(0x46));
    uint16_t count = *(uint16_t*)(hdr + 6);
    F<uint16_t>(c, 4) = count;
    g_d97bb0 = *(uint16_t*)(hdr + 4);
    F<void*>(c, 0) = crt::malloc(count * 0x18);
    for (int32_t i = 0; i < *(uint16_t*)(hdr + 6); ++i) CollisionBlock_Load(c, 0, (uint32_t*)(F<uint8_t*>(c, 0) + i * 0x18));
}
RT3_REPLACE(0x567400, CollisionFile_Load);

// 0x5674c0. Frees every model of the set (+0, 0x18 bytes, count +4) and the array.
RT3_NOINLINE void __fastcall ModelSet_Free(uint8_t* s) {
    for (int32_t i = 0; i < F<uint16_t>(s, 4); ++i) Model_FreeFrames(F<uint8_t*>(s, 0) + i * 0x18);
    crt::free(F<void*>(s, 0));
}
RT3_REPLACE(0x5674c0, ModelSet_Free);

// 0x567500. Loads collision file `id` into c (see CollisionFile_Load) and returns c.
uint8_t* __fastcall CollisionFile_Construct(uint8_t* c, int, int32_t id) {
    CollisionFile_Load(c, 0, id);
    return c;
}
RT3_REPLACE(0x567500, CollisionFile_Construct);

// 0x567520. Thunk to ModelSet_Free.
void __fastcall ModelSet_FreeThunk(uint8_t* s) { ModelSet_Free(s); }
RT3_REPLACE(0x567520, ModelSet_FreeThunk);

// 0x567530. Of the angles a and a + pi, the one closer (around the circle) to the
// reference angle idx (0x625be0).
RT3_NOINLINE float __fastcall Angle_PickFacing(uint8_t idx, float a) {
    const double pi = *(const float*)0x5c8760, twoPi = *(const float*)0x5c8768;
    float ref = ((const float*)0x625be0)[idx];
    double d1 = (double)ref - a;
    if (d1 < 0) d1 = -d1;
    float b = (float)((double)a + pi);
    double d2 = (double)ref - b;
    if (d2 < 0) d2 = -d2;
    if (d1 > pi) {
        d1 = twoPi - d1;
        if (d1 < 0) d1 = -d1;
    }
    if (d2 > pi) {
        d2 = twoPi - d2;
        if (d2 < 0) d2 = -d2;
    }
    return d1 < d2 ? a : b;
}
RT3_REPLACE(0x567530, Angle_PickFacing);

// 0x5675c0. Finds the closest pair of corners (4 x 12 bytes, x/z) between quads a
// and b, the distance weighted 0.85 on the opposite-corner pairs, under *best and
// at least 0.01: stores them and the distance and averages the quads' +0x39 into
// the corners' +0x35 bytes; corners closer than 0.01 are marked (+0x31 = 1),
// corners marked 2 are skipped.
RT3_NOINLINE void __fastcall Quad_FindJoin(uint8_t* a, uint8_t* b, uint8_t** outA, uint8_t** outB, float* best) {
    static const uint32_t w[16] = { 0x40000000, 0x40000000, 0x40000000, 0x3f59999a, 0x40000000, 0x40000000,
                                    0x3f59999a, 0x40000000, 0x40000000, 0x3f59999a, 0x40000000, 0x40000000,
                                    0x3f59999a, 0x40000000, 0x40000000, 0x40000000 };
    for (int32_t i = 0; i < 4; ++i) {
        uint8_t* p = a + i * 12;
        double px = F<float>(p, 0), pz = F<float>(p, 8);
        for (int32_t j = 0; j < 4; ++j) {
            uint8_t* q = b + j * 12;
            double dz = pz - F<float>(q, 8), dx = px - F<float>(q, 0);
            double d = sqrt(dx * dx + dz * dz) * *(const float*)&w[i * 4 + j];
            if (a[0x31 + i] == 2 || b[0x31 + j] == 2) continue;
            if (d < *best && !(d < D(0x5c85b8) || d != d)) {
                *outA = p;
                *outB = q;
                uint8_t v = (uint8_t)(((int32_t)b[0x39] + a[0x39]) >> 1);
                b[0x35 + j] = v;
                a[0x35 + i] = v;
                *best = (float)d;
            } else if (d < D(0x5c85b8)) {
                a[0x31 + i] = 1;
                b[0x31 + j] = 1;
            }
        }
    }
}
RT3_REPLACE(0x5675c0, Quad_FindJoin);

// 0x567730. Welds the seams of the quad grid (+0x1669, (+0x15d9 + 1) x +0x15d5
// cells) within (x0, y0)..(x1, y1): for each quad, the closest corner pair with a
// neighbour (Quad_FindJoin, under 71.3) is moved to its midpoint on the terrain
// (+ a small lift), and the scan backs up one cell to re-check; at most 32000 welds.
RT3_NOINLINE void __fastcall QuadGrid_Weld(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    x0 = x0 > 0 ? x0 : 0;
    if (x1 >= F<int32_t>(t, 0x15d9)) x1 = F<int32_t>(t, 0x15d9);
    y0 = y0 > 0 ? y0 : 0;
    if (y1 >= F<int32_t>(t, 0x15d5)) y1 = F<int32_t>(t, 0x15d5);
    int32_t budget = 0x7d00;
    auto cellAt = [&](int32_t x, int32_t y) { return F<uint8_t**>(t, 0x1669)[(F<int32_t>(t, 0x15d9) + 1) * y + x]; };
    for (int32_t y = y0; y < y1; ++y) {
        for (int32_t x = x0; x < x1; ++x) {
            uint8_t* cell = cellAt(x, y);
            while (cell) {
                int32_t xm = x - 1 > 0 ? x - 1 : 0;
                int32_t xp = x + 1 < F<int32_t>(t, 0x15d9) ? x + 1 : F<int32_t>(t, 0x15d9);
                int32_t ym = y - 1 > 0 ? y - 1 : 0;
                int32_t yp = y + 1 < F<int32_t>(t, 0x15d5) ? y + 1 : F<int32_t>(t, 0x15d5);
                uint8_t* a = nullptr;
                uint8_t* b = nullptr;
                float best = 71.3f;
                if (ym > yp) break;
                for (int32_t j = ym; j <= yp; ++j)
                    for (int32_t i = xm; i <= xp; ++i) {
                        if (i == x && j == y) continue;
                        if (uint8_t* nb = cellAt(i, j)) Quad_FindJoin(cell, nb, &a, &b, &best);
                    }
                if (!a) break;
                uint8_t* grid = (uint8_t*)Engine_Get1C(*(void**)0x6d4024);
                double mx = ((double)F<float>(b, 0) + F<float>(a, 0)) * *(const double*)0x5c8628;
                float fx = (float)mx;
                double mz = ((double)F<float>(a, 8) + F<float>(b, 8)) * *(const double*)0x5c8628;
                float fz = (float)mz;
                F<float>(b, 0) = (float)mx;
                F<float>(a, 0) = (float)mx;
                F<float>(b, 8) = (float)mz;
                F<float>(a, 8) = (float)mz;
                double h = Terrain_HeightAt(grid, 0, fx, fz) + *(const double*)0x5c8d68;
                F<float>(b, 4) = (float)h;
                F<float>(a, 4) = (float)h;
                if (!budget) break;
                y = y0 > y - 1 ? y0 : y - 1;
                x = x0 > x - 1 ? x0 : x - 1;
                --budget;
                cell = cellAt(x, y);
            }
        }
    }
}
RT3_REPLACE(0x567730, QuadGrid_Weld);

// 0x5679b0. Expands a terrain quad (corners 0/+0xc/+0x18/+0x24, raised by `lift`)
// into two triangles (6 vertices of x, y, z, colour, u, v). The texture cell steps
// through an 8x8 atlas every 88 ms (GetTickCount + phase +0x3a); the colour is the
// tint 0xd97bbc with red/green darkened by each corner's shade (+0x35..), corners
// pinned white by the edge kind +0x30, alpha from the shade unless the corner is
// fixed (+0x31.. == 2).
RT3_NOINLINE void __fastcall Quad_ToVertices(uint8_t* out, uint8_t* q, float lift) {
    uint32_t now = imp::GetTickCount();
    uint32_t step = ((now + F<int32_t>(q, 0x3a)) / 88) & 0xff;
    int32_t cell = 0xff - (int32_t)step;
    const double k = *(const double*)0x5d08a0;
    float fu = (float)((double)(cell & 7) * k);
    double fv = (double)((cell >> 3) & 7) * k;
    uint32_t tint = G<uint32_t>(0xd97bbc);
    int32_t red = (tint >> 16) & 0xff, green = (tint >> 8) & 0xff, blue = tint & 0xff;
    double u1 = fu + *(const double*)0x5de408;
    double u2 = fu + *(const double*)0x5de400;
    double v2 = fv + k;
    uint32_t col[4];
    for (int32_t i = 0; i < 4; ++i) {
        int32_t s = (0xff - q[0x35 + i]) >> 1;
        int32_t r = red - s, g = green - s;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        col[i] = (((uint32_t)r << 8 | (uint32_t)g) << 8) | (uint32_t)blue;
    }
    uint8_t edge = q[0x30];
    if (edge == 2) {
        q[0x38] = 0;
        q[0x36] = 0;
        col[1] = 0xffffff;
        col[3] = 0xffffff;
    }
    uint32_t c0 = col[0];
    if (edge == 1) {
        q[0x37] = 0;
        q[0x35] = 0;
        col[2] = 0xffffff;
        c0 = 0xffffff;
    }
    uint32_t c1 = (uint32_t)q[0x36] << 24 | col[1];
    uint32_t c2 = (uint32_t)q[0x37] << 24 | col[2];
    uint32_t c3 = (uint32_t)q[0x38] << 24 | col[3];
    uint32_t a0 = (uint32_t)q[0x35] << 24 | c0;
    if (q[0x31] == 2) a0 = c0;
    if (q[0x32] == 2) c1 = col[1];
    if (q[0x33] == 2) c2 = col[2];
    if (q[0x34] == 2) c3 = col[3];
    auto put = [&](int32_t i, int32_t corner, uint32_t c, double u, double v) {
        F<uint32_t>(out, i * 0x18) = F<uint32_t>(q, corner * 0xc);
        F<float>(out, i * 0x18 + 4) = (float)((double)lift + F<float>(q, corner * 0xc + 4));
        F<uint32_t>(out, i * 0x18 + 8) = F<uint32_t>(q, corner * 0xc + 8);
        F<uint32_t>(out, i * 0x18 + 0xc) = c;
        F<float>(out, i * 0x18 + 0x10) = (float)u;
        F<float>(out, i * 0x18 + 0x14) = (float)v;
    };
    put(0, 0, a0, u2, v2);
    put(1, 1, c1, u1, v2);
    put(2, 3, c3, u2, fv);
    put(3, 1, c1, u1, v2);
    put(4, 2, c2, u1, fv);
    put(5, 3, c3, u2, fv);
}
RT3_REPLACE(0x5679b0, Quad_ToVertices);

RT3_NOINLINE void __fastcall Lists_FreeItems(uint8_t* o) {
    FreeItems(F<PtrList*>(o, 0x20));
    FreeItems(F<PtrList*>(o, 0x1c));
}
RT3_REPLACE(0x567bf0, Lists_FreeItems);

// 0x567c70 / 0x567ce0. Expands every quad of list +0x1c (flat) / +0x20 (raised by
// `lift`) with tint `tint` into `out`, adding 6 vertices per quad to *count.
void __fastcall Quads_EmitFlat(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t tint) {
    if (!F<int32_t>(F<uint8_t*>(o, 0x1c), 0xc)) return;
    g_d97bbc = tint;
    PtrList_Rewind(F<PtrList*>(o, 0x1c));
    for (uint8_t* q = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x1c)); q; q = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x1c))) {
        Quad_ToVertices(out, q, 0.0f);
        out += 0x90;
    }
    *count += F<int32_t>(F<uint8_t*>(o, 0x1c), 0xc) * 6;
}
RT3_REPLACE(0x567c70, Quads_EmitFlat);

void __fastcall Quads_EmitRaised(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t tint, float lift) {
    if (!F<int32_t>(F<uint8_t*>(o, 0x20), 0xc)) return;
    g_d97bbc = tint;
    PtrList_Rewind(F<PtrList*>(o, 0x20));
    for (uint8_t* q = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x20)); q; q = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x20))) {
        Quad_ToVertices(out, q, lift);
        out += 0x90;
    }
    *count += F<int32_t>(F<uint8_t*>(o, 0x20), 0xc) * 6;
}
RT3_REPLACE(0x567ce0, Quads_EmitRaised);

// 0x567d50. Frees the item lists +0x20 / +0x1c and the lists themselves.
void __fastcall Lists_Destroy(uint8_t* o) {
    Lists_FreeItems(o);
    if (PtrList* l = F<PtrList*>(o, 0x20)) {
        PtrList_Destroy(l);
        crt::operator_delete(l);
    }
    PtrList* l2 = F<PtrList*>(o, 0x1c);
    F<PtrList*>(o, 0x20) = nullptr;
    if (l2) {
        PtrList_Destroy(l2);
        crt::operator_delete(l2);
    }
    F<PtrList*>(o, 0x1c) = nullptr;
}
RT3_REPLACE(0x567d50, Lists_Destroy);

 // namespace

// 0x567da0. Rebuilds the water quads of the 16 x 16 block at (+0, +4) of
// grid g (cells +0x10, 3 bytes; heights +8 / +0xc; size +0x14 x +0x18; map
// +0x24): plain water (kinds 1, 3) as a turned square into list +0x1c; shore
// cells (other wet kinds) centred on their wet neighbours, faced away from
// the land, sized by the depth and its foam flags, into list +0x20 and the
// map's quad slots +0x1669. Nearby shore quads of list +0x1c then share
// their touching edges and are scaled up about their centres. Ends by
// welding the seams around the block.
RT3_NOINLINE void __fastcall Water_BuildQuads(uint8_t* g) {
    Lists_FreeItems(g);
    float baseX = 0.0f, baseZ = 0.0f;
    for (int32_t y = F<int32_t>(g, 4); y < F<int32_t>(g, 4) + 0x10; ++y) {
        int32_t row = (F<int32_t>(g, 0x14) + 1) * y + F<int32_t>(g, 0);
        uint8_t* cell = F<uint8_t*>(g, 0x10) + row * 3;
        int8_t* ha = F<int8_t*>(g, 8) + row;
        int8_t* hb = F<int8_t*>(g, 0xc) + row;
        for (int32_t x = F<int32_t>(g, 0); x < F<int32_t>(g, 0) + 0x10; ++x, cell += 3, ++ha, ++hb) {
            uint8_t* slots = F<uint8_t*>(F<uint8_t*>(g, 0x24), 0x1669);
            int32_t slot = (F<int32_t>(g, 0x14) + 1) * y + x;
            if (!*ha && !*hb) {
                ((uint8_t**)slots)[slot] = nullptr;
                continue;
            }
            uint8_t* q = (uint8_t*)crt::malloc(0x3e);
            float fa = (float)*ha;
            float nb = (float)-(double)*hb;
            QuadDelta d = { fa, nb, fa, nb, fa, nb, fa, nb };
            uint16_t kind = *(uint16_t*)cell & 7;
            if (kind == 1 || kind == 3) {
                memset(q, 0, 0x3e);
                baseX = (float)((double)(x * 0x5c + 0x2e) - ((double)fa + fa));
                q[0x38] = q[0x37] = q[0x36] = q[0x35] = 0xff;
                baseZ = (float)((double)(y * 0x5c + 0x2e) + ((double)nb + nb));
                WaterCorners(fa, nb, baseX, baseZ, &d, (int32_t*)(q + 0x3a));
            } else if (Wet(cell[0])) {
                memset(q, 0, 0x3e);
                // Centre on the wet neighbours (and the cell itself, weight 2).
                double sx = (double)(x * 0xb8 + 0x5c), sz = (double)(y * 0xb8 + 0x5c), cnt = Fl_191(0x5c878c);
                baseX = (float)sx;
                baseZ = (float)sz;
                float fcnt = (float)cnt;
                uint8_t shore = 0;
                int32_t w = F<int32_t>(g, 0x14), h = F<int32_t>(g, 0x18);
                int32_t x0 = x - 1 > 0 ? x - 1 : 0;
                int32_t x1 = x + 1 < w ? x + 1 : w;
                int32_t y0 = y - 1 > 0 ? y - 1 : 0;
                int32_t y1 = y + 1 < h ? y + 1 : h;
                if (y0 <= y1) {
                    for (int32_t yy = y0; yy <= y1; ++yy) {
                        uint8_t* r = F<uint8_t*>(g, 0x10) + (w + 1) * yy * 3;
                        for (int32_t xx = x0; xx <= x1; ++xx) {
                            if (xx == x && yy == y) continue;
                            if (r[xx * 3 + 2] & 0x20) shore = 1;
                            if (Wet(r[xx * 3])) {
                                sx += (double)(xx * 0x5c + 0x2e);
                                sz += (double)(yy * 0x5c + 0x2e);
                                cnt += Fl_191(0x5c85b4);
                            }
                        }
                    }
                    fcnt = (float)cnt;
                    baseZ = (float)sz;
                    baseX = (float)sx;
                }
                uint8_t* cp = F<uint8_t*>(g, 0x10) + ((w + 1) * y + x) * 3;
                // Facing: away from the wet neighbours (8 directions).
                float ang[8] = { 32.0f, 32.0f, 32.0f, 32.0f, 32.0f, 32.0f, 32.0f, 32.0f };
                static const uint32_t dirAngle[8] = { 0x4096cbe4, 0x407b53d2, 0x40490fdb, 0x4016cbe4, 0x3fc90fdb, 0x3f490fdb, 0, 0x40afede0 };
                float best = 32.0f;
                int32_t near_ = 0;
                for (int32_t pass = 0; pass < 2; ++pass) {
                    for (int k = 0; k < 8; ++k) {
                        int32_t nx = G<int32_t>(0x624b28 + k * 4) + x, ny = G<int32_t>(0x624b48 + k * 4) + y;
                        if (nx < 0 || ny < 0 || nx >= w || ny >= h) continue;
                        uint8_t* c = F<uint8_t*>(g, 0x10) + ((w + 1) * ny + nx) * 3;
                        if (!Wet(c[0])) continue;
                        if (!pass) ++near_;
                        else if (near_ < 3 || !(c[2] & 0x20)) *(uint32_t*)&ang[k] = dirAngle[k];
                    }
                }
                int32_t used = 0;
                bool mixed = false;
                for (int k = 0; k < 8; ++k) {
                    if (*(uint32_t*)&ang[k] == 0x42000000) continue;
                    if (best == *(const float*)0x5c8c40) {
                        best = ang[k];
                        used = 1;
                        continue;
                    }
                    ++used;
                    mixed = true;
                    double a = ang[k], b = best;
                    if (fabs((double)ang[k] - best) > *(const double*)0x5cb4a0) {
                        if (best < ang[k]) b = (double)best + *(const float*)0x5c8768;
                        else a = (double)ang[k] + *(const float*)0x5c8768;
                    }
                    float avg = (float)((b + a) * *(const double*)0x5c8628);
                    best = Wrap2Pi(avg);
                }
                if (best == *(const float*)0x5c8c40) *(uint32_t*)&best = 0x3fc90fdb;
                if (!mixed) {
                    best = (float)((double)best - *(const float*)0x5c8764);
                    if (best < *(const float*)0x5c8598) best = (float)((double)best + *(const float*)0x5c8768);
                }
                uint16_t word = *(uint16_t*)cp;
                best = (float)((double)Angle_PickFacing((uint8_t)((word & 0xff) >> 3 & 7), best) + *(const float*)0x5c8760);
                if (fcnt > *(const float*)0x5c8900 && fcnt < *(const float*)0x5c8554) {
                    double k = *(const double*)0x5c8770 / fcnt;
                    baseX = (float)((double)baseX * k);
                    baseZ = (float)(k * baseZ);
                } else {
                    baseX = (float)(x * 0x5c + 0x2e);
                    baseZ = (float)(y * 0x5c + 0x2e);
                }
                double level = *(const float*)(0x625c40 + ((word >> 6) & 7) * 4);
                uint16_t k7 = word & 7;
                if (k7 == 5 || k7 == 4) {
                    uint8_t* map = F<uint8_t*>(g, 0x24);
                    int32_t depth = 6;
                    for (int32_t dy = -6; dy <= 6; ++dy) {
                        for (int32_t dx = -6; dx <= 6; ++dx) {
                            int32_t cx = x + dx, cy = y + dy;
                            if (cx < 0 || cx >= F<int32_t>(map, 0x15d9) - 1 || cy < 0 || cy >= F<int32_t>(map, 0x15d5) - 1) continue;
                            if ((F<uint8_t*>(g, 0x10)[(w + 1) * cy * 3 + (x - 6) * 3 + (dx + 6) * 3] & 7) != 2) continue;
                            int32_t dd = (dy < 0 ? -dy : dy) + (dx < 0 ? -dx : dx);
                            if (depth >= dd) depth = dd;
                        }
                    }
                    double v = depth < 6 ? *(const double*)0x5c8770 - (double)depth * *(const float*)0x5de448 : (double)*(const float*)0x5c8598;
                    q[0x39] = (uint8_t)QuadFtol(v * *(const double*)0x5ca110);
                    if (!q[0x39]) q[0x39] = 1;
                } else {
                    q[0x39] = k7 == 3 ? 0x73 : 0xff;
                }
                uint8_t alpha = q[0x39];
                q[0x38] = q[0x37] = q[0x36] = q[0x35] = alpha;
                float scale = (float)((double)(0xff - alpha) * *(const double*)0x5dd8f8 + level);
                double rv = (double)HashRand(x * 7 + y * 0x3d, -0x100, 0x100) * *(const double*)0x5d1160;
                if (used > 2) rv = *(const float*)0x5c8598;
                if (shore && scale > *(const double*)0x5c8770) scale = 1.0f;
                uint16_t cw = *(uint16_t*)cp;
                double e = *(const float*)0x5c8598;
                float sxs = 1.0f, szs = 1.0f, off = 0.0f;
                bool foamX = (cw >> 11 & 1) != 0;
                if (cp[2] & 0x20) {
                    if (foamX) {
                        *(uint32_t*)&off = 0xc2133333;
                        sxs = 1.5f;
                        e = SinCos32(best) + *(const float*)0x5c8700;
                    }
                    if (cw & 0x400) {
                        szs = 1.5f;
                        off = (float)((double)*(const float*)0x5cb24c - SinCos32(best));
                        e = *(const float*)0x5de43c;
                    }
                } else {
                    if (foamX) {
                        *(uint32_t*)&off = 0xc1b80000;
                        sxs = 0.5f;
                    }
                    if (cw & 0x400) {
                        szs = 0.5f;
                        e = *(const float*)0x5dda44;
                    }
                }
                float e1 = (float)(e + *(const float*)0x5de438);
                double hs = scale;
                double ax = (double)sxs * hs * *(const double*)0x5de430;
                float p1 = (float)(ax + rv);
                float p2 = (float)(rv - ax);
                float o = (float)((double)off - *(const float*)0x5de438);
                double az = (double)szs * hs * *(const double*)0x5de430;
                float p3 = (float)(rv - az);
                float p4 = (float)(az + rv);
                if (foamX) {
                    best = (float)((double)*(const float*)(0x625be0 + ((cw >> 3) & 7) * 4) + *(const float*)0x5c8760);
                    q[0x32] = 2;
                    q[0x31] = 2;
                }
                uint16_t cw2 = *(uint16_t*)cp;
                if (cw2 & 0x400) {
                    best = (float)((double)*(const float*)(0x625be0 + ((cw2 >> 3) & 7) * 4) + *(const float*)0x5c8760);
                    q[0x34] = 2;
                    q[0x33] = 2;
                }
                *(int32_t*)(q + 0x3a) = (x + y) * 2;
                ShoreCorners(best, e1, p1, p2, o, p3, p4, &d);
            }
            float* qf = (float*)q;
            qf[0] = (float)((double)baseX + d.d0x);
            qf[2] = (float)((double)baseZ + d.d0z);
            qf[3] = (float)((double)baseX + d.d1x);
            qf[5] = (float)((double)baseZ + d.d1z);
            qf[6] = (float)((double)baseX + d.d2x);
            qf[8] = (float)((double)baseZ + d.d2z);
            qf[9] = (float)((double)baseX + d.d3x);
            qf[0xb] = (float)((double)baseZ + d.d3z);
            uint8_t* map = F<uint8_t*>(g, 0x24);
            if (Wet(cell[0])) {
                float h1 = (float)(Terrain_HeightAt(map, 0, (float)(((double)qf[3] + qf[0]) * *(const double*)0x5c8628),
                                                    (float)(((double)qf[5] + qf[2]) * *(const double*)0x5c8628)) +
                                   *(const float*)0x5c8554);
                float h2 = (float)(Terrain_HeightAt(F<uint8_t*>(g, 0x24), 0, (float)(((double)qf[6] + qf[9]) * *(const double*)0x5c8628),
                                                    (float)(((double)qf[0xb] + qf[8]) * *(const double*)0x5c8628)) +
                                   *(const float*)0x5c8554);
                qf[1] = h1;
                qf[7] = h2;
                qf[4] = h1;
                qf[0xa] = h2;
            } else {
                qf[1] = (float)(Terrain_HeightAt(map, 0, qf[0], qf[2]) + *(const float*)0x5c878c);
                qf[4] = (float)(Terrain_HeightAt(F<uint8_t*>(g, 0x24), 0, qf[3], qf[5]) + *(const float*)0x5c878c);
                qf[7] = (float)(Terrain_HeightAt(F<uint8_t*>(g, 0x24), 0, qf[6], qf[8]) + *(const float*)0x5c878c);
                qf[0xa] = (float)(Terrain_HeightAt(F<uint8_t*>(g, 0x24), 0, qf[9], qf[0xb]) + *(const float*)0x5c878c);
            }
            q[0x30] = 0;
            uint16_t k = *(uint16_t*)cell & 7;
            slots = F<uint8_t*>(F<uint8_t*>(g, 0x24), 0x1669);
            slot = (F<int32_t>(g, 0x14) + 1) * y + x;
            if ((k == 1 || k == 3) && !(cell[2] & 0x10)) {
                PtrList_Append(F<PtrList*>(g, 0x1c), 0, q);
            } else if ((k == 2 || k == 4 || k == 5) && q[0x39]) {
                PtrList_Append(F<PtrList*>(g, 0x20), 0, q);
                ((uint8_t**)F<uint8_t*>(F<uint8_t*>(g, 0x24), 0x1669))[(F<int32_t>(g, 0x14) + 1) * y + x] = q;
                continue;
            } else {
                crt::free(q);
            }
            ((uint8_t**)F<uint8_t*>(F<uint8_t*>(g, 0x24), 0x1669))[(F<int32_t>(g, 0x14) + 1) * y + x] = nullptr;
        }
    }
    int32_t n = F<int32_t>(F<uint8_t*>(g, 0x1c), 0xc);
    if (n > 1) {
        float** quads = (float**)crt::malloc(n * 4);
        PtrList_Rewind(F<PtrList*>(g, 0x1c));
        float** fill = quads;
        for (void* p = PtrList_Next(F<PtrList*>(g, 0x1c)); p; p = PtrList_Next(F<PtrList*>(g, 0x1c))) *fill++ = (float*)p;
        const double quarter = *(const float*)0x5c9704;
        for (int32_t i = 0; i < n; ++i) {
            for (int32_t j = i + 1; j < n; ++j) {
                float* P = quads[i];
                uint8_t pf = ((uint8_t*)P)[0x30];
                if (pf >= 3) continue;
                float* Q = quads[j];
                uint8_t qf = ((uint8_t*)Q)[0x30];
                if (qf >= 3) continue;
                bool both = false;
                if (pf == 0) both = true;
                else if (pf != 1) {
                    if (pf != 2) continue;
                    goto second;
                }
                if (qf != 2) {
                    double dx = (double)P[0] - Q[3], dz = (double)P[2] - Q[5];
                    double dist = sqrt(dz * dz + dx * dx);
                    if (dist < *(const float*)0x5cfcfc && dist > *(const float*)0x5dda44) {
                        float mx = (float)((((double)P[3] + Q[0]) + ((double)Q[3] + P[0])) * quarter);
                        float mz = (float)((((double)P[5] + Q[2]) + ((double)P[2] + Q[5])) * quarter);
                        Q[3] = mx;
                        P[0] = mx;
                        Q[5] = mz;
                        P[2] = mz;
                        float m2x = (float)((((double)P[6] + Q[9]) + ((double)P[9] + Q[6])) * quarter);
                        float m2z = (float)((((double)P[8] + Q[0xb]) + ((double)P[0xb] + Q[8])) * quarter);
                        Q[6] = m2x;
                        P[9] = m2x;
                        Q[8] = m2z;
                        P[0xb] = m2z;
                        ((uint8_t*)P)[0x30] = (uint8_t)((((uint8_t*)P)[0x30] != 0) + 2);
                        ((uint8_t*)Q)[0x30] = (uint8_t)((((uint8_t*)Q)[0x30] != 0) * 2 + 1);
                    }
                }
                if (!both) continue;
            second:
                Q = quads[j];
                if (((uint8_t*)Q)[0x30] == 1) continue;
                P = quads[i];
                {
                    double dx = (double)P[3] - Q[0], dz = (double)P[5] - Q[2];
                    double dist = sqrt(dz * dz + dx * dx);
                    if (dist < *(const float*)0x5cfcfc && dist > *(const float*)0x5dda44) {
                        float mx = (float)((((double)Q[3] + P[0]) + ((double)P[3] + Q[0])) * quarter);
                        float mz = (float)((((double)P[2] + Q[5]) + ((double)Q[2] + P[5])) * quarter);
                        Q[0] = mx;
                        P[3] = mx;
                        Q[2] = mz;
                        P[5] = mz;
                        float m2x = (float)((((double)P[9] + Q[6]) + ((double)P[6] + Q[9])) * quarter);
                        float m2z = (float)((((double)P[8] + Q[0xb]) + ((double)P[0xb] + Q[8])) * quarter);
                        Q[9] = m2x;
                        P[6] = m2x;
                        Q[0xb] = m2z;
                        P[8] = m2z;
                        ((uint8_t*)P)[0x30] = (uint8_t)((((uint8_t*)P)[0x30] != 0) * 2 + 1);
                        ((uint8_t*)Q)[0x30] = (uint8_t)((((uint8_t*)Q)[0x30] != 0) + 2);
                    }
                }
            }
        }
        PtrList_Rewind(F<PtrList*>(g, 0x1c));
        PtrList_Clear(F<PtrList*>(g, 0x1c));
        for (int32_t i = 0; i < n; ++i) {
            float* P = quads[i];
            if (!((uint8_t*)P)[0x30]) {
                crt::free(P);
                continue;
            }
            double cx = (((double)P[6] + P[3]) + P[0] + P[9]) * quarter;
            float cz = (float)((((double)P[8] + P[5]) + P[2] + P[0xb]) * quarter);
            for (int k = 0; k < 4; ++k) {
                double t = (double)P[k * 3] - cx;
                P[k * 3] = (float)(t + t + cx);
            }
            for (int k = 0; k < 4; ++k) {
                double t = (double)P[k * 3 + 2] - cz;
                P[k * 3 + 2] = (float)(t + t + cz);
            }
            PtrList_Append(F<PtrList*>(g, 0x1c), 0, P);
            PtrList_Next(F<PtrList*>(g, 0x1c));
        }
        if (quads) crt::free(quads);
    }
    QuadGrid_Weld(F<uint8_t*>(g, 0x24), 0, F<int32_t>(g, 0) - 2, F<int32_t>(g, 4) - 2, F<int32_t>(g, 0) + 0x12, F<int32_t>(g, 4) + 0x12);
}
RT3_REPLACE(0x567da0, Water_BuildQuads);

}  // namespace rt3
