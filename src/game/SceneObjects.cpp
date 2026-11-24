// SceneObjects.cpp - RT3.exe 0x454000-0x45e000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/crt.h"
#include "game/calendar.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/containers.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static const float kCellUnits = 92.0f;
double __fastcall Obj_CellX(void* o);
double __fastcall Obj_CellY(void* o);
uint8_t* __fastcall Obj_Region(void* o);
uint32_t __fastcall BlockIter_Begin(uint8_t* o);
int32_t ClockMs();
int32_t __fastcall BlockIter_BeginTest(uint8_t* o, uint32_t mask, int32_t idx);
float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m);
float* __fastcall Vec3_Normalize(float* v);
void __fastcall BlockIter_SetFlags(uint8_t* o, uint32_t mask, int32_t mode);
void __fastcall Vec3_CrossInPlace(float* a, int, const float* b);
double __fastcall Vec3_Length(const float* v);
void* __fastcall Table_Oldest(Collection* t, int, uint8_t all);
void* __fastcall ReturnThis455290(void* o);
void __fastcall Nop4552a0(void*);
void __fastcall Entry21a_Init(uint8_t* e, int, int32_t a, const char* name, int32_t b, int32_t c, int32_t d);
void __fastcall Obj_SetElementFlag1d8(void* o, int32_t mode, int32_t idx);
void __fastcall BlockIter_MarkSlot(uint8_t* o, int32_t idx, int32_t flag);
void __fastcall BlockIter_ClearSlot(uint8_t* o, int32_t idx);
void __fastcall Camera_Destruct(uint32_t* c);
void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall SceneObj_SetUvTransform(uint8_t* o, int, float u, float v, float su, float sv);
int32_t __fastcall SceneObj_Reset_Impl(uint8_t* o);
void __fastcall SceneObj_SaveFlags(uint8_t* o, int, Stream s);
void __fastcall SceneObj_SetAttachmentsFlag(uint8_t* o, int, int32_t on);
void __fastcall StreamWriteLenString(Stream s, const char* const* str);
uint8_t* __fastcall SceneObj_ConstructDefault(uint8_t* o);
int32_t __fastcall NamedObj_Reset(uint8_t* o);
void __fastcall NamedObj_Save(uint8_t* o, int, Stream s);
void __fastcall SceneObj_Destructor(uint8_t* o);
uint8_t* __fastcall NamedObj_Construct(uint8_t* o);
void __fastcall NamedObj_Destructor(uint8_t* o);
void __fastcall Obj_Decc60(uint8_t* o);

uint8_t* __fastcall Camera_ConstructSetup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z,
                                          uint32_t dist);

void __fastcall Flags24_Set1(uint8_t* o);
void __fastcall Flags24_Clear(uint8_t* o);
void __fastcall Obj_SetUnit25(uint8_t* o, int, float v);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                            const char* a6, const char* a7, const char* a8);

int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
uint8_t* __fastcall Camera_Construct(uint8_t* c, int, const char* name, uint32_t owner);
PtrListNode* __fastcall SceneObj_AddChild(uint8_t* o, int, uint8_t* child);
PtrListNode* __fastcall SceneObj_AddAttachment(uint8_t* o, int, uint8_t* item);
PtrListNode* __fastcall Camera_AddListener(uint8_t* c, int, void* item);

uint8_t* __fastcall Emitter_Construct(uint8_t* e, int, const char* name, uint32_t color, int32_t perPoint, const float* rect, uint8_t* obj,
                                      uint32_t flags);

void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);

uint8_t* __fastcall Channel_Setup(uint8_t* ch, int, const char* name, uint8_t* grid, float x, float y, float z, int32_t a6, int32_t a7,
                                  float vol);

void __fastcall Obj_AttachChannel(uint8_t* o, int, uint8_t* ch);
void __fastcall Channel_SetLoop(uint8_t* c, int, uint32_t v);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
int32_t __fastcall WorldModel_Load(uint8_t* o, int, int32_t s);
bool __fastcall Aircraft_Fly(uint8_t* v, int, float dt);

void __fastcall WorldModel_Init(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y,
                                int32_t freeStanding);

void __fastcall Map_RandomEdgeEntry(uint8_t* o, int, float* outX, float* outY, float* outZ, float* heading);
void __fastcall SceneObj_RotateAxis(uint8_t* o, int, float ax, float ay, float az, float angle);
void __fastcall SceneObj_ApplyQuat(uint8_t* o);
void __fastcall SceneObj_ResetQuat(uint8_t* o);
int32_t __fastcall Channel_Check(uint8_t* c);
void __fastcall Channel_SetRate(uint8_t* c, int, int32_t rate);
void __fastcall Channel_SetVolume(uint8_t* c, int, uint32_t v);
uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name);
void __fastcall Channel_SetName(uint8_t* c, int, const char* name);
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Obj_EntryClear1d8(uint8_t* o, int32_t i);
void __fastcall Group_ResetLists(uint8_t* g);
void __fastcall SceneObj_SetPos(uint8_t* o, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall SceneObj_GetPos2(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);

uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol,
                                 int32_t mode, int32_t paused, int32_t prio, float maxDist);

void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
uint8_t __fastcall Channel_Playing(uint8_t* c);
void* __fastcall Obj_Train390(void* o);
void* __fastcall Obj_CityRef(void* o);
int32_t __fastcall SimRand(int32_t lo, int32_t hi);
uint32_t __fastcall Terrain_CellByte(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n);
int32_t __fastcall Map_CellLow3(void* m, int, int32_t x, int32_t y);
void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
double __fastcall Terrain_HeightAtTile(uint8_t* r, int, float x, float z);
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
void* __fastcall Engine_Get1C(void* e);
void __fastcall Effect_Free(uint32_t* e);
void __fastcall SpriteSet_Free(uint8_t* o);
void __fastcall ModelSet_Clear(uint8_t* o);
void __fastcall EffectSet_Release(uint8_t* s);
void* __fastcall Anim_LoadFile(const char* name);
void __fastcall Attach_ReplaceObject(uint8_t* t, uint8_t* oldObj, uint8_t* newObj);
int32_t __fastcall ModelObj_ResetState(uint8_t* o, int, int32_t full);
void __fastcall String_Assign(char** s, const char* src);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t __fastcall Building_AttachSound(uint8_t* b, int, int32_t, uint8_t* grid, float x, float y, float z);

uint8_t* __fastcall WorldModel_Construct(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y,
                                         int32_t freeStanding);

int32_t __fastcall IsCurrent6acd3c(void* o);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t unused1, int32_t unused2);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t unused, int32_t count);
int32_t __fastcall StreamReadString(int32_t s, char** out);
void __fastcall FreeAndNull2(void** p);
void __fastcall Obj_AttachGroundSound(uint8_t* o, int, int32_t kind);
int32_t __fastcall Obj_StaticEntry(void* o, int, char* name, int32_t* a, int32_t* b);

char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*);

void __fastcall Channel_SetRange(uint8_t* c, int, uint32_t lo, uint32_t hi);
float __fastcall SceneObj_Angle1(uint8_t* o);
float __fastcall SceneObj_Angle2(uint8_t* o);
bool __fastcall Obj_MoveBy(uint8_t* o, int, float dx, float dy, float dz);
int32_t IsMultiplayer();
void __fastcall SceneObj_Init(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
int32_t SceneObj_Reset();
void __fastcall Nop52e670(void*, int, int32_t);
int32_t __fastcall Model_SetFrame(uint8_t* m, int, int32_t f);
void __fastcall Obj_PlaceAtCell(uint8_t* o, int, float cx, float cz, int32_t lift);
void __fastcall SceneObj_LoadFlags(uint8_t* o, int, int32_t s);
uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
int32_t __fastcall Vehicle_AttachEngineSound(uint8_t* o, int, const char* typeName, uint8_t* grid, float x, float y, float z);
uint32_t __fastcall Effects_SpawnForModel(uint8_t* parent, int, const char* prefix, uint8_t** out, int32_t count, uint32_t flags, float scale);
void __fastcall Fatal_Exit(const char* msg);
const char* __fastcall GetString(int32_t id);
uint8_t* __fastcall Effect_Create(uint8_t* parent, int32_t kind, float scale, uint32_t flags, int32_t listed);
int32_t __fastcall SceneObj_RecentlyDrawn(uint8_t* o);
void __fastcall Store3Globals(int32_t* v);
void __fastcall Obj_ApplyStored3(void* o, int32_t idx);
void __fastcall View_SetParams(uint8_t* v, int, int32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
void __fastcall Vehicle_AnimateLinks(uint8_t* v, int, float step);
int32_t __fastcall Obj_SumEntries6c(uint8_t* o);
int32_t __fastcall Obj_SumEntrySizes(uint8_t* o);
void __fastcall SceneObj_Rotate(uint8_t* o, int, float da, float db, float dc);
void __fastcall Obj_TurnPivot(uint8_t* o, int, float angle, int32_t a, int32_t b);
void __fastcall Vehicle_RollWheels(uint8_t* v, int, float dist);
int __fastcall ToUpper(char c);
double __fastcall Terrain_HeightAt(uint8_t* t, int, float x, float z);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Obj_Incc60(uint8_t* o);
void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
void __fastcall Engine_KeyUp(uint8_t* e, int, uint8_t k);
void* __fastcall Engine_Input(void* e);
void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
uint32_t __fastcall Obj_AnimFrame(void* o);
void __fastcall Camera_SetEye(uint8_t* c, int, uint32_t x, uint32_t y, uint32_t z);
void __fastcall Camera_SetTarget(uint8_t* c, int, float x, float y, float z);
void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e);
void __fastcall SceneObj_Destruct(uint8_t* o);
void __fastcall ModelObj_Destructor(uint8_t* o);
double __fastcall Obj_Float18(void* o);
void __fastcall Camera_SetZoom(uint8_t* c, int, uint32_t zoom);
void __fastcall ModelObj_Release(uint8_t* o);
void __fastcall SceneObj_Teardown(uint8_t* o);
void __fastcall Sites_Free(uint8_t* mgr, int, uint8_t* site);
int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void __fastcall Obj_DropCamera(uint8_t* o);
void __fastcall Building_LoadParts(uint8_t* o, int, int32_t upgraded);
uint8_t* __fastcall ModelObj_Construct(uint8_t* o);
void __fastcall Building_Init(uint8_t* o, int, const char* name, const char* tex, float x, float z, int32_t place, int32_t a6, int32_t a7, uint16_t a8);
void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);
uint8_t __fastcall Engine_KeyState(uint8_t* e, int, uint8_t k);
void __fastcall Obj_TakeCamera(uint8_t* o);
uint32_t __fastcall Input_Flag400();
void PushSeed();
void __fastcall SetSeed(int32_t seed);
void PopSeed();
void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
int32_t __fastcall TextureSlots_Add(uint8_t* t, int, const char* name);
int32_t __fastcall Terrain_AddMarker(uint8_t* r, int, uint32_t v, int32_t, int32_t);
void Vegetation_BuildTables();
void __fastcall Vehicle_BlinkLights(uint8_t* v, int, float dt);
void __fastcall Vehicle_SpinWheels(uint8_t* v, int, float speed);
void __fastcall TrainObj_CabKeys(uint8_t* o, int, float dt);
void __fastcall SceneObj_ScaleRotation(uint8_t* o, int, float f);
void __fastcall Obj_DownVector(uint8_t* o, int, float* out);
float* __fastcall Obj_BendOffset(uint8_t* o, int, float* out, float ax, float ay, float az, float bx, float by, float bz);
void __fastcall Obj_SetXYZ2c(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Obj_AxisTerms(uint8_t* o, int, float vx, float vy, float vz, float* dot, float* lenC, float* lenA);
void __fastcall Obj_OffsetVector(uint8_t* o, int, float* out);
void __fastcall Obj_ScaledPoint(uint8_t* o, int, float* out);
double __stdcall Vec3_AngleTerm(float a0, float a1, float a2, float b0, float b1, float b2);
void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
void __fastcall StreamWriteU32(Stream s, uint32_t value);
RT3_NOINLINE int32_t __fastcall DistanceInt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
RT3_NOINLINE double __stdcall Distance2D(float x1, float y1, float x2, float y2);
RT3_NOINLINE int __fastcall ToUpper(char c);
RT3_NOINLINE int32_t __fastcall SimRand(int32_t lo, int32_t hi);
RT3_NOINLINE void __fastcall SetSeed(int32_t seed);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE void* __fastcall Engine_Input(void* e);
RT3_NOINLINE void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
void __fastcall Map_UpdateBlockMask(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t);
void __fastcall PtrList_Clear(PtrList* l);
void __fastcall Nop5548c0(void*, int, int32_t, int32_t);
void __fastcall Attach_Animate(uint8_t* t, uint8_t* obj, int32_t now, int32_t mode);
void __fastcall Obj_SaveNamedPair(uint8_t* o, int, Stream s);
void __fastcall Car_Save(uint8_t* c, int, Stream s);
int32_t __fastcall Obj_TypeValue(uint8_t* o, int, int32_t* out);
int32_t __fastcall Obj_TypeRecord(uint8_t* o, int, char* name, int32_t* a, int32_t* b, int32_t* count);
void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
void __fastcall TrainObj_Clicked(uint8_t* o);
void __stdcall Obj_SyncGroupFlags(uint8_t* o, int32_t full);
uint8_t* __fastcall BlockIter_NextSameKey(uint8_t* b);
void __fastcall Obj_SwapBlockFlags(void*, int, uint8_t* o);
void __fastcall Emitter_Tick(uint8_t* e, int, int32_t flag);
int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);

