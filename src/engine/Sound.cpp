// Sound.cpp - RT3_mp.exe 0x530000-0x532000.
// Regrouped by tools/regroup.py from the per-batch files; one band of
// the exe per original object file.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <intrin.h>
#include <windows.h>
#include <initializer_list>
#include "hook/hook.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/stream.h"
#include "hook/keepregs.h"
#include "core/containers.h"
#include "core/crt.h"
#include "game/calendar.h"

#undef near
#undef far
#undef small

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
void* __fastcall Engine_Renderer(void* e);
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }
int32_t __fastcall SceneObj_Reset_Impl(uint8_t* o);

// 0x531e90. Stores a 4x4 matrix (+8) for a 3D sound and marks the listener dirty.
struct Mat16 { uint32_t v[16]; };

int32_t __fastcall Music_IsPlaying(uint8_t* m);
int32_t __fastcall Music_IsPaused(uint8_t* m);
void __fastcall Music_Stop(uint8_t* m, int, uint32_t notify);
void __fastcall Music_SetVolume(uint8_t* m, int, float vol);
int32_t __fastcall Music_Play(uint8_t* m, int, const char* file, void* digital, void* onEnd);
uint32_t __fastcall Sound_ProviderInfo(uint8_t* s);
uint32_t __fastcall Sound_Get3DProvider(int32_t n);
void __fastcall Sound_SetListenerPos(uint8_t* s, int, float x, float y, float z);
void __fastcall Sound_SetListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz);
void __fastcall Sound_SetSpeakerType(uint8_t* s, int, int32_t type, int32_t noApply);
int32_t __fastcall Sound_SpeakerType();
uint32_t __fastcall Sound_ProviderFlags(uint8_t* s, int, const char* name);
void __fastcall Sound2D_SetVolume(uint8_t* s, int, float vol);
void __fastcall Sound_SetEffectsVolume(uint8_t* s, int, float vol);
void __fastcall Sound2D_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol);
void __fastcall Sound_ReleaseTimer(uint8_t* s);
void __fastcall Sound2D_PauseAll(uint8_t* s);
void __fastcall Sound_PauseAll(uint8_t* s);
void __fastcall Sound2D_Stop(uint8_t* s, int, uint32_t key);
void __fastcall Sound3D_Stop(uint8_t* s, int, uint32_t key, uint8_t* smp);
int32_t __fastcall Module_RegisterCleanup();
uint8_t* __fastcall Mixer_Construct(uint8_t* m);
int32_t __fastcall Sound2D_Open(uint8_t* s);
uint8_t* __fastcall Sound2D_Construct(uint8_t* s);
int32_t __fastcall Sound3D_Open(uint8_t* s, int, const char* name);
void __fastcall Sound3D_Destruct(uint8_t* s);
void __fastcall Sound2D_Destruct(uint8_t* s);
void __fastcall Sound2D_StopAll(uint8_t* s);
void __fastcall Sound_StopAll(uint8_t* s);
void __fastcall Sound3D_Close(uint8_t* s);
void __fastcall Sound2D_Close(uint8_t* s);
void __fastcall Music_Close(uint8_t* m);

uint32_t __fastcall Sound2D_Play(uint8_t* s, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data);

uint32_t __fastcall Sound3D_Play(uint8_t* s, int, const char* name, float x, float y, float z, float vol, int32_t mode,
                                 int32_t paused, int32_t prio, float maxDist);

void __fastcall Sound_Shutdown(uint8_t* snd, int, int32_t immediate);
int32_t __fastcall Sound_Startup(uint8_t* snd, int, const char* name);

int32_t __stdcall Ray_HitsTriangle_Impl(float a0, float a1, float a2, float b0, float b1, float b2, float c0, float c1,
                                        float c2, float o0, float o1, float o2, float d0, float d1, float d2);

// The mesh walk state the original keeps in globals (current LOD, sub-mesh,
// triangle pointer and triangles left).
static uint8_t*& g_lod = hook::Global<uint8_t*>(0xcc3b6c);

static uint8_t*& g_sub = hook::Global<uint8_t*>(0xcc3b58);
static uint8_t*& g_tri = hook::Global<uint8_t*>(0xcc3b64);
static int32_t& g_left = hook::Global<int32_t>(0xcc3b5c);
struct PtrListNode;
void __fastcall Terrain_Remove1671(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
PtrListNode* __fastcall Terrain_Add1671(uint8_t* r, int, void* v, int32_t x, int32_t y);
void __fastcall Obj_MoveChannels(uint8_t* o);
void* __fastcall Engine_Get1C(void* e);
void __fastcall SceneObj_UpdateMatrix(uint8_t* o);
void __fastcall PtrList_Rewind(PtrList* l);
void* __fastcall PtrList_Next(PtrList* l);
const char* __fastcall GetString(int32_t id);

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall SceneObj_RecentlyDrawn(uint8_t* o);
double __fastcall Slider_StepA(uint8_t* s);
double __fastcall Slider_StepB(uint8_t* s);
void __fastcall Fog_ParamsAt(uint8_t*, int, int32_t x, int32_t z, float bias, float* scale, uint32_t* color, float* strength);
int32_t __fastcall Scene_ProjectNormalised(uint8_t* s, int, float x, float y, float z, float* ox, float* oy, float* oz);
void __fastcall Fade_Save();
void __fastcall Fade_Restore();
void __stdcall Fade_Set(uint32_t v);
int32_t __fastcall Camera_InBounds(uint8_t* c, int, float x, float y);
double __fastcall Scroll_Visible(uint8_t* s);
void __stdcall Camera_DetailFactor(int32_t x, int32_t z, float* out);
int32_t __fastcall Sound3D_Command(uint8_t* sys, int, uint8_t* h, int32_t* cmd);
int32_t __fastcall Sound2D_Command(uint8_t* s, int, uint8_t* node, int32_t* ctx);

char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*, const char*, const char*);

static StreamRecord* const g_streams = reinterpret_cast<StreamRecord*>(0xccb880);
static const int kStreams = 10;
static int32_t& g_streamsReady = hook::Global<int32_t>(0xccb9ec);
static int32_t& g_skipCount = hook::Global<int32_t>(0xccb9e8);
static const int kOpenRead = 0x8000;

           // _O_RDONLY | _O_BINARY
static const int kOpenWrite = 0x8301;

          // _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY
static const int kChunk = 200000;

 // set around writes whose first 4 bytes are a heap pointer
uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len);

