// Terrain.cpp - RT3_mp.exe 0x532000-0x538000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <initializer_list>
#include <stdio.h>
#include <windows.h>
#include "hook/hook.h"
#include "core/stream.h"
#include "core/crt.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

double __stdcall Plane_HeightAt(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2,
                                float z2, float qx, float qz);

void __fastcall SceneObj_Destructor(uint8_t* o);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall PtrList_Destroy(PtrList* l);
void __fastcall Terrain_DestroyCell1681(uint8_t* r);
void __fastcall Terrain_DestroyCell1685(uint8_t* r);
void __fastcall Terrain_DestroyCell1679(uint8_t* r);
void __fastcall Terrain_DestroyCell1675(uint8_t* r);
void __fastcall Terrain_DestroyCell167d(uint8_t* r);
void __fastcall Render_AllocGrid(uint8_t* r, int, int32_t rows, int32_t cols);
void __fastcall Terrain_AllocMips(uint8_t* t);
void __fastcall Terrain_FreeFixedChannels(uint8_t* r);
void __fastcall Render_ReleaseTextureSlots(uint8_t* r);
void __fastcall Texture_Destroy(uint8_t* t);
void* __fastcall Engine_Get1C(void* e);
void __fastcall Engine_Set1C(void* e, int, void* v);
void __fastcall Water_BuildQuads(uint8_t* g);
float __fastcall Terrain_ComputeVertexNormal(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_ShadeVertex(uint8_t* t, int, int32_t x, int32_t y, int32_t);
void __fastcall Terrain_RebuildMipsRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Fatal_Exit(const char* msg);
const char* __fastcall GetString(int32_t id);
void __fastcall Terrain_RebuildMips(uint8_t* t);
uint8_t* __fastcall WaterGrid_Init(uint8_t* g, int, int32_t bx, int32_t by, uint8_t* t);
void __fastcall Obj_GetXYZ1c(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall Effect_Release(uint8_t* e);
void __fastcall Texture_MakeBumpMaps(uint8_t* o);
int32_t __fastcall Texture_CreateTarget(uint8_t* t, int, int32_t w, int32_t h, void* dev);
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
void __fastcall Texture_DropTarget(uint8_t* t);
void __fastcall Render_DestroyCurrentTexture(uint8_t* r);
uint8_t* __fastcall Texture_ConstructDefault(uint8_t* t);
void __fastcall Render_CreateOverviewTarget(uint8_t* r, int, int32_t size);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
void __fastcall Sound_ReleaseTimer(uint8_t* s);
int32_t __fastcall Engine_TotalTextureMem(void* e);
uint32_t __fastcall SysInfo_TotalMemory();
int32_t __fastcall PowerOfTwo(int32_t v, int32_t roundUp);
void __fastcall Lists_Destroy(uint8_t* o);
void __fastcall BlockGrid_Set(uint8_t* g, int, int32_t, int32_t a, int32_t b, uint8_t v);
uint8_t __fastcall BlockGrid_Get(uint8_t* g, int, int32_t a, int32_t b);
void __fastcall BlockGrid_Free(uint8_t* g);
void __fastcall BlockGrid_SetByte(uint8_t* g, int, int32_t a, int32_t b, uint8_t v);
uint32_t __fastcall BlockGrid_Byte(uint8_t* g, int, int32_t a, int32_t b);
uint32_t __fastcall BlockGrid_Count(uint8_t* g, int, int32_t a, int32_t b);
void __fastcall BlockGrid_SetCount(uint8_t* g, int, int32_t a, int32_t b, uint8_t n);
int32_t __fastcall BlockGrid_Dec(uint8_t* g, int, int32_t a, int32_t b);
void __fastcall Channel_Detach(uint8_t* c);

void __fastcall Plane_From3Points(float* nx, float* ny, float x0, float y0, float z0, float x1, float y1, float z1,
                                  float x2, float y2, float z2, float* nz, float* d);

void __fastcall Terrain_Init(uint8_t* t);
float* __fastcall Terrain_NormalAt(uint8_t* t, int, float* out, float x, float z);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t __fastcall Texture_SetPixels(uint8_t* t, int, void* pixels);
uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag);
void __fastcall Texture_SaveTga(uint8_t* t, int, const char* path, int32_t flag);
int32_t __fastcall Texture_AppendJpeg(uint8_t* t, int, const char* path, uint32_t withAlpha, int32_t quality);
void __stdcall Render_ClampTextureSize(int32_t* w, int32_t* h);
void __fastcall Mem_Fill32(uint32_t* dst, uint32_t v, uint32_t count);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
void __fastcall Texture_ImageSize(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
void __fastcall Texture_Shrink(uint8_t* o, int, int32_t idx, int32_t fx, int32_t fy);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
int32_t __fastcall TextureSlots_Reset(uint8_t* t);
int32_t __fastcall TextureSlots_Add(uint8_t* t, int, const char* name);
void __fastcall Weather_LoadSounds(uint8_t* s);
void __fastcall Water_LoadTextures(uint8_t* t);
void __fastcall Rain_LoadFrames(uint8_t* t);
void __fastcall Terrain_InitMaps(uint8_t* t, int, char init, int32_t, int32_t);

// One terrain overlay quad (0x31 bytes): 4 corners (x, y, z) and a raise over the ground.
struct Quad {
    float c[12];
    uint8_t raise;
};

void __fastcall Grid_RefreshPoints(uint8_t* g, int, uint8_t* t);
void __fastcall Grid_AddPoint(uint8_t* g, int, uint8_t* t, int32_t x, int32_t z, uint8_t kind);
uint8_t* __fastcall BlockGrid_Init(uint8_t* g, int, float w, float h);
void __fastcall Sound_Shutdown(uint8_t* snd, int, int32_t immediate);
int32_t __fastcall Sound_Startup(uint8_t* snd, int, const char* name);
uint32_t __fastcall Sound_Get3DProvider(int32_t n);
int32_t __fastcall SoundCache_Init(uint8_t* s, int, int32_t capacity);
void __fastcall SoundCache_ClearThunk(uint8_t* s);
int32_t __fastcall Sound_Restart(uint8_t* snd, int, const char* name);
void __fastcall Terrain_DownsampleHeight(uint8_t* t, int, int32_t x, int32_t y, int32_t level);
void __fastcall Terrain_DownsampleWord(uint8_t* t, int, int32_t x, int32_t y, int32_t level);

uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6,
                                  int32_t a7, float vol);

int32_t __fastcall Sound_StartInSlot(uint8_t* s, int, const char* name);

// Functions of this file, in exe order.
uint8_t* __fastcall Sound_Construct(uint8_t* snd, int, int32_t provider);
RT3_NOINLINE void __fastcall Sound_Destruct(uint8_t* snd);
int32_t __fastcall Sound_SelectProvider(uint8_t* snd, int, int32_t n);
void __fastcall Sound_StartFadeTimer(uint8_t* s, int, int32_t mode);
void __fastcall Render_SetFog(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
void __fastcall Render_DisableFog(uint8_t* r);
uint8_t __fastcall Render_FogEnabled(uint8_t* r);
void __fastcall Render_GetFog(uint8_t* r, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d, uint32_t* e, uint32_t* f, uint32_t* g);
void __fastcall Nop5323e0(void*, int, int32_t, int32_t, int32_t, int32_t, int32_t);
uint32_t* __fastcall Overview_Pixels(uint8_t* r, int, int32_t w, int32_t h);
void __fastcall Overview_Refresh(uint8_t* r);
void __fastcall Render_ColourMap(uint8_t* r, int, uint32_t** pix, uint32_t* w, uint32_t* h);
int32_t __fastcall Render_Enable159b(uint8_t* r);
void __fastcall Render_Disable159b(uint8_t* r);
void __fastcall Render_Set159a(uint8_t* r, int, uint8_t v);
void __fastcall Render_SetRect15a4(uint8_t* r, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
void __fastcall Render_GetRect15a4(uint8_t* r, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Terrain_Init(uint8_t* t);
void __fastcall Render_DestroyCurrentTexture(uint8_t* r);
void __stdcall Render_ClampTextureSize(int32_t* w, int32_t* h);
void __fastcall Render_FixTileSeams(uint8_t* r);
void __fastcall Render_SnapToGrid(uint8_t* r, int, int32_t* a, int32_t* b);
RT3_NOINLINE int32_t __fastcall Render_GridCell(uint8_t* r, int, int32_t x, int32_t y);
uint32_t __fastcall Render_Get1554(uint8_t* r);
uint32_t __fastcall Render_AnimFrameA(uint8_t* r);
uint32_t __fastcall Render_Get1578(uint8_t* r);
uint32_t __fastcall Render_AnimFrameB(uint8_t* r);
RT3_NOINLINE int32_t __fastcall TextureSlots_Add(uint8_t* t, int, const char* name);
RT3_NOINLINE void __fastcall Render_ReleaseTextureSlots(uint8_t* r);
RT3_NOINLINE void __fastcall Render_AllocGrid(uint8_t* r, int, int32_t rows, int32_t cols);
void __fastcall Terrain_ApplyHeightCurve(uint8_t* t, int, float k);
void __fastcall Terrain_SmoothPeaks(uint8_t* t, int, int32_t r);
RT3_NOINLINE void __fastcall Render_SetLayerValue(uint8_t* r, int, int32_t i, float v);
float __fastcall Render_GetLayerValue(uint8_t* r, int, int32_t i);
void __fastcall Render_Set1534(uint8_t* r, int, uint32_t v);
uint32_t __fastcall Render_Get1534(uint8_t* r);
void __fastcall Render_Set1540(uint8_t* r, int, uint32_t v);
uint32_t __fastcall Render_Get1540(uint8_t* r);
void __fastcall Render_Set153c(uint8_t* r, int, uint32_t v);
uint32_t __fastcall Render_Get153c(uint8_t* r);
void __fastcall Render_Set1544(uint8_t* r, int, uint32_t v);
uint32_t __fastcall Render_Get1544(uint8_t* r);
void __fastcall Render_Set14e0(uint8_t* r, int, uint32_t v);
float __fastcall Render_Get14e0(uint8_t* r);
uint32_t __fastcall Render_Get1548(uint8_t* r);
void __fastcall Terrain_AverageNormal(uint8_t* t, int, int32_t x, int32_t y, int32_t level);
void __fastcall Terrain_DownsampleHeight(uint8_t* t, int, int32_t x, int32_t y, int32_t level);
void __fastcall Terrain_DownsampleWord(uint8_t* t, int, int32_t x, int32_t y, int32_t level);
RT3_NOINLINE void __fastcall Terrain_AllocMips(uint8_t* t);
void __fastcall Terrain_RebuildMips(uint8_t* t);
void __fastcall Terrain_RebuildMipsRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE int32_t __fastcall Terrain_Rows(uint8_t* r);
RT3_NOINLINE int32_t __fastcall Terrain_Cols(uint8_t* r);
void __fastcall Terrain_Set15e1(uint8_t* r, int, uint32_t v);
void __fastcall Plane_From3Points(float* nx, float* ny, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float* nz, float* d);
double __stdcall Plane_HeightAt(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float qx, float qz);
RT3_NOINLINE uint32_t __fastcall Terrain_Get165d(uint8_t* r);
uint32_t __fastcall Terrain_Get1661(uint8_t* r);
uint32_t __fastcall Terrain_Get1665(uint8_t* r);
RT3_NOINLINE void* __fastcall Terrain_HeightsSlot(uint8_t* r);
uint32_t __fastcall Terrain_Get15e9(uint8_t* r);
uint32_t __fastcall Terrain_Get15ed(uint8_t* r);
uint32_t __fastcall Terrain_Layer(uint8_t* r, int, uint8_t i);
uint32_t __fastcall Terrain_Get1655(uint8_t* r);
uint32_t __fastcall Terrain_Get1659(uint8_t* r);
RT3_NOINLINE uint32_t __fastcall Terrain_Get15e5(uint8_t* r);
RT3_NOINLINE uint32_t __fastcall Terrain_Get1619(uint8_t* r, int, int8_t i);
void* __fastcall Terrain_Slot15f1(uint8_t* r);
PtrList* __fastcall Terrain_Cell1671(uint8_t* r, int, int32_t x, int32_t y);
PtrListNode* __fastcall Terrain_AddGlobal(uint8_t* r, int, void* v);
void __fastcall Terrain_RemoveGlobal(uint8_t* r, int, PtrListNode* n);
PtrList* __fastcall Terrain_GlobalList(uint8_t* r);
PtrListNode* __fastcall Terrain_Add1671(uint8_t* r, int, void* v, int32_t x, int32_t y);
void __fastcall Terrain_Remove1671(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
PtrList* __fastcall Terrain_Cell1679(uint8_t* r, int, int32_t x, int32_t y);
PtrListNode* __fastcall Terrain_Add1679(uint8_t* r, int, void* v, int32_t x, int32_t y);
void __fastcall Terrain_Remove1679(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
PtrList* __fastcall Terrain_Cell167d(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Terrain_FreeCell167d(uint8_t* r);
PtrList* __fastcall Terrain_Cell1675(uint8_t* r, int, int32_t x, int32_t y);
PtrListNode* __fastcall Terrain_Add1675(uint8_t* r, int, void* v, int32_t x, int32_t y);
void __fastcall Terrain_Remove1675(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
PtrList* __fastcall Terrain_Cell1681(uint8_t* r, int, int32_t x, int32_t y);
PtrList* __fastcall Terrain_Cell1685(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Terrain_RefreshPointGrids(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
uint32_t __fastcall Terrain_CellByte(uint8_t* t, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
uint8_t __fastcall Terrain_GetFlag(uint8_t* t, int, int32_t x, int32_t y);
RT3_NOINLINE uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_SetCellCount(uint8_t* t, int, int32_t x, int32_t y, uint8_t n);
RT3_NOINLINE void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count);
RT3_NOINLINE void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n);
int32_t __fastcall Grid_SlotPos(uint8_t* o, int, int32_t col, int32_t row, int32_t slot, int32_t, float* outX, float* outY, uint32_t* outV, uint8_t* outFlag);
RT3_NOINLINE int32_t __fastcall Terrain_AddMarker(uint8_t* r, int, uint32_t v, int32_t, int32_t);
RT3_NOINLINE void __fastcall Terrain_DeleteMarkers(uint8_t* r);
RT3_NOINLINE double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
float* __fastcall Terrain_NormalAt(uint8_t* t, int, float* out, float x, float z);
RT3_NOINLINE void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
uint32_t __fastcall Render_Get1588(uint8_t* r);
uint32_t __fastcall Render_Get1584(uint8_t* r);
RT3_NOINLINE uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW, int32_t* outH);
RT3_NOINLINE void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard);
void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h);
RT3_NOINLINE void __fastcall Terrain_SaveTexture(uint8_t* t, int, const char* path, int32_t mode, int32_t quality);
void __fastcall Nop534e00(void*, int, int32_t, int32_t, int32_t, int32_t);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE uint32_t __fastcall Terrain_RecBit9(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y);
RT3_NOINLINE uint8_t* __fastcall Terrain_Construct(uint8_t* t);
void __fastcall Render_DestroyColourMap(uint8_t* r);
void __fastcall Render_CreateOverviewTarget(uint8_t* r, int, int32_t size);
int32_t __fastcall TextureSlots_Reset(uint8_t* t);
RT3_NOINLINE void __fastcall Terrain_BuildTiles(uint8_t* t, int, int32_t w, int32_t h, uint8_t* src);
RT3_NOINLINE int32_t __fastcall Terrain_LoadTexture(uint8_t* t, int, const char* name, int32_t fullSize);
void __fastcall Water_LoadTextures(uint8_t* t);
void __fastcall Rain_LoadFrames(uint8_t* t);
int32_t __fastcall Sound_StartInSlot(uint8_t* s, int, const char* name);
RT3_NOINLINE void __fastcall Terrain_FreeFixedChannels(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_DestroyCell1675(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_FreeBlockObjects(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_DestroyCell1681(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_DestroyCell1685(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_DestroyCell1679(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_DestroyCell167d(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_Free(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_Resize(uint8_t* r, int, int32_t w, int32_t h);
float __fastcall Terrain_ComputeVertexNormal(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Render_SetOverviewSize(uint8_t* r, int, int32_t size);
void __fastcall Terrain_ShadeVertex(uint8_t* t, int, int32_t x, int32_t y, int32_t);
RT3_NOINLINE void __fastcall Terrain_RefreshWaterFlow(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
PtrListNode* __fastcall Terrain_AddQuad(uint8_t* t, int, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, uint8_t raise);
void __fastcall Terrain_RefreshQuads(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __stdcall Camera_DetailFactor(int32_t x, int32_t z, float* out);
RT3_NOINLINE void __fastcall Fog_ParamsAt(uint8_t*, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color, float* strength);
RT3_NOINLINE void __fastcall Terrain_Destroy(uint8_t* r);
void __fastcall Weather_LoadSounds(uint8_t* s);
RT3_NOINLINE void __fastcall Terrain_RebuildWaterMask(uint8_t* t);
RT3_NOINLINE void __fastcall Terrain_RefreshRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
void __fastcall Terrain_InitMaps(uint8_t* t, int, char init, int32_t, int32_t);
RT3_NOINLINE void __fastcall Terrain_Destructor(uint8_t* r);
RT3_NOINLINE void __fastcall Terrain_Create(uint8_t* t, int, int32_t w, int32_t h, const char* tex);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
int32_t __fastcall Texture_CreateTarget(uint8_t* t, int, int32_t w, int32_t h, void* dev);
RT3_NOINLINE uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
RT3_NOINLINE uint8_t* __fastcall Texture_ConstructDefault(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Texture_SetPixels(uint8_t* t, int, void* pixels);
RT3_NOINLINE uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag);

// Per-16x16-block object lists: grids +0x1671, +0x1679, +0x167d (rows / 16 per line)
// and a global list +0x166d.
static inline PtrList* Cell(uint8_t* r, uint32_t grid, int32_t x, int32_t y) {
    return F<PtrList**>(r, grid)[(F<int32_t>(r, 0x15d9) >> 4) * y + x];
}

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

// Terrain vertex records (+0x165d, 3 bytes, stride +0x15dd): low 3 bits = kind.
static inline uint8_t* Rec(uint8_t* r, int32_t x, int32_t y) {
    return F<uint8_t*>(r, 0x165d) + (F<int32_t>(r, 0x15dd) * y + x) * 3;
}

namespace {
inline int32_t FlowFtol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}
}

namespace {
inline void* TerrainMalloc(uint32_t n) { return ((void*(__cdecl*)(uint32_t))0x5a125d)(n); }
inline void* TerrainNew(uint32_t n) { return ((void*(__cdecl*)(uint32_t))0x53b070)(n); }
inline void TerrainProgress() {
    if (hook::Global<void(__cdecl*)()>(0x6d4014)) hook::Global<void(__cdecl*)()>(0x6d4014)();
}
// A missing map gets `size` bytes filled with `fill`.
inline void TerrainAllocFilled(uint8_t* t, uint32_t off, uint32_t size, uint8_t fill) {
    if (F<void*>(t, off)) return;
    void* p = TerrainMalloc(size);
    F<void*>(t, off) = p;
    memset(p, fill, size);
}
// A missing block grid (bx x by, row stride cols >> 4) of empty pointer lists.
inline void TerrainAllocLists(uint8_t* t, uint32_t off, int32_t bx, int32_t by) {
    if (F<void*>(t, off)) return;
    TerrainAllocFilled(t, off, (uint32_t)(by * bx * 4), 0);
    for (int32_t i = 0; i < bx; ++i) {
        TerrainProgress();
        for (int32_t j = 0; j < by; ++j) {
            PtrList* l = (PtrList*)TerrainNew(0x18);
            l = l ? PtrList_Init(l) : nullptr;
            F<PtrList**>(t, off)[(F<int32_t>(t, 0x15d9) >> 4) * j + i] = l;
        }
    }
}
}

static inline double Fsqrt(double v) { double r; __asm { fld v } __asm { fsqrt } __asm { fstp r } return r; }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

// The texture name from the main pack's cache (all mips, flag 0x7f).
static uint8_t* Texture_Acquire(const char* name) { return TextureCache_Get(hook::Global<uint8_t*>(0x6d4020), 0, name, 0, 0, 0x7f); }

// The first free slot of t (-1 when full) gets texture name.
static int32_t TextureSlots_AddInline(uint8_t* t, const char* name) {
    int32_t slot = 0;
    while (F<void*>(t, 8 + slot * 4)) {
        if (++slot >= 0x100) return -1;
    }
    F<uint8_t*>(t, 8 + slot * 4) = Texture_Acquire(name);
    ++F<int32_t>(t, 4);
    return slot;
}

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }

// Terrain sound-flag grids: one block grid per 16x16 vertices (+0x1685).
static inline uint8_t* FlagGrid(uint8_t* t, int32_t x, int32_t y) {
    return F<uint8_t**>(t, 0x1685)[(F<int32_t>(t, 0x15d9) >> 4) * (y >> 4) + (x >> 4)];
}

// Terrain flag grids (+0x1685, one per 16x16 vertices).
static inline uint8_t** GridSlot(uint8_t* t, int32_t x, int32_t y) {
    return F<uint8_t**>(t, 0x1685) + ((F<int32_t>(t, 0x15d9) >> 4) * (y >> 4) + (x >> 4));
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

// Calls the optional hook 0x6d4014 (ECX as the original left it).
static inline void InitHook(void* ecx) {
    if (void* f = hook::Global<void*>(0x6d4014)) ((void(__thiscall*)(void*))f)(ecx);
}

namespace {
// Writes texel (x, y) of terrain t's 4 x 4 tile textures through the
// per-call pixel cache (filled from the tiles; slot = tile index + 1).
inline void TerrainPutTexel(uint8_t* t, uint32_t** cache, int32_t x, int32_t q, int32_t base, uint32_t v) {
    int32_t idx = x / F<int32_t>(t, 0x157c) + q * 4 + 1;
    if (!cache[idx]) cache[idx] = Texture_Pixels32(F<uint8_t*>(t, 8 + idx * 4), 0, nullptr, nullptr, 0);
    cache[idx][x % F<int32_t>(t, 0x157c) + base] = v;
}
inline void TerrainCommitCache(uint8_t* t, uint32_t** cache) {
    for (int32_t k = 0; k < 0xff; ++k)
        if (cache[k]) Texture_SetPixels(F<uint8_t*>(t, 8 + k * 4), 0, cache[k]);
}
}

// Puts the raised quad's corners on the terrain.
static void DropQuad(uint8_t* t, Quad* q) {
    float r = (float)q->raise;
    for (int k = 0; k < 4; ++k)
        q->c[k * 3 + 1] = (float)(Terrain_HeightAt(t, 0, q->c[k * 3], q->c[k * 3 + 2]) + r);
}

// Point grid of 16x16-cell block (bx, by).
static inline uint8_t*& PointGrid(uint8_t* t, int32_t bx, int32_t by) {
    return F<uint8_t**>(t, 0x1685)[(F<int32_t>(t, 0x15d9) >> 4) * by + bx];
}

static inline void MipCell(uint8_t* t, int32_t x, int32_t y, int32_t level) {
    Terrain_AverageNormal(t, 0, x, y, level);
    Terrain_DownsampleHeight(t, 0, x, y, level);
    Terrain_DownsampleWord(t, 0, x, y, level);
}

// 0x532150. Constructs the sound system: default volumes (1.0), no timer (+0x4c),
// starts it with 3D provider `provider` (-1 = none) and a 1 MB sound cache (+0).
uint8_t* __fastcall Sound_Construct(uint8_t* snd, int, int32_t provider) {
    F<int32_t>(snd, 4) = 0;
    hook::Global<void*>(0xccb9f8) = nullptr;
    hook::Global<void*>(0xccb9fc) = nullptr;
    hook::Global<void*>(0xccba00) = nullptr;
    F<void*>(snd, 0) = nullptr;
    hook::Global<uint32_t>(0x624d5c) = 0x3f800000;
    F<int32_t>(snd, 0x4c) = -1;
    F<int32_t>(snd, 0x50) = 0;
    memset(snd + 8, 0, 0x40);
    F<uint32_t>(snd, 0x44) = 0x3f800000;
    F<uint32_t>(snd, 0x30) = 0x3f800000;
    F<uint32_t>(snd, 0x1c) = 0x3f800000;
    F<uint32_t>(snd, 8) = 0x3f800000;
    F<int32_t>(snd, 0x48) = 0;
    const char* name = provider != -1 ? (const char*)Sound_Get3DProvider(provider) : nullptr;
    if (Sound_Startup(snd, 0, name) == 1) {
        uint8_t* c = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x44);
        if (c) F<int32_t>(c, 0x10) = 0;
        F<uint8_t*>(snd, 0) = c;
        SoundCache_Init(c, 0, 0x100000);
    }
    return snd;
}
RT3_REPLACE(0x532150, Sound_Construct);

// 0x532200. Destroys the sound system: shuts it down (fading) and frees the cache.
RT3_NOINLINE void __fastcall Sound_Destruct(uint8_t* snd) {
    if (F<int32_t>(snd, 4) != 1) return;
    Sound_Shutdown(snd, 0, 0);
    if (uint8_t* c = F<uint8_t*>(snd, 0)) {
        SoundCache_ClearThunk(c);
        ((void(__cdecl*)(void*))0x53b080)(c);
        F<void*>(snd, 0) = nullptr;
    }
}
RT3_REPLACE(0x532200, Sound_Destruct);

// 0x532230. Restarts the sound system with 3D provider number n (if 3D is up).
int32_t __fastcall Sound_SelectProvider(uint8_t* snd, int, int32_t n) {
    if (!hook::Global<void*>(0xccb9fc)) return 0;
    return Sound_Restart(snd, 0, (const char*)Sound_Get3DProvider(n));
}
RT3_REPLACE(0x532230, Sound_SelectProvider);

// 0x532260. (Re)starts the music fade timer (+0x4c, mode +0x50: 0 fades in from
// silence) via the Miles timer API.
void __fastcall Sound_StartFadeTimer(uint8_t* s, int, int32_t mode) {
    if (F<int32_t>(s, 4) != 1 || hook::Global<int32_t>(0xccb9f4)) return;
    if (F<int32_t>(s, 0x4c) != -1) {
        if (mode == F<int32_t>(s, 0x50)) return;
        hook::Global<int32_t>(0x624d58) = 1;
        Sound_ReleaseTimer(s);
    }
    int32_t t = Import<int32_t(__stdcall*)(uint32_t)>(0x5c83f4)(0x531fd0);
    F<int32_t>(s, 0x4c) = t;
    if (t == -1) return;
    F<int32_t>(s, 0x50) = mode;
    hook::Global<uint32_t>(0x624d60) = 0xbf800000;
    uint32_t user = 1;
    if (!mode) {
        Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, 0.0f);
        t = F<int32_t>(s, 0x4c);
        user = 2;
    }
    Import<void(__stdcall*)(int32_t, uint32_t)>(0x5c8400)(t, user);
    Import<void(__stdcall*)(int32_t, uint32_t)>(0x5c83c8)(F<int32_t>(s, 0x4c), 3);
    hook::Global<int32_t>(0x624d58) = 0;
    Import<void(__stdcall*)(int32_t)>(0x5c83a0)(F<int32_t>(s, 0x4c));
}
RT3_REPLACE(0x532260, Sound_StartFadeTimer);

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

// 0x5323e0 (vtable stub).
void __fastcall Nop5323e0(void*, int, int32_t, int32_t, int32_t, int32_t, int32_t) { __asm nop }
RT3_REPLACE(0x5323e0, Nop5323e0);

// 0x5323f0. The 32-bit pixels of terrain r's overview bitmap (+0x478),
// created w x h (kept in +0x159c / +0x15a0) on first use.
uint32_t* __fastcall Overview_Pixels(uint8_t* r, int, int32_t w, int32_t h) {
    if (!F<uint8_t*>(r, 0x478)) {
        F<int32_t>(r, 0x159c) = w;
        F<int32_t>(r, 0x15a0) = h;
        uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0xec);
        F<uint8_t*>(r, 0x478) = p ? Texture_InitMemory(p, 0, nullptr, w, h, 1) : nullptr;
    }
    return Texture_Pixels32(F<uint8_t*>(r, 0x478), 0, nullptr, nullptr, 0);
}
RT3_REPLACE(0x5323f0, Overview_Pixels);

// 0x532460. Re-uploads terrain r's overview bitmap (+0x478).
void __fastcall Overview_Refresh(uint8_t* r) {
    Texture_SetPixels(F<uint8_t*>(r, 0x478), 0, Texture_Pixels32(F<uint8_t*>(r, 0x478), 0, nullptr, nullptr, 0));
}
RT3_REPLACE(0x532460, Overview_Refresh);

// 0x532490. The terrain colour map (+0x478) pixels and its size (+0x159c/+0x15a0).
void __fastcall Render_ColourMap(uint8_t* r, int, uint32_t** pix, uint32_t* w, uint32_t* h) {
    if (pix) *pix = F<uint8_t*>(r, 0x478) ? Texture_Pixels32(F<uint8_t*>(r, 0x478), 0, nullptr, nullptr, 0) : nullptr;
    if (w) *w = F<uint32_t>(r, 0x159c);
    if (h) *h = F<uint32_t>(r, 0x15a0);
}
RT3_REPLACE(0x532490, Render_ColourMap);

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

// 0x532590. Resets the terrain renderer's fields to their initial state.
RT3_NOINLINE void __fastcall Terrain_Init(uint8_t* t) {
    t[0x1588] = 0;
    F<int32_t>(t, 0x154c) = 0;
    F<int32_t>(t, 0x15d9) = 0;
    F<int32_t>(t, 0x15d5) = 0;
    F<int32_t>(t, 0x155c) = -1;
    InitHook(t);
    t[0x1599] = 0;
    t[0x15b4] = 0;
    t[0x159b] = 0;
    t[0x159a] = 1;
    static const uint16_t zero1[] = { 0x478, 0x1568, 0x1570, 0x1558, 0x1560, 0x1564, 0x159c, 0x15a0, 0x15e1,
                                      0x1605, 0x15f1, 0x1619, 0x162d, 0x1631, 0x1635, 0x1639, 0x163d, 0x1669,
                                      0x1655, 0x1659, 0x165d, 0x1661, 0x1665 };
    for (uint16_t off : zero1) F<int32_t>(t, off) = 0;
    InitHook(t + 0x1631);
    for (int k = 0; k < 0x100; ++k) F<int32_t>(t, 8 + k * 4) = 0;
    for (int k = 0; k < 0x1b; ++k) F<int32_t>(t, 0x40c + k * 4) = 0;
    static const uint16_t zero2[] = { 0x166d, 0x1671, 0x1675, 0x1679, 0x167d, 0x1685, 0x1681, 0x15d1 };
    for (uint16_t off : zero2) F<int32_t>(t, off) = 0;
    for (int k = 0; k < 0x2a3; ++k) F<int32_t>(t, 0x1695 + k * 4) = 0;
    F<int32_t>(t, 0x1691) = 0;
    F<int32_t>(t, 0x15e9) = 0;
    F<int32_t>(t, 0x15ed) = 0;
    F<uint32_t>(t, 0x14e0) = 0x3f800000;
    F<int32_t>(t, 0x1534) = 0x60;
    for (int k = 0; k < 0x14; ++k) F<uint32_t>(t, 0x14e4 + k * 4) = 0x3f800000;
    InitHook(nullptr);
    F<int32_t>(t, 0x15e5) = 0;
    F<int32_t>(t, 0x1544) = 0x18;
    F<int32_t>(t, 0x1540) = 0x60;
    F<int32_t>(t, 0x153c) = 7;
    F<int32_t>(t, 0x1538) = 0x40;
    F<int32_t>(t, 0x1548) = 0x200;
    F<int32_t>(t, 0x1550) = 4;
    for (int k = 0; k < 0x19; ++k) F<int32_t>(t, 0x147c + k * 4) = 0;
}
RT3_REPLACE(0x532590, Terrain_Init);

// 0x532760. Destroys the current overview texture (+8 + +0x1558 * 4).
RT3_NOINLINE void __fastcall Render_DestroyCurrentTexture(uint8_t* r) {
    int32_t i = F<int32_t>(r, 0x1558);
    if (!i) return;
    uint8_t* t = F<uint8_t*>(r, 8 + i * 4);
    if (!t) return;
    Texture_Destroy(t);
    ExeDelete(t);
    F<uint8_t*>(r, 8 + F<int32_t>(r, 0x1558) * 4) = nullptr;
}
RT3_REPLACE(0x532760, Render_DestroyCurrentTexture);

// 0x5327a0. Clamps a texture size to a power of two within the card's budget
// (256 / 512 / 1024 by texture memory and RAM) and the engine maximum.
RT3_NOINLINE void __stdcall Render_ClampTextureSize(int32_t* w, int32_t* h) {
    int32_t mem = Engine_TotalTextureMem(Engine());
    int32_t maxs;
    if (mem >= 0x3d00000 && SysInfo_TotalMemory() > 0xb400000) maxs = 0x400;
    else maxs = mem < 0x1a00000 ? 0x100 : 0x200;
    *w = PowerOfTwo(*w, 0);
    *h = PowerOfTwo(*h, 0);
    if (maxs < *w) *w = maxs;
    if (maxs < *h) *h = maxs;
    int32_t mw = F<int32_t>(Engine(), 0x114243);
    if (*w / 4 > mw) *w = mw << 2;
    int32_t mh = F<int32_t>(Engine(), 0x114247);
    if (*h / 4 > mh) *h = mh * 4;
}
RT3_REPLACE(0x5327a0, Render_ClampTextureSize);

// 0x532860. Fixes the seams between the 12 terrain overview tiles (+8.., from
// +0x155c, 4 per row): each tile's last row gets the first row of the tile below
// and (except in the last column) its last column the first column of the tile to the right.
void __fastcall Render_FixTileSeams(uint8_t* r) {
    for (int32_t k = 0; k < 0xc; ++k) {
        int32_t w, h;
        uint32_t* p = Texture_Pixels32(F<uint8_t*>(r, (F<int32_t>(r, 0x155c) + k) * 4 + 8), 0, &w, &h, 0);
        const uint32_t* below = Texture_Pixels32(F<uint8_t*>(r, (F<int32_t>(r, 0x155c) + k) * 4 + 0x18), 0, &w, &h, 0);
        uint32_t* row = p + (h - 1) * w;
        for (int32_t j = 0; j < w; ++j) row[j] = below[j];
        if (k % 4 != 3) {
            const uint32_t* right = Texture_Pixels32(F<uint8_t*>(r, (F<int32_t>(r, 0x155c) + k) * 4 + 0xc), 0, &w, &h, 0);
            for (int32_t i = 0; i < h; ++i) p[(i + 1) * w - 1] = right[i * w];
        }
        Texture_DropTarget(F<uint8_t*>(r, (F<int32_t>(r, 0x155c) + k) * 4 + 8));
    }
}
RT3_REPLACE(0x532860, Render_FixTileSeams);

// 0x532960. Converts (*a, *b) between grid units of the size W x H (+0x15d5 / +0x15d9):
// *a = (*a * 4 / (H / 16)) * (H / 4), *b = (*b * 4 / (W / 16)) * (W / 4).
void __fastcall Render_SnapToGrid(uint8_t* r, int, int32_t* a, int32_t* b) {
    int32_t w = F<int32_t>(r, 0x15d5), h = F<int32_t>(r, 0x15d9);
    *a = ((int32_t)((uint32_t)*a << 2) / (h / 16)) * (h / 4);
    *b = ((int32_t)((uint32_t)*b << 2) / (w / 16)) * (w / 4);
}
RT3_REPLACE(0x532960, Render_SnapToGrid);

// 0x5329e0. 1-based grid cell index of (x, y) in a 4-wide grid.
RT3_NOINLINE int32_t __fastcall Render_GridCell(uint8_t* r, int, int32_t x, int32_t y) {
    int32_t row = (int32_t)((uint32_t)y << 2) / (F<int32_t>(r, 0x15d5) / 16);
    int32_t col = (int32_t)((uint32_t)x << 2) / (F<int32_t>(r, 0x15d9) / 16);
    return col + row * 4 + 1;
}
RT3_REPLACE(0x5329e0, Render_GridCell);

uint32_t __fastcall Render_Get1554(uint8_t* r) { return F<uint32_t>(r, 0x1554); }
RT3_REPLACE(0x532a30, Render_Get1554);

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

uint32_t __fastcall Render_Get1578(uint8_t* r) { return F<uint32_t>(r, 0x1578); }
RT3_REPLACE(0x532a90, Render_Get1578);

// 0x532aa0. Current frame of animated texture set B (+0x1560, +0x1564 frames): time / 12.
uint32_t __fastcall Render_AnimFrameB(uint8_t* r) {
    uint32_t t = Import<uint32_t(__stdcall*)()>(0x5c8068)();
    uint32_t idx = (t / 12) % F<uint32_t>(r, 0x1564);
    return F<uint32_t>(r, (F<int32_t>(r, 0x1560) + idx) * 4 + 8);
}
RT3_REPLACE(0x532aa0, Render_AnimFrameB);

// 0x532ad0. Puts texture name into the first free slot of t's 256 (+8, count
// +4); its index, or -1 when all are taken.
RT3_NOINLINE int32_t __fastcall TextureSlots_Add(uint8_t* t, int, const char* name) {
    int32_t i = 0;
    while (F<void*>(t, 8 + i * 4)) {
        if (++i >= 0x100) return -1;
    }
    F<uint8_t*>(t, 8 + i * 4) = Texture_Acquire(name);
    ++F<int32_t>(t, 4);
    return i;
}
RT3_REPLACE(0x532ad0, TextureSlots_Add);

// 0x532b30. Releases the renderer's texture slots (+8, 256 of them): with
// managed textures (engine +0x11422e) every texture is destroyed; otherwise slot 0
// and the pack images (slots from 17 up) go back to the pack (count +4), the
// generated ones (1..16, and the dynamic ones below +0x1554) are destroyed.
// The slot +0x1558 is kept unless textures are managed.
RT3_NOINLINE void __fastcall Render_ReleaseTextureSlots(uint8_t* r) {
    auto destroy = [](uint8_t*& t) {
        if (t) {
            Texture_Destroy(t);
            ExeDelete(t);
        }
        t = nullptr;
    };
    uint8_t** slots = (uint8_t**)(r + 8);
    if (hook::Global<uint8_t*>(0x6d4024)[0x11422e]) {
        destroy(slots[0]);
    } else if (slots[0]) {
        Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, slots[0]);
        slots[0] = nullptr;
        --F<int32_t>(r, 4);
    }
    int32_t i = 1;
    for (; i < F<int32_t>(r, 0x1554); ++i) destroy(slots[i]);
    for (; i < 0x100; ++i) {
        if (i == F<int32_t>(r, 0x1558)) {
            if (hook::Global<uint8_t*>(0x6d4024)[0x11422e]) destroy(slots[i]);
            continue;
        }
        if (!slots[i]) continue;
        if (i >= 1 && i < 0x11) {
            Texture_Destroy(slots[i]);
            ExeDelete(slots[i]);
        } else {
            Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, slots[i]);
        }
        slots[i] = nullptr;
        --F<int32_t>(r, 4);
    }
    F<int32_t>(r, 0x1554) = 0;
    F<int32_t>(r, 0x1568) = 0;
    F<int32_t>(r, 0x1570) = 0;
    F<int32_t>(r, 0x155c) = -1;
    F<int32_t>(r, 0x1560) = 0;
    F<int32_t>(r, 0x1564) = 0;
}
RT3_REPLACE(0x532b30, Render_ReleaseTextureSlots);

// 0x532c80. (Re)allocates the terrain grid buffers for (rows + 1) x (cols + 1)
// vertices: heights +0x1605 (zeroed), four layers +0x1631.., +0x1655 (2s),
// +0x1659 (1s) and +0x165d (3 bytes each, zeroed).
RT3_NOINLINE void __fastcall Render_AllocGrid(uint8_t* r, int, int32_t rows, int32_t cols) {
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

// 0x532d90. Reshapes the heights (+0x1605, (cols+1) stride) by a 101-step curve of
// steepness k (clamped to 0x5cb4a8..10): heights are scaled by the curve at their
// fraction of the highest point (at least 1).
void __fastcall Terrain_ApplyHeightCurve(uint8_t* t, int, float k) {
    int32_t rows = F<int32_t>(t, 0x15d5);
    double mx = Fl(0x5c85b4);
    for (int32_t y = 0; y < rows; ++y)
        for (int32_t x = 0; x < F<int32_t>(t, 0x15d9); ++x) {
            float h = F<float*>(t, 0x1605)[(F<int32_t>(t, 0x15d9) + 1) * y + x];
            if (mx < h) mx = h;
        }
    double kc = k;
    if (kc < D(0x5c86b8)) kc = Fl(0x5cb4a8);
    else if (kc > Fl(0x5c85f8)) kc = Fl(0x5c85f8);
    float tab[101];
    tab[0] = 1.0f;
    double inv = Fl(0x5c85b4) / kc;
    double acc = Fl(0x5c85b4);
    for (int i = 1; i <= 100; ++i) {
        double e = kc > Fl(0x5c85b4) ? kc : inv;
        acc = acc * (D(0x5c8770) - (e - D(0x5c8770)) * D(0x5ca1a8));
        tab[i] = (float)((double)tab[i - 1] + acc);
    }
    for (int n = 1; n <= 100; ++n) {
        float v = (float)(Fl(0x5c85b4) / (double)(n + 1) * tab[n]);
        tab[n] = v;
        if (kc > Fl(0x5c85b4)) tab[n] = (float)(D(0x5c8770) / v);
    }
    double s = kc / tab[100];
    for (int n = 1; n <= 100; ++n) tab[n] = (float)(s * tab[n]);
    for (int32_t y = 0; y < rows; ++y) {
        if (F<int32_t>(t, 0x15d9) <= 0) continue;
        double invMx = Fl(0x5c85b4) / mx;
        for (int32_t x = 0; x < F<int32_t>(t, 0x15d9); ++x) {
            float* h = &F<float*>(t, 0x1605)[(F<int32_t>(t, 0x15d9) + 1) * y + x];
            int32_t idx;
            __asm {
                fld invMx
                mov eax, h
                fmul dword ptr [eax]
                fmul dword ptr ds:[0x5c8c28]
                mov eax, 0x5a10d0
                call eax
                mov idx, eax
            }
            if (idx < 0) idx = 0;
            else if (idx > 100) idx = 100;
            *h = (float)((double)tab[idx] * *h);
        }
    }
}
RT3_REPLACE(0x532d90, Terrain_ApplyHeightCurve);

// 0x532f60. Smooths every height above 1 to the mean of the truncated heights in
// the (2r)x(2r) box from (x - r, y - r), in place.
void __fastcall Terrain_SmoothPeaks(uint8_t* t, int, int32_t r) {
    for (int32_t y = 0; y < F<int32_t>(t, 0x15d5); ++y) {
        int32_t cols = F<int32_t>(t, 0x15d9);
        for (int32_t x = 0; x < cols; ++x) {
            float* grid = F<float*>(t, 0x1605);
            float* cell = &grid[(cols + 1) * y + x];
            if ((double)*cell <= Fl(0x5c85b4)) {
                cols = F<int32_t>(t, 0x15d9);
                continue;
            }
            double sum = Fl(0x5c8598), count = Fl(0x5c8598);
            for (int32_t xx = x - r; xx < x + r; ++xx)
                for (int32_t yy = y - r; yy < y + r; ++yy) {
                    if (xx < 0 || xx >= cols || yy < 0 || yy >= F<int32_t>(t, 0x15d5)) continue;
                    sum += (int32_t)grid[(cols + 1) * yy + xx];
                    count += Fl(0x5c85b4);
                }
            if (count > Fl(0x5c8598)) *cell = (float)(sum / count);
            cols = F<int32_t>(t, 0x15d9);
        }
    }
}
RT3_REPLACE(0x532f60, Terrain_SmoothPeaks);

RT3_NOINLINE void __fastcall Render_SetLayerValue(uint8_t* r, int, int32_t i, float v) { F<float>(r, 0x14e4 + i * 4) = v; }
RT3_REPLACE(0x533100, Render_SetLayerValue);

// 0x533120..0x5331b0. Renderer layer settings.
float __fastcall Render_GetLayerValue(uint8_t* r, int, int32_t i) { return F<float>(r, 0x14e4 + i * 4); }
RT3_REPLACE(0x533120, Render_GetLayerValue);

void __fastcall Render_Set1534(uint8_t* r, int, uint32_t v) { F<uint32_t>(r, 0x1534) = v; }
RT3_REPLACE(0x533130, Render_Set1534);

uint32_t __fastcall Render_Get1534(uint8_t* r) { return F<uint32_t>(r, 0x1534); }
RT3_REPLACE(0x533140, Render_Get1534);

void __fastcall Render_Set1540(uint8_t* r, int, uint32_t v) { F<uint32_t>(r, 0x1540) = v; }
RT3_REPLACE(0x533150, Render_Set1540);

uint32_t __fastcall Render_Get1540(uint8_t* r) { return F<uint32_t>(r, 0x1540); }
RT3_REPLACE(0x533160, Render_Get1540);

void __fastcall Render_Set153c(uint8_t* r, int, uint32_t v) { F<uint32_t>(r, 0x153c) = v; }
RT3_REPLACE(0x533170, Render_Set153c);

uint32_t __fastcall Render_Get153c(uint8_t* r) { return F<uint32_t>(r, 0x153c); }
RT3_REPLACE(0x533180, Render_Get153c);

void __fastcall Render_Set1544(uint8_t* r, int, uint32_t v) { F<uint32_t>(r, 0x1544) = v; }
RT3_REPLACE(0x533190, Render_Set1544);

uint32_t __fastcall Render_Get1544(uint8_t* r) { return F<uint32_t>(r, 0x1544); }
RT3_REPLACE(0x5331a0, Render_Get1544);

void __fastcall Render_Set14e0(uint8_t* r, int, uint32_t v) { F<uint32_t>(r, 0x14e0) = v; }
RT3_REPLACE(0x5331b0, Render_Set14e0);

float __fastcall Render_Get14e0(uint8_t* r) { return F<float>(r, 0x14e0); }
RT3_REPLACE(0x5331c0, Render_Get14e0);

uint32_t __fastcall Render_Get1548(uint8_t* r) { return F<uint32_t>(r, 0x1548); }
RT3_REPLACE(0x5331d0, Render_Get1548);

// 0x5331e0. Averages the finer level's vec3 cells (+0x15ed[level], width
// (+0x15d9 >> (level - 1)) + 1) over the 3x3 block around (2x, 2y), clipped, into
// cell (x, y) of level +0x15f1[level]. The x sum is kept on the FPU stack between
// cells (rounded to float only at some steps), which this mirrors exactly.
void __fastcall Terrain_AverageNormal(uint8_t* t, int, int32_t x, int32_t y, int32_t level) {
    int32_t x0 = 2 * x - 1 < 0 ? 0 : 2 * x - 1;
    int32_t y0 = 2 * y - 1 < 0 ? 0 : 2 * y - 1;
    int32_t w = F<int32_t>(t, 0x15d9);
    int32_t sw = w >> (level - 1);
    int32_t x1 = sw - 1 >= 2 * x + 1 ? 2 * x + 1 : sw - 1;
    int32_t sh = F<int32_t>(t, 0x15d5) >> (level - 1);
    int32_t y1 = sh - 1 >= 2 * y + 1 ? 2 * y + 1 : sh - 1;
    double sx = 0.0;
    float sy = 0.0f, sz = 0.0f;
    int32_t n = 0;
    if (y0 > y1) return;
    int32_t cnt = x1 - x0 + 1;
    for (int32_t row = y0; row <= y1; ++row) {
        int32_t col = x0;
        const float* src = F<const float*>(t, 0x15ed + level * 4);
        if (cnt >= 4) {
            int32_t last;
            do {
                const float* e = src + ((sw + 1) * row + col) * 3;
                sx += e[0];
                double dy = (double)sy + e[1];
                sz = (float)((double)sz + e[2]);
                float tmp = (float)(sx + e[3]);
                dy += e[4];
                double dz = (double)sz + e[5];
                tmp = (float)((double)tmp + e[6]);
                sy = (float)(dy + e[7]);
                sz = (float)(dz + e[8]);
                sx = (double)tmp + e[9];
                sy = (float)((double)sy + e[10]);
                sz = (float)((double)sz + e[11]);
                n += 4;
                col += 4;
                last = col + 3;
            } while (last < x1);
        }
        if (col <= x1) {
            const float* e = src + ((sw + 1) * row + col) * 3;
            for (int32_t k = x1 - col + 1; k; --k, e += 3, ++n) {
                sx += e[0];
                sy = (float)((double)sy + e[1]);
                sz = (float)((double)sz + e[2]);
            }
        }
    }
    if (!n) return;
    float inv = (float)(1.0 / n);
    float* dst = F<float*>(t, 0x15f1 + level * 4) + (((w >> level) + 1) * y + x) * 3;
    dst[0] = (float)((double)inv * sx);
    dst[1] = (float)((double)inv * sy);
    dst[2] = (float)((double)inv * sz);
}
RT3_REPLACE(0x5331e0, Terrain_AverageNormal);

// 0x533430. Mip level `level` height (+0x1605 + 4 level) at (x, y): the mean of the
// level-1 heights (+0x1601 + 4 level) over (2x-1..2x+1, 2y-1..2y+1), clamped.
RT3_NOINLINE void __fastcall Terrain_DownsampleHeight(uint8_t* t, int, int32_t x, int32_t y, int32_t level) {
    int32_t x0 = 2 * x - 1 < 0 ? 0 : 2 * x - 1;
    int32_t y0 = 2 * y - 1 < 0 ? 0 : 2 * y - 1;
    int32_t w = F<int32_t>(t, 0x15d9);
    int32_t pw = w >> (level - 1);
    int32_t x1 = 2 * x + 1;
    if (pw - 1 < x1) x1 = pw - 1;
    int32_t ph = F<int32_t>(t, 0x15d5) >> (level - 1);
    int32_t y1 = ph - 1 < 2 * y + 1 ? ph - 1 : 2 * y + 1;
    double sum = D(0x5c8608);
    int32_t count = 0;
    const float* src = F<const float*>(t, 0x1601 + level * 4);
    for (int32_t yy = y0; yy <= y1; ++yy)
        for (int32_t xx = x0; xx <= x1; ++xx) {
            sum += src[(pw + 1) * yy + xx];
            ++count;
        }
    F<float*>(t, 0x1605 + level * 4)[((w >> level) + 1) * y + x] = (float)(sum / count);
}
RT3_REPLACE(0x533430, Terrain_DownsampleHeight);

// 0x5335a0. Mip level `level` word value (+0x1619 + 4 level) at (x, y) from 7
// level-1 samples (+0x1615 + 4 level) around (2x, 2y), edges mirrored: their sum
// / 5 plus the (2x, 2y-1) sample.
RT3_NOINLINE void __fastcall Terrain_DownsampleWord(uint8_t* t, int, int32_t x, int32_t y, int32_t level) {
    int32_t w = F<int32_t>(t, 0x15d9);
    int32_t pw = w >> (level - 1), ph = F<int32_t>(t, 0x15d5) >> (level - 1);
    int32_t sx = 2 * x, sy = 2 * y;
    if (sx >= pw) sx = pw - 1;
    if (sy >= ph) sy = ph - 1;
    int32_t xr = sx + 1 >= pw ? sx - 1 : sx + 1;
    int32_t yd = sy + 1 < ph ? sy + 1 : sy - 1;
    int32_t xl = sx - 1 > 0 ? sx - 1 : sx + 1;
    int32_t yu = sy - 1 > 0 ? sy - 1 : sy + 1;
    const uint16_t* src = F<const uint16_t*>(t, 0x1615 + level * 4);
    int32_t s = pw + 1;
    auto at = [&](int32_t i, int32_t j) { return (int32_t)src[s * j + i]; };
    int32_t top = at(sx, yu);
    int32_t sum = at(sx, sy) + at(xr, sy) + at(xr, yd) + at(sx, yd) + at(xl, sy) + at(xl, yu) + top;
    F<uint16_t*>(t, 0x1619 + level * 4)[((w >> level) + 1) * y + x] = (uint16_t)(sum / 5 + top);
}
RT3_REPLACE(0x5335a0, Terrain_DownsampleWord);

// 0x5336e0. Allocates the terrain's 4 mip levels once (+0x15d1): per level floats
// (+0x1609), 12-byte normals (+0x15f5) and words (+0x161d), zeroed; fatal 0xa4.
RT3_NOINLINE void __fastcall Terrain_AllocMips(uint8_t* t) {
    if (F<int32_t>(t, 0x15d1)) return;
    for (int32_t level = 1; level < 5; ++level) {
        if (F<int32_t>(t, 0x15d1)) continue;
        int32_t n = ((F<int32_t>(t, 0x15d9) >> level) + 1) * ((F<int32_t>(t, 0x15d5) >> level) + 1);
        uint8_t* slot = t + 0x15f5 + (level - 1) * 4;
        F<void*>(slot, 0x14) = Malloc(n * 4);
        F<void*>(slot, 0) = Malloc(n * 12);
        F<void*>(slot, 0x28) = Malloc(n * 2);
        if (!F<void*>(slot, 0x14) || !F<void*>(slot, 0) || !F<void*>(slot, 0x28)) Fatal_Exit(GetString(0xa4));
        memset(F<void*>(slot, 0x14), 0, n * 4);
        memset(F<void*>(slot, 0), 0, n * 12);
        memset(F<void*>(slot, 0x28), 0, n * 2);
    }
    F<int32_t>(t, 0x15d1) = 1;
}
RT3_REPLACE(0x5336e0, Terrain_AllocMips);

// 0x5337f0. Rebuilds the terrain's four coarser levels from scratch, pumping the
// progress callback (0x6d4014) per column.
RT3_NOINLINE void __fastcall Terrain_RebuildMips(uint8_t* t) {
    Terrain_AllocMips(t);
    if (auto cb = hook::Global<void(*)()>(0x6d4014)) cb();
    for (int32_t level = 1; level < 5; ++level)
        for (int32_t x = 0; x <= F<int32_t>(t, 0x15d9) >> level; ++x) {
            if (auto cb = hook::Global<void(*)()>(0x6d4014)) cb();
            for (int32_t y = 0; y <= F<int32_t>(t, 0x15d5) >> level; ++y) MipCell(t, x, y, level);
        }
}
RT3_REPLACE(0x5337f0, Terrain_RebuildMips);

// 0x533890. Rebuilds the coarser levels over the vertex rectangle (x0, y0)-(x1, y1),
// clipped to the terrain.
RT3_NOINLINE void __fastcall Terrain_RebuildMipsRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (F<int32_t>(t, 0x15d9) - 1 < x1) x1 = F<int32_t>(t, 0x15d9) - 1;
    if (F<int32_t>(t, 0x15d5) - 1 < y1) y1 = F<int32_t>(t, 0x15d5) - 1;
    for (int32_t level = 1; level < 5; ++level)
        for (int32_t y = y0 >> level; y <= y1 >> level; ++y)
            for (int32_t x = x0 >> level; x <= x1 >> level; ++x) MipCell(t, x, y, level);
}
RT3_REPLACE(0x533890, Terrain_RebuildMipsRect);

RT3_NOINLINE int32_t __fastcall Terrain_Rows(uint8_t* r) { return F<int32_t>(r, 0x15d9); }
RT3_REPLACE(0x533970, Terrain_Rows);

RT3_NOINLINE int32_t __fastcall Terrain_Cols(uint8_t* r) { return F<int32_t>(r, 0x15d5); }
RT3_REPLACE(0x533980, Terrain_Cols);

void __fastcall Terrain_Set15e1(uint8_t* r, int, uint32_t v) { F<uint32_t>(r, 0x15e1) = v; }
RT3_REPLACE(0x533990, Terrain_Set15e1);

// 0x5339a0. Plane through three points: normal (p1 - p0) x (p2 - p0) into
// *nx / *ny / *nz and d = -(n . p0) into *d. The second edge's x and y are
// rounded to float (stored back into the argument slots); nz is used unrounded for d.
RT3_NOINLINE void __fastcall Plane_From3Points(float* nx, float* ny, float x0, float y0, float z0, float x1, float y1, float z1,
                                  float x2, float y2, float z2, float* nz, float* d) {
    double a = (double)x1 - x0, b = (double)y1 - y0, c = (double)z1 - z0;
    float e1 = (float)((double)x2 - x0);
    float e2 = (float)((double)y2 - y0);
    double f = (double)z2 - z0;
    *nx = (float)(b * f - c * e2);
    *ny = (float)(c * e1 - a * f);
    double n3 = a * e2 - b * e1;
    *nz = (float)n3;
    *d = (float)-((double)x0 * *nx + (double)y0 * *ny + n3 * z0);
}
RT3_REPLACE(0x5339a0, Plane_From3Points);

// 0x533a30. Height (y) at (qx, qz) of the plane through the three points:
// -(d + nx qx + nz qz) * (1 / ny), left unrounded in ST0.
RT3_NOINLINE double __stdcall Plane_HeightAt(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2,
                                float z2, float qx, float qz) {
    double a = (double)x1 - x0, b = (double)y1 - y0, c = (double)z1 - z0;
    float e1 = (float)((double)x2 - x0);
    float e2 = (float)((double)y2 - y0);
    double f = (double)z2 - z0;
    float nx = (float)(b * f - c * e2);
    float ny = (float)(c * e1 - a * f);
    double nz = a * e2 - b * e1;
    double dd = -((double)nx * x0 + (double)ny * y0 + nz * z0);
    double u = dd + (double)nx * qx + nz * qz;
    return -(u * ((double)*(const float*)0x5c85b4 / ny));
}
RT3_REPLACE(0x533a30, Plane_HeightAt);

RT3_NOINLINE uint32_t __fastcall Terrain_Get165d(uint8_t* r) { return F<uint32_t>(r, 0x165d); }
RT3_REPLACE(0x533ae0, Terrain_Get165d);

uint32_t __fastcall Terrain_Get1661(uint8_t* r) { return F<uint32_t>(r, 0x1661); }
RT3_REPLACE(0x533af0, Terrain_Get1661);

uint32_t __fastcall Terrain_Get1665(uint8_t* r) { return F<uint32_t>(r, 0x1665); }
RT3_REPLACE(0x533b00, Terrain_Get1665);

RT3_NOINLINE void* __fastcall Terrain_HeightsSlot(uint8_t* r) { return r + 0x1605; }
RT3_REPLACE(0x533b10, Terrain_HeightsSlot);

uint32_t __fastcall Terrain_Get15e9(uint8_t* r) { return F<uint32_t>(r, 0x15e9); }
RT3_REPLACE(0x533b20, Terrain_Get15e9);

uint32_t __fastcall Terrain_Get15ed(uint8_t* r) { return F<uint32_t>(r, 0x15ed); }
RT3_REPLACE(0x533b30, Terrain_Get15ed);

uint32_t __fastcall Terrain_Layer(uint8_t* r, int, uint8_t i) { return F<uint32_t>(r, 0x1631 + i * 4); }
RT3_REPLACE(0x533b40, Terrain_Layer);

uint32_t __fastcall Terrain_Get1655(uint8_t* r) { return F<uint32_t>(r, 0x1655); }
RT3_REPLACE(0x533b50, Terrain_Get1655);

uint32_t __fastcall Terrain_Get1659(uint8_t* r) { return F<uint32_t>(r, 0x1659); }
RT3_REPLACE(0x533b60, Terrain_Get1659);

RT3_NOINLINE uint32_t __fastcall Terrain_Get15e5(uint8_t* r) { return F<uint32_t>(r, 0x15e5); }
RT3_REPLACE(0x533b70, Terrain_Get15e5);

RT3_NOINLINE uint32_t __fastcall Terrain_Get1619(uint8_t* r, int, int8_t i) { return F<uint32_t>(r, 0x1619 + i * 4); }
RT3_REPLACE(0x533b80, Terrain_Get1619);

void* __fastcall Terrain_Slot15f1(uint8_t* r) { return r + 0x15f1; }
RT3_REPLACE(0x533b90, Terrain_Slot15f1);

PtrList* __fastcall Terrain_Cell1671(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1671, x, y); }
RT3_REPLACE(0x533ba0, Terrain_Cell1671);

PtrListNode* __fastcall Terrain_AddGlobal(uint8_t* r, int, void* v) { return PtrList_Append(F<PtrList*>(r, 0x166d), 0, v); }
RT3_REPLACE(0x533bc0, Terrain_AddGlobal);

void __fastcall Terrain_RemoveGlobal(uint8_t* r, int, PtrListNode* n) { PtrList_RemoveNode(F<PtrList*>(r, 0x166d), 0, n); }
RT3_REPLACE(0x533bd0, Terrain_RemoveGlobal);

PtrList* __fastcall Terrain_GlobalList(uint8_t* r) { return F<PtrList*>(r, 0x166d); }
RT3_REPLACE(0x533be0, Terrain_GlobalList);

PtrListNode* __fastcall Terrain_Add1671(uint8_t* r, int, void* v, int32_t x, int32_t y) {
    return PtrList_Append(Cell(r, 0x1671, x, y), 0, v);
}
RT3_REPLACE(0x533bf0, Terrain_Add1671);

void __fastcall Terrain_Remove1671(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y) {
    PtrList_RemoveNode(Cell(r, 0x1671, x, y), 0, n);
}
RT3_REPLACE(0x533c20, Terrain_Remove1671);

PtrList* __fastcall Terrain_Cell1679(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1679, x, y); }
RT3_REPLACE(0x533c50, Terrain_Cell1679);

PtrListNode* __fastcall Terrain_Add1679(uint8_t* r, int, void* v, int32_t x, int32_t y) {
    return PtrList_Append(Cell(r, 0x1679, x, y), 0, v);
}
RT3_REPLACE(0x533c70, Terrain_Add1679);

void __fastcall Terrain_Remove1679(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y) {
    PtrList_RemoveNode(Cell(r, 0x1679, x, y), 0, n);
}
RT3_REPLACE(0x533ca0, Terrain_Remove1679);

PtrList* __fastcall Terrain_Cell167d(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x167d, x, y); }
RT3_REPLACE(0x533cd0, Terrain_Cell167d);

// 0x533cf0. Frees every item of the +0x167d block lists and clears them.
void __fastcall Terrain_FreeCell167d(uint8_t* r) {
    if (!F<void*>(r, 0x167d)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) <= 0) return;
    for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
        if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
        for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
            PtrList* l = Cell(r, 0x167d, i, j);
            int32_t n = F<int32_t>(l, 0xc);
            if (n <= 0) continue;
            PtrList_Rewind(l);
            void* p = PtrList_Next(l);
            do {
                Free(p);
                p = PtrList_Next(l);
            } while (--n);
            PtrList_Clear(l);
        }
    }
}
RT3_REPLACE(0x533cf0, Terrain_FreeCell167d);

PtrList* __fastcall Terrain_Cell1675(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1675, x, y); }
RT3_REPLACE(0x533db0, Terrain_Cell1675);

PtrListNode* __fastcall Terrain_Add1675(uint8_t* r, int, void* v, int32_t x, int32_t y) {
    return PtrList_Append(Cell(r, 0x1675, x, y), 0, v);
}
RT3_REPLACE(0x533dd0, Terrain_Add1675);

void __fastcall Terrain_Remove1675(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y) {
    PtrList_RemoveNode(Cell(r, 0x1675, x, y), 0, n);
}
RT3_REPLACE(0x533e00, Terrain_Remove1675);

PtrList* __fastcall Terrain_Cell1681(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1681, x, y); }
RT3_REPLACE(0x533e30, Terrain_Cell1681);

PtrList* __fastcall Terrain_Cell1685(uint8_t* r, int, int32_t x, int32_t y) { return Cell(r, 0x1685, x, y); }
RT3_REPLACE(0x533e50, Terrain_Cell1685);

// 0x533e70. Refreshes the point grids of the blocks covering cells (x0, y0)-(x1, y1).
RT3_NOINLINE void __fastcall Terrain_RefreshPointGrids(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    x0 = x0 < 0 ? 0 : x0;
    y0 = y0 < 0 ? 0 : y0;
    if (F<int32_t>(t, 0x15d9) - 2 < x1) x1 = F<int32_t>(t, 0x15d9) - 2;
    if (F<int32_t>(t, 0x15d5) - 2 < y1) y1 = F<int32_t>(t, 0x15d5) - 2;
    y1 >>= 4;
    x0 >>= 4;
    x1 >>= 4;
    y0 >>= 4;
    for (int32_t by = y0; by <= y1; ++by)
        for (int32_t bx = x0; bx <= x1; ++bx)
            if (uint8_t* g = PointGrid(t, bx, by)) Grid_RefreshPoints(g, 0, t);
}
RT3_REPLACE(0x533e70, Terrain_RefreshPointGrids);

void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v) {
    if (uint8_t* g = *GridSlot(t, x, y)) BlockGrid_SetByte(g, 0, x % 16, y % 16, v);
}
RT3_REPLACE(0x533f20, Terrain_SetCellByte);

uint32_t __fastcall Terrain_CellByte(uint8_t* t, int, int32_t x, int32_t y) {
    uint8_t* g = *GridSlot(t, x, y);
    return g ? BlockGrid_Byte(g, 0, x % 16, y % 16) : 0;
}
RT3_REPLACE(0x533f80, Terrain_CellByte);

RT3_NOINLINE void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v) {
    if (uint8_t* g = FlagGrid(t, x, y)) BlockGrid_Set(g, 0, (int32_t)t, x % 16, y % 16, v);
}
RT3_REPLACE(0x533fe0, Terrain_SetFlag);

uint8_t __fastcall Terrain_GetFlag(uint8_t* t, int, int32_t x, int32_t y) {
    uint8_t* g = FlagGrid(t, x, y);
    return g ? BlockGrid_Get(g, 0, x % 16, y % 16) : 0;
}
RT3_REPLACE(0x534040, Terrain_GetFlag);

RT3_NOINLINE uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y) {
    uint8_t* g = *GridSlot(t, x, y);
    return g ? BlockGrid_Count(g, 0, x % 16, y % 16) : 0;
}
RT3_REPLACE(0x5340a0, Terrain_CellCount);