// Functions of this file, in exe order.
RT3_NOINLINE int32_t __fastcall Obj_TypeValue(uint8_t* o, int, int32_t* out);
RT3_NOINLINE int32_t __fastcall Obj_TypeRecord(uint8_t* o, int, char* name, int32_t* a, int32_t* b, int32_t* count);
RT3_NOINLINE void __fastcall Sites_Free(uint8_t* mgr, int, uint8_t* s);
RT3_NOINLINE void __fastcall World_SetFollow(uint8_t* w, int, int32_t on, uint8_t* target);
RT3_NOINLINE void __fastcall World_FollowEntity(uint8_t* w, int, int32_t on, int32_t kind, int32_t id);
RT3_NOINLINE void Vegetation_BuildTables();
RT3_NOINLINE void __fastcall Terrain_AdjustLevel(uint8_t* m, int, float x, float y, float r, float amount);
RT3_NOINLINE void __fastcall Terrain_PaintPoints(uint8_t* t, int, float x, float y, float r, int32_t erase, int32_t kind, int32_t variant, float density, int32_t force);
RT3_NOINLINE void __fastcall Map_UpdateBlockMask(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall Map_UpdateBlockMaskRect(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
RT3_NOINLINE void __fastcall TreeTypes_Load(uint8_t* r);
void* __fastcall ReturnThis455290(void* o);
void __fastcall Nop4552a0(void*);
void __fastcall Entry21a_Init(uint8_t* e, int, int32_t a, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE void __fastcall Obj_Reset455310(void* o);
RT3_NOINLINE Collection* __fastcall Collection5cae10_Construct(Collection* c);
RT3_NOINLINE void __fastcall Collection5cae10_Destruct(Collection* c);
RT3_NOINLINE void* __fastcall Table_Newest(Collection* t);
RT3_NOINLINE void* __fastcall Table_Oldest(Collection* t, int, uint8_t all);
RT3_NOINLINE void* __fastcall Table_FindField4Prev(Collection* t, int, int32_t v);
RT3_NOINLINE void* __fastcall Table_FindField4Next(Collection* t, int, int32_t v);
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
RT3_NOINLINE void __fastcall Table_MarkStale(Collection* t);
int32_t __fastcall NamedObj_Reset(uint8_t* o);
RT3_NOINLINE void __fastcall NamedObj_Destructor(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_PlaceAtCell(uint8_t* o, int, float cx, float cz, int32_t lift);
RT3_NOINLINE void __fastcall Obj_MoveToCell(uint8_t* o, int, float cx, float cz, float y);
RT3_NOINLINE float __fastcall SceneObj_Angle2(uint8_t* o);
RT3_NOINLINE void __fastcall SceneObj_SetAngle2(uint8_t* o, int, uint32_t v);
float __fastcall SceneObj_Angle1(uint8_t* o);
RT3_NOINLINE void ModelPreview_Close();
int32_t __fastcall IsCurrent6acd3c(void* o);
RT3_NOINLINE double __fastcall Obj_CellX(void* o);
RT3_NOINLINE double __fastcall Obj_CellY(void* o);
RT3_NOINLINE void __fastcall NamedObj_Save(uint8_t* o, int, Stream s);
void Obj_VCall44();
int32_t __fastcall Obj_CloseCurrent(void* o, int, int32_t a);
void __fastcall Obj_DropToGround(uint8_t* o, int, int32_t lift);
void __fastcall Nop455b00(void*);
void __fastcall Nop455b10(void*);
uint8_t* __fastcall NamedObj_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall WorldModel_Init(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y, int32_t freeStanding);
RT3_NOINLINE void __fastcall SceneObj_Teardown(uint8_t* o);
RT3_NOINLINE void __fastcall SceneObj_ShowPreview(uint8_t* o);
RT3_NOINLINE uint8_t* __fastcall Obj_Region(void* o);
RT3_NOINLINE int32_t __fastcall WorldModel_Load(uint8_t* o, int, int32_t s);
float __fastcall Model_FrameTime(uint8_t* o);
RT3_NOINLINE uint8_t* __fastcall WorldModel_Construct(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y, int32_t freeStanding);
void __fastcall NamedObj456900_Destructor(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_MarkDone(uint8_t* o);
RT3_NOINLINE void __fastcall TrainObj_Clicked(uint8_t* o);
void __fastcall Vehicle_AnimateLinks(uint8_t* v, int, float step);
RT3_NOINLINE void __fastcall Vehicle_SpinWheels(uint8_t* v, int, float speed);
RT3_NOINLINE void __fastcall Vehicle_BlinkLights(uint8_t* v, int, float dt);
void __fastcall ModelObj_Save(uint8_t* o, int, Stream s);
void __fastcall Obj_FollowThis(uint8_t* o);
void Tooltip_Show1af();
int32_t Tooltip_Clear4566f0();
RT3_NOINLINE float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m);
RT3_NOINLINE float* __fastcall Vec3_Normalize(float* v);
RT3_NOINLINE void __fastcall Vec3_CrossInPlace(float* a, int, const float* b);
RT3_NOINLINE double __fastcall Vec3_Length(const float* v);
uint8_t* __fastcall NamedObj456900_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall Vehicle_LoadParts(uint8_t* v);
void __fastcall Map_RandomEdgeEntry(uint8_t* o, int, float* outX, float* outY, float* outZ, float* heading);
RT3_NOINLINE bool __fastcall Aircraft_Fly(uint8_t* v, int, float dt);
void __fastcall SceneObj_GetLoweredPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
RT3_NOINLINE void __fastcall Obj_DownVector(uint8_t* o, int, float* out);
RT3_NOINLINE void __fastcall Obj_OffsetVector(uint8_t* o, int, float* out);
RT3_NOINLINE void __fastcall Obj_ScaledPoint(uint8_t* o, int, float* out);
RT3_NOINLINE double __stdcall Vec3_AngleTerm(float a0, float a1, float a2, float b0, float b1, float b2);
RT3_NOINLINE float* __fastcall Obj_BendOffset(uint8_t* o, int, float* out, float ax, float ay, float az, float bx, float by, float bz);
RT3_NOINLINE void __fastcall Obj_AxisTerms(uint8_t* o, int, float vx, float vy, float vz, float* dot, float* lenC, float* lenA);
void __fastcall Obj_TakeCamera(uint8_t* o);
void __fastcall Obj_DropCamera(uint8_t* o);
int32_t __fastcall Vehicle_Load(uint8_t* v, int, int32_t s);
RT3_NOINLINE void __fastcall Vehicle_Init(uint8_t* v, int, uint32_t id, const uint32_t* type, int32_t atEdge, float x, float y, float z, float heading);
RT3_NOINLINE void __fastcall CameraObj_Teardown(uint8_t* o);
RT3_NOINLINE void __fastcall TrainObj_CabKeys(uint8_t* o, int, float dt);
RT3_NOINLINE bool __fastcall Vehicle_DriveUpdate(uint8_t* v, int, float dt);
void ObjFlags_Set();
void EntryWalk_Find();
uint8_t* __fastcall BlockIter_NextSameKey(uint8_t* b);
void Attach_SetValue();
void __stdcall Obj_SyncGroupFlags(uint8_t* o, int32_t full);
void __fastcall Obj_SwapBlockFlags(void*, int, uint8_t* o);
void Block_FinishSide();
void Block_CountSide();
void __fastcall BlockIter_SetFlags(uint8_t* o, uint32_t mask, int32_t mode);
int32_t __fastcall BlockIter_BeginTest(uint8_t* o, uint32_t mask, int32_t idx);
RT3_NOINLINE void Clear6acd60();
void __fastcall BlockIter_ClearSlot(uint8_t* o, int32_t idx);
void __fastcall BlockIter_MarkSlot(uint8_t* o, int32_t idx, int32_t flag);
void EntryWalk_Begin();
uint32_t __fastcall BlockIter_Begin(uint8_t* o);
void** __fastcall SList_Unlink(void** l, int, void** prev, void** node);
void Attach_SumVectors();
void __fastcall Attach_ReplaceObject(uint8_t* t, uint8_t* oldObj, uint8_t* newObj);
RT3_NOINLINE void __fastcall Attach_Animate(uint8_t* t, uint8_t* obj, int32_t now, int32_t mode);
void Attach_TickAll();
void* __fastcall Anim_LoadFile(const char* name);
void __fastcall EffectSet_Release(uint8_t* s);
RT3_NOINLINE void __fastcall ModelObj_Destructor(uint8_t* o);
int32_t __fastcall ModelObj_ResetState(uint8_t* o, int, int32_t full);
void __fastcall Vehicle_SetTint(uint8_t* v, int, uint32_t r, uint32_t g, uint32_t b);
void __fastcall Obj_SetColorRgb(uint8_t* o, int, uint32_t v);
void __fastcall Thunk45b110(uint8_t* p0, int p1, float p2, float p3, int32_t p4);
PtrListNode* __fastcall SceneObj_AddCamera(uint8_t* o, int, const char* name);
RT3_NOINLINE void __fastcall ModelObj_Release(uint8_t* o);
int32_t __fastcall Building_AttachSound(uint8_t* b, int, int32_t, uint8_t* grid, float x, float y, float z);
void __fastcall Obj_FreeEffects(uint8_t* o, int, int32_t force);
void __fastcall ModelObj_Select(uint8_t* o);
RT3_NOINLINE uint32_t __fastcall Obj_SetBlockFlags(uint8_t* o, int, uint32_t mask, int32_t mode);
RT3_NOINLINE int32_t __fastcall Obj_TestBlock(uint8_t* o, int, uint32_t mask, int32_t idx);
RT3_NOINLINE void __fastcall Obj_MarkBlockSlot(uint8_t* o, int, int32_t on, int32_t idx, int32_t mode);
RT3_NOINLINE int32_t __fastcall Obj_ClearAttachSlot(uint8_t* o, int, int32_t slot);
RT3_NOINLINE uint32_t __fastcall Obj_BeginBlocks(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_SaveNamedPair(uint8_t* o, int, Stream s);
void __fastcall Obj_AttachGroundSound(uint8_t* o, int, int32_t kind);
int32_t __fastcall Return0_45b670(void*);
uint8_t* __fastcall ModelObj_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall Building_LoadParts(uint8_t* o, int, int32_t upgraded);
RT3_NOINLINE void __fastcall Building_Init(uint8_t* o, int, const char* name, const char* tex, float x, float z, int32_t place, int32_t a6, int32_t a7, uint16_t a8);
void __fastcall SceneObj_AmbientSound(uint8_t* o);
void __fastcall SelectMarker_Update(uint8_t* o, int, int32_t now);
RT3_NOINLINE int32_t __fastcall Building_Load(uint8_t* o, int, int32_t s);
RT3_NOINLINE void __fastcall Building_AddUpgradeModel(uint8_t* o);
RT3_NOINLINE void __fastcall BuildingObj_DropModel(uint8_t* o);
RT3_NOINLINE void __fastcall PartModel_Destructor(uint8_t* o);
RT3_NOINLINE void __fastcall Model_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Model_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
RT3_NOINLINE void __fastcall Model_SetPartAngle2(uint8_t* o, int, int32_t part, uint32_t v);
void __fastcall Obj_TurnPivot(uint8_t* o, int, float angle, int32_t a, int32_t b);
RT3_NOINLINE double __fastcall Obj_Float242(void* o, int, int32_t i);
RT3_NOINLINE void __fastcall Car_Save(uint8_t* c, int, Stream s);
void __fastcall Model_SetAttachmentsFlag(uint8_t* o, int, int32_t on);
uint8_t* __fastcall PartModel_Construct(uint8_t* o);
RT3_NOINLINE void __fastcall Model_SetOpacity(uint8_t* m, int, float a);
RT3_NOINLINE void __fastcall Vehicle_RollWheels(uint8_t* v, int, float dist);
RT3_NOINLINE void __fastcall Car_SetCrashed(uint8_t* c, int, int32_t on);
RT3_NOINLINE void __fastcall Car_SetBrokenDown(uint8_t* c, int, int32_t on);
void __fastcall Model_SetPartFrame(uint8_t* o, int, int32_t part, int32_t frame);
int32_t __fastcall Vehicle_AttachEngineSound(uint8_t* o, int, const char* typeName, uint8_t* grid, float x, float y, float z);
RT3_NOINLINE void __fastcall Vehicle_SelectSound(uint8_t* v, int, int32_t kind);
int32_t __fastcall Vehicle_SetEngineSound(uint8_t* v, int, float scale, uint32_t volume);
void __fastcall Vehicle_UpdateHorn(uint8_t* v, int, int32_t);
RT3_NOINLINE void __fastcall Car_LoadModel(uint8_t* c, int, const char* name, const char* tex, float x, float z, int32_t reset);
RT3_NOINLINE uint32_t __fastcall Effects_SpawnForModel(uint8_t* parent, int, const char* prefix, uint8_t** out, int32_t count, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Effect_Free(uint32_t* e);
RT3_NOINLINE void __fastcall Loading_Pump(uint8_t* l, int, int32_t step, int32_t force);
RT3_NOINLINE void __fastcall SceneObj_SetTint(uint8_t* o, int, uint32_t r, uint32_t g, uint32_t b, int32_t deep);
RT3_NOINLINE void __fastcall Timer_SetBudget(uint8_t* t, int, uint32_t v);
RT3_NOINLINE void __fastcall Emitter_Tick(uint8_t* e, int, int32_t force);

static inline double ExeSqrt(double x) { return ((double(__cdecl*)(double))0x5a1634)(x); }

// Flags entry of `blk` in the current block iteration (0x6acd50), then calls
// its virtual slot 4 (flag 0x800 set) or 3 (unless 0x2000).
static void UpdateBlockEntry(uint8_t* blk, bool set) {
    uint8_t* it = g_6acd50;
    uint8_t* e = it + 0x10;
    for (int32_t i = 0; i < F<int32_t>(it, 0xc); ++i, e += 0xc) {
        if (F<uint8_t*>(e, 0) != blk) continue;
        uint32_t& f = F<uint32_t>(e, 4);
        f = set ? (f | 0x800) : (f & ~0x800u);
        if (f & 0x800) ((void(__thiscall*)(void*))(*(void***)blk)[0x10 / 4])(blk);
        else if (!(f & 0x2000)) ((void(__thiscall*)(void*))(*(void***)blk)[0xc / 4])(blk);
        return;
    }
}

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// Model parts: 32 entries of {flag byte, object*} at +0x276 (5 bytes each).
static inline uint8_t* PartObj(uint8_t* o, int32_t i) { return F<uint8_t*>(o, 0x277 + i * 5); }

static inline uint8_t PartFlag(uint8_t* o, int32_t i) { return o[0x276 + i * 5]; }

static inline void Show(uint8_t* p) { ((void(__thiscall*)(void*))(*(void***)p)[0xc / 4])(p); }

static inline void Hide(uint8_t* p) { ((void(__thiscall*)(void*))(*(void***)p)[0x10 / 4])(p); }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

namespace {
inline void* ExeNew_105(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
inline int32_t PackHas(const char* path) { return NameTable_ContainsName(g_pack, 0, path, 0, nullptr); }
// A scene object of model `path` (texture `tex`), or none when out of memory.
inline uint8_t* NewSceneObj(const char* path, const char* tex) {
    uint8_t* m = (uint8_t*)ExeNew_105(0x206);
    return m ? SceneObj_Construct(m, 0, 0, path, tex) : nullptr;
}
// A light of model `path` on `owner`, shown (flags 0x10).
inline uint8_t* NewLight(const char* path, uint8_t* owner) {
    uint8_t* m = (uint8_t*)ExeNew_105(0x35);
    uint8_t* c = m ? Camera_Construct(m, 0, path, (uint32_t)(uintptr_t)owner) : nullptr;
    return c;
}
inline void ShowLight(uint8_t* c) { c[0x24] = (uint8_t)((c[0x24] & 0xf) | 0x10); }
inline uint8_t* NewSound(const char* path, uint8_t* v, float vol) {
    uint8_t* m = (uint8_t*)ExeNew_105(0x141);
    return m ? Channel_Setup(m, 0, path, g_map, F<float>(v, 0x1e2), F<float>(v, 0x1e6), F<float>(v, 0x1ea), -1,
                             0, vol)
             : nullptr;
}
inline uint8_t* NewExhaust(const char* path, uint32_t color, int32_t perPoint, const float* rect, uint8_t* v, uint32_t flags,
                           uint32_t fade) {
    uint8_t* m = (uint8_t*)ExeNew_105(0x1fd);
    uint8_t* e = m ? Emitter_Construct(m, 0, path, color, perPoint, rect, v, flags) : nullptr;
    F<uint32_t>(e, 0x88) = 0;
    F<uint32_t>(e, 0x8c) = 0;
    F<uint32_t>(e, 0x90) = 0;
    F<uint32_t>(e, 0x98) = fade;
    Timer_SetBudget(e, 0, 0x3c);
    Camera_AddListener(v, 0, e);
    return e;
}
}

static inline float FromBits(uint32_t u) { return *(float*)&u; }

// Applies o's 4 element flags (+0xf4) to the group of each id in ids (up to 4,
// 0-terminated) looked up in the entry list 0x6acd50 ({id, flags, group}, +0xc
// entries); entries with flag 0x20000 only have it cleared.
static void SyncGroups(uint8_t* o, const uint32_t* ids, int32_t full, int32_t modeIfNotFull) {
    for (int32_t k = 0; k < 4; ++k) {
        uint32_t id = ids[k];
        if (!id) return;
        uint8_t* list = g_6acd50;
        int32_t n = F<int32_t>(list, 0xc);
        uint8_t** group = nullptr;
        for (int32_t i = 0; i < n; ++i)
            if (F<uint32_t>(list, 0x10 + i * 0xc) == id) {
                group = (uint8_t**)(list + 0x18 + i * 0xc);
                break;
            }
        uint32_t* flags = nullptr;
        for (int32_t i = 0; i < n; ++i)
            if (F<uint32_t>(list, 0x10 + i * 0xc) == id) {
                flags = (uint32_t*)(list + 0x14 + i * 0xc);
                break;
            }
        if (*flags & 0x20000) {
            *flags &= ~0x20000u;
            continue;
        }
        for (int32_t s = 0; s < 4; ++s) {
            int32_t r = Obj_EntryClear1d8(*group, s);
            if (!o[0xf4 + s]) continue;
            int32_t mode;
            if (!full) {
                Group_ResetLists(*group);
                mode = modeIfNotFull;
            } else {
                if (!r && F<int32_t>(*group, 8) > s) Group_ResetLists(*group);
                mode = !r ? 1 : 0;
            }
            Obj_SetElementFlag1d8(*group, mode, s);
        }
    }
}

static inline uint8_t* Packs() { return g_pack; }

static inline void Format0(char* dst, uint32_t fmt, const char* a1 = nullptr) {
    FormatText(dst, (const char*)fmt, a1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

// Loads a whole pack file by id into a new buffer (0 if empty).
static void* LoadWhole(int32_t id, int32_t* sizeOut) {
    int32_t size = NameTable_EntrySize(Packs(), 0, id);
    *sizeOut = size;
    if (size <= 0) return nullptr;
    NameTable_Seek(Packs(), 0, id, 0);
    void* p = Malloc(size);
    NameTable_Read(Packs(), 0, p, size);
    return p;
}

static inline uint8_t* Snd() { return g_sound; }

static inline double D(uint32_t a) { return *(const double*)a; }

static inline float Fl(uint32_t a) { return *(const float*)a; }

static inline double Fabs(double x) { return ((double(__cdecl*)(double))0x5a152e)(x); }

static inline uint8_t* Map() { return g_map; }

namespace {
void BuildingClear(uint8_t* o) {
    F<uint32_t>(o, 0x246) = 0;
    F<uint32_t>(o, 0x24a) = 0;
    F<int32_t>(o, 0x262) = -1;
    F<uint32_t>(o, 0x266) = 0;
    F<uint32_t>(o, 0x26a) = 0;
    F<uint16_t>(o, 0x23b) = 0;
    o[0x23a] = 0;
    o[0x23d] = 0;
}
}

static inline uint8_t* NewChannel() { return (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x141); }

static inline uint32_t U(float f) { return *(uint32_t*)&f; }

static inline double ExeCos(double v) { return ((double(__cdecl*)(double))0x5a13e4)(v); }

static inline double ExeSin(double v) { return ((double(__cdecl*)(double))0x5a1494)(v); }

static inline double ExeFabs(double v) { return ((double(__cdecl*)(double))0x5a152e)(v); }

static inline double Fl_188(uint32_t a) { return *(const float*)a; }

static inline float SnapInt(float v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return (float)r;
}

namespace {
inline int32_t CarHas(const char* name, const char* suffix = nullptr) { return NameTable_ContainsName(g_pack, 0, name, 0, suffix); }
inline const char* CarNum(char* buf, int32_t n) { return Num_Format(buf, 1, (double)n, 10, 1, 0, 0); }
inline void CarFormat(char* dst, uint32_t fmt, const char* a1, const char* a2 = nullptr) {
    FormatText(dst, (const char*)fmt, a1, a2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}
inline uint8_t* CarModel(const char* name, const char* model, const char* tex) {
    uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0x23a);
    return p ? WorldModel_Construct(p, 0, name, model, tex, 0xbf800000, 0xbf800000, 1) : nullptr;
}
// The current mesh frame record of a scene object (null without a mesh).
inline uint8_t* CarFrame(uint8_t* o) {
    uint8_t* m = F<uint8_t*>(o, 0x14);
    return m ? *(uint8_t**)(m + 0xc + F<int32_t>(o, 0x3a) * 0x18) + F<int32_t>(o, 0x21) * 0x45 : nullptr;
}
inline double CarAtan(double v) { return ((double(__cdecl*)(double))0x5a1270)(v); }
// Reads the first two frame points of a helper model `fmt` (+8 of each) into
// *id and *len (|integer| of the second); none when the file is missing.
void CarHelperPoint(char* path, const char* name, uint32_t fmt, uint32_t* id, float* len) {
    CarFormat(path, fmt, name);
    if (!CarHas(path)) return;
    uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0x206);
    uint8_t* o = p ? SceneObj_Construct(p, 0, 0, path, nullptr) : nullptr;
    uint8_t* fr = CarFrame(o);
    *id = F<uint32_t>(*(uint8_t**)fr, 8);
    int32_t n;
    float f = F<float>(*(uint8_t**)(fr + 0x45), 8);
    __asm {
        fld f
        mov eax, 0x5a10d0
        call eax
        mov n, eax
    }
    *len = (float)(n < 0 ? -n : n);
    SceneObj_Destructor(o);
    ((void(__cdecl*)(void*))0x53b080)(o);
}
}

namespace {
// Marks the car's emitters (+0x10.., up to a null) +0x1d8.
void CarMarkEmitters(uint8_t* c, uint8_t on) {
    for (uint8_t** e = (uint8_t**)(F<uint8_t*>(c, 0x316) + 0x10); *e; ++e) (*e)[0x1d8] = on;
}
// The effect size of car c: its frame's +0x14 x 0x5cb660; flag 0x10000 unless
// it was drawn lately.
float CarEffectSize(uint8_t* c, uint32_t* flags) {
    float size = (float)((double)F<float>(CarFrame(c), 0x14) * Fl_188(0x5cb660));
    *flags = SceneObj_RecentlyDrawn(c) ? 0 : 0x10000;
    return size;
}
}

static inline double Fl_198(uint32_t a) { return *(const float*)a; }

static void QuietEmitter(uint8_t* e, int32_t a, uint32_t e5) {
    View_SetParams(e, 0, a, 100, 0x3f333333, 0x3e4ccccd, e5, 0x3f000000);
    for (int32_t k = 0; k < 6; ++k) F<int32_t>(e, 0x70 + k * 4) = 0;
}

static inline double Fl_205(uint32_t a) { return *(const float*)a; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline double Fl_207(uint32_t a) { return *(const float*)a; }

// Part of 0x454f40 (Ghidra's body includes the tail-jump target 0x454630):
// (int)((base + off) * 92 * 100 + 0.5) / 100 with the x87 order and the exe's
// _ftol.
static float TreeTypes_Coord(float base, float off) {
    int32_t n;
    __asm {
        fld base
        fadd off
        fmul dword ptr ds:[0x5c8700]
        fmul dword ptr ds:[0x5c8c28]
        fadd qword ptr ds:[0x5c8628]
        mov eax, 0x5a10d0
        call eax
        mov n, eax
    }
    return (float)((double)n / *(const double*)0x5c85d8);
}

// 0x454630 (reached only by the tail jump of 0x454f40). Once (0x6acd20[0]),
// with the sim seed 0x11 pushed, builds four tables 0x6acd20[0..3] of 16 x 16
// cells x 6 tree spots (9 bytes: x, y in tile units, flags bit 0 / bit 1 for
// the right / lower half). Each spot jitters a base pattern (all centred;
// two rows of three; three columns of two; a 2 x 2 grid) by +-0.5 / +-0.05,
// 0.015 / +-0.04 / +-0.005 and is redrawn while it is closer than 20.24
// (shrinking by 0.92 every 32 tries) to an earlier spot of its cell or of an
// already-built neighbour cell (wrapping at 16 cells = 1472 units). QUIRK: a
// neighbour wrapped across the right edge is shifted in y, not x.
static void TreeTypes_BuildSpots() {
    if (g_6acd20) return;
    PushSeed();
    SetSeed(0x11);
    static const uint32_t kBase[4][12] = {
        { 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000, 0x3f000000 },
        { 0x3e800000, 0x3e23d70a, 0x3f400000, 0x3f000000, 0x3e800000, 0x3f547ae1, 0x3f400000, 0x3e23d70a, 0x3e800000, 0x3f000000, 0x3f400000, 0x3f547ae1 },
        { 0x3e23d70a, 0x3e800000, 0x3f000000, 0x3f400000, 0x3f547ae1, 0x3e800000, 0x3e23d70a, 0x3f400000, 0x3f000000, 0x3e800000, 0x3f547ae1, 0x3f400000 },
        { 0x3e800000, 0x3e800000, 0x3f400000, 0x3f400000, 0x3e800000, 0x3f400000, 0x3f400000, 0x3e800000, 0, 0, 0, 0 },
    };
    static const int32_t kJitter[4][2] = { { 500, 500 }, { 50, 15 }, { 40, 40 }, { 5, 5 } };
    Loading_Pump(g_app, 0, 1, 0);
    const int32_t* ndx = (const int32_t*)0x624b28;
    const int32_t* ndy = (const int32_t*)0x624b48;
    for (int32_t g = 0; g < 4; ++g) {
        uint8_t* tbl = (uint8_t*)Malloc(0x3600);
        ((uint8_t**)0x6acd20)[g] = tbl;
        memset(tbl, 0, 0x3600);
        float px[6] = {}, py[6] = {};
        const float* base = (const float*)kBase[g];
        for (int32_t j = 0; j < 0x10; ++j) {
            Loading_Pump(g_app, 0, 0, 0);
            float jf = (float)j;
            for (int32_t i = 0; i < 0x10; ++i) {
                float fi = (float)i;
                for (int32_t p = 0; p < 6; ++p) {
                    uint8_t* rec = tbl + ((j * 0x10 + i) + p * 0x100) * 9;
                    float minD = 20.24f;
                    uint32_t tries = 0;
                    float dx, dy;
                    for (;;) {
                        int32_t ax = kJitter[g][0], ay = kJitter[g][1];
                        dx = (float)((double)SimRand(-ax, ax) / *(const double*)0x5c87f8);
                        dy = (float)((double)SimRand(-ay, ay) / *(const double*)0x5c87f8);
                        dx = dx + base[p * 2];
                        dy = dy + base[p * 2 + 1];
                        F<float>(rec, 0) = TreeTypes_Coord(fi, dx);
                        F<float>(rec, 4) = TreeTypes_Coord(jf, dy);
                        bool ok = true;
                        for (int32_t s = 0; s < p && ok; ++s)
                            if ((float)Distance2D(F<float>(rec, 0), F<float>(rec, 4), px[s], py[s]) < minD) ok = false;
                        for (int32_t k = 0; k < 8 && ok; ++k) {
                            int32_t ni = (ndx[k] + i + 0x10) % 0x10, nj = (ndy[k] + j + 0x10) % 0x10;
                            if (!(nj < j || (nj == j && ni < i))) continue;
                            int32_t cell = nj * 0x10 + ni;
                            for (int32_t q = 0; q < 6 && ok; ++q, cell += 0x100) {
                                const uint8_t* n = tbl + (cell < 0 || cell >= 0x600 ? 0 : cell) * 9;
                                float nx = F<float>((void*)n, 0), ny = F<float>((void*)n, 4);
                                const float wrap = *(const float*)0x5cadc4;
                                if (ni == 0 && i == 0xf) ny = ny + wrap;
                                else if (i == 0 && ni == 0xf) nx = nx - wrap;
                                if (nj == 0 && j == 0xf) ny = ny + wrap;
                                else if (j == 0 && nj == 0xf) ny = ny - wrap;
                                if ((float)Distance2D(F<float>(rec, 0), F<float>(rec, 4), nx, ny) < minD) ok = false;
                            }
                        }
                        if (ok) break;
                        if ((++tries & 0x1f) == 0) minD = (float)(minD - *(const double*)0x5c8890);
                    }
                    px[p] = F<float>(rec, 0), py[p] = F<float>(rec, 4);
                    rec[8] = 0;
                    if (!(dx < *(const double*)0x5c8628)) ++rec[8];
                    if (!(dy < *(const double*)0x5c8628)) rec[8] += 2;
                }
            }
        }
    }
    PopSeed();
}

namespace {
// Vector and angle arguments the original copies with MOV (bit for bit).
typedef void(__fastcall* RotateBitsFn)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float);
typedef float*(__fastcall* BendBitsFn)(uint8_t*, int, float*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef double(__stdcall* AngleBitsFn)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
typedef float*(__fastcall* CrossBitsFn)(uint8_t*, int, float*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, float*);
typedef void(__fastcall* AxisBitsFn)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float*, float*, float*);
inline uint32_t Bits4588(float f) { return *(uint32_t*)&f; }
inline float Fl4588(uint32_t a) { return *(const float*)a; }
inline double D4588(uint32_t a) { return *(const double*)a; }
// 0x458140 stays the original (it leaves stack residue its callers read).
inline float* WorldCross(uint8_t* v, float* out, const float* a, const float* b, float* len) {
    const uint32_t* ab = (const uint32_t*)a;
    const uint32_t* bb = (const uint32_t*)b;
    return ((CrossBitsFn)0x458140)(v, 0, out, ab[0], ab[1], ab[2], bb[0], bb[1], bb[2], len);
}
inline void RotateBits(uint8_t* v, uint32_t axis, float angle) {
    const uint32_t* a = (const uint32_t*)(v + axis);
    ((RotateBitsFn)&SceneObj_RotateAxis)(v, 0, a[0], a[1], a[2], angle);
}
// The status line (0x1ad / 0x1ae) in the top tenth of the screen.
void DriveStatus(char* text, uint32_t width) {
    if (uint8_t* font = F<uint8_t*>(g_windowManager, 0xc))
        Font_DrawTextBox(font, 0, text, 0.1f, 0.0f, *(float*)&width, 1.0f, 0xffffffff, 3, 1, 0, 1.0f);
}
// Common end of both drive models: draw, reset the view, follow with the
// chase camera +0x352 (target ahead by dist, eye behind by far).
void DriveCamera(uint8_t* v) {
    F<int32_t>(g_app, 0xa0) = 1;
    F<int32_t>(g_engine, 0x114216) = 1;
    Camera_SetBounds(g_62be68, 0, 0, 0, 0x3f800000, 0x3f800000);
    F<int32_t>(Engine_Renderer(hook::As<void*>(g_engine)), 0x25) = 0;
}
}

namespace {
void Callback_Delete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
int32_t Tooltip_Clear() { return Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1); }
// A network message's handler gets the packet (+8) when it has one (+0xc).
template <typename P, typename Fn> int32_t NetMsg_Forward(uint8_t* m, Fn handler) {
    if (F<int32_t>(m, 0xc) > 0) handler(F<P>(m, 8));
    return 1;
}
}

// 0x459bc0 body. Sets (clear == 0) or clears the flag bits mask of obj's entry in
// the table 0x6acd50 (count +0xc, entries of { obj, flags, - } at +0x10), then
// notifies obj: method 0x10 when bit 0x800 is set, else method 0xc unless bit
// 0x2000 is. Returns what the original leaves in EAX.
static uint32_t __cdecl ObjFlags_SetImpl(uint32_t mask, uint8_t* obj, int32_t clear) {
    uint8_t* t = g_6acd50;
    int32_t n = F<int32_t>(t, 0xc);
    uint8_t* e = t + 0x10;
    if (n <= 0) return (uint32_t)(uintptr_t)e;
    int32_t i = 0;
    for (; i < n; ++i, e += 0xc)
        if (F<uint8_t*>(e, 0) == obj) break;
    if (i == n) return (uint32_t)(uintptr_t)e;
    uint32_t flags = F<uint32_t>(e, 4);
    flags = clear ? flags & ~mask : flags | mask;
    F<uint32_t>(e, 4) = flags;
    if (flags & 0x800) return ((uint32_t(__fastcall*)(uint8_t*))(*(void***)obj)[0x10 / 4])(obj);
    if (!(flags & 0x2000)) return ((uint32_t(__fastcall*)(uint8_t*))(*(void***)obj)[0xc / 4])(obj);
    return flags;
}

// 0x45a5f0 body. Starts walking the entry table t (globals 0x6acd44..0x6acd54,
// end pointer 0x6acd4c) and clears the first entry with an object (+8), passing on
// the caller's EDX.
static int32_t __cdecl EntryWalk_BeginImpl(uint8_t* t, int32_t edx) {
    g_6acd44 = 0;
    g_6acd48 = 0;
    g_6acd54 = 0;
    g_6acd50 = t;
    int32_t n = F<int32_t>(t, 0xc);
    g_6acd4c = t + (n > 0 ? n * 0xc + 0x10 : 0x10);
    uint8_t* e = t + 0x10;
    for (int32_t i = 0; i < n; ++i, e += 0xc)
        if (F<int32_t>(e, 8)) return Obj_EntryClear1d8(F<uint8_t*>(e, 8), edx);
    return 0;
}

// 0x459c30 body. Starts walking table t for obj (globals as in 0x45a5f0): records
// obj's entry (+0x6acd48 flags pointer, +0x6acd44 value). 1 when found or obj is
// null (then the end pointer 0x6acd4c is set), else 0.
static int32_t __cdecl EntryWalk_FindImpl(uint8_t* obj, uint8_t* t) {
    g_6acd44 = 0;
    g_6acd48 = 0;
    hook::As<uint8_t*>(g_6acd54) = obj;
    g_6acd50 = t;
    hook::As<int32_t>(g_6acd4c) = 0;
    int32_t found = 0;
    uint32_t end = 0x10;
    if (F<int32_t>(t, 0xc) > 0) {
        uint8_t* e = t + 0x10;
        for (int32_t i = 0; i < F<int32_t>(t, 0xc); ++i, e += 0xc, end += 0xc)
            if (obj && F<uint8_t*>(e, 0) == obj) {
                hook::As<uint8_t*>(g_6acd48) = e + 4;
                g_6acd44 = F<int32_t>(e, 8);
                found = 1;
            }
        if (found == 1) {
            g_6acd4c = t + end;
            return 1;
        }
    }
    if (obj) return found;
    g_6acd4c = t + end;
    return 1;
}

// 0x45a700 body. Sums the scaled vectors of obj's attachments in the current walk
// table (from 0x6acd4c, count +8 of 0x6acd50, 0x168-byte entries; entries flagged
// 0x400 in +0x8c skipped): integer vector +0x34 times float vector +4 scaled by
// 0x5cb4ac for mode 0, else +0x70 times +0x40; into out.
static void __cdecl Attach_SumVectorsImpl(float* out, int32_t mode, int32_t obj) {
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    uint8_t* e = g_6acd4c;
    float scale = 1.0f;
    int32_t* iv;
    float* fv;
    if (mode == 0) {
        scale = *(float*)0x5cb4ac;
        iv = (int32_t*)(e + 0x34);
        fv = (float*)(e + 4);
    } else {
        iv = (int32_t*)(e + 0x70);
        fv = (float*)(e + 0x40);
    }
    for (int32_t i = 0; i < F<int32_t>(g_6acd50, 8); ++i) {
        if (F<int32_t>(e, 0) == obj && !(F<uint32_t>(e, 0x8c) & 0x400)) {
            float iz = (float)iv[2], iy = (float)iv[1], ix = (float)iv[0];
            float z = (float)((double)iz * fv[2] * scale);
            float y = (float)((double)iy * fv[1] * scale);
            float x = (float)((double)ix * fv[0] * scale);
            out[0] = (float)((double)x + out[0]);
            out[1] = (float)((double)y + out[1]);
            out[2] = (float)((double)z + out[2]);
        }
        iv = (int32_t*)((uint8_t*)iv + 0x168);
        fv = (float*)((uint8_t*)fv + 0x168);
        e += 0x168;
    }
}

// 0x459d00 body. For each of obj's up to 4 attachment ids (+0x98, 0-ended), sets
// value (+0x80) and v2 (+0x94) and clears flag 2 on the matching entries of the
// current walk table (from 0x6acd4c). An entry flagged 0x10 is only set (and ends
// the search) when no later entry with the id is still unflagged by 2. 1 when a
// plain entry was set for an obj flagged 0x10 (+0x8c).
static int32_t __cdecl Attach_SetValueImpl(int32_t value, uint8_t* obj, int32_t v2) {
    int32_t result = 0;
    for (int32_t k = 0; k < 4; ++k) {
        int32_t id = F<int32_t>(obj, 0x98 + k * 4);
        if (!id) break;
        uint8_t* e = g_6acd4c;
        for (int32_t i = 0; i < F<int32_t>(g_6acd50, 8); ++i, e += 0x168) {
            if (id != F<int32_t>(e, 0)) continue;
            uint32_t f = F<uint32_t>(e, 0x8c);
            if (f & 0x10) {
                int32_t n = F<int32_t>(g_6acd50, 8);
                bool later = false;
                uint8_t* e2 = e + 0x168;
                for (int32_t j = i + 1; j < n; ++j, e2 += 0x168)
                    if (F<int32_t>(e2, 0) == id && !(F<uint32_t>(e2, 0x8c) & 2)) {
                        later = true;
                        break;
                    }
                if (!later) {
                    F<uint32_t>(e, 0x8c) &= ~2u;
                    F<int32_t>(e, 0x80) = value;
                    F<int32_t>(e, 0x94) = v2;
                }
                break;
            }
            F<uint32_t>(e, 0x8c) = f & ~2u;
            F<int32_t>(e, 0x80) = value;
            F<int32_t>(e, 0x94) = v2;
            if (obj[0x8c] & 0x10) result = 1;
        }
    }
    return result;
}

// 0x45a0d0 body. Finishes one side (1: the +0x64 group, else +0x28) of a fully
// flagged (0x380) block entry o: drops the flags whose side counters are set,
// clears the counters, flips the direction (+0x88 / +0x84) of a reversing entry
// (bit 1), then syncs groups (0x4000), marks it taken (2), passes value and
// level +0x90 * 100 on to its partner (0x10) or itself, and to attachments
// (0x20), swaps block flags (0x1000) and notifies the table owner (0x800). Bit 0
// of the result: this entry (or the earlier partner) took the level; bit 1: an
// attachment took it.
static int32_t __cdecl Block_FinishSideImpl(uint8_t* o, int32_t side) {
    uint32_t flags = F<uint32_t>(o, 0x8c);
    int32_t result = 0;
    if ((flags & 0x380) != 0x380) return 0;
    int32_t level = F<int32_t>(o, 0x90) * 100;
    if (side == 1) {
        if (F<int32_t>(o, 0x70)) F<uint32_t>(o, 0x8c) = flags & ~0x80u;
        if (F<int32_t>(o, 0x74)) F<uint32_t>(o, 0x8c) &= ~0x100u;
        if (F<int32_t>(o, 0x78)) F<uint32_t>(o, 0x8c) &= ~0x200u;
        flags = F<uint32_t>(o, 0x8c);
        F<int32_t>(o, 0x64) = 0;
        F<int32_t>(o, 0x68) = 0;
        F<int32_t>(o, 0x6c) = 0;
        if (flags & 1) F<int32_t>(o, 0x88) = -F<int32_t>(o, 0x88);
    } else {
        if (F<int32_t>(o, 0x34)) F<uint32_t>(o, 0x8c) = flags & ~0x80u;
        if (F<int32_t>(o, 0x38)) F<uint32_t>(o, 0x8c) &= ~0x100u;
        if (F<int32_t>(o, 0x3c)) F<uint32_t>(o, 0x8c) &= ~0x200u;
        flags = F<uint32_t>(o, 0x8c);
        F<int32_t>(o, 0x28) = 0;
        F<int32_t>(o, 0x2c) = 0;
        F<int32_t>(o, 0x30) = 0;
        if (flags & 1) F<int32_t>(o, 0x84) = -F<int32_t>(o, 0x84);
    }
    if (flags & 0x4000) Obj_SyncGroupFlags(o, (int32_t)(flags & 0x10000));
    if (F<uint32_t>(o, 0x8c) & 4) F<uint32_t>(o, 0x8c) |= 2;
    flags = F<uint32_t>(o, 0x8c);
    if (flags & 0x10) {
        F<uint32_t>(o, 0x8c) = flags | 2;
        uint8_t* q = BlockIter_NextSameKey(o);
        F<int32_t>(q, 0x94) = level;
        F<int32_t>(q, 0x80) = F<int32_t>(o, 0x80);
        if (q < o) {
            if (!(F<uint32_t>(q, 0x8c) & 0x40)) F<uint32_t>(q, 0x8c) &= ~2u;
            result = 1;
        } else F<uint32_t>(q, 0x8c) &= ~2u;
    } else {
        if (!(flags & 1)) result = 1;
        F<int32_t>(o, 0x94) = level;
    }
    if ((F<uint8_t>(o, 0x8c) & 0x20) && Attach_SetValueImpl(F<int32_t>(o, 0x80), o, level)) result |= 2;
    if (F<uint32_t>(o, 0x8c) & 0x1000) Obj_SwapBlockFlags(nullptr, 0, o);
    if (F<uint32_t>(o, 0x8c) & 0x800) ObjFlags_SetImpl(0x800, hook::As<uint8_t*>(g_6acd54), 0);
    return result;
}

// 0x45a280 body. One side of entry e (mask: its flag 0x80 / 0x100 / 0x200): while
// counter c (+0x24) is below its limit (+0x30) the flag is cleared and c counts up,
// and value passes through; at the limit the flag is set (once, finishing the
// side into *result) and 0 comes back. A null c passes value through. Returns the
// float's bits, which the entry stores back over value.
static uint32_t __cdecl Block_CountSideImpl(uint8_t* c, uint8_t* e, uint32_t mask, uint32_t* result, uint32_t value, int32_t side) {
    if (!c) return value;
    if (F<int32_t>(c, 0x24) < F<int32_t>(c, 0x30)) {
        F<uint32_t>(e, 0x8c) &= ~mask;
        ++F<int32_t>(c, 0x24);
        return value;
    }
    uint32_t flags = F<uint32_t>(e, 0x8c);
    if (!(flags & mask)) {
        F<uint32_t>(e, 0x8c) = flags | mask;
        *result |= (uint32_t)Block_FinishSideImpl(e, side);
    }
    return 0;
}

// 0x454010. Raises (amount > 0) or lowers the terrain level (cell byte % 7, 0..6)
// of the tiles inside the circle by a random rounding of |amount|, keeping the
// cell's other part (byte / 7); retries up to 30 times until some tile changed.
RT3_NOINLINE void __fastcall Terrain_AdjustLevel(uint8_t* m, int, float x, float y, float r, float amount) {
    int32_t x0, x1, y0, y1;
    float t = (float)((double)x - r - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    x0 = (int32_t)t;
    t = (float)((double)y - r - Fl(0x5c85b4));
    if (Fl(0x5c8598) > t) t = 0.0f;
    y0 = (int32_t)t;
    float mx = (float)(F<int32_t>(m, 0x2155) - 1);
    float u = (float)((double)x + r + Fl(0x5c85b4));
    if (mx < u) u = mx;
    x1 = (int32_t)u;
    float my = (float)(F<int32_t>(m, 0x2159) - 1);
    u = (float)((double)y + r + Fl(0x5c85b4));
    if (my < u) u = my;
    y1 = (int32_t)u;
    int32_t done = 0;
    for (int32_t tries = 30; tries; --tries) {
        if (done) continue;
        for (int32_t ty = y0; ty <= y1; ++ty)
            for (int32_t tx = x0; tx <= x1; ++tx) {
                if (!(r == Fl(0x5c8598))) {
                    float py = (float)((double)ty + D(0x5c8628)), px = (float)((double)tx + D(0x5c8628));
                    float d = (float)(((double)r - Distance2D(px, py, x, y)) / r);
                    if (d <= Fl(0x5c8598)) continue;
                }
                bool up = amount > Fl(0x5c8598);
                int32_t rnd = SimRand(0, 1000);
                double q = (Fabs(amount) * D(0x5c87f8) + (double)rnd) / D(0x5c87f8);
                int32_t v;
                __asm {
                    fld q
                    mov eax, 0x5a10d0
                    call eax
                    mov v, eax
                }
                if (v <= 0) continue;
                if (!up) v = -v;
                int32_t lvl = (uint8_t)Terrain_CellByte(m, 0, tx, ty) % 7 + v;
                if (lvl < 0) lvl = 0;
                else if (lvl > 6) lvl = 6;
                uint8_t b = (uint8_t)Terrain_CellByte(m, 0, tx, ty);
                Terrain_SetCellByte(m, 0, tx, ty, (uint8_t)(b / 7 * 7 + lvl));
                done = 1;
            }
    }
}
RT3_REPLACE(0x454010, Terrain_AdjustLevel);

// 0x454320. Paints terrain points (trees etc.) of kind (kind % 28) * 7 + variant in the
// circle: each tile gets one with chance falloff x density, unless the tile is
// occupied (low 3 bits) and not `force`; kinds marked 3 keep at most 4 per tile.
// With `erase` removes up to 6 per tile instead. Retries up to 30 passes until a
// tile was touched, then redraws.
RT3_NOINLINE void __fastcall Terrain_PaintPoints(uint8_t* t, int, float x, float y, float r, int32_t erase, int32_t kind,
                                    int32_t variant, float density, int32_t force) {
    int32_t x0, x1, y0, y1;
    float v = (float)((double)x - r);
    if (Fl(0x5c8598) > v) v = 0.0f;
    x0 = (int32_t)v;
    v = (float)((double)y - r);
    if (Fl(0x5c8598) > v) v = 0.0f;
    y0 = (int32_t)v;
    float mx = (float)(F<int32_t>(t, 0x2155) - 1);
    float u = (float)((double)x + r);
    if (mx < u) u = mx;
    x1 = (int32_t)u;
    float my = (float)(F<int32_t>(t, 0x2159) - 1);
    u = (float)((double)y + r);
    if (my < u) u = my;
    y1 = (int32_t)u;
    int32_t count = 0;
    for (int32_t tries = 30; tries; --tries) {
        if (count) continue;
        for (int32_t ty = y0; ty <= y1; ++ty)
            for (int32_t tx = x0; tx <= x1; ++tx) {
                float w;
                if (r == Fl(0x5c8598)) w = 1.0f;
                else {
                    float py = (float)((double)ty + D(0x5c8628)), px = (float)((double)tx + D(0x5c8628));
                    w = (float)(((double)r - Distance2D(px, py, x, y)) / r);
                    if (w <= Fl(0x5c8598)) continue;
                }
                float p = (float)((double)w * Fl(0x5cadc0));
                p = (float)((double)p * density);
                if (erase) {
                    ++count;
                    Terrain_DecCellCount(t, 0, tx, ty, 6);
                    continue;
                }
                float rnd = (float)SimRand(0, 10000);
                if ((double)p * Fl(0x5c88c0) <= rnd) continue;
                ++count;
                kind %= 0x1c;
                int32_t k = kind * 7 + variant;
                if (!Map_CellLow3(Map(), 0, tx, ty) || force) Terrain_AddPoints(t, 0, tx, ty, (uint8_t)k, 1);
                if (((const uint8_t*)0x688058)[k] == 3)
                    while ((int32_t)Terrain_CellCount(t, 0, tx, ty) > 4) Terrain_DecCellCount(t, 0, tx, ty, 1);
            }
    }
    void* g = hook::As<void*>(g_62be68);
    (*(void(__thiscall**)(void*))(*(void**)g))(g);
}
RT3_REPLACE(0x454320, Terrain_PaintPoints);

// 0x454d70. Recomputes the 4-bit mask of block (x, y): bit a + 2b set when the
// 4x4 quarter (a, b) of fine cells (x*4-1+2a.., y*4-1+2b..) has a cell in either
// bit grid (+0x2139 / +0x213d); stores it with Terrain_SetFlag and flags the renderer.
// Quirk: the far edge is clamped only when the near edge is already past it.
RT3_NOINLINE void __fastcall Map_UpdateBlockMask(uint8_t* m, int, int32_t x, int32_t y) {
    int32_t w = F<int32_t>(m, 0x215d);
    int32_t hMax = F<int32_t>(m, 0x2161) - 1;
    uint8_t mask = 0;
    for (int32_t a = 0; a < 2; ++a) {
        int32_t sx = x * 4 - 1 + 2 * a;
        for (int32_t b = 0; b < 2; ++b) {
            int32_t sy = y * 4 - 1 + 2 * b;
            int32_t x0 = sx < 0 ? 0 : sx, x1 = sx + 3;
            int32_t y0 = sy < 0 ? 0 : sy, y1 = sy + 3;
            if (x0 > w - 1) x1 = w - 1;
            if (y0 > hMax) y1 = hMax;
            for (int32_t i = x0; i <= x1; ++i) {
                const uint8_t* g0 = F<uint8_t*>(m, 0x2139);
                for (int32_t j = y0; j <= y1; ++j) {
                    int32_t idx = w * j + i;
                    uint8_t bit = (uint8_t)(1 << (idx & 7));
                    if ((g0[idx >> 3] & bit) || (F<uint8_t*>(m, 0x213d)[idx >> 3] & bit)) {
                        mask |= (uint8_t)(1 << (a + 2 * b));
                        goto next;
                    }
                }
            }
        next:;
        }
    }
    Terrain_SetFlag(m, 0, x, y, mask);
    F<uint8_t>(Engine_Renderer(hook::As<void*>(g_engine)), 0x374a) = 1;
}
RT3_REPLACE(0x454d70, Map_UpdateBlockMask);

// 0x454f00. Updates the block mask of every cell in [x0, x1] x [y0, y1].
void __fastcall Map_UpdateBlockMaskRect(uint8_t* m, int, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    for (int32_t y = y0; y <= y1; ++y)
        for (int32_t x = x0; x <= x1; ++x) Map_UpdateBlockMask(m, 0, x, y);
}
RT3_REPLACE(0x454f00, Map_UpdateBlockMaskRect);

// 0x454f40. Renderer r loads the tree types once (+0x2169): it fills the
// 196-byte tile vegetation map 0x688058 from the climate table (0x5ef3e4,
// every 7th tile per 0xb0 record), clears the 35 species records (0x5ee8e0,
// 0x45 bytes: name, model count +0x10, per size +0x14 counts and +0x1b three
// model ids), registers the "Trees" (+0x1689, also +0x168d) and "Stumps"
// (result dropped) texture slots at texture quality 2, then reads
// data\TreeTypes\TreeTypes.txt ("%s %c %f": model, letter, value; at most
// 300). Each model NAME_<size> loads NAME_Body.3dp as a scene object,
// becomes a marker (letter A.. -> 1..) and is filed under species NAME (the
// first when unknown) and size digit. A missing file or a name without '_'
// is fatal (0x1a9 / 0x1aa). QUIRK: fscanf at end of file leaves the buffer,
// so a trailing newline repeats the last model. Ends with the vegetation
// tables (0x4539f0) and the tree spot tables (0x454630).
RT3_NOINLINE void __fastcall TreeTypes_Load(uint8_t* r) {
    if (F<int32_t>(r, 0x2169)) return;
    Loading_Pump(g_app, 0, 0, 0);
    F<int32_t>(r, 0x2169) = 1;
    F<int32_t>(r, 0x216d) = 0;
    for (int32_t i = 0; i < 0xc4; ++i) ((uint8_t*)0x688058)[i] = ((const uint8_t*)0x5ef3e4)[i / 7 * 0xb0];
    for (uint8_t* rec = (uint8_t*)0x5ee8e0; rec + 0x1b < (uint8_t*)0x5ef26a; rec += 0x45) {
        F<int32_t>(rec, 0x10) = 0;
        for (int32_t k = 0; k < 7; ++k) {
            rec[0x14 + k] = 0;
            F<int32_t>(rec, 0x1b + k * 6) = 0;
            F<uint16_t>(rec, 0x1f + k * 6) = 0;
        }
    }
    char path[0x12c], err[0x11c], model[0x80], species[0x64], name[0x40];
    char letter;
    uint32_t value;
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(path, (const char*)0x5cadf0);
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5c89e8);
    if (!f) {
        FormatText(err, GetString(0x1a9), path, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        Fatal_Exit(err);
    }
    uint8_t* pk = g_engine;
    int32_t quality = F<int32_t>(pk, 0x114736);
    F<int32_t>(pk, 0x114736) = 2;
    F<int32_t>(r, 0x1689) = TextureSlots_Add(r, 0, (const char*)0x5cade8);
    TextureSlots_Add(r, 0, (const char*)0x5cade0);
    F<int32_t>(g_engine, 0x114736) = quality;
    F<int32_t>(r, 0x168d) = F<int32_t>(r, 0x1689);
    Loading_Pump(g_app, 0, 0, 0);
    int32_t count = 0;
    while (!(F<uint8_t>(f, 0xc) & 0x10)) {
        Loading_Pump(g_app, 0, 0, 0);
        ((int(__cdecl*)(void*, const char*, ...))0x5a17e1)(f, (const char*)0x5cadd4, name, &letter, &value);
        if (!strlen(name)) continue;
        FormatText(model, (const char*)0x5cadc8, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        uint8_t code = letter >= 0x41 ? (uint8_t)(ToUpper(letter) - 0x40) : 0;
        uint8_t* obj = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x206);
        if (obj) obj = SceneObj_Construct(obj, 0, 0, model, nullptr);
        Terrain_AddMarker(r, 0, (uint32_t)(uintptr_t)obj, (int32_t)code, (int32_t)value);
        char* u = name;
        while (*u && *u != '_') ++u;
        if (*u != '_') {
            FormatText(err, GetString(0x1aa), path, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
            Fatal_Exit(err);
        }
        *u = 0;
        strcpy(species, name);
        int32_t size = (int8_t)u[1] - 0x31;
        int32_t idx = 0;
        for (int32_t k = 0; k < 0x23; ++k)
            if (!((int(__cdecl*)(const char*, const char*))0x5a57cf)(species, *(const char**)(0x5ee8e0 + k * 0x45))) {
                idx = k;
                break;
            }
        uint8_t* rec = (uint8_t*)0x5ee8e0 + idx * 0x45;
        ++F<int32_t>(rec, 0x10);
        uint8_t n = rec[0x14 + size]++;
        F<uint16_t>(rec, 0x1b + (size * 3 + n) * 2) = (uint16_t)count;
        if (++count >= 0x12c) break;
    }
    F<int32_t>(r, 0x216d) = count;
    ((int(__cdecl*)(void*))0x5a1740)(f);
    Vegetation_BuildTables();
    TreeTypes_BuildSpots();
}
RT3_REPLACE(0x454f40, TreeTypes_Load);

RT3_NOINLINE void* __fastcall ReturnThis455290(void* o) { return o; }
RT3_REPLACE(0x455290, ReturnThis455290);

RT3_NOINLINE void __fastcall Nop4552a0(void*) { __asm nop }
RT3_REPLACE(0x4552a0, Nop4552a0);

// 0x4552b0. Initialises a 0x21a-byte entry; +4 gets the next serial 0x6acd30.
RT3_NOINLINE void __fastcall Entry21a_Init(uint8_t* e, int, int32_t a, const char* name, int32_t b, int32_t c, int32_t d) {
    F<int32_t>(e, 0) = a;
    char* p = (char*)e + 0xd;
    while ((*p++ = *name++) != 0) {}
    F<int32_t>(e, 0x20e) = b;
    F<int32_t>(e, 0x212) = c;
    F<int32_t>(e, 0x216) = d;
    F<int32_t>(e, 8) = 0;
    e[0xc] = 0;
    F<int32_t>(e, 4) = g_6acd30;
    g_6acd30++;
}
RT3_REPLACE(0x4552b0, Entry21a_Init);

RT3_NOINLINE void __fastcall Obj_Reset455310(void* o) {
    F<uint8_t>(o, 0xc) = 0;
    F<int32_t>(o, 8) = 0;
}
RT3_REPLACE(0x455310, Obj_Reset455310);

// 0x455320 / 0x455350. Collection subclass 0x5cae10 (inline 0x21a-byte records, 10 + 5).
RT3_NOINLINE Collection* __fastcall Collection5cae10_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5cae10;
    Collection_Init(c, 0, 1, 0x21a, 10, 5, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x455320, Collection5cae10_Construct);

RT3_NOINLINE void __fastcall Collection5cae10_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5cae10;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x455350, Collection5cae10_Destruct);

// 0x455370. Record with the highest serial +4 (ID from +0); 0 if none.
RT3_NOINLINE void* __fastcall Table_Newest(Collection* t) {
    uint32_t sel = 0;
    int32_t best = -1;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 4) > best) {
            sel = F<uint32_t>(r, 0);
            best = F<int32_t>(r, 4);
        }
    }
    return sel ? Collection_Get(t, 0, sel) : nullptr;
}
RT3_REPLACE(0x455370, Table_Newest);

// 0x4553d0. Record with the lowest serial +4 (skipping flagged +0xc ones unless `all`); 0 if none.
RT3_NOINLINE void* __fastcall Table_Oldest(Collection* t, int, uint8_t all) {
    uint32_t sel = 0;
    int32_t best = 0x7fffffff;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 4) >= best) continue;
        if (!all && r[0xc]) continue;
        sel = F<uint32_t>(r, 0);
        best = F<int32_t>(r, 4);
    }
    return sel ? Collection_Get(t, 0, sel) : nullptr;
}
RT3_REPLACE(0x4553d0, Table_Oldest);

// 0x455440 / 0x455490. Record whose +4 is v - 1 / v + 1; 0 if none.
RT3_NOINLINE void* __fastcall Table_FindField4Prev(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 4) == v - 1) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x455440, Table_FindField4Prev);

