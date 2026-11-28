// NetCore.cpp - RT3.exe 0x520000-0x524000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <initializer_list>
#include "hook/hook.h"
#include "core/globals.h"
#include "core/stream.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include "core/imports.h"
#include "game/collection.h"
#include "hook/keepregs.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Engine_ClampWindowSize(void* e, int, int32_t* w, int32_t* h);
const char* __fastcall GetString(int32_t id);
char* __stdcall Net_LocalAddress(char* out);
void __fastcall Texture_CountMemory(uint8_t* t);
uint32_t __fastcall Obj_Next0(uint8_t* o);
void __fastcall Nop545df0(void*, int, int32_t, int32_t);
void __fastcall Fatal_Exit(const char* msg);
ATOM __fastcall Engine_RegisterWindowClass(HINSTANCE inst);
double __fastcall Terrain_HeightAt(uint8_t* r, int, float x, float z);
int32_t Net_LockQueue();
void Net_UnlockQueue();
void __fastcall Render_SetViewOffset(uint8_t* r, int, uint32_t x, uint32_t y);
void __fastcall Thread_Destruct(uint8_t* t);
int32_t __fastcall Render_UnlockVb2647(uint8_t* r);
void __fastcall Render_DrawTriangles(uint8_t* r, int, int32_t first, int32_t end);
void* __fastcall Render_AppendVb2647(uint8_t* r, int, int32_t first, int32_t count);
void* __fastcall Render_LockVb2647(uint8_t* r, int, int32_t first, int32_t count);
int32_t __fastcall Render_LightingAndSun(uint8_t* r);
int32_t __fastcall Fx_Intensity(uint8_t* o);
void __fastcall Renderer_SetTimeOfDay(uint8_t* r, int, int32_t secs, int32_t night);
void __fastcall Render_ApplyFilterStates(uint8_t* r);
void __fastcall Render_ResetWorld(uint8_t* r);
int32_t __fastcall Render_SetShadeMode(uint8_t* r, int, uint32_t v);
wchar_t* __fastcall Str_ToWide(wchar_t* dst, const char* src, int32_t n);
void __fastcall Thread_Stop(uint8_t* t, int, uint32_t ms);
void __fastcall Thread_Start(uint8_t* t);
void __fastcall Hash_DestroyThunk(HashMap* m);
void __fastcall Pool_Free(NodePool* p);
int32_t __fastcall Hash_ForEach(HashMap* m, int, uint32_t key, HashVisit fn, void* ctx);
int32_t __fastcall Heap_PopMax(MaxHeap* h);
void __fastcall Heap_Destroy2(MaxHeap* h);
void __fastcall Fx_SetIntensity(uint8_t* o, int, int32_t v);
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
void __fastcall Pool_Init(NodePool* p, int, void* storage, int32_t elemSize, int32_t count, int32_t flag);
void __fastcall Set_d973b0(uint32_t v);
int32_t __stdcall Net_OpenChannel(uint8_t id);
void __stdcall Net_FreePlayerRecord(uint8_t* p);
void __fastcall Scene_ApplyLighting(uint8_t* s);
void __fastcall Scene_PushBusy(uint8_t* s);
void __fastcall Scene_PopBusy(uint8_t* s);
void __fastcall Scene_ApplyLighting(uint8_t* r);
int32_t __fastcall Sound_SetBufferMs(uint8_t* e, int, void* o, int32_t ms);
uint32_t* __stdcall Net_GetGuid(uint32_t* out);
int32_t __fastcall CmdQueue_Broadcast(int32_t a, int32_t b, int32_t c);
int32_t __stdcall DpAddress_AddString(void* addr, const char* text);
int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

int32_t ClockMs();
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Pack_ReleaseFontHandles(uint8_t* pack);
void __fastcall Display_DestroyRenderer(uint8_t* o);
void __fastcall Pack_RecreateFonts(uint8_t* pack);
uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag);
int32_t __fastcall Engine_CreateWindow(uint8_t* e, int, int32_t x, int32_t y, int32_t mode, uint8_t fullscreen);
uint8_t* __fastcall Mouse_Construct(uint8_t* m, int, void* hwnd, void* inst);
uint32_t __fastcall Obj_Get18(uint8_t* o);
void __fastcall Mouse_Acquire(uint8_t* m);
void __fastcall Mouse_Unacquire(uint8_t* m);
void __fastcall Mouse_Destroy(uint8_t* m);
void __fastcall Timer_Restart(uint8_t* t);
void __fastcall Engine_SaveConfig(uint8_t* e);
void StreamInit();
void DebugLog_WriteHeader();
void __fastcall Engine_LoadConfig(uint8_t* o);
void Language_Load();
void __fastcall Engine_EnumDisplayModes(uint8_t* e);
PtrList* __fastcall PtrList_Init(PtrList* l);
int32_t __fastcall Engine_PickDisplayMode(uint8_t* e, int, int32_t* w, int32_t* h, int32_t* bpp);
uint8_t* __fastcall NameTable_Construct(uint8_t* t);
uint8_t* __fastcall Input_Construct(uint8_t* in);
uint8_t* __fastcall KeyTable_Init(uint8_t* t);
uint32_t __fastcall SysInfo_TotalMemory();
void __fastcall ModelLibrary_Init(uint32_t* o, int, int32_t count);
uint8_t* __fastcall Sound_Construct(uint8_t* snd, int, int32_t provider);
int32_t __fastcall Sound_SelectProvider(uint8_t* snd, int, int32_t n);
int32_t __fastcall Sound_ActiveProviderIndex(uint8_t* s);
void __fastcall Obj_SetName24(uint8_t* o, int, const char* s);
void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind);
uint8_t* __fastcall HandleTable_Construct(uint8_t* t);
uint8_t* __fastcall Cursor_Construct(uint8_t* c);
void __fastcall Engine_BuildTrigTables(void* e);
int32_t __fastcall Engine_ModeHasFlags(void* e);
void __fastcall Scene_CreateRenderer(uint8_t* s);
void __fastcall Camera_Destruct(uint32_t* c);
void __fastcall Cursor_Draw(uint8_t* c);
void __fastcall Scene_DrawOverlay(uint8_t* s, int, int32_t frame);
void __fastcall Engine_UpdateFrameTime(void* e);
void __fastcall Mouse_Poll(uint8_t* m);
void __fastcall Engine_ClearRenderLists(uint8_t* e);
int32_t __fastcall Texture_AppendJpeg(uint8_t* t, int, const char* path, uint32_t withAlpha, int32_t quality);
void __fastcall Texture_SaveTga(uint8_t* t, int, const char* path, int32_t flag);
void __fastcall Texture_Destroy(uint8_t* t);
uint8_t* __fastcall Camera_ConstructSetup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z, uint32_t dist);
void __fastcall Camera_SetBounds(uint8_t* c, int, uint32_t a, uint32_t b, uint32_t d, uint32_t e);
void __fastcall Scene_SetViewOffset(uint8_t* s, int, uint32_t x, uint32_t y);
uint8_t __fastcall Engine_CaptureFrontBuffer(uint8_t* e, int, uint8_t* dst, int32_t x, int32_t y, int32_t stride);
void __fastcall Engine_DrawFrame(uint8_t* e);
RT3_NOINLINE uint8_t* __fastcall Pack_AcquireFont(uint8_t* pack, int, int32_t idx);
RT3_NOINLINE int32_t __fastcall Ui_SendEvent5(uint8_t** mgr, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Engine_Init(uint8_t* e, int, void* inst, int32_t w, int32_t h);
void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
uint8_t __fastcall Engine_PosterScreenshot(uint8_t* e);
uint8_t __fastcall Engine_Screenshot(uint8_t* e, int, int32_t hideCursor);
int32_t __fastcall Net_PlayerJoined(int32_t, uint8_t* msg);
int32_t __fastcall Net_PlayerLeft(int32_t, uint8_t* msg);
uint8_t* __fastcall Net_FindByName(const char* name);
uint8_t* __fastcall Net_FindById(int32_t id);
uint8_t* __fastcall Engine_FindTagged(char tag);
uint8_t* __fastcall CmdNode_New(int32_t clear, int32_t lock);
void __fastcall SList_InsertAfter(void** l, int, void** after, void** node);
int32_t __stdcall Sounds_Has(uint8_t* rec, int32_t touch);
void* __fastcall FreeList_Take(uint8_t* q);
void __fastcall SList_PushFront(void** l, int, void** node);
void __fastcall Engine_SetHardwareOption(uint8_t* e, int, int32_t on);
void __fastcall Engine_SetDisplayOption(uint8_t* e, int, int32_t v);
void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);
typedef void(WINAPI* CsFn)(LPCRITICAL_SECTION);
struct HashMap;
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
char* __fastcall Str_FromWide(char* dst, const wchar_t* src, int32_t n);
int32_t __fastcall Hash_Init(HashMap* m, int, uint32_t buckets, int32_t poolSize);
void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity);
uint32_t __fastcall SysInfo_CpuSignature();
void __fastcall PtrList_Destroy(PtrList* l);
void __fastcall Terrain_Destructor(uint8_t* r);
void __fastcall Engine_Shutdown(uint8_t* e, int, uint8_t full);
void __fastcall Windows_Shutdown(uint8_t* m);
void __fastcall Nop564170();
void __fastcall Cursor_Shutdown(uint8_t* c);
void __fastcall Pack_Shutdown(uint8_t* t);
void __fastcall Nop54e780();
void __fastcall Sound_Destruct(uint8_t* snd);
void __fastcall ModelLibrary_Free(uint32_t* o);
void FreeLanguageStrings();
void StreamReleaseAll();

