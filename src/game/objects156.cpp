// Record tag encoding / timed scenario event / save game listing / industry template cleanup in blocks 0x42..0x43 (batch 160). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __stdcall Text_ConvertTags(char* text, int32_t maxLen, int32_t encode);
const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char*, const char*, const char*, const char*, const char*, const char*,
                            const char*, const char*, const char*, const char*);
uint32_t __fastcall Table_AddNamedRecord(Collection* t, int, const char* name, uint32_t key);
uint32_t __fastcall Table_FindByName(Collection* t, int, const char* name);
int32_t __fastcall Options_Check1bf(void* self);
const char* __fastcall Options_SaveFolder(void* opt);
int32_t __fastcall Save_IsCompatible(uint8_t* g, int, const char* name);
void __fastcall StripExtension(char* s);
const char* __fastcall LocalizeName(const char* name);
void __fastcall SortRecordsByName(uint8_t* records, int32_t count, uint32_t size, int32_t keyOffset);
void __fastcall Industries_RebuildPortsWarehouses(Collection* c);

static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }
static inline void* ExeNew(size_t n) { return ((void*(__cdecl*)(size_t))0x53b070)(n); }
static inline char* Fmt(char* dst, const char* fmt, const char* a = 0, const char* b = 0) {
    return FormatText(dst, fmt, a, b, 0, 0, 0, 0, 0, 0);
}

// 0x42da90. Encodes the [tags] of the record's six texts (+0xe0 up to 800 chars, the
// rest up to 200) to their numbers.
void __fastcall Record_EncodeTags(uint8_t* r) {
    Text_ConvertTags((char*)r + 0xe0, 0x320, 1);
    static const uint16_t offs[] = { 0x401, 0x4ca, 0x593, 0x65c, 0x725 };
    for (uint16_t off : offs) Text_ConvertTags((char*)r + off, 0xc8, 1);
}
RT3_REPLACE(0x42da90, Record_EncodeTags);

// 0x430b50. Adds a scenario event record (table 0x62be18, named by string 0xd5 with
// this +0x18) that fires delay->years/months (+9/+0xd) from now (events 7 = year,
// 8 = month) with the condition (+0 id, +4 value, +8 kind: 1 = flag, 2 = negated).
// Optional values a/b/c fill +0x7f0 (else 99), +0x82c and +0x830.
void __fastcall Events_AddTimed(uint8_t* self, int, uint8_t* cond, int32_t* a, int32_t* b, int32_t* c) {
    Collection* t = hook::Global<Collection*>(0x62be18);
    uint32_t key = Collection_Count(t) + 1;
    char name[0x190];
    Fmt(name, GetString(0xd5), (const char*)self + 0x18);
    uint8_t* r = (uint8_t*)Collection_Get(hook::Global<Collection*>(0x62be18), 0,
                                          Table_AddNamedRecord(hook::Global<Collection*>(0x62be18), 0, name, key));
    r[0x7ee] = 0;
    r[0x7ef] = World()[0x11] < 0xe ? 2 : 3;
    F<int32_t>(r, 0x7f5) = 1;
    r[0x7f4] = self[0x7f4];
    if (b) {
        r[0x7f9] = 0x63;
        F<int32_t>(r, 0x82c) = *b;
    } else r[0x7f9] = 0;
    if (c) {
        r[0x7fa] = 0x63;
        F<int32_t>(r, 0x830) = *c;
    } else r[0x7fa] = 0;
    F<int32_t>(r, 0x7f0) = a ? *a : 0x63;
    r[0x7fb] = 0;
    F<int32_t>(r, 0x7ff) = 1;
    r[0x82b] = 1;
    uint8_t* w = World();
    int32_t year = F<uint16_t>(w, 0xd) + F<int32_t>(cond, 9);
    int32_t month = w[0xf] + F<int32_t>(cond, 0xd);
    if (month > 12) {
        year += (month - 1) / 12;
        month = (month - 1) % 12 + 1;
    }
    uint8_t* e1 = (uint8_t*)ExeNew(0x1e);
    memset(e1, 0, 0x1e);
    F<int32_t>(e1, 0) = 7;
    e1[4] = 4;
    F<int32_t>(e1, 5) = 0;
    F<int32_t>(e1, 9) = year;
    e1[0xd] = 2;
    F<uint8_t*>(r, 0) = e1;
    uint8_t* e2 = (uint8_t*)ExeNew(0x1e);
    memset(e2, 0, 0x1e);
    F<int32_t>(e2, 0) = 8;
    e2[4] = 4;
    F<int32_t>(e2, 5) = 0;
    F<int32_t>(e2, 9) = month;
    e2[0xd] = 0;
    F<int32_t>(e2, 0x1a) = 0;
    F<uint8_t*>(e1, 0x1a) = e2;
    uint8_t* n = (uint8_t*)ExeNew(0x28);
    F<int32_t>(n, 0) = F<int32_t>(cond, 0);
    uint8_t kind = cond[8];
    if (kind == 1) F<int32_t>(n, 4) = F<int32_t>(cond, 4) != 0;
    else if (kind == 2) F<int32_t>(n, 4) = -F<int32_t>(cond, 4);
    else F<int32_t>(n, 4) = 0;
    n[8] = cond[8];
    F<int32_t>(n, 9) = -1;
    F<int32_t>(n, 0xd) = -1;
    F<int32_t>(n, 0x18) = 0;
    F<int32_t>(n, 0x1c) = 0;
    F<int32_t>(n, 0x20) = 0;
    F<int32_t>(n, 0x24) = 0;
    F<uint8_t*>(r, 4) = n;
}
RT3_REPLACE(0x430b50, Events_AddTimed);

