// Model part / train naming helpers in blocks 0x45..0x4b (batch 78). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall SceneObj_SetUvTransform(uint8_t* o, int, float u, float v, float su, float sv);
void __fastcall Car_RearPoint(uint8_t* c, int, float* x, float* y);
void* __fastcall Engine_Renderer(void* e);
void __fastcall Scene_SetBrushRadius(uint8_t* s, int, float v);
uint16_t __stdcall Regions_NearestValue(int32_t x, int32_t y);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);

// Model parts: 32 entries of {flag byte, object*} at +0x276 (5 bytes each).
static inline uint8_t* PartObj(uint8_t* o, int32_t i) { return F<uint8_t*>(o, 0x277 + i * 5); }
static inline uint8_t PartFlag(uint8_t* o, int32_t i) { return o[0x276 + i * 5]; }
static inline void Show(uint8_t* p) { ((void(__thiscall*)(void*))(*(void***)p)[0xc / 4])(p); }
static inline void Hide(uint8_t* p) { ((void(__thiscall*)(void*))(*(void***)p)[0x10 / 4])(p); }

// 0x45e8c0. Hides all parts and shows part `sel` (texture reset when flagged).
void __fastcall Model_ShowOnlyPart(uint8_t* o, int, int32_t sel) {
    for (int32_t i = 0; i < 0x20; ++i)
        if (uint8_t* p = PartObj(o, i)) Hide(p);
    if (sel == -1 || sel < 0 || sel >= 0x20) return;
    uint8_t* p = PartObj(o, sel);
    if (!p) return;
    Show(p);
    if (PartFlag(o, sel)) SceneObj_SetUvTransform(PartObj(o, sel), 0, 0.0f, 0.0f, 1.0f, 1.0f);
}
RT3_REPLACE(0x45e8c0, Model_ShowOnlyPart);

// 0x45d030. Shows part `part` with sprite frame `frame` (1-based cell of an
// 8x8 atlas; 0 = whole texture), or hides it for frame -1.
void __fastcall Model_SetPartFrame(uint8_t* o, int, int32_t part, int32_t frame) {
    if (part < 0 || part >= 0x20) return;
    uint8_t* p = PartObj(o, part);
    if (!p) return;
    if (frame == -1) {
        Hide(p);
        return;
    }
    Show(p);
    if (!PartFlag(o, part)) return;
    if (!frame) {
        SceneObj_SetUvTransform(PartObj(o, part), 0, 0.0f, 0.0f, 1.0f, 1.0f);
        return;
    }
    int32_t f = frame - 1;
    const float cells = *(const float*)0x5c8898;
    float u = (float)((double)(float)(f % 8) / cells);
    float v = (float)((double)(float)(f / 8) / cells);
    SceneObj_SetUvTransform(PartObj(o, part), 0, u, v, 0.125f, 0.125f);
    F<int32_t>(PartObj(o, part), 0x3e) = 2;
}
RT3_REPLACE(0x45d030, Model_SetPartFrame);

// 0x4a77d0. Rear point of the train's first car; (10, 10) without cars.
void __fastcall Train_RearPoint(uint8_t* t, int, float* x, float* y) {
    int32_t id = F<int32_t>(t, 0x41);
    if (id != -1) {
        Car_RearPoint((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba84), 0, id), 0, x, y);
        return;
    }
    *y = 10.0f;
    *x = 10.0f;
}
RT3_REPLACE(0x4a77d0, Train_RearPoint);

// 0x4bc210. Selects water level preset idx (+0x78; table 0x621e24) for the renderer.
void __fastcall Obj_SetWaterPreset(uint8_t* o, int, int32_t idx) {
    F<int32_t>(o, 0x78) = idx;
    float v = (float)((double)((const int32_t*)0x621e24)[idx] + *(const double*)0x5c8628);
    Scene_SetBrushRadius((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, v);
}
RT3_REPLACE(0x4bc210, Obj_SetWaterPreset);

// 0x4a90d0. Gives the train a default name "<region> <random suffix>" (+0x69,
// 30 chars) from the region nearest to its rear (unless flag 1 of +0x1ce).
void __fastcall Train_DefaultName(uint8_t* t) {
    t[0x1a1] = 1;
    if (t[0x1ce] & 1) return;
    float x, y;
    int32_t id = F<int32_t>(t, 0x41);
    if (id != -1) {
        Car_RearPoint((uint8_t*)Collection_Get(hook::Global<Collection*>(0x62ba84), 0, id), 0, &x, &y);
    } else {
        x = 10.0f;
        y = 10.0f;
    }
    int32_t yi = (int32_t)y;
    int32_t xi = (int32_t)x;
    uint16_t r = Regions_NearestValue(xi, yi);
    const char* name = (const char*)0x5c87a8;
    if (r) {
        uint8_t* reg = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62bae0), 0, r);
        if (reg) name = (const char*)reg + 0x356;
    }
    const char* suffix = GetString(((const int32_t*)0x621aa8)[Rand(0, 4)]);
    char buf[0x7c];
    FormatText(buf, (const char*)0x5d0070, name, suffix, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    StrCopyN((char*)t + 0x69, buf, 0x1e);
}
RT3_REPLACE(0x4a90d0, Train_DefaultName);

} // namespace rt3