RT3_NOINLINE void* __fastcall Table_FindField4Next(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 4) == v + 1) return r;
    }
    return nullptr;
}
RT3_REPLACE(0x455490, Table_FindField4Next);

// 0x4554e0. Adds a chat/log entry (0x4552b0), first dropping the oldest while
// more than 50; entries more than 6 serials old are flagged (+0xc).
RT3_NOINLINE uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d) {
    while (Collection_Count(t) > 0x32) {
        uint32_t id = F<uint32_t>(Table_Oldest(t, 0, 1), 0);
        void** vt = *(void***)t;
        ((void(__thiscall*)(void*, uint32_t))vt[2])(t, id);
    }
    uint8_t local[0x21c] = {};  // original: uninitialised stack (mostly overwritten by 0x4552b0)
    ReturnThis455290(local);
    uint32_t id = Collection_Allocate(t, 0, local, 0, 0, 0);
    uint8_t* e = (uint8_t*)Collection_Get(t, 0, id);
    Entry21a_Init(e, 0, (int32_t)id, name, b, c, d);
    for (uint8_t* o = (uint8_t*)Table_Oldest(t, 0, 0); F<int32_t>(e, 4) - F<int32_t>(o, 4) > 6;
         o = (uint8_t*)Table_Oldest(t, 0, 0))
        o[0xc] = 1;
    Nop4552a0(local);
    return id;
}
RT3_REPLACE(0x4554e0, Log_Add);

// 0x4555b0. Stamps unstamped entries (+8) with the clock; flags (+0xc)
// those stamped more than 10 s ago.
RT3_NOINLINE void __fastcall Table_MarkStale(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        int32_t stamp = F<int32_t>(r, 8);
        int32_t now = ClockMs();
        if (stamp) {
            if ((uint32_t)(now - stamp) > 10000) r[0xc] = 1;
        } else {
            F<int32_t>(r, 8) = now;
        }
    }
}
RT3_REPLACE(0x4555b0, Table_MarkStale);

// 0x455610. Resets a named scene object: its three name strings (+0x206..),
// +0x212..+0x22d and +0x22e/+0x232, then the scene object part.
RT3_NOINLINE int32_t __fastcall NamedObj_Reset(uint8_t* o) {
    F<int32_t>(o, 0x206) = 0;
    F<int32_t>(o, 0x20a) = 0;
    F<int32_t>(o, 0x20e) = 0;
    F<int32_t>(o, 0x22e) = 0;
    F<int32_t>(o, 0x232) = 0;
    for (int k = 0; k < 7; ++k) F<int32_t>(o, 0x212 + k * 4) = 0;
    return SceneObj_Reset_Impl(o);
}
RT3_REPLACE(0x455610, NamedObj_Reset);

// 0x455650. Named scene object destructor.
RT3_NOINLINE void __fastcall NamedObj_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5cb1c0;
    SceneObj_Destructor(o);
}
RT3_REPLACE(0x455650, NamedObj_Destructor);

// 0x455660. Moves the object to cell (cx, cz) at `lift` above the ground (snapped to
// whole units in multiplayer).
RT3_NOINLINE void __fastcall Obj_PlaceAtCell(uint8_t* o, int, float cx, float cz, int32_t lift) {
    float z = (float)((double)cz * Fl_188(0x5c8700));
    float x = (float)((double)cx * Fl_188(0x5c8700));
    double h = Terrain_HeightAt((uint8_t*)Engine_Get1C(*(void**)0x6d4024), 0, x, z);
    float y = (float)(h + (float)lift);
    if (IsMultiplayer()) y = SnapInt(y);
    Obj_MoveTo(o, 0, x, y, z);
}
RT3_REPLACE(0x455660, Obj_PlaceAtCell);

// 0x4556f0. Moves the object to cell (cx, cz) at height y.
RT3_NOINLINE void __fastcall Obj_MoveToCell(uint8_t* o, int, float cx, float cz, float y) {
    float z = (float)((double)cz * Fl_188(0x5c8700));
    Obj_MoveTo(o, 0, (float)((double)cx * Fl_188(0x5c8700)), y, z);
}
RT3_REPLACE(0x4556f0, Obj_MoveToCell);

             // -> 0x53a3a0
RT3_REPLACE(0x455720, SceneObj_SetScale36);

// 0x455730 / 0x455790 / 0x455750. Second / first angle; set the second angle.
RT3_NOINLINE float __fastcall SceneObj_Angle2(uint8_t* o) {
    float a, b, c;
    SceneObj_GetAngles(o, 0, (uint32_t*)&a, (uint32_t*)&b, (uint32_t*)&c);
    return b;
}
RT3_REPLACE(0x455730, SceneObj_Angle2);

RT3_NOINLINE void __fastcall SceneObj_SetAngle2(uint8_t* o, int, uint32_t v) {
    uint32_t a, b, c;
    SceneObj_GetAngles(o, 0, &a, &b, &c);
    SceneObj_SetAngles(o, 0, a, v, c);
}
RT3_REPLACE(0x455750, SceneObj_SetAngle2);

RT3_NOINLINE float __fastcall SceneObj_Angle1(uint8_t* o) {
    float a, b, c;
    SceneObj_GetAngles(o, 0, (uint32_t*)&a, (uint32_t*)&b, (uint32_t*)&c);
    return a;
}
RT3_REPLACE(0x455790, SceneObj_Angle1);

// 0x4557b0. Closes the model preview: the previewed object (0x6acd3c) is
// forgotten and the preview model (0x6acd40) released and deleted.
RT3_NOINLINE void ModelPreview_Close() {
    if (!g_previewModel) return;
    hook::As<uint32_t>(g_6acd3c) = 0;
    ModelObj_Release(g_previewModel);
    if (uint8_t* m = g_previewModel) {
        ModelObj_Destructor(m);
        ((void(__cdecl*)(void*))0x53b080)(m);
    }
    hook::As<uint32_t>(g_previewModel) = 0;
}
RT3_REPLACE(0x4557b0, ModelPreview_Close);

RT3_NOINLINE int32_t __fastcall IsCurrent6acd3c(void* o) { return hook::As<void*>(g_6acd3c) == o ? 1 : 0; }
RT3_REPLACE(0x4557f0, IsCurrent6acd3c);

// 0x455800 / 0x455810. Object position in cells.
// x87: FLD float / FDIV float, kept in the FPU, so computed in double here.
RT3_NOINLINE double __fastcall Obj_CellX(void* o) { return (double)F<float>(o, 0x1e2) / kCellUnits; }
RT3_REPLACE(0x455800, Obj_CellX);

RT3_NOINLINE double __fastcall Obj_CellY(void* o) { return (double)F<float>(o, 0x1ea) / kCellUnits; }
RT3_REPLACE(0x455810, Obj_CellY);

    // -> 0x52ea90
RT3_REPLACE(0x455860, SceneObj_DebugTooltip);

// 0x4559d0. Saves the names (chunk 0x55f1), the object's own data (vtbl +0x4c,
// chunk 0x55f2) and its flags, closing with 0x55f3.
RT3_NOINLINE void __fastcall NamedObj_Save(uint8_t* o, int, Stream s) {
    StreamWriteU32(s, 0x55f1);
    StreamWriteLenString(s, (const char* const*)(o + 0x206));
    StreamWriteLenString(s, (const char* const*)(o + 0x20a));
    StreamWriteLenString(s, (const char* const*)(o + 0x20e));
    StreamWriteU32(s, 0x55f2);
    ((void(__thiscall*)(void*, Stream))(*(void***)o)[0x4c / 4])(o, s);
    SceneObj_SaveFlags(o, 0, s);
    StreamWriteU32(s, 0x55f3);
}
RT3_REPLACE(0x4559d0, NamedObj_Save);

// 0x455a40. Tail call to virtual method +0x44 (any arguments).
__declspec(naked) void Obj_VCall44() {
    __asm {
        mov eax, [ecx]
        jmp dword ptr [eax + 0x44]
    }
}
RT3_REPLACE(0x455a40, Obj_VCall44);

// 0x455a50. Virtual slot 16, then clears the current-object globals 0x6acd38..0x6acd40.
int32_t __fastcall Obj_CloseCurrent(void* o, int, int32_t a) {
    ((void(__thiscall*)(void*, int32_t))(*(void***)o)[0x40 / 4])(o, a);
    hook::As<int32_t>(g_6acd38) = 0;
    hook::As<int32_t>(g_6acd3c) = 0;
    hook::As<int32_t>(g_previewModel) = 0;
    return 0;
}
RT3_REPLACE(0x455a50, Obj_CloseCurrent);

// 0x455a70. Drops the object to `lift` above the ground under it (snapped to whole
// units in multiplayer).
void __fastcall Obj_DropToGround(uint8_t* o, int, int32_t lift) {
    float x, y, z;
    SceneObj_GetPos(o, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    double h = Terrain_HeightAt((uint8_t*)Engine_Get1C(*(void**)0x6d4024), 0, x, z);
    float ny = (float)(h + (float)lift);
    if (IsMultiplayer()) ny = SnapInt(ny);
    Obj_MoveTo(o, 0, x, ny, z);
}
RT3_REPLACE(0x455a70, Obj_DropToGround);

// 0x455b00 (vtable stub).
void __fastcall Nop455b00(void*) { __asm nop }
RT3_REPLACE(0x455b00, Nop455b00);

// 0x455b10 (vtable stub).
void __fastcall Nop455b10(void*) { __asm nop }
RT3_REPLACE(0x455b10, Nop455b10);

// 0x455b20. Named scene object constructor.
RT3_NOINLINE uint8_t* __fastcall NamedObj_Construct(uint8_t* o) {
    SceneObj_ConstructDefault(o);
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

// 0x455b70. Sets world model o up: name (+0x206), model (+0x20a; default the
// name's 0x5cb234 form) and texture (+0x20e; default the name), on the terrain
// unless free-standing, frame 0, visible (+0x20 bit 0). Models listed in table
// 0x5f0720 (33 {name, limit}) get their first mesh's +0x10 capped at the limit.
// Placed at cell (x, y) unless either is -1.
RT3_NOINLINE void __fastcall WorldModel_Init(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x, uint32_t y,
                                int32_t freeStanding) {
    F<uint32_t>(o, 0x206) = 0;
    F<uint32_t>(o, 0x20a) = 0;
    F<uint32_t>(o, 0x20e) = 0;
    F<uint32_t>(o, 0x22e) = 0;
    F<uint32_t>(o, 0x232) = 0;
    memset(o + 0x212, 0, 7 * 4);
    ((void(__fastcall*)(uint8_t*))&SceneObj_Reset)(o);
    if (name) String_Assign((char**)(o + 0x206), name);
    if (model) String_Assign((char**)(o + 0x20a), model);
    if (tex) String_Assign((char**)(o + 0x20e), tex);
    char file[0x80];
    if (model) strcpy(file, model);
    else FormatText(file, (const char*)0x5cb234, name, 0, 0, 0, 0, 0, 0, 0);
    const char* skin = tex ? tex : name;
    uint32_t owner = freeStanding ? 0 : (uint32_t)(uintptr_t)Engine_Get1C(hook::As<void*>(g_engine));
    SceneObj_Init(o, 0, owner, file, skin);
    Nop52e670(o, 0, (int32_t)(uintptr_t)name);
    Model_SetFrame(o, 0, 0);
    o[0x20] |= 1;
    for (int32_t i = 0; i < 0x21; ++i) {
        const char* m = F<const char*>(o, 0x20a);
        if (!m || ((int(__cdecl*)(const char*, const char*))0x5a57cf)(*(const char**)(0x5f0720 + i * 8), m)) continue;
        uint8_t* mdl = F<uint8_t*>(o, 0x14);
        if (mdl && F<int32_t>(mdl, 0x14) >= 1) {
            uint8_t* mesh = F<uint8_t*>(mdl, 0xc);
            float limit = *(const float*)(0x5f0724 + i * 8);
            if (!(F<float>(mesh, 0x10) < limit)) F<float>(mesh, 0x10) = limit;
        }
        break;
    }
    if (x != 0xbf800000 && y != 0xbf800000) Obj_PlaceAtCell(o, 0, *(float*)&x, *(float*)&y, 0);
}
RT3_REPLACE(0x455b70, WorldModel_Init);

// 0x455d20. Scene object o: stops the camera following it, destructs the
// scene part, forgets it as the world's selection (+0x66a2), frees +0x206 /
// +0x20a / +0x20e and, when it was the previewed object (0x6acd3c), drops
// the preview effect (0x6acd38) and the preview model (0x6acd40, unless o).
RT3_NOINLINE void __fastcall SceneObj_Teardown(uint8_t* o) {
    if (g_world && g_62be68 && F<uint8_t*>(g_62be68, 0x60) == o)
        World_SetFollow(g_world, 0, 0, o);
    SceneObj_Destruct(o);
    if (o == F<uint8_t*>(g_world, 0x66a2)) F<void*>(g_world, 0x66a2) = nullptr;
    FreeAndNull2((void**)(o + 0x206));
    FreeAndNull2((void**)(o + 0x20a));
    FreeAndNull2((void**)(o + 0x20e));
    if (g_6acd3c != o) return;
    hook::As<uint32_t>(g_6acd3c) = 0;
    if (g_6acd38) {
        Effect_Free(g_6acd38);
        hook::As<uint32_t>(g_6acd38) = 0;
    }
    if (!g_previewModel || o == g_previewModel) return;
    ModelObj_Release(g_previewModel);
    if (uint8_t* m = g_previewModel) {
        ModelObj_Destructor(m);
        ((void(__cdecl*)(void*))0x53b080)(m);
    }
    hook::As<uint32_t>(g_previewModel) = 0;
}
RT3_REPLACE(0x455d20, SceneObj_Teardown);

// 0x455de0. Scene object o: becomes the previewed object - plays the
// select sound, replaces the preview model (0x6acd40) by a "pointer" at o's
// cell, raised 32 above the top of o's model part (+0x14 / +0x3a / +0x21)
// over o's height, scaled 0.8 and shown, and notifies o (vtable slot 0)
// with the time.
// QUIRK: an object without a model table (+0x14) reads its part at 0
// (crash).
RT3_NOINLINE void __fastcall SceneObj_ShowPreview(uint8_t* o) {
    if (o == g_previewModel) return;
    Ui_PlaySound(0x21, 0, 0x3f000000);
    if (g_previewModel) {
        hook::As<uint32_t>(g_6acd3c) = 0;
        ModelObj_Release(g_previewModel);
        if (uint8_t* m = g_previewModel) {
            ModelObj_Destructor(m);
            ((void(__cdecl*)(void*))0x53b080)(m);
        }
        hook::As<uint32_t>(g_previewModel) = 0;
    }
    uint8_t* mem = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x26e);
    uint8_t* m = mem ? ModelObj_Construct(mem) : nullptr;
    g_previewModel = m;
    float cz = (float)((double)F<float>(o, 0x1ea) / Fl_207(0x5c8700));
    float cx = (float)((double)F<float>(o, 0x1e2) / Fl_207(0x5c8700));
    Building_Init(m, 0, (const char*)0x5cb244, (const char*)0x5cb244, cx, cz, 1, 0, 0, 0);
    uint8_t* part = nullptr;
    if (uint8_t* tbl = F<uint8_t*>(o, 0x14)) part = F<uint8_t*>(tbl, F<int32_t>(o, 0x3a) * 0x18 + 0xc) + F<int32_t>(o, 0x21) * 0x45;
    uint32_t px, py, pz;
    SceneObj_GetPos(o, 0, &px, &py, &pz);
    float top = (float)((double)F<float>(part, 0x18) + Fl_207(0x5c8c40));
    float y = (float)((double)top + BitsF(py));
    float z = (float)((double)F<float>(o, 0x1ea) / Fl_207(0x5c8700));
    float x = (float)((double)F<float>(o, 0x1e2) / Fl_207(0x5c8700));
    Obj_MoveTo(g_previewModel, 0, (float)((double)x * Fl_207(0x5c8700)), y, (float)((double)z * Fl_207(0x5c8700)));
    SceneObj_SetScale36(g_previewModel, 0, BitsF(0x3f4ccccd));
    g_previewModel[0x20] &= 0xdf;
    g_6acd3c = o;
    void** vt = *(void***)o;
    int32_t now = ClockMs();
    ((void(__fastcall*)(uint8_t*, int, int32_t))vt[0])(o, 0, now);
}
RT3_REPLACE(0x455de0, SceneObj_ShowPreview);

// 0x455f60. Region record (0x1d7 bytes, 4x4 cells each) under an object, in
// the region grid 0x62c120. QUIRK kept: computed with negative coordinates
// and subtracted from the base, as in the original.
RT3_NOINLINE uint8_t* __fastcall Obj_Region(void* o) {
    const float neg92 = -92.0f, four = 4.0f; // 0x5cb24c, 0x5c8568
    int32_t row = (int32_t)((double)F<float>(o, 0x1ea) / neg92 / four);
    int32_t col = (int32_t)((double)F<float>(o, 0x1e2) / neg92 / four);
    uint8_t* grid = g_map;
    return F<uint8_t*>(grid, 0x2129) - (row * F<int32_t>(grid, 0x2145) + col) * 0x1d7;
}
RT3_REPLACE(0x455f60, Obj_Region);

// 0x455fc0. Loads world model o from stream s: name, model and texture between
// marks 0x55f1 / 0x55f2, set up at cell (1, 1), then its own data (slot 0x48)
// and flags, mark 0x55f3. Returns the bytes read.
RT3_NOINLINE int32_t __fastcall WorldModel_Load(uint8_t* o, int, int32_t s) {
    F<uint32_t>(o, 0x206) = 0;
    F<uint32_t>(o, 0x20a) = 0;
    F<uint32_t>(o, 0x20e) = 0;
    F<uint32_t>(o, 0x22e) = 0;
    F<uint32_t>(o, 0x232) = 0;
    memset(o + 0x212, 0, 7 * 4);
    ((void(__fastcall*)(uint8_t*))&SceneObj_Reset)(o);
    int32_t n = StreamSkip4(s, 0x55f1, 0, 0);
    char* str[3] = { nullptr, nullptr, nullptr };
    n += StreamReadString(s, &str[0]);
    n += StreamReadString(s, &str[1]);
    n += StreamReadString(s, &str[2]);
    n += StreamSkip4(s, 0x55f2, 0, 0);
    WorldModel_Init(o, 0, str[0], str[1], str[2], 0x3f800000, 0x3f800000, 0);
    FreeAndNull2((void**)&str[0]);
    FreeAndNull2((void**)&str[1]);
    FreeAndNull2((void**)&str[2]);
    ((void(__thiscall*)(uint8_t*, int32_t))(*(void***)o)[0x48 / 4])(o, s);
    SceneObj_LoadFlags(o, 0, s);
    return StreamSkip4(s, 0x55f3, 0, 0) + n;
}
RT3_REPLACE(0x455fc0, WorldModel_Load);

// 0x4560c0 / 0x52eca0 (vtables). Model o's current frame record (frame +0x21
// of animation +0x3a: 24-byte entries at +0x14, frames of 0x45 bytes at +0xc,
// count +0x14): its time (+0x18, 0 without one) / the next frame (null at
// the end).
float __fastcall Model_FrameTime(uint8_t* o) {
    uint32_t v = 0;
    if (uint8_t* list = F<uint8_t*>(o, 0x14)) {
        uint8_t* rec = F<uint8_t*>(list + 24 * F<int32_t>(o, 0x3a), 0xc) + F<int32_t>(o, 0x21) * 0x45;
        if (rec) v = F<uint32_t>(rec, 0x18);
    }
    float f;
    memcpy(&f, &v, 4);
    return f;
}
RT3_REPLACE(0x4560c0, Model_FrameTime);

// 0x456100. Constructs world model o (vtable 0x5cb1c0) and sets it up (WorldModel_Init).
RT3_NOINLINE uint8_t* __fastcall WorldModel_Construct(uint8_t* o, int, const char* name, const char* model, const char* tex, uint32_t x,
                                         uint32_t y, int32_t freeStanding) {
    SceneObj_ConstructDefault(o);
    F<uint32_t>(o, 0) = 0x5cb1c0;
    F<uint32_t>(o, 0x206) = 0;
    F<uint32_t>(o, 0x20a) = 0;
    F<uint32_t>(o, 0x20e) = 0;
    F<uint32_t>(o, 0x22e) = 0;
    F<uint32_t>(o, 0x232) = 0;
    memset(o + 0x212, 0, 7 * 4);
    ((void(__fastcall*)(uint8_t*))&SceneObj_Reset)(o);
    WorldModel_Init(o, 0, name, model, tex, x, y, freeStanding);
    return o;
}
RT3_REPLACE(0x456100, WorldModel_Construct);

void __fastcall NamedObj456900_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5cb250;
    NamedObj_Destructor(o);
}
RT3_REPLACE(0x456170, NamedObj456900_Destructor);

