// Version info / tree rows / buttons / worker thread / key table / timed queue in blocks 0x48..0x56 (batch 114). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/containers.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

uint32_t __fastcall File_Version(const char* path);
const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
int32_t __fastcall TreeView_IsHidden(uint8_t* w, int, int32_t i);
void __fastcall String_Assign(char** s, const char* src);
uint32_t IsEnglish();

// 0x482d10. The game version as a number (major + minor / 0x5c8c28), cached.
float __fastcall Game_VersionNumber() {
    if (!hook::Global<uint8_t>(0x6cec84)) {
        hook::Global<uint8_t>(0x6cec84) = 1;
        uint32_t v = File_Version((const char*)0x5cec18);
        float lo = (float)(int32_t)(v & 0xffff);
        float hi = (float)(int32_t)(v >> 16);
        hook::Global<float>(0x6cec5c) = (float)((double)lo / *(const float*)0x5c8c28 + hi);
    }
    return hook::Global<float>(0x6cec5c);
}
RT3_REPLACE(0x482d10, Game_VersionNumber);

// 0x482d80. The localised version text (string 0xf54 with "major.minor"), cached in 0x6cec24.
const char* __fastcall Game_VersionText() {
    if (!hook::Global<uint8_t>(0x6cec85)) {
        hook::Global<uint8_t>(0x6cec85) = 1;
        char buf[0x34];
        uint32_t lo = File_Version((const char*)0x5cec18) & 0xffff;
        uint32_t hi = File_Version((const char*)0x5cec18) >> 16;
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(buf, (const char*)0x5ced5c, hi, lo);
        FormatText((char*)0x6cec24, hook::CallTarget<const char*(__fastcall*)(int32_t)>(0x482ddb)(0xf54), buf, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    return (const char*)0x6cec24;
}
RT3_REPLACE(0x482d80, Game_VersionText);

// 0x5602c0 / 0x560300. Node index <-> visible row (-1 passes through; 0 if not found).
int32_t __fastcall TreeView_RowOfNode(uint8_t* w, int, int32_t n) {
    if (n == -1) return -1;
    int32_t rows = 0;
    for (int32_t i = 0; i < n; ++i)
        if (!TreeView_IsHidden(w, 0, i)) ++rows;
    return rows;
}
RT3_REPLACE(0x5602c0, TreeView_RowOfNode);
int32_t __fastcall TreeView_NodeOfRow(uint8_t* w, int, int32_t row) {
    if (row == -1) return -1;
    int32_t cnt = 0;
    for (int32_t i = 0; i < F<int32_t>(w, 0x1d2); ++i) {
        if (TreeView_IsHidden(w, 0, i)) continue;
        if (row == cnt) return i;
        ++cnt;
    }
    return 0;
}
RT3_REPLACE(0x560300, TreeView_NodeOfRow);

// 0x5634e0. Button defaults (type 0x20).
void __fastcall Button_Init(uint8_t* w) {
    static const uint8_t zero[] = { 0xaa, 0xae, 0xa2, 0xa6, 0xb2, 0xb8, 0xbc, 0xc0, 0xc4, 0xc8, 0xd0, 0xd4, 0xd8, 0xcc };
    F<uint16_t>(w, 6) = 0x20;
    for (uint8_t off : zero) F<int32_t>(w, off) = 0;
    F<uint16_t>(w, 0xb6) = 0;
}
RT3_REPLACE(0x5634e0, Button_Init);

// 0x563550. Button setup: caption string, default font, and per-language width
// tweaks for a few known buttons.
void __fastcall Button_Setup(uint8_t* w) {
    if (uint16_t id = F<uint16_t>(w, 0xb6)) String_Assign((char**)(w + 0xb2), GetString(id));
    if (!w[0x2d]) w[0x2d] = (uint8_t)((F<int32_t>(w, 0xc8) == 0 ? 1 : 0) * 2 + 0x24);
    auto widen = [&](double k) {
        F<float>(w, 0x13) = (float)((double)F<float>(w, 0x13) + k);
        F<float>(w, 0xc0) = (float)((double)F<float>(w, 0xc0) + k);
    };
    const double small = *(const double*)0x5c9b50;
    int32_t lang = hook::Global<int32_t>(0x6d3c84);
    if ((lang == 4 || lang == 1 || lang == 2) && F<int16_t>(w, 4) == 0x4f4c) widen(small);
    lang = hook::Global<int32_t>(0x6d3c84);
    if ((lang == 4 || lang == 1 || lang == 2) && F<int16_t>(w, 4) == 0x4f4d) widen(small);
    if (!(uint8_t)IsEnglish() && F<int16_t>(w, 4) == 0x4ecc) widen(*(const double*)0x5d0aa0);
    if (hook::Global<int32_t>(0x6d3c84) == 2 && F<int16_t>(w, 4) == 0x502a) widen(small);
}
RT3_REPLACE(0x563550, Button_Setup);

// Worker thread object: handle +4, id +8, running +0xc (entry 0x563dd0).
static inline void JoinThread(uint8_t* t, uint32_t ms) {
    F<int32_t>(t, 0xc) = 0;
    Import<uint32_t(__stdcall*)(void*, uint32_t)>(0x5c81c4)(F<void*>(t, 4), ms);
    Import<int32_t(__stdcall*)(void*)>(0x5c81c8)(F<void*>(t, 4));
    F<void*>(t, 4) = nullptr;
}
void __fastcall Thread_Stop(uint8_t* t, int, uint32_t ms) {
    if (F<void*>(t, 4)) JoinThread(t, ms);
}
RT3_REPLACE(0x563df0, Thread_Stop);
void __fastcall Thread_Start(uint8_t* t) {
    if (F<int32_t>(t, 0xc)) return;
    if (F<void*>(t, 4)) JoinThread(t, 0xffffffff);
    F<void*>(t, 4) = Import<void*(__stdcall*)(void*, uint32_t, uint32_t, void*, uint32_t, void*)>(0x5c81c0)(
        nullptr, 0, 0x563dd0, t, 0, t + 8);
    F<int32_t>(t, 0xc) = 1;
}
RT3_REPLACE(0x563e30, Thread_Start);
void __fastcall Thread_Destruct(uint8_t* t) {
    F<uint32_t>(t, 0) = 0x5d1248;
    if (F<void*>(t, 4)) JoinThread(t, 0xffffffff);
}
RT3_REPLACE(0x563e90, Thread_Destruct);

void __fastcall Widget_RemoveListNode(uint8_t* w, int, PtrListNode* n) { PtrList_RemoveNode(F<PtrList*>(w, 0x8d), 0, n); }
RT3_REPLACE(0x563f10, Widget_RemoveListNode);

// Key table: 0x400 bytes, one flag per key code.
uint8_t* __fastcall KeyTable_Init(uint8_t* t) {
    for (int k = 0; k < 0x100; ++k) F<uint32_t>(t, k * 4) = 0;
    return t;
}
RT3_REPLACE(0x564150, KeyTable_Init);
void __fastcall Nop564170() {}
RT3_REPLACE(0x564170, Nop564170);
void __fastcall KeyTable_Set(uint8_t* t, int, uint8_t k) { t[k] = 1; }
RT3_REPLACE(0x564180, KeyTable_Set);
void __fastcall KeyTable_Clear(uint8_t* t, int, uint8_t k) { t[k] = 0; }
RT3_REPLACE(0x564190, KeyTable_Clear);
uint8_t __fastcall KeyTable_Get(uint8_t* t, int, uint8_t k) { return t[k]; }
RT3_REPLACE(0x5641a0, KeyTable_Get);
void __fastcall Obj_SetName24(uint8_t* o, int, const char* s) {
    char* d = (char*)o + 0x24;
    while ((*d++ = *s++) != 0) {}
}
RT3_REPLACE(0x5641b0, Obj_SetName24);

// 0x564310. Pops the highest-priority entry (heap +8): frees its buffer, removes it
// from the lookup (+4), recycles the node in the pool (+0) and counts it (+0x20).
// Returns the entry's value (+4), read after the node is released, as in the original.
uint32_t __fastcall TimedQueue_Pop(uint8_t* q) {
    uint8_t* e = (uint8_t*)Heap_PopMax(F<MaxHeap*>(q, 8));
    Free(F<void*>(e, 0x10));
    F<void*>(e, 0x10) = nullptr;
    Hash_Remove(F<HashMap*>(q, 4), 0, F<uint32_t>(e, 0));
    uint8_t* pool = F<uint8_t*>(q, 0);
    if (F<int32_t>(pool, 0xc) && (uint32_t)e >= F<uint32_t>(pool, 4) && (uint32_t)e < F<uint32_t>(pool, 8)) {
        uint32_t* fl = F<uint32_t*>(pool, 0);
        F<uint32_t>(e, 0) = fl[0];
        fl[0] = (uint32_t)e;
        fl[2]++;
        if (!fl[1]) fl[1] = (uint32_t)e;
    } else {
        ((void(__cdecl*)(void*))0x53b080)(e);
    }
    F<int32_t>(q, 0x20)++;
    return F<uint32_t>(e, 4);
}
RT3_REPLACE(0x564310, TimedQueue_Pop);

} // namespace rt3
