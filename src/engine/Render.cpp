// Render.cpp - RT3.exe 0x544000-0x54c000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include "game/collection.h"
#include "core/containers.h"
#include "hook/keepregs.h"

#undef near
#undef far
#undef small

struct HWND__;

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                            const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall Fatal_Exit(const char* msg);
int32_t __fastcall Engine_TotalTextureMem(void* e);
int32_t __fastcall Engine_TextureMemLimit(void* e);
void __fastcall Engine_CurrentMode(void* e, int, int32_t* w, int32_t* h, int32_t* bpp, int32_t* refresh, uint32_t* flags);
HWND__* __fastcall Engine_Window(void* e);
int32_t __fastcall Engine_Get1146ae(void* e);
uint8_t __fastcall Render_CreateDevice(uint8_t* r, int, uint32_t behaviour, void* params);
void __fastcall Engine_SaveConfig(uint8_t* e);
void __fastcall Engine_SampleStartTextureMem(void* e);
void __fastcall Render_InitStages(uint8_t* r, int, int32_t n);
void __fastcall Render_ReleaseBuffers(uint8_t* r);
void __fastcall Render_SetGamma(uint8_t* r, int, float gamma);
int32_t __fastcall Sound_BuildCapsReport(uint8_t* o);
void __fastcall Render_BlankFrames(uint8_t* r);
int32_t __fastcall Render_Shutdown(uint8_t* r);
void __fastcall Engine_SetC(void* e, int, int32_t v);
void __fastcall Sky_BuildSunTable(uint8_t* o);
::HWND__* __fastcall Engine_Window(void* e);
void __fastcall Render_SetProjection(uint8_t* r, int, int32_t w, int32_t h);
void __fastcall Render_SetColorOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
void __fastcall Render_SetAlphaOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
void __fastcall Render_SetStreams(uint8_t* r, int, void* vb, uint32_t stride, void* ib);
void* __fastcall Texture_TargetForFrame(uint8_t* t);
int32_t __fastcall Texture_Upload(uint8_t* t, int, void* dev, uint8_t force);
void __fastcall Camera_BoundsPixels(uint8_t* c, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1);
void __fastcall Render_SetPixelViewport(uint8_t* r, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Render_FreeStages(uint8_t* r);
void __fastcall Render_ReleaseTextures(uint8_t* r);
void* __fastcall Engine_Get1C(void* e);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
int32_t __fastcall Texture_Height(uint8_t* t);
void __fastcall Render_FogOn(uint8_t* r);
template <typename T> static inline T& G(uint32_t a) { return *(T*)a; }
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Obj_GetXYZ4(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall Obj_GetXYZ2c(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void __fastcall Renderer_SetFog(uint8_t* r, int, float density, float cr, float cg, float cb);
HWND __fastcall Engine_Window(void* e);
void __fastcall Render_PushStages(uint8_t* r, int, const uint32_t* states);
void __fastcall Render_AlphaTestOn(uint8_t* r, int, uint32_t ref);
void __fastcall Renderer_SetTexture(uint8_t* r, int, int32_t stage, uint8_t* tex);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall PtrList_Clear(PtrList* l);
void __fastcall Render_PopStages(uint8_t* r, int, int32_t n);
void __fastcall Render_SetTextureTransform(uint8_t* r, int, float a, float b, float c, float d);
uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
void __fastcall Render_SetFiltering(uint8_t* r, int, uint8_t filtered, int8_t stage);
uint8_t __fastcall SceneObj_GetUvTransform(uint8_t* o, int, uint32_t* u, uint32_t* v, uint32_t* su, uint32_t* sv);
void* __fastcall Render_LockVbRange(uint8_t* r, int, int32_t first, int32_t end);
void __fastcall SceneObj_EmitLit(uint8_t* o, int, uint32_t* out, int32_t* count);
template <typename Fn> static inline Fn DevSlot(void* dev, uint32_t off) { return *(Fn*)(*(uint8_t**)dev + off); }
void __fastcall Renderer_SetupWaterStages(uint8_t* r);
void __fastcall Renderer_SetupStages(uint8_t* r);
void __fastcall Renderer_SetShaderConstants(uint8_t* r);

// Functions of this file, in exe order.
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall Render_SetViewOffset(uint8_t* r, int, uint32_t x, uint32_t y);
void __fastcall Render_SetProjection(uint8_t* r, int, int32_t w, int32_t h);
void __fastcall Render_WorldIdentity(uint8_t* r);
void __fastcall Render_WorldFrom(uint8_t* r, int, uint8_t* obj);
void __fastcall Render_ApplyLocal(uint8_t* r, int, uint8_t* obj);
uint8_t __fastcall Render_TakeFlag36f7(uint8_t* r, int, uint8_t clear);
void __fastcall Render_UseView2(uint8_t* r);
void __fastcall Render_UseView1(uint8_t* r);
void __fastcall Render_UseView3(uint8_t* r);
void __fastcall Render_SetVec2533(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Render_GetViewport(uint8_t* r, int, uint32_t* sx, uint32_t* sy, uint32_t* ox, uint32_t* oy);
int32_t __fastcall Render_ProjectSimple(uint8_t* r, int, float x, float y, float z, float* ox, float* oy);
int32_t __fastcall Render_Project(uint8_t* r, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Render_TransformWorld(uint8_t* r, int, float* v);
int32_t __fastcall Render_ProjectPoint(uint8_t* r, int, float x, float y, float z, float* sx, float* sy);
void __fastcall Render_Unproject(uint8_t* r, int, float sx, float sy, float* ox, float* oy, float* oz);
void __fastcall Render_UnprojectDir(uint8_t* r, int, float sx, float sy, const float* eye, float* out);
void __fastcall Render_AddressBorder(uint8_t* r);
void __fastcall Render_AddressClamp(uint8_t* r, int, uint8_t stage);
void __fastcall Render_AddressMirror(uint8_t* r);
void __fastcall Render_AddressWrap(uint8_t* r);
int32_t __fastcall Render_SetTextureFactor(uint8_t* r, int, uint32_t v);
int32_t __fastcall Render_SetFactorAlpha(uint8_t* r, int, uint8_t a);
int32_t __fastcall Render_SetTexCoordIndex(uint8_t* r, int, uint32_t stage, uint32_t v);
void __fastcall Render_ApplyFilterStates(uint8_t* r);
void __fastcall Render_SetFiltering(uint8_t* r, int, uint8_t filtered, int8_t stage);
int32_t __fastcall Render_LightingOff(uint8_t* r);
int32_t __fastcall Render_LightingDefault(uint8_t* r);
int32_t __fastcall Render_LightingAndSun(uint8_t* r);
int32_t __fastcall Render_ZWriteOff(uint8_t* r);
int32_t __fastcall Render_ZWriteOn(uint8_t* r);
void __fastcall Render_GetFogBlock(uint8_t* r, int, uint8_t* dst);
void __fastcall Render_SunDir(uint8_t* r, int, float* x, float* y, float* z);
void __fastcall Render_SunDirVec(uint8_t* r, int, float* v);
void __fastcall Sky_BuildSunTable(uint8_t* o);
void __fastcall Render_SetLightRange(uint8_t* r, int, int32_t i, float v);
void __fastcall Render_SetPair25f7(uint8_t* r, int, int32_t i, float a, float b);
void __fastcall Render_GetPair25f7(uint8_t* r, int, int32_t i, uint32_t* a, uint32_t* b);
void __fastcall Render_SetLightPair(uint8_t* r, int, int32_t i, int32_t j, float a, float b);
float __fastcall Render_GetLightValue(uint8_t* r, int, int32_t i, int32_t j);
void* __fastcall Render_PushStageState(uint8_t* r);
void __fastcall Renderer_SetTexture(uint8_t* r, int, int32_t stage, uint8_t* tex);
void __fastcall Render_SetColorOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
void __fastcall Render_SetAlphaOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
void __fastcall Render_SetDefaultMaterial(uint8_t* r, int, uint8_t shiny);
void* __fastcall Render_LockVb2(uint8_t* r);
int32_t __fastcall Render_UnlockVb2(uint8_t* r);
void* __fastcall Render_LockVbRange(uint8_t* r, int, int32_t first, int32_t end);
void* __fastcall Render_LockVbDiscard(uint8_t* r, int, int32_t n);
void __fastcall Render_UnlockVbRange(uint8_t* r);
int32_t __fastcall Render_UnlockVb264f(uint8_t* r);
void* __fastcall Render_LockVb264f(uint8_t* r);
void* __fastcall Render_LockVb2653(uint8_t* r);
int32_t __fastcall Render_UnlockVb2653(uint8_t* r);
void* __fastcall Render_LockVb264b(uint8_t* r);
int32_t __fastcall Render_UnlockVb264b(uint8_t* r);
void* __fastcall Render_LockVb2647(uint8_t* r, int, int32_t first, int32_t count);
void* __fastcall Render_AppendVb2647(uint8_t* r, int, int32_t first, int32_t count);
int32_t __fastcall Render_UnlockVb2647(uint8_t* r);
void* __fastcall Render_LockVb263f(uint8_t* r);
int32_t __fastcall Render_UnlockVb263f(uint8_t* r);
void __fastcall Render_DrawIndexed(uint8_t* r, int, int32_t indices, uint32_t minIndex, uint32_t vertices);
void __fastcall Render_DrawTriangles(uint8_t* r, int, int32_t first, int32_t end);
int32_t __fastcall Render_DrawPoints(uint8_t* r, int, int32_t first, int32_t end);
RT3_NOINLINE int32_t __fastcall Render_TriangleCount(uint8_t* r);
int32_t __fastcall Render_BeginFrame(uint8_t* r, int, int32_t clear);
void __fastcall Render_EndFrame(uint8_t* r, int, int32_t noPresent);
void __fastcall Nop545df0(void*, int, int32_t, int32_t);
uint8_t __fastcall Render_DeviceState(uint8_t* r);
int32_t __fastcall Sound_BuildCapsReport(uint8_t* o);
uint8_t __fastcall Render_CreateDevice(uint8_t* r, int, uint32_t behaviour, void* params);
void __fastcall Render_ReleaseBuffers(uint8_t* r);
void __fastcall Render_BlankFrames(uint8_t* r);
int32_t __fastcall Render_ZFuncAlways(uint8_t* r);
int32_t __fastcall Render_ZFuncLessEqual(uint8_t* r);
int32_t __fastcall Render_ColorWriteOff(uint8_t* r);
int32_t __fastcall Render_ColorWriteOn(uint8_t* r);
void __fastcall Render_ReleaseTextures(uint8_t* r);
int32_t __fastcall Render_SetZEnable(uint8_t* r, int, uint32_t v);
int32_t __fastcall Render_SetShadeMode(uint8_t* r, int, uint32_t v);
void __fastcall Render_FogOn(uint8_t* r);
void __fastcall Render_FogOff(uint8_t* r);
void __fastcall Render_CullCcw(uint8_t* r);
void __fastcall Render_CullCw(uint8_t* r);
void __fastcall Render_CullNone(uint8_t* r);
RT3_NOINLINE int32_t __fastcall Render_ProjectClip(uint8_t* r, int, float x, float y, float z, float limit, float maxZ, float* out);
void __fastcall Render_ResetTextureSlots(uint8_t* r);
void __fastcall Render_ResetVertexSlots(uint8_t* r);
uint8_t* __fastcall Render_AcquireVertexSlot(uint8_t* r, int, int32_t key1, int32_t key2);
void __fastcall Renderer_SetShaderConstants(uint8_t* r);
void __fastcall Render_SetStreams(uint8_t* r, int, void* vb, uint32_t stride, void* ib);
int32_t __fastcall Render_BlendOff(uint8_t* r);
void __fastcall Render_AllowTranslucency(uint8_t* r);
void __fastcall Render_NoTranslucency(uint8_t* r);
int32_t __fastcall Render_BlendDefault(uint8_t* r);
void __fastcall Render_AlphaTestOn(uint8_t* r, int, uint32_t ref);
void __fastcall Render_AlphaTestOff(uint8_t* r);
void __fastcall Render_ScreenSpaceWorld(uint8_t* r);
void __fastcall Render_SetSubViewport(uint8_t* r, int, float x0, float y0, float x1, float y1);
RT3_NOINLINE void __fastcall Render_RestoreTarget(uint8_t* r);
void __fastcall Render_TargetChanged(uint8_t* r);
void __fastcall Renderer_SetupStages(uint8_t* r);
void __fastcall Renderer_SetupWaterStages(uint8_t* r);
void __fastcall Render_SetShaderOffset(uint8_t* r, int, float a, float b, float c, float d);
void __fastcall Render_SetTextureTransform(uint8_t* r, int, float a, float b, float c, float d);
void __fastcall Render_ScreenSpaceAll(uint8_t* r);
void __fastcall Render_SetGamma(uint8_t* r, int, float gamma);
void __fastcall Render_ApplyStoredTransforms(uint8_t* r);
int32_t __fastcall Render_BlendDarken(uint8_t* r);
int32_t __fastcall Render_BlendAlpha(uint8_t* r);
RT3_NOINLINE double __fastcall Render_ScaleByUnit(uint8_t*, int, float v);
double __stdcall PointLineDistance(float px, float py, float pz, float dx, float dy, float dz, float qx, float qy, float qz);
void __fastcall Render_ResetWorld(uint8_t* r);
void __fastcall Render_SetPixelViewport(uint8_t* r, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Render_SetCameraViewport(uint8_t* r, int, uint8_t* cam, uint8_t projOnly);
void __fastcall Renderer_SetupViews(uint8_t* r, int, uint8_t* cam, uint8_t reflect);
void __fastcall Render_ResetStageCache(uint8_t* r);
void __fastcall Render_PushStages(uint8_t* r, int, const uint32_t* states);
void __fastcall Render_PopStages(uint8_t* r, int, int32_t n);
RT3_NOINLINE int32_t __fastcall Render_ValidateStates(uint8_t* r, int, const uint32_t* states);
void __fastcall Render_InitStages(uint8_t* r, int, int32_t n);
void __fastcall Render_FreeStages(uint8_t* r);
void __fastcall Render_DrawLitGroups(uint8_t* r, int, PtrList* groups, uint8_t release);
RT3_NOINLINE int32_t __fastcall Render_InitDevice(uint8_t* r, int, uint8_t fullScreen);
int32_t __fastcall Render_Shutdown(uint8_t* r);
void __fastcall Renderer_SetFog(uint8_t* r, int, float density, float cr, float cg, float cb);
void __fastcall Terrain_DrawChunks(uint8_t* r, int, int32_t count);
void __fastcall Terrain_DrawChunksShaded(uint8_t* r, int, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy, uint8_t water, uint8_t* extra, uint8_t invert);
void __fastcall Terrain_DrawChunksPaired(uint8_t* r, int, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy, uint8_t water);
void __fastcall Terrain_DrawChunksFixed(uint8_t* r, int, int32_t vertices, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy);
void __fastcall Render_BindVb2643(uint8_t* r);
void __fastcall Render_BindVb263f(uint8_t* r);
void __fastcall Render_BindVb2647(uint8_t* r);
void __fastcall Render_BindPair264f(uint8_t* r);
void __fastcall Render_BindPair2647(uint8_t* r);
uint8_t* __fastcall Render_Construct(uint8_t* r, int, int32_t, uint8_t fullScreen);
RT3_NOINLINE void __fastcall Render_SaveAndShutdown(uint8_t* r);
void __fastcall Renderer_SetTimeOfDay(uint8_t* r, int, int32_t secs, int32_t night);

// D3DX8 statics in the exe.
static inline void MatMul(void* out, const void* a, const void* b) {
    ((void*(__stdcall*)(void*, const void*, const void*))0x56d3cd)(out, a, b);
}

static inline void MatTranspose(void* out, const void* m) { ((void*(__stdcall*)(void*, const void*))0x56d616)(out, m); }

static inline void MatInverse(void* out, float* det, const void* m) {
    ((void*(__stdcall*)(void*, float*, const void*))0x56d8d9)(out, det, m);
}

// Device +0x25df (IDirect3DDevice8): SetTransform (vtbl +0x94), Clear (+0x90).
static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }

static inline int32_t SetTransform(uint8_t* r, int32_t state, void* m) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, int32_t, void*))(*(void***)d)[0x94 / 4])(d, state, m);
}

// Row-major 4x4 at r + base: m(row, col).
static inline double M(uint8_t* r, uint32_t base, int row, int col) { return F<float>(r, base + (row * 4 + col) * 4); }

// Texture addressing (SetTextureStageState vtbl +0xfc, ADDRESSU 0xd / ADDRESSV 0xe).
static inline void SetTss(uint8_t* r, uint32_t stage, uint32_t type, uint32_t v) {
    void* d = F<void*>(r, 0x25df);
    ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))(*(void***)d)[0xfc / 4])(d, stage, type, v);
}

// 0x544d80 / 0x544da0. Texture factor render state (0x3c): raw / background colour with alpha.
static inline int32_t SetRs(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = F<void*>(r, 0x25df);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))(*(void***)d)[0xc8 / 4])(d, state, v);
}

static inline int32_t SetTss_96(uint8_t* r, uint32_t stage, uint32_t type, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))(*(void***)d)[0xfc / 4])(d, stage, type, v);
}

static inline int32_t SetRs_96(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))(*(void***)d)[0xc8 / 4])(d, state, v);
}

static inline uint8_t* Cfg(uint8_t* r) { return F<uint8_t*>(r, 0x83c); }

static inline int32_t SetTss_97(uint8_t* r, uint32_t stage, uint32_t type, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))(*(void***)d)[0xfc / 4])(d, stage, type, v);
}

static inline int32_t VbLock(void* vb, uint32_t off, uint32_t size, void* pp, uint32_t flags) {
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t))(*(void***)vb)[0x2c / 4])(vb, off, size, pp, flags);
}

// 0x5455e0 / 0x5456d0. Colour / alpha op of a stage (op, arg1, arg2), set on the
// device only where the cache differs (or just cached when not applying).
static inline void SetStage3(uint8_t* r, int32_t stage, uint32_t base, uint32_t tss, uint32_t arg1, uint32_t op,
                             uint32_t arg2, uint8_t apply) {
    if (stage >= F<int32_t>(r, 0x258f)) return;
    uint32_t at = stage * 24 + base;
    if (!apply) {
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at) = op;
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 4) = arg1;
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 8) = arg2;
        return;
    }
    if (F<uint32_t>(F<uint8_t*>(r, 0x2597), at) != op) {
        SetTss_97(r, stage, tss, op);
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at) = op;
    }
    if (F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 4) != arg1) {
        SetTss_97(r, stage, tss + 1, arg1);
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 4) = arg1;
    }
    if (F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 8) != arg2) {
        SetTss_97(r, stage, tss + 2, arg2);
        F<uint32_t>(F<uint8_t*>(r, 0x2597), at + 8) = arg2;
    }
}

static inline int32_t& LockCount() { return g_ccd824; }

static inline int32_t Lock(void* b, uint32_t off, uint32_t size, void* pp, uint32_t flags) {
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t))(*(void***)b)[0x2c / 4])(b, off, size, pp, flags);
}

static inline int32_t Unlock(void* b) { return ((int32_t(__stdcall*)(void*))(*(void***)b)[0x30 / 4])(b); }

static inline int32_t DrawPrimitive(void* d, uint32_t type, uint32_t first, uint32_t count) {
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))(*(void***)d)[0x118 / 4])(d, type, first, count);
}

namespace {
template <typename R = int32_t, typename... A> inline R Com(void* o, uint32_t slot, A... a) {
    return ((R(__stdcall*)(void*, A...))(*(void***)o)[slot / 4])(o, a...);
}
inline uint8_t* Eng() { return g_engine; }
inline void* D3DCreate(uint32_t sdk) { return ((void*(__stdcall*)(uint32_t))0x56c850)(sdk); }
// D3DXAssembleShaderFromFile (statically linked D3DX).
inline int32_t AssembleShader(const char* file, void** buf) {
    return ((int32_t(__stdcall*)(const char*, uint32_t, void*, void**, void*))0x57104e)(file, 0, nullptr, buf, nullptr);
}
// Appends string `id` (formatted with the current text and up to two more
// strings) to the device report +0x3c.
void Report(uint8_t* r, int32_t id, const char* a2 = nullptr, const char* a3 = nullptr) {
    char tmp[0x400];
    FormatText(tmp, GetString(id), (const char*)r + 0x3c, a2, a3, 0, 0, 0, 0, 0);
    strcpy((char*)r + 0x3c, tmp);
}
struct DisplayMode {
    uint32_t width, height, refresh, format;
};
const int32_t kNotAvailable = (int32_t)0x8876086a, kNotFound = (int32_t)0x8876086c;
inline bool Unsupported(int32_t r) { return r == kNotAvailable || r == kNotFound; }
}

static inline void** Vt(void* p) { return *(void***)p; }

static inline int32_t Call0(void* d, uint32_t off) { return ((int32_t(__stdcall*)(void*))Vt(d)[off / 4])(d); }

static inline int32_t SetRs_99(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))Vt(d)[0xc8 / 4])(d, state, v);
}

static inline int32_t Clear(void* d, uint32_t flags) {
    return ((int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t, uint32_t, uint32_t, uint32_t))Vt(d)[0x90 / 4])(
        d, 0, nullptr, flags, 0, 0x3f800000, 0);
}

static inline int32_t Present(void* d) {
    return ((int32_t(__stdcall*)(void*, void*, void*, void*, void*))Vt(d)[0x3c / 4])(d, nullptr, nullptr, nullptr, nullptr);
}

static inline uint8_t* Engine() { return g_engine; }

static inline int32_t SetRs_100(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))Vt(d)[0xc8 / 4])(d, state, v);
}

static inline int32_t SetTss_100(uint8_t* r, uint32_t stage, uint32_t type, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))Vt(d)[0xfc / 4])(d, stage, type, v);
}

static inline int32_t SetTransform_100(uint8_t* r, uint32_t state, const void* m) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, const void*))Vt(d)[0x94 / 4])(d, state, m);
}

