// EngineContainers.cpp - RT3.exe 0x538000-0x53e000.
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
#include "core/ptrlist.h"
#include "core/crt.h"
#include "core/imports.h"
#include "core/stream.h"
#include "core/containers.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

uint8_t* __fastcall Imb_Construct(uint8_t* d, int, int32_t id);

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
int32_t ClockMs();
void __fastcall String_Assign(char** s, const char* src);
void __fastcall Terrain_Remove1675(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
void __fastcall SceneObj_SetAngles(uint8_t* o, int, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Effect_Destroy(uint8_t* e, int, int32_t really);
int __fastcall ToUpper(char c);
uint8_t* __fastcall NameTable_Nth(uint8_t* t, int, int32_t n);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

int32_t __fastcall File_Size(const char* path);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall NameTable_CloseCurrent(uint8_t* t);
void __fastcall NameTable_Add(uint8_t* t, int, const char* dir, const char* name, int32_t size, int32_t counted);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall File_Exists(const char* path);
void __fastcall NameTable_Clear(uint8_t* t);
void __fastcall NameTable_ScanDirs(uint8_t* t);
void __fastcall Fatal_Exit(const char* msg);
int32_t __fastcall NameTable_Locate(uint8_t* t, int, int32_t id, int32_t* pack, int32_t* bucket, int32_t* index, int32_t fatal);
void __fastcall NameTable_Grow(uint8_t* t);
const char* __fastcall GetString(int32_t id);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
int32_t __fastcall Engine_TextureMemLimit(void* e);
float __fastcall Obj_Getc5c(uint8_t* o);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
int32_t __fastcall NameTable_OpenPack(uint8_t* t);
void __fastcall Anims_LoadPack(uint8_t* o, int, const char* path, int32_t);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall SceneObj_Destructor(uint8_t* o);
void __fastcall SceneObj_GetAngles(uint8_t* o, int, uint32_t* a, uint32_t* b, uint32_t* c);
void __fastcall SceneObj_GetPos(uint8_t* o, int, uint32_t* x, uint32_t* y, uint32_t* z);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
void __fastcall PtrList_Clear(PtrList* l);
void __fastcall Group_MoveAllTo(uint8_t* g, int, float x, float y, float z);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
PtrListNode* __fastcall Terrain_Add1675(uint8_t* r, int, void* v, int32_t x, int32_t y);
void* __fastcall Engine_Get1C(void* e);
void __fastcall Effect_SetStyle(uint8_t* e, int, uint8_t kind, uint8_t on);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
using SprintfFn = int(__cdecl*)(char*, const char*, ...);

void __fastcall DebugLog(const char* text, int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g,
                         int32_t h, int32_t i);

void __fastcall Track_Setup(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d,
                            void* terrain, uint8_t style);

void __fastcall FreeAndNull(void** p);
void __fastcall Collection_Destruct(Collection* c);
int32_t __fastcall StrIsBlank(const unsigned char* s);
void __fastcall ModelSet_FreeThunk(uint8_t* s);
void __fastcall Texture_Destroy(uint8_t* t);
void __fastcall BitmapFont_Destroy(uint8_t* f);
void __fastcall Pack_FreeSlot(uint8_t* pack, int, int32_t idx);
uint8_t* __fastcall Pack_Acquire(uint8_t* pack, int, int32_t kind, int32_t key);
uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
uint8_t* __fastcall Font_Construct(uint8_t* t, int, int32_t key);
void __fastcall Font_Create(uint8_t* t);
void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
void __fastcall Fade_StartSlow();
void __fastcall Widget_Redraw(uint8_t* w);
void __fastcall Engine_ResetTextureQueue(void* e);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall Fog_ParamsAt(uint8_t*, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color, float* strength);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Fade_Save();
void __fastcall Fade_Restore();
void __stdcall Fade_Set(uint32_t v);
void ReleaseGlobalResource();
void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
int32_t __fastcall Track_DebugTooltip(uint8_t* t, int, char* dst, int32_t n, int32_t, int32_t, int32_t, int32_t);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
void Engine_Nop();
int32_t __fastcall EditBox_ActivateKey(uint8_t* w, int, int32_t caret);
void __fastcall Input_Block(uint8_t* in);
void __fastcall Input_Unblock(uint8_t* in);
int32_t __fastcall Ui_Dispatch(uint8_t** mgr, int, uint32_t* ev);
int32_t __fastcall Dialog_HandleEvent(uint32_t* ev);
void __fastcall Input_PushEvent(uint8_t* in, int, const uint32_t* ev);
int32_t __fastcall Terrain_LoadTexture(uint8_t* t, int, const char* name, int32_t fullSize);
uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
void __fastcall Terrain_ApplyHeightCurve(uint8_t* t, int, float k);
void __fastcall Terrain_SmoothPeaks(uint8_t* t, int, int32_t r);
void __fastcall Terrain_InitMaps(uint8_t* t, int, char init, int32_t, int32_t);

uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4,
                                      void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH,
                                      uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio,
                                      int32_t check, int32_t* checked, int32_t timeout);

int32_t __fastcall Terrain_ImportHeightmap(uint8_t* t, int, const char* name, float scale, float curve, int32_t smooth);
uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
void __fastcall PtrList_Destroy(PtrList* l);

// --- CRC32 (table 0x624e48, init 0, no final xor) -------------------------

static const uint32_t* const kCrcTable = (const uint32_t*)0x624e48;

// Functions of this file, in exe order.
RT3_NOINLINE uint32_t __fastcall Dialog_RunMessage(const char* text, int32_t type, uint32_t px, uint32_t py, int32_t p4, void(__fastcall* place)(uint8_t*, int, uint32_t, uint32_t, uint32_t, float), int32_t pic, float extraH, uint32_t buttonText, uint32_t a8, int32_t count, const char** choices, int32_t* selected, int32_t radio, int32_t check, int32_t* checked, int32_t timeout);
RT3_NOINLINE void __fastcall Money_PromptTransfer(int32_t* a, int32_t* b);
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE int32_t __fastcall Terrain_ImportHeightmap(uint8_t* t, int, const char* name, float scale, float curve, int32_t smooth);
uint8_t* __fastcall HandleTable_Construct(uint8_t* t);
RT3_NOINLINE void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms);
RT3_NOINLINE void __fastcall Status_SetMessage(uint8_t* s, int, const char* text);
RT3_NOINLINE uint8_t* __fastcall Windows_FindChild(uint8_t* o, int, int32_t id);
void __fastcall HandleTable_ReleaseId(uint8_t* t, int, int32_t id);
void __fastcall HandleTable_Release(uint8_t* t, int, int32_t h);
RT3_NOINLINE int32_t __fastcall HandleTable_Acquire(uint8_t* t, int, uint16_t id);
RT3_NOINLINE int32_t __fastcall Ui_Dispatch(uint8_t** mgr, int, uint32_t* ev);
int32_t __fastcall Chain_ForEach(uint8_t* o, int, uint32_t arg);
RT3_NOINLINE void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win);
float __fastcall Obj_Getc5c(uint8_t* o);
void __fastcall Obj_Setc5c(uint8_t* o, int, uint32_t v);
RT3_NOINLINE void __fastcall Obj_Incc60(uint8_t* o);
RT3_NOINLINE void __fastcall Obj_Decc60(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Obj_Idlec60(uint8_t* o);
RT3_NOINLINE void __fastcall Windows_Shutdown(uint8_t* m);
RT3_NOINLINE void __fastcall WindowManager_Draw(uint8_t* m);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select);
RT3_NOINLINE void __fastcall Ui_ReloadImages(uint8_t* ui);
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus);
void __fastcall Effect_Destroy(uint8_t* e, int, int32_t really);
void __fastcall Light_SetMode(uint8_t* o, int, uint8_t mode);
void __fastcall Effect_SetStyle(uint8_t* e, int, uint8_t kind, uint8_t on);
RT3_NOINLINE void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);
void __fastcall Group_SetAngles(uint8_t* g, int, uint32_t a, uint32_t b);
void __fastcall Group_MoveAllTo(uint8_t* g, int, float x, float y, float z);
RT3_NOINLINE void __fastcall Group_Add(uint8_t* g, int, uint8_t* o, float lift);
void __fastcall Track_BuildGeometry(uint8_t* e);
RT3_NOINLINE double __fastcall Effect_Blend(uint8_t* e, int, float t);
void __fastcall Box_EmitVertices(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t color, int32_t noTop);
RT3_NOINLINE void __fastcall Obj_SetArgb(uint8_t* o, int, uint8_t a, uint8_t r, uint8_t g, uint8_t b);
RT3_NOINLINE void __fastcall Obj_SetRgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b);
void __fastcall Obj_SetRgbValue(uint8_t* o, int, uint32_t v);
void __fastcall Obj_SetAlpha5b(uint8_t* o, int, uint8_t a);
RT3_NOINLINE int32_t __fastcall Track_DebugTooltip(uint8_t* t, int, char* dst, int32_t n, int32_t, int32_t, int32_t, int32_t);
uint32_t __fastcall Get_ccba20();
void __fastcall Texture_SetCcba20(const char* name);
RT3_NOINLINE void ReleaseGlobalResource();
RT3_NOINLINE void __fastcall Nop53a580(void*, int, int32_t);
void __fastcall Effect_Release(uint8_t* e);
int32_t __fastcall Return0_53a5a0(void*);
void __fastcall Group_MoveAllToHeight(uint8_t* g, int, float y);
RT3_NOINLINE void __fastcall Group_SetObject(uint8_t* g, int, uint8_t* o);
void __fastcall Obj_DestroyList8(uint8_t* o);
void __fastcall Track_SetEnds(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d);
void __fastcall Track_DropOnTerrain(uint8_t* e, int, float lift);
void __fastcall Track_SetFlatHeight(uint8_t* e, int, uint32_t h);
void __fastcall SceneLabel_Draw(uint8_t* o);
int32_t __fastcall Track_AfterLoad(uint8_t* e, int, int32_t);
uint8_t* __fastcall Light_Construct(uint8_t* o);
void __fastcall Track_Setup(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d, void* terrain, uint8_t style);
void Mem_ReportLeaks();
RT3_NOINLINE void __fastcall FreeAndNull(void** p);
RT3_NOINLINE void __fastcall FreeAndNull2(void** p);
void __fastcall Set_ccba28();
void Mem_Shutdown();
void __fastcall Anims_LoadPack(uint8_t* o, int, const char* path, int32_t);
int32_t __fastcall NameTable_Locate(uint8_t* t, int, int32_t id, int32_t* pack, int32_t* bucket, int32_t* index, int32_t fatal);
int32_t __fastcall NameTable_Contains(uint8_t* t, int, int32_t id, int32_t fatal);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
uint8_t* __fastcall NameTable_Nth(uint8_t* t, int, int32_t n);
void __fastcall NameTable_CloseCurrent(uint8_t* t);
int32_t __fastcall NameTable_OpenPack(uint8_t* t);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
int32_t __fastcall NameTable_Skip4(uint8_t* t, int, int32_t);
void __fastcall NameTable_Grow(uint8_t* t);
void __fastcall Pack_RecreateFonts(uint8_t* pack);
RT3_NOINLINE void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
void __fastcall NameTable_Clear(uint8_t* t);
RT3_NOINLINE void __fastcall Pack_Shutdown(uint8_t* t);
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
FILE* __fastcall NameTable_OpenFile(uint8_t* t, int, int32_t id, uint8_t text);
void __fastcall Pack_FreeSlot(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint8_t* __fastcall Pack_Acquire(uint8_t* pack, int, int32_t kind, int32_t key);
RT3_NOINLINE uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
void __fastcall NameTable_Add(uint8_t* t, int, const char* dir, const char* name, int32_t size, int32_t counted);
void __fastcall NameTable_ScanDirs(uint8_t* t);
RT3_NOINLINE void __fastcall NameTable_Rescan(uint8_t* t);
RT3_NOINLINE int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
RT3_NOINLINE void __fastcall NameTable_PushPos(uint8_t* t);
RT3_NOINLINE void __fastcall NameTable_PopPosition(uint8_t* t);
int32_t __fastcall NameTable_ReadBlock(uint8_t* t, int, int32_t, void** out);
uint8_t* __fastcall NameTable_Construct(uint8_t* t);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name);
RT3_NOINLINE void __fastcall NameTable_AddPath(uint8_t* t, int, const char* path);
void __fastcall Heap_SiftUp(MaxHeap* h, int, int32_t i);
void __fastcall Heap_SiftDown(MaxHeap* h, int, int32_t i);
int32_t __fastcall Heap_TopKey(MaxHeap* h);
void __fastcall Heap_DecrementKey(MaxHeap* h, int, HeapNode* n);
RT3_NOINLINE ListNode* __fastcall List_Push(MaxList* l, int, int32_t key, int32_t value);
int32_t __fastcall List_PopMax(MaxList* l);
RT3_NOINLINE int32_t __fastcall List_Contains(MaxList* l, int, int32_t value);
void __fastcall Heap_Destroy(MaxHeap* h);
RT3_NOINLINE HeapNode* __fastcall Heap_Push(MaxHeap* h, int, int32_t key, int32_t value);
RT3_NOINLINE int32_t __fastcall Heap_PopMax(MaxHeap* h);
void __fastcall List_Destroy(MaxList* l);
void __fastcall List_Init(MaxList* l, int, int32_t capacity);
RT3_NOINLINE void __fastcall Heap_Destroy2(MaxHeap* h);
RT3_NOINLINE void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity);
void __fastcall List_DestroyThunk(MaxList* l);
RT3_NOINLINE uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
RT3_NOINLINE uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len);
RT3_NOINLINE void __fastcall Hash_Destroy(HashMap* m);
RT3_NOINLINE void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value);
RT3_NOINLINE int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key);
RT3_NOINLINE int32_t __fastcall Hash_Contains(HashMap* m, int, uint32_t key);
RT3_NOINLINE int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
int32_t __fastcall Hash_ForEach(HashMap* m, int, uint32_t key, HashVisit fn, void* ctx);
RT3_NOINLINE void __fastcall Hash_DestroyThunk(HashMap* m);
RT3_NOINLINE int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
RT3_NOINLINE void __fastcall Pool_Free(NodePool* p);
RT3_NOINLINE void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
double __fastcall Font_ScaleX(uint8_t* f, int, float v);
double __fastcall Font_ScaleY(uint8_t* f, int, float v);
RT3_NOINLINE uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
RT3_NOINLINE void __fastcall TextureDesc_Load(uint8_t* d);
RT3_NOINLINE void __fastcall TextureDesc_Reload(uint8_t* d);
void __fastcall Widget_SetValue(uint8_t* w, int, int32_t v);
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall Window_DeleteAllChildren(uint8_t* win);
RT3_NOINLINE uint8_t* __fastcall Texture_ConstructDefault(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Texture_AddAndLoad(uint8_t* t, int, int32_t id, int32_t format);
void __fastcall Font_ReleaseHandles(uint8_t* t);
void __fastcall Texture_ReleaseHandles(uint8_t* t);
void __fastcall Font_Create(uint8_t* t);
uint8_t* __fastcall Font_Construct(uint8_t* t, int, int32_t key);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
uint8_t* __fastcall CollisionFile_Construct(uint8_t* c, int, int32_t id);

// (int)(v * 0x5dcfe8) with the x87 product and the exe's _ftol.
static inline int32_t CellOf(float v) {
    int32_t r;
    __asm {
        fld v
        fmul dword ptr ds:[0x5dcfe8]
        call Rt3_Ftol
        mov r, eax
    }
    return r;
}

// Colour +0x5f (ARGB); low nibble of +0x59 = "tinted" (rgb != 7f7f7f).
static inline void SetTinted(uint8_t* o, uint8_t on) { o[0x59] = (uint8_t)((o[0x59] & 0xf0) | on); }


static inline size_t Len(const char* s) { size_t n = 0; while (s[n]) ++n; return n; }

static inline void Copy(char* d, const char* s) { while ((*d++ = *s++) != 0) {} }


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

static inline int32_t Lseek(int32_t fd, int32_t pos, int32_t origin) {
    return ((int32_t)crt::lseek(fd, pos, origin));
}

static inline FILE* Fdopen(int32_t fd, const char* mode) { return ((FILE*)crt::fdopen(fd, mode)); }

// Byte position of entry (pack, bucket, index) in its pack: the pack's data start
// (+0x1f404 x +0x417ec) + the entry's offset (+8) + 8.
static int32_t EntryPos(uint8_t* t, int32_t pack, int32_t bucket, int32_t index) {
    int32_t base = F<int32_t>(t, 0x1f404 + pack * 4) * F<int32_t>(t, 0x417ec + pack * 4);
    uint8_t* list = F<uint8_t*>(t, (pack * 32 + bucket) * 4);
    return base + F<int32_t>(list + index * 0xd, 8) + 8;
}

static Collection* NewTable(int32_t stride) {
    Collection* c = (Collection*)crt::operator_new(0x88);
    return c ? Collection_Construct(c, 0, 1, stride, 10, 5, 0, 0, 0) : nullptr;
}

// Reads the next whitespace-delimited token of `f` into `dst`.
static void Token(FILE* f, char* dst) { crt::fscanf(f, (const char*)0x5c89e4, dst); }

static void AddImb(char* s) { memcpy(s + strlen(s), (const void*)0x5dd630, 5); }

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


namespace {
uint32_t Message_Dialog(const char* text, uint32_t px) {
    return Dialog_RunMessage(text, 1, px, 0xbf800000, 0, nullptr, 0, 0.0f, 0, 0x1e, 0, nullptr, nullptr, 0, 0, nullptr, 0);
}
}

// --- node pool ------------------------------------------------------------

// Inlined everywhere in the original: storage-backed pools hand out nodes from
// their free list (NULL when exhausted); pools without storage use malloc.
static void* PoolAlloc(NodePool* p) {
    if (p->capacity == 0) return crt::malloc(p->elemSize);
    SList* f = p->freeList;
    void** n = f->head;
    if (!n) return nullptr;
    f->head = (void**)*n;
    if (n == f->tail) f->tail = (void**)*n;
    f->count--;
    return n;
}

static void PoolRelease(NodePool* p, void* node) {
    if (p->capacity == 0 || (uint8_t*)node < p->begin || (uint8_t*)node >= p->end) {
        crt::free(node);
        return;
    }
    SList* f = p->freeList;
    *(void***)node = f->head;
    f->count++;
    f->head = (void**)node;
    if (!f->tail) f->tail = (void**)node;
}

static NodePool* NewPool() {
    NodePool* p = (NodePool*)crt::malloc(sizeof(NodePool));
    if (p) memset(p, 0, sizeof *p);
    return p;
}

// --- hash map -------------------------------------------------------------

static inline uint32_t Bucket(const HashMap* m, uint32_t key) { return ((key >> 15) ^ key) % m->bucketCount; }

// 0x538060. Terrain t: imports heightmap image `name` (texture via format
// 0x5c8730 / 0x5dd3d0 and image via 0x5dd3c8, loaded at full size). The grid
// is the image size rounded up to 64n + 1 (asking for a size above 1024,
// at most 1025); heights are green * 32 for grey images or green * 256 +
// blue otherwise, times `scale`, capped at 0x5c88c0. Fully red (colour) /
// near-white (grey) pixels become water (cell kind 1); afterwards water
// cells touching a height above 0x5c8598 get the default ground back. Then
// the height curve `curve` (unless 0x5c85b4), `smooth` peak smoothing and
// the map rebuild. Returns the texture load result. 0xccb9e8 counts progress.
RT3_NOINLINE int32_t __fastcall Terrain_ImportHeightmap(uint8_t* t, int, const char* name, float scale, float curve, int32_t smooth) {
    char path[0x104], image[0x100];
    FormatText(path, (const char*)0x5c8730, (const char*)0x5dd3d0, name, 0, 0, 0, 0, 0, 0);
    int32_t result = Terrain_LoadTexture(t, 0, path, 1);
    uint8_t* e = g_engine;
    uint8_t keep254 = F<uint8_t>(e, 0x114254), keep255 = F<uint8_t>(e, 0x114255);
    uint32_t keep23f = F<uint32_t>(e, 0x11423f), keep23b = F<uint32_t>(e, 0x11423b);
    F<uint8_t>(e, 0x114255) = 0;
    F<uint8_t>(g_engine, 0x114254) = 0;
    F<uint32_t>(g_engine, 0x11423f) = 0x1000;
    F<uint32_t>(g_engine, 0x11423b) = 0x1000;
    FormatText(image, (const char*)0x5dd3c8, name, 0, 0, 0, 0, 0, 0, 0);
    uint8_t* tex = TextureCache_Get(g_pack, 0, image, 0, 0, 0x7f);
    F<uint32_t>(g_engine, 0x11423b) = keep23b;
    F<uint32_t>(g_engine, 0x11423f) = keep23f;
    F<uint8_t>(g_engine, 0x114255) = keep255;
    F<uint8_t>(g_engine, 0x114254) = keep254;
    int32_t w, h;
    uint32_t* pixels = Texture_Pixels32(tex, 0, &w, &h, 0);
    int32_t total = w * h;
    int32_t gw = w, gh = h;
    while (gw % 64 != 1) ++gw;
    while (gh % 64 != 1) ++gh;
    --gw;
    --gh;
    if (gw > 0x400 || gh > 0x400) Money_PromptTransfer(&gw, &gh);
    if (++gw > 0x401) gw = 0x401;
    if (++gh > 0x401) gh = 0x401;
    ++g_skipCount;
    float sx = (float)w / (float)gw, sy = (float)h / (float)gh;
    ((void(__thiscall*)(uint8_t*, int32_t, int32_t))(*(void***)t)[2])(t, gw, gh);
    ++g_skipCount;
    uint8_t fill = *F<uint8_t*>(t, 0x1655);
    int32_t gray = 1;
    int32_t rows = F<int32_t>(t, 0x15d5);
    for (int32_t i = 0, ry = rows - 1; i < rows && gray; i += 8, ry -= 8) {
        int32_t base = Ftol((double)ry * sy) * w;
        for (int32_t j = 0; j < F<int32_t>(t, 0x15d9); j += 8) {
            int32_t idx = Ftol((double)j * sx) + base;
            if (idx < 0 || idx >= total) continue;
            uint32_t p = pixels[idx];
            if (((p >> 8) ^ p) & 0xff00) {
                gray = 0;
                break;
            }
        }
    }
    for (int32_t y = 0; y < F<int32_t>(t, 0x15d5); ++y) {
        int32_t sr = Ftol((double)(F<int32_t>(t, 0x15d5) - y - 1) * sy);
        if (sr < 0) sr = 0;
        if (sr > h - 1) sr = h - 1;
        int32_t base = sr * w;
        for (int32_t x = 0; x < F<int32_t>(t, 0x15d9); ++x) {
            uint32_t p = pixels[Ftol((double)x * sx) + base];
            uint32_t g = (p >> 8) & 0xff, b = p & 0xff;
            int32_t v = gray ? (int32_t)(g << 5) : (int32_t)((g << 8) | b);
            float hgt = (float)v * scale;
            if (!(hgt < Fl(0x5c88c0))) hgt = Fl(0x5c88c0);
            F<float*>(t, 0x1605)[(F<int32_t>(t, 0x15d9) + 1) * y + x] = hgt;
            bool water = gray ? b >= 0xfa : ((p >> 16) & 0xff) == 0xff;
            if (water && x < F<int32_t>(t, 0x15d9) - 1 && y < F<int32_t>(t, 0x15d5) - 1) {
                F<uint8_t*>(t, 0x1655)[F<int32_t>(t, 0x15dd) * y + x] = 0;
                uint16_t* cell = (uint16_t*)(F<uint8_t*>(t, 0x165d) + (F<int32_t>(t, 0x15dd) * y + x) * 3);
                *cell = (uint16_t)((*cell & 0xfff9) | 1);
            }
            F<uint8_t*>(t, 0x1659)[F<int32_t>(t, 0x15dd) * y + x] = 0;
        }
    }
    for (int32_t y = 0; y < F<int32_t>(t, 0x15d5) - 1; ++y) {
        for (int32_t x = 0; x < F<int32_t>(t, 0x15d9) - 1; ++x) {
            int32_t row = F<int32_t>(t, 0x15dd) * y;
            if ((F<uint8_t*>(t, 0x165d)[(row + x) * 3] & 7) != 1) continue;
            int32_t W = F<int32_t>(t, 0x15d9);
            const float* hp = F<float*>(t, 0x1605) + (W + 1) * y + x;
            const float* hq = F<float*>(t, 0x1605) + (W + 1) * (y + 1) + x;
            float c = Fl(0x5c8598);
            if (hp[0] > c || hp[1] > c || hq[0] > c || hq[1] > c) {
                F<uint8_t*>(t, 0x1655)[row + x] = fill;
                F<uint8_t*>(t, 0x165d)[(F<int32_t>(t, 0x15dd) * y + x) * 3] &= 0xf8;
            }
        }
    }
    g_skipCount += 4;
    if (!(curve == Fl(0x5c85b4))) Terrain_ApplyHeightCurve(t, 0, curve);
    if (smooth > 0) Terrain_SmoothPeaks(t, 0, smooth);
    Pack_Release(g_pack, 0, tex);
    g_skipCount += 3;
    Terrain_InitMaps(t, 0, 1, 0, 0);
    return result;
}
RT3_REPLACE(0x538060, Terrain_ImportHeightmap);

// 0x538640. Handle table constructor: 1000 slots (in-use bytes +0x20, ids +0x408).
uint8_t* __fastcall HandleTable_Construct(uint8_t* t) {
    for (int k = 0; k < 0xfa; ++k) F<int32_t>(t, 0x20 + k * 4) = 0;
    t[0xc68] = 0;
    Collection* c = (Collection*)crt::operator_new(0x88);
    F<Collection*>(t, 0xc69) = c ? Collection_Construct(c, 0, 1, 4, 10, 5, 0, 0, 0) : nullptr;
    static const uint16_t zero[] = { 0xc6d, 0xc71, 0xc75, 0x0, 0x4, 0x8, 0x10, 0xc, 0x14, 0x18, 0x1c, 0xc79, 0xc58, 0xc60, 0xc64 };
    for (uint16_t off : zero) F<int32_t>(t, off) = 0;
    F<uint32_t>(t, 0xc5c) = 0x3e800000;
    return t;
}
RT3_REPLACE(0x538640, HandleTable_Construct);

// 0x5386e0. Shows `text` in the status line (+0x14, with font 7 in +0x18 while
// there is text; one character or less clears it) for `ms` milliseconds (+0x1c =
// end time, 0 = until replaced). Not re-entrant (0xccba18).
RT3_NOINLINE void __fastcall StatusLine_Set(uint8_t* s, int, const char* text, int32_t ms) {
    int32_t& busy = g_ccba18;
    if (busy) return;
    busy = 1;
    if (F<void*>(s, 0x14)) FreeAndNull(&F<void*>(s, 0x14));
    if (strlen(text) > 1) {
        String_Assign(&F<char*>(s, 0x14), text);
        if (!F<void*>(s, 0x18)) F<uint8_t*>(s, 0x18) = Pack_AcquireFont(g_pack, 0, 7);
    } else {
        Pack_Release(g_pack, 0, F<void*>(s, 0x18));
        F<void*>(s, 0x18) = nullptr;
    }
    F<int32_t>(s, 0x1c) = ms ? ClockMs() + ms : 0;
    --busy;
}
RT3_REPLACE(0x5386e0, StatusLine_Set);

// 0x5387a0. Shows a status message (+0x10) for 10 s (+8), guarded against re-entry.
RT3_NOINLINE void __fastcall Status_SetMessage(uint8_t* s, int, const char* text) {
    if (g_ccba1c) return;
    g_ccba1c = 1;
    F<int32_t>(s, 8) = ClockMs() + 10000;
    if (F<void*>(s, 0x10)) FreeAndNull((void**)(s + 0x10));
    size_t n = 0;
    while (text[n]) ++n;
    if (n) String_Assign((char**)(s + 0x10), text);
    g_ccba1c--;
}
RT3_REPLACE(0x5387a0, Status_SetMessage);

// 0x538810. Child `id` of any window in the chain (+0, next +0x58).
RT3_NOINLINE uint8_t* __fastcall Windows_FindChild(uint8_t* o, int, int32_t id) {
    for (uint8_t* w = F<uint8_t*>(o, 0); w; w = F<uint8_t*>(w, 0x58))
        if (uint8_t* c = Widget_FindChild(w, 0, id)) return c;
    return nullptr;
}
RT3_REPLACE(0x538810, Windows_FindChild);

// 0x538840 / 0x538880 / 0x538890. Handles are slot + 60000.
void __fastcall HandleTable_ReleaseId(uint8_t* t, int, int32_t id) {
    for (int32_t i = 0; i < 1000; ++i)
        if ((int32_t)F<uint16_t>(t, 0x408 + i * 2) == id && t[0x20 + i]) t[0x20 + i] = 0;
}
RT3_REPLACE(0x538840, HandleTable_ReleaseId);

void __fastcall HandleTable_Release(uint8_t* t, int, int32_t h) { t[h - 0xea40] = 0; }
RT3_REPLACE(0x538880, HandleTable_Release);

RT3_NOINLINE int32_t __fastcall HandleTable_Acquire(uint8_t* t, int, uint16_t id) {
    for (int32_t i = 0; i < 1000; ++i)
        if (!t[0x20 + i]) {
            t[0x20 + i] = 1;
            F<uint16_t>(t, 0x408 + i * 2) = id;
            return i + 60000;
        }
    return 0;
}
RT3_REPLACE(0x538890, HandleTable_Acquire);

// 0x5388d0. Dispatches an event through the UI: the capture widget (UI manager
// +0xc75) first, then the windows from the manager's first (+0, next +0x58) until
// one returns 0 (consumed). A window answering exactly 1 that changed the event
// makes the dispatch start over at that window (capture first again). A press (type 6) is
// then followed by a 0xb7 event (the press, re-typed). Returns the last answer.
RT3_NOINLINE int32_t __fastcall Ui_Dispatch(uint8_t** mgr, int, uint32_t* ev) {
    uint32_t orig[12];
    memcpy(orig, ev, sizeof orig);
    uint8_t* w = mgr[0];
    int32_t r = 1;
    for (;;) {
        if (uint8_t* cap = F<uint8_t*>(g_windowManager, 0xc75)) {
            r = ((int32_t(__fastcall*)(uint8_t*, int, uint32_t*))(*(void***)cap)[1])(cap, 0, ev);
            if (!r) break;
        }
        uint32_t seen[12];
        memcpy(seen, ev, sizeof seen);
        bool restart = false;
        while (w) {
            uint8_t* next = F<uint8_t*>(w, 0x58);
            r = Window_HandleEvent(w, 0, ev);
            if (!r) break;
            if (r == 1 && memcmp(ev, seen, sizeof seen)) {
                restart = true;
                break;
            }
            w = next;
        }
        if (!restart) break;
    }
    if (orig[0] == 6) {
        orig[0] = 0xb7;
        Ui_Dispatch(mgr, 0, orig);
    }
    return r;
}
RT3_REPLACE(0x5388d0, Ui_Dispatch);

// 0x538990. Calls vtbl[0](arg) along the chain (+0, next +0x58) until one returns 0.
int32_t __fastcall Chain_ForEach(uint8_t* o, int, uint32_t arg) {
    uint8_t* n = F<uint8_t*>(o, 0);
    int32_t r = 1;
    while (n) {
        uint8_t* next = F<uint8_t*>(n, 0x58);
        r = ((int32_t(__thiscall*)(void*, uint32_t))(*(void***)n)[0])(n, arg);
        if (!r) break;
        n = next;
    }
    return r;
}
RT3_REPLACE(0x538990, Chain_ForEach);

// 0x5389c0. Takes window `win` out of the window manager's list (first +0, last
// +4; links next +0x58 / previous +0x54) after deleting its children.
RT3_NOINLINE void __fastcall Windows_Remove(uint8_t* mgr, int, uint8_t* win) {
    if (!win) return;
    Window_DeleteAllChildren(win);
    if (win == F<uint8_t*>(mgr, 4)) F<uint8_t*>(mgr, 4) = F<uint8_t*>(win, 0x54);
    if (win == F<uint8_t*>(mgr, 0)) F<uint8_t*>(mgr, 0) = F<uint8_t*>(win, 0x58);
    if (uint8_t* n = F<uint8_t*>(win, 0x58)) F<uint8_t*>(n, 0x54) = F<uint8_t*>(win, 0x54);
    if (uint8_t* p = F<uint8_t*>(win, 0x54)) F<uint8_t*>(p, 0x58) = F<uint8_t*>(win, 0x58);
}
RT3_REPLACE(0x5389c0, Windows_Remove);

RT3_NOINLINE float __fastcall Obj_Getc5c(uint8_t* o) { return F<float>(o, 0xc5c); }
RT3_REPLACE(0x538a10, Obj_Getc5c);

void __fastcall Obj_Setc5c(uint8_t* o, int, uint32_t v) { F<uint32_t>(o, 0xc5c) = v; }
RT3_REPLACE(0x538a20, Obj_Setc5c);

RT3_NOINLINE void __fastcall Obj_Incc60(uint8_t* o) { F<int32_t>(o, 0xc60)++; }
RT3_REPLACE(0x538a30, Obj_Incc60);

RT3_NOINLINE void __fastcall Obj_Decc60(uint8_t* o) {
    if (--F<int32_t>(o, 0xc60) < 0) F<int32_t>(o, 0xc60) = 0;
}
RT3_REPLACE(0x538a40, Obj_Decc60);

RT3_NOINLINE int32_t __fastcall Obj_Idlec60(uint8_t* o) { return F<int32_t>(o, 0xc60) == 0 ? 1 : 0; }
RT3_REPLACE(0x538a60, Obj_Idlec60);

// 0x538a70. Shuts the window manager down: clears the status line, unlinks every
// window (after deleting its children; the windows themselves stay), frees the
// tooltip strings (+0xbd8, 16 bytes each, count +0xc58), the message (+0x10, font
// +0xc), the status text (+0x14, font +0x18) and the font list +0xc69.
RT3_NOINLINE void __fastcall Windows_Shutdown(uint8_t* m) {
    StatusLine_Set(m, 0, (const char*)0x5c87a8, 0xbb8);
    for (uint8_t* w = F<uint8_t*>(m, 0); w;) {
        uint8_t* next = F<uint8_t*>(w, 0x58);
        Window_DeleteAllChildren(w);
        if (w == F<uint8_t*>(m, 4)) F<uint8_t*>(m, 4) = F<uint8_t*>(w, 0x54);
        if (w == F<uint8_t*>(m, 0)) F<uint8_t*>(m, 0) = F<uint8_t*>(w, 0x58);
        if (uint8_t* n = F<uint8_t*>(w, 0x58)) F<uint8_t*>(n, 0x54) = F<uint8_t*>(w, 0x54);
        if (uint8_t* p = F<uint8_t*>(w, 0x54)) F<uint8_t*>(p, 0x58) = F<uint8_t*>(w, 0x58);
        w = next;
    }
    for (int32_t i = 0; i < F<int32_t>(m, 0xc58); ++i) crt::free(F<void*>(m, 0xbd8 + i * 0x10));
    F<int32_t>(m, 0xc58) = 0;
    FreeAndNull(&F<void*>(m, 0x10));
    if (F<void*>(m, 0xc)) Pack_Release(g_pack, 0, F<void*>(m, 0xc));
    F<void*>(m, 0xc) = nullptr;
    FreeAndNull(&F<void*>(m, 0x14));
    if (F<void*>(m, 0x18)) Pack_Release(g_pack, 0, F<void*>(m, 0x18));
    F<void*>(m, 0x18) = nullptr;
    if (Collection* fonts = F<Collection*>(m, 0xc69)) {
        Collection_Destruct(fonts);
        crt::operator_delete(fonts);
    }
    F<Collection*>(m, 0xc69) = nullptr;
}
RT3_REPLACE(0x538a70, Windows_Shutdown);

// 0x538b60. Draws the windows (list +4, next +0x54, the engine's texture queue
// reset after each) unless frozen (+0xc68). Before that, an expired status line
// (+0x1c) is cleared. After that come the status text (+0x10, until +8, in font
// 3, loaded on first use into +0xc) and the tooltip (+0x14, longer than one
// character, in font +0x18, translucent yellow, without shadow).
RT3_NOINLINE void __fastcall WindowManager_Draw(uint8_t* m) {
    if (m[0xc68]) return;
    Fade_StartSlow();
    if (F<int32_t>(m, 0x1c) && F<int32_t>(m, 0x1c) < ClockMs()) StatusLine_Set(m, 0, (const char*)0x5c87a8, 0xbb8);
    for (uint8_t* w = F<uint8_t*>(m, 4); w;) {
        Widget_Redraw(w);
        w = F<uint8_t*>(w, 0x54);
        Engine_ResetTextureQueue(hook::As<void*>(g_engine));
    }
    const char* status = F<const char*>(m, 0x10);
    if (status && *status && ClockMs() < F<int32_t>(m, 8)) {
        if (!F<uint8_t*>(m, 0xc)) F<uint8_t*>(m, 0xc) = Pack_AcquireFont(g_pack, 0, 3);
        Font_DrawTextBox(F<uint8_t*>(m, 0xc), 0, F<const char*>(m, 0x10), BitsF(0x3dcccccd), 0.0f, BitsF(0x3f4ccccc), 1.0f, 0xffffffff, 3, 1, 0, 1.0f);
    }
    const char* tip = F<const char*>(m, 0x14);
    if (tip && strlen(tip) > 1)
        Font_DrawTextBox(F<uint8_t*>(m, 0x18), 0, tip, BitsF(0x3dcccccd), BitsF(0x3e99999a), BitsF(0x3f4ccccd), BitsF(0x3ecccccd), 0x80ffff60, 0, 0, 2,
                         1.0f);
}
RT3_REPLACE(0x538b60, WindowManager_Draw);

// 0x538c70. Shows tooltip `text` (null: 0x624d64) with priority `prio`. Ignored
// while the engine is busy (+0x57) or a non-empty higher-priority tip is still
// showing; the same text only renews (2 s). Otherwise the text is copied (+0xbd8,
// +0xc58 set), +0xbdc = prio, +0xbe0 = `data`, shown until now + 4 s (`sticky`: ~28
// h; blank text: next ms), and an 0xae event carrying it is dispatched (the
// original leaves the event's other fields uninitialized). 1 if shown.
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data) {
    if (g_engine[0x57]) return 0;
    if (!text) text = g_624d64;
    if (F<int32_t>(ui, 0xc58) && prio < F<int32_t>(ui, 0xbdc) && F<int32_t>(ui, 0xbe4) > ClockMs() && *F<char*>(ui, 0xbd8)) return 0;
    if (F<int32_t>(ui, 0xc58)) {
        if (!strcmp(text, F<char*>(ui, 0xbd8))) {
            F<int32_t>(ui, 0xbdc) = prio;
            F<int32_t>(ui, 0xbe4) = ClockMs() + 2000;
            return 0;
        }
        if (F<int32_t>(ui, 0xc58)) crt::free(F<char*>(ui, 0xbd8));
    }
    F<int32_t>(ui, 0xc58) = 1;
    F<char*>(ui, 0xbd8) = (char*)crt::malloc(strlen(text) + 1);
    strcpy(F<char*>(ui, 0xbd8), text);
    F<int32_t>(ui, 0xbe0) = data;
    F<int32_t>(ui, 0xbdc) = prio;
    F<int32_t>(ui, 0xbe4) = ClockMs() + (sticky ? 99999999 : 4000);
    if (StrIsBlank(F<const unsigned char*>(ui, 0xbd8))) F<int32_t>(ui, 0xbe4) = ClockMs() + 1;
    uint32_t ev[12] = {};
    ev[0] = 0xae;
    ev[2] = (uint32_t)(uintptr_t)F<char*>(ui, 0xbd8);
    Ui_Dispatch((uint8_t**)ui, 0, ev);
    return 1;
}
RT3_REPLACE(0x538c70, Ui_ShowTooltip);

