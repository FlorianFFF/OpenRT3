// Model constructors / terrain layer accessors in blocks 0x40..0x53 (batch 84). See objects.cpp for conventions.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint8_t* __fastcall ModelObj_Construct(uint8_t* o);
uint8_t* __fastcall PartModel_Construct(uint8_t* o);
uint32_t __fastcall Render_Get1554(uint8_t* r);
void ResetPair5ee4fc();
void __fastcall Map_ClearFogMode(uint8_t* m);

uint8_t* __fastcall ModelObj40c950_Construct(uint8_t* o) {
    ModelObj_Construct(o);
    F<uint32_t>(o, 0) = 0x5c8c50;
    return o;
}
RT3_REPLACE(0x40c950, ModelObj40c950_Construct);
uint8_t* __fastcall PartModel41aae0_Construct(uint8_t* o) {
    PartModel_Construct(o);
    F<uint32_t>(o, 0) = 0x5c9748;
    return o;
}
RT3_REPLACE(0x41aae0, PartModel41aae0_Construct);

// 0x448d40. Paints terrain vertex (x, y) with the current brush (+0x1554) in layer
// +0x1655, clears the low 3 bits of its +0x165d record and notifies the main camera.
uint32_t __fastcall Terrain_PaintVertex(uint8_t* r, int, int32_t x, int32_t y) {
    uint8_t v = (uint8_t)Render_Get1554(r);
    int32_t i = F<int32_t>(r, 0x15dd) * y + x;
    F<uint8_t*>(r, 0x1655)[i] = v;
    F<uint8_t*>(r, 0x165d)[(F<int32_t>(r, 0x15dd) * y + x) * 3] &= 0xf8;
    void* cam = hook::Global<void*>(0x62be68);
    return ((uint32_t(__thiscall*)(void*))(*(void***)cam)[0])(cam);
}
RT3_REPLACE(0x448d40, Terrain_PaintVertex);

// 0x504a90. Leaves a tool mode: resets 0x5ee4fc/0x5ee500, drops this tool's user
// count (+0x7c, 0x62be84) and clears fog mode 1 unless a sub-mode (+0x2175) other
// than 0x15 is active.
void __fastcall Tool_Leave(uint8_t* t) {
    ResetPair5ee4fc();
    if (F<int32_t>(t, 0x7c)) {
        F<int32_t>(t, 0x7c) = 0;
        hook::Global<int32_t>(0x62be84)--;
    }
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    if (F<int32_t>(map, 0x2171) != 1) return;
    int32_t sub = F<int32_t>(map, 0x2175);
    if (sub && sub != 0x15) return;
    Map_ClearFogMode(map);
}
RT3_REPLACE(0x504a90, Tool_Leave);

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

} // namespace rt3
