// Controls.cpp - RT3.exe 0x55e000-0x564000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "core/stream.h"
#include "hook/keepregs.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

struct HWND__;

typedef HWND__* HWND;

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Texture_Destroy(uint8_t* t);
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
const char* __fastcall GetString(int32_t id);
int32_t __fastcall TreeView_IsHidden(uint8_t* w, int, int32_t i);
void __fastcall String_Assign(char** s, const char* src);
uint32_t IsEnglish();
int32_t __fastcall TreeView_NodeOfRow(uint8_t* w, int, int32_t row);
void __fastcall Channel_Command(uint8_t* c, int, uint8_t cmd, int32_t count);
struct PtrListNode;
int32_t __fastcall Terrain_Rows(uint8_t* r);
int32_t __fastcall Terrain_Cols(uint8_t* r);
PtrList* __fastcall Terrain_Cell1671(uint8_t* r, int, int32_t x, int32_t y);
PtrList* __fastcall Terrain_Cell1679(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall Scrollbar_SetPos(uint8_t* w, int, uint32_t v);
void __fastcall ListView_RefreshVisible(uint8_t* tv);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall FreeAndNull(void** p);
int32_t ClockMs();
void __fastcall Engine_SaveConfig(uint8_t* e);
void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepB(uint8_t* s);
void __fastcall Picture_Init(uint8_t* w);
void __fastcall TreeView_Init(uint8_t* w);
void __fastcall TimerWidget_Init(uint8_t* w);
void __fastcall Button_Init(uint8_t* w);
int32_t __fastcall NameTable_ReadBlock(uint8_t* t, int, int32_t, void** out);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
void __fastcall Window_AddChild(uint8_t* win, int, uint8_t* w, int32_t afterId);
void __fastcall String_Free(char** s);
void __fastcall HandleTable_ReleaseId(uint8_t* t, int, int32_t id);
void __fastcall HandleTable_Release(uint8_t* t, int, int32_t h);
int32_t __fastcall Widget_IsVisible(uint8_t* w);
void __fastcall PtrList_Destroy(PtrList* l);
uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Render_RestoreTarget(uint8_t* r);
int32_t __fastcall HandleTable_Acquire(uint8_t* t, int, uint16_t id);
void __fastcall Textures_ReleaseTargets();
void __fastcall Nop565c10();
void __fastcall VisitGrid_Free(uint8_t* g);
void ReleaseResourceD9399c();
void ReleaseResourceD93990();
void ReleaseGlobalResource();
void ReleaseResourceD97ab0();
void __fastcall Render_SaveAndShutdown(uint8_t* r);
int32_t __fastcall Widget_Send(uint8_t* w, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e);
uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);

void __fastcall TextLink_DrawPlain(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align,
                                   int32_t valign, uint32_t flags, const char* tip, int32_t id);

void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);
void __fastcall Picture_FreeTexture(uint8_t* w);
double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Widget_Destruct(uint8_t* w);
uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5, const char* a6,
                            const char* a7, const char* a8);

uint32_t __fastcall Sound_Get2c(uint8_t* s);
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
int32_t __fastcall Texture_SetPixels(uint8_t* t, int, void* pixels);
void __fastcall Movie_Stop(uint8_t* w, int, int32_t release);
void __fastcall Fatal_Exit(const char* msg);
uint8_t* __fastcall Widget_Canvas(uint8_t* g, int, int32_t w, int32_t h);
void __fastcall Movie_Tick(uint8_t* w, int, int32_t force);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
uint8_t* __fastcall Render_Construct(uint8_t* r, int, int32_t, uint8_t fullScreen);
void __fastcall View_RecordStagePresets(uint8_t* s);
int32_t __fastcall Renderer_CreateBuffers(uint8_t* r);
void __fastcall Scene_RebuildStaticBuffer(uint8_t* s);
void __fastcall Meshes_InvalidateAll();
void __fastcall Input_PushEvent(uint8_t* in, int, const uint32_t* ev);
int32_t __fastcall Widget_HandleEvent(uint8_t* w, int, uint8_t* e);
uint8_t* __fastcall Windows_FindChild(uint8_t* o, int, int32_t id);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
void __fastcall Widget_OnHover(uint8_t* w, int, float x, float y, int32_t moved, int32_t track);
int32_t __fastcall Widget_IsEnabled(uint8_t* w);
uint8_t* __fastcall TextureCache_Get(uint8_t* t, int, const char* name, int32_t start, int32_t count, uint8_t smallTex);
void __fastcall Engine_SetRenderer(void* e, int, void* v);
int32_t __fastcall Engine_Get1146f6(void* e);
HWND __fastcall Engine_Window(void* e);
void __fastcall Texture_SetD9399c(const char* name);
void __fastcall Texture_SetD93990(const char* name);
void __fastcall Texture_SetD97ab0(const char* name);
uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
uint32_t __fastcall Obj_Get1c(uint8_t* o);
void __fastcall Texture_SetCcba20(const char* name);
uint8_t* __fastcall Object_InitFromCollision(uint8_t* o, int, const char* name, int32_t);
void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
void __fastcall Scene_ApplyLighting(uint8_t* s);
void __fastcall Render_CullCcw(uint8_t* r);
double __fastcall Render_Benchmark(uint8_t* r);
int32_t __fastcall TreeView_RowOfNode(uint8_t* w, int, int32_t n);
void __fastcall ListBox_Select(uint8_t* l, int, int32_t node, int32_t sound);
void __fastcall List_SyncScrollbar(uint8_t* l);
void* __fastcall Engine_Input(void* e);
void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
double __fastcall List_RowHeight(uint8_t* w);
void __fastcall TreeView_ClosePopup(uint8_t* w);
void __fastcall Dropdown_Open(uint8_t* c);
int32_t __fastcall TreeView_Depth(uint8_t* w, int, int32_t i);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

void __fastcall Scene_CreateRenderer(uint8_t* s);
double __fastcall Widget_Pan_Impl(uint8_t* w);
void __fastcall Dropdown_Select(uint8_t* d, int, int32_t idx, int32_t sound, int32_t renotify);
void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
RT3_NOINLINE void* __fastcall Engine_Renderer(void* e);
RT3_NOINLINE void* __fastcall Engine_Input(void* e);
RT3_NOINLINE void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
RT3_NOINLINE uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
RT3_NOINLINE int32_t __fastcall Ui_ShowTooltip(uint8_t* ui, int, const char* text, int32_t prio, int32_t sticky, int32_t data);
RT3_NOINLINE HWND __fastcall Engine_Window(void* e);
void __fastcall Widget_OnLeave(uint8_t* w);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
void __fastcall Movie_Open(uint8_t* w, int, int32_t index);
void __fastcall Nop558460(void*, int, int32_t, int32_t, int32_t);
void Widget_Pan();
void Tutorial_Advance();
void Fade_Step();

void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12);

void __fastcall Video_CopyFrame(uint8_t* w);
int32_t __fastcall Texture_Width(uint8_t* t);
int32_t __fastcall Texture_Height(uint8_t* t);
void __fastcall ListBox_SetSelection(uint8_t* l, int, int32_t row, int32_t sound, int32_t renotify);
void __fastcall ListBox_Clear(uint8_t* l);
void __fastcall ListBox_RemoveItem(uint8_t* l, int, int32_t i);
void __fastcall TreeView_AddRow(uint8_t* tv, int, const uint32_t* row);
int32_t __fastcall ListBox_OnKey(uint8_t* l, int, uint32_t* ev);
int32_t __fastcall ListBox_OnMouse(uint8_t* l, int, uint32_t* ev);
void __fastcall ListBox_AutoScroll(uint8_t* l);
void __fastcall Nop558750(void*);

// Functions of this file, in exe order.
RT3_NOINLINE void Tutorial_Advance();
RT3_NOINLINE void __fastcall SceneObj_Destructor(uint8_t* o);
RT3_NOINLINE void ReleaseGlobalResource();
RT3_NOINLINE void __fastcall Pack_Release(uint8_t* pack, int, void* obj);
RT3_NOINLINE void __fastcall Cursor_Set(uint8_t* c, int, int32_t idx, int32_t force, int32_t now);
void __fastcall Window_DeleteChild(uint8_t* win, int, uint8_t* w);
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
RT3_NOINLINE uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
RT3_NOINLINE int32_t __fastcall Texture_SetPixels(uint8_t* t, int, void* pixels);
RT3_NOINLINE uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag);
RT3_NOINLINE void __fastcall Ui_FillRect(uint32_t color, int32_t snap, float x, float y, float w, float h, uint32_t flags);
RT3_NOINLINE void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
RT3_NOINLINE double __fastcall Font_TextHeight(uint8_t* f, int, const char* text, float width, float* textWidth);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
RT3_NOINLINE void ReleaseResourceD93990();
RT3_NOINLINE void ReleaseResourceD9399c();
void __fastcall Widget_LoadBase(uint8_t* w, int, const uint8_t* rec);
RT3_NOINLINE void __fastcall Widget_Destruct(uint8_t* w);
RT3_NOINLINE uint8_t* __fastcall Widget_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE int32_t __fastcall Widget_IsVisible(uint8_t* w);
RT3_NOINLINE void __fastcall Nop558750(void*);
RT3_NOINLINE void __fastcall Widget_OnHover(uint8_t* w, int, float x, float y, int32_t moved, int32_t track);
RT3_NOINLINE void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
RT3_NOINLINE uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
RT3_NOINLINE uint8_t* __fastcall Scrollbar_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall Dropdown_Select(uint8_t* d, int, int32_t idx, int32_t sound, int32_t renotify);
RT3_NOINLINE uint8_t* __fastcall ListBox_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall Scene_Shutdown(uint8_t* s);
RT3_NOINLINE void __fastcall Scene_CreateRenderer(uint8_t* s);
RT3_NOINLINE uint8_t* __fastcall Scene_Construct(uint8_t* s);
void __fastcall Picture_Init(uint8_t* w);
RT3_NOINLINE void __fastcall Video_CopyFrame(uint8_t* w);
uint8_t* __fastcall Widget_Canvas(uint8_t* g, int, int32_t w, int32_t h);
uint8_t* __fastcall Picture_Load(uint8_t* w, int, int32_t);
RT3_NOINLINE void __fastcall Picture_FreeTexture(uint8_t* w);
RT3_NOINLINE void __fastcall Movie_Stop(uint8_t* w, int, int32_t release);
RT3_NOINLINE void __fastcall Movie_StopIfHidden(uint8_t* w);
RT3_NOINLINE void __fastcall Movie_Tick(uint8_t* w, int, int32_t force);
RT3_NOINLINE void __fastcall Movie_Open(uint8_t* w, int, int32_t index);
int32_t __fastcall MovieWidget_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall Movie_Draw(uint8_t* w);
RT3_NOINLINE uint8_t* __fastcall Picture_Destroy(uint8_t* w, int, uint32_t flags);
RT3_NOINLINE void __fastcall TreeView_Init(uint8_t* w);
RT3_NOINLINE void __fastcall List_SyncScrollbar(uint8_t* l);
RT3_NOINLINE uint32_t __fastcall TreeView_NodeData(uint8_t* w, int, int32_t i);
RT3_NOINLINE uint32_t __fastcall TreeView_SelectedData(uint8_t* w);
int32_t __fastcall TreeView_IsHidden(uint8_t* w, int, int32_t i);
int32_t __fastcall TreeView_Depth(uint8_t* w, int, int32_t i);
RT3_NOINLINE void __fastcall TreeView_ClosePopup(uint8_t* w);
RT3_NOINLINE uint32_t __fastcall TreeView_NodeValue(uint8_t* w, int, int32_t i);
uint8_t* __fastcall TreeView_Load(uint8_t* w, int, int32_t);
RT3_NOINLINE uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE double __fastcall List_RowHeight(uint8_t* w);
void __fastcall List_CreateScrollbar(uint8_t* l);
int32_t __fastcall TreeView_RowOfNode(uint8_t* w, int, int32_t n);
RT3_NOINLINE int32_t __fastcall TreeView_NodeOfRow(uint8_t* w, int, int32_t row);
RT3_NOINLINE void __fastcall List_RowPos(uint8_t* w, int, int32_t row, float* x, float* y);
RT3_NOINLINE void __fastcall TreeView_Draw(uint8_t* w, int, int32_t, int32_t, int32_t, int32_t);
void __fastcall TreeView_OnMouse(uint8_t* w, int, float x, float y, int32_t click, int32_t hover);
RT3_NOINLINE void __fastcall ListView_RefreshVisible(uint8_t* tv);
void __fastcall ListWidget_Hover(uint8_t* w, int, float x, float y, int32_t node);
RT3_NOINLINE void __fastcall ListView_Refresh(uint8_t* l, int, int32_t toEnd);
void __fastcall Dropdown_Open(uint8_t* c);
void __fastcall Thunk5615e0(void* p0);
void __fastcall TreeView_Destruct(uint8_t* w);
void __fastcall TreeView_Render(uint8_t* w);
RT3_NOINLINE void __fastcall ListBox_Select(uint8_t* l, int, int32_t node, int32_t sound);
RT3_NOINLINE void __fastcall ListBox_SetSelection(uint8_t* l, int, int32_t row, int32_t sound, int32_t renotify);
RT3_NOINLINE void __fastcall TreeView_AddRow(uint8_t* tv, int, const uint32_t* row);
uint8_t* __fastcall TreeView_DeletingDestructor(uint8_t* w, int, uint32_t flags);
RT3_NOINLINE void __fastcall ListBox_RemoveItem(uint8_t* l, int, int32_t i);
RT3_NOINLINE void __fastcall TreeView_SetExpanded(uint8_t* l, int, int32_t node, int32_t expanded);
RT3_NOINLINE int32_t __fastcall ListBox_OnKey(uint8_t* l, int, uint32_t* ev);
RT3_NOINLINE int32_t __fastcall ListBox_OnMouse(uint8_t* l, int, uint32_t* ev);
RT3_NOINLINE void __fastcall ListBox_AutoScroll(uint8_t* l);
RT3_NOINLINE void __fastcall ListBox_Clear(uint8_t* l);
int32_t __fastcall ListBox_HandleEvent(uint8_t* l, int, uint32_t* msg);
void __fastcall TimerWidget_Init(uint8_t* w);
void __fastcall Widget_DrawDebugFrames(uint8_t* w);
void __fastcall Widget_OnHoverTimed(uint8_t* w, int, float x, float y, int32_t moved, int32_t track);
void __fastcall Widget_ResetAndLeave(uint8_t* w);
int32_t __fastcall PushButton_HandleEvent(uint8_t* w, int, uint32_t* msg);
uint8_t* __fastcall TimerWidget_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall TimerWidget_Delete(uint8_t* w, int, int32_t flags);
RT3_NOINLINE uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall Reset_625a94();
RT3_NOINLINE void __fastcall TextLink_Draw(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign, uint32_t flags, const char* tip, int32_t id, uint32_t color, uint32_t hover);
RT3_NOINLINE void __fastcall TextLink_DrawPlain(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, const char* tip, int32_t id);
void __fastcall Button_Init(uint8_t* w);
void __fastcall Button_Setup(uint8_t* w);
void __fastcall TextWidget_Draw(uint8_t* w);
int32_t __fastcall Toggle_HandleEvent(uint8_t* w, int, int32_t* e);
uint8_t* __fastcall Button_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall Button_Delete(uint8_t* w, int, int32_t flags);
RT3_NOINLINE uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __stdcall Obj_CallSlot1(uint8_t* o);
int32_t __fastcall Return0_563de0(void*);
void __fastcall Thread_Stop(uint8_t* t, int, uint32_t ms);
void __fastcall Thread_Start(uint8_t* t);
void __fastcall Thread_Destruct(uint8_t* t);
void __fastcall Channels_PlayAll(uint8_t* o);
void __fastcall Widget_RemoveListNode(uint8_t* w, int, PtrListNode* n);
void __fastcall Sound_RefreshArea(uint8_t* o, int, int32_t x, int32_t y, int32_t r, uint8_t* grid);
RT3_NOINLINE void ReleaseResourceD97ab0();

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }

static inline const int32_t* DisplayMode() {
    return (const int32_t*)(F<uint8_t*>(Engine(), 0x34) + F<int32_t>(Engine(), 0x11421e) * 0x15);
}

// Tree nodes (+0x176, 32 bytes): +0 value, +8 data, +0x10 has parent, +0x14 parent, +0x18 expanded.
static inline uint8_t* Node(uint8_t* w, int32_t i) { return F<uint8_t*>(w, 0x176) + i * 32; }

// Worker thread object: handle +4, id +8, running +0xc (entry 0x563dd0).
static inline void JoinThread(uint8_t* t, uint32_t ms) {
    F<int32_t>(t, 0xc) = 0;
    Import<uint32_t(__stdcall*)(void*, uint32_t)>(0x5c81c4)(F<void*>(t, 4), ms);
    Import<int32_t(__stdcall*)(void*)>(0x5c81c8)(F<void*>(t, 4));
    F<void*>(t, 4) = nullptr;
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

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

static inline double BitsD(uint64_t b) { return *(double*)&b; }

static inline void* ExeMalloc(size_t n) { return ((void*(__cdecl*)(size_t))0x5a125d)(n); }

// Pack-loaded widgets (the constructors take an unused argument): a bare base, the
// kind's init, then an `n`-byte record read from the pack's current position.
static uint8_t* Widget_ReadRecord(uint8_t* w, int32_t n) {
    uint8_t* rec = (uint8_t*)ExeMalloc(n);
    NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, rec, n);
    Widget_LoadBase(w, 0, rec);
    return rec;
}

// Widget scalar-deleting destructors: own cleanup, the base (0x558000), then
// delete when `flags` bit 0 is set.
static uint8_t* Widget_Finish(uint8_t* w, int32_t flags) {
    Widget_Destruct(w);
    if (flags & 1) ExeDelete(w);
    return w;
}

static void FreeField(uint8_t* w, uint32_t off) {
    if (F<void*>(w, off)) Free(F<void*>(w, off));
    F<void*>(w, off) = nullptr;
}

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

// x87 distance for the lookup table: sqrt(j*j + i*i) in the original's order.
static inline double Scene_TableDistance(int32_t i, int32_t j) {
    double r;
    __asm {
        fild i
        fld st(0)
        fmulp st(1), st(0)
        fild j
        fld st(0)
        fmul st(0), st(1)
        fadd st(0), st(2)
        fsqrt
        fstp r
        fstp st(0)
        fstp st(0)
    }
    return r;
}

// A new scene object (0x206 bytes) for model / texture, or null.
static uint8_t* Scene_NewObject(const char* model, const char* tex) {
    uint8_t* o = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x206);
    return o ? SceneObj_Construct(o, 0, 0, model, tex) : nullptr;
}

// Ui_PlaySound for widget w: its pan as float bits.
static void Widget_PlaySound(uint8_t* w, int32_t id) {
    float pan = (float)((double(__fastcall*)(uint8_t*))0x5584d0)(w);
    uint32_t bits;
    memcpy(&bits, &pan, 4);
    Ui_PlaySound(id, 0, bits);
}