// 0x538e00. Dispatches an event with its first five fields set through the UI.
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    uint32_t ev[12] = {};
    ev[0] = type;
    ev[1] = id;
    ev[2] = a;
    ev[3] = b;
    ev[4] = c;
    return Ui_Dispatch(mgr, 0, ev);
}
RT3_REPLACE(0x538e00, Ui_SendEvent5);

// 0x538e50. Inserts widget w into list L ({last, first}, links +0x54 next / +0x58
// previous) after every widget whose depth (+0x21) is not greater; `select` first
// sets its value to 1.
RT3_NOINLINE void __fastcall WidgetList_Insert(uint8_t** L, int, uint8_t* w, int32_t select) {
    if (select) Widget_SetValue(w, 0, 1);
    uint8_t* at = L[1];
    F<uint8_t*>(w, 0x58) = nullptr;
    while (at && F<int32_t>(at, 0x21) <= F<int32_t>(w, 0x21)) {
        F<uint8_t*>(w, 0x58) = at;
        at = F<uint8_t*>(at, 0x54);
    }
    if (at == L[1]) L[1] = w;
    if (!at) L[0] = w;
    F<uint8_t*>(w, 0x54) = at;
    if (at) F<uint8_t*>(at, 0x58) = w;
    if (uint8_t* prev = F<uint8_t*>(w, 0x58)) F<uint8_t*>(prev, 0x54) = w;
}
RT3_REPLACE(0x538e50, WidgetList_Insert);