// Screen-space world matrix: x * 2 - 1, -y * 2 + 1.
static inline void ScreenMatrix(uint32_t* m) {
    for (int k = 0; k < 16; ++k) m[k] = 0;
    m[15] = 0x3f800000;
    m[10] = 0x3f800000;
    m[0] = 0x40000000;
    m[5] = 0xc0000000;
    m[12] = 0xbf800000;
    m[13] = 0x3f800000;
}

static inline int32_t SetRs_101(uint8_t* r, uint32_t state, uint32_t v) {
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))Vt(d)[0xc8 / 4])(d, state, v);
}

static inline double Fsqrt(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }

static inline void ApplyStages(uint8_t* r, const uint32_t* st) {
    for (int32_t s = 0; s < F<int32_t>(r, 0x258f); ++s, st += 6) {
        Render_SetColorOp(r, 0, s, st[1], st[0], st[2], 1);
        Render_SetAlphaOp(r, 0, s, st[4], st[3], st[5], 1);
    }
}

// 0x54bab0 / 0x54bb10 / 0x54bb70. Bind a fixed vertex buffer without indices.
static inline void BindFixed(uint8_t* r, uint32_t slot, uint32_t fvf, uint32_t stride) {
    void* vb = F<void*>(r, slot);
    if (!vb) {
        F<void*>(r, 0x2637) = nullptr;
        return;
    }
    if (F<void*>(r, 0x2637) != vb) {
        void* d = Dev(r);
        ((int32_t(__stdcall*)(void*, uint32_t))Vt(d)[0x130 / 4])(d, fvf);
        d = Dev(r);
        ((int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t))Vt(d)[0x14c / 4])(d, 0, vb, stride);
        F<void*>(r, 0x2637) = vb;
    }
    F<void*>(r, 0x263b) = nullptr;
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline int32_t Release(void* p) { return ((int32_t(__stdcall*)(void*))Vt(p)[2])(p); }

// Releases COM object *slot (clearing it when asked); fatal (string 0x9b) when it
// still has references.
static void ReleaseChecked(uint8_t* r, uint32_t off, bool clear) {
    void* p = F<void*>(r, off);
    if (!p) {
        if (clear) F<void*>(r, off) = nullptr;
        return;
    }
    int32_t left = Release(p);
    if (clear) F<void*>(r, off) = nullptr;
    if (left) Fatal_Exit(GetString(0x9b));
}

static inline int32_t VbUnlock(void* vb) { return ((int32_t(__stdcall*)(void*))Vt(vb)[0x30 / 4])(vb); }

static inline uint8_t* Slot(uint8_t* r, int32_t i) { return r + 0x266b + i * 0x2d; }

// Saves the device viewport into *slot (6 dwords) the first time.
static void SaveViewport(uint8_t* r, uint32_t slot) {
    if (F<void*>(r, slot)) return;
    void* vp = Malloc(0x18);
    F<void*>(r, slot) = vp;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*))Vt(d)[0xa4 / 4])(d, vp);
}

static inline double Fsin(double v) {
    __asm {
        fld v
        fsin
        fstp v
    }
    return v;
}

static inline double Fcos(double v) {
    __asm {
        fld v
        fcos
        fstp v
    }
    return v;
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

static inline double Dq(uint32_t a) { return *(const double*)a; }

static inline double Fq(uint32_t a) { return *(const float*)a; }

static inline void StageState(uint8_t* r, uint32_t stage, uint32_t type, uint32_t value) {
    void* dev = F<void*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t, uint32_t))(*(uint8_t**)dev + 0xfc))(dev, stage, type, value);
}

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

static inline double FsinX(double v) { double r; __asm { fld v } __asm { fsin } __asm { fstp r } return r; }

static inline double FcosX(double v) { double r; __asm { fld v } __asm { fcos } __asm { fstp r } return r; }

static inline void RenderState(uint8_t* r, uint32_t state, uint32_t value) {
    void* dev = F<void*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t))(*(uint8_t**)dev + 0xc8))(dev, state, value);
}

static inline int32_t Clamp255(int32_t v) { return v < 0 ? 0 : v > 0xff ? 0xff : v; }

static inline double Fl_201(uint32_t a) { return *(const float*)a; }

static inline void LookAt(void* out, const float* eye, const float* at, const float* up) {
    ((void*(__stdcall*)(void*, const float*, const float*, const float*))0x56e2d2)(out, eye, at, up);
}

static inline double Fl_206(uint32_t a) { return *(const float*)a; }

static inline void DevRenderState(uint8_t* r, uint32_t state, uint32_t value) {
    void* dev = F<void*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t))(*(uint8_t**)dev + 0xc8))(dev, state, value);
}

static inline void DevStageState(uint8_t* r, uint32_t stage, uint32_t type, uint32_t value) {
    void* dev = F<void*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t, uint32_t))(*(uint8_t**)dev + 0xfc))(dev, stage, type, value);
}

static inline uint8_t* LitObj_VbEntry(uint8_t* o) {
    uint8_t* t = F<uint8_t*>(o, 0x14);
    if (!t) return nullptr;
    return F<uint8_t*>(t + F<int32_t>(o, 0x3a) * 0x18, 0xc) + F<int32_t>(o, 0x21) * 0x45;
}

// Next chunk of a chain in direction table entry dir (3 and above 4 keep it).
static inline int32_t Chunk_Next(uint8_t* r, int32_t j, int32_t dir) {
    switch ((uint32_t)dir) {
    case 0: return F<int32_t>(r + j * 0x2d, 0x2687);
    case 1: return F<int32_t>(r + j * 0x2d, 0x267f);
    case 2: return F<int32_t>(r + j * 0x2d, 0x2683);
    case 4: return F<int32_t>(r + j * 0x2d, 0x267b);
    default: return j;
    }
}

static void Render_DropStageTexture(uint8_t* r, uint32_t st) {
    if (F<int32_t>(r, 0x258f) <= (int32_t)st) return;
    if (F<void*>(F<uint8_t*>(r, 0x2593), st * 4)) {
        ++F<int32_t>(r, 0x370d);
        void* dev = F<void*>(r, 0x25df);
        DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*)>(dev, 0xf4)(dev, st, nullptr);
    }
    F<void*>(F<uint8_t*>(r, 0x2593), st * 4) = nullptr;
}

void __fastcall Render_SetViewOffset(uint8_t* r, int, uint32_t x, uint32_t y) {
    F<uint32_t>(r, 0x25ef) = x;
    F<uint32_t>(r, 0x25f3) = y;
}
RT3_REPLACE(0x544270, Render_SetViewOffset);

// 0x544290. Off-centre perspective projection (+0x352f; near 8, far 65535) for a
// w x h viewport, offset +0x25ef/+0x25f3 and scale +0x25eb.
RT3_NOINLINE void __fastcall Render_SetProjection(uint8_t* r, int, int32_t w, int32_t h) {
    float inv = (float)((double)*(const float*)0x5c85b4 / ((double)w / h));
    const double k = *(const float*)0x5c9690, s = F<float>(r, 0x25eb);
    double a = inv * k;
    double b = (double)inv * F<float>(r, 0x25f3);
    float top = (float)((b + a) * s);
    float bottom = (float)((b - a) * s);
    float right = (float)(((double)F<float>(r, 0x25ef) + k) * s);
    float left = (float)(((double)F<float>(r, 0x25ef) - k) * s);
    ((void*(__stdcall*)(void*, float, float, float, float, float, float))0x56e41a)(r + 0x352f, left, right, bottom,
                                                                                   top, 8.0f, 65535.0f);
    F<float>(r, 0x36f9) = (float)((double)inv * F<float>(F<uint8_t*>(r, 0x83c), 0x49));
    SetTransform(r, 3, r + 0x352f);
}
RT3_REPLACE(0x544290, Render_SetProjection);

// 0x544360 / 0x5443b0. World matrix +0x34ef: identity / the object's matrix (+0xce).
void __fastcall Render_WorldIdentity(uint8_t* r) {
    float* m = (float*)(r + 0x34ef);
    for (int k = 0; k < 16; ++k) F<uint32_t>(m, k * 4) = 0;
    F<uint32_t>(r, 0x352b) = 0x3f800000;
    F<uint32_t>(r, 0x3517) = 0x3f800000;
    F<uint32_t>(r, 0x3503) = 0x3f800000;
    F<uint32_t>(m, 0) = 0x3f800000;
    SetTransform(r, 0x100, m);
}
RT3_REPLACE(0x544360, Render_WorldIdentity);

void __fastcall Render_WorldFrom(uint8_t* r, int, uint8_t* obj) {
    for (int k = 0; k < 16; ++k) F<uint32_t>(r, 0x34ef + k * 4) = F<uint32_t>(obj, 0xce + k * 4);
    SetTransform(r, 0x100, r + 0x34ef);
}
RT3_REPLACE(0x5443b0, Render_WorldFrom);

// 0x5443f0. Pushes the object's local matrix (+0x14e) on the matrix stack (+0x33ab):
// +0x10e = the stack top before, +0xce = after.
void __fastcall Render_ApplyLocal(uint8_t* r, int, uint8_t* obj) {
    void* ms = F<void*>(r, 0x33ab);
    const uint32_t* top = ((uint32_t*(__stdcall*)(void*))(*(void***)ms)[0x44 / 4])(ms);
    for (int k = 0; k < 16; ++k) F<uint32_t>(obj, 0x10e + k * 4) = top[k];
    ms = F<void*>(r, 0x33ab);
    ((int32_t(__stdcall*)(void*, void*))(*(void***)ms)[0x18 / 4])(ms, obj + 0x14e);
    ms = F<void*>(r, 0x33ab);
    top = ((uint32_t*(__stdcall*)(void*))(*(void***)ms)[0x44 / 4])(ms);
    for (int k = 0; k < 16; ++k) F<uint32_t>(obj, 0xce + k * 4) = top[k];
}
RT3_REPLACE(0x5443f0, Render_ApplyLocal);

uint8_t __fastcall Render_TakeFlag36f7(uint8_t* r, int, uint8_t clear) {
    uint8_t v = r[0x36f7];
    if (clear) r[0x36f7] = 0;
    return v;
}
RT3_REPLACE(0x544450, Render_TakeFlag36f7);

// 0x544470. Activates projection +0x356f / view +0x362f: combined matrix +0x33af
// (copied to +0x33ef), its transpose +0x35af and inverse +0x342f.
void __fastcall Render_UseView2(uint8_t* r) {
    float det, m[16];
    SetTransform(r, 3, r + 0x356f);
    SetTransform(r, 2, r + 0x362f);
    MatMul(m, r + 0x34ef, r + 0x362f);
    MatMul(r + 0x33af, m, r + 0x356f);
    MatTranspose(r + 0x35af, r + 0x33af);
    MatInverse(r + 0x342f, &det, r + 0x33af);
    for (int k = 0; k < 16; ++k) F<uint32_t>(r, 0x33ef + k * 4) = F<uint32_t>(r, 0x33af + k * 4);
}
RT3_REPLACE(0x544470, Render_UseView2);

// 0x544500. Activates projection +0x352f / view +0x35ef: combined +0x33af and its
// inverse +0x342f, plus the inverse of view x projection (+0x366f).
void __fastcall Render_UseView1(uint8_t* r) {
    float det, m[16];
    SetTransform(r, 3, r + 0x352f);
    SetTransform(r, 2, r + 0x35ef);
    MatMul(m, r + 0x34ef, r + 0x35ef);
    MatMul(r + 0x33af, m, r + 0x352f);
    MatInverse(r + 0x342f, &det, r + 0x33af);
    MatMul(m, r + 0x35ef, r + 0x352f);
    MatInverse(r + 0x366f, &det, m);
}
RT3_REPLACE(0x544500, Render_UseView1);

// 0x5445a0. Activates projection +0x352f / view +0x346f (combined, transpose, inverse).
void __fastcall Render_UseView3(uint8_t* r) {
    float det, m[16];
    SetTransform(r, 3, r + 0x352f);
    SetTransform(r, 2, r + 0x346f);
    MatMul(m, r + 0x34ef, r + 0x346f);
    MatMul(r + 0x33af, m, r + 0x352f);
    MatTranspose(r + 0x35af, r + 0x33af);
    MatInverse(r + 0x342f, &det, r + 0x33af);
}
RT3_REPLACE(0x5445a0, Render_UseView3);

void __fastcall Render_SetVec2533(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c) {
    F<uint32_t>(r, 0x2533) = a;
    F<uint32_t>(r, 0x2537) = b;
    F<uint32_t>(r, 0x253b) = c;
}
RT3_REPLACE(0x544630, Render_SetVec2533);

// 0x544660. Viewport scale/offset (+0x2523 sx, +0x252b sy, +0x2527 ox, +0x252f oy).
void __fastcall Render_GetViewport(uint8_t* r, int, uint32_t* sx, uint32_t* sy, uint32_t* ox, uint32_t* oy) {
    if (sx) *sx = F<uint32_t>(r, 0x2523);
    if (sy) *sy = F<uint32_t>(r, 0x252b);
    if (ox) *ox = F<uint32_t>(r, 0x2527);
    if (oy) *oy = F<uint32_t>(r, 0x252f);
}
RT3_REPLACE(0x544660, Render_GetViewport);

// 0x5446b0. Projects (x, y, z) with the matrix +0x33ef (x/w, y/w).
int32_t __fastcall Render_ProjectSimple(uint8_t* r, int, float x, float y, float z, float* ox, float* oy) {
    const uint32_t b = 0x33ef;
    double py = (double)x * M(r, b, 0, 1) + (double)y * M(r, b, 1, 1) + (double)z * M(r, b, 2, 1) + M(r, b, 3, 1);
    double inv = *(const double*)0x5c8770 /
                 ((double)x * M(r, b, 0, 3) + (double)y * M(r, b, 1, 3) + (double)z * M(r, b, 2, 3) + M(r, b, 3, 3));
    double px = (double)x * M(r, b, 0, 0) + (double)y * M(r, b, 1, 0) + (double)z * M(r, b, 2, 0) + M(r, b, 3, 0);
    *ox = (float)(px * inv);
    *oy = (float)(py * inv);
    return 0;
}
RT3_REPLACE(0x5446b0, Render_ProjectSimple);

// 0x544750. Projects with the combined matrix +0x33af; 1 (culled) when w < 1 or
// depth > 1, else 0 with the screen x/y (and depth when asked).
int32_t __fastcall Render_Project(uint8_t* r, int, float x, float y, float z, float* ox, float* oy, float* oz) {
    const uint32_t b = 0x33af;
    float py = (float)((double)x * M(r, b, 0, 1) + (double)y * M(r, b, 1, 1) + (double)z * M(r, b, 2, 1) + M(r, b, 3, 1));
    float pz = (float)((double)x * M(r, b, 0, 2) + (double)y * M(r, b, 1, 2) + (double)z * M(r, b, 2, 2) + M(r, b, 3, 2));
    double w = (double)x * M(r, b, 0, 3) + (double)y * M(r, b, 1, 3) + (double)z * M(r, b, 2, 3) + M(r, b, 3, 3);
    if (w < *(const float*)0x5c85b4) return 1;
    double inv = *(const double*)0x5c8770 / w;
    double d = (double)pz * inv;
    pz = (float)d;
    if (d > *(const double*)0x5c8770) return 1;
    double px = (double)x * M(r, b, 0, 0) + (double)y * M(r, b, 1, 0) + (double)z * M(r, b, 2, 0) + M(r, b, 3, 0);
    *ox = (float)(px * inv);
    *oy = (float)(inv * py);
    if (oz) *oz = pz;
    return 0;
}
RT3_REPLACE(0x544750, Render_Project);

// 0x544860. Transforms v in place by the world matrix +0x34ef (homogeneous).
void __fastcall Render_TransformWorld(uint8_t* r, int, float* v) {
    const uint32_t b = 0x34ef;
    double x = v[0], y = v[1], z = v[2];
    float py = (float)(z * M(r, b, 2, 1) + y * M(r, b, 1, 1) + x * M(r, b, 0, 1) + M(r, b, 3, 1));
    float pz = (float)(z * M(r, b, 2, 2) + y * M(r, b, 1, 2) + x * M(r, b, 0, 2) + M(r, b, 3, 2));
    float inv = (float)(*(const double*)0x5c8770 / (z * M(r, b, 2, 3) + y * M(r, b, 1, 3) + x * M(r, b, 0, 3) + M(r, b, 3, 3)));
    double px = z * M(r, b, 2, 0) + y * M(r, b, 1, 0) + x * M(r, b, 0, 0) + M(r, b, 3, 0);
    v[0] = (float)(px * inv);
    v[1] = (float)((double)inv * py);
    v[2] = (float)((double)inv * pz);
}
RT3_REPLACE(0x544860, Render_TransformWorld);

// 0x544930. Projects (x, y, z) with view x projection x viewport (+0x34ef, +0x35ef,
// +0x352f): screen position to *sx / *sy and |x| + |y| x 106 as a distance code;
// 0xff when behind the eye (w < 1) or past the far plane.
int32_t __fastcall Render_ProjectPoint(uint8_t* r, int, float x, float y, float z, float* sx, float* sy) {
    float m1[16], m[16];
    auto mul = (void*(__stdcall*)(float*, const void*, const void*))0x56d3cd;
    mul(m1, r + 0x34ef, r + 0x35ef);
    mul(m, m1, r + 0x352f);
    double w = (double)m[3] * x + (double)m[7] * y + (double)m[11] * z + m[15];
    if (w < Fl(0x5c85b4)) return 0xff;
    float inv = (float)(D(0x5c8770) / w);
    double zz = ((double)m[6] * y + (double)m[10] * z + (double)m[2] * x + m[14]) * inv;
    if (zz > D(0x5c8770)) return 0xff;
    double px = ((double)m[0] * x + (double)m[4] * y + (double)m[8] * z + m[12]) * inv;
    double py = ((double)m[1] * x + (double)m[5] * y + (double)m[9] * z + m[13]) * inv;
    *sx = (float)px;
    *sy = (float)py;
    double code = (fabs(py) + fabs(px)) * D(0x5dd988);
    int32_t r32;
    __asm {
        fld code
        mov eax, 0x5a10d0
        call eax
        mov r32, eax
    }
    return r32;
}
RT3_REPLACE(0x544930, Render_ProjectPoint);

// 0x544a80. Unprojects screen (sx, sy) at depth 0x5dd990 with the inverse matrix +0x342f.
void __fastcall Render_Unproject(uint8_t* r, int, float sx, float sy, float* ox, float* oy, float* oz) {
    const uint32_t b = 0x342f;
    const double k = *(const float*)0x5dd990;
    double u = ((double)sx - F<float>(r, 0x2527)) * F<float>(r, 0x2523);
    double v = ((double)sy - F<float>(r, 0x252f)) * F<float>(r, 0x252b);
    float py = (float)(u * M(r, b, 0, 1) + v * M(r, b, 1, 1) + M(r, b, 2, 1) * k + M(r, b, 3, 1));
    float pz = (float)(u * M(r, b, 0, 2) + v * M(r, b, 1, 2) + M(r, b, 2, 2) * k + M(r, b, 3, 2));
    float inv = (float)(*(const double*)0x5c8770 / (u * M(r, b, 0, 3) + v * M(r, b, 1, 3) + M(r, b, 2, 3) * k + M(r, b, 3, 3)));
    double px = u * M(r, b, 0, 0) + v * M(r, b, 1, 0) + M(r, b, 2, 0) * k + M(r, b, 3, 0);
    *ox = (float)(px * inv);
    *oy = (float)((double)inv * py);
    *oz = (float)((double)inv * pz);
}
RT3_REPLACE(0x544a80, Render_Unproject);

// 0x544b80. Ray direction from the camera point `eye` through screen (sx, sy).
void __fastcall Render_UnprojectDir(uint8_t* r, int, float sx, float sy, const float* eye, float* out) {
    const uint32_t b = 0x342f;
    const double k = *(const float*)0x5dd990;
    double u = ((double)sx - F<float>(r, 0x2527)) * F<float>(r, 0x2523);
    double v = ((double)sy - F<float>(r, 0x252f)) * F<float>(r, 0x252b);
    float py = (float)(v * M(r, b, 1, 1) + u * M(r, b, 0, 1) + M(r, b, 2, 1) * k + M(r, b, 3, 1));
    float pz = (float)(v * M(r, b, 1, 2) + u * M(r, b, 0, 2) + M(r, b, 2, 2) * k + M(r, b, 3, 2));
    float inv = (float)(*(const double*)0x5c8770 / (v * M(r, b, 1, 3) + u * M(r, b, 0, 3) + M(r, b, 2, 3) * k + M(r, b, 3, 3)));
    double px = v * M(r, b, 1, 0) + u * M(r, b, 0, 0) + M(r, b, 2, 0) * k + M(r, b, 3, 0);
    out[0] = (float)(px * inv - eye[0]);
    out[1] = (float)((double)inv * py - eye[1]);
    out[2] = (float)((double)inv * pz - eye[2]);
}
RT3_REPLACE(0x544b80, Render_UnprojectDir);

void __fastcall Render_AddressBorder(uint8_t* r) {
    SetTss(r, 0, 0xd, 4);
    SetTss(r, 0, 0xe, 4);
}
RT3_REPLACE(0x544c80, Render_AddressBorder);

void __fastcall Render_AddressClamp(uint8_t* r, int, uint8_t stage) {
    SetTss(r, stage, 0xd, 3);
    SetTss(r, stage, 0xe, 3);
}
RT3_REPLACE(0x544cb0, Render_AddressClamp);

void __fastcall Render_AddressMirror(uint8_t* r) {
    SetTss(r, 0, 0xd, 2);
    SetTss(r, 0, 0xe, 2);
}
RT3_REPLACE(0x544cf0, Render_AddressMirror);

void __fastcall Render_AddressWrap(uint8_t* r) {
    SetTss(r, 0, 0xd, 1);
    SetTss(r, 0, 0xe, 1);
    SetTss(r, 1, 0xd, 1);
    SetTss(r, 1, 0xe, 1);
}
RT3_REPLACE(0x544d20, Render_AddressWrap);

int32_t __fastcall Render_SetTextureFactor(uint8_t* r, int, uint32_t v) { return SetRs(r, 0x3c, v); }
RT3_REPLACE(0x544d80, Render_SetTextureFactor);