// Functions of this file, in exe order.
RT3_NOINLINE char* __fastcall Num_Format(char* buf, int32_t style, double v, int32_t width, int32_t unused, int32_t inK, int32_t inM);
RT3_NOINLINE void __fastcall SceneObj_Init(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
RT3_NOINLINE void __fastcall SceneObj_Destruct(uint8_t* o);
int32_t __fastcall Mesh_RayHit(uint8_t* o, int, const float* dir, const float* origin);
void __fastcall SceneObj_DrawMarker(uint8_t* o);
int32_t __fastcall SceneObj_RecentlyDrawn(uint8_t* o);
void __fastcall SceneObj_SetAttachmentsFlag(uint8_t* o, int, int32_t on);
RT3_NOINLINE uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex);
RT3_NOINLINE uint8_t* __fastcall SceneObj_ConstructDefault(uint8_t* o);
RT3_NOINLINE void __fastcall SceneObj_Destructor(uint8_t* o);
void __fastcall SceneObj_ApplyStack(uint8_t* o, int, void* ms);
RT3_NOINLINE void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z);
RT3_NOINLINE int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);
RT3_NOINLINE void StreamInit();
RT3_NOINLINE void __fastcall StreamClose(Stream s);
int32_t __fastcall StreamTell(Stream s);
int32_t __fastcall StreamFd(Stream s);
void __fastcall StreamRelease(Stream s);
int32_t __fastcall StreamSize(Stream s);
int32_t __fastcall FileExists(const char* path);
RT3_NOINLINE void StreamReleaseAll();
RT3_NOINLINE Stream __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
RT3_NOINLINE void __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t);
RT3_NOINLINE int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t);
RT3_NOINLINE void __fastcall StreamWriteU32(Stream s, uint32_t value);
RT3_NOINLINE int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count);
int32_t __fastcall StreamReadString(Stream s, char** out);
RT3_NOINLINE void __fastcall StreamWriteLenString(Stream s, const char* const* str);
void __fastcall StreamWriteText(Stream s, const char* str);
uint32_t __fastcall Sound_Get3DProvider(int32_t n);
uint32_t __fastcall Sound_Is3D(uint8_t* s);
void __fastcall Sound_StopMusic(uint8_t* s, int, uint32_t notify);
RT3_NOINLINE void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind);
void __fastcall Sound_TransformPoint(uint8_t* s, int, float* x, float* y, float* z);
RT3_NOINLINE uint32_t __fastcall Sound_ActiveProvider(uint8_t* s);
RT3_NOINLINE int32_t __fastcall Sound_ActiveProviderIndex(uint8_t* s);
RT3_NOINLINE uint32_t __fastcall Sound_ProviderFlagsAt(uint8_t* s, int, int32_t n);
void __fastcall Sound_ListenerPos(uint8_t* s, int, float x, float y, float z);
void __fastcall Sound_ListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz);
RT3_NOINLINE void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply);
RT3_NOINLINE int32_t __fastcall Sound_GetSpeakers(uint8_t* s);
void __fastcall Sound_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol);
RT3_NOINLINE void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Sound_ReleaseTimer(uint8_t* s);
void __fastcall Sound_WaitIdle();
int32_t __fastcall Sound_Command(uint8_t* h, int32_t* cmd);
int32_t __fastcall Sound_Startup(uint8_t* snd, int, const char* name);
void __fastcall Sound_Shutdown(uint8_t* snd, int, int32_t immediate);
RT3_NOINLINE int32_t __fastcall Sound_Restart(uint8_t* snd, int, const char* name);
int32_t __fastcall Sound_PlayMusic(uint8_t* s, int, const char* file, void* onEnd);
RT3_NOINLINE uint32_t __fastcall Sound_ListenerIdle(uint8_t* s);
RT3_NOINLINE uint32_t __fastcall Sound_Play2D(uint8_t* snd, int, const char* name, float vol, uint32_t pan, int32_t mode, int32_t paused, void* data);
RT3_NOINLINE uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol, int32_t mode, int32_t paused, int32_t prio, float maxDist);
void __fastcall Sound_SetMatrix(uint8_t* s, int, Mat16 m);
RT3_NOINLINE uint32_t __fastcall Sound_Get2c(uint8_t* s);
RT3_NOINLINE int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Sound_Pause(uint8_t* s);
int32_t __fastcall Sound_RegisterEos(uint8_t* s, int, uint32_t sample, int32_t kind);
void __stdcall Music_FadeStep(int32_t mode);
RT3_NOINLINE void __fastcall Font_DrawTextBox(uint8_t* f, int, const char* text, float x, float y, float w, float h, uint32_t color, int32_t align, int32_t valign, uint32_t flags, float scale);
RT3_NOINLINE void __fastcall Glyph_Queue(uint8_t* g, int, float x, float y, uint32_t tint, float sx, float sy, uint32_t color, int32_t, int32_t snap, int32_t clip, float left, float top, float right, float bottom);