// 0x55e060. Shuts the scene down (when started, +1): releases its D3D object
// (+0x5d), restores the render target of the renderer (+0x2d) and the render
// targets of all textures, deletes the camera object +0x95, the sky / sun
// objects (+0x99..+0xa5, with the flare +0x3665, only when +0x9d exists), the
// object +0x364d and the visit grid +0x65, releases the global resources and
// the pack resources +0x3651..+0x3661, deletes the object lists +0x79..+0x8d
// and finally the renderer.
RT3_NOINLINE void __fastcall Scene_Shutdown(uint8_t* s) {
    if (!F<int32_t>(s, 1)) return;
    IUnknown* dev = F<IUnknown*>(s, 0x5d);
    dev->Release();
    Render_RestoreTarget(F<uint8_t*>(s, 0x2d));
    F<int32_t>(s, 1) = 0;
    Textures_ReleaseTargets();
    auto delObj = [](uint8_t* o) {
        if (o) {
            SceneObj_Destructor(o);
            ExeDelete(o);
        }
    };
    delObj(F<uint8_t*>(s, 0x95));
    if (F<uint8_t*>(s, 0x9d)) {
        delObj(F<uint8_t*>(s, 0x99));
        delObj(F<uint8_t*>(s, 0x9d));
        delObj(F<uint8_t*>(s, 0xa1));
        delObj(F<uint8_t*>(s, 0xa5));
        if (uint8_t* flare = F<uint8_t*>(s, 0x3665)) {
            Nop565c10();
            ExeDelete(flare);
        }
    }
    delObj(F<uint8_t*>(s, 0x364d));
    if (uint8_t* g = F<uint8_t*>(s, 0x65)) {
        VisitGrid_Free(g);
        ExeDelete(g);
    }
    ReleaseResourceD9399c();
    ReleaseResourceD93990();
    ReleaseGlobalResource();
    ReleaseResourceD97ab0();
    for (uint32_t off : { 0x3659u, 0x3651u, 0x3655u, 0x365du, 0x3661u })
        if (void* r = F<void*>(s, off)) Pack_Release(hook::Global<uint8_t*>(0x6d4020), 0, r);
    F<void*>(s, 0x3651) = nullptr;
    F<void*>(s, 0x3659) = nullptr;
    for (uint32_t off : { 0x7du, 0x81u, 0x85u, 0x89u, 0x8du, 0x79u }) {
        if (PtrList* l = F<PtrList*>(s, off)) {
            PtrList_Destroy(l);
            ExeDelete(l);
        }
    }
    if (uint8_t* r = F<uint8_t*>(s, 0x2d)) {
        Render_SaveAndShutdown(r);
        ExeDelete(r);
    }
}
RT3_REPLACE(0x55e060, Scene_Shutdown);

// 0x55e2b0. Creates scene s's renderer (+0x2d, windowed or full screen per
// engine +0x114226) and its buffers (fatal string 0xa0 on failure), records
// the stage presets, rebuilds the static buffer and resets the batch marks.
// (The original then jumps to the empty stub 0x55d420.)
RT3_NOINLINE void __fastcall Scene_CreateRenderer(uint8_t* s) {
    F<int32_t>(s, 5) = 0;
    F<int32_t>(s, 9) = 0;
    s[0x3747] = 1;
    s[0x3748] = 1;
    F<int32_t>(s, 0x3abb7) = 0;
    uint8_t full = hook::Global<uint8_t*>(0x6d4024)[0x114226];
    s[0x3749] = 1;
    uint8_t* r = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x3715);
    r = r ? Render_Construct(r, 0, F<int32_t>(s, 0x31), full) : nullptr;
    F<uint8_t*>(s, 0x2d) = r;
    View_RecordStagePresets(s);
    if (Renderer_CreateBuffers(F<uint8_t*>(s, 0x2d)) < 0) Fatal_Exit(GetString(0xa0));
    Scene_RebuildStaticBuffer(s);
    F<int32_t>(s, 0x71) = -1;
    F<int32_t>(s, 0x75) = -1;
    F<int32_t>(s, 0x6d) = -1;
    Meshes_InvalidateAll();
}
RT3_REPLACE(0x55e2b0, Scene_CreateRenderer);

// 0x55e370. Scene constructor: registers scene s as the engine's renderer
// (fatal 0x9e without an engine, 0x9f when one is set), resets its state,
// creates the renderer, the matrix stack (+0x5d), six object lists
// (+0x79..), the shared textures, the skybox / sun / water / cloud objects
// (+0x95.. +0xa5, +0x3665, +0x364d; the sky depends on hardware fog), fills
// the distance table 0x6d9098 (0x418 x 0x418) and clamp table 0xc33d38,
// applies the lighting and runs the benchmark when the options ask for it
// (+0x29 +0x67: the second run is kept in +0x68). Calls 0xd97968 after it.
RT3_NOINLINE uint8_t* __fastcall Scene_Construct(uint8_t* s) {
    if (!hook::Global<void*>(0x6d4024)) Fatal_Exit(GetString(0x9e));
    if (Engine_Renderer(hook::Global<void*>(0x6d4024))) Fatal_Exit(GetString(0x9f));
    F<uint8_t*>(s, 0x29) = hook::Global<uint8_t*>(0x6d4024) + 0x11468a;
    Engine_SetRenderer(hook::Global<void*>(0x6d4024), 0, s);
    F<uint32_t>(s, 5) = 0;
    F<uint32_t>(s, 9) = 0;
    F<uint32_t>(s, 0x25) = 0;
    F<uint32_t>(s, 0x22b63) = 0;
    F<uint32_t>(s, 0x22b8b) = 2;
    F<int32_t>(s, 0xd) = Engine_Get1146f6(hook::Global<void*>(0x6d4024));
    F<uint32_t>(s, 0x11) = 0;
    F<uint32_t>(s, 1) = 0;
    s[0x3740] = 0;
    F<uint32_t>(s, 0x3730) = 0;
    F<uint32_t>(s, 0x22b6b) = 1;
    F<uint32_t>(s, 0x22b6f) = 1;
    F<HWND>(s, 0x31) = Engine_Window(hook::Global<void*>(0x6d4024));
    F<uint32_t>(s, 0x3698) = 0x20;
    Scene_CreateRenderer(s);
    memset(s + 0x36e8, 0, 0x30);
    memset(s + 0x4ad, 0, 0xc68 * 4);
    F<uint32_t>(s, 0x22b5b) = 0x3f800000;
    F<uint32_t>(s, 0x22b5f) = 0x3f800000;
    s[0x366d] = 0;
    F<uint32_t>(s, 0x7d) = 0;
    F<uint32_t>(s, 0xa1) = 0;
    F<uint32_t>(s, 0x9d) = 0;
    F<uint32_t>(s, 0x99) = 0;
    F<uint32_t>(s, 0x95) = 0;
    F<uint32_t>(s, 0x364d) = 0;
    s[0x3693] = 1;
    F<uint32_t>(s, 0x3665) = 0;
    memset(s + 0x36b8, 0, 0x30);
    memset(s + 0xa9, 0, 0x400);
    F<uint32_t>(s, 0x4a9) = 0;
    F<uint32_t>(s, 0x366e) = 0;
    F<uint32_t>(s, 0x65) = 0;
    memset(s + 0x22b73, 0, 0x18);
    for (int32_t k = 0; k < 4; ++k) F<uint32_t>(s, 0x22b8f + k * 4) = 1;
    memset(s + 0x22b9f, 0, 0x18);
    ((int32_t(__stdcall*)(uint32_t, void*))0x570fbd)(0, s + 0x5d);
    for (int32_t k = 0; k < 6; ++k) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(s, 0x79 + k * 4) = l ? PtrList_Construct(l, 0, 1, 100) : nullptr;
    }
    Texture_SetD9399c((const char*)0x5ce4b4);
    Texture_SetD93990((const char*)0x5de124);
    Texture_SetD97ab0((const char*)0x5de118);
    static const uint32_t tex[][2] = { { 0x3659, 0x5de10c }, { 0x3651, 0x5de100 }, { 0x3655, 0x5de0f4 }, { 0x365d, 0x5de0dc }, { 0x3661, 0x5de0cc } };
    for (const auto& t : tex) F<uint8_t*>(s, t[0]) = TextureCache_Get(hook::Global<uint8_t*>(0x6d4020), 0, (const char*)t[1], 0, 0, 0x7f);
    F<uint8_t*>(s, 0x95) = Scene_NewObject((const char*)0x5de0b4, (const char*)0x5de0c4);
    ((uint8_t*)(uintptr_t)Obj_Get1c(F<uint8_t*>(s, 0x95)))[5] = 0x20;
    Texture_SetCcba20((const char*)0x5de0a4);
    bool fog = !F<uint8_t>(hook::Global<uint8_t*>(0x6d4024), 0x114251) && F<int32_t>(F<uint8_t*>(s, 0x4d), 4) == 2;
    F<uint8_t*>(s, 0x99) = Scene_NewObject((const char*)0x5de088, (const char*)(fog ? 0x5de098 : 0x5de080));
    ((uint8_t*)(uintptr_t)Obj_Get1c(F<uint8_t*>(s, 0x99)))[5] = 0x20;
    F<uint8_t*>(s, 0x9d) = Scene_NewObject((const char*)0x5de06c, (const char*)0x5de07c);
    uint8_t* o = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0xd9);
    F<uint8_t*>(s, 0x3665) = o ? Object_InitFromCollision(o, 0, (const char*)0x5de06c, 0x437f0000) : nullptr;
    F<uint8_t*>(s, 0xa1) = Scene_NewObject((const char*)0x5de054, (const char*)0x5de064);
    F<uint8_t*>(s, 0xa5) = Scene_NewObject((const char*)0x5de03c, (const char*)0x5de04c);
    Obj_MoveTo(F<uint8_t*>(s, 0x99), 0, 0.0f, -18.0f, 0.0f);
    Obj_MoveTo(F<uint8_t*>(s, 0x9d), 0, 0.0f, 16.0f, 0.0f);
    Obj_MoveTo(F<uint8_t*>(s, 0xa5), 0, 0.0f, 0.0f, 0.0f);
    F<uint8_t*>(s, 0x364d) = Scene_NewObject((const char*)0x5de01c, (const char*)0x5de030);
    F<uint32_t>(s, 0x3672) = 0;
    F<uint32_t>(s, 0x3720) = 0;
    F<uint32_t>(s, 0x3724) = 0;
    F<uint32_t>(s, 0x3728) = 0x42000000;
    F<uint32_t>(s, 0x372c) = 0;
    F<uint32_t>(s, 0x15) = 0;
    F<uint32_t>(s, 0x19) = 0;
    F<uint32_t>(s, 0x3741) = 0;
    s[0x3745] = 1;
    uint16_t* dist = (uint16_t*)0x6d9098;
    for (int32_t i = 0; i < 0x418; ++i)
        for (int32_t j = 0; j < 0x418; ++j) *dist++ = (uint16_t)Ftol(Scene_TableDistance(i, j));
    uint16_t* clamp = (uint16_t*)0xc33d38;
    for (int32_t v = -0x60; v < 0x176a0; v += 3) *clamp++ = (uint16_t)(v <= 0 ? 0 : v);
    Scene_ApplyLighting(s);
    Render_RestoreTarget(F<uint8_t*>(s, 0x2d));
    Render_CullCcw(F<uint8_t*>(s, 0x2d));
    F<uint32_t>(s, 0x22b67) = 0x4703a400;
    F<uint32_t>(s, 1) = 1;
    Scene_RebuildStaticBuffer(s);
    if (F<uint8_t*>(s, 0x29)[0x67]) {
        F<uint32_t>(F<uint8_t*>(s, 0x29), 0x68) = 0;
        F<uint8_t*>(s, 0x29)[0x67] = 0;
        Render_Benchmark(F<uint8_t*>(s, 0x2d));
        uint8_t* p = F<uint8_t*>(s, 0x29);
        F<float>(p, 0x68) = (float)(Render_Benchmark(F<uint8_t*>(s, 0x2d)) + F<float>(p, 0x68));
        if (void (*done)() = hook::Global<void (*)()>(0xd97968)) done();
    }
    return s;
}
RT3_REPLACE(0x55e370, Scene_Construct);

// 0x55e9f0. Picture widget defaults (type 0x80).
RT3_NOINLINE void __fastcall Picture_Init(uint8_t* w) {
    static const uint16_t zero[] = { 0xe6, 0xd6, 0xea, 0xa2, 0xa6, 0xb2, 0xf2, 0xfe, 0xbe, 0x102, 0xaa, 0xae, 0x106 };
    F<int32_t>(w, 0xe6) = 0;
    F<int32_t>(w, 0xda) = 1;
    F<uint16_t>(w, 6) = 0x80;
    for (uint16_t off : zero) F<int32_t>(w, off) = 0;
}
RT3_REPLACE(0x55e9f0, Picture_Init);

// 0x55ea60. Copies video widget w's current Bink frame (+0xf2) into its
// texture +0xe6 through the shared pixel buffer 0xd97984: directly at
// the texture size (+0xde x +0xe2), or via the full-size frame buffer +0xbe
// resampled nearest-neighbour; +0xda flips. Uploads only when the copy
// succeeded (BinkCopyToBuffer returned 0).
RT3_NOINLINE void __fastcall Video_CopyFrame(uint8_t* w) {
    typedef int32_t(__stdcall * CopyToBuffer)(void*, void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
    uint32_t flags = F<int32_t>(w, 0xda) ? 0x80000000u : 0;
    if (!hook::Global<uint32_t*>(0xd97984)) hook::Global<uint32_t*>(0xd97984) = Texture_Pixels32(F<uint8_t*>(w, 0xe6), 0, nullptr, nullptr, 0);
    flags |= 3;
    int32_t res;
    if (const uint32_t* src = F<const uint32_t*>(w, 0xbe)) {
        uint32_t* bink = F<uint32_t*>(w, 0xf2);
        res = (*(CopyToBuffer*)0x5c8384)(bink, (void*)src, bink[0] * 4, bink[1], 0, 0, flags | 0x70000000);
        uint32_t* dst = hook::Global<uint32_t*>(0xd97984);
        bink = F<uint32_t*>(w, 0xf2);
        float sx = (float)((double)bink[0] / F<int32_t>(w, 0xde));
        float sy = (float)((double)bink[1] / F<int32_t>(w, 0xe2));
        for (int32_t y = 0; y < F<int32_t>(w, 0xe2); ++y) {
            int32_t base = Ftol((double)y * sy) * (int32_t)F<uint32_t*>(w, 0xf2)[0];
            for (int32_t x = 0; x < F<int32_t>(w, 0xde); ++x) *dst++ = src[Ftol((double)x * sx) + base];
        }
    } else {
        res = (*(CopyToBuffer*)0x5c8384)(F<void*>(w, 0xf2), hook::Global<uint32_t*>(0xd97984), F<uint32_t>(w, 0xde) * 4, F<uint32_t>(w, 0xe2), 0, 0, flags);
    }
    if (!res) Texture_SetPixels(F<uint8_t*>(w, 0xe6), 0, hook::Global<uint32_t*>(0xd97984));
}
RT3_REPLACE(0x55ea60, Video_CopyFrame);

// 0x55ec00. Widget g's w x h drawing canvas (+0xe6, size +0xde / +0xe2),
// created cleared on first use; resets the cursor when the widget's cursor
// kind (+0xea, table 0x625830) asks for it.
RT3_NOINLINE uint8_t* __fastcall Widget_Canvas(uint8_t* g, int, int32_t w, int32_t h) {
    if (!F<uint8_t*>(g, 0xe6)) {
        F<int32_t>(g, 0xde) = w;
        F<int32_t>(g, 0xe2) = h;
        uint8_t* p = (uint8_t*)((void*(__cdecl*)(uint32_t))0x53b070)(0xec);
        uint8_t* t = p ? Texture_InitMemory(p, 0, nullptr, w, h, 1) : nullptr;
        F<uint8_t*>(g, 0xe6) = t;
        uint32_t* px = Texture_Pixels32(t, 0, nullptr, nullptr, 0);
        memset(px, 0, (size_t)(w * h * 4));
        Texture_SetPixels(F<uint8_t*>(g, 0xe6), 0, px);
    }
    if (((const uint8_t*)0x62583b)[F<int32_t>(g, 0xea) << 4]) Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 0, 0, 0);
    return F<uint8_t*>(g, 0xe6);
}
RT3_REPLACE(0x55ec00, Widget_Canvas);

// 0x55ecb0. Picture (vtable 0x5de2a8) from the pack, counted in 0xd9797c: fields
// +0xa2..+0xb5.
uint8_t* __fastcall Picture_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5de2a8;
    Picture_Init(w);
    ++hook::Global<int32_t>(0xd9797c);
    uint8_t* rec = Widget_ReadRecord(w, 0x87);
    memcpy(w + 0xa2, rec + 0x5f, 0x14);
    Free(rec);
    return w;
}
RT3_REPLACE(0x55ecb0, Picture_Load);

// 0x55ed40. Releases the picture's texture (+0xe6) and re-enables the engine flag.
RT3_NOINLINE void __fastcall Picture_FreeTexture(uint8_t* w) {
    uint8_t* t = F<uint8_t*>(w, 0xe6);
    if (!t) return;
    Texture_Destroy(t);
    ((void(__cdecl*)(void*))0x53b080)(t);
    F<void*>(w, 0xe6) = nullptr;
    uint8_t v = Engine()[0x114227];
    hook::Global<int32_t>(0xd97984) = 0;
    Engine()[0x114227] = (uint8_t)((v & 0xf1) | 1);
}
RT3_REPLACE(0x55ed40, Picture_FreeTexture);

// 0x55ed90 (with its fall-through tail 0x55ed9a, which Ghidra lists as a
// function of its own). Stops a movie widget: a pending config save
// (0xd97980) is written; an open Bink movie (+0xf2) is closed, the speaker
// setup restored and, for movie kinds with a cursor (0x625834), the cursor
// shown again. With `release`, also frees the frame buffers (+0xfe, +0xbe)
// and the texture.
RT3_NOINLINE void __fastcall Movie_Stop(uint8_t* w, int, int32_t release) {
    if (hook::Global<int32_t>(0xd97980)) {
        hook::Global<int32_t>(0xd97980) = 0;
        F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114746) = 0;
        Engine_SaveConfig(hook::Global<uint8_t*>(0x6d4024));
    }
    if (F<uint32_t>(w, 0xf2)) {
        hook::Global<int32_t>(0xd9796c) = 0;
        Import<void(__stdcall*)(uint32_t)>(0x5c8380)(F<uint32_t>(w, 0xf2));
        if (hook::Global<uint8_t*>(0x6d402c))
            Sound_Speakers(hook::Global<uint8_t*>(0x6d402c), 0, F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x11470a), 0);
        if (hook::Global<uint8_t>(0x625834 + F<int32_t>(w, 0xea) * 16)) Cursor_Set(hook::Global<uint8_t*>(0xccbb20), 0, 1, 0, 0);
    }
    F<uint32_t>(w, 0xf2) = 0;
    if (!release) return;
    if (F<void*>(w, 0xfe)) FreeAndNull(&F<void*>(w, 0xfe));
    if (F<void*>(w, 0xbe)) Free(F<void*>(w, 0xbe));
    F<void*>(w, 0xbe) = nullptr;
    Picture_FreeTexture(w);
}
RT3_REPLACE(0x55ed90, Movie_Stop);

// 0x55ee60. Stops (and releases) the movie of a widget that is no longer shown.
RT3_NOINLINE void __fastcall Movie_StopIfHidden(uint8_t* w) {
    if (!Widget_IsVisible(w)) Movie_Stop(w, 0, 1);
}
RT3_REPLACE(0x55ee60, Movie_StopIfHidden);