void __fastcall Terrain_SetCellCount(uint8_t* t, int, int32_t x, int32_t y, uint8_t n) {
    if (uint8_t* g = *GridSlot(t, x, y)) BlockGrid_SetCount(g, 0, x % 16, y % 16, n);
}
RT3_REPLACE(0x534100, Terrain_SetCellCount);

// 0x534160. Adds up to 6 points of `kind` at cell (x, z), creating the block's point
// grid (0x54 bytes, origin = block x 0x5c0) on demand.
RT3_NOINLINE void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count) {
    int32_t bx = x >> 4, bz = z >> 4;
    if (!PointGrid(t, bx, bz)) {
        uint8_t* g = (uint8_t*)ExeNew(0x54);
        if (g) g = BlockGrid_Init(g, 0, (float)(bx * 0x5c0), (float)(bz * 0x5c0));
        PointGrid(t, bx, bz) = g;
    }
    int32_t lx = x % 16, lz = z % 16;
    int32_t n = count > 6 ? 6 : count;
    for (; n > 0; --n) Grid_AddPoint(PointGrid(t, bx, bz), 0, t, lx, lz, kind);
}
RT3_REPLACE(0x534160, Terrain_AddPoints);

// 0x534270. Decrements cell (x, y)'s counter up to n (max 6) times; once its grid
// is empty the grid is freed.
RT3_NOINLINE void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n) {
    if (!*GridSlot(t, x, y)) return;
    int32_t cx = x % 16, cy = y % 16;
    if (n > 6) n = 6;
    for (int32_t i = 0; i < n; ++i) {
        if (BlockGrid_Dec(*GridSlot(t, x, y), 0, cx, cy)) continue;
        if (uint8_t* g = *GridSlot(t, x, y)) {
            BlockGrid_Free(g);
            ((void(__cdecl*)(void*))0x53b080)(g);
        }
        *GridSlot(t, x, y) = nullptr;
        return;
    }
}
RT3_REPLACE(0x534270, Terrain_DecCellCount);