// Sound objects: +4 == 1 for 3D sounds; 0xccb9f4 set while 3D sound is unavailable.
static inline bool Is3D(uint8_t* s) { return F<int32_t>(s, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }

static inline uint8_t* Sound3D() { return hook::Global<uint8_t*>(0xccb9fc); }

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

static inline uint8_t*& Snd2D() { return hook::Global<uint8_t*>(0xccb9f8); }

static inline uint8_t*& Snd3D() { return hook::Global<uint8_t*>(0xccb9fc); }

static inline uint8_t*& Music() { return hook::Global<uint8_t*>(0xccba00); }

static inline void Sleep_(uint32_t ms) { Import<void(__stdcall*)(uint32_t)>(0x5c81f0)(ms); }

// Destroys and clears one of the sound singletons.
static void DestroySnd(uint8_t*& p, void(__fastcall* dtor)(uint8_t*)) {
    if (uint8_t* o = p) {
        dtor(o);
        ExeDelete(o);
    }
    p = nullptr;
}

// Whether sound is up (+4) and not shutting down.
static inline bool SoundUp(uint8_t* snd) { return F<int32_t>(snd, 4) == 1 && !hook::Global<int32_t>(0xccb9f4); }

static inline int32_t Cell16(float v) {
    double d = (double)v * *(const float*)0x5dcfe8;
    int32_t r;
    __asm {
        fld d
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r >> 4;
}

static inline double D(uint32_t a) { return *(const double*)a; }

static inline double Fl(uint32_t a) { return *(const float*)a; }

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

static inline float BitsF(uint32_t b) { return *(float*)&b; }

static inline void FatalError(const char* msg) { ((void(__fastcall*)(const char*))0x51e980)(msg); }

static void FatalCannotOpen(const char* path) {
    char msg[0x180];
    FormatText(msg, GetString(0xf1b), path, 0, 0, 0, 0, 0, 0, 0);
    FatalError(msg);
}

// 0x530030. Whether the ray (origin, direction) hits a triangle of the object's
// current sub-mesh (+0x21 of LOD +0x3a): both are taken into the object's space
// with its inverse matrix (+0x8e, rebuilt from +0xce when +0x20 bit 2 is set) after
// subtracting its position (+0x1e2).
int32_t __fastcall Mesh_RayHit(uint8_t* o, int, const float* dir, const float* origin) {
    g_lod = F<uint8_t*>(o, 0x14) + F<int32_t>(o, 0x3a) * 0x18;
    g_sub = F<uint8_t*>(g_lod, 0xc) + F<int32_t>(o, 0x21) * 0x45;
    g_tri = F<uint8_t*>(g_sub, 4);
    g_left = F<int32_t>(g_sub, 0xc);
    float x = dir[0], y = dir[1], z = dir[2];
    if (o[0x20] & 4) {
        o[0x20] &= 0xfb;
        float det = 0.0f;
        ((void*(__stdcall*)(void*, float*, const void*))0x56d8d9)(o + 0x8e, &det, o + 0xce);
    }
    const float* m = (const float*)(o + 0x8e);
    float dx = (float)((double)z * m[8] + (double)x * m[0] + (double)y * m[4]);
    float dy = (float)((double)x * m[1] + (double)y * m[5] + (double)z * m[9]);
    float dz = (float)((double)y * m[6] + (double)z * m[10] + (double)x * m[2]);
    double px = (double)origin[0] - F<float>(o, 0x1e2);
    double py = (double)origin[1] - F<float>(o, 0x1e6);
    double pz = (double)origin[2] - F<float>(o, 0x1ea);
    float ox = (float)(pz * m[8] + px * m[0] + py * m[4]);
    float oy = (float)(px * m[1] + py * m[5] + pz * m[9]);
    float oz = (float)(py * m[6] + pz * m[10] + px * m[2]);
    if (!g_left) return 0;
    do {
        const float* a = F<const float*>(g_tri, 0);
        const float* b = F<const float*>(g_tri, 4);
        const float* c = F<const float*>(g_tri, 8);
        if (Ray_HitsTriangle_Impl(a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], ox, oy, oz, dx, dy, dz)) return 1;
        g_tri += 0x4c;
    } while (--g_left);
    return 0;
}
RT3_REPLACE(0x530030, Mesh_RayHit);

// 0x530280. Draws a scene object's marker (needs the overlay font): vtbl +0x30
// gives the kind (1 text, 2 icon, 3 text with the icon above it), the text,
// its colour, a fog bias and the icon; the height above the object (+0x1e6)
// comes from vtbl +0x34 (text kinds) or +0x38. Projected from (+0x1e2, +0x1ea),
// faded by depth, only inside the camera's bounds. Text is centred at 0.6 x
// 0.2 in the fog-scaled font; the icon is scaled by the camera detail factor
// (shrunk to vtbl +0x3c past 1; under 0.14 it fades) or 1.1 x the fog strength
// above text, and skipped at 0.1 or less.
void __fastcall SceneObj_DrawMarker(uint8_t* o) {
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    if (!e || !F<uint32_t>(e, 0x1141f5)) return;
    char text[0x130];
    int32_t color = -1;
    float bias = 0.0f;
    uint8_t* icon = nullptr;
    int32_t kind = ((int32_t(__thiscall*)(uint8_t*, char*, int32_t, int32_t*, float*, uint8_t**))(*(void***)o)[0x30 / 4])(o, text, 0x12c, &color, &bias,
                                                                                                                       &icon);
    if (!kind) return;
    int32_t h = kind == 1 || kind == 3 ? ((int32_t(__thiscall*)(uint8_t*))(*(void***)o)[0x34 / 4])(o)
                                       : ((int32_t(__thiscall*)(uint8_t*))(*(void***)o)[0x38 / 4])(o);
    float sx, sy, depth;
    int32_t shown = Scene_ProjectNormalised((uint8_t*)Engine_Renderer(hook::Global<uint8_t*>(0x6d4024)), 0, F<float>(o, 0x1e2),
                                            (float)((double)h + F<float>(o, 0x1e6)), F<float>(o, 0x1ea), &sx, &sy, &depth);
    Fade_Save();
    Fade_Set(*(uint32_t*)&depth);
    float scale = 1.0f, strength = 1.0f;
    float size;
    if (shown && Camera_InBounds(F<uint8_t*>(hook::Global<uint8_t*>(0x6d4024), 0x28), 0, sx, sy)) {
        bool drawIcon = false;
        if (kind == 1 || kind == 3) {
            uint8_t* font;
            int32_t fz = Ftol(F<float>(o, 0x1ea));
            int32_t fx = Ftol(F<float>(o, 0x1e2));
            Fog_ParamsAt((uint8_t*)Engine_Get1C(hook::Global<uint8_t*>(0x6d4024)), 0, fx, fz, bias, &scale, (uint32_t*)&font, &strength);
            Font_DrawTextBox(font, 0, text, (float)((double)sx - D(0x5c88c8)), (float)((double)sy - D(0x5c86b8)), BitsF(0x3f19999a), BitsF(0x3e4ccccd),
                             (uint32_t)color, 0, 0, 0, scale);
            if (kind == 3) {
                sy = (float)(Scroll_Visible(font) * scale * D(0x5ca0a8) + sy);
                size = (float)((double)strength * D(0x5c85a8));
                drawIcon = true;
            }
        } else if (kind == 2) {
            int32_t fz = Ftol(F<float>(o, 0x1ea));
            int32_t fx = Ftol(F<float>(o, 0x1e2));
            Camera_DetailFactor(fx, fz, &size);
            if (!(size <= D(0x5c8770))) {
                double m = ((double(__thiscall*)(uint8_t*))(*(void***)o)[0x3c / 4])(o);
                if (m < size) size = (float)m;
            }
            drawIcon = true;
        }
        if (drawIcon && size > D(0x5c86b8)) {
            if (kind != 3) {
                if (size < D(0x5c8620)) color = (Ftol(((double)size - D(0x5c86b8)) * D(0x5dd238) * Fl(0x5ca0ec)) << 24) | 0xffffff;
                else color = -1;
            }
            float gx = (float)((double)sx - Slider_StepA(icon) * size * D(0x5c8628));
            float gy = (float)((double)sy - Slider_StepB(icon) * size * D(0x5c8628));
            Glyph_Queue(icon, 0, gx, gy, 0x3f800000, size, size, (uint32_t)color, 0, 1, 0, 0.0f, 0.0f, 1.0f, 1.0f);
        }
    }
    Fade_Restore();
}
RT3_REPLACE(0x530280, SceneObj_DrawMarker);

// 0x5305c0. Whether the object was drawn within the last two renderer frames (+0x65).
RT3_NOINLINE int32_t __fastcall SceneObj_RecentlyDrawn(uint8_t* o) {
    int32_t d = F<int32_t>(Engine_Renderer(hook::Global<void*>(0x6d4024)), 5) - F<int32_t>(o, 0x65);
    return d > 2 || d < 0 ? 0 : 1;
}
RT3_REPLACE(0x5305c0, SceneObj_RecentlyDrawn);

// 0x5305f0. Sets or clears flag 1 (+0xb0) on every attachment (+0x79).
void __fastcall SceneObj_SetAttachmentsFlag(uint8_t* o, int, int32_t on) {
    if (!F<PtrList*>(o, 0x79)) return;
    PtrList_Rewind(F<PtrList*>(o, 0x79));
    for (uint8_t* a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79)); a;
         a = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x79))) {
        if (on) F<uint32_t>(a, 0xb0) |= 1;
        else F<uint32_t>(a, 0xb0) &= ~1u;
    }
}
RT3_REPLACE(0x5305f0, SceneObj_SetAttachmentsFlag);

// 0x530640. Constructs a scene object (vtable 0x5dd1f0) for owner with model and texture.
RT3_NOINLINE uint8_t* __fastcall SceneObj_Construct(uint8_t* o, int, uint32_t owner, const char* model, const char* tex) {
    F<uint32_t>(o, 0) = 0x5dd1f0;
    SceneObj_Init(o, 0, owner, model, tex);
    return o;
}
RT3_REPLACE(0x530640, SceneObj_Construct);

// 0x530670. Scene object constructor.
RT3_NOINLINE uint8_t* __fastcall SceneObj_ConstructDefault(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5dd1f0;
    SceneObj_Reset_Impl(o);
    return o;
}
RT3_REPLACE(0x530670, SceneObj_ConstructDefault);

// 0x530680. Scene object destructor.
RT3_NOINLINE void __fastcall SceneObj_Destructor(uint8_t* o) {
    F<uint32_t>(o, 0) = 0x5dd1f0;
    SceneObj_Destruct(o);
}
RT3_REPLACE(0x530680, SceneObj_Destructor);

