// Dialog strings / destructors / resource directory scan in blocks 0x4d..0x54 (batch 94). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/ptrlist.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }
template <typename Fn> static inline Fn Import(uint32_t iat) { return (Fn)hook::Global<void*>(iat); }

void __fastcall Widget_SetVtbl(uint8_t* w);
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id);
void __fastcall Widget_SetRect(uint8_t* w, int, float x, float y, float wd, float ht);
void __fastcall Options_Save(uint8_t* o, int, int32_t withEngine);
void __fastcall Obj_FreeBuffer8(void* o);
uint32_t __fastcall Obj_Get18(uint8_t* o);
int32_t __fastcall Obj_Zero18And14(uint8_t* o);
void __fastcall NameTable_CloseCurrent(uint8_t* t);
void __fastcall NameTable_Add(uint8_t* t, int, const char* dir, const char* name, int32_t size, int32_t counted);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
int32_t __fastcall File_Exists(const char* path);

static inline void ExeDelete(void* p) { ((void(__cdecl*)(void*))0x53b080)(p); }
static inline uint8_t* Options() { return hook::Global<uint8_t*>(0x6cec74); }
static inline size_t Len(const char* s) { size_t n = 0; while (s[n]) ++n; return n; }
static inline void Copy(char* d, const char* s) { while ((*d++ = *s++) != 0) {} }

void __fastcall Widget4d4840_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0cd4;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4d4840, Widget4d4840_Destruct);
void __fastcall Widget4d5b90_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d0cf4;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4d5b90, Widget4d5b90_Destruct);

// Replaces the text (+0xba) of child `id` with a heap copy of s (only if it had one).
static inline void SetChildText(uint8_t* dlg, int32_t id, const char* s) {
    uint8_t* c = Widget_FindChild(dlg, 0, id);
    if (!F<void*>(c, 0xba)) return;
    Free(F<void*>(c, 0xba));
    char* p = (char*)Malloc(Len(s) + 2);
    F<char*>(c, 0xba) = p;
    Copy(p, s);
}

// 0x4eccd0. Stores the player name (options +0x1ef) and shows it in fields 0xe48/0xe5d.
void __fastcall NameDialog_SetName(uint8_t* d, int, const char* name) {
    if (!name) return;
    Copy((char*)Options() + 0x1ef, name);
    SetChildText(d, 0xe48, (const char*)Options() + 0x1ef);
    SetChildText(d, 0xe5d, (const char*)Options() + 0x1ef);
}
RT3_REPLACE(0x4eccd0, NameDialog_SetName);

// 0x4ed590. Shows the name in both fields and stores it when it differs.
void __fastcall NameDialog_ShowName(uint8_t* d, int, const char* name) {
    SetChildText(d, 0xe48, name);
    SetChildText(d, 0xe5d, name);
    if (Stricmp((const char*)Options() + 0x1ef, name)) Copy((char*)Options() + 0x1ef, name);
}
RT3_REPLACE(0x4ed590, NameDialog_ShowName);

// 0x4ee3a0. Options screen destructor: saves the options and frees its globals.
void __fastcall OptionsScreen_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d12ac;
    hook::Global<int32_t>(0x6d1268) = 0;
    F<uint32_t>(Options(), 0x30d) = hook::Global<uint32_t>(0x6226d0);
    Options_Save(Options(), 0, 0);
    if (void* p = hook::Global<void*>(0x6d126c)) {
        Obj_FreeBuffer8(p);
        ExeDelete(p);
        hook::Global<void*>(0x6d126c) = nullptr;
    }
    if (void* p = hook::Global<void*>(0x6d1270)) {
        ExeDelete(p);
        hook::Global<void*>(0x6d1270) = nullptr;
    }
    if (void* p = hook::Global<void*>(0x6d1294)) {
        ((void(__thiscall*)(void*, int32_t))(*(void***)p)[0])(p, 1);
        hook::Global<void*>(0x6d1294) = nullptr;
    }
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x4ee3a0, OptionsScreen_Destruct);

uint32_t __fastcall Obj_Busy18(uint8_t* o) {
    if (Obj_Get18(o)) return 1;
    return Obj_Zero18And14(o) ? 1 : 0;
}
RT3_REPLACE(0x4fe140, Obj_Busy18);

void __fastcall Widget5157c0_Destruct(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5d19d0;
    hook::Global<int32_t>(0x6d3b34) = 0;
    Widget_SetVtbl(w);
}
RT3_REPLACE(0x5157c0, Widget5157c0_Destruct);

// 0x538810. Child `id` of any window in the chain (+0, next +0x58).
uint8_t* __fastcall Windows_FindChild(uint8_t* o, int, int32_t id) {
    for (uint8_t* w = F<uint8_t*>(o, 0); w; w = F<uint8_t*>(w, 0x58))
        if (uint8_t* c = Widget_FindChild(w, 0, id)) return c;
    return nullptr;
}
RT3_REPLACE(0x538810, Windows_FindChild);

// 0x53b970. Closes the current file and frees every entry.
void __fastcall NameTable_Clear(uint8_t* t) {
    if (F<int32_t>(t, 0x427bc) < 0) NameTable_CloseCurrent(t);
    for (int b = 0; b < 0x20; ++b) {
        uint8_t** head = (uint8_t**)(t + 0x203a4 + b * 4);
        while (*head) {
            uint8_t* e = *head;
            *head = F<uint8_t*>(e, 0x14);
            ExeDelete(F<void*>(e, 0x10));
            ExeDelete(e);
        }
    }
    F<int32_t>(t, 0x20424) = 0;
    F<int32_t>(t, 0x427b0) = -1;
}
RT3_REPLACE(0x53b970, NameTable_Clear);

