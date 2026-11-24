// Texture.cpp - RT3_mp.exe 0x540000-0x544000.
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
#include "hook/keepregs.h"
#include "core/containers.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void __fastcall Engine_SampleTextureMem(void* e);
uint32_t __fastcall Texture_Construct(uint8_t* s, int, uint8_t flag);
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
void __fastcall Nop552ff0();
FILE* __fastcall NameTable_OpenFile(uint8_t* t, int, int32_t id, uint8_t text);
void __fastcall Image_To4444(uint8_t* tex, int, uint8_t* img);
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize);
int32_t __fastcall Texture_LoadDds(uint8_t* o, int, int32_t id, uint8_t* img);
int32_t __fastcall Texture_LoadTga(uint8_t* tex, int, int32_t id, uint8_t* img);
void __fastcall Texture_RemoveImage(uint8_t* t, int, int32_t i);
void __fastcall Texture_CountMemory(uint8_t* t);
void __fastcall Texture_MakeBumpMaps(uint8_t* o);
void __fastcall Fatal_Exit(const char* msg);
int32_t __fastcall Texture_CreateTarget(uint8_t* t, int, int32_t w, int32_t h, void* dev);
void* __fastcall Engine_Renderer(void* e);
int32_t __fastcall Texture_Upload(uint8_t* t, int, void* dev, uint8_t force);
int32_t ClockMs();
void __stdcall Textures_MemoryTotals(int32_t* video, int32_t* resident, int32_t* count);
int32_t __fastcall Engine_TotalTextureMem(void* e);
int32_t __fastcall Obj_Sub2543(uint8_t* o);
void __fastcall Texture_DropTarget(uint8_t* t);
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
void __fastcall Texture_FreeImage(uint8_t*, int, uint8_t* img);
void __fastcall Texture_ReleaseTarget(uint8_t* t);
void __fastcall Window_FrameSize(void** o, int, int32_t* w, int32_t* h);
void __fastcall Mouse_Unacquire(uint8_t* m);
void __fastcall Scene_RestoreTarget(uint8_t* s);
void __fastcall Texture_RemoveAllImages(uint8_t* t);
void __fastcall Mouse_Acquire(uint8_t* m);
uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
const char* __fastcall GetString(int32_t id);
void __fastcall String_Assign(char** s, const char* src);

// Point-samples an image plane down by whole steps (w / fx, h / fy) in place.
template <typename P>
static void Shrink(uint8_t* img, uint32_t plane, int32_t fx, int32_t fy) {
    int32_t w = F<int32_t>(img, 8), h = F<int32_t>(img, 0xc);
    int32_t sx, sy;
    double qx = (double)w / fx, qy = (double)h / fy;
    __asm {
        fld qx
        mov eax, 0x5a10d0
        call eax
        mov sx, eax
        fld qy
        mov eax, 0x5a10d0
        call eax
        mov sy, eax
    }
    int32_t nw = w / sx, nh = h / sy;
    int32_t bytes = nw * nh * (int32_t)sizeof(P);
    P* buf = (P*)Malloc(bytes);
    P* dst = buf;
    for (int32_t r = 0, row = 0; r < nh; ++r, row += sy) {
        const P* src = F<const P*>(img, plane) + F<int32_t>(img, 8) * row;
        for (int32_t i = 0; i < nw; ++i, src += sx) *dst++ = *src;
    }
    F<int32_t>(img, 8) = nw;
    F<int32_t>(img, 0xc) = nh;
    F<int32_t>(img, 4) = bytes;
    Free(F<void*>(img, plane));
    F<P*>(img, plane) = buf;
    F<P*>(img, 0x22) = buf;
}

int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
int32_t __fastcall Texture_MaxSize(uint8_t* t);
void __stdcall Texture_DecompressDxt3(uint8_t* rec);
typedef int32_t(__stdcall* CreateBufFn)(void* dev, uint32_t length, uint32_t usage, uint32_t format, uint32_t pool, void** out);
double __fastcall Widget_Pan_Impl(uint8_t* w);
void __fastcall Ui_PlaySound(int32_t id, int, uint32_t pan);
void __fastcall Control_Init(uint8_t* c);
int32_t __fastcall NameTable_ReadBlock(uint8_t* t, int, int32_t, void** out);
void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
int32_t __fastcall Window_SendEvent4(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b);
uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);
void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
void __fastcall Widget_Destruct(uint8_t* w);
int32_t __fastcall Widget_IsVisible(uint8_t* w);

void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign,
                                 uint32_t flags, float scale);

void Engine_Nop();
struct InputEvent;
InputEvent* __fastcall Input_NextEvent(uint8_t* q, int, InputEvent* out);
void __fastcall Input_Block(uint8_t* in);
void __fastcall Input_Unblock(uint8_t* in);
int32_t __fastcall Widget_HandleEvent(uint8_t* w, int, uint8_t* e);

void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t, int32_t, float, float,
                            float, float);

struct InputEvent {
    uint32_t v[12];
};

int32_t __fastcall Button_OnPress(uint8_t* w, int, uint32_t* out);
int32_t __fastcall Button_OnRelease(uint8_t* w, int, uint32_t* out);
void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);
void __fastcall Ui_FrameRect(uint32_t color, int32_t snap, float x, float y, float w, float h, float thick, uint32_t flags);
void Tutorial_Advance();
void __fastcall Nop558750(void*);

