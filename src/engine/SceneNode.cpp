// SceneNode.cpp - RT3.exe 0x52c000-0x530000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <initializer_list>
#include <stdio.h>
#include <windows.h>
#include "hook/hook.h"
#include "core/globals.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include "game/collection.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void* __fastcall Engine_Get1C(void* e);
void __fastcall SceneObj_RotateAxis(uint8_t* o, int, float ax, float ay, float az, float angle);
PtrListNode* __fastcall Terrain_AddGlobal(uint8_t* r, int, void* v);
void __fastcall Camera_UpdateTarget(uint8_t* c);
uint32_t __fastcall Camera_Init(uint8_t* c);
void __fastcall Camera_KeepAboveGround(uint8_t* c);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Camera_Setup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name);
int32_t SceneObj_Reset();
void __fastcall ListPair_Reset(uint8_t* o);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
float __fastcall Camera_GroundAround(uint8_t* c, int, float x, float z);
float __fastcall Render_GetLayerValue(uint8_t* r, int, int32_t i);
void __fastcall Camera_Angles(uint8_t* c, int, float* yaw, float* pitch, float* dist);
PtrList* __fastcall PtrList_Init(PtrList* l);
void __fastcall Obj_Set8u(uint8_t* o, int, uint32_t v);
void __fastcall Obj_MoveChannels(uint8_t* o);
struct PtrListNode;
void __fastcall Terrain_Remove1671(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
PtrListNode* __fastcall Terrain_Add1671(uint8_t* r, int, void* v, int32_t x, int32_t y);

int32_t __stdcall Ray_HitsTriangle(float, float, float, float, float, float, float, float, float, float, float, float,
                                   float, float, float);

void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall PtrList_Destroy(PtrList* l);
void __fastcall Camera_SetMode(uint8_t* c, int, int32_t mode);
void __fastcall Terrain_RemoveGlobal(uint8_t* r, int, PtrListNode* n);
void __fastcall Scene_MarkDirty(uint8_t* s);
void __fastcall SpriteSet_Free(uint8_t* o);
void __fastcall Channel_Detach(uint8_t* c);
void __fastcall Scene_MarkDirty(uint8_t* r);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_SetDetail(uint8_t* s, int, int32_t v);
void __fastcall Obj_SetByte64_52ce60(uint8_t* o);
uint8_t* __fastcall Model_NextFrame(uint8_t* o);

// Functions of this file, in exe order.
void __fastcall Obj_GetVec1c(uint8_t* o, int, uint32_t* out);
void __fastcall Obj_GetVec4(uint8_t* o, int, uint32_t* out);
RT3_NOINLINE void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Obj_GetXYZ4(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall Obj_GetXYZ2c(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
RT3_NOINLINE void __fastcall Obj_SetXYZ2c(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Camera_SetLock64(uint8_t* c, int, int32_t v);
void __fastcall Camera_SetLock65(uint8_t* c, int, int32_t v);
RT3_NOINLINE void __fastcall Obj_SetByte64_52ce60(uint8_t* o);
int32_t __fastcall Camera_TakeChanged(uint8_t* c);
void __fastcall Obj_GetXY10(uint8_t* o, int, uint32_t* a, uint32_t* b);
RT3_NOINLINE double __fastcall SceneObj_GroundHeight(uint8_t* o);
float __fastcall Camera_GroundAround(uint8_t* c, int, float x, float z);
float __fastcall Camera_TargetGround(uint8_t* c);
RT3_NOINLINE int32_t __fastcall Camera_InBounds(uint8_t* c, int, float x, float y);
RT3_NOINLINE void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e);
RT3_NOINLINE void __fastcall Camera_SetMode(uint8_t* c, int, int32_t mode);
RT3_NOINLINE void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
RT3_NOINLINE void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z);
void __fastcall Camera_Angles(uint8_t* c, int, float* yaw, float* pitch, float* dist);
RT3_NOINLINE void __fastcall Camera_Save(uint8_t* c, int, Stream s);
int32_t __fastcall Obj_Get28_52d370(uint8_t* o);
void __fastcall Nop52d380(void*);
void Scene_SetDetail1();
double __fastcall Obj_FractionLeft(uint8_t* o);
RT3_NOINLINE void __fastcall Camera_Detach(void* c);
RT3_NOINLINE void __fastcall Camera_UpdateEye(uint8_t* c);
RT3_NOINLINE void __fastcall Camera_UpdateTarget(uint8_t* c);
void __fastcall Camera_BoundsPixels(uint8_t* c, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
void __fastcall Camera_Destruct(uint32_t* c);
void __fastcall Nop52d590(void*);
uint32_t __fastcall Camera_Init(uint8_t* c);
RT3_NOINLINE void __fastcall Camera_KeepAboveGround(uint8_t* c);
RT3_NOINLINE uint8_t* __fastcall Camera_ConstructBase(uint8_t* c);
RT3_NOINLINE void __fastcall Camera_Setup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
RT3_NOINLINE int32_t __fastcall Camera_Load(uint8_t* c, int, Stream s);
uint8_t* __fastcall Camera_ConstructSetup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
PtrListNode* __fastcall SceneObj_AddChild(uint8_t* o, int, uint8_t* child);
PtrListNode* __fastcall Camera_AddListener(uint8_t* c, int, void* item);
RT3_NOINLINE PtrListNode* __fastcall SceneObj_AddAttachment(uint8_t* o, int, uint8_t* item);
void __fastcall Obj_AttachChannel(uint8_t* o, int, uint8_t* ch);
void __fastcall SceneObj_Register(uint8_t* o);
int32_t __fastcall Model_SetFrame(uint8_t* m, int, int32_t f);
void __fastcall Nop52daf0(void*);
void __fastcall Nop52db00(void*, int, int32_t);
void __fastcall Model_PrepareTint(uint8_t* m, int, int32_t tint);
void __fastcall Model_AddTintRect(uint8_t* m, int, float x0, float y0, float x1, float y1, float tint);
void __fastcall SceneObj_EmitLit(uint8_t* o, int, uint32_t* out, int32_t* count);
void __fastcall SceneObj_EmitFlat(uint8_t* o, int, uint32_t* out, int32_t* count);
void __fastcall Model_EmitTriangles(uint8_t* m, int, uint8_t* out, int32_t* count, uint32_t colour, int32_t frame, float dx, float dy, float dz);
void __fastcall Model_EmitVertices(uint8_t* m, int, uint8_t* out, int32_t* count);
void __fastcall Mesh_EmitTriangles(uint8_t* o, int, uint8_t* verts, int32_t* count, uint32_t color, int32_t lod, float x, float y, float z);
void __fastcall Nop52e670(void*, int, int32_t);
RT3_NOINLINE void __fastcall SceneObj_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b, int32_t deep);
uint32_t __fastcall Obj_Get1c(uint8_t* o);
RT3_NOINLINE void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Obj_Get1e2(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
RT3_NOINLINE void __fastcall SceneObj_GetPos2(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
RT3_NOINLINE int32_t __fastcall SceneObj_Transform(uint8_t* o, int, float* x, float* y, float* z);
RT3_NOINLINE void __fastcall SceneObj_SetPos(uint8_t* o, int, uint32_t x, uint32_t y, uint32_t z);
RT3_NOINLINE void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
RT3_NOINLINE void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall SceneObj_Rotate(uint8_t* o, int, float da, float db, float dc);
void __fastcall SceneObj_ResetQuat(uint8_t* o);
RT3_NOINLINE void __fastcall SceneObj_ApplyQuat(uint8_t* o);
RT3_NOINLINE void __fastcall SceneObj_RotateAxis(uint8_t* o, int, float ax, float ay, float az, float angle);
RT3_NOINLINE void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);
void __fastcall SceneObj_SetUvTransform(uint8_t* o, int, float u, float v, float su, float sv);
uint8_t __fastcall SceneObj_GetUvTransform(uint8_t* o, int, uint32_t* u, uint32_t* v, uint32_t* su, uint32_t* sv);
int32_t __fastcall Return46_52eb60(void*);
int32_t __fastcall Return46_52eb70(void*);
float __fastcall Const5dd1e8_52eb80(void*);
RT3_NOINLINE void __fastcall SceneObj_ResetAttachmentLists(uint8_t* o);
void __fastcall SceneObj_LoadFlags(uint8_t* o, int, Stream s);
void __fastcall SceneObj_SaveFlags(uint8_t* o, int, Stream s);
RT3_NOINLINE uint8_t* __fastcall Model_NextFrame(uint8_t* o);
int32_t __fastcall SceneObj_Reset_Impl(uint8_t* o);
int32_t SceneObj_Reset();
RT3_NOINLINE void __fastcall SceneObj_Init(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
void __fastcall ModelSet_Clear(uint8_t* o);
void __fastcall SpriteSet_Free(uint8_t* o);
RT3_NOINLINE void __fastcall SceneObj_Destruct(uint8_t* o);
void __fastcall SceneObj_UpdateMatrix(uint8_t* o);
int32_t __stdcall Ray_HitsTriangle_Impl(float a0, float a1, float a2, float b0, float b1, float b2, float c0, float c1, float c2, float o0, float o1, float o2, float d0, float d1, float d2);
int32_t __stdcall Ray_HitsTriangle(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
int32_t __fastcall SceneObj_SelectLod(uint8_t* o, int, float x, float y, float z);
int32_t __fastcall Mesh_EmitPickTriangles(uint8_t* o, int, uint8_t* out, int32_t* count, const float* dir, const float* origin);
void __fastcall Mesh_FaceCamera(uint8_t* o, int, uint8_t* cam, int32_t key, int32_t idx);
RT3_NOINLINE void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alphaArg, int32_t deep);
void __fastcall Obj_MoveChannels(uint8_t* o);
RT3_NOINLINE bool __fastcall Obj_MoveBy(uint8_t* o, int, float dx, float dy, float dz);
RT3_NOINLINE void __fastcall SceneObj_ScaleRotation(uint8_t* o, int, float f);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
void __fastcall Sprite_ReleaseImage(uint8_t* s);
void __fastcall Channel_MoveTo(uint8_t* c, int, float x, float y, float z);
void __fastcall Model_Release(uint8_t* m);

static inline void Get3(uint8_t* o, uint32_t off, uint32_t* a, uint32_t* b, uint32_t* c) {
    *a = F<uint32_t>(o, off);
    *b = F<uint32_t>(o, off + 4);
    *c = F<uint32_t>(o, off + 8);
}

static inline double Fsqrt(double v) {
    double r;
    __asm {
        fld v
        fsqrt
        fstp r
    }
    return r;
}

static inline double Fpatan(double y, double x) {  // x87 FPATAN: atan2(y, x)
    double r;
    __asm {
        fld y
        fld x
        fpatan
        fstp r
    }
    return r;
}

static inline double Fsin(double v) { double r; __asm { fld v } __asm { fsin } __asm { fstp r } return r; }

static inline double Fcos(double v) { double r; __asm { fld v } __asm { fcos } __asm { fstp r } return r; }

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// The mesh being drawn and its cursors (0xcc3b6c model, 0xcc3b58 mesh,
// 0xcc3b64 triangles, 0xcc3b5c / 0xcc3b60 triangle / vertex counts left).
static uint8_t* SceneObj_BeginMesh(uint8_t* o) {
    uint8_t* model = F<uint8_t*>(o, 0x14) + F<int32_t>(o, 0x3a) * 0x18;
    g_cc3b6c = model;
    uint8_t* mesh = F<uint8_t*>(model, 0xc) + F<int32_t>(o, 0x21) * 0x45;
    g_cc3b58 = mesh;
    hook::As<uint32_t>(g_cc3b64) = F<uint32_t>(mesh, 4);
    hook::As<uint32_t>(g_cc3b5c) = F<uint32_t>(mesh, 0xc);
    hook::As<uint32_t>(g_cc3b60) = F<uint32_t>(mesh, 8);
    return mesh;
}

// Lays mesh vertices on the terrain (see SceneObj_EmitFlat); onceAtLeast runs
// each loop body before testing the count, as 0x52e060 does.
static void SceneObj_LayVertices(uint8_t* o, bool onceAtLeast) {
    uint8_t* terrain = (uint8_t*)Engine_Get1C(hook::As<void*>(g_engine));
    uint8_t* mesh = g_cc3b58;
    const float* src = F<const float*>(mesh, 0x35);
    float* dst = F<float*>(mesh, 0);
    const uint8_t* group = (const uint8_t*)(src + hook::As<uint32_t>(g_cc3b60) * 3);
    uint32_t& left = hook::As<uint32_t>(g_cc3b60);
    if (F<float>(o, 0x69) > *(const float*)rt3::Rdata(0x5c8598)) {
        if (onceAtLeast || left) do {
            --left;
            float x = src[0], y = src[1], z = src[2];
            float wx = (float)(((double)x * F<float>(o, 0xce) + (double)z * F<float>(o, 0xee)) + (double)y * F<float>(o, 0xde) +
                               F<float>(o, 0xfe));
            float wy = (float)(((double)z * F<float>(o, 0xf2) + (double)y * F<float>(o, 0xe2)) + (double)x * F<float>(o, 0xd2) +
                               F<float>(o, 0x102));
            float wz = (float)(((double)x * F<float>(o, 0xd6) + (double)z * F<float>(o, 0xf6)) + (double)y * F<float>(o, 0xe6) +
                               F<float>(o, 0x106));
            dst[2] = z;
            dst[0] = x;
            dst[1] = y;
            double h = Terrain_HeightAt(terrain, 0, wx, wz) + F<float>(o, 0x69 + *group * 4);
            if (wy < h) dst[1] = (float)((h - wy) + dst[1]);
            ++group;
            src += 3;
            dst += 3;
        } while (left);
    } else {
        float off = F<float>(o, 0x69);
        if (onceAtLeast || left) do {
            --left;
            float x = src[0], y = src[1], z = src[2];
            float wx = (float)(((double)x * F<float>(o, 0xce) + (double)z * F<float>(o, 0xee)) + (double)y * F<float>(o, 0xde) +
                               F<float>(o, 0xfe));
            dst[0] = wx;
            float wz = (float)(((double)x * F<float>(o, 0xd6) + (double)z * F<float>(o, 0xf6)) + (double)y * F<float>(o, 0xe6) +
                               F<float>(o, 0x106));
            dst[2] = wz;
            double h = Terrain_HeightAt(terrain, 0, wx, wz);
            src += 3;
            dst += 3;
            dst[-2] = (float)(h - off);
        } while (left);
    }
}

// The texture name from the main pack's cache (all mips, flag 0x7f).
static uint8_t* Texture_Acquire(const char* name) { return TextureCache_Get(g_pack, 0, name, 0, 0, 0x7f); }

static inline double D(uint32_t a) { return *(const double*)rt3::Rdata(a); }

static inline float Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline int32_t Cell16(float v) {
    double d = (double)v * *(const float*)rt3::Rdata(0x5dcfe8);
    int32_t r;
    __asm {
        fld d
        call Rt3_Ftol
        mov r, eax
    }
    return r >> 4;
}

static inline double Fl_205(uint32_t a) { return *(const float*)rt3::Rdata(a); }


static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline double Fl_207(uint32_t a) { return *(const float*)rt3::Rdata(a); }

void __fastcall Obj_GetVec1c(uint8_t* o, int, uint32_t* out) {
    out[0] = F<uint32_t>(o, 0x1c);
    out[1] = F<uint32_t>(o, 0x20);
    out[2] = F<uint32_t>(o, 0x24);
}
RT3_REPLACE(0x52cd20, Obj_GetVec1c);

void __fastcall Obj_GetVec4(uint8_t* o, int, uint32_t* out) {
    out[0] = F<uint32_t>(o, 4);
    out[1] = F<uint32_t>(o, 8);
    out[2] = F<uint32_t>(o, 0xc);
}
RT3_REPLACE(0x52cd40, Obj_GetVec4);

RT3_NOINLINE void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    *x = F<uint32_t>(o, 0x1c);
    *y = F<uint32_t>(o, 0x20);
    *z = F<uint32_t>(o, 0x24);
}
RT3_REPLACE(0x52cd60, Obj_GetXYZ1c);

void __fastcall Obj_GetXYZ4(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) { Get3(o, 4, a, b, c); }
RT3_REPLACE(0x52cd80, Obj_GetXYZ4);

void __fastcall Obj_GetXYZ2c(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) { Get3(o, 0x2c, a, b, c); }
RT3_REPLACE(0x52cda0, Obj_GetXYZ2c);

RT3_NOINLINE void __fastcall Obj_SetXYZ2c(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c) {
    F<uint32_t>(o, 0x2c) = a;
    F<uint32_t>(o, 0x30) = b;
    F<uint32_t>(o, 0x34) = c;
}
RT3_REPLACE(0x52cdc0, Obj_SetXYZ2c);

// 0x52cde0 / 0x52ce20. Sets a lock flag (+0x64 / +0x65); unlocking snapshots the current view state.
void __fastcall Camera_SetLock64(uint8_t* c, int, int32_t v) {
    c[0x64] = (uint8_t)v;
    if (v) return;
    F<uint32_t>(c, 0x44) = F<uint32_t>(c, 0x10);
    F<uint32_t>(c, 0x48) = F<uint32_t>(c, 0x14);
    F<uint32_t>(c, 0x38) = F<uint32_t>(c, 4);
    F<uint32_t>(c, 0x3c) = F<uint32_t>(c, 8);
    F<uint32_t>(c, 0x40) = F<uint32_t>(c, 0xc);
    F<uint32_t>(c, 0x4c) = F<uint32_t>(c, 0x18);
}
RT3_REPLACE(0x52cde0, Camera_SetLock64);

void __fastcall Camera_SetLock65(uint8_t* c, int, int32_t v) {
    c[0x65] = (uint8_t)v;
    if (v) return;
    F<uint32_t>(c, 0x72) = F<uint32_t>(c, 0x10);
    F<uint32_t>(c, 0x76) = F<uint32_t>(c, 0x14);
    F<uint32_t>(c, 0x7a) = F<uint32_t>(c, 4);
    F<uint32_t>(c, 0x7e) = F<uint32_t>(c, 8);
    F<uint32_t>(c, 0x82) = F<uint32_t>(c, 0xc);
    F<uint32_t>(c, 0x86) = F<uint32_t>(c, 0x18);
}
RT3_REPLACE(0x52ce20, Camera_SetLock65);

// 0x52ce60 (vtable stub).
RT3_NOINLINE void __fastcall Obj_SetByte64_52ce60(uint8_t* o) { o[0x64] = 0x1; }
RT3_REPLACE(0x52ce60, Obj_SetByte64_52ce60);

// 0x52ce70. Whether the camera changed (or the engine frame +0x114222 moved on); clears the flag.
int32_t __fastcall Camera_TakeChanged(uint8_t* c) {
    int32_t frame = F<int32_t>(g_engine, 0x114222);
    if (frame != F<int32_t>(c, 0x6a)) {
        c[0x64] = 1;
        F<int32_t>(c, 0x6a) = frame;
    }
    int32_t r = c[0x64];
    c[0x64] = 0;
    return r;
}
RT3_REPLACE(0x52ce70, Camera_TakeChanged);

void __fastcall Obj_GetXY10(uint8_t* o, int, uint32_t* a, uint32_t* b) {
    *a = F<uint32_t>(o, 0x10);
    *b = F<uint32_t>(o, 0x14);
}
RT3_REPLACE(0x52cea0, Obj_GetXY10);

// 0x52cec0. Terrain height under the object's position (+0x1c, +0x24), snapped
// through tile units (x87 products as in the original).
RT3_NOINLINE double __fastcall SceneObj_GroundHeight(uint8_t* o) {
    float x, z;
    __asm {
        mov ecx, o
        fld dword ptr [ecx + 0x1c]
        fmul dword ptr ds:[0x5dcfe8]
        fld dword ptr [ecx + 0x24]
        fmul dword ptr ds:[0x5dcfe8]
        fmul dword ptr ds:[0x5c8700]
        fstp z
        fmul dword ptr ds:[0x5c8700]
        fstp x
    }
    return Terrain_HeightAt(F<uint8_t*>(o, 0x6e), 0, x, z);
}
RT3_REPLACE(0x52cec0, SceneObj_GroundHeight);

// 0x52cf00. Ground height for the camera around cell (x, z) (clamped to 2..size-3):
// the height there, raised toward any higher point within 2.01 cells (averaging).
RT3_NOINLINE float __fastcall Camera_GroundAround(uint8_t* c, int, float x, float z) {
    if (Fl(0x5c878c) > x) x = 2.0f;
    if (Fl(0x5c878c) > z) z = 2.0f;
    uint8_t* r = F<uint8_t*>(c, 0x6e);
    if ((float)(Terrain_Rows(r) - 3) < x) x = (float)(Terrain_Rows(F<uint8_t*>(c, 0x6e)) - 3);
    if ((float)(Terrain_Cols(F<uint8_t*>(c, 0x6e)) - 3) < z) z = (float)(Terrain_Cols(F<uint8_t*>(c, 0x6e)) - 3);
    const float k = Fl(0x5c8700);
    const double span = D(0x5dd1b8);
    float best = (float)Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, (float)((double)x * k), (float)((double)z * k));
    float zz = (float)((double)z - span);
    double zEnd = (double)z + span;
    if (!(zz < zEnd)) return best;
    float xs = (float)((double)x - span);
    double xEnd = (double)x + span;
    double t;
    do {
        float xx = xs;
        if (xx < xEnd) {
            float zf = (float)((double)zz * k);
            do {
                double hgt = Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, (float)((double)xx * k), zf);
                if (hgt > best) best = (float)((hgt + best) * D(0x5c8628));
                t = (double)xx + D(0x5c8770);
                xx = (float)t;
            } while (t < xEnd);
        }
        t = (double)zz + D(0x5c8770);
        zz = (float)t;
    } while (t < zEnd);
    return best;
}
RT3_REPLACE(0x52cf00, Camera_GroundAround);

// 0x52d0c0. Ground height around the camera target (+0x1c, +0x24 in cells).
float __fastcall Camera_TargetGround(uint8_t* c) {
    const float k = Fl(0x5dcfe8);
    return Camera_GroundAround(c, 0, (float)((double)F<float>(c, 0x1c) * k), (float)((double)F<float>(c, 0x24) * k));
}
RT3_REPLACE(0x52d0c0, Camera_TargetGround);

// 0x52d0f0. Point (x, y) inside the bounds +0x50..+0x5c (inclusive).
RT3_NOINLINE int32_t __fastcall Camera_InBounds(uint8_t* c, int, float x, float y) {
    if (!(x >= F<float>(c, 0x50)) || !(y >= F<float>(c, 0x54))) return 0;
    if (!(x <= F<float>(c, 0x58)) || !(y <= F<float>(c, 0x5c))) return 0;
    return 1;
}
RT3_REPLACE(0x52d0f0, Camera_InBounds);

RT3_NOINLINE void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e) {
    F<uint32_t>(c, 0x50) = a;
    F<uint32_t>(c, 0x54) = b;
    F<uint32_t>(c, 0x58) = d;
    F<uint32_t>(c, 0x5c) = e;
}
RT3_REPLACE(0x52d140, Camera_SetBounds);