// 0x456180. Marks the object done (+0x23e) once, releasing a busy count (0x6d401c +0xc60).
RT3_NOINLINE void __fastcall Obj_MarkDone(uint8_t* o) {
    if (o[0x23e]) return;
    o[0x23e] = 1;
    Obj_Decc60(g_windowManager);
}
RT3_REPLACE(0x456180, Obj_MarkDone);

// 0x4561a0. Train object o: on its pending click (+0x23e) bumps the window
// manager and - unless it is a dummy (+0x352) - has the camera follow the
// train (+0x23a), zooming in to 300 when further away.
RT3_NOINLINE void __fastcall TrainObj_Clicked(uint8_t* o) {
    if (!o[0x23e]) return;
    o[0x23e] = 0;
    Obj_Incc60(g_windowManager);
    if (F<int32_t>(o, 0x352)) return;
    World_FollowEntity(g_world, 0, 1, 3, F<int32_t>(o, 0x23a));
    if (Obj_Float18(g_62be68) > Fl_207(0x5c87c0)) Camera_SetZoom(g_62be68, 0, 0x43960000);
}
RT3_REPLACE(0x4561a0, TrainObj_Clicked);

// 0x456210. Swings the two pairs of linked parts (+0x32e / +0x33e, phases +0x332..
// / +0x342..) along with the drive phase (+0x336 / +0x346, advanced by step x
// (max(cos, 0) x k + +0x34e)), then rocks the body: pitch from the drive phase, a
// vertical bob, and a damped roll accumulated in +0x37e.
RT3_NOINLINE void __fastcall Vehicle_AnimateLinks(uint8_t* v, int, float step) {
    for (int32_t i = 0; i < 2; ++i) {
        float w = (float)(D(0x5c8770) / ((double)(float)(i * 3) + D(0x5c8770)));
        if (uint8_t* part = F<uint8_t*>(v, 0x32e + i * 4)) {
            float a;
            if (i) {
                double c = ExeCos(F<float>(v, 0x336 + i * 4));
                a = (float)((ExeCos(F<float>(v, 0x332 + i * 4)) + c) * w - D(0x5cb2d0));
            } else a = (float)(ExeCos(F<float>(v, 0x336)) * w + D(0x5cb2c8));
            SceneObj_SetAngles(part, 0, 0, 0, Bits(a));
            float t = (float)(ExeCos(F<float>(v, 0x336 + i * 4)) * D(0x5c8798));
            if (t < *(const float*)0x5c8598) t = 0.0f;
            F<float>(v, 0x336 + i * 4) = (float)(((double)t + F<float>(v, 0x34e)) * step + F<float>(v, 0x336 + i * 4));
        }
        if (uint8_t* part = F<uint8_t*>(v, 0x33e + i * 4)) {
            float a;
            if (i) {
                double c = -ExeCos(F<float>(v, 0x346 + i * 4));
                a = (float)((c - ExeCos(F<float>(v, 0x342 + i * 4))) * w + D(0x5cb2d0));
            } else a = (float)(-(ExeCos(F<float>(v, 0x346)) * w) - D(0x5cb2c8));
            SceneObj_SetAngles(part, 0, 0, 0, Bits(a));
            float t = (float)(ExeCos(F<float>(v, 0x346 + i * 4)) * D(0x5c8798));
            if (t < *(const float*)0x5c8598) t = 0.0f;
            F<float>(v, 0x346 + i * 4) = (float)(((double)t + F<float>(v, 0x34e)) * step + F<float>(v, 0x346 + i * 4));
        }
    }
    double pitch = SceneObj_Angle1(v);
    float roll = (float)(pitch - ExeSin((double)F<float>(v, 0x336) * D(0x5c8628)) * D(0x5c8678));
    float yaw = SceneObj_Angle2(v);
    SceneObj_SetAngles(v, 0, Bits(roll), Bits(yaw), 0);
    float bob = (float)(ExeCos((double)F<float>(v, 0x336) - D(0x5c88f0)) * D(0x5c9ee0));
    Obj_MoveBy(v, 0, 0.0f, bob, 0.0f);
    float acc = (float)((double)roll + F<float>(v, 0x37e));
    F<float>(v, 0x37e) = acc;
    if (!(step < *(const float*)0x5c85b4)) step = 1.0f;
    float damp = 0.1f;
    if (ExeFabs(acc) > D(0x5c85d0)) damp = 0.3f;
    F<float>(v, 0x37e) = (float)((D(0x5c8770) - (double)damp * step) * F<float>(v, 0x37e));
}
RT3_REPLACE(0x456210, Vehicle_AnimateLinks);

// 0x456500. Spins the six wheel models (+0x302) by speed x (+0x2a5 / +0x285) x 30,
// around the third axis (or the second when +0x2f5 is set), wrapping past 2 pi.
RT3_NOINLINE void __fastcall Vehicle_SpinWheels(uint8_t* v, int, float speed) {
    for (int k = 0; k < 6; ++k) {
        uint8_t* w = F<uint8_t*>(v, 0x302 + k * 4);
        if (!w) continue;
        float ang;
        if (!v[0x2f5]) SceneObj_GetAngles(w, 0, nullptr, nullptr, (uint32_t*)&ang);
        else SceneObj_GetAngles(w, 0, nullptr, (uint32_t*)&ang, nullptr);
        double r = (double)F<float>(v, 0x2a5) / F<float>(v, 0x285);
        ang = (float)(r * ((double)speed * *(const float*)0x5c9aa8) + ang);
        const float twoPi = *(const float*)0x5c8768;
        if (ang > twoPi) ang = (float)((double)ang - twoPi);
        if (!v[0x2f5]) SceneObj_SetAngles(F<uint8_t*>(v, 0x302 + k * 4), 0, 0, 0, Bits(ang));
        else SceneObj_SetAngles(F<uint8_t*>(v, 0x302 + k * 4), 0, 0, Bits(ang), 0);
    }
}
RT3_REPLACE(0x456500, Vehicle_SpinWheels);

// 0x4565b0. Advances the two blink timers (+0x376) of the indicator lights (+0x36e):
// off below 0x5cb2e0, on below 0x5cb2d8, then on and restart.
RT3_NOINLINE void __fastcall Vehicle_BlinkLights(uint8_t* v, int, float dt) {
    for (int k = 0; k < 2; ++k) {
        uint8_t* light = F<uint8_t*>(v, 0x36e + k * 4);
        if (!light) continue;
        float t = (float)((double)dt + F<float>(v, 0x376 + k * 4));
        F<float>(v, 0x376 + k * 4) = t;
        if ((double)t < *(const double*)0x5cb2e0) {
            Flags24_Clear(light);
        } else if ((double)t < *(const double*)0x5cb2d8) {
            Flags24_Set1(light);
        } else {
            Flags24_Set1(light);
            F<int32_t>(v, 0x376 + k * 4) = 0;
        }
    }
}
RT3_REPLACE(0x4565b0, Vehicle_BlinkLights);

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

// 0x456680 (vtable). The world follows entity +0x23a (kind 3).
void __fastcall Obj_FollowThis(uint8_t* o) { World_FollowEntity(g_world, 0, 1, 3, F<int32_t>(o, 0x23a)); }
RT3_REPLACE(0x456680, Obj_FollowThis);

// 0x4566a0 (callback). Shows string 0x1af as a tooltip.
void Tooltip_Show1af() {
    char text[0x12c];
    FormatText(text, GetString(0x1af), 0, 0, 0, 0, 0, 0, 0, 0);
    Ui_ShowTooltip(g_windowManager, 0, text, 3, 0, -1);
}
RT3_REPLACE(0x4566a0, Tooltip_Show1af);

int32_t Tooltip_Clear4566f0() { return Tooltip_Clear(); }
RT3_REPLACE(0x4566f0, Tooltip_Clear4566f0);

// 0x456710. out = vec * matrix (4x4 floats, translation in row 3), x87 order of the original.
RT3_NOINLINE float* __fastcall Vec3_Transform(float* v, int, float* out, const float* m) { // returns out (callers use EAX)
    float z = (float)((((double)m[10] * v[2]) + (double)m[2] * v[0]) + (double)m[6] * v[1] + m[14]);
    float y = (float)((((double)m[9] * v[2]) + (double)m[1] * v[0]) + (double)m[5] * v[1] + m[13]);
    float x = (float)((((double)m[8] * v[2]) + (double)m[4] * v[1]) + (double)m[0] * v[0] + m[12]);
    out[1] = y;
    out[2] = z;
    out[0] = x;
    return out;
}
RT3_REPLACE(0x456710, Vec3_Transform);

// 0x456780. Normalises a float vec3 in place (if its length is > 0); returns it.
RT3_NOINLINE float* __fastcall Vec3_Normalize(float* v) {
    float sq = (float)(((double)v[0] * v[0] + (double)v[1] * v[1]) + (double)v[2] * v[2]);
    float len = (float)ExeSqrt(sq);
    if (*(const float*)0x5c8598 < len) {
        float inv = (float)(*(const float*)0x5c85b4 / (double)len);
        v[0] = (float)((double)inv * v[0]);
        v[1] = (float)((double)inv * v[1]);
        v[2] = (float)((double)inv * v[2]);
    }
    return v;
}
RT3_REPLACE(0x456780, Vec3_Normalize);

// 0x456820. a = a x b (float vectors).
RT3_NOINLINE void __fastcall Vec3_CrossInPlace(float* a, int, const float* b) {
    float a0 = a[0], a1 = a[1], a2 = a[2];
    float b0 = b[0], b1 = b[1], b2 = b[2];
    a[0] = (float)((double)b2 * a1 - (double)b1 * a2);
    a[1] = (float)((double)b0 * a2 - (double)b2 * a0);
    a[2] = (float)((double)b1 * a0 - (double)b0 * a1);
}
RT3_REPLACE(0x456820, Vec3_CrossInPlace);

// 0x4568a0. Length of a float vec3 (squared sum rounded to float, result too).
RT3_NOINLINE double __fastcall Vec3_Length(const float* v) {
    float sq = (float)(((double)v[0] * v[0] + (double)v[1] * v[1]) + (double)v[2] * v[2]);
    float len = (float)ExeSqrt(sq);
    return len;
}
RT3_REPLACE(0x4568a0, Vec3_Length);

uint8_t* __fastcall NamedObj456900_Construct(uint8_t* o) {
    NamedObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5cb250;
    return o;
}
RT3_REPLACE(0x456900, NamedObj456900_Construct);

 // namespace