// Functions of this file, in exe order.
int32_t __fastcall Engine_CreateWindow(uint8_t* e, int, int32_t x, int32_t y, int32_t mode, uint8_t fullscreen);
void __fastcall Window_FrameSize(void** o, int, int32_t* w, int32_t* h);
void __fastcall Engine_PlaceWindow(uint8_t* e, int, int32_t x, int32_t y);
RT3_NOINLINE void __fastcall Engine_SwitchDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);
RT3_NOINLINE void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode);
RT3_NOINLINE void __fastcall Engine_ClearRenderLists(uint8_t* e);
int32_t __fastcall Engine_PickDisplayMode(uint8_t* e, int, int32_t* w, int32_t* h, int32_t* bpp);
RT3_NOINLINE void __fastcall Engine_DrawFrame(uint8_t* e);
RT3_NOINLINE void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save);
int32_t __fastcall Obj_Sub2543(uint8_t* o);
RT3_NOINLINE void __stdcall Textures_MemoryTotals(int32_t* video, int32_t* resident, int32_t* count);
RT3_NOINLINE uint8_t __fastcall Engine_PosterScreenshot(uint8_t* e);
RT3_NOINLINE uint8_t __fastcall Engine_Screenshot(uint8_t* e, int, int32_t hideCursor);
RT3_NOINLINE void __fastcall Engine_SetHardwareOption(uint8_t* e, int, int32_t on);
RT3_NOINLINE void __fastcall Engine_SetDisplayOption(uint8_t* e, int, int32_t v);
void __fastcall Engine_Init(uint8_t* e, int, void* inst, int32_t w, int32_t h);
void __fastcall Engine_Destroy(uint8_t* e);
int32_t __fastcall Obj_FreeBuffer4(uint8_t* o);
void Net_DeleteLocks();
RT3_NOINLINE int32_t __stdcall DpAddress_AddString(void* addr, const char* text);
RT3_NOINLINE int32_t __fastcall Sound_SetBufferMs(uint8_t* e, int, void* o, int32_t ms);
RT3_NOINLINE int32_t Global6d4100();
RT3_NOINLINE int32_t Net_LockQueue();
RT3_NOINLINE void Net_UnlockQueue();
RT3_NOINLINE int32_t Global6d40e4();
RT3_NOINLINE void __stdcall Net_UnlockQueueArg(int32_t);
RT3_NOINLINE char* __stdcall Net_LocalAddress(char* out);
RT3_NOINLINE const char* __fastcall Net_ErrorText(int32_t code);
int32_t __fastcall Return1_521790(void*, int, int32_t, int32_t);
uint8_t __stdcall Net_MsgKind(const uint8_t* msg);
RT3_NOINLINE void __fastcall Nop5217b0(void*);
RT3_NOINLINE int32_t Net_LockList2();
RT3_NOINLINE void Net_UnlockList2();
RT3_NOINLINE void __fastcall Net_BroadcastGame(uint8_t* o, int, const char* name);
RT3_NOINLINE void __stdcall SetGlobal6d4108(int32_t v);
void __fastcall Net_StopListener();
RT3_NOINLINE const char* Net_SocketErrorText();
RT3_NOINLINE uint32_t* __stdcall Net_GetGuid(uint32_t* out);
uint8_t* __fastcall Thread_DeletingDestructor(uint8_t* t, int, uint32_t flags);
RT3_NOINLINE uint8_t* __fastcall Net_FindById(int32_t id);
RT3_NOINLINE uint8_t* __fastcall Net_FindByName(const char* name);
RT3_NOINLINE uint8_t* __fastcall Engine_FindTagged(char tag);
RT3_NOINLINE int32_t __stdcall Sounds_Has(uint8_t* rec, int32_t touch);
RT3_NOINLINE uint8_t* __fastcall Net_Construct(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Net_SetPlayerName(uint8_t* o, int, const char* name);
int32_t __fastcall Engine_DeviceCall68(uint8_t* e, int, int32_t v);
RT3_NOINLINE void* Engine_FirstListEntry();
RT3_NOINLINE uint8_t* __stdcall Textures_Find(int32_t key);
int32_t __fastcall Net_SendTo(uint8_t* net, int, const uint8_t* data, uint16_t size, uint8_t type, uint8_t channel, const char* to, uint32_t flags, uint32_t timeout, uint32_t serial, int32_t);
RT3_NOINLINE int32_t __fastcall Textures_QueryDesc(uint8_t* e, int, int32_t key, int32_t lock);
RT3_NOINLINE uint8_t* __stdcall Textures_Lookup(uint8_t* t, int32_t keep);
RT3_NOINLINE uint8_t* __fastcall CmdNode_New(int32_t clear, int32_t lock);
void __fastcall CmdQueue_Clear(uint8_t* q);
RT3_NOINLINE int32_t __fastcall CmdQueue_Broadcast(int32_t a, int32_t b, int32_t c);
int32_t __stdcall Net_OpenChannel(uint8_t id);
void __fastcall Net_ResetOutgoing();
RT3_NOINLINE void __stdcall Net_PurgeSentMessages(int32_t);
RT3_NOINLINE uint8_t* __fastcall Net_FindFlaggedPlayer();
RT3_NOINLINE char* __stdcall Net_PlayerNameById(int32_t id);
int32_t __stdcall Net_PlayerIdByName(const char* name);
int32_t __fastcall Lan_ListenThread(uint8_t* ctx);
int32_t __fastcall Net_SendQueueTotals(uint8_t* o, int, int32_t* msgs, int32_t* bytes);
int32_t __fastcall Net_FreePendingList(uint8_t* o);
RT3_NOINLINE int32_t __fastcall Net_PlayerJoined(int32_t, uint8_t* msg);
RT3_NOINLINE int32_t __fastcall Net_PlayerLeft(int32_t, uint8_t* msg);
void __stdcall Net_FreePlayerRecord(uint8_t* p);
uint32_t* __stdcall Net_TakeChannelMessages(uint8_t id);
void __fastcall Net_FinishReceived(uint8_t* net, int, uint32_t* list);
int32_t __fastcall Net_ShutdownShared(uint8_t* net);
int32_t __fastcall Net_HandleMessage(int32_t ctx, uint32_t type, uint8_t* msg);
RT3_NOINLINE int32_t __fastcall Net_InitShared(uint8_t* net);
RT3_NOINLINE int32_t __fastcall Net_Host(uint8_t* o, int, const char* game, const char* name, const char* password, int32_t ms, int32_t maxPlayers);
RT3_NOINLINE int32_t __fastcall Net_Connect(uint8_t* o, int, const char* host, const char* name, const char* password, int32_t ms);
void* __fastcall Scene_BeginBatch(uint8_t* s, int, int32_t count);
void __fastcall Scene_EndBatch(uint8_t* s, int, uint8_t draw);
void __fastcall Fx_SetIntensity(uint8_t* o, int, int32_t v);
RT3_NOINLINE void __fastcall Scene_ApplyLighting(uint8_t* s);
RT3_NOINLINE void __fastcall Scene_Set372c(uint8_t* s, int, uint32_t v);
RT3_NOINLINE void __fastcall Scene_Set3720(uint8_t* s, int, uint32_t a, uint32_t b);
RT3_NOINLINE void __fastcall Scene_PushBusy(uint8_t* s);
RT3_NOINLINE void __fastcall Scene_PopBusy(uint8_t* s);
void __fastcall Scene_SetViewOffset(uint8_t* s, int, uint32_t x, uint32_t y);
int32_t __stdcall Terrain_RayOcclusion(uint8_t* t, float* pos, const float* dir);
RT3_NOINLINE void __fastcall Scene_RenderFrame(uint8_t* s, int, uint8_t* cam, uint8_t* ter, uint8_t second);
RT3_NOINLINE void __fastcall Scene_CreateRenderer(uint8_t* s);
RT3_NOINLINE uint8_t* __fastcall Scene_Construct(uint8_t* s);
RT3_NOINLINE void __fastcall Scene_DrawOverlay(uint8_t* s, int, int32_t frame);

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

// Unlinks `node` from the singly linked list {head, tail, count}; a node that
// isn't found (or is the head) makes it drop the head instead, as the original.
static void SList_Unlink(uint32_t* head, uint32_t* tail, int32_t* count, uint32_t* node) {
    for (uint32_t* p = (uint32_t*)(uintptr_t)*head; p; p = (uint32_t*)(uintptr_t)p[0]) {
        if ((uint32_t*)(uintptr_t)p[0] == node) {
            p[0] = node[0];
            node[0] = 0;
            --*count;
            if ((uint32_t)(uintptr_t)node == *tail) *tail = (uint32_t)(uintptr_t)p;
            return;
        }
    }
    if (uint32_t* first = (uint32_t*)(uintptr_t)*head) {
        *head = first[0];
        if ((uint32_t)(uintptr_t)first == *tail) *tail = *head;
        --*count;
    }
}

// Gives `node` back to the block pool {free list, block start, block end, active}
// when it came from the pool's block, else deletes it.
static void Pool_Return(uint8_t* pool, uint32_t* node) {
    if (F<int32_t>(pool, 0xc) && (uintptr_t)node >= F<uint32_t>(pool, 4) && (uintptr_t)node < F<uint32_t>(pool, 8)) {
        uint32_t* free = F<uint32_t*>(pool, 0);
        node[0] = free[0];
        ++free[2];
        free[0] = (uint32_t)(uintptr_t)node;
        if (!free[1]) free[1] = (uint32_t)(uintptr_t)node;
    } else {
        crt::operator_delete(node);
    }
}

namespace {
int32_t Tooltip_Clear() { return Ui_ShowTooltip(g_windowManager, 0, (const char*)0x5c87a8, 3, 0, -1); }
// A network message's handler gets the packet (+8) when it has one (+0xc).
template <typename P, typename Fn> int32_t NetMsg_Forward(uint8_t* m, Fn handler) {
    if (F<int32_t>(m, 0xc) > 0) handler(F<P>(m, 8));
    return 1;
}
}

namespace {
// Win32 / Winsock imports, called through their IAT slots.
// fn: imp::EnterCriticalSection / LeaveCriticalSection / Interlocked*.
template <typename Fn> static void Lock456(Fn fn, uint32_t cs) { ((void(__stdcall*)(void*))fn)((void*)cs); }
}

template <typename Fn> static inline void Enter(uint32_t cs, Fn fn) { ((CsFn)fn)((LPCRITICAL_SECTION)cs); }

// The pool pop shared by the functions below.
static void* Pool_Take(uint8_t* pool) {
    if (!F<int32_t>(pool, 0xc)) return crt::operator_new(F<uint32_t>(pool, 0x10));
    uint8_t* list = F<uint8_t*>(pool, 0);
    uint8_t* node = F<uint8_t*>(list, 0);
    if (!node) return nullptr;
    uint8_t* next = F<uint8_t*>(node, 0);
    F<uint8_t*>(list, 0) = next;
    if (node == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = next;
    --F<int32_t>(list, 8);
    return node;
}

// The pool push: back onto the list when it is enabled and node is in range.
static void Pool_Give(uint8_t* pool, uint8_t* node) {
    if (F<int32_t>(pool, 0xc) && node >= F<uint8_t*>(pool, 4) && node < F<uint8_t*>(pool, 8)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        F<uint8_t*>(node, 0) = F<uint8_t*>(list, 0);
        F<uint8_t*>(list, 0) = node;
        ++F<int32_t>(list, 8);
        if (!F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = node;
    } else crt::operator_delete(node);
}

// 0x520040. Creates the game window at (x, y) (+0x4d/+0x51, reset when off the
// first quarter of the display mode +0x34[+0x11421e]): windowed with the frame
// style 0x4cb0000, or a bare popup for fullscreen; sets the icon 0x6d4038.
RT3_NOINLINE int32_t __fastcall Engine_CreateWindow(uint8_t* e, int, int32_t x, int32_t y, int32_t mode, uint8_t fullscreen) {
    F<int32_t>(e, 0x4d) = x;
    F<int32_t>(e, 0x51) = y;
    if (x < 0) F<int32_t>(e, 0x4d) = 0;
    if (y < 0) F<int32_t>(e, 0x51) = 0;
    const int32_t* res = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    if (F<int32_t>(e, 0x4d) > res[0] >> 2) F<int32_t>(e, 0x4d) = 0;
    if (F<int32_t>(e, 0x51) > res[1] >> 2) F<int32_t>(e, 0x51) = 0;
    if (mode > F<int32_t>(e, 0x30)) Fatal_Exit(GetString(0x15));
    res = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    int32_t w = res[0], h = res[1];
    if (!Engine_RegisterWindowClass(F<HINSTANCE>(e, 4))) Fatal_Exit(GetString(0x16));
    auto create = ((HWND(__stdcall*)(DWORD, const char*, const char*, DWORD, int, int, int, int, HWND, HMENU,
                                          HINSTANCE, void*))imp::CreateWindowExA);
    if (!fullscreen) {
        RECT r = { 0, 0, w - 1, h - 1 };
        ((BOOL(__stdcall*)(RECT*, DWORD, BOOL))imp::AdjustWindowRect)(&r, 0x4cb0000, FALSE);
        F<HWND>(e, 0) = create(0, (const char*)0x5dcee8, (const char*)0x624b68, 0x4cb0000, F<int32_t>(e, 0x4d),
                               F<int32_t>(e, 0x51), r.right - r.left + 1, r.bottom - r.top + 1, nullptr, nullptr,
                               F<HINSTANCE>(e, 4), nullptr);
    } else {
        F<HWND>(e, 0) = nullptr;
        HWND wnd = create(0, (const char*)0x5dcee8, (const char*)0x624b68, 0x4000000, 0, 0, w, h, nullptr, nullptr,
                          F<HINSTANCE>(e, 4), nullptr);
        F<HWND>(e, 0) = wnd;
        ((LONG(__stdcall*)(HWND, int, LONG))imp::SetWindowLongA)(wnd, -16, 0x4000000);
    }
    e[0x55] = 0;
    if (uint16_t icon = g_6d4038) {
        auto loadIcon = ((HICON(__stdcall*)(HINSTANCE, const char*))imp::LoadIconA);
        auto send = ((LRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM))imp::PostMessageA);
        send(F<HWND>(e, 0), 0x80, 1, (LPARAM)loadIcon(F<HINSTANCE>(e, 4), (const char*)(uintptr_t)icon));
        send(F<HWND>(e, 0), 0x80, 0,
             (LPARAM)loadIcon(F<HINSTANCE>(e, 4), (const char*)(uintptr_t)g_6d4038));
    }
    return 0;
}
RT3_REPLACE(0x520040, Engine_CreateWindow);

// 0x5201e0. Window frame size for the window +0 (two user32 rectangle calls).
void __fastcall Window_FrameSize(void** o, int, int32_t* w, int32_t* h) {
    int32_t r[4] = { 0, 0, 0x12c, 0x12c };
    int32_t r2[4];
    imp::GetWindowRect(o[0], r2);
    imp::AdjustWindowRect(r, 0x4cb0000, 0);
    *w = r2[0] - r[0];
    *h = r2[1] - r[1];
}
RT3_REPLACE(0x5201e0, Window_FrameSize);

// 0x520250. Moves the window client origin to (x, y) (window frame added).
void __fastcall Engine_PlaceWindow(uint8_t* e, int, int32_t x, int32_t y) {
    if (!F<void*>(e, 0xc)) return;
    const int32_t* mode = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    int32_t rc[4] = { 0, 0, mode[0] - 1, mode[1] - 1 };
    imp::AdjustWindowRect(rc, 0x4cb0000, 0);
    int32_t wx = x + rc[0], wy = y + rc[1];
    F<int32_t>(e, 0x51) = wy;
    F<int32_t>(e, 0x4d) = wx;
    Nop545df0(F<void*>(e, 0xc), 0, wx, wy);
}
RT3_REPLACE(0x520250, Engine_PlaceWindow);

// 0x5202c0. Engine e: switches the display to `mode` (+0x11421e) and
// `fullscreen` (+0x114226, the old one capped at 1). +0x114222 keeps the
// previous setting (10 << old mode | old fullscreen << 1 | 1). Recreates the
// window, calls the hook 0x6d400c, makes a new mouse (+0x20), the scene (+0x18,
// or a new renderer for an existing one) and the fonts, releases the mouse
// capture; in full screen it acquires the mouse when the cursor is game drawn
// (+0x11473e) or captures it again, otherwise it unacquires it. Finally UI
// event 0xac.
RT3_NOINLINE void __fastcall Engine_SwitchDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode) {
    if (e[0x114226] > 1) e[0x114226] = 1;
    uint32_t prev = (10u << (F<uint32_t>(e, 0x11421e) & 0x1f)) | ((uint32_t)e[0x114226] << 1) | 1;
    F<int32_t>(e, 0x11421e) = mode;
    F<uint32_t>(e, 0x114222) = prev;
    e[0x114226] = (uint8_t)fullscreen;
    Engine_CreateWindow(e, 0, F<int32_t>(e, 0x4d), F<int32_t>(e, 0x51), mode, (uint8_t)fullscreen);
    if (void (*hook_)() = hook::Global<void (*)()>(0x6d400c)) hook_();
    uint8_t* m = (uint8_t*)crt::operator_new(0x21);
    F<uint8_t*>(e, 0x20) = m ? Mouse_Construct(m, 0, F<void*>(e, 0), F<void*>(e, 4)) : nullptr;
    if (!F<uint8_t*>(e, 0x18)) {
        uint8_t* s = (uint8_t*)crt::operator_new(0x3abdb);
        F<uint8_t*>(e, 0x18) = s ? Scene_Construct(s) : nullptr;
    } else
        Scene_CreateRenderer(F<uint8_t*>(e, 0x18));
    Pack_RecreateFonts(g_pack);
    imp::ReleaseCapture();
    if (fullscreen) {
        uint8_t* c = g_cursorManager;
        bool acquire;
        if (c && Obj_Get18(c)) acquire = true;
        else if (g_cursorManager) acquire = false;
        else acquire = F<int32_t>(e, 0x11473e) != 0;
        if (acquire) Mouse_Acquire(F<uint8_t*>(e, 0x20));
        else imp::SetCapture(F<void*>(g_engine, 0));
    } else
        Mouse_Unacquire(F<uint8_t*>(e, 0x20));
    if (hook::As<uint8_t**>(g_windowManager)) Ui_SendEvent5(hook::As<uint8_t**>(g_windowManager), 0, 0xac, 0, 0, 0, 0);
}
RT3_REPLACE(0x5202c0, Engine_SwitchDisplay);

// 0x520400. Engine e: rebuilds the display for `fullscreen` / `mode` (-1:
// the current mode): drops the font handles, the mouse and the renderer,
// calls the hook 0x6d4010, destroys the window and its class, switches the
// display, keeps the new fullscreen flag (+0x114696), restarts the cursor
// timer and saves the configuration.
RT3_NOINLINE void __fastcall Engine_ResetDisplay(uint8_t* e, int, int32_t fullscreen, int32_t mode) {
    Pack_ReleaseFontHandles(g_pack);
    if (uint8_t* m = F<uint8_t*>(e, 0x20)) {
        Mouse_Destroy(m);
        crt::operator_delete(m);
    }
    F<uint8_t*>(e, 0x20) = nullptr;
    Display_DestroyRenderer(F<uint8_t*>(e, 0x18));
    if (void (*hook_)() = hook::Global<void (*)()>(0x6d4010)) hook_();
    e[0x55] = 0;
    imp::DestroyWindow(F<void*>(e, 0));
    ((int32_t(__stdcall*)(const char*, void*))imp::UnregisterClassA)((const char*)0x5dcee8, F<void*>(e, 4));
    F<void*>(e, 0) = nullptr;
    if (mode == -1) mode = F<int32_t>(e, 0x11421e);
    Engine_SwitchDisplay(e, 0, fullscreen, mode);
    e[0x114696] = g_engine[0x114226];
    if (uint8_t* c = g_cursorManager) Timer_Restart(c);
    Engine_SaveConfig(e);
}
RT3_REPLACE(0x520400, Engine_ResetDisplay);

// 0x5204b0. Clears the engine's three render lists (the third holds objects
// whose own list +0xe4 is cleared and flag +0xe8 reset).
RT3_NOINLINE void __fastcall Engine_ClearRenderLists(uint8_t* e) {
    PtrList* a = F<PtrList*>(e, 0x1136a1);
    if (F<int32_t>(a, 0xc)) PtrList_Clear(a);
    PtrList* b = F<PtrList*>(e, 0x11369d);
    if (F<int32_t>(b, 0xc)) PtrList_Clear(b);
    PtrList_Rewind(F<PtrList*>(e, 0x1136a5));
    while (uint8_t* o = (uint8_t*)PtrList_Next(F<PtrList*>(e, 0x1136a5))) {
        F<int32_t>(o, 0xe8) = 0;
        PtrList_Clear(F<PtrList*>(o, 0xe4));
    }
    PtrList_Clear(F<PtrList*>(e, 0x1136a5));
}
RT3_REPLACE(0x5204b0, Engine_ClearRenderLists);

// 0x520530. Picks the display mode (0x15-byte entries +0x34, count +0x30)
// closest to w x h x bpp (16 if no 32-bit mode exists) and refresh +0x44;
// writes it back and returns its index. QUIRK: no candidate reads entry -1.
RT3_NOINLINE int32_t __fastcall Engine_PickDisplayMode(uint8_t* e, int, int32_t* w, int32_t* h, int32_t* bpp) {
    Engine_ClampWindowSize(e, 0, w, h);
    int32_t n = F<int32_t>(e, 0x30);
    bool has32 = false;
    for (int32_t i = 0; i < n; ++i)
        if (F<int32_t>(F<uint8_t*>(e, 0x34) + i * 0x15, 8) == 0x20) has32 = true;
    if (!has32) *bpp = 0x10;
    int32_t best = 0x7fffffff, sel = -1;
    for (int32_t i = 0; i < F<int32_t>(e, 0x30); ++i) {
        uint8_t* m = F<uint8_t*>(e, 0x34) + i * 0x15;
        if (!F<int32_t>(m, 0)) continue;
        int32_t d = (*bpp - F<int32_t>(m, 8)) * 0xffff - F<int32_t>(m, 4) - F<int32_t>(m, 0) + *w + *h;
        int32_t rate = F<int32_t>(m, 0xc), want = F<int32_t>(e, 0x44);
        if (rate > want) d += rate;
        else if (rate < want) d += want - rate;
        if (d >= 0 && d <= best) {
            best = d;
            sel = i;
        }
    }
    uint8_t* m = F<uint8_t*>(e, 0x34) + sel * 0x15;
    *w = F<int32_t>(m, 0);
    *h = F<int32_t>(m, 4);
    *bpp = F<int32_t>(m, 8);
    return sel;
}
RT3_REPLACE(0x520530, Engine_PickDisplayMode);

// 0x520620. Engine e: draws a frame - the scene (+0x18) from the camera
// (+0x28) over the terrain (+0x1c) when both are set and the view is live
// (+0x56); otherwise just the frame time, cursor and overlay. Shows the
// window the first time (+0x55), then clears the render lists.
RT3_NOINLINE void __fastcall Engine_DrawFrame(uint8_t* e) {
    Mouse_Poll(F<uint8_t*>(e, 0x20));
    if (F<uint8_t*>(e, 0x1c) && e[0x56] && F<uint8_t*>(e, 0x28)) {
        Scene_RenderFrame(F<uint8_t*>(e, 0x18), 0, F<uint8_t*>(e, 0x28), F<uint8_t*>(e, 0x1c), 0);
    } else {
        Engine_UpdateFrameTime(e);
        Cursor_Draw(g_cursorManager);
        Scene_DrawOverlay(F<uint8_t*>(e, 0x18), 0, 1);
    }
    if (!e[0x55]) {
        e[0x55] = 1;
        imp::ShowWindow(F<void*>(e, 0), g_6cec58);
    }
    Engine_ClearRenderLists(e);
    F<int32_t>(e, 0x58) = 0;
    e[0x56] = 0;
}
RT3_REPLACE(0x520620, Engine_DrawFrame);

// 0x5206b0. Engine e: sets display size w x h and `fullscreen` (-1: the
// current one). Unless that is the current mode (table +0x34, 0x15 bytes per
// mode) with the same fullscreen and hardware flag (+0x11422c: +0x1146ae
// without +0x11427a in full screen), picks the best mode at 32 bits and
// rebuilds the display. With `save` the size is remembered (+0x11468a /
// +0x11468e); the configuration is saved either way.
RT3_NOINLINE void __fastcall Engine_SetDisplayMode(uint8_t* e, int, int32_t w, int32_t h, int32_t fullscreen, int32_t save) {
    uint8_t* eng = g_engine;
    if (fullscreen == -1) fullscreen = eng[0x114226];
    int32_t hw = e[0x1146ae] && !F<int32_t>(e, 0x11427a) && fullscreen ? 1 : 0;
    uint8_t* cur = F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
    if (w != F<int32_t>(cur, 0) || h != F<int32_t>(cur, 4) || fullscreen != eng[0x114226] || e[0x11422c] != hw) {
        int32_t bpp = 0x20;
        int32_t mode = Engine_PickDisplayMode(e, 0, &w, &h, &bpp);
        eng[0x114226] = (uint8_t)fullscreen;
        Engine_ResetDisplay(e, 0, g_engine[0x114226], mode);
    }
    if (save) {
        F<int32_t>(e, 0x11468a) = w;
        F<int32_t>(e, 0x11468e) = h;
    }
    Engine_SaveConfig(e);
}
RT3_REPLACE(0x5206b0, Engine_SetDisplayMode);

int32_t __fastcall Obj_Sub2543(uint8_t* o) { return F<int32_t>(F<uint8_t*>(o, 0xc), 0x2543); }
RT3_REPLACE(0x520790, Obj_Sub2543);

// 0x5207a0. Texture memory totals over the global texture list (video memory,
// resident, count).
RT3_NOINLINE void __stdcall Textures_MemoryTotals(int32_t* video, int32_t* resident, int32_t* count) {
    if (video) *video = 0;
    if (resident) *resident = 0;
    if (count) *count = 0;
    int32_t v = 0, r = 0, n = 0;
    for (uint8_t* t = g_ccbb2c; t; t = (uint8_t*)Obj_Next0(t)) {
        Texture_CountMemory(t);
        v += F<int32_t>(t, 0xcc);
        r += F<int32_t>(t, 0xd0);
        ++n;
    }
    if (video) *video = v;
    if (resident) *resident = r;
    if (count) *count = n;
}
RT3_REPLACE(0x5207a0, Textures_MemoryTotals);

// 0x520830. Engine e: saves a 3x3 poster screenshot - the frame redrawn
// without the cursor at view offsets of +-18 (scene fog +0x25eb dimmed to
// 0.3, the poster flag +0x114216 set), each tile captured into a
// three-times-the-display-mode image, saved like Engine_Screenshot (the
// name's numbers via Num_Format). The camera's first word (+0x28) is kept
// in a scratch camera across the shots. 0 when the device is not ready.
// QUIRK: the pixel buffer is never freed; a failed image allocation still
// saves through a null image (and a failed camera allocation crashes).
RT3_NOINLINE uint8_t __fastcall Engine_PosterScreenshot(uint8_t* e) {
    if (!e[0x114226]) return 0;
    uint8_t* mem = ((uint8_t*)crt::operator_new(0x8a));
    uint8_t* cam = mem ? Camera_ConstructSetup(mem, 0, F<uint32_t>(g_engine, 0x1c), 0, 0, 0, 0x44800000) : nullptr;
    F<uint32_t>(cam, 0) = F<uint32_t>(F<uint8_t*>(e, 0x28), 0);
    Camera_SetBounds(F<uint8_t*>(e, 0x28), 0, 0, 0, 0x3f800000, 0x3f800000);
    uint8_t* mode = F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
    int32_t w = F<int32_t>(mode, 0), h = F<int32_t>(mode, 4);
    uint32_t size = (uint32_t)(h * w) * 9 * 4;
    uint8_t* pixels = (uint8_t*)crt::malloc(size);
    memset(pixels, 0, size);
    uint8_t* tm = ((uint8_t*)crt::operator_new(0xec));
    uint8_t* img = tm ? Texture_InitMemory(tm, 0, pixels, w * 3, h * 3, 1) : nullptr;
    uint8_t* scene = F<uint8_t*>(g_engine, 0x18);
    Cursor_Set(g_cursorManager, 0, 0, 0, 0);
    g_windowManager[0xc68] = 1;
    int32_t poster = F<int32_t>(e, 0x114216);
    F<int32_t>(e, 0x114216) = 1;
    F<uint32_t>(F<uint8_t*>(scene, 0x2d), 0x25eb) = 0x3e99999a;
    static const uint32_t offs[3] = { 0xc1900000, 0, 0x41900000 };
    for (int32_t row = 0; row < 3; ++row)
        for (int32_t col = 0; col < 3; ++col) {
            Scene_SetViewOffset(scene, 0, offs[col], offs[2 - row]);
            if (F<int32_t>(e, 0x1c)) e[0x56] = 1;
            Engine_DrawFrame(e);
            Engine_CaptureFrontBuffer(e, 0, pixels, w * col, h * row, w * 3);
        }
    uint16_t st[8];  // SYSTEMTIME
    imp::GetLocalTime(st);
    char sec[0x20], min[0x20], hour[0x20], year[0x20], day[0x20], month[0x20], name[0x120], path[0x100];
    const char* s1 = Num_Format(sec, 1, (double)(int32_t)st[6], 10, 1, 0, 0);
    const char* s2 = Num_Format(min, 1, (double)(int32_t)st[5], 10, 1, 0, 0);
    const char* s3 = Num_Format(hour, 1, (double)(int32_t)st[4], 10, 1, 0, 0);
    const char* s4 = Num_Format(year, 1, (double)((int32_t)st[0] % 100), 10, 1, 0, 0);
    const char* s5 = Num_Format(day, 1, (double)(int32_t)st[3], 10, 1, 0, 0);
    const char* s6 = Num_Format(month, 1, (double)(int32_t)st[1], 10, 1, 0, 0);
    FormatText(name, (const char*)0x5dcf20, s6, s5, s4, s3, s2, s1, 0, 0);
    if (F<int32_t>(e, 0x114722)) {
        FormatText(path, (const char*)0x5dcf08, name, 0, 0, 0, 0, 0, 0, 0);
        Texture_AppendJpeg(img, 0, path, 1, F<int32_t>(e, 0x114726));
    } else {
        FormatText(path, (const char*)0x5dcef0, name, 0, 0, 0, 0, 0, 0, 0);
        Texture_SaveTga(img, 0, path, 1);
    }
    if (img) {
        Texture_Destroy(img);
        crt::operator_delete(img);
    }
    Cursor_Set(g_cursorManager, 0, 1, 0, 0);
    g_windowManager[0xc68] = 0;
    F<int32_t>(e, 0x114216) = poster;
    F<uint32_t>(F<uint8_t*>(scene, 0x2d), 0x25eb) = 0x3f800000;
    Scene_SetViewOffset(scene, 0, 0, 0);
    F<uint32_t>(F<uint8_t*>(e, 0x28), 0) = F<uint32_t>(cam, 0);
    Camera_Destruct((uint32_t*)cam);
    crt::operator_delete(cam);
    if (F<int32_t>(e, 0x1c)) e[0x56] = 1;
    return 1;
}
RT3_REPLACE(0x520830, Engine_PosterScreenshot);

// 0x520d70. Engine e: saves a screenshot of the front buffer (copied via a
// scratch A8R8G8B8 texture of the window size +0x11468a / +0x11468e) to
// ScreenShots\RT3_<mm_dd_yy__hh_mm_ss>.jpg (with +0x114722, quality
// +0x114726) or .tga. With hideCursor the frame is redrawn without the
// cursor first and with it afterwards. 0 when the device is not ready
// (+0x114226) or the front buffer could not be read.
// QUIRK: the pixel copy is never freed, and a failed image allocation still
// saves through a null image.
RT3_NOINLINE uint8_t __fastcall Engine_Screenshot(uint8_t* e, int, int32_t hideCursor) {
    if (!e[0x114226]) return 0;
    uint8_t ok = 1;
    if (hideCursor) {
        Cursor_Set(g_cursorManager, 0, 0, 0, 0);
        g_windowManager[0xc68] = 1;
        if (F<int32_t>(e, 0x1c)) e[0x56] = 1;
        Engine_DrawFrame(e);
    }
    void* dev = F<void*>(e, 8);
    int32_t w = F<int32_t>(e, 0x11468a), h = F<int32_t>(e, 0x11468e);
    void* tex;
    void* surf;
    ((int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void**))(*(void***)dev)[20])(dev, w, h, 1, 0, 0x15, 3, &tex);
    ((int32_t(__stdcall*)(void*, int32_t, void**))(*(void***)tex)[15])(tex, 0, &surf);
    void* d = F<void*>(e, 8);
    if (((int32_t(__stdcall*)(void*, void*))(*(void***)d)[30])(d, surf)) {
        ((uint32_t(__stdcall*)(void*))(*(void***)surf)[2])(surf);
        ((uint32_t(__stdcall*)(void*))(*(void***)tex)[2])(tex);
        ok = 0;
    } else {
        struct {
            int32_t pitch;
            uint8_t* bits;
        } lr;
        ((int32_t(__stdcall*)(void*, void*, void*, uint32_t))(*(void***)surf)[9])(surf, &lr, nullptr, 0);
        uint32_t size = (uint32_t)(h * w) * 4;
        uint8_t* pixels = (uint8_t*)crt::malloc(size);
        memcpy(pixels, lr.bits, size);
        ((int32_t(__stdcall*)(void*))(*(void***)surf)[10])(surf);
        ((uint32_t(__stdcall*)(void*))(*(void***)surf)[2])(surf);
        ((uint32_t(__stdcall*)(void*))(*(void***)tex)[2])(tex);
        uint8_t* mem = ((uint8_t*)crt::operator_new(0xec));
        uint8_t* img = mem ? Texture_InitMemory(mem, 0, pixels, w, h, 1) : nullptr;
        uint16_t st[8];  // SYSTEMTIME
        imp::GetLocalTime(st);
        char name[0x12c], path[0x1f0];
        crt::sprintf(name, (const char*)0x5dcf34, st[1], st[3], st[0] % 100, st[4], st[5], st[6]);
        if (F<int32_t>(e, 0x114722)) {
            FormatText(path, (const char*)0x5dcf08, name, 0, 0, 0, 0, 0, 0, 0);
            Texture_AppendJpeg(img, 0, path, 1, F<int32_t>(e, 0x114726));
        } else {
            FormatText(path, (const char*)0x5dcef0, name, 0, 0, 0, 0, 0, 0, 0);
            Texture_SaveTga(img, 0, path, 1);
        }
        if (img) {
            Texture_Destroy(img);
            crt::operator_delete(img);
        }
    }
    if (hideCursor) {
        Cursor_Set(g_cursorManager, 0, 1, 0, 0);
        g_windowManager[0xc68] = 0;
        if (F<int32_t>(e, 0x1c)) e[0x56] = 1;
        Engine_DrawFrame(e);
    }
    return ok;
}
RT3_REPLACE(0x520d70, Engine_Screenshot);

