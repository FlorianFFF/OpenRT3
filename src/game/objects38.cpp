// Functions in block 0x48 (batch 41). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/stream.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall StrCopyN(char* dst, const char* src, size_t n);

// 0x487960. As 0x426c20 for the world-level counters +0xd51.. (kind) and +0xd61.. (class).
void __fastcall World_CountTrack(uint8_t* o, int, uint8_t* t, int32_t add) {
    int32_t d = add ? 1 : -1;
    uint8_t kind = t[0x218];
    if (kind == 1) F<int32_t>(o, 0xd51) += d;
    else if (kind == 2) F<int32_t>(o, 0xd59) += d;
    else F<int32_t>(o, 0xd55) += d;
    int32_t cls = F<int32_t>(t, 0x226);
    if (cls == 3) F<int32_t>(o, 0xd61) += d;
    else if (cls == 1) F<int32_t>(o, 0xd65) += d;
    else if (((int32_t(__thiscall*)(void*))(*(void***)t)[0x14 / 4])(t)) F<int32_t>(o, 0xd69) += d;
}
RT3_REPLACE(0x487960, World_CountTrack);

// 0x487bd0 / 0x487c00. Collection subclass 0x5cf10c (heap records, 10 + 20).
Collection* __fastcall Collection5cf10c_Construct(Collection* c) {
    Collection_SetVtbl(c);
    c->vtbl = (CollectionVtbl*)0x5cf10c;
    Collection_Init(c, 0, 0, 0, 10, 20, 0, 0, 0);
    return c;
}
RT3_REPLACE(0x487bd0, Collection5cf10c_Construct);
void __fastcall Collection5cf10c_Destruct(Collection* c) {
    c->vtbl = (CollectionVtbl*)0x5cf10c;
    Collection_Clear(c);
    Collection_Destruct(c);
}
RT3_REPLACE(0x487c00, Collection5cf10c_Destruct);

// 0x487c20 / 0x487cc0. Station collection load / save framed by
// 0x61a9/0x61aa/0x61ab with the (empty) per-record hooks 0x487670 / 0x487680.
int32_t __fastcall Stations_Load(Collection* t, int, Stream s, int32_t recordSize) {
    int32_t n = StreamSkip4(s, 0x61a9, 0, 0);
    n += Collection_Load(t, 0, s, recordSize);
    n += StreamSkip4(s, 0x61aa, 0, 0);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        n += ((int32_t(__thiscall*)(void*, Stream))0x487670)(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), s);
    return StreamSkip4(s, 0x61ab, 0, 0) + n;
}
RT3_REPLACE(0x487c20, Stations_Load);
void __fastcall Stations_Save(Collection* t, int, Stream s, int32_t recordSize) {
    StreamWriteU32(s, 0x61a9);
    Collection_Save(t, 0, s, recordSize);
    StreamWriteU32(s, 0x61aa);
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        ((void(__thiscall*)(void*, Stream))0x487680)(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), s);
    StreamWriteU32(s, 0x61ab);
}
RT3_REPLACE(0x487cc0, Stations_Save);

int32_t __fastcall Table_CountPositive3d(Collection* t) {
    int32_t n = 0;
    for (int32_t i = 0; i < Collection_Count(t); ++i)
        if (F<int32_t>(Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0)), 0x3d) > 0) ++n;
    return n;
}
RT3_REPLACE(0x4880d0, Table_CountPositive3d);

// 0x4883b0. Number of runs of characters >= 'A' (signed) in s.
int32_t __fastcall CountWords(const char* s) {
    int32_t len = 0;
    while (s[len]) ++len;
    int32_t n = 0, inWord = 0;
    for (int32_t i = 0; i < len; ++i) {
        if ((signed char)s[i] < 0x41) {
            inWord = 0;
        } else if (!inWord) {
            inWord = 1;
            ++n;
        }
    }
    return n;
}
RT3_REPLACE(0x4883b0, CountWords);

void __fastcall Obj_SetName46b(uint8_t* o, int, const char* s) { StrCopyN((char*)o + 0x46b, s, 0x3c); }
RT3_REPLACE(0x4886c0, Obj_SetName46b);

// 0x489f40. Frees the singly linked list +0x244 (next at +0xe).
void __fastcall Obj_FreeList244(uint8_t* o) {
    while (uint8_t* n = F<uint8_t*>(o, 0x244)) {
        F<uint8_t*>(o, 0x244) = F<uint8_t*>(n, 0xe);
        Free(n);
    }
}
RT3_REPLACE(0x489f40, Obj_FreeList244);

// 0x48a070 / 0x48a130. Track piece end links (+0x206, +0x20a).
int32_t __fastcall Track_Link(uint8_t* t, int, int32_t second) { return second ? F<int32_t>(t, 0x20a) : F<int32_t>(t, 0x206); }
RT3_REPLACE(0x48a070, Track_Link);
int32_t __fastcall Track_HasLink(uint8_t* t, int, uint8_t end) {
    if (end == 1) return F<int32_t>(t, 0x206) != -1 ? 1 : 0;
    if (end == 2) return F<int32_t>(t, 0x20a) != -1 ? 1 : 0;
    return 0;
}
RT3_REPLACE(0x48a130, Track_HasLink);

// 0x48a340. Sets class +0x226, bytes +0x219 / +0x251 and flag bit 5 of +0x24c.
void __fastcall Track_SetProps(uint8_t* t, int, int32_t cls, uint8_t a, uint8_t b, uint8_t flag) {
    F<int32_t>(t, 0x226) = cls;
    t[0x219] = a;
    t[0x251] = b;
    t[0x24c] ^= (uint8_t)(((flag << 5) ^ t[0x24c]) & 0x20);
}
RT3_REPLACE(0x48a340, Track_SetProps);

// 0x48a380. A fully linked piece without +0x24d becomes class 12 with +0x24d = v.
uint32_t __fastcall Track_MarkSpecial(uint8_t* t, int, int32_t v) {
    if (F<int32_t>(t, 0x206) == -1 || F<int32_t>(t, 0x20a) == -1) return 0xffffffff;
    if (F<int32_t>(t, 0x24d)) return F<uint32_t>(t, 0x24d);
    F<int32_t>(t, 0x226) = 0xc;
    F<int32_t>(t, 0x24d) = v;
    return (uint32_t)v;
}
RT3_REPLACE(0x48a380, Track_MarkSpecial);

} // namespace rt3