// 0x55ee80. Advances a movie widget's Bink playback (+0xf2) for this frame
// (unless hidden and not `force`, or movies are frozen 0xd97970): skips ahead
// by the time since the last frame (+0x102) at the movie's rate, decodes up to
// four frames; at the end it loops (kinds with the loop flag 0x625836, from key
// frame 0x62583a) or stops / releases it and calls the end callback +0xd6.
RT3_NOINLINE void __fastcall Movie_Tick(uint8_t* w, int, int32_t force) {
    typedef int32_t(__stdcall * BinkFn1)(uint8_t*);
    if (!force && !Widget_IsVisible(w)) return;
    if (hook::Global<int32_t>(0xd97970)) return;
    uint8_t* b = F<uint8_t*>(w, 0xf2);
    if (!b) return;
    auto bgoto = [&](uint32_t frame, uint32_t flags) { Import<void(__stdcall*)(uint8_t*, uint32_t, uint32_t)>(0x5c836c)(b, frame, flags); };
    bool decode = false;
    if (!F<uint32_t>(b, 0x2a0) && F<uint32_t>(b, 0x10) >= 1 && F<uint32_t>(b, 0x10) != F<uint32_t>(b, 8) && F<int32_t>(w, 0x102)) {
        int32_t elapsed = ClockMs() - F<int32_t>(w, 0x102);
        uint32_t num = F<uint32_t>(b, 0x18) < 1 ? 1 : F<uint32_t>(b, 0x18);
        uint32_t den = F<uint32_t>(b, 0x14) < 1 ? 1 : F<uint32_t>(b, 0x14);
        uint32_t ms1000 = num * 1000;
        double fn = (double)(int32_t)ms1000;
        if ((int32_t)ms1000 < 0) fn += Fl(0x5c872c);
        double fd = (double)(int32_t)den;
        if ((int32_t)den < 0) fd += Fl(0x5c872c);
        int32_t step = Ftol(fn / fd * D(0x5c8660));
        int32_t n = elapsed / step;
        uint32_t cur = F<uint32_t>(b, 0x10), total = F<uint32_t>(b, 8);
        if (cur + (uint32_t)n + 1 > total) n = (int32_t)(total - cur - 1);
        if (n > 0) {
            bgoto(cur + (uint32_t)n + 1, 0);
            decode = true;
        }
    }
    if (!decode) {
        if (F<uint32_t>(b, 0x10) != F<uint32_t>(b, 8)) decode = true;
        else {
            int32_t kind = F<int32_t>(w, 0xea);
            if (hook::Global<uint8_t>(0x625836 + kind * 16)) {
                uint8_t key = hook::Global<uint8_t>(0x62583a + kind * 16);
                if (key) {
                    Import<uint32_t(__stdcall*)(uint8_t*, uint32_t, uint32_t)>(0x5c837c)(b, key, 2);
                    bgoto(hook::Global<uint8_t>(0x62583a + F<int32_t>(w, 0xea) * 16), 1);
                } else
                    bgoto(1, 0);
                F<int32_t>(w, 0xda) = 1;
                decode = true;
            } else {
                if (hook::Global<int32_t>(0x6d4000)) F<int32_t>(w, 0xae) = 1;
                int32_t old = F<int32_t>(w, 0x106);
                F<int32_t>(w, 0x106) = 1;
                if (!F<int32_t>(w, 0xae)) Movie_Stop(w, 0, 1);
                void (*cb)() = F<void (*)()>(w, 0xd6);
                if (cb && !old) cb();
            }
        }
    }
    if (decode) {
        Import<BinkFn1>(0x5c8370)(b);
        Import<BinkFn1>(0x5c8374)(b);
        if (!Import<BinkFn1>(0x5c8378)(F<uint8_t*>(w, 0xf2))) {
            for (int32_t extra = 0;;) {
                if (F<uint32_t>(b, 0x10) + 1 >= F<uint32_t>(b, 8)) break;
                if (extra++ >= 3) break;
                Import<BinkFn1>(0x5c8370)(b);
                Import<BinkFn1>(0x5c8374)(b);
                if (Import<BinkFn1>(0x5c8378)(F<uint8_t*>(w, 0xf2))) break;
            }
        }
    }
    F<int32_t>(w, 0x102) = ClockMs();
}
RT3_REPLACE(0x55ee80, Movie_Tick);

// 0x55f0b0. Opens movie `index` (table 0x625830: name, flags +5, volume
// +0xc) in video widget w: Miles sound when w has sound (+0xaa) and the
// sound system exists, the file from the game folder (0x5de2fc) or else,
// once, from the CD drive 0x625a90 (0x5de2e8, remembered in the config);
// failing that it stops and calls the done callback +0xd6. Then opens it in
// Bink (fatal on error), sizes the texture to powers of two within the
// screen (a full-size frame buffer +0xbe when shrunk), sets the volume
// (movie x engine 0x114702, mute below 0x5c85b8), the on-screen scale for
// auto-size (+0xa6) and shows the first frame.
RT3_NOINLINE void __fastcall Movie_Open(uint8_t* w, int, int32_t index) {
    typedef int32_t(__stdcall * SetSound)(void*, uint32_t);
    Movie_Stop(w, 0, 0);
    if (F<int32_t>(w, 0xaa) && hook::Global<uint8_t*>(0x6d402c))
        (*(SetSound*)0x5c8360)(*(void**)0x5c835c, Sound_Get2c(hook::Global<uint8_t*>(0x6d402c)));
    else
        F<int32_t>(w, 0xaa) = 0;
    F<int32_t>(w, 0xea) = index;
    F<int32_t>(w, 0x102) = 0;
    hook::Global<int32_t>(0xd97980) = 0;
    char path[0x190];
    auto open_ = (int(__cdecl*)(const char*, int, int))0x5a4c57;
    FormatText(path, (const char*)0x5de2fc, hook::Global<const char*>(0x625830 + F<int32_t>(w, 0xea) * 0x10), nullptr, nullptr, nullptr, nullptr, nullptr,
               nullptr, nullptr);
    int fd = open_(path, 0, 0x100);
    if (fd == -1) {
        bool found = false;
        if (!F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114746)) {
            char drive[4];
            drive[0] = (char)(hook::Global<uint8_t>(0x625a90) + 0x41);
            drive[1] = 0;
            FormatText(path, (const char*)0x5de2e8, drive, hook::Global<const char*>(0x625830 + F<int32_t>(w, 0xea) * 0x10), nullptr, nullptr, nullptr,
                       nullptr, nullptr, nullptr);
            fd = open_(path, 0, 0x100);
            if (fd != -1) {
                hook::Global<int32_t>(0xd97980) = 1;
                F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114746) = 1;
                Engine_SaveConfig(hook::Global<uint8_t*>(0x6d4024));
                found = true;
            }
        }
        if (!found) {
            Movie_Stop(w, 0, 1);
            if (void (*done)() = F<void (*)()>(w, 0xd6)) done();
            return;
        }
    }
    ((int(__cdecl*)(int))0x5a440d)(fd);
    uint32_t flags = hook::Global<uint8_t>(0x625835 + F<int32_t>(w, 0xea) * 0x10) ? 0x2000 : 0;
    (*(int32_t(__stdcall**)(uint32_t))0x5c8364)(0x400000);
    F<uint32_t*>(w, 0xf2) = (*(uint32_t*(__stdcall**)(const char*, uint32_t))0x5c8368)(path, flags | 0x1000000);
    if (!F<uint32_t*>(w, 0xf2)) {
        char msg[0x3ec];
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(msg, (*(const char*(__stdcall**)())0x5c8388)());
        Fatal_Exit(msg);
    }
    F<int32_t>(w, 0x106) = 0;
    hook::Global<uint32_t*>(0xd9796c) = F<uint32_t*>(w, 0xf2);
    uint32_t* bink = F<uint32_t*>(w, 0xf2);
    uint32_t pw = 1, ph = 1;
    if (bink[0] > 1)
        while (pw < F<uint32_t*>(w, 0xf2)[0]) pw += pw;
    if (bink[1] > 1)
        while (ph < F<uint32_t*>(w, 0xf2)[1]) ph += ph;
    F<uint32_t>(w, 0xb6) = pw;
    F<uint32_t>(w, 0xba) = ph;
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    while ((int32_t)pw > F<int32_t>(e, 0x11423b)) pw >>= 1;
    while ((int32_t)ph > F<int32_t>(e, 0x11423f)) ph >>= 1;
    F<void*>(w, 0xbe) = nullptr;
    if ((int32_t)pw < F<int32_t>(w, 0xb6) || (int32_t)ph < F<int32_t>(w, 0xba)) F<void*>(w, 0xbe) = ((void*(__cdecl*)(uint32_t))0x5a125d)(bink[1] * bink[0] * 4);
    Widget_Canvas(w, 0, (int32_t)pw, (int32_t)ph);
    float vol = hook::Global<float>(0x62583c + F<int32_t>(w, 0xea) * 0x10) * F<float>(hook::Global<uint8_t*>(0x6d4024), 0x114702);
    typedef int32_t(__stdcall * SetVolume)(void*, uint32_t, int32_t);
    if (F<int32_t>(w, 0xaa) && !(vol < D(0x5c85b8)))
        (*(SetVolume*)0x5c8358)(F<void*>(w, 0xf2), 0, Ftol((double)vol * Fl(0x5ddaec)));
    else
        (*(SetVolume*)0x5c8358)(F<void*>(w, 0xf2), 0, 0);
    if (F<int32_t>(w, 0xa6)) {
        int32_t* b = F<int32_t*>(w, 0xf2);
        int32_t bh = b[1];
        uint8_t* en = hook::Global<uint8_t*>(0x6d4024);
        F<float>(w, 0x13) = (float)((double)b[0] / F<int32_t>(F<uint8_t*>(en, 0x34) + F<int32_t>(en, 0x11421e) * 0x15, 0));
        en = hook::Global<uint8_t*>(0x6d4024);
        F<float>(w, 0x17) = (float)((double)bh / F<int32_t>(F<uint8_t*>(en, 0x34) + F<int32_t>(en, 0x11421e) * 0x15, 4));
    }
    Movie_Tick(w, 0, 1);
    Video_CopyFrame(w);
    F<int32_t>(w, 0xda) = 1;
}
RT3_REPLACE(0x55f0b0, Movie_Open);

// 0x55f3f0 (movie widget events). Its own button 0x80 sets the end callback
// (+0xd6), 0x8c opens movie ev[2]; 0xa5 stops it; 0x8f / 0x96 stop a playing
// (+0xf2, not +0xa2) movie and run the callback. Everything else goes to the
// widget.
int32_t __fastcall MovieWidget_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    switch (ev[0]) {
    case 0x80:
        if (ev[1] == F<uint16_t>(w, 4)) {
            F<uint32_t>(w, 0xd6) = ev[2];
            return Widget_HandleEvent(w, 0, (uint8_t*)ev);
        }
        break;
    case 0x8c:
        if (ev[1] == F<uint16_t>(w, 4)) {
            Movie_Open(w, 0, (int32_t)ev[2]);
            return 0;
        }
        break;
    case 0xa5:
        Movie_Stop(w, 0, 1);
        return Widget_HandleEvent(w, 0, (uint8_t*)ev);
    case 0x8f:
    case 0x96:
        if (!F<int32_t>(w, 0xa2) && F<int32_t>(w, 0xf2)) {
            Movie_Stop(w, 0, 1);
            if (void(*fn)() = F<void(*)()>(w, 0xd6)) fn();
        }
        break;
    }
    return Widget_HandleEvent(w, 0, (uint8_t*)ev);
}
RT3_REPLACE(0x55f3f0, MovieWidget_HandleEvent);

// 0x55f4e0 (vtable). Draws movie widget w (Bink +0xf2, slot +0xea in the
// 16-byte movie table 0x625837) unless paused (0xd97970): waits for the
// frame (0x5c8378), ticks and copies it; without a movie a playing slot is a
// black box. The frame texture (+0xe6, half-texel offsets 0.5 / size) is
// drawn over the box, or with letterboxing (+0xb2) letterboxed against the
// screen mode (0x6d4024 +0x34 by +0x11421e: bars when narrower, a cropped
// centre when taller; QUIRK: the bar height then serves as the v offset),
// with texture filtering off meanwhile (+0x114227); then the debug frames.
void __fastcall Movie_Draw(uint8_t* w) {
    if (!Widget_IsVisible(w) || hook::Global<int32_t>(0xd97970)) return;
    auto playing = [&]() { return *(const uint8_t*)(0x625837 + F<int32_t>(w, 0xea) * 16) != 0; };
    using BinkFn = int32_t(__stdcall*)(void*);
    if (F<uint8_t*>(w, 0xf2) && playing()) {
        while ((*(BinkFn*)0x5c8378)(F<uint8_t*>(w, 0xf2))) {
        }
        Movie_Tick(w, 0, 0);
        if (F<uint8_t*>(w, 0xf2)) Video_CopyFrame(w);
    }
    uint8_t* v = F<uint8_t*>(w, 0xf2);
    if (!v) {
        if (playing()) {
            uint8_t* p = F<uint8_t*>(w, 0x86);
            const float y = (float)((double)F<float>(p, 9) + F<float>(w, 0xf));
            const float x = (float)((double)F<float>(p, 5) + F<float>(w, 0xb));
            Ui_FillRect(0xff000000, 0, x, y, F<float>(w, 0x13), F<float>(w, 0x17), 3);
        }
        return;
    }
    if (!F<uint32_t>(v, 0xc)) Movie_Tick(w, 0, 0);
    if (!playing()) return;
    float du = (float)(D(0x5c8628) / Texture_Width(F<uint8_t*>(w, 0xe6)));
    float dv = (float)(D(0x5c8628) / Texture_Height(F<uint8_t*>(w, 0xe6)));
    auto unsignedF = [](int32_t i) { return i < 0 ? (double)i + Fl(0x5c872c) : (double)i; };
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    uint32_t q[8];
    if (F<uint32_t>(w, 0xb2)) {
        const uint8_t* mode = F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
        const int32_t sw = F<int32_t>((void*)mode, 4);
        uint8_t* vid = F<uint8_t*>(w, 0xf2);
        if (F<int32_t>(vid, 4) < sw) {
            const int32_t pad = (int32_t)((uint32_t)(sw - F<int32_t>(vid, 4)) >> 1) + 2;
            // The black bars above and below; the texel offsets stay.
            const float bar = (float)(unsignedF(pad) / sw);
            Ui_FillRect(0xff000000, 0, 0.0f, 0.0f, 1.0f, bar, 3);
            Ui_FillRect(0xff000000, 0, 0.0f, (float)((double)Fl(0x5c85b4) - bar), 1.0f, bar, 3);
            e = hook::Global<uint8_t*>(0x6d4024);
        }
        mode = F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
        vid = F<uint8_t*>(w, 0xf2);
        const int32_t sw2 = F<int32_t>((void*)mode, 4);
        const int32_t vw = F<int32_t>(vid, 4);
        const int32_t pad2 = (int32_t)((uint32_t)(sw2 - vw) >> 1);
        const int32_t sh = F<int32_t>((void*)mode, 0), vh = F<int32_t>(vid, 0);
        const float edge = (float)(unsignedF(pad2) / sw2);
        int32_t crop = 0;
        if (vh > sh) crop = Ftol((double)(vh - sh) * D(0x5c8628));
        const float cropV = (float)((double)crop / F<int32_t>(w, 0xde));
        const double vSpan = (double)((sh < vh ? sh : vh) + crop) / F<int32_t>(w, 0xde);
        const float uSpan = (float)(unsignedF(vw) / F<int32_t>(w, 0xe2));
        const double v1 = vSpan > D(0x5c8770) ? D(0x5c8770) : vSpan;
        const double u1 = uSpan > D(0x5c8770) ? D(0x5c8770) : (double)uSpan;
        e[0x114227] &= 0xf0;
        uint8_t* p = F<uint8_t*>(w, 0x86);
        q[0] = Bits((float)((double)cropV + du));
        q[1] = Bits(dv);
        q[2] = Bits((float)(v1 + du));
        q[3] = Bits((float)(u1 + dv));
        const float fade = ((float(__cdecl*)())(void*)&Fade_Step)();
        const float y1 = (float)((double)F<float>(w, 0xf) + F<float>(p, 9) + F<float>(w, 0x17) - edge);
        const float x1 = (float)((double)F<float>(w, 0x13) + F<float>(w, 0xb) + F<float>(p, 5));
        const float y0 = (float)((double)F<float>(w, 0xf) + F<float>(p, 9) + edge);
        const float x0 = (float)((double)F<float>(w, 0xb) + F<float>(p, 5));
        Engine_QueueSprite(hook::Global<void*>(0x6d4024), 0, Bits(x0), Bits(y0), Bits(x1), Bits(y1), Bits(fade), 0xffffffff, F<uint32_t>(w, 0xe6), q[0], q[1], q[2],
                           q[3], 0x3f800000);
    } else {
        e[0x114227] &= 0xf0;
        uint8_t* vid = F<uint8_t*>(w, 0xf2);
        const double us = unsignedF(F<int32_t>(vid, 4)) / F<int32_t>(w, 0xe2);
        const double u1 = us > D(0x5c8770) ? 1.0 : us;
        const double vs = unsignedF(F<int32_t>(vid, 0)) / F<int32_t>(w, 0xde);
        const double v1 = vs > D(0x5c8770) ? D(0x5c8770) : vs;
        uint8_t* p = F<uint8_t*>(w, 0x86);
        q[1] = Bits(dv);
        q[0] = Bits(du);
        q[3] = Bits((float)((double)dv + u1));
        q[2] = Bits((float)(v1 + du));
        const float fade = ((float(__cdecl*)())(void*)&Fade_Step)();
        const float y1 = (float)((double)F<float>(w, 0xf) + F<float>(p, 9) + F<float>(w, 0x17));
        const float x1 = (float)((double)F<float>(w, 0x13) + F<float>(w, 0xb) + F<float>(p, 5));
        const float y0 = (float)((double)F<float>(w, 0xf) + F<float>(p, 9));
        const float x0 = (float)((double)F<float>(w, 0xb) + F<float>(p, 5));
        Engine_QueueSprite(hook::Global<void*>(0x6d4024), 0, Bits(x0), Bits(y0), Bits(x1), Bits(y1), Bits(fade), 0xffffffff, F<uint32_t>(w, 0xe6), q[0], q[1], q[2],
                           q[3], 0x3f800000);
    }
    uint8_t* eng = hook::Global<uint8_t*>(0x6d4024);
    eng[0x114227] = (uint8_t)((eng[0x114227] & 0xf1) | 1);
    if (!(F<uint32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc79) & F<uint16_t>(w, 6))) return;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0xb)), (float)((double)F<float>(p, 9) + F<float>(w, 0xf)), F<float>(w, 0x13),
                 F<float>(w, 0x17), 1.0f, 3);
    if (!(F<float>(w, 0x23) == Fl(0x5c8598))) {
        p = F<uint8_t*>(w, 0x86);
        Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0x1b)), (float)((double)F<float>(p, 9) + F<float>(w, 0x1f)),
                     F<float>(w, 0x23), F<float>(w, 0x27), 1.0f, 3);
    }
}
RT3_REPLACE(0x55f4e0, Movie_Draw);

// 0x55f9b0. Picture widget destructor (vtable 0x5de2a8): stops its movie, drops
// it from the picture count 0xd9797c, destroys the widget base; frees it with
// flag 1.
RT3_NOINLINE uint8_t* __fastcall Picture_Destroy(uint8_t* w, int, uint32_t flags) {
    F<uint32_t>(w, 0) = 0x5de2a8;
    Movie_Stop(w, 0, 1);
    --hook::Global<int32_t>(0xd9797c);
    Widget_Destruct(w);
    if (flags & 1) ExeDelete(w);
    return w;
}
RT3_REPLACE(0x55f9b0, Picture_Destroy);

// 0x55f9f0. Tree view defaults (type 8).
RT3_NOINLINE void __fastcall TreeView_Init(uint8_t* w) {
    static const uint16_t zero[] = { 0x182, 0x186, 0x18a, 0x18e, 0x1da, 0x1d2, 0x176, 0x17a, 0x1e2, 0x1e6, 0x1de,
                                     0x17e, 0x1be, 0x1ce, 0x1ba, 0x1ca, 0x1c6, 0x1c2, 0xde,  0xe2,  0xe6,  0xee,
                                     0xf2,  0xa2,  0x19a, 0xea,  0x1a8, 0x1ac, 0x1b2, 0x1b6, 0x19e, 0x1a2, 0xfe,
                                     0x102, 0xfa };
    F<uint16_t>(w, 6) = 8;
    for (uint16_t off : zero) F<int32_t>(w, off) = 0;
    F<int32_t>(w, 0x1d6) = -1;
    F<int32_t>(w, 0x192) = -1;
    F<int32_t>(w, 0x172) = 1;
    F<uint16_t>(w, 0x1a6) = 0;
}
RT3_REPLACE(0x55f9f0, TreeView_Init);

