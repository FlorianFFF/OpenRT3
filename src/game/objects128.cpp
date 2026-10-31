// Fine map bits / regional names / effect scale / music playlist / minimap / site picking / slots / load in blocks 0x44..0x47 (batch 131). See objects.cpp for conventions.
#include "hook/hook.h"
#include "game/collection.h"
#include "core/crt.h"
#include "core/stream.h"
#include <stdint.h>
#include <string.h>

namespace rt3 {

template <typename T> static inline T& F(void* o, uint32_t off) { return *(T*)((uint8_t*)o + off); }

void __fastcall Map_UpdateBlockMask(uint8_t* m, int, int32_t x, int32_t y);
void __fastcall StripExtension(char* s);
int __fastcall ToUpper(char c);
void __fastcall StrCopyN(char* dst, const char* src, size_t n);
float __fastcall Effects_Multiplier(uint8_t* w, int, int32_t idx, uint32_t company, uint32_t player, uint32_t region);
void __fastcall Playlist_Next(uint8_t* p);
const char* __fastcall GetString(int32_t id);
char* __fastcall FormatText(char* dst, const char* fmt, const char* a1, const char* a2, const char* a3,
                            const char* a4, const char* a5, const char* a6, const char* a7, const char* a8);
uint32_t __fastcall Log_Add(Collection* t, int, const char* name, int32_t b, int32_t c, int32_t d);
int32_t __fastcall Sound_PlayMusic(uint8_t* s, int, const char* file, void* onEnd);
float __fastcall Obj_NodeCenterX(void* o);
float __fastcall Obj_NodeCenterY(void* o);
int32_t __fastcall Obj_BuildingStateActive(void* o);
void __fastcall Obj_CenterCell(void* o, int, float* x, float* y);
double __stdcall Distance2D(float x1, float y1, float x2, float y2);
int32_t __fastcall HashRand(int32_t seed, int32_t lo, int32_t hi);
void __fastcall Obj_AllocEntries(void* o);
char* __fastcall StrCopy(char* dst, const char* src);

static inline double D(uint32_t a) { return *(const double*)a; }
static inline uint8_t* World() { return hook::Global<uint8_t*>(0x6cec78); }

static inline void SetBit(uint8_t* bits, int32_t idx, int32_t on) {
    uint8_t b = (uint8_t)(1 << (idx & 7));
    if (on) bits[idx >> 3] |= b;
    else bits[idx >> 3] &= (uint8_t)~b;
}

// 0x4497a0. Sets or clears fine cell (x, y) in bit grid `which` (0 = +0x2139 and
// +0x213d, 1 = +0x213d, 2 = +0x2139, 3 = +0x2141); with `update` refreshes the
// block mask of the cell's block and of the neighbours it borders.
void __fastcall Map_SetFineBit(uint8_t* m, int, int32_t x, int32_t y, int32_t on, int32_t which, int32_t update) {
    int32_t w = F<int32_t>(m, 0x215d);
    int32_t idx = w * y + x;
    if (idx < 0 || idx >= F<int32_t>(m, 0x2161) * w) return;
    if (which == 0 || which == 2) SetBit(F<uint8_t*>(m, 0x2139), idx, on);
    if (which == 0 || which == 1) SetBit(F<uint8_t*>(m, 0x213d), idx, on);
    if (which == 3) SetBit(F<uint8_t*>(m, 0x2141), idx, on);
    if (!update) return;
    int32_t bx = x >> 2, by = y >> 2;
    if (bx <= 0 || by <= 0 || bx >= F<int32_t>(m, 0x2155) || by >= F<int32_t>(m, 0x2159)) return;
    Map_UpdateBlockMask(m, 0, bx, by);
    if (bx >= F<int32_t>(m, 0x2155) - 1 || by >= F<int32_t>(m, 0x2159) - 1) return;
    int32_t xm = x & 3, ym = y & 3;
    if (xm == 0 || xm == 3) {
        int32_t nx = xm == 0 ? bx - 1 : bx + 1;
        Map_UpdateBlockMask(m, 0, nx, by);
        if (ym == 0) {
            Map_UpdateBlockMask(m, 0, bx, by - 1);
            Map_UpdateBlockMask(m, 0, nx, by - 1);
        }
        if (ym == 3) {
            Map_UpdateBlockMask(m, 0, bx, by + 1);
            Map_UpdateBlockMask(m, 0, nx, by + 1);
        }
        return;
    }
    if (ym == 0) Map_UpdateBlockMask(m, 0, bx, by - 1);
    else if (ym == 3) Map_UpdateBlockMask(m, 0, bx, by + 1);
}
RT3_REPLACE(0x4497a0, Map_SetFineBit);

// 0x461830. Renames the collection's elements (+0x2a2) to their variant for the
// current region set (0x6d3c84 -> column 2..6 of table 0x5f0fe8, 0x1c-byte rows
// {map key, name, variants...}) when the row's key matches the map name (its
// extension stripped; the tutorials count as "Italy").
void __fastcall Names_ApplyRegionVariants(Collection* c, int, char* mapName) {
    char key[0x1fc];
    StripExtension(mapName);
    StrCopy(key, mapName);
    if (!Stricmp(key, (const char*)0x5ce05c) || !Stricmp(key, (const char*)0x5ce050))
        memcpy(key, (const void*)0x5cc9d8, 6);
    int32_t col;
    switch (hook::Global<int32_t>(0x6d3c84)) {
    case 2: col = 2; break;
    case 1: col = 3; break;
    case 3: col = 4; break;
    case 4: col = 5; break;
    case 0: col = 6; break;
    default: return;
    }
    for (int32_t i = 0; i < Collection_Count(c); ++i) {
        uint8_t* e = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        for (const char* const* row = (const char* const*)0x5f0fe8; (uint32_t)row < 0x5f2ad0; row += 7) {
            if ((char)ToUpper(row[0][0]) != (char)ToUpper(key[0])) continue;
            if (Stricmp(row[0], key)) continue;
            if (Stricmp(row[1], (const char*)e + 0x2a2)) continue;
            StrCopyN((char*)e + 0x2a2, row[col], 0x27);
        }
    }
}
RT3_REPLACE(0x461830, Names_ApplyRegionVariants);

// 0x4619c0. o's base value (+4) scaled by the effects 0x46 + kind (+0x10) and 0x45,
// and 0x160 + id (+0) when that is an effect, for company / player.
float __fastcall Obj_EffectScale(uint8_t* o, int, uint32_t company, uint32_t player) {
    float base = F<float>(o, 4);
    float m1 = Effects_Multiplier(World(), 0, F<int32_t>(o, 0x10) + 0x46, company, player, 0);
    float m2 = Effects_Multiplier(World(), 0, 0x45, company, player, 0);
    __asm {
        fld m2
        fmul m1
        fmul base
        fstp base
    }
    int32_t id = F<int32_t>(o, 0) + 0x160;
    if (id >= 0x160 && id <= 0x1c4)
        base = (float)((double)Effects_Multiplier(World(), 0, id, company, player, 0) * base);
    return base;
}
RT3_REPLACE(0x4619c0, Obj_EffectScale);

// 0x4687f0. Starts the playlist's current track (+0x108 names of 0x104, index
// +0x10c) when nothing plays (0x6ada98) and it has tracks (+0x100); on a pending
// advance (0x5f2b24) moves to the next and, with the log on, announces its title.
// Quirk: without a path the extension is cut off the playlist's own name.
void __fastcall Playlist_PlayCurrent(uint8_t* p) {
    if (hook::Global<int32_t>(0x6ada98) || F<int32_t>(p, 0x100) <= 0) return;
    if (hook::Global<int32_t>(0x5f2b24) == 1) {
        hook::Global<int32_t>(0x5f2b24) = 0;
        Playlist_Next(p);
        if (hook::Global<int32_t>(0x6d401c)) {
            char msg[0x100], path[0x100];
            char* cur = F<char*>(p, 0x108) + F<int32_t>(p, 0x10c) * 0x104;
            StrCopy(path, cur);
            char* title = ((char*(__cdecl*)(const char*, int))0x5a1df0)(path, '\\');
            title = title ? title + 1 : F<char*>(p, 0x108) + F<int32_t>(p, 0x10c) * 0x104;
            char* dot = ((char*(__cdecl*)(const char*, int))0x5a2a00)(title, '.');
            if (dot) *dot = 0;
            if (F<int32_t>(hook::Global<uint8_t*>(0x6d4024), 0x114712) > 0) {
                char* prev = (char*)0x6adab8;
                if (!*prev) FormatText(msg, GetString(0x1c0), title, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                else FormatText(msg, GetString(0x1c1), title, prev, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
                if (hook::Global<Collection*>(0x6acd34)) Log_Add(hook::Global<Collection*>(0x6acd34), 0, msg, 3, 1, 0);
            }
            StrCopy((char*)0x6adab8, title);
        }
    }
    hook::Global<int32_t>(0x6ada98) = Sound_PlayMusic(hook::Global<uint8_t*>(0x6d402c), 0,
                                                      F<char*>(p, 0x108) + F<int32_t>(p, 0x10c) * 0x104,
                                                      (void*)0x4686d0);
}
RT3_REPLACE(0x4687f0, Playlist_PlayCurrent);

// 0x477c10. Converts world (x, y) to the minimap's pixel position: zoomed views
// (+0x33 & 0x2030000) map the view rect (+0xb/+0xf origin plus the scroll at
// +0x86 +5/+9, size +0x13/+0x17) onto pixels +0xc3..+0xcb / +0xc7..+0xcf (y flipped);
// otherwise the whole map, letterboxed to its larger side and flipped in y.
void __fastcall Minimap_WorldToPixel(uint8_t* o, int, float x, float y, float* outX, float* outY) {
    if (F<uint32_t>(o, 0x33) & 0x2030000) {
        uint8_t* s = F<uint8_t*>(o, 0x86);
        float w = (float)(F<int32_t>(o, 0xcb) - F<int32_t>(o, 0xc3));
        *outX = (float)(((double)x - ((double)F<float>(s, 5) + F<float>(o, 0xb))) / F<float>(o, 0x13) * w +
                        (float)F<int32_t>(o, 0xc3));
        *outY = (float)((D(0x5c8770) - ((double)y - ((double)F<float>(s, 9) + F<float>(o, 0xf))) / F<float>(o, 0x17)) *
                            (double)(F<int32_t>(o, 0xcf) - F<int32_t>(o, 0xc7)) +
                        (double)F<int32_t>(o, 0xc7));
        return;
    }
    uint8_t* m = hook::Global<uint8_t*>(0x62c120);
    int32_t w = F<int32_t>(m, 0x2155), h = F<int32_t>(m, 0x2159);
    int32_t big = w > h ? w : h;
    float size = (float)big;
    *outX = (float)(((double)x - F<float>(o, 0xb)) / F<float>(o, 0x13) * size);
    float t = (float)(((double)y - F<float>(o, 0xf)) / F<float>(o, 0x17) * size);
    *outY = t;
    if (w > h) *outY = (float)((double)t - (double)(big - h) * D(0x5c8628));
    else *outX = (float)((double)*outX - (double)(big - w) * D(0x5c8628));
    *outY = (float)((double)(float)h - *outY);
}
RT3_REPLACE(0x477c10, Minimap_WorldToPixel);

// 0x479b50. Picks the building (0x6cec20; active ones at 0x5c8880 of their
// distance) nearest to (x, y) within the minimap's pick radius (+0xcb - +0xc3);
// failing that and with option 4 (0x6d4018 +0xa8c), the nearest 0x6cfca8 object
// within half of it (*other = its +0x202).
int32_t __fastcall Minimap_PickNearest(uint8_t* o, int, float x, float y, uint32_t* building, uint32_t* other) {
    *building = 0;
    *other = 0;
    float r = (float)((double)(F<int32_t>(o, 0xcb) - F<int32_t>(o, 0xc3)) / D(0x5cab00) * D(0x5c85c8));
    int32_t all = F<int32_t>(hook::Global<uint8_t*>(0x6d4018), 0xa8c) & 4;
    for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cec20)); ++i) {
        Collection* c = hook::Global<Collection*>(0x6cec20);
        uint8_t* s = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
        float cx = Obj_NodeCenterX(s);
        float cy = Obj_NodeCenterY(s);
        float d = (float)Distance2D(x, y, cx, cy);
        if (Obj_BuildingStateActive(s)) d = (float)((double)d * D(0x5c8880));
        else if (!all) continue;
        if (d < r) {
            r = d;
            *building = F<uint32_t>(s, 0);
        }
    }
    if (*building) return 1;
    r = (float)((double)r * D(0x5c8750));
    if (all) {
        for (int32_t i = 0; i < Collection_Count(hook::Global<Collection*>(0x6cfca8)); ++i) {
            Collection* c = hook::Global<Collection*>(0x6cfca8);
            uint8_t* t = (uint8_t*)Collection_Get(c, 0, Collection_IdAt(c, 0, i, 0));
            float cx, cy;
            Obj_CenterCell(t, 0, &cx, &cy);
            float d = (float)Distance2D(x, y, cx, cy);
            if (d < r) {
                r = d;
                *other = F<uint32_t>(t, 0x202);
            }
        }
    }
    return *other != 0 ? 1 : 0;
}
RT3_REPLACE(0x479b50, Minimap_PickNearest);

