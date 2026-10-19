// The engine/display object ("PopTop 3D Engine", global 0x6d4024, ~1.13 MB).
// Methods 0x51eff0..0x51ffb0. Layout is accessed by offset; known fields:
//   +0x000 HWND                    +0x004 HINSTANCE
//   +0x008 IDirect3DDevice8*       +0x00c render target / surface owner
//   +0x018 renderer object         +0x01c second subsystem (nonzero = 3D active)
//   +0x020 input/cursor object     +0x030 display mode count
//   +0x034 display modes (21-byte entries: w, h, bpp, refresh, ..., flags @0x14)
//   +0x038/+0x03c desktop width/height     +0x056 "repaint" byte
//   +0x058 render queue count      +0x05c render queue (12000 x 94-byte items)
//   +0x11369c sorted-queue flag    +0x11369d sorted queue (PtrList*)
//   +0x1136a1 default queue        +0x1136a5 texture queue (PtrList*)
//   +0x1136a9 texture memory at start  +0x1136ad texture memory now
//   +0x1136b1 sin table [360]      +0x113c51 cos table [360]
//   +0x11421e current mode index   +0x114226 fullscreen byte
//   +0x114227 current layer (low 4 bits copied into queue items)
//   +0x114282 average frame ms     +0x114286 last tick   +0x11428a frame ms ring [256]
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <windows.h>
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* e, uint32_t off) { return *(T*)((uint8_t*)e + off); }

static int32_t& g_asianText = hook::Global<int32_t>(0x6d4034); // DBCS language: text goes to the sorted queue
static int32_t& g_frameCounter = hook::Global<int32_t>(0x6d403c);
static void*& g_engine = hook::Global<void*>(0x6d4024);
static const int32_t* const kTexMemOverride = (const int32_t*)0x624c34;
// Through the exe's import slot, like the original (lets tests fake the clock).
static inline DWORD Ticks() { return ((DWORD(WINAPI*)())*(void**)0x5c8068)(); }

void __fastcall Engine_SetC(void* e, int, int32_t v) { F<int32_t>(e, 0xc) = v; }
RT3_REPLACE(0x51eff0, Engine_SetC);

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // 0x54e3a0 (original for now)

ATOM __fastcall Engine_RegisterWindowClass(HINSTANCE inst) {
    WNDCLASSEXA wc;
    wc.cbSize = sizeof wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)0x54e3a0;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = inst;
    wc.hIcon = LoadIconA(inst, (LPCSTR)0x7f00);
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "World";
    wc.hIconSm = nullptr;
    return RegisterClassExA(&wc);
}
RT3_REPLACE(0x51f000, Engine_RegisterWindowClass);

void* __fastcall Engine_Renderer(void* e) { return F<void*>(e, 0x18); }
RT3_REPLACE(0x51f070, Engine_Renderer);
void __fastcall Engine_SetRenderer(void* e, int, void* v) { F<void*>(e, 0x18) = v; }
RT3_REPLACE(0x51f080, Engine_SetRenderer);
void* __fastcall Engine_Get1C(void* e) { return F<void*>(e, 0x1c); }
RT3_REPLACE(0x51f090, Engine_Get1C);
void __fastcall Engine_Set1C(void* e, int, void* v) { F<void*>(e, 0x1c) = v; }
RT3_REPLACE(0x51f0a0, Engine_Set1C);
void* __fastcall Engine_Input(void* e) { return F<void*>(e, 0x20); }
RT3_REPLACE(0x51f0b0, Engine_Input);
HWND __fastcall Engine_Window(void* e) { return F<HWND>(e, 0); }
RT3_REPLACE(0x51f0f0, Engine_Window);

// Current display mode. Bpp is reported as 0 in windowed-with-desktop mode.
void __fastcall Engine_CurrentMode(void* e, int, int32_t* w, int32_t* h, int32_t* bpp, int32_t* refresh, uint32_t* flags) {
    uint8_t* m = F<uint8_t*>(e, 0x34) + F<int32_t>(e, 0x11421e) * 0x15;
    *w = *(int32_t*)m;
    *h = *(int32_t*)(m + 4);
    if (bpp) *bpp = *(int32_t*)(m + 8);
    if (refresh) *refresh = F<int32_t>(e, 0x11470e) == 0 ? *(int32_t*)(m + 0xc) : 0;
    if (flags) *flags = m[0x14];
}
RT3_REPLACE(0x51f100, Engine_CurrentMode);