// 0x55faf0. Syncs a list's scrollbar (+0x19a): it is needed when there are more
// rows (+0x1da) than fit (+0xa2) or the list doesn't force it off (+0xea == 1), and
// never with flag +0x33 bit 15. The bar's range becomes rows - visible + 1 (at
// least 1) and it is shown (flags 3) or hidden when that changes (+0x172).
RT3_NOINLINE void __fastcall List_SyncScrollbar(uint8_t* l) {
    int32_t rows = F<int32_t>(l, 0x1da), fit = F<int32_t>(l, 0xa2);
    int32_t on = rows > fit || F<int32_t>(l, 0xea) != 1 ? 1 : 0;
    if (F<uint32_t>(l, 0x33) & 0x8000) on = 0;
    if (uint8_t* bar = F<uint8_t*>(l, 0x19a)) Scrollbar_SetPos(bar, 0, rows > fit ? rows - fit + 1 : 1);
    if (F<int32_t>(l, 0x172) == on) return;
    F<int32_t>(l, 0x172) = on;
    uint8_t* bar = F<uint8_t*>(l, 0x19a);
    if (!bar) return;
    if (on) Widget_SetFlags(bar, 0, 3, 0, 0);
    else Widget_ClearFlags(bar, 0, 3);
}
RT3_REPLACE(0x55faf0, List_SyncScrollbar);

RT3_NOINLINE uint32_t __fastcall TreeView_NodeData(uint8_t* w, int, int32_t i) { return F<uint32_t>(Node(w, i), 8); }
RT3_REPLACE(0x55fb80, TreeView_NodeData);

RT3_NOINLINE uint32_t __fastcall TreeView_SelectedData(uint8_t* w) { return F<uint32_t>(Node(w, F<int32_t>(w, 0x1d6)), 8); }
RT3_REPLACE(0x55fba0, TreeView_SelectedData);

// 0x55fbc0. Whether some ancestor of node i is collapsed.
RT3_NOINLINE int32_t __fastcall TreeView_IsHidden(uint8_t* w, int, int32_t i) {
    uint8_t* n = Node(w, i);
    if (!F<int32_t>(n, 0x10)) return 0;
    do {
        n = Node(w, F<int32_t>(n, 0x14));
        if (!F<int32_t>(n, 0x18)) return 1;
    } while (F<int32_t>(n, 0x10));
    return 0;
}
RT3_REPLACE(0x55fbc0, TreeView_IsHidden);

RT3_NOINLINE int32_t __fastcall TreeView_Depth(uint8_t* w, int, int32_t i) {
    int32_t d = 0;
    for (uint8_t* n = Node(w, i); F<int32_t>(n, 0x10); ++d) n = Node(w, F<int32_t>(n, 0x14));
    return d;
}
RT3_REPLACE(0x55fc10, TreeView_Depth);

// 0x55fc50. Closes a tree view's popup (+0x1a8): click sound 7 at the centre, the
// popup's widgets (parent w) deleted and its handle released.
RT3_NOINLINE void __fastcall TreeView_ClosePopup(uint8_t* w) {
    if (!F<int32_t>(w, 0x1a8)) return;
    Ui_PlaySound(7, 0, 0x3f000000);
    Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, 0, 0xffff, (uint32_t)(uintptr_t)w);
    HandleTable_ReleaseId(hook::Global<uint8_t*>(0x6d401c), 0, F<uint16_t>(w, 4));
    F<int32_t>(w, 0x1a8) = 0;
}
RT3_REPLACE(0x55fc50, TreeView_ClosePopup);

RT3_NOINLINE uint32_t __fastcall TreeView_NodeValue(uint8_t* w, int, int32_t i) { return F<uint32_t>(Node(w, i), 0); }
RT3_REPLACE(0x55fca0, TreeView_NodeValue);

// 0x55fcc0 / 0x55ff70. Tree view (vtable 0x5de318): fields +0xa2... (sic) The
// loader defaults +0xfa to 0x20 when zero, then copies the zero over it again.
uint8_t* __fastcall TreeView_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5de318;
    TreeView_Init(w);
    uint8_t* rec = Widget_ReadRecord(w, 0x12f);
    TreeView_Init(w);
    memcpy(w + 0xa2, rec + 0x5f, 0xbc);
    Free(rec);
    return w;
}
RT3_REPLACE(0x55fcc0, TreeView_Load);

RT3_NOINLINE uint8_t* __fastcall TreeView_Construct(uint8_t* w, int, const uint8_t* def) {
    Widget_Construct(w, 0, def);
    F<uint32_t>(w, 0) = 0x5de318;
    TreeView_Init(w);
    memcpy(w + 0xa2, def + 0x5f, 0xd0);
    F<uint16_t>(w, 6) = 8;
    return w;
}
RT3_REPLACE(0x55ff70, TreeView_Construct);

// 0x55ffb0. Row height as a fraction of the screen, snapped to whole pixels
// (+0x5d0250) (x87 with the exe's _ftol).
RT3_NOINLINE double __fastcall List_RowHeight(uint8_t* w) {
    int32_t h = DisplayMode()[1];
    float rh = F<float>(w, 0xb6);
    int32_t px;
    __asm {
        fld rh
        fimul h
        mov eax, 0x5a10d0
        call eax
        mov px, eax
    }
    return ((double)px + *(const double*)0x5d0250) / h;
}
RT3_REPLACE(0x55ffb0, List_RowHeight);

// 0x560000. Creates a list's scrollbar (+0x19a, id +0x1b0) unless it has one
// (+0x126) or flag 0x8000 (+0x33) forbids it: a slider (0x55ab50) beside the
// list at the smaller of its two up / down button corners (+0xc6 / +0xce,
// +0xca / +0xd2), with the button rectangles (+0xc6..+0xda) relative to it,
// the list's skin and parts (+0x47, +0x43, +0xba.., +0xf2.., +0x106..), its
// page size (rows +0xa2 - 1, at least 2) and, with +0xee and +0xf2, the size
// up to the list's corner. The colour +0xf6 defaults to 0xffffff00.
void __fastcall List_CreateScrollbar(uint8_t* l) {
    if (F<int32_t>(l, 0x126)) return;
    if (F<int32_t>(l, 0x33) & 0x8000) return;
    uint8_t def[0xe3];
    memset(def, 0, sizeof def);
    F<uint16_t>(def, 5) = F<uint16_t>(l, 9);
    def[4] = (uint8_t)(l[8] + 1);
    String_Assign(&F<char*>(def, 0x43), F<const char*>(l, 0x47));
    F<uint32_t>(def, 0xa3) = F<uint32_t>(l, 0xbe);
    F<uint32_t>(def, 0x8b) = F<uint32_t>(l, 0x10a);
    F<uint32_t>(def, 0x9f) = F<uint32_t>(l, 0xba);
    F<uint32_t>(def, 0xa7) = F<uint32_t>(l, 0xc2);
    F<uint32_t>(def, 0x97) = F<uint32_t>(l, 0x116);
    F<uint32_t>(def, 0x87) = F<uint32_t>(l, 0x106);
    F<uint32_t>(def, 0x8f) = F<uint32_t>(l, 0x10e);
    int32_t page = F<int32_t>(l, 0xa2) - 1;
    F<uint8_t*>(def, 0x47) = l;
    F<uint32_t>(def, 0x93) = F<uint32_t>(l, 0x112);
    F<uint32_t>(def, 0x9b) = F<uint32_t>(l, 0x11a);
    F<int32_t>(def, 0x63) = page < 2 ? 2 : page;
    F<float>(def, 7) = (float)((double)(F<float>(l, 0xce) < F<float>(l, 0xc6) ? F<float>(l, 0xce) : F<float>(l, 0xc6)) + F<float>(l, 0xb));
    F<float>(def, 0xb) = (float)((double)(F<float>(l, 0xd2) < F<float>(l, 0xca) ? F<float>(l, 0xd2) : F<float>(l, 0xca)) + F<float>(l, 0xf));
    int32_t id = HandleTable_Acquire(hook::Global<uint8_t*>(0x6d401c), 0, 0);
    F<uint32_t>(def, 0x7b) = F<uint32_t>(l, 0xfa);
    F<uint32_t>(def, 0x7f) = F<uint32_t>(l, 0xfe);
    F<uint32_t>(def, 0x83) = F<uint32_t>(l, 0x102);
    F<uint16_t>(def, 0) = (uint16_t)id;
    F<uint32_t>(def, 0x77) = F<uint32_t>(l, 0xf2);
    F<int32_t>(def, 0x2f) = (l[0x33] & 8) ? 8 : 0;
    double dx = (double)F<float>(def, 7) - F<float>(l, 0xb);
    double dy = (double)F<float>(def, 0xb) - F<float>(l, 0xf);
    F<float>(def, 0xb7) = (float)(F<float>(l, 0xc6) - dx);
    F<float>(def, 0xbb) = (float)(F<float>(l, 0xca) - dy);
    F<float>(def, 0xbf) = (float)(F<float>(l, 0xce) - dx);
    F<float>(def, 0xc3) = (float)(F<float>(l, 0xd2) - dy);
    F<float>(def, 0xc7) = (float)(F<float>(l, 0xd6) - dx);
    F<float>(def, 0xcb) = (float)(F<float>(l, 0xda) - dy);
    if (F<int32_t>(l, 0xee) && F<int32_t>(l, 0xf2)) {
        F<float>(def, 0xf) = (float)((double)F<float>(l, 0x13) + F<float>(l, 0xb) - F<float>(def, 7));
        F<float>(def, 0x13) = (float)((double)F<float>(l, 0x17) + F<float>(l, 0xf) - F<float>(def, 0xb));
    }
    if (F<const char*>(l, 0x43)) String_Assign(&F<char*>(def, 0x3f), F<const char*>(l, 0x43));
    uint8_t* bar = (uint8_t*)ExeNew(0x1b9);
    bar = bar ? ListBox_Construct(bar, 0, def) : nullptr;
    F<uint8_t*>(l, 0x19a) = bar;
    F<uint16_t>(l, 0x1b0) = F<uint16_t>(def, 0);
    Window_AddChild(F<uint8_t*>(l, 0x86), 0, bar, 0);
    Free(F<void*>(def, 0x43));
    if (F<const char*>(l, 0x43)) Free(F<void*>(def, 0x3f));
    if (!F<uint32_t>(l, 0xf6)) F<uint32_t>(l, 0xf6) = 0xffffff00;
    List_SyncScrollbar(l);
}
RT3_REPLACE(0x560000, List_CreateScrollbar);

// 0x5602c0 / 0x560300. Node index <-> visible row (-1 passes through; 0 if not found).
RT3_NOINLINE int32_t __fastcall TreeView_RowOfNode(uint8_t* w, int, int32_t n) {
    if (n == -1) return -1;
    int32_t rows = 0;
    for (int32_t i = 0; i < n; ++i)
        if (!TreeView_IsHidden(w, 0, i)) ++rows;
    return rows;
}
RT3_REPLACE(0x5602c0, TreeView_RowOfNode);

RT3_NOINLINE int32_t __fastcall TreeView_NodeOfRow(uint8_t* w, int, int32_t row) {
    if (row == -1) return -1;
    int32_t cnt = 0;
    for (int32_t i = 0; i < F<int32_t>(w, 0x1d2); ++i) {
        if (TreeView_IsHidden(w, 0, i)) continue;
        if (row == cnt) return i;
        ++cnt;
    }
    return 0;
}
RT3_REPLACE(0x560300, TreeView_NodeOfRow);

// 0x560350. Screen position of row `row` (x into *x, y into *y).
RT3_NOINLINE void __fastcall List_RowPos(uint8_t* w, int, int32_t row, float* x, float* y) {
    int32_t h = DisplayMode()[1];
    float rh = F<float>(w, 0xb6);
    uint8_t* p = F<uint8_t*>(w, 0x86);
    *x = (float)(((double)F<float>(p, 5) + F<float>(w, 0xa6)) + F<float>(w, 0xb));
    int32_t px;
    __asm {
        fild h
        fmul rh
        mov eax, 0x5a10d0
        call eax
        mov px, eax
    }
    p = F<uint8_t*>(w, 0x86);
    *y = (float)((((((double)px + *(const double*)0x5d0250) / h + F<float>(w, 0xb2)) * row + F<float>(p, 9)) +
                  F<float>(w, 0xaa)) + F<float>(w, 0xf));
}
RT3_REPLACE(0x560350, List_RowPos);

// 0x5603e0. Draws a tree view (when visible). A closed one (+0x126) is its
// button image (+0x12a), the selected node's text (+0x14e.. box) and the
// arrow (+0x132 open / +0x12e). An open one draws its visible rows (+0xa2
// rows, +0x1de nodes from +0x1e2) in two passes: first per node the indent
// (+0x122: depth * 0.01), a +/- box for nodes with children (+0x1c, the bar
// of the + while closed +0x18), the text (or the owner's callback +0x1ce) and
// the selection frame (+0xe2); then the separators between rows (+0xe6: image
// +0x11e or a line). Row metrics come from the screen's font table.
RT3_NOINLINE void __fastcall TreeView_Draw(uint8_t* w, int, int32_t, int32_t, int32_t, int32_t) {
    if (!Widget_IsVisible(w)) return;
    auto glyph = [&](int32_t image, float x, float y) {
        Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, image), 0, x, y, 0x3f800000, 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f,
                    0.0f, 1.0f, 1.0f);
    };
    if (F<int32_t>(w, 0x126)) {
        uint8_t* r = F<uint8_t*>(w, 0x86);
        float y = (float)((double)F<float>(r, 9) + F<float>(w, 0xf));
        glyph(F<int32_t>(w, 0x12a), (float)((double)F<float>(r, 5) + F<float>(w, 0xb)), y);
        int32_t sel = F<int32_t>(w, 0x1d6);
        if (sel != -1) {
            uint8_t* e = F<uint8_t*>(w, 0x176) + sel * 32;
            uint32_t color = F<uint32_t>(e, 0xc) ? F<uint32_t>(e, 0xc) : F<uint32_t>(w, 0x38);
            r = F<uint8_t*>(w, 0x86);
            float ty = (float)((double)F<float>(r, 9) + F<float>(w, 0x152) + F<float>(w, 0xf));
            float tx = (float)((double)F<float>(r, 5) + F<float>(w, 0x14e) + F<float>(w, 0xb));
            Font_DrawTextBox(F<uint8_t*>(w, 0x63), 0, F<const char*>(e, 0), tx, ty, F<float>(w, 0x156), F<float>(w, 0x15a), color, w[0x2b], w[0x2c], 0, 1.0f);
        }
        int32_t arrow = F<int32_t>(w, 0x1a8) ? F<int32_t>(w, 0x132) : F<int32_t>(w, 0x12e);
        r = F<uint8_t*>(w, 0x86);
        float ay = (float)((double)F<float>(r, 9) + F<float>(w, 0x14a) + F<float>(w, 0xf));
        glyph(arrow, (float)((double)F<float>(r, 5) + F<float>(w, 0x146) + F<float>(w, 0xb)), ay);
        return;
    }
    auto metrics = [] {
        uint8_t* g = hook::Global<uint8_t*>(0x6d4024);
        return F<uint8_t*>(g, 0x34) + F<int32_t>(g, 0x11421e) * 0x15;
    };
    const double half = D(0x5c8628), one = D(0x5c8770);
    float pad = (float)(D(0x5c85a8) / (double)F<int32_t>(metrics(), 4));
    for (int32_t pass = 0; pass < 2; ++pass) {
        for (int32_t row = 0; row < F<int32_t>(w, 0xa2); ++row) {
            uint8_t* r = F<uint8_t*>(w, 0x86);
            float left = (float)((double)F<float>(r, 5) + F<float>(w, 0xb) + F<float>(w, 0xa6));
            uint8_t* fm = metrics();
            int32_t fh = F<int32_t>(fm, 4);
            float hf = (float)fh;
            double hd = (double)fh;
            int32_t k = Ftol((double)hf * F<float>(w, 0xb6));
            double gap = ((double)k + D(0x5d0250)) / hd;
            float rowY = (float)(((gap + F<float>(w, 0xb2)) * row + F<float>(r, 9) + F<float>(w, 0xf)) + F<float>(w, 0xaa));
            float right = (float)((double)F<float>(r, 5) + F<float>(w, 0xae) + F<float>(w, 0xb) + F<float>(w, 0xa6));
            double bottomD = (double)rowY + F<float>(w, 0xb2);
            float bottom = (float)bottomD, below = (float)(bottomD + gap);
            bool sep = row < F<int32_t>(w, 0x1de) && row + 1 < F<int32_t>(w, 0xa2) && F<int32_t>(w, 0xe6);
            if (row >= F<int32_t>(w, 0x1de)) continue;
            int32_t node = TreeView_NodeOfRow(w, 0, F<int32_t>(w, 0x1e2) + row);
            if (pass == 1) {
                if (!sep) continue;
                if (F<int32_t>(w, 0x11e)) glyph(F<int32_t>(w, 0x11e), left, bottom);
                else Ui_FillRect(0xff000000, 0, left, bottom, (float)((double)right - left), (float)((double)below - bottom), 3);
                continue;
            }
            float indent = 0.0f;
            uint8_t* nodes = F<uint8_t*>(w, 0x176);
            if (F<int32_t>(w, 0x122)) {
                uint8_t* e = nodes + node * 32;
                int32_t depth = 0;
                while (F<int32_t>(e, 0x10)) {
                    e = nodes + F<int32_t>(e, 0x14) * 32;
                    ++depth;
                }
                indent = (float)((double)(depth + 1) * D(0x5c8918));
            }
            uint8_t* e = F<uint8_t*>(w, 0x176) + node * 32;
            if (F<int32_t>(e, 0x1c)) {
                int32_t fw = F<int32_t>(fm, 0);
                double wd = (double)fw;
                double invW = one / wd;
                float invWf = (float)invW;
                double invH = one / hd;
                float invHf = (float)invH;
                float wf = (float)fw;
                double cx = (double)indent + left - D(0x5c85b8);
                double v1 = (double)Ftol((cx + invW * half) * wd) / wf;
                float v1f = (float)v1;
                double v2 = (double)Ftol(((double)F<float>(w, 0xb2) * half + rowY + invH * half) * hd) / hf;
                float v2f = (float)v2;
                int32_t n = Ftol(wd * D(0x5ca2f0) + one);
                if (!(n & 1)) ++n;
                float boxW = (float)((double)n / wf);
                int32_t m = Ftol(hd * D(0x5ca2e0) + one);
                if (!(m & 1)) ++m;
                float boxH = (float)((double)m / hf);
                Ui_FrameRect(0xffcccccc, 1, (float)(v1 - (double)boxW * half), (float)(v2 - (double)boxH * half), boxW, boxH, (float)(wd * D(0x5cab68)), 3);
                fm = metrics();
                int32_t a = Ftol((double)F<int32_t>(fm, 0) * D(0x5ca300));
                int32_t b = Ftol((double)F<int32_t>(fm, 4) * D(0x5ca2f8));
                if (!(a & 1)) ++a;
                if (!(b & 1)) ++b;
                int32_t thick = F<int32_t>(fm, 0) > 0x500 ? 3 : 1;
                float thickF = (float)thick, flagF = (float)(thick >= 3);
                double am1 = (double)(a - 1);
                Ui_FillRect(0xffcccccc, 0, (float)(v1 - (double)invWf * am1 * half), (float)((double)v2f - (double)flagF * invHf), (float)((double)a * invWf),
                            (float)((double)thickF * invHf), 3);
                if (!F<int32_t>(F<uint8_t*>(w, 0x176) + node * 32, 0x18))
                    Ui_FillRect(0xffcccccc, 0, (float)((double)v1f - (double)flagF * invWf), (float)(v2 - (double)invHf * am1 * half),
                                (float)((double)thickF * invWf), (float)((double)b * invHf), 3);
            }
            if (auto cb = F<void(__fastcall*)(int32_t, int32_t, float, float, float, uint32_t, int32_t)>(w, 0x1ce)) {
                int32_t isSel = node == F<int32_t>(w, 0x1d6);
                cb(F<uint16_t>(w, 4), F<int32_t>(F<uint8_t*>(w, 0x176) + node * 32, 8), (float)((double)indent + left), rowY,
                   (float)((double)F<float>(w, 0xae) - indent), F<uint32_t>(w, 0xb2), isSel);
            } else {
                uint32_t color = F<uint32_t>(w, 0xf6) && node == F<int32_t>(w, 0x1d6) ? F<uint32_t>(w, 0xf6) : F<uint32_t>(w, 0x38);
                uint8_t* en = F<uint8_t*>(w, 0x176) + node * 32;
                if (F<uint32_t>(en, 0xc)) color = F<uint32_t>(en, 0xc);
                uint8_t align = w[0x2b];
                double inset = align == 2 ? BitsD(0x3f770a3d70a3d70aull) : 0.0;
                double lead = align == 2 ? D(0x5de358) : D(0x5c8608);
                float tw = (float)((double)F<float>(w, 0xae) - (inset + indent));
                float tx = (float)((lead + indent) + left);
                Font_DrawTextBox(F<uint8_t*>(w, 0x63), 0, F<const char*>(en, 0), tx, rowY, tw, F<float>(w, 0xb2), color, align, w[0x2c], 0, 1.0f);
            }
            if (F<int32_t>(w, 0xe2) && node == F<int32_t>(w, 0x1d6)) {
                float end = sep ? below : bottom;
                Ui_FillRect(0x40000000, 0, (float)((double)indent + left), (float)((double)rowY - pad), (float)((double)right - left - indent),
                            (float)((double)pad + pad + end - rowY), 3);
            }
        }
    }
}
RT3_REPLACE(0x5603e0, TreeView_Draw);