// 0x538ec0. Reloads every interface image descriptor of ui's list (+0xc69).
RT3_NOINLINE void __fastcall Ui_ReloadImages(uint8_t* ui) {
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(ui, 0xc69)); ++i) {
        Collection* c = F<Collection*>(ui, 0xc69);
        uint8_t** rec = (uint8_t**)Collection_Get(c, 0, Collection_IdAt(c, 0, (uint32_t)i, 0));
        TextureDesc_Reload(*rec);
    }
}
RT3_REPLACE(0x538ec0, Ui_ReloadImages);

// 0x538f10. Runs window `win` (added to UI manager mgr with +0x6c = a2 and
// +0x21 = a4, the box `focus` activated) modally: each input event goes to
// the input hook (0x6d401c +0xc75), the dialog layer (0x6d3b4c) and the
// window, again while a handler keeps rewriting it, then to the window's
// own handler (vtable +4). Returns the value of event 0xc9; event 0xcd is
// re-posted and ends it with 0; so does the timeout (ms, dropped by events
// 3 / 6 / 8). Idles through 0x6d4004 every 20 ms or on an empty queue.
RT3_NOINLINE uint32_t __fastcall Ui_RunModal(uint8_t** mgr, int, uint8_t* win, int32_t a2, int32_t timeout, int32_t a4, int32_t focus) {
    uint8_t* m = (uint8_t*)mgr;
    ++F<int32_t>(m, 0xc64);
    int32_t start = ClockMs();
    uint32_t result = 0;
    if (win) {
        F<int32_t>(win, 0x21) = a4;
        F<int32_t>(win, 0x6c) = a2;
        WidgetList_Insert(mgr, 0, win, 1);
    }
    if (focus) {
        uint8_t* box = nullptr;
        for (uint8_t* w = *mgr; w; w = F<uint8_t*>(w, 0x58))
            if ((box = Widget_FindChild(w, 0, focus))) break;
        EditBox_ActivateKey(box, 0, 99999);
    }
    int32_t lastPump = ClockMs();
    uint32_t prev[12];
    prev[0] = 0;
    Input_Block(g_input);
    for (;;) {
        uint32_t ev[12], saved[12], scratch[12];
        // Through the call site 0x538fbe: the determinism harness wraps it to dismiss modals.
        memcpy(ev, hook::CallTarget<void*(__fastcall*)(uint8_t*, int, void*)>(0x538fbe)(g_input, 0, scratch), sizeof ev);
        uint32_t type = ev[0];
        bool settled = false;
        for (;;) {
            if (prev[0] == 6) {
                prev[0] = 0xb7;
                Ui_Dispatch(mgr, 0, prev);
            }
            memcpy(saved, ev, sizeof ev);
            memcpy(prev, ev, sizeof ev);
            int32_t r = 1;
            if (uint8_t* hk = F<uint8_t*>(g_windowManager, 0xc75)) r = ((int32_t(__thiscall*)(uint8_t*, uint32_t*))(*(void***)hk)[1])(hk, ev);
            if (r) {
                if (hook::As<void*>(g_6d3b4c)) r = Dialog_HandleEvent(ev);
                if (r && win) r = Window_HandleEvent(win, 0, ev);
            }
            if (type == 3 || type == 6 || type == 8) timeout = 0;
            if (ev[0] == 0xc9) {
                result = ev[1];
                goto done;
            }
            if (ev[0] == 0xcd) {
                Input_PushEvent(g_input, 0, ev);
                goto done;
            }
            if (memcmp(ev, saved, sizeof ev)) {
                if (r == 1) continue;
            } else if (r == 1 && ((int32_t(__thiscall*)(uint8_t*, uint32_t*))(*(void***)win)[1])(win, ev) == 1)
                settled = true;
            break;
        }
        if (!settled && (type == 3 || type == 6 || type == 8)) timeout = 0;
        if (ev[0] == 0xc9) {
            result = ev[1];
            break;
        }
        if (timeout && start + timeout < ClockMs()) {
            result = 0;
            break;
        }
        Engine_Nop();
        if (ClockMs() - lastPump <= 0x14 && ev[0]) continue;
        if (void (*pump)() = hook::Global<void (*)()>(0x6d4004)) pump();
        lastPump = ClockMs();
    }
done:
    Input_Unblock(g_input);
    --F<int32_t>(m, 0xc64);
    return result;
}
RT3_REPLACE(0x538f10, Ui_RunModal);

// 0x5391b0. Destroys a placed effect: removes it from its terrain block list
// (+0x55; unless the engine is shutting down), frees +0x1e3 and calls vtbl[0].
RT3_NOINLINE void __fastcall Effect_Destroy(uint8_t* e, int, int32_t really) {
    if (!really) return;
    if (!F<int32_t>(g_engine, 0x114256)) {
        if (uint8_t* t = F<uint8_t*>(e, 4)) {
            int32_t y = CellOf(F<float>(e, 0x10)) >> 4;
            int32_t x = CellOf(F<float>(e, 0xc)) >> 4;
            Terrain_Remove1675(t, 0, F<PtrListNode*>(e, 0x55), x, y);
        }
    }
    if (F<void*>(e, 0x1e3)) {
        crt::free(F<void*>(e, 0x1e3));
        F<void*>(e, 0x1e3) = nullptr;
    }
    ((void(__thiscall*)(void*))(*(void***)e)[0])(e);
}
RT3_REPLACE(0x5391b0, Effect_Destroy);

// 0x539230. Sets the light's colours (+0x73..+0x11f) for mode 0..3 (+0x5a).
void __fastcall Light_SetMode(uint8_t* o, int, uint8_t mode) {
    const uint32_t a65 = 0x3f266666, a85 = 0x3f59999a, half = 0x3f000000, one = 0x3f800000;
    F<uint32_t>(o, 0xeb) = a65;
    F<uint32_t>(o, 0x8b) = a65;
    F<uint32_t>(o, 0x103) = a85;
    F<uint32_t>(o, 0xa3) = a85;
    o[0x5a] = mode;
    F<uint32_t>(o, 0xd3) = half;
    F<uint32_t>(o, 0x73) = half;
    F<uint32_t>(o, 0x11b) = one;
    F<uint32_t>(o, 0xbb) = one;
    F<uint32_t>(o, 0xbf) = half;
    F<uint32_t>(o, 0xa7) = half;
    F<uint32_t>(o, 0x8f) = half;
    F<uint32_t>(o, 0x77) = half;
    F<uint32_t>(o, 0x11f) = one;
    F<uint32_t>(o, 0x107) = one;
    F<uint32_t>(o, 0xef) = one;
    F<uint32_t>(o, 0xd7) = one;
    static const uint16_t lo[4] = { 0x77, 0x8f, 0xa7, 0xbf }, hi[4] = { 0xd7, 0xef, 0x107, 0x11f };
    if (mode == 2) {
        for (int k = 0; k < 4; ++k) {
            F<uint32_t>(o, lo[k]) = 0;
            F<uint32_t>(o, hi[k]) = half;
        }
        F<uint32_t>(o, 0x8b) = 0x3f333333;
        F<uint32_t>(o, 0xa3) = 0x3f4ccccd;
    } else if (mode == 3) {
        for (int k = 0; k < 4; ++k) {
            F<uint32_t>(o, lo[k]) = half;
            F<uint32_t>(o, hi[k]) = 0;
        }
        F<uint32_t>(o, 0xeb) = 0x3f333333;
        F<uint32_t>(o, 0x103) = 0x3f4ccccd;
    } else if (mode == 1) {
        F<uint32_t>(o, 0xbb) = half;
        F<uint32_t>(o, 0x11b) = half;
        F<uint32_t>(o, 0x8b) = 0x3e4ccccb;
        F<uint32_t>(o, 0xeb) = 0x3e4ccccb;
        F<uint32_t>(o, 0x73) = 0;
        F<uint32_t>(o, 0xd3) = 0;
        F<uint32_t>(o, 0xa3) = 0x3e99999a;
        F<uint32_t>(o, 0x103) = 0x3e99999a;
    }
}
RT3_REPLACE(0x539230, Light_SetMode);