// Functions of this file, in exe order.
RT3_NOINLINE void Tutorial_Advance();
RT3_NOINLINE void __fastcall Window_DeleteChildren(uint8_t* win, int, int32_t lo, int32_t hi, uint32_t parent);
RT3_NOINLINE int32_t __fastcall Window_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall Window_DeleteAllChildren(uint8_t* win);
int32_t __fastcall Window_SendEvent4(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b);
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c);
void __fastcall Control_Refresh(uint8_t* c);
void __fastcall Control_Init(uint8_t* c);
uint8_t* __fastcall Control_Load(uint8_t* w, int, int32_t);
uint8_t* __fastcall Control_Delete(uint8_t* w, int, int32_t flags);
RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def);
void __fastcall Thunk5404c0(void* p0);
int32_t __fastcall Button_OnRelease(uint8_t* w, int, uint32_t* out);
void __fastcall Button_Draw(uint8_t* w);
int32_t __fastcall Button_OnPress(uint8_t* w, int, uint32_t* out);
int32_t __fastcall Button_HandleEvent(uint8_t* w, int, uint32_t* ev);
void __fastcall Vec_Cross(float* x, float* y, float ax, float ay, float az, float bx, float by, float bz, float* z);
uint32_t __fastcall Texture_Construct(uint8_t* s, int, uint8_t flag);
uint32_t __fastcall Obj_Next0(uint8_t* o);
void __fastcall Texture_FreeImage(uint8_t*, int, uint8_t* img);
RT3_NOINLINE void __fastcall Texture_CountMemory(uint8_t* t);
RT3_NOINLINE void __fastcall Texture_FreePixels(uint8_t* t);
int32_t __fastcall Texture_MaxSize(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Tga_IsPlain(const char* path);
void __fastcall Image_To4444(uint8_t* tex, int, uint8_t* img);
void __fastcall Texture_Shrink(uint8_t* o, int, int32_t idx, int32_t fx, int32_t fy);
RT3_NOINLINE void __fastcall Texture_ReleaseTarget(uint8_t* t);
int32_t __fastcall Texture_CreateTarget(uint8_t* t, int, int32_t w, int32_t h, void* dev);
void __fastcall Texture_BindTarget(uint8_t* t, int, void* dev);
void __fastcall Texture_ImageSize(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
RT3_NOINLINE uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i);
RT3_NOINLINE int32_t __fastcall Texture_Width(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Texture_Height(uint8_t* t);
RT3_NOINLINE uint8_t* __fastcall Texture_ConstructDefault(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Texture_AddImage(uint8_t* t, int, int32_t w, int32_t h, int32_t format);
void __fastcall Texture_RemoveImage(uint8_t* t, int, int32_t i);
void* __fastcall Texture_TargetForFrame(uint8_t* t);
void __fastcall Texture_DropTarget(uint8_t* t);
RT3_NOINLINE int32_t __fastcall Texture_LoadJpeg(uint8_t* t, int, int32_t id, uint8_t* img);
int32_t __fastcall Texture_LoadTga(uint8_t* tex, int, int32_t id, uint8_t* img);
void __stdcall Texture_DecompressDxt3(uint8_t* rec);
void __fastcall Texture_MakeBumpMaps(uint8_t* o);
void __fastcall Texture_RemoveAllImages(uint8_t* t);
RT3_NOINLINE void __fastcall Texture_SaveJpeg(uint8_t* t, int, void* f, uint32_t withAlpha, int32_t quality);
void __fastcall Texture_SaveTga(uint8_t* t, int, const char* path, int32_t topDown);
RT3_NOINLINE void __fastcall Textures_ReleaseTargets();
RT3_NOINLINE void __fastcall Texture_Destroy(uint8_t* t);
int32_t __fastcall Texture_LoadDds(uint8_t* o, int, int32_t id, uint8_t* img);
RT3_NOINLINE int32_t __fastcall Texture_AppendJpeg(uint8_t* t, int, const char* path, uint32_t withAlpha, int32_t quality);
RT3_NOINLINE int32_t __fastcall Texture_LoadImage(uint8_t* t, int, int32_t id, uint8_t* img);
RT3_NOINLINE int32_t __fastcall Texture_AddAndLoad(uint8_t* t, int, int32_t id, int32_t format);
RT3_NOINLINE int32_t __fastcall Texture_Load(uint8_t* t, int, int32_t limit, uint8_t upload);
RT3_NOINLINE void __fastcall Textures_Manage(int32_t now, int32_t statsOnly);
RT3_NOINLINE int32_t __fastcall Texture_Upload(uint8_t* t, int, void* dev, uint8_t force);
RT3_NOINLINE int32_t __fastcall Texture_SetPixels(uint8_t* t, int, void* pixels);
RT3_NOINLINE uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag);
uint8_t* __fastcall Mouse_Construct(uint8_t* m, int, void* hwnd, void* inst);
void __fastcall Mouse_Acquire(uint8_t* m);
void __fastcall Mouse_Unacquire(uint8_t* m);
void __fastcall Mouse_GetPos3(uint8_t* m, int, uint32_t* x, uint32_t* y, uint32_t* z);
RT3_NOINLINE void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y);
void __fastcall Mouse_Poll(uint8_t* m);
RT3_NOINLINE uint32_t __fastcall Input_Flag400();
RT3_NOINLINE uint32_t __fastcall Input_Flag2000();
RT3_NOINLINE uint32_t __fastcall Input_Flag800();
RT3_NOINLINE void __fastcall Mouse_Destroy(uint8_t* m);
int32_t __fastcall Render_ClearAll(uint8_t* r);
int32_t __fastcall Render_Clear(uint8_t* r, int, uint8_t target, uint8_t coloured, uint32_t z);
int32_t __fastcall Renderer_CreateBuffers(uint8_t* r);
RT3_NOINLINE void __fastcall Input_OnMouse(uint8_t* in, int, float x, float y, float wheel, int32_t left, int32_t middle, int32_t right);
void __fastcall Widget_LoadBase(uint8_t* w, int, const uint8_t* rec);
RT3_NOINLINE void __fastcall Widget_Destruct(uint8_t* w);
RT3_NOINLINE uint8_t* __fastcall Widget_Construct(uint8_t* w, int, const uint8_t* def);
RT3_NOINLINE void __fastcall Nop558750(void*);
RT3_NOINLINE void __fastcall Widget_ClearFlags(uint8_t* w, int, uint32_t mask);
RT3_NOINLINE uint32_t __fastcall Widget_SetFlags(uint8_t* w, int, uint32_t mask, uint32_t a, uint32_t b);

static inline uint8_t* Engine() { return hook::Global<uint8_t*>(0x6d4024); }

static inline void Release(void* com) { ((uint32_t(__stdcall*)(void*))(*(void***)com)[2])(com); }

static inline uint8_t* Input() { return hook::Global<uint8_t*>(0x6d4018); }

static inline void RemoveImage(uint8_t* t, int32_t i) {
    t[4]--;
    Texture_FreeImage(t, 0, F<uint8_t*>(t, 0xa0 + i * 4));
    t[0x98 + i] = 0;
    F<void*>(t, 0xa0 + i * 4) = nullptr;
}

// Device +0x25df (IDirect3DDevice8): SetTransform (vtbl +0x94), Clear (+0x90).
static inline void* Dev(uint8_t* r) { return F<void*>(r, 0x25df); }

static inline int32_t Clear(void* d, uint32_t flags, uint32_t colour, uint32_t z) {
    return ((int32_t(__stdcall*)(void*, uint32_t, void*, uint32_t, uint32_t, uint32_t, uint32_t))(*(void***)d)[0x90 / 4])(
        d, 0, nullptr, flags, colour, z, 0);
}

static inline void** Vt(void* p) { return *(void***)p; }

static inline int32_t Release_139(void* p) { return ((int32_t(__stdcall*)(void*))Vt(p)[2])(p); }

static inline void* Device(uint8_t* r) { return F<void*>(r, 0x25df); }

static inline void VertexBuffer(uint8_t* r, uint32_t len, uint32_t usage, uint32_t fvf, uint32_t at, int32_t err) {
    F<void*>(r, at) = nullptr;
    void* dev = Device(r);
    (*(CreateBufFn*)(*(uint8_t**)dev + 0x5c))(dev, len, usage, fvf, 0, &F<void*>(r, at));
    if (!F<void*>(r, at)) Fatal_Exit(GetString(err));
    F<uint32_t>(r, 0x2543) += len;
}

static inline void IndexBuffer(uint8_t* r, uint32_t len, uint32_t at, int32_t err) {
    void* dev = Device(r);
    (*(CreateBufFn*)(*(uint8_t**)dev + 0x60))(dev, len, 0x208, 0x65, 0, &F<void*>(r, at));
    if (!F<void*>(r, at)) Fatal_Exit(GetString(err));
    F<uint32_t>(r, 0x2543) += len;
}

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline uint32_t FBits(float f) { return *(uint32_t*)&f; }

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

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

static inline uint32_t Bits(float f) { return *(uint32_t*)&f; }

static inline float BitsF(uint32_t b) { return *(float*)&b; }

// 0x5400c0. Deletes all of a window's top-level children (ids 0..0xffff, parent 0).
void __fastcall Window_DeleteAllChildren(uint8_t* win) { Window_DeleteChildren(win, 0, 0, 0xffff, 0); }
RT3_REPLACE(0x5400c0, Window_DeleteAllChildren);

// 0x5400d0. Sends window w an event {type, id, .., +0x18 a, +0x1c b}.
int32_t __fastcall Window_SendEvent4(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b) {
    uint32_t ev[12] = {};
    ev[0] = type;
    ev[1] = id;
    ev[6] = a;
    ev[7] = b;
    return Window_HandleEvent(w, 0, ev);
}
RT3_REPLACE(0x5400d0, Window_SendEvent4);

// 0x540120. Sends window w an event with its first five fields set.
RT3_NOINLINE int32_t __fastcall Window_SendEvent5(uint8_t* w, int, uint32_t type, uint32_t id, uint32_t a, uint32_t b, uint32_t c) {
    uint32_t ev[12] = {};
    ev[0] = type;
    ev[1] = id;
    ev[2] = a;
    ev[3] = b;
    ev[4] = c;
    return Window_HandleEvent(w, 0, ev);
}
RT3_REPLACE(0x540120, Window_SendEvent5);

// 0x540170. Sizes the control from its image (+0x92, entry +0xa2) when unset and
// loads its caption string (+0xd2) into +0xce.
void __fastcall Control_Refresh(uint8_t* c) {
    const float zero = *(const float*)0x5c8598;
    if ((F<float>(c, 0x13) == zero || F<float>(c, 0x17) == zero) && F<uint8_t*>(c, 0x92)) {
        F<float>(c, 0x13) = (float)Slider_StepA((uint8_t*)Table_Entry(F<uint8_t*>(c, 0x92), 0, F<int16_t>(c, 0xa2)));
        F<float>(c, 0x17) = (float)Slider_StepB((uint8_t*)Table_Entry(F<uint8_t*>(c, 0x92), 0, F<int16_t>(c, 0xa2)));
    }
    if (uint16_t id = F<uint16_t>(c, 0xd2)) String_Assign((char**)(c + 0xce), GetString(id));
}
RT3_REPLACE(0x540170, Control_Refresh);

// 0x540200. Control defaults.
RT3_NOINLINE void __fastcall Control_Init(uint8_t* c) {
    F<uint16_t>(c, 6) = 1;
    for (int k = 0; k < 8; ++k) F<uint16_t>(c, 0xa2 + k * 2) = 0;
    F<int32_t>(c, 0xb2) = 0;
    F<int32_t>(c, 0xb6) = 0;
    F<int32_t>(c, 0xba) = 0;
    F<int32_t>(c, 0xbe) = 0;
    c[0xc2] = 0;
    c[0xc3] = 0;
    F<uint32_t>(c, 0xc4) = 0x3b03126f;
    F<uint32_t>(c, 0xc8) = 0x3b03126f;
    c[0xcc] = 0;
    c[0xcd] = 0;
    F<uint16_t>(c, 0xd2) = 0;
    F<int32_t>(c, 0xce) = 0;
    c[0xe8] = 0;
}
RT3_REPLACE(0x540200, Control_Init);

// 0x5402a0 / 0x540430. Generic control (vtable 0x5dd870): fields +0xa2..+0xcd and
// +0xd2, its text +0xce (pack block 0xbba); built from a definition, zero scales
// +0xc4 / +0xc8 become 0.002.
uint8_t* __fastcall Control_Load(uint8_t* w, int, int32_t) {
    Widget_Construct(w, 0, nullptr);
    F<uint32_t>(w, 0) = 0x5dd870;
    Control_Init(w);
    uint8_t* rec = Widget_ReadRecord(w, 0xa5);
    memcpy(w + 0xa2, rec + 0x5f, 0x2c);
    F<uint16_t>(w, 0xd2) = F<uint16_t>(rec, 0x8f);
    F<void*>(w, 0xce) = nullptr;
    NameTable_ReadBlock(hook::Global<uint8_t*>(0x6d4020), 0, 0xbba, &F<void*>(w, 0xce));
    Free(rec);
    return w;
}
RT3_REPLACE(0x5402a0, Control_Load);

// 0x5403e0. Control: its text +0xce.
uint8_t* __fastcall Control_Delete(uint8_t* w, int, int32_t flags) {
    F<uint32_t>(w, 0) = 0x5dd870;
    FreeField(w, 0xce);
    return Widget_Finish(w, flags);
}
RT3_REPLACE(0x5403e0, Control_Delete);

RT3_NOINLINE uint8_t* __fastcall Control_Construct(uint8_t* w, int, const uint8_t* def) {
    Widget_Construct(w, 0, def);
    F<uint32_t>(w, 0) = 0x5dd870;
    Control_Init(w);
    memcpy(w + 0xa2, def + 0x5f, 0x46);
    F<char*>(w, 0xce) = nullptr;
    String_Assign(&F<char*>(w, 0xce), *(const char* const*)(def + 0x8b));
    F<uint16_t>(w, 6) = 1;
    if (F<float>(w, 0xc4) == 0.0f) F<uint32_t>(w, 0xc4) = 0x3b03126f;
    if (F<float>(w, 0xc8) == 0.0f) F<uint32_t>(w, 0xc8) = 0x3b03126f;
    return w;
}
RT3_REPLACE(0x540430, Control_Construct);

// 0x5404c0 / 0x55a080 / 0x55ab90 / 0x5615e0 (vtable stubs).
// Forward to Nop558750.
void __fastcall Thunk5404c0(void* p0) { Nop558750(p0); }
RT3_REPLACE(0x5404c0, Thunk5404c0);

// 0x5404d0. A button's release handler: when pressed (+0x6a & 0x10) it un-presses;
// unless it is a disabled toggle (+0xc2 set, +0xe8 clear) it plays its click
// (sound +0xcd panned by position, unless silent +0x33 bit 10) and drops the 0x20
// state; then fills `out` as a click event (0xcb, its id +4, itself). Returns 1.
RT3_NOINLINE int32_t __fastcall Button_OnRelease(uint8_t* w, int, uint32_t* out) {
    if (!(w[0x6a] & 0x10)) return 1;
    Widget_ClearFlags(w, 0, 0x10);
    if (!w[0xc2] || w[0xe8]) {
        if (!(F<uint32_t>(w, 0x33) & 0x400)) {
            float pan = (float)Widget_Pan_Impl(w);
            Ui_PlaySound(w[0xcd], 0, FBits(pan));
        }
        if (w[0x6a] & 0x20) Widget_ClearFlags(w, 0, 0x20);
    }
    out[0] = 0xcb;
    out[1] = F<uint16_t>(w, 4);
    out[0xb] = (uint32_t)(uintptr_t)w;
    out[2] = F<uint16_t>(w, 4);
    return 1;
}
RT3_REPLACE(0x5404d0, Button_OnRelease);

// 0x540550 (vtable). Draws button w when visible: its frame (+0xa2, the
// pressed one +0xa4 with style 0x20) tinted 1.0 when enabled (2) else 0.65,
// darkened (0x5c87e8) when disabled (4), then its label (+0xce) inside the
// margins (+0xb2 / +0xb6, or the size +0xba / +0xbe), shifted by +0xc8 /
// +0xc4 while pressed; with the debug mask the frames and the label box.
void __fastcall Button_Draw(uint8_t* w) {
    if (!Widget_IsVisible(w)) return;
    const uint16_t style = F<uint16_t>(w, 0x6a);
    const int32_t frame = (style & 0x20) ? F<int16_t>(w, 0xa4) : F<int16_t>(w, 0xa2);
    float tint = (style & 2) ? 1.0f : BitsF(0x3f266666);
    if (style & 4) tint = (float)((double)tint * D(0x5c87e8));
    uint8_t* p = F<uint8_t*>(w, 0x86);
    const float gy = (float)((double)F<float>(p, 9) + F<float>(w, 0xf));
    const float gx = (float)((double)F<float>(p, 5) + F<float>(w, 0xb));
    Glyph_Queue((uint8_t*)(uintptr_t)Table_Entry(F<uint8_t*>(w, 0x92), 0, frame), 0, gx, gy, Bits(tint), 1.0f, 1.0f, 0xffffffff, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    // The label box: size (+0xba / +0xbe, else the box less both margins)
    // and position (margins plus the pressed shift).
    auto box = [&](float* bw, float* bh, float* bx, float* by) {
        *bh = F<float>(w, 0xbe) != Fl(0x5c8598) ? F<float>(w, 0xbe) : (float)((double)F<float>(w, 0x17) - ((double)F<float>(w, 0xb6) + F<float>(w, 0xb6)));
        *bw = F<float>(w, 0xba) != Fl(0x5c8598) ? F<float>(w, 0xba) : (float)((double)F<float>(w, 0x13) - ((double)F<float>(w, 0xb2) + F<float>(w, 0xb2)));
        const bool pressed = (F<uint16_t>(w, 0x6a) & 0x20) != 0;
        const double sy = pressed ? F<float>(w, 0xc4) : Fl(0x5c8598);
        const double sx = pressed ? F<float>(w, 0xc8) : Fl(0x5c8598);
        uint8_t* q = F<uint8_t*>(w, 0x86);
        *by = (float)((double)F<float>(q, 9) + F<float>(w, 0xb6) + F<float>(w, 0xf) + sy);
        *bx = (float)((double)F<float>(q, 5) + F<float>(w, 0xb2) + F<float>(w, 0xb) + sx);
    };
    const char* text = F<const char*>(w, 0xce);
    if (text && strlen(text)) {
        float bw, bh, bx, by;
        box(&bw, &bh, &bx, &by);
        Font_DrawTextBox(F<uint8_t*>(w, 0x63), 0, text, bx, by, bw, bh, F<uint32_t>(w, 0x38), 0, 0, 2, 1.0f);
    }
    if (!(F<uint32_t>(hook::Global<uint8_t*>(0x6d401c), 0xc79) & F<uint16_t>(w, 6))) return;
    p = F<uint8_t*>(w, 0x86);
    Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0xb)), (float)((double)F<float>(p, 9) + F<float>(w, 0xf)), F<float>(w, 0x13),
                 F<float>(w, 0x17), 1.0f, 3);
    if (!(F<float>(w, 0x23) == Fl(0x5c8598))) {
        p = F<uint8_t*>(w, 0x86);
        Ui_FrameRect(0xffffffff, 0, (float)((double)F<float>(p, 5) + F<float>(w, 0x1b)), (float)((double)F<float>(p, 9) + F<float>(w, 0x1f)),
                     F<float>(w, 0x23), F<float>(w, 0x27), 1.0f, 3);
    }
    text = F<const char*>(w, 0xce);
    if (text && strlen(text)) {
        float bw, bh, bx, by;
        box(&bw, &bh, &bx, &by);
        Ui_FrameRect(0xffffffff, 0, bx, by, bw, bh, 1.0f, 3);
    }
}
RT3_REPLACE(0x540550, Button_Draw);

// 0x540890. A button's press handler: remembers whether it was latched (+0xe8 =
// state bit 0x20, never for momentary buttons +0xc3), presses it (0x10), fills
// `out` as a press event (0xca, its id, itself) and, if not yet latched, latches it
// and plays its press sound (+0xcc, unless silent). Returns 1.
RT3_NOINLINE int32_t __fastcall Button_OnPress(uint8_t* w, int, uint32_t* out) {
    w[0xe8] = (uint8_t)((w[0x6a] >> 5) & 1);
    if (w[0xc3]) w[0xe8] = 0;
    Widget_SetFlags(w, 0, 0x10, 0, 0);
    out[0] = 0xca;
    out[1] = F<uint16_t>(w, 4);
    out[0xb] = (uint32_t)(uintptr_t)w;
    if (!(w[0x6a] & 0x20)) {
        Widget_SetFlags(w, 0, 0x20, 0, 0);
        if (!(F<uint32_t>(w, 0x33) & 0x400)) {
            float pan = (float)Widget_Pan_Impl(w);
            Ui_PlaySound(w[0xcc], 0, FBits(pan));
        }
    }
    return 1;
}
RT3_REPLACE(0x540890, Button_OnPress);