// 0x520fd0. Engine e: turns the hardware option +0x1146ae on or off (only on
// when the mode supports it) and rebuilds the display when it changes.
RT3_NOINLINE void __fastcall Engine_SetHardwareOption(uint8_t* e, int, int32_t on) {
    int32_t cur = e[0x1146ae] && !F<int32_t>(e, 0x11427a) ? 1 : 0;
    if (on == cur) return;
    if (on && !Engine_ModeHasFlags(e)) return;
    int32_t fs = e[0x114226], mode = F<int32_t>(e, 0x11421e);
    e[0x1146ae] = (uint8_t)on;
    Engine_ResetDisplay(e, 0, fs, mode);
}
RT3_REPLACE(0x520fd0, Engine_SetHardwareOption);

// 0x521030. Engine e: sets option +0x11470e; in full screen a change
// rebuilds the display.
RT3_NOINLINE void __fastcall Engine_SetDisplayOption(uint8_t* e, int, int32_t v) {
    if (v == F<int32_t>(e, 0x11470e)) return;
    F<int32_t>(e, 0x11470e) = v;
    if (e[0x114226]) Engine_ResetDisplay(e, 0, e[0x114226], F<int32_t>(e, 0x11421e));
}
RT3_REPLACE(0x521030, Engine_SetDisplayOption);