// 0x534380 (vtable). Grid cell (col, row) of item `slot`: x = col x 0x5c +
// slot x 13 and y = row x 0x5c + slot x 13; with outV the slot's value
// (+0x16c2, 9-byte entries) and *outFlag = 0. Returns 1.
int32_t __fastcall Grid_SlotPos(uint8_t* o, int, int32_t col, int32_t row, int32_t slot, int32_t, float* outX, float* outY, uint32_t* outV, uint8_t* outFlag) {
    const double off = (double)(slot * 0xd);
    *outX = (float)((double)(col * 0x5c) + off);
    *outY = (float)((double)(row * 0x5c) + off);
    if (outV) {
        *outV = *(const uint32_t*)(o + slot * 9 + 0x16c2);
        *outFlag = 0;
    }
    return 1;
}
RT3_REPLACE(0x534380, Grid_SlotPos);

// 0x5343f0. Appends an entry (9 bytes, first dword = v) to the table +0x1695
// (count +0x1691, max 300); returns the new count or -1 when full.
RT3_NOINLINE int32_t __fastcall Terrain_AddMarker(uint8_t* r, int, uint32_t v, int32_t, int32_t) {
    int32_t n = F<int32_t>(r, 0x1691);
    if (n >= 0x12c) return -1;
    F<uint32_t>(r, 0x1695 + n * 9) = v;
    return ++F<int32_t>(r, 0x1691);
}
RT3_REPLACE(0x5343f0, Terrain_AddMarker);