// 0x456920. Loads the parts of vehicle v (name +0x241) from the packs:
// propellers ("%1_Prop%2", else rotors "%1_Rotor%2", which mark it a
// helicopter +0x2f5) +0x302 and engine lights +0x356 (6 each), up to 7 lights,
// two strobes +0x36e with random phases +0x376, jet / prop exhaust emitters
// +0x2f6 / +0x2fa, the turret chain (turret +0x31a, mantlet +0x31e, cannon
// +0x322, its sound +0x32a and muzzle flash +0x326), two wing pairs +0x32e /
// +0x33e with random flap phases, the engine sound +0x2fe and, for a plane,
// the axes of its control surfaces (+0x2d1 aileron, +0x2c5 elevator, +0x2b9
// rudder, +0x2dd thrust) from the first vertex of each model; +0x240 says
// whether they were all found.
RT3_NOINLINE void __fastcall Vehicle_LoadParts(uint8_t* v) {
    char num[0x10], path[0xc8];
    const char* name = (const char*)v + 0x241;
    v[0x2f5] = 0;
    for (int32_t k = 0; k < 6; ++k) {
        F<uint8_t*>(v, 0x302 + k * 4) = nullptr;
        double n = (double)(k + 1);
        FormatText(path, (const char*)0x5cb440, name, Num_Format(num, 1, n, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
        bool found = PackHas(path) != 0;
        if (!found) {
            FormatText(path, (const char*)0x5cb430, name, Num_Format(num, 1, n, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
            if (PackHas(path)) {
                v[0x2f5] = 1;
                found = true;
            }
        }
        if (found) {
            uint8_t* o = NewSceneObj(path, name);
            F<uint8_t*>(v, 0x302 + k * 4) = o;
            SceneObj_AddChild(v, 0, o);
        }
        F<uint8_t*>(v, 0x356 + k * 4) = nullptr;
        FormatText(path, (const char*)0x5cb418, name, Num_Format(num, 1, (double)k, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
        if (PackHas(path)) {
            uint8_t* c = NewLight(path, v);
            F<uint8_t*>(v, 0x356 + k * 4) = c;
            ShowLight(c);
            SceneObj_AddAttachment(v, 0, F<uint8_t*>(v, 0x356 + k * 4));
        }
    }
    for (int32_t i = 1; i < 8; ++i) {
        FormatText(path, (const char*)0x5cb408, name, Num_Format(num, 1, (double)i, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
        if (!PackHas(path)) break;
        uint8_t* c = NewLight(path, v);
        ShowLight(c);
        SceneObj_AddAttachment(v, 0, c);
    }
    for (int32_t k = 0; k < 2; ++k) {
        FormatText(path, (const char*)0x5cb3f8, name, Num_Format(num, 1, (double)(k + 1), 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
        F<uint8_t*>(v, 0x36e + k * 4) = nullptr;
        F<uint32_t>(v, 0x376 + k * 4) = 0;
        if (!PackHas(path)) continue;
        float r = (float)(((int(__cdecl*)())0x5a29ab)() % 2000);
        F<float>(v, 0x376 + k * 4) = (float)((double)r / *(const float*)0x5cb3f4);
        uint8_t* c = NewLight(path, v);
        F<uint8_t*>(v, 0x36e + k * 4) = c;
        SceneObj_AddAttachment(v, 0, c);
        ShowLight(F<uint8_t*>(v, 0x36e + k * 4));
    }
    F<uint8_t*>(v, 0x2f6) = nullptr;
    FormatText(path, (const char*)0x5cb3e0, name, 0, 0, 0, 0, 0, 0, 0);
    if (PackHas(path)) {
        uint32_t r[4] = { 0, 0, 0x3f000000, 0x3f000000 };
        F<uint8_t*>(v, 0x2f6) = NewExhaust(path, 0xffffffff, 0x100, (const float*)r, v, 0x80, 0x3f333333);
    }
    F<uint8_t*>(v, 0x2fa) = nullptr;
    FormatText(path, (const char*)0x5cb3cc, name, 0, 0, 0, 0, 0, 0, 0);
    if (PackHas(path)) {
        uint32_t r[4] = { 0, 0x3e800000, 0x3e800000, 0x3f000000 };
        F<uint8_t*>(v, 0x2fa) = NewExhaust(path, 0xff3f3f3f, 0x20, (const float*)r, v, 0x190, 0x3e800000);
    }
    do {
        FormatText(path, (const char*)0x5cb3bc, name, 0, 0, 0, 0, 0, 0, 0);
        if (!PackHas(path)) break;
        F<uint8_t*>(v, 0x31a) = NewSceneObj(path, name);
        SceneObj_AddChild(v, 0, F<uint8_t*>(v, 0x31a));
        FormatText(path, (const char*)0x5cb3ac, name, 0, 0, 0, 0, 0, 0, 0);
        if (!PackHas(path)) break;
        F<uint8_t*>(v, 0x31e) = NewSceneObj(path, name);
        SceneObj_AddChild(F<uint8_t*>(v, 0x31a), 0, F<uint8_t*>(v, 0x31e));
        FormatText(path, (const char*)0x5cb39c, name, 0, 0, 0, 0, 0, 0, 0);
        if (!PackHas(path)) break;
        F<uint8_t*>(v, 0x322) = NewSceneObj(path, name);
        SceneObj_AddChild(F<uint8_t*>(v, 0x31e), 0, F<uint8_t*>(v, 0x322));
        FormatText(path, (const char*)0x5cb388, name, 0, 0, 0, 0, 0, 0, 0);
        if (PackHas(path)) {
            FormatText(path, (const char*)0x5cb378, name, 0, 0, 0, 0, 0, 0, 0);
            uint32_t minus1 = 0xbf800000;
            F<uint8_t*>(v, 0x32a) = NewSound(path, v, *(float*)&minus1);
            Obj_AttachChannel(v, 0, F<uint8_t*>(v, 0x32a));
            Channel_SetLoop(F<uint8_t*>(v, 0x32a), 0, 1);
        }
        FormatText(path, (const char*)0x5cb364, name, 0, 0, 0, 0, 0, 0, 0);
        if (!PackHas(path)) break;
        uint8_t* c = NewLight(path, F<uint8_t*>(v, 0x31e));
        F<uint8_t*>(v, 0x326) = c;
        ShowLight(c);
        Flags24_Clear(F<uint8_t*>(v, 0x326));
        SceneObj_AddAttachment(F<uint8_t*>(v, 0x31e), 0, F<uint8_t*>(v, 0x326));
    } while (0);
    for (int32_t k = 0; k < 2; ++k) {
        F<uint8_t*>(v, 0x32e + k * 4) = nullptr;
        F<uint8_t*>(v, 0x33e + k * 4) = nullptr;
        float r = (float)Rand(0, 0x7ab7);
        float phase = (float)((double)r / *(const float*)0x5c88c0);
        F<float>(v, 0x346 + k * 4) = phase;
        F<float>(v, 0x336 + k * 4) = phase;
        double n = (double)(k + 1);
        FormatText(path, (const char*)0x5cb354, name, Num_Format(num, 1, n, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
        if (PackHas(path)) {
            uint8_t* o = NewSceneObj(path, name);
            F<uint8_t*>(v, 0x32e + k * 4) = o;
            SceneObj_AddChild(k ? F<uint8_t*>(v, 0x32a + k * 4) : v, 0, o);
        }
        FormatText(path, (const char*)0x5cb344, name, Num_Format(num, 1, n, 10, 1, 0, 0), 0, 0, 0, 0, 0, 0);
        if (PackHas(path)) {
            uint8_t* o = NewSceneObj(path, name);
            F<uint8_t*>(v, 0x33e + k * 4) = o;
            SceneObj_AddChild(k ? F<uint8_t*>(v, 0x33a + k * 4) : v, 0, o);
        }
    }
    F<uint8_t*>(v, 0x2fe) = nullptr;
    FormatText(path, (const char*)0x5cb334, name, 0, 0, 0, 0, 0, 0, 0);
    if (PackHas(path)) {
        FormatText(path, (const char*)0x5cb328, name, 0, 0, 0, 0, 0, 0, 0);
        uint8_t* m = (uint8_t*)ExeNew_105(0x141);
        uint8_t* ch = nullptr;
        if (m) {
            float vol = (float)((v[0x261] ? 0.47 : 3.0) * *(const double*)0x5c8c48);
            ch = Channel_Setup(m, 0, path, g_map, F<float>(v, 0x1e2), F<float>(v, 0x1e6), F<float>(v, 0x1ea), -1,
                               0, vol);
        }
        F<uint8_t*>(v, 0x2fe) = ch;
        Obj_AttachChannel(v, 0, ch);
    }
    if (v[0x261]) {
        float r = (float)Rand(0x1388, 0x1770);
        F<float>(v, 0x34e) = (float)((double)r / *(const float*)0x5c86a0);
    }
    if (v[0x2f5]) {
        F<uint32_t>(v, 0x2d1) = 0;
        F<uint32_t>(v, 0x2d5) = 0;
        F<uint32_t>(v, 0x2d9) = 0x3f800000;
        F<uint32_t>(v, 0x2b9) = 0;
        F<uint32_t>(v, 0x2bd) = 0x3f800000;
        F<uint32_t>(v, 0x2c1) = 0;
        F<uint32_t>(v, 0x2c5) = 0x3f800000;
        F<uint32_t>(v, 0x2c9) = 0;
        F<uint32_t>(v, 0x2cd) = 0;
        v[0x240] = 1;
        return;
    }
    v[0x240] = 1;
    static const uint32_t parts[4][2] = { { 0x5cb318, 0x2d1 }, { 0x5cb308, 0x2c5 }, { 0x5cb2f8, 0x2b9 }, { 0x5cb2e8, 0x2dd } };
    for (const auto& p : parts) {
        FormatText(path, (const char*)p[0], name, 0, 0, 0, 0, 0, 0, 0);
        if (!PackHas(path)) {
            v[0x240] = 0;
            return;
        }
        uint8_t* o = NewSceneObj(path, nullptr);
        uint8_t* mesh = F<uint8_t*>(o, 0x14);
        uint8_t* rec = mesh ? F<uint8_t*>(mesh, 0xc + F<int32_t>(o, 0x3a) * 0x18) + F<int32_t>(o, 0x21) * 0x45 : nullptr;
        const uint32_t* vert = *(const uint32_t**)rec;
        uint32_t* axis = (uint32_t*)(v + p[1]);
        axis[0] = vert[0];
        axis[1] = vert[1];
        axis[2] = vert[2];
        Vec3_Normalize((float*)axis);
        SceneObj_Destructor(o);
        ExeDelete(o);
    }
}
RT3_REPLACE(0x456920, Vehicle_LoadParts);

// 0x4575d0. Picks a random entry point and heading at the map edge (positions in
// world units, height above ground): over the land for +0x261 (north or south edge by
// season) else any of the four edges, heading towards a random point on the
// opposite edge.
RT3_NOINLINE void __fastcall Map_RandomEdgeEntry(uint8_t* o, int, float* outX, float* outY, float* outZ, float* heading) {
    int32_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    uint8_t* m;
    if (o[0x261]) {
        int32_t half = F<int32_t>(Map(), 0x2155) / 2;
        int32_t r = Rand(0, half);
        double q = (double)half * D(0x5c8628) + (double)r;
        __asm {
            fld q
            mov eax, 0x5a10d0
            call eax
            mov x0, eax
        }
        x1 = Rand(-10, 10) + x0;
        uint8_t month = g_world[0xf];
        if (month >= 1 && month <= 6) {
            y1 = F<int32_t>(Map(), 0x2159) - 2;
            y0 = 1;
        } else {
            y0 = F<int32_t>(Map(), 0x2159) - 2;
            y1 = 1;
        }
    } else {
        switch (Rand(0, 3)) {
        case 0:
            x0 = 1;
            y0 = Rand(1, F<int32_t>(Map(), 0x2159) - 2);
            x1 = F<int32_t>(Map(), 0x2155) - 2;
            y1 = Rand(1, F<int32_t>(Map(), 0x2159) - 2);
            break;
        case 1:
            x0 = F<int32_t>(Map(), 0x2155) - 2;
            y0 = Rand(1, F<int32_t>(Map(), 0x2159) - 2);
            x1 = 1;
            y1 = Rand(1, F<int32_t>(Map(), 0x2159) - 2);
            break;
        case 2:
            x0 = Rand(1, F<int32_t>(Map(), 0x2155) - 2);
            y0 = F<int32_t>(Map(), 0x2159) - 2;
            x1 = Rand(1, F<int32_t>(Map(), 0x2155) - 2);
            y1 = 1;
            break;
        case 3:
            x0 = Rand(1, F<int32_t>(Map(), 0x2155) - 2);
            y0 = 1;
            x1 = Rand(1, F<int32_t>(Map(), 0x2155) - 2);
            y1 = F<int32_t>(Map(), 0x2159) - 2;
            break;
        }
    }
    *outX = (float)(x0 * 0x5c);
    *outZ = (float)(y0 * 0x5c);
    F<uint32_t>(outY, 0) = F<uint32_t>(o, 0x26d);
    m = Map();
    if (x0 >= 0 && x0 < F<int32_t>(m, 0x15d9) - 1 && y0 >= 0 && y0 < F<int32_t>(m, 0x15d5) - 1)
        *outY = (float)(Terrain_HeightAtTile(m, 0, (float)x0, (float)y0) + *outY);
    float dx = (float)(x0 - x1), dy = (float)(y0 - y1);
    if (dy == Fl(0x5c8598)) dy = 0.01f;
    *heading = (float)((double(__cdecl*)(double, double))0x5a29d0)(dx, dy);
}
RT3_REPLACE(0x4575d0, Map_RandomEdgeEntry);

// 0x4578e0. Flies the aircraft for `dt`: forward along its heading at +0x275, its
// height kept around the cruise altitude +0x271 over the ground (pitching up when
// low, down when far above, nose limited to +-pi/8) with occasional random course
// changes (+0x23f left, timer +0x29d, heading target +0x2a1); pitch and heading ease
// toward their targets, the exhaust emitters are calmed and the props spun.
// Returns whether the final move succeeded (0x4588e0 passes it on).
RT3_NOINLINE bool __fastcall Aircraft_Fly(uint8_t* v, int, float dt) {
    float yaw = SceneObj_Angle2(v);
    float pitch = SceneObj_Angle1(v);
    float dx = (float)-(ExeCos(yaw) * F<float>(v, 0x275));
    float dz = (float)-(ExeSin(yaw) * F<float>(v, 0x275));
    float x, y, z;
    SceneObj_GetPos(v, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    float h = (float)Terrain_HeightAt(F<uint8_t*>(v, 4), 0, (float)((double)x + dx), (float)((double)z + dz));
    dx = (float)((double)dx * dt);
    float dy = (float)((double)dt * Fl_198(0x5c8598));
    dz = (float)((double)dz * dt);
    float target = (float)((double)y - h);
    double half = (double)F<float>(v, 0x271) * D(0x5c8628);
    if (!(target > half) && target == target) {
        y = (float)(h + half);
        target = (float)((double)F<float>(v, 0x271) - target);
    } else if (!(target > F<float>(v, 0x271)) && target == target) {
        float low = (float)((double)F<float>(v, 0x271) - target);
        double q = (double)low / Fl_198(0x5cb460) * D(0x5c8c20);
        target = (float)(D(0x5c85b8) > q ? 0.01 : q);
        if (!(target < Fl_198(0x5c8a28))) target = 0.3926991f;
        y = (float)((double)pitch / target * dt * (low / D(0x5c8d68)) + y);
    } else {
        double far = (double)F<float>(v, 0x26d) * D(0x5c8680);
        bool climbDown = false;
        if (!(far >= target)) {
            double far2 = (double)F<float>(v, 0x26d) * D(0x5c8640);
            climbDown = !(far2 >= target) || !(pitch > D(0x5c9bc0));
        }
        if (climbDown) {
            double q = (double)F<float>(v, 0x26d) * D(0x5c8660);
            if (!(target >= q)) target = 0.0f;
            else {
                float t = (float)(q - target);
                if (t < Fl_198(0x5cb45c)) t = -500.0f;
                target = -0.1f;
                y = (float)((double)pitch / Fl_198(0x5cb458) * dt * (t / D(0x5caad8)) + y);
            }
        } else {
            F<uint32_t>(&target, 0) = 0x3d6a0ea2; // ~0.0572
            y = (float)((double)pitch / Fl_198(0x5cb454) * dt * D(0x5c85f0) + y);
        }
    }
    float timer = (float)((double)F<float>(v, 0x29d) - dt);
    F<float>(v, 0x29d) = timer;
    if (v[0x23f] && !(timer > Fl_198(0x5c8598)) && timer == timer) {
        F<int32_t>(v, 0x29d) = 0;
        if (Rand(0, 0x1388) < 10) {
            --v[0x23f];
            F<float>(v, 0x29d) = 100.0f;
            float r = (float)Rand(-25, 25);
            F<float>(v, 0x2a1) = (float)((double)r / Fl_198(0x5c8c28) + F<float>(v, 0x2a1));
        }
    }
    if (target > Fl_198(0x5c8a28)) target = 0.3926991f;
    else if (target < Fl_198(0x5cb450)) target = -0.3926991f;
    pitch = (float)(((double)target - pitch) * dt * D(0x5c8628) + pitch);
    yaw = (float)(((double)F<float>(v, 0x2a1) - yaw) * dt * D(0x5c8758) + yaw);
    SceneObj_SetAngles(v, 0, Bits(pitch), Bits(yaw), 0);
    Obj_MoveTo(v, 0, x, y, z);
    F<uint32_t>(v, 0x2a5) = F<uint32_t>(v, 0x285);
    if (uint8_t* e = F<uint8_t*>(v, 0x2f6)) QuietEmitter(e, 0x30d4, 0x40800000);
    if (uint8_t* e = F<uint8_t*>(v, 0x2fa)) QuietEmitter(e, 0x5dc, 0x3f333333);
    if (v[0x261]) Vehicle_AnimateLinks(v, 0, dt);
    return Obj_MoveBy(v, 0, dx, dy, dz);
}
RT3_REPLACE(0x4578e0, Aircraft_Fly);

// 0x457d60 (vtable). Scene object o's position, lowered by +0x37e and
// 0x5cb464 while +0x261 is set.
void __fastcall SceneObj_GetLoweredPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    SceneObj_GetPos(o, 0, x, y, z);
    if (o[0x261]) {
        float* fy = (float*)y;
        *fy = (float)((double)*fy - (double)F<float>(o, 0x37e) - Fl_207(0x5cb464));
    }
}
RT3_REPLACE(0x457d60, SceneObj_GetLoweredPos);

// 0x457da0. (0, -2.5, 0) through the matrix +0x18e, scaled by +0x27d.
RT3_NOINLINE void __fastcall Obj_DownVector(uint8_t* o, int, float* out) {
    float v[3] = { 0.0f, -2.5f, 0.0f };
    float r[3];
    float s = F<float>(o, 0x27d);
    float* t = Vec3_Transform(v, 0, r, (const float*)(o + 0x18e));
    float z = (float)((double)s * t[2]);
    float y = (float)((double)s * t[1]);
    out[1] = y;
    out[2] = z;
    out[0] = (float)((double)s * t[0]);
}
RT3_REPLACE(0x457da0, Obj_DownVector);

// 0x457e10. (+0x2dd, +0x2e1, +0x2e5) scaled by 0x5cb468, through the matrix
// +0x18e, then scaled by +0x27d.
RT3_NOINLINE void __fastcall Obj_OffsetVector(uint8_t* o, int, float* out) {
    const float k = *(const float*)0x5cb468;
    float v[3] = { (float)((double)F<float>(o, 0x2dd) * k), (float)((double)F<float>(o, 0x2e1) * k),
                   (float)((double)F<float>(o, 0x2e5) * k) };
    float r[3];
    float s = F<float>(o, 0x27d);
    float* t = Vec3_Transform(v, 0, r, (const float*)(o + 0x18e));
    float z = (float)((double)s * t[2]);
    float y = (float)((double)s * t[1]);
    out[1] = y;
    out[2] = z;
    out[0] = (float)((double)s * t[0]);
}
RT3_REPLACE(0x457e10, Obj_OffsetVector);

// 0x457eb0. +0x2b9 through the matrix +0x18e, scaled by +0x27d then by +0x281.
RT3_NOINLINE void __fastcall Obj_ScaledPoint(uint8_t* o, int, float* out) {
    float r[3];
    float s = F<float>(o, 0x27d);
    float* t = Vec3_Transform((float*)(o + 0x2b9), 0, r, (const float*)(o + 0x18e));
    float a2 = (float)((double)s * t[2]);
    float a1 = (float)((double)s * t[1]);
    float a0 = (float)((double)s * t[0]);
    float s2 = F<float>(o, 0x281);
    out[2] = (float)((double)a2 * s2);
    out[1] = (float)((double)a1 * s2);
    out[0] = (float)((double)a0 * s2);
}
RT3_REPLACE(0x457eb0, Obj_ScaledPoint);

// 0x457f40. For unit vectors a, b with t = (a.b) * pi/2: sin t * cos t * pi
// (exe CRT 0x5a13e4 / 0x5a1494), returned unrounded.
RT3_NOINLINE double __stdcall Vec3_AngleTerm(float a0, float a1, float a2, float b0, float b1, float b2) {
    float a[3] = { a0, a1, a2 }, b[3] = { b0, b1, b2 };
    Vec3_Normalize(a);
    Vec3_Normalize(b);
    double dot = (double)a[2] * b[2] + (double)a[1] * b[1];
    dot = dot + (double)a[0] * b[0];
    float t = (float)(dot * *(const float*)0x5c8764);
    float f1 = (float)((double(__cdecl*)(double))0x5a13e4)(t);
    float f2 = (float)((double(__cdecl*)(double))0x5a1494)(t);
    return (double)f2 * f1 * *(const float*)0x5c8760;
}
RT3_REPLACE(0x457f40, Vec3_AngleTerm);

// 0x457fb0. Bending offset: (+0x2ad, +0x2b1, +0x2b5) scaled by -(+0x27d)
// and by (2 cos((a.b) pi/2))^2 + (+0x289) for unit a, b.
RT3_NOINLINE float* __fastcall Obj_BendOffset(uint8_t* o, int, float* out, float ax, float ay, float az, float bx, float by, float bz) {
    float a[3] = { ax, ay, az }, b[3] = { bx, by, bz };
    Vec3_Normalize(a);
    Vec3_Normalize(b);
    double dot = (double)a[2] * b[2] + (double)a[1] * b[1];
    dot = dot + (double)a[0] * b[0];
    float t = (float)(dot * *(const float*)0x5c8764);
    double c = ((double(__cdecl*)(double))0x5a13e4)(t);
    float k = (float)(c + c);
    float s = (float)-(double)F<float>(o, 0x27d);
    float l0 = (float)((double)s * F<float>(o, 0x2b5));
    float l1 = (float)((double)s * F<float>(o, 0x2b1));
    float l2 = (float)((double)s * F<float>(o, 0x2ad));
    float m = (float)((double)k * k + F<float>(o, 0x289));
    out[2] = (float)((double)l0 * m);
    out[1] = (float)((double)l1 * m);
    out[0] = (float)((double)l2 * m);
    return out;
}
RT3_REPLACE(0x457fb0, Obj_BendOffset);

// 0x458090. For the unit direction v: dot of transformed +0x2d1 with v, and
// lengths of (transformed +0x2b9) x v and (transformed +0x2c5) x v.
RT3_NOINLINE void __fastcall Obj_AxisTerms(uint8_t* o, int, float vx, float vy, float vz, float* dot, float* lenC, float* lenA) {
    const float* m = (const float*)(o + 0x18e);
    float a[3], b[3], c[3], v[3] = { vx, vy, vz };
    Vec3_Transform((float*)(o + 0x2b9), 0, a, m);
    Vec3_Transform((float*)(o + 0x2d1), 0, b, m);
    Vec3_Transform((float*)(o + 0x2c5), 0, c, m);
    Vec3_Normalize(v);
    Vec3_CrossInPlace(a, 0, v);
    Vec3_CrossInPlace(c, 0, v);
    *lenA = (float)Vec3_Length(a);
    double d = (double)b[2] * v[2] + (double)b[1] * v[1];
    *dot = (float)(d + (double)b[0] * v[0]);
    *lenC = (float)Vec3_Length(c);
}
RT3_REPLACE(0x458090, Obj_AxisTerms);

// 0x4581d0. Gives the object a private camera (+0x352) over its terrain and
// switches the view to it.
RT3_NOINLINE void __fastcall Obj_TakeCamera(uint8_t* o) {
    if (!F<void*>(o, 0x352)) {
        uint8_t* c = (uint8_t*)ExeNew(0x8a);
        F<uint8_t*>(o, 0x352) = c ? Camera_ConstructSetup(c, 0, F<uint32_t>(o, 4), 0, 0, 0, 0x44800000) : nullptr;
    }
    F<int32_t>(g_app, 0xa0) = 1;
    F<int32_t>(g_engine, 0x114216) = 1;
}
RT3_REPLACE(0x4581d0, Obj_TakeCamera);

// 0x458230. Drops the object's private camera (+0x352), handing the engine back the main camera.
RT3_NOINLINE void __fastcall Obj_DropCamera(uint8_t* o) {
    if (!F<void*>(o, 0x352)) return;
    F<void*>(g_engine, 0x28) = hook::As<void*>(g_62be68);
    if (uint32_t* c = F<uint32_t*>(o, 0x352)) {
        Camera_Destruct(c);
        ((void(__cdecl*)(void*))0x53b080)(c);
    }
    F<void*>(o, 0x352) = nullptr;
    F<int32_t>(g_app, 0xa0) = 0;
    F<int32_t>(g_engine, 0x114216) = 0;
}
RT3_REPLACE(0x458230, Obj_DropCamera);

// 0x4582a0. Loads vehicle v from stream s (marks 0x6979..0x697b): its world
// model, own data (slot 0x48), then rebuilds its parts and settles it.
// Returns the bytes read.
int32_t __fastcall Vehicle_Load(uint8_t* v, int, int32_t s) {
    NamedObj_Reset(v);
    int32_t n = StreamSkip4(s, 0x6979, 0, 0);
    n += WorldModel_Load(v, 0, s);
    n += StreamSkip4(s, 0x697a, 0, 0);
    ((void(__thiscall*)(uint8_t*, int32_t))(*(void***)v)[0x48 / 4])(v, s);
    Vehicle_LoadParts(v);
    Aircraft_Fly(v, 0, 0.0f);
    return StreamSkip4(s, 0x697b, 0, 0) + n;
}
RT3_REPLACE(0x4582a0, Vehicle_Load);

// 0x458310. Sets up vehicle v (id, 0x5c-byte type record) at (x, y, z)
// heading `heading` (with `atEdge`, a random map edge entry instead): its
// model and parts, a speed of 100, rotors turned to the heading, then
// settles it and counts it once (0x6d401c).
RT3_NOINLINE void __fastcall Vehicle_Init(uint8_t* v, int, uint32_t id, const uint32_t* type, int32_t atEdge, float x, float y, float z, float heading) {
    F<uint32_t>(v, 0x31e) = 0;
    F<uint32_t>(v, 0x31a) = 0;
    F<uint32_t>(v, 0x322) = 0;
    F<uint32_t>(v, 0x326) = 0;
    F<uint32_t>(v, 0x32a) = 0;
    v[0x2f5] = 0;
    F<uint32_t>(v, 0x352) = 0;
    F<uint32_t>(v, 0x37e) = 0;
    memset(v + 0x382, 0, 0x14);
    NamedObj_Reset(v);
    memcpy(v + 0x241, type, 0x5c);
    F<uint32_t>(v, 0x23a) = id;
    F<uint32_t>(v, 0x2ad) = 0;
    F<uint32_t>(v, 0x2b1) = 0;
    F<uint32_t>(v, 0x2b5) = 0;
    WorldModel_Init(v, 0, (const char*)v + 0x241, nullptr, (const char*)v + 0x241, 0, 0, 0);
    v[0x5c] |= 1;
    if (atEdge) Map_RandomEdgeEntry(v, 0, &x, &y, &z, &heading);
    F<uint32_t>(v, 0x352) = 0;
    Vehicle_LoadParts(v);
    F<uint32_t>(v, 0x2a5) = F<uint32_t>(v, 0x285);
    v[0x23f] = v[0x261] == 0;
    F<uint32_t>(v, 0x2a9) = 0;
    F<float>(v, 0x2a1) = heading;
    F<uint32_t>(v, 0x2f1) = 0;
    F<uint32_t>(v, 0x2ed) = 0;
    F<uint32_t>(v, 0x2e9) = 0;
    F<uint32_t>(v, 0x29d) = 0x42c80000;
    Obj_MoveTo(v, 0, x, y, z);
    if (v[0x240]) {
        F<float>(v, 0x2ad) = (float)(((double(__cdecl*)(double))0x5a13e4)(heading) * *(const double*)0x5c85f0);
        F<float>(v, 0x2b5) = (float)(((double(__cdecl*)(double))0x5a1494)(heading) * *(const double*)0x5c85f0);
        SceneObj_RotateAxis(v, 0, F<float>(v, 0x2b9), F<float>(v, 0x2bd), F<float>(v, 0x2c1), heading);
        SceneObj_ApplyQuat(v);
        SceneObj_ResetQuat(v);
    }
    SceneObj_SetAngles(v, 0, 0, *(uint32_t*)&heading, 0);
    Aircraft_Fly(v, 0, 0.0f);
    if (!v[0x23e]) {
        v[0x23e] = 1;
        Obj_Decc60(g_windowManager);
    }
}
RT3_REPLACE(0x458310, Vehicle_Init);

// 0x4584e0. Camera-carrying object o: stops the camera following it, drops
// its camera and tears down the scene object.
RT3_NOINLINE void __fastcall CameraObj_Teardown(uint8_t* o) {
    if (g_world && g_62be68 && F<uint8_t*>(g_62be68, 0x60) == o)
        World_SetFollow(g_world, 0, 0, o);
    Obj_DropCamera(o);
    SceneObj_Teardown(o);
}
RT3_REPLACE(0x4584e0, CameraObj_Teardown);

// 0x458520. Cab view of train object o: steers the look angles with the
// numpad / arrow keys over dt - pitch +0x2ed within +-pi/4 of +0x28d,
// yaw +0x2f1 and roll +0x2e9 within +-pi/4 - easing back when released
// (towards the rest pitch, or to zero; quickly with +0x2f5). ']' / '['
// raise / lower the zoom +0x2a5 by a third of +0x285 a second, 'p' resets it,
// ';' maxes it. 'c' toggles the cab camera: leaving it (+0x352) drops the
// camera, clears the cab options and follows the train zoomed to 200;
// otherwise the camera is taken.
RT3_NOINLINE void __fastcall TrainObj_CabKeys(uint8_t* o, int, float dt) {
    auto key = [](uint8_t k) { return Engine_KeyState(g_engine, 0, k); };
    float* pitch = &F<float>(o, 0x2ed);
    if (key(0x62) || key(0x28)) {
        if ((double)F<float>(o, 0x28d) + Fl_207(0x5cb470) > (double)*pitch) *pitch = (float)((double)dt + *pitch);
    } else if (key(0x68) || key(0x26)) {
        if ((double)F<float>(o, 0x28d) - Fl_207(0x5cb470) < (double)*pitch) *pitch = (float)((double)*pitch - dt);
    } else if (o[0x2f5])
        *pitch = (float)((double)*pitch - ((double)*pitch - F<float>(o, 0x28d)) * dt * Fl_207(0x5c85f8));
    else
        *pitch = (float)((double)*pitch - ((double)*pitch - F<float>(o, 0x28d)) * dt * D(0x5c86b8));
    for (int32_t axis = 0; axis < 2; ++axis) {
        float* a = &F<float>(o, axis ? 0x2e9 : 0x2f1);
        uint8_t up1 = axis ? 0x69 : 0x66, up2 = axis ? 0x21 : 0x27, dn1 = axis ? 0x67 : 0x64, dn2 = axis ? 0x24 : 0x25;
        if (key(up1) || key(up2)) {
            if (*a < Fl_207(0x5cb470)) *a = (float)((double)dt + *a);
        } else if (key(dn1) || key(dn2)) {
            if (*a > Fl_207(0x5cb46c)) *a = (float)((double)*a - dt);
        } else if (o[0x2f5])
            *a = (float)((double)*a - (double)dt * *a * Fl_207(0x5c85f8));
        else
            F<uint32_t>(o, axis ? 0x2e9 : 0x2f1) = 0;
    }
    auto scan = hook::Global<int16_t(__stdcall*)(char)>(0x5c8240);
    if (key((uint8_t)scan(']'))) {
        float v = (float)((double)F<float>(o, 0x285) / D(0x5c85c8));
        if (F<float>(o, 0x2a5) < F<float>(o, 0x285)) F<float>(o, 0x2a5) = (float)((double)v * dt + F<float>(o, 0x2a5));
    } else if (key((uint8_t)scan('['))) {
        float v = (float)((double)F<float>(o, 0x285) / D(0x5c85c8));
        if (F<float>(o, 0x2a5) > 0.0f) F<float>(o, 0x2a5) = (float)((double)F<float>(o, 0x2a5) - (double)v * dt);
        else F<uint32_t>(o, 0x2a5) = 0;
    }
    if (key((uint8_t)scan('p'))) F<uint32_t>(o, 0x2a5) = 0;
    if (key((uint8_t)scan(';'))) F<uint32_t>(o, 0x2a5) = F<uint32_t>(o, 0x285);
    if (!key((uint8_t)scan('c'))) return;
    Engine_KeyUp(g_engine, 0, (uint8_t)scan('c'));
    if (F<int32_t>(o, 0x352)) {
        Obj_DropCamera(o);
        F<int32_t>(g_app, 0xa0) = 0;
        F<int32_t>(g_engine, 0x114216) = 0;
        World_FollowEntity(g_world, 0, 1, 3, F<int32_t>(o, 0x23a));
        Camera_SetZoom(g_62be68, 0, 0x43480000);
        return;
    }
    World_SetFollow(g_world, 0, 0, nullptr);
    Obj_TakeCamera(o);
}
RT3_REPLACE(0x458520, TrainObj_CabKeys);

 // namespace

// 0x4588e0. Advances driven vehicle v by dt (in game minutes; t = dt / 1440
// days): lights, wheels, then aircraft fly on their own (+0x23e). Without a
// model (+0x240) it reports error 0x1ac and returns false. The engine sound
// (+0x2fe) and the six smoke emitters follow the throttle (+0x2a5 / +0x285),
// the cab keys are read, and a vehicle below the ground + 5 is lifted out
// with its velocity (+0x2ad) damped. Rail vehicles (+0x2f5) steer by the
// down vector and the bend of the track, with the cab view (+0x31a / +0x31e
// following the mouse, +0x322 the wipers on key 0x400 with +0x326 / +0x32a)
// and speed / throttle readouts; free vehicles steer by their axis terms
// with drag 0.7 - 0.7 / (1 + |v|) and a four-value readout, and feed their
// exhaust emitters (+0x2f6 / +0x2fa). Both rotate about their three axes,
// move by -velocity and update the chase camera +0x352. Returns whether the
// move succeeded (false removes the vehicle).
RT3_NOINLINE bool __fastcall Vehicle_DriveUpdate(uint8_t* v, int, float dt) {
    float t = (float)((double)dt / Fl4588(0x5cb3f4));
    Vehicle_BlinkLights(v, 0, t);
    Vehicle_SpinWheels(v, 0, t);
    if (v[0x23e]) return Aircraft_Fly(v, 0, t);
    if (!v[0x240]) {
        const char* caption = GetString(0xa2);
        const char* text = GetString(0x1ac);
        ((int(__stdcall*)(void*, const char*, const char*, uint32_t))g_5c8220)(nullptr, text, caption, 0);
        return false;
    }
    float throttle = (float)((double)F<float>(v, 0x2a5) / F<float>(v, 0x285));
    if (uint8_t* ch = F<uint8_t*>(v, 0x2fe)) {
        Channel_SetVolume(ch, 0, Bits4588(throttle));
        int32_t rate;
        __asm {
            fld throttle
            fmul dword ptr ds:[0x5c89a0]
            fadd dword ptr ds:[0x5ca1c0]
            mov eax, 0x5a10d0
            call eax
            mov rate, eax
        }
        Channel_SetRate(F<uint8_t*>(v, 0x2fe), 0, rate);
    }
    for (int32_t k = 0; k < 6; ++k)
        if (uint8_t* e = F<uint8_t*>(v, 0x356 + k * 4)) Obj_SetUnit25(e, 0, throttle);
    TrainObj_CabKeys(v, 0, t);
    float px, py, pz;
    uint32_t lift = 0x3ffae148;
    SceneObj_GetPos(v, 0, (uint32_t*)&px, (uint32_t*)&py, (uint32_t*)&pz);
    float ground = (float)(Terrain_HeightAt(F<uint8_t*>(v, 4), 0, px, pz) + Fl4588(0x5c8554));
    float* vel = &F<float>(v, 0x2ad);
    if (!(ground <= py)) {
        lift = 0;
        float a = (float)((double)t * vel[2]), b = (float)((double)t * vel[1]), c = (float)((double)t * vel[0]);
        a = (float)((double)a * Fl4588(0x5cb4a8));
        b = (float)((double)b * Fl4588(0x5cb4a8));
        c = (float)((double)c * Fl4588(0x5cb4a8));
        vel[0] = vel[0] - c;
        vel[1] = vel[1] - b;
        vel[2] = vel[2] - a;
        F<uint32_t>(v, 0x2b1) = 0;
        RotateBits(v, 0x2b9, (float)((double)t * F<float>(v, 0x2e9)));
        Obj_MoveTo(v, 0, px, ground, pz);
    }
    float scratch[3], cross[3], axis[3];
    float tmp[3];
    if (v[0x2f5]) {
        if (!(ground <= py)) {
            float a = (float)((double)t * vel[2]), b = (float)((double)t * vel[1]), c = (float)((double)t * vel[0]);
            vel[0] = vel[0] - c;
            vel[1] = vel[1] - b;
            vel[2] = vel[2] - a;
            SceneObj_ScaleRotation(v, 0, (float)((double)t * Fl4588(0x5c8780)));
        }
        float down[3];
        Obj_DownVector(v, 0, down);
        const uint32_t* db = (const uint32_t*)down;
        const uint32_t* vb = (const uint32_t*)vel;
        float* bend = ((BendBitsFn)&Obj_BendOffset)(v, 0, cross, db[0], db[1], db[2], vb[0], vb[1], vb[2]);
        float bz = (float)((double)bend[2] * Fl4588(0x5c8d48));
        float by = (float)((double)bend[1] * Fl4588(0x5c8d48));
        float bx = (float)((double)bend[0] * Fl4588(0x5c8d48));
        uint32_t nv[3] = { vb[0], vb[1], vb[2] };
        float* n = Vec3_Normalize((float*)nv);
        double along = ((double)down[2] * n[2] + (double)down[1] * n[1]) + (double)down[0] * n[0];
        float grip = (float)(((double(__cdecl*)(double))0x5a152e)(along) * D4588(0x5c85f0) + D4588(0x5c86b8));
        float gz = (float)((double)grip * bz), gy = (float)((double)grip * by), gx = (float)((double)grip * bx);
        float speed = F<float>(v, 0x2a5);
        float sz = (float)((double)down[2] * speed), sy = (float)((double)down[1] * speed), sx = (float)((double)speed * down[0]);
        sy = *(float*)&lift + sy;
        uint32_t up[3] = { 0, 0xc0000000, 0 };
        sz = sz + gz;
        sy = sy + gy;
        sx = sx + gx;
        float dz = (float)((double)sz * t), dy = (float)((double)sy * t), dx = (float)((double)sx * t);
        vel[0] = dx + vel[0];
        vel[1] = dy + vel[1];
        vel[2] = dz + vel[2];
        float len;
        Vec3_Transform((float*)up, 0, tmp, &F<float>(v, 0x18e));
        WorldCross(v, cross, down, tmp, &len);
        SceneObj_ScaleRotation(v, 0, (float)((double)t * Fl4588(0x5c8554)));
        RotateBits(v, 0x2d1, (float)((double)F<float>(v, 0x2f1) * F<float>(v, 0x291) * t));
        RotateBits(v, 0x2c5, (float)((double)F<float>(v, 0x2ed) * F<float>(v, 0x295) * t));
        RotateBits(v, 0x2b9, (float)((double)F<float>(v, 0x2e9) * F<float>(v, 0x299) * t));
        SceneObj_ApplyQuat(v);
        float k = (float)((double)t * Fl4588(0x5cb3f4) / Fl4588(0x5c8c40));
        SceneObj_ScaleRotation(v, 0, k + k);
        float mx = (float)-((double)k * vel[0]), my = (float)-((double)k * F<float>(v, 0x2b1)), mz = (float)-((double)k * F<float>(v, 0x2b5));
        bool moved = Obj_MoveBy(v, 0, mx, my, mz);
        float mouseX, mouseY;
        if (uint8_t* cab = F<uint8_t*>(v, 0x31a)) {
            Mouse_GetPos((uint8_t*)Engine_Input(hook::As<void*>(g_engine)), 0, (uint32_t*)&mouseX, (uint32_t*)&mouseY);
            float yaw = (float)(((double)mouseX - D4588(0x5c8628)) * D4588(0x5cb4a0));
            SceneObj_SetAngles(cab, 0, 0, Bits4588(yaw), 0);
            if (uint8_t* head = F<uint8_t*>(v, 0x31e)) {
                float pitch = (float)(((double)mouseY - D4588(0x5c8628)) * D4588(0x5cb498));
                SceneObj_SetAngles(head, 0, Bits4588(pitch), 0, 0);
                if (uint8_t* wiper = F<uint8_t*>(v, 0x322)) {
                    Engine_Input(hook::As<void*>(g_engine));
                    if (Input_Flag400()) {
                        float a;
                        SceneObj_GetAngles(F<uint8_t*>(v, 0x322), 0, nullptr, nullptr, (uint32_t*)&a);
                        a = (float)((double)t * Fl4588(0x5c873c) + a);
                        a = (float)((double(__cdecl*)(double, double))0x5a1390)(a, 12.566370964050293);
                        SceneObj_SetAngles(F<uint8_t*>(v, 0x322), 0, 0, 0, Bits4588(a));
                        Flags24_Set1(F<uint8_t*>(v, 0x326));
                        float r = (float)(((int(__cdecl*)())0x5a29ab)() & 0xff);
                        r = (float)((double)r / Fl4588(0x5cb490) + D4588(0x5c86b8));
                        Obj_SetUnit25(F<uint8_t*>(v, 0x326), 0, r);
                        if (uint8_t* s = F<uint8_t*>(v, 0x32a)) Channel_SetLoop(s, 0, 0);
                    } else {
                        if (uint8_t* s = F<uint8_t*>(v, 0x32a)) Channel_SetLoop(s, 0, 1);
                        if (uint8_t* w = F<uint8_t*>(v, 0x326)) Flags24_Clear(w);
                    }
                }
            }
        }
        char speedText[0x10], throttleText[0x10], line[0x100];
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(speedText, (const char*)0x5cb488, Vec3_Length(vel) * Fl4588(0x5c9ac8));
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(throttleText, (const char*)0x5cb488, (double)throttle * Fl4588(0x5ca19c));
        FormatText(line, GetString(0x1ad), speedText, throttleText, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        DriveStatus(line, 0x3f666666);
        DriveCamera(v);
        if (uint8_t* cam = F<uint8_t*>(v, 0x352)) {
            SceneObj_GetPos(v, 0, (uint32_t*)&px, (uint32_t*)&py, (uint32_t*)&pz);
            const float* m = &F<float>(v, 0x18e);
            float* o = Vec3_Transform(&F<float>(v, 0x2b9), 0, scratch, m);
            float oz = (float)((double)o[2] * Fl4588(0x5c8554)), oy = (float)((double)o[1] * Fl4588(0x5c8554)), ox = (float)((double)o[0] * Fl4588(0x5c8554));
            float cx = ox + px, cy = oy + py, cz = oz + pz;
            o = Vec3_Transform(&F<float>(v, 0x2d1), 0, scratch, m);
            oz = (float)((double)o[2] * Fl4588(0x5c8568)), oy = (float)((double)o[1] * Fl4588(0x5c8568)), ox = (float)((double)o[0] * Fl4588(0x5c8568));
            cx = cx - ox, cy = cy - oy, cz = cz - oz;
            Camera_SetTarget(cam, 0, cx, cy, cz);
            o = Vec3_Transform(&F<float>(v, 0x2d1), 0, scratch, m);
            oz = (float)((double)o[2] * Fl4588(0x5cb484)), oy = (float)((double)o[1] * Fl4588(0x5cb484)), ox = (float)((double)o[0] * Fl4588(0x5cb484));
            cx = cx - ox, cy = cy - oy, cz = cz - oz;
            Camera_SetEye(F<uint8_t*>(v, 0x352), 0, Bits4588(cx), Bits4588(cy), Bits4588(cz));
            uint32_t upv[3];
            Vec3_Transform(&F<float>(v, 0x2b9), 0, (float*)upv, m);
            Obj_SetXYZ2c(F<uint8_t*>(v, 0x352), 0, upv[0], upv[1], upv[2]);
            F<uint8_t*>(g_engine, 0x28) = F<uint8_t*>(v, 0x352);
        }
        return moved;
    }
    // Free model.
    float dot, lenC, lenA;
    const uint32_t* vb = (const uint32_t*)vel;
    ((AxisBitsFn)&Obj_AxisTerms)(v, 0, vb[0], vb[1], vb[2], &dot, &lenC, &lenA);
    float off[3], pt[3];
    Obj_OffsetVector(v, 0, off);
    Obj_ScaledPoint(v, 0, pt);
    const uint32_t* pb = (const uint32_t*)pt;
    float angle = (float)((AngleBitsFn)&Vec3_AngleTerm)(pb[0], pb[1], pb[2], vb[0], vb[1], vb[2]);
    float bend[3];
    ((BendBitsFn)&Obj_BendOffset)(v, 0, bend, pb[0], pb[1], pb[2], vb[0], vb[1], vb[2]);
    float len = 0.0f;
    WorldCross(v, cross, vel, &F<float>(v, 0x2dd), &len);
    float cz = (float)((double)pt[2] * lenC);
    uint32_t dir[3];
    memcpy(dir, off, sizeof dir);
    float cy = (float)((double)pt[1] * lenC), cx = (float)((double)pt[0] * lenC);
    cz = (float)((double)cz * angle);
    cy = (float)((double)cy * angle);
    cx = (float)((double)cx * angle);
    float neg = -len;
    cz = (float)((double)cz * neg);
    cy = (float)((double)cy * neg);
    cx = (float)((double)cx * neg);
    cy = *(float*)&lift + cy;
    float sz = bend[2] + cz, sy = bend[1] + cy, sx = cx + bend[0];
    Vec3_Normalize(off);
    uint32_t nvb[3] = { vb[0], vb[1], vb[2] };
    float* nv = (float*)nvb;
    Vec3_Normalize(nv);
    float facing = (float)(((double)nv[2] * off[2] + (double)nv[1] * off[1]) + (double)nv[0] * off[0]);
    float speed = F<float>(v, 0x2a5);
    if (speed < Fl4588(0x5c8598)) speed = 0.0f;
    float* d = (float*)dir;
    float fz = (float)((double)d[2] * speed), fy = (float)((double)d[1] * speed), fx = (float)((double)d[0] * speed);
    sx = fx + sx;
    sy = fy + sy;
    sz = fz + sz;
    float dz = (float)((double)sz * t), dy = (float)((double)sy * t), dx = (float)((double)sx * t);
    vel[0] = dx + vel[0];
    vel[1] = dy + vel[1];
    vel[2] = dz + vel[2];
    float drag = (float)(D4588(0x5c8770) / (Vec3_Length(vel) + D4588(0x5c8770)));
    float keep = (float)(D4588(0x5c8680) - (double)drag * Fl4588(0x5cb480));
    facing = (float)((double)keep * facing);
    float spin = (float)(Vec3_Length(vel) * D4588(0x5c85a0));
    spin = (float)((double)spin * F<float>(v, 0x27d) * t);
    float k = (float)((double)t * Fl4588(0x5cb3f4) / Fl4588(0x5c8c40));
    SceneObj_ScaleRotation(v, 0, k + k);
    RotateBits(v, 0x2d1, (float)((double)F<float>(v, 0x2f1) * F<float>(v, 0x291) * spin * dot));
    RotateBits(v, 0x2c5, (float)((double)F<float>(v, 0x2ed) * F<float>(v, 0x295) * spin * lenC));
    RotateBits(v, 0x2b9, (float)((double)F<float>(v, 0x2e9) * F<float>(v, 0x299) * spin * lenA));
    {
        const uint32_t* c = (const uint32_t*)cross;
        ((RotateBitsFn)&SceneObj_RotateAxis)(v, 0, c[0], c[1], c[2], (float)-((double)facing * F<float>(v, 0x27d) * len * t));
    }
    SceneObj_ApplyQuat(v);
    char t1[0x10], t2[0x10], t3[0x10], t4[0x10], line[0x100];
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(t1, (const char*)0x5cb488, Vec3_Length(vel) * Fl4588(0x5c9ac8));
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(t2, (const char*)0x5cb478, (double)dot);
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(t3, (const char*)0x5cb478, (double)lenC);
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(t4, (const char*)0x5cb478, (double)lenA);
    FormatText(line, GetString(0x1ae), t1, t2, t3, t4, nullptr, nullptr, nullptr, nullptr);
    DriveStatus(line, 0x3f4ccccd);
    float mx = (float)-((double)k * vel[0]), my = (float)-((double)k * F<float>(v, 0x2b1)), mz = (float)-((double)k * F<float>(v, 0x2b5));
    if (uint8_t* e = F<uint8_t*>(v, 0x2f6)) {
        float ez = (float)((double)t * vel[2]), ey = (float)((double)t * vel[1]), ex = (float)((double)t * vel[0]);
        F<float>(e, 0x70) = ex, F<float>(e, 0x74) = ey, F<float>(e, 0x78) = ez;
        F<uint32_t>(e, 0x7c) = 0, F<uint32_t>(e, 0x80) = 0, F<uint32_t>(e, 0x84) = 0;
    }
    if (uint8_t* e = F<uint8_t*>(v, 0x2fa)) {
        View_SetParams(e, 0, 0x1f4, 0x64, 0x3f333333, 0x3e4ccccd, 0x3f333333, 0x3f000000);
        uint8_t* e2 = F<uint8_t*>(v, 0x2fa);
        for (int32_t o = 0x70; o <= 0x84; o += 4) F<uint32_t>(e2, o) = 0;
    }
    bool moved = Obj_MoveBy(v, 0, mx, my, mz);
    DriveCamera(v);
    uint32_t upv[3];
    if (uint8_t* cam = F<uint8_t*>(v, 0x352)) {
        SceneObj_GetPos(v, 0, (uint32_t*)&px, (uint32_t*)&py, (uint32_t*)&pz);
        const float* m = &F<float>(v, 0x18e);
        float* o = Vec3_Transform(&F<float>(v, 0x2b9), 0, scratch, m);
        memcpy(upv, o, sizeof upv);
        float qx = px, qy = py, qz = pz;
        Vec3_Normalize((float*)upv);
        const float* u = (const float*)upv;
        float az = (float)((double)u[2] * Fl4588(0x5c8554)), ay = (float)((double)u[1] * Fl4588(0x5c8554)), ax = (float)((double)u[0] * Fl4588(0x5c8554));
        qx = ax + qx, qy = ay + qy, qz = az + qz;
        float* w = Vec3_Transform(&F<float>(v, 0x2dd), 0, scratch, m);
        memcpy(axis, w, sizeof axis);
        uint8_t* rec = F<uint8_t*>(v, 0x14) ? F<uint8_t*>(F<uint8_t*>(v, 0x14), F<int32_t>(v, 0x3a) * 0x18 + 0xc) + F<int32_t>(v, 0x21) * 0x45 : nullptr;
        float dist = F<float>(rec, 0x14);
        float tz = (float)((double)axis[2] * dist), ty = (float)((double)axis[1] * dist), tx = (float)((double)dist * axis[0]);
        qx = qx - tx, qy = qy - ty, qz = qz - tz;
        Camera_SetTarget(cam, 0, qx, qy, qz);
        tz = (float)((double)axis[2] * Fl4588(0x5cb474)), ty = (float)((double)axis[1] * Fl4588(0x5cb474)), tx = (float)((double)axis[0] * Fl4588(0x5cb474));
        qx = qx - tx, qy = qy - ty, qz = qz - tz;
        Camera_SetEye(F<uint8_t*>(v, 0x352), 0, Bits4588(qx), Bits4588(qy), Bits4588(qz));
        Obj_SetXYZ2c(F<uint8_t*>(v, 0x352), 0, upv[0], upv[1], upv[2]);
        F<uint8_t*>(g_engine, 0x28) = F<uint8_t*>(v, 0x352);
        return moved;
    }
    Vec3_Normalize(pt);
    return moved;
}
RT3_REPLACE(0x4588e0, Vehicle_DriveUpdate);

// 0x459bc0. Mask in EAX, object in ECX, clear on the stack.
__declspec(naked) void ObjFlags_Set() {
    __asm {
        push [esp + 4]
        push ecx
        push eax
        call ObjFlags_SetImpl
        add esp, 12
        ret 4
    }
}
RT3_REPLACE(0x459bc0, ObjFlags_Set);

// 0x459c30. Object in EBX, table in EDX (kept).
__declspec(naked) void EntryWalk_Find() {
    __asm {
        push edx
        push edx
        push ebx
        call EntryWalk_FindImpl
        add esp, 8
        pop edx
        ret
    }
}
RT3_REPLACE(0x459c30, EntryWalk_Find);

// 0x459cb0. Among the blocks of the current iteration (0x6acd4c, stride
// 0x168) flagged 0x10 with the same key as `b`: the first one after `b`,
// else the first one before it, else b.
RT3_NOINLINE uint8_t* __fastcall BlockIter_NextSameKey(uint8_t* b) {
    uint8_t* p = g_6acd4c;
    int32_t n = F<int32_t>(g_6acd50, 8);
    uint8_t* r = b;
    for (int32_t i = 0; i < n; ++i, p += 0x168) {
        if (!(p[0x8c] & 0x10) || F<int32_t>(p, 0) != F<int32_t>(b, 0) || p == b) continue;
        if (p > b) return p;
        if (r == b) r = p;
    }
    return r;
}
RT3_REPLACE(0x459cb0, BlockIter_NextSameKey);

// 0x459d00. Value in EAX, object and v2 on the stack.
__declspec(naked) void Attach_SetValue() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call Attach_SetValueImpl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x459d00, Attach_SetValue);

// 0x459e10. Syncs o's element flags to its groups (+0xc8, then +0xd8).
RT3_NOINLINE void __stdcall Obj_SyncGroupFlags(uint8_t* o, int32_t full) {
    SyncGroups(o, (const uint32_t*)(o + 0xc8), full, 1);
    SyncGroups(o, (const uint32_t*)(o + 0xd8), full, 0);
}
RT3_REPLACE(0x459e10, Obj_SyncGroupFlags);

// 0x45a000. Clears flag 0x800 of the four blocks +0xa8.. and sets it on the four +0xb8..
RT3_NOINLINE void __fastcall Obj_SwapBlockFlags(void*, int, uint8_t* o) {
    for (int k = 0; k < 4; ++k)
        if (uint8_t* b = F<uint8_t*>(o, 0xa8 + k * 4)) UpdateBlockEntry(b, false);
    for (int k = 0; k < 4; ++k)
        if (uint8_t* b = F<uint8_t*>(o, 0xb8 + k * 4)) UpdateBlockEntry(b, true);
}
RT3_REPLACE(0x45a000, Obj_SwapBlockFlags);

// 0x45a0d0. Entry in ESI, side on the stack.
__declspec(naked) void Block_FinishSide() {
    __asm {
        push [esp + 4]
        push esi
        call Block_FinishSideImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x45a0d0, Block_FinishSide);

// 0x45a280. Counter in EAX, entry in EDX, mask in ECX, result in EDI; value
// (float) and side on the stack; the float comes back in ST0.
__declspec(naked) void Block_CountSide() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push edi
        push ecx
        push edx
        push eax
        call Block_CountSideImpl
        add esp, 24
        mov [esp + 4], eax
        fld dword ptr [esp + 4]
        ret 8
    }
}
RT3_REPLACE(0x45a280, Block_CountSide);

// 0x45a2e0. Starts a block iteration of o and clears (mode 1) or sets `mask`
// in the flags +0x8c of every block.
RT3_NOINLINE void __fastcall BlockIter_SetFlags(uint8_t* o, uint32_t mask, int32_t mode) {
    g_6acd44 = 0;
    g_6acd48 = 0;
    g_6acd54 = 0;
    g_6acd50 = o;
    hook::As<int32_t>(g_6acd4c) = 0;
    int32_t n = F<int32_t>(o, 0xc);
    uint8_t* p = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    g_6acd4c = p;
    for (int32_t i = 0; i < F<int32_t>(o, 8); ++i, p += 0x168) {
        if (mode == 1) F<uint32_t>(p, 0x8c) &= ~mask;
        else F<uint32_t>(p, 0x8c) |= mask;
        o = g_6acd50;
    }
}
RT3_REPLACE(0x45a2e0, BlockIter_SetFlags);

// 0x45a380. Starts a block iteration of o (see 0x45a660) and tests `mask`
// against the flags +0x8c of the first block. QUIRK: a negative index walks
// 2^31 blocks back as in the original.
RT3_NOINLINE int32_t __fastcall BlockIter_BeginTest(uint8_t* o, uint32_t mask, int32_t idx) {
    g_6acd44 = 0;
    g_6acd48 = 0;
    g_6acd54 = 0;
    g_6acd50 = o;
    hook::As<int32_t>(g_6acd4c) = 0;
    int32_t n = F<int32_t>(o, 0xc);
    uint8_t* p = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    g_6acd4c = p;
    uint32_t u = (uint32_t)idx;
    if ((int32_t)u < 0) {
        do {
            --u;
            p += 0x168;
        } while ((int32_t)u < 0);
    }
    return (F<uint32_t>(p, 0x8c) & mask) ? 1 : 0;
}
RT3_REPLACE(0x45a380, BlockIter_BeginTest);

RT3_NOINLINE void Clear6acd60() { g_6acd60 = 0; }
RT3_REPLACE(0x45a3f0, Clear6acd60);

// 0x45a400. Starts a block iteration and clears slot idx (-1: all four) of
// the first block's +0xf4 for each entry, unmarking its element (0x475240)
// and flagging the entry 0x20000 (then virtual slot 4 or 3).
RT3_NOINLINE void __fastcall BlockIter_ClearSlot(uint8_t* o, int32_t idx) {
    g_6acd44 = 0;
    g_6acd48 = 0;
    g_6acd54 = 0;
    g_6acd50 = o;
    hook::As<int32_t>(g_6acd4c) = 0;
    int32_t n = F<int32_t>(o, 0xc);
    g_6acd4c = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    for (int32_t i = 0; i < F<int32_t>(g_6acd50, 0xc); ++i) {
        uint8_t* entry = o + 0x10 + i * 0xc;
        if (!F<void*>(entry, 8)) continue;
        if (idx == -1) {
            for (int k = 0; k < 4; ++k) g_6acd4c[0xf4 + k] = 0;
        } else {
            g_6acd4c[0xf4 + idx] = 0;
        }
        Obj_SetElementFlag1d8(F<void*>(entry, 8), 0, idx);
        uint8_t* it = g_6acd50;
        uint8_t* obj = F<uint8_t*>(entry, 0);
        uint8_t* e = it + 0x10;
        for (int32_t j = 0; j < F<int32_t>(it, 0xc); ++j, e += 0xc) {
            if (F<uint8_t*>(e, 0) != obj) continue;
            uint32_t& f = F<uint32_t>(e, 4);
            f |= 0x20000;
            if (f & 0x800) ((void(__thiscall*)(void*))(*(void***)obj)[0x10 / 4])(obj);
            else if (!(f & 0x2000)) ((void(__thiscall*)(void*))(*(void***)obj)[0xc / 4])(obj);
            break;
        }
    }
}
RT3_REPLACE(0x45a400, BlockIter_ClearSlot);

// 0x45a520. Starts a block iteration and marks slot idx (-1: all four) in
// the first block's +0xf4; with flag 1 also updates each entry's element (0x475240).
RT3_NOINLINE void __fastcall BlockIter_MarkSlot(uint8_t* o, int32_t idx, int32_t flag) {
    g_6acd44 = 0;
    g_6acd48 = 0;
    g_6acd54 = 0;
    g_6acd50 = o;
    hook::As<int32_t>(g_6acd4c) = 0;
    int32_t n = F<int32_t>(o, 0xc);
    g_6acd4c = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    for (int32_t i = 0; i < F<int32_t>(g_6acd50, 0xc); ++i) {
        void* el = F<void*>(o, 0x18 + i * 0xc);
        if (!el) continue;
        uint8_t* blk = g_6acd4c;
        if (idx == -1) {
            for (int k = 0; k < 4; ++k) g_6acd4c[0xf4 + k] = 1;
        } else {
            blk[0xf4 + idx] = 1;
        }
        if (flag == 1) Obj_SetElementFlag1d8(el, 1, idx);
    }
}
RT3_REPLACE(0x45a520, BlockIter_MarkSlot);

// 0x45a5f0. Table in ECX.
__declspec(naked) void EntryWalk_Begin() {
    __asm {
        push edx
        push ecx
        call EntryWalk_BeginImpl
        add esp, 8
        ret
    }
}
RT3_REPLACE(0x45a5f0, EntryWalk_Begin);

// 0x45a660. Starts an iteration over the block list o: resets the cursor
// globals 0x6acd44..0x6acd54 and returns the first block's +0x84.
RT3_NOINLINE uint32_t __fastcall BlockIter_Begin(uint8_t* o) {
    g_6acd44 = 0;
    g_6acd48 = 0;
    g_6acd54 = 0;
    hook::As<int32_t>(g_6acd4c) = 0;
    g_6acd50 = o;
    int32_t n = F<int32_t>(o, 0xc);
    uint8_t* p = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    g_6acd4c = p;
    return F<uint32_t>(p, 0x84);
}
RT3_REPLACE(0x45a660, BlockIter_Begin);

// 0x45a6b0. Unlinks `node` (after `prev`, or the head when prev is null)
// from a singly linked list {head, tail, count}; returns node.
void** __fastcall SList_Unlink(void** l, int, void** prev, void** node) {
    if (prev) {
        *prev = *node;
        *node = nullptr;
        ((int32_t*)l)[2]--;
        if (node == (void**)l[1]) l[1] = prev;
        return node;
    }
    void** n = (void**)l[0];
    if (n) {
        l[0] = *n;
        if (n == (void**)l[1]) l[1] = *n;
        ((int32_t*)l)[2]--;
    }
    return node;
}
RT3_REPLACE(0x45a6b0, SList_Unlink);

// 0x45a700. Output in EAX, mode in ECX, object on the stack.
__declspec(naked) void Attach_SumVectors() {
    __asm {
        push [esp + 4]
        push ecx
        push eax
        call Attach_SumVectorsImpl
        add esp, 12
        ret 4
    }
}
RT3_REPLACE(0x45a700, Attach_SumVectors);

// 0x45a7f0. Swaps object oldObj for newObj in attachment table t: in every
// entry record (0x168 bytes from 0x6acd4c) naming it, and in all records'
// link slots (+0x98.. / +0xa8.. / +0xb8.. / +0xc8.. / +0xd8..; the first group
// also flags its record +0x8c 0x40), queueing changed records (+0xec) on
// list 0x6acd5c while the entry flags have 0x8000; then points the entry at
// newObj (refreshing it with flag 0x800, clearing its +0x20 bit 5 unless
// +0xe8) and, for an effect entry, recreates the effect on it.
RT3_NOINLINE void __fastcall Attach_ReplaceObject(uint8_t* t, uint8_t* oldObj, uint8_t* newObj) {
    int32_t found;
    void* walk = (void*)&EntryWalk_Find;
    __asm {
        push ebx
        mov ebx, oldObj
        mov edx, t
        call walk
        pop ebx
        mov found, eax
    }
    if (!found) return;
    uint8_t* base = g_6acd4c;
    for (int32_t i = 0; i < F<int32_t>(g_6acd50, 8); ++i) {
        uint8_t* r = base + i * 0x168;
        if (F<uint8_t*>(r, 0) != oldObj) continue;
        F<uint8_t*>(r, 0) = newObj;
        for (int32_t j = 0; j < F<int32_t>(g_6acd50, 8); ++j) {
            uint8_t* p = g_6acd4c + 0xa8 + j * 0x168;
            for (int32_t k = 0; k < 4; ++k, p += 4) {
                if (F<uint8_t*>(p, -0x10) == oldObj) {
                    F<uint8_t*>(p, -0x10) = newObj;
                    F<uint32_t>(r, 0x8c) |= 0x40;
                }
                for (int32_t g = 0; g < 4; ++g)
                    if (F<uint8_t*>(p, g * 0x10) == oldObj) F<uint8_t*>(p, g * 0x10) = newObj;
            }
        }
        if (!(*hook::As<uint32_t*>(g_6acd48) & 0x8000)) continue;
        uint8_t** list = hook::As<uint8_t**>(g_6acd5c);
        uint8_t* node = r + 0xec;
        bool listed = false;
        for (uint8_t* n = list[0]; n; n = *(uint8_t**)n)
            if (n == node) listed = true;
        if (listed) continue;
        F<uint8_t*>(node, 4) = t;
        F<uint8_t*>(node, 0) = list[0];
        list[0] = node;
        ++((int32_t*)list)[2];
        if (!list[1]) list[1] = node;
        ((int32_t*)list)[3] = 0xec;
    }
    F<uint8_t*>(hook::As<uint8_t*>(g_6acd48), -4) = newObj;
    if (*hook::As<uint32_t*>(g_6acd48) & 0x800) ((void(__thiscall*)(uint8_t*))(*(void***)newObj)[4])(newObj);
    if (!F<int32_t>(g_6acd4c, 0xe8)) newObj[0x20] &= 0xdf;
    if (!g_6acd44) return;
    uint8_t* l = g_6acd50;
    int32_t* slot = nullptr;
    for (int32_t k = 0; k < F<int32_t>(l, 0xc); ++k)
        if (F<uint8_t*>(l, 0x10 + k * 0xc) == newObj) {
            slot = (int32_t*)(l + 0x18 + k * 0xc);
            break;
        }
    uint8_t* e = Effect_Create(newObj, *slot, 0.4f, 0, 0);
    *(uint8_t**)slot = e;
    Obj_SetElementFlag1d8(e, 0, -1);
}
RT3_REPLACE(0x45a7f0, Attach_ReplaceObject);

// 0x45a9c0. Steps the attachment animations of obj in walk table t at time
// `now`: a paused entry (0x8000) only ticks its emitters while the walk is
// live (0x6acd58); mode > 1 pauses (0x2000), resuming restarts every entry's
// clock (+0x80). Each due entry of obj (delay +0x94, period +0x7c) turns obj
// by its angle rates +4 (direction +0x84) and moves it by its speeds +0x40
// (direction +0x88), counting each axis down (+0x28.. / +0x64..) and
// finishing the side when done; a finished level takes back the summed
// vectors (bit 0), an attachment taking it ends the walk (bit 1).
RT3_NOINLINE void __fastcall Attach_Animate(uint8_t* t, uint8_t* obj, int32_t now, int32_t mode) {
    if (!EntryWalk_FindImpl(obj, t)) return;
    uint32_t* state = hook::As<uint32_t*>(g_6acd48);
    if (!state) {
        if (g_6acd58 == 1) return;
    } else {
        if (*state & 0x8000) {
            if (!g_6acd58) return;
            uint8_t* list = hook::As<uint8_t*>(g_6acd44);
            if (list) {
                for (int32_t i = 0; i < F<int32_t>(hook::As<uint8_t*>(g_6acd44), 8); ++i) {
                    uint8_t* em = F<uint8_t*>(hook::As<uint8_t*>(g_6acd44), 0xc + i * 4);
                    uint32_t f = F<uint32_t>(em, 0xac) & 0xfeffffff;
                    F<uint32_t>(em, 0xac) = f;
                    Emitter_Tick(F<uint8_t*>(hook::As<uint8_t*>(g_6acd44), 0xc + i * 4), 0, 0);
                    F<uint32_t>(F<uint8_t*>(hook::As<uint8_t*>(g_6acd44), 0xc + i * 4), 0xac) = f | 0x1000000;
                }
                state = hook::As<uint32_t*>(g_6acd48);
            }
        } else if (g_6acd58 == 1) {
            return;
        }
        uint32_t f = *state;
        if (mode > 1) {
            if (f & 0x2000) return;
            *state = f | 0x2000;
            return;
        }
        if (f & 0x2000) {
            *state = f & ~0x2000u;
            for (int32_t i = 0; i < F<int32_t>(g_6acd50, 8); ++i)
                F<int32_t>(g_6acd4c, 0x80 + i * 0x168) = now;
        }
    }
    float ang[3], pos[3];
    SceneObj_GetAngles(obj, 0, (uint32_t*)&ang[2], (uint32_t*)&ang[1], (uint32_t*)&ang[0]);
    SceneObj_GetPos(obj, 0, (uint32_t*)&pos[0], (uint32_t*)&pos[1], (uint32_t*)&pos[2]);
    uint8_t* e = g_6acd4c;
    for (int32_t i = 0; i < F<int32_t>(g_6acd50, 8); ++i, e += 0x168) {
        if (F<uint8_t*>(e, 0) != obj || (F<uint8_t>(e, 0x8c) & 2)) continue;
        int32_t dt = now - F<int32_t>(e, 0x80);
        if (F<int32_t>(e, 0x94) > 0) {
            F<int32_t>(e, 0x94) -= dt;
            if (F<int32_t>(e, 0x94) > 0) continue;
        }
        if (F<int32_t>(e, 0x7c) && dt < F<int32_t>(e, 0x7c)) continue;
        F<int32_t>(e, 0x80) = now;
        uint32_t result = 0;
        float out[3];
        for (int k = 0; k < 3; ++k) {
            float* rate = (float*)(e + 4) + k;
            out[k] = 0.0f;
            if (!(*(const float*)0x5c8598 != *rate)) continue;
            float v;
            if (F<int32_t>(e, 0x84) == 1) *(uint32_t*)&v = *(uint32_t*)rate;
            else v = -*rate;
            uint8_t* c = (*(const float*)0x5c8598 != rate[3]) ? (uint8_t*)rate : nullptr;
            float scaled = (float)((double)*(const float*)0x5cb4ac * v);
            uint32_t r = Block_CountSideImpl(c, e, 1u << (k + 7), &result, *(uint32_t*)&scaled, 0);
            out[k] = *(float*)&r;
        }
        ang[0] = (float)((double)ang[0] + out[0]);
        ang[1] = (float)((double)ang[1] + out[1]);
        ang[2] = (float)((double)ang[2] + out[2]);
        for (int k = 0; k < 3; ++k) {
            float* rate = (float*)(e + 0x40) + k;
            out[k] = 0.0f;
            if (!(*(const float*)0x5c8598 != *rate)) continue;
            uint32_t v;
            if (F<int32_t>(e, 0x88) == 1) {
                v = *(uint32_t*)rate;
            } else {
                float n = -*rate;
                v = *(uint32_t*)&n;
            }
            uint8_t* c = (*(const float*)0x5c8598 != rate[3]) ? (uint8_t*)rate : nullptr;
            uint32_t mask = 1u << (k + 7);
            if (c) {
                if (F<int32_t>(c, 0x24) < F<int32_t>(c, 0x30)) {
                    F<uint32_t>(e, 0x8c) &= ~mask;
                    ++F<int32_t>(c, 0x24);
                } else {
                    uint32_t fl = F<uint32_t>(e, 0x8c);
                    if (!(fl & mask)) {
                        F<uint32_t>(e, 0x8c) = fl | mask;
                        result |= (uint32_t)Block_FinishSideImpl(e, 1);
                    }
                    v = 0;
                }
            }
            *(uint32_t*)&out[k] = v;
        }
        pos[0] = (float)((double)pos[0] + out[0]);
        pos[1] = (float)((double)pos[1] + out[1]);
        pos[2] = (float)((double)pos[2] + out[2]);
        if (result & 1) {
            float sum[3];
            Attach_SumVectorsImpl(sum, 1, F<int32_t>(e, 0));
            pos[0] = (float)((double)pos[0] - sum[0]);
            pos[1] = (float)((double)pos[1] - sum[1]);
            pos[2] = (float)((double)pos[2] - sum[2]);
            Attach_SumVectorsImpl(sum, 0, F<int32_t>(e, 0));
            ang[0] = (float)((double)ang[0] - sum[0]);
            ang[1] = (float)((double)ang[1] - sum[1]);
            ang[2] = (float)((double)ang[2] - sum[2]);
        }
        if (result & 2) break;
    }
    SceneObj_SetAngles(obj, 0, *(uint32_t*)&ang[2], *(uint32_t*)&ang[1], *(uint32_t*)&ang[0]);
    Obj_MoveTo(obj, 0, pos[0], pos[1], pos[2]);
}
RT3_REPLACE(0x45a9c0, Attach_Animate);

// 0x45ade0. Steps the attachment animations (list 0x6acd5c, clock 0x6acd60)
// in 15-tick steps up to the world clock (+0x15), with the walk live
// (0x6acd58); a stopped clock just restarts at the world time.
void Attach_TickAll() {
    uint8_t* list = hook::As<uint8_t*>(g_6acd5c);
    if (!list) return;
    int32_t last = g_6acd60;
    if (!last) {
        g_6acd60 = F<int32_t>(g_world, 0x15);
        return;
    }
    int32_t now = F<int32_t>(g_world, 0x15);
    g_6acd58 = 1;
    while (now - last >= 0xf) {
        for (uint8_t** node = *(uint8_t***)list; node; node = *(uint8_t***)node) {
            uint8_t* o = (uint8_t*)node - F<int32_t>(list, 0xc);
            if (F<uint8_t*>(o, 0xf0)) {
                Attach_Animate(F<uint8_t*>(o, 0xf0), F<uint8_t*>(o, 0), last, 0);
                list = hook::As<uint8_t*>(g_6acd5c);
                last = g_6acd60;
            }
        }
        last += 0xf;
        g_6acd60 = last;
    }
    g_6acd58 = 0;
}
RT3_REPLACE(0x45ade0, Attach_TickAll);

// 0x45ae80. Loads "<name>_Anim.3dp" from the packs (0 if missing or empty);
// creates the empty animation list 0x6acd5c on first use.
RT3_NOINLINE void* __fastcall Anim_LoadFile(const char* name) {
    char buf[0x100];
    Format0(buf, 0x5cb4b0, name);
    if (!NameTable_ContainsName(Packs(), 0, buf, 0, nullptr)) return nullptr;
    int32_t size;
    void* p = LoadWhole(NameTable_Hash(Packs(), 0, buf), &size);
    if (!p) return nullptr;
    if (!hook::As<void*>(g_6acd5c)) {
        uint32_t* l = (uint32_t*)((void*(__cdecl*)(size_t))0x53b070)(0x10);
        if (l) {
            l[0] = 0;
            l[1] = 0;
            l[2] = 0;
        }
        l[0] = 0;
        l[1] = 0;
        l[2] = 0;
        g_6acd5c = l;
        l[3] = 0;
    }
    return p;
}
RT3_REPLACE(0x45ae80, Anim_LoadFile);

// 0x45af50. Releases an effect set: records it in the globals 0x6acd44..0x6acd54
// (0x6acd4c = its end), frees its effects (entries +0x18, 12 bytes, count +0xc;
// values above 15 are effect pointers) and drops the pending nodes that belong to
// it (+0xf0) from the list 0x6acd5c, deleting the list when it empties.
RT3_NOINLINE void __fastcall EffectSet_Release(uint8_t* s) {
    g_6acd44 = 0;
    g_6acd48 = 0;
    g_6acd54 = 0;
    g_6acd50 = s;
    hook::As<int32_t>(g_6acd4c) = 0;
    int32_t n = F<int32_t>(s, 0xc);
    g_6acd4c = s + (n > 0 ? n * 12 + 0x10 : 0x10);
    for (int32_t i = 0; i < F<int32_t>(g_6acd50, 0xc); ++i) {
        int32_t v = F<int32_t>(s, 0x18 + i * 12);
        g_6acd44 = v;
        if (v > 0xf) Effect_Free((uint32_t*)(uintptr_t)v);
    }
    uint32_t* list = g_6acd5c;
    if (!list) return;
    uint32_t* prev = nullptr;
    for (uint32_t* node = (uint32_t*)(uintptr_t)list[0]; node; node = (uint32_t*)(uintptr_t)node[0]) {
        uint8_t* owner = (uint8_t*)node - list[3];
        if (F<uint8_t*>(owner, 0xf0) != s) {
            prev = node;
            continue;
        }
        if (!prev) {
            if (uint32_t* head = (uint32_t*)(uintptr_t)list[0]) {
                list[0] = head[0];
                if ((uint32_t)(uintptr_t)head == list[1]) list[1] = head[0];
                --list[2];
            }
        } else {
            prev[0] = node[0];
            node[0] = 0;
            --list[2];
            if ((uint32_t)(uintptr_t)node == list[1]) list[1] = (uint32_t)(uintptr_t)prev;
            node = prev;
        }
    }
    if (!list[0]) {
        ExeDelete(list);
        g_6acd5c = nullptr;
    }
}
RT3_REPLACE(0x45af50, EffectSet_Release);

RT3_NOINLINE void __fastcall ModelObj_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5cb4c0;
    NamedObj_Destructor(o);
}
RT3_REPLACE(0x45b040, ModelObj_Destructor);

// 0x45b050. Clears the model state fields, also resetting the named object when asked.
RT3_NOINLINE int32_t __fastcall ModelObj_ResetState(uint8_t* o, int, int32_t full) {
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

// 0x45b0b0. Tints a vehicle and its attached object (+0x246), with children.
void __fastcall Vehicle_SetTint(uint8_t* v, int, uint32_t r, uint32_t g, uint32_t b) {
    SceneObj_SetTint(v, 0, r, g, b, 1);
    if (F<uint8_t*>(v, 0x246)) SceneObj_SetTint(F<uint8_t*>(v, 0x246), 0, r, g, b, 1);
}
RT3_REPLACE(0x45b0b0, Vehicle_SetTint);

// 0x45b0f0 (vtable). Sets o's colour from 0xRRGGBB (slot 0x58 gets the
// value shifted by 16, 8 and 0 bits).
void __fastcall Obj_SetColorRgb(uint8_t* o, int, uint32_t v) {
    ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t))(*(void***)o)[0x58 / 4])(o, 0, v >> 16, v >> 8, v);
}
RT3_REPLACE(0x45b0f0, Obj_SetColorRgb);

// 0x45b110 (vtable stub).
// Forwards to Obj_PlaceAtCell (0x455660).
void __fastcall Thunk45b110(uint8_t* p0, int p1, float p2, float p3, int32_t p4) { Obj_PlaceAtCell(p0, p1, p2, p3, p4); }
RT3_REPLACE(0x45b110, Thunk45b110);

// 0x45b120 (vtable). Attaches a new camera called `name` to scene object o.
PtrListNode* __fastcall SceneObj_AddCamera(uint8_t* o, int, const char* name) {
    uint8_t* m = ((uint8_t*(__cdecl*)(size_t))0x53b070)(0x35);
    return SceneObj_AddAttachment(o, 0, m ? Camera_Construct(m, 0, name, (uint32_t)(uintptr_t)o) : nullptr);
}
RT3_REPLACE(0x45b120, SceneObj_AddCamera);

// 0x45b160. Model object o: releases its parts - marks it dead (+0x23a bit
// 0), frees the effect set (+0x256), the effects (+0x25a / +0x25e) and its
// site (+0x24e, Sites_Free on 0x62b2fc), calls its vtable cleanup (slot 30),
// tears down the scene object and frees the buffers +0x23e / +0x242.
RT3_NOINLINE void __fastcall ModelObj_Release(uint8_t* o) {
    o[0x23a] |= 1;
    F<int32_t>(o, 0x24a) = 0;
    if (F<uint8_t*>(o, 0x256)) {
        EffectSet_Release(F<uint8_t*>(o, 0x256));
        Free(F<void*>(o, 0x256));
        F<void*>(o, 0x256) = nullptr;
    }
    if (F<uint32_t*>(o, 0x25a)) {
        Effect_Free(F<uint32_t*>(o, 0x25a));
        F<void*>(o, 0x25a) = nullptr;
    }
    if (F<uint32_t*>(o, 0x25e)) {
        Effect_Free(F<uint32_t*>(o, 0x25e));
        F<void*>(o, 0x25e) = nullptr;
    }
    if (F<uint8_t*>(o, 0x24e)) {
        Sites_Free(hook::As<uint8_t*>(g_buildingTypes), 0, F<uint8_t*>(o, 0x24e));
        F<void*>(o, 0x24e) = nullptr;
    }
    ((void(__fastcall*)(uint8_t*))(*(void***)o)[30])(o);
    SceneObj_Teardown(o);
    FreeAndNull2((void**)(o + 0x23e));
    FreeAndNull2((void**)(o + 0x242));
}
RT3_REPLACE(0x45b160, ModelObj_Release);

// 0x45b210. Gives an industry building its looping ambient sound (from its type's
// static entry: name, volume, range) on terrain `grid` at (x, y, z), stored in
// +0x24a. Returns 0 when the building has no industry or the type no sound.
RT3_NOINLINE int32_t __fastcall Building_AttachSound(uint8_t* b, int, int32_t, uint8_t* grid, float x, float y, float z) {
    typedef void*(__thiscall * IndFn)(void*);
    if (!((IndFn)(*(void***)b)[0x80 / 4])(b)) return 0;
    char base[0x1a8 - 0x18];
    int32_t vol, range;
    int32_t found = Obj_StaticEntry(((IndFn)(*(void***)b)[0x80 / 4])(b), 0, base, &vol, &range);
    if (!found) return 0;
    char name[0x190];
    FormatText(name, (const char*)0x5cb550, base, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    uint8_t* p = NewChannel();
    uint8_t* ch = p ? Channel_Setup(p, 0, name, grid, x, y, z, -1, 1, -1.0f) : nullptr;
    F<uint8_t*>(b, 0x24a) = ch;
    float lo = (float)((double)*(float*)&range * *(const double*)0x5cb548);
    Channel_SetRange(ch, 0, U(lo), U((float)((double)lo * *(const float*)0x5ca210)));
    float v = (float)((double)*(float*)&vol * *(const double*)0x5c8750);
    float cv = (float)(v < *(const double*)0x5c8770 ? (double)v : 1.0);
    Channel_SetVolume(F<uint8_t*>(b, 0x24a), 0, U(cv));
    Obj_AttachChannel(b, 0, F<uint8_t*>(b, 0x24a));
    return found;
}
RT3_REPLACE(0x45b210, Building_AttachSound);

// 0x45b370. Frees the object's two effects (+0x25a, +0x25e): always with `force`,
// otherwise only finished ones (more than 0x1000 elapsed and nothing pending).
void __fastcall Obj_FreeEffects(uint8_t* o, int, int32_t force) {
    for (uint32_t off : { 0x25au, 0x25eu }) {
        uint32_t* e = F<uint32_t*>(o, off);
        if (!e) continue;
        if (!force && (Obj_SumEntries6c((uint8_t*)e) <= 0x1000 || Obj_SumEntrySizes(F<uint8_t*>(o, off)))) continue;
        Effect_Free(F<uint32_t*>(o, off));
        F<uint32_t*>(o, off) = nullptr;
    }
}
RT3_REPLACE(0x45b370, Obj_FreeEffects);

// 0x45b400. Model object o: previewed (+0x236 cleared); a station model
// (+0x246) refreshes the preview model (vtable slot 29).
void __fastcall ModelObj_Select(uint8_t* o) {
    F<int32_t>(o, 0x236) = 0;
    SceneObj_ShowPreview(o);
    if (!F<void*>(o, 0x246)) return;
    uint8_t* p = g_previewModel;
    ((void(__fastcall*)(uint8_t*))(*(void***)p)[29])(p);
}
RT3_REPLACE(0x45b400, ModelObj_Select);

// 0x45b430. Block flag update (0x45a2e0) on +0x256 when +0x75 is set.
RT3_NOINLINE uint32_t __fastcall Obj_SetBlockFlags(uint8_t* o, int, uint32_t mask, int32_t mode) {
    uint8_t* blocks = F<uint8_t*>(o, 0x256);
    if (!blocks || !F<int32_t>(o, 0x75)) return (uint32_t)o;
    BlockIter_SetFlags(blocks, mask, mode);
    return 0;
}
RT3_REPLACE(0x45b430, Obj_SetBlockFlags);

// 0x45b460. Block test (0x45a380) on +0x256 when +0x75 is set; 0 otherwise.
RT3_NOINLINE int32_t __fastcall Obj_TestBlock(uint8_t* o, int, uint32_t mask, int32_t idx) {
    uint8_t* blocks = F<uint8_t*>(o, 0x256);
    if (!blocks || !F<int32_t>(o, 0x75)) return 0;
    return BlockIter_BeginTest(blocks, mask, idx);
}
RT3_REPLACE(0x45b460, Obj_TestBlock);

// 0x45b490. Marks (mode 1: unflagged, remembering idx in +0x262) or clears /
// marks slot idx in the object's blocks (+0x256), when +0x75 is set.
RT3_NOINLINE void __fastcall Obj_MarkBlockSlot(uint8_t* o, int, int32_t on, int32_t idx, int32_t mode) {
    uint8_t* blocks = F<uint8_t*>(o, 0x256);
    if (!blocks || !F<int32_t>(o, 0x75)) return;
    if (mode == 1) {
        BlockIter_MarkSlot(blocks, idx, 0);
        F<int32_t>(o, 0x262) = idx;
        return;
    }
    if (!on) BlockIter_ClearSlot(blocks, idx);
    else BlockIter_MarkSlot(blocks, idx, 1);
}
RT3_REPLACE(0x45b490, Obj_MarkBlockSlot);

// 0x45b4f0. Clears object o's attachment entries for walk slot (+0x256 table, +0x75
// needed): one slot, or for -1 the first of slots 0..3 that clears one. 0 when o has
// no table.
RT3_NOINLINE int32_t __fastcall Obj_ClearAttachSlot(uint8_t* o, int, int32_t slot) {
    if (!F<void*>(o, 0x256) || !F<int32_t>(o, 0x75)) return 0;
    if (slot != -1) return EntryWalk_BeginImpl(F<uint8_t*>(o, 0x256), slot);
    int32_t r = 0;
    for (int32_t i = 0; i < 4; ++i)
        if ((r = EntryWalk_BeginImpl(F<uint8_t*>(o, 0x256), i)) == 1) break;
    return r;
}
RT3_REPLACE(0x45b4f0, Obj_ClearAttachSlot);

// 0x45b540. Begins the block iteration of +0x256 when +0x75 is set; 1 otherwise.
RT3_NOINLINE uint32_t __fastcall Obj_BeginBlocks(uint8_t* o) {
    uint8_t* blocks = F<uint8_t*>(o, 0x256);
    if (!blocks || !F<int32_t>(o, 0x75)) return 1;
    return BlockIter_Begin(blocks);
}
RT3_REPLACE(0x45b540, Obj_BeginBlocks);

// 0x45b560 (vtable). Saves object o: the named base, tag 0x5dc1, the two
// names (+0x23e / +0x242), whether +0x246 is set, +0x24e, +0x252, tag 0x5dc2.
RT3_NOINLINE void __fastcall Obj_SaveNamedPair(uint8_t* o, int, Stream s) {
    NamedObj_Save(o, 0, s);
    StreamWriteU32(s, 0x5dc1);
    StreamWriteLenString(s, (const char* const*)(o + 0x23e));
    StreamWriteLenString(s, (const char* const*)(o + 0x242));
    const uint8_t has = F<int32_t>(o, 0x246) != 0;
    StreamWrite(s, &has, 1, 0);
    const uint32_t v = F<uint32_t>(o, 0x24e);
    StreamWrite(s, &v, 4, 0);
    StreamWrite(s, o + 0x252, 4, 0);
    StreamWriteU32(s, 0x5dc2);
}
RT3_REPLACE(0x45b560, Obj_SaveNamedPair);

// 0x45b5f0. Attaches sound `kind` to the object at ground level under its position,
// then turns looping off on its channel (+0x24a).
RT3_NOINLINE void __fastcall Obj_AttachGroundSound(uint8_t* o, int, int32_t kind) {
    float x, y, z;
    SceneObj_GetPos(o, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
    void* engine = *(void**)0x6d4024;
    float h = (float)Terrain_HeightAt((uint8_t*)Engine_Get1C(engine), 0, x, z);
    Building_AttachSound(o, 0, kind, (uint8_t*)Engine_Get1C(engine), x, h, z);
    if (uint8_t* ch = F<uint8_t*>(o, 0x24a)) Channel_SetLoop(ch, 0, 0);
}
RT3_REPLACE(0x45b5f0, Obj_AttachGroundSound);

// 0x45b670 (vtable stub).
int32_t __fastcall Return0_45b670(void*) { return 0; }
RT3_REPLACE(0x45b670, Return0_45b670);

// 0x45b680. Model object: same state fields as ModelObj_ResetState.
RT3_NOINLINE uint8_t* __fastcall ModelObj_Construct(uint8_t* o) {
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

// 0x45b6f0. Rebuilds building o's extra parts for its normal or `upgraded`
// look (name +0x23e, textures +0x242): drops its sprites, effects (+0x25a,
// +0x25e) and models, adds its lights (vtable +0x7c, %1_Light<n> /
// %1_Upgrade1Light<n>) and emitters, (re)loads its animation table (+0x256,
// reloaded when not upgraded) and puts each animation part model (%1_Anim<n>
// / %1_Upgrade1Anim<n>; not upgraded only without +0x246) into it.
RT3_NOINLINE void __fastcall Building_LoadParts(uint8_t* o, int, int32_t upgraded) {
    SpriteSet_Free(o);
    if (F<uint32_t*>(o, 0x25a)) {
        Effect_Free(F<uint32_t*>(o, 0x25a));
        F<uint32_t*>(o, 0x25a) = nullptr;
    }
    if (F<uint32_t*>(o, 0x25e)) {
        Effect_Free(F<uint32_t*>(o, 0x25e));
        F<uint32_t*>(o, 0x25e) = nullptr;
    }
    ModelSet_Clear(o);
    char num[0x10], path[0x130], part[0x130];
    auto has = [](const char* n) { return NameTable_ContainsName(g_pack, 0, n, 0, nullptr); };
    for (int32_t i = 1; i < 0xff; ++i) {
        const char* n = Num_Format(num, 1, (double)i, 10, 1, 0, 0);
        FormatText(path, upgraded ? (const char*)0x5cb598 : (const char*)0x5cb408, F<const char*>(o, 0x23e), n, nullptr, nullptr, nullptr, nullptr, nullptr,
                   nullptr);
        if (!has(path)) break;
        ((void(__thiscall*)(uint8_t*, const char*))(*(void***)o)[0x1f])(o, path);
    }
    FormatText(path, upgraded ? (const char*)0x5cb58c : (const char*)0x5cb588, F<const char*>(o, 0x23e), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
               nullptr);
    Effects_SpawnForModel(o, 0, path, nullptr, -1, 0x400, 1.0f);
    if (!upgraded && F<uint8_t*>(o, 0x256)) {
        EffectSet_Release(F<uint8_t*>(o, 0x256));
        ((void(__cdecl*)(void*))0x5a1145)(F<uint8_t*>(o, 0x256));
        F<uint8_t*>(o, 0x256) = nullptr;
    }
    if (!F<uint8_t*>(o, 0x256)) {
        F<uint8_t*>(o, 0x256) = (uint8_t*)Anim_LoadFile(F<const char*>(o, 0x23e));
        if (!F<uint8_t*>(o, 0x256)) return;
    }
    for (int32_t i = 1; i < 0xff; ++i) {
        const char* fmt;
        if (upgraded == 1)
            fmt = (const char*)0x5cb570;
        else {
            if (F<int32_t>(o, 0x246)) return;
            fmt = (const char*)0x5cb560;
        }
        const char* n = Num_Format(num, 1, (double)i, 10, 1, 0, 0);
        FormatText(part, fmt, F<const char*>(o, 0x23e), n, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        if (!has(part)) return;
        uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0x206);
        uint8_t* m = p ? SceneObj_Construct(p, 0, 0, part, F<const char*>(o, 0x242)) : nullptr;
        SceneObj_AddChild(o, 0, m);
        uint32_t key = Crc32String(part, 1);
        Attach_ReplaceObject(F<uint8_t*>(o, 0x256), (uint8_t*)(uintptr_t)key, m);
    }
}
RT3_REPLACE(0x45b6f0, Building_LoadParts);

// 0x45b9b0. Sets up building o (name, textures; flags a6 / a7 in +0x23a bits
// 1 / 2, a8 in +0x23b): resets it, and with `place` loads it as a world
// model at tile (x, z) with its parts, moves it onto the ground, attaches
// its sound and loops its channel (+0x24a, when +0x138 is 1).
RT3_NOINLINE void __fastcall Building_Init(uint8_t* o, int, const char* name, const char* tex, float x, float z, int32_t place, int32_t a6, int32_t a7, uint16_t a8) {
    ModelObj_ResetState(o, 0, place);
    F<uint32_t>(o, 0x25a) = 0;
    F<uint32_t>(o, 0x25e) = 0;
    F<uint32_t>(o, 0x24e) = 0;
    F<uint32_t>(o, 0x256) = 0;
    F<uint32_t>(o, 0x23e) = 0;
    F<uint32_t>(o, 0x242) = 0;
    String_Assign((char**)(o + 0x23e), name);
    String_Assign((char**)(o + 0x242), tex);
    o[0x23a] = (uint8_t)((((a7 & 1) << 1 | (a6 & 1)) << 1) | (o[0x23a] & 0xf9));
    F<uint16_t>(o, 0x23b) = a8;
    if (!place) return;
    WorldModel_Init(o, 0, F<const char*>(o, 0x23e), nullptr, F<const char*>(o, 0x242), *(uint32_t*)&x, *(uint32_t*)&z, 0);
    Building_LoadParts(o, 0, 0);
    float pz = (float)((double)z * Fl(0x5c8700));
    float px = (float)((double)x * Fl(0x5c8700));
    float py = (float)Terrain_HeightAt((uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), 0, px, pz);
    Obj_MoveTo(o, 0, px, py, pz);
    Building_AttachSound(o, 0, (int32_t)(uintptr_t)name, (uint8_t*)Engine_Get1C(hook::As<void*>(g_engine)), px, py, pz);
    if (F<uint8_t*>(o, 0x24a) && F<int32_t>(F<uint8_t*>(o, 0x24a), 0x138) == 1) Channel_SetLoop(F<uint8_t*>(o, 0x24a), 0, 0);
}
RT3_REPLACE(0x45b9b0, Building_Init);

// 0x45baf0 (scene object vtables 0x5c8c54 / 0x5cb4c4). Ambient sounds of
// object o: every 7 s of game ticks (at most 100 ms a tick, +0x266 / +0x26a)
// with the camera (0x62be68) closer than 24 units (92 each, ground distance
// then height), the type's chance per ms (0.6 of it beyond 6 units) against
// Rand(0, 20000) picks one of its "rnd_<name>_<nn>.wav" samples (its range
// +b: heard within 15 b, volume 0.75 of 110.4 b capped at 1). Then the two
// effects +0x25a / +0x25e are freed when done (over 0x1000, no entries).
void __fastcall SceneObj_AmbientSound(uint8_t* o) {
    int32_t dt = ClockMs() - F<int32_t>(o, 0x26a);
    F<int32_t>(o, 0x26a) = ClockMs();
    if (dt >= 0x64) dt = 0x64;
    F<int32_t>(o, 0x266) += dt;
    auto type = [&]() { return ((uint8_t*(__fastcall**)(uint8_t*))F<void**>(o, 0))[0x20](o); };
    if (F<int32_t>(o, 0x266) > 0x1b58) {
        uint8_t* cam = g_62be68;
        const float camX = F<float>(cam, 0x1c), camY = F<float>(cam, 0x24);
        const int32_t oy = (int32_t)F<float>(o, 0x1ea), ox = (int32_t)F<float>(o, 0x1e2), cy = (int32_t)camY, cx = (int32_t)camX;
        const float ground = (float)(uint16_t)DistanceInt(cx, cy, ox, oy);
        const float camZ = F<float>(g_62be68, 0x20);
        const int32_t oz = (int32_t)F<float>(o, 0x1e6), g = (int32_t)ground, cz = (int32_t)camZ;
        const uint16_t d = (uint16_t)DistanceInt(0, cz, g, oz);
        const float dist = (float)((double)d / D(0x5c8c48));
        if (dist < BitsF(0x41c00000)) {
            float rate;
            if (!type() || !Obj_TypeValue(type(), 0, (int32_t*)&rate)) {
                F<int32_t>(o, 0x266) = 0;
            } else {
                if (dist > BitsF(0x40c00000)) rate = (float)(rate * D(0x5c8880));
                const float ms = (float)dt;
                const float chance = ms * rate;
                const float roll = (float)Rand(0, 0x4e20);
                if (chance >= roll) {
                    char name[0x190], num[0x10], file[0x188];
                    int32_t a;
                    float range;
                    int32_t count;
                    F<int32_t>(o, 0x266) = 0;
                    const int32_t found = Obj_TypeRecord(type(), 0, name, &a, (int32_t*)&range, &count);
                    if (range * D(0x5c8798) > dist && found) {
                        const float far = (float)(range * D(0x5cb548));
                        uint32_t x, y, z;
                        SceneObj_GetPos(o, 0, &x, &y, &z);
                        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(num, (const char*)0x5c9aec, Rand(1, count));
                        FormatText(file, (const char*)0x5cb5b0, name, num, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                        float vol = (float)(far * D(0x5c8750));
                        vol = (double)vol < D(0x5c8770) ? vol : 1.0f;
                        float fx, fy, fz;
                        memcpy(&fx, &x, 4), memcpy(&fy, &y, 4), memcpy(&fz, &z, 4);
                        Sound_Play3D(g_sound, 0, file, fx, fy, fz, vol, 0, 0, 2, far);
                    }
                }
            }
        }
    }
    for (int32_t off = 0x25a; off <= 0x25e; off += 4) {
        uint8_t* e = F<uint8_t*>(o, off);
        if (e && Obj_SumEntries6c(e) > 0x1000 && !Obj_SumEntrySizes(e)) {
            Effect_Free((uint32_t*)F<uint8_t*>(o, off));
            F<uint8_t*>(o, off) = nullptr;
        }
    }
}
RT3_REPLACE(0x45baf0, SceneObj_AmbientSound);

// 0x45be50. Animates scene object o at time `now` (the world clock 0x6cec78
// +0x15 while it runs and o has attachments +0x75 on an animator +0x256):
// steady ones (+0x23a bit 4) just face up; the selection marker (0x6acd40,
// or with bit 2 itself) bobs above its target (bit 2: the ground under o by
// its height +0x23b; else the marked object 0x6acd3c's height or animation
// frame) with a bounce (+0x22e speed, +0x232 direction, flipping at the
// 0x5cb5cc / 0x5cb5d0 bounds) scaled by the elapsed time (+0x236, 1..8
// frames) and spins.
void __fastcall SelectMarker_Update(uint8_t* o, int, int32_t now) {
    if (F<uint32_t>(o, 0x256) && F<PtrList*>(o, 0x75) && F<int32_t>(g_world, 0x19)) {
        now = F<int32_t>(g_world, 0x15);
        PtrList_Rewind(F<PtrList*>(o, 0x75));
        for (uint8_t* a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)); a; a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)))
            Attach_Animate(F<uint8_t*>(o, 0x256), a, now, F<int32_t>(o, 0x21));
    }
    const uint8_t flags = o[0x23a];
    if (flags & 4) {
        SceneObj_SetAngles(o, 0, 0, 0x3fc90fdb, 0);
        return;
    }
    if (g_previewModel != o && !(flags & 2)) return;
    if (F<float>(o, 0x232) == Fl_207(0x5c8598)) {
        F<uint32_t>(o, 0x232) = 0xbf800000;
        F<uint32_t>(o, 0x22e) = 0x3f800000;
    }
    if (!F<int32_t>(o, 0x236)) {
        F<uint32_t>(o, 0x22e) = 0x3f800000;
        F<uint32_t>(o, 0x232) = 0xbf800000;
        F<int32_t>(o, 0x236) = now;
        return;
    }
    const float bounce = (float)(((double)Fl_207(0x5c85b4) - F<float>(o, 0x22e)) * Fl_207(0x5cb5d0) + (double)F<float>(o, 0x22e) * Fl_207(0x5cb5cc));
    float x, y, z;
    if (flags & 2) {
        SceneObj_GetPos(o, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
        const float ground = (float)Terrain_HeightAtTile(g_map, 0, (float)((double)x / Fl_207(0x5c8700)), (float)((double)z / Fl_207(0x5c8700)));
        const float lift = (float)((double)bounce * Fl_207(0x5c9af4));
        const float height = (float)F<uint16_t>(o, 0x23b);
        Obj_MoveTo(o, 0, x, (float)((double)height + lift + ground), z);
    } else {
        uint8_t* target = g_6acd3c;
        SceneObj_GetPos(target, 0, (uint32_t*)&x, (uint32_t*)&y, (uint32_t*)&z);
        target = g_6acd3c;
        float v;
        if (!F<uint32_t>(target, 0x20a))
            v = (float)(((double(__thiscall*)(uint8_t*))(*(void***)target)[0x68 / 4])(target) + (double)bounce * Fl_207(0x5c9af4));
        else
            v = (float)((double)bounce * Fl_207(0x5c9af4) + *(const float*)(uintptr_t)(Obj_AnimFrame(target) + 0x18));
        Obj_MoveTo(g_previewModel, 0, x, (float)((double)v + y), z);
    }
    float frames = (float)(now - F<int32_t>(o, 0x236));
    frames = (float)((double)frames / Fl_207(0x5c8554));
    if (frames < Fl_207(0x5c85b4))
        frames = 1.0f;
    else if (frames > Fl_207(0x5c8898))
        frames = 8.0f;
    const float speed = (float)(((double)Fl_207(0x5cb5c8) - ((double)Fl_207(0x5cb5d0) - F<float>(o, 0x22e))) * F<float>(o, 0x232) * Fl_207(0x5cb5c4) * frames +
                                F<float>(o, 0x22e));
    F<float>(o, 0x22e) = speed;
    if (speed <= Fl_207(0x5cb5cc)) {
        F<uint32_t>(o, 0x22e) = 0x3ecccccd;
        F<float>(o, 0x232) = (float)((double)F<float>(o, 0x232) * Fl_207(0x5c9b08));
    } else if (speed >= Fl_207(0x5cb5d0)) {
        F<uint32_t>(o, 0x22e) = 0x3f4ccccd;
        F<float>(o, 0x232) = (float)((double)F<float>(o, 0x232) * Fl_207(0x5c9b08));
    }
    SceneObj_Rotate(o, 0, 0.0f, (float)((double)frames * Fl_207(0x5cb5c0)), 0.0f);
    F<int32_t>(o, 0x236) = now;
}
RT3_REPLACE(0x45be50, SelectMarker_Update);

 // namespace

// 0x45c150. Loads building o from stream s: its world model, name and
// textures (its sound attached by name), its parts, then whether it is
// upgraded (vtable +0x74) and two dwords (+0x24e, cleared, then +0x252).
// Returns the bytes read.
RT3_NOINLINE int32_t __fastcall Building_Load(uint8_t* o, int, int32_t s) {
    F<uint32_t>(o, 0x24e) = 0;
    F<uint32_t>(o, 0x23e) = 0;
    F<uint32_t>(o, 0x242) = 0;
    BuildingClear(o);
    NamedObj_Reset(o);
    int32_t total = WorldModel_Load(o, 0, s);
    total += StreamSkip4(s, 0x5dc1, 0, 0);
    char* name = nullptr;
    char* tex = nullptr;
    total += StreamReadString(s, &name);
    total += StreamReadString(s, &tex);
    BuildingClear(o);
    F<uint32_t>(o, 0x25a) = 0;
    F<uint32_t>(o, 0x25e) = 0;
    F<uint32_t>(o, 0x24e) = 0;
    F<uint32_t>(o, 0x256) = 0;
    F<uint32_t>(o, 0x23e) = 0;
    F<uint32_t>(o, 0x242) = 0;
    String_Assign((char**)(o + 0x23e), name);
    String_Assign((char**)(o + 0x242), tex);
    o[0x23a] &= 0xf9;
    F<uint16_t>(o, 0x23b) = 0;
    Obj_AttachGroundSound(o, 0, (int32_t)(uintptr_t)name);
    FreeAndNull2((void**)&name);
    FreeAndNull2((void**)&tex);
    Building_LoadParts(o, 0, 0);
    uint8_t upgraded;
    total += StreamRead(s, &upgraded, 1, 0, 1);
    if (upgraded) {
        F<uint32_t>(o, 0x246) = 0;
        ((void(__thiscall*)(uint8_t*))(*(void***)o)[0x1d])(o);
    }
    total += StreamRead(s, o + 0x24e, 4, 0, 1);
    F<uint32_t>(o, 0x24e) = 0;
    total += StreamRead(s, o + 0x252, 4, 0, 1);
    return StreamSkip4(s, 0x5dc2, 0, 0) + total;
}
RT3_REPLACE(0x45c150, Building_Load);

// 0x45c310. Gives building o its upgraded model (%1_Upgrade1, +0x246) and
// parts, then refreshes it (vtable +0x5c) when it is the current one
// (0x6acd3c).
RT3_NOINLINE void __fastcall Building_AddUpgradeModel(uint8_t* o) {
    char path[0x12c];
    FormatText(path, (const char*)0x5cb5d4, F<const char*>(o, 0x23e), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0x23a);
    uint8_t* w = p ? WorldModel_Construct(p, 0, F<const char*>(o, 0x23e), path, F<const char*>(o, 0x242), 0xbf800000, 0xbf800000, 1) : nullptr;
    F<uint8_t*>(o, 0x246) = w;
    Obj_MoveTo(w, 0, 0.0f, 0.0f, 0.0f);
    Building_LoadParts(o, 0, 1);
    SceneObj_AddChild(o, 0, F<uint8_t*>(o, 0x246));
    if (IsCurrent6acd3c(o)) ((void(__thiscall*)(uint8_t*))(*(void***)o)[0x17])(o);
}
RT3_REPLACE(0x45c310, Building_AddUpgradeModel);

// 0x45c3c0. Building object o: drops its station model (+0x246): tears it
// down, deletes the other parts in its list (+0x75), reloads the parts
// unless dead (+0x23a bit 0), deletes the model and clears the list.
RT3_NOINLINE void __fastcall BuildingObj_DropModel(uint8_t* o) {
    if (!F<uint8_t*>(o, 0x246)) return;
    SceneObj_Teardown(F<uint8_t*>(o, 0x246));
    PtrList_Rewind(F<PtrList*>(o, 0x75));
    for (uint8_t* v = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)); v; v = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75))) {
        if (v == F<uint8_t*>(o, 0x246)) continue;
        SceneObj_Destructor(v);
        ((void(__cdecl*)(void*))0x53b080)(v);
    }
    if (!(o[0x23a] & 1)) Building_LoadParts(o, 0, 0);
    if (uint8_t* m = F<uint8_t*>(o, 0x246)) {
        NamedObj_Destructor(m);
        ((void(__cdecl*)(void*))0x53b080)(m);
    }
    F<void*>(o, 0x246) = nullptr;
    PtrList_Clear(F<PtrList*>(o, 0x75));
}
RT3_REPLACE(0x45c3c0, BuildingObj_DropModel);

RT3_NOINLINE void __fastcall PartModel_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5cb5e8;
    NamedObj_Destructor(o);
}
RT3_REPLACE(0x45c460, PartModel_Destructor);

// 0x45c470 / 0x45c480 / 0x45c4d0. Angles of the model part list +0x316 (0xc0-byte parts, object +0xc).
RT3_NOINLINE void __fastcall Model_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c) {
    SceneObj_SetAngles(F<uint8_t*>(F<uint8_t*>(o, 0x316), 0xc), 0, a, b, c);
}
RT3_REPLACE(0x45c470, Model_SetAngles);

void __fastcall Model_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c) {
    uint32_t x, y, z;
    SceneObj_GetAngles(F<uint8_t*>(F<uint8_t*>(o, 0x316), 0xc), 0, &x, &y, &z);
    if (a) *a = x;
    if (b) *b = y;
    if (c) *c = z;
}
RT3_REPLACE(0x45c480, Model_GetAngles);

RT3_NOINLINE void __fastcall Model_SetPartAngle2(uint8_t* o, int, int32_t part, uint32_t v) {
    if (!part) return;
    SceneObj_SetAngles(F<uint8_t*>(F<uint8_t*>(o, 0x316), part * 0xc0 + 0xc), 0, 0, v, 0);
}
RT3_REPLACE(0x45c4d0, Model_SetPartAngle2);

// 0x45c500. Turns pivot (a, b) of o (+0x316 +0xa0, 48 per row) to `angle`: rotates
// its offset (+0x1c-+0x20, +0x28-+0x2c) to place the parts +0x38 / +0x34, then
// swings arm +0x30 around +0x34 and aims +0x34 at it (atan2 less +4).
// Quirk: the arm uses sin/cos of +0 alone, not +0 + angle, for its axes.
RT3_NOINLINE void __fastcall Obj_TurnPivot(uint8_t* o, int, float angle, int32_t a, int32_t b) {
    uint8_t* p = F<uint8_t*>(F<uint8_t*>(o, 0x316), 0xa0 + (a * 48 + b) * 4);
    auto sin_ = (double(__cdecl*)(double))0x5a1494;
    auto cos_ = (double(__cdecl*)(double))0x5a13e4;
    float u = (float)((double)F<float>(p, 0x1c) - F<float>(p, 0x20));
    float v = (float)((double)F<float>(p, 0x28) - F<float>(p, 0x2c));
    double t = cos_(angle) * v;
    float rx = (float)(sin_(angle) * u + t);
    t = cos_(angle) * u;
    float rz = (float)(t - sin_(angle) * v);
    SceneObj_SetAngles(F<uint8_t*>(p, 0x3c), 0, Bits(angle), 0, 0);
    if (F<uint8_t*>(p, 0x38))
        SceneObj_SetPos(F<uint8_t*>(p, 0x38), 0, F<uint32_t>(p, 0x14), Bits((float)((double)rx + F<float>(p, 0x20))),
                        Bits((float)((double)rz + F<float>(p, 0x2c))));
    if (!F<uint8_t*>(p, 0x34)) return;
    SceneObj_SetPos(F<uint8_t*>(p, 0x34), 0, F<uint32_t>(p, 0x10), Bits((float)((double)rx + F<float>(p, 0x20))),
                    Bits((float)((double)rz + F<float>(p, 0x2c))));
    float base = F<float>(p, 0);
    float arm = (float)(cos_((double)base + angle) * u);
    float az = (float)(sin_(base) * arm + F<float>(p, 0x24));
    float ax = (float)(cos_(base) * arm + F<float>(p, 0x18));
    SceneObj_SetPos(F<uint8_t*>(p, 0x30), 0, F<uint32_t>(p, 0xc), Bits(ax), Bits(az));
    uint32_t x1, y1, z1, x2, y2, z2;
    SceneObj_GetPos2(F<uint8_t*>(p, 0x30), 0, &x1, &y1, &z1);
    SceneObj_GetPos2(F<uint8_t*>(p, 0x34), 0, &x2, &y2, &z2);
    double r = ((double(__cdecl*)(double, double))0x5a29d0)((double)FromBits(z1) - FromBits(z2),
                                                           (double)FromBits(y1) - FromBits(y2));
    SceneObj_SetAngles(F<uint8_t*>(p, 0x34), 0, Bits((float)(r - F<float>(p, 4))), 0, 0);
}
RT3_REPLACE(0x45c500, Obj_TurnPivot);

RT3_NOINLINE double __fastcall Obj_Float242(void* o, int, int32_t i) { return F<float>(o, 0x242 + i * 4); }
RT3_REPLACE(0x45c6e0, Obj_Float242);

// 0x45c6f0 (via thunk 0x41ab70). Saves car c to stream s (the inverse of
// Car_Load 0x45e530): its model and texture set names, the emitter hook per
// emitter slot, then 0x61b2, the count of parts and attachments and each
// one's state (vtable +0x4c), framed by 0x61a9 / 0x61aa ... 0x61ab.
RT3_NOINLINE void __fastcall Car_Save(uint8_t* c, int, Stream s) {
    StreamWriteU32(s, 0x61a9);
    StreamWriteU32(s, 0x61aa);
    StreamWriteLenString(s, (const char* const*)(c + 0x32e));
    StreamWriteLenString(s, (const char* const*)(c + 0x332));
    for (uint32_t k = 0x10; k < 0x30; k += 4)
        if (int32_t e = F<int32_t>(F<uint8_t*>(c, 0x316), k)) Nop5548c0(hook::As<void*>(g_models), 0, e, s);
    StreamWriteU32(s, 0x61b2);
    int32_t count = 0;
    for (int32_t pass = 0; pass < 2; ++pass) {
        for (int32_t m = 0; m < F<int32_t>(c, 0x31a); ++m) {
            uint8_t* o = F<uint8_t*>(F<uint8_t*>(c, 0x316) + m * 0xc0, 0xc);
            if (!pass)
                ++count;
            else
                ((void(__thiscall*)(uint8_t*, Stream))(*(void***)o)[0x13])(o, s);
            if (!F<PtrList*>(o, 0x75)) continue;
            PtrList_Rewind(F<PtrList*>(o, 0x75));
            for (uint8_t* a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)); a; a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75))) {
                if (!pass)
                    ++count;
                else
                    ((void(__thiscall*)(uint8_t*, Stream))(*(void***)a)[0x13])(a, s);
            }
        }
        if (!pass) StreamWrite(s, &count, 4, 0);
    }
    StreamWriteU32(s, 0x61ab);
}
RT3_REPLACE(0x45c6f0, Car_Save);