// 0x539380. Applies effect style `kind` (1/2/4 -> 0/1/2) from the tables at
// 0x624d68.., with +0x44 = style + 1 when enabled.
RT3_NOINLINE void __fastcall Effect_SetStyle(uint8_t* e, int, uint8_t kind, uint8_t on) {
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
RT3_NOINLINE void __fastcall Effect_GetVectors(uint8_t* e, int, uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
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

// 0x539580. Moves every object of the group (list +8) to (x, y, z).
RT3_NOINLINE void __fastcall Group_MoveAllTo(uint8_t* g, int, float x, float y, float z) {
    PtrList* l = F<PtrList*>(g, 8);
    if (!l) return;
    PtrList_Rewind(l);
    for (uint8_t* o = (uint8_t*)PtrList_Next(l); o; o = (uint8_t*)PtrList_Next(F<PtrList*>(g, 8))) Obj_MoveTo(o, 0, x, y, z);
}
RT3_REPLACE(0x539580, Group_MoveAllTo);

// 0x5395d0. Adds `o` to the group (creating its list +8) and places it at the
// group's point (+0xc, +0x10) at `lift` above the mean of the corner heights
// (+0x18 / +0x24 / +0x30 / +0x3c) less one.
RT3_NOINLINE void __fastcall Group_Add(uint8_t* g, int, uint8_t* o, float lift) {
    if (!F<PtrList*>(g, 8)) {
        PtrList* l = (PtrList*)crt::operator_new(0x18);
        F<PtrList*>(g, 8) = l ? PtrList_Init(l) : nullptr;
    }
    PtrList_Append(F<PtrList*>(g, 8), 0, o);
    double sum = (double)F<float>(g, 0x3c) + F<float>(g, 0x30);
    sum = sum + F<float>(g, 0x24);
    sum = sum + F<float>(g, 0x18);
    float y = (float)(sum * *(const double*)rt3::Rdata(0x5c8758) - *(const double*)rt3::Rdata(0x5c8770) + lift);
    Obj_MoveTo(o, 0, F<float>(g, 0xc), y, F<float>(g, 0x10));
}
RT3_REPLACE(0x5395d0, Group_Add);

// 0x539640. Rebuilds a track piece's geometry from its end points (+0x14..+0x28 and
// +0x2c..+0x40): the corner vertices of its two cross sections (rail half widths
// +0x1f5 / +0x1f9 in 0x18-byte vertices +0x63..+0x123, with terrain heights at
// four of them, kept at most 0.45 below the bed), the ballast bed (half width
// +0x1fd, top 0.45 / 0.9 up, +0x123..+0x1d3), optionally a side quad set (+0x1e3,
// kind +0x44: 1 = end posts, 2 / 3 = wall panels, heights +0x45..+0x51) and the
// piece's centre (+0xc / +0x10). Also caches one corner height in 0x624e00.
void __fastcall Track_BuildGeometry(uint8_t* e) {
    const double half = D(0x5c8628), c45 = D(0x5c8c18), c90 = D(0x5c8888);
    auto f = [&](uint32_t o) { return (double)F<float>(e, o); };
    auto P = [&](uint32_t o) -> float& { return F<float>(e, o); };
    float d0 = (float)((f(0x14) - f(0x20)) * half), d1 = (float)((f(0x18) - f(0x24)) * half);
    float d2 = (float)((f(0x1c) - f(0x28)) * half), d3 = (float)((f(0x38) - f(0x2c)) * half);
    float d4 = (float)((f(0x3c) - f(0x30)) * half), d5 = (float)((f(0x40) - f(0x34)) * half);
    float m0 = (float)((f(0x20) + f(0x14)) * half), m1 = (float)((f(0x18) + f(0x24)) * half);
    float m2 = (float)((f(0x1c) + f(0x28)) * half), m3 = (float)((f(0x38) + f(0x2c)) * half);
    float m4 = (float)((f(0x30) + f(0x3c)) * half), m5 = (float)((f(0x40) + f(0x34)) * half);
    double w = f(0x1f5), v = f(0x1f9);
    double a = d0 * w, b = d1 * w, c = d2 * w;
    P(0x7b) = (float)(m0 + a);
    P(0x7f) = (float)(m1 + b);
    P(0x83) = (float)(c + m2);
    P(0x93) = (float)(m0 - a);
    P(0x97) = (float)(m1 - b);
    P(0x9b) = (float)((double)m2 - (float)c);
    a = d3 * v;
    b = d4 * v;
    c = d5 * v;
    P(0xdb) = (float)(m3 + a);
    P(0xdf) = (float)(m4 + b);
    P(0xe3) = (float)(c + m5);
    P(0xf3) = (float)(m3 - a);
    P(0xf7) = (float)(m4 - b);
    P(0xfb) = (float)((double)m5 - (float)c);
    double W = w + half, V = v + half;
    float pw = (float)(W * d0);
    float ax = (float)(m0 - pw);
    P(0xab) = ax;
    float qw = (float)(W * d2);
    float az = (float)(m2 - qw);
    P(0xb3) = az;
    float rw = (float)(d3 * V);
    P(0x10b) = (float)(m3 - rw);
    double sw = V * d5;
    P(0x113) = (float)(m5 - sw);
    P(0x63) = (float)(pw + (double)m0);
    P(0x6b) = (float)(qw + (double)m2);
    P(0xc3) = (float)(rw + (double)m3);
    P(0xcb) = (float)(sw + m5);
    uint8_t* t = F<uint8_t*>(e, 4);
    P(0xaf) = (float)Terrain_HeightAt(t, 0, ax, az);
    P(0x10f) = (float)Terrain_HeightAt(t, 0, P(0x10b), P(0x113));
    P(0x67) = (float)Terrain_HeightAt(t, 0, P(0x63), P(0x6b));
    P(0xc7) = (float)Terrain_HeightAt(t, 0, P(0xc3), P(0xcb));
    X87Copy((void*)0x624e00, e + 0x67 + g_ccba24 * 0x18);

    double Z = f(0x1fd);
    double t0 = d0 * Z, t1 = d1 * Z, t2 = d2 * Z, t3 = d3 * Z, t4 = d4 * Z, t5 = d5 * Z;
    double s0 = t0 + m0;
    P(0x123) = (float)s0;
    float s2 = (float)(t2 + m2);
    P(0x12b) = s2;
    double s1 = t1 + m1;
    float top1 = (float)(c45 + s1);
    P(0x127) = top1;
    float r0 = (float)((double)m0 - (float)t0);
    P(0x13b) = r0;
    float r2 = (float)((double)m2 - (float)t2);
    P(0x143) = r2;
    double r1 = (double)m1 - (float)t1;
    float top1b = (float)(c45 + r1);
    P(0x13f) = top1b;
    float s3 = (float)(m3 + t3);
    P(0x153) = s3;
    float s5 = (float)(m5 + t5);
    P(0x15b) = s5;
    double s4 = t4 + m4;
    float top4 = (float)(s4 + c45);
    P(0x157) = top4;
    float r3 = (float)(m3 - t3);
    P(0x16b) = r3;
    float r5 = (float)(m5 - t5);
    P(0x18b) = s2;
    P(0x1b3) = s3;
    P(0x1d3) = r5;
    P(0x173) = r5;
    P(0x19b) = r0;
    double r4 = (double)m4 - (float)t4;
    P(0x1a3) = r2;
    P(0x1bb) = s5;
    P(0x1cb) = r3;
    float top4b = (float)(r4 + c45);
    P(0x16f) = top4b;
    P(0x183) = (float)s0;
    P(0x187) = (float)(s1 + c90);
    P(0x19f) = (float)(r1 + c90);
    P(0x1b7) = (float)(s4 + c90);
    P(0x1cf) = (float)(r4 + c90);
    auto clamp = [&](uint32_t o, float top) {
        double lim = (double)top - c45;
        double h = f(o);
        P(o) = (float)(h < lim ? h : lim);
    };
    clamp(0x67, top1);
    clamp(0xaf, top1b);
    clamp(0xc7, top4);
    clamp(0x10f, top4b);

    uint8_t kind = e[0x44];
    if (!kind) {
        if (F<void*>(e, 0x1e3)) {
            crt::free(F<void*>(e, 0x1e3));
            F<void*>(e, 0x1e3) = nullptr;
        }
    } else {
        if (!F<void*>(e, 0x1e3)) F<void*>(e, 0x1e3) = crt::malloc(0xc0);
        uint32_t* q = F<uint32_t*>(e, 0x1e3);
        static const uint32_t uv[8][2] = { { 0x3e0f5c29, 0 }, { 0x3e800000, 0 }, { 0x3e0f5c29, 0x3f000000 }, { 0x3e800000, 0x3f000000 },
                                           { 0x3e0f5c29, 0 }, { 0x3e800000, 0 }, { 0x3e0f5c29, 0x3f000000 }, { 0x3e800000, 0x3f000000 } };
        for (int i = 0; i < 8; ++i) q[i * 6 + 3] = 0xff7f7f7f;
        for (int i = 0; i < 8; ++i) {
            q[i * 6 + 4] = uv[i][0];
            q[i * 6 + 5] = uv[i][1];
        }
        float* bb = (float*)q;
        double h45 = f(0x45), h49 = f(0x49), h4d = f(0x4d), h51 = f(0x51);
        if (kind == 1) {
            bb[0] = m3;
            bb[1] = (float)(m4 + h45);
            bb[2] = m5;
            bb[6] = m3;
            bb[7] = (float)((m4 + h49) + h45);
            bb[8] = m5;
            bb[12] = m0;
            bb[13] = (float)(m1 + h4d);
            bb[14] = m2;
            bb[18] = m0;
            bb[19] = (float)((h51 + h4d) + m1);
            bb[20] = m2;
        } else if (kind <= 3) {
            float hw = (float)(w * half);
            float gw = (float)(v * half);
            if (kind == 2) gw = 0.0f;
            double g3 = (double)gw * d3, g4 = (double)gw * d4, g5 = (double)gw * d5;
            double h0 = (double)hw * d0, h1 = (double)hw * d1, h2 = (double)hw * d2;
            double x0 = m3 + g3, z0 = g5 + m5, x1 = h0 + m0, z1 = h2 + m2;
            bb[0] = (float)x0;
            bb[1] = (float)((g4 + m4) + h45);
            bb[2] = (float)z0;
            bb[6] = (float)x0;
            bb[7] = (float)((((double)(float)g4 + h49) + m4) + h45);
            bb[8] = (float)z0;
            bb[12] = (float)x1;
            bb[13] = (float)((h1 + h4d) + m1);
            bb[14] = (float)z1;
            bb[18] = (float)x1;
            bb[19] = (float)((((double)(float)h1 + h4d) + h51) + m1);
            bb[20] = (float)z1;
            double x2 = m3 - g3;
            double q4 = (double)m4 - (float)g4;
            double z2 = (double)m5 - (float)g5;
            bb[24] = (float)x2;
            bb[25] = (float)(q4 + h45);
            bb[26] = (float)z2;
            bb[30] = (float)x2;
            bb[31] = (float)(((double)(float)q4 + h49) + h45);
            bb[32] = (float)z2;
            double x3 = (double)m0 - (float)h0;
            double q1 = (double)m1 - (float)h1;
            double z3 = (double)m2 - (float)h2;
            bb[36] = (float)x3;
            bb[37] = (float)(q1 + h4d);
            bb[38] = (float)z3;
            bb[42] = (float)x3;
            bb[43] = (float)(((double)(float)q1 + h4d) + h51);
            bb[44] = (float)z3;
        }
    }
    P(0xc) = (float)((((f(0x20) + f(0x38)) + f(0x2c)) + f(0x14)) * D(0x5c8758));
    P(0x10) = (float)((((f(0x40) + f(0x34)) + f(0x1c)) + f(0x28)) * D(0x5c8758));
}
RT3_REPLACE(0x539640, Track_BuildGeometry);

// 0x539f90. Linear blend (1 - t) * +0x1f1 + t * +0x1ed (unrounded).
RT3_NOINLINE double __fastcall Effect_Blend(uint8_t* e, int, float t) {
    return ((double)*(const float*)rt3::Rdata(0x5c85b4) - t) * F<float>(e, 0x1f1) + (double)t * F<float>(e, 0x1ed);
}
RT3_REPLACE(0x539f90, Effect_Blend);

// 0x539fb0. Emits a box model as triangle-list vertices (0x18 bytes each) into `out`,
// counting them in *count: the 16 corner vertices (+0x63) are first tinted (+0x5f
// when +0x59 & 0xf, else `color` keeping their alpha); then the sides (+0x59 & 0x10,
// 18 vertices), the top unless `noTop` (12) and the roof from +0x1e3 (kind +0x44:
// 1 one face, 2/3 two).
void __fastcall Box_EmitVertices(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t color, int32_t noTop) {
    for (int32_t i = 0; i < 16; ++i) {
        uint32_t& c = F<uint32_t>(o, 0x6f + i * 0x18);
        c = o[0x59] & 0xf ? F<uint32_t>(o, 0x5f) : (c & 0xff000000) | color;
    }
    auto emit = [&](const uint8_t* src) {
        memcpy(out, src, 0x18);
        out += 0x18;
    };
    if (o[0x59] & 0x10) {
        static const uint16_t sides[18] = { 0x7b, 0xf3, 0xdb, 0x7b, 0x93, 0xf3, 0x63, 0xc3, 0xdb,
                                            0x63, 0xdb, 0x7b, 0x93, 0xf3, 0x10b, 0x93, 0x10b, 0xab };
        for (uint16_t v : sides) emit(o + v);
        *count += 0x12;
    }
    if (!noTop) {
        static const uint16_t top[12] = { 0x123, 0x153, 0x16b, 0x123, 0x16b, 0x13b, 0x183, 0x1b3, 0x1cb, 0x183, 0x1cb, 0x19b };
        for (uint16_t v : top) emit(o + v);
        *count += 0xc;
    }
    uint8_t kind = o[0x44];
    if (kind == 0 || kind > 3) return;
    if (kind != 1) {
        static const uint8_t back[6] = { 0x60, 0x78, 0x90, 0xa8, 0x90, 0x78 };
        for (uint8_t v : back) emit(F<uint8_t*>(o, 0x1e3) + v);
        *count += 6;
    }
    static const uint8_t front[6] = { 0, 0x18, 0x30, 0x48, 0x30, 0x18 };
    for (uint8_t v : front) emit(F<uint8_t*>(o, 0x1e3) + v);
    *count += 6;
}
RT3_REPLACE(0x539fb0, Box_EmitVertices);

RT3_NOINLINE void __fastcall Obj_SetArgb(uint8_t* o, int, uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    SetTinted(o, r == 0x7f && g == 0x7f && b == 0x7f ? 0 : 1);
    F<uint32_t>(o, 0x5f) = ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
RT3_REPLACE(0x53a350, Obj_SetArgb);

RT3_NOINLINE void __fastcall Obj_SetRgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b) {
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

// 0x53a450. Tooltip for a track piece in debug mode (engine +0x1146bd): "Midpoint
// x, z" (string 0xa9, two decimals) copied into `dst` (at most n-1 chars); returns
// whether it wrote one. The other arguments are unused.
RT3_NOINLINE int32_t __fastcall Track_DebugTooltip(uint8_t* t, int, char* dst, int32_t n, int32_t, int32_t, int32_t, int32_t) {
    if (!g_engine[0x1146bd]) return 0;
    char zs[0x40], xs[0x80], text[0x1f0];
    Num_Format(zs, 3, (double)F<float>(t, 0x10), 10, 1, 0, 0);
    Num_Format(xs, 3, (double)F<float>(t, 0xc), 10, 1, 0, 0);
    FormatText(text, GetString(0xa9), xs, zs, 0, 0, 0, 0, 0, 0);
    StrCopyN(dst, text, n - 1);
    return 1;
}
RT3_REPLACE(0x53a450, Track_DebugTooltip);

uint32_t __fastcall Get_ccba20() { return g_ccba20; }
RT3_REPLACE(0x53a510, Get_ccba20);

void __fastcall Texture_SetCcba20(const char* name) {
    if (name) Texture_Replace(0xccba20, name);
}
RT3_REPLACE(0x53a520, Texture_SetCcba20);

// 0x53a560. Releases the pack resource held in 0xccba20.
RT3_NOINLINE void ReleaseGlobalResource() {
    void*& r = hook::As<void*>(g_ccba20);
    if (!r) return;
    Pack_Release(g_pack, 0, r);
    r = nullptr;
}
RT3_REPLACE(0x53a560, ReleaseGlobalResource);

RT3_NOINLINE void __fastcall Nop53a580(void*, int, int32_t) { __asm nop }
RT3_REPLACE(0x53a580, Nop53a580);

void __fastcall Effect_Release(uint8_t* e) {
    F<uint32_t>(e, 0) = 0x5dd3d8;
    Effect_Destroy(e, 0, 1);
}
RT3_REPLACE(0x53a590, Effect_Release);

// 0x53a5a0 (vtable stub).
int32_t __fastcall Return0_53a5a0(void*) { return 0; }
RT3_REPLACE(0x53a5a0, Return0_53a5a0);

// 0x53a5b0. Moves the group's objects to its point (+0xc, +0x10) at height y.
void __fastcall Group_MoveAllToHeight(uint8_t* g, int, float y) {
    Group_MoveAllTo(g, 0, F<float>(g, 0xc), y, F<float>(g, 0x10));
}
RT3_REPLACE(0x53a5b0, Group_MoveAllToHeight);

// 0x53a5d0. Puts `o` into the group: the first one is added normally; later
// ones replace the group's object, taking over its angles and position (the
// old one is deleted).
RT3_NOINLINE void __fastcall Group_SetObject(uint8_t* g, int, uint8_t* o) {
    if (!F<PtrList*>(g, 8)) {
        PtrList* l = (PtrList*)crt::operator_new(0x18);
        F<PtrList*>(g, 8) = l ? PtrList_Init(l) : nullptr;
        Group_Add(g, 0, o, 0.0f);
        return;
    }
    PtrList_Rewind(F<PtrList*>(g, 8));
    uint32_t a = 0, b = 0, c = 0;
    uint8_t* old = (uint8_t*)PtrList_Next(F<PtrList*>(g, 8));
    PtrList_Clear(F<PtrList*>(g, 8));
    PtrList_Rewind(F<PtrList*>(g, 8));
    SceneObj_GetAngles(old, 0, &a, &b, &c);
    SceneObj_SetAngles(o, 0, a, b, c);
    SceneObj_GetPos(old, 0, &a, &b, &c);
    if (old) {
        SceneObj_Destructor(old);
        crt::operator_delete(old);
    }
    // The position goes on as raw bits (the original pushes the dwords).
    ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, uint32_t))&Obj_MoveTo)(o, 0, a, b, c);
    PtrList_Append(F<PtrList*>(g, 8), 0, o);
}
RT3_REPLACE(0x53a5d0, Group_SetObject);

// 0x53a6d0 (vtable). Destroys o's object list (+8) and every object in it.
void __fastcall Obj_DestroyList8(uint8_t* o) {
    if (!F<PtrList*>(o, 8)) return;
    PtrList_Rewind(F<PtrList*>(o, 8));
    for (uint8_t* p = (uint8_t*)PtrList_Next(F<PtrList*>(o, 8)); p; p = (uint8_t*)PtrList_Next(F<PtrList*>(o, 8))) {
        SceneObj_Destructor(p);
        crt::operator_delete(p);
    }
    PtrList_Clear(F<PtrList*>(o, 8));
    if (PtrList* l = F<PtrList*>(o, 8)) {
        PtrList_Destroy(l);
        crt::operator_delete(l);
    }
    F<PtrList*>(o, 8) = nullptr;
}
RT3_REPLACE(0x53a6d0, Obj_DestroyList8);

// 0x53a740. Sets the piece's end points (any of the four corner vectors may be
// null) and, unless the engine is loading (+0x114256) or there is no terrain (+4),
// rebuilds its geometry and moves its terrain-cell entry (+0x55, 16-tile cells)
// when its centre changed cell. Then sets the mid heights +0x1ed / +0x1f1.
void __fastcall Track_SetEnds(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d) {
    const uint32_t* v[4] = { a, b, c, d };
    for (int k = 0; k < 4; ++k)
        if (v[k])
            for (int j = 0; j < 3; ++j) F<uint32_t>(e, 0x14 + k * 0xc + j * 4) = v[k][j];
    if (!F<int32_t>(g_engine, 0x114256) && F<uint8_t*>(e, 4)) {
        const double k = Fl(0x5dcfe8);
        int32_t x0 = Ftol(F<float>(e, 0xc) * k) >> 4;
        int32_t y0 = Ftol(F<float>(e, 0x10) * k) >> 4;
        Track_BuildGeometry(e);
        int32_t x1 = Ftol(F<float>(e, 0xc) * k) >> 4;
        int32_t y1 = Ftol(F<float>(e, 0x10) * k) >> 4;
        if (x0 != x1 || y0 != y1 || !F<PtrListNode*>(e, 0x55)) {
            if (F<PtrListNode*>(e, 0x55)) Terrain_Remove1675(F<uint8_t*>(e, 4), 0, F<PtrListNode*>(e, 0x55), x0, y0);
            F<PtrListNode*>(e, 0x55) = Terrain_Add1675(F<uint8_t*>(e, 4), 0, e, x1, y1);
        }
    }
    F<float>(e, 0x1ed) = (float)(((double)F<float>(e, 0x3c) + F<float>(e, 0x30)) * Fl(0x5c8d48));
    F<float>(e, 0x1f1) = (float)(((double)F<float>(e, 0x24) + F<float>(e, 0x18)) * Fl(0x5c8d48));
}
RT3_REPLACE(0x53a740, Track_SetEnds);

// 0x53a890. Puts the four corners `lift` above the terrain, levels each end to its
// higher corner and rebuilds the geometry.
void __fastcall Track_DropOnTerrain(uint8_t* e, int, float lift) {
    for (int k = 0; k < 4; ++k) {
        float* p = &F<float>(e, 0x14 + k * 0xc);
        p[1] = (float)(Terrain_HeightAt(F<uint8_t*>(e, 4), 0, p[0], p[2]) + lift);
    }
    float hi1 = F<float>(e, 0x18) > F<float>(e, 0x24) ? F<float>(e, 0x18) : F<float>(e, 0x24);
    F<float>(e, 0x24) = hi1;
    F<float>(e, 0x18) = hi1;
    float hi2 = F<float>(e, 0x30) > F<float>(e, 0x3c) ? F<float>(e, 0x30) : F<float>(e, 0x3c);
    F<float>(e, 0x3c) = hi2;
    F<float>(e, 0x30) = hi2;
    F<float>(e, 0x1ed) = (float)(((double)hi2 + hi2) * Fl(0x5c8d48));
    F<float>(e, 0x1f1) = (float)(((double)hi1 + hi1) * Fl(0x5c8d48));
    Track_BuildGeometry(e);
}
RT3_REPLACE(0x53a890, Track_DropOnTerrain);

// 0x53a920. Sets all four corners and both mid heights to h and rebuilds.
void __fastcall Track_SetFlatHeight(uint8_t* e, int, uint32_t h) {
    for (int k = 0; k < 4; ++k) F<uint32_t>(e, 0x18 + k * 0xc) = h;
    F<uint32_t>(e, 0x1f1) = h;
    F<uint32_t>(e, 0x1ed) = h;
    Track_BuildGeometry(e);
}
RT3_REPLACE(0x53a920, Track_SetFlatHeight);