// 0x52d160. Changes the mode +0x60 (after virtual slot 4) when different.
RT3_NOINLINE void __fastcall Camera_SetMode(uint8_t* c, int, int32_t mode) {
    if (F<int32_t>(c, 0x60) == mode) return;
    ((void(__thiscall*)(void*))(*(void***)c)[0x10 / 4])(c);
    F<int32_t>(c, 0x60) = mode;
}
RT3_REPLACE(0x52d160, Camera_SetMode);

// 0x52d180. Sets the eye position (+4..+0xc), flagging a change when it moved
// more than 1472 units from the snapshot (+0x38..+0x40) on any axis.
RT3_NOINLINE void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z) {
    const double lim = *(const double*)rt3::Rdata(0x5dd1c0);
    auto moved = [&](uint32_t snap, uint32_t cur) {
        double d = (double)F<float>(c, snap) - F<float>(c, cur);
        return (d < 0 ? -d : d) > lim;
    };
    if (moved(0x38, 4) || moved(0x3c, 8) || moved(0x40, 0xc)) c[0x64] = 1;
    F<uint32_t>(c, 4) = x;
    F<uint32_t>(c, 8) = y;
    F<uint32_t>(c, 0xc) = z;
}
RT3_REPLACE(0x52d180, Camera_SetEye);

