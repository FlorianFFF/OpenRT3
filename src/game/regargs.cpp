// Small game functions that the compiler gave register arguments (EAX, EBX, EDX
// outside the usual conventions). Each has a naked entry that moves the registers
// into a C++ body and returns with the original's register and stack effects.
#include "hook/hook.h"
#include <stdint.h>
#include <string.h>
#include "game/collection.h"

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
static inline double D(uint32_t a) { return *(const double*)a; }
static inline int Stricmp(const char* a, const char* b) { return ((int(__cdecl*)(const char*, const char*))0x5a57cf)(a, b); }
int32_t __fastcall HasString(int32_t id);
const char* __fastcall GetString(int32_t id);
uint32_t __fastcall Crc32String(const char* s, int32_t ignoreCase);
int32_t __fastcall Map_CellFlag(void* m, int, int32_t x, int32_t y, int32_t which);

// 0x45ec70 body. The value for a named entry of the table at 0x5f0a34 (0x5b
// entries of { value, string id, fallback name, - }), matched case-insensitively
// against the localized string or the fallback; 0xff for "", -1 when unknown.
static int32_t __cdecl NamedValue_FindImpl(const char* name) {
    if (!*name) return 0xff;
    for (uint32_t i = 0; i < 0x5b; ++i) {
        uint8_t* e = (uint8_t*)0x5f0a34 + i * 0x10;
        int32_t id = F<int32_t>(e, 4);
        const char* s = id && (uint8_t)HasString(id) ? GetString(F<int32_t>(e, 4)) : F<const char*>(e, 8);
        if (!Stricmp(name, s)) return F<int32_t>(e, 0);
    }
    return -1;
}
// 0x45ec70. Name in EBX.
__declspec(naked) void NamedValue_Find() {
    __asm {
        push ebx
        call NamedValue_FindImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x45ec70, NamedValue_Find);

// 0x45ede0 body. The model in the hash buckets 0x6acd78..0x6ad96c (12 bytes each)
// whose name hash (+0x90) matches name (first 127 characters, case-insensitive).
static uint8_t* __cdecl Models_FindByNameImpl(const char* name) {
    char buf[0x100];
    strncpy(buf, name, 0x7f);
    buf[0x7f] = 0;
    uint32_t h = Crc32String(buf, 1);
    for (uint32_t b = 0x6acd78; b <= 0x6ad96c; b += 0xc)
        for (uint8_t* n = *(uint8_t**)b; n; n = F<uint8_t*>(n, 0))
            if (h == F<uint32_t>(n, 0x90)) return n;
    return nullptr;
}
// 0x45ede0. Name in EAX.
__declspec(naked) void Models_FindByName() {
    __asm {
        push eax
        call Models_FindByNameImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x45ede0, Models_FindByName);

// 0x459bc0 body. Sets (clear == 0) or clears the flag bits mask of obj's entry in
// the table 0x6acd50 (count +0xc, entries of { obj, flags, - } at +0x10), then
// notifies obj: method 0x10 when bit 0x800 is set, else method 0xc unless bit
// 0x2000 is. Returns what the original leaves in EAX.
static uint32_t __cdecl ObjFlags_SetImpl(uint32_t mask, uint8_t* obj, int32_t clear) {
    uint8_t* t = hook::Global<uint8_t*>(0x6acd50);
    int32_t n = F<int32_t>(t, 0xc);
    uint8_t* e = t + 0x10;
    if (n <= 0) return (uint32_t)(uintptr_t)e;
    int32_t i = 0;
    for (; i < n; ++i, e += 0xc)
        if (F<uint8_t*>(e, 0) == obj) break;
    if (i == n) return (uint32_t)(uintptr_t)e;
    uint32_t flags = F<uint32_t>(e, 4);
    flags = clear ? flags & ~mask : flags | mask;
    F<uint32_t>(e, 4) = flags;
    if (flags & 0x800) return ((uint32_t(__fastcall*)(uint8_t*))(*(void***)obj)[0x10 / 4])(obj);
    if (!(flags & 0x2000)) return ((uint32_t(__fastcall*)(uint8_t*))(*(void***)obj)[0xc / 4])(obj);
    return flags;
}
// 0x459bc0. Mask in EAX, object in ECX, clear on the stack.
__declspec(naked) void ObjFlags_Set() {
    __asm {
        push [esp + 4]
        push ecx
        push eax
        call ObjFlags_SetImpl
        add esp, 12
        ret 4
    }
}
RT3_REPLACE(0x459bc0, ObjFlags_Set);

// 0x414c50 body. Whether any cell of the 4x4 block (cx, cy) has map flag 2; stops
// (false) at the map edge.
static int32_t __cdecl Map_BlockHasFlag2Impl(int32_t cx, int32_t cy) {
    uint32_t y = (uint32_t)cy * 4;
    for (int32_t dy = 0; dy < 4; ++dy, ++y) {
        uint32_t x = (uint32_t)cx * 4;
        for (int32_t dx = 0; dx < 4; ++dx, ++x) {
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            if (x >= F<uint32_t>(m, 0x215d) || y >= F<uint32_t>(m, 0x2161)) return 0;
            if (Map_CellFlag(m, 0, (int32_t)x, (int32_t)y, 2)) return 1;
        }
    }
    return 0;
}
// 0x414c50. Block x in EAX, y in EDX. Returns with ECX = x * 4 (the original
// pops its x * 4 local into ECX).
__declspec(naked) void Map_BlockHasFlag2() {
    __asm {
        push eax
        push edx
        push eax
        call Map_BlockHasFlag2Impl
        add esp, 8
        pop ecx
        shl ecx, 2
        ret
    }
}
RT3_REPLACE(0x414c50, Map_BlockHasFlag2);

struct HashMap;
uint32_t __fastcall Crc32(const uint8_t* data, uint32_t len);
int32_t __fastcall Hash_Get(HashMap* m, int, uint32_t key);
int32_t __fastcall Obj_EntryClear1d8(uint8_t* o, int32_t i);

// 0x472ea0 body. Marks the message (+4, bit 8 of byte +0xb) as already seen when
// the player's received-hash table (other +0x2580, keyed by the session id) holds
// an entry whose checksum equals the message's first word. Always 1.
static int32_t __cdecl Msg_MarkSeenImpl(uint8_t* msg, uint8_t* other) {
    uint32_t first = *F<uint32_t*>(msg, 8);
    if (!first) return 1;
    void** list = (void**)(uintptr_t)Hash_Get(F<HashMap*>(other, 0x2580), 0, F<uint32_t>(hook::Global<uint8_t*>(0x6cd8d8), 0x9888));
    if (!list) return 1;
    for (void* n = *list; n; n = *(void**)n)
        if (F<int32_t>(msg, 0xc) > 0 && F<uint8_t*>(msg, 8) && Crc32(F<uint8_t*>(msg, 8), F<uint32_t>(msg, 0xc)) == first) {
            F<uint8_t*>(msg, 4)[0xb] |= 8;
            return 1;
        }
    return 1;
}
// 0x472ea0. Message in ECX, other player in EDX.
__declspec(naked) void Msg_MarkSeen() {
    __asm {
        push edx
        push ecx
        call Msg_MarkSeenImpl
        add esp, 8
        ret
    }
}
RT3_REPLACE(0x472ea0, Msg_MarkSeen);

// 0x45a5f0 body. Starts walking the entry table t (globals 0x6acd44..0x6acd54,
// end pointer 0x6acd4c) and clears the first entry with an object (+8), passing on
// the caller's EDX.
static int32_t __cdecl EntryWalk_BeginImpl(uint8_t* t, int32_t edx) {
    hook::Global<int32_t>(0x6acd44) = 0;
    hook::Global<int32_t>(0x6acd48) = 0;
    hook::Global<int32_t>(0x6acd54) = 0;
    hook::Global<uint8_t*>(0x6acd50) = t;
    int32_t n = F<int32_t>(t, 0xc);
    hook::Global<uint8_t*>(0x6acd4c) = t + (n > 0 ? n * 0xc + 0x10 : 0x10);
    uint8_t* e = t + 0x10;
    for (int32_t i = 0; i < n; ++i, e += 0xc)
        if (F<int32_t>(e, 8)) return Obj_EntryClear1d8(F<uint8_t*>(e, 8), edx);
    return 0;
}
// 0x45a5f0. Table in ECX.
__declspec(naked) void EntryWalk_Begin() {
    __asm {
        push edx
        push ecx
        call EntryWalk_BeginImpl
        add esp, 8
        ret
    }
}
RT3_REPLACE(0x45a5f0, EntryWalk_Begin);

// 0x45ec00 body. Splits "a+b" (unless it starts with '+' or is string 0xdf2):
// dst gets b and dst2 a; otherwise dst gets the whole text and dst2 is emptied.
static void __cdecl Text_SplitPlusImpl(char* dst, char* src, char* dst2) {
    char* p;
    if (*src != '+' && Stricmp(src, GetString(0xdf2)) && (p = ((char*(__cdecl*)(const char*, int))0x5a2a00)(src, '+')) != nullptr) {
        strcpy(dst, p + 1);
        *p = 0;
        strcpy(dst2, src);
        return;
    }
    strcpy(dst, src);
    *dst2 = 0;
}
// 0x45ec00. Destination in EAX, source in ESI, second destination in EBX.
__declspec(naked) void Text_SplitPlus() {
    __asm {
        push ebx
        push esi
        push eax
        call Text_SplitPlusImpl
        add esp, 12
        ret
    }
}
RT3_REPLACE(0x45ec00, Text_SplitPlus);

// 0x459c30 body. Starts walking table t for obj (globals as in 0x45a5f0): records
// obj's entry (+0x6acd48 flags pointer, +0x6acd44 value). 1 when found or obj is
// null (then the end pointer 0x6acd4c is set), else 0.
static int32_t __cdecl EntryWalk_FindImpl(uint8_t* obj, uint8_t* t) {
    hook::Global<int32_t>(0x6acd44) = 0;
    hook::Global<int32_t>(0x6acd48) = 0;
    hook::Global<uint8_t*>(0x6acd54) = obj;
    hook::Global<uint8_t*>(0x6acd50) = t;
    hook::Global<int32_t>(0x6acd4c) = 0;
    int32_t found = 0;
    uint32_t end = 0x10;
    if (F<int32_t>(t, 0xc) > 0) {
        uint8_t* e = t + 0x10;
        for (int32_t i = 0; i < F<int32_t>(t, 0xc); ++i, e += 0xc, end += 0xc)
            if (obj && F<uint8_t*>(e, 0) == obj) {
                hook::Global<uint8_t*>(0x6acd48) = e + 4;
                hook::Global<int32_t>(0x6acd44) = F<int32_t>(e, 8);
                found = 1;
            }
        if (found == 1) {
            hook::Global<uint8_t*>(0x6acd4c) = t + end;
            return 1;
        }
    }
    if (obj) return found;
    hook::Global<uint8_t*>(0x6acd4c) = t + end;
    return 1;
}
// 0x459c30. Object in EBX, table in EDX (kept).
__declspec(naked) void EntryWalk_Find() {
    __asm {
        push edx
        push edx
        push ebx
        call EntryWalk_FindImpl
        add esp, 8
        pop edx
        ret
    }
}
RT3_REPLACE(0x459c30, EntryWalk_Find);

// 0x46d980 body. The index of the player-profile entry (table 0x6ceb9c, name +8)
// called name, or 0xff.
static int32_t __cdecl Profiles_IndexOfImpl(const char* name) {
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6ceb9c)); ++i) {
        Collection* t = hook::Global<Collection*>(0x6ceb9c);
        uint8_t* e = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (!Stricmp((const char*)e + 8, name)) return i;
    }
    return 0xff;
}
// 0x46d980. Name in EBX.
__declspec(naked) void Profiles_IndexOf() {
    __asm {
        push ebx
        call Profiles_IndexOfImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x46d980, Profiles_IndexOf);

// 0x46ed30 body. Whether a new company clashes with an existing one: 1 when a
// company has its player (+0x37 == p +4), 2 when one has its name (+4 == p +0x10).
static int32_t __cdecl Companies_ClashImpl(uint8_t* p) {
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x62be10)); ++i) {
        Collection* t = hook::Global<Collection*>(0x62be10);
        uint8_t* c = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
        if (F<int32_t>(c, 0x37) == F<int32_t>(p, 4)) return 1;
        if (!Stricmp((const char*)c + 4, (const char*)p + 0x10)) return 2;
    }
    return 0;
}
// 0x46ed30. Request in EBX.
__declspec(naked) void Companies_Clash() {
    __asm {
        push ebx
        call Companies_ClashImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x46ed30, Companies_Clash);

// 0x4a78d0 body. Walks a train's track segments (table 0x6cfca8, links +0x206 /
// +0x20a, node +0x202) from its current one (+0x356, direction +0x35a) for up to
// max steps and returns the steps taken when node target is reached, -1 otherwise.
// The node checked trails the walk by one segment, as in the original.
static int32_t __cdecl Train_StepsToNodeImpl(uint8_t* o, int32_t target, int32_t max) {
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, F<uint32_t>(o, 0x356));
    int32_t node = F<int32_t>(r, 0x202);
    int32_t flag = F<int32_t>(o, 0x35a);
    int32_t n = 0;
    for (int32_t i = 0; i < max; ++i) {
        if (node == target) return n;
        int32_t here = F<int32_t>(r, 0x202), a = F<int32_t>(r, 0x206), b = F<int32_t>(r, 0x20a);
        bool useA = i == 0 ? flag == 0 : b == node;
        int32_t next = useA ? a : b;
        if (next == -1) return -1;
        r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x6cfca8), 0, (uint32_t)next);
        ++n;
        node = here;
    }
    return -1;
}
// 0x4a78d0. Train in EAX, target and max on the stack.
__declspec(naked) void Train_StepsToNode() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call Train_StepsToNodeImpl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x4a78d0, Train_StepsToNode);