// 0x4333f0. Lists the compatible saves (*.gm*, or *.smp with option +0x1bf) in the save
// folder into list (+0 count, +8 0x25a-byte entries: file name, display name at +0x12d,
// localized for kinds 5..7), sorted by display name. Quirk: the folder is scanned twice
// and the second pass is not bounded by the first count.
void __fastcall SaveList_Scan(uint8_t* list, int, int32_t kind) {
    auto findFirst = Import<void*(__stdcall*)(const char*, void*)>(0x5c8190);
    auto findNext = Import<int32_t(__stdcall*)(void*, void*)>(0x5c8194);
    auto findClose = Import<int32_t(__stdcall*)(void*)>(0x5c819c);
    char fd[0x140];
    char* file = fd + 0x2c;
    F<int32_t>(list, 0) = 0;
    char pat[0x12c], dir[0x12c], path[0x190], full[0x1f4], path2[0x1f4], base[0x12c];
    Fmt(pat, (const char*)(Options_Check1bf(hook::Global<void*>(0x6cec74)) ? 0x5c9d1c : 0x5c9d14));
    Fmt(dir, (const char*)0x5c9d10, Options_SaveFolder(hook::Global<void*>(0x6cec74)));
    Fmt(path, (const char*)0x5c9d08, dir, pat);
    Fmt(full, path);
    void* h = findFirst(full, fd);
    if (h != (void*)-1) {
        do {
            if (*file != '.' && Save_IsCompatible(list, 0, file)) ++F<int32_t>(list, 0);
        } while (findNext(h, fd));
    }
    findClose(h);
    uint8_t* buf = (uint8_t*)Malloc(F<int32_t>(list, 0) * 0x25a + 1);
    F<uint8_t*>(list, 8) = buf;
    memset(buf, 0, F<int32_t>(list, 0) * 0x25a);
    Fmt(path2, (const char*)0x5c9d08, dir, pat);
    Fmt(full, path2);
    h = findFirst(full, fd);
    if (h != (void*)-1) {
        int32_t off = 0;
        do {
            if (*file == '.' || !Save_IsCompatible(list, 0, file)) continue;
            strcpy((char*)F<uint8_t*>(list, 8) + off, file);
            strcpy(base, file);
            StripExtension(base);
            const char* shown = (kind == 5 || kind == 6 || kind == 7) ? LocalizeName(base) : base;
            strcpy((char*)F<uint8_t*>(list, 8) + off + 0x12d, shown);
            off += 0x25a;
        } while (findNext(h, fd));
    }
    findClose(h);
    SortRecordsByName(F<uint8_t*>(list, 8), F<int32_t>(list, 0), 0x25a, 0x12d);
}
RT3_REPLACE(0x4333f0, SaveList_Scan);

