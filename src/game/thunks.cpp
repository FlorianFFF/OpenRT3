// Compiler-generated jump thunks (a single JMP to the real function). Each one is the
// same function under a second address, so it maps to the existing rewrite.
#include "hook/hook.h"
#include <stdint.h>

namespace rt3 {

int32_t __fastcall RulesDialog_StoreRule(uint8_t* w);
void __fastcall ModelLibrary_Free(uint32_t* o);
void __fastcall Obj_SetRgb(uint8_t* o, int, uint8_t r, uint8_t g, uint8_t b);
void __fastcall SceneObj_SetScale36(uint8_t* o, int, float v);
int32_t __fastcall SceneObj_DebugTooltip(uint8_t* o, int, char* dst, int32_t n, int32_t, int32_t, int32_t);
void __fastcall BitmapFont_Destroy(uint8_t* f);

RT3_REPLACE(0x4d9ec0, RulesDialog_StoreRule);  // -> 0x4d8ea0
RT3_REPLACE(0x556760, ModelLibrary_Free);      // -> 0x556620
RT3_REPLACE(0x48a060, Obj_SetRgb);             // -> 0x53a3a0
RT3_REPLACE(0x455720, SceneObj_SetScale36);    // -> 0x52ea90
RT3_REPLACE(0x455860, SceneObj_DebugTooltip);  // -> 0x530820
RT3_REPLACE(0x53ee60, BitmapFont_Destroy);     // -> 0x53e2e0

} // namespace rt3