// 0x53a960. Draws a floating 3D label (needs the overlay font, engine
// +0x1141f5). vtbl +0x10 fills the text (one or two lines, a colour and a fog
// bias); it floats 15 (+20 for text over 2 characters) above half the object's
// height (+0x18 + +0x30), at least 50 above the ground when vtbl +0x14 says so,
// projected at (+0xc, +0x10) and drawn only on screen, faded by depth and
// scaled by the fog at (+0x63, +0x6b). Two lines go either side of the point
// (the first right-aligned in the label colour, the second left in white);
// one line is centred, at half size when longer than 2 characters.
void __fastcall SceneLabel_Draw(uint8_t* o) {
    uint8_t* e = g_engine;
    if (!e || !F<uint32_t>(e, 0x1141f5)) return;
    char line1[0x130], line2[0x130];
    int32_t color = -1;
    float bias = 0.0f;
    int32_t lines = ((int32_t(__thiscall*)(uint8_t*, char*, int32_t, int32_t*, float*, char*, int32_t))(*(void***)o)[0x10 / 4])(
        o, line1, 0x12c, &color, &bias, line2, 0x12c);
    if (!lines) return;
    int32_t wide = strlen(line1) > 2 ? 1 : 0;
    float y = (float)(((double)F<float>(o, 0x30) + F<float>(o, 0x18)) * D(0x5c8628) + (double)(wide ? 0x14 : 0) + D(0x5c8798));
    if (((int32_t(__thiscall*)(uint8_t*))(*(void***)o)[0x14 / 4])(o)) {
        double ground = Terrain_HeightAt((uint8_t*)Engine_Get1C(g_engine), 0, F<float>(o, 0xc), F<float>(o, 0x10)) + Fl(0x5c8780);
        if (!(y > ground)) y = (float)ground;
    }
    float sx, sy, depth;
    if (!Scene_ProjectNormalised((uint8_t*)Engine_Renderer(g_engine), 0, F<float>(o, 0xc), y, F<float>(o, 0x10), &sx, &sy, &depth))
        return;
    if (sx < Fl(0x5c8598) || !(sx <= Fl(0x5c85b4))) return;
    if (sy < Fl(0x5c8598) || !(sy <= Fl(0x5c85b4))) return;
    Fade_Save();
    Fade_Set(*(uint32_t*)&depth);
    int32_t fx = Ftol(F<float>(o, 0x63));
    int32_t fz = Ftol(F<float>(o, 0x6b));
    float scale, strength;
    uint8_t* font;
    Fog_ParamsAt((uint8_t*)Engine_Get1C(g_engine), 0, fx, fz, bias, &scale, (uint32_t*)&font, &strength);
    if (lines == 2) {
        float gap = (float)((double)strength * D(0x5caae0));
        Font_DrawTextBox(font, 0, line1, (float)((double)sx - D(0x5c88c8) - gap), (float)((double)sy - D(0x5c86b8)), BitsF(0x3e99999a), BitsF(0x3e4ccccd),
                         (uint32_t)color, 3, 0, 0, scale);
        Font_DrawTextBox(font, 0, line2, (float)((double)gap + sx), (float)((double)sy - D(0x5c86b8)), BitsF(0x3e99999a), BitsF(0x3e4ccccd), 0xffffffff, 2, 0,
                         0, scale);
    } else {
        if (wide) {
            scale = 0.5f;
            F<int32_t>(F<uint8_t*>(font, 0x18), 0x18) = 1;
        }
        Font_DrawTextBox(font, 0, line1, (float)((double)sx - D(0x5c88c8)), (float)((double)sy - D(0x5c86b8)), BitsF(0x3f19999a), BitsF(0x3e4ccccd),
                         (uint32_t)color, 0, 0, 0, scale);
    }
    Fade_Restore();
}
RT3_REPLACE(0x53a960, SceneLabel_Draw);

// 0x53ac40. Fixes a piece up after loading: drops its stale side-quad pointer
// (+0x1e3, not freed), terrain (+4) and cell entry (+0x55), takes the engine's
// terrain, re-applies its end points and rebuilds the geometry if it had side quads.
int32_t __fastcall Track_AfterLoad(uint8_t* e, int, int32_t) {
    bool quads = F<void*>(e, 0x1e3) != nullptr;
    F<void*>(e, 0x1e3) = nullptr;
    F<void*>(e, 4) = nullptr;
    F<void*>(e, 0x55) = nullptr;
    F<void*>(e, 4) = Engine_Get1C(hook::As<void*>(g_engine));
    Track_SetEnds(e, 0, &F<uint32_t>(e, 0x14), &F<uint32_t>(e, 0x20), &F<uint32_t>(e, 0x2c), &F<uint32_t>(e, 0x38));
    F<int32_t>(e, 8) = 0;
    if (quads) Track_BuildGeometry(e);
    return 0;
}
RT3_REPLACE(0x53ac40, Track_AfterLoad);

// 0x53aca0. Constructs a light (vtable 0x5dd3d8) with its default colours.
uint8_t* __fastcall Light_Construct(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5dd3d8;
    F<uint32_t>(o, 4) = 0;
    F<uint32_t>(o, 0x5b) = 0;
    o[0x5a] = 0xff;
    F<uint32_t>(o, 0x1e3) = 0;
    F<uint32_t>(o, 0x5f) = 0xff7f7f7f;
    F<uint32_t>(o, 0x1e7) = 0;
    F<uint16_t>(o, 0x1eb) = 0;
    for (int k = 0; k < 16; ++k) F<uint32_t>(o, 0x6f + k * 0x18) = 0xff7f7f7f;
    F<uint32_t>(o, 0x4d) = 0x41c80000;
    F<uint32_t>(o, 0x45) = 0x41c80000;
    F<uint32_t>(o, 0x49) = 0x41000000;
    F<uint32_t>(o, 0x51) = 0x41000000;
    F<uint32_t>(o, 0xeb) = 0x3f266666;
    F<uint32_t>(o, 0x8b) = 0x3f266666;
    o[0x44] = 0;
    static const uint16_t halves[] = { 0xd3, 0x73, 0xbf, 0xa7, 0x8f, 0x77 };
    for (uint16_t off : halves) F<uint32_t>(o, off) = 0x3f000000;
    F<uint32_t>(o, 0x103) = 0x3f59999a;
    F<uint32_t>(o, 0xa3) = 0x3f59999a;
    static const uint16_t ones[] = { 0x11b, 0xbb, 0x11f, 0x107, 0xef, 0xd7 };
    for (uint16_t off : ones) F<uint32_t>(o, off) = 0x3f800000;
    F<uint32_t>(o, 0x1fd) = g_624dd4;
    F<uint32_t>(o, 0x17b) = g_624d88;
    F<uint32_t>(o, 0x17f) = g_624d8c;
    F<uint32_t>(o, 0x163) = g_624d90;
    F<uint32_t>(o, 0x167) = g_624d8c;
    F<uint32_t>(o, 0x133) = g_624d90;
    F<uint32_t>(o, 0x137) = g_624d94;
    F<uint32_t>(o, 0x14b) = g_624d88;
    F<uint32_t>(o, 0x14f) = g_624d94;
    uint8_t f = (uint8_t)((o[0x59] & 0xf) | 0x10);
    F<uint32_t>(o, 0xc) = 0;
    F<uint32_t>(o, 0x10) = 0;
    F<uint32_t>(o, 0x1f1) = 0;
    F<uint32_t>(o, 0x1ed) = 0;
    o[0x59] = f;
    F<uint32_t>(o, 8) = 0;
    F<uint32_t>(o, 0x55) = 0;
    return o;
}
RT3_REPLACE(0x53aca0, Light_Construct);

// 0x53ae10. Sets a piece up on `terrain` with style `style`, at the given corners.
void __fastcall Track_Setup(uint8_t* e, int, const uint32_t* a, const uint32_t* b, const uint32_t* c, const uint32_t* d,
                            void* terrain, uint8_t style) {
    F<void*>(e, 4) = terrain;
    Effect_SetStyle(e, 0, style, 0);
    F<int32_t>(e, 0xc) = 0;
    F<int32_t>(e, 0x10) = 0;
    F<void*>(e, 0x55) = nullptr;
    Track_SetEnds(e, 0, a, b, c, d);
    e[0x59] &= 0xf0;
    F<uint32_t>(e, 0x5f) = 0xff7f7f7f;
}
RT3_REPLACE(0x53ae10, Track_Setup);

// 0x53ae70. The debug allocator's leak report: for every still-live tracked block
// (0xccba30 pointers, 0xccba2c 0x30-byte records: size, file, line +0x19, last
// resource +0x1b), newest first, prints "Memory Leak #n from file ..." to the
// debugger and the debug log (numbered by 0xccba4c), then "Memory Leaked = N"
// (total 0xccba3c).
void Mem_ReportLeaks() {
    auto out = imp::OutputDebugStringA;
    const int32_t none = -1234;
    for (int32_t i = g_ccba34 - 1; i >= 0; --i) {
        void* p = hook::As<void**>(g_ccba30)[i];
        if (!p) continue;
        uint8_t* r = hook::As<uint8_t*>(g_ccba2c) + i * 0x30;
        int32_t n = ++g_ccba4c;
        char line[0x200];
        crt::sprintf(line, (const char*)0x5dd408, n, (const char*)(r + 4), (int32_t)F<uint16_t>(r, 0x19), F<int32_t>(r, 0), p,
                     (const char*)(r + 0x1b));
        DebugLog(line, none, none, none, none, none, none, none, none, none);
        out(line);
        out((const char*)0x5c9ce8);
    }
    char num[0x20], text[0x100];
    Num_Format(num, 8, (double)g_ccba3c, 10, 1, 0, 0);
    FormatText(text, (const char*)0x5dd3f0, num, 0, 0, 0, 0, 0, 0, 0);
    out(text);
    DebugLog(text, none, none, none, none, none, none, none, none, none);
}
RT3_REPLACE(0x53ae70, Mem_ReportLeaks);

RT3_NOINLINE void __fastcall FreeAndNull(void** p) {
    if (*p) {
        crt::free(*p);
        *p = nullptr;
    }
}
RT3_REPLACE(0x53afd0, FreeAndNull);

RT3_NOINLINE void __fastcall FreeAndNull2(void** p) {
    if (!*p) return;
    crt::free(*p);
    *p = nullptr;
}
RT3_REPLACE(0x53aff0, FreeAndNull2);

void __fastcall Set_ccba28() { g_ccba28 = 1; }
RT3_REPLACE(0x53b010, Set_ccba28);

// 0x53b020. Shuts the debug allocator down: prints the leak report, frees its
// record and pointer tables and clears its counters.
void Mem_Shutdown() {
    Mem_ReportLeaks();
    if (void* r = g_ccba2c) crt::free(r);
    g_ccba2c = nullptr;
    if (void* p = g_ccba30) crt::free(p);
    g_ccba30 = nullptr;
    g_ccba34 = 0;
    g_ccba28 = 0;
}
RT3_REPLACE(0x53b020, Mem_Shutdown);

// 0x53b090. Opens the next animation pack (up to 999; the file stays open, fd in
// +0x204ac) and reads its 13-byte frame records (count +0x417ec, record size
// +0x1f404: 13 before v1003, else 0x4a with a 0x3d-byte name each, ".dsc" names
// registered with 0xccba50), sorted into 32 buckets by the low 5 bits of the
// first byte (+0 pointers, +0x223ec counts).
RT3_NOINLINE void __fastcall Anims_LoadPack(uint8_t* o, int, const char* path, int32_t) {
    int32_t& cur = F<int32_t>(o, 0x427b8);
    if (cur >= 0x3e7) return;
    int32_t fd = crt::open(path, 0x8000, 0);
    if (fd == -1) Fatal_Exit(GetString(0x60));
    F<int32_t>(o, 0x204ac + cur * 4) = fd;
    int32_t ver = 0, n;
    crt::read(F<int32_t>(o, 0x204ac + cur * 4), &ver, 4);
    if (ver < 0x3e9) Fatal_Exit(GetString(0x61));
    crt::read(F<int32_t>(o, 0x204ac + cur * 4), &n, 4);
    uint8_t* buf = (uint8_t*)crt::malloc(n * 0xd);
    if (ver < 0x3eb) {
        F<int32_t>(o, 0x1f404 + cur * 4) = 0xd;
        crt::read(F<int32_t>(o, 0x204ac + cur * 4), buf, n * 0xd);
    } else {
        F<int32_t>(o, 0x1f404 + cur * 4) = 0x4a;
        for (int32_t i = 0; i < n; ++i) {
            char name[0x3d];
            crt::read(F<int32_t>(o, 0x204ac + cur * 4), buf + i * 0xd, 0xd);
            crt::read(F<int32_t>(o, 0x204ac + cur * 4), name, 0x3d);
            const char* dot = ((const char*)crt::strchr(name, '.'));
            if (dot && !crt::stricmp(dot, (const char*)0x5dd56c)) {
                void* reg = hook::As<void*>(g_ccba50);
                (*(void(__thiscall**)(void*, const char*, int32_t, int32_t, int32_t))(*(uint8_t**)reg + 4))(reg, name, 0, 0, 0);
            }
        }
    }
    F<int32_t>(o, 0x417ec + cur * 4) = n;
    int32_t fill[32] = {};
    for (int32_t k = 0; k < 32; ++k) F<int32_t>(o, 0x223ec + (cur * 32 + k) * 4) = 0;
    for (int32_t i = 0; i < n; ++i) ++F<int32_t>(o, 0x223ec + (cur * 32 + (buf[i * 0xd] & 0x1f)) * 4);
    for (int32_t k = 0; k < 32; ++k) {
        int32_t c = F<int32_t>(o, 0x223ec + (cur * 32 + k) * 4);
        F<void*>(o, (cur * 32 + k) * 4) = crt::malloc((c < 1 ? 1 : c) * 0xd);
    }
    for (int32_t i = 0; i < n; ++i) {
        int32_t k = *(uint32_t*)(buf + i * 0xd) & 0x1f;
        memcpy(F<uint8_t*>(o, (cur * 32 + k) * 4) + fill[k] * 0xd, buf + i * 0xd, 0xd);
        ++fill[k];
    }
    crt::free(buf);
    ++cur;
}
RT3_REPLACE(0x53b090, Anims_LoadPack);

// 0x53b390. Locates name id in the pack name table: in a hash bucket (+0x203a4,
// 32 by id & 31) -> 2 with *pack = -1 - position; in the 10-entry lookup cache
// (+0x4290c) or the packs' sorted lists (+0x427b8 packs, lists +0 / counts +0x223ec
// per bucket, 13-byte entries) -> 1 with (pack, bucket, index), cached. Else 0,
// fatal (string 0x62, table +0x427c0) when asked.
RT3_NOINLINE int32_t __fastcall NameTable_Locate(uint8_t* t, int, int32_t id, int32_t* pack, int32_t* bucket, int32_t* index,
                                    int32_t fatal) {
    int32_t b = id & 0x1f;
    int32_t pos = F<int32_t>(t, 0x2042c + b * 4);
    for (uint8_t* e = F<uint8_t*>(t, 0x203a4 + b * 4); e; e = F<uint8_t*>(e, 0x14), ++pos)
        if (id == F<int32_t>(e, 0)) {
            *index = 0;
            *bucket = 0;
            *pack = -1 - pos;
            return 2;
        }
    for (int32_t i = 0; i < 10; ++i) {
        int32_t c = F<int32_t>(t, 0x4290c + i * 4);
        if (c == id && c != -1) {
            *pack = F<int32_t>(t, 0x42934 + i * 4);
            *bucket = F<int32_t>(t, 0x4295c + i * 4);
            *index = F<int32_t>(t, 0x42984 + i * 4);
            return 1;
        }
    }
    for (int32_t p = 0; p < F<int32_t>(t, 0x427b8); ++p) {
        uint8_t* slot = t + b * 4 + p * 0x80;
        for (int32_t k = F<int32_t>(slot, 0x223ec) - 1; k >= 0; --k) {
            if (F<int32_t>(F<uint8_t*>(slot, 0) + k * 0xd, 0) != id) continue;
            *pack = p;
            *bucket = b;
            *index = k;
            int32_t s = F<int32_t>(t, 0x429ac);
            F<int32_t>(t, 0x4290c + s * 4) = id;
            F<int32_t>(t, 0x42934 + s * 4) = p;
            F<int32_t>(t, 0x4295c + s * 4) = b;
            F<int32_t>(t, 0x42984 + s * 4) = k;
            F<int32_t>(t, 0x429ac) = s == 9 ? 0 : s + 1;
            return 1;
        }
    }
    if (fatal) {
        char msg[0x100];
        FormatText(msg, GetString(0x62), (const char*)t + 0x427c0, nullptr, nullptr, nullptr, nullptr, nullptr,
                   nullptr, nullptr);
        Fatal_Exit(msg);
    }
    return 0;
}
RT3_REPLACE(0x53b390, NameTable_Locate);

// 0x53b590. Whether name id is in the pack name table (NameTable_Locate's result).
int32_t __fastcall NameTable_Contains(uint8_t* t, int, int32_t id, int32_t fatal) {
    int32_t pack, bucket, index;
    return NameTable_Locate(t, 0, id, &pack, &bucket, &index, fatal);
}
RT3_REPLACE(0x53b590, NameTable_Contains);

// 0x53b5c0. Copies the name to +0x427c0 and returns its case-insensitive hash
// (h = upper(c) + h * 0x41c64e6e + (h >> 16) + 0x3039, from 7).
RT3_NOINLINE int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name) {
    char* dst = (char*)(t + 0x427c0);
    int32_t k = 0;
    while ((dst[k] = name[k]) != 0) ++k;
    int32_t len = 0;
    while (name[len]) ++len;
    int32_t h = 7;
    for (int32_t i = 0; i < len; ++i) {
        int32_t u = (int8_t)ToUpper(name[i]);
        h = (int32_t)((uint32_t)u + (uint32_t)h * 0x41c64e6eu + (uint32_t)(h >> 16) + 0x3039u);
    }
    return h;
}
RT3_REPLACE(0x53b5c0, NameTable_Hash);

// 0x53b630. The n-th entry (1-based) across the 32 hash buckets (+0x203a4, next
// +0x14), with the last position cached (+0x427b0 / +0x20424) for sequential walks.
RT3_NOINLINE uint8_t* __fastcall NameTable_Nth(uint8_t* t, int, int32_t n) {
    if (n == F<int32_t>(t, 0x427b0) + 1) return F<uint8_t*>(t, 0x20424);
    int32_t left = n;
    for (int32_t i = 0; i < 0x20; ++i) {
        uint8_t* e = F<uint8_t*>(t, 0x203a4 + i * 4);
        if (!left) continue;
        while (e) {
            if (!--left) {
                F<int32_t>(t, 0x427b0) = n - 1;
                F<uint8_t*>(t, 0x20424) = e;
                return e;
            }
            e = F<uint8_t*>(e, 0x14);
        }
    }
    return nullptr;
}
RT3_REPLACE(0x53b630, NameTable_Nth);