// 0x560bf0 (vtable). Mouse at (x, y) over tree view w: finds the row under
// it inside the list area (+0xa6 / +0xaa, width +0xae, rows of List_RowHeight
// + gap +0xb2, +0xa2 visible from +0x1e2; the fixed node +0x1d6 with +0x126)
// and, for a click, runs the row callback (+0x1ba, with the node's data) or
// the area callback (+0x9e); hovering it (every move while it is the hovered
// widget 0x6d401c +0xc6d, else on a new row) runs the hover callback (+0x96), then shows the row's tooltip (node +4).
void __fastcall TreeView_OnMouse(uint8_t* w, int, float x, float y, int32_t click, int32_t hover) {
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const float lx = (float)((double)x - ((double)F<float>(p, 5) + F<float>(w, 0xb)));
    const float ly = (float)((double)y - ((double)F<float>(p, 9) + F<float>(w, 0xf)));
    int32_t row = -1, col = -1;
    if (lx >= F<float>(w, 0xa6) && ly >= F<float>(w, 0xaa) && lx < (double)F<float>(w, 0xae) + F<float>(w, 0xa6)) {
        const float bottom = (float)(((double)List_RowHeight(w) + F<float>(w, 0xb2)) * F<int32_t>(w, 0xa2) + F<float>(w, 0xaa));
        if (bottom - List_RowHeight(w) > ly) {
            const float dy = (float)((double)ly - F<float>(w, 0xaa));
            const int32_t r = (int16_t)(Ftol(dy / (List_RowHeight(w) + F<float>(w, 0xb2))) + F<uint16_t>(w, 0x1e2));
            row = r >= F<int32_t>(w, 0x1da) ? -1 : TreeView_NodeOfRow(w, 0, r);
            if (F<uint32_t>(w, 0x126)) row = (row & 0xffff0000) | F<uint16_t>(w, 0x1d6);
            col = 0;
        }
    }
    const int16_t r16 = (int16_t)row, c16 = (int16_t)col;
    if (click) {
        auto rowCb = F<void(__fastcall*)(uint32_t, int32_t, uint32_t, float, float)>(w, 0x1ba);
        if (rowCb && r16 >= 0 && r16 < F<int32_t>(w, 0x1d2)) {
            rowCb(F<uint16_t>(w, 4), r16, F<uint32_t>(F<uint8_t*>(w, 0x176) + r16 * 32, 8), lx, ly);
        } else if (auto areaCb = F<void(__fastcall*)(uint32_t, uint32_t, float, float)>(w, 0x9e)) {
            areaCb(F<uint16_t>(w, 4), Bits(ly), lx, ly);
        }
    }
    if (hover) {
        if (auto hoverCb = F<void(__fastcall*)(uint32_t, int32_t, int32_t, float, float, int32_t, int32_t)>(w, 0x96)) {
            uint8_t* over = F<uint8_t*>(hook::Global<uint8_t*>(0x6d401c), 0xc6d);
            if (over == w || r16 != F<int16_t>(w, 0x7e) || c16 != F<int16_t>(w, 0x80)) {
                p = F<uint8_t*>(w, 0x86);
                const float hy = (float)((double)y - F<float>(p, 9) + F<float>(w, 0xf));
                const float hx = (float)((double)x - F<float>(p, 5) + F<float>(w, 0xb));
                hoverCb(F<uint16_t>(w, 4), over == w ? 1 : 0, 1, hx, hy, r16, c16);
            }
        }
    }
    F<int16_t>(w, 0x7e) = r16;
    F<int16_t>(w, 0x80) = c16;
    Widget_OnHover(w, 0, x, y, 0, 0);
    if (r16 < 0 || r16 >= F<int32_t>(w, 0x1d2)) return;
    const char* tip = F<const char*>(F<uint8_t*>(w, 0x176) + r16 * 32, 4);
    if (strlen(tip) > 1) Ui_ShowTooltip(hook::Global<uint8_t*>(0x6d401c), 0, tip, 1, 1, -1);
}
RT3_REPLACE(0x560bf0, TreeView_OnMouse);

// 0x560e50. Refreshes the list's visible rows: releases the old ones (callback
// +0x1c6 per row item), takes +0x1de rows from +0x1e2 (first node +0x18a) and lays
// each out (callback +0x1ca with its rect, row height +0xb6 of the screen + +0xb2).
RT3_NOINLINE void __fastcall ListView_RefreshVisible(uint8_t* tv) {
    if (F<int32_t>(tv, 0x182)) return;
    if (!F<void*>(tv, 0x1ca) && !F<void*>(tv, 0x1c6)) return;
    for (int32_t i = 0; i < F<int32_t>(tv, 0x186); ++i)
        ((void(__fastcall*)(uint8_t*, int32_t, uint32_t))F<void*>(tv, 0x1c6))(tv, F<int32_t>(tv, 0x18a) + i,
                                                                              F<uint32_t*>(tv, 0x18e)[i]);
    int32_t n = F<int32_t>(tv, 0x1de);
    F<int32_t>(tv, 0x186) = n;
    F<int32_t>(tv, 0x18a) = n ? TreeView_NodeOfRow(tv, 0, F<int32_t>(tv, 0x1e2)) : -1;
    if (F<void*>(tv, 0x18e)) Free(F<void*>(tv, 0x18e));
    if (!F<int32_t>(tv, 0x1de)) {
        F<void*>(tv, 0x18e) = nullptr;
        return;
    }
    uint8_t* eng = hook::Global<uint8_t*>(0x6d4024);
    int32_t screenH = F<int32_t>(F<uint8_t*>(eng, 0x34) + F<int32_t>(eng, 0x11421e) * 0x15, 4);
    int32_t rowH;
    float rowF = F<float>(tv, 0xb6);
    __asm {
        fld rowF
        fimul screenH
        mov eax, 0x5a10d0
        call eax
        mov rowH, eax
    }
    float step = (float)(((double)rowH + *(const double*)0x5d0250) / screenH);
    F<uint32_t*>(tv, 0x18e) = (uint32_t*)Malloc(F<int32_t>(tv, 0x1de) * 4);
    for (int32_t i = 0; i < F<int32_t>(tv, 0x1de); ++i) {
        float x0 = (float)((double)F<float>(tv, 0xb) + F<float>(tv, 0xa6));
        double t = ((double)step + F<float>(tv, 0xb2)) * i;
        float y0 = (float)((double)F<float>(tv, 0xaa) + F<float>(tv, 0xf) + t);
        double x1 = (double)F<float>(tv, 0xae) + F<float>(tv, 0xb) + F<float>(tv, 0xa6);
        double y1 = (double)F<float>(tv, 0xaa) + F<float>(tv, 0xf) + t + F<float>(tv, 0xb2);
        int32_t node = TreeView_NodeOfRow(tv, 0, F<int32_t>(tv, 0x1e2) + i);
        float h = (float)(y1 - y0);
        uint32_t item = F<uint32_t>(F<uint8_t*>(tv, 0x176) + node * 32, 8);
        F<uint32_t*>(tv, 0x18e)[i] = item;
        float w = (float)(x1 - x0);
        ((void(__fastcall*)(uint8_t*, int32_t, uint32_t, float, float, float, float))F<void*>(tv, 0x1ca))(
            tv, F<int32_t>(tv, 0x18a) + i, F<uint32_t*>(tv, 0x18e)[i], x0, y0, w, h);
    }
}
RT3_REPLACE(0x560e50, ListView_RefreshVisible);

// 0x561050 (list widget hover). Turns the point (x, y) relative to the
// parent (+0x86) into the hovered row's node (in place of the third
// argument) when it lies over a visible row, then lets the base handle it.
void __fastcall ListWidget_Hover(uint8_t* w, int, float x, float y, int32_t node) {
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const int32_t ix = Ftol((double)x - ((double)F<float>(p, 5) + (double)F<float>(w, 0xb)));
    const int32_t iy = Ftol((double)y - ((double)F<float>(p, 9) + (double)F<float>(w, 0xf)));
    if ((double)ix >= (double)F<float>(w, 0xa6) && (double)iy >= (double)F<float>(w, 0xaa) &&
        (double)ix < (double)F<float>(w, 0xae) + (double)F<float>(w, 0xa6)) {
        const float fy = (float)iy;
        const double bottom = (List_RowHeight(w) + (double)F<float>(w, 0xb2)) * (double)F<int32_t>(w, 0xa2) + (double)F<float>(w, 0xaa);
        if (bottom > (double)fy) {
            const float d = (float)((double)fy - (double)F<float>(w, 0xaa));
            const int32_t row = Ftol((double)d / (List_RowHeight(w) + (double)F<float>(w, 0xb2)) + (double)F<int32_t>(w, 0x1e2));
            if (row < F<int32_t>(w, 0x1da)) node = TreeView_NodeOfRow(w, 0, row);
        }
    }
    uint32_t bx, by;
    memcpy(&bx, &x, 4), memcpy(&by, &y, 4);
    Nop558460(w, 0, (int32_t)bx, (int32_t)by, node);
}
RT3_REPLACE(0x561050, ListWidget_Hover);

// 0x561150. Refreshes a list view's rows and scrollbar; with `toEnd` on a list
// that keeps its end in view (+0x33 bit 8) also jumps to the last row (+0x1e2 =
// +0x1e6, bar position +0x185 = its range - 1).
RT3_NOINLINE void __fastcall ListView_Refresh(uint8_t* l, int, int32_t toEnd) {
    ListView_RefreshVisible(l);
    List_SyncScrollbar(l);
    if (!(F<uint32_t>(l, 0x33) & 0x100) || !toEnd) return;
    F<int32_t>(l, 0x1e2) = F<int32_t>(l, 0x1e6);
    if (uint8_t* bar = F<uint8_t*>(l, 0x19a)) F<int32_t>(bar, 0x185) = F<int32_t>(bar, 0x101) - 1;
}
RT3_REPLACE(0x561150, ListView_Refresh);

// 0x5611a0. Opens a dropdown (once; +0x1a8) with a click sound (6, muted
// while 0xccba14 counts up): enough rows of height +0xb2 + gap +0xb6 for its
// +0x1d2 items, at most +0xa2 (+0x1ac when cut, adding a scrollbar), drawn as
// frame pieces under the box (images +0x136 / +0x13a, scrolling +0x13e /
// +0x142, the last one of cap height), and a list (0x1ea bytes, id +0x1a6)
// over them taking the dropdown's list settings (+0xa2..), its items
// (+0x176, 0x20 bytes: text, data +8, value +4, colour +0xc), the open time
// +0x17e and the selection +0x1d6.
RT3_NOINLINE void __fastcall Dropdown_Open(uint8_t* c) {
    if (F<int32_t>(c, 0x1a8)) return;
    Ui_PlaySound(6, 0, 0x3f000000);
    ++hook::Global<int32_t>(0xccba14);
    float rowH = (float)((double)F<float>(c, 0xb2) + F<float>(c, 0xb6));
    float capH = (float)Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(c, 0x92), 0, F<int32_t>(c, 0x13a)));
    int32_t n = F<int32_t>(c, 0x1d2);
    int32_t rows = Ftol(((double)(n - 1) * F<float>(c, 0xb6) + (double)n * F<float>(c, 0xb2)) / rowH) + 1;
    F<int32_t>(c, 0x1ac) = 0;
    if (rows > F<int32_t>(c, 0xa2)) {
        rows = F<int32_t>(c, 0xa2);
        F<int32_t>(c, 0x1ac) = 1;
    }
    float ty = (float)((double)F<float>(c, 0xf) + F<float>(c, 0x17));
    for (int32_t j = 0; j < rows + 1; ++j) {
        bool last = j == rows;
        if (last) ty = (float)((double)ty - F<float>(c, 0xb6));
        uint8_t d[0x89];
        memset(d, 0, sizeof d);
        F<uint32_t>(d, 7) = F<uint32_t>(c, 0xb);
        d[4] = (uint8_t)(c[8] + 1);
        F<float>(d, 0xb) = ty;
        F<uint8_t*>(d, 0x47) = c;
        if (F<int32_t>(c, 0x1ac)) F<uint16_t>(d, 0x5f) = last ? F<uint16_t>(c, 0x142) : F<uint16_t>(c, 0x13e);
        else F<uint16_t>(d, 0x5f) = last ? F<uint16_t>(c, 0x13a) : F<uint16_t>(c, 0x136);
        F<uint16_t>(d, 0) = (uint16_t)HandleTable_Acquire(hook::Global<uint8_t*>(0x6d401c), 0, F<uint16_t>(c, 4));
        String_Assign(&F<char*>(d, 0x43), F<const char*>(c, 0x47));
        uint8_t* f = (uint8_t*)ExeNew(0x111);
        f = f ? Scrollbar_Construct(f, 0, d) : nullptr;
        Window_AddChild(F<uint8_t*>(c, 0x86), 0, f, 0);
        Free(F<void*>(d, 0x43));
        ty = (float)((double)(last ? capH : rowH) + ty);
    }
    uint8_t p[0x12f];
    memset(p, 0, sizeof p);
    F<float>(p, 0xb) = (float)((double)F<float>(c, 0xf) + F<float>(c, 0x17));
    F<uint32_t>(p, 7) = F<uint32_t>(c, 0xb);
    p[0x33] = c[0x37];
    p[4] = (uint8_t)(c[8] + 1);
    F<uint32_t>(p, 0xf) = F<uint32_t>(c, 0x13);
    F<float>(p, 0x13) = (float)((double)rows * rowH + capH);
    F<uint32_t>(p, 0x2f) = F<uint32_t>(c, 0x33);
    F<uint32_t>(p, 0x34) = F<uint32_t>(c, 0x38);
    p[0x3e] = c[0x42];
    uint16_t id = (uint16_t)HandleTable_Acquire(hook::Global<uint8_t*>(0x6d401c), 0, F<uint16_t>(c, 4));
    F<uint16_t>(p, 0) = id;
    F<uint16_t>(c, 0x1a6) = id;
    memcpy(p + 0x5f, c + 0xa2, 0xd0);
    F<uint8_t*>(p, 0x47) = c;
    F<int32_t>(p, 0xe3) = 0;
    F<int32_t>(p, 0xa7) = 1;
    if (!F<int32_t>(c, 0x1ac)) F<uint32_t>(p, 0x2f) |= 0x8000;
    String_Assign(&F<char*>(p, 0x43), F<const char*>(c, 0x47));
    uint8_t* lst = (uint8_t*)ExeNew(0x1ea);
    lst = lst ? TreeView_Construct(lst, 0, p) : nullptr;
    for (int32_t k = 0; k < F<int32_t>(c, 0x1d2); ++k) {
        uint8_t* item = F<uint8_t*>(c, 0x176) + k * 0x20;
        Widget_Send(lst, 0, 0x88, F<uint16_t>(c, 0x1a6), F<uint32_t>(item, 0), F<uint32_t>(item, 8), F<uint32_t>(item, 4));
        if (uint32_t col = F<uint32_t>(F<uint8_t*>(c, 0x176) + k * 0x20, 0xc)) Widget_Send(lst, 0, 0xaf, F<uint16_t>(c, 0x1a6), (uint32_t)k, col, 0);
    }
    F<int32_t>(lst, 0x19e) = 1;
    F<int32_t>(lst, 0x17a) = 1;
    Window_AddChild(F<uint8_t*>(c, 0x86), 0, lst, 0);
    Free(F<void*>(p, 0x43));
    F<int32_t>(lst, 0x17e) = ClockMs();
    if (F<int32_t>(c, 0x1d6) != -1) {
        Widget_Send(lst, 0, 0x66, F<uint16_t>(c, 0x1a6), F<uint32_t>(c, 0x1d6), 0, 0);
        Widget_Send(lst, 0, 0x6b, F<uint16_t>(c, 0x1a6), F<uint32_t>(c, 0x1d6), 0, 0);
    }
    F<int32_t>(c, 0x1a8) = 1;
    --hook::Global<int32_t>(0xccba14);
}
RT3_REPLACE(0x5611a0, Dropdown_Open);

void __fastcall Thunk5615e0(void* p0) { Nop558750(p0); }
RT3_REPLACE(0x5615e0, Thunk5615e0);

// 0x5615f0. Tree view destructor: closes its popup (+0x1a8: click sound 7, its
// children, its handle), removes the edit box +0x19a, lets the item callback
// +0x1c6 release every item (+0x18e values, ids from +0x18a), frees the rows
// (+0x176, 32 bytes, count +0x1d2) and its strings, then the widget.
void __fastcall TreeView_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5de318;
    if (F<int32_t>(w, 0x1a8)) {
        Ui_PlaySound(7, 0, 0x3f000000);
        Window_DeleteChildren(F<uint8_t*>(w, 0x86), 0, 0, 0xffff, (uint32_t)(uintptr_t)w);
        HandleTable_ReleaseId(hook::Global<uint8_t*>(0x6d401c), 0, F<uint16_t>(w, 4));
        F<int32_t>(w, 0x1a8) = 0;
    }
    if (uint8_t* edit = F<uint8_t*>(w, 0x19a)) {
        HandleTable_Release(hook::Global<uint8_t*>(0x6d401c), 0, F<uint16_t>(edit, 0x7c));
        Window_DeleteChild(F<uint8_t*>(w, 0x86), 0, F<uint8_t*>(w, 0x19a));
    }
    F<uint8_t*>(w, 0x19a) = nullptr;
    for (int32_t i = 0; i < F<int32_t>(w, 0x186); ++i)
        if (auto cb = F<void(__fastcall*)(uint8_t*, int32_t, uint32_t)>(w, 0x1c6)) cb(w, F<int32_t>(w, 0x18a) + i, F<uint32_t*>(w, 0x18e)[i]);
    for (int32_t k = 0; k < F<int32_t>(w, 0x1d2); ++k) {
        Free(F<void*>(F<uint8_t*>(w, 0x176) + k * 0x20, 4));
        Free(F<void*>(F<uint8_t*>(w, 0x176) + k * 0x20, 0));
    }
    if (void* rows = F<void*>(w, 0x176)) Free(rows);
    F<void*>(w, 0x176) = nullptr;
    String_Free(&F<char*>(w, 0x47));
    F<int32_t>(w, 0x1de) = 0;
    uint8_t* mgr = hook::Global<uint8_t*>(0x6d401c);
    if (F<uint8_t*>(mgr, 0xc75) == w) F<uint8_t*>(mgr, 0xc75) = nullptr;
    ListView_RefreshVisible(w);
    Widget_Destruct(w);
}
RT3_REPLACE(0x5615f0, TreeView_Destruct);