// 0x530690. Places scene object o under the current top of matrix stack ms
// (ID3DXMatrixStack): keeps the parent matrix (+0x10e), multiplies in its own world
// matrix (+0x14e, rebuilt if dirty) for its world transform (+0xce), and recurses
// into its children (+0x75) before popping.
void __fastcall SceneObj_ApplyStack(uint8_t* o, int, void* ms) {
    void** vt = *(void***)ms;
    auto top = [&]() { return ((const uint32_t*(__stdcall*)(void*))(*(void***)ms)[0x44 / 4])(ms); };
    memcpy(o + 0x10e, top(), 0x40);
    ((long(__stdcall*)(void*))vt[0x10 / 4])(ms);
    SceneObj_UpdateMatrix(o);
    ((long(__stdcall*)(void*, const void*))(*(void***)ms)[0x20 / 4])(ms, o + 0x14e);
    memcpy(o + 0xce, top(), 0x40);
    if (F<PtrList*>(o, 0x75)) {
        PtrList_Rewind(F<PtrList*>(o, 0x75));
        for (uint8_t* c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)); c; c = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x75)))
            SceneObj_ApplyStack(c, 0, ms);
    }
    ((long(__stdcall*)(void*))(*(void***)ms)[0xc / 4])(ms);
}
RT3_REPLACE(0x530690, SceneObj_ApplyStack);

// 0x530720. Moves the object to (x, y, z), marking it moved (+0x20 bit 1) and
// re-filing it in its terrain's 16-cell grid (node +8) when it changes cell (no
// bounds check); its sound channels follow.
RT3_NOINLINE void __fastcall Obj_MoveTo(uint8_t* o, int, float x, float y, float z) {
    o[0x20] |= 2;
    if (!F<uint8_t*>(o, 4)) {
        F<float>(o, 0x1e2) = x;
        F<float>(o, 0x1e6) = y;
        F<float>(o, 0x1ea) = z;
        Obj_MoveChannels(o);
        return;
    }
    int32_t ox = Cell16(F<float>(o, 0x1e2)), oz = Cell16(F<float>(o, 0x1ea));
    F<float>(o, 0x1e2) = x;
    F<float>(o, 0x1e6) = y;
    F<float>(o, 0x1ea) = z;
    int32_t cx = Cell16(x), cz = Cell16(z);
    if (ox != cx || oz != cz) {
        if (F<PtrListNode*>(o, 8)) Terrain_Remove1671(F<uint8_t*>(o, 4), 0, F<PtrListNode*>(o, 8), ox, oz);
        F<PtrListNode*>(o, 8) = Terrain_Add1671(F<uint8_t*>(o, 4), 0, o, cx, cz);
    }
    Obj_MoveChannels(o);
}
RT3_REPLACE(0x530720, Obj_MoveTo);

// 0x530820. Debug tooltip for a scene object (engine +0x1146be on), into `dst`
// (at most n-1 chars): for a visible, recently drawn model object (+0x14) "Model #
// N  : Polys P   Area: A" (string 0xa7; model N = +0x21, polys from the model's
// part table, area in K) or in mode 1 just the model number ("X" when +0x5c bit 1);
// "Non 3DP World Object" (0xa8) without a model. Returns whether it wrote one; the
// last three arguments are unused.
RT3_NOINLINE int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t) {
    int32_t mode = F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x1146be);
    if (!mode || (o[0x20] & 0x10)) return 0;
    if (((int32_t(__fastcall*)(uint8_t*))(*(void***)o)[0x14 / 4])(o)) return 0;
    if (!SceneObj_RecentlyDrawn(o)) return 0;
    char text[0x1f4];
    uint8_t* model = F<uint8_t*>(o, 0x14);
    if (!model) {
        FormatText(text, GetString(0xa8), 0, 0, 0, 0, 0, 0, 0, 0);
    } else if (F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x1146be) == 1) {
        if (o[0x5c] & 2) {
            text[0] = 'X';
            text[1] = 0;
        } else {
            Num_Format(text, 8, (double)F<int32_t>(o, 0x21), 10, 1, 0, 0);
        }
    } else {
        char area[0x40], polys[0x40], num[0x40];
        uint8_t* parts = F<uint8_t*>(model, 0xc);
        Num_Format(area, 2, (double)F<float>(parts, 0x10), 10, 1, 0, 0);
        Num_Format(polys, 1, (double)F<int32_t>(parts, F<int32_t>(o, 0x21) * 0x45 + 0xc), 10, 1, 0, 0);
        Num_Format(num, 1, (double)F<int32_t>(o, 0x21), 10, 1, 0, 0);
        FormatText(text, GetString(0xa7), num, polys, area, 0, 0, 0, 0, 0);
    }
    StrCopyN(dst, text, n - 1);
    return 1;
}
RT3_REPLACE(0x530820, SceneObj_DebugTooltip);

RT3_NOINLINE void StreamInit() {
    if (g_streamsReady == 0) {
        memset(g_streams, 0, sizeof(StreamRecord) * kStreams);
        g_streamsReady = 1;
    }
}
RT3_REPLACE(0x5309d0, StreamInit);

RT3_NOINLINE void __fastcall StreamClose(Stream s) {
    StreamRecord& r = g_streams[s];
    switch (r.mode) {
    case kStreamWriteFile:
    case kStreamReadFile:
        Close(r.fd);
        r.allocated = 0;
        break;
    case kStreamWriteBuffered: {
        int fd = Open(r.path, kOpenWrite, 0x80);
        if (fd == -1) {
            r.open = 0;
            r.allocated = 0;
            Close(-1);
            return; // QUIRK: path and buffer are leaked
        }
        Write(fd, r.buffer, r.size);
        Close(fd);
        Free(r.buffer); // QUIRK: pointer not cleared
        r.allocated = 0;
        break;
    }
    case kStreamWriteMemory:
        if (r.size < r.capacity) { // shrink to fit; stays allocated for mode-3 re-open
            uint8_t* b = (uint8_t*)Malloc(r.size);
            memcpy(b, r.buffer, r.size);
            Free(r.buffer);
            r.buffer = b;
            r.capacity = r.size;
        }
        break;
    case kStreamReadMemory:
        break;
    case kStreamReadWhole:
        FreeAndNull((void**)&r.buffer);
        r.capacity = 0;
        r.allocated = 0;
        break;
    case kStreamReadChunked:
        FreeAndNull((void**)&r.buffer);
        r.capacity = 0;
        r.allocated = 0;
        Close(r.fd);
        break;
    }
    if (r.allocated == 0) FreeAndNull((void**)&r.path);
    r.open = 0;
}
RT3_REPLACE(0x530a00, StreamClose);

int32_t __fastcall StreamTell(Stream s) { return Tell(g_streams[s].fd); }
RT3_REPLACE(0x530b70, StreamTell);

int32_t __fastcall StreamFd(Stream s) { return g_streams[s].fd; }
RT3_REPLACE(0x530b90, StreamFd);

void __fastcall StreamRelease(Stream s) {
    StreamRecord& r = g_streams[s];
    if (r.open) StreamClose(s);
    if (r.allocated) {
        Free(r.path);
        if (r.buffer) Free(r.buffer);
        r.allocated = 0;
        r.path = nullptr;
        r.buffer = nullptr;
    }
}
RT3_REPLACE(0x530ba0, StreamRelease);

int32_t __fastcall StreamSize(Stream s) { return g_streams[s].size; }
RT3_REPLACE(0x530c10, StreamSize);

int32_t __fastcall FileExists(const char* path) {
    int fd = Open(path, kOpenRead, 0);
    if (fd == -1) return 0;
    Close(fd);
    return 1;
}
RT3_REPLACE(0x530c20, FileExists);

RT3_NOINLINE void StreamReleaseAll() {
    if (g_streamsReady == 0) return;
    for (int i = 0; i < kStreams; ++i) StreamRelease(i);
    g_streamsReady = 0;
}
RT3_REPLACE(0x530c50, StreamReleaseAll);