// 0x45c840. SceneObj_SetAttachmentsFlag on every model part (+0x316, 0xc0 bytes, count +0x31a).
void __fastcall Model_SetAttachmentsFlag(uint8_t* o, int, int32_t on) {
    for (int32_t i = 0; i < F<int32_t>(o, 0x31a); ++i)
        SceneObj_SetAttachmentsFlag(F<uint8_t*>(F<uint8_t*>(o, 0x316), i * 0xc0 + 0xc), 0, on);
}
RT3_REPLACE(0x45c840, Model_SetAttachmentsFlag);

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

// 0x45c8d0. Sets the opacity of every mesh (+0x44.., count +8) of every part and
// of the attachments.
RT3_NOINLINE void __fastcall Model_SetOpacity(uint8_t* m, int, float a) {
    for (int32_t i = 0; i < F<int32_t>(m, 0x31a); ++i) {
        uint8_t* part = F<uint8_t*>(m, 0x316) + i * 0xc0;
        for (int32_t j = 0; j < F<int32_t>(part, 8); ++j) Obj_SetUnit25(F<uint8_t*>(part, 0x44 + j * 4), 0, a);
    }
    if (!F<PtrList*>(m, 0x7d)) return;
    PtrList_Rewind(F<PtrList*>(m, 0x7d));
    for (uint8_t* e = (uint8_t*)PtrList_Next(F<PtrList*>(m, 0x7d)); e; e = (uint8_t*)PtrList_Next(F<PtrList*>(m, 0x7d)))
        Obj_SetUnit25(e, 0, a);
}
RT3_REPLACE(0x45c8d0, Model_SetOpacity);