// 0x534430. Deletes the marker objects of the table +0x1695 (9-byte entries,
// count +0x1691) and empties it.
RT3_NOINLINE void __fastcall Terrain_DeleteMarkers(uint8_t* r) {
    for (int32_t i = 0; i < F<int32_t>(r, 0x1691); ++i) {
        uint8_t* o = F<uint8_t*>(r, 0x1695 + i * 9);
        if (o) {
            SceneObj_Destructor(o);
            ((void(__cdecl*)(void*))0x53b080)(o);
        }
        F<uint8_t*>(r, 0x1695 + i * 9) = nullptr;
    }
    F<int32_t>(r, 0x1691) = 0;
}
RT3_REPLACE(0x534430, Terrain_DeleteMarkers);

// 0x534490. Terrain height at world (x, z): the cell (92 units) is clamped to
// the grid and split along its diagonal into two triangles (heights +0x1605).
RT3_NOINLINE double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z) {
    int32_t ix = CellOf(x);
    int32_t iz = CellOf(z);
    float xs = x;
    if (ix < 0) {
        ix = 0;
        xs = 0.0f;
    } else if (ix + 1 >= F<int32_t>(r, 0x15d9)) {
        ix = F<int32_t>(r, 0x15d9) - 2;
        xs = (float)(ix * 0x5c);
    }
    double zv;
    if (iz < 0) {
        iz = 0;
        zv = *(const float*)0x5c8598;
    } else if (iz + 1 >= F<int32_t>(r, 0x15d5)) {
        iz = F<int32_t>(r, 0x15d5) - 2;
        zv = (double)(iz * 0x5c);
    } else {
        zv = z;
    }
    double dx = (double)xs - (double)(ix * 0x5c);
    float fx = (float)dx;
    float fz = (float)(zv - (double)(iz * 0x5c));
    int32_t stride = F<int32_t>(r, 0x15d9) + 1;
    const float* h = F<float*>(r, 0x1605) + (stride * iz + ix);
    if (dx > fz)
        return Plane_HeightAt(0.0f, h[0], 0.0f, 92.0f, h[1], 0.0f, 92.0f, h[stride + 1], 92.0f, fx, fz);
    return Plane_HeightAt(0.0f, h[0], 0.0f, 0.0f, h[stride], 92.0f, 92.0f, h[stride + 1], 92.0f, fx, fz);
}
RT3_REPLACE(0x534490, Terrain_HeightAt);