RT3_NOINLINE Stream __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing) {
    StreamInit();
    int k = -1;
    if (mode == kStreamReadMemory || mode == kStreamWriteMemory) {
        for (int i = 0; i < kStreams; ++i) {
            StreamRecord& r = g_streams[i];
            if (r.allocated && (r.mode == kStreamReadMemory || r.mode == kStreamWriteMemory) &&
                Stricmp(r.path, path) == 0) {
                if (mode != kStreamWriteMemory) { // re-open the kept buffer for reading
                    r.open = 1;
                    r.pos = 0;
                    return i;
                }
                StreamRelease(i); // rewrite: reuse this slot
                k = i;
                break;
            }
        }
    }
    if (k < 0) {
        if (mode == kStreamReadMemory) FatalCannotOpen(path); // no such buffer
        for (int i = 0; i < kStreams; ++i)
            if (!g_streams[i].allocated) { k = i; break; }
        if (k < 0) return 0; // QUIRK: 0 is also a valid handle
    }

    StreamRecord& r = g_streams[k];
    r.size = 0;
    r.pos = 0;
    r.mode = mode;
    r.open = 1;
    r.allocated = 1;
    size_t n = strlen(path) + 1;
    r.path = (char*)Malloc(n);
    r.capacity = 0;
    r.buffer = nullptr;
    memcpy(r.path, path, n);

    if (r.mode == kStreamReadFile || r.mode == kStreamReadWhole || r.mode == kStreamReadChunked) {
        r.fd = Open(path, kOpenRead, 0x100);
        if (r.fd == -1) {
            if (fatalIfMissing) FatalCannotOpen(path);
            FreeAndNull((void**)&r.path);
            r.open = 0;
            r.allocated = 0;
            return -1;
        }
        if (r.mode == kStreamReadWhole) {
            r.capacity = r.size = FileLength(r.fd);
            r.buffer = (uint8_t*)Malloc(r.capacity);
            Read(r.fd, r.buffer, r.capacity);
            Close(r.fd);
        } else if (r.mode == kStreamReadChunked) {
            if (FileLength(r.fd) < kChunk) {
                r.capacity = r.size = FileLength(r.fd);
                r.mode = kStreamReadWhole;
            } else {
                r.capacity = r.size = kChunk;
            }
            r.buffer = (uint8_t*)Malloc(r.capacity);
            Read(r.fd, r.buffer, r.capacity);
            if (r.mode == kStreamReadWhole) Close(r.fd);
        }
        return k;
    }
    if (r.mode == kStreamWriteFile) {
        r.fd = Open(path, kOpenWrite, 0x80);
        if (r.fd != -1) return k;
        r.allocated = 0;
        r.open = 0;
        Close(-1);
        return -1;
    }
    // Memory modes: check the file can be created (QUIRK: this truncates it).
    Stream probe = StreamOpen(path, kStreamWriteFile, kChunk, 1);
    if (probe == -1) return -1; // QUIRK: slot k stays taken
    StreamClose(probe);
    r.buffer = (uint8_t*)Malloc(bufferSize);
    r.capacity = bufferSize;
    return k;
}
RT3_REPLACE(0x530c80, StreamOpen);

// Save tracing for the determinism harness: when a trace file is set, every
// write logs its caller, size and CRC, so two runs' saves can be compared
// write by write (which subsystem diverged first).
FILE* g_writeTrace;

int g_traceMaskPointer;

RT3_NOINLINE void __fastcall StreamWrite(Stream s, const void* data, uint32_t size, int32_t) {
    if (g_writeTrace) {
        uint32_t skip = (g_traceMaskPointer && size >= 4) ? 4 : 0;
        fprintf(g_writeTrace, "%08x %u %08x\n", (unsigned)(uintptr_t)_ReturnAddress(), size,
                Crc32((const uint8_t*)data + skip, size - skip));
    }
    StreamRecord& r = g_streams[s];
    if (r.mode == kStreamWriteMemory || r.mode == kStreamWriteBuffered) {
        while (r.capacity < (int32_t)(r.size + size)) {
            int32_t grow = r.capacity / 3 > 99999 ? r.capacity / 3 : 100000;
            uint8_t* b = (uint8_t*)Malloc(r.capacity + grow);
            memcpy(b, r.buffer, r.size);
            Free(r.buffer);
            r.buffer = b;
            r.capacity += grow;
        }
        memcpy(r.buffer + r.pos, data, size);
        r.pos += size;
        if (r.pos > r.size) r.size = r.pos;
    } else if (r.mode == kStreamWriteFile) {
        Write(r.fd, data, size);
    }
}
RT3_REPLACE(0x531030, StreamWrite);

RT3_NOINLINE int32_t __fastcall StreamRead(Stream s, void* dst, uint32_t size, int32_t, int32_t) {
    StreamRecord& r = g_streams[s];
    uint8_t* out = (uint8_t*)dst;
    if (r.mode == kStreamReadFile) return Read(r.fd, dst, size);
    if (r.mode != kStreamReadChunked) {
        int32_t n = r.size - r.pos;
        if ((int32_t)size < n) n = size;
        memcpy(out, r.buffer + r.pos, n);
        r.pos += n;
        return n;
    }
    int32_t avail = kChunk - r.pos; // QUIRK: ignores the real file size; past the end reads zeros
    if (avail < (int32_t)size) {
        memcpy(out, r.buffer + r.pos, avail);
        memset(r.buffer, 0, kChunk);
        Read(r.fd, r.buffer, kChunk);
        int32_t rest = size - avail;
        r.pos = 0;
        if (rest > kChunk) {
            int32_t chunks = (rest - (kChunk + 1)) / kChunk + 1;
            rest -= chunks * kChunk;
            do {
                memcpy(out + avail, r.buffer, kChunk);
                memset(r.buffer, 0, kChunk);
                avail += kChunk;
                Read(r.fd, r.buffer, kChunk);
            } while (--chunks);
        }
        memcpy(out + avail, r.buffer, rest);
        r.pos += rest;
        return size;
    }
    memcpy(out, r.buffer + r.pos, size);
    r.pos += size;
    return size;
}
RT3_REPLACE(0x531150, StreamRead);

RT3_NOINLINE void __fastcall StreamWriteU32(Stream s, uint32_t value) { StreamWrite(s, &value, 4, 0); }
RT3_REPLACE(0x531340, StreamWriteU32);

// Reads 4 bytes and drops them; counts the skip (0xccb9e8) when asked.
RT3_NOINLINE int32_t __fastcall StreamSkip4(Stream s, int, int32_t, int32_t count) {
    if (count) g_skipCount++;
    uint32_t junk;
    return StreamRead(s, &junk, 4, 0, 1);
}
RT3_REPLACE(0x531360, StreamSkip4);

// Length-prefixed string: 1 byte length, or 2 bytes (high bit set) for 128..32767.
// The stored length includes the terminator. *out = NULL for length 0.
int32_t __fastcall StreamReadString(Stream s, char** out) {
    if (*out) Free(*out);
    *out = nullptr;
    uint8_t len;
    int32_t total = StreamRead(s, &len, 1, 0, 1);
    if (len == 0) return total;
    uint32_t size;
    if (len < 0x80) {
        size = len;
    } else {
        uint8_t low;
        size = (uint32_t)(len & 0x7f) << 8;
        total += StreamRead(s, &low, 1, 0, 1);
        size |= low;
    }
    *out = (char*)Malloc(size);
    return StreamRead(s, *out, size, 0, 1) + total;
}
RT3_REPLACE(0x531380, StreamReadString);