int32_t __fastcall Company_OwnerFlag1e1(void* c);
void __fastcall Company_AddStat(void* c, int, int32_t stat, float amount);
int32_t __fastcall Collection_IsValid(Collection* c, int, uint32_t id);
static inline int32_t Ftol(double v) {
    int32_t r;
    __asm {
        fld v
        mov eax, 0x5a10d0
        call eax
        mov r, eax
    }
    return r;
}

// 0x46f870 body. Applies a received cash transfer { float amount, byte company }:
// the amount (truncated) is booked as stat 0xf and taken from cash (stat 0xd) of
// a company that accepts it. slot is the original's local in the ECX push slot.
static void __cdecl Net_ApplyCashImpl(uint8_t* m, uint32_t* slot) {
    if (!Collection_IsValid(hook::Global<Collection*>(0x62be10), 0, m[4])) return;
    uint8_t* c = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be10), 0, m[4]);
    if (!c || !Company_OwnerFlag1e1(c)) return;
    int32_t v = Ftol(*(float*)m);
    float a = (float)v;
    Company_AddStat(c, 0, 0xf, a);
    float b = (float)-v;
    *slot = *(uint32_t*)&b;
    Company_AddStat(c, 0, 0xd, b);
}
// 0x46f870. Message in EAX; returns with ECX as its local slot holds.
__declspec(naked) void Net_ApplyCash() {
    __asm {
        push ecx
        push esp
        push eax
        call Net_ApplyCashImpl
        add esp, 8
        pop ecx
        ret
    }
}
RT3_REPLACE(0x46f870, Net_ApplyCash);

// 0x45ecd0 body. Classifies a scenario team name (uppercased in place first):
// 1 for string 0xdf3 or names starting like 0x5cbc64 / 0xdf3 (3 characters),
// else 4 for 0xdf4, 2 for 0xdf5, 0 otherwise.
static int32_t __cdecl TeamName_KindImpl(char* name) {
    ((char*(__cdecl*)(char*))0x5a2abe)(name);
    typedef int(__cdecl * StrnicmpFn)(const char*, const char*, size_t);
    StrnicmpFn strnicmp = (StrnicmpFn)0x5a5f26;
    if (!Stricmp(name, GetString(0xdf3))) return 1;
    if (!strnicmp(name, (const char*)0x5cbc64, 3) && !strnicmp(name, GetString(0xdf3), 3)) return 1;
    if (!Stricmp(name, GetString(0xdf4))) return 4;
    if (!Stricmp(name, GetString(0xdf5))) return 2;
    return 0;
}
// 0x45ecd0. Name in ESI.
__declspec(naked) void TeamName_Kind() {
    __asm {
        push esi
        call TeamName_KindImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x45ecd0, TeamName_Kind);

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }

