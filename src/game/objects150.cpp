// Building cargo output / site cell release / site table save in blocks 0x40..0x41 (batch 153). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/containers.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Obj_Ref3cc(void* o);
double __fastcall Industry_OutputTotal(uint8_t* ind, int, int32_t cargo, int32_t p);
void __fastcall Map_SetCoarseByte(void* m, int, int32_t x, int32_t y, uint8_t v);
void __stdcall Map_PixelsPerTile(float* sx, float* sy);
void __fastcall List_DestroyThunk(MaxList* l);

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x40fba0. The building's output of cargo: its type's industry total (second set
// when +0x246) times the building's count (+0x3cc type +0x141).
double __fastcall Building_CargoOutput(uint8_t* b, int, int32_t cargo) {
    float n = (float)F<int32_t>(Obj_Ref3cc(b), 0x141);
    uint8_t* t = ((uint8_t*(__thiscall*)(void*))(*(void***)b)[0x80 / 4])(b);
    return Industry_OutputTotal(t, 0, cargo, F<int32_t>(b, 0x246) ? 1 : 0) * n;
}
RT3_REPLACE(0x40fba0, Building_CargoOutput);

// 0x4162d0. Releases site's claim on the cells of rect {x0, x1, y0, y1}: unlinks it
// from each cell's chain in the cell hash (key y << 16 | x; nodes {next, site}),
// handing its per-cell data (+0x4c8 / +0x664 / +0x6c9, k-th cell) to the site
// before it, clears the placement grid (0x62b300) where it was the owner, and
// returns the node to the pool (or frees it).
void __stdcall Site_ReleaseCells(const int32_t* rect, HashMap* hash, uint32_t* pool, uint8_t* site) {
    int32_t k = 0;
    for (int32_t y = rect[2]; y <= rect[3]; ++y)
        for (int32_t x = rect[0]; x <= rect[1]; ++x, ++k) {
            uint32_t key = ((uint32_t)y << 16) | (uint32_t)(x & 0xffff);
            uint32_t* n = (uint32_t*)Hash_Get(hash, 0, key);
            if (!n) continue;
            int32_t count = 0;
            for (uint32_t* p = n; p; p = (uint32_t*)p[0]) ++count;
            uint32_t* node = n;
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            if (count > 1) {
                if ((uint8_t*)n[1] == site) {
                    Hash_Remove(hash, 0, key);
                    Hash_Insert(hash, 0, key, (int32_t)n[0]);
                } else {
                    uint32_t* prev;
                    do {
                        prev = node;
                        node = (uint32_t*)node[0];
                    } while ((uint8_t*)node[1] != site);
                    uint8_t* other = (uint8_t*)prev[1];
                    int32_t j = 0, found = 0;
                    for (int32_t yy = F<int32_t>(other, 0x4a8); yy <= F<int32_t>(other, 0x4ac) && !found; ++yy)
                        for (int32_t xx = F<int32_t>(other, 0x4a0); xx <= F<int32_t>(other, 0x4a4); ++xx, ++j)
                            if (yy == y && xx == x) {
                                found = 1;
                                break;
                            }
                    if (!found) j = 0;
                    prev[0] = node[0];
                    uint8_t* s = (uint8_t*)node[1];
                    F<uint32_t>(other, 0x4c8 + j * 4) = F<uint32_t>(s, 0x4c8 + k * 4);
                    other[0x664 + j] = s[0x664 + k];
                    other[0x6c9 + j] = s[0x6c9 + k];
                }
                uint8_t* s = (uint8_t*)node[1];
                F<uint32_t>(s, 0x4c8 + k * 4) = 0x497423f0;
                uint32_t* occ = hook::Global<uint32_t*>(0x62b300);
                if (F<int32_t>(s, 0x732) != -1 && (uint8_t*)(occ ? occ[F<int32_t>(m, 0x2155) * y + x] : 0) == s) {
                    occ[F<int32_t>(m, 0x2155) * y + x] = 0;
                } else {
                    s[0x664 + k] = 0xff;
                    s[0x6c9 + k] = 0xff;
                }
            } else {
                Hash_Remove(hash, 0, key);
                if (F<int32_t>((uint8_t*)n[1], 0x732) != -1)
                    hook::Global<uint32_t*>(0x62b300)[F<int32_t>(hook::Global<uint8_t*>(0x62c120), 0x2155) * y + x] = 0;
                Map_SetCoarseByte(hook::Global<void*>(0x62c120), 0, x, y, 0);
            }
            if (pool[3] && (uint32_t)node >= pool[1] && (uint32_t)node < pool[2]) {
                uint32_t* l = (uint32_t*)pool[0];
                node[0] = l[0];
                l[0] = (uint32_t)node;
                ++l[2];
                if (!l[1]) l[1] = (uint32_t)node;
            } else {
                ExeDelete(node);
            }
        }
}
RT3_REPLACE(0x4162d0, Site_ReleaseCells);

// 0x416a70. Saves the site table: the sites collected from the cell hash (+0x88,
// callback 0x416830 into the list 0x62ba58, count 0x62ba64) after a {0xbabe, 1,
// count} header, each record with its variable parts (see Sites_LoadTable).
void __fastcall Sites_SaveTable(uint8_t* c, int, Stream s) {
    int32_t one = 1;
    uint32_t* l = (uint32_t*)ExeNew(0xc);
    if (l) {
        l[0] = 0;
        l[1] = 0;
        l[2] = 0;
    }
    hook::Global<uint32_t*>(0x62ba58) = l;
    List_Init((MaxList*)l, 0, 0);
    hook::Global<int32_t>(0x62ba64) = 0;
    Hash_ForEach(F<HashMap*>(c, 0x88), 0, 0, (HashVisit)0x416830, &one);
    int32_t hdr[3] = { 0xbabe, 1, hook::Global<int32_t>(0x62ba64) };
    StreamWrite(s, hdr, 0xc, 0);
    float sx, sy;
    Map_PixelsPerTile(&sx, &sy);
    for (uint8_t* e = (uint8_t*)List_PopMax(hook::Global<MaxList*>(0x62ba58)); e;
         e = (uint8_t*)List_PopMax(hook::Global<MaxList*>(0x62ba58))) {
        static const uint16_t fixed[] = { 0x74a, 4, 8, 0xc, 0x4b8, 0x4bc, 0x49c };
        for (uint16_t off : fixed) StreamWrite(s, e + off, 4, 0);
        for (uint32_t k = 0; k < F<uint32_t>(e, 0x49c) + 1; ++k) StreamWrite(s, e + 0x10 + k * 4, 4, 0);
        StreamWrite(s, e + 0x4a0, 0x10, 0);
        StreamWrite(s, e + 0x4c4, 4, 0);
        if (F<int32_t>(e, 0x4c4) > 0) StreamWrite(s, F<void*>(e, 0x4c0), F<int32_t>(e, 0x4c4), 0);
        StreamWrite(s, e + 0x65c, 4, 0);
        for (uint32_t k = 0; k < F<uint32_t>(e, 0x65c); ++k) StreamWrite(s, e + 0x4c8 + k * 4, 4, 0);
        StreamWrite(s, e + 0x72e, 4, 0);
        for (uint32_t k = 0; k < F<uint32_t>(e, 0x72e); ++k) {
            StreamWrite(s, e + 0x664 + k, 1, 0);
            StreamWrite(s, e + 0x6c9 + k, 1, 0);
        }
        StreamWrite(s, e + 0x732, 4, 0);
    }
    if (MaxList* lst = hook::Global<MaxList*>(0x62ba58)) {
        List_DestroyThunk(lst);
        ExeDelete(lst);
    }
    hook::Global<void*>(0x62ba58) = nullptr;
}
RT3_REPLACE(0x416a70, Sites_SaveTable);

} // namespace rt3
