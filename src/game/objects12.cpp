// Block 0x41xxxx, more functions (batch 15). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_cities     = hook::Global<Collection*>(0x62ba84);
static Collection*& g_trains     = hook::Global<Collection*>(0x6cfcbc);
static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);
static uint8_t*& g_world = hook::Global<uint8_t*>(0x6cec78);

int32_t __fastcall Obj_GetCell(void* o, int, uint16_t* x, uint16_t* y);
void __stdcall RotateNegated(float x, float y, float angle, float* outX, float* outY);
void __fastcall Obj_UnlinkFromNode(void* o);
void __fastcall Node_ClearLinks(void* n, int, int32_t a, int32_t b);

static inline void Virtual(void* o, uint32_t slot) { ((void(__thiscall*)(void*))(*(void***)o)[slot / 4])(o); }

// 0x417840. Cell of record `id` (if it has one) relative to its footprint
// centre, rotated by `angle`. QUIRK: without a cell the original uses
// uninitialised stack (x) and the low word of `id` (y); here x = 0.
void __fastcall Type_RotatedCell(Collection* t, int, uint32_t id, float* outX, float* outY, float angle) {
    uint8_t* r = (uint8_t*)Collection_Get(t, 0, id);
    uint16_t cx = 0, cy = (uint16_t)id;
    Obj_GetCell(r, 0, &cx, &cy);
    const double c48 = *(const double*)0x5c96c8, c2 = *(const double*)0x5c85d0;
    float y = (float)((double)(int16_t)cy / c48 - (double)F<uint8_t>(r, 0xb9) / c2);
    float x = (float)((double)(int16_t)cx / c48 - (double)F<uint8_t>(r, 0xb8) / c2);
    RotateNegated(x, y, angle, outX, outY);
}
RT3_REPLACE(0x417840, Type_RotatedCell);

// 0x41c8c0. Detaches every city in the chain from +0x352 (virtual slot 4 and
// node unlink), clears this train's link in its node (+0x4d), then this's
// virtual slot 4.
void __fastcall Obj_DetachChain(void* o) {
    int32_t id = F<int32_t>(o, 0x352);
    if (id != -1) {
        for (uint8_t* c = (uint8_t*)Collection_Get(g_cities, 0, id); c; ) {
            Virtual(c, 0x10);
            Obj_UnlinkFromNode(c);
            int32_t next = F<int32_t>(c, 0x352);
            if (next == -1) break;
            c = (uint8_t*)Collection_Get(g_cities, 0, next);
        }
    }
    if (Collection_IsValid(g_trains, 0, F<uint32_t>(o, 0x390))) {
        // The train is looked up twice, as in the original (a -1 ID reads through null).
        auto trainNode = [&]() -> uint32_t {
            int32_t t = F<int32_t>(o, 0x390);
            uint8_t* tr = t != -1 ? (uint8_t*)Collection_Get(g_trains, 0, t) : nullptr;
            return F<uint32_t>(tr, 0x4d);
        };
        if (Collection_IsValid(g_trackNodes, 0, trainNode())) {
            void* node = Collection_Get(g_trackNodes, 0, trainNode());
            Node_ClearLinks(node, 0, F<int32_t>(o, 0x390), -3);
        }
    }
    Virtual(o, 0x10);
}
RT3_REPLACE(0x41c8c0, Obj_DetachChain);

// 0x41d4c0. Save of a collection whose records save themselves (virtual slot
// 17), framed by tags 0x4a39/0x4a3a/0x4a3b.
void __fastcall Collection_SaveWithRecords(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x4a39);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x4a3a);
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        void* r = Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        ((void(__thiscall*)(void*, Stream))(*(void***)r)[0x44 / 4])(r, s);
    }
    StreamWriteU32(s, 0x4a3b);
}
RT3_REPLACE(0x41d4c0, Collection_SaveWithRecords);

void __fastcall Collection5c9824_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c9824;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x41d7a0, Collection5c9824_Destruct);

// 0x41e100. ID of the record with +0x41 == v whose years [+0x105, +0x109] contain now; 0 if none.
uint32_t __fastcall Table_FindCurrentFor(Collection* t, int, int32_t v) {
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 0x41) != v) continue;
        int32_t year = F<uint16_t>(g_world, 0xd);
        if (year >= F<int32_t>(r, 0x105) && year <= F<int32_t>(r, 0x109)) return Collection_IdAt(t, 0, i, 0);
    }
    return 0;
}
RT3_REPLACE(0x41e100, Table_FindCurrentFor);

// 0x41e1b0 / 0x41e1d0 / 0x41e1f0. Cargo amount bookkeeping (floats).
void __fastcall Cargo_MoveTo66(void* c, int, float a) {
    F<float>(c, 0x5a) = (float)((double)F<float>(c, 0x5a) - a);
    F<float>(c, 0x66) = (float)((double)a + F<float>(c, 0x66));
}
RT3_REPLACE(0x41e1b0, Cargo_MoveTo66);
void __fastcall Cargo_MoveTo62(void* c, int, float a) {
    F<float>(c, 0x5a) = (float)((double)F<float>(c, 0x5a) - a);
    F<float>(c, 0x62) = (float)((double)a + F<float>(c, 0x62));
}
RT3_REPLACE(0x41e1d0, Cargo_MoveTo62);
void __fastcall Cargo_Produce(void* c, int, float a, float price) {
    F<float>(c, 0x5a) = (float)((double)a + F<float>(c, 0x5a));
    F<float>(c, 0x5e) = (float)((double)a + F<float>(c, 0x5e));
    F<float>(c, 0x76) = (float)((double)a * price + F<float>(c, 0x76));
    F<float>(c, 0x7a) = (float)((double)a + F<float>(c, 0x7a));
}
RT3_REPLACE(0x41e1f0, Cargo_Produce);

void __fastcall Collection5c997c_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5c997c;
    Collection_Destruct(c);
}
RT3_REPLACE(0x41e8d0, Collection5c997c_Destruct);

// 0x41e8e0 / 0x41e930. Load / save framed by tags 0x6979/0x697a.
int32_t __fastcall Collection_LoadTagged6979(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x6979, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    return StreamSkip4(s, 0x697a, 0, 0) + n;
}
RT3_REPLACE(0x41e8e0, Collection_LoadTagged6979);
void __fastcall Collection_SaveTagged6979(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x6979);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x697a);
}
RT3_REPLACE(0x41e930, Collection_SaveTagged6979);

} // namespace rt3
