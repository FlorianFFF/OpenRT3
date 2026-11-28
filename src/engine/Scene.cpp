// Scene.cpp - RT3.exe 0x524000-0x52c000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include "core/imports.h"
#include "hook/keepregs.h"
#include "game/collection.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Engine_Set1146f6(void* e, int, int32_t v);
void __fastcall Fade_StartFast();

void __fastcall Scene_VisibleBounds(uint8_t* s, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1, uint8_t* terrain,
                                    int32_t shift);

void __fastcall Render_GetFog(uint8_t* r, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d, uint32_t* e,
                              uint32_t* f, uint32_t* g);

void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void* __fastcall Terrain_HeightsSlot(uint8_t* r);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Engine_ResetTextureQueue(void* e);
void __fastcall TimeOfDay_Split(int32_t v, int32_t* hours, int32_t* minutes, int32_t* seconds);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
void* __fastcall Engine_Get1C(void* e);
uint8_t __fastcall Render_TakeFlag36f7(uint8_t* r, int, uint8_t clear);
int32_t __fastcall Scene_FogDensity(uint8_t* s);
float* __fastcall Vec3_Normalize(float* v);
void __fastcall Obj_GetVec1c(uint8_t* o, int, uint32_t* out);
void __fastcall Obj_GetVec4(uint8_t* o, int, uint32_t* out);
void __fastcall Render_Unproject(uint8_t* r, int, float sx, float sy, float* ox, float* oy, float* oz);
int32_t __fastcall Terrain_Cols(uint8_t* r);
uint32_t __fastcall Render_Get153c(uint8_t* r);
uint32_t __fastcall Render_Get1540(uint8_t* r);
int32_t __fastcall Render_ProjectClip(uint8_t* r, int, float x, float y, float z, float limit, float maxZ, float* out);
void __fastcall Mesh_FaceCamera(uint8_t* o, int, uint8_t* cam, int32_t key, int32_t idx);
void __fastcall Model_EmitTriangles(uint8_t* m, int, uint8_t* out, int32_t* count, uint32_t colour, int32_t frame, float dx, float dy, float dz);
void __fastcall Mesh_EmitTriangles(uint8_t* o, int, uint8_t* verts, int32_t* count, uint32_t color, int32_t lod, float x, float y, float z);
int32_t __fastcall Render_UnlockVb2647(uint8_t* r);
void __fastcall Render_DrawTriangles(uint8_t* r, int, int32_t first, int32_t end);
void* __fastcall Render_LockVb2647(uint8_t* r, int, int32_t first, int32_t count);
void* __fastcall Render_AppendVb2647(uint8_t* r, int, int32_t first, int32_t count);
int32_t __fastcall VisitGrid_Log2(uint8_t* g);
uint8_t* __fastcall VisitGrid_Flags(uint8_t* g);
void __fastcall TerrainPatch_Triangulate(uint8_t* g, int, int32_t x0, int32_t y0);
uint32_t __fastcall Terrain_Get15e9(uint8_t* r);
uint32_t __fastcall Terrain_Get15ed(uint8_t* r);
uint32_t __fastcall Terrain_Get15e5(uint8_t* r);
void* __fastcall Terrain_Slot15f1(uint8_t* r);
uint32_t __fastcall Terrain_Get1655(uint8_t* r);
uint32_t __fastcall Terrain_Get1659(uint8_t* r);
void* __fastcall Render_LockVb2(uint8_t* r);
void __fastcall Render_ResetTextureSlots(uint8_t* r);
uint32_t __fastcall Render_Get1554(uint8_t* r);
void __fastcall Render_SnapToGrid(uint8_t* r, int, int32_t* a, int32_t* b);
uint8_t* __fastcall Render_AcquireVertexSlot(uint8_t* r, int, int32_t key1, int32_t key2);
void __fastcall Render_SetVec2533(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c);
int32_t __fastcall Render_UnlockVb2(uint8_t* r);
void __fastcall Render_ResetVertexSlots(uint8_t* r);
void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);
int32_t __fastcall Render_Project(uint8_t* r, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Render_GetViewport(uint8_t* r, int, uint32_t* sx, uint32_t* sy, uint32_t* ox, uint32_t* oy);
void __fastcall Render_SetLightPair(uint8_t* r, int, int32_t i, int32_t j, float a, float b);
void __fastcall Render_SetLightRange(uint8_t* r, int, int32_t i, float v);
void __fastcall Render_SetPair25f7(uint8_t* r, int, int32_t i, float a, float b);
float __fastcall Render_GetLightValue(uint8_t* r, int, int32_t i, int32_t j);
void __fastcall Render_RestoreTarget(uint8_t* r);
int32_t __fastcall Fx_Intensity(uint8_t* o);
void __fastcall Render_FixTileSeams(uint8_t* r);
uint8_t* __fastcall VisitGrid_Init(uint8_t* g, int, int32_t n);
void __fastcall VisitGrid_Free(uint8_t* g);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
int32_t ClockMs();
void __fastcall Render_UnprojectDir(uint8_t* r, int, float sx, float sy, float* out, const float* eye);
uint8_t __fastcall Obj_GetPair10(uint8_t* o, int, uint32_t* a, uint32_t* b);
void __fastcall Particles_Draw(uint8_t* p, int, int32_t i, float x, float y, float dx, float dy);
double One();
void __fastcall Scene_GetVectors(uint8_t* s, int, uint32_t* a, uint32_t* b, uint32_t* c);
int32_t __fastcall Render_TriangleCount(uint8_t* r);
void __stdcall Textures_MemoryTotals(int32_t* video, int32_t* resident, int32_t* count);
int32_t __fastcall Engine_TotalTextureMem(void* e);
int32_t __fastcall Engine_FreeTextureMem(void* e);
int32_t __fastcall SysInfo_Zero();
void __stdcall Overlay_DrawText(const char* text, int32_t shadow);
int32_t __fastcall Render_GridCell(uint8_t* r, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_Get1619(uint8_t* r, int, int8_t i);
uint32_t __fastcall Terrain_Get165d(uint8_t* r);
void __fastcall Render_BindVb2647(uint8_t* r);
void __fastcall Render_PushStages(uint8_t* r, int, const uint32_t* states);
void __fastcall Render_SetDefaultMaterial(uint8_t* r, int, uint8_t shiny);
int32_t __fastcall Render_BlendDefault(uint8_t* r);
void __fastcall Render_AlphaTestOn(uint8_t* r, int, uint32_t ref);
void __fastcall Render_CullNone(uint8_t* r);
int32_t __fastcall Render_LightingOff(uint8_t* r);
int32_t __fastcall Render_ZWriteOff(uint8_t* r);
void __fastcall Render_WorldIdentity(uint8_t* r);
uint32_t __fastcall Get_d93990();
void __fastcall Renderer_SetTexture(uint8_t* r, int, int32_t stage, uint8_t* tex);
void __fastcall Particles_FrameSetup(uint8_t* cam);
void* __fastcall Scene_BeginBatch(uint8_t* s, int, int32_t count);
void __fastcall Particle_EmitQuads(uint8_t* pd, int, float* out, int32_t* count);
int32_t __fastcall Render_ZWriteOn(uint8_t* r);
int32_t __fastcall Render_LightingDefault(uint8_t* r);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall PtrList_Clear(PtrList* l);
void __fastcall Render_PopStages(uint8_t* r, int, int32_t n);
int32_t __fastcall Model_SetFrame(uint8_t* m, int, int32_t f);
uint32_t __fastcall Obj_Get1c(uint8_t* o);
void __fastcall SceneObj_SetAlpha(uint8_t* o, int, uint32_t alphaArg, int32_t deep);
void __fastcall SceneObj_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b, int32_t deep);
void __fastcall Model_PrepareTint(uint8_t* m, int, int32_t tint);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall SceneObj_UpdateMatrix(uint8_t* o);
void __fastcall Render_ApplyLocal(uint8_t* r, int, uint8_t* obj);
void __fastcall SceneObj_EmitFlat(uint8_t* o, int, uint32_t* out, int32_t* count);
void __fastcall View_PickTerrain(uint8_t* v, int, uint8_t* cam, uint8_t* terrain);
void __fastcall Render_SetColorOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
void __fastcall Render_SetAlphaOp(uint8_t* r, int, int32_t stage, uint32_t arg1, uint32_t op, uint32_t arg2, uint8_t apply);
int32_t __fastcall Render_SetTextureFactor(uint8_t* r, int, uint32_t v);
void __fastcall Render_WorldFrom(uint8_t* r, int, uint8_t* obj);
void __fastcall Render_SetTextureTransform(uint8_t* r, int, float a, float b, float c, float d);
int32_t __fastcall Render_SetZEnable(uint8_t* r, int, uint32_t v);
void __fastcall Render_AlphaTestOff(uint8_t* r);
int32_t __fastcall Render_BlendOff(uint8_t* r);
void __fastcall Render_SetFiltering(uint8_t* r, int, uint8_t filtered, int8_t stage);
void __fastcall Render_GetFogBlock(uint8_t* r, int, uint8_t* dst);
PtrList* __fastcall Terrain_Cell167d(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Render_CullCcw(uint8_t* r);
void __fastcall Render_GetPair25f7(uint8_t* r, int, int32_t i, uint32_t* a, uint32_t* b);
PtrList* __fastcall Terrain_Cell1675(uint8_t* r, int, int32_t x, int32_t y);
uint32_t __fastcall Get_ccba20();
void __fastcall Render_AddressMirror(uint8_t* r);
void __fastcall Render_AddressWrap(uint8_t* r);
void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Obj_SetAlpha5b(uint8_t* o, int, uint8_t a);
void __fastcall Box_EmitVertices(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t color, int32_t noTop);
void __fastcall SceneLabel_Draw(uint8_t* o);
PtrList* __fastcall Terrain_Cell1681(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Render_AddressClamp(uint8_t* r, int, uint8_t stage);
uint32_t __fastcall Render_Get1578(uint8_t* r);
void __fastcall Quads_EmitFlat(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t tint);
void __fastcall Quads_EmitRaised(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t tint, float lift);
double __fastcall Scene_FogFactor(uint8_t* s, int, uint8_t* r);
PtrList* __fastcall Terrain_Cell1685(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Scene_EmitScenery(uint8_t* sc, int, uint8_t* src, uint8_t* cam, uint8_t* cell, uint8_t** out, uint32_t colour);
void __fastcall Ambience_Visit(uint8_t* a, int, uint32_t x, uint32_t y, uint32_t z);
uint32_t __fastcall Get_d9399c();
uint8_t* __fastcall Emitter_WorldPoints(uint8_t* e, int, int32_t* count);
void __fastcall Emitter_RetintAll(uint8_t* e);
void __fastcall Emitter_Draw(uint8_t* e, int, uint8_t* out, int32_t* count);
void* __fastcall Render_LockVb264f(uint8_t* r);
int32_t __fastcall Render_ProjectSimple(uint8_t* r, int, float x, float y, float z, float* ox, float* oy);
int32_t __fastcall Render_UnlockVb264f(uint8_t* r);
void* __fastcall Render_LockVb2653(uint8_t* r);
int32_t __fastcall Render_UnlockVb2653(uint8_t* r);
void __fastcall Render_CullCw(uint8_t* r);
int32_t __fastcall Render_BlendDarken(uint8_t* r);
void __fastcall Render_AddressBorder(uint8_t* r);
void __fastcall Render_BindPair264f(uint8_t* r);
void __fastcall Render_DrawIndexed(uint8_t* r, int, int32_t indices, uint32_t minIndex, uint32_t vertices);
int32_t __fastcall Render_BlendAlpha(uint8_t* r);
void __fastcall Render_BindVb263f(uint8_t* r);
void __fastcall Render_UseView1(uint8_t* r);
int32_t __fastcall Render_SetFactorAlpha(uint8_t* r, int, uint8_t a);
void __fastcall Renderer_SetFog(uint8_t* r, int, float density, float cr, float cg, float cb);
int32_t __fastcall Render_DrawPoints(uint8_t* r, int, int32_t first, int32_t end);
void __fastcall Obj_GetVec0(uint32_t* o, int, uint32_t* out);
int32_t __fastcall Render_ProjectPoint(uint8_t* r, int, float x, float y, float z, float* sx, float* sy);
void __fastcall Render_TransformWorld(uint8_t* r, int, float* v);
int32_t __stdcall Terrain_RayOcclusion(uint8_t* t, float* pos, const float* dir);
void __fastcall Obj_SetPair10(uint8_t* o, int, uint32_t a, uint32_t b, uint8_t c);
void __fastcall Obj_GetXYZ4(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall Render_BindPair2647(uint8_t* r);
void* __fastcall Render_LockVb264b(uint8_t* r);
int32_t __fastcall Render_UnlockVb264b(uint8_t* r);
uint32_t __fastcall Terrain_Layer(uint8_t* r, int, uint8_t i);
double __fastcall Terrain_HeightAt(uint8_t* t, int, float x, float z);
void __fastcall Meshes_InvalidateAll();
uint8_t __fastcall Render_DeviceState(uint8_t* r);
void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
void __fastcall Display_DestroyRenderer(uint8_t* o);
void __fastcall Pack_RecreateFonts(uint8_t* pack);
void __fastcall SceneStats_Draw(uint8_t* s, int, float dt);
int32_t __fastcall Render_BeginFrame(uint8_t* r, int, int32_t clear);
void __fastcall Render_SetPixelViewport(uint8_t* r, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Render_ApplyFilterStates(uint8_t* r);
void __fastcall Render_FogOn(uint8_t* r);
void __fastcall Render_EndFrame(uint8_t* r, int, int32_t noPresent);
void __fastcall PumpMessages();
void __fastcall SceneObj_ApplyStack(uint8_t* o, int, void* ms);
int32_t __fastcall SceneObj_SelectLod(uint8_t* o, int, float x, float y, float z);
uint32_t __fastcall Obj_AnimFrame(void* o);
double __stdcall PointLineDistance(float px, float py, float pz, float dx, float dy, float dz, float qx, float qy, float qz);
void __fastcall Render_DrawLitGroups(uint8_t* r, int, PtrList* groups, uint8_t release);
void* __fastcall Render_LockVbDiscard(uint8_t* r, int, int32_t n);
void* __fastcall Render_LockVbRange(uint8_t* r, int, int32_t first, int32_t end);
int32_t __fastcall Mesh_EmitPickTriangles(uint8_t* o, int, uint8_t* out, int32_t* count, const float* dir, const float* origin);
void __fastcall Render_UnlockVbRange(uint8_t* r);
int32_t __fastcall Mesh_RayHit(uint8_t* o, int, const float* dir, const float* origin);
void __fastcall Emitter_Tick(uint8_t* e, int, int32_t force);
uint8_t __fastcall Flags24_Low(uint8_t* o);
uint32_t __fastcall Render_Get1534(uint8_t* r);
PtrList* __fastcall Terrain_GlobalList(uint8_t* r);
void __fastcall SceneObj_DrawMarker(uint8_t* o);
PtrList* __fastcall Terrain_Cell1671(uint8_t* r, int, int32_t x, int32_t y);
uint8_t __fastcall Scene_CursorRay(uint8_t* s, int, float* out, const float* eye);
void __fastcall Render_BindVb2643(uint8_t* r);

struct Mat16 {
    uint32_t v[16];
};

void __fastcall Scene_CreateRenderer(uint8_t* s);
void __fastcall Scene_SetupVisitGrid(uint8_t* s, int, uint8_t* t);
uint8_t __fastcall Scene_ViewNeedsRebuild(uint8_t* s, int, uint8_t* cam);
void __fastcall Fx_UpdateIntensity(uint8_t* o);
void __fastcall Render_SunDirVec(uint8_t* r, int, float* v);
void __fastcall Render_TargetChanged(uint8_t* r);
void __fastcall Texture_BindTarget(uint8_t* t, int, void* dev);
void __fastcall Renderer_SetupViews(uint8_t* r, int, uint8_t* cam, uint8_t reflect);
void __fastcall Sound_SetMatrix(uint8_t* s, int, Mat16 m);
int32_t __fastcall Scene_NeedsRedraw(uint8_t* s);
uint32_t __fastcall Render_Get1548(uint8_t* r);
void __fastcall Render_UseView2(uint8_t* r);
int32_t __fastcall Render_Clear(uint8_t* r, int, uint8_t target, uint8_t coloured, uint32_t z);
int32_t __fastcall Render_ColorWriteOff(uint8_t* r);
void __fastcall Scene_DrawFogPatch(uint8_t* s, int, uint8_t* t, uint8_t shadow);
int32_t __fastcall Render_ColorWriteOn(uint8_t* r);
int32_t __fastcall Render_ClearAll(uint8_t* r);
void __fastcall Render_Set1534(uint8_t* r, int, uint32_t v);
void __fastcall Scene_DrawCellBoxes(uint8_t* s, int, uint8_t* c, uint8_t* t, uint8_t reflected);
uint8_t* __fastcall Camera_ConstructBase(uint8_t* c);
void __fastcall Render_SunDir(uint8_t* r, int, float* x, float* y, float* z);
void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z);
uint32_t __fastcall Render_Get1544(uint8_t* r);
void __fastcall Render_Set1540(uint8_t* r, int, uint32_t v);
void __fastcall Scene_DrawScenery(uint8_t* s, int, uint8_t* cam, uint8_t* t);
void __fastcall Camera_Destruct(uint32_t* c);
void __fastcall Render_SetCameraViewport(uint8_t* r, int, uint8_t* cam, uint8_t projOnly);
void __fastcall Render_AllowTranslucency(uint8_t* r);
void __fastcall Render_NoTranslucency(uint8_t* r);
void __fastcall Renderer_SetTimeOfDay(uint8_t* r, int, int32_t secs, int32_t night);
void __fastcall Textures_Manage(int32_t now, int32_t statsOnly);
void __fastcall Sky_Render(uint8_t* s, int, float fog);
void __fastcall Render_UseView3(uint8_t* r);
int32_t __fastcall Camera_TakeChanged(uint8_t* c);
void __fastcall Scene_ComputeFootprint(uint8_t* s);
void __fastcall Camera_SetLock64(uint8_t* c, int, int32_t v);
void __fastcall Sound_RefreshArea(uint8_t* o, int, int32_t x, int32_t y, int32_t r, uint8_t* grid);
void __fastcall TerrainLod_Build(uint8_t* self, int, int32_t levelArg, uint8_t* t, int32_t u0, int32_t v0, int32_t uEnd, int32_t vEnd);
void __fastcall Scene_SortEntries(uint8_t* s);
void __fastcall TerrainSkirt_Build(uint8_t* self, int, uint8_t* t);
void __fastcall Terrain_BuildMesh(uint8_t* sc, int, uint8_t shift, uint8_t* t);
uint32_t __fastcall Render_AnimFrameA(uint8_t* r);
void __fastcall Terrain_DrawChunksFixed(uint8_t* r, int, int32_t vertices, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy);

void __fastcall Terrain_DrawChunksShaded(uint8_t* r, int, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy, uint8_t water, uint8_t* extra,
                                         uint8_t invert);

void __fastcall Terrain_DrawChunksPaired(uint8_t* r, int, uint8_t* tex0, int32_t dx, uint8_t* tex1, int32_t dy, uint8_t water);
int32_t __fastcall Render_SetTexCoordIndex(uint8_t* r, int, uint32_t stage, uint32_t v);
void __fastcall Render_GetRect15a4(uint8_t* r, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Render_SetShaderOffset(uint8_t* r, int, float a, float b, float c, float d);
void __fastcall Terrain_DrawChunks(uint8_t* r, int, int32_t count);
void __fastcall Scene_DrawDetailLayers(uint8_t* s, int, uint8_t* t, uint8_t* cam);
void __fastcall Render_FogOff(uint8_t* r);
void __fastcall Scene_DrawCellLayer(uint8_t* s, int, uint8_t* unused, uint8_t* t, uint8_t raised);
void __fastcall Scene_DrawCellQuads(uint8_t* s, int, uint8_t* unused, uint8_t* t);
void __fastcall Scene_DrawEmitters(uint8_t* s, int, uint8_t* cam);
void __fastcall Rain_Update(uint8_t* s, int, uint8_t* w);
void __fastcall Particles_Render(uint8_t* v, int, uint8_t* cam);
int32_t __fastcall Render_ZFuncAlways(uint8_t* r);
int32_t __fastcall Render_ZFuncLessEqual(uint8_t* r);
uint8_t __fastcall Render_FogEnabled(uint8_t* r);
void __fastcall Scene_VisitTerrain(uint8_t* s, int, uint8_t* visitor);
void __fastcall Marker_Render(uint8_t* s, int, uint8_t* cam, uint8_t* terrain);
void __fastcall Scene_UpdateLensFlare(uint8_t* s);
void __fastcall Cursor_Draw(uint8_t* c);
double __fastcall Engine_AverageFrameMs(void* e);
void __fastcall Scene_DrawOverlay(uint8_t* s, int, int32_t frame);
void __fastcall Engine_SampleTextureMem(void* e);
void __fastcall Camera_SetLock65(uint8_t* c, int, int32_t v);
void __fastcall Engine_UpdateFrameTime(void* e);
RT3_NOINLINE void __fastcall PumpMessages();

// Functions of this file, in exe order.
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
uint8_t __fastcall Scene_RayToHeight(uint8_t* s, int, uint8_t* o, float sx, float sy, float height, float* out);
RT3_NOINLINE void __fastcall Scene_GetVectors(uint8_t* s, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall Scene_ComputeFootprint(uint8_t* s);
RT3_NOINLINE void __fastcall CloudShadows_Render(uint8_t* v);
void __fastcall Scene_UpdateLensFlare(uint8_t* s);
uint8_t __fastcall Scene_CursorRay(uint8_t* s, int, float* out, const float* eye);
double __fastcall Scene_FogFactor(uint8_t* s, int, uint8_t* r);
double __stdcall Render_Scaled1540(uint8_t* r);
void __fastcall Scene_EmitScenery(uint8_t* sc, int, uint8_t* src, uint8_t* cam, uint8_t* cell, uint8_t** out, uint32_t colour);
void __fastcall Particles_Render(uint8_t* v, int, uint8_t* cam);
void __fastcall TerrainSkirt_Build(uint8_t* self, int, uint8_t* t);
void __fastcall Terrain_BuildMesh(uint8_t* sc, int, uint8_t shift, uint8_t* t);
int32_t __fastcall Scene_FogDensity(uint8_t* s);
int32_t __fastcall Scene_NeedsRedraw(uint8_t* s);
RT3_NOINLINE void __fastcall Scene_SetDetail(uint8_t* s, int, int32_t v);
RT3_NOINLINE void __fastcall TerrainLod_Mark(uint8_t* self, int, int32_t levelArg, uint8_t* t, int32_t u0, int32_t v0, int32_t uEndArg, int32_t vEndArg);
void __fastcall TerrainLod_Build(uint8_t* self, int, int32_t levelArg, uint8_t* t, int32_t u0, int32_t v0, int32_t uEnd, int32_t vEnd);
RT3_NOINLINE void __fastcall Scene_MarkDirty(uint8_t* s);
void __fastcall Scene_SortEntries(uint8_t* s);
void __fastcall Scene_RestoreTarget(uint8_t* s);
void __fastcall Scene_VisibleBounds(uint8_t* s, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1, uint8_t* terrain, int32_t shift);
RT3_NOINLINE int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
RT3_NOINLINE void __fastcall Scene_SetLightPair0(uint8_t* s, int, int32_t i, float a, float b);
RT3_NOINLINE void __fastcall Scene_SetLightPair1(uint8_t* s, int, int32_t i, float a, float b);
float __fastcall Scene_GetLightValue0(uint8_t* s, int, int32_t i);
RT3_NOINLINE void __fastcall Scene_SetLightRange(uint8_t* s, int, int32_t i, float v);
void __fastcall Scene_SetPair0(uint8_t* s, int, float a, float b);
void __fastcall Scene_SetPair1(uint8_t* s, int, float a, float b);
RT3_NOINLINE int32_t __fastcall Scene_FocusCell(uint8_t* s, int, float* x, float* y);
void __fastcall TimeOfDay_Split(int32_t v, int32_t* hours, int32_t* minutes, int32_t* seconds);
void __fastcall SceneStats_Draw(uint8_t* s, int, float dt);
void __fastcall Clock_Set11(uint8_t* o, int, int32_t h, int32_t m, int32_t s);
void __fastcall Clock_SetD(uint8_t* o, int, int32_t h, int32_t m, int32_t s);
RT3_NOINLINE void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s);
void __fastcall Scene_SetupVisitGrid(uint8_t* s, int, uint8_t* t);
RT3_NOINLINE void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);
uint8_t __fastcall Scene_ViewNeedsRebuild(uint8_t* s, int, uint8_t* cam);
RT3_NOINLINE double __fastcall Obj_Scene36f9(uint8_t* o);
void __fastcall Scene_VisitTerrain(uint8_t* s, int, uint8_t* visitor);
void __fastcall View_PickTerrain(uint8_t* v, int, uint8_t* cam, uint8_t* terrain);
void __fastcall Marker_Render(uint8_t* s, int, uint8_t* cam, uint8_t* terrain);
void __fastcall Sky_Render(uint8_t* s, int, float fog);
RT3_NOINLINE void __fastcall Scene_QueueNode(uint8_t* r, int, uint8_t* n, uint8_t cull, float cx, float cy, float cz, float ox, float oy, float oz, float dx, float dy, float dz, uint8_t pick, int32_t lod, uint8_t shadow);
RT3_NOINLINE void __fastcall Scene_QueueWorld(uint8_t* r, int, uint8_t* cam, uint8_t* ter, uint8_t shadow);
void __fastcall Scene_DrawCellQuads(uint8_t* s, int, uint8_t* unused, uint8_t* t);
void __fastcall Scene_DrawCellBoxes(uint8_t* s, int, uint8_t* c, uint8_t* t, uint8_t reflected);
void __fastcall Scene_DrawCellLayer(uint8_t* s, int, uint8_t* unused, uint8_t* t, uint8_t raised);
void __fastcall Scene_DrawScenery(uint8_t* s, int, uint8_t* cam, uint8_t* t);
void __fastcall Scene_DrawEmitters(uint8_t* s, int, uint8_t* cam);
void __fastcall Scene_DrawDetailLayers(uint8_t* s, int, uint8_t* t, uint8_t* cam);
void __fastcall Scene_DrawFogPatch(uint8_t* s, int, uint8_t* t, uint8_t shadow);
RT3_NOINLINE void __fastcall Scene_RenderFrame(uint8_t* s, int, uint8_t* cam, uint8_t* ter, uint8_t second);
RT3_NOINLINE void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
RT3_NOINLINE void __fastcall Scene_CreateRenderer(uint8_t* s);
RT3_NOINLINE void __fastcall Scene_DrawOverlay(uint8_t* s, int, int32_t frame);
RT3_NOINLINE void __stdcall Overlay_DrawText(const char* text, int32_t shadow);

static inline int32_t SecondsOfDay(int32_t h, int32_t m, int32_t s) {
    int32_t sec = s % 60;
    int32_t mm = m + s / 60;
    int32_t min = mm % 60;
    int32_t hour = (h + mm / 60) % 24;
    return (hour * 60 + min) * 60 + sec;
}

static inline int32_t FtolD(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline int32_t ClampLight(int32_t i) { return i >= 0x90 ? 0x8f : i < 0 ? 0 : i; }


static inline double D(uint32_t a) { return *(const double*)rt3::Rdata(a); }

static inline double Fl(uint32_t a) { return *(const float*)rt3::Rdata(a); }

static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

static float* CellQuads_Relock(uint8_t* s) {
    int32_t at = F<int32_t>(s, 0x71);
    if (at != -1 && at <= 0xdde8) {
        F<int32_t>(s, 0x75) = at;
        return (float*)Render_AppendVb2647(F<uint8_t*>(s, 0x2d), 0, at, at + 0x2000);
    }
    F<int32_t>(s, 0x71) = 0;
    F<int32_t>(s, 0x75) = 0;
    return (float*)Render_LockVb2647(F<uint8_t*>(s, 0x2d), 0, 0, 0x2000);
}

static inline int32_t Emitter_VertexCount(uint8_t* e) {
    if (F<uint32_t>(e, 0xac) & 0x400) return F<int32_t>(e, 0x38);
    uint8_t* m = F<uint8_t*>(e, 4);
    return m ? F<int32_t>(m, 8) : 0;
}

// Flushes view s's batch when emitter e would not fit (vertex 0xfdd5); the
// new range is appended only when the batch is at 0, else restarted.
static uint8_t* Emitters_MakeRoom(uint8_t* s, uint8_t* e, uint8_t* vb) {
    if (F<int32_t>(s, 0x71) <= 0xfdd5 - Emitter_VertexCount(e) * 6) return vb;
    Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
    if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
    int32_t at = F<int32_t>(s, 0x71);
    if (at != -1 && at <= 0) {
        F<int32_t>(s, 0x75) = at;
        return (uint8_t*)Render_AppendVb2647(F<uint8_t*>(s, 0x2d), 0, at, at + 0xfde8);
    }
    F<int32_t>(s, 0x71) = 0;
    F<int32_t>(s, 0x75) = 0;
    return (uint8_t*)Render_LockVb2647(F<uint8_t*>(s, 0x2d), 0, 0, 0xfde8);
}

// Orders emitter e's points (unless it is far, +0x1d9 from 0xb80) farthest
// from (x, y, z) first; like the original it keeps comparing against the
// first pick's distance after a swap.
static void Emitter_SortPoints(uint8_t* e, float x, float y, float z) {
    int32_t n;
    float** p = (float**)Emitter_WorldPoints(e, 0, &n);
    if (F<int32_t>(e, 0x1d9) >= 0xb80 || n <= 0) return;
    for (int32_t i = 0; i < n; ++i) {
        float* a = p[i];
        double da = fabs((double)a[1] - y);
        da += fabs((double)a[2] - z);
        da += fabs((double)a[0] - x);
        float di = (float)da;
        for (int32_t j = i + 1; j < n; ++j) {
            float* b = p[j];
            double dj = fabs((double)b[1] - y);
            dj += fabs((double)b[2] - z);
            dj += fabs((double)b[0] - x);
            if (dj > di) {
                p[j] = p[i];
                p[i] = b;
            }
        }
    }
}

static inline void EngineDevRenderState(uint32_t state, uint32_t value) {
    void* dev = F<void*>(g_engine, 8);
    (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t))(*(uint8_t**)dev + 0xc8))(dev, state, value);
}

static inline float Terrain_Coord(const float* xs, int32_t i) { return i > 0 ? xs[i] : -xs[-i]; }

static inline void Sky_Place(uint8_t* r, uint8_t* o, uint32_t a, uint32_t c) {
    SceneObj_SetAngles(o, 0, a, 0, c);
    SceneObj_UpdateMatrix(o);
    Render_ApplyLocal(r, 0, o);
    Render_WorldFrom(r, 0, o);
}

static inline void Sky_Draw(uint8_t* r, uint8_t* o) {
    Renderer_SetTexture(r, 0, 0, (uint8_t*)(uintptr_t)Obj_Get1c(o));
    Render_DrawTriangles(r, 0, F<int32_t>(o, 0x1ee), F<int32_t>(o, 0x1fa));
}

static inline double Fsqrt(double v) {
    __asm {
        fld v
        fsqrt
        fstp v
    }
    return v;
}

// The device's render state call (method 0x32) behind the engine (+8).
static void Scene_DeviceRs(int32_t state, int32_t v) {
    void* dev = F<void*>(g_engine, 8);
    ((int32_t(__stdcall*)(void*, int32_t, int32_t))(*(void***)dev)[0xc8 / 4])(dev, state, v);
}

// 0x524070. Where the view ray through screen point (sx, sy) (0..1) from object
// `o`'s position hits the plane y = `height`: false when the ray doesn't point
// down enough (normalized y not above 0.1).
uint8_t __fastcall Scene_RayToHeight(uint8_t* s, int, uint8_t* o, float sx, float sy, float height, float* out) {
    float px, py, pz;
    Obj_GetXYZ1c(o, 0, (uint32_t*)&px, (uint32_t*)&py, (uint32_t*)&pz);
    float ux, uy, uz;
    float vy = (float)(D(0x5c8770) - ((double)sy + sy));
    float vx = (float)((double)sx + sx - D(0x5c8770));
    Render_Unproject(F<uint8_t*>(s, 0x2d), 0, vx, vy, &ux, &uy, &uz);
    double dx = (double)px - ux;
    double dy = (double)py - uy;
    double dz = (double)pz - uz;
    float dzf = (float)dz;
    double len = sqrt((dz * dzf + dy * dy) + dx * dx);
    float inv = (float)(Fl(0x5c85b4) / len);
    double ny = dy * inv;
    if (!(ny > D(0x5c9ba0)) && ny == ny) return 0;
    double t = ((double)uy - height) / ny;
    out[1] = height;
    out[0] = (float)((double)ux - (double)inv * dx * t);
    out[2] = (float)((double)uz - (double)inv * dzf * t);
    return 1;
}
RT3_REPLACE(0x524070, Scene_RayToHeight);

// 0x524170. Copies the scene's three 16-byte vectors (+0x36e8, +0x36f8, +0x3708).
RT3_NOINLINE void __fastcall Scene_GetVectors(uint8_t* s, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    memcpy(a, s + 0x36e8, 16);
    memcpy(b, s + 0x36f8, 16);
    memcpy(c, s + 0x3708, 16);
}
RT3_REPLACE(0x524170, Scene_GetVectors);

// 0x5241e0. The ground footprint of the view: for the four screen corners, where
// the ray from the eye (+0x369c) meets y = 0 (+0x36e8 x, +0x3708 z), with a
// horizon fade +0x36f8 for rays that hardly go down (y step under 0.0?).
RT3_NOINLINE void __fastcall Scene_ComputeFootprint(uint8_t* s) {
    const float xs[4] = { -1.0f, 1.0f, -1.0f, 1.0f };
    const float ys[4] = { -1.0f, -1.0f, 1.0f, 1.0f };
    for (int i = 0; i < 4; ++i) {
        float x, y, z;
        Render_Unproject(F<uint8_t*>(s, 0x2d), 0, xs[i], ys[i], &x, &y, &z);
        double dx = (double)F<float>(s, 0x369c) - x;
        double dy = (double)F<float>(s, 0x36a0) - y;
        float dzf = (float)((double)F<float>(s, 0x36a4) - z);
        double den;
        if (dy < D(0x5c86b8)) {
            F<float>(s, 0x36f8 + i * 4) = (float)((fabs(dy) - D(0x5c85b8)) * D(0x5c86b8));
            den = Fl(0x5cb4a8);
        } else {
            F<float>(s, 0x36f8 + i * 4) = 0.0f;
            den = dy;
        }
        double t = -(F<float>(s, 0x36a0) / den);
        F<float>(s, 0x36e8 + i * 4) = (float)(t * dx + F<float>(s, 0x369c));
        F<float>(s, 0x3708 + i * 4) = (float)(t * dzf + F<float>(s, 0x36a4));
    }
}
RT3_REPLACE(0x5241e0, Scene_ComputeFootprint);

// 0x5242e0. Draws view v's cloud shadow layers (+0x29 +0x59; model +0x95):
// alpha from the fog intensity (46..255) scaled down with the camera
// height +0x368a (3500 = none; at least 46 with +0x3692), texture scale
// from the map size (+0x11); the layers (+0x29 +0x5a + 1) placed against
// the camera and scrolled in opposite directions.
RT3_NOINLINE void __fastcall CloudShadows_Render(uint8_t* v) {
    if (!F<uint8_t*>(v, 0x29)[0x59]) return;
    int32_t a = Fx_Intensity(v);
    if (a < 0x2e)
        a = 0x2e;
    else if (a > 0xff)
        a = 0xff;
    int32_t alpha = (int32_t)((*(const double*)rt3::Rdata(0x5dcfc8) - F<int32_t>(v, 0x368a)) * *(const double*)rt3::Rdata(0x5dcfc0) * a);
    if (v[0x3692] && alpha < 0x2e)
        alpha = 0x2e;
    else if (alpha < 0)
        alpha = 0;
    else if (alpha > 0xff)
        alpha = 0xff;
    float scale = (float)((double)F<int32_t>(v, 0x11) * *(const float*)rt3::Rdata(0x5dcfb8));
    if (!F<uint8_t*>(v, 0x95)) return;
    Render_PushStages(F<uint8_t*>(v, 0x2d), 0, F<const uint32_t*>(v, 0x3d));
    Render_SetColorOp(F<uint8_t*>(v, 0x2d), 0, 0, 2, 5, 1, 1);
    Render_SetAlphaOp(F<uint8_t*>(v, 0x2d), 0, 0, 2, 6, 3, 1);
    Render_AlphaTestOn(F<uint8_t*>(v, 0x2d), 0, 3);
    Render_BlendDefault(F<uint8_t*>(v, 0x2d));
    Render_SetTextureFactor(F<uint8_t*>(v, 0x2d), 0, (uint32_t)alpha << 24);
    uint32_t tex = Obj_Get1c(F<uint8_t*>(v, 0x95));
    Renderer_SetTexture(F<uint8_t*>(v, 0x2d), 0, 0, (uint8_t*)(uintptr_t)tex);
    for (int32_t k = F<uint8_t*>(v, 0x29)[0x5a]; k >= 0; --k) {
        int32_t off = k * 0x6a4 - Fx_Intensity(v) + 0xdac;
        uint8_t* m = F<uint8_t*>(v, 0x95);
        float z = -F<float>(v, 0x36a4);
        float y = (F<uint8_t*>(v, 0x29)[0x61] && v[0x3692]) ? (float)((double)off + F<float>(v, 0x36a0)) : (float)((double)off - F<float>(v, 0x36a0));
        float x = -F<float>(v, 0x369c);
        Obj_MoveTo(m, 0, x, y, z);
        SceneObj_UpdateMatrix(F<uint8_t*>(v, 0x95));
        Render_ApplyLocal(F<uint8_t*>(v, 0x2d), 0, F<uint8_t*>(v, 0x95));
        Render_WorldFrom(F<uint8_t*>(v, 0x2d), 0, F<uint8_t*>(v, 0x95));
        if (k == 0)
            Render_SetTextureTransform(F<uint8_t*>(v, 0x2d), 0, (float)((double)scale + scale), (float)((double)scale + scale), 1.0f, 1.0f);
        else if (k == 1)
            Render_SetTextureTransform(F<uint8_t*>(v, 0x2d), 0, -scale, -scale, -1.0f, -1.0f);
        uint8_t* mm = F<uint8_t*>(v, 0x95);
        Render_DrawTriangles(F<uint8_t*>(v, 0x2d), 0, F<int32_t>(mm, 0x1ee), F<int32_t>(mm, 0x1fa));
    }
    Render_SetTextureTransform(F<uint8_t*>(v, 0x2d), 0, 0.0f, 0.0f, 1.0f, 1.0f);
    Render_PopStages(F<uint8_t*>(v, 0x2d), 0, 1);
}
RT3_REPLACE(0x5242e0, CloudShadows_Render);

// 0x524530. Lens flare: when the settings allow it (+0x29 -> +0x58) and the sun
// (+0x99) shows on screen (flare object +0x3665, brightness under 0xf5), places
// its 12 sprites along the line from the sun through the screen centre.
RT3_NOINLINE void __fastcall Scene_UpdateLensFlare(uint8_t* s) {
    if (!F<uint8_t*>(s, 0x29)[0x58] || !F<uint32_t>(s, 0x99)) return;
    float sunX = 0.0f, sunY = 0.0f;
    if (Obj_GetPair10(F<uint8_t*>(s, 0x3665), 0, (uint32_t*)&sunX, (uint32_t*)&sunY) >= 0xf5) return;
    float vw = 0.0f, vh = 0.0f;
    Render_GetViewport(F<uint8_t*>(s, 0x2d), 0, (uint32_t*)&vw, (uint32_t*)&vh, nullptr, nullptr);
    float u = (float)((double)sunX / vw * D(0x5c8628) + D(0x5c8628) / vw);
    float v = (float)(D(0x5c8628) / vh - (double)sunY / vh * D(0x5c8628));
    float du = (float)((D(0x5c8628) - u) * D(0x5c85a0));
    float dv = (float)((D(0x5c8628) - v) * D(0x5c85a0));
    for (int i = 0; i < 12; ++i) Particles_Draw(F<uint8_t*>(s, 0x3665), 0, i, u, v, du, dv);
}
RT3_REPLACE(0x524530, Scene_UpdateLensFlare);

// 0x524650. Direction under the scene's cursor (+0x3720 / +0x3724, 0..1 over the
// view) from `eye` into `out` (out first, as in the original), when the cursor is in the view (+0x3745). False
// while the scene is frozen (+0x3692).
RT3_NOINLINE uint8_t __fastcall Scene_CursorRay(uint8_t* s, int, float* out, const float* eye) {
    if (s[0x3692]) return 0;
    if (s[0x3745]) {
        float sy = (float)(D(0x5c8770) - ((double)F<float>(s, 0x3724) + F<float>(s, 0x3724)));
        float sx = (float)((double)F<float>(s, 0x3720) + F<float>(s, 0x3720) - D(0x5c8770));
        Render_UnprojectDir(F<uint8_t*>(s, 0x2d), 0, sx, sy, out, eye);
    }
    return 1;
}
RT3_REPLACE(0x524650, Scene_CursorRay);

// 0x5246b0. Fog factor of the scene for the renderer's view distance r +0x1540
// (at least 60): 1 - (depth +0x368e - near) / (far - near) with near = 0x5c88c8
// of the distance, clamped to 0..1.
RT3_NOINLINE double __fastcall Scene_FogFactor(uint8_t* s, int, uint8_t* r) {
    int32_t n = (int32_t)Render_Get1540(r);
    if (n < 0x3c) n = 0x3c;
    double dist = (double)n * D(0x5dcfd0);
    float nearD = (float)(D(0x5c88c8) * dist);
    double span = dist - nearD;
    if (!(span > D(0x5c8678))) span = D(0x5c8678);
    double t = D(0x5c8770) - ((double)F<float>(s, 0x368e) - nearD) / span;
    if (t < Fl(0x5c8598)) return Fl(0x5c8598);
    if (t > D(0x5c8770)) return Fl(0x5c85b4);
    return t;
}
RT3_REPLACE(0x5246b0, Scene_FogFactor);

// 0x524750. The renderer's value +0x1540 (at least 60) scaled by 0x5dcfd0.
double __stdcall Render_Scaled1540(uint8_t* r) {
    int32_t n = (int32_t)Render_Get1540(r);
    if (n < 0x3c) n = 0x3c;
    return (double)n * D(0x5dcfd0);
}
RT3_REPLACE(0x524750, Render_Scaled1540);

// 0x524780. Emits the scenery of terrain block `cell` (origin +8 / +0xc, 4 x 4
// tiles of 0x170 with their slot +0x14.. holding a height +2, a height scale
// +6 and 4 x 4 sub-cells of 8 bytes +0xa) into the vertex buffer at *out:
// each tile fades with its distance from the camera (+0x369c / +0x36a4)
// beyond half the view range of `src`; far tiles are culled as a sphere and
// beyond the detail range use the low-detail models (facing camera `cam`).
// Each sub-cell holds (byte >> 4) & 7 objects, kinds from src slot 1, minus
// the variants masked by its low bits. Flushes the buffer when it nears full.
RT3_NOINLINE void __fastcall Scene_EmitScenery(uint8_t* sc, int, uint8_t* src, uint8_t* cam, uint8_t* cell, uint8_t** out, uint32_t colour) {
    uint8_t* v = *out;
    float cz = F<float>(cell, 0xc);
    float cx = F<float>(cell, 8);
    int32_t count = 0;
    int32_t detail = (int32_t)Render_Get153c(src);
    int32_t range = (int32_t)Render_Get1540(src);
    double half = *(const double*)rt3::Rdata(0x5c8628);
    float near_ = (float)(half * range);
    float fadeLen = (float)(((double)range - near_) * half);
    double edge = *(const float*)rt3::Rdata(0x5cfcfc);
    uint8_t** slot = (uint8_t**)(cell + 0x14);
    for (int32_t rowOff = 0, r4 = 0; rowOff < 0x5c0; rowOff += 0x170, r4 += 4) {
        double x = (double)rowOff + cx + edge;
        float fx = (float)x;
        double ddx = x - F<float>(sc, 0x369c);
        float dx2 = (float)(ddx * ddx);
        for (int32_t colOff = 0, c4 = 0; colOff < 0x5c0; colOff += 0x170, c4 += 4, ++slot) {
            uint8_t* tile = *slot;
            if (!tile) continue;
            float y0 = F<float>(tile, 2);
            float yScale = F<float>(tile, 6);
            double z = (double)colOff + cz + edge;
            float fz = (float)z;
            double ddz = z - F<float>(sc, 0x36a4);
            double dist = sqrt(ddz * ddz + dx2) * *(const double*)rt3::Rdata(0x5dcfd8);
            float fd = (float)dist;
            double t = dist - near_;
            if (t < *(const float*)rt3::Rdata(0x5c8598)) t = *(const float*)rt3::Rdata(0x5c8598);
            t = t / fadeLen;
            if (t > *(const double*)rt3::Rdata(0x5c8770)) t = *(const float*)rt3::Rdata(0x5c85b4);
            else if (t < *(const double*)rt3::Rdata(0x5c8608)) t = *(const float*)rt3::Rdata(0x5c8598);
            int32_t alpha = FtolD((double)F<int32_t>(sc, 0x36b4) - t * *(const double*)rt3::Rdata(0x5ca110));
            if (alpha <= F<int32_t>(sc, 0x22b8b)) continue;
            if (alpha > 0xff) alpha = 0xff;
            int32_t lowDetail = 0;
            uint32_t col = (uint32_t)alpha << 24 | colour;
            int32_t lim = detail < 8 ? detail : 8;
            int32_t lod = FtolD(fd);
            if (lod > lim) {
                uint32_t radius;
                if (fd < *(const float*)rt3::Rdata(0x5c9ac8)) radius = 0x3fcccccd;
                else if (fd < *(const float*)rt3::Rdata(0x5c8d4c)) radius = 0x3fa66666;
                else if (fd < *(const float*)rt3::Rdata(0x5ca0f0)) radius = 0x3f933333;
                else radius = 0x3f88f5c3;
                if (!Render_ProjectClip(F<uint8_t*>(sc, 0x2d), 0, fx, y0, fz, *(float*)&radius, F<float>(sc, 0x36ac), nullptr)) continue;
                if (lod > detail) lowDetail = 1;
            }
            uint8_t* b = tile + 0xa;
            for (int32_t gi = 0; gi < 4; ++gi) {
                int32_t row = r4 + gi;
                for (int32_t gj = 0; gj < 4; ++gj, b += 8) {
                    if ((b[0] & 0x70) && (b[0] & 0xf) != 0xf) {
                        g_cc3b3c += (b[0] >> 4) & 7;
                        for (int32_t k = 0; k < ((b[0] >> 4) & 7); ++k) {
                            int32_t colIdx = c4 + gj;
                            float ox, oz;
                            uint8_t* model;
                            int32_t variant;
                            if (!((int32_t(__thiscall*)(uint8_t*, int32_t, int32_t, int32_t, uint32_t, float*, float*, uint8_t**,
                                                        int32_t*))(*(void***)src)[1])(src, row, colIdx, k, b[1], &ox, &oz, &model, &variant))
                                continue;
                            if ((b[0] & 0xf) && ((uint8_t)(1 << (variant & 0x1f)) & b[0] & 0xf)) continue;
                            count = 0;
                            float px = (float)((double)ox + cx);
                            float py = (float)((double)(int8_t)b[k + 2] * yScale + y0);
                            float pz = (float)((double)oz + cz);
                            if (lowDetail && F<int32_t>(F<uint8_t*>(model, 0x14), 0x14) > 1) {
                                Mesh_FaceCamera(model, 0, cam, F<int32_t>(sc, 5), lowDetail);
                                Model_EmitTriangles(model, 0, v, &count, col, lowDetail, px, py, pz);
                            } else {
                                Mesh_EmitTriangles(model, 0, v, &count, col, lowDetail, px, py, pz);
                            }
                            F<int32_t>(sc, 0x71) += count;
                            v += count * 0x18;
                        }
                    }
                    if (F<int32_t>(sc, 0x71) > 0xf5e8) {
                        Render_UnlockVb2647(F<uint8_t*>(sc, 0x2d));
                        if (F<int32_t>(sc, 0x71) > F<int32_t>(sc, 0x75)) Render_DrawTriangles(F<uint8_t*>(sc, 0x2d), 0, F<int32_t>(sc, 0x75), F<int32_t>(sc, 0x71));
                        int32_t n = F<int32_t>(sc, 0x71);
                        if (n == -1 || n > 0xdde8) {
                            F<int32_t>(sc, 0x71) = 0;
                            F<int32_t>(sc, 0x75) = 0;
                            v = (uint8_t*)Render_LockVb2647(F<uint8_t*>(sc, 0x2d), 0, 0, 0x2000);
                        } else {
                            F<int32_t>(sc, 0x75) = n;
                            v = (uint8_t*)Render_AppendVb2647(F<uint8_t*>(sc, 0x2d), 0, n, n + 0x2000);
                        }
                    }
                }
            }
        }
    }
    *out = v;
}
RT3_REPLACE(0x524780, Scene_EmitScenery);

// 0x524c70. Draws view v's queued particle emitters (+0x89; dropped when
// particles are off, +0x29 +0x5e, or with the engine's +0x114262): unlit,
// no z writes, alpha tested, textured with 0xd93990, quads streamed in
// 0x2000-vertex batches of the shared buffer (+0x71 used, +0x75 drawn;
// restarting near the end), then the states are restored and the queue
// emptied.
RT3_NOINLINE void __fastcall Particles_Render(uint8_t* v, int, uint8_t* cam) {
    if (!F<uint8_t*>(v, 0x29)[0x5e] || F<int32_t>(g_engine, 0x114262)) {
        PtrList_Clear(F<PtrList*>(v, 0x89));
        return;
    }
    bool drew = false;
    PtrList_Rewind(F<PtrList*>(v, 0x89));
    uint8_t* n = (uint8_t*)PtrList_Next(F<PtrList*>(v, 0x89));
    float* vb;
    if (n) {
        uint8_t* r = F<uint8_t*>(v, 0x2d);
        Render_BindVb2647(r);
        Render_PushStages(F<uint8_t*>(v, 0x2d), 0, F<const uint32_t*>(v, 0x45));
        Render_SetDefaultMaterial(F<uint8_t*>(v, 0x2d), 0, 0);
        Render_BlendDefault(F<uint8_t*>(v, 0x2d));
        Render_AlphaTestOn(F<uint8_t*>(v, 0x2d), 0, 1);
        Render_CullNone(F<uint8_t*>(v, 0x2d));
        Render_LightingOff(F<uint8_t*>(v, 0x2d));
        Render_ZWriteOff(F<uint8_t*>(v, 0x2d));
        Render_WorldIdentity(F<uint8_t*>(v, 0x2d));
        uint32_t tex = Get_d93990();
        Renderer_SetTexture(F<uint8_t*>(v, 0x2d), 0, 0, (uint8_t*)(uintptr_t)tex);
        Particles_FrameSetup(cam);
        vb = (float*)Scene_BeginBatch(v, 0, 0x2000);
        drew = true;
    } else
        vb = (float*)cam;
    int32_t left = F<int32_t>(F<uint8_t*>(v, 0x89), 0xc);
    if (left) {
        do {
            if (F<int32_t>(v, 0x71) > 0xf9e8) {
                Render_UnlockVb2647(F<uint8_t*>(v, 0x2d));
                if (F<int32_t>(v, 0x71) > F<int32_t>(v, 0x75)) Render_DrawTriangles(F<uint8_t*>(v, 0x2d), 0, F<int32_t>(v, 0x75), F<int32_t>(v, 0x71));
                int32_t at = F<int32_t>(v, 0x71);
                if (at != -1 && at <= 0xdde8) {
                    F<int32_t>(v, 0x75) = at;
                    vb = (float*)Render_AppendVb2647(F<uint8_t*>(v, 0x2d), 0, at, at + 0x2000);
                } else {
                    F<int32_t>(v, 0x71) = 0;
                    F<int32_t>(v, 0x75) = 0;
                    vb = (float*)Render_LockVb2647(F<uint8_t*>(v, 0x2d), 0, 0, 0x2000);
                }
            }
            int32_t count = 0;
            Particle_EmitQuads(n, 0, vb, &count);
            vb += count * 6;
            F<int32_t>(v, 0x71) += count;
            n = (uint8_t*)PtrList_Next(F<PtrList*>(v, 0x89));
        } while (--left);
    }
    if (!drew) return;
    Render_UnlockVb2647(F<uint8_t*>(v, 0x2d));
    if (F<int32_t>(v, 0x71) > F<int32_t>(v, 0x75)) Render_DrawTriangles(F<uint8_t*>(v, 0x2d), 0, F<int32_t>(v, 0x75), F<int32_t>(v, 0x71));
    Render_ZWriteOn(F<uint8_t*>(v, 0x2d));
    Render_LightingDefault(F<uint8_t*>(v, 0x2d));
    PtrList_Clear(F<PtrList*>(v, 0x89));
    Render_SetDefaultMaterial(F<uint8_t*>(v, 0x2d), 0, 1);
    Render_PopStages(F<uint8_t*>(v, 0x2d), 0, 1);
}
RT3_REPLACE(0x524c70, Particles_Render);

// 0x524e60. Builds the skirts that hide the terrain's outer edge: for each map
// side the camera is near (within 4 of the camera cell from +0x369c / +0x36a4,
// scaled by 0x5dcfe8), a strip down to y = -1024 per run of marked LOD cells
// (grid +0x61 via settings +0x65). Each quad (6 vertices of x, y, z, 0, u, v
// at +0x22bb7, count +0x3abb7) runs from a marked cell to the next marked
// one, its top at the cell heights of their levels plus +0x25, faded by the
// cell attributes' low nibble. +0x3abbb..+0x3abd7 get each side's vertex
// range. QUIRK: the level of "the next cell" carries over between cells and
// sides in the argument slot, so a run reaching the side's end uses the
// previous level again, one lower each time, starting from the low byte of
// `t` itself; the right side reads its attributes one entry too early.
RT3_NOINLINE void __fastcall TerrainSkirt_Build(uint8_t* self, int, uint8_t* t) {
    int32_t shift = VisitGrid_Log2(F<uint8_t*>(self, 0x65));
    uint8_t* grid = VisitGrid_Flags(F<uint8_t*>(self, 0x65));
    float** slots = (float**)Terrain_HeightsSlot(t);
    uint32_t* xs = (uint32_t*)(uintptr_t)Terrain_Get15e5(t);
    int32_t W = Terrain_Rows(t);
    int32_t H = Terrain_Cols(t);
    uint8_t* attr = (uint8_t*)(uintptr_t)Terrain_Get165d(t);
    uint32_t* out = (uint32_t*)(self + 0x22bb7);
    int32_t& count = F<int32_t>(self, 0x3abb7);
    count = 0;
    int32_t camX = Ftol((double)F<float>(self, 0x369c) * Fl(0x5dcfe8));
    int32_t camY = Ftol((double)F<float>(self, 0x36a4) * Fl(0x5dcfe8));
    for (uint32_t o = 0x3abbb; o <= 0x3abd7; o += 4) F<int32_t>(self, o) = 0;
    uint8_t carry = (uint8_t)(uintptr_t)t;
    auto alphaOf = [](uint8_t nib) { return (float)((D(0x5c8770) - (double)(int32_t)nib * D(0x5d0aa0)) * D(0x5c9ba0)); };
    auto height = [&](int8_t lvl, int32_t row, int32_t col) {
        return slots[lvl][((W >> (lvl & 31)) + 1) * (row >> (lvl & 31)) + (col >> (lvl & 31))];
    };
    auto put = [&](uint32_t x, float y, uint32_t z, float u, float v) {
        out[0] = x;
        out[1] = FBits(y);
        out[2] = z;
        out[3] = 0;
        out[4] = FBits(u);
        out[5] = FBits(v);
        out += 6;
    };
    const float low = BitsF(0xc4800000u), bottomV = BitsF(0x3f7d70a4u);
    // One side: `n` cells, cell(k) its LOD byte, nib(k) its attribute
    // nibble, top(lvl, k) its height and emit() the quad's six vertices.
    auto side = [&](int32_t n, auto cell, auto nib, auto top, auto emit) {
        for (int32_t i = 0; i < n; ++i) {
            uint8_t c = cell(i);
            if (!c) continue;
            int8_t l1 = (int8_t)(c - 1);
            int32_t j = i + 1;
            if (j > n) {
                if (!carry) continue;
            } else {
                for (;; ++j) {
                    if (j > n) break;
                    carry = cell(j);
                    if (carry) break;
                }
                if (j > n) continue;
            }
            --carry;
            uint8_t na = nib(i), nb = nib(j);
            float aA = alphaOf(na), aB = alphaOf(nb);
            float h1 = top(l1, i), h2 = top((int8_t)carry, j);
            float a1 = na ? aA : 0.05f;
            float a2 = nb ? aB : Fl(0x5d0058);
            float ui = (float)((double)i * D(0x5dcfe0)), uj = (float)((double)j * D(0x5dcfe0));
            float t1 = h1 + F<float>(self, 0x25), t2 = h2 + F<float>(self, 0x25);
            emit(i, j, t1, t2, ui, uj, a1, a2);
            count += 6;
            i = j - 1;
        }
    };
    if (camY < 4) {
        F<int32_t>(self, 0x3abbb) = 0;
        side(W, [&](int32_t k) { return grid[k]; }, [&](int32_t k) { return (uint8_t)(attr[k * 3 + 2] & 0xf); },
             [&](int8_t l, int32_t k) { return height(l, 0, k); },
             [&](int32_t i, int32_t j, float t1, float t2, float ui, float uj, float a1, float a2) {
                 put(xs[i], t1, xs[0], ui, a1);
                 put(xs[j], low, xs[0], uj, bottomV);
                 put(xs[i], low, xs[0], ui, bottomV);
                 put(xs[j], low, xs[0], uj, bottomV);
                 put(xs[i], t1, xs[0], ui, a1);
                 put(xs[j], t2, xs[0], uj, a2);
             });
        F<int32_t>(self, 0x3abbf) = count;
    }
    if (camX < 4) {
        F<int32_t>(self, 0x3abc3) = count;
        side(H, [&](int32_t k) { return grid[k << (shift & 31)]; }, [&](int32_t k) { return (uint8_t)(attr[(W + 1) * k * 3 + 2] & 0xf); },
             [&](int8_t l, int32_t k) { return height(l, k, 0); },
             [&](int32_t i, int32_t j, float t1, float t2, float ui, float uj, float a1, float a2) {
                 put(xs[0], t1, xs[i], ui, a1);
                 put(xs[0], low, xs[i], ui, bottomV);
                 put(xs[0], low, xs[j], uj, bottomV);
                 put(xs[0], t1, xs[i], ui, a1);
                 put(xs[0], low, xs[j], uj, bottomV);
                 put(xs[0], t2, xs[j], uj, a2);
             });
        F<int32_t>(self, 0x3abc7) = count;
    }
    int32_t r = H - 1;
    if (camY > H - 4) {
        F<int32_t>(self, 0x3abcb) = count;
        side(W, [&](int32_t k) { return grid[(r << (shift & 31)) + k]; },
             [&](int32_t k) { return (uint8_t)(attr[(r * 3 - 3) * (W + 1) + k * 3 + 2] & 0xf); },
             [&](int8_t l, int32_t k) { return height(l, r, k); },
             [&](int32_t i, int32_t j, float t1, float t2, float ui, float uj, float a1, float a2) {
                 put(xs[i], t1, xs[r], ui, a1);
                 put(xs[i], low, xs[r], ui, bottomV);
                 put(xs[j], low, xs[r], uj, bottomV);
                 put(xs[i], t1, xs[r], ui, a1);
                 put(xs[j], low, xs[r], uj, bottomV);
                 put(xs[j], t2, xs[r], uj, a2);
             });
        F<int32_t>(self, 0x3abcf) = count;
    }
    int32_t c = W - 1;
    if (camX > W - 4) {
        F<int32_t>(self, 0x3abd3) = count;
        side(H, [&](int32_t k) { return grid[(k << (shift & 31)) + c]; },
             [&](int32_t k) { return (uint8_t)(attr[((W + 1) * k + c) * 3 - 3 + 2] & 0xf); },
             [&](int8_t l, int32_t k) { return height(l, k, c); },
             [&](int32_t i, int32_t j, float t1, float t2, float ui, float uj, float a1, float a2) {
                 put(xs[c], t1, xs[i], ui, a1);
                 put(xs[c], low, xs[j], uj, bottomV);
                 put(xs[c], low, xs[i], ui, bottomV);
                 put(xs[c], low, xs[j], uj, bottomV);
                 put(xs[c], t1, xs[i], ui, a1);
                 put(xs[c], t2, xs[j], uj, a2);
             });
        F<int32_t>(self, 0x3abd7) = count;
    }
}
RT3_REPLACE(0x524e60, TerrainSkirt_Build);

// 0x525ae0. Builds the terrain mesh of the visible patches (list +0x374f,
// count +0x22b53, 16-byte entries x, y, texture) at level `shift`: each patch
// is triangulated by the grid +0x65 and its triangles batched per (ground
// type, texture) pair of terrain t; shared corners reuse their vertex (index
// table 0x8f2520 / texture 0xb33530, both 0x401 wide, undone at the end with
// the lod flags). Vertices carry position, normal and the patch's texture
// coordinates; plain-ground triangles near the camera cell (+0x3682 / +0x3686)
// weigh into the mean ground height and count +0x3694 given to the renderer.
RT3_NOINLINE void __fastcall Terrain_BuildMesh(uint8_t* sc, int, uint8_t shift, uint8_t* t) {
    int32_t left = F<int32_t>(sc, 0x22b53);
    uint32_t prevType = 0xffffffe0, prevTex = 0xffffffe0;
    int32_t* entry = (int32_t*)(sc + 0x374f);
    uint8_t* batch = nullptr;
    float sum = 0.0f, weights = 0.0f;
    uint8_t* grid = F<uint8_t*>(sc, 0x65);
    int32_t lodShift = VisitGrid_Log2(grid);
    uint8_t* lodFlags = VisitGrid_Flags(F<uint8_t*>(sc, 0x65));
    int32_t w = Terrain_Rows(t);
    int32_t h = Terrain_Cols(t);
    const uint32_t* uTab = (const uint32_t*)(uintptr_t)Terrain_Get15e9(t);
    const uint32_t* vTab = (const uint32_t*)(uintptr_t)Terrain_Get15ed(t);
    const uint32_t* posTab = (const uint32_t*)(uintptr_t)Terrain_Get15e5(t);
    float** heights = (float**)Terrain_HeightsSlot(t);
    uint8_t** normals = (uint8_t**)Terrain_Slot15f1(t);
    const uint8_t* types = (const uint8_t*)(uintptr_t)Terrain_Get1655(t);
    Terrain_Get1659(t);
    uint32_t* vb = (uint32_t*)Render_LockVb2(F<uint8_t*>(sc, 0x2d));
    uint16_t& next = g_cc3b34;
    next = 1;
    uint32_t lastIdx = 0xffff;
    uint16_t** slotList = (uint16_t**)0xc43b38;
    uint8_t** texList = (uint8_t**)0xaf3530;
    uint8_t** lodList = (uint8_t**)0xc83b38;
    uint16_t* const idxTab = (uint16_t*)0x8f2520;
    uint8_t* const texTab = (uint8_t*)0xb33530;
    Render_ResetTextureSlots(F<uint8_t*>(sc, 0x2d));
    F<int32_t>(sc, 0x3694) = 0;
    uint32_t* pos = (uint32_t*)0xccd828;
    uint32_t defType = Render_Get1554(t);
    float worldY = F<float>(sc, 0x25);
    if (left) {
        int32_t wMax = w >> shift;
        do {
            int32_t px = entry[0], py = entry[1];
            uint32_t tex = (uint32_t)entry[2];
            --left;
            entry += 4;
            F<int32_t>(F<uint8_t*>(sc, 0x65), 4) = 0;
            if (px >= wMax || py >= h >> shift) continue;
            TerrainPatch_Triangulate(F<uint8_t*>(sc, 0x65), 0, px << shift, py << shift);
            if (!F<int32_t>(F<uint8_t*>(sc, 0x65), 4)) continue;
            int32_t ox = px, oy = py;
            Render_SnapToGrid(t, 0, &ox, &oy);
            const int16_t* tri = (const int16_t*)(F<uint8_t*>(sc, 0x65) + 8);
            uint32_t type;
            if (!batch) {
                if (g_engine[0x1146eb])
                    type = types[(tri[0] + tri[2] + tri[4]) / 3 + ((tri[1] + tri[3] + tri[5]) / 3) * (w + 1)];
                else
                    type = defType;
                batch = Render_AcquireVertexSlot(F<uint8_t*>(sc, 0x2d), 0, F<int32_t>(t, 8 + type * 4), F<int32_t>(t, 8 + tex * 4));
                if (!batch) goto done;
                prevType = type;
                prevTex = tex;
            }
            uint32_t* nrm = vb + 3;
            for (int32_t i = 0; i < F<int32_t>(F<uint8_t*>(sc, 0x65), 4); i += 3, tri += 6) {
                int32_t vx[3] = { tri[0], tri[2], tri[4] };
                int32_t vy[3] = { tri[1], tri[3], tri[5] };
                int32_t same = (vy[1] == vy[2]) + (vy[0] == vy[2]) + (vx[1] == vx[2]) + (vx[0] == vx[2]) + (vy[0] == vy[1]) + (vx[0] == vx[1]);
                if (same > 3) continue;
                int32_t cx = (vx[2] + vx[1] + vx[0]) / 3;
                int32_t cy = (vy[2] + vy[1] + vy[0]) / 3;
                if (g_engine[0x1146eb]) type = types[(w + 1) * cy + cx];
                else type = defType;
                if (F<int32_t>(batch, 0x21) <= 3 || prevType != type || prevTex != tex) {
                    if ((uint16_t)lastIdx != 0xffff) {
                        F<uint16_t*>(batch, 0x29)[0] = (uint16_t)lastIdx;
                        F<uint16_t*>(batch, 0x29)[1] = (uint16_t)lastIdx;
                        F<uint16_t*>(batch, 0x29)[2] = (uint16_t)lastIdx;
                    }
                    batch = Render_AcquireVertexSlot(F<uint8_t*>(sc, 0x2d), 0, F<int32_t>(t, 8 + type * 4), F<int32_t>(t, 8 + tex * 4));
                    if (!batch) goto done;
                    prevType = type;
                    prevTex = tex;
                }
                uint16_t* slot = nullptr;
                for (int k = 0; k < 3; ++k) {
                    int32_t x = vx[k], y = vy[k];
                    int32_t idx = x * 0x401 + y;
                    slot = &idxTab[idx];
                    if (!*slot || texTab[idx] != tex) {
                        *slotList++ = slot;
                        *slot = next;
                        *texList++ = &texTab[idx];
                        texTab[idx] = (uint8_t)tex;
                        ++next;
                        uint8_t* lodp = lodFlags + (y << lodShift) + x;
                        *lodList++ = lodp;
                        uint8_t lod = (uint8_t)(*lodp - 1);
                        vb[0] = pos[0] = posTab[x];
                        vb[2] = pos[2] = posTab[y];
                        int32_t cell = ((w >> lod) + 1) * (y >> lod) + (x >> lod);
                        float hy = (float)((double)heights[lod][cell] + worldY);
                        ((float*)vb)[1] = hy;
                        ((float*)pos)[1] = hy;
                        const uint32_t* n = (const uint32_t*)(normals[lod] + cell * 0xc);
                        nrm[0] = n[0];
                        nrm[1] = n[1];
                        nrm[2] = n[2];
                        vb[6] = uTab[x - ox];
                        vb[7] = vTab[y - oy];
                        vb += 8;
                        pos += 3;
                        nrm += 8;
                    }
                    if (k == 2 && type == 0) {
                        int32_t dx = F<int32_t>(sc, 0x3682) - cx;
                        int32_t dy = F<int32_t>(sc, 0x3686) - cy;
                        int32_t d = (dx < 0 ? -dx : dx) * 0x418 + (dy < 0 ? -dy : dy);
                        int32_t near_ = ((const uint16_t*)0x6d9098)[d];
                        if (near_ < 1) near_ = 1;
                        double fw = (double)(near_ >> 1);
                        double wgt;
                        if (fw < *(const float*)rt3::Rdata(0x5c8898)) {
                            wgt = *(const float*)rt3::Rdata(0x5c85b4);
                        } else {
                            double e = fw - *(const double*)rt3::Rdata(0x5c8bf0);
                            wgt = *(const double*)rt3::Rdata(0x5c8770) / (e * e);
                        }
                        double g = heights[0][(w + 1) * cy + cx];
                        ++F<int32_t>(sc, 0x3694);
                        sum = (float)(g * wgt + sum);
                        weights = (float)(wgt + weights);
                    }
                    uint16_t v = (uint16_t)(*slot - 1);
                    if (v < F<uint16_t>(batch, 8)) F<uint16_t>(batch, 8) = v;
                    if (v > F<uint16_t>(batch, 0xa)) F<uint16_t>(batch, 0xa) = v;
                    *F<uint16_t*>(batch, 0x29) = v;
                    F<uint16_t*>(batch, 0x29) += 1;
                    if (k == 2) lastIdx = v;
                }
                F<int32_t>(batch, 0x21) -= 3;
                F<int32_t>(batch, 0x25) += 1;
                if (next > 0xfffa) goto done;
            }
        } while (left);
    }
done:
    if (int32_t n = F<int32_t>(sc, 0x3694)) {
        sum = (float)((double)sum / weights);
        int32_t c = FtolD((double)n - (double)weights * *(const float*)rt3::Rdata(0x5c9ac8));
        F<int32_t>(sc, 0x3694) = c;
        if (c < 1) F<int32_t>(sc, 0x3694) = 1;
        Render_SetVec2533(F<uint8_t*>(sc, 0x2d), 0, 0, *(uint32_t*)&sum, 0);
    } else {
        Render_SetVec2533(F<uint8_t*>(sc, 0x2d), 0, 0, 0, 0);
    }
    --next;
    Render_UnlockVb2(F<uint8_t*>(sc, 0x2d));
    Render_ResetVertexSlots(F<uint8_t*>(sc, 0x2d));
    for (uint16_t i = 1; i <= next; ++i) {
        *((uint16_t**)0xc43b38)[i - 1] = 0;
        *((uint8_t**)0xaf3530)[i - 1] = 0;
        *((uint8_t**)0xc83b38)[i - 1] = 0;
    }
}
RT3_REPLACE(0x525ae0, Terrain_BuildMesh);

// 0x526590. Fog density 0..79 from the fog factor (+0x21), the effect intensity
// and, above 0x5dcff0, the height +0x368e (x87 with the exe's _ftol).
RT3_NOINLINE int32_t __fastcall Scene_FogDensity(uint8_t* s) {
    float f21 = F<float>(s, 0x21);
    int32_t v;
    __asm {
        fld dword ptr ds:[0x5c85b4]
        fsub f21
        fsub qword ptr ds:[0x5c8758]
        fmul qword ptr ds:[0x5dcff8]
        fsubr qword ptr ds:[0x5dcff8]
        call Rt3_Ftol
        mov v, eax
    }
    v -= Fx_Intensity(s) >> 2;
    float h = F<float>(s, 0x368e);
    if ((double)h > *(const double*)rt3::Rdata(0x5dcff0)) {
        int32_t base = v;
        __asm {
            fld dword ptr ds:[0x5ca164]
            fsub h
            fmul qword ptr ds:[0x5d0c70]
            fimul base
            call Rt3_Ftol
            mov v, eax
        }
    }
    if (v > 0x4f) return 0x4f;
    if (v < 0) return 0;
    return v;
}
RT3_REPLACE(0x526590, Scene_FogDensity);

RT3_NOINLINE int32_t __fastcall Scene_NeedsRedraw(uint8_t* s) {
    if (Render_TakeFlag36f7(F<uint8_t*>(s, 0x2d), 0, 0)) return 1;
    return s[0x3749] || s[0x3748] ? 1 : 0;
}
RT3_REPLACE(0x526620, Scene_NeedsRedraw);

RT3_NOINLINE void __fastcall Scene_SetDetail(uint8_t* s, int, int32_t v) { F<int32_t>(s, 0x22b6b) = v < 1 ? 1 : v; }
RT3_REPLACE(0x526650, Scene_SetDetail);

// 0x526670. Marks terrain LOD levels for the cell range [u0, uEnd) x [v0, vEnd) at
// `level` (grid this +0x61, base shift +0x69): every cell writes level + 1 into the
// four grid corners; where its height variation (heights +0x1619 per level) times
// the level reaches the distance threshold (camera at +0x3682 / +0x3686, distance
// table 0x6d9098 -> 0xc33d38 offset by 0xaf3528), the cell is refined: above level
// 1 by recursing at twice the resolution, at level 1 by marking the 3x3 finest-grid
// block. Coordinates are 16-bit as in the original.
RT3_NOINLINE void __fastcall TerrainLod_Mark(uint8_t* self, int, int32_t levelArg, uint8_t* t, int32_t u0, int32_t v0, int32_t uEndArg,
                                int32_t vEndArg) {
    int32_t level = (int8_t)levelArg;
    int16_t uEnd = (int16_t)uEndArg, vEnd = (int16_t)vEndArg;
    uint32_t sh = (uint32_t)(F<int32_t>(self, 0x69) + level);
    int32_t step = 1 << (sh & 31);
    const uint16_t* H = (const uint16_t*)(uintptr_t)F<uint32_t>(t, 0x1619 + level * 4);
    int32_t W = (F<int32_t>(t, 0x15d9) >> (level & 31)) + 1;
    int32_t dyCam = (((int32_t)(int16_t)v0 << (level & 31)) - F<int32_t>(self, 0x3686));
    if (dyCam < 0) dyCam = -dyCam;
    int32_t lstep = 1 << (level & 31);
    F<uint32_t>(t, 0x15e1) = (uint32_t)level;
    const uint16_t* dist = (const uint16_t*)0x6d9098;
    const int16_t* thrTable = (const int16_t*)0xc33d38;
    uint8_t mark = (uint8_t)(levelArg + 1);
    auto threshold = [&](uint16_t d) {
        int32_t i = Ftol((double)(int16_t)d * g_6d9090) + g_af3528;
        return (int32_t)thrTable[(int16_t)i];
    };
    for (int32_t u = u0, ui = (int16_t)u0; (int16_t)u < uEnd; ++u, ++ui) {
        int32_t du = (ui << (level & 31)) - F<int32_t>(self, 0x3682);
        if (du < 0) du = -du;
        int16_t uLo = (int16_t)(u << (level & 31));
        int16_t uHi = (int16_t)((u + 1) << (level & 31));
        int32_t dv = dyCam;
        if ((int32_t)(uint16_t)v0 >= vEnd) continue;
        int32_t row0 = (int32_t)(int16_t)v0 << (sh & 31);
        int32_t row1 = ((int32_t)(int16_t)v0 + 1) << (sh & 31);
        int32_t v;
        do {
            uint8_t* G = F<uint8_t*>(self, 0x61);
            uint8_t* r0 = G + row0;
            uint8_t* r1 = G + row1;
            const uint16_t* dp = dist + ((uint32_t)(uint16_t)du * 0x418 + dv);
            v = v0;
            int32_t childV = v0 * 2 + 2;
            do {
                dv += lstep;
                uint16_t d = *dp;
                dp += lstep;
                int32_t thr = threshold(d);
                r1[uHi] = mark;
                r1[uLo] = mark;
                r0[uHi] = mark;
                r0[uLo] = mark;
                r0 += step;
                r1 += step;
                int32_t h = H[(uint32_t)(uint16_t)v * W + ui];
                if ((h - thr) * level >= thr) {
                    if (level > 1) {
                        TerrainLod_Mark(self, 0, (uint8_t)(levelArg - 1), t, u * 2, v * 2, u * 2 + 2, childV);
                    } else {
                        uint32_t s0 = (uint32_t)F<int32_t>(self, 0x69);
                        int32_t fs = 1 << (s0 & 31);
                        uint8_t* pb = F<uint8_t*>(self, 0x61) + ((uint32_t)(uint16_t)v << ((s0 + 1) & 31)) + ui * 2;
                        for (int y = 0; y < 3; ++y)
                            for (int x = 0; x < 3; ++x) pb[y * fs + x] = 1;
                    }
                }
                ++v;
                childV += 2;
            } while ((int32_t)(uint16_t)v < vEnd);
        } while ((int32_t)(uint16_t)(v + 1) < vEnd);
    }
}
RT3_REPLACE(0x526670, TerrainLod_Mark);

// 0x526b70. Top level of the terrain LOD pass over cells [u0, uEnd) x [v0,
// vEnd) at `level` (grid +0x61 and base shift +0x69 from the settings +0x65).
// A cell is kept when it is close enough (distance table 0x6d9098 at most
// 0x18), or else on screen: for mid distances (up to 0x2e) any corner, for
// far ones its centre with a wider margin (renderer +0x2d). A kept cell is
// listed at +0x374f ({u, v, grid cell, threshold}, count +0x22b53), marks
// its grid corners with level + 1, and is refined by 0x526670 where its
// height variation (+0x1619 per level) beats its threshold (scaled by
// +0x22b5b). The threshold offset 0xaf3528 is set from 0x8f2518 * 0x5dd000.
RT3_NOINLINE void __fastcall TerrainLod_Build(uint8_t* self, int, int32_t levelArg, uint8_t* t, int32_t u0, int32_t v0, int32_t uEnd, int32_t vEnd) {
    uint8_t* g = F<uint8_t*>(self, 0x65);
    F<int32_t>(self, 0x69) = F<int32_t>(g, 0x1f418);
    int32_t level = (int8_t)levelArg;
    uint32_t sh = (uint32_t)(F<int32_t>(g, 0x1f418) + level);
    F<uint32_t>(self, 0x61) = F<uint32_t>(g, 0x1f414);
    int32_t step = 1 << (sh & 31);
    float** slots = (float**)Terrain_HeightsSlot(t);
    float* H0 = slots[0];
    float* Hl = slots[level];
    int32_t rows1 = Terrain_Rows(t) + 1;
    uint32_t cellPx = 0x5c << (level & 31);
    uint16_t half = (uint16_t)((uint16_t)cellPx >> 1);
    const uint16_t* var = (const uint16_t*)(uintptr_t)Terrain_Get1619(t, 0, (int8_t)levelArg);
    int32_t W = (Terrain_Rows(t) >> (level & 31)) + 1;
    uint8_t* out = self + 0x374f;
    g_af3528 = Ftol((double)g_8f2518 * g_5dd000);
    if (!((int16_t)u0 < (int16_t)uEnd)) return;
    int32_t rowOff0 = (int32_t)(int16_t)v0 << (sh & 31);
    int32_t rowOff1 = ((int32_t)(int16_t)v0 + 1) << (sh & 31);
    uint16_t cpx = (uint16_t)cellPx;
    auto clip = [&](int32_t x, float z, int32_t y, uint32_t limit) {
        return Render_ProjectClip(F<uint8_t*>(self, 0x2d), 0, (float)x, z, (float)y, BitsF(limit), 1.0f, nullptr);
    };
    int32_t u = u0, ub = (int16_t)u0, u1 = u0 + 1, childU = u0 * 2 + 2;
    do {
        int32_t du = (ub << (level & 31)) - F<int32_t>(self, 0x3682);
        if (du < 0) du = -du;
        int32_t uLo = u << (level & 31), uHi = u1 << (level & 31);
        uint8_t* G = F<uint8_t*>(self, 0x61);
        uint8_t* r0 = G + rowOff0;
        uint8_t* r1 = G + rowOff1;
        int32_t v = v0;
        int32_t vi = (uint16_t)v0;
        if (vi < (int16_t)vEnd) {
            int32_t duRow = (int32_t)(uint16_t)du * 0x418;
            int32_t childV = v0 * 2 + 2;
            do {
                int32_t vs = vi << (level & 31);
                int32_t dv = vs - F<int32_t>(self, 0x3686);
                if (dv < 0) dv = -dv;
                const uint16_t* dp = (const uint16_t*)0x6d9098 + ((int16_t)dv + duRow);
                uint16_t d = *dp;
                bool keep;
                if (d > 0x2e)
                    keep = clip(ub * cpx + half, Hl[vi * W + ub], cpx * vi + half, 0x3ff33333) != 0;
                else if (d > 0x18) {
                    int32_t x0 = ub * cpx, y0 = vi * cpx;
                    int32_t row = vs * rows1;
                    keep = clip(x0, H0[(int16_t)uLo + row], y0, 0x3fe00000) != 0;
                    if (!keep) {
                        int32_t x1 = x0 + half * 2;
                        keep = clip(x1, H0[(int16_t)uHi + row], y0, 0x3fe00000) != 0;
                        if (!keep) {
                            int32_t row1 = ((vi + 1) << (level & 31)) * rows1;
                            int32_t y1 = y0 + half * 2;
                            keep = clip(x1, H0[(int16_t)uHi + row1], y1, 0x3fe00000) != 0;
                            if (!keep) keep = clip(x1 - half * 2, H0[row1 + (int16_t)uLo], y1, 0x3fe00000) != 0;
                        }
                    }
                } else
                    keep = true;
                if (keep) {
                    int32_t dd = (int16_t)*dp;
                    int32_t thr = Ftol((double)dd * g_6d9090) + g_af3528;
                    F<int32_t>(out, 0) = ub;
                    F<int32_t>(out, 4) = vi;
                    F<int32_t>(out, 8) = Render_GridCell(t, 0, ub, vi);
                    F<int32_t>(out, 0xc) = (int16_t)thr;
                    out += 0x10;
                    ++F<int32_t>(self, 0x22b53);
                    int32_t t2 = ((const int16_t*)0xc33d38)[(int16_t)thr];
                    int32_t thr2 = Ftol((double)t2 * F<float>(self, 0x22b5b));
                    uint8_t mark = (uint8_t)(levelArg + 1);
                    r1[(int16_t)uHi] = mark;
                    r1[(int16_t)uLo] = mark;
                    r0[(int16_t)uHi] = mark;
                    r0[(int16_t)uLo] = mark;
                    r1 += step;
                    r0 += step;
                    if ((int32_t)var[vi * W + ub] - (int16_t)thr2 > 0)
                        TerrainLod_Mark(self, 0, (uint8_t)(levelArg - 1), t, u * 2, v * 2, childU, childV);
                } else {
                    r0 += step;
                    r1 += step;
                }
                ++v;
                vi = (uint16_t)v;
                childV += 2;
            } while (vi < (int16_t)vEnd);
        }
        ++u;
        ++u1;
        childU += 2;
        ++ub;
    } while ((int16_t)u < (int16_t)uEnd);
}
RT3_REPLACE(0x526b70, TerrainLod_Build);

RT3_NOINLINE void __fastcall Scene_MarkDirty(uint8_t* s) { s[0x3749] = 1; }
RT3_REPLACE(0x5270d0, Scene_MarkDirty);

// 0x5270e0. Insertion sort of the 16-byte draw entries (+0x374f, count
// +0x22b53) by their key +8, ascending.
RT3_NOINLINE void __fastcall Scene_SortEntries(uint8_t* s) {
    uint32_t* a = (uint32_t*)(s + 0x374f);
    int32_t n = F<int32_t>(s, 0x22b53);
    for (int32_t i = 0; i < n; ++i) {
        uint32_t tmp[4] = { a[i * 4], a[i * 4 + 1], a[i * 4 + 2], a[i * 4 + 3] };
        int32_t j = i;
        for (; j > 0 && (int32_t)a[(j - 1) * 4 + 2] > (int32_t)tmp[2]; --j)
            for (int k = 0; k < 4; ++k) a[j * 4 + k] = a[(j - 1) * 4 + k];
        for (int k = 0; k < 4; ++k) a[j * 4 + k] = tmp[k];
    }
}
RT3_REPLACE(0x5270e0, Scene_SortEntries);

void __fastcall Scene_RestoreTarget(uint8_t* s) { Render_RestoreTarget(F<uint8_t*>(s, 0x2d)); }
RT3_REPLACE(0x5271a0, Scene_RestoreTarget);

// 0x5271b0. Bounding box (in 92-unit tiles) of the four ground corners of the view
// (+0x36e8.. x, +0x3708.. z), widened to include the camera tile (+0x3682/+0x3686),
// scaled down by `shift` with a margin (-1 / +2) and clipped to the terrain.
RT3_NOINLINE void __fastcall Scene_VisibleBounds(uint8_t* s, int, int32_t* x0, int32_t* y0, int32_t* x1, int32_t* y1, uint8_t* terrain,
                                    int32_t shift) {
    int32_t x = (int32_t)F<float>(s, 0x36e8) / 0x5c;
    int32_t z = (int32_t)F<float>(s, 0x3708) / 0x5c;
    *x0 = x;
    *y0 = z;
    *x1 = x;
    *y1 = z;
    for (int32_t k = 1; k < 4; ++k) {
        x = (int32_t)F<float>(s, 0x36e8 + k * 4) / 0x5c;
        z = (int32_t)F<float>(s, 0x3708 + k * 4) / 0x5c;
        if (*x0 >= x) *x0 = x;
        if (*y0 >= z) *y0 = z;
        if (*x1 <= x) *x1 = x;
        if (*y1 <= z) *y1 = z;
    }
    if (*x0 >= F<int32_t>(s, 0x3682)) *x0 = F<int32_t>(s, 0x3682);
    if (*y0 >= F<int32_t>(s, 0x3686)) *y0 = F<int32_t>(s, 0x3686);
    if (*x1 <= F<int32_t>(s, 0x3682)) *x1 = F<int32_t>(s, 0x3682);
    if (*y1 <= F<int32_t>(s, 0x3686)) *y1 = F<int32_t>(s, 0x3686);
    *x0 = (*x0 >> shift) - 1;
    *y0 = (*y0 >> shift) - 1;
    *x1 = (*x1 >> shift) + 2;
    *y1 = (*y1 >> shift) + 2;
    int32_t w = Terrain_Rows(terrain) >> shift;
    int32_t h = Terrain_Cols(terrain) >> shift;
    if (*x0 < 0) *x0 = 0;
    else if (*x0 > w) *x0 = w;
    if (*y0 < 0) *y0 = 0;
    else if (*y0 > h) *y0 = h;
    if (*x1 > w) *x1 = w;
    else if (*x1 < *x0) *x1 = *x0;
    if (*y1 > h) *y1 = h;
    else if (*y1 < *y0) *y1 = *y0;
}
RT3_REPLACE(0x5271b0, Scene_VisibleBounds);

// 0x527450. Projects a world point to normalised screen coordinates
// (x: 0..1 left to right, y: 1..0 bottom-up); 0 when culled.
RT3_NOINLINE int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz) {
    if (Render_Project(F<uint8_t*>(s, 0x2d), 0, x, y, z, ox, oy, oz)) return 0;
    float sx, sy;
    Render_GetViewport(F<uint8_t*>(s, 0x2d), 0, (uint32_t*)&sx, (uint32_t*)&sy, nullptr, nullptr);
    const double half = *(const double*)rt3::Rdata(0x5c8628);
    *ox = (float)((double)*ox / sx * half + half / sx);
    *oy = (float)(half / sy - (double)*oy / sy * half);
    return 1;
}
RT3_REPLACE(0x527450, Scene_ProjectNormalised);

RT3_NOINLINE void __fastcall Scene_SetLightPair0(uint8_t* s, int, int32_t i, float a, float b) {
    Render_SetLightPair(F<uint8_t*>(s, 0x2d), 0, ClampLight(i), 0, a, b);
}
RT3_REPLACE(0x5274e0, Scene_SetLightPair0);

RT3_NOINLINE void __fastcall Scene_SetLightPair1(uint8_t* s, int, int32_t i, float a, float b) {
    Render_SetLightPair(F<uint8_t*>(s, 0x2d), 0, ClampLight(i), 1, a, b);
}
RT3_REPLACE(0x527510, Scene_SetLightPair1);

float __fastcall Scene_GetLightValue0(uint8_t* s, int, int32_t i) { return Render_GetLightValue(F<uint8_t*>(s, 0x2d), 0, i, 0); }
RT3_REPLACE(0x527540, Scene_GetLightValue0);

RT3_NOINLINE void __fastcall Scene_SetLightRange(uint8_t* s, int, int32_t i, float v) { Render_SetLightRange(F<uint8_t*>(s, 0x2d), 0, i, v); }
RT3_REPLACE(0x527560, Scene_SetLightRange);

void __fastcall Scene_SetPair0(uint8_t* s, int, float a, float b) { Render_SetPair25f7(F<uint8_t*>(s, 0x2d), 0, 0, a, b); }
RT3_REPLACE(0x527570, Scene_SetPair0);

void __fastcall Scene_SetPair1(uint8_t* s, int, float a, float b) { Render_SetPair25f7(F<uint8_t*>(s, 0x2d), 0, 1, a, b); }
RT3_REPLACE(0x527590, Scene_SetPair1);

// 0x5275b0. Camera focus in cells (+0x3734, +0x373c scaled by 1/92); returns the flag byte +0x3746.
RT3_NOINLINE int32_t __fastcall Scene_FocusCell(uint8_t* s, int, float* x, float* y) {
    const float k = *(const float*)rt3::Rdata(0x5dcfe8);
    *x = (float)((double)F<float>(s, 0x3734) * k);
    *y = (float)((double)F<float>(s, 0x373c) * k);
    return s[0x3746];
}
RT3_REPLACE(0x5275b0, Scene_FocusCell);

// 0x5275e0. Splits seconds of day v into hours / minutes / seconds (any output may be null).
RT3_NOINLINE void __fastcall TimeOfDay_Split(int32_t v, int32_t* hours, int32_t* minutes, int32_t* seconds) {
    if (seconds) *seconds = v % 60;
    if (minutes) *minutes = (v / 60) % 60;
    if (hours) *hours = (v / 3600) % 24;
}
RT3_REPLACE(0x5275e0, TimeOfDay_Split);

// 0x527650. The debug statistics overlay of the scene `s` for a frame of
// `dt` seconds (the shown frame time is held for 150 ms, 0xcc3b44 / 0xcc3b48):
// with screen +0x114206 the frame rate alone (options +0x8c) or the full
// render stats (FPS, ms per frame, max index, polygons and million triangles
// per second, reflection polygons and ratio; trees, objects, sorted vertices,
// bursts); with options +0x18 the memory stats (KB / MB) and with +0x1a the
// texture changes per frame. The ten number slots sit 16 bytes apart in the
// frame, as in the original.
RT3_NOINLINE void __fastcall SceneStats_Draw(uint8_t* s, int, float dt) {
    if (ClockMs() - 0x96 < g_cc3b48) dt = g_cc3b44;
    else {
        g_cc3b44 = dt;
        g_cc3b48 = ClockMs();
    }
    char text[0x400];
    text[0] = 0;
    int32_t tris = Render_TriangleCount(F<uint8_t*>(s, 0x2d));
    float fps = dt == D(0x5c8608) ? 0.0f : (float)(D(0x5c87f8) / dt);
    int32_t shadow = 0;
    char area[0x70];  // the original's number slots at +0x20, +0x30, ... +0x80
    char* slot[7];
    for (int32_t i = 0; i < 7; ++i) slot[i] = area + i * 0x10;
    char big[0x200];
    auto nf = [](char* b, int32_t style, double v) { return Num_Format(b, style, v, 10, 1, 0, 0); };
    if (g_engine[0x114206]) {
        int32_t rate = Ftol((double)tris * fps);
        if (F<int32_t>(F<uint8_t*>(s, 0x29), 0x8c)) {
            char* n = nf(big, 4, (double)fps);
            FormatText(text, (const char*)0x5dd150, n, 0, 0, 0, 0, 0, 0, 0);
            shadow = 1;
        } else {
            float c87 = F<float>(s, 0x22b87);
            double ratio = (double)F<float>(s, 0x22b83) / (Fl(0x5c85b4) > c87 ? Fl(0x5c85b4) : c87);
            if (ratio > D(0x5c8770)) ratio = Fl(0x5c85b4);
            float fpsC = fps > Fl(0x5c85b4) ? fps : 1.0f;
            char* n1 = nf(slot[5], 3, ratio);
            char* n2 = nf(slot[6], 8, (double)F<int32_t>(s, 0x22b7b));
            char* n3 = nf(slot[1], 3, (double)rate * Fl(0x5dd14c));
            char* n4 = nf(slot[3], 8, (double)tris);
            char* n5 = nf(slot[2], 8, (double)g_cc3b34);
            char* n6 = nf(slot[4], 4, D(0x5c87f8) / fpsC);
            char* n7 = nf(slot[0], 4, (double)fps);
            FormatText(text, (const char*)0x5dd0d8, n7, n6, n5, n4, n3, n2, n1, 0);
            char* m1 = nf(slot[1], 3, (double)F<int32_t>(s, 0xd) * Fl(0x5dcfb8));
            char* m2 = nf(slot[3], 8, (double)g_ccd81c);
            char* m3 = nf(slot[2], 8, (double)hook::As<int32_t>(g_ccd818));
            char* m4 = nf(slot[4], 8, (double)g_cc3b38);
            char* m5 = nf(slot[0], 8, (double)g_cc3b3c);
            FormatText(text, (const char*)0x5dd090, text, m5, m4, m3, m2, m1, 0, 0);
        }
    }
    if (F<uint8_t*>(s, 0x29)[0x18]) {
        memcpy(text + strlen(text), (const void*)0x5c9ce8, 2);
        int32_t video, resident;
        Textures_MemoryTotals(&video, &resident, nullptr);
        int32_t total = Engine_TotalTextureMem(hook::As<void*>(g_engine));
        int32_t used = (total - Engine_FreeTextureMem(hook::As<void*>(g_engine))) / 1024;
        char* l1 = nf(slot[1], 1, (double)(resident / 1024));
        char* l2 = nf(slot[3], 1, (double)(video / 1024));
        char* l3 = nf(slot[2], 1, (double)used);
        char* l4 = nf(slot[4], 1, (double)(total / 1024));
        uint32_t z = (uint32_t)SysInfo_Zero();
        char* l5 = nf(slot[0], 3, (double)z * Fl(0x5dd08c));
        FormatText(big, (const char*)0x5dd028, l5, l4, l3, l2, l1, 0, 0, 0);
        strcat(text, big);
    }
    if (F<uint8_t*>(s, 0x29)[0x1a]) {
        memcpy(text + strlen(text), (const void*)0x5c9ce8, 2);
        char* n = nf(slot[0], 1, (double)F<int32_t>(F<uint8_t*>(s, 0x2d), 0x370d));
        FormatText(big, (const char*)0x5dd004, n, 0, 0, 0, 0, 0, 0, 0);
        strcat(text, big);
    }
    Overlay_DrawText(text, shadow);
}
RT3_REPLACE(0x527650, SceneStats_Draw);

// 0x527c20 / 0x527c70. Sets a time of day (normalised) into +0x11 / into +0xd and the engine.
void __fastcall Clock_Set11(uint8_t* o, int, int32_t h, int32_t m, int32_t s) { F<int32_t>(o, 0x11) = SecondsOfDay(h, m, s); }
RT3_REPLACE(0x527c20, Clock_Set11);

void __fastcall Clock_SetD(uint8_t* o, int, int32_t h, int32_t m, int32_t s) {
    int32_t v = SecondsOfDay(h, m, s);
    F<int32_t>(o, 0xd) = v;
    Engine_Set1146f6(hook::As<void*>(g_engine), 0, v);
}
RT3_REPLACE(0x527c70, Clock_SetD);

RT3_NOINLINE void __fastcall Clock_SplitD(uint8_t* o, int, int32_t* h, int32_t* m, int32_t* s) { TimeOfDay_Split(F<int32_t>(o, 0xd), h, m, s); }
RT3_REPLACE(0x527cc0, Clock_SplitD);

// 0x527ce0. (Re)builds the visit grid (+0x65) for the terrain size, after fixing
// the overview tile seams; nothing to do when the terrain is unchanged.
RT3_NOINLINE void __fastcall Scene_SetupVisitGrid(uint8_t* s, int, uint8_t* t) {
    int32_t rows = Terrain_Rows(t);
    int32_t cols = Terrain_Cols(t);
    if (F<void*>(s, 0x65)) {
        if (g_624d40 == rows && g_624d3c == cols &&
            g_624d38 == F<int32_t>(t, 0x2121))
            return;
        Render_FixTileSeams(t);
        F<int32_t>(s, 5) = 0;
        int32_t v = F<int32_t>(t, 0x2121);
        uint8_t* old = F<uint8_t*>(s, 0x65);
        g_624d40 = rows;
        g_624d3c = cols;
        g_624d38 = v;
        if (old) {
            VisitGrid_Free(old);
            crt::operator_delete(old);
        }
    } else {
        Render_FixTileSeams(t);
        g_624d40 = rows;
        g_624d3c = cols;
        g_624d38 = F<int32_t>(t, 0x2121);
    }
    int32_t n = cols > rows ? cols : rows;
    s[0x3749] = 1;
    uint8_t* g = (uint8_t*)crt::operator_new(0x3e83f);
    F<uint8_t*>(s, 0x65) = g ? VisitGrid_Init(g, 0, n + 1) : nullptr;
    uint32_t* a = (uint32_t*)0x8f2520;
    for (uint32_t k = 0; k < 0x80400; ++k) a[k] = 0;
    *(uint16_t*)(a + 0x80400) = 0;
    uint32_t* b = (uint32_t*)0xb33530;
    for (uint32_t k = 0; k < 0x40200; ++k) b[k] = 0;
    *(uint8_t*)(b + 0x40200) = 0;
}
RT3_REPLACE(0x527ce0, Scene_SetupVisitGrid);

// 0x527de0. Sets the map-tool brush radius: the brush marker's scale (x 5.5) and +0x3728 (x 4).
RT3_NOINLINE void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v) {
    SceneObj_SetScale36(F<uint8_t*>(s, 0x364d), 0, (float)((double)v * *(const double*)rt3::Rdata(0x5dd158)));
    F<float>(s, 0x3728) = (float)((double)v * *(const float*)rt3::Rdata(0x5c8568));
}
RT3_REPLACE(0x527de0, Scene_SetBrushRadius);

// 0x527e10. Whether scene s must rebuild its terrain view for camera cam (only
// while drawing is on, s+0x29 +0x61, and some terrain was drawn, +0x3694): when
// the camera moved more than 0x5c86b8 and further than its height above the
// ground allows, its distance to the target changed by more than that, its
// pitch or yaw (asin of the view direction) turned past 0x5dd170 / 0x5dd168,
// the camera changed (+0x3672), fog changed (flag 0x36f7, +0x3748 / +0x3749,
// at density 8 and up) or a rebuild was asked (+0x3747); otherwise, for a
// slow frame, after enough game time (by the view range +0x368a) or real time
// (0x5dd160 / terrain size). The view state (+0x36b8..+0x36cc, time 0xcc3b4c,
// game time 0x624d44) is updated on a rebuild. Only AL is meaningful.
RT3_NOINLINE uint8_t __fastcall Scene_ViewNeedsRebuild(uint8_t* s, int, uint8_t* cam) {
    if (!F<uint8_t*>(s, 0x29)[0x61]) return 0;
    uint32_t tick = imp::GetTickCount();
    int32_t drawn = F<int32_t>(s, 0x3694);
    if (!drawn) return 0;
    int32_t load = drawn - (F<int32_t>(s, 0x368a) >> 4);
    if (load < drawn) load = drawn;
    float pos[3], tgt[3], d[3];
    Obj_GetVec1c(cam, 0, (uint32_t*)pos);
    Obj_GetVec4(cam, 0, (uint32_t*)tgt);
    d[0] = (float)((double)pos[0] - tgt[0]);
    d[1] = (float)((double)pos[1] - tgt[1]);
    double dz = (double)pos[2] - tgt[2];
    d[2] = (float)dz;
    // The original squares z as its unrounded difference times its stored float.
    float dist = (float)sqrt((dz * d[2] + (double)d[0] * d[0]) + (double)d[1] * d[1]);
    Vec3_Normalize(d);
    float pitch, yaw;
    {
        float dirY = d[1], dirX = d[0];
        __asm {
            fld dirY
            mov eax, 0x5a3e90
            call eax
            fst pitch
            fcos
            fdivr qword ptr ds:[0x5c8770]
            fmul dirX
            mov eax, 0x5a3e90
            call eax
            fstp yaw
        }
    }
    double ex = (double)pos[0] - F<float>(s, 0x36c4), ey = (double)pos[1] - F<float>(s, 0x36c8),
           ez = (double)pos[2] - F<float>(s, 0x36cc);
    double movedExact = sqrt((ez * ez + ey * ey) + ex * ex);
    float moved = (float)movedExact;
    int32_t changed = 0;
    if (movedExact > *(const double*)rt3::Rdata(0x5c86b8)) {
        double h = Terrain_HeightAt((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, pos[0], pos[2]);
        double q = ((double)pos[1] - h) * *(const double*)rt3::Rdata(0x5c8918);
        if (moved > q) changed = 1;
        if (q < fabs((double)dist - F<float>(s, 0x36b8))) changed = 1;
    }
    changed |= fabs((double)pitch - F<float>(s, 0x36bc)) > *(const double*)rt3::Rdata(0x5dd170) ? 1 : 0;
    int32_t turned = fabs((double)yaw - F<float>(s, 0x36c0)) > *(const double*)rt3::Rdata(0x5dd168) ? 1 : 0;
    changed |= (F<uint8_t*>(s, 0x3672) != cam ? 1 : 0) | turned;
    int32_t fog = 0;
    if (Render_TakeFlag36f7(F<uint8_t*>(s, 0x2d), 0, 0) || s[0x3749] || s[0x3748]) fog = Scene_FogDensity(s) >= 8 ? 1 : 0;
    changed |= (int32_t)s[0x3747] | fog;
    s[0x3747] = 0;
    if (!F<int32_t>(s, 5)) return 0;
    if (!changed) {
        int32_t limit;
        {
            double v = *(const double*)rt3::Rdata(0x5dd160) / (double)F<int32_t>(s, 0x3694);
            __asm {
                fld v
                call Rt3_Ftol
                mov limit, eax
            }
        }
        bool slow = (int32_t)(tick - g_cc3b4c) / 10 > limit;
        int32_t wait = (F<int32_t>(s, 0x368a) >> 6) - (load >> 1) + 1000;
        if (wait < 0) wait = 0;
        else if (wait > 0x5dc) wait = 0x5dc;
        int32_t now = F<int32_t>(s, 0xd), last = g_624d44;
        int32_t t = now;
        if (last > t) t += 0x15180;
        if (t - last > wait + 0x40) g_624d44 = now;
        else if (load < 0x180 || !slow) return 0;
    }
    g_cc3b4c = tick;
    F<float>(s, 0x36c4) = pos[0];
    F<float>(s, 0x36c8) = pos[1];
    F<float>(s, 0x36cc) = pos[2];
    F<float>(s, 0x36b8) = dist;
    F<float>(s, 0x36bc) = pitch;
    F<float>(s, 0x36c0) = yaw;
    return 1;
}
RT3_REPLACE(0x527e10, Scene_ViewNeedsRebuild);

RT3_NOINLINE double __fastcall Obj_Scene36f9(uint8_t* o) { return F<float>(F<uint8_t*>(o, 0x2d), 0x36f9); }
RT3_REPLACE(0x528180, Obj_Scene36f9);

// 0x528190. Hands every terrain vertex of visitor's range (x0..x1 / y0..y1 in
// its steps; within its radius of the camera, in cells of 0x5dcfe8, when that is
// under 1000) that projects onto the screen to its slot 3 as (screen x, y, depth,
// cell x, cell y).
RT3_NOINLINE void __fastcall Scene_VisitTerrain(uint8_t* s, int, uint8_t* visitor) {
    Fade_StartFast();
    int32_t bx0, by0, bx1, by1;
    Scene_VisibleBounds(s, 0, &bx0, &by0, &bx1, &by1, visitor, 0);
    int32_t x0, y0, x1, y1, xs, ys, r;
    Render_GetFog(visitor, 0, (uint32_t*)&x0, (uint32_t*)&y0, (uint32_t*)&x1, (uint32_t*)&y1, (uint32_t*)&xs, (uint32_t*)&ys,
                  (uint32_t*)&r);
    int32_t limit = r < 1000 ? 1 : 0;
    int32_t rr = (int32_t)((uint32_t)r * (uint32_t)r);
    float cam[3];
    Obj_GetXYZ1c(F<uint8_t*>(g_engine, 0x28), 0, (uint32_t*)&cam[0], (uint32_t*)&cam[1],
                 (uint32_t*)&cam[2]);
    double k = *(const float*)rt3::Rdata(0x5dcfe8);
    int32_t camX, camZ;
    double vx = (double)cam[0] * k, vz = (double)cam[2] * k;
    __asm {
        fld vx
        call Rt3_Ftol
        mov camX, eax
        fld vz
        call Rt3_Ftol
        mov camZ, eax
    }
    float** grid = (float**)Terrain_HeightsSlot(visitor);
    int32_t rows = Terrain_Rows(visitor);
    typedef void(__thiscall * VisitFn)(uint8_t*, uint32_t, uint32_t, uint32_t, int32_t, int32_t);
    for (int32_t x = x0; x < x1; x += xs) {
        float fx = (float)(x * 0x5c);
        for (int32_t y = y0; y < y1; y += ys) {
            uint32_t dx = (uint32_t)(x - camX), dy = (uint32_t)(y - camZ);
            if (limit && (int32_t)(dy * dy + dx * dx) > rr) continue;
            float h = (*grid)[(rows + 1) * y + x];
            float ox, oy, oz;
            if (Scene_ProjectNormalised(s, 0, fx, h, (float)(y * 0x5c), &ox, &oy, &oz))
                ((VisitFn)(*(void***)visitor)[3])(visitor, *(uint32_t*)&ox, *(uint32_t*)&oy, *(uint32_t*)&oz, x, y);
        }
    }
    Engine_ResetTextureQueue(hook::As<void*>(g_engine));
}
RT3_REPLACE(0x528190, Scene_VisitTerrain);

// 0x528360. Picks the terrain point under the cursor (+0x3720 / +0x3724 in
// 0..1): marches from the unprojected cursor point +0x3734 away from the
// camera `cam` in steps of half the height above the ground (at least 2, a
// minimum that grows tenfold while the ray stays over the map), up to 0x80
// steps (+0x32 per growth, below 0x100). +0x3746: 0 no hit, 1 hit, 2 hit off
// the map; +0x3718 / +0x371c the cell.
RT3_NOINLINE void __fastcall View_PickTerrain(uint8_t* v, int, uint8_t* cam, uint8_t* terrain) {
    float eye[3];
    Obj_GetXYZ1c(cam, 0, (uint32_t*)&eye[0], (uint32_t*)&eye[1], (uint32_t*)&eye[2]);
    float* px = (float*)(v + 0x3734);
    float* py = (float*)(v + 0x3738);
    float* pz = (float*)(v + 0x373c);
    double one = *(const double*)rt3::Rdata(0x5c8770), half = *(const double*)rt3::Rdata(0x5c8628);
    float sy = (float)(one - ((double)F<float>(v, 0x3724) + F<float>(v, 0x3724)));
    float sx = (float)((double)F<float>(v, 0x3720) + F<float>(v, 0x3720) - one);
    Render_Unproject(F<uint8_t*>(v, 0x2d), 0, sx, sy, px, py, pz);
    double dx = (double)eye[0] - *px;
    double dy = (double)eye[1] - *py;
    *(uint32_t*)py = *(uint32_t*)&eye[1];
    double dz = (double)eye[2] - *pz;
    *(uint32_t*)px = *(uint32_t*)&eye[0];
    float fdz = (float)dz;
    *(uint32_t*)pz = *(uint32_t*)&eye[2];
    double len = sqrt(dz * fdz + dy * dy + dx * dx) * half;
    float k = (float)((double)*(const float*)rt3::Rdata(0x5c85b4) / len);
    float stepX = (float)(k * dx);
    float stepY = (float)(k * dy);
    float stepZ = (float)(k * (double)fdz);
    int32_t maxX = Terrain_Rows(terrain) * 0x5c - 0x5c;
    int32_t maxZ = Terrain_Cols(terrain) * 0x5c - 0x5c;
    v[0x3746] = 0;
    double h = Terrain_HeightAt(terrain, 0, *px, *pz);
    double st = fabs((double)*py - h) * half;
    double lo = *(const double*)rt3::Rdata(0x5c85d0);
    if (lo > st) st = lo;
    float step = (float)st;
    int32_t i = 0, limit = 0x80, minStep = 2;
    while (i < limit) {
        do {
            ++i;
            float nx = (float)((double)*px - (double)step * stepX);
            *px = nx;
            *py = (float)((double)*py - (double)step * stepY);
            float nz = (float)((double)*pz - (double)step * stepZ);
            *pz = nz;
            double g = Terrain_HeightAt(terrain, 0, nx, nz);
            float fg = (float)g;
            if (g + (double)*(const float*)rt3::Rdata(0x5c8568) > (double)*py) {
                *py = fg;
                v[0x3746] = 1;
                goto done;
            }
            double d = ((double)*py - fg) * half;
            double m = (double)minStep;
            step = (float)(m > d ? m : d);
        } while (i < limit);
        if (v[0x3746]) break;
        double zero = (double)*(const float*)rt3::Rdata(0x5c8598);
        if (limit < 0x100 && (double)*px > zero && (double)(Terrain_Rows(terrain) * 0x5c) > (double)*px && (double)*pz > zero &&
            (double)(Terrain_Cols(terrain) * 0x5c) > (double)*pz) {
            minStep *= 10;
            limit += 0x32;
        }
    }
done:
    if (v[0x3746]) {
        double zero = (double)*(const float*)rt3::Rdata(0x5c8598);
        bool off = (double)*px < zero || (double)maxX < (double)*px || (double)*pz < zero || (double)maxZ < (double)*pz;
        v[0x3746] = (uint8_t)(v[0x3746] + (off ? 1 : 0));
    }
    double cell = *(const float*)rt3::Rdata(0x5dcfe8);
    F<int32_t>(v, 0x3718) = FtolD((double)*px * cell);
    F<int32_t>(v, 0x371c) = FtolD((double)*pz * cell);
}
RT3_REPLACE(0x528360, View_PickTerrain);

// 0x528690. Draws view s's placement marker (+0x364d; when +0x3745 and the
// marker is active, +0x3741 with +0x3746 == 1): picks the terrain point
// first unless held (+0x3730), shows the frame for small scales (+0x36 below
// 0x5c8948), unlit and alpha tested at the point (+0x3734..) tinted -+0x3728,
// turning with the clock (GetTickCount & 0x1fff x 0x5dd178 x 0x5c8768).
RT3_NOINLINE void __fastcall Marker_Render(uint8_t* s, int, uint8_t* cam, uint8_t* terrain) {
    uint32_t ticks = (imp::GetTickCount)() & 0x1fff;
    float spin = (float)((double)(int32_t)ticks * *(const float*)rt3::Rdata(0x5dd178) * *(const float*)rt3::Rdata(0x5c8768));
    if (!s[0x3745]) return;
    if (!F<int32_t>(s, 0x3730)) View_PickTerrain(s, 0, cam, terrain);
    if (!F<int32_t>(s, 0x3741) || s[0x3746] != 1) return;
    uint8_t* m = F<uint8_t*>(s, 0x364d);
    Model_SetFrame(m, 0, F<float>(m, 0x36) < *(const float*)rt3::Rdata(0x5c8948) ? 1 : 0);
    Render_BindVb2647(F<uint8_t*>(s, 0x2d));
    Render_BlendDefault(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_CullNone(F<uint8_t*>(s, 0x2d));
    Render_ZWriteOff(F<uint8_t*>(s, 0x2d));
    Render_LightingOff(F<uint8_t*>(s, 0x2d));
    uint32_t tex = Obj_Get1c(F<uint8_t*>(s, 0x364d));
    Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, (uint8_t*)(uintptr_t)tex);
    SceneObj_SetAlpha(F<uint8_t*>(s, 0x364d), 0, 0xff, 1);
    SceneObj_SetTint(F<uint8_t*>(s, 0x364d), 0, 0xff, 0xff, 0xff, 1);
    float tint = -F<float>(s, 0x3728);
    Model_PrepareTint(F<uint8_t*>(s, 0x364d), 0, *(int32_t*)&tint);
    Obj_MoveTo(F<uint8_t*>(s, 0x364d), 0, F<float>(s, 0x3734), F<float>(s, 0x3738), F<float>(s, 0x373c));
    SceneObj_SetAngles(F<uint8_t*>(s, 0x364d), 0, 0, *(uint32_t*)&spin, 0);
    SceneObj_UpdateMatrix(F<uint8_t*>(s, 0x364d));
    Render_ApplyLocal(F<uint8_t*>(s, 0x2d), 0, F<uint8_t*>(s, 0x364d));
    Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
    uint32_t* vb = (uint32_t*)Scene_BeginBatch(s, 0, 0x1000);
    SceneObj_EmitFlat(F<uint8_t*>(s, 0x364d), 0, vb, &F<int32_t>(s, 0x71));
    Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
    if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    Render_ZWriteOn(F<uint8_t*>(s, 0x2d));
}
RT3_REPLACE(0x528690, Marker_Render);

// 0x528860. Draws view s's sky (scene option +0x29 +0x58, dome +0x99):
// on multitexture cards first the dome swaying with the time of day
// (+0x1d, damped by the effect intensity), then with the sky rotation the
// stars (+0xa5 as points, faded and fogged by darkness +0x21, randomly
// twinkling), clouds (+0x9d, above 0x5ca0c8 daylight, with the sun flare
// +0x3665 placed at the projected sun and dimmed by terrain occlusion) and
// night layer (+0xa1); then the fog `fog`, cloud shadows and default states.
RT3_NOINLINE void __fastcall Sky_Render(uint8_t* s, int, float fog) {
    if (!F<uint8_t*>(s, 0x29)[0x58] || !F<uint8_t*>(s, 0x99)) return;
    uint8_t* r = F<uint8_t*>(s, 0x2d);
    Render_BindVb263f(r);
    Render_BlendOff(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOff(F<uint8_t*>(s, 0x2d));
    Render_LightingOff(F<uint8_t*>(s, 0x2d));
    Render_SetZEnable(F<uint8_t*>(s, 0x2d), 0, 0);
    Render_ZWriteOff(F<uint8_t*>(s, 0x2d));
    Render_CullNone(F<uint8_t*>(s, 0x2d));
    Render_UseView1(F<uint8_t*>(s, 0x2d));
    uint8_t* e = g_engine;
    if (e[0x114251] && e[0x114250]) {
        Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x49));
        Render_SetFactorAlpha(F<uint8_t*>(s, 0x2d), 0, 0);
        int32_t k = Fx_Intensity(s);
        float a = F<float>(s, 0x1d), sway;
        __asm {
            fild k
            mov eax, 0x5dd17c
            fmul dword ptr [eax]
            mov eax, 0x5c85b4
            fsubr dword ptr [eax]
            fld a
            fchs
            fsin
            fmulp st(1), st
            mov eax, 0x5c86b8
            fmul qword ptr [eax]
            fstp sway
        }
        Sky_Place(F<uint8_t*>(s, 0x2d), F<uint8_t*>(s, 0x99), 0, *(uint32_t*)&sway);
        Sky_Draw(F<uint8_t*>(s, 0x2d), F<uint8_t*>(s, 0x99));
        Render_PopStages(F<uint8_t*>(s, 0x2d), 0, 1);
    }
    Render_BlendDefault(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 2);
    float spin = (float)(F<float>(s, 0x1d) + Fl(0x5c8760));
    Sky_Place(F<uint8_t*>(s, 0x2d), F<uint8_t*>(s, 0xa5), 0xbe19999a, *(uint32_t*)&spin);
    if (!s[0x3692] && F<float>(s, 0x21) < D(0x5c8758)) {
        Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x51));
        uint32_t fade = (uint32_t)Ftol(((double)F<float>(s, 0x21) - D(0x5c8758)) * D(0x5c9be8) * Fl(0x5ca0ec));
        if (fade > 0xff) fade = 0xff;
        Render_SetTextureFactor(F<uint8_t*>(s, 0x2d), 0, fade << 24);
        int32_t n = Fx_Intensity(s) + 0x40;
        if (n < 0x14) n = 0x14;
        uint32_t seed = g_lcg * 0x19660d + 0x3c6ef35f;
        g_lcg = seed;
        int32_t twinkle = (int32_t)(seed & 7) + n;
        Renderer_SetFog(F<uint8_t*>(s, 0x2d), 0, (float)twinkle, 1.0f, 1.0f, 1.0f);
        Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, (uint8_t*)(uintptr_t)Obj_Get1c(F<uint8_t*>(s, 0xa5)));
        Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 1, -1);
        uint8_t* st = F<uint8_t*>(s, 0xa5);
        Render_DrawPoints(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(st, 0x1ee), F<int32_t>(st, 0x1fa));
        Renderer_SetFog(F<uint8_t*>(s, 0x2d), 0, (float)(D(0x5c8770) - (double)F<float>(s, 0x36a0) / F<float>(s, 0x22b67)), 1.0f, 1.0f, 1.0f);
        Render_PopStages(F<uint8_t*>(s, 0x2d), 0, 1);
        Render_SetFactorAlpha(F<uint8_t*>(s, 0x2d), 0, 0);
    }
    Renderer_SetFog(F<uint8_t*>(s, 0x2d), 0, fog, 1.0f, 1.0f, 1.0f);
    Sky_Place(F<uint8_t*>(s, 0x2d), F<uint8_t*>(s, 0xa5), 0xbe19999a, F<uint32_t>(s, 0x1d));
    Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x41));
    bool flare = false;
    if (F<float>(s, 0x21) > D(0x5ca0c8)) {
        Sky_Draw(F<uint8_t*>(s, 0x2d), F<uint8_t*>(s, 0x9d));
        if (!s[0x3692] && F<float>(s, 0x21) > D(0x5ca180)) {
            flare = true;
            float sx = 0, sy = 0, v[3];
            Obj_GetVec0(F<uint32_t*>(s, 0x3665), 0, (uint32_t*)v);
            int32_t vis = Render_ProjectPoint(F<uint8_t*>(s, 0x2d), 0, v[0], v[1], v[2], &sx, &sy);
            vis += Fx_Intensity(s) * 2;
            if (vis >= 0xff) {
                vis = 0xff;
            } else {
                float cam[3] = { F<float>(s, 0x369c), F<float>(s, 0x36a0), F<float>(s, 0x36a4) };
                Render_TransformWorld(F<uint8_t*>(s, 0x2d), 0, v);
                vis += Terrain_RayOcclusion((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), cam, v);
                if (vis > 0xff) vis = 0xff;
                else if (vis < 0x40) vis = 0x40;
            }
            Obj_SetPair10(F<uint8_t*>(s, 0x3665), 0, *(uint32_t*)&sx, *(uint32_t*)&sy, (uint8_t)vis);
        }
    }
    if (!flare) Obj_SetPair10(F<uint8_t*>(s, 0x3665), 0, 0, 0, 0xff);
    if (F<float>(s, 0x21) < D(0x5c86b8)) Sky_Draw(F<uint8_t*>(s, 0x2d), F<uint8_t*>(s, 0xa1));
    Render_PopStages(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_CullCcw(F<uint8_t*>(s, 0x2d));
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    Renderer_SetFog(F<uint8_t*>(s, 0x2d), 0, fog, 1.0f, 1.0f, 1.0f);
    CloudShadows_Render(s);
    Render_ZWriteOn(F<uint8_t*>(s, 0x2d));
    Render_SetZEnable(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
}
RT3_REPLACE(0x528860, Sky_Render);

// 0x528d90. Scene r: queues node n and its subtree for drawing. Skips
// nodes that say so (method 5) and, in the shadow pass (or with +0x3692),
// nodes without flag 0x20. With `cull`, far meshes (0x170 from the camera c)
// facing away (view direction +0x3676) or projecting outside the view are
// dropped; drawn nodes get the matrix stack +0x5d, a tick (method 0), their
// level of detail (dropped beyond +0x3e or with +0x5c bit 1) and count
// 0xcc3b38. Meshes stamp the frame +5 into +0x65, fill their vertex range
// once per mesh (+0x39 / +0x3d; the buffer +0x91 restarts, flushing the lit
// groups +0x7d, when 0xffbf would be passed) and record pick hits along the
// ray (origin o, direction d; when `pick` and the node is closer than its
// frame radius) with their distance (+0xa9 / +0xad, count +0x4a9); the node
// goes into its model's list (+0xdc; the model into +0x7d once, +0xe0).
// Outside the shadow pass, emitters (+0x79, into +0x81 / +0x85 by bit
// 0x2000) and lights (+0x7d into +0x89; bright ones only while r+0x21 is
// below -0.15) are queued. Then the children (+0x75) whose level-of-detail
// range admits lod recurse.
RT3_NOINLINE void __fastcall Scene_QueueNode(uint8_t* r, int, uint8_t* n, uint8_t cull, float cx, float cy, float cz, float ox, float oy, float oz, float dx, float dy,
                                float dz, uint8_t pick, int32_t lod, uint8_t shadow) {
    if (((int32_t(__fastcall*)(uint8_t*))(*(void***)n)[5])(n)) return;
    if ((shadow || r[0x3692]) && !(n[0x20] & 0x20)) return;
    float o[3] = { ox, oy, oz }, d[3] = { dx, dy, dz };
    float x = F<float>(n, 0x1e2), y = F<float>(n, 0x1e6), z = F<float>(n, 0x1ea);
    bool hasMesh = F<int32_t>(n, 0x10) != 0;
    auto meshOf = [&]() { return F<uint8_t*>(F<uint8_t*>(n, 0x14), F<int32_t>(n, 0x3a) * 0x18 + 0xc) + F<int32_t>(n, 0x21) * 0x45; };
    uint8_t* mesh = nullptr;
    bool nearRay = false;
    if (hasMesh && F<uint8_t*>(n, 0x14)) mesh = meshOf();
    if (cull) {
        if (!hasMesh) {
            if (!Render_ProjectClip(F<uint8_t*>(r, 0x2d), 0, x, y, z, 1.0f, F<float>(r, 0x36a8), nullptr)) return;
        } else {
            int32_t ax = Ftol((double)x - cx), az = Ftol((double)z - cz);
            if ((ax < 0 ? -ax : ax) > 0x170 || (az < 0 ? -az : az) > 0x170) {
                double dot = (((double)cz - F<float>(n, 0x1ea)) * F<float>(r, 0x367e) + ((double)cy - F<float>(n, 0x1e6)) * F<float>(r, 0x367a)) +
                             ((double)cx - F<float>(n, 0x1e2)) * F<float>(r, 0x3676);
                if (dot < 0.0) return;
                float scale = (float)((double)F<float>(mesh, 0x14) / (dot - D(0x5c8970)) + D(0x5c8770));
                if (!Render_ProjectClip(F<uint8_t*>(r, 0x2d), 0, x, y, z, scale, F<float>(r, 0x36a8), nullptr)) return;
            }
        }
        void* ms = F<void*>(r, 0x5d);
        ((void(__stdcall*)(void*))(*(void***)ms)[5])(ms);
        SceneObj_ApplyStack(n, 0, F<void*>(r, 0x5d));
        uint32_t now = imp::GetTickCount();
        ((void(__fastcall*)(uint8_t*, int, uint32_t))(*(void***)n)[0])(n, 0, now);
        if (hasMesh) {
            lod = SceneObj_SelectLod(n, 0, (float)((double)cx - x), (float)((double)cy - y), (float)((double)cz - z));
            mesh = (uint8_t*)(uintptr_t)Obj_AnimFrame(n);
            if (F<int32_t>(n, 0x3e) >= 0 && lod > F<int32_t>(n, 0x3e)) return;
            if (n[0x5c] & 2) return;
        }
        ++g_cc3b38;
    }
    F<uint32_t>(n, 0x65) = F<uint32_t>(r, 5);
    if (hasMesh) {
        if (pick && !n[0x8d]) {
            float f = (float)PointLineDistance(o[0], o[1], o[2], d[0], d[1], d[2], F<float>(n, 0x1e2), F<float>(n, 0x1e6), F<float>(n, 0x1ea));
            if (f < F<float>((uint8_t*)(uintptr_t)Obj_AnimFrame(n), 0x14)) nearRay = true;
        }
        auto recordHit = [&]() {
            int32_t ax = Ftol((double)x - cx), ay = Ftol((double)y - cy), az = Ftol((double)z - cz);
            ax = ax < 0 ? -ax : ax, ay = ay < 0 ? -ay : ay, az = az < 0 ? -az : az;
            int32_t i = F<int32_t>(r, 0x4a9);
            F<uint8_t*>(r, 0xa9 + i * 8) = n;
            F<float>(r, 0xad + F<int32_t>(r, 0x4a9) * 8) = (float)sqrt(((double)az * az + (double)ay * ay) + (double)ax * ax);
            ++F<int32_t>(r, 0x4a9);
        };
        uint32_t end;
        if (F<int32_t>(mesh, 0x39) == -1) {
            int32_t verts = F<int32_t>(meshOf(), 0xc) * 3;
            int32_t first = F<int32_t>(r, 0x6d), last = first + verts;
            void* vb;
            if (last >= 0xffbf || first == -1) {
                if (last >= 0xffbf) {
                    Render_DrawLitGroups(F<uint8_t*>(r, 0x2d), 0, F<PtrList*>(r, 0x7d), 1);
                    PtrList_Clear(F<PtrList*>(r, 0x7d));
                    Meshes_InvalidateAll();
                }
                F<int32_t>(r, 0x6d) = 0;
                vb = Render_LockVbDiscard(F<uint8_t*>(r, 0x2d), 0, verts);
            } else
                vb = Render_LockVbRange(F<uint8_t*>(r, 0x2d), 0, first, last);
            F<void*>(r, 0x91) = vb;
            F<int32_t>(mesh, 0x39) = F<int32_t>(r, 0x6d);
            F<int32_t>(n, 0x1ee) = F<int32_t>(r, 0x6d);
            int32_t count = 0;
            if (nearRay && pick && !F<int32_t>(r, 0x366e)) {
                if ((uint8_t)Mesh_EmitPickTriangles(n, 0, F<uint8_t*>(r, 0x91), &count, d, o)) recordHit();
            } else
                Mesh_EmitPickTriangles(n, 0, F<uint8_t*>(r, 0x91), &count, nullptr, nullptr);
            Render_UnlockVbRange(F<uint8_t*>(r, 0x2d));
            F<uint8_t*>(r, 0x91) += count * 0x20;
            F<int32_t>(r, 0x6d) += count;
            F<int32_t>(mesh, 0x3d) = F<int32_t>(r, 0x6d);
            end = F<uint32_t>(r, 0x6d);
        } else {
            if (nearRay && pick && !F<int32_t>(r, 0x366e) && (uint8_t)Mesh_RayHit(n, 0, d, o)) recordHit();
            F<int32_t>(n, 0x1ee) = F<int32_t>(mesh, 0x39);
            end = F<uint32_t>(mesh, 0x3d);
        }
        F<uint32_t>(n, 0x1fa) = end;
        uint8_t* model = (uint8_t*)(uintptr_t)Obj_Get1c(n);
        if (!F<void*>(model, 0xe0)) F<PtrListNode*>(model, 0xe0) = PtrList_Append(F<PtrList*>(r, 0x7d), 0, model);
        PtrList_Append(F<PtrList*>(model, 0xdc), 0, n);
    }
    if (!shadow && !r[0x3692]) {
        int32_t az = Ftol((double)cz - z), ax = Ftol((double)cx - x);
        int32_t dist = (az < 0 ? -az : az) + (ax < 0 ? -ax : ax);
        if (PtrList* l = F<PtrList*>(n, 0x79)) {
            PtrList_Rewind(l);
            for (uint8_t* e = (uint8_t*)PtrList_Next(F<PtrList*>(n, 0x79)); e; e = (uint8_t*)PtrList_Next(F<PtrList*>(n, 0x79))) {
                F<int32_t>(e, 0x1d9) = dist;
                PtrList_Append(F<uint32_t>(e, 0xac) & 0x2000 ? F<PtrList*>(r, 0x81) : F<PtrList*>(r, 0x85), 0, e);
                Emitter_Tick(e, 0, 0);
            }
        }
        if (PtrList* l = F<PtrList*>(n, 0x7d)) {
            bool dark = (double)F<float>(r, 0x21) < D(0x5dd180);
            PtrList_Rewind(l);
            for (uint8_t* e = (uint8_t*)PtrList_Next(F<PtrList*>(n, 0x7d)); e; e = (uint8_t*)PtrList_Next(F<PtrList*>(n, 0x7d)))
                if (Flags24_Low(e) && ((e[0x24] & 0xf0) || dark)) PtrList_Append(F<PtrList*>(r, 0x89), 0, e);
        }
    }
    if (PtrList* l = F<PtrList*>(n, 0x75)) {
        PtrList_Rewind(l);
        for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(n, 0x75)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(n, 0x75))) {
            if (F<int32_t>(c, 0x3e) >= 0 && lod > F<int32_t>(c, 0x3e)) continue;
            uint8_t* cm = F<uint8_t*>(c, 0x14);
            if (!cm || F<int32_t>(cm, 0x14) <= 1) lod = 0;
            if (lod < (cm ? F<int32_t>(cm, 0x14) : 0) && Model_SetFrame(c, 0, lod))
                Scene_QueueNode(r, 0, c, 0, cx, cy, cz, o[0], o[1], o[2], d[0], d[1], d[2], 0, lod, shadow);
        }
    }
}
RT3_REPLACE(0x528d90, Scene_QueueNode);

