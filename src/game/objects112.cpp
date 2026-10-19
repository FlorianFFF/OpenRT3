// MP version / thread dtors / key forwarders / overlay state / visit grid in blocks 0x46..0x56 (batch 115). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t Net_LockQueue();
void Net_UnlockQueue();
float __fastcall Game_VersionNumber();
void __fastcall Thread_Destruct(uint8_t* t);
void __fastcall KeyTable_Set(uint8_t* t, int, uint8_t k);
void __fastcall KeyTable_Clear(uint8_t* t, int, uint8_t k);
uint8_t __fastcall KeyTable_Get(uint8_t* t, int, uint8_t k);
void __fastcall Render_ScreenSpaceAll(uint8_t* r);
void __fastcall Render_CullNone(uint8_t* r);
int32_t __fastcall Render_LightingOff(uint8_t* r);
void __fastcall Render_FogOff(uint8_t* r);
void __fastcall Render_SetDefaultMaterial(uint8_t* r, int, uint8_t shiny);
int32_t __fastcall Render_BlendDefault(uint8_t* r);
void __fastcall Render_AlphaTestOn(uint8_t* r, int, uint32_t ref);
int32_t __fastcall Render_SetZEnable(uint8_t* r, int, uint32_t v);
void __fastcall Render_BindVb2647(uint8_t* r);