// 0x52d1e0. Sets the camera target and its height above the terrain (+0x66).
RT3_NOINLINE void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z) {
    F<float>(c, 0x20) = y;
    F<float>(c, 0x1c) = x;
    F<float>(c, 0x24) = z;
    if (!F<uint8_t*>(c, 0x6e)) {
        F<float>(c, 0x66) = y;
        return;
    }
    double h = Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, x, z);
    F<float>(c, 0x66) = (float)((double)F<float>(c, 0x20) - h);
}
RT3_REPLACE(0x52d1e0, Camera_SetTarget);

// 0x52d230. Yaw, pitch and distance from the eye (+4) to the target (+0x1c); outputs may be null.
RT3_NOINLINE void __fastcall Camera_Angles(uint8_t* c, int, float* yaw, float* pitch, float* dist) {
    double dx = (double)F<float>(c, 0x1c) - F<float>(c, 4);
    double dy = (double)F<float>(c, 0x20) - F<float>(c, 8);
    double dz = (double)F<float>(c, 0x24) - F<float>(c, 0xc);
    const double q = *(const double*)rt3::Rdata(0x5c8748);
    if (dist) {
        double s = dz * dz + dy * dy;
        *dist = (float)Fsqrt(s + dx * dx);
    }
    if (yaw) *yaw = (float)(q - Fpatan(dz, dx));
    if (pitch) {
        double h = dz * dz + dx * dx;
        *pitch = (float)(q - Fpatan(Fsqrt(h), dy));
    }
}
RT3_REPLACE(0x52d230, Camera_Angles);

// 0x52d2b0. Camera save: eye, the +0x18/+0x10/+0x14 triple, target, 20 zero bytes.
RT3_NOINLINE void __fastcall Camera_Save(uint8_t* c, int, Stream s) {
    static const uint32_t kFields[] = { 4, 8, 0xc, 0x18, 0x10, 0x14, 0x1c, 0x20, 0x24 };
    for (uint32_t off : kFields) StreamWrite(s, c + off, 4, 0);
    uint8_t pad[0x14] = {};
    StreamWrite(s, pad, 0x14, 0);
}
RT3_REPLACE(0x52d2b0, Camera_Save);

// 0x52d370 (vtable stub).
int32_t __fastcall Obj_Get28_52d370(uint8_t* o) { return F<int32_t>(o, 0x28); }
RT3_REPLACE(0x52d370, Obj_Get28_52d370);

// 0x52d380 (vtable stub).
void __fastcall Nop52d380(void*) { __asm nop }
RT3_REPLACE(0x52d380, Nop52d380);

// 0x52d390 (callback). Sets the renderer's detail to 1.
void Scene_SetDetail1() { Scene_SetDetail((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0, 1); }
RT3_REPLACE(0x52d390, Scene_SetDetail1);

// 0x52d3b0 (vtable). 0x5c8578 - +0x20 / +0x28 x 0x5ca048, at least 0 (the
// unrounded x87 value).
double __fastcall Obj_FractionLeft(uint8_t* o) {
    const double v = D(0x5c8578) - (double)F<float>(o, 0x20) / (double)F<int32_t>(o, 0x28) * D(0x5ca048);
    return v < Fl_207(0x5c8598) ? Fl_207(0x5c8598) : v;
}
RT3_REPLACE(0x52d3b0, Obj_FractionLeft);

// 0x52d3e0 / 0x52d570. Detach from the engine's active camera slot (+0x28); the second is the destructor.
RT3_NOINLINE void __fastcall Camera_Detach(void* c) {
    uint8_t* e = g_engine;
    if (e && F<void*>(e, 0x28) == c) F<void*>(e, 0x28) = nullptr;
}
RT3_REPLACE(0x52d3e0, Camera_Detach);

// 0x52d400. Places the eye at distance +0x18 from the target (+0x1c) along yaw +0x10 / pitch +0x14.
RT3_NOINLINE void __fastcall Camera_UpdateEye(uint8_t* c) {
    double cp = Fcos(F<float>(c, 0x14));
    float r = F<float>(c, 0x18);
    float z = (float)((double)F<float>(c, 0x24) - Fcos(F<float>(c, 0x10)) * r * cp);
    float y = (float)((double)F<float>(c, 0x20) - Fsin(F<float>(c, 0x14)) * r);
    float x = (float)((double)F<float>(c, 0x1c) - Fsin(F<float>(c, 0x10)) * r * cp);
    Camera_SetEye(c, 0, Bits(x), Bits(y), Bits(z));
}
RT3_REPLACE(0x52d400, Camera_UpdateEye);

// 0x52d450. Places the target at distance +0x18 from the eye (+4..) along yaw +0x10 /
// pitch +0x14 and updates its height above the terrain.
RT3_NOINLINE void __fastcall Camera_UpdateTarget(uint8_t* c) {
    double cp = Fcos(F<float>(c, 0x14));
    float x = (float)(Fsin(F<float>(c, 0x10)) * cp * F<float>(c, 0x18) + F<float>(c, 0x4));
    F<float>(c, 0x1c) = x;
    float y = (float)(Fsin(F<float>(c, 0x14)) * F<float>(c, 0x18) + F<float>(c, 0x8));
    F<float>(c, 0x20) = y;
    float z = (float)(Fcos(F<float>(c, 0x10)) * cp * F<float>(c, 0x18) + F<float>(c, 0xc));
    F<float>(c, 0x24) = z;
    if (!F<uint8_t*>(c, 0x6e)) {
        F<float>(c, 0x66) = y;
        return;
    }
    double h = Terrain_HeightAt(F<uint8_t*>(c, 0x6e), 0, x, z);
    F<float>(c, 0x66) = (float)((double)F<float>(c, 0x20) - h);
}
RT3_REPLACE(0x52d450, Camera_UpdateTarget);

// 0x52d4d0. The camera bounds (+0x50..+0x5c, fractions) in pixels of the current display mode.
void __fastcall Camera_BoundsPixels(uint8_t* c, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1) {
    auto mode = []() {
        uint8_t* e = g_engine;
        return F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
    };
    *x0 = (int32_t)((double)F<int32_t>(mode(), 0) * F<float>(c, 0x50));
    *y0 = (int32_t)((double)F<int32_t>(mode(), 4) * F<float>(c, 0x54));
    *x1 = (int32_t)((double)F<int32_t>(mode(), 0) * F<float>(c, 0x58));
    *y1 = (int32_t)((double)F<int32_t>(mode(), 4) * F<float>(c, 0x5c));
}
RT3_REPLACE(0x52d4d0, Camera_BoundsPixels);

void __fastcall Camera_Destruct(uint32_t* c) {
    c[0] = 0x5dd1c8;
    Camera_Detach(c);
}
RT3_REPLACE(0x52d570, Camera_Destruct);

// 0x52d590 (vtable stub).
void __fastcall Nop52d590(void*) { __asm nop }
RT3_REPLACE(0x52d590, Nop52d590);

// 0x52d5a0. Camera defaults: pitch 0.2, distance 100, scale 1.
RT3_NOINLINE uint32_t __fastcall Camera_Init(uint8_t* c) {
    F<int32_t>(c, 0x6e) = 0;
    F<int32_t>(c, 0x66) = 0;
    F<int32_t>(c, 0x10) = 0;
    F<uint32_t>(c, 0x14) = 0x3e4ccccd;
    F<int32_t>(c, 0x44) = 0;
    F<int32_t>(c, 0x48) = 0;
    F<int32_t>(c, 0x60) = 0;
    Camera_UpdateTarget(c);
    uint32_t yaw = F<uint32_t>(c, 0x10), pitch = F<uint32_t>(c, 0x14);
    F<uint32_t>(c, 0x58) = 0x3f800000;
    F<uint32_t>(c, 0x5c) = 0x3f800000;
    F<uint32_t>(c, 0x30) = 0x3f800000;
    static const uint8_t zero[] = { 0x50, 0x54, 0x2c, 0x34, 0x40, 0x3c, 0x38, 0xc, 0x8, 0x4 };
    for (uint8_t off : zero) F<int32_t>(c, off) = 0;
    c[0x65] = 0;
    F<int32_t>(c, 0x7a) = 0;
    F<int32_t>(c, 0x7e) = 0;
    F<int32_t>(c, 0x82) = 0;
    c[0x64] = 1;
    F<int32_t>(c, 0x6a) = -1;
    F<int32_t>(c, 0x28) = 0x64;
    F<uint32_t>(c, 0x18) = 0x42c80000;
    F<uint32_t>(c, 0x4c) = 0x42c80000;
    F<uint32_t>(c, 0x72) = yaw;
    F<uint32_t>(c, 0x76) = pitch;
    F<uint32_t>(c, 0x86) = 0x42c80000;
    return 0x42c80000;
}
RT3_REPLACE(0x52d5a0, Camera_Init);

// 0x52d640. Keeps the target above the ground limit (vtbl +0xc): pulls the
// distance in by the shortfall / sin(pitch) (pitch sine at least 0.01).
RT3_NOINLINE void __fastcall Camera_KeepAboveGround(uint8_t* c) {
    Camera_UpdateTarget(c);
    auto limit = (int32_t(__thiscall*)(void*))(*(void***)c)[0xc / 4];
    if (!((double)limit(c) <= F<float>(c, 0x20))) return;
    double s = Fsin(F<float>(c, 0x14));
    if (s < *(const double*)rt3::Rdata(0x5c85b8)) s = 0.01;
    double dist = F<float>(c, 0x18);
    double g = (double)limit(c);
    F<float>(c, 0x18) = (float)(dist - ((double)F<float>(c, 0x20) - g) / s);
    Camera_UpdateTarget(c);
}
RT3_REPLACE(0x52d640, Camera_KeepAboveGround);

RT3_NOINLINE uint8_t* __fastcall Camera_ConstructBase(uint8_t* c) {
    F<uint32_t>(c, 0) = 0x5dd1c8;
    Camera_Init(c);
    return c;
}
RT3_REPLACE(0x52d6d0, Camera_ConstructBase);

// 0x52d6f0. Attaches the camera to a terrain with eye (x, y, z) and distance.
RT3_NOINLINE void __fastcall Camera_Setup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist) {
    F<uint32_t>(c, 0x4) = x;
    F<uint32_t>(c, 0x6e) = terrain;
    F<uint32_t>(c, 0x8) = y;
    F<uint32_t>(c, 0xc) = z;
    F<uint32_t>(c, 0x18) = dist;
    F<uint32_t>(c, 0x4c) = dist;
    F<uint32_t>(c, 0x6a) = F<uint32_t>(g_engine, 0x114222);
    F<int32_t>(c, 0x28) = (int32_t)F<float>(Engine_Renderer(hook::As<void*>(g_engine)), 0x22b67);
    Camera_KeepAboveGround(c);
}
RT3_REPLACE(0x52d6f0, Camera_Setup);