// 0x521060. Engine e: start-up for instance `inst` at display size (w, h)
// (-1: the saved +0x11468a / +0x11468e). Streams, debug log, configuration,
// language and display modes; the object lists (+0x1136a1, +0x1136a5,
// +0x11369d); the display mode (+0x11421e) and fullscreen flag; the name
// table 0x6d4020, input 0x6d4018, key table (+0x24) and model library
// 0x6d4030 (10000 entries below 192 MB of memory); the display; the sound
// system 0x6d402c with its provider (+0x114706), name 0x5dcf54 and the three
// volumes; the UI 0x6d401c, cursor 0xccbb20, the fonts 4 / 1 / 2 and the trig
// tables.
void __fastcall Engine_Init(uint8_t* e, int, void* inst, int32_t w, int32_t h) {
    auto New = [](size_t n) { return (uint8_t*)crt::operator_new(n); };
    e[0x114205] = 0;
    F<uint32_t>(e, 0x114201) = 1;
    F<void*>(e, 4) = inst;
    F<uint32_t>(e, 0x114206) = 0;
    StreamInit();
    DebugLog_WriteHeader();
    Engine_LoadConfig(e);
    Language_Load();
    Engine_EnumDisplayModes(e);
    memset(e + 0x11428a, 0, 0x400);
    F<uint32_t>(e, 0x114286) = 0;
    e[0x11369c] = 0;
    PtrList* l = (PtrList*)New(0x18);
    F<PtrList*>(e, 0x1136a1) = l ? PtrList_Init(l) : nullptr;
    l = (PtrList*)New(0x18);
    F<PtrList*>(e, 0x1136a5) = l ? PtrList_Init(l) : nullptr;
    l = (PtrList*)New(0x18);
    F<PtrList*>(e, 0x11369d) = l ? PtrList_Construct(l, 0, 1, 100) : nullptr;
    if (w == -1) {
        w = F<int32_t>(e, 0x11468a);
        h = F<int32_t>(e, 0x11468e);
    }
    F<int32_t>(e, 0x11421e) = Engine_PickDisplayMode(e, 0, &w, &h, (int32_t*)(e + 0x114692));
    e[0x114226] = e[0x114696];
    uint8_t* o = New(0x429bc);
    g_pack = o ? NameTable_Construct(o) : nullptr;
    o = New(0xaa8);
    g_input = o ? Input_Construct(o) : nullptr;
    o = New(0x400);
    F<uint8_t*>(e, 0x24) = o ? KeyTable_Init(o) : nullptr;
    o = New(0x38);
    if (o) {
        F<uint32_t>(o, 0) = 0;
        for (int32_t k = 8; k <= 0x18; k += 4) F<uint32_t>(o, k) = 0;
        F<uint32_t>(o, 0x30) = 0x3f800000;
        F<uint32_t>(o, 0x34) = 0xbf800000;
    }
    g_models = o;
    ModelLibrary_Init(hook::As<uint32_t*>(g_models), 0, (SysInfo_TotalMemory() & 0xfff00000) < 0xc000000 ? 10000 : 0);
    Engine_SwitchDisplay(e, 0, e[0x114226], F<int32_t>(e, 0x11421e));
    F<uint32_t>(e, 0x10) = 0;
    o = New(0x54);
    g_sound = o ? Sound_Construct(o, 0, -1) : nullptr;
    Sound_SelectProvider(g_sound, 0, F<int32_t>(e, 0x114706));
    F<int32_t>(e, 0x114706) = Sound_ActiveProviderIndex(g_sound);
    if (uint8_t* d = F<uint8_t*>(g_sound, 0)) Obj_SetName24(d, 0, (const char*)0x5dcf54);
    uint8_t* eng = g_engine;
    Sound_SetVolume(g_sound, 0, F<float>(eng, 0x1146fa), 2);
    eng = g_engine;
    Sound_SetVolume(g_sound, 0, (float)(F<float>(eng, 0x1146fe) * D(0x5c9c80)), 0);
    eng = g_engine;
    Sound_SetVolume(g_sound, 0, (float)(F<float>(eng, 0x114702) * D(0x5c8640)), 1);
    o = New(0xc7d);
    g_windowManager = o ? HandleTable_Construct(o) : nullptr;
    o = New(0x2c);
    g_cursorManager = o ? Cursor_Construct(o) : nullptr;
    F<uint8_t*>(e, 0x1141f5) = Pack_AcquireFont(g_pack, 0, 4);
    F<uint8_t*>(e, 0x1141f9) = Pack_AcquireFont(g_pack, 0, 1);
    F<uint8_t*>(e, 0x1141fd) = Pack_AcquireFont(g_pack, 0, 2);
    Engine_BuildTrigTables(e);
}
RT3_REPLACE(0x521060, Engine_Init);

// 0x521390. Tears the engine e down: its three pointer lists, the terrain (+0x1c),
// the device (Engine_Shutdown), the three pack images, then the window manager
// 0x6d401c, +0x24, the cursor 0xccbb20, the mouse (+0x20), the pack 0x6d4020, the
// globals 0x6d4018 / 0x6d402c (sound) / 0x6d4030 (models), the buffer +0x34, the
// language strings and the streams; 0x6d4024 (the engine) is cleared.
void __fastcall Engine_Destroy(uint8_t* e) {
    F<int32_t>(e, 0x114201) = 0;
    static const uint32_t lists[] = { 0x11369d, 0x1136a1, 0x1136a5 };
    for (uint32_t off : lists)
        if (PtrList* l = F<PtrList*>(e, off)) {
            PtrList_Destroy(l);
            crt::operator_delete(l);
        }
    if (uint8_t* t = F<uint8_t*>(e, 0x1c)) {
        Terrain_Destructor(t);
        crt::operator_delete(t);
    }
    F<uint8_t*>(e, 0x1c) = nullptr;
    Engine_Shutdown(e, 0, 1);
    static const uint32_t images[] = { 0x1141f5, 0x1141f9, 0x1141fd };
    for (uint32_t off : images) {
        if (void* p = F<void*>(e, off)) Pack_Release(g_pack, 0, p);
        F<void*>(e, off) = nullptr;
    }
    if (uint8_t* m = g_windowManager) {
        Windows_Shutdown(m);
        crt::operator_delete(m);
    }
    g_windowManager = nullptr;
    if (uint8_t* p = F<uint8_t*>(e, 0x24)) {
        Nop564170();
        crt::operator_delete(p);
    }
    F<uint8_t*>(e, 0x24) = nullptr;
    if (uint8_t* c = g_cursorManager) {
        Cursor_Shutdown(c);
        crt::operator_delete(c);
    }
    g_cursorManager = nullptr;
    if (uint8_t* m = F<uint8_t*>(e, 0x20)) {
        Mouse_Destroy(m);
        crt::operator_delete(m);
    }
    F<uint8_t*>(e, 0x20) = nullptr;
    if (uint8_t* p = g_pack) {
        Pack_Shutdown(p);
        crt::operator_delete(p);
    }
    g_pack = nullptr;
    if (uint8_t* p = g_input) {
        Nop54e780();
        crt::operator_delete(p);
    }
    g_input = nullptr;
    if (uint8_t* s = g_sound) {
        Sound_Destruct(s);
        crt::operator_delete(s);
    }
    g_sound = nullptr;
    if (uint32_t* m = hook::As<uint32_t*>(g_models)) {
        ModelLibrary_Free(m);
        crt::operator_delete(m);
    }
    hook::As<uint32_t*>(g_models) = nullptr;
    crt::free(F<void*>(e, 0x34));
    FreeLanguageStrings();
    StreamReleaseAll();
    hook::As<void*>(g_engine) = nullptr;
}
RT3_REPLACE(0x521390, Engine_Destroy);

// 0x521580 / 0x468ae0 (vtables). Free the buffer +4 (the second when set).
int32_t __fastcall Obj_FreeBuffer4(uint8_t* o) {
    crt::operator_delete(F<void*>(o, 4));
    return 0;
}
RT3_REPLACE(0x521580, Obj_FreeBuffer4);

// 0x521590. Deletes the four network critical sections.
void Net_DeleteLocks() {
    auto del = imp::DeleteCriticalSection;
    del((void*)0x6d4090);
    del((void*)0x6d406c);
    del((void*)0x6d4048);
    del((void*)0x6d40b4);
}
RT3_REPLACE(0x521590, Net_DeleteLocks);

// 0x5215c0. Adds `text` as a wide-string component (name 0x5dcf5c) to the
// DirectPlay8 address `addr` (AddComponent); 1 on success.
RT3_NOINLINE int32_t __stdcall DpAddress_AddString(void* addr, const char* text) {
    wchar_t buf[0x80];
    Str_ToWide(buf, text, -1);
    int32_t len = ((int32_t)crt::wcslen(buf));
    int32_t hr = (*(int32_t(__stdcall**)(void*, const void*, const void*, int32_t, int32_t))(*(uint8_t**)addr + 0x48))(addr, (const void*)0x5dcf5c, buf,
                                                                                                              len * 2 + 2, 1);
    return hr >= 0 ? 1 : 0;
}
RT3_REPLACE(0x5215c0, DpAddress_AddString);

// 0x521620. Sets the stream buffer length property (0x5dcf70, 4 bytes, how 2) of
// COM object o to ms (9000 when 0), keeping it in +0x216; whether it succeeded.
RT3_NOINLINE int32_t __fastcall Sound_SetBufferMs(uint8_t* e, int, void* o, int32_t ms) {
    if (!ms) ms = 0x2328;
    F<int32_t>(e, 0x216) = ms;
    return ((HRESULT(__stdcall*)(void*, const void*, int32_t*, int32_t, int32_t))(*(void***)o)[0x48 / 4])(o, (const void*)0x5dcf70, &ms, 4, 2) == 0 ? 1 : 0;
}
RT3_REPLACE(0x521620, Sound_SetBufferMs);

RT3_NOINLINE int32_t Global6d4100() { return g_6d4100; }
RT3_REPLACE(0x521670, Global6d4100);

// 0x521680 / 0x5216a0. Locks / unlocks the network queue (critical section
// 0x6d4090, depth 0x6d40a8); lock returns the queue 0x6d408c.
RT3_NOINLINE int32_t Net_LockQueue() {
    imp::EnterCriticalSection((void*)0x6d4090);
    g_6d40a8++;
    return hook::As<int32_t>(g_6d408c);
}
RT3_REPLACE(0x521680, Net_LockQueue);

RT3_NOINLINE void Net_UnlockQueue() {
    g_6d40a8--;
    imp::LeaveCriticalSection((void*)0x6d4090);
}
RT3_REPLACE(0x5216a0, Net_UnlockQueue);

RT3_NOINLINE int32_t Global6d40e4() { return g_6d40e4; }
RT3_REPLACE(0x5216c0, Global6d40e4);

// 0x5216d0. Unlock variant that pops one (unused) argument.
RT3_NOINLINE void __stdcall Net_UnlockQueueArg(int32_t) {
    g_6d40a8--;
    imp::LeaveCriticalSection((void*)0x6d4090);
}
RT3_REPLACE(0x5216d0, Net_UnlockQueueArg);

// 0x5216f0. This machine's IP address as text (winsock), or "" .
RT3_NOINLINE char* __stdcall Net_LocalAddress(char* out) {
    char host[0x104];
    out[0] = 0;
    imp::gethostname(host, 0x104);
    uint8_t* h = imp::gethostbyname(host);
    if (!h) return out;
    uint32_t addr = **F<uint32_t**>(h, 0xc);
    const char* ip = imp::inet_ntoa(addr);
    if (!ip) return out;
    char* d = out;
    while ((*d++ = *ip++) != 0) {}
    return out;
}
RT3_REPLACE(0x5216f0, Net_LocalAddress);

// 0x521750. Text for a network error code (table 0x624c60: {code, string id, ..}).
RT3_NOINLINE const char* __fastcall Net_ErrorText(int32_t code) {
    for (int32_t i = 0; i < 0xd8 / 12; ++i)
        if (((const int32_t*)0x624c60)[i * 3] == code) return GetString(((const int32_t*)0x624c64)[i * 3]);
    return GetString(((const int32_t*)0x624c58)[(0xd8 / 12) * 3]);
}
RT3_REPLACE(0x521750, Net_ErrorText);

int32_t __fastcall Return1_521790(void*, int, int32_t, int32_t) { return 1; }
RT3_REPLACE(0x521790, Return1_521790);

uint8_t __stdcall Net_MsgKind(const uint8_t* msg) { return F<uint8_t*>((void*)msg, 4)[9]; }
RT3_REPLACE(0x5217a0, Net_MsgKind);

RT3_NOINLINE void __fastcall Nop5217b0(void*) { __asm nop }
RT3_REPLACE(0x5217b0, Nop5217b0);

// 0x5217c0 / 0x5217e0. Lock / unlock the second network list (critical
// section 0x6d4048, depth 0x6d4060); lock returns the list 0x6d4044.
RT3_NOINLINE int32_t Net_LockList2() {
    imp::EnterCriticalSection((void*)0x6d4048);
    g_6d4060++;
    return hook::As<int32_t>(g_6d4044);
}
RT3_REPLACE(0x5217c0, Net_LockList2);

RT3_NOINLINE void Net_UnlockList2() {
    g_6d4060--;
    imp::LeaveCriticalSection((void*)0x6d4048);
}
RT3_REPLACE(0x5217e0, Net_UnlockList2);

// 0x521800. Broadcasts the LAN game announcement "name[;tag];ip;port" to UDP port 1234.
RT3_NOINLINE void __fastcall Net_BroadcastGame(uint8_t* o, int, const char* name) {
    if (g_6d4100 >= 4) return;
    char msg[0x100], ip[0x100];
    int32_t port = F<int32_t>(o, 0x216);
    if (o[0x21a]) {
        const char* addr = Net_LocalAddress(ip);
        crt::sprintf(msg, (const char*)0x5dcf88, name, GetString(0xdba), addr, port);
    } else {
        const char* addr = Net_LocalAddress(ip);
        crt::sprintf(msg, (const char*)0x5dcf7c, name, addr, port);
    }
    int32_t s = imp::socket(2, 2, 0);
    if (s < 0) return;
    int32_t one = 1;
    if (imp::setsockopt(s, 0xffff, 0x20, &one, 4) == 0) {
        uint8_t addr[16] = {};
        F<uint16_t>(addr, 0) = 2;
        F<uint32_t>(addr, 4) = imp::htonl(0xffffffff);
        F<uint16_t>(addr, 2) = imp::htons(0x4d2);
        size_t len = 0;
        while (msg[len]) ++len;
        imp::sendto(
            s, msg, (int32_t)len, 0, addr, 16);
    }
    imp::closesocket(s);
}
RT3_REPLACE(0x521800, Net_BroadcastGame);