// 0x47bc80. Fills o's free slots (+0x3ae entries of 9 bytes at +0x31a, first byte
// = slot 1..40) with random free slots (seeded by 0x6cec7c +0x83, fixed at 7 in
// multiplayer), first making sure as many slots are enabled (+0x2c9 + k) as
// needed; 0x5f2d28 (41 x 0x27 bytes) marks slots taken. Not in the editor (+0x68).
void __fastcall Obj_AssignRandomSlots(uint8_t* o) {
    if (F<int32_t>(hook::Global<uint8_t*>(0x6cec74), 0x68)) return;
    uint8_t* taken = (uint8_t*)0x5f2d28;
    for (uint32_t a = 0x5f2d28; a < 0x5f3367; a += 0x27) *(uint8_t*)a = 0;
    for (int32_t i = 0; i < F<int32_t>(o, 0x3ae); ++i) {
        int8_t s = (int8_t)o[0x31a + i * 9];
        if (s) taken[s * 0x27] = 1;
    }
    int32_t nfree = 0;
    for (int k = 0; k < 40; ++k)
        if (o[0x2ca + k]) ++nfree;
    while (nfree < F<int32_t>(o, 0x3ae)) {
        for (int32_t k = 1; k < 0x29; ++k)
            if (!o[0x2c9 + k]) {
                o[0x2c9 + k] = 1;
                ++nfree;
                break;
            }
    }
    if (hook::Global<void*>(0x6cd8d8)) F<int32_t>(hook::Global<uint8_t*>(0x6cec7c), 0x83) = 7;
    for (int32_t i = 0; i < F<int32_t>(o, 0x3ae); ++i) {
        uint8_t* e = o + 0x31a + i * 9;
        if (*e) continue;
        int32_t n = HashRand(F<int32_t>(hook::Global<uint8_t*>(0x6cec7c), 0x83), 1, nfree);
        int32_t k = 0, tries = 0;
        if (n > 0) {
            while (nfree) {
                if (++k >= 0x29) k = 1;
                if (!taken[k * 0x27] && (o[0x2c9 + k] || tries > 200)) --n;
                if (++tries > 500) {
                    k = i + 10;
                    break;
                }
                if (n <= 0) break;
            }
        }
        *e = (uint8_t)k;
        --nfree;
        taken[k * 0x27] = 1;
    }
}
RT3_REPLACE(0x47bc80, Obj_AssignRandomSlots);