// 0x46a4b0. The common game version (lowest non-zero version among the queued
// session players, else our own), as an integer x 0x5c85d8; cached in 0x6cd96c.
float __fastcall Net_CommonVersion() {
    if (int32_t v = hook::Global<int32_t>(0x6cd96c)) return (float)v;
    float best = 99999.0f;
    for (uint8_t* n = *(uint8_t**)Net_LockQueue(); n; n = *(uint8_t**)n) {
        float v = F<float>(n, 0x268c);
        if (v < best && !(*(const float*)0x5c8598 == v)) best = v;
    }
    Net_UnlockQueue();
    if (best == *(const float*)0x5c8598 || *(uint32_t*)&best == 0x47c34f80) best = Game_VersionNumber();
    int32_t r;
    __asm {
        fld best
        fadd qword ptr ds:[0x5c88a0]
        fmul qword ptr ds:[0x5c85d8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    hook::Global<int32_t>(0x6cd96c) = r;
    return (float)r;
}
RT3_REPLACE(0x46a4b0, Net_CommonVersion);

// 0x4ed7e0 / 0x4ed870. Scalar deleting destructors of two worker thread classes.
uint8_t* __fastcall WorkerA_Delete(uint8_t* t, int, uint32_t flags) {
    Thread_Destruct(t);
    if (flags & 1) ((void(__cdecl*)(void*))0x53b080)(t);
    return t;
}
RT3_REPLACE(0x4ed7e0, WorkerA_Delete);
uint8_t* __fastcall WorkerB_Delete(uint8_t* t, int, uint32_t flags) {
    Thread_Destruct(t);
    if (flags & 1) ((void(__cdecl*)(void*))0x53b080)(t);
    return t;
}
RT3_REPLACE(0x4ed870, WorkerB_Delete);

// 0x51f0c0..0x51f0e0. Engine key table (+0x24) forwarders.
void __fastcall Engine_KeyDown(uint8_t* e, int, uint8_t k) { KeyTable_Set(F<uint8_t*>(e, 0x24), 0, k); }
RT3_REPLACE(0x51f0c0, Engine_KeyDown);
void __fastcall Engine_KeyUp(uint8_t* e, int, uint8_t k) { KeyTable_Clear(F<uint8_t*>(e, 0x24), 0, k); }
RT3_REPLACE(0x51f0d0, Engine_KeyUp);
uint8_t __fastcall Engine_KeyState(uint8_t* e, int, uint8_t k) { return KeyTable_Get(F<uint8_t*>(e, 0x24), 0, k); }
RT3_REPLACE(0x51f0e0, Engine_KeyState);

// 0x564c10. Render state for 2D overlays.
void __fastcall Scene_BeginOverlay(uint8_t* s) {
    Render_ScreenSpaceAll(F<uint8_t*>(s, 0x2d));
    Render_CullNone(F<uint8_t*>(s, 0x2d));
    Render_LightingOff(F<uint8_t*>(s, 0x2d));
    Render_FogOff(F<uint8_t*>(s, 0x2d));
    Render_SetDefaultMaterial(F<uint8_t*>(s, 0x2d), 0, 0);
    Render_BlendDefault(F<uint8_t*>(s, 0x2d));
    Render_AlphaTestOn(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_SetZEnable(F<uint8_t*>(s, 0x2d), 0, 1);
    Render_BindVb2647(F<uint8_t*>(s, 0x2d));
}
RT3_REPLACE(0x564c10, Scene_BeginOverlay);

// 0x565370. Visit grid for n cells per side: rounded up to a power of two (+0x1f418
// = log2, +0x3e833 = size), (size + 1)^2 flags (+0x1f414), 0x144 stamps (+0x1f40c, row stride 17).
uint8_t* __fastcall VisitGrid_Init(uint8_t* g, int, int32_t n) {
    F<int32_t>(g, 0x1f418) = n;
    g[0x3e832] = 0;
    F<int32_t>(g, 4) = 0;
    F<int32_t>(g, 0x1f41c) = 0;
    int32_t bits = 0;
    if (n - 1 != 0) {
        int32_t v = F<int32_t>(g, 0x1f418) - 1;
        do {
            ++bits;
        } while ((v >> (bits & 31)) != 0);
    }
    int32_t size = 1 << bits;
    F<int32_t>(g, 0x1f418) = bits;
    uint32_t cells = (uint32_t)((size + 1) * (size + 1));
    F<int32_t>(g, 0x3e833) = size;
    F<uint8_t*>(g, 0x1f414) = (uint8_t*)Malloc(cells);
    F<int32_t>(g, 0x1f408) = 1;
    uint32_t* st = (uint32_t*)Malloc(0x510);
    F<uint32_t*>(g, 0x1f40c) = st;
    for (int k = 0; k < 0x144; ++k) st[k] = 0;
    if (uint8_t* fl = F<uint8_t*>(g, 0x1f414)) {
        g[0x3e832] = 1;
        for (uint32_t k = 0; k < cells; ++k) fl[k] = 0;
    }
    F<uint32_t*>(g, 0) = F<uint32_t*>(g, 0x1f40c);
    F<int32_t>(g, 0x3e83b) = 1 << F<int32_t>(g, 0x1f418);
    return g;
}
RT3_REPLACE(0x565370, VisitGrid_Init);

void __fastcall VisitGrid_Free(uint8_t* g) {
    if (!g[0x3e832]) return;
    Free(F<void*>(g, 0x1f414));
    Free(F<void*>(g, 0x1f40c));
}
RT3_REPLACE(0x565440, VisitGrid_Free);

// 0x565470. Marks block (x, y) visited this pass (+0x1f408) and appends it to the
// output list (+0x3e820) once.
void __fastcall VisitGrid_Visit(uint8_t* g, int, int16_t x, int16_t y) {
    uint32_t* cell = F<uint32_t*>(g, 0x1f40c) + (y * 0x11 + x);
    uint32_t stamp = F<uint32_t>(g, 0x1f408);
    if (*cell == stamp) return;
    *cell = stamp;
    *F<int16_t*>(g, 0x3e820) = x;
    F<int16_t*>(g, 0x3e820) += 1;
    *F<int16_t*>(g, 0x3e820) = y;
    F<int16_t*>(g, 0x3e820) += 1;
    F<int32_t>(g, 0x1f41c)++;
}
RT3_REPLACE(0x565470, VisitGrid_Visit);

int32_t __fastcall VisitGrid_Log2(uint8_t* g) { return F<int32_t>(g, 0x1f418); }
RT3_REPLACE(0x5654f0, VisitGrid_Log2);
uint8_t* __fastcall VisitGrid_Flags(uint8_t* g) { return F<uint8_t*>(g, 0x1f414); }
RT3_REPLACE(0x565500, VisitGrid_Flags);

void __fastcall Nop565c10() {}
RT3_REPLACE(0x565c10, Nop565c10);
void __fastcall Obj_GetVec0(uint32_t* o, int, uint32_t* out) {
    out[0] = o[0];
    out[1] = o[1];
    out[2] = o[2];
}
RT3_REPLACE(0x565c20, Obj_GetVec0);
void __fastcall Obj_SetPair10(uint8_t* o, int, uint32_t a, uint32_t b, uint8_t c) {
    F<uint32_t>(o, 0x10) = a;
    F<uint32_t>(o, 0x14) = b;
    o[0xd8] = c;
}
RT3_REPLACE(0x565c40, Obj_SetPair10);
uint8_t __fastcall Obj_GetPair10(uint8_t* o, int, uint32_t* a, uint32_t* b) {
    if (a) *a = F<uint32_t>(o, 0x10);
    if (b) *b = F<uint32_t>(o, 0x14);
    return o[0xd8];
}
RT3_REPLACE(0x565c60, Obj_GetPair10);

} // namespace rt3
