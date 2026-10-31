// Functions across blocks 0x43..0x46 (batch 32). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void* __fastcall Record42d5a0_Init(uint8_t* r);
void __fastcall Record42d5a0_SetName(uint8_t* r, int, int32_t v, const char* name);
void __fastcall Obj_FreeAllLists2(void* o);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);

template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

// 0x432ea0. Adds a fresh 0x88f-byte record (initialised by 0x42d5a0) under
// `key`, names it, and frees the template. Returns the new ID.
uint32_t __fastcall Table_AddNamedRecord(Collection* t, int, const char* name, uint32_t key) {
    uint8_t* tmp = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x88f);
    if (tmp) tmp = (uint8_t*)Record42d5a0_Init(tmp);
    uint32_t id = hook::CallTarget<uint32_t(__fastcall*)(Collection*, int, const void*, uint32_t, uint32_t, uint32_t)>(0x432ed2)(t, 0, tmp, 0x88f, key, 0);
    Record42d5a0_SetName((uint8_t*)Collection_Get(t, 0, id), 0, (int32_t)id, name);
    if (tmp) {
        Obj_FreeAllLists2(tmp);
        ((void(__cdecl*)(void*))0x53b080)(tmp);
    }
    return id;
}
RT3_REPLACE(0x432ea0, Table_AddNamedRecord);

// 0x442330. Pops the last option history entry (see 0x4422d0) back into the options.
void PopOptionHistory() {
    int32_t n = --hook::Global<int32_t>(0x62bedc);
    const uint8_t* d = (const uint8_t*)0x62be90 + n * 5;
    uint8_t* opt = hook::Global<uint8_t*>(0x6cec74);
    F<int32_t>(opt, 0x140) = (int8_t)d[0];
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x13c) = (int8_t)d[1];
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x138) = (int8_t)d[2];
    F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x144) = d[3];
    F<int32_t>(hook::Global<uint8_t*>(0x6cec78), 0x4c74) = d[4];
}
RT3_REPLACE(0x442330, PopOptionHistory);

// 0x4553d0. Record with the lowest serial +4 (skipping flagged +0xc ones unless `all`); 0 if none.
RT3_NOINLINE void* __fastcall Table_Oldest(Collection* t, int, uint8_t all) {
    uint32_t sel = 0;
    int32_t best = 0x7fffffff;
    for (int32_t i = 0; i < Collection_Count(t); ++i) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (F<int32_t>(r, 4) >= best) continue;
        if (!all && r[0xc]) continue;
        sel = F<uint32_t>(r, 0);
        best = F<int32_t>(r, 4);
    }
    return sel ? Collection_Get(t, 0, sel) : nullptr;
}
RT3_REPLACE(0x4553d0, Table_Oldest);

// 0x4681f0. Releases every record whose u16 +0xa, +0xc or +0xe equals key.
void __fastcall Table_ReleaseKeyAny(Collection* t, int, int32_t key) {
    for (int32_t i = 0; i < Collection_Count(t); ) {
        uint8_t* r = (uint8_t*)Collection_Get(t, 0, Collection_IdAt(t, 0, i, 0));
        if (key == F<uint16_t>(r, 0xa) || key == F<uint16_t>(r, 0xc) || key == F<uint16_t>(r, 0xe)) {
            void** vt = *(void***)t;
            ((void(__thiscall*)(void*, uint32_t))vt[2])(t, Collection_IdAt(t, 0, i, 0));
        } else {
            ++i;
        }
    }
}
RT3_REPLACE(0x4681f0, Table_ReleaseKeyAny);

// 0x468420. Number of files matching "<dir>*.mp3".
int32_t __fastcall CountMp3Files(const char* dir) {
    char pattern[0x100];
    uint8_t findData[0x140];
    FormatText(pattern, (const char*)0x5ce13c, dir, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    int32_t n = 0;
    void* h = Import<void*(__stdcall*)(const char*, void*)>(0x5c8190)(pattern, findData);
    if (h == (void*)-1) return 0;
    auto findNext = Import<int32_t(__stdcall*)(void*, void*)>(0x5c8194);
    do ++n;
    while (findNext(h, findData) == 1);
    Import<int32_t(__stdcall*)(void*)>(0x5c819c)(h);
    return n;
}
RT3_REPLACE(0x468420, CountMp3Files);

// 0x4689a0. Writes the string value `name` = data under subKey of this
// registry key (+4). 1 on success. QUIRK: if creating the key fails, the
// subKey pointer is used as the handle.
int32_t __fastcall RegKey_SetString(void* o, int, const char* subKey, const char* name, const char* data) {
    void* hk = (void*)subKey;
    uint32_t disposition;
    Import<int32_t(__stdcall*)(void*, const char*, uint32_t, void*, uint32_t, uint32_t, void*, void*, void*)>(0x5c8008)(
        F<void*>(o, 4), subKey, 0, nullptr, 0, 0xf003f, nullptr, &hk, &disposition);
    uint32_t len = 0;
    while (data[len]) ++len;
    int32_t ok = 0;
    if (Import<int32_t(__stdcall*)(void*, const char*, uint32_t, uint32_t, const void*, uint32_t)>(0x5c8014)(
            hk, name, 0, 1, data, len + 1) == 0)
        ok = 1;
    Import<int32_t(__stdcall*)(void*)>(0x5c800c)(hk);
    return ok;
}
RT3_REPLACE(0x4689a0, RegKey_SetString);

} // namespace rt3
