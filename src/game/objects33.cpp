// Functions across blocks 0x43..0x47 (batch 36). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

// 0x433ea0. Year roll-over of the world stats: this year's 53 cargo values
// (+0xa26) and three totals are added to the running sums and cleared.
void __fastcall World_RollStats(uint8_t* w) {
    for (int32_t i = 0; i < 0x35; ++i) {
        float t = F<float>(w, 0xa26 + i * 4);
        F<float>(w, 0xa26 + i * 4) = 0.0f;
        F<float>(w, 0xafa + i * 4) = (float)((double)t + F<float>(w, 0xafa + i * 4));
    }
    float t = F<float>(w, 0xbce);
    F<float>(w, 0xbce) = 0.0f;
    F<float>(w, 0xbd2) = (float)((double)t + F<float>(w, 0xbd2));
    t = F<float>(w, 0xbd6);
    F<float>(w, 0xbd6) = 0.0f;
    F<float>(w, 0xbda) = (float)((double)F<float>(w, 0xbda) + t);
    t = F<float>(w, 0x4caa);
    F<float>(w, 0x4caa) = 0.0f;
    F<float>(w, 0x4ca6) = (float)((double)F<float>(w, 0x4ca6) + t);
}
RT3_REPLACE(0x433ea0, World_RollStats);

// 0x474610. As 0x40b6a0 with tags 0x61a9/0x61aa/0x61ab.
void __fastcall Collection_SaveWithRecords61a9(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x61a9);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x61aa);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, 0x61ab);
}
RT3_REPLACE(0x474610, Collection_SaveWithRecords61a9);

// 0x474db0. Frees the buffers 0x6cea54/0x6cea5c/0x6cea60 and the map 0x6cea58.
void FreeBuffers6cea54() {
    static const uint32_t kBuffers[] = { 0x6cea54, 0x6cea5c, 0x6cea60 };
    for (uint32_t va : kBuffers) {
        void*& p = hook::Global<void*>(va);
        if (p) {
            Free(p);
            p = nullptr;
        }
    }
    void*& m = hook::Global<void*>(0x6cea58);
    if (m) {
        void* p = m;
        ((void(__thiscall*)(void*))0x53dcb0)(p);
        ((void(__cdecl*)(void*))0x53b080)(p);
        m = nullptr;
    }
}
RT3_REPLACE(0x474db0, FreeBuffers6cea54);

// 0x476ab0 / 0x476b30. Field +0x40 / +0x20 of the (*0x6d4020)+0x429b4 entry
// named like this +0xc47. QUIRK: without a match the last entry is used,
// and with no entries this object itself.
static uint8_t* FindNamedEntry(uint8_t* o) {
    auto list = []() { return F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b4); };
    uint8_t* e = o;
    for (int32_t i = 0; i < Collection_Count(list()); ++i) {
        Collection* l = list();
        e = (uint8_t*)Collection_Get(l, 0, Collection_IdAt(l, 0, i, 0));
        if (Stricmp((const char*)e, (const char*)o + 0xc47) == 0) break;
    }
    return e;
}
uint8_t* __fastcall Obj_NamedEntry40(uint8_t* o) { return FindNamedEntry(o) + 0x40; }
RT3_REPLACE(0x476ab0, Obj_NamedEntry40);
uint8_t* __fastcall Obj_NamedEntry20(uint8_t* o) { return FindNamedEntry(o) + 0x20; }
RT3_REPLACE(0x476b30, Obj_NamedEntry20);

// 0x4778c0 / 0x477920. Active (+4) players: ID of the n-th (0-based) / index of `id` among them.
uint32_t __fastcall Table_NthActive(Collection* t, int, int32_t n) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (!F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 4)) continue;
        if (n == 0) return Collection_IdAt(t, 0, i, 0);
        --n;
    }
    return 0;
}
RT3_REPLACE(0x4778c0, Table_NthActive);
int32_t __fastcall Table_ActiveIndexOf(Collection* t, int, uint32_t id) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        if (Collection_IdAt(t, 0, i, 0) == id) return n;
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 4)) ++n;
    }
    return 0;
}
RT3_REPLACE(0x477920, Table_ActiveIndexOf);

// 0x4779c0. Linear map of x from [+0xc3, +0xcb] scaled by +0x13, plus the
// offsets (+0x86)->+5 and +0xb; returned unrounded.
double __fastcall Obj_MapX(uint8_t* o, int, float x) {
    float lo = (float)F<int32_t>(o, 0xc3);
    float span = (float)(F<int32_t>(o, 0xcb) - F<int32_t>(o, 0xc3));
    double v = ((double)x - lo) * F<float>(o, 0x13);
    v = v / span;
    v = v + F<float>(F<uint8_t*>(o, 0x86), 5);
    return v + F<float>(o, 0xb);
}
RT3_REPLACE(0x4779c0, Obj_MapX);

} // namespace rt3