// 0x52d750. Loads a camera (eye, distance, angles, target; 20 bytes skipped) and
// re-attaches it to the engine's terrain. Returns the bytes read.
RT3_NOINLINE int32_t __fastcall Camera_Load(uint8_t* c, int, Stream s) {
    uint8_t* e = g_engine;
    if (e && F<uint8_t*>(e, 0x28) == c) F<uint8_t*>(e, 0x28) = nullptr;
    int32_t n = StreamRead(s, c + 4, 4, 0, 1);
    n += StreamRead(s, c + 8, 4, 0, 1);
    n += StreamRead(s, c + 0xc, 4, 0, 1);
    n += StreamRead(s, c + 0x18, 4, 0, 1);
    Camera_Setup(c, 0, (uint32_t)Engine_Get1C(hook::As<void*>(g_engine)), F<uint32_t>(c, 4), F<uint32_t>(c, 8),
                 F<uint32_t>(c, 0xc), F<uint32_t>(c, 0x18));
    static const uint8_t rest[] = { 0x10, 0x14, 0x1c, 0x20, 0x24 };
    for (uint8_t off : rest) n += StreamRead(s, c + off, 4, 0, 1);
    uint8_t skip[0x14];
    return StreamRead(s, skip, 0x14, 0, 1) + n;
}
RT3_REPLACE(0x52d750, Camera_Load);

uint8_t* __fastcall Camera_ConstructSetup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z,
                                          uint32_t dist) {
    F<uint32_t>(c, 0) = 0x5dd1c8;
    Camera_Init(c);
    Camera_Setup(c, 0, terrain, x, y, z, dist);
    return c;
}
RT3_REPLACE(0x52d860, Camera_ConstructSetup);

// 0x52d8a0. Attaches a child object: makes its position (+0x1e2..) relative
// to this object's (or to the parent transform +0x14), flags it 0x10 and
// appends it to the child list +0x75 (created on first use).
PtrListNode* __fastcall SceneObj_AddChild(uint8_t* o, int, uint8_t* child) {
    if (!F<PtrList*>(o, 0x75)) {
        PtrList* l = (PtrList*)crt::operator_new(0x18);
        F<PtrList*>(o, 0x75) = l ? PtrList_Init(l) : nullptr;
    }
    if (const float* p = F<float*>(o, 0x14)) {
        for (int k = 0; k < 3; ++k)
            F<float>(child, 0x1e2 + k * 4) = (float)((double)F<float>(child, 0x1e2 + k * 4) - F<float*>(o, 0x14)[k]);
    } else if (!(o[0x20] & 0x10)) {
        for (int k = 0; k < 3; ++k)
            F<float>(child, 0x1e2 + k * 4) = (float)((double)F<float>(child, 0x1e2 + k * 4) - F<float>(o, 0x1e2 + k * 4));
    }
    child[0x20] |= 0x10;
    return PtrList_Append(F<PtrList*>(o, 0x75), 0, child);
}
RT3_REPLACE(0x52d8a0, SceneObj_AddChild);

// 0x52d950. Appends to the listener list +0x79 (created on first use).
PtrListNode* __fastcall Camera_AddListener(uint8_t* c, int, void* item) {
    if (!F<PtrList*>(c, 0x79)) {
        PtrList* l = (PtrList*)crt::operator_new(0x18);
        F<PtrList*>(c, 0x79) = l ? PtrList_Init(l) : nullptr;
    }
    return PtrList_Append(F<PtrList*>(c, 0x79), 0, item);
}
RT3_REPLACE(0x52d950, Camera_AddListener);

// 0x52d9a0. Adds an attachment at this object's transform origin (+0x14) to the list +0x7d.
RT3_NOINLINE PtrListNode* __fastcall SceneObj_AddAttachment(uint8_t* o, int, uint8_t* item) {
    if (!F<PtrList*>(o, 0x7d)) {
        PtrList* l = (PtrList*)crt::operator_new(0x18);
        F<PtrList*>(o, 0x7d) = l ? PtrList_Init(l) : nullptr;
    }
    for (int k = 0; k < 3; ++k) F<uint32_t>(item, 0x29 + k * 4) = F<uint32_t*>(o, 0x14)[k];
    return PtrList_Append(F<PtrList*>(o, 0x7d), 0, item);
}
RT3_REPLACE(0x52d9a0, SceneObj_AddAttachment);

// 0x52da00. Attaches sound channel ch to the object (list +0x81, created on first
// use), puts it at the object's position (+0x1e2) and makes the object its owner.
void __fastcall Obj_AttachChannel(uint8_t* o, int, uint8_t* ch) {
    if (!F<PtrList*>(o, 0x81)) {
        void* p = crt::operator_new(0x18);
        F<PtrList*>(o, 0x81) = p ? PtrList_Init((PtrList*)p) : nullptr;
    }
    PtrList_Append(F<PtrList*>(o, 0x81), 0, ch);
    Channel_MoveTo(ch, 0, F<float>(o, 0x1e2), F<float>(o, 0x1e6), F<float>(o, 0x1ea));
    Obj_Set8u(ch, 0, (uint32_t)(uintptr_t)o);
}
RT3_REPLACE(0x52da00, Obj_AttachChannel);

// 0x52da70. Registers the object with its terrain (+4) once (pending flag 0x40).
void __fastcall SceneObj_Register(uint8_t* o) {
    if (!F<uint8_t*>(o, 4) || !(o[0x20] & 0x40)) return;
    o[0x20] &= 0xbf;
    F<PtrListNode*>(o, 0xc) = Terrain_AddGlobal(F<uint8_t*>(o, 4), 0, o);
}
RT3_REPLACE(0x52da70, SceneObj_Register);

// 0x52daa0. Selects animation frame f (+0x21, frame pointer +0x18); clamps to the
// last frame and returns 0 past the end.
int32_t __fastcall Model_SetFrame(uint8_t* m, int, int32_t f) {
    if (!F<int32_t>(m, 0x10)) return 0;
    uint8_t* anim = F<uint8_t*>(m, 0x14);
    int32_t n = F<int32_t>(anim, 0x14);
    if (f >= n) {
        F<int32_t>(m, 0x21) = n - 1;
        F<uint8_t*>(m, 0x18) = F<uint8_t*>(anim, 0xc) + (n - 1) * 0x45;
        return 0;
    }
    F<int32_t>(m, 0x21) = f;
    F<uint8_t*>(m, 0x18) = F<uint8_t*>(anim, 0xc) + f * 0x45;
    return 1;
}
RT3_REPLACE(0x52daa0, Model_SetFrame);

// 0x52daf0 (vtable stub).
void __fastcall Nop52daf0(void*) { __asm nop }
RT3_REPLACE(0x52daf0, Nop52daf0);

// 0x52db00 (vtable stub).
void __fastcall Nop52db00(void*, int, int32_t) { __asm nop }
RT3_REPLACE(0x52db00, Nop52db00);

// 0x52db10. Prepares the current animation (+0x3a) for colour tinting (+0x69):
// each frame gets a private vertex copy (+0x35: positions plus one byte per vertex).
void __fastcall Model_PrepareTint(uint8_t* m, int, int32_t tint) {
    F<int32_t>(m, 0x69) = tint;
    m[0x8d] = 1;
    m[0x20] |= 2;
    uint8_t* anim = F<uint8_t*>(m, 0x14) + F<int32_t>(m, 0x3a) * 24;
    g_cc3b6c = anim;
    for (int32_t i = 0; i < F<int32_t>(g_cc3b6c, 0x14); ++i) {
        uint8_t* fr = F<uint8_t*>(g_cc3b6c, 0xc) + i * 0x45;
        g_cc3b58 = fr;
        if (F<void*>(fr, 0x35)) continue;
        int32_t n = F<int32_t>(fr, 8);
        g_cc3b60 = n;
        uint8_t* buf = (uint8_t*)crt::malloc(n + n * 12);
        F<uint8_t*>(g_cc3b58, 0x35) = buf;
        const uint8_t* src = F<uint8_t*>(g_cc3b58, 0);
        for (int32_t k = 0; k < n * 12; ++k) buf[k] = src[k];
        for (int32_t k = 0; k < n; ++k) buf[n * 12 + k] = 0;
    }
}
RT3_REPLACE(0x52db10, Model_PrepareTint);

// 0x52dc00. Adds a tint layer (+0x69, up to 2) and marks every vertex of the
// current animation inside the rectangle (x0, y0)-(x1, y1) (by z / y) with it.
void __fastcall Model_AddTintRect(uint8_t* m, int, float x0, float y0, float x1, float y1, float tint) {
    if (!m[0x8d] || m[0x5b] >= 2) return;
    m[0x5b]++;
    F<float>(m, 0x69 + m[0x5b] * 4) = tint;
    uint8_t* anim = F<uint8_t*>(m, 0x14) + F<int32_t>(m, 0x3a) * 24;
    g_cc3b6c = anim;
    for (int32_t i = 0; i < F<int32_t>(anim, 0x14); ++i) {
        uint8_t* fr = F<uint8_t*>(anim, 0xc) + i * 0x45;
        g_cc3b58 = fr;
        int32_t n = F<int32_t>(fr, 8);
        g_cc3b60 = n;
        uint8_t* buf = F<uint8_t*>(fr, 0x35);
        uint8_t* marks = buf + n * 12;
        for (int32_t k = 0; g_cc3b60; ++k) {
            float vz = F<float>(buf, k * 12 + 8), vy = F<float>(buf, k * 12 + 4);
            if (vz > x0 && vz < x1 && vy > y0 && vy < y1) marks[k] = m[0x5b];
            g_cc3b60--;
        }
    }
}
RT3_REPLACE(0x52dc00, Model_AddTintRect);

// 0x52dd00. As SceneObj_EmitFlat, but with lit vertices: {x, y, z, normal (the
// triangle's +0xc + k * 0xc), u + +0x85, v + +0x89}, 8 dwords each; empty mesh
// parts are skipped.
void __fastcall SceneObj_EmitLit(uint8_t* o, int, uint32_t* out, int32_t* count) {
    SceneObj_BeginMesh(o);
    SceneObj_LayVertices(o, false);
    uint32_t& tris = hook::As<uint32_t>(g_cc3b5c);
    *count += (int32_t)(tris * 3);
    while (tris) {
        const uint32_t* t = g_cc3b64;
        for (int32_t k = 0; k < 3; ++k) {
            const uint32_t* v = (const uint32_t*)(uintptr_t)t[k];
            out[0] = v[0];
            out[1] = v[1];
            out[2] = v[2];
            out[3] = t[3 + k * 3];
            out[4] = t[4 + k * 3];
            out[5] = t[5 + k * 3];
            *(float*)&out[6] = (float)((double)*(const float*)&t[0xc + k * 2] + F<float>(o, 0x85));
            *(float*)&out[7] = (float)((double)*(const float*)&t[0xd + k * 2] + F<float>(o, 0x89));
            out += 8;
        }
        g_cc3b64 = t + 0x13;
        --tris;
    }
}
RT3_REPLACE(0x52dd00, SceneObj_EmitLit);