// 0x531410. Writes *str with its length (incl. NUL) as a 1-byte prefix, or 2 bytes
// (high byte | 0x80 first) past 0x7f; a null string is a single 0 byte.
RT3_NOINLINE void __fastcall StreamWriteLenString(Stream s, const char* const* str) {
    const char* p = *str;
    uint8_t b;
    if (!p) {
        b = 0;
        StreamWrite(s, &b, 1, 0);
        return;
    }
    int32_t len = 0;
    while (p[len]) ++len;
    ++len;
    if (len > 0x7f) {
        b = (uint8_t)((len >> 8) | 0x80);
        StreamWrite(s, &b, 1, 0);
    }
    b = (uint8_t)len;
    StreamWrite(s, &b, 1, 0);
    StreamWrite(s, *str, len, 0);
}
RT3_REPLACE(0x531410, StreamWriteLenString);

// 0x531490. Writes str without its NUL.
void __fastcall StreamWriteText(Stream s, const char* str) {
    uint32_t len = 0;
    while (str[len]) ++len;
    StreamWrite(s, str, len, 0);
}
RT3_REPLACE(0x531490, StreamWriteText);

// 0x5314b0. The n-th Miles 3D provider (AIL_enumerate_3D_providers), 0 when there is none.
RT3_NOINLINE uint32_t __fastcall Sound_Get3DProvider(int32_t n) {
    uint32_t next = 0;
    uint8_t name[4];
    hook::Global<uint32_t>(0xccba08) = 0;
    auto enumerate = Import<int32_t(__stdcall*)(uint32_t*, void*, uint32_t*)>(0x5c8454);
    for (int32_t i = 0; i <= n; ++i)
        if (!enumerate(&next, name, (uint32_t*)0xccba08)) return 0;
    return hook::Global<uint32_t>(0xccba08);
}
RT3_REPLACE(0x5314b0, Sound_Get3DProvider);

uint32_t __fastcall Sound_Is3D(uint8_t* s) {
    if (F<int32_t>(s, 4) != 1) return 0;
    return hook::Global<int32_t>(0xccb9f4) ? 0 : 1;
}
RT3_REPLACE(0x531500, Sound_Is3D);

// 0x531520. Stops the game music (0xccba00) when it is playing or paused.
void __fastcall Sound_StopMusic(uint8_t* s, int, uint32_t notify) {
    if (F<int32_t>(s, 4) != 1 || hook::Global<int32_t>(0xccb9f4)) return;
    uint8_t* m = hook::Global<uint8_t*>(0xccba00);
    if (Music_IsPlaying(m) == 1 || Music_IsPaused(hook::Global<uint8_t*>(0xccba00)) == 1)
        Music_Stop(hook::Global<uint8_t*>(0xccba00), 0, notify);
}
RT3_REPLACE(0x531520, Sound_StopMusic);

// 0x531560. Sets a volume: kind 0 music, kind 2 effects (2D and 3D).
RT3_NOINLINE void __fastcall Sound_SetVolume(uint8_t* s, int, float vol, int32_t kind) {
    if (!Is3D(s)) return;
    if (kind == 0) {
        Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, vol);
    } else if (kind == 2) {
        Sound2D_SetVolume(hook::Global<uint8_t*>(0xccb9f8), 0, vol);
        Sound_SetEffectsVolume(hook::Global<uint8_t*>(0xccb9fc), 0, vol);
    }
}
RT3_REPLACE(0x531560, Sound_SetVolume);

// 0x5315c0. Transforms a point by the sound's matrix (+8, rows of 4 floats).
void __fastcall Sound_TransformPoint(uint8_t* s, int, float* x, float* y, float* z) {
    double px = *x, py = *y, pz = *z;
    const float* m = (const float*)(s + 8);
    *x = (float)(pz * m[8] + py * m[4] + px * m[0] + m[12]);
    *y = (float)(pz * m[9] + py * m[5] + px * m[1] + m[13]);
    *z = (float)(pz * m[10] + py * m[6] + px * m[2] + m[14]);
}
RT3_REPLACE(0x5315c0, Sound_TransformPoint);

// 0x531620. The active 3D provider's info (or the empty default 0xccba0c).
RT3_NOINLINE uint32_t __fastcall Sound_ActiveProvider(uint8_t* s) {
    if (Is3D(s)) return Sound_ProviderInfo(Sound3D());
    return 0xccba0c;
}
RT3_REPLACE(0x531620, Sound_ActiveProvider);

// 0x531640. Index of the active provider among the enumerated ones (the count if absent).
RT3_NOINLINE int32_t __fastcall Sound_ActiveProviderIndex(uint8_t* s) {
    int32_t i = 0;
    if (!Is3D(s)) return 0;
    const char* cur = (const char*)Sound_ProviderInfo(Sound3D());
    for (const char* p = (const char*)Sound_Get3DProvider(0); p; p = (const char*)Sound_Get3DProvider(++i)) {
        const char* a = cur;
        const char* b = p;
        while (*a == *b && *a) ++a, ++b;
        if (*a == *b) return i;
    }
    return i;
}
RT3_REPLACE(0x531640, Sound_ActiveProviderIndex);

// 0x5316d0. Flags of the n-th 3D provider (0 without 3D sound).
RT3_NOINLINE uint32_t __fastcall Sound_ProviderFlagsAt(uint8_t* s, int, int32_t n) {
    if (!Is3D(s)) return 0;
    const char* name = (const char*)Sound_Get3DProvider(n);
    return Sound_ProviderFlags(hook::Global<uint8_t*>(0xccb9fc), 0, name);
}
RT3_REPLACE(0x5316d0, Sound_ProviderFlagsAt);

void __fastcall Sound_ListenerPos(uint8_t* s, int, float x, float y, float z) {
    if (Is3D(s)) Sound_SetListenerPos(Sound3D(), 0, x, y, z);
}
RT3_REPLACE(0x531710, Sound_ListenerPos);

void __fastcall Sound_ListenerDir(uint8_t* s, int, float fx, float fy, float fz, float ux, float uy, float uz) {
    if (Is3D(s)) Sound_SetListenerDir(Sound3D(), 0, fx, fy, fz, ux, uy, uz);
}
RT3_REPLACE(0x531730, Sound_ListenerDir);

RT3_NOINLINE void __fastcall Sound_Speakers(uint8_t* s, int, int32_t type, int32_t noApply) {
    if (Is3D(s)) Sound_SetSpeakerType(Sound3D(), 0, type, noApply);
}
RT3_REPLACE(0x531750, Sound_Speakers);

RT3_NOINLINE int32_t __fastcall Sound_GetSpeakers(uint8_t* s) { return Is3D(s) ? Sound_SpeakerType() : 0; }
RT3_REPLACE(0x531770, Sound_GetSpeakers);

void __fastcall Sound_SetSampleVolume(uint8_t* s, int, uint32_t key, float vol) {
    if (Is3D(s)) Sound2D_SetSampleVolume(hook::Global<uint8_t*>(0xccb9f8), 0, key, vol);
}
RT3_REPLACE(0x531790, Sound_SetSampleVolume);

// 0x5317b0. Stops sample `key`: kind 0 = 2D, kind 1 = 3D.
RT3_NOINLINE void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind) {
    if (!Is3D(s)) return;
    if (kind == 0) Sound2D_Stop(hook::Global<uint8_t*>(0xccb9f8), 0, key);
    else if (kind == 1) Sound3D_Stop(hook::Global<uint8_t*>(0xccb9fc), 0, key, nullptr);
}
RT3_REPLACE(0x5317b0, Sound_Stop);