// 0x53b690. Closes the current entry's file (handle +4 or FILE* +8).
RT3_NOINLINE void __fastcall NameTable_CloseCurrent(uint8_t* t) {
    uint8_t* e = NameTable_Nth(t, 0, -F<int32_t>(t, 0x427bc));
    if (F<int32_t>(e, 4) == -1) return;
    if (!F<void*>(e, 8)) crt::close(F<int32_t>(e, 4));
    else crt::fclose(F<void*>(e, 8));
    F<int32_t>(e, 4) = -1;
    F<void*>(e, 8) = nullptr;
}
RT3_REPLACE(0x53b690, NameTable_CloseCurrent);

// 0x53b6f0. Opens the current pack's file (entry -(+0x427bc), name +0x10, fd +4,
// position +8 = 0); fatal (string 0x63, with the working directory) on failure.
RT3_NOINLINE int32_t __fastcall NameTable_OpenPack(uint8_t* t) {
    char path[0x12c];
    path[0] = 0;
    uint8_t* e = NameTable_Nth(t, 0, -F<int32_t>(t, 0x427bc));
    const char* name = F<const char*>(e, 0x10);
    char* d = path;
    while ((*d++ = *name++) != 0) {}
    int32_t fd = crt::open(path, 0x8000);
    F<int32_t>(e, 4) = fd;
    F<int32_t>(e, 8) = 0;
    if (fd == -1) {
        char cwd[0x100], msg[0x100];
        imp::GetCurrentDirectoryA(0x100, cwd);
        FormatText(msg, GetString(0x63), path, cwd, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        Fatal_Exit(msg);
    }
    return F<int32_t>(e, 4);
}
RT3_REPLACE(0x53b6f0, NameTable_OpenPack);

// 0x53b7c0. Reads n bytes from the current pack (+0x427bc: < 0 = opened entry,
// else fd table +0x204ac), counting bytes (+0x428ec) and reads (+0x428f0); fatal
// (string 0x64) on a short read.
RT3_NOINLINE int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n) {
    int32_t k = F<int32_t>(t, 0x427bc);
    int32_t fd = k < 0 ? F<int32_t>(NameTable_Nth(t, 0, -k), 4) : F<int32_t>(t, 0x204ac + k * 4);
    int32_t r = crt::read(fd, dst, n);
    F<int32_t>(t, 0x428ec) += n;
    F<int32_t>(t, 0x428f0) += 1;
    if (r != n) Fatal_Exit(GetString(0x64));
    return r;
}
RT3_REPLACE(0x53b7c0, NameTable_Read);

// 0x53b830. Skips a dword of the current pack.
int32_t __fastcall NameTable_Skip4(uint8_t* t, int, int32_t) {
    int32_t v;
    return NameTable_Read(t, 0, &v, 4);
}
RT3_REPLACE(0x53b830, NameTable_Skip4);

// 0x53b850. Grows the 13-byte record array +0x1f400 by 100 (capacity +0x427b4).
RT3_NOINLINE void __fastcall NameTable_Grow(uint8_t* t) {
    uint32_t cap = F<uint32_t>(t, 0x427b4);
    uint8_t* p = (uint8_t*)crt::malloc((cap + 100) * 13);
    for (uint32_t k = 0; k < 100 * 13; ++k) p[cap * 13 + k] = 0;
    if (uint8_t* old = F<uint8_t*>(t, 0x1f400)) {
        uint32_t n = F<uint32_t>(t, 0x427b4) * 13;
        for (uint32_t k = 0; k < n; ++k) p[k] = old[k];
        crt::free(F<void*>(t, 0x1f400));
    }
    F<uint8_t*>(t, 0x1f400) = p;
    F<uint32_t>(t, 0x427b4) += 100;
}
RT3_REPLACE(0x53b850, NameTable_Grow);

// 0x53b8d0. Recreates every loaded font (kind 7), e.g. after a mode change.
void __fastcall Pack_RecreateFonts(uint8_t* pack) {
    for (int32_t i = 0; i < F<int32_t>(pack, 0x427b4); ++i) {
        uint8_t* e = F<uint8_t*>(pack, 0x1f400) + i * 0xd;
        if (F<uint32_t>(e, 0) && e[8] == 7) Font_Create(F<uint8_t*>(e, 9));
    }
}
RT3_REPLACE(0x53b8d0, Pack_RecreateFonts);

// 0x53b920. Releases the handles of every loaded font (kind 7), e.g. before a mode
// change.
RT3_NOINLINE void __fastcall Pack_ReleaseFontHandles(uint8_t* pack) {
    for (int32_t i = 0; i < F<int32_t>(pack, 0x427b4); ++i) {
        uint8_t* e = F<uint8_t*>(pack, 0x1f400) + i * 0xd;
        if (F<uint32_t>(e, 0) && e[8] == 7) Font_ReleaseHandles(F<uint8_t*>(e, 9));
    }
}
RT3_REPLACE(0x53b920, Pack_ReleaseFontHandles);

// 0x53b970. Closes the current file and frees every entry.
RT3_NOINLINE void __fastcall NameTable_Clear(uint8_t* t) {
    if (F<int32_t>(t, 0x427bc) < 0) NameTable_CloseCurrent(t);
    for (int b = 0; b < 0x20; ++b) {
        uint8_t** head = (uint8_t**)(t + 0x203a4 + b * 4);
        while (*head) {
            uint8_t* e = *head;
            *head = F<uint8_t*>(e, 0x14);
            crt::operator_delete(F<void*>(e, 0x10));
            crt::operator_delete(e);
        }
    }
    F<int32_t>(t, 0x20424) = 0;
    F<int32_t>(t, 0x427b0) = -1;
}
RT3_REPLACE(0x53b970, NameTable_Clear);

// 0x53b9e0. Shuts the resource pack system t down: clears the name table, destroys
// its three side tables (+0x429b0..) and the global one (0xccba50), reports every
// item still in use (table +0x1f400, 13-byte entries {uses, id, type, ptr}) as
// "Pack Item leaked #:... id:... type:... usedCount:... ptr:..." to the debugger and
// the debug log (the second field is the item's index from NameTable_Locate; when
// that fails the original prints whatever the variable last held, so this keeps
// the value across items too), frees every pack's 32 bucket arrays and closes its
// file, frees the item table and reports "Pack System Leak Count : N".
RT3_NOINLINE void __fastcall Pack_Shutdown(uint8_t* t) {
    auto out = imp::OutputDebugStringA;
    const int32_t none = -1234;
    NameTable_Clear(t);
    for (uint32_t off = 0x429b0; off <= 0x429b8; off += 4) {
        if (Collection* c = F<Collection*>(t, off)) {
            Collection_Destruct(c);
            crt::operator_delete(c);
        }
        F<void*>(t, off) = nullptr;
    }
    if (Collection* c = g_ccba50) {
        Collection_Destruct(c);
        crt::operator_delete(c);
    }
    hook::As<void*>(g_ccba50) = nullptr;
    int32_t leaks = 0, pack = 0, bucket = 0, index = 0;
    for (int32_t i = 0; i < F<int32_t>(t, 0x427b4); ++i) {
        uint8_t* e = F<uint8_t*>(t, 0x1f400) + i * 0xd;
        if (!F<uint32_t>(e, 0)) continue;
        ++leaks;
        NameTable_Locate(t, 0, F<int32_t>(e, 4), &pack, &bucket, &index, 0);
        e = F<uint8_t*>(t, 0x1f400) + i * 0xd;
        char ptr[0x130], uses[0x134], type[0x130], id[0x130], idx[0x130], line[0x200];
        Num_Format(ptr, 8, (double)F<int32_t>(e, 9), 10, 1, 0, 0);
        Num_Format(uses, 8, (double)F<uint32_t>(e, 0), 10, 1, 0, 0);
        Num_Format(type, 8, (double)e[8], 10, 1, 0, 0);
        Num_Format(id, 8, (double)F<uint32_t>(e, 4), 10, 1, 0, 0);
        Num_Format(idx, 8, (double)index, 10, 1, 0, 0);
        FormatText(line, (const char*)0x5dd598, (const char*)0x5c87a8, idx, id, type, uses, ptr, 0, 0);
        DebugLog(line, none, none, none, none, none, none, none, none, none);
        out(line);
        out((const char*)0x5c9ce8);
    }
    for (int32_t p = 0; p < F<int32_t>(t, 0x427b8); ++p) {
        for (int k = 0; k < 0x20; ++k) FreeAndNull((void**)(t + (p * 0x20 + k) * 4));
        int32_t& fd = F<int32_t>(t, 0x204ac + p * 4);
        void*& f = F<void*>(t, 0x2144c + p * 4);
        if (fd != -1) {
            if (!f) crt::close(fd);
            else crt::fclose(f);
            fd = -1;
            f = nullptr;
        }
    }
    crt::free(F<void*>(t, 0x1f400));
    char num[0x20], line[0x200];
    F<int32_t>(t, 0x427b8) = 0;
    Num_Format(num, 8, (double)leaks, 10, 1, 0, 0);
    FormatText(line, (const char*)0x5dd574, num, 0, 0, 0, 0, 0, 0, 0);
    DebugLog(line, none, none, none, none, none, none, none, none, none);
    out(line);
}
RT3_REPLACE(0x53b9e0, Pack_Shutdown);

// 0x53bd40. Positions the pack file at byte `offset` of name id (making its pack
// current, +0x427bc; counting seeks +0x428f4); loose files are opened instead.
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset) {
    if (F<int32_t>(t, 0x427bc) < 0) NameTable_CloseCurrent(t);
    int32_t pack, bucket, index;
    NameTable_Locate(t, 0, id, &pack, &bucket, &index, 1);
    F<int32_t>(t, 0x427bc) = pack;
    if (pack >= 0) {
        ++F<int32_t>(t, 0x428f4);
        return Lseek(F<int32_t>(t, 0x204ac + pack * 4), EntryPos(t, pack, bucket, index) + offset, 0);
    }
    int32_t fd = NameTable_OpenPack(t);
    if (offset) return Lseek(fd, offset, 0);
    return 0;
}
RT3_REPLACE(0x53bd40, NameTable_Seek);

// 0x53be10. A stdio stream on name id ("r" when text, else "rb"; one cached per
// pack, +0x2144c), positioned at its data; 0 when the open failed for binary.
FILE* __fastcall NameTable_OpenFile(uint8_t* t, int, int32_t id, uint8_t text) {
    if (F<int32_t>(t, 0x427bc) < 0) NameTable_CloseCurrent(t);
    int32_t pack, bucket, index;
    NameTable_Locate(t, 0, id, &pack, &bucket, &index, 1);
    F<int32_t>(t, 0x427bc) = pack;
    if (pack < 0) {
        NameTable_OpenPack(t);
        uint8_t* e = NameTable_Nth(t, 0, -pack);
        if (!F<FILE*>(e, 8)) F<FILE*>(e, 8) = Fdopen(F<int32_t>(e, 4), (const char*)0x5c9678);
        return F<FILE*>(e, 8);
    }
    ++F<int32_t>(t, 0x428f4);
    Lseek(F<int32_t>(t, 0x204ac + pack * 4), EntryPos(t, pack, bucket, index), 0);
    int32_t p = F<int32_t>(t, 0x427bc);
    if (!F<FILE*>(t, 0x2144c + p * 4))
        F<FILE*>(t, 0x2144c + F<int32_t>(t, 0x427bc) * 4) =
            Fdopen(F<int32_t>(t, 0x204ac + p * 4), (const char*)(text ? 0x5c89e8 : 0x5c9678));
    if (*crt::errno_() && !text) return nullptr;
    return F<FILE*>(t, 0x2144c + F<int32_t>(t, 0x427bc) * 4);
}
RT3_REPLACE(0x53be10, NameTable_OpenFile);

// 0x53bf60. Frees pack resource slot `idx` (table +0x1f400, 13 bytes: refcount
// +0, kind +8, object +9): by kind the object is destroyed (1 model set, 2..4
// texture, 6 sound (0x53ee60), 7 texture handles; 5 is left alone) and deleted;
// the slot is cleared.
void __fastcall Pack_FreeSlot(uint8_t* pack, int, int32_t idx) {
    uint8_t* e = F<uint8_t*>(pack, 0x1f400) + idx * 0xd;
    uint8_t* obj = F<uint8_t*>(e, 9);
    if (obj) {
        bool del = true;
        switch (e[8]) {
        case 1: ModelSet_FreeThunk(obj); break;
        case 2:
        case 3:
        case 4: Texture_Destroy(obj); break;
        case 6: BitmapFont_Destroy(obj); break;
        case 7: Texture_ReleaseHandles(obj); break;
        default: del = false; break;
        }
        if (del) crt::operator_delete(obj);
    }
    e = F<uint8_t*>(pack, 0x1f400) + idx * 0xd;
    F<uint32_t>(e, 0) = 0;
    F<uint32_t>(e, 4) = 0;
    F<uint32_t>(e, 8) = 0;
    e[0xc] = 0;
}
RT3_REPLACE(0x53bf60, Pack_FreeSlot);

// 0x53c000. Drops one reference to pack resource `obj` (by its object pointer in
// the first +0x427b4 slots); at zero the slot is freed unless its kind is kept
// loaded (+0x4278c + kind * 4).
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj) {
    if (!obj) return;
    for (int32_t i = 0; i < F<int32_t>(pack, 0x427b4); ++i) {
        uint8_t* e = F<uint8_t*>(pack, 0x1f400) + i * 0xd;
        if (F<void*>(e, 9) != obj) continue;
        --F<uint32_t>(e, 0);
        if (F<uint32_t>(e, 0) > 0) return;
        if (F<int32_t>(pack, 0x4278c + e[8] * 4)) return;
        Pack_FreeSlot(pack, 0, i);
        return;
    }
}
RT3_REPLACE(0x53c000, Pack_Release);

// 0x53c070. Takes a reference to pack resource `key`, loading it into a free slot
// (refcount and object both 0, searched from the top; the table grows when full) by
// kind: 1 collision file, 6 bitmap font (0x53ee70), 7 font; other kinds load nothing
// and return 0. (sic) The key match also hits free slots, whose key is 0.
uint8_t* __fastcall Pack_Acquire(uint8_t* pack, int, int32_t kind, int32_t key) {
    for (int32_t i = 0; i < F<int32_t>(pack, 0x427b4); ++i) {
        uint8_t* e = F<uint8_t*>(pack, 0x1f400) + i * 0xd;
        if (F<int32_t>(e, 4) != key) continue;
        ++F<uint32_t>(e, 0);
        return F<uint8_t*>(e, 9);
    }
    int32_t i;
    for (;;) {
        for (i = F<int32_t>(pack, 0x427b4) - 1; i >= 0; --i) {
            uint8_t* e = F<uint8_t*>(pack, 0x1f400) + i * 0xd;
            if (!F<void*>(e, 9) && !F<uint32_t>(e, 0)) break;
        }
        if (i >= 0) break;
        NameTable_Grow(pack);
    }
    F<uint32_t>(F<uint8_t*>(pack, 0x1f400) + i * 0xd, 0) = 1;
    F<int32_t>(F<uint8_t*>(pack, 0x1f400) + i * 0xd, 4) = key;
    F<uint8_t>(F<uint8_t*>(pack, 0x1f400) + i * 0xd, 8) = (uint8_t)kind;
    uint8_t* obj;
    switch (kind) {
    case 1: obj = (uint8_t*)crt::operator_new(0xa); if (obj) obj = CollisionFile_Construct(obj, 0, key); break;
    case 6: obj = (uint8_t*)crt::operator_new(0x3a); if (obj) obj = Imb_Construct(obj, 0, key); break;
    case 7: obj = (uint8_t*)crt::operator_new(0x84); if (obj) obj = Font_Construct(obj, 0, key); break;
    default: return nullptr;
    }
    F<uint8_t*>(F<uint8_t*>(pack, 0x1f400) + i * 0xd, 9) = obj;
    return obj;
}
RT3_REPLACE(0x53c070, Pack_Acquire);

// 0x53c1c0. The texture for name from pack table t's cache (+0x1f400, +0x427b4
// entries of 13 bytes {refs, id, kind, texture}): a name with an extension is
// one image of that kind (.tga 3, .dds 2, .jpg 4, else fatal 0x5c); otherwise
// images name_A.. from mip start (0..7) for count (1..8) are taken, as DDS,
// else JPEG, else TGA (fatal 0x5f), as long as they exist. A cached texture
// gets another reference; otherwise a free entry (the table grows until there is
// one) gets a new texture (small flag +5) with every image loaded.
RT3_NOINLINE uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex) {
    typedef int(__cdecl * SprintfFn)(char*, const char*, ...);
    char paths[8][0x100];
    char msg[0x12c];
    int32_t a = 0, b = 0, c = 0, w = 0, h = 0, n = 0, kind = 0;
    int32_t first = 0;
    const char* dot = ((const char*)crt::strchr(name, '.'));
    if (dot) {
        strcpy(paths[0], name);
        n = 1;
        typedef int(__cdecl * StricmpFn)(const char*, const char*);
        if (!crt::stricmp(dot, (const char*)0x5dd27c)) kind = 3;
        else if (!crt::stricmp(dot, (const char*)0x5dd608)) kind = 2;
        else if (!crt::stricmp(dot, (const char*)0x5dd274)) kind = 4;
        else Fatal_Exit(GetString(0x5c));
        if (!NameTable_Locate(t, 0, NameTable_Hash(t, 0, name), &c, &b, &a, 0)) crt::sprintf(msg, GetString(0x5d), name);
    } else {
        if (count <= 0 || count > 8) count = 8;
        first = start < 0 ? 0 : start > 7 ? 7 : start;
        if (first + count > 8) {
            count = 8 - first;
            if (count < 1) {
                count = 1;
                first = 7;
            }
        }
        int32_t end = count + first;
        kind = 2;
        static const uint32_t fmts[3] = { 0x5dd5fc, 0x5dd5f0, 0x5dd5e4 };
        static const int32_t kinds[3] = { 2, 4, 3 };
        for (int32_t f = 0; f < 3; ++f) {
            kind = kinds[f];
            n = 0;
            bool missing = false;
            for (int32_t i = first; i < end; ++i) {
                char* p = paths[i];
                crt::sprintf(p, (const char*)fmts[f], name, i + 0x41);
                int32_t id = NameTable_Hash(t, 0, p);
                int32_t ok = (i == first && f == 0) ? NameTable_Locate(t, 0, id, &b, &a, &w, 0)
                                                    : NameTable_Locate(t, 0, id, &w, &a, &b, 0);
                if (!ok) {
                    missing = i == first;
                    break;
                }
                ++n;
            }
            if (!missing) break;
            if (f == 2) {
                FormatText(msg, GetString(0x5f), name, 0, 0, 0, 0, 0, 0, 0);
                Fatal_Exit(msg);
            }
        }
    }
    const char* path0 = paths[first];
    int32_t key = NameTable_Hash(t, 0, path0);
    for (int32_t i = 0; i < F<int32_t>(t, 0x427b4); ++i) {
        uint8_t* e = F<uint8_t*>(t, 0x1f400) + i * 0xd;
        if (F<int32_t>(e, 4) == key) {
            ++F<int32_t>(e, 0);
            return F<uint8_t*>(F<uint8_t*>(t, 0x1f400) + i * 0xd, 9);
        }
    }
    int32_t slot;
    for (;;) {
        for (slot = F<int32_t>(t, 0x427b4) - 1; slot >= 0; --slot) {
            uint8_t* e = F<uint8_t*>(t, 0x1f400) + slot * 0xd;
            if (!F<void*>(e, 9) && !F<int32_t>(e, 0)) break;
        }
        if (slot >= 0) break;
        NameTable_Grow(t);
    }
    int32_t off = slot * 0xd;
    F<int32_t>(F<uint8_t*>(t, 0x1f400), off) = 1;
    F<int32_t>(F<uint8_t*>(t, 0x1f400), off + 4) = key;
    F<uint8_t>(F<uint8_t*>(t, 0x1f400), off + 8) = (uint8_t)kind;
    uint8_t* tex = (uint8_t*)crt::operator_new(0xec);
    if (tex) tex = Texture_ConstructDefault(tex);
    tex[5] = smallTex;
    F<uint8_t*>(F<uint8_t*>(t, 0x1f400), off + 9) = tex;
    const char* p = path0;
    for (int32_t i = first; i < n + first; ++i, p += 0x100) {
        int32_t id = NameTable_Hash(t, 0, p);
        if (i == first) {
            if (!NameTable_Locate(t, 0, id, &h, &b, &a, 0)) break;
        } else if (!NameTable_Locate(t, 0, id, &b, &a, &h, 1)) {
            continue;
        }
        Texture_AddAndLoad(tex, 0, id, kind);
    }
    return F<uint8_t*>(F<uint8_t*>(t, 0x1f400) + off, 9);
}
RT3_REPLACE(0x53c1c0, TextureCache_Get);