// 0x5294b0. Scene r (when shown, +0x29 -> +0x5b, and the engine isn't
// minimised, +0x114266): queues the frame from camera cam over terrain
// ter. Clears the light / emitter lists, the pick (+0x366e) and its hits;
// marks the always-visible objects in view; then (camera below 33700) walks
// the r+0x22b53 visible cells (+0x374f, 16 bytes each) within the view
// distance (Render_Get1534, by the distance table 0x6d9098 from the centre
// cell +0x3682 / +0x3686) and queues their objects with culling, the first
// cell also casting the cursor ray (normalised) and binding the vertex
// buffer. The nearest pick hit becomes +0x366e; the lit groups are drawn.
// QUIRK: in the shadow pass the ray handed to Scene_QueueNode is stack
// residue (unused there); zero here.
RT3_NOINLINE void __fastcall Scene_QueueWorld(uint8_t* r, int, uint8_t* cam, uint8_t* ter, uint8_t shadow) {
    g_cc3b38 = 0;
    if (!F<uint8_t*>(r, 0x29)[0x5b] || F<int32_t>(g_engine, 0x114266)) return;
    int32_t maxDist = (int32_t)Render_Get1534(ter);
    float c[3];
    Obj_GetXYZ1c(cam, 0, (uint32_t*)&c[0], (uint32_t*)&c[1], (uint32_t*)&c[2]);
    bool started = false;
    int32_t cells = F<int32_t>(r, 0x22b53);
    const int32_t* cell = (const int32_t*)(r + 0x374f);
    PtrList_Clear(F<PtrList*>(r, 0x7d));
    PtrList_Clear(F<PtrList*>(r, 0x81));
    PtrList_Clear(F<PtrList*>(r, 0x85));
    F<int32_t>(r, 0x366e) = 0;
    F<int32_t>(r, 0x4a9) = 0;
    PtrList* all = Terrain_GlobalList(ter);
    PtrList_Rewind(all);
    for (uint8_t* o = (uint8_t*)PtrList_Next(all); o; o = (uint8_t*)PtrList_Next(all))
        if (!shadow && !r[0x3692] &&
            Render_ProjectClip(F<uint8_t*>(r, 0x2d), 0, F<float>(o, 0x1e2), F<float>(o, 0x1e6), F<float>(o, 0x1ea), 1.0f, 1.0f, nullptr))
            SceneObj_DrawMarker(o);
    if (!((double)c[1] <= D(0x5dd188))) return;
    float origin[3] = {}, dir[3] = {};
    uint8_t pick = 0;
    for (;;) {
        if (cells < 0) cells = 0;
        if (!cells) break;
        --cells;
        int32_t cx = cell[0], cy = cell[1];
        cell += 4;
        int32_t dx = F<int32_t>(r, 0x3682) - cx * 16 - 8, dy = F<int32_t>(r, 0x3686) - cy * 16 - 8;
        dx = dx < 0 ? -dx : dx;
        dy = dy < 0 ? -dy : dy;
        if ((int32_t)((const uint16_t*)0x6d9098)[dx * 0x418 + dy] > maxDist) continue;
        PtrList* l = Terrain_Cell1671(ter, 0, cx, cy);
        PtrList_Rewind(l);
        uint8_t* o = (uint8_t*)PtrList_Next(l);
        if (!started) {
            if (!o) continue;
            if (!shadow && !r[0x3692]) {
                origin[0] = c[0];
                origin[1] = c[1];
                origin[2] = c[2];
                pick = Scene_CursorRay(r, 0, dir, origin);  // into dir, from origin
                double inv = D(0x5c8770) / sqrt(((double)dir[2] * dir[2] + (double)dir[1] * dir[1]) + (double)dir[0] * dir[0]);
                dir[0] = (float)(dir[0] * inv);
                dir[1] = (float)(dir[1] * inv);
                dir[2] = (float)(dir[2] * inv);
            } else
                pick = 0;
            Render_BindVb2643(F<uint8_t*>(r, 0x2d));
            started = true;
        }
        for (; o; o = (uint8_t*)PtrList_Next(l)) {
            Scene_QueueNode(r, 0, o, 1, c[0], c[1], c[2], origin[0], origin[1], origin[2], dir[0], dir[1], dir[2], pick, 0, shadow);
            if ((o[0x20] & 0x40) && !shadow && !r[0x3692]) SceneObj_DrawMarker(o);
        }
    }
    int32_t n = F<int32_t>(r, 0x4a9);
    if (n) {
        int32_t best = -1;
        float d = F<float>(r, 0xad);
        for (int32_t i = 0; i < n; ++i)
            if (!(d < F<float>(r, 0xad + i * 8))) {
                d = F<float>(r, 0xad + i * 8);
                best = i;
            }
        if (best >= 0) F<int32_t>(r, 0x366e) = F<int32_t>(r, 0xa9 + best * 8);
    }
    if (started) {
        Render_DrawLitGroups(F<uint8_t*>(r, 0x2d), 0, F<PtrList*>(r, 0x7d), 0);
        Render_WorldIdentity(F<uint8_t*>(r, 0x2d));
    }
}
RT3_REPLACE(0x5294b0, Scene_QueueWorld);

