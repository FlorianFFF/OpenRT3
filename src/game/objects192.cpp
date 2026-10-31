// Terrain quad batches and the 2D sample command visitor in block 0x56 (batch 198).
// See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/containers.h"
#include "core/crt.h"
#include "core/ptrlist.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Quad_ToVertices(uint8_t* out, uint8_t* q, float lift);

// 0x567c70 / 0x567ce0. Expands every quad of list +0x1c (flat) / +0x20 (raised by
// `lift`) with tint `tint` into `out`, adding 6 vertices per quad to *count.
void __fastcall Quads_EmitFlat(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t tint) {
    if (!F<int32_t>(F<uint8_t*>(o, 0x1c), 0xc)) return;
    hook::Global<uint32_t>(0xd97bbc) = tint;
    PtrList_Rewind(F<PtrList*>(o, 0x1c));
    for (uint8_t* q = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x1c)); q; q = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x1c))) {
        Quad_ToVertices(out, q, 0.0f);
        out += 0x90;
    }
    *count += F<int32_t>(F<uint8_t*>(o, 0x1c), 0xc) * 6;
}
RT3_REPLACE(0x567c70, Quads_EmitFlat);

void __fastcall Quads_EmitRaised(uint8_t* o, int, uint8_t* out, int32_t* count, uint32_t tint, float lift) {
    if (!F<int32_t>(F<uint8_t*>(o, 0x20), 0xc)) return;
    hook::Global<uint32_t>(0xd97bbc) = tint;
    PtrList_Rewind(F<PtrList*>(o, 0x20));
    for (uint8_t* q = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x20)); q; q = (uint8_t*)PtrList_Next(F<PtrList*>(o, 0x20))) {
        Quad_ToVertices(out, q, lift);
        out += 0x90;
    }
    *count += F<int32_t>(F<uint8_t*>(o, 0x20), 0xc) * 6;
}
RT3_REPLACE(0x567ce0, Quads_EmitRaised);

void __fastcall Voices_Release(uint8_t* v, int, uint32_t key);

// Drops sample entry `e` (handle h): releases its voice, removes it from the table
// and frees its data.
static void Sample_Drop(uint8_t* s, uint8_t* e, uint32_t h) {
    Voices_Release(s, 0, h);
    Hash_Remove((HashMap*)(s + 0xc), 0, F<uint32_t>(e, 0));
    if (void* d = F<void*>(e, 4)) {
        Free(d);
        F<void*>(e, 4) = nullptr;
    }
}

// 0x56a970. Applies command ctx[0] to one 2D sample (table node -> entry e, Miles
// handle e[-4]): 1 end it (only entry ctx[1] unless -1), 2 refresh volume of a
// looping one, 3 apply its pending changes (+0x44: 0x100 drop, 0x200 loop count,
// 1 volume, 4 volume + pan, 0x80 rate, 8 stop / delayed resume), 4/5 pause (5 only
// looping ones), 6/7 resume (7 only looping ones). Always 0.
int32_t __fastcall Sound2D_Command(uint8_t* s, int, uint8_t* node, int32_t* ctx) {
    uint8_t* e = F<uint8_t*>(node, 4);
    uint32_t h = F<uint32_t>(e, -4);
    auto volume = [&]() { return (float)((double)F<float>(e, 0x28) * F<float>(s, 0x20)); };
    auto stop = [&]() { Import<void(__stdcall*)(uint32_t)>(0x5c846c)(h); };
    auto resume = [&]() { Import<void(__stdcall*)(uint32_t)>(0x5c8468)(h); };
    auto pause = [&]() {
        if (F<int32_t>(e, 0xc) && !(F<uint32_t>(e, 0x44) & 8)) return;
        F<int32_t>(e, 0xc) = 1;
        F<uint32_t>(e, 0x44) &= ~8u;
        stop();
    };
    auto unpause = [&]() {
        if (F<int32_t>(e, 0xc) != 1 && !(F<uint32_t>(e, 0x44) & 8)) return;
        F<uint32_t>(e, 0x44) &= ~8u;
        F<int32_t>(e, 0xc) = 0;
        resume();
    };
    switch (ctx[0]) {
    case 1:
        if (ctx[1] != -1 && F<int32_t>(e, 4) != ctx[1]) break;
        Import<int32_t(__stdcall*)(uint32_t, uint32_t)>(0x5c83a8)(h, 0);
        Import<void(__stdcall*)(uint32_t)>(0x5c8474)(h);
        Sample_Drop(s, e, h);
        break;
    case 2:
        if (F<int32_t>(e, 8) == 1) {
            float v = volume();
            Import<void(__stdcall*)(uint32_t, float, float)>(0x5c8470)(h, v, v);
        }
        break;
    case 3: {
        uint32_t flags = F<uint32_t>(e, 0x44);
        if (!flags) break;
        F<uint32_t>(e, 0x44) = 0;
        if (flags & 0x100) {
            if (F<int32_t>(e, 8) == 1) --F<int32_t>(s, 0x24);
            Sample_Drop(s, e, h);
            break;
        }
        if (flags & 0x200) Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c845c)(h, F<int32_t>(e, 0x48));
        if (flags & 1) {
            float v = volume();
            Import<void(__stdcall*)(uint32_t, float, float)>(0x5c8470)(h, v, v);
        }
        if (flags & 4) Import<void(__stdcall*)(uint32_t, float, float)>(0x5c8458)(h, volume(), F<float>(e, 0x2c));
        if (flags & 0x80) Import<void(__stdcall*)(uint32_t, int32_t)>(0x5c844c)(h, F<int32_t>(e, 0x3c));
        if (!(flags & 8)) break;
        if (F<int32_t>(e, 0xc) == 1) {
            stop();
            break;
        }
        if (F<int32_t>(e, 0x50) > 0) {
            uint32_t now = Import<uint32_t(__stdcall*)()>(0x5c8068)();
            if ((int32_t)(now - F<uint32_t>(e, 0x4c)) < F<int32_t>(e, 0x50)) {
                F<uint32_t>(e, 0x44) |= 8;
                break;
            }
            F<int32_t>(e, 0x50) = 0;
        }
        resume();
        break;
    }
    case 4:
        pause();
        break;
    case 5:
        if (F<int32_t>(e, 8) == 1) pause();
        break;
    case 6:
        unpause();
        break;
    case 7:
        if (F<int32_t>(e, 8) == 1) unpause();
        break;
    }
    return 0;
}
RT3_REPLACE(0x56a970, Sound2D_Command);

} // namespace rt3