// 0x45ee40 body. Unlinks a model from its hash bucket (index +0xc into the
// { head, tail, count } buckets at 0x6acd78) and, with recycle == 1, returns it to
// the model pool 0x6acd6c (or frees it when outside the pool). A model not in its
// bucket is left alone.
static void __cdecl Models_UnlinkImpl(uint8_t* m, int32_t recycle) {
    uint8_t* b = (uint8_t*)0x6acd78 + F<int32_t>(m, 0xc) * 0xc;
    uint8_t* head = F<uint8_t*>(b, 0);
    if (m == head) {
        uint8_t* next = F<uint8_t*>(head, 0);
        F<uint8_t*>(b, 0) = next;
        if (head == F<uint8_t*>(b, 4)) F<uint8_t*>(b, 4) = next;
        --F<int32_t>(b, 8);
    } else {
        if (!head) return;
        uint8_t* prev = head;
        for (;;) {
            uint8_t* cur = F<uint8_t*>(prev, 0);
            if (cur == m) break;
            prev = cur;
            if (!prev) return;
        }
        F<uint8_t*>(prev, 0) = F<uint8_t*>(m, 0);
        F<uint8_t*>(m, 0) = nullptr;
        --F<int32_t>(b, 8);
        if (m == F<uint8_t*>(b, 4)) F<uint8_t*>(b, 4) = prev;
    }
    if (recycle != 1) return;
    uint8_t* pool = hook::Global<uint8_t*>(0x6acd6c);
    if (F<int32_t>(pool, 0xc) && m >= F<uint8_t*>(pool, 4) && m < F<uint8_t*>(pool, 8)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        F<uint8_t*>(m, 0) = F<uint8_t*>(list, 0);
        F<uint8_t*>(list, 0) = m;
        ++F<int32_t>(list, 8);
        if (!F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = m;
    } else ExeDelete(m);
}
// 0x45ee40. Model in EAX, recycle flag in EDI.
__declspec(naked) void Models_Unlink() {
    __asm {
        push edi
        push eax
        call Models_UnlinkImpl
        add esp, 8
        ret
    }
}
RT3_REPLACE(0x45ee40, Models_Unlink);

// 0x413df0 body. Adds cell (x, y) with flag a to the dirty-cell list 0x62b318
// (-1 ended, count 0x62b7a4; entries ((x & 0x7fff) | a << 15) << 16 | (y & 0x7fff))
// unless (x << 16 | y) is already there, and widens the dirty block bounds
// (0x62b7a8..0x62b7b4, in 4-cell blocks). 1 when it was added.
static int32_t __cdecl DirtyCells_AddImpl(int32_t a, int32_t x, int32_t y) {
    uint32_t key = (uint32_t)x << 16 | (uint32_t)y;
    for (uint32_t* e = (uint32_t*)0x62b318; *e != 0xffffffff; ++e)
        if ((*e & 0x7fffffff) == key) return 0;
    int32_t& n = hook::Global<int32_t>(0x62b7a4);
    ((uint32_t*)0x62b318)[n] = ((uint32_t)(x & 0x7fff) | (uint32_t)a << 15) << 16 | (uint32_t)(y & 0x7fff);
    ++n;
    ((uint32_t*)0x62b318)[n] = 0xffffffff;
    int32_t by = y >> 2, bx = x >> 2;
    if (hook::Global<int32_t>(0x62b7a8) > by) hook::Global<int32_t>(0x62b7a8) = by;
    if (hook::Global<int32_t>(0x62b7ac) < by) hook::Global<int32_t>(0x62b7ac) = by;
    if (hook::Global<int32_t>(0x62b7b0) > bx) hook::Global<int32_t>(0x62b7b0) = bx;
    if (hook::Global<int32_t>(0x62b7b4) < bx) hook::Global<int32_t>(0x62b7b4) = bx;
    return 1;
}
// 0x413df0. Flag in EAX, x in ECX, y in EDX. Like the original, returns with ECX
// and EDX shifted right by 2 when the cell was added and unchanged otherwise.
__declspec(naked) void DirtyCells_Add() {
    __asm {
        push ecx
        push edx
        push edx
        push ecx
        push eax
        call DirtyCells_AddImpl
        add esp, 12
        pop edx
        pop ecx
        test eax, eax
        jz done
        sar edx, 2
        sar ecx, 2
    done:
        ret
    }
}
RT3_REPLACE(0x413df0, DirtyCells_Add);

// 0x45a700 body. Sums the scaled vectors of obj's attachments in the current walk
// table (from 0x6acd4c, count +8 of 0x6acd50, 0x168-byte entries; entries flagged
// 0x400 in +0x8c skipped): integer vector +0x34 times float vector +4 scaled by
// 0x5cb4ac for mode 0, else +0x70 times +0x40; into out.
static void __cdecl Attach_SumVectorsImpl(float* out, int32_t mode, int32_t obj) {
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    uint8_t* e = hook::Global<uint8_t*>(0x6acd4c);
    float scale = 1.0f;
    int32_t* iv;
    float* fv;
    if (mode == 0) {
        scale = *(float*)0x5cb4ac;
        iv = (int32_t*)(e + 0x34);
        fv = (float*)(e + 4);
    } else {
        iv = (int32_t*)(e + 0x70);
        fv = (float*)(e + 0x40);
    }
    for (int32_t i = 0; i < F<int32_t>(hook::Global<uint8_t*>(0x6acd50), 8); ++i) {
        if (F<int32_t>(e, 0) == obj && !(F<uint32_t>(e, 0x8c) & 0x400)) {
            float iz = (float)iv[2], iy = (float)iv[1], ix = (float)iv[0];
            float z = (float)((double)iz * fv[2] * scale);
            float y = (float)((double)iy * fv[1] * scale);
            float x = (float)((double)ix * fv[0] * scale);
            out[0] = (float)((double)x + out[0]);
            out[1] = (float)((double)y + out[1]);
            out[2] = (float)((double)z + out[2]);
        }
        iv = (int32_t*)((uint8_t*)iv + 0x168);
        fv = (float*)((uint8_t*)fv + 0x168);
        e += 0x168;
    }
}
// 0x45a700. Output in EAX, mode in ECX, object on the stack.
__declspec(naked) void Attach_SumVectors() {
    __asm {
        push [esp + 4]
        push ecx
        push eax
        call Attach_SumVectorsImpl
        add esp, 12
        ret 4
    }
}
RT3_REPLACE(0x45a700, Attach_SumVectors);

// 0x45f140. Offers record r (kind +0 3 or 4, model id +8) to the handlers chained in
// its model's hash bucket (0x6acd78): the first handler whose kind mask (+0x98)
// matches, whose variant (+0x94) equals r's flag variant (+0x20; ids 0x21..0x24 and
// 0x2c..0x2e fold variant 1 into 0) and that is allowed by the options (+4 needs
// option +0x28f, +8 needs 0x6d3b4c) is called (+0xa0, thiscall on r). 1 otherwise.
int32_t __fastcall Records_Dispatch(uint8_t* r) {
    uint32_t mask = (F<int32_t>(r, 0) == 4 ? 2 : 0) | (F<int32_t>(r, 0) == 3 ? 1 : 0);
    if (!mask) return 1;
    int32_t id = F<int32_t>(r, 8);
    if (id == 0xff) return 1;
    uint32_t variant = 1;
    for (uint8_t* n = *(uint8_t**)(0x6acd78 + id * 0xc); n; n = F<uint8_t*>(n, 0)) {
        if (!(F<uint32_t>(n, 0x98) & mask)) continue;
        uint32_t v = F<uint32_t>(r, 0x20);
        variant = ((v & 3) != 0 ? 1u : 0u) | (v >> 4 & 2) | (v & 4);
        if (((id >= 0x21 && id <= 0x24) || (id >= 0x2c && id <= 0x2e)) && variant == 1) variant = 0;
        if (variant != F<uint32_t>(n, 0x94)) continue;
        if (!F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x28f) && F<int32_t>(n, 4)) continue;
        if (hook::Global<int32_t>(0x6d3b4c) && !F<int32_t>(n, 8)) continue;
        void* fn = F<void*>(n, 0xa0);
        if (!fn) return 1;
        return ((int32_t(__fastcall*)(uint8_t*))fn)(r);
    }
    return 1;
}
RT3_REPLACE(0x45f140, Records_Dispatch);

// 0x459d00 body. For each of obj's up to 4 attachment ids (+0x98, 0-ended), sets
// value (+0x80) and v2 (+0x94) and clears flag 2 on the matching entries of the
// current walk table (from 0x6acd4c). An entry flagged 0x10 is only set (and ends
// the search) when no later entry with the id is still unflagged by 2. 1 when a
// plain entry was set for an obj flagged 0x10 (+0x8c).
static int32_t __cdecl Attach_SetValueImpl(int32_t value, uint8_t* obj, int32_t v2) {
    int32_t result = 0;
    for (int32_t k = 0; k < 4; ++k) {
        int32_t id = F<int32_t>(obj, 0x98 + k * 4);
        if (!id) break;
        uint8_t* e = hook::Global<uint8_t*>(0x6acd4c);
        for (int32_t i = 0; i < F<int32_t>(hook::Global<uint8_t*>(0x6acd50), 8); ++i, e += 0x168) {
            if (id != F<int32_t>(e, 0)) continue;
            uint32_t f = F<uint32_t>(e, 0x8c);
            if (f & 0x10) {
                int32_t n = F<int32_t>(hook::Global<uint8_t*>(0x6acd50), 8);
                bool later = false;
                uint8_t* e2 = e + 0x168;
                for (int32_t j = i + 1; j < n; ++j, e2 += 0x168)
                    if (F<int32_t>(e2, 0) == id && !(F<uint32_t>(e2, 0x8c) & 2)) {
                        later = true;
                        break;
                    }
                if (!later) {
                    F<uint32_t>(e, 0x8c) &= ~2u;
                    F<int32_t>(e, 0x80) = value;
                    F<int32_t>(e, 0x94) = v2;
                }
                break;
            }
            F<uint32_t>(e, 0x8c) = f & ~2u;
            F<int32_t>(e, 0x80) = value;
            F<int32_t>(e, 0x94) = v2;
            if (obj[0x8c] & 0x10) result = 1;
        }
    }
    return result;
}
// 0x459d00. Value in EAX, object and v2 on the stack.
__declspec(naked) void Attach_SetValue() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call Attach_SetValueImpl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x459d00, Attach_SetValue);

double __stdcall Distance2D(float x1, float y1, float x2, float y2);

// 0x415900 body. Whether any cell of the 4x4 blocks of rect { x0, x1, y0, y1 },
// grown by one block and clamped to the map, has map flag 1.
static int32_t __cdecl Map_RectHasFlag1Impl(const int32_t* r) {
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t bx0 = r[0] - 1 <= 0 ? 0 : r[0] - 1;
    int32_t bx1 = r[1] + 1 < F<int32_t>(m, 0x2155) - 1 ? r[1] + 1 : F<int32_t>(m, 0x2155) - 1;
    int32_t by0 = r[2] - 1 <= 0 ? 0 : r[2] - 1;
    int32_t by1 = r[3] + 1 < F<int32_t>(m, 0x2159) - 1 ? r[3] + 1 : F<int32_t>(m, 0x2159) - 1;
    for (int32_t by = by0; by <= by1; ++by)
        for (int32_t bx = bx0; bx <= bx1; ++bx)
            for (int32_t dy = 0; dy < 4; ++dy)
                for (int32_t dx = 0; dx < 4; ++dx)
                    if (Map_CellFlag(hook::Global<void*>(0x62c120), 0, dx + bx * 4, dy + by * 4, 1)) return 1;
    return 0;
}
// 0x415900. Rect in EAX.
__declspec(naked) void Map_RectHasFlag1() {
    __asm {
        push eax
        call Map_RectHasFlag1Impl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x415900, Map_RectHasFlag1);

// 0x414cd0 body. Distance (in blocks, cell distance / 4) from the centre of 4x4
// block (cx, cy) to the nearest cell around it (-4..3) with map flag 2, into *out
// (9999 when none); whether one was found. Cells on the map's first row or column
// are skipped.
static int32_t __cdecl Map_NearestFlag2Impl(float* out, int32_t cx, int32_t cy) {
    *out = 9999.0f;
    int32_t found = 0;
    for (int32_t dy = -4; dy < 4; ++dy)
        for (int32_t dx = -4; dx < 4; ++dx) {
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            int32_t x = dx + cx * 4, y = dy + cy * 4;
            if (x <= 0 || y <= 0 || x >= F<int32_t>(m, 0x215d) || y >= F<int32_t>(m, 0x2161)) continue;
            if (!Map_CellFlag(m, 0, x, y, 2)) continue;
            found = 1;
            float fy = (float)((double)dy + D(0x5c8628));
            float fx = (float)((double)dx + D(0x5c8628));
            double best = *out;
            double d = Distance2D(0.0f, 0.0f, fx, fy) / D(0x5c87b8);
            double v = d > best ? (double)*out : Distance2D(0.0f, 0.0f, fx, fy) / D(0x5c87b8);
            *out = (float)v;
        }
    return found;
}
// 0x414cd0. Output in EBX, block on the stack.
__declspec(naked) void Map_NearestFlag2() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push ebx
        call Map_NearestFlag2Impl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x414cd0, Map_NearestFlag2);