// 0x561740. Renders a tree view: draws it at its pixel box, then (debug flag
// 0x6d401c +0xc79 for its group +6) outlines it and its list area (+0x1b..).
void __fastcall TreeView_Render(uint8_t* w) {
    uint8_t* r = F<uint8_t*>(w, 0x86);
    int32_t y1 = Ftol((double)F<float>(r, 9) + F<float>(w, 0xf) + F<float>(w, 0x17) - Fl(0x5c85b4));
    int32_t x1 = Ftol((double)F<float>(w, 0xb) + F<float>(w, 0x13) + F<float>(r, 5) - Fl(0x5c85b4));
    int32_t y0 = Ftol((double)F<float>(r, 9) + F<float>(w, 0xf));
    int32_t x0 = Ftol((double)F<float>(w, 0xb) + F<float>(r, 5));
    TreeView_Draw(w, 0, x0, y0, x1, y1);
    if (!(F<uint32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc79) & F<uint16_t>(w, 6)) || !Widget_IsVisible(w)) return;
    r = F<uint8_t*>(w, 0x86);
    float y = (float)((double)F<float>(r, 9) + F<float>(w, 0xf));
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(r, 5) + F<float>(w, 0xb)), y, F<float>(w, 0x13), F<float>(w, 0x17), 1.0f, 3);
    if (F<float>(w, 0x23) == Fl(0x5c8598)) return;
    r = F<uint8_t*>(w, 0x86);
    y = (float)((double)F<float>(r, 9) + F<float>(w, 0x1f));
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(r, 5) + F<float>(w, 0x1b)), y, F<float>(w, 0x23), F<float>(w, 0x27), 1.0f, 3);
}
RT3_REPLACE(0x561740, TreeView_Render);

// 0x561840. List box l: selects node `node` (clamped to 0..+0x1e6; the row
// is kept in +0x1e2). Its drop-down +0x19a follows silently, `sound` plays
// the click (+0xfa), the selection callback +0x1c2 gets (row, item data +8 of
// the 0x20-byte items +0x176) or (-1, -1), and the visible rows refresh.
RT3_NOINLINE void __fastcall ListBox_Select(uint8_t* l, int, int32_t node, int32_t sound) {
    typedef void(__fastcall * Selected)(int32_t, int32_t);
    if (node < 0) node = 0;
    if (node > F<int32_t>(l, 0x1e6)) node = F<int32_t>(l, 0x1e6);
    if (node == F<int32_t>(l, 0x1e2)) return;
    F<int32_t>(l, 0x1e2) = TreeView_RowOfNode(l, 0, node);
    if (uint8_t* d = F<uint8_t*>(l, 0x19a)) Dropdown_Select(d, 0, F<int32_t>(l, 0x1e2), 0, 0);
    if (sound) Widget_PlaySound(l, F<int32_t>(l, 0xfa));
    if (Selected cb = F<Selected>(l, 0x1c2)) {
        int32_t row = F<int32_t>(l, 0x1e2);
        if (row == -1) cb(-1, -1);
        else cb(row, F<int32_t>(F<uint8_t*>(l, 0x176) + row * 0x20, 8));
    }
    ListView_RefreshVisible(l);
}
RT3_REPLACE(0x561840, ListBox_Select);

// 0x5618f0. List box l: selects row `row` (clamped to the +0x1d2 rows; -1:
// none) as +0x1d6, mirroring it to the bound ints +0x1b2 (row) / +0x1b6 (item
// data +8). Re-selecting only calls the callback +0x1be when `renotify`.
// Otherwise scrolls the row into view (+0x1e2 top, +0xa2 visible rows), plays
// the click (+0xfa) when `sound` and it did not scroll, calls the callback
// (row, data) and posts input event 0xce (id +4, row, data, l).
RT3_NOINLINE void __fastcall ListBox_SetSelection(uint8_t* l, int, int32_t row, int32_t sound, int32_t renotify) {
    typedef void(__fastcall * Selected)(int32_t, int32_t);
    auto data = [&](int32_t r) { return r != -1 && F<uint8_t*>(l, 0x176) ? F<int32_t>(F<uint8_t*>(l, 0x176) + r * 0x20, 8) : -1; };
    int32_t last = F<int32_t>(l, 0x1d2) - 1;
    if (row >= last) row = last;
    if (int32_t* p = F<int32_t*>(l, 0x1b2)) *p = row;
    if (int32_t* q = F<int32_t*>(l, 0x1b6)) *q = data(row);
    if (F<int32_t>(l, 0x1d6) == row) {
        if (renotify)
            if (Selected cb = F<Selected>(l, 0x1be)) cb(F<int32_t>(l, 0x1d6), data(F<int32_t>(l, 0x1d6)));
        return;
    }
    int32_t scrolled = 0;
    if (row != -1) {
        int32_t top = F<int32_t>(l, 0x1e2);
        if (row < top) {
            scrolled = 1;
            ListBox_Select(l, 0, row, 1);
        } else if (row >= top + F<int32_t>(l, 0xa2)) {
            scrolled = 1;
            ListBox_Select(l, 0, row - F<int32_t>(l, 0xa2) + 1, 1);
        }
    }
    F<int32_t>(l, 0x1d6) = row;
    if (!scrolled && row != -1 && sound) Widget_PlaySound(l, F<int32_t>(l, 0xfa));
    if (Selected cb = F<Selected>(l, 0x1be)) cb(F<int32_t>(l, 0x1d6), data(F<int32_t>(l, 0x1d6)));
    uint32_t ev[12] = {};
    ev[0] = 0xce;
    ev[1] = F<uint16_t>(l, 4);
    ev[2] = F<uint32_t>(l, 0x1d6);
    ev[3] = (uint32_t)data(F<int32_t>(l, 0x1d6));
    ev[11] = (uint32_t)(uintptr_t)l;
    if (uint8_t* in = hook::Global<uint8_t*>(0x6d4018)) Input_PushEvent(in, 0, ev);
}
RT3_REPLACE(0x5618f0, ListBox_SetSelection);

// 0x561aa0. Appends a row (32 bytes: two strings, copied; +0x10 set = child of row
// +0x14, which gets its has-children flag +0x1c; the new row's own starts clear)
// to tree view tv's rows (+0x176,
// count +0x1d2, grown by one each time). A visible row counts towards the shown
// rows (+0x1da), scrolling (+0x1e6) once they overflow the view (+0xa2); then the
// view refreshes and, if it was scrolled to the end and keeps the end in view
// (+0x33 bit 8), stays there.
RT3_NOINLINE void __fastcall TreeView_AddRow(uint8_t* tv, int, const uint32_t* row) {
    uint32_t r[8];
    memcpy(r, row, sizeof r);
    r[0] = 0;
    String_Assign((char**)&r[0], row[0] ? (const char*)row[0] : (const char*)0x5c87a8);
    r[1] = 0;
    String_Assign((char**)&r[1], row[1] ? (const char*)row[1] : (const char*)0x5c87a8);
    r[7] = 0;
    if (r[4]) F<int32_t>(F<uint8_t*>(tv, 0x176) + r[5] * 0x20, 0x1c) = 1;
    uint8_t* rows = (uint8_t*)((void*(__cdecl*)(size_t))0x5a125d)((F<int32_t>(tv, 0x1d2) + 1) * 0x20);
    if (F<int32_t>(tv, 0x1d2)) memcpy(rows, F<uint8_t*>(tv, 0x176), F<int32_t>(tv, 0x1d2) * 0x20);
    if (F<void*>(tv, 0x176)) Free(F<void*>(tv, 0x176));
    memcpy(rows + F<int32_t>(tv, 0x1d2) * 0x20, r, sizeof r);
    int32_t atEnd = F<int32_t>(tv, 0x1e2) == F<int32_t>(tv, 0x1e6) ? 1 : 0;
    int32_t n = F<int32_t>(tv, 0x1d2) + 1;
    F<uint8_t*>(tv, 0x176) = rows;
    F<int32_t>(tv, 0x1d2) = n;
    if (!TreeView_IsHidden(tv, 0, n - 1)) {
        int32_t shown = ++F<int32_t>(tv, 0x1da);
        if (shown > F<int32_t>(tv, 0xa2)) F<int32_t>(tv, 0x1e6) = shown - F<int32_t>(tv, 0xa2);
    }
    F<int32_t>(tv, 0x1de) = F<int32_t>(tv, 0x1e6) > 0 ? F<int32_t>(tv, 0xa2) : F<int32_t>(tv, 0x1da);
    ListView_RefreshVisible(tv);
    List_SyncScrollbar(tv);
    if (!(F<uint32_t>(tv, 0x33) & 0x100) || !atEnd) return;
    F<int32_t>(tv, 0x1e2) = F<int32_t>(tv, 0x1e6);
    if (uint8_t* bar = F<uint8_t*>(tv, 0x19a)) F<int32_t>(bar, 0x185) = F<int32_t>(bar, 0x101) - 1;
}
RT3_REPLACE(0x561aa0, TreeView_AddRow);

// 0x561c30. Deleting destructor of the tree view.
uint8_t* __fastcall TreeView_DeletingDestructor(uint8_t* w, int, uint32_t flags) {
    TreeView_Destruct(w);
    if (flags & 1) ExeDelete(w);
    return w;
}
RT3_REPLACE(0x561c30, TreeView_DeletingDestructor);

// 0x561c50. List box l: removes item i (its two strings and the 0x20-byte
// slot; the array goes with the last item), keeping the scroll position, the
// hot item +0x192 and the selection in step; a removed selection moves to
// the item at its row (or before it), or with no items left sends itself
// event 0x66 (-1). QUIRK: the original leaves the rest of that event
// uninitialised (stack residue); here it is zero.
RT3_NOINLINE void __fastcall ListBox_RemoveItem(uint8_t* l, int, int32_t i) {
    if (i >= F<int32_t>(l, 0x1d2)) return;
    int32_t row = 0;
    if (!TreeView_IsHidden(l, 0, i)) {
        row = TreeView_RowOfNode(l, 0, i);
        if (F<int32_t>(l, 0x1d6) == i) F<int32_t>(l, 0x192) = -1;
        int32_t top = F<int32_t>(l, 0x1e2);
        if (top == row && F<int32_t>(l, 0x1e6) <= top) ListBox_Select(l, 0, TreeView_NodeOfRow(l, 0, top - 1), 1);
        if (F<int32_t>(l, 0x1e6) > 0) --F<int32_t>(l, 0x1e6);
        ListBox_Select(l, 0, TreeView_NodeOfRow(l, 0, F<int32_t>(l, 0x1e2)), 0);
        --F<int32_t>(l, 0x1da);
    }
    int32_t n = --F<int32_t>(l, 0x1d2);
    Free(F<void*>(F<uint8_t*>(l, 0x176) + i * 0x20, 4));
    Free(F<void*>(F<uint8_t*>(l, 0x176) + i * 0x20, 0));
    if (F<int32_t>(l, 0x1d2)) {
        if (n - i > 0) memmove(F<uint8_t*>(l, 0x176) + i * 0x20, F<uint8_t*>(l, 0x176) + i * 0x20 + 0x20, (size_t)(n - i) * 0x20);
    } else {
        Free(F<void*>(l, 0x176));
        F<void*>(l, 0x176) = nullptr;
    }
    F<int32_t>(l, 0x1de) = F<int32_t>(l, 0x1da) < F<int32_t>(l, 0xa2) ? F<int32_t>(l, 0x1da) : F<int32_t>(l, 0xa2);
    if (F<int32_t>(l, 0x192) > i) --F<int32_t>(l, 0x192);
    else if (F<int32_t>(l, 0x192) == i) F<int32_t>(l, 0x192) = -1;
    int32_t sel = F<int32_t>(l, 0x1d6);
    if (sel > i) {
        if (sel > 0) ListBox_SetSelection(l, 0, sel - 1, 1, 0);
    } else if (sel == i) {
        if (F<int32_t>(l, 0x1d2) > 0) {
            if (sel >= F<int32_t>(l, 0x1d2)) F<int32_t>(l, 0x1d6) = row > 0 ? TreeView_NodeOfRow(l, 0, row - 1) : 0;
            ListBox_SetSelection(l, 0, F<int32_t>(l, 0x1d6), 1, 0);
        } else {
            uint32_t ev[12] = {};
            ev[0] = 0x66;
            ev[1] = F<uint16_t>(l, 4);
            ev[2] = 0xffffffff;
            ((void(__fastcall*)(uint8_t*, int, uint32_t*))(*(void***)l)[1])(l, 0, ev);
        }
    }
    ListView_RefreshVisible(l);
    List_SyncScrollbar(l);
}
RT3_REPLACE(0x561c50, ListBox_RemoveItem);

// 0x561e40. Tree view l: expands or collapses item `node` (+0x18; only
// items with children, +0x1c). Recounts the visible rows (+0x1da, the page
// +0x1de: +0xa2 when scrolling, +0x1e6), moves a selection hidden under a
// collapsed ancestor up to that parent, and syncs the scroll bar.
RT3_NOINLINE void __fastcall TreeView_SetExpanded(uint8_t* l, int, int32_t node, int32_t expanded) {
    uint8_t* it = F<uint8_t*>(l, 0x176) + node * 0x20;
    if (!F<int32_t>(it, 0x1c) || F<int32_t>(it, 0x18) == expanded) return;
    F<int32_t>(it, 0x18) = expanded;
    F<int32_t>(l, 0x1da) = 0;
    for (int32_t i = 0; i < F<int32_t>(l, 0x1d2); ++i)
        if (!TreeView_IsHidden(l, 0, i)) ++F<int32_t>(l, 0x1da);
    F<int32_t>(l, 0x1de) = F<int32_t>(l, 0x1e6) > 0 ? F<int32_t>(l, 0xa2) : F<int32_t>(l, 0x1da);
    int32_t sel = F<int32_t>(l, 0x1d6);
    if (sel >= 0) {
        uint8_t* items = F<uint8_t*>(l, 0x176);
        for (;;) {
            uint8_t* s = items + sel * 0x20;
            if (!F<int32_t>(s, 0x10)) break;
            uint8_t* a = s;
            bool hidden = false;
            for (;;) {
                uint8_t* p = items + F<int32_t>(a, 0x14) * 0x20;
                if (!F<int32_t>(p, 0x18)) {
                    hidden = true;
                    break;
                }
                items = F<uint8_t*>(l, 0x176);
                if (!F<int32_t>(items + F<int32_t>(a, 0x14) * 0x20, 0x10)) break;
                a = p;
            }
            if (!hidden) break;
            sel = F<int32_t>(s, 0x14);
        }
        if (sel != F<int32_t>(l, 0x1d6)) ListBox_SetSelection(l, 0, sel, 1, 0);
    }
    List_SyncScrollbar(l);
}
RT3_REPLACE(0x561e40, TreeView_SetExpanded);

// 0x561f30. List box l (enabled, +0xde): keys. Enter on a selection other
// than the first posts change event 0xca (when +0x33 has bit 7); Home / End
// / Up / Down move the selection (row-based for End / Up / Down) and keep it
// in view. 0 when the selection moved, else 1.
RT3_NOINLINE int32_t __fastcall ListBox_OnKey(uint8_t* l, int, uint32_t* ev) {
    if (!F<int32_t>(l, 0xde)) return 1;
    int32_t n = F<int32_t>(l, 0x1d2);
    if (!n) return 1;
    int32_t old = F<int32_t>(l, 0x1d6);
    if (old == -1) F<int32_t>(l, 0x1d6) = 0;
    int32_t node;
    switch (ev[2]) {
    case 0xd:
        if (F<int32_t>(l, 0x1d6) && (int8_t)l[0x33] < 0) {
            ev[0] = 0xca;
            ev[1] = F<uint16_t>(l, 4);
            ev[11] = (uint32_t)(uintptr_t)l;
            int32_t s = F<int32_t>(l, 0x1d6);
            ev[2] = (uint32_t)s;
            ev[3] = s == -1 ? 0xffffffff : F<uint32_t>(F<uint8_t*>(l, 0x176) + s * 0x20, 8);
            ev[8] = 2;
        }
        return 1;
    case 0x24: node = 0; break;
    case 0x23: node = TreeView_NodeOfRow(l, 0, F<int32_t>(l, 0x1da) - 1); break;
    case 0x26: {
        int32_t r = F<int32_t>(l, 0x1d6) - 1;
        node = TreeView_NodeOfRow(l, 0, r < 0 ? 0 : r);
        break;
    }
    case 0x28: {
        int32_t r = F<int32_t>(l, 0x1da) - 1, nx = F<int32_t>(l, 0x1d6) + 1;
        node = TreeView_NodeOfRow(l, 0, r < nx ? r : nx);
        break;
    }
    default:
        return 1;
    }
    if (old == -1 && (node < 0 || node >= n)) {
        F<int32_t>(l, 0x1d6) = -1;
        return 1;
    }
    int32_t sel = node > 0 ? node : 0;
    if (sel >= n - 1) sel = n - 1;
    if (sel == F<int32_t>(l, 0x1d6)) return 0;
    int32_t row = TreeView_RowOfNode(l, 0, F<int32_t>(l, 0x1d6));
    ListBox_SetSelection(l, 0, sel, 1, 0);
    int32_t top = F<int32_t>(l, 0x1e2);
    if (row < top) {
        ListBox_Select(l, 0, F<int32_t>(l, 0x1d6), 1);
        return 0;
    }
    if (row > F<int32_t>(l, 0xa2) + top - 1) ListBox_Select(l, 0, TreeView_NodeOfRow(l, 0, row - F<int32_t>(l, 0xa2) + 1), 1);
    return 0;
}
RT3_REPLACE(0x561f30, ListBox_OnKey);