// 0x529840. Draws the ground quads (four corners of 3 floats each) listed
// in terrain t's cells (Terrain_Cell167d) for view s's visible cells
// (+0x22b53 of them at +0x374f, x/y pairs every 0x10 bytes): unlit,
// uncull, blended and alpha tested with texture +0x3661, grey by the fog
// brightness (fog block float x 0x5ca0ec), as two triangles each, flushed
// whenever the shared buffer +0x71 nears its end.
RT3_NOINLINE void __fastcall Scene_DrawCellQuads(uint8_t* s, int, uint8_t* unused, uint8_t* t) {
    uint32_t cells = F<uint32_t>(s, 0x22b53);
    const int32_t* cell = (const int32_t*)(s + 0x374f);
    uint8_t started = 0;
    uint8_t fog[0x34];
    Render_GetFogBlock(F<uint8_t*>(s, 0x2d), 0, fog);
    int32_t g = Ftol((double)*(float*)fog * Fl(0x5ca0ec));
    int32_t a = g > 0xff ? 0xff : g, b = g > 0xff ? 0xff : g, c = g > 0xff ? 0xff : g;
    uint32_t col = (((uint32_t)a | 0xffffff00) << 8 | (uint32_t)b) << 8 | (uint32_t)c;
    if (!cells) return;
    float* vb = (float*)t;
    do {
        PtrList* list = Terrain_Cell167d(t, 0, cell[0], cell[1]);
        cell += 4;
        PtrList_Rewind(list);
        int32_t n = F<int32_t>((uint8_t*)list, 0xc);
        const float* q = (const float*)PtrList_Next(list);
        if (!started && n) {
            started = 1;
            Render_BindVb2647(F<uint8_t*>(s, 0x2d));
            vb = CellQuads_Relock(s);
            Render_CullNone(F<uint8_t*>(s, 0x2d));
            Render_LightingOff(F<uint8_t*>(s, 0x2d));
            Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
            Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, F<uint8_t*>(s, 0x3661));
            Render_BlendDefault(F<uint8_t*>(s, 0x2d));
            Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 1);
            Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 0, -1);
        }
        for (; n > 0; --n) {
            static const int corner[6] = { 0, 1, 2, 0, 2, 3 };
            static const uint32_t us[6] = { 0, 0x3f800000, 0x3f800000, 0, 0x3f800000, 0 };
            static const uint32_t vs[6] = { 0, 0, 0x3f800000, 0, 0x3f800000, 0x3f800000 };
            uint32_t* o = (uint32_t*)vb;
            for (int k = 0; k < 6; ++k, o += 6) {
                const uint32_t* p = (const uint32_t*)q + corner[k] * 3;
                o[0] = p[0];
                o[1] = p[1];
                o[2] = p[2];
                o[3] = col;
                o[4] = us[k];
                o[5] = vs[k];
            }
            vb = (float*)o;
            F<int32_t>(s, 0x71) += 6;
            if (F<int32_t>(s, 0x71) > 0xf9e8) {
                Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
                if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
                vb = CellQuads_Relock(s);
            }
            q = (const float*)PtrList_Next(list);
        }
    } while (--cells);
    if (!started) return;
    Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
    if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    Render_CullCcw(F<uint8_t*>(s, 0x2d));
    Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 1, -1);
}
RT3_REPLACE(0x529840, Scene_DrawCellQuads);