uint8_t __fastcall Engine_IsFullscreen(void* e) { return F<uint8_t>(e, 0x114226); }
RT3_REPLACE(0x51f1a0, Engine_IsFullscreen);
int32_t __fastcall Engine_Get1146f6(void* e) { return F<int32_t>(e, 0x1146f6); }
RT3_REPLACE(0x51f1b0, Engine_Get1146f6);
void __fastcall Engine_Set1146f6(void* e, int, int32_t v) { F<int32_t>(e, 0x1146f6) = v; }
RT3_REPLACE(0x51f1c0, Engine_Set1146f6);

// Unlinks every texture that has queued items from the texture queue.
void __fastcall Engine_ResetTextureQueue(void* e) {
    PtrList* textures = F<PtrList*>(e, 0x1136a5);
    PtrList_Rewind(textures);
    for (uint8_t* tex = (uint8_t*)PtrList_Next(textures); tex; tex = (uint8_t*)PtrList_Next(textures)) {
        if (PtrList_TailValue(*(PtrList**)(tex + 0xe4))) {
            *(void**)(tex + 0xe8) = nullptr;
            PtrList_Append(*(PtrList**)(tex + 0xe4), 0, nullptr);
        }
    }
}
RT3_REPLACE(0x51f1d0, Engine_ResetTextureQueue);

// Routes a queue item: textured items go to their texture's list (registering
// the texture once), untextured ones to the default queue; in DBCS mode with
// sorting on, everything goes to the sorted queue.
void __fastcall Engine_Enqueue(void* e, int, uint8_t* texture, void* item) {
    if (g_asianText) {
        if (F<uint8_t>(e, 0x11369c)) PtrList_Prepend(F<PtrList*>(e, 0x11369d), 0, item);
        else PtrList_Append(F<PtrList*>(e, 0x11369d), 0, item);
        return;
    }
    if (texture) {
        if (*(void**)(texture + 0xe8) == nullptr)
            *(PtrListNode**)(texture + 0xe8) = PtrList_Append(F<PtrList*>(e, 0x1136a5), 0, texture);
        PtrList_Append(*(PtrList**)(texture + 0xe4), 0, item);
        return;
    }
    PtrList_Append(F<PtrList*>(e, 0x1136a1), 0, item);
}
RT3_REPLACE(0x51f230, Engine_Enqueue);

static inline uint8_t* NewItem(void* e) {
    int32_t n = F<int32_t>(e, 0x58);
    if (n >= 12000) return nullptr;
    F<int32_t>(e, 0x58) = n + 1;
    return (uint8_t*)e + 0x5c + n * 0x5e;
}
// The original does two 4-bit field updates that together copy the whole byte.
static inline void SetLayer(void* e, uint8_t* item) { item[0x5d] = F<uint8_t>(e, 0x114227); }
static inline void EnqueueUntextured(void* e, uint8_t* item) {
    if (g_asianText && F<uint8_t>(e, 0x11369c)) PtrList_Prepend(F<PtrList*>(e, 0x11369d), 0, item);
    else PtrList_Append(F<PtrList*>(e, g_asianText ? 0x11369d : 0x1136a1), 0, item);
}

// Item type 5 (4 dwords).
void __fastcall Engine_QueueType5(void* e, int, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    *(uint32_t*)(it + 1) = a;
    *(uint32_t*)(it + 5) = b;
    *(uint32_t*)(it + 9) = c;
    it[0] = 5;
    *(uint32_t*)(it + 0xd) = d;
    EnqueueUntextured(e, it);
}
RT3_REPLACE(0x51f2b0, Engine_QueueType5);

// Item type 6 (no data).
void __fastcall Engine_QueueType6(void* e) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    it[0] = 6;
    EnqueueUntextured(e, it);
}
RT3_REPLACE(0x51f320, Engine_QueueType6);

