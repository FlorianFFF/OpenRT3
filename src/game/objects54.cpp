// Functions across blocks 0x46..0x4b (batch 57). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

int32_t __fastcall Load_Age(uint8_t* l);
uint8_t* __fastcall Station_Construct(uint8_t* s);
void __fastcall Station_InitWithId(uint8_t* s, int, int32_t id);
void __fastcall Nop487630(void*);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);

// 0x468160. Ages every cargo load, releasing the expired ones.
void __fastcall Loads_Age(Collection* t) {
    for (int32_t i = 0; i < Collection_Count(t); ) {
        if (Load_Age((uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)))) {
            void** vt = *(void***)t;
            ((void(__thiscall*)(void*, uint32_t))vt[2])(t, Collection_IdAt(t, 0, i, 0));
        } else {
            ++i;
        }
    }
}
RT3_REPLACE(0x468160, Loads_Age);

// 0x47da40. Site extra data save (tags 0x4e21/0x4e24): key list +0x466,
// the three 5-byte lists +0x24 (count +0x30), the five pair arrays +0x5a4
// and the 12-byte array +0x34 (count +0x38).
void __fastcall Site_SaveExtra(uint8_t* o, int, Stream s) {
    StreamWriteU32(s, 0x4e21);
    StreamWrite(s, o + 0x462, 4, 0);
    for (int32_t i = 0; i < F<int32_t>(o, 0x462); ++i) StreamWrite(s, F<uint8_t*>(o, 0x466) + i * 6, 6, 0);
    if (F<int32_t>(o, 0x30))
        for (int k = 0; k < 3; ++k) StreamWrite(s, F<void*>(o, 0x24 + k * 4), F<int32_t>(o, 0x30) * 5, 0);
    for (int k = 0; k < 5; ++k)
        if (F<int32_t>(o, 0x590 + k * 4) > 0) StreamWrite(s, F<void*>(o, 0x5a4 + k * 4), F<int32_t>(o, 0x590 + k * 4) * 8, 0);
    if (F<int32_t>(o, 0x38) > 0) StreamWrite(s, F<void*>(o, 0x34), F<int32_t>(o, 0x38) * 12, 0);
    StreamWriteU32(s, 0x4e24);
}
RT3_REPLACE(0x47da40, Site_SaveExtra);

// 0x487d40. Adds a new station (0xdd1-byte record), optionally named; returns its ID.
uint32_t __fastcall Stations_Add(Collection* t, int, const char* name) {
    static uint8_t local[0xdd4];  // original: stack (initialised by 0x487620)
    for (auto& b : local) b = 0;
    Station_Construct(local);
    uint32_t id = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x487d63)(t, 0, local, 0xdd1, 0, 0);
    uint8_t* r = (uint8_t*)Collection_Get(t, 0, id);
    Station_InitWithId(r, 0, (int32_t)id);
    if (name) StrCopyN((char*)r + 4, name, 0x28);
    Nop487630(local);
    return id;
}
RT3_REPLACE(0x487d40, Stations_Add);

// 0x4bc260. Outputs +0x80, +0x84 and the table value 0x621e24[+0x78] as float.
void __fastcall Obj_GetDims(uint8_t* o, int, int32_t* a, int32_t* b, float* c) {
    *a = F<int32_t>(o, 0x80);
    *b = F<int32_t>(o, 0x84);
    *c = (float)((const int32_t*)0x621e24)[F<int32_t>(o, 0x78)];
}
RT3_REPLACE(0x4bc260, Obj_GetDims);

} // namespace rt3