void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update);
void __fastcall Terrain_SetFlag(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void* __fastcall Engine_Renderer(void* e);

// 0x415820 body. Applies a dirty-cell list (list +0x10, or the global 0x62b318 for
// null; entries as in 0x413df0, -1 ended): sets each cell's fine bit (1 for flagged
// entries, else 2), marks each changed 4x4 block for redraw (terrain flag 0xf and
// block byte bit 1), counts the pass (0x62ba78) and marks the scene dirty. Returns
// the list end in *cursor (the original leaves it in ECX).
static int32_t __cdecl DirtyCells_ApplyImpl(uint8_t* list, uint32_t** cursor) {
    int32_t lastX = -1, lastY = -1;
    ++hook::Global<int32_t>(0x62ba78);
    uint32_t* e = list ? (uint32_t*)(list + 0x10) : (uint32_t*)0x62b318;
    *cursor = e;
    for (; *e != 0xffffffff; *cursor = ++e) {
        uint32_t v = *e;
        int32_t x = (int32_t)(v & 0x7fff), y = (int32_t)(v >> 16 & 0x7fff);
        Map_SetFineBit(hook::Global<uint8_t*>(0x62c120), 0, x, y, 1, (int32_t)v < 0 ? 1 : 2, 0);
        int32_t bx = x >> 2, by = y >> 2;
        if (bx == lastX && by == lastY) continue;
        Terrain_SetFlag(hook::Global<uint8_t*>(0x62c120), 0, bx, by, 0xf);
        uint8_t* m = hook::Global<uint8_t*>(0x62c120);
        F<uint8_t*>(m, 0x2131)[F<int32_t>(m, 0x2155) * by + bx] |= 1;
        lastX = bx;
        lastY = by;
    }
    ((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)))[0x374a] = 1;
    return 1;
}
// 0x415820. List in EAX.
__declspec(naked) void DirtyCells_Apply() {
    __asm {
        push ecx
        push esp
        push eax
        call DirtyCells_ApplyImpl
        add esp, 8
        pop ecx
        ret
    }
}
RT3_REPLACE(0x415820, DirtyCells_Apply);

void __fastcall Terrain_SetCellByte(uint8_t* t, int, int32_t x, int32_t y, uint8_t v);
void __fastcall Terrain_SetCellCount(uint8_t* t, int, int32_t x, int32_t y, uint8_t n);
void __fastcall Terrain_AddPoints(uint8_t* t, int, int32_t x, int32_t z, uint8_t kind, int32_t count);

// 0x414ee0 body. Restores the terrain cells of rect { x0, x1, y0, y1 } (row by row)
// from saved bytes: flags (low nibble) and counts (high nibble) from src, the cell
// byte from src2; 0xff entries and cells outside the map are skipped. Marks the
// scene dirty.
static void __cdecl Terrain_RestoreRectImpl(const uint8_t* src, const int32_t* r, const uint8_t* src2) {
    for (int32_t y = r[2]; y <= r[3]; ++y)
        for (int32_t x = r[0]; x <= r[1]; ++x, ++src, ++src2) {
            uint8_t b = *src;
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            if (b == 0xff || x < 0 || x >= F<int32_t>(m, 0x15d9) - 1 || y < 0 || y >= F<int32_t>(m, 0x15d5) - 1) continue;
            Terrain_SetFlag(m, 0, x, y, (uint8_t)(b & 0x0f));
            Terrain_SetCellByte(hook::Global<uint8_t*>(0x62c120), 0, x, y, *src2);
            Terrain_SetCellCount(hook::Global<uint8_t*>(0x62c120), 0, x, y, (uint8_t)(*src >> 4));
        }
    ((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)))[0x374a] = 1;
}
// 0x414ee0. Source in EAX, rect and second source on the stack.
__declspec(naked) void Terrain_RestoreRect() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push eax
        call Terrain_RestoreRectImpl
        add esp, 12
        ret 8
    }
}
RT3_REPLACE(0x414ee0, Terrain_RestoreRect);

