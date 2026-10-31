// 2D building models / pack file loads (animations, emitters) / emitter file iteration / pack position stack and blocks in blocks 0x41..0x53 (batch 141). See objects.cpp for conventions.
#include "hook/hook.h"
#include "core/crt.h"
#include "core/containers.h"
#include <stdint.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id);
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
void __fastcall NameTable_CloseCurrent(uint8_t* t);
int32_t __fastcall NameTable_OpenPack(uint8_t* t);

static inline uint8_t* Packs() { return hook::Global<uint8_t*>(0x6d4020); }
#define SPRINTF ((int(__cdecl*)(char*, const char*, ...))0x5a19c4)

static inline void Format0(char* dst, uint32_t fmt, const char* a1 = nullptr) {
    FormatText(dst, (const char*)fmt, a1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

// 0x413bc0. The 2D model file of the building type into out: "2D<name>.imb" if it
// exists, else the house set model of its style (+0x7c "<style>houses"; multi-family
// from size 2, +0x141), else the commercial S/M/L model for names CommercialX00..09,
// else the default building.
void __fastcall BuildingType_Model2D(uint8_t* o, int, char* out) {
    char buf[0x120];
    const char* name = (const char*)o + 0x40;
    Format0(out, 0x5c9510, name);
    if (NameTable_ContainsName(Packs(), 0, out, 0, nullptr)) return;
    for (uint32_t a = 0x5f3c80; a < 0x5f3c98; a += 4) {
        const char* style = *(const char* const*)a;
        SPRINTF(buf, (const char*)0x5c9504, style);
        if (Stricmp((const char*)o + 0x7c, buf)) continue;
        Format0(out, F<int32_t>(o, 0x141) >= 2 ? 0x5c94ec : 0x5c94d4, *(const char* const*)a);
    }
    if (NameTable_ContainsName(Packs(), 0, out, 0, nullptr)) return;
    static const uint32_t pat[3] = { 0x5c94c4, 0x5c94a0, 0x5c947c }, model[3] = { 0x5c94b0, 0x5c948c, 0x5c9468 };
    for (int32_t i = 0; i < 10; ++i)
        for (int k = 0; k < 3; ++k) {
            SPRINTF(buf, (const char*)pat[k], i);
            if (!Stricmp(name, buf)) Format0(out, model[k]);
        }
    if (!NameTable_ContainsName(Packs(), 0, out, 0, nullptr)) Format0(out, 0x5c9450);
}
RT3_REPLACE(0x413bc0, BuildingType_Model2D);

// Loads a whole pack file by id into a new buffer (0 if empty).
static void* LoadWhole(int32_t id, int32_t* sizeOut) {
    int32_t size = NameTable_EntrySize(Packs(), 0, id);
    *sizeOut = size;
    if (size <= 0) return nullptr;
    NameTable_Seek(Packs(), 0, id, 0);
    void* p = Malloc(size);
    NameTable_Read(Packs(), 0, p, size);
    return p;
}

// 0x45ae80. Loads "<name>_Anim.3dp" from the packs (0 if missing or empty);
// creates the empty animation list 0x6acd5c on first use.
void* __fastcall Anim_LoadFile(const char* name) {
    char buf[0x100];
    Format0(buf, 0x5cb4b0, name);
    if (!NameTable_ContainsName(Packs(), 0, buf, 0, nullptr)) return nullptr;
    int32_t size;
    void* p = LoadWhole(NameTable_Hash(Packs(), 0, buf), &size);
    if (!p) return nullptr;
    if (!hook::Global<void*>(0x6acd5c)) {
        uint32_t* l = (uint32_t*)((void*(__cdecl*)(size_t))0x53b070)(0x10);
        if (l) {
            l[0] = 0;
            l[1] = 0;
            l[2] = 0;
        }
        l[0] = 0;
        l[1] = 0;
        l[2] = 0;
        hook::Global<uint32_t*>(0x6acd5c) = l;
        l[3] = 0;
    }
    return p;
}
RT3_REPLACE(0x45ae80, Anim_LoadFile);

// 0x474790. (Re)loads emitters.dat (0x6cea60) from the packs.
void __cdecl Emitters_Load() {
    if (hook::Global<void*>(0x6cea60)) {
        Free(hook::Global<void*>(0x6cea60));
        hook::Global<void*>(0x6cea60) = nullptr;
    }
    if (!NameTable_ContainsName(Packs(), 0, (const char*)0x5ce4d8, 0, nullptr)) return;
    int32_t id = NameTable_Hash(Packs(), 0, (const char*)0x5ce4d8);
    int32_t size = NameTable_EntrySize(Packs(), 0, id);
    if (size <= 0) return;
    NameTable_Seek(Packs(), 0, id, 0);
    void* p = Malloc(size);
    hook::Global<void*>(0x6cea60) = p;
    NameTable_Read(Packs(), 0, p, size);
}
RT3_REPLACE(0x474790, Emitters_Load);

// 0x474860. Iterates the emitter model files "<prefix><emitter name><n>.3dp" (n
// from 1, up to 254; emitters 0x140 bytes from emitters.dat +0xc, count +8):
// returns 0x6cea90 = {emitter*, file name} for the next one present (only pack
// entries when 0x6cea64), or 0 at the end. restart == 1 starts over.
uint8_t* __fastcall Emitters_NextFile(const char* prefix, int32_t restart) {
    if (restart == 1) {
        hook::Global<int32_t>(0x5f2c30) = restart;
        hook::Global<int32_t>(0x6ceb94) = 0;
    }
    int32_t n = hook::Global<int32_t>(0x5f2c30);
    int32_t idx = hook::Global<int32_t>(0x6ceb94);
    uint8_t* data = hook::Global<uint8_t*>(0x6cea60);
    uint8_t* e = data + 0xc + idx * 0x140;
    char buf[0x100];
    for (; idx < F<int32_t>(data, 8); ++idx, e += 0x140) {
        if (n >= 0xff) continue;
        SPRINTF(buf, (const char*)0x5ce4e8, prefix, (const char*)e + 4, n);
        int32_t r = NameTable_ContainsName(Packs(), 0, buf, 0, nullptr);
        if ((!hook::Global<int32_t>(0x6cea64) || r == 2) && r) {
            hook::Global<int32_t>(0x6ceb94) = idx;
            hook::Global<int32_t>(0x5f2c30) = n + 1;
            hook::Global<uint8_t*>(0x6cea90) = e;
            char* d = (char*)0x6cea94;
            for (const char* s = buf; (*d++ = *s++) != 0;) {}
            return (uint8_t*)0x6cea90;
        }
        data = hook::Global<uint8_t*>(0x6cea60);
        n = 1;
        hook::Global<int32_t>(0x5f2c30) = 1;
    }
    hook::Global<int32_t>(0x5f2c30) = 1;
    hook::Global<int32_t>(0x6ceb94) = 0;
    return nullptr;
}
RT3_REPLACE(0x474860, Emitters_NextFile);

// 0x53ca90. Restores the last pushed pack position (+0x20428 stack of {pos, pack,
// next}): makes its pack current and seeks back; counts pops (+0x428f8).
RT3_NOINLINE void __fastcall NameTable_PopPosition(uint8_t* t) {
    if (!F<void*>(t, 0x20428)) return;
    ++F<int32_t>(t, 0x428f8);
    if (F<int32_t>(t, 0x427bc) < 0) NameTable_CloseCurrent(t);
    int32_t pack = F<int32_t>(F<uint8_t*>(t, 0x20428), 4);
    F<int32_t>(t, 0x427bc) = pack;
    int32_t fd = pack < 0 ? NameTable_OpenPack(t) : F<int32_t>(t, 0x204ac + pack * 4);
    ++F<int32_t>(t, 0x428f4);
    ((int32_t(__cdecl*)(int32_t, int32_t, int32_t))0x5ac18d)(fd, F<int32_t>(F<uint8_t*>(t, 0x20428), 0), 0);
    uint8_t* top = F<uint8_t*>(t, 0x20428);
    F<uint8_t*>(t, 0x20428) = F<uint8_t*>(top, 8);
    Free(top);
}
RT3_REPLACE(0x53ca90, NameTable_PopPosition);

// 0x53cb10. Reads a block of the current pack: a skipped dword, the size and the
// data into a new buffer (*out; untouched when empty).
int32_t __fastcall NameTable_ReadBlock(uint8_t* t, int, int32_t, void** out) {
    int32_t skip, size;
    NameTable_Read(t, 0, &skip, 4);
    NameTable_Read(t, 0, &size, 4);
    if (!size) return 0;
    void* p = Malloc(size);
    *out = p;
    return NameTable_Read(t, 0, p, size);
}
RT3_REPLACE(0x53cb10, NameTable_ReadBlock);

// 0x53dbe0. Thunk to Hash_Destroy.
void __fastcall Hash_DestroyThunk(HashMap* m) { Hash_Destroy(m); }
RT3_REPLACE(0x53dbe0, Hash_DestroyThunk);

} // namespace rt3