// 0x45c990. Rolls a vehicle's wheels by `dist`: per wheel group (+0x316, +0x31a of
// 0xc0 bytes) each wheel (+0x98 objects, +0x9c circumference / pi) spins by
// -2 pi dist / (c pi), and each pivot (+0xa0) turns on from its current angle (a
// bad circumference becomes 0.1 after formatting an unused warning, text 0x1b1). Then
// places the front / back light emitters (+0x38 / +0x3c, from +0x242 / +0x246 along
// the heading +0xee) and ticks the vehicle's emitters (+0x10.., null-ended).
RT3_NOINLINE void __fastcall Vehicle_RollWheels(uint8_t* v, int, float dist) {
    const double pi = Fl_205(0x5c8760), tau = Fl_205(0x5c8768);
    for (int32_t i = 0; i < F<int32_t>(v, 0x31a); ++i) {
        uint8_t* g = F<uint8_t*>(v, 0x316) + i * 0xc0;
        for (int32_t k = 0; k < F<int32_t>(g, 0); ++k) {
            float c = F<float*>(g, 0x9c)[k];
            float a = (float)(-(dist / ((double)c * pi)) * tau);
            SceneObj_Rotate(F<uint8_t**>(g, 0x98)[k], 0, a, 0.0f, 0.0f);
        }
        for (int32_t k = 0; k < F<int32_t>(g, 4); ++k) {
            uint8_t* w = F<uint8_t*>(g, 0xa0 + k * 4);
            float base = SceneObj_Angle1(F<uint8_t*>(w, 0x3c));
            float c = F<float>(w, 0x40);
            if (c <= Fl_205(0x5c8598)) {
                char msg[0x200];
                c = BitsF(0x3dcccccd);
                FormatText(msg, GetString(0x1b1), F<const char*>(v, 0x32e), 0, 0, 0, 0, 0, 0, 0);
            }
            float a = (float)(-(dist / ((double)c * pi)) * tau);
            Obj_TurnPivot(v, 0, (float)((double)a + base), i, k);
        }
    }
    uint8_t* parts = F<uint8_t*>(v, 0x316);
    if (F<int32_t>(parts, 0x34)) {
        auto f = [&](uint32_t o) { return (double)F<float>(v, o); };
        float back = (float)-F<float>(v, 0x246);
        float p1[3], p2[3];
        p1[2] = (float)((double)(float)(back * f(0xf6)) + f(0x1ea));
        p1[1] = (float)((double)(float)(back * f(0xf2)) + f(0x1e6));
        p1[0] = (float)((double)(float)(back * f(0xee)) + f(0x1e2));
        double front = f(0x242);
        p2[2] = (float)((double)(float)(f(0xf6) * front) + f(0x1ea));
        p2[1] = (float)((double)(float)(f(0xf2) * front) + f(0x1e6));
        p2[0] = (float)((double)(float)(f(0xee) * front) + f(0x1e2));
        Store3Globals((int32_t*)p1);
        Obj_ApplyStored3(F<void*>(F<uint8_t*>(v, 0x316), 0x38), -1);
        Store3Globals((int32_t*)p2);
        Obj_ApplyStored3(F<void*>(F<uint8_t*>(v, 0x316), 0x3c), -1);
    }
    for (uint8_t** e = (uint8_t**)(F<uint8_t*>(v, 0x316) + 0x10); *e; ++e) Emitter_Tick(*e, 0, 0);
}
RT3_REPLACE(0x45c990, Vehicle_RollWheels);

 // namespace