// 0x4156b0 body. Stamps a building footprint into the terrain: for each cell of the
// 8x8 shape (0x5edd30, by shape +0x732; -1 none) at block (+0x4a0, +0x4a8) that is
// not map flag 0, records owner in the cell-owner map 0x62b300 (allocated zeroed on
// first use) and adds terrain points of kind (type index of +0x736) * 7 + 6, 4 of
// them for type kind 3, else 999. Then marks the scene dirty and refreshes the view.
static void __cdecl Footprint_StampImpl(uint8_t* o, int32_t owner) {
    if (!hook::Global<int32_t*>(0x62b300)) {
        uint8_t* m = hook::Global<uint8_t*>(0x62c120);
        size_t n = (size_t)(F<int32_t>(m, 0x2159) * F<int32_t>(m, 0x2155) * 4);
        void* p = ((void*(__cdecl*)(size_t))0x5a125d)(n);
        hook::Global<void*>(0x62b300) = p;
        memset(p, 0, n);
    }
    if (!o) o = (uint8_t*)0x62b308;
    if (F<int32_t>(o, 0x732) == -1) return;
    int32_t kind = (int32_t)(F<uint32_t>(o, 0x736) - 0x5ef3e0) / 0xb0 * 7 + 6;
    for (int32_t dy = 0; dy < 8; ++dy) {
        int32_t y = F<int32_t>(o, 0x4a8) + dy;
        for (int32_t dx = 0; dx < 8; ++dx) {
            int32_t x = F<int32_t>(o, 0x4a0) + dx;
            if (!*(uint8_t*)(0x5edd30 + (dy + F<int32_t>(o, 0x732) * 8) * 8 + dx)) continue;
            if (Map_CellFlag(hook::Global<void*>(0x62c120), 0, x * 4, y * 4, 0)) continue;
            uint8_t* m = hook::Global<uint8_t*>(0x62c120);
            hook::Global<int32_t*>(0x62b300)[F<int32_t>(m, 0x2155) * y + x] = owner;
            Terrain_AddPoints(hook::Global<uint8_t*>(0x62c120), 0, x, y, (uint8_t)kind, F<int32_t>(F<uint8_t*>(o, 0x736), 4) == 3 ? 4 : 0x3e7);
        }
    }
    ((uint8_t*)Engine_Renderer(hook::Global<void*>(0x6d4024)))[0x374a] = 1;
    void* view = hook::Global<void*>(0x62be68);
    ((void(__fastcall*)(void*))(*(void***)view)[0])(view);
}
// 0x4156b0. Object in EAX, owner on the stack.
__declspec(naked) void Footprint_Stamp() {
    __asm {
        push [esp + 4]
        push eax
        call Footprint_StampImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x4156b0, Footprint_Stamp);

void __fastcall StreamWrite(int32_t s, const void* data, uint32_t size, int32_t);
void __fastcall StreamWriteU32(int32_t s, uint32_t value);

// 0x441ec0 body. Saves the 0x3c2-byte block data as: version (0x620e94), marker
// 0x2ee0, the block, marker 0x2ee1 - to file f from its start, or to stream s
// when f is null.
static void __cdecl Block3c2_SaveImpl(void* f, const uint8_t* data, int32_t s) {
    uint32_t v = hook::Global<uint32_t>(0x620e94);
    if (f) {
        typedef size_t(__cdecl * FwriteFn)(const void*, size_t, size_t, void*);
        FwriteFn fwrite_ = (FwriteFn)0x5a276f;
        ((int(__cdecl*)(void*, long, int))0x5a261f)(f, 0, 0);
        fwrite_(&v, 4, 1, f);
        v = 0x2ee0;
        fwrite_(&v, 4, 1, f);
        fwrite_(data, 0x3c2, 1, f);
        v = 0x2ee1;
        fwrite_(&v, 4, 1, f);
        return;
    }
    StreamWrite(s, &v, 4, 0);
    StreamWriteU32(s, 0x2ee0);
    StreamWrite(s, data, 0x3c2, 0);
    StreamWriteU32(s, 0x2ee1);
}
// 0x441ec0. File in ESI, block in EBX, stream in EDI.
__declspec(naked) void Block3c2_Save() {
    __asm {
        push edi
        push ebx
        push esi
        call Block3c2_SaveImpl
        add esp, 12
        ret
    }
}
RT3_REPLACE(0x441ec0, Block3c2_Save);

// 0x45ea90 body. The display name of record rec into out: the qualifier words of
// its flags +0x94 (4: string 0xdef, 2: 0xdf0, 1: 0xdf1) followed by its type's name
// from the table 0x5f0a34 (by value +0xc; localized string or fallback). Empty for
// type 0xff; only the qualifiers for an unknown type.
static void __cdecl Record_DisplayNameImpl(char* out, uint8_t* rec) {
    out[0] = 0;
    if (F<int32_t>(rec, 0xc) == 0xff) return;
    if (F<uint8_t>(rec, 0x94) & 4) strcat(out, GetString(0xdef));
    if (F<uint8_t>(rec, 0x94) & 2) strcat(out, GetString(0xdf0));
    if (F<uint8_t>(rec, 0x94) & 1) strcat(out, GetString(0xdf1));
    for (uint32_t i = 0; i < 0x5b; ++i) {
        uint8_t* e = (uint8_t*)0x5f0a34 + i * 0x10;
        if (F<int32_t>(rec, 0xc) != F<int32_t>(e, 0)) continue;
        int32_t id = F<int32_t>(e, 4);
        strcat(out, id && (uint8_t)HasString(id) ? GetString(F<int32_t>(e, 4)) : F<const char*>(e, 8));
        return;
    }
}
// 0x45ea90. Output in EAX, record on the stack.
__declspec(naked) void Record_DisplayName() {
    __asm {
        push [esp + 4]
        push eax
        call Record_DisplayNameImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x45ea90, Record_DisplayName);

void __fastcall Str_TrimRight(char* s);
void __fastcall Str_TrimLeft(char* s);
char* __fastcall StrCopy(char* dst, const char* src);
static inline char* StrChr(const char* s, int c) { return ((char*(__cdecl*)(const char*, int))0x5a2a00)(s, c); }

// 0x557330 body. Reduces s to the trimmed text between the open and close
// characters (0x6257e0 / 0x6257e4, '(' and ')'); 0 when either is missing.
static int32_t __cdecl Str_InnerParensImpl(char* s) {
    char* open = StrChr(s, hook::Global<int32_t>(0x6257e0));
    char* close = StrChr(s, hook::Global<int32_t>(0x6257e4));
    if (!open || !close) return 0;
    ++open;
    *close = 0;
    Str_TrimLeft(open);
    Str_TrimRight(open);
    if (open != s) StrCopy(s, open);
    return 1;
}
// 0x557330. String in EDI.
__declspec(naked) void Str_InnerParens() {
    __asm {
        push edi
        call Str_InnerParensImpl
        add esp, 4
        ret
    }
}
RT3_REPLACE(0x557330, Str_InnerParens);

// 0x557390 body. Splits "key = value" into out +4 (key) and +0x104 (value), both
// trimmed; 0 without '=' or with an empty key.
static int32_t __cdecl Str_SplitKeyValueImpl(const char* line, uint8_t* out) {
    const char* eq = StrChr(line, '=');
    if (!eq) return 0;
    char key[0x100], value[0x100];
    size_t n = (size_t)(eq - line);
    strncpy(key, line, n);
    key[n] = 0;
    Str_TrimLeft(key);
    Str_TrimRight(key);
    if (!strlen(key)) return 0;
    strcpy(value, eq + 1);
    Str_TrimLeft(value);
    Str_TrimRight(value);
    strcpy((char*)out + 4, key);
    strcpy((char*)out + 0x104, value);
    return 1;
}
// 0x557390. Line in EDI, output on the stack.
__declspec(naked) void Str_SplitKeyValue() {
    __asm {
        push [esp + 4]
        push edi
        call Str_SplitKeyValueImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x557390, Str_SplitKeyValue);

// 0x45f400. The value (+0xc) of the model called name, or 0.
RT3_NOINLINE int32_t __fastcall Models_ValueByName(const char* name) {
    uint8_t* m = Models_FindByNameImpl(name);
    return m ? F<int32_t>(m, 0xc) : 0;
}
RT3_REPLACE(0x45f400, Models_ValueByName);

// 0x45f3d0. The display name of the model called name into out; 0 when unknown.
RT3_NOINLINE int32_t __fastcall Models_DisplayName(const char* name, char* out) {
    uint8_t* m = Models_FindByNameImpl(name);
    if (!m) return 0;
    Record_DisplayNameImpl(out, m);
    return 1;
}
RT3_REPLACE(0x45f3d0, Models_DisplayName);

// 0x45b4f0. Clears object o's attachment entries for walk slot (+0x256 table, +0x75
// needed): one slot, or for -1 the first of slots 0..3 that clears one. 0 when o has
// no table.
RT3_NOINLINE int32_t __fastcall Obj_ClearAttachSlot(uint8_t* o, int, int32_t slot) {
    if (!F<void*>(o, 0x256) || !F<int32_t>(o, 0x75)) return 0;
    if (slot != -1) return EntryWalk_BeginImpl(F<uint8_t*>(o, 0x256), slot);
    int32_t r = 0;
    for (int32_t i = 0; i < 4; ++i)
        if ((r = EntryWalk_BeginImpl(F<uint8_t*>(o, 0x256), i)) == 1) break;
    return r;
}
RT3_REPLACE(0x45b4f0, Obj_ClearAttachSlot);

uint8_t* __stdcall Textures_Find(int32_t key);
int32_t __stdcall Profiles_IndexOfEntry(int32_t key);

// 0x46f8f0 body. Company number i (by position in 0x62be10) when it is a human
// player's company whose profile is the one of texture entry key; else null.
static uint8_t* __cdecl Companies_ForEntryImpl(int32_t i, int32_t key) {
    Collection* t = hook::Global<Collection*>(0x62be10);
    uint8_t* c = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, (uint32_t)i, 0));
    if (!c || !Company_OwnerFlag1e1(c)) return nullptr;
    int32_t idx = Profiles_IndexOfEntry(key);
    if (idx == 0xff) return nullptr;
    uint32_t id = Collection_IdAt(hook::Global<Collection*>(0x6ceb9c), 0, (uint32_t)idx, 0);
    if (id == 0xffffffff || id == 0) return nullptr;
    return F<uint32_t>(c, 0x3b) == id ? c : nullptr;
}
// 0x46f8f0. Position in EAX, key on the stack.
__declspec(naked) void Companies_ForEntry() {
    __asm {
        push [esp + 4]
        push eax
        call Companies_ForEntryImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x46f8f0, Companies_ForEntry);

struct MaxHeap;
int32_t __fastcall Heap_PopMax(MaxHeap* h);
struct HeapNode;
HeapNode* __fastcall Heap_Push(MaxHeap* h, int, int32_t key, int32_t value);
void __fastcall Heap_Init(MaxHeap* h, int, int32_t capacity);
void __fastcall Heap_Destroy2(MaxHeap* h);
static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }

// 0x45f250. The next model in list order (by priority +0x9c, smallest first; hidden
// ones only with option +0x28f): its display name, file name and two values in
// the static record 0x6ad978 (returned); null at the end, which also resets the
// walk (0x5f0fe0, heap 0x6acd74).
RT3_NOINLINE uint8_t* Models_NextInOrder() {
    if (hook::Global<int32_t>(0x5f0fe0) == 1) {
        hook::Global<int32_t>(0x5f0fe0) = 0;
        uint32_t* h = (uint32_t*)ExeNew(0x1c);
        if (h) h[0] = h[1] = h[2] = h[3] = 0;
        hook::Global<void*>(0x6acd74) = h;
        Heap_Init((MaxHeap*)h, 0, 0x400);
        for (uint32_t b = 0x6acd78; b <= 0x6ad96c; b += 0xc)
            for (uint8_t* m = *(uint8_t**)b; m; m = F<uint8_t*>(m, 0))
                if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x28f) || !F<int32_t>(m, 4))
                    Heap_Push(hook::Global<MaxHeap*>(0x6acd74), 0, 0x7ffffffe - F<int32_t>(m, 0x9c), (int32_t)(uintptr_t)m);
    }
    uint8_t* m = (uint8_t*)(uintptr_t)Heap_PopMax(hook::Global<MaxHeap*>(0x6acd74));
    if (m) {
        Record_DisplayNameImpl((char*)0x6ad980, m);
        strcpy((char*)0x6ada00, (const char*)m + 0x10);
        hook::Global<int32_t>(0x6ad978) = F<int32_t>(m, 4);
        hook::Global<int32_t>(0x6ad97c) = F<int32_t>(m, 8);
        return (uint8_t*)0x6ad978;
    }
    if (MaxHeap* h = hook::Global<MaxHeap*>(0x6acd74)) {
        Heap_Destroy2(h);
        ExeDelete(h);
    }
    hook::Global<void*>(0x6acd74) = nullptr;
    hook::Global<int32_t>(0x5f0fe0) = 1;
    return nullptr;
}
RT3_REPLACE(0x45f250, Models_NextInOrder);

// 0x45f550. Writes the hotkey file path: the comment header (strings 0xdf6..0xdfc),
// "!Hotkeys~", then "file = name" for every shown model in list order, the file
// name padded to 32 columns. 0 when the file cannot be created.
RT3_NOINLINE int32_t __fastcall Hotkeys_Save(const char* path) {
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5cbc80);
    if (!f) return 0;
    int(__cdecl* const print)(void*, const char*, ...) = (int(__cdecl*)(void*, const char*, ...))0x5a2bd2;
    for (int32_t id = 0xdf6; id <= 0xdfc; ++id) print(f, GetString(id));
    print(f, (const char*)0x5cbc74);
    for (uint8_t* r = Models_NextInOrder(); r; r = Models_NextInOrder()) {
        char line[0x100];
        memset(line, 0, sizeof line);
        strcpy(line, (const char*)r + 0x88);
        int32_t n = (int32_t)strlen(line);
        if (n < 0x20) memset(line + n, ' ', 0x20 - n);
        if (F<int32_t>(r, 0) == 0) print(f, (const char*)0x5cbc68, line, (const char*)r + 8);
    }
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return 1;
}
RT3_REPLACE(0x45f550, Hotkeys_Save);

struct NodePool;
void __fastcall Pool_Free(NodePool* p);
void __fastcall Hash_DestroyThunk(HashMap* m);

// 0x461070. Shuts the model tables down: with a model pool 0x6acd6c the hotkeys
// are saved first (0x5cbc84), then the pool (its block +4), the name map 0x6acd70,
// the buckets 0x6acd78..0x6ad977 and the list-order heap 0x6acd74 are released.
void Models_Shutdown() {
    if (hook::Global<uint8_t*>(0x6acd6c)) {
        Hotkeys_Save((const char*)0x5cbc84);
        if (uint8_t* pool = hook::Global<uint8_t*>(0x6acd6c)) {
            if (void* b = F<void*>(pool, 4)) ((void(__cdecl*)(void*))0x5a1145)(b);
            pool = hook::Global<uint8_t*>(0x6acd6c);
            if (pool) {
                Pool_Free((NodePool*)pool);
                ExeDelete(pool);
            }
            hook::Global<uint8_t*>(0x6acd6c) = nullptr;
        }
    }
    if (HashMap* m = hook::Global<HashMap*>(0x6acd70)) {
        Hash_DestroyThunk(m);
        ExeDelete(m);
        hook::Global<HashMap*>(0x6acd70) = nullptr;
    }
    for (uint32_t b = 0x6acd78; b <= 0x6ad96c; b += 0xc) memset((void*)b, 0, 0xc);
    if (MaxHeap* h = hook::Global<MaxHeap*>(0x6acd74)) {
        Heap_Destroy2(h);
        ExeDelete(h);
        hook::Global<MaxHeap*>(0x6acd74) = nullptr;
    }
}
RT3_REPLACE(0x461070, Models_Shutdown);