RT3_NOINLINE void __stdcall SetGlobal6d4108(int32_t v) { g_6d4108 = v; }
RT3_REPLACE(0x521910, SetGlobal6d4108);

// 0x521920. Stops the network listener: flags the thread (+0x10) to quit,
// closes its socket 0x6d40e8, waits up to 30 ms and deletes it; then shuts
// Winsock down if it was started (0x6d410c).
void __fastcall Net_StopListener() {
    if (uint8_t* t = g_6d40d4) {
        g_6d40ec = 1;
        F<int32_t>(t, 0x10) = 1;
        imp::closesocket(g_6d40e8);
        g_6d40e8 = 0;
        Thread_Stop(g_6d40d4, 0, 0x1e);
        if (uint8_t* t2 = g_6d40d4) {
            (*(void(__fastcall**)(uint8_t*, int, int32_t))F<void**>(t2, 0))(t2, 0, 1);
            g_6d40d4 = nullptr;
        }
    }
    if (g_6d410c) imp::WSACleanup();
}
RT3_REPLACE(0x521920, Net_StopListener);

// 0x521990. Text (in 0x6d4120) for the last socket error 0x6d4118.
RT3_NOINLINE const char* Net_SocketErrorText() {
    int32_t id;
    switch (g_6d4118) {
    case 10036: id = 0xffb; break;
    case 10067: id = 0xffc; break;
    case 10091: id = 0xff9; break;
    case 10092: id = 0xffa; break;
    default: id = 0xffd; break;
    }
    const char* s = GetString(id);
    char* d = (char*)0x6d4120;
    while ((*d++ = *s++) != 0) {}
    return (const char*)0x6d4120;
}
RT3_REPLACE(0x521990, Net_SocketErrorText);

// 0x521aa0. Copies the session GUID 0x6257e8.
RT3_NOINLINE uint32_t* __stdcall Net_GetGuid(uint32_t* out) {
    for (int k = 0; k < 4; ++k) out[k] = ((const uint32_t*)0x6257e8)[k];
    return out;
}
RT3_REPLACE(0x521aa0, Net_GetGuid);

// 0x521ad0. Deleting destructor of the worker thread object.
uint8_t* __fastcall Thread_DeletingDestructor(uint8_t* t, int, uint32_t flags) {
    Thread_Destruct(t);
    if (flags & 1) crt::operator_delete(t);
    return t;
}
RT3_REPLACE(0x521ad0, Thread_DeletingDestructor);

// 0x521af0 / 0x521b10. Network queue entry by ID (+4) / by name CRC (+0x48).
RT3_NOINLINE uint8_t* __fastcall Net_FindById(int32_t id) {
    for (uint8_t* e = *g_6d408c; e; e = *(uint8_t**)e)
        if (F<int32_t>(e, 4) == id) return e;
    return nullptr;
}
RT3_REPLACE(0x521af0, Net_FindById);

RT3_NOINLINE uint8_t* __fastcall Net_FindByName(const char* name) {
    uint32_t crc = Crc32String(name, 1);
    for (uint8_t* e = *g_6d408c; e; e = *(uint8_t**)e)
        if (F<uint32_t>(e, 0x48) == crc) return e;
    return nullptr;
}
RT3_REPLACE(0x521b10, Net_FindByName);

// 0x521b40. The node of the list at 0x6d40b0 whose byte +0x1c is tag, or null.
RT3_NOINLINE uint8_t* __fastcall Engine_FindTagged(char tag) {
    for (uint8_t* n = *g_6d40b0; n; n = F<uint8_t*>(n, 0))
        if ((char)n[0x1c] == tag) return n;
    return nullptr;
}
RT3_REPLACE(0x521b40, Engine_FindTagged);

// 0x521b60. Whether a sound with rec's id (+0x104) and name (+0x84) is in the list
// 0x6d4044; touch also stamps the found one's time (+0x118).
RT3_NOINLINE int32_t __stdcall Sounds_Has(uint8_t* rec, int32_t touch) {
    for (uint8_t* n = *hook::As<uint8_t**>(g_6d4044); n; n = F<uint8_t*>(n, 0))
        if (F<int32_t>(n, 0x104) == F<int32_t>(rec, 0x104) && !strcmp((const char*)n + 0x84, (const char*)rec + 0x84)) {
            if (touch) F<int32_t>(n, 0x118) = ClockMs();
            return 1;
        }
    return 0;
}
RT3_REPLACE(0x521b60, Sounds_Has);

// 0x521bf0. Network object constructor: clears the DirectPlay pointers (+0x18e,
// +0x212, +0x216) and, the first time (no socket 0x6d40e8), starts Winsock 1.0
// (WSADATA at the object's start; result in 0x6d4118) and the listener thread
// 0x6d40d4 (vtable 0x5dcf94).
RT3_NOINLINE uint8_t* __fastcall Net_Construct(uint8_t* o) {
    F<uint32_t>(o, 0x18e) = 0;
    F<uint32_t>(o, 0x212) = 0;
    F<uint32_t>(o, 0x216) = 0;
    g_6d4118 = 0;
    g_6d411c = 0;
    if (g_6d40e8) return o;
    g_6d40ec = 0;
    int32_t r = imp::WSAStartup(1, o);
    g_6d410c = 1;
    uint32_t* t = (uint32_t*)crt::operator_new(0x14);
    if (t) {
        t[1] = 0;
        t[2] = 0;
        t[3] = 0;
        t[0] = 0x5dcf94;
        t[4] = 0;
    }
    hook::As<uint32_t*>(g_6d40d4) = t;
    Thread_Start((uint8_t*)t);
    g_6d4118 = r;
    return o;
}
RT3_REPLACE(0x521bf0, Net_Construct);

// 0x521c80. Sets the local player's name (IDirectPlay8Peer::SetPeerInfo, sync);
// the peer interface goes to 0x6d40e0. 1 on success.
RT3_NOINLINE int32_t __fastcall Net_SetPlayerName(uint8_t* o, int, const char* name) {
    g_6d40e0 = F<void*>(F<uint8_t*>(o, 0x18e), 8);
    struct { uint32_t size, flags; wchar_t* name; void* data; uint32_t dataSize, playerFlags; } info;
    wchar_t buf[0x80];
    Str_ToWide(buf, name, -1);
    memset(&info, 0, sizeof info);
    info.name = buf;
    info.size = 0x18;
    info.flags = 1;
    void* peer = F<void*>(F<uint8_t*>(o, 0x18e), 8);
    int32_t hr = (*(int32_t(__stdcall**)(void*, void*, void*, void*, uint32_t))(*(uint8_t**)peer + 0x50))(peer, &info, nullptr, nullptr, 0x80000000);
    return hr >= 0 ? 1 : 0;
}
RT3_REPLACE(0x521c80, Net_SetPlayerName);

// 0x521d10. When 0x6d40dc is set, calls method 0x68 of the device at +0x18e -> +8
// with (v, 0, 0, 0).
int32_t __fastcall Engine_DeviceCall68(uint8_t* e, int, int32_t v) {
    if (!g_6d40dc) return 0;
    void* dev = F<void*>(F<uint8_t*>(e, 0x18e), 8);
    return ((int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t))(*(void***)dev)[0x68 / 4])(dev, v, 0, 0, 0);
}
RT3_REPLACE(0x521d10, Engine_DeviceCall68);

// 0x521d40. The first entry of the list at 0x6d408c, or null.
RT3_NOINLINE void* Engine_FirstListEntry() {
    void** l = hook::As<void**>(g_6d408c);
    if (!l || !*l) return nullptr;
    return *l;
}
RT3_REPLACE(0x521d40, Engine_FirstListEntry);

// 0x521d60. The texture entry with id key (list 0x6d408c). When found it returns
// with the lock 0x6d4090 still held (and its count +1), as the original does.
RT3_NOINLINE uint8_t* __stdcall Textures_Find(int32_t key) {
    Enter(0x6d4090, imp::EnterCriticalSection);
    ++g_6d40a8;
    uint8_t** l = g_6d408c;
    if (l)
        for (uint8_t* n = *l; n; n = F<uint8_t*>(n, 0))
            if (F<int32_t>(n, 4) == key) return n;
    --g_6d40a8;
    Enter(0x6d4090, imp::LeaveCriticalSection);
    return nullptr;
}
RT3_REPLACE(0x521d60, Textures_Find);

// 0x521dc0. Sends a game message through DirectPlay (IDirectPlay8Peer::SendTo).
// The packet is a 12-byte header {0, `serial`, `channel`, `type`, 12, high flag
// bits} followed by `size` bytes of `data` (on the heap when guaranteed and over
// 4 KB). `to` names the receiving player (looked up by the name's CRC in the
// player list, +0x48; unknown names send nothing); null sends to everyone.
// Flags: a serial makes it guaranteed (8, not 2), otherwise non-guaranteed
// sends get 0x12 (no loopback, non-sequential). The byte count goes to 0x6d4104.
// True only when sent to a named player.
int32_t __fastcall Net_SendTo(uint8_t* net, int, const uint8_t* data, uint16_t size, uint8_t type, uint8_t channel, const char* to,
                              uint32_t flags, uint32_t timeout, uint32_t serial, int32_t) {
    uint8_t local[0x100c];
    uint8_t* pkt = local;
    if ((flags & 8) && (uint32_t)size + 0xc > 0x1000) pkt = (uint8_t*)crt::operator_new((uint32_t)size + 0xc);
    memset(pkt, 0, 0xc);
    pkt[8] = channel;
    pkt[9] = type;
    F<uint32_t>(pkt, 4) = serial;
    uint8_t high = (uint8_t)(((flags >> 31) & 1) | ((flags >> 27) & 4) | ((flags >> 29) & 2));
    flags &= 0x1fffffff;
    pkt[0xa] = 0xc;
    pkt[0xb] = high;
    if (size) memcpy(pkt + 0xc, data, size);
    struct { uint32_t size; uint8_t* data; } desc = { (uint32_t)size + 0xc, pkt };
    uint32_t dpnid = 0;
    bool send = true;
    if (to) {
        imp::EnterCriticalSection((void*)0x6d4090);
        ++g_6d40a8;
        uint32_t crc = Crc32String(to, 1);
        send = false;
        for (uint8_t* n = *g_6d408c; n; n = F<uint8_t*>(n, 0)) {
            if (F<uint32_t>(n, 0x48) == crc) {
                dpnid = F<uint32_t>(n, 4);
                send = true;
                break;
            }
        }
        --g_6d40a8;
        imp::LeaveCriticalSection((void*)0x6d4090);
    }
    if (send) {
        if (serial)
            flags = (flags & ~2u) | 8;
        else if (!(flags & 8))
            flags |= 0x12;
        g_6d4104 += desc.size;
        void* peer = F<void*>(F<uint8_t*>(net, 0x18e), 8);
        uint32_t handle = (uint32_t)(uintptr_t)net;
        (*(int32_t(__stdcall**)(void*, uint32_t, void*, uint32_t, uint32_t, void*, uint32_t*, uint32_t))(*(uint8_t**)peer + 0x1c))(
            peer, dpnid, &desc, 1, timeout, nullptr, &handle, flags);
    }
    if (pkt != local) crt::operator_delete(pkt);
    return dpnid != 0;
}
RT3_REPLACE(0x521dc0, Net_SendTo);

// 0x521f90. Refreshes the stored surface description (+0x2698, 0x5c bytes) of
// texture key from the device (method 0x88); lock == 1 takes the lock around it.
// Whether it succeeded.
RT3_NOINLINE int32_t __fastcall Textures_QueryDesc(uint8_t* e, int, int32_t key, int32_t lock) {
    int32_t ok = 0;
    if (lock == 1) {
        Enter(0x6d4090, imp::EnterCriticalSection);
        ++g_6d40a8;
    }
    for (uint8_t* n = *g_6d408c; n; n = F<uint8_t*>(n, 0))
        if (F<int32_t>(n, 4) == key) {
            uint32_t desc[0x17];
            memset(desc, 0, sizeof desc);
            desc[0] = 0x5c;
            void* dev = F<void*>(F<uint8_t*>(e, 0x18e), 8);
            if (((HRESULT(__stdcall*)(void*, int32_t, uint32_t*, int32_t))(*(void***)dev)[0x88 / 4])(dev, key, desc, 0) >= 0) {
                memcpy(n + 0x2698, desc, sizeof desc);
                ok = 1;
            }
            break;
        }
    if (lock == 1) {
        --g_6d40a8;
        Enter(0x6d4090, imp::LeaveCriticalSection);
    }
    return ok;
}
RT3_REPLACE(0x521f90, Textures_QueryDesc);

// 0x522050. Looks up the entry for t's id (+0x18). keep == 1 returns the entry
// itself with the lock held; otherwise a copy (0x26f4 bytes at 0x6d4220) is returned
// after unlocking. Null when absent.
RT3_NOINLINE uint8_t* __stdcall Textures_Lookup(uint8_t* t, int32_t keep) {
    uint8_t* result = nullptr;
    Enter(0x6d4090, imp::EnterCriticalSection);
    ++g_6d40a8;
    for (uint8_t* n = *g_6d408c; n; n = F<uint8_t*>(n, 0))
        if (F<int32_t>(n, 4) == F<int32_t>(t, 0x18)) {
            if (keep == 1) return n;
            memcpy((void*)0x6d4220, n, 0x9bd * 4);
            result = (uint8_t*)0x6d4220;
            break;
        }
    --g_6d40a8;
    Enter(0x6d4090, imp::LeaveCriticalSection);
    return result;
}
RT3_REPLACE(0x522050, Textures_Lookup);

// 0x5220d0. A command node (0x28 bytes, +0x20 cleared; all zeroed when clear == 1)
// from the pool, locked when lock == 1.
RT3_NOINLINE uint8_t* __fastcall CmdNode_New(int32_t clear, int32_t lock) {
    uint8_t* n;
    if (lock == 1) {
        Enter(0x6d406c, imp::EnterCriticalSection);
        ++g_6d4084;
        n = (uint8_t*)Pool_Take(g_6d4064);
        --g_6d4084;
        Enter(0x6d406c, imp::LeaveCriticalSection);
    } else n = (uint8_t*)Pool_Take(g_6d4064);
    if (n) {
        F<int32_t>(n, 0x20) = 0;
        if (clear == 1) memset(n, 0, 0x28);
    }
    return n;
}
RT3_REPLACE(0x5220d0, CmdNode_New);

// 0x5221a0. Empties a node queue { head, tail, count }, returning the nodes to the
// command pool 0x6d4064 (lock 0x6d406c).
void __fastcall CmdQueue_Clear(uint8_t* q) {
    Enter(0x6d406c, imp::EnterCriticalSection);
    ++g_6d4084;
    for (;;) {
        uint8_t* n = F<uint8_t*>(q, 0);
        if (!n) break;
        F<uint8_t*>(q, 0) = F<uint8_t*>(n, 0);
        if (n == F<uint8_t*>(q, 4)) F<uint8_t*>(q, 4) = F<uint8_t*>(n, 0);
        --F<int32_t>(q, 8);
        Pool_Give(g_6d4064, n);
    }
    --g_6d4084;
    Enter(0x6d406c, imp::LeaveCriticalSection);
}
RT3_REPLACE(0x5221a0, CmdQueue_Clear);

