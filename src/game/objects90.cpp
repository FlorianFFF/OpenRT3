// Company logo pick / resource name table / UI widget helpers in blocks 0x42..0x54 (batch 93). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint32_t __fastcall Table_Entry(uint8_t* t, int, int32_t idx);
int32_t CountFreeCompanySlots();
int32_t __fastcall NthFreeCompanySlot(int32_t n);
int32_t __fastcall Rand(int32_t lo, int32_t hi);
uint8_t* __fastcall NameTable_Nth(uint8_t* t, int, int32_t n);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
int32_t __fastcall File_Size(const char* path);
void Engine_Nop();
void __fastcall Engine_ResetTextureQueue(void* e);
void __fastcall Engine_ClearRepaint(void* e);

static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

// 0x426b10. The company's logo entry: its stored index (+0x37), the logo whose
// name matches +0xd65, or a random free slot (stored).
uint32_t __fastcall Company_Logo(uint8_t* c) {
    if (F<int32_t>(c, 0x37) > -1) return Table_Entry(F<uint8_t*>(World(), 0x46a80), 0, F<int32_t>(c, 0x37));
    for (int32_t i = 0; i < Collection_Count(F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0)); ++i) {
        Collection* names = F<Collection*>(hook::Global<uint8_t*>(0x6d4020), 0x429b0);
        const char* s = (const char*)Collection_Get(names, 0, Collection_IdAt(names, 0, i, 0));
        if (!Stricmp((const char*)c + 0xd65, s)) return Table_Entry(F<uint8_t*>(World(), 0x46aa4 + i * 4), 0, 0);
    }
    int32_t r = Rand(0, CountFreeCompanySlots() - 1);
    F<int32_t>(c, 0x37) = r;
    F<int32_t>(c, 0x37) = NthFreeCompanySlot(r);
    return Table_Entry(F<uint8_t*>(World(), 0x46a80), 0, F<int32_t>(c, 0x37));
}
RT3_REPLACE(0x426b10, Company_Logo);

// 0x53b690. Closes the current entry's file (handle +4 or FILE* +8).
void __fastcall NameTable_CloseCurrent(uint8_t* t) {
    uint8_t* e = NameTable_Nth(t, 0, -F<int32_t>(t, 0x427bc));
    if (F<int32_t>(e, 4) == -1) return;
    if (!F<void*>(e, 8)) Close(F<int32_t>(e, 4));
    else ((int(__cdecl*)(void*))0x5a1740)(F<void*>(e, 8));
    F<int32_t>(e, 4) = -1;
    F<void*>(e, 8) = nullptr;
}
RT3_REPLACE(0x53b690, NameTable_CloseCurrent);