int32_t __fastcall Render_SetFactorAlpha(uint8_t* r, int, uint8_t a) {
    return SetRs(r, 0x3c, (F<uint32_t>(r, 0x25a3) & 0xffffff) | ((uint32_t)a << 24));
}
RT3_REPLACE(0x544da0, Render_SetFactorAlpha);

int32_t __fastcall Render_SetTexCoordIndex(uint8_t* r, int, uint32_t stage, uint32_t v) { return SetTss_96(r, stage, 0xb, v); }
RT3_REPLACE(0x544dd0, Render_SetTexCoordIndex);

// 0x544df0. Applies the texture filter settings (+0x83c): render states 0x8e/0x1d
// (by engine +0x11422e), blend 5/6 when +0x21, and per stage (+0x258f) the LOD bias
// (2 x +0x25) and the min filter (+0x29: 1 point, 2 linear) .
void __fastcall Render_ApplyFilterStates(uint8_t* r) {
    auto rs = [&](uint32_t s, uint32_t v) {
        void* d = F<void*>(r, 0x25df);
        ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))Vt(d)[0xc8 / 4])(d, s, v);
    };
    auto tss = [&](uint32_t stage, uint32_t type, uint32_t v) {
        void* d = F<void*>(r, 0x25df);
        ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))Vt(d)[0xfc / 4])(d, stage, type, v);
    };
    if (g_engine[0x11422e]) {
        rs(0x8e, 1);
        rs(0x1d, 0);
    } else {
        rs(0x1d, 0);
    }
    if (F<uint8_t*>(r, 0x83c)[0x21]) {
        rs(0x13, 5);
        rs(0x14, 6);
    }
    uint8_t* s = F<uint8_t*>(r, 0x83c);
    float bias = (float)((double)F<float>(s, 0x25) + F<float>(s, 0x25));
    uint32_t biasBits = *(uint32_t*)&bias;
    uint32_t filter = 0;
    int32_t mode = F<int32_t>(s, 0x29);
    if (mode == 1 || mode == 2) {
        filter = (uint32_t)mode;
        for (int32_t i = 0; i < F<int32_t>(r, 0x258f); ++i) tss(i, 0x13, biasBits);
    }
    for (int32_t i = 0; i < F<int32_t>(r, 0x258f); ++i) tss(i, 0x12, filter);
}
RT3_REPLACE(0x544df0, Render_ApplyFilterStates);

// 0x544f30. Texture filtering for all stages (+0x258f) or one: point, linear, or
// anisotropic (config +0x1e; max anisotropy 2), with dithering when filtered.
RT3_NOINLINE void __fastcall Render_SetFiltering(uint8_t* r, int, uint8_t filtered, int8_t stage) {
    int32_t end = F<int32_t>(r, 0x258f), start = 0;
    uint32_t mode = 1;
    if (stage >= 0) {
        start = stage;
        end = stage + 1;
    }
    if (filtered) {
        mode = 2;
        if (Cfg(r)[0x1e]) {
            mode = 3;
            for (int32_t n = end - start; start < end && n; --n) SetTss_96(r, 0, 0x15, 2);
        }
        SetRs_96(r, 0x1a, 1);
    }
    for (int32_t s = start; s < end; ++s) {
        SetTss_96(r, s, 0x11, mode);
        SetTss_96(r, s, 0x10, mode);
    }
}
RT3_REPLACE(0x544f30, Render_SetFiltering);

int32_t __fastcall Render_LightingOff(uint8_t* r) { return SetRs_96(r, 0x89, 0); }
RT3_REPLACE(0x545000, Render_LightingOff);

int32_t __fastcall Render_LightingDefault(uint8_t* r) { return SetRs_96(r, 0x89, Cfg(r)[0x22]); }
RT3_REPLACE(0x545020, Render_LightingDefault);

int32_t __fastcall Render_LightingAndSun(uint8_t* r) {
    SetRs_96(r, 0x89, Cfg(r)[0x22]);
    void* d = Dev(r);
    return ((int32_t(__stdcall*)(void*, uint32_t, int32_t))(*(void***)d)[0xb8 / 4])(d, 0, Cfg(r)[0x22]);
}
RT3_REPLACE(0x545040, Render_LightingAndSun);

int32_t __fastcall Render_ZWriteOff(uint8_t* r) { return SetRs_96(r, 0xe, 0); }
RT3_REPLACE(0x545080, Render_ZWriteOff);

int32_t __fastcall Render_ZWriteOn(uint8_t* r) { return SetRs_96(r, 0xe, 1); }
RT3_REPLACE(0x5450a0, Render_ZWriteOn);

void __fastcall Render_GetFogBlock(uint8_t* r, int, uint8_t* dst) {
    for (int k = 0; k < 0x33; ++k) dst[k] = r[0x24f0 + k];
}
RT3_REPLACE(0x5450c0, Render_GetFogBlock);

// 0x5450e0 / 0x545130. Sun direction (negated +0x2517/+0x251b/+0x251f; z scaled
// by 0x5c8758, height at least 0.5).
void __fastcall Render_SunDir(uint8_t* r, int, float* x, float* y, float* z) {
    *x = -F<float>(r, 0x2517);
    *z = (float)(-(double)F<float>(r, 0x251f) * *(const double*)0x5c8758);
    double h = -(double)F<float>(r, 0x251b);
    *y = (float)h;
    if (h < *(const double*)0x5c8628) *(uint32_t*)y = 0x3f000000;
}
RT3_REPLACE(0x5450e0, Render_SunDir);

void __fastcall Render_SunDirVec(uint8_t* r, int, float* v) {
    v[0] = -F<float>(r, 0x2517);
    v[2] = (float)(-(double)F<float>(r, 0x251f) * *(const double*)0x5c8758);
    double h = -(double)F<float>(r, 0x251b);
    v[1] = (float)h;
    if (h < *(const double*)0x5c8628) *(uint32_t*)&v[1] = 0x3f000000;
}
RT3_REPLACE(0x545130, Render_SunDirVec);

// 0x545180. Builds the 144 sun positions over the day (+0x86b.., 0x33 bytes each,
// every 600 units from -43200): the light direction (-4/0/+4), a tilt (-0x2b..-0x1f),
// the sun colour bytes (-0xb..-9, warm at dawn and dusk, dim at night), the light
// colour floats (-0x17..-0xf), a strength (-0x1b) and the fog falloff (-8).
RT3_NOINLINE void __fastcall Sky_BuildSunTable(uint8_t* o) {
    uint8_t* p = o + 0x86b;
    for (int32_t i = 0, deg = -0xa8c0; i < 0x90; ++i, deg += 0x258, p += 0x33) {
        double a = (double)deg * Fq(0x5dd9e0);
        float c = (float)Fcos(a);
        double s2 = Fsin(a) * 2.0;
        float s2f = (float)s2;
        double inv = 1.0 / sqrt(s2 * s2f + (double)c * c + Fq(0x5c85b4));
        F<float>(p, -4) = (float)((double)s2f * inv);
        double ci = (double)c * inv;
        F<float>(p, 0) = (float)ci;
        F<float>(p, 4) = (float)inv;
        double t = -ci;
        float tilt = (float)(Fcos(t) * Dq(0x5cecd0));
        F<float>(p, -0x1f) = tilt;
        F<float>(p, -0x23) = tilt;
        float lift = (float)(Fsin(t) * Dq(0x5c8c18) + Dq(0x5c8da8));
        F<float>(p, -0x27) = lift;
        F<float>(p, -0x2b) = lift;
        int32_t r, g, b;
        if (c > Dq(0x5c8680)) {
            r = 0xcd;
            g = 0xd0;
            b = 0xde;
        } else if (c > Fq(0x5c8598)) {
            double q = Dq(0x5c8680) - c;
            double k = Dq(0x5c88a8) * q;
            r = Ftol(Dq(0x5dd9d8) + k);
            g = Ftol(Dq(0x5dd9d0) - k);
            b = Ftol(Dq(0x5dd9c0) - q * Dq(0x5dd9c8));
        } else {
            r = Ftol((double)c * Fq(0x5dd9b8) + Fq(0x5dd9b4));
            g = Ftol((double)c * Fq(0x5c8904) + Fq(0x5dd9b0));
            b = Ftol((double)c * Fq(0x5dd9ac) + Fq(0x5dd9a8));
            if (r < 2) r = 2;
            if (g < 2) g = 2;
            if (b < 0xf) b = 0xf;
        }
        p[-0xb] = (uint8_t)r;
        p[-0xa] = (uint8_t)g;
        p[-9] = (uint8_t)b;
        if (c > Dq(0x5c8c20)) F<uint32_t>(p, -0x1b) = 0x3f170a3d;
        else {
            double v = (double)c + Dq(0x5dd9a0);
            F<float>(p, -0x1b) = (float)v;
            if (v < Fq(0x5c8598)) F<float>(p, -0x1b) = 0.0f;
        }
        F<float>(p, -0x17) = (float)((double)(uint8_t)r * Fq(0x5dd17c) * Dq(0x5c8578) + Dq(0x5c8c20));
        F<float>(p, -0x13) = (float)((double)(uint8_t)g * Fq(0x5dd17c) * Dq(0x5c87e8) + Dq(0x5c88e8));
        F<float>(p, -0xf) = (float)((double)(uint8_t)b * Fq(0x5dd17c) * Dq(0x5c87e8) + Dq(0x5c88c8));
        double v = F<float>(p, -0x1b);
        if (v < Dq(0x5c8628)) v = Fq(0x5c8d48);
        F<float>(p, -8) = (float)((Dq(0x5c8770) - v * v * v) / ((v + Dq(0x5c86b8)) * Dq(0x5dd998)));
    }
}
RT3_REPLACE(0x545180, Sky_BuildSunTable);

// Light records (0x33 bytes at +0x840) and the two-value table +0x25f7/+0x25ff.
void __fastcall Render_SetLightRange(uint8_t* r, int, int32_t i, float v) { F<float>(r, 0x863 + i * 0x33) = v; }
RT3_REPLACE(0x545400, Render_SetLightRange);

void __fastcall Render_SetPair25f7(uint8_t* r, int, int32_t i, float a, float b) {
    F<float>(r, 0x25f7 + i * 4) = a;
    F<float>(r, 0x25ff + i * 4) = b;
}
RT3_REPLACE(0x545420, Render_SetPair25f7);

void __fastcall Render_GetPair25f7(uint8_t* r, int, int32_t i, uint32_t* a, uint32_t* b) {
    if (a) *a = F<uint32_t>(r, 0x25f7 + i * 4);
    if (b) *b = F<uint32_t>(r, 0x25ff + i * 4);
}
RT3_REPLACE(0x545440, Render_GetPair25f7);

void __fastcall Render_SetLightPair(uint8_t* r, int, int32_t i, int32_t j, float a, float b) {
    F<float>(r, 0x840 + i * 0x33 + j * 4) = a;
    F<float>(r, 0x848 + i * 0x33 + j * 4) = b;
}
RT3_REPLACE(0x545470, Render_SetLightPair);

float __fastcall Render_GetLightValue(uint8_t* r, int, int32_t i, int32_t j) { return F<float>(r + i * 0x33, 0x840 + j * 4); }
RT3_REPLACE(0x5454a0, Render_GetLightValue);

// 0x5454c0. Pushes a copy of the texture stage state cache (+0x2597, 24 bytes per
// stage, +0x258f stages) onto the list +0x259f.
void* __fastcall Render_PushStageState(uint8_t* r) {
    uint32_t n = (uint32_t)F<int32_t>(r, 0x258f) * 24;
    uint8_t* p = (uint8_t*)Malloc(n);
    const uint8_t* src = F<uint8_t*>(r, 0x2597);
    n = (uint32_t)F<int32_t>(r, 0x258f) * 24;
    for (uint32_t k = 0; k < n; ++k) p[k] = src[k];
    PtrList_Append(F<PtrList*>(r, 0x259f), 0, p);
    return p;
}
RT3_REPLACE(0x5454c0, Render_PushStageState);

// 0x545520. Binds texture tex (null = none) to stage `stage` of renderer
// r's device (+0x25df, SetTexture), uploading it first when it is not
// current this frame; skipped when it is already bound. Counts the binds
// (+0x370d).
RT3_NOINLINE void __fastcall Renderer_SetTexture(uint8_t* r, int, int32_t stage, uint8_t* tex) {
    if (stage >= F<int32_t>(r, 0x258f)) return;
    uint8_t* cur = F<uint8_t**>(r, 0x2593)[stage];
    if (tex) {
        if (F<uint32_t>(tex, 7) != F<uint32_t>(g_engine, 0x114222))
            Texture_Upload(tex, 0, F<void*>(r, 0x25df), 0);
        else if (tex == cur) {
            F<uint8_t**>(r, 0x2593)[stage] = tex;
            return;
        }
        void** vt = *(void***)F<void*>(r, 0x25df);
        void* h = Texture_TargetForFrame(tex);
        ((int32_t(__stdcall*)(void*, uint32_t, void*))vt[0x3d])(F<void*>(r, 0x25df), (uint32_t)stage, h);
        ++F<int32_t>(r, 0x370d);
        F<uint8_t**>(r, 0x2593)[stage] = tex;
        return;
    }
    if (cur) {
        ++F<int32_t>(r, 0x370d);
        void* d = F<void*>(r, 0x25df);
        ((int32_t(__stdcall*)(void*, uint32_t, void*))(*(void***)d)[0x3d])(d, (uint32_t)stage, nullptr);
    }
    F<uint8_t**>(r, 0x2593)[stage] = nullptr;
}
RT3_REPLACE(0x545520, Renderer_SetTexture);

RT3_NOINLINE void __fastcall Render_SetColorOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply) {
    SetStage3(r, stage, 0, 1, arg1, op, arg2, apply);
}
RT3_REPLACE(0x5455e0, Render_SetColorOp);

RT3_NOINLINE void __fastcall Render_SetAlphaOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply) {
    SetStage3(r, stage, 0xc, 4, arg1, op, arg2, apply);
}
RT3_REPLACE(0x5456d0, Render_SetAlphaOp);

// 0x5457c0. Default material (diffuse 1, ambient 0.1; specular 0.3 with power
// +0x25fb x +0x24f4 x 0x5c8568 when shiny and supported) and ambient light 0xffafafaf.
void __fastcall Render_SetDefaultMaterial(uint8_t* r, int, uint8_t shiny) {
    uint32_t m[17] = {};
    if (shiny && g_engine[0x11422e]) {
        for (int k = 8; k < 12; ++k) m[k] = 0x3e99999a;
        float p = (float)((double)F<float>(r, 0x25fb) * F<float>(r, 0x24f4) * *(const float*)0x5c8568);
        m[16] = *(uint32_t*)&p;
    }
    for (int k = 0; k < 4; ++k) m[k] = 0x3f800000;
    for (int k = 4; k < 8; ++k) m[k] = 0x3dcccccd;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))(*(void***)d)[0xc8 / 4])(d, 0x8b, 0xffafafaf);
    d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*))(*(void***)d)[0xa8 / 4])(d, m);
}
RT3_REPLACE(0x5457c0, Render_SetDefaultMaterial);

// 0x5458a0 / 0x5458d0. Locks / unlocks the whole vertex buffer +0x265b (lock count 0xccd824).
void* __fastcall Render_LockVb2(uint8_t* r) {
    void* p;
    g_ccd824++;
    VbLock(F<void*>(r, 0x265b), 0, 0, &p, 0x2800);
    return p;
}
RT3_REPLACE(0x5458a0, Render_LockVb2);

int32_t __fastcall Render_UnlockVb2(uint8_t* r) {
    g_ccd824--;
    void* vb = F<void*>(r, 0x265b);
    return ((int32_t(__stdcall*)(void*))(*(void***)vb)[0x30 / 4])(vb);
}
RT3_REPLACE(0x5458d0, Render_UnlockVb2);

// 0x5458f0. Locks vertices first..end (32 bytes each; to 0xffff when end is 0) of
// the dynamic buffer +0x2643 (once, flag +0x254e) without overwriting; returns +0x2657.
RT3_NOINLINE void* __fastcall Render_LockVbRange(uint8_t* r, int, int32_t first, int32_t end) {
    if (r[0x254e]) return nullptr;
    uint8_t desc[0x18];
    void* vb = F<void*>(r, 0x2643);
    r[0x254e] = 1;
    ((int32_t(__stdcall*)(void*, void*))(*(void***)vb)[0x34 / 4])(vb, desc);
    g_ccd824++;
    uint32_t size = end ? (uint32_t)(end << 5) - (uint32_t)(first << 5) : (uint32_t)(0xffff - first) << 5;
    VbLock(F<void*>(r, 0x2643), (uint32_t)first << 5, size, r + 0x2657, 0x1800);
    return F<void*>(r, 0x2657);
}
RT3_REPLACE(0x5458f0, Render_LockVbRange);

// 0x5459a0. Locks the first n vertices of the dynamic buffer with discard.
void* __fastcall Render_LockVbDiscard(uint8_t* r, int, int32_t n) {
    if (r[0x254e]) return nullptr;
    r[0x254e] = 1;
    g_ccd824++;
    VbLock(F<void*>(r, 0x2643), 0, (uint32_t)n << 5, r + 0x2657, 0x2800);
    return F<void*>(r, 0x2657);
}
RT3_REPLACE(0x5459a0, Render_LockVbDiscard);

// 0x5459f0. Unlocks the dynamic vertex buffer (+0x2643) if locked.
void __fastcall Render_UnlockVbRange(uint8_t* r) {
    if (!r[0x254e]) return;
    r[0x254e] = 0;
    LockCount()--;
    Unlock(F<void*>(r, 0x2643));
}
RT3_REPLACE(0x5459f0, Render_UnlockVbRange);

// Fixed buffers: +0x264f (0x7800 bytes), +0x2653 / +0x264b (0x3e80), +0x263f (0x2ee00).
int32_t __fastcall Render_UnlockVb264f(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x264f));
}
RT3_REPLACE(0x545a20, Render_UnlockVb264f);

void* __fastcall Render_LockVb264f(uint8_t* r) {
    LockCount()++;
    Lock(F<void*>(r, 0x264f), 0, 0x7800, r + 0x2657, 0x2800);
    return F<void*>(r, 0x2657);
}
RT3_REPLACE(0x545a40, Render_LockVb264f);

void* __fastcall Render_LockVb2653(uint8_t* r) {
    void* p;
    LockCount()++;
    Lock(F<void*>(r, 0x2653), 0, 0x3e80, &p, 0x2800);
    return p;
}
RT3_REPLACE(0x545a70, Render_LockVb2653);

int32_t __fastcall Render_UnlockVb2653(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x2653));
}
RT3_REPLACE(0x545aa0, Render_UnlockVb2653);

void* __fastcall Render_LockVb264b(uint8_t* r) {
    void* p;
    LockCount()++;
    Lock(F<void*>(r, 0x264b), 0, 0x3e80, &p, 0x2800);
    return p;
}
RT3_REPLACE(0x545ac0, Render_LockVb264b);

int32_t __fastcall Render_UnlockVb264b(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x264b));
}
RT3_REPLACE(0x545af0, Render_UnlockVb264b);

// 0x545b10 / 0x545b50. Locks records first..first+count (24 bytes) of buffer +0x2647
// (discard / no-overwrite).
void* __fastcall Render_LockVb2647(uint8_t* r, int, int32_t first, int32_t count) {
    void* p;
    Lock(F<void*>(r, 0x2647), first * 24, count * 24, &p, 0x2800);
    LockCount()++;
    return p;
}
RT3_REPLACE(0x545b10, Render_LockVb2647);

void* __fastcall Render_AppendVb2647(uint8_t* r, int, int32_t first, int32_t count) {
    Lock(F<void*>(r, 0x2647), first * 24, count * 24, r + 0x2657, 0x1800);
    LockCount()++;
    return F<void*>(r, 0x2657);
}
RT3_REPLACE(0x545b50, Render_AppendVb2647);

int32_t __fastcall Render_UnlockVb2647(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x2647));
}
RT3_REPLACE(0x545b90, Render_UnlockVb2647);

void* __fastcall Render_LockVb263f(uint8_t* r) {
    void* p;
    LockCount()++;
    Lock(F<void*>(r, 0x263f), 0, 0x2ee00, &p, 0x800);
    return p;
}
RT3_REPLACE(0x545bb0, Render_LockVb263f);

int32_t __fastcall Render_UnlockVb263f(uint8_t* r) {
    LockCount()--;
    return Unlock(F<void*>(r, 0x263f));
}
RT3_REPLACE(0x545be0, Render_UnlockVb263f);

// 0x545c00. Indexed triangle list of `indices` indices (DrawIndexedPrimitive),
// counting triangles in +0x3711.
void __fastcall Render_DrawIndexed(uint8_t* r, int, int32_t indices, uint32_t minIndex, uint32_t vertices) {
    int32_t tri = indices / 3;
    if (tri < 1) return;
    F<int32_t>(r, 0x3711) += tri;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))(*(void***)d)[0x11c / 4])(
        d, 4, minIndex, vertices, 0, tri);
}
RT3_REPLACE(0x545c00, Render_DrawIndexed);

// 0x545c50. Triangle list of vertices first..end.
void __fastcall Render_DrawTriangles(uint8_t* r, int, int32_t first, int32_t end) {
    int32_t tri = (end - first) / 3;
    if (tri < 1) return;
    F<int32_t>(r, 0x3711) += tri;
    DrawPrimitive(Dev(r), 4, first, tri);
}
RT3_REPLACE(0x545c50, Render_DrawTriangles);

// 0x545ca0. Untextured point list of vertices first..end (point size 1).
int32_t __fastcall Render_DrawPoints(uint8_t* r, int, int32_t first, int32_t end) {
    if (!r[0x254d] && F<int32_t>(r, 0x258f) > 0) {
        if (*F<int32_t*>(r, 0x2593)) {
            F<int32_t>(r, 0x370d)++;
            void* d = Dev(r);
            ((int32_t(__stdcall*)(void*, uint32_t, void*))(*(void***)d)[0xf4 / 4])(d, 0, nullptr);
        }
        *F<int32_t*>(r, 0x2593) = 0;
    }
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, uint32_t))(*(void***)d)[0xc8 / 4])(d, 0x9e, 0x3f800000);
    int32_t n = end - first;
    F<int32_t>(r, 0x3711) += n / 3;
    return DrawPrimitive(Dev(r), 1, first, n);
}
RT3_REPLACE(0x545ca0, Render_DrawPoints);