// 0x522240. Queues command (a, b, c) on every channel of the list 0x6d40b0 (each a
// queue at +4), stamped with the frame 0x6d40e4; the number queued.
RT3_NOINLINE int32_t __fastcall CmdQueue_Broadcast(int32_t a, int32_t b, int32_t c) {
    int32_t count = 0;
    Enter(0x6d406c, imp::EnterCriticalSection);
    ++g_6d4084;
    for (uint8_t* ch = *g_6d40b0; ch; ch = F<uint8_t*>(ch, 0)) {
        uint8_t* n = (uint8_t*)Pool_Take(g_6d4064);
        if (!n) continue;
        F<int32_t>(n, 0x20) = 0;
        memset(n, 0, 0x28);
        F<int32_t>(n, 0x10) = a;
        F<int32_t>(n, 0x1c) = b;
        F<int32_t>(n, 8) = c;
        F<int32_t>(n, 0x18) = g_6d40e4;
        ++count;
        uint8_t* tail = F<uint8_t*>(ch, 8);
        if (tail) {
            F<uint8_t*>(n, 0) = F<uint8_t*>(tail, 0);
            F<uint8_t*>(tail, 0) = n;
        } else {
            F<uint8_t*>(n, 0) = F<uint8_t*>(ch, 4);
            F<uint8_t*>(ch, 4) = n;
        }
        ++F<int32_t>(ch, 0xc);
        F<uint8_t*>(ch, 8) = n;
    }
    --g_6d4084;
    Enter(0x6d406c, imp::LeaveCriticalSection);
    return count;
}
RT3_REPLACE(0x522240, CmdQueue_Broadcast);

// 0x522340. Opens message channel `id` in the channel list 0x6d40b0 unless it
// exists: the node comes from the pool 0x6d40ac (its free list when +0xc is
// set, else a new +0x10-byte block), gets an empty message list and goes to the
// front.
int32_t __stdcall Net_OpenChannel(uint8_t id) {
    for (uint8_t* n = *g_6d40b0; n; n = F<uint8_t*>(n, 0))
        if (n[0x1c] == id) return 0;
    uint8_t* pool = g_6d40ac;
    uint8_t* node;
    if (F<int32_t>(pool, 0xc)) {
        uint32_t* free = F<uint32_t*>(pool, 0);
        node = (uint8_t*)(uintptr_t)free[0];
        if (node) {
            free[0] = F<uint32_t>(node, 0);
            if ((uint32_t)(uintptr_t)node == free[1]) free[1] = F<uint32_t>(node, 0);
            --free[2];
        }
    } else {
        node = (uint8_t*)crt::operator_new(F<uint32_t>(pool, 0x10));
    }
    F<uint32_t>(node, 4) = 0;
    F<uint32_t>(node, 8) = 0;
    F<uint32_t>(node, 0xc) = 0;
    node[0x1c] = id;
    uint32_t* list = hook::As<uint32_t*>(g_6d40b0);
    F<uint32_t>(node, 0) = list[0];
    ++list[2];
    list[0] = (uint32_t)(uintptr_t)node;
    if (!list[1]) list[1] = (uint32_t)(uintptr_t)node;
    return 0;
}
RT3_REPLACE(0x522340, Net_OpenChannel);

// 0x5223d0. Empties the outgoing message list 0x6d4044 (under 0x6d4048; the
// nodes are not freed) and clears 0x6d40ec.
void __fastcall Net_ResetOutgoing() {
    imp::EnterCriticalSection((void*)0x6d4048);
    ++g_6d4060;
    uint32_t* list = g_6d4044;
    list[0] = 0;
    list[1] = 0;
    list[2] = 0;
    --g_6d4060;
    imp::LeaveCriticalSection((void*)0x6d4048);
    g_6d40ec = 0;
}
RT3_REPLACE(0x5223d0, Net_ResetOutgoing);

// 0x522430. Removes every outgoing message marked done (+0x118 = -1) from the
// list 0x6d4044 (under 0x6d4048), returning nodes of the preallocated block to
// the pool 0x6d4040 and deleting the rest. The unused argument is real: the
// original reuses its slot to tail-call LeaveCriticalSection (RET 4).
RT3_NOINLINE void __stdcall Net_PurgeSentMessages(int32_t) {
    imp::EnterCriticalSection((void*)0x6d4048);
    ++g_6d4060;
    for (;;) {
        uint32_t* list = g_6d4044;
        uint32_t* head = (uint32_t*)(uintptr_t)list[0];
        uint32_t* node = head;
        while (node && F<int32_t>(node, 0x118) != -1) node = (uint32_t*)(uintptr_t)node[0];
        if (!node) break;
        uint32_t* prev = head;
        while (prev && (uint32_t*)(uintptr_t)prev[0] != node) prev = (uint32_t*)(uintptr_t)prev[0];
        if (prev) {
            prev[0] = node[0];
            node[0] = 0;
            --list[2];
            if ((uint32_t)(uintptr_t)node == list[1]) list[1] = (uint32_t)(uintptr_t)prev;
        } else if (uint32_t* first = (uint32_t*)(uintptr_t)list[0]) {
            list[0] = first[0];
            if ((uint32_t)(uintptr_t)first == list[1]) list[1] = list[0];
            --list[2];
        }
        uint8_t* pool = hook::As<uint8_t*>(g_6d4040);
        if (F<int32_t>(pool, 0xc) && (uintptr_t)node >= F<uint32_t>(pool, 4) && (uintptr_t)node < F<uint32_t>(pool, 8)) {
            uint32_t* free = F<uint32_t*>(pool, 0);
            node[0] = free[0];
            ++free[2];
            free[0] = (uint32_t)(uintptr_t)node;
            if (!free[1]) free[1] = (uint32_t)(uintptr_t)node;
        } else {
            crt::operator_delete(node);
        }
    }
    --g_6d4060;
    imp::LeaveCriticalSection((void*)0x6d4048);
}
RT3_REPLACE(0x522430, Net_PurgeSentMessages);

// Network player list (0x6d408c, guarded by Net_LockQueue): nodes are
// { next, id +4, name +8, ..., ping +0x54 }, 0x26f4 bytes each.

// 0x522510. Copy (into 0x6d6918) of the first player with +0x50 set, or null.
RT3_NOINLINE uint8_t* __fastcall Net_FindFlaggedPlayer() {
    uint8_t* found = nullptr;
    uint8_t** q = (uint8_t**)(uintptr_t)Net_LockQueue();
    for (uint8_t* n = q[0]; n; n = F<uint8_t*>(n, 0)) {
        if (F<int32_t>(n, 0x50)) {
            memcpy((void*)0x6d6918, n, 0x9bd * 4);
            found = (uint8_t*)0x6d6918;
            break;
        }
    }
    Net_UnlockQueue();
    return found;
}
RT3_REPLACE(0x522510, Net_FindFlaggedPlayer);

// 0x522580. Name of player `id` (in the buffer 0x6d9010, empty if none).
RT3_NOINLINE char* __stdcall Net_PlayerNameById(int32_t id) {
    char* out = (char*)0x6d9010;
    out[0] = 0;
    uint8_t** q = (uint8_t**)(uintptr_t)Net_LockQueue();
    for (uint8_t* n = q[0]; n; n = F<uint8_t*>(n, 0)) {
        if (F<int32_t>(n, 4) == id) {
            strcpy(out, (const char*)(n + 8));
            break;
        }
    }
    Net_UnlockQueue();
    return out;
}
RT3_REPLACE(0x522580, Net_PlayerNameById);

// 0x522600. Id of the player called `name` (case-insensitive), 0 if none.
int32_t __stdcall Net_PlayerIdByName(const char* name) {
    int32_t id = 0;
    uint8_t** q = (uint8_t**)(uintptr_t)Net_LockQueue();
    for (uint8_t* n = q[0]; n; n = F<uint8_t*>(n, 0)) {
        if (!crt::stricmp(name, (const char*)(n + 8))) {
            id = F<int32_t>(n, 4);
            break;
        }
    }
    Net_UnlockQueue();
    return id;
}
RT3_REPLACE(0x522600, Net_PlayerIdByName);

// 0x522670 (thread). LAN game listener for ctx (stop flag +0x10): lowers its
// priority, waits until the game list (0x6d4044, pool 0x6d4040, lock 0x6d4048)
// exists and empties it, then binds a UDP socket (0x6d40e8) on port 1234 and
// until stopped (+0x10 or 0x6d40ec) reads "name;address;port" announcements
// (QUIRK: the delimiter set also holds the address length's low byte) and
// adds each new game to the list with its arrival time. Returns 1 when the
// bind fails, 0 when stopped before it, else 0xff after closing the socket.
int32_t __fastcall Lan_ListenThread(uint8_t* ctx) {
    ((int(__stdcall*)(uint32_t, int))imp::SetThreadPriority)(F<uint32_t>(ctx, 4), -15);
    struct {
        char delim[4];  // L+3.. (with the length after it)
        int32_t fromLen;
    } d;
    d.fromLen = 0x10;
    int32_t cleared = 0;
    do {
        if (F<int32_t>(ctx, 0x10)) return 0;
        if (hook::As<void*>(g_6d4044) && g_6d4040) {
            Lock456(imp::EnterCriticalSection, 0x6d4048);
            ++g_6d4060;
            if (uint32_t* q = g_6d4044) {
                cleared = 1;
                q[0] = 0, q[1] = 0, q[2] = 0;
            }
            --g_6d4060;
            Lock456(imp::LeaveCriticalSection, 0x6d4048);
        }
        imp::Sleep(0);
    } while (!cleared);
    g_6d40e8 = ((int32_t(__stdcall*)(int, int, int))imp::socket)(2, 2, 0);
    uint8_t addr[0x10];
    if (g_6d40e8 != -1) {
        *(uint16_t*)addr = 2;
        *(uint32_t*)(addr + 4) = imp::htonl(0);
        *(uint16_t*)(addr + 2) = imp::htons(0x4d2);
        if (imp::bind(g_6d40e8, addr, d.fromLen)) {
            imp::WSAGetLastError();
            return 1;
        }
    }
    if (!g_6d40ec) {
        char name[0x80], where[0x80], buf[0x200];
        int32_t port = 0;
        auto recvfrom_ = imp::recvfrom;
        auto strtok_ = (char*(__cdecl*)(char*, const char*))0x5a3db9;
        do {
            if (F<int32_t>(ctx, 0x10)) break;
            const int n = recvfrom_(g_6d40e8, buf, 0x1ff, 0, addr, &d.fromLen);
            if (n == -1) {
                imp::WSAGetLastError();
                continue;
            }
            if (g_6d40ec || F<int32_t>(ctx, 0x10)) break;
            buf[n] = 0;
            d.delim[3] = ';';
            char* tok = strtok_(buf, &d.delim[3]);
            if (!tok) continue;
            for (int32_t field = 0; tok; ++field, tok = strtok_(nullptr, &d.delim[3])) {
                if (field == 0)
                    strcpy(name, tok);
                else if (field == 1)
                    strcpy(where, tok);
                else if (field == 2)
                    port = ((int)crt::atol(tok));
            }
            if (!hook::As<void*>(g_6d4044) || !g_6d4040) continue;
            Lock456(imp::EnterCriticalSection, 0x6d4048);
            ++g_6d4060;
            if (hook::As<void*>(g_6d4044) && g_6d4040 && !Sounds_Has((uint8_t*)name, 1)) {
                if (uint8_t* g = (uint8_t*)FreeList_Take(hook::As<uint8_t*>(g_6d4040))) {
                    memset(g, 0, 0x11c);
                    F<int32_t>(g, 0x104) = port;
                    F<int32_t>(g, 0x118) = ClockMs();
                    strcpy((char*)g + 4, name);
                    strcpy((char*)g + 0x84, where);
                    SList_PushFront(hook::As<void**>(g_6d4044), 0, (void**)g);
                }
            }
            --g_6d4060;
            Lock456(imp::LeaveCriticalSection, 0x6d4048);
        } while (!g_6d40ec);
    }
    ((int(__stdcall*)(int32_t))imp::closesocket)(g_6d40e8);
    g_6d40ec = 0;
    g_6d40e8 = 0;
    return 0xff;
}
RT3_REPLACE(0x522670, Lan_ListenThread);

// 0x522990. Adds the DirectPlay send queues of all players (GetSendQueueInfo)
// to *msgs / *bytes; 1 if any query worked.
int32_t __fastcall Net_SendQueueTotals(uint8_t* o, int, int32_t* msgs, int32_t* bytes) {
    int32_t any = 0;
    uint8_t** q = (uint8_t**)(uintptr_t)Net_LockQueue();
    if (q) {
        for (uint8_t* n = q[0]; n; n = F<uint8_t*>(n, 0)) {
            uint32_t m = 0, b = 0;
            void* peer = F<void*>(F<uint8_t*>(o, 0x18e), 8);
            int32_t hr = (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t*, uint32_t*, uint32_t))(*(uint8_t**)peer + 0x20))(peer, F<uint32_t>(n, 4), &m, &b, 0);
            if (hr >= 0) {
                *msgs += (int32_t)m;
                *bytes += (int32_t)b;
                any = 1;
            }
        }
    }
    Net_UnlockQueue();
    return any;
}
RT3_REPLACE(0x522990, Net_SendQueueTotals);

// 0x522a40. Frees the pending list +4 (nodes and the list itself).
int32_t __fastcall Net_FreePendingList(uint8_t* o) {
    uint32_t* list = F<uint32_t*>(o, 4);
    if (!list) return 0;
    while (uint32_t* n = (uint32_t*)list[0]) {
        list[0] = n[0];
        if ((uint32_t)(uintptr_t)n == list[1]) list[1] = n[0];
        --list[2];
        crt::operator_delete(n);
    }
    crt::operator_delete(list);
    F<uint32_t*>(o, 4) = nullptr;
    return 0;
}
RT3_REPLACE(0x522a40, Net_FreePendingList);