// 0x45cc90. Starts (on 1) or stops car c's crash effects: smoke kinds 1-3
// (+0x34 / +0x38 / +0x3c; the last two flagged 0x100000 and placed at the
// car's front / back along its axis +0xee.., -+0x246 / +0x242 from +0x1e2..),
// marking its emitters.
RT3_NOINLINE void __fastcall Car_SetCrashed(uint8_t* c, int, int32_t on) {
    if (on == 1) {
        if (F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x34)) return;
        uint32_t flags;
        float size = CarEffectSize(c, &flags);
        F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x34) = Effect_Create(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 1, size, flags, 0);
        flags |= 0x100000;
        F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x38) = Effect_Create(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 2, size, flags, 0);
        F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x3c) = Effect_Create(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 3, size, flags, 0);
        float ax = F<float>(c, 0xee), ay = F<float>(c, 0xf2), az = F<float>(c, 0xf6);
        float px = F<float>(c, 0x1e2), py = F<float>(c, 0x1e6), pz = F<float>(c, 0x1ea);
        float back = -F<float>(c, 0x246), front = F<float>(c, 0x242);
        float a[3], b[3];
        a[2] = (float)((double)(float)((double)back * az) + pz);
        a[1] = (float)((double)(float)((double)back * ay) + py);
        a[0] = (float)((double)(float)((double)back * ax) + px);
        b[2] = (float)((double)(float)((double)az * front) + pz);
        b[1] = (float)((double)(float)((double)ay * front) + py);
        b[0] = (float)((double)(float)((double)ax * front) + px);
        Store3Globals((int32_t*)a);
        Obj_ApplyStored3(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x38), -1);
        Store3Globals((int32_t*)b);
        Obj_ApplyStored3(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x3c), -1);
        CarMarkEmitters(c, 1);
        return;
    }
    if (!F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x34)) return;
    Effect_Free(F<uint32_t*>(F<uint8_t*>(c, 0x316), 0x3c));
    F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x3c) = nullptr;
    Effect_Free(F<uint32_t*>(F<uint8_t*>(c, 0x316), 0x38));
    F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x38) = nullptr;
    Effect_Free(F<uint32_t*>(F<uint8_t*>(c, 0x316), 0x34));
    F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x34) = nullptr;
    CarMarkEmitters(c, 0);
}
RT3_REPLACE(0x45cc90, Car_SetCrashed);

// 0x45cf40. Starts (on 1) or stops car c's broken-down steam (+0x40, effect
// kind 5), marking its emitters.
RT3_NOINLINE void __fastcall Car_SetBrokenDown(uint8_t* c, int, int32_t on) {
    if (on == 1) {
        if (F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x40)) return;
        uint32_t flags;
        float size = CarEffectSize(c, &flags);
        F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x40) = Effect_Create(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 5, size, flags, 0);
        CarMarkEmitters(c, 1);
        return;
    }
    if (!F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x40)) return;
    Effect_Free(F<uint32_t*>(F<uint8_t*>(c, 0x316), 0x40));
    F<uint8_t*>(F<uint8_t*>(c, 0x316), 0x40) = nullptr;
    CarMarkEmitters(c, 0);
}
RT3_REPLACE(0x45cf40, Car_SetBrokenDown);

// 0x45d030. Shows part `part` with sprite frame `frame` (1-based cell of an
// 8x8 atlas; 0 = whole texture), or hides it for frame -1.
void __fastcall Model_SetPartFrame(uint8_t* o, int, int32_t part, int32_t frame) {
    if (part < 0 || part >= 0x20) return;
    uint8_t* p = PartObj(o, part);
    if (!p) return;
    if (frame == -1) {
        Hide(p);
        return;
    }
    Show(p);
    if (!PartFlag(o, part)) return;
    if (!frame) {
        SceneObj_SetUvTransform(PartObj(o, part), 0, 0.0f, 0.0f, 1.0f, 1.0f);
        return;
    }
    int32_t f = frame - 1;
    const float cells = *(const float*)0x5c8898;
    float u = (float)((double)(float)(f % 8) / cells);
    float v = (float)((double)(float)(f / 8) / cells);
    SceneObj_SetUvTransform(PartObj(o, part), 0, u, v, 0.125f, 0.125f);
    F<int32_t>(PartObj(o, part), 0x3e) = 2;
}
RT3_REPLACE(0x45d030, Model_SetPartFrame);

// 0x45d130. Gives a vehicle the engine sound of engine type `typeName` (its sample
// table entry 0x5f3a60[+0x83]) on terrain `grid` at (x, y, z), into +0x336.
RT3_NOINLINE int32_t __fastcall Vehicle_AttachEngineSound(uint8_t* o, int, const char* typeName, uint8_t* grid, float x, float y, float z) {
    uint32_t id = Table_FindByAltName(g_6ada84, 0, typeName);
    if (!id) return 0;
    uint8_t* e = (uint8_t*)Collection_Get(g_6ada84, 0, id);
    uint8_t* p = NewChannel();
    uint8_t* ch = p ? Channel_Setup(p, 0, ((const char**)0x5f3a60)[F<int32_t>(e, 0x83)], grid, x, y, z, -1, 1, -1.0f)
                    : nullptr;
    F<uint8_t*>(o, 0x336) = ch;
    Channel_SetRange(ch, 0, 0x42b80000, 0x45b80000);
    Channel_SetVolume(F<uint8_t*>(o, 0x336), 0, 0x3f800000);
    Obj_AttachChannel(o, 0, F<uint8_t*>(o, 0x336));
    return 1;
}
RT3_REPLACE(0x45d130, Vehicle_AttachEngineSound);

// 0x45d1e0. Picks the engine sound of the vehicle (+0x336): 0 = its type's sound
// (table 0x5f3a60 by +0x83), 1 / 2 = the fixed sounds 0x5cb680 / 0x5cb664.
RT3_NOINLINE void __fastcall Vehicle_SelectSound(uint8_t* v, int, int32_t kind) {
    Collection* types = g_6ada84;
    uint32_t id = Table_FindByAltName(types, 0, F<const char*>(v, 0x32e));
    if (!id) return;
    uint8_t* t = (uint8_t*)Collection_Get(g_6ada84, 0, id);
    uint8_t* ch = F<uint8_t*>(v, 0x336);
    if (!ch) return;
    if (kind == 0) Channel_SetName(ch, 0, ((const char* const*)0x5f3a60)[F<int32_t>(t, 0x83)]);
    else if (kind == 1) Channel_SetName(ch, 0, (const char*)0x5cb680);
    else if (kind == 2) Channel_SetName(ch, 0, (const char*)0x5cb664);
}
RT3_REPLACE(0x45d1e0, Vehicle_SelectSound);

// 0x45d260. Sets the engine sound (+0x336) pitch (sample base rate x scale, at
// least 8000 Hz) and volume, stopping a looping sample; returns the rate (0 if none).
int32_t __fastcall Vehicle_SetEngineSound(uint8_t* v, int, float scale, uint32_t volume) {
    uint8_t* ch = F<uint8_t*>(v, 0x336);
    if (!ch) return 0;
    uint8_t* s = (uint8_t*)Channel_Check(ch);
    if (!s) return 0;
    float base = (float)F<int32_t>(F<uint8_t*>(s, 4), 0x20);
    int32_t rate = (int32_t)((double)base * scale);
    if (rate < 0x1f40) rate = 0x1f40;
    Channel_SetRate(F<uint8_t*>(v, 0x336), 0, rate);
    Channel_SetVolume(F<uint8_t*>(v, 0x336), 0, volume);
    if (F<int32_t>(F<uint8_t*>(v, 0x336), 0x138) == 1) Channel_SetLoop(F<uint8_t*>(v, 0x336), 0, 0);
    return rate;
}
RT3_REPLACE(0x45d260, Vehicle_SetEngineSound);

// 0x45d2e0. Updates the vehicle's horn sound (+0x346) at its position, or starts a
// requested one: crash (+0x33e, scale 2), breakdown (+0x342, 1.5) or its engine
// whistle (+0x33a while its channel +0x336 plays; whistle 0x5f3a84 by type +0x87);
// distances 92 / 5888 x scale. Clears the requests.
void __fastcall Vehicle_UpdateHorn(uint8_t* v, int, int32_t) {
    if (F<uint8_t*>(v, 0x336)) {
        uint32_t x, y, z;
        SceneObj_GetPos(v, 0, &x, &y, &z);
        if (uint32_t key = F<uint32_t>(v, 0x346)) {
            if (uint8_t* p = (uint8_t*)Sound_LookupSample(Snd(), 0, key, 1)) {
                F<uint32_t>(p, 0x10) = x;
                F<uint32_t>(p, 0x14) = y;
                F<uint32_t>(p, 0x18) = z;
                F<uint32_t>(p, 0x44) |= 2;
            } else {
                F<uint32_t>(v, 0x346) = 0;
            }
        } else if (!F<int32_t>(F<uint8_t*>(v, 0x336), 0x138)) {
            bool go;
            if (Channel_Playing(F<uint8_t*>(v, 0x336)) == 1 && F<int32_t>(v, 0x33a)) go = true;
            else go = F<int32_t>(v, 0x33e) || F<int32_t>(v, 0x342);
            if (go) {
                char name[0x100];
                float scale;
                if (F<int32_t>(v, 0x33e)) {
                    memcpy(name, (const void*)0x5cb6b4, 16);
                    scale = 2.0f;
                } else if (F<int32_t>(v, 0x342)) {
                    memcpy(name, (const void*)0x5cb6a0, 20);
                    scale = 1.5f;
                } else {
                    scale = 1.0f;
                    uint8_t* type = (uint8_t*)Obj_CityRef(Obj_Train390(v));
                    const char* w = ((const char* const*)0x5f3a84)[F<int32_t>(type, 0x87)];
                    char* d = name;
                    while ((*d++ = *w++) != 0) {}
                }
                uint32_t key = Sound_Play3D(Snd(), 0, name, FromBits(x), FromBits(y), FromBits(z), 1.0f, 0, 1, 2, 0.0f);
                F<uint32_t>(v, 0x346) = key;
                if (key) {
                    uint8_t* p = (uint8_t*)Sound_LookupSample(Snd(), 0, key, 1);
                    F<float>(p, 0x30) = (float)((double)scale * *(const double*)0x5c8c48);
                    F<int32_t>(p, 0xc) = 0;
                    F<float>(p, 0x34) = (float)((double)scale * *(const double*)0x5cb698);
                    F<uint32_t>(p, 0x44) |= 0x18;
                }
            }
        }
    }
    F<int32_t>(v, 0x33a) = 0;
    F<int32_t>(v, 0x33e) = 0;
    F<int32_t>(v, 0x342) = 0;
}
RT3_REPLACE(0x45d2e0, Vehicle_UpdateHorn);

 // namespace

// 0x45d4b0. Loads rail car `name` (texture set `tex`, else the name; with
// `reset` from scratch, as a world model) at (x, z) x 92 on the ground: its
// length / track points, then each of its trucks (%1_Truck<n>, 0xc0-byte
// records +0x316, count +0x31a; truck 0 is the body with the service point,
// cargo icons and cargo models in the 32 slots at +0x276), each with its
// bogies (+0x98 models, +0x9c lengths), drive wheels with their coupling
// bars, connecting rods and pistons (0x44-byte records +0xa0.., angles
// first), up to 8 emitters (+0x10) and lights (cameras +0x44..), then moves
// it into place and adds its engine sound.
RT3_NOINLINE void __fastcall Car_LoadModel(uint8_t* c, int, const char* name, const char* tex, float x, float z, int32_t reset) {
    if (reset) {
        F<uint32_t>(c, 0x316) = 0;
        F<uint32_t>(c, 0x31a) = 0;
        F<uint32_t>(c, 0x32e) = 0;
        F<uint32_t>(c, 0x332) = 0;
        F<uint32_t>(c, 0x336) = 0;
        NamedObj_Reset(c);
    }
    F<uint32_t>(c, 0x32e) = 0;
    F<uint32_t>(c, 0x332) = 0;
    String_Assign((char**)(c + 0x32e), name);
    String_Assign((char**)(c + 0x332), tex ? tex : name);
    char num[0x10], path[0x100], texName[0x130], prefix[0x130], path2[0x104];
    strcpy(texName, F<const char*>(c, 0x332));
    if (!CarHas(texName)) {
        strcpy(texName, name);
        if (!CarHas(texName) && !CarHas(texName, (const char*)0x5cb7e8)) {
            char* last = texName + strlen(texName) - 1;
            *last = 'l';
            if (!CarHas(texName, (const char*)0x5cb7e8)) {
                *last = 0;
                if (!CarHas(texName, (const char*)0x5cb7e8)) strcpy(texName, tex);
            }
        }
    }
    float px = (float)((double)x * Fl_188(0x5c8700));
    float pz = (float)((double)z * Fl_188(0x5c8700));
    uint8_t* scene = (uint8_t*)Engine_Get1C(hook::As<void*>(g_engine));
    float py = (float)Terrain_HeightAt(scene, 0, px, pz);
    memset(c + 0x262, 0, 0x14);
    F<uint32_t>(c, 0x246) = 0;
    F<uint32_t>(c, 0x242) = 0;
    F<uint32_t>(c, 0x23a) = 0;
    F<uint32_t>(c, 0x23e) = 0;
    CarHelperPoint(path, name, 0x5c9954, &F<uint32_t>(c, 0x23e), &F<float>(c, 0x23a));
    CarHelperPoint(path, name, 0x5cb7d4, &F<uint32_t>(c, 0x242), &F<float>(c, 0x246));
    F<int32_t>(c, 0x31a) = 1;
    for (int32_t i = 1; i < 0xff; ++i) {
        CarFormat(path, 0x5cb7c4, name, CarNum(num, i));
        if (!CarHas(path)) break;
        ++F<int32_t>(c, 0x31a);
    }
    uint32_t bytes = (uint32_t)(F<int32_t>(c, 0x31a) * 0xc0);
    uint8_t* recs = (uint8_t*)((void*(__cdecl*)(uint32_t))0x5a125d)(bytes);
    F<uint8_t*>(c, 0x316) = recs;
    memset(recs, 0, bytes);
    for (int32_t m = 0; m < F<int32_t>(c, 0x31a); ++m) {
        if (!m) {
            CarFormat(prefix, 0x5cb588, name);
            CarFormat(path, 0x5cb7b8, prefix);
            if (reset) WorldModel_Init(c, 0, name, path, texName, 0xbf800000, 0xbf800000, 0);
            F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc) = c;
            CarFormat(path, 0x5cb7a4, prefix);
            if (CarHas(path)) {
                uint8_t* col = Pack_AcquireCollision(g_pack, 0, path);
                const uint32_t* v = **(uint32_t***)(*(uint8_t**)col + 0xc);
                F<uint32_t>(c, 0x31e) = v[0];
                F<uint32_t>(c, 0x322) = v[1];
                F<uint32_t>(c, 0x326) = v[2];
                F<uint32_t>(c, 0x32a) = v[2];
                Pack_Release(g_pack, 0, col);
            }
            for (int32_t k = 0; k < 0x20; ++k) {
                uint8_t* slot = c + 0x276 + k * 5;
                CarFormat(path, 0x5cb790, name, CarNum(num, k + 1));
                if (!CarHas(path)) {
                    F<uint8_t*>(slot, 1) = nullptr;
                    continue;
                }
                slot[0] = 1;
                uint8_t* w = CarModel(nullptr, path, (const char*)0x5cb784);
                F<uint8_t*>(slot, 1) = w;
                ((void(__thiscall*)(uint8_t*))(*(void***)w)[4])(w);
                SceneObj_AddChild(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 0, F<uint8_t*>(slot, 1));
            }
            for (int32_t k = 2; k <= 0x20; ++k) {
                uint8_t* slot = c + 0x276 + (k - 1) * 5;
                ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(num, (const char*)0x5c9aec, k);
                CarFormat(path, 0x5cb770, name, num);
                if (!CarHas(path)) continue;
                if (uint8_t* old = F<uint8_t*>(slot, 1)) {
                    NamedObj_Destructor(old);
                    ((void(__cdecl*)(void*))0x53b080)(old);
                }
                slot[0] = 0;
                uint8_t* w = CarModel(nullptr, path, (const char*)0x5cb764);
                F<uint8_t*>(slot, 1) = w;
                ((void(__thiscall*)(uint8_t*))(*(void***)w)[4])(w);
                SceneObj_AddChild(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 0, F<uint8_t*>(slot, 1));
            }
        } else {
            CarFormat(prefix, 0x5cb758, name, CarNum(num, m));
            CarFormat(path, 0x5cb750, prefix);
            uint8_t* w = CarModel(name, path, texName);
            uint8_t* rs = F<uint8_t*>(c, 0x316);
            F<uint8_t*>(rs + m * 0xc0, 0xc) = w;
            SceneObj_AddChild(F<uint8_t*>(rs, 0xc), 0, F<uint8_t*>(rs + m * 0xc0, 0xc));
        }
        uint8_t* rec = F<uint8_t*>(c, 0x316) + m * 0xc0;
        // Bogies: counted, then loaded.
        for (int32_t pass = 0; pass < 2; ++pass) {
            if (pass == 1) {
                F<void*>(rec, 0x98) = ((void*(__cdecl*)(uint32_t))0x5a125d)((uint32_t)(F<int32_t>(rec, 0) * 4 + 1));
                F<void*>(rec, 0x9c) = ((void*(__cdecl*)(uint32_t))0x5a125d)((uint32_t)(F<int32_t>(rec, 0) * 4 + 1));
            }
            int32_t at = 0;
            for (int32_t i = 1; i < 0xff; ++i) {
                CarFormat(path, 0x5cb740, prefix, CarNum(num, i));
                if (!CarHas(path)) CarFormat(path, 0x5cb730, prefix, CarNum(num, i));
                if (!CarHas(path)) break;
                if (!pass) {
                    ++F<int32_t>(rec, 0);
                    continue;
                }
                uint8_t* w = CarModel(name, path, texName);
                F<uint8_t**>(rec, 0x98)[at] = w;
                SceneObj_AddChild(F<uint8_t*>(rec, 0xc), 0, F<uint8_t**>(rec, 0x98)[at]);
                uint8_t* fr = CarFrame(F<uint8_t**>(rec, 0x98)[at]);
                F<float*>(rec, 0x9c)[at] = (float)((double)F<float>(fr, 0x14) * *(const double*)0x5cb728);
                ++at;
            }
        }
        // Drive wheels with their rods.
        for (int32_t i = 1; i < 0xff; ++i) {
            CarFormat(path, 0x5cb710, prefix, CarNum(num, i));
            if (!CarHas(path)) break;
            int32_t idx = F<int32_t>(F<uint8_t*>(c, 0x316) + m * 0xc0, 4);
            float* pf = (float*)((void*(__cdecl*)(uint32_t))0x5a125d)(0x44);
            F<float*>(rec, 0xa0 + idx * 4) = pf;
            memset(pf, 0, 0x44);
            uint8_t* w = CarModel(name, path, texName);
            F<uint8_t*>(pf, 0x3c) = w;
            uint8_t* fr = CarFrame(w);
            pf[0x10] = (float)((double)F<float>(fr, 0x14) * *(const double*)0x5cb728);
            SceneObj_AddChild(F<uint8_t*>(rec, 0xc), 0, w);
            SceneObj_GetPos2(F<uint8_t*>(pf, 0x3c), 0, (uint32_t*)&pf[5], (uint32_t*)&pf[8], (uint32_t*)&pf[0xb]);
            CarFormat(path, 0x5cb6fc, prefix, CarNum(num, i));
            if (CarHas(path)) {
                uint8_t* bar = CarModel(name, path, texName);
                F<uint8_t*>(pf, 0x38) = bar;
                SceneObj_AddChild(F<uint8_t*>(rec, 0xc), 0, bar);
                SceneObj_GetPos2(F<uint8_t*>(pf, 0x38), 0, (uint32_t*)&pf[4], (uint32_t*)&pf[7], (uint32_t*)&pf[0xa]);
            }
            CarFormat(path, 0x5cb6e4, prefix, CarNum(num, i));
            if (CarHas(path)) {
                CarFormat(path2, 0x5cb6d4, prefix, CarNum(num, i));
                if (CarHas(path2)) {
                    F<uint8_t*>(pf, 0x34) = CarModel(name, path, texName);
                    F<uint8_t*>(pf, 0x30) = CarModel(name, path2, texName);
                    SceneObj_AddChild(F<uint8_t*>(rec, 0xc), 0, F<uint8_t*>(pf, 0x34));
                    SceneObj_AddChild(F<uint8_t*>(rec, 0xc), 0, F<uint8_t*>(pf, 0x30));
                    SceneObj_GetPos2(F<uint8_t*>(pf, 0x30), 0, (uint32_t*)&pf[3], (uint32_t*)&pf[6], (uint32_t*)&pf[9]);
                    SceneObj_GetPos2(F<uint8_t*>(pf, 0x34), 0, (uint32_t*)&pf[4], (uint32_t*)&pf[7], (uint32_t*)&pf[0xa]);
                    float d = (float)((double)pf[9] - pf[0xb]);
                    if (d == 0.0f)
                        F<uint32_t>(pf, 0) = 0x3fc90fdb;
                    else {
                        float a = (float)((double)pf[6] - pf[8]);
                        pf[0] = (float)CarAtan((double)a / d);
                    }
                    float n = (float)((double)pf[9] - pf[0xa]);
                    float dd = (float)((double)pf[6] - pf[7]);
                    pf[1] = (float)CarAtan((double)n / dd);
                }
            }
            ++F<int32_t>(rec, 4);
        }
        uint8_t* r2 = F<uint8_t*>(c, 0x316) + m * 0xc0;
        Effects_SpawnForModel(F<uint8_t*>(r2, 0xc), 0, prefix, (uint8_t**)(r2 + 0x10), 8, 0x400010, 1.0f);
        // Lights.
        for (int32_t i = 1; i < 0xff; ++i) {
            CarFormat(path, 0x5cb6c4, prefix, CarNum(num, i));
            if (!CarHas(path)) break;
            uint8_t* r = F<uint8_t*>(c, 0x316) + m * 0xc0;
            if (F<int32_t>(r, 8) >= 0x15) Fatal_Exit(GetString(0x1b0));
            uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0x35);
            uint8_t* cam = p ? Camera_Construct(p, 0, path, F<uint32_t>(r, 0xc)) : nullptr;
            SceneObj_AddAttachment(F<uint8_t*>(r, 0xc), 0, cam);
            F<uint8_t*>(r, 0x44 + F<int32_t>(r, 8) * 4) = cam;
            ++F<int32_t>(r, 8);
        }
    }
    Obj_MoveTo(F<uint8_t*>(F<uint8_t*>(c, 0x316), 0xc), 0, px, py, pz);
    Vehicle_AttachEngineSound(c, 0, name, scene, px, py, pz);
}
RT3_REPLACE(0x45d4b0, Car_LoadModel);

}  // namespace rt3