// 0x5345c0. Surface normal of the terrain triangle under world point (x, z) (cells
// of 92, clamped to the map), from the cell's lower or upper triangle; returns out.
RT3_NOINLINE float* __fastcall Terrain_NormalAt(uint8_t* t, int, float* out, float x, float z) {
    int32_t ix, iz;
    __asm {
        fld x
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov ix, eax
        fld z
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov iz, eax
    }
    if (ix < 0) {
        ix = 0;
        x = 0.0f;
    } else if (ix + 1 >= F<int32_t>(t, 0x15d9)) {
        ix = F<int32_t>(t, 0x15d9) - 2;
        x = (float)(ix * 0x5c);
    }
    double zz = z;
    if (iz < 0) {
        iz = 0;
        zz = Fl(0x5c8598);
    } else if (iz + 1 >= F<int32_t>(t, 0x15d5)) {
        iz = F<int32_t>(t, 0x15d5) - 2;
        zz = (double)(iz * 0x5c);
    }
    int32_t w1 = F<int32_t>(t, 0x15d9) + 1;
    const float* h = F<const float*>(t, 0x1605) + w1 * iz + ix;
    double fx = (double)x - ix * 0x5c;
    double fz = zz - iz * 0x5c;
    float nx, ny, nz, d;
    if (fz < fx)
        Plane_From3Points(&nx, &ny, 0.0f, h[0], 0.0f, 92.0f, h[1], 0.0f, 92.0f, h[w1 + 1], 92.0f, &nz, &d);
    else
        Plane_From3Points(&nx, &ny, 0.0f, h[0], 0.0f, 0.0f, h[w1], 92.0f, 92.0f, h[w1 + 1], 92.0f, &nz, &d);
    out[0] = nx;
    out[1] = ny;
    out[2] = nz;
    return out;
}
RT3_REPLACE(0x5345c0, Terrain_NormalAt);

// 0x534730. Writes the part (x0, y0)-(x1, y1) of the extracted texture area
// (clamped to the map texture) back into the tiles and re-uploads them.
// Quirk: the source index is taken at the absolute position (origin
// included) with the area's row width.
RT3_NOINLINE void __fastcall Terrain_PasteTexture(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    if (!t[0x1588]) return;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (F<int32_t>(t, 0x157c) * 4 < x1) x1 = F<int32_t>(t, 0x157c) * 4;
    if (F<int32_t>(t, 0x1580) * 4 < y1) y1 = F<int32_t>(t, 0x1580) * 4;
    if (x0 >= x1 || y0 >= y1) return;
    uint32_t* cache[0xff] = {};
    int32_t ax = F<int32_t>(t, 0x1589), w = F<int32_t>(t, 0x1591) - ax;
    int32_t ay = F<int32_t>(t, 0x158d);
    int32_t rowStart = ay + y0;
    int32_t rowEnd = F<int32_t>(t, 0x1595) < ay + y1 ? F<int32_t>(t, 0x1595) : ay + y1;
    int32_t colStart = ax + x0;
    int32_t colEnd = ax + w < ax + x1 ? ax + w : ax + x1;
    const uint8_t* src = (const uint8_t*)F<uint32_t*>(t, 0x1584) + (rowStart * w + colStart) * 4;
    for (int32_t row = rowStart; row < rowEnd; ++row, src += w * 4) {
        int32_t q = row / F<int32_t>(t, 0x1580);
        int32_t base = F<int32_t>(t, 0x157c) * (row % F<int32_t>(t, 0x1580));
        const uint32_t* s = (const uint32_t*)src;
        for (int32_t col = colStart; col < colEnd; ++col) TerrainPutTexel(t, cache, col, q, base, *s++);
    }
    TerrainCommitCache(t, cache);
}
RT3_REPLACE(0x534730, Terrain_PasteTexture);

uint32_t __fastcall Render_Get1588(uint8_t* r) { return r[0x1588]; }
RT3_REPLACE(0x534910, Render_Get1588);

uint32_t __fastcall Render_Get1584(uint8_t* r) { return F<uint32_t>(r, 0x1584); }
RT3_REPLACE(0x534920, Render_Get1584);

// 0x534930. Once (+0x1588): copies the terrain texture area (x0, y0)-(x1, y1) (map
// fractions; tiles +8 of +0x157c x +0x1580 pixels, 4 per row) into a new pixel
// buffer (+0x1584, origin +0x1589/+0x158d, end +0x1591/+0x1595); 0 if done before.
// Quirk: x0 scales by the tile width, the others by the tile height.
RT3_NOINLINE uint32_t* __fastcall Terrain_ExtractTexture(uint8_t* t, int, float x0, float y0, float x1, float y1, int32_t* outW,
                                            int32_t* outH) {
    if (t[0x1588]) return nullptr;
    uint32_t* cache[0xff] = {};
    int32_t tw = F<int32_t>(t, 0x157c) * 4, th = F<int32_t>(t, 0x1580) * 4;
    t[0x1588] = 1;
    int32_t a = (int32_t)((double)tw * x0);
    F<int32_t>(t, 0x1589) = a;
    int32_t b = (int32_t)((double)y0 * th);
    F<int32_t>(t, 0x158d) = b;
    int32_t c = (int32_t)((double)x1 * th);
    F<int32_t>(t, 0x1591) = c;
    int32_t d = (int32_t)((double)th * y1);
    F<int32_t>(t, 0x1595) = d;
    int32_t w = c - a, h = d - b;
    uint32_t* buf = (uint32_t*)Malloc(h * w * 4);
    F<uint32_t*>(t, 0x1584) = buf;
    uint32_t* dst = buf;
    for (int32_t row = F<int32_t>(t, 0x158d); row < F<int32_t>(t, 0x158d) + h; ++row) {
        int32_t q = row / F<int32_t>(t, 0x1580);
        int32_t base = F<int32_t>(t, 0x157c) * (row % F<int32_t>(t, 0x1580));
        for (int32_t col = F<int32_t>(t, 0x1589); col < F<int32_t>(t, 0x1589) + w; ++col) {
            int32_t idx = col / F<int32_t>(t, 0x157c) + q * 4 + 1;
            if (!cache[idx]) cache[idx] = Texture_Pixels32(F<uint8_t*>(t, 8 + idx * 4), 0, nullptr, nullptr, 0);
            *dst++ = cache[idx][col % F<int32_t>(t, 0x157c) + base];
        }
    }
    if (outW) *outW = w;
    if (outH) *outH = h;
    return F<uint32_t*>(t, 0x1584);
}
RT3_REPLACE(0x534930, Terrain_ExtractTexture);

 // namespace

// 0x534af0. Unless `discard`, writes the extracted texture area (+0x1584)
// back into the tiles and re-uploads them; then frees it (+0x1588 cleared).
RT3_NOINLINE void __fastcall Terrain_CommitTexture(uint8_t* t, int, uint8_t discard) {
    if (!t[0x1588]) return;
    if (!discard) {
        uint32_t* cache[0xff] = {};
        int32_t h = F<int32_t>(t, 0x1595) - F<int32_t>(t, 0x158d);
        int32_t w = F<int32_t>(t, 0x1591) - F<int32_t>(t, 0x1589);
        const uint32_t* src = F<uint32_t*>(t, 0x1584);
        for (int32_t row = F<int32_t>(t, 0x158d); row < F<int32_t>(t, 0x158d) + h; ++row) {
            int32_t q = row / F<int32_t>(t, 0x1580);
            int32_t base = F<int32_t>(t, 0x157c) * (row % F<int32_t>(t, 0x1580));
            for (int32_t col = F<int32_t>(t, 0x1589); col < F<int32_t>(t, 0x1589) + w; ++col) TerrainPutTexel(t, cache, col, q, base, *src++);
        }
        TerrainCommitCache(t, cache);
    }
    void* buf = F<void*>(t, 0x1584);
    t[0x1588] = 0;
    ((void(__cdecl*)(void*))0x5a1145)(buf);
    F<void*>(t, 0x1584) = nullptr;
}
RT3_REPLACE(0x534af0, Terrain_CommitTexture);

void __fastcall Render_GetSize4(uint8_t* r, int, int32_t* w, int32_t* h) {
    *w = F<int32_t>(r, 0x157c) << 2;
    *h = F<int32_t>(r, 0x1580) << 2;
}
RT3_REPLACE(0x534c50, Render_GetSize4);

// 0x534c80. Saves terrain t's whole tile texture (4 x 4 tiles, flipped to
// bottom-up rows) to `path`: mode 0 as a TGA, 1 as a JPEG of quality
// `quality`.
RT3_NOINLINE void __fastcall Terrain_SaveTexture(uint8_t* t, int, const char* path, int32_t mode, int32_t quality) {
    int32_t tw = F<int32_t>(t, 0x157c) * 4, th = F<int32_t>(t, 0x1580) * 4;
    uint32_t* buf = (uint32_t*)((void*(__cdecl*)(uint32_t))0x5a125d)((uint32_t)(th * tw * 4));
    uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0xec);
    uint8_t* tex = p ? Texture_InitMemory(p, 0, buf, tw, th, 1) : nullptr;
    for (int32_t by = 0; by < 4; ++by)
        for (int32_t bx = 0; bx < 4; ++bx) {
            const uint32_t* src = Texture_Pixels32(F<uint8_t*>(t, 0xc + (by * 4 + bx) * 4), 0, nullptr, nullptr, 0);
            for (int32_t row = 0; row < F<int32_t>(t, 0x1580); ++row) {
                uint32_t* dst = buf + (th - F<int32_t>(t, 0x1580) * by - row - 1) * tw + F<int32_t>(t, 0x157c) * bx;
                for (int32_t col = 0; col < F<int32_t>(t, 0x157c); ++col) *dst++ = src[F<int32_t>(t, 0x157c) * row + col];
            }
        }
    if (mode == 0)
        Texture_SaveTga(tex, 0, path, 1);
    else if (mode == 1)
        Texture_AppendJpeg(tex, 0, path, 1, quality);
    if (!tex) return;
    Texture_Destroy(tex);
    ((void(__cdecl*)(void*))0x53b080)(tex);
}
RT3_REPLACE(0x534c80, Terrain_SaveTexture);

// 0x534e00 (vtable stub).
void __fastcall Nop534e00(void*, int, int32_t, int32_t, int32_t, int32_t) { __asm nop }
RT3_REPLACE(0x534e00, Nop534e00);

RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or3to5(uint8_t* r, int, int32_t x, int32_t y) {
    int32_t k = Rec(r, x, y)[0] & 7;
    return k == 1 || (k > 2 && k <= 5) ? 1 : 0;
}
RT3_REPLACE(0x534e10, Terrain_KindIs1Or3to5);

RT3_NOINLINE int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y) {
    uint8_t k = Rec(r, x, y)[0] & 7;
    return k == 1 || k == 4 ? 1 : 0;
}
RT3_REPLACE(0x534e50, Terrain_KindIs1Or4);

RT3_NOINLINE uint32_t __fastcall Terrain_RecBit9(uint8_t* r, int, int32_t x, int32_t y) {
    return (*(const uint16_t*)Rec(r, x, y) >> 9) & 1;
}
RT3_REPLACE(0x534e90, Terrain_RecBit9);

RT3_NOINLINE int32_t __fastcall Terrain_KindIs2Or4Or5(uint8_t* r, int, int32_t x, int32_t y) {
    uint8_t k = Rec(r, x, y)[0] & 7;
    return k == 2 || k == 5 || k == 4 ? 1 : 0;
}
RT3_REPLACE(0x534ec0, Terrain_KindIs2Or4Or5);

RT3_NOINLINE int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y) {
    uint8_t k = Rec(r, x, y)[0] & 7;
    return k == 3 || k == 5 ? 1 : 0;
}
RT3_REPLACE(0x534f00, Terrain_KindIs3Or5);

RT3_NOINLINE uint8_t* __fastcall Terrain_Rec(uint8_t* r, int, int32_t x, int32_t y) { return Rec(r, x, y); }
RT3_REPLACE(0x534f40, Terrain_Rec);

// 0x534f60. Constructs the terrain renderer (vtable 0x5dd25c).
RT3_NOINLINE uint8_t* __fastcall Terrain_Construct(uint8_t* t) {
    F<uint32_t>(t, 0) = 0x5dd25c;
    F<int32_t>(t, 0x154c) = 0;
    Terrain_Init(t);
    return t;
}
RT3_REPLACE(0x534f60, Terrain_Construct);

// 0x534f80. Destroys the terrain colour map texture (+0x478).
void __fastcall Render_DestroyColourMap(uint8_t* r) {
    uint8_t* t = F<uint8_t*>(r, 0x478);
    if (!t) return;
    Texture_Destroy(t);
    ExeDelete(t);
    F<uint8_t*>(r, 0x478) = nullptr;
    r[0x159b] = 0;
}
RT3_REPLACE(0x534f80, Render_DestroyColourMap);

// 0x534fc0. (Re)creates the current overview render target of size x size.
RT3_NOINLINE void __fastcall Render_CreateOverviewTarget(uint8_t* r, int, int32_t size) {
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

// 0x535070. Clears texture slot table t; with render targets (0x6d4024
// +0x11422e) slot 0 becomes a 256 x 256 target (none: +0x1146eb cleared),
// otherwise the "Water" texture. Returns the count / the slot.
RT3_NOINLINE int32_t __fastcall TextureSlots_Reset(uint8_t* t) {
    memset(t + 8, 0, 0x400);
    F<int32_t>(t, 4) = 0;
    if (!hook::Global<uint8_t*>(0x6d4024)[0x11422e]) return TextureSlots_Add(t, 0, (const char*)0x5dd26c);
    uint8_t* tex = (uint8_t*)ExeNew(0xec);
    if (tex) tex = Texture_ConstructDefault(tex);
    F<uint8_t*>(t, 8) = tex;
    if (!Texture_CreateTarget(tex, 0, 0x100, 0x100, F<void*>(hook::Global<uint8_t*>(0x6d4024), 8)))
        hook::Global<uint8_t*>(0x6d4024)[0x1146eb] = 0;
    return ++F<int32_t>(t, 4);
}
RT3_REPLACE(0x535070, TextureSlots_Reset);

// 0x535100. (Re)builds terrain t's 4 x 4 tile textures (16 slots from +0x155c
// in +8.., found on first use; tile size +0x157c x +0x1580 = the clamped w x
// h / 4) from texture src: rows bottom-up, by a straight copy when the sizes
// match or else by stepping through it; plain 0x96586131 without a source.
// Quirk: the source row is the destination row x (w / source height).
RT3_NOINLINE void __fastcall Terrain_BuildTiles(uint8_t* t, int, int32_t w, int32_t h, uint8_t* src) {
    Render_ClampTextureSize(&w, &h);
    F<int32_t>(t, 0x157c) = w / 4;
    F<int32_t>(t, 0x1580) = h / 4;
    if (F<int32_t>(t, 0x155c) == -1) {
        int32_t k = 0;
        while (k < 0x100 && F<uint8_t*>(t, 8 + k * 4)) ++k;
        F<int32_t>(t, 0x155c) = k < 0x100 ? k : -1;
        F<int32_t>(t, 4) += 0x10;
    } else {
        for (int32_t k = 0; k < 0x10; ++k) {
            if (uint8_t* old = F<uint8_t*>(t, 8 + (F<int32_t>(t, 0x155c) + k) * 4)) {
                Texture_Destroy(old);
                ((void(__cdecl*)(void*))0x53b080)(old);
            }
            F<uint8_t*>(t, 8 + (F<int32_t>(t, 0x155c) + k) * 4) = nullptr;
        }
    }
    uint32_t* pixels = nullptr;
    int32_t sw = 0, sh = 0, fullW = w, fullH = 0;
    if (src) {
        pixels = Texture_Pixels32(src, 0, &sw, &sh, 0);
        fullW = F<int32_t>(t, 0x157c) * 4;
        fullH = F<int32_t>(t, 0x1580) * 4;
    }
    int32_t slot = 0;
    for (int32_t by = 3; by >= 0; --by)
        for (int32_t bx = 0; bx < 4; ++bx) {
            ++hook::Global<int32_t>(0xccb9e8);
            if (hook::Global<void(__cdecl*)()>(0x6d4014)) hook::Global<void(__cdecl*)()>(0x6d4014)();
            int32_t tw = F<int32_t>(t, 0x157c), th = F<int32_t>(t, 0x1580);
            uint32_t* pix = (uint32_t*)((void*(__cdecl*)(uint32_t))0x5a125d)((uint32_t)(tw * th * 4));
            if (src) {
                const uint32_t* base = pixels + ((sh / 4) * by * sw + (sw / 4) * bx);
                for (int32_t row = 0; row < F<int32_t>(t, 0x1580); ++row) {
                    if (sw == fullW && sh == fullH) {
                        memcpy(pix + F<int32_t>(t, 0x157c) * row, base + (F<int32_t>(t, 0x1580) - row - 1) * sw, (size_t)F<int32_t>(t, 0x157c) * 4);
                        continue;
                    }
                    double xs = (double)fullW / sw;
                    uint32_t* d = pix + F<int32_t>(t, 0x157c) * row;
                    int32_t srow = (int32_t)((double)fullH / sh * row);
                    const uint32_t* s = base + srow * sw;
                    if (xs < 1.0) xs = *(const float*)0x5c8598;
                    int32_t step = (int32_t)(xs);
                    int32_t skip = sw / fullW;
                    if (skip < 1) skip = 1;
                    int32_t left = step;
                    for (int32_t col = 0; col < F<int32_t>(t, 0x157c); ++col) {
                        *d++ = *s;
                        if (step) {
                            if (--left <= 0) {
                                ++s;
                                left = step;
                            }
                        } else
                            s += skip;
                    }
                }
            } else
                Mem_Fill32(pix, 0x96586131, (uint32_t)(F<int32_t>(t, 0x157c) * F<int32_t>(t, 0x1580)));
            uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0xec);
            uint8_t* tile = p ? Texture_InitMemory(p, 0, pix, F<int32_t>(t, 0x157c), F<int32_t>(t, 0x1580), 1) : nullptr;
            F<uint8_t*>(t, 8 + (F<int32_t>(t, 0x155c) + slot) * 4) = tile;
            ++slot;
        }
}
RT3_REPLACE(0x535100, Terrain_BuildTiles);