// Item type 4: textured quad (20 dwords; p11 = texture).
void __fastcall Engine_QueueQuad(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                 uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10, uint32_t tex,
                                 uint32_t p12, uint32_t p13, uint32_t p14, uint32_t p15, uint32_t p16,
                                 uint32_t p17, uint32_t p18, uint32_t p19, uint32_t p20) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    *(uint32_t*)(it + 0x01) = p1;  *(uint32_t*)(it + 0x05) = p2;  *(uint32_t*)(it + 0x09) = p3;
    *(uint32_t*)(it + 0x0d) = p4;  *(uint32_t*)(it + 0x11) = p5;  *(uint32_t*)(it + 0x15) = p6;
    *(uint32_t*)(it + 0x19) = p7;  *(uint32_t*)(it + 0x45) = p12; *(uint32_t*)(it + 0x49) = p13;
    *(uint32_t*)(it + 0x4d) = p14; *(uint32_t*)(it + 0x51) = p15; *(uint32_t*)(it + 0x55) = p16;
    *(uint32_t*)(it + 0x59) = p17; *(uint32_t*)(it + 0x29) = p18; *(uint32_t*)(it + 0x2d) = p19;
    *(uint32_t*)(it + 0x31) = p20; *(uint32_t*)(it + 0x1d) = p8;  *(uint32_t*)(it + 0x21) = p9;
    *(uint32_t*)(it + 0x25) = p10;
    it[0] = 4;
    SetLayer(e, it);
    *(uint32_t*)(it + 0x41) = tex;
    Engine_Enqueue(e, 0, (uint8_t*)tex, it);
}
RT3_REPLACE(0x51f370, Engine_QueueQuad);

// Item type 1: textured sprite (12 dwords; p7 = texture).
void __fastcall Engine_QueueSprite(void* e, int, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5,
                                   uint32_t p6, uint32_t tex, uint32_t p8, uint32_t p9, uint32_t p10,
                                   uint32_t p11, uint32_t p12) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    *(uint32_t*)(it + 0x01) = p1;  *(uint32_t*)(it + 0x05) = p2;  *(uint32_t*)(it + 0x09) = p3;
    *(uint32_t*)(it + 0x0d) = p4;  *(uint32_t*)(it + 0x19) = p5;  *(uint32_t*)(it + 0x45) = p8;
    *(uint32_t*)(it + 0x49) = p9;  *(uint32_t*)(it + 0x4d) = p10; *(uint32_t*)(it + 0x51) = p11;
    *(uint32_t*)(it + 0x29) = p12; *(uint32_t*)(it + 0x1d) = p6;
    it[0] = 1;
    SetLayer(e, it);
    *(uint32_t*)(it + 0x41) = tex;
    Engine_Enqueue(e, 0, (uint8_t*)tex, it);
}
RT3_REPLACE(0x51f460, Engine_QueueSprite);

void __fastcall Engine_SetCursorObject(void* e, int, uint32_t v) {
    if (F<int32_t>(e, 0xc) != 0) ((void(__fastcall*)(void*, uint32_t))0x547f20)(F<void*>(e, 0xc), v);
    F<uint32_t>(e, 0x1146b9) = v;
}
RT3_REPLACE(0x51f510, Engine_SetCursorObject);

int32_t __fastcall Engine_ModeCount(void* e) { return F<int32_t>(e, 0x30); }
RT3_REPLACE(0x51f820, Engine_ModeCount);

void __fastcall Engine_ModeAt(void* e, int, int32_t i, int32_t* w, int32_t* h, int32_t* bpp) {
    uint8_t* m = F<uint8_t*>(e, 0x34) + i * 0x15;
    *w = *(int32_t*)m;
    *h = *(int32_t*)(m + 4);
    *bpp = *(int32_t*)(m + 8);
}
RT3_REPLACE(0x51f830, Engine_ModeAt);

// Windowed sizes that reach the desktop size are snapped down to the next
// smaller standard size (512x384 .. 1600x1200).
void __fastcall Engine_ClampWindowSize(void* e, int, int32_t* w, int32_t* h) {
    static const int32_t kW[6] = { 512, 640, 800, 1024, 1280, 1600 };
    static const int32_t kH[6] = { 384, 480, 600, 768, 1024, 1200 };
    if (F<uint8_t>(e, 0x114696) != 0) return;
    if (F<int32_t>(e, 0x38) > *w && F<int32_t>(e, 0x3c) > *h) return;
    int i = 0;
    for (; i < 6; ++i) {
        if (*w <= kW[i] && *h <= kH[i]) {
            if (--i < 0) i = 0;
            break;
        }
    }
    if (i > 5) i = 5;
    *w = kW[i];
    *h = kH[i];
}
RT3_REPLACE(0x51f870, Engine_ClampWindowSize);

