// Private camera / effects teardown / name hash table / cursors in blocks 0x45..0x53 (batch 92). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

uint8_t* __fastcall Camera_ConstructSetup(uint8_t* c, int, uint32_t terrain, uint32_t x, uint32_t y, uint32_t z,
                                          uint32_t dist);
void __fastcall Effect_Release(uint8_t* e);
void __fastcall Vehicle_PushState(uint8_t* o, int, uint32_t state, uint8_t mode);
int __fastcall ToUpper(char c);
int32_t ClockMs();

static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x4581d0. Gives the object a private camera (+0x352) over its terrain and
// switches the view to it.
void __fastcall Obj_TakeCamera(uint8_t* o) {
    if (!F<void*>(o, 0x352)) {
        uint8_t* c = (uint8_t*)ExeNew(0x8a);
        F<uint8_t*>(o, 0x352) = c ? Camera_ConstructSetup(c, 0, F<uint32_t>(o, 4), 0, 0, 0, 0x44800000) : nullptr;
    }
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0xa0) = 1;
    F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114216) = 1;
}
RT3_REPLACE(0x4581d0, Obj_TakeCamera);

void __fastcall Effect489e80_Release(uint8_t* e) {
    F<uint32_t>(e, 0) = 0x5cfce0;
    Effect_Release(e);
}
RT3_REPLACE(0x489e80, Effect489e80_Release);

// 0x491cf0. Pushes a new undo state (+0x90 += 2) onto vehicle `id`; the first
// state is remembered in +0x88.
void __fastcall Vehicles_PushState(Collection* c, int, uint32_t id, uint8_t mode) {
    if (!Collection_IsValid(c, 0, id)) return;
    uint8_t* v = (uint8_t*)Collection_Get(c, 0, id);
    uint8_t* cc = (uint8_t*)c;
    F<int32_t>(cc, 0x90) += 2;
    Vehicle_PushState(v, 0, F<uint32_t>(cc, 0x90), mode);
    if (F<int32_t>(cc, 0x88) == -1) F<uint32_t>(cc, 0x88) = F<uint32_t>(v, 0x240);
}
RT3_REPLACE(0x491cf0, Vehicles_PushState);

// 0x535a10. Releases every effect in the +0x1675 block lists, then the lists and the grid.
RT3_NOINLINE void __fastcall Terrain_DestroyCell1675(uint8_t* r) {
    if (!F<void*>(r, 0x1675)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                PtrList* l = F<PtrList**>(r, 0x1675)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                int32_t n = F<int32_t>(l, 0xc);
                if (n > 0) {
                    PtrList_Rewind(l);
                    uint8_t* e = (uint8_t*)PtrList_Next(l);
                    do {
                        if (e) {
                            Effect_Release(e);
                            ExeDelete(e);
                        }
                        e = (uint8_t*)PtrList_Next(l);
                    } while (--n);
                }
                l = F<PtrList**>(r, 0x1675)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (l) {
                    PtrList_Destroy(l);
                    ExeDelete(l);
                }
            }
        }
    }
    Free(F<void*>(r, 0x1675));
    F<void*>(r, 0x1675) = nullptr;
}
RT3_REPLACE(0x535a10, Terrain_DestroyCell1675);

// 0x53b5c0. Copies the name to +0x427c0 and returns its case-insensitive hash
// (h = upper(c) + h * 0x41c64e6e + (h >> 16) + 0x3039, from 7).
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name) {
    char* dst = (char*)(t + 0x427c0);
    int32_t k = 0;
    while ((dst[k] = name[k]) != 0) ++k;
    int32_t len = 0;
    while (name[len]) ++len;
    int32_t h = 7;
    for (int32_t i = 0; i < len; ++i) {
        int32_t u = (int8_t)ToUpper(name[i]);
        h = (int32_t)((uint32_t)u + (uint32_t)h * 0x41c64e6eu + (uint32_t)(h >> 16) + 0x3039u);
    }
    return h;
}
RT3_REPLACE(0x53b5c0, NameTable_Hash);