// 0x531880. Stops and releases the sound's Miles timer (+0x4c).
RT3_NOINLINE void __fastcall Sound_ReleaseTimer(uint8_t* s) {
    if (F<int32_t>(s, 0x4c) == -1 || hook::Global<int32_t>(0x624d58) != 1) return;
    Import<void(__stdcall*)(int32_t)>(0x5c83b8)(F<int32_t>(s, 0x4c));
    Import<void(__stdcall*)(int32_t)>(0x5c8450)(F<int32_t>(s, 0x4c));
    F<int32_t>(s, 0x4c) = -1;
}
RT3_REPLACE(0x531880, Sound_ReleaseTimer);

// 0x5318b0. Waits until the sound thread clears 0xccb9f0.
void __fastcall Sound_WaitIdle() {
    while (*(volatile int32_t*)0xccb9f0) {}
}
RT3_REPLACE(0x5318b0, Sound_WaitIdle);

// 0x5318c0. Sends sound command cmd for h to the 3D (0xccba04) or 2D system.
int32_t __fastcall Sound_Command(uint8_t* h, int32_t* cmd) {
    if (hook::Global<int32_t>(0xccba04)) return Sound3D_Command(hook::Global<uint8_t*>(0xccb9fc), 0, h, cmd);
    return Sound2D_Command(hook::Global<uint8_t*>(0xccb9f8), 0, h, cmd);
}
RT3_REPLACE(0x5318c0, Sound_Command);

// 0x5318f0. Starts Miles (redist data\miles\) and creates the 2D (0xccb9f8), 3D
// (0xccb9fc, provider `name`) and music (0xccba00, volume 1) systems; +4 = 1 when
// all came up (the failed ones are torn down again).
RT3_NOINLINE int32_t __fastcall Sound_Startup(uint8_t* snd, int, const char* name) {
    Import<void(__stdcall*)(const char*)>(0x5c847c)((const char*)0x5dd240);
    Module_RegisterCleanup();
    Import<int32_t(__stdcall*)()>(0x5c83b4)();
    uint8_t* m = (uint8_t*)ExeNew(0x30);
    Snd2D() = m ? Mixer_Construct(m) : nullptr;
    if (Sound2D_Open(Snd2D()) == 1) {
        uint8_t* p = (uint8_t*)ExeNew(0x64);
        Snd3D() = p ? Sound2D_Construct(p) : nullptr;
        if (Sound3D_Open(Snd3D(), 0, name) == 1) {
            uint8_t* q = (uint8_t*)ExeNew(0x108);
            if (q) {
                F<uint32_t>(q, 0) = 0;
                F<uint32_t>(q, 0x104) = 0x3f800000;
            }
            Music() = q;
            if (Snd2D() && Snd3D() && q) F<int32_t>(snd, 4) = 1;
            return F<int32_t>(snd, 4);
        }
        DestroySnd(Snd3D(), Sound3D_Destruct);
    }
    DestroySnd(Snd2D(), Sound2D_Destruct);
    return F<int32_t>(snd, 4);
}
RT3_REPLACE(0x5318f0, Sound_Startup);

// 0x531a10. Shuts the sound system down (0xccb9f4 set meanwhile): unless
// `immediate`, fades music and effects out over steps of 0x5d12c8 / 0x5d0058 of
// their volume (16 ms apart, then 200 ms), waits for the worker (0xccb9f0), stops
// everything, releases the timer (+0x4c), closes Miles and frees the systems.
RT3_NOINLINE void __fastcall Sound_Shutdown(uint8_t* snd, int, int32_t immediate) {
    hook::Global<int32_t>(0xccb9f4) = 1;
    if (F<int32_t>(snd, 4) != 1) {
        F<int32_t>(snd, 4) = 0;
        hook::Global<int32_t>(0xccb9f4) = 0;
        return;
    }
    if (!immediate) {
        float mv = F<float>(Music(), 0x104), sv = F<float>(Snd3D(), 0x58);
        float ms = (float)((double)mv * *(const float*)0x5d12c8), ss = (float)((double)sv * *(const float*)0x5d0058);
        const float zero = *(const float*)0x5c8598;
        while (zero < mv || zero < sv) {
            mv = mv > ms ? (float)((double)mv - ms) : 0.0f;
            sv = sv > ss ? (float)((double)sv - ss) : 0.0f;
            Music_SetVolume(Music(), 0, mv);
            Sound_SetEffectsVolume(Snd3D(), 0, sv);
            Sound_PauseAll(Snd3D());
            Sleep_(0x10);
        }
        Sleep_(200);
    }
    while (hook::Global<volatile int32_t>(0xccb9f0)) {}
    Music_Stop(Music(), 0, 0);
    Sound2D_StopAll(Snd2D());
    Sound_StopAll(Snd3D());
    if (F<int32_t>(snd, 0x4c) != -1) {
        Import<void(__stdcall*)(int32_t)>(0x5c83b8)(F<int32_t>(snd, 0x4c));
        Import<void(__stdcall*)(int32_t)>(0x5c8450)(F<int32_t>(snd, 0x4c));
        F<int32_t>(snd, 0x4c) = -1;
    }
    Sound3D_Close(Snd3D());
    Sound2D_Close(Snd2D());
    Import<void(__stdcall*)()>(0x5c83ac)();
    DestroySnd(Music(), Music_Close);
    DestroySnd(Snd3D(), Sound3D_Destruct);
    DestroySnd(Snd2D(), Sound2D_Destruct);
    F<int32_t>(snd, 4) = 0;
    hook::Global<int32_t>(0xccb9f4) = 0;
}
RT3_REPLACE(0x531a10, Sound_Shutdown);

// 0x531c00. Restarts the sound system with 3D provider `name` and re-applies the
// listener and the music (engine +0x1146fe x 0.24) / effects (+0x1146fa) volumes.
RT3_NOINLINE int32_t __fastcall Sound_Restart(uint8_t* snd, int, const char* name) {
    if (!hook::Global<void*>(0xccb9fc)) return 0;
    char buf[0x1f8];
    char* d = buf;
    while ((*d++ = *name++) != 0) {}
    Sound_Shutdown(snd, 0, 1);
    if (Sound_Startup(snd, 0, buf) != 1) return 0;
    if (SoundUp(snd)) Sound_SetListenerDir(hook::Global<uint8_t*>(0xccb9fc), 0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    if (SoundUp(snd)) Sound_SetListenerPos(hook::Global<uint8_t*>(0xccb9fc), 0, 0.0f, 0.0f, 0.0f);
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    float mv = (float)((double)F<float>(e, 0x1146fe) * *(const double*)0x5c9c80);
    if (SoundUp(snd)) Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, mv);
    float sv = F<float>(hook::Global<uint8_t*>(0x6d4024), 0x1146fa);
    if (SoundUp(snd)) {
        Sound2D_SetVolume(hook::Global<uint8_t*>(0xccb9f8), 0, sv);
        Sound_SetEffectsVolume(hook::Global<uint8_t*>(0xccb9fc), 0, sv);
    }
    return 0;
}
RT3_REPLACE(0x531c00, Sound_Restart);