// 0x540910. Push button w (id +4, hot keys +0xaa..+0xb0, pressed flag 0x10
// of +0x6a, click callback +0x9a, label +0xce) event handler:
//   3 / 4 key down / up (down only without modifier 4): a hot key presses /
//         releases the button (else 1);
//   6     press inside (vcalls 0x30 / 0x20): with a callback it gets the
//         position relative to the button; otherwise the button is pressed
//         and, unless +0xc2, tracked in a modal loop - pressed while the
//         mouse is over it - until the release (type 7), calling the idle
//         callback (0x6d4004) at least every 20 ms; released over it, a
//         tutorial waiting for it (0x622b34) advances and it is released (1);
//   7     release of a pressed button (tutorial as above);
//   0x6f / 0x76 set the label (1 for 0x76), 0x71 / 0x72 set +0xa2 / +0xa4,
//   0x8d  add a hot key in the first free slot (then the default handling).
// Everything else goes to Widget_HandleEvent.
int32_t __fastcall Button_HandleEvent(uint8_t* w, int, uint32_t* ev) {
    auto visible = [&] { return ((int32_t(__fastcall*)(uint8_t*))(*(void***)w)[0xc])(w); };
    auto hit = [&] { return ((int32_t(__fastcall*)(uint8_t*, int, uint32_t, uint32_t, int32_t))(*(void***)w)[8])(w, 0, ev[9], ev[10], 1); };
    auto hotKey = [&] {
        for (uint32_t off = 0xaa; off <= 0xb0; off += 2) {
            int16_t k = F<int16_t>(w, off);
            if (k && (int32_t)k == (int32_t)ev[2]) return true;
        }
        return false;
    };
    auto tutorial = [&] {
        if (hook::Global<int32_t>(0x6d3b4c) && (int32_t)F<uint16_t>(w, 4) == hook::Global<int32_t>(0x622b34)) Tutorial_Advance();
    };
    switch (ev[0]) {
    case 3:
        if (!visible() || ev[8] & 4) break;
        return hotKey() ? Button_OnPress(w, 0, ev) : 1;
    case 4:
        if (!visible()) break;
        return hotKey() ? Button_OnRelease(w, 0, ev) : 1;
    case 6: {
        if (!visible()) break;
        if (!hit()) return 1;
        if (w[0x6a] & 0x10) return 0;
        if (auto cb = F<void(__fastcall*)(uint32_t, int, float, float)>(w, 0x9a)) {
            uint8_t* p = F<uint8_t*>(w, 0x86);
            float y = (float)(*(float*)&ev[10] - ((double)F<float>(p, 9) + F<float>(w, 0xf)));
            float x = (float)(*(float*)&ev[9] - ((double)F<float>(p, 5) + F<float>(w, 0xb)));
            cb(F<uint16_t>(w, 4), 0, x, y);
            return 0;
        }
        Button_OnPress(w, 0, ev);
        if (w[0xc2]) return 1;
        Input_Block(hook::Global<uint8_t*>(0x6d4018));
        int32_t last = ClockMs();
        while (ev[0] != 7) {
            if (ev[0] == 2) {
                int32_t over = hit();
                uint8_t f = w[0x6a];
                if (over) {
                    if (!(f & 0x10)) Button_OnPress(w, 0, ev);
                } else if (f & 0x10)
                    Button_OnRelease(w, 0, ev);
            }
            Engine_Nop();
            uint32_t next[12];
            memcpy(ev, Input_NextEvent(hook::Global<uint8_t*>(0x6d4018), 0, (InputEvent*)next), 0x30);
            if (ClockMs() - last > 0x14 || ev[0] == 0) {
                if (void* idle = hook::Global<void*>(0x6d4004)) ((void(*)())idle)();
                last = ClockMs();
            }
        }
        Input_Unblock(hook::Global<uint8_t*>(0x6d4018));
        if (!(w[0x6a] & 0x10)) return 0;
        tutorial();
        Button_OnRelease(w, 0, ev);
        return 1;
    }
    case 7:
        if (!visible() || !(w[0x6a] & 0x10)) break;
        tutorial();
        return Button_OnRelease(w, 0, ev);
    case 0x6f:
    case 0x76:
        if (ev[1] != F<uint16_t>(w, 4)) break;
        if (void* old = F<void*>(w, 0xce)) Free(old);
        F<char*>(w, 0xce) = nullptr;
        if (ev[2]) String_Assign((char**)(w + 0xce), (const char*)(uintptr_t)ev[2]);
        return ev[0] != 0x6f ? 1 : 0;
    case 0x71:
        if (ev[1] != F<uint16_t>(w, 4)) break;
        F<uint16_t>(w, 0xa2) = (uint16_t)ev[2];
        return 0;
    case 0x72:
        if (ev[1] != F<uint16_t>(w, 4)) break;
        F<uint16_t>(w, 0xa4) = (uint16_t)ev[2];
        return 0;
    case 0x8d:
        if (ev[1] != F<uint16_t>(w, 4)) break;
        for (uint32_t off = 0xaa; off <= 0xb0; off += 2)
            if (!F<int16_t>(w, off)) {
                F<uint16_t>(w, off) = (uint16_t)ev[2];
                break;
            }
        break;
    }
    return Widget_HandleEvent(w, 0, (uint8_t*)ev);
}
RT3_REPLACE(0x540910, Button_HandleEvent);

// 0x540de0. Cross product a x b into (*x, *y, *z).
void __fastcall Vec_Cross(float* x, float* y, float ax, float ay, float az, float bx, float by, float bz, float* z) {
    *x = (float)((double)ay * bz - (double)az * by);
    *y = (float)((double)az * bx - (double)ax * bz);
    *z = (float)((double)ax * by - (double)ay * bx);
}
RT3_REPLACE(0x540de0, Vec_Cross);

// 0x540e30. Texture constructor: links itself at the head of the global
// list 0xccbb2c (count 0xccbb30). Returns the previous head.
RT3_NOINLINE uint32_t __fastcall Texture_Construct(uint8_t* s, int, uint8_t flag) {
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)((char*)s + 0x13, (const char*)0x5c87a8);
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    s[4] = 0;
    F<uint32_t>(s, 0xd8) = F<uint32_t>(e, 0x114736);
    F<int32_t>(s, 0xcc) = 0;
    F<int32_t>(s, 0xd0) = 0;
    s[6] = 1;
    s[5] = 0x7f;
    F<int32_t>(s, 0xc0) = 0;
    F<int32_t>(s, 0xc4) = 0;
    F<int32_t>(s, 0xc8) = 0;
    s[0x93] = 0;
    s[0x94] = 0;
    s[0x95] = 0;
    s[0x97] = flag;
    F<int32_t>(s, 0xd4) = 0;
    s[0x96] = hook::Global<uint8_t*>(0x6d4024)[0x114254];
    static const uint32_t lists[] = { 0xdc, 0xe4 };
    for (uint32_t off : lists) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(s, off) = l ? PtrList_Construct(l, 0, 1, 100) : nullptr;
        F<int32_t>(s, off + 4) = 0;
    }
    for (int i = 0; i < 8; ++i) {
        s[0x98 + i] = 0;
        F<int32_t>(s, 0xa0 + i * 4) = 0;
    }
    F<int32_t>(s, 7) = 0;
    uint32_t head = hook::Global<uint32_t>(0xccbb2c);
    if (!head) {
        hook::Global<int32_t>(0xccbb30) = 0;
        hook::Global<int32_t>(0xccbb34) = 0;
    }
    hook::Global<int32_t>(0xccbb30)++;
    F<uint32_t>(s, 0) = head;
    hook::Global<uint8_t*>(0xccbb2c) = s;
    return head;
}
RT3_REPLACE(0x540e30, Texture_Construct);

uint32_t __fastcall Obj_Next0(uint8_t* o) { return F<uint32_t>(o, 0); }
RT3_REPLACE(0x540f50, Obj_Next0);

// 0x540f60. Frees an image and its pixel buffers.
RT3_NOINLINE void __fastcall Texture_FreeImage(uint8_t*, int, uint8_t* img) {
    if (F<void*>(img, 0x2a)) Free(F<void*>(img, 0x2a));
    if (F<void*>(img, 0x26)) Free(F<void*>(img, 0x26));
    if (F<void*>(img, 0x2e)) Free(F<void*>(img, 0x2e));
    for (int k = 0; k < 0x32; ++k) img[k] = 0;
    Free(img);
}
RT3_REPLACE(0x540f60, Texture_FreeImage);

// 0x540fc0. Recomputes the memory totals: +0xd0 used slots resident in the
// current device generation (+7), +0xcc images in video memory.
RT3_NOINLINE void __fastcall Texture_CountMemory(uint8_t* t) {
    F<int32_t>(t, 0xcc) = 0;
    F<int32_t>(t, 0xd0) = 0;
    for (int32_t i = 0; i < 8; ++i) {
        uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
        if (!img) continue;
        if (t[0x98 + i] && F<int32_t>(t, 7) == F<int32_t>(Engine(), 0x114222)) F<int32_t>(t, 0xd0) += F<int32_t>(img, 4);
        if (F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 0x22)) F<int32_t>(t, 0xcc) += F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 4);
    }
}
RT3_REPLACE(0x540fc0, Texture_CountMemory);

// 0x5410f0. Drops every image's pixel buffers (device generation reset).
RT3_NOINLINE void __fastcall Texture_FreePixels(uint8_t* t) {
    for (int32_t i = 0; i < 8; ++i) {
        uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
        if (!img) continue;
        if (F<void*>(img, 0x2a)) Free(F<void*>(img, 0x2a));
        if (F<void*>(img, 0x26)) Free(F<void*>(img, 0x26));
        if (F<void*>(img, 0x2e)) Free(F<void*>(img, 0x2e));
        F<int32_t>(img, 0x2e) = 0;
        F<int32_t>(img, 0x26) = 0;
        F<int32_t>(img, 0x2a) = 0;
        F<int32_t>(img, 0x22) = 0;
        F<int32_t>(t, 7) = 0;
    }
}
RT3_REPLACE(0x5410f0, Texture_FreePixels);

// 0x541160. Maximum texture size: 2048 (256 on low-end cards) capped by the
// detail level +0xd8 (0: 256, 1: 512, else 1024); 2048 for non-scaled textures (+5 == 0).
RT3_NOINLINE int32_t __fastcall Texture_MaxSize(uint8_t* t) {
    int32_t v = 0x800;
    if (!t[5]) return v;
    if (!Engine()[0x11422f]) v = 0x100;
    int32_t q = F<int32_t>(t, 0xd8);
    int32_t cap = q == 0 ? 0x100 : q == 1 ? 0x200 : 0x400;
    return v > cap ? cap : v;
}
RT3_REPLACE(0x541160, Texture_MaxSize);

// 0x5411c0. Whether the file is an uncompressed true-colour TGA (type 2, descriptor 0).
RT3_NOINLINE int32_t __fastcall Tga_IsPlain(const char* path) {
    int32_t fd = ((int(__cdecl*)(const char*, int))0x5a4c57)(path, 0x8000);
    if (fd == -1) return 0;
    uint8_t hdr[0x12];
    Read(fd, hdr, 0x12);
    Close(fd);
    return !hdr[0x11] && hdr[2] == 2 ? 1 : 0;
}
RT3_REPLACE(0x5411c0, Tga_IsPlain);

// 0x541210. Converts the image's 32-bit pixels (+0x26, w +8 x h +0xc) to ARGB4444
// (+0x2a, allocated on first use; format fields +4/+0x10/+0x1c/+0x22 set), with the
// colour rounding error carried to the next pixel when the texture asks for
// dithering (+0x96); frees the source unless it is kept (+0x97). Only when 16-bit
// textures are in use and not disabled (engine +0x114255 / +0x11422f).
RT3_NOINLINE void __fastcall Image_To4444(uint8_t* tex, int, uint8_t* img) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (e[0x11422f] || !e[0x114255]) return;
    if (!F<void*>(img, 0x26)) return;
    int32_t n = F<int32_t>(img, 0xc) * F<int32_t>(img, 8);
    if (!F<void*>(img, 0x2a)) F<void*>(img, 0x2a) = ((void*(__cdecl*)(size_t))0x5a125d)(n * 2);
    uint16_t* dst = F<uint16_t*>(img, 0x2a);
    F<int32_t>(img, 4) = n * 2;
    const uint32_t* src = F<const uint32_t*>(img, 0x26);
    F<int32_t>(img, 0x10) = 0x10;
    F<int32_t>(img, 0x1c) = 0x1a;
    F<uint16_t*>(img, 0x22) = dst;
    if (tex[0x96]) {
        int32_t r = 0, g = 0, b = 0;
        for (int32_t i = 0; i < n; ++i) {
            uint32_t px = src[i];
            r += (px >> 16) & 0xff;
            g += (px >> 8) & 0xff;
            b += px & 0xff;
            if (r > 0xff) r = 0xff;
            if (g > 0xff) g = 0xff;
            if (b > 0xff) b = 0xff;
            dst[i] = (uint16_t)(((b >> 4) & 0xf) | ((r & 0xf0) << 4) | (g & 0xf0) | ((px >> 16) & 0xf000));
            r &= 0xf;
            g &= 0xf;
            b &= 0xf;
        }
    } else {
        for (int32_t i = 0; i < n; ++i) {
            uint32_t px = src[i];
            dst[i] = (uint16_t)(((px >> 16) & 0xf000) | ((px >> 12) & 0xf00) | ((px >> 8) & 0xf0) | ((px >> 4) & 0xf));
        }
    }
    if (!tex[0x97]) {
        Free(F<void*>(img, 0x26));
        F<void*>(img, 0x26) = nullptr;
    }
}
RT3_REPLACE(0x541210, Image_To4444);