RT3_NOINLINE int32_t __fastcall Render_TriangleCount(uint8_t* r) { return F<int32_t>(r, 0x3711); }
RT3_REPLACE(0x545d50, Render_TriangleCount);

// 0x545d60. Starts a frame: resets the counters (+0x370d/+0x3711), clears z when asked.
int32_t __fastcall Render_BeginFrame(uint8_t* r, int, int32_t clear) {
    F<int32_t>(r, 0x370d) = 0;
    F<int32_t>(r, 0x3711) = 0;
    if (clear) Clear(Dev(r), 2);
    return Call0(Dev(r), 0x88);
}
RT3_REPLACE(0x545d60, Render_BeginFrame);

// 0x545db0. Ends the frame, presenting unless asked not to.
void __fastcall Render_EndFrame(uint8_t* r, int, int32_t noPresent) {
    Call0(Dev(r), 0x8c);
    if (!noPresent) Present(Dev(r));
}
RT3_REPLACE(0x545db0, Render_EndFrame);

void __fastcall Nop545df0(void*, int, int32_t, int32_t) { __asm nop }
RT3_REPLACE(0x545df0, Nop545df0);

// 0x545e00. Device state: 0 ok, 1 lost, 2 needs reset, 3 other error.
uint8_t __fastcall Render_DeviceState(uint8_t* r) {
    uint32_t hr = (uint32_t)Call0(Dev(r), 0xc);
    if (hr == 0x88760868) return 1;
    if (hr == 0x88760869) return 2;
    return hr == 0 ? 0 : 3;
}
RT3_REPLACE(0x545e00, Render_DeviceState);

// 0x545e30. The sound card report at +0x43c (1 KB): DirectSoundCreate8 on the
// default device, priority cooperative level on the game window, then GetCaps;
// one line per capability flag (continuous rate, emulated, certified, 8/16 bit,
// mono/stereo primary, secondary buffer formats; strings 0x74..0x7b and some
// literals), the secondary sample-rate range and the hardware mixing / 3D buffer
// counts (0x7c..0x7e). Each line is formatted with the text so far as %1.
// Returns 1 (with string 0x73) when DirectSound is unavailable.
RT3_NOINLINE int32_t __fastcall Sound_BuildCapsReport(uint8_t* o) {
    struct Dsound { void** vtbl; };
    Dsound* ds;
    char* text = (char*)(o + 0x43c);
    if (((int32_t(__stdcall*)(void*, Dsound**, void*))0x58bc36)(nullptr, &ds, nullptr) >= 0) {
        HWND wnd = Engine_Window(hook::As<void*>(g_engine));
        if (((int32_t(__stdcall*)(Dsound*, HWND, uint32_t))ds->vtbl[6])(ds, wnd, 2) >= 0) {
            uint32_t caps[0x18] = {};
            caps[0] = 0x60;
            ((int32_t(__stdcall*)(Dsound*, uint32_t*))ds->vtbl[4])(ds, caps);
            memset(text, 0, 0x400);
            char line[0x400];
            line[0] = 0;
            auto add = [&](const char* fmt, const char* a2, const char* a3) {
                FormatText(line, fmt, text, a2, a3, 0, 0, 0, 0, 0);
                strcpy(text, line);
            };
            uint32_t flags = caps[1];
            if (flags & 0x40) add(GetString(0x74), 0, 0);
            if (flags & 0x20) add(GetString(0x75), 0, 0);
            add(GetString(0x76), 0, 0);
            if (flags & 0x8) add((const char*)0x5dda10, 0, 0);
            if (flags & 0x4) add((const char*)0x5dda04, 0, 0);
            if (flags & 0x1) add(GetString(0x77), 0, 0);
            if (flags & 0x2) add(GetString(0x78), 0, 0);
            if (flags & 0xf00) {
                add(GetString(0x79), 0, 0);
                if (flags & 0x400) add((const char*)0x5dd9f4, 0, 0);
                if (flags & 0x800) add((const char*)0x5dd9e4, 0, 0);
                if (flags & 0x100) add(GetString(0x7a), 0, 0);
                if (flags & 0x200) add(GetString(0x7b), 0, 0);
            }
            auto u = [](uint32_t v) {
                double d = (double)(int32_t)v;
                if ((int32_t)v < 0) d += D(0x5c8910);
                return d;
            };
            char n1[0x100], n2[0x100];
            const char* maxRate = Num_Format(n1, 8, u(caps[3]), 10, 1, 0, 0);
            const char* minRate = Num_Format(n2, 8, u(caps[2]), 10, 1, 0, 0);
            add(GetString(0x7c), minRate, maxRate);
            add(GetString(0x7d), Num_Format(n2, 8, u(caps[5]), 10, 1, 0, 0), 0);
            add(GetString(0x7e), Num_Format(n2, 8, u(caps[0xb]), 10, 1, 0, 0), 0);
            ((uint32_t(__stdcall*)(Dsound*))ds->vtbl[2])(ds);
            return 0;
        }
        ((uint32_t(__stdcall*)(Dsound*))ds->vtbl[2])(ds);
        ds = nullptr;
    }
    FormatText(text, GetString(0x73), 0, 0, 0, 0, 0, 0, 0, 0);
    return 1;
}
RT3_REPLACE(0x545e30, Sound_BuildCapsReport);

// 0x546390. Creates the HAL device (+0x25df) on the engine window.
RT3_NOINLINE uint8_t __fastcall Render_CreateDevice(uint8_t* r, int, uint32_t behaviour, void* params) {
    void* d3d = F<void*>(r, 0x25db);
    ::HWND__* wnd = Engine_Window(hook::As<void*>(g_engine));
    int32_t hr = ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, ::HWND__*, uint32_t, void*, void*))Vt(d3d)[0x3c / 4])(
        F<void*>(r, 0x25db), 0, 1, wnd, behaviour, params, r + 0x25df);
    return hr >= 0 ? 1 : 0;
}
RT3_REPLACE(0x546390, Render_CreateDevice);

// 0x5463e0. Releases the renderer's 5 vertex/index buffers.
RT3_NOINLINE void __fastcall Render_ReleaseBuffers(uint8_t* r) {
    static const uint16_t offs[] = { 0x2627, 0x260f, 0x262f, 0x261f, 0x2617 };
    for (uint16_t off : offs)
        if (F<void*>(r, off)) ReleaseChecked(r, off, true);
}
RT3_REPLACE(0x5463e0, Render_ReleaseBuffers);

// 0x5464d0. Clears and presents four blank frames.
RT3_NOINLINE void __fastcall Render_BlankFrames(uint8_t* r) {
    for (int k = 0; k < 4; ++k) {
        Call0(Dev(r), 0x88);
        Clear(Dev(r), 3);
        Call0(Dev(r), 0x8c);
        Present(Dev(r));
    }
}
RT3_REPLACE(0x5464d0, Render_BlankFrames);

int32_t __fastcall Render_ZFuncAlways(uint8_t* r) { return SetRs_99(r, 0x17, 7); }
RT3_REPLACE(0x546540, Render_ZFuncAlways);

int32_t __fastcall Render_ZFuncLessEqual(uint8_t* r) { return SetRs_99(r, 0x17, 4); }
RT3_REPLACE(0x546560, Render_ZFuncLessEqual);

int32_t __fastcall Render_ColorWriteOff(uint8_t* r) { return SetRs_99(r, 0xa8, 0); }
RT3_REPLACE(0x546580, Render_ColorWriteOff);

int32_t __fastcall Render_ColorWriteOn(uint8_t* r) { return SetRs_99(r, 0xa8, 0xf); }
RT3_REPLACE(0x5465a0, Render_ColorWriteOn);

// 0x5465c0. Releases the 64 texture slots (+0x31ab, cleared) and the 9 fixed
// textures (left set).
RT3_NOINLINE void __fastcall Render_ReleaseTextures(uint8_t* r) {
    for (int k = 0; k < 0x40; ++k) ReleaseChecked(r, 0x31ab + k * 4, true);
    static const uint16_t offs[] = { 0x265b, 0x2663, 0x265f, 0x263f, 0x2643, 0x264f, 0x264b, 0x2653, 0x2647 };
    for (uint16_t off : offs) ReleaseChecked(r, off, false);
}
RT3_REPLACE(0x5465c0, Render_ReleaseTextures);

int32_t __fastcall Render_SetZEnable(uint8_t* r, int, uint32_t v) { return SetRs_99(r, 7, v); }
RT3_REPLACE(0x546760, Render_SetZEnable);

int32_t __fastcall Render_SetShadeMode(uint8_t* r, int, uint32_t v) { return SetRs_99(r, 9, v + 1); }
RT3_REPLACE(0x546780, Render_SetShadeMode);

// 0x5467a0 / 0x5467e0. Fog on (when configured, +0x2e) / off, cached in +0x2549.
RT3_NOINLINE void __fastcall Render_FogOn(uint8_t* r) {
    if (r[0x2549] || !Cfg(r)[0x2e]) return;
    r[0x2549] = 1;
    SetRs_99(r, 0x1c, 1);
}
RT3_REPLACE(0x5467a0, Render_FogOn);

void __fastcall Render_FogOff(uint8_t* r) {
    if (!r[0x2549]) return;
    r[0x2549] = 0;
    SetRs_99(r, 0x1c, 0);
}
RT3_REPLACE(0x5467e0, Render_FogOff);

// 0x546810 / 0x546850 / 0x546890. Cull mode cache +0x254a: CCW (2, when culling
// is configured, +0x1c), CW (1), none (0).
void __fastcall Render_CullCcw(uint8_t* r) {
    if (r[0x254a] == 2 || !Cfg(r)[0x1c]) return;
    SetRs_99(r, 0x16, 3);
    r[0x254a] = 2;
}
RT3_REPLACE(0x546810, Render_CullCcw);

void __fastcall Render_CullCw(uint8_t* r) {
    if (r[0x254a] == 1 || !Cfg(r)[0x1c]) return;
    SetRs_99(r, 0x16, 2);
    r[0x254a] = 1;
}
RT3_REPLACE(0x546850, Render_CullCw);

void __fastcall Render_CullNone(uint8_t* r) {
    if (!r[0x254a]) return;
    SetRs_99(r, 0x16, 1);
    r[0x254a] = 0;
}
RT3_REPLACE(0x546890, Render_CullNone);

// 0x5468c0. Projects (x, y, z) with the combined matrix (+0x33af, 4x4); 1 with
// out = (x, y, z) when in front (w >= 0.0001), z <= maxZ and |x|, |y| <= limit.
RT3_NOINLINE int32_t __fastcall Render_ProjectClip(uint8_t* r, int, float x, float y, float z, float limit, float maxZ, float* out) {
    const float* m = (const float*)(r + 0x33af);
    double w = (double)x * m[3] + (double)y * m[7] + (double)z * m[11] + m[15];
    if (w < D(0x5c88a0)) return 0;
    float inv = (float)(Fl(0x5c85b4) / w);
    double vz = ((double)x * m[2] + (double)y * m[6] + (double)z * m[10] + m[14]) * inv;
    float zf = (float)vz;
    if (vz > maxZ) return 0;
    double vx = ((double)x * m[0] + (double)y * m[4] + (double)z * m[8] + m[12]) * inv;
    float xf = (float)vx;
    if (limit < fabs(vx)) return 0;
    double vy = ((double)x * m[1] + (double)y * m[5] + (double)z * m[9] + m[13]) * inv;
    if (limit < fabs(vy)) return 0;
    if (out) {
        out[1] = (float)vy;
        out[0] = xf;
        out[2] = zf;
    }
    return 1;
}
RT3_REPLACE(0x5468c0, Render_ProjectClip);

// 0x546a00. Clears the 64 cached texture slots (+0x266b, 0x2d bytes).
void __fastcall Render_ResetTextureSlots(uint8_t* r) {
    F<int32_t>(r, 0x2667) = 0;
    for (int k = 0; k < 0x40; ++k) {
        uint8_t* s = r + 0x266b + k * 0x2d;
        F<int32_t>(s, 4) = 0;
        F<int32_t>(s, 0) = 0;
        F<int32_t>(s, 0x18) = 0;
        F<int32_t>(s, 0x14) = 0;
        F<int32_t>(s, 0x25) = 0;
        s[0x20] = 0;
    }
}
RT3_REPLACE(0x546a00, Render_ResetTextureSlots);

// 0x546a30. Unlocks all 64 vertex buffers (+0x31ab, locked flags in the slots;
// count 0xccd824) and relinks the slots (+0x266b, 0x2d bytes, count +0x2667): +0x14
// chains slots with the same key 1, +0x18 those with the same key 2, +0x1c = next.
void __fastcall Render_ResetVertexSlots(uint8_t* r) {
    for (int32_t k = 0; k < 0x40; ++k) {
        uint8_t* s = Slot(r, k);
        if (s[0x20]) {
            --g_ccd824;
            VbUnlock(F<void*>(r, 0x31ab + k * 4));
        }
        s[0x20] = 0;
    }
    int32_t i = 0;
    for (; i < F<int32_t>(r, 0x2667); ++i) {
        uint8_t* s = Slot(r, i);
        for (int pass = 0; pass < 2; ++pass) {
            uint32_t link = pass ? 0x18 : 0x14, key = pass ? 4 : 0;
            if (F<int32_t>(s, link)) continue;
            int32_t want = F<int32_t>(s, key);
            F<int32_t>(s, link) = -1;
            uint8_t* prev = s;
            for (int32_t j = i + 1; j < F<int32_t>(r, 0x2667); ++j) {
                uint8_t* t = Slot(r, j);
                if (want != F<int32_t>(t, key)) continue;
                F<int32_t>(prev, link) = j;
                F<int32_t>(t, link) = -1;
                prev = t;
            }
        }
        F<int32_t>(s, 0x1c) = i + 1;
    }
    if (F<int32_t>(r, 0x2667) > 0) F<int32_t>(Slot(r, i - 1), 0x1c) = -1;
}
RT3_REPLACE(0x546a30, Render_ResetVertexSlots);

// 0x546b80. The vertex slot for (key1, key2): an existing one with room (+0x21 > 3)
// is returned; otherwise (or new) slot n = +0x2667 (< 64) is locked (data +0x32ab,
// also +0x29), set up with 0x5208 free and chained from the old one. 0 when full.
uint8_t* __fastcall Render_AcquireVertexSlot(uint8_t* r, int, int32_t key1, int32_t key2) {
    int32_t n = F<int32_t>(r, 0x2667);
    if (n >= 0x40) return nullptr;
    for (int32_t i = 0; i < n; ++i) {
        uint8_t* s = Slot(r, i);
        if (key1 != F<int32_t>(s, 0) || key2 != F<int32_t>(s, 4)) continue;
        int32_t* tail = &F<int32_t>(s, 0xc);
        int32_t t = *tail;
        uint8_t* ts = Slot(r, t);
        if (F<int32_t>(ts, 0x21) > 3) return ts;
        if (ts[0x20]) {
            --g_ccd824;
            VbUnlock(F<void*>(r, 0x31ab + t * 4));
            ts[0x20] = 0;
        }
        F<int32_t>(ts, 0x10) = F<int32_t>(r, 0x2667);
        *tail = F<int32_t>(r, 0x2667);
        n = F<int32_t>(r, 0x2667);
        break;
    }
    uint8_t* s = Slot(r, n);
    if (!s[0x20]) {
        ++g_ccd824;
        int32_t m = F<int32_t>(r, 0x2667);
        void* vb = F<void*>(r, 0x31ab + m * 4);
        ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t))Vt(vb)[0x2c / 4])(vb, 0, 0, r + 0x32ab + m * 4,
                                                                                            0x2800);
        Slot(r, F<int32_t>(r, 0x2667))[0x20] = 1;
    }
    int32_t m = F<int32_t>(r, 0x2667);
    s = Slot(r, m);
    F<int32_t>(s, 0) = key1;
    F<int32_t>(s, 4) = key2;
    F<int32_t>(s, 0xc) = m;
    F<int32_t>(s, 0x10) = -1;
    F<int32_t>(s, 0x21) = 0x5208;
    F<uint32_t>(s, 0x29) = F<uint32_t>(r, 0x32ab + m * 4);
    F<uint16_t>(s, 8) = 0xffff;
    F<uint16_t>(s, 0xa) = 0;
    F<int32_t>(r, 0x2667) = m + 1;
    return s;
}
RT3_REPLACE(0x546b80, Render_AcquireVertexSlot);

// 0x546d40. Uploads the per-frame shader constants: vertex registers 0-3 (the
// matrix at +0x35af), 4/5 (0.5 scale-bias), 6 (0.95, 100, 0.1), 7 (the negated
// light direction +0x2517..+0x251f with the ambient term), 8 (sun colour x
// intensity), 9, 10 (detail scale +0x36fd), 11 (ambient), 12 (texture scale over the
// map size), 13 (lit sun colour), 14 (fog: -density x start, when table fog), and the
// pixel registers 0-3.
RT3_NOINLINE void __fastcall Renderer_SetShaderConstants(uint8_t* r) {
    auto dev = [&]() { return F<uint8_t*>(r, 0x25df); };
    auto vs = [&](uint32_t reg, const void* data, uint32_t n) {
        uint8_t* d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t, const void*, uint32_t))(*(uint8_t**)d + 0x13c))(d, reg, data, n);
    };
    auto ps = [&](uint32_t reg, const void* data) {
        uint8_t* d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t, const void*, uint32_t))(*(uint8_t**)d + 0x16c))(d, reg, data, 1);
    };
    double a = (double)F<float>(r, 0x25ff) * F<float>(r, 0x24f8) * *(const float*)0x5c8d60;
    double b = (double)F<float>(r, 0x25f7) * F<float>(r, 0x24f0) * *(const double*)0x5c8c18;
    uint32_t c4[4] = { 0x3f000000, 0x3f000000, 0, 0 };
    uint32_t c6[4] = { 0x3f733333, 0x42c80000, 0x3dcccccd, 0 };
    uint32_t c7[4] = { Bits(-F<float>(r, 0x2517)), F<uint32_t>(r, 0x251b), Bits(-F<float>(r, 0x251f)), Bits((float)b) };
    uint32_t c8[4] = { Bits((float)a), Bits((float)a), Bits((float)a), 0x3f800000 };
    uint32_t c9[4] = { 0x3f800000, 0x3f800000, 0, 0x3f800000 };
    uint32_t c10[4] = { F<uint32_t>(r, 0x36fd), F<uint32_t>(r, 0x36fd), 0, 0x3f800000 };
    uint32_t c11[4] = { Bits((float)b), Bits((float)b), Bits((float)b), 0 };
    uint32_t c13[4] = { Bits((float)-((double)F<float>(r, 0x2504) * F<float>(r, 0x2517) * a)),
                        Bits((float)((double)F<float>(r, 0x2508) * F<float>(r, 0x251b) * a)),
                        Bits((float)-((double)F<float>(r, 0x250c) * F<float>(r, 0x251f) * a)), 0 };
    vs(0, r + 0x35af, 4);
    vs(4, c4, 1);
    vs(5, c4, 1);
    vs(6, c6, 1);
    vs(7, c7, 1);
    vs(8, c8, 1);
    vs(9, c9, 1);
    vs(10, c10, 1);
    vs(11, c11, 1);
    vs(13, c13, 1);
    float fog[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    if (F<uint8_t*>(r, 0x83c)[0x1f]) {
        fog[1] = 1.0f;
        fog[0] = (float)(-((double)F<float>(r, 0x25b3) * F<float>(r, 0x2513)) * *(const double*)0x5dda20);
    }
    vs(14, fog, 1);
    int32_t w = Terrain_Rows((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine))) * 0x5c;
    int32_t h = Terrain_Cols((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine))) * 0x5c;
    double ih = *(const float*)0x5c85b4 / (double)h;
    float c12[4] = { (float)(*(const float*)0x5c85b4 / (double)w * F<float>(r, 0x36fd)), (float)(ih * F<float>(r, 0x36fd)), 0.0f,
                     0.0f };
    vs(12, c12, 1);
    uint32_t p0[4] = { 0x3f4ccccd, 0x3f59999a, 0x3f4ccccd, 0x3f800000 };
    uint32_t p1[4] = { 0, 0, 0, 0x3f000000 };
    uint32_t p2[4] = { 0, 0, 0, 0 };
    uint32_t p3[4] = { 0, 0, 0, 0x3f800000 };
    ps(0, p0);
    ps(1, p1);
    ps(2, p2);
    ps(3, p3);
}
RT3_REPLACE(0x546d40, Renderer_SetShaderConstants);

// 0x5471f0. Binds a vertex buffer (stride 0x18 / 0x20 / other picks the FVF) and
// index buffer, skipping unchanged ones (+0x2637 / +0x263b).
RT3_NOINLINE void __fastcall Render_SetStreams(uint8_t* r, int, void* vb, uint32_t stride, void* ib) {
    if (!vb) {
        F<void*>(r, 0x2637) = nullptr;
        return;
    }
    if (F<void*>(r, 0x2637) != vb) {
        void* d = Dev(r);
        uint32_t fvf = stride == 0x18 ? 0x142 : stride == 0x20 ? 0x112 : 0x152;
        ((int32_t(__stdcall*)(void*, uint32_t))Vt(d)[0x130 / 4])(d, fvf);
        d = Dev(r);
        ((int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t))Vt(d)[0x14c / 4])(d, 0, vb, stride);
        F<void*>(r, 0x2637) = vb;
    }
    if (!ib) {
        F<void*>(r, 0x263b) = nullptr;
        return;
    }
    if (F<void*>(r, 0x263b) == ib) return;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*, uint32_t))Vt(d)[0x154 / 4])(d, ib, 0);
    F<void*>(r, 0x263b) = ib;
}
RT3_REPLACE(0x5471f0, Render_SetStreams);