// 0x562100. List box / tree view l: mouse event ev (2 move, 6 press, 7
// release; position ev[9], ev[10]). A press on a drop-down (+0x126) opens or
// closes its popup, otherwise it goes to the click callback +0x9a when set,
// else selects the row under the pointer (left of the item's indent,
// TreeView_Depth, it toggles the item's expansion) and starts a drag
// (+0x17a). Dragging (button 0x400) moves the selection along the rows.
// The release ends the drag: a new selection is only remembered (+0x192,
// time +0x196); releasing again on it within 700 ms (or on a list with
// +0x33 bit 24, or outside a list with +0x19e) turns ev into change event
// 0xca, a double click marking it ev[8] = 2. 0 when handled, else 1.
RT3_NOINLINE int32_t __fastcall ListBox_OnMouse(uint8_t* l, int, uint32_t* ev) {
    auto hit = [&]() { return ((int32_t(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, int32_t))(*(void***)l)[8])(l, 0, ev[9], ev[10], 1); };
    auto item = [&](int32_t i) { return F<uint8_t*>(l, 0x176) + i * 0x20; };
    uint8_t* base = F<uint8_t*>(l, 0x86);
    float x = (float)((double)F<float>(ev, 0x24) - ((double)F<float>(base, 5) + F<float>(l, 0xb)));
    float y = (float)((double)F<float>(ev, 0x28) - ((double)F<float>(base, 9) + F<float>(l, 0xf)));
    switch (ev[0]) {
    case 2: {
        if (F<int32_t>(l, 0x19e) && !F<int32_t>(l, 0x1a2) && !hit()) return 1;
        if (!F<int32_t>(l, 0x17a) || !(ev[8] & 0x400)) return 1;
        float t = (float)((double)y - F<float>(l, 0xaa));
        int32_t r = Ftol((double)t / (List_RowHeight(l) + F<float>(l, 0xb2)));
        if (r >= F<int32_t>(l, 0x1de) - 1) r = F<int32_t>(l, 0x1de) - 1;
        int32_t row = F<int32_t>(l, 0x1e2) + (r > 0 ? r : 0);
        if (TreeView_NodeOfRow(l, 0, row) != F<int32_t>(l, 0x1d6)) {
            ListBox_SetSelection(l, 0, TreeView_NodeOfRow(l, 0, row), 1, 0);
            ((void(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, int32_t, int32_t))(*(void***)l)[3])(l, 0, ev[9], ev[10], 1, 1);
        }
        return 0;
    }
    case 6: {
        if (!hit()) {
            if (F<int32_t>(l, 0x1a8)) TreeView_ClosePopup(l);
            return 1;
        }
        if (F<int32_t>(l, 0x126)) {
            if (F<int32_t>(l, 0x1a8)) TreeView_ClosePopup(l);
            else Dropdown_Open(l);
            return 0;
        }
        if (void* cb = F<void*>(l, 0x9a)) {
            float cy = (float)((double)F<float>(ev, 0x28) - ((double)F<float>(base, 9) + F<float>(l, 0xf)));
            float cx = (float)((double)F<float>(ev, 0x24) - ((double)F<float>(base, 5) + F<float>(l, 0xb)));
            ((void(__fastcall*)(uint32_t, void*, float, float))cb)(F<uint16_t>(l, 4), cb, cx, cy);
            return 0;
        }
        if (!F<int32_t>(l, 0x1d2)) return 1;
        if (F<int32_t>(l, 0x172) && !(x < F<float>(l, 0xce))) return 1;
        if (x < F<float>(l, 0xa6) || y < F<float>(l, 0xaa)) return 1;
        if (!((double)F<float>(l, 0xae) + F<float>(l, 0xa6) > x)) return 1;
        if (!((List_RowHeight(l) + F<float>(l, 0xb2)) * (double)F<int32_t>(l, 0xa2) + F<float>(l, 0xaa) > y)) return 1;
        if ((int8_t)l[0x33] < 0) return 1;
        float t = (float)((double)y - F<float>(l, 0xaa));
        int32_t r = Ftol((double)t / (List_RowHeight(l) + F<float>(l, 0xb2)) + (double)F<int32_t>(l, 0x1e2));
        if (r >= F<int32_t>(l, 0x1da)) return 1;
        int32_t node = TreeView_NodeOfRow(l, 0, r);
        int32_t depth = TreeView_Depth(l, 0, node) + 1;
        if (!((double)depth * D(0x5c8918) + F<float>(l, 0xa6) > x)) {
            F<int32_t>(l, 0x17a) = 1;
            if (F<int32_t>(l, 0x1d6) != node) ListBox_SetSelection(l, 0, node, 1, 0);
            return 0;
        }
        if (F<int32_t>(item(node), 0x1c)) TreeView_SetExpanded(l, 0, node, !F<int32_t>(item(node), 0x18));
        return 0;
    }
    case 7: {
        F<int32_t>(l, 0x1a2) = 1;
        if (!F<int32_t>(l, 0x17a)) return 1;
        bool force = false;
        F<int32_t>(l, 0x17a) = 0;
        if (F<int32_t>(l, 0x19e) && hit()) force = true;
        if (F<uint32_t>(l, 0x33) & 0x1000000) force = true;
        if (F<int32_t>(l, 0x1d6) != F<int32_t>(l, 0x192) && !force) {
            F<int32_t>(l, 0x192) = F<int32_t>(l, 0x1d6);
            F<int32_t>(l, 0x196) = ClockMs();
            return 0;
        }
        ev[0] = 0xca;
        ev[11] = (uint32_t)(uintptr_t)l;
        ev[1] = F<uint16_t>(l, 4);
        ev[2] = F<uint32_t>(l, 0x1d6);
        ev[3] = F<int32_t>(l, 0x1d6) == -1 ? 0xffffffff : F<uint32_t>(item(F<int32_t>(l, 0x1d6)), 8);
        if (!force && (F<int32_t>(l, 0x1d6) != F<int32_t>(l, 0x192) || !(F<int32_t>(l, 0x196) + 700 > ClockMs()))) {
            ListBox_SetSelection(l, 0, (int32_t)ev[2], 0, 1);
            F<int32_t>(l, 0x196) = ClockMs();
            return 0;
        }
        ev[8] = 2;
        F<int32_t>(l, 0x196) = ClockMs();
        return 1;
    }
    }
    return 1;
}
RT3_REPLACE(0x562100, ListBox_OnMouse);

// 0x562540. List box l: auto-scrolls while the pointer is held above (one
// row up) or below (one row down) the rows, at most every 225 ms (+0x17e),
// moving the selection along.
RT3_NOINLINE void __fastcall ListBox_AutoScroll(uint8_t* l) {
    if ((uint32_t)(F<int32_t>(l, 0x17e) + 0xe1) > (uint32_t)ClockMs()) return;
    float pos[2];
    Mouse_GetPos((uint8_t*)Engine_Input(hook::Global<void*>(0x6d4024)), 0, (uint32_t*)&pos[1], (uint32_t*)&pos[0]);
    double y = (double)pos[0] - ((double)F<float>(F<uint8_t*>(l, 0x86), 9) + F<float>(l, 0xf));
    pos[0] = (float)y;
    if (y < F<float>(l, 0xaa)) {
        F<int32_t>(l, 0x17e) = ClockMs();
        if (F<int32_t>(l, 0x1e2) > 0) {
            ListBox_Select(l, 0, F<int32_t>(l, 0x1e2) - 1, 1);
            ListView_RefreshVisible(l);
            ListBox_SetSelection(l, 0, F<int32_t>(l, 0x1e2), 1, 0);
        }
        return;
    }
    double bottom = (List_RowHeight(l) + F<float>(l, 0xb2)) * (double)F<int32_t>(l, 0xa2) + F<float>(l, 0xaa) - F<float>(l, 0xb6);
    if (!(bottom < pos[0])) return;
    F<int32_t>(l, 0x17e) = ClockMs();
    if (F<int32_t>(l, 0x1e2) < F<int32_t>(l, 0x1e6)) {
        ListBox_Select(l, 0, F<int32_t>(l, 0x1e2) + 1, 1);
        ListView_RefreshVisible(l);
        ListBox_SetSelection(l, 0, F<int32_t>(l, 0x1d6) + 1, 1, 0);
    }
}
RT3_REPLACE(0x562540, ListBox_AutoScroll);

// 0x562650. List box l: removes all items, with redraws suppressed (+0x182)
// meanwhile, then refreshes the visible rows.
RT3_NOINLINE void __fastcall ListBox_Clear(uint8_t* l) {
    int32_t saved = F<int32_t>(l, 0x182);
    F<int32_t>(l, 0x182) = 1;
    while (F<int32_t>(l, 0x1d2)) ListBox_RemoveItem(l, 0, F<int32_t>(l, 0x1d2) - 1);
    F<int32_t>(l, 0x182) = saved;
    ListView_RefreshVisible(l);
}
RT3_REPLACE(0x562650, ListBox_Clear);

// 0x5626a0 (vtable). List box l events: its popup's pick (0xca from +0x1a6,
// selecting and closing it), callbacks and settings for this id (0xb1 /
// 0x7e / 0x7f / 0x83 / 0x79 / 0x7a / 0xaa / 0xab), close popup (0xb0),
// clear / remove / add rows (0x8b / 0x8a / 0x89 / 0x88), refresh (0x95),
// row data (0xaf), select by data (0x68 / 0x69, re-sent as 0x66), select
// rows (0x66 / 0x67 / 0x6a / 0x6b, 0x8e by the current one), stop dragging
// (0xbe), keys and mouse (2 / 3 / 6 / 7 while enabled), clicks outside the
// list and its popup closing the popup (0xb7) and idle auto-scroll (0).
// The pick advances the tutorial (0x622b34); the rest goes to the default.
int32_t __fastcall ListBox_HandleEvent(uint8_t* l, int, uint32_t* msg) {
    auto slot = [&](uint32_t off) { return ((int32_t(__thiscall*)(uint8_t*))(*(void***)l)[off / 4])(l); };
    const uint32_t id = F<uint16_t>(l, 4);
    switch (msg[0]) {
    case 0xca:
        if (F<uint32_t>(l, 0x126) && F<uint32_t>(l, 0x1a8) && msg[1] == F<uint16_t>(l, 0x1a6)) {
            ++hook::Global<int32_t>(0xccba14);
            ListBox_SetSelection(l, 0, (int32_t)msg[2], 1, 0);
            TreeView_ClosePopup(l);
            --hook::Global<int32_t>(0xccba14);
            msg[1] = F<uint16_t>(l, 4);
        }
        if (hook::Global<int32_t>(0x6d3b4c) && F<uint16_t>(l, 4) == hook::Global<uint32_t>(0x622b34)) Tutorial_Advance();
        break;
    case 0xb1:
        if (msg[1] == id) F<uint32_t>(l, 0x1ba) = msg[2];
        break;
    case 0x7e:
        if (msg[1] == id) F<uint32_t>(l, 0x1ca) = msg[2];
        break;
    case 0x7f:
        if (msg[1] == id) F<uint32_t>(l, 0x1c6) = msg[2];
        break;
    case 0x83:
        if (msg[1] == id) F<uint32_t>(l, 0x1c2) = msg[2];
        break;
    case 0x79:
        if (msg[1] == id) F<uint32_t>(l, 0x1be) = msg[2];
        break;
    case 0x7a:
        if (msg[1] == id) F<uint32_t>(l, 0x1ce) = msg[2];
        break;
    case 0xb0:
        if (msg[1] == id && F<uint32_t>(l, 0x1a8)) TreeView_ClosePopup(l);
        break;
    case 0x8b:
        if (msg[1] == id) ListBox_Clear(l);
        break;
    case 0x8a:
        if (msg[1] == id) ListBox_RemoveItem(l, 0, (int32_t)msg[2]);
        break;
    case 0x89:
        if (msg[1] == id) TreeView_AddRow(l, 0, (const uint32_t*)(uintptr_t)msg[2]);
        break;
    case 0x88:
        if (msg[1] == id) {
            uint32_t row[8] = {};
            row[0] = msg[2], row[1] = msg[4], row[2] = msg[3];
            TreeView_AddRow(l, 0, row);
        }
        break;
    case 0x95:
        if (msg[1] == id) {
            ListView_RefreshVisible(l);
            List_SyncScrollbar(l);
        }
        break;
    case 0xaf:
        if (msg[1] == id && (int32_t)msg[2] < F<int32_t>(l, 0x1d2)) F<uint32_t>(F<uint8_t*>(l, 0x176) + msg[2] * 32, 0xc) = msg[3];
        break;
    case 0x68:
    case 0x69: {
        if (msg[1] != id) break;
        const uint32_t key = msg[2];
        msg[2] = 0xffffffff;
        for (int32_t i = 0; i < F<int32_t>(l, 0x1d2); ++i)
            if (F<uint32_t>(F<uint8_t*>(l, 0x176) + i * 32, 8) == key) msg[2] = (uint32_t)i;
        if (msg[2] == 0xffffffff) return 0;
        msg[0] = 0x66;
        return ((int32_t(__thiscall*)(uint8_t*, uint32_t*))(*(void***)l)[1])(l, msg);
    }
    case 0x6b:
        if (msg[1] != id) break;
        ListBox_Select(l, 0, (int32_t)msg[2], 1);
        return 0;
    case 0x67:
        if ((int32_t)msg[2] >= F<int32_t>(l, 0x1d2)) break;
        // fall through
    case 0x66:
    case 0x6a:
        if (msg[1] != id) break;
        ++hook::Global<int32_t>(0xccba14);
        ListBox_SetSelection(l, 0, (int32_t)msg[2], 0, msg[0] == 0x6a ? 1 : 0);
        --hook::Global<int32_t>(0xccba14);
        return 0;
    case 0x8e: {
        if (msg[1] != id || F<int32_t>(l, 0x1d6) == -1) break;
        const int32_t top = F<int32_t>(l, 0x1e2);
        const int32_t sel = F<int32_t>(l, 0x1d6) < F<int32_t>(l, 0x1e6) ? F<int32_t>(l, 0x1d6) : F<int32_t>(l, 0x1e6);
        ListBox_Select(l, 0, TreeView_NodeOfRow(l, 0, sel), 1);
        if (top != F<int32_t>(l, 0x1e2)) ListView_RefreshVisible(l);
        break;
    }
    case 0xbe:
        F<uint32_t>(l, 0x17a) = 0;
        return 1;
    case 2:
    case 3:
    case 6:
    case 7: {
        if (!slot(0x30)) break;
        const int32_t r = msg[0] == 3 ? ListBox_OnKey(l, 0, msg) : ListBox_OnMouse(l, 0, msg);
        if (r != 1) return r;
        break;
    }
    case 0xb7: {
        if (((int32_t(__thiscall*)(uint8_t*, uint32_t, uint32_t, int32_t))(*(void***)l)[0x20 / 4])(l, msg[9], msg[10], 1)) break;
        if (F<uint32_t>(l, 0x1a8)) {
            uint8_t* p = Widget_FindChild(F<uint8_t*>(l, 0x86), 0, F<uint16_t>(l, 0x1a6));
            if (!p || !((int32_t(__thiscall*)(uint8_t*, uint32_t, uint32_t, int32_t))(*(void***)p)[0x20 / 4])(p, msg[9], msg[10], 2)) TreeView_ClosePopup(l);
        }
        return 1;
    }
    case 0xaa:
        F<uint32_t>(l, 0x1b2) = msg[2];
        break;
    case 0xab:
        F<uint32_t>(l, 0x1b6) = msg[2];
        break;
    case 0:
        if (Widget_IsEnabled(l) && F<uint32_t>(l, 0x17a)) ListBox_AutoScroll(l);
        break;
    }
    return Widget_HandleEvent(l, 0, (uint8_t*)msg);
}
RT3_REPLACE(0x5626a0, ListBox_HandleEvent);

// 0x562d30. Timer widget defaults (type 0x10, 500 ms).
RT3_NOINLINE void __fastcall TimerWidget_Init(uint8_t* w) {
    F<uint16_t>(w, 6) = 0x10;
    F<int32_t>(w, 0xa2) = 0;
    F<int32_t>(w, 0xa6) = 0x1f4;
    for (int k = 0; k < 8; ++k) F<uint16_t>(w, 0xaa + k * 2) = 0;
    F<int32_t>(w, 0xd6) = 0;
    F<int32_t>(w, 0xce) = 0;
    F<int32_t>(w, 0xd2) = 0;
}
RT3_REPLACE(0x562d30, TimerWidget_Init);

// 0x562da0. Debug outlines of a visible widget whose kind bit (+6) is switched on
// in the window manager (+0xc79): its box (+0xb..+0x17) and, when it has one, its
// second box (+0x1b..+0x27), relative to the parent's origin (+0x86 -> +5 / +9).
void __fastcall Widget_DrawDebugFrames(uint8_t* w) {
    if (!Widget_IsVisible(w)) return;
    if (!(F<uint32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc79) & F<uint16_t>(w, 6))) return;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0xb)), (float)((double)F<float>(p, 9) + F<float>(w, 0xf)), F<float>(w, 0x13),
                 F<float>(w, 0x17), 1.0f, 3);
    if (F<float>(w, 0x23) == Fl(0x5c8598)) return;
    p = F<uint8_t*>(w, 0x86);
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0x1b)), (float)((double)F<float>(p, 9) + F<float>(w, 0x1f)), F<float>(w, 0x23),
                 F<float>(w, 0x27), 1.0f, 3);
}
RT3_REPLACE(0x562da0, Widget_DrawDebugFrames);

// 0x562e40. Hover with timing: stamps +0xce / +0xd2 with the clock, calls the move
// callback (as Widget_OnHover) when `moved`, then Widget_OnHover without it.
void __fastcall Widget_OnHoverTimed(uint8_t* w, int, float x, float y, int32_t moved, int32_t track) {
    int32_t now = ClockMs();
    F<int32_t>(w, 0xd2) = now;
    F<int32_t>(w, 0xce) = now;
    if (moved && F<void*>(w, 0x9e)) {
        uint8_t* p = F<uint8_t*>(w, 0x86);
        float rx = (float)((double)x - ((double)F<float>(p, 5) + F<float>(w, 0xb)));
        float ry = (float)((double)y - ((double)F<float>(p, 9) + F<float>(w, 0xf)));
        F<void(__fastcall*)(uint32_t, void*, float, float)>(w, 0x9e)(F<uint16_t>(w, 4), F<void*>(w, 0x9e), rx, ry);
    }
    Widget_OnHover(w, 0, x, y, 0, track);
}
RT3_REPLACE(0x562e40, Widget_OnHoverTimed);

// 0x562eb0 (vtable). Widget w forgets its hover times, then leaves.
void __fastcall Widget_ResetAndLeave(uint8_t* w) {
    F<int32_t>(w, 0xd2) = 0;
    F<int32_t>(w, 0xce) = 0;
    Widget_OnLeave(w);
}
RT3_REPLACE(0x562eb0, Widget_ResetAndLeave);

// 0x562ed0 (vtable). Push button w: keys matching one of its hotkeys
// (+0xb2..+0xb8, without flag 4) while active (slot 0x30) click it (sound
// +0x2d panned, event 0xca with its id); mouse clicks inside (slot 0x20)
// advance the tutorial (0x622b34) and run its action: callback +0xd6, or the
// command of window +0xac, with +0xae / +0xb0. Event 0x7d for it sets the
// callback. Everything else, and the clicks, also go to the widget default.
int32_t __fastcall PushButton_HandleEvent(uint8_t* w, int, uint32_t* msg) {
    auto slot = [&](uint32_t off) { return ((int32_t(__thiscall*)(uint8_t*))(*(void***)w)[off / 4])(w); };
    switch (msg[0]) {
    case 0:
        Widget_IsEnabled(w);
        break;
    case 3: {
        if (!slot(0x30) || (*(const uint8_t*)&msg[8] & 4)) break;
        const int32_t key = (int32_t)msg[2];
        const int16_t k1 = F<int16_t>(w, 0xb2), k2 = F<int16_t>(w, 0xb4), k3 = F<int16_t>(w, 0xb6), k4 = F<int16_t>(w, 0xb8);
        if (!((k1 && k1 == key) || (k2 && k2 == key) || (k3 && k3 == key) || (k4 && k4 == key))) return 1;
        if (w[0x2d]) {
            const float pan = (float)((double(__fastcall*)(uint8_t*))(void*)&Widget_Pan)(w);
            Ui_PlaySound(w[0x2d], 0, Bits(pan));
        }
        msg[0] = 0xca;
        msg[1] = F<uint16_t>(w, 4);
        return 1;
    }
    case 6: {
        if (!slot(0x30)) break;
        if (!((int32_t(__thiscall*)(uint8_t*, uint32_t, uint32_t, int32_t))(*(void***)w)[0x20 / 4])(w, msg[9], msg[10], 1)) break;
        if (hook::Global<int32_t>(0x6d3b4c) && F<uint16_t>(w, 4) == hook::Global<uint32_t>(0x622b34)) Tutorial_Advance();
        if (!F<uint16_t>(w, 0xac) && !F<void*>(w, 0xd6)) break;
        if (auto cb = F<void(__fastcall*)(uint32_t, uint32_t)>(w, 0xd6)) {
            cb(F<uint16_t>(w, 0xae), F<uint16_t>(w, 0xb0));
        } else {
            uint8_t* target = Windows_FindChild(hook::Global<uint8_t*>(0x6d401c), 0, F<uint16_t>(w, 0xac));
            if (!target) break;
            ((void(__thiscall*)(uint8_t*, uint32_t, uint32_t))(*(void***)target)[0x28 / 4])(target, F<uint16_t>(w, 0xae), F<uint16_t>(w, 0xb0));
        }
        break;
    }
    case 0x7d:
        if (msg[1] == F<uint16_t>(w, 4)) F<uint32_t>(w, 0xd6) = msg[2];
        break;
    }
    return Widget_HandleEvent(w, 0, (uint8_t*)msg);
}
RT3_REPLACE(0x562ed0, PushButton_HandleEvent);

// 0x563120 / 0x563210. Timer widget (vtable 0x5de360): fields +0xa2...
uint8_t* __fastcall TimerWidget_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5de360;
    TimerWidget_Init(w);
    uint8_t* rec = Widget_ReadRecord(w, 0x8b);
    memcpy(w + 0xa2, rec + 0x5f, 0x18);
    Free(rec);
    return w;
}
RT3_REPLACE(0x563120, TimerWidget_Load);

// 0x5631e0. Timer widget: nothing of its own.
uint8_t* __fastcall TimerWidget_Delete(uint8_t* w, int, int32_t flags) {
    F<uint32_t>(w, 0) = 0x5de360;
    return Widget_Finish(w, flags);
}
RT3_REPLACE(0x5631e0, TimerWidget_Delete);

RT3_NOINLINE uint8_t* __fastcall TimerWidget_Construct(uint8_t* w, int, const uint8_t* def) {
    Widget_Construct(w, 0, def);
    F<uint32_t>(w, 0) = 0x5de360;
    TimerWidget_Init(w);
    memcpy(w + 0xa2, def + 0x5f, 0x2c);
    F<uint16_t>(w, 6) = 0x10;
    return w;
}
RT3_REPLACE(0x563210, TimerWidget_Construct);