// 0x5414e0. Shrinks texture slot idx's image (+0xa0[idx]) by the factors: its
// 32-bit plane (+0x26), then its 16-bit plane (+0x2a) from the already shrunk size.
// Not for kind-2 images.
void __fastcall Texture_Shrink(uint8_t* o, int, int32_t idx, int32_t fx, int32_t fy) {
    uint8_t* img = F<uint8_t*>(o, 0xa0 + idx * 4);
    if (F<int32_t>(img, 0x14) == 2) return;
    if (F<void*>(img, 0x26)) Shrink<uint32_t>(img, 0x26, fx, fy);
    img = F<uint8_t*>(o, 0xa0 + idx * 4);
    if (F<void*>(img, 0x2a)) Shrink<uint16_t>(img, 0x2a, fx, fy);
}
RT3_REPLACE(0x5414e0, Texture_Shrink);

// 0x541780. Releases the render target (+0xc0) and depth surface (+0xc8).
RT3_NOINLINE void __fastcall Texture_ReleaseTarget(uint8_t* t) {
    if (void* p = F<void*>(t, 0xc0)) {
        Release(p);
        F<void*>(t, 0xc0) = nullptr;
    }
    if (void* p = F<void*>(t, 0xc8)) {
        Release(p);
        F<void*>(t, 0xc8) = nullptr;
    }
    Engine_SampleTextureMem(Engine());
    int32_t gen = F<int32_t>(t, 7);
    F<int32_t>(t, 0xcc) = 0;
    F<int32_t>(t, 0xd0) = 0;
    F<int32_t>(t, 0xd4) = 0;
    if (gen) hook::Global<int32_t>(0xccbb34)--;
    F<int32_t>(t, 7) = 0;
}
RT3_REPLACE(0x541780, Texture_ReleaseTarget);

// 0x5417e0. Creates a w x h render target texture (+0xc0, surface +0xc4) and its
// depth buffer (+0xc8) on the D3D8 device.
RT3_NOINLINE int32_t __fastcall Texture_CreateTarget(uint8_t* t, int, int32_t w, int32_t h, void* dev) {
    uint8_t* e = Engine();
    int32_t depthFmt = (F<int32_t>(e, 0x114237) ? 3 : 0) + 0x4d;
    int32_t fmt = e[0x11424f] ? 0x17 : F<int32_t>(e, 0x11424b);
    F<int32_t>(t, 0xb) = w;
    F<int32_t>(t, 0xf) = h;
    t[0x93] = 1;
    void** vt = *(void***)dev;
    if (((int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void**))vt[0x50 / 4])(
            dev, w, h, 1, 1, fmt, 0, (void**)(t + 0xc0))) {
        F<void*>(t, 0xc0) = nullptr;
        return 0;
    }
    if (((int32_t(__stdcall*)(void*, int32_t, int32_t, int32_t, int32_t, void**))(*(void***)dev)[0x68 / 4])(
            dev, w, h, depthFmt, 0, (void**)(t + 0xc8))) {
        Release(F<void*>(t, 0xc0));
        F<void*>(t, 0xc0) = nullptr;
        F<void*>(t, 0xc8) = nullptr;
        return 0;
    }
    void* tex = F<void*>(t, 0xc0);
    ((int32_t(__stdcall*)(void*, int32_t, void**))(*(void***)tex)[0x3c / 4])(tex, 0, (void**)(t + 0xc4));
    Release(F<void*>(t, 0xc4));
    F<int32_t>(t, 7) = F<int32_t>(Engine(), 0x114222);
    t[5] = 0;
    Engine_SampleTextureMem(Engine());
    return 1;
}
RT3_REPLACE(0x5417e0, Texture_CreateTarget);

// 0x5418f0. Makes this texture the render target (created on demand).
void __fastcall Texture_BindTarget(uint8_t* t, int, void* dev) {
    if (!F<int32_t>(t, 7)) Texture_CreateTarget(t, 0, F<int32_t>(t, 0xb), F<int32_t>(t, 0xf), dev);
    void* depth = F<void*>(t, 0xc8);
    if (!depth) return;
    void* surf = F<void*>(t, 0xc4);
    if (!surf) return;
    ((int32_t(__stdcall*)(void*, void*, void*))(*(void***)dev)[0x7c / 4])(dev, surf, depth);
}
RT3_REPLACE(0x5418f0, Texture_BindTarget);

void __fastcall Texture_ImageSize(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i) {
    if (w) *w = F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 8);
    if (h) *h = F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 0xc);
}
RT3_REPLACE(0x541930, Texture_ImageSize);

// 0x541970. 32-bit pixels of image i, expanded from ARGB4444 on first use;
// 0 for format-2 images.
RT3_NOINLINE uint32_t* __fastcall Texture_Pixels32(uint8_t* t, int, int32_t* w, int32_t* h, int32_t i) {
    uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
    if (F<int32_t>(img, 0x14) == 2) return nullptr;
    if (w) *w = F<int32_t>(img, 8);
    if (h) *h = F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 0xc);
    img = F<uint8_t*>(t, 0xa0 + i * 4);
    if (!F<void*>(img, 0x26)) {
        uint32_t n = (uint32_t)(F<int32_t>(img, 0xc) * F<int32_t>(img, 8));
        F<void*>(F<uint8_t*>(t, 0xa0 + i * 4), 0x26) = Malloc(n * 4);
        img = F<uint8_t*>(t, 0xa0 + i * 4);
        const uint16_t* src = F<uint16_t*>(img, 0x2a);
        uint32_t* dst = F<uint32_t*>(img, 0x26);
        for (uint32_t k = 0; k < n; ++k) {
            uint32_t v = src[k];
            uint32_t a = (v >> 12) & 0xf, r = (v >> 8) & 0xf, g = (v >> 4) & 0xf, b = v & 0xf;
            dst[k] = (((((a << 4) | a) << 8 | (r << 4) | r) << 8 | (g << 4) | g) << 8) | (b << 4) | b;
        }
    }
    return F<uint32_t*>(F<uint8_t*>(t, 0xa0 + i * 4), 0x26);
}
RT3_REPLACE(0x541970, Texture_Pixels32);

RT3_NOINLINE int32_t __fastcall Texture_Width(uint8_t* t) {
    if (t[4] && t[0x98] && F<uint8_t*>(t, 0xa0)) return F<int32_t>(F<uint8_t*>(t, 0xa0), 8);
    return 1;
}
RT3_REPLACE(0x541aa0, Texture_Width);

RT3_NOINLINE int32_t __fastcall Texture_Height(uint8_t* t) {
    if (t[4] && t[0x98] && F<uint8_t*>(t, 0xa0)) return F<int32_t>(F<uint8_t*>(t, 0xa0), 0xc);
    return 1;
}
RT3_REPLACE(0x541ad0, Texture_Height);

RT3_NOINLINE uint8_t* __fastcall Texture_ConstructDefault(uint8_t* t) {
    Texture_Construct(t, 0, 0);
    return t;
}
RT3_REPLACE(0x541b00, Texture_ConstructDefault);

// 0x541b10. Adds an empty w x h image of `format` in the first free slot; -1 when full.
RT3_NOINLINE int32_t __fastcall Texture_AddImage(uint8_t* t, int, int32_t w, int32_t h, int32_t format) {
    int32_t i = 0;
    while (F<void*>(t, 0xa0 + i * 4)) {
        if (++i >= 8) return -1;
    }
    t[4]++;
    t[0x98 + i] = 1;
    uint8_t* img = (uint8_t*)Malloc(0x32);
    for (int k = 0; k < 0x32; ++k) img[k] = 0;
    F<int32_t>(img, 0xc) = h;
    F<int32_t>(img, 0x14) = format;
    F<int32_t>(img, 0x18) = format;
    F<int32_t>(img, 8) = w;
    F<uint8_t*>(t, 0xa0 + i * 4) = img;
    return i;
}
RT3_REPLACE(0x541b10, Texture_AddImage);

RT3_NOINLINE void __fastcall Texture_RemoveImage(uint8_t* t, int, int32_t i) {
    if (i >= 8 || i < 0 || !F<void*>(t, 0xa0 + i * 4)) return;
    RemoveImage(t, i);
}
RT3_REPLACE(0x541ba0, Texture_RemoveImage);

void* __fastcall Texture_TargetForFrame(uint8_t* t) {
    F<uint32_t>(t, 0xd4) = F<uint32_t>(Engine_Renderer(Engine()), 5);
    return F<void*>(t, 0xc0);
}
RT3_REPLACE(0x541bf0, Texture_TargetForFrame);

RT3_NOINLINE void __fastcall Texture_DropTarget(uint8_t* t) {
    if (!F<int32_t>(t, 7)) return;
    Texture_ReleaseTarget(t);
    F<int32_t>(t, 7) = 0;
}
RT3_REPLACE(0x541c10, Texture_DropTarget);

// 0x541c30. Loads image id (pack 0x6d4020) into img as a JPEG with the alpha
// plane Texture_SaveJpeg appends (end mark 0xbabe, then 0xbaba packed / 0xbabb raw
// with its size; none: opaque) and converts it for texture t (Image_To4444).
// An image already loaded (+0x26, or +0x2a without full colour 0x6d4024
// +0x11422f) is only converted. 1 when it is larger than the device allows
// (0x11423b / 0x11423f, or 256 for small textures, t+5), else 0.
// QUIRK: an unknown alpha mark leaves the alpha pointers unset in the original
// (stack garbage); here they are null.
RT3_NOINLINE int32_t __fastcall Texture_LoadJpeg(uint8_t* t, int, int32_t id, uint8_t* img) {
    typedef size_t(__cdecl * FioFn)(void*, size_t, size_t, void*);
    typedef int(__cdecl * FseekFn)(void*, long, int);
    FioFn fread_ = (FioFn)0x5a1b05;
    FseekFn fseek_ = (FseekFn)0x5a261f;
    uint8_t* dev = hook::Global<uint8_t*>(0x6d4024);
    if (!F<uint8_t>(dev, 0x11422f)) {
        if (F<void*>(img, 0x2a)) return 0;
        if (F<void*>(img, 0x26)) {
            Image_To4444(t, 0, img);
            return 0;
        }
    } else if (F<void*>(img, 0x26)) {
        return 0;
    }
    FILE* f = NameTable_OpenFile(hook::Global<uint8_t*>(0x6d4020), 0, id, 0);
    struct {
        uint8_t* row;
        uint8_t cinfo[0x1a0];
        uint8_t err[0x84];
    } l;
    F<void*>(l.cinfo, 0) = ((void*(__fastcall*)(void*))0x5b3b30)(l.err);
    ((void(__fastcall*)(void*, int32_t, int32_t))0x5b3230)(l.cinfo, 0x3e, 0x19f);
    ((void(__fastcall*)(void*, void*))0x5b3980)(l.cinfo, f);
    ((int(__fastcall*)(void*, uint8_t))0x5b3610)(l.cinfo, 1);
    ((int(__fastcall*)(void*))0x5b37e0)(l.cinfo);
    int32_t w = F<int32_t>(l.cinfo, 0x52), h = F<int32_t>(l.cinfo, 0x56);
    int32_t n = h * w;
    dev = hook::Global<uint8_t*>(0x6d4024);
    if (w > F<int32_t>(dev, 0x11423b) || h > F<int32_t>(dev, 0x11423f)) return 1;
    if (!F<uint8_t>(dev, 0x11422f) && t[5] && (w > 0x100 || h > 0x100)) return 1;
    uint8_t* rgb = (uint8_t*)Malloc((size_t)(n * 3));
    while (F<uint32_t>(l.cinfo, 0x6e) < F<uint32_t>(l.cinfo, 0x56)) {
        l.row = rgb + F<uint32_t>(l.cinfo, 0x6e) * F<uint32_t>(l.cinfo, 0x52) * 3;
        ((uint32_t(__fastcall*)(void*, uint8_t**, uint32_t))0x5b3740)(l.cinfo, &l.row, 1);
    }
    ((int(__fastcall*)(void*))0x5b3550)(l.cinfo);
    ((void(__fastcall*)(void*))0x5b3300)(l.cinfo);
    fseek_(f, 0, 2);
    long size = ((long(__cdecl*)(void*))0x5a254f)(f);
    fseek_(f, size - 2, 0);
    uint32_t mark = 0;
    fread_(&mark, 1, 2, f);
    uint8_t* alpha = nullptr;
    uint8_t* alphaFree = nullptr;
    if ((uint16_t)mark != 0xbabe) {
        alpha = (uint8_t*)Malloc((size_t)(F<int32_t>(l.cinfo, 0x56) * F<int32_t>(l.cinfo, 0x52)));
        memset(alpha, 0xff, (size_t)(h * w));
        alphaFree = alpha;
    } else {
        fseek_(f, size - 4, 0);
        fread_(&mark, 1, 2, f);
        int32_t stored;
        fseek_(f, size - 8, 0);
        fread_(&stored, 1, 4, f);
        int32_t packer[3] = { 0x91, 0, 0 };
        fseek_(f, size - stored - 8, 0);
        if ((uint16_t)mark == 0xbaba) {
            uint8_t* buf = (uint8_t*)Malloc((size_t)stored);
            fread_(buf, 1, (size_t)stored, f);
            int32_t outSize;
            Pack_Decompress(packer, 0, (const int32_t*)buf, stored, &alpha, &outSize);
            Free(buf);
            alphaFree = alpha;
        } else if ((uint16_t)mark == 0xbabb) {
            alpha = (uint8_t*)Malloc((size_t)(F<int32_t>(l.cinfo, 0x56) * F<int32_t>(l.cinfo, 0x52)));
            fread_(alpha, 1, (size_t)stored, f);
            alphaFree = alpha;
        }
        ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
    }
    uint32_t bytes = (uint32_t)(h * w) << 2;
    uint32_t* pix = (uint32_t*)Malloc(bytes);
    const uint8_t* a = alpha;
    const uint8_t* c = rgb;
    uint32_t* p = pix;
    for (int32_t k = n; k != 0; --k) {
        *p++ = (uint32_t)*a++ << 24 | (uint32_t)c[0] << 16 | (uint32_t)c[1] << 8 | c[2];
        c += 3;
    }
    Free(alphaFree);
    Free(rgb);
    F<int32_t>(img, 8) = w;
    F<int32_t>(img, 0xc) = h;
    F<int32_t>(img, 0x10) = 0x20;
    F<uint8_t>(img, 0x21) = 0;
    F<int32_t>(img, 0x14) = 3;
    F<int32_t>(img, 0x18) = 4;
    F<int32_t>(img, 0x1c) = 0x15;
    F<int32_t>(img, 0) = id;
    F<uint32_t*>(img, 0x26) = pix;
    F<uint32_t>(img, 4) = bytes;
    F<uint32_t*>(img, 0x22) = pix;
    F<uint8_t>(img, 0x20) = (h < 0x40 && w < 0x40) ? 0 : 1;
    Image_To4444(t, 0, img);
    return 0;
}
RT3_REPLACE(0x541c30, Texture_LoadJpeg);