// 0x53c6f0. Pack_Acquire of font `idx` (kind 7, key 0x754d4c0 + idx).
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx) { return Pack_Acquire(pack, 0, 7, idx + 0x754d4c0); }
RT3_REPLACE(0x53c6f0, Pack_AcquireFont);

// 0x53c710. Registers a file: the path dir + name (0x5c8730) under the hash of
// `name`, with its size (looked up when -1); `counted` bumps the per-bucket
// offsets (+0x2042c) of the later buckets.
RT3_NOINLINE void __fastcall NameTable_Add(uint8_t* t, int, const char* dir, const char* name, int32_t size, int32_t counted) {
    char path[0x1f4];
    FormatText(path, (const char*)0x5c8730, dir, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (size == -1) size = File_Size(path);
    uint8_t* e = (uint8_t*)crt::operator_new(0x18);
    for (int k = 0; k < 6; ++k) F<int32_t>(e, k * 4) = 0;
    F<int32_t>(e, 0) = NameTable_Hash(t, 0, name);
    size_t n = 0;
    while (path[n]) ++n;
    char* copy = (char*)crt::operator_new(n + 1);
    F<char*>(e, 0x10) = copy;
    F<int32_t>(e, 0xc) = size;
    F<int32_t>(e, 4) = -1;
    uint32_t b = F<uint32_t>(e, 0) & 0x1f;
    for (size_t k = 0; (copy[k] = path[k]) != 0; ++k) {}
    F<uint8_t*>(e, 0x14) = F<uint8_t*>(t, 0x203a4 + b * 4);
    F<uint8_t*>(t, 0x203a4 + b * 4) = e;
    if (counted && b + 1 < 0x20)
        for (uint32_t j = b + 1; j < 0x20; ++j) F<int32_t>(t, 0x2042c + j * 4)++;
}
RT3_REPLACE(0x53c710, NameTable_Add);

// 0x53c820. Registers every file of the 13 resource directories (0x624e14,
// pattern 0x5dd610; the first two find results, "." and "..", are skipped),
// then rebuilds the per-bucket start indices (+0x2042c).
RT3_NOINLINE void __fastcall NameTable_ScanDirs(uint8_t* t) {
    auto findFirst = ((void*(__stdcall*)(const char*, void*))imp::FindFirstFileA);
    auto findNext = imp::FindNextFileA;
    for (const char* const* dir = (const char* const*)0x624e14; dir < (const char* const*)0x624e48; ++dir) {
        char pattern[0x11c];
        uint8_t fd[0x140];
        FormatText(pattern, (const char*)0x5dd610, *dir, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        void* h = findFirst(pattern, fd);
        findNext(h, fd);
        while (findNext(h, fd)) NameTable_Add(t, 0, *dir, (const char*)fd + 0x2c, F<int32_t>(fd, 0x20), 0);
        if (h != (void*)-1) imp::FindClose(h);
    }
    int32_t n = 0;
    for (int b = 0; b < 0x20; ++b) {
        F<int32_t>(t, 0x2042c + b * 4) = n;
        for (uint8_t* e = F<uint8_t*>(t, 0x203a4 + b * 4); e; e = F<uint8_t*>(e, 0x14)) ++n;
    }
}
RT3_REPLACE(0x53c820, NameTable_ScanDirs);

// 0x53c900. Rescans the resource directories and forgets the open files (+0x4290c).
RT3_NOINLINE void __fastcall NameTable_Rescan(uint8_t* t) {
    NameTable_Clear(t);
    NameTable_ScanDirs(t);
    for (int k = 0; k < 10; ++k) F<int32_t>(t, 0x4290c + k * 4) = -1;
}
RT3_REPLACE(0x53c900, NameTable_Rescan);

// 0x53c930. Whether name + suffix is in the pack name table.
RT3_NOINLINE int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix) {
    char buf[0x138];
    char* d = buf;
    while ((*d++ = *name++) != 0) {}
    if (suffix) {
        --d;
        while ((*d++ = *suffix++) != 0) {}
    }
    int32_t pack, bucket, index;
    return NameTable_Locate(t, 0, NameTable_Hash(t, 0, buf), &pack, &bucket, &index, fatal);
}
RT3_REPLACE(0x53c930, NameTable_ContainsName);

// 0x53c9c0. Size of name id's data (entry +4; loose files: +0xc).
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id) {
    int32_t pack, bucket, index;
    NameTable_Locate(t, 0, id, &pack, &bucket, &index, 1);
    if (pack < 0) return F<int32_t>(NameTable_Nth(t, 0, -pack), 0xc);
    return F<int32_t>(F<uint8_t*>(t, (pack * 32 + bucket) * 4) + index * 0xd, 4);
}
RT3_REPLACE(0x53c9c0, NameTable_EntrySize);

// 0x53ca20. Pushes {file position, current entry, previous} onto the stack +0x20428.
RT3_NOINLINE void __fastcall NameTable_PushPos(uint8_t* t) {
    uint32_t* rec = (uint32_t*)crt::malloc(0xc);
    int32_t cur = F<int32_t>(t, 0x427bc);
    int32_t fd = cur < 0 ? F<int32_t>(NameTable_Nth(t, 0, -cur), 4) : F<int32_t>(t, 0x204ac + cur * 4);
    rec[0] = (uint32_t)crt::tell(fd);
    rec[1] = F<uint32_t>(t, 0x427bc);
    rec[2] = F<uint32_t>(t, 0x20428);
    F<uint32_t*>(t, 0x20428) = rec;
    F<int32_t>(t, 0x428f8)++;
}
RT3_REPLACE(0x53ca20, NameTable_PushPos);

// 0x53ca90. Restores the last pushed pack position (+0x20428 stack of {pos, pack,
// next}): makes its pack current and seeks back; counts pops (+0x428f8).
RT3_NOINLINE void __fastcall NameTable_PopPosition(uint8_t* t) {
    if (!F<void*>(t, 0x20428)) return;
    ++F<int32_t>(t, 0x428f8);
    if (F<int32_t>(t, 0x427bc) < 0) NameTable_CloseCurrent(t);
    int32_t pack = F<int32_t>(F<uint8_t*>(t, 0x20428), 4);
    F<int32_t>(t, 0x427bc) = pack;
    int32_t fd = pack < 0 ? NameTable_OpenPack(t) : F<int32_t>(t, 0x204ac + pack * 4);
    ++F<int32_t>(t, 0x428f4);
    ((int32_t)crt::lseek(fd, F<int32_t>(F<uint8_t*>(t, 0x20428), 0), 0));
    uint8_t* top = F<uint8_t*>(t, 0x20428);
    F<uint8_t*>(t, 0x20428) = F<uint8_t*>(top, 8);
    crt::free(top);
}
RT3_REPLACE(0x53ca90, NameTable_PopPosition);

// 0x53cb10. Reads a block of the current pack: a skipped dword, the size and the
// data into a new buffer (*out; untouched when empty).
int32_t __fastcall NameTable_ReadBlock(uint8_t* t, int, int32_t, void** out) {
    int32_t skip, size;
    NameTable_Read(t, 0, &skip, 4);
    NameTable_Read(t, 0, &size, 4);
    if (!size) return 0;
    void* p = crt::malloc(size);
    *out = p;
    return NameTable_Read(t, 0, p, size);
}
RT3_REPLACE(0x53cb10, NameTable_ReadBlock);

// 0x53cb60. Constructs the file system: clears its state, creates the art tables
// (company logos 0x20, portraits 0x60, train skins 0xc0 bytes) and the index list
// 0xccba50, scans the loose directories, then mounts every .pk4 archive of the 13
// data folders (0x624e14) from the highest name down, and reads the art indexes
// listed in 0xccba50 ("companyLogo name", "portrait a b c", "trainSkin a b c d e f";
// image names get .imb).
uint8_t* __fastcall NameTable_Construct(uint8_t* t) {
    memset(t + 0x4278c, 0, 9 * 4);
    F<int32_t>(t, 0x1f400) = 0;
    for (int32_t k = 0; k < 0x20; ++k) {
        F<int32_t>(t, 0x203a4 + k * 4) = 0;
        F<int32_t>(t, 0x2042c + k * 4) = 0;
    }
    F<int32_t>(t, 0x20424) = 0;
    memset(t + 0x4290c, 0, 10 * 4);
    F<int32_t>(t, 0x429ac) = 0;
    F<int32_t>(t, 0x427b0) = -1;
    F<int32_t>(t, 0x427b4) = 0;
    F<int32_t>(t, 0x427b8) = 0;
    F<int32_t>(t, 0x427bc) = 0;
    t[0x427c0] = 0;
    for (int32_t k = 0; k < 8; ++k) F<int32_t>(t, 0x428ec + k * 4) = 0;
    F<Collection*>(t, 0x429b0) = NewTable(0x20);
    F<Collection*>(t, 0x429b4) = NewTable(0x60);
    F<Collection*>(t, 0x429b8) = NewTable(0xc0);
    g_ccba50 = NewTable(0x20);
    NameTable_ScanDirs(t);
    NameTable_Grow(t);
    memset(t + 0x2144c, 0, 1000 * 4);
    for (uint32_t dir = 0x624e14; dir < 0x624e48; dir += 4) {
        const char* folder = *(const char**)dir;
        char limit[0x130];
        memcpy(limit, (const void*)0x5dd64c, 9);
        for (;;) {
            char pattern[0x130], best[0x130];
            FormatText(pattern, (const char*)0x5dd644, folder, 0, 0, 0, 0, 0, 0, 0);
            best[0] = 0;
            bool found = false;
            WIN32_FIND_DATAA fd;
            HANDLE h = (((HANDLE(__stdcall*)(const char*, WIN32_FIND_DATAA*))imp::FindFirstFileA))(pattern, &fd);
            if (h == INVALID_HANDLE_VALUE) break;
            do {
                if (crt::stricmp(fd.cFileName, best) > 0 && crt::stricmp(fd.cFileName, limit) < 0) {
                    strcpy(best, fd.cFileName);
                    found = true;
                }
            } while ((((BOOL(__stdcall*)(HANDLE, WIN32_FIND_DATAA*))imp::FindNextFileA))(h, &fd));
            (((BOOL(__stdcall*)(HANDLE))imp::FindClose))(h);
            if (!found) break;
            char path[0x320];
            FormatText(path, (const char*)0x5c8730, folder, best, 0, 0, 0, 0, 0, 0);
            Anims_LoadPack(t, 0, path, (int32_t)folder);
            strcpy(limit, best);
        }
    }
    Collection* lists = g_ccba50;
    for (int32_t i = 0; i < Collection_Count(g_ccba50); ++i) {
        lists = g_ccba50;
        const char* name = (const char*)Collection_Get(lists, 0, Collection_IdAt(lists, 0, i, 0));
        if (!NameTable_ContainsName(t, 0, name, 0, nullptr)) break;
        FILE* f = NameTable_OpenFile(t, 0, NameTable_Hash(t, 0, name), 1);
        char kind[0x400];
        Token(f, kind);
        if (!crt::stricmp(kind, (const char*)0x5dd638)) {
            char rec[0x400] = {};
            Token(f, rec);
            AddImb(rec);
            Collection* c = F<Collection*>(t, 0x429b0);
            c->vtbl->allocate(c, rec, 0, 0, 0);
        } else if (!crt::stricmp(kind, (const char*)0x5dd624)) {
            char rec[0x600] = {};
            Token(f, rec);
            Token(f, rec + 0x20);
            Token(f, rec + 0x40);
            AddImb(rec + 0x40);
            Token(f, rec + 0x60);
            Token(f, rec + 0x80);
            AddImb(rec + 0x80);
            Token(f, rec + 0xa0);
            Collection* c = F<Collection*>(t, 0x429b8);
            c->vtbl->allocate(c, rec, 0, 0, 0);
        } else if (!crt::stricmp(kind, (const char*)0x5dd618)) {
            char rec[0x600] = {};
            Token(f, rec);
            Token(f, rec + 0x20);
            AddImb(rec + 0x20);
            Token(f, rec + 0x40);
            AddImb(rec + 0x40);
            Collection* c = F<Collection*>(t, 0x429b4);
            c->vtbl->allocate(c, rec, 0, 0, 0);
        }
    }
    return t;
}
RT3_REPLACE(0x53cb60, NameTable_Construct);

// 0x53d0f0. Pack_Acquire of collision file `name` (kind 1, by name hash).
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireCollision(uint8_t* pack, int, const char* name) {
    return Pack_Acquire(pack, 0, 1, NameTable_Hash(pack, 0, name));
}
RT3_REPLACE(0x53d0f0, Pack_AcquireCollision);

// 0x53d110. Pack_Acquire of bitmap font `name` (kind 6, by name hash).
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireNamed(uint8_t* pack, int, const char* name) {
    return Pack_Acquire(pack, 0, 6, NameTable_Hash(pack, 0, name));
}
RT3_REPLACE(0x53d110, Pack_AcquireNamed);

// 0x53d130. Registers a single existing file by full path (split at the last
// backslash; without one the name starts at index 1).
RT3_NOINLINE void __fastcall NameTable_AddPath(uint8_t* t, int, const char* path) {
    char name[0x12c], dir[0x124];
    int32_t len = (int32_t)Len(path);
    if (len <= 0) return;
    int32_t i = len;
    while (path[i] != '\\') {
        if (--i <= 0) break;
    }
    if (i == len) return;
    ++i;
    Copy(name, path + i);
    StrCopyN(dir, path, i);
    if (!File_Exists(path)) return;
    NameTable_Add(t, 0, dir, name, -1, 1);
    F<int32_t>(t, 0x427b0) = -1;
}
RT3_REPLACE(0x53d130, NameTable_AddPath);

// --- max-heap -------------------------------------------------------------

void __fastcall Heap_SiftUp(MaxHeap* h, int, int32_t i) {
    HeapNode* n = h->array[i];
    HeapNode* parent = h->array[i / 2];
    while (parent->key <= n->key) { // QUIRK: ties move up; a key of 0x7fffffff never stops
        h->array[i] = parent;
        parent->slot = &h->array[i];
        i /= 2;
        parent = h->array[i / 2];
    }
    h->array[i] = n;
    n->slot = &h->array[i];
}
RT3_REPLACE(0x53d1d0, Heap_SiftUp);

void __fastcall Heap_SiftDown(MaxHeap* h, int, int32_t i) {
    HeapNode* n = h->array[i];
    while (i <= h->count / 2) {
        int32_t child = i * 2;
        HeapNode* c = h->array[child];
        if (child < h->count && c->key < h->array[child + 1]->key) {
            c = h->array[child + 1];
            ++child;
        }
        if (c->key <= n->key) break;
        h->array[i] = c;
        c->slot = &h->array[i];
        i = child;
    }
    h->array[i] = n;
    n->slot = &h->array[i];
}
RT3_REPLACE(0x53d240, Heap_SiftDown);

int32_t __fastcall Heap_TopKey(MaxHeap* h) { return h->count > 0 ? h->array[1]->key : 0; }
RT3_REPLACE(0x53d2e0, Heap_TopKey);

void __fastcall Heap_DecrementKey(MaxHeap* h, int, HeapNode* n) {
    n->key--;
    Heap_SiftDown(h, 0, (int32_t)(n->slot - h->array));
}
RT3_REPLACE(0x53d300, Heap_DecrementKey);

// --- unsorted list with max extraction -----------------------------------

RT3_NOINLINE ListNode* __fastcall List_Push(MaxList* l, int, int32_t key, int32_t value) {
    ListNode* n = (ListNode*)PoolAlloc(l->pool);
    if (!n) return nullptr;
    SList* s = l->items;
    n->next = (ListNode*)s->head;
    s->head = (void**)n;
    s->count++;
    if (!s->tail) s->tail = (void**)n;
    n->key = key;
    n->value = value;
    return n;
}
RT3_REPLACE(0x53d320, List_Push);

