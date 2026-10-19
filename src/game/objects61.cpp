// Functions across blocks 0x41..0x51 (batch 64). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

static Collection*& g_trackNodes = hook::Global<Collection*>(0x6cfca8);

void __fastcall Node_SetLinkPair(void* n, int, int32_t a, int32_t b, int32_t first);
int32_t __fastcall Track_Field238(uint8_t* t, int, int32_t flag);
void __fastcall Obj_SetElementFlag1d8(void* o, int32_t mode, int32_t idx);
double __fastcall Train_Float109(uint8_t* t, int, int32_t i);

// 0x41aea0. Links this vehicle (+0x390, +0x34a) into track node `id` and,
// where still free, into its two neighbours (on the matching side).
void __fastcall Car_LinkToNode(uint8_t* car, int, int32_t id, int32_t side) {
    if (id == -1) return;
    uint8_t* n = (uint8_t*)Collection_Get(g_trackNodes, 0, id);
    Node_SetLinkPair(n, 0, F<int32_t>(car, 0x390), F<int32_t>(car, 0x34a), side);
    if (F<int32_t>(n, 0x206) != -1) {
        uint8_t* m = (uint8_t*)Collection_Get(g_trackNodes, 0, F<uint32_t>(n, 0x206));
        int32_t s = side;
        if (F<int32_t>(m, 0x206) == id || (F<int32_t>(m, 0x20e) == id && (m[0x201] & 2))) s = side == 0;
        if (Track_Field238(m, 0, s) == -1) Node_SetLinkPair(m, 0, F<int32_t>(car, 0x390), F<int32_t>(car, 0x34a), s);
    }
    if (F<int32_t>(n, 0x20a) != -1) {
        uint8_t* m = (uint8_t*)Collection_Get(g_trackNodes, 0, F<uint32_t>(n, 0x20a));
        int32_t s = side;
        if (F<int32_t>(m, 0x20a) == id || (F<int32_t>(m, 0x20e) == id && !(m[0x201] & 2))) s = side == 0;
        if (Track_Field238(m, 0, s) == -1) Node_SetLinkPair(m, 0, F<int32_t>(car, 0x390), F<int32_t>(car, 0x34a), s);
    }
}
RT3_REPLACE(0x41aea0, Car_LinkToNode);

// 0x45a400. Starts a block iteration and clears slot idx (-1: all four) of
// the first block's +0xf4 for each entry, unmarking its element (0x475240)
// and flagging the entry 0x20000 (then virtual slot 4 or 3).
void __fastcall BlockIter_ClearSlot(uint8_t* o, int32_t idx) {
    hook::Global<int32_t>(0x6acd44) = 0;
    hook::Global<int32_t>(0x6acd48) = 0;
    hook::Global<int32_t>(0x6acd54) = 0;
    hook::Global<uint8_t*>(0x6acd50) = o;
    hook::Global<int32_t>(0x6acd4c) = 0;
    int32_t n = F<int32_t>(o, 0xc);
    hook::Global<uint8_t*>(0x6acd4c) = o + (n > 0 ? n * 12 + 0x10 : 0x10);
    for (int32_t i = 0; i < F<int32_t>(hook::Global<uint8_t*>(0x6acd50), 0xc); ++i) {
        uint8_t* entry = o + 0x10 + i * 0xc;
        if (!F<void*>(entry, 8)) continue;
        if (idx == -1) {
            for (int k = 0; k < 4; ++k) hook::Global<uint8_t*>(0x6acd4c)[0xf4 + k] = 0;
        } else {
            hook::Global<uint8_t*>(0x6acd4c)[0xf4 + idx] = 0;
        }
        Obj_SetElementFlag1d8(F<void*>(entry, 8), 0, idx);
        uint8_t* it = hook::Global<uint8_t*>(0x6acd50);
        uint8_t* obj = F<uint8_t*>(entry, 0);
        uint8_t* e = it + 0x10;
        for (int32_t j = 0; j < F<int32_t>(it, 0xc); ++j, e += 0xc) {
            if (F<uint8_t*>(e, 0) != obj) continue;
            uint32_t& f = F<uint32_t>(e, 4);
            f |= 0x20000;
            if (f & 0x800) ((void(__thiscall*)(void*))(*(void***)obj)[0x10 / 4])(obj);
            else if (!(f & 0x2000)) ((void(__thiscall*)(void*))(*(void***)obj)[0xc / 4])(obj);
            break;
        }
    }
}
RT3_REPLACE(0x45a400, BlockIter_ClearSlot);

void __stdcall SetGlobal6d19f4(int32_t v) { hook::Global<int32_t>(0x6d19f4) = v; }
RT3_REPLACE(0x50a660, SetGlobal6d19f4);

void __fastcall Obj_CopyTo6d1a08(const uint32_t* o) {
    for (int k = 0; k < 10; ++k) ((uint32_t*)0x6d1a08)[k] = o[k];
}
RT3_REPLACE(0x50c4e0, Obj_CopyTo6d1a08);

uint8_t Global622b0cIs64() { return hook::Global<int32_t>(0x622b0c) == 0x40 ? 1 : 0; }
RT3_REPLACE(0x50d0a0, Global622b0cIs64);

int32_t __fastcall Obj_Flag7dOrTracks118(uint8_t* o) {
    return o[0x7d] || F<int32_t>(hook::Global<uint8_t*>(0x6cfca8), 0x118) ? 1 : 0;
}
RT3_REPLACE(0x50d9b0, Obj_Flag7dOrTracks118);

void* Get6d3ae8() { return (void*)0x6d3ae8; }
RT3_REPLACE(0x50f5f0, Get6d3ae8);
void* Get6d3ae8b() { return (void*)0x6d3ae8; }
RT3_REPLACE(0x50f600, Get6d3ae8b);
void __stdcall SetGlobal6d1aa4(int32_t v) { hook::Global<int32_t>(0x6d1aa4) = v; }
RT3_REPLACE(0x50f730, SetGlobal6d1aa4);
int32_t Global6d3ae4() { return hook::Global<int32_t>(0x6d3ae4); }
RT3_REPLACE(0x50f740, Global6d3ae4);
void SetGlobal6d3adc() { hook::Global<int32_t>(0x6d3adc) = 1; }
RT3_REPLACE(0x50f750, SetGlobal6d3adc);

double __fastcall Train_Total109(uint8_t* t) { return Train_Float109(t, 0, 0) + F<float>(t, 0xfd); }
RT3_REPLACE(0x5130b0, Train_Total109);

} // namespace rt3