// 0x542030. Loads pack entry `id` as an uncompressed true-colour .tga (24 / 32 bit,
// either row order) into the image record as 32-bit pixels (+0x26 / +0x22, 24-bit
// gets full alpha), streaming through a 64K-pixel buffer, then converts it for the
// texture. 0 = done (or nothing to do), 1 = too big for the device or for a small
// texture (+5, 256 max), 2 = not that kind of TGA / no memory.
RT3_NOINLINE int32_t __fastcall Texture_LoadTga(uint8_t* tex, int, int32_t id, uint8_t* img) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (!e[0x11422f]) {
        if (F<void*>(img, 0x2a)) return 0;
        if (F<void*>(img, 0x26)) {
            Image_To4444(tex, 0, img);
            return 0;
        }
    } else if (F<void*>(img, 0x26)) return 0;
    auto fs = []() { return hook::Global<uint8_t*>(0x6d4020); };
    NameTable_Seek(fs(), 0, id, 0);
    NameTable_EntrySize(fs(), 0, id);
    uint8_t hdr[0x12];
    NameTable_Read(fs(), 0, hdr, 0x12);
    uint16_t w = F<uint16_t>(hdr, 0xc), h = F<uint16_t>(hdr, 0xe);
    e = hook::Global<uint8_t*>(0x6d4024);
    if (!hook::Global<int32_t>(0xccbb58) &&
        ((int32_t)w > F<int32_t>(e, 0x11423b) || (int32_t)h > F<int32_t>(e, 0x11423f)))
        return 1;
    if (!e[0x11422f] && tex[5] && (w > 0x100 || h > 0x100)) return 1;
    if (hdr[1] || hdr[2] != 2) return 2;
    if (hdr[0]) {
        uint8_t skip[0x100];
        NameTable_Read(fs(), 0, skip, hdr[0]);
    }
    int32_t W = w, H = h, bpp = hdr[0x10];
    int32_t n = H * W;
    uint32_t* pix = (uint32_t*)Malloc(n * 4);
    uint8_t* buf = (uint8_t*)Malloc(0x40000);
    if (!pix) return 2;
    int32_t inBuf = 0, left = n;
    int32_t bytes = bpp / 8;
    const uint8_t* src = buf;
    for (int32_t row = 0; row < H; ++row) {
        int32_t off = hdr[0x11] & 0x20 ? row * W * 4 : (H * 4 - 4) * W - row * W * 4;
        uint32_t* p = (uint32_t*)((uint8_t*)pix + off);
        uint32_t* end = (uint32_t*)((uint8_t*)p + W * 4);
        if (bpp != 0x20 && bpp != 0x18) {
            Fatal_Exit((const char*)0x5dd8b0);
            continue;
        }
        int32_t rest = bytes * left;
        while (p < end) {
            if (!inBuf) {
                NameTable_Read(fs(), 0, buf, left > 0x10000 ? bytes << 16 : rest);
                src = buf;
                inBuf = 0x10000;
            }
            if (bpp == 0x20) {
                *p = *(const uint32_t*)src;
                src += 4;
            } else {
                *p = (((0xffffff00u | src[2]) << 8 | src[1]) << 8) | src[0];
                src += 3;
            }
            ++p;
            --inBuf;
            --left;
            rest -= bytes;
        }
    }
    Free(buf);
    F<int32_t>(img, 0x10) = bpp;
    F<int32_t>(img, 0x14) = 3;
    F<int32_t>(img, 0x18) = 3;
    F<int32_t>(img, 0) = id;
    F<int32_t>(img, 0xc) = H;
    F<int32_t>(img, 8) = W;
    img[0x21] = 0;
    F<uint32_t>(img, 0x1c) = 0x15;
    F<uint32_t**>(img, 0x26) = (uint32_t**)pix;
    F<int32_t>(img, 4) = n * 4;
    F<uint32_t**>(img, 0x22) = (uint32_t**)pix;
    img[0x20] = (H >= 0x40 || W >= 0x40) ? 1 : 0;
    Image_To4444(tex, 0, img);
    return 0;
}
RT3_REPLACE(0x542030, Texture_LoadTga);

// 0x5423d0. Converts a DXT3 image record (data +0x2e, w +8, h +0xc) to A4R4G4B4
// once (+0x2a): decodes it through two D3DX scratch textures and keeps the 16-bit
// pixels (+0x2a/+0x22, size +4, bpp +0x10 = 16, format +0x1c = 0x1a, +0x14 = 3).
RT3_NOINLINE void __stdcall Texture_DecompressDxt3(uint8_t* rec) {
    if (F<void*>(rec, 0x2a) || !F<void*>(rec, 0x2e)) return;
    int32_t w = F<int32_t>(rec, 8), h = F<int32_t>(rec, 0xc);
    void* dev = F<void*>(hook::Global<uint8_t*>(0x6d4024), 8);
    auto create = (int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, void**))0x570691;
    void* texA = nullptr;
    void* texB = nullptr;
    create(dev, w, h, 1, 0, 0x33545844, 3, &texA);
    create(F<void*>(hook::Global<uint8_t*>(0x6d4024), 8), w, h, 1, 0, 0x1a, 3, &texB);
    void* surfA = nullptr;
    void* surfB = nullptr;
    auto level = [](void* tex, void** out) { ((int32_t(__stdcall*)(void*, uint32_t, void**))Vt(tex)[0x3c / 4])(tex, 0, out); };
    level(texA, &surfA);
    level(texB, &surfB);
    int32_t rect[4] = { 0, 0, w, h };
    ((int32_t(__stdcall*)(void*, void*, void*, const void*, uint32_t, uint32_t, void*, const void*, uint32_t, uint32_t))0x57025d)(
        surfA, nullptr, nullptr, F<void*>(rec, 0x2e), 0x33545844, (uint32_t)((w >> 2) << 4), nullptr, rect, 0xffffffff, 0);
    ((int32_t(__stdcall*)(void*, void*, void*, void*, void*, void*, uint32_t, uint32_t))0x5706f7)(
        surfB, nullptr, nullptr, surfA, nullptr, nullptr, 0xffffffff, 0);
    Release_139(surfA);
    Release_139(texA);
    Free(F<void*>(rec, 0x2e));
    int32_t size = h * w * 2;
    F<void*>(rec, 0x2e) = nullptr;
    void* p = Malloc(size);
    F<void*>(rec, 0x2a) = p;
    F<void*>(rec, 0x22) = p;
    F<int32_t>(rec, 4) = size;
    F<int32_t>(rec, 0x10) = 0x10;
    F<int32_t>(rec, 0x1c) = 0x1a;
    F<int32_t>(rec, 0x14) = 3;
    struct { int32_t pitch; void* bits; } locked;
    ((int32_t(__stdcall*)(void*, void*, const void*, uint32_t))Vt(surfB)[0x24 / 4])(surfB, &locked, nullptr, 0);
    memcpy(F<void*>(rec, 0x2a), locked.bits, F<int32_t>(rec, 4));
    ((int32_t(__stdcall*)(void*))Vt(surfB)[0x28 / 4])(surfB);
    Release_139(surfB);
    Release_139(texB);
}
RT3_REPLACE(0x5423d0, Texture_DecompressDxt3);

// 0x542550. Turns the texture's bump slots (+0x98 flags, count +4) into du/dv maps
// once (+0x95; needs engine bump support +0x11422d): from the height in each pixel's
// low byte, the slopes to the right and down neighbours (wrapping) give a 2D
// direction, normalised with a per-slot scale (1.0, growing by k each slot), stored
// as two signed bytes; marks the image converted (+0x21, format 0x3c).
RT3_NOINLINE void __fastcall Texture_MakeBumpMaps(uint8_t* o) {
    if (!hook::Global<uint8_t*>(0x6d4024)[0x11422d] || o[0x95]) return;
    o[0x95] = 1;
    float scale = 1.0f;
    const double k = *(const float*)0x5ca210, zero = *(const float*)0x5c8598, k3 = *(const float*)0x5ca0ec;
    for (int32_t s = 0; s < o[4]; ++s) {
        uint8_t* img = F<uint8_t*>(o, 0xa0 + s * 4);
        if (!o[0x98 + s] || F<int32_t>(img, 0x14) == 2 || img[0x21]) continue;
        Texture_Pixels32(o, 0, nullptr, nullptr, s);
        img = F<uint8_t*>(o, 0xa0 + s * 4);
        img[0x21] = 1;
        F<int32_t>(img, 0x1c) = 0x3c;
        if (F<void*>(img, 0x2a)) {
            Free(F<void*>(img, 0x2a));
            F<void*>(img, 0x2a) = nullptr;
        }
        uint16_t* out = (uint16_t*)Malloc(F<int32_t>(img, 0x10) == 0x20 ? F<int32_t>(img, 4) >> 1 : F<int32_t>(img, 4));
        const uint32_t* src = F<const uint32_t*>(img, 0x26);
        int32_t w = F<int32_t>(img, 8);
        uint16_t* dstRow = out;
        int32_t curRow = 0;
        for (int32_t row = 1; row - 1 < F<int32_t>(img, 0xc); ++row) {
            int32_t h = F<int32_t>(img, 0xc);
            int32_t next = (row < h ? row : row - h) * w;
            uint16_t* d = dstRow;
            for (int32_t j = 0; j < F<int32_t>(img, 8); ++j) {
                int32_t right = j + 1 < w ? j + 1 : j + 1 - w;
                double c = (double)(int32_t)(src[curRow + j] & 0xff);
                double dv = (double)(int32_t)(src[next + j] & 0xff) - c;
                double du = (double)(int32_t)(src[curRow + right] & 0xff) - c;
                float a = (float)(k * du - dv * zero);
                double b = k * dv - du * zero;
                double inv = 1.0 / (sqrt((double)a * a + b * b + *(const float*)0x5dd8f0) * scale);
                double qu = (double)a * inv * k3, qv = inv * b * k3;
                int32_t u, v;
                __asm {
                    fld qu
                    mov eax, 0x5a10d0
                    call eax
                    mov u, eax
                    fld qv
                    mov eax, 0x5a10d0
                    call eax
                    mov v, eax
                }
                *d++ = (uint16_t)(((uint32_t)(uint8_t)u << 8) | (uint32_t)v);
            }
            dstRow += w;
            curRow += w;
        }
        Free(F<void*>(img, 0x26));
        F<void*>(img, 0x26) = nullptr;
        F<uint16_t*>(img, 0x22) = out;
        F<uint16_t*>(img, 0x2a) = out;
        if (F<int32_t>(img, 0x10) == 0x20) F<int32_t>(img, 4) >>= 1;
        scale = (float)((double)scale * *(const double*)0x5c85a8 + scale);
    }
}
RT3_REPLACE(0x542550, Texture_MakeBumpMaps);

RT3_NOINLINE void __fastcall Texture_RemoveAllImages(uint8_t* t) {
    for (int32_t i = 0; i < 8; ++i)
        if (F<void*>(t, 0xa0 + i * 4)) RemoveImage(t, i);
}
RT3_REPLACE(0x542870, Texture_RemoveAllImages);