// 0x53c710. Registers a file: the path dir + name (0x5c8730) under the hash of
// `name`, with its size (looked up when -1); `counted` bumps the per-bucket
// offsets (+0x2042c) of the later buckets.
void __fastcall NameTable_Add(uint8_t* t, int, const char* dir, const char* name, int32_t size, int32_t counted) {
    char path[0x1f4];
    FormatText(path, (const char*)0x5c8730, dir, name, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (size == -1) size = File_Size(path);
    uint8_t* e = (uint8_t*)((void*(__cdecl*)(size_t))0x53b070)(0x18);
    for (int k = 0; k < 6; ++k) F<int32_t>(e, k * 4) = 0;
    F<int32_t>(e, 0) = NameTable_Hash(t, 0, name);
    size_t n = 0;
    while (path[n]) ++n;
    char* copy = (char*)((void*(__cdecl*)(size_t))0x53b070)(n + 1);
    F<char*>(e, 0x10) = copy;
    F<int32_t>(e, 0xc) = size;
    F<int32_t>(e, 4) = -1;
    uint32_t b = F<uint32_t>(e, 0) & 0x1f;
    for (size_t k = 0; (copy[k] = path[k]) != 0; ++k) {}
    F<uint8_t*>(e, 0x14) = F<uint8_t*>(t, 0x203a4 + b * 4);
    F<uint8_t*>(t, 0x203a4 + b * 4) = e;
    if (counted && b + 1 < 0x20)
        for (uint32_t j = b + 1; j < 0x20; ++j) F<int32_t>(t, 0x2042c + j * 4)++;
}
RT3_REPLACE(0x53c710, NameTable_Add);

// 0x53ca20. Pushes {file position, current entry, previous} onto the stack +0x20428.
void __fastcall NameTable_PushPos(uint8_t* t) {
    uint32_t* rec = (uint32_t*)Malloc(0xc);
    int32_t cur = F<int32_t>(t, 0x427bc);
    int32_t fd = cur < 0 ? F<int32_t>(NameTable_Nth(t, 0, -cur), 4) : F<int32_t>(t, 0x204ac + cur * 4);
    rec[0] = (uint32_t)Tell(fd);
    rec[1] = F<uint32_t>(t, 0x427bc);
    rec[2] = F<uint32_t>(t, 0x20428);
    F<uint32_t*>(t, 0x20428) = rec;
    F<int32_t>(t, 0x428f8)++;
}
RT3_REPLACE(0x53ca20, NameTable_PushPos);

uint32_t __fastcall Obj_Get18(uint8_t* o) { return F<uint32_t>(o, 0x18); }
RT3_REPLACE(0x53ef60, Obj_Get18);
int32_t __fastcall Obj_Zero18And14(uint8_t* o) { return !F<int32_t>(o, 0x18) && !F<int32_t>(o, 0x14) ? 1 : 0; }
RT3_REPLACE(0x53ef70, Obj_Zero18And14);

// 0x53ef90. Sets the text (+0x1c, heap copy) and value +0x20.
void __fastcall Obj_SetText1c(uint8_t* o, int, const char* s, uint32_t v) {
    if (F<void*>(o, 0x1c)) Free(F<void*>(o, 0x1c));
    F<void*>(o, 0x1c) = nullptr;
    if (s) {
        size_t n = 0;
        while (s[n]) ++n;
        char* p = (char*)Malloc(n + 1);
        F<char*>(o, 0x1c) = p;
        for (size_t k = 0; (p[k] = s[k]) != 0; ++k) {}
    }
    F<uint32_t>(o, 0x20) = v;
}
RT3_REPLACE(0x53ef90, Obj_SetText1c);

void __fastcall Widget_SetVtbl(uint8_t* w) { F<uint32_t>(w, 0) = 0x5dd860; }
RT3_REPLACE(0x53f7d0, Widget_SetVtbl);
uint8_t* __fastcall Widget_SetVtblRet(uint8_t* w) {
    F<uint32_t>(w, 0) = 0x5dd860;
    return w;
}
RT3_REPLACE(0x53f7e0, Widget_SetVtblRet);

// 0x53f830. Child widget (+0x70, next +0x8e) with id +4.
uint8_t* __fastcall Widget_FindChild(uint8_t* w, int, int32_t id) {
    for (uint8_t* c = F<uint8_t*>(w, 0x70); c; c = F<uint8_t*>(c, 0x8e))
        if ((int32_t)F<uint16_t>(c, 4) == id) return c;
    return nullptr;
}
RT3_REPLACE(0x53f830, Widget_FindChild);

// 0x53f8d0. Sets the rectangle (+5/+9/+0xd/+0x11; 0x5c9b08 = keep); a size change
// is passed to every child (vtbl +8 with +0x64, +0x68, w, h).
void __fastcall Widget_SetRect(uint8_t* w, int, float x, float y, float wd, float ht) {
    const float keep = *(const float*)0x5c9b08;
    if (x == keep) x = F<float>(w, 5);
    if (y == keep) y = F<float>(w, 9);
    if (wd == keep) wd = F<float>(w, 0xd);
    if (ht == keep) ht = F<float>(w, 0x11);
    bool changed = !(F<float>(w, 0xd) == wd) || !(ht == F<float>(w, 0x11));
    F<float>(w, 0x11) = ht;
    F<float>(w, 5) = x;
    F<float>(w, 9) = y;
    F<float>(w, 0xd) = wd;
    if (!changed) return;
    for (uint8_t* c = F<uint8_t*>(w, 0x70); c; c = F<uint8_t*>(c, 0x8e))
        ((void(__thiscall*)(void*, uint32_t, uint32_t, uint32_t, uint32_t))(*(void***)c)[8 / 4])(
            c, F<uint32_t>(w, 0x64), F<uint32_t>(w, 0x68), F<uint32_t>(w, 0xd), F<uint32_t>(w, 0x11));
}
RT3_REPLACE(0x53f8d0, Widget_SetRect);

// 0x53fda0. Redraws the visible widget's children (vtbl +0x18), resetting the
// texture queue for flagged ones (+0x42).
void __fastcall Widget_Redraw(uint8_t* w) {
    if (!F<int32_t>(w, 0x5c)) return;
    Engine_Nop();
    for (uint8_t* c = F<uint8_t*>(w, 0x70); c; c = F<uint8_t*>(c, 0x8e)) {
        if (c[0x42] && F<int32_t>(w, 0x5c)) Engine_ResetTextureQueue(hook::Global<void*>(0x6d4024));
        ((void(__thiscall*)(void*))(*(void***)c)[0x18 / 4])(c);
    }
    if (F<int32_t>(w, 0x1d)) Engine_ClearRepaint(hook::Global<void*>(0x6d4024));
}
RT3_REPLACE(0x53fda0, Widget_Redraw);

// 0x540200. Control defaults.
void __fastcall Control_Init(uint8_t* c) {
    F<uint16_t>(c, 6) = 1;
    for (int k = 0; k < 8; ++k) F<uint16_t>(c, 0xa2 + k * 2) = 0;
    F<int32_t>(c, 0xb2) = 0;
    F<int32_t>(c, 0xb6) = 0;
    F<int32_t>(c, 0xba) = 0;
    F<int32_t>(c, 0xbe) = 0;
    c[0xc2] = 0;
    c[0xc3] = 0;
    F<uint32_t>(c, 0xc4) = 0x3b03126f;
    F<uint32_t>(c, 0xc8) = 0x3b03126f;
    c[0xcc] = 0;
    c[0xcd] = 0;
    F<uint16_t>(c, 0xd2) = 0;
    F<int32_t>(c, 0xce) = 0;
    c[0xe8] = 0;
}
RT3_REPLACE(0x540200, Control_Init);

} // namespace rt3
