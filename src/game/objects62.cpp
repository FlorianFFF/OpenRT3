// Functions across blocks 0x45..0x51 (batch 65). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_trains = hook::Global<Collection*>(0x6cfcbc);

void __fastcall BlockIter_MarkSlot(uint8_t* o, int32_t idx, int32_t flag);
void __fastcall BlockIter_ClearSlot(uint8_t* o, int32_t idx);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
double __fastcall Train_Float131(uint8_t* t, int, int32_t i);
int32_t __fastcall Obj_Get21(void* o);
int32_t ClockMs();

// 0x45b490. Marks (mode 1: unflagged, remembering idx in +0x262) or clears /
// marks slot idx in the object's blocks (+0x256), when +0x75 is set.
void __fastcall Obj_MarkBlockSlot(uint8_t* o, int, int32_t on, int32_t idx, int32_t mode) {
    uint8_t* blocks = F<uint8_t*>(o, 0x256);
    if (!blocks || !F<int32_t>(o, 0x75)) return;
    if (mode == 1) {
        BlockIter_MarkSlot(blocks, idx, 0);
        F<int32_t>(o, 0x262) = idx;
        return;
    }
    if (!on) BlockIter_ClearSlot(blocks, idx);
    else BlockIter_MarkSlot(blocks, idx, 1);
}
RT3_REPLACE(0x45b490, Obj_MarkBlockSlot);

// 0x491af0. Track collection constructor (vtbl 0x5cfe18, heap records
// 1000 + 500) with a zeroed column array of `columns` entries (+0x8c).
Collection* __fastcall Tracks_Construct(Collection* c, int, int32_t columns, int32_t) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5cfe18;
    Collection_Init(c, 0, 0, 0, 1000, 500, 0, 0, 0);
    static const uint32_t kZero[] = { 0x98, 0x90, 0x94, 0xfe, 0x102, 0xa0, 0xa4, 0xa8, 0xac, 0x9c, 0xb0, 0xb4, 0xb8,
                                      0xbc, 0xc0, 0xc4, 0xc8, 0xcc, 0xd0, 0xd4, 0xe5, 0xe9, 0xed, 0xf1 };
    for (uint32_t off : kZero) F<int32_t>(c, off) = 0;
    F<uint8_t>(c, 0xf5) = 0;
    F<int32_t>(c, 0x106) = 0;
    F<uint8_t>(c, 0x10a) = 0;
    F<int32_t>(c, 0x88) = -1;
    F<uint32_t>(c, 0x206) = 0xbf800000;  // -1.0f
    for (int k = 0; k < 13; ++k) F<uint8_t>(c, 0x10b + k) = 0;
    F<int32_t>(c, 0x8c) = 0;
    uint8_t* cols = (uint8_t*)Malloc(columns * 4);
    F<uint8_t*>(c, 0x8c) = cols;
    for (int32_t k = 0; k < columns * 4; ++k) cols[k] = 0;
    return c;
}
RT3_REPLACE(0x491af0, Tracks_Construct);

// 0x50d0b0. Cell bounds {minX, maxX, minY, maxY} of the company's plain
// track pieces (+0x216 == 0); returns how many there are.
int32_t __stdcall Company_TrackBounds(int32_t* out, int32_t company) {
    Collection* nodes = hook::Global<Collection*>(0x6cfca8);
    float minX = 1000000.0f, maxX = -1000000.0f, minY = 1000000.0f, maxY = -1000000.0f;
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(nodes); ++i) {
        uint8_t* t = (uint8_t*)Collection_Get(nodes, 0, Collection_IdAt(nodes, 0, i, 0));
        if (t[0x216] || F<uint16_t>(t, 0x22e) != company) continue;
        ++n;
        float x, y;
        Obj_CenterCell(t, 0, &x, &y);
        if (minX > x) minX = x;
        if (maxX < x) maxX = x;
        if (minY > y) minY = y;
        if (maxY < y) maxY = y;
    }
    if (n) {
        out[0] = (int32_t)minX;
        out[1] = (int32_t)maxX;
        out[2] = (int32_t)minY;
        out[3] = (int32_t)maxY;
    }
    return n;
}
RT3_REPLACE(0x50d0b0, Company_TrackBounds);

double __fastcall Train_Total131(uint8_t* t) { return Train_Float131(t, 0, 0) + F<float>(t, 0x101); }
RT3_REPLACE(0x5130d0, Train_Total131);

// 0x5130f0..0x5131a0. The train selected in the current screen (0x6d0818 +0x90).
static bool SelectedTrainValid() {
    uint8_t* s = hook::Global<uint8_t*>(0x6d0818);
    return s && F<int32_t>(s, 0x90) > 0 && Collection_IsValid(g_trains, 0, F<uint32_t>(s, 0x90));
}
void* SelectedTrain() {
    if (!SelectedTrainValid()) return nullptr;
    return Collection_Get(g_trains, 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
}
RT3_REPLACE(0x5130f0, SelectedTrain);
int32_t SelectedTrainId() { return SelectedTrainValid() ? F<int32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90) : -1; }
RT3_REPLACE(0x513130, SelectedTrainId);
int32_t HasSelectedTrain() { return SelectedTrainValid() ? 1 : 0; }
RT3_REPLACE(0x513170, HasSelectedTrain);
int32_t SelectedTrainIsOwn() {
    if (!SelectedTrainValid() || !SelectedTrainValid()) return 0;
    uint8_t* t = (uint8_t*)Collection_Get(g_trains, 0, F<uint32_t>(hook::Global<uint8_t*>(0x6d0818), 0x90));
    if (!t) return 0;
    return F<int32_t>(t, 0x51) == Obj_Get21(hook::Global<void*>(0x6cec78)) ? 1 : 0;
}
RT3_REPLACE(0x5131a0, SelectedTrainIsOwn);

RT3_NOINLINE int32_t Global6d3b30() { return hook::Global<int32_t>(0x6d3b30); }
RT3_REPLACE(0x5157e0, Global6d3b30);

// 0x5163b0. Index of the tutorial step (0x622b48, 0x40 bytes each) with id v; 0 if none.
int32_t __stdcall Tutorial_IndexOf(float v) {
    int32_t i = 0;
    for (const int32_t* p = (const int32_t*)0x622b48; p < (const int32_t*)0x624308; p += 0x10, ++i)
        if ((float)*p == v) return i;
    return 0;
}
RT3_REPLACE(0x5163b0, Tutorial_IndexOf);

// 0x5164f0 / 0x516540. Blink phase (350 ms) while the current tutorial step
// (0x6d3b50) is 0x348 / 0x352 or 0x357.
static int32_t TutorialBlink(bool match) {
    if (!hook::Global<int32_t>(0x6d3b4c) || !match) return 0;
    return (ClockMs() / 350) & 1 ? 1 : 0;
}
static inline int32_t TutorialStep() { return ((const int32_t*)0x622b48)[hook::Global<int32_t>(0x6d3b50) * 0x10]; }
int32_t Tutorial_Blink348() { return TutorialBlink(hook::Global<int32_t>(0x6d3b4c) && TutorialStep() == 0x348); }
RT3_REPLACE(0x5164f0, Tutorial_Blink348);
int32_t Tutorial_Blink352() {
    if (!hook::Global<int32_t>(0x6d3b4c)) return 0;
    int32_t s = TutorialStep();
    return TutorialBlink(s == 0x352 || s == 0x357);
}
RT3_REPLACE(0x516540, Tutorial_Blink352);

} // namespace rt3
