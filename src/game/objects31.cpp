// Functions across blocks 0x40..0x46 (batch 34). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall BlockIter_BeginTest(uint8_t* o, uint32_t mask, int32_t idx);

// Save of a collection whose records save themselves (virtual slot 17), between three tags.
static void SaveWithRecords(Collection* t, Stream s, int32_t recordSize, uint32_t tag) {
    StreamWriteU32(s, tag);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, tag + 1);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, tag + 2);
}
void __fastcall Collection_SaveWithRecords2af9(Collection* t, int, Stream s, int32_t recordSize) { SaveWithRecords(t, s, recordSize, 0x2af9); }
RT3_REPLACE(0x40b6a0, Collection_SaveWithRecords2af9);
void __fastcall Collection_SaveWithRecords5209b(Collection* t, int, Stream s, int32_t recordSize) { SaveWithRecords(t, s, recordSize, 0x5209); }
RT3_REPLACE(0x461650, Collection_SaveWithRecords5209b);

// 0x452ea0. Tracks whether the value at (*0x62be68)+0x20 exceeds 0x5ca1c0
// (+0x21a1); while +0x219d is set, each change adjusts two engine counters.
void __fastcall Obj_UpdateThreshold(uint8_t* o) {
    int32_t on = F<float>(hook::Global<uint8_t*>(0x62be68), 0x20) > *(const float*)0x5ca1c0 ? 1 : 0;
    if (on == F<int32_t>(o, 0x21a1)) return;
    F<int32_t>(o, 0x21a1) = on;
    if (!F<int32_t>(o, 0x219d)) return;
    int32_t d = on ? 1 : -1;
    F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114272) += d;
    F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114266) += d;
}
RT3_REPLACE(0x452ea0, Obj_UpdateThreshold);

// 0x456820. a = a x b (float vectors).
void __fastcall Vec3_CrossInPlace(float* a, int, const float* b) {
    float a0 = a[0], a1 = a[1], a2 = a[2];
    float b0 = b[0], b1 = b[1], b2 = b[2];
    a[0] = (float)((double)b2 * a1 - (double)b1 * a2);
    a[1] = (float)((double)b0 * a2 - (double)b2 * a0);
    a[2] = (float)((double)b1 * a0 - (double)b0 * a1);
}
RT3_REPLACE(0x456820, Vec3_CrossInPlace);

// 0x45b460. Block test (0x45a380) on +0x256 when +0x75 is set; 0 otherwise.
int32_t __fastcall Obj_TestBlock(uint8_t* o, int, uint32_t mask, int32_t idx) {
    uint8_t* blocks = F<uint8_t*>(o, 0x256);
    if (!blocks || !F<int32_t>(o, 0x75)) return 0;
    return BlockIter_BeginTest(blocks, mask, idx);
}
RT3_REPLACE(0x45b460, Obj_TestBlock);

// 0x46d530. Used slots of the table 0x6ce2d8 (10 x 0x5c bytes).
int32_t CountUsedSlots6ce2d8() {
    int32_t n = 0;
    for (int32_t i = 0; i < 10; ++i)
        if (*(const int32_t*)(0x6ce2d8 + i * 0x5c)) ++n;
    return n;
}
RT3_REPLACE(0x46d530, CountUsedSlots6ce2d8);

} // namespace rt3