// 0x529b50. Draws the boxes listed in terrain t's cells (Terrain_Cell1675)
// for view s's visible cells within t's range +0x1538 (distance table
// 0x6d9098 from the camera cell +0x3682/+0x3686; 0 everywhere in the
// 0x114272 mode, where the camera c must also be low enough, +0x20): unlit,
// uncull with texture 0xccba20, faded by distance, lit by fog x light, and
// with their labels unless `reflected` (then unblended, seen from c's
// mirror point and without tops). Needs the scene option +0x29 +0x63.
RT3_NOINLINE void __fastcall Scene_DrawCellBoxes(uint8_t* s, int, uint8_t* c, uint8_t* t, uint8_t reflected) {
    if (!F<uint8_t*>(s, 0x29)[0x63]) return;
    uint8_t* e = g_engine;
    if (F<int32_t>(e, 0x11426a)) return;
    if (F<int32_t>(e, 0x114272) && F<float>(c, 0x20) > Fl(0x5dd198)) return;
    uint32_t cells = F<uint32_t>(s, 0x22b53);
    int32_t range = F<int32_t>(t, 0x1538);
    uint32_t cam[3] = { F<uint32_t>(s, 0x369c), F<uint32_t>(s, 0x36a0), F<uint32_t>(s, 0x36a4) };
    int32_t count = 0;
    const int32_t* cell = (const int32_t*)(s + 0x374f);
    uint8_t started = 0;
    if (reflected) Obj_GetXYZ1c(c, 0, &cam[0], &cam[1], &cam[2]);
    uint8_t fog[0x34];
    Render_GetFogBlock(F<uint8_t*>(s, 0x2d), 0, fog);
    uint32_t light = 0;
    Render_GetPair25f7(F<uint8_t*>(s, 0x2d), 0, 0, &light, nullptr);
    int32_t g = Ftol((double)*(float*)&light * *(float*)fog * Fl(0x5ca0ec));
    int32_t a = g > 0xff ? 0xff : g, b = g > 0xff ? 0xff : g, cc = g > 0xff ? 0xff : g;
    uint32_t col = ((uint32_t)a << 8 | (uint32_t)b) << 8 | (uint32_t)cc;
    uint8_t* vb = (uint8_t*)(uintptr_t)reflected;
    for (; cells; --cells, cell += 4) {
        int32_t cx = cell[0], cy = cell[1];
        int32_t dx = F<int32_t>(s, 0x3682) - cx * 16 - 8;
        int32_t dy = F<int32_t>(s, 0x3686) - cy * 16 - 8;
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        int32_t dist = hook::Global<uint16_t>(0x6d9098 + (dx * 0x418 + dy) * 2);
        if (F<int32_t>(g_engine, 0x114272)) dist = 0;
        if (dist > range) continue;
        PtrList* list = Terrain_Cell1675(t, 0, cx, cy);
        PtrList_Rewind(list);
        int32_t n = F<int32_t>((uint8_t*)list, 0xc);
        uint8_t* o = (uint8_t*)PtrList_Next(list);
        if (!started) {
            if (!n) continue;
            started = 1;
            Render_BindVb2647(F<uint8_t*>(s, 0x2d));
            vb = (uint8_t*)CellQuads_Relock(s);
            Render_CullNone(F<uint8_t*>(s, 0x2d));
            Render_LightingOff(F<uint8_t*>(s, 0x2d));
            Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
            Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, (uint8_t*)(uintptr_t)Get_ccba20());
            if (!reflected) {
                Render_BlendDefault(F<uint8_t*>(s, 0x2d));
                Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 0x3f);
            } else {
                Render_BlendOff(F<uint8_t*>(s, 0x2d));
                Render_AlphaTestOff(F<uint8_t*>(s, 0x2d));
            }
            Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 0, -1);
            Render_AddressMirror(F<uint8_t*>(s, 0x2d));
        }
        for (; n > 0; --n, o = (uint8_t*)PtrList_Next(list)) {
            float pos[3];
            Effect_GetVectors(o, 0, (uint32_t*)pos, nullptr, nullptr, nullptr);
            double fx = (double)pos[0] - *(float*)&cam[0];
            double fz = (double)pos[2] - *(float*)&cam[2];
            int32_t ix = Ftol((fx < 0 ? -fx : fx) * D(0x5dd190));
            int32_t iz = Ftol((fz < 0 ? -fz : fz) * D(0x5dd190));
            int32_t d = *(const uint16_t*)(uintptr_t)(0x6d9098 - (uint32_t)((ix * 0x418 + iz) << 1));
            if (F<int32_t>(g_engine, 0x114272)) d = 0;
            if (d >= range) continue;
            if ((*(int32_t(__fastcall**)(uint8_t*))(*(uint8_t**)o + 0xc))(o)) continue;
            Obj_SetAlpha5b(o, 0, (uint8_t)Ftol((double)d / range * Fl(0x5ca0ec)));
            count = 0;
            Box_EmitVertices(o, 0, vb, &count, col, reflected);
            F<int32_t>(s, 0x71) += count;
            vb += count * 0x18;
            if (F<int32_t>(s, 0x71) > 0xf9e8) {
                Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
                if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
                vb = (uint8_t*)CellQuads_Relock(s);
            }
            if (!reflected && !s[0x3692]) SceneLabel_Draw(o);
        }
    }
    if (!started) return;
    Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
    if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    Render_CullCcw(F<uint8_t*>(s, 0x2d));
    Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 1, -1);
    Render_AddressWrap(F<uint8_t*>(s, 0x2d));
}
RT3_REPLACE(0x529b50, Scene_DrawCellBoxes);