// 0x52e060. Lays scene object o's current mesh on the terrain and appends its
// triangles to out (three {x, y, z, tint +0x43, u + +0x85, v + +0x89} vertices
// each; *count grows by 3 per triangle). With a height offset (+0x69) above 0
// the mesh vertices keep their shape and are only lifted where the terrain
// (plus the vertex group's offset) is above their world position (matrix
// +0xce..+0x106); otherwise every vertex sits at terrain height less the offset.
// Both loops run at least once, as in the original.
void __fastcall SceneObj_EmitFlat(uint8_t* o, int, uint32_t* out, int32_t* count) {
    SceneObj_BeginMesh(o);
    SceneObj_LayVertices(o, true);
    uint32_t& tris = hook::As<uint32_t>(g_cc3b5c);
    *count += (int32_t)(tris * 3);
    do {
        --tris;
        const uint32_t* t = g_cc3b64;
        for (int32_t k = 0; k < 3; ++k) {
            const uint32_t* v = (const uint32_t*)(uintptr_t)t[k];
            out[0] = v[0];
            out[1] = v[1];
            out[2] = v[2];
            out[3] = F<uint32_t>(o, 0x43);
            t = g_cc3b64;
            *(float*)&out[4] = (float)((double)*(const float*)&t[0xc + k * 2] + F<float>(o, 0x85));
            t = g_cc3b64;
            *(float*)&out[5] = (float)((double)*(const float*)&t[0xd + k * 2] + F<float>(o, 0x89));
            t = g_cc3b64;
            out += 6;
        }
        g_cc3b64 = t + 0x13;
    } while (tris);
}
RT3_REPLACE(0x52e060, SceneObj_EmitFlat);

// 0x52e380. Emits animation 0's frame (clamped) as a triangle list of 24-byte
// vertices {xyz + offset, colour, uv} from the tinted vertex copy (+0x35) and
// adds the vertex count to *count.
void __fastcall Model_EmitTriangles(uint8_t* m, int, uint8_t* out, int32_t* count, uint32_t colour,
                                    int32_t frame, float dx, float dy, float dz) {
    uint8_t* anim = F<uint8_t*>(m, 0x14);
    int32_t last = F<int32_t>(anim, 0x14) - 1;
    if (frame > last) frame = last;
    uint8_t* fr = F<uint8_t*>(anim, 0xc) + frame * 0x45;
    uint8_t* tri = F<uint8_t*>(fr, 4);
    int32_t delta = (int32_t)(F<uint8_t*>(fr, 0x35) - F<uint8_t*>(fr, 0));
    int32_t n = F<int32_t>(fr, 0xc);
    *count += n * 3;
    do {
        const float* v0 = (const float*)(F<uint8_t*>(tri, 0) + delta);
        const float* v1 = (const float*)(F<uint8_t*>(tri, 4) + delta);
        const float* v2 = (const float*)(F<uint8_t*>(tri, 8) + delta);
        --n;
        const float* p[3] = { v0, v1, v2 };
        for (int k = 0; k < 3; ++k) {
            uint8_t* o = out + k * 0x18;
            F<float>(o, 0) = (float)((double)dx + p[k][0]);
            F<float>(o, 4) = (float)((double)dy + p[k][1]);
            F<float>(o, 8) = (float)((double)dz + p[k][2]);
            F<uint32_t>(o, 0xc) = colour;
            F<uint32_t>(o, 0x10) = F<uint32_t>(tri, 0x30 + k * 8);
            F<uint32_t>(o, 0x14) = F<uint32_t>(tri, 0x34 + k * 8);
        }
        tri += 0x4c;
        out += 0x48;
    } while (n);
}
RT3_REPLACE(0x52e380, Model_EmitTriangles);

// 0x52e480. Emits the current frame's vertices as 32-byte {xyz, 0, 1, 0, 0, 0}
// records into out and adds their count to *count.
void __fastcall Model_EmitVertices(uint8_t* m, int, uint8_t* out, int32_t* count) {
    uint8_t* anim = F<uint8_t*>(m, 0x14) + F<int32_t>(m, 0x3a) * 24;
    g_cc3b6c = anim;
    uint8_t* fr = F<uint8_t*>(anim, 0xc) + F<int32_t>(m, 0x21) * 0x45;
    g_cc3b58 = fr;
    g_cc3b68 = F<uint8_t*>(fr, 0);
    g_cc3b60 = F<int32_t>(fr, 8);
    *count += g_cc3b60;
    while (g_cc3b60) {
        const uint32_t* src = hook::As<uint32_t*>(g_cc3b68);
        F<uint32_t>(out, 0) = src[0];
        F<uint32_t>(out, 4) = src[1];
        F<uint32_t>(out, 8) = src[2];
        F<uint32_t>(out, 0x1c) = 0;
        F<uint32_t>(out, 0x18) = 0;
        F<uint32_t>(out, 0xc) = 0;
        F<uint32_t>(out, 0x10) = 0x3f800000;
        F<uint32_t>(out, 0x14) = 0;
        g_cc3b68 += 0xc;
        out += 0x20;
        g_cc3b60--;
    }
}
RT3_REPLACE(0x52e480, Model_EmitVertices);

// 0x52e520. Appends the triangles of the object's mesh (+0x14 meshes of 24 bytes,
// index +0x3a; LOD lod, clamped) to verts (x, y, z, color, u, v) offset by (x, y, z);
// *count grows by 3 per triangle. Leaves the walk state in 0xcc3b58..0xcc3b6c.
void __fastcall Mesh_EmitTriangles(uint8_t* o, int, uint8_t* verts, int32_t* count, uint32_t color, int32_t lod,
                                   float x, float y, float z) {
    uint8_t* mesh = F<uint8_t*>(o, 0x14) + F<int32_t>(o, 0x3a) * 24;
    g_cc3b6c = mesh;
    if (lod >= F<int32_t>(mesh, 0x14)) lod = F<int32_t>(mesh, 0x14) - 1;
    uint8_t* l = F<uint8_t*>(mesh, 0xc) + lod * 0x45;
    g_cc3b58 = l;
    g_cc3b5c = F<int32_t>(l, 0xc);
    hook::As<uint8_t*>(g_cc3b64) = F<uint8_t*>(l, 4);
    *count += F<int32_t>(l, 0xc) * 3;
    uint32_t* v = (uint32_t*)verts;
    do {
        uint8_t* tri = hook::As<uint8_t*>(g_cc3b64);
        const float* p0 = F<const float*>(tri, 0);
        const float* p1 = F<const float*>(tri, 4);
        const float* p2 = F<const float*>(tri, 8);
        --g_cc3b5c;
        const float* ps[3] = { p0, p1, p2 };
        for (int k = 0; k < 3; ++k) {
            float* f = (float*)(v + k * 6);
            f[0] = (float)((double)x + ps[k][0]);
            f[1] = (float)((double)y + ps[k][1]);
            f[2] = (float)((double)z + ps[k][2]);
            v[k * 6 + 3] = color;
            v[k * 6 + 4] = F<uint32_t>(hook::As<uint8_t*>(g_cc3b64), 0x30 + k * 8);
            v[k * 6 + 5] = F<uint32_t>(hook::As<uint8_t*>(g_cc3b64), 0x34 + k * 8);
        }
        v += 18;
        hook::As<uint8_t*>(g_cc3b64) += 0x4c;
    } while (g_cc3b5c);
}
RT3_REPLACE(0x52e520, Mesh_EmitTriangles);

void __fastcall Nop52e670(void*, int, int32_t) { __asm nop }
RT3_REPLACE(0x52e670, Nop52e670);

// 0x52e680. Tints scene object o (+0x43: r g b over the alpha byte +0x47; black means
// the neutral grey 0x7f7f7f), and with `deep` all its children (+0x75) too.
RT3_NOINLINE void __fastcall SceneObj_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b, int32_t deep) {
    if (!(uint8_t)r && !(uint8_t)g && !(uint8_t)b)
        F<uint32_t>(o, 0x43) = 0x7f7f7f;
    else
        F<uint32_t>(o, 0x43) = ((((uint32_t)(uint8_t)r << 8) | (uint8_t)g) << 8 | F<uint32_t>(o, 0x47)) | (uint8_t)b;
    if (!deep || !F<PtrList*>(o, 0x75)) return;
    PtrList_Rewind(F<PtrList*>(o, 0x75));
    for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)))
        SceneObj_SetTint(c, 0, r, g, b, 1);
}
RT3_REPLACE(0x52e680, SceneObj_SetTint);

uint32_t __fastcall Obj_Get1c(uint8_t* o) { return F<uint32_t>(o, 0x1c); }
RT3_REPLACE(0x52e710, Obj_Get1c);

RT3_NOINLINE void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    *x = F<uint32_t>(o, 0x1e2);
    *y = F<uint32_t>(o, 0x1e6);
    *z = F<uint32_t>(o, 0x1ea);
}
RT3_REPLACE(0x52e720, SceneObj_GetPos);

// 0x52e750 (vtable). Copies the three values +0x1e2 / +0x1e6 / +0x1ea out.
void __fastcall Obj_Get1e2(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    *a = F<uint32_t>(o, 0x1e2);
    *b = F<uint32_t>(o, 0x1e6);
    *c = F<uint32_t>(o, 0x1ea);
}
RT3_REPLACE(0x52e750, Obj_Get1e2);

RT3_NOINLINE void __fastcall SceneObj_GetPos2(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    *x = F<uint32_t>(o, 0x1e2);
    *y = F<uint32_t>(o, 0x1e6);
    *z = F<uint32_t>(o, 0x1ea);
}
RT3_REPLACE(0x52e780, SceneObj_GetPos2);

// 0x52e7b0. Transforms a point by the object's world matrix (+0x10e, rows of
// 4 floats, translation +0x13e); 0 while the matrix is dirty (flag 2).
RT3_NOINLINE int32_t __fastcall SceneObj_Transform(uint8_t* o, int, float* x, float* y, float* z) {
    if (o[0x20] & 2) return 0;
    double px = *x, py = *y, pz = *z;
    const float* m = (const float*)(o + 0x10e);
    *x = (float)(pz * m[8] + py * m[4] + px * m[0] + m[12]);
    *y = (float)(pz * m[9] + py * m[5] + px * m[1] + m[13]);
    *z = (float)(pz * m[10] + py * m[6] + px * m[2] + m[14]);
    return 1;
}
RT3_REPLACE(0x52e7b0, SceneObj_Transform);

RT3_NOINLINE void __fastcall SceneObj_SetPos(uint8_t* o, int, uint32_t x, uint32_t y, uint32_t z) {
    o[0x20] |= 2;
    F<uint32_t>(o, 0x1e2) = x;
    F<uint32_t>(o, 0x1e6) = y;
    F<uint32_t>(o, 0x1ea) = z;
}
RT3_REPLACE(0x52e850, SceneObj_SetPos);

RT3_NOINLINE void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    if (a) *a = F<uint32_t>(o, 0x4b);
    if (b) *b = F<uint32_t>(o, 0x4f);
    if (c) *c = F<uint32_t>(o, 0x53);
}
RT3_REPLACE(0x52e880, SceneObj_GetAngles);

RT3_NOINLINE void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c) {
    o[0x20] = (uint8_t)((o[0x20] & 0x7d) | 2);
    F<uint32_t>(o, 0x4b) = a;
    F<uint32_t>(o, 0x4f) = b;
    F<uint32_t>(o, 0x53) = c;
}
RT3_REPLACE(0x52e8b0, SceneObj_SetAngles);