int32_t __fastcall Render_BlendOff(uint8_t* r) { return SetRs_100(r, 0x1b, 0); }
RT3_REPLACE(0x5472a0, Render_BlendOff);

void __fastcall Render_AllowTranslucency(uint8_t* r) {
    if (Engine()[0x11422e] || Engine()[0x11422f]) r[0x36f8] = 1;
}
RT3_REPLACE(0x5472c0, Render_AllowTranslucency);

void __fastcall Render_NoTranslucency(uint8_t* r) { r[0x36f8] = 0; }
RT3_REPLACE(0x5472f0, Render_NoTranslucency);

int32_t __fastcall Render_BlendDefault(uint8_t* r) { return SetRs_100(r, 0x1b, Cfg(r)[0x21] & 1); }
RT3_REPLACE(0x547300, Render_BlendDefault);

// 0x547320 / 0x547380. Alpha test (ref, func 7 = greater-equal) when supported (+0x114252).
RT3_NOINLINE void __fastcall Render_AlphaTestOn(uint8_t* r, int, uint32_t ref) {
    if (!Engine()[0x114252]) return;
    SetRs_100(r, 0xf, 1);
    SetRs_100(r, 0x18, ref);
    SetRs_100(r, 0x19, 7);
}
RT3_REPLACE(0x547320, Render_AlphaTestOn);

void __fastcall Render_AlphaTestOff(uint8_t* r) {
    if (!Engine()[0x114252]) return;
    SetRs_100(r, 0xf, 0);
    SetRs_100(r, 0x18, 0);
}
RT3_REPLACE(0x547380, Render_AlphaTestOff);

// 0x5473c0. Drops the saved state block (+0x25e7) and sets the screen-space world matrix.
void __fastcall Render_ScreenSpaceWorld(uint8_t* r) {
    if (void* sb = F<void*>(r, 0x25e7)) {
        void* d = Dev(r);
        ((int32_t(__stdcall*)(void*, void*))Vt(d)[0xa0 / 4])(d, sb);
        Free(F<void*>(r, 0x25e7));
        F<void*>(r, 0x25e7) = nullptr;
    }
    uint32_t m[16];
    ScreenMatrix(m);
    SetTransform_100(r, 0x100, m);
}
RT3_REPLACE(0x5473c0, Render_ScreenSpaceWorld);

// 0x5474a0. Sets the viewport to the screen fraction (x0, y0)-(x1, y1) (screen
// +0x25c7 x +0x25cb) and the world transform mapping it to clip space.
void __fastcall Render_SetSubViewport(uint8_t* r, int, float x0, float y0, float x1, float y1) {
    SaveViewport(r, 0x25e7);
    int32_t w = F<int32_t>(r, 0x25c7), h = F<int32_t>(r, 0x25cb);
    float hf = (float)h;
    float dx = (float)((double)x1 - x0), dy = (float)((double)y1 - y0);
    struct { int32_t x, y, w, h; float minZ, maxZ; } vp;
    vp.x = (int32_t)((double)x0 * w);
    vp.y = (int32_t)((double)hf * y0);
    vp.w = (int32_t)((double)dx * w);
    vp.h = (int32_t)((double)dy * hf);
    vp.minZ = 0.0f;
    vp.maxZ = 1.0f;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*))Vt(d)[0xa0 / 4])(d, &vp);
    float m[16] = {};
    double sx = D(0x5c85d0) / dx, sy = D(0x5c9be8) / dy;
    m[0] = (float)sx;
    m[5] = (float)sy;
    m[10] = 1.0f;
    m[15] = 1.0f;
    m[12] = (float)((double)Fl(0x5c9b08) - sx * x0);
    m[13] = (float)(-(sy * y0) + Fl(0x5c85b4));
    void* dev = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, const float*))Vt(dev)[0x94 / 4])(dev, 0x100, m);
}
RT3_REPLACE(0x5474a0, Render_SetSubViewport);

// 0x547610 / 0x547640. Restores the back buffer render target once (+0x254c).
RT3_NOINLINE void __fastcall Render_RestoreTarget(uint8_t* r) {
    if (!r[0x254c]) {
        void* d = Dev(r);
        ((int32_t(__stdcall*)(void*, void*, void*))Vt(d)[0x7c / 4])(d, F<void*>(r, 0x2607), F<void*>(r, 0x260b));
    }
    r[0x254c] = 1;
}
RT3_REPLACE(0x547610, Render_RestoreTarget);

void __fastcall Render_TargetChanged(uint8_t* r) { r[0x254c] = 0; }
RT3_REPLACE(0x547640, Render_TargetChanged);

// 0x547650. Texture stage setup: with bump mapping (+0x11422d) every stage uses its
// own coordinates untransformed, stage 1 addresses mirrored, and the bump shaders
// (+0x2623 vertex, +0x2633 pixel) are selected; else with detail textures (+0x114250)
// stage 1 reuses coordinates 0 scaled by +0x36fd and wraps. Stage 0 clamps.
RT3_NOINLINE void __fastcall Renderer_SetupStages(uint8_t* r) {
    uint8_t* e = g_engine;
    if (e[0x11422d]) {
        for (uint32_t s = 0; s < 4; ++s) StageState(r, s, 0xb, s);
        for (uint32_t s = 0; s < 4; ++s) StageState(r, s, 0x18, 0);
        StageState(r, 1, 0xd, 2);
        StageState(r, 1, 0xe, 2);
        void* dev = F<void*>(r, 0x25df);
        (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)dev + 0x130))(dev, F<uint32_t>(r, 0x2623));
        dev = F<void*>(r, 0x25df);
        (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)dev + 0x160))(dev, F<uint32_t>(r, 0x2633));
        return;
    }
    if (e[0x114250]) {
        float m[16] = {};
        F<uint32_t>(m, 0) = F<uint32_t>(r, 0x36fd);
        F<uint32_t>(m, 0x14) = F<uint32_t>(r, 0x36fd);
        m[10] = 1.0f;
        m[15] = 1.0f;
        StageState(r, 1, 0xb, 0);
        StageState(r, 1, 0x18, 2);
        void* dev = F<void*>(r, 0x25df);
        (*(int32_t(__stdcall**)(void*, uint32_t, const float*))(*(uint8_t**)dev + 0x94))(dev, 0x11, m);
        StageState(r, 1, 0xd, 1);
        StageState(r, 1, 0xe, 1);
    }
    StageState(r, 0, 0xd, 3);
    StageState(r, 0, 0xe, 3);
}
RT3_REPLACE(0x547650, Renderer_SetupStages);

// 0x5478a0. Sets up the water texture stages: with bump mapping (+0x11422d) stages
// 0-3 use their own coordinates untransformed, stages 2 / 3 get a rotated bump
// matrix (the second one scaled), stages 0 and 2 clamp, and the water shaders
// (+0x261b / +0x262b) are bound; otherwise stage 0 projects camera-space positions
// through a fixed matrix whose y offset follows the texture height.
RT3_NOINLINE void __fastcall Renderer_SetupWaterStages(uint8_t* r) {
    ((uint32_t(__stdcall*)())g_5c8068)();
    auto dev = [&]() { return F<uint8_t*>(r, 0x25df); };
    auto tss = [&](uint32_t stage, uint32_t type, uint32_t v) {
        uint8_t* d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t, uint32_t))(*(uint8_t**)d + 0xfc))(d, stage, type, v);
    };
    if (g_engine[0x11422d]) {
        for (uint32_t k = 0; k < 4; ++k) tss(k, 0xb, k);
        for (uint32_t k = 0; k < 4; ++k) tss(k, 0x18, 0);
        float sn = (float)(FsinX(*(const double*)0x5c8608) * *(const double*)0x5dda28);
        float cs = (float)(FcosX(*(const double*)0x5c8608) * *(const double*)0x5dda28);
        tss(2, 7, Bits(sn));
        tss(2, 8, Bits(cs));
        tss(2, 9, Bits(cs));
        tss(2, 0xa, Bits(-sn));
        float sn2 = (float)((double)sn * *(const double*)0x5c8c20);
        float cs2 = (float)((double)cs * *(const double*)0x5c9b60);
        tss(3, 7, Bits(sn2));
        tss(3, 8, Bits(cs2));
        tss(3, 9, Bits(cs2));
        tss(3, 0xa, Bits(-sn2));
        tss(0, 0xd, 3);
        tss(0, 0xe, 3);
        tss(2, 0xd, 3);
        tss(2, 0xe, 3);
        uint8_t* d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)d + 0x130))(d, F<uint32_t>(r, 0x261b));
        d = dev();
        (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)d + 0x160))(d, F<uint32_t>(r, 0x262b));
        return;
    }
    uint32_t m[16] = { 0x3edeb852, 0, 0, 0, 0, 0x3f59999a, 0, 0, 0x3f000000, 0, 0x3f800000, 0, 0, 0, 0, 0x3f800000 };
    int32_t th = Texture_Height(*F<uint8_t**>(r, 0x2593));
    m[9] = Bits((float)(*(const double*)0x5c8628 - (double)F<float>(r, 0x2537) / F<int32_t>(r, 0x25cb) / th));
    tss(0, 0xb, 0x20000);
    tss(0, 0x18, 0x103);
    uint8_t* d = dev();
    (*(int32_t(__stdcall**)(void*, uint32_t, const void*))(*(uint8_t**)d + 0x94))(d, 0x10, m);
}
RT3_REPLACE(0x5478a0, Renderer_SetupWaterStages);

// 0x547c30. Stores the fog/texture offset (+0x36df..) and uploads it as vertex
// shader constant 7 ({-a, -b, c, d}, or {0, 0, 1, 1} when all zero).
void __fastcall Render_SetShaderOffset(uint8_t* r, int, float a, float b, float c, float d) {
    const float zero = *(const float*)0x5c8598;
    F<float>(r, 0x36df) = a;
    F<float>(r, 0x36e7) = c;
    F<float>(r, 0x36e3) = b;
    F<float>(r, 0x36eb) = d;
    float v[4];
    if (a == zero && b == zero && c == zero && d == zero) {
        F<uint32_t>(v, 0) = 0;
        F<uint32_t>(v, 4) = 0;
        F<uint32_t>(v, 8) = 0x3f800000;
        F<uint32_t>(v, 0xc) = 0x3f800000;
    } else {
        v[0] = -a;
        v[2] = c;
        v[1] = -b;
        v[3] = d;
    }
    void* dv = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, const void*, uint32_t))Vt(dv)[0x13c / 4])(dv, 7, v, 1);
}
RT3_REPLACE(0x547c30, Render_SetShaderOffset);

// 0x547d10. Stage 0 texture transform: scale (c, d) and offset (a, b) via the
// third row; disabled when all are zero.
RT3_NOINLINE void __fastcall Render_SetTextureTransform(uint8_t* r, int, float a, float b, float c, float d) {
    const float zero = *(const float*)0x5c8598;
    if (a == zero && b == zero && c == zero && d == zero) {
        SetTss_100(r, 0, 0x18, 0);
        return;
    }
    float m[16];
    for (int k = 0; k < 16; ++k) F<uint32_t>(m, k * 4) = 0;
    m[0] = c;
    m[8] = a;
    m[15] = 1.0f;
    m[5] = d;
    m[9] = b;
    SetTss_100(r, 0, 0x18, 2);
    SetTransform_100(r, 0x10, m);
}
RT3_REPLACE(0x547d10, Render_SetTextureTransform);

// 0x547e30. Identity view and projection, screen-space world.
void __fastcall Render_ScreenSpaceAll(uint8_t* r) {
    uint32_t m[16] = {};
    m[15] = m[10] = m[5] = m[0] = 0x3f800000;
    SetTransform_100(r, 2, m);
    SetTransform_100(r, 3, m);
    m[0] = 0x40000000;
    m[5] = 0xc0000000;
    m[12] = 0xbf800000;
    m[13] = 0x3f800000;
    SetTransform_100(r, 0x100, m);
}
RT3_REPLACE(0x547e30, Render_ScreenSpaceAll);

// 0x547f20. Sets a linear gamma ramp scaled by `gamma` (fullscreen with gamma
// support only) and remembers it (config +0x2f).
RT3_NOINLINE void __fastcall Render_SetGamma(uint8_t* r, int, float gamma) {
    uint8_t* e = Engine();
    if (!e[0x114226] || !e[0x114231]) return;
    uint16_t ramp[3][256];
    for (int32_t i = 0, v = 0; v < 0xff00; ++i, v += 0xff) {
        int32_t x = (int32_t)((double)v * gamma);
        if (x > 0xffff) x = 0xffff;
        ramp[2][i] = (uint16_t)x;
        ramp[1][i] = (uint16_t)x;
        ramp[0][i] = (uint16_t)x;
    }
    void* d = Dev(r);
    ((void(__stdcall*)(void*, uint32_t, const void*))Vt(d)[0x48 / 4])(d, 1, ramp);
    F<float>(Cfg(r), 0x2f) = gamma;
}
RT3_REPLACE(0x547f20, Render_SetGamma);

// 0x547fe0. Re-applies the stored view (+0x346f), projection (+0x352f) and world (+0x34ef).
void __fastcall Render_ApplyStoredTransforms(uint8_t* r) {
    SetTransform_100(r, 2, r + 0x346f);
    SetTransform_100(r, 3, r + 0x352f);
    SetTransform_100(r, 0x100, r + 0x34ef);
}
RT3_REPLACE(0x547fe0, Render_ApplyStoredTransforms);

// 0x548030 / 0x548060. Blend factors: (ZERO, INVSRCALPHA) / (SRCALPHA, INVSRCALPHA).
int32_t __fastcall Render_BlendDarken(uint8_t* r) {
    SetRs_101(r, 0x13, 1);
    return SetRs_101(r, 0x14, 6);
}
RT3_REPLACE(0x548030, Render_BlendDarken);

int32_t __fastcall Render_BlendAlpha(uint8_t* r) {
    SetRs_101(r, 0x13, 5);
    return SetRs_101(r, 0x14, 6);
}
RT3_REPLACE(0x548060, Render_BlendAlpha);

RT3_NOINLINE double __fastcall Render_ScaleByUnit(uint8_t*, int, float v) { return (double)v * *(const float*)0x5ce4fc; }
RT3_REPLACE(0x548090, Render_ScaleByUnit);

// 0x5480a0. Distance from q to the line p + t d (t = (q - p) . d).
double __stdcall PointLineDistance(float px, float py, float pz, float dx, float dy, float dz, float qx, float qy,
                                   float qz) {
    double a = (double)qx - px, b = (double)qy - py, c = (double)qz - pz;
    double t = (a * dx + c * dz) + b * dy;
    float ex = (float)((double)dx * t + px - qx);
    float ey = (float)((double)dy * t + py - qy);
    double ez = t * dz + pz - qz;
    return Fsqrt((double)ex * ex + (double)ey * ey + ez * ez);
}
RT3_REPLACE(0x5480a0, PointLineDistance);

// 0x548120. Identity world (D3DX translation 0) and the projection for the stored size.
void __fastcall Render_ResetWorld(uint8_t* r) {
    uint8_t* m = r + 0x34ef;
    ((void*(__stdcall*)(void*, float, float, float))0x56dd2a)(m, 0.0f, 0.0f, 0.0f);
    F<uint32_t>(r, 0x352b) = 0x3f800000;
    F<uint32_t>(r, 0x3517) = 0x3f800000;
    F<uint32_t>(r, 0x3503) = 0x3f800000;
    F<uint32_t>(m, 0) = 0x3f800000;
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, uint32_t, void*))Vt(d)[0x94 / 4])(d, 0x100, m);
    Render_SetProjection(r, 0, F<int32_t>(r, 0x25c7), F<int32_t>(r, 0x25cb));
}
RT3_REPLACE(0x548120, Render_ResetWorld);

// 0x548180. Sets the viewport to the pixel rect (x0, y0)-(x1, y1) (the saved full
// one, +0x25e3, when under 2 pixels) with matching projection, and records the
// scale (+0x2523/+0x252b) and centre offset (+0x2527/+0x252f) against the screen.
RT3_NOINLINE void __fastcall Render_SetPixelViewport(uint8_t* r, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    SaveViewport(r, 0x25e3);
    int32_t sw = F<int32_t>(r, 0x25c7), sh = F<int32_t>(r, 0x25cb);
    int32_t w = x1 - x0, h = y1 - y0;
    struct { int32_t x, y, w, h; float minZ, maxZ; } vp = { x0, y0, w, h, 0.0f, 1.0f };
    F<float>(r, 0x2523) = (float)((double)sw / (double)(uint32_t)w);
    F<float>(r, 0x252b) = (float)((double)sh / (double)(uint32_t)h);
    int32_t hw = sw / 2, cx = (x1 + x0) / 2, hh = sh / 2, cy = (y1 + y0) / 2;
    F<float>(r, 0x2527) = (float)(((double)hw - cx) / hw);
    F<float>(r, 0x252f) = (float)(((double)hh - cy) / hh);
    if (w < 2 || h < 2) vp = *(decltype(vp)*)F<void*>(r, 0x25e3);
    void* d = Dev(r);
    ((int32_t(__stdcall*)(void*, void*))Vt(d)[0xa0 / 4])(d, &vp);
    Render_SetProjection(r, 0, vp.w, vp.h);
}
RT3_REPLACE(0x548180, Render_SetPixelViewport);

// 0x5482e0. Sets the viewport (or, with projOnly, just the projection) to the
// camera's pixel bounds.
void __fastcall Render_SetCameraViewport(uint8_t* r, int, uint8_t* cam, uint8_t projOnly) {
    int32_t x0, y0, x1, y1;
    Camera_BoundsPixels(cam, 0, &x0, &y0, &x1, &y1);
    if (!projOnly) Render_SetPixelViewport(r, 0, x0, y0, x1, y1);
    else Render_SetProjection(r, 0, x1 - x0, y1 - y0);
}
RT3_REPLACE(0x5482e0, Render_SetCameraViewport);