// 0x529fd0. Draws the flat (+0x1c list) or, for `raised`, lifted by 0.25
// (+0x20 list) quad layer of terrain t's cells for view s's visible cells
// (scene option +0x29 +0x5f / +0x60): unlit, uncull, clamped, blended and
// without z writes, grey by the fog brightness, with t's current texture
// (+0x1574, raised: Render_Get1578, then also filtered by +0x3693).
RT3_NOINLINE void __fastcall Scene_DrawCellLayer(uint8_t* s, int, uint8_t* unused, uint8_t* t, uint8_t raised) {
    if (!F<uint8_t*>(s, 0x29)[raised ? 0x60 : 0x5f]) return;
    uint32_t cells = F<uint32_t>(s, 0x22b53);
    int32_t count = 0;
    const int32_t* cell = (const int32_t*)(s + 0x374f);
    uint8_t started = 0;
    uint8_t fog[0x34];
    Render_GetFogBlock(F<uint8_t*>(s, 0x2d), 0, fog);
    int32_t g = Ftol((double)*(float*)fog * Fl(0x5ca0ec));
    int32_t m = g > 0xff ? 0xff : g;
    uint32_t col = ((uint32_t)m << 8 | (uint32_t)m) << 8 | (uint32_t)m;
    if (!cells) return;
    uint8_t* vb = (uint8_t*)(uintptr_t)raised;
    do {
        uint8_t* c = (uint8_t*)Terrain_Cell1681(t, 0, cell[0], cell[1]);
        cell += 4;
        int32_t n = F<int32_t>(F<uint8_t*>(c, raised ? 0x20 : 0x1c), 0xc);
        if (!started) {
            if (!n) continue;
            Render_BindVb2647(F<uint8_t*>(s, 0x2d));
            vb = (uint8_t*)CellQuads_Relock(s);
            started = 1;
            Render_LightingOff(F<uint8_t*>(s, 0x2d));
            Render_CullNone(F<uint8_t*>(s, 0x2d));
            Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
            Render_AddressClamp(F<uint8_t*>(s, 0x2d), 0, 0);
            if (!raised) {
                Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, F<uint8_t*>(t, F<int32_t>(t, 0x1574) * 4 + 8));
            } else {
                uint32_t i = Render_Get1578(t);
                Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, F<uint8_t*>(t, i * 4 + 8));
                Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, s[0x3693], -1);
            }
            Render_BlendDefault(F<uint8_t*>(s, 0x2d));
            Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 1);
            Render_ZWriteOff(F<uint8_t*>(s, 0x2d));
        }
        if (!n) continue;
        count = 0;
        if (!raised) Quads_EmitFlat(c, 0, vb, &count, col);
        else Quads_EmitRaised(c, 0, vb, &count, col, 0.25f);
        F<int32_t>(s, 0x71) += count;
        vb += count * 0x18;
        if (F<int32_t>(s, 0x71) > 0xf9e8) {
            Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
            if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
            vb = (uint8_t*)CellQuads_Relock(s);
        }
    } while (--cells);
    if (!started) return;
    Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
    if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
    Render_CullCcw(F<uint8_t*>(s, 0x2d));
    Render_ZWriteOn(F<uint8_t*>(s, 0x2d));
    Render_AddressWrap(F<uint8_t*>(s, 0x2d));
    Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 1, -1);
}
RT3_REPLACE(0x529fd0, Scene_DrawCellLayer);