// 0x52e8e0. Rotates by (da, db, dc), wrapping each angle once past 2 pi
// (ignored for quaternion-driven objects, flag 0x80).
RT3_NOINLINE void __fastcall SceneObj_Rotate(uint8_t* o, int, float da, float db, float dc) {
    if (o[0x20] & 0x80) return;
    o[0x20] |= 2;
    double a = (double)da + F<float>(o, 0x4b);
    F<float>(o, 0x4b) = (float)a;
    float b = (float)((double)db + F<float>(o, 0x4f));
    F<float>(o, 0x4f) = b;
    float c = (float)((double)dc + F<float>(o, 0x53));
    F<float>(o, 0x53) = c;
    const float twoPi = *(const float*)rt3::Rdata(0x5c8768);
    if (a > twoPi) F<float>(o, 0x4b) = (float)(a - twoPi);
    if (b > twoPi) F<float>(o, 0x4f) = (float)((double)b - twoPi);
    if (c > twoPi) F<float>(o, 0x53) = (float)((double)c - twoPi);
}
RT3_REPLACE(0x52e8e0, SceneObj_Rotate);

void __fastcall SceneObj_ResetQuat(uint8_t* o) {
    F<int32_t>(o, 0x1d6) = 0;
    F<int32_t>(o, 0x1d2) = 0;
    F<int32_t>(o, 0x1ce) = 0;
    F<uint32_t>(o, 0x1da) = 0x3f800000;
}
RT3_REPLACE(0x52e970, SceneObj_ResetQuat);

// 0x52e990. For quaternion-driven objects: local matrix +0x18e = rotation(+0x1ce) x local (D3DX).
RT3_NOINLINE void __fastcall SceneObj_ApplyQuat(uint8_t* o) {
    if (!(o[0x20] & 0x80)) return;
    o[0x20] |= 2;
    float rot[16], res[16];
    ((void*(__stdcall*)(float*, const void*))0x56e092)(rot, o + 0x1ce);
    ((void*(__stdcall*)(float*, const float*, const void*))0x56d3cd)(res, rot, o + 0x18e);
    for (int k = 0; k < 16; ++k) F<float>(o, 0x18e + k * 4) = res[k];
}
RT3_REPLACE(0x52e990, SceneObj_ApplyQuat);

// 0x52e9f0. Rotates the object's quaternion (+0x1ce) by `angle` about the
// axis (ax, ay, az) (D3DX), switching it to quaternion mode (flags 0x82).
RT3_NOINLINE void __fastcall SceneObj_RotateAxis(uint8_t* o, int, float ax, float ay, float az, float angle) {
    double h = (double)angle * *(const double*)rt3::Rdata(0x5c8628);
    o[0x20] |= 0x82;
    double s = Fsin(h);
    float q[4] = { (float)((double)ax * s), (float)((double)ay * s), (float)((double)az * s), (float)Fcos(h) };
    auto normalize = (void*(__stdcall*)(void*, const void*))0x56e979;
    normalize(q, q);
    float r[4];
    ((void*(__stdcall*)(void*, const void*, const void*))0x56e8d0)(r, q, o + 0x1ce);
    for (int k = 0; k < 4; ++k) F<float>(o, 0x1ce + k * 4) = r[k];
    normalize(o + 0x1ce, o + 0x1ce);
}
RT3_REPLACE(0x52e9f0, SceneObj_RotateAxis);

RT3_NOINLINE void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v) {
    if (v == F<float>(o, 0x36)) return;
    o[0x20] |= 2;
    F<float>(o, 0x36) = v;
}
RT3_REPLACE(0x52ea90, SceneObj_SetScale36);

// 0x52eab0 / 0x52eb20. Texture transform {u, v, su, sv} (+0x26..); +0x25
// tells whether it differs from the identity.
void __fastcall SceneObj_SetUvTransform(uint8_t* o, int, float u, float v, float su, float sv) {
    F<float>(o, 0x26) = u;
    F<float>(o, 0x2e) = su;
    F<float>(o, 0x2a) = v;
    F<float>(o, 0x32) = sv;
    bool identity = (double)su == *(const double*)rt3::Rdata(0x5c8770) && (double)sv == *(const double*)rt3::Rdata(0x5c8770) &&
                    (double)u == *(const double*)rt3::Rdata(0x5c8608) && (double)v == *(const double*)rt3::Rdata(0x5c8608);
    o[0x25] = identity ? 0 : 1;
}
RT3_REPLACE(0x52eab0, SceneObj_SetUvTransform);

uint8_t __fastcall SceneObj_GetUvTransform(uint8_t* o, int, uint32_t* u, uint32_t* v, uint32_t* su, uint32_t* sv) {
    if (!o[0x25]) return 0;
    *u = F<uint32_t>(o, 0x26);
    *v = F<uint32_t>(o, 0x2a);
    *su = F<uint32_t>(o, 0x2e);
    *sv = F<uint32_t>(o, 0x32);
    return 1;
}
RT3_REPLACE(0x52eb20, SceneObj_GetUvTransform);

// 0x52eb60 (vtable stub).
int32_t __fastcall Return46_52eb60(void*) { return 0x46; }
RT3_REPLACE(0x52eb60, Return46_52eb60);

// 0x52eb70 (vtable stub).
int32_t __fastcall Return46_52eb70(void*) { return 0x46; }
RT3_REPLACE(0x52eb70, Return46_52eb70);

// 0x52eb80 (vtable stub).
float __fastcall Const5dd1e8_52eb80(void*) { return g_5dd1e8; }
RT3_REPLACE(0x52eb80, Const5dd1e8_52eb80);

RT3_NOINLINE void __fastcall SceneObj_ResetAttachmentLists(uint8_t* o) {
    if (!F<PtrList*>(o, 0x79)) return;
    PtrList_Rewind(F<PtrList*>(o, 0x79));
    for (uint8_t* e = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79)); e; e = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79)))
        ListPair_Reset(e);
}
RT3_REPLACE(0x52eb90, SceneObj_ResetAttachmentLists);

// 0x52ebd0. Loads flag bits 5 and 6 (see SceneObj_SaveFlags); an object saved
// without bit 6 is registered with its terrain right away.
void __fastcall SceneObj_LoadFlags(uint8_t* o, int, Stream s) {
    uint8_t b = 0;
    StreamRead(s, &b, 1, 0, 1);
    o[0x20] ^= (uint8_t)(((b << 5) ^ o[0x20]) & 0x20);
    StreamRead(s, &b, 1, 0, 1);
    uint8_t fl = o[0x20];
    fl ^= (uint8_t)(((b << 6) ^ fl) & 0x40);
    o[0x20] = fl;
    if (fl & 0x40) return;
    fl |= 0x40;
    o[0x20] = fl;
    if (!F<uint8_t*>(o, 4) || !(fl & 0x40)) return;
    o[0x20] = fl & 0xbf;
    F<PtrListNode*>(o, 0xc) = Terrain_AddGlobal(F<uint8_t*>(o, 4), 0, o);
}
RT3_REPLACE(0x52ebd0, SceneObj_LoadFlags);

// 0x52ec50. Saves flag bits 5 and 6 of +0x20 as two bytes.
void __fastcall SceneObj_SaveFlags(uint8_t* o, int, Stream s) {
    uint8_t b = (o[0x20] >> 5) & 1;
    StreamWrite(s, &b, 1, 0);
    b = (o[0x20] >> 6) & 1;
    StreamWrite(s, &b, 1, 0);
}
RT3_REPLACE(0x52ec50, SceneObj_SaveFlags);

RT3_NOINLINE uint8_t* __fastcall Model_NextFrame(uint8_t* o) {
    uint8_t* list = F<uint8_t*>(o, 0x14);
    if (!list) return nullptr;
    uint8_t* anim = list + 24 * F<int32_t>(o, 0x3a);
    const int32_t next = F<int32_t>(o, 0x21) + 1;
    if (next >= F<int32_t>(anim, 0x14)) return nullptr;
    return F<uint8_t*>(anim, 0xc) + next * 0x45;
}
RT3_REPLACE(0x52eca0, Model_NextFrame);

// 0x52ecd0. Resets a scene object: identity transform/quaternion, scale 1, no parts.
int32_t __fastcall SceneObj_Reset_Impl(uint8_t* o) {
    o[0x20] &= 0x77;
    o[0x8d] = 0;
    o[0x5c] = 0;
    F<int32_t>(o, 0x5d) = 0;
    F<int32_t>(o, 0x61) = 0;
    F<int32_t>(o, 0x1ee) = -1;
    F<int32_t>(o, 0x1fa) = -1;
    F<int32_t>(o, 0x3e) = -1;
    uint8_t fl = o[0x20];
    F<uint32_t>(o, 0x47) = 0xff000000;
    static const uint16_t zero[] = { 0x8, 0xc, 0x75, 0x79, 0x7d, 0x81, 0x4, 0x10, 0x1c, 0x14 };
    for (uint16_t off : zero) F<int32_t>(o, off) = 0;
    o[0x5b] = 0;
    F<int32_t>(o, 0x85) = 0;
    F<int32_t>(o, 0x89) = 0;
    o[0x25] = 0;
    o[0x42] = 0;
    o[0x20] = (uint8_t)((fl & 0xee) | 0x62);
    F<int32_t>(o, 0x4b) = 0;
    F<int32_t>(o, 0x4f) = 0;
    F<int32_t>(o, 0x53) = 0;
    for (int k = 0; k < 16; ++k) F<uint32_t>(o, 0x18e + k * 4) = (k % 5 == 0) ? 0x3f800000 : 0;
    F<int32_t>(o, 0x1d6) = 0;
    F<int32_t>(o, 0x1d2) = 0;
    F<int32_t>(o, 0x1ce) = 0;
    F<uint32_t>(o, 0x1da) = 0x3f800000;
    F<uint32_t>(o, 0x36) = 0x3f800000;
    F<int32_t>(o, 0x3a) = 0;
    F<int32_t>(o, 0x21) = 0;
    F<int32_t>(o, 0x65) = 0;
    return 0;
}

// 0x53067b keeps using ECX (the object) after the call.
__declspec(naked) int32_t SceneObj_Reset() { RT3_KEEP_ECX_EDX(SceneObj_Reset_Impl, 0) }
RT3_REPLACE(0x52ecd0, SceneObj_Reset);

// 0x52edf0. Resets scene object o for owner (+4) with collision model (+0x10,
// its first mesh +0x14 and that mesh's +0xc at +0x18, its position copied to
// +0x1e2) and texture (+0x1c); grey tint (+0x43), dirty flag 8 (+0x20).
RT3_NOINLINE void __fastcall SceneObj_Init(uint8_t* o, int, uint32_t owner, const char* model, const char* tex) {
    ((void(__fastcall*)(uint8_t*))&SceneObj_Reset)(o);
    F<uint32_t>(o, 4) = owner;
    F<uint32_t>(o, 0x75) = 0;
    if (model) {
        F<uint8_t*>(o, 0x10) = Pack_AcquireCollision(g_pack, 0, model);
        F<uint8_t*>(o, 0x1c) = tex ? Texture_Acquire(tex) : nullptr;
        uint8_t* mesh = *F<uint8_t**>(o, 0x10);
        F<uint8_t*>(o, 0x14) = mesh;
        F<uint32_t>(o, 0x18) = F<uint32_t>(mesh, 0xc);
        memcpy(o + 0x1e2, F<uint8_t*>(o, 0x14), 12);
    } else {
        F<uint32_t>(o, 0x10) = 0;
        F<uint32_t>(o, 0x14) = 0;
        F<uint32_t>(o, 0x18) = 0;
        memset(o + 0x1e2, 0, 12);
    }
    F<uint32_t>(o, 0x43) = 0x7f7f7f;
    o[0x20] |= 8;
}
RT3_REPLACE(0x52edf0, SceneObj_Init);