// 0x548350. Builds the view matrices from camera `cam`: the camera view (+0x34af),
// the reflection view (+0x346f; mirrored in the water plane 2 x +0x2537 when
// `reflect`), the light-direction view (+0x35ef) and the shadow map: an
// orthographic projection (+0x356f) sized by the camera's height above the ground
// (max 1536) and a view from the light (+0x362f) placed back from the camera
// position snapped to the terrain cell grid (stored in +0x3701..). Changed shadow
// parameters (cached at 0x625438..) mark the shadow map dirty (+0x36f7).
void __fastcall Renderer_SetupViews(uint8_t* r, int, uint8_t* cam, uint8_t reflect) {
    float at[3], eye[3], up[3];
    Obj_GetXYZ4(cam, 0, (uint32_t*)&at[0], (uint32_t*)&at[1], (uint32_t*)&at[2]);
    Obj_GetXYZ1c(cam, 0, (uint32_t*)&eye[0], (uint32_t*)&eye[1], (uint32_t*)&eye[2]);
    Obj_GetXYZ2c(cam, 0, (uint32_t*)&up[0], (uint32_t*)&up[1], (uint32_t*)&up[2]);
    LookAt(r + 0x34af, eye, at, up);
    if (reflect) {
        double w2 = (double)F<float>(r, 0x2537) + F<float>(r, 0x2537);
        at[1] = (float)(w2 - at[1]);
        eye[1] = (float)(w2 - eye[1]);
        up[0] = (float)(up[0] * D(0x5c9bf0));
        up[2] = (float)(up[2] * D(0x5c9bf0));
    }
    LookAt(r + 0x346f, eye, at, up);
    {
        double dx = (double)at[0] - eye[0];
        at[0] = (float)dx;
        double dy = (double)at[1] - eye[1];
        float ndx = (float)dx;
        double dz = (double)at[2] - eye[2];
        double len = Fsqrt(dz * dz + dy * dy + (double)at[0] * at[0]);
        if (Fl_201(0x5c8598) < len) {
            double inv = Fl_201(0x5c85b4) / len;
            ndx = (float)((double)at[0] * inv);
            dy = dy * inv;
            dz = dz * inv;
        }
        float dir[3] = { ndx, (float)dy, (float)dz };
        float zero[3] = { 0.0f, 0.0f, 0.0f };
        LookAt(r + 0x35ef, zero, dir, up);
    }
    Obj_GetXYZ4(cam, 0, (uint32_t*)&eye[0], (uint32_t*)&eye[1], (uint32_t*)&eye[2]);
    Obj_GetXYZ1c(cam, 0, (uint32_t*)&at[0], (uint32_t*)&at[1], (uint32_t*)&at[2]);
    double h = Terrain_HeightAt((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, at[0], at[2]);
    uint8_t* e = g_engine;
    double g = F<int32_t>(e, 0x11420e);
    int32_t n = Ftol(((double)at[1] - h) / g * g);
    double sz = (double)n * Fl_201(0x5dda44) + Fl_201(0x5cb490);
    float size = (float)sz;
    if (sz > Fl_201(0x5dda40)) size = 1536.0f;
    float d[3];
    d[0] = (float)((double)at[0] - eye[0]);
    d[1] = (float)((double)at[1] - eye[1]);
    d[2] = (float)((double)at[2] - eye[2]);
    float ny = d[1], nz = d[2];
    float back = (float)((double)size * D(0x5dda38) * D(0x5c8c48));
    float len = (float)Fsqrt((double)d[2] * d[2] + (double)d[1] * d[1] + (double)d[0] * d[0]);
    double nx = d[0];
    if (Fl_201(0x5c8598) < len) {
        float inv = (float)(Fl_201(0x5c85b4) / len);
        nx = (double)d[0] * inv;
        ny = (float)((double)d[1] * inv);
        nz = (float)((double)d[2] * inv);
    }
    float px = (float)((double)at[0] - nx * back);
    float py = (float)((double)at[1] - (double)ny * back);
    float pz = (float)((double)at[2] - (double)nz * back);
    int32_t gi = F<int32_t>(e, 0x11420e);
    px = (float)(Ftol(px) / gi * gi);
    pz = (float)(Ftol(pz) / gi * gi);
    py = (float)(Ftol(py) / gi * gi);
    uint8_t* lvl = r + F<int32_t>(r, 0x36f3) * 0x33;
    float sx = (float)((double)px - F<float>(lvl, 0x867));
    float sz2 = (float)((double)pz - F<float>(lvl, 0x86f) * D(0x5c8758));
    ((void*(__stdcall*)(void*, float, float, float, float))0x56e4bb)(r + 0x356f, size, size, -65535.0f, 65535.0f);
    F<float>(r, 0x3701) = px;
    F<float>(r, 0x3705) = py;
    F<float>(r, 0x3709) = pz;
    double floor = F<float>(r + F<int32_t>(r, 0x36f3) * 0x33, 0x86b);
    if (floor > D(0x5ca180)) floor = Fl_201(0x5dda30);
    float sy = (float)((double)py - floor);
    float sp[3] = { sx, sy, sz2 };
    if (sp[0] != G<float>(0x625450) || sp[1] != G<float>(0x62544c) || G<float>(0x625448) != size ||
        sp[2] != G<float>(0x625444) || up[0] != G<float>(0x625440) || up[1] != G<float>(0x62543c) ||
        up[2] != G<float>(0x625438)) {
        G<float>(0x625450) = sp[0];
        G<float>(0x625444) = sp[2];
        G<float>(0x62544c) = sp[1];
        G<float>(0x625448) = size;
        r[0x36f7] = 1;
        G<float>(0x625440) = up[0];
        G<float>(0x62543c) = up[1];
        G<float>(0x625438) = up[2];
    }
    float lp[3] = { px, py, pz };
    LookAt(r + 0x362f, sp, lp, up);
}
RT3_REPLACE(0x548350, Renderer_SetupViews);

// 0x548820. Resets the stage cache to MODULATE(TEXTURE, DIFFUSE) for colour and alpha.
void __fastcall Render_ResetStageCache(uint8_t* r) {
    for (int32_t s = 0; s < F<int32_t>(r, 0x258f); ++s) {
        uint32_t* st = (uint32_t*)(F<uint8_t*>(r, 0x2597) + s * 24);
        st[0] = 1;
        st[1] = 2;
        st[2] = 1;
        if (s < F<int32_t>(r, 0x258f)) {
            st = (uint32_t*)(F<uint8_t*>(r, 0x2597) + s * 24);
            st[3] = 1;
            st[4] = 2;
            st[5] = 1;
        }
    }
}
RT3_REPLACE(0x548820, Render_ResetStageCache);

// 0x5488a0. Saves the stage cache on the stack +0x259b and applies `states` when given.
RT3_NOINLINE void __fastcall Render_PushStages(uint8_t* r, int, const uint32_t* states) {
    uint32_t n = (uint32_t)F<int32_t>(r, 0x258f) * 24;
    uint8_t* p = (uint8_t*)Malloc(n);
    n = (uint32_t)F<int32_t>(r, 0x258f) * 24;
    const uint8_t* src = F<uint8_t*>(r, 0x2597);
    for (uint32_t k = 0; k < n; ++k) p[k] = src[k];
    PtrList_Prepend(F<PtrList*>(r, 0x259b), 0, p);
    if (states) ApplyStages(r, states);
}
RT3_REPLACE(0x5488a0, Render_PushStages);

// 0x548940. Pops n saved stage caches (at most the stack size) and applies the last.
RT3_NOINLINE void __fastcall Render_PopStages(uint8_t* r, int, int32_t n) {
    int32_t cnt = F<int32_t>(F<uint8_t*>(r, 0x259b), 0xc);
    if (!cnt) return;
    if (n > cnt) n = cnt;
    uint32_t* p = nullptr;
    for (int32_t i = n; i; --i) {
        if (p) Free(p);
        p = (uint32_t*)PtrList_PopFront(F<PtrList*>(r, 0x259b));
    }
    ApplyStages(r, p);
    Free(p);
}
RT3_REPLACE(0x548940, Render_PopStages);

// 0x5489f0. Whether renderer r's device can draw stage setup `states` in
// one pass (ValidateDevice) with the test texture bound to every stage;
// unbinds all stages afterwards.
RT3_NOINLINE int32_t __fastcall Render_ValidateStates(uint8_t* r, int, const uint32_t* states) {
    uint8_t* tex = TextureCache_Get(g_pack, 0, (const char*)0x5dda48, 0, 0, 0x7f);
    for (int32_t s = 0; s < F<int32_t>(r, 0x258f); ++s) Renderer_SetTexture(r, 0, s, tex);
    Render_PushStages(r, 0, states);
    uint32_t passes = 0;
    void* dev = F<void*>(r, 0x25df);
    int32_t hr = ((int32_t(__stdcall*)(void*, uint32_t*))(*(void***)dev)[0x40])(dev, &passes);
    int32_t ok = !hr && passes == 1;
    Render_PopStages(r, 0, 1);
    for (int32_t s = 0; s < F<int32_t>(r, 0x258f); ++s) {
        if (F<uint8_t**>(r, 0x2593)[s]) {
            ++F<int32_t>(r, 0x370d);
            void* d = F<void*>(r, 0x25df);
            ((int32_t(__stdcall*)(void*, uint32_t, void*))(*(void***)d)[0x3d])(d, (uint32_t)s, nullptr);
        }
        F<uint8_t**>(r, 0x2593)[s] = nullptr;
    }
    Pack_Release(g_pack, 0, tex);
    return ok;
}
RT3_REPLACE(0x5489f0, Render_ValidateStates);

// 0x548ae0. Sets up n texture stages once (+0x258f; bound textures +0x2593, 6-dword
// states +0x2597 = {1, 2, 1, 1, 2, 1}) and the two draw lists (+0x259f, +0x259b).
RT3_NOINLINE void __fastcall Render_InitStages(uint8_t* r, int, int32_t n) {
    if (F<int32_t>(r, 0x258f)) return;
    F<int32_t>(r, 0x258f) = n;
    F<void*>(r, 0x2593) = Malloc(n * 4);
    F<void*>(r, 0x2597) = Malloc(n * 24);
    for (int32_t i = 0; i < F<int32_t>(r, 0x258f); ++i) {
        int32_t* st = (int32_t*)(F<uint8_t*>(r, 0x2597) + i * 24);
        st[0] = 1;
        st[1] = 2;
        st[2] = 1;
        if (i < F<int32_t>(r, 0x258f)) {
            st[3] = 1;
            st[4] = 2;
            st[5] = 1;
        }
        F<int32_t*>(r, 0x2593)[i] = 0;
        if (i < F<int32_t>(r, 0x258f)) {
            if (F<int32_t*>(r, 0x2593)[i]) {
                ++F<int32_t>(r, 0x370d);
                void* d = Dev(r);
                ((int32_t(__stdcall*)(void*, uint32_t, void*))Vt(d)[0xf4 / 4])(d, i, nullptr);
            }
            F<int32_t*>(r, 0x2593)[i] = 0;
        }
    }
    auto newList = []() -> PtrList* {
        void* p = ((void*(__cdecl*)(size_t))0x53b070)(0x18);
        return p ? PtrList_Init((PtrList*)p) : nullptr;
    };
    if (!F<PtrList*>(r, 0x259f)) F<PtrList*>(r, 0x259f) = newList();
    if (!F<PtrList*>(r, 0x259b)) F<PtrList*>(r, 0x259b) = newList();
}
RT3_REPLACE(0x548ae0, Render_InitStages);

// 0x548c50. Frees the draw lists (and their items) and the texture stages, unbinding
// any bound textures (counting +0x370d).
RT3_NOINLINE void __fastcall Render_FreeStages(uint8_t* r) {
    if (!F<int32_t>(r, 0x258f)) return;
    static const uint32_t lists[2] = { 0x259f, 0x259b };
    for (uint32_t off : lists) {
        PtrList_Rewind(F<PtrList*>(r, off));
        for (void* p = PtrList_Next(F<PtrList*>(r, off)); p; p = PtrList_Next(F<PtrList*>(r, off))) Free(p);
        if (PtrList* l = F<PtrList*>(r, off)) {
            PtrList_Destroy(l);
            ((void(__cdecl*)(void*))0x53b080)(l);
        }
        F<PtrList*>(r, off) = nullptr;
    }
    for (int32_t i = 0; i < F<int32_t>(r, 0x258f); ++i) {
        if (F<int32_t*>(r, 0x2593)[i]) {
            ++F<int32_t>(r, 0x370d);
            void* d = Dev(r);
            ((int32_t(__stdcall*)(void*, uint32_t, void*))Vt(d)[0xf4 / 4])(d, i, nullptr);
        }
        F<int32_t*>(r, 0x2593)[i] = 0;
        F<int32_t*>(r, 0x2593)[i] = 0;
    }
    F<int32_t>(r, 0x258f) = 0;
    Free(F<void*>(r, 0x2593));
    Free(F<void*>(r, 0x2597));
}
RT3_REPLACE(0x548c50, Render_FreeStages);

// 0x548da0. Draws texture groups `groups` (each texture's object list
// +0xdc, emptied) of lit scene objects with renderer r: per object its
// world matrix (+0xce), UV transform, tint (+0x43, plain when 0x7f7f7f)
// and alpha (+0x47 top byte, opaque when 0xff) as texture factor, refilling
// its vertices when dirty (+0x8d) and drawing its triangles (+0x1ee..
// +0x1fa); `release` drops each object's vertex range afterwards. Multi-
// texture (+0x36f8) adds the detail stage; fog-table cards (0x11422e) get
// specular around the pass.
void __fastcall Render_DrawLitGroups(uint8_t* r, int, PtrList* groups, uint8_t release) {
    uint8_t tinted = 0xe1, detail = 0, blended = 0xe0, uvMoved = 0;
    PtrList_Rewind(groups);
    uint8_t* grp = (uint8_t*)PtrList_Next(groups);
    if (g_engine[0x11422e]) {
        DevRenderState(r, 0x8e, 1);
        DevRenderState(r, 0x1d, 1);
    } else {
        DevRenderState(r, 0x1d, 0);
    }
    Render_PushStages(r, 0, nullptr);
    Render_SetColorOp(r, 0, 0, 2, F<uint32_t>(r, 4), 1, 1);
    Render_AlphaTestOn(r, 0, 0x3f);
    if (r[0x36f8]) {
        Render_SetFiltering(r, 0, 0, -1);
        if (F<int32_t>(F<uint8_t*>(r, 0x83c), 0x29) == 2)
            for (uint32_t s = 0; s < 4; ++s) DevStageState(r, s, 0x12, 1);
        detail = 1;
        Render_SetColorOp(r, 0, 1, 1, 0x10, 3, 1);
        DevRenderState(r, 0x3c, 0xffffffaf);
    }
    while (grp) {
        PtrList* objs = F<PtrList*>(grp, 0xdc);
        F<int32_t>(grp, 0xe0) = 0;
        PtrList_Rewind(objs);
        uint8_t* o = (uint8_t*)PtrList_Next(objs);
        Renderer_SetTexture(r, 0, 0, grp);
        while (o) {
            memcpy(r + 0x34ef, o + 0xce, 0x40);
            void* dev = F<void*>(r, 0x25df);
            (*(int32_t(__stdcall**)(void*, uint32_t, void*))(*(uint8_t**)dev + 0x94))(dev, 0x100, r + 0x34ef);
            uint32_t uv[4];
            if (SceneObj_GetUvTransform(o, 0, &uv[0], &uv[1], &uv[2], &uv[3])) {
                uvMoved = 1;
                Render_SetTextureTransform(r, 0, *(float*)&uv[0], *(float*)&uv[1], *(float*)&uv[2], *(float*)&uv[3]);
            } else if (uvMoved) {
                uvMoved = 0;
                Render_SetTextureTransform(r, 0, 0.0f, 0.0f, 0.0f, 0.0f);
            }
            uint32_t col = F<uint32_t>(o, 0x43) & 0xffffff;
            uint32_t alpha = F<uint32_t>(o, 0x47) & 0xff000000;
            if (col != 0x7f7f7f) {
                if (tinted != 1) {
                    Render_SetColorOp(r, 0, 0, 2, 4, 3, 1);
                    tinted = 1;
                }
            } else if (tinted) {
                Render_SetColorOp(r, 0, 0, 2, F<uint32_t>(r, 4), 1, 1);
                tinted = 0;
            }
            bool factor = true;
            if (alpha != 0xff000000) {
                if (blended != 1) {
                    DevRenderState(r, 0x1b, F<uint8_t*>(r, 0x83c)[0x21] & 1);
                    Render_SetAlphaOp(r, 0, 0, 2, 4, 3, 1);
                    blended = 1;
                }
            } else {
                if (blended) {
                    DevRenderState(r, 0x1b, 0);
                    Render_SetAlphaOp(r, 0, 0, 2, 4, 1, 1);
                    blended = 0;
                }
                if (!tinted) {
                    factor = false;
                    if (r[0x36f8] && !detail) {
                        detail = 1;
                        Render_SetColorOp(r, 0, 1, 1, 0x10, 3, 1);
                        DevRenderState(r, 0x3c, 0xffffffaf);
                    }
                }
            }
            if (factor) {
                DevRenderState(r, 0x3c, alpha | col);
                if (detail == 1) {
                    detail = 0;
                    Render_SetColorOp(r, 0, 1, 2, 1, 1, 1);
                }
            }
            if (o[0x8d]) {
                int32_t count = 0;
                uint8_t* ent = LitObj_VbEntry(o);
                void* vb = Render_LockVbRange(r, 0, F<int32_t>(ent, 0x39), F<int32_t>(ent, 0x3d));
                SceneObj_EmitLit(o, 0, (uint32_t*)vb, &count);
                if (r[0x254e]) {
                    r[0x254e] = 0;
                    --g_ccd824;
                    void* b = F<void*>(r, 0x2643);
                    (*(int32_t(__stdcall**)(void*))(*(uint8_t**)b + 0x30))(b);
                }
            }
            int32_t first = F<int32_t>(o, 0x1ee);
            int32_t n = (F<int32_t>(o, 0x1fa) - first) / 3;
            if (n >= 1) {
                F<int32_t>(r, 0x3711) += n;
                void* d = F<void*>(r, 0x25df);
                (*(int32_t(__stdcall**)(void*, uint32_t, int32_t, int32_t))(*(uint8_t**)d + 0x118))(d, 4, first, n);
            }
            if (release) {
                F<int32_t>(LitObj_VbEntry(o), 0x39) = -1;
                F<int32_t>(o, 0x1ee) = -1;
            }
            o = (uint8_t*)PtrList_Next(objs);
        }
        PtrList_Clear(objs);
        grp = (uint8_t*)PtrList_Next(groups);
    }
    if (uvMoved) Render_SetTextureTransform(r, 0, 0.0f, 0.0f, 0.0f, 0.0f);
    if (r[0x36f8]) {
        Render_SetFiltering(r, 0, 1, -1);
        if (F<int32_t>(F<uint8_t*>(r, 0x83c), 0x29) == 2)
            for (uint32_t s = 0; s < 4; ++s) DevStageState(r, s, 0x12, 2);
    }
    if (g_engine[0x11422e]) DevRenderState(r, 0x1d, 0);
    Render_PopStages(r, 0, 1);
}
RT3_REPLACE(0x548da0, Render_DrawLitGroups);

 // namespace

// 0x5492d0. Creates the Direct3D 8 object and device for renderer r
// (+0x25db / +0x25df): reads the HAL caps and desktop mode, builds the
// capability report +0x3c, picks the texture (DXT3 compression, else 32-bit
// A8R8G8B8 / X8R8G8B8 / 16-bit) and depth (D24X8, else D16) formats, and
// creates the device windowed or (`fullScreen`) in the configured mode,
// retrying without the refresh rate; a windowed failure retries full screen
// from the start. Then sizes the frame buffers +0x253f, sets up stages,
// antialiasing, the shaders when supported, gamma and the matrices. Returns
// 0, or -1 when Direct3D can't be used.
RT3_NOINLINE int32_t __fastcall Render_InitDevice(uint8_t* r, int, uint8_t fullScreen) {
    void* d3d = D3DCreate(0xdc);
    F<void*>(r, 0x25db) = d3d;
    if (!d3d) {
        Fatal_Exit(GetString(0x7f));
        return -1;
    }
    uint8_t* cfg = F<uint8_t*>(r, 0x83c);
    uint32_t caps[0x35];
    DisplayMode mode;
    uint32_t pp[0xd];
    for (;;) {
        memset(caps, 0, sizeof caps);
        Com(F<void*>(r, 0x25db), 0x34, 0u, 1u, (void*)caps);
        if (Com(F<void*>(r, 0x25db), 0x20, 0u, (void*)&mode) < 0) {
            Com(F<void*>(r, 0x25db), 8);
            return -1;
        }
        uint32_t f = mode.format;
        bool known = (f >= 0x14 && f <= 0x1a) || (f >= 0x1d && f <= 0x1f);
        if (!known && !fullScreen) Fatal_Exit(GetString(0x80));
        memset(r + 0x3c, 0, 0x400);
        FormatText((char*)r + 0x3c, GetString(0x81), 0, 0, 0, 0, 0, 0, 0, 0);
        if ((caps[0x1c / 4] & 0x10000) && Eng()[0x11422e]) {
            Eng()[0x11422e] = 1;
            Report(r, 0x82);
        } else {
            Eng()[0x11422e] = 0;
            Eng()[0x1146d2] = 1;
            F<uint8_t*>(r, 0x83c)[0x61] = 0;
        }
        if (caps[0x34 / 4] & 0x40) Eng()[0x114252] = 1;
        uint32_t c24 = caps[0x24 / 4];
        if (!(c24 & 0x80)) {
            if (c24 & 0x100) F<uint8_t*>(r, 0x83c)[0x1f] = 0;
            else F<uint8_t*>(r, 0x83c)[0x2e] = 0;
        }
        if (caps[0x90 / 4] & 0x800000) {
            Eng()[0x114230] = 1;
        } else {
            Eng()[0x114230] = 0;
            F<uint8_t*>(r, 0x83c)[0x2d] = 0;
        }
        if (caps[0xc / 4] & 0x20000) {
            Eng()[0x114231] = 1;
            Report(r, 0x83);
        }
        F<uint32_t>(Eng(), 0x11423b) = caps[0x58 / 4];
        F<uint32_t>(Eng(), 0x11423f) = caps[0x5c / 4];
        Engine_TotalTextureMem(Eng());
        F<uint32_t>(Eng(), 0x114243) = F<uint32_t>(Eng(), 0x11423b);
        F<uint32_t>(Eng(), 0x114247) = F<uint32_t>(Eng(), 0x11423f);
        Engine_CurrentMode(Eng(), 0, (int32_t*)(r + 0x25c7), (int32_t*)(r + 0x25cb), (int32_t*)(r + 0x25cf), (int32_t*)(r + 0x25d3),
                           (uint32_t*)(r + 0x25d7));
        Eng()[0x11424f] = 0;
        if (fullScreen) {
            mode.width = F<uint32_t>(r, 0x25c7);
            mode.height = F<uint32_t>(r, 0x25cb);
            mode.format = 0x16;
        }
        // Texture format: DXT3 when it can be used, else 32-bit, else 16-bit.
        uint32_t tex = 0x15;
        bool dxt = false;
        if (Eng()[0x114230]) {
            int32_t c = Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, mode.format, 0u, 3u, 0x33545844u);
            if (!Unsupported(c) && !F<int32_t>(F<uint8_t*>(r, 0x83c), 0x12)) {
                Eng()[0x11422f] = 1;
                Report(r, 0x84);
                dxt = true;
            }
        }
        if (!dxt) {
            int32_t c = Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, mode.format, 0u, 3u, 0x33545844u);
            if (!Unsupported(c)) Report(r, 0x85);
            Eng()[0x11422f] = 0;
            tex = 0x1a;
            Eng()[0x11424f] = 1;
        }
        if (Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, mode.format, 0u, 3u, tex) < 0) {
            mode.format = 0x15;
            if (Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, 0x15u, 0u, 3u, tex) < 0) {
                mode.format = 0x14;
                if (Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, 0x14u, 0u, 3u, tex) < 0) {
                    tex = 0x1a;
                    for (;;) {
                        mode.format = 0x17;
                        if (Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, 0x17u, 0u, 3u, tex) >= 0) break;
                        mode.format = 0x18;
                        if (Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, 0x18u, 0u, 3u, tex) >= 0) break;
                        mode.format = 0x1a;
                        if (Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, 0x1au, 0u, 3u, tex) >= 0) break;
                        if (tex != 0x1a) {
                            Com(F<void*>(r, 0x25db), 8);
                            Fatal_Exit(GetString(0xdb9));
                            return -1;
                        }
                        tex = 0x19;
                    }
                    Eng()[0x11424f] = 1;
                }
            }
        }
        F<uint32_t>(Eng(), 0x11424b) = tex;
        Eng()[0x114232] = 0;
        F<uint32_t>(Eng(), 0x114237) = 0;
        Eng()[0x114232] = 1;
        // Present parameters; depth D24X8, else D16.
        memset(pp, 0, sizeof pp);
        pp[8] = 1;
        pp[9] = 0x4d;
        if (Unsupported(Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, mode.format, 2u, 1u, 0x4du))) {
            pp[9] = 0x50;
            F<uint32_t>(Eng(), 0x114237) = 1;
            if (Unsupported(Com(F<void*>(r, 0x25db), 0x28, 0u, 1u, mode.format, 2u, 1u, 0x50u))) {
                Fatal_Exit(GetString(0x87));
                Com(F<void*>(r, 0x25db), 8);
                return -1;
            }
        }
        pp[2] = mode.format;
        pp[0] = F<uint32_t>(r, 0x25c7);
        pp[1] = F<uint32_t>(r, 0x25cb);
        pp[5] = 1;
        pp[7] = 1;
        Eng()[0x11422c] = 0;
        if (!fullScreen) {
            pp[3] = 1;
        } else {
            pp[0xb] = F<uint32_t>(r, 0x25d3);
            pp[7] = 0;
            pp[0xc] = 0x80000000;
            pp[6] = (uint32_t)(uintptr_t)Engine_Window(Eng());
            pp[5] = F<uint32_t>(Eng(), 0x114228);
            if (pp[5] == 2) pp[3] = (Eng()[0x1146d2] == 0) + 1;
            else pp[3] = 1;
            if (F<int32_t>(r, 0x25d7) && Engine_Get1146ae(Eng())) {
                Eng()[0x11422c] = 1;
                pp[4] = 2;
                pp[5] = 1;
                pp[3] = 1;
            }
        }
        uint32_t behaviour = Eng()[0x11422e] ? 0x40 : 0x20;
        Eng()[0x114205] = 0;
        void* d3dNow = F<void*>(r, 0x25db);
        HWND__* wnd = Engine_Window(Eng());
        if (Com(d3dNow, 0x3c, 0u, 1u, wnd, behaviour, (void*)pp, (void*)(r + 0x25df)) >= 0) break;
        if (fullScreen) {
            pp[5] = F<uint32_t>(Eng(), 0x114228);
            pp[3] = 1;
            pp[4] = 0;
            Eng()[0x1146d2] = 1;
            Eng()[0x114205] |= 2;
            if (Render_CreateDevice(r, 0, behaviour, pp)) break;
            F<uint32_t>(r, 0x25d3) = 0;
            pp[0xb] = 0;
            Eng()[0x114205] |= 4;
            if (Render_CreateDevice(r, 0, behaviour, pp)) break;
            Com(F<void*>(r, 0x25db), 8);
            Fatal_Exit(GetString(0xdb9));
            break;
        }
        Com(F<void*>(r, 0x25db), 8);
        Eng()[0x114205] |= 1;
        fullScreen = 1;
        d3d = D3DCreate(0xdc);
        F<void*>(r, 0x25db) = d3d;
        if (!d3d) {
            Fatal_Exit(GetString(0x7f));
            return -1;
        }
    }
    memcpy(r + 8, pp, sizeof pp);
    Eng()[0x114226] = fullScreen;
    F<uint8_t*>(r, 0x83c)[0xc] = fullScreen;
    Engine_SaveConfig(Eng());
    F<void*>(Eng(), 8) = F<void*>(r, 0x25df);
    Engine_SampleStartTextureMem(Eng());
    F<int32_t>(r, 0x253f) = 0;
    int32_t frames = (int32_t)pp[3];
    if (fullScreen) ++frames;
    int32_t px = frames * (int32_t)pp[1] * (int32_t)pp[0];
    F<int32_t>(r, 0x253f) = Eng()[0x11424f] ? px * 2 : px * 4;
    F<int32_t>(r, 0x253f) += F<int32_t>(Eng(), 0x114237) ? px * 2 : px * 4;
    Report(r, Eng()[0x11424f] ? 0x8b : 0x8c);
    Report(r, F<int32_t>(Eng(), 0x114237) ? 0x8d : 0x8e);
    Render_InitStages(r, 0, (int32_t)caps[0x98 / 4]);
    char num[0x20], num2[0x20];
    int32_t mem = Engine_TotalTextureMem(Eng());
    Report(r, 0x8f, Num_Format(num, 3, (double)mem * *(const float*)0x5dd08c, 10, 1, 0, 0));
    mem = Engine_TextureMemLimit(Eng());
    Report(r, 0x90, Num_Format(num, 3, (double)mem * *(const float*)0x5dd08c, 10, 1, 0, 0));
    void* dev = F<void*>(r, 0x25df);
    int32_t aa = Engine_Get1146ae(Eng());
    Com(dev, 0xc8, 0xa1u, aa);
    Com(F<void*>(r, 0x25df), 0x80, (void*)(r + 0x2607));
    Com(F<void*>(r, 0x25df), 0x84, (void*)(r + 0x260b));
    Com(F<void*>(r, 0x2607), 8);
    Com(F<void*>(r, 0x260b), 8);
    Eng()[0x11422d] = 0;
    if (Eng()[0x11422e]) {
        uint32_t vs = caps[0xc4 / 4];
        int32_t major = (vs >> 8) & 0xff, minor = vs & 0xff;
        memset(caps, 0, sizeof caps);
        Com(F<void*>(r, 0x25df), 0x1c, (void*)caps);
        float psMax = *(float*)&caps[0xd0 / 4];
        if ((double)psMax >= *(const double*)0x5c8770) {
            const char* sMinor = Num_Format(num2, 1, (double)minor, 10, 1, 0, 0);
            const char* sMajor = Num_Format(num, 1, (double)major, 10, 1, 0, 0);
            Report(r, 0x93, sMajor, sMinor);
            Report(r, 0x94, Num_Format(num, 3, (double)psMax, 10, 1, 0, 0));
            static const uint32_t pixel[2][3] = { { 0x5ddac8, 0x2627, 0x262b }, { 0x5ddaac, 0x262f, 0x2633 } };
            static const uint32_t vertex[3][3] = { { 0x5dda90, 0x2617, 0x261b }, { 0x5dda74, 0x261f, 0x2623 }, { 0x5dda58, 0x260f, 0x2613 } };
            bool ok = true;
            for (const auto& p : pixel) {
                if (AssembleShader((const char*)p[0], (void**)(r + p[1])) ||
                    Com(F<void*>(r, 0x25df), 0x15c, Com<void*>(F<void*>(r, p[1]), 0xc), (void*)(r + p[2]))) {
                    Render_ReleaseBuffers(r);
                    ok = false;
                    break;
                }
            }
            if (ok && caps[0xc4 / 4]) {
                uint32_t decl[5] = { 0x20000000, 0x40020000, 0x40020003, 0x40010007, 0xffffffff };
                for (const auto& v : vertex) {
                    if (AssembleShader((const char*)v[0], (void**)(r + v[1])) ||
                        Com(F<void*>(r, 0x25df), 0x12c, (void*)decl, Com<void*>(F<void*>(r, v[1]), 0xc), (void*)(r + v[2]), 0u)) {
                        Render_ReleaseBuffers(r);
                        ok = false;
                        break;
                    }
                }
                if (ok) Eng()[0x11422d] = 1;
            }
        }
    }
    uint32_t gamma = F<uint32_t>(F<uint8_t*>(r, 0x83c), 0x2f);
    if (gamma != 0x3f800000) Render_SetGamma(r, 0, *(float*)&gamma);
    ((void(__stdcall*)(uint32_t, void*))0x570fbd)(0, r + 0x33ab);
    Sound_BuildCapsReport(r);
    Render_BlankFrames(r);
    r[0x254d] = 0;
    F<int32_t>(r, 4) = 4;
    if (F<int32_t>(Eng(), 0x11423f) > 0x100 || F<int32_t>(Eng(), 0x11423b) > 0x100) F<int32_t>(r, 4) = 5;
    return 0;
}
RT3_REPLACE(0x5492d0, Render_InitDevice);