// 0x5428c0. Writes the texture's first image to file f as a baseline RGB JPEG at
// quality (bundled libjpeg; ARGB4444 images are expanded first when 0x6d4024
// +0x11424f is set), then with withAlpha its alpha plane: packed + its size +
// 0xbaba, or raw + its size + 0xbabb when packing fails, and the end mark 0xbabe.
// QUIRK: the raw plane is written from the end of the alpha buffer (the pointer
// the pixel loop advanced), so it holds the n bytes of heap that follow it.
RT3_NOINLINE void __fastcall Texture_SaveJpeg(uint8_t* t, int, void* f, uint32_t withAlpha, int32_t quality) {
    typedef size_t(__cdecl * FwriteFn)(const void*, size_t, size_t, void*);
    FwriteFn fwrite_ = (FwriteFn)0x5a276f;
    uint8_t* img = F<uint8_t*>(t, 0xa0);
    int32_t w = F<int32_t>(img, 8), h = F<int32_t>(img, 0xc);
    int32_t n = h * w;
    uint8_t* alpha = (uint8_t*)Malloc((size_t)n);
    uint8_t* rgb = (uint8_t*)Malloc((size_t)(n * 3));
    if (F<uint8_t>(hook::Global<uint8_t*>(0x6d4024), 0x11424f)) Texture_Pixels32(t, 0, nullptr, nullptr, 0);
    const uint32_t* px = F<uint32_t*>(F<uint8_t*>(t, 0xa0), 0x26);
    uint8_t* a = alpha;
    uint8_t* c = rgb;
    for (int32_t k = n; k != 0; --k) {
        uint32_t v = *px++;
        *a++ = (uint8_t)(v >> 24);
        *c++ = (uint8_t)(v >> 16);
        *c++ = (uint8_t)(v >> 8);
        *c++ = (uint8_t)v;
    }
    struct {
        uint8_t* row;
        uint8_t cinfo[0x15c];
        uint8_t err[0x84];
    } l;
    F<void*>(l.cinfo, 0) = ((void*(__fastcall*)(void*))0x5b3b30)(l.err);
    ((void(__fastcall*)(void*, int32_t, int32_t))0x5b3b80)(l.cinfo, 0x3e, 0x15b);
    ((void(__fastcall*)(void*, void*))0x5b48f0)(l.cinfo, f);
    F<int32_t>(l.cinfo, 0x19) = w;
    F<int32_t>(l.cinfo, 0x1d) = h;
    F<int32_t>(l.cinfo, 0x21) = 3;
    F<int32_t>(l.cinfo, 0x25) = 2;
    ((void(__fastcall*)(void*))0x5b4710)(l.cinfo);
    ((void(__fastcall*)(void*, int32_t, int32_t))0x5b4290)(l.cinfo, quality, 1);
    ((void(__fastcall*)(void*, uint8_t))0x5b3db0)(l.cinfo, 1);
    while (F<uint32_t>(l.cinfo, 0xc6) < F<uint32_t>(l.cinfo, 0x1d)) {
        l.row = rgb + F<uint32_t>(l.cinfo, 0xc6) * (uint32_t)w * 3;
        ((uint32_t(__fastcall*)(void*, uint8_t**, uint32_t))0x5b3e20)(l.cinfo, &l.row, 1);
    }
    ((void(__fastcall*)(void*))0x5b3cc0)(l.cinfo);
    ((void(__fastcall*)(void*))0x5b3c40)(l.cinfo);
    if ((uint8_t)withAlpha) {
        uint8_t* out;
        int32_t outSize;
        int32_t packer[3] = { 0x91, 0, 0 };
        uint32_t mark;
        if (Pack_Compress(packer, 0, alpha, n, &out, &outSize) == 1) {
            fwrite_(out, 1, (size_t)outSize, f);
            fwrite_(&outSize, 1, 4, f);
            mark = 0xbaba;
            fwrite_(&mark, 1, 2, f);
            Free(out);
        } else {
            fwrite_(a, 1, (size_t)n, f);
            int32_t size = n;
            fwrite_(&size, 1, 4, f);
            mark = 0xbabb;
            fwrite_(&mark, 1, 2, f);
        }
        mark = 0xbabe;
        fwrite_(&mark, 1, 2, f);
        ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
    }
    Free(alpha);
    Free(rgb);
}
RT3_REPLACE(0x5428c0, Texture_SaveJpeg);

// 0x542b30. Writes image 0 as an uncompressed 32-bit TGA (top-down when asked).
void __fastcall Texture_SaveTga(uint8_t* t, int, const char* path, int32_t topDown) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5ca334);
    if (!f) return;
    auto fwrite = (size_t(__cdecl*)(const void*, size_t, size_t, void*))0x5a276f;
    uint8_t hdr[0x14] = {};
    hdr[2] = 2;
    uint8_t* img = F<uint8_t*>(t, 0xa0);
    *(uint16_t*)(hdr + 0xc) = (uint16_t)F<int32_t>(img, 8);
    *(uint16_t*)(hdr + 0xe) = (uint16_t)F<int32_t>(img, 0xc);
    hdr[0x10] = 0x20;
    hdr[0x11] = topDown ? 0x20 : 0;
    fwrite(hdr, 0x12, 1, f);
    img = F<uint8_t*>(t, 0xa0);
    uint32_t n = (uint32_t)(F<int32_t>(img, 0xc) * F<int32_t>(img, 8)) << 2;
    if (Engine()[0x11424f]) Texture_Pixels32(t, 0, nullptr, nullptr, 0);
    fwrite(F<void*>(F<uint8_t*>(t, 0xa0), 0x26), 1, n, f);
    ((int(__cdecl*)(void*))0x5a1740)(f);
}
RT3_REPLACE(0x542b30, Texture_SaveTga);

// 0x542c10. Releases every texture's render target (device lost).
RT3_NOINLINE void __fastcall Textures_ReleaseTargets() {
    for (uint8_t* t = hook::Global<uint8_t*>(0xccbb2c); t; t = F<uint8_t*>(t, 0)) {
        if (!F<int32_t>(t, 7)) continue;
        if (void* p = F<void*>(t, 0xc0)) {
            Release(p);
            F<void*>(t, 0xc0) = nullptr;
        }
        if (void* p = F<void*>(t, 0xc8)) {
            Release(p);
            F<void*>(t, 0xc8) = nullptr;
        }
        Engine_SampleTextureMem(Engine());
        int32_t gen = F<int32_t>(t, 7);
        F<int32_t>(t, 0xcc) = 0;
        F<int32_t>(t, 0xd0) = 0;
        F<int32_t>(t, 0xd4) = 0;
        if (gen) hook::Global<int32_t>(0xccbb34)--;
        F<int32_t>(t, 7) = 0;
    }
    Engine_SampleTextureMem(Engine());
}
RT3_REPLACE(0x542c10, Textures_ReleaseTargets);

// 0x542c90. Texture teardown: frees its lists, unlinks it from the global texture
// list, restores the back buffer if it was a render target and frees everything.
RT3_NOINLINE void __fastcall Texture_Destroy(uint8_t* t) {
    uint8_t* head = hook::Global<uint8_t*>(0xccbb2c);
    static const uint32_t lists[] = { 0xdc, 0xe4 };
    for (uint32_t off : lists) {
        if (PtrList* l = F<PtrList*>(t, off)) {
            PtrList_Destroy(l);
            ((void(__cdecl*)(void*))0x53b080)(l);
        }
    }
    F<int32_t>(t, 0xe0) = 0;
    if (hook::Global<uint8_t*>(0xccbb2c) == t) {
        hook::Global<uint8_t*>(0xccbb2c) = F<uint8_t*>(t, 0);
    } else {
        for (uint8_t* p = head; p; p = F<uint8_t*>(p, 0))
            if (F<uint8_t*>(p, 0) == t) {
                F<uint8_t*>(p, 0) = F<uint8_t*>(t, 0);
                break;
            }
    }
    if (t[0x93]) Scene_RestoreTarget((uint8_t*)Engine_Renderer(Engine()));
    Texture_ReleaseTarget(t);
    Texture_RemoveAllImages(t);
    F<int32_t>(t, 7) = 0;
    F<int32_t>(t, 0xc0) = 0;
}
RT3_REPLACE(0x542c90, Texture_Destroy);

// 0x542d50. Loads pack entry `id` as a DXT3 .dds into the image record (unless it
// already has pixels): 0 = done (or nothing to do), 1 = too big for the device
// (+0x11423b/+0x11423f) or the texture (Texture_MaxSize), 2 = not a DDS / no memory.
// Unpacks it right away when compressed textures are off (+0x11422f clear).
RT3_NOINLINE int32_t __fastcall Texture_LoadDds(uint8_t* o, int, int32_t id, uint8_t* img) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if ((!e[0x11422f] && F<void*>(img, 0x2a)) || F<void*>(img, 0x2e)) return 0;
    uint8_t* fs = hook::Global<uint8_t*>(0x6d4020);
    NameTable_Seek(fs, 0, id, 0);
    uint32_t size = (uint32_t)NameTable_EntrySize(hook::Global<uint8_t*>(0x6d4020), 0, id);
    uint8_t* buf = (uint8_t*)Malloc(size);
    int32_t hdr = size < 0x80 ? (int32_t)size : 0x80;
    NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, buf, hdr);
    if (F<uint32_t>(buf, 0) != 0x20534444) {
        Free(buf);
        return 2;
    }
    e = hook::Global<uint8_t*>(0x6d4024);
    int32_t w = F<int32_t>(buf, 0x10), h = F<int32_t>(buf, 0xc);
    if ((uint32_t)w > F<uint32_t>(e, 0x11423b) || (uint32_t)h > F<uint32_t>(e, 0x11423f)) {
        Free(buf);
        return 1;
    }
    int32_t m = Texture_MaxSize(o);
    if (w > m || h > m) {
        Free(buf);
        return 1;
    }
    if (hdr < (int32_t)size) NameTable_Read(hook::Global<uint8_t*>(0x6d4020), 0, buf + hdr, size - hdr);
    uint32_t n = (uint32_t)(h / 4 * (w / 4)) << 4; // whole 4x4 blocks only (signed division, as the original)
    uint8_t* data = (uint8_t*)Malloc(n);
    if (!data) {
        Free(buf);
        return 2;
    }
    memcpy(data, buf + 0x80, n);
    Free(buf);
    F<int32_t>(img, 0x14) = 2;
    F<int32_t>(img, 0x18) = 2;
    F<int32_t>(img, 0) = id; // the record keeps its pack id (a reload after a device reset needs it)
    F<int32_t>(img, 0xc) = h;
    F<int32_t>(img, 8) = w;
    F<int32_t>(img, 0x10) = 0;
    img[0x21] = 0;
    F<uint32_t>(img, 0x1c) = 0x33545844;
    F<uint8_t*>(img, 0x2e) = data;
    F<uint32_t>(img, 4) = n;
    F<uint8_t*>(img, 0x22) = data;
    img[0x20] = (h >= 0x80 || w >= 0x80) ? 1 : 0;
    if (!hook::Global<uint8_t*>(0x6d4024)[0x11422f]) Texture_DecompressDxt3(img);
    return 0;
}
RT3_REPLACE(0x542d50, Texture_LoadDds);

// 0x542f50. Appends texture t's first image as a JPEG (Texture_SaveJpeg) to file
// path; returns where it starts, 0 when the file can't be opened.
RT3_NOINLINE int32_t __fastcall Texture_AppendJpeg(uint8_t* t, int, const char* path, uint32_t withAlpha, int32_t quality) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5dd8f4);
    if (!f) return 0;
    ((int(__cdecl*)(void*, long, int))0x5a261f)(f, 0, 2);
    int32_t pos = ((long(__cdecl*)(void*))0x5a254f)(f);
    Texture_SaveJpeg(t, 0, f, withAlpha, quality);
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return pos;
}
RT3_REPLACE(0x542f50, Texture_AppendJpeg);

// 0x542fb0. Loads image id into img by its file kind (+0x18): 2 DDS, 3 TGA,
// 4 JPEG; 2 for anything else.
RT3_NOINLINE int32_t __fastcall Texture_LoadImage(uint8_t* t, int, int32_t id, uint8_t* img) {
    switch (F<int32_t>(img, 0x18)) {
    case 2: return Texture_LoadDds(t, 0, id, img);
    case 3: return Texture_LoadTga(t, 0, id, img);
    case 4: return Texture_LoadJpeg(t, 0, id, img);
    }
    return 2;
}
RT3_REPLACE(0x542fb0, Texture_LoadImage);

// 0x542ff0. Adds an image of format to texture t (+7 cleared) and loads id into
// it by its kind (as Texture_LoadImage); the image is removed again when that
// returns 1 or 2. Recounts the texture memory; 0 when no image could be added.
RT3_NOINLINE int32_t __fastcall Texture_AddAndLoad(uint8_t* t, int, int32_t id, int32_t format) {
    F<uint32_t>(t, 7) = 0;
    int32_t i = Texture_AddImage(t, 0, 0, 0, format);
    if (i == -1) return 0;
    uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
    int32_t r;
    switch (F<int32_t>(img, 0x18)) {
    case 2: r = Texture_LoadDds(t, 0, id, img); break;
    case 3: r = Texture_LoadTga(t, 0, id, img); break;
    case 4: r = Texture_LoadJpeg(t, 0, id, img); break;
    default: r = 2; break;
    }
    if (r == 1 || r == 2) Texture_RemoveImage(t, 0, i);
    Texture_CountMemory(t);
    return r;
}
RT3_REPLACE(0x542ff0, Texture_AddAndLoad);