// 0x531d30. Starts a music file at the configured music volume (engine +0x1146fe).
int32_t __fastcall Sound_PlayMusic(uint8_t* s, int, const char* file, void* onEnd) {
    if (F<int32_t>(s, 4) != 1 || hook::Global<int32_t>(0xccb9f4)) return 0;
    float vol = (float)((double)F<float>(hook::Global<uint8_t*>(0x6d4024), 0x1146fe) * *(const double*)0x5c9c80);
    Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, vol);
    uint8_t* m = hook::Global<uint8_t*>(0xccba00);
    if (!(F<float>(m, 0x104) > *(const float*)0x5c8598)) return 0;
    if (Music_IsPlaying(m) == 1) Music_Stop(hook::Global<uint8_t*>(0xccba00), 0, 0);
    return Music_Play(hook::Global<uint8_t*>(0xccba00), 0, file,
                      F<void*>(hook::Global<uint8_t*>(0xccb9f8), 0x2c), onEnd);
}
RT3_REPLACE(0x531d30, Sound_PlayMusic);

// 0x531dc0. Whether the 3D listener is idle (both speeds zero); 1 for 2D sounds.
RT3_NOINLINE uint32_t __fastcall Sound_ListenerIdle(uint8_t* s) {
    if (!Is3D(s)) return 1;
    const float zero = *(const float*)0x5c8598;
    if (!(F<float>(hook::Global<uint8_t*>(0xccb9f8), 0x20) == zero)) return 0;
    return F<float>(hook::Global<uint8_t*>(0xccb9fc), 0x58) == zero ? 1 : 0;
}
RT3_REPLACE(0x531dc0, Sound_ListenerIdle);

// 0x531e10. Plays a 2D sound when the 2D volume (+0x20) is up; else 0.
RT3_NOINLINE uint32_t __fastcall Sound_Play2D(uint8_t* snd, int, const char* name, float vol, uint32_t pan, int32_t mode,
                                 int32_t paused, void* data) {
    if (!SoundUp(snd) || !(F<float>(Snd2D(), 0x20) > *(const float*)0x5c8598)) return 0;
    return Sound2D_Play(Snd2D(), 0, name, vol, pan, mode, paused, data);
}
RT3_REPLACE(0x531e10, Sound_Play2D);

// 0x531e50. Plays a named 3D sound when the effects volume (+0x58) is up; else 0.
RT3_NOINLINE uint32_t __fastcall Sound_Play3D(uint8_t* snd, int, const char* name, float x, float y, float z, float vol,
                                 int32_t mode, int32_t paused, int32_t prio, float maxDist) {
    if (!SoundUp(snd) || !(F<float>(Snd3D(), 0x58) > *(const float*)0x5c8598) || !name[0]) return 0;
    return Sound3D_Play(Snd3D(), 0, name, x, y, z, vol, mode, paused, prio, maxDist);
}
RT3_REPLACE(0x531e50, Sound_Play3D);

void __fastcall Sound_SetMatrix(uint8_t* s, int, Mat16 m) {
    if (!Is3D(s)) return;
    F<int32_t>(s, 0x48) = 1;
    for (int k = 0; k < 16; ++k) F<uint32_t>(s, 8 + k * 4) = m.v[k];
    F<int32_t>(hook::Global<uint8_t*>(0xccb9fc), 0x60) = 2;
}
RT3_REPLACE(0x531e90, Sound_SetMatrix);

RT3_NOINLINE uint32_t __fastcall Sound_Get2c(uint8_t* s) {
    if (!Is3D(s)) return 0;
    return F<uint32_t>(hook::Global<uint8_t*>(0xccb9f8), 0x2c);
}
RT3_REPLACE(0x531ed0, Sound_Get2c);

// 0x531ef0. Looks key up in the 2D (kind 0) or 3D (kind 1) sample table.
RT3_NOINLINE int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind) {
    if (F<int32_t>(s, 4) != 1 || hook::Global<int32_t>(0xccb9f4)) return 0;
    if (kind == 0) return Hash_Get((HashMap*)(hook::Global<uint8_t*>(0xccb9f8) + 0xc), 0, key);
    if (kind == 1) return Hash_Get((HashMap*)(hook::Global<uint8_t*>(0xccb9fc) + 0x14), 0, key);
    return 0;
}
RT3_REPLACE(0x531ef0, Sound_LookupSample);

// 0x531f40. Pauses all sound (stopping the fade timer first).
void __fastcall Sound_Pause(uint8_t* s) {
    if (!Is3D(s)) return;
    if (F<int32_t>(s, 0x4c) != -1 && hook::Global<int32_t>(0x624d58) == 1) Sound_ReleaseTimer(s);
    Sound2D_PauseAll(hook::Global<uint8_t*>(0xccb9f8));
    Sound_PauseAll(hook::Global<uint8_t*>(0xccb9fc));
    F<int32_t>(s, 0x48) = 0;
}
RT3_REPLACE(0x531f40, Sound_Pause);

// 0x531f90. Registers the end-of-sample callback (0x531800 for 2D samples,
// 0x531840 for 3D ones; AIL_register_(3D_)EOS_callback).
int32_t __fastcall Sound_RegisterEos(uint8_t* s, int, uint32_t sample, int32_t kind) {
    if (F<int32_t>(s, 4) != 1) return 0;
    if (int32_t busy = hook::Global<int32_t>(0xccb9f4)) return busy;
    if (kind == 0) return Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a8)(sample, 0x531800);
    if (kind == 1) return Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a4)(sample, 0x531840);
    return kind - 1;
}
RT3_REPLACE(0x531f90, Sound_RegisterEos);

// 0x531fd0. Music fade step (mode 1 out, 2 in) unless paused (0xccb9f4):
// the target volume 0x624d60 starts at the music's volume times 0x5dd24c /
// the option +0x1146fe times 0x5dd250; at the end the music stops (out) or
// keeps the option volume, and the target resets (-1, 0x624d58 set).
void __stdcall Music_FadeStep(int32_t mode) {
    if (hook::Global<int32_t>(0xccb9f4)) return;
    ++hook::Global<int32_t>(0xccb9f0);
    uint8_t* mus = hook::Global<uint8_t*>(0xccba00);
    uint8_t* eng = hook::Global<uint8_t*>(0x6d4024);
    auto finish = [&](double vol) {
        Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, (float)vol);
        hook::Global<uint32_t>(0x624d60) = 0xbf800000;
        hook::Global<int32_t>(0x624d58) = 1;
    };
    if (mode == 2) {
        const double base = F<float>(mus, 0x104);
        if (Fl(0x5c9b08) == (double)hook::Global<float>(0x624d60)) hook::Global<float>(0x624d60) = (float)((double)F<float>(eng, 0x1146fe) * D(0x5dd250));
        const float v = (float)((double)hook::Global<float>(0x624d60) + base);
        const double lim = (double)F<float>(hook::Global<uint8_t*>(0x6d4024), 0x1146fe) * D(0x5c9c80);
        if ((double)v < lim) Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, v);
        else finish(lim);
    } else if (mode == 1) {
        const double base = F<float>(mus, 0x104);
        if (Fl(0x5c9b08) == (double)hook::Global<float>(0x624d60)) hook::Global<float>(0x624d60) = (float)(Fl(0x5dd24c) * base);
        if ((double)hook::Global<float>(0x624d60) < base) {
            Music_SetVolume(hook::Global<uint8_t*>(0xccba00), 0, (float)(base - (double)hook::Global<float>(0x624d60)));
        } else {
            if (Music_IsPlaying(hook::Global<uint8_t*>(0xccba00)) == 1 || Music_IsPaused(hook::Global<uint8_t*>(0xccba00)) == 1)
                Music_Stop(hook::Global<uint8_t*>(0xccba00), 0, 1);
            finish((double)F<float>(hook::Global<uint8_t*>(0x6d4024), 0x1146fe) * D(0x5c9c80));
        }
    }
    --hook::Global<int32_t>(0xccb9f0);
}
RT3_REPLACE(0x531fd0, Music_FadeStep);

}  // namespace rt3