// 0x53c820. Registers every file of the 13 resource directories (0x624e14,
// pattern 0x5dd610; the first two find results, "." and "..", are skipped),
// then rebuilds the per-bucket start indices (+0x2042c).
void __fastcall NameTable_ScanDirs(uint8_t* t) {
    auto findFirst = Import<void*(__stdcall*)(const char*, void*)>(0x5c8190);
    auto findNext = Import<int32_t(__stdcall*)(void*, void*)>(0x5c8194);
    for (const char* const* dir = (const char* const*)0x624e14; dir < (const char* const*)0x624e48; ++dir) {
        char pattern[0x11c];
        uint8_t fd[0x140];
        FormatText(pattern, (const char*)0x5dd610, *dir, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        void* h = findFirst(pattern, fd);
        findNext(h, fd);
        while (findNext(h, fd)) NameTable_Add(t, 0, *dir, (const char*)fd + 0x2c, F<int32_t>(fd, 0x20), 0);
        if (h != (void*)-1) Import<int32_t(__stdcall*)(void*)>(0x5c819c)(h);
    }
    int32_t n = 0;
    for (int b = 0; b < 0x20; ++b) {
        F<int32_t>(t, 0x2042c + b * 4) = n;
        for (uint8_t* e = F<uint8_t*>(t, 0x203a4 + b * 4); e; e = F<uint8_t*>(e, 0x14)) ++n;
    }
}
RT3_REPLACE(0x53c820, NameTable_ScanDirs);

// 0x53d130. Registers a single existing file by full path (split at the last
// backslash; without one the name starts at index 1).
void __fastcall NameTable_AddPath(uint8_t* t, int, const char* path) {
    char name[0x12c], dir[0x124];
    int32_t len = (int32_t)Len(path);
    if (len <= 0) return;
    int32_t i = len;
    while (path[i] != '\\') {
        if (--i <= 0) break;
    }
    if (i == len) return;
    ++i;
    Copy(name, path + i);
    StrCopyN(dir, path, i);
    if (!File_Exists(path)) return;
    NameTable_Add(t, 0, dir, name, -1, 1);
    F<int32_t>(t, 0x427b0) = -1;
}
RT3_REPLACE(0x53d130, NameTable_AddPath);

void __fastcall Widget_Move(uint8_t* w, int, float x, float y) {
    Widget_SetRect(w, 0, x, y, F<float>(w, 0xd), F<float>(w, 0x11));
}
RT3_REPLACE(0x53fe70, Widget_Move);

// 0x540de0. Cross product a x b into (*x, *y, *z).
void __fastcall Vec_Cross(float* x, float* y, float ax, float ay, float az, float bx, float by, float bz, float* z) {
    *x = (float)((double)ay * bz - (double)az * by);
    *y = (float)((double)az * bx - (double)ax * bz);
    *z = (float)((double)ax * by - (double)ay * bx);
}
RT3_REPLACE(0x540de0, Vec_Cross);

// 0x540e30. Texture constructor: links itself at the head of the global
// list 0xccbb2c (count 0xccbb30). Returns the previous head.
uint32_t __fastcall Texture_Construct(uint8_t* s, int, uint8_t flag) {
    ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)((char*)s + 0x13, (const char*)0x5c87a8);
    uint8_t* e = hook::Global<uint8_t*>(0x6d4024);
    s[4] = 0;
    F<uint32_t>(s, 0xd8) = F<uint32_t>(e, 0x114736);
    F<int32_t>(s, 0xcc) = 0;
    F<int32_t>(s, 0xd0) = 0;
    s[6] = 1;
    s[5] = 0x7f;
    F<int32_t>(s, 0xc0) = 0;
    F<int32_t>(s, 0xc4) = 0;
    F<int32_t>(s, 0xc8) = 0;
    s[0x93] = 0;
    s[0x94] = 0;
    s[0x95] = 0;
    s[0x97] = flag;
    F<int32_t>(s, 0xd4) = 0;
    s[0x96] = hook::Global<uint8_t*>(0x6d4024)[0x114254];
    static const uint32_t lists[] = { 0xdc, 0xe4 };
    for (uint32_t off : lists) {
        PtrList* l = (PtrList*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
        F<PtrList*>(s, off) = l ? PtrList_Construct(l, 0, 1, 100) : nullptr;
        F<int32_t>(s, off + 4) = 0;
    }
    for (int i = 0; i < 8; ++i) {
        s[0x98 + i] = 0;
        F<int32_t>(s, 0xa0 + i * 4) = 0;
    }
    F<int32_t>(s, 7) = 0;
    uint32_t head = hook::Global<uint32_t>(0xccbb2c);
    if (!head) {
        hook::Global<int32_t>(0xccbb30) = 0;
        hook::Global<int32_t>(0xccbb34) = 0;
    }
    hook::Global<int32_t>(0xccbb30)++;
    F<uint32_t>(s, 0) = head;
    hook::Global<uint8_t*>(0xccbb2c) = s;
    return head;
}
RT3_REPLACE(0x540e30, Texture_Construct);

uint32_t __fastcall Obj_Next0(uint8_t* o) { return F<uint32_t>(o, 0); }
RT3_REPLACE(0x540f50, Obj_Next0);

} // namespace rt3