// 0x52a2f0. Draws terrain t's scenery cells (Terrain_Cell1685, those of
// view s's visible cells within t's range +0x1540) seen from camera cam,
// with t's current material (+0x168d, alpha byte +5 set to 0xbf): fades
// by the fog factor (alpha +0x36b4, nothing below +0x22b8b; brightness
// boost up to 0.25), unlit, uncull, alpha tested at 2 and clamped. Needs
// the scene option +0x29 +0x5d and t's scenery (+0x1691).
RT3_NOINLINE void __fastcall Scene_DrawScenery(uint8_t* s, int, uint8_t* cam, uint8_t* t) {
    if (!F<uint8_t*>(s, 0x29)[0x5d]) return;
    uint8_t* e = g_engine;
    if (F<int32_t>(e, 0x11425e) || F<int32_t>(e, 0x114276)) return;
    if (!F<int32_t>(t, 0x1691)) return;
    g_cc3b3c = 0;
    int32_t range = (int32_t)Render_Get1540(t);
    double f = Scene_FogFactor(s, 0, t);
    int32_t alpha = Ftol(Fl(0x5ca0ec) * f);
    F<int32_t>(s, 0x36b4) = alpha;
    double y = (D(0x5c8770) - f) * D(0x5c8628);
    float boost = (float)y;
    if (y < D(0x5c8608)) boost = 0.0f;
    else if (boost > D(0x5c8758)) boost = 0.25f;
    if (alpha > 0xff) F<int32_t>(s, 0x36b4) = 0xff;
    else if (alpha < F<int32_t>(s, 0x22b8b)) return;
    uint8_t* mat = F<uint8_t*>(t, F<int32_t>(t, 0x168d) * 4 + 8);
    uint32_t cells = F<uint32_t>(s, 0x22b53);
    mat[5] = 0xbf;
    uint8_t started = 0;
    const int32_t* cell = (const int32_t*)(s + 0x374f);
    uint8_t* vb = nullptr;
    uint8_t fog[0x34];
    Render_GetFogBlock(F<uint8_t*>(s, 0x2d), 0, fog);
    int32_t g = Ftol((boost + D(0x5c8da8)) * ((double)*(float*)fog * Fl(0x5ca0ec)));
    int32_t m = g > 0xff ? 0xff : g;
    uint32_t col = ((uint32_t)m << 8 | (uint32_t)m) << 8 | (uint32_t)m;
    if (!cells) return;
    do {
        int32_t cx = cell[0], cy = cell[1];
        cell += 4;
        int32_t dx = F<int32_t>(s, 0x3682) - cx * 16 - 8;
        int32_t dy = F<int32_t>(s, 0x3686) - cy * 16 - 8;
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        if (hook::Global<uint16_t>(0x6d9098 + (dx * 0x418 + dy) * 2) > range) continue;
        uint8_t* c = (uint8_t*)Terrain_Cell1685(t, 0, cx, cy);
        if (!c) continue;
        if (!started) {
            started = 1;
            Render_BindVb2647(F<uint8_t*>(s, 0x2d));
            Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
            Render_LightingOff(F<uint8_t*>(s, 0x2d));
            Render_CullNone(F<uint8_t*>(s, 0x2d));
            Render_BlendDefault(F<uint8_t*>(s, 0x2d));
            Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 2);
            Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 0, -1);
            Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x45));
            Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, mat);
            vb = (uint8_t*)Scene_BeginBatch(s, 0, 0x2000);
            Render_AddressClamp(F<uint8_t*>(s, 0x2d), 0, 0);
        }
        Scene_EmitScenery(s, 0, t, cam, c, &vb, col);
    } while (--cells);
    if (!started) return;
    Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
    if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
    Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, 1, -1);
    Render_PopStages(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_AddressWrap(F<uint8_t*>(s, 0x2d));
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    Render_CullNone(F<uint8_t*>(s, 0x2d));
}
RT3_REPLACE(0x52a2f0, Scene_DrawScenery);