// 0x543080. Brings texture t's images (+0xa0, 8 slots; marks +0x98) up to
// date unless it is a render target (+0x93), static (+0x94) or current for
// this frame (+7): reloads each from its pack id, dropping the ones that
// fail and, with a size limit, marking larger ones (over 0x84d0) as not
// loaded; +6 = all present loaded. 2 without images (+4); else makes the
// bump maps when wanted (+0x95; not for DDS images) and with `upload`
// uploads it. 0 otherwise.
RT3_NOINLINE int32_t __fastcall Texture_Load(uint8_t* t, int, int32_t limit, uint8_t upload) {
    if (t[0x93] || t[0x94]) return 0;
    if (F<uint32_t>(t, 7) == F<uint32_t>(Engine(), 0x114222)) return 0;
    bool bump = true;
    for (int32_t i = 0; i < 8; ++i) {
        uint8_t* img = F<uint8_t*>(t, 0xa0 + i * 4);
        if (!img || !F<int32_t>(img, 0)) continue;
        if (F<int32_t>(img, 0x18) == 2) bump = false;
        int32_t r = Texture_LoadImage(t, 0, F<int32_t>(img, 0), img);
        if (r == 1 || r == 2) {
            Texture_RemoveImage(t, 0, i);
            continue;
        }
        if (!limit) continue;
        int32_t size = F<int32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 4);
        if (size > limit && size > 0x84d0) {
            t[0x98 + i] = 0;
            t[6] = 0;
        } else
            t[0x98 + i] = 1;
    }
    t[6] = 1;
    for (int32_t k = 0; k < 8; ++k)
        if (!t[0x98 + k] && F<uint8_t*>(t, 0xa0 + k * 4)) t[6] = 0;
    if (!t[4]) return 2;
    if (bump && t[0x95]) {
        t[0x95] = 0;
        Texture_MakeBumpMaps(t);
    }
    if (upload) Texture_Upload(t, 0, F<void*>(Engine(), 8), 0);
    return 0;
}
RT3_REPLACE(0x543080, Texture_Load);

// 0x543220. Texture memory upkeep (all textures, list 0x6ccbb2c). Unless
// `now`, runs at most every 300 ms and only after a number of calls that
// shrinks the more the camera moved (eye and target sum of differences
// against 0xccbb3c..: 0xff calls, down to 0x172). Then: the per-texture
// budget (0xccbb28) from the total texture memory over the upload count
// (0xccbb34) x 0x5c87d8; unless `statsOnly`, frees textures unused for a
// second, and reloads at that budget the small ones (when free memory
// exceeds the total x 0x5cadc0 x 0x5c85a8) or else the big ones over it.
RT3_NOINLINE void __fastcall Textures_Manage(int32_t now, int32_t statsOnly) {
    int32_t calls = ++hook::Global<int32_t>(0xccbb38);
    if (!now) {
        if (ClockMs() - hook::Global<int32_t>(0xccbb54) < 300) return;
        hook::Global<int32_t>(0xccbb54) = ClockMs();
        uint8_t* cam = F<uint8_t*>(Engine(), 0x28);
        if (!cam) return;
        int32_t need = 0xff;
        if (calls >= 0xff) {
            double m = fabs((double)F<float>(cam, 0xc) - hook::Global<float>(0xccbb50));
            m += fabs((double)F<float>(cam, 8) - hook::Global<float>(0xccbb4c));
            m += fabs((double)F<float>(cam, 4) - hook::Global<float>(0xccbb48));
            m += fabs((double)F<float>(cam, 0x24) - hook::Global<float>(0xccbb44));
            m += fabs((double)F<float>(cam, 0x20) - hook::Global<float>(0xccbb40));
            m += fabs((double)F<float>(cam, 0x1c) - hook::Global<float>(0xccbb3c));
            float mf = (float)m;
            if (m > *(const float*)0x5c86a0)
                need = 0x9fb;
            else if (mf > *(const float*)0x5ca06c)
                need = 0x613;
            else if (mf > *(const float*)0x5c9ac4)
                need = 0x41f;
            else if (mf > *(const float*)0x5ca128)
                need = 0x325;
            else if (mf > *(const float*)0x5c8990)
                need = 0x2c1;
            else if (mf > *(const float*)0x5c9aa8)
                need = 0x28f;
            else if (mf > *(const float*)0x5c85f8)
                need = 0x25d;
            else if (mf > *(const double*)0x5c8628)
                need = 0x172;
        }
        hook::Global<uint32_t>(0xccbb3c) = F<uint32_t>(cam, 0x1c);
        hook::Global<uint32_t>(0xccbb40) = F<uint32_t>(cam, 0x20);
        hook::Global<uint32_t>(0xccbb44) = F<uint32_t>(cam, 0x24);
        hook::Global<uint32_t>(0xccbb48) = F<uint32_t>(cam, 4);
        hook::Global<uint32_t>(0xccbb4c) = F<uint32_t>(cam, 8);
        hook::Global<uint32_t>(0xccbb50) = F<uint32_t>(cam, 0xc);
        if (calls < need) return;
    }
    uint8_t* list = hook::Global<uint8_t*>(0xccbb2c);
    hook::Global<int32_t>(0xccbb38) = 0;
    int32_t frame = F<int32_t>((uint8_t*)Engine_Renderer(Engine()), 5);
    int32_t total = Engine_TotalTextureMem(Engine());
    int32_t resident;
    Textures_MemoryTotals(nullptr, &resident, nullptr);
    resident += Obj_Sub2543(Engine());
    int32_t room = (int32_t)((double)total * *(const float*)0x5cadc0);
    int32_t freeMem = total - resident;
    if (hook::Global<int32_t>(0xccbb34))
        hook::Global<int32_t>(0xccbb28) = (int32_t)((double)(total / hook::Global<int32_t>(0xccbb34)) * *(const double*)0x5c87d8);
    if (statsOnly) return;
    for (uint8_t* t = list; t; t = *(uint8_t**)t) {
        if (!t[5] || F<int32_t>(t, 0xd4) >= frame - 1000) continue;
        if (F<uint32_t>(t, 7)) {
            Texture_ReleaseTarget(t);
            F<uint32_t>(t, 7) = 0;
        }
        Texture_FreePixels(t);
    }
    if (!hook::Global<int32_t>(0xccbb34)) return;
    auto cost = [](uint8_t* t) { return (int32_t)(((double)t[5] * *(const double*)0x5dd8f8 + *(const double*)0x5ca250) * F<int32_t>(t, 0xd0)); };
    if ((double)room * *(const double*)0x5c85a8 < (double)freeMem) {
        for (uint8_t* t = hook::Global<uint8_t*>(0xccbb2c); t; t = *(uint8_t**)t) {
            if (!F<uint32_t>(t, 7) || t[6] || !t[5]) continue;
            if (cost(t) >= hook::Global<int32_t>(0xccbb28)) continue;
            Texture_DropTarget(t);
            Texture_Load(t, 0, hook::Global<int32_t>(0xccbb28), 0);
        }
    } else {
        for (uint8_t* t = hook::Global<uint8_t*>(0xccbb2c); t; t = *(uint8_t**)t) {
            if (!F<uint32_t>(t, 7) || !t[5]) continue;
            int32_t c = cost(t);
            if (c <= hook::Global<int32_t>(0xccbb28) || c <= 0x84d0) continue;
            Texture_DropTarget(t);
            Texture_Load(t, 0, hook::Global<int32_t>(0xccbb28), 0);
        }
    }
    Engine_SampleTextureMem(Engine());
}
RT3_REPLACE(0x543220, Textures_Manage);

// 0x5435d0. Puts texture t on device dev: a render target is created when
// missing (2 while it has no surfaces); otherwise (unless it already has a
// D3D texture +0xc0 and not `force`) loads it, creates the texture from the
// first loaded image down (levels of halving size) and copies each image
// into its level, frees the images' pixel copies (+0x22.. when +0x20), marks
// it current (+7) and recounts. 1 without a loaded image, 2 on failure.
RT3_NOINLINE int32_t __fastcall Texture_Upload(uint8_t* t, int, void* dev, uint8_t force) {
    if (t[0x93]) {
        if (!F<uint32_t>(t, 7)) Texture_CreateTarget(t, 0, F<int32_t>(t, 0xb), F<int32_t>(t, 0xf), dev);
        if (!F<void*>(t, 0xc8) || !F<void*>(t, 0xc4)) return 2;
        F<uint32_t>(t, 7) = F<uint32_t>(Engine(), 0x114222);
        return 0;
    }
    if (F<void*>(t, 0xc0) && !force) return 2;
    if (Texture_Load(t, 0, 0, 0)) return 2;
    int32_t n = t[4];
    int32_t first = 0;
    while (first < n && !t[0x98 + first]) ++first;
    if (first >= n) return 1;
    int32_t levels = n - first;
    uint8_t* img = F<uint8_t*>(t, 0xa0 + first * 4);
    uint32_t format = F<uint32_t>(img, 0x1c);
    F<uint32_t>(t, 0xd4) = F<uint32_t>((uint8_t*)Engine_Renderer(Engine()), 5);
    void** tex = &F<void*>(t, 0xc0);
    if (!*tex || !force)
        ((int32_t(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, void**))(*(void***)dev)[0x14])(
            dev, F<uint32_t>(img, 8), F<uint32_t>(img, 0xc), (uint32_t)levels, 0, format, 1, tex);
    if (!*tex) return 2;
    int32_t w = F<int32_t>(img, 8), h = F<int32_t>(img, 0xc);
    for (int32_t i = first, k = 0; i < n; ++i, ++k) {
        uint8_t* e = F<uint8_t*>(t, 0xa0 + i * 4);
        if (F<int32_t>(e, 8) != w || F<int32_t>(e, 0xc) != h) break;
        h >>= 1;
        w >>= 1;
        void* surf;
        if (((int32_t(__stdcall*)(void*, uint32_t, void**))(*(void***)*tex)[0xf])(*tex, (uint32_t)k, &surf)) Fatal_Exit((const char*)0x5dd900);
        struct { int32_t pitch; void* bits; } lr;
        ((int32_t(__stdcall*)(void*, void*, const void*, uint32_t))(*(void***)surf)[9])(surf, &lr, nullptr, 0);
        uint8_t* src = F<uint8_t*>(F<uint8_t*>(t, 0xa0 + i * 4), 0x22);
        memcpy(lr.bits, src, F<uint32_t>(F<uint8_t*>(t, 0xa0 + i * 4), 4));
        ((int32_t(__stdcall*)(void*))(*(void***)surf)[0xa])(surf);
        Release(surf);
    }
    for (int32_t j = 0; j < t[4]; ++j) {
        uint8_t* e = F<uint8_t*>(t, 0xa0 + j * 4);
        if (!e[0x20]) continue;
        if (F<void*>(e, 0x2a)) ((void(__cdecl*)(void*))0x5a1145)(F<void*>(e, 0x2a));
        if (F<void*>(F<uint8_t*>(t, 0xa0 + j * 4), 0x26)) ((void(__cdecl*)(void*))0x5a1145)(F<void*>(F<uint8_t*>(t, 0xa0 + j * 4), 0x26));
        if (F<void*>(F<uint8_t*>(t, 0xa0 + j * 4), 0x2e)) ((void(__cdecl*)(void*))0x5a1145)(F<void*>(F<uint8_t*>(t, 0xa0 + j * 4), 0x2e));
        e = F<uint8_t*>(t, 0xa0 + j * 4);
        F<void*>(e, 0x2e) = nullptr;
        F<void*>(e, 0x26) = nullptr;
        F<void*>(e, 0x2a) = nullptr;
        F<void*>(e, 0x22) = nullptr;
    }
    F<uint32_t>(t, 7) = F<uint32_t>(Engine(), 0x114222);
    ++hook::Global<int32_t>(0xccbb34);
    Engine_SampleTextureMem(Engine());
    Texture_CountMemory(t);
    return 0;
}
RT3_REPLACE(0x5435d0, Texture_Upload);

// 0x5438d0. Gives memory texture t new 32-bit pixels (or re-uploads the
// current ones when null; none: 0): the first image takes them (freeing
// the old ones, format 0x15, size w x h x 4), converts and uploads with
// force, and forgets +0xcc / +0xd0.
RT3_NOINLINE int32_t __fastcall Texture_SetPixels(uint8_t* t, int, void* pixels) {
    uint8_t* img = F<uint8_t*>(t, 0xa0);
    if (pixels) {
        void* old = F<void*>(img, 0x26);
        if (old && old != pixels) ((void(__cdecl*)(void*))0x5a1145)(old);
        F<void*>(F<uint8_t*>(t, 0xa0), 0x26) = pixels;
    } else if (!F<void*>(img, 0x26))
        return 0;
    F<uint8_t*>(t, 0xa0)[0x20] = 0;
    F<uint32_t>(F<uint8_t*>(t, 0xa0), 0x1c) = 0x15;
    img = F<uint8_t*>(t, 0xa0);
    F<void*>(img, 0x22) = F<void*>(img, 0x26);
    img = F<uint8_t*>(t, 0xa0);
    F<int32_t>(img, 4) = F<int32_t>(img, 0xc) * F<int32_t>(img, 8) * 4;
    Image_To4444(t, 0, F<uint8_t*>(t, 0xa0));
    Texture_Upload(t, 0, F<void*>(Engine(), 8), 1);
    F<uint32_t>(t, 0xcc) = 0;
    F<uint32_t>(t, 0xd0) = 0;
    return 0;
}
RT3_REPLACE(0x5438d0, Texture_SetPixels);

