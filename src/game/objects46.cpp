// Functions across blocks 0x49..0x4a (batch 49). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Obj_FreeList98(uint8_t* o);
void __fastcall Obj_FreeColumns(uint8_t* o);

// 0x493ba0. Track collection destructor (vtbl 0x5cfe18).
void __fastcall Tracks_Destruct(Collection* c) {
    F<int32_t>(c, 0xf1)++;
    c->vtbl = (CollectionVtbl*)0x5cfe18;
    Collection_Clear(c);
    Obj_FreeList98((uint8_t*)c);
    Obj_FreeColumns((uint8_t*)c);
    F<int32_t>(c, 0xf1)--;
    Collection_Destruct(c);
}
RT3_REPLACE(0x493ba0, Tracks_Destruct);

// 0x4a6e70. Books `amount` of revenue on a train: per paying company `key`
// (other than its own +0x51) in the 3-slot table +0x1ac/+0x1b2 (count
// +0x1b8), then into +0x39 and the running +0x65 / scaled +0x61.
void __fastcall Train_AddRevenue(uint8_t* t, int, int32_t amount, int32_t key) {
    if (key != F<int32_t>(t, 0x51)) {
        int found = 0;
        for (int32_t i = 0; i < t[0x1b8]; ++i) {
            if (key != F<uint16_t>(t, 0x1ac + i * 2)) continue;
            F<uint16_t>(t, 0x1b2 + i * 2) += (uint16_t)amount;
            found = 1;
        }
        if (!found && t[0x1b8] < 3) {
            F<uint16_t>(t, 0x1b2 + t[0x1b8] * 2) = (uint16_t)amount;
            F<uint16_t>(t, 0x1ac + t[0x1b8] * 2) = (uint16_t)key;
            t[0x1b8]++;
        }
    }
    float f = (float)amount;
    F<float>(t, 0x39) = (float)((double)f + F<float>(t, 0x39));
    float v = (float)((double)f + F<float>(t, 0x65));
    F<float>(t, 0x65) = v;
    F<float>(t, 0x61) = (float)((double)v * F<float>(t, 0xb4));
}
RT3_REPLACE(0x4a6e70, Train_AddRevenue);

// 0x4a6fd0. out = a - b (float vec3).
void __fastcall Vec3_Sub(const float* a, int, float* out, const float* b) {
    float z = (float)((double)a[2] - b[2]);
    float y = (float)((double)a[1] - b[1]);
    float x = (float)((double)a[0] - b[0]);
    out[1] = y;
    out[2] = z;
    out[0] = x;
}
RT3_REPLACE(0x4a6fd0, Vec3_Sub);

int32_t __fastcall Train_StateIs7(uint8_t* t) { return F<int32_t>(t, 0xe8) == 7 ? 1 : 0; }
RT3_REPLACE(0x4a7010, Train_StateIs7);
void __fastcall Train_Clear1a3(uint8_t* t) { F<int32_t>(t, 0x1a3) = 0; }
RT3_REPLACE(0x4a7020, Train_Clear1a3);

// 0x4a7030. Saves an array of 0x33-byte records {data +0, count +4} between tags 0x5dc1/0x5dc2.
void __fastcall Array33_Save(uint32_t* o, int, Stream s) {
    StreamWriteU32(s, 0x5dc1);
    if (o[1]) StreamWrite(s, (const void*)o[0], o[1] * 0x33, 0);
    StreamWriteU32(s, 0x5dc2);
}
RT3_REPLACE(0x4a7030, Array33_Save);

// 0x4a7170. Sets the speed target +0xcc (clearing +0x1d8) unless v is at most 0x5d0058.
void __fastcall Train_SetTarget(uint8_t* t, int, float v) {
    if (*(const float*)0x5d0058 >= v) {
        t[0x1d8] = 1;
        return;
    }
    t[0x1d8] = 0;
    F<float>(t, 0xcc) = v;
}
RT3_REPLACE(0x4a7170, Train_SetTarget);

void __fastcall Train_ResetStop(uint8_t* t) {
    F<uint32_t>(t, 0x8c) = 0xbf800000;  // -1.0f
    F<int32_t>(t, 0x1ba) = F<int32_t>(t, 0x94);
    F<int32_t>(t, 0x94) = -1;
}
RT3_REPLACE(0x4a71a0, Train_ResetStop);

int32_t __fastcall Train_StateSpecial(uint8_t* t) {
    int32_t s = F<int32_t>(t, 0xe8);
    return s == 0x12 || s == 0x11 || s == 0xb || s == 0xc || s == 0xd ? 1 : 0;
}
RT3_REPLACE(0x4a71d0, Train_StateSpecial);
int32_t __fastcall Train_StateBOr12(uint8_t* t) {
    int32_t s = F<int32_t>(t, 0xe8);
    return s == 0xb || s == 0x12 ? 1 : 0;
}
RT3_REPLACE(0x4a7200, Train_StateBOr12);

double __fastcall Train_Float109(uint8_t* t, int, int32_t i) { return F<float>(t, 0x109 + i * 4); }
RT3_REPLACE(0x4a7240, Train_Float109);
double __fastcall Train_Float131(uint8_t* t, int, int32_t i) { return F<float>(t, 0x131 + i * 4); }
RT3_REPLACE(0x4a7250, Train_Float131);
double __fastcall Train_Float159(uint8_t* t, int, int32_t i) { return F<float>(t, 0x159 + i * 4); }
RT3_REPLACE(0x4a7260, Train_Float159);

// 0x4a72b0. Train year roll-over: three stats (first entries of +0x109,
// +0x131, +0x159) into the totals +0xfd.., histories shifted by one.
void __fastcall Train_RollStats(uint8_t* t) {
    F<float>(t, 0xfd) = (float)((double)F<float>(t, 0xfd) + F<float>(t, 0x109));
    F<float>(t, 0x101) = (float)((double)F<float>(t, 0x131) + F<float>(t, 0x101));
    F<float>(t, 0x105) = (float)((double)F<float>(t, 0x105) + F<float>(t, 0x159));
    for (int k = 9; k >= 1; --k)
        for (int a = 0; a < 3; ++a) F<uint32_t>(t, 0x109 + a * 0x28 + k * 4) = F<uint32_t>(t, 0x109 + a * 0x28 + (k - 1) * 4);
    for (int a = 0; a < 3; ++a) F<uint32_t>(t, 0x109 + a * 0x28) = 0;
}
RT3_REPLACE(0x4a72b0, Train_RollStats);

} // namespace rt3
