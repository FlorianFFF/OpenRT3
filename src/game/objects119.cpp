// Sound slots / terrain flag snapshot / camera dtor / water marks / cell digits / channel teardown in blocks 0x40..0x55 (batch 122). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Sound_LookupSample(uint8_t* s, int, uint32_t key, int32_t kind);
void __fastcall Sound_Stop(uint8_t* s, int, uint32_t key, int32_t kind);
uint8_t __fastcall Terrain_GetFlag(uint8_t* t, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellCount(uint8_t* t, int, int32_t x, int32_t y);
uint32_t __fastcall Terrain_CellByte(uint8_t* t, int, int32_t x, int32_t y);
void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Terrain_DecCellCount(uint8_t* t, int, int32_t x, int32_t y, int32_t n);
int32_t __fastcall Terrain_KindIs1Or4(uint8_t* r, int, int32_t x, int32_t y);
int32_t __fastcall Terrain_KindIs3Or5(uint8_t* r, int, int32_t x, int32_t y);
void __fastcall Camera_Detach(void* c);
void __fastcall Camera_Destruct(uint32_t* c);
void __fastcall Terrain_Remove1679(uint8_t* r, int, PtrListNode* n, int32_t x, int32_t y);
void __fastcall Widget_RemoveListNode(uint8_t* w, int, PtrListNode* n);
void* __fastcall Engine_Renderer(void* e);

static inline uint8_t* SoundSys() { return hook::Global<uint8_t*>(0x6d402c); }
static inline uint8_t* Map() { return hook::Global<uint8_t*>(0x62c120); }

// 0x40b7d0. Stops and clears the eight ambient sound slots (0x5ed238, 0x20 bytes).
void __fastcall Ambient_StopSlots() {
    for (uint8_t* s = (uint8_t*)0x5ed238; s < (uint8_t*)0x5ed338; s += 0x20) {
        if (uint32_t key = F<uint32_t>(s, 0)) {
            int32_t smp = Sound_LookupSample(SoundSys(), 0, key, 1);
            F<int32_t>(s, 0x14) = smp;
            if (smp) Sound_Stop(SoundSys(), 0, F<uint32_t>(s, 0), 1);
        }
        F<uint32_t>(s, 0) = 0;
        F<int32_t>(s, 0x14) = 0;
    }
    hook::Global<int32_t>(0x62b248) = 0;
}
RT3_REPLACE(0x40b7d0, Ambient_StopSlots);

// 0x414e10. Snapshots the terrain flags (low nibble | counter << 4) and cell bytes
// of the rectangle 0x62b7a8..0x62b7b4 into 0x62b96c / 0x62b9d1 (count 0x62ba36).
void __fastcall Terrain_SnapshotFlags() {
    uint8_t* flags = (uint8_t*)0x62b96c;
    uint8_t* bytes = (uint8_t*)0x62b9d1;
    int32_t& n = hook::Global<int32_t>(0x62ba36);
    n = 0;
    for (int32_t y = hook::Global<int32_t>(0x62b7b0); y <= hook::Global<int32_t>(0x62b7b4); ++y)
        for (int32_t x = hook::Global<int32_t>(0x62b7a8); x <= hook::Global<int32_t>(0x62b7ac); ++x, ++flags, ++bytes) {
            if (x < 0 || x >= F<int32_t>(Map(), 0x15d9) - 1 || y < 0 || y >= F<int32_t>(Map(), 0x15d5) - 1) {
                ++n;
                *flags = 0;
                *bytes = 0;
                continue;
            }
            *flags = Terrain_GetFlag(Map(), 0, x, y);
            *flags |= (uint8_t)(Terrain_CellCount(Map(), 0, x, y) << 4);
            *bytes = (uint8_t)Terrain_CellByte(Map(), 0, x, y);
            ++n;
        }
}
RT3_REPLACE(0x414e10, Terrain_SnapshotFlags);

// 0x43a560. Follow camera destructor: stops its sound (+0x3b2), then the camera's.
void __fastcall FollowCamera_Destruct(uint32_t* c) {
    c[0] = 0x5ca078;
    if (uint32_t key = F<uint32_t>(c, 0x3b2)) Sound_Stop(SoundSys(), 0, key, 0);
    F<uint32_t>(c, 0x3b2) = 0;
    Camera_Detach(c);
    Camera_Destruct(c);
}
RT3_REPLACE(0x43a560, FollowCamera_Destruct);

// 0x448c20 / 0x448cb0. Marks a land vertex as flooded (kinds 1/4) or drained (3/5):
// clears its layer +0x1655, sets the kind bits, drops its counters and notifies the camera.
static inline void MarkWater(uint8_t* t, int32_t x, int32_t y, uint32_t keep2, uint32_t set2, uint32_t keep, uint32_t set) {
    F<uint8_t*>(t, 0x1655)[F<int32_t>(t, 0x15dd) * y + x] = 0;
    uint16_t* rec = (uint16_t*)(F<uint8_t*>(t, 0x165d) + (F<int32_t>(t, 0x15dd) * y + x) * 3);
    uint32_t w = *rec;
    w = (w & 7) == 2 ? ((w & keep2) | set2) : ((w & keep) | set);
    *rec = (uint16_t)w;
    Terrain_DecCellCount(t, 0, x, y, 6);
    void* cam = hook::Global<void*>(0x62be68);
    ((void(__thiscall*)(void*))(*(void***)cam)[0])(cam);
}
void __fastcall Terrain_MarkFlooded(uint8_t* t, int, int32_t x, int32_t y) {
    if (!Terrain_KindIs1Or4(t, 0, x, y)) MarkWater(t, x, y, 0xfffc, 4, 0xfff9, 1);
}
RT3_REPLACE(0x448c20, Terrain_MarkFlooded);
void __fastcall Terrain_MarkDrained(uint8_t* t, int, int32_t x, int32_t y) {
    if (!Terrain_KindIs3Or5(t, 0, x, y)) MarkWater(t, x, y, 0xfffd, 5, 0xfffb, 3);
}
RT3_REPLACE(0x448cb0, Terrain_MarkDrained);

// 0x4537b0 / 0x4537e0 / 0x453800 / 0x453840. The cell byte holds two base-7 digits.
int32_t __fastcall Map_CellHigh(uint8_t* m, int, int32_t x, int32_t y) {
    return (int32_t)(uint8_t)Terrain_CellByte(m, 0, x, y) / 7;
}
RT3_REPLACE(0x4537b0, Map_CellHigh);
uint32_t __fastcall Map_CellLow(uint8_t* m, int, int32_t x, int32_t y) {
    int32_t v = (uint8_t)Terrain_CellByte(m, 0, x, y);
    return (uint32_t)((v / 7) & 0xffffff00) | (uint32_t)(v % 7);
}
RT3_REPLACE(0x4537e0, Map_CellLow);
void __fastcall Map_SetCellHigh(uint8_t* m, int, int32_t x, int32_t y, uint8_t hi) {
    int32_t v = (uint8_t)Terrain_CellByte(m, 0, x, y);
    Terrain_SetCellByte(m, 0, x, y, (uint8_t)(v % 7 + (uint8_t)(hi * 7)));
}
RT3_REPLACE(0x453800, Map_SetCellHigh);
void __fastcall Map_SetCellLow(uint8_t* m, int, int32_t x, int32_t y, uint8_t lo) {
    int32_t v = (uint8_t)Terrain_CellByte(m, 0, x, y);
    Terrain_SetCellByte(m, 0, x, y, (uint8_t)((uint8_t)(v / 7) * 7 + lo));
}
RT3_REPLACE(0x453840, Map_SetCellLow);

static inline int32_t CellOf(float v) {
    int32_t r;
    __asm {
        fld v
        fmul dword ptr ds:[0x5dcfe8]
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x5540b0. Detaches a sound channel: from its terrain block list, the renderer
// list (+0x13d) and stops its sample.
void __fastcall Channel_Detach(uint8_t* c) {
    if (uint8_t* t = F<uint8_t*>(c, 4)) {
        if (PtrListNode* node = F<PtrListNode*>(c, 0)) {
            int32_t y = CellOf(F<float>(c, 0x11c)) >> 4;
            int32_t x = CellOf(F<float>(c, 0x114)) >> 4;
            Terrain_Remove1679(t, 0, node, x, y);
            F<void*>(c, 4) = nullptr;
        }
    }
    if (PtrListNode* n = F<PtrListNode*>(c, 0x13d))
        Widget_RemoveListNode((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)), 0, n);
    if (uint32_t key = F<uint32_t>(c, 0x110)) {
        Sound_Stop(SoundSys(), 0, key, 1);
        F<uint32_t>(c, 0x110) = 0;
    }
}
RT3_REPLACE(0x5540b0, Channel_Detach);

void __fastcall Channel_StopSample(uint8_t* c) {
    if (uint32_t key = F<uint32_t>(c, 0x110)) Sound_Stop(SoundSys(), 0, key, 1);
}
RT3_REPLACE(0x554210, Channel_StopSample);

// 0x554280. Sets the channel's sample name (+0x10), stopping the current sample.
void __fastcall Channel_SetName(uint8_t* c, int, const char* name) {
    char* d = (char*)c + 0x10;
    while ((*d++ = *name++) != 0) {}
    if (uint32_t key = F<uint32_t>(c, 0x110)) {
        Sound_Stop(SoundSys(), 0, key, 1);
        F<uint32_t>(c, 0x110) = 0;
    }
}
RT3_REPLACE(0x554280, Channel_SetName);

} // namespace rt3