// 0x535430. Builds terrain t's tiles from texture `name` (.tga / .jpg, or
// "%1_A" of it, in the packs; loaded at full size 0x800 and shrunk to the
// clamped maximum), else from the built-in TGA image 0x62bed4 (when larger
// than 3000 bytes; returns 1), else plain (at the maximum size with
// `fullSize`, else 128 x 128). 1 when a texture was used.
RT3_NOINLINE int32_t __fastcall Terrain_LoadTexture(uint8_t* t, int, const char* name, int32_t fullSize) {
    ++hook::Global<int32_t>(0xccb9e8);
    if (hook::Global<void(__cdecl*)()>(0x6d4014)) hook::Global<void(__cdecl*)()>(0x6d4014)();
    ++F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11427e);
    uint8_t* tex = nullptr;
    int32_t w = 0x80, h = 0x80, maxW = 0x4000, maxH = 0x4000;
    Render_ClampTextureSize(&maxW, &maxH);
    int32_t used = 0;
    if (name) {
        char alt[0x100];
        FormatText(alt, (const char*)0x5dd284, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        uint8_t* packs = hook::Global<uint8_t*>(0x6d4020);
        if (NameTable_ContainsName(packs, 0, name, 0, (const char*)0x5dd27c) ||
            NameTable_ContainsName(hook::Global<uint8_t*>(0x6d4020), 0, name, 0, (const char*)0x5dd274) ||
            NameTable_ContainsName(hook::Global<uint8_t*>(0x6d4020), 0, alt, 0, (const char*)0x5dd27c) ||
            NameTable_ContainsName(hook::Global<uint8_t*>(0x6d4020), 0, alt, 0, (const char*)0x5dd274)) {
            uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
            int32_t savedA = F<int32_t>(e, 0x11423b), savedB = F<int32_t>(e, 0x11423f);
            F<int32_t>(e, 0x11423b) = 0x800;
            F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11423f) = 0x800;
            used = 1;
            tex = TextureCache_Get(hook::Global<uint8_t*>(0x6d4020), 0, name, 0, 0, 0);
            F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11423b) = savedA;
            F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11423f) = savedB;
            Texture_ImageSize(tex, 0, &w, &h, 0);
            if (maxW < w || maxH < h) {
                if (w >= maxW) w = maxW;
                if (h >= maxH) h = maxH;
                Texture_Shrink(tex, 0, 0, w, h);
            }
            Texture_Pixels32(tex, 0, &w, &h, 0);
        } else {
            const uint8_t* img = hook::Global<uint8_t*>(0x62bed4);
            if (img && hook::Global<int32_t>(0x62bed8) > 0xbb8) {
                uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0xec);
                uint8_t* mem = p ? Texture_InitMemory(p, 0, (void*)(img + img[0] + 0x12), *(const uint16_t*)(img + 0xc), *(const uint16_t*)(img + 0xe), 0)
                                 : nullptr;
                Terrain_BuildTiles(t, 0, *(const uint16_t*)(img + 0xc), *(const uint16_t*)(img + 0xe), mem);
                --F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11427e);
                return 1;
            }
            if (fullSize) {
                w = maxW;
                h = maxH;
            }
        }
    } else if (fullSize) {
        w = maxW;
        h = maxH;
    }
    Terrain_BuildTiles(t, 0, w, h, tex);
    if (used) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, tex);
    --F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11427e);
    return used;
}
RT3_REPLACE(0x535430, Terrain_LoadTexture);

// 0x5356e0. Loads the water textures into t once: the 32 ocean animation frames
// Ocean101..Ocean132 (first slot +0x1568, count +0x156c, all small, turned into
// bump maps; every third frame counts a skipped stream read 0xccb9e8), then
// Shoreline (+0x1574) and RiverFlow (+0x1578), with the progress hook 0x6d4014.
// QUIRK: with no free slot the original clears byte +5 of t's count (slot -1).
RT3_NOINLINE void __fastcall Water_LoadTextures(uint8_t* t) {
    if (F<int32_t>(t, 0x1568)) return;
    typedef void (*HookFn)();
    if (HookFn f = hook::Global<HookFn>(0x6d4014)) f();
    F<int32_t>(t, 0x1568) = TextureSlots_Add(t, 0, (const char*)0x5dd2b0);
    F<int32_t>(t, 0x156c) = 1;
    for (int32_t i = 0x66; i <= 0x84; ++i) {
        char name[0x12c];
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(name, (const char*)0x5dd2a4, i);
        int32_t slot = TextureSlots_AddInline(t, name);
        F<uint8_t*>(t, 8 + slot * 4)[5] = 0;
        if (HookFn f = hook::Global<HookFn>(0x6d4014)) f();
        ++F<int32_t>(t, 0x156c);
    }
    int32_t first = F<int32_t>(t, 0x1568), end = F<int32_t>(t, 0x156c) + first;
    for (int32_t i = first; i < end; ++i) {
        if (HookFn f = hook::Global<HookFn>(0x6d4014)) f();
        F<uint8_t*>(t, 8 + i * 4)[5] = 0;
        Texture_MakeBumpMaps(F<uint8_t*>(t, 8 + i * 4));
        if (i % 3 == 0) ++hook::Global<int32_t>(0xccb9e8);
    }
    F<int32_t>(t, 0x1574) = TextureSlots_AddInline(t, (const char*)0x5dd298);
    F<int32_t>(t, 0x1578) = TextureSlots_AddInline(t, (const char*)0x5dd28c);
}
RT3_REPLACE(0x5356e0, Water_LoadTextures);

// 0x535890. Loads the rain animation into t once (+0x1560 = slot of
// raindropLOOP01, then raindropLOOP02..32 into free slots, +0x1564 = 32),
// calling the progress hook 0x6d4014 around each.
RT3_NOINLINE void __fastcall Rain_LoadFrames(uint8_t* t) {
    if (F<int32_t>(t, 0x1560)) return;
    typedef void (*HookFn)();
    if (HookFn f = hook::Global<HookFn>(0x6d4014)) f();
    F<int32_t>(t, 0x1560) = TextureSlots_Add(t, 0, (const char*)0x5dd2d0);
    for (int32_t i = 2; i <= 0x20; ++i) {
        char name[0x12c];
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(name, (const char*)0x5dd2bc, i);
        int32_t slot = 0;
        while (slot < 0x100 && F<void*>(t, 8 + slot * 4)) ++slot;
        if (slot < 0x100) {
            F<uint8_t*>(t, 8 + slot * 4) = Texture_Acquire(name);
            ++F<int32_t>(t, 4);
        }
        if (HookFn f = hook::Global<HookFn>(0x6d4014)) f();
    }
    F<int32_t>(t, 0x1564) = 0x20;
}
RT3_REPLACE(0x535890, Rain_LoadFrames);

// 0x535950. Starts sample `name` in the first free one of the 27 sound slots
// (+0x40c, count +0x408); returns the slot or -1 when all are taken.
RT3_NOINLINE int32_t __fastcall Sound_StartInSlot(uint8_t* s, int, const char* name) {
    int32_t i = 0;
    while (F<uint8_t*>(s, 0x40c + i * 4)) {
        if (++i >= 0x1b) return -1;
    }
    uint8_t* ch = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x141);
    if (ch) ch = Channel_Setup(ch, 0, name, s, 0.0f, 0.0f, 0.0f, 0, 1, -1.0f);
    F<uint8_t*>(s, 0x40c + i * 4) = ch;
    ++F<int32_t>(s, 0x408);
    return i;
}
RT3_REPLACE(0x535950, Sound_StartInSlot);

// 0x5359c0. Frees the 27 fixed channels (+0x40c; count +0x408).
RT3_NOINLINE void __fastcall Terrain_FreeFixedChannels(uint8_t* r) {
    for (int k = 0; k < 0x1b; ++k) {
        uint8_t* c = F<uint8_t*>(r, 0x40c + k * 4);
        if (!c) continue;
        Channel_Detach(c);
        ExeDelete(c);
        F<void*>(r, 0x40c + k * 4) = nullptr;
        F<int32_t>(r, 0x408)--;
    }
}
RT3_REPLACE(0x5359c0, Terrain_FreeFixedChannels);

// 0x535a10. Releases every effect in the +0x1675 block lists, then the lists and the grid.
RT3_NOINLINE void __fastcall Terrain_DestroyCell1675(uint8_t* r) {
    if (!F<void*>(r, 0x1675)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                PtrList* l = F<PtrList**>(r, 0x1675)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                int32_t n = F<int32_t>(l, 0xc);
                if (n > 0) {
                    PtrList_Rewind(l);
                    uint8_t* e = (uint8_t*)PtrList_Next(l);
                    do {
                        if (e) {
                            Effect_Release(e);
                            ExeDelete(e);
                        }
                        e = (uint8_t*)PtrList_Next(l);
                    } while (--n);
                }
                l = F<PtrList**>(r, 0x1675)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (l) {
                    PtrList_Destroy(l);
                    ExeDelete(l);
                }
            }
        }
    }
    Free(F<void*>(r, 0x1675));
    F<void*>(r, 0x1675) = nullptr;
}
RT3_REPLACE(0x535a10, Terrain_DestroyCell1675);

// 0x535b40. Deletes the per-16x16-block object lists +0x1671 with all their
// scene objects, then the grid itself.
RT3_NOINLINE void __fastcall Terrain_FreeBlockObjects(uint8_t* r) {
    if (!F<PtrList**>(r, 0x1671)) return;
    for (int32_t x = 0; x < F<int32_t>(r, 0x15d9) >> 4; ++x) {
        for (int32_t y = 0; y < F<int32_t>(r, 0x15d5) >> 4; ++y) {
            PtrList* l = F<PtrList**>(r, 0x1671)[(F<int32_t>(r, 0x15d9) >> 4) * y + x];
            int32_t n = l->count;
            if (n > 0) {
                PtrList_Rewind(l);
                uint8_t* o = (uint8_t*)PtrList_Next(l);
                do {
                    if (o) {
                        SceneObj_Destructor(o);
                        ((void(__cdecl*)(void*))0x53b080)(o);
                    }
                    o = (uint8_t*)PtrList_Next(l);
                } while (--n);
            }
            l = F<PtrList**>(r, 0x1671)[(F<int32_t>(r, 0x15d9) >> 4) * y + x];
            if (l) {
                PtrList_Destroy(l);
                ((void(__cdecl*)(void*))0x53b080)(l);
            }
        }
    }
    ((void(__cdecl*)(void*))0x5a1145)(F<void*>(r, 0x1671));
    F<void*>(r, 0x1671) = nullptr;
}
RT3_REPLACE(0x535b40, Terrain_FreeBlockObjects);

// 0x535c70. Destroys the per-block sound lists (+0x1681) and the grid.
RT3_NOINLINE void __fastcall Terrain_DestroyCell1681(uint8_t* r) {
    if (!F<void*>(r, 0x1681)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                uint8_t* l = F<uint8_t**>(r, 0x1681)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (!l) continue;
                Lists_Destroy(l);
                ((void(__cdecl*)(void*))0x53b080)(l);
                F<uint8_t**>(r, 0x1681)[(F<int32_t>(r, 0x15d9) >> 4) * j + i] = nullptr;
            }
        }
    }
    Free(F<void*>(r, 0x1681));
    F<void*>(r, 0x1681) = nullptr;
}
RT3_REPLACE(0x535c70, Terrain_DestroyCell1681);

// 0x535d30. Destroys the flag grids (+0x1685).
RT3_NOINLINE void __fastcall Terrain_DestroyCell1685(uint8_t* r) {
    if (!F<void*>(r, 0x1685)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                uint8_t* g = F<uint8_t**>(r, 0x1685)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (!g) continue;
                BlockGrid_Free(g);
                ((void(__cdecl*)(void*))0x53b080)(g);
                F<uint8_t**>(r, 0x1685)[(F<int32_t>(r, 0x15d9) >> 4) * j + i] = nullptr;
            }
        }
    }
    Free(F<void*>(r, 0x1685));
    F<void*>(r, 0x1685) = nullptr;
}
RT3_REPLACE(0x535d30, Terrain_DestroyCell1685);

// 0x535df0. Frees every channel in the per-block lists (+0x1679), the lists and the grid.
RT3_NOINLINE void __fastcall Terrain_DestroyCell1679(uint8_t* r) {
    if (!F<void*>(r, 0x1679)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                PtrList* l = F<PtrList**>(r, 0x1679)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                int32_t n = F<int32_t>(l, 0xc);
                if (n > 0) {
                    PtrList_Rewind(l);
                    uint8_t* c = (uint8_t*)PtrList_Next(l);
                    do {
                        if (c) {
                            Channel_Detach(c);
                            ExeDelete(c);
                        }
                        c = (uint8_t*)PtrList_Next(l);
                    } while (--n);
                }
                l = F<PtrList**>(r, 0x1679)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (l) {
                    PtrList_Destroy(l);
                    ExeDelete(l);
                }
            }
        }
    }
    Free(F<void*>(r, 0x1679));
    F<void*>(r, 0x1679) = nullptr;
}
RT3_REPLACE(0x535df0, Terrain_DestroyCell1679);

// 0x535f20. Frees the +0x167d block lists (items, lists and the grid).
RT3_NOINLINE void __fastcall Terrain_DestroyCell167d(uint8_t* r) {
    if (!F<void*>(r, 0x167d)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                PtrList* l = F<PtrList**>(r, 0x167d)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                int32_t n = F<int32_t>(l, 0xc);
                if (n > 0) {
                    PtrList_Rewind(l);
                    void* p = PtrList_Next(l);
                    do {
                        Free(p);
                        p = PtrList_Next(l);
                    } while (--n);
                }
                l = F<PtrList**>(r, 0x167d)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (l) {
                    PtrList_Destroy(l);
                    ((void(__cdecl*)(void*))0x53b080)(l);
                }
            }
        }
    }
    Free(F<void*>(r, 0x167d));
    F<void*>(r, 0x167d) = nullptr;
}
RT3_REPLACE(0x535f20, Terrain_DestroyCell167d);

// 0x536030. Frees the terrain (when its height grids +0x1605 exist): the five
// level grids (+0x15f1 / +0x1605 / +0x1619), the block object lists and cell
// grids, and the remaining buffers.
RT3_NOINLINE void __fastcall Terrain_Free(uint8_t* r) {
    auto crtFree = [](void* p) { ((void(__cdecl*)(void*))0x5a1145)(p); };
    if (!F<void*>(r, 0x1605)) return;
    for (int k = 0; k < 5; ++k) {
        crtFree(F<void*>(r, 0x1605 + k * 4));
        crtFree(F<void*>(r, 0x15f1 + k * 4));
        crtFree(F<void*>(r, 0x1619 + k * 4));
        F<void*>(r, 0x1605 + k * 4) = nullptr;
        F<void*>(r, 0x15f1 + k * 4) = nullptr;
        F<void*>(r, 0x1619 + k * 4) = nullptr;
    }
    Terrain_FreeBlockObjects(r);
    Terrain_DestroyCell1681(r);
    Terrain_DestroyCell1685(r);
    Terrain_DestroyCell1679(r);
    Terrain_DestroyCell1675(r);
    Terrain_DestroyCell167d(r);
    if (F<void*>(r, 0x15e5)) {
        crtFree(F<void*>(r, 0x15e5));
        F<void*>(r, 0x15e5) = nullptr;
    }
    if (F<void*>(r, 0x162d)) crtFree(F<void*>(r, 0x162d));
    for (int k = 0; k < 4; ++k) {
        if (F<void*>(r, 0x1631 + k * 4)) crtFree(F<void*>(r, 0x1631 + k * 4));
        F<void*>(r, 0x1631 + k * 4) = nullptr;
    }
    for (uint32_t off : { 0x1669u, 0x1655u, 0x1659u })
        if (F<void*>(r, off)) crtFree(F<void*>(r, off));
    F<void*>(r, 0x1669) = nullptr;
    F<void*>(r, 0x1659) = nullptr;
    F<void*>(r, 0x1655) = nullptr;
    F<void*>(r, 0x162d) = nullptr;
    if (F<void*>(r, 0x165d)) crtFree(F<void*>(r, 0x165d));
    F<void*>(r, 0x165d) = nullptr;
    if (F<void*>(r, 0x1661)) crtFree(F<void*>(r, 0x1661));
    if (F<void*>(r, 0x1665)) crtFree(F<void*>(r, 0x1665));
    F<void*>(r, 0x1661) = nullptr;
    F<void*>(r, 0x1665) = nullptr;
    crtFree(F<void*>(r, 0x15ed));
    crtFree(F<void*>(r, 0x15e9));
    F<void*>(r, 0x15e9) = nullptr;
    F<void*>(r, 0x15ed) = nullptr;
    F<int32_t>(r, 0x15d1) = 0;
}
RT3_REPLACE(0x536030, Terrain_Free);

