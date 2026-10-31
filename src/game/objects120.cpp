// Vehicle sound selection / channel teardown in blocks 0x45..0x53 (batch 123). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint32_t __fastcall Table_FindByAltName(Collection* t, int, const char* name);
void __fastcall Channel_SetName(uint8_t* c, int, const char* name);
void __fastcall Channel_Detach(uint8_t* c);

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x45d1e0. Picks the engine sound of the vehicle (+0x336): 0 = its type's sound
// (table 0x5f3a60 by +0x83), 1 / 2 = the fixed sounds 0x5cb680 / 0x5cb664.
void __fastcall Vehicle_SelectSound(uint8_t* v, int, int32_t kind) {
    Collection* types = hook::Global<Collection*>(0x6ada84);
    uint32_t id = Table_FindByAltName(types, 0, F<const char*>(v, 0x32e));
    if (!id) return;
    uint8_t* t = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6ada84), 0, id);
    uint8_t* ch = F<uint8_t*>(v, 0x336);
    if (!ch) return;
    if (kind == 0) Channel_SetName(ch, 0, ((const char* const*)0x5f3a60)[F<int32_t>(t, 0x83)]);
    else if (kind == 1) Channel_SetName(ch, 0, (const char*)0x5cb680);
    else if (kind == 2) Channel_SetName(ch, 0, (const char*)0x5cb664);
}
RT3_REPLACE(0x45d1e0, Vehicle_SelectSound);

// 0x5359c0. Frees the 27 fixed channels (+0x40c; count +0x408).
RT3_NOINLINE void __fastcall Terrain_FreeFixedChannels(uint8_t* r) {
    for (int k = 0; k < 0x1b; ++k) {
        uint8_t* c = F<uint8_t*>(r, 0x40c + k * 4);
        if (!c) continue;
        Channel_Detach(c);
        ExeDelete(c);
        F<void*>(r, 0x40c + k * 4) = nullptr;
        F<int32_t>(r, 0x408)--;
    }
}
RT3_REPLACE(0x5359c0, Terrain_FreeFixedChannels);

// 0x535df0. Frees every channel in the per-block lists (+0x1679), the lists and the grid.
RT3_NOINLINE void __fastcall Terrain_DestroyCell1679(uint8_t* r) {
    if (!F<void*>(r, 0x1679)) return;
    if ((F<int32_t>(r, 0x15d9) & ~0xf) > 0) {
        for (int32_t i = 0; i < (F<int32_t>(r, 0x15d9) >> 4); ++i) {
            if ((F<int32_t>(r, 0x15d5) & ~0xf) <= 0) continue;
            for (int32_t j = 0; j < (F<int32_t>(r, 0x15d5) >> 4); ++j) {
                PtrList* l = F<PtrList**>(r, 0x1679)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                int32_t n = F<int32_t>(l, 0xc);
                if (n > 0) {
                    PtrList_Rewind(l);
                    uint8_t* c = (uint8_t*)PtrList_Next(l);
                    do {
                        if (c) {
                            Channel_Detach(c);
                            ExeDelete(c);
                        }
                        c = (uint8_t*)PtrList_Next(l);
                    } while (--n);
                }
                l = F<PtrList**>(r, 0x1679)[(F<int32_t>(r, 0x15d9) >> 4) * j + i];
                if (l) {
                    PtrList_Destroy(l);
                    ExeDelete(l);
                }
            }
        }
    }
    Free(F<void*>(r, 0x1679));
    F<void*>(r, 0x1679) = nullptr;
}
RT3_REPLACE(0x535df0, Terrain_DestroyCell1679);

} // namespace rt3
