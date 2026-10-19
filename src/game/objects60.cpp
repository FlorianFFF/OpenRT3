// Functions across blocks 0x4e..0x50 (batch 63). See objects.cpp for conventions.
#include "hook/hook.h"
#include "hook/keepregs.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

// 0x4ecc50. Lobby entries (9-byte, from +0x31b, count +0x3ae) with both flags clear.
int32_t __fastcall Lobby_CountFreeEntries(uint8_t* l) {
    if (!l) l = hook::Global<uint8_t*>(0x6d1270);
    if (!l) return 0;
    int32_t n = 0;
    for (int32_t i = 0; i < F<int32_t>(l, 0x3ae); ++i)
        if (!l[0x31b + i * 9] && !l[0x31d + i * 9]) ++n;
    return n;
}
RT3_REPLACE(0x4ecc50, Lobby_CountFreeEntries);

// 0x4ecc90. Sets the lobby mode 0x6d1288 for kinds 5, 6 and 8.
void __fastcall Lobby_SetMode(int32_t kind, int, int32_t, int32_t) {
    if (kind == 5) hook::Global<int32_t>(0x6d1288) = 5;
    else if (kind == 6) hook::Global<int32_t>(0x6d1288) = 6;
    else if (kind == 8) hook::Global<int32_t>(0x6d1288) = 8;
}
RT3_REPLACE(0x4ecc90, Lobby_SetMode);

// 0x4ed800. Clears the network session object; returns it.
uint8_t* __fastcall Session_Init(uint8_t* s) {
    static const uint32_t kZero[] = { 0, 4, 8, 0xc, 0x8f18, 0x8f20, 0x8f24, 0x8f28, 0x8f2c, 0x8f30, 0x8f3c,
                                      0x8f40, 0x8f44, 0x9050, 0x9054, 0x985c };
    for (uint32_t off : kZero) F<int32_t>(s, off) = 0;
    F<int32_t>(s, 0x9860) = -1;
    F<int32_t>(s, 0x9878) = 0;
    F<int32_t>(s, 0x9884) = 0;
    return s;
}
RT3_REPLACE(0x4ed800, Session_Init);

int32_t Global6d12bcSet() { return hook::Global<void*>(0x6d12bc) ? 1 : 0; }
RT3_REPLACE(0x4f2e80, Global6d12bcSet);
void SetGlobal6d12cc() { hook::Global<int32_t>(0x6d12cc) = 1; }
RT3_REPLACE(0x4f2e90, SetGlobal6d12cc);

int32_t __fastcall Screen_IsMode5_2(uint8_t* s) { return F<int32_t>(s, 0x90) == 5 && F<int32_t>(s, 0x94) == 2 ? 1 : 0; }
RT3_REPLACE(0x4f5ea0, Screen_IsMode5_2);

// 0x4f6930. Value of the overlay grid (+0x114, +0xfc x +0x100) at map position (x, y); 0 outside.
int32_t __fastcall Overlay_ValueAt(uint8_t* o, int, float x, float y) {
    uint8_t* map = hook::Global<uint8_t*>(0x62c120);
    float w = (float)F<int32_t>(map, 0x2155);
    int32_t cols = F<int32_t>(o, 0xfc);
    float fc = (float)cols;
    int32_t ix = (int32_t)((double)x / w * fc);
    float h = (float)F<int32_t>(map, 0x2159);
    int32_t rows = F<int32_t>(o, 0x100);
    float fr = (float)rows;
    int32_t iy = (int32_t)((double)y / h * fr);
    if (ix < 0 || ix >= cols || iy < 0 || iy >= rows) return 0;
    return F<int32_t*>(o, 0x114)[cols * iy + ix];
}
RT3_REPLACE(0x4f6930, Overlay_ValueAt);

int32_t Global6d1350Set() { return hook::Global<void*>(0x6d1350) ? 1 : 0; }
RT3_REPLACE(0x4fe120, Global6d1350Set);
void SetGlobal6d1360() { hook::Global<int32_t>(0x6d1360) = 1; }
RT3_REPLACE(0x4fe130, SetGlobal6d1360);

// 0x507980. Whether v is one of the special command codes -11..-17, -20..-22.
static int32_t __fastcall IsSpecialCommand_Impl(int32_t v) {
    return (v <= -11 && v >= -17) || (v <= -20 && v >= -22) ? 1 : 0;
}
// Callers (0x508e07, 0x508c24) keep using ECX/EDX after the call.
__declspec(naked) int32_t IsSpecialCommand() { RT3_KEEP_ECX_EDX(IsSpecialCommand_Impl, 0) }
RT3_REPLACE(0x507980, IsSpecialCommand);

} // namespace rt3