int32_t __fastcall List_PopMax(MaxList* l) {
    SList* s = l->items;
    ListNode* best = (ListNode*)s->head;
    if (!best) return 0;
    ListNode* bestPrev = nullptr;
    for (ListNode *cur = best, *prev = nullptr; cur; prev = cur, cur = cur->next) {
        if (best->key < cur->key) { // first of equal maxima wins
            best = cur;
            bestPrev = prev;
        }
    }
    if (!bestPrev) {
        ListNode* h = (ListNode*)s->head;
        if (h) {
            s->head = (void**)h->next;
            if ((void**)h == s->tail) s->tail = (void**)h->next;
            s->count--;
        }
    } else {
        bestPrev->next = best->next;
        best->next = nullptr;
        s->count--;
        if ((void**)best == s->tail) s->tail = (void**)bestPrev;
    }
    int32_t value = best->value;
    PoolRelease(l->pool, best);
    return value;
}
RT3_REPLACE(0x53d390, List_PopMax);

RT3_NOINLINE int32_t __fastcall List_Contains(MaxList* l, int, int32_t value) {
    for (ListNode* n = (ListNode*)l->items->head; n; n = n->next)
        if (n->value == value) return 1;
    return 0;
}
RT3_REPLACE(0x53d440, List_Contains);

void __fastcall Heap_Destroy(MaxHeap* h) {
    if (h->array) {
        crt::free(h->array);
        h->array = nullptr;
    }
    if (NodePool* p = h->pool) {
        Pool_Free(p);
        crt::free(p);
        h->pool = nullptr;
    }
    if (h->storage) {
        crt::free(h->storage);
        h->storage = nullptr;
    }
    h->count = 0;
}
RT3_REPLACE(0x53d470, Heap_Destroy);

RT3_NOINLINE HeapNode* __fastcall Heap_Push(MaxHeap* h, int, int32_t key, int32_t value) {
    HeapNode* n = (HeapNode*)PoolAlloc(h->pool);
    if (!n) return nullptr;
    int32_t i = ++h->count;
    h->array[i] = n;
    n->slot = &h->array[i];
    n->key = key;
    n->value = value;
    Heap_SiftUp(h, 0, h->count);
    return n;
}
RT3_REPLACE(0x53d4c0, Heap_Push);

RT3_NOINLINE int32_t __fastcall Heap_PopMax(MaxHeap* h) {
    if (h->count <= 0) return 0;
    HeapNode* top = h->array[1];
    int32_t value = top->value;
    PoolRelease(h->pool, *top->slot);
    if (h->count > 1) {
        h->array[1] = h->array[h->count];
        h->count--;
        h->array[1]->slot = &h->array[1];
        Heap_SiftDown(h, 0, 1);
    } else {
        h->count--;
    }
    return value;
}
RT3_REPLACE(0x53d540, Heap_PopMax);

RT3_NOINLINE void __fastcall List_Destroy(MaxList* l) {
    if (l->items) {
        crt::free(l->items);
        l->items = nullptr;
    }
    if (NodePool* p = l->pool) {
        Pool_Free(p);
        crt::free(p);
        l->pool = nullptr;
    }
    if (l->storage) {
        crt::free(l->storage);
        l->storage = nullptr;
    }
}
RT3_REPLACE(0x53d5d0, List_Destroy);

void __fastcall List_Init(MaxList* l, int, int32_t capacity) {
    if (l->pool) List_Destroy(l);
    if (capacity > 0) {
        l->storage = crt::malloc(capacity * sizeof(ListNode));
        memset(l->storage, 0, capacity * sizeof(ListNode));
    }
    l->pool = NewPool();
    Pool_Init(l->pool, 0, l->storage, sizeof(ListNode), capacity, 0);
    SList* s = (SList*)crt::malloc(sizeof(SList));
    if (s) memset(s, 0, sizeof *s);
    l->items = s;
}
RT3_REPLACE(0x53d630, List_Init);

RT3_NOINLINE void __fastcall Heap_Destroy2(MaxHeap* h) { Heap_Destroy(h); }

 // identical copy in the exe
RT3_REPLACE(0x53d6f0, Heap_Destroy2);

RT3_NOINLINE void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity) {
    if (h->pool) Heap_Destroy(h);
    h->storage = crt::malloc(capacity * sizeof(HeapNode));
    memset(h->storage, 0, capacity * sizeof(HeapNode));
    h->pool = NewPool();
    Pool_Init(h->pool, 0, h->storage, sizeof(HeapNode), capacity, 0);
    uint32_t bytes = capacity * 4 + 4;
    h->array = (HeapNode**)crt::malloc(bytes);
    memset(h->array, 0, bytes);
    h->sentinel.value = 0;
    h->sentinel.key = 0x7fffffff;
    h->sentinel.slot = nullptr;
    h->array[0] = &h->sentinel;
}
RT3_REPLACE(0x53d740, Heap_Init);

// 0x53d800. Thunk to List_Destroy.
void __fastcall List_DestroyThunk(MaxList* l) { List_Destroy(l); }
RT3_REPLACE(0x53d800, List_DestroyThunk);

RT3_NOINLINE uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase) {
    uint32_t crc = 0;
    for (; *s; ++s) {
        uint32_t c = (uint8_t)*s;
        if (ignoreCase == 1) c &= 0xffffffdf; // folds a-z onto A-Z (and other bytes too)
        crc = (crc >> 8) ^ kCrcTable[(c ^ crc) & 0xff];
    }
    return crc;
}
RT3_REPLACE(0x53d810, Crc32String);

RT3_NOINLINE uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len) {
    uint32_t crc = 0;
    for (uint32_t i = 0; i < len; ++i) crc = (crc >> 8) ^ kCrcTable[(data[i] ^ crc) & 0xff];
    return crc;
}
RT3_REPLACE(0x53d870, Crc32);

RT3_NOINLINE void __fastcall Hash_Destroy(HashMap* m) {
    if (m->pool) {
        if (!m->storage && m->bucketCount) { // nodes came from malloc: release each
            for (uint32_t b = 0; b < m->bucketCount; ++b) {
                for (HashNode* n = m->buckets[b]; n;) {
                    HashNode* next = n->next;
                    PoolRelease(m->pool, n);
                    n = next;
                }
            }
        }
        if (NodePool* p = m->pool) {
            Pool_Free(p);
            crt::free(p);
        }
        m->pool = nullptr;
    }
    if (m->storage) {
        crt::free(m->storage);
        m->storage = nullptr;
    }
    if (m->buckets) {
        crt::free(m->buckets);
        m->buckets = nullptr;
    }
    m->poolSize = 0;
    m->bucketCount = 0;
}
RT3_REPLACE(0x53d8a0, Hash_Destroy);

RT3_NOINLINE void __fastcall Hash_Insert(HashMap* m, int, uint32_t key, int32_t value) {
    HashNode* n = (HashNode*)PoolAlloc(m->pool);
    if (!n) return; // QUIRK: silently dropped when the pool is exhausted
    uint32_t b = Bucket(m, key);
    n->key = key;
    n->value = value;
    n->prev = nullptr;
    n->next = nullptr;
    if (HashNode* head = m->buckets[b]) {
        head->prev = n;
        n->next = m->buckets[b];
    }
    m->buckets[b] = n;
}
RT3_REPLACE(0x53d960, Hash_Insert);

RT3_NOINLINE int32_t __fastcall Hash_Remove(HashMap* m, int, uint32_t key) {
    if (key == 0) return 0;
    HashNode** bucket = &m->buckets[Bucket(m, key)];
    HashNode* n = *bucket;
    if (!n) return 0;
    while (n->key != key) {
        n = n->next;
        if (!n) return 0;
    }
    int32_t value = n->value;
    if (!n->prev) {
        *bucket = n->next;
        if (n->next) n->next->prev = nullptr;
    } else {
        n->prev->next = n->next;
        if (n->next) n->next->prev = n->prev;
    }
    PoolRelease(m->pool, n);
    return value;
}
RT3_REPLACE(0x53d9e0, Hash_Remove);

RT3_NOINLINE int32_t __fastcall Hash_Contains(HashMap* m, int, uint32_t key) {
    if (key == 0) return 0;
    for (HashNode* n = m->buckets[Bucket(m, key)]; n; n = n->next)
        if (n->key == key) return 1;
    return 0;
}
RT3_REPLACE(0x53daa0, Hash_Contains);

RT3_NOINLINE int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key) {
    if (key == 0) return 0;
    for (HashNode* n = m->buckets[Bucket(m, key)]; n; n = n->next)
        if (n->key == key) return n->value;
    return 0;
}
RT3_REPLACE(0x53dae0, Hash_Get);

// Calls fn(node, ctx) for every node (key 0) or every node with `key`, until
// fn returns nonzero; returns that value.
int32_t __fastcall Hash_ForEach(HashMap* m, int, uint32_t key, HashVisit fn, void* ctx) {
    int32_t r = 0;
    if (key == 0) {
        for (uint32_t b = 0; b < m->bucketCount && r == 0; ++b) {
            for (HashNode* n = m->buckets[b]; n;) {
                HashNode* next = n->next; // fetched first: fn may remove n
                r = fn(n, ctx);
                if (r) break;
                n = next;
            }
        }
        return r;
    }
    for (HashNode* n = m->buckets[Bucket(m, key)]; n;) {
        HashNode* next = n->next;
        if (n->key == key && (r = fn(n, ctx)) != 0) break;
        n = next;
    }
    return r;
}
RT3_REPLACE(0x53db20, Hash_ForEach);

// 0x53dbe0. Thunk to Hash_Destroy.
RT3_NOINLINE void __fastcall Hash_DestroyThunk(HashMap* m) { Hash_Destroy(m); }
RT3_REPLACE(0x53dbe0, Hash_DestroyThunk);

RT3_NOINLINE int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize) {
    Hash_Destroy(m);
    if (poolSize) {
        m->storage = crt::malloc(poolSize * sizeof(HashNode));
        memset(m->storage, 0, poolSize * sizeof(HashNode));
    }
    m->pool = NewPool();
    Pool_Init(m->pool, 0, m->storage, sizeof(HashNode), poolSize, 0);
    m->buckets = (HashNode**)crt::malloc(buckets * 4);
    memset(m->buckets, 0, buckets * 4);
    m->poolSize = poolSize;
    m->bucketCount = buckets;
    return (int32_t)((buckets + poolSize * 4) * 4);
}
RT3_REPLACE(0x53dbf0, Hash_Init);

RT3_NOINLINE void __fastcall Pool_Free(NodePool* p) {
    if (p->freeList) {
        crt::free(p->freeList);
        p->freeList = nullptr;
    }
    if (p->ownsStorage == 1 && p->begin) {
        crt::free(p->begin);
        p->begin = nullptr;
    }
}
RT3_REPLACE(0x53dcb0, Pool_Free);

RT3_NOINLINE void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag) {
    p->flag = flag;
    if (p->freeList) {
        crt::free(p->freeList);
        p->freeList = nullptr;
    }
    if (p->ownsStorage == 1 && p->begin) {
        crt::free(p->begin);
        p->begin = nullptr;
    }
    p->ownsStorage = 0;
    if (!storage) {
        if (count == 0) {
            p->capacity = 0;
            p->elemSize = elemSize;
            return;
        }
        p->ownsStorage = 1;
        storage = crt::malloc(elemSize * count);
    }
    if (count != 0) {
        p->begin = (uint8_t*)storage;
        p->end = (uint8_t*)storage + elemSize * count;
        SList* f = (SList*)crt::malloc(sizeof(SList));
        memset(f, 0, sizeof *f);
        p->freeList = f;
        // Free list in address order: push from the last element down.
        for (uint8_t* e = (uint8_t*)storage + (count - 1) * elemSize; e >= (uint8_t*)storage; e -= elemSize) {
            *(void***)e = f->head;
            f->count++;
            f->head = (void**)e;
            if (!f->tail) f->tail = (void**)e;
        }
    }
    p->elemSize = elemSize;
    p->capacity = count;
}
RT3_REPLACE(0x53dcf0, Pool_Init);

double __fastcall Font_ScaleX(uint8_t* f, int, float v) { return (double)v * F<float>(f, 0x2e); }
RT3_REPLACE(0x53dde0, Font_ScaleX);

double __fastcall Font_ScaleY(uint8_t* f, int, float v) { return (double)v * F<float>(f, 0x32); }
RT3_REPLACE(0x53ddf0, Font_ScaleY);

// 0x53de00. Entry idx of the table +0x24 (count +0x28); out of range formats a
// (discarded) diagnostic with the table name (+0x20) and returns entry 0.
RT3_NOINLINE uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx) {
    int32_t n = F<uint16_t>(t, 0x28);
    if (idx < n) return F<uint32_t*>(t, 0x24)[idx];
    char buf[0x190];
    const char* name = F<const char*>(t, 0x20);
    if (!name) name = (const char*)0x5c87a8;
    crt::sprintf(buf, (const char*)0x5dd658, idx, n, name);
    return F<uint32_t*>(t, 0x24)[0];
}
RT3_REPLACE(0x53de00, Table_Entry);

// 0x53de60. Loads the DDS texture of descriptor d (base name +0x20) at the mip
// level its use deserves: d's screen size (+0 x +4 over the display mode, by
// its scale +0x10 / +0x14, halved per level) and memory (+8 x +0xc at the
// device's bytes per texel, a quarter when compressed) against the texture
// budget (memory limit x quality +0xc5c x weight +0x1c) and the device's size
// limits; a texture quality setting (+0x1146dd, 1..5) fixes the level instead.
// The first existing name_X.dds from there (going up, then from A) and, for a
// mipmapped texture (+0x18), all levels below it are taken (+0x2a); the
// descriptor's index list (+0x24, +0x28 entries) is copied to a new block.
RT3_NOINLINE void __fastcall TextureDesc_Load(uint8_t* d) {
    typedef int(__cdecl * SprintfFn)(char*, const char*, ...);
    uint8_t* dev = g_engine;
    const int32_t* mode = (const int32_t*)(F<uint8_t*>(dev, 0x34) + F<int32_t>(dev, 0x11421e) * 0x15);
    float scaleF;
    {
        int32_t w = F<int32_t>(d, 0), h = F<int32_t>(d, 4);
        float sx = F<float>(d, 0x10), sy = F<float>(d, 0x14);
        float k = *(const float*)rt3::Rdata(0x5c8d48);
        const int32_t* m = mode;
        __asm {
            fild h
            mov eax, m
            fidiv dword ptr [eax + 4]
            fdiv sy
            fild w
            fidiv dword ptr [eax]
            fdiv sx
            faddp st(1), st
            fmul k
            fstp scaleF
        }
    }
    char ext[0xc8];
    strcpy(ext, (const char*)0x5dd6c0);
    float limit = (float)Engine_TextureMemLimit(dev);
    double budget = (double)Obj_Getc5c(g_windowManager) * limit * F<float>(d, 0x1c);
    dev = g_engine;
    int32_t bpp = dev[0x11424f] ? 2 : dev[0x11422f] ? 1 : 4;
    int32_t size = F<int32_t>(d, 8) * F<int32_t>(d, 0xc) * bpp;
    if (dev[0x11422f]) size /= 4;
    float sz = (float)size;
    double scale = scaleF;
    int32_t level = 0, shift = 0;
    for (;;) {
        bool lower;
        if (level + 1 < 8 &&
            (scale > *(const double*)rt3::Rdata(0x5dd6b8) || (double)sz > budget ||
             (budget < (double)sz * *(const double*)rt3::Rdata(0x5c8698) && scale > *(const double*)rt3::Rdata(0x5c8698)) ||
             (budget < (double)sz * *(const double*)rt3::Rdata(0x5c8820) && scale > *(const double*)rt3::Rdata(0x5c8570))))
            lower = true;
        else
            lower = (F<int32_t>(d, 8) >> level) > F<int32_t>(dev, 0x114243) || (F<int32_t>(d, 0xc) >> level) > F<int32_t>(dev, 0x114247);
        if (!lower) break;
        scale *= *(const double*)rt3::Rdata(0x5c8628);
        shift += 2;
        ++level;
        sz = (float)((double)size / (double)(1 << shift));
    }
    switch (F<int32_t>(dev, 0x1146dd)) {
    case 1: level = 0; break;
    case 2: level = 1; break;
    case 3: level = 2; break;
    case 4: level = 3; break;
    case 5: level = 4; break;
    }
    char** list = (char**)crt::malloc(0x24);
    char names[9][0x100];
    for (int32_t i = 0; i < 9; ++i) {
        crt::sprintf(names[i], (const char*)0x5c87a8);
        list[i] = nullptr;
    }
    list[0] = names[0];
    uint8_t* pack = g_pack;
    const char* base = F<const char*>(d, 0x20);
    int32_t first = level;
    crt::sprintf(names[0], (const char*)0x5dd6a8, base, level + 0x41, ext);
    if (!NameTable_ContainsName(g_pack, 0, names[0], 0, nullptr)) {
        for (int32_t i = 1; i <= 5; ++i) {
            first = i + level;
            crt::sprintf(names[0], (const char*)0x5dd6a8, base, level + 0x41 + i, ext);
            if (NameTable_ContainsName(g_pack, 0, names[0], 0, nullptr)) break;
        }
    }
    if (!NameTable_ContainsName(g_pack, 0, names[0], 0, nullptr)) {
        for (int32_t i = 1 - level;; ++i) {
            first = i;
            crt::sprintf(names[0], (const char*)0x5dd6a8, base, i + 0x41, ext);
            if (NameTable_ContainsName(g_pack, 0, names[0], 0, nullptr)) break;
            if (i + 1 + level > 5) break;
        }
    }
    (void)pack;
    int32_t count = 1;
    if (F<int32_t>(d, 0x18)) {
        char tmp[0x12c];
        for (int32_t c = first + 1 + 0x41, k = 1; first + 1 < 8 && c < 0x49; ++c, ++k) {
            crt::sprintf(tmp, (const char*)0x5dd6a8, base, c, ext);
            if (NameTable_ContainsName(g_pack, 0, tmp, 0, nullptr)) {
                strcpy(names[k], tmp);
                list[k] = names[k];
            }
        }
        if (F<int32_t>(d, 0x18)) count = 8 - first;
    }
    F<uint8_t*>(d, 0x2a) = TextureCache_Get(g_pack, 0, base, first, count, 0);
    crt::free(list);
    uint32_t n = (uint32_t)F<uint16_t>(d, 0x28) * 4;
    uint8_t* copy = (uint8_t*)crt::malloc(n);
    memcpy(copy, F<void*>(d, 0x24), n);
    crt::free(F<void*>(d, 0x24));
    F<uint8_t*>(d, 0x24) = copy;
}
RT3_REPLACE(0x53de60, TextureDesc_Load);

}  // namespace rt3