// 0x5361d0. Resizes the terrain to w x h cells (vertex stride w + 1) when the
// size changes: frees it and allocates the grids and mip levels again.
RT3_NOINLINE void __fastcall Terrain_Resize(uint8_t* r, int, int32_t w, int32_t h) {
    if (w == F<int32_t>(r, 0x15d9) && h == F<int32_t>(r, 0x15d5)) return;
    Terrain_Free(r);
    F<int32_t>(r, 0x15d9) = w;
    F<int32_t>(r, 0x15d5) = h;
    F<int32_t>(r, 0x15dd) = w + 1;
    Render_AllocGrid(r, 0, 0, 0);
    Terrain_AllocMips(r);
}
RT3_REPLACE(0x5361d0, Terrain_Resize);

// 0x536230. Vertex normal (+0x15f1, 12 bytes per vertex) at (x, y): the normalised
// mean of the surface normals at the vertices around it. Returns 0.
RT3_NOINLINE float __fastcall Terrain_ComputeVertexNormal(uint8_t* t, int, int32_t x, int32_t y) {
    int32_t x0 = x - 1 < 0 ? 0 : x - 1;
    int32_t y0 = y - 1 < 0 ? 0 : y - 1;
    int32_t x1 = F<int32_t>(t, 0x15d9) - 1 < x + 1 ? F<int32_t>(t, 0x15d9) - 1 : x + 1;
    int32_t y1 = F<int32_t>(t, 0x15d5) - 1 < y + 1 ? F<int32_t>(t, 0x15d5) - 1 : y + 1;
    int32_t count = 0;
    float sx = 0.0f, sy = 0.0f, sz = 0.0f;
    if (y0 > y1) return 0.0f;
    for (int32_t yy = y0; yy <= y1; ++yy) {
        float fz = (float)(yy * 0x5c);
        if (x0 > x1) continue;
        count += x1 - x0 + 1;
        for (int32_t xx = x0; xx <= x1; ++xx) {
            float n[3];
            float* r = Terrain_NormalAt(t, 0, n, (float)(xx * 0x5c), fz);
            sx = (float)((double)sx + r[0]);
            sy = (float)((double)sy + r[1]);
            sz = (float)((double)sz + r[2]);
        }
    }
    if (!count) return 0.0f;
    double inv = D(0x5c8770) / count;
    float invf = (float)inv;
    double a = inv * sx, b = (double)invf * sy;
    double c = (double)invf * sz;
    sz = (float)c;
    double len = sqrt(c * sz + b * b + a * a);
    if (Fl(0x5c8598) < len) {
        double k = Fl(0x5c85b4) / len;
        a *= k;
        b *= k;
        sz = (float)(k * sz);
    }
    float* out = (float*)(F<uint8_t*>(t, 0x15f1) + ((F<int32_t>(t, 0x15d9) + 1) * y + x) * 12);
    out[2] = sz;
    out[0] = (float)a;
    out[1] = (float)b;
    return 0.0f;
}
RT3_REPLACE(0x536230, Terrain_ComputeVertexNormal);

void __fastcall Render_SetOverviewSize(uint8_t* r, int, int32_t size) {
    F<int32_t>(r, 0x1548) = size;
    Render_CreateOverviewTarget(r, 0, size);
}
RT3_REPLACE(0x536400, Render_SetOverviewSize);

// 0x536420. Shading value of terrain vertex (x, y) into +0x1619 (u16): the mean
// cross-product length between its normal (+0x15f1) and its 3x3 neighbours', plus
// one, scaled; type-0 cells (+0x1655) get a bonus, raised to a fixed level next to
// other types (not on the border); then the detail byte (+0x162d at level +0x15e1)
// is added and the result scaled by +0x14e0, capped.
RT3_NOINLINE void __fastcall Terrain_ShadeVertex(uint8_t* t, int, int32_t x, int32_t y, int32_t) {
    int32_t w = F<int32_t>(t, 0x15d9);
    int32_t x0 = x - 1 < 0 ? 0 : x - 1;
    int32_t ym1 = y - 1;
    int32_t y0 = ym1 < 0 ? 0 : ym1;
    int32_t x1 = w - 1 >= x + 1 ? x + 1 : w - 1;
    int32_t yp1 = y + 1;
    int32_t y1 = F<int32_t>(t, 0x15d5) - 1 >= yp1 ? yp1 : F<int32_t>(t, 0x15d5) - 1;
    double sum = 0.0;
    int32_t n = 0;
    int32_t idx = (w + 1) * y + x;
    const float* nrm = F<const float*>(t, 0x15f1);
    float cx = nrm[idx * 3], cy = nrm[idx * 3 + 1], cz = nrm[idx * 3 + 2];
    if (y0 > y1) return;
    for (int32_t row = y0; row <= y1; ++row)
        for (int32_t col = x0; col <= x1; ++col) {
            if (col == x && row == y) continue;
            const float* e = nrm + ((w + 1) * row + col) * 3;
            float ex = e[0], ey = e[1], ez = e[2];
            ++n;
            double X = (double)ey * cz - (double)ez * cy;
            double Y = (double)ez * cx - (double)cz * ex;
            double Z = (double)cy * ex - (double)ey * cx;
            double len = sqrt(Z * Z + Y * Y + X * X);
            sum += len * *(const double*)0x5dd2e8;
        }
    if (!n) return;
    double v = (sum + *(const float*)0x5c85b4) * *(const float*)0x5c9aa8 / n;
    if (v < *(const float*)0x5c8598) v = *(const float*)0x5c8598;
    int32_t stride = F<int32_t>(t, 0x15dd);
    const uint8_t* map = F<const uint8_t*>(t, 0x1655);
    const uint8_t* p = map + stride * y + x;
    uint8_t b = *p;
    if (!b) {
        v += *(const float*)0x5c8554;
        if (x > 0 && y > 0 && x < w - 2 && y < F<int32_t>(t, 0x15d5) - 2) {
            const uint8_t* q = map + yp1 * stride + x;
            const uint8_t* r = map + ym1 * stride + x;
            if (b != p[1] || b != q[1] || b != q[0] || b != q[-1] || b != p[-1] || b != r[-1] || b != r[0] || b != r[1])
                v = v + ((double)*(const float*)0x5dd2e4 - v);
        }
    }
    const uint8_t* det = F<const uint8_t*>(t, 0x162d) + ((w >> F<int32_t>(t, 0x15e1)) + 1) * y;
    v = (v + det[x]) * F<float>(t, 0x14e0);
    if (v > *(const float*)0x5dd2e0) v = *(const float*)0x5dd2e0;
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    F<uint16_t*>(t, 0x1619)[idx] = (uint16_t)r;
}
RT3_REPLACE(0x536420, Terrain_ShadeVertex);

 // namespace

// 0x536710. Recomputes the water flow (the per-cell direction bytes +0x1661 /
// +0x1665, scale 125) over cells [x0, x1) x [y0, y1) of terrain t: dry cells
// get none; plain water (kinds 1, 3, not frozen) away from cliffs (4, 5)
// points away from its 2..4 land neighbours unless land lies within 8 steps
// that way (also clearing its 4 edge flags +0x1631..); shore water keeps its
// direction at full length. A second pass replaces each plain water cell's
// direction by the mean of its 7 x 7 neighbourhood (fewer than 5 flowing
// cells, else none), then the touched water grids +0x1681 are rebuilt. The
// progress callback 0x6d4014 runs once per row.
RT3_NOINLINE void __fastcall Terrain_RefreshWaterFlow(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    if (x0 < 1) x0 = 1;
    if (y0 < 1) y0 = 1;
    if (F<int32_t>(t, 0x15d9) - 2 < x1) x1 = F<int32_t>(t, 0x15d9) - 2;
    if (F<int32_t>(t, 0x15d5) - 2 < y1) y1 = F<int32_t>(t, 0x15d5) - 2;
    auto progress = [] {
        if (hook::Global<void(__cdecl*)()>(0x6d4014)) hook::Global<void(__cdecl*)()>(0x6d4014)();
    };
    auto kindAt = [&](int32_t x, int32_t y) { return (uint16_t)(*(uint16_t*)(F<uint8_t*>(t, 0x165d) + (F<int32_t>(t, 0x15dd) * y + x) * 3) & 7); };
    const double k125 = *(const float*)0x5ca128, minLen = *(const double*)0x5c8678;
    for (int32_t y = y0; y < y1; ++y) {
        progress();
        for (int32_t x = x0; x < x1; ++x) {
            int32_t row = F<int32_t>(t, 0x15dd) * y;
            uint8_t* cell = F<uint8_t*>(t, 0x165d) + (row + x) * 3;
            uint8_t kind = cell[0] & 7;
            if (kind == 0 || ((kind == 1 || kind == 3) && ((cell[2] >> 4) & 1) == 1)) {
                F<uint8_t*>(t, 0x1661)[row + x] = 0;
                F<uint8_t*>(t, 0x1665)[F<int32_t>(t, 0x15dd) * y + x] = 0;
                continue;
            }
            if (kind == 1 || kind == 3) {
                F<uint8_t*>(t, 0x1661)[row + x] = 0;
                F<uint8_t*>(t, 0x1665)[F<int32_t>(t, 0x15dd) * y + x] = 0;
                bool cliff = false;
                for (int32_t cy = y - 2; cy <= y + 2; ++cy)
                    for (int32_t cx = x - 2; cx <= x + 2; ++cx) {
                        if (cx < 0 || cx >= F<int32_t>(t, 0x15d9) - 1 || cy < 0 || cy >= F<int32_t>(t, 0x15d5) - 1) continue;
                        uint16_t k = kindAt(cx, cy);
                        if (k == 4 || k == 5) cliff = true;
                    }
                if (cliff) continue;
                int32_t cnt = 0, sx = 0, sz = 0;
                for (int32_t cy = y - 1; cy <= y + 1; ++cy)
                    for (int32_t cx = x - 1; cx <= x + 1; ++cx) {
                        if (cy == y && cx == x) continue;
                        uint16_t k = kindAt(cx, cy);
                        if (k == 1 || k == 3) continue;
                        ++cnt;
                        sx += cx - x;
                        sz += cy - y;
                    }
                F<uint8_t*>(t, 0x1631)[F<int32_t>(t, 0x15dd) * y + x] = 0xff;
                F<uint8_t*>(t, 0x1635)[F<int32_t>(t, 0x15dd) * y + x] = 0xff;
                F<uint8_t*>(t, 0x1639)[F<int32_t>(t, 0x15dd) * y + x] = 0xff;
                F<uint8_t*>(t, 0x163d)[F<int32_t>(t, 0x15dd) * y + x] = 0xff;
                if (cnt <= 1 || cnt >= 5) continue;
                double len = sqrt((double)(sz * sz + sx * sx));
                if (!(len > minLen)) continue;
                double inv = *(const double*)0x5c8770 / len;
                float px = (float)x, py = (float)y;
                float a = (float)((double)sx * inv);
                float fsz = (float)sz;
                float b = (float)((double)sz * inv);
                bool land = false;
                for (int32_t step = 0; step < 8; ++step) {
                    px = (float)((double)px - a);
                    py = (float)((double)py - b);
                    int32_t ix = FlowFtol(px), iy = FlowFtol(py);
                    if (ix < 0 || ix >= F<int32_t>(t, 0x15d9) || iy < 0 || iy >= F<int32_t>(t, 0x15d5)) break;
                    if (F<uint8_t*>(t, 0x1655)[F<int32_t>(t, 0x15dd) * iy + ix]) {
                        land = true;
                        break;
                    }
                }
                if (land) continue;
                F<uint8_t*>(t, 0x1661)[F<int32_t>(t, 0x15dd) * y + x] = (uint8_t)FlowFtol((double)sx * k125 * inv);
                F<uint8_t*>(t, 0x1665)[F<int32_t>(t, 0x15dd) * y + x] = (uint8_t)FlowFtol((double)fsz * k125 * inv);
                continue;
            }
            if (kind == 2 || kind == 4 || kind == 5) {
                int8_t* pz = (int8_t*)F<uint8_t*>(t, 0x1665) + row + x;
                int8_t* px = (int8_t*)F<uint8_t*>(t, 0x1661) + row + x;
                int32_t vz = *pz, vx = *px;
                double len = sqrt((double)(vz * vz + vx * vx));
                if (!(len > minLen)) continue;
                double inv = (double)*(const float*)0x5c85b4 / len;
                *px = (int8_t)FlowFtol((double)vx * inv * k125);
                F<uint8_t*>(t, 0x1665)[F<int32_t>(t, 0x15dd) * y + x] = (uint8_t)FlowFtol((double)vz * inv * k125);
            }
        }
    }
    for (int32_t y = y0 + 2; y < y1 - 2; ++y) {
        progress();
        for (int32_t x = x0 + 2; x < x1 - 2; ++x) {
            int32_t rowl = F<int32_t>(t, 0x15dd);
            uint16_t k = kindAt(x, y);
            if (k != 1 && k != 3) continue;
            uint8_t* dxp = F<uint8_t*>(t, 0x1661) + rowl * y + x;
            if (!*dxp || !F<uint8_t*>(t, 0x1665)[rowl * y + x]) continue;
            double sumX = *(const float*)0x5c8598, cnt = *(const float*)0x5c8598, sumZ = 0.0;
            for (int32_t yy = y - 3; yy <= y + 3; ++yy) {
                const int8_t* zr = (const int8_t*)F<uint8_t*>(t, 0x1665) + rowl * yy;
                const int8_t* xr = (const int8_t*)F<uint8_t*>(t, 0x1661) + rowl * yy;
                for (int32_t xx = x - 3; xx <= x + 3; ++xx) {
                    int8_t vz = zr[xx], vx = xr[xx];
                    if (!vx || !vz || y == yy || x == xx) continue;
                    sumX += vx;
                    sumZ += vz;
                    cnt += *(const float*)0x5c85b4;
                }
            }
            float fz = (float)sumZ;
            if (!(cnt > *(const float*)0x5c8598) || !(cnt < *(const float*)0x5c8554)) {
                *dxp = 0;
                F<uint8_t*>(t, 0x1665)[F<int32_t>(t, 0x15dd) * y + x] = 0;
                continue;
            }
            float invCnt = (float)((double)*(const float*)0x5c85b4 / cnt);
            double mx = sumX * invCnt;
            double mz = (double)fz * invCnt;
            float fmz = (float)mz;
            double len = sqrt(mz * fmz + mx * mx);
            float invLen = (float)((double)*(const float*)0x5c85b4 / len);
            *dxp = (uint8_t)FlowFtol((double)invLen * mx * k125);
            F<uint8_t*>(t, 0x1665)[F<int32_t>(t, 0x15dd) * y + x] = (uint8_t)FlowFtol((double)fmz * invLen * k125);
        }
    }
    if (!F<uint8_t**>(t, 0x1681)) return;
    int32_t bx1 = x1 >> 4, by1 = y1 >> 4;
    if ((F<int32_t>(t, 0x15d9) >> 4) - 1 < bx1) bx1 = (F<int32_t>(t, 0x15d9) >> 4) - 1;
    if ((F<int32_t>(t, 0x15d5) >> 4) - 1 < by1) by1 = (F<int32_t>(t, 0x15d5) >> 4) - 1;
    for (int32_t bx = x0 >> 4; bx <= bx1; ++bx) {
        progress();
        for (int32_t by = y0 >> 4; by <= by1; ++by) Water_BuildQuads(F<uint8_t**>(t, 0x1681)[(F<int32_t>(t, 0x15d9) >> 4) * by + bx]);
    }
}
RT3_REPLACE(0x536710, Terrain_RefreshWaterFlow);

// 0x536ea0. Adds an overlay quad with corners (x, y, z) x 4 (laid on the ground +
// raise when raise != 0) to the per-16-cell block list (+0x167d) of its centre.
PtrListNode* __fastcall Terrain_AddQuad(uint8_t* t, int, float x0, float y0, float z0, float x1, float y1, float z1,
                                        float x2, float y2, float z2, float x3, float y3, float z3, uint8_t raise) {
    Quad* q = (Quad*)Malloc(0x31);
    const float in[12] = { x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3 };
    for (int k = 0; k < 12; ++k) q->c[k] = in[k];
    q->raise = raise;
    if (raise) DropQuad(t, q);
    int32_t sx, sz;
    __asm {
        fld x3
        fadd x2
        fadd x1
        fadd x0
        mov eax, 0x5a10d0
        call eax
        mov sx, eax
        fld z3
        fadd z2
        fadd z1
        fadd z0
        mov eax, 0x5a10d0
        call eax
        mov sz, eax
    }
    int32_t bx = ((sx >> 2) / 92) >> 4;
    int32_t bz = ((sz >> 2) / 92) >> 4;
    PtrList* l = F<PtrList**>(t, 0x167d)[(F<int32_t>(t, 0x15d9) >> 4) * bz + bx];
    return PtrList_Append(l, 0, q);
}
RT3_REPLACE(0x536ea0, Terrain_AddQuad);