void __fastcall Engine_ClearRepaint(void* e) { F<uint8_t>(e, 0x56) = 0; }
RT3_REPLACE(0x51f930, Engine_ClearRepaint);
void __fastcall Engine_RequestRepaint(void* e) { if (F<int32_t>(e, 0x1c)) F<uint8_t>(e, 0x56) = 1; }
RT3_REPLACE(0x51f940, Engine_RequestRepaint);
void Engine_Nop() {}
RT3_REPLACE(0x51f950, Engine_Nop);

// IDirect3DDevice8::GetAvailableTextureMem is vtable slot 4.
static inline int32_t AvailableTextureMem(void* dev) {
    return ((int32_t(__stdcall*)(void*))(*(void***)dev)[4])(dev);
}
void __fastcall Engine_SampleStartTextureMem(void* e) {
    if (F<int32_t>(e, 0x1136a9) == 0 && F<void*>(e, 8)) F<int32_t>(e, 0x1136a9) = AvailableTextureMem(F<void*>(e, 8));
}
RT3_REPLACE(0x51f960, Engine_SampleStartTextureMem);
void __fastcall Engine_SampleTextureMem(void* e) {
    if (F<void*>(e, 8)) F<int32_t>(e, 0x1136ad) = AvailableTextureMem(F<void*>(e, 8));
}
RT3_REPLACE(0x51f990, Engine_SampleTextureMem);

void __fastcall Engine_SetFreeTexMemOverride(void* e, int, int32_t v) { F<int32_t>(e, 0x1146ca) = v; }
RT3_REPLACE(0x51f9b0, Engine_SetFreeTexMemOverride);
void __fastcall Engine_SetTotalTexMemOverride(void* e, int, int32_t v) {
    F<int32_t>(e, 0x1146c6) = v;
    if (hook::Global<int32_t>(0x6d401c)) ((void(*)())0x538ec0)();
}
RT3_REPLACE(0x51f9c0, Engine_SetTotalTexMemOverride);
void __fastcall Engine_SetTexMemOverride3(void* e, int, int32_t v) {
    F<int32_t>(e, 0x1146ce) = v;
    if (hook::Global<int32_t>(0x6d401c)) ((void(*)())0x538ec0)();
}
RT3_REPLACE(0x51f9e0, Engine_SetTexMemOverride3);

// Free texture memory, honouring the settings overrides (table 0x624c34).
int32_t __fastcall Engine_FreeTextureMem(void* e) {
    if (F<int32_t>(e, 0x1146ca)) return kTexMemOverride[F<int32_t>(e, 0x1146ca)];
    int32_t v = F<int32_t>(e, 0x1136ad);
    if (F<int32_t>(e, 0x1146c6)) v += kTexMemOverride[F<int32_t>(e, 0x1146c6)] - F<int32_t>(e, 0x1136a9);
    return v < 0 ? 0 : v;
}
RT3_REPLACE(0x51fa00, Engine_FreeTextureMem);

int32_t __fastcall Engine_TotalTextureMem(void* e) {
    if (F<int32_t>(e, 0x1146c6)) return kTexMemOverride[F<int32_t>(e, 0x1146c6)];
    return F<int32_t>(e, 0x1136a9);
}
RT3_REPLACE(0x51fa40, Engine_TotalTextureMem);

int32_t __fastcall Engine_TextureMemLimit(void* e) {
    if (F<int32_t>(e, 0x1146ce)) return kTexMemOverride[F<int32_t>(e, 0x1146ce)];
    int32_t v = F<int32_t>(e, 0x1146c6) ? kTexMemOverride[F<int32_t>(e, 0x1146c6)] : F<int32_t>(e, 0x1136a9);
    return v < 0 ? 0 : v;
}
RT3_REPLACE(0x51fa60, Engine_TextureMemLimit);