// 0x52a5d0. Draws view s's particle emitters seen from camera cam: the lit
// ones (+0x85; points sorted by distance, emitters by +0x1d9 descending,
// tinted by the fog light, capped at 0.75, unless +0xac 0x400 / 0x2000) and
// then the others (+0x81), both lists emptied; unlit, blended, alpha
// tested, clamped, without z writes, texture 0xd9399c. Option +0x29 +0x5e.
RT3_NOINLINE void __fastcall Scene_DrawEmitters(uint8_t* s, int, uint8_t* cam) {
    if (!F<uint8_t*>(s, 0x29)[0x5e] || F<int32_t>(g_engine, 0x114262)) {
        PtrList_Clear(F<PtrList*>(s, 0x81));
        PtrList_Clear(F<PtrList*>(s, 0x85));
        return;
    }
    uint8_t** sorted = nullptr;
    int32_t nSorted = 0;
    PtrList_Rewind(F<PtrList*>(s, 0x81));
    PtrList_Rewind(F<PtrList*>(s, 0x85));
    if (F<int32_t>(F<uint8_t*>(s, 0x85), 0xc) <= 0 && F<int32_t>(F<uint8_t*>(s, 0x81), 0xc) <= 0) return;
    Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
    Render_SetDefaultMaterial(F<uint8_t*>(s, 0x2d), 0, 0);
    uint8_t fog[0x34];
    Render_GetFogBlock(F<uint8_t*>(s, 0x2d), 0, fog);
    double v = *(float*)fog * D(0x5c85a0);
    float lr = (float)v, lg = lr, lb = lr;
    if (lr > D(0x5c8750)) lr = 0.75f;
    if (lg > D(0x5c8750)) lg = 0.75f;
    if (v > D(0x5c8750)) lb = 0.75f;
    Ambience_Visit(g_models, 0, *(uint32_t*)&lr, *(uint32_t*)&lg, *(uint32_t*)&lb);
    Render_LightingOff(F<uint8_t*>(s, 0x2d));
    Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x45));
    Render_BlendDefault(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_ZWriteOff(F<uint8_t*>(s, 0x2d));
    Render_AddressClamp(F<uint8_t*>(s, 0x2d), 0, 0);
    Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
    Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, (uint8_t*)(uintptr_t)Get_d9399c());
    Render_CullNone(F<uint8_t*>(s, 0x2d));
    Particles_FrameSetup(cam);
    uint8_t* item = cam;
    if (F<int32_t>(F<uint8_t*>(s, 0x85), 0xc)) item = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x85));
    else if (F<int32_t>(F<uint8_t*>(s, 0x81), 0xc)) item = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x81));
    Render_BindVb2647(F<uint8_t*>(s, 0x2d));
    uint8_t* vb = (uint8_t*)Scene_BeginBatch(s, 0, 0x2000);
    float cx = 0, cy = 0, cz = 0;
    if (int32_t n85 = F<int32_t>(F<uint8_t*>(s, 0x85), 0xc)) {
        sorted = ((uint8_t**)crt::malloc((uint32_t)n85 * 4));
        Obj_GetXYZ1c(cam, 0, (uint32_t*)&cx, (uint32_t*)&cy, (uint32_t*)&cz);
        for (; item; item = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x85))) {
            Emitter_SortPoints(item, cx, cy, cz);
            sorted[nSorted++] = item;
        }
        for (int32_t i = 0; i < nSorted; ++i)
            for (int32_t j = i + 1; j < nSorted; ++j)
                if (F<int32_t>(sorted[i], 0x1d9) < F<int32_t>(sorted[j], 0x1d9)) {
                    uint8_t* t = sorted[i];
                    sorted[i] = sorted[j];
                    sorted[j] = t;
                }
        PtrList_Clear(F<PtrList*>(s, 0x85));
        for (int32_t k = 0; k < nSorted; ++k) {
            uint8_t* e = sorted[k];
            vb = Emitters_MakeRoom(s, e, vb);
            int32_t count = 0;
            uint32_t fl = F<uint32_t>(e, 0xac);
            if (!(fl & 0x400)) {
                if (!(fl & 0x2000)) {
                    F<float>(e, 0x3c) = lr;
                    F<float>(e, 0x40) = lg;
                    F<float>(e, 0x44) = lb;
                }
                Emitter_RetintAll(e);
            }
            Emitter_Draw(e, 0, vb, &count);
            F<int32_t>(s, 0x71) += count;
            vb += count * 0x18;
        }
    }
    PtrList_Rewind(F<PtrList*>(s, 0x81));
    for (item = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x81)); item; item = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x81))) {
        vb = Emitters_MakeRoom(s, item, vb);
        Emitter_SortPoints(item, cx, cy, cz);
        vb = Emitters_MakeRoom(s, item, vb);
        int32_t count = 0;
        Emitter_Draw(item, 0, vb, &count);
        vb += count * 0x18;
        F<int32_t>(s, 0x71) += count;
    }
    PtrList_Clear(F<PtrList*>(s, 0x81));
    Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
    if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
    Render_ZWriteOn(F<uint8_t*>(s, 0x2d));
    Render_AddressWrap(F<uint8_t*>(s, 0x2d));
    Render_SetDefaultMaterial(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_PopStages(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    if (sorted) crt::free(sorted);
}
RT3_REPLACE(0x52a5d0, Scene_DrawEmitters);

// 0x52acd0. Draws view s's ground detail layers (four, texture stages via
// render state 0x2f) on the 32 x 32 vertex patch of terrain t ahead of
// camera cam: the patch follows the look direction (offset by its
// horizontal part, pulled in when looking down) and is only rebuilt for
// layers marked visible (+0x22b8f) unless the camera moved (+0x22b9f..);
// vertex alpha is the fade level (0x5dd1a8 - +0x368e) less the distance
// (table 0x6d9098) and the layer's coverage, grey by fog, triangles only
// where some corner shows.
RT3_NOINLINE void __fastcall Scene_DrawDetailLayers(uint8_t* s, int, uint8_t* t, uint8_t* cam) {
    int32_t level = Ftol(Fl(0x5dd1a8) - (double)F<float>(s, 0x368e) * Fl(0x5cb4a8));
    if (level < 0) return;
    float dx, y, dz;
    Obj_GetXYZ4(cam, 0, (uint32_t*)&dx, (uint32_t*)&y, (uint32_t*)&dz);
    dx = (float)(dx - (double)F<float>(s, 0x369c));
    dz = (float)(dz - (double)F<float>(s, 0x36a4));
    double moved = fabs((double)F<float>(s, 0x22b9f) - dx);
    moved += fabs((double)F<float>(s, 0x22ba3) - y);
    moved += fabs((double)F<float>(s, 0x22ba7) - dz);
    moved += fabs((double)F<float>(s, 0x22bb3) - F<float>(s, 0x36a4));
    moved += fabs((double)F<float>(s, 0x22baf) - F<float>(s, 0x36a0));
    moved += fabs((double)F<float>(s, 0x22bab) - F<float>(s, 0x369c));
    int32_t* flags = (int32_t*)(s + 0x22b8f);
    int32_t cached;
    if (moved > D(0x5c8938)) {
        memset(flags, 0, 16);
        F<float>(s, 0x22b9f) = dx;
        F<float>(s, 0x22ba3) = y;
        F<float>(s, 0x22ba7) = dz;
        F<uint32_t>(s, 0x22bab) = F<uint32_t>(s, 0x369c);
        F<uint32_t>(s, 0x22baf) = F<uint32_t>(s, 0x36a0);
        F<uint32_t>(s, 0x22bb3) = F<uint32_t>(s, 0x36a4);
        cached = 0;
    } else {
        cached = 1;
        bool any = false;
        for (int k = 0; k < 4; ++k)
            if (flags[k]) any = true;
        if (!any) return;
    }
    uint8_t* r = F<uint8_t*>(s, 0x2d);
    Render_BindPair2647(r);
    Render_WorldIdentity(F<uint8_t*>(s, 0x2d));
    Render_SetDefaultMaterial(F<uint8_t*>(s, 0x2d), 0, 0);
    Render_LightingOff(F<uint8_t*>(s, 0x2d));
    Render_ZWriteOff(F<uint8_t*>(s, 0x2d));
    Render_BlendDefault(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_CullCw(F<uint8_t*>(s, 0x2d));
    Render_AddressWrap(F<uint8_t*>(s, 0x2d));
    int32_t w = Terrain_Rows(t) + 1;
    int32_t h = Terrain_Cols(t);
    Terrain_Get15e5(t);
    const float* heights = *(const float**)Terrain_HeightsSlot(t);
    // look direction: normalised (dx, dy, dz) when it has length
    double ddy = (double)y - F<float>(s, 0x36a0);
    double ndz = dz, ndy = ddy;
    float ndx = dx;
    double len = Fsqrt(ddy * ddy + (double)dz * dz + (double)dx * dx);
    if (Fl(0x5c8598) < len) {
        double inv = Fl(0x5c85b4) / len;
        ndx = (float)(dx * inv);
        ndy = ddy * inv;
        ndz = (double)dz * inv;
    }
    double hlen = Fsqrt((double)dx * dx + (double)dz * dz);
    double k = hlen > D(0x5c8678) ? D(0x5c8770) / hlen : hlen;
    double tilt = (ndz + ndx) * Fl(0x5c8598) - ndy * Fl(0x5c85b4);
    double f = k * ((D(0x5c8770) - fabs(tilt)) * D(0x5dd1a0));
    dx = (float)(dx * f);
    dz = (float)(f * dz);
    int32_t cx = Ftol((double)F<int32_t>(s, 0x3682) + dx);
    int32_t cz = Ftol((double)F<int32_t>(s, 0x3686) + dz);
    int32_t x0 = cx - 16, x1 = cx + 16, z0 = cz - 16, z1 = cz + 16;
    uint8_t fog[0x34];
    Render_GetFogBlock(F<uint8_t*>(s, 0x2d), 0, fog);
    int32_t g = Ftol((double)*(float*)fog * Fl(0x5ca0ec) * D(0x5c8698));
    int32_t m = g > 0xf0 ? 0xf0 : g < 0x10 ? 0x10 : g;
    uint32_t col = ((uint32_t)m << 8 | (uint32_t)m) << 8 | (uint32_t)m;
    bool inside = x0 >= 0 && x1 < w && z0 >= 0 && z1 < h;
    for (int32_t i = 0; i < 4; ++i) {
        if (cached && !flags[i]) continue;
        uint8_t mask[0x400];
        memset(mask, 0, sizeof mask);
        float* v = CellQuads_Relock(s);
        const uint8_t* layer = (const uint8_t*)(uintptr_t)Terrain_Layer(t, 0, (uint8_t)i);
        double scaleX = F<float>(t, 0x1641 + (uint8_t)i * 4) * D(0x5dcfd8);
        float scale = (float)scaleX;
        float step = (float)(scaleX * Fl(0x5c8700));
        float fx0 = (float)(x0 * 0x5c);
        double zf = z0 * 0x5c;
        double vz = scale * zf;
        if (z0 < z1) {
            double u0 = (double)fx0 * scale;
            const uint8_t* lrow = layer + z0 * w;
            const float* hrow = heights + z0 * w + x0;
            uint8_t* mp = mask;
            for (int32_t z = z0; z < z1; ++z, hrow += w, lrow += w) {
                int32_t adz = (F<int32_t>(s, 0x3686) - z) * 7;
                if (adz < 0) adz = -adz;
                double xw = fx0, u = u0;
                const float* hp = hrow;
                for (int32_t x = x0; x < x1; ++x, ++mp, ++hp) {
                    int32_t adx = (F<int32_t>(s, 0x3682) - x) * 7;
                    if (adx < 0) adx = -adx;
                    int32_t a = level - hook::Global<uint16_t>(0x6d9098 + (adx * 0x418 + adz) * 2);
                    if (a > 0xa4) a = 0xa4;
                    float hgt;
                    if (inside || (x > 0 && x < w - 2 && z > 0 && z < h - 1)) {
                        hgt = *hp;
                        a -= lrow[x];
                        if (a <= 0xc) a = 0;
                        else {
                            *mp = 1;
                            if (!cached) flags[i] = 1;
                        }
                    } else {
                        int32_t xc = x < 0 ? 0 : x > w - 2 ? w - 1 : x;
                        int32_t zc = z < 0 ? 0 : z > h - 1 ? h - 1 : z;
                        hgt = heights[zc * w + xc];
                        a = 0;
                    }
                    v[0] = (float)xw;
                    v[1] = hgt;
                    v[2] = (float)zf;
                    *(uint32_t*)&v[3] = (uint32_t)a << 24 | col;
                    v[4] = (float)u;
                    v[5] = (float)vz;
                    v += 6;
                    ++F<int32_t>(s, 0x71);
                    u += step;
                    xw += Fl(0x5c8700);
                }
                vz += step;
                zf += Fl(0x5c8700);
            }
        }
        Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
        int32_t count = 0;
        uint16_t* ix = (uint16_t*)Render_LockVb264b(F<uint8_t*>(s, 0x2d));
        for (int32_t row = 0; row < 0x3e0; row += 0x20) {
            int32_t base = F<int32_t>(s, 0x75) + row;
            for (int32_t c = 0; c < 0x1f; ++c) {
                if (!mask[row + c] && !mask[row + 0x20 + c] && !mask[row + 1 + c] && !mask[row + 0x21 + c]) continue;
                uint16_t a0 = (uint16_t)(c + base);
                *ix++ = a0;
                *ix++ = (uint16_t)(c + base + 1);
                *ix++ = (uint16_t)(c + base + 0x21);
                *ix++ = a0;
                *ix++ = (uint16_t)(c + base + 0x21);
                *ix++ = (uint16_t)(c + base + 0x20);
                count += 6;
            }
        }
        Render_UnlockVb264b(F<uint8_t*>(s, 0x2d));
        uint32_t li = Render_Get1554(t);
        EngineDevRenderState(0x2f, (uint32_t)i + 1);
        Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, F<uint8_t*>(t, (li + (uint32_t)i) * 4 + 0xc));
        Render_DrawIndexed(F<uint8_t*>(s, 0x2d), 0, (uint16_t)count, (uint32_t)F<int32_t>(s, 0x75), (uint32_t)(F<int32_t>(s, 0x71) - F<int32_t>(s, 0x75)));
    }
    Render_ZWriteOn(F<uint8_t*>(s, 0x2d));
    Render_SetDefaultMaterial(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
    EngineDevRenderState(0x2f, 0);
}
RT3_REPLACE(0x52acd0, Scene_DrawDetailLayers);

// 0x52b4a0. Draws the 28 x 28 vertex patch of terrain t around the
// renderer's focus point (+0x3701 / +0x3709 scaled by 0x5dcfe8) as view s's
// fog layer when there is fog (+5 > 0 or a pending rebuild +0x3748) of
// density 8 or more: rebuilds the vertices when the renderer asks
// (Render_TakeFlag36f7, +0x3749, +0x3748; alpha = density - distance x 8
// inside the map, projected UVs) and the shared index list once (+0x374b),
// then draws it darkening with t's texture +0x1558, or for `shadow` plain
// and untextured.
RT3_NOINLINE void __fastcall Scene_DrawFogPatch(uint8_t* s, int, uint8_t* t, uint8_t shadow) {
    if (F<int32_t>(s, 5) <= 0 && !s[0x3748]) return;
    int32_t level = Scene_FogDensity(s);
    if (level < 8) return;
    int32_t w = Terrain_Rows(t) + 1;
    int32_t h = Terrain_Cols(t);
    const float* xs = (const float*)(uintptr_t)Terrain_Get15e5(t);
    const float* hgt = *(const float**)Terrain_HeightsSlot(t);
    if (Render_TakeFlag36f7(F<uint8_t*>(s, 0x2d), 0, 0) || s[0x3749] || s[0x3748]) {
        s[0x3748] = 0;
        float* v = (float*)Render_LockVb264f(F<uint8_t*>(s, 0x2d));
        uint8_t* r = F<uint8_t*>(s, 0x2d);
        float u = 0, vv = 0;
        int32_t cx = Ftol((double)F<float>(r, 0x3701) * Fl(0x5dcfe8));
        g_cc3b54 = 0;
        float fz0 = F<float>(r, 0x3709);
        int32_t cz = Ftol((double)fz0 * Fl(0x5dcfe8));
        int32_t midX = (F<int32_t>(s, 0x3682) + cx) >> 1;
        int32_t midZ = (F<int32_t>(s, 0x3686) + cz) >> 1;
        for (int32_t z = cz - 14; z < cz + 14; ++z) {
            int32_t adz = z - midZ;
            if (adz < 0) adz = -adz;
            float fz = Terrain_Coord(xs, z);
            for (int32_t x = cx - 14; x < cx + 14; ++x) {
                int32_t adx = x - midX;
                if (adx < 0) adx = -adx;
                int32_t d = hook::Global<uint16_t>(0x6d9098 + (adx + adz * 0x418) * 2) << 3;
                float fx = Terrain_Coord(xs, x);
                v[0] = fx;
                float y;
                int32_t alpha;
                if (x > 0 && x < w - 2 && z > 0 && z < h - 1) {
                    y = hgt[z * w + x];
                    alpha = level - d;
                    if (alpha < 0) alpha = 0;
                } else {
                    int32_t xc = x < 0 ? 0 : x >= w ? w - 1 : x;
                    int32_t zc = z < 0 ? 0 : z >= h ? h - 1 : z;
                    y = hgt[zc * w + xc];
                    alpha = 0;
                }
                v[1] = y;
                *(uint32_t*)&v[3] = (uint32_t)alpha << 24 | 0xffffff;
                v[2] = fz;
                Render_ProjectSimple(F<uint8_t*>(s, 0x2d), 0, fx, y, fz, &u, &vv);
                u = (float)(u * D(0x5c8628));
                v += 6;
                ++g_cc3b54;
                vv = (float)(vv * D(0x5ca180));
                v[-2] = (float)(u + D(0x5c8628));
                v[-1] = (float)(vv + D(0x5c8628));
            }
        }
        Render_UnlockVb264f(F<uint8_t*>(s, 0x2d));
    }
    if (!s[0x374b]) {
        g_cc3b50 = 0;
        s[0x374b] = 1;
        uint16_t* ix = (uint16_t*)Render_LockVb2653(F<uint8_t*>(s, 0x2d));
        g_cc3b50 += 0x1116;
        for (int32_t row = 0; row < 0x2f4; row += 0x1c)
            for (int32_t c = 0; c < 0x1b; ++c) {
                uint16_t a = (uint16_t)(row + c);
                *ix++ = a;
                *ix++ = (uint16_t)(a + 1);
                *ix++ = (uint16_t)(a + 0x1d);
                *ix++ = a;
                *ix++ = (uint16_t)(a + 0x1d);
                *ix++ = (uint16_t)(a + 0x1c);
            }
        Render_UnlockVb2653(F<uint8_t*>(s, 0x2d));
    }
    Render_LightingOff(F<uint8_t*>(s, 0x2d));
    if (!shadow) {
        Render_CullCw(F<uint8_t*>(s, 0x2d));
        Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, F<uint8_t*>(t, F<int32_t>(t, 0x1558) * 4 + 8));
        Render_ZWriteOff(F<uint8_t*>(s, 0x2d));
        Render_BlendDefault(F<uint8_t*>(s, 0x2d));
        Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 2);
        Render_BlendDarken(F<uint8_t*>(s, 0x2d));
        Render_AddressBorder(F<uint8_t*>(s, 0x2d));
        Render_BindPair264f(F<uint8_t*>(s, 0x2d));
        EngineDevRenderState(0x2f, 1);
        Render_DrawIndexed(F<uint8_t*>(s, 0x2d), 0, g_cc3b50, 0, g_cc3b54);
        EngineDevRenderState(0x2f, 0);
        Render_AddressWrap(F<uint8_t*>(s, 0x2d));
        Render_ZWriteOn(F<uint8_t*>(s, 0x2d));
        Render_BlendAlpha(F<uint8_t*>(s, 0x2d));
        Render_LightingDefault(F<uint8_t*>(s, 0x2d));
        return;
    }
    Render_CullNone(F<uint8_t*>(s, 0x2d));
    Render_BlendOff(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOff(F<uint8_t*>(s, 0x2d));
    Render_BindPair264f(F<uint8_t*>(s, 0x2d));
    Renderer_SetTexture(F<uint8_t*>(s, 0x2d), 0, 0, nullptr);
    Render_DrawIndexed(F<uint8_t*>(s, 0x2d), 0, g_cc3b50, 0, g_cc3b54);
    Render_CullCcw(F<uint8_t*>(s, 0x2d));
    Render_LightingDefault(F<uint8_t*>(s, 0x2d));
}
RT3_REPLACE(0x52b4a0, Scene_DrawFogPatch);

// 0x52b990. Scene s: renders a frame from camera cam over terrain ter. A
// lost device waits (pumping messages); a reset one rebuilds the renderer
// around the fonts. A full frame (not `second`, and the view needs a
// rebuild, +0x3692) first renders the reflection pass into the target and
// then calls itself with `second`. The pass sets up the camera (position
// +0x369c, direction +0x3676, height over ground +0x368e, cell +0x3682 /
// +0x3686), lights and fog, renders the reflected / water view (when
// shown, +0x65 / +0x66, every +0x22b6b frames or on demand, with the near
// distance clamped by the terrain size) and the scenery from the sun, then
// the frame: sky, time of day, the terrain chunks (fixed / paired / shaded
// by engine options, plus the overlay +0x159b and the detail layers), the
// cell triangles, the objects (Scene_QueueWorld), cells, boxes, scenery,
// emitters, rain, particles, markers, lens flare, cursor, stats and the
// overlay; frame counter +5.
RT3_NOINLINE void __fastcall Scene_RenderFrame(uint8_t* s, int, uint8_t* cam, uint8_t* ter, uint8_t second) {
    uint8_t* r = F<uint8_t*>(s, 0x2d);
    uint8_t* eng = g_engine;
    uint8_t st = Render_DeviceState(F<uint8_t*>(s, 0x2d));
    if (st == 1) {
        while (Render_DeviceState(F<uint8_t*>(s, 0x2d)) == 1) PumpMessages();
        return;
    }
    if (st == 2) {
        if (g_pack && g_6d4034) Pack_ReleaseFontHandles(g_pack);
        Display_DestroyRenderer(s);
        Scene_CreateRenderer(s);
        if (g_pack && g_6d4034) Pack_RecreateFonts(g_pack);
    }
    g_engine[0x11369c] = 1;
    Fade_StartFast();
    if (F<int32_t>(s, 1) != 1) return;
    eng = g_engine;
    int32_t frame = (int32_t)Render_Get1554(ter);
    Scene_SetupVisitGrid(s, 0, ter);
    uint8_t full = second == 0;
    if (full) {
        double a = (double)F<float>(s, 0x22b83) * D(0x5c8888);
        F<float>(s, 0x22b83) = (float)a;
        F<float>(s, 0x22b87) = (float)((double)F<float>(s, 0x22b87) * D(0x5c8888));
        if (a < D(0x5c9ba0)) F<int32_t>(s, 0x22b83) = 0;
    }
    if (!second && full) full &= Scene_ViewNeedsRebuild(s, 0, cam);
    s[0x3692] = full;
    if (full) F<float>(s, 0x22b83) = (float)((double)F<float>(s, 0x22b83) + Fl(0x5c85b4));
    else F<float>(s, 0x22b87) = (float)((double)F<float>(s, 0x22b87) + Fl(0x5c85b4));
    Obj_GetXYZ4(cam, 0, &F<uint32_t>(s, 0x369c), &F<uint32_t>(s, 0x36a0), &F<uint32_t>(s, 0x36a4));
    F<uint32_t>(s, 0x3676) = F<uint32_t>(s, 0x369c);
    F<uint32_t>(s, 0x367a) = F<uint32_t>(s, 0x36a0);
    F<uint32_t>(s, 0x367e) = F<uint32_t>(s, 0x36a4);
    Obj_GetXYZ1c(cam, 0, &F<uint32_t>(s, 0x369c), &F<uint32_t>(s, 0x36a0), &F<uint32_t>(s, 0x36a4));
    F<float>(s, 0x3676) = (float)((double)F<float>(s, 0x369c) - F<float>(s, 0x3676));
    F<float>(s, 0x367a) = (float)((double)F<float>(s, 0x36a0) - F<float>(s, 0x367a));
    F<float>(s, 0x367e) = (float)((double)F<float>(s, 0x36a4) - F<float>(s, 0x367e));
    Vec3_Normalize(&F<float>(s, 0x3676));
    Fx_UpdateIntensity(s);
    F<float>(s, 0x368e) = (float)((double)F<float>(s, 0x36a0) - Terrain_HeightAt(ter, 0, F<float>(s, 0x369c), F<float>(s, 0x36a4)));
    F<int32_t>(s, 0x368a) = Ftol((double)F<float>(s, 0x36a0));
    int32_t cx = Ftol((double)F<float>(s, 0x369c) * Fl(0x5dcfe8));
    F<int32_t>(s, 0x3682) = cx < 0 ? -cx : cx;
    int32_t cz = Ftol((double)F<float>(s, 0x36a4) * Fl(0x5dcfe8));
    F<int32_t>(s, 0x3686) = cz < 0 ? -cz : cz;
    if (F<uint8_t*>(s, 0x3672) != cam) s[0x3749] = 1;
    Render_SunDirVec(F<uint8_t*>(s, 0x2d), 0, (float*)0xccb874);
    Render_SetDefaultMaterial(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x41));
    int32_t fogDense = Scene_FogDensity(s) >= 8;
    uint8_t* view = F<uint8_t*>(s, 0x29);
    uint32_t keepNear = F<uint32_t>(view, 0x25);
    auto clampNear = [&](uint32_t c) {
        uint8_t* v = F<uint8_t*>(s, 0x29);
        F<float>(v, 0x25) = (double)F<float>(v, 0x25) > D(c) ? F<float>(v, 0x25) : (float)D(c);
    };
    if (full) {
        clampNear(0x5c8880);
        Render_ApplyFilterStates(F<uint8_t*>(s, 0x2d));
        Render_TargetChanged(F<uint8_t*>(s, 0x2d));
        Texture_BindTarget(F<uint8_t*>(ter, 8), 0, F<void*>(g_engine, 8));
        g_624d54 = 0;
        Renderer_SetupViews(F<uint8_t*>(s, 0x2d), 0, cam, 1);
    } else {
        Renderer_SetupViews(F<uint8_t*>(s, 0x2d), 0, cam, 0);
        Mat16 m;
        memcpy(&m, F<uint8_t*>(s, 0x2d) + 0x346f, sizeof m);
        Sound_SetMatrix(g_sound, 0, m);
        ((void(__fastcall*)(uint8_t*))(*(void***)cam)[5])(cam);
        view = F<uint8_t*>(s, 0x29);
        eng = g_engine;
        bool shown = (view[0x65] || view[0x66]) && eng[0x11422e] && !eng[0x11424f] && F<int32_t>(s, 5) && fogDense;
        if (shown) {
            int32_t c = ++F<int32_t>(s, 0x22b6f);
            shown = c >= F<int32_t>(s, 0x22b6b) || Scene_NeedsRedraw(s);
        }
        if (shown) {
            F<int32_t>(s, 0x22b6f) = 0;
            uint32_t keepNear2 = F<uint32_t>(F<uint8_t*>(s, 0x29), 0x25);
            if (Render_Get1548(ter) >= 0x800) clampNear(0x5c8758);
            else if (Render_Get1548(ter) >= 0x400) clampNear(0x5c88e8);
            else if (Render_Get1548(ter) >= 0x200) clampNear(0x5c8770);
            else clampNear(0x5c8578);
            Render_ApplyFilterStates(F<uint8_t*>(s, 0x2d));
            F<int32_t>(s, 0x22b73) = 0;
            F<int32_t>(s, 0x22b77) = 0;
            int32_t k = F<int32_t>(ter, 0x1558);
            Render_TargetChanged(F<uint8_t*>(s, 0x2d));
            Texture_BindTarget(F<uint8_t*>(ter, 8 + k * 4), 0, F<void*>(g_engine, 8));
            Render_BeginFrame(F<uint8_t*>(s, 0x2d), 0, 0);
            Render_UseView2(F<uint8_t*>(s, 0x2d));
            uint8_t zb = g_engine[0x11421a];
            if (zb) {
                Render_SetZEnable(F<uint8_t*>(s, 0x2d), 0, 1);
                Render_ZWriteOn(F<uint8_t*>(s, 0x2d));
                Render_Clear(F<uint8_t*>(s, 0x2d), 0, 1, 0, 0x3f800000);
                Render_LightingOff(F<uint8_t*>(s, 0x2d));
                Render_ColorWriteOff(F<uint8_t*>(s, 0x2d));
                Render_PushStages(F<uint8_t*>(s, 0x2d), 0, F<const uint32_t*>(s, 0x59));
                Scene_DrawFogPatch(s, 0, ter, 1);
                Render_PopStages(F<uint8_t*>(s, 0x2d), 0, 1);
                Render_ColorWriteOn(F<uint8_t*>(s, 0x2d));
            } else {
                Render_ClearAll(F<uint8_t*>(s, 0x2d));
                Render_SetZEnable(F<uint8_t*>(s, 0x2d), 0, 0);
            }
            if (F<uint8_t*>(s, 0x29)[0x65]) {
                int32_t keepPick = F<int32_t>(s, 0x366e);
                uint32_t keepDist = Render_Get1534(ter);
                Render_Set1534(ter, 0, 0x14);
                F<int32_t>(s, 0x366e) = 1;
                Render_CullNone(F<uint8_t*>(s, 0x2d));
                Render_LightingOff(F<uint8_t*>(s, 0x2d));
                Render_BlendOff(F<uint8_t*>(s, 0x2d));
                ++F<int32_t>(s, 0x22b63);
                Scene_QueueWorld(s, 0, cam, ter, 1);
                --F<int32_t>(s, 0x22b63);
                Scene_DrawCellBoxes(s, 0, cam, ter, 1);
                Render_LightingDefault(F<uint8_t*>(s, 0x2d));
                Render_Set1534(ter, 0, keepDist);
                F<int32_t>(s, 0x366e) = keepPick;
            }
            if (F<uint8_t*>(s, 0x29)[0x5d] && F<uint8_t*>(s, 0x29)[0x66]) {
                uint8_t* c = (uint8_t*)crt::operator_new(0x8a);
                c = c ? Camera_ConstructBase(c) : nullptr;
                float sx, sy, sz;
                Render_SunDir(F<uint8_t*>(s, 0x2d), 0, &sx, &sy, &sz);
                Camera_SetEye(c, 0, 0, 0, 0);
                Camera_SetTarget(c, 0, sx, sy, sz);
                uint32_t e = Render_Get1540(ter);
                int32_t keepFrame = F<int32_t>(s, 5);
                F<uint32_t>(s, 5) = 0xffdc13c6;
                Render_Set1540(ter, 0, (int32_t)e < (int32_t)Render_Get1544(ter) ? e : Render_Get1544(ter));
                Scene_DrawScenery(s, 0, c, ter);
                Render_Set1540(ter, 0, e);
                F<int32_t>(s, 5) = keepFrame;
                if (c) {
                    Camera_Destruct((uint32_t*)c);
                    crt::operator_delete(c);
                }
            }
            if (!zb) Render_SetZEnable(F<uint8_t*>(s, 0x2d), 0, 1);
            F<int32_t>(s, 0x22b73) = F<int32_t>(F<uint8_t*>(s, 0x2d), 0x3711);
            F<int32_t>(s, 0x22b77) = F<int32_t>(F<uint8_t*>(s, 0x2d), 0x370d);
            Render_CullCcw(F<uint8_t*>(s, 0x2d));
            Render_EndFrame(F<uint8_t*>(s, 0x2d), 0, 1);
            F<uint32_t>(F<uint8_t*>(s, 0x29), 0x25) = keepNear2;
            Render_ApplyFilterStates(F<uint8_t*>(s, 0x2d));
        }
        Render_RestoreTarget(F<uint8_t*>(s, 0x2d));
        uint8_t* e = g_engine;
        const int32_t* vp = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
        g_624d54 = 1;
        Render_SetPixelViewport(F<uint8_t*>(s, 0x2d), 0, 0, 0, vp[0], vp[1]);
    }
    eng = g_engine;
    if (!eng[0x114251] || !eng[0x114250]) Render_Clear(F<uint8_t*>(s, 0x2d), 0, 1, 1, 0x3f800000);
    Render_BeginFrame(F<uint8_t*>(s, 0x2d), 0, 1);
    int32_t div = F<int32_t>(s, 0x22b6b) < 1 ? 1 : F<int32_t>(s, 0x22b6b);
    F<int32_t>(F<uint8_t*>(s, 0x2d), 0x3711) += F<int32_t>(s, 0x22b73) / div;
    div = F<int32_t>(s, 0x22b6b) >= 1 ? F<int32_t>(s, 0x22b6b) : 1;
    F<int32_t>(F<uint8_t*>(s, 0x2d), 0x370d) += F<int32_t>(s, 0x22b77) / div;
    Render_SetCameraViewport(F<uint8_t*>(s, 0x2d), 0, cam, full ? 1 : 0);
    if ((double)F<float>(s, 0x21) < D(0x5dd180)) Render_AllowTranslucency(F<uint8_t*>(s, 0x2d));
    else Render_NoTranslucency(F<uint8_t*>(s, 0x2d));
    Engine_Set1146f6(g_engine, 0, F<int32_t>(s, 0xd));
    int32_t t = F<int32_t>(s, 0xd);
    double a = (double)t * D(0x5dd1b0) - D(0x5cb4a0);
    F<float>(s, 0x1d) = (float)a;
    F<float>(s, 0x21) = (float)cos(-a);
    if (t != g_624d50 || !F<int32_t>(s, 5) || Fx_Intensity(s) != g_624d4c) {
        int32_t fx = Fx_Intensity(s);
        Renderer_SetTimeOfDay(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0xd), fx);
        int32_t now = F<int32_t>(s, 0xd), last = g_624d48, d = now;
        if (last > d) d += 0x15180;
        if (d - last > 0x3c) g_624d48 = now;
        g_624d50 = now;
        g_624d4c = Fx_Intensity(s);
    }
    F<uint8_t*>(s, 0x3672) = cam;
    Textures_Manage(0, 0);
    if (!full) {
        float fog = (float)((double(__fastcall*)(uint8_t*))(*(void***)cam)[6])(cam);
        Renderer_SetFog(F<uint8_t*>(s, 0x2d), 0, fog, 1.0f, 1.0f, 1.0f);
        Sky_Render(s, 0, fog);
    } else {
        Sky_Render(s, 0, 2.0f);
        Renderer_SetFog(F<uint8_t*>(s, 0x2d), 0, 2.0f, 0.75f, 0.75f, BitsF(0x3f666666));
    }
    Render_UseView3(F<uint8_t*>(s, 0x2d));
    F<float>(s, 0x36a8) = (float)(Fl(0x5c85b4) - Fl(0x5c8898) / (double)(((int32_t)Render_Get1534(ter) - 8) * 0x5c));
    F<float>(s, 0x36ac) = (float)(Fl(0x5c85b4) - Fl(0x5c8898) / (double)(((int32_t)Render_Get1540(ter) - 8) * 0x5c));
    if (!full || !F<int32_t>(s, 5)) {
        uint8_t changed = (uint8_t)Camera_TakeChanged(cam);
        if (cam[0x65]) s[0x3747] = 1;
        Scene_ComputeFootprint(s);
        if (changed || s[0x3749]) {
            s[0x3749] = 1;
            if (F<uint8_t*>(s, 0x29)[0x62]) {
                F<int32_t>(s, 9) = F<int32_t>(s, 5);
                Camera_SetLock64(cam, 0, 0);
                s[0x3747] = 1;
                Sound_RefreshArea(s, 0, F<int32_t>(s, 0x3682), F<int32_t>(s, 0x3686), F<int32_t>(s, 0x3698), ter);
                int32_t x0, y0, x1, y1;
                Scene_VisibleBounds(s, 0, &x0, &y0, &x1, &y1, ter, 4);
                float v = (float)((double)F<float>(s, 0x36a0) / F<float>(s, 0x22b67));
                if (!(v < Fl(0x5c85b4))) v = BitsF(*(const uint32_t*)rt3::Rdata(0x5c85b4));
                g_8f2518 = (float)((double)v * v);
                g_6d9090 = (float)(Fl(0x5c85b4) - g_8f2518);
                F<int32_t>(s, 0x22b53) = 0;
                TerrainLod_Build(s, 0, 4, ter, x0, y0, x1, y1);
                Scene_SortEntries(s);
                if (F<uint8_t*>(s, 0x29)[0x57]) {
                    if (F<uint8_t*>(s, 0x29)[0x5c]) TerrainSkirt_Build(s, 0, ter);
                    Terrain_BuildMesh(s, 0, 4, ter);
                }
            }
        }
    }
    bool objects = true;
    if (F<uint8_t*>(s, 0x29)[0x57]) {
        Render_SetFiltering(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(g_engine, 0x114212) == 0, -1);
        F<float>(F<uint8_t*>(s, 0x2d), 0x36fd) = (float)(D(0x5c8770) / F<float>(ter, 0x1651) * D(0x5c8c48));
        Render_CullCcw(F<uint8_t*>(s, 0x2d));
        uint8_t* anim = (uint8_t*)(uintptr_t)Render_AnimFrameA(ter);
        hook::As<int32_t>(g_ccd818) = 0;
        g_ccd81c = 0;
        eng = g_engine;
        uint8_t* tex = F<uint8_t*>(ter, 8 + frame * 4);
        auto R = [&]() { return F<uint8_t*>(s, 0x2d); };
        bool pop = true;
        if (!eng[0x11422e]) {
            Render_PushStages(R(), 0, F<const uint32_t*>(s, eng[0x114250] ? 0x35 : 0x45));
            Terrain_DrawChunksFixed(R(), 0, g_cc3b34, F<uint8_t*>(ter, 8 + frame * 4), 1, nullptr, 0);
        } else if (eng[0x11422d]) {
            pop = false;
            if (F<uint8_t*>(s, 0x29)[0x61]) {
                Terrain_DrawChunksShaded(R(), 0, F<uint8_t*>(ter, 8), 2, nullptr, 0, 0, tex, 1);
                if (!full) {
                    Render_AlphaTestOff(R());
                    Renderer_SetTexture(R(), 0, 3, F<uint8_t*>(ter, 8 + frame * 4));
                    Terrain_DrawChunksShaded(R(), 0, F<uint8_t*>(ter, 8), 2, nullptr, 0, 1, anim, 0);
                    Renderer_SetTexture(R(), 0, 3, nullptr);
                }
            } else
                Terrain_DrawChunksShaded(R(), 0, tex, 1, nullptr, 0, 0, nullptr, 0);
            Render_AlphaTestOff(R());
        } else {
            Render_PushStages(R(), 0, F<const uint32_t*>(s, 0x35));
            Terrain_DrawChunksPaired(R(), 0, F<uint8_t*>(ter, 8 + frame * 4), 1, nullptr, 0, 0);
            if (!full && F<uint8_t*>(s, 0x29)[0x61]) {
                Scene_DeviceRs(0x2f, 1);
                Render_SetTexCoordIndex(R(), 0, 1, 0);
                Render_PushStages(R(), 0, F<const uint32_t*>(s, 0x39));
                Render_AddressClamp(R(), 0, 0);
                Render_AddressClamp(R(), 0, 1);
                Render_SetTextureFactor(R(), 0, 0x7f7fafff);
                Terrain_DrawChunksPaired(R(), 0, F<uint8_t*>(ter, 8), 2, nullptr, 0, 1);
                Render_AddressWrap(R());
                Render_SetTexCoordIndex(R(), 0, 1, 1);
                Render_PopStages(R(), 0, 1);
                Scene_DeviceRs(0x2f, 0);
            }
        }
        if (pop) Render_PopStages(R(), 0, 1);
        if (ter[0x159b] && !full) {
            int32_t rows = Terrain_Rows(ter), cols = Terrain_Cols(ter);
            int32_t q[4];
            Render_GetRect15a4(ter, 0, (uint32_t*)&q[0], (uint32_t*)&q[1], (uint32_t*)&q[2], (uint32_t*)&q[3]);
            float sx = (float)((double)rows / (double)(q[2] - q[0]));
            float sy = (float)((double)cols / (double)(q[3] - q[1]));
            float oy = (float)-((double)q[1] / (double)cols * sy);
            float ox = (float)-((double)q[0] / (double)rows * sx);
            Render_SetShaderOffset(R(), 0, ox, oy, sx, sy);
            if (!ter[0x159a]) Render_SetFiltering(R(), 0, 0, -1);
            Scene_DeviceRs(0x2f, 1);
            Terrain_DrawChunks(R(), 0, g_cc3b34);
            Scene_DeviceRs(0x2f, 0);
            if (!ter[0x159a]) Render_SetFiltering(R(), 0, 1, -1);
            Render_SetShaderOffset(R(), 0, 0.0f, 0.0f, 1.0f, 1.0f);
            Render_AddressWrap(R());
        }
        if (F<uint8_t*>(s, 0x29)[0x64] && !F<int32_t>(g_engine, 0x11426e) && !full) Scene_DrawDetailLayers(s, 0, ter, cam);
        if (F<uint8_t*>(s, 0x29)[0x5c] && !full && F<int32_t>(s, 0x3abb7)) {
            Render_BindVb2647(R());
            Render_LightingOff(R());
            Render_FogOff(R());
            Render_PushStages(R(), 0, F<const uint32_t*>(s, 0x41));
            Renderer_SetTexture(R(), 0, 0, F<uint8_t*>(s, 0x3659));
            Render_CullCcw(R());
            Render_AlphaTestOn(R(), 0, 0x3f);
            uint8_t fogBlock[0x40];
            Render_GetFogBlock(R(), 0, fogBlock);
            Terrain_Rows(ter);
            Terrain_Cols(ter);
            for (int32_t g = 0; g < 4; ++g)
                for (int32_t i = F<int32_t>(s, 0x3abbb + g * 8); i < F<int32_t>(s, 0x3abbf + g * 8); ++i) F<uint32_t>(s, 0x22bc3 + i * 0x18) = 0xff7f7f7f;
            int32_t total = F<int32_t>(s, 0x3abb7) + 0x20, c = F<int32_t>(s, 0x71);
            void* p;
            if (c == -1 || c > 0xfde8 - total) {
                F<int32_t>(s, 0x71) = 0;
                F<int32_t>(s, 0x75) = 0;
                p = Render_LockVb2647(R(), 0, 0, total);
            } else {
                F<int32_t>(s, 0x75) = c;
                p = Render_AppendVb2647(R(), 0, c, c + total);
            }
            memcpy(p, s + 0x22bb7, F<int32_t>(s, 0x3abb7) * 0x18);
            F<int32_t>(s, 0x71) += F<int32_t>(s, 0x3abb7);
            Render_UnlockVb2647(R());
            if (F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(R(), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
            Render_LightingDefault(R());
            Render_PopStages(R(), 0, 1);
            Render_FogOn(R());
        }
    }
    (void)objects;
    auto R = [&]() { return F<uint8_t*>(s, 0x2d); };
    if (!full) {
        PtrList* l = F<PtrList*>(s, 0x79);
        PtrList_Rewind(l);
        for (uint8_t* o = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x79)); o; o = (uint8_t*)PtrList_Next(F<PtrList*>(s, 0x79)))
            ((void(__fastcall*)(uint8_t*))(*(void***)o)[1])(o);
        int32_t cull = F<int32_t>(g_engine, 0x11420a);
        if (cull == 0) Render_CullNone(R());
        else if (cull == 1) Render_CullCw(R());
        else if (cull == 2) Render_CullCcw(R());
        Scene_QueueWorld(s, 0, cam, ter, 0);
        Scene_DrawCellLayer(s, 0, cam, ter, 0);
        Scene_DrawCellLayer(s, 0, cam, ter, 1);
        uint8_t* v = F<uint8_t*>(s, 0x29);
        eng = g_engine;
        if ((v[0x65] || v[0x66]) && eng[0x11422e] && !eng[0x11424f] && ((s[0x3748] && F<int32_t>(s, 5) >= 0) || F<int32_t>(s, 5) > 3) && fogDense) {
            Scene_DrawFogPatch(s, 0, ter, 0);
            F<uint8_t*>(s, 0x2d)[0x36f7] = 0;
        }
        Scene_DrawCellBoxes(s, 0, cam, ter, 0);
        Scene_DrawCellQuads(s, 0, cam, ter);
        Scene_DrawScenery(s, 0, cam, ter);
        Scene_DrawEmitters(s, 0, cam);
        Rain_Update(s, 0, ter);
        Particles_Render(s, 0, cam);
        Render_CullCcw(R());
    } else {
        eng = g_engine;
        if (eng[0x11422e]) {
            Render_CullCw(R());
            Render_BlendOff(R());
            Render_AlphaTestOff(R());
            uint8_t keep = g_engine[0x11422d];
            g_engine[0x11422d] = 0;
            Render_ColorWriteOff(R());
            Terrain_DrawChunksPaired(R(), 0, F<uint8_t*>(ter, 8), 2, nullptr, 0, 0);
            g_engine[0x11422d] = keep;
            Render_ColorWriteOn(R());
            Render_BlendDefault(R());
            Render_AlphaTestOn(R(), 0, 1);
        }
        Render_CullNone(R());
        Render_ZFuncAlways(R());
        uint32_t keepDist = Render_Get1534(ter);
        Render_Set1534(ter, 0, (int32_t)keepDist >= 0x34 ? 0x34 : keepDist);
        Scene_QueueWorld(s, 0, cam, ter, 0);
        Render_Set1534(ter, 0, keepDist);
        Render_ZFuncLessEqual(R());
        Render_CullCcw(R());
        Rain_Update(s, 0, ter);
    }
    if (!full) {
        if (Render_FogEnabled(ter)) Scene_VisitTerrain(s, 0, ter);
        Marker_Render(s, 0, cam, ter);
        Scene_UpdateLensFlare(s);
        Cursor_Draw(g_cursorManager);
        float dt = (float)Engine_AverageFrameMs(g_engine);
        SceneStats_Draw(s, 0, dt);
        Scene_DrawOverlay(s, 0, 0);
        s[0x3749] = 0;
    }
    if (!g_624d54) g_624d54 = 1;
    Render_PopStages(R(), 0, 1);
    Render_EndFrame(R(), 0, full);
    Engine_SampleTextureMem(g_engine);
    if (full) {
        Camera_SetLock65(cam, 0, 0);
        F<int32_t>(s, 0x22b7b) = F<int32_t>(R(), 0x3711);
        F<int32_t>(s, 0x22b7f) = F<int32_t>(R(), 0x370d);
        Render_RestoreTarget(R());
    } else {
        Render_SetCameraViewport(R(), 0, cam, 0);
        Engine_UpdateFrameTime(g_engine);
    }
    ++F<int32_t>(s, 5);
    if (full) {
        F<uint32_t>(F<uint8_t*>(s, 0x29), 0x25) = keepNear;
        Render_ApplyFilterStates(R());
    }
    g_engine[0x11369c] = 0;
    if (full) Scene_RenderFrame(s, 0, cam, ter, 1);
    Render_UseView3(R());
    (void)r;
}
RT3_REPLACE(0x52b990, Scene_RenderFrame);

}  // namespace rt3