// 0x537010. Re-lays the raised overlay quads of the blocks covering cells
// (x0, y0)-(x1, y1) on the terrain.
void __fastcall Terrain_RefreshQuads(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    x0 = x0 < 0 ? 0 : x0;
    y0 = y0 < 0 ? 0 : y0;
    if (F<int32_t>(t, 0x15d9) - 2 < x1) x1 = F<int32_t>(t, 0x15d9) - 2;
    if (F<int32_t>(t, 0x15d5) - 2 < y1) y1 = F<int32_t>(t, 0x15d5) - 2;
    y1 >>= 4;
    y0 >>= 4;
    x0 >>= 4;
    x1 >>= 4;
    for (int32_t by = y0; by <= y1; ++by)
        for (int32_t bx = x0; bx <= x1; ++bx) {
            PtrList* l = F<PtrList**>(t, 0x167d)[(F<int32_t>(t, 0x15d9) >> 4) * by + bx];
            PtrList_Rewind(l);
            for (Quad* q = (Quad*)PtrList_Next(l); q; q = (Quad*)PtrList_Next(l))
                if (q->raise) DropQuad(t, q);
        }
}
RT3_REPLACE(0x537010, Terrain_RefreshQuads);

// 0x537170. Detail factor for the tile (x, z) from its distance to the camera
// (engine +0x28; distance at least 0x5c85b4): (c / d + sqrt(c / d)) * 0x5dd2f0.
RT3_NOINLINE void __stdcall Camera_DetailFactor(int32_t x, int32_t z, float* out) {
    float cx, cy, cz;
    Obj_GetXYZ1c(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0, (uint32_t*)&cx, (uint32_t*)&cy,
                 (uint32_t*)&cz);
    double dx = (double)x - cx;
    double dz = (double)z - cz;
    double d = Fsqrt(dz * dz + dx * dx + (double)cy * cy);
    const float lo = *(const float*)0x5c85b4;
    if ((double)lo > d) d = lo;
    double q = (double)*(const float*)0x5c87c0 / d;
    *out = (float)((Fsqrt(q) + q) * *(const double*)0x5dd2f0);
}
RT3_REPLACE(0x537170, Camera_DetailFactor);

// 0x5371f0. Fog/haze for the point (x, z): a strength from the camera distance
// (sqrt(300 / distance), or `bias` itself / scaled by -bias), clamped to
// 0.25..1.25; *color = engine +0x1141f5. Weak values (< 0.3667) or option
// 0x6d4034 use the fixed haze (engine +0x1141f9, 0.3667, scale 1).
RT3_NOINLINE void __fastcall Fog_ParamsAt(uint8_t*, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color,
                             float* strength) {
    float cy, cx, cz;
    Obj_GetXYZ1c(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0, (uint32_t*)&cx, (uint32_t*)&cy,
                 (uint32_t*)&cz);
    double dx = (double)x - cx, dz = (double)z - cz;
    double len = sqrt(dz * dz + dx * dx + (double)cy * cy);
    if (Fl(0x5c85b4) > len) len = Fl(0x5c85b4);
    double v = sqrt(Fl(0x5c87c0) / len);
    if (bias < Fl(0x5c8598)) v = -(double)bias * v;
    else if (bias > Fl(0x5c8598)) v = bias;
    if (v < D(0x5c8758)) v = D(0x5c8758);
    if (v > D(0x5c87e8)) v = D(0x5c87e8);
    float s = (float)v;
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    *color = F<uint32_t>(e, 0x1141f5);
    *strength = s;
    if ((double)s < D(0x5dd2f8) || hook::Global<int32_t>(0x6d4034)) {
        *(uint32_t*)strength = 0x3ebbbbbc;
        *color = F<uint32_t>(hook::Global<uint8_t*>(0x6d4024), 0x1141f9);
        *scale = 1.0f;
        return;
    }
    *scale = s;
}
RT3_REPLACE(0x5371f0, Fog_ParamsAt);

// 0x537330. Destroys the terrain (when built, +0x154c): its channels, grids
// and markers, texture slots, the texture +0x478 and the list +0x166d.
RT3_NOINLINE void __fastcall Terrain_Destroy(uint8_t* r) {
    if (!F<int32_t>(r, 0x154c)) return;
    Terrain_FreeFixedChannels(r);
    Terrain_Free(r);
    Terrain_DeleteMarkers(r);
    Render_ReleaseTextureSlots(r);
    if (uint8_t* t = F<uint8_t*>(r, 0x478)) {
        Texture_Destroy(t);
        ((void(__cdecl*)(void*))0x53b080)(t);
        F<uint8_t*>(r, 0x478) = nullptr;
        r[0x159b] = 0;
    }
    if (PtrList* l = F<PtrList*>(r, 0x166d)) {
        PtrList_Destroy(l);
        ((void(__cdecl*)(void*))0x53b080)(l);
    }
    F<PtrList*>(r, 0x166d) = nullptr;
    F<int32_t>(r, 0x154c) = 0;
}
RT3_REPLACE(0x537330, Terrain_Destroy);

// 0x5373b0. Clears the 27 sound slots and loads the weather sounds: rain
// (wea_rain.wav) and six thunder claps (wea_thunder_01..06.wav).
RT3_NOINLINE void __fastcall Weather_LoadSounds(uint8_t* s) {
    for (int32_t i = 0; i < 0x1b; ++i) F<uint8_t*>(s, 0x40c + i * 4) = nullptr;
    F<int32_t>(s, 0x408) = 0;
    static const uint32_t names[] = { 0x5dd378, 0x5dd364, 0x5dd350, 0x5dd33c, 0x5dd328, 0x5dd314, 0x5dd300 };
    for (uint32_t n : names) Sound_StartInSlot(s, 0, (const char*)n);
}
RT3_REPLACE(0x5373b0, Weather_LoadSounds);

// 0x537420. Clears the water mask (+0x47c, one byte per 16 x 16 block, 0x80
// = has water) and rebuilds it from the land map (+0x1655), setting +0x1599
// when any cell is water, then refreshes the flow over the whole map.
RT3_NOINLINE void __fastcall Terrain_RebuildWaterMask(uint8_t* t) {
    int32_t xl = F<int32_t>(t, 0x15d9) - 1, yl = F<int32_t>(t, 0x15d5) - 1;
    memset(t + 0x47c, 0, 0x1000);
    t[0x1599] = 0;
    for (int32_t y = 0; y <= yl; ++y) {
        if (hook::Global<void(__cdecl*)()>(0x6d4014)) hook::Global<void(__cdecl*)()>(0x6d4014)();
        for (int32_t x = 0; x <= xl; ++x) {
            if (F<uint8_t*>(t, 0x1655)[F<int32_t>(t, 0x15dd) * y + x]) continue;
            t[0x1599] = 1;
            t[0x47c + (x >> 4) * 64 + (y >> 4)] |= 0x80;
        }
    }
    Terrain_RefreshWaterFlow(t, 0, 0, 0, xl, yl);
}
RT3_REPLACE(0x537420, Terrain_RebuildWaterMask);

// 0x5374d0. Refreshes vertices [x0 - 1, x1 + 1] x [y0 - 1, y1 + 1] (clamped
// to the map) of terrain t after an edit: normals, then shading, then water
// flow and the mip levels; clears +0x15e1.
RT3_NOINLINE void __fastcall Terrain_RefreshRect(uint8_t* t, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    x0 = x0 - 1 < 0 ? 0 : x0 - 1;
    F<int32_t>(t, 0x15e1) = 0;
    y0 = y0 - 1 < 0 ? 0 : y0 - 1;
    x1 = F<int32_t>(t, 0x15d9) < x1 + 1 ? F<int32_t>(t, 0x15d9) : x1 + 1;
    y1 = F<int32_t>(t, 0x15d5) < y1 + 1 ? F<int32_t>(t, 0x15d5) : y1 + 1;
    for (int32_t y = y0; y <= y1; ++y)
        for (int32_t x = x0; x <= x1; ++x) Terrain_ComputeVertexNormal(t, 0, x, y);
    for (int32_t y = y0; y <= y1; ++y)
        for (int32_t x = x0; x <= x1; ++x) Terrain_ShadeVertex(t, 0, x, y, 0);
    Terrain_RefreshWaterFlow(t, 0, x0, y0, x1, y1);
    Terrain_RebuildMipsRect(t, 0, x0, y0, x1, y1);
}
RT3_REPLACE(0x5374d0, Terrain_RefreshRect);

 // namespace

// 0x5375c0. With `init`, allocates whatever per-vertex maps of terrain t are
// still missing (cleared; the 4 edge maps +0x1631.. random from the LCG
// 0x624324, the land map +0x1655 all 2, +0x1659 all 1), the 16 x 16 block
// grids (four of pointer lists, the water grids +0x1681 built), the axis
// tables (+0x15e9 / +0x15ed = k / (n / 4) over a quarter of the columns /
// rows, +0x15e5 = 0x5c k), fails fatally (strings 0xa5 / 0xa6) without a
// height map or when an allocation failed, then computes every normal and
// shade, the water mask and the mip levels.
RT3_NOINLINE void __fastcall Terrain_InitMaps(uint8_t* t, int, char init, int32_t, int32_t) {
    if (!init) return;
    F<int32_t>(t, 0x15e1) = 0;
    if (!F<int32_t>(t, 0x1605)) Fatal_Exit(GetString(0xa5));
    int32_t cols = F<int32_t>(t, 0x15d9), rows = F<int32_t>(t, 0x15d5);
    uint32_t n = (uint32_t)((rows + 1) * (cols + 1));
    int32_t bx = (cols - 1) >> 4, by = (rows - 1) >> 4;
    TerrainAllocFilled(t, 0x15f1, n * 12, 0);
    TerrainProgress();
    TerrainAllocFilled(t, 0x1619, n * 2, 0);
    TerrainAllocFilled(t, 0x162d, n, 0);
    for (int k = 0; k < 4; ++k) {
        uint8_t*& e = F<uint8_t*>(t, 0x1631 + k * 4);
        if (e) continue;
        e = (uint8_t*)TerrainMalloc(n);
        for (int32_t i = 0; i < (int32_t)n; ++i) {
            uint32_t s = hook::Global<uint32_t>(0x624324) * 0x19660d + 0x3c6ef35f;
            hook::Global<uint32_t>(0x624324) = s;
            e[i] = (uint8_t)s;
        }
    }
    TerrainProgress();
    TerrainAllocFilled(t, 0x1669, n * 4, 0);
    TerrainAllocFilled(t, 0x1655, n, 2);
    TerrainProgress();
    TerrainAllocFilled(t, 0x1659, n, 1);
    TerrainAllocFilled(t, 0x165d, n * 3, 0);
    TerrainProgress();
    TerrainAllocFilled(t, 0x1661, n, 0);
    TerrainProgress();
    TerrainAllocFilled(t, 0x1665, n, 0);
    TerrainProgress();
    TerrainAllocFilled(t, 0x1685, (uint32_t)(by * bx * 4), 0);
    TerrainProgress();
    TerrainAllocLists(t, 0x1675, bx, by);
    TerrainAllocLists(t, 0x167d, bx, by);
    TerrainAllocLists(t, 0x1679, bx, by);
    TerrainAllocLists(t, 0x1671, bx, by);
    int32_t m = F<int32_t>(t, 0x15d9) > F<int32_t>(t, 0x15d5) ? F<int32_t>(t, 0x15d9) : F<int32_t>(t, 0x15d5);
    int32_t m2 = m * 2;
    if (!F<void*>(t, 0x15ed)) F<void*>(t, 0x15ed) = TerrainMalloc((uint32_t)(m2 * 4));
    if (!F<void*>(t, 0x15e9)) F<void*>(t, 0x15e9) = TerrainMalloc((uint32_t)(m2 * 4));
    if (!F<void*>(t, 0x15e5)) F<void*>(t, 0x15e5) = TerrainMalloc((uint32_t)(m2 * 4));
    TerrainProgress();
    if (!F<void*>(t, 0x15f1) || !F<void*>(t, 0x162d) || !F<void*>(t, 0x1619) || !F<void*>(t, 0x15ed) || !F<void*>(t, 0x15e9) || !F<void*>(t, 0x1655) ||
        !F<void*>(t, 0x15e5) || !F<void*>(t, 0x1659) || !F<void*>(t, 0x165d))
        Fatal_Exit(GetString(0xa6));
    int32_t qx = F<int32_t>(t, 0x15d9) / 4, qy = F<int32_t>(t, 0x15d5) / 4;
    float qyf = (float)qy;
    if (qx + 1 > 0) {
        double step = (double)*(const float*)0x5c85b4 / (double)qx;
        for (int32_t k = 0; k < qx + 1; ++k) F<float*>(t, 0x15e9)[k] = (float)((double)k * step);
    }
    TerrainProgress();
    if (qy + 1 > 0) {
        double step = (double)*(const float*)0x5c85b4 / (double)qyf;
        for (int32_t k = 0; k < qy + 1; ++k) F<float*>(t, 0x15ed)[k] = (float)((double)k * step);
    }
    for (int32_t k = 0; k < m2; ++k) F<float*>(t, 0x15e5)[k] = (float)(k * 0x5c);
    TerrainProgress();
    float last = qyf;  // the shade pass gets the last normal (this when there is none)
    for (int32_t x = 0; x < F<int32_t>(t, 0x15d9); ++x) {
        TerrainProgress();
        for (int32_t y = 0; y < F<int32_t>(t, 0x15d5); ++y) last = Terrain_ComputeVertexNormal(t, 0, x, y);
    }
    int32_t lastBits = *(int32_t*)&last;
    for (int32_t x = 0; x < F<int32_t>(t, 0x15d9); ++x) {
        TerrainProgress();
        for (int32_t y = 0; y < F<int32_t>(t, 0x15d5); ++y) Terrain_ShadeVertex(t, 0, x, y, lastBits);
    }
    Terrain_RebuildWaterMask(t);
    if (!F<void*>(t, 0x1681)) {
        TerrainAllocFilled(t, 0x1681, (uint32_t)(by * bx * 4), 0);
        for (int32_t i = 0; i < bx; ++i) {
            TerrainProgress();
            for (int32_t j = 0; j < by; ++j) {
                uint8_t* g = (uint8_t*)TerrainNew(0x28);
                g = g ? WaterGrid_Init(g, 0, i, j, t) : nullptr;
                F<uint8_t**>(t, 0x1681)[(F<int32_t>(t, 0x15d9) >> 4) * j + i] = g;
            }
        }
    }
    Terrain_RebuildMips(t);
}
RT3_REPLACE(0x5375c0, Terrain_InitMaps);

// 0x537e30. Terrain renderer destructor: destroys it and unhooks it from the
// engine (+0x1c) when it is the current one.
RT3_NOINLINE void __fastcall Terrain_Destructor(uint8_t* r) {
    F<uint32_t>(r, 0) = 0x5dd25c;
    Terrain_Destroy(r);
    if (Engine_Get1C(hook::Global<uint8_t*>(0x6d4024)) == r) Engine_Set1C(hook::Global<uint8_t*>(0x6d4024), 0, nullptr);
}
RT3_REPLACE(0x537e30, Terrain_Destructor);

// 0x537e60. Sets up terrain t of w x h cells (registered with the engine,
// serial +0x2121 from 0xccba10; fatal without an engine): its grids and
// maps, texture slots and tile texture `tex`, the five detail textures
// (+0x1554 = the first), their blend levels (+0x1641..), water, overview
// target (+0x1548), rain and weather sounds, reporting progress between
// the steps.
RT3_NOINLINE void __fastcall Terrain_Create(uint8_t* t, int, int32_t w, int32_t h, const char* tex) {
    auto progress = [] {
        if (hook::Global<void(__cdecl*)()>(0x6d4014)) hook::Global<void(__cdecl*)()>(0x6d4014)();
    };
    progress();
    Terrain_Init(t);
    PtrList* l = (PtrList*)((void*(__cdecl*)(uint32_t))0x53b070)(0x18);
    F<PtrList*>(t, 0x166d) = l ? PtrList_Init(l) : nullptr;
    progress();
    F<int32_t>(t, 0x2121) = hook::Global<int32_t>(0xccba10);
    ++hook::Global<int32_t>(0xccba10);
    if (!hook::Global<void*>(0x6d4024)) Fatal_Exit(GetString(0xa3));
    Engine_Set1C(hook::Global<void*>(0x6d4024), 0, t);
    F<int32_t>(t, 0x15d9) = w;
    F<int32_t>(t, 0x15d5) = h;
    F<int32_t>(t, 0x15dd) = w + 1;
    progress();
    if (w && h) Render_AllocGrid(t, 0, 0, 0);
    progress();
    F<int32_t>(t, 0x154c) = 1;
    if (w && h) Terrain_InitMaps(t, 0, 1, 0, 0);
    progress();
    TextureSlots_Reset(t);
    hook::Global<int32_t>(0xccb9e8) += 2;
    progress();
    Terrain_LoadTexture(t, 0, tex, 0);
    progress();
    F<int32_t>(t, 0x1554) = TextureSlots_Add(t, 0, (const char*)0x5dd3b8);
    TextureSlots_Add(t, 0, (const char*)0x5dd3ac);
    TextureSlots_Add(t, 0, (const char*)0x5dd3a0);
    TextureSlots_Add(t, 0, (const char*)0x5dd394);
    TextureSlots_Add(t, 0, (const char*)0x5dd388);
    F<uint32_t>(t, 0x1641) = 0x3e99999a;
    F<uint32_t>(t, 0x1645) = 0x3ecccccd;
    F<uint32_t>(t, 0x1649) = 0x3edc28f6;
    F<uint32_t>(t, 0x164d) = 0x3f333333;
    F<uint32_t>(t, 0x1651) = 0x40000000;
    progress();
    Water_LoadTextures(t);
    progress();
    Render_CreateOverviewTarget(t, 0, F<int32_t>(t, 0x1548));
    progress();
    Rain_LoadFrames(t);
    progress();
    Weather_LoadSounds(t);
}
RT3_REPLACE(0x537e60, Terrain_Create);

}  // namespace rt3
