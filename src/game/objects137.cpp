// Track light ctor and relight / pack name table contains, skip, seek, open file, size in blocks 0x48..0x53 (batch 140). See objects.cpp for conventions.
#include <windows.h>
#include "hook/hook.h"
#include "core/crt.h"
#include "game/collection.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

uint8_t* __fastcall Light_Construct(uint8_t* o);
void __fastcall Node_UpdateLightFromNeighbors(uint8_t* n);
int32_t __fastcall NameTable_Locate(uint8_t* t, int, int32_t id, int32_t* pack, int32_t* bucket, int32_t* index,
                                    int32_t fatal);
int32_t __fastcall NameTable_Read(uint8_t* t, int, void* dst, int32_t n);
void __fastcall NameTable_CloseCurrent(uint8_t* t);
int32_t __fastcall NameTable_OpenPack(uint8_t* t);
uint8_t* __fastcall NameTable_Nth(uint8_t* t, int, int32_t n);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);

static inline int32_t Lseek(int32_t fd, int32_t pos, int32_t origin) {
    return ((int32_t(__cdecl*)(int32_t, int32_t, int32_t))0x5ac18d)(fd, pos, origin);
}
static inline FILE* Fdopen(int32_t fd, const char* mode) { return ((FILE*(__cdecl*)(int32_t, const char*))0x5c7905)(fd, mode); }

// 0x489e60. Constructs a track light (vtable 0x5cfce0, flag +0x201 bit 0 clear).
uint8_t* __fastcall TrackLight_Construct(uint8_t* o) {
    Light_Construct(o);
    o[0x201] &= 0xfe;
    F<uint32_t>(o, 0) = 0x5cfce0;
    return o;
}
RT3_REPLACE(0x489e60, TrackLight_Construct);

// 0x497ea0. Walks the track from node `start` in direction dir (+0x20a ahead, else
// +0x206) until a junction (+0x20e), the end or back at the start, relighting the
// nodes whose stamp (+0x240) is current (collection +0x1d5).
void __fastcall Track_RelightChain(Collection* c, int, int32_t start, int32_t dir, int32_t) {
    uint8_t* n = (uint8_t*)Collection_Get(c, 0, start);
    int32_t prev = F<int32_t>(n, 0x202);
    int32_t next = dir ? F<int32_t>(n, 0x20a) : F<int32_t>(n, 0x206);
    while (next != -1) {
        uint8_t* m = (uint8_t*)Collection_Get(c, 0, next);
        if (F<uint32_t>(m, 0x240) >= F<uint32_t>(c, 0x1d5)) Node_UpdateLightFromNeighbors(m);
        if (F<int32_t>(m, 0x20e) != -1) next = -1;
        else next = F<int32_t>(m, 0x206) == prev ? F<int32_t>(m, 0x20a) : F<int32_t>(m, 0x206);
        prev = F<int32_t>(m, 0x202);
        if (prev == start) break;
    }
}
RT3_REPLACE(0x497ea0, Track_RelightChain);

// 0x53b590. Whether name id is in the pack name table (NameTable_Locate's result).
int32_t __fastcall NameTable_Contains(uint8_t* t, int, int32_t id, int32_t fatal) {
    int32_t pack, bucket, index;
    return NameTable_Locate(t, 0, id, &pack, &bucket, &index, fatal);
}
RT3_REPLACE(0x53b590, NameTable_Contains);

// 0x53b830. Skips a dword of the current pack.
int32_t __fastcall NameTable_Skip4(uint8_t* t, int, int32_t) {
    int32_t v;
    return NameTable_Read(t, 0, &v, 4);
}
RT3_REPLACE(0x53b830, NameTable_Skip4);

// Byte position of entry (pack, bucket, index) in its pack: the pack's data start
// (+0x1f404 x +0x417ec) + the entry's offset (+8) + 8.
static int32_t EntryPos(uint8_t* t, int32_t pack, int32_t bucket, int32_t index) {
    int32_t base = F<int32_t>(t, 0x1f404 + pack * 4) * F<int32_t>(t, 0x417ec + pack * 4);
    uint8_t* list = F<uint8_t*>(t, (pack * 32 + bucket) * 4);
    return base + F<int32_t>(list + index * 0xd, 8) + 8;
}

// 0x53bd40. Positions the pack file at byte `offset` of name id (making its pack
// current, +0x427bc; counting seeks +0x428f4); loose files are opened instead.
int32_t __fastcall NameTable_Seek(uint8_t* t, int, int32_t id, int32_t offset) {
    if (F<int32_t>(t, 0x427bc) < 0) NameTable_CloseCurrent(t);
    int32_t pack, bucket, index;
    NameTable_Locate(t, 0, id, &pack, &bucket, &index, 1);
    F<int32_t>(t, 0x427bc) = pack;
    if (pack >= 0) {
        ++F<int32_t>(t, 0x428f4);
        return Lseek(F<int32_t>(t, 0x204ac + pack * 4), EntryPos(t, pack, bucket, index) + offset, 0);
    }
    int32_t fd = NameTable_OpenPack(t);
    if (offset) return Lseek(fd, offset, 0);
    return 0;
}
RT3_REPLACE(0x53bd40, NameTable_Seek);