RT3_NOINLINE void __fastcall Reset_625a94() { hook::Global<int32_t>(0x625a94) = -1; }
RT3_REPLACE(0x563250, Reset_625a94);

// 0x563290. Draws a clickable text label in window `win` with font `font`: the
// text box (colour `color`, default 0xffefcf40, or `hover` / 0xffffff80 while it
// is the hovered id 0x625a94), then, unless the window already has child `id`,
// an invisible hotspot (timer widget) over it with tooltip `tip` and the click
// handler 0x563260 (event 0x7b). The hotspot hugs the measured text (aligned like
// the text, height at most `h`) unless flags 0x20 make it cover the whole box.
RT3_NOINLINE void __fastcall TextLink_Draw(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, int32_t align, int32_t valign,
                              uint32_t flags, const char* tip, int32_t id, uint32_t color, uint32_t hover) {
    int32_t fit = (flags & 0x20) ? 0 : 1;
    if (!color) color = 0xffefcf40;
    if (!hover) hover = 0xffffff80;
    if (hook::Global<int32_t>(0x625a94) != id) hover = color;
    Font_DrawTextBox(font, 0, text, x, y, w, h, hover, align, valign, flags, 1.0f);
    if (Widget_FindChild(win, 0, id)) return;
    float boxH = h;
    double bx, by, bw;
    if (fit) {
        float tw;
        float th = (float)Font_TextHeight(font, 0, text, w, &tw);
        double v;
        if (align == 3)
            v = (double)w - tw + x;
        else if (align == 0)
            v = ((double)w - tw) * Fl(0x5c8d48) + x;
        else
            v = x;
        x = (float)(v - D(0x5de3a8));
        bw = (double)tw + D(0x5de3a0);
        double yv;
        if (valign == 4)
            yv = (double)h - th + y;
        else if (valign == 0)
            yv = ((double)h - th) * Fl(0x5c8d48) + y;
        else
            yv = y;
        by = yv - D(0x5caae0);
        double hb = (double)th + D(0x5c9bc0);
        h = (float)hb;
        if (!(hb < boxH)) h = boxH;
    } else {
        bw = w;
        by = y;
    }
    bx = x;
    uint8_t def[0x8b];
    memset(def, 0, sizeof def);
    F<float>(def, 7) = (float)(bx - F<float>(win, 5));
    F<float>(def, 0x13) = h;
    F<uint16_t>(def, 0) = (uint16_t)id;
    F<float>(def, 0xb) = (float)(by - F<float>(win, 9));
    F<float>(def, 0xf) = (float)bw;
    if (strlen(tip)) String_Assign(&F<char*>(def, 0x3f), tip);
    uint8_t* hot = (uint8_t*)ExeNew(0xda);
    if (hot) hot = TimerWidget_Construct(hot, 0, def);
    Window_AddChild(win, 0, hot, 0);
    FreeAndNull(&F<void*>(def, 0x3f));
    uint32_t ev[12] = {};
    ev[0] = 0x7b;
    ev[1] = (uint32_t)id;
    ev[2] = 0x563260;
    Window_HandleEvent(win, 0, ev);
}
RT3_REPLACE(0x563290, TextLink_Draw);

// 0x5634a0. TextLink_Draw with one colour for normal and hovered.
RT3_NOINLINE void __fastcall TextLink_DrawPlain(uint8_t* win, uint8_t* font, const char* text, float x, float y, float w, float h, uint32_t color,
                                   int32_t align, int32_t valign, uint32_t flags, const char* tip, int32_t id) {
    TextLink_Draw(win, font, text, x, y, w, h, align, valign, flags, tip, id, color, color);
}
RT3_REPLACE(0x5634a0, TextLink_DrawPlain);

// 0x5634e0. Button defaults (type 0x20).
RT3_NOINLINE void __fastcall Button_Init(uint8_t* w) {
    static const uint8_t zero[] = { 0xaa, 0xae, 0xa2, 0xa6, 0xb2, 0xb8, 0xbc, 0xc0, 0xc4, 0xc8, 0xd0, 0xd4, 0xd8, 0xcc };
    F<uint16_t>(w, 6) = 0x20;
    for (uint8_t off : zero) F<int32_t>(w, off) = 0;
    F<uint16_t>(w, 0xb6) = 0;
}
RT3_REPLACE(0x5634e0, Button_Init);

// 0x563550. Button setup: caption string, default font, and per-language width
// tweaks for a few known buttons.
void __fastcall Button_Setup(uint8_t* w) {
    if (uint16_t id = F<uint16_t>(w, 0xb6)) String_Assign((char**)(w + 0xb2), GetString(id));
    if (!w[0x2d]) w[0x2d] = (uint8_t)((F<int32_t>(w, 0xc8) == 0 ? 1 : 0) * 2 + 0x24);
    auto widen = [&](double k) {
        F<float>(w, 0x13) = (float)((double)F<float>(w, 0x13) + k);
        F<float>(w, 0xc0) = (float)((double)F<float>(w, 0xc0) + k);
    };
    const double small = *(const double*)0x5c9b50;
    int32_t lang = hook::Global<int32_t>(0x6d3c84);
    if ((lang == 4 || lang == 1 || lang == 2) && F<int16_t>(w, 4) == 0x4f4c) widen(small);
    lang = hook::Global<int32_t>(0x6d3c84);
    if ((lang == 4 || lang == 1 || lang == 2) && F<int16_t>(w, 4) == 0x4f4d) widen(small);
    if (!(uint8_t)IsEnglish() && F<int16_t>(w, 4) == 0x4ecc) widen(*(const double*)0x5d0aa0);
    if (hook::Global<int32_t>(0x6d3c84) == 2 && F<int16_t>(w, 4) == 0x502a) widen(small);
}
RT3_REPLACE(0x563550, Button_Setup);

// 0x563670 (vtable). Draws text widget w when visible: its glyph (+0x92,
// frame +0xaa or the selected +0xae with +0xcc; enlarged 0x5de3f0 while
// hovered with flag 1) at +0xa2 / +0xa6, then its text (+0xb2) at offset
// +0xb8 / +0xbc in colour +0x38 (+0xd0 / +0xd4 / +0xd8 when selected; 20
// brighter per channel while hovered), clipped to +0xc0 / +0xc4, and the
// debug frames.
void __fastcall TextWidget_Draw(uint8_t* w) {
    if (!Widget_IsVisible(w)) return;
    const bool hot = (w[0x33] & 1) && F<uint8_t*>(hook::Global<uint8_t*>(0x6d401c), 0xc6d) == w;
    if (F<uint8_t*>(w, 0x92)) {
        const float scale = (float)(hot ? D(0x5de3f0) : D(0x5c8770));
        const int32_t frame = F<uint32_t>(w, 0xcc) ? F<int32_t>(w, 0xae) : F<int32_t>(w, 0xaa);
        uint8_t* p = F<uint8_t*>(w, 0x86);
        const float gy = (float)((double)F<float>(p, 9) + F<float>(w, 0xa6) + F<float>(w, 0xf));
        const float gx = (float)((double)F<float>(p, 5) + F<float>(w, 0xa2) + F<float>(w, 0xb));
        Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, frame), 0, gx, gy, Bits(scale), 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    }
    const char* text = F<const char*>(w, 0xb2);
    if (text && strlen(text)) {
        float oy = F<float>(w, 0xbc);
        uint32_t color = F<uint32_t>(w, 0x38);
        float ox = F<float>(w, 0xb8);
        if (F<uint32_t>(w, 0xcc)) {
            if (F<float>(w, 0xd0) != Fl(0x5c8598)) ox = F<float>(w, 0xd0);
            if (F<float>(w, 0xd4) != Fl(0x5c8598)) oy = F<float>(w, 0xd4);
            if (F<uint32_t>(w, 0xd8)) color = F<uint32_t>(w, 0xd8);
        }
        if (hot) {
            uint32_t r = ((color >> 16) & 0xff) + 0x14, g = ((color >> 8) & 0xff) + 0x14, b = (color & 0xff) + 0x14;
            if (r > 0xff) r = 0xff;
            if (g > 0xff) g = 0xff;
            if (b > 0xff) b = 0xff;
            color = (color & 0xff000000) | (r << 16) | (g << 8) | b;
        }
        uint8_t* p = F<uint8_t*>(w, 0x86);
        const float hAvail = (float)((double)F<float>(p, 0x11) - F<float>(w, 0xf) - F<float>(w, 0xbc));
        const float hClip = hAvail > F<float>(w, 0xc4) ? F<float>(w, 0xc4) : hAvail;
        const float wAvail = (float)((double)F<float>(p, 0xd) - F<float>(w, 0xb) - F<float>(w, 0xb8));
        const float wClip = wAvail > F<float>(w, 0xc0) ? F<float>(w, 0xc0) : wAvail;
        const float ty = (float)((double)oy + F<float>(p, 9) + F<float>(w, 0xf));
        const float tx = (float)((double)F<float>(p, 5) + F<float>(w, 0xb) + ox);
        Font_DrawTextBox(F<uint8_t*>(w, 0x63), 0, text, tx, ty, wClip, hClip, color, w[0x2b], w[0x2c], 0, 1.0f);
    }
    if (!(F<uint32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc79) & F<uint16_t>(w, 6))) return;
    uint8_t* p = F<uint8_t*>(w, 0x86);
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0xb)), (float)((double)F<float>(p, 9) + F<float>(w, 0xf)), F<float>(w, 0x13),
                 F<float>(w, 0x17), 1.0f, 3);
    if (!(F<float>(w, 0x23) == Fl(0x5c8598))) {
        p = F<uint8_t*>(w, 0x86);
        Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0x1b)), (float)((double)F<float>(p, 9) + F<float>(w, 0x1f)),
                     F<float>(w, 0x23), F<float>(w, 0x27), 1.0f, 3);
    }
}
RT3_REPLACE(0x563670, TextWidget_Draw);

// 0x563950. Toggle button w (id +4, checked +0xcc, radio group +0xc8, label
// +0xb2, click sound +0x2d) event handler:
//   6     click inside (vcalls 0x30 / 0x20): advances a tutorial waiting for
//         it (0x622b34), flips the state (a checked radio button stays),
//         tells the group (0x93 to the owner +0x86), clicks and turns the
//         event into 0xca (w, id, state) - 1;
//   0x6f / 0x76 set the label when it differs (1 for 0x76);
//   0x74  set the state (telling the group when checked);
//   0x93  another button of the group was checked: uncheck;
//   0x94  query the group's checked id.
// Everything else (and unhandled cases) goes to Widget_HandleEvent.
int32_t __fastcall Toggle_HandleEvent(uint8_t* w, int, int32_t* e) {
    uint32_t id = F<uint16_t>(w, 4);
    switch (e[0]) {
    case 6:
        if (!((int32_t(__fastcall*)(uint8_t*))(*(void***)w)[0xc])(w)) break;
        if (!((int32_t(__fastcall*)(uint8_t*, int, int32_t, int32_t, int32_t))(*(void***)w)[8])(w, 0, e[9], e[10], 1)) break;
        if (hook::Global<int32_t>(0x6d3b4c) && (int32_t)F<uint16_t>(w, 4) == hook::Global<int32_t>(0x622b34)) Tutorial_Advance();
        if (F<int32_t>(w, 0xc8) && F<int32_t>(w, 0xcc)) return 0;
        F<int32_t>(w, 0xcc) = F<int32_t>(w, 0xcc) ? 0 : 1;
        if (F<int32_t>(w, 0xcc) && F<int32_t>(w, 0xc8))
            Window_SendEvent5(F<uint8_t*>(w, 0x86), 0, 0x93, F<uint16_t>(w, 4), F<uint32_t>(w, 0xc8), 0, 0);
        if (w[0x2d]) {
            float pan = (float)Widget_Pan_Impl(w);
            Ui_PlaySound(w[0x2d], 0, *(uint32_t*)&pan);
        }
        e[0] = 0xca;
        e[0xb] = (int32_t)(uintptr_t)w;
        e[1] = F<uint16_t>(w, 4);
        e[2] = F<int32_t>(w, 0xcc);
        return 1;
    case 0x6f:
    case 0x76:
        if ((uint32_t)e[1] != id) break;
        if (F<char*>(w, 0xb2) && !strcmp((const char*)(uintptr_t)e[2], F<char*>(w, 0xb2))) break;
        String_Assign((char**)(w + 0xb2), (const char*)(uintptr_t)e[2]);
        return e[0] != 0x6f ? 1 : 0;
    case 0x74: {
        if ((uint32_t)e[1] != id) break;
        int32_t v = e[2];
        if (v == F<int32_t>(w, 0xcc)) return 0;
        int32_t group = F<int32_t>(w, 0xc8);
        F<int32_t>(w, 0xcc) = v;
        if (group) Window_SendEvent5(F<uint8_t*>(w, 0x86), 0, 0x93, id, (uint32_t)group, 0, 0);
        return 0;
    }
    case 0x93:
        if (e[2] != F<int32_t>(w, 0xc8) || (uint32_t)e[1] == id) break;
        F<int32_t>(w, 0xcc) = 0;
        break;
    case 0x94:
        if (e[1] != F<int32_t>(w, 0xc8) || !F<int32_t>(w, 0xcc)) break;
        e[2] = (int32_t)id;
        return 0;
    }
    return Widget_HandleEvent(w, 0, (uint8_t*)e);
}
RT3_REPLACE(0x563950, Toggle_HandleEvent);

// 0x563c00 / 0x563d70. Button (vtable 0x5de3b0): fields +0xa2.., its text +0xb2
// (pack block 0xbc1, else "").
uint8_t* __fastcall Button_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5de3b0;
    Button_Init(w);
    uint8_t* rec = Widget_ReadRecord(w, 0xa1);
    Button_Init(w);
    memcpy(w + 0xa2, rec + 0x5f, 0x10);
    F<uint16_t>(w, 0xb6) = F<uint16_t>(rec, 0x73);
    F<char*>(w, 0xb2) = nullptr;
    NameTable_ReadBlock(hook::Global<uint8_t*>(0x6d4020), 0, 0xbc1, &F<void*>(w, 0xb2));
    if (!F<char*>(w, 0xb2)) String_Assign(&F<char*>(w, 0xb2), (const char*)0x5c87a8);
    memcpy(w + 0xb8, rec + 0x75, 0x24);
    Free(rec);
    return w;
}
RT3_REPLACE(0x563c00, Button_Load);

// 0x563d20. Button: its text +0xb2.
uint8_t* __fastcall Button_Delete(uint8_t* w, int, int32_t flags) {
    F<uint32_t>(w, 0) = 0x5de3b0;
    FreeField(w, 0xb2);
    return Widget_Finish(w, flags);
}
RT3_REPLACE(0x563d20, Button_Delete);

RT3_NOINLINE uint8_t* __fastcall Button_Construct(uint8_t* w, int, const uint8_t* def) {
    Widget_Construct(w, 0, def);
    F<uint32_t>(w, 0) = 0x5de3b0;
    Button_Init(w);
    memcpy(w + 0xa2, def + 0x5f, 0x42);
    F<char*>(w, 0xb2) = nullptr;
    String_Assign(&F<char*>(w, 0xb2), *(const char* const*)(def + 0x6f));
    F<uint16_t>(w, 6) = 0x20;
    return w;
}
RT3_REPLACE(0x563d70, Button_Construct);

// 0x563dd0 (stdcall). Calls o's second virtual function.
int32_t __stdcall Obj_CallSlot1(uint8_t* o) { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[1])(o); }
RT3_REPLACE(0x563dd0, Obj_CallSlot1);

// 0x563de0 (vtable stub).
int32_t __fastcall Return0_563de0(void*) { return 0; }
RT3_REPLACE(0x563de0, Return0_563de0);

void __fastcall Thread_Stop(uint8_t* t, int, uint32_t ms) {
    if (F<void*>(t, 4)) JoinThread(t, ms);
}
RT3_REPLACE(0x563df0, Thread_Stop);

void __fastcall Thread_Start(uint8_t* t) {
    if (F<int32_t>(t, 0xc)) return;
    if (F<void*>(t, 4)) JoinThread(t, 0xffffffff);
    F<void*>(t, 4) = Import<void*(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t, void*)>(0x5c81c0)(
        nullptr, 0, 0x563dd0, t, 0, t + 8);
    F<int32_t>(t, 0xc) = 1;
}
RT3_REPLACE(0x563e30, Thread_Start);

void __fastcall Thread_Destruct(uint8_t* t) {
    F<uint32_t>(t, 0) = 0x5d1248;
    if (F<void*>(t, 4)) JoinThread(t, 0xffffffff);
}
RT3_REPLACE(0x563e90, Thread_Destruct);

// 0x563ed0. Restarts every sound channel of the object (+0x8d list), looping.
void __fastcall Channels_PlayAll(uint8_t* o) {
    PtrList_Rewind(F<PtrList*>(o, 0x8d));
    for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x8d)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x8d)))
        Channel_Command(c, 0, 1, -1);
}
RT3_REPLACE(0x563ed0, Channels_PlayAll);

void __fastcall Widget_RemoveListNode(uint8_t* w, int, PtrListNode* n) { PtrList_RemoveNode(F<PtrList*>(w, 0x8d), 0, n); }
RT3_REPLACE(0x563f10, Widget_RemoveListNode);

// 0x563f20. Rebuilds the audible area (16-cell units) around (x, y) with radius r
// in the terrain's sound grid: clears the audible flags (keeping bit 7), gathers the
// ambient sources of the cells (+0x1671) into +0x79, marks the cells audible, starts
// the positional channels there (+0x1679; new ones join the active list +0x8d) and
// stops the active channels of other sessions (+0xc != +5).
void __fastcall Sound_RefreshArea(uint8_t* o, int, int32_t x, int32_t y, int32_t r, uint8_t* grid) {
    x >>= 4;
    y >>= 4;
    r >>= 4;
    for (int32_t k = 0; k < 0x1000; ++k) grid[0x47c + k] &= 0x80;
    int32_t x0 = x - r <= 0 ? 0 : x - r;
    int32_t y0 = y - r <= 0 ? 0 : y - r;
    int32_t x1 = x + r < Terrain_Rows(grid) >> 4 ? x + r : Terrain_Rows(grid) >> 4;
    int32_t y1 = y + r < Terrain_Cols(grid) >> 4 ? y + r : Terrain_Cols(grid) >> 4;
    PtrList_Clear(F<PtrList*>(o, 0x79));
    for (int32_t cy = y0; cy < y1; ++cy)
        for (int32_t cx = x0; cx < x1; ++cx) {
            PtrList* l = Terrain_Cell1671(grid, 0, cx, cy);
            PtrList_Rewind(l);
            while (void* v = PtrList_Next(l)) PtrList_Append(F<PtrList*>(o, 0x79), 0, v);
            grid[0x47c + cx * 64 + cy] |= 1;
            PtrList* ch = Terrain_Cell1679(grid, 0, cx, cy);
            PtrList_Rewind(ch);
            for (uint8_t* c = (uint8_t*)PtrList_Next(ch); c; c = (uint8_t*)PtrList_Next(ch)) {
                if (!F<void*>(c, 0x13d)) {
                    Channel_Command(c, 0, 1, F<int32_t>(o, 5));
                    F<PtrListNode*>(c, 0x13d) = PtrList_Append(F<PtrList*>(o, 0x8d), 0, c);
                } else Channel_Command(c, 0, 1, F<int32_t>(o, 5));
            }
        }
    PtrList_Rewind(F<PtrList*>(o, 0x8d));
    for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x8d)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x8d))) {
        if (F<int32_t>(c, 0xc) == F<int32_t>(o, 5)) continue;
        Channel_Command(c, 0, 2, 0);
        PtrList_RemoveNode(F<PtrList*>(o, 0x8d), 0, F<PtrListNode*>(c, 0x13d));
        F<PtrListNode*>(c, 0x13d) = nullptr;
    }
}
RT3_REPLACE(0x563f20, Sound_RefreshArea);

}  // namespace rt3