// 0x53b630. The n-th entry (1-based) across the 32 hash buckets (+0x203a4, next
// +0x14), with the last position cached (+0x427b0 / +0x20424) for sequential walks.
uint8_t* __fastcall NameTable_Nth(uint8_t* t, int, int32_t n) {
    if (n == F<int32_t>(t, 0x427b0) + 1) return F<uint8_t*>(t, 0x20424);
    int32_t left = n;
    for (int32_t i = 0; i < 0x20; ++i) {
        uint8_t* e = F<uint8_t*>(t, 0x203a4 + i * 4);
        if (!left) continue;
        while (e) {
            if (!--left) {
                F<int32_t>(t, 0x427b0) = n - 1;
                F<uint8_t*>(t, 0x20424) = e;
                return e;
            }
            e = F<uint8_t*>(e, 0x14);
        }
    }
    return nullptr;
}
RT3_REPLACE(0x53b630, NameTable_Nth);

// 0x53b850. Grows the 13-byte record array +0x1f400 by 100 (capacity +0x427b4).
void __fastcall NameTable_Grow(uint8_t* t) {
    uint32_t cap = F<uint32_t>(t, 0x427b4);
    uint8_t* p = (uint8_t*)Malloc((cap + 100) * 13);
    for (uint32_t k = 0; k < 100 * 13; ++k) p[cap * 13 + k] = 0;
    if (uint8_t* old = F<uint8_t*>(t, 0x1f400)) {
        uint32_t n = F<uint32_t>(t, 0x427b4) * 13;
        for (uint32_t k = 0; k < n; ++k) p[k] = old[k];
        Free(F<void*>(t, 0x1f400));
    }
    F<uint8_t*>(t, 0x1f400) = p;
    F<uint32_t>(t, 0x427b4) += 100;
}
RT3_REPLACE(0x53b850, NameTable_Grow);

double __fastcall Font_ScaleX(uint8_t* f, int, float v) { return (double)v * F<float>(f, 0x2e); }
RT3_REPLACE(0x53dde0, Font_ScaleX);
double __fastcall Font_ScaleY(uint8_t* f, int, float v) { return (double)v * F<float>(f, 0x32); }
RT3_REPLACE(0x53ddf0, Font_ScaleY);

// 0x53de00. Entry idx of the table +0x24 (count +0x28); out of range formats a
// (discarded) diagnostic with the table name (+0x20) and returns entry 0.
RT3_NOINLINE uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx) {
    int32_t n = F<uint16_t>(t, 0x28);
    if (idx < n) return F<uint32_t*>(t, 0x24)[idx];
    char buf[0x190];
    const char* name = F<const char*>(t, 0x20);
    if (!name) name = (const char*)0x5c87a8;
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(buf, (const char*)0x5dd658, idx, n, name);
    return F<uint32_t*>(t, 0x24)[0];
}
RT3_REPLACE(0x53de00, Table_Entry);

// 0x53ee90. Loads cursor idx (colour or b/w set) from data\cursors, falling back to the arrow.
void __fastcall Cursor_Load(int32_t colour, int32_t idx) {
    char path[0x12c];
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(path, colour ? (const char*)0x5dd7f0 : (const char*)0x5dd80c,
                                                      F<uint32_t>((void*)0x625258, idx * 0x14));
    void* h = Import<void*(__stdcall*)(const char*)>(0x5c829c)(path);
    void** slot = (void**)0xccba60 + (colour * 0x17 + idx);
    *slot = h;
    if (!h) *slot = Import<void*(__stdcall*)(void*, uint32_t)>(0x5c82a0)(nullptr, 0x7f00);
}
RT3_REPLACE(0x53ee90, Cursor_Load);

// 0x53ef00. Restarts the timer (+0x24 = now, +0x28 = 0).
void __fastcall Timer_Restart(uint8_t* t) {
    F<int32_t>(t, 0x24) = ClockMs();
    F<int32_t>(t, 0x28) = 0;
}
RT3_REPLACE(0x53ef00, Timer_Restart);

// 0x53ef20. Enters a nested section; the outermost saves +0x18/+0x14 and the
// engine values +0x11473e/+0x11473a in globals.
void __fastcall Section_Enter(uint8_t* s) {
    if (++hook::Global<int32_t>(0xccbb24) > 1) return;
    hook::Global<uint32_t>(0xccbb1c) = F<uint32_t>(s, 0x18);
    hook::Global<uint32_t>(0xccba58) = F<uint32_t>(s, 0x14);
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    hook::Global<uint32_t>(0xccba5c) = F<uint32_t>(e, 0x11473e);
    hook::Global<uint32_t>(0xccbb18) = F<uint32_t>(e, 0x11473a);
}
RT3_REPLACE(0x53ef20, Section_Enter);

} // namespace rt3