// 0x53be10. A stdio stream on name id ("r" when text, else "rb"; one cached per
// pack, +0x2144c), positioned at its data; 0 when the open failed for binary.
FILE* __fastcall NameTable_OpenFile(uint8_t* t, int, int32_t id, uint8_t text) {
    if (F<int32_t>(t, 0x427bc) < 0) NameTable_CloseCurrent(t);
    int32_t pack, bucket, index;
    NameTable_Locate(t, 0, id, &pack, &bucket, &index, 1);
    F<int32_t>(t, 0x427bc) = pack;
    if (pack < 0) {
        NameTable_OpenPack(t);
        uint8_t* e = NameTable_Nth(t, 0, -pack);
        if (!F<FILE*>(e, 8)) F<FILE*>(e, 8) = Fdopen(F<int32_t>(e, 4), (const char*)0x5c9678);
        return F<FILE*>(e, 8);
    }
    ++F<int32_t>(t, 0x428f4);
    Lseek(F<int32_t>(t, 0x204ac + pack * 4), EntryPos(t, pack, bucket, index), 0);
    int32_t p = F<int32_t>(t, 0x427bc);
    if (!F<FILE*>(t, 0x2144c + p * 4))
        F<FILE*>(t, 0x2144c + F<int32_t>(t, 0x427bc) * 4) =
            Fdopen(F<int32_t>(t, 0x204ac + p * 4), (const char*)(text ? 0x5c89e8 : 0x5c9678));
    if (*((int*(__cdecl*)())0x5a3f5b)() && !text) return nullptr;
    return F<FILE*>(t, 0x2144c + F<int32_t>(t, 0x427bc) * 4);
}
RT3_REPLACE(0x53be10, NameTable_OpenFile);

// 0x53c930. Whether name + suffix is in the pack name table.
RT3_NOINLINE int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix) {
    char buf[0x138];
    char* d = buf;
    while ((*d++ = *name++) != 0) {}
    if (suffix) {
        --d;
        while ((*d++ = *suffix++) != 0) {}
    }
    int32_t pack, bucket, index;
    return NameTable_Locate(t, 0, NameTable_Hash(t, 0, buf), &pack, &bucket, &index, fatal);
}
RT3_REPLACE(0x53c930, NameTable_ContainsName);

// 0x53c9c0. Size of name id's data (entry +4; loose files: +0xc).
int32_t __fastcall NameTable_EntrySize(uint8_t* t, int, int32_t id) {
    int32_t pack, bucket, index;
    NameTable_Locate(t, 0, id, &pack, &bucket, &index, 1);
    if (pack < 0) return F<int32_t>(NameTable_Nth(t, 0, -pack), 0xc);
    return F<int32_t>(F<uint8_t*>(t, (pack * 32 + bucket) * 4) + index * 0xd, 4);
}
RT3_REPLACE(0x53c9c0, NameTable_EntrySize);

void __fastcall NameTable_ScanDirs(uint8_t* t);
void __fastcall NameTable_Grow(uint8_t* t);
int32_t __fastcall NameTable_ContainsName(uint8_t* t, int, const char* name, int32_t fatal, const char* suffix);
int32_t __fastcall NameTable_Hash(uint8_t* t, int, const char* name);
void __fastcall Anims_LoadPack(uint8_t* o, int, const char* path, int32_t);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3, const char* a4,
                            const char* a5, const char* a6, const char* a7, const char* a8);

static Collection* NewTable(int32_t stride) {
    Collection* c = (Collection*)((void*(__cdecl*)(size_t))0x53b070)(0x88);
    return c ? Collection_Construct(c, 0, 1, stride, 10, 5, 0, 0, 0) : nullptr;
}

// Reads the next whitespace-delimited token of `f` into `dst`.
static void Token(FILE* f, char* dst) { ((int(__cdecl*)(FILE*, const char*, ...))0x5a17e1)(f, (const char*)0x5c89e4, dst); }
static void AddImb(char* s) { memcpy(s + strlen(s), (const void*)0x5dd630, 5); }