// 0x543980. Sets texture t up as a w x h "memory bitmap" (static, pixels
// allocated when null) and returns it.
RT3_NOINLINE uint8_t* __fastcall Texture_InitMemory(uint8_t* t, int, void* pixels, int32_t w, int32_t h, uint8_t flag) {
    if (!pixels) pixels = ((void*(__cdecl*)(uint32_t))0x5a125d)((uint32_t)(w * h * 4));
    Texture_Construct(t, 0, flag);
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)((char*)t + 0x13, (const char*)0x5dd924);
    if (!F<void*>(t, 0xa0)) {
        t[0x94] = 1;
        t[0x96] = 0;
        Texture_AddImage(t, 0, w, h, 3);
        Texture_SetPixels(t, 0, pixels);
    }
    Texture_CountMemory(t);
    t[5] = 0;
    return t;
}
RT3_REPLACE(0x543980, Texture_InitMemory);

// 0x543a10. Creates the DirectInput 8 mouse (+0 interface, +4 device; exclusive
// foreground) with centred position (+9/+0xd = 0.5) and cleared state; fatal
// (strings 0x47..0x4a) on failure.
uint8_t* __fastcall Mouse_Construct(uint8_t* m, int, void* hwnd, void* inst) {
    F<uint32_t>(m, 0xd) = 0x3f000000;
    F<uint32_t>(m, 9) = 0x3f000000;
    F<int32_t>(m, 0x19) = 0;
    F<int32_t>(m, 0x15) = 0;
    F<int32_t>(m, 0x11) = 0;
    F<int32_t>(m, 0x1d) = 0;
    if (((int32_t(__stdcall*)(void*, uint32_t, const void*, void**, void*))0x58bc30)(inst, 0x800, (const void*)0x5dd974,
                                                                                      (void**)m, nullptr) < 0)
        Fatal_Exit(GetString(0x47));
    void* di = F<void*>(m, 0);
    if (((int32_t(__stdcall*)(void*, const void*, void**, void*))Vt(di)[0xc / 4])(di, (const void*)0x5dd934,
                                                                                    (void**)(m + 4), nullptr) < 0)
        Fatal_Exit(GetString(0x48));
    void* dev = F<void*>(m, 4);
    if (((int32_t(__stdcall*)(void*, const void*))Vt(dev)[0x2c / 4])(dev, (const void*)0x5e1b34) < 0)
        Fatal_Exit(GetString(0x49));
    dev = F<void*>(m, 4);
    if (((int32_t(__stdcall*)(void*, void*, uint32_t))Vt(dev)[0x34 / 4])(dev, hwnd, 5) < 0) Fatal_Exit(GetString(0x4a));
    m[8] = 0;
    return m;
}
RT3_REPLACE(0x543a10, Mouse_Construct);

// 0x543ae0. Acquires the mouse device (+4, vtbl +0x1c), remembering the cursor (+9/+0xd).
RT3_NOINLINE void __fastcall Mouse_Acquire(uint8_t* m) {
    if (m[8]) return;
    void* dev = F<void*>(m, 4);
    m[8] = 1;
    ((int32_t(__stdcall*)(void*))(*(void***)dev)[0x1c / 4])(dev);
    if (!Input()) return;
    F<float>(m, 9) = F<float>(Input(), 0xa94);
    if (!Input()) return;
    F<float>(m, 0xd) = F<float>(Input(), 0xa98);
}
RT3_REPLACE(0x543ae0, Mouse_Acquire);

// 0x543b20. Unacquires the mouse and puts the Windows cursor back at the
// remembered position (fractions of the display mode, plus the frame offset).
RT3_NOINLINE void __fastcall Mouse_Unacquire(uint8_t* m) {
    if (!m[8]) return;
    void* dev = F<void*>(m, 4);
    m[8] = 0;
    ((int32_t(__stdcall*)(void*))(*(void***)dev)[0x20 / 4])(dev);
    uint8_t* e = Engine();
    const int32_t* mode = (const int32_t*)(F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15);
    int32_t x = (int32_t)((double)mode[0] * F<float>(m, 9));
    int32_t y = (int32_t)((double)mode[1] * F<float>(m, 0xd));
    int32_t fw, fh;
    Window_FrameSize((void**)e, 0, &fw, &fh);
    Import<int32_t(__stdcall*)(int32_t, int32_t)>(0x5c8264)(fw + x, fh + y);
}
RT3_REPLACE(0x543b20, Mouse_Unacquire);

// 0x543ba0 / 0x543c10. Cursor position (live input values while not acquired).
void __fastcall Mouse_GetPos3(uint8_t* m, int, uint32_t* x, uint32_t* y, uint32_t* z) {
    *x = F<uint32_t>(m, 0x11);
    *y = F<uint32_t>(m, 0x15);
    if (z) *z = F<uint32_t>(m, 0x19);
    if (m[8] || !Input()) return;
    *x = F<uint32_t>(Input(), 0xa94);
    *y = F<uint32_t>(Input(), 0xa98);
    if (z) *z = F<uint32_t>(Input(), 0xa9c);
}
RT3_REPLACE(0x543ba0, Mouse_GetPos3);

RT3_NOINLINE void __fastcall Mouse_GetPos(uint8_t* m, int, uint32_t* x, uint32_t* y) {
    *x = F<uint32_t>(m, 9);
    *y = F<uint32_t>(m, 0xd);
    if (m[8] || !Input()) return;
    *x = F<uint32_t>(Input(), 0xa94);
    *y = F<uint32_t>(Input(), 0xa98);
}
RT3_REPLACE(0x543c10, Mouse_GetPos);

// 0x543c50. Polls DirectInput mouse m (+4 device, +8 acquired; lost: re-
// acquire): moves the cursor (+9 / +0xd, clamped to 0..1; +0x11 / +0x15
// unclamped) and wheel (+0x19) by the deltas x 0x5dd984 (none in the frame
// the right button or a big wheel turn starts or ends), keeps the buttons
// (+0x1d: 1 left, 2 right, 4 middle) and feeds Input_OnMouse.
void __fastcall Mouse_Poll(uint8_t* m) {
    if (!m[8]) return;
    struct {
        int32_t x, y, z;
        uint8_t b[4];
        int32_t pad;
    } st = {};
    void* dev = F<void*>(m, 4);
    if (((int32_t(__stdcall*)(void*, uint32_t, void*))(*(void***)dev)[9])(dev, 0x14, &st)) {
        m[8] = 0;
        Mouse_Acquire(m);
        return;
    }
    const float k = *(const float*)0x5dd984;
    float dx = (float)((double)st.x * k);
    float dy = (float)((double)st.y * k);
    float dz = (float)((double)st.z * k);
    int32_t right = st.b[2] != 0;
    int32_t big = fabs((double)dz) > *(const double*)0x5c8988 ? 1 : 0;
    if (right != hook::Global<int32_t>(0xccbb5c) || big != hook::Global<int32_t>(0xccbb60)) {
        dy = 0.0f;
        dx = 0.0f;
    }
    hook::Global<int32_t>(0xccbb5c) = right;
    hook::Global<int32_t>(0xccbb60) = big;
    double sx = (double)dx + F<float>(m, 9);
    F<float>(m, 9) = (float)sx;
    float sy = (float)((double)dy + F<float>(m, 0xd));
    F<float>(m, 0xd) = sy;
    F<float>(m, 0x11) = (float)((double)dx + F<float>(m, 0x11));
    F<float>(m, 0x15) = (float)((double)dy + F<float>(m, 0x15));
    F<float>(m, 0x19) = (float)((double)dz + F<float>(m, 0x19));
    if (sx < *(const float*)0x5c8598)
        F<float>(m, 9) = 0.0f;
    else if (sx > *(const float*)0x5c85b4)
        F<float>(m, 9) = 1.0f;
    if (sy < *(const float*)0x5c8598)
        F<float>(m, 0xd) = 0.0f;
    else if (sy > *(const float*)0x5c85b4)
        F<float>(m, 0xd) = 1.0f;
    uint32_t f = 0;
    if (st.b[0]) f = 1;
    if (st.b[1]) f |= 4;
    if (st.b[2]) f |= 2;
    F<uint32_t>(m, 0x1d) = f;
    if (uint8_t* in = hook::Global<uint8_t*>(0x6d4018))
        Input_OnMouse(in, 0, F<float>(m, 9), F<float>(m, 0xd), dz, (int32_t)(f & 1), (int32_t)(f & 2), (int32_t)(f & 4));
}
RT3_REPLACE(0x543c50, Mouse_Poll);

RT3_NOINLINE uint32_t __fastcall Input_Flag400() { return Input() ? F<uint32_t>(Input(), 0xa8c) & 0x400 : 0; }
RT3_REPLACE(0x543e00, Input_Flag400);

RT3_NOINLINE uint32_t __fastcall Input_Flag2000() { return Input() ? F<uint32_t>(Input(), 0xa8c) & 0x2000 : 0; }
RT3_REPLACE(0x543e20, Input_Flag2000);

RT3_NOINLINE uint32_t __fastcall Input_Flag800() {
    uint8_t* in = hook::Global<uint8_t*>(0x6d4018);
    return in ? F<uint32_t>(in, 0xa8c) & 0x800 : 0;
}
RT3_REPLACE(0x543e40, Input_Flag800);

// 0x543e60. Releases the mouse device (+4) and DirectInput (+0).
RT3_NOINLINE void __fastcall Mouse_Destroy(uint8_t* m) {
    Mouse_Unacquire(m);
    Release(F<void*>(m, 4));
    Release(F<void*>(m, 0));
}
RT3_REPLACE(0x543e60, Mouse_Destroy);

int32_t __fastcall Render_ClearAll(uint8_t* r) { return Clear(Dev(r), 3, 0, 0x3f800000); }
RT3_REPLACE(0x543e80, Render_ClearAll);

// 0x543ea0. Clears target + z (with the background colour +0x25a3 when asked) or only z.
int32_t __fastcall Render_Clear(uint8_t* r, int, uint8_t target, uint8_t coloured, uint32_t z) {
    if (!target) return Clear(Dev(r), 2, 0, 0x3f800000);
    uint32_t c = coloured ? F<uint32_t>(r, 0x25a3) & 0xffffff : 0;
    return Clear(Dev(r), 3, c, z);
}
RT3_REPLACE(0x543ea0, Render_Clear);

// 0x543f10. Creates the renderer's dynamic vertex buffers (terrain, models, two
// batch buffers, sprites, water / shadow by device caps, lines, overlay) and index
// buffers (one shared, 64 per-batch, one more), counting their bytes in +0x2543;
// any failure is fatal (strings 0x68..0x71). Then selects FVF 0x152.
int32_t __fastcall Renderer_CreateBuffers(uint8_t* r) {
    F<uint32_t>(r, 0x2543) = 0;
    VertexBuffer(r, 0x17cdc0, 0x208, 0x142, 0x2647, 0x68);
    IndexBuffer(r, 0x3e80, 0x264b, 0x69);
    VertexBuffer(r, 0x1fffe0, 0x208, 0x112, 0x2643, 0x6a);
    VertexBuffer(r, 0x1fffe0, 0x208, 0x112, 0x265b, 0x6b);
    VertexBuffer(r, 0x17ffe8, 0x208, 0x142, 0x2663, 0x6c);
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (!e[0x11422e]) {
        F<void*>(r, 0x265f) = nullptr;
        void* dev = Device(r);
        if (e[0x114250]) {
            (*(CreateBufFn*)(*(uint8_t**)dev + 0x5c))(dev, 0x27ffd8, 0x10, 0x2c4, 0, &F<void*>(r, 0x265f));
            F<uint32_t>(r, 0x2543) += 0x27ffd8;
        } else {
            (*(CreateBufFn*)(*(uint8_t**)dev + 0x5c))(dev, 0x1fffe0, 0x10, 0x1c4, 0, &F<void*>(r, 0x265f));
            F<uint32_t>(r, 0x2543) += 0x1fffe0;
        }
        if (!F<void*>(r, 0x265f)) Fatal_Exit(GetString(0x6d));
    }
    VertexBuffer(r, 0x2ee00, 8, 0x112, 0x263f, 0x6e);
    for (int32_t k = 0; k < 0x40; ++k) IndexBuffer(r, 0xa410, 0x31ab + k * 4, 0x6f);
    IndexBuffer(r, 0x3e80, 0x2653, 0x70);
    VertexBuffer(r, 0x7800, 0x208, 0x142, 0x264f, 0x71);
    void* dev = Device(r);
    (*(int32_t(__stdcall**)(void*, uint32_t))(*(uint8_t**)dev + 0x130))(dev, 0x152);
    return 0;
}
RT3_REPLACE(0x543f10, Renderer_CreateBuffers);

}  // namespace rt3