// 0x52eeb0. Releases and deletes every model in the list +0x79, then the list.
void __fastcall ModelSet_Clear(uint8_t* o) {
    if (!F<PtrList*>(o, 0x79)) return;
    PtrList_Rewind(F<PtrList*>(o, 0x79));
    for (uint8_t* m; (m = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79))) != nullptr;) {
        Model_Release(m);
        crt::operator_delete(m);
    }
    if (PtrList* l = F<PtrList*>(o, 0x79)) {
        PtrList_Destroy(l);
        crt::operator_delete(l);
    }
    F<PtrList*>(o, 0x79) = nullptr;
}
RT3_REPLACE(0x52eeb0, ModelSet_Clear);

// 0x52ef10. Frees the sprite list +0x7d: each sprite's image is released and the
// sprite deleted, then the list itself.
void __fastcall SpriteSet_Free(uint8_t* o) {
    if (!F<PtrList*>(o, 0x7d)) return;
    PtrList_Rewind(F<PtrList*>(o, 0x7d));
    for (uint8_t* s = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x7d)); s; s = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x7d))) {
        Sprite_ReleaseImage(s);
        crt::operator_delete(s);
    }
    if (PtrList* l = F<PtrList*>(o, 0x7d)) {
        PtrList_Destroy(l);
        crt::operator_delete(l);
    }
    F<PtrList*>(o, 0x7d) = nullptr;
}
RT3_REPLACE(0x52ef10, SpriteSet_Free);

// 0x52ef70. Scene object teardown: lets go of the camera that follows it,
// unregisters it from the terrain (global list node +0xc, cell list node +8 at
// its cell from +0x1e2 / +0x1ea), deletes its models, child objects (+0x75,
// recursively), sprites and sound channels (+0x81), clears the scene's
// selection (+0x366e) and releases its two pack resources (+0x10, +0x1c).
RT3_NOINLINE void __fastcall SceneObj_Destruct(uint8_t* o) {
    uint8_t* e = g_engine;
    if (e && F<uint8_t*>(e, 0x28) && F<uint8_t*>(F<uint8_t*>(e, 0x28), 0x60) == o) Camera_SetMode(F<uint8_t*>(e, 0x28), 0, 0);
    if (F<uint8_t*>(o, 4) && F<PtrListNode*>(o, 0xc)) Terrain_RemoveGlobal(F<uint8_t*>(o, 4), 0, F<PtrListNode*>(o, 0xc));
    Scene_MarkDirty((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)));
    ModelSet_Clear(o);
    if (F<PtrList*>(o, 0x75)) {
        PtrList_Rewind(F<PtrList*>(o, 0x75));
        for (uint8_t* c; (c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75))) != nullptr;) {
            F<uint32_t>(c, 0) = 0x5dd1f0;
            SceneObj_Destruct(c);
            crt::operator_delete(c);
        }
        if (PtrList* l = F<PtrList*>(o, 0x75)) {
            PtrList_Destroy(l);
            crt::operator_delete(l);
        }
        F<PtrList*>(o, 0x75) = nullptr;
    }
    SpriteSet_Free(o);
    if (F<PtrList*>(o, 0x81)) {
        PtrList_Rewind(F<PtrList*>(o, 0x81));
        for (uint8_t* c; (c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x81))) != nullptr;) {
            Channel_Detach(c);
            crt::operator_delete(c);
        }
        if (PtrList* l = F<PtrList*>(o, 0x81)) {
            PtrList_Destroy(l);
            crt::operator_delete(l);
        }
        F<PtrList*>(o, 0x81) = nullptr;
    }
    if (F<uint8_t*>((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0x366e) == o)
        F<uint8_t*>((uint8_t*)Engine_Renderer(hook::As<void*>(g_engine)), 0x366e) = nullptr;
    if (uint8_t* t = F<uint8_t*>(o, 4)) {
        if (PtrListNode* n = F<PtrListNode*>(o, 8)) {
            int32_t y = Ftol((double)F<float>(o, 0x1ea) * Fl_205(0x5dcfe8)) >> 4;
            int32_t x = Ftol((double)F<float>(o, 0x1e2) * Fl_205(0x5dcfe8)) >> 4;
            Terrain_Remove1671(t, 0, n, x, y);
            F<uint8_t*>(o, 4) = nullptr;
        }
    }
    if (void* r = F<void*>(o, 0x10)) Pack_Release(g_pack, 0, r);
    F<void*>(o, 0x10) = nullptr;
    if (void* r = F<void*>(o, 0x1c)) Pack_Release(g_pack, 0, r);
    F<void*>(o, 0x1c) = nullptr;
}
RT3_REPLACE(0x52ef70, SceneObj_Destruct);

// 0x52f120. Rebuilds the world matrix +0x14e when dirty (flag 2): scale +0x36 and
// translation, then rotation by yaw/pitch/roll (+0x4f/+0x4b/+0x53) or, for
// quaternion-driven objects (flag 0x80), by the local matrix +0x18e (D3DX).
void __fastcall SceneObj_UpdateMatrix(uint8_t* o) {
    if (!(o[0x20] & 2)) return;
    o[0x20] = (uint8_t)((o[0x20] & 0xfd) | 4);
    float* m = (float*)(o + 0x14e);
    for (int k = 0; k < 16; ++k) m[k] = 0.0f;
    float s = F<float>(o, 0x36);
    m[10] = s;
    m[5] = s;
    m[0] = s;
    F<uint32_t>(o, 0x17e) = F<uint32_t>(o, 0x1e2);
    F<uint32_t>(o, 0x186) = F<uint32_t>(o, 0x1ea);
    F<uint32_t>(o, 0x18a) = 0x3f800000;
    F<uint32_t>(o, 0x182) = F<uint32_t>(o, 0x1e6);
    float rot[16], res[16];
    auto mul = (void*(__stdcall*)(float*, const void*, const float*))0x56d3cd;
    if (!(o[0x20] & 0x80)) {
        ((void*(__stdcall*)(float*, float, float, float))0x56e1a8)(rot, F<float>(o, 0x4f), F<float>(o, 0x4b),
                                                                   F<float>(o, 0x53));
        mul(res, rot, m);
    } else {
        mul(res, o + 0x18e, m);
    }
    for (int k = 0; k < 16; ++k) m[k] = res[k];
}
RT3_REPLACE(0x52f120, SceneObj_UpdateMatrix);

// 0x52f1e0. Whether the ray from o along dir hits triangle (a, b, c) from the front
// (determinant at least 0x5dd230), Moller-Trumbore style. Callers keep ECX/EDX.
int32_t __stdcall Ray_HitsTriangle_Impl(float a0, float a1, float a2, float b0, float b1, float b2, float c0, float c1,
                                   float c2, float o0, float o1, float o2, float d0, float d1, float d2) {
    float e2x = (float)((double)c0 - a0), e2y = (float)((double)c1 - a1), e2z = (float)((double)c2 - a2);
    double e1x = (double)b0 - a0, e1y = (double)b1 - a1, e1z = (double)b2 - a2;
    float px = (float)((double)d1 * e1z - (double)d2 * e1y);
    float py = (float)((double)d2 * e1x - e1z * d0);
    double pz = e1y * d0 - (double)d1 * e1x;
    double detx = (double)e2z * pz + (double)e2y * py + (double)px * e2x;
    float det = (float)detx;
    if (detx < Fl(0x5dd230)) return 0;
    float t0 = (float)((double)o0 - a0), t1 = (float)((double)o1 - a1);
    double t2x = (double)o2 - a2;
    float t2 = (float)t2x;
    float u = (float)(t2x * pz + (double)t1 * py + (double)t0 * px);
    if (u < Fl(0x5c8598)) return 0;
    if (u > det) return 0;
    double q0 = (double)t1 * e2z - (double)t2 * e2y;
    double q1 = (double)t2 * e2x - (double)e2z * t0;
    double q2 = (double)e2y * t0 - (double)t1 * e2x;
    double v = (double)d2 * q2 + (double)d1 * q1 + q0 * d0;
    if (v < Fl(0x5c8598)) return 0;
    return u + v > det ? 0 : 1;
}

__declspec(naked) int32_t __stdcall Ray_HitsTriangle(float, float, float, float, float, float, float, float, float,
                                                     float, float, float, float, float, float) {
    RT3_KEEP_ECX_EDX(Ray_HitsTriangle_Impl, 15)
}
RT3_REPLACE(0x52f1e0, Ray_HitsTriangle);

// 0x52f370. Picks the mesh LOD (+0x21, entry +0x18) of the object for the viewer
// at (x, y, z): its screen size (base +0x10 over 4 d^2 + 1, d from the distance
// table 0x6d9098, times the renderer's layer value and quality halvings) against
// each LOD's threshold (+0xc); forced by engine +0x1146c2. +0x5c bit 2 = tiny.
int32_t __fastcall SceneObj_SelectLod(uint8_t* o, int, float x, float y, float z) {
    if (!F<void*>(o, 4)) return 0;
    const uint16_t* tab = (const uint16_t*)0x6d9098;
    auto cell = [](float v) { int32_t i = (int32_t)v; return (uint16_t)((i < 0 ? -i : i) >> 6); };
    uint32_t a = cell(x);
    uint32_t b = cell(z);
    uint32_t t = tab[a * 0x418 + b];
    uint32_t d = tab[t * 0x418 + cell(y)];
    int32_t dist2 = (int32_t)(d * d * 4 + 1);
    int32_t layer = ((int32_t(__thiscall*)(void*))(*(void***)o)[0x18 / 4])(o);
    float r = Render_GetLayerValue(F<uint8_t*>(o, 4), 0, layer);
    if (F<int32_t>(Engine_Renderer(hook::As<void*>(g_engine)), 0x22b63) > 0) r = (float)((double)r * D(0x5d08a0));
    double q = r;
    if (F<uint8_t>(Engine_Renderer(hook::As<void*>(g_engine)), 0x3692)) q = (double)r * D(0x5d08a0);
    uint8_t* mesh = F<uint8_t*>(o, 0x14);
    float size = (float)((double)F<float>(F<uint8_t*>(mesh, 0xc), 0x10) / dist2 * q);
    if ((o[0x5c] & 1) && size <= D(0x5c8628)) o[0x5c] |= 2;
    else o[0x5c] &= 0xfd;
    F<int32_t>(o, 0x21) = F<int32_t>(mesh, 0x14) - 1;
    for (int32_t i = 0; i < F<int32_t>(F<uint8_t*>(o, 0x14), 0x14); ++i)
        if ((double)F<int32_t>(F<uint8_t*>(mesh, 0xc), 0xc + i * 0x45) < size) {
            F<int32_t>(o, 0x21) = i;
            break;
        }
    int32_t forced = F<int32_t>(g_engine, 0x1146c2);
    if (forced >= 0) {
        int32_t last = F<int32_t>(mesh, 0x14) - 1;
        F<int32_t>(o, 0x21) = forced < last ? forced : last;
    }
    int32_t lod = F<int32_t>(o, 0x21);
    F<uint8_t*>(o, 0x18) = F<uint8_t*>(mesh, 0xc) + lod * 0x45;
    return lod;
}
RT3_REPLACE(0x52f370, SceneObj_SelectLod);