// 0x522ab0. A player joined (message: id +4): reads its DirectPlay8 peer info
// (name, flags) and adds a player record to the list 0x6d408c (name +8, id +4,
// name hash +0x48, message tables +0x60 / +0x2580, queues +0x64 / +0x6c); the
// local player (flag 2) becomes 0x6d40e4, the host (flag 4) gets +0x50. The player
// count 0x6d4100 rises and a command 5 for it is queued. 0 when added, else -1.
RT3_NOINLINE int32_t __fastcall Net_PlayerJoined(int32_t, uint8_t* msg) {
    void* peer = g_6d40e0;
    typedef int32_t(__stdcall * GetInfo)(void*, int32_t, void*, uint32_t*, uint32_t);
    uint32_t size = 0;
    int32_t result = -1;
    if ((*(GetInfo*)(*(uint8_t**)peer + 0x54))(peer, F<int32_t>(msg, 4), nullptr, &size, 0) != (int32_t)0x80158100) return result;
    uint32_t* info = (uint32_t*)crt::operator_new(size);
    memset(info, 0, 0x18);
    info[0] = 0x18;
    peer = g_6d40e0;
    if ((*(GetInfo*)(*(uint8_t**)peer + 0x54))(peer, F<int32_t>(msg, 4), info, &size, 0) >= 0) {
        char name[0x40];
        Str_FromWide(name, (const wchar_t*)(uintptr_t)info[2], -1);
        uint32_t hash = Crc32String(name, 1);
        Enter(0x6d4090, imp::EnterCriticalSection);
        ++g_6d40a8;
        if (hook::As<void*>(g_6d408c)) {
            if (uint8_t* p = (uint8_t*)FreeList_Take(g_6d4088)) {
                memset(p, 0, 0x9bd * 4);
                strcpy((char*)p + 8, name);
                F<int32_t>(p, 4) = F<int32_t>(msg, 4);
                F<uint32_t>(p, 0x48) = hash;
                uint32_t* m = (uint32_t*)crt::operator_new(0x14);
                if (m) memset(m, 0, 0x14);
                F<uint32_t*>(p, 0x60) = m;
                Hash_Init((HashMap*)m, 0, 0x65, 0x200);
                m = (uint32_t*)crt::operator_new(0x14);
                if (m) memset(m, 0, 0x14);
                F<uint32_t*>(p, 0x2580) = m;
                Hash_Init((HashMap*)m, 0, 0x1f, 0x200);
                m = (uint32_t*)crt::operator_new(0xc);
                if (m) memset(m, 0, 0xc);
                F<uint32_t*>(p, 0x64) = m;
                F<int32_t>(p, 0x68) = 0;
                m = (uint32_t*)crt::operator_new(0x1c);
                if (m) memset(m, 0, 0x10);
                F<uint32_t*>(p, 0x6c) = m;
                Heap_Init((MaxHeap*)m, 0, 0x400);
                F<int32_t>(p, 0x70) = 0x7ffffffe;
                F<int32_t>(p, 0x74) = 0x7ffffffe;
                F<int32_t>(p, 0x5c) = 1;
                F<int32_t>(p, 0x2690) = 1;
                void** list = hook::As<void**>(g_6d408c);
                SList_InsertAfter(list, 0, (void**)list[1], (void**)p);
                if (info[5] & 2) {
                    g_6d40e4 = F<int32_t>(p, 4);
                    F<uint32_t>(p, 0x2694) = SysInfo_CpuSignature();
                    F<int32_t>(p, 0x268c) = g_6d4108;
                }
                if (info[5] & 4) F<int32_t>(p, 0x50) = 1;
                Enter(0x6d4100, imp::InterlockedIncrement);
                CmdQueue_Broadcast(5, 0, (int32_t)(uintptr_t)p);
                result = 0;
            }
        }
        --g_6d40a8;
        Enter(0x6d4090, imp::LeaveCriticalSection);
    }
    crt::operator_delete(info);
    return result;
}
RT3_REPLACE(0x522ab0, Net_PlayerJoined);

// 0x522d20. A player left the session (message: id +4, reason +0xc): the player
// record with that id (list 0x6d408c) gets the matching leave text (strings
// 0x58..0x5b, with its name +8) at +0x78, the player count 0x6d4100 drops and a
// command 6 for it is queued on every channel (count kept at +0x4c).
RT3_NOINLINE int32_t __fastcall Net_PlayerLeft(int32_t, uint8_t* msg) {
    Enter(0x6d4090, imp::EnterCriticalSection);
    ++g_6d40a8;
    uint8_t* p = *g_6d408c;
    while (p && F<int32_t>(p, 4) != F<int32_t>(msg, 4)) p = F<uint8_t*>(p, 0);
    --g_6d40a8;
    Enter(0x6d4090, imp::LeaveCriticalSection);
    if (!p) return 0;
    int32_t why = F<int32_t>(msg, 0xc);
    if (why >= 1 && why <= 4)
        FormatText((char*)p + 0x78, GetString(0x57 + why), (const char*)p + 8, 0, 0, 0, 0, 0, 0, 0);
    Enter(0x6d4100, imp::InterlockedDecrement);
    F<int32_t>(p, 0x4c) = CmdQueue_Broadcast(6, 0, (int32_t)(uintptr_t)p);
    return 0;
}
RT3_REPLACE(0x522d20, Net_PlayerLeft);

// 0x522e30. Frees a network player record's containers: the list +0x64, the
// hash +0x60 (entries freed by 0x521580), the pending-message hash +0x2580
// (lists freed by Net_FreePendingList, except for the local player, 0x6d40e4)
// and the send heap +0x6c (packets freed, nodes back to the pool 0x6d4064).
void __stdcall Net_FreePlayerRecord(uint8_t* p) {
    if (uint32_t* list = F<uint32_t*>(p, 0x64)) {
        while (uint32_t* n = (uint32_t*)(uintptr_t)list[0]) {
            list[0] = n[0];
            if ((uint32_t)(uintptr_t)n == list[1]) list[1] = n[0];
            --list[2];
            crt::operator_delete(n);
        }
        crt::operator_delete(list);
        F<uint32_t*>(p, 0x64) = nullptr;
    }
    if (HashMap* h = F<HashMap*>(p, 0x60)) {
        Hash_ForEach(h, 0, 0, (HashVisit)0x521580, nullptr);
        if (HashMap* h2 = F<HashMap*>(p, 0x60)) {
            Hash_DestroyThunk(h2);
            crt::operator_delete(h2);
            F<HashMap*>(p, 0x60) = nullptr;
        }
    }
    if (HashMap* h = F<HashMap*>(p, 0x2580)) {
        if (F<int32_t>(p, 4) != g_6d40e4) Hash_ForEach(h, 0, 0, (HashVisit)0x522a40, nullptr);
        if (HashMap* h2 = F<HashMap*>(p, 0x2580)) {
            Hash_DestroyThunk(h2);
            crt::operator_delete(h2);
            F<HashMap*>(p, 0x2580) = nullptr;
        }
    }
    if (MaxHeap* heap = F<MaxHeap*>(p, 0x6c)) {
        for (uint32_t* m; (m = (uint32_t*)(uintptr_t)Heap_PopMax(heap)) != nullptr; heap = F<MaxHeap*>(p, 0x6c)) {
            crt::operator_delete((void*)(uintptr_t)m[1]);
            Pool_Return(g_6d4064, m);
        }
        if (MaxHeap* h2 = F<MaxHeap*>(p, 0x6c)) {
            Heap_Destroy2(h2);
            crt::operator_delete(h2);
            F<MaxHeap*>(p, 0x6c) = nullptr;
        }
    }
}
RT3_REPLACE(0x522e30, Net_FreePlayerRecord);

// 0x522f70. Takes the pending messages of channel `id` (under 0x6d406c): moves
// the channel's list (+4) into its taken list (+0x10) and returns that, or null
// when the network is down, the channel is unknown or has nothing.
uint32_t* __stdcall Net_TakeChannelMessages(uint8_t id) {
    uint32_t* taken = nullptr;
    if (!hook::As<void*>(g_6d408c)) return nullptr;
    imp::EnterCriticalSection((void*)0x6d406c);
    ++g_6d4084;
    for (uint8_t* n = *g_6d40b0; n; n = F<uint8_t*>(n, 0)) {
        if (n[0x1c] != id) continue;
        if (F<uint32_t>(n, 4)) {
            taken = (uint32_t*)(n + 0x10);
            memcpy(taken, n + 4, 12);
            memset(n + 4, 0, 12);
        }
        break;
    }
    --g_6d4084;
    imp::LeaveCriticalSection((void*)0x6d406c);
    return taken;
}
RT3_REPLACE(0x522f70, Net_TakeChannelMessages);

// 0x523010. Finishes a batch of received messages (list {head, tail, count},
// nodes: +0x10 kind, +8 player, +0x14 DirectPlay buffer, +0x20 flags): a
// "player left" message (kind 6) drops a reference to the player record and,
// with the last one, unlinks it from the player list, frees it and returns it to
// the player pool; every DirectPlay buffer goes back (ReturnBuffer). Then the
// nodes are taken off the list and returned to the packet pool 0x6d4064 unless
// flag 1 says they live elsewhere.
void __fastcall Net_FinishReceived(uint8_t* net, int, uint32_t* list) {
    if (!list) return;
    auto enter = imp::EnterCriticalSection;
    auto leave = imp::LeaveCriticalSection;
    for (uint8_t* m = (uint8_t*)(uintptr_t)list[0]; m; m = F<uint8_t*>(m, 0)) {
        if (F<int32_t>(m, 0x10) == 6) {
            enter((void*)0x6d406c);
            ++g_6d4084;
            enter((void*)0x6d4090);
            ++g_6d40a8;
            uint32_t* p = F<uint32_t*>(m, 8);
            if (--F<int32_t>(p, 0x4c) == 0) {
                uint32_t* players = hook::As<uint32_t*>(g_6d408c);
                SList_Unlink(&players[0], &players[1], (int32_t*)&players[2], p);
                Net_FreePlayerRecord((uint8_t*)p);
                Pool_Return(g_6d4088, p);
            }
            --g_6d40a8;
            leave((void*)0x6d4090);
            --g_6d4084;
            leave((void*)0x6d406c);
        }
        if (uint32_t buffer = F<uint32_t>(m, 0x14)) {
            void* peer = F<void*>(F<uint8_t*>(net, 0x18e), 8);
            (*(int32_t(__stdcall**)(void*, uint32_t, uint32_t))(*(uint8_t**)peer + 0x6c))(peer, buffer, 0);
        }
    }
    enter((void*)0x6d406c);
    ++g_6d4084;
    uint32_t* node = (uint32_t*)(uintptr_t)list[0];
    if (node) {
        list[0] = node[0];
        if ((uint32_t)(uintptr_t)node == list[1]) list[1] = list[0];
        --list[2];
    }
    for (;;) {
        if (!(((uint8_t*)node)[0x20] & 1)) Pool_Return(g_6d4064, node);
        node = (uint32_t*)(uintptr_t)list[0];
        if (!node) break;
        list[0] = node[0];
        if ((uint32_t)(uintptr_t)node == list[1]) list[1] = list[0];
        --list[2];
    }
    --g_6d4084;
    leave((void*)0x6d406c);
}
RT3_REPLACE(0x523010, Net_FinishReceived);

// 0x523200. Tears the network layer's shared state down: stops the receive
// callbacks (0x6d40f4, waiting for 0x6d40f0 to drain), detaches the DirectPlay
// object, frees every player record and the pools / lists set up by
// Net_InitShared, and clears the session globals.
int32_t __fastcall Net_ShutdownShared(uint8_t* net) {
    if (!hook::As<void*>(g_6d408c)) return 0;
    g_6d40f4 = 1;
    while (g_6d40f0 > 0) imp::Sleep(0);
    if (F<uint32_t>(net, 0x18e)) {
        Set_d973b0(0);
        F<uint32_t>(net, 0x18e) = 0;
    }
    for (uint8_t* n = *g_6d408c; n; n = F<uint8_t*>(n, 0)) Net_FreePlayerRecord(n);
    auto freeList = [](uint32_t at) {
        if (void* l = hook::Global<void*>(at)) {
            crt::operator_delete(l);
            hook::Global<void*>(at) = nullptr;
        }
    };
    auto freePool = [](uint32_t at) {
        if (NodePool* p = hook::Global<NodePool*>(at)) {
            Pool_Free(p);
            crt::operator_delete(p);
            hook::Global<NodePool*>(at) = nullptr;
        }
    };
    freeList(0x6d408c);
    freePool(0x6d4088);
    freeList(0x6d4068);
    freePool(0x6d4064);
    freeList(0x6d4044);
    freePool(0x6d4040);
    freeList(0x6d40b0);
    freePool(0x6d40ac);
    g_6d4100 = 0;
    g_6d40dc = 0;
    hook::As<int32_t>(g_6d40e0) = 0;
    g_6d40e4 = 0;
    F<int32_t>(net, 0x212) = 0;
    F<int32_t>(net, 0x216) = 0;
    return 1;
}
RT3_REPLACE(0x523200, Net_ShutdownShared);

// 0x523380 (DirectPlay message callback). Under the session lock 0x6d40f0
// (nothing once shut down, 0x6d40f4): player create / destroy (0xffff0007 /
// 0xffff0009); indicate connect 0xffff000e refuses (reply code 0 / 10 at
// 0x6d40d8) a duplicate name or a closed game (0x6d4110); connect complete
// 0xffff0005 broadcasts its outcome (7 / 9 for the two failure codes, 3 on
// success, else the reply's first dword or 2 / 8); terminate 0xffff000d
// marks the player (+0x50) and broadcasts 4, flagging 0x6d40dc for this one;
// session lost 0xffff0016 sets 0x6d4114; receive 0xffff0011 queues the data
// to its tagged engine queue (returning 0x15800e, pending).
int32_t __fastcall Net_HandleMessage(int32_t ctx, uint32_t type, uint8_t* msg) {
    if (g_6d40f4) return 0;
    Lock456(imp::InterlockedIncrement, 0x6d40f0);
    int32_t r = 0;
    switch (type) {
    case 0xffff0007:
        r = Net_PlayerJoined(ctx, msg);
        break;
    case 0xffff0009:
        r = Net_PlayerLeft(ctx, msg);
        break;
    case 0xffff000e:
        Lock456(imp::EnterCriticalSection, 0x6d4090);
        ++g_6d40a8;
        if (F<const char*>(msg, 4) && Net_FindByName(F<const char*>(msg, 4))) {
            g_6d40d8 = 0;
            F<int32_t>(msg, 0x10) = 4;
            F<uint32_t>(msg, 0xc) = 0x6d40d8;
            r = 1;
        }
        if (g_6d4110) {
            g_6d40d8 = 0xa;
            F<int32_t>(msg, 0x10) = 4;
            F<uint32_t>(msg, 0xc) = 0x6d40d8;
            r = 1;
        }
        --g_6d40a8;
        Lock456(imp::LeaveCriticalSection, 0x6d4090);
        break;
    case 0xffff0005: {
        const uint32_t code = F<uint32_t>(msg, 0xc);
        int32_t what;
        if (code == 0x80158410)
            what = 7;
        else if (code == 0x80158610)
            what = 9;
        else if (code == 0)
            what = 3;
        else if (const int32_t* reply = F<const int32_t*>(msg, 0x10))
            what = *reply ? *reply : 8;
        else
            what = 2;
        CmdQueue_Broadcast(what, 0, 0);
        break;
    }
    case 0xffff000d: {
        Lock456(imp::EnterCriticalSection, 0x6d4090);
        ++g_6d40a8;
        uint8_t* p = Net_FindById(F<int32_t>(msg, 4));
        F<int32_t>(p, 0x50) = 1;
        CmdQueue_Broadcast(4, 0, (int32_t)(uintptr_t)p);
        --g_6d40a8;
        Lock456(imp::LeaveCriticalSection, 0x6d4090);
        if (F<int32_t>(msg, 4) == g_6d40e4) g_6d40dc = 1;
        break;
    }
    case 0xffff0016:
        g_6d4114 = 0xa;
        break;
    case 0xffff0011: {
        Lock456(imp::EnterCriticalSection, 0x6d406c);
        ++g_6d4084;
        uint8_t* data = F<uint8_t*>(msg, 0xc);
        if (uint8_t* e = Engine_FindTagged((char)data[8])) {
            if (uint8_t* n = CmdNode_New(1, 0)) {
                F<uint8_t*>(n, 4) = data;
                F<uint32_t>(n, 8) = F<uint32_t>(msg, 0xc) + data[0xa];
                F<int32_t>(n, 0xc) = F<int32_t>(msg, 0x10) - data[0xa];
                F<uint32_t>(n, 0x18) = F<uint32_t>(msg, 4);
                F<uint32_t>(n, 0x14) = F<uint32_t>(msg, 0x14);
                SList_InsertAfter((void**)(e + 4), 0, F<void**>(e, 8), (void**)n);
                r = 0x15800e;
            }
        }
        --g_6d4084;
        Lock456(imp::LeaveCriticalSection, 0x6d406c);
        break;
    }
    }
    Lock456(imp::InterlockedDecrement, 0x6d40f0);
    return r;
}
RT3_REPLACE(0x523380, Net_HandleMessage);