// 0x54a190. Shuts the renderer down: stages, the shadow texture (+0x33ab),
// buffers, textures, saved viewports, then the device (+0x25df) and Direct3D
// (+0x25db); fatal (strings 0x9b/0x9c) when still referenced. Returns 0 (-1 when
// the device leaks).
RT3_NOINLINE int32_t __fastcall Render_Shutdown(uint8_t* r) {
    Render_FreeStages(r);
    if (void* p = F<void*>(r, 0x33ab))
        if (Release(p)) Fatal_Exit(GetString(0x9b));
    Render_ReleaseBuffers(r);
    Render_ReleaseTextures(r);
    if (F<void*>(r, 0x25e3)) Free(F<void*>(r, 0x25e3));
    if (F<void*>(r, 0x25e7)) Free(F<void*>(r, 0x25e7));
    if (void* dev = F<void*>(r, 0x25df)) {
        if (Release(dev)) {
            if (void* d3d = F<void*>(r, 0x25db)) Release(d3d);
            Fatal_Exit(GetString(0x9b));
            return -1;
        }
    }
    if (void* d3d = F<void*>(r, 0x25db))
        if (Release(d3d)) Fatal_Exit(GetString(0x9c));
    F<void*>(r, 0x2643) = nullptr;
    F<void*>(r, 0x25df) = nullptr;
    F<void*>(r, 0x25db) = nullptr;
    F<void*>(g_engine, 8) = nullptr;
    return 0;
}
RT3_REPLACE(0x54a190, Render_Shutdown);

// 0x54a280. Sets the fog (density `density`, colour +0x2510 scaled by r/g/b) when
// the device supports it (caps +0x83c +0x2e): off when r <= 0; table or vertex fog
// (caps +0x1f), linear from start to end (caps +0x20, from +0x2513) or exponential.
RT3_NOINLINE void __fastcall Renderer_SetFog(uint8_t* r, int, float density, float cr, float cg, float cb) {
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

// 0x54a500. Draws renderer r's terrain chunks (+0x2667 of them, 0x2d bytes
// each from +0x2673, chained by +0x267b, index buffers +0x31ab) unlit,
// without z writes or fog: on the fixed pipeline (+0x260f clear) after
// filling the shared vertex buffer +0x2663 with the `count` points at
// 0xccd828 and their texture coordinates (map size x 0x5c, offset
// +0x36df/+0x36e3, scale +0x36e7/+0x36eb); with the vertex shader +0x2613
// otherwise, the scale going to constant 4 (stream +0x265b).
void __fastcall Terrain_DrawChunks(uint8_t* r, int, int32_t count) {
    if (!F<int32_t>(r, 0x2667)) return;
    DevRenderState(r, 0x89, 0);
    if (r[0x2549]) {
        r[0x2549] = 0;
        DevRenderState(r, 0x1c, 0);
    }
    memset(r + 0x254f, 0, 0x40);
    DevRenderState(r, 0xe, 0);
    float su = (float)(Fl_206(0x5c85b4) / (Terrain_Rows((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine))) * 0x5c));
    float sv = (float)(Fl_206(0x5c85b4) / (Terrain_Cols((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine))) * 0x5c));
    DevRenderState(r, 0x1b, F<uint8_t*>(r, 0x83c)[0x21] & 1);
    Render_AlphaTestOn(r, 0, 1);
    Renderer_SetTexture(r, 0, 0, F<uint8_t*>(Engine_Get1C(hook::As<void*>(g_engine)), 0x478));
    void* dev = F<void*>(r, 0x25df);
    if (!F<int32_t>(r, 0x260f)) {
        Render_PushStages(r, 0, nullptr);
        Render_SetColorOp(r, 0, 0, 2, F<uint32_t>(r, 4), 1, 1);
        Render_SetAlphaOp(r, 0, 0, 2, 4, 1, 1);
        DevSlot<int32_t(__stdcall*)(void*, uint32_t)>(dev, 0x130)(dev, 0x142);
        dev = F<void*>(r, 0x25df);
        DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t)>(dev, 0x14c)(dev, 0, F<void*>(r, 0x2663), 0x18);
        ++g_ccd824;
        void* vb = F<void*>(r, 0x2663);
        float* out = nullptr;
        DevSlot<int32_t(__stdcall*)(void*, uint32_t, uint32_t, float**, uint32_t)>(vb, 0x2c)(vb, 0, 0, &out, 0x2800);
        const float* p = (const float*)0xccd828;
        for (int32_t i = 0; i < count; ++i, p += 3, out += 6) {
            out[0] = p[0];
            out[1] = p[1];
            out[2] = p[2];
            *(uint32_t*)&out[3] = 0xffffffff;
            out[4] = (float)(((double)su * p[0] + F<float>(r, 0x36df)) * F<float>(r, 0x36e7));
            out[5] = (float)(((double)sv * p[2] + F<float>(r, 0x36e3)) * F<float>(r, 0x36eb));
        }
        --g_ccd824;
        vb = F<void*>(r, 0x2663);
        DevSlot<int32_t(__stdcall*)(void*)>(vb, 0x30)(vb);
    } else {
        DevSlot<int32_t(__stdcall*)(void*, uint32_t)>(dev, 0x130)(dev, F<uint32_t>(r, 0x2613));
        void* stream = F<void*>(r, 0x265b);
        if (!stream) {
            F<void*>(r, 0x2637) = nullptr;
        } else {
            if (F<void*>(r, 0x2637) != stream) {
                dev = F<void*>(r, 0x25df);
                DevSlot<int32_t(__stdcall*)(void*, uint32_t)>(dev, 0x130)(dev, 0x112);
                dev = F<void*>(r, 0x25df);
                DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t)>(dev, 0x14c)(dev, 0, stream, 0x20);
                F<void*>(r, 0x2637) = stream;
            }
            F<int32_t>(r, 0x263b) = 0;
        }
        typedef int32_t(__stdcall * SetConst)(void*, uint32_t, const void*, uint32_t);
        dev = F<void*>(r, 0x25df);
        DevSlot<SetConst>(dev, 0x13c)(dev, 0, r + 0x35af, 4);
        float c4[4] = { su, sv, 0.0f, 0.0f };
        dev = F<void*>(r, 0x25df);
        DevSlot<SetConst>(dev, 0x13c)(dev, 4, c4, 1);
        float c5[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        dev = F<void*>(r, 0x25df);
        DevSlot<SetConst>(dev, 0x13c)(dev, 5, c5, 1);
        float c6[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        dev = F<void*>(r, 0x25df);
        DevSlot<SetConst>(dev, 0x13c)(dev, 6, c6, 1);
    }
    for (int32_t i = 0; i < F<int32_t>(r, 0x2667); ++i) {
        if (r[0x254f + i] || i == -1) continue;
        int32_t j = i;
        do {
            uint8_t* c = r + j * 0x2d;
            int32_t prims = F<int32_t>(c, 0x2690);
            if (prims > 0) {
                dev = F<void*>(r, 0x25df);
                DevSlot<int32_t(__stdcall*)(void*, void*, uint32_t)>(dev, 0x154)(dev, F<void*>(r, j * 4 + 0x31ab), 0);
                uint32_t lo = F<uint16_t>(c, 0x2673), hi = F<uint16_t>(c, 0x2675);
                dev = F<void*>(r, 0x25df);
                DevSlot<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, int32_t)>(dev, 0x11c)(dev, 4, lo, hi - lo + 1, 0, prims);
                F<int32_t>(r, 0x3711) += prims;
            }
            r[0x254f + j] = 1;
            j = F<int32_t>(c, 0x267b);
        } while (j != -1);
    }
    DevRenderState(r, 0xe, 1);
    if (!r[0x2549] && F<uint8_t*>(r, 0x83c)[0x2e]) {
        r[0x2549] = 1;
        DevRenderState(r, 0x1c, 1);
    }
    DevRenderState(r, 0x89, 1);
    if (!F<int32_t>(r, 0x260f)) Render_PopStages(r, 0, 1);
}
RT3_REPLACE(0x54a500, Terrain_DrawChunks);

// 0x54ab20. Draws renderer r's terrain chunks with the shader pipeline,
// walking each chain in direction table 0x625414 [dx + dy x 3] (next links
// +0x2687 / +0x267f / +0x2683 / -, / +0x267b). Each chunk's two textures
// (+0x266b, +0x266f) are replaced by tex0 / tex1 when dx / dy is 1; with
// dx / dy 2 only chunks using tex0 / tex1 are drawn, or for `invert` all
// others, those then textured with `extra`. `water` uses the water stages
// (extra on stage 1). Afterwards resets clamping, the shaders and the
// higher texture stages, and restores solid fill in wireframe mode
// (+0x83c +0x19).
void __fastcall Terrain_DrawChunksShaded(uint8_t* r, int, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy, uint8_t water, uint8_t* extra,
                                         uint8_t invert) {
    if (!F<int32_t>(r, 0x2667)) return;
    memset(r + 0x254f, 0, 0x40);
    void* stream = F<void*>(r, 0x265b);
    void* dev;
    if (!stream) {
        F<void*>(r, 0x2637) = nullptr;
    } else {
        if (F<void*>(r, 0x2637) != stream) {
            dev = F<void*>(r, 0x25df);
            DevSlot<int32_t(__stdcall*)(void*, uint32_t)>(dev, 0x130)(dev, 0x112);
            dev = F<void*>(r, 0x25df);
            DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t)>(dev, 0x14c)(dev, 0, stream, 0x20);
            F<void*>(r, 0x2637) = stream;
        }
        F<int32_t>(r, 0x263b) = 0;
    }
    if (F<uint8_t*>(r, 0x83c)[0x19]) DevRenderState(r, 8, 2);
    if (water) Renderer_SetupWaterStages(r);
    else Renderer_SetupStages(r);
    if (g_engine[0x114252]) {
        DevRenderState(r, 0xf, 0);
        DevRenderState(r, 0x18, 0);
    }
    DevRenderState(r, 0x1b, 0);
    Renderer_SetShaderConstants(r);
    int32_t dir = hook::Global<int32_t>(0x625414 + (dx + dy * 3) * 4);
    for (int32_t i = 0; i < F<int32_t>(r, 0x2667); ++i) {
        if (r[0x254f + i]) continue;
        uint8_t* first = r + i * 0x2d;
        uint8_t* t0 = dx == 1 ? tex0 : F<uint8_t*>(first, 0x266b);
        uint8_t* t1 = dy == 1 ? tex1 : F<uint8_t*>(first, 0x266f);
        bool bind = true;
        int32_t j = i;
        if (j == -1) continue;
        do {
            bool ok = true;
            bool skip = false;
            if (!invert) {
                if (dx == 2 && t0 != tex0) ok = false;
                if (dy == 2 && t1 != tex1) skip = true;
            } else {
                if (dx == 2) {
                    if (t0 == tex0) ok = false;
                    t0 = extra;
                }
                if (dy == 2) {
                    if (t1 == tex1) ok = false;
                    t1 = extra;
                }
            }
            if (!skip && ok) {
                if (bind) {
                    if (!water) {
                        if (!extra) {
                            Renderer_SetTexture(r, 0, 0, t0);
                            Renderer_SetTexture(r, 0, 1, t1);
                        } else {
                            Renderer_SetTexture(r, 0, 1, t1);
                            Renderer_SetTexture(r, 0, 2, extra);
                            Renderer_SetTexture(r, 0, 0, t0);
                        }
                    } else {
                        Renderer_SetTexture(r, 0, 1, extra);
                        Renderer_SetTexture(r, 0, 2, t0);
                        Renderer_SetTexture(r, 0, 0, t1);
                    }
                    bind = false;
                }
                uint8_t* c = r + j * 0x2d;
                int32_t prims = F<int32_t>(c, 0x2690);
                if (prims > 0) {
                    dev = F<void*>(r, 0x25df);
                    DevSlot<int32_t(__stdcall*)(void*, void*, uint32_t)>(dev, 0x154)(dev, F<void*>(r, j * 4 + 0x31ab), 0);
                    uint32_t lo = F<uint16_t>(c, 0x2673);
                    uint32_t nv = F<uint16_t>(c, 0x2675) - lo + 1;
                    g_ccd818 += nv;
                    dev = F<void*>(r, 0x25df);
                    DevSlot<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, int32_t)>(dev, 0x11c)(dev, 4, lo, nv, 0, prims);
                    ++g_ccd81c;
                    F<int32_t>(r, 0x3711) += prims;
                }
            }
            r[0x254f + j] = 1;
            switch ((uint32_t)dir) {
            case 0: j = F<int32_t>(r + j * 0x2d, 0x2687); break;
            case 1: j = F<int32_t>(r + j * 0x2d, 0x267f); break;
            case 2: j = F<int32_t>(r + j * 0x2d, 0x2683); break;
            case 4: j = F<int32_t>(r + j * 0x2d, 0x267b); break;
            default: break;
            }
        } while (j != -1);
    }
    dev = F<void*>(r, 0x25df);
    typedef int32_t(__stdcall * Tss)(void*, uint32_t, uint32_t, uint32_t);
    if (!water) {
        DevSlot<Tss>(dev, 0xfc)(dev, 1, 0xd, 1);
        dev = F<void*>(r, 0x25df);
        DevSlot<Tss>(dev, 0xfc)(dev, 1, 0xe, 1);
    } else {
        DevSlot<Tss>(dev, 0xfc)(dev, 0, 0xd, 1);
        dev = F<void*>(r, 0x25df);
        DevSlot<Tss>(dev, 0xfc)(dev, 0, 0xe, 1);
        dev = F<void*>(r, 0x25df);
        DevSlot<Tss>(dev, 0xfc)(dev, 2, 0xd, 1);
        dev = F<void*>(r, 0x25df);
        DevSlot<Tss>(dev, 0xfc)(dev, 2, 0xe, 1);
    }
    dev = F<void*>(r, 0x25df);
    DevSlot<int32_t(__stdcall*)(void*, uint32_t)>(dev, 0x160)(dev, 0);
    dev = F<void*>(r, 0x25df);
    DevSlot<int32_t(__stdcall*)(void*, uint32_t)>(dev, 0x130)(dev, 0x112);
    for (uint32_t st = 1; st < 4; ++st) {
        if (F<int32_t>(r, 0x258f) <= (int32_t)st) continue;
        if (F<void*>(F<uint8_t*>(r, 0x2593), st * 4)) {
            ++F<int32_t>(r, 0x370d);
            dev = F<void*>(r, 0x25df);
            DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*)>(dev, 0xf4)(dev, st, nullptr);
        }
        F<void*>(F<uint8_t*>(r, 0x2593), st * 4) = nullptr;
    }
    if (F<uint8_t*>(r, 0x83c)[0x19]) DevRenderState(r, 8, 3);
}
RT3_REPLACE(0x54ab20, Terrain_DrawChunksShaded);