// 0x47d8e0. Loads o's arrays from stream s (chunk 0x4e21..0x4e24): +0x462 entries of
// 6 bytes (+0x466), 3 x +0x30 x 5 bytes (+0x24..), 5 lists of 8-byte items (counts
// +0x590, data +0x5a4) and +0x38 x 12 bytes (+0x34).
int32_t __fastcall Obj_LoadArrays(uint8_t* o, int, Stream s) {
    StreamSkip4(s, 0x4e21, 0, 0);
    StreamRead(s, o + 0x462, 4, 0, 1);
    F<void*>(o, 0x466) = nullptr;
    if (F<int32_t>(o, 0x462) > 0) {
        F<uint8_t*>(o, 0x466) = (uint8_t*)Malloc((F<int32_t>(o, 0x462) * 3 + 3) * 2);
        for (int32_t k = 0; k < F<int32_t>(o, 0x462); ++k) StreamRead(s, F<uint8_t*>(o, 0x466) + k * 6, 6, 0, 1);
    }
    if (F<int32_t>(o, 0x30) > 0) {
        int32_t n = F<int32_t>(o, 0x30) * 5;
        for (int j = 0; j < 3; ++j) {
            void* p = Malloc(n);
            F<void*>(o, 0x24 + j * 4) = p;
            StreamRead(s, p, n, 0, 1);
        }
    }
    for (int j = 0; j < 5; ++j) {
        int32_t n = F<int32_t>(o, 0x590 + j * 4);
        if (n <= 0) continue;
        void* p = Malloc(n * 8);
        F<void*>(o, 0x5a4 + j * 4) = p;
        StreamRead(s, p, n * 8, 0, 1);
    }
    if (F<int32_t>(o, 0x38) > 0) {
        int32_t n = F<int32_t>(o, 0x38) * 12;
        void* p = Malloc(n);
        F<void*>(o, 0x34) = p;
        StreamRead(s, p, n, 0, 1);
    }
    F<int32_t>(o, 0x5bd) = 0;
    Obj_AllocEntries(o);
    StreamSkip4(s, 0x4e24, 0, 0);
    return 0;
}
RT3_REPLACE(0x47d8e0, Obj_LoadArrays);

} // namespace rt3