void __stdcall Obj_SyncGroupFlags(uint8_t* o, int32_t full);
uint8_t* __fastcall BlockIter_NextSameKey(uint8_t* b);
void __fastcall Obj_SwapBlockFlags(void*, int, uint8_t* o);

// 0x45a0d0 body. Finishes one side (1: the +0x64 group, else +0x28) of a fully
// flagged (0x380) block entry o: drops the flags whose side counters are set,
// clears the counters, flips the direction (+0x88 / +0x84) of a reversing entry
// (bit 1), then syncs groups (0x4000), marks it taken (2), passes value and
// level +0x90 * 100 on to its partner (0x10) or itself, and to attachments
// (0x20), swaps block flags (0x1000) and notifies the table owner (0x800). Bit 0
// of the result: this entry (or the earlier partner) took the level; bit 1: an
// attachment took it.
static int32_t __cdecl Block_FinishSideImpl(uint8_t* o, int32_t side) {
    uint32_t flags = F<uint32_t>(o, 0x8c);
    int32_t result = 0;
    if ((flags & 0x380) != 0x380) return 0;
    int32_t level = F<int32_t>(o, 0x90) * 100;
    if (side == 1) {
        if (F<int32_t>(o, 0x70)) F<uint32_t>(o, 0x8c) = flags & ~0x80u;
        if (F<int32_t>(o, 0x74)) F<uint32_t>(o, 0x8c) &= ~0x100u;
        if (F<int32_t>(o, 0x78)) F<uint32_t>(o, 0x8c) &= ~0x200u;
        flags = F<uint32_t>(o, 0x8c);
        F<int32_t>(o, 0x64) = 0;
        F<int32_t>(o, 0x68) = 0;
        F<int32_t>(o, 0x6c) = 0;
        if (flags & 1) F<int32_t>(o, 0x88) = -F<int32_t>(o, 0x88);
    } else {
        if (F<int32_t>(o, 0x34)) F<uint32_t>(o, 0x8c) = flags & ~0x80u;
        if (F<int32_t>(o, 0x38)) F<uint32_t>(o, 0x8c) &= ~0x100u;
        if (F<int32_t>(o, 0x3c)) F<uint32_t>(o, 0x8c) &= ~0x200u;
        flags = F<uint32_t>(o, 0x8c);
        F<int32_t>(o, 0x28) = 0;
        F<int32_t>(o, 0x2c) = 0;
        F<int32_t>(o, 0x30) = 0;
        if (flags & 1) F<int32_t>(o, 0x84) = -F<int32_t>(o, 0x84);
    }
    if (flags & 0x4000) Obj_SyncGroupFlags(o, (int32_t)(flags & 0x10000));
    if (F<uint32_t>(o, 0x8c) & 4) F<uint32_t>(o, 0x8c) |= 2;
    flags = F<uint32_t>(o, 0x8c);
    if (flags & 0x10) {
        F<uint32_t>(o, 0x8c) = flags | 2;
        uint8_t* q = BlockIter_NextSameKey(o);
        F<int32_t>(q, 0x94) = level;
        F<int32_t>(q, 0x80) = F<int32_t>(o, 0x80);
        if (q < o) {
            if (!(F<uint32_t>(q, 0x8c) & 0x40)) F<uint32_t>(q, 0x8c) &= ~2u;
            result = 1;
        } else F<uint32_t>(q, 0x8c) &= ~2u;
    } else {
        if (!(flags & 1)) result = 1;
        F<int32_t>(o, 0x94) = level;
    }
    if ((F<uint8_t>(o, 0x8c) & 0x20) && Attach_SetValueImpl(F<int32_t>(o, 0x80), o, level)) result |= 2;
    if (F<uint32_t>(o, 0x8c) & 0x1000) Obj_SwapBlockFlags(nullptr, 0, o);
    if (F<uint32_t>(o, 0x8c) & 0x800) ObjFlags_SetImpl(0x800, hook::Global<uint8_t*>(0x6acd54), 0);
    return result;
}
// 0x45a0d0. Entry in ESI, side on the stack.
__declspec(naked) void Block_FinishSide() {
    __asm {
        push [esp + 4]
        push esi
        call Block_FinishSideImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x45a0d0, Block_FinishSide);

void* __fastcall RegKey_Construct(uint32_t* o, int, const char* subKey, void* root);
const char* __fastcall RegKey_GetString(void* o, int, const char* subKey, const char* name, const char* def);
uint32_t __fastcall HandleObj_Destruct(void* o);
const char* __fastcall StrFind(const char* hay, const char* needle, int32_t caseSensitive);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4, const char* a5,
                            const char* a6, const char* a7, const char* a8);

// 0x501f20 body. The command line (in 0x6d14d8) that opens target with the
// registered handler: kind 0 the web browser (HKCR http\shell\open\command; a
// Netscape command with a "%1" placeholder gets it filled in, any other has
// target appended), kind 1 the rtf viewer (rtffile, cut at its "%1, then
// appended). Null when no handler is registered.
static char* __cdecl Shell_OpenCommandImpl(int32_t kind, const char* target) {
    char* out = (char*)0x6d14d8;
    const char* key = kind == 0 ? (const char*)0x5d1634 : kind == 1 ? (const char*)0x5d1650 : nullptr;
    if (!key) return nullptr;
    uint32_t reg[3];
    RegKey_Construct(reg, 0, key, (void*)0x80000000);
    char* cmd = (char*)RegKey_GetString(reg, 0, (const char*)0x5d1648, (const char*)0x5c87a8, nullptr);
    if (!cmd) {
        HandleObj_Destruct(reg);
        return nullptr;
    }
    if (kind == 0 && StrFind(cmd, (const char*)0x5d162c, 0) && StrFind(cmd, (const char*)0x5ce074, 0)) {
        FormatText(out, cmd, target, 0, 0, 0, 0, 0, 0, 0);
    } else {
        if (kind == 1) {
            char* q = ((char*(__cdecl*)(const char*, const char*))0x5a1ff0)(cmd, (const char*)0x5d1644);
            if (q) *q = 0;
        }
        ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)(out, (const char*)0x5ce184, cmd, target);
    }
    HandleObj_Destruct(reg);
    return out;
}
// 0x501f20. Kind in EAX, target in EDI.
__declspec(naked) void Shell_OpenCommand() {
    __asm {
        push edi
        push eax
        call Shell_OpenCommandImpl
        add esp, 8
        ret
    }
}
RT3_REPLACE(0x501f20, Shell_OpenCommand);

uint32_t __fastcall LookupSingleFlag(int32_t key, int32_t mask);