// sin/cos per whole degree, float. x87 fsin/fcos as in the original.
void __fastcall Engine_BuildTrigTables(void* e) {
    float* sinT = (float*)((uint8_t*)e + 0x1136b1);
    float* cosT = (float*)((uint8_t*)e + 0x113c51);
    for (int32_t i = 0; i < 360; ++i) {
        float s, c;
        __asm {
            fild  i
            fmul  qword ptr ds:[0x5d0c70]
            fmul  qword ptr ds:[0x5cb4a0]
            fld   st(0)
            fsin
            fstp  s
            fcos
            fstp  c
        }
        sinT[i] = s;
        cosT[i] = c;
    }
}
RT3_REPLACE(0x51faa0, Engine_BuildTrigTables);

void __fastcall Engine_QueueText(void* e, int, uint32_t p1, float x, float y, uint32_t p4, float w, float h,
                                 uint32_t p7, uint32_t p8, uint32_t p9) {
    uint8_t* it = NewItem(e);
    if (!it) return;
    *(float*)(it + 9) = x + w;
    *(float*)(it + 1) = x;
    *(uint32_t*)(it + 0x19) = p4;
    *(float*)(it + 0xd) = y + h;
    *(float*)(it + 5) = y;
    *(uint32_t*)(it + 0x39) = p8;
    *(uint32_t*)(it + 0x1d) = p7;
    *(uint32_t*)(it + 0x35) = p9;
    it[0] = 3;
    *(uint32_t*)(it + 0x29) = 0x3f800000; // 1.0f
    SetLayer(e, it);
    *(char**)(it + 0x3d) = nullptr;
    ((void(__fastcall*)(char**, const char*))0x51d820)((char**)(it + 0x3d), (const char*)p1); // String_Assign
    EnqueueUntextured(e, it);
}
RT3_REPLACE(0x51fc20, Engine_QueueText);

int32_t __fastcall Engine_ModeHasFlags(void* e) { // full 0/1 in EAX
    if (!F<uint8_t>(e, 0x114226)) return 0;
    int32_t w, h, bpp, refresh;
    uint32_t flags;
    Engine_CurrentMode(e, 0, &w, &h, &bpp, &refresh, &flags);
    return flags != 0 ? 1 : 0;
}
RT3_REPLACE(0x51fd00, Engine_ModeHasFlags);

int32_t __fastcall Engine_Get1146ae(void* e) {
    return F<uint8_t>(e, 0x1146ae) != 0 && F<int32_t>(e, 0x11427a) == 0;
}
RT3_REPLACE(0x51fd40, Engine_Get1146ae);
int32_t __fastcall Engine_Get11470e(void* e) { return F<int32_t>(e, 0x11470e); }
RT3_REPLACE(0x51fd60, Engine_Get11470e);

// Frame timing: records this frame's ms in a 256-entry ring and, when enabled,
// averages the most recent frames until 1000 ms worth (constant 0x5c85b0) are
// covered. The sum stays on the x87 stack, so it is a double here.
void __fastcall Engine_UpdateFrameTime(void* e) {
    if (F<DWORD>(e, 0x114286) == 0) F<DWORD>(e, 0x114286) = Ticks();
    int32_t* ring = (int32_t*)((uint8_t*)e + 0x11428a);
    ring[g_frameCounter % 256] = (int32_t)(Ticks() - F<DWORD>(e, 0x114286));
    F<DWORD>(e, 0x114286) = Ticks();
    int32_t c = ++g_frameCounter;
    if (F<int32_t>((uint8_t*)g_engine, 0x114206) == 0) return;
    const float limit = *(const float*)0x5c85b0;
    double sum = *(const float*)0x5c8598;
    int32_t frames = 0;
    c += 0x1fe;
    for (int32_t done = 0; done < 0x100; done += 8, c -= 8) {
        // QUIRK: the frame that crosses the limit is counted, except the very first of a group of 8.
        if ((sum += ring[(c + 1) % 256]) > limit) break;
        int k = 0;
        for (; k < 6; ++k)
            if ((sum += ring[(c - k) % 256]) > limit) break;
        if (k < 6) { frames += k + 2; break; }
        sum += ring[(c - 6) % 256];
        frames += 8;
        if (sum > limit) break;
    }
    F<float>(e, 0x114282) = (float)(sum / frames);
}
RT3_REPLACE(0x51fd70, Engine_UpdateFrameTime);

double __fastcall Engine_AverageFrameMs(void* e) { return F<float>(e, 0x114282); }
RT3_REPLACE(0x51ff80, Engine_AverageFrameMs);

} // namespace rt3