// 0x5236a0. Sets up the network layer's shared state once (no player list
// 0x6d408c yet): the DirectPlay object (0xd973b4 -> +0x18e), the four critical
// sections, the node pools and their lists (players 0x26f4 x 4, packets 0x28 x
// 0x2000, outgoing 0x11c x 0x20, channels 0x1d x 0x100), opens the broadcast
// channel 0xff and installs the message handler 0x523380.
RT3_NOINLINE int32_t __fastcall Net_InitShared(uint8_t* net) {
    if (hook::As<void*>(g_6d408c)) return 0;
    F<uint32_t>(net, 0x18e) = hook::As<uint32_t>(g_d973b4);
    memset((void*)0x6d4088, 0, 9 * 4);
    memset((void*)0x6d4064, 0, 9 * 4);
    memset((void*)0x6d4040, 0, 9 * 4);
    memset((void*)0x6d40ac, 0, 9 * 4);
    g_6d40f0 = 0;
    g_6d40f4 = 0;
    auto init = imp::InitializeCriticalSection;
    init((void*)0x6d4090);
    init((void*)0x6d406c);
    init((void*)0x6d4048);
    init((void*)0x6d40b4);
    auto newPool = [](uint32_t poolAt, uint32_t listAt, int32_t size, int32_t count) {
        uint32_t* p = (uint32_t*)crt::operator_new(0x1c);
        if (p) memset(p, 0, 0x1c);
        hook::Global<uint32_t*>(poolAt) = p;
        Pool_Init((NodePool*)p, 0, nullptr, size, count, 0);
        uint32_t* l = (uint32_t*)crt::operator_new(0xc);
        if (l) memset(l, 0, 0xc);
        hook::Global<uint32_t*>(listAt) = l;
    };
    newPool(0x6d4088, 0x6d408c, 0x26f4, 4);
    newPool(0x6d4064, 0x6d4068, 0x28, 0x2000);
    newPool(0x6d4040, 0x6d4044, 0x11c, 0x20);
    newPool(0x6d40ac, 0x6d40b0, 0x1d, 0x100);
    memset(net + 0x21a, 0, 0x10 * 4);
    Net_OpenChannel(0xff);
    Set_d973b0(0x523380);
    g_6d4110 = 0;
    g_6d4114 = 0;
    return 1;
}
RT3_REPLACE(0x5236a0, Net_InitShared);

// 0x5238e0. Hosts game (named with the "password" suffix, string 0xdba, when
// password is given; the name kept at +0x192) for name as host player, timeout
// ms, up to maxPlayers (+0x212, default 4) players (DirectPlay8 Peer Host on the
// device address +0x18e +0xc). 1 when hosting (0x6d40dc set, command 4 queued).
RT3_NOINLINE int32_t __fastcall Net_Host(uint8_t* o, int, const char* game, const char* name, const char* password, int32_t ms, int32_t maxPlayers) {
    Net_InitShared(o);
    if (!hook::As<void*>(g_6d408c)) return 0;
    struct {
        int32_t caps;
        uint32_t desc[0x12];
        char title[0x100];
        wchar_t pass[0x42];
        wchar_t wtitle[0x7e];
    } l;
    if (!password) strcpy(l.title, game);
    else crt::sprintf(l.title, (const char*)0x5c9d08, game, GetString(0xdba));
    if (Net_SetPlayerName(o, 0, name) != 1) return 0;
    if (Sound_SetBufferMs(o, 0, F<void*>(F<uint8_t*>(o, 0x18e), 0xc), ms) != 1) return 0;
    void* dev = F<void*>(F<uint8_t*>(o, 0x18e), 0xc);
    l.caps = 0;
    (*(int32_t(__stdcall**)(void*, const void*, void*, int32_t, int32_t))(*(uint8_t**)dev + 0x48))(dev, (const void*)0x5dcf9c, &l.caps, 4, 2);
    F<int32_t>(o, 0x212) = maxPlayers ? maxPlayers : 4;
    Str_ToWide(l.wtitle, l.title, -1);
    strcpy((char*)o + 0x192, l.title);
    memset(l.desc, 0, sizeof l.desc);
    for (int k = 0; k < 4; ++k) l.desc[6 + k] = ((const uint32_t*)0x6257e8)[k];
    l.desc[0] = 0x48;
    l.desc[0xc] = (uint32_t)(uintptr_t)l.wtitle;
    l.desc[0xa] = F<uint32_t>(o, 0x212);
    l.desc[1] = 0x44;
    if (password) {
        Str_ToWide(l.pass, password, -1);
        strcpy((char*)o + 0x21a, password);
        l.desc[0xd] = (uint32_t)(uintptr_t)l.pass;
        l.desc[1] |= 0x80;
    }
    uint8_t* d = F<uint8_t*>(o, 0x18e);
    void* peer = F<void*>(d, 8);
    int32_t hr = (*(int32_t(__stdcall**)(void*, void*, void*, uint32_t, void*, void*, void*, uint32_t))(*(uint8_t**)peer + 0x24))(
        peer, l.desc, d + 0xc, 1, nullptr, nullptr, nullptr, 0);
    if (hr < 0) return 0;
    g_6d40dc = 1;
    CmdQueue_Broadcast(4, 0, 0);
    return 1;
}
RT3_REPLACE(0x5238e0, Net_Host);

// 0x523af0. Joins the game at host as name (DirectPlay8 Peer Connect on
// +0x18e: peer +8, device address +0xc, host address +0x10), with password (kept
// at +0x21a) when given and timeout ms on both addresses. Returns the Connect
// result (its async handle is queued as command 1), or DPNERR_GENERIC 0x80158480
// when the session isn't ready, a connection is up (0x6d40dc) or a setup step fails.
RT3_NOINLINE int32_t __fastcall Net_Connect(uint8_t* o, int, const char* host, const char* name, const char* password, int32_t ms) {
    Net_InitShared(o);
    if (!hook::As<void*>(g_6d408c) || g_6d40dc) return (int32_t)0x80158480;
    if (Net_SetPlayerName(o, 0, name) != 1) return (int32_t)0x80158480;
    if (DpAddress_AddString(F<void*>(F<uint8_t*>(o, 0x18e), 0x10), host) != 1) return (int32_t)0x80158480;
    if (Sound_SetBufferMs(o, 0, F<void*>(F<uint8_t*>(o, 0x18e), 0xc), ms) != 1) return (int32_t)0x80158480;
    if (Sound_SetBufferMs(o, 0, F<void*>(F<uint8_t*>(o, 0x18e), 0x10), ms) != 1) return (int32_t)0x80158480;
    struct {
        uint32_t handle;
        int32_t caps;
        uint32_t desc[0x12];
        uint8_t guid[0x10];
        char user[0x40];
        wchar_t pass[0x40];
    } l;
    void* dev = F<void*>(F<uint8_t*>(o, 0x18e), 0xc);
    l.caps = 0;
    (*(int32_t(__stdcall**)(void*, const void*, void*, int32_t, int32_t))(*(uint8_t**)dev + 0x48))(dev, (const void*)0x5dcf9c, &l.caps, 4, 2);
    memset(l.desc, 0, sizeof l.desc);
    l.desc[0] = 0x48;
    const uint32_t* g = Net_GetGuid((uint32_t*)l.guid);
    l.desc[6] = g[0];
    l.desc[7] = g[1];
    l.desc[8] = g[2];
    l.desc[9] = g[3];
    l.desc[1] = 0x44;
    if (password) {
        Str_ToWide(l.pass, password, -1);
        strcpy((char*)o + 0x21a, password);
        l.desc[0xd] = (uint32_t)(uintptr_t)l.pass;
        l.desc[1] |= 0x80;
    }
    memset(l.user, 0, sizeof l.user);
    strcpy(l.user, name);
    uint8_t* d = F<uint8_t*>(o, 0x18e);
    void* peer = F<void*>(d, 8);
    int32_t hr = (*(int32_t(__stdcall**)(void*, void*, void*, void*, void*, void*, void*, uint32_t, void*, void*, uint32_t*, uint32_t))(
        *(uint8_t**)peer + 0x18))(peer, l.desc, F<void*>(d, 0x10), F<void*>(d, 0xc), nullptr, nullptr, l.user, 0x40, nullptr, nullptr, &l.handle, 0);
    if (hr >= 0) CmdQueue_Broadcast(1, (int32_t)l.handle, 0);
    return hr;
}
RT3_REPLACE(0x523af0, Net_Connect);

// 0x523cf0. Starts a batch of `count` vertices: appended after the previous batch
// while it still fits under 65000, otherwise the buffer is locked from the start.
void* __fastcall Scene_BeginBatch(uint8_t* s, int, int32_t count) {
    int32_t first = F<int32_t>(s, 0x71);
    if (first != -1 && first <= 0xfde8 - count) {
        F<int32_t>(s, 0x75) = first;
        return Render_AppendVb2647(F<uint8_t*>(s, 0x2d), 0, first, first + count);
    }
    F<int32_t>(s, 0x71) = 0;
    F<int32_t>(s, 0x75) = 0;
    return Render_LockVb2647(F<uint8_t*>(s, 0x2d), 0, 0, count);
}
RT3_REPLACE(0x523cf0, Scene_BeginBatch);

// 0x523d30. Ends the batch; with `draw` its triangles (+0x75..+0x71) are drawn.
void __fastcall Scene_EndBatch(uint8_t* s, int, uint8_t draw) {
    Render_UnlockVb2647(F<uint8_t*>(s, 0x2d));
    if (draw && F<int32_t>(s, 0x71) > F<int32_t>(s, 0x75)) Render_DrawTriangles(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0x75), F<int32_t>(s, 0x71));
}
RT3_REPLACE(0x523d30, Scene_EndBatch);

// 0x523d60. Effect intensity +0x15, clamped to 0..255.
void __fastcall Fx_SetIntensity(uint8_t* o, int, int32_t v) {
    if (v > 0xff) v = 0xff;
    else if (v < 0) v = 0;
    F<int32_t>(o, 0x15) = v;
}
RT3_REPLACE(0x523d60, Fx_SetIntensity);

// 0x523d90. Applies the scene's lighting: sun, time of day (+0xd) with the
// effect intensity, filter states, world transform and the shade mode of the
// settings +0x29 (+0x23).
RT3_NOINLINE void __fastcall Scene_ApplyLighting(uint8_t* s) {
    Render_LightingAndSun(F<uint8_t*>(s, 0x2d));
    int32_t i = Fx_Intensity(s);
    Renderer_SetTimeOfDay(F<uint8_t*>(s, 0x2d), 0, F<int32_t>(s, 0xd), i);
    Render_ApplyFilterStates(F<uint8_t*>(s, 0x2d));
    Render_ResetWorld(F<uint8_t*>(s, 0x2d));
    Render_SetShadeMode(F<uint8_t*>(s, 0x2d), 0, F<uint8_t*>(s, 0x29)[0x23]);
}
RT3_REPLACE(0x523d90, Scene_ApplyLighting);

// 0x523de0 / 0x523df0. Plain setters.
RT3_NOINLINE void __fastcall Scene_Set372c(uint8_t* s, int, uint32_t v) { F<uint32_t>(s, 0x372c) = v; }
RT3_REPLACE(0x523de0, Scene_Set372c);

RT3_NOINLINE void __fastcall Scene_Set3720(uint8_t* s, int, uint32_t a, uint32_t b) {
    F<uint32_t>(s, 0x3720) = a;
    F<uint32_t>(s, 0x3724) = b;
}
RT3_REPLACE(0x523df0, Scene_Set3720);

// Scene view object (renderer at +0x2d): busy depth +0x3741, batch vertex range
// +0x75..+0x71 in the shared vertex buffer.

// 0x523e10 / 0x523e20. Nests / unnests the busy depth (never below 0).
RT3_NOINLINE void __fastcall Scene_PushBusy(uint8_t* s) { ++F<int32_t>(s, 0x3741); }
RT3_REPLACE(0x523e10, Scene_PushBusy);

RT3_NOINLINE void __fastcall Scene_PopBusy(uint8_t* s) {
    if (--F<int32_t>(s, 0x3741) < 0) F<int32_t>(s, 0x3741) = 0;
}
RT3_REPLACE(0x523e20, Scene_PopBusy);

// 0x523e40. Sets the three redraw flags (+0x3747..+0x3749) and the renderer's
// view offset.
RT3_NOINLINE void __fastcall Scene_SetViewOffset(uint8_t* s, int, uint32_t x, uint32_t y) {
    s[0x3747] = 1;
    s[0x3748] = 1;
    s[0x3749] = 1;
    Render_SetViewOffset(F<uint8_t*>(s, 0x2d), 0, x, y);
}
RT3_REPLACE(0x523e40, Scene_SetViewOffset);

// 0x523e60. Marches a ray from `pos` along `dir` (normalized, steps of 0x5c8940)
// over the terrain for up to 511 steps or until it leaves the map. Returns how
// much the terrain blocks it (0..255, from how close it passes below a height
// margin that grows with the distance); 255 and the hit point when it goes into
// the ground. `pos` ends at the last point visited.
int32_t __stdcall Terrain_RayOcclusion(uint8_t* t, float* pos, const float* dir) {
    float x = pos[0], y = pos[1], z = pos[2];
    double dx = dir[0], dy = dir[1];
    float dzf = dir[2];
    double len = sqrt((double)dzf * dzf + dy * dy + dx * dx);
    float inv = (float)(Fl(0x5c85b4) / len);
    float sx = (float)((double)inv * dx);
    float sy = (float)((double)inv * dy);
    float sz = (float)((double)inv * dzf);
    int32_t maxX = Terrain_Rows(t) * 0x5c;
    int32_t maxZ = Terrain_Cols(t) * 0x5c;
    Terrain_HeightAt(t, 0, x, z);
    int32_t block = 0;
    sx = (float)((double)sx * Fl(0x5c8940));
    sy = (float)((double)sy * Fl(0x5c8940));
    sz = (float)((double)sz * Fl(0x5c8940));
    for (int32_t step = 0x21; step <= 0x220; ++step) {
        x = (float)((double)sx + x);
        y = (float)((double)sy + y);
        z = (float)((double)sz + z);
        if (x < Fl(0x5c8598) || (double)maxX < x || z < Fl(0x5c8598) || (double)maxZ < z) break;
        double h = Terrain_HeightAt(t, 0, x, z);
        float above = (float)((double)y - h);
        float margin = (float)step;
        if (above < margin) {
            if (above <= Fl(0x5c8598)) {
                y = (float)h;
                block = 0xff;
                break;
            }
            double v = ((double)margin - above) / margin * D(0x5ca110);
            if (v > (double)block) block = Ftol(v);
        }
    }
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    return block;
}
RT3_REPLACE(0x523e60, Terrain_RayOcclusion);

}  // namespace rt3