// 0x45ef00 body. Puts a copy of the model called name (taken from the model pool
// 0x6acd6c) at the head of bucket slot under key (+0x94); the original record is
// unlinked and recycled, and an older record of the slot with the same key goes
// to the free list (bucket 0xff). Needs slot to be 0xff or a known flag for key.
// 1 when done.
static int32_t __cdecl Models_AddCopyImpl(int32_t slot, int32_t key, const char* name) {
    if (slot != 0xff && !LookupSingleFlag(slot, key)) return 0;
    uint8_t* src = Models_FindByNameImpl(name);
    if (!src) return 0;
    uint8_t* pool = hook::Global<uint8_t*>(0x6acd6c);
    uint8_t* m;
    if (F<int32_t>(pool, 0xc)) {
        uint8_t* list = F<uint8_t*>(pool, 0);
        m = F<uint8_t*>(list, 0);
        if (m) {
            F<uint8_t*>(list, 0) = F<uint8_t*>(m, 0);
            if (m == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = F<uint8_t*>(m, 0);
            --F<int32_t>(list, 8);
        }
    } else m = (uint8_t*)ExeNew(F<uint32_t>(pool, 0x10));
    F<int32_t>(m, 0xc) = slot;
    F<int32_t>(m, 0x94) = key;
    F<int32_t>(m, 0x98) = F<int32_t>(src, 0x98);
    F<int32_t>(m, 0xa0) = F<int32_t>(src, 0xa0);
    F<int32_t>(m, 0x9c) = F<int32_t>(src, 0x9c);
    F<int32_t>(m, 0x90) = F<int32_t>(src, 0x90);
    F<int32_t>(m, 4) = F<int32_t>(src, 4);
    F<int32_t>(m, 8) = F<int32_t>(src, 8);
    strcpy((char*)m + 0x10, (const char*)src + 0x10);
    Models_UnlinkImpl(src, 1);
    uint8_t* b = (uint8_t*)0x6acd78 + slot * 0xc;
    if (slot != 0xff) {
        for (uint8_t* o = F<uint8_t*>(b, 0); o; o = F<uint8_t*>(o, 0)) {
            if (F<int32_t>(o, 0x94) != key) continue;
            Models_UnlinkImpl(o, 0);
            F<int32_t>(o, 0xc) = 0xff;
            F<uint8_t*>(o, 0) = hook::Global<uint8_t*>(0x6ad96c);
            ++hook::Global<int32_t>(0x6ad974);
            hook::Global<uint8_t*>(0x6ad96c) = o;
            if (!hook::Global<uint8_t*>(0x6ad970)) hook::Global<uint8_t*>(0x6ad970) = o;
            break;
        }
    }
    F<uint8_t*>(m, 0) = F<uint8_t*>(b, 0);
    F<uint8_t*>(b, 0) = m;
    ++F<int32_t>(b, 8);
    if (!F<uint8_t*>(b, 4)) F<uint8_t*>(b, 4) = m;
    return 1;
}
// 0x45ef00. Slot in ECX, key in EDX, name on the stack; leaves ECX = key.
__declspec(naked) void Models_AddCopy() {
    __asm {
        push edx
        push [esp + 8]
        push edx
        push ecx
        call Models_AddCopyImpl
        add esp, 12
        pop ecx
        ret 4
    }
}
RT3_REPLACE(0x45ef00, Models_AddCopy);

// 0x45a280 body. One side of entry e (mask: its flag 0x80 / 0x100 / 0x200): while
// counter c (+0x24) is below its limit (+0x30) the flag is cleared and c counts up,
// and value passes through; at the limit the flag is set (once, finishing the
// side into *result) and 0 comes back. A null c passes value through. Returns the
// float's bits, which the entry stores back over value.
static uint32_t __cdecl Block_CountSideImpl(uint8_t* c, uint8_t* e, uint32_t mask, uint32_t* result, uint32_t value, int32_t side) {
    if (!c) return value;
    if (F<int32_t>(c, 0x24) < F<int32_t>(c, 0x30)) {
        F<uint32_t>(e, 0x8c) &= ~mask;
        ++F<int32_t>(c, 0x24);
        return value;
    }
    uint32_t flags = F<uint32_t>(e, 0x8c);
    if (!(flags & mask)) {
        F<uint32_t>(e, 0x8c) = flags | mask;
        *result |= (uint32_t)Block_FinishSideImpl(e, side);
    }
    return 0;
}
// 0x45a280. Counter in EAX, entry in EDX, mask in ECX, result in EDI; value
// (float) and side on the stack; the float comes back in ST0.
__declspec(naked) void Block_CountSide() {
    __asm {
        push [esp + 8]
        push [esp + 8]
        push edi
        push ecx
        push edx
        push eax
        call Block_CountSideImpl
        add esp, 24
        mov [esp + 4], eax
        fld dword ptr [esp + 4]
        ret 8
    }
}
RT3_REPLACE(0x45a280, Block_CountSide);

void __fastcall Holder_Alloc(uint8_t* h);
void __fastcall Pool_Release(int32_t* p, int, void** node);
const char* __fastcall GetString(int32_t id);

// The node pop of a pool { list*, lo, hi, enabled, size } (heap when disabled).
static uint8_t* Config_Take(uint8_t* pool) {
    if (!F<int32_t>(pool, 0xc)) return (uint8_t*)ExeNew(F<uint32_t>(pool, 0x10));
    uint8_t* list = F<uint8_t*>(pool, 0);
    uint8_t* n = F<uint8_t*>(list, 0);
    if (n) {
        F<uint8_t*>(list, 0) = F<uint8_t*>(n, 0);
        if (n == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = F<uint8_t*>(n, 0);
        --F<int32_t>(list, 8);
    }
    return n;
}

// 0x557630. Loads the config file path into c: "[section]" lines (records of
// 0x114 bytes from the pool +4, kept in file order in the list +8; entries list
// +0x108 / +0x10c / count +0x110, total +0x104) and "key = value" lines (0x204-
// byte records from the pool +0, appended to the current section); ';' starts a
// comment and blank lines are skipped. +0x114 counts the lines read. On an error
// (no file 0x27, entry outside a section 0x25, bad entry 0x26) its text goes to
// +0x14 and 0 comes back, else 1.
int32_t __fastcall Config_Load(uint8_t* c, int, const char* path) {
    Holder_Alloc(c);
    void* f = ((void*(__cdecl*)(const char*, const char*))0x5a1887)(path, (const char*)0x5ce2c0);
    if (!f) {
        strcpy((char*)c + 0x14, GetString(0x27));
        return 0;
    }
    uint8_t* section = nullptr;
    uint8_t* entry = nullptr;
    char line[0x100];
    while (!(F<uint8_t>(f, 0xc) & 0x10)) {
        if (!((char*(__cdecl*)(char*, int, void*))0x5a3fe0)(line, 0x100, f)) continue;
        if (char* semi = ((char*(__cdecl*)(const char*, int))0x5a2a00)(line, ';')) *semi = 0;
        const char* p = line;
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p;
        if (*p) {
            if (Str_InnerParensImpl(line)) {
                uint8_t* prev = section;
                section = Config_Take(F<uint8_t*>(c, 4));
                memset(section, 0, 0x114);
                F<int32_t>(section, 0x108) = 0;
                F<int32_t>(section, 0x10c) = 0;
                F<int32_t>(section, 0x110) = 0;
                F<int32_t>(section, 0x104) = 0;
                strcpy((char*)section + 4, line);
                uint8_t* list = F<uint8_t*>(c, 8);
                if (prev) {
                    F<uint8_t*>(section, 0) = F<uint8_t*>(prev, 0);
                    F<uint8_t*>(prev, 0) = section;
                    ++F<int32_t>(list, 8);
                    if (prev == F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = section;
                } else {
                    F<uint8_t*>(section, 0) = F<uint8_t*>(list, 0);
                    ++F<int32_t>(list, 8);
                    F<uint8_t*>(list, 0) = section;
                    if (!F<uint8_t*>(list, 4)) F<uint8_t*>(list, 4) = section;
                }
                entry = nullptr;
            } else {
                if (!section) {
                    strcpy((char*)c + 0x14, GetString(0x25));
                    break;
                }
                uint8_t* prev = entry;
                entry = Config_Take(F<uint8_t*>(c, 0));
                memset(entry, 0, 0x204);
                if (!Str_SplitKeyValueImpl(line, entry)) {
                    Pool_Release(F<int32_t*>(c, 0), 0, (void**)entry);
                    strcpy((char*)c + 0x14, GetString(0x26));
                    break;
                }
                if (prev) {
                    F<uint8_t*>(entry, 0) = F<uint8_t*>(prev, 0);
                    F<uint8_t*>(prev, 0) = entry;
                    ++F<int32_t>(section, 0x110);
                    if (prev == F<uint8_t*>(section, 0x10c)) F<uint8_t*>(section, 0x10c) = entry;
                } else {
                    F<uint8_t*>(entry, 0) = F<uint8_t*>(section, 0x108);
                    ++F<int32_t>(section, 0x110);
                    F<uint8_t*>(section, 0x108) = entry;
                    if (!F<uint8_t*>(section, 0x10c)) F<uint8_t*>(section, 0x10c) = entry;
                }
                ++F<int32_t>(section, 0x104);
            }
        }
        ++F<int32_t>(c, 0x114);
    }
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return c[0x14] ? 0 : 1;
}
RT3_REPLACE(0x557630, Config_Load);

char* __fastcall CopyBaseName(const char* src, char* dst);
void __fastcall NameTable_Rescan(uint8_t* t);

typedef void*(__cdecl* B49Fopen)(const char*, const char*);
typedef size_t(__cdecl* B49Fio)(void*, size_t, size_t, void*);
// The image file next to path: its base name + "_A.jpg" (jpg) or "_A.tga".
static void B49ImageName(const char* path, char* out, bool jpg) {
    CopyBaseName(path, out);
    memcpy(out + strlen(out), (const void*)(jpg ? 0x5ca328 : 0x5ca320), 7);
}

// 0x441f70 body. Appends the scenario's image (base name of path + "_A.jpg" when
// the block's +9 says jpg, else "_A.tga") to the file path, records where it
// starts (+0x1c), deletes both image files and rewrites the block at the start.
static void __cdecl Block_AppendImageImpl(uint8_t* block, const char* path) {
    void* f = ((B49Fopen)0x5a1887)(path, (const char*)0x5ca330);
    ((int(__cdecl*)(void*, long, int))0x5a261f)(f, 0, 2);
    F<int32_t>(block, 0x1c) = ((int32_t(__cdecl*)(void*))0x5a254f)(f);
    char name[0x40];
    B49ImageName(path, name, block[9] != 0);
    void* in = ((B49Fopen)0x5a1887)(name, (const char*)0x5c9678);
    uint8_t buf[0x200];
    for (size_t n; (int32_t)(n = ((B49Fio)0x5a1b05)(buf, 1, 0x200, in)) > 0;) ((B49Fio)0x5a276f)(buf, 1, n, f);
    ((int(__cdecl*)(void*))0x5a1740)(in);
    ((int(__cdecl*)(const char*))0x5a27bb)(name);
    B49ImageName(path, name, block[9] == 0);
    ((int(__cdecl*)(const char*))0x5a27bb)(name);
    Block3c2_SaveImpl(f, block, 0);
    ((int(__cdecl*)(void*))0x5a1740)(f);
}
// 0x441f70. Block in EAX, path on the stack.
__declspec(naked) void Block_AppendImage() {
    __asm {
        push [esp + 4]
        push eax
        call Block_AppendImageImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x441f70, Block_AppendImage);

// 0x4420f0 body. Extracts the image appended to the file path (from the block's
// +0x1c) into its "_A.jpg" / "_A.tga" file, rescans the resources and copies the
// file name to out. When that file can't be created the image goes into the
// buffer 0x62bed4 (0x402ee0 bytes; its length in 0x62bed8) instead.
static void __cdecl Block_ExtractImageImpl(const char* path, uint8_t* block, char* out) {
    void* f = ((B49Fopen)0x5a1887)(path, (const char*)0x5c9678);
    ((int(__cdecl*)(void*, long, int))0x5a261f)(f, F<int32_t>(block, 0x1c), 0);
    char name[0x44];
    B49ImageName(path, name, block[9] != 0);
    void* o = ((B49Fopen)0x5a1887)(name, (const char*)0x5ca334);
    uint8_t buf[0x200];
    if (!o) {
        if (void* old = hook::Global<void*>(0x62bed4)) ((void(__cdecl*)(void*))0x5a1145)(old);
        hook::Global<int32_t>(0x62bed8) = 0x402ee0;
        hook::Global<uint8_t*>(0x62bed4) = ((uint8_t*(__cdecl*)(size_t))0x5a125d)(0x402ee0);
        int32_t used = 0;
        for (int32_t n = (int32_t)((B49Fio)0x5a1b05)(buf, 1, 0x200, f); n > 0; n = (int32_t)((B49Fio)0x5a1b05)(buf, 1, 0x200, f)) {
            if (used + 0x200 >= hook::Global<int32_t>(0x62bed8)) break;
            memcpy(hook::Global<uint8_t*>(0x62bed4) + used, buf, (size_t)n);
            used += n;
        }
        hook::Global<int32_t>(0x62bed8) = used;
        ((int(__cdecl*)(void*))0x5a1740)(f);
        return;
    }
    for (size_t n; (int32_t)(n = ((B49Fio)0x5a1b05)(buf, 1, 0x200, f)) > 0;) ((B49Fio)0x5a276f)(buf, 1, n, o);
    ((int(__cdecl*)(void*))0x5a2945)(o);
    ((int(__cdecl*)(void*))0x5a1740)(o);
    ((int(__cdecl*)(void*))0x5a1740)(f);
    NameTable_Rescan(hook::Global<uint8_t*>(0x6d4020));
    strcpy(out, name);
}
// 0x4420f0. Path in ECX, block in EAX, out on the stack.
__declspec(naked) void Block_ExtractImage() {
    __asm {
        push [esp + 4]
        push eax
        push ecx
        call Block_ExtractImageImpl
        add esp, 12
        ret 4
    }
}
RT3_REPLACE(0x4420f0, Block_ExtractImage);

const char* __fastcall Options_SaveFolder(void* opt);
int32_t __fastcall Options_Check1bf(void* self);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);
int32_t __fastcall StreamOpen(const char* path, int32_t mode, int32_t bufferSize, int32_t fatalIfMissing);
int32_t __fastcall StreamRead(int32_t s, void* dst, uint32_t size, int32_t, int32_t);
int32_t __fastcall StreamSkip4(int32_t s, int, int32_t, int32_t count);
void __fastcall StreamClose(int32_t s);
uint8_t* __fastcall Jpeg_Load(const char* path, int32_t* width, int32_t* height, int32_t offset);

// 0x442400. Loads the description block (0x403c2 bytes) of save / map name (from
// the tutorial folder when tutorial is set). With option 0x1bf the whole block is
// read from file version 0x3f1 on (before 0x3f7 only up to +0x2c9, the rest
// cleared); otherwise its 0x3c2-byte header is read and the preview (+0x3c2,
// 0xffRRGGBB) is decoded from the JPEG appended at the header's offset +0x18.
RT3_NOINLINE void __fastcall Block_Load(const char* name, uint8_t* block, int32_t tutorial) {
    memset(block, 0, 0x403c2);
    char path[0x120];
    const char* folder = tutorial ? (const char*)0x5ca338 : Options_SaveFolder(hook::Global<void*>(0x6cec74));
    FormatText(path, (const char*)0x5c9d00, folder, name, 0, 0, 0, 0, 0, 0);
    int32_t s = StreamOpen(path, 1, 0x30d40, 1);
    int32_t version;
    if (Options_Check1bf(hook::Global<void*>(0x6cec74))) {
        StreamRead(s, &version, 4, 0, 1);
        if (version >= 0x3f1) {
            StreamSkip4(s, 0x2ee0, 0, 0);
            if (version >= 0x3f7) {
                StreamRead(s, block, 0x403c2, 0, 1);
            } else {
                StreamRead(s, block, 0x402c9, 0, 1);
                memset(block + 0x2c9, 0, 0x400f9);
            }
            StreamSkip4(s, 0x2ee1, 0, 0);
        }
        StreamClose(s);
        return;
    }
    StreamRead(s, &version, 4, 0, 1);
    StreamSkip4(s, 0x2ee0, 1, 1);
    StreamRead(s, block, 0x3c2, 0, 1);
    StreamSkip4(s, 0x2ee1, 1, 1);
    StreamClose(s);
    int32_t w, h;
    uint8_t* pix = Jpeg_Load(path, &w, &h, F<int32_t>(block, 0x18));
    for (int32_t y = 0; y < h; ++y)
        for (int32_t x = 0; x < w; ++x) {
            int32_t i = y * w + x;
            const uint8_t* p = pix + i * 3;
            F<uint32_t>(block, 0x3c2 + i * 4) = 0xff000000u | (uint32_t)p[0] << 16 | (uint32_t)p[1] << 8 | p[2];
        }
    ((void(__cdecl*)(void*))0x5a1145)(pix);
}
RT3_REPLACE(0x442400, Block_Load);

void __fastcall Input_RemoveHandler(uint8_t* in, int, uint32_t h);

// 0x4425d0. Takes the game options stored with save name (its description block,
// when its flag +0x21 is set): 0x6cec7c +0xc5 = 1, +0xc4 / +0x7d from +0x22 /
// +0x33 and the 16 bytes +0xc6.. from +0x23..; then drops input handler 0xcc.
// 1 when the save had them.
int32_t __fastcall Save_LoadOptions(const char* name) {
    uint8_t block[0x403c4];
    Block_Load(name, block, 0);
    if (!block[0x21]) return 0;
    F<uint8_t>(hook::Global<uint8_t*>(0x6cec7c), 0xc5) = 1;
    F<uint8_t>(hook::Global<uint8_t*>(0x6cec7c), 0xc4) = block[0x22];
    F<uint8_t>(hook::Global<uint8_t*>(0x6cec7c), 0x7d) = block[0x33];
    for (int32_t i = 0; i < 0x10; ++i) F<uint8_t>(hook::Global<uint8_t*>(0x6cec7c), 0xc6 + i) = block[0x23 + i];
    Input_RemoveHandler(hook::Global<uint8_t*>(0x6d4018), 0, 0xcc);
    return 1;
}
RT3_REPLACE(0x4425d0, Save_LoadOptions);

void __fastcall StripExtension(char* s);
int32_t __fastcall Pack_Compress(int32_t* p, int, const void* src, int32_t n, uint8_t** out, int32_t* outSize);
int32_t __fastcall Pack_Decompress(int32_t* p, int, const int32_t* src, int32_t n, uint8_t** out, int32_t* outSize);
void __fastcall Nop552ff0();
typedef int(__cdecl* B60Fseek)(void*, long, int);
typedef long(__cdecl* B60Ftell)(void*);

// The whole of file path in a new heap buffer (*size bytes).
static uint8_t* B60ReadAll(const char* path, int32_t* size) {
    void* f = ((B49Fopen)0x5a1887)(path, (const char*)0x5c9678);
    ((B60Fseek)0x5a261f)(f, 0, 2);
    *size = ((B60Ftell)0x5a254f)(f);
    ((B60Fseek)0x5a261f)(f, 0, 0);
    uint8_t* buf = ((uint8_t*(__cdecl*)(size_t))0x5a125d)((size_t)*size);
    ((B49Fio)0x5a1b05)(buf, 1, (size_t)*size, f);
    ((int(__cdecl*)(void*))0x5a1740)(f);
    return buf;
}

// 0x4427c0. Unpacks save file path into path's base name + ".tmp": its 0x3ce-byte
// header, the LZSS payload at +0x3ce unpacked, then everything after the packed
// data. Nothing is written when unpacking fails.
void __fastcall Save_UnpackToTemp(const char* path) {
    char name[0x30];
    strcpy(name, path);
    StripExtension(name);
    memcpy(name + strlen(name), (const void*)0x5ca354, 5);
    int32_t size;
    uint8_t* buf = B60ReadAll(path, &size);
    uint8_t* out = nullptr;
    int32_t outSize = 0;
    int32_t packer[3] = { 0x91, 0, 0 };
    if (Pack_Decompress(packer, 0, (const int32_t*)(buf + 0x3ce), 0, &out, &outSize) == 1) {
        void* f = ((B49Fopen)0x5a1887)(name, (const char*)0x5ca334);
        ((B49Fio)0x5a276f)(buf, 1, 0x3ce, f);
        ((B49Fio)0x5a276f)(out, 1, (size_t)outSize, f);
        int32_t packed = F<int32_t>(buf + 0x3ce, 4);
        ((B49Fio)0x5a276f)(buf + 0x3ce + packed, 1, (size_t)(size - packed - 0x3ce), f);
        ((int(__cdecl*)(void*))0x5a1740)(f);
        ((void(__cdecl*)(void*))0x5a1145)(out);
    }
    ((void(__cdecl*)(void*))0x5a1145)(buf);
    ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
}
RT3_REPLACE(0x4427c0, Save_UnpackToTemp);

// 0x442900 body. Packs save file path in place: the payload from +0x3ce to the
// block's data offset (+0x18) is LZSS-packed (progress to 0x441e20), the rest
// is kept, the offset moves to where the data now starts and the block header
// is rewritten at the start.
static void __cdecl Save_PackImpl(uint8_t* block, const char* path) {
    int32_t size;
    uint8_t* buf = B60ReadAll(path, &size);
    uint8_t* out;
    int32_t outSize;
    int32_t packer[3] = { 0x91, 0, 0 };
    hook::Global<uint32_t>(0xd93988) = 0x441e20;
    Pack_Compress(packer, 0, buf + 0x3ce, F<int32_t>(block, 0x18) - 0x3ce, &out, &outSize);
    void* f = ((B49Fopen)0x5a1887)(path, (const char*)0x5ca334);
    ((B49Fio)0x5a276f)(buf, 1, 0x3ce, f);
    ((B49Fio)0x5a276f)(out, 1, (size_t)outSize, f);
    int32_t pos = ((B60Ftell)0x5a254f)(f);
    ((B49Fio)0x5a276f)(buf + F<int32_t>(block, 0x18), 1, (size_t)(F<int32_t>(block, 0x1c) - F<int32_t>(block, 0x18)), f);
    ((B49Fio)0x5a276f)(buf + F<int32_t>(block, 0x1c), 1, (size_t)(size - F<int32_t>(block, 0x1c)), f);
    F<int32_t>(block, 0x18) = pos;
    Block3c2_SaveImpl(f, block, 0);
    ((int(__cdecl*)(void*))0x5a1740)(f);
    ((void(__cdecl*)(void*))0x5a1145)(buf);
    ((void(__cdecl*)(void*))0x5a1145)(out);
    hook::Global<uint32_t>(0xd93988) = 0;
    ((void(__fastcall*)(int32_t*))&Nop552ff0)(packer);
}
// 0x442900. Block in EAX, path on the stack.
__declspec(naked) void Save_Pack() {
    __asm {
        push [esp + 4]
        push eax
        call Save_PackImpl
        add esp, 8
        ret 4
    }
}
RT3_REPLACE(0x442900, Save_Pack);

} // namespace rt3