// 0x53cb60. Constructs the file system: clears its state, creates the art tables
// (company logos 0x20, portraits 0x60, train skins 0xc0 bytes) and the index list
// 0xccba50, scans the loose directories, then mounts every .pk4 archive of the 13
// data folders (0x624e14) from the highest name down, and reads the art indexes
// listed in 0xccba50 ("companyLogo name", "portrait a b c", "trainSkin a b c d e f";
// image names get .imb).
uint8_t* __fastcall NameTable_Construct(uint8_t* t) {
    memset(t + 0x4278c, 0, 9 * 4);
    F<int32_t>(t, 0x1f400) = 0;
    for (int32_t k = 0; k < 0x20; ++k) {
        F<int32_t>(t, 0x203a4 + k * 4) = 0;
        F<int32_t>(t, 0x2042c + k * 4) = 0;
    }
    F<int32_t>(t, 0x20424) = 0;
    memset(t + 0x4290c, 0, 10 * 4);
    F<int32_t>(t, 0x429ac) = 0;
    F<int32_t>(t, 0x427b0) = -1;
    F<int32_t>(t, 0x427b4) = 0;
    F<int32_t>(t, 0x427b8) = 0;
    F<int32_t>(t, 0x427bc) = 0;
    t[0x427c0] = 0;
    for (int32_t k = 0; k < 8; ++k) F<int32_t>(t, 0x428ec + k * 4) = 0;
    F<Collection*>(t, 0x429b0) = NewTable(0x20);
    F<Collection*>(t, 0x429b4) = NewTable(0x60);
    F<Collection*>(t, 0x429b8) = NewTable(0xc0);
    hook::Global<Collection*>(0xccba50) = NewTable(0x20);
    NameTable_ScanDirs(t);
    NameTable_Grow(t);
    memset(t + 0x2144c, 0, 1000 * 4);
    for (uint32_t dir = 0x624e14; dir < 0x624e48; dir += 4) {
        const char* folder = *(const char**)dir;
        char limit[0x130];
        memcpy(limit, (const void*)0x5dd64c, 9);
        for (;;) {
            char pattern[0x130], best[0x130];
            FormatText(pattern, (const char*)0x5dd644, folder, 0, 0, 0, 0, 0, 0, 0);
            best[0] = 0;
            bool found = false;
            WIN32_FIND_DATAA fd;
            HANDLE h = ((HANDLE(__stdcall*)(const char*, WIN32_FIND_DATAA*))hook::Global<void*>(0x5c8190))(pattern, &fd);
            if (h == INVALID_HANDLE_VALUE) break;
            do {
                if (Stricmp(fd.cFileName, best) > 0 && Stricmp(fd.cFileName, limit) < 0) {
                    strcpy(best, fd.cFileName);
                    found = true;
                }
            } while (((BOOL(__stdcall*)(HANDLE, WIN32_FIND_DATAA*))hook::Global<void*>(0x5c8194))(h, &fd));
            ((BOOL(__stdcall*)(HANDLE))hook::Global<void*>(0x5c819c))(h);
            if (!found) break;
            char path[0x320];
            FormatText(path, (const char*)0x5c8730, folder, best, 0, 0, 0, 0, 0, 0);
            Anims_LoadPack(t, 0, path, (int32_t)folder);
            strcpy(limit, best);
        }
    }
    Collection* lists = hook::Global<Collection*>(0xccba50);
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0xccba50)); ++i) {
        lists = hook::Global<Collection*>(0xccba50);
        const char* name = (const char*)Collection_Get(lists, 0, Collection_IdAt(lists, 0, i, 0));
        if (!NameTable_ContainsName(t, 0, name, 0, nullptr)) break;
        FILE* f = NameTable_OpenFile(t, 0, NameTable_Hash(t, 0, name), 1);
        char kind[0x400];
        Token(f, kind);
        if (!Stricmp(kind, (const char*)0x5dd638)) {
            char rec[0x400] = {};
            Token(f, rec);
            AddImb(rec);
            Collection* c = F<Collection*>(t, 0x429b0);
            c->vtbl->allocate(c, rec, 0, 0, 0);
        } else if (!Stricmp(kind, (const char*)0x5dd624)) {
            char rec[0x600] = {};
            Token(f, rec);
            Token(f, rec + 0x20);
            Token(f, rec + 0x40);
            AddImb(rec + 0x40);
            Token(f, rec + 0x60);
            Token(f, rec + 0x80);
            AddImb(rec + 0x80);
            Token(f, rec + 0xa0);
            Collection* c = F<Collection*>(t, 0x429b8);
            c->vtbl->allocate(c, rec, 0, 0, 0);
        } else if (!Stricmp(kind, (const char*)0x5dd618)) {
            char rec[0x600] = {};
            Token(f, rec);
            Token(f, rec + 0x20);
            AddImb(rec + 0x20);
            Token(f, rec + 0x40);
            AddImb(rec + 0x40);
            Collection* c = F<Collection*>(t, 0x429b4);
            c->vtbl->allocate(c, rec, 0, 0, 0);
        }
    }
    return t;
}
RT3_REPLACE(0x53cb60, NameTable_Construct);

} // namespace rt3