// 0x52f500. Emits the object's current triangles (mesh +0x18: 0x4c-byte records of
// 3 vertex pointers, 3 normals, 3 uvs; walked through the globals 0xcc3b64 /
// 0xcc3b5c) as 0x20-byte vertices (position, normal, uv) into `out`, adding them to
// *count. When pickable (+0x20 & 1) and given a ray (`dir` from `origin`), each
// triangle is first tested against the ray in model space (inverse transform +0x8e,
// refreshed when +0x20 & 4); after the first hit the rest are copied untested.
// Returns whether the ray hit.
int32_t __fastcall Mesh_EmitPickTriangles(uint8_t* o, int, uint8_t* out, int32_t* count, const float* dir, const float* origin) {
    uint8_t*& tri = hook::As<uint8_t*>(g_cc3b64);
    int32_t& left = g_cc3b5c;
    tri = F<uint8_t*>(F<uint8_t*>(o, 0x18), 4);
    left = F<int32_t>(F<uint8_t*>(o, 0x18), 0xc);
    if (left <= 0) return 0;
    if (!(o[0x20] & 1)) dir = origin = nullptr;
    *count += left * 3;
    auto emit = [&]() {
        for (int32_t k = 0; k < 3; ++k) {
            memcpy(out, F<uint8_t*>(tri, k * 4), 12);
            memcpy(out + 0xc, tri + 0xc + k * 0xc, 12);
            F<uint32_t>(out, 0x18) = F<uint32_t>(tri, 0x30 + k * 8);
            F<uint32_t>(out, 0x1c) = F<uint32_t>(tri, 0x34 + k * 8);
            out += 0x20;
        }
        tri += 0x4c;
    };
    if (dir) {
        float dx = dir[0], dy = dir[1], dz = dir[2];
        if (o[0x20] & 4) {
            o[0x20] &= ~4;
            float det = 0.0f;
            ((void*(__stdcall*)(void*, float*, const void*))0x56d8d9)(o + 0x8e, &det, o + 0xce);
            (void)det;
        }
        auto m = [&](int r, int c) { return (double)F<float>(o, 0x8e + r * 16 + c * 4); };
        float ld[3], lo[3];
        ld[0] = (float)((dy * m(1, 0) + dz * m(2, 0)) + dx * m(0, 0));
        ld[1] = (float)((dz * m(2, 1) + dx * m(0, 1)) + dy * m(1, 1));
        ld[2] = (float)((dy * m(1, 2) + dz * m(2, 2)) + dx * m(0, 2));
        double px = (double)origin[0] - F<float>(o, 0x1e2), py = (double)origin[1] - F<float>(o, 0x1e6),
               pz = (double)origin[2] - F<float>(o, 0x1ea);
        lo[0] = (float)((py * m(1, 0) + pz * m(2, 0)) + px * m(0, 0));
        lo[1] = (float)((pz * m(2, 1) + px * m(0, 1)) + py * m(1, 1));
        lo[2] = (float)((py * m(1, 2) + pz * m(2, 2)) + px * m(0, 2));
        do {
            const float* a = F<float*>(tri, 0);
            const float* b = F<float*>(tri, 4);
            const float* c = F<float*>(tri, 8);
            --left;
            if (Ray_HitsTriangle(a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], lo[0], lo[1], lo[2], ld[0], ld[1], ld[2])) {
                emit();
                while (left) {
                    --left;
                    emit();
                }
                return 1;
            }
            emit();
        } while (left);
        return 0;
    }
    do {
        --left;
        emit();
    } while (left);
    return 0;
}
RT3_REPLACE(0x52f500, Mesh_EmitPickTriangles);

// 0x52f970. Turns sub-mesh `idx` (clamped to the LOD's count) of the object to face
// camera `cam`: when `key` changed (+0x31) the vertices (+0, count +8) are rotated
// by -pitch/2 then yaw into the copy at +0x35 (allocated on first use).
void __fastcall Mesh_FaceCamera(uint8_t* o, int, uint8_t* cam, int32_t key, int32_t idx) {
    uint8_t* lod = F<uint8_t*>(o, 0x14);
    if (idx > F<int32_t>(lod, 0x14) - 1) idx = F<int32_t>(lod, 0x14) - 1;
    uint8_t* sub = F<uint8_t*>(lod, 0xc) + idx * 0x45;
    if (F<int32_t>(sub, 0x31) == key) return;
    F<int32_t>(sub, 0x31) = key;
    if (!F<void*>(sub, 0x35)) F<void*>(sub, 0x35) = crt::malloc(F<int32_t>(sub, 8) * 0xc);
    float yaw, pitch;
    float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    float r[16];
    Camera_Angles(cam, 0, &yaw, &pitch, nullptr);
    auto mul = (void*(__stdcall*)(float*, const float*, const float*))0x56d3cd;
    ((void*(__stdcall*)(float*, float))0x56dda8)(r, (float)(-(double)pitch * *(const double*)rt3::Rdata(0x5c8628)));
    mul(m, m, r);
    ((void*(__stdcall*)(float*, float))0x56de44)(r, yaw);
    mul(m, m, r);
    const float* src = F<const float*>(sub, 0);
    float* dst = F<float*>(sub, 0x35);
    for (int32_t i = 0; i < F<int32_t>(sub, 8); ++i, src += 3, dst += 3) {
        double x = src[0], y = src[1], z = src[2];
        dst[0] = (float)((double)m[8] * z + y * m[4] + x * m[0]);
        dst[1] = (float)((double)m[9] * z + y * m[5] + x * m[1]);
        dst[2] = (float)(z * m[10] + y * m[6] + x * m[2]);
    }
}
RT3_REPLACE(0x52f970, Mesh_FaceCamera);

// 0x52fb70. Sets scene object o's alpha (+0x47 top byte), and with `deep` its
// children's. In the fixed-alpha mode (engine +0x114742) transparency becomes a
// grey tint instead (0x3c3c3c below 0xff, white at 0xff) on the object, its
// children and their children, and the alpha stays opaque.
RT3_NOINLINE void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alphaArg, int32_t deep) {
    uint8_t alpha = (uint8_t)alphaArg;
    auto forEach = [](uint8_t* p, auto&& fn) {
        PtrList* l = F<PtrList*>(p, 0x75);
        if (!l) return;
        PtrList_Rewind(l);
        for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(p, 0x75)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(p, 0x75))) fn(c);
    };
    if (!F<int32_t>(g_engine, 0x114742)) {
        F<uint32_t>(o, 0x47) = (uint32_t)alpha << 24;
        if (deep) forEach(o, [&](uint8_t* c) { SceneObj_SetAlpha(c, 0, alphaArg, 1); });
        return;
    }
    auto grey = [&](uint8_t* p) {
        uint32_t v = alpha < 0xff ? 0x3c : 0xff;
        F<uint32_t>(p, 0x43) = F<uint32_t>(p, 0x47) | (alpha < 0xff ? 0x3c3c3cu : 0xffffffu);
        forEach(p, [&](uint8_t* c) { SceneObj_SetTint(c, 0, v, v, v, 1); });
    };
    grey(o);
    if (deep)
        forEach(o, [&](uint8_t* c) {
            grey(c);
            SceneObj_SetAlpha(c, 0, alphaArg, 1);
        });
    F<uint32_t>(o, 0x47) = 0xff000000;
}
RT3_REPLACE(0x52fb70, SceneObj_SetAlpha);

// 0x52fd60. Moves all the object's sound channels to its position.
RT3_NOINLINE void __fastcall Obj_MoveChannels(uint8_t* o) {
    PtrList* l = F<PtrList*>(o, 0x81);
    if (!l || !l->count) return;
    PtrList_Rewind(l);
    for (uint8_t* ch = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x81)); ch; ch = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x81)))
        Channel_MoveTo(ch, 0, F<float>(o, 0x1e2), F<float>(o, 0x1e6), F<float>(o, 0x1ea));
}
RT3_REPLACE(0x52fd60, Obj_MoveChannels);

// 0x52fdc0. Moves the object (+0x1e2..+0x1ea) by (dx, dy, dz), marking it moved
// (+0x20 bit 1); on a terrain (+4) it is re-filed in the 16-cell grid (node +8) when
// it changes cell, and the move is undone (returning false) when it would leave the
// map. Its sound channels follow.
RT3_NOINLINE bool __fastcall Obj_MoveBy(uint8_t* o, int, float dx, float dy, float dz) {
    o[0x20] |= 2;
    uint8_t* grid = F<uint8_t*>(o, 4);
    if (!grid) {
        F<float>(o, 0x1e2) = (float)((double)dx + F<float>(o, 0x1e2));
        F<float>(o, 0x1e6) = (float)((double)dy + F<float>(o, 0x1e6));
        F<float>(o, 0x1ea) = (float)((double)dz + F<float>(o, 0x1ea));
        Obj_MoveChannels(o);
        return true;
    }
    int32_t ox = Cell16(F<float>(o, 0x1e2)), oz = Cell16(F<float>(o, 0x1ea));
    float nx = (float)((double)dx + F<float>(o, 0x1e2));
    F<float>(o, 0x1e2) = nx;
    float ny = (float)((double)dy + F<float>(o, 0x1e6));
    F<float>(o, 0x1e6) = ny;
    float nz = (float)((double)dz + F<float>(o, 0x1ea));
    F<float>(o, 0x1ea) = nz;
    int32_t cx = Cell16(nx), cz = Cell16(nz);
    if (ox == cx && oz == cz) {
        Obj_MoveChannels(o);
        return true;
    }
    if (cx < 0 || cz < 0) {
        F<float>(o, 0x1e2) = (float)((double)nx - dx);
        F<float>(o, 0x1e6) = (float)((double)ny - dy);
        F<float>(o, 0x1ea) = (float)((double)nz - dz);
        return false;
    }
    int32_t w = (Terrain_Rows(F<uint8_t*>(o, 4)) - 1) >> 4;
    int32_t h = (Terrain_Cols(F<uint8_t*>(o, 4)) - 1) >> 4;
    if (cx >= w || cz >= h) {
        F<float>(o, 0x1e2) = (float)((double)F<float>(o, 0x1e2) - dx);
        F<float>(o, 0x1e6) = (float)((double)F<float>(o, 0x1e6) - dy);
        F<float>(o, 0x1ea) = (float)((double)F<float>(o, 0x1ea) - dz);
        return false;
    }
    if (F<PtrListNode*>(o, 8)) Terrain_Remove1671(F<uint8_t*>(o, 4), 0, F<PtrListNode*>(o, 8), ox, oz);
    F<PtrListNode*>(o, 8) = Terrain_Add1671(F<uint8_t*>(o, 4), 0, o, cx, cz);
    Obj_MoveChannels(o);
    return true;
}
RT3_REPLACE(0x52fdc0, Obj_MoveBy);

// 0x52ffb0. Scales the object's quaternion rotation (+0x1ce): rotates again
// about its own axis (scaled by 0x5c9b08) by its angle times f (D3DX).
RT3_NOINLINE void __fastcall SceneObj_ScaleRotation(uint8_t* o, int, float f) {
    float angle, axis[3];
    ((void*(__stdcall*)(const void*, float*, float*))0x56e556)(o + 0x1ce, axis, &angle);
    const float k = *(const float*)rt3::Rdata(0x5c9b08);
    float ax = (float)((double)axis[0] * k);
    axis[0] = ax;
    axis[1] = (float)((double)axis[1] * k);
    axis[2] = (float)((double)axis[2] * k);
    float a = (float)((double)angle * f);
    SceneObj_RotateAxis(o, 0, ax, axis[1], axis[2], a);
}
RT3_REPLACE(0x52ffb0, SceneObj_ScaleRotation);

}  // namespace rt3