// 0x54b030. Two-texture variant of Terrain_DrawChunksShaded: chunks keep
// their textures unless dx / dy is 1 (dy only off `water`), dx / dy 2
// draws only chunks using tex0 / tex1, stage 0 gets the second texture
// unless `water`. Afterwards resets stages 0 and 1 (clamping, texture
// coordinates, identity texture transform) and the higher texture stages.
void __fastcall Terrain_DrawChunksPaired(uint8_t* r, int, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy, uint8_t water) {
    if (!F<int32_t>(r, 0x2667)) return;
    memset(r + 0x254f, 0, 0x40);
    void* stream = F<void*>(r, 0x265b);
    void* dev;
    if (!stream) {
        F<void*>(r, 0x2637) = nullptr;
    } else {
        if (F<void*>(r, 0x2637) != stream) {
            dev = F<void*>(r, 0x25df);
            DevSlot<int32_t(__stdcall*)(void*, uint32_t)>(dev, 0x130)(dev, 0x112);
            dev = F<void*>(r, 0x25df);
            DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t)>(dev, 0x14c)(dev, 0, stream, 0x20);
            F<void*>(r, 0x2637) = stream;
        }
        F<int32_t>(r, 0x263b) = 0;
    }
    if (F<uint8_t*>(r, 0x83c)[0x19]) DevRenderState(r, 8, 2);
    if (water) Renderer_SetupWaterStages(r);
    else Renderer_SetupStages(r);
    int32_t dir = hook::Global<int32_t>(0x625414 + (dx + dy * 3) * 4);
    for (int32_t i = 0; i < F<int32_t>(r, 0x2667); ++i) {
        if (r[0x254f + i]) continue;
        uint8_t* first = r + i * 0x2d;
        uint8_t* t0 = dx == 1 ? tex0 : F<uint8_t*>(first, 0x266b);
        uint8_t* t1 = (!water && dy == 1) ? tex1 : F<uint8_t*>(first, 0x266f);
        bool bind = true;
        int32_t j = i;
        if (j == -1) continue;
        do {
            bool ok = !(dx == 2 && t0 != tex0);
            if (!(dy == 2 && t1 != tex1) && ok) {
                if (bind) {
                    if (water) {
                        Renderer_SetTexture(r, 0, 0, t0);
                        Renderer_SetTexture(r, 0, 1, t1);
                    } else {
                        Renderer_SetTexture(r, 0, 0, t1);
                        Renderer_SetTexture(r, 0, 1, t0);
                    }
                    bind = false;
                }
                uint8_t* c = r + j * 0x2d;
                int32_t prims = F<int32_t>(c, 0x2690);
                if (prims > 0) {
                    dev = F<void*>(r, 0x25df);
                    DevSlot<int32_t(__stdcall*)(void*, void*, uint32_t)>(dev, 0x154)(dev, F<void*>(r, j * 4 + 0x31ab), 0);
                    uint32_t lo = F<uint16_t>(c, 0x2673);
                    uint32_t nv = F<uint16_t>(c, 0x2675) - lo + 1;
                    g_ccd818 += nv;
                    dev = F<void*>(r, 0x25df);
                    DevSlot<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, int32_t)>(dev, 0x11c)(dev, 4, lo, nv, 0, prims);
                    ++g_ccd81c;
                    F<int32_t>(r, 0x3711) += prims;
                }
            }
            r[0x254f + j] = 1;
            j = Chunk_Next(r, j, dir);
        } while (j != -1);
    }
    if (F<uint8_t*>(r, 0x83c)[0x19]) DevRenderState(r, 8, 3);
    typedef int32_t(__stdcall * Tss)(void*, uint32_t, uint32_t, uint32_t);
    typedef int32_t(__stdcall * Xform)(void*, uint32_t, const float*);
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 0, 0xd, 1);
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 0, 0xe, 1);
    float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 0, 0xb, 0);
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 0, 0x18, 0);
    dev = F<void*>(r, 0x25df);
    DevSlot<Xform>(dev, 0x94)(dev, 0x10, m);
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 1, 0xb, 1);
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 1, 0x18, 0);
    dev = F<void*>(r, 0x25df);
    DevSlot<Xform>(dev, 0x94)(dev, 0x10, m);
    for (uint32_t st = 1; st < 4; ++st) {
        if (F<int32_t>(r, 0x258f) <= (int32_t)st) continue;
        if (F<void*>(F<uint8_t*>(r, 0x2593), st * 4)) {
            ++F<int32_t>(r, 0x370d);
            dev = F<void*>(r, 0x25df);
            DevSlot<int32_t(__stdcall*)(void*, uint32_t, void*)>(dev, 0xf4)(dev, st, nullptr);
        }
        F<void*>(F<uint8_t*>(r, 0x2593), st * 4) = nullptr;
    }
    if (F<uint8_t*>(r, 0x83c)[0x19]) DevRenderState(r, 8, 3);
}
RT3_REPLACE(0x54b030, Terrain_DrawChunksPaired);

// 0x54b4e0. Fixed-pipeline variant of the terrain chunk draw: first runs the
// `vertices` chunk vertices through ProcessVertices into +0x265f (on
// failure only selects the raw stream +0x265b, stride 0x24, and shader
// 0x152); multitexture cards (0x114250) get the second texture on stage 1
// with an identity texture transform, the others draw the second texture
// only. Chunk selection as in Terrain_DrawChunksPaired.
void __fastcall Terrain_DrawChunksFixed(uint8_t* r, int, int32_t vertices, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy) {
    if (!F<int32_t>(r, 0x2667)) return;
    memset(r + 0x254f, 0, 0x40);
    if (F<uint8_t*>(r, 0x83c)[0x19]) DevRenderState(r, 8, 2);
    void* stream = F<void*>(r, 0x265b);
    void* dev;
    typedef int32_t(__stdcall * SetShader)(void*, uint32_t);
    typedef int32_t(__stdcall * SetStream)(void*, uint32_t, void*, uint32_t);
    typedef int32_t(__stdcall * Tss)(void*, uint32_t, uint32_t, uint32_t);
    if (!stream) {
        F<void*>(r, 0x2637) = nullptr;
    } else {
        if (F<void*>(r, 0x2637) != stream) {
            dev = F<void*>(r, 0x25df);
            DevSlot<SetShader>(dev, 0x130)(dev, 0x112);
            dev = F<void*>(r, 0x25df);
            DevSlot<SetStream>(dev, 0x14c)(dev, 0, stream, 0x20);
            F<void*>(r, 0x2637) = stream;
        }
        F<int32_t>(r, 0x263b) = 0;
    }
    dev = F<void*>(r, 0x25df);
    DevSlot<SetShader>(dev, 0x130)(dev, 0x112);
    if (g_engine[0x114252]) {
        DevRenderState(r, 0xf, 0);
        DevRenderState(r, 0x18, 0);
    }
    DevRenderState(r, 0x1b, 0);
    Renderer_SetupStages(r);
    int32_t dir = hook::Global<int32_t>(0x625414 + (dx + dy * 3) * 4);
    dev = F<void*>(r, 0x25df);
    if (DevSlot<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, void*, uint32_t)>(dev, 0x128)(dev, 0, 0, (uint32_t)vertices, F<void*>(r, 0x265f), 0)) {
        dev = F<void*>(r, 0x25df);
        DevSlot<SetStream>(dev, 0x14c)(dev, 0, F<void*>(r, 0x265b), 0x24);
        dev = F<void*>(r, 0x25df);
        DevSlot<SetShader>(dev, 0x130)(dev, 0x152);
        return;
    }
    bool multi = g_engine[0x114250] != 0;
    dev = F<void*>(r, 0x25df);
    if (multi) {
        DevSlot<Tss>(dev, 0xfc)(dev, 1, 0xb, 1);
        float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        dev = F<void*>(r, 0x25df);
        DevSlot<Tss>(dev, 0xfc)(dev, 1, 0x18, 0);
        dev = F<void*>(r, 0x25df);
        DevSlot<int32_t(__stdcall*)(void*, uint32_t, const float*)>(dev, 0x94)(dev, 0x11, m);
        dev = F<void*>(r, 0x25df);
        DevSlot<SetShader>(dev, 0x130)(dev, 0x2c4);
        dev = F<void*>(r, 0x25df);
        DevSlot<SetStream>(dev, 0x14c)(dev, 0, F<void*>(r, 0x265f), 0x28);
    } else {
        DevSlot<SetShader>(dev, 0x130)(dev, 0x1c4);
        dev = F<void*>(r, 0x25df);
        DevSlot<SetStream>(dev, 0x14c)(dev, 0, F<void*>(r, 0x265f), 0x20);
    }
    for (int32_t i = 0; i < F<int32_t>(r, 0x2667); ++i) {
        if (r[0x254f + i]) continue;
        uint8_t* first = r + i * 0x2d;
        uint8_t* t0 = dx == 1 ? tex0 : F<uint8_t*>(first, 0x266b);
        uint8_t* t1 = dy == 1 ? tex1 : F<uint8_t*>(first, 0x266f);
        bool bind = true;
        int32_t j = i;
        if (j == -1) continue;
        do {
            bool ok = !(dx == 2 && t0 != tex0);
            if (!(dy == 2 && t1 != tex1) && ok) {
                if (bind) {
                    if (g_engine[0x114250]) {
                        Renderer_SetTexture(r, 0, 0, t1);
                        Renderer_SetTexture(r, 0, 1, t0);
                    } else {
                        Renderer_SetTexture(r, 0, 0, t1);
                        Render_DropStageTexture(r, 1);
                    }
                    bind = false;
                }
                uint8_t* c = r + j * 0x2d;
                int32_t prims = F<int32_t>(c, 0x2690);
                if (prims > 0) {
                    dev = F<void*>(r, 0x25df);
                    DevSlot<int32_t(__stdcall*)(void*, void*, uint32_t)>(dev, 0x154)(dev, F<void*>(r, j * 4 + 0x31ab), 0);
                    uint32_t lo = F<uint16_t>(c, 0x2673);
                    uint32_t nv = F<uint16_t>(c, 0x2675) - lo + 1;
                    g_ccd818 += nv;
                    dev = F<void*>(r, 0x25df);
                    DevSlot<int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, int32_t)>(dev, 0x11c)(dev, 4, lo, nv, 0, prims);
                    ++g_ccd81c;
                    F<int32_t>(r, 0x3711) += prims;
                }
            }
            r[0x254f + j] = 1;
            j = Chunk_Next(r, j, dir);
        } while (j != -1);
    }
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 0, 0x18, 0);
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 1, 0x18, 0);
    Render_DropStageTexture(r, 1);
    Render_DropStageTexture(r, 2);
    if (F<uint8_t*>(r, 0x83c)[0x19]) DevRenderState(r, 8, 3);
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 0, 0xd, 1);
    dev = F<void*>(r, 0x25df);
    DevSlot<Tss>(dev, 0xfc)(dev, 0, 0xe, 1);
    dev = F<void*>(r, 0x25df);
    DevSlot<SetShader>(dev, 0x130)(dev, 0x152);
}
RT3_REPLACE(0x54b4e0, Terrain_DrawChunksFixed);

void __fastcall Render_BindVb2643(uint8_t* r) { BindFixed(r, 0x2643, 0x112, 0x20); }
RT3_REPLACE(0x54bab0, Render_BindVb2643);

void __fastcall Render_BindVb263f(uint8_t* r) { BindFixed(r, 0x263f, 0x112, 0x20); }
RT3_REPLACE(0x54bb10, Render_BindVb263f);

void __fastcall Render_BindVb2647(uint8_t* r) { BindFixed(r, 0x2647, 0x142, 0x18); }
RT3_REPLACE(0x54bb70, Render_BindVb2647);

void __fastcall Render_BindPair264f(uint8_t* r) { Render_SetStreams(r, 0, F<void*>(r, 0x264f), 0x18, F<void*>(r, 0x2653)); }
RT3_REPLACE(0x54bbd0, Render_BindPair264f);

void __fastcall Render_BindPair2647(uint8_t* r) { Render_SetStreams(r, 0, F<void*>(r, 0x2647), 0x18, F<void*>(r, 0x264b)); }
RT3_REPLACE(0x54bbf0, Render_BindPair2647);

// 0x54bc10. Constructs renderer r: clears its state (config +0x83c in the
// engine, scales 1, colour 0xff000000, fog 46, the 0x31ab / 0x266b tables),
// creates the device (`fullScreen`; a failure shuts down and is fatal), sets
// the far bounds, the identity matrix +0x346f (shared with the engine) and
// the 64 light slots, then the sun table (or the saved copy 0xccbb68) and
// tells the camera.
uint8_t* __fastcall Render_Construct(uint8_t* r, int, int32_t, uint8_t fullScreen) {
    F<uint8_t*>(r, 0x83c) = Eng() + 0x11468a;
    static const uint16_t zero[] = { 0x2653, 0x25ef, 0x25f3, 0x253f, 0x2543, 0x264b, 0x2647, 0x2643, 0x263f, 0x265b, 0x264f };
    F<uint32_t>(r, 0x2653) = 0;
    F<uint32_t>(r, 0x25ef) = 0;
    F<uint32_t>(r, 0x25f3) = 0;
    F<uint32_t>(r, 0x25eb) = 0x3f800000;
    for (int k = 3; k < 11; ++k) F<uint32_t>(r, zero[k]) = 0;
    F<uint32_t>(r, 0x25fb) = 0x3f800000;
    F<uint32_t>(r, 0x2603) = 0x3f800000;
    F<uint32_t>(r, 0x25f7) = 0x3f800000;
    F<uint32_t>(r, 0x25ff) = 0x3f800000;
    static const uint16_t zero2[] = { 0x2663, 0x265f, 0x25df, 0x25db, 0x25e3, 0x25e7, 0x2637, 0x263b, 0x260f, 0x2617, 0x261f, 0x2627, 0x262f };
    for (uint16_t off : zero2) F<uint32_t>(r, off) = 0;
    r[0x36f7] = 1;
    F<uint32_t>(r, 0x259f) = 0;
    F<uint32_t>(r, 0x259b) = 0;
    r[0x254e] = 0;
    r[0x36f8] = 0;
    F<int32_t>(r, 0x25ab) = -1;
    F<uint32_t>(r, 0x262b) = 0;
    F<uint32_t>(r, 0x2633) = 0;
    F<uint32_t>(r, 0x2613) = 0;
    F<uint32_t>(r, 0x36fd) = 0x42380000;
    F<uint32_t>(r, 0x258f) = 0;
    F<uint32_t>(r, 0x25a3) = 0xff000000;
    F<uint32_t>(r, 0x2667) = 0;
    memset(r + 0x31ab, 0, 0x40 * 4);
    memset(r + 0x266b, 0, 0x2d0 * 4);
    if (Render_InitDevice(r, 0, fullScreen) < 0) {
        Render_Shutdown(r);
        Fatal_Exit(GetString(0x67));
    }
    for (uint32_t off = 0x36af; off <= 0x36c3; off += 4) F<uint32_t>(r, off) = 0xc6fa0000;
    for (uint32_t off = 0x36c7; off <= 0x36cf; off += 4) F<uint32_t>(r, off) = 0xc89c4000;
    float* m = (float*)(r + 0x346f);
    F<float*>(Eng(), 0x1141f1) = m;
    r[0x254c] = 1;
    memset(m, 0, 0x40);
    F<uint32_t>(r, 0x34ab) = 0x3f800000;
    F<uint32_t>(r, 0x3497) = 0x3f800000;
    F<uint32_t>(r, 0x3483) = 0x3f800000;
    *(uint32_t*)m = 0x3f800000;
    Engine_SetC(Eng(), 0, (int32_t)(uintptr_t)r);
    uint8_t* slot = r + 0x267b;
    for (int32_t k = 1; k <= 0x40; ++k, slot += 0x2d) {
        slot[0x10] = 0;
        F<int32_t>(slot, 0) = -1;
        F<int32_t>(slot, 4) = -1;
        F<int32_t>(slot, 8) = -1;
        F<int32_t>(slot, 0xc) = k;
    }
    if (!g_ccd820) Sky_BuildSunTable(r);
    else memcpy(r + 0x840, (const void*)0xccbb68, 0x72c * 4);
    if (Eng() && F<void*>(Eng(), 0x28)) ((void(__thiscall*)(void*))(*(void***)F<void*>(Eng(), 0x28))[0x1c / 4])(F<void*>(Eng(), 0x28));
    return r;
}
RT3_REPLACE(0x54bc10, Render_Construct);

// 0x54be60. Saves the renderer settings (+0x840, 0x72c dwords) to 0xccbb68 (flag
// 0xccd820), shuts the renderer down and clears engine +0xc.
RT3_NOINLINE void __fastcall Render_SaveAndShutdown(uint8_t* r) {
    g_ccd820 = 1;
    memcpy((void*)0xccbb68, r + 0x840, 0x72c * 4);
    Render_Shutdown(r);
    Engine_SetC(hook::As<void*>(g_engine), 0, 0);
}
RT3_REPLACE(0x54be60, Render_SaveAndShutdown);

// 0x54bea0. Lighting for time of day `secs` (0..86400) and night amount `night`:
// blends the two nearest of the 144 keyframes (+0x840, 0x33 bytes each; index
// +0x36f3) into the sun / ambient terms (+0x24f0..+0x251f, fog colour bytes
// +0x2510..), darkened by the night factor (+0x25af), then sets the directional
// light (sun direction, diffuse, ambient, specular when +0x11422e), the fog colour
// +0x25a3 and the fog. Only when the device does lighting (+0x83c +0x22).
void __fastcall Renderer_SetTimeOfDay(uint8_t* r, int, int32_t secs, int32_t night) {
    if (secs < 0 || secs > 0x15180 || !F<uint8_t*>(r, 0x83c)[0x22]) return;
    double n = (double)night * Fl_201(0x5ddb08);
    F<float>(r, 0x25af) = (float)n;
    if (n > D(0x5c8770)) F<uint32_t>(r, 0x25af) = 0x3f800000;
    else if (n < D(0x5c8608)) F<uint32_t>(r, 0x25af) = 0;
    double inv = Fl_201(0x5c85b4) - (double)F<float>(r, 0x25af);
    double t = (double)secs * Fl_201(0x5dcfb8) * Fl_201(0x5cece8);
    int32_t i = Ftol(t);
    int32_t j = (i + 1) % 0x90;
    F<int32_t>(r, 0x36f3) = i;
    double frac = t - (double)i;
    double w0 = Fl_201(0x5c85b4) - frac;
    uint8_t* a = r + i * 0x33;
    uint8_t* b = r + j * 0x33;
    auto mix = [&](uint32_t off) { return frac * F<float>(b, off) + w0 * F<float>(a, off); };
    auto night1 = [&](uint32_t off, double k) {
        F<float>(r, off) = (float)((double)F<float>(r, 0x25af) * F<float>(r, off) * k + inv * F<float>(r, off));
    };
    F<float>(r, 0x24f0) = (float)mix(0x840);
    F<float>(r, 0x24f8) = (float)mix(0x848);
    night1(0x24f0, D(0x5ddb00));
    night1(0x24f8, D(0x5c8758));
    F<float>(r, 0x24f4) = (float)mix(0x844);
    F<float>(r, 0x24fc) = (float)mix(0x84c);
    double x = (double)F<float>(r, 0x25af) * F<float>(r, 0x24f0) * D(0x5ddb00) + inv * F<float>(r, 0x24f0);
    F<float>(r, 0x24f0) = (float)x;
    double nf = F<float>(r, 0x25af);
    F<float>(r, 0x24f8) = (float)((double)F<float>(r, 0x24f8) * nf * D(0x5c8758) + inv * F<float>(r, 0x24f8));
    float l0c = (float)((double)F<float>(r, 0x2500) * nf * D(0x5c8c18));
    double y = x * D(0x5c8880) * F<float>(r, 0x25af);
    float l14 = (float)y;
    float l10 = (float)(D(0x5c8650) * y);
    float l18 = (float)(y * D(0x5c85a0));
    F<float>(r, 0x2500) = (float)mix(0x850);
    F<float>(r, 0x2504) = (float)mix(0x854);
    F<float>(r, 0x2508) = (float)mix(0x858);
    F<float>(r, 0x250c) = (float)mix(0x85c);
    F<float>(r, 0x2504) = (float)(inv * F<float>(r, 0x2504) + l18);
    F<float>(r, 0x2508) = (float)(inv * F<float>(r, 0x2508) + l14);
    F<float>(r, 0x250c) = (float)(inv * F<float>(r, 0x250c) + l10);
    F<float>(r, 0x2500) = (float)(inv * F<float>(r, 0x2500) + l0c);
    F<float>(r, 0x2513) = (float)(mix(0x863) * inv * Fl_201(0x5d1808) + (double)F<float>(r, 0x25af) * Fl_201(0x5ddaf8));
    F<float>(r, 0x2517) = (float)mix(0x867);
    F<float>(r, 0x251b) = (float)mix(0x86b);
    F<float>(r, 0x251f) = (float)mix(0x86f);
    for (int32_t k = 0; k < 3; ++k)
        r[0x2510 + k] = (uint8_t)Ftol((double)b[0x860 + k] * frac + (double)a[0x860 + k] * w0);
    r[0x2510] = (uint8_t)Ftol((double)r[0x2510] * inv + (double)l18 * Fl_201(0x5ca0ec));
    r[0x2511] = (uint8_t)Ftol((double)r[0x2511] * inv + (double)l14 * Fl_201(0x5ca0ec));
    r[0x2512] = (uint8_t)Ftol((double)r[0x2512] * inv + (double)l10 * Fl_201(0x5ca0ec));
    float light[0x1a] = {};
    F<uint32_t>(light, 0) = 3;
    if (g_engine[0x11422e]) {
        float sp = (float)((double)F<float>(r, 0x25fb) * F<float>(r, 0x24f4) * D(0x5c8758));
        for (int32_t k = 0; k < 4; ++k) light[5 + k] = sp;
    }
    float diff = (float)((double)F<float>(r, 0x2603) * F<float>(r, 0x24fc));
    for (int32_t k = 0; k < 4; ++k) light[1 + k] = diff;
    F<uint32_t>(light, 0x48) = F<uint32_t>(r, 0x251f);
    float amb = (float)((double)F<float>(r, 0x25fb) * F<float>(r, 0x24f4));
    F<uint32_t>(light, 0x4c) = 0x447a0000;
    F<uint32_t>(light, 0x40) = F<uint32_t>(r, 0x2517);
    for (int32_t k = 0; k < 4; ++k) light[9 + k] = amb;
    F<float>(light, 0x44) = -F<float>(r, 0x251b);
    uint8_t* d = F<uint8_t*>(r, 0x25df);
    (*(int32_t(__stdcall**)(void*, uint32_t, const void*))(*(uint8_t**)d + 0xb0))(d, 0, light);
    F<uint32_t>(r, 0x25a3) = (((0xffffff00u | r[0x2510]) << 8 | r[0x2511]) << 8) | r[0x2512];
    Renderer_SetFog(r, 0, 1.0f, 1.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x54bea0, Renderer_SetTimeOfDay);

}  // namespace rt3