static inline void FirstCargoName(char* dst, int32_t pos) {
    Collection* c = hook::Global<Collection*>(0x62ba8c);
    strcpy(dst, (const char*)Collection_Get(c, 0, Collection_IdAt(c, 0, pos, 0)) + 4);
}

// 0x435630. Cleans up the 12 industry templates (+0x13d4, 0x4e1 bytes; five 0x30-byte
// recipe slots at +4: kind 1 = input only, 2 = output only, 3 = conversion, output
// name +8, input name +0x1c, rate +4): missing names get the first cargo, conversions
// get an output different from the input, low rates become 1 (the speed +0 becomes 4).
// Then rebuilds each template's recipe table (count -0x3b0, 0xbc-byte entries at
// -0x3ac) and the industries' port/warehouse lists.
void __fastcall Templates_Normalize(uint8_t* self) {
    const float lim = *(const float*)0x5c85b4;
    uint8_t* t = self + 0x13d4;
    for (int32_t k = 12; k; --k, t += 0x4e1) {
        if (F<float>(t, 0) < lim) F<uint32_t>(t, 0) = 0x40800000;
        int32_t count = 0;
        uint8_t* e = t + 4;
        for (int32_t j = 5; j; --j, e += 0x30) {
            int32_t kind = F<int32_t>(e, 0);
            if (kind) ++count;
            if (lim > F<float>(e, 4)) F<uint32_t>(e, 4) = 0x3f800000;
            if ((kind == 1 || kind == 3) && !e[0x1c]) FirstCargoName((char*)e + 0x1c, 0);
            kind = F<int32_t>(e, 0);
            if ((kind == 2 || kind == 3) && !e[8]) FirstCargoName((char*)e + 8, 0);
            if (F<int32_t>(e, 0) == 3) {
                int32_t i = 0;
                while (!Stricmp((const char*)e + 8, (const char*)e + 0x1c)) {
                    FirstCargoName((char*)e + 8, i++);
                    if (F<int32_t>(e, 0) != 3) break;
                }
            }
        }
        F<int32_t>(t, -0x3b0) = count;
        uint8_t* r = t - 0x3ac;
        e = t + 4;
        for (int32_t j = 5; j; --j, e += 0x30) {
            if (!F<int32_t>(e, 0)) continue;
            memset(r, 0, 0xbc);
            F<int32_t>(r, 0) = F<int32_t>(e, 0) == 1;
            F<uint16_t>(r, 0x20) = 0x708;
            F<uint16_t>(r, 0x22) = 0x270f;
            F<int32_t>(r, 0x24) = 0;
            F<int32_t>(r, 0x28) = 0;
            bool input = true;
            if (F<int32_t>(e, 0) != 1) {
                strcpy((char*)r + 8, (const char*)e + 8);
                F<uint32_t>(r, 0x1c) = Table_FindByName(hook::Global<Collection*>(0x62ba8c), 0, (const char*)e + 8);
                F<uint32_t>(r, 4) = F<uint32_t>(e, 4);
                input = F<int32_t>(e, 0) == 1 || F<int32_t>(e, 0) == 3;
            }
            if (input) {
                F<int32_t>(r, 0x2c) = 1;
                strcpy((char*)r + 0x30, (const char*)e + 0x1c);
                F<uint32_t>(r, 0x44) = Table_FindByName(hook::Global<Collection*>(0x62ba8c), 0, (const char*)e + 0x1c);
                F<uint32_t>(r, 0x48) = F<int32_t>(e, 0) == 3 ? 0x3f800000 : F<uint32_t>(e, 4);
            }
            r += 0xbc;
        }
    }
    if (Collection* inds = hook::Global<Collection*>(0x62b268)) Industries_RebuildPortsWarehouses(inds);
}
RT3_REPLACE(0x435630, Templates_Normalize);

} // namespace rt3
